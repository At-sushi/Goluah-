#pragma once


// CDlgPadConfig �_�C�A���O

class CDlgPadConfig : public CDialog
{
	DECLARE_DYNAMIC(CDlgPadConfig)

public:
	CDlgPadConfig(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CDlgPadConfig();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_PAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	PADCONFIG m_cfg;
	int m_crnt_pad;

	int m_padindex;
	int m_buttonA;
	int m_buttonB;
	int m_buttonC;
	int m_buttonD;
	afx_msg void OnCbnSelchangeComboPadindex();
protected:
	virtual void OnCancel();
};
