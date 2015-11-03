#pragma once

#include <windows.h>
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "gobjbase.h"

//sphere
#define NUM_CIRCLE1 20
#define NUM_CIRCLE2 20

//floor
#define FLOORDIV_X	30
#define FLOORDIV_Z	30


/*!
*	@brief �X�e�[�W�N���X
*/

class CStage : public CStageBase
{
public:
	CStage(SDI_STAGEINFO2 *info);
	~CStage();
	void InitializeStage();
protected:
	DWORD DrawBack();

	void InitVrtx();
	void LoadTextures();

	void DrawJimen();//�n��
	void DrawBuilding1();//�r���P
	void DrawBuilding2();//�r���Q
	void DrawBackGround();//�w�i

	LPDIRECT3DDEVICE9 d3ddev;

	LPDIRECT3DTEXTURE9 ptex_jimen;
	LPDIRECT3DTEXTURE9 ptex_fens;
	MYVERTEX3D vb_jimen[4];
	MYVERTEX3D vb_fens[4];
	MYVERTEX3D vb_fens2[2][4];

	LPDIRECT3DTEXTURE9 ptex_buil1a;
	MYVERTEX3D vb_buil1a[2][4];
	MYVERTEX3D vb_buil1b[4];//��������
	LPDIRECT3DTEXTURE9 ptex_brdura;
	MYVERTEX3D vb_buil1c[4];//�Ŕ�
	LPDIRECT3DVERTEXBUFFER9 vb_buil1d[10];//�Ŕx��

	LPDIRECT3DTEXTURE9 ptex_buil2a;
	MYVERTEX3D vb_buil2a[4];
	MYVERTEX3D vb_buil2b[4];

	LPDIRECT3DTEXTURE9 ptex_sky;
	MYVERTEX3D vb_sky[4];
	LPDIRECT3DTEXTURE9 ptex_bg1;
	MYVERTEX3D vb_bg1[4];
	LPDIRECT3DTEXTURE9 ptex_bg2;
	MYVERTEX3D vb_bg2[4];
};

#ifndef RELEASE
#define RELEASE(a) if(a!=NULL){a->Release();a=NULL;}
#endif
