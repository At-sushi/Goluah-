#pragma once


#include "define.h"

// CDlgPage1 �_�C�A���O
// �S�̃^�u - �t�@�C������

class CDlgPage1 : public CMyDialogTab
{
	DECLARE_DYNAMIC(CDlgPage1)

public:
	CDlgPage1(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CDlgPage1();
	void UpdateItems(){}//�\�����X�V�A���ɂ�邱�ƂȂ��H

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_TAB1 };

public:
	CString GetBMPFilename(UINT i){return m_str_image[i];}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonLoad();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonRef1();
	afx_msg void OnBnClickedButtonRef2();
	afx_msg void OnBnClickedButtonRef3();
	afx_msg void OnBnClickedButtonRef4();
	afx_msg void OnBnClickedButtonRef5();
	afx_msg void OnBnClickedButtonRef6();
	afx_msg void OnBnClickedButtonRef7();
	afx_msg void OnBnClickedButtonRef8();
	afx_msg void OnBnClickedButtonRef9();
	afx_msg void OnBnClickedButtonRef10();
	afx_msg void OnBnClickedButtonRef11();
	afx_msg void OnBnClickedButtonRef12();
	void OnBnClickedButtonRef(UINT idx);
	
	BOOL m_bDefaltBMP;						//"�f�t�H���g�t�@�C�������g�p����"�`�F�b�N
	CString m_str_image[MAX_IMAGES];		//�e�r�b�g�}�b�v�t�@�C����������

	void SetupDefaultFilenames();
	afx_msg void OnBnClickedButtonLoadHeader();
	afx_msg void OnBnClickedButtonSaveHeader();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnEnChangeEdit1();
};
