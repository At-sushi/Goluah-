// DlgPage2.cpp : 実装ファイル
//

#include "stdafx.h"
#include "goluah_config.h"
#include "DlgPage2.h"

#include "DlgKeyConfig.h"
#include "DlgPadConfig.h"
#include "DlgInputAssign.h"
#include ".\dlgpage2.h"

// CDlgPage2 ダイアログ

IMPLEMENT_DYNAMIC(CDlgPage2, CCfgDialogBase)
CDlgPage2::CDlgPage2(CWnd* pParent /*=NULL*/)
	: CCfgDialogBase(CDlgPage2::IDD, pParent)
	, ck_debugmode(FALSE)
	, ck_showmovie(FALSE)
	, limiter_cut(FALSE)
{
}

CDlgPage2::~CDlgPage2()
{
}

void CDlgPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_DEBUGMODE, ck_debugmode);
	DDX_Check(pDX, IDC_CHECK_SHOWINFO, ck_showinfo);
	DDX_Check(pDX, IDC_CHECK_SHOWMOVIE, ck_showmovie);
	DDX_Check(pDX, IDC_CHECK_POINTLIMITCUT, limiter_cut);
}


BEGIN_MESSAGE_MAP(CDlgPage2, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_KEYBORD_SETTEING, &CDlgPage2::OnBnClickedButtonKeybordSetteing)
	ON_BN_CLICKED(IDC_BUTTON_PAD_SETTING, &CDlgPage2::OnBnClickedButtonPadSetting)
	ON_BN_CLICKED(IDC_BUTTON_INPUTASSIGN, &CDlgPage2::OnBnClickedButtonInputassign)
	ON_BN_CLICKED(IDC_CHECK_POINTLIMITCUT, &CDlgPage2::OnBnClickedCheckPointlimitcut)
END_MESSAGE_MAP()


// CDlgPage2 メッセージ ハンドラ

/*------------------------------------------------------------------
	設定→ダイアログアイテム
--------------------------------------------------------------------*/
BOOL CDlgPage2::OnInitDialog()
{
	CCfgDialogBase::OnInitDialog();

	// TODO :  ここに初期化を追加してください
	Reset();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CDlgPage2::Reset()
{
#ifndef _DEBUG
	if(!g_config.limiter_cut_enable)
	{
		CWnd *lc_item = GetDlgItem(IDC_CHECK_POINTLIMITCUT);
		lc_item->ShowWindow(FALSE);
	}
#endif

	ck_debugmode	=g_config.IsDebugMode();
	ck_showinfo		=g_config.SwShowInfo();
	ck_showmovie	=g_config.SwShowMovieFirst();
	limiter_cut	=g_config.IsLimiterCut();
	UpdateData(FALSE);
}

/*------------------------------------------------------------------
	設定←ダイアログアイテム
--------------------------------------------------------------------*/
void CDlgPage2::UpdateSettings()
{
	UpdateData(TRUE);

	g_config.SetDebugMode(ck_debugmode);
	g_config.SetShowInfo(ck_showinfo);
	g_config.SetShowMovie(ck_showmovie);
	g_config.SetLimiterCut(limiter_cut);
}

/*------------------------------------------------------------------
	キーボード設定
--------------------------------------------------------------------*/
void CDlgPage2::OnBnClickedButtonKeybordSetteing()
{
	CDlgKeyConfig dlg(this, g_config.keycfg);
	dlg.DoModal();
}

/*------------------------------------------------------------------
	パッドボタン設定
--------------------------------------------------------------------*/
void CDlgPage2::OnBnClickedButtonPadSetting()
{
	CDlgPadConfig dlg(this);
	dlg.DoModal();
}

/*------------------------------------------------------------------
	パッドボタン設定
--------------------------------------------------------------------*/
void CDlgPage2::OnBnClickedButtonInputassign()
{
	CDlgInputAssign dlg(this);
	dlg.DoModal();
}

void CDlgPage2::OnBnClickedCheckPointlimitcut()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
}
