
/*===============================================================

        スタッフロール　タスク

=================================================================*/

#include "global.h"
#include "task_staffroll.h"

/*----------------------------------------------------------------------
        構築
------------------------------------------------------------------------*/
CTStaffRoll::CTStaffRoll() {
  credits = NULL;
  txtpath = NULL;
  ms = NULL;
}

/*----------------------------------------------------------------------
        終了時処理
------------------------------------------------------------------------*/

void CTStaffRoll::Terminate() {
  g_sound.BGMStop();
  CleanUp();
}

void CTStaffRoll::CleanUp() {
  DELETEARRAY(credits);
  DELETEARRAY(txtpath);
  RELSURFACE(ms);
}

/*----------------------------------------------------------------------
        初期設定
------------------------------------------------------------------------*/
void CTStaffRoll::Initialize() {
  fsize = 0;
  counter = 0;
  staffindex = 0;
  countersr = 0;

  HANDLE hFile = CreateFile(txtpath ? txtpath : ".\\system\\staff.txt", //"story\\std\\test\\staff.txt",//
                            GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE) {
    fsize = 256;
    credits = new char[fsize];
    sprintf(credits, "\n\n\n\n\n\n\n\nファイルのオープンに失敗しました");
  } else {
    fsize = GetFileSize(hFile, NULL);
    credits = new char[fsize + 2];
    ZeroMemory(credits, fsize + 2);

    DWORD br;
    ReadFile(hFile, credits, fsize, &br, NULL);
    CloseHandle(hFile);
  }

  ms = g_draw.CreateSurfaceFrom256Image(".\\system\\ending");
  g_sound.BGMPlay(".\\system\\bgm\\ending", FALSE);
}

/*----------------------------------------------------------------------
        更新
------------------------------------------------------------------------*/
#define GYOHEIGHT 22

BOOL CTStaffRoll::Execute(DWORD time) {
  BOOL ext = FALSE;

  r.top = GYOHEIGHT - countersr % GYOHEIGHT;
  r.left = 0;
  r.right = 640;
  r.bottom = 480;

  int zurast = 480 - countersr;
  if (zurast > 0)
    r.top = zurast;
  else {
    if (countersr % GYOHEIGHT == 0 && counter % 2 == 0) {
      staffindex += Get1Gyo(&credits[staffindex]);
    }
  }

  if (strlen(&credits[staffindex]) == 0)
    ext = TRUE;

  DWORD key = (g_input.GetKey(m_keyindex, 0));
  if (((key & KEYSTA_BA2 || key & KEYSTA_BB2 || key & KEYSTA_BC2) && counter > 20)) {
    ext = TRUE;
  }

  counter++;
  if (counter % 2 == 0)
    countersr++;

  return ext ? FALSE : TRUE;
}

/*----------------------------------------------------------------------
        描画
------------------------------------------------------------------------*/
void CTStaffRoll::Draw() {
  g_draw.SetTransform(FALSE);

  //背景を描画
  RECT r_all;
  r_all.top = r_all.left = 0;
  r_all.right = 640;
  r_all.bottom = 480;
  g_draw.CheckBlt(ms, 0, 0, r_all);

  //文字描画
  g_draw.DrawBlueText(r, &credits[staffindex], -1, DT_CENTER, 2);
}

int CTStaffRoll::Get1Gyo(char *str) {
  DWORD i;

  for (i = 0; i < strlen(str) - 1; i++) {
    if (str[i] == 13 && str[i + 1] == 10) {
      i += 2;
      return (i);
    }
  }
  return (i);
}

/*----------------------------------------------------------------------
        スクリプトの設定を取り込み
------------------------------------------------------------------------*/
void CTStaffRoll::Setup(char *basedir, CStoryElement_Staff *ele_staff) {
  if (credits)
    return; //すでに再生中なので無視
  if (!basedir)
    return; //設定できないので無視
  CleanUp();

  UINT baselen = strlen(basedir + 1);

  //テキストファイル名取り込み
  if (ele_staff->m_filename) {
    txtpath = new char[baselen + strlen(ele_staff->m_filename) + 3];
    sprintf(txtpath, "%s\\%s", basedir, ele_staff->m_filename);
  }
}
