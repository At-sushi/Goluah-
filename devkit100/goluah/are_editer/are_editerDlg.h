// are_editerDlg.h : ヘッダー ファイル
//

#pragma once

#include "ChildWnd.h"
#define MAX_TAB_PAGES	(4)


// CAre_editerDlg ダイアログ
class CAre_editerDlg : public CMyDialogTab
{
	DECLARE_DYNAMIC(CAre_editerDlg)

// コンストラクション
public:
	CAre_editerDlg(CWnd* pParent = NULL);	// 標準コンストラクタ
	void UpdateItems();//表示物更新
	void SpecialCtrl(UINT key);

// ダイアログ データ
	enum { IDD = IDD_ARE_EDITER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

// 実装
public:
	void SetStatus(CString str);
	CString GetBMPFilename(UINT i);
	CMyDialogTab* GetDialogTab(int num)	{ return m_tab_pages[num]; }

protected:
	HICON m_hIcon;

	//各タブページに相当するダイアログクラス
	CMyDialogTab *m_tab_pages[MAX_TAB_PAGES];

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDropFiles( HDROP hDropInfo );
	virtual BOOL DestroyWindow();
	CString m_status;
	afx_msg void OnClose();
};
