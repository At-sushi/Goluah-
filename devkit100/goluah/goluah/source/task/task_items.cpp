
/*=======================================================================================

	�\���n�x�[�X

	��ʏ�\�����x�[�X�N���X

=========================================================================================*/

#include "stdafx.h"
#include "define_const.h"
#include "task_items.h"
#include "global.h"



/*=======================================================================================

	�����O��A�C�R���\��&�I���N���X

=========================================================================================*/

/*-----------------------------------------------------------
	������
-------------------------------------------------------------*/
void CTIconRingBase::Initialize()
{
	//�p�����[�^���Z�b�g
	m_counter = 0;
	m_state = CTCRS_NotReady;
	m_radius_base = 120;
	m_sleepCount = 0;

	InitializeSub();
	InitializeIcons();
}


/*-----------------------------------------------------------
	�j��
-------------------------------------------------------------*/
void CTIconRingBase::Terminate()
{
	DestroyIcons();
	TerminateSub();
}


/*-----------------------------------------------------------
	����E���s
-------------------------------------------------------------*/
BOOL CTIconRingBase::Execute(DWORD time)
{
	CTIconRingState sta_prv = m_state;

	switch(m_state){
	case CTCRS_NotReady://�o����
		Exec_NotReady();break;
	case CTCRS_Ready://�I���\
		Exec_Ready();break;
	case CTCRS_RotateRight://�E��]��
		Exec_RotateRight();break;
	case CTCRS_RotateLeft://����]��
		Exec_RotateLeft();break;
	case CTCRS_ChangeUp://�����O��ύX��
		Exec_ChangeUp();break;
	case CTCRS_ChangeDown://�����O���ύX��
		Exec_ChangeDown();break;
	case CTCRS_Hide://�����A�j���[�V����
		Exec_Hide();break;
	case CTCRS_Restore://���A�A�j���[�V����
		Exec_Restore();break;
	}

	if(sta_prv!=m_state)m_counter=0;
	else m_counter++;
	if(m_sleepCount>0)m_sleepCount--;

	return TRUE;
}

//�J�n���A�j���[�V����
void CTIconRingBase::Exec_NotReady()
{
	double tick = (m_counter - 30.0)/30.0 ;// -1 �� 0

	m_rotation2 = tick*400.0f;
	m_alpha =  (BYTE)( (1.0+tick)* 255 );
	m_radius_ratio = 1.0f - tick*2.0;

	if(m_counter>=30)m_state = CTCRS_Ready;
}

void CTIconRingBase::Exec_Ready()
{
	//���[�e�[�V�����m��
	m_alpha = 255;
	m_radius_ratio = 1;

	//�L�[����
	if(m_keyindex>=0 && m_sleepCount==0){
		DWORD keystate = g_input.GetKey(m_keyindex,0);
		//�{�^�������ꂽ
		if(keystate & KEYSTA_BA2){OnButtonDown(KEYSTA_BA2);}
		else if(keystate & KEYSTA_BD2){OnButtonDown(KEYSTA_BD2);}
		else if(keystate & KEYSTA_BB2){OnButtonDown(KEYSTA_BB2);}
		else if(keystate & KEYSTA_BC2){OnButtonDown(KEYSTA_BC2);}
		//���E��
		else if(keystate & KEYSTA_ARIGHT && g_input.SeekKey(m_keyindex, 1, 30, KEYSTA_ARIGHT2) < 0 &&
				this->m_counter >= 5){
			// �ςȏ������򂾂��A�v�����SeekKey�Ɉ���������Ȃ���ΐ^�B
			if(m_selected_index==0)m_selected_index =GetMaxIndex(m_selected_ring)-1;
			else m_selected_index--;
			m_state = CTCRS_RotateLeft;
			m_counter = 0;
			Exec_RotateLeft();
			OnChangeIndex();
		}
		else if(keystate & KEYSTA_ALEFT && g_input.SeekKey(m_keyindex, 1, 30, KEYSTA_ALEFT2) < 0 &&
				this->m_counter >= 5){
			// �ςȏ������򂾂��A�v�����SeekKey�Ɉ���������Ȃ���ΐ^�B
			m_selected_index++;
			if((int)m_selected_index >= GetMaxIndex(m_selected_ring))
				m_selected_index=0;
			m_state = CTCRS_RotateRight;
			m_counter = 0;
			Exec_RotateRight();
			OnChangeIndex();
		}
		//���E��(�����O�ύX)
		else if( keystate & KEYSTA_UP2 ){
			m_previous_ring = m_selected_ring;
			m_previous_index = m_selected_index;
			if(m_selected_ring==0)m_selected_ring=GetMaxRing()-1;
			else m_selected_ring--;
			while(GetMaxIndex(m_selected_ring)==0){
				if(m_selected_ring==0)m_selected_ring=GetMaxRing()-1;
				else m_selected_ring--;
			}
			if(GetMaxIndex(m_selected_ring)-1 < (int)m_selected_index)
				m_selected_index = GetMaxIndex(m_selected_ring)-1 ;
			m_state = CTCRS_ChangeUp;
			OnChangeRing();
		}
		else if( keystate & KEYSTA_DOWN2 ){
			m_previous_ring = m_selected_ring;
			m_previous_index = m_selected_index;
			m_selected_ring++;
			if((int)m_selected_ring > GetMaxRing()-1)m_selected_ring=0;
			while(GetMaxIndex(m_selected_ring)==0){
				m_selected_ring++;
				if((int)m_selected_ring>GetMaxRing()-1)m_selected_ring=0;
			}
			if(GetMaxIndex(m_selected_ring)-1 < (int)m_selected_index)
				m_selected_index = GetMaxIndex(m_selected_ring)-1 ;
			m_state = CTCRS_ChangeDown;
			OnChangeRing();
		}
	}
}

void CTIconRingBase::Exec_RotateRight()
{
	DWORD cur_num = GetMaxIndex(m_selected_ring);
	double drot = 360.0/cur_num;

	m_radius_ratio = 1;
	m_alpha = 255;
	m_rotation2 = (10.0 - m_counter)/10.0 * drot;

	if(m_counter>=10)m_state = CTCRS_Ready;
}

void CTIconRingBase::Exec_RotateLeft()
{
	DWORD cur_num = GetMaxIndex(m_selected_ring);
	double drot = 360.0/cur_num;

	m_radius_ratio = 1;
	m_alpha = 255;
	m_rotation2 = (10.0 - m_counter)/10.0 * drot * -1.0;

	if(m_counter>=10)m_state = CTCRS_Ready;
}

void CTIconRingBase::Exec_ChangeUp()
{
	double tick = m_counter/30.0;//0 �� 1

	m_rotation2 = (1.0-tick)*270.0f;
	m_radius_ratio = 1 + 2*(1-tick);
	m_radius_ratio2 = 1-tick;

	//��
	m_alpha  = (BYTE)(tick * 255);
	m_alpha2 = 255 - m_alpha;

	if(m_counter>=30)m_state = CTCRS_Ready;
}

void CTIconRingBase::Exec_ChangeDown()
{
	double tick = m_counter/30.0;//0 �� 1

	m_rotation2 = (1.0-tick)*270.0f;
	m_radius_ratio = tick;
	m_radius_ratio2 = 1 + 2*(tick);

	//��
	m_alpha  = (BYTE)(tick * 255);
	m_alpha2 = 255 - m_alpha;

	if(m_counter>=30)m_state = CTCRS_Ready;
}


//�I�����A�j���[�V�����J�n
void CTIconRingBase::Exec_Hide()
{
	double tick = (30.0 - m_counter)/30.0 ;// 0 �� -1

	m_rotation2 = tick*400.0f;
	m_alpha =  (BYTE)( (1.0+tick)* 255 );
	m_radius_ratio = 1.0f - tick*2.0;

	if(m_counter>=30)m_state = CTCRS_HideComplete;
}

//���A
void CTIconRingBase::Exec_Restore()
{
	double tick = (m_counter - 30.0)/30.0 ;// -1 �� 0

	m_rotation2 = (1.0-tick)*300.0f;
	m_alpha =  (BYTE)( (1.0+tick)* 255 );
	m_radius_ratio = 1.0f - tick*2.0;

	if(m_counter>=30){
		m_rotation2=0;
		m_state = CTCRS_Ready;
	}
}



/*-----------------------------------------------------------
	����
-------------------------------------------------------------*/

//�L�[���̓\�[�X�ݒ�
void CTIconRingBase::SetKeyInputIndex(int i)
{
	m_keyindex=i;
	m_sleepCount = 5;//�ύX���ꂽ�Ƃ������Ȃ茈�肵�Ă��܂��̂����
}

//���A
void CTIconRingBase::Restore()
{
	m_sleepCount = 5;//�ύX���ꂽ�Ƃ������Ȃ茈�肵�Ă��܂��̂����
	m_state = CTCRS_Restore;
	m_counter = 0;
}

/*-----------------------------------------------------------
	�`��
-------------------------------------------------------------*/
void CTIconRingBase::Draw()
{
	if(m_state == CTCRS_HideComplete)return;

	if(m_state==CTCRS_ChangeUp || m_state==CTCRS_ChangeDown)
										g_draw.EnableZ(FALSE,FALSE);
	else g_draw.EnableZ();

	//local val.
	DWORD i;
	double rot;
	double drot;
	double rotbase;
	double x,y,z;
	double ring_x_len;
	double ring_y_len;
	double camz = g_draw.camera_z * -1;
	double adj;
	LONG faceWidth = 50;
	RECT r_face = {0,0,faceWidth,faceWidth};
	MYSURFACE *dds;
	MYRECT3D rdst;
	DWORD r_num;
	double adj_sel;
	r_num = GetMaxIndex(m_selected_ring);
	ring_x_len = m_radius_base * m_radius_ratio;
	ring_y_len = m_height;
	drot=360.0 / r_num;
	rotbase = (360.0/r_num)*m_selected_index;
	for(i=0;i<r_num;i++)//���C�������O�`��
	{
		//���W�Z�o
		rot = (drot)*i - rotbase + m_rotation2 + 90 ;
		rot = rot/180.0*3.1415;//rad�ɕϊ�
		x = ring_x_len*(cos(rot)) ;
		y = ring_y_len*(sin(rot)) ;
		z = y*-0.01;
		if (i==m_selected_index)
			z += -0.02;	// �I������Ă�̂����O�ɏo��
		//�␳
		adj = (1.0+(z/camz));
		x *= adj;
		y *= adj;
		//�I������Ă�����傫���\��
		adj_sel = (m_state==CTCRS_Ready ? (i==m_selected_index ? 1.2 : 1.0) : 1.0);
		dds = GetIcon(m_selected_ring,i) ;
		if(!dds)dds=gbl.GetBlankIcon();
		if(dds){
			rdst.z = (float)z;
			rdst.left  = (float)((x - (faceWidth*0.5*adj*adj_sel)) /240.0f + m_basePosX);
			rdst.right = (float)((x + (faceWidth*0.5*adj*adj_sel)) /240.0f + m_basePosX);
			rdst.top   = (float)((y - (faceWidth*0.5*adj*adj_sel)) /240.0f + m_basePosY);
			rdst.bottom= (float)((y + (faceWidth*0.5*adj*adj_sel)) /240.0f + m_basePosY);
			g_draw.MyBlt3D( dds,r_face,rdst,0,GetColor(m_selected_ring,i) | ((DWORD)m_alpha<<24) );
		}
	}
	if(m_state==CTCRS_ChangeUp || m_state==CTCRS_ChangeDown)//���������O�`��
	{
		r_num = GetMaxIndex(m_previous_ring);
		ring_x_len = m_radius_base * m_radius_ratio2;
		ring_y_len = m_height;
		drot=360.0 / r_num;
		rotbase = (360.0/r_num)*m_previous_index;
		for(i=0;i<r_num;i++)
		{
			//���W�Z�o
			rot = (drot)*i - rotbase - m_rotation2 + 90 ;
			rot = rot/180.0*3.1415;//rad�ɕϊ�
			x = ring_x_len*(cos(rot)) ;
			y = ring_y_len*(sin(rot)) ;
			z = y*-0.01;
			//�␳
			adj = (1.0+(z/camz));
			x *= adj;
			y *= adj;
			dds = GetIcon(m_previous_ring,i) ;
			if(dds){
				rdst.z = (float)z;
				rdst.left  = (float)((x - (faceWidth*0.5*adj)) /240.0f + m_basePosX);
				rdst.right = (float)((x + (faceWidth*0.5*adj)) /240.0f + m_basePosX);
				rdst.top   = (float)((y - (faceWidth*0.5*adj)) /240.0f + m_basePosY);
				rdst.bottom= (float)((y + (faceWidth*0.5*adj)) /240.0f + m_basePosY);
				g_draw.MyBlt3D( dds,r_face,rdst,0,GetColor(m_previous_ring,i) | ((DWORD)m_alpha2<<24) );
			}
		}
	}

	g_draw.EnableZ();
}


/*-----------------------------------------------------------
	�I���E�傫�����ď���
-------------------------------------------------------------*/
void CTIconRingBase::Hide()
{
	m_state = CTCRS_Hide;
	m_counter = 0;
}



/*=======================================================================================

	�E�B���h�E�\����{�N���X
	�P�Ƀv���~�e�B�u�Ŏl�p�`�`������

=========================================================================================*/

void CTWindowBase::Initialize()
{
	//�p�����[�^�����l
	m_ratio			= 0.0f;
//	m_winwidth		= 100.0f;
//	m_winheight		= 100.0f;
//	m_winleft		= 50.0f;
//	m_wintop		= 50.0f;
	m_wincolor		= 0xFFFFFFFF;
	m_winOpenTime	= 20.0f;
	m_winCloseTime	= 20.0f;
	m_state			= state_restore;
	m_counter		= 0;
}

BOOL CTWindowBase::Execute(DWORD time)
{
	switch(m_state)
	{
	case state_restore:
		{
			m_ratio = m_counter/m_winOpenTime;
			if(m_ratio>1.0f){
				m_ratio=1.0f;
				m_state = state_show;
			}
		}break;
	case state_show:
		{
			m_ratio = 1.0f;
		}break;
	case state_hide:
		{
			m_ratio = 1.0f - m_counter/m_winCloseTime;
			if(m_ratio<0.0f)m_ratio=0.0f;
		}break;
	}
	m_ratio = m_ratio * m_ratio; // 2��
	m_counter++;

	return TRUE;
}

void CTWindowBase::Draw()
{
	MYVERTEX3D* vb;

	if ( !g_draw.pMyVertex || FAILED(g_draw.pMyVertex->Lock(0, 0, (void**)&vb, D3DLOCK_DISCARD)) )
		return;

	vb[0].color = m_wincolor;
	vb[1].color = m_wincolor;
	vb[2].color = m_wincolor;
	vb[3].color = m_wincolor;

	vb[0].z = 0.0f;
	vb[1].z = 0.0f;
	vb[2].z = 0.0f;
	vb[3].z = 0.0f;

	vb[0].x = m_drx1;
	vb[1].x = m_drx2;
	vb[2].x = m_drx1;
	vb[3].x = m_drx2;

	vb[0].y = m_dry1;
	vb[1].y = m_dry1;
	vb[2].y = m_dry2;
	vb[3].y = m_dry2;

	if (g_draw.pMyVertex) g_draw.pMyVertex->Unlock();

	g_draw.EnableZ(FALSE,FALSE);
	g_draw.SetTransform(FALSE);
	g_draw.d3ddev->SetTexture(0,NULL);
	g_draw.d3ddev->SetStreamSource(0, g_draw.pMyVertex, 0, sizeof(MYVERTEX3D));
	g_draw.d3ddev->SetFVF(FVF_3DVERTEX);
	g_draw.d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
}

void CTWindowBase::CalcDrawParameters()
{
	float div = 1.0f/240.0f;

	m_drx1 = m_winleft*div;
	m_drx2 = m_drx1 + m_winwidth*m_ratio*div;
	m_dry1 = m_wintop*div;
	m_dry2 = m_dry1 + m_winheight*m_ratio*div;
}

void CTWindowBase::Open()
{
	m_state = state_restore;
	m_counter = 0;
}

void CTWindowBase::Close()
{
	m_state = state_hide;
	m_counter = 0;
}


/*------------------------------------------------------------------
	�e�L�X�g�`��B�E�C���h�E�̃T�C�Y�ɍ��킹�ĕ`�悵�Ă�������B
	������CSystem��DrawBMPText�ƈꏏ�����ǁA�ʒu�̓E�C���h�E���ł̈ʒu
--------------------------------------------------------------------*/
void CTWindowBase::DrawText(float x,float y,float z,
		char *str,DWORD col,float scaX,float scaY,DWORD flags)
{
	//�X�P�[������
	x *= m_ratio;
	y *= m_ratio;
	scaX *= m_ratio;
	scaY *= m_ratio;

	x += m_winleft;
	y += m_wintop;

	g_system.DrawBMPTextEx(x,y,z,str,col,scaX,scaY,flags);
}



/*=======================================================================================

	�w�i�I�r��{�N���X

=========================================================================================*/

//�p�����[�^���Z�b�g
CTBeltBase::CTBeltBase()
{
	strcpy(m_disp_str,_T(""));
	strcpy(m_disp_str2,_T(""));
	m_pos = 2;//center
	m_base_y = 240.0f;
	m_height_base = 50.0f;
	m_ratio = 1.0f;
}

//m_top , m_bottom�Z�o
void CTBeltBase::CalcTopBottom()
{
	m_top    = m_base_y - m_height_base * m_ratio*m_ratio *0.5f;
	m_bottom = m_base_y + m_height_base * m_ratio*m_ratio *0.5f;
}

//�`��
void CTBeltBase::Draw()
{
	if(m_ratio<0.02f)return;

	g_draw.EnableZ(FALSE,FALSE);
	g_draw.SetTransform(FALSE);
	g_draw.d3ddev->SetTexture(0,NULL);
	g_draw.d3ddev->SetFVF(FVF_3DVERTEX);

	MYVERTEX3D vb[6];

	for(int i=0;i<6;i++){
		vb[i].z =  0.0f;
		vb[i].tu = vb[i].tv = 0.0f;
	}
	vb[0].x = 0.0f / 240.0f;
	vb[1].x = vb[0].x;
	vb[2].x = 320.0f / 240.0f;
	vb[3].x = vb[2].x;
	vb[4].x = 640.0f / 240.0f;
	vb[5].x = vb[4].x;

	//���ݒ菀��
	DWORD alpha1,alpha2,alpha3;
	switch(m_pos)
	{
	case 0://left
		{
			alpha1 = m_alpha1 << 24;
			alpha3 = m_alpha2 << 24;
			alpha2 = (m_alpha1 + m_alpha2)/2;
			alpha2 <<= 24;
		}break;
	case 1://right
		{
			alpha3 = m_alpha1 << 24;
			alpha1 = m_alpha2 << 24;
			alpha2 = (m_alpha1 + m_alpha2)/2;
			alpha2 <<= 24;
		}break;
	case 2://center
		{
			alpha2 = m_alpha1 << 24;
			alpha1 = alpha3 = m_alpha2 << 24;
		}break;
	}

	

	//�F�����C���`��
	if(m_lineWidth > 0.1f)
	{

		vb[0].color = m_lineCol | alpha1 ;
		vb[1].color = m_lineCol | alpha1 ;
		vb[2].color = m_lineCol | alpha2 ;
		vb[3].color = m_lineCol | alpha2 ;
		vb[4].color = m_lineCol | alpha3 ;
		vb[5].color = m_lineCol | alpha3 ;

		//�F���E��
		vb[0].y = (m_top-m_lineWidth) / 240.0f;
		vb[1].y = (m_top) / 240.0f;
		vb[2].y = vb[0].y;
		vb[3].y = vb[1].y;
		vb[4].y = vb[0].y;
		vb[5].y = vb[1].y;
		g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,4,vb,sizeof(MYVERTEX3D));

		//�F���E��
		vb[0].y = (m_bottom) / 240.0f;
		vb[1].y = (m_bottom+m_lineWidth) / 240.0f;
		vb[2].y = vb[0].y;
		vb[3].y = vb[1].y;
		vb[4].y = vb[0].y;
		vb[5].y = vb[1].y;
		g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,4,vb,sizeof(MYVERTEX3D));
	}


	//���n
	vb[0].color = m_bodyCol | alpha1 ;
	vb[1].color = m_bodyCol | alpha1 ;
	vb[2].color = m_bodyCol | alpha2 ;
	vb[3].color = m_bodyCol | alpha2 ;
	vb[4].color = m_bodyCol | alpha3 ;
	vb[5].color = m_bodyCol | alpha3 ;

	vb[0].y = (m_top) / 240.0f;
	vb[1].y = (m_bottom) / 240.0f;
	vb[2].y = vb[0].y;
	vb[3].y = vb[1].y;
	vb[4].y = vb[0].y;
	vb[5].y = vb[1].y;
	g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,4,vb,sizeof(MYVERTEX3D));


	//�����`��
	if(m_show_text){
		DWORD txtflg = SYSBMPTXT_PROP;
		if(m_txtR2L)txtflg += SYSBMPTXT_R2L;
		int txtZure = 3;//�e�����炵��
		if(m_txtTop){
			g_system.DrawBMPTextEx(m_txtLeft+txtZure,m_top+txtZure,0,m_disp_str,m_txtCol1,1.0f,m_ratio,txtflg);
			g_system.DrawBMPTextEx(m_txtLeft+txtZure,m_top+33+txtZure,0,m_disp_str2,m_txtCol1,1.0f,m_ratio,txtflg);
			if(m_txtCol2 & 0xFF000000)
			g_system.DrawBMPTextEx(m_txtLeft,m_top,0,m_disp_str,m_txtCol2,1.0f,m_ratio,txtflg);
			g_system.DrawBMPTextEx(m_txtLeft,m_top+33,0,m_disp_str2,m_txtCol2,1.0f,m_ratio,txtflg);
		}
		else{
			g_system.DrawBMPTextEx(640.0f-m_txtLeft+txtZure,m_bottom-33+txtZure,0,m_disp_str,m_txtCol1,1.0f,m_ratio,txtflg);
			g_system.DrawBMPTextEx(640.0f-m_txtLeft+txtZure,m_bottom-66+txtZure,0,m_disp_str2,m_txtCol1,1.0f,m_ratio,txtflg);
			if(m_txtCol2 & 0xFF000000)
			g_system.DrawBMPTextEx(640.0f-m_txtLeft,m_bottom-33,0,m_disp_str,m_txtCol2,1.0f,m_ratio,txtflg);
			g_system.DrawBMPTextEx(640.0f-m_txtLeft,m_bottom-66,0,m_disp_str2,m_txtCol2,1.0f,m_ratio,txtflg);
		}
	}
	g_draw.EnableZ();
}


