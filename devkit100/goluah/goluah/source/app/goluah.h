// goluah.h : goluah �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

/*#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
*/
#include "../resource.h"       // ���C�� �V���{��
#include "../../goluah_i.h"


/*!
*	@defgroup Application
*	@brief �A�v���P�[�V����
*/

/*!
*	@ingroup Application
*	@brief  �A�v���P�[�V�����N���X
*	�v���W�F�N�g��MFC�A�v���ɂȂ������߂��傤���Ȃ�����N���X�B
*	InitInstance�ŋ�WinMain���Ă�ŏI��
*/
class CgoluahApp : public CWinApp
{
public:
	CgoluahApp();


// �I�[�o�[���C�h
public:
	//!��WinMain���Ă�ł��邾��
	virtual BOOL InitInstance();

// ����

public:
	DECLARE_MESSAGE_MAP()
};

extern CgoluahApp theApp;
