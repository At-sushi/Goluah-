
#include "stdafx.h"
#include "global.h"
#include "task_practice.h"

void CBattlePractice::Initialize() {
  m_reset_counter = 0;
  CBattleTask::Initialize();
}

void CBattlePractice::T_Command() {
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
  CBattleTask::T_Command(); // 普通の処理で良くなったみたい
}

void CBattlePractice::StartRound() {
  CBattleTask::StartRound();

  m_hpratio = g_battleinfo.GetLimitTime() / 100.0;
  limittime = -1;
}

void CBattlePractice::T_UpdateStatus_Fighting() {
  CGObject *eobj;
  CGObject *pobj;

  for (int i = 0; i < MAXNUM_TEAM; i++) {
    eobj = GetGObject(charobjid[1][i]);
    pobj = GetGObject(charobjid[0][i]);

    BOOL reset_flag = FALSE;

    if (!(eobj->data.aid & ACTID_KURAI) && !(eobj->data.aid & ACTID_GUARD) &&
        !(pobj->data.aid & ACTID_ATTACK) && !(pobj->data.aid & ACTID_KUCYU)) {
      m_reset_counter++;
      if (m_reset_counter > 20) {
        reset_flag = TRUE;
      }
    }

    if (reset_flag && (pobj->data.id != 0 || eobj->data.id != 0)) {
      m_reset_counter = 0;
      eobj->data.hp = eobj->data.hpmax;
      pobj->data.hp = (int)(pobj->data.hpmax * m_hpratio);
      eobj->data.gauge = eobj->data.gaugemax;
      pobj->data.gauge = pobj->data.gaugemax;
    }

    if (eobj->data.hp <= 0)
      eobj->data.hp = 1;
    if (pobj->data.hp <= 0)
      pobj->data.hp = 1;
    if (eobj->data.aid == ACTID_KAITENFINISH || eobj->data.aid == ACTID_FINALDOWN) {
      eobj->data.aid = ACTID_FUTTOBI;
    }
    if (pobj->data.aid == ACTID_KAITENFINISH || pobj->data.aid == ACTID_FINALDOWN) {
      pobj->data.aid = ACTID_FUTTOBI;
    }
  }

  CBattleTask::T_UpdateStatus_Fighting();
}
