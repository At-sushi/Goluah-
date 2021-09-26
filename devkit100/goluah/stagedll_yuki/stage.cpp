

#include "stage.h"
#include <math.h>

SET_STAGEINFO(CStage)

/*!
 *	@brief コンストラクタ
 */
CStage::CStage(SDI_STAGEINFO2 *info) : CStageBase(info) {
  bitmap = NULL;
  particle = NULL;
  particle_front = NULL;

  aki3d.Initialize(GetD3DDevice(), GetDLLPath());
}

/*!
 *	@brief デストラクタ
 *	読み込んだimage1.bmpを破棄する
 */
CStage::~CStage() { UnloadBitMap(bitmap); }

/*!
 *	@brief 初期化
 *	image1.bmpを読み込む。
 */
void CStage::InitializeStage() {
  // image1.bmpをロード
  char filename[256];
  sprintf(filename, "%s\\back1", GetDLLPath());
  bitmap = LoadImage(filename, NULL);

  particle = aki3d.CreateParticleObject();
  particle->Create(MAXNUM_SNOW_DRAW, "snow.png");

  particle_front = aki3d.CreateParticleObject();
  particle_front->Create(MAXNUM_SNOW_FRONT_DRAW, "snow.png");

  //雪の初期位置を設定

  for (int i = 0; i < MAXNUM_SNOW; i++) {
    snow[i].Init(FALSE);
  }
  for (int i = 0; i < MAXNUM_SNOW_FRONT; i++) {
    snow_front[i].Init(TRUE);
  }
}

/*!
 *	@brief 描画
 *	image1.bmpをステージ全体に描画する
 */
DWORD CStage::DrawBack() {
  //ビットマップ描画
  if (bitmap != NULL) {
    //転送元・ビットマップ全体
    RECT src;
    src.left = src.top = 0;
    src.right = (long)bitmap->wg;
    src.bottom = (long)bitmap->hg;

    //転送先
    MYRECT3D dst;
    if (bitmap->wg != 640)
      dst.z = 640.0f / (bitmap->wg - 640) - 1.0f;
    dst.z = 0.03f;
    dst.left = -(dst.z + 2.0f) * (640.0f / 480.0f);
    dst.right = (dst.z + 2.0f) * (640.0f / 480.0f);
    dst.top = (-1.0f - (float)tan(40.0 / 180.0 * 3.1415926)) * (1.0f + dst.z);
    dst.bottom = (1.0f - (float)tan(40.0 / 180.0 * 3.1415926)) * (1.0f + dst.z);

    //描画
    Blt3D(bitmap, src, dst, pdat->color);
  }

  //雪描画
  aki3d.EnableZ(FALSE, FALSE);
  aki3d.SetBlend_Add();
  particle->Render(NULL);
  aki3d.EnableZ();
  aki3d.SetBlend_Normal();

  return FALSE; //デフォルト描画は行わない
}

DWORD CStage::DrawFront() {
  aki3d.EnableZ(FALSE, FALSE);
  aki3d.SetBlend_Add();
  particle_front->Render(NULL);
  aki3d.EnableZ();
  aki3d.SetBlend_Normal();

  return 0;
}

DWORD CStage::Action() {
  int i, dn;
  float center_x = (float)GetDisplayCenter_X();
  float min_x = (center_x - 330.0f) / 240.0f;
  float max_x = (center_x + 330.0f) / 240.0f;

  //後ろ
  for (i = 0; i < MAXNUM_SNOW; i++) {
    snow[i].Update(FALSE);
  }
  dn = 0;
  for (i = 0; i < MAXNUM_SNOW; i++) {
    if (snow[i].x > min_x && snow[i].x < max_x) {
      particle->SetPos(dn, V3d(snow[i].x, snow[i].y, snow[i].z));
      particle->SetWidth(dn, snow[i].width);
      particle->SetColorDW(dn, 0xAAFFFFFF);
      particle->SetAxis(dn, Zero);

      dn++;
    }
    if (dn >= MAXNUM_SNOW_DRAW - 1)
      break;
  }
  for (; dn < MAXNUM_SNOW_DRAW; dn++) {
    particle->SetColorDW(dn, 0);
    particle->SetWidth(dn, 0);
  }

  //前
  for (i = 0; i < MAXNUM_SNOW_FRONT; i++) {
    snow_front[i].Update(TRUE);
  }
  dn = 0;
  for (i = 0; i < MAXNUM_SNOW_FRONT; i++) {
    if (snow_front[i].x > min_x && snow_front[i].x < max_x) {
      particle_front->SetPos(dn, V3d(snow_front[i].x, snow_front[i].y, snow_front[i].z));
      particle_front->SetWidth(dn, snow_front[i].width);
      particle_front->SetColorDW(dn, 0xAAFFFFFF);
      particle_front->SetAxis(dn, Zero);

      dn++;
    }
    if (dn >= MAXNUM_SNOW_FRONT_DRAW - 1)
      break;
  }
  for (; dn < MAXNUM_SNOW_FRONT_DRAW; dn++) {
    particle_front->SetColorDW(dn, 0);
    particle_front->SetWidth(dn, 0);
  }

  return 0;
}

void CStage::SNOWPARAMS::Init(BOOL front) {
  y = (-2.5f / 200.0f) * (rand() % 200);
  z = aki3d.RandomOne() * (front ? -1.5f : 1.0f);

  isou = 0.001f * (rand() % 300);
  amp = 0.005f * (rand() % 10);
  vy = 0.001f * (5 + rand() % 15);
  vx = -0.001f * (rand() % 10);
  base_x = (aki3d.RandomOne2() * 800.0f + 100.0f) / 240.0f; //-2.5f + (5.0f/200.0f)*(rand()%200);

  width = (2.0f + aki3d.RandomOne() * 4.0f) / 240.0f;
}

void CStage::SNOWPARAMS::Update(BOOL front) {
  y += vy;
  if (y > 0) {
    Init(front);
    y = -2.5f;
  }
  base_x += vx;
  x = base_x + amp * sinf(isou);

  isou += 0.05f;
}
