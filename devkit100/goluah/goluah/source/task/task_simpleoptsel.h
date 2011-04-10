
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
*	�J�X�^���ݒ�̍ۂɓ�����CTOptionSelecter���Ăяo���܂��B
*	�J�X�^���ݒ��ʃN���X�ł�낤�Ƃ��Ď��s�������B
*/
class CTSimpleOptionSelecter : public CTOptionSelecter
{
public:
	CTSimpleOptionSelecter();
	virtual ~CTSimpleOptionSelecter(){}

	virtual BOOL Execute(DWORD time);
	virtual void Draw();

	//����E�ݒ�
	virtual int  SetAndShow(DWORD cindex,DWORD kindex);
	virtual void SetPos(float x,float y)
	{
		m_customselect.SetPos(x, y);
		CTOptionSelecter::SetPos(x, y);
	}
	virtual float GetHeight();
	const CTOptionSelecter* GetCustomSelecter()	{ return &m_customselect; }	//!< OnOptionSelect�����̎b��[�u

protected:
	virtual void EndSelect();//!<�I���I�����̏���
	CCSimpleOptionSelecter* GetSelecter()	{ return dynamic_cast<CCSimpleOptionSelecter*>(m_selecter); }

protected:
	CTOptionSelecter m_customselect;	//!<�J�X�^���I���Ɏg���I�v�V�����Z���N�^�[
	BOOL  custom_mode;					//!<�J�X�^���iFree�j�I�����[�h���ǂ���
};

