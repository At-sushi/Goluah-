#pragma once


// CDlgOther ダイアログ

class CDlgOther : public CCfgDialogBase
{
	DECLARE_DYNAMIC(CDlgOther)

public:
	CDlgOther(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDlgOther();

	void UpdateSettings();
	void Reset();

	void EnableLogChecks(BOOL en);

// ダイアログ データ
	enum { IDD = IDD_DIALOG_PAGE4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	BOOL ck_log2file;
	BOOL ck_loginfo;
	BOOL ck_logwarning;
	BOOL ck_logdebug;
	int  combo_imgformat;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckUselog();
};
