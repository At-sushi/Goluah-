
#ifndef GOLDAT_MYSURFACE

#define MYSUF_MAXNUMTEX	(16)

struct MYSURFACE
{
	BOOL valid;
	DWORD xsufsize[MYSUF_MAXNUMTEX];//テクスチャの幅の配列
	DWORD ysufsize[MYSUF_MAXNUMTEX];//テクスチャの高さの配列
	DWORD xsufindx[MYSUF_MAXNUMTEX];//MYSURFACE中でのleft座標
	DWORD ysufindx[MYSUF_MAXNUMTEX];//MYSURFACE中でのtop座標
	DWORD xsufnum;
	DWORD ysufnum;
	LPDIRECT3DTEXTURE8 *pTex;//pTexの配列
	float wg;
	float hg;
};

#endif

#define MAXNUMGOLUAHTEXTURES (1024)//(12+12+12+12)

struct MYPALLET
{
	BYTE blue;
	BYTE green;
	BYTE red;
	BYTE alpha;
};

struct MYRECT3D
{
	float top;
	float bottom;
	float left;
	float right;
	float z;
};

#define MINIMUM_TEXSIZE	(64)

class CDirectDraw
{
public:
	CDirectDraw(HWND hwnd,BOOL win);
	~CDirectDraw();

	BOOL InitDirectDraw(HWND hwnd,BOOL win);
	LPD3DXFONT CDirectDraw::CreateMyFont(DWORD h);
	void CleanDirectDraw();
	void StartDraw();
	void EndDraw();

	//ロード関連
	MYSURFACE* CreateSurfaceFrom256BMP(char *filename,char *pallet=NULL,BOOL dmy=TRUE);//256色のビットマップからサーフェイスを作成
	DWORD CopyOne_R5G6B5(MYPALLET src,PBYTE dst);
	DWORD CopyOne_A1R5G5B5(MYPALLET src,PBYTE dst);
	DWORD CopyOne_A4R4G4B4(MYPALLET src,PBYTE dst);
	DWORD CopyOne_A8R3G3B2(MYPALLET src,PBYTE dst);
	DWORD CopyOne_A8R8G8B8(MYPALLET src,PBYTE dst);
	BOOL CopyBB2TS(MYPALLET *pbb,DWORD bbpitch,DWORD offset_x,DWORD offset_y,LPDIRECT3DTEXTURE8 ptex,DWORD damex=0,DWORD damey=0);
	BOOL AssignTextureDiv(MYSURFACE *pmsf,DWORD width,DWORD height);
	BOOL Load256Bitmapbits(MYPALLET **bits,DWORD *width,DWORD *height,char *bmpfilename,char *palfilename=NULL);
	BOOL GetPallet(char *filename,MYPALLET *pal);//256ｂｍｐファイルから、パレットだけを取出
	void RelSurface(MYSURFACE *s);//テクスチャ開放

	void CheckBlt(MYSURFACE *dds,int x,int y,RECT r,
		BOOL revx=FALSE,BOOL revy=FALSE,DWORD flag=0,float z=0.0f,DWORD color=0xFFFFFF);
	void CheckBlt2(MYSURFACE *dds,int x,int y,RECT r,
		double magx=1.0,double magy=1.0,BOOL revx=FALSE,BOOL revy=FALSE,DWORD flag=0,float z=0.0f,DWORD color=0xFFFFFF);
	void CellDraw(MYSURFACE **psuf,LPVOID pcdat,LPVOID prdat,DWORD cn,
		int x,int y,float z,int rot,BOOL revx=FALSE,BOOL revy=FALSE,DWORD color=0xFFFFFFFF,float magx=1.0f,float magy=1.0f);
#ifdef ___GCDEDITER
	void CellDrawED(int chcol,MYSURFACE **psuf,LPVOID pcdat,LPVOID prdat,DWORD cn,
		int x,int y,float z,int rot,BOOL revx=FALSE,BOOL revy=FALSE,float magx=1.0f,float magy=1.0f,
		BOOL linkprv=TRUE,BOOL linknext=TRUE);
#endif
	void HRectDraw(LPVOID cdat,LPVOID phdat,DWORD cn,
		int x,int y,float z,int rot,BOOL revx=FALSE,BOOL revy=FALSE,float magx=1.0f,float magy=1.0f);
	void GCenterDraw(LPVOID cdat,DWORD cn,
		int x,int y,float z,int rot,BOOL revx=FALSE,BOOL revy=FALSE,float magx=1.0f,float magy=1.0f);

	void DrawLine(int sx,int sy,int gx,int gy,DWORD col);

	void ClearBackBuffer();
	void MyBlt3D(MYSURFACE *dds,RECT src,MYRECT3D dst,DWORD flag=0,DWORD color=0xFFFFFF);

	void DrawCircle(int x,int y,int rad,int w);//削除きぼん *ahyahya*
	void Draw3DText();

	//for directx8
	void ResetTransformMatrix();//view & projection
	//world
	D3DXMATRIX SetParentMatrix(D3DXMATRIX mat,BOOL root=TRUE,BOOL insert=FALSE);
	D3DXMATRIX ResetParentMatrix();
public:
	BOOL cdirectdrawready;
	LPDIRECT3D8 dd;
	LPDIRECT3DDEVICE8 d3ddev;
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDEVTYPE devtypenow;
	DWORD maxtexturewidth,maxtextureheight;
	D3DXPLANE d3dxplane_x,d3dxplane_y;
	D3DFORMAT texformat,dispformat;
	LPD3DXFONT lpFont[3];

	D3DXMATRIX matparent;
	MYSURFACE ms[MAXNUMGOLUAHTEXTURES];

	float yurey;
	float camera_x,camera_y,camera_z;
	DWORD clearbgcolor;
};

//blt系関数のフラグ
#define CKBLT_YUREY			(0x00000001)//揺れ

#define RELEASE(a)	if(a!=NULL){a->Release();a=NULL;}

// Flexible Vertex Format -----------------------------------------------------

#define FVF_AKIVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)
struct AKIVERTEX
{
	float x,y,z;
	DWORD diff;	
};

#define FVF_3DVERTEX	(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
struct MYVERTEX3D
{
	float x,y,z;//座標
	DWORD color;
	float tu,tv;//テクスチャ座標
};


//エディターに組み込む場合の処理

#ifdef ___GCDEDITER

struct CONFIG2
{
	DWORD flag;
};

#define CONFIG2_HALFMODE			0x00000001
#define CONFIG2_FULLSCREEN			0x00000002
#define CONFIG2_DEBUGMODE			0x00000004
#define CONFIG2_SHOWCHARACTERS		0x00000008
#define CONFIG2_SHOWSTAGES			0x00000010
#define CONFIG2_SHOWSTORIES			0x00000020
#define CONFIG2_ENABLEHVP			0x00000040//hardwear vertexprocessing?
//texture format
#define CONFIG2_USETEXFMT_A1R5G5B5	0x00000000
#define CONFIG2_USETEXFMT_A4R4G4B4	0x10000000
#define CONFIG2_USETEXFMT_A8R3G3B2	0x20000000
#define CONFIG2_USETEXFMT_A8R8G8B8	0x30000000
//devicetype
#define CONFIG2_DEVICE_HAL			0x00000000
#define CONFIG2_DEVICE_REF			0x01000000
#define CONFIG2_DEVICE_SW			0x02000000

#endif
