/*===============================================================

	VSモードタスク
	
	Activateされるたびにキャラクター選択タスクor勝利画面タスクを起動する

=================================================================*/
#pragma once

#include "task.h"

/*!
*	@brief VSモード
*	@ingroup Tasks
*
*	Activateされるたびにキャラクター選択タスクor勝利画面タスクを起動する
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



