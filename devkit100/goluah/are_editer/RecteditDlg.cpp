// RecteditDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "are_editer.h"
#include "RecteditDlg.h"

#include "DlgRPage1.h"
#include "DlgTPage2.h"

// CRecteditDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CRecteditDlg, CMyDialogTab)
CRecteditDlg::CRecteditDlg(CWnd* pParent /*=NULL*/)
	: CMyDialogTab(CRecteditDlg::IDD, pParent)
{
	for(int i=0;i<MAX_TAB_PAGES_RECT;i++){
		m_tab_pages[i]=NULL;
	}
}

CRecteditDlg::~CRecteditDlg()
{
}

void CRecteditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRecteditDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_RECT, OnTcnSelchangeTabRect)
END_MESSAGE_MAP()


// CRecteditDlg ���b�Z�[�W �n���h��

/*--------------------------------------------------------------------
	�_�C�A���O������
----------------------------------------------------------------------*/
BOOL CRecteditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO :  �����ɏ�������ǉ����Ă�������
	//���^�u�R���g���[���̍��ڐ���
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_RECT);
	pTab->InsertItem(0,"�S��");
	pTab->InsertItem(1,"tools");

	//���e�^�u�̃y�[�W�ɑ�������_�C�A���O�𐶐�
	m_tab_pages[0] = new CDlgRPage1();
	m_tab_pages[0]->Create(IDD_DIALOG_RTAB1,this);
	m_tab_pages[1] = new CDlgTPage2();
	m_tab_pages[1]->Create(IDD_DIALOG_RTAB2,this);

	//���E�B���h�E���^�u�̈ʒu�ɍ����悤�Ɉړ�
	CRect r;
	for(int i=0;i<MAX_TAB_PAGES_RECT;i++){
		if(m_tab_pages[i]){
			m_tab_pages[i]->GetClientRect(&r);
			r.OffsetRect(8,30);
			m_tab_pages[i]->MoveWindow(&r,FALSE);
		}
	}

	OnTcnSelchangeTabRect(0,0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*--------------------------------------------------------------------
	�^�u�R���g���[���̕ύX
----------------------------------------------------------------------*/
void CRecteditDlg::OnTcnSelchangeTabRect(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if(pResult)
		*pResult = 0;

	if (pResult) *pResult = 0;
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_RECT);
	int		i = pTab->GetCurSel();
	for(int j=0;j<MAX_TAB_PAGES_RECT;j++){
		if(m_tab_pages[j])
		{
			m_tab_pages[j]->ShowWindow(i == j ? SW_SHOW : SW_HIDE);
		}
	}
}


/*--------------------------------------------------------------------
	�q���E�C���h�E�����̔j��
----------------------------------------------------------------------*/
BOOL CRecteditDlg::DestroyWindow()
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	//�^�u�y�[�W�p�_�C�A���O�̔j��
	for(int i=0;i<MAX_TAB_PAGES_RECT;i++)
	{
		if(m_tab_pages[i]){
			m_tab_pages[i]->DestroyWindow();
			delete m_tab_pages[i];
			m_tab_pages[i] = NULL;
		}
	}

	return CDialog::DestroyWindow();
}


/*--------------------------------------------------------------------
	�\�����X�V
----------------------------------------------------------------------*/
void CRecteditDlg::UpdateItems()
{
	for(int i=0;i<MAX_TAB_PAGES_RECT;i++)
	{
		if(m_tab_pages[i]){
			m_tab_pages[i]->UpdateItems();
		}
	}
}

/*--------------------------------------------------------------------
	���ꑀ��
----------------------------------------------------------------------*/
void CRecteditDlg::BlankNext()
{
	((CDlgRPage1*)m_tab_pages[0])->OnBnClickedButtonNextBlank();
}
