// are_editerDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once

#include "ChildWnd.h"
#define MAX_TAB_PAGES	(4)


// CAre_editerDlg �_�C�A���O
class CAre_editerDlg : public CMyDialogTab
{
	DECLARE_DYNAMIC(CAre_editerDlg)

// �R���X�g���N�V����
public:
	CAre_editerDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^
	void UpdateItems();//�\�����X�V
	void SpecialCtrl(UINT key);

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ARE_EDITER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g

// ����
public:
	void SetStatus(CString str);
	CString GetBMPFilename(UINT i);
	CMyDialogTab* GetDialogTab(int num)	{ return m_tab_pages[num]; }

protected:
	HICON m_hIcon;

	//�e�^�u�y�[�W�ɑ�������_�C�A���O�N���X
	CMyDialogTab *m_tab_pages[MAX_TAB_PAGES];

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDropFiles( HDROP hDropInfo );
	virtual BOOL DestroyWindow();
	CString m_status;
	afx_msg void OnClose();
};
