// gcd_tool03Dlg.h : �w�b�_�[ �t�@�C��
//

#pragma once


// Cgcd_tool03Dlg �_�C�A���O
class Cgcd_tool03Dlg : public CDialog
{
// �R���X�g���N�V����
public:
	Cgcd_tool03Dlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^

// �_�C�A���O �f�[�^
	enum { IDD = IDD_GCD_TOOL03_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDropFiles( HDROP hDropInfo );
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	CString m_str_status;
	// �`�F�b�N�{�b�N�X1�̒l
	BOOL m_bCheck1;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	UINT m_edit_number1;
	afx_msg void OnBnClickedButton7();
};
