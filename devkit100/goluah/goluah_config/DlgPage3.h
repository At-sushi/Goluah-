#pragma once
#include "afxwin.h"


// CDlgPage3 ダイアログ

class CDlgPage3 : public CCfgDialogBase
{
	DECLARE_DYNAMIC(CDlgPage3)

public:
	CDlgPage3(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDlgPage3();

	void UpdateSettings();
	void Reset();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_PAGE3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL use_dinput;
	BOOL use_dsound;
	BOOL use_dshow;
	BOOL ignore_exceptions;
	int m_combo_devtype;
};
