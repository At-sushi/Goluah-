
/*=======================================================================================

	オプション選択

=========================================================================================*/
#pragma once

#include "define_const.h"
#include "task_optselecter.h"
#include "task_select.h"

/*!
*	@brief オプション選択ベース(単純版)
*	@ingroup Tasks
*	@sa CTOptionSelecter
*
*	プリセットとカスタム指定
*	カスタム設定の際に内部でCTOptionSelecterを呼び出します。
*	カスタム設定を別クラスでやろうとして失敗した物。
*/
class CTSimpleOptionSelecter : public CTOptionSelecter
{
public:
	CTSimpleOptionSelecter();
	virtual ~CTSimpleOptionSelecter(){}

	virtual BOOL Execute(DWORD time);
	virtual void Draw();

	//操作・設定
	virtual int  SetAndShow(DWORD cindex,DWORD kindex);
	virtual void SetPos(float x,float y)
	{
		m_customselect.SetPos(x, y);
		CTOptionSelecter::SetPos(x, y);
	}
	virtual float GetHeight();
	const CTOptionSelecter* GetCustomSelecter()	{ return &m_customselect; }	//!< OnOptionSelect向けの暫定措置

protected:
	virtual void EndSelect();//!<選択終了時の処理
	CCSimpleOptionSelecter* GetSelecter()	{ return dynamic_cast<CCSimpleOptionSelecter*>(m_selecter); }

protected:
	CTOptionSelecter m_customselect;	//!<カスタム選択に使うオプションセレクター
	BOOL  custom_mode;					//!<カスタム（Free）選択モードかどうか
};

