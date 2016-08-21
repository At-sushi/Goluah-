/*--------------------------------------------------------------
	人魂みたいなの
----------------------------------------------------------------*/
#pragma once
#include "stage.h"

#define NUM_SOUL	384

class CSoul : public CStageObjectBase
{
public:
	CSoul();
	void Init();
	void Update();
	void Draw();
	void Release();

	CParticle* m_soul[7];
	struct SoulParameter
	{
		float time;
		float time_end;
		V3d pos;
		V3d vel;
		V3d acc;
		float width;
		void Reset();
	};
	SoulParameter m_soulp[NUM_SOUL];
	int counter;
};