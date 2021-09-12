
/*==============================================================

	行動関数（必殺技）

================================================================*/
#include "character.h"

/*------------------------------------------------------------------
					竜巻旋風客・もどき(弱)
--------------------------------------------------------------------*/

void CCharacter::act_tatumaki1(DWORD atkid)
{
	if(ccounter==0)PlayMySound(wave_tatumaki);

	if(pdat->counter == 0){
		pdat->vy = -16;
		pdat->vx = 2;
		pdat->muteki=TRUE;
		pdat->counter=3;
	}
	pdat->y += pdat->vy;
	pdat->vy++;
	movex(pdat->vx);
	// CREATE_GOBJ_FX( CShadowEffect(this) )

	if(pdat->vy > 0)pdat->aid=ACTID_TATUMAKIR;
	if(pdat->counter<8)pdat->muteki=FALSE;

	switch( (pdat->counter+2)%5 )
	{
	case 0:cell(CELL_TATU1);pdat->revx=FALSE;break;
	case 1:cell(CELL_TATU2);pdat->revx=FALSE;break;
	case 2:cell(CELL_TATU3);pdat->revx=FALSE;break;
	case 3:cell(CELL_TATU2);pdat->revx=TRUE;break;
	case 4:cell(CELL_TATU1);pdat->revx=TRUE;break;
	}

	if(pdat->cnow == CELL_TATU3)StartAttack(atkid);
}


/*------------------------------------------------------------------
					竜巻旋風客・もどき(中)
--------------------------------------------------------------------*/
void CCharacter::act_tatumaki2(DWORD atkid)
{
	if(ccounter==0)PlayMySound(wave_tatumaki);

	if(pdat->counter == 0){
		pdat->vy = -20;
		pdat->vx = 4;
		pdat->muteki=TRUE;
		pdat->counter=3;
	}
	pdat->y += pdat->vy;
	pdat->vy++;
	movex(pdat->vx);
	// CREATE_GOBJ_FX( CShadowEffect(this) )

	if(pdat->vy > 0)pdat->aid=ACTID_TATUMAKIR;
	if(pdat->counter<8)pdat->muteki=FALSE;

	switch( (pdat->counter+2)%5 )
	{
	case 0:cell(CELL_TATU1);pdat->revx=FALSE;break;
	case 1:cell(CELL_TATU2);pdat->revx=FALSE;break;
	case 2:cell(CELL_TATU3);pdat->revx=FALSE;break;
	case 3:cell(CELL_TATU2);pdat->revx=TRUE;break;
	case 4:cell(CELL_TATU1);pdat->revx=TRUE;break;
	}

	if(pdat->cnow == CELL_TATU3)StartAttack(atkid);
}


/*------------------------------------------------------------------
					竜巻旋風客・もどき(強)
--------------------------------------------------------------------*/
void CCharacter::act_tatumaki3(DWORD atkid)
{
	if(ccounter==0)PlayMySound(wave_tatumaki);

	if(pdat->counter == 0){
		pdat->vy = -24;
		pdat->vx = 6;
		pdat->muteki=TRUE;
		pdat->counter=3;
	}
	pdat->y += pdat->vy;
	pdat->vy++;
	movex(pdat->vx);
	// CREATE_GOBJ_FX( CShadowEffect(this) )

	if(pdat->vy > 0)pdat->aid=ACTID_TATUMAKIR;
	if(pdat->counter<8)pdat->muteki=FALSE;

	switch( (pdat->counter+2)%5 )
	{
	case 0:cell(CELL_TATU1);pdat->revx=FALSE;break;
	case 1:cell(CELL_TATU2);pdat->revx=FALSE;break;
	case 2:cell(CELL_TATU3);pdat->revx=FALSE;break;
	case 3:cell(CELL_TATU2);pdat->revx=TRUE;break;
	case 4:cell(CELL_TATU1);pdat->revx=TRUE;break;
	}

	if(pdat->cnow == CELL_TATU3)StartAttack(atkid);
}


/*------------------------------------------------------------------
					竜巻旋風客・もどき(落下・着地)
--------------------------------------------------------------------*/
void CCharacter::act_tatumakir()
{
	pdat->y += pdat->vy;
	pdat->vy += pdat->counter%2;
	if(pdat->y >= 0){
		pdat->aid=ACTID_LANDING;
		pdat->y=0;
		Furimuki();
	}

	pdat->cnow = CELL_TATU4;
}

void CCharacter::act_landing()
{
	if(pdat->counter<10)cell(CELL_TATU5);
	else ChangeAction(CELL_TATU5);
}

/*------------------------------------------------------------------
					ガードキャンセル竜巻旋風脚
--------------------------------------------------------------------*/
void CCharacter::act_gcancel1()
{
	if(ccounter==0){
		pdat->alphamode = GBLEND_KASAN;
		AddPowerGauge(-1.0*NEEDGAUGE_GC);
		AddEffect(EFCTID_GCANCEL,(int)pdat->x,(int)pdat->y-60,pdat->muki);
	}
	
	pdat->muteki=TRUE;
	pdat->nagerare=FALSE;

	if(ccounter > 2 ){
		pdat->aid = ACTID_GCANCEL1J;
		pdat->alphamode = 0;
	}
}
void CCharacter::act_gcancel1j()
{
	act_tatumaki1(ATTACKINFO_GCANCEL);
}

void CCharacter::act_gcancel2()
{
	if(ccounter==0){
		pdat->alphamode = GBLEND_KASAN;
		AddPowerGauge(-1.0*NEEDGAUGE_GC);
		AddEffect(EFCTID_GCANCEL,(int)pdat->x,(int)pdat->y-60,pdat->muki);
	}
	
	pdat->muteki=TRUE;
	pdat->nagerare=FALSE;

	if(ccounter > 2 ){
		pdat->aid = ACTID_GCANCEL2J;
		pdat->alphamode = 0;
	}
}
void CCharacter::act_gcancel2j()
{
	act_tatumaki2(ATTACKINFO_GCANCEL);
}

void CCharacter::act_gcancel3()
{
	if(ccounter==0){
		pdat->alphamode = GBLEND_KASAN;
		AddPowerGauge(-1.0*NEEDGAUGE_GC);
		AddEffect(EFCTID_GCANCEL,(int)pdat->x,(int)pdat->y-60,pdat->muki);
	}
	
	pdat->muteki=TRUE;
	pdat->nagerare=FALSE;

	if(ccounter > 2 ){
		pdat->aid = ACTID_GCANCEL3J;
		pdat->alphamode = 0;
	}
}
void CCharacter::act_gcancel3j()
{
	act_tatumaki3(ATTACKINFO_GCANCEL);
}


/*------------------------------------------------------------------
					「レ」「イ」「ク」(弱)
--------------------------------------------------------------------*/
void CCharacter::act_reiku1a()
{
	if(ccounter==0)PlayMySound(wave_reiku_re);

	if(pdat->counter<20){pdat->cnow=CELL_REIKU_RE1;movex(10);}
	else if(pdat->counter<28)pdat->cnow=CELL_REIKU_RE2;
	else if(pdat->counter<36)pdat->cnow=CELL_REIKU_RE3;
	else pdat->aid=ACTID_NEUTRAL;

	if(pdat->counter == 0){
		StartAttack(ATTACKINFO_REIKU);
		pefe_re->Go(pdat->muki,pdat->x,pdat->y-50,0);
	}
	if(ccounter==20){
		EndAttack();
	}

	// CREATE_GOBJ_FX( CShadowEffect(this) )
}

void CCharacter::act_reiku1b()
{
	if(ccounter==0)PlayMySound(wave_reiku_i);

	if(pdat->counter<20){pdat->cnow=CELL_REIKU_I1;movex(10);}
	else if(pdat->counter<28)pdat->cnow=CELL_REIKU_I2;
	else if(pdat->counter<36)pdat->cnow=CELL_REIKU_I3;
	else pdat->aid=ACTID_NEUTRAL;

	if(pdat->counter == 0){
		StartAttack(ATTACKINFO_REIKU);
		pefe_i->Go(pdat->muki,pdat->x,pdat->y-50,0);
	}
	if(ccounter==20){
		EndAttack();
	}

	// CREATE_GOBJ_FX( CShadowEffect(this) )
}

void CCharacter::act_reiku1c()
{
	if(ccounter==0)PlayMySound(wave_reiku_ku);

	if(pdat->counter<20){pdat->cnow=CELL_REIKU_KU1;movex(10);}
	else if(pdat->counter<28)pdat->cnow=CELL_REIKU_KU2;
	else if(pdat->counter<36)pdat->cnow=CELL_REIKU_KU3;
	else pdat->aid=ACTID_NEUTRAL;

	if(pdat->counter == 0){
		StartAttack(ATTACKINFO_REIKU_FINISH);
		pefe_ku->Go(pdat->muki,pdat->x,pdat->y-50,0);
	}
	if(ccounter==20){
		EndAttack();
	}

	// CREATE_GOBJ_FX( CShadowEffect(this) )
}


/*------------------------------------------------------------------
					「レ」「イ」「ク」(中)
--------------------------------------------------------------------*/
void CCharacter::act_reiku2a()
{
	if(ccounter==0)PlayMySound(wave_reiku_re);

	if(pdat->counter<20){pdat->cnow=CELL_REIKU_RE1;movex(10);}
	else if(pdat->counter<28)pdat->cnow=CELL_REIKU_RE2;
	else if(pdat->counter<36)pdat->cnow=CELL_REIKU_RE3;
	else pdat->aid=ACTID_NEUTRAL;

	if(pdat->counter == 0){
		StartAttack(ATTACKINFO_REIKU);
		pefe_re->Go(pdat->muki,pdat->x,pdat->y-50,0);
	}
	if(ccounter==20){
		EndAttack();
	}

	// CREATE_GOBJ_FX( CShadowEffect(this) )
}

void CCharacter::act_reiku2b()
{
	if(ccounter==0)PlayMySound(wave_reiku_i);

	if(pdat->counter<20){pdat->cnow=CELL_REIKU_I1;movex(10);}
	else if(pdat->counter<28)pdat->cnow=CELL_REIKU_I2;
	else if(pdat->counter<36)pdat->cnow=CELL_REIKU_I3;
	else pdat->aid=ACTID_NEUTRAL;

	if(pdat->counter == 0){
		StartAttack(ATTACKINFO_REIKU);
		pefe_i->Go(pdat->muki,pdat->x,pdat->y-50,0);
	}

	// CREATE_GOBJ_FX( CShadowEffect(this) )
}

void CCharacter::act_reiku2c()
{
	if(ccounter==0)PlayMySound(wave_reiku_ku);

	if(pdat->counter<20){pdat->cnow=CELL_REIKU_KU1;movex(10);}
	else if(pdat->counter<28)pdat->cnow=CELL_REIKU_KU2;
	else if(pdat->counter<36)pdat->cnow=CELL_REIKU_KU3;
	else pdat->aid=ACTID_NEUTRAL;

	if(pdat->counter == 0){
		StartAttack(ATTACKINFO_REIKU_FINISH);
		pefe_ku->Go(pdat->muki,pdat->x,pdat->y-50,0);
	}
	if(ccounter==20){
		EndAttack();
	}

	// CREATE_GOBJ_FX( CShadowEffect(this) )
}


/*------------------------------------------------------------------
					「レ」「イ」「ク」(強)
--------------------------------------------------------------------*/
void CCharacter::act_reiku3a()
{
	if(ccounter==0)PlayMySound(wave_reiku_re);

	if(pdat->counter<20){pdat->cnow=CELL_REIKU_RE1;movex(10);}
	else if(pdat->counter<28)pdat->cnow=CELL_REIKU_RE2;
	else if(pdat->counter<36)pdat->cnow=CELL_REIKU_RE3;
	else pdat->aid=ACTID_NEUTRAL;

	if(pdat->counter == 0){
		StartAttack(ATTACKINFO_REIKU);
		pefe_re->Go(pdat->muki,pdat->x,pdat->y-50,0);
	}
	if(ccounter==20){
		EndAttack();
	}

	// CREATE_GOBJ_FX( CShadowEffect(this) )
}

void CCharacter::act_reiku3b()
{
	if(ccounter==0)PlayMySound(wave_reiku_i);

	if(pdat->counter<20){pdat->cnow=CELL_REIKU_I1;movex(10);}
	else if(pdat->counter<28)pdat->cnow=CELL_REIKU_I2;
	else if(pdat->counter<36)pdat->cnow=CELL_REIKU_I3;
	else pdat->aid=ACTID_NEUTRAL;

	if(pdat->counter == 0){
		StartAttack(ATTACKINFO_REIKU);
		pefe_i->Go(pdat->muki,pdat->x,pdat->y-50,0);
	}
	if(ccounter==20){
		EndAttack();
	}

	// CREATE_GOBJ_FX( CShadowEffect(this) )
}

void CCharacter::act_reiku3c()
{
	if(ccounter==0)PlayMySound(wave_reiku_ku);

	pdat->cnow=CELL_REIKU_KU1;
	movex(10);

	if(pdat->counter == 0){
		pdat->vy=-15;
		StartAttack(ATTACKINFO_REIKU_FINISH);
		pefe_ku->Go(pdat->muki,pdat->x,pdat->y-50,0);
	}

	pdat->y += pdat->vy;
	pdat->vy += 1+pdat->counter%2;
	if(pdat->y > 0){
		pdat->aid = ACTID_REIKU3CS;
		pdat->y =0;
	}
	if(ccounter==20){
		EndAttack();
	}

	// CREATE_GOBJ_FX( CShadowEffect(this) )
}

void CCharacter::act_reiku3cs()
{
	if(pdat->counter<7)pdat->cnow=CELL_REIKU_KU2;
	else if(pdat->counter<14)pdat->cnow=CELL_REIKU_KU3;
	else pdat->aid=ACTID_NEUTRAL;
}

/*------------------------------------------------------------------
					「レ」「イ」「ク」(コンピュータ用)
--------------------------------------------------------------------* /
void CCharacter::act_reikucom()
{
	if(pdat->counter<13)//「レ」
	{
		if(ccounter==0)PlayMySound(wave_reiku_re);

		pdat->cnow=CELL_REIKU_RE1;
		movex(10);
		if(pdat->counter == 0){
			StartAttack(ATTACKINFO_REIKU);
			pefe_re->Go(pdat->muki,pdat->x,pdat->y-50,0);
		}
	}
	else if(pdat->counter < 13+13)//「イ」
	{
		if(ccounter==13)PlayMySound(wave_reiku_i);

		pdat->cnow=CELL_REIKU_I1;
		movex(10);
		if(pdat->counter == 0+13){
			StartAttack(ATTACKINFO_REIKU);
			pefe_i->Go(pdat->muki,pdat->x,pdat->y-50,0);
		}
	}
	else//「ク」
	{
		if(ccounter==13+13)PlayMySound(wave_reiku_ku);

		if(pdat->counter<20+13+13){
			pdat->cnow=CELL_REIKU_KU1;
			movex(10);
		}
		else if(pdat->counter<25+13+13)pdat->cnow=CELL_REIKU_KU2;
		else if(pdat->counter<30+13+13)pdat->cnow=CELL_REIKU_KU3;
		else pdat->aid=ACTID_NEUTRAL;

		if(pdat->counter == 0+13+13){
			StartAttack(ATTACKINFO_REIKU);
			pefe_ku->Go(pdat->muki,pdat->x,pdat->y-50,0);
		}
	}
}*/


/*------------------------------------------------------------------
	波動拳（飛ばない）
--------------------------------------------------------------------*/
//弱
void CCharacter::act_hadou1a()
{
	if(ccounter==0)PlayMySound(wave_hado1);

	UINT anmspd = 1;

	if(ccounter<anmspd*2)		cell(CELL_HADOU1);
	else if(ccounter<anmspd*3)	cell(CELL_HADOU2);
	else if(ccounter<anmspd*4)	cell(CELL_HADOU3);
	else if(ccounter<anmspd*5)	cell(CELL_HADOU4);
	else if(ccounter<anmspd*6)	cell(CELL_HADOU5);
	else if(ccounter<anmspd*8)	cell(CELL_HADOU6);
	else if(ccounter<anmspd*10+20)cell(CELL_HADOU7);
	else ChangeAction(ACTID_NEUTRAL);

	if(ccounter==anmspd*6)
	{
		pbul_hadou->GoA(
			pdat->muki,
			pdat->x+zurex(70),
			-50,
			4,
			&aif[ATTACKINFO_HADOU1A]);
	}
}

//中
void CCharacter::act_hadou2a()
{
	if(ccounter==0)PlayMySound(wave_hado1);
	UINT anmspd = 1;

	if(ccounter<anmspd*2)		cell(CELL_HADOU1);
	else if(ccounter<anmspd*3)	cell(CELL_HADOU2);
	else if(ccounter<anmspd*4)	cell(CELL_HADOU3);
	else if(ccounter<anmspd*5)	cell(CELL_HADOU4);
	else if(ccounter<anmspd*6)	cell(CELL_HADOU5);
	else if(ccounter<anmspd*8)	cell(CELL_HADOU6);
	else if(ccounter<anmspd*10+30)cell(CELL_HADOU7);
	else ChangeAction(ACTID_NEUTRAL);

	if(ccounter==anmspd*6)
	{
		pbul_hadou->GoA(
			pdat->muki,
			pdat->x+zurex(70),
			-50,
			5,
			&aif[ATTACKINFO_HADOU2A]);
	}
}

//強
void CCharacter::act_hadou3a()
{
	if(ccounter==0)PlayMySound(wave_hado1);
	UINT anmspd = 1;

	if(ccounter<anmspd*2)		cell(CELL_HADOU1);
	else if(ccounter<anmspd*3)	cell(CELL_HADOU2);
	else if(ccounter<anmspd*4)	cell(CELL_HADOU3);
	else if(ccounter<anmspd*5)	cell(CELL_HADOU4);
	else if(ccounter<anmspd*6)	cell(CELL_HADOU5);
	else if(ccounter<anmspd*8)	cell(CELL_HADOU6);
	else if(ccounter<anmspd*10+40)cell(CELL_HADOU7);
	else ChangeAction(ACTID_NEUTRAL);

	if(ccounter==anmspd*6)
	{
		pbul_hadou->GoA(
			pdat->muki,
			pdat->x+zurex(70),
			-50,
			6,
			&aif[ATTACKINFO_HADOU3A]);
	}
}

/*------------------------------------------------------------------
	波動拳（飛ぶ）
--------------------------------------------------------------------*/
//弱
void CCharacter::act_hadou1b()
{
	if(ccounter==0)PlayMySound(wave_hado2);
	UINT anmspd = 1;

	if(ccounter<anmspd*2)		cell(CELL_HADOU1);
	else if(ccounter<anmspd*3)	cell(CELL_HADOU2);
	else if(ccounter<anmspd*4)	cell(CELL_HADOU3);
	else if(ccounter<anmspd*5)	cell(CELL_HADOU4);
	else if(ccounter<anmspd*6)	cell(CELL_HADOU5);
	else if(ccounter<anmspd*8)	cell(CELL_HADOU6);
	else if(ccounter<anmspd*10+20)cell(CELL_HADOU7);
	else ChangeAction(ACTID_NEUTRAL);

	if(ccounter==anmspd*8)
	{
		pbul_hadou->GoB(
			pdat->muki,
			pdat->x+zurex(100),
			-50,
			12,
			&aif[ATTACKINFO_HADOU1B]);
	}
}

//中
void CCharacter::act_hadou2b()
{
	if(ccounter==0)PlayMySound(wave_hado2);
	UINT anmspd = 2;

	if(ccounter<anmspd*2)		cell(CELL_HADOU1);
	else if(ccounter<anmspd*3)	cell(CELL_HADOU2);
	else if(ccounter<anmspd*4)	cell(CELL_HADOU3);
	else if(ccounter<anmspd*5)	cell(CELL_HADOU4);
	else if(ccounter<anmspd*6)	cell(CELL_HADOU5);
	else if(ccounter<anmspd*8)	cell(CELL_HADOU6);
	else if(ccounter<anmspd*10+20)cell(CELL_HADOU7);
	else ChangeAction(ACTID_NEUTRAL);

	if(ccounter==anmspd*8)
	{
		pbul_hadou->GoB(
			pdat->muki,
			pdat->x+zurex(100),
			-50,
			16,
			&aif[ATTACKINFO_HADOU2B]);
	}
}

//強
void CCharacter::act_hadou3b()
{
	if(ccounter==0)PlayMySound(wave_hado2);
	UINT anmspd = 3;

	if(ccounter<anmspd*2)		cell(CELL_HADOU1);
	else if(ccounter<anmspd*3)	cell(CELL_HADOU2);
	else if(ccounter<anmspd*4)	cell(CELL_HADOU3);
	else if(ccounter<anmspd*5)	cell(CELL_HADOU4);
	else if(ccounter<anmspd*6)	cell(CELL_HADOU5);
	else if(ccounter<anmspd*8)	cell(CELL_HADOU6);
	else if(ccounter<anmspd*10+20)cell(CELL_HADOU7);
	else ChangeAction(ACTID_NEUTRAL);

	if(ccounter==anmspd*8)
	{
		pbul_hadou->GoB(
			pdat->muki,
			pdat->x+zurex(100),
			-50,
			20,
			&aif[ATTACKINFO_HADOU3B]);
	}
}


/*------------------------------------------------------------------
	グッバイ・フォーエバー
--------------------------------------------------------------------*/
//始動→前進
void CCharacter::act_goodbye1()
{
	if(ccounter==0)PlayMySound(wave_goodbye);

	if(ccounter==0){
		AddPowerGauge(-1.0);
		AddEffect(EFCTID_CYOHI,(int)pdat->x,(int)pdat->y-50,30);
	}

	if(ccounter<2)cell(CELL_GOODBYE1);
	else if(ccounter<9)
	{
		StartAttack(ATTACKINFO_GOODBYE1);
		cell(CELL_GOODBYE2);
		movex(30);
	}
	else if(ccounter<30)cell(CELL_GOODBYE3);
	else ChangeAction(ACTID_NEUTRAL);

	// CREATE_GOBJ_FX( CShadowEffect(this, 0x330000FF) )
}

//ヒット→打ち上げ
void CCharacter::act_goodbye2()
{
	if(ccounter<1)cell(CELL_GOODBYE3);
	else if(ccounter<3)cell(CELL_SC1);
	else if(ccounter<5)cell(CELL_SC2);
	else if(ccounter<7)cell(CELL_SC3);
	else if(ccounter<9)cell(CELL_SC4);
	else if(ccounter<20)cell(CELL_SC5);//*
	else if(ccounter<24)cell(CELL_SC6);
	else if(ccounter<28)cell(CELL_SC7);
//	else if(ccounter<43)cell(CELL_GOODBYE4);
//	else if(ccounter<100)cell(CELL_GOODBYE5);
	else ChangeAction(ACTID_NEUTRAL);

	if(ccounter==9){
		pbul_goodbye->GoA(
			pdat->muki,
			pdat->x+zurex(80),
			0,
			12,
			&aif[ATTACKINFO_GOODBYE2]);
		AddEffect(EFCTID_FLASH, 4, 0, 0);
	}

//	if (ccounter==28)
//		pbul_eraser99->Go(pdat->muki, pdat->x + zurex(50), -50, 0);

	// CREATE_GOBJ_FX( CShadowEffect(this, 0x00000088, GBLEND_KASAN) )
}

