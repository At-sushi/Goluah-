
/*==============================================================

	通常動作

================================================================*/
#include "character.h"

//*********************************************************
//　　通常動作
//*********************************************************

void CCharacter::act_neutral()//ニュートラル
{
	pdat->cnow = CELL_NEUTRAL;
}

void CCharacter::act_crouch()//しゃがみ
{
	pdat->cnow = CELL_CROUCH;
}

void CCharacter::act_rakka()//落下
{
	JiyuuRakka(2, TRUE, ACTID_NEUTRAL);

	pdat->cnow = CELL_JAMP;
}

void CCharacter::act_tojyo()//登場
{
	if(ccounter==0)PlayMySound(wave_tojyo);
	if(pdat->counter==0)PlayMySound(4);

	if(pdat->counter < 30)pdat->cnow=CELL_TOJO1;
	else if(pdat->counter < 32)pdat->cnow=CELL_TOJO2;
	else if(pdat->counter < 37)pdat->cnow=CELL_TOJO3;
	else if(pdat->counter < 60)pdat->cnow=CELL_TOJO4;
	else if(pdat->counter < 65)pdat->cnow=CELL_TOJO5;
	else {
		pdat->aid = ACTID_NEUTRAL;
		Message2System(MSGOBJ2SYS_TOJYOEND,0);
	}
}

void CCharacter::act_win()//勝利
{
	if(ccounter==0)PlayMySound(wave_win);
	if(pdat->counter==0){
		PlayMySound(5);
		LoadAndSetKatiSerif();
	}
	
	if(pdat->counter < 10)pdat->cnow=CELL_WIN1;
	else if(pdat->counter < 30)pdat->cnow=CELL_WIN2;
	else if(pdat->counter < 75)pdat->cnow=CELL_WIN3;
	else if(pdat->counter < 100)pdat->cnow=CELL_WIN4;
	else if(pdat->counter < 105)pdat->cnow=CELL_WIN5;
	else if(pdat->counter < 106)pdat->cnow=CELL_WIN6;
	else pdat->cnow=CELL_WIN7;

	if(pdat->counter == 120)
		Message2System(MSGOBJ2SYS_WINPOSEEND,0);
}

void CCharacter::act_timeoverlose()//タイムオーバー
{
	if(pdat->counter < 10)pdat->cnow=CELL_WIN1;
	else if(pdat->counter < 30)pdat->cnow=CELL_WIN2;
	else if(pdat->counter < 70){pdat->cnow=CELL_WIN2;pdat->revx=TRUE;}
	else if(pdat->counter < 75)pdat->cnow=CELL_WIN5;
	else if(pdat->counter < 76)pdat->cnow=CELL_WIN6;
	else pdat->cnow=CELL_WIN7;
}

void CCharacter::act_walkf()//前進
{
	pdat->cnow = CELL_WALK1+(pdat->counter/3)%6;
	movex(3);
}

void CCharacter::act_walkb()//後退
{
	pdat->revx = TRUE;

	pdat->cnow = CELL_WALK1+(pdat->counter/3)%6;
	movex(-3);
}

void CCharacter::act_jamps()//ジャンプ予備動作
{
	if(pdat->counter >2)pdat->aid = ACTID_JAMPC;
	pdat->cnow = CELL_JAMP;
}

void CCharacter::act_jampc()//垂直ジャンプ
{
	if(pdat->counter == 0){pdat->vy=-30;pdat->vx=0;}

	pdat->cnow = CELL_JAMP;
	JiyuuRakka(2, TRUE);
}

void CCharacter::act_jampf()//前ジャンプ
{
	if(pdat->counter == 0){pdat->vy=-30;pdat->vx=6;}

	pdat->cnow = CELL_JAMP;
	JiyuuRakka(2, TRUE);
}

void CCharacter::act_jampb()//後ジャンプ
{
	if(pdat->counter == 0){pdat->vy=-30;pdat->vx=-6;}

	pdat->cnow = CELL_JAMP;
	JiyuuRakka(2, TRUE);
}


//*********************************************************
//　その他
//*********************************************************
void CCharacter::act_taiki()
{
//	if(pdat->counter<2)pdat->cnow=CELL_;
//	else pdat->cnow= CELL_;
	CCharacterBase::act_taiki();//キャラクター基本クラスの動作を流用
}

void CCharacter::act_koutai_out()
{
//	if(pdat->counter<2)pdat->cnow=CELL_;
//	else pdat->cnow= CELL_;
	CCharacterBase::act_koutai_out();//キャラクター基本クラスの動作を流用
}

void CCharacter::act_koutai_in()
{
//	pdat->cnow= CELL_;
	CCharacterBase::act_koutai_in();//キャラクター基本クラスの動作を流用
}

void CCharacter::act_koutai()
{
//	GOBJECT* pdat = GetGObject();

//	pdat->cnow= CELL_;
	CCharacterBase::act_koutai();//キャラクター基本クラスの動作を流用
}

void CCharacter::act_koutai2()
{
/*	{//行動の最後にはこれを行うこと
		pdat->aid = ACTID_NEUTRAL;
		Message2System(MSGOBJ2SYS_KOUTAIEND,0);
	}*/	
	CCharacterBase::act_koutai2();//キャラクター基本クラスの動作を流用
}


/*---------------------------------------------------------------------
	ダッシュ
-----------------------------------------------------------------------*/

void CCharacter::act_dashf()
{
	if(pdat->counter == 0){pdat->vy=-10;pdat->vx=8;}

	if(pdat->counter < 5){pdat->cnow = CELL_CROUCH;}
	else{
		JiyuuRakka(0.65,TRUE);
		pdat->cnow = CELL_DASHF1+(pdat->counter/2)%8;
	}
}

void CCharacter::act_dashb()
{
	if(pdat->counter == 0){
		pdat->vy=-8;pdat->vx=-7;
	}

	pdat->cnow = CELL_DASHB1;
	JiyuuRakka(1,TRUE);
}


/*---------------------------------------------------------------------
	2段ジャンプ
-----------------------------------------------------------------------*/

void CCharacter::act_2danjamps()
{
	if(pdat->counter==0)PlayMySound(6);

	if(pdat->counter >2)pdat->aid = ACTID_2DANJAMPC;
	pdat->cnow = CELL_NEUTRAL;
}

void CCharacter::act_2danjampc()
{
	if(pdat->counter == 0){pdat->vy=-27;pdat->vx=0;}
	pdat->cnow = CELL_JAMP;
	JiyuuRakka(2,TRUE);
}

void CCharacter::act_2danjampf()
{
	if(pdat->counter == 0){pdat->vy=-27;pdat->vx=6;}
	pdat->cnow = CELL_JAMP;
	JiyuuRakka(2,TRUE);
}

void CCharacter::act_2danjampb()
{
	if(pdat->counter == 0){pdat->vy=-27;pdat->vx=-6;}
	pdat->cnow = CELL_JAMP;
	JiyuuRakka(2,TRUE);
}



