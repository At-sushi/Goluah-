
/*========================================================
	
	内部定義

==========================================================*/
#pragma once
#include "aki3d.h"

/*--------------------------------------------------------
	テクスチャ管理
----------------------------------------------------------*/
class CTexManager
{
public:
	CTexManager(){ m_texPath=NULL; }
	~CTexManager(){Destroy();}
	void SetBasePath(const char *path);

	LPDIRECT3DTEXTURE8 LoadTexture(const char *filename);
	void UnloadTexture(LPDIRECT3DTEXTURE8 ptex);

protected:
	void Destroy();

	char*							m_texPath;
	std::vector<LPDIRECT3DTEXTURE8>	m_texList;
	std::vector<char*>				m_texNameList;
	std::vector<UINT>				m_texRefcntList;
};

extern CTexManager* tex_man;


/*--------------------------------------------------------
	Tストリップ実体
----------------------------------------------------------*/
class CTristripBody : public CTristrip
{
public:
	CTristripBody();
	~CTristripBody(){Destroy();}

	void Create(UINT num_node , const char* tex_filename);
	void Destroy();
	void Render(D3DMATRIX *ltm = NULL);

	void SetWidth(UINT index,float value)	{if(index<node_num)rad[index]=value*2.0f;}
	void SetPos(UINT index,V3d& value)		{if(index<node_num)pos[index]=value;}
	void SetColor(UINT index,ARGB value)	{if(index<node_num)col[index]=value;}
	void SetColorDW(UINT index,DWORD value)	{if(index<node_num)col[index].dwcol=value;}

protected:
	virtual void SetupTexCoords();	//Create時にテクスチャ座標を設定する。

	UINT				node_num;	//ノード(節)数
	float				*rad;		//節の太さ
	V3d					*pos;		//節の位置
	ARGB				*col;		//節のの色

	V2d					*vtemp;		//テンポラリ頂点バッファ。以下のバッファはこれをオフセットして使うので、deleteしないこと
	V2d					*vsubs;		//各ノード間のsubベクトルと垂直なベクトル
	V2d					*norms;		//各ノードの法線みたいなもの
	MYVERTEX3D			*vb;		//描画用頂点バッファ

	LPDIRECT3DTEXTURE8	tex;		//テクスチャ

	BOOL				zenka;		//前科
};

/*--------------------------------------------------------
	パーティクル 実体
----------------------------------------------------------*/
#define CParticleBody_3D
class CParticleBody : public CParticle
{
public:
	CParticleBody();
	~CParticleBody(){Destroy();}

	void Create(UINT num_node , const char* tex_filename);
	void Destroy();
	void Render(D3DMATRIX *ltm = NULL);

	void SetWidth(UINT index,float value)	{if(index<num)rad[index]=value;}
	void SetPos(UINT index,V3d& value)		{if(index<num)pos[index]=value;}
	void SetColor(UINT index,ARGB value)	{if(index<num)col[index]=value;}
	void SetColorDW(UINT index,DWORD value)	{if(index<num)col[index].dwcol=value;}
	void SetAxis(UINT index,V3d& value)		{if(index<num)ax[index] =value;}

protected:
	virtual void SetupTexCoords();	//Create時にテクスチャ座標を設定する。

	UINT				num;		//粒子数
	V3d					*pos;		//位置
	ARGB				*col;		//色
	float				*rad;		//大きさ
	V3d					*ax;		//のばし軸

	MYVERTEX3D			*vb;		//描画用頂点バッファ
	WORD				*index_arr;	//頂点インデックスリスト

	LPDIRECT3DTEXTURE8	tex;		//テクスチャ
	
	#ifdef CParticleBody_3D
	D3DXVECTOR4			*tpos;		//座標変換済み頂点
	D3DXVECTOR3			*tax;		//座標変換済みベクトル
	#endif
	
	BOOL				zenka;		//前科
};


/*--------------------------------------------------------
	フラットパーティクル 実体
----------------------------------------------------------*/
class CFlatBoardsBody : public CFlatBoards
{
public:
	CFlatBoardsBody();
	~CFlatBoardsBody(){Destroy();}
	
	void Create(UINT num_node , const char* tex_filename);
	void Destroy();
	void Render(D3DMATRIX *ltm = NULL);

	void	SetPos(UINT index,V3d value)		{if(index<num)pos[index]=value;}
	void	SetAxis1(UINT index,V3d value)		{if(index<num)ax1[index]=value;}
	void	SetAxis2(UINT index,V3d value)		{if(index<num)ax2[index]=value;}
	void	SetColor(UINT index,ARGB value)		{if(index<num)col[index]=value;}
	void	SetColorDW(UINT index,DWORD value)	{if(index<num)col[index].dwcol=value;}

protected:
	UINT		num;		//粒子数
	V3d			*pos;		//位置
	V3d			*ax1;		//ジク1
	V3d			*ax2;		//ジク2
	ARGB		*col;		//色
	MYVERTEX3D	*vb;		//描画用頂点バッファ
	WORD		*index_arr;	//頂点インデックスリスト
	LPDIRECT3DTEXTURE8 tex;	//テクスチャ
	
	BOOL		zenka;		//前科
};


/*--------------------------------------------------------
	メッシュ実体
----------------------------------------------------------*/
class CMeshBody : public CMesh
{
public:
	CMeshBody();
	~CMeshBody(){Destroy();}

	void Create(const char* x_filename);
	void Destroy();
	void Render(D3DMATRIX *ltm = NULL);

	void SetWidth(float value)	{rad=value;}
	void SetPos(V3d& value)		{pos=value;}
	void SetRot(V3d& value)		{rot=value;}
	/*void SetColor(UINT index,ARGB value)	{if(index<node_num)col[index]=value;}
	void SetColorDW(UINT index,DWORD value)	{if(index<node_num)col[index].dwcol=value;}*/
	void SetLightDir(V3d& value)		{ldir=value;}
	void SetAmbient(D3DCOLOR value)		{amb=value;}
	void SetSpecular(BOOL value)		{isspecular=value;}

protected:
	DWORD				node_num;	//ノード(節)数
	float				rad;		//節の太さ
	V3d					pos;		//節の位置
	V3d					rot;		//回転
	/*ARGB				*col;		//節のの色

	V2d					*vtemp;		//テンポラリ頂点バッファ。以下のバッファはこれをオフセットして使うので、deleteしないこと
	V2d					*vsubs;		//各ノード間のsubベクトルと垂直なベクトル
	V2d					*norms;		//各ノードの法線みたいなもの*/
	ID3DXMesh			*x_mesh;	//描画用頂点バッファ
	V3d					ldir;		//ライトの向き
	D3DCOLOR			amb;		//環境光
	BOOL				isspecular;	//反射光を使うかどうか

	D3DMATERIAL8		*mat;		//マテリアル（配列）
	LPDIRECT3DTEXTURE8	*tex;		//テクスチャ（配列）

	BOOL				zenka;		//前科
};

