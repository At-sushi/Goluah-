
/*!
*	@file
*	@brief DX描画系
*
*	実際にはDirectDrawではなくDirect3Dです。
*/
/*!
*	@defgroup DirectX
*	@brief DirectX
*/
#pragma once

#ifdef GCD_EDITER
#include <d3d9.h>
#include <d3dx9.h>
#endif
#include "define_gcd.h"	//GCDフォーマットが必要

#define MAXNUMGOLUAHTEXTURES (1024)		//!<独自形式偽テクスチャー最大管理数
#define MINIMUM_TEXSIZE	(64)			//!<最小テクスチャーサイズ。コレ以下のサイズのテクスチャしか作れないような環境は知らん。

#define AKIDX_FONTNUM	(4)

/*!
*	@ingroup DirectX
*	@brief 画像ビット変換時利用構造体
*/
struct MYPALLET
{
	BYTE blue;
	BYTE green;
	BYTE red;
	BYTE alpha;
};
struct Vtx {
	float x, y, z;
	float u, v;
};
//!CDirectDrawクラスの状態？
enum CDIRECTDRAW_STATE
{
	CDDSTATE_NOINITIALIZE,		//!<まだ初期化されていない
	CDDSTATE_READY,				//!<準備が完了している
	CDDSTATE_ERROR				//!<エラーが発生している。利用できない。
};

/*!
*	@ingroup DirectX
*	@brief 描画管理クラス。
*	
*	現在はDirectX8以上のDirect3Dを使用するが、以前はDirectDrawを使っていたのでこんな名前。
*	由緒正しい古株クラス。
*
*	2D描画：
*	任意サイズのビットマップを最大テクスチャサイズで分割した独自形式のニセサーフェイスを
*	使用することによって、DirectDrawのときと同じような感じで描画を行うことができる。
*	（2の累乗以外のサイズのビットマップを使ってもボケたりしない）
*
*	3D描画：
*	Direct3DDeviceオブジェクトを渡すからあとはDXのAPI自分でたたいてなんとかしておくれ？
*/
class CDirectDraw
{
public:
	CDirectDraw();
	~CDirectDraw(){Destroy();}

	//■初期化と破棄
	BOOL Initialize(HWND hwnd,BOOL win);
	void Destroy();

	//■描画の開始と終了
	void StartDraw(BOOL erbs);//描画を開始する前にこの関数をコールする必要がある
	void EndDraw();//描画を終了し、FLIP動作を行う

	//■ファイルロード・アンロード関連
	//256色のビットマップから独自形式サーフェイスを作成
	MYSURFACE* CreateSurfaceFrom256BMP(TCHAR *filename,TCHAR *pallet=NULL,BOOL dmy=TRUE);
	void RelSurface(MYSURFACE *s);//テクスチャ開放

	//!各種形式の画像を読み込む。PNGと合わせるために追加。
	MYSURFACE* CreateSurfaceFrom256Image(TCHAR *filename,TCHAR *pallet=NULL,BOOL dmy=TRUE);

	//■2D基本描画
	//独自形式サーフェイス転送。転送位置を3D空間上で指定できる
	void MyBlt3D(MYSURFACE *dds,RECT src,MYRECT3D dst,DWORD flag=0,DWORD color=0xFFFFFFFF);
	//範囲チェック転送
	//DirectDrawのときのコードとの互換のために作成されたようなきがする
	void CheckBlt(MYSURFACE *dds,int x,int y,RECT r,
		BOOL revx=FALSE,BOOL revy=FALSE,DWORD flag=0,float z=0.0f,DWORD color=0xFFFFFFFF, BOOL drawShadow = FALSE);
	//↑の簡易版
	void CheckBlt2(MYSURFACE *dds,int x,int y,RECT r,
		double magx=1.0,double magy=1.0,BOOL revx=FALSE,BOOL revy=FALSE,DWORD flag=0,float z=0.0f,DWORD color=0xFFFFFFFF, BOOL drawShadow = FALSE);

	//■フロントバッファのコピー
	//現在のフロントバッファの内容をコピーしたテクスチャーを生成して返す。
	//D3Dのヘルプによると、処理コストはデカイらしい
	#ifndef GCD_EDITER
	LPDIRECT3DTEXTURE9 GetFrontBufferCopy();
	DWORD* GetFrontBufferCopyRaw(UINT *wdt,UINT *hgt);
	#endif

	//■Goluah特化描画関数
	//GCD形式セル描画
	void CellDraw(MYSURFACE **psuf,LPVOID pcdat,LPVOID prdat,DWORD cn,
		int x,int y,float z,int rot,BOOL revx=FALSE,BOOL revy=FALSE,DWORD color=0xFFFFFFFF,float magx=1.0f,float magy=1.0f, BOOL shadowed=FALSE);
	void CellDraw090(MYSURFACE **psuf,LPVOID pcdat,LPVOID prdat,DWORD cn,
		int x,int y,float z,int rot,BOOL revx=FALSE,BOOL revy=FALSE,DWORD color=0xFFFFFFFF,float magx=1.0f,float magy=1.0f, BOOL shadowed=FALSE);
	void CellDraw070(MYSURFACE **psuf,LPVOID pcdat,LPVOID prdat,DWORD cn,
		int x,int y,float z,int rot,BOOL revx=FALSE,BOOL revy=FALSE,DWORD color=0xFFFFFFFF,float magx=1.0f,float magy=1.0f, BOOL shadowed=FALSE);
#ifdef GCD_EDITER
	//指定矩形を強調して描画
	void CellDrawED(MYSURFACE **psuf,LPVOID pcdat,LPVOID prdat,DWORD cn,
		int x,int y,float z,int rot,BOOL revx,BOOL revy,DWORD color,float magx,float magy,
		UINT ed_rect,					//編集中の矩形番号
		BOOL color_modulat,				//強調表示を行う
		BOOL is_previous_cell=FALSE,	//*さかのぼって描画されたセルかどうか
		BOOL is_next_cell=FALSE,		//*"次"扱いで描画されたセルかどうか
		BOOL is_edit_cell=TRUE);		//*現在編集ちゅうのセルかどうか);
#endif

	//あたり判定描画
	void HanteiDraw(LPVOID pcdat,LPVOID phdat,DWORD cn,BOOL b_atr,BOOL b_kas,BOOL b_atk,
		int x,int y,float z,int rot,BOOL revx=FALSE,BOOL revy=FALSE,float magx=1.0f,float magy=1.0f);
	//矩形描画
	void HRectDraw(LPVOID cdat,LPVOID phdat,DWORD cn,
		int x,int y,float z,int rot,BOOL revx=FALSE,BOOL revy=FALSE,float magx=1.0f,float magy=1.0f);
	//重心描画
	void GCenterDraw(LPVOID cdat,DWORD cn,
		int x,int y,float z,int rot,BOOL revx=FALSE,BOOL revy=FALSE,float magx=1.0f,float magy=1.0f);

	//■その他描画
	//ライン描画
	void DrawLine(int sx,int sy,int gx,int gy,DWORD col);
	//バックバッファのクリア
	void ClearBackBuffer();
	//円の描画
	void DrawCircle(int x,int y,int rad,int w,
		float z=0,DWORD color=0xFF55AAFF,BOOL toumei=FALSE,float rot=0,BOOL rot_y=TRUE,BOOL hosei=TRUE);
	//なにこれ？
	void Draw3DText();
	//テキスト描画
	void DrawBlueText(RECT& r,TCHAR *text,int len,DWORD method,DWORD size);
	void DrawRedText(RECT& r,TCHAR *text,int len,DWORD method,DWORD size);
	// 独自描画軽量化版
//	void MyBlt3DLite(MYSURFACE* dds, RECT& r, int x, int y, BOOL noAlpha);
	// サーフェスに直接書き込み
//	void Kakikomi(LPDIRECT3DTEXTURE9 SouShin, RECT& r2, LPDIRECT3DTEXTURE9 BackBuffer, POINT& point, BOOL revx);

	//■便利関数？
	void SetAlphaMode(DWORD alphamode);//αブレンドのモードを設定
	void EnableZ(BOOL test=TRUE,BOOL write=TRUE);//Zテスト・ライトの有効・無効を設定

	//■座標変換マトリクス関連
	//for directx8
	void ResetTransformMatrix();//view & projection
	void SetTransform(BOOL b);
	void SetWorldMatrix(D3DXMATRIX *pmat);
	//world
	D3DXMATRIX SetParentMatrix(D3DXMATRIX& mat,BOOL root=TRUE,BOOL insert=FALSE);
	D3DXMATRIX ResetParentMatrix();
	void SetParentMatrix2(D3DXMATRIX *mat,BOOL root,D3DXMATRIX *matprv);

	//■デバッグ用関数
	//D3Dエラーのトレース
	void TraceCreateDeviceError(HRESULT ret,HWND hwnd);

	//! ウィンドウサイズ変更処理
	void OnWindowResized(int width, int height);

	//! スクリーンモード変更
	BOOL ChangeScreenMode(BOOL win);
	void ReduceColor(DWORD alpha, bool isShadow = false);//!<画面を暗くするエフェクト、未完成。
	bool StencilEnable()	{ return stencil_enable; }
	void UpdateD3DPP()		{ ResetDirectDraw(); }

private:
	//■ビットマップロード時内部関数
	//テクスチャーフォーマット変換関数
	DWORD CopyOne_R5G6B5(MYPALLET src,PBYTE dst);
	DWORD CopyOne_A1R5G5B5(MYPALLET src,PBYTE dst);
	DWORD CopyOne_A4R4G4B4(MYPALLET src,PBYTE dst);
	DWORD CopyOne_A8R3G3B2(MYPALLET src,PBYTE dst);
	DWORD CopyOne_A8R8G8B8(MYPALLET src,PBYTE dst);
	//ビットマップロード時に分割テクスチャ数を計算
	BOOL AssignTextureDiv(MYSURFACE *pmsf,DWORD width,DWORD height);
	//256色ビットマップのイメージ部分をロードする
	BOOL Load256Bitmapbits(MYPALLET **bits,DWORD *width,DWORD *height,TCHAR *bmpfilename,TCHAR *palfilename=NULL);
	BOOL Load256PNGbits(MYPALLET **pbits,DWORD *width,DWORD *height,TCHAR *pngfilename,TCHAR *palfilename);//PNG版
	BOOL LoadJPEGbits(MYPALLET **pbits,DWORD *width,DWORD *height,TCHAR *jpegfilename);//JPEG版（フルカラー）
	//256ビットマップのパレット部分をロードする
	BOOL GetPallet(TCHAR *filename,MYPALLET *pal);//256ｂｍｐファイルから、パレットだけを取出
	BOOL GetPalletPNG(TCHAR *filename,MYPALLET *pal);//256PNGファイルから、パレットだけを取出

	//■便利関数？
	//バックバッファからテクスチャへの転送
	//・・・そんな関数作ってたっけ?
	//…違うと思われ、ビットマップをビットごとにテクスチャに貼り付けてるみたいです。
	BOOL CopyBB2TS(MYPALLET *pbb,DWORD bbpitch,DWORD offset_x,DWORD offset_y,LPDIRECT3DTEXTURE9 ptex,DWORD damex=0,DWORD damey=0);

	//■生成破棄時内部関数
	//Direct3Dを初期化する。関数名は昔の名残っぽい
	BOOL InitDirectDraw(HWND hwnd,BOOL win);
	//テキスト描画用のフォントを生成する
	LPD3DXFONT CreateMyFont(DWORD h);
	//高速軽量文字作成・・・したかった
//	int CreateMyFont2(DWORD h, TCHAR *text); // color,x,y,z,…
	//Direct3D後始末関数
	void CleanDirectDraw();
	//レンダリングステート設定
	void InitStates();
	//! リセット用、DirectDrawデバイスが消えたときに呼び出されます。
	void ResetDirectDraw();

	//■データメンバ
public:
	CDIRECTDRAW_STATE state;			//!<クラスの状態
	LPDIRECT3D9 dd;						//!<[重要]Direct3Dオブジェクト
	LPDIRECT3DDEVICE9 d3ddev;			//!<[重要]Direct3Dデバイスオブジェクト
	D3DPRESENT_PARAMETERS d3dpp;		//!<なんだっけ？これ。
	D3DDEVTYPE devtypenow;				//!<現在のデバイスタイプ。HALとかSWとか、そういうの
	DWORD maxtexturewidth,
					maxtextureheight;	//!<デバイスで規定されるテクスチャの最大サイズ
	D3DFORMAT texformat,dispformat;		//!<使用する画像フォーマット

	D3DXMATRIX matparent;				//!<2D描画関数マトリックス
	MYSURFACE ms[MAXNUMGOLUAHTEXTURES];	//!<独自形式サーフェイス管理

	float yurey;						//!<画面揺れエフェクトに使用するY方向のずらし量
	float center_x;						//2!<D描画座標系中心X
	float camera_x,camera_y,camera_z;	//!<カメラの位置
	float camera_zurax,camera_zuray;	//!<カメラずらし？よく分からん
	DWORD clearbgcolor;					//!<バックバッファクリア時に使用する塗りつぶし色
	LPD3DXFONT lpFont[AKIDX_FONTNUM];	//!<テキスト描画に使用するフォント
	//LPD3DXSPRITE pSprite;				//!<スプライト(゜д゜)ウマー
	LPDIRECT3DVERTEXBUFFER9 pMyVertex;	//!<頂点バッファ、一応作ってみた。
private:
	D3DXPLANE d3dxplane_x,d3dxplane_y;	//!<マトリクス回転に使用する X,Y 平面
	BOOL m_is_win;						//!<ウィンドウモードでの起動かどうか
	UINT m_desktop_width;				//!<ウィンドウモードの場合、デスクトップの幅
	UINT m_desktop_height;				//!<ウィンドウモードの場合、デスクトップの高さ
	bool stencil_enable;				//!<ステンシルが使えるかどうか
};



//blt系関数のフラグ
#define CKBLT_YUREY			(0x00000001)//!<揺れを与える

// Flexible Vertex Format -----------------------------------------------------
// 使用するDirect3D頂点フォーマット
#ifndef FVF_3DVERTEX
#define FVF_3DVERTEX	(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

/*!
*	@ingroup DirectX
*	@brief D3D描画で使用する頂点フォーマット
*/
struct MYVERTEX3D
{
	float x,y,z;//!< 座標
	DWORD color;//!< 頂点色
	float tu,tv;//!< テクスチャ座標
};
#endif//FVF_3DVERTEX


