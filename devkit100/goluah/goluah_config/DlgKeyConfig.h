#pragma once
#include "config.h"
#include <vector>

// CDlgKeyConfig ダイアログ

class CDlgKeyConfig : public CDialog
{
	DECLARE_DYNAMIC(CDlgKeyConfig)

public:
	CDlgKeyConfig(CWnd* pParent , KEYCONFIG& cfg);   // 標準コンストラクタ
	virtual ~CDlgKeyConfig();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_KEYBORD };

protected:
	KEYCONFIG m_cfg;
	void UpdateNames();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSetkey11();
	afx_msg void OnBnClickedButtonSetkey12();
	afx_msg void OnBnClickedButtonSetkey13();
	afx_msg void OnBnClickedButtonSetkey14();
	afx_msg void OnBnClickedButtonSetkey15();
	afx_msg void OnBnClickedButtonSetkey16();
	afx_msg void OnBnClickedButtonSetkey17();
	afx_msg void OnBnClickedButtonSetkey18();
	afx_msg void OnBnClickedButtonSetkey21();
	afx_msg void OnBnClickedButtonSetkey22();
	afx_msg void OnBnClickedButtonSetkey23();
	afx_msg void OnBnClickedButtonSetkey24();
	afx_msg void OnBnClickedButtonSetkey26();
	afx_msg void OnBnClickedButtonSetkey27();
	afx_msg void OnBnClickedButtonSetkey28();
	afx_msg void OnBnClickedButtonSetkey29();
	CString sstr_1_1;
	CString sstr_1_2;
	CString sstr_1_3;
	CString sstr_1_4;
	CString sstr_1_5;
	CString sstr_1_6;
	CString sstr_1_7;
	CString sstr_1_8;
	CString sstr_2_1;
	CString sstr_2_2;
	CString sstr_2_3;
	CString sstr_2_4;
	CString sstr_2_5;
	CString sstr_2_6;
	CString sstr_2_7;
	CString sstr_2_8;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnBnClickedOk();
protected:
	virtual void OnCancel();
};



class CKeyCode2Str
{
public:
	CKeyCode2Str();
	~CKeyCode2Str();
	const char* GetStr( BYTE param );

protected:
	void Add(const char *str);
	std::vector<const char*> m_list;
};