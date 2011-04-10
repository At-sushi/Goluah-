#pragma once
#include "afxwin.h"

// �Ǝ����b�Z�[�W
#define WM_UPDATELOG	WM_APP

// CNetState �_�C�A���O

//!�l�b�g�ΐ풆�̃`���b�g�Ƃ��X�e�[�g�\���_�C�A���O
class CNetState : public CDialog
{
	DECLARE_DYNAMIC(CNetState)

public:
	CNetState(CWnd* pParent = NULL, CNetState** pPointerMe = NULL);   // �W���R���X�g���N�^
	virtual ~CNetState();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_NETSTATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
	// �`���b�g���͗�
	CString m_chat;
	// ���O
	CListBox m_log;
	// �����c
	CListBox m_member;

	CNetState** m_pMe;
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnEnUpdateEditChat();
	// ���O�ɏ�������
	void WriteLog(const char* pStr);
};
