// DlgCPageTool.cpp : 実装ファイル
//

#include "stdafx.h"
#include "are_editer.h"
#include "DlgCPageTool.h"


// CDlgCPageTool ダイアログ

IMPLEMENT_DYNAMIC(CDlgCPageTool, CMyDialogTab)
CDlgCPageTool::CDlgCPageTool(CWnd* pParent /*=NULL*/)
	: CMyDialogTab(CDlgCPageTool::IDD, pParent)
	, m_insEmptyCellIndex(0)
	, m_bUnder100(TRUE)
{
}

CDlgCPageTool::~CDlgCPageTool()
{
}

void CDlgCPageTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, 1009, m_insEmptyCellIndex);
	DDX_Check(pDX, 1004, m_bUnder100);
}


BEGIN_MESSAGE_MAP(CDlgCPageTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RFLIP0, OnBnClickedButtonRflip0)
	ON_BN_CLICKED(IDC_BUTTON_RFLIP1, OnBnClickedButtonRflip1)
	ON_BN_CLICKED(IDC_BUTTON_RFLIP2, OnBnClickedButtonRflip2)
	ON_BN_CLICKED(IDC_BUTTON_RFLIP3, OnBnClickedButtonRflip3)
	ON_BN_CLICKED(IDC_BUTTON_RFLIP4, OnBnClickedButtonRflip4)
	ON_BN_CLICKED(IDC_BUTTON_RFLIP5, OnBnClickedButtonRflip5)
	ON_BN_CLICKED(IDC_BUTTON_RFLIP6, OnBnClickedButtonRflip6)
	ON_BN_CLICKED(IDC_BUTTON_RFLIP7, OnBnClickedButtonRflip7)
	ON_BN_CLICKED(IDC_BUTTON_RFLIP8, OnBnClickedButtonRflip8)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CDlgCPageTool メッセージ ハンドラ

/*------------------------------------------------------------
	矩形入れ替えボタン
--------------------------------------------------------------*/
void CDlgCPageTool::OnBnClickedButtonRflip0()
{
	if(theApp.GetCurrentCellNO()<2){
		theApp.SetStatus("矩形入れ替え：前セルが範囲外です。できません。");
		return;
	}
	CGCDHandler *gcd = theApp.GetGCD();

	GCD_CELL2 *pc0 = gcd->GetCell(theApp.GetCurrentCellNO()-1);
	GCD_CELL2 *pc1 = theApp.GetCurrentCell();

	GCD_CELL c = pc1->cell[0];
	pc1->cell[0] = pc0->cell[7];
	pc0->cell[7] = c;

	theApp.SetStatus("矩形入れ替え：前7⇔0");
	theApp.InvalidateAll();
}

void CDlgCPageTool::OnBnClickedButtonRflip1()
{
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	GCD_CELL c = pc->cell[0];
	pc->cell[0] = pc->cell[1];
	pc->cell[1] = c;

	theApp.SetStatus("矩形入れ替え：0⇔1");
	theApp.InvalidateAll();
}

void CDlgCPageTool::OnBnClickedButtonRflip2()
{
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	GCD_CELL c = pc->cell[1];
	pc->cell[1] = pc->cell[2];
	pc->cell[2] = c;

	theApp.SetStatus("矩形入れ替え：1⇔2");
	theApp.InvalidateAll();
}

void CDlgCPageTool::OnBnClickedButtonRflip3()
{
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	GCD_CELL c = pc->cell[2];
	pc->cell[2] = pc->cell[3];
	pc->cell[3] = c;

	theApp.SetStatus("矩形入れ替え：2⇔3");
	theApp.InvalidateAll();
}

void CDlgCPageTool::OnBnClickedButtonRflip4()
{
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	GCD_CELL c = pc->cell[3];
	pc->cell[3] = pc->cell[4];
	pc->cell[4] = c;

	theApp.SetStatus("矩形入れ替え：3⇔4");
	theApp.InvalidateAll();
}

void CDlgCPageTool::OnBnClickedButtonRflip5()
{
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	GCD_CELL c = pc->cell[4];
	pc->cell[4] = pc->cell[5];
	pc->cell[5] = c;

	theApp.SetStatus("矩形入れ替え：4⇔5");
	theApp.InvalidateAll();
}

void CDlgCPageTool::OnBnClickedButtonRflip6()
{
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	GCD_CELL c = pc->cell[5];
	pc->cell[5] = pc->cell[6];
	pc->cell[6] = c;

	theApp.SetStatus("矩形入れ替え：5⇔6");
	theApp.InvalidateAll();
}

void CDlgCPageTool::OnBnClickedButtonRflip7()
{
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	GCD_CELL c = pc->cell[6];
	pc->cell[6] = pc->cell[7];
	pc->cell[7] = c;

	theApp.SetStatus("矩形入れ替え：6⇔7");
	theApp.InvalidateAll();
}

void CDlgCPageTool::OnBnClickedButtonRflip8()
{
	if(theApp.GetCurrentCellNO()>1022){
		theApp.SetStatus("矩形入れ替え：次セルが範囲外です。できません。");
		return;
	}
	CGCDHandler *gcd = theApp.GetGCD();

	GCD_CELL2 *pc0 = gcd->GetCell(theApp.GetCurrentCellNO()+1);
	GCD_CELL2 *pc1 = theApp.GetCurrentCell();

	GCD_CELL c = pc1->cell[7];
	pc1->cell[7] = pc0->cell[0];
	pc0->cell[0] = c;

	theApp.SetStatus("矩形入れ替え：前7⇔0");
	theApp.InvalidateAll();
}


/*------------------------------------------------------------
	空セル挿入ボタン
--------------------------------------------------------------*/
void CDlgCPageTool::OnBnClickedButton6()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	UpdateData(TRUE);

	CString str;

	if( m_insEmptyCellIndex<1 || 1022<m_insEmptyCellIndex )
	{
		str.Format("指定されたインデックスが範囲外です(%d)",m_insEmptyCellIndex);
		MessageBox(str,"空セル挿入処理");
		return;
	}

	str.Format("%dに空セルを挿入します",m_insEmptyCellIndex);
	if( IDYES==MessageBox(str,"空セル挿入処理",MB_YESNO) ){
		theApp.InsertEmptyCell(m_insEmptyCellIndex);
		return;
	}

	theApp.SetStatus("空セル挿入処理はキャンセルされました");
}

/*------------------------------------------------------------
	空の矩形しか参照していないCELLを削除
--------------------------------------------------------------*/
void CDlgCPageTool::OnBnClickedButton5()
{
	CString war_str;
	war_str += "空の矩形しか参照していないCELLを削除します。\n";
	war_str += "この操作は、オフセットや拡大率,判定矩形等の数値パラメータのみが\n";
	war_str += "存在するために空でないとみなされている不可視CELLのパラメータをクリアし、\n";
	war_str += "「空である」と認識されるようにします。\n";
	war_str += "\n";
	war_str += "実行前にバックアップを取ることをお勧めします。\n";
	war_str += "実行しますか？";

	if(IDYES!=MessageBox(war_str,"",MB_YESNO))return;

	theApp.DeleteInvalidCell();
}

/*------------------------------------------------------------
	空の矩形しか参照していないCELLを削除
--------------------------------------------------------------*/
void CDlgCPageTool::OnBnClickedButton3()
{
	UpdateData(TRUE);

	CString war_str;
	war_str += "空である矩形を削除し、間を詰めて番号を整理します。\n";
	war_str += "キャラクターの場合、通常 0～100のCELLを対象としないようにしてください。";
	war_str += "\n";
	war_str += "実行前にバックアップを取ることをお勧めします。\n";
	war_str += "実行しますか？";

	if(IDYES!=MessageBox(war_str,"",MB_YESNO))return;

	theApp.DeleteEmptyCell(m_bUnder100);
}
