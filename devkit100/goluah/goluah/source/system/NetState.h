#pragma once
#include "afxwin.h"

// 独自メッセージ
#define WM_UPDATELOG	WM_APP

// CNetState ダイアログ

//!ネット対戦中のチャットとかステート表示ダイアログ
class CNetState : public CDialog
{
	DECLARE_DYNAMIC(CNetState)

public:
	CNetState(CWnd* pParent = NULL, CNetState** pPointerMe = NULL);   // 標準コンストラクタ
	virtual ~CNetState();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_NETSTATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
	// チャット入力欄
	CString m_chat;
	// ログ
	CListBox m_log;
	// メンツ
	CListBox m_member;

	CNetState** m_pMe;
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnEnUpdateEditChat();
	// ログに書き込む
	void WriteLog(const char* pStr);
};
