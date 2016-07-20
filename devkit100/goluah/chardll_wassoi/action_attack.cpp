
/*==============================================================

	行動関数

================================================================*/
#include "character.h"
#include <math.h>

//*********************************************************
//通常.攻撃
//*********************************************************

void CCharacter::act_att_sa()//立弱
{
	pdat->cnow = CELL_SA;
	if(pdat->counter > 15){
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

	pdat->cnow = CELL_SB;
	if(pdat->counter > 15){
		pdat->aid = ACTID_NEUTRAL;
	}
}

void CCharacter::act_att_sc()//立強
{
	if(pdat->counter < 2)pdat->cnow = 103;
	else if(pdat->counter < 12){
		pdat->cnow = CELL_SC;
		movex(4);
	}
	else if(pdat->counter < 20){
		pdat->cnow = CELL_SC;
		movex(1);
	}
	else{
		pdat->aid = ACTID_NEUTRAL;
	}

	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_STAND_C);
	}
}

void CCharacter::act_att_ca()//屈弱
{
	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_CROUCH_A);
	}

	pdat->cnow = CELL_CA;
	if(pdat->counter > 15){
		pdat->aid = ACTID_CROUCH;
	}
}

void CCharacter::act_att_cb()//屈中
{
	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_CROUCH_B);
	}

	movex(1);

	if(pdat->counter < 2)pdat->cnow = CELL_CB1;
	else if(pdat->counter < 4)pdat->cnow = CELL_CB2;
	else if(pdat->counter < 6)pdat->cnow = CELL_CB3;
	else if(pdat->counter < 8)pdat->cnow = CELL_CB4;
	else if(pdat->counter <10)pdat->cnow = CELL_CB3;
	else if(pdat->counter <12)pdat->cnow = CELL_CB2;
	else if(pdat->counter <20)pdat->cnow = CELL_CB1;
	else{
		pdat->aid = ACTID_CROUCH;
	}
}

void CCharacter::act_att_cc()//屈強
{
	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_CROUCH_C);
	}

	pdat->cnow = CELL_CC;

	if(pdat->counter < 12)movex(2);
	else if(pdat->counter < 20)movex(1);
	else{
		pdat->aid = ACTID_CROUCH;
	}
}

void CCharacter::act_att_ja()//飛弱
{
	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_JAMP_A);
	}

	if(pdat->counter < 21)pdat->cnow = CELL_JA;
	else pdat->aid = ACTID_RAKKA2;

	JiyuuRakka(3,TRUE);
}

void CCharacter::act_att_jb()//飛中
{
	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_JAMP_B);
	}

	if(pdat->counter < 11)pdat->cnow = CELL_JB;
	else if(pdat->counter <25)pdat->cnow = CELL_JAMP1;
	else pdat->aid = ACTID_RAKKA2;

	JiyuuRakka(3,TRUE);
}

void CCharacter::act_att_jc()//飛強
{
	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_JAMP_C);
	}

	if(pdat->counter<3)pdat->cnow = CELL_JAMP1;
	else if(pdat->counter <15)pdat->cnow = CELL_JC;
	else if(pdat->counter <35)pdat->cnow = CELL_JAMP1;
	else pdat->aid = ACTID_RAKKA2;

	JiyuuRakka(3,TRUE);
}

//*********************************************************
//　　追加動作
//*********************************************************

void CCharacter::act_att_smc()//座ってんじゃねえ！
{
	if(pdat->counter < 5)pdat->cnow = CELL_SUWA1;
	else if(pdat->counter < 20)pdat->cnow = CELL_SUWA2;
	else if(pdat->counter < 25)pdat->cnow = CELL_SUWA3;
	else if(pdat->counter < 30)pdat->cnow = CELL_SUWA4;
	else{
		pdat->aid = ACTID_NEUTRAL;
	}

	movex(1);

	if(pdat->counter == 1){
		StartAttack(ATTACKINFO_STAND_MB);
	}
}



//********************************************************************
//　投げ
//********************************************************************

void CCharacter::act_nage()
{
	DrawFront();
	if(pdat->counter==0)PlayMySound(wave_nage);

	if(pdat->counter < 10+10)pdat->cnow=CELL_NAGE1;
	else if(pdat->counter < 14+10){pdat->cnow=CELL_NAGE2;}
	else if(pdat->counter < 18+10){pdat->cnow=CELL_NAGE3;}
	else if(pdat->counter < 22+10){pdat->cnow=CELL_NAGE4;}
	else if(pdat->counter < 26+10){pdat->cnow=CELL_NAGE5;}
	else if(pdat->counter < 30+10){pdat->cnow=CELL_NAGE6;}
	else if(pdat->counter < 40+10){pdat->cnow=CELL_NAGE7;}
	else pdat->aid = ACTID_NEUTRAL;

	if(pdat->counter == 0){
		AddEffect(EFCTID_MARKC,(int)pdat->x + zurex(50),-50,0);
		PlaySysSound(SYSTEMSOUND_CATCH);
	}
	if(pdat->counter == 14+10){
		AddEffect(EFCTID_MARK2,(int)pdat->x + zurex(70),0,0);
		PlaySysSound(SYSTEMSOUND_HIT2);
	}
}

DWORD CCharacter::act_nagee(GOBJECT *pedat)
{
	if(pedat->counter < 10+10){
		pedat->cnow=DCELL_DAMAGES1;
		pedat->x=pdat->x+zurex(50);
		pedat->y= 0;
		pedat->muki = !pdat->muki;
		return(0);
	}
	else if(pedat->counter < 30+10){
		pedat->x=pdat->x -zurex(20) +zurex(80)*cos( (pedat->counter-20)*12/180.0*3.1415 - 3.1415/4.0 );
		pedat->y=-60 - 80*sin( (pedat->counter-20)*12/180.0*3.1415  - 3.1415/4.0  );
		pedat->rot = (pedat->counter-20)*9;
		return(0);
	}
	else{
		StartAttack(ATTACKINFO_NAGE);
		AddDamage(pedat->id,(int)pedat->x,0);
		pedat->y=0;
		return(ACTID_DOWN);
	}
}

//********************************************************************
//　支援攻撃
//********************************************************************

void CCharacter::act_striker1()//飛んでくるとコ
{
	pdat->kasanari=FALSE;

	if(pdat->counter==0){
		pdat->objtype |= GOBJTYPE_DISPLAY;//画面内にいる
	}

	GOBJECT *pedat;
	if(pdat->counter==0){
		pedat = GetActiveCharacter(pdat->tid);//"e"じゃないけど。
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
		pdat->aid = ACTID_STRIKER2;
	}
}

void CCharacter::act_striker2()//攻撃(1)
{
	if(pdat->counter==0)
	{
		DWORD tgt_oid = 0;
		GOBJECT *ptgt = GetActiveCharacter(pdat->tid);
		if(ptgt)
		{
			tgt_oid = ptgt->id;
		}

		b_wlight->Go(FALSE,0,0,0,0);
		b_wlight->SetTarget(tgt_oid);
	}

	if(pdat->counter < 60){
		switch((pdat->counter/4)%6){
		case 0:pdat->cnow=CELL_WALK3;pdat->muki=FALSE;break;
		case 1:pdat->cnow=CELL_WALK1;pdat->muki=FALSE;break;
		case 2:pdat->cnow=CELL_WALK2;pdat->muki=FALSE;break;
		case 3:pdat->cnow=CELL_WALK3;pdat->muki=TRUE;break;
		case 4:pdat->cnow=CELL_WALK1;pdat->muki=TRUE;break;
		case 5:pdat->cnow=CELL_WALK2;pdat->muki=TRUE;break;
		}
	}
	else pdat->aid = ACTID_STRIKER3;

	GOBJECT *pmdat = GetActiveCharacter(pdat->tid);
	if(pmdat!=NULL){
		if(pmdat->hp > 0){
			pmdat->hp ++;
			if(pmdat->hp > (int)pmdat->hpmax)pmdat->hp = pmdat->hpmax;
		}
	/*	if(pdat->counter%4==0){
			CreateWhiteLight(pmdat->x + rand()%50-25,pmdat->y-rand()%50);
		}*/
	}
}

void CCharacter::act_striker3()//攻撃(2)
{
	if(pdat->counter<10)pdat->cnow = CELL_NEUTRAL;
	else pdat->aid=ACTID_STRIKER4;
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
