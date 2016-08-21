// goluah.h : goluah アプリケーションのメイン ヘッダー ファイル
//
#pragma once

/*#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
*/
#include "../resource.h"       // メイン シンボル


/*!
*	@defgroup Application
*	@brief アプリケーション
*/

/*!
*	@ingroup Application
*	@brief  アプリケーションクラス
*	プロジェクトがMFCアプリになったためしょうがなくあるクラス。
*	InitInstanceで旧WinMainを呼んで終了
*/
class CgoluahApp : public CWinApp
{
public:
	CgoluahApp();


// オーバーライド
public:
	//!旧WinMainを呼んでいるだけ
	virtual BOOL InitInstance();

// 実装

public:
	DECLARE_MESSAGE_MAP()
};

extern CgoluahApp theApp;
