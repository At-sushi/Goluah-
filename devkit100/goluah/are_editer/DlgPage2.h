#pragma once


// CDlgPage2 ダイアログ

class CDlgPage2 : public CMyDialogTab
{
	DECLARE_DYNCREATE(CDlgPage2)

public:
	CDlgPage2(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDlgPage2();
	void UpdateItems();//表示物更新

protected:
	void OnChangeIndex();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_TAB2 };

	GCD_CELL2 m_CopiedCell;
	GCD_HANTEI m_CopiedHantei;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	BOOL m_skip_link;
	BOOL m_sell_link;
	DWORD m_current_index;
	CString m_str_define;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonPrev10();
	afx_msg void OnBnClickedButtonNext10();
	afx_msg void OnBnClickedButtonPrevFind();
	afx_msg void OnBnClickedButtonNextFind();
	afx_msg void OnBnClickedButtonPrevBlank();
	afx_msg void OnBnClickedButtonNextBlank();
	afx_msg void OnBnClickedButtonCopy();
	afx_msg void OnBnClickedButtonPaste();
	afx_msg void OnBnClickedButtonErase();
	afx_msg void OnBnClickedCheckLink();
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnBnClickedCheckNozw();
	afx_msg void OnBnClickedCheckNozt();
	afx_msg void OnBnClickedCheckScaBasepoint();
	afx_msg void OnBnClickedCheckRotBasepoint();
	BOOL m_flag_zw;
	BOOL m_flag_zt;
	BOOL m_flag_scaBP;
	BOOL m_flag_rotBP;
};
