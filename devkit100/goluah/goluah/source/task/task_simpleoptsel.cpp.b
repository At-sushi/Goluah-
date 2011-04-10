
/*=======================================================================================

	オプション選択クラス

=========================================================================================*/

#include "stdafx.h"
#include "global.h"
#include "task_simpleoptsel.h"

/*-----------------------------------------------------------
	構築
-------------------------------------------------------------*/
CTSimpleOptionSelecter::CTSimpleOptionSelecter()
{
	m_selecter = NULL;
	m_state = CTOPTSS_HideComplete;
	superclass_mode = FALSE;
}


/*-----------------------------------------------------------
	初期化
-------------------------------------------------------------*/
void CTSimpleOptionSelecter::Initialize()
{
}


/*-----------------------------------------------------------
	破棄
-------------------------------------------------------------*/
void CTSimpleOptionSelecter::Terminate()
{
	if(m_selecter){
		delete m_selecter;
		m_selecter = NULL;
	}
}


/*-----------------------------------------------------------
	実行
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
				// プリセットがないので以前のモードで表示
				CTOptionSelecter::SetAndShow(m_cindex, m_keyIndex);
				CTOptionSelecter::SetPos(offset_x, m_top);
				superclass_mode = TRUE;
			}
		}break;
	case CTOPTSS_Ready:
		{
			if(m_keyIndex>=0){
				if(m_selecter->HandlePad(m_keyIndex))//TRUEが帰ると選択終了
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
	描画
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

	//排他されているフラグをゲット
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

/*	//テキスト色（非選択時）
	DWORD color_option		= 0x00222222;//--OPTIONS--表示
	DWORD color_ok			= 0x00666666;//ポイント・OK表示(有効時)
	DWORD color_ng			= 0x00AA3333;//ポイント・OK表示(無効時)
	DWORD color_collis		= 0x00336633;//競合フラグあり
	DWORD color_na2			= 0x00333366;//depend不足
	DWORD color_enable		= 0x00222222;//有効状態
	DWORD color_disable		= 0x00444444;//無効状態
	DWORD color_pointb		= 0x00663333;//ポイント不足
	//テキスト色（選択時）
	DWORD color_ok_sel		= 0x00000000;//ポイント・OK表示(有効時)
	DWORD color_ng_sel		= 0x00AA0000;//ポイント・OK表示(無効時)
	DWORD color_collis_sel	= 0x00009900;//競合フラグあり
	DWORD color_na2_sel		= 0x000000FF;//depend不足
	DWORD color_enable_sel	= 0x00222222;//有効状態
	DWORD color_disable_sel	= 0x00444444;//無効状態
	DWORD color_pointb_sel	= 0x00AA0000;//ポイント不足*/

	//テキスト色（非選択時）
	DWORD color_option		= 0x00444444;//--OPTIONS--表示
    DWORD color_ok			= 0x00444444;//ポイント・OK表示(有効時)
	DWORD color_ng			= 0x00AA3333;//ポイント・OK表示(無効時)
	DWORD color_collis		= 0x0033AA33;//競合フラグあり
	DWORD color_na2			= 0x0033AA33;//depend不足
	DWORD color_enable		= 0x00222222;//有効状態
	DWORD color_disable		= 0x004444AA;//無効状態
	DWORD color_pointb		= 0x00AA3333;//ポイント不足
	//テキスト色（選択時）
	DWORD color_ok_sel		= 0x00222222;//ポイント・OK表示(有効時)
	DWORD color_ng_sel		= 0x00AA0000;//ポイント・OK表示(無効時)
	DWORD color_collis_sel	= 0x0000AA00;//競合フラグあり
	DWORD color_na2_sel		= 0x0000AA00;//depend不足
	DWORD color_enable_sel	= 0x00222222;//有効状態
	DWORD color_disable_sel	= 0x000000AA;//無効状態
	DWORD color_pointb_sel	= 0x00AA0000;//ポイント不足

	float y=m_top,z=0;
	float text_xscale = 0.8f;
	float text_yscale = m_fontheight/30.0f;
	float text_spacing = m_fontheight + 1.0f;

	BYTE  _alpha = (BYTE)(m_tick * 255);
	DWORD alpha = _alpha << 24;

	//OPTIONS 描画
	DWORD okcolor;
	double tmpx = g_system.DrawBMPTextEx(offset_x,y,z,"--OPTION PRESETS--",color_option|alpha,1.0f,text_yscale*1.2f,SYSBMPTXT_PROP);
	sprintf(tmp_str,"POINT: %d",m_selecter->current_point);
	if(m_selecter->current_point > 0)okcolor = color_ok;
	else okcolor = color_ng;
	g_system.DrawBMPTextEx(tmpx+20,y,z,tmp_str,okcolor|alpha,1.0f,text_yscale*1.2f,SYSBMPTXT_PROP);

	y+=text_spacing*1.2f;

	//設定名表示
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
	操作・設定
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
		// CUSTOMが選択されたときにはモードを切り替える
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


