
/*=======================================================================================

	�I�v�V�����I��

=========================================================================================*/
#pragma once

#include "define_const.h"
#include "task_items.h"
#include "charlist.h"

/*!
*	@brief CTOptionSelecterBase �̏��
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
*	@brief �I�v�V�����I���x�[�X
*	@ingroup Tasks
*
*	�����I�ɌÂ��N���X�������A�f�[�^�̊Ǘ��͂���������Ă���B
*	�\�����������悭�������ĐV�N���X�ɂ����͗l�B
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

	//!�`��v���C�I���e�B
	int m_draw_priority;
	virtual int GetDrawPriority(){return m_state==CTOPTSS_HideComplete ? -1 : m_draw_priority;}
	virtual void SetPriority(int pri){m_draw_priority = pri;}

	//����E�ݒ�
	virtual int  SetAndShow(DWORD cindex,DWORD kindex);
	virtual void SetPos(float x,float y){offset_x=x;m_top=y;}
	virtual float GetHeight();
	DWORD State() { return m_state; }
	CCOptionSelecter* GetSelecter()	{ return m_selecter; }

protected:
	virtual void EndSelect() = 0;//!<�I���I�����̏���

protected:
	CTOptionSelecterState m_state;
	CCOptionSelecter *m_selecter;//!<�{�́H

	UINT  m_cindex;
	float m_tick;
	DWORD m_keyIndex;		//!<�L�[���̓\�[�X
	float m_fontheight;
	float m_top,offset_x;
};

