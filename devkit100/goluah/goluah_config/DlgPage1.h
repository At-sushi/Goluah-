#pragma once


// CDlgPage1 �_�C�A���O

class CDlgPage1 : public CCfgDialogBase
{
	DECLARE_DYNAMIC(CDlgPage1)

public:
	CDlgPage1(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CDlgPage1();

	virtual void Reset();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_PAGE1 };

	void UpdateSettings();

	DECLARE_MESSAGE_MAP()
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

public:
	virtual BOOL OnInitDialog();
	BOOL ck_full_screen;
	BOOL ck_halfmode;
	BOOL ck_noalphatest;
//	afx_msg void OnClose();
};
