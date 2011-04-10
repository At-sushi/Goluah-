#pragma once


#define MAX_TAB_PAGES_CELL 5

// CCelleditDlg �_�C�A���O

class CCelleditDlg : public CMyDialogTab
{
	DECLARE_DYNAMIC(CCelleditDlg)

public:
	CCelleditDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CCelleditDlg();

	void UpdateItems();//�\�����X�V
	void OnChangeRectIndex(UINT idx);
	void BlankNext();

protected:
	DWORD m_rectIndex;

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_SUBTAB_CELL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:

	//�e�^�u�y�[�W�ɑ�������_�C�A���O�N���X
	CMyDialogTab *m_tab_pages[MAX_TAB_PAGES_CELL];

	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
};
