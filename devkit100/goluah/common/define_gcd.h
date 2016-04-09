
/*!
*	@file
*	@brief GCD(Goluah Cell Data) 関係 各種構造体/定数定義
*
*	Goluah!!本体と、全てのキャラクタークラスから参照される。
*
*	ver0.90からどのバージョンのGCDファイルが読み込まれたバッファか判断するために、
*	ロード時にセルの0番のcell[0]->flagにバージョン番号 (0.90だったら900) を埋め込むことにした。
*
*	Direct3Dを直接使用する場合はこのヘッダの前に d3d8.h , d3dx8.h をincludeしないと、
*	typedefが働いてビルドが通らなくなるので注意すること。
*/
#pragma once

//DirectX SDK がない環境への対応
#ifndef _D3D9_H_
typedef struct DIRECT3D9 {} *LPDIRECT3D9;
typedef struct DIRECT3DDEVICE9 {} *LPDIRECT3DDEVICE9;
typedef struct DIRECT3DTEXTURE9 {} *LPDIRECT3DTEXTURE9;
#endif // _D3D9_H_
#ifndef __D3DX9MATH_H__
typedef struct D3DXMATRIX {} *LPD3DXMATRIX;
#endif // __D3DX9MATH_H__

#define GCD_VERSION			(900)
#define GCDMAX_RECTANGLES	(1024)
#define GCDMAX_CELLS		(1024)
#define GCDMAX_IMAGES		(12)
#define GCD_CELLGROUPNUM	(8)
#define GCD_MAXDEFINELENGTH	(64)

/*!
*	@ingroup gobject
*/
/*@{*/

//! GCD切り取り矩形構造体
struct GCD_RECT
{
	DWORD bmpno;			//!< ビットマップ番号
	RECT r;					//!< 切り取り矩形領域
	int center_x,center_y;	//!< 矩形重心
};

//! GCDセル(1要素)
struct GCD_CELL
{
	DWORD cdr;				//!< 切り取り矩形番号
	int dx;					//!< Xオフセット値
	int dy;					//!< Yオフセット値
	DWORD flag;				//!< フラグ
	float magx;				//!< X拡大率
	float magy;				//!< Y拡大率
	int rot;				//!< 回転各

	float red_ratio;		//!< 赤描画率(0～1、通常1)
	float green_ratio;		//!< 緑描画率(0～1、通常1)
	float blue_ratio;		//!< 青描画率(0～1、通常1)
	float alpha_ratio;		//!< α描画率(0～1、通常1)
};

#define GCDCELL_REVERSE_X		0x00000001//!< GCD描画時にX反転を行う
#define GCDCELL_REVERSE_Y		0x00000002//!< GCD描画時にY反転を行う
#define GCDCELL_BLEND_ADD		0x00000010//!< GCD描画時に加算合成を行う

//! GCDセル (GCD_CELLのセット)
struct GCD_CELL2
{
	GCD_CELL cell[GCD_CELLGROUPNUM];	//!< セル要素
	int gcx;							//!< X重心位置
	int gcy;							//!< y重心位置
	DWORD flag;							//!< フラグ
};

#define GCDCELL2_LINK			0x00000001//!< 配列中で次のインデックスのセルの描画も行う
#define GCDCELL2_DISABLE_ZW		0x00000002//!< zバッファへの書き込みを行わない
#define GCDCELL2_DISABLE_ZT		0x00000004//!< zテストを行わない
#define GCDCELL2_SCA_GCENTER	0x00000010//!< スケール中心を重心のポイントとする（デフォルトは足元）
#define GCDCELL2_ROT_BASEPOINT	0x00000020//!< 回転中心を足元のポイントとする（デフォルトは重心）

//! GCDあたり判定矩形
struct GCD_HANTEI
{
	RECT attack[3];
	RECT kas[3];
	RECT kurai[3];
};

//! GCDファイルヘッダ
struct GCD_FILEHEADER
{
	DWORD size;			//!< 固定　この構造体のサイズらしい.最低でも8とする
	DWORD ver;			//!< 固定　どのバージョンのものか
};

//! GCD定義名構造体
struct GCD_CELLNAMES
{
	TCHAR name[GCDMAX_CELLS][GCD_MAXDEFINELENGTH];
};

//! 圧縮形式ファイルヘッダ
struct GCD_COMP_FILEHEADER
{
	BYTE signature[4];	//!< 固定　ファイル識別用のシグネチャ
	DWORD ver;			//!< 固定　どのバージョンのものか
	DWORD flags;		//!< フラグ
};

#define GCDFILE_COMP_HAVE_CELLNAMES	0x00000001 //!< セル定義名を含む
#define GCDFILE_COMP_USING_FILTER	0x00000002 //!< フィルターを使用（まだ未実装）

//! 圧縮形式データヘッダ
struct GCD_COMP_DATAHEADER
{
	DWORD DataSize;		//!< データ（圧縮後）のサイズ
	DWORD CRCCheckSum;	//!< 元データのCRC値
};


// old version ********************************************************

//0.90
typedef GCD_FILEHEADER		GCD_FILEHEADER_090;
typedef GCD_CELL			GCD_CELL_090;
typedef GCD_CELL2			GCD_CELL2_090;
typedef GCD_RECT			GCD_RECT_090;
typedef GCD_HANTEI			GCD_HANTEI_090;
typedef GCD_CELLNAMES		GCD_CELLNAMES_090;

//0.70
typedef GCD_FILEHEADER		GCD_FILEHEADER_070;
typedef GCD_RECT			GCD_RECT_070;
typedef GCD_HANTEI			GCD_HANTEI_070;
typedef GCD_CELLNAMES		GCD_CELLNAMES_070;

//!旧バージョンCELL構造体(1要素)
struct GCD_CELL_070
{
	DWORD cdr;
	int dx;
	int dy;
	DWORD flag;
	float magx;
	float magy;
	int rot;
};

//!旧バージョンCELL構造体
struct GCD_CELL2_070
{
	GCD_CELL_070 cell[8];
	int gcx;
	int gcy;
	DWORD flag;
};

//*********************************************************************
#define MYSUF_MAXNUMTEX	(16)		//!< MYSURFACEテクスチャ分割限界値

/*!
*	@brief 独自形式インチキサーフェイス
*
*	2^n 縛りを回避して任意の大きさのビットマップをロードするために、
*	適切なサイズで画像を分割して複数のテクスチャとして保持する。
*	描画はCDirectDrawの専用関数を使って行う。
*/
struct MYSURFACE
{
	BOOL valid;
	DWORD xsufsize[MYSUF_MAXNUMTEX];//!< テクスチャの幅の配列
	DWORD ysufsize[MYSUF_MAXNUMTEX];//!< テクスチャの高さの配列
	DWORD xsufindx[MYSUF_MAXNUMTEX];//!< MYSURFACE中でのleft座標
	DWORD ysufindx[MYSUF_MAXNUMTEX];//!< MYSURFACE中でのtop座標
	DWORD xsufnum;					//!< X方向テクスチャ分割数
	DWORD ysufnum;					//!< Y方向テクスチャ分割数
	LPDIRECT3DTEXTURE9 *pTex;		//!< テクスチャ配列
	float wg;						//!< ビットマップの幅
	float hg;						//!< ビットマップの高さ
};

//! 3次元的描画矩形指定用構造体
struct MYRECT3D
{
	float top;
	float bottom;
	float left;
	float right;
	float z;
};

#ifndef FVF_3DVERTEX
#define FVF_3DVERTEX	(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
struct MYVERTEX3D
{
	float x,y,z;//!< 座標
	DWORD color;//!< 頂点色
	float tu,tv;//!< テクスチャ座標
};
#endif

/*@}*/