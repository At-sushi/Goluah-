// source\system\NameC.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "goluah.h"
#include "NameC.h"


// CNameC �_�C�A���O

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


// CNameC ���b�Z�[�W �n���h��
