// DlgKBKeyGet.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "goluah_config.h"
#include "DlgKBKeyGet.h"

CDlgKBKeyGet* CDlgKBKeyGet::m_instance=NULL;

// CDlgKBKeyGet �_�C�A���O

IMPLEMENT_DYNAMIC(CDlgKBKeyGet, CDialog)
CDlgKBKeyGet::CDlgKBKeyGet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgKBKeyGet::IDD, pParent)
{
	m_instance = this;
}

CDlgKBKeyGet::~CDlgKBKeyGet()
{
	m_instance = NULL;
}

void CDlgKBKeyGet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


#define ON_WM_KEYDOWN() \
	{ WM_KEYDOWN, 0, 0, 0, AfxSig_vwww, \
		(AFX_PMSG)(AFX_PMSGW) \
		(static_cast< void (AFX_MSG_CALL CWnd::*)(UINT, UINT, UINT) > (&CDlgKBKeyGet::OnKeyDown)) },

BEGIN_MESSAGE_MAP(CDlgKBKeyGet, CDialog)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CDlgKBKeyGet ���b�Z�[�W �n���h��

void CDlgKBKeyGet::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	EndDialog(0);
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CDlgKBKeyGet::PreTranslateMessage(MSG* pMsg)
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	
	if(pMsg->message == WM_KEYDOWN){
        if(pMsg->wParam != VK_ESCAPE)
		{
			BYTE code = (BYTE)pMsg->wParam;

			if(code==VK_SHIFT)
			{
				if(GetKeyState(VK_LSHIFT)&0x8000)code = VK_LSHIFT;
				if(GetKeyState(VK_RSHIFT)&0x8000)code = VK_RSHIFT;
			}
			if(code==VK_CONTROL)
			{
				if(GetKeyState(VK_LCONTROL)&0x8000)code = VK_LCONTROL;
				if(GetKeyState(VK_RCONTROL)&0x8000)code = VK_RCONTROL;
				
			}
			if(code==VK_MENU)
			{
				if(GetKeyState(VK_LMENU)&0x8000)code = VK_LMENU;
				if(GetKeyState(VK_RMENU)&0x8000)code = VK_RMENU;
				
			}

			EndDialog( code );
        }
    }

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgKBKeyGet::OnCancel()
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	//CDialog::OnCancel();

	EndDialog(0);
}
