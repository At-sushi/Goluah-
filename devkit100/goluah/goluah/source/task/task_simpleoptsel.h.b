
/*=======================================================================================

	�I�v�V�����I��

=========================================================================================*/
#pragma once

#include "define_const.h"
#include "task_optselecter.h"
#include "task_select.h"

/*!
*	@brief �I�v�V�����I���x�[�X(�P����)
*	@ingroup Tasks
*	@sa CTOptionSelecter
*
*	�v���Z�b�g�ƃJ�X�^���w��
*	�J�X�^���ݒ�̍ۂɊ���CTOptionSelecter���Ăяo���܂��B
*
*	�{���̓J�X�^���p�ɂ����P���L��������������ł����A�Ăяo���@�\�̓s���ɂ��A���N���X�̒��g���Ď�������
*	���N���X��ʃN���X�ł��邩�̂悤�Ɉ����Ă��܂��B
*	���[��A���Ƃ��Ȃ�Ȃ����̂��c
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

	//!�`��v���C�I���e�B
	int m_draw_priority;
	virtual int GetDrawPriority(){return m_state==CTOPTSS_HideComplete ? -1 : m_draw_priority;}
	virtual void SetPriority(int pri){m_draw_priority = pri;}

	//����E�ݒ�
	virtual int  SetAndShow(DWORD cindex,DWORD kindex);
	virtual void SetPos(float x,float y){offset_x=x;m_top=y;}
	virtual float GetHeight();
	DWORD State() { return m_state; }

protected:
	virtual void EndSelect();//!<�I���I�����̏���

protected:
	CTOptionSelecterState m_state;
	CCSimpleOptionSelecter *m_selecter;//!<�{�́H

	UINT  m_cindex;
	float m_tick;
	DWORD m_keyIndex;		//!<�L�[���̓\�[�X
	float m_fontheight;
	float m_top,offset_x;
	BOOL  superclass_mode;	//!<�J�X�^���iFree�j�I�����[�h���ǂ���
};

