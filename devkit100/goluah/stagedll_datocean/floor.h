/*--------------------------------------------------------------
	‚ä‚©
----------------------------------------------------------------*/
#pragma once
#include "stage.h"

#define NUM_UNDERFLOOR	(32)

class CFloor : public CStageObjectBase
{
public:
	CFloor();
	void Init();
	void Update();
	void Draw();
	void Release();

	LPDIRECT3DTEXTURE8 tex;
	LPDIRECT3DVERTEXBUFFER8 pvb;
	LPDIRECT3DINDEXBUFFER8 pindex;

	struct UnderFloor
	{
		V2d pos;
		V2d vel;
		V2d acc;

		float rad;
		float radv;
		float rada;

		float alpha;
		float alphav;
		float alphaa;
	};

	BOOL m_1stpass;
	BOOL m_1stupdate;
	BOOL m_initialized;

	UnderFloor uf[NUM_UNDERFLOOR];
	MYVERTEX3D uf_vb[4*NUM_UNDERFLOOR+32];
	WORD uf_idx[NUM_UNDERFLOOR*6];
	LPDIRECT3DTEXTURE8 uf_tex;
};