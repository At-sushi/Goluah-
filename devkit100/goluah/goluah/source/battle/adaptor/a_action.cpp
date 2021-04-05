/*
2011/10/29	空中でストライカーを呼んだ時に浮遊しないようにしたが、落下速度に影響アリ
*/
#include "StdAfx.h"

#define LPDIRECT3DTEXTURE8 LPVOID

#include "define_gcd.h"
#include "gobject.h"
#include "define_data.h"
#include "define_char.h"
#include "mimikaki.h"
#include "adaptor.h"
//#include "cell.h"

extern DI_FUNCTIONS_S *funcs;
extern DI_FUNCTIONS_O *funco;
extern DI_FUNCTIONS_D *funcd;
extern ATTACKINFO atkmuki;

// #define MAXNUM_ATTACKINFO	64
// ATTACKINFO ai[MAXNUM_ATTACKINFO];//攻撃力情報

void CCharAdaptor::InitAttackInfo()
{
//	ZeroMemory(ai,sizeof(ai));

	/*ここで攻撃力情報の初期化を行ってください*/
	atkmuki.damage = 0;
	atkmuki.hit = 0;
	atkmuki.muki = TRUE;
}

//デフォルト通常動作=================================================

void CCharAdaptor::act_taiki()//画面外に飛んでって待機する
{
	if(pdat->counter < 2){
//		pdat->cnow = CELL_CROUCH;
		pdat->vx = -12;
		pdat->vy = -24;
	}
	else {
		pdat->revx=TRUE;
		movex(pdat->vx);
		pdat->y += pdat->vy;
		if(++pdat->vy>-10)pdat->vy=-10;
//		pdat->cnow = CELL_JA;
		if(pdat->counter > 60){
			pdat->aid = ACTID_TAIKICYU;
			pdat->x = 2000;
			pdat->y = -2000;
		}
	}
}

void CCharAdaptor::act_koutai()//交代して画面内に入る(1)
{
	GOBJECT *pedat;
	if(pdat->counter==0){
		if(pdat->tid==TEAM_PLAYER1)
			pedat = (GOBJECT*)funco->getactivechar(TEAM_PLAYER2);
		else
			pedat = (GOBJECT*)funco->getactivechar(TEAM_PLAYER1);
		if(pedat!=NULL){
			if(pedat->x>0){
				pdat->x = pedat->x -(320+400);
				pdat->muki=FALSE;
				pdat->vx=14;
			}
			else{
				pdat->x = pedat->x +(320+400);
				pdat->muki=TRUE;
				pdat->vx=14;
			}
		}
		else {
			pdat->x=0;
			pdat->vx=0;
		}
		pdat->y = -400;
		pdat->vy= 0;
	}
	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy++;

//	pdat->cnow = CELL_JAMP1;

	if(pdat->y > 0){
		pdat->objtype &= ~GOBJFLG_TAIKI;//待機フラグを消す
		pdat->y=0;
		pdat->aid = ACTID_NEUTRAL;
		(*funcs->msg2system)(pdat->id,MSGOBJ2SYS_KOUTAIEND,0);
	}
}

void CCharAdaptor::act_koutai2()//登場ポーズ・キメ2
{
	/*ここに交代後の登場ポーズ動作を記述してください*/
}

void CCharAdaptor::act_koutai_out()//チーム戦、交代して画面外にいく
{
	if(pdat->counter==0){
		pdat->objtype &= GOBJTYPE_NODISPLAY;//画面外へいけるようにしておく
		pdat->objtype |= GOBJFLG_TAIKI;//待機フラグを立てる
		pdat->objtype &= ~GOBJFLG_TOBETARGET;//ターゲットにはならない
	}

	if(pdat->counter < 2){
//		pdat->cnow = CELL_CROUCH;
		pdat->vx = -12;
		pdat->vy = -16;
	}
	else {
		pdat->revx=TRUE;
		movex(pdat->vx);
		pdat->y += pdat->vy;
		pdat->vy++;
		if(pdat->vy>-6)pdat->vy=-6;
//		pdat->cnow = CELL_JA;
		
		if(pdat->counter > 60){
			pdat->aid = ACTID_TAIKICYU;
			pdat->x = 2000;
			pdat->y = -2000;
		}
	}
}

void CCharAdaptor::act_koutai_in()//チーム戦、交代して画面内に入っていく(2)
{
	if(pdat->counter==0){
		pdat->objtype |= GOBJTYPE_DISPLAY;//画面内にいる
	}

	GOBJECT *pedat;
	if(pdat->counter==0){
		if(pdat->tid==TEAM_PLAYER1)
			pedat = (GOBJECT*)funco->getactivechar(TEAM_PLAYER2);
		else
			pedat = (GOBJECT*)funco->getactivechar(TEAM_PLAYER1);
		if(pedat!=NULL){
			if(pedat->x>0){
				pdat->x = pedat->x -(320+400);
				pdat->muki=FALSE;
				pdat->vx=14;
			}
			else{
				pdat->x = pedat->x +(320+400);
				pdat->muki=TRUE;
				pdat->vx=14;
			}
		}
		else {
			pdat->x=0;
			pdat->vx=0;
		}
		pdat->y = -400;
		pdat->vy= 0;
	}
	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy++;

//	pdat->cnow = CELL_JAMP1;
	if(pdat->y > 0){
		pdat->objtype &= ~GOBJFLG_TAIKI;//待機フラグを消す
		pdat->y=0;
		pdat->aid = ACTID_NEUTRAL;
	}
}

void CCharAdaptor::act_strikercomeon()
{
	/*ここにストライカーを呼んだときの動作を記述してください*/
	pdat->aid = ACTID_KUCYU;
}

//通常動作===========================================================

