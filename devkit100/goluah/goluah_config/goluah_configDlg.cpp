// goluah_configDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "goluah_config.h"
#include "goluah_configDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "DlgPage1.h"
#include "DlgPage2.h"
#include "DlgPage3.h"
#include "DlgOther.h"


IMPLEMENT_DYNAMIC(CCfgDialogBase,CDialog)
CCfgDialogBase::CCfgDialogBase(UINT tn,CWnd* p) : CDialog(tn,p)
{
}

// Cgoluah_configDlg ダイアログ



Cgoluah_configDlg::Cgoluah_configDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cgoluah_configDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	for(int i=0;i<MAX_TAB_PAGES;i++){
		m_tab_pages[i]=NULL;
	}
}

void Cgoluah_configDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

#define ON_WM_PAINT() \
	{ WM_PAINT, 0, 0, 0, AfxSig_vv, \
		(AFX_PMSG)(AFX_PMSGW) \
		(static_cast< void (AFX_MSG_CALL CWnd::*)(void) > (&Cgoluah_configDlg::OnPaint)) },

#define ON_WM_QUERYDRAGICON() \
	{ WM_QUERYDRAGICON, 0, 0, 0, AfxSig_hv, \
		(AFX_PMSG)(AFX_PMSGW) \
		(static_cast< HCURSOR (AFX_MSG_CALL CWnd::*)(void) > (&Cgoluah_configDlg::OnQueryDragIcon)) },

#define ON_WM_DESTROY() \
	{ WM_DESTROY, 0, 0, 0, AfxSig_vv, \
		(AFX_PMSG)(AFX_PMSGW) \
		(static_cast< void (AFX_MSG_CALL CWnd::*)(void) > (&Cgoluah_configDlg::OnDestroy)) },

#define ON_WM_KEYDOWN() \
	{ WM_KEYDOWN, 0, 0, 0, AfxSig_vwww, \
		(AFX_PMSG)(AFX_PMSGW) \
		(static_cast< void (AFX_MSG_CALL CWnd::*)(UINT, UINT, UINT) > (&Cgoluah_configDlg::OnKeyDown)) },

BEGIN_MESSAGE_MAP(Cgoluah_configDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &Cgoluah_configDlg::OnTcnSelchangeTab1)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &Cgoluah_configDlg::OnBnClickedOk)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BUTTON_RESTORE_ALL, &Cgoluah_configDlg::OnBnClickedButtonRestoreAll)
END_MESSAGE_MAP()


// Cgoluah_configDlg メッセージ ハンドラ

BOOL Cgoluah_configDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	//■タブコントロールの項目生成
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB1);
	pTab->InsertItem(0,"描画設定");
	pTab->InsertItem(1,"ゲーム設定");
	pTab->InsertItem(2,"システム設定");
	pTab->InsertItem(3,"システム設定(2)");

	//■各タブのページに相当するダイアログを生成
	m_tab_pages[0] = new CDlgPage1();
	m_tab_pages[0]->Create(IDD_DIALOG_PAGE1,this);
	m_tab_pages[1] = new CDlgPage2();
	m_tab_pages[1]->Create(IDD_DIALOG_PAGE2,this);
	m_tab_pages[2] = new CDlgPage3();
	m_tab_pages[2]->Create(IDD_DIALOG_PAGE3,this);
	m_tab_pages[3] = new CDlgOther();
	m_tab_pages[3]->Create(IDD_DIALOG_PAGE4,this);

	//■ウィンドウをタブの位置に合うように移動
	CRect r;
	for(int i=0;i<MAX_TAB_PAGES;i++){
		if(m_tab_pages[i]){
			m_tab_pages[i]->GetClientRect(&r);
			r.OffsetRect(20,35);
			m_tab_pages[i]->MoveWindow(&r,FALSE);
		}
	}
	OnTcnSelchangeTab1(0,0);

	if(!g_config.readok1)
	{
		MessageBox("system/config.dat の読み込みに失敗しました","警告");
	}
	if(!g_config.readok2)
	{
		MessageBox("system/config2.dat の読み込みに失敗しました","警告");
	}
	
	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void Cgoluah_configDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR Cgoluah_configDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Cgoluah_configDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	if(pResult)
		*pResult = 0;

	if (pResult) *pResult = 0;
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB1);
	int		i = pTab->GetCurSel();
	for(int j=0;j<MAX_TAB_PAGES;j++){
		if(m_tab_pages[j]){
			m_tab_pages[j]->ShowWindow(i == j ? SW_SHOW : SW_HIDE);
		}
	}
}



void Cgoluah_configDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO : ここにメッセージ ハンドラ コードを追加します。
}

BOOL Cgoluah_configDlg::DestroyWindow()
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	//タブページ用ダイアログの破棄
	for(int i=0;i<MAX_TAB_PAGES;i++)
	{
		if(m_tab_pages[i]){
			m_tab_pages[i]->DestroyWindow();
			delete m_tab_pages[i];
			m_tab_pages[i] = NULL;
		}
	}
	return CDialog::DestroyWindow();
}

void Cgoluah_configDlg::OnBnClickedOk()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。

	//廃止した
	//Cancelデ同様の操作を行う
}

void Cgoluah_configDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void Cgoluah_configDlg::OnBnClickedButtonRestoreAll()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	g_config.SetDefaultConfig2();
	g_config.SetDefaultConfigAss();
	g_config.SetDefaultConfigKB();
	g_config.SetDefaultConfigPad();
	for(int i=0;i<MAX_TAB_PAGES;i++)
	{
		m_tab_pages[i]->Reset();
	}
}

void Cgoluah_configDlg::OnCancel()
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	for(int i=0;i<MAX_TAB_PAGES;i++)
	{
		if(m_tab_pages[i]){
			m_tab_pages[i]->UpdateSettings();
		}
	}

	BOOL fail=FALSE;
	if(! g_config.SaveConfig() ){
		MessageBox("ファイル、system/config.dat の保存に失敗しました","えらー");
		fail = TRUE;
	}
	if(! g_config.SaveConfig2() ){
		MessageBox("ファイル、system/config2.dat の保存に失敗しました","えらー");
		fail = FALSE;
	}

	if(!fail) CDialog::OnCancel();
}
