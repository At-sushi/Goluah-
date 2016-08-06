
/*=======================================================================================

	vsCOM または 1Pvs2P でのキャラセレ

=========================================================================================*/

#include "define_macro.h"
#include "define_const.h"
#include "global.h"
#include "task_select.h"
#include "task_battle.h"
#include "task_simpleoptsel.h"

/*=======================================================================================

	キャラセレクラス本体

=========================================================================================*/

//各サブタスクの描画プライオリティ
int CCharacterSelect::m_pri_background	= 2000;
int CCharacterSelect::m_pri_face		= 1500;
int CCharacterSelect::m_pri_firstSetting= 1060;
int CCharacterSelect::m_pri_belt		= 1050;
int CCharacterSelect::m_pri_cselecter	= 1040;
int CCharacterSelect::m_pri_oselecter	= 1020;
int CCharacterSelect::m_pri_sselecter	= 1010;

/*-----------------------------------------------------------
	構築
-------------------------------------------------------------*/
CCharacterSelect::CCharacterSelect()
{
	//特にやること無し
}

/*----------------------------------------------------------
	初期化
------------------------------------------------------------*/
void CCharacterSelect::Initialize()
{
	//パラメータリセット
	charsel_ok[0]=charsel_ok[1]=FALSE;
	optsel_ok[0] = optsel_ok[1] = TRUE;
	stgsel_ok = FALSE;
	num_selected[0]=num_selected[1]=0;
	m_condition_sel_ok = FALSE;
	for(int i=0;i<2;i++){
		for(int j=0;j<MAXNUM_TEAM;j++){
			selected_color[i][j] = 1;
			selected_option[i][j] = 0;
		}
	}

	//BGM再生
	g_sound.BGMPlay(_T(".\\system\\bgm\\choice"));

	//■コンディションセレクター登録
	CTConditionSelecter *csel = new CTConditionSelecter;
	csel->SetPriority( m_pri_firstSetting );
	g_system.AddTask(csel);

	//■拝啓描画クラス登録
	m_bg = new CTCharacterSelectBG();
	m_bg->SetPriority(m_pri_background);
	g_system.AddTask(m_bg);
}


/*----------------------------------------------------------
	破棄
------------------------------------------------------------*/
void CCharacterSelect::Terminate()
{
	g_sound.BGMStop();
}

/*-----------------------------------------------------------
	更新処理
-------------------------------------------------------------*/
BOOL CCharacterSelect::Execute(DWORD time)
{
	//■step1
	if(!m_condition_sel_ok)return TRUE;//試合設定がまだ終わっていない

	//■step2
	if(!charsel_ok[0] || !charsel_ok[1]){
		//デカface更新
		if (optsel_ok[0])	//ランセレ時にもm_ring[0]->GetSelected()の絵で更新されてしまうのを防止する
			m_bface[0]->SetTemporary(m_ring[0]->GetSelected(),selected_color[0][num_selected[0]],OPT2ALT(selected_option[0][num_selected[0]]));
		if (optsel_ok[1])
			m_bface[1]->SetTemporary(m_ring[1]->GetSelected(), selected_color[1][num_selected[1]], OPT2ALT(selected_option[1][num_selected[1]]));
		//オビ表示更新
		m_belt[0]->SetRing(m_ring[0]->GetRing());
		m_belt[1]->SetRing(m_ring[1]->GetRing());
		return TRUE;
	}

	//■step3
	if(stgsel_ok)//ステージ選択後の演出スキップ判定
	{
		DWORD keystate = 0;

		//試合に参加する全てのプレイヤーのキー状態を足す
		int ass_num=0;
		for(int i=0;i<2;i++){
			for(int j=0;j<MAXNUM_TEAM;j++){
				if(!(m_assign[i][j]&CASSIGN_SPECIFIC)){
					keystate |= g_input.GetKey(m_assign[i][j],0);
					ass_num++;
				}
			}
		}
		if(!ass_num){//全部コンピュータならばしょうがないからプレイヤー1の入力を受け取る
			keystate = g_input.GetKey(0,0);
		}

		//ボタンが押されていたら次に進んじゃう
		if(keystate & KEYSTA_BUTTONS){
			ResolveRandom();
			SetupBattleInfo();
			CreateNextTask();
		}
	}

	return TRUE;
}

void CCharacterSelect::CreateNextTask()
{
	g_system.AddTask( new CBattleTask );
}

/*-----------------------------------------------------------
	試合コンディション選択時処理
-------------------------------------------------------------*/
void CCharacterSelect::OnConditionDecided(CTConditionSelecter *pcsel)
{
	m_condition_sel_ok = TRUE;
	int i,j;

	//割り当て情報をコピー
	memcpy(m_assign , pcsel->m_assign , sizeof(BYTE)*MAXNUM_TEAM*2);
	//人数カウント
	for(i=0;i<2;i++){
		wanted_char_num[i] = 0;
		for(j=0;j<MAXNUM_TEAM;j++){
			if(m_assign[i][j]!=CASSIGN_NONE){
				wanted_char_num[i]++;
			}
		}
	}
	//ソート（「割り当てなし」を、後ろのほうに移動）
	for(i=0;i<2;i++){
		for(j=0;j<MAXNUM_TEAM-1;j++){
			if(m_assign[i][j]==CASSIGN_NONE)
			{
				for(int k=j+1;k<MAXNUM_TEAM;k++)//jよりも後で、割り当てのあるものを探す
				{
					if(m_assign[i][k]!=CASSIGN_NONE){//ハケーン
						//交換
						m_assign[i][j] = m_assign[i][k];
						m_assign[i][k] = CASSIGN_NONE;
						break;
					}
				}
			}
		}
	}

	g_battleinfo.Initialize();					//対戦情報初期化
	g_battleinfo.SetBattleType(pcsel->m_type);	//対戦形式を設定
	g_battleinfo.SetLimitTime(
		pcsel->m_limit_time[pcsel->m_limit_time_index]);//制限時間を設定

	for(i=0;i<2;i++)
	{
		//■キャラクターリングクラス登録
		m_ring[i] = new CTCharacterRing();
		m_ring[i]->SetPriority( m_pri_cselecter );
		m_ring[i]->SetPos(320.0,i==0?160.0:320.0);
		m_ring[i]->SetHeight(i==0?40.0f:22.0f);
		g_system.AddTask(m_ring[i]);
		//■キャラクターリング下・オビ
		m_belt[i] = new CTCharacterSelectBelt;
		i==0 ? m_belt[i]->SetLeft() : m_belt[i]->SetRight();
		m_belt[i]->SetPriority( m_pri_belt );
		m_belt[i]->SetBaseY(i==0 ? 160.0f : 320.0f);
		g_system.AddTask(m_belt[i]);
		//■デカ顔表示クラス登録
		m_bface[i] = new CTCharacterBigFace();
		m_bface[i]->SetPriority( m_pri_face );
		m_bface[i]->SetLeftRight(i==0 ? TRUE : FALSE);
		m_bface[i]->SetNum(wanted_char_num[i]);
		g_system.AddTask( m_bface[i] );
		//■オプションセレクタークラス
		m_selecter[i] = new CTSimpleOptionSelecter;
		m_selecter[i]->SetPriority( m_pri_oselecter );
		g_system.AddTask(m_selecter[i]);
	}
	
	//キャラ選択キー入力割り当て
	AssignKeys(0);
	AssignKeys(1);
}

/*-----------------------------------------------------------
	キャラクタ選択キー入力割り当て
	+オビクラスにCOM,キー入力番号の通知
-------------------------------------------------------------*/
void CCharacterSelect::AssignKeys(int team)
{
	int team2 = AnotherTeam(team);

	//次に選択するのはコンピュータ
	if(m_assign[team][num_selected[team]]==CASSIGN_COM||m_assign[team][num_selected[team]]==CASSIGN_STABLE)
	{
		m_belt[team]->SetCom(TRUE);
		if(CkAllCom(team))//チームが全てCOMだったら、
		{
			if(CkAllCom(team2))//もう片方のチームも全員COM
			{
				if(team==0 || charsel_ok[team2]){	//チーム1 または もう片方のチームが選択終了済み
					m_ring[team]->SetKeyInputIndex(0);
					m_belt[team]->SetKey(0);
					return;
				}
			}

			if(charsel_ok[team2]){//もう片方のチームは選択終了済み
				for(int i=0;i<MAXNUM_KEYI;i++)
				{
					//もう片方のチームに存在するキー入力番号を割り当て
					if(CkKey(team2,i))
					{
						m_ring[team]->SetKeyInputIndex(i);
						m_belt[team]->SetKey(i);
						return;
					}
				}
			}

			//もう片方のチームの選択が終わるまではロック
			m_ring[team]->SetKeyInputIndex(-1);
			m_belt[team]->SetKey(-1);
			return;
		}
		else
		{
			for(int i=0;i<MAXNUM_KEYI;i++)
			{
				//そのチームに存在するキー入力番号を割り当て
				if(CkKey(team,i))
				{
					m_ring[team]->SetKeyInputIndex(i);
					m_belt[team]->SetKey(i);
					return;
				}
			}
		}
	}
	//次に選択するのはプレイヤー
	else{
		m_belt[team]->SetCom(FALSE);
		m_ring[team]->SetKeyInputIndex( m_assign[team][num_selected[team]] );
		m_belt[team]->SetKey( m_assign[team][num_selected[team]] );
		return;
	}
}

//指定チームに指定キー入力番号のプレイヤーが割り当てられているかどうか
BOOL CCharacterSelect::CkKey(int team,BYTE ki)
{
	for(int i=0;i<MAXNUM_TEAM;i++){
		if(m_assign[team][i]==ki){
			return TRUE;
		}
	}
	return FALSE;
}

//指定チームの残り選択が、コンピュータのみかどうか
BOOL CCharacterSelect::CkAllCom(int team)
{
	for(int i=0;i<wanted_char_num[team];i++){
		if(m_assign[team][i]!=CASSIGN_COM&&m_assign[team][i]!=CASSIGN_STABLE){
			return FALSE;
		}
	}
	return TRUE;
}

void CCharacterSelect::OnCancel()
{
/*	if (charsel_ok[0]){
		num_selected[1]--;
	}
*/	for (int i = 0; i < 2; i++)
	{
		m_ring[i]->Hide();
		m_belt[i]->Show(FALSE);
		m_bface[i]->SetPriority(0);
		m_bface[i]->ResetNum();
//		g_system.RemoveTask(m_selecter[i]->GetID());	// 今のところ効果無し
		for (int j = 0; j < MAXNUM_TEAM; j++)
		{
			m_bface[i]->Set(j, 0, 0);
			m_bface[i]->SetTemporary(j, 0, 0);
		}
	}
	Initialize();
}

/*-----------------------------------------------------------
	キャラクタ選択時処理
-------------------------------------------------------------*/
void CCharacterSelect::OnSelect(CTCharacterRing *pring,int cindex)
{
	//どのリングから？
	DWORD team = 3;
	if(pring==m_ring[0])	team = 0;
	if(pring==m_ring[1])	team = 1;
	if(team>1)return;

	// ランダム選択処理
	if (cindex < 0){
		BOOL jyufuku;	//別チームの重複
		BOOL jyufuku2;	//自チームの重複
		int count;

		jyufuku = TRUE;
		count = 100;
		while ((jyufuku && count>0) || jyufuku2)
		{
			cindex = rand() % g_charlist.GetCharacterCount();
			jyufuku = FALSE;
			jyufuku2 = FALSE;
			for (int k = 0; k<2; k++){//重複チェック
				for (int l = 0; l<wanted_char_num[team]; l++){
					if (!(k == team && l == num_selected[team])){
						if (cindex == selected_char[k][l]){
							jyufuku = TRUE;
							if (team == k)jyufuku2 = TRUE;
						}
					}
				}
			}
			count--;//countが0になったら重複しててもそのままいっちゃう
		}//while
		selected_color[team][num_selected[team]] = rand() % MAXNUM_CHARACTERCOLOR + 1;
		ResolveColor(team, num_selected[team]);//カラーの重複をチェック
		optsel_ok[team] = FALSE;	//デカ顔が更新されるのを防止する
		m_bface[team]->SetTemporary(cindex, selected_color[team][num_selected[team]], 0);
	}

	selected_char[team][num_selected[team]] = cindex;

	if(cindex>=0){
		m_ring[team]->Hide();
		/* 同じチーム内での同キャラ選択に対応しようとして保留中。カラーが問題になる。
		if (g_charlist.GetCharacterVer(cindex) < 1000 &&
			g_charlist.GetCharacterVer(cindex) >= 680)*/
			m_ring[team]->AddSelected(cindex);		// バージョン0.6x～0.8xのキャラは1チーム複数参加に対応してない
		ResolveColor(team,num_selected[team]);
		int optnum = m_selecter[team]->SetAndShow(cindex, pring->GetKeyInputIndex() );
		//オプションセレクターの高さ計算・位置指定
		if(optnum<1)optnum=1;
		float optheight = m_selecter[team]->GetHeight() + 15.0f;//10:まーじん
		float base_y = team==0 ? 160.0f : 320.0f;
		float top_y = base_y - optheight*( team==0 ? 0.65f : 0.25f );
		float btm_y = base_y + optheight*( team==0 ? 0.25f : 0.65f );
		if(top_y < 0.0f)top_y = 5.0f;
		if(btm_y > 480.0f)btm_y = 475.0f;
		m_selecter[team]->SetPos( team==0 ? 50.0f : 235.0f , top_y+5.0f );
		m_belt[team]->Extend( top_y , btm_y );
		return;
	}

	//以下ランダムキャラ処理
/*	m_bface[team]->Set(cindex, selected_color[team][num_selected[team]], 0);
	num_selected[team]++;

	if(num_selected[team]==wanted_char_num[team])//チームメンバーがそろった
	{
		charsel_ok[team] = TRUE;
		m_ring[team]->Hide();
		m_belt[team]->Show(FALSE);
		CheckCharacterSelectOK();
	}
	else{
		AssignKeys(team);//キー再割り当て
	}*/
}


/*-----------------------------------------------------------
	オプション選択時処理
-------------------------------------------------------------*/
void CCharacterSelect::OnOptionSelect(CTOptionSelecter *pselecter,DWORD option)
{
	if (pselecter->State() == 0xFFFFFF)
		return;

	//team？
	DWORD team = 3;
	if(pselecter==m_selecter[0] || pselecter==m_selecter[0]->GetCustomSelecter())team = 0;
	if(pselecter==m_selecter[1] || pselecter==m_selecter[1]->GetCustomSelecter())team = 1;
	if(team>1)return;
	
	selected_option[team][num_selected[team]] = option;
	m_bface[team]->Set(selected_char[team][num_selected[team]],selected_color[team][num_selected[team]],OPT2ALT(option));//デカface更新
	num_selected[team]++;
	optsel_ok[team] = TRUE;	//デカ顔の更新を許可する

	if(!(m_assign[team][num_selected[team]-1] & CASSIGN_SPECIFIC)){
		pselecter->ApplyToPreviousSelect();
	}

	if(num_selected[team]==wanted_char_num[team])//チームメンバーがそろった
	{
		charsel_ok[team] = TRUE;
		m_belt[team]->Show(FALSE);
		CheckCharacterSelectOK();
	}
	else//続行
	{
		AssignKeys(team);//キー再割り当て
		m_ring[team]->Restore();
		m_belt[team]->Restore();
	}
}

/*-----------------------------------------------------------
	色変更時処理
-------------------------------------------------------------*/
void CCharacterSelect::OnChangeColor(CTCharacterRing *pring)
{
	//どのリングから？
	DWORD team = 3;
	if(pring==m_ring[0])	team = 0;
	if(pring==m_ring[1])	team = 1;
	if(team>1)return;

	selected_color[team][num_selected[team]]++;
	if(selected_color[team][num_selected[team]]>3)selected_color[team][num_selected[team]]=1;
}


/*-----------------------------------------------------------
	インストON/OFF時処理
-------------------------------------------------------------*/
void CCharacterSelect::OnInstOnOff(CTCharacterRing *pring)
{
	//どのリングから？
	DWORD team = 3;
	if(pring==m_ring[0])	team = 0;
	if(pring==m_ring[2])	team = 1;
	if(team>1)return;
}


/*-----------------------------------------------------------
	キャラクター選択・オプション選択イベント後に
	ステージ選択に進むかどうかのチェック
-------------------------------------------------------------*/
void CCharacterSelect::CheckCharacterSelectOK()
{
/*	if(g_battleinfo.GetCharacterIsCOM(1,0)){//1PvsCOMの場合
		//リング1を無効にしてリング2の入力を有効にする
		if(charsel_ok[0]){
			m_ring[0]->SetKeyInputIndex(-1);
			m_ring[1]->SetKeyInputIndex(0);
		}
	}*/

	//ステージセレクトの開始
	if(charsel_ok[0] && charsel_ok[1]){
		CTStageSelecter *sselect = new CTStageSelecter;
		sselect->SetPriority( m_pri_sselecter );
		//キー割り当て
		int ass_num=0;
		for(int i=0;i<2;i++){
			for(int j=0;j<MAXNUM_TEAM;j++){
				if(!(m_assign[i][j]&CASSIGN_SPECIFIC)){
					sselect->AddKeyInput(m_assign[i][j]);
					ass_num++;
				}
			}
		}
		if(!ass_num){
			sselect->AddKeyInput(0);
		}

		g_system.AddTask( sselect );
		m_bg->ChangeState(TRUE);
	}

	//キャラセレ・キー再割り当て
	if(charsel_ok[0]){
		AssignKeys(1);
	}
	else if(charsel_ok[1]){
		AssignKeys(0);
	}
}


/*-----------------------------------------------------------
	ステージ選択時処理
-------------------------------------------------------------*/
void CCharacterSelect::OnStageSelect(CTStageSelecter *spelecter,int sindex)
{
	selected_stage = sindex;
	stgsel_ok = TRUE;
}


/*-----------------------------------------------------------
	終了アニメーションカン了通知
-------------------------------------------------------------*/
void CCharacterSelect::OnEndAnimComplete(CTaskBase* ptask)
{
	ResolveRandom();
	SetupBattleInfo();

	CreateNextTask();
	return;
}

/*-----------------------------------------------------------
	描画
-------------------------------------------------------------*/
void CCharacterSelect::Draw()
{
	g_draw.SetTransform(FALSE);
}


/*-----------------------------------------------------------
	終了前処理
-------------------------------------------------------------*/
void CCharacterSelect::SetupBattleInfo()
{
	//キャラクタ設定
	for(int i=0;i<2;i++){
		for(int j=0;j<wanted_char_num[i];j++){
			g_battleinfo.AddCharacter(i,
				selected_char[i][j],
				selected_color[i][j],
				m_assign[i][j],
				selected_option[i][j]);
		}
	}

	//ステージ設定
	g_battleinfo.SetStage( selected_stage );
}


/*-----------------------------------------------------------
	ランダム指定されたキャラクター・ステージに
	有効なインデックスを割り当てる
-------------------------------------------------------------*/
void CCharacterSelect::ResolveRandom()
{
	srand(timeGetTime());
	BOOL jyufuku;	//別チームの重複
	BOOL jyufuku2;	//自チームの重複
	int count;

	//キャラクター・ランダム解決
	for(int j=0;j<2;j++){
		for(int i=0;i<wanted_char_num[j];i++)
		{
			if(selected_char[j][i]<0){
				jyufuku = TRUE;
				count = 100;
				while((jyufuku && count>0) || jyufuku2)
				{
					selected_char[j][i] = rand()%g_charlist.GetCharacterCount();
					jyufuku = FALSE;
					jyufuku2 = FALSE;
					for(int k=0;k<2;k++){//重複チェック
						for(int l=0;l<wanted_char_num[j];l++){
							if(!(k==j && l==i)){
								if(selected_char[j][i] == selected_char[k][l]){
									jyufuku = TRUE;
									if(j==k)jyufuku2=TRUE;
								}
							}
						}
					}
					count--;//countが0になったら重複しててもそのままいっちゃう
				}//while
				selected_option[j][i] = g_charlist.GetRandomOption(selected_char[j][i]);
				selected_color[j][i] = rand()%MAXNUM_CHARACTERCOLOR+1;
				ResolveColor(j,i);//カラーの重複をチェック
			}
		}
	}

	if(selected_stage<0){
		selected_stage = rand()%g_stagelist.GetStageCount();
	}
}

/*-----------------------------------------------------------
	カラーが重複しないよう、適切なカラーを選択しなおす
-------------------------------------------------------------*/
void CCharacterSelect::ResolveColor(int team,int index)
{
	int team2 = team==0 ? 1 : 0;

	for(int i=0;i<num_selected[team2];i++)
	{
		if(selected_char[team][index]==selected_char[team2][i])//キャラクターが同じ
		{
			if(selected_color[team][index]==selected_color[team2][i])//色が同じ
			{
				switch(selected_color[team2][i]){
				case 1:selected_color[team][index]=2;break;
				case 2:selected_color[team][index]=1;break;
				case 3:selected_color[team][index]=1;break;
				}
				return;
			}
		}
	}
}



/*=======================================================================================

	ミニ顔リング

=========================================================================================*/

DWORD CTCharacterRing::m_instanceCount;
MYSURFACE** CTCharacterRing::m_dds_miniface;

UINT CTCharacterRing::sm_selected_ring1p	=0;
UINT CTCharacterRing::sm_selected_index1p	=0;
UINT CTCharacterRing::sm_selected_ring2p	=0;
UINT CTCharacterRing::sm_selected_index2p	=0;

/*-----------------------------------------------------------
	初期化
-------------------------------------------------------------*/
void CTCharacterRing::InitializeSub()
{
	//パラメータリセット
	m_counter = 0;
	m_state = CTCRS_NotReady;
	m_radius_base = 120;
	m_sleepCount = 0;
}

void CTCharacterRing::InitializeIcons()
{
	TCHAR *filepath = new TCHAR [MAX_PATH];

	//staticから選択されていた値復帰
	m_selected_ring = (m_instanceCount==0) ? sm_selected_ring1p : sm_selected_ring2p;
	m_selected_index = (m_instanceCount==0) ? sm_selected_index1p : sm_selected_index2p;

	//ミニ顔の読み込み
	if(m_instanceCount==0){
		m_dds_miniface = new MYSURFACE* [g_charlist.GetCharacterCount()];
		for(int i=0;i<g_charlist.GetCharacterCount();i++){
			m_dds_miniface[i]=NULL;
			_stprintf(filepath,_T("%s\\face1"),g_charlist.GetCharacterDir(i));
			m_dds_miniface[i] = g_draw.CreateSurfaceFrom256Image(filepath);
		}
	}
	delete [] filepath;
	m_instanceCount++;
}


/*-----------------------------------------------------------
	破棄
-------------------------------------------------------------*/

void CTCharacterRing::DestroyIcons()
{
	m_instanceCount--;

	//static に値を保存
	if(m_instanceCount==0)
	{
		//（たぶん）2P
		sm_selected_ring2p = m_selected_ring;
		sm_selected_index2p = m_selected_index;
	}
	else
	{
		//（たぶん）1P
		sm_selected_ring1p = m_selected_ring;
		sm_selected_index1p = m_selected_index;
	}

	if(m_instanceCount==0){
		for(int i=0;i<g_charlist.GetCharacterCount();i++){
			RELSURFACE(m_dds_miniface[i]);
		}
		delete [] m_dds_miniface;
	}

}

/*-----------------------------------------------------------
	選択されたキャラクタを取得
-------------------------------------------------------------*/
UINT CTCharacterRing::GetSelected()
{
	return g_charlist.RingIndex2SerialIndex(m_selected_ring,m_selected_index);
}


/*-----------------------------------------------------------
	描画用の色を取得
-------------------------------------------------------------*/
DWORD CTCharacterRing::GetColor(UINT ridx,UINT idx)
{
	UINT sindex = g_charlist.RingIndex2SerialIndex(ridx,idx);

	std::list<DWORD>::iterator i=m_selectedlist.begin();
	std::list<DWORD>::iterator ied=m_selectedlist.end();
	for(;i!=ied;i++){
		if(*i==sindex){//既に選択されたキャラクター
			return 0x00888888;
		}
	}
	return 0x00FFFFFF;//まだ選択されていないキャラクター
}

/*------------------------------------------------------------
	指定キャラクターが既に選択されているかどうかチェック
--------------------------------------------------------------*/
BOOL CTCharacterRing::IsSelected(DWORD cindex)
{
	std::list<DWORD>::iterator i= m_selectedlist.begin();
	std::list<DWORD>::iterator ied= m_selectedlist.end();
	for(;i!=ied;i++){
		if(*i==cindex)return TRUE;
	}
	return FALSE;
}

/*--------------------------------------------------------------------------------------
	表示するアイコン画像を取得（Drawからコール）
----------------------------------------------------------------------------------------*/
MYSURFACE* CTCharacterRing::GetIcon(UINT ridx,UINT idx)
{
	if(!m_dds_miniface)return NULL;

	UINT sindex = g_charlist.RingIndex2SerialIndex(ridx,idx);
	if((int)sindex > g_charlist.GetCharacterCount())return NULL;

	return m_dds_miniface[sindex];
}

/*--------------------------------------------------------------------------------------
	ボタンが押されたときの処理
----------------------------------------------------------------------------------------*/
void CTCharacterRing::OnButtonDown(DWORD key)
{
	//SELECTタスクゲット
	CCharacterSelectBase* ccselect = dynamic_cast<CCharacterSelectBase*>(g_system.GetCurrentMainTask());
	if(!ccselect)return;

	//決定
	if(key & KEYSTA_BA2){//「A」決定
		if(!IsSelected(GetSelected())){
			ccselect->OnSelect(this,GetSelected());
		}
	}
	else if(key & KEYSTA_BD2){//ランダム決定
		ccselect->OnSelect(this,-1);
	}
	else if(key & KEYSTA_BB2){//× on/off instruction
		//ccselect->OnInstOnOff(this);
		ccselect->OnCancel();	// 臨時(キャンセル)
	}
	else if(key & KEYSTA_BC2){//change color
		ccselect->OnChangeColor(this);
	}
}

/*--------------------------------------------------------------------------------------
	リングがまわされてインデックスが変化したときの処理
----------------------------------------------------------------------------------------*/
void CTCharacterRing::OnChangeIndex()
{
	//特にやること無し
}

/*--------------------------------------------------------------------------------------
	リングが変更されたときの処理
----------------------------------------------------------------------------------------*/
void CTCharacterRing::OnChangeRing()
{
	//特にやること無し
}

/*--------------------------------------------------------------------------------------
	リングの総数を取得する
----------------------------------------------------------------------------------------*/
UINT CTCharacterRing::GetMaxRing()
{
	return g_charlist.GetRingNum();
}

/*--------------------------------------------------------------------------------------
	指定リングが持っているアイコンの数を取得
----------------------------------------------------------------------------------------*/
UINT CTCharacterRing::GetMaxIndex(UINT ridx)
{
	return g_charlist.GetCharacterCountRing(ridx);
}

/*--------------------------------------------------------------------------------------
	描画
----------------------------------------------------------------------------------------*/
void CTCharacterRing::Draw()
{
	CTIconRingBase::Draw();

	// 選択されてるﾔｼの名前を表示
	//g_system.DrawBMPText(m_radius_base, m_height, 0, g_charlist.GetCharacterDir( GetSelected() ), 0xFF4455AA);
}


/*=======================================================================================

	デカface表示クラス

=========================================================================================*/

/*-----------------------------------------------------------
	初期化
-------------------------------------------------------------*/
void CTCharacterBigFace::Initialize()
{
	int j;
	
	//パラメータリセット
	m_counter  = 0;
	m_counter2 = 0;
	m_selected_num = 0;
	for(j=0;j<3;j++){
		m_cindex[j]=0;
		m_color[j]=0;
	}

	m_draw_temporary = TRUE;
}

/*-----------------------------------------------------------
	破棄
-------------------------------------------------------------*/
void CTCharacterBigFace::Terminate()
{
	//特にやること無し
}

/*-----------------------------------------------------------
	実行
-------------------------------------------------------------*/
BOOL CTCharacterBigFace::Execute(DWORD time)
{
	m_counter+=10;//決定時押しやられカウンタ加算
	if(m_counter>100)m_counter=100;

	m_counter2+=35;//選択変更時出現カウンタ加算

	return TRUE;
}

/*-----------------------------------------------------------
	描画
-------------------------------------------------------------*/
void CTCharacterBigFace::Draw()
{
	g_draw.EnableZ(FALSE,FALSE);

	//local val.
	int l;
	DWORD face2_maxx , face2_col;
	RECT r_face;
	MYSURFACE *pddstmp;

	//描画数決定
	int kmax;
	if(m_draw_temporary)kmax= m_selected_num==m_max_num ? m_max_num : m_selected_num+1;
	else				kmax= m_max_num;

	for(int k=0;k<kmax;k++){
		l=m_selected_num-k;//奥のほうに押しやられる数
		if(m_selected_num==m_max_num)l--;
		if(l==1)l=(int)(60*(m_counter/100.0));
		if(l==2)l=(int)(60+60*(m_counter/100.0));

		pddstmp = gbl.GetBigFace(m_cindex[k],m_color[k],m_alt[k]);
		if(pddstmp){
			face2_col = GetDispColor(k);
			
			r_face.top = 0;
			r_face.left = 0;
			r_face.right = (int)pddstmp->wg;
			r_face.bottom = (int)pddstmp->hg;

			if(k==m_selected_num){
				if(m_counter2 > pddstmp->wg)face2_maxx=(DWORD)pddstmp->wg;
				else face2_maxx = m_counter2;
			}
			else face2_maxx=(DWORD)pddstmp->wg;
			
			if(m_left){//player1
				g_draw.CheckBlt(
					pddstmp,
					face2_maxx-(DWORD)pddstmp->wg - (m_max_num-1)*50 +l,
					240-(DWORD)pddstmp->hg/2,
					r_face,
					FALSE,
					FALSE,
					0,
					0.02f + l*0.008f,
					face2_col);
			}
			else{//player2
				g_draw.CheckBlt(
					pddstmp,
					640-face2_maxx + (m_max_num-1)*50 - l,
					240-(DWORD)pddstmp->hg/2,
					r_face,
					TRUE,
					FALSE,
					0,
					0.02f + l*0.008f,
					face2_col);
			}
		}
	}

	g_draw.EnableZ();

}

DWORD CTCharacterBigFace::GetDispColor(UINT idx)
{
	return idx==m_selected_num ? 0xFFFFFFFF : 0xFFAAAAAA;
}

/*-----------------------------------------------------------
	キャラ確定
-------------------------------------------------------------*/
void CTCharacterBigFace::Set(int cid,DWORD color,int alt)
{
	if(m_selected_num==m_max_num)return;

	/*
	MYSURFACE **tgt = &m_dds[m_selected_num];
	BOOL needReload = TRUE;
	TCHAR *filepath,*palpath;

	if(*tgt!=NULL){
		if(m_cindex[m_selected_num]==cid && m_color[m_selected_num]==color && m_alt[m_selected_num]==alt){
			//同じモノが既にロードされている
			needReload = FALSE;
		}
	}
	if(cid<0){
		//ランダム指定
		if(*tgt){
			RELSURFACE(*tgt);
		}
		needReload = FALSE;
	}
	if(needReload){
		if(*tgt){
			RELSURFACE(*tgt);
		}
		filepath = new TCHAR[MAX_PATH];
		palpath = new TCHAR[MAX_PATH];
		TCHAR altstr[3]={'\0','\0'};
		if(alt!=0)altstr[0]='a'+alt-1;
		_stprintf(filepath,_T("%s\\face2%s.bmp"),g_charlist.GetCharacterDir(cid) ,altstr);
		_stprintf(palpath,_T("%s\\pal%d.bmp"),g_charlist.GetCharacterDir(cid),color);
		*tgt = g_draw.CreateSurfaceFrom256BMP(filepath,palpath);
		delete [] filepath;
		delete [] palpath;
	}*/
	m_cindex[m_selected_num] = cid ;
	m_color[m_selected_num] = color;
	m_alt[m_selected_num] = alt;
	m_selected_num ++;
	if(m_selected_num!=m_max_num)m_counter = 0;
	m_counter2 = 0;
}

/*-----------------------------------------------------------
	未選択キャラ顔設定
-------------------------------------------------------------*/
void CTCharacterBigFace::SetTemporary(int cid,DWORD color,int alt)
{
	if(m_selected_num==m_max_num)return;

	if(m_cindex[m_selected_num]==cid && m_color[m_selected_num]==color && m_alt[m_selected_num]==alt){
		//同じモノが既にロードされている・・・ハズ
		return;
	}

	if(cid<0){//ランダム指定
		m_cindex[m_selected_num] = cid ;
		m_color[m_selected_num] = color;
		m_alt[m_selected_num] = alt;
		return;
	}
/*
	MYSURFACE **tgt = &m_dds[m_selected_num];
	TCHAR *filepath,*palpath;

	if(m_cindex[m_selected_num]==cid && m_color[m_selected_num]==color && m_alt[m_selected_num]==alt){
		//同じモノが既にロードされている・・・ハズ
		return;
	}
	if(*tgt){
		RELSURFACE(*tgt);
	}
	if(cid<0){//ランダム指定
		m_cindex[m_selected_num] = cid ;
		m_color[m_selected_num] = color;
		return;
	}
	filepath = new TCHAR[MAX_PATH];
	palpath = new TCHAR[MAX_PATH];
	TCHAR altstr[3]={'\0','\0'};
	if(alt!=0)altstr[0]='a'+alt-1;
	_stprintf(filepath,_T("%s\\face2%s.bmp"),g_charlist.GetCharacterDir(cid),altstr );
	_stprintf(palpath,_T("%s\\pal%d.bmp"),g_charlist.GetCharacterDir(cid),color);
	*tgt = g_draw.CreateSurfaceFrom256BMP(filepath,palpath);
	delete [] filepath;
	delete [] palpath;
	*/
	m_cindex[m_selected_num] = cid ;
	m_color[m_selected_num] = color;
	m_alt[m_selected_num] = alt;

	m_counter2 = 0;
}

/*=======================================================================================

	背景描画

=========================================================================================*/


/*-----------------------------------------------------------
	初期化
-------------------------------------------------------------*/
void CTCharacterSelectBG::Initialize()
{
	//テクスチャロード
	D3DXCreateTextureFromFileEx(g_draw.d3ddev,
		_T("system\\texture\\tex_cs1.png"),
		0,0,
		1,0,
		g_draw.texformat,
		D3DPOOL_MANAGED,
		D3DX_FILTER_POINT,
		D3DX_DEFAULT,
		0x000000,
		NULL,
		NULL,
		&ptex_cs1);

	//頂点座標設定
	vbg[0].x = -0.3f;//-(ASPECTRATIO);
	vbg[0].y = -0.2f;//-(1.0f);
	vbg[0].z = 0.2f;
	vbg[1].x = -0.3f;//-(ASPECTRATIO);
	vbg[1].y = (1.0f)*2.2f;
	vbg[1].z = 0.2f;
	vbg[2].x = (ASPECTRATIO*2.2f);
	vbg[2].y = -0.2f;//-(1.0f);
	vbg[2].z = 0.2f;
	vbg[3].x = (ASPECTRATIO)*2.2f;
	vbg[3].y = (1.0f)*2.2f;
	vbg[3].z = 0.2f;

	//パラメータリセット
	m_state = FALSE;
	m_counter = 0;
}


/*-----------------------------------------------------------
	破棄
-------------------------------------------------------------*/
void CTCharacterSelectBG::Terminate()
{
	RELEASE(ptex_cs1);//テクスチャ後始末
}


/*-----------------------------------------------------------
	実行
-------------------------------------------------------------*/
BOOL CTCharacterSelectBG::Execute(DWORD time)
{
	if(!ptex_cs1)return FALSE;

	BYTE bgcolor;//0:キャラセレ色　255:ステージセレクト色
	double ratio;
	if(m_state){
		ratio = m_counter/60.0/4.0;
		ratio = 1.0 - ratio;
	}
	else{
		ratio = m_counter/60.0/4.0;
	}
	bgcolor = (BYTE)(255*ratio);

	//カラー設定
	vbg[0].color = 0xFF0000FF;
	vbg[0].color |= bgcolor*256*256 + bgcolor*256;
	vbg[1].color = vbg[2].color = 0xFF000000;
	vbg[1].color |= bgcolor*256*256 + bgcolor*256 + bgcolor;
	vbg[2].color |= bgcolor*256*256 + bgcolor*256 + bgcolor;
	vbg[3].color = 0xFFFF0000;
	vbg[3].color |= bgcolor*256 + bgcolor;

	//テクスチャUV設定
	float texzurasi= (float)(timeGetTime()%5000) / 5000.0f;
	vbg[0].tu = texzurasi;
	vbg[0].tv = texzurasi;
	vbg[1].tu = texzurasi;
	vbg[1].tv = texzurasi+7.5f;
	vbg[2].tu = texzurasi+5.0f;
	vbg[2].tv = texzurasi;
	vbg[3].tu = texzurasi+5.0f;
	vbg[3].tv = texzurasi+7.5f;

	if(m_counter!=0)m_counter--;
	return TRUE;
}


/*-----------------------------------------------------------
	描画
-------------------------------------------------------------*/
void CTCharacterSelectBG::Draw()
{
	g_draw.SetTransform(FALSE);
	g_draw.EnableZ(FALSE,FALSE);
	
	g_draw.d3ddev->SetTexture(0,ptex_cs1);
	g_draw.d3ddev->SetFVF(FVF_3DVERTEX);
	g_draw.d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_draw.d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,
		2,vbg,sizeof(MYVERTEX3D));
	g_draw.d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	g_draw.d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	g_draw.EnableZ();
}


/*-----------------------------------------------------------
	モード変更
-------------------------------------------------------------*/
void CTCharacterSelectBG::ChangeState(BOOL f)
{
	m_state = f;
	m_counter = 60*4 - m_counter;
}



/*=======================================================================================

	リング背景オビ表示クラス

=========================================================================================*/


/*-----------------------------------------------------------
	初期化
-------------------------------------------------------------*/
void CTCharacterSelectBelt::Initialize()
{
	m_state = CTCSBS_NotReady;
	m_ratio = 0;
	m_height_base = 116.0f;

	UpdateText();

	m_txtTop	= m_pos==0 ? TRUE : FALSE;		//テキストを上に配置するか、下に配置するか
	m_txtLeft	= 20.0f;	//テキストの左位置
	m_txtR2L	= m_pos==0 ? FALSE : TRUE;		//テキスト右→左描画

	m_alpha1	= 230;
	m_alpha2	= 20;									//α、濃いとこ・薄いとこ
	m_lineCol	= m_pos==0 ? 0x002200FF : 0x00FF0022;	//ラインのカラー（α以外）
	m_bodyCol	= 0x00FFFFFF;							//本体のカラー（α以外）
	m_txtCol1	= m_pos==0 ? 0x55220099 : 0x55990022;
	m_txtCol2	= m_pos==0 ? 0xDD5237FF : 0xDDFF3752;	//テキストカラー（α含）

	m_lineWidth = 3.0f;			//色つきライン太さ
}


/*-----------------------------------------------------------
	破棄
-------------------------------------------------------------*/
void CTCharacterSelectBelt::Terminate()
{
}


/*-----------------------------------------------------------
	実行
-------------------------------------------------------------*/
BOOL CTCharacterSelectBelt::Execute(DWORD time)
{
	float animSpd = 0.05f;

	switch(m_state){
	case CTCSBS_NotReady:
		{
			m_ratio += animSpd;
			if(m_ratio > 1.0f){
				m_ratio = 1.0f;
				m_state = CTCSBS_Ready;
			}
			CalcTopBottom();
			m_show_text = TRUE;
		}break;
	case CTCSBS_Ready:
		{
			//no action
		}break;
	case CTCSBS_Change:
		{
			m_ratio -= animSpd*2.0f;
			if(m_ratio < 0.0f){
				m_ratio = 0.0f;
				m_state = CTCSBS_Change2;
			}
			CalcTopBottom();
			m_show_text = TRUE;
		}break;
	case CTCSBS_Change2:
		{
			m_ratio += animSpd*2.0f;
			if(m_ratio > 1.0f){
				m_ratio = 1.0f;
				m_state = CTCSBS_Ready;
			}
			CalcTopBottom();
			m_show_text = TRUE;
			UpdateText();
		}break;
	case CTCSBS_Hide:
		{
			m_ratio -= animSpd;
			if(m_ratio < 0.0f){
				m_ratio = 0.0f;
				m_state = CTCSBS_HideComplete;
			}
			CalcTopBottom();
			m_show_text = TRUE;
		}break;
	case CTCSBS_Show:
		{
			m_ratio += animSpd;
			if(m_ratio > 1.0f){
				m_ratio = 1.0f;
				m_state = CTCSBS_Ready;
			}
			CalcTopBottom();
			m_show_text = TRUE;
		}break;
	case CTCSBS_HideComplete:
		{
			//no action
		}break;
	case CTCSBS_Extend:
		{
			m_ratio -= animSpd*2.0f;
			if(m_ratio < 0.0f){
				m_ratio = 0.0f;
				m_state = CTCSBS_Extend2;
			}
			CalcTopBottom();
			m_show_text = TRUE;
		}break;
	case CTCSBS_Extend2:
		{
			m_ratio += animSpd*2.0f;
			if(m_ratio > 1.0f){
				m_ratio = 1.0f;
				m_state = CTCSBS_Extended;
			}
			m_top    = m_base_y + (m_given_top-m_base_y) * m_ratio;
			m_bottom = m_base_y + (m_given_bottom-m_base_y) * m_ratio;
			m_show_text = FALSE;
		}break;
	case CTCSBS_Extended:
		{
			//no action
		}break;
	case CTCSBS_ExtendedHide:
		{
			m_ratio -= animSpd*2.0f;
			if(m_ratio < 0.0f){
				m_ratio = 0.0f;
				m_state = CTCSBS_HideComplete;
			}
			m_top    = m_base_y + (m_given_top-m_base_y) * m_ratio;
			m_bottom = m_base_y + (m_given_bottom-m_base_y) * m_ratio;
			m_show_text = FALSE;
		}break;
	case CTCSBS_Restore:
		{
			m_ratio -= animSpd*2.0f;
			if(m_ratio < 0.0f){
				m_ratio = 0.0f;
				m_state = CTCSBS_Change2;
			}
			m_top    = m_base_y + (m_given_top-m_base_y) * m_ratio;
			m_bottom = m_base_y + (m_given_bottom-m_base_y) * m_ratio;
			m_show_text = FALSE;
		}break;
	}

	return TRUE;
}

void CTCharacterSelectBelt::UpdateText()
{
	if(m_com){
		if(m_current_key>=0){
			_stprintf(m_disp_str,_T("COM/Player%d"),m_current_key+1);
		}
		else{
			_stprintf(m_disp_str,_T("COM/Wait"),m_current_key+1);
		}
	}
	else{
		_stprintf(m_disp_str,_T("Player%d"),m_current_key+1);
	}
//	TCHAR *p = g_charlist.GetCharacterDir(0, m_ringIndex);

	_stprintf(m_disp_str2, _T("%s"), g_charlist.GetRingName(m_ringIndex));
}

/*-----------------------------------------------------------
	その他 操作
-------------------------------------------------------------*/
void CTCharacterSelectBelt::SetRing(DWORD ringIndex)
{
	if(m_ringIndex==ringIndex)return;

	m_ringIndex = ringIndex;
	m_state = CTCSBS_Change;
}

void CTCharacterSelectBelt::SetCom(BOOL com)
{
	m_com = com;
}

void CTCharacterSelectBelt::SetKey(TCHAR idx)
{
	TCHAR prev_key = m_current_key;
	m_current_key = idx;

	if(prev_key != m_current_key){
		UpdateText();
	}
}

void CTCharacterSelectBelt::Show(BOOL b)
{
	if(b){
		m_state = CTCSBS_Show;
	}
	else{
		if(m_state==CTCSBS_Extend2 || m_state==CTCSBS_Extended){
			m_state = CTCSBS_ExtendedHide;
		}
		else m_state = CTCSBS_Hide;
	}
}


void CTCharacterSelectBelt::Extend(float top,float bottom)
{
	m_state = CTCSBS_Extend;
	m_given_top = top;
	m_given_bottom = bottom;
}

void CTCharacterSelectBelt::Restore()
{
	m_state = CTCSBS_Restore;
}

/*=======================================================================================

	オプション選択クラス

=========================================================================================*/
void CTOptionSelecter::EndSelect()
{
	CCharacterSelect *csel = dynamic_cast<CCharacterSelect*>( g_system.GetCurrentMainTask() );
	if(csel){
		csel->OnOptionSelect(this,m_selecter->GetSettings());
	}
}

/*=======================================================================================

	ステージ選択クラス

=========================================================================================*/

DWORD CTStageSelecter::m_previous_selected_ring  =0;
DWORD CTStageSelecter::m_previous_selected_index =0;

/*-----------------------------------------------------------
	初期化
-------------------------------------------------------------*/
void CTStageSelecter::Initialize()
{
	m_state = CTSSS_NotReady;
	m_counter = 0;
	m_dds = NULL;

	m_selected_ring  = m_previous_selected_ring;
	m_selected_index = m_previous_selected_index;
	
	LoadBMP();

	//オビパラメータ初期化
	m_txtTop	= TRUE;				//テキストを上に配置するか、下に配置するか
	m_txtLeft	= 50.0f;			//テキストの左位置
	m_txtR2L	= FALSE;			//テキスト右→左描画
	m_alpha1	= 230;
	m_alpha2	= 150;				//α、濃いとこ・薄いとこ
	m_lineCol	= 0x00555555;		//ラインのカラー（α以外）
	m_bodyCol	= 0x00FFFFFF;		//本体のカラー（α以外）
	m_txtCol1	= 0xFF555555;
	m_txtCol2	= 0xFF000000;		//テキストカラー（α含）
	m_lineWidth = 2.0f;				//色つきライン太さ
	m_base_y = 240.0f;
	m_height_base = 380.0f;

	UpdateText();
}


/*-----------------------------------------------------------
	破棄
-------------------------------------------------------------*/
void CTStageSelecter::Terminate()
{
	if(m_dds){
		RELSURFACE(m_dds);
	}
}


/*-----------------------------------------------------------
	更新
-------------------------------------------------------------*/
BOOL CTStageSelecter::Execute(DWORD time)
{
	CTStageSelecterState prv_state = m_state;
	float tick;

	const float height_max = 0.7f;
	const float width_max = 1.0f;
	const float height_min = 0.01f;

	switch(m_state)
	{
	case CTSSS_NotReady:
		{
			tick = m_counter*0.05f;
			if(tick < 0.5f){
				m_width  = width_max * tick*2.0f;
				m_height = height_min;
			}
			else{
				m_width  = width_max;
				m_height = height_max * (  (tick-0.5f)*2.0f );
			}
			if(tick >= 1.0f){
				m_state = CTSSS_Ready;
				tick = 1.0f;
			}
			m_ratio = tick;
		}break;
	case CTSSS_Ready:
		{
			HandlePad();
			tick = 1.0f;
			m_ratio = tick;
		}break;
	case CTSSS_Change:
		{
			tick = m_counter*0.05f;
			if(tick < 0.5f){
				m_width  = width_max * tick*2.0f;
				m_height = height_min;
			}
			else{
				m_width  = width_max;
				m_height = height_max * (  (tick-0.5f)*2.0f );
			}
			if(tick >= 1.0f){
				m_state = CTSSS_Ready;
				tick = 1.0f;
			}
		}break;
	case CTSSS_Change2:
		{
			tick = m_counter*0.05f;
			if(tick < 0.5f){
				m_width  = width_max * tick*2.0f;
				m_height = height_min;
			}
			else{
				m_width  = width_max;
				m_height = height_max * (  (tick-0.5f)*2.0f );
			}
			if(tick >= 1.0f){
				m_state = CTSSS_Ready;
				tick = 1.0f;
			}
		}break;
	case CTSSS_Hide:
		{
			tick = 1.0f - m_counter*0.035f;
			if(tick < 0.5f){
				m_width = (m_width <= 0) ? 0 : width_max * tick*0.8f;
				m_height = height_min;
			}
			else{
				m_width  = width_max;
				m_height = height_max * (  (tick-0.5f)*2.0f );
			}
			if(tick<-8.0f){
				m_state = CTSSS_HideComplete;
				CCharacterSelectBase* ccselect = dynamic_cast<CCharacterSelectBase*>(g_system.GetCurrentMainTask());
				if(ccselect){
					ccselect->OnEndAnimComplete(this);
				}
				tick = 0.0f;
			}
			m_ratio = tick;
		}break;
	case CTSSS_HideComplete:
		{
			return FALSE;
			tick = 0.0f;
			m_ratio = tick;
		}break;
	}

	if(m_state != prv_state){
		m_counter = 0;
	}
	else m_counter++;
	return TRUE;
}


/*-----------------------------------------------------------
	描画
-------------------------------------------------------------*/
void CTStageSelecter::Draw()
{
	//オビ・タイトル描画
	CTBeltBase::CalcTopBottom();
	CTBeltBase::Draw();

	if(m_dds)//プレビュー描画
	{
		RECT rsrc;
		MYRECT3D rdst;
		rsrc.top=rsrc.left=0;
		rsrc.right =(LONG)m_dds->wg;
		rsrc.bottom=(LONG)m_dds->hg;
		rdst.z = 0.0f;
		rdst.left  = (1.0f -0.5f*m_width)*ASPECTRATIO ;
		rdst.right = (1.0f +0.5f*m_width)*ASPECTRATIO ;
		rdst.top   = 1.0f -0.5f*m_height;
		rdst.bottom= 1.0f +0.5f*m_height;
		g_draw.MyBlt3D(m_dds,rsrc,rdst);
	}

	// ステージ名を表示
	if (m_state != CTSSS_NotReady && m_state != CTSSS_Hide)
	{
		RECT r;
		r.left = r.top =0;
		r.right = 640;
		r.bottom = 480 - 90;
		g_draw.DrawBlueText(r, g_stagelist.GetStageName( GetSelection() ), -1, DT_CENTER | DT_BOTTOM, 3);
	}

	//V!S!ﾁｬｷｰﾝ!って力技でやる
	float tick = 1.0f - m_counter*0.035f;
	int alpha = (DWORD)(255 * (1 - (-(tick+1.795f) * 0.4)));
	if (tick< 0.705f && tick > -1.795f)
	{
		alpha = (DWORD)(255 * (-(tick+0.705f) * 0.4));
	}
	int red = 0xFF;
	int green = 0xFF;
	int blue = 0xFF;
	DWORD color = 0;
	color = alpha << 24 | red << 16 | green << 8 | blue;

	if (m_state == CTSSS_Hide){
		if (tick < 0.0f){
			if (tick > -0.705f){
//			if (tick > -0.598f){
				g_system.DrawBMPTextEx(-51.0 - tick*450.0, 197.0, 0.0f, _T("V"), 0xFF5237FF, 3.0f, 3.0f, SYSBMPTXT_PROP);
				g_system.DrawBMPTextEx(640.0 + tick*450.0, 197.0, 0.0f, _T("S"), 0xFFFF3752, 3.0f, 3.0f, SYSBMPTXT_PROP);
//				g_system.DrawBMPTextEx(282.0, -58.0 - tick*450.0, 0.0f, _T("V"), 0xFF5237FF, 2.0f, 2.0f, SYSBMPTXT_PROP);
//				g_system.DrawBMPTextEx(320.0, 480.0 + tick*450.0, 0.0f, _T("S"), 0xFFFF3752, 2.0f, 2.0f, SYSBMPTXT_PROP);
			}
			else{
				g_system.DrawBMPTextEx(267.0, 197.0, 0.0f, _T("V"), 0xFF5237FF, 3.0f, 3.0f, SYSBMPTXT_PROP);
				g_system.DrawBMPTextEx(323.0, 197.0, 0.0f, _T("S"), 0xFFFF3752, 3.0f, 3.0f, SYSBMPTXT_PROP);
//				g_system.DrawBMPTextEx(282.0, 211.0, 0.0f, _T("V"), 0xFF5237FF, 2.0f, 2.0f, SYSBMPTXT_PROP);
//				g_system.DrawBMPTextEx(320.0, 211.0, 0.0f, _T("S"), 0xFFFF3752, 2.0f, 2.0f, SYSBMPTXT_PROP);
				if (tick > -1.795f)
					g_system.DrawBMPTextEx(267.0, 197.0, -0.0f, _T("VS"), color, 3.0f, 3.0f, SYSBMPTXT_PROP);
				else if (tick > -4.295f){
					g_system.DrawBMPTextEx(267.0 + (tick + 1.795f)*370.0f, 197.0f + (tick + 1.795f)*290.0f, -0.0f, _T("VS"), color, 3.0f - (tick + 1.795f)*20.0f, 3.0f - (tick + 1.795f)*20.0f, SYSBMPTXT_PROP);
//					g_system.DrawBMPTextEx(282.0 + (tick + 0.598f)*370.0f, 211.0f + (tick + 0.598f)*290.0f, -0.0f, _T("VS"), color, 2.0f - (tick + 0.598f)*20.0f, 2.0f - (tick + 0.598f)*20.0f, SYSBMPTXT_PROP);
				}
			}
		}
	}
}



/*-----------------------------------------------------------
	プレビューの読み直し
-------------------------------------------------------------*/
void CTStageSelecter::LoadBMP()
{
	if(m_dds){
		RELSURFACE(m_dds);
	}

	TCHAR *filepath = new TCHAR[MAX_PATH];
	_stprintf(filepath,_T("%s\\preview"),g_stagelist.GetStageDir( GetSelection() ) );
	m_dds = g_draw.CreateSurfaceFrom256Image(filepath);
	delete [] filepath;
}


/*-----------------------------------------------------------
	テキスト更新
-------------------------------------------------------------*/
void CTStageSelecter::UpdateText()
{
	_stprintf(m_disp_str,_T("%s"),
		g_stagelist.GetStageDir( GetSelection() )
		);

	for(UINT i=0;i<strlen(m_disp_str);i++){
		if(m_disp_str[i]=='\\'){
			m_disp_str[i]='/';//こっちのほうがかっこよさげ
		}
	}
}


/*-----------------------------------------------------------
	キー入力処理
-------------------------------------------------------------*/
void CTStageSelecter::HandlePad()
{
	//SELECTタスクゲット
	CCharacterSelectBase* ccselect = dynamic_cast<CCharacterSelectBase*>(g_system.GetCurrentMainTask());
	if(!ccselect)return;

	//キー入力取得
	DWORD keystate=0;
	std::vector<DWORD>::iterator ite=m_keyIndexList.begin();
	std::vector<DWORD>::iterator itee=m_keyIndexList.end();
	for(;ite!=itee;ite++){
		keystate |= g_input.GetKey( *ite ,0);
	}
	
	DWORD prvring = m_selected_ring;
	DWORD prvindx = m_selected_index;

	//決定
	if(keystate & KEYSTA_BA2){//「A」決定
		ccselect->OnStageSelect(this,GetSelection());
		m_state = CTSSS_Hide;
	}
	else if(keystate & KEYSTA_BD2){//ランダム決定
		ccselect->OnStageSelect(this,-1);
		m_state = CTSSS_Hide;
	}
	//←・→
	else if(keystate & KEYSTA_ARIGHT2){//LEFT2){
		if(m_selected_index==0)m_selected_index =g_stagelist.GetStageCountRing(m_selected_ring)-1;
		else m_selected_index--;
		m_state = CTSSS_Change;
		m_counter = 0;
	}
	else if(keystate & KEYSTA_ALEFT2){//RIGHT2){
		m_selected_index++;
		if((int)m_selected_index >= g_stagelist.GetStageCountRing(m_selected_ring))
			m_selected_index=0;
		m_state = CTSSS_Change;
		m_counter = 0;
	}
	//↑・↓(リング変更)
	else if( keystate & KEYSTA_UP2 ){
		if(m_selected_ring==0)m_selected_ring=g_stagelist.GetRingNum()-1;
		else m_selected_ring--;
		while(g_stagelist.GetStageCountRing(m_selected_ring)==0){
			if(m_selected_ring==0)m_selected_ring=g_stagelist.GetRingNum()-1;
			else m_selected_ring--;
		}
		if(g_stagelist.GetStageCountRing(m_selected_ring)-1 < (int)m_selected_index)
			m_selected_index = g_stagelist.GetStageCountRing(m_selected_ring)-1 ;
		m_state = CTSSS_Change2;
	}
	else if( keystate & KEYSTA_DOWN2 ){
		m_selected_ring++;
		if((int)m_selected_ring > g_stagelist.GetRingNum()-1)m_selected_ring=0;
		while(g_stagelist.GetStageCountRing(m_selected_ring)==0){
			m_selected_ring++;
			if((int)m_selected_ring>g_stagelist.GetRingNum()-1)m_selected_ring=0;
		}
		if(g_stagelist.GetStageCountRing(m_selected_ring)-1 < (int)m_selected_index)
			m_selected_index = g_stagelist.GetStageCountRing(m_selected_ring)-1 ;
		m_state = CTSSS_Change2;
	}
	
	if( prvring != m_selected_ring || prvindx != m_selected_index )
	{
		LoadBMP();
		UpdateText();
	}
}


//リング+インデックス　→　全体でのインデックス
int CTStageSelecter::GetSelection()
{
	return g_stagelist.Ring2Serial( m_selected_ring , m_selected_index );
}





/*=======================================================================================

	対戦・基本設定設定タスク

=========================================================================================*/

BYTE CTConditionSelecter::m_type = TAISENKEISIKI_GOCYAMAZE;

BYTE CTConditionSelecter::m_assign[2][MAXNUM_TEAM] = 
	{
		{0,			  CASSIGN_NONE,CASSIGN_NONE},
		{CASSIGN_COM, CASSIGN_NONE,CASSIGN_NONE}
	};

int CTConditionSelecter::m_limit_time_index = 1;
int CTConditionSelecter::m_limit_time[5] = {40,60,99,120,-1};

/*---------------------------------------------------------------------------------------
	初期化
-----------------------------------------------------------------------------------------*/
void CTConditionSelecter::Initialize()
{
	Check();

	m_state = CTCoS_Start;
	m_counter = 0;
	m_selected = MAXNUM_TEAM*2 + 2;
	m_ratio = 0.0f;

	//オビパラメータ初期化
	m_txtTop	= TRUE;				//テキストを上に配置するか、下に配置するか
	m_txtLeft	= 50.0f;			//テキストの左位置
	m_txtR2L	= FALSE;			//テキスト右→左描画
	m_alpha1	= 230;
	m_alpha2	= 150;				//α、濃いとこ・薄いとこ
	m_lineCol	= 0x00555555;		//ラインのカラー（α以外）
	m_bodyCol	= 0x00FFFFFF;		//本体のカラー（α以外）
	m_txtCol1	= 0xFF555555;
	m_txtCol2	= 0xFF000000;		//テキストカラー（α含）
	m_lineWidth = 2.0f;				//色つきライン太さ
	m_base_y = 240.0f;
	m_height_base = 380.0f;
	_stprintf(m_disp_str,_T("SETTINGS"));

	for(int j=0;j<2;j++){
		for(int i=0;i<MAXNUM_TEAM;i++)
		{
			// Stableは使わない
			if (m_assign[j][i] == CASSIGN_STABLE)
				m_assign[j][i] = CASSIGN_COM;
		}
	}
}


/*---------------------------------------------------------------------------------------
	終了処理
-----------------------------------------------------------------------------------------*/
void CTConditionSelecter::Terminate()
{
	CTBeltBase::Terminate();
}


/*---------------------------------------------------------------------------------------
	更新
-----------------------------------------------------------------------------------------*/
BOOL CTConditionSelecter::Execute(DWORD time)
{
	CTConditionSelecter_State pstate=m_state;
	const int select_max = MAXNUM_TEAM*2+3;

	switch(m_state)
	{
	case CTCoS_Start://出現中
		{
			float tick = (float)m_counter/20.0f;
			if(tick>1.0f){
				tick=1.0f;
				m_state = CTCoS_Execute;
			}
			m_ratio = tick;
		}break;
	case CTCoS_Execute://実行中
		{
			m_ratio = 1.0f;

			DWORD key = g_input.GetAllKey();
			if(m_selected==select_max-1 && key&KEYSTA_BUTTONS && m_ok){//ケテーイ
				CCharacterSelect* ccselect = dynamic_cast<CCharacterSelect*>(g_system.GetCurrentMainTask());
				ccselect->OnConditionDecided(this);
				m_state = CTCoS_Hide;
			}
			else if(key&KEYSTA_BC2){//OKに移動
				m_selected=select_max-1;
			}
			else if( m_selected==select_max && key&KEYSTA_BUTTONS ){//タイトルに戻る
				g_system.ReturnTitle();
				return FALSE;
			}
			else if (key&(KEYSTA_BA2 | KEYSTA_BB2) && m_selected != select_max - 1)//項目選択
			{
				m_state = CTCoS_Selected;
			}
			else if(key&KEYSTA_ALEFT2){//左移動
				if(1+MAXNUM_TEAM < m_selected && m_selected < select_max-1)
					m_selected-=MAXNUM_TEAM;
				else if(m_selected==1)
					m_selected--;
			}
			else if(key&KEYSTA_ARIGHT2){//右移動
				if(1 < m_selected && m_selected < 2+MAXNUM_TEAM)
					m_selected+=MAXNUM_TEAM;
				else if(m_selected==0)
					m_selected++;
			}
			else if(key&KEYSTA_DOWN2){//下移動
				if(m_selected == 1 || m_selected == 1+MAXNUM_TEAM)
					m_selected+=MAXNUM_TEAM+1;
				else if(m_selected==0)
					m_selected+=2;
				else
					m_selected++;
			}
			else if(key&KEYSTA_UP2){//上移動
				if(m_selected == 1 || m_selected == 2)
					m_selected-=2;
				else if(m_selected == 2+MAXNUM_TEAM)
					m_selected-=MAXNUM_TEAM+1;
				else
					m_selected--;
			}
			m_selected = (m_selected+select_max+1) % (select_max+1);
		}break;
	case CTCoS_Selected:	// 項目選択中
		{
			DWORD key = g_input.GetAllKey();
			if (key&(KEYSTA_ALEFT2 | KEYSTA_UP2)){//変更
				Change(FALSE);
			}
			else if (key&(KEYSTA_ARIGHT2 | KEYSTA_DOWN2)){//変更
				Change(TRUE);
			}
			else if (key&(KEYSTA_BA2 | KEYSTA_BB2)){//決定
				m_state = CTCoS_Execute;
			}
		}break;
	case CTCoS_Hide://消失中
		{
			float tick = (float)m_counter/20.0f;
			if(tick>1.0f){
				tick=1.0f;
				m_state = CTCoS_HideComplete;
			}
			m_ratio = 1.0f-tick;
		}break;
	case CTCoS_HideComplete://消失完了
		{
			m_ratio = 0.0f;
		}break;
	}

	m_counter++;
	if(pstate!=m_state){
		m_counter=0;
	}
	return TRUE;
}

//設定変更
void CTConditionSelecter::Change(BOOL key)
{
	//対戦形式変更
	if(m_selected==0)
	{
		m_type += key ? 1 : -1;
		m_type = (m_type+3-1)%3 +1;
		Check();
		return;
	}
	//制限時間変更
	else if(m_selected==1 && m_type==TAISENKEISIKI_GOCYAMAZE)
	{
		m_limit_time_index += key ? 1 : -1;
		m_limit_time_index = (m_limit_time_index+5)%5 ;
		Check();
		return;
	}

	int sel1 = (m_selected-2)/MAXNUM_TEAM;
	int sel2 = (m_selected-2)%MAXNUM_TEAM;

	if(m_assign[sel1][sel2]&CASSIGN_SPECIFIC){
		switch(m_assign[sel1][sel2]){
		case CASSIGN_COM:
			m_assign[sel1][sel2] = key ? CASSIGN_NONE : MAXNUM_KEYI-1 ;
			break;
		case CASSIGN_NONE:
			m_assign[sel1][sel2] = key ? 0 : CASSIGN_COM ;
			break;
		}
	}

	else{
		//通常範囲を超えてしまう場合
		if(m_assign[sel1][sel2]==0 && !key){
			m_assign[sel1][sel2] = CASSIGN_NONE;
		}
		else if(m_assign[sel1][sel2]==MAXNUM_KEYI-1 && key){
			m_assign[sel1][sel2] = CASSIGN_COM;
		}
		//普通
		else{
			m_assign[sel1][sel2] += key ? 1 : -1;
		}
	}

	Check();
}


/*---------------------------------------------------------------------------------------
	描画
-----------------------------------------------------------------------------------------*/
void CTConditionSelecter::Draw()
{
	CTBeltBase::CalcTopBottom();
	CTBeltBase::Draw();//オビ・タイトル描画

	const float x=40.0f;					//表示左位置
	const float txt_xr=1.00f;			//テキスト・y拡大率
	const float txt_yr=1.00f*m_ratio;	//テキスト・x拡大率
	float y = 100.0f;
	const float ystep = 35.0f;
	float shiftX=0.0f;	//TEAM2改行用

	int i,j;

	if(m_ratio<0.01f)return;

	TCHAR *tstr = new TCHAR[64];

	//対戦形式
	TCHAR *typenames[]={
		_T("Cooperation"),
		_T("Changeable"),
		_T("Unchangeable")
	};
	_stprintf(tstr,_T("Mode: %s"),typenames[m_type-1]);
	g_system.DrawBMPTextEx(x,y,0.0f,
				tstr,
				TxtCol(0),txt_xr,txt_yr,SYSBMPTXT_PROP);

	//制限時間
	if(m_type==TAISENKEISIKI_GOCYAMAZE){
		if(m_limit_time[m_limit_time_index]>0)
			_stprintf(tstr,_T("Time: %d"),m_limit_time[m_limit_time_index]);
		else
			_stprintf(tstr,_T("Time: -infinity-"));
	}
	else
		_stprintf(tstr,_T("Time: -no use-"));
	g_system.DrawBMPTextEx(x+341.0f,y,0.0f,
				tstr,
				TxtCol(1),txt_xr,txt_yr,SYSBMPTXT_PROP);
	y += ystep;
	y += ystep;

	_stprintf(tstr,_T("TEAM1"));
	g_system.DrawBMPTextEx(x,y,0.0f,tstr,0xFF5237FF,txt_xr,txt_yr,SYSBMPTXT_PROP);
	_stprintf(tstr,_T("TEAM2"));
	g_system.DrawBMPTextEx(x+300.0f,y,0.0f,tstr,0xFFFF3752,txt_xr,txt_yr,SYSBMPTXT_PROP);
	y += ystep;

	//TEAM*-x : ～
	for(j=0;j<2;j++){
		for(i=0;i<MAXNUM_TEAM;i++)
		{
			_stprintf(tstr,_T("%d: "),i+1);
			if(m_assign[j][i]&CASSIGN_SPECIFIC)//特別
			{
				switch(m_assign[j][i]){
				case CASSIGN_COM:	_stprintf(&tstr[strlen(tstr)],_T("Computer"));break;
				case CASSIGN_NONE:	_stprintf(&tstr[strlen(tstr)],_T("--None--"));break;
				default:			_stprintf(&tstr[strlen(tstr)],_T("Error?"));
				}
			}
			else{
				_stprintf(&tstr[strlen(tstr)],_T("Player%d"),m_assign[j][i]+1);
			}
			if(j==0)	//TEAM1
				shiftX=40.0f;
			else	//TEAM2
			{
				y=100.0f + (i+3)*ystep;
				shiftX=340.0f;
			}
			g_system.DrawBMPTextEx(x+shiftX,y,0.0f,
				tstr,TxtCol(j*MAXNUM_TEAM+i+2),txt_xr,txt_yr,SYSBMPTXT_PROP);
			y += ystep;
		}
		y += ystep;
	}

	//"OK"描画
	g_system.DrawBMPTextEx(640.0f-x,y,0.0f,
				m_ok ? _T("OK") : m_str_error,
				TxtCol(2*MAXNUM_TEAM+2),txt_xr,txt_yr,SYSBMPTXT_PROP|SYSBMPTXT_R2L);
	y += ystep;
	//"return title"
	g_system.DrawBMPTextEx(640.0f-x,y,0.0f,
				_T("Return Title"),
				TxtCol(2*MAXNUM_TEAM+3),txt_xr,txt_yr,SYSBMPTXT_PROP|SYSBMPTXT_R2L);

	delete [] tstr;
}


/*---------------------------------------------------------------------------------------
	設定の正当性チェック
-----------------------------------------------------------------------------------------*/
void CTConditionSelecter::Check()
{
	m_ok=TRUE;
	int i,j;

	//すべてNONEになってないか？
	BOOL ok;
	for(i=0;i<2;i++){
		ok=FALSE;
		for(j=0;j<MAXNUM_TEAM;j++){
			if(m_assign[i][j]!=CASSIGN_NONE){
				ok=TRUE;
			}
		}
		if(!ok){
			_stprintf(m_str_error,_T("Error, All None"));
			m_ok=FALSE;
			return;
		}
	}

	if(m_type==TAISENKEISIKI_GOCYAMAZE){
		//プレイヤーが重複していないか？
		int cnt;
		for(int k=0;k<MAXNUM_KEYI;k++)
		{
			cnt=0;
			for(i=0;i<2;i++){
				for(j=0;j<MAXNUM_TEAM;j++){
					if(m_assign[i][j]==k)cnt++;
				}
			}
			if(cnt>1){
				_stprintf(m_str_error,_T("Error, Player%d Duplication"),k+1);
				m_ok=FALSE;
				return;
			}
		}
	}
	else{
		//1プレイヤーが2つのチームにまたがって配置されていないか？
		BOOL belong[2];
		for(int k=0;k<MAXNUM_KEYI;k++)
		{
			belong[0]=belong[1]=FALSE;
			for(i=0;i<2;i++){
				for(j=0;j<MAXNUM_TEAM;j++){
					if(m_assign[i][j]==k)belong[i]=TRUE;
				}
			}
			if(belong[0] && belong[1]){
				_stprintf(m_str_error,_T("Error, Player%d is Betrayer"),k+1);//裏切り者ハケーン
				m_ok=FALSE;
				return;
			}
		}
	}
}








