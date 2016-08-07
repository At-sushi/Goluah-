
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
	custom_mode = FALSE;
}


/*-----------------------------------------------------------
	���s
-------------------------------------------------------------*/
BOOL CTSimpleOptionSelecter::Execute(DWORD time)
{
	float anmSpd = 0.08f;

	if (custom_mode)
		return m_customselect.Execute(time);

	return CTOptionSelecter::Execute(time);
}


/*-----------------------------------------------------------
	�`��
-------------------------------------------------------------*/
void CTSimpleOptionSelecter::Draw()
{
	if(!m_selecter)return;

	if (custom_mode){
		m_customselect.Draw();
		return;
	}

	g_draw.EnableZ(FALSE,FALSE);

	TCHAR *tmp_str;
	tmp_str = new TCHAR[128];

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

	//�e�L�X�g�F�i��I�����j
	DWORD color_option		= 0x00444444;//--OPTIONS--�\��
    DWORD color_ok			= 0x005237FF;//�|�C���g�EOK�\��(�L����)
	DWORD color_ng			= 0x00FF3752;//�|�C���g�EOK�\��(������)
	DWORD color_collis		= 0x0033AA33;//�����t���O����
	DWORD color_na2			= 0x0033AA33;//depend�s��
	DWORD color_enable		= 0x00222222;//�L�����
	DWORD color_disable		= 0x004444AA;//�������
	DWORD color_pointb		= 0x00AA3333;//�|�C���g�s��
	//�e�L�X�g�F�i�I�����j
	DWORD color_ok_sel		= 0x0000AAFF;//�|�C���g�EOK�\��(�L����)
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
	double tmpx = g_system.DrawBMPTextEx(offset_x,y,z,_T("--PRESETS--"),color_option|alpha,1.0f,text_yscale*1.2f,SYSBMPTXT_PROP);
	_stprintf(tmp_str,_T("POINT:%d"),m_selecter->current_point);
	if(m_selecter->current_point >= 0)okcolor = color_ok;
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
	g_system.DrawBMPTextEx(offset_x,y,z,_T("CUSTOM"),color,1.0f,text_yscale,SYSBMPTXT_PROP);

	delete [] tmp_str;

	g_draw.EnableZ();
}


/*-----------------------------------------------------------
	����E�ݒ�
-------------------------------------------------------------*/

int CTSimpleOptionSelecter::SetAndShow(DWORD cindex,DWORD keyindex)
{
	if(m_selecter){
		delete m_selecter;
		m_selecter = NULL;
	}

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
		custom_mode = FALSE;	// ���Z�b�g
		if (optnum == 0)
		{
			// �J�X�^���I�������ł��Ȃ�����m_customselect�̏������[�h�ɋ����ڍs
			m_customselect.SetAndShow(m_cindex, m_keyIndex);
			custom_mode = TRUE;
		}
	}
	else return -1;

	return m_selecter->list->size();
}

float CTSimpleOptionSelecter::GetHeight()
{
	// �I�v�V�����̌��Ɠ_�v���̌��ƂŁA�������B
	return max((m_selecter->m_ref_cinfo->fav_opts.size()+2), (m_selecter->list->size()+2))
			*(m_fontheight+1.0f) + (m_fontheight*1.2f) ;
}

void CTSimpleOptionSelecter::EndSelect()
{
/*	if (custom_mode)
		m_customselect.EndSelect();	// �������ŕ񍐂��邩��v��Ȃ�����
	else*/ if (m_selecter->m_current_favorite == 0 && m_selecter->state != 0xFFFFFFFF){
		// CUSTOM���I�΂ꂽ�̂ŁAm_customselect�̏������[�h�Ɉڍs
		m_customselect.SetAndShow(m_cindex, m_keyIndex);
		custom_mode = TRUE;
	}
	else{
		CTOptionSelecter::EndSelect();
	}
}


