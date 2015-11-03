/*===============================================================

	ストーリーモードタスク

=================================================================*/

#include "stdafx.h"

#include "global.h"
#include "task_storymode.h"
#include "task_win.h"
#include "task_battle.h"
#include "task_staffroll.h"
#include "task_demo.h"


/*===============================================================

	ストーリーモード本体

=================================================================*/

extern int story_selectflag;		// 暫定設置、今の置き場はscript_story.cpp。

/*----------------------------------------------------------------
	構築
------------------------------------------------------------------*/
CTaskStoryMode::CTaskStoryMode()
{
	m_setting = NULL;
}

/*----------------------------------------------------------------
	削除
------------------------------------------------------------------*/
CTaskStoryMode::~CTaskStoryMode()
{
	SAFEDELETE(m_setting);
	CGoluahStoryScript::DestroyScriptElementList(scr_list);
}

/*----------------------------------------------------------------
	開始情報設定
------------------------------------------------------------------*/
void CTaskStoryMode::Setup(UINT storyIndex,UINT keyindex,CStoryList::CStoryInfo* info)
{
	SAFEDELETE(m_setting);
	m_setting = info->Clone();

	for(int i=0;i<m_setting->cnum;i++)
	{
		//排他・依存オプションで、不正になっているものを正す
		g_charlist.CorrectOption( m_setting->characters[i],&m_setting->option[i] );
	}

	m_story_index = storyIndex;
	m_keyindex = keyindex;
	in_select = false;
}

/*----------------------------------------------------------------
	初期化
------------------------------------------------------------------*/
void CTaskStoryMode::Initialize()
{
	//スクリプトの読み込み(entry.txtは自動でくっつくらしい)
//	char *filename = new char[MAX_PATH];
//	sprintf(filename,"%s\\entry.txt",g_storylist.GetStoryDir(m_story_index));
	BOOL ret = CGoluahStoryScript::CreateScriptElementList(
											scr_list,
											g_storylist.GetStoryDir(m_story_index));//filename);
//	delete [] filename;
	if(!ret){
		g_system.ReturnTitle();
		return;
	}

	//vsブロックで、キャラクターのオプションの不正を直す
	CStoryElement_VS	*ele_vs;
	SScriptElementList::iterator i =scr_list.begin();
	SScriptElementList::iterator ie=scr_list.begin();
	for(;i!=ie;i++){
		ele_vs = dynamic_cast<CStoryElement_VS*>( *i );
		if(ele_vs){
			for(int t=0;t<2;t++){
				for(int j=0;j<ele_vs->num[t];j++){
					if(ele_vs->characters[t][j]>=0 && !ele_vs->is_random_opt[t][j])
					{
						g_charlist.CorrectOption(
										ele_vs->characters[t][j],
										&ele_vs->option[t][j]);
					}
				}
			}
		}
	}

	scr_i = scr_list.begin();

	//コンティニュー回数を拾う
	CStoryElement_Settings* pset=dynamic_cast<CStoryElement_Settings*>(*scr_i);
	if(!pset){
		g_system.ReturnTitle();
		return;
	}
	m_continue_num = pset->continue_num;

	//settingブロックを飛ばす
	while(dynamic_cast<CStoryElement_Settings*>(*scr_i))scr_i++;

	//ランダムキャラクター・ステージ選択クラスを初期化
	for(UINT n=0;n<MAXNUM_TEAM;n++)
	{
		m_RandomCharacterGetter.chars[n] = m_setting->characters[n];
	}
	m_RandomCharacterGetter.Initialize(scr_list);
	m_RandomStageGetter.Initialize(scr_list);

	m_prev_task_id = 'strt';
}

/*----------------------------------------------------------------
	実行
------------------------------------------------------------------*/
BOOL CTaskStoryMode::Execute(DWORD time)
{
	CStoryElement_Staff	*ele_staff	= dynamic_cast<CStoryElement_Staff*>(*scr_i);
	CStoryElement_VS	*ele_vs		= dynamic_cast<CStoryElement_VS*>(*scr_i);
	CStoryElement_Demo	*ele_demo	= dynamic_cast<CStoryElement_Demo*>(*scr_i);

	if(!ele_staff && !ele_vs && !ele_demo){
		//scr_i == scr_list.end() ?
		return FALSE;
	}

	switch(m_prev_task_id)
	{
	//#vsブロックの終了
	case 'Btl':
		{
			if(!ele_vs)return FALSE;
			if(ele_vs->show_win || (g_battleresult.GetWinner()/*==1*/ && !ele_vs->nogameover)){
				//勝利画面へ
				CYouWin *t_win = new CYouWin;
				if(g_battleresult.GetWinner() && ele_vs->text_lose)
				{
					//負けテキスト設定
					t_win->SetStoryText(ele_vs->text_lose);
				}
				if(!g_battleresult.GetWinner() && ele_vs->text_win)
				{
					//勝利テキスト設定
					t_win->SetStoryText(ele_vs->text_win);
				}
				g_system.AddTask( t_win );
				return TRUE;
			}
			scr_i++;
			StartNextTask();
		}
		break;
	case 'Win':
		{
			if(!ele_vs)return FALSE;

			if (ele_vs->nogameover)	// nogameoverなら処理をパス
			{
				story_selectflag += ele_vs->sel_shift + g_battleresult.GetWinner();
			} else {
				//負けちゃってる
				if(g_battleresult.GetWinner()){
					if(m_continue_num<0){
						//無限コンティニュー
						StartContinueTask();
						return TRUE;
					}
					if(m_continue_num != 0){
						//コンティニュー
						m_continue_num--;
						StartContinueTask();
						return TRUE;
					}
					//ゲームオーバー
					CTStoryGameOver *go = new CTStoryGameOver;
					g_system.AddTask(go);
					return TRUE;
				}
			}
			//勝った
			scr_i++;
			StartNextTask();
		}
		break;
	//#demoブロックの終了
	case 'Demo':
		{
			if(!ele_demo)return FALSE;
			scr_i++;
			StartNextTask();
		}
		break;
	//#staffブロックの終了
	case 'Staf':
		{
			if(!ele_staff)return FALSE;
			scr_i++;
			StartNextTask();
		}
		break;
	//最初のブロック
	case 'strt':
		{
			StartNextTask();
		}
		break;
	//コンティニュー画面から戻ってきた
	case 'Cntn':
		{
			if(!ele_vs)return FALSE;
			StartVSTask();
		}
		break;
	//ゲームオーバー？
	default:
		g_system.ReturnTitle();
	}
	return TRUE;
}

/*----------------------------------------------------------------
	終了
------------------------------------------------------------------*/
void CTaskStoryMode::Terminate()
{
	//特にやることなし
}

/*----------------------------------------------------------------
	
------------------------------------------------------------------*/
void CTaskStoryMode::Activate(DWORD prvTaskID)
{
	m_prev_task_id = prvTaskID;
}

/*----------------------------------------------------------------
	タスク起動
------------------------------------------------------------------*/
void CTaskStoryMode::StartNextTask()
{
	// #select
	//      ↓のイテレータの仕様ってちょっと不便だよね
	while (scr_i != scr_list.end() && (*scr_i)->GetType() == CStoryScriptElement::CStoryElement_Types::GSE_ST_SELECT){
		CStoryElement_Select *ele_select	= dynamic_cast<CStoryElement_Select*>(*scr_i);

		scr_i++;	// 自分自身はもう処理したので次へ
		// 条件判断に合わない部分を飛ばす
		if (ele_select){
			if (ele_select->GetNum() == story_selectflag && !in_select)
				in_select = true;					// 入った
			else if (ele_select->GetNum() == -1)
				in_select = false;					// 抜けた
			else {
				// 次のselect文まで飛ばす
				while (scr_i != scr_list.end() && (*scr_i)->GetType() != CStoryScriptElement::CStoryElement_Types::GSE_ST_SELECT)
					scr_i++;
			}
		}
	}

	CStoryElement_Staff	*ele_staff	= dynamic_cast<CStoryElement_Staff*>(*scr_i);
	CStoryElement_VS	*ele_vs		= dynamic_cast<CStoryElement_VS*>(*scr_i);
	CStoryElement_Demo	*ele_demo	= dynamic_cast<CStoryElement_Demo*>(*scr_i);

	if(ele_staff)StartStaffTask();
	else if(ele_vs)StartVSTask();
	else if(ele_demo)StartDemoTask();
	else g_system.ReturnTitle();
}

//試合
void CTaskStoryMode::StartVSTask()
{
	CStoryElement_VS *ele_vs = dynamic_cast<CStoryElement_VS*>(*scr_i);
	if(!ele_vs){
		g_system.ReturnTitle();
		return;
	}

	int t,j,n;

	//char1-3指定を解決する
	for(t=0;t<2;t++){
		for(j=0;j<ele_vs->num[t];j++)
		{
			if(ele_vs->characters[t][j]<0)
			{
				switch(ele_vs->characters[t][j]){
				case -2:n=0;break;//char1
				case -3:n=1;break;//char2
				case -4:n=2;break;//char3
				default:n=3;
				}
				if(n<3){
					ele_vs->characters[t][j] = m_setting->characters[n];
					ele_vs->option[t][j] = m_setting->option[n];
					ele_vs->color[t][j] = m_setting->color[n];
				}
			}
		}
	}

	//ランダムの人数を数え、ランダムキャラ取得クラスに通知
	UINT rdm_cnum=0;
	for(t=0;t<2;t++){
		for(j=0;j<ele_vs->num[t];j++){
			if(ele_vs->characters[t][j]==-1){
				rdm_cnum++;
			}
		}
	}
	m_RandomCharacterGetter.Req(rdm_cnum);

	//ランダムキャラクター解決
	for(t=0;t<2;t++){
		for(j=0;j<ele_vs->num[t];j++)
		{
			if(ele_vs->characters[t][j]<0){
				ele_vs->characters[t][j] = m_RandomCharacterGetter.GetNext();
				ele_vs->option[t][j] = g_charlist.GetRandomOption( ele_vs->characters[t][j] );
			}
		}
	}

	int t2,j2,c;

	//色のauto指定を解決する
	for(t=0;t<2;t++){
		for(j=0;j<ele_vs->num[t];j++)
		{
			if(ele_vs->color[t][j]<0)
			{
				c=1;
				//すでに決定してるやつらとはかぶらないようにする
				for(t2=0;t2<2;t2++){
					for(j2=0;j2<ele_vs->num[t2];j2++)
					{
						if(t!=t2 && j!=j2){
							if(c==ele_vs->color[t2][j2] &&								//色同じ
								ele_vs->characters[t][j]==ele_vs->characters[t2][j2])	//キャラ同じ
							{
								c++;
								t2=0;
								j2=0;
							}
						}
						if(c==3)break;//だめぽ
					}
					if(c==3)break;//だめぽ
				}
				ele_vs->color[t][j]=c;
			}
		}
	}

	//ランダムステージを解決する
	if(ele_vs->stage < 0){
		ele_vs->stage = m_RandomStageGetter.GetNext();
	}

	BOOL com;
	BYTE key;

	//試合情報クラスに情報設定
	g_battleinfo.Initialize();
	for(t=0;t<2;t++){
		com = t==0 ? FALSE : TRUE;
		for(j=0;j<ele_vs->num[t];j++)
		{
			key = com ? CASSIGN_COM : m_keyindex;
			if(ele_vs->type==TAISENKEISIKI_GOCYAMAZE && j!=0){//同時対戦の場合はチーム1でもCOM扱い
				key = CASSIGN_COM;
			}
			g_battleinfo.AddCharacter(
							t,
							ele_vs->characters[t][j],
							ele_vs->color[t][j],
							key,
							ele_vs->option[t][j]);
			//comレベルを設定
			g_battleinfo.SetComLevel( t,j,ele_vs->level[t][j],ele_vs->is_related_level[t][j]);
		}
	}

	char *tmpbgmname = new char[256];
	sprintf(tmpbgmname,"%s\\%s",g_storylist.GetStoryDir(m_story_index),ele_vs->bgm_filename);
	g_battleinfo.SetStoryBGM( tmpbgmname );				//BGM設定
	delete [] tmpbgmname;

	g_battleinfo.SetBattleType( ele_vs->type );			//試合形式
	g_battleinfo.SetLimitTime( ele_vs->limit_time );	//制限時間
	g_battleinfo.SetStage( ele_vs->stage );				//ステージ

	//試合タスク起動
	g_system.AddTask( new CBattleTask );
}

//デモ
void CTaskStoryMode::StartDemoTask()
{
	CStoryElement_Demo	*ele_demo	= dynamic_cast<CStoryElement_Demo*>(*scr_i);
	if(!ele_demo){
		g_system.ReturnTitle();
		return;
	}

	CTDemo *tdemo = new CTDemo;
	tdemo->SetKeyIndex(m_keyindex);
	tdemo->Setup(m_story_index,ele_demo->m_filename);
	g_system.AddTask( tdemo );
}

//スタッフロール
void CTaskStoryMode::StartStaffTask()
{
	CStoryElement_Staff	*ele_staff	= dynamic_cast<CStoryElement_Staff*>(*scr_i);

	CTStaffRoll *tstaff = new CTStaffRoll;
	tstaff->SetKeyIndex(m_keyindex);
	tstaff->Setup( g_storylist.GetStoryDir(m_story_index) , ele_staff );
	g_system.AddTask( tstaff );
}

//コンティニュー
void CTaskStoryMode::StartContinueTask()
{
	CTStoryContinue *con = new CTStoryContinue;
	con->remain_cont = m_continue_num;
	g_system.AddTask(con);
}


/*===============================================================

	ランダム キャラクター・ステージ 取得クラス

=================================================================*/

/*----------------------------------------------------------------
	ランダムキャラクタークラス
------------------------------------------------------------------*/
void CTaskStoryMode::CRandomCharacterGetter::Initialize(SScriptElementList& scr_list)
{
	//全キャラクターを取り込み
	UINT rseed = rand()%g_charlist.GetCharacterCount();//インチキランダムシード
	UINT n = g_charlist.GetCharacterCount();
	UINT nn = g_charlist.GetCharacterCount();
	while(n!=0){
		n--;

		BOOL in=TRUE;
		for(int k=0;k<MAXNUM_TEAM;k++)
		{
			if(chars[k]==(n+rseed)%nn)in=FALSE;
		}
		if(in)all_list.push_back((n+rseed)%nn);
	}

	int t,j;

	gbl.ods("★★★ ランダムキャラクターall(%d) ★★★",all_list.size());

	//ストーリースクリプトで明示的に指定されているキャラクターはリストから除く
	CStoryElement_VS *ele_vs;
	CStoryElement_Settings *ele_set;
	SScriptElementList::iterator i,ie;
	i =scr_list.begin();
	ie=scr_list.end();
	for(;i!=ie;i++){
		ele_vs = dynamic_cast<CStoryElement_VS*>( *i );
		if(ele_vs){
			for(t=0;t<2;t++){
				for(j=0;j<ele_vs->num[t];j++){
					if(ele_vs->characters[t][j]>=0){
						EraseFromAllList( ele_vs->characters[t][j] );
					}
				}
			}
		}
		ele_set = dynamic_cast<CStoryElement_Settings*>( *i );
		if(ele_set){
			for(t=0;t<ele_set->cnum;t++){
				if(ele_set->characters[t]>=0){
					EraseFromAllList( ele_set->characters[t] );
				}
			}
		}
	}
	gbl.ods("★★★ ランダムキャラクターall(%d) ★★★",all_list.size());

	//現在リストを生成（サイズが0なので必ず取り込み発生）
	Req(1);
}

//次の試合で,指定した数のランダムキャラクターが必要。
//足りない場合は、全体リストを取り込んでシャッフルしなおす
void CTaskStoryMode::CRandomCharacterGetter::Req(UINT num)
{
	if(crnt_list.size() > num)return;

	crnt_list.clear();

	if(all_list.size() < num)//だめぽ
	{
		//全体リストを食っても足りない
		//仕方がないのでとりあえず全キャラクターを取り込み
		gbl.ods("★★★ ランダムキャラクター不足しすぎ(%d/%d) ★★★",all_list.size(),num);
		UINT n = g_charlist.GetCharacterCount();
		while(n!=0){
			n--;
			all_list.push_back(n);
		}
	}

	//全体リストを取り込み
	CIndexList::iterator i,ie;
	i = all_list.begin();
	ie= all_list.end();
	while(i!=ie){
		crnt_list.push_back(*i);
		i++;
	}

	//シャッフル
	srand(timeGetTime());
	UINT tempra;
	UINT tempra1;
	UINT tempra2;
	for(UINT n=0;n<crnt_list.size();n++)
	{
		tempra1 = rand()%crnt_list.size();
		tempra2 = rand()%crnt_list.size();

		tempra = crnt_list[tempra1];
		crnt_list[tempra1] = crnt_list[tempra2];
		crnt_list[tempra2] = tempra;
	}

	gbl.ods("★★★ ランダムキャラクターリストリセット(%d) ★★★",crnt_list.size());
}

//次のランダムキャラクターを取得する
UINT CTaskStoryMode::CRandomCharacterGetter::GetNext()
{
	if(crnt_list.size()==0){
		g_system.Log("CTaskStoryMode::CRandomCharacterGetter::GetNext() , list size zero\n",SYSLOG_WARNING);
		Req(1);
	}

	CIndexList::iterator nxt = crnt_list.begin();
	UINT ret = *nxt;
	crnt_list.erase(nxt);

	gbl.ods("CTaskStoryMode::CRandomCharacterGetter::GetNext() - %d",ret);

	return ret;
}

//指定されたキャラクターを全体リストから除外する
void CTaskStoryMode::CRandomCharacterGetter::EraseFromAllList(UINT n)
{
	CIndexList::iterator i =all_list.begin();
	CIndexList::iterator ie=all_list.end();
	for(;i!=ie;i++){
		if( *i==n ){
			all_list.erase(i);
			return;
		}
	}
}


/*----------------------------------------------------------------
	ランダムステージクラス
------------------------------------------------------------------*/
void CTaskStoryMode::CRandomStageGetter::Initialize(SScriptElementList& scr_list)
{
	//全ステージを取り込み
	UINT nrand = rand()%g_stagelist.GetStageCount();
	UINT n = g_stagelist.GetStageCount();
	UINT nn = g_stagelist.GetStageCount();
	while(n!=0){
		n--;
		all_list.push_back((n+nrand)%nn);
	}

	//ストーリースクリプトで明示的に指定されているステージはリストから除く
	CStoryElement_VS *ele_vs;
	SScriptElementList::iterator i,ie;
	i =scr_list.begin();
	ie=scr_list.end();
	for(;i!=ie;i++){
		ele_vs = dynamic_cast<CStoryElement_VS*>( *i );
		if(ele_vs){
			if(ele_vs->stage>=0)EraseFromAllList( ele_vs->stage );
		}
	}

	//現在リストを生成（サイズが0なので必ず取り込み発生）
	Restore();
}

//全体リストを、現在リストに取り込む
void CTaskStoryMode::CRandomStageGetter::Restore()
{
	SIndexList::iterator i,ie;
	i = all_list.begin();
	ie= all_list.end();
	while(i!=ie){
		crnt_list.push_back(*i);
		i++;
	}

	//シャッフル
	srand(timeGetTime());
	UINT tempra;
	UINT tempra1;
	UINT tempra2;
	for(UINT n=0;n<crnt_list.size();n++)
	{
		tempra1 = rand()%crnt_list.size();
		tempra2 = rand()%crnt_list.size();

		tempra = crnt_list[tempra1];
		crnt_list[tempra1] = crnt_list[tempra2];
		crnt_list[tempra2] = tempra;
	}
	
	gbl.ods("★★★ ランダムステージリストリセット(%d) ★★★",crnt_list.size());
}

//次のランダムステージを取得する
UINT CTaskStoryMode::CRandomStageGetter::GetNext()
{
	if(crnt_list.size()==0){
		Restore();
	}

	SIndexList::iterator nxt = crnt_list.begin();
	UINT ret = *nxt;
	crnt_list.erase(nxt);

	return ret;
}

//指定されたステージを全体リストから除外する
void CTaskStoryMode::CRandomStageGetter::EraseFromAllList(UINT n)
{
	SIndexList::iterator i =all_list.begin();
	SIndexList::iterator ie=all_list.end();
	for(;i!=ie;i++){
		if( *i==n ){
			all_list.erase(i);
			return;
		}
	}
}



/*===============================================================

	コンティニュータスク

=================================================================*/

void CTStoryContinue::Initialize()
{
	m_texfb = g_draw.GetFrontBufferCopy();
	m_time = 0;
	m_yes_selected = TRUE;
}

BOOL CTStoryContinue::Execute(DWORD time)
{
	m_time+=1.0f;

	if(m_time>30.0f)
	{
		if(g_input.GetAllKey() & KEYSTA_ANYKEY)
		{
			if(m_yes_selected)
				g_system.AddTask( new CBattleTask );//前の設定のまま再戦
			else
				g_system.ReturnTitle();
		}
		else if((g_input.GetAllKey() & KEYSTA_UP2)||(g_input.GetAllKey() & KEYSTA_DOWN2))
		{
			m_yes_selected = !m_yes_selected ;
		}
	}
	return TRUE;
}

void CTStoryContinue::Terminate()
{
	RELEASE(m_texfb);
}

void CTStoryContinue::Draw()
{
	g_draw.SetTransform(FALSE);

	float t = 1.0f - m_time/30.0f;
	if(t<0.2f)t = 0.2f;

	DrawFBC( t );

	t = m_time/30.0f;
	if(t>1.0f)t = 1.0f;

	DWORD c = (DWORD)(t*255.0f);
	DWORD col = 0xFF000000 | (c<<16) | (c<<8) | c;

	char *tmpstr = new char[256];
	sprintf(tmpstr,"continue? (%d)",remain_cont);
	g_system.DrawBMPText(50,230,0.0f,tmpstr,col);
	delete [] tmpstr;

	c/=2;
	col = 0xFF000000 | (c<<16) | (c<<8) | c;
	g_system.DrawBMPText(350,280,0.0f,"Yes",col);
	g_system.DrawBMPText(350,310,0.0f,"No",col);
	
	c*=2;
	col = 0xFF000000 | (c<<16) | (c<<8) | c;
	if(m_yes_selected)
		g_system.DrawBMPText(348,278,0.0f,"Yes",col);
	else
		g_system.DrawBMPText(348,308,0.0f,"No",col);
}

void CTStoryContinue::DrawFBC(float t)
{
	DWORD c = (DWORD)(t*255.0f);

	MYVERTEX3D* vb;

	if ( !g_draw.pMyVertex || FAILED(g_draw.pMyVertex->Lock(0, 0, (void**)&vb, D3DLOCK_DISCARD)) )
		return;

	vb[0].z = 0.0f;
	vb[1].z = 0.0f;
	vb[2].z = 0.0f;
	vb[3].z = 0.0f;

	vb[0].tu = 0.0f;
	vb[1].tu = 0.0f;
	vb[2].tu = 1.0f;
	vb[3].tu = 1.0f;
	
	vb[0].tv = 0.0f;
	vb[1].tv = 1.0f;
	vb[2].tv = 0.0f;
	vb[3].tv = 1.0f;
	
	vb[0].color = 
	vb[1].color = 
	vb[2].color = 
	vb[3].color = 0xFF000000 | (c<<16) | (c<<8) | c;

	vb[0].x =  0.0f;
	vb[1].x =  0.0f;
	vb[2].x =  2.0f*320.0f/240.0f;
	vb[3].x =  2.0f*320.0f/240.0f;

	vb[0].y =  0.0f;
	vb[1].y =  2.0f;
	vb[2].y =  0.0f;
	vb[3].y =  2.0f;

	if (g_draw.pMyVertex) g_draw.pMyVertex->Unlock();

	D3DXMATRIX mati;
	D3DXMatrixIdentity(&mati);
	g_draw.EnableZ(FALSE,FALSE);
	g_draw.SetTransform(FALSE);
	g_draw.d3ddev->SetStreamSource(0, g_draw.pMyVertex, 0, sizeof(MYVERTEX3D));
	g_draw.d3ddev->SetFVF(FVF_3DVERTEX);
	g_draw.d3ddev->SetTexture(0,m_texfb);
	g_draw.d3ddev->SetTransform(D3DTS_WORLD,&mati);
	g_draw.d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
}

/*===============================================================

	ゲームオーバータスク

=================================================================*/

BOOL CTStoryGameOver::Execute(DWORD time)
{
	m_time += 1.0f;

	if((g_input.GetAllKey() & KEYSTA_ANYKEY) && m_time>60.0f){
		g_system.ReturnTitle();
	}
	return TRUE;
}

void CTStoryGameOver::Draw()
{
	g_draw.SetTransform(FALSE);

	float t = 1.0f - m_time/200.0f;
	if(t<0.1f)t=0.1f;
	DrawFBC(t);

	if(m_time>20.0f)
	{
		t = (m_time-20.0f)/60.0f;
		if(t>1.0f)t=1.0f;
		DWORD c = (DWORD)(t*255.0f);
		DWORD col = 0xFF000000 | (c<<16) | (c<<8) | c;
		g_system.DrawBMPText(50,230,0.0f,"game over!",col);
	}
}

