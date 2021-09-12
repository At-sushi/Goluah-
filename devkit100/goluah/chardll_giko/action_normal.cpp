
/*==============================================================

	行動関数

================================================================*/
#include "character.h"

//*********************************************************
//　　通常動作
//*********************************************************

/*---------------------------------------------------------
	ニュートラル
-----------------------------------------------------------*/
void CCharacter::act_neutral()
{
	cell(CELL_NEUTRAL);
}

/*---------------------------------------------------------
	しゃがみ
-----------------------------------------------------------*/
void CCharacter::act_crouch()
{
	cell(CELL_CROUCH);
}

/*---------------------------------------------------------
	落下
-----------------------------------------------------------*/
void CCharacter::act_rakka()
{
	JiyuuRakka(2, TRUE, ACTID_NEUTRAL);
	pdat->cnow = (pdat->y<-170) ? CELL_JAMP2 : CELL_JAMP1;
}

/*---------------------------------------------------------
	登場
-----------------------------------------------------------*/
void CCharacter::act_tojyo()
{
	if(ccounter==0)PlayMySound(wave_tojyo);

	const UINT anmspd = 3;
	const UINT offset = 25;

	if(ccounter<anmspd+offset)cell( CELL_TOJYO1 );
	else if(ccounter<anmspd*2+offset)cell( CELL_TOJYO2 );
	else if(ccounter<anmspd*3+offset)cell( CELL_TOJYO3 );
	else if(ccounter<anmspd*4+offset)cell( CELL_TOJYO4 );
	else if(ccounter<anmspd*5+offset)cell( CELL_TOJYO5 );
	else if(ccounter<anmspd*8+offset)cell( CELL_TOJYO6 );
	else
	{
		pdat->aid = ACTID_NEUTRAL;
		Message2System(MSGOBJ2SYS_TOJYOEND,0);
	}
}

/*---------------------------------------------------------
	勝利
-----------------------------------------------------------*/
void CCharacter::act_win()
{
	if(ccounter==0)PlayMySound(wave_win);

	if(pdat->counter==0){
		LoadAndSetKatiSerif();
	}

	if(ccounter<50)	cell( CELL_WIN1 + (ccounter/2)%2 );
	else			cell( CELL_WIN3 + (ccounter/2)%2 );

	if(ccounter==120)Message2System(MSGOBJ2SYS_WINPOSEEND,0);
}

/*---------------------------------------------------------
	前進
-----------------------------------------------------------*/
void CCharacter::act_walkf()
{
	movex(3);
	cell( CELL_WALKF1 + (ccounter/5)%5 );
}

/*---------------------------------------------------------
	後退
-----------------------------------------------------------*/
void CCharacter::act_walkb()
{
	movex(-3);
	cell( CELL_WALKB1 + (ccounter/5)%5 );
}

/*---------------------------------------------------------
	ジャンプ予備動作
-----------------------------------------------------------*/
void CCharacter::act_jamps()
{
	if(pdat->counter==0)pdat->cnow=CELL_FUNBARI;
	else pdat->aid = ACTID_JAMPC;
}

/*---------------------------------------------------------
	垂直ジャンプ
-----------------------------------------------------------*/
void CCharacter::act_jampc()
{
	if(pdat->counter == 0){pdat->vy=-27;pdat->vx=0;}//初期速度

	pdat->cnow = (pdat->y<-170) ? CELL_JAMP2 : CELL_JAMP1;
	JiyuuRakka(2, TRUE, ACTID_NEUTRAL);
}

/*---------------------------------------------------------
	前ジャンプ
-----------------------------------------------------------*/
void CCharacter::act_jampf()
{
	if(pdat->counter == 0){pdat->vy=-27;pdat->vx=6;}//初期速度

	pdat->cnow = (pdat->y<-170) ? CELL_JAMP2 : CELL_JAMP1;
	JiyuuRakka(2, TRUE, ACTID_NEUTRAL);
}

/*---------------------------------------------------------
	後ジャンプ
-----------------------------------------------------------*/
void CCharacter::act_jampb()
{
	if(pdat->counter == 0){pdat->vy=-27;pdat->vx=-6;}//初期速度

	pdat->cnow = (pdat->y<-170) ? CELL_JAMP2 : CELL_JAMP1;
	JiyuuRakka(2, TRUE, ACTID_NEUTRAL);
}


/*---------------------------------------------------------
	ダッシュ
-----------------------------------------------------------*/
void CCharacter::act_dashf()
{
	cell( CELL_DASHF1 + (ccounter/2)%8);
	movex(9);
}

void CCharacter::act_dashb()
{
	if(pdat->counter == 0){pdat->vy=-6;pdat->vx=-6;}

	pdat->muteki = TRUE;
	cell( CELL_DASHB1);
	
	JiyuuRakka( 0.8 ,TRUE);
}

/*---------------------------------------------------------
	タイムアップ負け
-----------------------------------------------------------*/
void CCharacter::act_timeoverlose()
{
	cell(CELL_TLOSE);
}

/*---------------------------------------------------------
	召喚
-----------------------------------------------------------*/
void CCharacter::act_strikercomeon()
{
	if(ccounter==0)PlayMySound(wave_striker_call);
	const UINT anmspd = 3;
	const UINT offset = 25;

	if(ccounter<anmspd+offset)cell( CELL_TOJYO1 );
	else if(ccounter<anmspd*2+offset)cell( CELL_TOJYO2 );
	else if(ccounter<anmspd*3+offset)cell( CELL_TOJYO3 );
	else if(ccounter<anmspd*4+offset)cell( CELL_TOJYO4 );
	else if(ccounter<anmspd*5+offset)cell( CELL_TOJYO5 );
	else if(ccounter<anmspd*8+offset)cell( CELL_TOJYO6 );
	else ChangeAction(ACTID_NEUTRAL);
}

//*********************************************************
//　その他
//*********************************************************
void CCharacter::act_taiki()
{
//	if(pdat->counter<2)pdat->cnow=CELL_;
//	else 
	
	pdat->cnow= CELL_JAMP1;
	CCharacterBase::act_taiki();//キャラクター基本クラスの動作を流用
}

void CCharacter::act_koutai_out()
{
//	if(pdat->counter<2)pdat->cnow=CELL_;
//	else 
	
	pdat->cnow= CELL_JAMP1;
	CCharacterBase::act_koutai_out();//キャラクター基本クラスの動作を流用
}

void CCharacter::act_koutai_in()
{
	pdat->cnow= CELL_JAMP1;
	CCharacterBase::act_koutai_in();//キャラクター基本クラスの動作を流用
}

void CCharacter::act_koutai()
{
	pdat->cnow= CELL_JAMP1;
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

