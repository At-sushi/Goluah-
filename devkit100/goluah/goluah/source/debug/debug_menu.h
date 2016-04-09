/*!
*	@file
*	@brief �f�o�b�O���j�E
*/
#pragma once

#include "task.h"

/*!
*	@defgroup Debug
*	@brief �f�o�b�O�p���낢��H
*/

/*!
*	@brief �f�o�b�O�p�ȈՃ^�C�g�����
*	@ingroup Debug
*
*	�f�o�b�O�r���h�̏ꍇ�A�^�C�g����ʂ̑���ɂ��ꂪ�o�܂�
*	�^�C�g����ʖ߂�̑��������ɖ߂�܂��B
*/
class CDebugMenu : public CExclusiveTaskBase
{
public:
	CDebugMenu(){m_selected=0;m_loop_exec=FALSE;}
	void Initialize(){Activate(0);}
	void Terminate(){Inactivate(0);}

	void Activate(DWORD ptid);
	BOOL Inactivate(DWORD ntid);

	BOOL Execute(DWORD time);
	void Draw();
	DWORD GetID(){return('debg');}

protected:
	UINT m_cnt;
	std::vector<TCHAR*> namelist;
	UINT m_selected;

	UINT m_last_selected;
	BOOL m_loop_exec;
};