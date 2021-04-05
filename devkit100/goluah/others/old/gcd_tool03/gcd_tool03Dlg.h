// gcd_tool03Dlg.h : ヘッダー ファイル
//

#pragma once


// Cgcd_tool03Dlg ダイアログ
class Cgcd_tool03Dlg : public CDialog
{
// コンストラクション
public:
	Cgcd_tool03Dlg(CWnd* pParent = NULL);	// 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_GCD_TOOL03_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDropFiles( HDROP hDropInfo );
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	CString m_str_status;
	// チェックボックス1の値
	BOOL m_bCheck1;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	UINT m_edit_number1;
	afx_msg void OnBnClickedButton7();
};
