#pragma once


// CRecteditDlg �_�C�A���O

#define MAX_TAB_PAGES_RECT 2

class CRecteditDlg : public CMyDialogTab
{
	DECLARE_DYNAMIC(CRecteditDlg)

public:
	CRecteditDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CRecteditDlg();
	void UpdateItems();//�\�����X�V
	void BlankNext();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_SUBTAB_RECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	//�e�^�u�y�[�W�ɑ�������_�C�A���O�N���X
	CMyDialogTab *m_tab_pages[MAX_TAB_PAGES_RECT];

	afx_msg void OnTcnSelchangeTabRect(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
};
