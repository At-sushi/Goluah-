#include "..\goluah_config\dlgpage2.h"
#include "..\goluah_config\dlgpage2.h"
#include "..\goluah_config\dlgpage2.h"
#include "..\goluah_config\dlgpage2.h"
// DlgPage2.cpp : 実装ファイル
//

#include "stdafx.h"
#include "are_editer.h"
#include "DlgPage2.h"
#include "DlgPage4.h"
#include "ChildWnd.h"

// CDlgPage2 ダイアログ

IMPLEMENT_DYNCREATE(CDlgPage2, CDialog)

CDlgPage2::CDlgPage2(CWnd* pParent /*=NULL*/)
	: CMyDialogTab(CDlgPage2::IDD, pParent)
	, m_skip_link(FALSE)
	, m_sell_link(FALSE)
	, m_str_define(_T(""))
	, m_current_index(0)
	, m_flag_zw(FALSE)
	, m_flag_zt(FALSE)
	, m_flag_scaBP(FALSE)
	, m_flag_rotBP(FALSE)
{
	m_current_index = 1;
	theApp.SetCurrentCellNOPointer( &m_current_index);

	CGCDHandler::GCDSetEmptyCell2(&m_CopiedCell,&m_CopiedHantei);
}

CDlgPage2::~CDlgPage2()
{
}

void CDlgPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_SKIPLINK, m_skip_link);
	DDX_Check(pDX, IDC_CHECK_LINK, m_sell_link);
	DDX_Text(pDX, IDC_EDIT6, m_str_define);
	DDX_Text(pDX, IDC_EDIT1, m_current_index);
	DDX_Check(pDX, IDC_CHECK_NOZW, m_flag_zw);
	DDX_Check(pDX, IDC_CHECK_NOZT, m_flag_zt);
	DDX_Check(pDX, IDC_CHECK_SCA_BASEPOINT, m_flag_scaBP);
	DDX_Check(pDX, IDC_CHECK_ROT_BASEPOINT, m_flag_rotBP);
}

BEGIN_MESSAGE_MAP(CDlgPage2, CDialog)
	ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_BN_CLICKED(IDC_BUTTON_PREV10, OnBnClickedButtonPrev10)
	ON_BN_CLICKED(IDC_BUTTON_NEXT10, OnBnClickedButtonNext10)
	ON_BN_CLICKED(IDC_BUTTON_PREV_FIND, OnBnClickedButtonPrevFind)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_FIND, OnBnClickedButtonNextFind)
	ON_BN_CLICKED(IDC_BUTTON_PREV_BLANK, OnBnClickedButtonPrevBlank)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_BLANK, OnBnClickedButtonNextBlank)
	ON_BN_CLICKED(IDC_BUTTON_COPY, OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_PASTE, OnBnClickedButtonPaste)
	ON_BN_CLICKED(IDC_BUTTON_ERASE, OnBnClickedButtonErase)
	ON_BN_CLICKED(IDC_CHECK_LINK, OnBnClickedCheckLink)
	ON_EN_CHANGE(IDC_EDIT6, OnEnChangeEdit6)
	ON_BN_CLICKED(IDC_CHECK_NOZW, OnBnClickedCheckNozw)
	ON_BN_CLICKED(IDC_CHECK_NOZT, OnBnClickedCheckNozt)
	ON_BN_CLICKED(IDC_CHECK_SCA_BASEPOINT, OnBnClickedCheckScaBasepoint)
	ON_BN_CLICKED(IDC_CHECK_ROT_BASEPOINT, OnBnClickedCheckRotBasepoint)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CDlgPage2)
END_DHTML_EVENT_MAP()



/*--------------------------------------------------------------
	ダイアログボックス初期化処理
----------------------------------------------------------------*/
BOOL CDlgPage2::OnInitDialog()
{
	CDialog::OnInitDialog();

	//スピンコントロールの設定
	CSpinButtonCtrl* spin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN1);
	if(!spin){
		OutputDebugString("CDlgRPage1::OnInitDialog spin取得失敗？？\n");
		throw;
	}
	spin->SetRange32(1,1023);
	spin->SetPos( m_current_index );

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}


/*--------------------------------------------------------------
	CELL#直接(？)変更時処理
----------------------------------------------------------------*/
void CDlgPage2::OnEnChangeEdit1()
{
	// TODO :  ここにコントロール通知ハンドラ コードを追加してください。
	DWORD prv = m_current_index;
	UpdateData(TRUE);

	if(m_current_index<1 || m_current_index>1023){
		m_current_index = prv;
		theApp.SetStatus("セル番号に不正な値が入力された");
		return;
	}
	OnChangeIndex();
}


/*--------------------------------------------------------------
	CELL#ヨコ、スピンコントロール処理
----------------------------------------------------------------*/
void CDlgPage2::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
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
	セル番号移動ボタン処理
----------------------------------------------------------------*/
void CDlgPage2::OnBnClickedButtonPrev10()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	if(m_current_index<11)m_current_index=1;
	else m_current_index-=10;

	OnChangeIndex();
}

void CDlgPage2::OnBnClickedButtonNext10()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	m_current_index+=10;
	if(m_current_index>1023)m_current_index=1023;

	OnChangeIndex();
}

void CDlgPage2::OnBnClickedButtonPrevFind()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	if(m_current_index==1){
		theApp.SetStatus("Find Previous : 行き止まり");
		return;
	}
	CGCDHandler* pdat = theApp.GetGCD();

	UINT next = m_current_index;
	next--;
	while(pdat->IsEmptyCell(next)){
		if(next==1){
			theApp.SetStatus("Find Previous : ありません");
			return;
		}
		next--;
	}
	m_current_index = next;
	OnChangeIndex();
}

void CDlgPage2::OnBnClickedButtonNextFind()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	if(m_current_index==1023){
		theApp.SetStatus("Find Next : 行き止まりだぜ");
		return;
	}
	CGCDHandler* pdat = theApp.GetGCD();

	UINT next = m_current_index;
	next++;
	while(pdat->IsEmptyCell(next)){
		if(next==1023){
			theApp.SetStatus("Find Next : ありません");
			return;
		}
		next++;
	}
	m_current_index = next;
	OnChangeIndex();
}

void CDlgPage2::OnBnClickedButtonPrevBlank()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	if(m_current_index==1){
		theApp.SetStatus("Blank Previous : 行き止まり");
		return;
	}
	CGCDHandler* pdat = theApp.GetGCD();

	UINT next = m_current_index;
	next--;
	while(!pdat->IsEmptyCell(next)){
		if(next==1){
			theApp.SetStatus("Blank Previous : ありません");
			return;
		}
		next--;
	}
	m_current_index = next;
	OnChangeIndex();
}

void CDlgPage2::OnBnClickedButtonNextBlank()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	if(m_current_index==1023){
		theApp.SetStatus("Blank Next : 行き止まりだぜ");
		return;
	}
	CGCDHandler* pdat = theApp.GetGCD();

	UINT next = m_current_index;
	next++;
	while(!pdat->IsEmptyCell(next)){
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
	セル「コピー」処理
----------------------------------------------------------------*/
void CDlgPage2::OnBnClickedButtonCopy()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	m_CopiedCell = *theApp.GetCurrentCell();
	m_CopiedHantei = *theApp.GetCurrentHantei();
	theApp.SetStatus("セルデータをコピーしました");
}

/*--------------------------------------------------------------
	セル「ペースト」処理
----------------------------------------------------------------*/
void CDlgPage2::OnBnClickedButtonPaste()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	if(IDYES!=MessageBox("いいの？","セルデータ貼り付け",MB_YESNO)){
		theApp.SetStatus("セルデータ貼り付けキャンセル");
		return;
	}

	*theApp.GetCurrentCell() = m_CopiedCell;
	*theApp.GetCurrentHantei() = m_CopiedHantei;
	theApp.SetStatus("セルデータを貼り付けました");
	
	g_childwnd.Dirty();
}

/*--------------------------------------------------------------
	セル「消去」処理
----------------------------------------------------------------*/
void CDlgPage2::OnBnClickedButtonErase()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	if(IDYES!=MessageBox("いいの？","セルデータ消去",MB_YESNO)){
		theApp.SetStatus("セルデータ消去処理キャンセル");
		return;
	}

	CGCDHandler::GCDSetEmptyCell2(theApp.GetCurrentCell(),theApp.GetCurrentHantei());
	g_childwnd.Dirty();
}

/*--------------------------------------------------------------
	#define 文字列変更時処理
----------------------------------------------------------------*/
void CDlgPage2::OnEnChangeEdit6()
{
	// TODO :  ここにコントロール通知ハンドラ コードを追加してください。
	UpdateData(TRUE);
	CGCDHandler *gcd = theApp.GetGCD();
	strcpy(gcd->GetName(m_current_index),m_str_define.GetBuffer());
}


/*--------------------------------------------------------------
	現在セル番号変更時処理
----------------------------------------------------------------*/
void CDlgPage2::OnChangeIndex()
{
	//#defineの文字列変更
	CGCDHandler *gcd = theApp.GetGCD();
	m_str_define.Format("%s",gcd->GetName(m_current_index));

	GCD_CELL2 *pc = theApp.GetCurrentCell();
	if(pc){
		//リンクフラグチェックボックス設定
		m_sell_link = (pc->flag & GCDCELL2_LINK) ? TRUE : FALSE;
		m_flag_zw = (pc->flag & GCDCELL2_DISABLE_ZW) ? TRUE : FALSE;
		m_flag_zt = (pc->flag & GCDCELL2_DISABLE_ZT) ? TRUE : FALSE;
		m_flag_scaBP = (pc->flag & GCDCELL2_SCA_GCENTER) ? TRUE : FALSE;
		m_flag_rotBP = (pc->flag & GCDCELL2_ROT_BASEPOINT) ? TRUE : FALSE;
	}

	UpdateData(FALSE);

	CDlgPage4::m_pinstance->UpdateItems();
	g_childwnd.Dirty();
	

	//スピンコントロールポジション変更
	CSpinButtonCtrl* spin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN1);
	if(!spin){
		OutputDebugString("CDlgPage2::OnChangeIndex spin取得失敗？？\n");
		throw;
	}
	spin->SetPos(m_current_index);
}


/*--------------------------------------------------------------
	表示物更新
----------------------------------------------------------------*/
void CDlgPage2::UpdateItems()
{
	OnChangeIndex();
}

/*--------------------------------------------------------------
	フラグON/OFF時処理
----------------------------------------------------------------*/

//セルリンク
void CDlgPage2::OnBnClickedCheckLink()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	UpdateData(TRUE);
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	if(!pc){
		theApp.SetStatus("リンクフラグ操作失敗");
		return;
	}

	//スペルミスってるけど･･･
	if(m_sell_link){
		pc->flag |= GCDCELL2_LINK;
		theApp.SetStatus("リンクフラグ ON");
	}
	else{
		pc->flag &= ~GCDCELL2_LINK;
		theApp.SetStatus("リンクフラグ OFF");
	}

	g_childwnd.Dirty();
}

//Z-Write
void CDlgPage2::OnBnClickedCheckNozw()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	UpdateData(TRUE);
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	if(!pc){
		theApp.SetStatus("ZWフラグ操作失敗");
		return;
	}

	if(m_flag_zw){
		pc->flag |= GCDCELL2_DISABLE_ZW;
		theApp.SetStatus("no-ZWフラグ ON");
	}
	else{
		pc->flag &= ~GCDCELL2_DISABLE_ZW;
		theApp.SetStatus("no-ZWフラグ OFF");
	}

	g_childwnd.Dirty();
}

//Z-Test
void CDlgPage2::OnBnClickedCheckNozt()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	UpdateData(TRUE);
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	if(!pc){
		theApp.SetStatus("ZTフラグ操作失敗");
		return;
	}

	if(m_flag_zt){
		pc->flag |= GCDCELL2_DISABLE_ZT;
		theApp.SetStatus("no-ZTフラグ ON");
	}
	else{
		pc->flag &= ~GCDCELL2_DISABLE_ZT;
		theApp.SetStatus("no-ZTフラグ OFF");
	}

	g_childwnd.Dirty();
}

//重心中心スケール
void CDlgPage2::OnBnClickedCheckScaBasepoint()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	UpdateData(TRUE);
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	if(!pc){
		theApp.SetStatus("重心中心スケールフラグ操作失敗");
		return;
	}

	if(m_flag_scaBP){
		pc->flag |= GCDCELL2_SCA_GCENTER;
		theApp.SetStatus("重心中心スケールフラグ ON");
	}
	else{
		pc->flag &= ~GCDCELL2_SCA_GCENTER;
		theApp.SetStatus("重心中心スケールフラグ OFF");
	}

	g_childwnd.Dirty();
}

//足元中心回転
void CDlgPage2::OnBnClickedCheckRotBasepoint()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	UpdateData(TRUE);
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	if(!pc){
		theApp.SetStatus("足元中心回転フラグ操作失敗");
		return;
	}

	if(m_flag_rotBP){
		pc->flag |= GCDCELL2_ROT_BASEPOINT;
		theApp.SetStatus("足元中心回転フラグ ON");
	}
	else{
		pc->flag &= ~GCDCELL2_ROT_BASEPOINT;
		theApp.SetStatus("足元中心回転フラグ OFF");
	}
	g_childwnd.Dirty();
}


