/*!
*	@file
*	@brief (�f�o�b�O�p)�ϋv�e�X�g
*/
#pragma once
#include "task.h"

/*!
*	@brief �f�o�b�O�p�ϋv�e�X�g�^�X�N�E�x�[�X
*	@ingroup debug
*
*	�f�o�b�O�p�Ɏ�����A�����Ď��s�B
*	�`����L�����N�^�[�����ׂă����_���B�K����Setting�n�֐����I�[�o�[���C�h�����
*	����̏����ŌJ��Ԃ����Ƃ��ł���Ǝv���B
*	���s����F8�̃^�C�g����ʖ߂�ŏI�����Ă��������B
*/
class CTEnduranceTest : public CExclusiveTaskBase
{
public:
	virtual ~CTEnduranceTest(){}

	virtual BOOL Inactivate(DWORD nextTaskID){return TRUE;}	//!< �����^�X�N���n�܂��Ă��I�����Ȃ��悤�ATRUE��Ԃ�
	virtual BOOL Execute(DWORD time);						//!< �����ݒ�����āA�����^�X�N���J�n����݂̂ł�

protected:
	virtual void Setting_BattleType();			//!< �ΐ�`����ݒ�
	virtual void Setting_Characters();			//!< �L�����N�^�[��ݒ�
	virtual void Setting_Stage();				//!< �X�e�[�W��ݒ�
	virtual void Setting_Other();				//!< ���̑��H���ԂƂ��B
	virtual void Setting_CreateBattleTask();	//!< �퓬�^�X�N��ݒ�

	void InitRandomCharacters();				//!< m_randchar �ɓK�؂Ȓl��I������
	UINT m_randchar[2][MAXNUM_TEAM];			//!< (�Ȃ�ׂ�)�d�����Ȃ��悤�ɑI�����������_���L����
};

/*!
*	@brief �^�C�}���e�X�g
*	@ingroup debug
*
*	1vs1�̑ΐ���J�肩�����A�ǂ��炪�������������O�ɏo���B
*	���Ƃ̓��O��Excel�Ƃ��ŏW�v���Đ��ѕ\������Ă݂悤�B
*/
class CTTaimanTest : public CTEnduranceTest
{
public:
	CTTaimanTest();

protected:

	virtual BOOL Execute(DWORD time);			//!< ����N�����ɏ����L�����N�^�[�̑I��
	virtual void Draw();						//!< ����N�����̑���\���p

	virtual void Setting_BattleType();			//!< �����፬����
	virtual void Setting_Characters();			//!< �L�����N�^�[�́A���Ԃ�
	virtual void Setting_Other();				//!< ��������60
	virtual void Setting_CreateBattleTask();	//!< �ΐ퐬�т����O�ɏo���Ă���A�퓬�^�X�N��ݒ�

	static UINT char1;
	static UINT char2;
	BOOL select_flag;
	static BOOL first_exec;
};


