
//*******************************************************************************
//  l_directdraw.cpp  CDirectDrawクラスの基本的な部分
//*******************************************************************************

#include <windows.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <stdio.h>
#include <string.h>
#include "define.h"

//#include "define_data.h"
//#include "define_system.h"
#include "l_directdraw.h"

extern RECT rcClient;
#ifdef ___GCDEDITER
CONFIG2 config2 = {0};
#endif
extern int g_DISPLAYWIDTH;
extern int g_DISPLAYHEIGHT;

//初期化************************************************************

CDirectDraw::CDirectDraw(HWND hwnd, BOOL win) {
  cdirectdrawready = FALSE;
  dd = NULL;
  d3ddev = NULL;

  ZeroMemory(ms, sizeof(ms));

  if (!InitDirectDraw(hwnd, win)) {
    MessageBox(hwnd, "描画系の初期化に失敗", "", MB_OK);
    return;
  }

  lpFont[0] = lpFont[1] = lpFont[2] = NULL;

  //最大テクスチャサイズ取得
  D3DCAPS8 cap8;
  if (dd->GetDeviceCaps(D3DADAPTER_DEFAULT, devtypenow, &cap8) != D3D_OK) {
    MessageBox(hwnd, "GetDeviceCapsに失敗", "", MB_OK);
    return;
  }
  maxtexturewidth = cap8.MaxTextureWidth;
  maxtextureheight = cap8.MaxTextureHeight;

#ifdef _DEBUG
  char msg[128];
  sprintf(msg, "■ max texture size : %d x %d \n", maxtexturewidth, maxtextureheight);
  OutputDebugString(msg);

  OutputDebugString("D3DPCMPCAPS_ALWAYS...");
  if (cap8.AlphaCmpCaps & D3DPCMPCAPS_ALWAYS)
    OutputDebugString("○\n");
  else
    OutputDebugString("×\n");
  OutputDebugString("D3DPCMPCAPS_EQUAL...");
  if (cap8.AlphaCmpCaps & D3DPCMPCAPS_EQUAL)
    OutputDebugString("○\n");
  else
    OutputDebugString("×\n");
  OutputDebugString("D3DPCMPCAPS_GREATER ...");
  if (cap8.AlphaCmpCaps & D3DPCMPCAPS_GREATER)
    OutputDebugString("○\n");
  else
    OutputDebugString("×\n");
  OutputDebugString("D3DPCMPCAPS_GREATEREQUAL...");
  if (cap8.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL)
    OutputDebugString("○\n");
  else
    OutputDebugString("×\n");
  OutputDebugString("D3DPCMPCAPS_GREATEREQUAL..."); //
  if (cap8.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL)
    OutputDebugString("○\n");
  else
    OutputDebugString("×\n");
  OutputDebugString("D3DPCMPCAPS_LESS...");
  if (cap8.AlphaCmpCaps & D3DPCMPCAPS_LESS)
    OutputDebugString("○\n");
  else
    OutputDebugString("×\n");
  OutputDebugString("D3DPCMPCAPS_LESSEQUAL...");
  if (cap8.AlphaCmpCaps & D3DPCMPCAPS_LESSEQUAL)
    OutputDebugString("○\n");
  else
    OutputDebugString("×\n");
  OutputDebugString("D3DPCMPCAPS_NEVER...");
  if (cap8.AlphaCmpCaps & D3DPCMPCAPS_NEVER)
    OutputDebugString("○\n");
  else
    OutputDebugString("×\n");
  OutputDebugString("D3DPCMPCAPS_NOTEQUAL...");
  if (cap8.AlphaCmpCaps & D3DPCMPCAPS_NOTEQUAL)
    OutputDebugString("○\n");
  else
    OutputDebugString("×\n");

#endif

  d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); //カリングなし
  d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);        //ライティング無し
  //アルファブレンディング有効
  d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  //アルファテスト *ahyahya*
  /*	if(d3ddev->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE ) !=D3D_OK)
                  OutputDebugString("■アルファテスト友好化に失敗\n");
          else {
                  d3ddev->SetRenderState( D3DRS_ALPHAREF, 0x50 );
                  d3ddev->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER );
          }*/
  d3ddev->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
  //	d3ddev->SetTextureStageState(0,D3DTSS_MINFILTER,D3DTEXF_ANISOTROPIC);
  //	d3ddev->SetTextureStageState(0,D3DTSS_MINFILTER,D3DTEXF_FLATCUBIC );
  //	d3ddev->SetTextureStageState(0,D3DTSS_MINFILTER,D3DTEXF_GAUSSIANCUBIC);//D3DTEXF_POINT);

  // x,y平面のデータセット
  d3dxplane_x.a = -1.0f; // x平面 (x=0)
  d3dxplane_x.b = 0;
  d3dxplane_x.c = 0;
  d3dxplane_x.d = 0;
  d3dxplane_y.a = 0; // y平面 (y=0)
  d3dxplane_y.b = -1.0f;
  d3dxplane_y.c = 0;
  d3dxplane_y.d = 0;

  //テクスチャに用いるフォーマット
  switch (config2.flag & 0xF0000000) {
  case CONFIG2_USETEXFMT_A4R4G4B4:
    texformat = D3DFMT_A4R4G4B4;
    break;
  case CONFIG2_USETEXFMT_A8R3G3B2:
    texformat = D3DFMT_A8R3G3B2;
    break;
  case CONFIG2_USETEXFMT_A8R8G8B8:
    texformat = D3DFMT_A8R8G8B8;
    break;
  default:
    texformat = D3DFMT_A1R5G5B5;
    break;
  }
  //フォントの作成
  lpFont[0] = CreateMyFont(8);
  lpFont[1] = CreateMyFont(16);
  lpFont[2] = CreateMyFont(32);

  D3DXMATRIX mat;
  D3DXMatrixIdentity(&mat);
  SetParentMatrix(mat, TRUE);

  camera_x = 0;
  camera_y = 0;
  camera_z = -1.0f;
  ResetTransformMatrix();

  clearbgcolor = 0xFF000000;
  cdirectdrawready = TRUE;
}

BOOL CDirectDraw::InitDirectDraw(HWND hwnd, BOOL win) {
  // Direct3D オブジェクトを作成
  dd = Direct3DCreate8(D3D_SDK_VERSION);
  if (NULL == dd) {
    MessageBox(hwnd, "Direct3DCreate8に失敗", "", MB_OK | MB_ICONSTOP);
    return (FALSE);
  }

  // Direct3D 初期化パラメータの設定
  ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
  d3dpp.BackBufferCount = 1;
  d3dpp.EnableAutoDepthStencil = TRUE;
  d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

  if (win) {
    // 現在の画面モードを取得
    D3DDISPLAYMODE d3ddm;
    if (dd->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm) != D3D_OK) {
      MessageBox(hwnd, "GetAdapterDisplayModeに失敗", "", MB_OK | MB_ICONSTOP);
      RELEASE(dd);
      return (FALSE);
    }
    // ウインドウ : 現在の画面モードを使用
    d3dpp.BackBufferFormat = d3ddm.Format;
    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hwnd;
    d3dpp.Windowed = TRUE;
    d3dpp.BackBufferWidth = 0;
    d3dpp.BackBufferHeight = 0;
  } else {
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hwnd;
    d3dpp.Windowed = FALSE;
    d3dpp.BackBufferWidth = g_DISPLAYWIDTH;
    d3dpp.BackBufferHeight = g_DISPLAYHEIGHT;
  }

  // デバイスの作成
  switch (config2.flag & 0x0F000000) {
  case CONFIG2_DEVICE_REF:
    devtypenow = D3DDEVTYPE_REF;
    break;
  case CONFIG2_DEVICE_SW:
    devtypenow = D3DDEVTYPE_SW;
    break;
  default:
    devtypenow = D3DDEVTYPE_HAL;
    break;
  }
  DWORD vertexprocessmode = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
  if (config2.flag & CONFIG2_ENABLEHVP)
    D3DCREATE_HARDWARE_VERTEXPROCESSING;

  if (D3D_OK != dd->CreateDevice(D3DADAPTER_DEFAULT, devtypenow, hwnd, vertexprocessmode, &d3dpp, &d3ddev)) {
    CleanDirectDraw();
    return (FALSE);
  }

  return (TRUE);
}

// 後始末 *****************************************************************

CDirectDraw::~CDirectDraw() { CleanDirectDraw(); }

void CDirectDraw::CleanDirectDraw() {
  RELEASE(lpFont[0]);
  RELEASE(lpFont[1]);
  RELEASE(lpFont[2]);
  for (int i = 0; i < MAXNUMGOLUAHTEXTURES; i++) {
    RelSurface(&ms[i]);
  }
  RELEASE(d3ddev);
  RELEASE(dd);
}

void CDirectDraw::StartDraw() {
  d3ddev->BeginScene();
  d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
}

void CDirectDraw::ClearBackBuffer() {
  d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clearbgcolor, 1.0f, 0);
}

void CDirectDraw::EndDraw() {
  d3ddev->EndScene();
  d3ddev->Present(NULL, NULL, NULL, NULL);
}

LPD3DXFONT CDirectDraw::CreateMyFont(DWORD h) {
  HDC hTextDC = NULL;
  HFONT hFont = NULL, hOldFont = NULL;
  LPD3DXFONT cf;

  hTextDC = CreateCompatibleDC(NULL);
  hFont = CreateFont(h, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
                     CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "ＭＳ Ｐゴシック");
  if (!hFont)
    return (NULL);
  hOldFont = (HFONT)SelectObject(hTextDC, hFont);
  HRESULT ret = D3DXCreateFont(d3ddev, hFont, &cf);

  SelectObject(hTextDC, hOldFont);
  DeleteObject(hFont);

  if (ret != D3D_OK)
    return (NULL);
  return (cf);
}
