// CelleditDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "are_editer.h"
#include "CelleditDlg.h"
#include "ChildWnd.h"

#include "DlgPage2.h"
#include "DlgPage3.h"
#include "DlgPage4.h"
#include "DlgCellDisp.h"
#include "DlgCPageTool.h"

// CCelleditDlg ダイアログ

IMPLEMENT_DYNAMIC(CCelleditDlg, CMyDialogTab)
CCelleditDlg::CCelleditDlg(CWnd* pParent /*=NULL*/)
	: CMyDialogTab(CCelleditDlg::IDD, pParent)
{
	for(int i=0;i<MAX_TAB_PAGES_CELL;i++){
		m_tab_pages[i]=NULL;
	}
	m_rectIndex = 0;
	theApp.SetCurrentCellRectNOPointer( &m_rectIndex );
}

CCelleditDlg::~CCelleditDlg()
{
}

void CCelleditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCelleditDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CELL, OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CCelleditDlg メッセージ ハンドラ

/*--------------------------------------------------------------------
	ダイアログ初期化
----------------------------------------------------------------------*/
BOOL CCelleditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO :  ここに初期化を追加してください

	//■タブコントロールの項目生成
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_CELL);
	pTab->InsertItem(0,"全般");
	pTab->InsertItem(1,"マウス操作");
	pTab->InsertItem(2,"矩形操作");
	pTab->InsertItem(3,"表示");
	pTab->InsertItem(4,"tools");

	CDlgPage4 *pdlg4 = new CDlgPage4();
	pdlg4->SetNParent(this);

	//■各タブのページに相当するダイアログを生成
	m_tab_pages[0] = new CDlgPage2();
	m_tab_pages[0]->Create(IDD_DIALOG_TAB2,this);
	m_tab_pages[1] = new CDlgPage3();
	m_tab_pages[1]->Create(IDD_DIALOG_TAB3,this);
	m_tab_pages[2] = pdlg4;
	m_tab_pages[2]->Create(IDD_DIALOG_TAB4,this);
	m_tab_pages[3] = new CDlgCellDisp();
	m_tab_pages[3]->Create(IDD_DIALOG_CTAB_DISP,this);
	m_tab_pages[4] = new CDlgCPageTool();
	m_tab_pages[4]->Create(IDD_DIALOG_CTAB_TOOLS,this);

	//■ウィンドウをタブの位置に合うように移動
	CRect r;
	for(int i=0;i<MAX_TAB_PAGES_CELL;i++){
		if(m_tab_pages[i]){
			m_tab_pages[i]->GetClientRect(&r);
			r.OffsetRect(8,30);
			m_tab_pages[i]->MoveWindow(&r,FALSE);
		}
	}

	OnTcnSelchangeTab1(0,0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*--------------------------------------------------------------------
	タブコントロールの変更
----------------------------------------------------------------------*/
void CCelleditDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	if(pResult)
		*pResult = 0;

	if (pResult) *pResult = 0;
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_CELL);
	int		i = pTab->GetCurSel();
	for(int j=0;j<MAX_TAB_PAGES_CELL;j++){
		if(m_tab_pages[j]){
			m_tab_pages[j]->ShowWindow(i == j ? SW_SHOW : SW_HIDE);
		}
	}

	if(i==2)m_tab_pages[2]->UpdateItems();
}


/*--------------------------------------------------------------------
	子供ウインドウたちの破棄
----------------------------------------------------------------------*/
BOOL CCelleditDlg::DestroyWindow()
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	
	//タブページ用ダイアログの破棄
	for(int i=0;i<MAX_TAB_PAGES_CELL;i++)
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
	ダイアログ内表示物の更新
----------------------------------------------------------------------*/
void CCelleditDlg::UpdateItems()
{
	for(int i=0;i<MAX_TAB_PAGES_CELL;i++)
	{
		if(m_tab_pages[i]){
			m_tab_pages[i]->UpdateItems();
		}
	}
}

/*--------------------------------------------------------------------
	ダイアログ内表示物の更新
----------------------------------------------------------------------*/
void CCelleditDlg::OnChangeRectIndex(UINT idx)
{
	m_rectIndex = idx;
	theApp.InvalidateAll();
}

/*--------------------------------------------------------------------
	特殊操作
----------------------------------------------------------------------*/
void CCelleditDlg::BlankNext()
{
	((CDlgPage2*)m_tab_pages[0])->OnBnClickedButtonNextBlank();
}

