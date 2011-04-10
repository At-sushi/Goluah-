
/*=======================================================================================

	オプション選択

=========================================================================================*/
#pragma once

#include "define_const.h"
#include "task_items.h"
#include "charlist.h"

/*!
*	@brief CTOptionSelecterBase の状態
*	@ingroup Tasks
*/
enum CTOptionSelecterState
{
	CTOPTSS_NotReady,
	CTOPTSS_Ready,
	CTOPTSS_Hide,
	CTOPTSS_HideComplete
};

/*!
*	@brief オプション選択ベース
*	@ingroup Tasks
*
*	内部的に古いクラスをもち、データの管理はそいつがやっている。
*	表示をかっこよくしたくて新クラスにした模様。
*/
class CTOptionSelecterBase : public CTaskBase
{
public:
	CTOptionSelecterBase();
	virtual ~CTOptionSelecterBase(){}

	virtual void Initialize();
	virtual void Terminate();
	virtual BOOL Execute(DWORD time);
	virtual void Draw();
	virtual void SetInitialValue(DWORD val){m_selecter->Initialize(val);}

	//!描画プライオリティ
	int m_draw_priority;
	virtual int GetDrawPriority(){return m_state==CTOPTSS_HideComplete ? -1 : m_draw_priority;}
	virtual void SetPriority(int pri){m_draw_priority = pri;}

	//操作・設定
	virtual int  SetAndShow(DWORD cindex,DWORD kindex);
	virtual void SetPos(float x,float y){offset_x=x;m_top=y;}
	virtual float GetHeight();
	DWORD State() { return m_state; }
	CCOptionSelecter* GetSelecter()	{ return m_selecter; }

protected:
	virtual void EndSelect() = 0;//!<選択終了時の処理

protected:
	CTOptionSelecterState m_state;
	CCOptionSelecter *m_selecter;//!<本体？

	UINT  m_cindex;
	float m_tick;
	DWORD m_keyIndex;		//!<キー入力ソース
	float m_fontheight;
	float m_top,offset_x;
};

