// source\system\NetState.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "global.h"
#include "goluah.h"
#include "NetState.h"
#include ".\netstate.h"


// CNetState �_�C�A���O

IMPLEMENT_DYNAMIC(CNetState, CDialog)
CNetState::CNetState(CWnd* pParent /*=NULL*/, CNetState** pPointerMe /*=NULL*/)
	: CDialog(CNetState::IDD, pParent)
	, m_chat(_T(""))
{
	m_pMe = pPointerMe;

	Create(CNetState::IDD, pParent);
	ShowWindow(SW_SHOW);
}

CNetState::~CNetState()
{
}

void CNetState::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CHAT, m_chat);
	DDX_Control(pDX, IDC_MEMBER, m_member);
	DDX_Control(pDX, IDC_LOG, m_log);
}


BEGIN_MESSAGE_MAP(CNetState, CDialog)
	ON_EN_UPDATE(IDC_EDIT_CHAT, &CNetState::OnEnUpdateEditChat)
END_MESSAGE_MAP()


// CNetState ���b�Z�[�W �n���h��

void CNetState::PostNcDestroy()
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	CDialog::PostNcDestroy();

	if (m_pMe)
		*m_pMe = NULL;
	delete this;
}

BOOL CNetState::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	if (message == WM_UPDATELOG)
		UpdateData(FALSE);

	return CDialog::OnWndMsg(message, wParam, lParam, pResult);
}

void CNetState::OnEnUpdateEditChat()
{
	// TODO :  ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A�܂��ACDialog::OnInitDialog() �֐����I�[�o�[���C�h���āA
	// OR ��Ԃ� ENM_CORRECTTEXT �t���O���}�X�N�ɓ���āA
	// OR ��Ԃ� ENM_CORRECTTEXT �t���O�� IParam �}�X�N�̂ɓ���āA
	// OR ��Ԃ� ENM_UPDATE �t���O�� lParam �}�X�N�ɓ���āA

	// TODO :  �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����Ă��������B
	UpdateData(TRUE);

	if (m_chat.Find('\n') >= 0)
	{
		m_chat.Insert(0, (TCHAR)GNETMSG_CHAT);	// ���Ƀ��b�Z�[�W���ʎq��t����
		m_chat.TrimRight('\n');			// ���s���O��
		if (!m_chat.IsEmpty())
		{
			g_play.SendMsg(DPNID_ALL_PLAYERS_GROUP,
						(BYTE*)m_chat.GetString(), m_chat.GetLength() + 1,
						30000, DPNSEND_GUARANTEED);
			m_chat.Empty();
		}

		UpdateData(FALSE);
	}
}

// ���O�ɏ�������
void CNetState::WriteLog(const TCHAR* pStr)
{
	m_log.InsertString(0, pStr);
	PostMessage(WM_UPDATELOG);
}
