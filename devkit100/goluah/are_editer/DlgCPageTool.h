#pragma once


// CDlgCPageTool ダイアログ

class CDlgCPageTool : public CMyDialogTab
{
	DECLARE_DYNAMIC(CDlgCPageTool)

public:
	CDlgCPageTool(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDlgCPageTool();
	void UpdateItems(){}//表示物更新

// ダイアログ データ
	enum { IDD = IDD_DIALOG_CTAB_TOOLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRflip0();
	afx_msg void OnBnClickedButtonRflip1();
	afx_msg void OnBnClickedButtonRflip2();
	afx_msg void OnBnClickedButtonRflip3();
	afx_msg void OnBnClickedButtonRflip4();
	afx_msg void OnBnClickedButtonRflip5();
	afx_msg void OnBnClickedButtonRflip6();
	afx_msg void OnBnClickedButtonRflip7();
	afx_msg void OnBnClickedButtonRflip8();
	// 空セル挿入位置
	DWORD m_insEmptyCellIndex;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton3();
	BOOL m_bUnder100;
};
