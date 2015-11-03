// goluah_configDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "goluah_config.h"
#include "goluah_configDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "DlgPage1.h"
#include "DlgPage2.h"
#include "DlgPage3.h"
#include "DlgOther.h"


IMPLEMENT_DYNAMIC(CCfgDialogBase,CDialog)
CCfgDialogBase::CCfgDialogBase(UINT tn,CWnd* p) : CDialog(tn,p)
{
}

// Cgoluah_configDlg �_�C�A���O



Cgoluah_configDlg::Cgoluah_configDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cgoluah_configDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	for(int i=0;i<MAX_TAB_PAGES;i++){
		m_tab_pages[i]=NULL;
	}
}

void Cgoluah_configDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

#define ON_WM_PAINT() \
	{ WM_PAINT, 0, 0, 0, AfxSig_vv, \
		(AFX_PMSG)(AFX_PMSGW) \
		(static_cast< void (AFX_MSG_CALL CWnd::*)(void) > (&Cgoluah_configDlg::OnPaint)) },

#define ON_WM_QUERYDRAGICON() \
	{ WM_QUERYDRAGICON, 0, 0, 0, AfxSig_hv, \
		(AFX_PMSG)(AFX_PMSGW) \
		(static_cast< HCURSOR (AFX_MSG_CALL CWnd::*)(void) > (&Cgoluah_configDlg::OnQueryDragIcon)) },

#define ON_WM_DESTROY() \
	{ WM_DESTROY, 0, 0, 0, AfxSig_vv, \
		(AFX_PMSG)(AFX_PMSGW) \
		(static_cast< void (AFX_MSG_CALL CWnd::*)(void) > (&Cgoluah_configDlg::OnDestroy)) },

#define ON_WM_KEYDOWN() \
	{ WM_KEYDOWN, 0, 0, 0, AfxSig_vwww, \
		(AFX_PMSG)(AFX_PMSGW) \
		(static_cast< void (AFX_MSG_CALL CWnd::*)(UINT, UINT, UINT) > (&Cgoluah_configDlg::OnKeyDown)) },

BEGIN_MESSAGE_MAP(Cgoluah_configDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &Cgoluah_configDlg::OnTcnSelchangeTab1)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &Cgoluah_configDlg::OnBnClickedOk)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BUTTON_RESTORE_ALL, &Cgoluah_configDlg::OnBnClickedButtonRestoreAll)
END_MESSAGE_MAP()


// Cgoluah_configDlg ���b�Z�[�W �n���h��

BOOL Cgoluah_configDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B

	//���^�u�R���g���[���̍��ڐ���
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB1);
	pTab->InsertItem(0,"�`��ݒ�");
	pTab->InsertItem(1,"�Q�[���ݒ�");
	pTab->InsertItem(2,"�V�X�e���ݒ�");
	pTab->InsertItem(3,"�V�X�e���ݒ�(2)");

	//���e�^�u�̃y�[�W�ɑ�������_�C�A���O�𐶐�
	m_tab_pages[0] = new CDlgPage1();
	m_tab_pages[0]->Create(IDD_DIALOG_PAGE1,this);
	m_tab_pages[1] = new CDlgPage2();
	m_tab_pages[1]->Create(IDD_DIALOG_PAGE2,this);
	m_tab_pages[2] = new CDlgPage3();
	m_tab_pages[2]->Create(IDD_DIALOG_PAGE3,this);
	m_tab_pages[3] = new CDlgOther();
	m_tab_pages[3]->Create(IDD_DIALOG_PAGE4,this);

	//���E�B���h�E���^�u�̈ʒu�ɍ����悤�Ɉړ�
	CRect r;
	for(int i=0;i<MAX_TAB_PAGES;i++){
		if(m_tab_pages[i]){
			m_tab_pages[i]->GetClientRect(&r);
			r.OffsetRect(20,35);
			m_tab_pages[i]->MoveWindow(&r,FALSE);
		}
	}
	OnTcnSelchangeTab1(0,0);

	if(!g_config.readok1)
	{
		MessageBox("system/config.dat �̓ǂݍ��݂Ɏ��s���܂���","�x��");
	}
	if(!g_config.readok2)
	{
		MessageBox("system/config2.dat �̓ǂݍ��݂Ɏ��s���܂���","�x��");
	}
	
	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void Cgoluah_configDlg::OnPaint() 
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

//���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR Cgoluah_configDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Cgoluah_configDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
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
}



void Cgoluah_configDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
}

BOOL Cgoluah_configDlg::DestroyWindow()
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
	return CDialog::DestroyWindow();
}

void Cgoluah_configDlg::OnBnClickedOk()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B

	//�p�~����
	//Cancel�f���l�̑�����s��
}

void Cgoluah_configDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void Cgoluah_configDlg::OnBnClickedButtonRestoreAll()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	g_config.SetDefaultConfig2();
	g_config.SetDefaultConfigAss();
	g_config.SetDefaultConfigKB();
	g_config.SetDefaultConfigPad();
	for(int i=0;i<MAX_TAB_PAGES;i++)
	{
		m_tab_pages[i]->Reset();
	}
}

void Cgoluah_configDlg::OnCancel()
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	for(int i=0;i<MAX_TAB_PAGES;i++)
	{
		if(m_tab_pages[i]){
			m_tab_pages[i]->UpdateSettings();
		}
	}

	BOOL fail=FALSE;
	if(! g_config.SaveConfig() ){
		MessageBox("�t�@�C���Asystem/config.dat �̕ۑ��Ɏ��s���܂���","����[");
		fail = TRUE;
	}
	if(! g_config.SaveConfig2() ){
		MessageBox("�t�@�C���Asystem/config2.dat �̕ۑ��Ɏ��s���܂���","����[");
		fail = FALSE;
	}

	if(!fail) CDialog::OnCancel();
}
