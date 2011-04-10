// DlgPage4.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "are_editer.h"
#include "DlgPage4.h"

CDlgPage4* CDlgPage4::m_pinstance=NULL;

// CDlgPage4 �_�C�A���O

IMPLEMENT_DYNAMIC(CDlgPage4, CMyDialogTab)
CDlgPage4::CDlgPage4(CWnd* pParent /*=NULL*/)
	: CMyDialogTab(CDlgPage4::IDD, pParent)
	, m_rectIndex(0)
	, m_cellrectNO(0)
	, m_flipX(FALSE)
	, m_flipY(FALSE)
	, m_alpha_ratio(0)
	, m_red_ratio(0)
	, m_green_ratio(0)
	, m_blue_ratio(0)
	, m_scale_x(0)
	, m_scale_y(0)
	, m_rotation(0)
	, m_blendadd(FALSE)
{
	m_pinstance = this;
}

CDlgPage4::~CDlgPage4()
{
}

void CDlgPage4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_cellrectNO);
	DDX_Check(pDX, IDC_CHECK_FLIPX, m_flipX);
	DDX_Check(pDX, IDC_CHECK_FLIPY, m_flipY);
	DDX_Text(pDX, IDC_EDIT_COLOR_A, m_alpha_ratio);
	DDX_Text(pDX, IDC_EDIT_COLOR_R, m_red_ratio);
	DDX_Text(pDX, IDC_EDIT_COLOR_G, m_green_ratio);
	DDX_Text(pDX, IDC_EDIT_COLOR_B, m_blue_ratio);
	DDX_Text(pDX, IDC_EDIT_SCAX, m_scale_x);
	DDX_Text(pDX, IDC_EDIT_SCAY, m_scale_y);
	DDX_Text(pDX, IDC_EDIT_ROT, m_rotation);
	DDX_Check(pDX, IDC_CHECK_BLENDADD, m_blendadd);
}


BEGIN_MESSAGE_MAP(CDlgPage4, CDialog)
	ON_BN_CLICKED(IDC_CHECK_FLIPX, OnBnClickedCheckFlipx)
	ON_BN_CLICKED(IDC_CHECK_FLIPY, OnBnClickedCheckFlipy)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_BN_CLICKED(IDC_BUTTON_PREV10, OnBnClickedButtonPrev10)
	ON_BN_CLICKED(IDC_BUTTON_NEXT10, OnBnClickedButtonNext10)
	ON_BN_CLICKED(IDC_BUTTON_PREV_FIND, OnBnClickedButtonPrevFind)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_FIND, OnBnClickedButtonNextFind)
	ON_BN_CLICKED(IDC_BUTTON_ERASE, OnBnClickedButtonErase)
	ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO4, OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO8, OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO9, OnBnClickedRadio9)
	ON_BN_CLICKED(IDC_RADIO10, OnBnClickedRadio10)
	ON_BN_CLICKED(IDC_CHECK_BLENDADD, OnBnClickedCheckBlendadd)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_COLOR_A, OnNMCustomdrawSliderColorA)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_COLOR_R, OnNMCustomdrawSliderColorR)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_COLOR_G, OnNMCustomdrawSliderColorG)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_COLOR_B, OnNMCustomdrawSliderColorB)
	ON_EN_CHANGE(IDC_EDIT_COLOR_A, OnEnChangeEditColorA)
	ON_EN_CHANGE(IDC_EDIT_COLOR_R, OnEnChangeEditColorR)
	ON_EN_CHANGE(IDC_EDIT_COLOR_G, OnEnChangeEditColorG)
	ON_EN_CHANGE(IDC_EDIT_COLOR_B, OnEnChangeEditColorB)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SCAX, OnNMCustomdrawSliderScax)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SCAY, OnNMCustomdrawSliderScay)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ROT, OnNMCustomdrawSliderRot)
	ON_EN_CHANGE(IDC_EDIT_SCAX, OnEnChangeEditScax)
	ON_EN_CHANGE(IDC_EDIT_SCAY, OnEnChangeEditScay)
	ON_EN_CHANGE(IDC_EDIT_ROT, OnEnChangeEditRot)
END_MESSAGE_MAP()


// CDlgPage4 ���b�Z�[�W �n���h��

/*-----------------------------------------------------------------------
	�_�C�A���O����������
-------------------------------------------------------------------------*/
BOOL CDlgPage4::OnInitDialog()
{
	CMyDialogTab::OnInitDialog();

	// TODO :  �����ɏ�������ǉ����Ă�������

	//��`�C���f�b�N�X�R���{�{�b�N�X�ɍ��ړ˂�����
/*	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_COMBO_RECT);
	if(!combo){
		OutputDebugString("CDlgPage4::OnInitDialog �擾���s�H�H\n");
		throw;
	}
	RECT r = {65,24,65+100,24+200};
	combo->MoveWindow(&r,FALSE);
	combo->InsertString(0, "��`0" );
	combo->InsertString(1, "��`1" );
	combo->InsertString(2, "��`2" );
	combo->InsertString(3, "��`3" );
	combo->InsertString(4, "��`4" );
	combo->InsertString(5, "��`5" );
	combo->InsertString(6, "��`6" );
	combo->InsertString(7, "��`7" );
	combo->SetCurSel(0);*/

	//�X�s���R���g���[���ݒ�
	CSpinButtonCtrl* spin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN1);
	if(!spin){
		OutputDebugString("CDlgRPage1::OnInitDialog spin�擾���s�H�H\n");
		throw;
	}
	spin->SetRange32(0,1023);
	spin->SetPos(0);

	//�X���C�_�[�ݒ�
	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_A);
	sl->SetRange(0,1000);
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_R);
	sl->SetRange(0,1000);
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_G);
	sl->SetRange(0,1000);
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_B);
	sl->SetRange(0,1000);
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SCAX);
	sl->SetRange(0,500);
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SCAY);
	sl->SetRange(0,500);
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_ROT);
	sl->SetRange(0,360*2);

	//���W�I�{�^���ݒ�
	CButton* item_radio = (CButton*)GetDlgItem(IDC_RADIO1);
	if(item_radio)item_radio->SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*------------------------------------------------------------------------
	��`�C���f�b�N�X�ύX������
--------------------------------------------------------------------------*
void CDlgPage4::OnCbnSelchangeComboRect()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	CCelleditDlg* pdlg = m_niseParent;

	if(!pdlg){
		theApp.SetStatus("CDlgPage4::OnCbnSelchangeCombo1 �G���[�A�e�_�C�A���O�擾���s");
		return;
	}

	UpdateData(TRUE);
	pdlg->OnChangeRectIndex(m_rectIndex);
}

/*------------------------------------------------------------------------
	�\�����X�V
--------------------------------------------------------------------------*/
void CDlgPage4::UpdateItems()
{
	GCD_CELL2 *pc = theApp.GetCurrentCell();

	m_cellrectNO = pc->cell[ theApp.GetCurrentCellRectNO() ].cdr;
	m_flipX = (pc->cell[ theApp.GetCurrentCellRectNO() ].flag & GCDCELL_REVERSE_X) ? TRUE : FALSE;
	m_flipY = (pc->cell[ theApp.GetCurrentCellRectNO() ].flag & GCDCELL_REVERSE_Y) ? TRUE : FALSE;
	m_blendadd = (pc->cell[ theApp.GetCurrentCellRectNO() ].flag & GCDCELL_BLEND_ADD) ? TRUE : FALSE;

	m_alpha_ratio	= pc->cell[ theApp.GetCurrentCellRectNO() ].alpha_ratio;
	m_red_ratio		= pc->cell[ theApp.GetCurrentCellRectNO() ].red_ratio;
	m_green_ratio	= pc->cell[ theApp.GetCurrentCellRectNO() ].green_ratio;
	m_blue_ratio	= pc->cell[ theApp.GetCurrentCellRectNO() ].blue_ratio;

	CSliderCtrl* sl;
	
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_A);
	sl->SetPos( (int)(m_alpha_ratio*1000.0f) );
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_R);
	sl->SetPos( (int)(m_red_ratio*1000.0f) );
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_G);
	sl->SetPos( (int)(m_green_ratio*1000.0f) );
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_B);
	sl->SetPos( (int)(m_blue_ratio*1000.0f) );

	m_scale_x = pc->cell[ theApp.GetCurrentCellRectNO() ].magx;
	m_scale_y = pc->cell[ theApp.GetCurrentCellRectNO() ].magy;
	m_rotation = pc->cell[ theApp.GetCurrentCellRectNO() ].rot;

	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SCAX);
	sl->SetPos( (int)(m_scale_x*100.0f) );
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SCAY);
	sl->SetPos( (int)(m_scale_y*100.0f) );
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_ROT);
	sl->SetPos( m_rotation + 360 );

	UpdateData(FALSE);
}

/*------------------------------------------------------------------------
	���]�t���O�@�`�F�b�N�{�b�N�X
--------------------------------------------------------------------------*/
void CDlgPage4::OnBnClickedCheckFlipx()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	UpdateData(TRUE);
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	if(!pc)return;

	CString str;

	if(m_flipX){
		pc->cell[ theApp.GetCurrentCellRectNO() ].flag |= GCDCELL_REVERSE_X;
		str.Format("�Z��%d - ��`%d : X���] ON",
			theApp.GetCurrentCellNO(),theApp.GetCurrentCellRectNO());
		theApp.SetStatus(str);
	}
	else{
		pc->cell[ theApp.GetCurrentCellRectNO() ].flag &= ~GCDCELL_REVERSE_X;
		str.Format("�Z��%d - ��`%d : X���] OFF",
			theApp.GetCurrentCellNO(),theApp.GetCurrentCellRectNO());
		theApp.SetStatus(str);
	}
	theApp.InvalidateAll();
}

void CDlgPage4::OnBnClickedCheckFlipy()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	UpdateData(TRUE);
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	if(!pc)return;

	CString str;

	if(m_flipY){
		pc->cell[ theApp.GetCurrentCellRectNO() ].flag |= GCDCELL_REVERSE_Y;
		str.Format("�Z��%d - ��`%d : Y���] ON",
			theApp.GetCurrentCellNO(),theApp.GetCurrentCellRectNO());
		theApp.SetStatus(str);
	}
	else{
		pc->cell[ theApp.GetCurrentCellRectNO() ].flag &= ~GCDCELL_REVERSE_Y;
		str.Format("�Z��%d - ��`%d : Y���] OFF",
			theApp.GetCurrentCellNO(),theApp.GetCurrentCellRectNO());
		theApp.SetStatus(str);
	}
	theApp.InvalidateAll();
}

/*------------------------------------------------------------------------
	���Z�����@�`�F�b�N�{�b�N�X
--------------------------------------------------------------------------*/
void CDlgPage4::OnBnClickedCheckBlendadd()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	UpdateData(TRUE);
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	if(!pc)return;

	CString str;

	if(m_blendadd){
		pc->cell[ theApp.GetCurrentCellRectNO() ].flag |= GCDCELL_BLEND_ADD;
		str.Format("�Z��%d - ��`%d : ���Z����",
			theApp.GetCurrentCellNO(),theApp.GetCurrentCellRectNO());
		theApp.SetStatus(str);
	}
	else{
		pc->cell[ theApp.GetCurrentCellRectNO() ].flag &= ~GCDCELL_BLEND_ADD;
		str.Format("�Z��%d - ��`%d : �ʏ퍇��",
			theApp.GetCurrentCellNO(),theApp.GetCurrentCellRectNO());
		theApp.SetStatus(str);
	}
	theApp.InvalidateAll();
}

/*------------------------------------------------------------------------
	��`�ԍ��ύX������
--------------------------------------------------------------------------*/
void CDlgPage4::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	*pResult = 0;

	GCD_CELL2 *pc = theApp.GetCurrentCell();
	UINT next = pc->cell[ theApp.GetCurrentCellRectNO() ].cdr + pNMUpDown->iDelta;

	if(next<1 || next>1023){
		return;
	}
	pc->cell[ theApp.GetCurrentCellRectNO() ].cdr = next;
	theApp.InvalidateAll();
}

void CDlgPage4::OnBnClickedButtonPrev10()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	UINT next = pc->cell[ theApp.GetCurrentCellRectNO() ].cdr - 10;

	if(next<1 || next>1023){
		return;
	}
	pc->cell[ theApp.GetCurrentCellRectNO() ].cdr = next;
	theApp.InvalidateAll();
}

void CDlgPage4::OnBnClickedButtonNext10()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	UINT next = pc->cell[ theApp.GetCurrentCellRectNO() ].cdr + 10;

	if(next<1 || next>1023){
		return;
	}
	pc->cell[ theApp.GetCurrentCellRectNO() ].cdr = next;
	theApp.InvalidateAll();
}

void CDlgPage4::OnBnClickedButtonPrevFind()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	UINT next = pc->cell[ theApp.GetCurrentCellRectNO() ].cdr - 1;
	if(next==0){
		theApp.SetStatus("�s���~�܂�");
		return;
	}

	CGCDHandler *gcd = theApp.GetGCD();

	while(gcd->IsEmptyRect(next)){
		next--;
		if(next==0){
			theApp.SetStatus("Prev Rect : ����܂���");
			return;
		}
	}
	pc->cell[ theApp.GetCurrentCellRectNO() ].cdr = next;
	theApp.InvalidateAll();
}

void CDlgPage4::OnBnClickedButtonNextFind()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	UINT next = pc->cell[ theApp.GetCurrentCellRectNO() ].cdr + 1;
	if(next==1023){
		theApp.SetStatus("�s���~�܂�");
		return;
	}

	CGCDHandler *gcd = theApp.GetGCD();

	while(gcd->IsEmptyRect(next)){
		next++;
		if(next==1023){
			theApp.SetStatus("Next Rect : ����܂���");
			return;
		}
	}
	pc->cell[ theApp.GetCurrentCellRectNO() ].cdr = next;
	theApp.InvalidateAll();
}

/*------------------------------------------------------------------------
	��`�ԍ���0�i�Ȃ��j�ɐݒ�
--------------------------------------------------------------------------*/
void CDlgPage4::OnBnClickedButtonErase()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].cdr = 0;

	theApp.SetStatus("��`����(0��)�ݒ�");
	theApp.InvalidateAll();
}

/*------------------------------------------------------------------------
	�Z����`�ԍ�EDIT BOX
--------------------------------------------------------------------------*/
void CDlgPage4::OnEnChangeEdit1()
{
	// TODO :  ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A�܂��ACMyDialogTab::OnInitDialog() �֐����I�[�o�[���C�h���āA
	// OR ��Ԃ� ENM_CORRECTTEXT �t���O���}�X�N�ɓ���āA
	// CRichEditCtrl().SetEventMask() ���Ăяo���Ȃ�����A
	// �R���g���[���́A���̒ʒm�𑗐M���܂���B

	// TODO :  �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����Ă��������B
	DWORD prv = m_cellrectNO;
	UpdateData(TRUE);

	GCD_CELL2 *pc = theApp.GetCurrentCell();

	if(m_cellrectNO<1 || m_cellrectNO>1023){
		m_cellrectNO = prv;
		theApp.SetStatus("��`�ԍ��ɕs���Ȓl�����͂��ꂽ");
		return;
	}
	pc->cell[ theApp.GetCurrentCellRectNO() ].cdr = m_cellrectNO;
	theApp.InvalidateAll();
}

/*------------------------------------------------------------------------
	��`�C���f�b�N�X�E���W�I�{�^������
--------------------------------------------------------------------------*/
void CDlgPage4::OnBnClickedRadio1()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_rectIndex = 0;
	m_niseParent->OnChangeRectIndex(0);
}

void CDlgPage4::OnBnClickedRadio2()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_rectIndex = 1;
	m_niseParent->OnChangeRectIndex(1);
}

void CDlgPage4::OnBnClickedRadio3()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_rectIndex = 2;
	m_niseParent->OnChangeRectIndex(2);
}

void CDlgPage4::OnBnClickedRadio4()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_rectIndex = 3;
	m_niseParent->OnChangeRectIndex(3);
}

void CDlgPage4::OnBnClickedRadio5()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_rectIndex = 4;
	m_niseParent->OnChangeRectIndex(4);
}

void CDlgPage4::OnBnClickedRadio8()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_rectIndex = 5;
	m_niseParent->OnChangeRectIndex(5);
}

void CDlgPage4::OnBnClickedRadio9()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_rectIndex = 6;
	m_niseParent->OnChangeRectIndex(6);
}

void CDlgPage4::OnBnClickedRadio10()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_rectIndex = 7;
	m_niseParent->OnChangeRectIndex(7);
}

/*------------------------------------------------------------------------
	�F�����X���C�_�[�R���g���[������
--------------------------------------------------------------------------*/
void CDlgPage4::OnNMCustomdrawSliderColorA(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	*pResult = 0;

	int pos ;
	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_A);
	pos = sl->GetPos();
	m_alpha_ratio = pos/1000.0f;

	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].alpha_ratio = m_alpha_ratio;

	theApp.InvalidateAll();
}

void CDlgPage4::OnNMCustomdrawSliderColorR(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	*pResult = 0;
	
	int pos ;
	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_R);
	pos = sl->GetPos();
	m_red_ratio = pos/1000.0f;

	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].red_ratio = m_red_ratio;

	theApp.InvalidateAll();
}

void CDlgPage4::OnNMCustomdrawSliderColorG(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	*pResult = 0;

	int pos ;
	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_G);
	pos = sl->GetPos();
	m_green_ratio = pos/1000.0f;

	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].green_ratio = m_green_ratio;

	theApp.InvalidateAll();
}

void CDlgPage4::OnNMCustomdrawSliderColorB(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	*pResult = 0;

	int pos ;
	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_B);
	pos = sl->GetPos();
	m_blue_ratio = pos/1000.0f;

	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].blue_ratio = m_blue_ratio;
	
	theApp.InvalidateAll();
}


/*------------------------------------------------------------------------
	�F�����G�f�B�b�g�R���g���[������
--------------------------------------------------------------------------*/
void CDlgPage4::OnEnChangeEditColorA()
{
	UpdateData(TRUE);
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].alpha_ratio = m_alpha_ratio;

	theApp.InvalidateAll();
}

void CDlgPage4::OnEnChangeEditColorR()
{
	UpdateData(TRUE);
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].red_ratio = m_red_ratio;

	theApp.InvalidateAll();
}

void CDlgPage4::OnEnChangeEditColorG()
{
	UpdateData(TRUE);
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].green_ratio = m_green_ratio;

	theApp.InvalidateAll();
}

void CDlgPage4::OnEnChangeEditColorB()
{
	UpdateData(TRUE);

	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].blue_ratio = m_blue_ratio;

	theApp.InvalidateAll();
}

/*------------------------------------------------------------------------
	X/Y �X�P�[��
--------------------------------------------------------------------------*/
void CDlgPage4::OnNMCustomdrawSliderScax(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	*pResult = 0;

	int pos ;
	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SCAX);
	pos = sl->GetPos();
	m_scale_x = pos/100.0f;

	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].magx = m_scale_x;
	
	theApp.InvalidateAll();
}

void CDlgPage4::OnNMCustomdrawSliderScay(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	*pResult = 0;

	int pos ;
	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SCAY);
	pos = sl->GetPos();
	m_scale_y = pos/100.0f;
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].magy = m_scale_y;
	
	theApp.InvalidateAll();
}


void CDlgPage4::OnEnChangeEditScax()
{
	UpdateData(TRUE);
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].magx = m_scale_x;

	theApp.InvalidateAll();
}

void CDlgPage4::OnEnChangeEditScay()
{
	UpdateData(TRUE);
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].magy = m_scale_y;

	theApp.InvalidateAll();
}

/*------------------------------------------------------------------------
	��]
--------------------------------------------------------------------------*/
void CDlgPage4::OnNMCustomdrawSliderRot(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	*pResult = 0;

	int pos ;
	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_ROT);
	pos = sl->GetPos();

	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].rot = pos - 360;
	
	theApp.InvalidateAll();
}

void CDlgPage4::OnEnChangeEditRot()
{
	UpdateData(TRUE);
	
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	pc->cell[ theApp.GetCurrentCellRectNO() ].rot = m_rotation;

	theApp.InvalidateAll();
}

