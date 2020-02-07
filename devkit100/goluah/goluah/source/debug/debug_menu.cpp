/*------------------------------------------------------------

	デバッグメニウ

--------------------------------------------------------------*/
#include "stdafx.h"
#include "debug_menu.h"
#include "global.h"

#include "task_title.h"
#include "task_staffroll.h"
#include "task_opening.h"
#include "..\resource.h"
#include "Name.h"
#include "NameC.h"

/*------------------------------------------------------------
	(生成)
--------------------------------------------------------------*/
void CDebugMenu::Activate(DWORD pid)
{
	TCHAR* str;

	g_sound.BGMStop();

	str = _T("タイトル画面へ");
	namelist.push_back(str);

	str = _T("キャラセレ");
	namelist.push_back(str);

	str = _T("勝利画面");
	namelist.push_back(str);
	
	str = _T("スタッフロール");
	namelist.push_back(str);

	str = _T("耐久テスト");
	namelist.push_back(str);

	str = _T("OPムービー");
	namelist.push_back(str);

	if (!g_config.IsFullScreen())
	{
		str = _T("ネット対戦モ－ド（ホスト）");
		namelist.push_back(str);
		str = _T("ネット対戦モ－ド（クライアント）");
		namelist.push_back(str);
	}

	m_cnt = 0;

	if(GetKeyState(VK_SHIFT) & 0x8000)m_loop_exec=FALSE;
}


/*------------------------------------------------------------
	(破棄)
--------------------------------------------------------------*/
BOOL CDebugMenu::Inactivate(DWORD nid)
{
	namelist.clear();
	return TRUE;
}


/*------------------------------------------------------------
	更新
--------------------------------------------------------------*/
BOOL CDebugMenu::Execute(DWORD time)
{
	DWORD keysta = g_input.GetKey(0,0);
	if(m_cnt++ < 10)return TRUE;

	//連続実行
	if(m_loop_exec){
		keysta |= KEYSTA_BA2;
		m_selected = m_last_selected;
	}

	if(keysta&KEYSTA_UP2){
		m_selected--;
	}
	else if(keysta&KEYSTA_DOWN2){
		m_selected++;
	}
	else if(keysta&KEYSTA_BA2)
	{
		m_last_selected = m_selected;
		if(GetKeyState(VK_SHIFT) & 0x8000)m_loop_exec=TRUE;

		switch(m_selected%namelist.size())
		{
			case 0:g_system.AddTask( new CTitle );return FALSE;
			case 2:
				{
					return FALSE;
				}
				break;
			case 3:
				{
					return FALSE;
				}
				break;
			case 4:
				break;
			case 5:
				g_system.AddTask(new CTOpening);
				break;
			case 6:
				{
/*				if (g_play.IsConnected() || g_play.InitConnection())
					g_system.AddTask( new CBattleTaskNet );
					*/	
				CName dialog;

				if (dialog.DoModal() == IDCANCEL)
					return TRUE;

				g_play.Initialize(dialog.m_name, dialog.m_port);
				g_play.Host();
				break;
				}

			case 7:
				{
				CNameC dialog;

				if (dialog.DoModal() == IDCANCEL)
					return TRUE;

				g_play.Initialize(dialog.m_name, dialog.m_port);
				g_play.Connect(dialog.m_hostIP, atoi(dialog.m_hostPort));
				break;
				}
		}
	}

	return TRUE;
}


/*------------------------------------------------------------
	描画
--------------------------------------------------------------*/
void CDebugMenu::Draw()
{
	RECT r = {30,30,540,380};
	UINT i;

	for(i=0;i<namelist.size();i++){
		if(i==(m_selected%namelist.size())){
			g_draw.DrawRedText(r,namelist[i],strlen(namelist[i]),DT_LEFT,1);
		}
		else{
			g_draw.DrawBlueText(r,namelist[i],strlen(namelist[i]),DT_LEFT,1);
		}
		r.top += 24;
	}

	r.top += 24;
	TCHAR *str_ipt = new TCHAR [256];
	for(i=0;i<MAXNUM_KEYI;i++)
	{
		DWORD ipt = g_input.GetKey(i,0);
		_stprintf(str_ipt,_T("%d : %s %s %s %s %s %s %s %s"),
			i,
			(ipt&KEYSTA_UP)		? _T("↑") : _T("　"),
			(ipt&KEYSTA_DOWN)	? _T("↓") : _T("　"),
			(ipt&KEYSTA_ALEFT)	? _T("←") : _T("　"),
			(ipt&KEYSTA_ARIGHT)	? _T("→") : _T("　"),
			(ipt&KEYSTA_BA)		? _T("Ａ") : _T("　"),
			(ipt&KEYSTA_BB)		? _T("Ｂ") : _T("　"),
			(ipt&KEYSTA_BC)		? _T("Ｃ") : _T("　"),
			(ipt&KEYSTA_BD)		? _T("Ｄ") : _T("　")
			);
		g_draw.DrawBlueText(r,str_ipt,strlen(str_ipt),DT_LEFT,1);
		r.top += 24;
	}
	delete [] str_ipt;
}

