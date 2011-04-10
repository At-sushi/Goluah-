#include "..\goluah_config\dlgpage2.h"
#include "..\goluah_config\dlgpage2.h"
#include "..\goluah_config\dlgpage2.h"
#include "..\goluah_config\dlgpage2.h"
// DlgPage2.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "are_editer.h"
#include "DlgPage2.h"
#include "DlgPage4.h"
#include "ChildWnd.h"

// CDlgPage2 �_�C�A���O

IMPLEMENT_DYNCREATE(CDlgPage2, CDialog)

CDlgPage2::CDlgPage2(CWnd* pParent /*=NULL*/)
	: CMyDialogTab(CDlgPage2::IDD, pParent)
	, m_skip_link(FALSE)
	, m_sell_link(FALSE)
	, m_str_define(_T(""))
	, m_current_index(0)
	, m_flag_zw(FALSE)
	, m_flag_zt(FALSE)
	, m_flag_scaBP(FALSE)
	, m_flag_rotBP(FALSE)
{
	m_current_index = 1;
	theApp.SetCurrentCellNOPointer( &m_current_index);

	CGCDHandler::GCDSetEmptyCell2(&m_CopiedCell,&m_CopiedHantei);
}

CDlgPage2::~CDlgPage2()
{
}

void CDlgPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_SKIPLINK, m_skip_link);
	DDX_Check(pDX, IDC_CHECK_LINK, m_sell_link);
	DDX_Text(pDX, IDC_EDIT6, m_str_define);
	DDX_Text(pDX, IDC_EDIT1, m_current_index);
	DDX_Check(pDX, IDC_CHECK_NOZW, m_flag_zw);
	DDX_Check(pDX, IDC_CHECK_NOZT, m_flag_zt);
	DDX_Check(pDX, IDC_CHECK_SCA_BASEPOINT, m_flag_scaBP);
	DDX_Check(pDX, IDC_CHECK_ROT_BASEPOINT, m_flag_rotBP);
}

BEGIN_MESSAGE_MAP(CDlgPage2, CDialog)
	ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_BN_CLICKED(IDC_BUTTON_PREV10, OnBnClickedButtonPrev10)
	ON_BN_CLICKED(IDC_BUTTON_NEXT10, OnBnClickedButtonNext10)
	ON_BN_CLICKED(IDC_BUTTON_PREV_FIND, OnBnClickedButtonPrevFind)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_FIND, OnBnClickedButtonNextFind)
	ON_BN_CLICKED(IDC_BUTTON_PREV_BLANK, OnBnClickedButtonPrevBlank)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_BLANK, OnBnClickedButtonNextBlank)
	ON_BN_CLICKED(IDC_BUTTON_COPY, OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_PASTE, OnBnClickedButtonPaste)
	ON_BN_CLICKED(IDC_BUTTON_ERASE, OnBnClickedButtonErase)
	ON_BN_CLICKED(IDC_CHECK_LINK, OnBnClickedCheckLink)
	ON_EN_CHANGE(IDC_EDIT6, OnEnChangeEdit6)
	ON_BN_CLICKED(IDC_CHECK_NOZW, OnBnClickedCheckNozw)
	ON_BN_CLICKED(IDC_CHECK_NOZT, OnBnClickedCheckNozt)
	ON_BN_CLICKED(IDC_CHECK_SCA_BASEPOINT, OnBnClickedCheckScaBasepoint)
	ON_BN_CLICKED(IDC_CHECK_ROT_BASEPOINT, OnBnClickedCheckRotBasepoint)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CDlgPage2)
END_DHTML_EVENT_MAP()



/*--------------------------------------------------------------
	�_�C�A���O�{�b�N�X����������
----------------------------------------------------------------*/
BOOL CDlgPage2::OnInitDialog()
{
	CDialog::OnInitDialog();

	//�X�s���R���g���[���̐ݒ�
	CSpinButtonCtrl* spin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN1);
	if(!spin){
		OutputDebugString("CDlgRPage1::OnInitDialog spin�擾���s�H�H\n");
		throw;
	}
	spin->SetRange32(1,1023);
	spin->SetPos( m_current_index );

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}


/*--------------------------------------------------------------
	CELL#����(�H)�ύX������
----------------------------------------------------------------*/
void CDlgPage2::OnEnChangeEdit1()
{
	// TODO :  �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����Ă��������B
	DWORD prv = m_current_index;
	UpdateData(TRUE);

	if(m_current_index<1 || m_current_index>1023){
		m_current_index = prv;
		theApp.SetStatus("�Z���ԍ��ɕs���Ȓl�����͂��ꂽ");
		return;
	}
	OnChangeIndex();
}


/*--------------------------------------------------------------
	CELL#���R�A�X�s���R���g���[������
----------------------------------------------------------------*/
void CDlgPage2::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	*pResult = 0;

	UINT next = m_current_index + pNMUpDown->iDelta;
	if(next<1 || next>1023){
		return;
	}
	m_current_index = next;
	OnChangeIndex();
}


/*--------------------------------------------------------------
	�Z���ԍ��ړ��{�^������
----------------------------------------------------------------*/
void CDlgPage2::OnBnClickedButtonPrev10()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if(m_current_index<11)m_current_index=1;
	else m_current_index-=10;

	OnChangeIndex();
}

void CDlgPage2::OnBnClickedButtonNext10()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_current_index+=10;
	if(m_current_index>1023)m_current_index=1023;

	OnChangeIndex();
}

void CDlgPage2::OnBnClickedButtonPrevFind()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if(m_current_index==1){
		theApp.SetStatus("Find Previous : �s���~�܂�");
		return;
	}
	CGCDHandler* pdat = theApp.GetGCD();

	UINT next = m_current_index;
	next--;
	while(pdat->IsEmptyCell(next)){
		if(next==1){
			theApp.SetStatus("Find Previous : ����܂���");
			return;
		}
		next--;
	}
	m_current_index = next;
	OnChangeIndex();
}

void CDlgPage2::OnBnClickedButtonNextFind()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if(m_current_index==1023){
		theApp.SetStatus("Find Next : �s���~�܂肾��");
		return;
	}
	CGCDHandler* pdat = theApp.GetGCD();

	UINT next = m_current_index;
	next++;
	while(pdat->IsEmptyCell(next)){
		if(next==1023){
			theApp.SetStatus("Find Next : ����܂���");
			return;
		}
		next++;
	}
	m_current_index = next;
	OnChangeIndex();
}

void CDlgPage2::OnBnClickedButtonPrevBlank()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if(m_current_index==1){
		theApp.SetStatus("Blank Previous : �s���~�܂�");
		return;
	}
	CGCDHandler* pdat = theApp.GetGCD();

	UINT next = m_current_index;
	next--;
	while(!pdat->IsEmptyCell(next)){
		if(next==1){
			theApp.SetStatus("Blank Previous : ����܂���");
			return;
		}
		next--;
	}
	m_current_index = next;
	OnChangeIndex();
}

void CDlgPage2::OnBnClickedButtonNextBlank()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if(m_current_index==1023){
		theApp.SetStatus("Blank Next : �s���~�܂肾��");
		return;
	}
	CGCDHandler* pdat = theApp.GetGCD();

	UINT next = m_current_index;
	next++;
	while(!pdat->IsEmptyCell(next)){
		if(next==1023){
			theApp.SetStatus("Blank Next : ����܂���");
			return;
		}
		next++;
	}
	m_current_index = next;
	OnChangeIndex();
}

/*--------------------------------------------------------------
	�Z���u�R�s�[�v����
----------------------------------------------------------------*/
void CDlgPage2::OnBnClickedButtonCopy()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_CopiedCell = *theApp.GetCurrentCell();
	m_CopiedHantei = *theApp.GetCurrentHantei();
	theApp.SetStatus("�Z���f�[�^���R�s�[���܂���");
}

/*--------------------------------------------------------------
	�Z���u�y�[�X�g�v����
----------------------------------------------------------------*/
void CDlgPage2::OnBnClickedButtonPaste()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if(IDYES!=MessageBox("�����́H","�Z���f�[�^�\��t��",MB_YESNO)){
		theApp.SetStatus("�Z���f�[�^�\��t���L�����Z��");
		return;
	}

	*theApp.GetCurrentCell() = m_CopiedCell;
	*theApp.GetCurrentHantei() = m_CopiedHantei;
	theApp.SetStatus("�Z���f�[�^��\��t���܂���");
	
	g_childwnd.Dirty();
}

/*--------------------------------------------------------------
	�Z���u�����v����
----------------------------------------------------------------*/
void CDlgPage2::OnBnClickedButtonErase()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if(IDYES!=MessageBox("�����́H","�Z���f�[�^����",MB_YESNO)){
		theApp.SetStatus("�Z���f�[�^���������L�����Z��");
		return;
	}

	CGCDHandler::GCDSetEmptyCell2(theApp.GetCurrentCell(),theApp.GetCurrentHantei());
	g_childwnd.Dirty();
}

/*--------------------------------------------------------------
	#define ������ύX������
----------------------------------------------------------------*/
void CDlgPage2::OnEnChangeEdit6()
{
	// TODO :  �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����Ă��������B
	UpdateData(TRUE);
	CGCDHandler *gcd = theApp.GetGCD();
	strcpy(gcd->GetName(m_current_index),m_str_define.GetBuffer());
}


/*--------------------------------------------------------------
	���݃Z���ԍ��ύX������
----------------------------------------------------------------*/
void CDlgPage2::OnChangeIndex()
{
	//#define�̕�����ύX
	CGCDHandler *gcd = theApp.GetGCD();
	m_str_define.Format("%s",gcd->GetName(m_current_index));

	GCD_CELL2 *pc = theApp.GetCurrentCell();
	if(pc){
		//�����N�t���O�`�F�b�N�{�b�N�X�ݒ�
		m_sell_link = (pc->flag & GCDCELL2_LINK) ? TRUE : FALSE;
		m_flag_zw = (pc->flag & GCDCELL2_DISABLE_ZW) ? TRUE : FALSE;
		m_flag_zt = (pc->flag & GCDCELL2_DISABLE_ZT) ? TRUE : FALSE;
		m_flag_scaBP = (pc->flag & GCDCELL2_SCA_GCENTER) ? TRUE : FALSE;
		m_flag_rotBP = (pc->flag & GCDCELL2_ROT_BASEPOINT) ? TRUE : FALSE;
	}

	UpdateData(FALSE);

	CDlgPage4::m_pinstance->UpdateItems();
	g_childwnd.Dirty();
	

	//�X�s���R���g���[���|�W�V�����ύX
	CSpinButtonCtrl* spin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN1);
	if(!spin){
		OutputDebugString("CDlgPage2::OnChangeIndex spin�擾���s�H�H\n");
		throw;
	}
	spin->SetPos(m_current_index);
}


/*--------------------------------------------------------------
	�\�����X�V
----------------------------------------------------------------*/
void CDlgPage2::UpdateItems()
{
	OnChangeIndex();
}

/*--------------------------------------------------------------
	�t���OON/OFF������
----------------------------------------------------------------*/

//�Z�������N
void CDlgPage2::OnBnClickedCheckLink()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	UpdateData(TRUE);
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	if(!pc){
		theApp.SetStatus("�����N�t���O���쎸�s");
		return;
	}

	//�X�y���~�X���Ă邯�ǥ��
	if(m_sell_link){
		pc->flag |= GCDCELL2_LINK;
		theApp.SetStatus("�����N�t���O ON");
	}
	else{
		pc->flag &= ~GCDCELL2_LINK;
		theApp.SetStatus("�����N�t���O OFF");
	}

	g_childwnd.Dirty();
}

//Z-Write
void CDlgPage2::OnBnClickedCheckNozw()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	UpdateData(TRUE);
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	if(!pc){
		theApp.SetStatus("ZW�t���O���쎸�s");
		return;
	}

	if(m_flag_zw){
		pc->flag |= GCDCELL2_DISABLE_ZW;
		theApp.SetStatus("no-ZW�t���O ON");
	}
	else{
		pc->flag &= ~GCDCELL2_DISABLE_ZW;
		theApp.SetStatus("no-ZW�t���O OFF");
	}

	g_childwnd.Dirty();
}

//Z-Test
void CDlgPage2::OnBnClickedCheckNozt()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	UpdateData(TRUE);
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	if(!pc){
		theApp.SetStatus("ZT�t���O���쎸�s");
		return;
	}

	if(m_flag_zt){
		pc->flag |= GCDCELL2_DISABLE_ZT;
		theApp.SetStatus("no-ZT�t���O ON");
	}
	else{
		pc->flag &= ~GCDCELL2_DISABLE_ZT;
		theApp.SetStatus("no-ZT�t���O OFF");
	}

	g_childwnd.Dirty();
}

//�d�S���S�X�P�[��
void CDlgPage2::OnBnClickedCheckScaBasepoint()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	UpdateData(TRUE);
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	if(!pc){
		theApp.SetStatus("�d�S���S�X�P�[���t���O���쎸�s");
		return;
	}

	if(m_flag_scaBP){
		pc->flag |= GCDCELL2_SCA_GCENTER;
		theApp.SetStatus("�d�S���S�X�P�[���t���O ON");
	}
	else{
		pc->flag &= ~GCDCELL2_SCA_GCENTER;
		theApp.SetStatus("�d�S���S�X�P�[���t���O OFF");
	}

	g_childwnd.Dirty();
}

//�������S��]
void CDlgPage2::OnBnClickedCheckRotBasepoint()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	UpdateData(TRUE);
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	if(!pc){
		theApp.SetStatus("�������S��]�t���O���쎸�s");
		return;
	}

	if(m_flag_rotBP){
		pc->flag |= GCDCELL2_ROT_BASEPOINT;
		theApp.SetStatus("�������S��]�t���O ON");
	}
	else{
		pc->flag &= ~GCDCELL2_ROT_BASEPOINT;
		theApp.SetStatus("�������S��]�t���O OFF");
	}
	g_childwnd.Dirty();
}


