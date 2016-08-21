#pragma once


#define MAX_TAB_PAGES_CELL 5

// CCelleditDlg ダイアログ

class CCelleditDlg : public CMyDialogTab
{
	DECLARE_DYNAMIC(CCelleditDlg)

public:
	CCelleditDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CCelleditDlg();

	void UpdateItems();//表示物更新
	void OnChangeRectIndex(UINT idx);
	void BlankNext();

protected:
	DWORD m_rectIndex;

// ダイアログ データ
	enum { IDD = IDD_DIALOG_SUBTAB_CELL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:

	//各タブページに相当するダイアログクラス
	CMyDialogTab *m_tab_pages[MAX_TAB_PAGES_CELL];

	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
};
