// goluah_config.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル


// Cgoluah_configApp:
// このクラスの実装については、goluah_config.cpp を参照してください。
//

class Cgoluah_configApp : public CWinApp
{
public:
	Cgoluah_configApp();

// オーバーライド
	public:
	virtual BOOL InitInstance();

// 実装

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

#include "d3d8.h"
#include "config.h"
