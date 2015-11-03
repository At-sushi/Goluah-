
/*==============================================================

	�ʏ�U��

================================================================*/
#include "character.h"


/*----------------------------------------------------------------
	��
------------------------------------------------------------------*/

void CCharacter::act_att_sa()//����
{
	if(pdat->counter == 1)StartAttack(ATTACKINFO_STAND_A);

	pdat->cnow = CELL_SA1;
	if(pdat->counter > 15){
		pdat->aid = ACTID_NEUTRAL;
	}
}

void CCharacter::act_att_sb()//����
{
	if(pdat->counter == 1)StartAttack(ATTACKINFO_STAND_B);

	if(pdat->counter < 2)pdat->cnow = CELL_SB1;
	else if(pdat->counter < 4)pdat->cnow = CELL_SB2;
	else if(pdat->counter < 10)pdat->cnow = CELL_SB3;
	else if(pdat->counter < 15)pdat->cnow = CELL_SB2;
	else if(pdat->counter < 20)pdat->cnow = CELL_SB1;
	else{
		pdat->aid = ACTID_NEUTRAL;
	}
}

void CCharacter::act_att_sc()//����
{
	if(pdat->counter ==5)StartAttack(ATTACKINFO_STAND_C);

	if(pdat->counter < 3)pdat->cnow = CELL_SC1;
	else if(pdat->counter < 6)pdat->cnow = CELL_SC2;
	else if(pdat->counter < 9)pdat->cnow = CELL_SC3;
	else if(pdat->counter < 10)pdat->cnow = CELL_SC4;
	else if(pdat->counter < 15)pdat->cnow = CELL_SC5;
	else if(pdat->counter < 20)pdat->cnow = CELL_SC3;
	else if(pdat->counter < 25)pdat->cnow = CELL_SC2;
	else if(pdat->counter < 30)pdat->cnow = CELL_SC1;
	else{
		pdat->aid = ACTID_NEUTRAL;
		
	}
}

/*----------------------------------------------------------------
	��
------------------------------------------------------------------*/

void CCharacter::act_att_ca()//����
{
	if(pdat->counter == 1)StartAttack(ATTACKINFO_CROUCH_A);

	pdat->cnow = CELL_CA1;
	if(pdat->counter > 15){
		pdat->aid = ACTID_CROUCH;
	}
}

void CCharacter::act_att_cb()//����
{
	if(pdat->counter == 1)StartAttack(ATTACKINFO_CROUCH_B);

	if(pdat->counter < 2)pdat->cnow = CELL_CB1;
	else if(pdat->counter < 4)pdat->cnow = CELL_CB2;
	else if(pdat->counter < 10)pdat->cnow = CELL_CB3;
	else if(pdat->counter < 15)pdat->cnow = CELL_CB2;
	else if(pdat->counter < 20)pdat->cnow = CELL_CB1;
	else{
		pdat->aid = ACTID_CROUCH;
	}
}

void CCharacter::act_att_cc()//����
{
	if(pdat->counter == 6)StartAttack(ATTACKINFO_CROUCH_C);

	if(pdat->counter < 3)pdat->cnow = CELL_CC1;
	else if(pdat->counter < 6)pdat->cnow = CELL_CC2;
	else if(pdat->counter < 7)pdat->cnow = CELL_CC3;
	else if(pdat->counter < 17)pdat->cnow = CELL_CC4;
	else if(pdat->counter < 22)pdat->cnow = CELL_CC2;
	else if(pdat->counter < 37)pdat->cnow = CELL_CC1;
	else{
		pdat->aid = ACTID_CROUCH;
	}
}

/*----------------------------------------------------------------
	��
------------------------------------------------------------------*/

void CCharacter::act_att_ja()//���
{
	if(pdat->counter == 1)StartAttack(ATTACKINFO_JAMP_A);

	if(pdat->counter < 3)pdat->cnow = CELL_JA1;
	else if(pdat->counter < 20)pdat->cnow = CELL_JA2;
	else pdat->aid = ACTID_RAKKA2;

	JiyuuRakka(ACTID_NEUTRAL,TRUE);
}

void CCharacter::act_att_jb()//��
{
	if(pdat->counter == 1)StartAttack(ATTACKINFO_JAMP_B);

	if(pdat->counter < 1)pdat->cnow = CELL_JB1;
	else if(pdat->counter < 2)pdat->cnow = CELL_JB2;
	else if(pdat->counter < 3)pdat->cnow = CELL_JB3;
	else if(pdat->counter < 4)pdat->cnow = CELL_JB4;
	else if(pdat->counter <10)pdat->cnow = CELL_JB5;
	else if(pdat->counter <15)pdat->cnow = CELL_JB4;
	else if(pdat->counter <20)pdat->cnow = CELL_JB3;
	else if(pdat->counter <25)pdat->cnow = CELL_JB2;
	else if(pdat->counter <30)pdat->cnow = CELL_JB1;
	else{
		pdat->aid = ACTID_RAKKA2;
	}
	JiyuuRakka(ACTID_NEUTRAL,TRUE);
}

void CCharacter::act_att_jc()//��
{
	if(pdat->counter%4 == 0)StartAttack(ATTACKINFO_JAMP_C);

	if(pdat->counter < 9)pdat->cnow=CELL_DASHF1;
	else if(pdat->counter < 18)pdat->cnow = CELL_JC1+pdat->counter%8;
	else pdat->cnow = CELL_JAMP;
	JiyuuRakka(ACTID_NEUTRAL,TRUE);
}



/*----------------------------------------------------------------
	�������
------------------------------------------------------------------*/
//��+C
void CCharacter::act_staff()
{
	if(ccounter==0)PlayMySound(wave_staff);
	if(ccounter==0)
	{
		pdat->vx = 1;
		pdat->vy = -8;
	}
	if(ccounter==16)
	{
		StartAttack( ATTACKINFO_STAFF );
	}

	JiyuuRakka(0.6,TRUE);

	UINT anmspd = 2;

	if(pdat->counter < anmspd)pdat->cnow = CELL_STAFF7;
	else if(pdat->counter < anmspd*2)pdat->cnow = CELL_STAFF6;
	else if(pdat->counter < anmspd*3)pdat->cnow = CELL_STAFF5;
	else if(pdat->counter < anmspd*4)pdat->cnow = CELL_STAFF4;
	else if(pdat->counter < anmspd*5)pdat->cnow = CELL_STAFF3;
	else if(pdat->counter < anmspd*6)pdat->cnow = CELL_STAFF2;
	else if(pdat->counter < anmspd*7)pdat->cnow = CELL_STAFF1;
	else if(pdat->counter < anmspd*8)pdat->cnow = CELL_STAFF2;
	else if(pdat->counter < anmspd*9)pdat->cnow = CELL_STAFF3;
	else if(pdat->counter < anmspd*10)pdat->cnow = CELL_STAFF4;
	else if(pdat->counter < anmspd*11)pdat->cnow = CELL_STAFF5;
	else if(pdat->counter < anmspd*12)pdat->cnow = CELL_STAFF6;
	else pdat->cnow = CELL_STAFF7;
}


/*----------------------------------------------------------------
	��
------------------------------------------------------------------*/
void CCharacter::act_nage()
{
	if(ccounter==0){
		AddEffect(EFCTID_MARKC,(int)pdat->x + zurex(50),-55,0);
		PlaySysSound(SYSTEMSOUND_CATCH);
	}

	if(pdat->counter < 25)pdat->cnow = CELL_NAGE1;
	else if(pdat->counter < 28)pdat->cnow = CELL_NAGE2;
	else if(pdat->counter < 39)pdat->cnow = CELL_NAGE3;
	else if(pdat->counter < 40)pdat->cnow = CELL_NAGE4;
	else if(pdat->counter < 80)pdat->cnow = CELL_NAGE5;
	else if(pdat->counter < 115)pdat->cnow = CELL_NAGE6;
	else if(pdat->counter < 120)pdat->cnow = CELL_NAGE7;
	else ChangeAction( ACTID_NEUTRAL );
}

DWORD CCharacter::eact_nage(GOBJECT *pedat)
{
	pedat->cnow = DCELL_NAGERARE1;
	float t;
	if(pedat->counter==0)
	{
		pedat->muki = !pdat->muki;
		m_nage_magx = pedat->magx;
		m_nage_magy = pedat->magy;
	}

	if(pedat->counter < 25){ pedat->x = pdat->x+zurex(50); pedat->y = 0;}
	else if(pedat->counter < 37)
	{
		t = ((float)pedat->counter-25)/(37.0f-25.0f);

		pedat->y = ( -1.0f + (2.0f*t-1.0)*(2.0f*t-1.0f) )*120.0f;
		pedat->x += pedat->muki ? -3.5 : 3.5;

		pedat->magx = pedat->magy = 1.0f - t;
	}
	else if(pedat->counter <80)
	{
		pedat->magx = pedat->magy = 0;

		if(pedat->counter == 53){		
			StartAttack(ATTACKINFO_NAGE);
			AddDamage(pedat->id,(int)pdat->x+zurex(15),-20);
		}
		if(pedat->counter == 55){		
			StartAttack(ATTACKINFO_NAGE);
			AddDamage(pedat->id,(int)pdat->x,-10);
		}
		if(pedat->counter == 57){		
			StartAttack(ATTACKINFO_NAGE);
			AddDamage(pedat->id,(int)pdat->x-zurex(15),-30);
		}
	}
	else if(pedat->counter <85)
	{
		t = (float)(pedat->counter-80) / 5.0f;
		pedat->magx = pedat->magy = t;

		pedat->y -= 20;
	}
	else return ACTID_FUTTOBI2;

	pedat->magx *= m_nage_magx;
	pedat->magy *= m_nage_magy;
	return 0;
}

/*----------------------------------------------------------------
	�X�g���C�J�[
------------------------------------------------------------------*/
void CCharacter::act_striker_in()
{
	if(ccounter==0)PlayMySound(wave_striker_in);
	pdat->kasanari=FALSE;

	if(pdat->counter==0){
		AddProperty( GOBJTYPE_DISPLAY );//��ʓ��ɂ���
	}

	GOBJECT *pedat;
	if(pdat->counter==0){
		pedat = GetActiveEnemy();
		if(pedat!=NULL){
			if(pedat->x>0){
				pdat->x = pedat->x -(400);
				pdat->muki=FALSE;
				pdat->vx=14;
			}
			else{
				pdat->x = pedat->x +(400);
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

	pdat->cnow = CELL_JAMP;
	if(pdat->y > 0){
		pdat->y=0;
		pdat->aid = ACTID_STRIKER_ATTACK;
	}
}

void CCharacter::act_striker_attack()
{
	if(ccounter==0)PlayMySound(wave_striker_atk);
	act_poi_missile_v2( ccounter%15 );
	if(ccounter > 15*3)ChangeAction(ACTID_STRIKER_OUT);
}

void CCharacter::act_striker_out()
{
	pdat->kasanari=FALSE;

	if(pdat->counter==0){
		pdat->objtype &= GOBJTYPE_NODISPLAY;//��ʊO�ւ�����悤�ɂ��Ă���
		pdat->objtype |= GOBJFLG_TAIKI;//�ҋ@�t���O�𗧂Ă�
	}

	if(pdat->counter < 2){
		pdat->cnow = CELL_CROUCH;
		pdat->vx = -12;
		pdat->vy = -24;
	}
	else {
		movex(pdat->vx);
		pdat->y += pdat->vy;
		pdat->vy++;
		if(pdat->vy>-10)pdat->vy=-10;
		pdat->cnow = CELL_JAMP;
		
		if(pdat->counter > 60){
			pdat->aid = ACTID_TAIKICYU;
			pdat->x = 2000;
			pdat->y = -2000;
		}
	}
}


/*----------------------------------------------------------------
	���o�[�T��
------------------------------------------------------------------*/
void CCharacter::act_reversal()
{
	if(ccounter==0)
	{
		pdat->vx = 0.0f;
		pdat->vy = -15.0f;
		pdat->muteki = TRUE;
		pdat->nagerare = FALSE;

		AddPowerGauge( -1.0*NEEDGAUGE_REV );
	}

	if(pdat->counter%3 == 0)StartAttack(ATTACKINFO_REVERSAL);

	if(pdat->vy>0)pdat->muteki = FALSE;

	if(pdat->counter < 6)pdat->cnow=CELL_DASHF1;
	else if(pdat->counter < 18)pdat->cnow = CELL_REVERSAL1+pdat->counter%8;
	else pdat->cnow = CELL_JAMP;
	JiyuuRakka(ACTID_NEUTRAL,TRUE);
}


