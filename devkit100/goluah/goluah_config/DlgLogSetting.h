#pragma once


// CDlgLogSetting �_�C�A���O

class CDlgLogSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgLogSetting)

public:
	CDlgLogSetting(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CDlgLogSetting();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_LOGSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
};
