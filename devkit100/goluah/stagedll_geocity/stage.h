
#pragma once
#include <windows.h>
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "gobjbase.h"


#define NUM_GURD 4

#define NUMDIV_ROADY 5
#define NUMDIV_ROADX 4

#define NUMDIV_JCTY 3
#define NUMDIV_JCTX 4

#define NUM_GURDPOLL	10
#define NUM_LIGHT		10

#define NUM_STARS		50

class CStage : public CStageBase
{
public:
	CStage(SDI_STAGEINFO2 *info);
	~CStage();
	void InitializeStage();

	void DrawStage();
	DWORD DrawBack(){DrawStage();return FALSE;}

private:
	void InitVrtx();
	void LoadTextures();
	void UnloadTextures();

	void DrawBacks();
	void DrawJct();
	void DrawRoad();

private:

	//���A�����ĊX
	LPDIRECT3DTEXTURE9 ptex_moon;
	LPDIRECT3DVERTEXBUFFER9 vb_moon;
	LPDIRECT3DTEXTURE9 ptex_city;
	LPDIRECT3DVERTEXBUFFER9 vb_city;

	//���H
	LPDIRECT3DTEXTURE9 ptex_road;
	LPDIRECT3DVERTEXBUFFER9 vb_road[NUMDIV_ROADY];
	LPDIRECT3DTEXTURE9 ptex_gurd;
	//�K�[�h���[��
	LPDIRECT3DVERTEXBUFFER9 vb_gurd;
	LPDIRECT3DVERTEXBUFFER9 vb_gurdpoll;//��
	
	//�����_�n��
	LPDIRECT3DTEXTURE9 ptex_jct;
	LPDIRECT3DVERTEXBUFFER9 vb_jct[NUMDIV_JCTY];

	LPDIRECT3DVERTEXBUFFER9 vb_ground;//�n�ʍ����h��Ԃ�

	//�Ŕ�
	LPDIRECT3DTEXTURE9 ptex_brd1a,ptex_brd1b;
	LPDIRECT3DVERTEXBUFFER9 vb_brd1a,vb_brd1b;
	LPDIRECT3DTEXTURE9 ptex_tlight;
	MYVERTEX3D vb_tlight[4];

	//�Ŕi�������j
	LPDIRECT3DTEXTURE9 ptex_brd2;
	MYVERTEX3D vb_brd2a[4],vb_brd2b[4];

	//�X��
	LPDIRECT3DTEXTURE9 ptex_light;
	MYVERTEX3D vb_light[NUM_LIGHT][4];

	//�K�[�h���[���i��O���j
	MYVERTEX3D vb_maegurd[4],vb_maegurdpoll[4];

	//��
	MYVERTEX3D vb_stars[NUM_STARS];

	//��i�H�j
	MYVERTEX3D vb_sky[4];
};

#ifndef RELEASE
#define RELEASE(a) if(a!=NULL){a->Release();a=NULL;}
#endif

