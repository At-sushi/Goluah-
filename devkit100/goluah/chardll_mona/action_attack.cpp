
/*==============================================================

	行動関数（通常攻撃技）

================================================================*/
#include "character.h"


/*---------------------------------------------------------------
	立弱
-----------------------------------------------------------------*/
void CCharacter::act_att_sa()
{
	if(ccounter==2)StartAttack(ATTACKINFO_STAND_A);

	else if(ccounter<6)cell(CELL_SA2);//*
	else if(ccounter<9)cell(CELL_SA3);
	else if(ccounter<15)cell(CELL_SA4);
	else ChangeAction(ACTID_NEUTRAL);
}

/*---------------------------------------------------------------
	立中
-----------------------------------------------------------------*/
void CCharacter::act_att_sb()
{
	if(ccounter==5)StartAttack(ATTACKINFO_STAND_B);
	if(ccounter==18)StartAttack(ATTACKINFO_STAND_B);
	pdat->z = ZZAHYO_CHARACTER_F;

	movex(3);

	if(ccounter<4)cell(CELL_SB1);
	else if(ccounter<8)cell(CELL_SB2);
	else if(ccounter<12)cell(CELL_SB3);
	else if(ccounter<15)cell(CELL_SB4);
	else if(ccounter<18)cell(CELL_SB5);
	else if(ccounter<24)cell(CELL_SB6);
	else if(ccounter<30)cell(CELL_SB7);
	else ChangeAction(ACTID_NEUTRAL);
}

/*---------------------------------------------------------------
	立強
-----------------------------------------------------------------*/
void CCharacter::act_att_sc()
{
	if(ccounter==4)StartAttack(ATTACKINFO_STAND_C);
	pdat->z = ZZAHYO_CHARACTER_B;

	if(ccounter<9)movex(2);

	if(ccounter<3)cell(CELL_SC1);
	else if(ccounter<5)cell(CELL_SC2);
	else if(ccounter<7)cell(CELL_SC3);
	else if(ccounter<9)cell(CELL_SC4);
	else if(ccounter<20)cell(CELL_SC5);//*
	else if(ccounter<24)cell(CELL_SC6);
	else if(ccounter<28)cell(CELL_SC7);
	else ChangeAction(ACTID_NEUTRAL);
}

/*---------------------------------------------------------------
	立中 - レバー前入れ
-----------------------------------------------------------------*/
void CCharacter::act_att_sd()
{
	if(ccounter==0)PlayMySound(wave_body_ga);

	if(ccounter==5)StartAttack(ATTACKINFO_STAND_D);
	pdat->z = ZZAHYO_CHARACTER_F;

	if(ccounter==0)movex(40);
	if(ccounter==5)movex(13);
	if(ccounter==9)movex(20);
	if(ccounter==11)movex(26);
	if(ccounter==13)movex(10);

	if(ccounter<3)cell(CELL_SD1);
	else if(ccounter<5)cell(CELL_SD2);
	else if(ccounter<7)cell(CELL_SD3);
	else if(ccounter<9)cell(CELL_SD4);
	else if(ccounter<11)cell(CELL_SD5);
	else if(ccounter<13)cell(CELL_SD6);
	else if(ccounter<15)cell(CELL_SD7);
	else if(ccounter<21)cell(CELL_SD8);
	else if(ccounter<26)cell(CELL_SD7);
	else if(ccounter<31)cell(CELL_SD6);
	else ChangeAction(ACTID_NEUTRAL);
}

/*---------------------------------------------------------------
	立強 - レバー前入れ
-----------------------------------------------------------------*/
void CCharacter::act_att_se()
{
	if(ccounter==0)PlayMySound(wave_kurae);

	if(ccounter==5)StartAttack(ATTACKINFO_STAND_E);

	if(ccounter==2)movex(36);
	if(ccounter==6)movex(44);
	if(ccounter==8)movex(22);
	if(ccounter==10)movex(13);
	if(ccounter==15)movex(8);
	if(ccounter==19)movex(9);

	if(ccounter<2)cell(CELL_SE1);
	else if(ccounter<4)cell(CELL_SE2);
	else if(ccounter<6)cell(CELL_SE3);
	else if(ccounter<8)cell(CELL_SE4);
	else if(ccounter<10)cell(CELL_SE5);
	else if(ccounter<15)cell(CELL_SE6);//*
	else if(ccounter<19)cell(CELL_SE7);
	else if(ccounter<23)cell(CELL_SE8);
	else ChangeAction(ACTID_NEUTRAL);
}


/*---------------------------------------------------------------
	屈弱
-----------------------------------------------------------------*/
void CCharacter::act_att_ca()
{
	if(ccounter==1)StartAttack(ATTACKINFO_CROUCH_A);

	if(ccounter<5)cell(CELL_CA1);
	else if(ccounter<15)cell(CELL_CA2);
	else ChangeAction(ACTID_CROUCH);
}

/*---------------------------------------------------------------
	屈中
-----------------------------------------------------------------*/
void CCharacter::act_att_cb()
{
	if(ccounter==1)StartAttack(ATTACKINFO_CROUCH_B);
	pdat->z = ZZAHYO_CHARACTER_F;

	if(ccounter<2)cell(CELL_CB1);
	else if(ccounter<4)cell(CELL_CB2);
	else if(ccounter<6)cell(CELL_CB3);
	else if(ccounter<14)cell(CELL_CB4);
	else if(ccounter<18)cell(CELL_CB3);
	else if(ccounter<22)cell(CELL_CB2);
	else if(ccounter<26)cell(CELL_CB1);
	else ChangeAction(ACTID_CROUCH);
}

/*---------------------------------------------------------------
	屈強
-----------------------------------------------------------------*/
void CCharacter::act_att_cc()
{
	if(ccounter==1)StartAttack(ATTACKINFO_CROUCH_C);
	pdat->z = ZZAHYO_CHARACTER_B;

	if(ccounter<2)cell(CELL_CC1);
	else if(ccounter<4)cell(CELL_CC2);
	else if(ccounter<6)cell(CELL_CC3);
	else if(ccounter<8)cell(CELL_CC4);
	else if(ccounter<10)cell(CELL_CC5);
	else if(ccounter<12)cell(CELL_CC6);
	else if(ccounter<20)cell(CELL_CC7);
	else if(ccounter<25)cell(CELL_CC8);
	else if(ccounter<30)cell(CELL_CC9);
	else ChangeAction(ACTID_CROUCH);
}

/*---------------------------------------------------------------
	飛弱
-----------------------------------------------------------------*/
void CCharacter::act_att_ja()
{
	if(ccounter==1)StartAttack(m_aerial ? ATTACKINFO_AERIAL_A : ATTACKINFO_JAMP_A );

	if(ccounter<2)cell(CELL_JA1);
	else if(ccounter<4)cell(CELL_JA2);
	else if(ccounter<14)cell(CELL_JA3);
	else if(ccounter<17)cell(CELL_JA2);
	else if(ccounter<20)cell(CELL_JA1);
	else if(ccounter<24)ChangeAction(ACTID_RAKKA);

	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

/*---------------------------------------------------------------
	飛中
-----------------------------------------------------------------*/
void CCharacter::act_att_jb()
{	
	if(ccounter==5)StartAttack(m_aerial ? ATTACKINFO_AERIAL_B : ATTACKINFO_JAMP_B);
	
	if(!m_aerial){
		if(ccounter<2)cell(CELL_JB1);
		else if(ccounter<4)cell(CELL_JB2);
		else if(ccounter<6)cell(CELL_JB3);
		else if(ccounter<16)cell(CELL_JB4);
		else if(ccounter<20)cell(CELL_JB3);
		else if(ccounter<24)cell(CELL_JB2);
		else if(ccounter<28)cell(CELL_JB1);
		else if(ccounter<32)ChangeAction(ACTID_RAKKA);
	}
	else{
		if(ccounter<1)cell(CELL_JB1);
		else if(ccounter<2)cell(CELL_JB2);
		else if(ccounter<3)cell(CELL_JB3);
		else if(ccounter<16)cell(CELL_JB4);
		else if(ccounter<20)cell(CELL_JB3);
		else if(ccounter<24)cell(CELL_JB2);
		else if(ccounter<28)cell(CELL_JB1);
		else if(ccounter<32)ChangeAction(ACTID_RAKKA);
	}

	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

/*---------------------------------------------------------------
	飛強
-----------------------------------------------------------------*/
void CCharacter::act_att_jc()
{
	if(ccounter==1)StartAttack(m_aerial ? ATTACKINFO_AERIAL_C : ATTACKINFO_JAMP_C);

	if(!m_aerial){
		if(ccounter<2)cell(CELL_JC1);
		else if(ccounter<4)cell(CELL_JC2);
		else if(ccounter<6)cell(CELL_JC3);
		else if(ccounter<16)cell(CELL_JC4);
		else if(ccounter<20)cell(CELL_JC5);
	}
	else{
		if(ccounter<1)cell(CELL_JC1);
		else if(ccounter<2)cell(CELL_JC2);
		else if(ccounter<3)cell(CELL_JC3);
		else if(ccounter<16)cell(CELL_JC4);
		else if(ccounter<20)cell(CELL_JC5);
	}

	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

/*---------------------------------------------------------------
	投げ
-----------------------------------------------------------------*/
void CCharacter::act_nage()
{
	DrawFront();

	if(pdat->counter < 10+10)pdat->cnow=CELL_SB1;
	else if(pdat->counter < 14+10){pdat->cnow=CELL_SB2;}
	else if(pdat->counter < 18+10){pdat->cnow=CELL_SB4;}
	else if(pdat->counter < 20+10){pdat->cnow=CELL_SB5;}
	else if(pdat->counter < 24+10){pdat->cnow=CELL_SB6;}
	else if(pdat->counter < 40+10){pdat->cnow=CELL_SB7;}
	else pdat->aid = ACTID_NEUTRAL;

	if(pdat->counter == 0){
		AddEffect(EFCTID_MARKC,(int)pdat->x + zurex(50),-55,0);
		PlaySysSound(SYSTEMSOUND_CATCH);
	}
}

//投げ（敵側）
DWORD CCharacter::eact_nage(GOBJECT *pedat)
{
	if(pedat->counter < 18+10){
		pedat->cnow=DCELL_DAMAGES1;
		pedat->x=pdat->x+zurex(50);
		pedat->y=0;
		pedat->muki = !pdat->muki;
		return(0);
	}
	else{
		StartAttack(ATTACKINFO_NAGE);
		AddDamage(pedat->id,(int)pdat->x + zurex(80),-70);
		EndAttack();
		return(ACTID_FUTTOBI);
	}
	return(0);
}


/*---------------------------------------------------------------
	支援攻撃
-----------------------------------------------------------------*/
void CCharacter::act_striker_in()
{
	if(ccounter==0)PlayMySound(wave_striker_in);
	pdat->kasanari=FALSE;

	if(pdat->counter==0){
		AddProperty( GOBJTYPE_DISPLAY );//画面内にいる
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

	pdat->cnow = CELL_JAMP1;
	if(pdat->y > 0){
		pdat->y=0;
		pdat->aid = ACTID_STRIKER_DASH;
	}
}

void CCharacter::act_striker_dash()
{
	if(ccounter==0)
	{
		Furimuki();
		StartAttack( ATTACKINFO_STRIKER_DASH );
		pdat->vx = 10;
	}

	cell( CELL_STRIKER_DASH );
	movex(pdat->vx);
	pdat->vx--;
	if(pdat->vx<0)ChangeAction( ACTID_STRIKER_ATTACK );
}

void CCharacter::act_striker_attack()
{
	if(ccounter==0)PlayMySound(wave_striker_atk);
	if(pdat->counter==0){
		pdat->vy=-8;
	}
	pdat->kasanari=FALSE;

	if(pdat->vy < 0)
	{
		pdat->kasanari=FALSE;

		pdat->y += pdat->vy;
		pdat->vy += 0.4;//ゆっくり

		switch( (pdat->counter+2)%5 )
		{
		case 0:cell(CELL_TATU1);pdat->revx=FALSE;break;
		case 1:cell(CELL_TATU2);pdat->revx=FALSE;break;
		case 2:cell(CELL_TATU3);pdat->revx=FALSE;break;
		case 3:cell(CELL_TATU2);pdat->revx=TRUE;break;
		case 4:cell(CELL_TATU1);pdat->revx=TRUE;break;
		}
		if(pdat->cnow == CELL_TATU3)StartAttack(ATTACKINFO_STRIKER);
	}
	else
	{
		pdat->y += pdat->vy;
		pdat->vy += 0.4;//ゆっくり
		pdat->cnow = CELL_TATU4;
	
		if(pdat->y > 0){
			pdat->y=-0.001;
			pdat->cnow = CELL_NEUTRAL;
			if(pdat->counter>60){
				pdat->aid = ACTID_STRIKER_OUT;
			}
		}
	}
}

void CCharacter::act_striker_out()
{
	pdat->kasanari=FALSE;

	if(pdat->counter==0){
		pdat->objtype &= GOBJTYPE_NODISPLAY;//画面外へいけるようにしておく
		pdat->objtype |= GOBJFLG_TAIKI;//待機フラグを立てる
	}

	if(pdat->counter < 2){
		pdat->cnow = CELL_TATU5;
		pdat->vx = -12;
		pdat->vy = -24;
	}
	else {
		movex(pdat->vx);
		pdat->y += pdat->vy;
		pdat->vy++;
		if(pdat->vy>-10)pdat->vy=-10;
		pdat->cnow = CELL_JAMP1;
		
		if(pdat->counter > 60){
			pdat->aid = ACTID_TAIKICYU;
			pdat->x = 2000;
			pdat->y = -2000;
		}
	}
}