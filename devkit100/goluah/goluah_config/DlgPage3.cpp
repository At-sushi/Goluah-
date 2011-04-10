// DlgPage3.cpp : 実装ファイル
//

#include "stdafx.h"
#include "goluah_config.h"
#include "DlgPage3.h"


// CDlgPage3 ダイアログ

IMPLEMENT_DYNAMIC(CDlgPage3, CCfgDialogBase)
CDlgPage3::CDlgPage3(CWnd* pParent /*=NULL*/)
	: CCfgDialogBase(CDlgPage3::IDD, pParent)
	, use_dinput(FALSE)
	, m_combo_devtype(0)
{
}

CDlgPage3::~CDlgPage3()
{
}

void CDlgPage3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_USEDINPUT, use_dinput);
	DDX_Check(pDX, IDC_CHECK_USEDSOUND, use_dsound);
	DDX_Check(pDX, IDC_CHECK_USEDSHOW, use_dshow);
	DDX_Check(pDX, IDC_CHECK_IGNORE_EXCEPTIONS, ignore_exceptions);
	DDX_CBIndex(pDX, IDC_COMBO_DEVTYPE, m_combo_devtype);
}


BEGIN_MESSAGE_MAP(CDlgPage3, CDialog)
END_MESSAGE_MAP()


// CDlgPage3 メッセージ ハンドラ



/*------------------------------------------------------------------
	設定→ダイアログアイテム
--------------------------------------------------------------------*/
BOOL CDlgPage3::OnInitDialog()
{
	CCfgDialogBase::OnInitDialog();

	// TODO :  ここに初期化を追加してください
	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_COMBO_DEVTYPE);
	if(!combo){
		throw;
	}

	combo->InsertString(D3DDEVTYPE_HAL - 1, "HAL");
	combo->InsertString(D3DDEVTYPE_REF - 1, "REF");
	Reset();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CDlgPage3::Reset()
{
	use_dinput	=g_config.UseDInput();
	use_dsound	=g_config.UseDSound();
	use_dshow	=g_config.UseDShow();
	ignore_exceptions = g_config.IgnoreExceptions();
	m_combo_devtype = g_config.DeviceType() - 1;
	UpdateData(FALSE);
}

/*------------------------------------------------------------------
	設定←ダイアログアイテム
--------------------------------------------------------------------*/
void CDlgPage3::UpdateSettings()
{
	UpdateData(TRUE);

	g_config.SetUseDInput(use_dinput);
	g_config.SetUseDSound(use_dsound);
	g_config.SetUseDShow(use_dshow);
	g_config.SetIgnoreExceptions(ignore_exceptions);
	g_config.SetDeviceType((D3DDEVTYPE)(m_combo_devtype + 1));
}