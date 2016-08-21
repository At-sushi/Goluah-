// DlgKBKeyGet.cpp : 実装ファイル
//

#include "stdafx.h"
#include "goluah_config.h"
#include "DlgKBKeyGet.h"

CDlgKBKeyGet* CDlgKBKeyGet::m_instance=NULL;

// CDlgKBKeyGet ダイアログ

IMPLEMENT_DYNAMIC(CDlgKBKeyGet, CDialog)
CDlgKBKeyGet::CDlgKBKeyGet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgKBKeyGet::IDD, pParent)
{
	m_instance = this;
}

CDlgKBKeyGet::~CDlgKBKeyGet()
{
	m_instance = NULL;
}

void CDlgKBKeyGet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

#define ON_WM_KEYDOWN() \
	{ WM_KEYDOWN, 0, 0, 0, AfxSig_vwww, \
		(AFX_PMSG)(AFX_PMSGW) \
		(static_cast< void (AFX_MSG_CALL CWnd::*)(UINT, UINT, UINT) > (&CDlgKBKeyGet::OnKeyDown)) },

BEGIN_MESSAGE_MAP(CDlgKBKeyGet, CDialog)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CDlgKBKeyGet メッセージ ハンドラ

void CDlgKBKeyGet::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	EndDialog(0);
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CDlgKBKeyGet::PreTranslateMessage(MSG* pMsg)
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	
	if(pMsg->message == WM_KEYDOWN){
        if(pMsg->wParam != VK_ESCAPE)
		{
			BYTE code = (BYTE)pMsg->wParam;

			if(code==VK_SHIFT)
			{
				if(GetKeyState(VK_LSHIFT)&0x8000)code = VK_LSHIFT;
				if(GetKeyState(VK_RSHIFT)&0x8000)code = VK_RSHIFT;
			}
			if(code==VK_CONTROL)
			{
				if(GetKeyState(VK_LCONTROL)&0x8000)code = VK_LCONTROL;
				if(GetKeyState(VK_RCONTROL)&0x8000)code = VK_RCONTROL;
				
			}
			if(code==VK_MENU)
			{
				if(GetKeyState(VK_LMENU)&0x8000)code = VK_LMENU;
				if(GetKeyState(VK_RMENU)&0x8000)code = VK_RMENU;
				
			}

			EndDialog( code );
        }
    }

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgKBKeyGet::OnCancel()
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//CDialog::OnCancel();

	EndDialog(0);
}
