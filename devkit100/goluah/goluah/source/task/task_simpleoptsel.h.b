
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
*	カスタム設定の際に基底のCTOptionSelecterを呼び出します。
*
*	本当はカスタム用にもう１つ所有させたかったんですが、呼び出し機構の都合により、基底クラスの中身を再実装しつつ
*	基底クラスを別クラスであるかのように扱っています。
*	うーん、何とかならないものか…
*/
class CTSimpleOptionSelecter : public CTOptionSelecter
{
public:
	CTSimpleOptionSelecter();
	virtual ~CTSimpleOptionSelecter(){}

	virtual void Initialize();
	virtual void Terminate();
	virtual BOOL Execute(DWORD time);
	virtual void Draw();
	virtual void SetInitialValue(DWORD val){m_selecter->Initialize(val);}
	void ApplyToPreviousSelect(){m_selecter->ApplyToPreviousSelect();}

	//!描画プライオリティ
	int m_draw_priority;
	virtual int GetDrawPriority(){return m_state==CTOPTSS_HideComplete ? -1 : m_draw_priority;}
	virtual void SetPriority(int pri){m_draw_priority = pri;}

	//操作・設定
	virtual int  SetAndShow(DWORD cindex,DWORD kindex);
	virtual void SetPos(float x,float y){offset_x=x;m_top=y;}
	virtual float GetHeight();
	DWORD State() { return m_state; }

protected:
	virtual void EndSelect();//!<選択終了時の処理

protected:
	CTOptionSelecterState m_state;
	CCSimpleOptionSelecter *m_selecter;//!<本体？

	UINT  m_cindex;
	float m_tick;
	DWORD m_keyIndex;		//!<キー入力ソース
	float m_fontheight;
	float m_top,offset_x;
	BOOL  superclass_mode;	//!<カスタム（Free）選択モードかどうか
};

