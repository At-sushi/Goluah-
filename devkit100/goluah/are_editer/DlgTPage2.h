#pragma once


// CDlgTPage2 �_�C�A���O

class CDlgTPage2 : public CMyDialogTab
{
	DECLARE_DYNAMIC(CDlgTPage2)

public:
	CDlgTPage2(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CDlgTPage2();

	void UpdateItems(){}//�\�����X�V

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_RTAB2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClicked1001();
};
