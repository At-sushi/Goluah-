/*==============================================================

        行動関数(必殺)

================================================================*/
#include "character.h"

//必殺==========================================================

//さぁ！1さん
void CCharacter::act_sah1() {
  if (pdat->counter == 0) {
    pdat->vy = -16;
    pdat->vx = 6;
    pdat->muteki = TRUE;
    PlayMySound(wave_sah);
    sah_sw = sah_sw ? FALSE : TRUE;
  }
  pdat->y += pdat->vy;
  pdat->vy++;
  movex(pdat->vx);

  if (pdat->vy > 0)
    pdat->aid = ACTID_SAHS;

  pdat->cnow = CELL_SAH;

  if (pdat->counter % 3 == 0) {
    StartAttack(sah_sw ? ATTACKINFO_SAH1A : ATTACKINFO_SAH2A);
  }
}

void CCharacter::act_sah2() {
  if (pdat->counter == 0) {
    pdat->vy = -20;
    pdat->vx = 6;
    pdat->muteki = TRUE;
    PlayMySound(wave_sah);
    sah_sw = sah_sw ? FALSE : TRUE;
  }
  pdat->y += pdat->vy;
  pdat->vy++;
  movex(pdat->vx);

  if (pdat->vy > 0)
    pdat->aid = ACTID_SAHS;

  pdat->cnow = CELL_SAH;

  if (pdat->counter % 3 == 0) {
    StartAttack(sah_sw ? ATTACKINFO_SAH1B : ATTACKINFO_SAH2B);
  }
}

void CCharacter::act_sah3() {
  if (pdat->counter == 0) {
    pdat->vy = -24;
    pdat->vx = 6;
    pdat->muteki = TRUE;
    PlayMySound(wave_sah);
    sah_sw = sah_sw ? FALSE : TRUE;
  }
  pdat->y += pdat->vy;
  pdat->vy++;
  movex(pdat->vx);

  if (pdat->vy > 0)
    pdat->aid = ACTID_SAHS;

  pdat->cnow = CELL_SAH;

  if (pdat->counter % 3 == 0) {
    StartAttack(sah_sw ? ATTACKINFO_SAH1C : ATTACKINFO_SAH2C);
  }
}

void CCharacter::act_sahs() {
  pdat->y += pdat->vy;
  pdat->vy += (pdat->counter % 3) % 2;

  pdat->cnow = CELL_WALK1;
  if (pdat->y > 0) {
    pdat->y = 0;
    pdat->aid = ACTID_SAH_LANDING;
    Furimuki();
  }
}

void CCharacter::act_sah_landing() {
  if (ccounter < 5) {
    cell(CELL_LANDING);
  } else
    ChangeAction(ACTID_NEUTRAL);
}

//コレガ　アイ　ノ　カタチ
void CCharacter::act_fig1() {
  if (pdat->counter < 3)
    pdat->cnow = CELL_FIG1;
  else if (pdat->counter < 6) {
    pdat->cnow = CELL_FIG2;
  } else if (pdat->counter < 9) {
    pdat->cnow = CELL_FIG3;
  } else if (pdat->counter < 12) {
    pdat->cnow = CELL_FIG4;
  } else if (pdat->counter < 15) {
    pdat->cnow = CELL_FIG5;
  } else if (pdat->counter < 40) {
    pdat->cnow = CELL_FIG6;
  } else {
    pdat->aid = ACTID_NEUTRAL;
  }

  if (pdat->counter == 1) {
    StartAttack(ATTACKINFO_FIG);
    GoHeart();
    PlayMySound(wave_fig);
  }
}

void CCharacter::act_fig2() { act_fig1(); } //中・性能おんなじ
void CCharacter::act_fig3() { act_fig1(); } //強・性能おんなじ

//怒りをぶつける投げ
void CCharacter::act_ikari() {
  DrawFront();

  if (pdat->counter == 0) {
    PlayMySound(wave_ikari1);
  }

  if (pdat->counter < 20) {
    pdat->cnow = CELL_NAGE1;
    pdat->y = 0;
  } else if (pdat->counter < 24) {
    pdat->cnow = CELL_IKARI1;
  } else if (pdat->counter < 40) {
    pdat->cnow = CELL_IKARI2;
  } else if (pdat->counter < 42) {
    pdat->cnow = CELL_JAMP;
  } else if (pdat->counter < 55) {
    pdat->cnow = CELL_JAMP1;
    movex(2);
    pdat->y -= 12;
    pdat->vy = -12;
  }                              //ジャンプして追いかけ
  else if (pdat->counter < 90) { //掴んで落下
    pdat->cnow = CELL_IKARI3;
    pdat->y += pdat->vy;
    pdat->vy++;
  } else if (pdat->counter < 150) {
    pdat->cnow = CELL_IKARI4;
    pdat->y = 0;
    pdat->vy = 0;
  } else {
    pdat->aid = ACTID_NEUTRAL;
  }

  if (pdat->counter == 0) {
    AddEffect(EFCTID_MARKC, (int)pdat->x + zurex(50), -100, 0);
    PlaySysSound(SYSTEMSOUND_CATCH);
  }
}

DWORD CCharacter::act_ikarie(GOBJECT *pedat) {
  if (pdat->counter == 24) {
    StartAttack(ATTACKINFO_IKARI1);
    AddDamage(pedat->id, (int)pdat->x + zurex(50), -150);
  }
  if (pdat->counter == 90) {
    StartAttack(ATTACKINFO_IKARI2);
    AddDamage(pedat->id, (int)pdat->x + zurex(10), -200);
  }

  if (pdat->counter < 24) {
    pedat->cnow = DCELL_NAGERARE1;
    pedat->x = pdat->x + zurex(50);
    pedat->y = -100;
    pedat->muki = !pdat->muki;
    return (0);
  } else if (pdat->counter < 55) {
    pedat->cnow = DCELL_NAGERARE1;
    pedat->x = pdat->x + zurex(50);
    pedat->y -= 7;
    pedat->muki = !pdat->muki;
    return (0);
  } else if (pdat->counter < 90) {
    pedat->cnow = DCELL_NAGERARE1;
    pedat->rot = 90;
    pedat->x = pdat->x + zurex(10);
    pedat->y = pdat->y - 200;
    pedat->muki = !pdat->muki;
    return (0);
  } else {
    pedat->muki = pdat->muki;
    PlayMySound(wave_ikari2);
    return (ACTID_FUTTOBI);
  }

  //なに？
  return (0);
}

//「1さんがあっちに逃げたぞ！」-----------------------------------------------------------------

void CCharacter::act_escape() {
  if (pdat->counter == 0) {
    AddPowerGauge(-1.0);
    PlayMySound(wave_escape);
    AddEffect(EFCTID_CYOHI, (int)pdat->x + zurex(100), (int)pdat->y - 170, 30);
    AddEffect(EFCTID_COMEON, (int)pdat->x, (int)pdat->y - 200, pdat->muki);
  }

  pdat->cnow = CELL_ESCAPE;
  if (pdat->counter > 30)
    pdat->aid = ACTID_NEUTRAL;

  int i;

  if (pdat->counter == 0) {
    i = 0;
    clabul_fola[i]->Go(pdat->muki, pdat->muki ? 800 : -800, 0);
    clabul_folb[i]->Go(pdat->muki, pdat->muki ? 800 : -800, 0);
    clabul_folc[i]->Go(pdat->muki, pdat->muki ? 800 : -800, 0);
    clabul_fold[i]->Go(pdat->muki, pdat->muki ? 800 : -800, 0);
    clabul_fole[i]->Go(pdat->muki, pdat->muki ? 800 : -800, 0);
  }

  if (pdat->counter == 12) {
    i = 1;
    clabul_fola[i]->Go(pdat->muki, pdat->muki ? 850 : -850, 0);
    clabul_folb[i]->Go(pdat->muki, pdat->muki ? 850 : -850, 0);
    clabul_folc[i]->Go(pdat->muki, pdat->muki ? 850 : -850, 0);
    clabul_fold[i]->Go(pdat->muki, pdat->muki ? 850 : -850, 0);
    clabul_fole[i]->Go(pdat->muki, pdat->muki ? 850 : -850, 0);
  }

  if (pdat->counter == 24) {
    i = 2;
    clabul_fola[i]->Go(pdat->muki, pdat->muki ? 900 : -900, 0);
    clabul_folb[i]->Go(pdat->muki, pdat->muki ? 900 : -900, 0);
    clabul_folc[i]->Go(pdat->muki, pdat->muki ? 900 : -900, 0);
    clabul_fold[i]->Go(pdat->muki, pdat->muki ? 900 : -900, 0);
    clabul_fole[i]->Go(pdat->muki, pdat->muki ? 900 : -900, 0);
    for (i = 0; i < 6; i++) {
      if (clabul_folx[i])
        clabul_folx[i]->Go(pdat->muki, pdat->muki ? 1000 : -1000, 0);
    }
  }
}
