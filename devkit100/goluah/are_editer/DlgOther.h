#pragma once


// CDlgOther ダイアログ

class CDlgOther : public CMyDialogTab
{
	DECLARE_DYNAMIC(CDlgOther)

public:
	CDlgOther(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDlgOther();
	void UpdateItems(){}
	void UpdateBGColor();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_SUBTAB_OTHER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	DWORD m_color;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
};
