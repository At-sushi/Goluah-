// goluah.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "goluah.h"


// CgoluahApp

BEGIN_MESSAGE_MAP(CgoluahApp, CWinApp)
END_MESSAGE_MAP()

extern INT gOldEntryPoint(HINSTANCE hInst);

// CgoluahApp コンストラクション

CgoluahApp::CgoluahApp()
{
	//new TCHAR[774];

	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
}


// 唯一の CgoluahApp オブジェクトです。

CgoluahApp theApp;

// CgoluahApp 初期化

BOOL CgoluahApp::InitInstance()
{
	#ifdef ARRAYBOUNDARY_DEBUG
	afxMemDF |= checkAlwaysMemDF;
	#endif

	CWinApp::InitInstance();

	BOOL ret = gOldEntryPoint(m_hInstance);

	//_CrtDumpMemoryLeaks();この位置では、staticなオブジェクトがもつメモリがリークとしてレポートされる
	return ret;
}




