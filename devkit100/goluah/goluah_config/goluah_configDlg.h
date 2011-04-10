// goluah_configDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once

#define MAX_TAB_PAGES	(4)


// Cgoluah_configDlg �_�C�A���O
class Cgoluah_configDlg : public CDialog
{
// �R���X�g���N�V����
public:
	Cgoluah_configDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^

// �_�C�A���O �f�[�^
	enum { IDD = IDD_GOLUAH_CONFIG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	CCfgDialogBase* m_tab_pages[MAX_TAB_PAGES];

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedOk();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBnClickedButtonRestoreAll();
protected:
	virtual void OnCancel();
};



