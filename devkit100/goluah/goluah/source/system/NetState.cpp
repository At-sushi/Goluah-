// source\system\NetState.cpp : 実装ファイル
//

#include "stdafx.h"
#include "global.h"
#include "goluah.h"
#include "NetState.h"
#include ".\netstate.h"


// CNetState ダイアログ

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


// CNetState メッセージ ハンドラ

void CNetState::PostNcDestroy()
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	CDialog::PostNcDestroy();

	if (m_pMe)
		*m_pMe = NULL;
	delete this;
}

BOOL CNetState::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if (message == WM_UPDATELOG)
		UpdateData(FALSE);

	return CDialog::OnWndMsg(message, wParam, lParam, pResult);
}

void CNetState::OnEnUpdateEditChat()
{
	// TODO :  これが RICHEDIT コントロールの場合、まず、CDialog::OnInitDialog() 関数をオーバーライドして、
	// OR 状態の ENM_CORRECTTEXT フラグをマスクに入れて、
	// OR 状態の ENM_CORRECTTEXT フラグを IParam マスクのに入れて、
	// OR 状態の ENM_UPDATE フラグを lParam マスクに入れて、

	// TODO :  ここにコントロール通知ハンドラ コードを追加してください。
	UpdateData(TRUE);

	if (m_chat.Find('\n') >= 0)
	{
		m_chat.Insert(0, (TCHAR)GNETMSG_CHAT);	// 頭にメッセージ識別子を付ける
		m_chat.TrimRight('\n');			// 改行を外す
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

// ログに書き込む
void CNetState::WriteLog(const TCHAR* pStr)
{
	m_log.InsertString(0, pStr);
	PostMessage(WM_UPDATELOG);
}
