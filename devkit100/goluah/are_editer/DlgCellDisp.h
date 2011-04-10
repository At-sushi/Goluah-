#pragma once


// CDlgCellDisp ダイアログ

class CDlgCellDisp : public CMyDialogTab
{
	DECLARE_DYNAMIC(CDlgCellDisp)

public:
	CDlgCellDisp(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDlgCellDisp();
	void UpdateItems(){}

// ダイアログ データ
	enum { IDD = IDD_DIALOG_CTAB_DISP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	
	void SetupSliders();
	void NotifyChild();//表示ウインドウに、スライダーコントロールの値を通知

	int m_ofsx,m_ofsy;
	float m_scax,m_scay;
	int m_rotation;
	
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	int m_mag;			//拡大率
	BOOL m_marker_on;	//マーカーON/OFF
	BOOL x_flip;		//x反転チェックボックス
	BOOL y_flip;		//y反転チェックボックス
	afx_msg void OnCbnSelchangeComboMag();		//拡大率コンボ
	afx_msg void OnBnClickedCheck1();			//マーカー(多分)
	afx_msg void OnBnClickedCheckFlipxtest();	//X反転
	afx_msg void OnBnClickedCheckFlipytest();	//Y反転
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMCustomdrawSliderOfsx(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderOfsy(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderScax(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderScay(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderRot(NMHDR *pNMHDR, LRESULT *pResult);
};
