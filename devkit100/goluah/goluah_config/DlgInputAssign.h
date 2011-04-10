#pragma once


// CDlgInputAssign ダイアログ

class CDlgInputAssign : public CDialog
{
	DECLARE_DYNAMIC(CDlgInputAssign)

public:
	CDlgInputAssign(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDlgInputAssign();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_INPUTASSIGN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	void SetComboItem(int id,int left,int top);

	DECLARE_MESSAGE_MAP()
public:
	INPUT2PLAYER m_cfg;
	int m_kb1;
	int m_kb2;
	int m_pad1;
	int m_pad2;
	int m_pad3;
	int m_pad4;
	int m_pad5;
	int m_pad6;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboPad3();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnCbnSelchangeComboKb1();
};
