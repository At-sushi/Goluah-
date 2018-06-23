
/*!
*	@file
*	@brief DX�`��n
*
*	���ۂɂ�DirectDraw�ł͂Ȃ�Direct3D�ł��B
*/
/*!
*	@defgroup DirectX
*	@brief DirectX
*/
#pragma once

#ifdef GCD_EDITER
#include <d3d8.h>
#include <d3dx8.h>
#endif
#include "define_gcd.h"	//GCD�t�H�[�}�b�g���K�v

#define MAXNUMGOLUAHTEXTURES (1024)		//!<�Ǝ��`���U�e�N�X�`���[�ő�Ǘ���
#define MINIMUM_TEXSIZE	(64)			//!<�ŏ��e�N�X�`���[�T�C�Y�B�R���ȉ��̃T�C�Y�̃e�N�X�`���������Ȃ��悤�Ȋ��͒m���B

#define AKIDX_FONTNUM	(4)

/*!
*	@ingroup DirectX
*	@brief �摜�r�b�g�ϊ������p�\����
*/
struct MYPALLET
{
	BYTE blue;
	BYTE green;
	BYTE red;
	BYTE alpha;
};

//!CDirectDraw�N���X�̏�ԁH
enum CDIRECTDRAW_STATE
{
	CDDSTATE_NOINITIALIZE,		//!<�܂�����������Ă��Ȃ�
	CDDSTATE_READY,				//!<�������������Ă���
	CDDSTATE_ERROR				//!<�G���[���������Ă���B���p�ł��Ȃ��B
};

/*!
*	@ingroup DirectX
*	@brief �`��Ǘ��N���X�B
*	
*	���݂�DirectX8�ȏ��Direct3D���g�p���邪�A�ȑO��DirectDraw���g���Ă����̂ł���Ȗ��O�B
*	�R���������Ê��N���X�B
*
*	2D�`��F
*	�C�ӃT�C�Y�̃r�b�g�}�b�v���ő�e�N�X�`���T�C�Y�ŕ��������Ǝ��`���̃j�Z�T�[�t�F�C�X��
*	�g�p���邱�Ƃɂ���āADirectDraw�̂Ƃ��Ɠ����悤�Ȋ����ŕ`����s�����Ƃ��ł���B
*	�i2�̗ݏ�ȊO�̃T�C�Y�̃r�b�g�}�b�v���g���Ă��{�P���肵�Ȃ��j
*
*	3D�`��F
*	Direct3DDevice�I�u�W�F�N�g��n�����炠�Ƃ�DX��API�����ł������ĂȂ�Ƃ����Ă�����H
*/
class CDirectDraw
{
public:
	CDirectDraw();
	~CDirectDraw(){Destroy();}

	//���������Ɣj��
	BOOL Initialize(HWND hwnd,BOOL win);
	void Destroy();

	//���`��̊J�n�ƏI��
	void StartDraw(BOOL erbs);//�`����J�n����O�ɂ��̊֐����R�[������K�v������
	void EndDraw();//�`����I�����AFLIP������s��

	//���t�@�C�����[�h�E�A�����[�h�֘A
	//256�F�̃r�b�g�}�b�v����Ǝ��`���T�[�t�F�C�X���쐬
	MYSURFACE* CreateSurfaceFrom256BMP(char *filename,char *pallet=NULL,BOOL dmy=TRUE);
	void RelSurface(MYSURFACE *s);//�e�N�X�`���J��

	//!�e��`���̉摜��ǂݍ��ށBPNG�ƍ��킹�邽�߂ɒǉ��B
	MYSURFACE* CreateSurfaceFrom256Image(char *filename,char *pallet=NULL,BOOL dmy=TRUE);

	//��2D��{�`��
	//�Ǝ��`���T�[�t�F�C�X�]���B�]���ʒu��3D��ԏ�Ŏw��ł���
	void MyBlt3D(MYSURFACE *dds,RECT src,MYRECT3D dst,DWORD flag=0,DWORD color=0xFFFFFFFF);
	//�͈̓`�F�b�N�]��
	//DirectDraw�̂Ƃ��̃R�[�h�Ƃ̌݊��̂��߂ɍ쐬���ꂽ�悤�Ȃ�������
	void CheckBlt(MYSURFACE *dds,int x,int y,RECT r,
		BOOL revx=FALSE,BOOL revy=FALSE,DWORD flag=0,float z=0.0f,DWORD color=0xFFFFFFFF, BOOL drawShadow = FALSE);
	//���̊ȈՔ�
	void CheckBlt2(MYSURFACE *dds,int x,int y,RECT r,
		double magx=1.0,double magy=1.0,BOOL revx=FALSE,BOOL revy=FALSE,DWORD flag=0,float z=0.0f,DWORD color=0xFFFFFFFF, BOOL drawShadow = FALSE);

	//���t�����g�o�b�t�@�̃R�s�[
	//���݂̃t�����g�o�b�t�@�̓��e���R�s�[�����e�N�X�`���[�𐶐����ĕԂ��B
	//D3D�̃w���v�ɂ��ƁA�����R�X�g�̓f�J�C�炵��
	#ifndef GCD_EDITER
	LPDIRECT3DTEXTURE8 GetFrontBufferCopy();
	DWORD* GetFrontBufferCopyRaw(UINT *wdt,UINT *hgt);
	#endif

	//��Goluah�����`��֐�
	//GCD�`���Z���`��
	void CellDraw(MYSURFACE **psuf,LPVOID pcdat,LPVOID prdat,DWORD cn,
		int x,int y,float z,int rot,BOOL revx=FALSE,BOOL revy=FALSE,DWORD color=0xFFFFFFFF,float magx=1.0f,float magy=1.0f, BOOL shadowed=FALSE);
	void CellDraw090(MYSURFACE **psuf,LPVOID pcdat,LPVOID prdat,DWORD cn,
		int x,int y,float z,int rot,BOOL revx=FALSE,BOOL revy=FALSE,DWORD color=0xFFFFFFFF,float magx=1.0f,float magy=1.0f, BOOL shadowed=FALSE);
	void CellDraw070(MYSURFACE **psuf,LPVOID pcdat,LPVOID prdat,DWORD cn,
		int x,int y,float z,int rot,BOOL revx=FALSE,BOOL revy=FALSE,DWORD color=0xFFFFFFFF,float magx=1.0f,float magy=1.0f, BOOL shadowed=FALSE);
#ifdef GCD_EDITER
	//�w���`���������ĕ`��
	void CellDrawED(MYSURFACE **psuf,LPVOID pcdat,LPVOID prdat,DWORD cn,
		int x,int y,float z,int rot,BOOL revx,BOOL revy,DWORD color,float magx,float magy,
		UINT ed_rect,					//�ҏW���̋�`�ԍ�
		BOOL color_modulat,				//�����\�����s��
		BOOL is_previous_cell=FALSE,	//*�����̂ڂ��ĕ`�悳�ꂽ�Z�����ǂ���
		BOOL is_next_cell=FALSE,		//*"��"�����ŕ`�悳�ꂽ�Z�����ǂ���
		BOOL is_edit_cell=TRUE);		//*���ݕҏW���イ�̃Z�����ǂ���);
#endif

	//�����蔻��`��
	void HanteiDraw(LPVOID pcdat,LPVOID phdat,DWORD cn,BOOL b_atr,BOOL b_kas,BOOL b_atk,
		int x,int y,float z,int rot,BOOL revx=FALSE,BOOL revy=FALSE,float magx=1.0f,float magy=1.0f);
	//��`�`��
	void HRectDraw(LPVOID cdat,LPVOID phdat,DWORD cn,
		int x,int y,float z,int rot,BOOL revx=FALSE,BOOL revy=FALSE,float magx=1.0f,float magy=1.0f);
	//�d�S�`��
	void GCenterDraw(LPVOID cdat,DWORD cn,
		int x,int y,float z,int rot,BOOL revx=FALSE,BOOL revy=FALSE,float magx=1.0f,float magy=1.0f);

	//�����̑��`��
	//���C���`��
	void DrawLine(int sx,int sy,int gx,int gy,DWORD col);
	//�o�b�N�o�b�t�@�̃N���A
	void ClearBackBuffer();
	//�~�̕`��
	void DrawCircle(int x,int y,int rad,int w,
		float z=0,DWORD color=0xFF55AAFF,BOOL toumei=FALSE,float rot=0,BOOL rot_y=TRUE,BOOL hosei=TRUE);
	//�Ȃɂ���H
	void Draw3DText();
	//�e�L�X�g�`��
	void DrawBlueText(RECT& r,char *text,int len,DWORD method,DWORD size);
	void DrawRedText(RECT& r,char *text,int len,DWORD method,DWORD size);
	// �Ǝ��`��y�ʉ���
//	void MyBlt3DLite(MYSURFACE* dds, RECT& r, int x, int y, BOOL noAlpha);
	// �T�[�t�F�X�ɒ��ڏ�������
//	void Kakikomi(LPDIRECT3DSURFACE8 SouShin, RECT& r2, LPDIRECT3DSURFACE8 BackBuffer, POINT& point, BOOL revx);

	//���֗��֐��H
	void SetAlphaMode(DWORD alphamode);//���u�����h�̃��[�h��ݒ�
	void EnableZ(BOOL test=TRUE,BOOL write=TRUE);//Z�e�X�g�E���C�g�̗L���E������ݒ�

	//�����W�ϊ��}�g���N�X�֘A
	//for directx8
	void ResetTransformMatrix();//view & projection
	void SetTransform(BOOL b);
	void SetWorldMatrix(D3DXMATRIX *pmat);
	//world
	D3DXMATRIXA16 SetParentMatrix(D3DXMATRIX& mat,BOOL root=TRUE,BOOL insert=FALSE);
	D3DXMATRIXA16 ResetParentMatrix();
	void SetParentMatrix2(D3DXMATRIX *mat,BOOL root,D3DXMATRIX *matprv);

	//���f�o�b�O�p�֐�
	//D3D�G���[�̃g���[�X
	void TraceCreateDeviceError(HRESULT ret,HWND hwnd);

	//! �E�B���h�E�T�C�Y�ύX����
	void OnWindowResized(int width, int height);

	//! �X�N���[�����[�h�ύX
	BOOL ChangeScreenMode(BOOL win);
	void ReduceColor(DWORD alpha, bool isShadow = false);//!<��ʂ��Â�����G�t�F�N�g�A�������B
	bool StencilEnable()	{ return stencil_enable; }
	void UpdateD3DPP()		{ ResetDirectDraw(); }

private:
	//���r�b�g�}�b�v���[�h�������֐�
	//�e�N�X�`���[�t�H�[�}�b�g�ϊ��֐�
	DWORD CopyOne_R5G6B5(MYPALLET src,PBYTE dst);
	DWORD CopyOne_A1R5G5B5(MYPALLET src,PBYTE dst);
	DWORD CopyOne_A4R4G4B4(MYPALLET src,PBYTE dst);
	DWORD CopyOne_A8R3G3B2(MYPALLET src,PBYTE dst);
	DWORD CopyOne_A8R8G8B8(MYPALLET src,PBYTE dst);
	//�r�b�g�}�b�v���[�h���ɕ����e�N�X�`�������v�Z
	BOOL AssignTextureDiv(MYSURFACE *pmsf,DWORD width,DWORD height);
	//256�F�r�b�g�}�b�v�̃C���[�W���������[�h����
	BOOL Load256Bitmapbits(MYPALLET **bits,DWORD *width,DWORD *height,char *bmpfilename,char *palfilename=NULL);
	BOOL Load256PNGbits(MYPALLET **pbits,DWORD *width,DWORD *height,char *pngfilename,char *palfilename);//PNG��
	BOOL LoadJPEGbits(MYPALLET **pbits,DWORD *width,DWORD *height,char *jpegfilename);//JPEG�Łi�t���J���[�j
	//256�r�b�g�}�b�v�̃p���b�g���������[�h����
	BOOL GetPallet(char *filename,MYPALLET *pal);//256�������t�@�C������A�p���b�g��������o
	BOOL GetPalletPNG(char *filename,MYPALLET *pal);//256PNG�t�@�C������A�p���b�g��������o

	//���֗��֐��H
	//�o�b�N�o�b�t�@����e�N�X�`���ւ̓]��
	//�E�E�E����Ȋ֐�����Ă�����?
	//�c�Ⴄ�Ǝv���A�r�b�g�}�b�v���r�b�g���ƂɃe�N�X�`���ɓ\��t���Ă�݂����ł��B
	BOOL CopyBB2TS(MYPALLET *pbb,DWORD bbpitch,DWORD offset_x,DWORD offset_y,LPDIRECT3DTEXTURE8 ptex,DWORD damex=0,DWORD damey=0);

	//�������j���������֐�
	//Direct3D������������B�֐����̖͐̂��c���ۂ�
	BOOL InitDirectDraw(HWND hwnd,BOOL win);
	//�e�L�X�g�`��p�̃t�H���g�𐶐�����
	LPD3DXFONT CreateMyFont(DWORD h);
	//Direct3D��n���֐�
	void CleanDirectDraw();
	//�����_�����O�X�e�[�g�ݒ�
	void InitStates();
	//! ���Z�b�g�p�ADirectDraw�f�o�C�X���������Ƃ��ɌĂяo����܂��B
	void ResetDirectDraw();

	//���f�[�^�����o
public:
	CDIRECTDRAW_STATE state;			//!<�N���X�̏��
	LPDIRECT3D8 dd;						//!<[�d�v]Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE8 d3ddev;			//!<[�d�v]Direct3D�f�o�C�X�I�u�W�F�N�g
	D3DPRESENT_PARAMETERS d3dpp;		//!<�Ȃ񂾂����H����B
	D3DDEVTYPE devtypenow;				//!<���݂̃f�o�C�X�^�C�v�BHAL�Ƃ�SW�Ƃ��A����������
	DWORD maxtexturewidth,
					maxtextureheight;	//!<�f�o�C�X�ŋK�肳���e�N�X�`���̍ő�T�C�Y
	D3DFORMAT texformat,dispformat;		//!<�g�p����摜�t�H�[�}�b�g

	D3DXMATRIXA16 matparent;			//!<2D�`��֐��}�g���b�N�X
	MYSURFACE ms[MAXNUMGOLUAHTEXTURES];	//!<�Ǝ��`���T�[�t�F�C�X�Ǘ�

	float yurey;						//!<��ʗh��G�t�F�N�g�Ɏg�p����Y�����̂��炵��
	float center_x;						//2!<D�`����W�n���SX
	float camera_x,camera_y,camera_z;	//!<�J�����̈ʒu
	float camera_zurax,camera_zuray;	//!<�J�������炵�H�悭�������
	DWORD clearbgcolor;					//!<�o�b�N�o�b�t�@�N���A���Ɏg�p����h��Ԃ��F
	LPD3DXFONT lpFont[AKIDX_FONTNUM];	//!<�e�L�X�g�`��Ɏg�p����t�H���g
	//LPD3DXSPRITE pSprite;				//!<�X�v���C�g(߄t�)�ϰ
	LPDIRECT3DVERTEXBUFFER8 pMyVertex;	//!<���_�o�b�t�@�A�ꉞ����Ă݂��B
private:
	D3DXPLANE d3dxplane_x,d3dxplane_y;	//!<�}�g���N�X��]�Ɏg�p���� X,Y ����
	BOOL m_is_win;						//!<�E�B���h�E���[�h�ł̋N�����ǂ���
	UINT m_desktop_width;				//!<�E�B���h�E���[�h�̏ꍇ�A�f�X�N�g�b�v�̕�
	UINT m_desktop_height;				//!<�E�B���h�E���[�h�̏ꍇ�A�f�X�N�g�b�v�̍���
	bool stencil_enable;				//!<�X�e���V�����g���邩�ǂ���
};



//blt�n�֐��̃t���O
#define CKBLT_YUREY			(0x00000001)//!<�h���^����

// Flexible Vertex Format -----------------------------------------------------
// �g�p����Direct3D���_�t�H�[�}�b�g
#ifndef FVF_3DVERTEX
#define FVF_3DVERTEX	(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

/*!
*	@ingroup DirectX
*	@brief D3D�`��Ŏg�p���钸�_�t�H�[�}�b�g
*/
struct MYVERTEX3D
{
	float x,y,z;//!< ���W
	DWORD color;//!< ���_�F
	float tu,tv;//!< �e�N�X�`�����W
};
#endif//FVF_3DVERTEX


