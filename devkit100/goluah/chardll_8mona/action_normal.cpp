/*=====================================================================================

Goluah!! Copyright (C) 2001-2004 aki, 2004-2016 At-sushi, 2014-2016 logger

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

=======================================================================================*/

/*==============================================================

	行動関数(通常動作)

================================================================*/
#include "character.h"

//*********************************************************
//　　通常動作
//*********************************************************

//! ニュートラ
void CCharacter::act_neutral()
{
	pdat->cnow = CELL_NEUTRAL;
}

//! しゃがみ
void CCharacter::act_crouch()
{
	pdat->cnow = CELL_CROUCH;
}

//! 落下
void CCharacter::act_rakka()
{
	pdat->cnow = CELL_JAMP1;

	JiyuuRakka(1, TRUE);
}

//! 登場
void CCharacter::act_tojyo()
{
	if(pdat->counter == 0){
		pdat->y = -400;
		pdat->vy = 5;
		PlayMySound(wave_tojyo);
	}

	if(pdat->y < 0){
		pdat->cnow = CELL_JAMP1;
		pdat->y += pdat->vy;
		pdat->vy ++;
	}
	else pdat->cnow = CELL_NAKAMA16;

	if(pdat->counter > 100){
		pdat->y=0;
		pdat->aid = ACTID_NEUTRAL;
		Message2System(MSGOBJ2SYS_TOJYOEND,0);
	}
}

//! 勝利
void CCharacter::act_win()
{
	if(pdat->counter==0){
		LoadAndSetKatiSerif();
		clabul_nak1->Go(pdat->muki,pdat->x-100,0);
		clabul_nak2->Go(pdat->muki,pdat->x+100,0);
		PlayMySound(wave_win);
	}

	if(pdat->counter == 62){
		clabul_nak1->Hide();
		clabul_nak2->Hide();
		pdat->cnow = CELL_WIN;
	}

	if(pdat->counter == 120)
		Message2System(MSGOBJ2SYS_WINPOSEEND,0);
}

//! 前進
void CCharacter::act_walkf()
{
	pdat->cnow = CELL_WALK1+(pdat->counter/6)%4;
	movex(6);
}

//! 後退
void CCharacter::act_walkb()
{
	pdat->cnow = CELL_WALKB1+(pdat->counter/6)%4;
	movex(-6);
}

//! ジャンプ予備動作
void CCharacter::act_jamps()
{
	if(pdat->counter >2)pdat->aid = ACTID_JAMPC;
	pdat->cnow = CELL_JAMP;
}

//!垂直ジャンプ
void CCharacter::act_jampc()
{
	if(pdat->counter == 0){pdat->vy=-20;pdat->vx=0;}

	pdat->cnow = CELL_JAMP1;

	JiyuuRakka(1, TRUE);
}

//!前ジャンプ
void CCharacter::act_jampf()
{
	if(pdat->counter == 0){pdat->vy=-20;pdat->vx=6;}

	pdat->cnow = CELL_JAMP1;

	JiyuuRakka(1, TRUE);
}

//!後ジャンプ
void CCharacter::act_jampb()
{
	if(pdat->counter == 0){pdat->vy=-20;pdat->vx=-6;}

	pdat->cnow = CELL_JAMP1;

	JiyuuRakka(1, TRUE);
}


//*********************************************************
//　その他
//*********************************************************
void CCharacter::act_taiki()
{
	pdat->cnow= CELL_JAMP1;
	CCharacterBase::act_taiki();//キャラクター基本クラスの動作を流用
}

void CCharacter::act_koutai_out()
{
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
	//pdat->cnow= CELL_JAMP1;
	//CCharacterBase::act_koutai();//キャラクター基本クラスの動作を流用

	pdat->objtype |= GOBJTYPE_DISPLAY;//画面内にいる

	GOBJECT *pedat;
	if(pdat->counter==0){
		if(pdat->tid==TEAM_PLAYER1)
			pedat = GetActiveCharacter( TEAM_PLAYER2 );
		else
			pedat = GetActiveCharacter(TEAM_PLAYER1);
		if(pedat!=NULL){
			if(pedat->x>0){
				pdat->x = pedat->x -(360);
				pdat->muki=FALSE;
				//pdat->vx=14;
			}
			else{
				pdat->x = pedat->x +(360);
				pdat->muki=TRUE;
				//pdat->vx=14;
			}
		}
		else {
			pdat->x=0;
			pdat->vx=0;
		}
		//pdat->y = -600;
		//pdat->vy= 0;
	}

	pdat->objtype &= ~GOBJFLG_TAIKI;//待機フラグを消す
	AddProperty( GOBJFLG_TOBETARGET );//ターゲットになる
	pdat->aid = ACTID_KOUTAI2;//登場ポーズ
}

void CCharacter::act_koutai2()
{
	CCharacter::act_tojyo();	//動作を流用
}

//*********************************************************
//　追加動作
//*********************************************************

void CCharacter::act_strikercomeon()
{
	if(pdat->counter<30)pdat->cnow=CELL_ESCAPE;
	else pdat->aid=ACTID_NEUTRAL;
}

void CCharacter::act_dashf()
{
	if(pdat->counter == 0){
		clabul_wind->Go(pdat->muki,pdat->x,0,0);
		pdat->vx=15;
	}
	pdat->cnow = opt_super_armer ? CELL_DASHF2 : CELL_DASHF;
	movex(pdat->vx);
	pdat->vx--;
	if(pdat->vx <=0){
		pdat->aid = ACTID_NEUTRAL;
	}
}

void CCharacter::act_dashb()
{
	if(pdat->counter == 0){
		clabul_wind->Go(!pdat->muki,pdat->x,0,0);
		pdat->vx = -15;
	}
	pdat->cnow = CELL_DASHB;
	movex(pdat->vx);
	pdat->vx++;
	if(pdat->vx >=0)pdat->aid = ACTID_NEUTRAL;
}


void CCharacter::act_timeoverlose()
{
	cell(CELL_TLOSE);
}