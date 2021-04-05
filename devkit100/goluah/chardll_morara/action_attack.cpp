
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

	if(ccounter<1)cell(CELL_SA1);
	else if(ccounter<2)cell(CELL_SA2);
	else if(ccounter<4)cell(CELL_SA3);//*
	else if(ccounter<9)cell(CELL_SA4);
	else if(ccounter<12)cell(CELL_SA5);
	else if(ccounter<15)cell(CELL_SA6);
	else ChangeAction(ACTID_NEUTRAL);
}

/*---------------------------------------------------------------
	立中
-----------------------------------------------------------------*/
void CCharacter::act_att_sb()
{
	if(ccounter==5)StartAttack(ATTACKINFO_STAND_B);

	movex(ccounter<10 ? 2 : 1);

	if(ccounter<2)cell(CELL_SB1);
	else if(ccounter<4)cell(CELL_SB2);
	else if(ccounter<6)cell(CELL_SB3);
	else if(ccounter<8)cell(CELL_SB4);
	else if(ccounter<10)cell(CELL_SB5);
	else if(ccounter<12)cell(CELL_SB6);//*
	else if(ccounter<15)cell(CELL_SB7);
	else if(ccounter<18)cell(CELL_SB8);
	else if(ccounter<21)cell(CELL_SB9);
	else if(ccounter<24)cell(CELL_SB10);
	else if(ccounter<27)cell(CELL_SB11);
	else if(ccounter<30)cell(CELL_SB12);
	else ChangeAction(ACTID_NEUTRAL);
}

/*---------------------------------------------------------------
	立中 - 追加入力
-----------------------------------------------------------------*/
void CCharacter::act_att_sb2()
{
	if(ccounter==5)StartAttack(ATTACKINFO_STAND_B2);

	movex(4);

	if(ccounter<2)cell(CELL_SB11);
	else if(ccounter<4)cell(CELL_SB10);
	else if(ccounter<6)cell(CELL_SB9);
	else if(ccounter<8)cell(CELL_SB8);
	else if(ccounter<10)cell(CELL_SB7);
	else if(ccounter<12)cell(CELL_SB6);//*
	else if(ccounter<16)cell(CELL_SB5);
	else if(ccounter<20)cell(CELL_SB4);
	else if(ccounter<24)cell(CELL_SB3);
	else if(ccounter<28)cell(CELL_SB2);
	else if(ccounter<36)cell(CELL_SB1);
	else ChangeAction(ACTID_NEUTRAL);
}

/*---------------------------------------------------------------
	立強
-----------------------------------------------------------------*/
void CCharacter::act_att_sc()
{
	if(ccounter==4)StartAttack(ATTACKINFO_STAND_C);

	if(ccounter<1)cell(CELL_SC1_1);
	else if(ccounter<2)cell(CELL_SC1_2);
	else if(ccounter<3)cell(CELL_SC1_3);
	else if(ccounter<4)cell(CELL_SC1_4);
	else if(ccounter<10)cell(CELL_SC1_5);//*
	else if(ccounter<12)cell(CELL_SC1_4);
	else if(ccounter<14)cell(CELL_SC1_3);
	else if(ccounter<16)cell(CELL_SC1_2);
	else if(ccounter<18)cell(CELL_SC1_1);
	else ChangeAction(ACTID_NEUTRAL);
}

/*---------------------------------------------------------------
	立強 - 追加入力
-----------------------------------------------------------------*/
void CCharacter::act_att_sc2()
{
	if(ccounter==0)PlayMySound(wave_kakato);
	if(pdat->counter == 0){pdat->vy=-12;pdat->vx=5;}//初期速度
	if(ccounter==5)StartAttack(ATTACKINFO_STAND_C2);

	if(ccounter<5)cell(CELL_SC2_1);
	else if(ccounter<9)cell(CELL_SC2_2);
	else if(ccounter<12)cell(CELL_SC2_3);
	else if(ccounter<15)cell(CELL_SC2_4);
	else if(ccounter<18)cell(CELL_SC2_5);
	else if(ccounter<20)cell(CELL_SC2_6);
	else if(ccounter<24)cell(CELL_SC2_7);//*
	else cell(CELL_SC2_8);

	if(ccounter>3)JiyuuRakka(1, TRUE, ACTID_LANDING);
}

/*---------------------------------------------------------------
	立強 - レバー前入れ
-----------------------------------------------------------------*/
void CCharacter::act_att_sc3()
{
	if(ccounter==0)PlayMySound(wave_punch);
	if(ccounter==5)StartAttack(ATTACKINFO_STAND_C3);

	if(ccounter==4)movex(20);
	if(ccounter==6)movex(15);
	if(ccounter==8)movex(5);

	if(ccounter<2){cell(CELL_SC3_1);}
	else if(ccounter<4){cell(CELL_SC3_2);}
	else if(ccounter<6){cell(CELL_SC3_3);}
	else if(ccounter<8){cell(CELL_SC3_4);}
	else if(ccounter<10){cell(CELL_SC3_5);}
	else if(ccounter<15)cell(CELL_SC3_6);//*
	else if(ccounter<19)cell(CELL_SC3_7);
	else if(ccounter<23)cell(CELL_SC3_8);
	else if(ccounter<27)cell(CELL_SC3_9);
	else ChangeAction(ACTID_NEUTRAL);
}


/*---------------------------------------------------------------
	屈弱
-----------------------------------------------------------------*/
void CCharacter::act_att_ca()
{
	if(ccounter==1)StartAttack(ATTACKINFO_CROUCH_A);

	if(ccounter<1)cell(CELL_CA1);
	else if(ccounter<3)cell(CELL_CA2);//*
	else if(ccounter<6)cell(CELL_CA3);
	else if(ccounter<9)cell(CELL_CA4);
	else ChangeAction(ACTID_CROUCH);
}

/*---------------------------------------------------------------
	屈中
-----------------------------------------------------------------*/
void CCharacter::act_att_cb()
{
	if(ccounter==1)StartAttack(ATTACKINFO_CROUCH_B);

	if(ccounter<1)cell(CELL_CB1);
	else if(ccounter<3){cell(CELL_CB2);movex(3);}
	else if(ccounter<10){cell(CELL_CB3);movex(4);}//*
	else if(ccounter<14){cell(CELL_CB4);movex(2);}
	else if(ccounter<20)cell(CELL_CB5);
	else if(ccounter<26)cell(CELL_CB6);
	else ChangeAction(ACTID_CROUCH);
}

/*---------------------------------------------------------------
	屈強
-----------------------------------------------------------------*/
void CCharacter::act_att_cc()
{
	if(ccounter==1)StartAttack(ATTACKINFO_CROUCH_C);

	if(ccounter<2)cell(CELL_CC1);
	else if(ccounter<4)cell(CELL_CC2);
	else if(ccounter<6)cell(CELL_CC3);
	else if(ccounter<8)cell(CELL_CC4);
	else if(ccounter<12)cell(CELL_CC5);//*
	else if(ccounter<16)cell(CELL_CC4);
	else if(ccounter<18)cell(CELL_CC3);
	else if(ccounter<24)cell(CELL_CC2);
	else if(ccounter<28)cell(CELL_CC1);
	else ChangeAction(ACTID_CROUCH);
}

/*---------------------------------------------------------------
	飛弱
-----------------------------------------------------------------*/
void CCharacter::act_att_ja()
{
	if(ccounter==1)StartAttack(ATTACKINFO_JAMP_A);

	if(ccounter<1)cell(CELL_JA1);
	else if(ccounter<2)cell(CELL_JA2);
	else if(ccounter<8)cell(CELL_JA3);//*
	else if(ccounter<11)cell(CELL_JA4);
	else cell(CELL_JA5);

	JiyuuRakka(2, TRUE, ACTID_NEUTRAL);
}

/*---------------------------------------------------------------
	飛中
-----------------------------------------------------------------*/
void CCharacter::act_att_jb()
{
	
	if(ccounter==5)StartAttack(ATTACKINFO_JAMP_B);

	if(ccounter<1)cell(CELL_SB1);
	else if(ccounter<2)cell(CELL_SB2);
	else if(ccounter<3)cell(CELL_SB3);
	else if(ccounter<4)cell(CELL_SB4);
	else if(ccounter<5)cell(CELL_SB5);
	else if(ccounter<12)cell(CELL_SB6);//*
	else if(ccounter<14)cell(CELL_SB5);
	else if(ccounter<16)cell(CELL_SB4);
	else if(ccounter<18)cell(CELL_SB3);
	else if(ccounter<20)cell(CELL_SB2);
	else cell(CELL_SB1);

	JiyuuRakka(2, TRUE, ACTID_NEUTRAL);
}

/*---------------------------------------------------------------
	飛強
-----------------------------------------------------------------*/
void CCharacter::act_att_jc()
{
	if(ccounter==1)StartAttack(ATTACKINFO_JAMP_C);

	if(ccounter<2)cell(CELL_JC1);
	else if(ccounter<3)cell(CELL_JC2);
	else if(ccounter<5)cell(CELL_JC3);//*
	else if(ccounter<6)cell(CELL_JC4);
	else if(ccounter<8)cell(CELL_JC5);
	else if(ccounter<20)cell(CELL_JC6);
	else if(ccounter<24)cell(CELL_JC7);
	else cell(CELL_JC8);

	JiyuuRakka(2, TRUE, ACTID_NEUTRAL);
}

/*---------------------------------------------------------------
	投げ
-----------------------------------------------------------------*/
//自分側
void CCharacter::act_nage()
{
	DrawBack();
	if(pdat->counter == 10+10)pdat->muki = !pdat->muki;

	if(pdat->counter < 10+10)		{DrawFront();pdat->cnow=CELL_JA2;}
	else if(pdat->counter < 13+10)	{pdat->cnow=CELL_CHIBA2;}
	else if(pdat->counter < 16+10)	{pdat->cnow=CELL_CHIBA3;}
	else if(pdat->counter < 19+10)	{pdat->cnow=CELL_CHIBA4;}
	else if(pdat->counter < 21+10)	{pdat->cnow=CELL_CHIBA5;}
	else if(pdat->counter < 24+10)	{pdat->cnow=CELL_CHIBA6;}
	else if(pdat->counter < 27+10)	{pdat->cnow=CELL_CHIBA7;}
	else if(pdat->counter < 30+10)	{pdat->cnow=CELL_CHIBA8;}
	else if(pdat->counter < 33+10)	{pdat->cnow=CELL_CHIBA9;}
	else pdat->aid = ACTID_NEUTRAL;

	if(pdat->counter == 0){
		AddEffect(EFCTID_MARKC,(int)pdat->x + zurex(50),-55,0);
		PlaySysSound(SYSTEMSOUND_CATCH);
	}
}

//敵側
DWORD CCharacter::eact_nage(GOBJECT *pedat)
{
	if(pedat->counter==18+10){
		StartAttack(ATTACKINFO_NAGE);
		AddDamage(pedat->id,(int)pdat->x + zurex(80),-70);
		pdat->kougeki = FALSE;
		pedat->vx = -18;
	}

	if(pedat->counter < 18+10){
		pedat->cnow=DCELL_DAMAGES1;
		pedat->x=pdat->x+zurex(50);
		pedat->y=0;
		pedat->muki = !pdat->muki;
		return(0);
	}
	else
	{
		pedat->x += pedat->muki ? -pedat->vx : pedat->vx;
		pedat->vx++;
		if(pedat->vx<-15)pedat->cnow=DCELL_DAMAGES1;
		else if(pedat->vx<-7)pedat->cnow=DCELL_BOUND;
		else pedat->cnow=DCELL_DOWN;

		if(pedat->vx>0){
			pedat->vy=0;
			return(ACTID_DOWN2);
		}
	}
	return(0);
}


/*---------------------------------------------------------------
	支援攻撃
-----------------------------------------------------------------*/
void CCharacter::act_striker_in()
{
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
		pdat->aid = ACTID_STRIKER_ATK1;
	}
}

void CCharacter::act_striker_atk1()
{
	if(pdat->counter==0)
	{
		Furimuki();
		pdat->vy=-15;
		pdat->vx =3;
		StartAttack(ATTACKINFO_STRIKER1);
		
		if(efe_track){
			efe_track->Startup(
				20.0f,
				3,
				5,
				PI*0.25f,
				0.8f,
				0.12f,
				80.0f,
				0.00f,
				80.0f);
		}
	}
	pdat->kasanari = FALSE;

	if(pdat->counter<2)cell(CELL_GENO2);
	else if(pdat->counter<4)cell(CELL_GENO3);
	else{
		if(pdat->vy<0)cell(CELL_GENO4);
		else ChangeAction( ACTID_STRIKER_ATK2 );
	}

	JiyuuRakka(1.8,TRUE,ACTID_STRIKER_OUT);
}

void CCharacter::act_striker_atk2()
{
	if(pdat->counter==0){
		pdat->vy-=3;
		//pdat->vx*=1.5;
		StartAttack(ATTACKINFO_STRIKER2);
		if(efe_track){
			efe_track->Startup(
				20.0f,
				3,
				8,
				PI*0.25f,
				0.8f,
				0.05f,
				120.0f,
				0.00f,
				50.0f,
				TRUE);
		}
	}
	pdat->kasanari = FALSE;

	if(ccounter<2)cell(CELL_SC2_1);
	else if(ccounter<4)cell(CELL_SC2_2);
	else if(ccounter<6)cell(CELL_SC2_3);
	else if(ccounter<8)cell(CELL_SC2_4);
	else if(ccounter<10)cell(CELL_SC2_5);
	else if(ccounter<12)cell(CELL_SC2_6);
	else if(ccounter<14)cell(CELL_SC2_7);
	else cell(CELL_SC2_8);

	JiyuuRakka(0.75f,TRUE,ACTID_STRIKER_OUT);
}

void CCharacter::act_striker_out()
{
	pdat->kasanari=FALSE;

	if(pdat->counter==0){
		pdat->objtype &= GOBJTYPE_NODISPLAY;//画面外へいけるようにしておく
		pdat->objtype |= GOBJFLG_TAIKI;//待機フラグを立てる
	}

	if(pdat->counter < 2){
		pdat->cnow = CELL_LANDING1;
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

