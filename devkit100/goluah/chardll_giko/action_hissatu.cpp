/*==============================================================

	行動関数・必殺技

================================================================*/
#include "character.h"
#include <math.h>

/*--------------------------------------------------------------
	ageタックル
----------------------------------------------------------------*/
void CCharacter::act_agetackle1(DWORD atkid)
{
	if(ccounter==0)PlayMySound(wave_age_ta);
	if(pdat->counter == 0)
	{
		pdat->vy = -16;
		pdat->vx = 2;
		pdat->muteki=TRUE;
		pdat->counter=3;
		movex(30);
	}

	pdat->muteki = (pdat->counter>8) ? FALSE : TRUE;
	
	if(pdat->vy<0)//上昇中
	{
		pdat->cnow = CELL_AGE1 + (pdat->counter%8);
		if(pdat->counter%2==0)StartAttack( atkid );
	}
	else//下降中
	{
		cell( CELL_JAMP1 );
	}
	JiyuuRakka(1,TRUE,ACTID_AGETACKLESUKI);
}

void CCharacter::act_agetackle2(DWORD atkid)
{
	if(ccounter==0)PlayMySound(wave_age_ta);
	if(pdat->counter == 0)
	{
		pdat->vy = -19;
		pdat->vx = 3;
		pdat->muteki=TRUE;
		pdat->counter=3;
		movex(38);
	}

	pdat->muteki = (pdat->counter>8) ? FALSE : TRUE;
	
	if(pdat->vy<0)//上昇中
	{
		pdat->cnow = CELL_AGE1 + (pdat->counter%8);
		if(pdat->counter%2==0)StartAttack(atkid);
	}
	else//下降中
	{
		cell( CELL_JAMP1 );
	}
	JiyuuRakka(1,TRUE,ACTID_AGETACKLESUKI);
}

void CCharacter::act_agetackle3(DWORD atkid)
{
	if(ccounter==0)PlayMySound(wave_age_ta);
	if(pdat->counter == 0)
	{
		pdat->vy = -21;
		pdat->vx = 4;
		pdat->muteki=TRUE;
		pdat->counter=3;
		movex(45);
	}

	pdat->muteki = (pdat->counter>8) ? FALSE : TRUE;
	
	if(pdat->vy<0)//上昇中
	{
		pdat->cnow = CELL_AGE1 + (pdat->counter%8);
		if(pdat->counter%2==0)StartAttack(atkid);
	}
	else//下降中
	{
		cell( CELL_JAMP1 );
	}
	JiyuuRakka(1,TRUE,ACTID_AGETACKLESUKI);
}

void CCharacter::act_agetacklesuki()
{
	cell(CELL_FUNBARI);
	if(ccounter>20)ChangeAction(ACTID_NEUTRAL);
}


/*--------------------------------------------------------------
	当て身投げ
----------------------------------------------------------------*/
//上段
void CCharacter::act_atemi1()
{
	if(ccounter==0)PlayMySound(wave_atemi);
	cell( CELL_ATEMI_A );
	if(ccounter>30)ChangeAction(ACTID_ATEMI_SUKI);
}

//下段
void CCharacter::act_atemi2()
{
	if(ccounter==0)PlayMySound(wave_atemi);
	cell( CELL_ATEMI_B );
	if(ccounter>30)ChangeAction(ACTID_ATEMI_SUKI);
}

//フェイント
void CCharacter::act_atemi3()
{
	if(ccounter==0)PlayMySound(wave_atemi);
	cell( CELL_ATEMI_A );
	if(ccounter>15)ChangeAction(ACTID_NEUTRAL);
}

//隙
void CCharacter::act_atemi_suki()
{
	if(ccounter<5)cell( CELL_ATEMI1 );
	else if(ccounter<15)cell( CELL_ATEMI2 );
	else if(ccounter<20)cell( CELL_ATEMI1 );
	else ChangeAction( ACTID_NEUTRAL );
}

//投げ動作
void CCharacter::act_atemi_nage()
{
	if(ccounter==0)PlayMySound(wave_atemi_nage);
	if(ccounter==0)return;
	if(ccounter==1)pdat->muki = !pdat->muki;

	DrawFront();

	if(ccounter<4)cell( CELL_ATEMI_NAGE1 );
	else if(ccounter< 8)cell( CELL_ATEMI_NAGE2 );
	else if(ccounter<12)cell( CELL_ATEMI_NAGE3 );
	else if(ccounter<20)cell( CELL_ATEMI_NAGE4 );
	else ChangeAction(ACTID_NEUTRAL);
}

//敵側動作
DWORD CCharacter::eact_atemi_nage(GOBJECT *pedat)
{
	if(pedat->counter<2)return 0;
	if(pedat->counter==2)pedat->muki = pdat->muki;

	double rot;

	if(pedat->counter<20)
	{
		if(pedat->counter<4)		rot=45;
		else if(pedat->counter< 8)	rot=90;
		else if(pedat->counter<12)	rot=135;
		else if(pedat->counter<20)	rot=180;

		pedat->cnow = DCELL_NAGERARE1;

		pedat->x=pdat->x -zurex(40)*cos( rot/180.0*3.1415 - 3.1415/4.0 );
		pedat->y=-60 - 40*sin( rot/180.0*3.1415  - 3.1415/4.0  );
		pedat->rot = (int)rot;
		return 0;
	}
/*	if(pedat->counter < 10){
		pedat->cnow = DCELL_NAGERARE1;
		pedat->x=pdat->x -zurex(20) +zurex(80)*cos( (pedat->counter-20)*12/180.0*3.1415 - 3.1415/4.0 );
		pedat->y=-60 - 80*sin( (pedat->counter-20)*12/180.0*3.1415  - 3.1415/4.0  );
		pedat->rot = (pedat->counter-20)*9;
		return(0);
	}*/
	else{
		StartAttack(ATTACKINFO_ATEMI_NAGE);
		AddDamage(pedat->id,(int)pdat->x + zurex(130),0);
		pedat->y=0;
		return(ACTID_DOWN);
	}
}


/*--------------------------------------------------------------
	超必
----------------------------------------------------------------*/
void CCharacter::act_cyohi()
{
	if(ccounter==0)PlayMySound(wave_storm1);
	if(pdat->counter==0){
		AddPowerGauge(-1.0);
		AddEffect(EFCTID_CYOHI,(int)pdat->x,(int)pdat->y-50,30);
	}

	if(ccounter<2)cell( CELL_STORM1 );
	else if(ccounter<4)cell( CELL_STORM2 );
	else if(ccounter<6)cell( CELL_STORM3 );
	else if(ccounter<40)cell( CELL_STORM4 );
	else if(ccounter<45)cell( CELL_STORM5 );
	else ChangeAction( ACTID_NEUTRAL );

	if(ccounter==6)
	{
		PlayMySound(wave_storm2);
		AddEffect(EFCTID_SINDO,4,20,0);
		AddEffect(EFCTID_GNDCIR,(int)pdat->x,(int)pdat->y,0);
		pbul_storm->Go(pdat->muki,pdat->x,pdat->y);
	}
}

DWORD CCharacter::eact_rotfuttobi(GOBJECT *pedat)
{
	if(pedat->counter==0)
	{
		pedat->vy = -30;
		pedat->muteki = TRUE;
	}

	pedat->y += pedat->vy;
	pedat->vy += 1;

	pedat->cnow = DCELL_KAITENFUTTOBI;
	pedat->rot = pedat->counter * 50;

	//地面到達
	if(pedat->y > 0){
		pedat->y = 0;
		pedat->aid = ACTID_BOUND;
	}

	return TRUE;
}


/*--------------------------------------------------------------
	遠当て
----------------------------------------------------------------*/
void CCharacter::act_toate1()
{
	if(ccounter==0)PlayMySound(wave_toate);

	if(ccounter<10)cell( CELL_TOATE1 );
	else if(ccounter<13)cell( CELL_TOATE2 );
	else if(ccounter<40)cell( CELL_TOATE3 );
	else ChangeAction( ACTID_NEUTRAL );

	if(ccounter==13)
	{
		pbul_toate->Go( pdat->muki, (int)(pdat->x+zurex(150)),-50);
		AddEffect( EFCTID_GNDCIR,(int)(pdat->x + zurex(30)),0,0 );
		AddEffect( EFCTID_SINDO , 5,5,5 );
	}
}

void CCharacter::act_toate2()
{
	if(ccounter==0)PlayMySound(wave_toate);

	if(ccounter<10)cell( CELL_TOATE1 );
	else if(ccounter<13)cell( CELL_TOATE2 );
	else if(ccounter<50)cell( CELL_TOATE3 );
	else ChangeAction( ACTID_NEUTRAL );

	if(ccounter==13)
	{
		pbul_toate->Go( pdat->muki, (int)(pdat->x+zurex(250)),-50);
		AddEffect( EFCTID_GNDCIR,(int)(pdat->x + zurex(30)),0,0 );
		AddEffect( EFCTID_SINDO , 5,5,5 );
	}
}

void CCharacter::act_toate3()
{
	if(ccounter==0)PlayMySound(wave_toate);

	if(ccounter<10)cell( CELL_TOATE1 );
	else if(ccounter<13)cell( CELL_TOATE2 );
	else if(ccounter<60)cell( CELL_TOATE3 );
	else ChangeAction( ACTID_NEUTRAL );

	if(ccounter==13)
	{
		pbul_toate->Go( pdat->muki, (int)(pdat->x+zurex(350)),-50);
		AddEffect( EFCTID_GNDCIR,(int)(pdat->x + zurex(30)),0,0 );
		AddEffect( EFCTID_SINDO , 5,5,5 );
	}
}
