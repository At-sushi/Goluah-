
/*==============================================================

        キャラクター定義ヘッダ

        Goluah!! Copyright (C) 2001-2004 aki, 2014-2015 logger, 2004-2015 At-sushi

        This program is free software; you can redistribute it and/or modify it under the terms of the GNU
General Public License as published by the Free Software Foundation; either version 2 of the License, or (at
your option) any later version.

        This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
License for more details.

        You should have received a copy of the GNU General Public License along with this program; if not,
write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

================================================================*/
#pragma once

#include <windows.h>
#include <stdio.h>

#include <d3d8.h>
#include <d3dx8.h>

#include "gobjbase.h"
#include "cell.h" // 通常、are_editerで生成したヘッダーファイルをここにincludeします

//オプション定義
#define OPTIONS_HEAVY_HIT 0x00000001
#define OPTIONS_AUTO_GUARD 0x00000002
#define OPTIONS_GUARD_CANCEL 0x00000004
#define OPTIONS_KOROKORO 0x00000008
#define OPTIONS_HPUP 0x00000010
#define OPTIONS_REGENERATOR 0x00000020

#define AIF_NUM (32)
#define NEEDGAUGE_GC (0.3f)
#define NEEDGAUGE_REV (0.4f)
#define NUM_BLURS (3)

class CWassoiLight;

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
  void PreAction();
  DWORD TouchA(ATTACKINFO *info, DWORD ta_eid);
  DWORD TouchB(ATTACKINFO *info, BOOL hit);
  BOOL Command_OnSystem(DWORD key);
  BOOL Command_OnNormal(DWORD keyinfo);
  BOOL Command_Normal(DWORD keyinfo);
  BOOL Command_Hissatu(DWORD keyinfo);
  BOOL Command_OnAttacking(DWORD key);
  DWORD CommandCOM(DWORD wid);
  DWORD CmdCom_OnSystem(DWORD wid);
  DWORD CmdCom_OnNormal(DWORD wid);
  void InitAttackInfo(); //攻撃力情報の初期化
  void InitWazInfo();    //技情報の初期化
  void InitBullets();
  DWORD ComThink();
  DWORD Draw();

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

  void act_timeoverlose(); //タイムオーバー

  //*********************************************
  //　CCharacterクラス独自の関数
  //*********************************************
private:
  void InitParameters(); //パラメータ初期化
  void StartAttack(DWORD atk_info_id);

  //追加行動関数
  void act_dashf();
  void act_dashb();
  void act_att_smc();
  void act_wassoi1();
  void act_wassoi2();
  void act_wassoi3();
  void act_wassoif();
  void act_hadou1();
  void act_hadou2();
  void act_hadou3();
  void act_syara1();
  void act_syara2();
  void act_syara3();
  void act_syaraf();
  void act_festival();
  void act_festivaldash();
  void act_festivalsuki();
  void act_festivalrush();
  void act_festivalfinish();
  void act_gcancel1();
  void act_gcancel2();
  void act_gcancel3();
  void act_nage();
  DWORD act_nagee(GOBJECT *pedat);
  void act_okiagari_foword();
  void act_okiagari_back();
  void act_striker1();
  void act_striker2();
  void act_striker3();
  void act_striker4();

  //攻撃力情報
  ATTACKINFO aif[AIF_NUM];

  //飛道具
  CClassicalBullet *b_nori;
  CClassicalBullet *b_nori2;
  CWassoiLight *b_wlight;

  // options
  DWORD option_flag;
  BOOL OptHeavyHit() { return FALSE; }
  BOOL OptGuardCancel() { return TRUE; }
  BOOL OptKoroKoro() { return (option_flag & OPTIONS_KOROKORO) ? TRUE : FALSE; }
  BOOL OptRegenerator() { return (option_flag & OPTIONS_REGENERATOR) ? TRUE : FALSE; }

  DWORD hp_to_regenerate;

  // ブラー
  struct BLUR {
    DWORD cnum;
    int x, y;
    float z;
    int rot;
    BOOL revx, revy;
    DWORD color;
    float magx, magy;
  } m_blur[NUM_BLURS];
  int m_blur_target;
};

extern LPTSTR CharDirectory;

//行動ID
#define ACTID_ATT_SMC (ACTID_ATTACK | 0x0001)
#define ACTID_WASSYORYUKEN1 (ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0002)
#define ACTID_WASSYORYUKEN2 (ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0003)
#define ACTID_WASSYORYUKEN3 (ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0004)
#define ACTID_WASSYORYUKENF (ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0005)
#define ACTID_HADOU1 (ACTID_ATTACK | ACTID_HISSATU | 0x0006)
#define ACTID_HADOU2 (ACTID_ATTACK | ACTID_HISSATU | 0x0007)
#define ACTID_HADOU3 (ACTID_ATTACK | ACTID_HISSATU | 0x0008)
#define ACTID_DASHF (0x0009)
#define ACTID_DASHB (ACTID_KUCYU | 0x000A)
#define ACTID_SYARA1 (ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x000B)
#define ACTID_SYARA2 (ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x000C)
#define ACTID_SYARA3 (ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x000D)
#define ACTID_SYARAF (ACTID_ATTACK | ACTID_HISSATU | 0x000E)
#define ACTID_FESTIVAL (ACTID_ATTACK | ACTID_HISSATU | 0x000F)
#define ACTID_FESTIVALDASH (ACTID_ATTACK | ACTID_HISSATU | 0x0010)
#define ACTID_FESTIVALSUKI (ACTID_ATTACK | ACTID_HISSATU | 0x0011)
#define ACTID_FESTIVALRUSH (ACTID_ATTACK | ACTID_HISSATU | 0x0012)
#define ACTID_FESTIVALFINISH (ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0013)
#define ACTID_GCANCEL1 (ACTID_ATTACK | ACTID_HISSATU | 0x0014)
#define ACTID_GCANCEL2 (ACTID_ATTACK | ACTID_HISSATU | 0x0015)
#define ACTID_GCANCEL3 (ACTID_ATTACK | ACTID_HISSATU | 0x0016)
#define ACTID_NAGE1 (ACTID_ATTACK | ACTID_NAGE | 0x0017)
#define ACTID_OKIAGARI_FOWORD (ACTID_SYSTEM | 0x0018)
#define ACTID_OKIAGARI_BACK (ACTID_SYSTEM | 0x0019)
#define ACTID_STRIKER1 (ACTID_INOUT | 0x001A)
#define ACTID_STRIKER2 (ACTID_INOUT | 0x001B)
#define ACTID_STRIKER3 (ACTID_INOUT | 0x001C)
#define ACTID_STRIKER4 (ACTID_INOUT | 0x001D)

//攻撃力番号
#define ATTACKINFO_STAND_A 0
#define ATTACKINFO_STAND_B 1
#define ATTACKINFO_STAND_C 2
#define ATTACKINFO_CROUCH_A 3
#define ATTACKINFO_CROUCH_B 4
#define ATTACKINFO_CROUCH_C 5
#define ATTACKINFO_JAMP_A 6
#define ATTACKINFO_JAMP_B 7
#define ATTACKINFO_JAMP_C 8
#define ATTACKINFO_STAND_MB 9
#define ATTACKINFO_DIVE1 10
#define ATTACKINFO_DIVE2 11
#define ATTACKINFO_DIVE3 12
#define ATTACKINFO_SYARA1 13
#define ATTACKINFO_SYARA2 14
#define ATTACKINFO_SYARA3 15
#define ATTACKINFO_FESTA_D 16
#define ATTACKINFO_FESTA 17
#define ATTACKINFO_FESTA_F 18
#define ATTACKINFO_NAGE 19

//攻撃ID（必要があれば変更する）
#define ATTACK_WEAK 1
#define ATTACK_MIDDLE 2
#define ATTACK_STRONG 3
#define ATTACK_HISSATU 4
#define ATTACK_CYOHI 5
#define ATTACK_SCDASH 6

//ユーザー定義メッセージ
#define GOBJMSG_NAGE (GOBJMSG_USERDEFINE | 0x0001)              //投げ用
#define GOBJMSG_FESTIVAL_NORI_HIT (GOBJMSG_USERDEFINE | 0x0002) //海苔ヒット

//間合い
#define MAAI_SHORT (50)
#define MAAI_MIDDLE (120)
#define MAAI_LONG (220)
#define MAAI_NAGE (20)

// waveナンバー
#define wave_tojyo 4
#define wave_win 5
#define wave_nage 6
#define wave_dive 7
#define wave_nori 8
#define wave_syara 9
#define wave_fdash 10
#define wave_frush 11
#define wave_ffinish 12

// Chain
#define CHAIN_SMC 0x00000001

//光あれ
#define WLIGHT_NUM 16
class CWassoiLight : public CBulletBase {
public:
  CWassoiLight(CCharacterBase *parent);
  void SetTarget(DWORD oid) { tgt_oid = oid; }

protected:
  void act_run();
  void act_bomb();
  DWORD Draw();

  DWORD tgt_oid;

  struct CWassoiLightInfo {
    BOOL isCreated;
    UINT counter;
    float x, y;
    float sca;

    void Start(float tgt_x, float tgt_y);
    void Update();
    void Draw(LPDIRECT3DDEVICE8 dev);
  };

  CWassoiLightInfo info[WLIGHT_NUM];
};
