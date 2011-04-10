// gcd_tool03Dlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "gcd_tool03.h"
#include "gcd_tool03Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND ghwnd = NULL;
extern Cgcd_tool03App theApp;

// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Cgcd_tool03Dlg �_�C�A���O



Cgcd_tool03Dlg::Cgcd_tool03Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cgcd_tool03Dlg::IDD, pParent)
	, m_str_status(_T(""))
	, m_bCheck1(FALSE)
	, m_edit_number1(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cgcd_tool03Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATUS, m_str_status);
	DDX_Check(pDX, IDC_CHECK1, m_bCheck1);
	DDX_Text(pDX, IDC_EDIT1, m_edit_number1);
}

BEGIN_MESSAGE_MAP(Cgcd_tool03Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedButton7)
END_MESSAGE_MAP()


// Cgcd_tool03Dlg ���b�Z�[�W �n���h��

BOOL Cgcd_tool03Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B
	ghwnd = m_hWnd;
	m_str_status = "�t�@�C���̓h���b�O&�h���b�v�Ń��[�h����";
	m_bCheck1 = TRUE;
	UpdateData(FALSE);
	
	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

void Cgcd_tool03Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void Cgcd_tool03Dlg::OnPaint() 
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
HCURSOR Cgcd_tool03Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



/*--------------------------------------------------------------------------------
	�t�@�C���̃h���b�v
----------------------------------------------------------------------------------*/
void Cgcd_tool03Dlg::OnDropFiles( HDROP hDropInfo )
{
	char filename[1024];
	UINT fileCount = ::DragQueryFile(  hDropInfo, 
					0,    
					filename, 
					1024);
	theApp.Load(filename);
}


/*--------------------------------------------------------------------------------
	���`�f�[�^�̏���
----------------------------------------------------------------------------------*/
void Cgcd_tool03Dlg::OnBnClickedButton1()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	theApp.DeleteEmptyRect();
}


/*--------------------------------------------------------------------------------
	��`�ԍ��̐���
----------------------------------------------------------------------------------*/
void Cgcd_tool03Dlg::OnBnClickedButton2()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	theApp.SortRects();
}


/*--------------------------------------------------------------------------------
	�Z���ԍ��̐���
----------------------------------------------------------------------------------*/
void Cgcd_tool03Dlg::OnBnClickedButton3()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	UpdateData(TRUE);
	theApp.SortCells(m_bCheck1);
}



/*--------------------------------------------------------------------------------
	�ۑ�
----------------------------------------------------------------------------------*/
void Cgcd_tool03Dlg::OnBnClickedButton4()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	theApp.Save();
}



/*--------------------------------------------------------------------------------
	���`�����Q�Ƃ��Ă��Ȃ��Z�����폜
----------------------------------------------------------------------------------*/
void Cgcd_tool03Dlg::OnBnClickedButton5()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	theApp.DeleteEmptyCell();
}


/*--------------------------------------------------------------------------------
	�w��ԍ��ɋ�Z����ǉ�����
----------------------------------------------------------------------------------*/
void Cgcd_tool03Dlg::OnBnClickedButton6()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	UpdateData(TRUE);

	if(m_edit_number1>GCDMAX_CELLS){
		m_str_status = "���͂��ꂽ�l���s���ł�";
	}
	else{
		if(theApp.InsertEmptyCell(m_edit_number1)){
			m_str_status.Format("%d�Ԃɋ�̃Z����}�����܂���",m_edit_number1);
		}
		else m_str_status = "����Ɏ��s���܂���";
	}

	UpdateData(FALSE);
}

/*--------------------------------------------------------------------------------
	�F���V�I��1.0�ɂ���
----------------------------------------------------------------------------------*/
void Cgcd_tool03Dlg::OnBnClickedButton7()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	theApp.ForceRatio();
}
