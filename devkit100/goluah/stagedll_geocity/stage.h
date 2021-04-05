
#pragma once
#include <windows.h>
#include <stdio.h>
#include <d3d8.h>
#include <d3dx8.h>
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

	//月、そして街
	LPDIRECT3DTEXTURE8 ptex_moon;
	LPDIRECT3DVERTEXBUFFER8 vb_moon;
	LPDIRECT3DTEXTURE8 ptex_city;
	LPDIRECT3DVERTEXBUFFER8 vb_city;

	//道路
	LPDIRECT3DTEXTURE8 ptex_road;
	LPDIRECT3DVERTEXBUFFER8 vb_road[NUMDIV_ROADY];
	LPDIRECT3DTEXTURE8 ptex_gurd;
	//ガードレール
	LPDIRECT3DVERTEXBUFFER8 vb_gurd;
	LPDIRECT3DVERTEXBUFFER8 vb_gurdpoll;//柱
	
	//交差点地面
	LPDIRECT3DTEXTURE8 ptex_jct;
	LPDIRECT3DVERTEXBUFFER8 vb_jct[NUMDIV_JCTY];

	LPDIRECT3DVERTEXBUFFER8 vb_ground;//地面黒く塗りつぶし

	//看板
	LPDIRECT3DTEXTURE8 ptex_brd1a,ptex_brd1b;
	LPDIRECT3DVERTEXBUFFER8 vb_brd1a,vb_brd1b;
	LPDIRECT3DTEXTURE8 ptex_tlight;
	MYVERTEX3D vb_tlight[4];

	//看板（裏向き）
	LPDIRECT3DTEXTURE8 ptex_brd2;
	MYVERTEX3D vb_brd2a[4],vb_brd2b[4];

	//街灯
	LPDIRECT3DTEXTURE8 ptex_light;
	MYVERTEX3D vb_light[NUM_LIGHT][4];

	//ガードレール（手前側）
	MYVERTEX3D vb_maegurd[4],vb_maegurdpoll[4];

	//★
	MYVERTEX3D vb_stars[NUM_STARS];

	//空（？）
	MYVERTEX3D vb_sky[4];
};

#ifndef RELEASE
#define RELEASE(a) if(a!=NULL){a->Release();a=NULL;}
#endif

