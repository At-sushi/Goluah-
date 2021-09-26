/*------------------------------------------------------------

        デバッグメニウ

--------------------------------------------------------------*/
#include "stdafx.h"
#include "debug_menu.h"
#include "global.h"

#include "task_title.h"
#include "task_select.h"
#include "task_win.h"
#include "task_staffroll.h"
#include "task_battle_net.h"
#include "..\resource.h"

#include "debug_endurance.h"

/*------------------------------------------------------------
        (生成)
--------------------------------------------------------------*/
void CDebugMenu::Activate(DWORD pid) {
  char *str;

  g_sound.BGMStop();

  str = "タイトル画面へ";
  namelist.push_back(str);

  str = "キャラセレ";
  namelist.push_back(str);

  str = "勝利画面";
  namelist.push_back(str);

  str = "スタッフロール";
  namelist.push_back(str);

  str = "耐久テスト";
  namelist.push_back(str);

  str = "耐久テスト2";
  namelist.push_back(str);

  if (!g_config.IsFullScreen()) {
    str = "ネット対戦モ－ド（ホスト）（現在使用不可）";
    namelist.push_back(str);
    str = "ネット対戦モ－ド（クライアント）（現在使用不可）";
    namelist.push_back(str);
  }

  m_cnt = 0;

  if (GetKeyState(VK_SHIFT) & 0x8000)
    m_loop_exec = FALSE;
}

/*------------------------------------------------------------
        (破棄)
--------------------------------------------------------------*/
BOOL CDebugMenu::Inactivate(DWORD nid) {
  namelist.clear();
  return TRUE;
}

/*------------------------------------------------------------
        更新
--------------------------------------------------------------*/
BOOL CDebugMenu::Execute(DWORD time) {
  DWORD keysta = g_input.GetKey(0, 0);
  if (m_cnt++ < 10)
    return TRUE;

  //連続実行
  if (m_loop_exec) {
    keysta |= KEYSTA_BA2;
    m_selected = m_last_selected;
  }

  if (keysta & KEYSTA_UP2) {
    m_selected--;
  } else if (keysta & KEYSTA_DOWN2) {
    m_selected++;
  } else if (keysta & KEYSTA_BA2) {
    m_last_selected = m_selected;
    if (GetKeyState(VK_SHIFT) & 0x8000)
      m_loop_exec = TRUE;

    switch (m_selected % namelist.size()) {
    case 0:
      g_system.AddTask(new CTitle);
      return FALSE;
    case 1:
      g_system.AddTask(new CCharacterSelect);
      return FALSE;
    case 2: {
      srand(timeGetTime());
      UINT team = timeGetTime() % 2;
      UINT num = rand() % 3 + 1;
      for (UINT i = 0; i < num; i++) {
        g_battleinfo.AddCharacter(team, rand() % g_charlist.GetCharacterCount(), rand() % 3 + 1, FALSE, 0);
      }
      g_battleresult.Initialize(team);
      g_system.AddTask(new CYouWin);
      return FALSE;
    } break;
    case 3: {
      CStoryElement_Staff *ele_staff = new CStoryElement_Staff;
      ele_staff->FeedLine("file staff.txt");
      CTStaffRoll *psr = new CTStaffRoll;
      psr->SetKeyIndex(0);
      psr->Setup(g_storylist.GetStoryDir(1), ele_staff);
      g_system.AddTask(psr);
      delete ele_staff;
      return FALSE;
    } break;
    case 4:
      g_system.AddTask(new CTEnduranceTest);
      break;
    case 5:
      g_system.AddTask(new CTTaimanTest);
      break;
    case 6: {
      /*				if (g_play.IsConnected() || g_play.InitConnection())
                                              g_system.AddTask( new CBattleTaskNet );
                                              */
      break;
    }

    case 7: {
      break;
    }
    }
  }

  return TRUE;
}

/*------------------------------------------------------------
        描画
--------------------------------------------------------------*/
void CDebugMenu::Draw() {
  RECT r = {30, 30, 540, 380};
  UINT i;

  for (i = 0; i < namelist.size(); i++) {
    if (i == (m_selected % namelist.size())) {
      g_draw.DrawRedText(r, namelist[i], strlen(namelist[i]), DT_LEFT, 1);
    } else {
      g_draw.DrawBlueText(r, namelist[i], strlen(namelist[i]), DT_LEFT, 1);
    }
    r.top += 24;
  }

  r.top += 24;
  char *str_ipt = new char[256];
  for (i = 0; i < MAXNUM_KEYI; i++) {
    DWORD ipt = g_input.GetKey(i, 0);
    sprintf(str_ipt, "%d : %s %s %s %s %s %s %s %s", i, (ipt & KEYSTA_UP) ? "↑" : "　",
            (ipt & KEYSTA_DOWN) ? "↓" : "　", (ipt & KEYSTA_ALEFT) ? "←" : "　",
            (ipt & KEYSTA_ARIGHT) ? "→" : "　", (ipt & KEYSTA_BA) ? "Ａ" : "　",
            (ipt & KEYSTA_BB) ? "Ｂ" : "　", (ipt & KEYSTA_BC) ? "Ｃ" : "　",
            (ipt & KEYSTA_BD) ? "Ｄ" : "　");
    g_draw.DrawBlueText(r, str_ipt, strlen(str_ipt), DT_LEFT, 1);
    r.top += 24;
  }
  delete[] str_ipt;
}
