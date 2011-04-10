
/*===============================================================

	�V�X�e���G�t�F�N�g�I�u�W�F�N�g�Ǘ��N���X

=================================================================*/
#pragma once

#include "define_gcd.h"
#include "gobject.h"

class CBattleTaskBase;

class CEffectList;
class CSystemFxBase;

/*---------------------------------------------------------------
	�V�X�e���G�t�F�N�g�p�e�N�X�`���[�ԍ���`
-----------------------------------------------------------------*/
#define SYSFXTEX_FIRE			0
#define SYSFXTEX_KO_K			1
#define SYSFXTEX_KO_O			2

#define MAXNUM_SYSFXTEXTURES	3

/*!
*	@brief �G�t�F�N�g�����E�`��N���X
*	@ingroup Battle
*
*	�{��EXE����̕����������
*/
class CEffectList
{
public:
	CEffectList();
	~CEffectList(){Destroy();}

	void Initialize();
	void Destroy();

	void AddEffect(DWORD type,int prm1,int prm2,int prm3=0);			//!< �w��ID�̃G�t�F�N�g�`��I�u�W�F�N�g�𐶐�
	static DWORD EfctObjectMessage(DWORD msg,LPVOID dat,DWORD prm1);	//!< �G�t�F�N�g�`��I�u�W�F�N�g���ʃ��b�Z�����֐�
	LPDIRECT3DTEXTURE8 GetTexture(UINT no){return textures[no];}		//!< �G�t�F�N�g���ʃe�N�X�`���[�擾

protected:
	CBattleTaskBase *battleTask;
	static CEffectList *pinst;
	
	//!�G�t�F�N�g�p���ʃe�N�X�`���[
	typedef std::vector<LPDIRECT3DTEXTURE8> TextureList;
	TextureList textures;
	void LoadTextures();
	void UnloadTextures();
	
friend class CSystemFxBase;
};


/*!
*	@brief �G�t�F�N�g�`��I�u�W�F�N�g�E��b��`
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