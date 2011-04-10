/*==============================================================

	行動関数（必殺）

================================================================*/
#include "character.h"

//必殺投げ------------------------------------------------------------------

BOOL nageend;

void CCharacter::act_hnage()
{
	if(pdat->counter==0)PlayMySound(wave_hnage1);
	DrawFront();
	//pdat->kasanari = FALSE;

	if(pdat->counter < 10+10){
		nageend=FALSE;
		pdat->cnow=CELL_NAGE1;
	}
	else if(pdat->counter < 15+10){pdat->cnow=CELL_NAGE2;}
	else if(pdat->counter < 20+10){pdat->cnow=CELL_NAGE3;}
	else if(pdat->counter < 30+10){pdat->cnow=CELL_NAGE4;}
	else if(pdat->counter < 40+10){
		pdat->cnow=CELL_NAGE5;pdat->vx=2.5;
		pdat->vy=-15;pdat->y=-1;
	}
	else if(pdat->y < 0){
		movex(pdat->vx);
		pdat->y += pdat->vy;
		pdat->vy += 0.5;
		if(pdat->vy < 0)pdat->cnow = CELL_JAMP1;
		else pdat->cnow = CELL_FUNZUKE;
	}
	else {
		PlayMySound(wave_hnage2);
		nageend=TRUE;
		pdat->aid = ACTID_NEUTRAL;
		pdat->y = 0;
	}

	if(pdat->counter == 0){
		AddEffect(EFCTID_MARKC,(int)pdat->x + zurex(150),-50,0);
		PlaySysSound(SYSTEMSOUND_CATCH);
	}
}

DWORD CCharacter::act_hnagee(GOBJECT *pedat)
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
	else if(pedat->counter == 30+10){
		pedat->muki = !(pdat->muki);
		pedat->x=pdat->x+zurex(170);
		pedat->y=0;
		pedat->cnow=DCELL_DOWN;
		StartAttack(ATTACKINFO_HNAGE1);
		AddDamage(pedat->id,(int)pedat->x,0);
		return(0);
	}
	else if(nageend){
		pedat->muki = !(pdat->muki);
		pedat->y=0;
		pedat->cnow=DCELL_DOWN;
		StartAttack(ATTACKINFO_HNAGE2);
		AddDamage(pedat->id,(int)pedat->x,0);
		return(ACTID_FUTTOBI);
	}
	else if(pedat->counter >= 300){ // 強制発射
		pedat->muki = !(pdat->muki);
		pedat->y=0;
		nageend = TRUE;
		return(ACTID_DOWN);
	}
	else return(0);

	return(0);
}

//「体当たり」======================================================================

void CCharacter::act_tetu1()
{
	if(pdat->counter==0)PlayMySound(wave_charge);

	if(pdat->counter < 2)pdat->cnow = CELL_TETU1;
	else if(pdat->counter < 4)pdat->cnow = CELL_TETU2;
	else if(pdat->counter < 12){
		pdat->vx = 10;
		movex(pdat->vx);
		pdat->cnow = CELL_TETU3;
	}
	else if(pdat->counter < 34){
		pdat->vx -=2;
		if(pdat->vx<0){
			pdat->vx=0;
		}
		movex(pdat->vx);
		pdat->cnow = CELL_TETU3;
	}
	else if(pdat->counter < 37)pdat->cnow = CELL_TETU2;
	else if(pdat->counter < 40)pdat->cnow = CELL_TETU1;
	else{
		pdat->aid=ACTID_NEUTRAL;
	}

	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_CHARGE1);
	}
}

void CCharacter::act_tetu2()
{
	if(pdat->counter==0)PlayMySound(wave_charge);
	if(pdat->counter < 4)pdat->cnow = CELL_TETU1;
	else if(pdat->counter < 8)pdat->cnow = CELL_TETU2;
	else if(pdat->counter < 16){
		pdat->vx = 15;
		movex(pdat->vx);
		pdat->cnow = CELL_TETU3;
	}
	else if(pdat->counter < 40){
		pdat->vx -=2;
		if(pdat->vx<0){
			pdat->vx=0;
		}
		movex(pdat->vx);
		pdat->cnow = CELL_TETU3;
	}
	else if(pdat->counter < 45)pdat->cnow = CELL_TETU2;
	else if(pdat->counter < 50)pdat->cnow = CELL_TETU1;
	else{
		pdat->aid=ACTID_NEUTRAL;
	}

	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_CHARGE2);
	}
}

void CCharacter::act_tetu3()
{
	if(pdat->counter==0)PlayMySound(wave_charge);
	if(pdat->counter < 6)pdat->cnow = CELL_TETU1;
	else if(pdat->counter < 12)pdat->cnow = CELL_TETU2;
	else if(pdat->counter < 20){
		pdat->vx = 20;
		movex(pdat->vx);
		pdat->cnow = CELL_TETU3;
	}
	else if(pdat->counter < 50){
		pdat->vx -=2;
		if(pdat->vx<0){
			pdat->vx=0;
		}
		movex(pdat->vx);
		pdat->cnow = CELL_TETU3;
	}
	else if(pdat->counter < 57)pdat->cnow = CELL_TETU2;
	else if(pdat->counter < 64)pdat->cnow = CELL_TETU1;
	else{
		pdat->aid=ACTID_NEUTRAL;
	}

	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_CHARGE3);
	}
}

//超必投げ-------------------------------------------------------------------

void CCharacter::act_snage()
{
	if(pdat->counter==0){
		PlayMySound(wave_snage1);
		DrawFront();
		AddEffect(EFCTID_CYOHI,
			(int)pdat->x + zurex(30),(int)pdat->y-60,50);
		AddPowerGauge(-1.0);
		pdat->cnow=CELL_NAGE1;
		nageend=FALSE;
	}

	DWORD a_spd=7,
		b_spd=6,
		c_spd=6,
		d_spd=5,
		e_spd=4,
		f_spd=3,
		g_spd=2,
		a_end=a_spd*5,
		b_end=a_end + b_spd*5,
		c_end=b_end + c_spd*5,
		d_end=c_end + d_spd*5,
		e_end=d_end + e_spd*5,
		f_end=e_end + f_spd*5,
		g_end=f_end + g_spd*5;

		//a part ←
		if(pdat->counter < a_spd*1){pdat->revx=FALSE;pdat->cnow = CELL_SNAGE2;}
		else if(pdat->counter < a_spd*2){pdat->revx=FALSE;pdat->cnow = CELL_SNAGE1;}
		else if(pdat->counter < a_spd*3){pdat->revx=TRUE;pdat->cnow = CELL_SNAGE1;}
		else if(pdat->counter < a_spd*4){pdat->revx=TRUE;pdat->cnow = CELL_SNAGE2;}
		else if(pdat->counter < a_spd*5){pdat->revx=TRUE;pdat->cnow = CELL_SNAGE3;}
		//b part →
		else if(pdat->counter < a_end + b_spd*1){pdat->revx=TRUE;pdat->cnow = CELL_SNAGE2;}
		else if(pdat->counter < a_end + b_spd*2){pdat->revx=TRUE;pdat->cnow = CELL_SNAGE1;}
		else if(pdat->counter < a_end + b_spd*3){pdat->revx=FALSE;pdat->cnow = CELL_SNAGE1;}
		else if(pdat->counter < a_end + b_spd*4){pdat->revx=FALSE;pdat->cnow = CELL_SNAGE2;}
		else if(pdat->counter < a_end + b_spd*5){pdat->revx=FALSE;pdat->cnow = CELL_SNAGE3;}
		//c part ←
		else if(pdat->counter < b_end + c_spd*1){pdat->revx=FALSE;pdat->cnow = CELL_SNAGE2;}
		else if(pdat->counter < b_end + c_spd*2){pdat->revx=FALSE;pdat->cnow = CELL_SNAGE1;}
		else if(pdat->counter < b_end + c_spd*3){pdat->revx=TRUE;pdat->cnow = CELL_SNAGE1;}
		else if(pdat->counter < b_end + c_spd*4){pdat->revx=TRUE;pdat->cnow = CELL_SNAGE2;}
		else if(pdat->counter < b_end + c_spd*5){pdat->revx=TRUE;pdat->cnow = CELL_SNAGE3;}
		//d part →
		else if(pdat->counter < c_end + d_spd*1){pdat->revx=TRUE;pdat->cnow = CELL_SNAGE2;}
		else if(pdat->counter < c_end + d_spd*2){pdat->revx=TRUE;pdat->cnow = CELL_SNAGE1;}
		else if(pdat->counter < c_end + d_spd*3){pdat->revx=FALSE;pdat->cnow = CELL_SNAGE1;}
		else if(pdat->counter < c_end + d_spd*4){pdat->revx=FALSE;pdat->cnow = CELL_SNAGE2;}
		else if(pdat->counter < c_end + d_spd*5){pdat->revx=FALSE;pdat->cnow = CELL_SNAGE3;}
		//e part ←
		else if(pdat->counter < d_end + e_spd*1){pdat->revx=FALSE;pdat->cnow = CELL_SNAGE2;}
		else if(pdat->counter < d_end + e_spd*2){pdat->revx=FALSE;pdat->cnow = CELL_SNAGE1;}
		else if(pdat->counter < d_end + e_spd*3){pdat->revx=TRUE;pdat->cnow = CELL_SNAGE1;}
		else if(pdat->counter < d_end + e_spd*4){pdat->revx=TRUE;pdat->cnow = CELL_SNAGE2;}
		else if(pdat->counter < d_end + e_spd*5){pdat->revx=TRUE;pdat->cnow = CELL_SNAGE3;}
		//f part →
		else if(pdat->counter < e_end + f_spd*1){pdat->revx=TRUE;pdat->cnow = CELL_SNAGE2;}
		else if(pdat->counter < e_end + f_spd*2){pdat->revx=TRUE;pdat->cnow = CELL_SNAGE1;}
		else if(pdat->counter < e_end + f_spd*3){pdat->revx=FALSE;pdat->cnow = CELL_SNAGE1;}
		else if(pdat->counter < e_end + f_spd*4){pdat->revx=FALSE;pdat->cnow = CELL_SNAGE2;}
		else if(pdat->counter < e_end + f_spd*5){pdat->revx=FALSE;pdat->cnow = CELL_SNAGE3;}
		//g part ←
		else if(pdat->counter < f_end + g_spd*1){pdat->revx=FALSE;pdat->cnow = CELL_SNAGE2;}
		else if(pdat->counter < f_end + g_spd*2){pdat->revx=FALSE;pdat->cnow = CELL_SNAGE1;}
		else if(pdat->counter < f_end + g_spd*3){pdat->revx=TRUE;pdat->cnow = CELL_SNAGE1;}
		else if(pdat->counter < f_end + g_spd*4){pdat->revx=TRUE;pdat->cnow = CELL_SNAGE2;}
		else if(pdat->counter < f_end + g_spd*5){
			pdat->revx=TRUE;pdat->cnow = CELL_SNAGE3;
			pdat->y=-1;
			pdat->vy=-30;
		}

		else if(pdat->y < 0){
			movex(-4.5);
			pdat->y += pdat->vy;
			pdat->vy += 1.5;
			pdat->cnow = CELL_SNAGEKAITEN1 + pdat->counter%5;
		}
		else {
			PlayMySound(wave_snage2);
			nageend=TRUE;
			pdat->aid = ACTID_NEUTRAL;
			pdat->y = 0;
			pdat->muki = !(pdat->muki);
		}
}

DWORD CCharacter::act_snagee(GOBJECT *pedat)
{
	if(pdat->counter==0){
		pedat->cnow=DCELL_NAGERARE1;pedat->rot=0;
	}

	DWORD a_spd=7,
		b_spd=6,
		c_spd=6,
		d_spd=5,
		e_spd=4,
		f_spd=3,
		g_spd=2,
		a_end=a_spd*5,
		b_end=a_end + b_spd*5,
		c_end=b_end + c_spd*5,
		d_end=c_end + d_spd*5,
		e_end=d_end + e_spd*5,
		f_end=e_end + f_spd*5,
		g_end=f_end + g_spd*5;

	int zurex1=150,
		zurex2=100,
		zurex3=200,
		zurey1=-120,
		zurey2=-200;

	if(pdat->counter==a_end-1 || pdat->counter==c_end-1 || pdat->counter==e_end-1 || pdat->counter==g_end-1)
	{
		pedat->cnow=DCELL_DOWN;
		AddEffect(EFCTID_GNDCIR,(int)pedat->x + zurex(-40),0,0);
		StartAttack(ATTACKINFO_SNAGE1);
		AddDamage(pedat->id,(int)pedat->x + zurex(-40),0);
	}
	if(pdat->counter==b_end-1 || pdat->counter==d_end-1 || pdat->counter==f_end-1)
	{
		pedat->cnow=DCELL_DOWN;
		AddEffect(EFCTID_GNDCIR,(int)pedat->x + zurex(40),0,0);
		StartAttack(ATTACKINFO_SNAGE1);
		AddDamage(pedat->id,(int)pedat->x + zurex(40),0);
	}


		//a part ←
		if(pedat->counter < a_spd*1){pedat->revx=FALSE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=90;pedat->x=pdat->x+zurex(zurex1);pedat->y=zurey1;}
		else if(pedat->counter < a_spd*2){pedat->revx=FALSE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=135;pedat->x=pdat->x+zurex(zurex2);pedat->y=zurey2;}
		else if(pedat->counter < a_spd*3){pedat->revx=TRUE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=135;pedat->x=pdat->x+zurex(zurex2*(-1));pedat->y=zurey2;}
		else if(pedat->counter < a_spd*4){pedat->revx=TRUE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=90;pedat->x=pdat->x+zurex(zurex2*(-1));pedat->y=zurey1;}
		else if(pedat->counter < a_spd*5){pedat->revx=TRUE;pedat->cnow = DCELL_DOWN;pedat->x=pdat->x+zurex(zurex3*(-1));pedat->y=0;}
		//b part →
		else if(pedat->counter < a_end + b_spd*1){pedat->revx=TRUE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=90;pedat->x=pdat->x+zurex(zurex1*(-1));pedat->y=zurey1;}
		else if(pedat->counter < a_end + b_spd*2){pedat->revx=TRUE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=135;pedat->x=pdat->x+zurex(zurex2*(-1));pedat->y=zurey2;}
		else if(pedat->counter < a_end + b_spd*3){pedat->revx=FALSE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=135;pedat->x=pdat->x+zurex(zurex2);pedat->y=zurey2;}
		else if(pedat->counter < a_end + b_spd*4){pedat->revx=FALSE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=90;pedat->x=pdat->x+zurex(zurex1);pedat->y=zurey1;}
		else if(pedat->counter < a_end + b_spd*5){pedat->revx=FALSE;pedat->cnow = DCELL_DOWN;pedat->x=pdat->x+zurex(zurex3);pedat->y=0;}
		//c part ←
		else if(pedat->counter < b_end + c_spd*1){pedat->revx=FALSE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=90;pedat->x=pdat->x+zurex(zurex1);pedat->y=zurey1;}
		else if(pedat->counter < b_end + c_spd*2){pedat->revx=FALSE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=135;pedat->x=pdat->x+zurex(zurex2);pedat->y=zurey2;}
		else if(pedat->counter < b_end + c_spd*3){pedat->revx=TRUE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=135;pedat->x=pdat->x+zurex(zurex2*(-1));pedat->y=zurey2;}
		else if(pedat->counter < b_end + c_spd*4){pedat->revx=TRUE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=90;pedat->x=pdat->x+zurex(zurex2*(-1));pedat->y=zurey1;}
		else if(pedat->counter < b_end + c_spd*5){pedat->revx=TRUE;pedat->cnow = DCELL_DOWN;pedat->x=pdat->x+zurex(zurex3*(-1));pedat->y=0;}
		//d part →
		else if(pedat->counter < c_end + d_spd*1){pedat->revx=TRUE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=90;pedat->x=pdat->x+zurex(zurex1*(-1));pedat->y=zurey1;}
		else if(pedat->counter < c_end + d_spd*2){pedat->revx=TRUE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=135;pedat->x=pdat->x+zurex(zurex2*(-1));pedat->y=zurey2;}
		else if(pedat->counter < c_end + d_spd*3){pedat->revx=FALSE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=135;pedat->x=pdat->x+zurex(zurex2);pedat->y=zurey2;}
		else if(pedat->counter < c_end + d_spd*4){pedat->revx=FALSE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=90;pedat->x=pdat->x+zurex(zurex1);pedat->y=zurey1;}
		else if(pedat->counter < c_end + d_spd*5){pedat->revx=FALSE;pedat->cnow = DCELL_DOWN;pedat->x=pdat->x+zurex(zurex3);pedat->y=0;}
		//e part ←
		else if(pedat->counter < d_end + e_spd*1){pedat->revx=FALSE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=90;pedat->x=pdat->x+zurex(zurex1);pedat->y=zurey1;}
		else if(pedat->counter < d_end + e_spd*2){pedat->revx=FALSE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=135;pedat->x=pdat->x+zurex(zurex2);pedat->y=zurey2;}
		else if(pedat->counter < d_end + e_spd*3){pedat->revx=TRUE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=135;pedat->x=pdat->x+zurex(zurex2*(-1));pedat->y=zurey2;}
		else if(pedat->counter < d_end + e_spd*4){pedat->revx=TRUE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=90;pedat->x=pdat->x+zurex(zurex2*(-1));pedat->y=zurey1;}
		else if(pedat->counter < d_end + e_spd*5){pedat->revx=TRUE;pedat->cnow = DCELL_DOWN;pedat->x=pdat->x+zurex(zurex3*(-1));pedat->y=0;}
		//f part →
		else if(pedat->counter < e_end + f_spd*1){pedat->revx=TRUE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=90;pedat->x=pdat->x+zurex(zurex1*(-1));pedat->y=zurey1;}
		else if(pedat->counter < e_end + f_spd*2){pedat->revx=TRUE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=135;pedat->x=pdat->x+zurex(zurex2*(-1));pedat->y=zurey2;}
		else if(pedat->counter < e_end + f_spd*3){pedat->revx=FALSE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=135;pedat->x=pdat->x+zurex(zurex2);pedat->y=zurey2;}
		else if(pedat->counter < e_end + f_spd*4){pedat->revx=FALSE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=90;pedat->x=pdat->x+zurex(zurex1);pedat->y=zurey1;}
		else if(pedat->counter < e_end + f_spd*5){pedat->revx=FALSE;pedat->cnow = DCELL_DOWN;pedat->x=pdat->x+zurex(zurex3);pedat->y=0;}
		//g part ←
		else if(pedat->counter < f_end + g_spd*1){pedat->revx=FALSE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=90;pedat->x=pdat->x+zurex(zurex1);pedat->y=zurey1;}
		else if(pedat->counter < f_end + g_spd*2){pedat->revx=FALSE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=135;pedat->x=pdat->x+zurex(zurex2);pedat->y=zurey2;}
		else if(pedat->counter < f_end + g_spd*3){pedat->revx=TRUE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=135;pedat->x=pdat->x+zurex(zurex2*(-1));pedat->y=zurey2;}
		else if(pedat->counter < f_end + g_spd*4){pedat->revx=TRUE;pedat->cnow = DCELL_NAGERARE1;pedat->rot=90;pedat->x=pdat->x+zurex(zurex2*(-1));pedat->y=zurey1;}
		else if(pedat->counter < f_end + g_spd*5){pedat->revx=TRUE;pedat->cnow = DCELL_DOWN;pedat->x=pdat->x+zurex(zurex3*(-1));pedat->y=0;}

		else if(nageend){
			pedat->muki = !(pdat->muki);
			pedat->y=0;
			pedat->cnow=DCELL_DOWN;
			StartAttack(ATTACKINFO_SNAGE2);
			AddDamage(pedat->id,(int)pedat->x,0);
			return(ACTID_FUTTOBI);
		}
		else if(pedat->counter >= g_end + 200){
			pedat->muki = !(pdat->muki);
			pedat->y=0;
			nageend = TRUE;
			return(ACTID_DOWN);
		}

	return(0);
}

