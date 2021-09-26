

/*!
 *	@file
 *	@brief システムクラス等の定義
 *	メインシステム。ゲーム全般の統括
 */

/*!
 *	@defgroup System
 *	@brief ゲーム全般の統括風味
 */

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "system_cell.h"

#include "define_gcd.h"
#include "task.h"

#define NUM_SYSTEMSOUND 32 // 0-31までのwavを読み込む

/*!
 *	@ingroup System
 *	@brief System::Log ログの分類
 */
enum SyatemLogCategory {
  SYSLOG_NONE,    //とくになんもなし
  SYSLOG_INFO,    //情報　・・・って、なんの？
  SYSLOG_ERROR,   //エラー
  SYSLOG_WARNING, //警告
  SYSLOG_ALERT,   //警報（同じじゃん！）
  SYSLOG_DEBUG    //デバッグ情報
};

/*!
 *	@ingroup System
 *
 *	@brief システムクラス。ゲーム全般の統括
 *
 *	実際にはあんまり仕事していない。
 *	DX回りのクラスを初期化したり、メインループでタスク管理クラスを呼ぶぐらい。
 *	あとはログ吐き出しとかの雑用系。
 */
class CSystem {
  friend class CExport; //タスクマネージャから直接戦闘タスクを抜き出すために必要

public:
  CSystem();                //!< コンストラクタ
  ~CSystem() { Destroy(); } //!< デストラクタ

  //初期化・破棄
  BOOL Initialize(HWND hwnd); //!< 初期化
  BOOL GameStart(); //!< ゲーム開始時にエントリーポイント（の方面）から呼ばれる
  void Destroy();   //!< 破棄。デストラクタの実処理

  void MainLoop(); //!< メインループから呼ばれる。定時処理
  void WndMessage(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam); //!< ウインドプロシージャから呼ばれる

  //状態取得
  BOOL IsWindowActive(); //!< アプリケーションウインドウがアクティブな状態かどうか
  BOOL IsReady() { return ready; } //!< ゲームが開始できる状態かどうか（だっけ？
  HWND GetHWnd() { return hwnd; } //!< アプリケーションウインドウのウインドウハンドルを取得

  //サービス
  void PlaySystemSound(DWORD sid);                                       //!< サウンド再生
  double DrawBMPText(double x, double y, float z, char *str, DWORD col); //!< ビットマップテキストを描画
  double DrawBMPTextEx(double x,
                       double y,
                       float z,
                       char *str, //!< ビットマップテキストを描画(拡縮・プロポーショナルオプションつき)
                       DWORD col,
                       float scaX,
                       float scaY,
                       DWORD flags);
  void ReturnTitle();                                   //!< タイトル画面へ戻る
  void DisableRendering() { m_render_disabled = TRUE; } //!< タスク管理で描画をしない（動画再生用）
  void EnableRendering() { m_render_disabled = FALSE; } //!< DisableRendering復帰

  // fps
  DWORD GetFPS() { return m_fps; } //!< 実際フレームレート取得
  DWORD GetMaxFPS() {
    return (DWORD)(1000.0 / m_loopTime);
  } //!< 最大フレームレート（処理落ちしていない状態で出る）取得
  void SetMaxFPS(float newFPS) { m_loopTime = (DWORD)(1000.0 / newFPS); } //!< フレームレートリミット設定
  void UpdateFrameRate();                                                 //!< コンフィグから設定

  //タスク
  void AddTask(CTaskBase *task) { taskManager.AddTask(task); }  //!< 新規タスク追加
  void RemoveTask(DWORD id) { taskManager.RemoveTaskByID(id); } //!< 指定IDを持つタスクを削除
  CExclusiveTaskBase *GetCurrentMainTask();                     //!< 現在の排他タスクを取得
  CBackgroundTaskBase *FindBGTask(DWORD id); //!< BG系タスクの中から指定IDを持つタスクを探す
  CTaskBase *FindTask(DWORD id); //!< 通常系タスクの中から指定IDを持つタスクを探す

  //システムグラフィックを取得
  MYSURFACE **GetSystemGraphicSurface() {
    return sdds;
  } //!< システムとして保持されているGCD用のビットマップ配列を取得
  GCD_RECT *GetSystemGraphicRect() { return srdat; } //!< システムとして保持されているGCDの矩形配列を取得
  GCD_CELL2 *GetSystemGraphicCell() { return scdat; } //!< システムとして保持されているGCDのセル配列を取得
  GCD_HANTEI *GetSystemGraphicHantei() {
    return shdat;
  } //!< システムとして保持されているGCDのあたり判定矩形配列を取得

  //デバッグ
  void NotifyExcption(); //!< 例外発生を通知。次回メインループでタイトル戻り（設定に依る）
  BOOL IsErrorTitle() { return m_error_title; } //!< エラー発生でタイトルに戻ったのかフラグ取得
  static void Log(const char *msg, DWORD category); //!< 分類つきでファイルへログを出す
  static void LogErr(const char *fmt, ...); //!< ファイルへログを出す(エラー扱、直後ファイルFLUSH)
  static void LogWarning(const char *fmt, ...); //!< 分類:警告 でログを出す。フォーマットつき
  void PushSysTag(const char *tag_str);         //!< タグを設定
  void PopSysTag();                             //!< タグをひとつPop、DLLタグのクリア
  void ClearSysTag();                           //!< システムタグのクリア、DLLタグもクリア
  void PushDLLTag(const char *tag_str);         //!< タグを設定
  void PopDLLTag();                             //!< タグをひとつPop
  void ClearDLLTag();                           //!< DLLタグのクリア
  void DumpTags();                              //!< タグをエラーログとして出力

protected:
  void InitSystemSound();    //!< システムサウンドのロード
  void InitSystemGraphics(); //!< システムグラフィック(system/cell.gcd等)をロード
public:
  void OnWMKeydown(WPARAM wp, LPARAM lp); //!< WM_KEYDOWNウィンドウメッセージ処理
protected:
  BOOL ShowInformation(); //!< 初期化後の情報ダイアログの表示
  void SaveScreenShot();  //!< スクリーンショット保存

  //データメンバ
public:
  //スイッチ
  BOOL sw_rects;     //!< 未使用
  BOOL sw_showfps;   //!< 未使用
  BOOL sw_showbg;    //!< 未使用
  int sw_showstates; //!< 未使用
  int sw_slowmode;   //!< 未使用
protected:
  BOOL ready;             //!< 初期化が成功したかどうか
  HWND hwnd;              //!< アプリケーションウインドウのウインドウハンドル
  DWORD m_fps;            //!< 現在のフレームレート
  DWORD m_loopTime;       //!< メインループを何msで回すか（フレームレート制御）
  BOOL m_render_disabled; //!< 描画開始/終了を行わない（SirectShow動画再生用）

  //システム・グラフィック
  MYSURFACE *sdds[GCDMAX_IMAGES];    //!< system/image?.bmpをロードしたもの
  GCD_RECT srdat[GCDMAX_RECTANGLES]; //!< system/cellgcdを ロードしたもの
  GCD_CELL2 scdat[GCDMAX_CELLS];     //!< system/cellgcdを ロードしたもの
  GCD_HANTEI shdat[GCDMAX_CELLS];    //!< system/cellgcdを ロードしたもの

  //システム・サウンド
  LPDIRECTSOUNDBUFFER dsb_efct[2][NUM_SYSTEMSOUND]; //!< システムサウンド
  BOOL b_efctlst
      [NUM_SYSTEMSOUND]; //!< システムサインどのうちどちらのサウンドを再生したか。連続して同じwavを再生するために必要

  CTaskManager taskManager;  //!< タスク管理クラス
  BOOL m_return_title_flag;  //!< ReturnTitleフラグ
  BOOL m_error_title;        //!< エラータイトル画面フラグ
  BOOL m_first_update_flag;  //!< 起動後初回フラグ
  BOOL m_debug_menu_enabled; //!< デバッグメニューが有効

  static char *logBuffer;            //!< デバッグログ文字列フォーマット用バッファ
  std::stack<const char *> m_systag; //!< 例外発生時に吐き出すシステムタグ
  std::stack<const char *> m_dlltag; //!< 例外発生時に吐き出すDLLタグ
};

#endif //_SYSTEM_H_
