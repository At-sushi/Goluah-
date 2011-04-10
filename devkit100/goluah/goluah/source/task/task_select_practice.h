
/*=======================================================================================

	���K���[�h �L�����Z��

=========================================================================================*/

#include "task_select.h"

/*!
*	@brief PRACTICE ���[�h�p�L�����Z��
*	@ingroup Tasks
*
*	���͂��Ȃ�蔲��
*/
class CCharacterSelectPractice : public CCharacterSelect
{
public:
	virtual void Initialize();
	virtual void CreateNextTask();
};


/*!
*	@brief PRACTICE ���[�h�p���������I���^�X�N
*	@ingroup Tasks
*
*	�v���C���[�ɑI���������A�Œ������Ԃ��悤�ɂ����B
*	�蔲���N���X�B
*
*	�{��(1.00)�ɔ�ׂď����C���������Ă���̂ŁA���͂���������Ƃ��Ă܂��B
*/
class CTConditionSelecterPractice : public CTConditionSelecter
{
public:
	virtual void Initialize();
	virtual BOOL Execute(DWORD time);
	virtual void Draw();

protected:
	void Change(BOOL key);

	static const int m_hp_ratio[5];				//80,100,10,30,50
};