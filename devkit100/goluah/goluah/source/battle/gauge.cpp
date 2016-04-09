/*
主な変更点等
2011/10/29
	DrawPowerGaugeType
		・MAX時のcellを12に増やした(=12コマアニメーションするように)
		・1人の時はもっとゲージが長くてもいいと思うので、人数に応じて横幅を拡縮したい
	DrawHPG
		・DrawHPGaugeの代替品。ただし赤ゲージは未完成
		・特に問題がなければソース軽量化のためDrawHPGaugeは削除する予定
		・Ver.0.55らへん同様にHPゲージをimage3から表示するようにした
		・marvelとkofでアクティブなキャラのが一番上になるようにした
		・face1のサイズと位置を変更、marvelとkofで控えが暗くなるの廃止
		・WINとストライカーのマークの位置を少し下げた
		・1人だけのときはストライカーのマークを非表示にした
		・z=-0.05fのせいか、幅200で読み込んでるのに表示時に幅204になるのが納得いかない
	DrawHPwaku
		・対戦形式&人数に応じてHPの枠をcellから表示出来るようにした
*/
/*===================================================================

	体力ゲージ
	キャラクタ顔・勝利カウント・FPS
	パワーゲージ

	･･･等、戦闘時のシステム情報描画関係

=====================================================================*/

#include "stdafx.h"
#include "define_macro.h"
#include "gauge.h"
#include "global.h"
#include "gobject.h"
#include <math.h>


/*-------------------------------------------------------------------
	生成破棄
---------------------------------------------------------------------*/
CGauge::CGauge()
{
	for(int i=0;i<2;i++){
		for(int j=0;j<3;j++){
			dds_face[i][j]=NULL;
		}
	}
}

void CGauge::CleanUp()//開放する必要のあるものを 開放する
{
	for(int i=0;i<2;i++){
		for(int j=0;j<3;j++){
			RELSURFACE(dds_face[i][j]);
		}
	}
}


/*-------------------------------------------------------------------
	初期化
---------------------------------------------------------------------*/
void CGauge::Initialize()
{
	CleanUp();//まずは以前のデータをクリア

	battleTask = dynamic_cast<CBattleTaskBase*>( g_system.GetCurrentMainTask() );
	if(!battleTask)throw;

//	DWORD num_teammember = g_battleinfo.GetNumTeam();

	//変数の初期化
	activegauge[0] = activegauge[1] = 0;

	DWORD i,j;

	flip_counter=0;
	GOBJECT *pdat=NULL;
	for(i=0;i<2;i++){
		for(j=0;j<g_battleinfo.GetNumTeam(i);j++){
			pdat = battleTask->GetCharacterInfo(i,j);
			if(pdat!=NULL){
				gauge_prv[i][j] = pdat->hpmax;
				gauge_prv2[i][j] = pdat->hpmax;
			}
			pgauge_prv[i][j]=0;
			numprv[i][j]=0;
			pg_draw[i][j]=FALSE;//パワーゲージを描画するかどうか
			pg_dx[i][j]=-200;//パワーゲージのずらし幅
		}
	}

	//カオの読み込み
	TCHAR filename[256],palname[256];
	for(i=0;i<2;i++){
		for(j=0;j<g_battleinfo.GetNumTeam(i);j++){
			_stprintf(filename,_T("%s\\face1"),g_charlist.GetCharacterDir(g_battleinfo.GetCharacter(i,j)));
			_stprintf(palname,_T("%s\\pal%d"),g_charlist.GetCharacterDir(g_battleinfo.GetCharacter(i,j)),g_battleinfo.GetColor(i,j));
			dds_face[i][j]=g_draw.CreateSurfaceFrom256Image(filename,palname);
		}
	}

	oid = battleTask->CreateGObject();
	pdat=(GOBJECT*)battleTask->GetGObjectInfo(oid);
	pdat->objtype |= GOBJFLG_DONOTSTOP ;
	pdat->tid = TEAM_SYSTEM;
	pdat->msghandler = GaugeObjectMessage;
	pdat->pobjdat_u = this;
}


/*-------------------------------------------------------------------
	ゲージの、戦闘オブジェクトとしてのメッセージハンドラ
---------------------------------------------------------------------*/
DWORD CGauge::GaugeObjectMessage(DWORD msg,LPVOID pdat,DWORD prm)
{
	GOBJECT *pobj = (GOBJECT*)pdat;
	CGauge *cp_gauge = (CGauge*)pobj->pobjdat_u;
	if(!cp_gauge)return 0;

	switch(msg){
	case GOBJMSG_ACTION:
		cp_gauge->Action();return(TRUE);
	case GOBJMSG_DRAWFRONT:
		cp_gauge->Draw();return(TRUE);
	case GOBJMSG_CNGROUND:
		return(TRUE);
	}
	return(TRUE);
}


/*-------------------------------------------------------------------
	フレームごとのUpdate処理
---------------------------------------------------------------------*/
void CGauge::Action()
{
	flip_counter++;
	if(flip_counter>50)flip_counter=50;

	int i,j;
	GOBJECT *pdat;

	
	for(j=0;j<2;j++){
		for(i=0;i<(int)g_battleinfo.GetNumTeam(j);i++){
			//prv の更新
			pdat= (GOBJECT*)battleTask->GetCharacterInfo(j,i);
			if(pdat!=NULL){
				if(pgauge_prv[j][i] < pdat->gauge){
					pgauge_prv[j][i] += 0.01;
					if(pgauge_prv[j][i] > pdat->gauge)pgauge_prv[j][i] = pdat->gauge;
				}
				else if(pgauge_prv[j][i] > pdat->gauge){
					pgauge_prv[j][i] -= 0.01;
					if(pgauge_prv[j][i] < pdat->gauge)pgauge_prv[j][i] = pdat->gauge;
				}
			}
			//パワーゲージの移動
			if(g_battleinfo.GetBattleType()!=TAISENKEISIKI_GOCYAMAZE){
				if((int)battleTask->GetActiveCharacterID(j) == i){
					pg_dx[j][i] += 5;
					if(pg_dx[j][i]>0)pg_dx[j][i]=0;
				}
				else{
					pg_dx[j][i] -= 5;
					if(pg_dx[j][i]<-200)pg_dx[j][i]=-200;
				}
				if(pg_dx[j][i]<-180)pg_draw[j][i]=FALSE;
				else pg_draw[j][i]=TRUE;
			}
		}
	}
}


/*-------------------------------------------------------------------
	描画
---------------------------------------------------------------------*/
void CGauge::Draw()
{
	//全体の枠(?)
	battleTask->SetTransform(FALSE);
	g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_HPGWAKU1,320,43,-0.05f,0);
	g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_HPGWAKU2,320,40,-0.04f,0);
	//時間制限orFPS
	if(!g_system.sw_showfps){
		if(battleTask->limittime >= 0)
			battleTask->DrawNumber9(battleTask->limittime,310,45,FALSE,-0.055f);
		else 
		{
			switch(g_battleinfo.GetBattleType())
			{
			case TAISENKEISIKI_KOUTAI:
				g_draw.CellDraw(g_system.GetSystemGraphicSurface(),
								g_system.GetSystemGraphicCell(),
								g_system.GetSystemGraphicRect(),
								CELL_DISP_MARVEL_LIKE,
								320,42,-0.051f,0);
				break;
			case TAISENKEISIKI_JYUNBAN:
				g_draw.CellDraw(g_system.GetSystemGraphicSurface(),
								g_system.GetSystemGraphicCell(),
								g_system.GetSystemGraphicRect(),
								CELL_DISP_KOF_LIKE,
								320,42,-0.051f,0);
				break;
			default:
				g_draw.CellDraw(g_system.GetSystemGraphicSurface(),
								g_system.GetSystemGraphicCell(),
								g_system.GetSystemGraphicRect(),
								CELL_TIME_INFINITY,
								320,42,-0.051f,0);
			}
		}
	}
	else battleTask->DrawNumber(g_system.GetFPS(),310,45,FALSE,-0.055f);

	switch(g_battleinfo.GetBattleType()){
	case TAISENKEISIKI_GOCYAMAZE://ごっちゃまぜ
		DrawPowerGauge1();
		if (g_config.GetGaugeMode() == GAUGE_1220)
		{
			DrawHPG1();
			DrawHPwaku1();
		}
		else if (g_config.GetGaugeMode() == GAUGE_1000)
			DrawHPGauge1();
		break;
	default:
		DrawPowerGauge2();
		if (g_config.GetGaugeMode() == GAUGE_1220)
		{
			DrawHPwaku2();
			DrawHPG2();
		}
		else if (g_config.GetGaugeMode() == GAUGE_1000)
			DrawHPGauge2();
	}
	
	battleTask->SetTransform(TRUE);
}

// private **************************************************************
//　体力ゲージ描画
//***********************************************************************

void CGauge::DrawHPGauge1()//体力ゲージの描画（TAISENKEISIKI_GOCYAMAZE）
{
	int i;
	DWORD j;
	D3DXMATRIX mati;
	D3DXMatrixIdentity(&mati);

	GOBJECT *pdat;
	MYVERTEX3D vb[4];
	double hiritu;

	g_draw.d3ddev->SetTexture(0,NULL);
	g_draw.d3ddev->SetTransform(D3DTS_WORLD,&mati);

	BYTE gauge_blue,gauge_yellow;
	
	double turiage_y;
	double top;
	double bottom;

	for(j=0;j<g_battleinfo.GetNumTeam(0);j++)
	{
		switch(g_battleinfo.GetNumTeam(0)){
		case 2:turiage_y=-0.025;
			top=0.025;
			bottom=0.02;break;
		case 3:turiage_y=-0.075;
			top=0.02;
			bottom=0.02;break;
		default:turiage_y=0;
			top=0.03;
			bottom=0.03;
		}
		//1p側体力棒
		pdat = (GOBJECT*)battleTask->GetCharacterInfo(0,j);
		if(pdat!=NULL){
			//体力
			if(pdat->hpmax!=0){
				gauge_prv2[0][j] -= (gauge_prv2[0][j] - pdat->hp) / 2.718282;
					if(gauge_prv2[0][j] < pdat->hp)gauge_prv2[0][j] = pdat->hp;
				hiritu = (double)gauge_prv2[0][j] / (double)pdat->hpmax;
				if(hiritu<0)hiritu=0;
				vb[0].x = (float)( 320.0/240.0 - 50.0/240.0 );
				vb[0].y = (float)( 0.18 - top +turiage_y +0.05*j);
				vb[1].x = (float)( 320.0/240.0 - 50.0/240.0 );
				vb[1].y = (float)( 0.18 + bottom +turiage_y +0.05*j);
				vb[2].x = (float)( 320.0/240.0  - 196.0/240.0*hiritu - 50.0/240.0 );
				vb[2].y = (float)( 0.18 - top +turiage_y +0.05*j);
				vb[3].x = (float)( 320.0/240.0  - 196.0/240.0*hiritu - 50.0/240.0 );
				vb[3].y = (float)( 0.18 + bottom +turiage_y +0.05*j);
				gauge_blue = (BYTE)(255*hiritu);
				gauge_yellow = (BYTE)(255*(1-hiritu));
				vb[2].color = vb[3].color = 0xFF000000 + gauge_yellow*256*256 + gauge_yellow*256 +gauge_blue;
				vb[0].color = vb[1].color = 0xFFAA77FF;
				for(i=0;i<4;i++){
					vb[i].z = -0.05f;
					vb[i].tu = vb[i].tv = 0;
				}
				g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));

				//減った分
				if(pdat->hp < (int)pdat->hpmax)
				{
					for(i=0;i<4;i++){
						vb[i].color = 0xFFFF0000;
					}
					gauge_prv[0][j] -= 3;
					if(pdat->hp<0){
						if(gauge_prv[0][j] < 0)gauge_prv[0][j] = 0;
					}
					else{
						if(gauge_prv[0][j] < pdat->hp)gauge_prv[0][j] = pdat->hp;
					}
					hiritu = (double)gauge_prv[0][j] / (double)pdat->hpmax;
					if(hiritu<0)hiritu=0;
					vb[0].x = vb[1].x = (float)( 320.0/240.0  - 200.0/240.0*hiritu - 50.0/240.0 );
					g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
				}
				else
				{
					gauge_prv[0][j] = 0;
				}
			}
		}
	}
	for(j=0;j<g_battleinfo.GetNumTeam(1);j++)
	{
		switch(g_battleinfo.GetNumTeam(1)){
		case 2:turiage_y=-0.025;
			top=0.025;
			bottom=0.02;break;
		case 3:turiage_y=-0.075;
			top=0.02;
			bottom=0.02;break;
		default:turiage_y=0;
			top=0.03;
			bottom=0.03;
		}
		//2p側体力棒
		pdat = (GOBJECT*)battleTask->GetCharacterInfo(1,j);
		if(pdat!=NULL){
			//体力
			if(pdat->hpmax!=0){
				gauge_prv2[1][j] -= (gauge_prv2[1][j] - pdat->hp) / 2.718282;
					if(gauge_prv2[1][j] < pdat->hp)gauge_prv2[1][j] = pdat->hp;
				hiritu = (double)gauge_prv2[1][j] / (double)pdat->hpmax;
				if(hiritu<0)hiritu=0;
				vb[0].x = (float)( 320.0/240.0 + 50.0/240.0 );
				vb[0].y = (float)( 0.18 - top +turiage_y +0.05*j);
				vb[1].x = (float)( 320.0/240.0 + 50.0/240.0 );
				vb[1].y = (float)( 0.18 + bottom +turiage_y +0.05*j);
				vb[2].x = (float)( 320.0/240.0  + 200.0/240.0*hiritu + 50.0/240.0 );
				vb[2].y = (float)( 0.18 - top +turiage_y +0.05*j);
				vb[3].x = (float)( 320.0/240.0  + 200.0/240.0*hiritu + 50.0/240.0 );
				vb[3].y = (float)( 0.18 + bottom +turiage_y +0.05*j);
				gauge_blue = (BYTE)(255*hiritu);
				gauge_yellow = (BYTE)(255*(1-hiritu));
				vb[2].color = vb[3].color = 0xFF000000 + gauge_yellow*256*256 + gauge_yellow*256 +gauge_blue;
				vb[0].color = vb[1].color = 0xFFAA77FF;
				for(i=0;i<4;i++){
					vb[i].z = -0.05f;
					vb[i].tu = vb[i].tv = 0;
				}
				g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
			
				//減った分
				if(pdat->hp < (int)pdat->hpmax)
				{
					for(i=0;i<4;i++){
						vb[i].color = 0xFFFF0000;
					}
					gauge_prv[1][j] -= 3;
					if(pdat->hp<0){
						if(gauge_prv[1][j] < 0)gauge_prv[1][j] = 0;
					}
					else{
						if(gauge_prv[1][j] < pdat->hp)gauge_prv[1][j] = pdat->hp;
					}
					hiritu = (double)gauge_prv[1][j] / (double)pdat->hpmax;
					if(hiritu<0)hiritu=0;
					vb[0].x = vb[1].x = (float)( 320.0/240.0  + 200.0/240.0*hiritu + 50.0/240.0 );
					g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
				}
				else
				{
					gauge_prv[1][j] = 0;
				}
				
			}
		}
	}

	MYRECT3D rdst;
	RECT rsrc;
	rsrc.top=rsrc.left=0;
	//カオ(1p)
	for(j=0;j<g_battleinfo.GetNumTeam(0);j++){
		if(dds_face[0][j]!=NULL){
			int kx = 0, ky = 0;

			// 揺れ
			CGObject* pobj = battleTask->GetCharacterObject(0,j);
			if (pobj && pobj->siver)
			{
				kx = rand() % 16 - 8;
				ky = rand() % 16 - 8;
			}

			rsrc.right = (long)dds_face[0][j]->wg;
			rsrc.bottom = (long)dds_face[0][j]->hg;
			rdst.z = -0.04;
			rdst.left = 20.0/240.0 + 0.08*j+kx/240.0;
			rdst.right = rdst.left + 49.0/240.0;	//50.0x50.0だと何故か51x51で表示されてしまう
			rdst.top = (float)(20.0/240.0) + 0.065*j+ky/240.0;
			rdst.bottom = rdst.top + (49.0/240.0);
			g_draw.ResetParentMatrix();
			
			pdat =(GOBJECT*)battleTask->GetCharacterInfo(0,j); 
			if(pdat!=NULL){
				if(pdat->hp <= 0)
					g_draw.MyBlt3D(dds_face[0][j],rsrc,rdst,0,0xFFFF0000);
				else g_draw.MyBlt3D(dds_face[0][j],rsrc,rdst,0,0xFFFFFFFF);
			}
			else g_draw.MyBlt3D(dds_face[0][j],rsrc,rdst,0,0xFFFFFFFF);
		}
	}
	//カオ(2p)
	for(j=0;j<g_battleinfo.GetNumTeam(1);j++){
		if(dds_face[1][j]!=NULL){
			int kx = 0, ky = 0;

			// 揺れ
			CGObject* pobj = battleTask->GetCharacterObject(1,j);
			if (pobj && pobj->siver)
			{
				kx = rand() % 16 - 8;
				ky = rand() % 16 - 8;
			}

			rsrc.right = (long)dds_face[1][j]->wg;
			rsrc.bottom = (long)dds_face[1][j]->hg;
			rdst.z = -0.04;
			rdst.left = 2.0*(320.0/240.0) - 20.0/240.0 - 0.08*j+kx/240.0;
			rdst.right = rdst.left - (49.0/240.0);
			rdst.top = 20.0/240.0 + 0.065*j+ky/240.0;
			rdst.bottom = rdst.top + (49.0/240.0);
			g_draw.ResetParentMatrix();
			pdat =(GOBJECT*)battleTask->GetCharacterInfo(1,j); 
			if(pdat!=NULL){
				if(pdat->hp <= 0)
					g_draw.MyBlt3D(dds_face[1][j],rsrc,rdst,0,0xFFFF0000);
				else g_draw.MyBlt3D(dds_face[1][j],rsrc,rdst,0,0xFFFFFFFF);
			}
			else g_draw.MyBlt3D(dds_face[1][j],rsrc,rdst,0,0xFFFFFFFF);
		}
	}

	//勝利マーク
	DWORD wincount;
	//1p
	wincount = battleTask->GetWinCount(0);
	for(i=0;i<(int)g_config.GetMaxPoint();i++){
		if((int)wincount>i)g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_WINMARK,	320-65-30*i,65,-0.05f,0);
		else g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_WINMARK2,			320-65-30*i,65,-0.05f,0);
	}
	//2p
	wincount = battleTask->GetWinCount(1);
	for(i=0;i<(int)g_config.GetMaxPoint();i++){
		if((int)wincount>i)g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_WINMARK,	320+65+30*i,65,-0.05f,0);
		else g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_WINMARK2,			320+65+30*i,65,-0.05f,0);
	}
}

void CGauge::DrawHPGauge2()//体力ゲージの描画（2vs2）
{
	int i;
	D3DXMATRIX mati;
	D3DXMatrixIdentity(&mati);
	DWORD j;

	GOBJECT *pdat;
	MYVERTEX3D vb[4];
	double hiritu;

	g_draw.d3ddev->SetTexture(0,NULL);
	g_draw.d3ddev->SetTransform(D3DTS_WORLD,&mati);

	BYTE gauge_blue,gauge_yellow;
	
	double turiage_y;

	for(j=0;j<g_battleinfo.GetNumTeam(0);j++)
	{
		switch(g_battleinfo.GetNumTeam(0)){
		case 2:turiage_y=-0.025;break;
		case 3:turiage_y=-0.075;break;
		default:turiage_y=0;
		}
		//1p側体力棒
		pdat = (GOBJECT*)battleTask->GetCharacterInfo(0,j);
		if(pdat!=NULL){
			//体力
			if(pdat->hpmax!=0){
				gauge_prv2[0][j] -= 20;
					if(gauge_prv2[0][j] < pdat->hp)gauge_prv2[0][j] = pdat->hp;
				hiritu = (double)gauge_prv2[0][j] / (double)pdat->hpmax;
				if(hiritu<0)hiritu=0;
				vb[0].x = (float)( 320.0/240.0 - 50.0/240.0 );
				vb[0].y = (float)( 0.18 - 0.03 +turiage_y +0.05*j);
				vb[1].x = (float)( 320.0/240.0 - 50.0/240.0 );
				vb[1].y = (float)( 0.18 + 0.03 +turiage_y +0.05*j);
				vb[2].x = (float)( 320.0/240.0  - 200.0/240.0*hiritu - 50.0/240.0 );
				vb[2].y = (float)( 0.18 - 0.03 +turiage_y +0.05*j);
				vb[3].x = (float)( 320.0/240.0  - 200.0/240.0*hiritu - 50.0/240.0 );
				vb[3].y = (float)( 0.18 + 0.03 +turiage_y +0.05*j);
				gauge_blue = (BYTE)(255*hiritu);
				gauge_yellow = (BYTE)(255*(1-hiritu));
				vb[2].color = vb[3].color = 0xFF000000 + gauge_yellow*256*256 + gauge_yellow*256 +gauge_blue;
				vb[0].color = vb[1].color = 0xFFAA77FF;
				for(i=0;i<4;i++){
					vb[i].z = -0.05f;
					vb[i].tu = vb[i].tv = 0;
				}
				//控えは細く&暗く
				if(j!=battleTask->GetActiveCharacterID(0)){
					vb[0].y = (float)( 0.18 - 0.01 +turiage_y +0.05*j);
					vb[1].y = (float)( 0.18 + 0.01 +turiage_y +0.05*j);
					vb[2].y = (float)( 0.18 - 0.01 +turiage_y +0.05*j);
					vb[3].y = (float)( 0.18 + 0.01 +turiage_y +0.05*j);
					vb[0].color = vb[1].color = vb[2].color = vb[3].color = 0xFF000077;
				}
				g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
			
				//減った分
				if(pdat->hp < (int)pdat->hpmax)
				{
					for(i=0;i<4;i++){
						vb[i].color = 0xFFFF0000;
					}
					gauge_prv[0][j] -= 3;
					if(pdat->hp<0){
						if(gauge_prv[0][j] < 0)gauge_prv[0][j] = 0;
					}
					else{
						if(gauge_prv[0][j] < pdat->hp)gauge_prv[0][j] = pdat->hp;
					}
					hiritu = (double)gauge_prv[0][j] / (double)pdat->hpmax;
					if(hiritu<0)hiritu=0;
					vb[0].x = vb[1].x = (float)( 320.0/240.0  - 200.0/240.0*hiritu - 50.0/240.0 );
					g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
				}
				else
				{
					gauge_prv[0][j] = 0;
				}
				
			}
		}
	}
	for(j=0;j<g_battleinfo.GetNumTeam(1);j++)
	{
		switch(g_battleinfo.GetNumTeam(1)){
		case 2:turiage_y=-0.025;break;
		case 3:turiage_y=-0.075;break;
		default:turiage_y=0;
		}
		//2p側体力棒
		pdat = (GOBJECT*)battleTask->GetCharacterInfo(1,j);
		if(pdat!=NULL){
			//体力
			if(pdat->hpmax!=0){
				gauge_prv2[1][j] -= 20;
					if(gauge_prv2[1][j] < pdat->hp)gauge_prv2[1][j] = pdat->hp;
				hiritu = (double)gauge_prv2[1][j] / (double)pdat->hpmax;
				if(hiritu<0)hiritu=0;
				vb[0].x = (float)( 320.0/240.0 + 50.0/240.0 );
				vb[0].y = (float)( 0.18 - 0.03 +turiage_y +0.05*j);
				vb[1].x = (float)( 320.0/240.0 + 50.0/240.0 );
				vb[1].y = (float)( 0.18 + 0.03 +turiage_y +0.05*j);
				vb[2].x = (float)( 320.0/240.0  + 200.0/240.0*hiritu + 50.0/240.0 );
				vb[2].y = (float)( 0.18 - 0.03 +turiage_y +0.05*j);
				vb[3].x = (float)( 320.0/240.0  + 200.0/240.0*hiritu + 50.0/240.0 );
				vb[3].y = (float)( 0.18 + 0.03 +turiage_y +0.05*j);
				gauge_blue = (BYTE)(255*hiritu);
				gauge_yellow = (BYTE)(255*(1-hiritu));
				vb[2].color = vb[3].color = 0xFF000000 + gauge_yellow*256*256 + gauge_yellow*256 +gauge_blue;
				vb[0].color = vb[1].color = 0xFFAA77FF;
				for(i=0;i<4;i++){
					vb[i].z = -0.05f;
					vb[i].tu = vb[i].tv = 0;
				}
				if(j!=battleTask->GetActiveCharacterID(1)){
					vb[0].y = (float)( 0.18 - 0.01 +turiage_y +0.05*j);
					vb[1].y = (float)( 0.18 + 0.01 +turiage_y +0.05*j);
					vb[2].y = (float)( 0.18 - 0.01 +turiage_y +0.05*j);
					vb[3].y = (float)( 0.18 + 0.01 +turiage_y +0.05*j);
					vb[0].color = vb[1].color = vb[2].color = vb[3].color = 0xFF000077;
				}
				g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
			
				//減った分
				if(pdat->hp < (int)pdat->hpmax)
				{
					for(i=0;i<4;i++){
						vb[i].color = 0xFFFF0000;
					}
					gauge_prv[1][j] -= 3;
					if(pdat->hp<0){
						if(gauge_prv[1][j] < 0)gauge_prv[1][j] = 0;
					}
					else{
						if(gauge_prv[1][j] < pdat->hp)gauge_prv[1][j] = pdat->hp;
					}
					hiritu = (double)gauge_prv[1][j] / (double)pdat->hpmax;
					if(hiritu<0)hiritu=0;
					vb[0].x = vb[1].x = (float)( 320.0/240.0  + 200.0/240.0*hiritu + 50.0/240.0 );
					g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
				}
				else
				{
					gauge_prv[1][j] = 0;
				}
			}
		}
	}

	MYRECT3D rdst;
	RECT rsrc;
	rsrc.top=rsrc.left=0;
	//カオ(1p)
	for(j=0;j<g_battleinfo.GetNumTeam(0);j++){
		if(dds_face[0][j]!=NULL){
			int kx = 0, ky = 0;

			// 揺れ
			CGObject* pobj = battleTask->GetCharacterObject(0,j);
			if (pobj && pobj->siver)
			{
				kx = rand() % 16 - 8;
				ky = rand() % 16 - 8;
			}

			rsrc.right = (long)dds_face[0][j]->wg;
			rsrc.bottom = (long)dds_face[0][j]->hg;
			rdst.z = -0.03;
			rdst.left = 20.0f / 240.0f + 0.08f*j + kx / 240.0f;
			rdst.right = rdst.left + 49.0/240.0;
			rdst.top = (float)(20.0 / 240.0) + 0.065f*j + ky / 240.0f;
			rdst.bottom = rdst.top + (49.0/240.0);
			g_draw.ResetParentMatrix();
			if(j==battleTask->GetActiveCharacterID(0)){
				rdst.z = -0.04;	//戦闘中のはレイヤを一番上に
				pdat =(GOBJECT*)battleTask->GetCharacterInfo(0,j); 
				if(pdat!=NULL){
					if(pdat->hp <= 0)
						g_draw.MyBlt3D(dds_face[0][j],rsrc,rdst,0,0xFFFF0000);
					else g_draw.MyBlt3D(dds_face[0][j],rsrc,rdst,0,0xFFFFFFFF);
				}
				else g_draw.MyBlt3D(dds_face[0][j],rsrc,rdst,0,0xFFFFFFFF);
			}
			else {
				pdat =(GOBJECT*)battleTask->GetCharacterInfo(0,j); 
				if(pdat!=NULL){
					if(pdat->hp <= 0)
						g_draw.MyBlt3D(dds_face[0][j],rsrc,rdst,0,0xFFFF0000);
					else g_draw.MyBlt3D(dds_face[0][j],rsrc,rdst,0,0xFF777777);
				}
				else g_draw.MyBlt3D(dds_face[0][j],rsrc,rdst,0,0xFF777777);
			}
		}
	}
	for(j=0;j<g_battleinfo.GetNumTeam(1);j++){
		//カオ(2p)
		if(dds_face[1][j]!=NULL){
			int kx = 0, ky = 0;

			// 揺れ
			CGObject* pobj = battleTask->GetCharacterObject(1,j);
			if (pobj && pobj->siver)
			{
				kx = rand() % 16 - 8;
				ky = rand() % 16 - 8;
			}

			rsrc.right = (long)dds_face[1][j]->wg;
			rsrc.bottom = (long)dds_face[1][j]->hg;
			rdst.z = -0.03;
			rdst.left = 2.0*(320.0/240.0) - 20.0/240.0 - 0.08*j+kx/240.0;
			rdst.right = rdst.left - (49.0/240.0);
			rdst.top = 20.0f/240.0 + 0.065*j+ky/240.0;
			rdst.bottom = rdst.top + (49.0/240.0);
			g_draw.ResetParentMatrix();
			if(j==battleTask->GetActiveCharacterID(1)){
				rdst.z = -0.04;
				pdat =(GOBJECT*)battleTask->GetCharacterInfo(1,j); 
				if(pdat!=NULL){
					if(pdat->hp <= 0)
						g_draw.MyBlt3D(dds_face[1][j],rsrc,rdst,0,0xFFFF0000);
					else g_draw.MyBlt3D(dds_face[1][j],rsrc,rdst,0,0xFFFFFFFF);
				}
				else g_draw.MyBlt3D(dds_face[1][j],rsrc,rdst,0,0xFFFFFFFF);
			}
			else {
				pdat =(GOBJECT*)battleTask->GetCharacterInfo(1,j); 
				if(pdat!=NULL){
					if(pdat->hp <= 0)
						g_draw.MyBlt3D(dds_face[1][j],rsrc,rdst,0,0xFFFF0000);
					else g_draw.MyBlt3D(dds_face[1][j],rsrc,rdst,0,0xFF777777);
				}
				else g_draw.MyBlt3D(dds_face[1][j],rsrc,rdst,0,0xFF777777);
			}
		}
	}
	//ストライカーマーク
	DWORD wincount;
	//1p
	wincount = battleTask->GetStrikerCount(0);
	for(i=0;i<(int)g_config.GetStrikerCount();i++){
		if((int)wincount>i)g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_STRIKERMARK1,	320-65-20*i,65,-0.05f,0);
		else g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_STRIKERMARK2,			320-65-20*i,65,-0.05f,0);
	}
	//2p
	wincount = battleTask->GetStrikerCount(1);
	for(i=0;i<(int)g_config.GetStrikerCount();i++){
		if((int)wincount>i)g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_STRIKERMARK1,	320+65+20*i,65,-0.05f,0);
		else g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_STRIKERMARK2,			320+65+20*i,65,-0.05f,0);
	}
}

//****************************************************************************
//　パワーゲージの描画
//****************************************************************************

void CGauge::DrawPowerGauge1()
{
	GOBJECT *pdat;
	int j;
	BOOL player;

	for(j=0;j<2;j++){
		switch(g_battleinfo.GetNumTeam(j)){
		case 1:
			if(j==0)player=FALSE;
			else player=TRUE;
			pdat = (GOBJECT*)battleTask->GetCharacterInfo(j,0);
			if(pdat!=NULL){
				if(pdat->gaugetype == GOBJFLGA_POWGAUGE1)DrawPowerGaugeType1(player,0,0,0);
				else if(pdat->gaugetype == GOBJFLGA_POWGAUGE2)DrawPowerGaugeType2(player,0,0,0,TRUE);
			}
			break;
		case 2:
			if(j==0)player=FALSE;
			else player=TRUE;
			pdat = (GOBJECT*)battleTask->GetCharacterInfo(j,0);
			if(pdat!=NULL){
				if(pdat->gaugetype == GOBJFLGA_POWGAUGE1)DrawPowerGaugeType1(player,0,0,0);
				else if(pdat->gaugetype == GOBJFLGA_POWGAUGE2) DrawPowerGaugeType2(player,0,0,0,FALSE);
			}
			pdat = (GOBJECT*)battleTask->GetCharacterInfo(j,1);
			if(pdat!=NULL){
				if(pdat->gaugetype == GOBJFLGA_POWGAUGE1)DrawPowerGaugeType1(player,1,150,0);
				else if(pdat->gaugetype == GOBJFLGA_POWGAUGE2) DrawPowerGaugeType2(player,1,150,0,FALSE);
			}
			break;
		case 3:
			if(j==0)player=FALSE;
			else player=TRUE;
			pdat = (GOBJECT*)battleTask->GetCharacterInfo(j,0);
			if(pdat!=NULL){
				if(pdat->gaugetype == GOBJFLGA_POWGAUGE1)DrawPowerGaugeType1(player,0,0,-20);
				else if(pdat->gaugetype == GOBJFLGA_POWGAUGE2) DrawPowerGaugeType2(player,0,0,-20,FALSE);
			}
			pdat = (GOBJECT*)battleTask->GetCharacterInfo(j,1);
			if(pdat!=NULL){
				if(pdat->gaugetype == GOBJFLGA_POWGAUGE1)DrawPowerGaugeType1(player,1,0,10);
				else if(pdat->gaugetype == GOBJFLGA_POWGAUGE2) DrawPowerGaugeType2(player,1,0,10,FALSE);
			}
			pdat = (GOBJECT*)battleTask->GetCharacterInfo(j,2);
			if(pdat!=NULL){
				if(pdat->gaugetype == GOBJFLGA_POWGAUGE1)DrawPowerGaugeType1(player,2,150,10);
				else if(pdat->gaugetype == GOBJFLGA_POWGAUGE2) DrawPowerGaugeType2(player,2,150,10,FALSE);
			}
			break;
		}
	}
}

//交換あり
void CGauge::DrawPowerGauge2()
{
	GOBJECT *pdat;
	int j;
	BOOL player;

	for(j=0;j<2;j++){
		for(int i=0;i<(int)g_battleinfo.GetNumTeam(j);i++){	
			if(pg_draw[j][i]){
				if(j==0)player=FALSE;
				else player=TRUE;
				pdat = (GOBJECT*)battleTask->GetCharacterInfo(j,i);
				if(pdat!=NULL){
					if(pdat->gaugetype == GOBJFLGA_POWGAUGE1)DrawPowerGaugeType1(player,i,pg_dx[j][i],0);
					else if(pdat->gaugetype == GOBJFLGA_POWGAUGE2)DrawPowerGaugeType2(player,i,pg_dx[j][i],0,TRUE);
				}
			}
		}
	}
}

//ゲージ・タイプ1(複数ストック可能なゲージ)
void CGauge::DrawPowerGaugeType1(BOOL player,int pindex,int x,int y)
{
	int j;
	int num;
	GOBJECT *pdat;
	DWORD gcolor=0xFFFFFFFF;
	DWORD cell_max;
	DWORD TGTCnt = (timeGetTime()/50)%12;
	
	RECT rpg;
	rpg.top=3;
	rpg.bottom=rpg.top+15;
	rpg.left=139;

	RECT rpg_R;
	rpg_R.top=21;
	rpg_R.bottom=rpg_R.top+15;
	rpg_R.right=225;

	if(!player){
		j=0;
		//プレイヤー1側
		g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_PG2_BLANKL,x+10,y+470,0.0f,0,FALSE,FALSE,gcolor);//ワク
		pdat = (GOBJECT*)battleTask->GetCharacterInfo( j,pindex );
		if(pdat!=NULL){
			num = (int)pdat->gauge;//ゲージの整数部分
			//整数部分が増えてたら音を鳴らす
			if(num > (int)numprv[j][pindex])g_system.PlaySystemSound(SYSTEMSOUND_CHARGE);
			numprv[j][pindex] = num;
			num = (int)pgauge_prv[j][pindex];
			battleTask->DrawNumber3(pdat->gauge,x+35,y+445,0.0f);
			if(pdat->gauge == pdat->gaugemax){//ゲージマックス!!
				switch(TGTCnt){
					case 0:cell_max=CELL_PG2_MAX1;break;
					case 1:cell_max=CELL_PG2_MAX2;break;
					case 2:cell_max=CELL_PG2_MAX3;break;
					case 3:cell_max=CELL_PG2_MAX4;break;
					case 4:cell_max=CELL_PG2_MAX5;break;
					case 5:cell_max=CELL_PG2_MAX6;break;
					case 6:cell_max=CELL_PG2_MAX7;break;
					case 7:cell_max=CELL_PG2_MAX8;break;
					case 8:cell_max=CELL_PG2_MAX9;break;
					case 9:cell_max=CELL_PG2_MAX10;break;
					case 10:cell_max=CELL_PG2_MAX11;break;
					default:cell_max=CELL_PG2_MAX12;
				}
				g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),cell_max,x+45,y+455,0.0f,0,FALSE,FALSE,gcolor);
			}
			else {//棒を描く
				rpg.right = rpg.left+(int)((pgauge_prv[j][pindex] - num)*87);
				g_draw.CheckBlt(g_system.GetSystemGraphicSurface()[1],x+66,y+448,rpg,FALSE,FALSE,0,0.0f,gcolor);
			}
		}
	}
	else{
		j=1;
		x*=-1;
		//プレイヤー2側
		g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_PG2_BLANKR,x+630,y+470,0.0f,0,FALSE,FALSE,gcolor);//ワク
		pdat = (GOBJECT*)battleTask->GetCharacterInfo( j,pindex );
		if(pdat!=NULL){
			num = (int)pdat->gauge;//ゲージの整数部分
			//整数部分が増えてたら音を鳴らす
			if(num > (int)numprv[j][pindex])g_system.PlaySystemSound(SYSTEMSOUND_CHARGE);
			numprv[j][pindex] = num;
			num = (int)pgauge_prv[j][pindex];
			battleTask->DrawNumber7(pdat->gauge,x+600,y+445,0.0f);
			if(pdat->gauge == pdat->gaugemax){//ゲージマックス!!
				switch(TGTCnt){
					case 0:cell_max=CELL_PG2_MAX_R1;break;
					case 1:cell_max=CELL_PG2_MAX_R2;break;
					case 2:cell_max=CELL_PG2_MAX_R3;break;
					case 3:cell_max=CELL_PG2_MAX_R4;break;
					case 4:cell_max=CELL_PG2_MAX_R5;break;
					case 5:cell_max=CELL_PG2_MAX_R6;break;
					case 6:cell_max=CELL_PG2_MAX_R7;break;
					case 7:cell_max=CELL_PG2_MAX_R8;break;
					case 8:cell_max=CELL_PG2_MAX_R9;break;
					case 9:cell_max=CELL_PG2_MAX_R10;break;
					case 10:cell_max=CELL_PG2_MAX_R11;break;
					default:cell_max=CELL_PG2_MAX_R12;
				}
				g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),cell_max,x+487,y+455,0.0f,0,FALSE,FALSE,gcolor);
			}
			else {//棒を描く
				rpg_R.left = rpg_R.right-(int)((pgauge_prv[j][pindex] - num)*87);//*横幅
				g_draw.CheckBlt(g_system.GetSystemGraphicSurface()[1],x+575-(rpg_R.right-rpg_R.left),y+448,rpg_R,FALSE,FALSE,0,0.0f,gcolor);
			}
		}
	}
}

//ゲージ・タイプ2（0-100％なゲージ）
void CGauge::DrawPowerGaugeType2(BOOL player,int pindex,int x,int y,BOOL pow)
{
	int j;
	int num;
	GOBJECT *pdat;
	DWORD gcolor=0xFFFFFFFF;
	
	RECT rpg;
	rpg.top=3;
	rpg.bottom=rpg.top+15;
	rpg.left=3;

	RECT rpg_R;
	rpg_R.top=21;
	rpg_R.bottom=rpg_R.top+15;
	rpg_R.right=135;

	DWORD cell_max;
	DWORD TGTCnt = (timeGetTime()/50)%12;

	if(!player){
		j=0;
		//プレイヤー1側
		if(pow)g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_PG1_POW,x+18,y+455,0.0f,0,FALSE,FALSE,gcolor);//「パウッ」
		else x-=40;
		pdat = (GOBJECT*)battleTask->GetCharacterInfo( j,pindex );
		if(pdat!=NULL){
			num = (int)pdat->gauge;
			if(num > (int)numprv[j][pindex])g_system.PlaySystemSound(SYSTEMSOUND_CHARGE);
			numprv[j][pindex] = num;
			if(pdat->gauge == pdat->gaugemax){//ゲージマックス
				switch(TGTCnt){
					case 0:cell_max=CELL_PG1_MAX1;break;
					case 1:cell_max=CELL_PG1_MAX2;break;
					case 2:cell_max=CELL_PG1_MAX3;break;
					case 3:cell_max=CELL_PG1_MAX4;break;
					case 4:cell_max=CELL_PG1_MAX5;break;
					case 5:cell_max=CELL_PG1_MAX6;break;
					case 6:cell_max=CELL_PG1_MAX7;break;
					case 7:cell_max=CELL_PG1_MAX8;break;
					case 8:cell_max=CELL_PG1_MAX9;break;
					case 9:cell_max=CELL_PG1_MAX10;break;
					case 10:cell_max=CELL_PG1_MAX11;break;
					default:cell_max=CELL_PG1_MAX12;
				}
				g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),cell_max,x+45,y+455,0.0f,0,FALSE,FALSE,gcolor);
			}
			else {
				g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_PG1_BLANK,x+45,y+455,0.0f,0,FALSE,FALSE,gcolor);//ワク
				rpg.right = rpg.left+(int)((pgauge_prv[j][pindex])*132);//*横幅
				g_draw.CheckBlt(g_system.GetSystemGraphicSurface()[1],x+48,y+448,rpg,FALSE,FALSE,0,0.0f,gcolor);//棒
				battleTask->DrawNumber4(pgauge_prv[j][pindex],x+97,y+454,0.0f);//パーセンテージ
			}
		}
	}
	else{
		j=1;
		x*=-1;
		//プレイヤー2側
		if(pow)g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_PG_POW_R,x+612,y+455,0.0f,0,FALSE,FALSE,gcolor);//「パウッ」
		else x+=40;
		pdat = (GOBJECT*)battleTask->GetCharacterInfo( j,pindex );
		if(pdat!=NULL){
			num = (int)pdat->gauge;
			if(num > (int)numprv[j][pindex])g_system.PlaySystemSound(SYSTEMSOUND_CHARGE);
			numprv[j][pindex] = num;
			if(pdat->gauge == pdat->gaugemax){//ゲージマックス
				switch(TGTCnt){
					case 0:cell_max=CELL_PG1_MAX_R1;break;
					case 1:cell_max=CELL_PG1_MAX_R2;break;
					case 2:cell_max=CELL_PG1_MAX_R3;break;
					case 3:cell_max=CELL_PG1_MAX_R4;break;
					case 4:cell_max=CELL_PG1_MAX_R5;break;
					case 5:cell_max=CELL_PG1_MAX_R6;break;
					case 6:cell_max=CELL_PG1_MAX_R7;break;
					case 7:cell_max=CELL_PG1_MAX_R8;break;
					case 8:cell_max=CELL_PG1_MAX_R9;break;
					case 9:cell_max=CELL_PG1_MAX_R10;break;
					case 10:cell_max=CELL_PG1_MAX_R11;break;
					default:cell_max=CELL_PG1_MAX_R12;
				}
				g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),cell_max,x+452,y+455,0.0f,0,FALSE,FALSE,gcolor);
			}
			else {
				g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_PG1_BLANK_R,x+452,y+455,0.0f,0,FALSE,FALSE,gcolor);//ワク
				rpg_R.left = rpg_R.right-(int)((pgauge_prv[j][pindex])*132);
				g_draw.CheckBlt(g_system.GetSystemGraphicSurface()[1],x+588-(rpg_R.right-rpg_R.left),y+448,rpg_R,FALSE,FALSE,0,0.0f,gcolor);//棒
				battleTask->DrawNumber4(pgauge_prv[j][pindex],x+505,y+454,0.0f);//パーセンテージ
			}
		}
	}
}

void CGauge::DrawHPwaku1(){	//HPの枠	coop
	DWORD hpw;

	//TEAM1
	switch(g_battleinfo.GetNumTeam(0)){
		case 2:hpw=CELL_HPWAKU_L2;break;
		case 3:hpw=CELL_HPWAKU_L3;break;
		default:hpw=CELL_HPWAKU_L1;
	}
	g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),hpw,45,0,0.0f,0,FALSE,FALSE,0xFFFFFFFF);

	//TEAM2
	switch(g_battleinfo.GetNumTeam(1)){
		case 2:hpw=CELL_HPWAKU_R2;break;
		case 3:hpw=CELL_HPWAKU_R3;break;
		default:hpw=CELL_HPWAKU_R1;
	}
	g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),hpw,400,0,0.0f,0,FALSE,FALSE,0xFFFFFFFF);

}

void CGauge::DrawHPwaku2(){	//marvel kof
	DWORD hpw;

	switch(g_battleinfo.GetNumTeam(0)){
		case 2:hpw=CELL_HPWAKU2_L2;break;
		case 3:hpw=CELL_HPWAKU2_L3;break;
		default:hpw=CELL_HPWAKU2_L1;
	}
	g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),hpw,45,0,0.0f,0,FALSE,FALSE,0xFFFFFFFF);

	//TEAM2
	switch(g_battleinfo.GetNumTeam(1)){
		case 2:hpw=CELL_HPWAKU2_R2;break;
		case 3:hpw=CELL_HPWAKU2_R3;break;
		default:hpw=CELL_HPWAKU2_R1;
	}
		g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),hpw,400,0,0.0f,0,FALSE,FALSE,0xFFFFFFFF);

}

void CGauge::DrawHPG1(){	//coop
	DWORD j;
	int i;
	int y;
	GOBJECT *pdat;
	double hiritu;

	for(j=0;j<g_battleinfo.GetNumTeam(0);j++){
		pdat = (GOBJECT*)battleTask->GetCharacterInfo(0,j);
		if(pdat!=NULL){
			//体力
			if(pdat->hpmax!=0){
				gauge_prv2[0][j] -= (gauge_prv2[0][j] - pdat->hp) / 2.718282;
				if(gauge_prv2[0][j] < pdat->hp)gauge_prv2[0][j] = pdat->hp;
				hiritu = (double)gauge_prv2[0][j] / (double)pdat->hpmax;
				if(hiritu<0)hiritu=0;
				RECT hpg;
				hpg.right=203;
				hpg.left=hpg.right-200*hiritu;
				switch(g_battleinfo.GetNumTeam(0)){
					case 2:
						y=30+16*j;
						hpg.top=23+17*j;
						hpg.bottom=hpg.top+16;
						break;
					case 3:
						y=25+14*j;
						hpg.top=57+15*j;
						hpg.bottom=hpg.top+14;
						break;
					default:
						y=36;
						hpg.top=3;
						hpg.bottom=hpg.top+19;
				}
				g_draw.CheckBlt(g_system.GetSystemGraphicSurface()[2],272-200*hiritu,y,hpg,FALSE,FALSE,0,-0.05f,0xFFFFFFFF);
			}
			else{
				gauge_prv[0][j] = 0;
			}
		}
	}

	//TEAM2
	for(j=0;j<g_battleinfo.GetNumTeam(1);j++){
		pdat = (GOBJECT*)battleTask->GetCharacterInfo(1,j);
		if(pdat!=NULL){
			//体力
			if(pdat->hpmax!=0){
				gauge_prv2[1][j] -= (gauge_prv2[1][j] - pdat->hp) / 2.718282;
				if(gauge_prv2[1][j] < pdat->hp)gauge_prv2[1][j] = pdat->hp;
				hiritu = (double)gauge_prv2[1][j] / (double)pdat->hpmax;
				if(hiritu<0)hiritu=0;
				RECT hpg;
				hpg.left=206;
				hpg.right=hpg.left+200*hiritu;
				switch(g_battleinfo.GetNumTeam(1)){
					case 2:
						y=30+16*j;
						hpg.top=23+17*j;
						hpg.bottom=hpg.top+16;
						break;
					case 3:
						y=25+14*j;
						hpg.top=57+15*j;
						hpg.bottom=hpg.top+14;
						break;
					default:
						y=36;
						hpg.top=3;
						hpg.bottom=hpg.top+19;
				}
				g_draw.CheckBlt(g_system.GetSystemGraphicSurface()[2],367,y,hpg,FALSE,FALSE,0,-0.05f,0xFFFFFFFF);
			}
			else{
				gauge_prv[1][j] = 0;
			}
		}
	}


	MYRECT3D rdst;
	RECT rsrc;
	rsrc.top=rsrc.left=0;
	//カオ(1p)
	for(j=0;j<g_battleinfo.GetNumTeam(0);j++){
		if(dds_face[0][j]!=NULL){
			int kx = 0, ky = 0;
			pdat =(GOBJECT*)battleTask->GetCharacterInfo(0,j); 

			// 揺れ
			CGObject* pobj = battleTask->GetCharacterObject(0,j);
			if (pobj && pobj->siver)
			{
				kx = rand() % 16 - 8;
				ky = rand() % 16 - 8;
			}

			rsrc.right = (long)dds_face[0][j]->wg;
			rsrc.bottom = (long)dds_face[0][j]->hg;
			rdst.z = -0.04f;
			g_draw.ResetParentMatrix();
			if(g_battleinfo.GetNumTeam(0) == 3){	//30x30
				rdst.left = 41.0f/240.0f + kx/240.0f;
				rdst.right = rdst.left + 30.0f/240.0f;
				rdst.top = 4.0f/240.0f + 0.12f*j + ky/240.0f;
				rdst.bottom = rdst.top + 29.0f/240.0f;
			}
			else if(g_battleinfo.GetNumTeam(0) == 2){	//40x40
				rdst.left = 31.0f/240.0f + kx/240.0f;
				rdst.right = rdst.left + 40.0f/240.0f;
				rdst.top = 6.5f/240.0f + 0.165f*j + ky/240.0f;
				rdst.bottom = rdst.top + 39.0f/240.0f;
			}
			else{
				rdst.left = 22.0f/240.0f + kx/240.0f;
				rdst.right = rdst.left + 49.0f/240.0f;
				rdst.top = 20.0f/240.0f + ky/240.0f;
				rdst.bottom = rdst.top + 49.0f/240.0f;
			}
			if(pdat->hp <= 0)
				g_draw.MyBlt3D(dds_face[0][j],rsrc,rdst,0,0xFFFF0000);
			else g_draw.MyBlt3D(dds_face[0][j],rsrc,rdst,0,0xFFFFFFFF);
		}
	}
	for(j=0;j<g_battleinfo.GetNumTeam(1);j++){
		//カオ(2p)
		if(dds_face[1][j]!=NULL){
			int kx = 0, ky = 0;
			pdat =(GOBJECT*)battleTask->GetCharacterInfo(1,j); 

			// 揺れ
			CGObject* pobj = battleTask->GetCharacterObject(1,j);
			if (pobj && pobj->siver)
			{
				kx = rand() % 16 - 8;
				ky = rand() % 16 - 8;
			}

			rsrc.right = (long)dds_face[1][j]->wg;
			rsrc.bottom = (long)dds_face[1][j]->hg;
			rdst.z = -0.04f;
			g_draw.ResetParentMatrix();
			if(g_battleinfo.GetNumTeam(1) == 3){
				rdst.left = 598.0f/240.0f + kx/240.0f;
				rdst.right = rdst.left - 30.0f/240.0f;
				rdst.top = 4.0f/240.0f + 0.12f*j + ky/240.0f;
				rdst.bottom = rdst.top + 29.0f/240.0f;
			}
			else if(g_battleinfo.GetNumTeam(1) == 2){
				rdst.left = 608.0f/240.0f + kx/240.0f;
				rdst.right = rdst.left - 40.0f/240.0f;
				rdst.top = 6.5f/240.0f + 0.165f*j + ky/240.0f;
				rdst.bottom = rdst.top + 39.0f/240.0f;
			}
			else{
				rdst.left = 617.0f/240.0f + kx/240.0f;
				rdst.right = rdst.left - 49.0f/240.0f;
				rdst.top = 20.0f/240.0f + ky/240.0f;
				rdst.bottom = rdst.top + 49.0f/240.0f;
			}
			if(pdat->hp <= 0)
				g_draw.MyBlt3D(dds_face[1][j],rsrc,rdst,0,0xFFFF0000);
			else g_draw.MyBlt3D(dds_face[1][j],rsrc,rdst,0,0xFFFFFFFF);
		}
	}
	//勝利マーク
	DWORD wincount;
	//1p
	wincount = battleTask->GetWinCount(0);
	for(i=0;i<(int)g_config.GetMaxPoint();i++){
		if((int)wincount>i)g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_WINMARK,	320-65-30*i,75,-0.05f,0);
		else g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_WINMARK2,			320-65-30*i,75,-0.05f,0);
	}
	//2p
	wincount = battleTask->GetWinCount(1);
	for(i=0;i<(int)g_config.GetMaxPoint();i++){
		if((int)wincount>i)g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_WINMARK,	320+65+30*i,75,-0.05f,0);
		else g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_WINMARK2,			320+65+30*i,75,-0.05f,0);
	}

}

void CGauge::DrawHPG2(){	//marvel kof
	DWORD j;
	int i;
	int y;
	GOBJECT *pdat;
	double hiritu;

	for(j=0;j<g_battleinfo.GetNumTeam(0);j++){
		pdat = (GOBJECT*)battleTask->GetCharacterInfo(0,j);
		if(pdat!=NULL){
			//体力
			if(pdat->hpmax!=0){
				gauge_prv2[0][j] -= 20;
				if(gauge_prv2[0][j] < pdat->hp)gauge_prv2[0][j] = pdat->hp;
				hiritu = (double)gauge_prv2[0][j] / (double)pdat->hpmax;
				if(hiritu<0)hiritu=0;
				RECT hpg;
				hpg.right=203;
				hpg.left=hpg.right-200*hiritu;
				switch(g_battleinfo.GetNumTeam(0)){
					case 2:
						if(j==battleTask->GetActiveCharacterID(0)){
						y=29;
						hpg.top=102+20*j;
						hpg.bottom=hpg.top+19;
						}
						else{
						y=48;
						hpg.top=162+15*j;
						hpg.bottom=hpg.top+14;
						}
						break;
					case 3:
						if(j==battleTask->GetActiveCharacterID(0)){
							y=21;
							hpg.top=102+20*j;
							hpg.bottom=hpg.top+19;
						}
						else{
							hpg.top=162+15*j;
							hpg.bottom=hpg.top+14;
							if(battleTask->GetCharacterInfo(0,0)->hp <= 0 || battleTask->GetCharacterInfo(0,1)->hp <= 0 || battleTask->GetCharacterInfo(0,2)->hp <= 0)
								y=40;
							else{
								switch(battleTask->GetActiveCharacterID(0)){
									case 0:y=40+14*(j-1);break;
									case 1:y=40+7*(-j+2);break;
									default:y=40+14*j;
								}
							}
						}
						break;
					default:
						y=36;
						hpg.top=102;
						hpg.bottom=hpg.top+19;
				}
				g_draw.CheckBlt(g_system.GetSystemGraphicSurface()[2],272-200*hiritu,y,hpg,FALSE,FALSE,0,-0.05f,0xFFFFFFFF);
			}
			else{
				gauge_prv[0][j] = 0;
			}
		}
	}

	//TEAM2
	for(j=0;j<g_battleinfo.GetNumTeam(1);j++){
		pdat = (GOBJECT*)battleTask->GetCharacterInfo(1,j);
		if(pdat!=NULL){
			//体力
			if(pdat->hpmax!=0){
				gauge_prv2[1][j] -= 20;
				if(gauge_prv2[1][j] < pdat->hp)gauge_prv2[1][j] = pdat->hp;
				hiritu = (double)gauge_prv2[1][j] / (double)pdat->hpmax;
				if(hiritu<0)hiritu=0;
				RECT hpg;
				hpg.left=206;
				hpg.right=hpg.left+200*hiritu;
				switch(g_battleinfo.GetNumTeam(1)){
					case 2:
						if(j==battleTask->GetActiveCharacterID(1)){
						y=29;
						hpg.top=102+20*j;
						hpg.bottom=hpg.top+19;
						}
						else{
						y=48;
						hpg.top=162+15*j;
						hpg.bottom=hpg.top+14;
						}
						break;
					case 3:
						if(j==battleTask->GetActiveCharacterID(1)){
							y=21;
							hpg.top=102+20*j;
							hpg.bottom=hpg.top+19;
						}
						else{
							hpg.top=162+15*j;
							hpg.bottom=hpg.top+14;
							if(battleTask->GetCharacterInfo(1,0)->hp <= 0 || battleTask->GetCharacterInfo(1,1)->hp <= 0 || battleTask->GetCharacterInfo(1,2)->hp <= 0)
								y=40;
							else{
								switch(battleTask->GetActiveCharacterID(1)){
									case 0:y=40+14*(j-1);break;
									case 1:y=40+7*(-j+2);break;
									default:y=40+14*j;
								}
							}
						}
						break;
					default:
						y=36;
						hpg.top=102;
						hpg.bottom=hpg.top+19;
				}
				g_draw.CheckBlt(g_system.GetSystemGraphicSurface()[2],367,y,hpg,FALSE,FALSE,0,-0.05f,0xFFFFFFFF);
			}
			else{
				gauge_prv[1][j] = 0;
			}
		}
	}

	MYRECT3D rdst;
	RECT rsrc;
	rsrc.top=rsrc.left=0;
	//カオ(1p)
	for(j=0;j<g_battleinfo.GetNumTeam(0);j++){
		if(dds_face[0][j]!=NULL){
			int kx = 0, ky = 0;
			pdat =(GOBJECT*)battleTask->GetCharacterInfo(0,j); 

			// 揺れ
			CGObject* pobj = battleTask->GetCharacterObject(0,j);
			if (pobj && pobj->siver)
			{
				kx = rand() % 16 - 8;
				ky = rand() % 16 - 8;
			}

			rsrc.right = (long)dds_face[0][j]->wg;
			rsrc.bottom = (long)dds_face[0][j]->hg;
			rdst.z = -0.04f;
			g_draw.ResetParentMatrix();
			if(j==battleTask->GetActiveCharacterID(0)){	//戦闘中のは一番上で原寸表示
				rdst.left = 22.0f/240.0f + kx/240.0f;
				rdst.right = rdst.left + 49.0f/240.0f;
				rdst.top = 20.0f/240.0f + ky/240.0f;
				rdst.bottom = rdst.top + 49.0f/240.0f;
			}
			else {
				rdst.top = 69.0f/240.0f;
				if(g_battleinfo.GetNumTeam(0) == 3){	//3人なら控え2人は25x25
					switch(battleTask->GetActiveCharacterID(0)){
						case 0:
							if(battleTask->GetCharacterInfo(0,1)->hp <= 0)
								rdst.left = 22.0f/240.0f + 0.10f*(2-j);	//死んだ方は右側に
							else
								rdst.left = 22.0f/240.0f + 0.10f*(j-1);
						break;
						case 1:
							if(battleTask->GetCharacterInfo(0,2)->hp <= 0)
								rdst.left = 22.0f/240.0f + 0.05f*j;
							else
								rdst.left = 22.0f/240.0f + 0.05f*(2-j);
						break;
						default:
							if(battleTask->GetCharacterInfo(0,0)->hp <= 0)
								rdst.left = 22.0f/240.0f + 0.10f*(1-j);
							else
								rdst.left = 22.0f/240.0f + 0.10f*j;
					}
					rdst.bottom = rdst.top + 25.0f/240.0f;
					rdst.right = rdst.left + 25.0f/240.0f;
				}
				else{	//2人なら控えは35x35
					rdst.left = 22.0f/240.0f;
					rdst.bottom = rdst.top + 35.0f/240.0f;
					rdst.right = rdst.left + 34.0f/240.0f;
				}
			}
			if(pdat->hp <= 0)
				g_draw.MyBlt3D(dds_face[0][j],rsrc,rdst,0,0xFFFF0000);
			else g_draw.MyBlt3D(dds_face[0][j],rsrc,rdst,0,0xFFFFFFFF);
		}
	}
	for(j=0;j<g_battleinfo.GetNumTeam(1);j++){
		//カオ(2p)
		if(dds_face[1][j]!=NULL){
			int kx = 0, ky = 0;
			pdat =(GOBJECT*)battleTask->GetCharacterInfo(1,j); 

			// 揺れ
			CGObject* pobj = battleTask->GetCharacterObject(1,j);
			if (pobj && pobj->siver)
			{
				kx = rand() % 16 - 8;
				ky = rand() % 16 - 8;
			}

			rsrc.right = (long)dds_face[1][j]->wg;
			rsrc.bottom = (long)dds_face[1][j]->hg;
			rdst.z = -0.04f;
			g_draw.ResetParentMatrix();
			if(j==battleTask->GetActiveCharacterID(1)){
				rdst.left = 617.0f/240.0f + kx/240.0f;
				rdst.right = rdst.left - 49.0f/240.0f;
				rdst.top = 20.0f/240.0f + ky/240.0f;
				rdst.bottom = rdst.top + 49.0f/240.0f;
			}
			else {
				rdst.top = 69.0f/240.0f;
				if(g_battleinfo.GetNumTeam(1) == 3){
					switch(battleTask->GetActiveCharacterID(1)){
						case 0:
							if(battleTask->GetCharacterInfo(1,1)->hp <= 0)
								rdst.left = 617.0f/240.0f - 0.10f*(2-j);
							else
								rdst.left = 617.0f/240.0f - 0.10f*(j-1);
						break;
						case 1:
							if(battleTask->GetCharacterInfo(1,2)->hp <= 0)
								rdst.left = 617.0f/240.0f - 0.05f*j;
							else
								rdst.left = 617.0f/240.0f - 0.05f*(2-j);
						break;
						default:
							if(battleTask->GetCharacterInfo(1,0)->hp <= 0)
								rdst.left = 617.0f/240.0f - 0.10f*(1-j);
							else
								rdst.left = 617.0f/240.0f - 0.10f*j;
					}
					rdst.bottom = rdst.top + 25.0f/240.0f;
					rdst.right = rdst.left - 25.0f/240.0f;
				}
				else{
					rdst.left = 617.0f/240.0f;
					rdst.bottom = rdst.top + 35.0f/240.0f;
					rdst.right = rdst.left - 34.0f/240.0f;
				}
			}
			if(pdat->hp <= 0)
				g_draw.MyBlt3D(dds_face[1][j],rsrc,rdst,0,0xFFFF0000);
			else g_draw.MyBlt3D(dds_face[1][j],rsrc,rdst,0,0xFFFFFFFF);
		}
	}
	//ストライカーマーク
	DWORD wincount;

	//生存者が1人だけなら非表示　上手い書き方が思いつきませんでしたorz
	int q;
	switch(g_battleinfo.GetNumTeam(0)){
		case 1:
			q=0;
		break;
		case 2:
			if(battleTask->GetCharacterInfo(0,0)->hp <= 0 || battleTask->GetCharacterInfo(0,1)->hp <= 0)
				q=0;
			else
				q=1;
		break;
		default:
			if(battleTask->GetCharacterInfo(0,0)->hp <= 0 && battleTask->GetCharacterInfo(0,1)->hp <= 0)
				q=0;
			else if(battleTask->GetCharacterInfo(0,1)->hp <= 0 && battleTask->GetCharacterInfo(0,2)->hp <= 0)
				q=0;
			else if(battleTask->GetCharacterInfo(0,0)->hp <= 0 && battleTask->GetCharacterInfo(0,2)->hp <= 0)
				q=0;
			else
				q=1;
	}

	//1p
	if(q == 1){
		wincount = battleTask->GetStrikerCount(0);
		for(i=0;i<(int)g_config.GetStrikerCount();i++){
			if((int)wincount>i)g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_STRIKERMARK1,	320-65-20*i,75,-0.05f,0);
			else g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_STRIKERMARK2,			320-65-20*i,75,-0.05f,0);
		}
	}

	switch(g_battleinfo.GetNumTeam(1)){
		case 1:
			q=0;
		break;
		case 2:
			if(battleTask->GetCharacterInfo(1,0)->hp <= 0 || battleTask->GetCharacterInfo(1,1)->hp <= 0)
				q=0;
			else
				q=1;
		break;
		default:
			if(battleTask->GetCharacterInfo(1,0)->hp <= 0 && battleTask->GetCharacterInfo(1,1)->hp <= 0)
				q=0;
			else if(battleTask->GetCharacterInfo(1,1)->hp <= 0 && battleTask->GetCharacterInfo(1,2)->hp <= 0)
				q=0;
			else if(battleTask->GetCharacterInfo(1,0)->hp <= 0 && battleTask->GetCharacterInfo(1,2)->hp <= 0)
				q=0;
			else
				q=1;
	}

	//2p
	if(q == 1){
		wincount = battleTask->GetStrikerCount(1);
		for(i=0;i<(int)g_config.GetStrikerCount();i++){
			if((int)wincount>i)g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_STRIKERMARK1,	320+65+20*i,75,-0.05f,0);
			else g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_STRIKERMARK2,			320+65+20*i,75,-0.05f,0);
		}
	}
}
