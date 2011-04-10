#pragma once


// CNameC ダイアログ

//!名前入力(クライアント接続設定)ダイアログ
class CNameC : public CDialog
{
	DECLARE_DYNAMIC(CNameC)

public:
	CNameC(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CNameC();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_NAMEC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	// 名前
	CString m_name;
	// ポート番号
	CString m_port;
	// ホストのIP
	CString m_hostIP;
	// ホストのポート番号
	CString m_hostPort;
};
