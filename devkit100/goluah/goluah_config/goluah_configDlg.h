// goluah_configDlg.h : ヘッダー ファイル
//

#pragma once

#define MAX_TAB_PAGES	(4)


// Cgoluah_configDlg ダイアログ
class Cgoluah_configDlg : public CDialog
{
// コンストラクション
public:
	Cgoluah_configDlg(CWnd* pParent = NULL);	// 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_GOLUAH_CONFIG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	CCfgDialogBase* m_tab_pages[MAX_TAB_PAGES];

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedOk();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBnClickedButtonRestoreAll();
protected:
	virtual void OnCancel();
};



