// DlgPage4.cpp : 実装ファイル
//

#include "stdafx.h"
#include "are_editer.h"
#include "DlgPage4.h"

CDlgPage4* CDlgPage4::m_pinstance=NULL;

// CDlgPage4 ダイアログ

IMPLEMENT_DYNAMIC(CDlgPage4, CMyDialogTab)
CDlgPage4::CDlgPage4(CWnd* pParent /*=NULL*/)
	: CMyDialogTab(CDlgPage4::IDD, pParent)
	, m_rectIndex(0)
	, m_cellrectNO(0)
	, m_flipX(FALSE)
	, m_flipY(FALSE)
	, m_alpha_ratio(0)
	, m_red_ratio(0)
	, m_green_ratio(0)
	, m_blue_ratio(0)
	, m_scale_x(0)
	, m_scale_y(0)
	, m_rotation(0)
	, m_blendadd(FALSE)
{
	m_pinstance = this;
}

CDlgPage4::~CDlgPage4()
{
}

void CDlgPage4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_cellrectNO);
	DDX_Check(pDX, IDC_CHECK_FLIPX, m_flipX);
	DDX_Check(pDX, IDC_CHECK_FLIPY, m_flipY);
	DDX_Text(pDX, IDC_EDIT_COLOR_A, m_alpha_ratio);
	DDX_Text(pDX, IDC_EDIT_COLOR_R, m_red_ratio);
	DDX_Text(pDX, IDC_EDIT_COLOR_G, m_green_ratio);
	DDX_Text(pDX, IDC_EDIT_COLOR_B, m_blue_ratio);
	DDX_Text(pDX, IDC_EDIT_SCAX, m_scale_x);
	DDX_Text(pDX, IDC_EDIT_SCAY, m_scale_y);
	DDX_Text(pDX, IDC_EDIT_ROT, m_rotation);
	DDX_Check(pDX, IDC_CHECK_BLENDADD, m_blendadd);
}


BEGIN_MESSAGE_MAP(CDlgPage4, CDialog)
	ON_BN_CLICKED(IDC_CHECK_FLIPX, OnBnClickedCheckFlipx)
	ON_BN_CLICKED(IDC_CHECK_FLIPY, OnBnClickedCheckFlipy)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_BN_CLICKED(IDC_BUTTON_PREV10, OnBnClickedButtonPrev10)
	ON_BN_CLICKED(IDC_BUTTON_NEXT10, OnBnClickedButtonNext10)
	ON_BN_CLICKED(IDC_BUTTON_PREV_FIND, OnBnClickedButtonPrevFind)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_FIND, OnBnClickedButtonNextFind)
	ON_BN_CLICKED(IDC_BUTTON_ERASE, OnBnClickedButtonErase)
	ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO4, OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO8, OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO9, OnBnClickedRadio9)
	ON_BN_CLICKED(IDC_RADIO10, OnBnClickedRadio10)
	ON_BN_CLICKED(IDC_CHECK_BLENDADD, OnBnClickedCheckBlendadd)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_COLOR_A, OnNMCustomdrawSliderColorA)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_COLOR_R, OnNMCustomdrawSliderColorR)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_COLOR_G, OnNMCustomdrawSliderColorG)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_COLOR_B, OnNMCustomdrawSliderColorB)
	ON_EN_CHANGE(IDC_EDIT_COLOR_A, OnEnChangeEditColorA)
	ON_EN_CHANGE(IDC_EDIT_COLOR_R, OnEnChangeEditColorR)
	ON_EN_CHANGE(IDC_EDIT_COLOR_G, OnEnChangeEditColorG)
	ON_EN_CHANGE(IDC_EDIT_COLOR_B, OnEnChangeEditColorB)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SCAX, OnNMCustomdrawSliderScax)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SCAY, OnNMCustomdrawSliderScay)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ROT, OnNMCustomdrawSliderRot)
	ON_EN_CHANGE(IDC_EDIT_SCAX, OnEnChangeEditScax)
	ON_EN_CHANGE(IDC_EDIT_SCAY, OnEnChangeEditScay)
	ON_EN_CHANGE(IDC_EDIT_ROT, OnEnChangeEditRot)
END_MESSAGE_MAP()


// CDlgPage4 メッセージ ハンドラ

/*-----------------------------------------------------------------------
	ダイアログ初期化処理
-------------------------------------------------------------------------*/
BOOL CDlgPage4::OnInitDialog()
{
	CMyDialogTab::OnInitDialog();

	// TODO :  ここに初期化を追加してください

	//矩形インデックスコンボボックスに項目突っ込み
/*	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_COMBO_RECT);
	if(!combo){
		OutputDebugString("CDlgPage4::OnInitDialog 取得失敗？？\n");
		throw;
	}
	RECT r = {65,24,65+100,24+200};
	combo->MoveWindow(&r,FALSE);
	combo->InsertString(0, "矩形0" );
	combo->InsertString(1, "矩形1" );
	combo->InsertString(2, "矩形2" );
	combo->InsertString(3, "矩形3" );
	combo->InsertString(4, "矩形4" );
	combo->InsertString(5, "矩形5" );
	combo->InsertString(6, "矩形6" );
	combo->InsertString(7, "矩形7" );
	combo->SetCurSel(0);*/

	//スピンコントロール設定
	CSpinButtonCtrl* spin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN1);
	if(!spin){
		OutputDebugString("CDlgRPage1::OnInitDialog spin取得失敗？？\n");
		throw;
	}
	spin->SetRange32(0,1023);
	spin->SetPos(0);

	//スライダー設定
	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_A);
	sl->SetRange(0,1000);
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_R);
	sl->SetRange(0,1000);
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_G);
	sl->SetRange(0,1000);
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_B);
	sl->SetRange(0,1000);
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SCAX);
	sl->SetRange(0,500);
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SCAY);
	sl->SetRange(0,500);
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_ROT);
	sl->SetRange(0,360*2);

	//ラジオボタン設定
	CButton* item_radio = (CButton*)GetDlgItem(IDC_RADIO1);
	if(item_radio)item_radio->SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*------------------------------------------------------------------------
	矩形インデックス変更時処理
--------------------------------------------------------------------------*
void CDlgPage4::OnCbnSelchangeComboRect()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	CCelleditDlg* pdlg = m_niseParent;

	if(!pdlg){
		theApp.SetStatus("CDlgPage4::OnCbnSelchangeCombo1 エラー、親ダイアログ取得失敗");
		return;
	}

	UpdateData(TRUE);
	pdlg->OnChangeRectIndex(m_rectIndex);
}

/*------------------------------------------------------------------------
	表示物更新
--------------------------------------------------------------------------*/
void CDlgPage4::UpdateItems()
{
	GCD_CELL2 *pc = theApp.GetCurrentCell();

	m_cellrectNO = pc->cell[ theApp.GetCurrentCellRectNO() ].cdr;
	m_flipX = (pc->cell[ theApp.GetCurrentCellRectNO() ].flag & GCDCELL_REVERSE_X) ? TRUE : FALSE;
	m_flipY = (pc->cell[ theApp.GetCurrentCellRectNO() ].flag & GCDCELL_REVERSE_Y) ? TRUE : FALSE;
	m_blendadd = (pc->cell[ theApp.GetCurrentCellRectNO() ].flag & GCDCELL_BLEND_ADD) ? TRUE : FALSE;

	m_alpha_ratio	= pc->cell[ theApp.GetCurrentCellRectNO() ].alpha_ratio;
	m_red_ratio		= pc->cell[ theApp.GetCurrentCellRectNO() ].red_ratio;
	m_green_ratio	= pc->cell[ theApp.GetCurrentCellRectNO() ].green_ratio;
	m_blue_ratio	= pc->cell[ theApp.GetCurrentCellRectNO() ].blue_ratio;

	CSliderCtrl* sl;
	
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_A);
	sl->SetPos( (int)(m_alpha_ratio*1000.0f) );
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_R);
	sl->SetPos( (int)(m_red_ratio*1000.0f) );
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_G);
	sl->SetPos( (int)(m_green_ratio*1000.0f) );
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_B);
	sl->SetPos( (int)(m_blue_ratio*1000.0f) );

	m_scale_x = pc->cell[ theApp.GetCurrentCellRectNO() ].magx;
	m_scale_y = pc->cell[ theApp.GetCurrentCellRectNO() ].magy;
	m_rotation = pc->cell[ theApp.GetCurrentCellRectNO() ].rot;

	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SCAX);
	sl->SetPos( (int)(m_scale_x*100.0f) );
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SCAY);
	sl->SetPos( (int)(m_scale_y*100.0f) );
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_ROT);
	sl->SetPos( m_rotation + 360 );

	UpdateData(FALSE);
}

/*------------------------------------------------------------------------
	反転フラグ　チェックボックス
--------------------------------------------------------------------------*/
void CDlgPage4::OnBnClickedCheckFlipx()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	UpdateData(TRUE);
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	if(!pc)return;

	CString str;

	if(m_flipX){
		pc->cell[ theApp.GetCurrentCellRectNO() ].flag |= GCDCELL_REVERSE_X;
		str.Format("セル%d - 矩形%d : X反転 ON",
			theApp.GetCurrentCellNO(),theApp.GetCurrentCellRectNO());
		theApp.SetStatus(str);
	}
	else{
		pc->cell[ theApp.GetCurrentCellRectNO() ].flag &= ~GCDCELL_REVERSE_X;
		str.Format("セル%d - 矩形%d : X反転 OFF",
			theApp.GetCurrentCellNO(),theApp.GetCurrentCellRectNO());
		theApp.SetStatus(str);
	}
	theApp.InvalidateAll();
}

void CDlgPage4::OnBnClickedCheckFlipy()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	UpdateData(TRUE);
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	if(!pc)return;

	CString str;

	if(m_flipY){
		pc->cell[ theApp.GetCurrentCellRectNO() ].flag |= GCDCELL_REVERSE_Y;
		str.Format("セル%d - 矩形%d : Y反転 ON",
			theApp.GetCurrentCellNO(),theApp.GetCurrentCellRectNO());
		theApp.SetStatus(str);
	}
	else{
		pc->cell[ theApp.GetCurrentCellRectNO() ].flag &= ~GCDCELL_REVERSE_Y;
		str.Format("セル%d - 矩形%d : Y反転 OFF",
			theApp.GetCurrentCellNO(),theApp.GetCurrentCellRectNO());
		theApp.SetStatus(str);
	}
	theApp.InvalidateAll();
}

/*------------------------------------------------------------------------
	加算合成　チェックボックス
--------------------------------------------------------------------------*/
void CDlgPage4::OnBnClickedCheckBlendadd()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	UpdateData(TRUE);
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	if(!pc)return;

	CString str;

	if(m_blendadd){
		pc->cell[ theApp.GetCurrentCellRectNO() ].flag |= GCDCELL_BLEND_ADD;
		str.Format("セル%d - 矩形%d : 加算合成",
			theApp.GetCurrentCellNO(),theApp.GetCurrentCellRectNO());
		theApp.SetStatus(str);
	}
	else{
		pc->cell[ theApp.GetCurrentCellRectNO() ].flag &= ~GCDCELL_BLEND_ADD;
		str.Format("セル%d - 矩形%d : 通常合成",
			theApp.GetCurrentCellNO(),theApp.GetCurrentCellRectNO());
		theApp.SetStatus(str);
	}
	theApp.InvalidateAll();
}

/*------------------------------------------------------------------------
	矩形番号変更時処理
--------------------------------------------------------------------------*/
void CDlgPage4::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	*pResult = 0;

	GCD_CELL2 *pc = theApp.GetCurrentCell();
	UINT next = pc->cell[ theApp.GetCurrentCellRectNO() ].cdr + pNMUpDown->iDelta;

	if(next<1 || next>1023){
		return;
	}
	pc->cell[ theApp.GetCurrentCellRectNO() ].cdr = next;
	theApp.InvalidateAll();
}

void CDlgPage4::OnBnClickedButtonPrev10()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	UINT next = pc->cell[ theApp.GetCurrentCellRectNO() ].cdr - 10;

	if(next<1 || next>1023){
		return;
	}
	pc->cell[ theApp.GetCurrentCellRectNO() ].cdr = next;
	theApp.InvalidateAll();
}

void CDlgPage4::OnBnClickedButtonNext10()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	UINT next = pc->cell[ theApp.GetCurrentCellRectNO() ].cdr + 10;

	if(next<1 || next>1023){
		return;
	}
	pc->cell[ theApp.GetCurrentCellRectNO() ].cdr = next;
	theApp.InvalidateAll();
}

void CDlgPage4::OnBnClickedButtonPrevFind()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	UINT next = pc->cell[ theApp.GetCurrentCellRectNO() ].cdr - 1;
	if(next==0){
		theApp.SetStatus("行き止まり");
		return;
	}

	CGCDHandler *gcd = theApp.GetGCD();

	while(gcd->IsEmptyRect(next)){
		next--;
		if(next==0){
			theApp.SetStatus("Prev Rect : ありません");
			return;
		}
	}
	pc->cell[ theApp.GetCurrentCellRectNO() ].cdr = next;
	theApp.InvalidateAll();
}

void CDlgPage4::OnBnClickedButtonNextFind()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	UINT next = pc->cell[ theApp.GetCurrentCellRectNO() ].cdr + 1;
	if(next==1023){
		theApp.SetStatus("行き止まり");
		return;
	}

	CGCDHandler *gcd = theApp.GetGCD();

	while(gcd->IsEmptyRect(next)){
		next++;
		if(next==1023){
			theApp.SetStatus("Next Rect : ありません");
			return;
		}
	}
	pc->cell[ theApp.GetCurrentCellRectNO() ].cdr = next;
	theApp.InvalidateAll();
}

/*------------------------------------------------------------------------
	矩形番号を0（なし）に設定
--------------------------------------------------------------------------*/
void CDlgPage4::OnBnClickedButtonErase()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].cdr = 0;

	theApp.SetStatus("矩形無し(0番)設定");
	theApp.InvalidateAll();
}

/*------------------------------------------------------------------------
	セル矩形番号EDIT BOX
--------------------------------------------------------------------------*/
void CDlgPage4::OnEnChangeEdit1()
{
	// TODO :  これが RICHEDIT コントロールの場合、まず、CMyDialogTab::OnInitDialog() 関数をオーバーライドして、
	// OR 状態の ENM_CORRECTTEXT フラグをマスクに入れて、
	// CRichEditCtrl().SetEventMask() を呼び出さない限り、
	// コントロールは、この通知を送信しません。

	// TODO :  ここにコントロール通知ハンドラ コードを追加してください。
	DWORD prv = m_cellrectNO;
	UpdateData(TRUE);

	GCD_CELL2 *pc = theApp.GetCurrentCell();

	if(m_cellrectNO<1 || m_cellrectNO>1023){
		m_cellrectNO = prv;
		theApp.SetStatus("矩形番号に不正な値が入力された");
		return;
	}
	pc->cell[ theApp.GetCurrentCellRectNO() ].cdr = m_cellrectNO;
	theApp.InvalidateAll();
}

/*------------------------------------------------------------------------
	矩形インデックス・ラジオボタン処理
--------------------------------------------------------------------------*/
void CDlgPage4::OnBnClickedRadio1()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	m_rectIndex = 0;
	m_niseParent->OnChangeRectIndex(0);
}

void CDlgPage4::OnBnClickedRadio2()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	m_rectIndex = 1;
	m_niseParent->OnChangeRectIndex(1);
}

void CDlgPage4::OnBnClickedRadio3()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	m_rectIndex = 2;
	m_niseParent->OnChangeRectIndex(2);
}

void CDlgPage4::OnBnClickedRadio4()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	m_rectIndex = 3;
	m_niseParent->OnChangeRectIndex(3);
}

void CDlgPage4::OnBnClickedRadio5()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	m_rectIndex = 4;
	m_niseParent->OnChangeRectIndex(4);
}

void CDlgPage4::OnBnClickedRadio8()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	m_rectIndex = 5;
	m_niseParent->OnChangeRectIndex(5);
}

void CDlgPage4::OnBnClickedRadio9()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	m_rectIndex = 6;
	m_niseParent->OnChangeRectIndex(6);
}

void CDlgPage4::OnBnClickedRadio10()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	m_rectIndex = 7;
	m_niseParent->OnChangeRectIndex(7);
}

/*------------------------------------------------------------------------
	色割合スライダーコントロール処理
--------------------------------------------------------------------------*/
void CDlgPage4::OnNMCustomdrawSliderColorA(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	*pResult = 0;

	int pos ;
	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_A);
	pos = sl->GetPos();
	m_alpha_ratio = pos/1000.0f;

	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].alpha_ratio = m_alpha_ratio;

	theApp.InvalidateAll();
}

void CDlgPage4::OnNMCustomdrawSliderColorR(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	*pResult = 0;
	
	int pos ;
	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_R);
	pos = sl->GetPos();
	m_red_ratio = pos/1000.0f;

	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].red_ratio = m_red_ratio;

	theApp.InvalidateAll();
}

void CDlgPage4::OnNMCustomdrawSliderColorG(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	*pResult = 0;

	int pos ;
	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_G);
	pos = sl->GetPos();
	m_green_ratio = pos/1000.0f;

	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].green_ratio = m_green_ratio;

	theApp.InvalidateAll();
}

void CDlgPage4::OnNMCustomdrawSliderColorB(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	*pResult = 0;

	int pos ;
	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_B);
	pos = sl->GetPos();
	m_blue_ratio = pos/1000.0f;

	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].blue_ratio = m_blue_ratio;
	
	theApp.InvalidateAll();
}


/*------------------------------------------------------------------------
	色割合エディットコントロール処理
--------------------------------------------------------------------------*/
void CDlgPage4::OnEnChangeEditColorA()
{
	UpdateData(TRUE);
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].alpha_ratio = m_alpha_ratio;

	theApp.InvalidateAll();
}

void CDlgPage4::OnEnChangeEditColorR()
{
	UpdateData(TRUE);
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].red_ratio = m_red_ratio;

	theApp.InvalidateAll();
}

void CDlgPage4::OnEnChangeEditColorG()
{
	UpdateData(TRUE);
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].green_ratio = m_green_ratio;

	theApp.InvalidateAll();
}

void CDlgPage4::OnEnChangeEditColorB()
{
	UpdateData(TRUE);

	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].blue_ratio = m_blue_ratio;

	theApp.InvalidateAll();
}

/*------------------------------------------------------------------------
	X/Y スケール
--------------------------------------------------------------------------*/
void CDlgPage4::OnNMCustomdrawSliderScax(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	*pResult = 0;

	int pos ;
	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SCAX);
	pos = sl->GetPos();
	m_scale_x = pos/100.0f;

	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].magx = m_scale_x;
	
	theApp.InvalidateAll();
}

void CDlgPage4::OnNMCustomdrawSliderScay(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	*pResult = 0;

	int pos ;
	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SCAY);
	pos = sl->GetPos();
	m_scale_y = pos/100.0f;
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].magy = m_scale_y;
	
	theApp.InvalidateAll();
}


void CDlgPage4::OnEnChangeEditScax()
{
	UpdateData(TRUE);
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].magx = m_scale_x;

	theApp.InvalidateAll();
}

void CDlgPage4::OnEnChangeEditScay()
{
	UpdateData(TRUE);
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].magy = m_scale_y;

	theApp.InvalidateAll();
}

/*------------------------------------------------------------------------
	回転
--------------------------------------------------------------------------*/
void CDlgPage4::OnNMCustomdrawSliderRot(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	*pResult = 0;

	int pos ;
	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_ROT);
	pos = sl->GetPos();

	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].rot = pos - 360;
	
	theApp.InvalidateAll();
}

void CDlgPage4::OnEnChangeEditRot()
{
	UpdateData(TRUE);
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].rot = m_rotation;

	theApp.InvalidateAll();
}

