#pragma once


// CNameC �_�C�A���O

//!���O����(�N���C�A���g�ڑ��ݒ�)�_�C�A���O
class CNameC : public CDialog
{
	DECLARE_DYNAMIC(CNameC)

public:
	CNameC(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CNameC();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_NAMEC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

public:
	// ���O
	CString m_name;
	// �|�[�g�ԍ�
	CString m_port;
	// �z�X�g��IP
	CString m_hostIP;
	// �z�X�g�̃|�[�g�ԍ�
	CString m_hostPort;
};
