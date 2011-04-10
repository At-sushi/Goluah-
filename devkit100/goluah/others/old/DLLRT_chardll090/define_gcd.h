
/*====================================================================================

	Goluah Cell Data
    ~      ~    ~
	�e��\����/�萔��`

	ver0.90����ǂ̃o�[�W������GCD�t�@�C�����ǂݍ��܂ꂽ�o�b�t�@�����f���邽�߂ɁA
	���[�h���ɃZ����0�Ԃ�cell[0]->flag�Ƀo�[�W�����ԍ� (0.90��������900) �𖄂ߍ��ނ��Ƃɂ����B

======================================================================================*/
#pragma once

#include <d3d8.h>
//#define LPDIRECT3DTEXTURE8 LPVOID//directx sdk ���Ȃ��ꍇ�͂�������`

#define GCD_VERSION			(900)
#define GCDMAX_RECTANGLES	(1024)
#define GCDMAX_CELLS		(1024)
#define GCDMAX_IMAGES		(12)
#define GCD_CELLGROUPNUM	(8)
#define GCD_MAXDEFINELENGTH	(64)

struct GCD_RECT
{
	DWORD bmpno;
	RECT r;
	int center_x,center_y;
};

struct GCD_CELL
{
	DWORD cdr;
	int dx;
	int dy;
	DWORD flag;
	float magx;
	float magy;
	int rot;

	float red_ratio;
	float green_ratio;
	float blue_ratio;
	float alpha_ratio;
};

#define GCDCELL_REVERSE_X		0x00000001
#define GCDCELL_REVERSE_Y		0x00000002
#define GCDCELL_BLEND_ADD		0x00000010//�`�掞�ɉ��Z�������s��

struct GCD_CELL2
{
	GCD_CELL cell[GCD_CELLGROUPNUM];
	int gcx;
	int gcy;
	DWORD flag;
};

#define GCDCELL2_LINK			0x00000001
#define GCDCELL2_DISABLE_ZR		0x00000002//z�o�b�t�@�ւ̏������݂��s��Ȃ�
#define GCDCELL2_DISABLE_ZT		0x00000004//z�e�X�g���s��Ȃ�

struct GCD_HANTEI
{
	RECT attack[3];
	RECT kas[3];
	RECT kurai[3];
};

struct GCD_FILEHEADER
{
	DWORD size;			//�Œ�@���̍\���̂̃T�C�Y�炵��.�Œ�ł�8�Ƃ���
	DWORD ver;			//�Œ�@�ǂ̃o�[�W�����̂��̂�
};

struct GCD_CELLNAMES
{
	char name[GCDMAX_CELLS][GCD_MAXDEFINELENGTH];
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

struct GCD_CELL2_070
{
	GCD_CELL_070 cell[8];
	int gcx;
	int gcy;
	DWORD flag;
};

//*********************************************************************
#define MYSUF_MAXNUMTEX	(16)

struct MYSURFACE
{
	BOOL valid;
	DWORD xsufsize[MYSUF_MAXNUMTEX];//�e�N�X�`���̕��̔z��
	DWORD ysufsize[MYSUF_MAXNUMTEX];//�e�N�X�`���̍����̔z��
	DWORD xsufindx[MYSUF_MAXNUMTEX];//MYSURFACE���ł�left���W
	DWORD ysufindx[MYSUF_MAXNUMTEX];//MYSURFACE���ł�top���W
	DWORD xsufnum;
	DWORD ysufnum;
	LPDIRECT3DTEXTURE8 *pTex;//pTex�̔z��
	float wg;
	float hg;
};

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
	float x,y,z;//���W
	DWORD color;
	float tu,tv;//�e�N�X�`�����W
};

#endif

