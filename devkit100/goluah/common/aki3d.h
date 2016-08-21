
/*!
*	@file
*	@brief aki3d.lib 定義
*
*	aki3d.lib の機能定義。
*	Goluah!!本体、キャラクター/ステージDLL(aki製のもの)から参照される
*/
#pragma once
#define _aki3d_h_

#include <vector>
#include "d3dx9.h"
#include <tchar.h>

#define FAR_CLIP	(50.0f)
#define NEAR_CLIP	( 0.1f)

/*----------------------------------------------------------
	使用するDirect3D頂点フォーマット　定義
------------------------------------------------------------*/
#ifndef FVF_3DVERTEX
#define FVF_3DVERTEX	(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
/*!
*	@brief 描画に使用する頂点構造体
*
*	D3D8の柔軟な頂点フォーマットとかいうやつ。
*	シェーダー設定では FVF_3DVERTEX を使用する
*/
struct MYVERTEX3D
{
	float x,y,z;	//!< 座標
	DWORD color;	//!< 頂点カラー
	float tu,tv;	//!< テクスチャ座標
};
#endif//FVF_3DVERTEX

/*---------------------------------------------------------
	型定義
-----------------------------------------------------------*/
typedef D3DXVECTOR3 V3d;

/*!
*	@brief 2次元ベクトル構造体
*
*	描画用というよりは、秒構えの物理計算等で使用？
*	いくつか便利関数アリ
*/
struct V2d
{
	float x,y;

	void Normalize();				//!< 正規化
	void Verticalize();				//!< 垂直化(長さが0の場合は適当な向きの正規ベクトル)
	float Length();					//!< 長さ取得
	void SetLength(float len);		//!< 長さ設定(向きは残る)

	V2d& operator=(const V2d& v);
	V2d  operator+(const V2d& v);
	V2d  operator-(const V2d& v);
	V2d& operator+=(const V2d& v);
	V2d& operator-=(const V2d& v);
	V2d  operator*(const float& d);
	V2d& operator*=(const float& d);
	V2d  operator/(const float& d);
};

#ifndef ARGB_STRUCT
/*!
*	@brief 色情報構造体
*
*	ARGB個別形式と、DWORD32ビット形式との相互変換が可能
*/
struct ARGB
{
	ARGB(){}
	ARGB(BYTE a,BYTE r,BYTE g,BYTE b){ blue=b; green=g; red=r; alpha=a; }

	union
	{
		struct{
			BYTE blue;
			BYTE green;
			BYTE red;
			BYTE alpha;
		};
		DWORD dwcol;
	};
};
#define ARGB_STRUCT
#endif

/*---------------------------------------------------------
	基本ベクトル
-----------------------------------------------------------*/
extern V3d Xaxis;
extern V3d Yaxis;
extern V3d Zaxis;
extern V3d Zero;

/*----------------------------------------------------------
	エフェクトサービスクラス
------------------------------------------------------------*/

class CTristrip;
class CParticle;
class CFlatBoards;
class CMesh;

typedef void (*WarningReportFunctionPtr)(const TCHAR* war_str);

/*!
*	@brief 3D描画サービスクラス
*
*	主にDLL向けに3D描画機能を提供する。テクスチャ管理等も。
*	このクラスはライブラリ中に aki3d というグローバルなインスタンスが存在するので、
*	それを使用すること。
*
*	取得した各クラスのインスタンスはこのクラスの破棄時に自動的にdeleteされるので、
*	取得側でdeleteしないこと。(生成したインスタンスをリストで持ってるので)
*	インスタンスの生成と破棄が頻繁に行われる場合(goluah本体)は、Destroy〜系の関数で破棄を行う。
*	そのときテクスチャーも参照カウントにかかわらず全て破棄される。
*/
class Aki3d
{
public:
	Aki3d(){d3ddev=NULL;}

	//!初期化。全ての操作の前に行うこと
	void Initialize(
		LPDIRECT3DDEVICE9 d3d_device,		//!< D3Dデバイスオブジェクト
		const TCHAR*	tex_path,				//!< テクスチャロード用のベースディレクトリ
		WarningReportFunctionPtr pwf=NULL	//!< エラーリポート用の関数ポインタ
		);
	~Aki3d(){Destroy();}

	CTristrip*		CreateTristripObject();		//!< CTristripクラスインスタンス取得。deleteしないこと
	CParticle*		CreateParticleObject();		//!< CParticleクラスインスタンス取得。deleteしないこと
	CFlatBoards*	CreateFlatBoardsObject();	//!< CFlatBordsクラスインスタンス取得。deleteしないこと
	CMesh*			CreateMeshObject();			//!< CMeshクラスインスタンス取得。deleteしないこと

	void DestroyTristripObject(CTristrip*);		//!< CTristripクラスインスタンス明示破棄。
	void DestroyParticleObject(CParticle*);		//!< CParticleクラスインスタンス明示破棄。
	void DestroyFlatBoardsObject(CFlatBoards*);	//!< CFlatBordsクラスインスタンス明示破棄。
	void DestroyMeshObject(CMesh*);				//!< CMeshクラスインスタンス明示破棄。

	/*!
	*	@brief マトリックス生成
	*	@param scale スケール。マイナス値で反転を行うことができる
	*	@param rotation ベクトルの向きを回転軸として、長さをラジアン単位の回転角として解釈
	*	@param trans 平行移動量
	*	@return 指定ベクトルから生成した座標変換マトリクス
	*	
	*	引数がそれぞれNULLの場合に該当する操作を行わない.
	*	ゼロベクトルが操作なしではないことに注意すること
	*	（例えばゼロベクトルによるスケールを行うと描画されなくなる）
	*/
	D3DXMATRIX& CreateMatrix( V3d* scale, V3d* rotation, V3d* trans );

public:
	static float RandomOne();		//!< 0〜1実数ランダム
	static float RandomOne2();		//!< -1〜1実数ランダム
	float zo_tri(float t,float mid=0.5f);

	//レンダーステート設定
	void EnableZ(BOOL t=TRUE,BOOL w=TRUE);
	void SetBlend_Add();
	void SetBlend_Normal();
	void SetBlend_Nega();

	//テクスチャー
	LPDIRECT3DTEXTURE9 LoadTexture(const TCHAR *filename);
	void UnloadTexture(LPDIRECT3DTEXTURE9 ptex);

	LPDIRECT3DDEVICE9 GetD3DDev(){ return d3ddev; }

	void LogWarning(const TCHAR* fmt,...);

protected:
	void Destroy();

	LPDIRECT3DDEVICE9 d3ddev;		//D3Dデバイス
	WarningReportFunctionPtr warning_report;
};

extern Aki3d aki3d;


/*!
*	@brief △ストリップ描画クラス
*
*	連続した滑らかな線を描くことができる。
*	Createでノード(節)数を指定し、各ノードに対して位置と太さを設定すると、
*	それらをつないだトライアングルストリップを計算して描画する。
*
*	インスタンスはAki3d::CreateTristripObject で取得する。
*	破棄はAki3dが行うので、取得したインスタンスをdeleteしないこと。
*
*	@sa Aki3d
*/
class CTristrip
{
public:
	virtual void Create(UINT num_node , const TCHAR* tex_filename) = 0;
	virtual void Destroy() = 0;
	virtual void Render(D3DMATRIX *ltm = NULL) = 0;

	virtual void SetWidth(UINT index,float value) = 0;
	virtual void SetPos(UINT index,V3d& value) = 0;
	virtual void SetColor(UINT index,ARGB value) = 0;
	virtual void SetColorDW(UINT index,DWORD value) = 0;

	virtual ~CTristrip(){}
};


/*!
*	@brief つぶつぶ描画クラス
*
*	基本的に、位置と大きさを指定して丸い(テクスチャーによる)点々を描画する。
*	Axisを設定すると、その方向に粒を伸ばして楕円状にすることができる。
*
*	インスタンスはAki3d::CreateParticleObject で取得する。
*	破棄はAki3dが行うので、取得したインスタンスをdeleteしないこと。
*
*	@sa Aki3d
*/
class CParticle
{
public:
	virtual void Create(UINT num_node , const TCHAR* tex_filename) = 0;
	virtual void Destroy() = 0;
	virtual void Render(D3DMATRIX *ltm = NULL) = 0;

	virtual void SetWidth(UINT index,float value) = 0;
	virtual void SetPos(UINT index,V3d& value) = 0;
	virtual void SetColor(UINT index,ARGB value) = 0;
	virtual void SetColorDW(UINT index,DWORD value) = 0;
	virtual void SetAxis(UINT index,V3d& value) = 0;

	virtual ~CParticle(){}
};


/*!
*	@brief メッシュ描画クラス
*
*	インスタンスはAki3d::CreateMeshObject で取得する。
*	破棄はAki3dが行うので、取得したインスタンスをdeleteしないこと。
*
*	@sa Aki3d
*/
class CMesh
{
public:
	virtual void Create(const TCHAR* x_filename) = 0;
	virtual void Destroy() = 0;
	virtual void Render(D3DMATRIX *ltm = NULL) = 0;

	virtual void SetWidth(float value) = 0;
	virtual void SetPos(V3d& value) = 0;
	virtual void SetRot(V3d& value) = 0;
	/*virtual void SetColor(UINT index,ARGB value) = 0;
	virtual void SetColorDW(UINT index,DWORD value) = 0;*/
	virtual void SetLightDir(V3d& value) = 0;
	virtual void SetAmbient(D3DCOLOR value) = 0;
	virtual void SetSpecular(BOOL value) = 0;

	virtual ~CMesh(){}
};


/*!
*	@brief フラットボード描画クラス
*
*	CParticleと似ているが、こちらは大きさの指定はない。
*	Axis1 と Axis2 で指定された平たい四角形プリミティブを描画する。
*
*	インスタンスはAki3d::CreateFlatBordObject で取得する。
*	破棄はAki3dが行うので、取得したインスタンスをdeleteしないこと。
*
*	@sa Aki3d
*/
class CFlatBoards
{
public:
	virtual void Create(UINT num_node , const TCHAR* tex_filename) = 0;
	virtual void Destroy() = 0;
	virtual void Render(D3DMATRIX *ltm = NULL) = 0;

	virtual void SetPos(UINT index,V3d value)		= 0;
	virtual void SetAxis1(UINT index,V3d value)		= 0;
	virtual void SetAxis2(UINT index,V3d value)		= 0;
	virtual void SetColor(UINT index,ARGB value)	= 0;
	virtual void SetColorDW(UINT index,DWORD value)	= 0;

	virtual ~CFlatBoards(){}
};


/*--------------------------------------------------------
	マクロ
----------------------------------------------------------*/
#ifndef RELEASE
#define RELEASE(a)			if(a){a->Release();a=NULL;}
#endif

#ifndef DELETEARRAY
#define DELETEARRAY(a)		if(a){delete[]a;a=NULL;}
#endif

#ifndef SAFEDELETE
#define SAFEDELETE(a)			if(a){delete(a);a=NULL;}
#endif

