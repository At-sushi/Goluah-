#pragma once


// CDlgPage1 ダイアログ

class CDlgPage1 : public CCfgDialogBase
{
	DECLARE_DYNAMIC(CDlgPage1)

public:
	CDlgPage1(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDlgPage1();

	virtual void Reset();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_PAGE1 };

	void UpdateSettings();

	DECLARE_MESSAGE_MAP()
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

public:
	virtual BOOL OnInitDialog();
	BOOL ck_full_screen;
	BOOL ck_halfmode;
	BOOL ck_noalphatest;
//	afx_msg void OnClose();
};
