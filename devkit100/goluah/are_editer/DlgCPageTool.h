#pragma once


// CDlgCPageTool �_�C�A���O

class CDlgCPageTool : public CMyDialogTab
{
	DECLARE_DYNAMIC(CDlgCPageTool)

public:
	CDlgCPageTool(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CDlgCPageTool();
	void UpdateItems(){}//�\�����X�V

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_CTAB_TOOLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRflip0();
	afx_msg void OnBnClickedButtonRflip1();
	afx_msg void OnBnClickedButtonRflip2();
	afx_msg void OnBnClickedButtonRflip3();
	afx_msg void OnBnClickedButtonRflip4();
	afx_msg void OnBnClickedButtonRflip5();
	afx_msg void OnBnClickedButtonRflip6();
	afx_msg void OnBnClickedButtonRflip7();
	afx_msg void OnBnClickedButtonRflip8();
	// ��Z���}���ʒu
	DWORD m_insEmptyCellIndex;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton3();
	BOOL m_bUnder100;
};
