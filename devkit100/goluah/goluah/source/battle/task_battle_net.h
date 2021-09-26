#pragma once

#include "define_const.h"
#include "battletaskbase.h"
#include "gauge.h"
#include "efctlist.h"

#define MAXEFCT_SIVER 16
#define DEBUGMSGBUFFERSIZE (1024 * 4)
#define OBJECTS_MEMINCRATE 32 //!< オブジェクト配列、メモリ増分値

#include <PshPack1.h>
//! ネットに送信するメッセージのベース
typedef struct NetMessage {
  BYTE msgid;
  operator BYTE *() { return &msgid; }
} NET_MESSAGE;

//! 10フレームごと、ACTID更新時などの同期メッセージ
#define GNETMSG_TESTSYNC 0
struct TestSyncMes : NetMessage {
  DWORD id;
  float x, y;
  DWORD aid, /*now, */ counter;
  bool muki;
};

//! HP・ゲージ同期(要改良）
#define GNETMSG_SYNC2 1
struct Sync2Mes : NetMessage {
  BYTE pid;
  int hp;
  float gauge;
};

//!フレームが１進んだ通知
#define GNETMSG_ACTION 2
struct ActionMes : NetMessage {
  bool isStop; // ストップ中かどうか
};

//!攻撃を喰らった通知
#define GNETMSG_HIT 3
struct HitMes : NetMessage {
  DWORD oid /*, aid*/;
  BOOL hit;
};

//!キー入力通知
#define GNETMSG_INPUT 4
struct InputMes : NetMessage {
  DWORD key;
};

//!コマンド技発動通知
#define GNETMSG_COMMAND 5
struct CommandMes : NetMessage {
  DWORD aid;
};

//! HP同期
#define GNETMSG_SYNCHP 6
struct SyncHPMes : NetMessage {
  BYTE pid;
  int hp;
};
#include <PopPack.h> // ネットメッセージ終わり

//!フレームが２進んだ通知
#define GNETMSG_ACTION2 7
typedef struct ActionMes Action2Mes;

/*!
 *	@brief ネット対戦版戦闘タスク
 *	@ingroup Battle
 *	@sa CBattleTask
 *	@sa CDirectPlay
 *
 *	ネット対戦に使う予定のタスククラス(読みにくい…)です
 *	実装されている部分はCBattleTaskと似た部分が多いですが、送信命令(SendMsg)の位置がばらばらなのを何とかしたい…
 *
 *	暫定措置でKキーで自分のHPを回復できます
 */
class CBattleTaskNet : public CBattleTaskBase {
public:
  CBattleTaskNet(void);
  virtual ~CBattleTaskNet(void);
  virtual DWORD GetID() { return 'BtlN'; }

  //タスク関数
  virtual void Initialize();
  virtual BOOL Execute(DWORD time);
  virtual void Draw();
  virtual void WndMessage(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam); //!< F7でポーズ
  static HRESULT CALLBACK DPlayCallBack(PVOID UserCont, DWORD mtype, PVOID pmes);
  HRESULT DPlayMessage(PVOID UserCont, DWORD mtype, PVOID pmes);

  //情報取得系
  virtual double GetDisplayCenterX() { return disp_center_x; }
  virtual double GetDisplayCenterY() { return disp_center_y; }
  virtual BOOL IsNetwork() { return TRUE; }
  virtual BOOL IsLocal(BYTE dllid) {
    return g_play.IsHost() /*g_battleinfo.GetKeyAssign((dllid-1)/MAXNUM_TEAM, (dllid-1)%MAXNUM_TEAM) == 0*/;
  } // 暫定

  // void SetKatiSerif(DWORD tid,char *serif);
  virtual DWORD MessageFromObject(DWORD oid, DWORD msg, DWORD prm);
  virtual BOOL CatchObject(DWORD eoid, LPVOID cy);
  virtual void AddEffect(DWORD efctid, int prm1, int prm2, int prm3 = 0);
  virtual BOOL Atari(DWORD a_id, DWORD k_id, MY2DVECTOR &kas_point);
  virtual void HitStop(DWORD len, DWORD oid);

  virtual void SetTransform(BOOL b);

protected:
  // Initializeからコール
  virtual void InitializeObjectList();
  virtual void InitializeSubTasks();
  virtual void InitializeParameters();

  // Terminateからコール
  virtual void TerminateObjectList();
  virtual void TerminateDestroySubTasks();

  // Execute部分処理
  virtual void T_Command();         //!< COMMAND メッセージを送信する
  virtual void T_Action(BOOL stop); //!< Action メッセージを送信する
  virtual void T_Sousai();          //!< 飛道具相殺用 矩形あたり判定処理
  virtual void T_AtariHantei();     //!< 攻撃用　　　 矩形あたり判定処理
  virtual void T_KasanariHantei();  //!< 重なり判定用 矩形あたり判定処理
                                    //	virtual void T_Draw();				//!< DRAW メッセージを送信する
  virtual void T_ChangeTarget();    //!< オブジェクトターゲット変更処理
  virtual void T_UpdateStatus();    //!< 試合の状態更新

  virtual void StartRound();
  virtual void UpdateKeyInputDirections();

protected:
  //全体エフェクト関連
  int efct_slowdown, efct_stop, efct_darkbg, efct_nobg;
  int efct_sindo, efct_sindom;
  int efct_hitstop;
  int efct_fadein;
  int efct_flash;

  //状態
  DWORD hprecratio[2][MAXNUM_TEAM]; //!<自由交代制のときの体力回復率
  BOOL battle_end;                  //!<タスクをヌけるのに使用
  int actcount;                     //!<あと何回Actionの実行が許可されたか
  bool hoststop;                    //!<ホストが停止中

  //!デバッグテキスト表示用バッファ
  char *debugmsgbuff;

  //!表示中心
  double disp_center_x, disp_center_y;

  //クラス
  CGauge *cp_gauge;
  CEffectList *cp_efctlist;
  DWORD score[2]; //!< スコレ

  //関数を分割したのでローカルじゃいけなくなったもの
  BOOL act_stop;

  /*■オブジェクト管理■-----------------------------------------------------*/
public:
  CGObject *GetGObject(DWORD id); //!< 指定IDオブジェクト取得
  DWORD CreateGObject();          //!< オブジェクト生成
  DWORD CreateGObjectFx();        //!< オブジェクト生成(エフェクト)
  void DeleteGObject(DWORD oid);  //!< オブジェクト破棄
  void SuicideGObject(DWORD oid); //!< オブジェクト破棄予約
  CGObject *GetCharacterObject(DWORD j, DWORD i); //!< キャラクターのオブジェクト取得（ゲージ等で必要）
protected:
  std::deque<CGObject *> p_objects;           //!< オブジェクト操作クラスのポインタ
  DWORD object_regindex[MAXNUM_TEAM * 2 + 2]; //!< 次に生成するオブジェクトのインデックス
  std::deque<WORD> object_regno; //!< そのインデックスでいくつのオブジェクトが生成されてきたか
  std::deque<DWORD> suicide_list; //!< 消滅オブジェクトリスト
};

#define BATTLETASK_FXOBJFLAG 0x80000000
#define BATTLETASK_ISNOTFXOBJ(p) ((p->id & BATTLETASK_FXOBJFLAG) ? FALSE : TRUE)
