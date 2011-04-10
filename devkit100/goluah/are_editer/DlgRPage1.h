#pragma once


// CDlgRPage1 �_�C�A���O

class CDlgRPage1 : public CMyDialogTab
{
	DECLARE_DYNAMIC(CDlgRPage1)

public:
	CDlgRPage1(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CDlgRPage1();
	void UpdateItems();

	void ResetBMPComboBox();
	UINT GetCurrentRectNo(){return m_current_index;}

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_RTAB1 };

protected:
	void OnChangeIndex();
	void UpdateInfo();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_current_index;
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButtonPrev10();
	afx_msg void OnBnClickedButtonNext10();
	afx_msg void OnBnClickedButtonPrevFind();
	afx_msg void OnBnClickedButtonNextFind();
	afx_msg void OnBnClickedButtonPrevBlank();
	afx_msg void OnBnClickedButtonNextBlank();
	int m_radio_editmode;
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButton2();
	CString m_str_info1;
	CString m_str_info2;
	afx_msg void OnBnClickedButtonSetcurrentbmp();
	// �I�����ꂽ�r�b�g�}�b�v
	int m_selected_bmp;
	afx_msg void OnCbnSelchangeCombo1();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
};
