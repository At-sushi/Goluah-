#pragma once


// CDlgTPage2 ダイアログ

class CDlgTPage2 : public CMyDialogTab
{
	DECLARE_DYNAMIC(CDlgTPage2)

public:
	CDlgTPage2(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDlgTPage2();

	void UpdateItems(){}//表示物更新

// ダイアログ データ
	enum { IDD = IDD_DIALOG_RTAB2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClicked1001();
};
