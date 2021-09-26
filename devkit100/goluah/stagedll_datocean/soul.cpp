/*--------------------------------------------------------------
        人魂みたいなの
----------------------------------------------------------------*/
#include "soul.h"

#define BASE_Y (-0.3f)

CSoul::CSoul() {
  for (int i = 0; i < 7; i++)
    m_soul[i] = aki3d.CreateParticleObject();

  counter = 0;
}

void CSoul::Init() {
  for (int i = 0; i < 7; i++)
    m_soul[i]->Create(NUM_SOUL, "tex_soul.bmp");

  for (UINT i = 0; i < NUM_SOUL; i++) {
    m_soulp[i].Reset();
  }
}

void CSoul::Update() {
  for (UINT i = 0; i < NUM_SOUL; i++) {
    m_soulp[i].time += 1.0f;
    if (m_soulp[i].time > m_soulp[i].time_end) {
      m_soulp[i].Reset();
    }
    float t = m_soulp[i].time / m_soulp[i].time_end;

    m_soulp[i].acc.x += (aki3d.RandomOne() - 0.5f) * 2.0f * 0.00001f;
    m_soulp[i].acc.y += (aki3d.RandomOne() - 0.5f) * 2.0f * 0.00001f;
    m_soulp[i].acc.z += (aki3d.RandomOne() - 0.5f) * 2.0f * 0.00001f;

    m_soulp[i].vel += m_soulp[i].acc;
    if (m_soulp[i].vel.y > 0)
      m_soulp[i].vel.y = 0;
    float len = D3DXVec3Length(&m_soulp[i].vel);
    if (len > 5.0f) {
      m_soulp[i].acc = Zero;
      len /= 5.0f;
      m_soulp[i].vel /= len;
    }

    m_soulp[i].pos += m_soulp[i].vel;
    if (m_soulp[i].pos.z < 0)
      m_soulp[i].pos.z = 0;
    if (m_soulp[i].pos.y > 3.0f) {
      if (t < 0.75f)
        m_soulp[i].time = m_soulp[i].time_end * 0.75f;
    }

    float alpha;
    if (t < 0.1f)
      alpha = 255.0f * t * 10.0f;
    else if (t < 0.75f)
      alpha = 255;
    else
      alpha = 255.0f * (1.0f - (t - 0.75f) / (1.0f - 0.75f));
    ARGB col((BYTE)alpha, 115, 255, 100);

    int c = /*counter++ % 7*/ 0;
    m_soul[c]->SetPos(i, m_soulp[i].pos);
    m_soul[c]->SetAxis(i, Zero);
    m_soul[c]->SetWidth(i, m_soulp[i].width * (0.7f + sinf(t * 12.0f) * 0.3f));
    m_soul[c]->SetColor(i, col);

/*		// 古い残像を薄くする処理
		for (int j = (c + 1) % 7; j != c; j = (j + 1) % 7)
		{
			col.alpha = (BYTE)( ((int)col.alpha - 0x22) > 0 ? ((int)col.alpha - 0x22) : 0 );
			m_soul[j]->SetColor(i,col);
		}
*/	}
}

void CSoul::SoulParameter::Reset() {
  pos.x = (aki3d.RandomOne() - 0.5f) * 2.0f * 1024.0f / 240.0f;
  pos.z = aki3d.RandomOne() * 10.0f;
  pos.y = BASE_Y;

  vel = Zero;
  acc = Zero;

  time = 0;
  time_end = 360.0f + 360.0f * aki3d.RandomOne();

  width = 90.0f / 240.0f * (0.5f + aki3d.RandomOne());
}

void CSoul::Draw() {
  aki3d.EnableZ(FALSE, FALSE);
  aki3d.SetBlend_Add();
  for (int i = 0; i < 1; i++)
    m_soul[i]->Render();
  aki3d.SetBlend_Normal();
  aki3d.EnableZ();
}

void CSoul::Release() {
  for (int i = 0; i < 7; i++)
    m_soul[i]->Destroy();
}
