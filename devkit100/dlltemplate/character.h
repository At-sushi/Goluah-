
/*==============================================================

        キャラクター定義ヘッダ

================================================================*/
#pragma once

#include <windows.h>
#include <stdio.h>

#include "gobjbase.h"
#include "cell.h" // 通常、are_editerで生成したヘッダーファイルをここにincludeします

/*--------------------------------------------------------------
        メインキャラクタークラス
----------------------------------------------------------------*/
class CCharacter : public CCharacterBase {
public:
  CCharacter(CDI_CHARACTERINFO2 *info);
  virtual ~CCharacter();

protected:
  //*********************************************
  //  オーバーライドする関数
  //*********************************************

  DWORD Message(DWORD msg, LPVOID pd, DWORD prm);
  DWORD Action();
  BOOL Command_OnNormal(DWORD keyinfo);
  BOOL Command_Normal(DWORD keyinfo);
  BOOL Command_Hissatu(DWORD keyinfo);
  void InitAttackInfo(); //攻撃力情報の初期化
  void InitWazInfo();    //技情報の初期化

  //通常動作
  void act_neutral(); //ニュートラル
  void act_crouch();  //しゃがみ
  void act_rakka();   //落下
  void act_tojyo();   //登場
  void act_win();     //勝利
  void act_walkf();   //前進
  void act_walkb();   //後退
  void act_jamps();   //ジャンプ予備動作
  void act_jampc();   //垂直ジャンプ
  void act_jampf();   //前ジャンプ
  void act_jampb();   //後ジャンプ
  //通常.攻撃
  void act_att_sa(); //立弱
  void act_att_sb(); //立中
  void act_att_sc(); //立強
  void act_att_ca(); //屈弱
  void act_att_cb(); //屈中
  void act_att_cc(); //屈強
  void act_att_ja(); //飛弱
  void act_att_jb(); //飛中
  void act_att_jc(); //飛強
  //その他
  void act_taiki();
  void act_koutai_out();
  void act_koutai_in();
  void act_koutai();
  void act_koutai2();

  //*********************************************
  //　CCharacterクラス独自の関数
  //*********************************************
private:
  void InitParameters(); //パラメータ初期化
  void StartAttack(DWORD atk_info_id);
  void AddPowerGauge(double dp);

  //追加行動関数

  //攻撃力情報
  ATTACKINFO aif[32];
};

extern LPTSTR CharDirectory;

//行動IDをここに追加する

//攻撃力番号をここに定義する
/* sample
#define ATTACKINFO_STAND_A		0
#define ATTACKINFO_STAND_B		1
#define ATTACKINFO_STAND_C		2
#define ATTACKINFO_CROUCH_A		3
#define ATTACKINFO_CROUCH_B		4
#define ATTACKINFO_CROUCH_C		5
#define ATTACKINFO_JAMP_A		6
#define ATTACKINFO_JAMP_B		7
#define ATTACKINFO_JAMP_C		8
*/

//攻撃ID（必要があれば変更する）
#define ATTACK_WEAK 1
#define ATTACK_MIDDLE 2
#define ATTACK_STRONG 3
#define ATTACK_HISSATU 4
#define ATTACK_EREAL 0x00010000

//ユーザー定義メッセージ
#define GOBJMSG_NAGE (GOBJMSG_USERDEFINE | 0x0001) //投げ用

//オプション定義
#define OPTIONS_CHAIN_COMBO 0x00000001
