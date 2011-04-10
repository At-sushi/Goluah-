
/*===================================================================

	�̗̓Q�[�W
	�L�����N�^��E�����J�E���g�EFPS
	�p���[�Q�[�W

	������A�퓬���̃V�X�e�����`��֌W

=====================================================================*/

#include "stdafx.h"
#include "define_macro.h"
#include "gauge.h"
#include "global.h"
#include "gobject.h"


/*-------------------------------------------------------------------
	�����j��
---------------------------------------------------------------------*/
CGauge::CGauge()
{
	for(int i=0;i<2;i++){
		for(int j=0;j<3;j++){
			dds_face[i][j]=NULL;
		}
	}
}

void CGauge::CleanUp()//�J������K�v�̂�����̂� �J������
{
	for(int i=0;i<2;i++){
		for(int j=0;j<3;j++){
			RELSURFACE(dds_face[i][j]);
		}
	}
}


/*-------------------------------------------------------------------
	������
---------------------------------------------------------------------*/
void CGauge::Initialize()
{
	CleanUp();//�܂��͈ȑO�̃f�[�^���N���A

	battleTask = dynamic_cast<CBattleTaskBase*>( g_system.GetCurrentMainTask() );
	if(!battleTask)throw;

//	DWORD num_teammember = g_battleinfo.GetNumTeam();

	//�ϐ��̏�����
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
			pg_draw[i][j]=FALSE;//�p���[�Q�[�W��`�悷�邩�ǂ���
			pg_dx[i][j]=-200;//�p���[�Q�[�W�̂��炵��
		}
	}

	//�J�I�̓ǂݍ���
	char filename[256],palname[256];
	for(i=0;i<2;i++){
		for(j=0;j<g_battleinfo.GetNumTeam(i);j++){
			sprintf(filename,"%s\\face1",g_charlist.GetCharacterDir(g_battleinfo.GetCharacter(i,j)));
			sprintf(palname,"%s\\pal%d",g_charlist.GetCharacterDir(g_battleinfo.GetCharacter(i,j)),g_battleinfo.GetColor(i,j));
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
	�Q�[�W�́A�퓬�I�u�W�F�N�g�Ƃ��Ẵ��b�Z�[�W�n���h��
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
	�t���[�����Ƃ�Update����
---------------------------------------------------------------------*/
void CGauge::Action()
{
	flip_counter++;
	if(flip_counter>50)flip_counter=50;

	int i,j;
	GOBJECT *pdat;

	
	for(j=0;j<2;j++){
		for(i=0;i<(int)g_battleinfo.GetNumTeam(j);i++){
			//prv �̍X�V
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
			//�p���[�Q�[�W�̈ړ�
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
	�`��
---------------------------------------------------------------------*/
void CGauge::Draw()
{
	//�S�̘̂g(?)
	battleTask->SetTransform(FALSE);
	g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_HPGWAKU1,320,43,-0.05f,0);
	g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_HPGWAKU2,320,40,-0.04f,0);
	//���Ԑ���orFPS
	if(!g_system.sw_showfps){
		if(battleTask->limittime >= 0)
			battleTask->DrawNumber(battleTask->limittime,310,45,FALSE,-0.055f);
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
	case TAISENKEISIKI_GOCYAMAZE://��������܂�
		DrawHPGauge1();
		DrawPowerGauge1();
		break;
	default:
		DrawHPGauge2();
		DrawPowerGauge2();
	}
	
	battleTask->SetTransform(TRUE);
}

// private **************************************************************
//�@�̗̓Q�[�W�`��
//***********************************************************************

void CGauge::DrawHPGauge1()//�̗̓Q�[�W�̕`��iTAISENKEISIKI_GOCYAMAZE�j
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

	//�����ۂ��w�i
	vb[0].x = (float)( 320.0/240.0 - 50.0/240.0 );
	vb[0].y = (float)( 0.172 - 0.04 - 0.02*(g_battleinfo.GetNumTeam(0)-1));
	vb[1].x = (float)( 320.0/240.0 - 50.0/240.0 );
	vb[1].y = (float)( 0.172 + 0.04 + 0.02*(g_battleinfo.GetNumTeam(0)-1) );
	vb[2].x = (float)( 320.0/240.0  - 230.0/240.0 - 50.0/240.0 );
	vb[2].y = (float)( 0.172 - 0.04 - 0.02*(g_battleinfo.GetNumTeam(0)-1));
	vb[3].x = (float)( 320.0/240.0  - 230.0/240.0 - 50.0/240.0 );
	vb[3].y = (float)( 0.172 + 0.04 + 0.02*(g_battleinfo.GetNumTeam(0)-1));
	vb[0].color = vb[1].color = 0xDD000000;
	vb[2].color = vb[3].color = 0x00000000;
	for(i=0;i<4;i++){
		vb[i].z = -0.04f;
		vb[i].tu = vb[i].tv = 0;
	}
	g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
	//�����ۂ��w�i(2p)
	vb[0].x = (float)( 320.0/240.0 + 50.0/240.0 );
	vb[0].y = (float)( 0.172 - 0.04 - 0.02*(g_battleinfo.GetNumTeam(1)-1));
	vb[1].x = (float)( 320.0/240.0 + 50.0/240.0 );
	vb[1].y = (float)( 0.172 + 0.04 + 0.02*(g_battleinfo.GetNumTeam(1)-1));
	vb[2].x = (float)( 320.0/240.0  + 230.0/240.0 + 50.0/240.0 );
	vb[2].y = (float)( 0.172 - 0.04 - 0.02*(g_battleinfo.GetNumTeam(1)-1));
	vb[3].x = (float)( 320.0/240.0  + 230.0/240.0 + 50.0/240.0 );
	vb[3].y = (float)( 0.172 + 0.04 + 0.02*(g_battleinfo.GetNumTeam(1)-1));
	vb[0].color = vb[1].color = 0xDD000000;
	vb[2].color = vb[3].color = 0x00000000;
			for(i=0;i<4;i++){
				vb[i].z = -0.04f;
				vb[i].tu = vb[i].tv = 0;
			}
			g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));

	BYTE gauge_blue,gauge_yellow;
	
	double turiage_y;

	for(j=0;j<g_battleinfo.GetNumTeam(0);j++)
	{
		switch(g_battleinfo.GetNumTeam(0)){
		case 2:turiage_y=-0.025;break;
		case 3:turiage_y=-0.075;break;
		default:turiage_y=0;
		}
		//1p���̗͖_
		pdat = (GOBJECT*)battleTask->GetCharacterInfo(0,j);
		if(pdat!=NULL){
			//�̗�
			if(pdat->hpmax!=0){
				gauge_prv2[0][j] -= 20;
					if(gauge_prv2[0][j] < pdat->hp)gauge_prv2[0][j] = pdat->hp;
				hiritu = (double)gauge_prv2[0][j] / (double)pdat->hpmax;
				if(hiritu<0)hiritu=0;
				vb[0].x = (float)( 320.0/240.0 - 50.0/240.0 );
				vb[0].y = (float)( 0.18 - 0.02 +turiage_y +0.05*j);
				vb[1].x = (float)( 320.0/240.0 - 50.0/240.0 );
				vb[1].y = (float)( 0.18 + 0.02 +turiage_y +0.05*j);
				vb[2].x = (float)( 320.0/240.0  - 210.0/240.0*hiritu - 50.0/240.0 );
				vb[2].y = (float)( 0.18 - 0.02 +turiage_y +0.05*j);
				vb[3].x = (float)( 320.0/240.0  - 210.0/240.0*hiritu - 50.0/240.0 );
				vb[3].y = (float)( 0.18 + 0.02 +turiage_y +0.05*j);
				gauge_blue = (BYTE)(255*hiritu);
				gauge_yellow = (BYTE)(255*(1-hiritu));
				vb[2].color = vb[3].color = 0xFF000000 + gauge_yellow*256*256 + gauge_yellow*256 +gauge_blue;
				vb[0].color = vb[1].color = 0xFFAA77FF;
				for(i=0;i<4;i++){
					vb[i].z = -0.05f;
					vb[i].tu = vb[i].tv = 0;
				}
				g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
			
				//��������
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
					vb[0].x = vb[1].x = (float)( 320.0/240.0  - 210.0/240.0*hiritu - 50.0/240.0 );
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
		//2p���̗͖_
		pdat = (GOBJECT*)battleTask->GetCharacterInfo(1,j);
		if(pdat!=NULL){
			//�̗�
			if(pdat->hpmax!=0){
				gauge_prv2[1][j] -= 20;
					if(gauge_prv2[1][j] < pdat->hp)gauge_prv2[1][j] = pdat->hp;
				hiritu = (double)gauge_prv2[1][j] / (double)pdat->hpmax;
				if(hiritu<0)hiritu=0;
				vb[0].x = (float)( 320.0/240.0 + 50.0/240.0 );
				vb[0].y = (float)( 0.18 - 0.02 +turiage_y +0.05*j);
				vb[1].x = (float)( 320.0/240.0 + 50.0/240.0 );
				vb[1].y = (float)( 0.18 + 0.02 +turiage_y +0.05*j);
				vb[2].x = (float)( 320.0/240.0  + 210.0/240.0*hiritu + 50.0/240.0 );
				vb[2].y = (float)( 0.18 - 0.02 +turiage_y +0.05*j);
				vb[3].x = (float)( 320.0/240.0  + 210.0/240.0*hiritu + 50.0/240.0 );
				vb[3].y = (float)( 0.18 + 0.02 +turiage_y +0.05*j);
				gauge_blue = (BYTE)(255*hiritu);
				gauge_yellow = (BYTE)(255*(1-hiritu));
				vb[2].color = vb[3].color = 0xFF000000 + gauge_yellow*256*256 + gauge_yellow*256 +gauge_blue;
				vb[0].color = vb[1].color = 0xFFAA77FF;
				for(i=0;i<4;i++){
					vb[i].z = -0.05f;
					vb[i].tu = vb[i].tv = 0;
				}
				g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
			
				//��������
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
					vb[0].x = vb[1].x = (float)( 320.0/240.0  + 210.0/240.0*hiritu + 50.0/240.0 );
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
	//�J�I(1p)
	for(j=0;j<g_battleinfo.GetNumTeam(0);j++){
		if(dds_face[0][j]!=NULL){
			int kx = 0, ky = 0;

			// �h��
			CGObject* pobj = battleTask->GetCharacterObject(0,j);
			if (pobj && pobj->siver)
			{
				kx = rand() % 16 - 8;
				ky = rand() % 16 - 8;
			}

			rsrc.right = (long)dds_face[0][j]->wg;
			rsrc.bottom = (long)dds_face[0][j]->hg;
			rdst.z = -0.04f;
			rdst.left = 20.0f/240.0f + 0.08f*j+kx/240.0f;
			rdst.right = rdst.left + 40.0f/240.0f;
			rdst.top = (float)(20.0/240.0) + 0.065f*j+ky/240.0f;
			rdst.bottom = rdst.top + (40.0f/240.0f);
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
	//�J�I(2p)
	for(j=0;j<g_battleinfo.GetNumTeam(1);j++){
		if(dds_face[1][j]!=NULL){
			int kx = 0, ky = 0;

			// �h��
			CGObject* pobj = battleTask->GetCharacterObject(1,j);
			if (pobj && pobj->siver)
			{
				kx = rand() % 16 - 8;
				ky = rand() % 16 - 8;
			}

			rsrc.right = (long)dds_face[1][j]->wg;
			rsrc.bottom = (long)dds_face[1][j]->hg;
			rdst.z = -0.04f;
			rdst.left = 2.0f*(320.0f/240.0f) - 20.0f/240.0f - 0.08f*j+kx/240.0f;
			rdst.right = rdst.left - (40.0f/240.0f);
			rdst.top = 20.0f/240.0f + 0.065f*j+ky/240.0f;
			rdst.bottom = rdst.top + (40.0f/240.0f);
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

	//�����}�[�N
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

void CGauge::DrawHPGauge2()//�̗̓Q�[�W�̕`��i2vs2�j
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

	//�����ۂ��w�i
	vb[0].x = (float)( 320.0/240.0 - 50.0/240.0 );
	vb[0].y = (float)( 0.172 - 0.04 - 0.02*(g_battleinfo.GetNumTeam(0)-1));
	vb[1].x = (float)( 320.0/240.0 - 50.0/240.0 );
	vb[1].y = (float)( 0.172 + 0.04 + 0.02*(g_battleinfo.GetNumTeam(0)-1) );
	vb[2].x = (float)( 320.0/240.0  - 230.0/240.0 - 50.0/240.0 );
	vb[2].y = (float)( 0.172 - 0.04 - 0.02*(g_battleinfo.GetNumTeam(0)-1));
	vb[3].x = (float)( 320.0/240.0  - 230.0/240.0 - 50.0/240.0 );
	vb[3].y = (float)( 0.172 + 0.04 + 0.02*(g_battleinfo.GetNumTeam(0)-1));
	vb[0].color = vb[1].color = 0xDD000000;
	vb[2].color = vb[3].color = 0x00000000;
	for(i=0;i<4;i++){
		vb[i].z = -0.04f;
		vb[i].tu = vb[i].tv = 0;
	}
	g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
	//�����ۂ��w�i(2p)
	vb[0].x = (float)( 320.0/240.0 + 50.0/240.0 );
	vb[0].y = (float)( 0.172 - 0.04 - 0.02*(g_battleinfo.GetNumTeam(1)-1));
	vb[1].x = (float)( 320.0/240.0 + 50.0/240.0 );
	vb[1].y = (float)( 0.172 + 0.04 + 0.02*(g_battleinfo.GetNumTeam(1)-1));
	vb[2].x = (float)( 320.0/240.0  + 230.0/240.0 + 50.0/240.0 );
	vb[2].y = (float)( 0.172 - 0.04 - 0.02*(g_battleinfo.GetNumTeam(1)-1));
	vb[3].x = (float)( 320.0/240.0  + 230.0/240.0 + 50.0/240.0 );
	vb[3].y = (float)( 0.172 + 0.04 + 0.02*(g_battleinfo.GetNumTeam(1)-1));
	vb[0].color = vb[1].color = 0xDD000000;
	vb[2].color = vb[3].color = 0x00000000;
			for(i=0;i<4;i++){
				vb[i].z = -0.04f;
				vb[i].tu = vb[i].tv = 0;
			}
			g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));

	BYTE gauge_blue,gauge_yellow;
	
	double turiage_y;

	for(j=0;j<g_battleinfo.GetNumTeam(0);j++)
	{
		switch(g_battleinfo.GetNumTeam(0)){
		case 2:turiage_y=-0.025;break;
		case 3:turiage_y=-0.075;break;
		default:turiage_y=0;
		}
		//1p���̗͖_
		pdat = (GOBJECT*)battleTask->GetCharacterInfo(0,j);
		if(pdat!=NULL){
			//�̗�
			if(pdat->hpmax!=0){
				gauge_prv2[0][j] -= 20;
					if(gauge_prv2[0][j] < pdat->hp)gauge_prv2[0][j] = pdat->hp;
				hiritu = (double)gauge_prv2[0][j] / (double)pdat->hpmax;
				if(hiritu<0)hiritu=0;
				vb[0].x = (float)( 320.0/240.0 - 50.0/240.0 );
				vb[0].y = (float)( 0.18 - 0.02 +turiage_y +0.05*j);
				vb[1].x = (float)( 320.0/240.0 - 50.0/240.0 );
				vb[1].y = (float)( 0.18 + 0.02 +turiage_y +0.05*j);
				vb[2].x = (float)( 320.0/240.0  - 210.0/240.0*hiritu - 50.0/240.0 );
				vb[2].y = (float)( 0.18 - 0.02 +turiage_y +0.05*j);
				vb[3].x = (float)( 320.0/240.0  - 210.0/240.0*hiritu - 50.0/240.0 );
				vb[3].y = (float)( 0.18 + 0.02 +turiage_y +0.05*j);
				gauge_blue = (BYTE)(255*hiritu);
				gauge_yellow = (BYTE)(255*(1-hiritu));
				vb[2].color = vb[3].color = 0xFF000000 + gauge_yellow*256*256 + gauge_yellow*256 +gauge_blue;
				vb[0].color = vb[1].color = 0xFFAA77FF;
				for(i=0;i<4;i++){
					vb[i].z = -0.05f;
					vb[i].tu = vb[i].tv = 0;
				}
				
				if(j!=battleTask->GetActiveCharacterID(0)){
					vb[0].color = vb[1].color = vb[2].color = vb[3].color = 0xFF000077;
				}
				g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
			
				//��������
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
					vb[0].x = vb[1].x = (float)( 320.0/240.0  - 210.0/240.0*hiritu - 50.0/240.0 );
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
		//2p���̗͖_
		pdat = (GOBJECT*)battleTask->GetCharacterInfo(1,j);
		if(pdat!=NULL){
			//�̗�
			if(pdat->hpmax!=0){
				gauge_prv2[1][j] -= 20;
					if(gauge_prv2[1][j] < pdat->hp)gauge_prv2[1][j] = pdat->hp;
				hiritu = (double)gauge_prv2[1][j] / (double)pdat->hpmax;
				if(hiritu<0)hiritu=0;
				vb[0].x = (float)( 320.0/240.0 + 50.0/240.0 );
				vb[0].y = (float)( 0.18 - 0.02 +turiage_y +0.05*j);
				vb[1].x = (float)( 320.0/240.0 + 50.0/240.0 );
				vb[1].y = (float)( 0.18 + 0.02 +turiage_y +0.05*j);
				vb[2].x = (float)( 320.0/240.0  + 210.0/240.0*hiritu + 50.0/240.0 );
				vb[2].y = (float)( 0.18 - 0.02 +turiage_y +0.05*j);
				vb[3].x = (float)( 320.0/240.0  + 210.0/240.0*hiritu + 50.0/240.0 );
				vb[3].y = (float)( 0.18 + 0.02 +turiage_y +0.05*j);
				gauge_blue = (BYTE)(255*hiritu);
				gauge_yellow = (BYTE)(255*(1-hiritu));
				vb[2].color = vb[3].color = 0xFF000000 + gauge_yellow*256*256 + gauge_yellow*256 +gauge_blue;
				vb[0].color = vb[1].color = 0xFFAA77FF;
				for(i=0;i<4;i++){
					vb[i].z = -0.05f;
					vb[i].tu = vb[i].tv = 0;
				}
				if(j!=battleTask->GetActiveCharacterID(1)){
					vb[0].color = vb[1].color = vb[2].color = vb[3].color = 0xFF000077;
				}
				g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
			
				//��������
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
					vb[0].x = vb[1].x = (float)( 320.0/240.0  + 210.0/240.0*hiritu + 50.0/240.0 );
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
	//�J�I(1p)
	for(j=0;j<g_battleinfo.GetNumTeam(0);j++){
		if(dds_face[0][j]!=NULL){
			int kx = 0, ky = 0;

			// �h��
			CGObject* pobj = battleTask->GetCharacterObject(0,j);
			if (pobj && pobj->siver)
			{
				kx = rand() % 16 - 8;
				ky = rand() % 16 - 8;
			}

			rsrc.right = (long)dds_face[0][j]->wg;
			rsrc.bottom = (long)dds_face[0][j]->hg;
			rdst.z = -0.04f;
			rdst.left = 20.0f/240.0f + 0.08f*j+kx/240.0f;
			rdst.right = rdst.left + 40.0f/240.0f;
			rdst.top = (float)(20.0/240.0) + 0.065f*j+ky/240.0f;
			rdst.bottom = rdst.top + (40.0f/240.0f);
			g_draw.ResetParentMatrix();
			if(j==battleTask->GetActiveCharacterID(0)){
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
		//�J�I(2p)
		if(dds_face[1][j]!=NULL){
			int kx = 0, ky = 0;

			// �h��
			CGObject* pobj = battleTask->GetCharacterObject(1,j);
			if (pobj && pobj->siver)
			{
				kx = rand() % 16 - 8;
				ky = rand() % 16 - 8;
			}

			rsrc.right = (long)dds_face[1][j]->wg;
			rsrc.bottom = (long)dds_face[1][j]->hg;
			rdst.z = -0.04f;
			rdst.left = 2.0f*(320.0f/240.0f) - 20.0f/240.0f - 0.08f*j+kx/240.0f;
			rdst.right = rdst.left - (40.0f/240.0f);
			rdst.top = 20.0f/240.0f + 0.065f*j+ky/240.0f;
			rdst.bottom = rdst.top + (40.0f/240.0f);
			g_draw.ResetParentMatrix();
			if(j==battleTask->GetActiveCharacterID(1)){
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
	//�X�g���C�J�[�}�[�N
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
//�@�p���[�Q�[�W�̕`��
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

//��������
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

//�Q�[�W�E�^�C�v1(�����X�g�b�N�\�ȃQ�[�W)
void CGauge::DrawPowerGaugeType1(BOOL player,int pindex,int x,int y)
{
	int j;
	int num;
	GOBJECT *pdat;
	DWORD gcolor=0xFFFFFFFF;
	
	RECT rpg;
	rpg.top=185;
	rpg.bottom=rpg.top+13;
	rpg.left=0;

	if(!player){
		j=0;
		//�v���C���[1��
		g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_PG2_BLANKL,x+10,y+470,0.0f,0,FALSE,FALSE,gcolor);//���N
		pdat = (GOBJECT*)battleTask->GetCharacterInfo( j,pindex );
		if(pdat!=NULL){
			num = (int)pdat->gauge;//�Q�[�W�̐�������
			//���������������Ă��特��炷
			if(num > (int)numprv[j][pindex])g_system.PlaySystemSound(SYSTEMSOUND_CHARGE);
			numprv[j][pindex] = num;
			num = (int)pgauge_prv[j][pindex];
			battleTask->DrawNumber3(pdat->gauge,x+35,y+445,0.0f);
			if(pdat->gauge == pdat->gaugemax){//�Q�[�W�}�b�N�X!!
				g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_PG2_MAX,x+65,y+448,0.0f,0,FALSE,FALSE,gcolor);
			}
			else {//�_��`��
				rpg.right = rpg.left+(int)((pgauge_prv[j][pindex] - num)*87);
				g_draw.CheckBlt(g_system.GetSystemGraphicSurface()[1],x+66,y+448,rpg,FALSE,FALSE,0,0.0f,gcolor);
			}
		}
	}
	else{
		j=1;
		x*=-1;
		//�v���C���[2��
		g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_PG2_BLANKR,x+630,y+470,0.0f,0,FALSE,FALSE,gcolor);//���N
		pdat = (GOBJECT*)battleTask->GetCharacterInfo( j,pindex );
		if(pdat!=NULL){
			num = (int)pdat->gauge;//�Q�[�W�̐�������
			//���������������Ă��特��炷
			if(num > (int)numprv[j][pindex])g_system.PlaySystemSound(SYSTEMSOUND_CHARGE);
			numprv[j][pindex] = num;
			num = (int)pgauge_prv[j][pindex];
			battleTask->DrawNumber3(pdat->gauge,x+600,y+445,0.0f);
			if(pdat->gauge == pdat->gaugemax){//�Q�[�W�}�b�N�X!!
				g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_PG2_MAX,x+487,y+448,0.0f,0,FALSE,FALSE,gcolor);
			}
			else {//�_��`��
				rpg.right = rpg.left+(int)((pgauge_prv[j][pindex] - num)*87);
				g_draw.CheckBlt(g_system.GetSystemGraphicSurface()[1],x+575-(rpg.right-rpg.left),y+448,rpg,FALSE,FALSE,0,0.0f,gcolor);
			}
		}
	}
}

//�Q�[�W�E�^�C�v2�i0-100���ȃQ�[�W�j
void CGauge::DrawPowerGaugeType2(BOOL player,int pindex,int x,int y,BOOL pow)
{
	int j;
	int num;
	GOBJECT *pdat;
	DWORD gcolor=0xFFFFFFFF;
	
	RECT rpg;
	rpg.top=185;
	rpg.bottom=rpg.top+13;
	rpg.left=0;

	DWORD cell_max;

	if(!player){
		j=0;
		//�v���C���[1��
		if(pow)g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_PG1_POW,x+18,y+455,0.0f,0,FALSE,FALSE,gcolor);//�u�p�E�b�v
		else x-=40;
		pdat = (GOBJECT*)battleTask->GetCharacterInfo( j,pindex );
		if(pdat!=NULL){
			num = (int)pdat->gauge;
			if(num > (int)numprv[j][pindex])g_system.PlaySystemSound(SYSTEMSOUND_CHARGE);
			numprv[j][pindex] = num;
			if(pdat->gauge == pdat->gaugemax){//�Q�[�W�}�b�N�X
				if((timeGetTime()/50)%2==0)cell_max=CELL_PG1_MAX1;
				else cell_max=CELL_PG1_MAX2;
				g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),cell_max,x+45,y+455,0.0f,0,FALSE,FALSE,gcolor);
			}
			else {
				g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_PG1_BLANK,x+45,y+455,0.0f,0,FALSE,FALSE,gcolor);//���N
				rpg.right = rpg.left+(int)((pgauge_prv[j][pindex])*132);
				g_draw.CheckBlt(g_system.GetSystemGraphicSurface()[1],x+48,y+452,rpg,FALSE,FALSE,0,0.0f,gcolor);//�_
				battleTask->DrawNumber4(pgauge_prv[j][pindex],x+97,y+454,0.0f);//�p�[�Z���e�[�W
			}
		}
	}
	else{
		j=1;
		x*=-1;
		//�v���C���[2��
		if(pow)g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_PG1_POW,x+612,y+455,0.0f,0,FALSE,FALSE,gcolor);//�u�p�E�b�v
		else x+=40;
		pdat = (GOBJECT*)battleTask->GetCharacterInfo( j,pindex );
		if(pdat!=NULL){
			num = (int)pdat->gauge;
			if(num > (int)numprv[j][pindex])g_system.PlaySystemSound(SYSTEMSOUND_CHARGE);
			numprv[j][pindex] = num;
			if(pdat->gauge == pdat->gaugemax){//�Q�[�W�}�b�N�X
				if((timeGetTime()/50)%2==0)cell_max=CELL_PG1_MAX1;
				else cell_max=CELL_PG1_MAX2;
				g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),cell_max,x+452,y+455,0.0f,0,FALSE,FALSE,gcolor);
			}
			else {
				g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),CELL_PG1_BLANK,x+452,y+455,0.0f,0,FALSE,FALSE,gcolor);//���N
				rpg.right = rpg.left+(int)((pgauge_prv[j][pindex])*132);
				g_draw.CheckBlt(g_system.GetSystemGraphicSurface()[1],x+588-(rpg.right-rpg.left),y+452,rpg,FALSE,FALSE,0,0.0f,gcolor);//�_
				battleTask->DrawNumber4(pgauge_prv[j][pindex],x+505,y+454,0.0f);//�p�[�Z���e�[�W
			}
		}
	}
}
