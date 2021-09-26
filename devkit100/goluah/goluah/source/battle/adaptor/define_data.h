
// ビットマップ用構造体 ********************************************
/*#define MYSUF_MAXNUMTEX	(16)
struct MYSURFACE
{
        BOOL valid;
        DWORD xsufsize[MYSUF_MAXNUMTEX];//テクスチャの幅の配列
        DWORD ysufsize[MYSUF_MAXNUMTEX];//テクスチャの高さの配列
        DWORD xsufindx[MYSUF_MAXNUMTEX];//MYSURFACE中でのleft座標
        DWORD ysufindx[MYSUF_MAXNUMTEX];//MYSURFACE中でのtop座標
        DWORD xsufnum;
        DWORD ysufnum;
        LPDIRECT3DTEXTURE8 *pTex;		//テクスチャ配列。d3d8.hを組み込まない場合はLPVOID型に。
        float w;						//ビットマップの幅
        float h;						//ビットマップの高さ
        float w2; //ビットマップの幅（320x240モードでも変化無し） float h2;
//ビットマップの高さ（320x240モードでも変化無し）
};

struct MYRECT3D
{
        float top;
        float bottom;
        float left;
        float right;
        float z;
};
*/
// キャラクター関係の構造体 ***************************************

#define MAXNUM_RECTANGLES (1024)
#define MAXNUM_CELLS (1024)

struct CHARDAT_RECT {
  DWORD bmpno; //使用するビットマップの番号(0から)
  RECT r;      //切り取り矩形
};

struct CHARDAT_CELL {
  DWORD cdr;   // CHARDAT_RECT構造体の番号
  int dx;      //ずらし幅x
  int dy;      //ずらし幅y
  DWORD flag;  //↓フラグ
  double magx; //拡大率x
  double magy; //拡大率y
  DWORD res;   //？忘れた
};

#define CHARDAT_CELL_REVERSE_X 0x00000001 // x反転
#define CHARDAT_CELL_REVERSE_Y 0x00000002 // y反転

struct CHARDAT_CELL2 {
  RECT attack;          //攻撃矩形
  RECT kas;             //重なり矩形
  RECT kurai;           //くらい矩形
  CHARDAT_CELL cell[8]; // CHARDAT_CELL配列
};

struct CHARACTERINFOS {
  MYSURFACE *dds[12];                   //読み込んだビットマップ
  CHARDAT_RECT rdat[MAXNUM_RECTANGLES]; //
  CHARDAT_CELL2 cdat[MAXNUM_CELLS];     //
  BOOL iscomputerplayer;                //コンピュータかどうか
};

// 以下キャラクタ・ステージの製作とはあんまり関係なし *******************************************************
struct CHARDAT_FILEHEADER2 //ファイル先頭
{
  DWORD size;      //構造体サイズ
  DWORD num_rects; //入っているCHARDAT_RECT構造体の数
  DWORD num_cells; //入っているCHARDAT_CELL2構造体の数
  DWORD flag;      //忘れた
};

struct CHARDAT_FILEHEADER //二番目(ないこともあり)
{
  //	char path[256];
  //	char filename[64];
  char name[MAXNUM_CELLS][64]; //セルにつけた名前
};

#define MAX_IMAGES 12

// 旧フォーマット ******************************************
struct DATOLD_FILEHEADER {
  char path[4][256];
  char name[MAXNUM_CELLS][64];
};

struct DATOLD_CELL {
  DWORD cdr[3];
  int dx[3];
  int dy[3];
  DWORD flag[3];

  RECT attack[3];
  RECT kas[3];
  RECT kurai[3];
};
