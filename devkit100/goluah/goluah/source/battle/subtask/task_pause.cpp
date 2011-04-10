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
	D3DXCreateTextureFromFileA(g_draw.d3ddev,"system\\texture\\pause.png",&tex_pause);

	m_counter = 0;
	m_face_counter[0] = 0;
	m_face_counter[1] = 0;

	CBattleTaskBase* battleTask = dynamic_cast<CBattleTaskBase*>( g_system.GetCurrentMainTask() );
	if(!battleTask)
	{
		m_face_idx[0] = 0;
		m_face_idx[1] = 0;
	}
	else
	{
		m_face_idx[0] = battleTask->GetActiveCharacterID(0);
		m_face_idx[1] = battleTask->GetActiveCharacterID(1);
	}

	m_kill_flag = FALSE;
	m_inst_on[0] = m_inst_on[1] = FALSE;

	ms_inst[0] = ms_inst[1] = NULL;
}

void CTBattlePause::Terminate()
{
	RELEASE(tex_fb);
	RELEASE(tex_pause);

	g_draw.RelSurface( ms_inst[0] );
	g_draw.RelSurface( ms_inst[1] );
}

BOOL CTBattlePause::Execute(DWORD time)
{
	m_counter ++;

	for(UINT i=0;i<2;i++)
	{
		DWORD key = 0;

		if(m_inst_on[i])
		{
			m_face_counter[i] ++;
		}

		//そのチームでのキー入力の総和をとる
		for(UINT j=0;j<MAXNUM_TEAM;j++)
		{
			UINT ki = g_battleinfo.GetKeyAssign(i,j);
			if(!(ki&CASSIGN_SPECIFIC))
			{
				key |= g_input.GetKey(ki,0);
			}
		}

		if(m_inst_on[i])
		{
			if(key & KEYSTA_LEFT2)
			{
				m_face_idx[i] --;
				m_face_idx[i] = (m_face_idx[i]+g_battleinfo.GetNumTeam(i)) % g_battleinfo.GetNumTeam(i);
				m_face_counter[i] = 0;
				ChangeInst(i);
			}
			if(key & KEYSTA_RIGHT2)
			{
				m_face_idx[i] ++;
				m_face_idx[i] %= g_battleinfo.GetNumTeam(i);
				m_face_counter[i] = 0;
				ChangeInst(i);
			}
		}

		if((key & KEYSTA_BA2) || (key & KEYSTA_BB2) || (key & KEYSTA_BC2))
		{
			m_inst_on[i] = !m_inst_on[i];
			if(!m_inst_on[i])
			{
				g_draw.RelSurface( ms_inst[i] );
				ms_inst[i] = NULL;
				m_face_counter[i] = 0;
			}
			else
			{
				ChangeInst(i);
			}
		}
	}

	return m_kill_flag ? FALSE : TRUE;
}

void CTBattlePause::Draw()
{
	g_draw.EnableZ(FALSE,FALSE);
	g_draw.SetTransform(FALSE);
	g_draw.d3ddev->SetVertexShader(FVF_3DVERTEX);

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

	int alt;
	int t;
	MYSURFACE *dds_face;
	RECT r_face;
	int x,y;

	for(t=1;t>=0;t--)
	{
		if(!m_inst_on[t])continue;

		//デカ顔

		alt = OPT2ALT(g_battleinfo.GetCharacterOption(t,m_face_idx[t]));
		dds_face = gbl.GetBigFace(g_battleinfo.GetCharacter(t,m_face_idx[t]),g_battleinfo.GetColor(t,m_face_idx[t]),alt);

		if(!dds_face)continue;

		r_face.top = 0;
		r_face.left = 0;
		r_face.right = (int)dds_face->wg;
		r_face.bottom = (int)dds_face->hg;

		if(t==0){
			x = m_face_counter[t]*30 - (int)dds_face->wg;
			if(x>0)x=0;
		}
		else{
			x = 640 - m_face_counter[t]*30;
			if(x< 640-(int)dds_face->wg)x=640-(int)dds_face->wg;
		}

		g_draw.CheckBlt(
					dds_face,
					x,
					240-(DWORD)dds_face->hg/2,
					r_face,
					t==0 ? FALSE : TRUE,
					FALSE,
					0,
					-0.01f,
					0xFFFFFFFF);
		//inst
		if(ms_inst[t])
		{
			r_face.top = 0;
			r_face.left = 0;
			r_face.right = (int)ms_inst[t]->wg;
			r_face.bottom = (int)ms_inst[t]->hg;

			DWORD alpha ;
			x = t==0 ? 20 : 620-(int)ms_inst[t]->wg;
			y = 450-(DWORD)ms_inst[t]->hg;
			if(y >320 ) y = 320;

			//下地
			int mgn = 5;
			vb[0].x =  (x-mgn)/240.0f;
			vb[1].x =  (x-mgn)/240.0f;
			vb[2].x =  (x+mgn+ms_inst[t]->wg)/240.0f;
			vb[3].x =  (x+mgn+ms_inst[t]->wg)/240.0f;

			vb[0].y =  (y-mgn)/240.0f;
			vb[1].y =  (y+mgn+ms_inst[t]->hg)/240.0f;
			vb[2].y =  (y-mgn)/240.0f;
			vb[3].y =  (y+mgn+ms_inst[t]->hg)/240.0f;
			
			vb[0].z = 
			vb[1].z = 
			vb[2].z = 
			vb[3].z = -0.02f;
			
			alpha = m_face_counter[t]*10>220 ? 220 : m_face_counter[t]*10;

			vb[0].color = 
			vb[1].color = 
			vb[2].color = 
			vb[3].color = alpha<<24 | 0x00FFFFFF;
			
			D3DXMATRIX mat;
			D3DXMatrixIdentity(&mat);
			g_draw.d3ddev->SetTransform(D3DTS_WORLD,&mat);

			g_draw.d3ddev->SetTexture(0,NULL);
			g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));

			//ビットマップ
			alpha = m_face_counter[t]*10>255 ? 255 : m_face_counter[t]*10;
			g_draw.CheckBlt(
					ms_inst[t],
					x,
					y,
					r_face,
					FALSE,
					FALSE,
					0,
					-0.03f,
					alpha<<24 | 0x00FFFFFF );
		}
	}
}


void CTBattlePause::ChangeInst(UINT t)
{
	char *filepath = new char[256];
	char altstr[2] ={'\0','\0'};
	int alt = OPT2ALT(g_battleinfo.GetCharacterOption(t,m_face_idx[t]));
	if(alt!=0)
	{
		altstr[0] = 'a' + alt - 1;
	}
	UINT char_index = g_battleinfo.GetCharacter(t,m_face_idx[t]);
	
	sprintf(filepath,"%s\\inst%s",
		g_charlist.GetCharacterDir(char_index),
		altstr);

	ms_inst[t] = g_draw.CreateSurfaceFrom256Image(filepath);
	if(!ms_inst[t])
	{
		ms_inst[t] = g_draw.CreateSurfaceFrom256Image("system\\inst");
	}

	DELETE_ARRAY(filepath);
}

