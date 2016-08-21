// DlgCellDisp.cpp : 実装ファイル
//

#include "stdafx.h"
#include "are_editer.h"
#include "DlgCellDisp.h"

#include "ChildWnd.h"


// CDlgCellDisp ダイアログ

IMPLEMENT_DYNAMIC(CDlgCellDisp, CMyDialogTab)
CDlgCellDisp::CDlgCellDisp(CWnd* pParent /*=NULL*/)
	: CMyDialogTab(CDlgCellDisp::IDD, pParent)
	, x_flip(FALSE)
	, y_flip(FALSE)
	, m_mag(0)
	, m_marker_on(FALSE)
{
	m_marker_on = FALSE;

	m_ofsx = m_ofsy = 0;
	m_scax = m_scay = 1.0f;
	m_rotation = 0;
}

CDlgCellDisp::~CDlgCellDisp()
{
}

void CDlgCellDisp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_FLIPXTEST, x_flip);
	DDX_Check(pDX, IDC_CHECK_FLIPYTEST, y_flip);
	DDX_CBIndex(pDX, IDC_COMBO_MAG, m_mag);
	DDX_Check(pDX, IDC_CHECK1, m_marker_on);
}


BEGIN_MESSAGE_MAP(CDlgCellDisp, CMyDialogTab)
	ON_CBN_SELCHANGE(IDC_COMBO_MAG, OnCbnSelchangeComboMag)
	ON_BN_CLICKED(IDC_CHECK_FLIPXTEST, OnBnClickedCheckFlipxtest)
	ON_BN_CLICKED(IDC_CHECK_FLIPYTEST, OnBnClickedCheckFlipytest)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_OFSX, OnNMCustomdrawSliderOfsx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_OFSY, OnNMCustomdrawSliderOfsy)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SCAX, OnNMCustomdrawSliderScax)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SCAY, OnNMCustomdrawSliderScay)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ROT, OnNMCustomdrawSliderRot)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CDlgCellDisp)
END_DHTML_EVENT_MAP()

// CDlgCellDisp メッセージ ハンドラ

/*------------------------------------------------------------------------
	初期化
--------------------------------------------------------------------------*/
BOOL CDlgCellDisp::OnInitDialog()
{
	CMyDialogTab::OnInitDialog();

	// TODO :  ここに初期化を追加してください
	//拡大率項目設定
	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_COMBO_MAG);
	if(!combo){
		OutputDebugString("CDlgRPage1::OnInitDialog 取得失敗？？\n");
		throw;
	}
	RECT r = {90,8,90+100,8+200};
	combo->MoveWindow(&r,FALSE);
	combo->InsertString(0, "25%" );
	combo->InsertString(1, "50%" );
	combo->InsertString(2, "75%" );
	combo->InsertString(3, "100%" );
	combo->InsertString(4, "200%" );
	combo->InsertString(5, "400%" );
	m_mag = 3;
	combo->SetCurSel(m_mag);

	//スライダーコントロールに範囲セット
	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_OFSX);
	sl->SetRange(-300,300,TRUE);
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_OFSY);
	sl->SetRange(-300,300,TRUE);
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SCAX);
	sl->SetRange(0,500);
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SCAY);
	sl->SetRange(0,500);
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_ROT);
	sl->SetRange(-180,180,TRUE);

	SetupSliders();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*--------------------------------------------------------------
	スライダーコントロール値設定
----------------------------------------------------------------*/
void CDlgCellDisp::SetupSliders()
{
	CSliderCtrl *sl;

	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_OFSX);
	sl->SetPos(m_ofsx);
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_OFSY);
	sl->SetPos(m_ofsy);
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SCAX);
	sl->SetPos((int)(m_scax*100.0f));
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SCAY);
	sl->SetPos((int)(m_scay*100.0f));
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_ROT);
	sl->SetPos(m_rotation);
}

/*--------------------------------------------------------------
	拡大率コンボボックス変更時処理
----------------------------------------------------------------*/
void CDlgCellDisp::OnCbnSelchangeComboMag()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	UpdateData(TRUE);
	g_childwnd.SetCMag(m_mag);

	CString str;
	str.Format("mag:%d\n",m_mag);
	OutputDebugString(str);
}

/*--------------------------------------------------------------
	テスト用 x・y反転チェックボックス変更時処理
----------------------------------------------------------------*/
void CDlgCellDisp::OnBnClickedCheckFlipxtest()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	UpdateData(TRUE);
	g_childwnd.SetCFlipX( x_flip );
}
void CDlgCellDisp::OnBnClickedCheckFlipytest()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	UpdateData(TRUE);
	g_childwnd.SetCFlipY( y_flip );
}



/*--------------------------------------------------------------
	マーカーON / OFF
----------------------------------------------------------------*/
void CDlgCellDisp::OnBnClickedCheck1()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	UpdateData(TRUE);
	g_childwnd.MarkerON(m_marker_on);

	if(m_marker_on)	theApp.SetStatus("マーカーON");
	else			theApp.SetStatus("マーカーOFF");
}


/*--------------------------------------------------------------
	チェック用表示設定クリア
----------------------------------------------------------------*/
void CDlgCellDisp::OnBnClickedButton1()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。

	m_ofsx = m_ofsy = 0;
	m_scax = m_scay = 1.0f;
	m_rotation = 0;

	SetupSliders();
}


/*--------------------------------------------------------------
	チェック用表示設定・スライダーコントロール操作時処理
----------------------------------------------------------------*/
void CDlgCellDisp::OnNMCustomdrawSliderOfsx(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	*pResult = 0;

	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_OFSX);
	m_ofsx = sl->GetPos();

	NotifyChild();
}

void CDlgCellDisp::OnNMCustomdrawSliderOfsy(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	*pResult = 0;

	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_OFSY);
	m_ofsy = sl->GetPos();

	NotifyChild();
}

void CDlgCellDisp::OnNMCustomdrawSliderScax(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	*pResult = 0;

	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SCAX);
	m_scax = sl->GetPos() / 100.0f;

	NotifyChild();
}

void CDlgCellDisp::OnNMCustomdrawSliderScay(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	*pResult = 0;

	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SCAY);
	m_scay = sl->GetPos() / 100.0f;

	NotifyChild();
}

void CDlgCellDisp::OnNMCustomdrawSliderRot(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	*pResult = 0;

	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_ROT);
	m_rotation = sl->GetPos();

	NotifyChild();
}


/*--------------------------------------------------------------
	表示ウインドウにスライダーコントロールの値を通知
----------------------------------------------------------------*/
void CDlgCellDisp::NotifyChild()
{
	g_childwnd.SetCDisplaySettings(
			m_ofsx,	m_ofsy, m_scax, m_scay, m_rotation);
}

