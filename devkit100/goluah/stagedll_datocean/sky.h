/*--------------------------------------------------------------
	‚»‚ç
----------------------------------------------------------------*/
#pragma once
#include "stage.h"

class CSky : public CStageObjectBase
{
public:
	void Init();
	void Update();
	void Draw();
	void Release();

	LPDIRECT3DTEXTURE8 tex;
	MYVERTEX3D sky_vb[8];

	UINT time;
};