
/*=======================================================================================

	���K���[�h �퓬

=========================================================================================*/

#include "task_battle.h"

/*!
*	@brief ���K���[�h�p�퓬�^�X�N
*	@ingroup Tasks
*
*	�蔲���B
*	�X�V�����Œ���I�ɃQ�[�W�Ƒ̗͂̑����E���S��������B
*
*	@sa CBattleTask
*/
class CBattlePractice : public CBattleTask
{
public:
	virtual void Initialize();
protected:
	virtual void T_Command();
	virtual void T_UpdateStatus_Fighting();
	virtual void StartRound();

	UINT m_reset_counter;
	double m_hpratio;
};