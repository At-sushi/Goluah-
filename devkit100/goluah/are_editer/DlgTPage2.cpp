// DlgTPage2.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "are_editer.h"
#include "DlgTPage2.h"


// CDlgTPage2 �_�C�A���O

IMPLEMENT_DYNAMIC(CDlgTPage2, CMyDialogTab)
CDlgTPage2::CDlgTPage2(CWnd* pParent /*=NULL*/)
	: CMyDialogTab(CDlgTPage2::IDD, pParent)
{
}

CDlgTPage2::~CDlgTPage2()
{
}

void CDlgTPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTPage2, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(1001, OnBnClicked1001)
END_MESSAGE_MAP()

// CDlgTPage2 ���b�Z�[�W �n���h��

/*-----------------------------------------------------------
	��̋�`����������
-------------------------------------------------------------*/
void CDlgTPage2::OnBnClickedButton1()
{
	if(IDYES!=MessageBox(
			"��`���̃r�b�g�}�b�v�F���`�F�b�N���A���ׂĂ������F�̏ꍇ���̋�`���N���A���܂��B\n�r�b�g�}�b�v������ɓǂݍ��܂�Ă��Ȃ��ꍇ�A���̃r�b�g�}�b�v���Q�Ƃ����`�͂��ׂď�������܂�\n\n���s�O�Ƀo�b�N�A�b�v���Ƃ��Ă������Ƃ������߂��܂��B\n���s���܂����H",
			"",MB_YESNO))
	{return;}

	theApp.DeleteInvalidRect();
}

/*-----------------------------------------------------------
	��`�ԍ��𐮗�����
-------------------------------------------------------------*/
void CDlgTPage2::OnBnClicked1001()
{
	if(IDYES!=MessageBox(
			"�ʐ�0�̋�`�f�[�^�����C���f�b�N�X���������A�ԍ����߂܂��B\n\n���s�O�Ƀo�b�N�A�b�v���Ƃ��Ă������Ƃ������߂��܂��B\n\n���s���܂����H",
			"",MB_YESNO))
	{return;}

	theApp.DeleteEmptyRect();
}
