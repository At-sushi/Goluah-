/*--------------------------------------------------------------
	êlç∞Ç›ÇΩÇ¢Ç»ÇÃ(óéâ∫)
----------------------------------------------------------------*/
#pragma once
#include "stage.h"


#define NUM_SOUL_DOWN	96

class CSoulDown : public CStageObjectBase
{
public:
	CSoulDown();
	void Init();
	void Update();
	void Draw();
	void Release();

	CParticle* m_soul;
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
	SoulParameter m_soulp[NUM_SOUL_DOWN];
};
