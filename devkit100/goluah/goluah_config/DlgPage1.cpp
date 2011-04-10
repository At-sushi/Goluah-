// DlgPage1.cpp : 実装ファイル
//

#include "stdafx.h"
#include "goluah_config.h"
#include "DlgPage1.h"
#include ".\dlgpage1.h"


// CDlgPage1 ダイアログ

IMPLEMENT_DYNAMIC(CDlgPage1, CCfgDialogBase)
CDlgPage1::CDlgPage1(CWnd* pParent /*=NULL*/)
	: CCfgDialogBase(CDlgPage1::IDD, pParent)
{
}

CDlgPage1::~CDlgPage1()
{
}

void CDlgPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_FULL_SCREEN, ck_full_screen);
	DDX_Check(pDX, IDC_CHECK_HALFMODE, ck_halfmode);
	DDX_Check(pDX, IDC_CHECK_ALPHATEST, ck_noalphatest);
}

BEGIN_MESSAGE_MAP(CDlgPage1, CCfgDialogBase)
END_MESSAGE_MAP()

/*------------------------------------------------------------------
	設定→ダイアログアイテム
--------------------------------------------------------------------*/
BOOL CDlgPage1::OnInitDialog()
{
	CCfgDialogBase::OnInitDialog();

	// TODO :  ここに初期化を追加してください
	Reset();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CDlgPage1::Reset()
{
	ck_full_screen	= g_config.IsFullScreen();
	ck_halfmode		= g_config.IsHalfMode();
	ck_noalphatest	= g_config.NoAlphaTest();
	UpdateData(FALSE);
}

/*------------------------------------------------------------------
	設定←ダイアログアイテム
--------------------------------------------------------------------*/
void CDlgPage1::UpdateSettings()
{
	UpdateData(TRUE);

	g_config.SetFullScreen(ck_full_screen);
	g_config.SetHalfMode(ck_halfmode);
	g_config.SetNoAlphaTest(ck_noalphatest);
}


//void CDlgPage1::OnClose()
//{
//	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
//
//	CCfgDialogBase::OnClose();
//}
