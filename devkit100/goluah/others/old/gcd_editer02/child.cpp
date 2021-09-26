
#include <windows.h>
#include <stdio.h>
#include <d3d8.h>
#include <d3dx8.h>
#include "define.h"

#include "function.h"
#include "l_directdraw.h"
#include "define_gcd.h"
#include "function_gcd.h"

#define MYCHILDWIDTH (480)
#define MYCHILDHEIGHT (360)
#define MYCHILDCENTERX (MYCHILDWIDTH / 2)
#define MYCHILDCENTERY (MYCHILDHEIGHT * 2 / 3)

// global val.-------------------------------------------------------------------------------

extern GCD_CELLNAMES ed_hed;
extern GCD_RECT ed_r[GCDMAX_RECTANGLES];
extern GCD_CELL2 ed_c[GCDMAX_CELLS];
extern GCD_HANTEI ed_h[GCDMAX_CELLS];

extern int radio, radio2, radioview;
extern int edit_now;

BOOL mousedown = FALSE;
BOOL drawprv = FALSE;

//マウスで選択されている矩形
int sx = 0, gx = 0, sy = 0, gy = 0, mu_xpos = 0, mu_ypos = 0;

extern BOOL testmirrorx, testmirrory;

RECT rcClient;
int g_DISPLAYWIDTH;
int g_DISPLAYHEIGHT;

extern CDirectDraw *pddraw;
HWND hwc;
extern MYSURFACE *dds[12];

// function ------------------------------------------------------------------------------
LRESULT WINAPI CldMsgProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
HWND CreateChildWindow(HWND hdlg);
void WMScroll(HWND hwnd, UINT msg, DWORD wp, DWORD lp);
void MakeScrollInfo(HWND hWnd);
void CldDraw(HWND hwnd);
void CldReset() { InvalidateRect(hwc, NULL, TRUE); }
void CldRedraw() { InvalidateRect(hwc, NULL, TRUE); }

void CldMove() {
  POINT pAdj;
  pAdj.x = pAdj.y = 0;
  GetClientRect(hwc, &rcClient);
  ClientToScreen(hwc, &pAdj);
  rcClient.left += pAdj.x;
  rcClient.top += pAdj.y;
  rcClient.right += pAdj.x;
  rcClient.bottom += pAdj.y;
}

RECT GetRECTNow() {
  RECT rr;
  if (sx < gx) {
    rr.left = sx;
    rr.right = gx;
  } else {
    rr.left = gx;
    rr.right = sx;
  }
  if (sy < gy) {
    rr.top = sy;
    rr.bottom = gy;
  } else {
    rr.top = gy;
    rr.bottom = sy;
  }

  rr.left -= MYCHILDCENTERX;   // mu_xpos;
  rr.right -= MYCHILDCENTERX;  // mu_xpos;
  rr.top -= MYCHILDCENTERY;    // mu_ypos;
  rr.bottom -= MYCHILDCENTERY; // mu_ypos;

  return (rr);
}

// 描画 -----------------------------------------------------------------------------

void CldDraw(HWND hwnd) {
  drawprv = FALSE;

  PAINTSTRUCT ps;
  BeginPaint(hwnd, &ps);
  EndPaint(hwnd, &ps);

  pddraw->StartDraw();
  pddraw->ClearBackBuffer();

  // camera settings
  pddraw->camera_x = 0.0f;
  float cmz;
  switch (radioview) {
  case 0:
    cmz = -4.0f;
    break; // 25%
  case 1:
    cmz = -2.0f;
    break; // 50%
  case 2:
    cmz = -1.5f;
    break; // 75%
  case 4:
    cmz = -0.5f;
    break; // 200%
  case 5:
    cmz = -0.25f;
    break; // 400%
  default:
    cmz = -1.0f;
    break; // 100%
  }
  pddraw->camera_z = cmz;
  pddraw->camera_y = cmz / 3.0f;
  pddraw->ResetTransformMatrix();

  // center line
  pddraw->ResetParentMatrix();
  pddraw->DrawLine(0, -999, 0, 999, 0xFF000000);
  pddraw->DrawLine(-999, 0, 999, 0, 0xFF000000);

  pddraw->CellDrawED(radio2, dds, ed_c, ed_r, edit_now, 0, 0, 0, 0, //(z/rot)
                     testmirrorx, testmirrory, 1.0f, 1.0f);         // magx/y

  if (radio == 1) {
    pddraw->ResetParentMatrix();
    pddraw->HRectDraw(ed_c, ed_h, edit_now, 0, 0, 0, 0, testmirrorx, testmirrory, 1.0f, 1.0f);
  }
  if (radio == 2) {
    pddraw->ResetParentMatrix();
    pddraw->GCenterDraw(ed_c, edit_now, 0, 0, 0, 0, testmirrorx, testmirrory, 1.0f, 1.0f);
  }

  pddraw->EndDraw();

  HDC hdc = GetDC(hwnd);
  ReleaseDC(hwnd, hdc);
}

// window message handler -----------------------------------------------------------

LRESULT WINAPI CldMsgProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  int tmp;
  double mag;

  switch (msg) {
  case WM_CREATE:
    g_DISPLAYWIDTH = MYCHILDWIDTH;
    g_DISPLAYHEIGHT = MYCHILDHEIGHT;
    GetClientRect(hWnd, &rcClient);
    pddraw = new CDirectDraw(hWnd, TRUE);
    break;
  case WM_LBUTTONDOWN:
    mousedown = TRUE;
    if (radio == 1) {
      if (drawprv)
        DrawRECT(hWnd, sx, gx, sy, gy, 0);
      sx = gx = LOWORD(lparam);
      sy = gy = HIWORD(lparam);
    } else if (radio == 2) { //重心設定
      switch (radioview) {
      case 0:
        mag = 4;
        break; // 25%
      case 1:
        mag = 2;
        break; // 50%
      case 2:
        mag = 1.5;
        break; // 75%
      case 4:
        mag = 0.5;
        break; // 200%
      case 5:
        mag = 0.25;
        break; // 400%
      default:
        mag = 1.0;
      }
      ed_c[edit_now].gcx = (int)((LOWORD(lparam) - MYCHILDCENTERX) * mag);
      ed_c[edit_now].gcy = (int)((HIWORD(lparam) - MYCHILDCENTERY) * mag);
      CldRedraw();
    } else { //その他(線引きを始める)
      sx = gx = LOWORD(lparam);
      sy = gy = HIWORD(lparam);
    }
    break;
  case WM_LBUTTONUP:
    if (mousedown) {
      mousedown = FALSE;
      if (radio == 1) { //判定設定
        gx = LOWORD(lparam);
        gy = HIWORD(lparam);
      } else if (radio == 0) { //移動
        if (drawprv)
          DrawLINE(hWnd, sx, gx, sy, gy, 0); //とりあえず線を消しとく
        gx = LOWORD(lparam);
        gy = HIWORD(lparam);
        drawprv = FALSE;
        ed_c[edit_now].cell[radio2].dx += gx - sx;
        ed_c[edit_now].cell[radio2].dy += gy - sy;
        CldRedraw();
      } else if (radio == 2) { //重心(何もしない。押したときにすでに指定される)
      } else if (radio == 3) { //回転
        if (drawprv)
          DrawLINE(hWnd, sx, gx, sy, gy, 0); //とりあえず線を消しとく
        gx = LOWORD(lparam);
        ed_c[edit_now].cell[radio2].rot += gx - sx;
        CldRedraw();
      } else if (radio == 4) { //拡大(X)
        if (drawprv)
          DrawLINE(hWnd, sx, gx, sy, gy, 0); //とりあえず線を消しとく
        gx = LOWORD(lparam);
        if (gx > sx) {                          //拡大
          for (tmp = 0; tmp < gx - sx; tmp++) { // 100pixelで10%大きくなる
            ed_c[edit_now].cell[radio2].magx *= 1.001f;
          }
        } else {                                //縮小
          for (tmp = 0; tmp < sx - gx; tmp++) { // 100pixelで10%小さくなる
            ed_c[edit_now].cell[radio2].magx /= 1.001f;
          }
        }
        CldRedraw();
      } else if (radio == 5) { //拡大(Y)
        if (drawprv)
          DrawLINE(hWnd, sx, gx, sy, gy, 0); //とりあえず線を消しとく
        gy = HIWORD(lparam);
        if (gy > sy) {                          //拡大
          for (tmp = 0; tmp < gy - sy; tmp++) { // 100pixelで10%大きくなる
            ed_c[edit_now].cell[radio2].magy *= 1.001f;
          }
        } else {                                //縮小
          for (tmp = 0; tmp < sy - gy; tmp++) { // 100pixelで10%小さくなる
            ed_c[edit_now].cell[radio2].magy /= 1.001f;
          }
        }
        CldRedraw();
      }
    }
    break;
  case WM_MOUSEMOVE:
    if (mousedown) {
      if (radio == 0) { //線を引く
        if (drawprv)
          DrawLINE(hWnd, sx, gx, sy, gy, 0);
        gx = LOWORD(lparam);
        gy = HIWORD(lparam);
        DrawLINE(hWnd, sx, gx, sy, gy, 0);
        drawprv = TRUE;
      } else if (radio == 1) { //箱を描く
        if (drawprv)
          DrawRECT(hWnd, sx, gx, sy, gy, 0);
        gx = LOWORD(lparam);
        gy = HIWORD(lparam);
        DrawRECT(hWnd, sx, gx, sy, gy, 0);
        drawprv = TRUE;
      }
      if (radio == 3 || radio == 4) { //ヨコ線を引く(回転＆X伸縮)
        if (drawprv)
          DrawLINE(hWnd, sx, gx, sy, gy, 0);
        gx = LOWORD(lparam);
        //				gy=HIWORD(lparam);
        DrawLINE(hWnd, sx, gx, sy, gy, 0);
        drawprv = TRUE;
      }
      if (radio == 5) { //タテ線を引く(Y伸縮)
        if (drawprv)
          DrawLINE(hWnd, sx, gx, sy, gy, 0);
        //				gx=LOWORD(lparam);
        gy = HIWORD(lparam);
        DrawLINE(hWnd, sx, gx, sy, gy, 0);
        drawprv = TRUE;
      }
    }
    break;

  case WM_PAINT:
    CldDraw(hWnd);
    break;

  case WM_CLOSE:
    break;
  case WM_DESTROY:
    //		delete(pddraw);
    DestroyWindow(hWnd);
    return (0);
  default:
    break;
  }
  return DefWindowProc(hWnd, msg, wparam, lparam);
}

// window 生成----------------------------------------------------------------------------

HWND CreateChildWindow(HWND hdlg) {
  // Register the window class
  WNDCLASSEX wc;
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_CLASSDC;
  wc.lpfnWndProc = CldMsgProc;
  wc.cbClsExtra = 0L;
  wc.cbWndExtra = 0L;
  wc.hInstance = GetModuleHandle(NULL);
  wc.hIcon = NULL;
  wc.hCursor = NULL;
  wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = "child";
  wc.hIconSm = NULL;
  RegisterClassEx(&wc);

  // Create the application's window
  HWND hWnd = CreateWindow("child", "child", WS_CHILD, 5, 5, MYCHILDWIDTH, MYCHILDHEIGHT, hdlg, NULL,
                           wc.hInstance, NULL);
  hwc = hWnd;

  return (hWnd);
}
