// RecteditDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "are_editer.h"
#include "RecteditDlg.h"

#include "DlgRPage1.h"
#include "DlgTPage2.h"

// CRecteditDlg ダイアログ

IMPLEMENT_DYNAMIC(CRecteditDlg, CMyDialogTab)
CRecteditDlg::CRecteditDlg(CWnd* pParent /*=NULL*/)
	: CMyDialogTab(CRecteditDlg::IDD, pParent)
{
	for(int i=0;i<MAX_TAB_PAGES_RECT;i++){
		m_tab_pages[i]=NULL;
	}
}

CRecteditDlg::~CRecteditDlg()
{
}

void CRecteditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRecteditDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_RECT, OnTcnSelchangeTabRect)
END_MESSAGE_MAP()


// CRecteditDlg メッセージ ハンドラ

/*--------------------------------------------------------------------
	ダイアログ初期化
----------------------------------------------------------------------*/
BOOL CRecteditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO :  ここに初期化を追加してください
	//■タブコントロールの項目生成
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_RECT);
	pTab->InsertItem(0,"全般");
	pTab->InsertItem(1,"tools");

	//■各タブのページに相当するダイアログを生成
	m_tab_pages[0] = new CDlgRPage1();
	m_tab_pages[0]->Create(IDD_DIALOG_RTAB1,this);
	m_tab_pages[1] = new CDlgTPage2();
	m_tab_pages[1]->Create(IDD_DIALOG_RTAB2,this);

	//■ウィンドウをタブの位置に合うように移動
	CRect r;
	for(int i=0;i<MAX_TAB_PAGES_RECT;i++){
		if(m_tab_pages[i]){
			m_tab_pages[i]->GetClientRect(&r);
			r.OffsetRect(8,30);
			m_tab_pages[i]->MoveWindow(&r,FALSE);
		}
	}

	OnTcnSelchangeTabRect(0,0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*--------------------------------------------------------------------
	タブコントロールの変更
----------------------------------------------------------------------*/
void CRecteditDlg::OnTcnSelchangeTabRect(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	if(pResult)
		*pResult = 0;

	if (pResult) *pResult = 0;
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_RECT);
	int		i = pTab->GetCurSel();
	for(int j=0;j<MAX_TAB_PAGES_RECT;j++){
		if(m_tab_pages[j])
		{
			m_tab_pages[j]->ShowWindow(i == j ? SW_SHOW : SW_HIDE);
		}
	}
}


/*--------------------------------------------------------------------
	子供ウインドウたちの破棄
----------------------------------------------------------------------*/
BOOL CRecteditDlg::DestroyWindow()
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//タブページ用ダイアログの破棄
	for(int i=0;i<MAX_TAB_PAGES_RECT;i++)
	{
		if(m_tab_pages[i]){
			m_tab_pages[i]->DestroyWindow();
			delete m_tab_pages[i];
			m_tab_pages[i] = NULL;
		}
	}

	return CDialog::DestroyWindow();
}


/*--------------------------------------------------------------------
	表示物更新
----------------------------------------------------------------------*/
void CRecteditDlg::UpdateItems()
{
	for(int i=0;i<MAX_TAB_PAGES_RECT;i++)
	{
		if(m_tab_pages[i]){
			m_tab_pages[i]->UpdateItems();
		}
	}
}

/*--------------------------------------------------------------------
	特殊操作
----------------------------------------------------------------------*/
void CRecteditDlg::BlankNext()
{
	((CDlgRPage1*)m_tab_pages[0])->OnBnClickedButtonNextBlank();
}
