
/*==============================================================================

    グローバル関数
    グローバル変数　など

================================================================================*/

#include "stdafx.h"
#include "global.h"
#include "bigface_cache.h"

/*------------------------------------------------------------------------------
    グローバルオブジェクト(クラス)
--------------------------------------------------------------------------------*/

CDirectDraw g_draw;   //描画オブジェクト
CDirectInput g_input; //入力オブジェクト
CDirectSound g_sound; //音系オブジェクト
CDirectPlay g_play;   //通信オブジェクト

CSystem g_system; //システムオブジェクト

AkiGlobal gbl;                    //グローバル関数の入れ物
CLog2File g_log2file;             //ファイルへログ保存クラス
CExport g_exp;                    // DLLへの関数提供クラス
CStoryList g_storylist;           //ストーリー管理クラス
CStageList g_stagelist;           //ステージリスト管理クラス
CCharacterList g_charlist;        //キャラクターリスト管理クラス
CBattleLoadInfo g_battleinfo;     //試合情報クラス
CBattleResultInfo g_battleresult; //試合結果情報クラス

/*------------------------------------------------------------------------------
    グローバル変数
--------------------------------------------------------------------------------*/
char AppName[] = "Goluah!! (ﾟДﾟ)"; //アプリケーション名
BOOL g_programexit = FALSE;        //メインループ終了フラグ
RECT g_rcClient;                   // window modeのときに必要(?)
int g_DISPLAYWIDTH = 640;          //生成ウインドウサイズ（幅）
int g_DISPLAYHEIGHT = 480;         //生成ウインドウサイズ（高さ）
BOOL g_muki[MAXNUM_KEYI];          //キー入力の向き

/*==============================================================================

    グローバル関数　(gbl.～)

================================================================================*/

/*
    ウインドウをディスプレイ中心にもっていく。
    ソースはwebで調べてどっかからコピった記憶がある。数年前に。

    @param hWnd 操作対象のウインドウハンドル
    @return 意味ないみたい。常に0。
*/
int AkiGlobal::SetWinCenter(HWND hWnd) {
  HWND hDeskWnd;
  RECT deskrc, rc;
  int x, y;

  hDeskWnd = GetDesktopWindow();
  GetWindowRect(hDeskWnd, (LPRECT)&deskrc);
  GetWindowRect(hWnd, (LPRECT)&rc);
  x = (deskrc.right - (rc.right - rc.left)) / 2;
  y = (deskrc.bottom - (rc.bottom - rc.top)) / 2;
  SetWindowPos(hWnd, HWND_TOP, x, y, (rc.right - rc.left), (rc.bottom - rc.top), SWP_SHOWWINDOW);
  return 0;
}

/*
    指定ファイルがあるかどうか調べる

    @param path 本体実行ファイルを基準とした相対パス。たぶん。
    @return TRUE:あるよ , FALSE:ないよ
*/
BOOL AkiGlobal::FileExist(char *path) {
  HANDLE hFile = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  if (hFile == INVALID_HANDLE_VALUE) {
    return (FALSE);
  } else {
    CloseHandle(hFile);
    return (TRUE);
  }
}

/*------------------------------------------------------------------------------
    矩形の衝突検出

    Syototu2RECTs	: 関数本体
    Syototu2RECTsA	: 回転無しの場合の矩形の衝突検出
    Syototu2RECTsB	: 回転のある場合の矩形の衝突検出
    getmiddle		: 4つの値のうち中間の二つを見つける
    GetKas_LINEvsRECT: 線と矩形の衝突検出
    g2dt_rot , g2dt_tns , g2dt_mag , g2dt_rvx , g2dt_rvy : 2D座標変換

--------------------------------------------------------------------------------*/

//! 2D座標変換
inline void g2dt_rot(MY2DVECTOR *v, int rot) //回転
{
  double ox = v->x;
  double oy = v->y;
  v->x = ox * cos(D3DXToRadian(rot)) - oy * sin(D3DXToRadian(rot));
  v->y = oy * cos(D3DXToRadian(rot)) + ox * sin(D3DXToRadian(rot));
}
inline void g2dt_tns(MY2DVECTOR *v, double tx, double ty) //!< 平行移動
{
  v->x += tx;
  v->y += ty;
}
inline void g2dt_mag(MY2DVECTOR *v, double mx, double my) //!< 拡大
{
  v->x *= mx;
  v->y *= my;
}
inline void g2dt_rvx(MY2DVECTOR *v) { v->x *= (-1); }
inline void g2dt_rvy(MY2DVECTOR *v) { v->y *= (-1); }

/*!
    @brief 回転無しの場合の矩形の衝突検出
    @sa AkiGlobal::Syototu2RECTs
    @param r1 1P側判定矩形
    @param r2 2P側判定矩形
    @param point [out] 衝突ポイントを受け取るためのポインタ
    @param gx1 1P側重心(x)
    @param gy1 1P側重心(y)
    @param rx1 1P側X反転フラグ
    @param ry1 1P側Y反転フラグ
    @param tx1 1P側平行移動量X
    @param ty1 1P側平行移動量Y
    @param mx1 1P側X拡大率
    @param my1 1P側Y拡大率
    @param gx2 2P側重心(x)
    @param gy2 2P側重心(y)
    @param rx2 2P側X反転フラグ
    @param ry2 2P側Y反転フラグ
    @param tx2 2P側平行移動量X
    @param ty2 2P側平行移動量Y
    @param mx2 2P側X拡大率
    @param my2 2P側Y拡大率
    @param magmode1 1P側拡大モード
    @param magmode2 2P側拡大モード
*/
BOOL Syototu2RECTsA(RECT r1,
                    RECT r2,
                    MY2DVECTOR *point,
                    int gx1,
                    int gy1,
                    BOOL rx1,
                    BOOL ry1,
                    double tx1,
                    double ty1,
                    double mx1,
                    double my1,
                    int gx2,
                    int gy2,
                    BOOL rx2,
                    BOOL ry2,
                    double tx2,
                    double ty2,
                    double mx2,
                    double my2,
                    UINT magmode1,
                    UINT magmode2);

/*!
    @brief 回転無しの場合の矩形の衝突検出
    矩形面積が1P側<2P側になるようにすること

    @sa AkiGlobal::Syototu2RECTs
    @param r1 1P側判定矩形
    @param r2 2P側判定矩形
    @param point [out] 衝突ポイントを受け取るためのポインタ
    @param gx1 1P側重心(x)
    @param gy1 1P側重心(y)
    @param rx1 1P側X反転フラグ
    @param ry1 1P側Y反転フラグ
    @param rot1 1P側回転角(degree)
    @param tx1 1P側平行移動量X
    @param ty1 1P側平行移動量Y
    @param mx1 1P側X拡大率
    @param my1 1P側Y拡大率
    @param gx2 2P側重心(x)
    @param gy2 2P側重心(y)
    @param rx2 2P側X反転フラグ
    @param ry2 2P側Y反転フラグ
    @param rot2 2P側回転角(degree)
    @param tx2 2P側平行移動量X
    @param ty2 2P側平行移動量Y
    @param mx2 2P側X拡大率
    @param my2 2P側Y拡大率
    @param magmode1 1P側拡大モード
    @param magmode2 2P側拡大モード
*/
BOOL Syototu2RECTsB(RECT r1,
                    RECT r2,
                    MY2DVECTOR *point,
                    int gx1,
                    int gy1,
                    BOOL rx1,
                    BOOL ry1,
                    int rot1,
                    double tx1,
                    double ty1,
                    double mx1,
                    double my1,
                    int gx2,
                    int gy2,
                    BOOL rx2,
                    BOOL ry2,
                    int rot2,
                    double tx2,
                    double ty2,
                    double mx2,
                    double my2,
                    UINT magmode1,
                    UINT magmode2);
void getmiddle(int a, int b, int c, int d, long *m1, long *m2);
DWORD GetKas_LINEvsRECT(MY2DVECTOR *kaspoint,
                        MY2DVECTOR &line_start,
                        MY2DVECTOR &line_end,
                        MY2DVECTOR rect_pos[4]);

/*!
    @brief 矩形の衝突検出

    実際の処理は回転の有無で Syototu2RECTsA と Syototu2RECTsB に振っている。

    @sa AkiGlobal::Syototu2RECTs
    @param r1 1P側判定矩形
    @param r2 2P側判定矩形
    @param point [out] 衝突ポイントを受け取るためのポインタ
    @param gx1 1P側重心(x)
    @param gy1 1P側重心(y)
    @param rx1 1P側X反転フラグ
    @param ry1 1P側Y反転フラグ
    @param rot1 1P側回転角(degree)
    @param tx1 1P側平行移動量X
    @param ty1 1P側平行移動量Y
    @param mx1 1P側X拡大率
    @param my1 1P側Y拡大率
    @param gx2 2P側重心(x)
    @param gy2 2P側重心(y)
    @param rx2 2P側X反転フラグ
    @param ry2 2P側Y反転フラグ
    @param rot2 2P側回転角(degree)
    @param tx2 2P側平行移動量X
    @param ty2 2P側平行移動量Y
    @param mx2 2P側X拡大率
    @param my2 2P側Y拡大率
    @param magmode1 1P側拡大モード
    @param magmode2 2P側拡大モード
*/
BOOL AkiGlobal::Syototu2RECTs(RECT r1,
                              RECT r2,
                              MY2DVECTOR *point,
                              int gx1,
                              int gy1,
                              BOOL rx1,
                              BOOL ry1,
                              int rot1,
                              double tx1,
                              double ty1,
                              double mx1,
                              double my1,
                              int gx2,
                              int gy2,
                              BOOL rx2,
                              BOOL ry2,
                              int rot2,
                              double tx2,
                              double ty2,
                              double mx2,
                              double my2,
                              UINT magmode1,
                              UINT magmode2) {
  //面積が0なら門前払い
  if (r1.left == r1.right || r1.top == r1.bottom)
    return (FALSE);
  if (r2.left == r2.right || r2.top == r2.bottom)
    return (FALSE);
  if (mx1 == 0 || my1 == 0 || mx2 == 0 || my2 == 0)
    return (FALSE);

  try { //たまに0除算例外を起こすらしい

    double menseki[2];

    if (rot1 % 360 == 0 && rot2 % 360 == 0) {
      return (Syototu2RECTsA(r1, r2, point, gx2, gy2, rx1, ry1, tx1, ty1, mx1, my1, gx2, gy2, rx2, ry2, tx2,
                             ty2, mx2, my2, magmode1, magmode2));
    } else {
      //９０度になってしまうといろいろまずいので、少しごまかす
      if ((rot1 + rot2) % 90 == 0)
        rot1 += 1;
      //面積比較
      menseki[0] = (r1.right - r1.left) * (r1.bottom - r1.top);
      menseki[0] = menseki[0] * menseki[0];
      menseki[1] = (r2.right - r2.left) * (r2.bottom - r2.top);
      menseki[1] = menseki[1] * menseki[1];
      if (menseki[0] < menseki[1])
        return (Syototu2RECTsB(r1, r2, point, gx1, gy1, rx1, ry1, rot1, tx1, ty1, mx1, my1, gx2, gy2, rx2,
                               ry2, rot2, tx2, ty2, mx2, my2, magmode1, magmode2));
      else
        return (Syototu2RECTsB(r2, r1, point, gx2, gy2, rx2, ry2, rot2, tx2, ty2, mx2, my2, gx1, gy1, rx1,
                               ry1, rot1, tx1, ty1, mx1, my1, magmode1, magmode2));
    }
  } catch (...) {
    return FALSE;
  }
}

//回転がない場合、2つの矩形の衝突を検出する
BOOL Syototu2RECTsA(RECT r1,
                    RECT r2,
                    MY2DVECTOR *point,
                    int gx1,
                    int gy1,
                    BOOL rx1,
                    BOOL ry1,
                    double tx1,
                    double ty1,
                    double mx1,
                    double my1,
                    int gx2,
                    int gy2,
                    BOOL rx2,
                    BOOL ry2,
                    double tx2,
                    double ty2,
                    double mx2,
                    double my2,
                    UINT magmode1,
                    UINT magmode2) {
  if (magmode1 == 1) {
    // x/y拡大
    r1.left = (long)(r1.left * mx1);
    r1.right = (long)(r1.right * mx1);
    r1.top = (long)(r1.top * my1);
    r1.bottom = (long)(r1.bottom * my1);
  }
  if (magmode2 == 1) {
    r2.left = (long)(r2.left * mx2);
    r2.right = (long)(r2.right * mx2);
    r2.top = (long)(r2.top * my2);
    r2.bottom = (long)(r2.bottom * my2);
  }

  //重心に持っていく
  r1.left -= gx1;
  r1.right -= gx1;
  r1.top -= gy1;
  r1.bottom -= gy1;
  r2.left -= gx2;
  r2.right -= gx2;
  r2.top -= gy2;
  r2.bottom -= gy2;

  // y反転
  if (ry1) {
    r1.top *= -1;
    r1.bottom *= -1;
  }
  if (ry2) {
    r2.top *= -1;
    r2.bottom *= -1;
  }

  if (magmode1 == 0) {
    // x/y拡大
    r1.left = (long)(r1.left * mx1);
    r1.right = (long)(r1.right * mx1);
    r1.top = (long)(r1.top * my1);
    r1.bottom = (long)(r1.bottom * my1);
  }
  if (magmode2 == 0) {
    r2.left = (long)(r2.left * mx2);
    r2.right = (long)(r2.right * mx2);
    r2.top = (long)(r2.top * my2);
    r2.bottom = (long)(r2.bottom * my2);
  }

  //重心から元に戻す
  r1.left += gx1;
  r1.right += gx1;
  r1.top += gy1;
  r1.bottom += gy1;
  r2.left += gx2;
  r2.right += gx2;
  r2.top += gy2;
  r2.bottom += gy2;

  // x反転
  if (rx1) {
    r1.left *= -1;
    r1.right *= -1;
  }
  if (rx2) {
    r2.left *= -1;
    r2.right *= -1;
  }

  //移動
  r1.left = (long)(r1.left + tx1);
  r1.right = (long)(r1.right + tx1);
  r1.top = (long)(r1.top + ty1);
  r1.bottom = (long)(r1.bottom + ty1);
  r2.left = (long)(r2.left + tx2);
  r2.right = (long)(r2.right + tx2);
  r2.top = (long)(r2.top + ty2);
  r2.bottom = (long)(r2.bottom + ty2);

  //値を正しくする
  int tmp;
  if (r1.left > r1.right) {
    tmp = r1.left;
    r1.left = r1.right;
    r1.right = tmp;
  }
  if (r2.left > r2.right) {
    tmp = r2.left;
    r2.left = r2.right;
    r2.right = tmp;
  }
  if (r1.top > r1.bottom) {
    tmp = r1.top;
    r1.top = r1.bottom;
    r1.bottom = tmp;
  }
  if (r2.top > r2.bottom) {
    tmp = r2.top;
    r2.top = r2.bottom;
    r2.bottom = tmp;
  }

  if (r1.right < r2.left)
    return (FALSE);
  if (r2.right < r1.left)
    return (FALSE);
  if (r1.bottom < r2.top)
    return (FALSE);
  if (r2.bottom < r1.top)
    return (FALSE);

  RECT r3;
  if (point != NULL) {
    getmiddle(r1.left, r1.right, r2.left, r2.right, &(r3.left), &(r3.right));
    getmiddle(r1.top, r2.top, r1.bottom, r2.bottom, &(r3.top), &(r3.bottom));

    point->x = (r3.left + r3.right) / 2;
    point->y = (r3.top + r3.bottom) / 2;
  }

  return (TRUE);
}

void getmiddle(int a, int b, int c, int d, long *m1, long *m2) //!< 4つの値のうち中間の二つを見つける
{
  if (a > b && a > c && a > d) { // aが最大
    if (b < c && b < d) {        // bが最小
      if (c < d) {
        *m1 = c;
        *m2 = d;
      } else {
        *m1 = d;
        *m2 = c;
      }
    } else if (c < d) { // cが最小
      if (b < d) {
        *m1 = b;
        *m2 = d;
      } else {
        *m1 = d;
        *m2 = b;
      }
    } else { // dが最小
      if (b < c) {
        *m1 = b;
        *m2 = c;
      } else {
        *m1 = c;
        *m2 = b;
      }
    }
  } else if (b > c && b > d) { // bが最大
    if (a < c && a < d) {      // aが最小
      if (c < d) {
        *m1 = c;
        *m2 = d;
      } else {
        *m1 = d;
        *m2 = c;
      }
    } else if (c < d) { // cが最小
      if (a < d) {
        *m1 = a;
        *m2 = d;
      } else {
        *m1 = d;
        *m2 = a;
      }
    } else { // dが最小
      if (a < c) {
        *m1 = a;
        *m2 = c;
      } else {
        *m1 = c;
        *m2 = a;
      }
    }
  } else if (c > d) {     // cが最大
    if (b < a && b < d) { // bが最小
      if (a < d) {
        *m1 = a;
        *m2 = d;
      } else {
        *m1 = d;
        *m2 = a;
      }
    } else if (a < d) { // aが最小
      if (b < d) {
        *m1 = b;
        *m2 = d;
      } else {
        *m1 = d;
        *m2 = b;
      }
    } else { // dが最小
      if (b < a) {
        *m1 = b;
        *m2 = a;
      } else {
        *m1 = a;
        *m2 = b;
      }
    }
  } else {                // dが最大
    if (b < c && b < a) { // bが最小
      if (c < d) {
        *m1 = c;
        *m2 = a;
      } else {
        *m1 = a;
        *m2 = c;
      }
    } else if (c < a) { // cが最小
      if (b < a) {
        *m1 = b;
        *m2 = a;
      } else {
        *m1 = a;
        *m2 = b;
      }
    } else { // aが最小
      if (b < c) {
        *m1 = b;
        *m2 = c;
      } else {
        *m1 = c;
        *m2 = b;
      }
    }
  }
}

//回転もちゃんと考慮して矩形の衝突を判定する / r1のほうがr2よりちいさい
BOOL Syototu2RECTsB(RECT r1,
                    RECT r2,
                    MY2DVECTOR *point,
                    int gx1,
                    int gy1,
                    BOOL rx1,
                    BOOL ry1,
                    int rot1,
                    double tx1,
                    double ty1,
                    double mx1,
                    double my1,
                    int gx2,
                    int gy2,
                    BOOL rx2,
                    BOOL ry2,
                    int rot2,
                    double tx2,
                    double ty2,
                    double mx2,
                    double my2,
                    UINT magmode1,
                    UINT magmode2) {
  if (!point)
    return FALSE;

  // top<bottom , left<rightを保証
  int tmp;
  if (r2.left > r2.right) {
    tmp = r2.left;
    r2.left = r2.right;
    r2.right = tmp;
  }
  if (r2.top > r2.bottom) {
    tmp = r2.top;
    r2.top = r2.bottom;
    r2.bottom = tmp;
  }

  MY2DVECTOR mv[2][4];
  //左上
  mv[0][0].x = r1.left;
  mv[0][0].y = r1.top;
  mv[1][0].x = r2.left;
  mv[1][0].y = r2.top;
  //右上
  mv[0][1].x = r1.right;
  mv[0][1].y = r1.top;
  mv[1][1].x = r2.right;
  mv[1][1].y = r2.top;
  //左下
  mv[0][2].x = r1.left;
  mv[0][2].y = r1.bottom;
  mv[1][2].x = r2.left;
  mv[1][2].y = r2.bottom;
  //右下
  mv[0][3].x = r1.right;
  mv[0][3].y = r1.bottom;
  mv[1][3].x = r2.right;
  mv[1][3].y = r2.bottom;

  // r1をr2の座標系に変換する
  int i;
  for (i = 0; i < 4; i++) {
    if (magmode1 == 1) {
      g2dt_mag(&mv[0][i], mx1, my1); //拡大（足元中心）
    }
    g2dt_tns(&mv[0][i], gx1, gx1); //重心座標に持っていく
    if (ry1)
      g2dt_rvy(&mv[0][i]); // y反転
    if (magmode1 == 0) {
      g2dt_mag(&mv[0][i], mx1, my1); //拡大（重心中心）
    }
    g2dt_rot(&mv[0][i], rot1);       //回転
    g2dt_tns(&mv[0][i], -gx1, -gx1); //重心戻し
    if (rx1)
      g2dt_rvx(&mv[0][i]); // x反転
    g2dt_tns(&mv[0][i], tx1, ty1);

    // here , the world

    g2dt_tns(&mv[0][i], -tx2, -ty2);
    if (rx2)
      g2dt_rvx(&mv[0][i]);         // x反転
    g2dt_tns(&mv[0][i], gx2, gx2); //重心戻し
    g2dt_rot(&mv[0][i], -rot2);    //回転
    if (magmode2 == 0) {
      g2dt_mag(&mv[0][i], 1.0 / mx2, 1.0 / my2); //拡大(重心中心)
    }
    if (ry2)
      g2dt_rvy(&mv[0][i]);           // y反転
    g2dt_tns(&mv[0][i], -gx2, -gx2); //重心座標に持っていく
    if (magmode2 == 1) {
      g2dt_mag(&mv[0][i], 1.0 / mx2, 1.0 / my2); //拡大
    }
  }

  int num_kas_line = 0;     //交わったラインの数
  MY2DVECTOR kas_points[8]; //交わった点のリスト。多分最大8点
  // 4辺同士の交わりを出すッ！
  num_kas_line += GetKas_LINEvsRECT(&kas_points[num_kas_line], mv[0][0], mv[0][1], mv[1]);
  num_kas_line += GetKas_LINEvsRECT(&kas_points[num_kas_line], mv[0][1], mv[0][2], mv[1]);
  num_kas_line += GetKas_LINEvsRECT(&kas_points[num_kas_line], mv[0][2], mv[0][3], mv[1]);
  num_kas_line += GetKas_LINEvsRECT(&kas_points[num_kas_line], mv[0][3], mv[0][0], mv[1]);

  if (num_kas_line > 0) { //交わってるゥゥゥゥウゥ
    point->x = 0;
    point->y = 0;
    for (tmp = 0; tmp < num_kas_line; tmp++) {
      point->x += kas_points[tmp].x;
      point->y += kas_points[tmp].y;
    }
    point->x /= num_kas_line;
    point->y /= num_kas_line;
    //衝突した点をワールド座標系に変換する
    // here , the world
    g2dt_tns(point, gx2, gx2); //重心座標に持っていく
    if (ry2)
      g2dt_rvy(point);           // y反転
    g2dt_mag(point, mx2, my2);   //拡大
    g2dt_rot(point, rot2);       //回転
    g2dt_tns(point, -gx2, -gx2); //重心戻し
    if (rx2)
      g2dt_rvx(point); // x反転
    g2dt_tns(point, tx2, ty2);

    return (TRUE);
  }

  //一方の矩形がもう一方の矩形の内部にあるか？
  if (mv[0][0].x > mv[1][2].x && mv[0][0].x < mv[1][1].x) {
    if (mv[0][0].y > mv[1][1].y && mv[0][0].y < mv[1][2].y) {
      if (mv[0][1].x > mv[1][2].x && mv[0][1].x < mv[1][1].x) {
        if (mv[0][1].y > mv[1][1].y && mv[0][1].y < mv[1][2].y) {
          if (mv[0][2].x > mv[1][2].x && mv[0][2].x < mv[1][1].x) {
            if (mv[0][2].y > mv[1][1].y && mv[0][2].y < mv[1][2].y) {
              if (mv[0][3].x > mv[1][2].x && mv[0][3].x < mv[1][1].x) {
                if (mv[0][3].y > mv[1][1].y && mv[0][3].y < mv[1][2].y) {
                  //内部にあるゥゥゥゥゥッゥゥゥゥ
                  point->x = 0;
                  point->y = 0;
                  for (tmp = 0; tmp < 4; tmp++) {
                    point->x += mv[0][tmp].x;
                    point->y += mv[0][tmp].y;
                  }
                  point->x /= 4;
                  point->y /= 4;
                  //衝突した点をワールド座標系に変換する
                  g2dt_tns(point, gx2, gx2); //重心座標に持っていく
                  if (ry2)
                    g2dt_rvy(point);           // y反転
                  g2dt_mag(point, mx2, my2);   //拡大
                  g2dt_rot(point, rot2);       //回転
                  g2dt_tns(point, -gx2, -gx2); //重心戻し
                  if (rx2)
                    g2dt_rvx(point); // x反転
                  g2dt_tns(point, tx2, ty2);

                  return (TRUE);
                }
              }
            }
          }
        }
      }
    }
  }

  return (FALSE); //衝突はありませんでした
}

//!直線と矩形の各辺との交わりを調べる
DWORD GetKas_LINEvsRECT(MY2DVECTOR *kaspoint,
                        MY2DVECTOR &line_start,
                        MY2DVECTOR &line_end,
                        MY2DVECTOR rect_pos[4]) {
  double a, b;                   //直線の方程式 ax+by+1=0
  double xmin, xmax, ymin, ymax; //直線と各辺が交わる可能性がある範囲
  double x[7], y[7];             //てんぽらり
  DWORD num_kas = 0;

  //まずは直線の方程式を求める
  x[3] = line_start.x;
  y[3] = line_start.y;
  x[4] = line_end.x;
  y[4] = line_end.y;
  if (x[3] == 0 || x[4] * y[3] - x[3] * y[4] == 0)
    return (0); // 0での除算はできないの
  b = (x[3] - x[4]) / (x[4] * y[3] - x[3] * y[4]);
  a = (-1.0 / x[3]) * (1.0 + b * y[3]);

  if (a == 0 || b == 0)
    return (0); //かなり面倒なので･･･

  //ヨコ線とのまじわり
  xmin = __min(x[3], x[4]);
  xmax = __max(x[3], x[4]);
  if (xmin < rect_pos[1].x) {
    if (xmax > rect_pos[0].x) {
      x[5] = __max(xmin, rect_pos[0].x);
      x[6] = __min(xmax, rect_pos[1].x);
      y[5] = -(a / b) * x[5] - 1.0 / b;
      y[6] = -(a / b) * x[6] - 1.0 / b;
      y[1] = rect_pos[1].y;
      if ((y[5] - y[1]) * (y[6] - y[1]) < 0) { //交わる
        //交わった点の計算
        y[0] = y[1];
        x[0] = -(b * y[0] + 1) / a;
        kaspoint[num_kas].x = x[0];
        kaspoint[num_kas].y = y[0];
        num_kas++;
      }
      y[1] = rect_pos[2].y;
      if ((y[5] - y[1]) * (y[6] - y[1]) < 0) { //交わる
        //交わった点の計算
        y[0] = y[1];
        x[0] = -(b * y[0] + 1) / a;
        kaspoint[num_kas].x = x[0];
        kaspoint[num_kas].y = y[0];
        num_kas++;
      }
    }
  }

  //タテ線との交わり
  ymin = __min(y[3], y[4]);
  ymax = __max(y[3], y[4]);
  if (ymin < rect_pos[2].y) {
    if (ymax > rect_pos[1].y) {
      y[5] = __max(ymin, rect_pos[1].y);
      y[6] = __min(ymax, rect_pos[2].y);
      x[5] = -(b * y[5] + 1.0) / a;
      x[6] = -(b * y[6] + 1.0) / a;
      x[1] = rect_pos[1].x;
      if ((x[5] - x[1]) * (x[6] - x[1]) < 0) { //交わる
        //交わった点の計算
        x[0] = x[1];
        y[0] = -(a * x[0] + 1.0) / b;
        kaspoint[num_kas].x = x[0];
        kaspoint[num_kas].y = y[0];
        num_kas++;
      }
      x[1] = rect_pos[2].x;
      if ((x[5] - x[1]) * (x[6] - x[1]) < 0) { //交わる
        //交わった点の計算
        x[0] = x[1];
        y[0] = -(a * x[0] + 1.0) / b;
        kaspoint[num_kas].x = x[0];
        kaspoint[num_kas].y = y[0];
        num_kas++;
      }
    }
  }

  return (num_kas);
}

// 2文字列を足して新しいバッファを返す
//戻り値はnew[]で確保したバッファ。str1はdelete[]されます。
char *AkiGlobal::MergeString(char *str1, const char *str2) {
  UINT len = 0;

  if (str1) {
    len += strlen(str1);
  }

  if (str2) {
    len += strlen(str2);
  } else
    return str1;

  char *ret = new char[len + 1];
  if (!ret)
    return NULL;

  if (str1) {
    sprintf(ret, "%s%s", str1, str2);
  } else
    sprintf(ret, "%s", str2);

  DELETEARRAY(str1);

  return ret;
}

/*
    @brief 指定ディレクトリ下からランダムなBGMを選択して再生
    @param dir 指定ディレクトリ。本体実行ファイル相対
*/
void AkiGlobal::PlayRandomBGM(char *dir) {
  if (!g_config.UseDShow())
    return;

  //ファイル名リスト
  std::vector<char *> filelist;
  std::vector<char *>::iterator ite;
  std::vector<char *>::iterator itee;

  int numstr;              //文字数
  char filepath[MAX_PATH]; //テンポラリパス

  //指定ディレクトリ下のファイル名をリストに収集（拡張子ぬき）
  HANDLE hFind;
  WIN32_FIND_DATA fd;
  sprintf(filepath, "%s\\*.*", dir);
  hFind = FindFirstFile(filepath, &fd);
  if (hFind == INVALID_HANDLE_VALUE)
    return; //失敗
  do {
    if (strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0)
      ;
    else if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
      numstr = strlen(fd.cFileName);
      char *str = new char[numstr + 1];
      str[numstr] = '\0';

      //ふぁいるめいこぴー
      while (numstr > 0) {
        numstr--;
        str[numstr] = fd.cFileName[numstr];
      }

      //拡張子削除
      numstr = strlen(fd.cFileName);
      while (numstr > 0) {
        numstr--;
        if (fd.cFileName[numstr] == '.') {
          str[numstr] = '\0';
          break;
        }
      }

      filelist.push_back(str);
    }
  } while (FindNextFile(hFind, &fd));
  FindClose(hFind);

  //ファイルがひとつもない
  if (filelist.size() == 0) {
    gbl.ods("AkiGlobal::PlayRandomBGM : no files\n");
    return;
  }

  //シャッフル
  ite = filelist.begin();
  itee = filelist.end();
  std::random_shuffle(ite, itee);

  //再生を試みる
  ite = filelist.begin();
  itee = filelist.end();
  for (; ite != itee; ite++) {
    sprintf(filepath, "%s\\%s", dir, *ite);
    if (g_sound.BGMPlay(filepath)) {
      break; //再生に成功したら終了
    }
    gbl.ods2("AkiGlobal::PlayRandomBGM : %s ...failed", filepath);
  }
  //	delete [] filepath;

  //ファイル名のリストを破棄
  ite = filelist.begin();
  itee = filelist.end();
  for (; ite != itee; ite++) {
    delete[](*ite);
  }
}

/*=========================================================================
    非static
===========================================================================*/

#define MAX_DUMMY_STRING 16

//確保
AkiGlobal::AkiGlobal() {
  m_dummybuf = CreateStringArray(MAX_DUMMY_STRING, 128);
  m_dummy_next = 0;

  g_system.AddTask(new CTBigFaceCache);
  m_blankIcon = NULL;

  m_ods_buffer = new char[1024 * 8];
}

//開放
AkiGlobal::~AkiGlobal() {
  DeleteStringArray(m_dummybuf);
  // RELSURFACE(m_blankIcon);//g_draw破棄時に自動的に消えるからいいや

  DELETEARRAY(m_ods_buffer);
}

/*----------------------------------------------------------------
    文字列ユーティリティ
------------------------------------------------------------------*/

//スクリプト読み込みsscanfで使うダミー用charバッファを取得。
char *AkiGlobal::GetDummyString() {
  m_dummy_next++;
  return m_dummybuf[m_dummy_next % MAX_DUMMY_STRING];
}

//テンポラリ文字列配列の生成・破棄
char **AkiGlobal::CreateTemporaryStrBuff(UINT num, UINT max_len) { return CreateStringArray(num, max_len); }

void AkiGlobal::DeleteTemporaryStrBuff(char **buf) { DeleteStringArray(buf); }

//指定バッファの先頭に、指定文字列が入っているかどうかチェック
BOOL AkiGlobal::strcheck(const char *buf, const char *str) {
  int len = strlen(str);
  while (len > 0) {
    if (*buf != *str)
      return FALSE;
    buf++;
    str++;
    len--;
  }
  return TRUE;
}

// 2次元char配列確保
char **AkiGlobal::CreateStringArray(UINT num, UINT max_len) {
  if (num == 0 || max_len == 0)
    return NULL;

  char **ret = new char *[num + 1];
  ret[num] = NULL;

  for (UINT i = 0; i < num; i++) {
    ret[i] = new char[max_len];
  }

  return ret;
}

// 2次元char配列破棄
void AkiGlobal::DeleteStringArray(char **pstrarr) {
  if (!pstrarr)
    return;

  char **tmp = pstrarr;
  while (*tmp != NULL) {
    delete[] * tmp;
    tmp++;
  }

  delete[] pstrarr;
}

/*----------------------------------------------------------------
    デカ顔イメージ取得
------------------------------------------------------------------*/
MYSURFACE *AkiGlobal::GetBigFace(int cindex, int col, int alt) {
  CBackgroundTaskBase *bgt = g_system.FindBGTask(BIGFACE_TASKID);
  CTBigFaceCache *bfc = dynamic_cast<CTBigFaceCache *>(bgt);

  if (!bfc)
    return NULL;
  return bfc->Get(cindex, col, alt);
}

/*----------------------------------------------------------------
    ブランク用アイコン取得
------------------------------------------------------------------*/
MYSURFACE *AkiGlobal::GetBlankIcon() {
  if (m_blankIcon == (MYSURFACE *)0xFFFFFFFF) { //すでに一度読み込みに失敗した
    return NULL;
  }

  if (!m_blankIcon) {
    m_blankIcon = g_draw.CreateSurfaceFrom256Image(".\\system\\blank_icon");
    if (m_blankIcon == NULL) {
      m_blankIcon = (MYSURFACE *)0xFFFFFFFF;
      return NULL;
    }
  }

  return m_blankIcon;
}

/*----------------------------------------------------------------
    実数 0～1 ランダム
------------------------------------------------------------------*/
float AkiGlobal::RandomOne() {
  UINT div1 = 9999;
  float div2 = 9999.0f;
  return ((float)(rand() % div1)) / div2;
}

/*----------------------------------------------------------------
    ファイル丸ごと読み込み
------------------------------------------------------------------*/

AkiFile::AkiFile() {
  m_buffer = NULL;
  m_size = 0;
}

void AkiFile::Destroy() {
  DELETEARRAY(m_buffer);
  m_size = 0;
}

BOOL AkiFile::Load(char *filename) {
  Destroy();
  if (!filename)
    return FALSE;

  //オープソ
  HANDLE hFile = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE) {
    return (FALSE);
  }

  //ファイルサイズ取得・メモリ確保
  DWORD size = GetFileSize(hFile, NULL);
  if (size == 0)
    return FALSE;
  m_buffer = new BYTE[size + 1];

  //読み込み
  DWORD br;
  DWORD ret = ReadFile(hFile, m_buffer, size, &br, NULL);
  CloseHandle(hFile);
  if (!ret || br != size) {
    delete[] m_buffer;
    return (FALSE);
  }

  m_buffer[size] = '\0'; //文字列として扱うときのための便宜

  m_size = size;
  return TRUE;
}

BOOL AkiFile::Save(char *filename) {
  if (!filename)
    return FALSE;
  if (!m_buffer)
    return FALSE;
  if (m_size == 0)
    return FALSE;

  //オープソ
  HANDLE hFile = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE) {
    return (FALSE);
  }

  //書き込み
  DWORD br;
  DWORD ret = ReadFile(hFile, m_buffer, m_size, &br, NULL);
  CloseHandle(hFile);

  if (!ret)
    return FALSE;
  if (br != m_size)
    return FALSE;
  return TRUE;
}

void AkiFile::Set(DWORD size, BYTE *ptr) {
  Destroy();

  m_size = size;
  m_buffer = new BYTE[m_size];

  if (ptr) {
    memcpy(m_buffer, ptr, m_size);
  }
}

/*----------------------------------------------------------------
    Output Debug String
------------------------------------------------------------------*/
void AkiGlobal::ods(const char *format, ...) {
  va_list args;
  va_start(args, format);

  vsprintf(m_ods_buffer, format, args);
  OutputDebugString(m_ods_buffer);
  OutputDebugString("\n");

  g_system.Log(m_ods_buffer, SYSLOG_DEBUG);

  va_end(args);
}

void AkiGlobal::ods2(const char *format, ...) {
  va_list args;
  va_start(args, format);

  vsprintf(m_ods_buffer, format, args);
  OutputDebugString(m_ods_buffer);

  g_system.Log(m_ods_buffer, SYSLOG_DEBUG);

  va_end(args);
}
