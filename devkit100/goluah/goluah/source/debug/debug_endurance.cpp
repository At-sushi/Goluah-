/*------------------------------------------------------------------

	耐久テスト

--------------------------------------------------------------------*/
#include "stdafx.h"

#include "global.h"
#include "debug_endurance.h"
#include "task_battle.h"

BOOL CTEnduranceTest::Execute(DWORD time)
{
	srand( timeGetTime() );

	g_battleinfo.Initialize();
	InitRandomCharacters();

	Setting_BattleType();
	Setting_Characters();
	Setting_Stage();
	Setting_Other();
	Setting_CreateBattleTask();

	return TRUE;
}

void CTEnduranceTest::Setting_BattleType()
{
	UINT battle_type;

	switch(rand()%3)
	{
	case 0:battle_type = TAISENKEISIKI_JYUNBAN;break;
	case 1:battle_type = TAISENKEISIKI_KOUTAI;break;
	default:battle_type = TAISENKEISIKI_GOCYAMAZE;
	}

	g_battleinfo.SetBattleType( battle_type );
}

void CTEnduranceTest::Setting_Characters()
{
	UINT i,j;

	for(i=0;i<2;i++)
	{
		UINT num = 1 + rand()%3;
		for(j=0;j<num;j++)
		{
			g_battleinfo.AddCharacter(i,
					m_randchar[i][j],
					1,
					CASSIGN_COM,
					g_charlist.GetRandomOption( m_randchar[i][j] )
					);
		}
	}
}

void CTEnduranceTest::Setting_Stage()
{
	g_battleinfo.SetStage( rand()%g_stagelist.GetStageCount() );
}

void CTEnduranceTest::Setting_Other()
{
	g_battleinfo.SetLimitTime( rand()%60 + 30 );
}

void CTEnduranceTest::Setting_CreateBattleTask()
{
	g_system.Log(_T("start battle"),SYSLOG_INFO);
	g_system.AddTask( new CBattleTask );
}

void CTEnduranceTest::InitRandomCharacters()
{
	int i,j,k,l;

	for(i=0;i<2;i++){
		for(j=0;j<MAXNUM_TEAM;j++)
		{
			m_randchar[i][j] = 0xFFFFFFFF;
		}
	}

	UINT breaker;
	BOOL jyufuku;
	for(i=0;i<2;i++){
		for(j=0;j<MAXNUM_TEAM;j++)
		{
			breaker = 0;
			while(breaker<100)
			{
				m_randchar[i][j] = rand()%g_charlist.GetCharacterCount();
				jyufuku = FALSE;
				for(k=0;k<2;k++)
				{
					for(l=0;l<MAXNUM_TEAM;l++)
					{
						if(m_randchar[i][j]==m_randchar[k][l] && !(i==k && j==l))
						{
							jyufuku = TRUE;
						}
					}
				}
				if(!jyufuku)break;
				breaker++;
			}
		}
	}
}

//やンのかゴルァ―――!!(タイマンのイメージ)

UINT CTTaimanTest::char1 = 0;
UINT CTTaimanTest::char2 = 0;
BOOL CTTaimanTest::first_exec = TRUE;

CTTaimanTest::CTTaimanTest()
{
	select_flag = FALSE;
}

BOOL CTTaimanTest::Execute(DWORD time)
{
	DWORD key = g_input.GetAllKey();

	if(first_exec)
	{
		if(key&KEYSTA_DOWN2 || key&KEYSTA_UP2)
		{	
			select_flag = !select_flag;
		}
		if(key&KEYSTA_LEFT2)
		{
			if(select_flag)
			{
				if(char1>0)char1--;
			}
			else
			{
				if(char2>0)char2--;
			}
		}
		if(key&KEYSTA_RIGHT2)
		{
			if(select_flag)
			{
				if(char1+1 < (UINT)g_charlist.GetCharacterCount())char1++;
			}
			else
			{
				if(char2+1 < (UINT)g_charlist.GetCharacterCount())char2++;
			}
		}

		if(!(key&KEYSTA_BC2))return TRUE;
	}

	return CTEnduranceTest::Execute(time);
}

void CTTaimanTest::Draw()
{
	if(first_exec)
	{
		TCHAR *str = new TCHAR[64];

		_stprintf(str,_T("%d/%d"),char1,g_charlist.GetCharacterCount());
		g_system.DrawBMPText(10,50,0,str,select_flag ? 0xFFFFFFFF : 0x99999999);
		
		_stprintf(str,_T("%d/%d"),char2,g_charlist.GetCharacterCount());
		g_system.DrawBMPText(10,100,0,str,select_flag ? 0x99999999 : 0xFFFFFFFF);

		delete [] str;
	}
}

void CTTaimanTest::Setting_BattleType()
{
	g_battleinfo.SetBattleType( TAISENKEISIKI_GOCYAMAZE );
}

void CTTaimanTest::Setting_Characters()
{
	if(!first_exec)
	{
		TCHAR *str = new TCHAR [256];

		UINT winner = g_battleresult.GetWinner() ? char2 : char1;
		UINT loser	= g_battleresult.GetWinner() ? char1 : char2;

		_stprintf(str,_T("\t%s(%d)\t\t%s(%d)"),
			g_charlist.GetCharacterName(winner),
			winner,
			g_charlist.GetCharacterName(loser),
			loser
			);

		CSystem::Log(str,SYSLOG_INFO);
		delete [] str;

		g_log2file.Flush();
	}
	else first_exec = FALSE;

	char2++;
	if(char2>=(UINT)g_charlist.GetCharacterCount())
	{
		char2 = 0;
		char1++;

		if(char1>=(UINT)g_charlist.GetCharacterCount())
		{
			char1=0;
		}
	}

	g_battleinfo.AddCharacter(0,
					char1,
					1,
					CASSIGN_COM,
					g_charlist.GetRandomOption( char1 )
					);
	
	g_battleinfo.AddCharacter(1,
					char2,
					1,
					CASSIGN_COM,
					g_charlist.GetRandomOption( char2 )
					);
}

void CTTaimanTest::Setting_Other()
{
	g_battleinfo.SetLimitTime( 60 );
}

void CTTaimanTest::Setting_CreateBattleTask()
{
	CTEnduranceTest::Setting_CreateBattleTask();
}
