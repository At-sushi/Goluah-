#pragma once


// CRecteditDlg ダイアログ

#define MAX_TAB_PAGES_RECT 2

class CRecteditDlg : public CMyDialogTab
{
	DECLARE_DYNAMIC(CRecteditDlg)

public:
	CRecteditDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CRecteditDlg();
	void UpdateItems();//表示物更新
	void BlankNext();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_SUBTAB_RECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	//各タブページに相当するダイアログクラス
	CMyDialogTab *m_tab_pages[MAX_TAB_PAGES_RECT];

	afx_msg void OnTcnSelchangeTabRect(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
};
