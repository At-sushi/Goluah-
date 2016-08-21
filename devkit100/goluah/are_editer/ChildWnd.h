
/*===================================================================

	コドモウインドウ
	DirectXでの描画に使用

=====================================================================*/
#pragma once

class CChildWnd : public CWnd
{
public:
	CChildWnd();

	void ChangeEditMode(BOOL em);
	void ChangeEditModeR(UINT mode);
	void ChangeEditModeC(UINT mode);
	void Dirty();
	void ChangeBitmap(UINT n);
	RECT GetCurrentRect();
	BOOL GetEditMode(){return m_EditMode;}

	void SetCMag(UINT n)		{mc_mag=n;		Invalidate();}
	void SetCFlipX(BOOL f)		{mc_mirrorX=f;	Invalidate();}
	void SetCFlipY(BOOL f)		{mc_mirrorY=f;	Invalidate();}
	void MarkerON(BOOL f)		{mc_MarkerON=f;	Invalidate();}
	void SetCDisplaySettings(int offset_x,int offset_y,
							float scale_x,float scale_y,int rotation)
							{
								mc_offset_x=offset_x;	mc_offset_y=offset_y;
								mc_scale_x=scale_x;		mc_scale_y=scale_y;
								mc_rotation=rotation;
								Invalidate();
							}

protected:
	BOOL m_EditMode;//FALSE:矩形編集 / TRUE:セル編集

	void OnPaint_RectMode(CPaintDC& dc);
	void OnPaint_CellMode(CPaintDC& dc);
	void ResetScrollInfo();

	//■矩形編集モード時パラメータ
	UINT mr_current_bmp;
	UINT mr_EditMode;

	//スクロール関係
	SCROLLINFO si_y,si_x;
	UINT m_prev_scroll_width;
	UINT m_prev_scroll_height;

	//マウス関連
	BOOL mousedown;
	BOOL drawprv;
	int m_sx,m_gx,m_sy,m_gy,mu_xpos,mu_ypos;

	//■セル編集モード時パラメータ
	UINT	mc_mag;
	UINT	mc_EditMode;
	BOOL	mc_mirrorX;
	BOOL	mc_mirrorY;
	CPoint	mc_MarkerPoint;
	BOOL	mc_MarkerON;
	CPoint	mc_prevPoint;
	BOOL	mc_MouseCtrl;
	int		mc_offset_x;
	int		mc_offset_y;
	float	mc_scale_x;
	float	mc_scale_y;
	int		mc_rotation;

	//■GDI描画関数
	void DrawRECT(int sx,int gx,int sy,int gy,DWORD type);
	void DrawRECT2(HDC  hdc ,int sx,int gx,int sy,int gy,DWORD type);
	void DrawLINE(int sx,int gx,int sy,int gy,DWORD type);
	void DrawPos(HDC hdc,int x,int y,BOOL not);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnClose();
};


extern CChildWnd g_childwnd;