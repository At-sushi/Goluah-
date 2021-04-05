#pragma once

#include "CelleditDlg.h"
#include "afxwin.h"

// CDlgPage4 ダイアログ

class CDlgPage4 : public CMyDialogTab
{
	DECLARE_DYNAMIC(CDlgPage4)

public:
	static CDlgPage4 *m_pinstance;
	
	CDlgPage4(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDlgPage4();
	void UpdateItems();//表示物更新

// ダイアログ データ
	enum { IDD = IDD_DIALOG_TAB4 };

	void SetNParent(CCelleditDlg* p){m_niseParent=p;}
	CCelleditDlg *m_niseParent;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_rectIndex;
	DWORD m_cellrectNO;
	BOOL m_flipX;
	BOOL m_flipY;
	afx_msg void OnBnClickedCheckFlipx();
	afx_msg void OnBnClickedCheckFlipy();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonPrev10();
	afx_msg void OnBnClickedButtonNext10();
	afx_msg void OnBnClickedButtonPrevFind();
	afx_msg void OnBnClickedButtonNextFind();
	afx_msg void OnBnClickedButtonErase();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio8();
	afx_msg void OnBnClickedRadio9();
	afx_msg void OnBnClickedRadio10();
	float m_alpha_ratio;
	float m_red_ratio;
	float m_green_ratio;
	float m_blue_ratio;
	float m_scale_x;
	float m_scale_y;
	int  m_rotation;
	BOOL m_blendadd;
	afx_msg void OnBnClickedCheckBlendadd();
	afx_msg void OnNMCustomdrawSliderColorA(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderColorR(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderColorG(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderColorB(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditColorA();
	afx_msg void OnEnChangeEditColorR();
	afx_msg void OnEnChangeEditColorG();
	afx_msg void OnEnChangeEditColorB();
	afx_msg void OnNMCustomdrawSliderScax(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderScay(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderRot(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditScax();
	afx_msg void OnEnChangeEditScay();
	afx_msg void OnEnChangeEditRot();
};
