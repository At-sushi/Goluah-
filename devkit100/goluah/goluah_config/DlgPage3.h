#pragma once
#include "afxwin.h"


// CDlgPage3 �_�C�A���O

class CDlgPage3 : public CCfgDialogBase
{
	DECLARE_DYNAMIC(CDlgPage3)

public:
	CDlgPage3(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CDlgPage3();

	void UpdateSettings();
	void Reset();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_PAGE3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL use_dinput;
	BOOL use_dsound;
	BOOL use_dshow;
	BOOL ignore_exceptions;
	int m_combo_devtype;
};
