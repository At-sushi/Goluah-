
/*===============================================================

	VSモードタスク

=================================================================*/

#include "global.h"
#include "task_vsmode.h"
#include "task_select.h"
#include "task_win.h"


/*--------------------------------------------------------------
	VSモード
----------------------------------------------------------------*/
void CTaskVsMode::Initialize()
{
	Activate('Win');
}

void CTaskVsMode::Activate(DWORD prvTaskID)
{
	m_prev_task_id = prvTaskID;
}

BOOL CTaskVsMode::Execute(DWORD time)
{
	switch(m_prev_task_id)
	{
	case 'titl':
	case 'Win':
		g_system.AddTask( new CCharacterSelect );
		return TRUE;
	case 'Btl':
		g_system.AddTask( new CYouWin );
		return TRUE;
	default:
		return FALSE;
	}
	
}

