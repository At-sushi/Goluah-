
/*==============================================================

	行動関数（通常動作）

================================================================*/
#include "character.h"

//**************************************************************
//　　通常動作
//**************************************************************

/*--------------------------------------------------------------
	ニュートラル
----------------------------------------------------------------*/
void CCharacter::act_neutral()
{
	cell(CELL_NEUTRAL);
}

/*--------------------------------------------------------------
	しゃがみ
----------------------------------------------------------------*/
void CCharacter::act_crouch()
{
	cell(CELL_CROUCH);
}

/*--------------------------------------------------------------
	落下
----------------------------------------------------------------*/
void CCharacter::act_rakka()
{
	JiyuuRakka(2, TRUE, ACTID_NEUTRAL);
	cell(CELL_JAMP);
}

/*--------------------------------------------------------------
	登場
----------------------------------------------------------------*/
void CCharacter::act_tojyo()
{
	if(ccounter==0)PlayMySound(wave_tojyo);
	
	if(pdat->counter < 30)pdat->cnow=CELL_TOJYO1;
	else if(pdat->counter < 33)pdat->cnow=CELL_TOJYO2;
	else if(pdat->counter < 60)pdat->cnow=CELL_TOJYO3;
	else if(pdat->counter < 68)pdat->cnow=CELL_TOJYO4;
	else if(pdat->counter < 76)pdat->cnow=CELL_TOJYO5;
	else if(pdat->counter < 84)pdat->cnow=CELL_TOJYO6;
	else {
		pdat->aid = ACTID_NEUTRAL;
		Message2System(MSGOBJ2SYS_TOJYOEND,0);
	}
}

/*--------------------------------------------------------------
	勝利
----------------------------------------------------------------*/
void CCharacter::act_win()
{
	if(ccounter==0)PlayMySound(wave_win);

	if(pdat->counter==0){
		PlayMySound(wave_win);
		LoadAndSetKatiSerif();
	}
	
	if(pdat->counter==0){
		pdat->vy=-12;
		pdat->y = -1;
	}
	
	if(pdat->y < 0){
		pdat->y += pdat->vy;
		pdat->vy += pdat->counter%2;
		if(pdat->y >= 0){pdat->y=1;pdat->counter=100;}
		pdat->cnow = CELL_WIN1;
	}
	else{
		if(pdat->counter<105)pdat->cnow=CELL_WIN2;
		else pdat->cnow=CELL_WIN3;
	}

	if(pdat->counter==180)
		Message2System(MSGOBJ2SYS_WINPOSEEND,0);
}


/*--------------------------------------------------------------
	ストライカー
----------------------------------------------------------------*/
void CCharacter::act_strikercomeon()
{
	if(ccounter==0)PlayMySound(wave_striker_call);

	if(pdat->counter<5)cell(CELL_WIN2);
	else if(pdat->counter<20)cell(CELL_WIN3);
	else ChangeAction(ACTID_NEUTRAL);
}


/*--------------------------------------------------------------
	タイムオーバー
----------------------------------------------------------------*/
void CCharacter::act_timeoverlose()
{
	cell(CELL_TLOSE);
}


/*--------------------------------------------------------------
	前進
----------------------------------------------------------------*/
void CCharacter::act_walkf()
{
	pdat->cnow = CELL_WALKF1+(pdat->counter/3)%2;
	movex(3);
}

/*--------------------------------------------------------------
	後退
----------------------------------------------------------------*/

void CCharacter::act_walkb()
{
	pdat->cnow = CELL_WALKB1+(pdat->counter/3)%2;
	movex(-3);
}

/*--------------------------------------------------------------
	前ダッシュ
----------------------------------------------------------------*/
void CCharacter::act_dashf()
{
	if(pdat->counter==0){
		pefe_dash->Go( pdat->muki,pdat->x,pdat->y,2);
	}

	pdat->cnow = CELL_DASH1+(pdat->counter/2)%2;
	movex(9);
	// CREATE_GOBJ_FX( CShadowEffect(this) )
}

/*--------------------------------------------------------------
	後ダッシュ
----------------------------------------------------------------*/
void CCharacter::act_dashb()
{
	if(pdat->counter==0){
		pefe_dash->Go( !pdat->muki,pdat->x,pdat->y,2);
	}

	pdat->cnow = CELL_DASH1+(pdat->counter/2)%2;
	pdat->revx = TRUE;
	movex(-9);
	// CREATE_GOBJ_FX( CShadowEffect(this) )
}

/*--------------------------------------------------------------
	ジャンプ予備動作
----------------------------------------------------------------*/
void CCharacter::act_jamps()
{
	if(pdat->counter >2)pdat->aid = ACTID_JAMPC;
	pdat->cnow = CELL_JAMP;
}

/*--------------------------------------------------------------
	2段ジャンプ予備動作
----------------------------------------------------------------*/
void CCharacter::act_2nd_step()
{
	m_2nd_step_flag = FALSE;
	chainlist &= ~CHAIN_JAMPS;//空中のチェーンコンボフラグをクリア

	if(pdat->counter<2)cell(CELL_JAMP);
	else pdat->aid = ACTID_JAMPC;
}

/*--------------------------------------------------------------
	垂直ジャンプ
----------------------------------------------------------------*/
void CCharacter::act_jampc()
{
	if(pdat->counter == 0){
		if(m_aerial && m_2nd_step_flag)pdat->vy= m_opt_AStep ? -27 : -15;
		else pdat->vy= m_opt_AStep ? /*-22 : -23*/-25 : -27;
		pdat->vx=0;
	}

	pdat->cnow = CELL_JAMP1;

	pdat->y += pdat->vy;
	pdat->vy+=2;
	if(pdat->y > 0){
		pdat->aid = ACTID_NEUTRAL;
		pdat->y =0;
		Furimuki();
	}
}

/*--------------------------------------------------------------
	前ジャンプ
----------------------------------------------------------------*/
void CCharacter::act_jampf()
{
	if(pdat->counter == 0){
		pdat->vy= m_opt_AStep ? /*-23 : -23*/-25 : -27;
		pdat->vx=6;
	}

	pdat->cnow = CELL_JAMP1;

	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy+=2;
	if(pdat->y > 0){
		pdat->aid = ACTID_NEUTRAL;
		pdat->y =0;
		Furimuki();
	}
}

/*--------------------------------------------------------------
	後ジャンプ
----------------------------------------------------------------*/
void CCharacter::act_jampb()
{
	if(pdat->counter == 0){
		pdat->vy= m_opt_AStep ? /*-23 : -23*/-25: -27;
		pdat->vx=-6;
	}

	pdat->cnow = CELL_JAMP1;

	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy+=2;
	if(pdat->y > 0){
		pdat->aid = ACTID_NEUTRAL;
		pdat->y =0;
		Furimuki();
	}
}


/*--------------------------------------------------------------
	エリアル用ジャンプ
----------------------------------------------------------------*/
void CCharacter::act_ajamp()
{
	if(pdat->counter == 0)
	{
		pdat->vy= -38;
		pdat->vx=7;
		m_aerial = TRUE;
	}

	pdat->cnow = CELL_JAMP1;

	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy += 3;
	if(pdat->y > 0){
		pdat->aid = ACTID_NEUTRAL;
		pdat->y =0;
		Furimuki();
	}
}

//***************************************************************
//　その他
//***************************************************************

/*--------------------------------------------------------------
	待機行動
----------------------------------------------------------------*/
void CCharacter::act_taiki()
{
	if(pdat->counter<2)pdat->cnow=CELL_CROUCH;
	else pdat->cnow= CELL_JAMP1;
	CCharacterBase::act_taiki();//キャラクター基本クラスの動作を流用
}

/*--------------------------------------------------------------
	交代して、画面外に出て行く
----------------------------------------------------------------*/
void CCharacter::act_koutai_out()
{
	if(pdat->counter<2)pdat->cnow=CELL_CROUCH;
	else pdat->cnow= CELL_JAMP1;
	CCharacterBase::act_koutai_out();//キャラクター基本クラスの動作を流用
}

/*--------------------------------------------------------------
	交代して、画面内に入ってくる
----------------------------------------------------------------*/
void CCharacter::act_koutai_in()
{
	pdat->cnow= CELL_JAMP1;
	CCharacterBase::act_koutai_in();//キャラクター基本クラスの動作を流用
}

/*--------------------------------------------------------------
	交代？
	なんだっけ？
----------------------------------------------------------------*/
void CCharacter::act_koutai()
{
//	GOBJECT* pdat = GetGObject();

	pdat->cnow= CELL_JAMP1;
	CCharacterBase::act_koutai();//キャラクター基本クラスの動作を流用
}

/*--------------------------------------------------------------
	交代して、画面内に入ってくる
	KOFライクな対戦形式の場合
----------------------------------------------------------------*/
void CCharacter::act_koutai2()
{
/*	{//行動の最後にはこれを行うこと
		pdat->aid = ACTID_NEUTRAL;
		Message2System(MSGOBJ2SYS_KOUTAIEND,0);
	}*/	

	pdat->cnow= CELL_JAMP1;
	CCharacterBase::act_koutai2();//キャラクター基本クラスの動作を流用
}

