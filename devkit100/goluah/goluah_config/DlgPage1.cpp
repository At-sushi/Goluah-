// DlgPage1.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "goluah_config.h"
#include "DlgPage1.h"
#include ".\dlgpage1.h"


// CDlgPage1 �_�C�A���O

IMPLEMENT_DYNAMIC(CDlgPage1, CCfgDialogBase)
CDlgPage1::CDlgPage1(CWnd* pParent /*=NULL*/)
	: CCfgDialogBase(CDlgPage1::IDD, pParent)
{
}

CDlgPage1::~CDlgPage1()
{
}

void CDlgPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_FULL_SCREEN, ck_full_screen);
	DDX_Check(pDX, IDC_CHECK_HALFMODE, ck_halfmode);
	DDX_Check(pDX, IDC_CHECK_ALPHATEST, ck_noalphatest);
}

BEGIN_MESSAGE_MAP(CDlgPage1, CCfgDialogBase)
END_MESSAGE_MAP()

/*------------------------------------------------------------------
	�ݒ聨�_�C�A���O�A�C�e��
--------------------------------------------------------------------*/
BOOL CDlgPage1::OnInitDialog()
{
	CCfgDialogBase::OnInitDialog();

	// TODO :  �����ɏ�������ǉ����Ă�������
	Reset();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

void CDlgPage1::Reset()
{
	ck_full_screen	= g_config.IsFullScreen();
	ck_halfmode		= g_config.IsHalfMode();
	ck_noalphatest	= g_config.NoAlphaTest();
	UpdateData(FALSE);
}

/*------------------------------------------------------------------
	�ݒ聩�_�C�A���O�A�C�e��
--------------------------------------------------------------------*/
void CDlgPage1::UpdateSettings()
{
	UpdateData(TRUE);

	g_config.SetFullScreen(ck_full_screen);
	g_config.SetHalfMode(ck_halfmode);
	g_config.SetNoAlphaTest(ck_noalphatest);
}


//void CDlgPage1::OnClose()
//{
//	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
//
//	CCfgDialogBase::OnClose();
//}
