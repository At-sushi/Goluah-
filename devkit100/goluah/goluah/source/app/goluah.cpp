// goluah.cpp : �A�v���P�[�V�����̃N���X������`���܂��B
//

#include "stdafx.h"
#include "goluah.h"


// CgoluahApp

BEGIN_MESSAGE_MAP(CgoluahApp, CWinApp)
END_MESSAGE_MAP()

extern INT gOldEntryPoint(HINSTANCE hInst);

// CgoluahApp �R���X�g���N�V����

CgoluahApp::CgoluahApp()
{
	//new TCHAR[774];

	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
}


// �B��� CgoluahApp �I�u�W�F�N�g�ł��B

CgoluahApp theApp;

// CgoluahApp ������

BOOL CgoluahApp::InitInstance()
{
	#ifdef ARRAYBOUNDARY_DEBUG
	afxMemDF |= checkAlwaysMemDF;
	#endif

	CWinApp::InitInstance();

	BOOL ret = gOldEntryPoint(m_hInstance);

	//_CrtDumpMemoryLeaks();���̈ʒu�ł́Astatic�ȃI�u�W�F�N�g���������������[�N�Ƃ��ă��|�[�g�����
	return ret;
}




