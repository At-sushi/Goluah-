/*==============================================================

        飛び道具

================================================================*/
#include "character.h"

#define PI (3.141592f)

#define WHITELIGHT_LINES 5
#define WHITELIGHT_LIFETIME 30
MYVERTEX3D p_vbwl[WHITELIGHT_LIFETIME][WHITELIGHT_LINES * 3];

void CCharacter::InitBullets() {
  b_nori = b_nori2 = NULL;

  BULLETINFO_A bul;
  int bulcr[4], bulcd[2];

  bul.cid = pdat->id;
  bul.type = BULLETA_VSHUMAN | BULLETA_VSBULLET;
  bulcr[0] = CELL_NORI1;
  bulcr[1] = CELL_NORI2;
  bulcr[2] = CELL_NORI3;
  bulcr[3] = -1;
  bulcd[0] = CELL_NORI3;
  bulcd[1] = -1;
  bul.cell_run = bulcr;
  bul.cell_dis = bulcd;
  bul.spd_run = 1;
  bul.spd_dis = 4;
  bul.vx = 30;
  bul.vy = 0;
  bul.dur = 120;
  bul.state = 0;
  bul.info_b = NULL;
  bul.muki = pdat->muki;
  bul.y = -50;
  bul.vx = 35;
  bul.vy = 0;
  bul.ax = 0;
  bul.ay = 0;
  bul.dur = 80;
  bul.atk.damage = 20;
  bul.atk.dur = 90;
  bul.atk.pg_hit = 0.04;
  bul.atk.pg_guard = 0.02;
  bul.atk.hit = HITINFO_MARK4 | HITINFO_REACT2 | HITINFO_SNDHIT3 | HITINFO_SIV3;
  bul.atk.guard = GUARDINFO_REACT2 | GUARDINFO_SIV3;
  bul.atk.kezuri = 1;
  bul.atk.muki = pdat->muki;
  //通常海苔
  b_nori = new CClassicalBullet(this, NULL, &bul);

  bul.muki = pdat->muki;
  bul.y = -50;
  bul.vx = 25;
  bul.vy = 0;
  bul.ax = 0;
  bul.ay = 0;
  bul.dur = 80;
  bul.atk.damage = 10;
  bul.atk.dur = 90;
  bul.atk.pg_hit = 0.0;
  bul.atk.pg_guard = 0.0;
  bul.atk.hit = HITINFO_MARK4 | HITINFO_REACT3A | HITINFO_SNDHIT3 | HITINFO_STOP;
  bul.atk.guard = GUARDINFO_REACT3 | GUARDINFO_SIV3;
  bul.atk.kezuri = 5;
  bul.atk.muki = pdat->muki;
  //超必海苔

  b_nori2 = new CClassicalBullet(this, NULL, &bul);
  b_nori2->SetHitMessage(GOBJMSG_FESTIVAL_NORI_HIT, 0);

  //頂点設定
  {
    int i, j;

    float nagasa_wariai;
    float nagasa;
    double rot;
    double rot_prv;
    double sita; //θ

    double ng_s_min = 0.2; //この長さ以下でθ一定
    double ng_s_max = 1.0; //この長さ以上でθ一定
    double sita_max = 80;  //θmax

    MYVERTEX3D *pvb[3];

    srand(timeGetTime());

    for (j = 0; j < WHITELIGHT_LINES; j++) {
      nagasa_wariai = (float)(1.0 + (rand() % 100) * 0.004);
      rot_prv = (360.0 / WHITELIGHT_LINES) * j; //初期回転角
      for (i = 0; i < WHITELIGHT_LIFETIME; i++) {

        pvb[0] = &p_vbwl[i][j * 3];
        pvb[1] = &p_vbwl[i][j * 3 + 1];
        pvb[2] = &p_vbwl[i][j * 3 + 2];

        //長さ
        if (i < 10)
          nagasa = 0.8f - 0.03f * i;
        else
          nagasa = (0.8f - 0.03f * 10) + 0.04f * (i - 10);
        nagasa *= nagasa_wariai;

        //角度
        if (nagasa > ng_s_max)
          sita = 0;
        else if (nagasa < ng_s_min)
          sita = sita_max;
        else {
          sita =
              (sita_max / (ng_s_max - ng_s_min)) * nagasa - (ng_s_max * (sita_max / (ng_s_max - ng_s_min)));
        }

        //回転角度（長さから決める）
        rot = rot_prv + (0.8 - nagasa) * 7;

        //ざひょー
        pvb[0]->z = pvb[1]->z = pvb[2]->z = 0;
        pvb[0]->x = pvb[0]->y = 0;
        pvb[1]->x = nagasa * (float)sin((rot + sita) / 360 * 2 * PI);
        pvb[1]->y = nagasa * (float)cos((rot + sita) / 360 * 2 * PI);
        pvb[2]->x = nagasa * (float)sin((rot - sita) / 360 * 2 * PI);
        pvb[2]->y = nagasa * (float)cos((rot - sita) / 360 * 2 * PI);

        //色
        pvb[0]->color = pvb[1]->color = pvb[2]->color = 0xFFAAAAAA;
        pvb[1]->color &= 0x00FFFFFF;
        pvb[2]->color &= 0x00FFFFFF;

        // UV座標
        pvb[0]->tu = pvb[1]->tu = pvb[2]->tu = 0;
        pvb[0]->tv = pvb[1]->tv = pvb[2]->tv = 0;

        rot_prv = rot;
      }
    }
  }
  b_wlight = new CWassoiLight(this);
}

/*---------------------------------------------------------
        煌く何か
-----------------------------------------------------------*/
CWassoiLight::CWassoiLight(CCharacterBase *parent) : CBulletBase(parent) {
  RemoveProperty(GOBJFLG_ATTACK);
  base_z = ZZAHYO_EFFECT2;
}

void CWassoiLight::act_run() {
  UINT i;

  if (pdat->counter == 0) {
    for (i = 0; i < WLIGHT_NUM; i++) {
      info[i].isCreated = FALSE;
    }
  }

  if (pdat->counter % 2 == 0 && pdat->counter < 60) {
    GOBJECT *tgt = GetInfo(tgt_oid);
    if (tgt) {
      info[(pdat->counter / 2) % WLIGHT_NUM].Start((float)tgt->x, (float)tgt->y);
    }
  }

  for (i = 0; i < WLIGHT_NUM; i++) {
    info[i].Update();
  }

  if (pdat->counter > 60 + WHITELIGHT_LIFETIME) {
    Hide();
  }
}

void CWassoiLight::act_bomb() { Hide(); }

DWORD CWassoiLight::Draw() {
  LPDIRECT3DDEVICE8 dev = GetD3DDevice();
  UINT i;

  if (pdat->aid == CBB_STATE_RUN && dev) {
    SetBlend(GBLEND_KASAN);
    for (i = 0; i < WLIGHT_NUM; i++) {
      info[i].Draw(dev);
    }
    SetBlend(0);
  }
  return (TRUE);
}

void CWassoiLight::CWassoiLightInfo::Start(float tgt_x, float tgt_y) {
  x = tgt_x + rand() % 120 - 60;
  y = tgt_y - rand() % 80;
  isCreated = TRUE;
  counter = 0;
  sca = 0.1f + (rand() % 100) * 0.002f;
}

void CWassoiLight::CWassoiLightInfo::Update() {
  y -= 3;
  counter++;
}

void CWassoiLight::CWassoiLightInfo::Draw(LPDIRECT3DDEVICE8 dev) {
  if (!isCreated)
    return;

  D3DXMATRIX matw, mats, matt;

  if (counter < WHITELIGHT_LIFETIME) {
    dev->SetTexture(0, NULL);
    D3DXMatrixScaling(&mats, sca, sca, sca);
    D3DXMatrixTranslation(&matt, (x) / 240.0f, (y) / 240.0f, ZZAHYO_EFFECT2);
    matw = mats * matt;
    dev->SetTransform(D3DTS_WORLD, &matw);
    dev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, WHITELIGHT_LINES, p_vbwl[counter], sizeof(MYVERTEX3D));
  }
}
