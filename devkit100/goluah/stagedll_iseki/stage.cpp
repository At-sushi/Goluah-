

#include "stage.h"
#include <math.h>

#define WATERHEIGHT -0.025f
const float g_jimenheightarr[15][16] = //地面の高さ
    {9, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4, 9, 9, 6, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 6, 9, 9, 6, 1,
     0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 6, 9, 9, 6, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 6, 9, 9, 6, 1, 0, 1, 0,
     0, 0, 0, 0, 1, 0, 0, 0, 6, 9, 9, 6, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 6, 9, 9, 6, 1, 0, 0, 0, 0, 0, 0,
     0, 1, 1, 0, 0, 6, 9, 9, 6, 1, 0, 2, 1, 2, 0, 0, 2, 2, 1, 0, 0, 6, 9, 9, 6, 1, 0, 2, 4, 3, 2, 0, 0, 0, 2,
     0, 0, 6, 9, 9, 6, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1, 0, 6, 9, 9, 6, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 6,
     9, 9, 6, 2, 1, 1, 0, 0, 0, 0, 0, 1, 1, 2, 2, 6, 9, 9, 6, 3, 2, 3, 1, 1, 0, 0, 1, 1, 3, 3, 3, 4, 9, 9, 6,
     5, 5, 6, 6, 5, 5, 6, 6, 6, 5, 5, 6, 6, 9, 9, 8, 9, 8, 9, 8, 9, 8, 8, 9, 8, 9, 8, 9, 8, 9};

SET_STAGEINFO(CStage)

/*!
 *	@brief コンストラクタ
 */
CStage::CStage(SDI_STAGEINFO2 *info) : CStageBase(info) {
  ptex_water = NULL;
  ptex_jimen = NULL;
  ptex_sky = NULL;
  ptex_jisaku1 = NULL;
  ptex_forest = NULL;
}

/*!
 *	@brief デストラクタ
 */
CStage::~CStage() {
  // unload textures
  RELEASE(ptex_water);
  RELEASE(ptex_jimen);
  RELEASE(ptex_sky);
  RELEASE(ptex_jisaku1);
  RELEASE(ptex_forest);
  for (int i = 0; i < NUM_JIMEN_Z; i++)
    RELEASE(vb_jimen[i]);
}

/*!
 *	@brief 初期化
 */
void CStage::InitializeStage() {
  InitVrtx();     //頂点座標初期化
  LoadTextures(); //テクスチャ読込み

  pdat->z = -2.0f;
}

void CStage::InitVrtx() //頂点座標初期化
{
  int i, j;
  MYVERTEX3D *vtx_tmp = NULL;
  LPDIRECT3DDEVICE8 d3ddev = GetD3DDevice();

  //水面
  vb_water[0].color = vb_water[1].color = vb_water[2].color = vb_water[3].color = 0xAAFFFFFF;
  vb_water[0].y = vb_water[1].y = vb_water[2].y = vb_water[3].y = WATERHEIGHT;
  vb_water[0].x = -50;
  vb_water[0].z = 50;
  vb_water[1].x = 50;
  vb_water[1].z = 50;
  vb_water[2].x = -50;
  vb_water[2].z = 0.5;
  vb_water[3].x = 50;
  vb_water[3].z = 0.5;
  vb_water[0].tu = 0;
  vb_water[0].tv = 0;
  vb_water[1].tu = 30;
  vb_water[1].tv = 0;
  vb_water[2].tu = 0;
  vb_water[2].tv = 30;
  vb_water[3].tu = 30;
  vb_water[3].tv = 30;

  //水面・手前
  vb_maewater[0].color = vb_maewater[1].color = vb_maewater[2].color = vb_maewater[3].color = 0xAAFFFFFF;
  vb_maewater[0].y = vb_maewater[1].y = vb_maewater[2].y = vb_maewater[3].y = WATERHEIGHT;
  vb_maewater[0].x = -50;
  vb_maewater[0].z = -5;
  vb_maewater[1].x = 50;
  vb_maewater[1].z = -5;
  vb_maewater[2].x = -50;
  vb_maewater[2].z = 0.5;
  vb_maewater[3].x = 50;
  vb_maewater[3].z = 0.5;
  vb_maewater[0].tu = 0;
  vb_maewater[0].tv = 0;
  vb_maewater[1].tu = 30;
  vb_maewater[1].tv = 0;
  vb_maewater[2].tu = 0;
  vb_maewater[2].tv = 10;
  vb_maewater[3].tu = 30;
  vb_maewater[3].tv = 10;

  srand(timeGetTime());

  //地面
  MYVERTEX3D hogehoge[NUM_JIMEN_Z][(NUM_JIMEN_X + 1) * 2];
  for (i = 0; i < NUM_JIMEN_Z; i++) {
    for (j = 0; j < NUM_JIMEN_X + 1; j++) {
      if (i == 0) {
        hogehoge[i][j * 2].z = -5.0f + ((40.0f) / (float)NUM_JIMEN_Z) * i;
        hogehoge[i][j * 2].x = -20.0f + (100.0f / (float)NUM_JIMEN_X) * j;
        hogehoge[i][j * 2].y = (-0.3f) * g_jimenheightarr[i][j] - 0.005f * (rand() % 5);
        hogehoge[i][j * 2].color = 0xFFFFFFFF;
      } else
        hogehoge[i][j * 2] = hogehoge[i - 1][j * 2 + 1];
      hogehoge[i][j * 2 + 1].z = -5.0f + ((55.0f) / (float)NUM_JIMEN_Z) * (i + 1);
      hogehoge[i][j * 2 + 1].x = -20.0f + (40.0f / (float)NUM_JIMEN_X) * j;
      hogehoge[i][j * 2 + 1].y = (-0.3f) * g_jimenheightarr[i][j] - 0.005f * (rand() % 5);
      hogehoge[i][j * 2 + 1].color = 0xFFFFFFFF;

      if (j != 0) {
        if (g_jimenheightarr[i][j - 1] > g_jimenheightarr[i][j]) {
          hogehoge[i][j * 2].color = 0xFFAAAAAA;
          hogehoge[i][j * 2 + 1].color = 0xFFAAAAAA;
        }
      }

      hogehoge[i][j * 2].tu = (float)(j % 2);
      hogehoge[i][j * 2].tv = (float)(i % 2);
      hogehoge[i][j * 2 + 1].tu = (float)(j % 2);
      hogehoge[i][j * 2 + 1].tv = (float)((i + 1) % 2);
    }

    if (SUCCEEDED(d3ddev->CreateVertexBuffer(sizeof(MYVERTEX3D) * ((NUM_JIMEN_X + 1) * 2), 0, FVF_3DVERTEX,
                                             D3DPOOL_MANAGED, &vb_jimen[i])) &&
        SUCCEEDED(vb_jimen[i]->Lock(0, 0, (BYTE **)&vtx_tmp, 0))) {
      memcpy(vtx_tmp, hogehoge[i], sizeof(hogehoge[i]));
      vb_jimen[i]->Unlock();
    }
  }

  //空
  vb_sky[0].color = vb_sky[1].color = vb_sky[2].color = vb_sky[3].color = 0xFFFFFFFF;
  vb_sky[0].z = vb_sky[1].z = vb_sky[2].z = vb_sky[3].z = 45.0f;
  vb_sky[0].x = -30;
  vb_sky[0].y = -20;
  vb_sky[1].x = 30;
  vb_sky[1].y = -20;
  vb_sky[2].x = -30;
  vb_sky[2].y = 0;
  vb_sky[3].x = 30;
  vb_sky[3].y = 0;
  vb_sky[0].tu = 0;
  vb_sky[0].tv = 0;
  vb_sky[1].tu = 1;
  vb_sky[1].tv = 0;
  vb_sky[2].tu = 0;
  vb_sky[2].tv = 1;
  vb_sky[3].tu = 1;
  vb_sky[3].tv = 1;

  //自作自演
  vb_jisaku1[0].color = vb_jisaku1[1].color = 0xFFFFFFFF;
  vb_jisaku1[2].color = vb_jisaku1[3].color = 0xFF777777;
  vb_jisaku1[0].z = vb_jisaku1[1].z = vb_jisaku1[2].z = vb_jisaku1[3].z = 40.0f;
  vb_jisaku1[0].x = -8;
  vb_jisaku1[0].y = -20;
  vb_jisaku1[1].x = 8;
  vb_jisaku1[1].y = -20;
  vb_jisaku1[2].x = -8;
  vb_jisaku1[2].y = 0;
  vb_jisaku1[3].x = 8;
  vb_jisaku1[3].y = 0;
  vb_jisaku1[0].tu = 0;
  vb_jisaku1[0].tv = 0;
  vb_jisaku1[1].tu = 1;
  vb_jisaku1[1].tv = 0;
  vb_jisaku1[2].tu = 0;
  vb_jisaku1[2].tv = 1;
  vb_jisaku1[3].tu = 1;
  vb_jisaku1[3].tv = 1;

  //森
  vb_forest[0].color = vb_forest[1].color = 0xFFFFFFFF;
  vb_forest[2].color = vb_forest[3].color = 0xFF777777;
  vb_forest[0].z = vb_forest[1].z = vb_forest[2].z = vb_forest[3].z = 41.0f;
  vb_forest[0].x = -24;
  vb_forest[0].y = -10;
  vb_forest[1].x = 24;
  vb_forest[1].y = -10;
  vb_forest[2].x = -24;
  vb_forest[2].y = 0;
  vb_forest[3].x = 24;
  vb_forest[3].y = 0;
  vb_forest[0].tu = 0;
  vb_forest[0].tv = 0;
  vb_forest[1].tu = 6;
  vb_forest[1].tv = 0;
  vb_forest[2].tu = 0;
  vb_forest[2].tv = 1;
  vb_forest[3].tu = 6;
  vb_forest[3].tv = 1;
}

void CStage::LoadTextures() //テクスチャ読込み
{
  LPDIRECT3DDEVICE8 d3ddev = GetD3DDevice();

  char *filename = (char *)malloc(256);
  if (filename == NULL)
    return;

  //水面
  ptex_water = NULL;
  sprintf(filename, "%s\\tex_water.bmp", GetDLLPath());
  D3DXCreateTextureFromFileA(d3ddev, filename, &ptex_water);
  //地面
  ptex_jimen = NULL;
  sprintf(filename, "%s\\tex_gnd.bmp", GetDLLPath());
  D3DXCreateTextureFromFileExA(d3ddev, filename, 0, 0, 0, 0, D3DFMT_X1R5G5B5, D3DPOOL_MANAGED,
                               D3DX_FILTER_POINT, D3DX_FILTER_POINT, 0, NULL, NULL, &ptex_jimen);
  //空
  ptex_sky = NULL;
  sprintf(filename, "%s\\tex_sky.png", GetDLLPath());
  D3DXCreateTextureFromFileExA(d3ddev, filename, 0, 0, 0, 0, D3DFMT_X1R5G5B5, D3DPOOL_MANAGED,
                               D3DX_FILTER_POINT, D3DX_FILTER_POINT, 0, NULL, NULL, &ptex_sky);

  //自作自演の遺跡（？）
  ptex_jisaku1 = NULL;
  sprintf(filename, "%s\\jien.png", GetDLLPath());
  D3DXCreateTextureFromFileExA(d3ddev, filename, 0, 0, 0, 0, D3DFMT_A1R5G5B5, D3DPOOL_MANAGED,
                               D3DX_FILTER_POINT, D3DX_FILTER_POINT, 0xFFFFFFFF, NULL, NULL, &ptex_jisaku1);

  //森（のつもり）
  ptex_forest = NULL;
  sprintf(filename, "%s\\tex_forest.bmp", GetDLLPath());
  D3DXCreateTextureFromFileExA(d3ddev, filename, 0, 0, 0, 0, D3DFMT_A1R5G5B5, D3DPOOL_MANAGED,
                               D3DX_FILTER_POINT, D3DX_FILTER_POINT, 0xFFFFFFFF, NULL, NULL, &ptex_forest);

  free(filename);
}

/*!
 *	@brief 描画
 */
DWORD CStage::DrawBack() {
  LPDIRECT3DDEVICE8 d3ddev = GetD3DDevice();
  if (d3ddev == NULL)
    return FALSE;

  //テクスチャアドレッシングモード-繰り返し
  d3ddev->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_MIRROR);
  d3ddev->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_MIRROR);

  d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(5, 0, 20), 1.0f, 0);

  //座標変換-なし
  D3DXMATRIX mati;
  D3DXMatrixIdentity(&mati);
  d3ddev->SetTransform(D3DTS_WORLD, &mati);

  //描画
  DrawJisaku1();
  DrawJimen();
  DrawWater();
  DrawSky();

  //テクスチャアドレシングモード - 元に戻す
  d3ddev->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
  d3ddev->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);

  return FALSE; //通常、デフォルト描画は行わない
}

void CStage::DrawWater() {
  LPDIRECT3DDEVICE8 d3ddev = GetD3DDevice();

  //水面
  d3ddev->SetTexture(0, ptex_water);
  d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vb_water, sizeof(MYVERTEX3D));
}

void CStage::DrawSky() {
  LPDIRECT3DDEVICE8 d3ddev = GetD3DDevice();

  //そら
  d3ddev->SetTexture(0, ptex_sky);
  d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vb_sky, sizeof(MYVERTEX3D));
}

void CStage::DrawJimen() {
  LPDIRECT3DDEVICE8 d3ddev = GetD3DDevice();

  //地面
  d3ddev->SetTexture(0, ptex_jimen);
  for (int i = 0; i < NUM_JIMEN_Z; i++) {
    d3ddev->SetStreamSource(0, vb_jimen[i], sizeof(MYVERTEX3D));
    d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_JIMEN_X * 2);
  }
}

void CStage::DrawJisaku1() {
  LPDIRECT3DDEVICE8 d3ddev = GetD3DDevice();

  //自作自演の遺跡（？）
  d3ddev->SetTexture(0, ptex_jisaku1);
  d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vb_jisaku1, sizeof(MYVERTEX3D));

  //森
  d3ddev->SetTexture(0, ptex_forest);
  d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vb_forest, sizeof(MYVERTEX3D));
}

//*******************************************************
//　手前描画
//*******************************************************
DWORD CStage::Draw() {
  LPDIRECT3DDEVICE8 d3ddev = GetD3DDevice();
  if (!d3ddev)
    return FALSE;

  //座標変換-なし
  D3DXMATRIX mati;
  D3DXMatrixIdentity(&mati);
  d3ddev->SetTransform(D3DTS_WORLD, &mati);

  d3ddev->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_MIRROR);
  d3ddev->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_MIRROR);

  d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

  //水面
  d3ddev->SetTexture(0, ptex_water);
  d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vb_maewater, sizeof(MYVERTEX3D));

  d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

  d3ddev->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
  d3ddev->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);

  return FALSE;
}

//*******************************************************
//　動き
//*******************************************************
DWORD CStage::Action() {
  for (int i = 0; i < 4; i++) {
    vb_water[i].tu += 0.0005f;
    vb_water[i].tv += 0.0002f;
    vb_maewater[i].tu += 0.0005f;
    vb_maewater[i].tv += 0.0002f;
  }
  return TRUE;
}
