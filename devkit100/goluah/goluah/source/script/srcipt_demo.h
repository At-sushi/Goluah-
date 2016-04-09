
/*!
*	@file
*	@brief �X�g�[���[���ԃf���X�N���v�g����(���g�p)
*
*	���ݎg���Ă���܂�����ۂ��B
*	ver0.8�܂ł̃f���X�N���v�g����蒼�����Ƃ��Ă��݂����B
*/
/*! @ingroup script*/
/*@{*/
#pragma once

#include "define_macro.h"
#include "script_loader.h"

class CDemoScriptElement;
typedef std::vector<CDemoScriptElement*> DScriptElementList;

/*!
*	@brief �X�g�[���[�f���ǂݍ��݁H
*
*	�f���X�N���v�g����蒼�����Ƃ��Ă��݂����B
*	���g�Ȃ�
*/
class CGoluahDemoScript
{
public:
	/*!
	*	@brief �ǂݍ��݊֐�
	*
	*	�f���X�N���v�g����蒼�����Ƃ��Ă��݂����B
	*	���g�Ȃ�
	*/
	static BOOL CreateScriptElementList(SScriptElementList& list,TCHAR *filename,CAliasList *args);
};

/*@}*/