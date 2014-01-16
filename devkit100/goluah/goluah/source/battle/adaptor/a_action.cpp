/*
2011/10/29	�󒆂ŃX�g���C�J�[���Ă񂾎��ɕ��V���Ȃ��悤�ɂ������A�������x�ɉe���A��
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
// ATTACKINFO ai[MAXNUM_ATTACKINFO];//�U���͏��

void CCharAdaptor::InitAttackInfo()
{
//	ZeroMemory(ai,sizeof(ai));

	/*�����ōU���͏��̏��������s���Ă�������*/
	atkmuki.damage = 0;
	atkmuki.hit = 0;
	atkmuki.muki = TRUE;
}

//�f�t�H���g�ʏ퓮��=================================================

void CCharAdaptor::act_taiki()//��ʊO�ɔ��ł��đҋ@����
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

void CCharAdaptor::act_koutai()//��サ�ĉ�ʓ��ɓ���(1)
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
		pdat->objtype &= ~GOBJFLG_TAIKI;//�ҋ@�t���O������
		pdat->y=0;
		pdat->aid = ACTID_NEUTRAL;
		(*funcs->msg2system)(pdat->id,MSGOBJ2SYS_KOUTAIEND,0);
	}
}

void CCharAdaptor::act_koutai2()//�o��|�[�Y�E�L��2
{
	/*�����Ɍ���̓o��|�[�Y������L�q���Ă�������*/
}

void CCharAdaptor::act_koutai_out()//�`�[����A��サ�ĉ�ʊO�ɂ���
{
	if(pdat->counter==0){
		pdat->objtype &= GOBJTYPE_NODISPLAY;//��ʊO�ւ�����悤�ɂ��Ă���
		pdat->objtype |= GOBJFLG_TAIKI;//�ҋ@�t���O�𗧂Ă�
		pdat->objtype &= ~GOBJFLG_TOBETARGET;//�^�[�Q�b�g�ɂ͂Ȃ�Ȃ�
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

void CCharAdaptor::act_koutai_in()//�`�[����A��サ�ĉ�ʓ��ɓ����Ă���(2)
{
	if(pdat->counter==0){
		pdat->objtype |= GOBJTYPE_DISPLAY;//��ʓ��ɂ���
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
		pdat->objtype &= ~GOBJFLG_TAIKI;//�ҋ@�t���O������
		pdat->y=0;
		pdat->aid = ACTID_NEUTRAL;
	}
}

void CCharAdaptor::act_strikercomeon()
{
	/*�����ɃX�g���C�J�[���Ă񂾂Ƃ��̓�����L�q���Ă�������*/
	pdat->aid = ACTID_KUCYU;
}

//�ʏ퓮��===========================================================

