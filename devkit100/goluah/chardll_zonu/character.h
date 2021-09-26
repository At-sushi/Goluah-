
/*==============================================================

        キャラクター定義ヘッダ

================================================================*/
#pragma once

#include <windows.h>
#include <stdio.h>

#include "gobjbase.h"
#include "cell.h" // 通常、are_editerで生成したヘッダーファイルをここにincludeします

#define NEEDGAUGE_SA (0.1f)
#define AIF_NUM (32)

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
  DWORD TouchA(ATTACKINFO *info, DWORD ta_eid);
  DWORD TouchB(ATTACKINFO *info, BOOL hit);
  BOOL Command_OnNormal(DWORD keyinfo);
  BOOL Command_Normal(DWORD keyinfo);
  BOOL Command_Hissatu(DWORD keyinfo);
  DWORD CommandCOM(DWORD wid);
  DWORD CmdCom_OnSystem(DWORD wid);
  DWORD CmdCom_OnHissatuAttacking(DWORD wid);
  DWORD CmdCom_OnNormal(DWORD wid);
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
  void act_strikercomeon();

  void act_timeoverlose(); //タイムオーバー

  //*********************************************
  //　CCharacterクラス独自の関数
  //*********************************************
private:
  void InitParameters(); //パラメータ初期化
  void StartAttack(DWORD atk_info_id);
  BOOL com624624(int dt);

  //攻撃力情報
  ATTACKINFO aif[AIF_NUM];

  //追加行動
  void act_dashf();
  void act_dashb();
  void act_nage();
  DWORD act_nagee(GOBJECT *pedat);
  void act_hnage();
  DWORD act_hnagee(GOBJECT *pedat);
  void act_tetu1();
  void act_tetu2();
  void act_tetu3();
  void act_snage();
  DWORD act_snagee(GOBJECT *pedat);
  void act_striker1();
  void act_striker2();
  void act_striker3();
  void act_striker4();

  //オプション値
  BOOL opt_heavy_hit;
  BOOL opt_super_armer;
};

extern LPTSTR CharDirectory;

//行動IDをここに追加する

#define ACTID_NAGE1 (ACTID_ATTACK | ACTID_NAGE | 0x0001)
#define ACTID_TETU1 (ACTID_ATTACK | ACTID_HISSATU | 0x0002)
#define ACTID_TETU2 (ACTID_ATTACK | ACTID_HISSATU | 0x0003)
#define ACTID_TETU3 (ACTID_ATTACK | ACTID_HISSATU | 0x0004)
#define ACTID_HNAGE (ACTID_ATTACK | ACTID_HISSATU | ACTID_NAGE | 0x0005)
#define ACTID_SNAGE (ACTID_ATTACK | ACTID_HISSATU | ACTID_NAGE | 0x0006)
#define ACTID_DASHF (0x0007)
#define ACTID_DASHB (0x0008)
#define ACTID_STRIKER1 (ACTID_INOUT | 0x0009) //ストライカー攻撃
#define ACTID_STRIKER2 (ACTID_INOUT | 0x000A)
#define ACTID_STRIKER3 (ACTID_INOUT | 0x000B)
#define ACTID_STRIKER4 (ACTID_INOUT | 0x000C) //すごすごと帰っていく

//攻撃力番号をここに定義する
#define ATTACKINFO_STAND_A 0
#define ATTACKINFO_STAND_B 1
#define ATTACKINFO_STAND_C 2
#define ATTACKINFO_CROUCH_A 3
#define ATTACKINFO_CROUCH_B 4
#define ATTACKINFO_CROUCH_C 5
#define ATTACKINFO_JAMP_A 6
#define ATTACKINFO_JAMP_B 7
#define ATTACKINFO_JAMP_C 8
#define ATTACKINFO_NAGE 9
#define ATTACKINFO_HNAGE1 10
#define ATTACKINFO_HNAGE2 11
#define ATTACKINFO_CHARGE1 12
#define ATTACKINFO_CHARGE2 13
#define ATTACKINFO_CHARGE3 14
#define ATTACKINFO_SNAGE1 15
#define ATTACKINFO_SNAGE2 16

//攻撃ID（必要があれば変更する）
#define ATTACK_WEAK 1
#define ATTACK_MIDDLE 2
#define ATTACK_STRONG 3
#define ATTACK_HISSATU 4

//ユーザー定義メッセージ
//投げ用メッセージ
#define GOBJMSG_NAGE1 (GOBJMSG_USERDEFINE | 0x0001) //通常投げ
#define GOBJMSG_NAGE2 (GOBJMSG_USERDEFINE | 0x0002) //必殺投げ
#define GOBJMSG_NAGE3 (GOBJMSG_USERDEFINE | 0x0003) //超必殺投げ

//オプション定義
#define OPTIONS_CHAIN_COMBO 0x00000001
#define OPTIONS_HEAVY_HIT 0x00000002
#define OPTIONS_SUPER_ARMER 0x00000004
#define OPTIONS_HP_UP 0x00000008
#define OPTIONS_HP_UP2 0x00000010

//間合い定義
#define MAAI_NAGE (20)
#define MAAI_HNAGE (35)
#define MAAI_SNAGE (50)

// wav番号
#define wave_tojyo 4
#define wave_win 5
#define wave_nage 6
#define wave_charge 7
#define wave_hnage1 8
#define wave_hnage2 9
#define wave_snage1 10
#define wave_snage2 11

// YEAH!!
#define YEAH TRUE
