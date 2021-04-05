
/*===============================================================

	システムエフェクトオブジェクト管理クラス

=================================================================*/
#pragma once

#include "define_gcd.h"
#include "gobject.h"

class CBattleTaskBase;

class CEffectList;
class CSystemFxBase;

/*---------------------------------------------------------------
	システムエフェクト用テクスチャー番号定義
-----------------------------------------------------------------*/
#define SYSFXTEX_FIRE			0
#define SYSFXTEX_KO_K			1
#define SYSFXTEX_KO_O			2

#define MAXNUM_SYSFXTEXTURES	3

/*!
*	@brief エフェクト生成・描画クラス
*	@ingroup Battle
*
*	本体EXEからの分離を画策中
*/
class CEffectList
{
public:
	CEffectList();
	~CEffectList(){Destroy();}

	void Initialize();
	void Destroy();

	void AddEffect(DWORD type,int prm1,int prm2,int prm3=0);			//!< 指定IDのエフェクト描画オブジェクトを生成
	static DWORD EfctObjectMessage(DWORD msg,LPVOID dat,DWORD prm1);	//!< エフェクト描画オブジェクト共通メッセ処理関数
	LPDIRECT3DTEXTURE8 GetTexture(UINT no){return textures[no];}		//!< エフェクト共通テクスチャー取得

protected:
	CBattleTaskBase *battleTask;
	static CEffectList *pinst;
	
	//!エフェクト用共通テクスチャー
	typedef std::vector<LPDIRECT3DTEXTURE8> TextureList;
	TextureList textures;
	void LoadTextures();
	void UnloadTextures();
	
friend class CSystemFxBase;
};


/*!
*	@brief エフェクト描画オブジェクト・基礎定義
*	@ingroup Battle
*/
class CSystemFxBase
{
friend class CEffectList;
public:
	CSystemFxBase();
	virtual ~CSystemFxBase();
	
	virtual BOOL Create(int prm1,int prm2,int prm3){return TRUE;}
	virtual void Update(){}
	virtual BOOL Draw(){return FALSE;}
	virtual BOOL DrawF(){return FALSE;}
	virtual void Destroy(){}

	GOBJECT* pdat;

protected:
	void AddEffect(DWORD type,int prm1,int prm2,int prm3);
	void End();
	LPDIRECT3DTEXTURE8 GetTexture(UINT no){return pefctlist->GetTexture(no);}

	CBattleTaskBase *battleTask;
	CEffectList *pefctlist;
};

#ifndef PI
#define PI (3.1416f)
#endif
