
/*===================================================================

	�R�h���E�C���h�E
	DirectX�ł̕`��Ɏg�p

=====================================================================*/

#include "stdafx.h"
#include "dx_draw.h"
#include "ChildWnd.h"
#include "are_editer.h"
#include ".\childwnd.h"

extern CDirectDraw g_draw;
extern int g_DISPLAYWIDTH ;
extern int g_DISPLAYHEIGHT;

CChildWnd g_childwnd;

#define CHILDCENTERX (g_DISPLAYWIDTH/2)
#define CHILDCENTERY (g_DISPLAYHEIGHT*2/3-20)

/*-------------------------------------------------------------------
	�\�z
---------------------------------------------------------------------*/
CChildWnd::CChildWnd() : CWnd()
{
	mousedown=FALSE;
	drawprv=FALSE;
	m_sx=0;
	m_gx=0;
	m_sy=0;
	m_gy=0;
	mu_xpos=0;
	mu_ypos=0;

	mr_current_bmp = 0;

	m_EditMode = FALSE;
	mr_EditMode = 0;

	mc_mag		=3;
	mc_EditMode	=7;//�}�E�X����:none
	mc_mirrorX	=FALSE;
	mc_mirrorY	=FALSE;

	mc_MarkerPoint.x = mc_MarkerPoint.y = 0;
	mc_MarkerON = FALSE;

	mc_MouseCtrl = FALSE;

	m_prev_scroll_width = 0;
	m_prev_scroll_height = 0;

	mc_offset_x = 0;
	mc_offset_y = 0;
	mc_scale_x = 1.0f;
	mc_scale_y = 1.0f;
	mc_rotation = 0;
}

/*-------------------------------------------------------------------
	���b�Z�[�W�}�b�v
---------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CChildWnd, CWnd)
ON_WM_PAINT()
ON_WM_VSCROLL()
ON_WM_HSCROLL()
ON_WM_CREATE()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_RBUTTONDOWN()
//ON_WM_CLOSE()
END_MESSAGE_MAP()


/*-------------------------------------------------------------------
	�`��
---------------------------------------------------------------------*/
void CChildWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
	// �`�惁�b�Z�[�W�� CWnd::OnPaint() ���Ăяo���Ȃ��ł��������B

	if(!m_EditMode)	OnPaint_RectMode(dc);
	else			OnPaint_CellMode(dc);

	
	if(!(m_EditMode && mc_MouseCtrl)){
		mousedown = FALSE;
		drawprv = FALSE;
	}
}

//����`�ҏW���[�h���̕`��
void CChildWnd::OnPaint_RectMode(CPaintDC& dc)
{
	//Direct3D�`��
	
	//�J�����ݒ�
	g_draw.camera_x = 0.0f;
	g_draw.camera_z = -3.0f;
	g_draw.camera_y = 0.0f;
	g_draw.ResetTransformMatrix();

	g_draw.StartDraw(TRUE);
	g_draw.SetTransform(FALSE);

	RECT r = {0,0,g_DISPLAYWIDTH,g_DISPLAYHEIGHT};
	DWORD bltw,blth;
	int ofsty;
	MYSURFACE *ms = theApp.GetBitmap(mr_current_bmp);
	if(!ms)
	{
		g_draw.DrawRedText(r,"�r�b�g�}�b�v���ǂݍ��߂Ă��܂���",-1,DT_LEFT,2);
		g_draw.EndDraw();
	}
	else
	{
		//�X�N���[���ʒu�ɂ��킹�ăr�b�g�}�b�v��]��
		GetClientRect(&r);

		if(r.right-r.left < ms->wg)bltw=r.right-r.left;//�r�b�g�}�b�v�̕�
		else bltw = (DWORD)ms->wg ;
		if(r.bottom-r.top < ms->hg)blth=r.bottom-r.top;//�r�b�g�}�b�v�̍���
		else blth = (DWORD)ms->hg;

		ofsty=((DWORD)ms->hg)-si_y.nPos-(r.bottom - r.top);
		if(ofsty<0)ofsty=0;

		//[�o�O]�@�E1�s�N�Z���A��2�s�N�Z�����\������Ȃ��B
		r.left	= si_x.nPos;
		r.right	= r.left+bltw;
		r.bottom= (LONG)ms->hg - ofsty;
		r.top	= r.bottom - blth;
		//g_draw.CheckBlt2(ms,0,0,r);

		g_draw.EndDraw();
		//GDI�`��
		CImage& img = theApp.GetImage(mr_current_bmp);
		RECT imgDst;
		imgDst.top = 0;
		imgDst.left = 0;
		imgDst.right = r.right-r.left;
		imgDst.bottom = r.bottom-r.top;
		POINT posSrc;
		posSrc.x = r.left;
		posSrc.y = r.top;
		img.BitBlt(dc,imgDst,posSrc);
	}
	//g_draw.EndDraw();

	//GDI�`��
	GCD_RECT *ed_r = theApp.GetCurrentRect();
	if(!ed_r)return;
	if(mr_current_bmp==ed_r->bmpno){//���̃r�b�g�}�b�v# �ɋ�`������
		//��`�\��
		DrawRECT2(dc,
			ed_r->r.left -si_x.nPos,
			ed_r->r.right-si_x.nPos,
			ed_r->r.top   -si_y.nPos,
			ed_r->r.bottom-si_y.nPos,
			2);
		//���S�\��
		DrawPos(dc,
			ed_r->r.left -si_x.nPos + ed_r->center_x,
			ed_r->r.top  -si_y.nPos + ed_r->center_y,
			FALSE);
	}
}

//���Z���ҏW���[�h���̕`��
void CChildWnd::OnPaint_CellMode(CPaintDC& dc)
{
	CGCDHandler *gcd = theApp.GetGCD();

	g_draw.StartDraw(TRUE);
	g_draw.SetTransform(FALSE);
	{
		//�J�����ݒ�
		float cmz;
		switch(mc_mag){
		case 0:cmz=-3.0f/(1.0f/4.0);break;	//25%
		case 1:cmz=-3.0f/(2.0f/4.0);break;	//50%
		case 2:cmz=-3.0f/(3.0f/4.0);break;	//75%
		case 4:cmz=-3.0f/2.0f;break;		//200%
		case 5:cmz=-3.0f/4.0f;break;		//400%
		default:cmz=-3.0f;break;			//100%
		}
		g_draw.camera_x = 0.0f;
		g_draw.camera_z = cmz;
		g_draw.camera_y = cmz/12.0f;
		g_draw.ResetTransformMatrix();

		//�Z���^�[���C���`��
		g_draw.ResetParentMatrix();
		g_draw.DrawLine(0 + mc_offset_x,-999,0 + mc_offset_x,999,0xFF000000);
		g_draw.DrawLine(-999,0 + mc_offset_y,999,0 + mc_offset_y,0xFF000000);

		//�Z���`��
		g_draw.CellDrawED(
			theApp.GetBitmaps(),
			gcd->m_cdat,
			gcd->m_rdat,
			theApp.GetCurrentCellNO(),
			mc_offset_x,//x
			mc_offset_y,//y
			0,//z
			mc_rotation,//rot
			mc_mirrorX,
			mc_mirrorY,
			0xFFFFFFFF,
			mc_scale_x,
			mc_scale_y,
			theApp.GetCurrentCellRectNO(),						//�ҏW���̋�`�ԍ�
			(mc_EditMode==0||mc_EditMode==5) ? TRUE : FALSE		//�����\�����s��?
			);

		if(mc_EditMode==1){
			//�����`�`��
			g_draw.ResetParentMatrix();
			g_draw.HRectDraw(
					gcd->m_cdat,
					gcd->m_hdat,
					theApp.GetCurrentCellNO(),
					0 + mc_offset_x,0 + mc_offset_y,0,mc_rotation,
					mc_mirrorX,
					mc_mirrorY,
					mc_scale_x, mc_scale_y);
		}
		if(mc_EditMode==2){
			//�d�S�`��
			g_draw.ResetParentMatrix();
			g_draw.GCenterDraw(
					gcd->m_cdat,
					theApp.GetCurrentCellNO(),
					0 + mc_offset_x,0 + mc_offset_y,0,0,
					mc_mirrorX,
					mc_mirrorY,
					mc_scale_x, mc_scale_y);
		}
	}
	g_draw.EndDraw();

	//GDI�`��
	//�}�[�J�[
	if(mc_MarkerON){
		int gcx = gcd->m_cdat[theApp.GetCurrentCellNO()].gcx;
		int gcy = gcd->m_cdat[theApp.GetCurrentCellNO()].gcy;

		DrawLINE(0,g_DISPLAYWIDTH, (mc_MarkerPoint.y - gcy) * mc_scale_y + gcy + mc_offset_y,(mc_MarkerPoint.y - gcy) * mc_scale_y + gcy + mc_offset_y, 0);
		DrawLINE((mc_MarkerPoint.x - gcx) * mc_scale_x + gcx + mc_offset_x,(mc_MarkerPoint.x - gcx) * mc_scale_x + gcx + mc_offset_x,0,g_DISPLAYHEIGHT, 0);
	}
}

/*-------------------------------------------------------------------
	�X�N���[���E�o�[�i�^�e�j
---------------------------------------------------------------------*/
void CChildWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	//CWnd::OnVScroll(nSBCode, nPos, pScrollBar);

	//����`�ҏW���[�h��
	if(!m_EditMode)
	{
		BOOL red=TRUE;
		int ddd=3;
		int dy;

		switch (nSBCode) {
			case SB_LINEUP:  dy = -1 * ddd;break;
			case SB_LINEDOWN:dy =  1 * ddd;break;
			case SB_PAGEUP:  dy = -1 * si_y.nPage;break;
			case SB_PAGEDOWN:dy =      si_y.nPage;break;
			case SB_THUMBPOSITION: dy = nPos - si_y.nPos;break;
			default:dy = 0;break;
		}
		dy = max(-1 * si_y.nPos, min(dy, si_y.nMax - si_y.nPos));
		if(dy>0){
			if( int(si_y.nPos + si_y.nPage + dy) > si_y.nMax)dy=si_y.nMax - si_y.nPage - si_y.nPos;
		}
		if (dy != 0) {
			si_y.nPos += dy;
			SetScrollInfo(SB_VERT, &si_y, TRUE);
			InvalidateRect(NULL,TRUE);
		}
	}
	else
	{
		si_y.nPos = (int)nPos;
		mc_offset_y = si_y.nPos;
		SetScrollInfo(SB_VERT, &si_y, TRUE);
		InvalidateRect(NULL,TRUE);
	}
}

/*-------------------------------------------------------------------
	�X�N���[���E�o�[�i���R�j
---------------------------------------------------------------------*/
void CChildWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	//CWnd::OnHScroll(nSBCode, nPos, pScrollBar);

	BOOL red=TRUE;
	int ddd=3;
	int dx;

	switch (nSBCode) {
		case SB_LINEUP:	 dx = -1 * ddd;break;
		case SB_LINEDOWN:dx =  1 * ddd;break;//�R�R
		case SB_PAGEUP:  dx = -1 * si_x.nPage;break;
		case SB_PAGEDOWN:dx =      si_x.nPage;break;//�R�R
		case SB_THUMBPOSITION:dx = nPos - si_x.nPos;break;
		default:dx = 0;break;
	}
    dx = max(-1 * si_x.nPos, min(dx, si_x.nMax - si_x.nPos));
	if(dx>0){
		if( int(si_x.nPos + si_x.nPage + dx) > si_x.nMax)dx=si_x.nMax - si_x.nPage - si_x.nPos;
	}
    if (dx != 0) {
        si_x.nPos += dx;
        SetScrollInfo(SB_HORZ, &si_x, TRUE);
		InvalidateRect(NULL,TRUE);
    }
}

/*-------------------------------------------------------------------
	�X�N���[���E�o�[���̃��Z�b�g
---------------------------------------------------------------------*/
void CChildWnd::ResetScrollInfo()
{
	if(!m_EditMode)//��`�ҏW���[�h�̏ꍇ
	{
		ShowScrollBar(SB_BOTH,TRUE);

		int wx = g_DISPLAYWIDTH;//MYCHILDWIDTH;
		int wy = g_DISPLAYHEIGHT;//MYCHILDHEIGHT;

		MYSURFACE *ms = theApp.GetBitmap(mr_current_bmp);
		if(ms &&
			m_prev_scroll_width == ms->wg &&
			m_prev_scroll_height == ms->hg)return;

		//�^�e ------------------------------------------------------------
		si_y.cbSize = sizeof(SCROLLINFO);
		si_y.fMask = SIF_POS | SIF_RANGE | SIF_PAGE | SIF_DISABLENOSCROLL;
		si_y.nMin = 0;
		if(ms)
			si_y.nMax = (int)ms->hg;
		else
			si_y.nMax = 0;
		si_y.nPage = wy - GetSystemMetrics(SM_CYHSCROLL);
		si_y.nPos = 0;
		SetScrollInfo(SB_VERT, &si_y, TRUE);
		
		//���R ------------------------------------------------------------
		si_x.cbSize = sizeof(SCROLLINFO);
		si_x.fMask = SIF_POS | SIF_RANGE | SIF_PAGE | SIF_DISABLENOSCROLL;
		si_x.nMin = 0;
		if(ms)
			si_x.nMax = (int)ms->wg;
		else
			si_x.nMax = 0;
		si_x.nPage = wx - GetSystemMetrics(SM_CXHSCROLL);
		si_x.nPos = 0;
		SetScrollInfo(SB_HORZ, &si_x, TRUE);

		m_prev_scroll_width = si_x.nMax;
		m_prev_scroll_height = si_y.nMax;
	}
	else{
		ShowScrollBar(SB_BOTH,FALSE/*TRUE*/);//�Z���ҏW���[�h�̏ꍇ�͔�\��

		int wx = g_DISPLAYWIDTH;//MYCHILDWIDTH;
		int wy = g_DISPLAYHEIGHT;//MYCHILDHEIGHT;

		si_y.cbSize = sizeof(SCROLLINFO);
		si_y.fMask = SIF_POS | SIF_RANGE | SIF_PAGE | SIF_DISABLENOSCROLL;
		si_y.nMin = -300;
		si_y.nMax =  300;
		si_y.nPage = wy - GetSystemMetrics(SM_CYHSCROLL);
		si_y.nPos = mc_offset_y;
		SetScrollInfo(SB_VERT, &si_y, TRUE);
	}
}


/*-------------------------------------------------------------------
	������
---------------------------------------------------------------------*/
int CChildWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO :  �����ɓ���ȍ쐬�R�[�h��ǉ����Ă��������B

	ResetScrollInfo();

	return 0;
}

/*-------------------------------------------------------------------
	�\�����X�V����K�v������
---------------------------------------------------------------------*/
void CChildWnd::Dirty()
{
	mc_MouseCtrl = FALSE;

	ResetScrollInfo();
	Invalidate(TRUE);
}


/*-------------------------------------------------------------------
	�ҏW���[�h�̕ύX
---------------------------------------------------------------------*/
void CChildWnd::ChangeEditMode(BOOL em)
{
	m_EditMode = em;

	Dirty();//�\�����X�V
}

/*-------------------------------------------------------------------
	�r�b�g�}�b�v�̕ύX
---------------------------------------------------------------------*/
void CChildWnd::ChangeBitmap(UINT n)
{
	mr_current_bmp = n;
	Dirty();
}


//******************************************************************************
// GDI�`��
//******************************************************************************

void CChildWnd::DrawRECT(int sx,int gx,int sy,int gy,DWORD type)
{
	HWND hwnd = m_hWnd;

	HDC hdc = ::GetDC(hwnd);
	DrawRECT2(hdc,sx,gx,sy,gy,type);
	::ReleaseDC(hwnd,hdc);
}

void CChildWnd::DrawRECT2(HDC hdc,int sx,int gx,int sy,int gy,DWORD type)
{
	COLORREF pencolor;

	switch(type){
	case 1:pencolor=RGB(0,0,255);break;
	case 2:pencolor=RGB(255,0,0);break;
	case 3:pencolor=RGB(255,255,0);break;
	default:pencolor=RGB(0,0,100);break;
	}

	HPEN penold,pen = CreatePen(PS_SOLID,0,pencolor);
	penold = (HPEN)SelectObject(hdc,pen);

	int oldrop;
	if(type==0)oldrop = SetROP2(hdc,R2_NOT);

	MoveToEx(hdc,sx,sy,NULL);
	LineTo(hdc,sx,gy);
	LineTo(hdc,gx,gy);
	LineTo(hdc,gx,sy);
	LineTo(hdc,sx,sy);

	if(type==0)SetROP2(hdc,oldrop);

	SelectObject(hdc,penold);
}

void CChildWnd::DrawLINE(int sx,int gx,int sy,int gy,DWORD type)
{
	HWND hwnd = m_hWnd;
	HDC hdc = ::GetDC(hwnd);

		COLORREF pencolor;

	switch(type){
	case 1:pencolor=RGB(0,0,255);break;
	case 2:pencolor=RGB(255,0,0);break;
	case 3:pencolor=RGB(255,255,0);break;
	default:pencolor=RGB(0,0,100);break;
	}

	HPEN penold,pen = CreatePen(PS_SOLID,0,pencolor);
	penold = (HPEN)SelectObject(hdc,pen);

	int oldrop;
	if(type==0)oldrop = SetROP2(hdc,R2_NOT);

	MoveToEx(hdc,sx,sy,NULL);
	LineTo(hdc,gx,gy);

	if(type==0)SetROP2(hdc,oldrop);

	SelectObject(hdc,penold);

	::ReleaseDC(hwnd,hdc);
}

void CChildWnd::DrawPos(HDC hdc,int x,int y,BOOL not)
{
	HPEN penold,pen = CreatePen(PS_SOLID,0,RGB(0,0,255));
	penold = (HPEN)SelectObject(hdc,pen);

	int oldrop;
	if(not)oldrop = SetROP2(hdc,R2_NOT);

	MoveToEx(hdc,x-5,y,NULL);
	LineTo(hdc,x+5,y);
	MoveToEx(hdc,x,y-5,NULL);
	LineTo(hdc,x,y+5);

    HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Ellipse(hdc, x-5, y-5, x+5, y+5);
    SelectObject(hdc, hOldBrush);

	if(not)SetROP2(hdc,oldrop);
	SelectObject(hdc,penold);
}



/*-------------------------------------------------------------------
	�}�E�X����
---------------------------------------------------------------------*/
void CChildWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	CWnd::OnLButtonDown(nFlags, point);
	GCD_CELL2 *pc = theApp.GetCurrentCell();
	float mag;

	//����`�ҏW���[�h��
	if(!m_EditMode)
	{
		GCD_RECT *ed_r = theApp.GetCurrentRect();

		if(mr_EditMode==0){
			mousedown=TRUE;
			SetCapture();
			if(drawprv)DrawRECT(m_sx,m_gx,m_sy,m_gy,0);
			m_sx=m_gx=point.x;
			m_sy=m_gy=point.y;
		}
		else if(ed_r!=NULL && !CGCDHandler::GCDIsEmptyRect(*ed_r)){//���̋�`����łȂ��ꍇ�̂�
			ed_r->center_x = point.x - (ed_r->r.left -si_x.nPos);
			ed_r->center_y = point.y - (ed_r->r.top  -si_y.nPos);
			InvalidateRect(NULL,TRUE);
		}
		else{
			theApp.SetStatus("��`�d�S�ݒ�F��`����ł��B�ݒ�ł��܂���B");
		}
	}
	else
	{
		mousedown=TRUE;
		SetCapture();

		//���Z�������`�ҏW��
		if(mc_EditMode==1){
			if(drawprv)DrawRECT(m_sx,m_gx,m_sy,m_gy,0);
			m_sx=m_gx= point.x;
			m_sy=m_gy= point.y;
		}
		//���d�S�ݒ�

		else if(mc_EditMode==2){
			switch(mc_mag){
			case 0:mag=4;break;//25%
			case 1:mag=2;break;//50%
			case 2:mag=1.5;break;//75%
			case 4:mag=0.5;break;//200%
			case 5:mag=0.25;break;//400%
			default:mag=1.0;
			}
			pc->gcx = (int)((point.x-CHILDCENTERX)*mag*(1.0f/mc_scale_x) - mc_offset_x/mc_scale_x);
			pc->gcy = (int)((point.y-CHILDCENTERY)*mag*(1.0f/mc_scale_y) - mc_offset_y/mc_scale_y);
			theApp.InvalidateAll();
		}
		//���}�[�J�[�|�C���g
		else if(mc_EditMode==6){
			mc_MarkerPoint = point;
			CString str;
			str.Format("�}�[�J�[�Z�b�g ( %d , %d )",point.x,point.y);
			theApp.SetStatus(str);
			Invalidate();
		}
		else mc_prevPoint = point;
	}
}

void CChildWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	CWnd::OnLButtonUp(nFlags, point);
	mousedown=FALSE;
	ReleaseCapture();

	//����`�ҏW���[�h��
	if(!m_EditMode)
	{
		if(mr_EditMode==0){
			m_gx=point.x;
			m_gy=point.y;
			mu_xpos=si_x.nPos;
			mu_ypos=si_y.nPos;//bmp_info[radio].hed.biHeight-si_y.nPos-MYCHILDHEIGHT;

			CString str;
			str.Format("%d,%d - %d,%d",
						point.x,point.y,
						si_x.nPos,si_y.nPos);
			theApp.SetStatus(str);
		}
	}
	else
	{
		//���Z�������`�ҏW��
		if(mc_EditMode==1){
			m_gx=point.x;
			m_gy=point.y;
		}
		
		mc_MouseCtrl = FALSE;
	}
}

void CChildWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	CWnd::OnMouseMove(nFlags, point);

	GCD_CELL2 *pc2 = theApp.GetCurrentCell();
	GCD_CELL  *pc  = &pc2->cell[ theApp.GetCurrentCellRectNO() ];

	BOOL no_move=FALSE;

	//����`�ҏW���[�h��
	if(!m_EditMode)
	{
		if(mousedown){
			if(drawprv)DrawRECT(m_sx,m_gx,m_sy,m_gy,0);
			m_gx=point.x;
			m_gy=point.y;
			DrawRECT(m_sx,m_gx,m_sy,m_gy,0);
			drawprv=TRUE;
		}
	}
	else
	{
		if(!mousedown)return;
		int dx = point.x - mc_prevPoint.x;
		int dy = point.y - mc_prevPoint.y;

		switch(mc_EditMode)
		{
			case 0://���I�t�Z�b�g�ݒ�
			{
				pc->dx += dx;
				pc->dy += dy;
			}
			break;
			case 1://���Z�������`�ҏW��
			{
				if(drawprv)DrawRECT(m_sx,m_gx,m_sy,m_gy,0);
				m_gx=point.x;
				m_gy=point.y;
				DrawRECT(m_sx,m_gx,m_sy,m_gy,0);
				drawprv=TRUE;
			}
			break;
			case 3://����]
			{
				pc->rot += dx;
			}
			break;
			case 4://���g��k��
			{
				pc->magx += (float)dx / 100.0f;
				pc->magy += (float)dy / 100.0f;
			}
			break;
			case 5://���I�t�Z�b�g�ݒ�(����)
			{
				dx/=10;
				dy/=10;
				if(dx==0 && dy==0)no_move=TRUE;
				else{
					pc->dx += dx;
					pc->dy += dy;
				}
			}
			break;
		}

		if((mc_EditMode!=5 || !no_move) && mc_EditMode!=1)
		{
			mc_prevPoint = point;
			mc_MouseCtrl = TRUE;
			Invalidate();
		}
	}
}


void CChildWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(GetKeyState(VK_SHIFT) & 0x8000){
		theApp.SpecialCtrl( m_EditMode ? 1 : 0 );//BlankNext(RECT/CELL)
		return;
	}

	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	CWnd::OnRButtonDown(nFlags, point);

	if(m_EditMode)//�Z���ҏW���[�h
	{
/*		mc_MouseLock = !mc_MouseLock;
		if(mc_MouseLock)	theApp.SetStatus("�}�E�X����@���b�N����");
		else				theApp.SetStatus("�}�E�X����@���b�N");
		Invalidate();*/
	}
	else{//��`�ҏW���[�h
		GCD_RECT *r = theApp.GetCurrentRect();
		if(!r)return;
		switch(mr_EditMode)
		{
		case 0:
			r->bmpno = mr_current_bmp;
			r->r = GetCurrentRect();
			break;
		default:return;
		}
		theApp.InvalidateAll();
	}
}


/*-------------------------------------------------------------------
	���ݑI�𒆂̋�`���擾
---------------------------------------------------------------------*/
RECT CChildWnd::GetCurrentRect()
{
	RECT rr;
	if(m_sx < m_gx){
		rr.left =m_sx;
		rr.right=m_gx;
	}
	else{
		rr.left =m_gx;
		rr.right=m_sx;
	}
	if(m_sy < m_gy){
		rr.top   =m_sy;
		rr.bottom=m_gy;
	}
	else{
		rr.top   =m_gy;
		rr.bottom=m_sy;
	}

	if(!m_EditMode)
	{
		rr.left  +=mu_xpos;
		rr.right +=mu_xpos;
		rr.top   +=mu_ypos;
		rr.bottom+=mu_ypos;
	}

	if(m_EditMode)
	{
		rr.left  -=CHILDCENTERX;//mu_xpos;
		rr.right -=CHILDCENTERX;//mu_xpos;
		rr.top   -=CHILDCENTERY;//mu_ypos;
		rr.bottom-=CHILDCENTERY;//mu_ypos;

		double mag=1.0;
		switch(mc_mag){
		case 0:mag=4;break;//25%
		case 1:mag=2;break;//50%
		case 2:mag=1.5;break;//75%
		case 4:mag=0.5;break;//200%
		case 5:mag=0.25;break;//400%
		default:mag=1.0;
		}
		rr.top = (int)(rr.top*mag*(1.0f/mc_scale_y) - mc_offset_y/mc_scale_y);
		rr.bottom = (int)(rr.bottom*mag*(1.0f/mc_scale_y) - mc_offset_y/mc_scale_y);
		rr.left = (int)(rr.left*mag*(1.0f/mc_scale_x) - mc_offset_x/mc_scale_x);
		rr.right = (int)(rr.right*mag*(1.0f/mc_scale_x) - mc_offset_x/mc_scale_x);
	}

	return(rr);
}

/*-------------------------------------------------------------------
	��`�ҏW���[�h�̕ύX
---------------------------------------------------------------------*/
void CChildWnd::ChangeEditModeR(UINT mode)
{
	mr_EditMode = mode;

	switch(mr_EditMode){
	case 0:theApp.SetStatus("��`�ҏW���[�h / ��`�w�胂�[�h");break;
	case 1:theApp.SetStatus("��`�ҏW���[�h / �d�S�w�胂�[�h");break;
	}
	Invalidate();
}

/*-------------------------------------------------------------------
	�Z���ҏW���[�h�̕ύX
---------------------------------------------------------------------*/
void CChildWnd::ChangeEditModeC(UINT m)
{
	mc_EditMode = m;

	switch(mc_EditMode){
	case 0:theApp.SetStatus("�Z���ҏW���[�h / �I�t�Z�b�g�ҏW���[�h");break;
	case 1:theApp.SetStatus("�Z���ҏW���[�h / �����`�w�胂�[�h");break;
	case 2:theApp.SetStatus("�Z���ҏW���[�h / �d�S�w�胂�[�h");break;
	case 3:theApp.SetStatus("�Z���ҏW���[�h / ��]���[�h");break;
	case 4:theApp.SetStatus("�Z���ҏW���[�h / �g�k���[�h");break;
	case 5:theApp.SetStatus("�Z���ҏW���[�h / �I�t�Z�b�g�ҏW���[�h(����)");break;
	case 6:theApp.SetStatus("�Z���ҏW���[�h / �}�[�J�[�ݒ胂�[�h�@�}�[�J�[ON");mc_MarkerON=TRUE;break;
	case 7:theApp.SetStatus("�Z���ҏW���[�h / �}�E�X����Ȃ�");break;
	}
	Invalidate();
}
