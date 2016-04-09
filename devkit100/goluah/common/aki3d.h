
/*!
*	@file
*	@brief aki3d.lib ��`
*
*	aki3d.lib �̋@�\��`�B
*	Goluah!!�{�́A�L�����N�^�[/�X�e�[�WDLL(aki���̂���)����Q�Ƃ����
*/
#pragma once
#define _aki3d_h_

#include <vector>
#include "d3dx9.h"

#define FAR_CLIP	(50.0f)
#define NEAR_CLIP	( 0.1f)

#if defined(UNICODE) || defined(_UNICODE)
#define __T(x) L ## x
#else
#define __T(x) x
#endif
#define _T(x) __T(x)

/*----------------------------------------------------------
	�g�p����Direct3D���_�t�H�[�}�b�g�@��`
------------------------------------------------------------*/
#ifndef FVF_3DVERTEX
#define FVF_3DVERTEX	(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
/*!
*	@brief �`��Ɏg�p���钸�_�\����
*
*	D3D8�̏_��Ȓ��_�t�H�[�}�b�g�Ƃ�������B
*	�V�F�[�_�[�ݒ�ł� FVF_3DVERTEX ���g�p����
*/
struct MYVERTEX3D
{
	float x,y,z;	//!< ���W
	DWORD color;	//!< ���_�J���[
	float tu,tv;	//!< �e�N�X�`�����W
};
#endif//FVF_3DVERTEX

/*---------------------------------------------------------
	�^��`
-----------------------------------------------------------*/
typedef D3DXVECTOR3 V3d;

/*!
*	@brief 2�����x�N�g���\����
*
*	�`��p�Ƃ������́A�b�\���̕����v�Z���Ŏg�p�H
*	�������֗��֐��A��
*/
struct V2d
{
	float x,y;

	void Normalize();				//!< ���K��
	void Verticalize();				//!< ������(������0�̏ꍇ�͓K���Ȍ����̐��K�x�N�g��)
	float Length();					//!< �����擾
	void SetLength(float len);		//!< �����ݒ�(�����͎c��)

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
*	@brief �F���\����
*
*	ARGB�ʌ`���ƁADWORD32�r�b�g�`���Ƃ̑��ݕϊ����\
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
	��{�x�N�g��
-----------------------------------------------------------*/
extern V3d Xaxis;
extern V3d Yaxis;
extern V3d Zaxis;
extern V3d Zero;

/*----------------------------------------------------------
	�G�t�F�N�g�T�[�r�X�N���X
------------------------------------------------------------*/

class CTristrip;
class CParticle;
class CFlatBoards;
class CMesh;

typedef void (*WarningReportFunctionPtr)(const char* war_str);

/*!
*	@brief 3D�`��T�[�r�X�N���X
*
*	���DLL������3D�`��@�\��񋟂���B�e�N�X�`���Ǘ������B
*	���̃N���X�̓��C�u�������� aki3d �Ƃ����O���[�o���ȃC���X�^���X�����݂���̂ŁA
*	������g�p���邱�ƁB
*
*	�擾�����e�N���X�̃C���X�^���X�͂��̃N���X�̔j�����Ɏ����I��delete�����̂ŁA
*	�擾����delete���Ȃ����ƁB(���������C���X�^���X�����X�g�Ŏ����Ă�̂�)
*	�C���X�^���X�̐����Ɣj�����p�ɂɍs����ꍇ(goluah�{��)�́ADestroy�`�n�̊֐��Ŕj�����s���B
*	���̂Ƃ��e�N�X�`���[���Q�ƃJ�E���g�ɂ�����炸�S�Ĕj�������B
*/
class Aki3d
{
public:
	Aki3d(){d3ddev=NULL;}

	//!�������B�S�Ă̑���̑O�ɍs������
	void Initialize(
		LPDIRECT3DDEVICE9 d3d_device,		//!< D3D�f�o�C�X�I�u�W�F�N�g
		const char*	tex_path,				//!< �e�N�X�`�����[�h�p�̃x�[�X�f�B���N�g��
		WarningReportFunctionPtr pwf=NULL	//!< �G���[���|�[�g�p�̊֐��|�C���^
		);
	~Aki3d(){Destroy();}

	CTristrip*		CreateTristripObject();		//!< CTristrip�N���X�C���X�^���X�擾�Bdelete���Ȃ�����
	CParticle*		CreateParticleObject();		//!< CParticle�N���X�C���X�^���X�擾�Bdelete���Ȃ�����
	CFlatBoards*	CreateFlatBoardsObject();	//!< CFlatBords�N���X�C���X�^���X�擾�Bdelete���Ȃ�����
	CMesh*			CreateMeshObject();			//!< CMesh�N���X�C���X�^���X�擾�Bdelete���Ȃ�����

	void DestroyTristripObject(CTristrip*);		//!< CTristrip�N���X�C���X�^���X�����j���B
	void DestroyParticleObject(CParticle*);		//!< CParticle�N���X�C���X�^���X�����j���B
	void DestroyFlatBoardsObject(CFlatBoards*);	//!< CFlatBords�N���X�C���X�^���X�����j���B
	void DestroyMeshObject(CMesh*);				//!< CMesh�N���X�C���X�^���X�����j���B

	/*!
	*	@brief �}�g���b�N�X����
	*	@param scale �X�P�[���B�}�C�i�X�l�Ŕ��]���s�����Ƃ��ł���
	*	@param rotation �x�N�g���̌�������]���Ƃ��āA���������W�A���P�ʂ̉�]�p�Ƃ��ĉ���
	*	@param trans ���s�ړ���
	*	@return �w��x�N�g�����琶���������W�ϊ��}�g���N�X
	*	
	*	���������ꂼ��NULL�̏ꍇ�ɊY�����鑀����s��Ȃ�.
	*	�[���x�N�g��������Ȃ��ł͂Ȃ����Ƃɒ��ӂ��邱��
	*	�i�Ⴆ�΃[���x�N�g���ɂ��X�P�[�����s���ƕ`�悳��Ȃ��Ȃ�j
	*/
	D3DXMATRIX& CreateMatrix( V3d* scale, V3d* rotation, V3d* trans );

public:
	static float RandomOne();		//!< 0�`1���������_��
	static float RandomOne2();		//!< -1�`1���������_��
	float zo_tri(float t,float mid=0.5f);

	//�����_�[�X�e�[�g�ݒ�
	void EnableZ(BOOL t=TRUE,BOOL w=TRUE);
	void SetBlend_Add();
	void SetBlend_Normal();
	void SetBlend_Nega();

	//�e�N�X�`���[
	LPDIRECT3DTEXTURE9 LoadTexture(const char *filename);
	void UnloadTexture(LPDIRECT3DTEXTURE9 ptex);

	LPDIRECT3DDEVICE9 GetD3DDev(){ return d3ddev; }

	void LogWarning(const char* fmt,...);

protected:
	void Destroy();

	LPDIRECT3DDEVICE9 d3ddev;		//D3D�f�o�C�X
	WarningReportFunctionPtr warning_report;
};

extern Aki3d aki3d;


/*!
*	@brief ���X�g���b�v�`��N���X
*
*	�A���������炩�Ȑ���`�����Ƃ��ł���B
*	Create�Ńm�[�h(��)�����w�肵�A�e�m�[�h�ɑ΂��Ĉʒu�Ƒ�����ݒ肷��ƁA
*	�������Ȃ����g���C�A���O���X�g���b�v���v�Z���ĕ`�悷��B
*
*	�C���X�^���X��Aki3d::CreateTristripObject �Ŏ擾����B
*	�j����Aki3d���s���̂ŁA�擾�����C���X�^���X��delete���Ȃ����ƁB
*
*	@sa Aki3d
*/
class CTristrip
{
public:
	virtual void Create(UINT num_node , const char* tex_filename) = 0;
	virtual void Destroy() = 0;
	virtual void Render(D3DMATRIX *ltm = NULL) = 0;

	virtual void SetWidth(UINT index,float value) = 0;
	virtual void SetPos(UINT index,V3d& value) = 0;
	virtual void SetColor(UINT index,ARGB value) = 0;
	virtual void SetColorDW(UINT index,DWORD value) = 0;

	virtual ~CTristrip(){}
};


/*!
*	@brief �Ԃԕ`��N���X
*
*	��{�I�ɁA�ʒu�Ƒ傫�����w�肵�Ċۂ�(�e�N�X�`���[�ɂ��)�_�X��`�悷��B
*	Axis��ݒ肷��ƁA���̕����ɗ���L�΂��đȉ~��ɂ��邱�Ƃ��ł���B
*
*	�C���X�^���X��Aki3d::CreateParticleObject �Ŏ擾����B
*	�j����Aki3d���s���̂ŁA�擾�����C���X�^���X��delete���Ȃ����ƁB
*
*	@sa Aki3d
*/
class CParticle
{
public:
	virtual void Create(UINT num_node , const char* tex_filename) = 0;
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
*	@brief ���b�V���`��N���X
*
*	�C���X�^���X��Aki3d::CreateMeshObject �Ŏ擾����B
*	�j����Aki3d���s���̂ŁA�擾�����C���X�^���X��delete���Ȃ����ƁB
*
*	@sa Aki3d
*/
class CMesh
{
public:
	virtual void Create(const char* x_filename) = 0;
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
*	@brief �t���b�g�{�[�h�`��N���X
*
*	CParticle�Ǝ��Ă��邪�A������͑傫���̎w��͂Ȃ��B
*	Axis1 �� Axis2 �Ŏw�肳�ꂽ�������l�p�`�v���~�e�B�u��`�悷��B
*
*	�C���X�^���X��Aki3d::CreateFlatBordObject �Ŏ擾����B
*	�j����Aki3d���s���̂ŁA�擾�����C���X�^���X��delete���Ȃ����ƁB
*
*	@sa Aki3d
*/
class CFlatBoards
{
public:
	virtual void Create(UINT num_node , const char* tex_filename) = 0;
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
	�}�N��
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

