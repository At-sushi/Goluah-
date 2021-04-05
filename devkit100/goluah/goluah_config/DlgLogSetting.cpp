// DlgLogSetting.cpp : 実装ファイル
//

#include "stdafx.h"
#include "goluah_config.h"
#include "DlgLogSetting.h"


// CDlgLogSetting ダイアログ

IMPLEMENT_DYNAMIC(CDlgLogSetting, CDialog)
CDlgLogSetting::CDlgLogSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLogSetting::IDD, pParent)
{
}

CDlgLogSetting::~CDlgLogSetting()
{
}

void CDlgLogSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLogSetting, CDialog)
END_MESSAGE_MAP()


// CDlgLogSetting メッセージ ハンドラ
