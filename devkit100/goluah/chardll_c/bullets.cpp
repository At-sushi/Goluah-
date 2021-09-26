
/*==============================================================

        飛び道具定義

================================================================*/
#include "character.h"
#include <math.h>

void CCharacter::InitBullets() {
  pbul_ocya = new COcya(this, aif[ATTACKINFO_OCYA]);
  pbul_jien = new CJien(this, aif[ATTACKINFO_JIEN]);
  pbul_aroe = new CAroe(this, aif[ATTACKINFO_AROE]);
  pbul_moukoneeyo = new CMoukoneeyo(this, aif[ATTACKINFO_MOUKONEEYO]);
  for (UINT i = 0; i < MISSILE_NUM; i++) {
    pbul_miss[i] = new CMissile(this, aif[ATTACKINFO_MISSILE]);
  }
  pbul_handg = new CHandg(this, aif[ATTACKINFO_HANDG]);
  pbul_craymore = new CCraymore(this, aif[ATTACKINFO_CRAYMORE]);
  pbul_rifle = new CRifle(this, aif[ATTACKINFO_RIFLE]);
  pbul_rocket = new CRocket(this, aif[ATTACKINFO_ROCKET]);
  pbul_firethrower = new CFireThrower(this, aif[ATTACKINFO_FIRETHROWER]);
  pbul_shotgun = new CShotgun(this, aif[ATTACKINFO_SHOT]);
  pbul_lantern = new CLantern(this, aif[ATTACKINFO_AGI]);
  pbul_comeon = new CComeon(this);
  pbul_httplaser = new CHttpLaser(this, aif[ATTACKINFO_HTTPLASER]);

  pbul_moyaA = new CMoyamoyaA(this);
  pbul_moyaB = new CMoyamoyaB(this);
  pbul_moyaC = new CMoyamoyaC(this);
  pbul_moya = pbul_moyaA;

  pbul_eraser99 = new CEraser99(this, aif[ATTACKINFO_ERASER99]);
  pbul_eraser99->SetRocket(pbul_rocket);

  pbul_cutin = new CCutin(this);
  pbul_blood = new CBlood(this);

  pbul_moyaA->Quick(); // 副低
  pbul_moyaB->Quick();
  pbul_moyaC->Quick();
  if (m_opt_quickA)
    pbul_moyaA->Quick();
  if (m_opt_quickB)
    pbul_moyaB->Quick();
  if (m_opt_quickC)
    pbul_moyaC->Quick();
  if (m_opt_crazy) {
    pbul_moyaA->Quick();
    pbul_moyaB->Quick();
    pbul_moyaC->Quick();
  }
}

void CCharacter::ArekoreCancel() {
  pbul_moya->ForceStop();
  ChangeAction(ACTID_NEUTRAL);
}

void CCharacter::ArekoreRelease() {
  MoyaType type = Moya_Invalid;
  if (pbul_moya) {
    type = pbul_moya->GetType();
    pbul_moya->ForceEnd();
  }
  switch (type) {
  case Moya_Ocya:
    ChangeAction(ACTID_POI_OCYA);
    break;
  case Moya_Jien:
    ChangeAction(ACTID_POI_JIEN);
    break;
  case Moya_Aroe:
    ChangeAction(ACTID_POI_AROE);
    break;
  case Moya_MissileH:
    ChangeAction(ACTID_POI_MISSILE_H);
    break;
  case Moya_Moukoneeyo:
    ChangeAction(ACTID_POI_MOUKONEEYO);
    break;
  case Moya_MissileSP:
    ChangeAction(ACTID_POI_MISSILE_SP);
    break;
  case Moya_Handg:
    ChangeAction(ACTID_POI_HANDG);
    break;
  case Moya_Craymore:
    ChangeAction(ACTID_POI_CRAYMORE);
    break;
  case Moya_MissileV:
    ChangeAction(ACTID_POI_MISSILE_V);
    break;
  case Moya_FireThrow:
    ChangeAction(ACTID_POI_FIRETHROW);
    break;
  case Moya_Shot:
    ChangeAction(ACTID_POI_SHOT);
    break;
  case Moya_Rifle:
    ChangeAction(ACTID_POI_RIFLE);
    break;
  case Moya_Rocket:
    ChangeAction(ACTID_POI_ROCKET);
    break;
  case Moya_Lantern:
    ChangeAction(ACTID_POI_LANTERN);
    break;
  case Moya_Hellfire:
    ChangeAction(ACTID_POI_HELLFIRE);
    break;
  case Moya_HttpLaser:
    ChangeAction(ACTID_POI_HTTPLASER);
    break;
  case Moya_Eraser99:
    ChangeAction(ACTID_POI_ERASER99);
    break;
  default:
    ChangeAction(ACTID_NEUTRAL);
    break;
  }
}

/*---------------------------------------------------------------
        お茶
-----------------------------------------------------------------*/

COcya::COcya(CCharacterBase *parent, ATTACKINFO &atk) : CBulletBase(parent) {
  atkinfo = atk;
  base_z = ZZAHYO_BULLET3;
}

void COcya::act_run() {
  movex(pdat->vx);
  pdat->y += pdat->vy;
  pdat->vy += 1;

  if (pdat->y > 0) {
    pdat->y = 0;
    Bomb();
  }

  pdat->kougeki = TRUE;
  cell(CELL_OCYA1 + (ccounter / 2) % 8);
}

void COcya::act_bomb() {
  if (ccounter < 3)
    cell(CELL_OCYAD1);
  else if (ccounter < 6)
    cell(CELL_OCYAD2);
  else if (ccounter < 9)
    cell(CELL_OCYAD3);
  else
    Hide();
}

/*---------------------------------------------------------------
        ジサクジエン
-----------------------------------------------------------------*/

CJien::CJien(CCharacterBase *parent, ATTACKINFO &atk) : CBulletBase(parent) {
  atkinfo = atk;
  base_z = ZZAHYO_BULLET3;
}

void CJien::act_run() {

  pdat->y += pdat->vy;
  pdat->vy += 1;

  if (pdat->y > 0) {
    pdat->y = 0;
    pdat->vy = -10;
    if (ccounter > 80)
      Bomb();

    cell(CELL_JISAKU1);
  } else {
    movex(pdat->vx);
    cell(CELL_JISAKU2);
  }

  pdat->kougeki = TRUE;
}

void CJien::act_bomb() {
  if (ccounter < 2)
    cell(CELL_JISAKUD1);
  else if (ccounter < 4)
    cell(CELL_JISAKUD2);
  else if (ccounter < 6)
    cell(CELL_JISAKUD3);
  else if (ccounter < 8)
    cell(CELL_JISAKUD4);
  else if (ccounter < 10)
    cell(CELL_JISAKUD5);
  else if (ccounter < 12)
    cell(CELL_JISAKUD6);
  else
    Hide();
}

/*---------------------------------------------------------------
        アロエリーナ
-----------------------------------------------------------------*/

CAroe::CAroe(CCharacterBase *parent, ATTACKINFO &atk) : CBulletBase(parent) {
  atkinfo = atk;
  base_z = ZZAHYO_BULLET3;
}

void CAroe::act_run() {
  movex(pdat->vx);
  pdat->y += pdat->vy;
  pdat->vy += 1;

  if (pdat->y > 0) {
    pdat->y = 0;
    Bomb();
  }
  cell(CELL_AROE);

  pdat->kougeki = TRUE;
}

void CAroe::act_bomb() {
  if (ccounter < 3)
    cell(CELL_AROED1);
  else if (ccounter < 6)
    cell(CELL_AROED2);
  else if (ccounter < 9)
    cell(CELL_AROED3);
  else
    Hide();
}

/*---------------------------------------------------------------
        モウコネエヨ
-----------------------------------------------------------------*/
CMoukoneeyo::CMoukoneeyo(CCharacterBase *parent, ATTACKINFO &atk) : CBulletBase(parent) {
  atkinfo = atk;
  base_z = ZZAHYO_BULLET3;
}

BOOL CMoukoneeyo::Go(BOOL muki, double x, double y, double vx, double vy) {
  pdat->muki = muki;

  pdat->x = muki ? 700 : -700;
  pdat->y = 0;
  pdat->vx = 10;

  atk_enabled[0] = atk_enabled[1] = atk_enabled[2] = atk_enabled[3] = TRUE;

  ChangeAction(CBB_STATE_RUN);
  return TRUE;
}

void CMoukoneeyo::act_run() {
  movex(pdat->vx);

  current_index = ccounter % 4;
  pdat->kougeki = atk_enabled[current_index];
  cell(CELL_MOUKONEEYO_HAN1 + current_index);
  dispcell = CELL_MOUKONEEYO1 + (ccounter / 3) % 3;

  BOOL end = FALSE;
  if (pdat->muki) {
    if (pdat->x < -800) {
      end = TRUE;
    }
  } else {
    if (pdat->x > 800) {
      end = TRUE;
    }
  }
  if (end)
    Hide();
}

DWORD CMoukoneeyo::TouchB(ATTACKINFO *info, BOOL hit) {
  atk_enabled[current_index] = FALSE;
  return CBulletBase::TouchB(info, hit);
}

DWORD CMoukoneeyo::Draw() {
  if (pdat->aid == CBB_STATE_RUN) {
    DWORD cold = pdat->cnow;
    pdat->cnow = dispcell;
    CellDraw(pdat);
    pdat->cnow = cold;
  }

  return TRUE;
}

/*---------------------------------------------------------------
        ミサイル
-----------------------------------------------------------------*/
CMissile::CMissile(CCharacterBase *parent, ATTACKINFO &atk) : CBulletBase(parent) {
  atkinfo = atk;
  AddProperty(GOBJFLG_NEEDTARGET);
  base_z = ZZAHYO_BULLET3;
}

void CMissile::act_run() {
  pdat->kougeki = TRUE;

  GOBJECT *pedat = GetInfo(pdat->eid);
  if (pedat && pdat->eid) {
    const float force = 0.8f;
    const float maxspd = 16;

    if (pedat->x - pdat->x > 0) {
      pdat->vx += force;
    } else {
      pdat->vx -= force;
    }

    if (pedat->y - pdat->y > 0) {
      pdat->vy += force;
    } else {
      pdat->vy -= force;
    }

    float spd = sqrtf((float)((pdat->vx * pdat->vx) + (pdat->vy * pdat->vy)));
    if (spd > maxspd) {
      pdat->vx /= spd / maxspd;
      pdat->vy /= spd / maxspd;
    }
  }

  pdat->x += pdat->vx;
  pdat->y += pdat->vy;

  float vxc = (float)pdat->vx;
  if (vxc < 0) {
    pdat->muki = TRUE;
    vxc *= -1.0;
  } else {
    pdat->muki = FALSE;
  }

  pdat->rot = (int)(atan2(pdat->vy, (double)vxc) / 3.14159265 * 180);

  if (ccounter > 200) {
    Bomb();
  }

  cell(CELL_MISSILE);
}

void CMissile::act_bomb() {
  pdat->magx = pdat->magy = 1.5f;
  pdat->vy = -2;

  if (ccounter < 1)
    cell(CELL_BOMB1);
  else if (ccounter < 2)
    cell(CELL_BOMB2);
  else if (ccounter < 8) {
    cell(CELL_BOMB3);
    pdat->y += pdat->vy;
  } else if (ccounter < 14) {
    cell(CELL_BOMB4);
    pdat->y += pdat->vy;
  } else if (ccounter < 20) {
    cell(CELL_BOMB5);
    pdat->y += pdat->vy;
  } else if (ccounter < 26) {
    cell(CELL_BOMB6);
    pdat->y += pdat->vy;
  } else
    Hide();
}

/*---------------------------------------------------------------
        ハンドグレネード
-----------------------------------------------------------------*/
CHandg::CHandg(CCharacterBase *parent, ATTACKINFO &atk) : CBulletBase(parent) {
  atkinfo = atk;
  base_z = ZZAHYO_BULLET3;

  m_part1 = aki3d.CreateParticleObject();
  m_part2 = aki3d.CreateParticleObject();

  m_part1->Create(HANDG_NUM1, "tex_bomb1.png");
  m_part2->Create(HANDG_NUM2, "tex_bomb2.png");
}

void CHandg::act_run() {
  pdat->y += pdat->vy;
  pdat->vy += 1.0f;

  movex(pdat->vx);

  if (pdat->y > 0) {
    pdat->y = 0;
    pdat->vy = pdat->vy * (-0.8);

    if (pdat->vx < 0)
      pdat->vx *= -1.0;
    if (rand() % 3 == 0)
      pdat->vx *= -1.0;
  }
  if (pdat->x < -640.0f) {
    pdat->vx *= -1.0f;
    pdat->x = -635.0f;
  }
  if (pdat->x > 640.0f) {
    pdat->vx *= -1.0f;
    pdat->x = 635.0f;
  }

  cell(CELL_HANDG);

  pdat->rot += 45;
  if (ccounter > 100)
    Bomb();
}

void CHandg::Bomb() {
  UINT i;

  for (i = 0; i < HANDG_NUM1; i++) {
    m_info1[i].Reset();
  }

  for (i = 0; i < HANDG_NUM2; i++) {
    m_info2[i].Reset();
  }

  CBulletBase::Bomb();
}

void CHandg::act_bomb() {
  if (ccounter == 0) {
    AddEffect(EFCTID_SINDO, 5, 5, 5);
  }
  if (ccounter > 60) {
    Hide();
    return;
  }
  float t = (float)ccounter / 60.0f;

  pdat->kougeki = (ccounter == 0) ? TRUE : FALSE;
  cell(CELL_HANDG_HANTEI);

  UINT i;
  V3d pos;
  V3d vel;
  float ar = 1.0f / 240.0f;

  for (i = 0; i < HANDG_NUM1; i++) {
    m_info1[i].Update();

    pos = m_info1[i].pos;
    pos.x *= ar;
    pos.y *= ar;
    pos.z *= ar;

    m_part1->SetPos(i, pos);
    m_part1->SetWidth(i, m_info1[i].width * ar);
    m_part1->SetAxis(i, Zero);
    m_part1->SetColor(i, m_info1[i].color);
  }
  for (i = 0; i < HANDG_NUM2; i++) {
    m_info2[i].Update();

    pos = m_info2[i].pos;
    pos.x *= ar;
    pos.y *= ar;
    pos.z *= ar;
    vel = m_info2[i].vel * 0.5f;
    vel.x *= ar;
    vel.y *= ar;
    vel.z *= ar;

    m_part2->SetPos(i, pos);
    m_part2->SetWidth(i, m_info2[i].width * ar * 2.0f);
    m_part2->SetAxis(i, vel);
    m_part2->SetColor(i, m_info2[i].color);
  }
}

DWORD CHandg::Draw() {
  switch (pdat->aid) {
  case CBB_STATE_RUN: {
    return 0;
  } break;
  case CBB_STATE_BOMB: {
    aki3d.SetBlend_Add();
    aki3d.EnableZ(TRUE, FALSE);
    V3d tra((float)pdat->x / 240.0f, (float)pdat->y / 240.0f, pdat->z);
    V3d sca((pdat->muki ? -1.0f : 1.0f), 1.0f, 1.0f);
    D3DXMATRIX l2w = aki3d.CreateMatrix(&sca, NULL, &tra);
    m_part1->Render(&l2w);
    m_part2->Render(&l2w);
    aki3d.EnableZ();
    aki3d.SetBlend_Normal();
  } break;
  }
  return TRUE;
}

void CHandg::HandgInfo1::Reset() {
  pos = Zero;
  vel.x = aki3d.RandomOne2() * 10.0f;
  vel.y = aki3d.RandomOne2() * 10.0f;
  vel.z = aki3d.RandomOne2() * 10.0f;
  width_base = aki3d.RandomOne() * 200.0f + 200.0f;
  time = 0;
  time_limit = aki3d.RandomOne() * 8.0f + 8.0f;
  color.red = 255;
  color.green = 150;
  color.blue = 100;
}

void CHandg::HandgInfo1::Update() {
  time += 1.0f;
  if (time > time_limit) {
    // Reset();
    color.alpha = 0;
    return;
  }
  float t = time / time_limit;

  vel.y -= 0.3f;
  pos += vel;

  width = width_base * (t * 0.5f + 0.5f);
  color.alpha = (BYTE)(255.0f * (1.0f - t));
}

void CHandg::HandgInfo2::Reset() {
  HandgInfo1::Reset();

  acc = Zero;
  pos.x = aki3d.RandomOne2() * 20.0f;
  pos.y = aki3d.RandomOne() * (-20.0f);
  pos.z = aki3d.RandomOne2() * 20.0f;

  time_limit = aki3d.RandomOne() * 20.0f + 10.0f;
  //	time = aki3d.RandomOne() * time_limit;

  width_base = 5.0f;
}

void CHandg::HandgInfo2::Update() {
  time += 1.0f;
  if (time > time_limit) {
    // Reset();
    color.alpha = 0;
    return;
  }
  float t = time / time_limit;

  acc.x += aki3d.RandomOne2() * 0.2f;
  acc.y += aki3d.RandomOne2() * 0.2f;
  acc.z += aki3d.RandomOne2() * 0.2f;

  vel += acc;
  pos += vel;
  if (pos.y > 0) {
    pos.y = 0;
    vel.y *= -1.0f;
  }

  width = width_base;
  color.alpha = (BYTE)(255.0f * (1.0f - t));
}

/*---------------------------------------------------------------
        クレイモア指向性対AA地雷
-----------------------------------------------------------------*/
CCraymore::CCraymore(CCharacterBase *parent, ATTACKINFO &atk) : CBulletBase(parent) {
  atkinfo = atk;
  base_z = ZZAHYO_BULLET3;

  m_part = aki3d.CreateParticleObject();
  m_part->Create(CRAYMORE_NUM, "tex_bomb2.png");

  atk_dummy = atk;
  atk_dummy.damage = 0;
}

void CCraymore::act_run() {
  pdat->y += pdat->vy;
  pdat->vy += 1.0f;

  movex(pdat->vx);

  if (pdat->y > 0) {
    pdat->y = 0;
    pdat->vy = pdat->vy * (-0.5);
    if (pdat->vy > -5) {
      // Bomb();
      ChangeAction(CRAYMORE_STATE_WAIT);
    }
  }
  if (pdat->x < -640.0f) {
    pdat->vx *= -1.0f;
    pdat->x = -635.0f;
  }
  if (pdat->x > 640.0f) {
    pdat->vx *= -1.0f;
    pdat->x = 635.0f;
  }

  cell(CELL_CRAYMORE);
}

void CCraymore::act_wait() {
  if (ccounter == 0) {
    pdat->atk = &atk_dummy;
    cell(CELL_CRAYMORE_WAIT);
  }

  if (ccounter > 15) {
    pdat->kougeki = TRUE;
  }

  if (ccounter > CRAYMORE_TIMEOUT)
    ChangeAction(CRAYMORE_STATE_ATTACK);
}

void CCraymore::act_atk() {
  UINT i;

  if (ccounter == 0) {
    AddEffect(EFCTID_MARK4, (int)(pdat->x + zurex(10)), -35, 0);
    pdat->atk = &atkinfo;
    pdat->z = ZZAHYO_BULLET2;

    for (i = 0; i < CRAYMORE_NUM; i++) {
      m_info[i].Reset();
    }
  }
  if (ccounter % 2)
    pdat->kougeki = TRUE;

  float falpha = 255.0f - (float)ccounter / 12.0f;
  ARGB color((BYTE)falpha, 255, 255, 150);
  for (i = 0; i < CRAYMORE_NUM; i++) {
    m_info[i].Update();

    m_part->SetPos(i, m_info[i].pos);
    m_part->SetAxis(i, m_info[i].vel);
    m_part->SetWidth(i, 10.0f / 240.0f);
    m_part->SetColor(i, color);
  }

  if (ccounter < 3)
    cell(CELL_CRAYMORED1);
  else if (ccounter < 6)
    cell(CELL_CRAYMORED2);
  else if (ccounter < 9)
    cell(CELL_CRAYMORED3);
  else if (ccounter < 12)
    cell(CELL_CRAYMORED4);
  else
    Bomb();
}

void CCraymore::act_bomb() {
  float falpha = 255.0f - 255.0f * (float)(ccounter - 30) / 30.0f;
  BYTE alpha = (BYTE)falpha;
  if (ccounter < 30)
    alpha = 255;
  pdat->color = 0x00FFFFFF | (alpha << 24);

  if (ccounter > 60)
    Hide();
}

DWORD CCraymore::Action() {
  switch (pdat->aid) {
  case CRAYMORE_STATE_WAIT:
    act_wait();
    break;
  case CRAYMORE_STATE_ATTACK:
    act_atk();
    break;
  default:
    return CBulletBase::Action();
  }
  return TRUE;
}

DWORD CCraymore::TouchC(ATTACKINFO *info, DWORD tc_eid) {
  if (info->damage == 0) {
    ChangeAction(CRAYMORE_STATE_ATTACK);
    return TOUCHC_CANCEL;
  }
  return CBulletBase::TouchC(info, tc_eid);
}

void CCraymore::ParticleInfo::Reset() {
  pos = Zero;
  pos.y = -20.0f / 240.0f;

  vel.x = (aki3d.RandomOne() + 1.0f) * 45.0f / 240.0f;
  vel.y = (aki3d.RandomOne() - 0.8f) * 30.0f / 240.0f;
  vel.z = 0.01f * aki3d.RandomOne2();
}

void CCraymore::ParticleInfo::Update() {
  vel *= 0.90f;
  pos += vel;
}

DWORD CCraymore::Draw() {
  if (pdat->aid == CRAYMORE_STATE_ATTACK) {
    aki3d.SetBlend_Add();
    aki3d.EnableZ(TRUE, FALSE);
    V3d tra((float)pdat->x / 240.0f, (float)pdat->y / 240.0f, pdat->z);
    V3d sca((pdat->muki ? -1.0f : 1.0f), 1.0f, 1.0f);
    D3DXMATRIX l2w = aki3d.CreateMatrix(&sca, NULL, &tra);
    m_part->Render(&l2w);
    aki3d.EnableZ();
    aki3d.SetBlend_Normal();
  }

  return CBulletBase::Draw();
}

DWORD CCraymore::Sousai(DWORD prm) {
  if (pdat->aid != CRAYMORE_STATE_ATTACK)
    ChangeAction(CRAYMORE_STATE_ATTACK);
  return TRUE;
}

/*---------------------------------------------------------------
        ショットガン
-----------------------------------------------------------------*/
CShotgun::CShotgun(CCharacterBase *parent, ATTACKINFO &atk) : CBulletBase(parent) {
  atkinfo = atk;
  base_z = ZZAHYO_BULLET3;

  m_part = aki3d.CreateParticleObject();
  m_part->Create(SHOTGUN_NUM, "tex_bomb2.png");
}

void CShotgun::act_run() {
  UINT i;
  if (ccounter == 0) {
    AddEffect(EFCTID_MARK2, (int)(pdat->x), (int)(pdat->y), 0);
    pdat->atk = &atkinfo;
    pdat->z = ZZAHYO_BULLET2;

    for (i = 0; i < SHOTGUN_NUM; i++) {
      m_info[i].Reset();
    }
  }
  if (ccounter % 2)
    pdat->kougeki = TRUE;
  cell(CELL_SHOTGUN_HANTEI);

  float falpha = 255.0f - (float)ccounter / 6.0f;
  ARGB color((BYTE)falpha, 255, 255, 150);
  for (i = 0; i < SHOTGUN_NUM; i++) {
    m_info[i].Update();

    m_part->SetPos(i, m_info[i].pos);
    m_part->SetAxis(i, m_info[i].vel);
    m_part->SetWidth(i, 20.0f / 240.0f);
    m_part->SetColor(i, color);
  }

  if (ccounter >= 6)
    Hide();
}

void CShotgun::act_bomb() {}

DWORD CShotgun::Draw() {
  if (pdat->aid != CBB_STATE_IDLE) {
    aki3d.SetBlend_Add();
    aki3d.EnableZ(TRUE, FALSE);
    V3d tra((float)pdat->x / 240.0f, (float)pdat->y / 240.0f, pdat->z);
    V3d sca((pdat->muki ? -1.0f : 1.0f), 1.0f, 1.0f);
    D3DXMATRIX l2w = aki3d.CreateMatrix(&sca, NULL, &tra);
    m_part->Render(&l2w);
    aki3d.EnableZ();
    aki3d.SetBlend_Normal();
  }

  return TRUE;
}

void CShotgun::ParticleInfo::Reset() {
  pos = Zero;

  vel.x = (aki3d.RandomOne() + 1.0f) * 20.0f / 240.0f;
  vel.y = (aki3d.RandomOne2()) * 16.0f / 240.0f;
  vel.z = 0.02f * aki3d.RandomOne2();
}

void CShotgun::ParticleInfo::Update() {
  vel *= 0.90f;
  pos += vel;
}

/*---------------------------------------------------------------
        ライフル
-----------------------------------------------------------------*/
CRifle::CRifle(CCharacterBase *parent, ATTACKINFO &atk) : CBulletBase(parent) {
  atkinfo = atk;
  base_z = ZZAHYO_BULLET3;

  AddProperty(GOBJFLG_NEEDTARGET);
}

void CRifle::act_run() {
  cell(CELL_RIFLE);

  if (ccounter == 0) {
    pdat->vx = 0;
    pdat->vy = 0;
  }

  if (key_idx >= 0) {
    DWORD key = CGoluahObject::GetKey(key_idx, 0);
    if (key & KEYSTA_UP)
      pdat->vy -= 0.325f;
    else if (key & KEYSTA_DOWN)
      pdat->vy += 0.325f;
    if (key & KEYSTA_ALEFT)
      pdat->vx -= 0.325f;
    else if (key & KEYSTA_ARIGHT)
      pdat->vx += 0.325f;
  } else {
    GOBJECT *obj = GetInfo(pdat->eid);
    if (pdat->eid == 0)
      obj = NULL;
    if (obj) {
      if (obj->y - 50 - pdat->y < -75)
        pdat->vy -= 0.15f;
      else if (obj->y - 50 - pdat->y > 75)
        pdat->vy += 0.15f;
      if (obj->x - pdat->x < -75)
        pdat->vx -= 0.15f;
      else if (obj->x - pdat->x > 75)
        pdat->vx += 0.15f;
    }
  }

  if (parent_obj->muki) {
    if (pdat->x > parent_obj->x - 160) {
      pdat->x = parent_obj->x - 160;
      pdat->vx = 0;
    } else if (pdat->x < parent_obj->x - 700) {
      pdat->x = parent_obj->x - 700;
      pdat->vx = 0;
    }
  } else {
    if (pdat->x < parent_obj->x + 160) {
      pdat->x = parent_obj->x + 160;
      pdat->vx = 0;
    } else if (pdat->x > parent_obj->x + 700) {
      pdat->x = parent_obj->x + 700;
      pdat->vx = 0;
    }
  }
  if (pdat->y > 0) {
    pdat->y = 0;
    pdat->vy = 0;
  }
  if (pdat->y < -480) {
    pdat->y = -480;
    pdat->vy = 0;
  }

  if (ccounter < 10) {
    ARGB col((BYTE)(25 * ccounter), 255, 255, 255);
    pdat->color = col.dwcol;
  }

  pdat->x += pdat->vx;
  pdat->y += pdat->vy;
  cell(CELL_RIFLE);

  if (ccounter == RIFLE_TIMEOUT) {
    pdat->kougeki = TRUE;
  }
  if (ccounter > RIFLE_TIMEOUT) {
    Bomb();
  }

  cell(CELL_RIFLE);
}

void CRifle::act_bomb() {
  cell(CELL_RIFLE);

  float t = (float)ccounter / 15.0f;
  DWORD alpha = (DWORD)(255.0f * (1.0f - t));
  pdat->color = 0x00FFFFFF | (alpha << 24);
  pdat->rot = ccounter * 6;
  float mag = 1.0f + 0.5f * t;
  pdat->magx = pdat->magy = mag;
  if (ccounter > 15)
    Hide();
}

void CRifle::ForceStop() {
  if (pdat->aid == CBB_STATE_RUN) {
    Bomb();
  }
}

/*---------------------------------------------------------------
        火炎放射器
-----------------------------------------------------------------*/

CFireThrower::CFireThrower(CCharacterBase *parent, ATTACKINFO &atk) : CBulletBase(parent) {
  atkinfo = atk;
  base_z = ZZAHYO_BULLET3;

  part = aki3d.CreateParticleObject();
  part->Create(NUM_FIRETHROWER, "tex_fire.png");
}

DWORD CFireThrower::TouchC(ATTACKINFO *info, DWORD tc_eid) {
  BOOL find = FALSE;
  for (UINT i = 0; i < list.size(); i++) {
    if (tc_eid == list[i]) {
      find = TRUE;
      break;
    }
  }

  if (!find) {
    info->hit |= HITINFO_EFCTBURN;
    list.push_back(tc_eid);
  } else
    info->hit &= ~HITINFO_EFCTBURN;

  return TOUCHC_OK;
}

void CFireThrower::ForceStop() {
  if (pdat->aid == CBB_STATE_RUN) {
    Bomb();
  }
}

void CFireThrower::act_run() {
  UINT i;
  if (ccounter == 0) {
    for (i = 0; i < NUM_FIRETHROWER; i++) {
      list.clear();
      info[i].Init();
    }
    posdelta = 0;
    pos_prv = parent_obj->x;
  }

  /*	const UINT rnum=3;
          //if( (ccounter*rnum)+rnum-1 < NUM_FIRETHROWER)
          {
                  for(i=0;i<rnum;i++)
                  {
                          if( ((ccounter*rnum)%NUM_FIRETHROWER)+i < NUM_FIRETHROWER )
                          {
                                  info[((ccounter*rnum)%NUM_FIRETHROWER)+i].Reset();
                          }
                  }
          }*/

  posdelta += parent_obj->x - pos_prv;
  pos_prv = parent_obj->x;

  for (i = 0; i < NUM_FIRETHROWER; i++) {
    info[i].Update(TRUE, posdelta);
    part->SetPos(i, info[i].pos);
    part->SetAxis(i, info[i].vel);
    part->SetWidth(i, info[i].width);
    part->SetColor(i, info[i].col);
  }

  if (ccounter % 3 == 0)
    pdat->kougeki = TRUE;

  if (ccounter > DUR_FIRETHROW) {
    Bomb();
  }

  cell(CELL_FIRETHROWER_HANTEI);
  if (ccounter < 10)
    pdat->magx = 0.1f * ccounter;
  else
    pdat->magx = 1;
}

void CFireThrower::act_bomb() {
  UINT i;
  for (i = 0; i < NUM_FIRETHROWER; i++) {
    info[i].Update(FALSE, posdelta);
    part->SetPos(i, info[i].pos);
    part->SetAxis(i, info[i].vel);
    part->SetWidth(i, info[i].width);
    part->SetColor(i, info[i].col);
  }

  if (ccounter > 30) {
    Hide();
  }
}

void CFireThrower::ParticleInfo::Init() {
  created = 0;
  t = aki3d.RandomOne();

  col.red = 255;
  col.green = 100;
  col.blue = 50;
}

void CFireThrower::ParticleInfo::Reset(double posdelta) {
  pos = Zero;

  vel.x = 20.0f + 20.0f * aki3d.RandomOne(); //+ (float)posdelta;//なんかヘン
  vel.y = 3.0f * aki3d.RandomOne();
  vel.z = 3.0f * aki3d.RandomOne();

  vel /= 240.0f;

  t = 0;
  created = TRUE;
}

void CFireThrower::ParticleInfo::Update(BOOL recreate, double posdelta) {

  t += 3.0f / ((float)NUM_FIRETHROWER);
  if (t > 1.0f) {
    if (recreate)
      Reset(posdelta);
    else
      created = FALSE;
  }

  const float width_base = 150.0f / 240.0f;

  vel *= 0.95f;
  vel.y -= 0.5f / 240.0f;
  pos += vel;

  width = width_base * ((t < 0.25f) ? t * 4.0f : 1.0f);
  col.alpha = (BYTE)(255.0f * (1.0f - t));
  if (!created)
    col.alpha = 0;
}

DWORD CFireThrower::Draw() {
  if (pdat->aid != CBB_STATE_IDLE) {
    aki3d.SetBlend_Add();
    aki3d.EnableZ(TRUE, FALSE);
    V3d tra((float)pdat->x / 240.0f, (float)pdat->y / 240.0f, pdat->z);
    V3d sca((pdat->muki ? -1.0f : 1.0f), 1.0f, 1.0f);
    D3DXMATRIX l2w = aki3d.CreateMatrix(&sca, NULL, &tra);
    part->Render(&l2w);
    aki3d.EnableZ();
    aki3d.SetBlend_Normal();
    //	return 0;
  }

  return TRUE;
}

/*---------------------------------------------------------------
        ロケット?
-----------------------------------------------------------------*/
CRocket::CRocket(CCharacterBase *parent, ATTACKINFO &atk) : CBulletBase(parent) {
  atkinfo = atk;
  base_z = ZZAHYO_BULLET3;

  part_bomb = aki3d.CreateParticleObject();
  part_bomb->Create(ROCKET_BOMB_NUM, "tex_bomb1.png");
}

void CRocket::act_run() {
  RemoveProperty(GOBJFLG_DONOTSTOP);

  if (ccounter == 0)
    pdat->vx = 0;

  cell(CELL_ROCKET);
  pdat->kougeki = TRUE;

  pdat->vx++;
  movex(pdat->vx);

  if (ccounter > 200)
    Bomb();
}

void CRocket::act_bomb() {
  UINT i;

  if (ccounter == 0) {
    for (i = 0; i < ROCKET_BOMB_NUM; i++) {
      m_info[i].Reset();
    }
  }

  V3d pos;
  float ar = 1.0f / 240.0f;

  for (i = 0; i < ROCKET_BOMB_NUM; i++) {
    m_info[i].Update();

    pos = m_info[i].pos;
    pos.x *= ar;
    pos.y *= ar;
    pos.z *= ar;

    part_bomb->SetPos(i, pos);
    part_bomb->SetWidth(i, m_info[i].width * ar);
    part_bomb->SetAxis(i, Zero);
    part_bomb->SetColor(i, m_info[i].color);
  }

  if (pdat->counter > 30)
    Hide();
}

void CRocket::PBombInfo::Reset() {
  pos = Zero;
  vel.x = aki3d.RandomOne2() * 8.0f;
  vel.y = aki3d.RandomOne2() * 8.0f;
  vel.z = aki3d.RandomOne2() * 8.0f;
  width_base = aki3d.RandomOne() * 100.0f + 300.0f;
  time = 0;
  time_limit = aki3d.RandomOne() * 8.0f + 22.0f;
  color.red = 255;
  color.green = 100;
  color.blue = 70;
}

void CRocket::PBombInfo::Update() {
  time += 1.0f;
  if (time > time_limit) {
    // Reset();
    color.alpha = 0;
    return;
  }
  float t = time / time_limit;

  vel.y -= 0.3f;
  pos += vel;

  width = width_base * (t * 0.5f + 0.5f);
  color.alpha = (BYTE)(255.0f * (1.0f - t));
}

DWORD CRocket::Draw() {
  if (pdat->aid == CBB_STATE_BOMB) {
    aki3d.SetBlend_Add();
    aki3d.EnableZ(TRUE, FALSE);
    V3d tra((float)pdat->x / 240.0f, (float)pdat->y / 240.0f, pdat->z);
    V3d sca((pdat->muki ? -1.0f : 1.0f), 1.0f, 1.0f);
    D3DXMATRIX l2w = aki3d.CreateMatrix(&sca, NULL, &tra);
    part_bomb->Render(&l2w);
    aki3d.EnableZ();
    aki3d.SetBlend_Normal();
  } else if (pdat->aid == CBB_STATE_RUN) {
    return 0;
  }

  return TRUE;
}

/*---------------------------------------------------------------
        アギ
-----------------------------------------------------------------*/
CAgi::CAgi(CCharacterBase *parent, ATTACKINFO &atk) : CBulletBase(parent) {
  atkinfo = atk;

  part = aki3d.CreateParticleObject();
  part->Create(AGI_NUM, "tex_fire.png");
}

void CAgi::act_run() {
  UINT i;

  if (ccounter == 0) {
    for (i = 0; i < AGI_NUM; i++) {
      info[i].Reset(pdat);
    }
  }

  for (i = 0; i < AGI_NUM; i++) {
    info[i].Update(pdat, TRUE);

    part->SetPos(i, info[i].pos);
    part->SetAxis(i, info[i].vel * 0.25f);
    part->SetWidth(i, info[i].width);
    part->SetColor(i, info[i].color);
  }

  cell(CELL_AGI_HANTEI);
  pdat->kougeki = TRUE;

  pdat->x += pdat->vx;
  pdat->y += pdat->vy;

  if (pdat->y > 0 || ccounter > 200) {
    Bomb();
  }
}

void CAgi::act_bomb() {
  UINT i;
  for (i = 0; i < AGI_NUM; i++) {
    info[i].Update(pdat, FALSE);

    part->SetPos(i, info[i].pos);
    part->SetAxis(i, info[i].vel * 0.25f);
    part->SetWidth(i, info[i].width);
    part->SetColor(i, info[i].color);
  }

  if (ccounter > 30)
    Hide();
}

DWORD CAgi::Draw() {
  if (pdat->aid != CBB_STATE_IDLE) {
    aki3d.SetBlend_Add();
    aki3d.EnableZ(TRUE, FALSE);
    part->Render();
    aki3d.EnableZ();
    aki3d.SetBlend_Normal();
  }
  return TRUE;
}

void CAgi::ParticleInfo::Reset(GOBJECT *parent) {
  pos.x = (float)parent->x;
  pos.y = (float)parent->y;
  pos.z = parent->z;

  vel.x = 2.0f * aki3d.RandomOne2();
  vel.y = 2.0f * aki3d.RandomOne2();
  vel.z = 2.0f * aki3d.RandomOne2();

  pos /= 240.0f;
  vel /= 240.0f;

  time = 0;
  time_limit = 5.0f + 5.0f * aki3d.RandomOne2();

  color.red = 255;
  color.green = 100;
  color.blue = 50;
}

void CAgi::ParticleInfo::Update(GOBJECT *parent, BOOL recreate) {
  time += 1.0f;
  if (time > time_limit) {
    if (recreate)
      Reset(parent);
    else {
      color.alpha = 0;
      return;
    }
  }

  vel.y -= 0.25f / 240.0f;
  pos += vel * (recreate ? 1.0f : 5.0f);

  float t = time / time_limit;
  const float width_base = 200.0f / 240.0f;

  color.alpha = (BYTE)(255.0f * (1.0f - t));
  width = width_base * (0.5f + 0.5f * t);
}

/*---------------------------------------------------------------
        ランタン
-----------------------------------------------------------------*/
CLantern::CLantern(CCharacterBase *parent, ATTACKINFO &atk) : CBulletBase(parent) {
  base_z = ZZAHYO_BULLET3;
  AddProperty(GOBJFLG_NEEDTARGET);
  RemoveProperty(GOBJFLG_ATTACK);

  for (UINT i = 0; i < LANTERN_NUM_AGI; i++) {
    m_agi[i] = new CAgi(parent, atk);
  }
}

void CLantern::act_run() {
  if (ccounter == 0) {
    aim_x = (float)parent_obj->x + zurex(200);

    pdat->x = pdat->muki ? 700 : -700;
    pdat->y = -300;
    pdat->vx = 5;
  }

  pdat->vy = 5.0f * sinf(((float)ccounter / 10.0f));
  pdat->y += pdat->vy;

  movex(pdat->vx);

  if ((aim_x - pdat->x) * (aim_x - pdat->x) < 50 * 50) {
    ChangeAction(ACT_LANTERN_AGI);
  }
  cell(CELL_LANTERN1);
}

void CLantern::act_agi() {
  cell(CELL_LANTERN2);

  if (ccounter == 0) {
    pdat->vx = 0;
    pdat->vy = 0;
  }

  UINT i;

  if (ccounter == 3) {
    BOOL lunched = FALSE;
    V2d vel;
    for (i = 0; i < LANTERN_NUM_AGI; i++) {
      if (m_agi[i]->isReady()) {
        GOBJECT *ene = GetInfo(pdat->eid);
        if (pdat->eid == 0)
          ene = FALSE;
        if (ene) {
          vel.x = (float)(ene->x - pdat->x);
          vel.y = (float)(ene->y - 50 - pdat->y);
        } else {
          vel.x = 1.0f;
          vel.y = 1.0f;
        }
        vel.Normalize();
        vel *= 15.0f;
        m_agi[i]->Go(pdat->muki, pdat->x, pdat->y, vel.x, vel.y);
        lunched = TRUE;
        break;
      }
    }

    if (lunched) {
      pdat->vy = -vel.y;
      pdat->vx = -vel.x;
    }
  }

  pdat->vx *= 0.85f;
  pdat->vy *= 0.85f;

  pdat->y += pdat->vy;
  pdat->x += pdat->vx;

  if (ccounter > 60)
    ChangeAction(ACT_LANTERN_DISAPPEAR);
}

void CLantern::act_bomb() {}

void CLantern::act_appear() {
  cell(CELL_LANTERN1);

  if (ccounter == 0) {
    pdat->muki = parent_obj->muki;
    pdat->x = parent_obj->x + zurex(150);
    pdat->y = -300.0f;
  }

  pdat->y++;

  if (ccounter >= 20) {
    ChangeAction(ACT_LANTERN_MAHA_AGI);
    ;
  }

  BYTE a = (BYTE)(255.0f * ((float)ccounter) / 20.0f);
  ARGB color(a, a, a, a);
  pdat->color = color.dwcol;
}

void CLantern::act_maha_agi() {
  if (ccounter < 20) {
    act_agi();
  } else if (ccounter < 40) {
    ccounter -= 20;
    act_agi();
    ccounter += 20;
  } else if (ccounter < 60) {
    ccounter -= 40;
    act_agi();
    ccounter += 40;
  } else if (ccounter < 80) {
    ccounter -= 60;
    act_agi();
    ccounter += 60;
  } else
    ChangeAction(ACT_LANTERN_DISAPPEAR);
}

void CLantern::act_disappear() {
  cell(CELL_LANTERN1);

  pdat->y--;

  if (ccounter >= 20) {
    Hide();
    return;
  }

  BYTE a = (BYTE)(255.0f * (1.0f - ((float)ccounter) / 20.0f));
  ARGB color(a, a, a, a);
  pdat->color = color.dwcol;
}

DWORD CLantern::Action() {
  switch (pdat->aid) {
  case ACT_LANTERN_AGI:
    act_agi();
    break;
  case ACT_LANTERN_MAHA_AGI:
    act_maha_agi();
    break;
  case ACT_LANTERN_APPEAR:
    act_appear();
    break;
  case ACT_LANTERN_DISAPPEAR:
    act_disappear();
    break;
  default:
    return CBulletBase::Action();
  }
  return TRUE;
}

/*---------------------------------------------------------------
        Come On!
-----------------------------------------------------------------*/

CComeon::CComeon(CCharacterBase *parent)
    : CBulletBase(parent) // CEffectBase(parent)
{
  base_z = ZZAHYO_BULLET3;
  RemoveProperty(GOBJFLG_ATTACK);
}

void CComeon::act_run() {
  cell(pdat->muki ? CELL_COMEON_R : CELL_COMEON_L);
  pdat->revx = pdat->muki;

  movex(2);
  pdat->y -= 2;

  if (ccounter >= 20)
    Hide();

  ARGB col(0, 255, 255, 255);
  col.alpha = (BYTE)(255.0f * (1.0f - ((float)ccounter) / 20.0f));
  pdat->color = col.dwcol;
}

void CComeon::act_bomb() {}

/*---------------------------------------------------------------
        HTTPレーザー
-----------------------------------------------------------------*/
CHttpLaser::CHttpLaser(CCharacterBase *parent, ATTACKINFO &atk) : CBulletBase(parent) {
  atkinfo = atk;
  base_z = ZZAHYO_BULLET3;
}

void CHttpLaser::act_run() {
  if (ccounter == 0) {
    list.clear();
  }
  if (ccounter > 18) {
    Hide();
    return;
  }
  pdat->kougeki = TRUE;
  cell(CELL_WWW1 + ccounter);
}

void CHttpLaser::act_bomb() {}

DWORD CHttpLaser::TouchC(ATTACKINFO *info, DWORD tc_eid) {
  BOOL find = FALSE;
  for (UINT i = 0; i < list.size(); i++) {
    if (tc_eid == list[i]) {
      return TOUCHC_CANCEL;
    }
  }

  list.push_back(tc_eid);
  return TOUCHC_OK;
}

/*---------------------------------------------------------------
        もやもや
-----------------------------------------------------------------*/

#define MOYA_CH_VAL (0.90f)

CMoyamoya::CMoyamoya(CCharacterBase *parent) : CBulletBase(parent) {
  RemoveProperty(GOBJFLG_ATTACK);
  base_z = ZZAHYO_EFFECT1;

  glow_spd = (1.0f / 45.0f);

  this->m_2ndFlag = FALSE;
}

BOOL CMoyamoya::Go(BOOL muki, double x, double y, double vx, double vy) {
  pdat->muki = muki;
  pdat->x = x;
  pdat->y = y;

  glow = 0;

  ChangeAction(CBB_STATE_RUN);
  next_type = GetNextType();

  return TRUE;
}

void CMoyamoya::act_run() {
  glow += glow_spd;
  if (glow > 1.0f) {
    crnt_type = next_type;
    next_type = GetNextType();
    glow = 0.0f;
    if (crnt_type == GetFirstType()) {
      m_2ndFlag = TRUE;
    }
  }

  alpha_base = 255.0f;

  if (parent_obj->aid & ACTID_MOYAMOYA) {
    pdat->x = parent_class->GetGObject()->x + zurex(75);
    pdat->y = parent_class->GetGObject()->y - 140;
  }
}

void CMoyamoya::act_bomb() {
  alpha_base = (float)(ccounter) / 30.0f;
  alpha_base = 255.0f * (1.0f - alpha_base);
  if (alpha_base > 1.0f) {
    Hide();
    if (m_force_flag) {
      crnt_type = GetFirstType();
      next_type = GetNextType();
    }
  }
}

void CMoyamoya::SetNextType(MoyaType next) {
  if (glow > MOYA_CH_VAL)
    return;
  next_type = next;
}

DWORD CMoyamoya::Draw() {
  if (pdat->aid != CBB_STATE_IDLE) {
    DWORD cellno = CELL_FUKI_BASE;
    DWORD alpha = (DWORD)alpha_base;

    SetDWAlpha(pdat->color, alpha);
    pdat->cnow = cellno;
    CellDraw(pdat);

    cellno = Type2Cell(crnt_type);
    if (cellno != 0) {
      alpha = (glow < MOYA_CH_VAL)
                  ? (DWORD)alpha_base
                  : (DWORD)(alpha_base * (1.0f - (glow - MOYA_CH_VAL) / (1.0f - MOYA_CH_VAL)));
      SetDWAlpha(pdat->color, alpha);
      pdat->cnow = cellno;
      CellDraw(pdat);
    }

    if (glow > MOYA_CH_VAL) {
      cellno = Type2Cell(next_type);
      if (cellno != 0) {
        alpha = (DWORD)(alpha_base * (glow - MOYA_CH_VAL) / (1.0f - MOYA_CH_VAL));
        SetDWAlpha(pdat->color, alpha);
        pdat->cnow = cellno;
        CellDraw(pdat);
      }
    }
  }

  return TRUE;
}

void CMoyamoya::ForceEnd() {
  if (pdat->aid == CBB_STATE_RUN) {
    m_2ndFlag = FALSE;
    m_force_flag = TRUE;
    Bomb();
  }
}

void CMoyamoya::ForceStop() {
  if (pdat->aid == CBB_STATE_RUN) {
    m_force_flag = FALSE;
    Bomb();
  }
}

DWORD CMoyamoya::Type2Cell(MoyaType type) {
  switch (type) {
  case Moya_Ocya:
    return CELL_FUKI_OCYA;
  case Moya_Jien:
    return CELL_FUKI_JIEN;
  case Moya_Aroe:
    return CELL_FUKI_AROE;
  case Moya_Moukoneeyo:
    return CELL_FUKI_MOUKONEEYO;
  case Moya_MissileH:
    return CELL_FUKI_MISSILE_H;
  case Moya_MissileSP:
    return CELL_FUKI_MISSILE_SP;
  case Moya_Handg:
    return CELL_FUKI_HANDG;
  case Moya_Craymore:
    return CELL_FUKI_CRAYMORE;
  case Moya_MissileV:
    return CELL_FUKI_MISSILE_V;
  case Moya_FireThrow:
    return CELL_FUKI_FIRETHROW;
  case Moya_Shot:
    return CELL_FUKI_SHOT;
  case Moya_Rifle:
    return CELL_FUKI_RIFLE;
  case Moya_Rocket:
    return CELL_FUKI_ROCKET;
  case Moya_Lantern:
    return CELL_FUKI_LANTERN;
  case Moya_Hellfire:
    return CELL_FUKI_HELLFIRE;
  case Moya_HttpLaser:
    return CELL_FUKI_LASER;
  case Moya_Eraser99:
    return CELL_FUKI_ERASER;
  default:
    return 0;
  }
}

// Aルート状態遷移
MoyaType CMoyamoyaA::GetNextType() {
  switch (crnt_type) {
  case Moya_Ocya:
    return Moya_Jien;
  case Moya_Jien:
    return Moya_Aroe;
  case Moya_Aroe:
    return Moya_Moukoneeyo;
  case Moya_Moukoneeyo:
    return Moya_MissileH;
  case Moya_MissileH: {
    if (!m_2ndFlag) {
      return GetFirstType();
    } else {
      return Moya_MissileSP;
    }
  } break;
  case Moya_MissileSP:
    return GetFirstType();
  }

  return Moya_Invalid;
}

// Bルート状態遷移
MoyaType CMoyamoyaB::GetNextType() {
  switch (crnt_type) {
  case Moya_Handg:
    return Moya_Craymore;
  case Moya_Craymore:
    return Moya_Lantern;
  case Moya_Lantern:
    return Moya_FireThrow;
  case Moya_FireThrow: {
    if (!m_2ndFlag) {
      return GetFirstType();
    } else {
      return Moya_Hellfire;
    }
  } break;
  case Moya_Hellfire:
    return GetFirstType();
  }
  return Moya_Invalid;
}

// Cルート状態遷移
MoyaType CMoyamoyaC::GetNextType() {
  switch (crnt_type) {
  case Moya_Shot:
    return Moya_Rifle;
  case Moya_Rifle:
    return Moya_Rocket;
  case Moya_Rocket:
    return Moya_MissileV;
  case Moya_MissileV:
    return Moya_HttpLaser;
  case Moya_HttpLaser: {
    if (!m_2ndFlag) {
      return GetFirstType();
    } else {
      return Moya_Eraser99;
    }
  } break;
  case Moya_Eraser99:
    return GetFirstType();
  }
  return Moya_Invalid;
}

/*---------------------------------------------------------------
        イレイサー"99"
-----------------------------------------------------------------*/

CEraser99::CEraser99(CCharacterBase *parent, ATTACKINFO &atk) : CBulletBase(parent) {
  atkinfo = atk;

  m_part = aki3d.CreateParticleObject();
  m_part->Create(ERASER99_NUM_CONPART, "tex_conpart.png");

  m_part_ray = aki3d.CreateParticleObject();
  m_part_ray->Create(ERASER99_NUM_RAY, "tex_tubu.png");
}

void CEraser99::act_run() {
  UINT i;

  if (ccounter == 0) {
    for (i = 0; i < ERASER99_NUM_CONPART; i++) {
      m_info[i].Reset();
    }
  }

  float t = (float)ccounter / 200.0f;

  ARGB col;
  col.blue = 255;
  col.red = 120;
  col.green = 40;
  col.alpha = (BYTE)(255.0f * (t < 0.25f ? t * 4.0f : 1.0f));

  float wscale = 1.0f;
  wscale += t * t * 6.0f;

  for (i = 0; i < ERASER99_NUM_CONPART; i++) {
    m_info[i].Update();

    float width;
    if (i < ERASER99_NUM_CONPART / 2)
      width = 10.0f / 240.0f;
    else if (i < ERASER99_NUM_CONPART / 4 * 3)
      width = 20.0f / 240.0f;
    else
      width = 30.0f / 240.0f;

    m_part->SetPos(i, m_info[i].pos);
    m_part->SetAxis(i, m_info[i].vel * 0.5f);
    m_part->SetWidth(i, width * wscale);
    m_part->SetColor(i, col);
  }

  if (ccounter > 200)
    Bomb();
}

void CEraser99::act_bomb() {
  UINT i;

  if (ccounter == 0) {
    AddEffect(EFCTID_SINDO, 10, 50, 0);

    for (i = 0; i < ERASER99_NUM_CONPART; i++) {
      m_info[i].Reset2();
    }

    for (i = 0; i < ERASER99_NUM_RAY; i++) {
      m_info_ray[i].Reset();
    }

    obj_rkt->Bomb();
    obj_rkt->SetPos(pdat->x, pdat->y);
  }

  float t = (float)ccounter / 80.0f;

  ARGB col;
  col.blue = 255;
  col.red = 120;
  col.green = 40;
  col.alpha = (BYTE)(250 * (1.0f - t));

  for (i = 0; i < ROCKET_BOMB_NUM; i++) {
    obj_rkt->m_info[i].color.red = col.red;
    obj_rkt->m_info[i].color.green = col.green;
    obj_rkt->m_info[i].color.blue = col.blue;
  }

  for (i = 0; i < ERASER99_NUM_CONPART; i++) {
    m_info[i].Update2();

    m_part->SetPos(i, m_info[i].pos);
    m_part->SetAxis(i, m_info[i].vel * 2.5f);
    m_part->SetWidth(i, 5.0f / 240.0f);
    m_part->SetColor(i, col);
  }

  for (i = 0; i < ERASER99_NUM_RAY; i++) {
    m_info_ray[i].Update(1.0f - t);

    m_part_ray->SetPos(i, m_info_ray[i].pos + m_info_ray[i].ax);
    m_part_ray->SetAxis(i, m_info_ray[i].ax * 0.5f);
    m_part_ray->SetWidth(i, m_info_ray[i].width);
    m_part_ray->SetColor(i, m_info_ray[i].col);
  }

  cell(CELL_ERASER_HANTEI);
  pdat->kougeki = (ccounter < 40) ? TRUE : FALSE;
  if (ccounter > 80)
    Hide();
}

void CEraser99::ParticleInfo::Reset() {
  pos.x = aki3d.RandomOne2() * 400.0f;
  pos.y = aki3d.RandomOne2() * 400.0f;
  pos.z = aki3d.RandomOne2() * 200.0f;

  pos /= 240.0f;

  vel = Zero;
}

void CEraser99::ParticleInfo::Update() {
  V3d acc;
  D3DXVec3Cross(&acc, &pos, &Xaxis);
  acc *= 0.3f / 240.0f / D3DXVec3Length(&acc);
  acc -= pos * 1.0f / 240.0f / D3DXVec3Length(&pos);

  vel += acc;
  vel *= 0.8f;
  pos += vel;

  if (D3DXVec3Length(&pos) < 20.0f / 240.0f) {
    vel *= 2.0f;
  }
}

void CEraser99::ParticleInfo::Reset2() {
  vel.x = (aki3d.RandomOne2() - 0.5f) * 8.0f;
  vel.y = (aki3d.RandomOne2() - 0.5f) * 8.0f;
  vel.z = aki3d.RandomOne2() * 8.0f;

  vel /= 240.0f;

  pos = Zero;
}

void CEraser99::ParticleInfo::Update2() { pos += vel; }

DWORD CEraser99::Draw() {
  if (pdat->aid != CBB_STATE_IDLE) {
    aki3d.SetBlend_Add();
    aki3d.EnableZ(TRUE, FALSE);
    V3d tra((float)pdat->x / 240.0f, (float)pdat->y / 240.0f, pdat->z);
    V3d sca((pdat->muki ? -1.0f : 1.0f), 1.0f, 1.0f);
    D3DXMATRIX l2w = aki3d.CreateMatrix(&sca, NULL, &tra);
    m_part->Render(&l2w);
    if (pdat->aid == CBB_STATE_BOMB) {
      m_part_ray->Render(&l2w);
    }
    aki3d.EnableZ();
    aki3d.SetBlend_Normal();
  }

  return TRUE;
}

void CEraser99::RayPartInfo::Reset() {
  ax_base.x = (aki3d.RandomOne()) * 640.0f;
  ax_base.y = (aki3d.RandomOne2()) * 250.0f;
  ax_base.z = aki3d.RandomOne2() * 50.0f;

  ax_base /= 240.0f;

  pos = Zero;
  pos.x = -70.0f * (aki3d.RandomOne() - 0.25f);
  pos /= 240.0f;

  width_base = (250.0f + aki3d.RandomOne() * 150.0f) / 240.0f;

  time = 0;
  time_end = 5.0f + 10.0f * aki3d.RandomOne();

  col.blue = 255;
  col.red = 120;
  col.green = 40;
}

void CEraser99::RayPartInfo::Update(float pt) {
  time += 1.0f;
  if (time > time_end)
    Reset();
  float t = time / time_end;

  ax = ax_base * (0.5f + t) * pt;
  width = width_base * (0.55f + t) * pt;

  pos += ax * 0.05f;

  col.alpha = (BYTE)(255.0f * (1.0f - t) * pt);
}

/*---------------------------------------------------------------
        ダッコ・血
-----------------------------------------------------------------*/

CBlood::CBlood(CCharacterBase *parent) : CBulletBase(parent) {
  AddProperty(GOBJFLG_DONOTSTOP);
  RemoveProperty(GOBJFLG_ATTACK);
}

void CBlood::act_run() {
  if (ccounter < 5)
    cell(CELL_BLOOD1);
  else if (ccounter < 10)
    cell(CELL_BLOOD2);
  else if (ccounter < 15)
    cell(CELL_BLOOD3);
  else if (ccounter < 20)
    cell(CELL_BLOOD4);
  else if (ccounter < 25)
    cell(CELL_BLOOD5);
  else if (ccounter < 30)
    cell(0);
  else
    Hide();
}

void CBlood::act_bomb() { Hide(); }

DWORD CBlood::DrawBack() {
  if (pdat->aid == CBB_STATE_RUN) {
    LPDIRECT3DDEVICE8 d3ddev = GetD3DDevice();
    if (d3ddev) {
      MYVERTEX3D vb[4];

      vb[0].color = vb[1].color = vb[2].color = vb[3].color = 0xFFFF0000;

      vb[0].x = (float)(-4.5f);
      vb[1].x = (float)(-4.5f);
      vb[2].x = (float)(4.5f);
      vb[3].x = (float)(4.5f);

      vb[0].y = (float)(-4.5f);
      vb[1].y = (float)(4.5f);
      vb[2].y = (float)(-4.5f);
      vb[3].y = (float)(4.5f);

      vb[0].z = vb[1].z = vb[2].z = vb[3].z = -1.0f;

      D3DXMATRIX mat;
      D3DXMatrixIdentity(&mat);
      d3ddev->SetTransform(D3DTS_WORLD, &mat);

      d3ddev->SetTexture(0, NULL);

      aki3d.EnableZ(FALSE, TRUE);
      d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vb, sizeof(MYVERTEX3D));
      aki3d.EnableZ();
    }
  }
  return 0;
}
