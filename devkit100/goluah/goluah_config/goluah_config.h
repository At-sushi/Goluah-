// goluah_config.h : PROJECT_NAME �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��


// Cgoluah_configApp:
// ���̃N���X�̎����ɂ��ẮAgoluah_config.cpp ���Q�Ƃ��Ă��������B
//

class Cgoluah_configApp : public CWinApp
{
public:
	Cgoluah_configApp();

// �I�[�o�[���C�h
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern Cgoluah_configApp theApp;

class CCfgDialogBase : public CDialog
{
public:
	DECLARE_DYNAMIC(CCfgDialogBase)

	virtual void Reset() = 0;

	CCfgDialogBase(UINT tn,CWnd* p);
	virtual void UpdateSettings() = 0;
};

#include "d3d9.h"
#include "config.h"
