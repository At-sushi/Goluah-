/*--------------------------------------------------------------
	人魂みたいなの
----------------------------------------------------------------*/
#include "soul_down.h"


#define BASE_Y	(-0.3f)

CSoulDown::CSoulDown()
{
	m_soul = aki3d.CreateParticleObject();
}

void CSoulDown::Init()
{
	m_soul->Create(NUM_SOUL_DOWN, "tex_soul.bmp" );

	for(UINT i=0;i<NUM_SOUL_DOWN;i++)
	{
		m_soulp[i].Reset();
		m_soulp[i].pos.x = aki3d.RandomOne()-0.5f;
		m_soulp[i].pos.z = 5.0f - aki3d.RandomOne()-0.5f;
		m_soulp[i].pos.y = -5.0f *aki3d.RandomOne();
	}
}

void CSoulDown::Update()
{
	for(UINT i=0;i<NUM_SOUL_DOWN;i++)
	{
		m_soulp[i].time+=1.0f;
	/*	if(m_soulp[i].time > m_soulp[i].time_end)
		{
			m_soulp[i].Reset();
		}*/
		float t = ((m_soulp[i].pos.y-BASE_Y)/(-5.0f)) ;//m_soulp[i].time / m_soulp[i].time_end;
		if(t<0)t=0;

		m_soulp[i].acc.x += (aki3d.RandomOne()-0.5f)*2.0f*0.00001f;
		m_soulp[i].acc.y += (aki3d.RandomOne()-0.5f)*2.0f*0.00001f;
		m_soulp[i].acc.z += (aki3d.RandomOne()-0.5f)*2.0f*0.00001f;

		if(m_soulp[i].pos.x > 1.0f || m_soulp[i].pos.x < -1.0f)
		{
			m_soulp[i].vel.x *= -1.0f;
			m_soulp[i].acc.x = 0.0f;
		}

		if(m_soulp[i].pos.z < 3.0f || m_soulp[i].pos.z > 5.0f)
		{
			m_soulp[i].vel.z *= -1.0f;
			m_soulp[i].acc.z = 0.0f;
		}

		m_soulp[i].vel += m_soulp[i].acc;
		if(m_soulp[i].vel.y<0.005f)m_soulp[i].vel.y=0.005f;
		float len =D3DXVec3Length(&m_soulp[i].vel);
		if(len>5.0f){
			m_soulp[i].acc = Zero;
			len/=5.0f;
			m_soulp[i].vel /= len;
		}
		
		m_soulp[i].pos += m_soulp[i].vel;
		if(m_soulp[i].pos.z<0)m_soulp[i].pos.z=0;
		if(m_soulp[i].pos.y>BASE_Y){
			m_soulp[i].Reset();
		}

		float alpha;
		if(t<0.1f)alpha = 255.0f*t*10.0f;
		else if(t<0.75f)alpha = 255;
		else alpha = 255.0f*(1.0f-(t-0.75f)/(1.0f-0.75f));
		ARGB col( (BYTE)alpha,115,255,100 );

		m_soul->SetPos(i,m_soulp[i].pos);
		m_soul->SetAxis(i,Zero);
		m_soul->SetWidth(i,m_soulp[i].width*( 0.6f+sinf(t*16.0f)*0.4f ));
		m_soul->SetColor(i,col);
	}
}

void CSoulDown::SoulParameter::Reset()
{
	pos.x = 0.0f;
	pos.z = 5.0f;
	pos.y = -5.0f;

	vel = Zero;
	acc = Zero;

	time = 0;
	time_end = 360.0f + 360.0f*aki3d.RandomOne();

	width = 110.0f/240.0f * (0.5f+aki3d.RandomOne());
}

void CSoulDown::Draw()
{
	aki3d.EnableZ(FALSE,FALSE);
	aki3d.SetBlend_Add();
	m_soul->Render();
	aki3d.SetBlend_Normal();
	aki3d.EnableZ();
}

void CSoulDown::Release()
{
	m_soul->Destroy();
}

