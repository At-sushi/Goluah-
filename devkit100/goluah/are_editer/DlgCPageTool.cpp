// DlgCPageTool.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "are_editer.h"
#include "DlgCPageTool.h"


// CDlgCPageTool �_�C�A���O

IMPLEMENT_DYNAMIC(CDlgCPageTool, CMyDialogTab)
CDlgCPageTool::CDlgCPageTool(CWnd* pParent /*=NULL*/)
	: CMyDialogTab(CDlgCPageTool::IDD, pParent)
	, m_insEmptyCellIndex(0)
	, m_bUnder100(TRUE)
{
}

CDlgCPageTool::~CDlgCPageTool()
{
}

void CDlgCPageTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, 1009, m_insEmptyCellIndex);
	DDX_Check(pDX, 1004, m_bUnder100);
}


BEGIN_MESSAGE_MAP(CDlgCPageTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RFLIP0, OnBnClickedButtonRflip0)
	ON_BN_CLICKED(IDC_BUTTON_RFLIP1, OnBnClickedButtonRflip1)
	ON_BN_CLICKED(IDC_BUTTON_RFLIP2, OnBnClickedButtonRflip2)
	ON_BN_CLICKED(IDC_BUTTON_RFLIP3, OnBnClickedButtonRflip3)
	ON_BN_CLICKED(IDC_BUTTON_RFLIP4, OnBnClickedButtonRflip4)
	ON_BN_CLICKED(IDC_BUTTON_RFLIP5, OnBnClickedButtonRflip5)
	ON_BN_CLICKED(IDC_BUTTON_RFLIP6, OnBnClickedButtonRflip6)
	ON_BN_CLICKED(IDC_BUTTON_RFLIP7, OnBnClickedButtonRflip7)
	ON_BN_CLICKED(IDC_BUTTON_RFLIP8, OnBnClickedButtonRflip8)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CDlgCPageTool ���b�Z�[�W �n���h��

/*------------------------------------------------------------
	��`����ւ��{�^��
--------------------------------------------------------------*/
void CDlgCPageTool::OnBnClickedButtonRflip0()
{
	if(theApp.GetCurrentCellNO()<2){
		theApp.SetStatus("��`����ւ��F�O�Z�����͈͊O�ł��B�ł��܂���B");
		return;
	}
	CGCDHandler *gcd = theApp.GetGCD();

	GCD_CELL2 *pc0 = gcd->GetCell(theApp.GetCurrentCellNO()-1);
	GCD_CELL2 *pc1 = theApp.GetCurrentCell();

	GCD_CELL c = pc1->cell[0];
	pc1->cell[0] = pc0->cell[7];
	pc0->cell[7] = c;

	theApp.SetStatus("��`����ւ��F�O7��0");
	theApp.InvalidateAll();
}

void CDlgCPageTool::OnBnClickedButtonRflip1()
{
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	GCD_CELL c = pc->cell[0];
	pc->cell[0] = pc->cell[1];
	pc->cell[1] = c;

	theApp.SetStatus("��`����ւ��F0��1");
	theApp.InvalidateAll();
}

void CDlgCPageTool::OnBnClickedButtonRflip2()
{
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	GCD_CELL c = pc->cell[1];
	pc->cell[1] = pc->cell[2];
	pc->cell[2] = c;

	theApp.SetStatus("��`����ւ��F1��2");
	theApp.InvalidateAll();
}

void CDlgCPageTool::OnBnClickedButtonRflip3()
{
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	GCD_CELL c = pc->cell[2];
	pc->cell[2] = pc->cell[3];
	pc->cell[3] = c;

	theApp.SetStatus("��`����ւ��F2��3");
	theApp.InvalidateAll();
}

void CDlgCPageTool::OnBnClickedButtonRflip4()
{
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	GCD_CELL c = pc->cell[3];
	pc->cell[3] = pc->cell[4];
	pc->cell[4] = c;

	theApp.SetStatus("��`����ւ��F3��4");
	theApp.InvalidateAll();
}

void CDlgCPageTool::OnBnClickedButtonRflip5()
{
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	GCD_CELL c = pc->cell[4];
	pc->cell[4] = pc->cell[5];
	pc->cell[5] = c;

	theApp.SetStatus("��`����ւ��F4��5");
	theApp.InvalidateAll();
}

void CDlgCPageTool::OnBnClickedButtonRflip6()
{
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	GCD_CELL c = pc->cell[5];
	pc->cell[5] = pc->cell[6];
	pc->cell[6] = c;

	theApp.SetStatus("��`����ւ��F5��6");
	theApp.InvalidateAll();
}

void CDlgCPageTool::OnBnClickedButtonRflip7()
{
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	GCD_CELL c = pc->cell[6];
	pc->cell[6] = pc->cell[7];
	pc->cell[7] = c;

	theApp.SetStatus("��`����ւ��F6��7");
	theApp.InvalidateAll();
}

void CDlgCPageTool::OnBnClickedButtonRflip8()
{
	if(theApp.GetCurrentCellNO()>1022){
		theApp.SetStatus("��`����ւ��F���Z�����͈͊O�ł��B�ł��܂���B");
		return;
	}
	CGCDHandler *gcd = theApp.GetGCD();

	GCD_CELL2 *pc0 = gcd->GetCell(theApp.GetCurrentCellNO()+1);
	GCD_CELL2 *pc1 = theApp.GetCurrentCell();

	GCD_CELL c = pc1->cell[7];
	pc1->cell[7] = pc0->cell[0];
	pc0->cell[0] = c;

	theApp.SetStatus("��`����ւ��F�O7��0");
	theApp.InvalidateAll();
}


/*------------------------------------------------------------
	��Z���}���{�^��
--------------------------------------------------------------*/
void CDlgCPageTool::OnBnClickedButton6()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	UpdateData(TRUE);

	CString str;

	if( m_insEmptyCellIndex<1 || 1022<m_insEmptyCellIndex )
	{
		str.Format("�w�肳�ꂽ�C���f�b�N�X���͈͊O�ł�(%d)",m_insEmptyCellIndex);
		MessageBox(str,"��Z���}������");
		return;
	}

	str.Format("%d�ɋ�Z����}�����܂�",m_insEmptyCellIndex);
	if( IDYES==MessageBox(str,"��Z���}������",MB_YESNO) ){
		theApp.InsertEmptyCell(m_insEmptyCellIndex);
		return;
	}

	theApp.SetStatus("��Z���}�������̓L�����Z������܂���");
}

/*------------------------------------------------------------
	��̋�`�����Q�Ƃ��Ă��Ȃ�CELL���폜
--------------------------------------------------------------*/
void CDlgCPageTool::OnBnClickedButton5()
{
	CString war_str;
	war_str += "��̋�`�����Q�Ƃ��Ă��Ȃ�CELL���폜���܂��B\n";
	war_str += "���̑���́A�I�t�Z�b�g��g�嗦,�����`���̐��l�p�����[�^�݂̂�\n";
	war_str += "���݂��邽�߂ɋ�łȂ��Ƃ݂Ȃ���Ă���s��CELL�̃p�����[�^���N���A���A\n";
	war_str += "�u��ł���v�ƔF�������悤�ɂ��܂��B\n";
	war_str += "\n";
	war_str += "���s�O�Ƀo�b�N�A�b�v����邱�Ƃ������߂��܂��B\n";
	war_str += "���s���܂����H";

	if(IDYES!=MessageBox(war_str,"",MB_YESNO))return;

	theApp.DeleteInvalidCell();
}

/*------------------------------------------------------------
	��̋�`�����Q�Ƃ��Ă��Ȃ�CELL���폜
--------------------------------------------------------------*/
void CDlgCPageTool::OnBnClickedButton3()
{
	UpdateData(TRUE);

	CString war_str;
	war_str += "��ł����`���폜���A�Ԃ��l�߂Ĕԍ��𐮗����܂��B\n";
	war_str += "�L�����N�^�[�̏ꍇ�A�ʏ� 0�`100��CELL��ΏۂƂ��Ȃ��悤�ɂ��Ă��������B";
	war_str += "\n";
	war_str += "���s�O�Ƀo�b�N�A�b�v����邱�Ƃ������߂��܂��B\n";
	war_str += "���s���܂����H";

	if(IDYES!=MessageBox(war_str,"",MB_YESNO))return;

	theApp.DeleteEmptyCell(m_bUnder100);
}
