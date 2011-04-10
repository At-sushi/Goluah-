#include "..\goluah_config\dlgother.h"
// DlgOther.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "are_editer.h"
#include "DlgOther.h"
#include "ChildWnd.h"
#include "dx_draw.h"

extern CDirectDraw g_draw;

// CDlgOther �_�C�A���O

IMPLEMENT_DYNAMIC(CDlgOther, CMyDialogTab)
CDlgOther::CDlgOther(CWnd* pParent /*=NULL*/)
	: CMyDialogTab(CDlgOther::IDD, pParent)
{
	CFile file;
	if( file.Open("are_editer.ini",CFile::modeRead) )
	{
		file.Read(&m_color,(UINT)sizeof(DWORD));
		file.Close();
	}
	else m_color = 0x333333;

	g_draw.clearbgcolor = m_color;
}

CDlgOther::~CDlgOther()
{
}

void CDlgOther::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgOther, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, OnNMCustomdrawSlider2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, OnNMCustomdrawSlider3)
END_MESSAGE_MAP()


// CDlgOther ���b�Z�[�W �n���h��


/*-------------------------------------------------------------------
	�_�C�A���O������
---------------------------------------------------------------------*/
BOOL CDlgOther::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO :  �����ɏ�������ǉ����Ă�������
	DWORD red = (m_color&0x00FF0000) >> 16;
	DWORD green = (m_color&0x0000FF00) >> 8;
	DWORD blue = (m_color&0x000000FF) ;

	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	sl->SetRange(0,255);
	sl->SetPos(red);
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER2);
	sl->SetRange(0,255);
	sl->SetPos(green);
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER3);
	sl->SetRange(0,255);
	sl->SetPos(blue);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*-------------------------------------------------------------------
	�w�i�F�w��
---------------------------------------------------------------------*/
void CDlgOther::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	*pResult = 0;

	UpdateBGColor();
}

void CDlgOther::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	*pResult = 0;

	UpdateBGColor();
}

void CDlgOther::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	*pResult = 0;
	
	UpdateBGColor();
}

void CDlgOther::UpdateBGColor()
{
	DWORD r,g,b;

	CSliderCtrl *sl;
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	r = sl->GetPos();
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER2);
	g = sl->GetPos();
	sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER3);
	b = sl->GetPos();

	DWORD col = 0;
	col |= r;
	col <<= 8;
	col |= g;
	col <<= 8;
	col |= b;
	col |= 0xFF000000;

	m_color = col;

	g_draw.clearbgcolor = col;
	g_childwnd.Invalidate();
}

/*-------------------------------------------------------------------
	�w�i�F�ݒ�ۑ�
---------------------------------------------------------------------*/
BOOL CDlgOther::DestroyWindow()
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	CFile file;
	if( file.Open("are_editer.ini",CFile::modeCreate | CFile::modeWrite) )
	{
		file.Write(&m_color,(UINT)sizeof(DWORD));
		file.Close();
	}

	return CMyDialogTab::DestroyWindow();
}

