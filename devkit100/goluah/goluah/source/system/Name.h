#pragma once


// CName �_�C�A���O

//!�z�X�g�ݒ�E���O���̓_�C�A���O
class CName : public CDialog
{
	DECLARE_DYNAMIC(CName)

public:
	CName(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CName();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	// ���O
	CString m_name;
	// �|�[�g
	CString m_port;
};
