
//*******************************************************************************
//  l_directdraw2.cpp ビットマップのロード関連
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

extern int g_DISPLAYWIDTH;
extern int g_DISPLAYHEIGHT;

// BOOL outtest=FALSE;

extern CONFIG2 config2;

// ビットマップをMYSURFACEとしてロード *******************************************
MYSURFACE *CDirectDraw::CreateSurfaceFrom256BMP(char *filename, char *palname, BOOL dmy) {
  BOOL bret;
  DWORD i, j;

  //未使用のものを探す
  int e = -1;
  for (i = 0; i < MAXNUMGOLUAHTEXTURES; i++) {
    if (!ms[i].valid) {
      e = i;
      i = MAXNUMGOLUAHTEXTURES;
    }
  }
  if (e < 0) {
    OutputDebugString("warning:テクスチャ数がMAXを超えています\n");
    return (NULL);
  }

  //ビットマップのビットを読み込み
  MYPALLET *bmpbits;
  DWORD bmpwidth, bmpheight;
  bret = Load256Bitmapbits(&bmpbits, &bmpwidth, &bmpheight, filename, palname);
  if (!bret) {
    OutputDebugString("Load256Bitmapbitsに失敗\n");
    return (NULL);
  }
  ms[e].wg = (float)bmpwidth;
  ms[e].hg = (float)bmpheight;

  //ビットマップの分割の仕方を決定
  bret = AssignTextureDiv(&ms[e], bmpwidth, bmpheight);
  if (!bret) {
    OutputDebugString("AssignTextureDivに失敗\n");
    return (NULL);
  }

  //指定サイズのテクスチャを作成
  LPVOID pointer;
  pointer = malloc(sizeof(LPDIRECT3DTEXTURE8) * (ms[e].xsufnum * ms[e].ysufnum + 1));
  ZeroMemory(pointer, sizeof(LPDIRECT3DTEXTURE8) * (ms[e].xsufnum * ms[e].ysufnum + 1));
  ms[e].pTex = (LPDIRECT3DTEXTURE8 *)pointer; //ポインタを確保する領域を確保
  for (i = 0; i < ms[e].ysufnum; i++) {
    for (j = 0; j < ms[e].xsufnum; j++) {
      d3ddev->CreateTexture(ms[e].xsufsize[j], ms[e].ysufsize[i],
                            1,         // mipmap level
                            0,         // D3DUSAGE_RENDERTARGET,
                            texformat, // D3DFMT_A8R8G8B8,//D3DFMT_R5G6B5,//D3DFMT_R5G6B5,
                            D3DPOOL_MANAGED, &(ms[e].pTex[i * ms[e].xsufnum + j]));
    }
  }

  DWORD dameyox = bmpwidth % MINIMUM_TEXSIZE;
  DWORD dameyoy = bmpheight % MINIMUM_TEXSIZE;

  //	char msg[256];

  //テクスチャにビットマップをコピー
  //	OutputDebugString("▼テクスチャにビットマップをコピー\n");
  DWORD damex, damey;
  for (i = 0; i < ms[e].ysufnum; i++) { // okasi?
    for (j = 0; j < ms[e].xsufnum; j++) {
      //コピー実行
      //			sprintf(msg,"%d : %d,%d - offset%d/%d ...",i*ms[e].xsufnum + j,i,j,ofx,ofy);
      //			OutputDebugString(msg);
      if (i == ms[e].ysufnum - 1) {
        damey = dameyoy;
      } else
        damey = 0;
      if (j == ms[e].xsufnum - 1) {
        damex = dameyox;
      } else
        damex = 0;
      CopyBB2TS(bmpbits, bmpwidth, ms[e].xsufindx[j], ms[e].ysufindx[i], ms[e].pTex[(i * ms[e].xsufnum + j)],
                damex, damey);
    }
  }

  //後始末
  free(bmpbits);

  //成功
  ms[e].valid = TRUE;
  /*
  #ifdef _DEBUG
          //test
          OutputDebugString("▼CreateSurfaceFromBMP - complete\n");
          char msg[256];
          int numemptytex=0;
          for(i=0;i<MAXNUMGOLUAHTEXTURES;i++)
                  if(!ms[i].valid)numemptytex++;
          sprintf(msg,"(ms残り%d)\n",numemptytex);
          OutputDebugString(msg);
          sprintf(msg,"テクスチャ数：%dx%d (x/ysufnum)\n",ms[e].xsufnum,ms[e].ysufnum);
          OutputDebugString(msg);
          sprintf(msg,"index-x : ");//indexx
          OutputDebugString(msg);
          sprintf(msg,"");
          for(i=0;i<MYSUF_MAXNUMTEX;i++){
                  sprintf(&msg[strlen(msg)]," %d,",ms[e].xsufindx[i]);
          }
          sprintf(&msg[strlen(msg)],"\n");
          OutputDebugString(msg);
          sprintf(msg,"index-y : ");//indexy
          OutputDebugString(msg);
          sprintf(msg,"");
          for(i=0;i<MYSUF_MAXNUMTEX;i++){
                  sprintf(&msg[strlen(msg)]," %d,",ms[e].ysufindx[i]);
          }
          sprintf(&msg[strlen(msg)],"\n");
          OutputDebugString(msg);
          sprintf(msg,"size-x : ");//size-x
          OutputDebugString(msg);
          sprintf(msg,"");
          for(i=0;i<MYSUF_MAXNUMTEX;i++){
                  sprintf(&msg[strlen(msg)]," %d,",ms[e].xsufsize[i]);
          }
          sprintf(&msg[strlen(msg)],"\n");
          OutputDebugString(msg);
          sprintf(msg,"size-y : ");//size-y
          OutputDebugString(msg);
          sprintf(msg,"");
          for(i=0;i<MYSUF_MAXNUMTEX;i++){
                  sprintf(&msg[strlen(msg)]," %d,",ms[e].ysufsize[i]);
          }
          sprintf(&msg[strlen(msg)],"\n");
          OutputDebugString(msg);

          sprintf(msg,"textures");
          OutputDebugString(msg);
          sprintf(msg,"");
          for(i=0;i<ms[e].xsufnum*ms[e].ysufnum;i++){
                  if(i%ms[e].xsufnum==0)sprintf(&msg[strlen(msg)],"\n");
                  if(ms[e].pTex[i]!=NULL)sprintf(&msg[strlen(msg)],"●");
                  else sprintf(&msg[strlen(msg)],"○");
          }
          sprintf(&msg[strlen(msg)],"\n");
          OutputDebugString(msg);

  #endif //↑debug//
  */
  return (&ms[e]);
}

//テクスチャのサーフェースに書き込み*******************************************************************************
BOOL CDirectDraw::CopyBB2TS(MYPALLET *pbb,
                            DWORD bbpitch,
                            DWORD offset_x,
                            DWORD offset_y,
                            LPDIRECT3DTEXTURE8 ptex,
                            DWORD damex,
                            DWORD damey) {
  if (pbb == NULL)
    return (FALSE);
  if (ptex == NULL)
    return (FALSE);

  LPDIRECT3DSURFACE8 psuf = NULL;
  if (D3D_OK != ptex->GetSurfaceLevel(0, &psuf)) {
    OutputDebugString("CopyBB2TS / GetSurfaceLevelに失敗\n");
    return (FALSE);
  }

  //幅と高さ、フォーマットを取得
  D3DFORMAT fmt;
  DWORD sw, sh;
  D3DSURFACE_DESC dsc;
  if (D3D_OK != psuf->GetDesc(&dsc)) {
    OutputDebugString("CopyBB2TS / GetDescに失敗\n");
    RELEASE(psuf);
    return (FALSE);
  }
  fmt = dsc.Format;
  sw = dsc.Width;
  sh = dsc.Height;

  if (damex != 0)
    sw = damex;
  if (damey != 0)
    sh = damey;

  /*	char msg[256];
          sprintf(msg,"surface %dx%d ",sw,sh);
          OutputDebugString(msg);*/

  //サーフェイスのロック
  D3DLOCKED_RECT lr;
  if (D3D_OK != psuf->LockRect(&lr, NULL, 0)) {
    OutputDebugString("CopyBB2TS / LockRectに失敗\n");
    RELEASE(psuf);
    return (FALSE);
  }

  //コピー
  DWORD i, j;
  PBYTE pline;
  int onepixsize;
  for (j = 0; j < sh; j++) {
    pline = (PBYTE)lr.pBits + lr.Pitch * j;
    //		sprintf(msg,"line %d\n",j);
    //		if(outtest && j>96 )OutputDebugString(msg);
    for (i = 0; i < sw; i++) {
      switch (fmt) {
      case D3DFMT_R5G6B5: //これは多分使えない
        onepixsize = CopyOne_R5G6B5(pbb[(j + offset_y) * bbpitch + i + offset_x], pline);
        break;
      case D3DFMT_A8R8G8B8:
        i = i;
        j = j;
        onepixsize = CopyOne_A8R8G8B8(pbb[(j + offset_y) * bbpitch + i + offset_x], pline);
        break;
      case D3DFMT_A1R5G5B5:
        onepixsize = CopyOne_A1R5G5B5(pbb[(j + offset_y) * bbpitch + i + offset_x], pline);
        break;
      case D3DFMT_A4R4G4B4:
        onepixsize = CopyOne_A4R4G4B4(pbb[(j + offset_y) * bbpitch + i + offset_x], pline);
        break;
      case D3DFMT_A8R3G3B2:
        onepixsize = CopyOne_A8R3G3B2(pbb[(j + offset_y) * bbpitch + i + offset_x], pline);
        break;
      default:
        OutputDebugString("CopyBB2TS / このフォーマットはコピーできにゃい\n");
        psuf->UnlockRect();
        RELEASE(psuf);
        return (FALSE);
      }
      pline += onepixsize;
      //			sprintf(msg," %d,",i);
      //			OutputDebugString(msg);
    }
  }
  //	sprintf(msg,"... %d/%d\n",i,j);
  //	OutputDebugString(msg);

  psuf->UnlockRect();
  RELEASE(psuf);

  return (TRUE);
}

// 1 ピクセルの書き込み処理********************************************************************************
DWORD CDirectDraw::CopyOne_R5G6B5(MYPALLET src, PBYTE dst) {
  WORD *pdst;
  pdst = (PWORD)dst;

  WORD r, g, b;
  r = src.red;
  r *= 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
  r &= 0xF800;

  g = src.green;
  g *= 2 * 2 * 2;
  g &= 0x07E0;

  b = src.blue;
  b /= 2 * 2 * 2;
  b &= 0x001F;

  WORD col = r | g | b;
  *pdst = col;

  return (2);
}

DWORD CDirectDraw::CopyOne_A1R5G5B5(MYPALLET src, PBYTE dst) {
  WORD *pdst;
  pdst = (PWORD)dst;

  WORD a, r, g, b;

  if (src.alpha != 0)
    a = 0x8000;
  else
    a = 0x0000;

  r = src.red;
  r *= 2 * 2 * 2 * 2 * 2 * 2 * 2;
  r &= 0x7C00;

  g = src.green;
  g *= 2 * 2;
  g &= 0x03E0;

  b = src.blue;
  b /= 2 * 2 * 2;
  b &= 0x001F;

  WORD col = a | r | g | b;
  *pdst = col;

  return (2);
}

DWORD CDirectDraw::CopyOne_A4R4G4B4(MYPALLET src, PBYTE dst) {
  WORD *pdst;
  pdst = (PWORD)dst;

  WORD a, r, g, b;

  a = src.alpha;
  a *= 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
  a &= 0xF000;

  r = src.red;
  r *= 2 * 2 * 2 * 2;
  r &= 0x0F00;

  g = src.green;
  g &= 0x00F0;

  b = src.blue;
  b /= 2 * 2 * 2 * 2;
  b &= 0x000F;

  WORD col = a | r | g | b;
  *pdst = col;

  return (2);
}

DWORD CDirectDraw::CopyOne_A8R3G3B2(MYPALLET src, PBYTE dst) {
  WORD *pdst;
  pdst = (PWORD)dst;

  WORD a, r, g, b;

  a = src.alpha;
  a *= 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;

  r = src.red;
  r &= 0x00080;

  g = src.green;
  g /= 2 * 2 * 2;
  g &= 0x001C;

  b = src.blue;
  b /= 2 * 2 * 2 * 2 * 2 * 2;
  b &= 0x0003;

  WORD col = a | r | g | b;
  *pdst = col;

  return (2);
}

DWORD CDirectDraw::CopyOne_A8R8G8B8(MYPALLET src, PBYTE dst) {
  MYPALLET *pdst;
  pdst = (MYPALLET *)dst;

  *pdst = src;

  return (4);
}

//分割方法決定*********************************************************************************
BOOL CDirectDraw::AssignTextureDiv(MYSURFACE *pmsf, DWORD width, DWORD height) {
  DWORD i;

  DWORD ntx, nty, numsufx = 0, numsufy = 0;
  BOOL bloop;

  //ヨコ方向
  bloop = TRUE;
  DWORD bmpwidthr = width;
  DWORD asstexsize = maxtexturewidth;

  while (bloop) {
    ntx = bmpwidthr / asstexsize;
    bmpwidthr = bmpwidthr % asstexsize;
    for (i = 0; i < ntx; i++) {
      pmsf->xsufsize[numsufx] = asstexsize;
      numsufx++;
      if (numsufx == MYSUF_MAXNUMTEX)
        return (FALSE);
    }
    if (bmpwidthr == 0)
      bloop = FALSE;                          //きっちり終わったらそれで終了
    else if (asstexsize <= MINIMUM_TEXSIZE) { //最後は細かいから省略だ
      pmsf->xsufsize[numsufx] = asstexsize;
      numsufx++;
      bloop = FALSE;
    }
    asstexsize /= 2;
  }
  pmsf->xsufsize[numsufx] = 0;
  pmsf->xsufnum = numsufx;
  pmsf->xsufindx[0] = 0;
  for (i = 1; i < pmsf->xsufnum; i++) {
    pmsf->xsufindx[i] = pmsf->xsufindx[i - 1] + pmsf->xsufsize[i - 1];
  }

  //タテ方向
  bloop = TRUE;
  DWORD bmpheightr = height;
  asstexsize = maxtextureheight;

  while (bloop) {
    nty = bmpheightr / asstexsize;
    bmpheightr = bmpheightr % asstexsize;
    for (i = 0; i < nty; i++) {
      pmsf->ysufsize[numsufy] = asstexsize;
      numsufy++;
      if (numsufy == MYSUF_MAXNUMTEX)
        return (FALSE);
    }
    if (bmpheightr == 0)
      bloop = FALSE;                          //きっちり終わったらそれで終了
    else if (asstexsize <= MINIMUM_TEXSIZE) { //最後は細かいから省略だ
      pmsf->ysufsize[numsufy] = MINIMUM_TEXSIZE;
      numsufy++;
      bloop = FALSE;
    }
    asstexsize /= 2;
  }
  pmsf->ysufsize[numsufy] = 0;
  pmsf->ysufnum = numsufy;
  pmsf->ysufindx[0] = 0;
  for (i = 1; i < pmsf->ysufnum; i++) {
    pmsf->ysufindx[i] = pmsf->ysufindx[i - 1] + pmsf->ysufsize[i - 1];
  }

  // test
  /*
#ifdef _DEBUG
  DWORD totalx=0,totaly=0;
  char msg[256];
  sprintf(msg,"▼AssignTextureDiv( %d x %d - %d x %d )\n",width,height,pmsf->xsufnum,pmsf->ysufnum);
  OutputDebugString(msg);
  OutputDebugString("x : ");
  for(i=0;i<MYSUF_MAXNUMTEX;i++){
          totalx+=pmsf->xsufsize[i];
          sprintf(msg," %d-%d(%d),",pmsf->xsufindx[i],pmsf->xsufsize[i],totalx);
          OutputDebugString(msg);
          if(pmsf->xsufsize[i]==0)i=MYSUF_MAXNUMTEX;
  }
  OutputDebugString("\n");
  OutputDebugString("y : ");
  for(i=0;i<MYSUF_MAXNUMTEX;i++){
          totaly+=pmsf->ysufsize[i];
          sprintf(msg," %d-%d(%d),",pmsf->xsufindx[i],pmsf->ysufsize[i],totaly);
          OutputDebugString(msg);
          if(pmsf->ysufsize[i]==0)i=MYSUF_MAXNUMTEX;
  }
  OutputDebugString("\n");
#endif
*/
  return (TRUE);
}

// 256ビットマップをファイルから読み取り*****************************************************************************
BOOL CDirectDraw::Load256Bitmapbits(
    MYPALLET **pbits, DWORD *width, DWORD *height, char *bmpfilename, char *palfilename) {
  BOOL ret2;

  //とりあえずパレットを確保
  MYPALLET pal[256];
  if (palfilename == NULL)
    palfilename = bmpfilename;
  ret2 = GetPallet(palfilename, pal);
  if (!ret2) {
    OutputDebugString("※CDirectDraw::Load256Bitmapbits パレット取得に失敗\n");
    return (FALSE);
  }

  //ビットマップのビットを取得~
  //ビットマップをロード
  HANDLE hFile;
  DWORD ret, br, err = FALSE;

  if (bmpfilename == NULL) {
    OutputDebugString("CDirectDraw::Load256Bitmapbits : ファイル名がNULLとはどういうことだ？\n");
  }

  //ファイルのオープン
  hFile = CreateFile(bmpfilename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE) {
    OutputDebugString("CDirectDraw::Load256Bitmapbits : ");
    OutputDebugString(bmpfilename);
    OutputDebugString("が開けませんでした。\n");
    return (NULL);
  }
  SetFilePointer(hFile, 0, NULL, FILE_BEGIN); //念のためファイルの先頭に移動

  //ﾌｧｲﾙヘッダの読み出し
  BITMAPFILEHEADER fileheader;
  ret = ReadFile(hFile, &fileheader, sizeof(BITMAPFILEHEADER), &br, NULL);
  if (!ret || br != sizeof(BITMAPFILEHEADER)) {
    OutputDebugString("CDirectDraw::Load256Bitmapbits : ファイルの読み込みに失敗(1)\n");
    err = TRUE;
  }
  if (fileheader.bfType != 0x4d42) { //"BM"
    OutputDebugString("CDirectDraw::Load256Bitmapbits : つーかこのファイルはビットマップではない\n");
    err = TRUE;
  }
  if (err) {
    CloseHandle(hFile);
    return (NULL);
  }

  // BITMAPINFOHEADERの読み込み
  BITMAPINFOHEADER infohed;
  ret = ReadFile(hFile, &infohed, sizeof(BITMAPINFOHEADER), &br, NULL);
  if (!ret || br != sizeof(BITMAPINFOHEADER)) {
    OutputDebugString("CDirectDraw::Load256Bitmapbits : ファイルの読み込みに失敗(2)\n");
    err = TRUE;
  }
  if (infohed.biSize != sizeof(BITMAPINFOHEADER)) {
    OutputDebugString("CDirectDraw::Load256Bitmapbits : BITMAPINFOHEADERのサイズが合わない\n");
    err = TRUE;
  }
  if (infohed.biBitCount != 8) {
    OutputDebugString("CDirectDraw::Load256Bitmapbits : つーかこのファイルは256ではない\n");
    err = TRUE;
  }
  if (infohed.biCompression != BI_RGB) {
    OutputDebugString("CDirectDraw::Load256Bitmapbits : 圧縮がかかっているらしい\n");
    err = TRUE;
  }
  if (err) {
    CloseHandle(hFile);
    return (NULL);
  }

  //パレットは読みとばし
  SetFilePointer(hFile, sizeof(RGBQUAD) * infohed.biClrUsed, NULL, FILE_CURRENT);

  if (infohed.biClrUsed == 0) {
    infohed.biClrUsed = 256;
  }

  //ビットマップビットのサイズを計算する(ビットマップの幅は4の倍数で格納されているらしい)
  DWORD linesize = infohed.biWidth;
  if (infohed.biWidth % 4 != 0)
    linesize += (4 - infohed.biWidth % 4);
  DWORD sizeimage = linesize * infohed.biHeight;
  //メモリを確保してビットマップビットを読み込む
  LPBYTE bits = (LPBYTE)(malloc(sizeimage));
  ret = ReadFile(hFile, bits, sizeimage, &br, NULL);
  CloseHandle(hFile);
  if (!ret || br != sizeimage) {
    OutputDebugString("CDirectDraw::Load256Bitmapbits : ファイルの読み込みに失敗(3)\n");
    free(bits);
    return (NULL);
  }

  //ビットマップの大きさにあわせて新たにメモリ領域を作成する
  LPVOID pnewbits2;
  DWORD ishalf = 1;
  if (config2.flag & CONFIG2_HALFMODE)
    ishalf = 2;
  pnewbits2 = malloc(sizeof(MYPALLET) * infohed.biWidth * infohed.biHeight / ishalf);
  MYPALLET *retbit;
  retbit = (MYPALLET *)pnewbits2;

  //そこにデータをコピー
  LONG i, j;
  PBYTE plinenow;
  DWORD halfcopy = 0;
  for (i = infohed.biHeight - 1; i >= 0; i--) {
    plinenow = bits;
    plinenow += linesize * i;
    for (j = 0; j < infohed.biWidth; j++) {
      if (config2.flag & CONFIG2_HALFMODE) { //半分しかコピーしない
        if (i % 2 == 0 && j % 2 == 0) {
          retbit[halfcopy] = pal[plinenow[j]];
          halfcopy++;
        }
      } else {
        retbit[(infohed.biHeight - 1 - i) * infohed.biWidth + j] = pal[plinenow[j]];
      }
    }
  }

  //完了
  free(bits);

  if (config2.flag & CONFIG2_HALFMODE) {
    infohed.biWidth /= 2;
    infohed.biHeight /= 2;
  }

  *pbits = retbit;
  *width = infohed.biWidth;
  *height = infohed.biHeight;
  return (TRUE);
}

// 256ビットマップのパレットのみを読み込み***************************************************************
BOOL CDirectDraw::GetPallet(char *filename, MYPALLET *pal) // 256ｂｍｐファイルから、パレットだけを取り出す
{
  HANDLE hFile;
  DWORD ret, br, err = FALSE;

  if (filename == NULL) {
    OutputDebugString("CDirectDraw::GetPallet : ファイル名がNULLとはどういうことだ？\n");
    return (FALSE);
  }
  if (pal == NULL) {
    OutputDebugString("CDirectDraw::GetPallet : パレットがNULLとはどういうことだ？\n");
    return (FALSE);
  }

  //ファイルのオープン
  hFile = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE) {
    OutputDebugString("CDirectDraw::GetPallet : ");
    OutputDebugString(filename);
    OutputDebugString("が開けませんでした。\n");
    return (NULL);
  }
  SetFilePointer(hFile, 0, NULL, FILE_BEGIN); //念のためファイルの先頭に移動

  //ﾌｧｲﾙヘッダの読み出し
  BITMAPFILEHEADER fileheader;
  ret = ReadFile(hFile, &fileheader, sizeof(BITMAPFILEHEADER), &br, NULL);
  if (!ret || br != sizeof(BITMAPFILEHEADER)) {
    OutputDebugString("CDirectDraw::GetPallet : ファイルの読み込みに失敗(1)\n");
    err = TRUE;
  }
  if (fileheader.bfType != 0x4d42) { //"BM"
    OutputDebugString("CDirectDraw::GetPallet : つーかこのファイルはビットマップではない\n");
    err = TRUE;
  }
  if (err) {
    CloseHandle(hFile);
    return (FALSE);
  }

  // BITMAPINFOHEADERの読み込み
  BITMAPINFOHEADER infohed;
  ret = ReadFile(hFile, &infohed, sizeof(BITMAPINFOHEADER), &br, NULL);
  if (!ret || br != sizeof(BITMAPINFOHEADER)) {
    OutputDebugString("CDirectDraw::GetPallet : ファイルの読み込みに失敗(2)\n");
    err = TRUE;
  }
  if (infohed.biSize != sizeof(BITMAPINFOHEADER)) {
    OutputDebugString("CDirectDraw::GetPallet : BITMAPINFOHEADERのサイズが合わない\n");
    err = TRUE;
  }
  if (infohed.biBitCount != 8) {
    OutputDebugString("CDirectDraw::GetPallet : つーかこのファイルは256ではない\n");
    err = TRUE;
  }
  if (infohed.biCompression != BI_RGB) {
    OutputDebugString("CDirectDraw::GetPallet : 圧縮がかかっているらしい\n");
    err = TRUE;
  }
  if (infohed.biClrUsed == 0) {
    infohed.biClrUsed = 256;
  }
  if (err) {
    CloseHandle(hFile);
    return (FALSE);
  }

  //いよいよパレットの読み込み
  RGBQUAD dpal[256];
  ZeroMemory(dpal, sizeof(RGBQUAD) * 256);
  ret = ReadFile(hFile, dpal, sizeof(RGBQUAD) * infohed.biClrUsed, &br, NULL);
  CloseHandle(hFile);
  if (!ret || br != sizeof(RGBQUAD) * infohed.biClrUsed) {
    OutputDebugString("CDirectDraw::GetPallet : ファイルの読み込みに失敗(3)\n");
    return (FALSE);
  }
  // 255番目の色は強制的に黒（透過色）
  dpal[255].rgbBlue = 0;
  dpal[255].rgbGreen = 0;
  dpal[255].rgbRed = 0;

  for (int i = 0; i < 256; i++) {
    if (dpal[i].rgbRed == 0 && dpal[i].rgbGreen == 0 && dpal[i].rgbBlue == 0)
      pal[i].alpha = 0; //透明
    else
      pal[i].alpha = 0xFF; //不透明
    pal[i].red = dpal[i].rgbRed;
    pal[i].green = dpal[i].rgbGreen;
    pal[i].blue = dpal[i].rgbBlue;

    /*		pal[i].alpha = 0;
                    pal[i].red = 0;
                    pal[i].green = 0xFF;
                    pal[i].blue = 0;*/
  }

  return (TRUE);
}

//開放********************************************************************

void CDirectDraw::RelSurface(MYSURFACE *s) {
  if (s == NULL)
    return;

  UINT j, k;

  if (s->valid) {
    s->valid = FALSE;
    for (j = 0; j < s->ysufnum; j++) {
      for (k = 0; k < s->xsufnum; k++) {
        RELEASE(s->pTex[k * (s->ysufnum) + j]);
      }
    }
  }
}
