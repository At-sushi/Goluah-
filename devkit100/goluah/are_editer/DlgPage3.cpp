#include "..\goluah_config\dlgpage3.h"
#include "..\goluah_config\dlgpage3.h"
#include "..\goluah_config\dlgpage3.h"
// DlgPage3.cpp : 実装ファイル
//

#include "stdafx.h"
#include "are_editer.h"
#include "DlgPage3.h"
#include "ChildWnd.h"


// CDlgPage3 ダイアログ

IMPLEMENT_DYNAMIC(CDlgPage3, CDialog)
CDlgPage3::CDlgPage3(CWnd* pParent /*=NULL*/)
	: CMyDialogTab(CDlgPage3::IDD, pParent)
	, m_str_info1(_T(""))
{
	m_mode = 7;
}

CDlgPage3::~CDlgPage3()
{
}

void CDlgPage3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INFO1, m_str_info1);
}


BEGIN_MESSAGE_MAP(CDlgPage3, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_BUTTON_RECT_KAS1, OnBnClickedButtonRectKas1)
	ON_BN_CLICKED(IDC_BUTTON_RECT_KAS2, OnBnClickedButtonRectKas2)
	ON_BN_CLICKED(IDC_BUTTON_RECT_KAS3, OnBnClickedButtonRectKas3)
	ON_BN_CLICKED(IDC_BUTTON_RECT_KUR1, OnBnClickedButtonRectKur1)
	ON_BN_CLICKED(IDC_BUTTON_RECT_KUR2, OnBnClickedButtonRectKur2)
	ON_BN_CLICKED(IDC_BUTTON_RECT_KUR3, OnBnClickedButtonRectKur3)
	ON_BN_CLICKED(IDC_BUTTON_RECT_ATK1, OnBnClickedButtonRectAtk1)
	ON_BN_CLICKED(IDC_BUTTON_RECT_ATK2, OnBnClickedButtonRectAtk2)
	ON_BN_CLICKED(IDC_BUTTON_RECT_ATK3, OnBnClickedButtonRectAtk3)
	ON_BN_CLICKED(IDC_RADIO6, OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO_NONE, OnBnClickedRadioNone)
END_MESSAGE_MAP()


// CDlgPage3 メッセージ ハンドラ





/*---------------------------------------------------------------
	ダイアログボックス初期化処理
-----------------------------------------------------------------*/
BOOL CDlgPage3::OnInitDialog()
{
	CMyDialogTab::OnInitDialog();

	// TODO :  ここに初期化を追加してください

	//ラジオボタン7にチェック入れる
	CButton *radio = (CButton*)GetDlgItem(IDC_RADIO_NONE);
	radio->SetCheck(1);
	m_mode = 0;
	EnableButtons(FALSE);//矩形指定ボタン無効

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*---------------------------------------------------------------
	「矩形決定」ボタン ON/OFF
-----------------------------------------------------------------*/
void CDlgPage3::EnableButtons(BOOL f)
{
	CWnd *w;

	w = GetDlgItem(IDC_BUTTON_RECT_KAS1);
	w->EnableWindow(f);
	w = GetDlgItem(IDC_BUTTON_RECT_KAS2);
	w->EnableWindow(f);
	w = GetDlgItem(IDC_BUTTON_RECT_KAS3);
	w->EnableWindow(f);
	
	w = GetDlgItem(IDC_BUTTON_RECT_KUR1);
	w->EnableWindow(f);
	w = GetDlgItem(IDC_BUTTON_RECT_KUR2);
	w->EnableWindow(f);
	w = GetDlgItem(IDC_BUTTON_RECT_KUR3);
	w->EnableWindow(f);

	w = GetDlgItem(IDC_BUTTON_RECT_ATK1);
	w->EnableWindow(f);
	w = GetDlgItem(IDC_BUTTON_RECT_ATK2);
	w->EnableWindow(f);
	w = GetDlgItem(IDC_BUTTON_RECT_ATK3);
	w->EnableWindow(f);

	UpdateInformation();
}


/*---------------------------------------------------------------
	ラジオボタン変更字処理
-----------------------------------------------------------------*/
//■オフセット
void CDlgPage3::OnBnClickedRadio1()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	EnableButtons(FALSE);
	m_mode = 0;
	g_childwnd.ChangeEditModeC(m_mode);
}

//■判定矩形
void CDlgPage3::OnBnClickedRadio2()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	EnableButtons(TRUE);
	m_mode = 1;
	g_childwnd.ChangeEditModeC(m_mode);
}

//■重心
void CDlgPage3::OnBnClickedRadio3()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	EnableButtons(FALSE);
	m_mode = 2;
	g_childwnd.ChangeEditModeC(m_mode);
}

//■回転
void CDlgPage3::OnBnClickedRadio4()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	EnableButtons(FALSE);
	m_mode = 3;
	g_childwnd.ChangeEditModeC(m_mode);
}

//■拡大縮小
void CDlgPage3::OnBnClickedRadio5()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	EnableButtons(FALSE);
	m_mode = 4;
	g_childwnd.ChangeEditModeC(m_mode);
}

//■オフセット（微調整）
void CDlgPage3::OnBnClickedRadio6()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	EnableButtons(FALSE);
	m_mode = 5;
	g_childwnd.ChangeEditModeC(m_mode);
}

//■マーカーポイント
void CDlgPage3::OnBnClickedRadio7()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	EnableButtons(FALSE);
	m_mode = 6;
	g_childwnd.ChangeEditModeC(m_mode);
}

//■none
void CDlgPage3::OnBnClickedRadioNone()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	EnableButtons(FALSE);
	m_mode = 7;
	g_childwnd.ChangeEditModeC(m_mode);
}

/*---------------------------------------------------------------
	表示物Update
-----------------------------------------------------------------*/
void CDlgPage3::UpdateItems()
{
	UpdateInformation();
}

/*---------------------------------------------------------------
	インフォメーション更新
-----------------------------------------------------------------*/
void CDlgPage3::UpdateInformation()
{
	CButton* rad = (CButton*)GetDlgItem(IDC_RADIO2);
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	
	if(rad->GetCheck()){
		m_str_info1.Format("あたり判定編集モード");
	}
	else if(pc){
		m_str_info1.Format("現在編集中の矩形：%d (%d)",
							theApp.GetCurrentCellRectNO(),
							pc->cell[ theApp.GetCurrentCellRectNO() ].cdr
							);
	}
	else{
		m_str_info1 = "エラー：セル情報の取得に失敗";
	}

	UpdateData(FALSE);
}


/*---------------------------------------------------------------
	矩形決定ボタン
-----------------------------------------------------------------*/
void CDlgPage3::OnBnClickedButtonRectKas1()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	GCD_HANTEI *ph = theApp.GetCurrentHantei();

	RECT r = g_childwnd.GetCurrentRect();
	ph->kas[0] = r;

	CString str;
	str.Format("重なり矩形(1)決定 : (%d,%d)-(%d,%d) , w:%d / h:%d",
		r.left,r.top,r.right,r.bottom, r.right-r.left , r.bottom-r.top );
	theApp.SetStatus(str);
	theApp.InvalidateAll();
}

void CDlgPage3::OnBnClickedButtonRectKas2()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	GCD_HANTEI *ph = theApp.GetCurrentHantei();

	RECT r = g_childwnd.GetCurrentRect();
	ph->kas[1] = r;

	CString str;
	str.Format("重なり矩形(2)決定 : (%d,%d)-(%d,%d) , w:%d / h:%d",
		r.left,r.top,r.right,r.bottom, r.right-r.left , r.bottom-r.top );
	theApp.SetStatus(str);
	theApp.InvalidateAll();
}

void CDlgPage3::OnBnClickedButtonRectKas3()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	GCD_HANTEI *ph = theApp.GetCurrentHantei();

	RECT r = g_childwnd.GetCurrentRect();
	ph->kas[2] = r;

	CString str;
	str.Format("重なり矩形(3)決定 : (%d,%d)-(%d,%d) , w:%d / h:%d",
		r.left,r.top,r.right,r.bottom, r.right-r.left , r.bottom-r.top );
	theApp.SetStatus(str);
	theApp.InvalidateAll();
}

void CDlgPage3::OnBnClickedButtonRectKur1()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	GCD_HANTEI *ph = theApp.GetCurrentHantei();

	RECT r = g_childwnd.GetCurrentRect();
	ph->kurai[0] = r;

	CString str;
	str.Format("喰らい矩形(1)決定 : (%d,%d)-(%d,%d) , w:%d / h:%d",
		r.left,r.top,r.right,r.bottom, r.right-r.left , r.bottom-r.top );
	theApp.SetStatus(str);
	theApp.InvalidateAll();
}

void CDlgPage3::OnBnClickedButtonRectKur2()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	GCD_HANTEI *ph = theApp.GetCurrentHantei();

	RECT r = g_childwnd.GetCurrentRect();
	ph->kurai[1] = r;

	CString str;
	str.Format("喰らい矩形(2)決定 : (%d,%d)-(%d,%d) , w:%d / h:%d",
		r.left,r.top,r.right,r.bottom, r.right-r.left , r.bottom-r.top );
	theApp.SetStatus(str);
	theApp.InvalidateAll();
}

void CDlgPage3::OnBnClickedButtonRectKur3()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	GCD_HANTEI *ph = theApp.GetCurrentHantei();

	RECT r = g_childwnd.GetCurrentRect();
	ph->kurai[2] = r;

	CString str;
	str.Format("喰らい矩形(3)決定 : (%d,%d)-(%d,%d) , w:%d / h:%d",
		r.left,r.top,r.right,r.bottom, r.right-r.left , r.bottom-r.top );
	theApp.SetStatus(str);
	theApp.InvalidateAll();
}

void CDlgPage3::OnBnClickedButtonRectAtk1()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	GCD_HANTEI *ph = theApp.GetCurrentHantei();

	RECT r = g_childwnd.GetCurrentRect();
	ph->attack[0] = r;

	CString str;
	str.Format("攻撃矩形(1)決定 : (%d,%d)-(%d,%d) , w:%d / h:%d",
		r.left,r.top,r.right,r.bottom, r.right-r.left , r.bottom-r.top );
	theApp.SetStatus(str);
	theApp.InvalidateAll();
}

void CDlgPage3::OnBnClickedButtonRectAtk2()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	GCD_HANTEI *ph = theApp.GetCurrentHantei();

	RECT r = g_childwnd.GetCurrentRect();
	ph->attack[1] = r;

	CString str;
	str.Format("攻撃矩形(2)決定 : (%d,%d)-(%d,%d) , w:%d / h:%d",
		r.left,r.top,r.right,r.bottom, r.right-r.left , r.bottom-r.top );
	theApp.SetStatus(str);
	theApp.InvalidateAll();
}

void CDlgPage3::OnBnClickedButtonRectAtk3()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	GCD_HANTEI *ph = theApp.GetCurrentHantei();

	RECT r = g_childwnd.GetCurrentRect();
	ph->attack[2] = r;

	CString str;
	str.Format("攻撃矩形(3)決定 : (%d,%d)-(%d,%d) , w:%d / h:%d",
		r.left,r.top,r.right,r.bottom, r.right-r.left , r.bottom-r.top );
	theApp.SetStatus(str);
	theApp.InvalidateAll();
}

