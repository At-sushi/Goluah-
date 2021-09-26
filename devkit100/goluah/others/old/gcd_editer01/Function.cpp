#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <ddraw.h>
#include "define.h"

#include "define_gcd.h"
#include "function.h"

// global val.---------------------------------------------------------------------------
DWORD bmp_loaded[GCDMAX_IMAGES] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
BMPINFO256 bmp_info[GCDMAX_IMAGES];
BMPINFO256 bmp_info2[GCDMAX_IMAGES];
PBYTE bmp_bits[GCDMAX_IMAGES] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

// global val.=================================================================
HWND ghwnd; //メインウインドウのハンドル
char datfilepath[256];

//******************************************************************************
// 描画関連
//******************************************************************************

void DrawRECT(HWND hwnd, int sx, int gx, int sy, int gy, DWORD type) {
  HDC hdc = GetDC(hwnd);
  DrawRECT2(hdc, sx, gx, sy, gy, type);
  ReleaseDC(hwnd, hdc);
}

void DrawRECT2(HDC hdc, int sx, int gx, int sy, int gy, DWORD type) {
  COLORREF pencolor;

  switch (type) {
  case 1:
    pencolor = RGB(0, 0, 255);
    break;
  case 2:
    pencolor = RGB(255, 0, 0);
    break;
  case 3:
    pencolor = RGB(255, 255, 0);
    break;
  default:
    pencolor = RGB(0, 0, 100);
    break;
  }

  HPEN penold, pen = CreatePen(PS_SOLID, 0, pencolor);
  penold = (HPEN)SelectObject(hdc, pen);

  int oldrop;
  if (type == 0)
    oldrop = SetROP2(hdc, R2_NOT);

  MoveToEx(hdc, sx, sy, NULL);
  LineTo(hdc, sx, gy);
  LineTo(hdc, gx, gy);
  LineTo(hdc, gx, sy);
  LineTo(hdc, sx, sy);

  if (type == 0)
    SetROP2(hdc, oldrop);

  SelectObject(hdc, penold);
}

void DrawLINE(HWND hwnd, int sx, int gx, int sy, int gy, DWORD type) {
  HDC hdc = GetDC(hwnd);

  COLORREF pencolor;

  switch (type) {
  case 1:
    pencolor = RGB(0, 0, 255);
    break;
  case 2:
    pencolor = RGB(255, 0, 0);
    break;
  case 3:
    pencolor = RGB(255, 255, 0);
    break;
  default:
    pencolor = RGB(0, 0, 100);
    break;
  }

  HPEN penold, pen = CreatePen(PS_SOLID, 0, pencolor);
  penold = (HPEN)SelectObject(hdc, pen);

  int oldrop;
  if (type == 0)
    oldrop = SetROP2(hdc, R2_NOT);

  MoveToEx(hdc, sx, sy, NULL);
  LineTo(hdc, gx, gy);

  if (type == 0)
    SetROP2(hdc, oldrop);

  SelectObject(hdc, penold);

  ReleaseDC(hwnd, hdc);
}

void DrawPos(HDC hdc, int x, int y, BOOL not ) {
  HPEN penold, pen = CreatePen(PS_SOLID, 0, RGB(0, 0, 255));
  penold = (HPEN)SelectObject(hdc, pen);

  int oldrop;
  if (not )
    oldrop = SetROP2(hdc, R2_NOT);

  MoveToEx(hdc, x - 5, y, NULL);
  LineTo(hdc, x + 5, y);
  MoveToEx(hdc, x, y - 5, NULL);
  LineTo(hdc, x, y + 5);

  HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
  HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
  Ellipse(hdc, x - 5, y - 5, x + 5, y + 5);
  SelectObject(hdc, hOldBrush);

  if (not )
    SetROP2(hdc, oldrop);
  SelectObject(hdc, penold);
}

//******************************************************************************
//ビットマップ読み込み関連
//******************************************************************************

// bmp_loaded[] の状態
// 0:正常に読み込まれている
// 1:まだ読み込まれていない
// 2:ファイルが存在しないか、または開けなかった
// 3:ファイルの読み出し中に失敗した
// 4:というか、ビットマップではなかった
// 5:ファイルヘッダに何らかの不都合があった(256でないとか)

BOOL ChangeBmp2(DWORD n) {
  if (bmp_loaded[n] == 0) {
    if (bmp_bits[n] != NULL) {
      free(bmp_bits[n]);
      bmp_bits[n] = NULL;
    }
  }

  char filename[256];
  sprintf(filename, "%s\\image%d.bmp", datfilepath, n + 1);

  HANDLE hFile = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE) {
    bmp_loaded[n] = 2;
    //		MessageBox(ghwnd,"ﾌｧｲﾙが開けなかった","ChangeBmp()",MB_OK);
    return (FALSE);
  }

  SetFilePointer(hFile, 0, NULL, FILE_BEGIN); //念のためファイルの先頭に移動

  BOOL err = FALSE;

  //ﾌｧｲﾙヘッダの読み出し
  BITMAPFILEHEADER fileheader;
  DWORD br;
  DWORD ret = ReadFile(hFile, &fileheader, sizeof(BITMAPFILEHEADER), &br, NULL);
  if (!ret || br != sizeof(BITMAPFILEHEADER)) {
    bmp_loaded[n] = 3;
    //		MessageBox(ghwnd,"ファイルの読み込みに失敗(1)\n","ChangeBmp()",MB_OK);
    err = TRUE;
  }
  if (fileheader.bfType != 0x4d42) { //"BM"
    bmp_loaded[n] = 4;
    //		MessageBox(ghwnd,"つーかこのファイルはビットマップではない\n","ChangeBmp()",MB_OK);
    err = TRUE;
  }
  if (err) {
    CloseHandle(hFile);
    return (FALSE);
  }

  // BITMAPINFOHEADERの読み込み
  ret = ReadFile(hFile, &bmp_info[n].hed, sizeof(BITMAPINFOHEADER), &br, NULL);
  if (!ret || br != sizeof(BITMAPINFOHEADER)) {
    //		MessageBox(ghwnd,"ファイルの読み込みに失敗(2)\n","ChangeBmp()",MB_OK);
    bmp_loaded[n] = 3;
    err = TRUE;
  }
  if (bmp_info[n].hed.biSize != sizeof(BITMAPINFOHEADER)) {
    //		MessageBox(ghwnd,"BITMAPINFOHEADERのサイズが合わない\n","ChangeBmp()",MB_OK);
    bmp_loaded[n] = 5;
    err = TRUE;
  }
  if (bmp_info[n].hed.biBitCount != 8) {
    bmp_loaded[n] = 2;
    //		MessageBox(ghwnd,"つーかこのファイルは256ではない\n","ChangeBmp()",MB_OK);
    bmp_loaded[n] = 5;
    err = TRUE;
  }
  if (bmp_info[n].hed.biCompression != BI_RGB) {
    //		MessageBox(ghwnd,"圧縮がかかっているらしい\n","ChangeBmp()",MB_OK);
    bmp_loaded[n] = 5;
    err = TRUE;
  }
  if (bmp_info[n].hed.biClrUsed == 0) {
    bmp_info[n].hed.biClrUsed = 256;
  }
  if (err) {
    CloseHandle(hFile);
    return (FALSE);
  }

  //パレットの読み込み
  ret = ReadFile(hFile, bmp_info[n].pal, sizeof(RGBQUAD) * bmp_info[n].hed.biClrUsed, &br, NULL);
  if (!ret || br != sizeof(RGBQUAD) * bmp_info[n].hed.biClrUsed) {
    //		MessageBox(ghwnd,"ファイルの読み込みに失敗(3)\n","ChangeBmp()",MB_OK);
    bmp_loaded[n] = 3;
    CloseHandle(hFile);
    return (FALSE);
  }
  bmp_info2[n].hed = bmp_info[n].hed;
  for (DWORD i = 0; i < 255; i++) {
    bmp_info2[n].pal[i].rgbBlue = 0;
    bmp_info2[n].pal[i].rgbRed = 0;
    bmp_info2[n].pal[i].rgbGreen = 0;
    bmp_info2[n].pal[i].rgbReserved = 0;
  }
  bmp_info2[n].pal[255].rgbBlue = 0xFF;
  bmp_info2[n].pal[255].rgbRed = 0xFF;
  bmp_info2[n].pal[255].rgbGreen = 0xFF;
  bmp_info2[n].pal[255].rgbReserved = 0;

  bmp_info[n].pal[255].rgbBlue = 0;
  bmp_info[n].pal[255].rgbRed = 0;
  bmp_info[n].pal[255].rgbGreen = 0;
  bmp_info[n].pal[255].rgbReserved = 0;

  //ビットマップビットのサイズを計算する(ビットマップの幅は4の倍数で格納されているらしい)
  DWORD sizeimage = bmp_info[n].hed.biWidth;
  if (bmp_info[n].hed.biWidth % 4 != 0)
    sizeimage += (4 - bmp_info[n].hed.biWidth % 4);
  sizeimage *= bmp_info[n].hed.biHeight;
  //メモリを確保してビットマップビットを読み込む
  bmp_bits[n] = (PBYTE)malloc(sizeimage);
  ret = ReadFile(hFile, bmp_bits[n], sizeimage, &br, NULL);
  CloseHandle(hFile);

  bmp_loaded[n] = 0;

  return (TRUE);
}

BOOL ChangeBmp(DWORD n) {
  ChangeBmp2(n);
  return (TRUE);
}

BOOL BitmapReflesh() {
  for (int i = 0; i < GCDMAX_IMAGES; i++) {
    ChangeBmp(i);
  }
  return (TRUE);
}
