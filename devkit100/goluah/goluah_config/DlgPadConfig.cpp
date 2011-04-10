// DlgPadConfig.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "goluah_config.h"
#include "DlgPadConfig.h"


// CDlgPadConfig �_�C�A���O

IMPLEMENT_DYNAMIC(CDlgPadConfig, CDialog)
CDlgPadConfig::CDlgPadConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPadConfig::IDD, pParent)
	, m_padindex(0)
	, m_buttonA(0)
	, m_buttonB(0)
	, m_buttonC(0)
	, m_buttonD(0)
{
}

CDlgPadConfig::~CDlgPadConfig()
{
}

void CDlgPadConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_PADINDEX, m_padindex);
	DDX_CBIndex(pDX, IDC_COMBO_PADBUTTON_A, m_buttonA);
	DDX_CBIndex(pDX, IDC_COMBO_PADBUTTON_B, m_buttonB);
	DDX_CBIndex(pDX, IDC_COMBO_PADBUTTON_C, m_buttonC);
	DDX_CBIndex(pDX, IDC_COMBO_PADBUTTON_D, m_buttonD);
}


BEGIN_MESSAGE_MAP(CDlgPadConfig, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_PADINDEX, &CDlgPadConfig::OnCbnSelchangeComboPadindex)
END_MESSAGE_MAP()


// CDlgPadConfig ���b�Z�[�W �n���h��

/*---------------------------------------------------------------
	������
-----------------------------------------------------------------*/
BOOL CDlgPadConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO :  �����ɏ�������ǉ����Ă�������
	m_cfg = g_config.padcfg;

	//�R���{�{�b�N�X�ɍ��ړ˂�����
	CComboBox* combo;
	RECT r;
	int i;

	//�p�b�h�C���f�b�N�X
	combo = (CComboBox*)GetDlgItem(IDC_COMBO_PADINDEX);
	if(!combo){
		throw;
	}
	r.left	= 85;
	r.top	= 20;
	r.right	= r.left+100;
	r.bottom= r.top +300;
	combo->MoveWindow(&r,FALSE);
	char padname[16];
	for(i=0;i<NUM_PAD;i++)
	{
		sprintf(padname,"�p�b�h%d",i+1);
		combo->InsertString(i, padname );
	}
	combo->SetCurSel(0);

	//�{�^��
	int comboid[] ={
		IDC_COMBO_PADBUTTON_A,
		IDC_COMBO_PADBUTTON_B,
		IDC_COMBO_PADBUTTON_C,
		IDC_COMBO_PADBUTTON_D,
	};
	for(i=0;i<4;i++)
	{
		combo = (CComboBox*)GetDlgItem( comboid[i] );
		if(!combo){
			throw;
		}
		r.left	= 110;
		r.top	= 80+27*i;
		r.right	= r.left+100;
		r.bottom= r.top +300;
		combo->MoveWindow(&r,FALSE);
		combo->InsertString(0, "�{�^��1" );
		combo->InsertString(1, "�{�^��2" );
		combo->InsertString(2, "�{�^��3" );
		combo->InsertString(3, "�{�^��4" );
		combo->InsertString(4, "�{�^��5" );
		combo->InsertString(5, "�{�^��6" );
		combo->InsertString(6, "�{�^��7" );
		combo->InsertString(7, "�{�^��8" );
		combo->InsertString(8, "�{�^��9" );
		combo->InsertString(9, "�{�^��10" );
		combo->InsertString(10, "�{�^��11" );
		combo->InsertString(11, "�{�^��12" );
		combo->SetCurSel(0);
	}

	m_crnt_pad = m_padindex = 0;
	m_buttonA = m_cfg.pad_button[0][m_crnt_pad];
	m_buttonB = m_cfg.pad_button[1][m_crnt_pad];
	m_buttonC = m_cfg.pad_button[2][m_crnt_pad];
	m_buttonD = m_cfg.pad_button[3][m_crnt_pad];
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*---------------------------------------------------------------
	�I������
-----------------------------------------------------------------*/
void CDlgPadConfig::OnOK()
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	UpdateData(TRUE);
	m_cfg.pad_button[0][m_crnt_pad] = m_buttonA;
	m_cfg.pad_button[1][m_crnt_pad] = m_buttonB;
	m_cfg.pad_button[2][m_crnt_pad] = m_buttonC;
	m_cfg.pad_button[3][m_crnt_pad] = m_buttonD;
	m_crnt_pad = m_padindex;

	g_config.padcfg = m_cfg;
	CDialog::OnOK();
}

/*---------------------------------------------------------------
	�p�b�h�ύX������
-----------------------------------------------------------------*/
void CDlgPadConfig::OnCbnSelchangeComboPadindex()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	UpdateData(TRUE);

	m_cfg.pad_button[0][m_crnt_pad] = m_buttonA;
	m_cfg.pad_button[1][m_crnt_pad] = m_buttonB;
	m_cfg.pad_button[2][m_crnt_pad] = m_buttonC;
	m_cfg.pad_button[3][m_crnt_pad] = m_buttonD;

	m_crnt_pad = m_padindex;
	
	m_buttonA = m_cfg.pad_button[0][m_crnt_pad];
	m_buttonB = m_cfg.pad_button[1][m_crnt_pad];
	m_buttonC = m_cfg.pad_button[2][m_crnt_pad];
	m_buttonD = m_cfg.pad_button[3][m_crnt_pad];
	UpdateData(FALSE);
}

void CDlgPadConfig::OnCancel()
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	OnOK();
//	CDialog::OnCancel();
}
