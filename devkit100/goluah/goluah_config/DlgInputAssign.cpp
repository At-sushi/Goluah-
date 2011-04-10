// DlgInputAssign.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "goluah_config.h"
#include "DlgInputAssign.h"
#include ".\dlginputassign.h"


// CDlgInputAssign �_�C�A���O

IMPLEMENT_DYNAMIC(CDlgInputAssign, CDialog)
CDlgInputAssign::CDlgInputAssign(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputAssign::IDD, pParent)
	, m_kb1(0)
	, m_kb2(0)
	, m_pad1(0)
	, m_pad2(0)
	, m_pad3(0)
	, m_pad4(0)
	, m_pad5(0)
	, m_pad6(0)
{
}

CDlgInputAssign::~CDlgInputAssign()
{
}

void CDlgInputAssign::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_KB1, m_kb1);
	DDX_CBIndex(pDX, IDC_COMBO_KB2, m_kb2);
	DDX_CBIndex(pDX, IDC_COMBO_PAD1, m_pad1);
	DDX_CBIndex(pDX, IDC_COMBO_PAD2, m_pad2);
	DDX_CBIndex(pDX, IDC_COMBO_PAD3, m_pad3);
	DDX_CBIndex(pDX, IDC_COMBO_PAD4, m_pad4);
	DDX_CBIndex(pDX, IDC_COMBO_PAD5, m_pad5);
	DDX_CBIndex(pDX, IDC_COMBO_PAD6, m_pad6);
}


BEGIN_MESSAGE_MAP(CDlgInputAssign, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgInputAssign::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_PAD3, &CDlgInputAssign::OnCbnSelchangeComboPad3)
	ON_CBN_SELCHANGE(IDC_COMBO_KB1, &CDlgInputAssign::OnCbnSelchangeComboKb1)
END_MESSAGE_MAP()


// CDlgInputAssign ���b�Z�[�W �n���h��

BOOL CDlgInputAssign::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO :  �����ɏ�������ǉ����Ă�������

	SetComboItem(IDC_COMBO_KB1, 60,10);
	SetComboItem(IDC_COMBO_KB2, 60,10);
	SetComboItem(IDC_COMBO_PAD1, 60,10);
	SetComboItem(IDC_COMBO_PAD2, 60,10);
	SetComboItem(IDC_COMBO_PAD3, 60,10);
	SetComboItem(IDC_COMBO_PAD4, 60,10);
	SetComboItem(IDC_COMBO_PAD5, 60,10);
	SetComboItem(IDC_COMBO_PAD6, 60,10);

	m_cfg = g_config.inputAssign;

	m_kb1 = m_cfg.kb[0];
	m_kb2 = m_cfg.kb[1];
	m_pad1 = m_cfg.pad[0];
	m_pad2 = m_cfg.pad[1];
	m_pad3 = m_cfg.pad[2];
	m_pad4 = m_cfg.pad[3];
	m_pad5 = m_cfg.pad[4];
	m_pad6 = m_cfg.pad[5];
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

void CDlgInputAssign::OnOK()
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	UpdateData(TRUE);

	//�L�[�{�[�h�d���`�F�b�N
/*	if(m_cfg.kb[0]==m_cfg.kb[1]){
		MessageBox("�L�[�{�[�h�̊��蓖�Ă��d�����Ă��܂�","�ʖڂ��E�E�E�I ���Ȃ������E�E�E�I�I");
		return;
	}
	//�p�b�h�d���`�F�b�N
	for(int i=0;i<NUM_PAD;i++)
	{
		for(int j=i+1;j<NUM_PAD;j++)
		{
			if(m_cfg.pad[i]==m_cfg.pad[j])
			{
				CString str;
				str.Format("�p�b�h%d��%d�̊��蓖�Ă��d�����Ă��܂�",i,j);
				MessageBox(str,"�ʖڂ��E�E�E�I ���Ȃ������E�E�E�I�I");
				return;
			}
		}
	}*/

	m_cfg.kb[0] = m_kb1;
	m_cfg.kb[1] = m_kb2;
	m_cfg.pad[0]= m_pad1;
	m_cfg.pad[1]= m_pad2;
	m_cfg.pad[2]= m_pad3;
	m_cfg.pad[3]= m_pad4;
	m_cfg.pad[4]= m_pad5;
	m_cfg.pad[5]= m_pad6;

	g_config.inputAssign = m_cfg;
	CDialog::OnOK();
}

void CDlgInputAssign::SetComboItem(int id,int left,int top)
{
	CComboBox* combo = (CComboBox*)GetDlgItem( id );
	if(!combo){
		throw;
	}
	combo->InsertString(0, "PLAYER1" );
	combo->InsertString(1, "PLAYER2" );
	combo->InsertString(2, "PLAYER3" );
	combo->InsertString(3, "PLAYER4" );
	combo->InsertString(4, "PLAYER5" );
	combo->InsertString(5, "PLAYER6" );
}

void CDlgInputAssign::OnBnClickedOk()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	OnOK();
}

void CDlgInputAssign::OnCbnSelchangeComboPad3()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
}

void CDlgInputAssign::OnCancel()
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	//CDialog::OnCancel();
	OnOK();
}

void CDlgInputAssign::OnCbnSelchangeComboKb1()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
}
