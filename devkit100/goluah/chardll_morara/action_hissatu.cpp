
/*==============================================================

        行動関数（コマンド技）

================================================================*/
#include "character.h"

/*--------------------------------------------------------------
        ジェノサイド・アレ（弱）
----------------------------------------------------------------*/
//地上部分
void CCharacter::act_genocide1g() {
  if (pdat->counter == 0) {
    movex(45);
  }
  cell(CELL_GENO1);
  pdat->muteki = TRUE;

  if (pdat->counter > 2)
    ChangeAction(ACTID_GENOCIDE1J);
}

//空中部分
void CCharacter::act_genocide1j() {
  if (ccounter == 0)
    PlayMySound(wave_gcut);
  if (pdat->counter == 0) {
    pdat->vy = -8;
    pdat->vx = 0;
    StartAttack(ATTACKINFO_GENOCIDE1);

    if (efe_track) {
      efe_track->Startup(50.0f, 3, 5, PI * 0.25f, 0.8f, 0.12f, 80.0f, 0.00f, 80.0f);
    }
  }

  if (pdat->counter < 2)
    cell(CELL_GENO2);
  else if (pdat->counter < 4)
    cell(CELL_GENO3);
  else {
    if (pdat->vy < 2)
      cell(CELL_GENO4);
    else
      cell(CELL_GENO5);
  }
  JiyuuRakka(0.75, TRUE, ACTID_LANDINGG);
}

/*--------------------------------------------------------------
        ジェノサイド・アレ（中）
----------------------------------------------------------------*/
//地上部分
void CCharacter::act_genocide2g() {
  if (pdat->counter == 0) {
    movex(45);
  }
  cell(CELL_GENO1);
  pdat->muteki = TRUE;

  if (pdat->counter > 3)
    ChangeAction(ACTID_GENOCIDE2J);
}

//空中部分
void CCharacter::act_genocide2j() {
  if (ccounter == 0)
    PlayMySound(wave_gcut);
  if (pdat->counter == 0) {
    pdat->vy = -12;
    pdat->vx = 3;
    StartAttack(ATTACKINFO_GENOCIDE2);
    pdat->muteki = TRUE;

    if (efe_track) {
      efe_track->Startup(40.0f, 4, 6, PI * 0.25f, 0.8f, 0.1f, 90.0f, -0.02f, 90.0f);
    }
  }

  if (pdat->counter < 2)
    cell(CELL_GENO2);
  else if (pdat->counter < 4)
    cell(CELL_GENO3);
  else {
    if (pdat->vy < 2)
      cell(CELL_GENO4);
    else {
      cell(CELL_GENO5);
      pdat->muteki = FALSE;
    }
  }
  JiyuuRakka(0.75, TRUE, ACTID_LANDINGG);
}

/*--------------------------------------------------------------
        ジェノサイド・アレ（強）
----------------------------------------------------------------*/
//地上部分
void CCharacter::act_genocide3g() {
  pdat->muteki = TRUE;

  if (pdat->counter < 3) {
    movex(20);
    cell(CELL_BDASH7);
  } else
    ChangeAction(ACTID_GENOCIDE3J);
}

//空中部分
void CCharacter::act_genocide3j() {
  if (ccounter == 0)
    PlayMySound(wave_gcut);
  if (pdat->counter == 0) {
    pdat->vy = -18;
    pdat->vx = 6;
    StartAttack(ATTACKINFO_GENOCIDE3);
    movex(45);

    if (efe_track) {
      efe_track->Startup(40.0f, 5, 9, PI * 0.25f, 0.8f, 0.1f, 90.0f, -0.05f, 90.0f);
    }
  }

  if (pdat->counter < 2)
    cell(CELL_GENO1);
  else if (pdat->counter < 4)
    cell(CELL_GENO2);
  else if (pdat->counter < 6)
    cell(CELL_GENO3);
  else {
    if (pdat->vy < 2)
      cell(CELL_GENO4);
    else
      cell(CELL_GENO5);
  }
  JiyuuRakka(1, TRUE, ACTID_LANDINGG);
}

/*--------------------------------------------------------------
        ジェノサイド・アレ（着地時隙）
----------------------------------------------------------------*/
void CCharacter::act_landingg() {
  if (pdat->counter < 2)
    cell(CELL_LANDING1);
  else if (pdat->counter < 10)
    cell(CELL_LANDING2);
  else if (pdat->counter < 12)
    cell(CELL_LANDING1);
  else
    ChangeAction(ACTID_NEUTRAL);
}

/*--------------------------------------------------------------
        ジェノサイド・アレ（ガードキャンセル）
----------------------------------------------------------------*/
void CCharacter::act_gcancel1() {
  if (pdat->counter == 0)
    AddPowerGauge(-0.3);

  pdat->muteki = TRUE;
  pdat->nagerare = FALSE;

  pdat->alphamode = GBLEND_KASAN;
  if (pdat->counter > 2) {
    ChangeAction(ACTID_GENOCIDE1G);
    pdat->alphamode = 0;
  }
}

void CCharacter::act_gcancel2() {
  if (pdat->counter == 0)
    AddPowerGauge(-0.3);

  pdat->muteki = TRUE;
  pdat->nagerare = FALSE;

  pdat->alphamode = GBLEND_KASAN;
  if (pdat->counter > 2) {
    ChangeAction(ACTID_GENOCIDE2G);
    pdat->alphamode = 0;
  }
}

void CCharacter::act_gcancel3() {
  if (pdat->counter == 0)
    AddPowerGauge(-0.3);

  pdat->muteki = TRUE;
  pdat->nagerare = FALSE;

  pdat->alphamode = GBLEND_KASAN;
  if (pdat->counter > 2) {
    ChangeAction(ACTID_GENOCIDE3G);
    pdat->alphamode = 0;
  }
}

/*--------------------------------------------------------------
        地走り（弱）
----------------------------------------------------------------*/
void CCharacter::act_chiba1() {
  if (ccounter == 0)
    PlayMySound(wave_chiwo);
  if (pdat->counter == 12) {
    bul_chiba->Startup(&aif[ATTACKINFO_CHIBA1], GOBJMSG_CHIBAHIT, GOBJMSG_CHIBAGUARD, pdat->muki, pdat->x,
                       pdat->y, 8);
  }

  if (pdat->counter < 3)
    cell(CELL_CHIBA1);
  else if (pdat->counter < 6)
    cell(CELL_CHIBA2);
  else if (pdat->counter < 9)
    cell(CELL_CHIBA3);
  else if (pdat->counter < 12)
    cell(CELL_CHIBA4);
  else if (pdat->counter < 15)
    cell(CELL_CHIBA5);
  else if (pdat->counter < 18)
    cell(CELL_CHIBA6);
  else if (pdat->counter < 21)
    cell(CELL_CHIBA7);
  else if (pdat->counter < 28)
    cell(CELL_CHIBA8);
  else if (pdat->counter < 35)
    cell(CELL_CHIBA9);
  else
    ChangeAction(ACTID_NEUTRAL);
}

/*--------------------------------------------------------------
        地走り（厨）
----------------------------------------------------------------*/
void CCharacter::act_chiba2() {
  if (ccounter == 0)
    PlayMySound(wave_chiwo);
  if (pdat->counter == 16) {
    bul_chiba->Startup(&aif[ATTACKINFO_CHIBA2], GOBJMSG_CHIBAHIT, GOBJMSG_CHIBAGUARD, pdat->muki, pdat->x,
                       pdat->y, 12);
  }

  if (pdat->counter < 5)
    cell(CELL_CHIBA1);
  else if (pdat->counter < 10)
    cell(CELL_CHIBA2);
  else if (pdat->counter < 13)
    cell(CELL_CHIBA3);
  else if (pdat->counter < 16)
    cell(CELL_CHIBA4);
  else if (pdat->counter < 18)
    cell(CELL_CHIBA5);
  else if (pdat->counter < 21)
    cell(CELL_CHIBA6);
  else if (pdat->counter < 24)
    cell(CELL_CHIBA7);
  else if (pdat->counter < 30)
    cell(CELL_CHIBA8);
  else if (pdat->counter < 36)
    cell(CELL_CHIBA9);
  else
    ChangeAction(ACTID_NEUTRAL);
}

/*--------------------------------------------------------------
        地走り（強）
----------------------------------------------------------------*/
void CCharacter::act_chiba3() {
  if (ccounter == 0)
    PlayMySound(wave_chiwo);
  if (pdat->counter == 22) {
    bul_chiba->Startup(&aif[ATTACKINFO_CHIBA3], GOBJMSG_CHIBAHIT, GOBJMSG_CHIBAGUARD, pdat->muki, pdat->x,
                       pdat->y, 16);
  }

  if (pdat->counter < 8)
    cell(CELL_CHIBA1);
  else if (pdat->counter < 16)
    cell(CELL_CHIBA2);
  else if (pdat->counter < 19)
    cell(CELL_CHIBA3);
  else if (pdat->counter < 22)
    cell(CELL_CHIBA4);
  else if (pdat->counter < 25)
    cell(CELL_CHIBA5);
  else if (pdat->counter < 28)
    cell(CELL_CHIBA6);
  else if (pdat->counter < 31)
    cell(CELL_CHIBA7);
  else if (pdat->counter < 40)
    cell(CELL_CHIBA8);
  else if (pdat->counter < 50)
    cell(CELL_CHIBA9);
  else
    ChangeAction(ACTID_NEUTRAL);
}

/*--------------------------------------------------------------
        ミニガン
----------------------------------------------------------------*/
void CCharacter::act_minigun() {
  if (ccounter == 0)
    PlayMySound(wave_gun);
  if (pdat->counter == 0) {
    AddPowerGauge(-1.0);
    AddEffect(EFCTID_CYOHI, (int)pdat->x, (int)pdat->y - 50, 30);
  }

  if (pdat->counter < 4)
    cell(CELL_MINIGUN1);
  else if (pdat->counter < 8)
    cell(CELL_MINIGUN2);
  else if (pdat->counter < 12)
    cell(CELL_MINIGUN3);
  else if (pdat->counter < 250) {
    cell(CELL_MINIGUN4 + (pdat->counter % 3));
    if (pdat->counter % 4 == 0) {
      bul_minigun[(pdat->counter / 4) % NUM_MINIGUN]->Go(pdat->muki, pdat->x + zurex(0), -120 + rand() % 120,
                                                         150);
      PlayMySound(((pdat->counter / 4) % 2) ? wave_gunfire : wave_gunfire2);
    }
    if (pdat->counter % 3 == 0) {
      efe_minigun->Go(pdat->muki, pdat->x + zurex(160), -50, 0);
    }
  } else
    ChangeAction(ACTID_MINIGUN2);
}

void CCharacter::act_minigun2() //終了隙
{
  if (pdat->counter < 10)
    cell(CELL_MINIGUN3);
  else if (pdat->counter < 20)
    cell(CELL_MINIGUN2);
  else if (pdat->counter < 30)
    cell(CELL_MINIGUN1);
  else
    ChangeAction(ACTID_NEUTRAL);
}

/*--------------------------------------------------------------
        ジェノサイド・アレ（追加入力）
----------------------------------------------------------------*/
void CCharacter::act_genocide_up() {
  if (pdat->counter == 0) {
    pdat->vy -= 8;
    pdat->vx = 6;
    StartAttack(ATTACKINFO_GENOCIDE_UP);
    movex(45);

    if (efe_track) {
      efe_track->Startup(40.0f, 3, 4, PI * 0.25f, 0.8f, 0.1f, 80.0f, -0.05f, 80.0f);
    }

    if (ccounter == 0)
      PlayMySound(wave_gcut_up);
  }

  if (pdat->counter < 2)
    cell(CELL_GENOT1);
  else if (pdat->counter < 4)
    cell(CELL_GENOT2);
  else if (pdat->counter < 10)
    cell(CELL_GENOT3);
  else if (pdat->counter < 20)
    cell(CELL_GENOT1);
  //	else ChangeAction( ACTID_RAKKA2 );

  JiyuuRakka(1, TRUE, ACTID_LANDINGG);
}

void CCharacter::act_genocide_down() {
  if (pdat->counter == 0) {
    pdat->vy -= 3;
    pdat->vx *= 1.5;
    StartAttack(ATTACKINFO_GENOCIDE_DOWN);
    if (efe_track) {
      efe_track->Startup(40.0f, 3, 8, PI * 0.25f, 0.8f, 0.05f, 120.0f, 0.00f, 50.0f, TRUE);
    }

    if (ccounter == 0)
      PlayMySound(wave_gcut_down);
  }

  if (ccounter < 2)
    cell(CELL_SC2_1);
  else if (ccounter < 4)
    cell(CELL_SC2_2);
  else if (ccounter < 6)
    cell(CELL_SC2_3);
  else if (ccounter < 8)
    cell(CELL_SC2_4);
  else if (ccounter < 10)
    cell(CELL_SC2_5);
  else if (ccounter < 12)
    cell(CELL_SC2_6);
  else if (ccounter < 14)
    cell(CELL_SC2_7);
  else
    cell(CELL_SC2_8);

  JiyuuRakka(1, TRUE, ACTID_LANDINGG);
}
