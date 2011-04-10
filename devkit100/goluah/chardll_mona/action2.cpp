

#include <windows.h>
#include <stdio.h>

//#define LPDIRECT3DTEXTURE8 LPVOID
#include <d3d8.h>
#include <d3dx8.h>

#include "define_gcd.h"
#include "gobject.h"
#include "gobjbase.h"

#include "character.h"
#include "cell.h"

extern char *g_chardir;
extern DI_FUNCTIONS_S *funcs;
extern DI_FUNCTIONS_O *funco;
extern DI_FUNCTIONS_D *funcd;

//*********************************************************
//　　追加動作
//*********************************************************

void CCharacter::act_dash()
{
	pdat->cnow = CELL_DASH1+(pdat->counter/2)%2;
	movex(10);
}

void CCharacter::act_dashb()
{
	pdat->revx=TRUE;
	pdat->cnow = CELL_DASH1+(pdat->counter/2)%2;
	movex(-10);
}

void CCharacter::act_att_smc()
{
	if(pdat->counter==0)PlayMySound(6);

	if(pdat->counter < 3){pdat->cnow = CELL_SMC1;movex(2);}
	else if(pdat->counter < 6){pdat->cnow = CELL_SMC2;movex(2);}
	else if(pdat->counter < 9){pdat->cnow = CELL_SMC3;movex(2);}
	else if(pdat->counter < 12){pdat->cnow = CELL_SMC2;movex(2);}
	else if(pdat->counter < 15){pdat->cnow = CELL_SMC1;movex(2);}
	else if(pdat->counter < 35)pdat->cnow = CELL_SMCA;
	else{
		pdat->aid = ACTID_NEUTRAL;
		
	}

	if(pdat->counter == 1)StartAttack(ATTACKINFO_STAND_MC);
}

/*------------------------------------------------------------------
					竜巻旋風客・もどき
--------------------------------------------------------------------*/

void CCharacter::act_tatumaki1()
{
	if(pdat->counter == 0){
		PlayMySound(9);
		pdat->vy = -16;
		pdat->vx = 2;
		pdat->muteki=TRUE;
		pdat->counter=3;
	}
	pdat->y += pdat->vy;
	pdat->vy++;
	movex(pdat->vx);

	if(pdat->vy > 0)pdat->aid=ACTID_TATUMAKIF;
	if(pdat->counter<8)pdat->muteki=FALSE;

	pdat->cnow = CELL_TATUMAKI2 + (pdat->counter%4);
	if(pdat->counter < 2)pdat->cnow = CELL_TATUMAKI1;

	if(pdat->cnow==CELL_TATUMAKI5)pdat->revx=TRUE;
	else pdat->revx=FALSE;

	if(pdat->cnow == CELL_TATUMAKI3)StartAttack(ATTACKINFO_TATUMAKI);
}

void CCharacter::act_tatumaki2()
{
	if(pdat->counter == 0){
		PlayMySound(9);
		pdat->vy = -20;
		pdat->vx = 4;
		pdat->muteki=TRUE;
		pdat->counter=3;
	}
	pdat->y += pdat->vy;
	pdat->vy++;
	movex(pdat->vx);

	if(pdat->vy > 0)pdat->aid=ACTID_TATUMAKIF;
	if(pdat->counter<8)pdat->muteki=FALSE;

	pdat->cnow = CELL_TATUMAKI2 + (pdat->counter%4);
	if(pdat->counter < 2)pdat->cnow = CELL_TATUMAKI1;

	if(pdat->cnow==CELL_TATUMAKI5)pdat->revx=TRUE;
	else pdat->revx=FALSE;

	if(pdat->cnow == CELL_TATUMAKI3)StartAttack(ATTACKINFO_TATUMAKI);
}

void CCharacter::act_tatumaki3()
{
	if(pdat->counter == 0){
		PlayMySound(9);
		pdat->vy = -24;
		pdat->vx = 6;
		pdat->muteki=TRUE;
		pdat->counter=3;
	}
	pdat->y += pdat->vy;
	pdat->vy++;
	movex(pdat->vx);

	if(pdat->vy > 0)pdat->aid=ACTID_TATUMAKIF;
	if(pdat->counter<8)pdat->muteki=FALSE;

	pdat->cnow = CELL_TATUMAKI2 + (pdat->counter%4);
	if(pdat->counter < 2)pdat->cnow = CELL_TATUMAKI1;

	if(pdat->cnow==CELL_TATUMAKI5)pdat->revx=TRUE;
	else pdat->revx=FALSE;

	if(pdat->cnow == CELL_TATUMAKI3)StartAttack(ATTACKINFO_TATUMAKI);
}

void CCharacter::act_tatumakif()
{
	pdat->y += pdat->vy;
	pdat->vy += pdat->counter%2;
	if(pdat->y >= 0){
		pdat->aid=ACTID_NEUTRAL;
		pdat->y=0;
		Furimuki();
	}

	pdat->cnow = CELL_TATUMAKIF;
}

//ガードキャンセル竜巻旋風脚
void CCharacter::act_gcancel1()
{
	AddPowerGauge(-0.3);
	pdat->cnow = CELL_GCANCEL;
	pdat->aid = ACTID_TATUMAKI1;
}

void CCharacter::act_gcancel2()
{
	AddPowerGauge(-0.3);
	pdat->cnow = CELL_GCANCEL;
	pdat->aid = ACTID_TATUMAKI2;
}

void CCharacter::act_gcancel3()
{
	AddPowerGauge(-0.3);
	pdat->cnow = CELL_GCANCEL;
	pdat->aid = ACTID_TATUMAKI3;
}

//「レ」「イ」「ク」
void CCharacter::act_reiku1a()
{
	if(pdat->counter<3)pdat->cnow=CELL_RE2;
	else if(pdat->counter<20){pdat->cnow=CELL_RE1;movex(10);}
	else if(pdat->counter<25)pdat->cnow=CELL_WALK1;
	else pdat->aid=ACTID_NEUTRAL;

	if(pdat->counter == 0)StartAttack(ATTACKINFO_REIKU);
}

void CCharacter::act_reiku2a()
{
	if(pdat->counter<3)pdat->cnow=CELL_RE2;
	else if(pdat->counter<20){pdat->cnow=CELL_RE1;movex(10);}
	else if(pdat->counter<25)pdat->cnow=CELL_WALK1;
	else pdat->aid=ACTID_NEUTRAL;

	if(pdat->counter == 0)StartAttack(ATTACKINFO_REIKU);
}

void CCharacter::act_reiku3a()
{
	if(pdat->counter<3)pdat->cnow=CELL_RE2;
	else if(pdat->counter<20){pdat->cnow=CELL_RE1;movex(10);}
	else if(pdat->counter<25)pdat->cnow=CELL_WALK1;
	else pdat->aid=ACTID_NEUTRAL;

	if(pdat->counter == 0)StartAttack(ATTACKINFO_REIKU_FINISH);
}

void CCharacter::act_reiku1b()
{
	if(pdat->counter<3)pdat->cnow=CELL_I2;
	else if(pdat->counter<20){pdat->cnow=CELL_I1;movex(10);}
	else if(pdat->counter<25)pdat->cnow=CELL_WALK1;
	else pdat->aid=ACTID_NEUTRAL;

	if(pdat->counter == 0)StartAttack(ATTACKINFO_REIKU);
}

void CCharacter::act_reiku2b()
{
	if(pdat->counter<3)pdat->cnow=CELL_I2;
	else if(pdat->counter<20){pdat->cnow=CELL_I1;movex(10);}
	else if(pdat->counter<25)pdat->cnow=CELL_WALK1;
	else pdat->aid=ACTID_NEUTRAL;

	if(pdat->counter == 0)StartAttack(ATTACKINFO_REIKU);
}

void CCharacter::act_reiku3b()
{
	if(pdat->counter<3)pdat->cnow=CELL_I2;
	else if(pdat->counter<20){pdat->cnow=CELL_I1;movex(10);}
	else if(pdat->counter<25)pdat->cnow=CELL_WALK1;
	else pdat->aid=ACTID_NEUTRAL;

	if(pdat->counter == 0)StartAttack(ATTACKINFO_REIKU_FINISH);
}

void CCharacter::act_reiku1c()
{
	if(pdat->counter<3)pdat->cnow=CELL_KU2;
	else if(pdat->counter<20){pdat->cnow=CELL_KU1;movex(10);}
	else if(pdat->counter<25)pdat->cnow=CELL_WALK1;
	else pdat->aid=ACTID_NEUTRAL;

	if(pdat->counter == 0)StartAttack(ATTACKINFO_REIKU);
}

void CCharacter::act_reiku2c()
{
	if(pdat->counter<3)pdat->cnow=CELL_KU2;
	else if(pdat->counter<20){pdat->cnow=CELL_KU1;movex(10);}
	else if(pdat->counter<25)pdat->cnow=CELL_WALK1;
	else pdat->aid=ACTID_NEUTRAL;

	if(pdat->counter == 0)StartAttack(ATTACKINFO_REIKU);
}

void CCharacter::act_reiku3c()
{
	if(pdat->counter<3)pdat->cnow=CELL_KU2;
	else if(pdat->counter<20){pdat->cnow=CELL_KU1;movex(10);}
	else if(pdat->counter<25)pdat->cnow=CELL_WALK1;
	else pdat->aid=ACTID_NEUTRAL;

	if(pdat->counter == 0){
		pdat->vy=-15;
		StartAttack(ATTACKINFO_REIKU_FINISH);
	}

	pdat->y += pdat->vy;
	pdat->vy += 1+pdat->counter%2;
	if(pdat->y > 0){
		pdat->aid = ACTID_NEUTRAL;
		pdat->y =0;
	}
}

void CCharacter::act_reikucom()
{
	if(pdat->counter<13){
		if(pdat->counter<3)pdat->cnow=CELL_RE2;
		else if(pdat->counter<20){pdat->cnow=CELL_RE1;movex(10);}
		else if(pdat->counter<25)pdat->cnow=CELL_WALK1;
		else pdat->aid=ACTID_NEUTRAL;
	
		if(pdat->counter == 0)StartAttack(ATTACKINFO_REIKU);
	}
	else if(pdat->counter < 13+13){//「イ」
		if(pdat->counter<3+13)pdat->cnow=CELL_I2;
		else if(pdat->counter<20+13){pdat->cnow=CELL_I1;movex(10);}
		else if(pdat->counter<25+13)pdat->cnow=CELL_WALK1;
		else pdat->aid=ACTID_NEUTRAL;

		if(pdat->counter == 0+13)StartAttack(ATTACKINFO_REIKU);
	}
	else{//「ク」
		if(pdat->counter<3+13+13)pdat->cnow=CELL_KU2;
		else if(pdat->counter<20+13+13){pdat->cnow=CELL_KU1;movex(10);}
		else if(pdat->counter<25+13+13)pdat->cnow=CELL_WALK1;
		else pdat->aid=ACTID_NEUTRAL;

		if(pdat->counter == 0+13+13)StartAttack(ATTACKINFO_REIKU_FINISH);
	}
}