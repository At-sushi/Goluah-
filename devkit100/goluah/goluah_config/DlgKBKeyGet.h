#pragma once


// CDlgKBKeyGet �_�C�A���O

class CDlgKBKeyGet : public CDialog
{
	DECLARE_DYNAMIC(CDlgKBKeyGet)

public:
	CDlgKBKeyGet(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CDlgKBKeyGet();

	static CDlgKBKeyGet *m_instance;

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_KEYGET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void OnCancel();
};
