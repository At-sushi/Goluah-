// DlgTPage2.cpp : 実装ファイル
//

#include "stdafx.h"
#include "are_editer.h"
#include "DlgTPage2.h"


// CDlgTPage2 ダイアログ

IMPLEMENT_DYNAMIC(CDlgTPage2, CMyDialogTab)
CDlgTPage2::CDlgTPage2(CWnd* pParent /*=NULL*/)
	: CMyDialogTab(CDlgTPage2::IDD, pParent)
{
}

CDlgTPage2::~CDlgTPage2()
{
}

void CDlgTPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTPage2, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(1001, OnBnClicked1001)
END_MESSAGE_MAP()

// CDlgTPage2 メッセージ ハンドラ

/*-----------------------------------------------------------
	空の矩形を消去する
-------------------------------------------------------------*/
void CDlgTPage2::OnBnClickedButton1()
{
	if(IDYES!=MessageBox(
			"矩形内のビットマップ色をチェックし、すべてが透明色の場合その矩形をクリアします。\nビットマップが正常に読み込まれていない場合、そのビットマップを参照する矩形はすべて消去されます\n\n実行前にバックアップをとっておくことをお勧めします。\n実行しますか？",
			"",MB_YESNO))
	{return;}

	theApp.DeleteInvalidRect();
}

/*-----------------------------------------------------------
	矩形番号を整理する
-------------------------------------------------------------*/
void CDlgTPage2::OnBnClicked1001()
{
	if(IDYES!=MessageBox(
			"面積0の矩形データをもつインデックスを消去し、番号をつめます。\n\n実行前にバックアップをとっておくことをお勧めします。\n\n実行しますか？",
			"",MB_YESNO))
	{return;}

	theApp.DeleteEmptyRect();
}
