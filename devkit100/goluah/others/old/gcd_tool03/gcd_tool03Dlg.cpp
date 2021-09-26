// gcd_tool03Dlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "gcd_tool03.h"
#include "gcd_tool03Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND ghwnd = NULL;
extern Cgcd_tool03App theApp;

// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog {
public:
  CAboutDlg();

  // ダイアログ データ
  enum { IDD = IDD_ABOUTBOX };

protected:
  virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV サポート

  // 実装
protected:
  DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD) {}

void CAboutDlg::DoDataExchange(CDataExchange *pDX) { CDialog::DoDataExchange(pDX); }

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// Cgcd_tool03Dlg ダイアログ

Cgcd_tool03Dlg::Cgcd_tool03Dlg(CWnd *pParent /*=NULL*/)
    : CDialog(Cgcd_tool03Dlg::IDD, pParent), m_str_status(_T("")), m_bCheck1(FALSE), m_edit_number1(0) {
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cgcd_tool03Dlg::DoDataExchange(CDataExchange *pDX) {
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_STATUS, m_str_status);
  DDX_Check(pDX, IDC_CHECK1, m_bCheck1);
  DDX_Text(pDX, IDC_EDIT1, m_edit_number1);
}

BEGIN_MESSAGE_MAP(Cgcd_tool03Dlg, CDialog)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_WM_DROPFILES()
//}}AFX_MSG_MAP
ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedButton7)
END_MESSAGE_MAP()

// Cgcd_tool03Dlg メッセージ ハンドラ

BOOL Cgcd_tool03Dlg::OnInitDialog() {
  CDialog::OnInitDialog();

  // "バージョン情報..." メニューをシステム メニューに追加します。

  // IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT(IDM_ABOUTBOX < 0xF000);

  CMenu *pSysMenu = GetSystemMenu(FALSE);
  if (pSysMenu != NULL) {
    CString strAboutMenu;
    strAboutMenu.LoadString(IDS_ABOUTBOX);
    if (!strAboutMenu.IsEmpty()) {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
    }
  }

  // このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
  //  Framework は、この設定を自動的に行います。
  SetIcon(m_hIcon, TRUE);  // 大きいアイコンの設定
  SetIcon(m_hIcon, FALSE); // 小さいアイコンの設定

  // TODO: 初期化をここに追加します。
  ghwnd = m_hWnd;
  m_str_status = "ファイルはドラッグ&ドロップでロードだよ";
  m_bCheck1 = TRUE;
  UpdateData(FALSE);

  return TRUE; // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void Cgcd_tool03Dlg::OnSysCommand(UINT nID, LPARAM lParam) {
  if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
  } else {
    CDialog::OnSysCommand(nID, lParam);
  }
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void Cgcd_tool03Dlg::OnPaint() {
  if (IsIconic()) {
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
  } else {
    CDialog::OnPaint();
  }
}

//ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR Cgcd_tool03Dlg::OnQueryDragIcon() { return static_cast<HCURSOR>(m_hIcon); }

/*--------------------------------------------------------------------------------
        ファイルのドロップ
----------------------------------------------------------------------------------*/
void Cgcd_tool03Dlg::OnDropFiles(HDROP hDropInfo) {
  char filename[1024];
  UINT fileCount = ::DragQueryFile(hDropInfo, 0, filename, 1024);
  theApp.Load(filename);
}

/*--------------------------------------------------------------------------------
        空矩形データの消去
----------------------------------------------------------------------------------*/
void Cgcd_tool03Dlg::OnBnClickedButton1() {
  // TODO : ここにコントロール通知ハンドラ コードを追加します。
  theApp.DeleteEmptyRect();
}

/*--------------------------------------------------------------------------------
        矩形番号の整理
----------------------------------------------------------------------------------*/
void Cgcd_tool03Dlg::OnBnClickedButton2() {
  // TODO : ここにコントロール通知ハンドラ コードを追加します。
  theApp.SortRects();
}

/*--------------------------------------------------------------------------------
        セル番号の整理
----------------------------------------------------------------------------------*/
void Cgcd_tool03Dlg::OnBnClickedButton3() {
  // TODO : ここにコントロール通知ハンドラ コードを追加します。
  UpdateData(TRUE);
  theApp.SortCells(m_bCheck1);
}

/*--------------------------------------------------------------------------------
        保存
----------------------------------------------------------------------------------*/
void Cgcd_tool03Dlg::OnBnClickedButton4() {
  // TODO : ここにコントロール通知ハンドラ コードを追加します。
  theApp.Save();
}

/*--------------------------------------------------------------------------------
        空矩形しか参照していないセルを削除
----------------------------------------------------------------------------------*/
void Cgcd_tool03Dlg::OnBnClickedButton5() {
  // TODO : ここにコントロール通知ハンドラ コードを追加します。
  theApp.DeleteEmptyCell();
}

/*--------------------------------------------------------------------------------
        指定番号に空セルを追加する
----------------------------------------------------------------------------------*/
void Cgcd_tool03Dlg::OnBnClickedButton6() {
  // TODO : ここにコントロール通知ハンドラ コードを追加します。
  UpdateData(TRUE);

  if (m_edit_number1 > GCDMAX_CELLS) {
    m_str_status = "入力された値が不正です";
  } else {
    if (theApp.InsertEmptyCell(m_edit_number1)) {
      m_str_status.Format("%d番に空のセルを挿入しました", m_edit_number1);
    } else
      m_str_status = "操作に失敗しました";
  }

  UpdateData(FALSE);
}

/*--------------------------------------------------------------------------------
        色レシオを1.0にする
----------------------------------------------------------------------------------*/
void Cgcd_tool03Dlg::OnBnClickedButton7() {
  // TODO : ここにコントロール通知ハンドラ コードを追加します。
  theApp.ForceRatio();
}
