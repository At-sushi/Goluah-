
/*=======================================================================================

	�I�v�V�����I���N���X

=========================================================================================*/

#include "stdafx.h"
#include "global.h"
#include "task_simpleoptsel.h"

/*-----------------------------------------------------------
	�\�z
-------------------------------------------------------------*/
CTSimpleOptionSelecter::CTSimpleOptionSelecter()
{
	m_selecter = NULL;
	m_state = CTOPTSS_HideComplete;
	superclass_mode = FALSE;
}


/*-----------------------------------------------------------
	������
-------------------------------------------------------------*/
void CTSimpleOptionSelecter::Initialize()
{
}


/*-----------------------------------------------------------
	�j��
-------------------------------------------------------------*/
void CTSimpleOptionSelecter::Terminate()
{
	if(m_selecter){
		delete m_selecter;
		m_selecter = NULL;
	}
}


/*-----------------------------------------------------------
	���s
-------------------------------------------------------------*/
BOOL CTSimpleOptionSelecter::Execute(DWORD time)
{
	float anmSpd = 0.08f;

	if (superclass_mode)
		return CTOptionSelecter::Execute(time);

	switch(m_state)
	{
	case CTOPTSS_NotReady:
		{
			m_tick += anmSpd;
			if(m_tick>1.0f){
				m_tick = 1.0f;
				m_state = CTOPTSS_Ready;
			}

			if (m_selecter->m_ref_cinfo->fav_opts.size() == 0){
				// �v���Z�b�g���Ȃ��̂ňȑO�̃��[�h�ŕ\��
				CTOptionSelecter::SetAndShow(m_cindex, m_keyIndex);
				CTOptionSelecter::SetPos(offset_x, m_top);
				superclass_mode = TRUE;
			}
		}break;
	case CTOPTSS_Ready:
		{
			if(m_keyIndex>=0){
				if(m_selecter->HandlePad(m_keyIndex))//TRUE���A��ƑI���I��
				{
					EndSelect();
					m_state = CTOPTSS_Hide;
				}
			}
		}break;
	case CTOPTSS_Hide:
		{
			m_tick -= anmSpd;
			if(m_tick<0){
				m_tick = 0;
				m_state = CTOPTSS_HideComplete;
				delete [] m_selecter;
				m_selecter = NULL;
			}
		}break;
	case CTOPTSS_HideComplete:
		{
		}break;
	}

	return superclass_mode ? CTOptionSelecter::Execute(time) : TRUE;
}


/*-----------------------------------------------------------
	�`��
-------------------------------------------------------------*/
void CTSimpleOptionSelecter::Draw()
{
	if(!m_selecter)return;

	if (superclass_mode){
		CTOptionSelecter::Draw();
		return;
	}

	g_draw.EnableZ(FALSE,FALSE);

	char *tmp_str;
	tmp_str = new char[128];

	//�r������Ă���t���O���Q�b�g
	DWORD ex_flag =0;
	DWORD k=0;
	CharOptionList::iterator ite;
	for(ite=m_selecter->list->begin();ite!=m_selecter->list->end();ite++)
	{
		if(m_selecter->enabled[k]){
			ex_flag |= ite->exclusive;
		}
		k++;
	}

/*	//�e�L�X�g�F�i��I�����j
	DWORD color_option		= 0x00222222;//--OPTIONS--�\��
	DWORD color_ok			= 0x00666666;//�|�C���g�EOK�\��(�L����)
	DWORD color_ng			= 0x00AA3333;//�|�C���g�EOK�\��(������)
	DWORD color_collis		= 0x00336633;//�����t���O����
	DWORD color_na2			= 0x00333366;//depend�s��
	DWORD color_enable		= 0x00222222;//�L�����
	DWORD color_disable		= 0x00444444;//�������
	DWORD color_pointb		= 0x00663333;//�|�C���g�s��
	//�e�L�X�g�F�i�I�����j
	DWORD color_ok_sel		= 0x00000000;//�|�C���g�EOK�\��(�L����)
	DWORD color_ng_sel		= 0x00AA0000;//�|�C���g�EOK�\��(������)
	DWORD color_collis_sel	= 0x00009900;//�����t���O����
	DWORD color_na2_sel		= 0x000000FF;//depend�s��
	DWORD color_enable_sel	= 0x00222222;//�L�����
	DWORD color_disable_sel	= 0x00444444;//�������
	DWORD color_pointb_sel	= 0x00AA0000;//�|�C���g�s��*/

	//�e�L�X�g�F�i��I�����j
	DWORD color_option		= 0x00444444;//--OPTIONS--�\��
    DWORD color_ok			= 0x00444444;//�|�C���g�EOK�\��(�L����)
	DWORD color_ng			= 0x00AA3333;//�|�C���g�EOK�\��(������)
	DWORD color_collis		= 0x0033AA33;//�����t���O����
	DWORD color_na2			= 0x0033AA33;//depend�s��
	DWORD color_enable		= 0x00222222;//�L�����
	DWORD color_disable		= 0x004444AA;//�������
	DWORD color_pointb		= 0x00AA3333;//�|�C���g�s��
	//�e�L�X�g�F�i�I�����j
	DWORD color_ok_sel		= 0x00222222;//�|�C���g�EOK�\��(�L����)
	DWORD color_ng_sel		= 0x00AA0000;//�|�C���g�EOK�\��(������)
	DWORD color_collis_sel	= 0x0000AA00;//�����t���O����
	DWORD color_na2_sel		= 0x0000AA00;//depend�s��
	DWORD color_enable_sel	= 0x00222222;//�L�����
	DWORD color_disable_sel	= 0x000000AA;//�������
	DWORD color_pointb_sel	= 0x00AA0000;//�|�C���g�s��

	float y=m_top,z=0;
	float text_xscale = 0.8f;
	float text_yscale = m_fontheight/30.0f;
	float text_spacing = m_fontheight + 1.0f;

	BYTE  _alpha = (BYTE)(m_tick * 255);
	DWORD alpha = _alpha << 24;

	//OPTIONS �`��
	DWORD okcolor;
	double tmpx = g_system.DrawBMPTextEx(offset_x,y,z,"--OPTION PRESETS--",color_option|alpha,1.0f,text_yscale*1.2f,SYSBMPTXT_PROP);
	sprintf(tmp_str,"POINT: %d",m_selecter->current_point);
	if(m_selecter->current_point > 0)okcolor = color_ok;
	else okcolor = color_ng;
	g_system.DrawBMPTextEx(tmpx+20,y,z,tmp_str,okcolor|alpha,1.0f,text_yscale*1.2f,SYSBMPTXT_PROP);

	y+=text_spacing*1.2f;

	//�ݒ薼�\��
	FavoriteOptionList::iterator itef;
	k=0;
	DWORD color;
	for(itef=m_selecter->m_ref_cinfo->fav_opts.begin();itef!=m_selecter->m_ref_cinfo->fav_opts.end();itef++){
		if (k == m_selecter->m_current_favorite-1)
			color = color_ok_sel|alpha;
		else color = color_ok|alpha;
		g_system.DrawBMPTextEx(offset_x,y,z,itef->name,color,1.0f,text_yscale,SYSBMPTXT_PROP);

		y+=text_spacing;
		k++;
	}
	if (m_selecter->m_current_favorite == 0)
		color = color_ok_sel|alpha;
	else color = color_ok|alpha;
	g_system.DrawBMPTextEx(offset_x,y,z,"CUSTOM",color,1.0f,text_yscale,SYSBMPTXT_PROP);

	delete [] tmp_str;

	g_draw.EnableZ();
}


/*-----------------------------------------------------------
	����E�ݒ�
-------------------------------------------------------------*/

int CTSimpleOptionSelecter::SetAndShow(DWORD cindex,DWORD keyindex)
{
	if(m_selecter)return -1;

	m_cindex = cindex;
	m_keyIndex = keyindex;
	m_selecter = g_charlist.GetSimpleOptionSelecter(cindex);

	if(m_selecter){
		m_state = CTOPTSS_NotReady;
		m_tick = 0.0f;
		int optnum = m_selecter->m_ref_cinfo->fav_opts.size();
		if(optnum<2)m_fontheight = 20.0f;
		else if(optnum<20)m_fontheight = 20.0f - (20.0f-8.0f)*((float)optnum-2)/18.0f ;
		else m_fontheight = 8.0f;
	}
	else return -1;

	return m_selecter->list->size();
}

float CTSimpleOptionSelecter::GetHeight()
{
	return max((m_selecter->m_ref_cinfo->fav_opts.size()+2), (m_selecter->list->size()+2))
			*(m_fontheight+1.0f) + (m_fontheight*1.2f) ;
}

void CTSimpleOptionSelecter::EndSelect()
{

	if (superclass_mode)
		CTOptionSelecter::EndSelect();
	else if (m_selecter->m_current_favorite == 0 && m_selecter->state != 0xFFFFFFFF){
		// CUSTOM���I�����ꂽ�Ƃ��ɂ̓��[�h��؂�ւ���
		CTOptionSelecter::SetAndShow(m_cindex, m_keyIndex);
		CTOptionSelecter::SetPos(offset_x, m_top);
		superclass_mode = TRUE;
	}
	else{
		CCharacterSelect *csel = dynamic_cast<CCharacterSelect*>( g_system.GetCurrentMainTask() );
		if(csel){
			csel->OnOptionSelect(this,m_selecter->GetSettings());
		}
	}
}


