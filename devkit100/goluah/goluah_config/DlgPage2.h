#pragma once
#include "afxwin.h"


// CDlgPage2 ダイアログ

class CDlgPage2 : public CCfgDialogBase
{
	DECLARE_DYNAMIC(CDlgPage2)

public:
	CDlgPage2(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDlgPage2();

	void UpdateSettings();
	void Reset();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_PAGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL ck_debugmode;
	BOOL ck_showinfo;
	BOOL ck_showmovie;
	afx_msg void OnBnClickedButtonKeybordSetteing();
	afx_msg void OnBnClickedButtonPadSetting();
	BOOL limiter_cut;
	afx_msg void OnBnClickedButtonInputassign();
	afx_msg void OnBnClickedCheckPointlimitcut();
};
