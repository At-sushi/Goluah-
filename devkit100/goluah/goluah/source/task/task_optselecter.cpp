
/*=======================================================================================

	�I�v�V�����I���N���X

=========================================================================================*/

#include "global.h"
#include "task_optselecter.h"

/*-----------------------------------------------------------
	�\�z
-------------------------------------------------------------*/
CTOptionSelecterBase::CTOptionSelecterBase()
{
	m_selecter = NULL;
	m_state = CTOPTSS_HideComplete;
}


/*-----------------------------------------------------------
	������
-------------------------------------------------------------*/
void CTOptionSelecterBase::Initialize()
{
}


/*-----------------------------------------------------------
	�j��
-------------------------------------------------------------*/
void CTOptionSelecterBase::Terminate()
{
	if(m_selecter){
		delete m_selecter;
		m_selecter = NULL;
	}
}


/*-----------------------------------------------------------
	���s
-------------------------------------------------------------*/
BOOL CTOptionSelecterBase::Execute(DWORD time)
{
	float anmSpd = 0.08f;

	switch(m_state)
	{
	case CTOPTSS_NotReady:
		{
			m_tick += anmSpd;
			if(m_tick>1.0f){
				m_tick = 1.0f;
				m_state = CTOPTSS_Ready;
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

	return TRUE;
}


/*-----------------------------------------------------------
	�`��
-------------------------------------------------------------*/
void CTOptionSelecterBase::Draw()
{
	if(!m_selecter)return;

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
	double tmpx = g_system.DrawBMPTextEx(offset_x,y,z,_T("--OPTIONS--"),color_option|alpha,1.0f,text_yscale*1.2f,SYSBMPTXT_PROP);
	_stprintf(tmp_str,_T("POINT:%d"),m_selecter->current_point);
	if(m_selecter->current_point > 0)okcolor = color_ok;
	else okcolor = color_ng;
	g_system.DrawBMPTextEx(tmpx+20,y,z,tmp_str,okcolor|alpha,1.0f,text_yscale*1.2f,SYSBMPTXT_PROP);

	y+=text_spacing*1.2f;

	//�ݒ薼�\��
	_stprintf(tmp_str,_T("PRESET:%s"),m_selecter->GetCurrentSetName());
	g_system.DrawBMPTextEx(offset_x,y,z,tmp_str,color_option|alpha,1.0f,text_yscale,SYSBMPTXT_PROP);

	y+=text_spacing;

	//���ڂ̕`��
	DWORD text_flag;
	k=0;
	DWORD color;
	DWORD setting_now = m_selecter->GetSettings();
	BOOL not_available;
	for(ite=m_selecter->list->begin();ite!=m_selecter->list->end();ite++){
		not_available = FALSE;
		//�F�͉��F�H
		if(ite->flag & ex_flag){//�����t���O�A��
			if(k!=m_selecter->current_selected)color=color_collis|alpha;
			else color=color_collis_sel|alpha;
			not_available = TRUE;
		}
		else if(m_selecter->enabled[k]){//�L�����
			if(k!=m_selecter->current_selected)color=color_enable|alpha;
			else color=color_enable_sel|alpha;
		}
		else {//�������
			if((ite->depends&setting_now)!=ite->depends){//depends�t���O������Ȃ�
				if(k!=m_selecter->current_selected)color=color_na2|alpha;
				else color=color_na2_sel|alpha;
				not_available = TRUE;
			}
			else if(m_selecter->current_point<0 || m_selecter->current_point < ite->point){//ON�ɂ���ƃ|�C���g����Ȃ�
				if(k!=m_selecter->current_selected)color=color_pointb|alpha;
				else color=color_pointb_sel|alpha;
			}
			else{//ON�ɂ��Ă��|�C���g�͑����
				if(k!=m_selecter->current_selected)color=color_disable|alpha;
				else color=color_disable_sel|alpha;
			}
		}
		//�`��
		text_flag = SYSBMPTXT_PROP;
		if(k==m_selecter->current_selected)text_flag |= SYSBMPTXT_SHADE_B;
			g_system.DrawBMPTextEx(offset_x,y,z,ite->name,color,text_xscale,text_yscale,text_flag);
		if(not_available)
			g_system.DrawBMPTextEx(offset_x+320,y,z,_T("N/A"),color,text_xscale,text_yscale,text_flag);
		else if(m_selecter->enabled[k])
			g_system.DrawBMPTextEx(offset_x+320,y,z,_T("ON"),color,text_xscale,text_yscale,text_flag);
		else
			g_system.DrawBMPTextEx(offset_x+320,y,z,_T("OFF"),color,text_xscale,text_yscale,text_flag);
		y+=text_spacing;
		k++;
	}

	//--OK--
	if(m_selecter->commit_enabled){
		if(m_selecter->current_selected!=m_selecter->list->size())
			g_system.DrawBMPTextEx(offset_x+150,y,z,_T("--OK--"),color_ok|alpha,text_xscale,text_yscale,SYSBMPTXT_PROP);
		else
			g_system.DrawBMPTextEx(offset_x+150,y,z,_T("--OK--"),color_ok_sel|alpha,text_xscale,text_yscale,SYSBMPTXT_PROP|SYSBMPTXT_SHADE_B);
	}
	else{
		if(g_config.IsLimiterCut())
		{
			if(m_selecter->current_selected!=m_selecter->list->size())
				g_system.DrawBMPTextEx(offset_x+150,y,z,_T("-LIMITER CUT-"),color_ng|alpha,text_xscale,text_yscale,SYSBMPTXT_PROP);
			else
				g_system.DrawBMPTextEx(offset_x+150,y,z,_T("-LIMITER CUT-"),color_ng_sel|alpha,text_xscale,text_yscale,SYSBMPTXT_PROP|SYSBMPTXT_SHADE_B);
		}
		else
		{
			if(m_selecter->current_selected!=m_selecter->list->size())
				g_system.DrawBMPTextEx(offset_x+150,y,z,_T("--OVER--"),color_ng|alpha,text_xscale,text_yscale,SYSBMPTXT_PROP);
			else
				g_system.DrawBMPTextEx(offset_x+150,y,z,_T("--OVER--"),color_ng_sel|alpha,text_xscale,text_yscale,SYSBMPTXT_PROP|SYSBMPTXT_SHADE_B);
		}
	}

	delete [] tmp_str;

	g_draw.EnableZ();
}


/*-----------------------------------------------------------
	����E�ݒ�
-------------------------------------------------------------*/

int CTOptionSelecterBase::SetAndShow(DWORD cindex,DWORD keyindex)
{
	if(m_selecter)return -1;

	m_cindex = cindex;
	m_keyIndex = keyindex;
	m_selecter = g_charlist.GetOptionSelecter(cindex);

	if(m_selecter){
		m_state = CTOPTSS_NotReady;
		m_tick = 0.0f;
		int optnum = m_selecter->list->size();
		if(optnum<2)m_fontheight = 20.0f;
		else if(optnum<20)m_fontheight = 20.0f - (20.0f-8.0f)*((float)optnum-2)/18.0f ;
		else m_fontheight = 8.0f;
	}
	else return -1;

	return m_selecter->list->size();
}

float CTOptionSelecterBase::GetHeight()
{
	return (m_selecter->list->size()+2)*(m_fontheight+1.0f) + (m_fontheight*1.2f) ;
}



