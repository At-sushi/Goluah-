// source\system\Name.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "goluah.h"
#include "Name.h"


// CName �_�C�A���O

IMPLEMENT_DYNAMIC(CName, CDialog)
CName::CName(CWnd* pParent /*=NULL*/)
	: CDialog(CName::IDD, pParent)
	, m_name(_T(""))
	, m_port(_T(""))
{
}

CName::~CName()
{
}

void CName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
}


BEGIN_MESSAGE_MAP(CName, CDialog)
END_MESSAGE_MAP()


// CName ���b�Z�[�W �n���h��
