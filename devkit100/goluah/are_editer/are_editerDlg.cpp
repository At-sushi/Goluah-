// are_editerDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "dx_draw.h"

#include "are_editer.h"
#include "are_editerDlg.h"
#include "DlgOther.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "DlgPage1.h"
#include "CelleditDlg.h"
#include "RecteditDlg.h"
#include "ChildWnd.h"
#include ".\are_editerdlg.h"

CDirectDraw g_draw;
//int g_DISPLAYWIDTH = 455;
//int g_DISPLAYHEIGHT = 495;
int g_DISPLAYWIDTH = 640;
int g_DISPLAYHEIGHT = 480;

HWND ghwnd;//gcdhandler���g�p����

IMPLEMENT_DYNAMIC(CAre_editerDlg, CMyDialogTab)


/*----------------------------------------------------------------------
	�\�z
------------------------------------------------------------------------*/
CAre_editerDlg::CAre_editerDlg(CWnd* pParent /*=NULL*/)
	: CMyDialogTab(CAre_editerDlg::IDD, pParent)
	, m_status(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	for(int i=0;i<MAX_TAB_PAGES;i++){
		m_tab_pages[i]=NULL;
	}
}

/*----------------------------------------------------------------------
	DDX
------------------------------------------------------------------------*/
void CAre_editerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STR_MAIN_STATUS, m_status);
}


/*----------------------------------------------------------------------
	���b�Z�[�W�}�b�v
------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CAre_editerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnTcnSelchangeTab1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


/*----------------------------------------------------------------------
	������
------------------------------------------------------------------------*/
BOOL CAre_editerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B
	ghwnd = m_hWnd;

	//���^�u�R���g���[���̍��ڐ���
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB1);
	pTab->InsertItem(0,"�t�@�C��");
	pTab->InsertItem(1,"��`�ҏW");
	pTab->InsertItem(2,"CELL�ҏW");
	pTab->InsertItem(3,"���̑�");

	//���e�^�u�̃y�[�W�ɑ�������_�C�A���O�𐶐�
	m_tab_pages[0] = new CDlgPage1();
	m_tab_pages[0]->Create(IDD_DIALOG_TAB1,this);
	m_tab_pages[1] = new CRecteditDlg();
	m_tab_pages[1]->Create(IDD_DIALOG_SUBTAB_RECT,this);
	m_tab_pages[2] = new CCelleditDlg();
	m_tab_pages[2]->Create(IDD_DIALOG_SUBTAB_CELL,this);
	m_tab_pages[3] = new CDlgOther();
	m_tab_pages[3]->Create(IDD_DIALOG_SUBTAB_OTHER,this);

	//���E�B���h�E���^�u�̈ʒu�ɍ����悤�Ɉړ�
	CRect r;
	for(int i=0;i<MAX_TAB_PAGES;i++){
		if(m_tab_pages[i]){
			m_tab_pages[i]->GetClientRect(&r);
			r.OffsetRect(482+185,30);
			m_tab_pages[i]->MoveWindow(&r,FALSE);
		}
	}

	//���q�E�C���h�E�𐶐�
	RECT rect = {10,10,10+g_DISPLAYWIDTH,10+g_DISPLAYHEIGHT};
	g_childwnd.Create(
					NULL,
					"AreEditer_ChildWindow",
					WS_CHILD | WS_VSCROLL | WS_HSCROLL,
					rect,
					this,
					1234	//ID �Ȃ�ł������́H
					);
	g_childwnd.ShowWindow(TRUE);

	//��Direct3D	������
	g_draw.Initialize(g_childwnd.m_hWnd,TRUE);

	OnTcnSelchangeTab1(0,0);

	//���R���{�{�b�N�X�̒l�Ƃ���������
	UpdateItems();
	
	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}



/*----------------------------------------------------------------------
// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B
	�E�E�E���Ƃ�
------------------------------------------------------------------------*/
void CAre_editerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


/*----------------------------------------------------------------------
	//���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
	//  �V�X�e�������̊֐����Ăяo���܂��B
	�E�E�E���Ƃ�
------------------------------------------------------------------------*/
HCURSOR CAre_editerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*-----------------------------------------------------------------------
	�^�u�R���g���[���̕ύX
-------------------------------------------------------------------------*/
void CAre_editerDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if(pResult)
		*pResult = 0;

	if (pResult) *pResult = 0;
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB1);
	int		i = pTab->GetCurSel();
	for(int j=0;j<MAX_TAB_PAGES;j++){
		if(m_tab_pages[j]){
			m_tab_pages[j]->ShowWindow(i == j ? SW_SHOW : SW_HIDE);
		}
	}

	if(i==1){
		g_childwnd.ChangeEditMode(FALSE);//��`�ҏW���[�h�ɕύX
		SetStatus("��`�ҏW���[�h");
	}
	if(i==2){
		g_childwnd.ChangeEditMode(TRUE);//�Z���ҏW���[�h�ɕύX
		SetStatus("�Z���ҏW���[�h");
	}
}

/*-----------------------------------------------------------------------
	�E�C���h�E�j��
-------------------------------------------------------------------------*/
BOOL CAre_editerDlg::DestroyWindow()
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	
	//�^�u�y�[�W�p�_�C�A���O�̔j��
	for(int i=0;i<MAX_TAB_PAGES;i++)
	{
		if(m_tab_pages[i]){
			m_tab_pages[i]->DestroyWindow();
			delete m_tab_pages[i];
			m_tab_pages[i] = NULL;
		}
	}
	//�q�E�C���h�E�̔j��
	g_childwnd.DestroyWindow();

	return CDialog::DestroyWindow();
}

/*-----------------------------------------------------------------------
	���̂ق��ɂłĂ�G�Z�X�e�[�^�X�̕�����X�V
-------------------------------------------------------------------------*/
void CAre_editerDlg::SetStatus(CString str)
{
	m_status = str;
	UpdateData(FALSE);
}


/*-----------------------------------------------------------------------
	�r�b�g�}�b�v�t�@�C�����擾
-------------------------------------------------------------------------*/
CString CAre_editerDlg::GetBMPFilename(UINT i)
{
	return ((CDlgPage1*)m_tab_pages[0])->GetBMPFilename(i);
}


/*-----------------------------------------------------------------------
	���w�^�u�R���g���[���ɏ��̍X�V��v��
-------------------------------------------------------------------------*/
void CAre_editerDlg::UpdateItems()
{
	for(int i=0;i<MAX_TAB_PAGES;i++)
	{
		if(m_tab_pages[i]){
			m_tab_pages[i]->UpdateItems();
		}
	}
}

/*----------------------------------------------------------------------
	�t�@�C���h���b�v
------------------------------------------------------------------------*/
void CAre_editerDlg::OnDropFiles(HDROP hDropInfo)
{
	char *pBuf = new char[MAX_PATH];
	UINT fileCount = ::DragQueryFile(  hDropInfo, 
					0,    
					pBuf, 
					MAX_PATH);

	CString str;
	str.Format("%s\n��ǂݍ��݂܂����H",pBuf);

	if(IDYES!=MessageBox(str,"",MB_YESNO)){
		delete [] pBuf;
		return;
	}

	theApp.LoadGCD(pBuf);
	delete [] pBuf;
}


/*----------------------------------------------------------------------
	�t�@�C���h���b�v
------------------------------------------------------------------------*/
void CAre_editerDlg::SpecialCtrl(UINT key)
{
	switch(key){
		case 0://blank next (rect)
			{
				((CRecteditDlg*)m_tab_pages[1])->BlankNext();
				theApp.InvalidateAll();
			}
			break;
		case 1://blank next (cell)
			{
				((CCelleditDlg*)m_tab_pages[2])->BlankNext();
				theApp.InvalidateAll();
			}
			break;
	}
}



void CAre_editerDlg::OnClose()
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	if (AfxMessageBox("�I������Ƃł����H", MB_YESNO | MB_ICONQUESTION, NULL) == IDYES)
		CMyDialogTab::OnClose();
}
