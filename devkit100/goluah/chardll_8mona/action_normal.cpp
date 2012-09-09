/*==============================================================

	�s���֐�(�ʏ퓮��)

================================================================*/
#include "character.h"

//*********************************************************
//�@�@�ʏ퓮��
//*********************************************************

//! �j���[�g��
void CCharacter::act_neutral()
{
	pdat->cnow = CELL_NEUTRAL;
}

//! ���Ⴊ��
void CCharacter::act_crouch()
{
	pdat->cnow = CELL_CROUCH;
}

//! ����
void CCharacter::act_rakka()
{
	pdat->cnow = CELL_JAMP1;

	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy++;
	if(pdat->y > 0){
		pdat->aid = ACTID_NEUTRAL;
		pdat->y =0;
		Furimuki();
	}
}

//! �o��
void CCharacter::act_tojyo()
{
	if(pdat->counter == 0){
		pdat->y = -400;
		pdat->vy = 5;
		PlayMySound(wave_tojyo);
	}

	if(pdat->y < 0){
		pdat->cnow = CELL_JAMP1;
		pdat->y += pdat->vy;
		pdat->vy ++;
	}
	else pdat->cnow = CELL_NAKAMA16;

	if(pdat->counter > 100){
		pdat->y=0;
		pdat->aid = ACTID_NEUTRAL;
		Message2System(MSGOBJ2SYS_TOJYOEND,0);
	}
}

//! ����
void CCharacter::act_win()
{
	if(pdat->counter==0){
		LoadAndSetKatiSerif();
		clabul_nak1->Go(pdat->muki,pdat->x-100,0);
		clabul_nak2->Go(pdat->muki,pdat->x+100,0);
		PlayMySound(wave_win);
	}

	if(pdat->counter == 62){
		clabul_nak1->Hide();
		clabul_nak2->Hide();
		pdat->cnow = CELL_WIN;
	}

	if(pdat->counter == 120)
		Message2System(MSGOBJ2SYS_WINPOSEEND,0);
}

//! �O�i
void CCharacter::act_walkf()
{
	pdat->cnow = CELL_WALK1+(pdat->counter/4)%4;
	movex(6);
}

//! ���
void CCharacter::act_walkb()
{
	pdat->cnow = CELL_WALKB1+(pdat->counter/4)%4;
	movex(-6);
}

//! �W�����v�\���
void CCharacter::act_jamps()
{
	if(pdat->counter >2)pdat->aid = ACTID_JAMPC;
	pdat->cnow = CELL_JAMP;
}

//!�����W�����v
void CCharacter::act_jampc()
{
	if(pdat->counter == 0){pdat->vy=-20;pdat->vx=0;}

	pdat->cnow = CELL_JAMP1;

	pdat->y += pdat->vy;
	pdat->vy++;
	if(pdat->y > 0){
		pdat->aid = ACTID_NEUTRAL;
		pdat->y =0;
		Furimuki();
	}
}

//!�O�W�����v
void CCharacter::act_jampf()
{
	if(pdat->counter == 0){pdat->vy=-20;pdat->vx=6;}

	pdat->cnow = CELL_JAMP1;

	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy++;
	if(pdat->y > 0){
		pdat->aid = ACTID_NEUTRAL;
		pdat->y =0;
		Furimuki();
	}
}

//!��W�����v
void CCharacter::act_jampb()
{
	if(pdat->counter == 0){pdat->vy=-20;pdat->vx=-6;}

	pdat->cnow = CELL_JAMP1;

	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy++;
	if(pdat->y > 0){
		pdat->aid = ACTID_NEUTRAL;
		pdat->y =0;
		Furimuki();
	}
}


//*********************************************************
//�@���̑�
//*********************************************************
void CCharacter::act_taiki()
{
	pdat->cnow= CELL_JAMP1;
	CCharacterBase::act_taiki();//�L�����N�^�[��{�N���X�̓���𗬗p
}

void CCharacter::act_koutai_out()
{
	pdat->cnow= CELL_JAMP1;
	CCharacterBase::act_koutai_out();//�L�����N�^�[��{�N���X�̓���𗬗p
}

void CCharacter::act_koutai_in()
{
	pdat->cnow= CELL_JAMP1;
	CCharacterBase::act_koutai_in();//�L�����N�^�[��{�N���X�̓���𗬗p
}

void CCharacter::act_koutai()
{
	//pdat->cnow= CELL_JAMP1;
	//CCharacterBase::act_koutai();//�L�����N�^�[��{�N���X�̓���𗬗p

	pdat->objtype |= GOBJTYPE_DISPLAY;//��ʓ�ɂ���

	GOBJECT *pedat;
	if(pdat->counter==0){
		if(pdat->tid==TEAM_PLAYER1)
			pedat = GetActiveCharacter( TEAM_PLAYER2 );
		else
			pedat = GetActiveCharacter(TEAM_PLAYER1);
		if(pedat!=NULL){
			if(pedat->x>0){
				pdat->x = pedat->x -(360);
				pdat->muki=FALSE;
				//pdat->vx=14;
			}
			else{
				pdat->x = pedat->x +(360);
				pdat->muki=TRUE;
				//pdat->vx=14;
			}
		}
		else {
			pdat->x=0;
			pdat->vx=0;
		}
		//pdat->y = -600;
		//pdat->vy= 0;
	}

	pdat->objtype &= ~GOBJFLG_TAIKI;//�ҋ@�t���O����
	AddProperty( GOBJFLG_TOBETARGET );//�^�[�Q�b�g�ɂȂ�
	pdat->aid = ACTID_KOUTAI2;//�o��|�[�Y
}

void CCharacter::act_koutai2()
{
//	CCharacterBase::act_koutai2();//�L�����N�^�[��{�N���X�̓���𗬗p
	if(pdat->counter == 0){
		pdat->vy = 5;
		PlayMySound(wave_tojyo);
	}
	if(pdat->y < 0){
		pdat->cnow = CELL_JAMP1;
		pdat->y += pdat->vy;
		pdat->vy ++;
	}
	else pdat->cnow = CELL_NAKAMA16;

	if(pdat->counter > 100){
		pdat->aid = ACTID_NEUTRAL;
		Message2System(MSGOBJ2SYS_KOUTAIEND,0);
	}
}

//*********************************************************
//�@�ǉ����
//*********************************************************

void CCharacter::act_strikercomeon()
{
	if(pdat->counter<30)pdat->cnow=CELL_ESCAPE;
	else pdat->aid=ACTID_NEUTRAL;
}

void CCharacter::act_dashf()
{
	if(pdat->counter == 0){
		clabul_wind->Go(pdat->muki,pdat->x,0,0);
		pdat->vx=15;
	}
	pdat->cnow = opt_super_armer ? CELL_DASHF2 : CELL_DASHF;
	movex(pdat->vx);
	pdat->vx--;
	if(pdat->vx <=0){
		pdat->aid = ACTID_NEUTRAL;
	}
}

void CCharacter::act_dashb()
{
	if(pdat->counter == 0){
		clabul_wind->Go(!pdat->muki,pdat->x,0,0);
		pdat->vx = -15;
	}
	pdat->cnow = CELL_DASHB;
	movex(pdat->vx);
	pdat->vx++;
	if(pdat->vx >=0)pdat->aid = ACTID_NEUTRAL;
}


void CCharacter::act_timeoverlose()
{
	cell(CELL_TLOSE);
}