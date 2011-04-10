// CelleditDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "are_editer.h"
#include "CelleditDlg.h"
#include "ChildWnd.h"

#include "DlgPage2.h"
#include "DlgPage3.h"
#include "DlgPage4.h"
#include "DlgCellDisp.h"
#include "DlgCPageTool.h"

// CCelleditDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CCelleditDlg, CMyDialogTab)
CCelleditDlg::CCelleditDlg(CWnd* pParent /*=NULL*/)
	: CMyDialogTab(CCelleditDlg::IDD, pParent)
{
	for(int i=0;i<MAX_TAB_PAGES_CELL;i++){
		m_tab_pages[i]=NULL;
	}
	m_rectIndex = 0;
	theApp.SetCurrentCellRectNOPointer( &m_rectIndex );
}

CCelleditDlg::~CCelleditDlg()
{
}

void CCelleditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCelleditDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CELL, OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CCelleditDlg ���b�Z�[�W �n���h��

/*--------------------------------------------------------------------
	�_�C�A���O������
----------------------------------------------------------------------*/
BOOL CCelleditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO :  �����ɏ�������ǉ����Ă�������

	//���^�u�R���g���[���̍��ڐ���
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_CELL);
	pTab->InsertItem(0,"�S��");
	pTab->InsertItem(1,"�}�E�X����");
	pTab->InsertItem(2,"��`����");
	pTab->InsertItem(3,"�\��");
	pTab->InsertItem(4,"tools");

	CDlgPage4 *pdlg4 = new CDlgPage4();
	pdlg4->SetNParent(this);

	//���e�^�u�̃y�[�W�ɑ�������_�C�A���O�𐶐�
	m_tab_pages[0] = new CDlgPage2();
	m_tab_pages[0]->Create(IDD_DIALOG_TAB2,this);
	m_tab_pages[1] = new CDlgPage3();
	m_tab_pages[1]->Create(IDD_DIALOG_TAB3,this);
	m_tab_pages[2] = pdlg4;
	m_tab_pages[2]->Create(IDD_DIALOG_TAB4,this);
	m_tab_pages[3] = new CDlgCellDisp();
	m_tab_pages[3]->Create(IDD_DIALOG_CTAB_DISP,this);
	m_tab_pages[4] = new CDlgCPageTool();
	m_tab_pages[4]->Create(IDD_DIALOG_CTAB_TOOLS,this);

	//���E�B���h�E���^�u�̈ʒu�ɍ����悤�Ɉړ�
	CRect r;
	for(int i=0;i<MAX_TAB_PAGES_CELL;i++){
		if(m_tab_pages[i]){
			m_tab_pages[i]->GetClientRect(&r);
			r.OffsetRect(8,30);
			m_tab_pages[i]->MoveWindow(&r,FALSE);
		}
	}

	OnTcnSelchangeTab1(0,0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*--------------------------------------------------------------------
	�^�u�R���g���[���̕ύX
----------------------------------------------------------------------*/
void CCelleditDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if(pResult)
		*pResult = 0;

	if (pResult) *pResult = 0;
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_CELL);
	int		i = pTab->GetCurSel();
	for(int j=0;j<MAX_TAB_PAGES_CELL;j++){
		if(m_tab_pages[j]){
			m_tab_pages[j]->ShowWindow(i == j ? SW_SHOW : SW_HIDE);
		}
	}

	if(i==2)m_tab_pages[2]->UpdateItems();
}


/*--------------------------------------------------------------------
	�q���E�C���h�E�����̔j��
----------------------------------------------------------------------*/
BOOL CCelleditDlg::DestroyWindow()
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	
	//�^�u�y�[�W�p�_�C�A���O�̔j��
	for(int i=0;i<MAX_TAB_PAGES_CELL;i++)
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
	�_�C�A���O���\�����̍X�V
----------------------------------------------------------------------*/
void CCelleditDlg::UpdateItems()
{
	for(int i=0;i<MAX_TAB_PAGES_CELL;i++)
	{
		if(m_tab_pages[i]){
			m_tab_pages[i]->UpdateItems();
		}
	}
}

/*--------------------------------------------------------------------
	�_�C�A���O���\�����̍X�V
----------------------------------------------------------------------*/
void CCelleditDlg::OnChangeRectIndex(UINT idx)
{
	m_rectIndex = idx;
	theApp.InvalidateAll();
}

/*--------------------------------------------------------------------
	���ꑀ��
----------------------------------------------------------------------*/
void CCelleditDlg::BlankNext()
{
	((CDlgPage2*)m_tab_pages[0])->OnBnClickedButtonNextBlank();
}

