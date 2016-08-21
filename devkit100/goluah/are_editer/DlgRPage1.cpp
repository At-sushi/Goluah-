// DlgRPage1.cpp : 実装ファイル
//

#include "stdafx.h"
#include "are_editer.h"
#include "DlgRPage1.h"
#include "ChildWnd.h"


// CDlgRPage1 ダイアログ

IMPLEMENT_DYNAMIC(CDlgRPage1, CMyDialogTab)
CDlgRPage1::CDlgRPage1(CWnd* pParent /*=NULL*/)
	: CMyDialogTab(CDlgRPage1::IDD, pParent)
	, m_current_index(0)
	, m_radio_editmode(0)
	, m_str_info1(_T(""))
	, m_str_info2(_T(""))
	, m_selected_bmp(0)
{
	m_current_index = 1;
	m_selected_bmp = 0;
	theApp.SetCurrentRectNOPointer( &m_current_index );
}

CDlgRPage1::~CDlgRPage1()
{
}

void CDlgRPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_current_index);
	DDV_MinMaxUInt(pDX, m_current_index, 1, 1024);
	DDX_Text(pDX, IDC_STR_INFO1, m_str_info1);
	DDX_Text(pDX, IDC_STR_INFO2, m_str_info2);
	DDX_CBIndex(pDX, IDC_COMBO1, m_selected_bmp);
}


BEGIN_MESSAGE_MAP(CDlgRPage1, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON_PREV10, OnBnClickedButtonPrev10)
	ON_BN_CLICKED(IDC_BUTTON_NEXT10, OnBnClickedButtonNext10)
	ON_BN_CLICKED(IDC_BUTTON_PREV_FIND, OnBnClickedButtonPrevFind)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_FIND, OnBnClickedButtonNextFind)
	ON_BN_CLICKED(IDC_BUTTON_PREV_BLANK, OnBnClickedButtonPrevBlank)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_BLANK, OnBnClickedButtonNextBlank)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_SETCURRENTBMP, OnBnClickedButtonSetcurrentbmp)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
END_MESSAGE_MAP()


// CDlgRPage1 メッセージ ハンドラ

/*--------------------------------------------------------------
	矩形番号変更時処理
----------------------------------------------------------------*/
void CDlgRPage1::OnEnChangeEdit1()
{
	// TODO :  ここにコントロール通知ハンドラ コードを追加してください。
	DWORD prv = m_current_index;
	UpdateData(TRUE);

	if(m_current_index<1 || m_current_index>1023){
		m_current_index = prv;
		theApp.SetStatus("矩形番号に不正な値が入力された");
		return;
	}
	OnChangeIndex();
}

/*--------------------------------------------------------------
	矩形番号ヨコ、スピン処理
----------------------------------------------------------------*/
void CDlgRPage1::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	*pResult = 0;

	UINT next = m_current_index + pNMUpDown->iDelta;
	if(next<1 || next>1023){
		return;
	}
	m_current_index = next;
	OnChangeIndex();
}


/*--------------------------------------------------------------
	矩形番号移動ボタン処理
----------------------------------------------------------------*/
void CDlgRPage1::OnBnClickedButtonPrev10()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	if(m_current_index<11)m_current_index=1;
	else m_current_index-=10;

	OnChangeIndex();
}

void CDlgRPage1::OnBnClickedButtonNext10()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	m_current_index+=10;
	if(m_current_index>1023)m_current_index=1023;

	OnChangeIndex();
}

void CDlgRPage1::OnBnClickedButtonPrevFind()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	if(m_current_index==1){
		theApp.SetStatus("Find Previous : 行き止まり");
		return;
	}
	CGCDHandler* pdat = theApp.GetGCD();

	UINT next = m_current_index;
	next--;
	while(pdat->IsEmptyRect(next)){
		if(next==1){
			theApp.SetStatus("Find Previous : ありません");
			return;
		}
		next--;
	}
	m_current_index = next;
	OnChangeIndex();
}

void CDlgRPage1::OnBnClickedButtonNextFind()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	if(m_current_index==1023){
		theApp.SetStatus("Find Next : 行き止まりだぜ");
		return;
	}
	CGCDHandler* pdat = theApp.GetGCD();

	UINT next = m_current_index;
	next++;
	while(pdat->IsEmptyRect(next)){
		if(next==1023){
			theApp.SetStatus("Find Next : ありません");
			return;
		}
		next++;
	}
	m_current_index = next;
	OnChangeIndex();
}

void CDlgRPage1::OnBnClickedButtonPrevBlank()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	if(m_current_index==1){
		theApp.SetStatus("Blank Previous : 行き止まり");
		return;
	}
	CGCDHandler* pdat = theApp.GetGCD();

	UINT next = m_current_index;
	next--;
	while(!pdat->IsEmptyRect(next)){
		if(next==1){
			theApp.SetStatus("Blank Previous : ありません");
			return;
		}
		next--;
	}
	m_current_index = next;
	OnChangeIndex();
}

void CDlgRPage1::OnBnClickedButtonNextBlank()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	if(m_current_index==1023){
		theApp.SetStatus("Blank Next : 行き止まりだぜ");
		return;
	}
	CGCDHandler* pdat = theApp.GetGCD();

	UINT next = m_current_index;
	next++;
	while(!pdat->IsEmptyRect(next)){
		if(next==1023){
			theApp.SetStatus("Blank Next : ありません");
			return;
		}
		next++;
	}
	m_current_index = next;
	OnChangeIndex();
}


/*--------------------------------------------------------------
	[決定]	ボタン処理
----------------------------------------------------------------*/
void CDlgRPage1::OnBnClickedButtonApply()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	GCD_RECT *r = theApp.GetCurrentRect();
	if(!r){
		theApp.SetStatus("矩形決定処理失敗？？？");
		return;
	}

	//現在のビットマップ番号に設定
	UpdateData(TRUE);
	r->bmpno = m_selected_bmp;

	//矩形設定
	r->r = g_childwnd.GetCurrentRect();

	CString str;
	str.Format("矩形(%d)決定 : (%d,%d)-(%d,%d) w:%d / h:%d",
		m_current_index,
		r->r.left, r->r.top, r->r.right, r->r.bottom,
		r->r.right-r->r.left, r->r.bottom-r->r.top );
	theApp.SetStatus(str);

	theApp.InvalidateAll();
}

/*--------------------------------------------------------------
	[消去]	ボタン処理
----------------------------------------------------------------*/
void CDlgRPage1::OnBnClickedButton2()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	if(MessageBox("いいの？","矩形消去",MB_YESNO)!=IDYES)return;

	GCD_RECT *r = theApp.GetCurrentRect();
	if(!r){
		theApp.SetStatus("矩形消去処理失敗？？？");
		return;
	}
	CGCDHandler::GCDSetEmptyRect(r);

	CString str;
	str.Format("矩形(%d)消去",m_current_index);
	theApp.SetStatus(str);

	theApp.InvalidateAll();
}

/*--------------------------------------------------------------
	[現在の矩形が定義されているところに移動]	ボタン処理
----------------------------------------------------------------*/
void CDlgRPage1::OnBnClickedButtonSetcurrentbmp()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	GCD_RECT *r = theApp.GetCurrentRect();
	if(!r){
		theApp.SetStatus("現在の矩形が定義されているところに移動：矩形データ取得に失敗？？");
		return;
	}
	if(CGCDHandler::GCDIsEmptyRect(*r)){
		theApp.SetStatus("現在の矩形が定義されているところに移動：矩形データが空です");
		return;
	}

	//ビットマップ選択コンボボックスが狭いので広げる
	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_COMBO1);
	if(!combo){
		theApp.SetStatus("CDlgRPage1::OnInitDialog combo取得失敗？");
		return;
	}
	combo->SetCurSel( r->bmpno );
	g_childwnd.ChangeBitmap( r->bmpno );
	m_selected_bmp = r->bmpno;
}


/*--------------------------------------------------------------
	ビットマップ選択コンボボックス
----------------------------------------------------------------*/
void CDlgRPage1::OnCbnSelchangeCombo1()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	UpdateData(TRUE);
	g_childwnd.ChangeBitmap(m_selected_bmp);
}

/*--------------------------------------------------------------
	コンボボックスの項目更新
----------------------------------------------------------------*/
void CDlgRPage1::ResetBMPComboBox()
{
	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_COMBO1);
	if(!combo){
		OutputDebugString("CDlgRPage1::ResetBMPComboBox 取得失敗？？\n");
		throw;
	}

	combo->ResetContent();
	for(UINT i=0;i<GCDMAX_IMAGES;i++){
		combo->InsertString(i, theApp.GetBMPFilename(i) );
	}
	combo->SetCurSel(m_selected_bmp);
	UpdateData(TRUE);
	g_childwnd.ChangeBitmap(m_selected_bmp);
}


/*--------------------------------------------------------------
	表示物の更新
----------------------------------------------------------------*/
void CDlgRPage1::UpdateItems()
{
	ResetBMPComboBox();
	UpdateInfo();
}


/*--------------------------------------------------------------
	初期化時処理
----------------------------------------------------------------*/
BOOL CDlgRPage1::OnInitDialog()
{
	CMyDialogTab::OnInitDialog();

	// TODO :  ここに初期化を追加してください

	//値の初期値
	m_current_index = 1;
	UpdateData(FALSE);

	//ビットマップ選択コンボボックスが狭いので広げる
	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_COMBO1);
	if(!combo){
		OutputDebugString("CDlgRPage1::OnInitDialog 取得失敗？？\n");
		throw;
	}
	RECT r;
	combo->GetClientRect(&r);
	r.bottom += (r.bottom - r.top)*6;
	r.left = 20;
	r.top = 300;
	r.bottom += r.top;
	combo->MoveWindow(&r,FALSE);

	//スピンコントロールの設定
	CSpinButtonCtrl* spin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN1);
	if(!spin){
		OutputDebugString("CDlgRPage1::OnInitDialog spin取得失敗？？\n");
		throw;
	}
	spin->SetRange32(1,1023);
	spin->SetPos( m_current_index );

	//ラジオボタン1(矩形指定モード)をチェック状態にする
	CButton *wndr1 = (CButton*)GetDlgItem(IDC_RADIO1);
	wndr1->SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*--------------------------------------------------------------
	矩形番号変更時の処理
----------------------------------------------------------------*/
void CDlgRPage1::OnChangeIndex()
{
	UpdateData(FALSE);
	g_childwnd.Dirty();

	CSpinButtonCtrl* spin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN1);
	if(!spin){
		OutputDebugString("CDlgRPage1::OnInitDialog spin取得失敗？？\n");
		throw;
	}
	spin->SetPos(m_current_index);

	UpdateInfo();
}

/*--------------------------------------------------------------
	矩形情報表示更新
----------------------------------------------------------------*/
void CDlgRPage1::UpdateInfo()
{
	m_str_info1 = "";
	m_str_info2 = "";

	GCD_RECT *r = theApp.GetCurrentRect();
	if(r==NULL){
		m_str_info1 = "error";
		UpdateData(FALSE);
		return;
	}
	else if(CGCDHandler::GCDIsEmptyRect(*r)){
		m_str_info1 = "- empty -";
		UpdateData(FALSE);
		return;
	}
	else{
		m_str_info1.Format("bmp:%d center(%d,%d)",
			r->bmpno,r->center_x,r->center_y);
		m_str_info2.Format("%d,%d - %d,%d (w:%d / h:%d)",
			r->r.left,r->r.top,r->r.right,r->r.bottom,
			r->r.right - r->r.left,r->r.bottom - r->r.top);
	}
	UpdateData(FALSE);
}


/*--------------------------------------------------------------
	矩形指定/重心指定ラジオボタン
----------------------------------------------------------------*/
void CDlgRPage1::OnBnClickedRadio1()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。

	//[決定]ボタン無効
	CButton *btn = (CButton*)GetDlgItem(IDC_BUTTON_APPLY);
	if(btn)btn->EnableWindow(TRUE);

	//表示ウインドウクラスに通知
	g_childwnd.ChangeEditModeR(0);
}

void CDlgRPage1::OnBnClickedRadio2()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	
	//[決定]ボタン無効
	CButton *btn = (CButton*)GetDlgItem(IDC_BUTTON_APPLY);
	if(btn)btn->EnableWindow(FALSE);

	//表示ウインドウクラスに通知
	g_childwnd.ChangeEditModeR(1);
}

