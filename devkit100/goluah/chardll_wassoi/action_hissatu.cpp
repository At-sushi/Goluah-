
/*==============================================================

	行動関数

	Goluah!! Copyright (C) 2001-2004 aki, 2014-2015 logger, 2004-2015 At-sushi

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
	
================================================================*/
#include "character.h"



//********************************************************************
//　必殺
//********************************************************************

//ﾜｼｮｰｲ

void CCharacter::act_wassoi1()
{
	if (pdat->counter < 10){
		pdat->cnow = CELL_CROUCH;
		return;
	}
	if (pdat->counter == 10){
		PlayMySound(wave_dive);
		pdat->vx=2;
		pdat->vy=-15;
	}

	pdat->cnow = CELL_WASSYO1;

	movex(pdat->vx);
	pdat->vy+=1;
	pdat->y+=pdat->vy;
	if(pdat->vy > 0)pdat->aid=ACTID_WASSYORYUKENF;

	if(pdat->y>0){
		pdat->y=0;
		pdat->aid=ACTID_NEUTRAL;
	}

	if (pdat->counter == 10){
		StartAttack(ATTACKINFO_DIVE1);
	}
}

void CCharacter::act_wassoi2()
{
	if (pdat->counter < 10){
		pdat->cnow = CELL_CROUCH;
		return;
	}
	if (pdat->counter == 10){
		PlayMySound(wave_dive);
		pdat->vx=3;
		pdat->vy=-18;
	}

	pdat->cnow = CELL_WASSYO1;

	movex(pdat->vx);
	pdat->vy+=1;
	pdat->y+=pdat->vy;
	if(pdat->vy > 0)pdat->aid=ACTID_WASSYORYUKENF;

	if(pdat->y>0){
		pdat->y=0;
		pdat->aid=ACTID_NEUTRAL;
	}

	if (pdat->counter == 10){
		StartAttack(ATTACKINFO_DIVE2);
	}
}

void CCharacter::act_wassoi3()
{
	if (pdat->counter < 10){
		pdat->cnow = CELL_CROUCH;
		return;
	}
	if (pdat->counter == 10){
		PlayMySound(wave_dive);
		pdat->vx=6;
		pdat->vy=-22;
	}

	pdat->cnow = CELL_WASSYO1;

	movex(pdat->vx);
	pdat->vy+=1;
	pdat->y+=pdat->vy;
	if(pdat->vy > 0)pdat->aid=ACTID_WASSYORYUKENF;

	if(pdat->y>0){
		pdat->y=0;
		pdat->aid=ACTID_NEUTRAL;
	}

	if (pdat->counter == 10){
		StartAttack(ATTACKINFO_DIVE3);
	}
}

void CCharacter::act_wassoif()
{
	pdat->cnow = CELL_WASSYO2;

	movex(pdat->vx);
	pdat->vy+=1;
	pdat->y+=pdat->vy;

	if(pdat->y>0){
		pdat->y=0;
		pdat->aid=ACTID_NEUTRAL;
	}
}

void CCharacter::act_gcancel1()
{
	AddPowerGauge(-1.0f*NEEDGAUGE_GC);
	AddEffect(EFCTID_GCANCEL,(int)pdat->x,(int)pdat->y,pdat->muki);
	pdat->cnow = CELL_GCANCEL;
	pdat->aid = ACTID_WASSYORYUKEN1;
}

void CCharacter::act_gcancel2()
{
	AddPowerGauge(-1.0f*NEEDGAUGE_GC);
	AddEffect(EFCTID_GCANCEL,(int)pdat->x,(int)pdat->y,pdat->muki);
	pdat->cnow = CELL_GCANCEL;
	pdat->aid = ACTID_WASSYORYUKEN2;
}

void CCharacter::act_gcancel3()
{
	AddPowerGauge(-1.0f*NEEDGAUGE_GC);
	AddEffect(EFCTID_GCANCEL,(int)pdat->x,(int)pdat->y,pdat->muki);
	pdat->cnow = CELL_GCANCEL;
	pdat->aid = ACTID_WASSYORYUKEN3;
}


//*****************************************************************************
//　海苔手裏剣
//*****************************************************************************

void CCharacter::act_hadou1()
{
	if(pdat->counter<4)pdat->cnow=CELL_CUTTER1;
	else if(pdat->counter<8)pdat->cnow=CELL_CUTTER2;
	else if(pdat->counter<52)pdat->cnow=CELL_CUTTER3;
	else if(pdat->counter<56)pdat->cnow=CELL_CUTTER4;
	else if(pdat->counter<60)pdat->cnow=CELL_CUTTER5;
	else if(pdat->counter<64)pdat->cnow=CELL_CUTTER6;
	else pdat->aid=ACTID_NEUTRAL;

	int m=1;
	if(pdat->muki)m=-1;

	if(pdat->counter==0)PlayMySound(wave_nori);

	if(pdat->counter == 16){
		if(b_nori)
			b_nori->Go(pdat->muki,pdat->x + zurex(75),-50);
	}
}

void CCharacter::act_hadou2()
{
	if(pdat->counter<4)pdat->cnow=CELL_CUTTER1;
	else if(pdat->counter<8)pdat->cnow=CELL_CUTTER2;
	else if (pdat->counter<52)pdat->cnow = CELL_CUTTER3;
	else if (pdat->counter<56)pdat->cnow = CELL_CUTTER4;
	else if (pdat->counter<60)pdat->cnow = CELL_CUTTER5;
	else if (pdat->counter<64)pdat->cnow = CELL_CUTTER6;
	else pdat->aid=ACTID_NEUTRAL;

	int m=1;
	if(pdat->muki)m=-1;

	if(pdat->counter==0)PlayMySound(wave_nori);

	if(pdat->counter == 16){
		if(b_nori)
			b_nori->Go(pdat->muki,pdat->x + zurex(75),-50);
	}
}

void CCharacter::act_hadou3()
{
	if(pdat->counter<4)pdat->cnow=CELL_CUTTER1;
	else if(pdat->counter<8)pdat->cnow=CELL_CUTTER2;
	else if (pdat->counter<52)pdat->cnow = CELL_CUTTER3;
	else if (pdat->counter<56)pdat->cnow = CELL_CUTTER4;
	else if (pdat->counter<60)pdat->cnow = CELL_CUTTER5;
	else if (pdat->counter<64)pdat->cnow = CELL_CUTTER6;
	else pdat->aid=ACTID_NEUTRAL;

	int m=1;
	if(pdat->muki)m=-1;

	if(pdat->counter==0)PlayMySound(wave_nori);
	if(pdat->counter == 16){
		if(b_nori)
			b_nori->Go(pdat->muki,pdat->x + zurex(75),-50);
	}
}


//*****************************************************************************
//　しゃら～
//*****************************************************************************

void CCharacter::act_syara1()
{
	if(pdat->counter == 0){
		PlayMySound(wave_syara);
		StartAttack(ATTACKINFO_SYARA1);
		pdat->vy = -6;
	}

	movex(7);
	pdat-> y += pdat->vy;
	pdat->vy += pdat->counter%2;
	if(pdat->y>0){
		pdat->y=0;
		pdat->aid = ACTID_SYARAF;
	}

	if(pdat->counter < 3)pdat->cnow = CELL_SYARA1;
	else if(pdat->counter < 6)pdat->cnow = CELL_SYARA2;
	else if(pdat->counter < 9)pdat->cnow = CELL_SYARA3;
	else pdat->cnow = CELL_SYARA4;
}

void CCharacter::act_syara2()
{
	if(pdat->counter == 0){
		PlayMySound(wave_syara);
		StartAttack(ATTACKINFO_SYARA2);
		pdat->vy = -6;
	}

	movex(9);
	pdat-> y += pdat->vy;
	pdat->vy += pdat->counter%2;
	if(pdat->y>0){
		pdat->y=0;
		pdat->aid = ACTID_SYARAF;
	}

	if(pdat->counter < 4)pdat->cnow = CELL_SYARA1;
	else if(pdat->counter < 8)pdat->cnow = CELL_SYARA2;
	else if(pdat->counter <12)pdat->cnow = CELL_SYARA3;
	else pdat->cnow = CELL_SYARA4;
}

void CCharacter::act_syara3()
{
	if(pdat->counter == 0){
		PlayMySound(wave_syara);
		StartAttack(ATTACKINFO_SYARA3);
		pdat->vy = -6;
	}

	movex(12);
	pdat-> y += pdat->vy;
	pdat->vy += pdat->counter%2;
	if(pdat->y>0){
		pdat->y=0;
		pdat->aid = ACTID_SYARAF;
	}

	if(pdat->counter < 5)pdat->cnow = CELL_SYARA1;
	else if(pdat->counter < 10)pdat->cnow = CELL_SYARA2;
	else if(pdat->counter < 15)pdat->cnow = CELL_SYARA3;
	else pdat->cnow = CELL_SYARA4;
}

void CCharacter::act_syaraf()
{
	if(pdat->counter < 10)pdat->cnow = CELL_SYARA1;
	else pdat->aid = ACTID_NEUTRAL;
}

//*****************************************************************************
//おにぎりフェスティバル！！
//*****************************************************************************

void CCharacter::act_festival()//海苔を投げる
{
	if(pdat->counter==0){
		PlayMySound(11);
		#ifdef _DEBUG
		if( pdat->gauge<1.0f ){
			dprint("%s gauge ck error(%f)",__FUNCTION__,pdat->gauge);
			throw;
		}
		#endif
		AddPowerGauge(-1.0);
		AddEffect(EFCTID_CYOHI,(int)pdat->x+zurex(20),(int)pdat->y-100,30);
	}

	if(pdat->counter<5)pdat->cnow=CELL_FESTIVAL1;
	else if(pdat->counter< 8)pdat->cnow=CELL_FESTIVAL2;
	else if(pdat->counter<60)pdat->cnow=CELL_FESTIVAL3;
	else if(pdat->counter<70)pdat->cnow=CELL_CUTTER4;
	else if(pdat->counter<80)pdat->cnow=CELL_CUTTER5;
	else if(pdat->counter<90)pdat->cnow=CELL_CUTTER6;
	else pdat->aid=ACTID_NEUTRAL;

	int m=1;
	if(pdat->muki)m=-1;

	if(pdat->counter == 12){
		if(b_nori2)
			b_nori2->Go(pdat->muki,pdat->x,-50);
	}
}

void CCharacter::act_festivaldash()
{
	if(pdat->counter == 2){
		PlayMySound(wave_fdash);
		StartAttack(ATTACKINFO_FESTA_D);
	}

	movex(30);

	if(pdat->counter < 40)pdat->cnow = CELL_FESTIVALDASH;
	else pdat->aid = ACTID_FESTIVALSUKI;
}

void CCharacter::act_festivalsuki()
{
	movex(3);

	if(pdat->counter < 30)pdat->cnow = CELL_SYARA1;
	else pdat->aid = ACTID_NEUTRAL;
}

void CCharacter::act_festivalrush()
{
	movex(2);
	
	
	if(pdat->counter==0)PlayMySound(wave_frush);

	if(pdat->counter%2==0){
		StartAttack(ATTACKINFO_FESTA);
	}

	pdat->cnow = CELL_RUSH01 + pdat->counter/2;

	if(pdat->counter > 26*2)pdat->aid = ACTID_FESTIVALFINISH;
}

void CCharacter::act_festivalfinish()
{
	if(pdat->counter==0){
		PlayMySound(wave_ffinish);
		movex(50);
		pdat->vx=6;
		pdat->vy=-22;
	}

	pdat->cnow = CELL_WASSYO1;

	movex(pdat->vx);
	pdat->vy+=1;
	pdat->y+=pdat->vy;
	if(pdat->vy > 0)pdat->aid=ACTID_WASSYORYUKENF;

	if(pdat->y>0){
		pdat->y=0;
		pdat->aid=ACTID_NEUTRAL;
	}

	if(pdat->counter == 0){
		StartAttack(ATTACKINFO_FESTA_F);
	}
}
