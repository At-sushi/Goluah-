// source\system\NameC.cpp : 実装ファイル
//

#include "stdafx.h"
#include "goluah.h"
#include "NameC.h"


// CNameC ダイアログ

IMPLEMENT_DYNAMIC(CNameC, CDialog)
CNameC::CNameC(CWnd* pParent /*=NULL*/)
	: CDialog(CNameC::IDD, pParent)
	, m_name(_T(""))
	, m_port(_T(""))
	, m_hostIP(_T(""))
	, m_hostPort(_T(""))
{
}

CNameC::~CNameC()
{
}

void CNameC::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDX_Text(pDX, IDC_EDIT_HOSTIP, m_hostIP);
	DDX_Text(pDX, IDC_EDIT_HOSTPORT, m_hostPort);
}


BEGIN_MESSAGE_MAP(CNameC, CDialog)
END_MESSAGE_MAP()


// CNameC メッセージ ハンドラ
