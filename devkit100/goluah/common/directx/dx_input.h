/*!
 *	@file
 *	@brief CDirectInput
 *
 *	キー入力管理クラス。
 *	入力のログをとり、ボタンが押されている・いないの判断だけでなく、
 *	押しっぱなしの状態なのか、現在フレームで押されたものなのかも判断できる。
 *
 *	キーボードの入力とパッド（DirectX8以上のDirectInputを使用）の
 *	両方の入力を入力を合成して処理する。
 *
 */
#pragma once

#include "define_const.h"
#include "config.h" //パッド数の定義が必要

/*!
 *	@ingroup DirectX
 */
/*@{*/

//!定数定義
#define DINPUT_MAX_GAMEPAD NUM_PAD

/*!
 *	@brief キー入力管理クラス。
 *	@ingroup DirectX
 *
 *	入力のログをとり、ボタンが押されている・いないの判断だけでなく、
 *	押しっぱなしの状態なのか、現在フレームで押されたものなのかも判断できる。
 *
 *	キーボードの入力とパッド（DirectX8以上のDirectInputを使用）の
 *	両方の入力を入力を合成して処理する。
 *
 *	都合により生のキー入力を返さない場合がある
 *	・KeyLock で入力をロックした場合
 *	・EnableComKey でコンピュータのガードキー入力を有効にした場合
 */
class CDirectInput {
public:
  CDirectInput();
  ~CDirectInput() { Destroy(); }

public:
  BOOL Initialize(HWND hwnd);
  BOOL InitializePad(); // DirectInput初期化
  void Destroy();

  // ■定時処理
  void KeyLog(); //キー入力ログ生成。毎フレーム呼ぶ必要がある
  void RecoverDirectInput(WPARAM wParam, LPARAM lParam); //ウインドウフォーカス復帰処理

  //!入力取得関数の戻り値を0に固定することによって、仮想的にキー入力を禁止する
  void KeyLock(BOOL lk = TRUE);

  //!全キーログを消去する
  void ClearKeyLog();

  //!仮想キーをDirectInputキーに変換
  static BYTE VirtKeyToDInputKey(int vKey);

  //!初期化時に必要なコールバック関数
  static BOOL FAR CALLBACK EnumGamePad(LPCDIDEVICEINSTANCE lpddi, LPVOID pvref);
  static BOOL FAR CALLBACK EnumAxis(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvref);

  //!フォース始動
  void StartForce(DWORD cid, DWORD num);

  // ■キー入力データ取得系
public:
  DWORD GetKeyEx(DWORD cid, DWORD ofst);
  int SeekKeyEx(DWORD cid, int offset, int num_seek, DWORD key);
  DWORD GetKeyNet(DWORD cid, DWORD ofst);
  int SeekKeyNet(DWORD cid, int offset, int num_seek, DWORD key);
  DWORD SetKeyNet(DWORD index, DWORD key);
  DWORD GetAllKey();
  DWORD GetKey(DWORD cid, DWORD ofst);
  int SeekKey(DWORD cid, int offset, int num_seek, DWORD key);

public:
  // ■COMガード時用
  void EnableComKey() { com_grd_enabled = TRUE; }
  void DisableComKey() { com_grd_enabled = FALSE; }
  void SetComKey(DWORD key) { com_grd_key = key; }

  // ■内部動作関数
private:
  DWORD KeyLog2(DWORD cid, int kb, int pad, PBYTE KeyState = NULL); //!< キーログ内部動作
  void CleanDInput();                                               //!< DirectInput後始末
  DWORD GetPadState(int pid);                                       //!< Pad データ取得
  void InitializeEffects();                                         //!< エフェクト初期化

  // ■データメンバ
public:
  int keylognow;                        //!< ログ開始位置
  DWORD keylog[MAXNUM_KEYI][256 + 256]; //!< キー入力のログ
  DWORD netkey[MAXNUM_KEYI][256 + 256]; //!< ネット対戦時のキーログ（パスワードクラックに使うアレではない）

  //パッド(DirectInput)関連
  int jsnum;                                       //!< 認識したパッドの数
  char gamepadname[DINPUT_MAX_GAMEPAD][256];       //!< パッドの名前
  LPDIRECTINPUT8 pdi;                              //!< DirectInputオブジェクト
  LPDIRECTINPUTDEVICE8 pdidev[DINPUT_MAX_GAMEPAD]; //!< デバイスオブジェクト
  LPDIRECTINPUTDEVICE8 pdidev_kb; //!< キーボードデバイスを所得してみるテスト
  LPDIRECTINPUTEFFECT pdieffect[DINPUT_MAX_GAMEPAD][3]; //!< フォースフィードバックのエフェクト

  DWORD com_grd_key;    //!< COMガードキー
  BOOL com_grd_enabled; //!< COMガードキーが有効

private:
  HWND hwnd;             //!< ウインドウハンドル（フォーカス等に関係）
  BOOL keylocked;        //!< キーロックフラグ
  UINT input_sleep_time; //!< ClearKeyLog() 後にちょっとの間キー入力を無効にするカウンタ
  KEYCONFIG DIKeyState;  //!< DirectInput用に変換した、キーコンフィグ情報
};

#define PADUP 0x00000001
#define PADDOWN 0x00000002
#define PADLEFT 0x00000004
#define PADRIGHT 0x00000008
#define PADA 0x00000010
#define PADB 0x00000020
#define PADC 0x00000040
#define PADD 0x00000080

/*@}*/
