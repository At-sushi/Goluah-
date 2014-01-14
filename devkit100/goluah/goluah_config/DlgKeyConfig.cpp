// DlgKeyConfig.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "goluah_config.h"
#include "DlgKeyConfig.h"
#include "DlgKBKeyGet.h"

CKeyCode2Str keycode;

// CDlgKeyConfig �_�C�A���O

IMPLEMENT_DYNAMIC(CDlgKeyConfig, CDialog)
CDlgKeyConfig::CDlgKeyConfig(CWnd* pParent , KEYCONFIG& cfg)
	: CDialog(CDlgKeyConfig::IDD, pParent)
	, sstr_1_1(_T(""))
	, sstr_1_2(_T(""))
	, sstr_1_3(_T(""))
	, sstr_1_4(_T(""))
	, sstr_1_5(_T(""))
	, sstr_1_6(_T(""))
	, sstr_1_7(_T(""))
	, sstr_1_8(_T(""))
	, sstr_2_1(_T(""))
	, sstr_2_2(_T(""))
	, sstr_2_3(_T(""))
	, sstr_2_4(_T(""))
	, sstr_2_5(_T(""))
	, sstr_2_6(_T(""))
	, sstr_2_7(_T(""))
	, sstr_2_8(_T(""))
{
	m_cfg = cfg;
}

CDlgKeyConfig::~CDlgKeyConfig()
{
}

void CDlgKeyConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEXT_1_1, sstr_1_1);
	DDX_Text(pDX, IDC_TEXT_1_2, sstr_1_2);
	DDX_Text(pDX, IDC_TEXT_1_3, sstr_1_3);
	DDX_Text(pDX, IDC_TEXT_1_4, sstr_1_4);
	DDX_Text(pDX, IDC_TEXT_1_5, sstr_1_5);
	DDX_Text(pDX, IDC_TEXT_1_6, sstr_1_6);
	DDX_Text(pDX, IDC_TEXT_1_7, sstr_1_7);
	DDX_Text(pDX, IDC_TEXT_1_8, sstr_1_8);
	DDX_Text(pDX, IDC_TEXT_2_1, sstr_2_1);
	DDX_Text(pDX, IDC_TEXT_2_2, sstr_2_2);
	DDX_Text(pDX, IDC_TEXT_2_3, sstr_2_3);
	DDX_Text(pDX, IDC_TEXT_2_4, sstr_2_4);
	DDX_Text(pDX, IDC_TEXT_2_5, sstr_2_5);
	DDX_Text(pDX, IDC_TEXT_2_6, sstr_2_6);
	DDX_Text(pDX, IDC_TEXT_2_7, sstr_2_7);
	DDX_Text(pDX, IDC_TEXT_2_8, sstr_2_8);
}


BEGIN_MESSAGE_MAP(CDlgKeyConfig, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_1_1, &CDlgKeyConfig::OnBnClickedButtonSetkey11)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_1_2, &CDlgKeyConfig::OnBnClickedButtonSetkey12)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_1_3, &CDlgKeyConfig::OnBnClickedButtonSetkey13)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_1_4, &CDlgKeyConfig::OnBnClickedButtonSetkey14)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_1_5, &CDlgKeyConfig::OnBnClickedButtonSetkey15)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_1_6, &CDlgKeyConfig::OnBnClickedButtonSetkey16)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_1_7, &CDlgKeyConfig::OnBnClickedButtonSetkey17)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_1_8, &CDlgKeyConfig::OnBnClickedButtonSetkey18)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_2_1, &CDlgKeyConfig::OnBnClickedButtonSetkey21)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_2_2, &CDlgKeyConfig::OnBnClickedButtonSetkey22)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_2_3, &CDlgKeyConfig::OnBnClickedButtonSetkey23)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_2_4, &CDlgKeyConfig::OnBnClickedButtonSetkey24)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_2_6, &CDlgKeyConfig::OnBnClickedButtonSetkey26)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_2_7, &CDlgKeyConfig::OnBnClickedButtonSetkey27)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_2_8, &CDlgKeyConfig::OnBnClickedButtonSetkey28)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_2_9, &CDlgKeyConfig::OnBnClickedButtonSetkey29)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CDlgKeyConfig::OnBnClickedButtonDefault)
	ON_BN_CLICKED(IDOK, &CDlgKeyConfig::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CDlgKeyConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO :  �����ɏ�������ǉ����Ă�������
	UpdateNames();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

// CDlgKeyConfig ���b�Z�[�W �n���h��
/*-----------------------------------------------------------------
	�L�[�ݒ�(1P)
-------------------------------------------------------------------*/
//��
void CDlgKeyConfig::OnBnClickedButtonSetkey11()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_left[0] = ret;
	UpdateNames();
}

//��
void CDlgKeyConfig::OnBnClickedButtonSetkey12()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_right[0] = ret;
	UpdateNames();
}

//��
void CDlgKeyConfig::OnBnClickedButtonSetkey13()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_up[0] = ret;
	UpdateNames();
}

//��
void CDlgKeyConfig::OnBnClickedButtonSetkey14()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_down[0] = ret;
	UpdateNames();
}

//�{�^��A
void CDlgKeyConfig::OnBnClickedButtonSetkey15()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_button[0][0] = ret;
	UpdateNames();
}

//�{�^��B
void CDlgKeyConfig::OnBnClickedButtonSetkey16()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_button[1][0] = ret;
	UpdateNames();
}

//�{�^��C
void CDlgKeyConfig::OnBnClickedButtonSetkey17()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_button[2][0] = ret;
	UpdateNames();
}

//�{�^��D
void CDlgKeyConfig::OnBnClickedButtonSetkey18()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_button[3][0] = ret;
	UpdateNames();
}

/*-----------------------------------------------------------------
	�L�[�ݒ�(2P)
-------------------------------------------------------------------*/
//��
void CDlgKeyConfig::OnBnClickedButtonSetkey21()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_left[1] = ret;
	UpdateNames();
}

//��
void CDlgKeyConfig::OnBnClickedButtonSetkey22()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_right[1] = ret;
	UpdateNames();
}

//��
void CDlgKeyConfig::OnBnClickedButtonSetkey23()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_up[1] = ret;
	UpdateNames();
}

//��
void CDlgKeyConfig::OnBnClickedButtonSetkey24()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_down[1] = ret;
	UpdateNames();
}

//�{�^��A
void CDlgKeyConfig::OnBnClickedButtonSetkey26()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_button[0][1] = ret;
	UpdateNames();
}

//�{�^��B
void CDlgKeyConfig::OnBnClickedButtonSetkey27()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_button[1][1] = ret;
	UpdateNames();
}

//�{�^��C
void CDlgKeyConfig::OnBnClickedButtonSetkey28()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_button[2][1] = ret;
	UpdateNames();
}

//�{�^��D
void CDlgKeyConfig::OnBnClickedButtonSetkey29()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_button[3][1] = ret;
	UpdateNames();
}

/*-----------------------------------------------------------------
	�\���X�V
-------------------------------------------------------------------*/
void CDlgKeyConfig::UpdateNames()
{
	//sstr_1_3.Format("�� : %s(%d)", keycode.GetStr( VK_CAPITAL ),m_cfg.key_up[0]);

	sstr_1_3.Format("�� : %s", keycode.GetStr( m_cfg.key_up[0] ));
	sstr_1_4.Format("�� : %s", keycode.GetStr( m_cfg.key_down[0] ));
	sstr_1_1.Format("�� : %s", keycode.GetStr( m_cfg.key_left[0] ));
	sstr_1_2.Format("�� : %s", keycode.GetStr( m_cfg.key_right[0] ));
	sstr_1_5.Format("A(��) : %s", keycode.GetStr( m_cfg.key_button[0][0] ));
	sstr_1_6.Format("B(��) : %s", keycode.GetStr( m_cfg.key_button[1][0] ));
	sstr_1_7.Format("C(��) : %s", keycode.GetStr( m_cfg.key_button[2][0] ));
	sstr_1_8.Format("D(��) : %s", keycode.GetStr( m_cfg.key_button[3][0] ));
	
	sstr_2_3.Format("�� : %s", keycode.GetStr( m_cfg.key_up[1] ));
	sstr_2_4.Format("�� : %s", keycode.GetStr( m_cfg.key_down[1] ));
	sstr_2_1.Format("�� : %s", keycode.GetStr( m_cfg.key_left[1] ));
	sstr_2_2.Format("�� : %s", keycode.GetStr( m_cfg.key_right[1] ));
	sstr_2_5.Format("A(��) : %s", keycode.GetStr( m_cfg.key_button[0][1] ));
	sstr_2_6.Format("B(��) : %s", keycode.GetStr( m_cfg.key_button[1][1] ));
	sstr_2_7.Format("C(��) : %s", keycode.GetStr( m_cfg.key_button[2][1] ));
	sstr_2_8.Format("D(��) : %s", keycode.GetStr( m_cfg.key_button[3][1] ));

	UpdateData(FALSE);
}

/*-----------------------------------------------------------------
	�L�[�R�[�h �� �����ϊ�
-------------------------------------------------------------------*/
CKeyCode2Str::CKeyCode2Str()
{
	//0x00-0x07
	while(m_list.size()<0x07+1)
	{
		Add("?");
	}

	Add("Back Space");	//8
	Add("Tab");		//9
	Add("?");			//a
	Add("?");			//b

	Add("Clear");		//c
	Add("Enter");		//d
	
	Add("?");			//e
	Add("?");			//f

	Add("SHIFT");
	Add("CONTROL");
	Add("Alt"); 
	Add("PAUSE");
	Add("Caps Lock");
	Add("����");
	
	Add("?");			//0x16

	Add("JUNJA");
	Add("FINAL");
	Add("KANJI");

	Add("?");			//0x1a

	Add("Esc"); 
	Add("�ϊ�");
	Add("���ϊ�"); 
	Add("ACCEPT");
	Add("���[�h�ύX");
	Add("�X�y�[�X");
	Add("Page Up");
	Add("Page Down");
	Add("End");
	Add("Home");
	Add("��");
	Add("��");
	Add("��");
	Add("��");
	Add("Select");
	Add("Print");
	Add("Execute"); 
	Add("Print Scr");
	Add("Insert");
	Add("Delete");
	Add("Help");

	Add("0");
	Add("1");
	Add("2");
	Add("3");
	Add("4");
	Add("5");
	Add("6");
	Add("7");
	Add("8");
	Add("9");

	
	if(m_list.size()!='9'+1)OutputDebugString("CKeyCode2Str : �`�F�b�N���s at 0x39\n");
	while(m_list.size()<0x40+1)
	{
		Add("?");
	}
	
	Add("A");
	Add("B");
	Add("C");
	Add("D");
	Add("E");
	Add("F");
	Add("G");
	Add("H");
	Add("I");
	Add("J");
	Add("K");
	Add("L");
	Add("M");
	Add("N");
	Add("O");
	Add("P");
	Add("Q");
	Add("R");
	Add("S");
	Add("T");
	Add("U");
	Add("V");
	Add("W");
	Add("X");
	Add("Y");
	Add("Z");

	Add("��Win");
	Add("�EWin");
	Add("�A�v��"); 
	Add("?");
	Add("SLEEP");

	if(m_list.size()!=0x5F+1)OutputDebugString("CKeyCode2Str : �`�F�b�N���s at 0x5F\n");
	
	Add("�e���L�[0");
	Add("�e���L�[1");
	Add("�e���L�[2");
	Add("�e���L�[3");
	Add("�e���L�[4");
	Add("�e���L�[5");
	Add("�e���L�[6");
	Add("�e���L�[7");
	Add("�e���L�[8");
	Add("�e���L�[9");

	Add("�e���L�[[ * ]");
	Add("�e���L�[[ + ]");
	Add("�e���L�[[Enter]");
	Add("�e���L�[[ - ]");
	Add("�e���L�[[ . ]");
	Add("�e���L�[[ / ]");

	Add("F1");
	Add("F2");
	Add("F3");
	Add("F4");
	Add("F5<���M�́L>");
	Add("F6");
	Add("F7");
	Add("F8");
	Add("F9");
	Add("F10");
	Add("F11");
	Add("F12");
	Add("F13");
	Add("F14");
	Add("F15");
	Add("F16");
	Add("F17");
	Add("F18");
	Add("F19");
	Add("F20");
	Add("F21");
	Add("F22");
	Add("F23");
	Add("F24");

	if(m_list.size()!=VK_F24+1)OutputDebugString("CKeyCode2Str : �`�F�b�N���s at 0x88\n");
	while(m_list.size()<0x8F+1)
	{
		Add("?");
	}

	Add("Num Lock");
	Add("Scroll Lock");
	Add("OEM1");
	Add("OEM2");
	Add("OEM3");
	Add("OEM4");
	Add("OEM5");

	if(m_list.size()!=0x96+1)OutputDebugString("CKeyCode2Str : �`�F�b�N���s at 0x96\n");
	while(m_list.size()<0x9F+1)
	{
		Add("?");
	}

	Add("��Shift"); 
	Add("�EShift") ;
	Add("��Ctrl");
	Add("�ECtrl") ;
	Add("��Alt");
	Add("�EAlt");

	if(m_list.size()!=0xA5+1)OutputDebugString("CKeyCode2Str : �`�F�b�N���s at 0xA5\n");

	Add("�u���E�U[�߂�]");
	Add("�u���E�U[�i��]");
	Add("�u���E�U[�c��C�i�΁j]");
	Add("�u���E�U[�u���C�N]");
	Add("�u���E�U[����]");
	Add("�u���E�U[���C�ɓ���]");
	Add("�u���E�U[�z�[���y�[�W]");
	Add("���f�B�A[�~���[�g]");
	Add("���f�B�A[����sage]");
	Add("���f�B�A[����age]");
	Add("���f�B�A[���̃g���b�N]");
	Add("���f�B�A[�O�̃g���b�N]");
	Add("���f�B�A[��~]");
	Add("���f�B�A[�Đ��^�ꎞ��~]");
	Add("���[��");
	Add("���f�B�A�I��");
	Add("�A�v���N���P");
	Add("�A�v���N���Q");

	if(m_list.size()!=0xB7+1)OutputDebugString("CKeyCode2Str : �`�F�b�N���s at 0xA5\n");
}

const char* CKeyCode2Str::GetStr(BYTE code)
{
	if(code >= m_list.size())
	{
		static char buffer[32]; // �����o�ɂ�������޸�
		
		sprintf(buffer, "�s��(%d��)", code);
		return buffer;
	}

	return m_list[code];
}

void CKeyCode2Str::Add(const char* str)
{
	char *tekito = new char [strlen(str)+1];
	strcpy(tekito,str);
	m_list.push_back(tekito);
}

CKeyCode2Str::~CKeyCode2Str()
{
	for(UINT i=0;i<m_list.size();i++)
	{
		delete [] m_list[i];
	}
}
void CDlgKeyConfig::OnBnClickedButtonDefault()
{
	g_config.SetDefaultConfigKB();
	m_cfg = g_config.keycfg;
	UpdateNames();
}

void CDlgKeyConfig::OnBnClickedOk()
{
	g_config.keycfg = m_cfg;
	OnOK();
}

void CDlgKeyConfig::OnCancel()
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	OnBnClickedOk();
	//CDialog::OnCancel();
}
