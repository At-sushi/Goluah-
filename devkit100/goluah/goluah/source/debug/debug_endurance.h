/*!
 *	@file
 *	@brief (デバッグ用)耐久テスト
 */
#pragma once
#include "task.h"

/*!
 *	@brief デバッグ用耐久テストタスク・ベース
 *	@ingroup debug
 *
 *	デバッグ用に試合を連続して実行。
 *	形式やキャラクター等すべてランダム。適当なSetting系関数をオーバーライドすれば
 *	特定の条件で繰り返すこともできると思う。
 *	実行時はF8のタイトル画面戻りで終了してください。
 */
class CTEnduranceTest : public CExclusiveTaskBase {
public:
  virtual ~CTEnduranceTest() {}

  virtual BOOL Inactivate(DWORD nextTaskID) {
    return TRUE;
  } //!< 試合タスクが始まっても終了しないよう、TRUEを返す
  virtual BOOL Execute(DWORD time); //!< 試合設定をして、試合タスクを開始するのみです

protected:
  virtual void Setting_BattleType();       //!< 対戦形式を設定
  virtual void Setting_Characters();       //!< キャラクターを設定
  virtual void Setting_Stage();            //!< ステージを設定
  virtual void Setting_Other();            //!< その他？時間とか。
  virtual void Setting_CreateBattleTask(); //!< 戦闘タスクを設定

  void InitRandomCharacters();     //!< m_randchar に適切な値を選択する
  UINT m_randchar[2][MAXNUM_TEAM]; //!< (なるべく)重複しないように選択したランダムキャラ
};

/*!
 *	@brief タイマンテスト
 *	@ingroup debug
 *
 *	1vs1の対戦を繰りかえし、どちらが勝ったかをログに出す。
 *	あとはログをExcelとかで集計して成績表を作ってみよう。
 */
class CTTaimanTest : public CTEnduranceTest {
public:
  CTTaimanTest();

protected:
  virtual BOOL Execute(DWORD time); //!< 初回起動時に初期キャラクターの選択
  virtual void Draw();              //!< 初回起動時の操作表示用

  virtual void Setting_BattleType();       //!< ごちゃ混ぜ戦
  virtual void Setting_Characters();       //!< キャラクターは、順番で
  virtual void Setting_Other();            //!< 制限時間60
  virtual void Setting_CreateBattleTask(); //!< 対戦成績をログに出してから、戦闘タスクを設定

  static UINT char1;
  static UINT char2;
  BOOL select_flag;
  static BOOL first_exec;
};
