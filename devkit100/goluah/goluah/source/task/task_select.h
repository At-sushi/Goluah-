

/*=======================================================================================

        vsCOM または 1Pvs2P でのキャラセレ

=========================================================================================*/
#pragma once

#include "define_const.h"
#include "task_items.h"
#include "charlist.h"
#include "task_optselecter.h"

class CTCharacterRing;
class CTCharacterBigFace;
class CTCharacterSelectBG;
class CTCharacterSelectBelt;
class CTOptionSelecter;
class CTSimpleOptionSelecter;
class CTStageSelecter;
class CTConditionSelecter;

/*!
 *	@brief キャラセレクラス・ベース
 *	@ingroup Tasks
 *
 *	キャラクターリングから利用する共通インターフェイスを定義したりしたかったみたい。
 */
class CCharacterSelectBase : public CExclusiveTaskBase {
  friend class CTCharacterRing;

public:
  virtual ~CCharacterSelectBase() {}
  virtual void OnSelect(CTCharacterRing *pring,
                        int cindex) = 0; //!< キャラクタ選択時処理。マイナスはランダム扱い
  virtual void OnChangeColor(CTCharacterRing *pring) = 0;                     //!< 色変更時処理
  virtual void OnInstOnOff(CTCharacterRing *pring) = 0;                       //!< インストON/OFF時処理
  virtual void OnOptionSelect(CTOptionSelecter *pselecter, DWORD option) = 0; //!< オプション選択時処理
  virtual void OnStageSelect(CTStageSelecter *spelecter, int sindex) = 0;     //!< ステージ選択時処理
  virtual void OnEndAnimComplete(CTaskBase *ptask) = 0; //!< 終了アニメーション完了を通知
  virtual void OnCancel() = 0;
};

/*!
 *	@brief キャラセレクラス・対戦用
 *	@ingroup Tasks
 */
class CCharacterSelect : public CCharacterSelectBase {
public:
  CCharacterSelect();
  virtual ~CCharacterSelect() { Terminate(); }

  void Initialize();
  BOOL Execute(DWORD time);
  void Terminate();
  void Draw();
  int GetDrawPriority() { return 100; }

  //キャラクターリングからの処理
  void OnSelect(CTCharacterRing *pring, int cindex);              //!< キャラクタ選択時処理
  void OnChangeColor(CTCharacterRing *pring);                     //!< 色変更時処理
  void OnInstOnOff(CTCharacterRing *pring);                       //!< インストON/OFF時処理
  void OnOptionSelect(CTOptionSelecter *pselecter, DWORD option); //!< オプション選択時処理
  void OnStageSelect(CTStageSelecter *spelecter, int sindex);     //!< ステージ選択時処理
  void OnEndAnimComplete(CTaskBase *ptask); //!< 終了アニメーション完了を通知
  void OnCancel();

  //! ConditionSelecterからの処理
  virtual void OnConditionDecided(CTConditionSelecter *pcsel);

protected:
  void ResolveColor(int team, int index); //!< 色の重複を解決する
  void ResolveRandom();                   //!< 重複を避けてランダム指定を解決する
  void SetupBattleInfo();                 //!< g_battleinfoに選択された項目を設定する
  //!キャラクター選択・オプション選択イベント後にステージ選択に進むかどうかのチェック
  void CheckCharacterSelectOK();

  //!継承クラス用
  virtual void CreateNextTask();

  //変数
protected:
  //選択された情報
  BYTE m_assign[2][MAXNUM_TEAM];         //!< プレイヤ割り当て
  int selected_char[2][MAXNUM_TEAM];     //!< キャラクタ
  int selected_color[2][MAXNUM_TEAM];    //!< キャラクタ・色
  DWORD selected_option[2][MAXNUM_TEAM]; //!< キャラクタ・オプション
  int selected_stage;                    //!< ステージ

  //進行フラグ等
  BOOL m_condition_sel_ok; //!< プレイヤ割り当ては終わったか？
  int wanted_char_num[2];  //!< 何人選び出せばいいのか？
  BOOL charsel_ok[2];      //!< 必要人数選び出されたか？
  BOOL stgsel_ok;          //!< ステージは既に選択されたか？
  int num_selected[2];     //!< 何人選んだか？

  // sub tasks
  CTCharacterSelectBG *m_bg;             //!< 背景
  CTCharacterRing *m_ring[2];            //!< リング
  CTCharacterBigFace *m_bface[2];        //!< デカface
  CTCharacterSelectBelt *m_belt[2];      //!< オビ
  CTSimpleOptionSelecter *m_selecter[2]; //!< オプションセレクタ

  //描画プライオリティ設定
  static int m_pri_background;
  static int m_pri_face;
  static int m_pri_belt;
  static int m_pri_cselecter;
  static int m_pri_oselecter;
  static int m_pri_sselecter;
  static int m_pri_firstSetting;

  //!キャラ選択キー入力割り当て
  void AssignKeys(int team);
  BOOL CkKey(int team, BYTE ki); //!< チェック
  BOOL CkAllCom(int team);       //!< チェック

  //!敵チーム取得
  int AnotherTeam(int team) { return team == 0 ? 1 : 0; }
};

/*!
 *	@brief VSモード時キャラクター選択リングクラス
 *	@ingroup Tasks
 */
class CTCharacterRing : public CTIconRingBase {
public:
  virtual ~CTCharacterRing() {}

  void AddSelected(DWORD sel) //!<選択済みキャラクターリスト追加
  {
    m_selectedlist.push_back(sel);
  }

  //純粋仮想関数の実装
protected:
  void InitializeSub(); //!< Initializeからコールされる
public:
  UINT GetSelected(); //!<リング番号+インデックス から、全体でのインデックスを取得する
protected:
  virtual void InitializeIcons();                  //!<表示するアイコン画像を準備する
  virtual void DestroyIcons();                     //!<表示するアイコン画像を破棄する
  virtual MYSURFACE *GetIcon(UINT ridx, UINT idx); //!<表示するアイコン画像を取得（Drawからコール）
  virtual DWORD GetColor(UINT ridx, UINT idx); //!<アイコン描画時の頂点色を指定する(Drawからコール,αは0のこと)
  virtual void OnButtonDown(DWORD key); //!<決定（Aボタン）が押されたときの処理
  virtual void OnChangeIndex(); //!<リングがまわされてインデックスが変化したときの処理
  virtual void OnChangeRing();  //!<リングが変更されたときの処理
  virtual UINT GetMaxRing();    //!<リングの総数を取得する
  virtual UINT GetMaxIndex(UINT ridx); //!<指定リングが持っているアイコンの数を取得
  void Draw();                         //!<描画

  //追加されたメンバ
  BOOL IsSelected(DWORD cindex); //!<指定キャラクターが既に選択されているかどうかチェック（色Get用）

  static MYSURFACE **m_dds_miniface; //!<キャラクタミニ顔イメージ
  static DWORD m_instanceCount;      //!<↑を複数のリングで共有するための参照カウント
  std::list<DWORD> m_selectedlist;   //!<選択済みキャラクターのリスト

private:
  static UINT sm_selected_ring1p;
  static UINT sm_selected_index1p;
  static UINT sm_selected_ring2p;
  static UINT sm_selected_index2p;
};

class CTCharacterSelectBelt2;
/*!
 *	@brief VSモード選択時　オプションセレクター・タスク
 *	@ingroup Tasks
 */
class CTOptionSelecter : public CTOptionSelecterBase {
public:
  virtual void ApplyToPreviousSelect() { m_selecter->ApplyToPreviousSelect(); }

protected:
  virtual void EndSelect();
};

/*!
 *	@brief VSモード選択時　キャラクター・デカフェイス表示
 *	@ingroup Tasks
 */
class CTCharacterBigFace : public CTaskBase {
public:
  virtual ~CTCharacterBigFace() {}
  virtual void Initialize();
  virtual void Terminate();
  virtual BOOL Execute(DWORD time);
  virtual void Draw();

  virtual int GetDrawPriority() { return m_draw_priority; }
  virtual void SetPriority(int pri) { m_draw_priority = pri; }

  void SetLeftRight(BOOL left) { m_left = left; }
  void SetNum(DWORD num) { m_max_num = num; }
  void Set(int cid, DWORD color, int alt);
  void SetTemporary(int cid, DWORD color, int alt);
  void ResetNum() { m_selected_num = 0; }

protected:
  int m_draw_priority; //!<描画プライオリティ
  BOOL m_left;         //!<左側か、右側か？
  UINT m_counter;      //!<奥に押しやるのに必要
  UINT m_counter2;     //!<選択が変化したときに必要
  UINT m_selected_num; //!<選択済みキャラの数
  DWORD m_max_num;     //!<最大人数

  int m_cindex[MAXNUM_TEAM]; //!<現在読み込まれているものが、どのキャラクターのものか
  DWORD m_color[MAXNUM_TEAM]; //!<現在読み込まれているものが、どの色のものか
  int m_alt[MAXNUM_TEAM];     //!<オプションによる変更

  //ストーリー選択のほうで継承したとき用。
  virtual DWORD GetDispColor(UINT idx); //!<描画色を取得する
  BOOL m_draw_temporary;                //!<テンポラリ選択キャラクタを描画しない
};

/*!
 *	@brief VSモード選択時の背景描画
 *	@ingroup Tasks
 */
class CTCharacterSelectBG : public CTaskBase {
public:
  virtual ~CTCharacterSelectBG() {}
  void Initialize();
  void Terminate();
  BOOL Execute(DWORD time);
  void Draw();
  int GetDrawPriority() { return m_draw_priority; }

  void SetPriority(int pri) { m_draw_priority = pri; }
  void ChangeState(BOOL f);

protected:
  int m_draw_priority;         //!<描画プライオリティ
  BOOL m_state;                //!< TRUE:キャラセレ色 / FALSE:ステージセレクト色
  DWORD m_counter;             //!<カウンタ
  MYVERTEX3D vbg[4];           //!<頂点
  LPDIRECT3DTEXTURE8 ptex_cs1; //!<背景用テクスチャ
};

/*---------------------------------------------------
        オビ
-----------------------------------------------------*/

/*!
 *	@brief CTCharacterSelectBeltの状態
 *	@ingroup Tasks
 */
enum CTCharacterSelectBeltState {
  CTCSBS_NotReady,
  CTCSBS_Ready,
  CTCSBS_Change,
  CTCSBS_Change2,
  CTCSBS_Hide,
  CTCSBS_Show,
  CTCSBS_HideComplete,
  CTCSBS_Extend,  //拡張前伸縮
  CTCSBS_Extend2, //拡張中
  CTCSBS_Extended,
  CTCSBS_ExtendedHide, //拡張中からHideへ
  CTCSBS_Restore,      //拡張中から通常動作へ
};

/*!
 *	@brief 帯状の描画物・ベース
 *	@ingroup Tasks
 *
 *	キャラ選択とかで、帯状の描画物がいくつもあったのでなんとなく作ってみた。
 *	あんまり効果的に使われてないかもしれない。
 */
class CTCharacterSelectBelt : public CTBeltBase {
public:
  virtual ~CTCharacterSelectBelt() {}
  virtual void Initialize();
  virtual void Terminate();
  virtual BOOL Execute(DWORD time);
  // void Draw();

  virtual int GetDrawPriority() { return m_state == CTCSBS_HideComplete ? -1 : m_draw_priority; }

  //操作・設定
  virtual void SetRing(DWORD ringIndex);
  virtual void SetCom(BOOL com);
  virtual void SetKey(char idx);

  virtual void Show(BOOL b);
  virtual void Extend(float top, float bottom); //!<領域拡大
  virtual void Restore();                       //!<通常の状態に戻す

protected:
  virtual void UpdateText();

  CTCharacterSelectBeltState m_state;
  DWORD m_ringIndex;  //!<リング名表示用
  BOOL m_com;         //!<コンピュータかどうか
  char m_current_key; //!<現在有効なキー入力番号

  float m_given_top, m_given_bottom;
};

/*---------------------------------------------------
        ステージセレクター・タスク
-----------------------------------------------------*/

/*!
 *	@brief CTStageSelecter 状態
 *	@ingroup Tasks
 */
enum CTStageSelecterState {
  CTSSS_NotReady,
  CTSSS_Ready,
  CTSSS_Change,       //左右変更時アニメ
  CTSSS_Change2,      //上下変更時アニメ
  CTSSS_Hide,         //消失時アニメ
  CTSSS_HideComplete, //終了
};

/*!
 *	@brief VSモード時　ステージ選択
 *	@ingroup Tasks
 */
class CTStageSelecter : public CTBeltBase {
public:
  void Initialize();
  void Terminate();
  BOOL Execute(DWORD time);
  void Draw();

  //描画プライオリティ
  int GetDrawPriority() { return m_state == CTCSBS_HideComplete ? -1 : m_draw_priority; }

  static DWORD m_previous_selected_ring;
  static DWORD m_previous_selected_index;

  //操作
  void AddKeyInput(DWORD ki) { m_keyIndexList.push_back(ki); }

protected:
  void LoadBMP();
  int GetSelection();
  void HandlePad();
  void UpdateText();

  CTStageSelecterState m_state;
  DWORD m_counter;
  DWORD m_selected_ring, m_selected_index;
  std::vector<DWORD> m_keyIndexList; //!<どのキー入力を参照するか

  MYSURFACE *m_dds;

  //描画時パラメータ
  float m_width, m_height;
};

/*!
 *	@brief VSモード時　対戦コンディション選択タスク
 *	@ingroup Tasks
 */
class CTConditionSelecter : public CTBeltBase {
public:
  friend class CCharacterSelectBase;
  friend class CCharacterSelect;

  enum CTConditionSelecter_State {
    CTCoS_Start,        //出現中
    CTCoS_Execute,      //実行中
    CTCoS_Hide,         //消失中
    CTCoS_HideComplete, //消失完了
    CTCoS_Selected,     //選択中
  };

public:
  virtual ~CTConditionSelecter() {}
  virtual void Initialize();
  virtual void Terminate();
  virtual BOOL Execute(DWORD time);
  virtual void Draw();

  //描画プライオリティ
  int GetDrawPriority() { return m_state == CTCoS_HideComplete ? -1 : m_draw_priority; }

protected:
  virtual DWORD TxtCol(int idx) {
    if (1 < idx && idx <= 1 + MAXNUM_TEAM)
      return idx == m_selected ? (m_state == CTCoS_Selected ? 0x99220066 : 0xFF220099) : 0xFF5237FF;
    else if (1 + MAXNUM_TEAM < idx && idx <= 1 + MAXNUM_TEAM * 2)
      return idx == m_selected ? (m_state == CTCoS_Selected ? 0x99660022 : 0xFF990022) : 0xFFFF3752;
    else
      return idx == m_selected ? (m_state == CTCoS_Selected ? 0x99000066 : 0xFF000000) : 0xFF777777;
  }
  virtual void Change(BOOL key);

  CTConditionSelecter_State m_state;
  DWORD m_counter;
  int m_selected;

  static BYTE m_type;                   // gobject.h , TAISENKEISIKI_～参照 BYTEで足りる
  static BYTE m_assign[2][MAXNUM_TEAM]; //割り当て
  static int m_limit_time_index;        //↓のインデックス
  static int m_limit_time[5];           // 45,60,99,120,∞  ,  const

  BOOL m_ok;
  char m_str_error[128];

  void Check(); //!< 割り当ての正当性をチェックする
};
