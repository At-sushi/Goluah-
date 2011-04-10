// DlgRPage1.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "are_editer.h"
#include "DlgRPage1.h"
#include "ChildWnd.h"


// CDlgRPage1 �_�C�A���O

IMPLEMENT_DYNAMIC(CDlgRPage1, CMyDialogTab)
CDlgRPage1::CDlgRPage1(CWnd* pParent /*=NULL*/)
	: CMyDialogTab(CDlgRPage1::IDD, pParent)
	, m_current_index(0)
	, m_radio_editmode(0)
	, m_str_info1(_T(""))
	, m_str_info2(_T(""))
	, m_selected_bmp(0)
{
	m_current_index = 1;
	m_selected_bmp = 0;
	theApp.SetCurrentRectNOPointer( &m_current_index );
}

CDlgRPage1::~CDlgRPage1()
{
}

void CDlgRPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_current_index);
	DDV_MinMaxUInt(pDX, m_current_index, 1, 1024);
	DDX_Text(pDX, IDC_STR_INFO1, m_str_info1);
	DDX_Text(pDX, IDC_STR_INFO2, m_str_info2);
	DDX_CBIndex(pDX, IDC_COMBO1, m_selected_bmp);
}


BEGIN_MESSAGE_MAP(CDlgRPage1, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON_PREV10, OnBnClickedButtonPrev10)
	ON_BN_CLICKED(IDC_BUTTON_NEXT10, OnBnClickedButtonNext10)
	ON_BN_CLICKED(IDC_BUTTON_PREV_FIND, OnBnClickedButtonPrevFind)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_FIND, OnBnClickedButtonNextFind)
	ON_BN_CLICKED(IDC_BUTTON_PREV_BLANK, OnBnClickedButtonPrevBlank)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_BLANK, OnBnClickedButtonNextBlank)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_SETCURRENTBMP, OnBnClickedButtonSetcurrentbmp)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
END_MESSAGE_MAP()


// CDlgRPage1 ���b�Z�[�W �n���h��

/*--------------------------------------------------------------
	��`�ԍ��ύX������
----------------------------------------------------------------*/
void CDlgRPage1::OnEnChangeEdit1()
{
	// TODO :  �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����Ă��������B
	DWORD prv = m_current_index;
	UpdateData(TRUE);

	if(m_current_index<1 || m_current_index>1023){
		m_current_index = prv;
		theApp.SetStatus("��`�ԍ��ɕs���Ȓl�����͂��ꂽ");
		return;
	}
	OnChangeIndex();
}

/*--------------------------------------------------------------
	��`�ԍ����R�A�X�s������
----------------------------------------------------------------*/
void CDlgRPage1::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
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
	��`�ԍ��ړ��{�^������
----------------------------------------------------------------*/
void CDlgRPage1::OnBnClickedButtonPrev10()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if(m_current_index<11)m_current_index=1;
	else m_current_index-=10;

	OnChangeIndex();
}

void CDlgRPage1::OnBnClickedButtonNext10()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_current_index+=10;
	if(m_current_index>1023)m_current_index=1023;

	OnChangeIndex();
}

void CDlgRPage1::OnBnClickedButtonPrevFind()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if(m_current_index==1){
		theApp.SetStatus("Find Previous : �s���~�܂�");
		return;
	}
	CGCDHandler* pdat = theApp.GetGCD();

	UINT next = m_current_index;
	next--;
	while(pdat->IsEmptyRect(next)){
		if(next==1){
			theApp.SetStatus("Find Previous : ����܂���");
			return;
		}
		next--;
	}
	m_current_index = next;
	OnChangeIndex();
}

void CDlgRPage1::OnBnClickedButtonNextFind()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if(m_current_index==1023){
		theApp.SetStatus("Find Next : �s���~�܂肾��");
		return;
	}
	CGCDHandler* pdat = theApp.GetGCD();

	UINT next = m_current_index;
	next++;
	while(pdat->IsEmptyRect(next)){
		if(next==1023){
			theApp.SetStatus("Find Next : ����܂���");
			return;
		}
		next++;
	}
	m_current_index = next;
	OnChangeIndex();
}

void CDlgRPage1::OnBnClickedButtonPrevBlank()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if(m_current_index==1){
		theApp.SetStatus("Blank Previous : �s���~�܂�");
		return;
	}
	CGCDHandler* pdat = theApp.GetGCD();

	UINT next = m_current_index;
	next--;
	while(!pdat->IsEmptyRect(next)){
		if(next==1){
			theApp.SetStatus("Blank Previous : ����܂���");
			return;
		}
		next--;
	}
	m_current_index = next;
	OnChangeIndex();
}

void CDlgRPage1::OnBnClickedButtonNextBlank()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if(m_current_index==1023){
		theApp.SetStatus("Blank Next : �s���~�܂肾��");
		return;
	}
	CGCDHandler* pdat = theApp.GetGCD();

	UINT next = m_current_index;
	next++;
	while(!pdat->IsEmptyRect(next)){
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
	[����]	�{�^������
----------------------------------------------------------------*/
void CDlgRPage1::OnBnClickedButtonApply()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	GCD_RECT *r = theApp.GetCurrentRect();
	if(!r){
		theApp.SetStatus("��`���菈�����s�H�H�H");
		return;
	}

	//���݂̃r�b�g�}�b�v�ԍ��ɐݒ�
	UpdateData(TRUE);
	r->bmpno = m_selected_bmp;

	//��`�ݒ�
	r->r = g_childwnd.GetCurrentRect();

	CString str;
	str.Format("��`(%d)���� : (%d,%d)-(%d,%d) w:%d / h:%d",
		m_current_index,
		r->r.left, r->r.top, r->r.right, r->r.bottom,
		r->r.right-r->r.left, r->r.bottom-r->r.top );
	theApp.SetStatus(str);

	theApp.InvalidateAll();
}

/*--------------------------------------------------------------
	[����]	�{�^������
----------------------------------------------------------------*/
void CDlgRPage1::OnBnClickedButton2()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if(MessageBox("�����́H","��`����",MB_YESNO)!=IDYES)return;

	GCD_RECT *r = theApp.GetCurrentRect();
	if(!r){
		theApp.SetStatus("��`�����������s�H�H�H");
		return;
	}
	CGCDHandler::GCDSetEmptyRect(r);

	CString str;
	str.Format("��`(%d)����",m_current_index);
	theApp.SetStatus(str);

	theApp.InvalidateAll();
}

/*--------------------------------------------------------------
	[���݂̋�`����`����Ă���Ƃ���Ɉړ�]	�{�^������
----------------------------------------------------------------*/
void CDlgRPage1::OnBnClickedButtonSetcurrentbmp()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	GCD_RECT *r = theApp.GetCurrentRect();
	if(!r){
		theApp.SetStatus("���݂̋�`����`����Ă���Ƃ���Ɉړ��F��`�f�[�^�擾�Ɏ��s�H�H");
		return;
	}
	if(CGCDHandler::GCDIsEmptyRect(*r)){
		theApp.SetStatus("���݂̋�`����`����Ă���Ƃ���Ɉړ��F��`�f�[�^����ł�");
		return;
	}

	//�r�b�g�}�b�v�I���R���{�{�b�N�X�������̂ōL����
	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_COMBO1);
	if(!combo){
		theApp.SetStatus("CDlgRPage1::OnInitDialog combo�擾���s�H");
		return;
	}
	combo->SetCurSel( r->bmpno );
	g_childwnd.ChangeBitmap( r->bmpno );
	m_selected_bmp = r->bmpno;
}


/*--------------------------------------------------------------
	�r�b�g�}�b�v�I���R���{�{�b�N�X
----------------------------------------------------------------*/
void CDlgRPage1::OnCbnSelchangeCombo1()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	UpdateData(TRUE);
	g_childwnd.ChangeBitmap(m_selected_bmp);
}

/*--------------------------------------------------------------
	�R���{�{�b�N�X�̍��ڍX�V
----------------------------------------------------------------*/
void CDlgRPage1::ResetBMPComboBox()
{
	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_COMBO1);
	if(!combo){
		OutputDebugString("CDlgRPage1::ResetBMPComboBox �擾���s�H�H\n");
		throw;
	}

	combo->ResetContent();
	for(UINT i=0;i<GCDMAX_IMAGES;i++){
		combo->InsertString(i, theApp.GetBMPFilename(i) );
	}
	combo->SetCurSel(m_selected_bmp);
	UpdateData(TRUE);
	g_childwnd.ChangeBitmap(m_selected_bmp);
}


/*--------------------------------------------------------------
	�\�����̍X�V
----------------------------------------------------------------*/
void CDlgRPage1::UpdateItems()
{
	ResetBMPComboBox();
	UpdateInfo();
}


/*--------------------------------------------------------------
	������������
----------------------------------------------------------------*/
BOOL CDlgRPage1::OnInitDialog()
{
	CMyDialogTab::OnInitDialog();

	// TODO :  �����ɏ�������ǉ����Ă�������

	//�l�̏����l
	m_current_index = 1;
	UpdateData(FALSE);

	//�r�b�g�}�b�v�I���R���{�{�b�N�X�������̂ōL����
	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_COMBO1);
	if(!combo){
		OutputDebugString("CDlgRPage1::OnInitDialog �擾���s�H�H\n");
		throw;
	}
	RECT r;
	combo->GetClientRect(&r);
	r.bottom += (r.bottom - r.top)*6;
	r.left = 20;
	r.top = 300;
	r.bottom += r.top;
	combo->MoveWindow(&r,FALSE);

	//�X�s���R���g���[���̐ݒ�
	CSpinButtonCtrl* spin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN1);
	if(!spin){
		OutputDebugString("CDlgRPage1::OnInitDialog spin�擾���s�H�H\n");
		throw;
	}
	spin->SetRange32(1,1023);
	spin->SetPos( m_current_index );

	//���W�I�{�^��1(��`�w�胂�[�h)���`�F�b�N��Ԃɂ���
	CButton *wndr1 = (CButton*)GetDlgItem(IDC_RADIO1);
	wndr1->SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*--------------------------------------------------------------
	��`�ԍ��ύX���̏���
----------------------------------------------------------------*/
void CDlgRPage1::OnChangeIndex()
{
	UpdateData(FALSE);
	g_childwnd.Dirty();

	CSpinButtonCtrl* spin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN1);
	if(!spin){
		OutputDebugString("CDlgRPage1::OnInitDialog spin�擾���s�H�H\n");
		throw;
	}
	spin->SetPos(m_current_index);

	UpdateInfo();
}

/*--------------------------------------------------------------
	��`���\���X�V
----------------------------------------------------------------*/
void CDlgRPage1::UpdateInfo()
{
	m_str_info1 = "";
	m_str_info2 = "";

	GCD_RECT *r = theApp.GetCurrentRect();
	if(r==NULL){
		m_str_info1 = "error";
		UpdateData(FALSE);
		return;
	}
	else if(CGCDHandler::GCDIsEmptyRect(*r)){
		m_str_info1 = "- empty -";
		UpdateData(FALSE);
		return;
	}
	else{
		m_str_info1.Format("bmp:%d center(%d,%d)",
			r->bmpno,r->center_x,r->center_y);
		m_str_info2.Format("%d,%d - %d,%d (w:%d / h:%d)",
			r->r.left,r->r.top,r->r.right,r->r.bottom,
			r->r.right - r->r.left,r->r.bottom - r->r.top);
	}
	UpdateData(FALSE);
}


/*--------------------------------------------------------------
	��`�w��/�d�S�w�胉�W�I�{�^��
----------------------------------------------------------------*/
void CDlgRPage1::OnBnClickedRadio1()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B

	//[����]�{�^������
	CButton *btn = (CButton*)GetDlgItem(IDC_BUTTON_APPLY);
	if(btn)btn->EnableWindow(TRUE);

	//�\���E�C���h�E�N���X�ɒʒm
	g_childwnd.ChangeEditModeR(0);
}

void CDlgRPage1::OnBnClickedRadio2()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	
	//[����]�{�^������
	CButton *btn = (CButton*)GetDlgItem(IDC_BUTTON_APPLY);
	if(btn)btn->EnableWindow(FALSE);

	//�\���E�C���h�E�N���X�ɒʒm
	g_childwnd.ChangeEditModeR(1);
}

