// DlgPadConfig.cpp : 実装ファイル
//

#include "stdafx.h"
#include "goluah_config.h"
#include "DlgPadConfig.h"


// CDlgPadConfig ダイアログ

IMPLEMENT_DYNAMIC(CDlgPadConfig, CDialog)
CDlgPadConfig::CDlgPadConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPadConfig::IDD, pParent)
	, m_padindex(0)
	, m_buttonA(0)
	, m_buttonB(0)
	, m_buttonC(0)
	, m_buttonD(0)
{
}

CDlgPadConfig::~CDlgPadConfig()
{
}

void CDlgPadConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_PADINDEX, m_padindex);
	DDX_CBIndex(pDX, IDC_COMBO_PADBUTTON_A, m_buttonA);
	DDX_CBIndex(pDX, IDC_COMBO_PADBUTTON_B, m_buttonB);
	DDX_CBIndex(pDX, IDC_COMBO_PADBUTTON_C, m_buttonC);
	DDX_CBIndex(pDX, IDC_COMBO_PADBUTTON_D, m_buttonD);
}


BEGIN_MESSAGE_MAP(CDlgPadConfig, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_PADINDEX, &CDlgPadConfig::OnCbnSelchangeComboPadindex)
END_MESSAGE_MAP()


// CDlgPadConfig メッセージ ハンドラ

/*---------------------------------------------------------------
	初期化
-----------------------------------------------------------------*/
BOOL CDlgPadConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO :  ここに初期化を追加してください
	m_cfg = g_config.padcfg;

	//コンボボックスに項目突っ込み
	CComboBox* combo;
	RECT r;
	int i;

	//パッドインデックス
	combo = (CComboBox*)GetDlgItem(IDC_COMBO_PADINDEX);
	if(!combo){
		throw;
	}
	r.left	= 85;
	r.top	= 20;
	r.right	= r.left+100;
	r.bottom= r.top +300;
	combo->MoveWindow(&r,FALSE);
	char padname[16];
	for(i=0;i<NUM_PAD;i++)
	{
		sprintf(padname,"パッド%d",i+1);
		combo->InsertString(i, padname );
	}
	combo->SetCurSel(0);

	//ボタン
	int comboid[] ={
		IDC_COMBO_PADBUTTON_A,
		IDC_COMBO_PADBUTTON_B,
		IDC_COMBO_PADBUTTON_C,
		IDC_COMBO_PADBUTTON_D,
	};
	for(i=0;i<4;i++)
	{
		combo = (CComboBox*)GetDlgItem( comboid[i] );
		if(!combo){
			throw;
		}
		r.left	= 110;
		r.top	= 80+27*i;
		r.right	= r.left+100;
		r.bottom= r.top +300;
		combo->MoveWindow(&r,FALSE);
		combo->InsertString(0, "ボタン1" );
		combo->InsertString(1, "ボタン2" );
		combo->InsertString(2, "ボタン3" );
		combo->InsertString(3, "ボタン4" );
		combo->InsertString(4, "ボタン5" );
		combo->InsertString(5, "ボタン6" );
		combo->InsertString(6, "ボタン7" );
		combo->InsertString(7, "ボタン8" );
		combo->InsertString(8, "ボタン9" );
		combo->InsertString(9, "ボタン10" );
		combo->InsertString(10, "ボタン11" );
		combo->InsertString(11, "ボタン12" );
		combo->SetCurSel(0);
	}

	m_crnt_pad = m_padindex = 0;
	m_buttonA = m_cfg.pad_button[0][m_crnt_pad];
	m_buttonB = m_cfg.pad_button[1][m_crnt_pad];
	m_buttonC = m_cfg.pad_button[2][m_crnt_pad];
	m_buttonD = m_cfg.pad_button[3][m_crnt_pad];
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*---------------------------------------------------------------
	終了処理
-----------------------------------------------------------------*/
void CDlgPadConfig::OnOK()
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	UpdateData(TRUE);
	m_cfg.pad_button[0][m_crnt_pad] = m_buttonA;
	m_cfg.pad_button[1][m_crnt_pad] = m_buttonB;
	m_cfg.pad_button[2][m_crnt_pad] = m_buttonC;
	m_cfg.pad_button[3][m_crnt_pad] = m_buttonD;
	m_crnt_pad = m_padindex;

	g_config.padcfg = m_cfg;
	CDialog::OnOK();
}

/*---------------------------------------------------------------
	パッド変更時処理
-----------------------------------------------------------------*/
void CDlgPadConfig::OnCbnSelchangeComboPadindex()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	UpdateData(TRUE);

	m_cfg.pad_button[0][m_crnt_pad] = m_buttonA;
	m_cfg.pad_button[1][m_crnt_pad] = m_buttonB;
	m_cfg.pad_button[2][m_crnt_pad] = m_buttonC;
	m_cfg.pad_button[3][m_crnt_pad] = m_buttonD;

	m_crnt_pad = m_padindex;
	
	m_buttonA = m_cfg.pad_button[0][m_crnt_pad];
	m_buttonB = m_cfg.pad_button[1][m_crnt_pad];
	m_buttonC = m_cfg.pad_button[2][m_crnt_pad];
	m_buttonD = m_cfg.pad_button[3][m_crnt_pad];
	UpdateData(FALSE);
}

void CDlgPadConfig::OnCancel()
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	OnOK();
//	CDialog::OnCancel();
}
