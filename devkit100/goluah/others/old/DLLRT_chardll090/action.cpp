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

//*********************************************************
//　　通常動作
//*********************************************************

void CCharacter::act_neutral() //ニュートラル
{
  GOBJECT *pdat = GetGObject();
}

void CCharacter::act_crouch() //しゃがみ
{
  GOBJECT *pdat = GetGObject();
}

void CCharacter::act_rakka() //落下
{
  JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
  GOBJECT *pdat = GetGObject();

  // pdat->cnow = CELL_;
}

void CCharacter::act_tojyo() //登場
{
  GOBJECT *pdat = GetGObject();

  /*	{//行動の最後にはこれを行うこと
                  pdat->aid = ACTID_NEUTRAL;
                  Message2System(MSGOBJ2SYS_TOJYOEND,0);
          }*/
}

void CCharacter::act_win() //勝利
{
  GOBJECT *pdat = GetGObject();

  if (pdat->counter == 0) {
    LoadAndSetKatiSerif();
  }

  //行動の最後にはコレを行うこと
  //	Message2System(MSGOBJ2SYS_WINPOSEEND,0);
}

void CCharacter::act_walkf() //前進
{
  GOBJECT *pdat = GetGObject();
}

void CCharacter::act_walkb() //後退
{
  GOBJECT *pdat = GetGObject();
}

void CCharacter::act_jamps() //ジャンプ予備動作
{
  GOBJECT *pdat = GetGObject();

  //	if(pdat->counter==0)pdat->cnow=CELL_;
  //	else if(pdat->counter==1)pdat->cnow=CELL_;
  //	else pdat->aid = ACTID_JAMPC;
}

void CCharacter::act_jampc() //垂直ジャンプ
{
  GOBJECT *pdat = GetGObject();

  if (pdat->counter == 0) {
    pdat->vy = -20;
    pdat->vx = 0;
  } //初期速度

  //	pdat->cnow = CELL_;
  JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

void CCharacter::act_jampf() //前ジャンプ
{
  GOBJECT *pdat = GetGObject();

  if (pdat->counter == 0) {
    pdat->vy = -20;
    pdat->vx = 6;
  } //初期速度

  //	pdat->cnow = CELL_;
  JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

void CCharacter::act_jampb() //後ジャンプ
{
  GOBJECT *pdat = GetGObject();

  if (pdat->counter == 0) {
    pdat->vy = -20;
    pdat->vx = -6;
  } //初期速度

  //	pdat->cnow = CELL_;
  JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

//*********************************************************
//通常.攻撃
//*********************************************************

void CCharacter::act_att_sa() //立弱
{
  GOBJECT *pdat = GetGObject();
}

void CCharacter::act_att_sb() //立中
{
  GOBJECT *pdat = GetGObject();
}

void CCharacter::act_att_sc() //立強
{
  GOBJECT *pdat = GetGObject();
}

void CCharacter::act_att_ca() //屈弱
{
  GOBJECT *pdat = GetGObject();
}

void CCharacter::act_att_cb() //屈中
{
  GOBJECT *pdat = GetGObject();
}

void CCharacter::act_att_cc() //屈強
{
  GOBJECT *pdat = GetGObject();
}

void CCharacter::act_att_ja() //飛弱
{
  GOBJECT *pdat = GetGObject();

  JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

void CCharacter::act_att_jb() //飛中
{
  GOBJECT *pdat = GetGObject();

  JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

void CCharacter::act_att_jc() //飛強
{
  GOBJECT *pdat = GetGObject();

  JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

//*********************************************************
//　その他
//*********************************************************
void CCharacter::act_taiki() {
  //	GOBJECT* pdat = GetGObject();

  //	if(pdat->counter<2)pdat->cnow=CELL_;
  //	else pdat->cnow= CELL_;
  CCharacterBase::act_taiki(); //キャラクター基本クラスの動作を流用
}

void CCharacter::act_koutai_out() {
  //	GOBJECT* pdat = GetGObject();

  //	if(pdat->counter<2)pdat->cnow=CELL_;
  //	else pdat->cnow= CELL_;
  CCharacterBase::act_koutai_out(); //キャラクター基本クラスの動作を流用
}

void CCharacter::act_koutai_in() {
  //	GOBJECT* pdat = GetGObject();

  //	pdat->cnow= CELL_;
  CCharacterBase::act_koutai_in(); //キャラクター基本クラスの動作を流用
}

void CCharacter::act_koutai() {
  //	GOBJECT* pdat = GetGObject();

  //	pdat->cnow= CELL_;
  CCharacterBase::act_koutai(); //キャラクター基本クラスの動作を流用
}

void CCharacter::act_koutai2() {
  //	GOBJECT* pdat = GetGObject();

  /*	{//行動の最後にはこれを行うこと
                  pdat->aid = ACTID_NEUTRAL;
                  Message2System(MSGOBJ2SYS_KOUTAIEND,0);
          }*/
  CCharacterBase::act_koutai2(); //キャラクター基本クラスの動作を流用
}
