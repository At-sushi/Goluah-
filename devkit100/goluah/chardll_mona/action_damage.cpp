
/*==============================================================

        行動関数（敵側行動・投げ以外）

================================================================*/
#include "character.h"

#define emovex(dx) pedat->x += (pedat->muki) ? -dx : dx

/*--------------------------------------------------------------
        エリアル・浮かせヒット
----------------------------------------------------------------*/
void CCharacter::eact_aerialpop(GOBJECT *pedat) {
  if (pedat->counter == 0) {
    pedat->vx = -2;
    pedat->vy = -3;
  }
  emovex(pedat->vx);
  pedat->y += pedat->vy;
  pedat->vy += pedat->vy < 0 ? 0.4 : 1.0;

  if (pedat->counter < 8) {
    pedat->cnow = DCELL_DAMAGEJ;
  } else if (pedat->counter < 12) {
    pedat->cnow = DCELL_RAKKA1;
  } else if (pedat->counter < 16) {
    pedat->cnow = DCELL_RAKKA2;
  }

  if (pedat->y > 0) {
    pedat->y = 0;
    pedat->aid = ACTID_DOWN2;
  }
}

/*--------------------------------------------------------------
        エリアル・空中ヒット
----------------------------------------------------------------*/
void CCharacter::eact_aerialhit1(GOBJECT *pedat) {
  if (!(pedat->aid & ACTID_KUCYU)) {
    pedat->aid = ACTID_DAMAGE1;
    return;
  }

  if (pedat->counter == 0) {
    pedat->vy -= 0.7;
    if (pedat->vy > 0)
      pedat->vy = -1.5;
  }
  emovex(pedat->vx);
  pedat->y += pedat->vy;
  pedat->vy += pedat->vy < 3 ? 0.4 : 1.0;

  if (pedat->counter < 8) {
    pedat->cnow = DCELL_DAMAGEJ;
  } else if (pedat->counter < 12) {
    pedat->cnow = DCELL_RAKKA1;
  } else if (pedat->counter < 16) {
    pedat->cnow = DCELL_RAKKA2;
  }

  if (pedat->y > 0) {
    pedat->y = 0;
    pedat->aid = ACTID_DOWN2;
  }
}

void CCharacter::eact_aerialhit2(GOBJECT *pedat) {
  if (!(pedat->aid & ACTID_KUCYU)) {
    pedat->aid = ACTID_DAMAGE2;
    return;
  }

  if (pedat->counter == 0) {
    pedat->vy -= 0.7;
    if (pedat->vy > 0)
      pedat->vy = -1.5;
  }
  emovex(pedat->vx);
  pedat->y += pedat->vy;
  pedat->vy += pedat->vy < 3 ? 0.4 : 1.0;

  if (pedat->counter < 8) {
    pedat->cnow = DCELL_DAMAGEJ;
  } else if (pedat->counter < 12) {
    pedat->cnow = DCELL_RAKKA1;
  } else if (pedat->counter < 16) {
    pedat->cnow = DCELL_RAKKA2;
  }

  if (pedat->y > 0) {
    pedat->y = 0;
    pedat->aid = ACTID_DOWN2;
  }
}

void CCharacter::eact_aerialhit3(GOBJECT *pedat) {
  if (!(pedat->aid & ACTID_KUCYU)) {
    pedat->aid = ACTID_DAMAGE3;
    return;
  }

  if (pedat->counter == 0) {
    pedat->vy -= 0.7;
    if (pedat->vy > 0)
      pedat->vy = -1.5;
  }
  emovex(pedat->vx);
  pedat->y += pedat->vy;
  pedat->vy += pedat->vy < 3 ? 0.4 : 1.0;

  if (pedat->counter < 8) {
    pedat->cnow = DCELL_DAMAGEJ;
  } else if (pedat->counter < 12) {
    pedat->cnow = DCELL_RAKKA1;
  } else if (pedat->counter < 16) {
    pedat->cnow = DCELL_RAKKA2;
  }

  if (pedat->y > 0) {
    pedat->y = 0;
    pedat->aid = ACTID_DOWN2;
  }
}
