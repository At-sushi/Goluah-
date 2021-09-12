#pragma once

#include <windows.h>
#include <stdio.h>
#include <d3d8.h>
#include <d3dx8.h>
#include "gobjbase.h"

#include "aki3d.h"

#define MOYA_NUM	32

/*!
*	@brief ステージクラス
*/
class CStage : public CStageBase
{
public:
	CStage(SDI_STAGEINFO2 *info);
	~CStage();
	void InitializeStage();
protected:
	DWORD Action();
	DWORD DrawBack();

	void InitVrtx();
	void LoadTextures();

	LPDIRECT3DDEVICE8 d3ddev;

	MYSURFACE *pms_back;
	LPDIRECT3DTEXTURE8 ptex_jimen;
	LPDIRECT3DVERTEXBUFFER8 vb_gnd;

	RECT src;
	MYRECT3D dst;

	CParticle *moya;
	struct MoyaInfo
	{
		float x;
		float z;
		float width;
		float vx;
		float axlen;

		void Init();
		void Update();
	};
	MoyaInfo moyainfo[MOYA_NUM];
};

#ifndef RELEASE
#define RELEASE(p)	if(p){p->Release();p=NULL;}
#endif

