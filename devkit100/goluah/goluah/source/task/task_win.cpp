
/*===========================================================

	勝利画面タスク

=============================================================*/

#include "stdafx.h"
#include "define_macro.h"
#include "global.h"
#include "task_win.h"


/*------------------------------------------------------------
	構築
--------------------------------------------------------------*/
CYouWin::CYouWin()
{
	int j;
	for(j=0;j<MAXNUM_TEAM;j++){
		dds_face[j]=NULL;
	}

	m_text_from_story=NULL;
}

//しゅーりょー
CYouWin::~CYouWin()
{
	DELETEARRAY(m_text_from_story);
}

/*------------------------------------------------------------
	タスク初期化
--------------------------------------------------------------*/
void CYouWin::Initialize()
{
	TCHAR filename[256],palname[256],*dir;
	wt = 0;
	if(g_battleresult.GetWinner()){
		wt=1;
	}
	m_text = m_text_from_story ? m_text_from_story : g_battleresult.GetKatiSerif();
	m_textlen=FALSE;

	//カオの読み込み
	for(UINT j=0;j<g_battleresult.GetWinnerCount();j++)
	{
		DWORD alt = OPT2ALT( g_battleinfo.GetCharacterOption(wt,j) );
		TCHAR altstr[3]={'\0','\0'};
		if(alt!=0)altstr[0]='a'+(TCHAR)alt-1;
		dir = g_charlist.GetCharacterDir(g_battleresult.GetCharacter(j));
		_stprintf(filename,_T("%s\\face3%s"),dir,altstr);
		_stprintf(palname,_T("%s\\pal%d"),dir,g_battleresult.GetColor(j));
		dds_face[j] = g_draw.CreateSurfaceFrom256Image(filename,palname);
	}

	//フロントバッファのコピー取得
	tex_fb = g_draw.GetFrontBufferCopy();

	//順番
	jun[0] = 0;//sc_battlefield->active_character[ wt ];//固定
	if(jun[0]==0){
		jun[1] = 1;
		jun[2] = 2;
	}
	else if(jun[0]==1){
		jun[1] = 0;
		jun[2] = 2;
	}
	else{
		jun[1] = 0;
		jun[2] = 1;
	}
	//zオーダー
	f_z[jun[0]]=0;
	f_z[jun[1]]=0.01f;
	f_z[jun[2]]=0.02f;

	counter=0;
	show_text = FALSE;

	g_sound.BGMPlay(_T(".\\system\\bgm\\win"),FALSE);
}

/*------------------------------------------------------------
	タスク終了
--------------------------------------------------------------*/
void CYouWin::Terminate()
{
	CleanUp();
}

/*------------------------------------------------------------
	タスク一実行
--------------------------------------------------------------*/
BOOL CYouWin::Execute(DWORD time)
{
	counter++;

	switch(g_battleresult.GetWinnerCount()){
	case 3:
		f_x[jun[2]] = 1800 - counter*40;//キャラ3の動き
		if(f_x[jun[2]] < 250){
			f_x[jun[2]] = 250;
		}
		f_x[jun[1]] = -1800 + counter*40;//キャラ2の動き
		if(f_x[jun[1]] > -200){
			f_x[jun[1]] = -200;
		}
		f_x[jun[0]] = 700 - counter*40;//キャラ1の動き
		if(f_x[jun[0]] < 0){
			f_x[jun[0]]=0;
		}
		if(f_x[jun[0]]==0)
			show_text=TRUE;
		break;
	case 2:
		f_x[jun[1]] = -1800 + counter*40;//キャラ2の動き
		if(f_x[jun[1]] > 200){
			f_x[jun[1]] = 200;
		}
		f_x[jun[0]] = 700 - counter*40;//キャラ1の動き
		if(f_x[jun[0]] < -100){
			f_x[jun[0]]=-100;
		}
		if(f_x[jun[1]]==200)
			show_text=TRUE;
		break;
	case 1:
		f_x[0] = 700 - counter*40;//キャラ1の動き
		if(f_x[0] < 0){
			f_x[0]=0;
		}
		if(f_x[0]==0)
			show_text=TRUE;
		break;
	}

	int n=3;
	while(show_text && n>0){
		if(m_textlen==strlen(m_text))break;
		m_textlen += (m_text[m_textlen]&0x80) ? 2 : 1;
		n--;
	}

	if(show_text && g_input.GetAllKey() ){
		return FALSE;//終了
	}

	return TRUE;
}


/*------------------------------------------------------------
	破棄
--------------------------------------------------------------*/
void CYouWin::CleanUp()
{
	int j;
	for(j=0;j<3;j++){
		RELSURFACE(dds_face[j]);
	}

	RELEASE(tex_fb);
}


/*------------------------------------------------------------
	描画
--------------------------------------------------------------*/
void CYouWin::Draw()
{
	g_draw.SetTransform(FALSE);
	g_draw.ResetParentMatrix();

	//フロントバッファコピー描画
	{
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
		vb[1].tv = 1.0f*360.0f/480.0f;
		vb[2].tv = 0.0f;
		vb[3].tv = 1.0f*360.0f/480.0f;
		
		vb[0].color = 
		vb[1].color = 
		vb[2].color = 
		vb[3].color = 0xFF555555;

		vb[0].x =  0.0f;
		vb[1].x =  0.0f;
		vb[2].x =  2.0f*320.0f/240.0f;
		vb[3].x =  2.0f*320.0f/240.0f;

		vb[0].y =  0.0f;
		vb[1].y =  2.0f*360.0f/480.0f;
		vb[2].y =  0.0f;
		vb[3].y =  2.0f*360.0f/480.0f;

		if (g_draw.pMyVertex) g_draw.pMyVertex->Unlock();

		g_draw.EnableZ(FALSE,FALSE);
		g_draw.d3ddev->SetStreamSource(0, g_draw.pMyVertex, 0, sizeof(MYVERTEX3D));
		g_draw.d3ddev->SetFVF(FVF_3DVERTEX);
		g_draw.d3ddev->SetTexture(0,tex_fb);
		g_draw.d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
	}

	
	RECT r;
	r.left=r.top=0;

	for(int i=0;i<3;i++){
		if(dds_face[i]!=NULL){
			r.right = (long)dds_face[i]->wg;
			r.bottom = (long)dds_face[i]->hg;
			g_draw.CheckBlt(dds_face[i],f_x[i],(int)(360-dds_face[i]->hg),r,
				FALSE,FALSE,0,f_z[i]);
		}
	}

	if(show_text && m_textlen>0)
	{
		r.left=0;
		r.top=360;
		r.right=640;
		r.bottom=480;
		TCHAR *disptxt = new TCHAR[m_textlen+1];
		memcpy(disptxt,m_text,m_textlen);
		disptxt[m_textlen]='\0';
		
		if(wt==0) g_draw.DrawBlueText(r,disptxt,-1,DT_LEFT|DT_WORDBREAK,3);
		else	  g_draw.DrawRedText(r,disptxt,-1,DT_LEFT|DT_WORDBREAK,3);

		delete [] disptxt;
	}
}

/*------------------------------------------------------------
	ストーリーから、テキストの設定
--------------------------------------------------------------*/
void CYouWin::SetStoryText(TCHAR *txt)
{
	DELETEARRAY(m_text_from_story);

	if(!txt)return;

	m_text_from_story = new TCHAR [ strlen(txt)+1 ];
	_tcscpy(m_text_from_story,txt);
}

