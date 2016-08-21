/*=====================================================================================

Goluah!! Copyright (C) 2001-2004 aki, 2004-2016 At-sushi, 2014-2016 logger

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

=======================================================================================*/

/*==============================================================

	行動関数・通常攻撃

================================================================*/
#include "character.h"


//*********************************************************
//通常.攻撃
//*********************************************************

/*---------------------------------------------------------
	立弱
-----------------------------------------------------------*/
void CCharacter::act_att_sa()
{
	if(ccounter==0)StartAttack(ATTACKINFO_STAND_A);

	if(ccounter<3)cell( CELL_SA1 );
	else if(ccounter<15)cell( CELL_SA2 );
	else ChangeAction(ACTID_NEUTRAL);
}

/*---------------------------------------------------------
	立中
-----------------------------------------------------------*/
void CCharacter::act_att_sb()
{
	if(ccounter==0)StartAttack(ATTACKINFO_STAND_B);

	if(ccounter<3)cell( CELL_SB1 );
	else if(ccounter<6)cell( CELL_SB2 );
	else if(ccounter<20)cell( CELL_SB3 );
	else if(ccounter<23)cell( CELL_SB2 );
	else ChangeAction(ACTID_NEUTRAL);
}

/*---------------------------------------------------------
	立強
-----------------------------------------------------------*/
void CCharacter::act_att_sc()
{
	if(ccounter==0)ComDelay(22);

	if(ccounter<3)cell( CELL_SC1 );
	else if(ccounter<6)cell( CELL_SC2 );
	else if(ccounter<9)cell( CELL_SC3 );
	else if(ccounter<40)
	{
		cell( CELL_SC4 + ccounter%4 );
		if(ccounter%5==0)StartAttack( ATTACKINFO_STAND_C );
	}
	else if(ccounter<45)cell( CELL_SC3 );
	else if(ccounter<50)cell( CELL_SC2 );
	else if(ccounter<55)cell( CELL_SC1 );
	else ChangeAction(ACTID_NEUTRAL);
}

/*---------------------------------------------------------
	屈弱
-----------------------------------------------------------*/
void CCharacter::act_att_ca()
{
	if(ccounter==0)StartAttack(ATTACKINFO_CROUCH_A);

	if(ccounter<3)cell( CELL_CA1 );
	else if(ccounter<15)cell( CELL_CA2 );
	else ChangeAction(ACTID_NEUTRAL);
}

/*---------------------------------------------------------
	屈中
-----------------------------------------------------------*/
void CCharacter::act_att_cb()
{
	if(ccounter==0){
		StartAttack(ATTACKINFO_CROUCH_B);
	}

	const UINT anmspd = 1;
	const UINT anmspd2 = 4;

	if(ccounter==0)ComDelay(anmspd*8+2);

	if(ccounter<anmspd)cell( CELL_CB1 );
	else if(ccounter<anmspd*2)cell( CELL_CB2 );
	else if(ccounter<anmspd*3)cell( CELL_CB3 );
	else if(ccounter<anmspd*4)cell( CELL_CB4 );
	else if(ccounter<anmspd*5)cell( CELL_CB5 );
	else if(ccounter<anmspd*6)cell( CELL_CB6 );
	else if(ccounter<anmspd*7)cell( CELL_CB7 );
	else if(ccounter<anmspd*8)cell( CELL_CB8 );
	else if(ccounter<anmspd*8+anmspd2*1)cell( CELL_CB7 );
	else if(ccounter<anmspd*8+anmspd2*2)cell( CELL_CB6 );
	else if(ccounter<anmspd*8+anmspd2*3)cell( CELL_CB5 );
	else if(ccounter<anmspd*8+anmspd2*4)cell( CELL_CB4 );
	else if(ccounter<anmspd*8+anmspd2*5)cell( CELL_CB3 );
	else if(ccounter<anmspd*8+anmspd2*6)cell( CELL_CB2 );
	else if(ccounter<anmspd*8+anmspd2*7)cell( CELL_CB1 );
	else ChangeAction( ACTID_CROUCH );
}

/*---------------------------------------------------------
	屈強
-----------------------------------------------------------*/
void CCharacter::act_att_cc()
{
	if(ccounter==0)StartAttack(ATTACKINFO_CROUCH_C);

	const UINT anmspd = 2;
	const UINT anmspd2 = 4;

	if(ccounter==0)ComDelay(anmspd*6+2);

	if(ccounter<anmspd)cell( CELL_CC1 );
	else if(ccounter<anmspd*2)cell( CELL_CC2 );
	else if(ccounter<anmspd*3)cell( CELL_CC3 );
	else if(ccounter<anmspd*4)cell( CELL_CC4 );
	else if(ccounter<anmspd*5)cell( CELL_CC5 );
	else if(ccounter<anmspd*6)cell( CELL_CC6 );
	else if(ccounter<anmspd*6+anmspd2*1)cell( CELL_CC6 );
	else if(ccounter<anmspd*6+anmspd2*2)cell( CELL_CC5 );
	else if(ccounter<anmspd*6+anmspd2*3)cell( CELL_CC4 );
	else if(ccounter<anmspd*6+anmspd2*4)cell( CELL_CC3 );
	else if(ccounter<anmspd*6+anmspd2*5)cell( CELL_CC2 );
	else if(ccounter<anmspd*6+anmspd2*6)cell( CELL_CC1 );
	else ChangeAction( ACTID_CROUCH );
}

/*---------------------------------------------------------
	飛弱
-----------------------------------------------------------*/
void CCharacter::act_att_ja()
{
	if(ccounter==0)StartAttack(ATTACKINFO_JAMP_A);

	if(ccounter<3)cell( CELL_JA1 );
	else if(ccounter<10)cell( CELL_JA2 );
	else if(ccounter<20)cell( CELL_JA1 );
	else ChangeAction( ACTID_RAKKA2 );

	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

/*---------------------------------------------------------
	飛中
-----------------------------------------------------------*/
void CCharacter::act_att_jb()
{
	if(ccounter==0)StartAttack(ATTACKINFO_JAMP_B);

	if(ccounter<3)cell( CELL_JB1 );
	else if(ccounter<6)cell( CELL_JB2 );
	else if(ccounter<15)cell( CELL_JB3 );
	else if(ccounter<20)cell( CELL_JB2 );
	else if(ccounter<26)cell( CELL_JB1 );
	else ChangeAction( ACTID_RAKKA2 );

	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

/*---------------------------------------------------------
	飛強
-----------------------------------------------------------*/
void CCharacter::act_att_jc()
{
	if(ccounter==0)StartAttack(ATTACKINFO_JAMP_C);

	const UINT anmspd = 2;

	if(ccounter<anmspd)cell( CELL_JC1 );
	else if(ccounter<anmspd*2)cell( CELL_JC2 );
	else if(ccounter<anmspd*3)cell( CELL_JC3 );
	else if(ccounter<anmspd*4)cell( CELL_JC4 );
	else if(ccounter<anmspd*5)cell( CELL_JC5 );
	else if(ccounter<anmspd*6)cell( CELL_JC6 );
	else if(ccounter<anmspd*7)cell( CELL_JC7 );
	else if(ccounter<anmspd*17)cell( CELL_JC8 );
	else ChangeAction( ACTID_RAKKA2 );

	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}



/*---------------------------------------------------------
	投げ
-----------------------------------------------------------*/
//自分動作
void CCharacter::act_nage()
{
	const UINT offset = 20;

	if(pdat->counter < offset)pdat->cnow=CELL_NAGE1;
	else if(pdat->counter < offset+25){
		//if(pdat->muki)
		{
			if(ccounter<offset+2)pdat->cnow=CELL_NAGE2;
			else if(ccounter<offset+15)pdat->cnow=CELL_NAGE3;
			else if(ccounter<offset+17)pdat->cnow=CELL_NAGE4;
			else if(ccounter<offset+19)pdat->cnow=CELL_NAGE5;
			else if(ccounter<offset+21)pdat->cnow=CELL_NAGE6;
			else if(ccounter<offset+23)pdat->cnow=CELL_NAGE7;
			else if(ccounter<offset+25)pdat->cnow=CELL_NAGE8;
		}
	}
	else pdat->aid = ACTID_NEUTRAL;

	if(pdat->counter == 0){
		AddEffect(EFCTID_MARKC,(int)pdat->x + zurex(50),-55,0);
		PlaySysSound(SYSTEMSOUND_CATCH);
	}
}

//敵側動作
DWORD CCharacter::eact_nage(GOBJECT *pedat)
{
	if(pedat->counter < 20){
		pedat->cnow=DCELL_DAMAGES1;
		pedat->x=pdat->x+zurex(50);
		pedat->y=0;
		pedat->muki = !pdat->muki;
		return(0);
	}
	else{
		StartAttack(ATTACKINFO_NAGE);
		AddDamage(pedat->id,(int)pdat->x + zurex(130),0);
		return(ACTID_FUTTOBI);
	}
}


/*---------------------------------------------------------
	キリバンゲット
-----------------------------------------------------------*/
void CCharacter::act_kiriget()
{
	if(ccounter==0)PlayMySound(wave_kiriget);
	if(ccounter==0)ComDelay(22);

	if(ccounter==0)
	{
		if(efe_wind)
		{
			efe_wind->Go(pdat->muki,
				pdat->x,
				0,
				2);
		}
		pdat->vx=15;
		StartAttack(ATTACKINFO_KIRIGET);
	}

	if(pdat->counter < 3)pdat->cnow = CELL_GET5;
	else if(pdat->counter < 25){
		pdat->cnow = CELL_GET1;
		movex(pdat->vx);
		pdat->vx-=0.5;
	}
	else if(pdat->counter < 30){
		pdat->cnow = CELL_GET2;
		movex(pdat->vx);
		pdat->vx-=0.5;
	}
	else if(pdat->counter < 35){pdat->cnow = CELL_GET3;}
	else if(pdat->counter < 40){pdat->cnow = CELL_GET4;}
	else if(pdat->counter < 45){pdat->cnow = CELL_GET5;}
	else{
		pdat->aid = ACTID_NEUTRAL;
	}
}

/*---------------------------------------------------------
	sageナックル
-----------------------------------------------------------*/
void CCharacter::act_sagena()
{
	if(ccounter==0)PlayMySound(wave_sage_na);

	if(ccounter==0)ComDelay(22);

	if(ccounter==0)
	{
		StartAttack(ATTACKINFO_SAGENA);
	}

	if(pdat->counter < 6)pdat->cnow = CELL_FUNBARI;
	else if(pdat->counter < 27){
		movex(5);
		if(pdat->counter < 9)pdat->cnow = CELL_SAGE1;
		else if(pdat->counter < 12){pdat->cnow = CELL_SAGE2;}
		else if(pdat->counter < 15){pdat->cnow = CELL_SAGE3;}
		else if(pdat->counter < 18){pdat->cnow = CELL_SAGE4;}
		else if(pdat->counter < 21){pdat->cnow = CELL_SAGE5;}
		else if(pdat->counter < 24){pdat->cnow = CELL_SAGE6;}
		else {pdat->cnow = CELL_SAGE7;}
	}
	else pdat->aid = ACTID_SAGENASUKI;
}

void CCharacter::act_sagenasuki()
{
	if(pdat->counter < 18){pdat->cnow = CELL_SAGE8;}
	else pdat->aid = ACTID_NEUTRAL;
}

/*---------------------------------------------------------
	ストライカー
-----------------------------------------------------------*/
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
		pdat->aid = ACTID_STRIKER_2GET;
	}
}

void CCharacter::act_striker_2get()
{
	if(ccounter==0)PlayMySound(wave_striker_atk);
	if(ccounter==0)
	{
		Furimuki();

		if(efe_wind)
		{
			efe_wind->Go(pdat->muki,
				pdat->x,
				0,
				2);
		}
		pdat->vx=15;
		StartAttack(ATTACKINFO_STRIKER2GET);
	}

	if(pdat->counter < 3)pdat->cnow = CELL_GET5;
	else if(pdat->counter < 25){
		pdat->cnow = CELL_GET1;
		movex(pdat->vx);
		pdat->vx-=0.5;
	}
	else if(pdat->counter < 30){
		pdat->cnow = CELL_GET2;
		movex(pdat->vx);
		pdat->vx-=0.5;
	}
	else if(pdat->counter < 35){pdat->cnow = CELL_GET3;}
	else if(pdat->counter < 40){pdat->cnow = CELL_GET4;}
	else if(pdat->counter < 45){pdat->cnow = CELL_GET5;}
	else{
		pdat->aid = ACTID_STRIKER_OUT;
	}
}

void CCharacter::act_striker_chainsaw()
{
	if(ccounter<3)cell( CELL_SC1 );
	else if(ccounter<6)cell( CELL_SC2 );
	else if(ccounter<9)cell( CELL_SC3 );
	else if(ccounter<20)
	{
		cell( CELL_SC4 + ccounter%4 );
		if(ccounter%5==0)StartAttack( ATTACKINFO_STRIKERCHAIN );
	}
	else if(ccounter<25)cell( CELL_SC3 );
	else if(ccounter<30)cell( CELL_SC2 );
	else if(ccounter<35)cell( CELL_SC1 );
	else ChangeAction(ACTID_STRIKER_OUT);
}

void CCharacter::act_striker_out()
{
	pdat->kasanari=FALSE;

	if(pdat->counter==0){
		pdat->objtype &= GOBJTYPE_NODISPLAY;//画面外へいけるようにしておく
		pdat->objtype |= GOBJFLG_TAIKI;//待機フラグを立てる
	}

	if(pdat->counter < 2){
		pdat->cnow = CELL_FUNBARI;
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


/*---------------------------------------------------------
	リバーサル / G.C
-----------------------------------------------------------*/
void CCharacter::act_gcancel1()
{
	if(ccounter==0){
		AddEffect(EFCTID_GCANCEL,(int)pdat->x,(int)pdat->y-50,pdat->muki);
		AddPowerGauge( -1.0*NEEDGAUGE_GC );
	}
	act_agetackle1( ATTACKINFO_GCANCEL );
}
void CCharacter::act_gcancel2()
{
	if(ccounter==0){
		AddEffect(EFCTID_GCANCEL,(int)pdat->x,(int)pdat->y-50,pdat->muki);
		AddPowerGauge( -1.0*NEEDGAUGE_GC );
	}
	act_agetackle2( ATTACKINFO_GCANCEL );
}
void CCharacter::act_gcancel3()
{
	if(ccounter==0){
		AddEffect(EFCTID_GCANCEL,(int)pdat->x,(int)pdat->y-50,pdat->muki);
		AddPowerGauge( -1.0*NEEDGAUGE_GC );
	}
	act_agetackle3( ATTACKINFO_GCANCEL );
}

void CCharacter::act_reversal()
{
	if(ccounter==0)AddPowerGauge( -1.0*NEEDGAUGE_REV );
	act_agetackle2( ATTACKINFO_REVERSAL );
}



