
/*==============================================================

	行動関数（通常動作）

================================================================*/
#include "el_character.h"

//*********************************************************
//　　通常動作
//*********************************************************

void CCharacterEL::act_neutral()//ニュートラル
{
	cell(CELL_NEUTRAL);
}

void CCharacterEL::act_crouch()//しゃがみ
{
	cell(CELL_CROUCH);
}

void CCharacterEL::act_rakka()//落下
{
	JiyuuRakka(2, TRUE, ACTID_NEUTRAL);

	pdat->cnow = CELL_JAMPF;
}

void CCharacterEL::act_tojyo()//登場
{
	if(ccounter==0)PlayMySound(wave_tojyo);

	if(ccounter < 12)		cell( CELL_TOJYO1 );
	else if(ccounter < 15)	cell( CELL_TOJYO2 );
	else
	{
		UINT cntr = ccounter-15;
		if(cntr==0){
			pdat->vy = -10;
		}
		pdat->y  += pdat->vy;
		if(pdat->vy<0)pdat->vy += 0.4;
		else if(pdat->cnow==CELL_JAMPF)pdat->vy += 1.0;
		else pdat->vy += 0.05;

		UINT anmspd = 8;

		if(cntr < 3)		cell( CELL_TOJYO3 );
		else if(cntr < 6)	cell( CELL_TOJYO4 );
		else if(cntr < 9)	cell( CELL_TOJYO5 );
		else if(cntr < 9+anmspd*1)	cell( CELL_TOJYO6 );
		else if(cntr < 9+anmspd*2)	cell( CELL_TOJYO7 );
		else if(cntr < 9+anmspd*3)	cell( CELL_TOJYO8 );
		else if(cntr < 9+anmspd*4)	cell( CELL_TOJYO9 );
		else if(cntr < 9+anmspd*5)	cell( CELL_TOJYO10 );
		else if(cntr < 9+anmspd*6)	cell( CELL_TOJYO11 );
		else if(cntr < 9+anmspd*7)	cell( CELL_TOJYO12 );
		else if(cntr < 9+anmspd*8)	cell( CELL_TOJYO13 );
		else if(cntr < 9+anmspd*9)	cell( CELL_TOJYO14 );
		else cell(CELL_JAMPF);

		//着地したら終了
		if(pdat->y>0){
			pdat->aid = ACTID_NEUTRAL;
			Message2System(MSGOBJ2SYS_TOJYOEND,0);
		}
	}
}

void CCharacterEL::act_win()//勝利
{
	if(ccounter==0)PlayMySound(wave_win);

	if(pdat->counter==0){
		LoadAndSetKatiSerif("morarael\\serifu.txt");
	}

	if(ccounter<120)cell( CELL_WIN1 + (ccounter/3)%10 );
	else Message2System(MSGOBJ2SYS_WINPOSEEND,0);
}

void CCharacterEL::act_walkf()//前進
{
	movex(5);
	cell( CELL_WALKF1 + (ccounter/3)%3 );
}

void CCharacterEL::act_walkb()//後退
{
	movex(-5);
	cell( CELL_WALKB1 + (ccounter/3)%3 );
}

void CCharacterEL::act_jamps()//ジャンプ予備動作
{
	if(pdat->counter==0)pdat->cnow=CELL_JAMP;
	else if(pdat->counter==1)pdat->cnow=CELL_JAMPF;
	else pdat->aid = ACTID_JAMPC;
}

void CCharacterEL::act_jampc()//垂直ジャンプ
{
	if(pdat->counter == 0){pdat->vy=-30;pdat->vx=0;}//初期速度

	pdat->cnow = CELL_JAMPF;
	JiyuuRakka(2, TRUE, ACTID_NEUTRAL);
}

void CCharacterEL::act_jampf()//前ジャンプ
{
	if(pdat->counter == 0){pdat->vy=-30;pdat->vx=6;}//初期速度

	pdat->cnow = CELL_JAMPF;
	JiyuuRakka(2, TRUE, ACTID_NEUTRAL);
}

void CCharacterEL::act_jampb()//後ジャンプ
{
	if(pdat->counter == 0){pdat->vy=-30;pdat->vx=-6;}//初期速度

	pdat->cnow = CELL_JAMPB;
	JiyuuRakka(2, TRUE, ACTID_NEUTRAL);
}

void CCharacterEL::act_dashf()
{
	if(pdat->counter==0){
		pdat->vx=13;
		pdat->vy=-6;
	}
	if(pdat->counter < 3){pdat->cnow=CELL_JAMP;}
	else if(pdat->y > -400){
		movex(pdat->vx);
		pdat->y += pdat->vy;
		pdat->cnow = CELL_DASHF1 + pdat->counter%2;
	}
	else{pdat->aid=ACTID_RAKKA2;}

	// 暫定処置、飛びすぎはぃゃん
	if(IsLocalCom() && pdat->counter==30)
		pdat->aid = ACTID_RAKKA2;
}

void CCharacterEL::act_dashb()
{
	if(pdat->counter==0){
		pdat->vx=-13;
		pdat->vy=-6;
	}
	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->cnow = CELL_DASHB1 + pdat->counter%2;

	if(pdat->y < -400){
		pdat->aid=ACTID_RAKKA2;
	}

	// 暫定処置、飛びすぎはぃゃん
	if(IsLocalCom() && pdat->counter==30)
		pdat->aid = ACTID_RAKKA2;
}

void CCharacterEL::act_dashfj()
{
	if(pdat->counter==0){
		pdat->vx=15;
		pdat->vy=-1;
	}

	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->cnow = CELL_DASHF1 + pdat->counter%2;

	if(pdat->y < -400){
		pdat->aid = ACTID_RAKKA2;
	}

	// 暫定処置、飛びすぎはぃゃん
	if(IsLocalCom() && pdat->counter==20)
		pdat->aid = ACTID_RAKKA2;
}

void CCharacterEL::act_dashbj()
{
	if(pdat->counter==0){
		pdat->vx=-15;
		pdat->vy=-1;
	}

	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->cnow = CELL_DASHB1 + pdat->counter%2;

	if(pdat->y < -400){
		pdat->aid = ACTID_RAKKA2;
	}

	// 暫定処置、飛びすぎはぃゃん
	if(IsLocalCom() && pdat->counter==20)
		pdat->aid = ACTID_RAKKA2;
}

void CCharacterEL::act_timeoverlose()
{
	cell( CELL_TLOSE1 + (ccounter/2)%2 );
}

//*********************************************************
//　その他
//*********************************************************
void CCharacterEL::act_taiki()
{
	if(pdat->counter<2)pdat->cnow=CELL_JAMP;
	else pdat->cnow= CELL_JAMPB;
	CCharacterBase::act_taiki();//キャラクター基本クラスの動作を流用
}

void CCharacterEL::act_koutai_out()
{
	if(pdat->counter<2)pdat->cnow=CELL_JAMP;
	else pdat->cnow= CELL_JAMPB;
	CCharacterBase::act_koutai_out();//キャラクター基本クラスの動作を流用
}

void CCharacterEL::act_koutai_in()
{
	pdat->cnow= CELL_JAMPF;
	CCharacterBase::act_koutai_in();//キャラクター基本クラスの動作を流用
}

void CCharacterEL::act_koutai()
{
	pdat->cnow= CELL_JAMPF;
	CCharacterBase::act_koutai();//キャラクター基本クラスの動作を流用
}

void CCharacterEL::act_koutai2()
{
/*	{//行動の最後にはこれを行うこと
		pdat->aid = ACTID_NEUTRAL;
		Message2System(MSGOBJ2SYS_KOUTAIEND,0);
	}*/	
	CCharacterBase::act_koutai2();//キャラクター基本クラスの動作を流用
}

