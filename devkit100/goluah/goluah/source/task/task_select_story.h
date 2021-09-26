
/*=======================================================================================

        ストーリーセレクト

=========================================================================================*/
#pragma once

#include "define_const.h"
#include "task_items.h"
#include "storylist.h"
#include "charlist.h"
#include "task_select.h"
#include "task_simpleoptsel.h"

class CTStorySelect;        //選択クラス本体
class CTStorySelecterRing;  //リング状ストーリー選択クラス
class CTStorySelectPreview; //ストーリープレビュークラス
class CTStoryBigFace;       //ストーリー用デカ顔表示クラス
class CTStorySelectBG;      //ストーリー用背景描画クラス
class CTStoryParamWindow;   //パラメータいじりウインドウ

/*!
 *	@brief ストーリーセレクト画面・本体
 *	@ingroup Tasks
 *
 *	ストーリーセレクト画面用のタスククラス。
 */
class CTStorySelect : public CExclusiveTaskBase {
public:
  void Initialize();
  BOOL Execute(DWORD time);
  void Terminate();

  //!操作時に参照するキー番号
  void SetKeyIndex(UINT ki) { m_keyindex = ki; }
  UINT m_keyindex;

  void OnSelect(CTStorySelecterRing *ring);
  void OnChange(CTStorySelecterRing *ring);
  void OnParamSelectComplete();
  void OnCancel();

  //ストーリー選択後のパラメータ変更
  void SelectChangeCharacter(UINT idx, int cindex);
  void SelectChangeColor(UINT idx, int col);
  void SelectChangeOption(UINT idx, DWORD opt);

  CStoryList::CStoryInfo *m_sinfo;

protected:
  BOOL m_first_update; //!< 初回Updateフラグ。リング・プレビューに値を設定するために使用
  BOOL m_sselected;            //!< ストーリーの選択完了フラグ
  BOOL m_complete_flag;        //!< 選択完了フラグ
  UINT m_complete_counter;     //!< 選択完了後、タスクを抜けるまでのウェイトに使用
  UINT m_selected_story_index; //!< 選択されたストーリー番号

  int m_alts[MAXNUM_TEAM];

  //オブジェクト
  CTStorySelecterRing *m_ring;    //!< リングクラス
  CTStorySelectBG *m_bg;          //!< 背景描画クラス
  CTStoryBigFace *m_face;         //!< デカ顔クラス
  CTStorySelectPreview *m_pv;     //!< プレビュー
  CTStoryParamWindow *m_paramwin; //!< パラメータウィンドウ

  //描画プライオリティ（定数）
  static int m_pri_ring;
  static int m_pri_brief;
  static int m_pri_bigface;
  static int m_pri_preview;
  static int m_pri_bg;
  static int m_pri_paramwin;
};

/*!
 *	@brief ストーリー選択リング
 *	@ingroup Tasks
 */
class CTStorySelecterRing : public CTIconRingBase {
public:
  BOOL Execute(DWORD time);
  void Hide();

  //純粋仮想関数の実装
protected:
  void InitializeSub(); //!< Initializeからコールされる
public:
  UINT GetSelected(); //!< リング番号+インデックス から、全体でのインデックスを取得する
protected:
  void InitializeIcons();                  //!< 表示するアイコン画像を準備する
  void DestroyIcons();                     //!< 表示するアイコン画像を破棄する
  MYSURFACE *GetIcon(UINT ridx, UINT idx); //!< 表示するアイコン画像を取得（Drawからコール）
  DWORD GetColor(UINT ridx, UINT idx); //!< アイコン描画時の頂点色を指定する(Drawからコール,αは0のこと)
  void OnButtonDown(DWORD key); //!< 決定（Aボタン）が押されたときの処理
  void OnChangeIndex(); //!< リングがまわされてインデックスが変化したときの処理
  void OnChangeRing();  //!< リングが変更されたときの処理
  UINT GetMaxRing();    //!< リングの総数を取得する
  UINT GetMaxIndex(UINT ridx); //!< 指定リングが持っているアイコンの数を取得

  //変数
protected:
  MYSURFACE **m_icons;

  //オビ
protected:
  class CTStorySelectBelt : public CTCharacterSelectBelt {
  public:
    void Initialize();
    void SetRing(UINT idx, BOOL real_change); //!< real_changeがFALSEなら伸縮しない
  protected:
    void UpdateText();
  };
  CTStorySelectBelt *m_belt;
};

/*!
 *	@brief ストーリー用背景描画クラス
 *	@ingroup Tasks
 *
 *	CTCharacterSelectBGから派生してるけど何にもオーバーライドとかされてないみたい。
 */
class CTStorySelectBG : public CTCharacterSelectBG {
  //そのまま
};

/*!
 *	@brief ストーリー選択時デカ顔表示クラス
 */
class CTStoryBigFace : public CTCharacterBigFace {
public:
  void Initialize();
  BOOL Execute(DWORD time);

  void Setup(int *chars, int *colors, int *alts, int num); //!< キャラ一括指定
  void ChangeCharacter(int cindex, int idx);               //!< 既存キャラ色変更
  void ChangeColor(int color, int idx);                    //!< 既存キャラ変更
  void ChangeAlt(int alt, int idx);                        //!< Alt変更

  void SetDisplayMode(BOOL dark); //!< dark=TRUE で 選択キャラ以外を暗く表示
  void SetLightIndex(int idx);    //!< dark=TRUE 時に明るく表示するキャラを指定

protected:
  BOOL m_dark;           //!< 暗くするフラグ
  int m_lighting;        //!< 暗くしたときに明るくするインデックス
  UINT m_appear_counter; //!< 出現カウンタ. Setup時にリセット
  int m_appear_cnt;      //!< 出現数

  int m_num;                     //!< 表示するべき人数
  int m_characters[MAXNUM_TEAM]; //!< 渡されたキャラクタ
  int m_colors[MAXNUM_TEAM];     //!< 渡されたカラー

  DWORD GetDispColor(UINT idx); //!< 描画時表示色指定
};

/*!
 *	@brief ストーリープレビュー表示クラス
 *	@ingroup Tasks
 */
class CTStorySelectPreview : public CTaskBase {
public:
  void Initialize();
  BOOL Execute(DWORD time);
  void Terminate();
  void Draw();

  void Change(UINT sindex); //!< 変更
  void HideText() { m_brief->Hide(); }
  void HidePreview();

  //描画プライオリティ
  int m_draw_priority;
  int m_txt_draw_priority;
  virtual int GetDrawPriority() { return m_draw_priority; }
  virtual void SetPriority(int pri, int txtpri) {
    m_draw_priority = pri;
    m_txt_draw_priority = txtpri;
  }

public:
  //変数
  UINT m_counter;    //!< 出現・変更時アニメーション用カウンタ
  int m_crnt_sindex; //!< ストーリー番号。マイナスは無効
  int m_prev_sindex; //!< ストーリー番号。マイナスは無効
  float y_pos;
  float m_height;

  //!プレビュービットマップ
  MYSURFACE *m_pv;
  MYSURFACE *m_pv_prev;

  //!描画時パラメータ
  float m_ratio;

  //!ストーリー概要テキスト描画用クラス
  class CTStoryBrief : public CTaskBase {
  public:
    void Initialize();
    BOOL Execute(DWORD time);
    void Terminate();
    void Draw();

    //描画プライオリチー
    int m_draw_priority;
    virtual int GetDrawPriority() { return m_draw_priority; }
    virtual void SetPriority(int pri) { m_draw_priority = pri; }

    void Set(UINT sindex);
    void Clear();
    void Hide(); //!<隠す

  protected:
    int disp_len;   //!<文字列表示長さ
    char *disp_txt; //!<表示文字列
    int m_sindex;   //!<ストーリー番号。マイナスは無効
    BOOL m_hideFlag;
    float m_hideMove;
  };
  CTStoryBrief *m_brief;
};

/*!
 *	@brief ストーリー・パラメータいじりウインドウ
 *	@ingroup Tasks
 *
 *	ストーリー選択後に、キャラクターとかオプションとか選ぶウインドウみたいなやつ
 */
class CTStoryParamWindow : public CTWindowBase {
public:
  void Initialize();
  BOOL Execute(DWORD time);
  void Terminate();
  void Draw();

  //!描画プライオリチー
  int m_draw_priority;
  virtual int GetDrawPriority() { return m_draw_priority; }
  virtual void SetPriority(int pri) { m_draw_priority = pri; }

  //!操作時に参照するキー番号
  void SetKeyIndex(UINT ki) { m_keyindex = ki; }
  UINT m_keyindex;

  UINT GetCurrentCharacter() //!<選択クラス本体が、デカ顔クラスの画像を明るくするときに使用
  {
    return m_crnt_cindex;
  }

public:
  UINT m_num;         //!<キャラ数
  UINT m_crnt_cindex; //!<現在キャラ
  UINT m_crnt_param;  //!<パラメタ（ 0:キャラ 1:カラー 2:オプション / 3:OKボタン）
  BOOL m_lock[MAXNUM_TEAM][3];      //!<選択が禁止されているパラメータ
  CStoryList::CStoryInfo **m_sinfo; //!<ストーリー情報（SELECTタスク本体の変数への参照）

  MYSURFACE **m_miniface;
  DWORD GetDrawColor(UINT cidx, UINT param_idx); //描画時テキストカラー取得

  BOOL m_ok;
  void OkCheck();
  void ToWaitMode();

  //状態とExec関数
  enum CTStoryParamWindowState {
    state_start,
    state_exec,
    state_wait,
    state_hide,
  };
  void Exec_Start();
  void Exec_Exec();
  void Exec_Wait();
  BOOL Exec_Hide();
  CTStoryParamWindowState m_state;

  /*!
   *	@brief ストーリー選択後・キャラ選択サブタスク
   */
  class CCharSelecter : public CTCharacterRing {
  protected:
    void InitializeIcons() {}     //!< アイコン準備、なにもしない
    void DestroyIcons() {}        //!< アイコン破棄、なにもしない
    void OnButtonDown(DWORD key); //!< ボタン押されたときの処理
    void OnChangeIndex() { Notify(); } //!< リングがまわされてインデックスが変化したときの処理
    void OnChangeRing() { Notify(); }        //!< リングが変更されたときの処理
    MYSURFACE *GetIcon(UINT ridx, UINT idx); //!< 表示するアイコン画像を取得（Drawからコール）
  public:
    MYSURFACE **m_miniface; //!< ミニ顔。本体クラスからいただく
    UINT cidx;              //!< キャラクタ

    void SetInitialValue(int chr);
    BOOL Execute(DWORD time); //!< HideComplete時、タスクヌケ
    void Terminate();         //!< 親タスク復帰のため
  protected:
    void Notify(); //!< ChangeIndex/Ring共通処理
  };
  /*!
   *	@brief ストーリー選択後・色選択サブタスク
   */
  class CColorSelecter : public CTWindowBase {
  public:
    void Initialize();
    BOOL Execute(DWORD time);
    void Terminate();
    void Draw();

    void SetInitialValue(int col);
    UINT m_keyindex; //操作時に参照するキー番号
    UINT cidx;       //キャラクタ

  protected:
    void Notify();   //変更通知
    UINT color;      //色
    BOOL m_end_flag; //終了フラグ
  };
  /*!
   *	@brief ストーリー選択後・オプション選択サブタスク
   */
  class COptionSelecter : public CTSimpleOptionSelecter {
  public:
    void Draw();
    void SetCIndex(UINT idx) { m_cindex2 = idx; }
    BOOL Execute(DWORD time);

  protected:
    void EndSelect();

    UINT m_cindex2;
    bool customend;
  };

  //!サブタスクからの終了メッセージ受け取り用
public:
  static void EndSub();

protected:
  static CTStoryParamWindow *m_instance; //上記関数中でインスタンスを取得するため
};
