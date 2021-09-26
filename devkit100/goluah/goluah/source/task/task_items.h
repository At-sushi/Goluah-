/*=======================================================================================

        表示系ベース

        画面上表示物ベースクラス

=========================================================================================*/
#pragma once

#include "task.h"
#include "define_gcd.h"
#include "define_const.h"

class CTBeltBase;     //帯状ウインドウ
class CTWindowBase;   //単純四角形ウインドウ
class CTIconRingBase; //アイコンリング表示&選択

/*!
 *	@ingroup Tasks
 */
/*@{*/

/*!
 *	@brief オビ状描画物ベースクラス
 */
class CTBeltBase : public CTaskBase {
public:
  CTBeltBase();
  virtual ~CTBeltBase() {}

  virtual void Draw();

  //描画プライオリティ
  int m_draw_priority;
  virtual int GetDrawPriority() { return m_draw_priority; }
  virtual void SetPriority(int pri) { m_draw_priority = pri; }

  //各種操作
  void SetLeft() { m_pos = 0; }
  void SetRight() { m_pos = 1; }
  void SetCenter() { m_pos = 2; }
  void SetBaseY(float y) { m_base_y = y; }
  void SetText(char *str) { strcpy(m_disp_str, str); }
  void FormatText(char *str_format, char *str) { sprintf(m_disp_str, str_format, str); }
  void SetAlpha1(BYTE a) { m_alpha1 = a; }
  void SetAlpha2(BYTE a) { m_alpha2 = a; }

protected:
  BYTE m_pos; //!< x位置 0:右 1:左 2:中央 （テキスト・α抜き に影響）

  char m_disp_str[128];  //!< 表示テキスト　（誰）
  char m_disp_str2[128]; //!< 表示テキスト　（フォルダ）
  BOOL m_show_text;      //!< テキスト on/off
  BOOL m_txtTop;         //!< テキストを上に配置するか、下に配置するか
  float m_txtLeft;       //!< テキストの左位置
  BOOL m_txtR2L;         //!< テキスト右→左描画

  BYTE m_alpha1, m_alpha2;    //!< α、濃いとこ・薄いとこ
  DWORD m_lineCol;            //!< ラインのカラー（α以外）
  DWORD m_bodyCol;            //!< 本体のカラー（α以外）
  DWORD m_txtCol1, m_txtCol2; //!< テキストカラー（α含）

  float m_lineWidth; //!< 色つきライン太さ

  // y 位置直指定用
  float m_top, m_bottom;

  // y 位置間接指定
  float m_base_y;       //!< y位置
  float m_height_base;  //!< 平常時高さ
  float m_ratio;        //!< 高さ率
  void CalcTopBottom(); //!< m_top , m_bottom 算出(描画前にコールすること)
};

/*!
 *	@brief 単純四角形ウインドウ状描画物ベースクラス
 */
class CTWindowBase : public CTaskBase {
public:
  virtual ~CTWindowBase() {}
  virtual void Initialize();
  virtual BOOL Execute(DWORD time);
  virtual void Draw();

  //描画プライオリティ
  int m_draw_priority;
  virtual int GetDrawPriority() { return (m_ratio < 0.01f) ? -1 : m_draw_priority; }
  virtual void SetPriority(int pri) { m_draw_priority = pri; }

  //操作
  virtual void Open();
  virtual void Close();
  virtual void SetPos(float x, float y) {
    m_winleft = x;
    m_wintop = y;
  }

protected:
  //ウインドウパラメータ
  float m_ratio;        //ウインドウ大きさ
  float m_winwidth;     //ウインドウの幅（標準時）
  float m_winheight;    //ウインドウの高さ（標準時）
  float m_winleft;      //ウインドウ位置
  float m_wintop;       //ウインドウ位置
  DWORD m_wincolor;     //描画に使う頂点色
  float m_winOpenTime;  //ウィンドウオープンアニメーション速度
  float m_winCloseTime; //ウィンドウクローズアニメーション速度

  //テキスト描画。ウインドウのサイズに合わせて描画してくださる。
  //引数はCSystemのDrawBMPTextと一緒だけど、位置はウインドウ内での位置
  void DrawText(float x,
                float y,
                float z,
                char *str,
                DWORD col,
                float scaX = 1.0f,
                float scaY = 1.0f,
                DWORD flags = SYSBMPTXT_PROP);

  //描画時に実際に使う値。サブクラスではCalcDrawParametersをコールしてからDrawすること。
  virtual void CalcDrawParameters();
  float m_drx1, m_drx2, m_dry1, m_dry2;

private:
  //状態
  enum CTWindowBaseState { state_restore, state_show, state_hide };
  CTWindowBaseState m_state;
  UINT m_counter;
};

/*!
 *	@brief リング状アイコン表示&選択 ベース
 */
class CTIconRingBase : public CTaskBase {
public:
  virtual ~CTIconRingBase() {}
  void Initialize();
  void Terminate();
  BOOL Execute(DWORD time);
  void Draw();

  //状態操作
  virtual void Hide();
  virtual void Restore();

  //パラメータ操作
  virtual void SetHeight(float h) { m_height = h; }
  virtual void SetPos(double basePosX, double basePosY) {
    m_basePosX = basePosX / 240.0;
    m_basePosY = basePosY / 240.0;
  }
  virtual void SetKeyInputIndex(int i);
  virtual int GetKeyInputIndex() { return m_keyindex; }
  virtual UINT GetRing() { return m_selected_ring; }

  //描画プライオリティ
  virtual void SetPriority(int pri) { m_draw_priority = pri; }
  virtual int GetDrawPriority() { return m_state == CTCRS_HideComplete ? -1 : m_draw_priority; }
  int m_draw_priority;

  //オーバーライドするべき関数
protected:
  virtual void InitializeSub() {} //!< Initializeからコールされる
  virtual void TerminateSub() {}  //!< Terminate からコールされる
public:
  virtual UINT GetSelected() = 0; //!< リング番号+インデックス から、全体でのインデックスを取得する
protected:
  virtual void InitializeIcons() = 0;                  //!< 表示するアイコン画像を準備する
  virtual void DestroyIcons() = 0;                     //!< 表示するアイコン画像を破棄する
  virtual MYSURFACE *GetIcon(UINT ridx, UINT idx) = 0; //!< 表示するアイコン画像を取得（Drawからコール）
  virtual DWORD GetColor(UINT ridx,
                         UINT idx) = 0; //!< アイコン描画時の頂点色を指定する(Drawからコール,αは0のこと)
  virtual void OnButtonDown(DWORD key) = 0; //!< 決定（Aボタン）が押されたときの処理
  virtual void OnChangeIndex() = 0; //!< リングがまわされてインデックスが変化したときの処理
  virtual void OnChangeRing() = 0;         //!< リングが変更されたときの処理
  virtual UINT GetMaxRing() = 0;           //!< リングの総数を取得する
  virtual UINT GetMaxIndex(UINT ridx) = 0; //!< 指定リングが持っているアイコンの数を取得

  //状態とUpdate関数
  enum CTIconRingState {
    CTCRS_NotReady,     //!< 出現中
    CTCRS_Ready,        //!< 選択可能
    CTCRS_RotateRight,  //!< 右回転中
    CTCRS_RotateLeft,   //!< 左回転中
    CTCRS_ChangeUp,     //!< リング上変更中
    CTCRS_ChangeDown,   //!< リング下変更中
    CTCRS_Hide,         //!< 消失中
    CTCRS_HideComplete, //!< 完全に消失
    CTCRS_Restore,      //!< 復帰途中
  };
  virtual void Exec_NotReady();
  virtual void Exec_Ready();
  virtual void Exec_RotateRight();
  virtual void Exec_RotateLeft();
  virtual void Exec_ChangeUp();
  virtual void Exec_ChangeDown();
  virtual void Exec_Hide();
  virtual void Exec_Restore();

  //変数
  int m_keyindex;          //!< キー入力インデックス。マイナスで選択をロック
  UINT m_sleepCount;       //!< リング変更・復帰後の何フレかはキー操作をロックする
  CTIconRingState m_state; //!< 状態
  DWORD m_selected_ring;   //!< 現在選択されているリング
  DWORD m_selected_index;  //!< 現在選択されているリング中のインデックス
  DWORD m_previous_ring; //!< 前回選択されていたリング（変更時の消失アニメーションに必要）
  DWORD m_previous_index; //!< 前回選択されていたリング中のインデックス

  double m_basePosX, m_basePosY; //!< 基準位置
  float m_height;                //!< 高さ

  DWORD m_counter;        //!< アニメーション用カウンタ
  double m_rotation2;     //!< 追加回転（degree）
  double m_radius_base;   //!< 基準半径
  double m_radius_ratio;  //!< 半径倍率
  double m_radius_ratio2; //!< 半径倍率（消えていくリング）
  BYTE m_alpha;           //!< α値
  BYTE m_alpha2;          //!< α値（消えていくリング）
};

/*@}*/
