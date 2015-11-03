/*==============================================================

	�s���֐�(�ʏ�U��)

================================================================*/
#include "character.h"


void CCharacter::act_att_sa()//����
{
	pdat->cnow = CELL_SA;
	if(pdat->counter > 15){
		pdat->aid = ACTID_NEUTRAL;
	}

	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_STAND_A);
	}
}

void CCharacter::act_att_sb()//����
{
	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_STAND_B);
	}

	if(pdat->counter < 15){pdat->cnow = CELL_SB;movex(4);}
	else if(pdat->counter < 25){pdat->cnow = CELL_SB;movex(2);}
	else{
		pdat->aid = ACTID_NEUTRAL;
	}
}

void CCharacter::act_att_sc()//����
{
	if(pdat->counter < 5)pdat->cnow = CELL_SC1;
	else if(pdat->counter < 25){pdat->cnow = CELL_SC2;movex(4);}
	else if(pdat->counter < 35){pdat->cnow = CELL_SC2;movex(2);}
	else{
		pdat->aid = ACTID_NEUTRAL;
		
	}

	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_STAND_C);
	}
}

void CCharacter::act_att_ca()//����
{
	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_CROUCH_A);
	}

	pdat->cnow = CELL_CA;
	if(pdat->counter > 15){
		pdat->aid = ACTID_CROUCH;
	}
}

void CCharacter::act_att_cb()//����
{
	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_CROUCH_B);
	}

	if(pdat->counter < 3)pdat->cnow = CELL_CB1;
	else if(pdat->counter < 10)pdat->cnow = CELL_CB2;
	else if(pdat->counter < 30)pdat->cnow = CELL_CB1;
	else{
		pdat->aid = ACTID_CROUCH;
	}
}

void CCharacter::act_att_cc()//����
{
	if(pdat->counter == 10){
		StartAttack(ATTACKINFO_CROUCH_C);
	}

	if(pdat->counter < 20){pdat->cnow = CELL_CC;movex(3);}
	else if(pdat->counter < 20){pdat->cnow = CELL_CC;movex(1);}
	else{
		pdat->aid = ACTID_CROUCH;
	}
}

void CCharacter::act_att_ja()//���
{
	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_JAMP_A);
	}

	pdat->cnow = CELL_JA;
	if(pdat->counter > 20)pdat->aid = ACTID_RAKKA;

	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy++;
	if(pdat->y > 0){
		pdat->aid = ACTID_NEUTRAL;
		pdat->y =0;
		Furimuki();
	}
}

void CCharacter::act_att_jb()//��
{
	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_JAMP_B);
	}

	pdat->cnow = CELL_JB;
	if(pdat->counter > 10)pdat->aid = ACTID_RAKKA;

	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy++;
	if(pdat->y > 0){
		pdat->aid = ACTID_NEUTRAL;
		pdat->y =0;
		Furimuki();
	}
}

void CCharacter::act_att_jc()//��
{
	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_JAMP_C);
	}

	if(pdat->counter < 4)pdat->cnow = CELL_JC1;
	else if(pdat->counter < 15)pdat->cnow = CELL_JC2;
	else if(pdat->counter < 30)pdat->cnow = CELL_JAMP1;
	else pdat->aid = ACTID_RAKKA;

	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy++;
	if(pdat->y > 0){
		pdat->aid = ACTID_NEUTRAL;
		pdat->y =0;
		Furimuki();
	}
}

//�x���U��=========================================================

void CCharacter::act_striker1()//���ł���g�R�~�\���˗���
{
	pdat->kasanari=FALSE;
	GOBJECT *pedat;

	if(pdat->counter==0){
		pdat->objtype |= GOBJTYPE_DISPLAY;//��ʓ��ɂ���
		PlayMySound(wave_striker);

		if(pdat->tid==TEAM_PLAYER1)
			pedat = GetActiveCharacter(TEAM_PLAYER2);
		else
			pedat = GetActiveCharacter(TEAM_PLAYER1);
		if(pedat!=NULL){
			pdat->x = pedat->x;
			pdat->vx=0;
		}
		else {
			pdat->x=0;
			pdat->vx=0;
		}
		pdat->y = -600;
		pdat->vy= 10;
		//�U��
		StartAttack(ATTACKINFO_STRIKER);
	}

	pdat->y += pdat->vy;
	pdat->vy++;

	pdat->cnow = CELL_STRIKER1;
	if(pdat->y > 0){
		AddEffect(EFCTID_GNDCIR,(int)pdat->x,0,FALSE);
		pdat->y=0;
		pdat->aid = ACTID_STRIKER2;
	}
}

void CCharacter::act_striker2()//�U��(1)
{
	if(pdat->counter>20)pdat->aid=ACTID_STRIKER4;
}

void CCharacter::act_striker3()//�U��(2)
{
}

void CCharacter::act_striker4()
{
	pdat->kasanari=FALSE;

	if(pdat->counter==0){
		pdat->objtype &= GOBJTYPE_NODISPLAY;//��ʊO�ւ�����悤�ɂ��Ă���
		pdat->objtype |= GOBJFLG_TAIKI;//�ҋ@�t���O�𗧂Ă�
	}

	if(pdat->counter < 2){
		pdat->cnow = CELL_STRIKER2;
		pdat->vx = -12;
		pdat->vy = -24;
	}
	else {
		movex(pdat->vx);
		pdat->y += pdat->vy;
		pdat->vy++;
		if(pdat->vy>-10)pdat->vy=-10;
		pdat->cnow = CELL_STRIKER2;
		
		if(pdat->counter > 60){
			pdat->aid = ACTID_TAIKICYU;
			pdat->x = 2000;
			pdat->y = -2000;
			pdat->objtype |= GOBJFLG_TOBETARGET;//�^�[�Q�b�g�ɂȂ�
		}
	}
}

//����======================================================================

void CCharacter::act_nage()
{
	DrawFront();

	if(pdat->counter < 20)pdat->cnow=CELL_NAGE1;
	else if(pdat->counter < 60){pdat->cnow=CELL_NAGE2;movex(-1);}
	else pdat->aid = ACTID_NEUTRAL;

	if(pdat->counter == 0){
		PlayMySound(wave_nage);
		AddEffect(EFCTID_MARKC,(int)pdat->x + zurex(50),-100,0);
		PlaySysSound(SYSTEMSOUND_CATCH);
	}
}

DWORD CCharacter::act_nagee(GOBJECT *pedat)
{
	if(pedat->counter < 20){
		pedat->cnow=DCELL_NAGERARE1;
		pedat->x=pdat->x+zurex(50);
		pedat->y=-100;
		pedat->muki = !pdat->muki;
		return(0);
	}
	else{
		StartAttack(ATTACKINFO_NAGE);
		AddDamage(pedat->id,(int)pedat->x,(int)pedat->y);
		//pedat->muki = !pedat->muki; �U���̂̂���������ɍ��E�����
		return(ACTID_FUTTOBI);
	}
}

//�K�[�h�L�����Z��

void CCharacter::act_gcancel1()
{
	if(pdat->counter==0)
	{
		AddPowerGauge(-1.0f*NEEDGAUGE_GC);
		AddEffect(EFCTID_GCANCEL,(int)pdat->x,(int)pdat->y-120,pdat->muki);
	}

	if(ccounter<2)cell(CELL_GCANCEL1);
	else if(ccounter<4)cell(CELL_GCANCEL2);
	else if(ccounter<10)cell(CELL_GCANCEL3);
	else ChangeAction(ACTID_NEUTRAL);

	if(ccounter==0)StartAttack(ATTACKINFO_GCANCEL1);
	if(ccounter==6)EndAttack();
}

void CCharacter::act_gcancel2()
{
	if(pdat->counter==0)
	{
		AddPowerGauge(-1.0f*NEEDGAUGE_GC);
		AddEffect(EFCTID_GCANCEL,(int)pdat->x,(int)pdat->y-120,pdat->muki);

		pdat->vx = 8;
	}

	if(ccounter<2)cell(CELL_GCANCEL1);
	else if(ccounter<4)cell(CELL_GCANCEL2);
	else if(ccounter<20){
		if(pdat->vx>0){
			movex(pdat->vx);
			pdat->vx--;
		}
		cell(CELL_GCANCEL3);
	}
	else ChangeAction(ACTID_NEUTRAL);

	if(ccounter==0)StartAttack(ATTACKINFO_GCANCEL1);
	if(ccounter==10)EndAttack();
}

void CCharacter::act_gcancel3()
{
	if(pdat->counter==0)
	{
		AddPowerGauge(-1.0f*NEEDGAUGE_GC);
		AddEffect(EFCTID_GCANCEL,(int)pdat->x,(int)pdat->y-120,pdat->muki);

		pdat->vx = 12;
	}

	if(ccounter<2)cell(CELL_GCANCEL1);
	else if(ccounter<4)cell(CELL_GCANCEL2);
	else if(ccounter<30){
		if(pdat->vx>0){
			movex(pdat->vx);
			pdat->vx-=0.5f;
		}
		cell(CELL_GCANCEL3);
	}
	else ChangeAction(ACTID_NEUTRAL);

	if(ccounter==0)StartAttack(ATTACKINFO_GCANCEL1);
	if(ccounter==18)EndAttack();
}

//�o�b�g
void CCharacter::act_bat()
{
	if(ccounter < 8)cell(CELL_BAT1);
	else if(ccounter < 11){cell(CELL_BAT2);}
	else if(ccounter < 14){cell(CELL_BAT3);}
	else if(ccounter < 17){cell(CELL_BAT4);}
	else if(ccounter < 35){cell(CELL_BAT5);}
	else{
		pdat->aid = ACTID_NEUTRAL;
	}

	if(ccounter==8)movex(42);
	if(ccounter==17)movex(50);

	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_BAT);
	}
	if(ccounter==20)EndAttack();
}

//�A�b�p�[(���ݍ���)
void CCharacter::act_upper_step()
{
	if(ccounter<6)
	{
		cell(CELL_UPPER1);
		movex(8);
	}
	else if(ccounter<12)
	{
		cell(CELL_UPPER2);
		movex(8);
	}
	else ChangeAction(ACTID_UPPER_JAMP);
}

//�A�b�p�[(�W�����v)
void CCharacter::act_upper_jamp()
{
	if(pdat->counter==0)
	{
		StartAttack(ATTACKINFO_UPPER);
		pdat->vy = -14;
		pdat->vx = 1;
	}

	if(ccounter < 3)cell(CELL_UPPER3);
	else if(ccounter < 6){cell(CELL_UPPER4);}
	else if(ccounter < 9){cell(CELL_UPPER5);}
	else if(pdat->vy>0)
	{
		cell(CELL_JAMP1);
	}

	JiyuuRakka(1.2f,TRUE,ACTID_UPPER_LANDING);
	movex(pdat->vx);
}

//�A�b�p�[(���n����)
void CCharacter::act_upper_landing()
{
	if(ccounter<5)
	{
		cell(CELL_LANDING);
	}
	else ChangeAction(ACTID_NEUTRAL);
}

//��?
void CCharacter::act_hand()
{
	if(ccounter < 4)cell(CELL_HAND1);
	else if(ccounter < 8){cell(CELL_HAND2);}
	else if(ccounter < 10){cell(CELL_HAND3);}
	else if(ccounter < 12){cell(CELL_HAND4);}
	else if(ccounter < 40){
		cell(CELL_HAND5+(ccounter%3));
		if(pdat->cnow==CELL_HAND7)StartAttack(ATTACKINFO_HAND);
	}
	else if(ccounter < 42)cell(CELL_HAND4);
	else if(ccounter < 44)cell(CELL_HAND3);
	else if(ccounter < 46)cell(CELL_HAND2);
	else{
		pdat->aid = ACTID_NEUTRAL;
	}

	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_STAND_C);
	}
}

//�n�C�L�b�N
void CCharacter::act_hkick()
{
	if(ccounter < 4)cell(CELL_HKICK1);
	else if(ccounter < 6){cell(CELL_HKICK2);}
	else if(ccounter < 12){cell(CELL_HKICK3);}
	else if(ccounter < 15){cell(CELL_HKICK1);}
	else{
		pdat->aid = ACTID_NEUTRAL;
	}

	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_HKICK1);
	}
	if(pdat->counter==6){
		StartAttack(ATTACKINFO_HKICK2);
	}
	if(pdat->counter==8){
		EndAttack();
	}
}

