/*===============================================================

	VS���[�h�^�X�N
	
	Activate����邽�тɃL�����N�^�[�I���^�X�Nor������ʃ^�X�N���N������

=================================================================*/
#pragma once

#include "task.h"

/*!
*	@brief VS���[�h
*	@ingroup Tasks
*
*	Activate����邽�тɃL�����N�^�[�I���^�X�Nor������ʃ^�X�N���N������
*/
class CTaskVsMode : public CExclusiveTaskBase
{
public:
	void Initialize();
	BOOL Execute(DWORD time);
	void Terminate(){}
	DWORD GetID(){return('VsMd');}

	void Activate(DWORD prvTaskID);
	BOOL Inactivate(DWORD nextTaskID){return TRUE;}

	void SetCondition(BOOL vsCom,DWORD numTeam){m_vsCom=vsCom;m_numTeam=numTeam;}
protected:
	BOOL m_vsCom;
	DWORD m_numTeam;
	DWORD m_prev_task_id;
};



