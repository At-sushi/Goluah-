
/*!
*	@file
*	@brief GCD(Goluah Cell Data) �֌W �e��\����/�萔��`
*
*	Goluah!!�{�̂ƁA�S�ẴL�����N�^�[�N���X����Q�Ƃ����B
*
*	ver0.90����ǂ̃o�[�W������GCD�t�@�C�����ǂݍ��܂ꂽ�o�b�t�@�����f���邽�߂ɁA
*	���[�h���ɃZ����0�Ԃ�cell[0]->flag�Ƀo�[�W�����ԍ� (0.90��������900) �𖄂ߍ��ނ��Ƃɂ����B
*
*	Direct3D�𒼐ڎg�p����ꍇ�͂��̃w�b�_�̑O�� d3d8.h , d3dx8.h ��include���Ȃ��ƁA
*	typedef�������ăr���h���ʂ�Ȃ��Ȃ�̂Œ��ӂ��邱�ƁB
*/
#pragma once

//DirectX SDK ���Ȃ����ւ̑Ή�
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

//! GCD�؂����`�\����
struct GCD_RECT
{
	DWORD bmpno;			//!< �r�b�g�}�b�v�ԍ�
	RECT r;					//!< �؂����`�̈�
	int center_x,center_y;	//!< ��`�d�S
};

//! GCD�Z��(1�v�f)
struct GCD_CELL
{
	DWORD cdr;				//!< �؂����`�ԍ�
	int dx;					//!< X�I�t�Z�b�g�l
	int dy;					//!< Y�I�t�Z�b�g�l
	DWORD flag;				//!< �t���O
	float magx;				//!< X�g�嗦
	float magy;				//!< Y�g�嗦
	int rot;				//!< ��]�e

	float red_ratio;		//!< �ԕ`�旦(0�`1�A�ʏ�1)
	float green_ratio;		//!< �Ε`�旦(0�`1�A�ʏ�1)
	float blue_ratio;		//!< �`�旦(0�`1�A�ʏ�1)
	float alpha_ratio;		//!< ���`�旦(0�`1�A�ʏ�1)
};

#define GCDCELL_REVERSE_X		0x00000001//!< GCD�`�掞��X���]���s��
#define GCDCELL_REVERSE_Y		0x00000002//!< GCD�`�掞��Y���]���s��
#define GCDCELL_BLEND_ADD		0x00000010//!< GCD�`�掞�ɉ��Z�������s��

//! GCD�Z�� (GCD_CELL�̃Z�b�g)
struct GCD_CELL2
{
	GCD_CELL cell[GCD_CELLGROUPNUM];	//!< �Z���v�f
	int gcx;							//!< X�d�S�ʒu
	int gcy;							//!< y�d�S�ʒu
	DWORD flag;							//!< �t���O
};

#define GCDCELL2_LINK			0x00000001//!< �z�񒆂Ŏ��̃C���f�b�N�X�̃Z���̕`����s��
#define GCDCELL2_DISABLE_ZW		0x00000002//!< z�o�b�t�@�ւ̏������݂��s��Ȃ�
#define GCDCELL2_DISABLE_ZT		0x00000004//!< z�e�X�g���s��Ȃ�
#define GCDCELL2_SCA_GCENTER	0x00000010//!< �X�P�[�����S���d�S�̃|�C���g�Ƃ���i�f�t�H���g�͑����j
#define GCDCELL2_ROT_BASEPOINT	0x00000020//!< ��]���S�𑫌��̃|�C���g�Ƃ���i�f�t�H���g�͏d�S�j

//! GCD�����蔻���`
struct GCD_HANTEI
{
	RECT attack[3];
	RECT kas[3];
	RECT kurai[3];
};

//! GCD�t�@�C���w�b�_
struct GCD_FILEHEADER
{
	DWORD size;			//!< �Œ�@���̍\���̂̃T�C�Y�炵��.�Œ�ł�8�Ƃ���
	DWORD ver;			//!< �Œ�@�ǂ̃o�[�W�����̂��̂�
};

//! GCD��`���\����
struct GCD_CELLNAMES
{
	TCHAR name[GCDMAX_CELLS][GCD_MAXDEFINELENGTH];
};

//! ���k�`���t�@�C���w�b�_
struct GCD_COMP_FILEHEADER
{
	BYTE signature[4];	//!< �Œ�@�t�@�C�����ʗp�̃V�O�l�`��
	DWORD ver;			//!< �Œ�@�ǂ̃o�[�W�����̂��̂�
	DWORD flags;		//!< �t���O
};

#define GCDFILE_COMP_HAVE_CELLNAMES	0x00000001 //!< �Z����`�����܂�
#define GCDFILE_COMP_USING_FILTER	0x00000002 //!< �t�B���^�[���g�p�i�܂��������j

//! ���k�`���f�[�^�w�b�_
struct GCD_COMP_DATAHEADER
{
	DWORD DataSize;		//!< �f�[�^�i���k��j�̃T�C�Y
	DWORD CRCCheckSum;	//!< ���f�[�^��CRC�l
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

//!���o�[�W����CELL�\����(1�v�f)
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

//!���o�[�W����CELL�\����
struct GCD_CELL2_070
{
	GCD_CELL_070 cell[8];
	int gcx;
	int gcy;
	DWORD flag;
};

//*********************************************************************
#define MYSUF_MAXNUMTEX	(16)		//!< MYSURFACE�e�N�X�`���������E�l

/*!
*	@brief �Ǝ��`���C���`�L�T�[�t�F�C�X
*
*	2^n �����������ĔC�ӂ̑傫���̃r�b�g�}�b�v�����[�h���邽�߂ɁA
*	�K�؂ȃT�C�Y�ŉ摜�𕪊����ĕ����̃e�N�X�`���Ƃ��ĕێ�����B
*	�`���CDirectDraw�̐�p�֐����g���čs���B
*/
struct MYSURFACE
{
	BOOL valid;
	DWORD xsufsize[MYSUF_MAXNUMTEX];//!< �e�N�X�`���̕��̔z��
	DWORD ysufsize[MYSUF_MAXNUMTEX];//!< �e�N�X�`���̍����̔z��
	DWORD xsufindx[MYSUF_MAXNUMTEX];//!< MYSURFACE���ł�left���W
	DWORD ysufindx[MYSUF_MAXNUMTEX];//!< MYSURFACE���ł�top���W
	DWORD xsufnum;					//!< X�����e�N�X�`��������
	DWORD ysufnum;					//!< Y�����e�N�X�`��������
	LPDIRECT3DTEXTURE9 *pTex;		//!< �e�N�X�`���z��
	float wg;						//!< �r�b�g�}�b�v�̕�
	float hg;						//!< �r�b�g�}�b�v�̍���
};

//! 3�����I�`���`�w��p�\����
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
	float x,y,z;//!< ���W
	DWORD color;//!< ���_�F
	float tu,tv;//!< �e�N�X�`�����W
};
#endif

/*@}*/