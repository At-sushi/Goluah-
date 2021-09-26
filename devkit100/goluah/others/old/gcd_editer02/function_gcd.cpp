#include <windows.h>
#include <stdio.h>
#include "define.h"

#ifdef ___GCDEDITER
extern HWND ghwnd;
extern char datfilepath[256];
#endif

#include "define_gcd.h"
#include "function_gcd.h"

//**********************************************************************************
//  load
//**********************************************************************************

#ifdef ___GCDEDITER

BOOL GCDLoadDlg(GCD_CELL2 *pcdat, GCD_RECT *prdat, GCD_HANTEI *phdat, GCD_CELLNAMES *pfhnames) {
  char filepath[256] = "";
  char filename[64];
  OPENFILENAME ofn;
  ZeroMemory(&ofn, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = ghwnd;
  ofn.lpstrFilter = "データ(*.gcd)\0*.gcd\0すべて(*.*)\0*.*\0\0";
  ofn.lpstrFile = filepath;
  ofn.nMaxFile = 256;
  ofn.lpstrFileTitle = filename;
  ofn.nMaxFileTitle = 64;
  ofn.Flags = OFN_FILEMUSTEXIST;
  ofn.lpstrTitle = "開く";

  if (!GetOpenFileName(&ofn))
    return (FALSE);

  int i, j;

  switch (GCDLoad(filepath, pcdat, prdat, phdat, pfhnames)) {
  case 0:                                    //成功
    for (i = strlen(filepath); i > 0; i--) { //グローバル datfilepath に読み込んだgcdファイルのパスをコピー
      if (filepath[i] == '\\') {
        j = i;
        i = 0;
      }
    }
    for (; j >= 0; j--) {
      datfilepath[j] = filepath[j];
    }
    return (TRUE);
  default:
    MessageBox(ghwnd, "読込に失敗しました", "", MB_OK);
  }

  return (FALSE);
}

#endif

GCD_FILEHEADER fhed;
GCD_CELLNAMES fh_names;
GCD_RECT dm_rdat[GCDMAX_RECTANGLES];
GCD_CELL2 dm_cdat[GCDMAX_CELLS];
GCD_HANTEI dm_hdat[GCDMAX_CELLS];

int GCDLoad(char *filename, GCD_CELL2 *pcdat, GCD_RECT *prdat, GCD_HANTEI *phdat, GCD_CELLNAMES *pfhnames) {
  ZeroMemory(&fhed, sizeof(&fhed));
  ZeroMemory(&fh_names, sizeof(&fh_names));
  ZeroMemory(dm_rdat, sizeof(dm_rdat));
  ZeroMemory(dm_cdat, sizeof(dm_cdat));
  ZeroMemory(dm_hdat, sizeof(dm_hdat));

  HANDLE hFile = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE) {
    return (1);
  }
  SetFilePointer(hFile, 0, NULL, FILE_BEGIN); //ファイル先頭に移動
  //ヘッダ部の読み出し
  DWORD br;
  DWORD ret = ReadFile(hFile, &fhed, sizeof(GCD_FILEHEADER), &br, NULL);
  if (!ret || br != sizeof(GCD_FILEHEADER)) {
    CloseHandle(hFile);
    return (2);
  }
  //ファイルの検証
  if (fhed.ver != GCD_VERSION) {
    CloseHandle(hFile);
    return (3);
  }

  //セル名を読み出し
  ret = ReadFile(hFile, &fh_names, sizeof(GCD_CELLNAMES), &br, NULL);
  if (!ret || br != sizeof(GCD_CELLNAMES)) {
    CloseHandle(hFile);
    return (4);
  }

  //矩形
  ret = ReadFile(hFile, dm_rdat, sizeof(GCD_RECT) * GCDMAX_RECTANGLES, &br, NULL);
  if (!ret || br != sizeof(GCD_RECT) * GCDMAX_RECTANGLES) {
    CloseHandle(hFile);
    return (5);
  }
  //ＣＥＬＬ
  ret = ReadFile(hFile, dm_cdat, sizeof(GCD_CELL2) * GCDMAX_CELLS, &br, NULL);
  if (!ret || br != sizeof(GCD_CELL2) * GCDMAX_CELLS) {
    CloseHandle(hFile);
    return (6);
  }
  //あたり判定
  ret = ReadFile(hFile, dm_hdat, sizeof(GCD_HANTEI) * GCDMAX_CELLS, &br, NULL);
  if (!ret || br != sizeof(GCD_HANTEI) * GCDMAX_CELLS) {
    CloseHandle(hFile);
    return (7);
  }

  CloseHandle(hFile);

  //コピー

  if (pfhnames != NULL) {
    memcpy(pfhnames, &fh_names, sizeof(GCD_CELLNAMES));
  }
  memcpy(prdat, dm_rdat, sizeof(GCD_RECT) * GCDMAX_RECTANGLES);
  memcpy(pcdat, dm_cdat, sizeof(GCD_CELL2) * GCDMAX_CELLS);
  memcpy(phdat, dm_hdat, sizeof(GCD_HANTEI) * GCDMAX_CELLS);

  return (0);
}

//**********************************************************************************
//  save
//**********************************************************************************

#ifdef ___GCDEDITER

BOOL GCDSaveDlg(GCD_CELL2 *pcdat, GCD_RECT *prdat, GCD_HANTEI *phdat, GCD_CELLNAMES *pfhnames) {
  char filepath[256] = "";
  char filename[64];
  OPENFILENAME ofn;
  ZeroMemory(&ofn, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = ghwnd;
  ofn.lpstrFilter = "データ(*.gcd)\0*.gcd\0すべて(*.*)\0*.*\0\0";
  ofn.lpstrFile = filepath;
  ofn.nMaxFile = 256;
  ofn.lpstrFileTitle = filename;
  ofn.nMaxFileTitle = 64;
  ofn.Flags = 0;
  ofn.lpstrTitle = "保存";

  if (!GetOpenFileName(&ofn))
    return (FALSE);

  //拡張子を無理やり.gcdに
  int len = strlen(filepath);
  if (len > 4) {
    if (!(filepath[len - 4] == '.' && (filepath[len - 3] == 'g' || filepath[len - 3] == 'G') &&
          (filepath[len - 2] == 'c' || filepath[len - 2] == 'C') &&
          (filepath[len - 1] == 'd' || filepath[len - 1] == 'D'))) {
      filepath[len] = '.';
      filepath[len + 1] = 'g';
      filepath[len + 2] = 'c';
      filepath[len + 3] = 'd';
    }
  }

  switch (GCDSave(filepath, pcdat, prdat, phdat, pfhnames)) {
  case 0:
    return (TRUE);
  case 1:
    MessageBox(ghwnd, "ファイルオープンに失敗", "保存に失敗", MB_OK);
    break;
  case 2:
    MessageBox(ghwnd, "書き込み失敗(2)", "保存に失敗", MB_OK);
    break;
  case 3:
    MessageBox(ghwnd, "書き込み失敗(3)", "保存に失敗", MB_OK);
    break;
  case 4:
    MessageBox(ghwnd, "書き込み失敗(4)", "保存に失敗", MB_OK);
    break;
  case 5:
    MessageBox(ghwnd, "書き込み失敗(5)", "保存に失敗", MB_OK);
    break;
  case 6:
    MessageBox(ghwnd, "書き込み失敗(6)", "保存に失敗", MB_OK);
    break;
  }
  return (FALSE);
}

int GCDSave(char *filename, GCD_CELL2 *pcdat, GCD_RECT *prdat, GCD_HANTEI *phdat, GCD_CELLNAMES *pfhnames) {
  GCD_FILEHEADER fhed;
  GCD_CELLNAMES fh_names;

  HANDLE hFile = CreateFile(filename, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE) {
    return (1);
  }
  SetFilePointer(hFile, 0, NULL, FILE_BEGIN); //ファイル先頭に移動

  //ファイルヘッダの準備
  fhed.size = sizeof(GCD_FILEHEADER);
  fhed.ver = GCD_VERSION;
  //ヘッダ部の書き込み
  DWORD br;
  DWORD ret = WriteFile(hFile, &fhed, sizeof(GCD_FILEHEADER), &br, NULL);
  if (!ret || br != sizeof(GCD_FILEHEADER)) {
    CloseHandle(hFile);
    return (2);
  }

  //セル名の準備
  if (pfhnames != NULL)
    memcpy(&fh_names, pfhnames, sizeof(GCD_CELLNAMES));
  else
    ZeroMemory(&pfhnames, sizeof(GCD_CELLNAMES));
  //セル名の書き込み
  ret = WriteFile(hFile, &fh_names, sizeof(GCD_CELLNAMES), &br, NULL);
  if (!ret || br != sizeof(GCD_CELLNAMES)) {
    CloseHandle(hFile);
    return (3);
  }

  //矩形
  ret = WriteFile(hFile, prdat, sizeof(GCD_RECT) * GCDMAX_RECTANGLES, &br, NULL);
  if (!ret || br != sizeof(GCD_RECT) * GCDMAX_RECTANGLES) {
    CloseHandle(hFile);
    return (4);
  }
  //ＣＥＬＬ
  ret = WriteFile(hFile, pcdat, sizeof(GCD_CELL2) * GCDMAX_CELLS, &br, NULL);
  if (!ret || br != sizeof(GCD_CELL2) * GCDMAX_CELLS) {
    CloseHandle(hFile);
    return (5);
  }
  //あたり判定
  ret = WriteFile(hFile, phdat, sizeof(GCD_HANTEI) * GCDMAX_CELLS, &br, NULL);
  if (!ret || br != sizeof(GCD_HANTEI) * GCDMAX_CELLS) {
    CloseHandle(hFile);
    return (6);
  }

  CloseHandle(hFile);

  return (0);
}

BOOL GCDSaveHeader(GCD_CELLNAMES *pfhnames) {
  //保存するファイルを選択させる
  char filepath[256] = "";
  char filename[64];
  OPENFILENAME ofn;
  ZeroMemory(&ofn, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = ghwnd;
  ofn.lpstrFilter = "C/C++ヘッダファイル(*.h)\0*.h\0すべて(*.*)\0*.*\0\0";
  ofn.lpstrFile = filepath;
  ofn.nMaxFile = 256;
  ofn.lpstrFileTitle = filename;
  ofn.nMaxFileTitle = 64;
  ofn.Flags = 0;
  ofn.lpstrTitle = "保存";

  if (!GetOpenFileName(&ofn))
    return (FALSE);

  //拡張子を無理やり.hに
  int len = strlen(filepath);
  if (len > 4) {
    if (!(filepath[len - 4] == '.' && (filepath[len - 3] == 'h' || filepath[len - 3] == 'H'))) {
      filepath[len] = '.';
      filepath[len + 1] = 'h';
    }
  }

  //保存
  HANDLE hFile = CreateFile(filepath, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE) {
    MessageBox(ghwnd, "ﾌｧｲﾙが開けなかった", "SaveFile()", MB_OK);
    return (FALSE);
  }

  char write[256];
  DWORD br;
  DWORD ret;
  for (DWORD i = 0; i < GCDMAX_CELLS; i++) {
    if (strlen(pfhnames->name[i]) != 0) {
      sprintf(write, "#define CELL_%s %d\n", pfhnames->name[i], i);
      ret = WriteFile(hFile, write, sizeof(char) * strlen(write), &br, NULL);
    }
  }

  CloseHandle(hFile);
  return (TRUE);
}

//***************************************************************************
// other
//***************************************************************************

void GCDSetInitialData(GCD_CELL2 *pcdat, GCD_RECT *prdat, GCD_HANTEI *phdat, GCD_CELLNAMES *pfhnames) {
  //一旦全部クリア
  if (pfhnames != NULL)
    ZeroMemory(&pfhnames, sizeof(&pfhnames));
  ZeroMemory(prdat, sizeof(GCD_RECT) * GCDMAX_RECTANGLES);
  ZeroMemory(pcdat, sizeof(GCD_CELL2) * GCDMAX_CELLS);
  ZeroMemory(phdat, sizeof(GCD_HANTEI) * GCDMAX_CELLS);

  //拡大率セット
  int i, j;
  for (i = 0; i < GCDMAX_CELLS; i++) {
    for (j = 0; j < 8; j++) {
      pcdat[i].cell[j].magx = pcdat[i].cell[j].magy = 1.0;
    }
  }
}

BOOL GCDIsEmptyCell2(GCD_CELL2 gcdc, GCD_HANTEI gcdh) {
  for (int i = 0; i < 8; i++) {
    if (gcdc.cell[i].cdr != 0)
      return (FALSE);
    if (gcdc.cell[i].dx != 0)
      return (FALSE);
    if (gcdc.cell[i].dy != 0)
      return (FALSE);
    if (gcdc.cell[i].flag != 0)
      return (FALSE);
    if (gcdc.cell[i].magx != 1.0)
      return (FALSE);
    if (gcdc.cell[i].magy != 1.0)
      return (FALSE);
    if (gcdc.cell[i].rot != 0)
      return (FALSE);
  }
  if (gcdc.gcx != 0)
    return (FALSE);
  if (gcdc.gcy != 0)
    return (FALSE);
  if (gcdc.flag != 0)
    return (FALSE);

  for (i = 0; i < 3; i++) {
    if (gcdh.attack[i].bottom != 0)
      return (FALSE);
    if (gcdh.attack[i].top != 0)
      return (FALSE);
    if (gcdh.attack[i].left != 0)
      return (FALSE);
    if (gcdh.attack[i].right != 0)
      return (FALSE);
    if (gcdh.kas[i].bottom != 0)
      return (FALSE);
    if (gcdh.kas[i].top != 0)
      return (FALSE);
    if (gcdh.kas[i].left != 0)
      return (FALSE);
    if (gcdh.kas[i].right != 0)
      return (FALSE);
    if (gcdh.kurai[i].bottom != 0)
      return (FALSE);
    if (gcdh.kurai[i].top != 0)
      return (FALSE);
    if (gcdh.kurai[i].left != 0)
      return (FALSE);
    if (gcdh.kurai[i].right != 0)
      return (FALSE);
  }

  return (TRUE);
}

BOOL GCDIsEmptyRect(GCD_RECT gcdr) {
  if (gcdr.bmpno != 0)
    return (FALSE);
  if (gcdr.center_x != 0)
    return (FALSE);
  if (gcdr.center_y != 0)
    return (FALSE);
  if (gcdr.r.bottom != 0)
    return (FALSE);
  if (gcdr.r.top != 0)
    return (FALSE);
  if (gcdr.r.left != 0)
    return (FALSE);
  if (gcdr.r.right != 0)
    return (FALSE);
  return (TRUE);
}

void GCDSetEmptyCell2(GCD_CELL2 *pgcdc, GCD_HANTEI *pgcdh) {
  for (int i = 0; i < 8; i++) {
    pgcdc->cell[i].cdr = 0;
    pgcdc->cell[i].dx = 0;
    pgcdc->cell[i].dy = 0;
    pgcdc->cell[i].flag = 0;
    pgcdc->cell[i].magx = 1.0f;
    pgcdc->cell[i].magy = 1.0f;
    pgcdc->cell[i].rot = 0;
  }
  pgcdc->flag = 0;
  pgcdc->gcx = pgcdc->gcy = 0;

  ZeroMemory(pgcdh, sizeof(GCD_HANTEI));
}

void GCDSetEmptyRect(GCD_RECT *pgcdr) { ZeroMemory(pgcdr, sizeof(GCD_RECT)); }

#endif
