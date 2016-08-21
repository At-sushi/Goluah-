// are_editer.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "dx_draw.h"
#include "gcdhandler.h"
#include "resource.h"		// メイン シンボル
#include <atlimage.h>

/*-------------------------------------------------------------------------
// CAre_editerApp:
// このクラスの実装については、are_editer.cpp を参照してください。
//・・・だとさ
---------------------------------------------------------------------------*/
class CAre_editerApp : public CWinApp
{
public:
	CAre_editerApp();

	int ReloadBitmaps();

	void LoadGCD(CString filename);
//	void SaveGCD070(CString filename);
//	void SaveGCD090(CString filename);

	GCD_RECT* GetCurrentRect();
	GCD_CELL2* GetCurrentCell();
	GCD_HANTEI* GetCurrentHantei();
	CGCDHandler* GetGCD(){return &m_gcd;}

	void InvalidateAll();
	void SetStatus(CString str);
	void SpecialCtrl(UINT key);

	MYSURFACE*  GetBitmap(UINT no){return m_bmps[no];}
	MYSURFACE** GetBitmaps(){return m_bmps;}
	CImage& GetImage(UINT no){return m_images[no];}
	CString GetBMPFilename(UINT n);

	void SetCurrentRectNOPointer(DWORD* p){m_pCurrentRectNO=p;}
	void SetCurrentCellNOPointer(DWORD* p){m_pCurrentCellNO=p;}
	UINT GetCurrentCellNO(){return *m_pCurrentCellNO;}
	void SetCurrentCellRectNOPointer(DWORD* p){m_pCurrentCellRectNO=p;}
	UINT GetCurrentCellRectNO(){return *m_pCurrentCellRectNO;}

	//r-tools
	void InsertEmptyRect(UINT index);	//指定位置に空の矩形を挿入
	void DeleteInvalidRect();			//ビットマップ上の何もない領域を参照している矩形を消去する
	void DeleteEmptyRect();				//空の矩形を消去して番号を詰める

	//c-tools
	void InsertEmptyCell(UINT index);	//指定位置に空きセルを挿入
	void DeleteInvalidCell();			//空の矩形しか参照していないセルを消去する
	void DeleteEmptyCell(BOOL under100);//空のCELLを消去して番号を詰める

protected:
	CGCDHandler m_gcd;
	MYSURFACE* m_bmps[GCDMAX_IMAGES];
	CImage m_images[GCDMAX_IMAGES];
	CString m_current_dir;
	DWORD *m_pCurrentRectNO;
	DWORD *m_pCurrentCellNO;
	DWORD *m_pCurrentCellRectNO;

// オーバーライド
	public:
	virtual BOOL InitInstance();

// 実装
	DECLARE_MESSAGE_MAP()
};

extern CAre_editerApp theApp;



/*-------------------------------------------------------------------------
	共通ダイアログベース
---------------------------------------------------------------------------*/
class CMyDialogTab : public CDialog
{
public:

	DECLARE_DYNAMIC(CMyDialogTab)//なんじゃこりゃ？

	CMyDialogTab(UINT nIDTemplate,CWnd* pParentWnd = NULL); 
	virtual void UpdateItems()=0;
};



