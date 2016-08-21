
#include "stdafx.h"
#include "global.h"
#include "task_practice.h"


void CBattlePractice::Initialize()
{
	m_reset_counter = 0;
	CBattleTask::Initialize();
}

void CBattlePractice::T_Command()
{
/*	int i;
	for(i=0;i<(int)p_objects.size();i++){
		if(p_objects[i]!=NULL)
		{
			if(p_objects[i]->data.tid != 1 || //敵キャラでない
				!(p_objects[i]->data.objtype&GOBJFLG_COMPUTER))//コンピュータ制御でない
			{
				p_objects[i]->Message(GOBJMSG_COMMAND);
			}
		}
	}*/
	CBattleTask::T_Command();	// 普通の処理で良くなったみたい
}

void CBattlePractice::StartRound()
{
	CBattleTask::StartRound();

	m_hpratio = g_battleinfo.GetLimitTime() / 100.0;
	limittime = -1;
}

void CBattlePractice::T_UpdateStatus_Fighting()
{
	CGObject* pobj;
	BOOL reset_flag;

	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < MAXNUM_TEAM; i++)
		{
			pobj = GetGObject(charobjid[j][i]);
			reset_flag = FALSE;

			if (!(pobj->data.aid & ACTID_KURAI) && !(pobj->data.aid & ACTID_GUARD))
			{
				m_reset_counter = 0;
				pobj->data.hp = (int)(pobj->data.hpmax * m_hpratio);
				pobj->data.gauge = pobj->data.gaugemax;
			}

			if (pobj->data.hp <= 0)pobj->data.hp = 1;
			if (pobj->data.aid == ACTID_KAITENFINISH || pobj->data.aid == ACTID_FINALDOWN)
			{
				pobj->data.aid = ACTID_FUTTOBI;
			}
		}
	}

	CBattleTask::T_UpdateStatus_Fighting();
}