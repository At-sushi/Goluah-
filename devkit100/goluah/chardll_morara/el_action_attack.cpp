
/*==============================================================

        行動関数（通常攻撃）

================================================================*/
#include "el_character.h"

//**************************************************************
//　立
//**************************************************************

void CCharacterEL::act_att_sa() //立弱
{
  if (ccounter == 6) {
    pbul_hadou->GoA(pdat->muki, pdat->x + zurex(60), -55, 6, &aif[ATTACKINFO_STAND_A]);
  }

  if (ccounter < 2)
    cell(CELL_SA1);
  else if (ccounter < 4)
    cell(CELL_SA2);
  else if (ccounter < 12)
    cell(CELL_SA3);
  else if (ccounter < 18)
    cell(CELL_SA2);
  else if (ccounter < 24)
    cell(CELL_SA1);
  else
    ChangeAction(ACTID_NEUTRAL);
}

void CCharacterEL::act_att_sb() //立中
{
  if (ccounter == 0)
    StartAttack(ATTACKINFO_STAND_B);

  if (ccounter < 2)
    cell(CELL_SB1);
  else if (ccounter < 4)
    cell(CELL_SB2);
  else if (ccounter < 6)
    cell(CELL_SB3);
  else if (ccounter < 8)
    cell(CELL_SB4);
  else if (ccounter < 14)
    cell(CELL_SB5);
  else if (ccounter < 20)
    cell(CELL_SB4);
  else if (ccounter < 26)
    cell(CELL_SB3);
  else if (ccounter < 22)
    cell(CELL_SB2);
  else if (ccounter < 30)
    cell(CELL_SB1);
  else
    ChangeAction(ACTID_NEUTRAL);
}

void CCharacterEL::act_att_sc() //立強
{
  if (ccounter == 5)
    StartAttack(ATTACKINFO_STAND_C);
  if (ccounter == 2)
    pefe_wind->Go(pdat->muki, pdat->x, pdat->y, 2);

  movex((30 - ccounter) * ccounter / 15.0 * 1.3);

  if (ccounter < 2)
    cell(CELL_SC1);
  else if (ccounter < 4)
    cell(CELL_SC2);
  else if (ccounter < 6)
    cell(CELL_SC3);
  else if (ccounter < 14)
    cell(CELL_SC4);
  else if (ccounter < 16)
    cell(CELL_SC3);
  else if (ccounter < 18)
    cell(CELL_SC2);
  else if (ccounter < 20)
    cell(CELL_SC1);
  else {
    BOOL mprev = pdat->muki;
    Furimuki();

    if (mprev == pdat->muki) {
      ChangeAction(ACTID_ATT_SCS1);
    } else {
      ChangeAction(ACTID_ATT_SCS2);
    }
  }
}

void CCharacterEL::act_att_scs1() {
  if (ccounter == 0) {
    pdat->vx = 10;
  }
  pdat->vx -= 0.5;
  if (pdat->vx < 0)
    pdat->vx = 0;
  movex(pdat->vx);

  if (ccounter < 3)
    cell(CELL_SCS1);
  else if (ccounter < 10)
    cell(CELL_SCS2);
  else if (ccounter < 15)
    cell(CELL_SCS1);
  else
    ChangeAction(ACTID_NEUTRAL);
}

void CCharacterEL::act_att_scs2() {
  if (ccounter == 0) {
    pdat->vx = -10;
  }
  pdat->vx += 0.5;
  if (pdat->vx > 0)
    pdat->vx = 0;
  movex(pdat->vx);

  if (ccounter < 15)
    cell(CELL_SCS3);
  else
    ChangeAction(ACTID_NEUTRAL);
}

//**************************************************************
//　屈
//**************************************************************

void CCharacterEL::act_att_ca() //屈弱
{
  if (ccounter == 6) {
    pbul_hadou->GoA(pdat->muki, pdat->x + zurex(65), -40, 6, &aif[ATTACKINFO_CROUCH_A]);
  }

  if (ccounter < 2)
    cell(CELL_CA1);
  else if (ccounter < 4)
    cell(CELL_CA2);
  else if (ccounter < 12)
    cell(CELL_CA2);
  else if (ccounter < 18)
    cell(CELL_CA2);
  else if (ccounter < 24)
    cell(CELL_CA1);
  else
    ChangeAction(ACTID_CROUCH);
}

void CCharacterEL::act_att_cb() //屈中
{
  if (ccounter == 0)
    StartAttack(ATTACKINFO_CROUCH_B);

  if (ccounter < 2)
    cell(CELL_CB1);
  else if (ccounter < 4)
    cell(CELL_CB2);
  else if (ccounter < 6)
    cell(CELL_CB3);
  else if (ccounter < 8)
    cell(CELL_CB4);
  else if (ccounter < 14)
    cell(CELL_CB5);
  else if (ccounter < 20)
    cell(CELL_CB4);
  else if (ccounter < 26)
    cell(CELL_CB3);
  else if (ccounter < 22)
    cell(CELL_CB2);
  else if (ccounter < 30)
    cell(CELL_CB1);
  else
    ChangeAction(ACTID_CROUCH);
}

void CCharacterEL::act_att_cc() //屈強
{
  if (ccounter == 5)
    StartAttack(ATTACKINFO_CROUCH_C);

  if (ccounter < 1)
    cell(CELL_CC1);
  else if (ccounter < 2)
    cell(CELL_CC2);
  else if (ccounter < 3)
    cell(CELL_CC3);
  else if (ccounter < 4)
    cell(CELL_CC4);
  else if (ccounter < 5)
    cell(CELL_CC5);
  else if (ccounter < 10)
    cell(CELL_CC6);
  else if (ccounter < 15)
    cell(CELL_CC5);
  else if (ccounter < 20)
    cell(CELL_CC4);
  else if (ccounter < 25)
    cell(CELL_CC3);
  else if (ccounter < 30)
    cell(CELL_CC2);
  else if (ccounter < 35)
    cell(CELL_CC1);
  else
    ChangeAction(ACTID_CROUCH);
}

//**************************************************************
//　飛
//**************************************************************

void CCharacterEL::act_att_ja() //飛弱
{
  if (ccounter == 6) {
    pbul_hadou->GoA(pdat->muki, pdat->x + zurex(80), -55, 6, &aif[ATTACKINFO_JAMP_A]);
  }

  if (ccounter < 2)
    cell(CELL_SA1);
  else if (ccounter < 4)
    cell(CELL_SA2);
  else if (ccounter < 6)
    cell(CELL_SA3);
  else if (ccounter < 10)
    cell(CELL_SA2);
  else if (ccounter < 15)
    cell(CELL_SA1);
  else
    ChangeAction(ACTID_RAKKA2);

  JiyuuRakka(2, TRUE, ACTID_NEUTRAL);
}

void CCharacterEL::act_att_jb() //飛中
{
  if (ccounter == 0)
    StartAttack(ATTACKINFO_JAMP_B);

  if (ccounter < 2)
    cell(CELL_SB1);
  else if (ccounter < 4)
    cell(CELL_SB2);
  else if (ccounter < 6)
    cell(CELL_SB3);
  else if (ccounter < 8)
    cell(CELL_SB4);
  else if (ccounter < 14)
    cell(CELL_SB5);
  else if (ccounter < 20)
    cell(CELL_SB4);
  else if (ccounter < 26)
    cell(CELL_SB3);
  else if (ccounter < 22)
    cell(CELL_SB2);
  else if (ccounter < 30)
    cell(CELL_SB1);
  else
    ChangeAction(ACTID_RAKKA2);

  JiyuuRakka(2, TRUE, ACTID_NEUTRAL);
}

void CCharacterEL::act_att_jc() //飛強
{
  if (ccounter == 5)
    StartAttack(ATTACKINFO_JAMP_C);

  if (ccounter < 1)
    cell(CELL_JC1);
  else if (ccounter < 2)
    cell(CELL_JC2);
  else if (ccounter < 3)
    cell(CELL_JC3);
  else if (ccounter < 4)
    cell(CELL_JC4);
  else if (ccounter < 5)
    cell(CELL_JC5);
  else if (ccounter < 10)
    cell(CELL_JC6);
  else if (ccounter < 15)
    cell(CELL_JC5);
  else if (ccounter < 20)
    cell(CELL_JC4);
  else if (ccounter < 25)
    cell(CELL_JC3);
  else if (ccounter < 30)
    cell(CELL_JC2);
  else if (ccounter < 35)
    cell(CELL_JC1);
  else
    ChangeAction(ACTID_RAKKA2);

  JiyuuRakka(2, TRUE, ACTID_NEUTRAL);
}

//**************************************************************
//　投
//**************************************************************

void CCharacterEL::act_nage1() {
  DrawFront();
  double dsx = GetDisplayCenter_X();

  if ((pdat->x - dsx) * (pdat->x - dsx) < 270 * 270 || pdat->counter < 5) {
    pdat->cnow = CELL_NAGE1 + (pdat->counter / 2) % 2;
    movex(20);
  } else
    pdat->aid = ACTID_NAGEEND;

  if (pdat->counter == 0) {
    AddEffect(EFCTID_MARKC, (int)pdat->x + zurex(50), -55, 0);
    PlaySysSound(SYSTEMSOUND_CATCH);
  }
}

void CCharacterEL::act_nage2() {
  DrawFront();
  double dsx = GetDisplayCenter_X();

  if ((pdat->x - dsx) * (pdat->x - dsx) < 270 * 270 || pdat->counter < 5) {
    pdat->cnow = CELL_NAGE1 + (pdat->counter / 2) % 2;
    movex(20);
  } else
    pdat->aid = ACTID_NAGEEND;

  if (pdat->counter == 0) {
    AddEffect(EFCTID_MARKC, (int)pdat->x + zurex(50), (int)pdat->y - 55, 0);
    PlaySysSound(SYSTEMSOUND_CATCH);
  }
}

DWORD CCharacterEL::eact_nage(GOBJECT *pedat) {
  double dsx = GetDisplayCenter_X();
  if ((pdat->x - dsx) * (pdat->x - dsx) < 270 * 270 || pdat->counter < 5) {
    pedat->cnow = DCELL_DAMAGES1;
    pedat->x = pdat->x + zurex(50);
    pedat->y = pdat->y;
    pedat->muki = !pdat->muki;
    return (0);
  } else {
    StartAttack(ATTACKINFO_NAGE);
    AddDamage(pedat->id, (int)pedat->x, (int)pdat->y - 100);
    return (ACTID_FUTTOBI2);
  }
}

void CCharacterEL::act_nageend() {
  if (pdat->counter == 0) {
    pdat->vx = -10;
    pdat->vy = -4;
  }
  movex(pdat->vx);
  pdat->y += pdat->vy;
  pdat->cnow = CELL_DASHB1 + pdat->counter % 2;

  if (pdat->counter > 20) {
    pdat->aid = ACTID_RAKKA2;
  }
}
