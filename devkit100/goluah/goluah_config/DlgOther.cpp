// DlgOther.cpp : 実装ファイル
//

#include "stdafx.h"
#include "goluah_config.h"
#include "DlgOther.h"


// CDlgOther ダイアログ

IMPLEMENT_DYNAMIC(CDlgOther, CDialog)
CDlgOther::CDlgOther(CWnd* pParent /*=NULL*/)
	: CCfgDialogBase(CDlgOther::IDD, pParent)
	, ck_log2file(FALSE)
	, ck_loginfo(FALSE)
	, ck_logwarning(FALSE)
	, ck_logdebug(FALSE)
	, combo_imgformat(0)
{
}

CDlgOther::~CDlgOther()
{
}

void CDlgOther::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_USELOG,			ck_log2file);
	DDX_Check(pDX, IDC_CHECK_USELOG_INFO,		ck_loginfo);
	DDX_Check(pDX, IDC_CHECK_USELOG_WARNING,	ck_logwarning);
	DDX_Check(pDX, IDC_CHECK_USELOG_DEBUG,		ck_logdebug);
	DDX_Check(pDX, IDC_COMBO_IMGFORMAT,			combo_imgformat);
}


BEGIN_MESSAGE_MAP(CDlgOther, CDialog)
	ON_BN_CLICKED(IDC_CHECK_USELOG, &CDlgOther::OnBnClickedCheckUselog)
END_MESSAGE_MAP()


// CDlgOther メッセージ ハンドラ

void CDlgOther::Reset()
{
	ck_log2file		= g_config.UseLog2File();
	ck_loginfo		= g_config.UseLog_Info();
	ck_logwarning	= g_config.UseLog_Warning();
	ck_logdebug		= g_config.UseLog_Debug();
	combo_imgformat	= g_config.GetSShotImageFormat();

	UpdateData(FALSE);

	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_COMBO_IMGFORMAT);
	if(!combo){
		throw;
	}
	combo->SetCurSel( combo_imgformat );

	EnableLogChecks(ck_log2file);
}

/*------------------------------------------------------------------
	設定←ダイアログアイテム
--------------------------------------------------------------------*/
void CDlgOther::UpdateSettings()
{
	UpdateData(TRUE);
	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_COMBO_IMGFORMAT);
	if(!combo){
		throw;
	}
	combo_imgformat = combo->GetCurSel();

	g_config.SetUseLog2File( ck_log2file );
	g_config.SetUseLog_Info( ck_loginfo );
	g_config.SetUseLog_Warning( ck_logwarning );
	g_config.SetUseLog_Debug( ck_logdebug );
	g_config.SetSShotImageFormat( (Config2_SShotFormat)combo_imgformat );
}


BOOL CDlgOther::OnInitDialog()
{
	CCfgDialogBase::OnInitDialog();

	// TODO :  ここに初期化を追加してください

	//パッドインデックス
	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_COMBO_IMGFORMAT);
	if(!combo){
		throw;
	}
	combo->InsertString(0, "JPEG" );
	combo->InsertString(1, "PNG" );
	combo->InsertString(2, "BMP" );

	Reset();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*------------------------------------------------------------------
	ＬＯＧ関係チェックボックス一括有効/無効化
--------------------------------------------------------------------*/
void CDlgOther::EnableLogChecks(BOOL en)
{
	CWnd *pctrl;

	pctrl = GetDlgItem(IDC_CHECK_USELOG_INFO);
	if(pctrl)pctrl->EnableWindow(en);

	pctrl = GetDlgItem(IDC_CHECK_USELOG_WARNING);
	if(pctrl)pctrl->EnableWindow(en);

	pctrl = GetDlgItem(IDC_CHECK_USELOG_DEBUG);
	if(pctrl)pctrl->EnableWindow(en);
}

void CDlgOther::OnBnClickedCheckUselog()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	UpdateData(TRUE);
	EnableLogChecks(ck_log2file);
}

