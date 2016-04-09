#include "StdAfx.h"
#include ".\task_bonus.h"
#include "global.h"

CBonusTask::CBonusTask(void)
{
}

CBonusTask::~CBonusTask(void)
{
}

void CBonusTask::Initialize(void)
{
	g_battleinfo.Initialize();
	g_battleinfo.AddCharacter(TEAM_PLAYER1, g_charlist.FindCharacter(_T("ÉÇÉiÅ[")), 1, 1, 0);
	g_battleinfo.SetStage(1);
	g_battleinfo.SetLimitTime(60);
	g_battleinfo.SetBattleType(TAISENKEISIKI_GOCYAMAZE);

	CBattleTask::Initialize();
}
