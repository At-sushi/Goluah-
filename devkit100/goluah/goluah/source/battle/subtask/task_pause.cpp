/*----------------------------------------------------------------------

	ポーズ画面

------------------------------------------------------------------------*/

#include "stdafx.h"
#include "task_pause.h"
#include "global.h"

void CTBattlePause::Initialize()
{
	tex_fb = NULL;
	tex_pause = NULL;

	tex_fb = g_draw.GetFrontBufferCopy();
	D3DXCreateTextureFromFileA(g_draw.d3ddev,_T("system\\texture\\pause.png"),&tex_pause);

	m_counter = 0;
	m_face_counter[0] = 0;
	m_face_counter[1] = 0;
	m_face_idx[0] = 0;
	m_face_idx[1] = 0;

	m_kill_flag = FALSE;
	m_inst_on[0] = m_inst_on[1] = FALSE;

	m_shiftY = 0;
}

void CTBattlePause::Terminate()
{
	RELEASE(tex_fb);
	RELEASE(tex_pause);
}

BOOL CTBattlePause::Execute(DWORD time)
{
	m_counter ++;

	return m_kill_flag ? FALSE : TRUE;
}

void CTBattlePause::Draw()
{
	g_draw.EnableZ(FALSE,FALSE);
	g_draw.SetTransform(FALSE);
	g_draw.d3ddev->SetFVF(FVF_3DVERTEX);

	float ar = 320.0f/240.0f;

	MYVERTEX3D vb[4];

	vb[0].z = 0.0f;
	vb[1].z = 0.0f;
	vb[2].z = 0.0f;
	vb[3].z = 0.0f;

	vb[0].tu = 0.0f;
	vb[1].tu = 0.0f;
	vb[2].tu = 1.0f;
	vb[3].tu = 1.0f;
	
	vb[0].tv = 0.0f;
	vb[1].tv = 1.0f;
	vb[2].tv = 0.0f;
	vb[3].tv = 1.0f;

	//フロントバッファをコピーしたやつ
	if(tex_fb)
	{
		DWORD col;
		
		col = 255 - (m_counter>100 ? 100 : m_counter);
		vb[0].color = 
		vb[1].color = 0xFF000000 | (col<<16) | (col<<8) | col;

		col = 255 - (m_counter>100 ? 100 : m_counter);
		vb[2].color = 
		vb[3].color = 0xFF000000 | (col<<16) | (col<<8) | col;

		vb[0].x =  0.0f*ar;
		vb[1].x =  0.0f*ar;
		vb[2].x =  2.0f*ar;
		vb[3].x =  2.0f*ar;

		vb[0].y =  0.0f;
		vb[1].y =  2.0f;
		vb[2].y =  0.0f;
		vb[3].y =  2.0f;

		g_draw.d3ddev->SetTexture(0,tex_fb);
		g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
	}
	
	//"Pause - Press F7 Key"
	g_draw.SetAlphaMode(GBLEND_KASAN);
	if(tex_pause)
	{
		vb[0].color = 
		vb[1].color = 
		vb[2].color = 
		vb[3].color = 0xFF55FF33;

		vb[0].x =  (320.0f-128.0f)/240.0f;
		vb[1].x =  (320.0f-128.0f)/240.0f;
		vb[2].x =  (320.0f+128.0f)/240.0f;
		vb[3].x =  (320.0f+128.0f)/240.0f;

		vb[0].y =  (105.0f-32.0f)/240.0f;
		vb[1].y =  (105.0f+32.0f)/240.0f;
		vb[2].y =  (105.0f-32.0f)/240.0f;
		vb[3].y =  (105.0f+32.0f)/240.0f;

		g_draw.d3ddev->SetTexture(0,tex_pause);
		g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
	}
	g_draw.SetAlphaMode(0);

	if(!m_inst_on)return;
}
