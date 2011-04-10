#pragma once


// CDlgCellDisp �_�C�A���O

class CDlgCellDisp : public CMyDialogTab
{
	DECLARE_DYNAMIC(CDlgCellDisp)

public:
	CDlgCellDisp(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CDlgCellDisp();
	void UpdateItems(){}

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_CTAB_DISP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	
	void SetupSliders();
	void NotifyChild();//�\���E�C���h�E�ɁA�X���C�_�[�R���g���[���̒l��ʒm

	int m_ofsx,m_ofsy;
	float m_scax,m_scay;
	int m_rotation;
	
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	int m_mag;			//�g�嗦
	BOOL m_marker_on;	//�}�[�J�[ON/OFF
	BOOL x_flip;		//x���]�`�F�b�N�{�b�N�X
	BOOL y_flip;		//y���]�`�F�b�N�{�b�N�X
	afx_msg void OnCbnSelchangeComboMag();		//�g�嗦�R���{
	afx_msg void OnBnClickedCheck1();			//�}�[�J�[(����)
	afx_msg void OnBnClickedCheckFlipxtest();	//X���]
	afx_msg void OnBnClickedCheckFlipytest();	//Y���]
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMCustomdrawSliderOfsx(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderOfsy(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderScax(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderScay(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderRot(NMHDR *pNMHDR, LRESULT *pResult);
};
