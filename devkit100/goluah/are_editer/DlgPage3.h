#pragma once


// CDlgPage3 ダイアログ

class CDlgPage3 : public CMyDialogTab
{
	DECLARE_DYNAMIC(CDlgPage3)

public:
	CDlgPage3(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDlgPage3();
	void UpdateItems();//表示物更新

// ダイアログ データ
	enum { IDD = IDD_DIALOG_TAB3 };

protected:
	void EnableButtons(BOOL f);
	void UpdateInformation();

	UINT m_mode;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	CString m_str_info1;
	afx_msg void OnBnClickedButtonRectKas1();
	afx_msg void OnBnClickedButtonRectKas2();
	afx_msg void OnBnClickedButtonRectKas3();
	afx_msg void OnBnClickedButtonRectKur1();
	afx_msg void OnBnClickedButtonRectKur2();
	afx_msg void OnBnClickedButtonRectKur3();
	afx_msg void OnBnClickedButtonRectAtk1();
	afx_msg void OnBnClickedButtonRectAtk2();
	afx_msg void OnBnClickedButtonRectAtk3();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadioNone();
};
