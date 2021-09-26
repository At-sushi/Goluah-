#include "StdAfx.h"

#include "define_macro.h"
#include "global.h"
#include ".\task_demobattle.h"

void CBattleTaskDemo::Initialize() {
  // ランダムな対戦条件を設定
  g_battleinfo.Initialize();
  InitRandomCharacters();

  Setting_BattleType();
  Setting_Characters();
  Setting_Stage();
  Setting_Other();

  CBattleTask::Initialize();

  // BGMを鳴らさない
  g_sound.BGMStop();
}

BOOL CBattleTaskDemo::Execute(DWORD time) {
  // キー入力か一定時間経過でデモモード中止
  if (bf_counter >= DEMOBATTLE_LIMITTIME || g_input.GetAllKey() & KEYSTA_ANYKEY || round != 1)
    return FALSE;

  return CBattleTask::Execute(time);
}

void CBattleTaskDemo::Setting_BattleType() {
  UINT battle_type;

  switch (rand() % 3) {
  case 0:
    battle_type = TAISENKEISIKI_JYUNBAN;
    break;
  case 1:
    battle_type = TAISENKEISIKI_KOUTAI;
    break;
  default:
    battle_type = TAISENKEISIKI_GOCYAMAZE;
  }

  g_battleinfo.SetBattleType(battle_type);
}

void CBattleTaskDemo::Setting_Characters() {
  UINT i, j;

  for (i = 0; i < 2; i++) {
    UINT num = 1 + rand() % DEMOBATTLE_MAXTEAM;
    for (j = 0; j < num; j++) {
      g_battleinfo.AddCharacter(i, m_randchar[i][j], 1, CASSIGN_COM,
                                g_charlist.GetRandomOption(m_randchar[i][j]));
    }
  }
}

void CBattleTaskDemo::Setting_Stage() { g_battleinfo.SetStage(rand() % g_stagelist.GetStageCount()); }

void CBattleTaskDemo::Setting_Other() { g_battleinfo.SetLimitTime(rand() % 60 + 30); }

void CBattleTaskDemo::InitRandomCharacters() {
  int i, j, k, l;

  for (i = 0; i < 2; i++) {
    for (j = 0; j < DEMOBATTLE_MAXTEAM; j++) {
      m_randchar[i][j] = 0xFFFFFFFF;
    }
  }

  UINT breaker;
  BOOL jyufuku;
  for (i = 0; i < 2; i++) {
    for (j = 0; j < DEMOBATTLE_MAXTEAM; j++) {
      breaker = 0;
      while (breaker < 100) {
        m_randchar[i][j] = rand() % g_charlist.GetCharacterCount();
        jyufuku = FALSE;
        for (k = 0; k < 2; k++) {
          for (l = 0; l < DEMOBATTLE_MAXTEAM; l++) {
            if (m_randchar[i][j] == m_randchar[k][l] && !(i == k && j == l)) {
              jyufuku = TRUE;
            }
          }
        }
        if (!jyufuku)
          break;
        breaker++;
      }
    }
  }
}
