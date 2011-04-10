#pragma once


// CDlgKBKeyGet ダイアログ

class CDlgKBKeyGet : public CDialog
{
	DECLARE_DYNAMIC(CDlgKBKeyGet)

public:
	CDlgKBKeyGet(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDlgKBKeyGet();

	static CDlgKBKeyGet *m_instance;

// ダイアログ データ
	enum { IDD = IDD_DIALOG_KEYGET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void OnCancel();
};
