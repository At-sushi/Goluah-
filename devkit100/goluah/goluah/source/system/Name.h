#pragma once


// CName ダイアログ

//!ホスト設定・名前入力ダイアログ
class CName : public CDialog
{
	DECLARE_DYNAMIC(CName)

public:
	CName(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CName();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	// 名前
	CString m_name;
	// ポート
	CString m_port;
};
