
/*----------------------------------------------------------------------

        ロード画面

------------------------------------------------------------------------*/

#include "stdafx.h"
#include "task_loading.h"
#include "global.h"

void CTNowLoading::Initialize() {
  tex_fb = g_draw.GetFrontBufferCopy();
  if (!tex_fb) {
    OutputDebugString("CTNowLoading: フロントバッファコピー失敗\n");
  }

  tex_nowload = NULL;
  tex_dll = NULL;
  tex_img = NULL;
  tex_gcd = NULL;
  tex_AA = NULL;

  D3DXCreateTextureFromFileA(g_draw.d3ddev, "system\\texture\\nowload.png", &tex_nowload);
  D3DXCreateTextureFromFileA(g_draw.d3ddev, "system\\texture\\load_dll.png", &tex_dll);
  D3DXCreateTextureFromFileA(g_draw.d3ddev, "system\\texture\\load_img.png", &tex_img);
  D3DXCreateTextureFromFileA(g_draw.d3ddev, "system\\texture\\load_gcd.png", &tex_gcd);
  D3DXCreateTextureFromFileA(g_draw.d3ddev, "system\\texture\\Loading1.png", &tex_AA);

  vb[0].z = 0.0f;
  vb[1].z = 0.0f;
  vb[2].z = 0.0f;
  vb[3].z = 0.0f;

  vb[0].tu = 0.0f;
  vb[1].tu = 0.0f;
  vb[2].tu = 1.0f;
  vb[3].tu = 1.0f;

  vb[0].tv = 0.0f;
  vb[1].tv = 1.0f;
  vb[2].tv = 0.0f;
  vb[3].tv = 1.0f;
}

void CTNowLoading::Terminate() {
  RELEASE(tex_AA);
  RELEASE(tex_fb);
  RELEASE(tex_nowload);
  RELEASE(tex_dll);
  RELEASE(tex_img);
  RELEASE(tex_gcd);
}

void CTNowLoading::Proceed(NowLoading_IconItem item) {
  m_iconlist.push_back(item);

  g_draw.StartDraw(TRUE);
  DrawIcon();
  g_draw.EndDraw();
}

void CTNowLoading::Progress(NowLoading_IconItem item, float value) {
  g_draw.StartDraw(TRUE);

  DrawIcon();

  g_draw.SetAlphaMode(GBLEND_KASAN);

  float ar = 320.0f / 240.0f;

  const float x_pitch = 0.13f * ar;
  const float x_left = (1.0f * ar - x_pitch * 3);
  const UINT x_num = 7;
  const float y_top = 0.35f;
  const float y_pitch = 0.10f;
  const float w = 16.0f / 240.0f * ar;
  const float h = 16.0f / 240.0f;
  const int i = m_iconlist.size();

  float x = x_left + x_pitch * (i % x_num);
  float y = y_top + y_pitch * (i / x_num);

  vb[0].x = x - w;
  vb[1].x = x - w;
  vb[2].x = x - w + (w * 2 * value);
  vb[3].x = x - w + (w * 2 * value);

  vb[0].y = y - h;
  vb[1].y = y + h;
  vb[2].y = y - h;
  vb[3].y = y + h;

  vb[2].tu = value;
  vb[3].tu = value;

  LPDIRECT3DTEXTURE8 tex = NULL;
  switch (item) {
  case NowLoading_DLL: {
    vb[0].color = vb[1].color = vb[2].color = vb[3].color = 0xFFFF7755;
    tex = tex_dll;
  } break;
  case NowLoading_Image: {
    vb[0].color = vb[1].color = vb[2].color = vb[3].color = 0xFF77FF66;
    tex = tex_img;
  } break;
  case NowLoading_GCD: {
    vb[0].color = vb[1].color = vb[2].color = vb[3].color = 0xFF7766FF;
    tex = tex_gcd;
  } break;
  }

  if (tex) {
    g_draw.d3ddev->SetTexture(0, tex);
    g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vb, sizeof(MYVERTEX3D));
  }

  vb[2].tu = 1.0f;
  vb[3].tu = 1.0f;
  g_draw.SetAlphaMode(0);
  g_draw.EndDraw();
}

void CTNowLoading::DrawIcon() {
  g_draw.EnableZ(FALSE, FALSE);
  g_draw.SetTransform(FALSE);
  g_draw.d3ddev->SetVertexShader(FVF_3DVERTEX);

  float ar = 320.0f / 240.0f;

  //フロントバッファをコピーしたやつ
  if (tex_fb) {
    vb[0].color = vb[1].color = vb[2].color = vb[3].color = 0xFF777777;

    vb[0].x = 0.0f * ar;
    vb[1].x = 0.0f * ar;
    vb[2].x = 2.0f * ar;
    vb[3].x = 2.0f * ar;

    vb[0].y = 0.0f;
    vb[1].y = 2.0f;
    vb[2].y = 0.0f;
    vb[3].y = 2.0f;

    g_draw.d3ddev->SetTexture(0, tex_fb);
    g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vb, sizeof(MYVERTEX3D));
  }

  g_draw.SetAlphaMode(GBLEND_KASAN);

  //"Now Loading"
  if (tex_nowload) {
    vb[0].color = vb[1].color = vb[2].color = vb[3].color = 0xFF55FF33;

    vb[0].x = (320.0f - 64.0f) / 240.0f;
    vb[1].x = (320.0f - 64.0f) / 240.0f;
    vb[2].x = (320.0f + 64.0f) / 240.0f;
    vb[3].x = (320.0f + 64.0f) / 240.0f;

    vb[0].y = (50.0f - 16.0f) / 240.0f;
    vb[1].y = (50.0f + 16.0f) / 240.0f;
    vb[2].y = (50.0f - 16.0f) / 240.0f;
    vb[3].y = (50.0f + 16.0f) / 240.0f;

    g_draw.d3ddev->SetTexture(0, tex_nowload);
    g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vb, sizeof(MYVERTEX3D));
  }

  // AA
  if (tex_AA) {
    vb[0].color = vb[1].color = vb[2].color = vb[3].color = 0xFF66FF99;

    vb[0].x = (400.0f - 200.0f) / 240.0f;
    vb[1].x = (400.0f - 200.0f) / 240.0f;
    vb[2].x = (400.0f + 200.0f) / 240.0f;
    vb[3].x = (400.0f + 200.0f) / 240.0f;

    vb[0].y = (400.0f - 60.0f) / 240.0f;
    vb[1].y = (400.0f + 60.0f) / 240.0f;
    vb[2].y = (400.0f - 60.0f) / 240.0f;
    vb[3].y = (400.0f + 60.0f) / 240.0f;

    g_draw.d3ddev->SetTexture(0, tex_AA);
    g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vb, sizeof(MYVERTEX3D));
  }

  const float x_pitch = 0.13f * ar;
  const float x_left = (1.0f * ar - x_pitch * 3);
  const UINT x_num = 7;
  const float y_top = 0.35f;
  const float y_pitch = 0.10f;
  const float w = 16.0f / 240.0f * ar;
  const float h = 16.0f / 240.0f;

  for (UINT i = 0; i < m_iconlist.size(); i++) {
    float x = x_left + x_pitch * (i % x_num);
    float y = y_top + y_pitch * (i / x_num);

    vb[0].x = x - w;
    vb[1].x = x - w;
    vb[2].x = x + w;
    vb[3].x = x + w;

    vb[0].y = y - h;
    vb[1].y = y + h;
    vb[2].y = y - h;
    vb[3].y = y + h;

    LPDIRECT3DTEXTURE8 tex = NULL;
    switch (m_iconlist[i]) {
    case NowLoading_DLL: {
      vb[0].color = vb[1].color = vb[2].color = vb[3].color = 0xFFFF7755;
      tex = tex_dll;
    } break;
    case NowLoading_Image: {
      vb[0].color = vb[1].color = vb[2].color = vb[3].color = 0xFF77FF66;
      tex = tex_img;
    } break;
    case NowLoading_GCD: {
      vb[0].color = vb[1].color = vb[2].color = vb[3].color = 0xFF7766FF;
      tex = tex_gcd;
    } break;
    }

    if (tex) {
      g_draw.d3ddev->SetTexture(0, tex);
      g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vb, sizeof(MYVERTEX3D));
    }
  }

  g_draw.SetAlphaMode(0);
}
