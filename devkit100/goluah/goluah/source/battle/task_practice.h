
/*=======================================================================================

        練習モード 戦闘

=========================================================================================*/

#include "task_battle.h"

/*!
 *	@brief 練習モード用戦闘タスク
 *	@ingroup Tasks
 *
 *	手抜き。
 *	更新処理で定期的にゲージと体力の増加・死亡回避処理。
 *
 *	@sa CBattleTask
 */
class CBattlePractice : public CBattleTask {
public:
  virtual void Initialize();

protected:
  virtual void T_Command();
  virtual void T_UpdateStatus_Fighting();
  virtual void StartRound();

  UINT m_reset_counter;
  double m_hpratio;
};
