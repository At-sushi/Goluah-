
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>

#define EXPORT extern "C" __declspec(dllexport) 

#include "define_gcd.h"
#include "gobject.h"
#include "stage.h"
#include "jisaku.h"
#include "cell.h"

extern DI_FUNCTIONS_S	*funcs;
extern DI_FUNCTIONS_O	*funco;
extern DI_FUNCTIONS_D	*funcd;
extern LPDIRECT3DDEVICE9 d3ddev;
extern char *g_stgdir;

//***************************************************************

MYSURFACE *pimg[12]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
GCD_CELL2 *cdat = NULL;
GCD_RECT  *rdat = NULL;
void      *muda = NULL;

//ｼﾞｻｸｼﾞｴｰﾝ
int prev_hp[2];
DWORD ouen;
DWORD teki;

//*****************************************************************
//　初期化
//*****************************************************************

void InitializeJien()
{
	char fn[256];

	//セル読み込み
	for(int i=0;i<8;i++){
		sprintf(fn,"%s\\image%d",g_stgdir,i+1);
		pimg[i] = (MYSURFACE*)(*funcd->loadimage)(fn,NULL);
	}
	sprintf(fn,"%s\\cell",g_stgdir);
	(*funcd->create_celldat2)(fn,(LPVOID*)&cdat,(LPVOID*)&rdat,&muda);

	//変数初期化（てきとう）
	ouen=TEAM_PLAYER1;
	teki=TEAM_PLAYER2;

	GOBJECT *pdat;

	pdat = (GOBJECT*)funco->getactivechar(ouen);
	if(pdat!=NULL)prev_hp[ouen]=pdat->hp;
	pdat = (GOBJECT*)funco->getactivechar(teki);
	if(pdat!=NULL)prev_hp[teki]=pdat->hp;

	//オブジェクト生成
	DWORD oid;
	oid = funco->objcreate();
	pdat = (GOBJECT*)funco->getinfo(oid);
	if(pdat!=NULL){
		pdat->tid=TEAM_STAGE;
		pdat->uid=JISAKUID_PYON;
		pdat->x=-150*(0.6f+1.0f);
		pdat->msghandler=gMsgJien;
	}
	oid = funco->objcreate();
	pdat = (GOBJECT*)funco->getinfo(oid);
	if(pdat!=NULL){
		pdat->tid=TEAM_STAGE;
		pdat->uid=JISAKUID_OUEN;
		pdat->x=0;
		pdat->msghandler=gMsgJien;
	}
	oid = funco->objcreate();
	pdat = (GOBJECT*)funco->getinfo(oid);
	if(pdat!=NULL){
		pdat->tid=TEAM_STAGE;
		pdat->uid=JISAKUID_MITUME;
		pdat->x=140*(0.6f+1.0f);
		pdat->msghandler=gMsgJien;
	}
	oid = funco->objcreate();
	pdat = (GOBJECT*)funco->getinfo(oid);
	if(pdat!=NULL){
		pdat->tid=TEAM_STAGE;
		pdat->uid=JISAKUID_MITUME2;
		pdat->x=170*(0.6f+1.0f);
		pdat->msghandler=gMsgJien;
	}
}

void CleanupJien()
{
	for(int i=0;i<8;i++){
		if(pimg[i]!=NULL){
			funcd->unloadbmp(pimg[i]);
			pimg[i]=NULL;
		}
	}

	funcd->destroy_celldat((LPVOID*)&cdat,(LPVOID*)&rdat,&muda);
}

//***************************************************************
//　メッセージ処理関数
//***************************************************************

DWORD gMsgJien(DWORD msg,LPVOID pd,DWORD prm)
{
	GOBJECT *pdat=(GOBJECT*)pd , *pmdat,*pedat;
	int tmp;

	switch(msg){
	case GOBJMSG_ACTION:
		switch(pdat->uid){
		case JISAKUID_PYON://ｼﾞｻｸｼﾞｴﾝ①
			tmp = pdat->counter%120;
			if(tmp < 2)pdat->cnow = CELL_1_4;
			else if(tmp < 4)pdat->cnow = CELL_1_2;
			else if(tmp < 6)pdat->cnow = CELL_1_3;
			else if(tmp < 8)pdat->cnow = CELL_1_4;
			else if(tmp <10)pdat->cnow = CELL_1_5;
			else if(tmp <12)pdat->cnow = CELL_1_6;
			else if(tmp <14)pdat->cnow = CELL_1_5;
			else if(tmp <16)pdat->cnow = CELL_1_4;
			else if(tmp <18)pdat->cnow = CELL_1_3;
			else if(tmp <20)pdat->cnow = CELL_1_2;
			else pdat->cnow = CELL_1_4;
			break;
		case JISAKUID_OUEN://ｼﾞｻｸｼﾞｴﾝ②
			switch(pdat->aid){
			case 0://（･∀･）ｲｲ!
				pdat->cnow=CELL_2_GOOD;
				if(pdat->counter > 20){
					pdat->aid=2;
				}
				break;
			case 1://（･Ａ･）ｲｸﾅｲ！
				pdat->cnow=CELL_2_BAD;
				if(pdat->counter > 20){
					pdat->aid=2;
				}
				break;
			case 2://静止
				pdat->cnow=CELL_2;
				if(pdat->counter>20){
					if(pdat->x-20 > funcs->getdispcenterx()){
						pdat->aid=3;
					}
					else if(pdat->x+20 < funcs->getdispcenterx()){
						pdat->aid=4;
					}
				}
				break;
			case 3://→移動
				pdat->x -=4;
				pdat->cnow = CELL_2_LRUN1 + (pdat->counter/2)%8;
				if(pdat->counter>60){
					pdat->aid = 2;
				}
				break;
			case 4://←移動
				pdat->x +=4;
				pdat->cnow = CELL_2_RRUN1 + (pdat->counter/2)%8;
				if(pdat->counter>60){
					pdat->aid = 2;
				}
				break;
			default:
				pdat->aid=2;
			}
			pmdat=(GOBJECT*)funco->getactivechar(ouen);
			pedat=(GOBJECT*)funco->getactivechar(teki);
			if(pmdat!=NULL && pedat!=NULL){
				if(prev_hp[ouen] > pmdat->hp){//ダメージを食らった
					pdat->aid=1;
					pdat->counter=0;
				}
				else if(prev_hp[teki] > pedat->hp){//ダメージを与えた
					pdat->aid=0;
					pdat->counter=0;
				}
				prev_hp[ouen]=pmdat->hp;
				prev_hp[teki]=pedat->hp;
			}
			break;
		case JISAKUID_MITUME://ｼﾞｻｸｼﾞｴﾝ③
			tmp = pdat->counter%300;
			if(tmp < 2)pdat->cnow = CELL_3_1;
			else if(tmp < 4)pdat->cnow = CELL_3_2;
			else if(tmp < 6)pdat->cnow = CELL_3_3;
			else if(tmp < 8)pdat->cnow = CELL_3_4;
			else if(tmp <10)pdat->cnow = CELL_3_5;
			else if(tmp <150)pdat->cnow = CELL_3_6;
			else if(tmp <152)pdat->cnow = CELL_3_5;
			else if(tmp <154)pdat->cnow = CELL_3_4;
			else if(tmp <156)pdat->cnow = CELL_3_3;
			else if(tmp <158)pdat->cnow = CELL_3_2;
			else pdat->cnow = CELL_3_1;
			break;
		case JISAKUID_MITUME2://ｼﾞｻｸｼﾞｴﾝ④
			tmp = pdat->counter%200;
			if(tmp < 2)pdat->cnow = CELL_3_1;
			else if(tmp < 4)pdat->cnow = CELL_3_2;
			else if(tmp < 6)pdat->cnow = CELL_3_3;
			else if(tmp < 8)pdat->cnow = CELL_3_4;
			else if(tmp <100)pdat->cnow = CELL_3_5;
			else if(tmp <100)pdat->cnow = CELL_3_6;
			else if(tmp <102)pdat->cnow = CELL_3_5;
			else if(tmp <104)pdat->cnow = CELL_3_4;
			else if(tmp <106)pdat->cnow = CELL_3_3;
			else if(tmp <108)pdat->cnow = CELL_3_2;
			else pdat->cnow = CELL_3_1;
			pdat->revx=TRUE;
			break;
		}
		return(TRUE);
	case GOBJMSG_DRAW:return(TRUE);
	case GOBJMSG_DRAWBACK:
		switch(pdat->uid){
		case JISAKUID_OUEN:pdat->z=0.5f;break;
		default:pdat->z=1.0f;
		}
		funcd->celldraw(
			(void**)pimg,
			cdat,
			rdat,
			pdat->cnow,
			(int)pdat->x,
			(int)pdat->y,
			(float)pdat->z,
			0,
			pdat->revx,
			0,
			0xFFFFFFFF,
			1.0f,
			1.0f,
			1);
		return(TRUE);
	case GOBJMSG_DELETE:
		CleanupJien();
		break;
	case GOBJMSG_CNGROUND:
		return(TRUE);
	}

	return(0);
}

