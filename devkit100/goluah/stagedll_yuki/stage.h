#pragma once

#include <windows.h>
#include <stdio.h>
#include <d3d8.h>
#include <d3dx8.h>
#include "gobjbase.h"
#include "aki3d.h"

#define MAXNUM_SNOW				(1024)
#define MAXNUM_SNOW_DRAW		(512)
#define MAXNUM_SNOW_FRONT		(512)
#define MAXNUM_SNOW_FRONT_DRAW	(256)

/*!
*	@brief ステージクラス
*/
class CStage : public CStageBase
{
public:
	CStage(SDI_STAGEINFO2 *info);
	~CStage();
	void InitializeStage();
	DWORD Action();
protected:
	DWORD DrawBack();
	DWORD DrawFront();

	MYSURFACE* bitmap;

	struct SNOWPARAMS
	{
		float isou;
		float amp;
		float x;
		float y;
		float z;
		float vx;
		float vy;
		float base_x;
		float width;

		void Init(BOOL front);
		void Update(BOOL front);
	};
	SNOWPARAMS snow[MAXNUM_SNOW];
	SNOWPARAMS snow_front[MAXNUM_SNOW_FRONT];
	CParticle* particle;
	CParticle* particle_front;
};

