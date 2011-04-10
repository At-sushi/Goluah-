// gcd_tool03.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include <atlimage.h>
#include "resource.h"		// メイン シンボル
#include "gcdhandler.h"


// Cgcd_tool03App:
// このクラスの実装については、gcd_tool03.cpp を参照してください。
//

class Cgcd_tool03App : public CWinApp
{
public:
	Cgcd_tool03App();

// オーバーライド
	public:
	virtual BOOL InitInstance();

// 実装
	DECLARE_MESSAGE_MAP()


public:
	CGCDHandler m_gcd;
	CImage m_img[GCDMAX_IMAGES];

	void Load(char *filename);
	void Save(){m_gcd.Save();}
	void DeleteEmptyRect();
	void SortRects();
	void DeleteEmptyCell();
	void SortCells(BOOL over100);
	BOOL InsertEmptyCell(UINT index);
	void ForceRatio();

	struct CHAR64
	{
		char name[64];
	};
};

extern Cgcd_tool03App theApp;
