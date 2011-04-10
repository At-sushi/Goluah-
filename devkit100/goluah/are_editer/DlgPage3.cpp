#include "..\goluah_config\dlgpage3.h"
#include "..\goluah_config\dlgpage3.h"
#include "..\goluah_config\dlgpage3.h"
// DlgPage3.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "are_editer.h"
#include "DlgPage3.h"
#include "ChildWnd.h"


// CDlgPage3 �_�C�A���O

IMPLEMENT_DYNAMIC(CDlgPage3, CDialog)
CDlgPage3::CDlgPage3(CWnd* pParent /*=NULL*/)
	: CMyDialogTab(CDlgPage3::IDD, pParent)
	, m_str_info1(_T(""))
{
	m_mode = 7;
}

CDlgPage3::~CDlgPage3()
{
}

void CDlgPage3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INFO1, m_str_info1);
}


BEGIN_MESSAGE_MAP(CDlgPage3, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_BUTTON_RECT_KAS1, OnBnClickedButtonRectKas1)
	ON_BN_CLICKED(IDC_BUTTON_RECT_KAS2, OnBnClickedButtonRectKas2)
	ON_BN_CLICKED(IDC_BUTTON_RECT_KAS3, OnBnClickedButtonRectKas3)
	ON_BN_CLICKED(IDC_BUTTON_RECT_KUR1, OnBnClickedButtonRectKur1)
	ON_BN_CLICKED(IDC_BUTTON_RECT_KUR2, OnBnClickedButtonRectKur2)
	ON_BN_CLICKED(IDC_BUTTON_RECT_KUR3, OnBnClickedButtonRectKur3)
	ON_BN_CLICKED(IDC_BUTTON_RECT_ATK1, OnBnClickedButtonRectAtk1)
	ON_BN_CLICKED(IDC_BUTTON_RECT_ATK2, OnBnClickedButtonRectAtk2)
	ON_BN_CLICKED(IDC_BUTTON_RECT_ATK3, OnBnClickedButtonRectAtk3)
	ON_BN_CLICKED(IDC_RADIO6, OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO_NONE, OnBnClickedRadioNone)
END_MESSAGE_MAP()


// CDlgPage3 ���b�Z�[�W �n���h��





/*---------------------------------------------------------------
	�_�C�A���O�{�b�N�X����������
-----------------------------------------------------------------*/
BOOL CDlgPage3::OnInitDialog()
{
	CMyDialogTab::OnInitDialog();

	// TODO :  �����ɏ�������ǉ����Ă�������

	//���W�I�{�^��7�Ƀ`�F�b�N�����
	CButton *radio = (CButton*)GetDlgItem(IDC_RADIO_NONE);
	radio->SetCheck(1);
	m_mode = 0;
	EnableButtons(FALSE);//��`�w��{�^������

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*---------------------------------------------------------------
	�u��`����v�{�^�� ON/OFF
-----------------------------------------------------------------*/
void CDlgPage3::EnableButtons(BOOL f)
{
	CWnd *w;

	w = GetDlgItem(IDC_BUTTON_RECT_KAS1);
	w->EnableWindow(f);
	w = GetDlgItem(IDC_BUTTON_RECT_KAS2);
	w->EnableWindow(f);
	w = GetDlgItem(IDC_BUTTON_RECT_KAS3);
	w->EnableWindow(f);
	
	w = GetDlgItem(IDC_BUTTON_RECT_KUR1);
	w->EnableWindow(f);
	w = GetDlgItem(IDC_BUTTON_RECT_KUR2);
	w->EnableWindow(f);
	w = GetDlgItem(IDC_BUTTON_RECT_KUR3);
	w->EnableWindow(f);

	w = GetDlgItem(IDC_BUTTON_RECT_ATK1);
	w->EnableWindow(f);
	w = GetDlgItem(IDC_BUTTON_RECT_ATK2);
	w->EnableWindow(f);
	w = GetDlgItem(IDC_BUTTON_RECT_ATK3);
	w->EnableWindow(f);

	UpdateInformation();
}


/*---------------------------------------------------------------
	���W�I�{�^���ύX������
-----------------------------------------------------------------*/
//���I�t�Z�b�g
void CDlgPage3::OnBnClickedRadio1()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	EnableButtons(FALSE);
	m_mode = 0;
	g_childwnd.ChangeEditModeC(m_mode);
}

//�������`
void CDlgPage3::OnBnClickedRadio2()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	EnableButtons(TRUE);
	m_mode = 1;
	g_childwnd.ChangeEditModeC(m_mode);
}

//���d�S
void CDlgPage3::OnBnClickedRadio3()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	EnableButtons(FALSE);
	m_mode = 2;
	g_childwnd.ChangeEditModeC(m_mode);
}

//����]
void CDlgPage3::OnBnClickedRadio4()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	EnableButtons(FALSE);
	m_mode = 3;
	g_childwnd.ChangeEditModeC(m_mode);
}

//���g��k��
void CDlgPage3::OnBnClickedRadio5()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	EnableButtons(FALSE);
	m_mode = 4;
	g_childwnd.ChangeEditModeC(m_mode);
}

//���I�t�Z�b�g�i�������j
void CDlgPage3::OnBnClickedRadio6()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	EnableButtons(FALSE);
	m_mode = 5;
	g_childwnd.ChangeEditModeC(m_mode);
}

//���}�[�J�[�|�C���g
void CDlgPage3::OnBnClickedRadio7()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	EnableButtons(FALSE);
	m_mode = 6;
	g_childwnd.ChangeEditModeC(m_mode);
}

//��none
void CDlgPage3::OnBnClickedRadioNone()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	EnableButtons(FALSE);
	m_mode = 7;
	g_childwnd.ChangeEditModeC(m_mode);
}

/*---------------------------------------------------------------
	�\����Update
-----------------------------------------------------------------*/
void CDlgPage3::UpdateItems()
{
	UpdateInformation();
}

/*---------------------------------------------------------------
	�C���t�H���[�V�����X�V
-----------------------------------------------------------------*/
void CDlgPage3::UpdateInformation()
{
	CButton* rad = (CButton*)GetDlgItem(IDC_RADIO2);
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	
	if(rad->GetCheck()){
		m_str_info1.Format("�����蔻��ҏW���[�h");
	}
	else if(pc){
		m_str_info1.Format("���ݕҏW���̋�`�F%d (%d)",
							theApp.GetCurrentCellRectNO(),
							pc->cell[ theApp.GetCurrentCellRectNO() ].cdr
							);
	}
	else{
		m_str_info1 = "�G���[�F�Z�����̎擾�Ɏ��s";
	}

	UpdateData(FALSE);
}


/*---------------------------------------------------------------
	��`����{�^��
-----------------------------------------------------------------*/
void CDlgPage3::OnBnClickedButtonRectKas1()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	GCD_HANTEI *ph = theApp.GetCurrentHantei();

	RECT r = g_childwnd.GetCurrentRect();
	ph->kas[0] = r;

	CString str;
	str.Format("�d�Ȃ��`(1)���� : (%d,%d)-(%d,%d) , w:%d / h:%d",
		r.left,r.top,r.right,r.bottom, r.right-r.left , r.bottom-r.top );
	theApp.SetStatus(str);
	theApp.InvalidateAll();
}

void CDlgPage3::OnBnClickedButtonRectKas2()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	GCD_HANTEI *ph = theApp.GetCurrentHantei();

	RECT r = g_childwnd.GetCurrentRect();
	ph->kas[1] = r;

	CString str;
	str.Format("�d�Ȃ��`(2)���� : (%d,%d)-(%d,%d) , w:%d / h:%d",
		r.left,r.top,r.right,r.bottom, r.right-r.left , r.bottom-r.top );
	theApp.SetStatus(str);
	theApp.InvalidateAll();
}

void CDlgPage3::OnBnClickedButtonRectKas3()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	GCD_HANTEI *ph = theApp.GetCurrentHantei();

	RECT r = g_childwnd.GetCurrentRect();
	ph->kas[2] = r;

	CString str;
	str.Format("�d�Ȃ��`(3)���� : (%d,%d)-(%d,%d) , w:%d / h:%d",
		r.left,r.top,r.right,r.bottom, r.right-r.left , r.bottom-r.top );
	theApp.SetStatus(str);
	theApp.InvalidateAll();
}

void CDlgPage3::OnBnClickedButtonRectKur1()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	GCD_HANTEI *ph = theApp.GetCurrentHantei();

	RECT r = g_childwnd.GetCurrentRect();
	ph->kurai[0] = r;

	CString str;
	str.Format("��炢��`(1)���� : (%d,%d)-(%d,%d) , w:%d / h:%d",
		r.left,r.top,r.right,r.bottom, r.right-r.left , r.bottom-r.top );
	theApp.SetStatus(str);
	theApp.InvalidateAll();
}

void CDlgPage3::OnBnClickedButtonRectKur2()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	GCD_HANTEI *ph = theApp.GetCurrentHantei();

	RECT r = g_childwnd.GetCurrentRect();
	ph->kurai[1] = r;

	CString str;
	str.Format("��炢��`(2)���� : (%d,%d)-(%d,%d) , w:%d / h:%d",
		r.left,r.top,r.right,r.bottom, r.right-r.left , r.bottom-r.top );
	theApp.SetStatus(str);
	theApp.InvalidateAll();
}

void CDlgPage3::OnBnClickedButtonRectKur3()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	GCD_HANTEI *ph = theApp.GetCurrentHantei();

	RECT r = g_childwnd.GetCurrentRect();
	ph->kurai[2] = r;

	CString str;
	str.Format("��炢��`(3)���� : (%d,%d)-(%d,%d) , w:%d / h:%d",
		r.left,r.top,r.right,r.bottom, r.right-r.left , r.bottom-r.top );
	theApp.SetStatus(str);
	theApp.InvalidateAll();
}

void CDlgPage3::OnBnClickedButtonRectAtk1()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	GCD_HANTEI *ph = theApp.GetCurrentHantei();

	RECT r = g_childwnd.GetCurrentRect();
	ph->attack[0] = r;

	CString str;
	str.Format("�U����`(1)���� : (%d,%d)-(%d,%d) , w:%d / h:%d",
		r.left,r.top,r.right,r.bottom, r.right-r.left , r.bottom-r.top );
	theApp.SetStatus(str);
	theApp.InvalidateAll();
}

void CDlgPage3::OnBnClickedButtonRectAtk2()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	GCD_HANTEI *ph = theApp.GetCurrentHantei();

	RECT r = g_childwnd.GetCurrentRect();
	ph->attack[1] = r;

	CString str;
	str.Format("�U����`(2)���� : (%d,%d)-(%d,%d) , w:%d / h:%d",
		r.left,r.top,r.right,r.bottom, r.right-r.left , r.bottom-r.top );
	theApp.SetStatus(str);
	theApp.InvalidateAll();
}

void CDlgPage3::OnBnClickedButtonRectAtk3()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	GCD_HANTEI *ph = theApp.GetCurrentHantei();

	RECT r = g_childwnd.GetCurrentRect();
	ph->attack[2] = r;

	CString str;
	str.Format("�U����`(3)���� : (%d,%d)-(%d,%d) , w:%d / h:%d",
		r.left,r.top,r.right,r.bottom, r.right-r.left , r.bottom-r.top );
	theApp.SetStatus(str);
	theApp.InvalidateAll();
}

