// are_editerDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "dx_draw.h"

#include "are_editer.h"
#include "are_editerDlg.h"
#include "DlgOther.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "DlgPage1.h"
#include "CelleditDlg.h"
#include "RecteditDlg.h"
#include "ChildWnd.h"
#include ".\are_editerdlg.h"

CDirectDraw g_draw;
//int g_DISPLAYWIDTH = 455;
//int g_DISPLAYHEIGHT = 495;
int g_DISPLAYWIDTH = 640;
int g_DISPLAYHEIGHT = 480;

HWND ghwnd;//gcdhandlerが使用する

IMPLEMENT_DYNAMIC(CAre_editerDlg, CMyDialogTab)


/*----------------------------------------------------------------------
	構築
------------------------------------------------------------------------*/
CAre_editerDlg::CAre_editerDlg(CWnd* pParent /*=NULL*/)
	: CMyDialogTab(CAre_editerDlg::IDD, pParent)
	, m_status(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	for(int i=0;i<MAX_TAB_PAGES;i++){
		m_tab_pages[i]=NULL;
	}
}

/*----------------------------------------------------------------------
	DDX
------------------------------------------------------------------------*/
void CAre_editerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STR_MAIN_STATUS, m_status);
}


/*----------------------------------------------------------------------
	メッセージマップ
------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CAre_editerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnTcnSelchangeTab1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


/*----------------------------------------------------------------------
	初期化
------------------------------------------------------------------------*/
BOOL CAre_editerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	ghwnd = m_hWnd;

	//■タブコントロールの項目生成
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB1);
	pTab->InsertItem(0,"ファイル");
	pTab->InsertItem(1,"矩形編集");
	pTab->InsertItem(2,"CELL編集");
	pTab->InsertItem(3,"その他");

	//■各タブのページに相当するダイアログを生成
	m_tab_pages[0] = new CDlgPage1();
	m_tab_pages[0]->Create(IDD_DIALOG_TAB1,this);
	m_tab_pages[1] = new CRecteditDlg();
	m_tab_pages[1]->Create(IDD_DIALOG_SUBTAB_RECT,this);
	m_tab_pages[2] = new CCelleditDlg();
	m_tab_pages[2]->Create(IDD_DIALOG_SUBTAB_CELL,this);
	m_tab_pages[3] = new CDlgOther();
	m_tab_pages[3]->Create(IDD_DIALOG_SUBTAB_OTHER,this);

	//■ウィンドウをタブの位置に合うように移動
	CRect r;
	for(int i=0;i<MAX_TAB_PAGES;i++){
		if(m_tab_pages[i]){
			m_tab_pages[i]->GetClientRect(&r);
			r.OffsetRect(482+185,30);
			m_tab_pages[i]->MoveWindow(&r,FALSE);
		}
	}

	//■子ウインドウを生成
	RECT rect = {10,10,10+g_DISPLAYWIDTH,10+g_DISPLAYHEIGHT};
	g_childwnd.Create(
					NULL,
					"AreEditer_ChildWindow",
					WS_CHILD | WS_VSCROLL | WS_HSCROLL,
					rect,
					this,
					1234	//ID なんでもいいの？
					);
	g_childwnd.ShowWindow(TRUE);

	//■Direct3D	初期化
	g_draw.Initialize(g_childwnd.m_hWnd,TRUE);

	OnTcnSelchangeTab1(0,0);

	//■コンボボックスの値とかを初期化
	UpdateItems();
	
	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}



/*----------------------------------------------------------------------
// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。
	・・・だとさ
------------------------------------------------------------------------*/
void CAre_editerDlg::OnPaint() 
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


/*----------------------------------------------------------------------
	//ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
	//  システムがこの関数を呼び出します。
	・・・だとさ
------------------------------------------------------------------------*/
HCURSOR CAre_editerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*-----------------------------------------------------------------------
	タブコントロールの変更
-------------------------------------------------------------------------*/
void CAre_editerDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
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

	if(i==1){
		g_childwnd.ChangeEditMode(FALSE);//矩形編集モードに変更
		SetStatus("矩形編集モード");
	}
	if(i==2){
		g_childwnd.ChangeEditMode(TRUE);//セル編集モードに変更
		SetStatus("セル編集モード");
	}
}

/*-----------------------------------------------------------------------
	ウインドウ破棄
-------------------------------------------------------------------------*/
BOOL CAre_editerDlg::DestroyWindow()
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
	//子ウインドウの破棄
	g_childwnd.DestroyWindow();

	return CDialog::DestroyWindow();
}

/*-----------------------------------------------------------------------
	下のほうにでてるエセステータスの文字列更新
-------------------------------------------------------------------------*/
void CAre_editerDlg::SetStatus(CString str)
{
	m_status = str;
	UpdateData(FALSE);
}


/*-----------------------------------------------------------------------
	ビットマップファイル名取得
-------------------------------------------------------------------------*/
CString CAre_editerDlg::GetBMPFilename(UINT i)
{
	return ((CDlgPage1*)m_tab_pages[0])->GetBMPFilename(i);
}


/*-----------------------------------------------------------------------
	下層タブコントロールに情報の更新を要求
-------------------------------------------------------------------------*/
void CAre_editerDlg::UpdateItems()
{
	for(int i=0;i<MAX_TAB_PAGES;i++)
	{
		if(m_tab_pages[i]){
			m_tab_pages[i]->UpdateItems();
		}
	}
}

/*----------------------------------------------------------------------
	ファイルドロップ
------------------------------------------------------------------------*/
void CAre_editerDlg::OnDropFiles(HDROP hDropInfo)
{
	char *pBuf = new char[MAX_PATH];
	UINT fileCount = ::DragQueryFile(  hDropInfo, 
					0,    
					pBuf, 
					MAX_PATH);

	CString str;
	str.Format("%s\nを読み込みますか？",pBuf);

	if(IDYES!=MessageBox(str,"",MB_YESNO)){
		delete [] pBuf;
		return;
	}

	theApp.LoadGCD(pBuf);
	delete [] pBuf;
}


/*----------------------------------------------------------------------
	ファイルドロップ
------------------------------------------------------------------------*/
void CAre_editerDlg::SpecialCtrl(UINT key)
{
	switch(key){
		case 0://blank next (rect)
			{
				((CRecteditDlg*)m_tab_pages[1])->BlankNext();
				theApp.InvalidateAll();
			}
			break;
		case 1://blank next (cell)
			{
				((CCelleditDlg*)m_tab_pages[2])->BlankNext();
				theApp.InvalidateAll();
			}
			break;
	}
}



void CAre_editerDlg::OnClose()
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	if (AfxMessageBox("終了するとですか？", MB_YESNO | MB_ICONQUESTION, NULL) == IDYES)
		CMyDialogTab::OnClose();
}
