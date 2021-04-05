#pragma once


// CDlgLogSetting ダイアログ

class CDlgLogSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgLogSetting)

public:
	CDlgLogSetting(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDlgLogSetting();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_LOGSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
