
/*============================================================================

        GOBJECT 内部処理クラス
        メッセージ送信と、メッセージのデフォルト処理

==============================================================================*/
#pragma once

#include "gobject.h"
#include "config.h"
class CBattleTaskBase;

/*!
 *	@brief GOBJECT内部処理クラス
 *	@ingroup Battle
 *
 *	DLLからオブジェクトの生成要求があったときに内部ではこのクラスが作られ、
 *	このクラスのGOBJECT構造体が返される。
 *	GOBJECTを扱いやすくラップすることと、各オブジェクトメッセージのデフォルト動作を実装するのが目的。
 *	一部のデフォルト行動関数は、staticでDLL側に公開される。
 *
 *	クラスの生成時に、CBattleTaskBaseから"現在のDLL-ID"を取得する。
 *	また、DLL側の関数の呼び出し前にCBattleTaskBaseの"現在のDLL-ID"を、自分のIDに設定する。
 *	これにより、基本的にはどのオブジェクトがどのDLLで作られたかがわかる。
 *	(thisポインタごと渡しておいたほうがいろいろできていいかも)
 *
 *	同様に、クラスの生成時に CBattleTaskBaseからコンピュータレベル値(ゲーム難易度)を取得する。
 *	これにより、コンピュータレベルがキャラクターごとに設定できるようになった。
 */
class CGObject {
public:
  CGObject(DWORD object_id);
  ~CGObject();

  DWORD Message(DWORD type, DWORD prm = 0);
  DWORD MessageDirect(DWORD type, DWORD prm);
  DWORD TouchA();
  void ActionIDChanged(BOOL ck, BOOL force = FALSE);

  static bool ZCompare(CGObject *obj1, CGObject *obj2) { return obj1->data.z > obj2->data.z; }

private:
  //デフォルト動作
  void Defmsg_Draw();
  DWORD Defmsg_TouchA();
  void Defmsg_Action();

public:
  static void dact_damages1(GOBJECT *pdat);
  static void dact_damages2(GOBJECT *pdat);
  static void dact_damages3(GOBJECT *pdat);
  static void dact_damagec1(GOBJECT *pdat);
  static void dact_damagec2(GOBJECT *pdat);
  static void dact_damagec3(GOBJECT *pdat);
  static void dact_damagej1(GOBJECT *pdat);
  static void dact_damagej2(GOBJECT *pdat);
  static void dact_damagej3(GOBJECT *pdat);
  static void dact_damages1a(GOBJECT *pdat);
  static void dact_damages2a(GOBJECT *pdat);
  static void dact_damages3a(GOBJECT *pdat);
  static void dact_damagec1a(GOBJECT *pdat);
  static void dact_damagec2a(GOBJECT *pdat);
  static void dact_damagec3a(GOBJECT *pdat);
  static void dact_guards1(GOBJECT *pdat);
  static void dact_guards2(GOBJECT *pdat);
  static void dact_guards3(GOBJECT *pdat);
  static void dact_guardc1(GOBJECT *pdat);
  static void dact_guardc2(GOBJECT *pdat);
  static void dact_guardc3(GOBJECT *pdat);
  static void dact_guardj1(GOBJECT *pdat);
  static void dact_guardj2(GOBJECT *pdat);
  static void dact_guardj3(GOBJECT *pdat);

  static void dact_down(GOBJECT *pdat);
  static void dact_down2(GOBJECT *pdat);
  static void dact_okiagari(GOBJECT *pdat);
  void dact_finaldown(GOBJECT *pdat);
  static void dact_futtobi(GOBJECT *pdat);
  static void dact_bound(GOBJECT *pdat);
  static void dact_finalbound(GOBJECT *pdat);
  void dact_finaldown2(GOBJECT *pdat);
  static void dact_futtobi2(GOBJECT *pdat);
  static void dact_tatakituke1a(GOBJECT *pdat);
  static void dact_tatakituke1b(GOBJECT *pdat);
  static void dact_tatakituke2a(GOBJECT *pdat);
  static void dact_tatakituke2b(GOBJECT *pdat);
  static void dact_tatakituke(GOBJECT *pdat);
  void dact_kaitenfinish(GOBJECT *pdat);

private:
  void dact_userhit1();
  void dact_userhit2();
  void dact_userguard1();
  void dact_userguard2();

  void ComCommand();                              //!< コンピュータコマンド処理
  static void DmgMovex(GOBJECT *pdat, double mx); //!< ダメージ喰らい時位置移動
  static void Furimuki(GOBJECT *pdat);            //!< 振り向き動作

  //変数
public:
  GOBJECT data;          //!< データ
  double comguard;       //!< コンピュータガード確率
  int siver;             //!< ダメージ食らったときのしびれ（のこり持続時間）
  CATCHYOU cy;           //!< 投げられ情報
  DWORD hitcount;        //!< 連続技ヒット数
  DWORD sexydamage;      //!< 連続技蓄積ダメージ
  DWORD sexydamage_anim; //!< 連続技蓄積ダメージ（画面表示用）
  DWORD sexydamage_haba; //!< 連続技蓄積ダメージ（画面表示用の増減幅）

  DWORD userhit_attacker; //!< ユーザー定義喰らいの場合、攻撃者
  DWORD userhit_id;       //!< ユーザー定義喰らいの場合、ID

  int comreach[3];   //!< リーチ. 短中長
  DWORD comhint_aid; //!< DLLからの行動遷移命令.行動ID
  int comhint_delay; //!< DLLからの行動遷移命令.ディレイ(マイナス値は無効)

  Config2_Difficulty com_level; //!< コンピュータのレベル(難易度)
  BYTE dll_id;                  //!< DLLID
  BYTE nage_muteki_cnt;         //!< 起き上がり、ガード後投げ無敵カウンタ

  DWORD object_id_original; //!< オブジェクトID（チェック用）
  DWORD comrange;           //!< COMの基本間合い

protected:
  int comcounter;              //!< コンピュータ動作用カウンタ
  DWORD actid_prv;             //!< 前回行動ID
  CBattleTaskBase *battleTask; //!< 戦闘タスク
  int actloop_count;           //!< 行動IDのループ回数

  DWORD comhint_counterprev; //!< 前回のカウンター、比較用。
  int comhint_capacity;
};
