/*=====================================================================================

Goluah!! Copyright (C) 2001-2004 aki, 2004-2016 At-sushi, 2014-2016 logger

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

=======================================================================================*/

/*==============================================================

	行動関数

================================================================*/
#include "character.h"


//*********************************************************
//通常.攻撃
//*********************************************************

void CCharacter::act_att_sa()//立弱
{
	if(pdat->counter < 3)pdat->cnow = CELL_SA1;
	else if(pdat->counter < 9)pdat->cnow = CELL_SA3;
	else if(pdat->counter <13)pdat->cnow = CELL_SA2;
	else if(pdat->counter <17)pdat->cnow = CELL_SA1;
	else{
		pdat->aid = ACTID_NEUTRAL;
	}

	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_STAND_A);
	}
}

void CCharacter::act_att_sb()//立中
{
	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_STAND_B);
	}

	if(pdat->counter < 5)pdat->cnow = CELL_SB1;
	else if(pdat->counter < 15){pdat->cnow = CELL_SB2;movex(3);}
	else if(pdat->counter < 20)pdat->cnow = CELL_SA1;
	else{
		pdat->aid = ACTID_NEUTRAL;
	}
}

void CCharacter::act_att_sc()//立強
{
	DrawFront();

	if(pdat->counter < 6)pdat->cnow = CELL_SC1;
	else if(pdat->counter < 30){pdat->cnow = CELL_SC2 + pdat->counter%4;}
	else if(pdat->counter < 40)pdat->cnow = CELL_SA1;
	else{
		pdat->aid = ACTID_NEUTRAL;
	}

	StartAttack(ATTACKINFO_STAND_C);
}

void CCharacter::act_att_ca()//屈弱
{
	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_CROUCH_A);
	}

	if(pdat->counter < 3)pdat->cnow = CELL_CA1;
	else if(pdat->counter < 9)pdat->cnow = CELL_CA3;
	else if(pdat->counter <13)pdat->cnow = CELL_CA2;
	else if(pdat->counter <17)pdat->cnow = CELL_CA1;
	else{
		pdat->aid = ACTID_CROUCH;
	}
}

void CCharacter::act_att_cb()//屈中
{
	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_CROUCH_B);
	}

	if(pdat->counter < 6)pdat->cnow = CELL_CB1;
	else if(pdat->counter < 7)pdat->cnow = CELL_CB2;
	else if(pdat->counter < 8)pdat->cnow = CELL_CB3;
	else if(pdat->counter < 9)pdat->cnow = CELL_CB4;
	else if(pdat->counter <12)pdat->cnow = CELL_CB5;
	else if(pdat->counter <21)pdat->cnow = CELL_CB6;
	else if(pdat->counter <22)pdat->cnow = CELL_CB4;
	else if(pdat->counter <23)pdat->cnow = CELL_CB3;
	else if(pdat->counter <24)pdat->cnow = CELL_CB2;
	else if(pdat->counter <30)pdat->cnow = CELL_CB1;
	else{
		pdat->aid = ACTID_CROUCH;
	}
}

void CCharacter::act_att_cc()//屈強
{
	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_CROUCH_C);
	}

	if(pdat->counter < 4)pdat->cnow = CELL_CROUCH;
	else if(pdat->counter < 6)pdat->cnow = CELL_CC1;
	else if(pdat->counter < 8)pdat->cnow = CELL_CC2;
	else if(pdat->counter <10)pdat->cnow = CELL_CC3;
	else if(pdat->counter <12)pdat->cnow = CELL_CC4;
	else if(pdat->counter <14)pdat->cnow = CELL_CC5;
	else if(pdat->counter <26)pdat->cnow = CELL_CC6;
	else{
		pdat->aid = ACTID_CROUCH;
	}
}

void CCharacter::act_att_ja()//飛弱
{
	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_JAMP_A);
	}

	if(pdat->counter < 4)pdat->cnow = CELL_JA4;
	else if(pdat->counter < 8)pdat->cnow = CELL_JA1;
	else if(pdat->counter <12)pdat->cnow = CELL_JA2;
	else if(pdat->counter <18)pdat->cnow = CELL_JA3;
	else if(pdat->counter <36)pdat->cnow = CELL_JA4;
	else{
		pdat->aid = ACTID_RAKKA2;
	}

	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy += pdat->counter%2;
	if(pdat->y > 0){
		pdat->aid = ACTID_NEUTRAL;
		pdat->y =0;
		Furimuki();
	}
}

void CCharacter::act_att_jb()//飛中
{
	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_JAMP_B);
	}

	if(pdat->counter < 5)pdat->cnow = CELL_JB1;
	else if(pdat->counter < 30)pdat->cnow = CELL_JB2;
	else{
		pdat->aid = ACTID_RAKKA2;
	}

	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy += pdat->counter%2;
	if(pdat->y > 0){
		pdat->aid = ACTID_NEUTRAL;
		pdat->y =0;
		Furimuki();
	}
}

void CCharacter::act_att_jc()//飛強
{
	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_JAMP_C);
	}

	if(pdat->counter < 8)pdat->cnow = CELL_JC1;
	else pdat->cnow = CELL_JC2;
	if (pdat->counter >15)
		GetGObject()->kougeki = FALSE;

	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy+=pdat->counter%2;
	if(pdat->y > 0){
		pdat->aid = ACTID_NEUTRAL;
		pdat->y =0;
	}
}

//支援攻撃=========================================================

void CCharacter::act_striker1()//飛んでくるとコ
{
	pdat->kasanari=FALSE;

	if(pdat->counter==0){
		pdat->objtype |= GOBJTYPE_DISPLAY;//画面内にいる
	}

	GOBJECT *pedat;
	if(pdat->counter==0){
		if(pdat->tid==TEAM_PLAYER1)
			pedat = GetActiveCharacter(TEAM_PLAYER2);
		else
			pedat = GetActiveCharacter(TEAM_PLAYER1);
		if(pedat!=NULL){
			if(pedat->x>0){
				pdat->x = pedat->x -(500);
				pdat->muki=FALSE;
				pdat->vx=14;
			}
			else{
				pdat->x = pedat->x +(500);
				pdat->muki=TRUE;
				pdat->vx=14;
			}
		}
		else {
			pdat->x=0;
			pdat->vx=0;
		}
		pdat->y = -400;
		pdat->vy= 4;
	}
	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy++;

	pdat->cnow = CELL_JAMP1;
	if(pdat->y > 0){
		pdat->y=0;
		pdat->aid = ACTID_STRIKER2;
	}
}

void CCharacter::act_striker2()//攻撃(1)
{
	DrawFront();

	if(pdat->counter < 6)pdat->cnow = CELL_SC1;
	else if(pdat->counter < 30){pdat->cnow = CELL_SC2 + pdat->counter%4;}
	else if(pdat->counter < 40)pdat->cnow = CELL_SA1;
	else{
		pdat->aid = ACTID_STRIKER4;
	}

	if(pdat->counter){
		StartAttack(ATTACKINFO_STAND_C);
	}
}

void CCharacter::act_striker3()//攻撃(2)
{
}

void CCharacter::act_striker4()
{
	pdat->kasanari=FALSE;

	if(pdat->counter==0){
		pdat->objtype &= GOBJTYPE_NODISPLAY;//画面外へいけるようにしておく
		pdat->objtype |= GOBJFLG_TAIKI;//待機フラグを立てる
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
		pdat->cnow = CELL_JAMP1;
		
		if(pdat->counter > 60){
			pdat->aid = ACTID_TAIKICYU;
			pdat->x = 2000;
			pdat->y = -2000;
			pdat->objtype |= GOBJFLG_TOBETARGET;//ターゲットになる
		}
	}
}

//投げ======================================================================

void CCharacter::act_nage()
{
	DrawFront();

	if(pdat->counter==0)PlayMySound(wave_nage);
	if(pdat->counter < 10+10)pdat->cnow=CELL_NAGE1;
	else if(pdat->counter < 15+10){pdat->cnow=CELL_NAGE2;}
	else if(pdat->counter < 20+10){pdat->cnow=CELL_NAGE3;}
	else if(pdat->counter < 30+10){pdat->cnow=CELL_NAGE4;}
	else if(pdat->counter < 40+10){pdat->cnow=CELL_NAGE5;}
	else pdat->aid = ACTID_NEUTRAL;

	if(pdat->counter == 0){
		AddEffect(EFCTID_MARKC,(int)pdat->x + zurex(150),-50,0);
		PlaySysSound(SYSTEMSOUND_CATCH);
	}
}

DWORD CCharacter::act_nagee(GOBJECT *pedat)
{
	if(pedat->counter < 10+10){
		pedat->cnow=DCELL_DAMAGES1;
		pedat->x=pdat->x+zurex(150);
		pedat->y=0;
		pedat->muki = !pdat->muki;
		return(0);
	}
	else if(pedat->counter < 15+10){
		pedat->cnow=DCELL_NAGERARE1;pedat->rot=0;
		pedat->x=pdat->x+zurex(120);
		pedat->y=-80;
		return(0);
	}
	else if(pedat->counter < 20+10){
		pedat->cnow=DCELL_NAGERARE1;pedat->rot=45;
		pedat->x=pdat->x+zurex(90);
		pedat->y=-130;
		return(0);
	}
	else if(pedat->counter < 30+10){
		pedat->cnow=DCELL_NAGERARE1;pedat->rot=90;
		pedat->x=pdat->x+zurex(50);
		pedat->y=-170;
		return(0);
	}
	else{
		pedat->x=pdat->x+zurex(170);
		StartAttack(ATTACKINFO_NAGE);
		AddDamage(pedat->id,(int)pedat->x,0);
		pedat->muki = !(pdat->muki);
		pedat->y=0;
		return(ACTID_FUTTOBI);
	}
}