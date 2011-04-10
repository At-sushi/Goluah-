
#include <windows.h>
#include <stdio.h>
#include "define.h"
#include "define_gcd.h"
#include "function_gcd.h"
#include "function.h"

#define MYCHILDWIDTH (500)
#define MYCHILDHEIGHT (400)
// global val.-------------------------------------------------------------------------------

extern HWND ghwnd;
extern GCD_CELLNAMES ed_hed;
extern GCD_RECT ed_r[GCDMAX_RECTANGLES];
extern GCD_CELL2 ed_c[GCDMAX_CELLS];
extern GCD_HANTEI ed_h[GCDMAX_CELLS];
extern int radio;
extern int radio2;
extern int edit_now;

extern DWORD bmp_loaded[GCDMAX_IMAGES];
extern BMPINFO256 bmp_info[GCDMAX_IMAGES];
extern PBYTE bmp_bits[GCDMAX_IMAGES];

SCROLLINFO si_y,si_x;//スクロール
HWND hwc;

BOOL mousedown=FALSE;
BOOL drawprv=FALSE;
int sx=0,gx=0,sy=0,gy=0,mu_xpos=0,mu_ypos=0;

// function ------------------------------------------------------------------------------
LRESULT WINAPI CldMsgProc( HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam );
HWND CreateChildWindow(HWND hdlg);
void WMScroll(HWND hwnd,UINT msg,DWORD wp,DWORD lp);
void MakeScrollInfo(HWND hWnd);
void CldDraw(HWND hwnd);
void CldReset(){MakeScrollInfo(hwc);InvalidateRect(hwc,NULL,TRUE);}
void CldRedraw(){InvalidateRect(hwc,NULL,TRUE);}

RECT GetRECTNow()
{
	RECT rr;
	if(sx < gx){
		rr.left =sx;
		rr.right=gx;
	}
	else{
		rr.left =gx;
		rr.right=sx;
	}
	if(sy < gy){
		rr.top   =sy;
		rr.bottom=gy;
	}
	else{
		rr.top   =gy;
		rr.bottom=sy;
	}

	rr.left  +=mu_xpos;
	rr.right +=mu_xpos;
	rr.top   +=mu_ypos;
	rr.bottom+=mu_ypos;

	return(rr);
}

//*****************************************************************************************
// window描画 
//*****************************************************************************************

void CldDraw(HWND hwnd)
{
	drawprv=FALSE;

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd,&ps);

	RECT r;
	GetClientRect(hwnd,&r);

	char msgout[256];
	DWORD bltw,blth;
	int ofsty;

	//念のためラジオボタンの値をチェック
	if(radio<0 || radio>=GCDMAX_IMAGES){
		sprintf(msgout,"ERROR , ラジオボタンの値が異常です");
		TextOut(hdc,0,0,msgout,strlen(msgout));
	}
	//ビットマップが読み込まれていないときはメッセージを表示。
	else if(bmp_loaded[radio] != 0){
		switch(bmp_loaded[radio]){
		case 1:
			sprintf(msgout,"まだビットマップが読み込まれていません\n");
			DrawText(hdc,msgout,-1,&r,DT_LEFT);
			break;
		case 2:
			sprintf(msgout,"ファイルが存在しないか、または開けませんでした\n");
			DrawText(hdc,msgout,-1,&r,DT_LEFT);
			break;
		case 3:
			sprintf(msgout,"ファイル読み込み中になにかエラーが起きました\n");
			DrawText(hdc,msgout,-1,&r,DT_LEFT);
			break;
		case 4:
			sprintf(msgout,"つーかこのファイルはビットマップではないだろ？ｺﾞﾙｧ\n");
			DrawText(hdc,msgout,-1,&r,DT_LEFT);
			break;
		case 5:
			sprintf(msgout,"ビットマップヘッダで何らかの不都合がありました\n");
			DrawText(hdc,msgout,-1,&r,DT_LEFT);
			break;
		default:
			sprintf(msgout,"エラーを特定できません\n");
			DrawText(hdc,msgout,-1,&r,DT_LEFT);
			break;
		}
	}
	//スクロール位置にあわせてビットマップを転送
	else{
		if(r.right-r.left < bmp_info[radio].hed.biWidth)bltw=r.right-r.left;//ビットマップの幅
		else bltw=bmp_info[radio].hed.biWidth;
		if(r.bottom-r.top < bmp_info[radio].hed.biHeight)blth=r.bottom-r.top;//ビットマップの高さ
		else blth=bmp_info[radio].hed.biHeight;

		ofsty=(bmp_info[radio].hed.biHeight)-si_y.nPos-(r.bottom - r.top);
		if(ofsty<0)ofsty=0;

		StretchDIBits(hdc,
			0,0,bltw,blth,
			si_x.nPos,ofsty,bltw,blth,
			bmp_bits[radio],(BITMAPINFO*)&bmp_info[radio],DIB_RGB_COLORS,SRCCOPY);
	}

	if((DWORD)radio==ed_r[edit_now].bmpno){//このビットマップ# に矩形がある
		//矩形表示
		DrawRECT2(hdc,
			ed_r[edit_now].r.left -si_x.nPos,
			ed_r[edit_now].r.right-si_x.nPos,
			ed_r[edit_now].r.top   -si_y.nPos,
			ed_r[edit_now].r.bottom-si_y.nPos,
			2);
		//中心表示
		DrawPos(hdc,
			ed_r[edit_now].r.left -si_x.nPos + ed_r[edit_now].center_x,
			ed_r[edit_now].r.top  -si_y.nPos + ed_r[edit_now].center_y,
			FALSE);
	}

	EndPaint(hwnd,&ps);
}

// window message handler -----------------------------------------------------------

LRESULT WINAPI CldMsgProc( HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	switch( msg )
    {
	case WM_LBUTTONDOWN:
		if(radio2==0){
			mousedown=TRUE;
			if(drawprv)DrawRECT(hWnd,sx,gx,sy,gy,0);
			sx=gx=LOWORD(lparam);
			sy=gy=HIWORD(lparam);
		}
		else if(ed_r[edit_now].bmpno != 0
					|| ed_r[edit_now].r.left!=ed_r[edit_now].r.right
					|| ed_r[edit_now].r.top!=ed_r[edit_now].r.bottom){//その矩形が空でない場合のみ
			ed_r[edit_now].center_x = LOWORD(lparam) - (ed_r[edit_now].r.left -si_x.nPos);
			ed_r[edit_now].center_y = HIWORD(lparam) - (ed_r[edit_now].r.top  -si_y.nPos);
			InvalidateRect(hWnd,NULL,TRUE);
		}
		break;
	case WM_LBUTTONUP:
		if(radio2==0){
			mousedown=FALSE;
			gx=LOWORD(lparam);
			gy=HIWORD(lparam);
			mu_xpos=si_x.nPos;
			mu_ypos=si_y.nPos;//bmp_info[radio].hed.biHeight-si_y.nPos-MYCHILDHEIGHT;
		}
		break;
	case WM_MOUSEMOVE:
		if(mousedown){
			if(drawprv)DrawRECT(hWnd,sx,gx,sy,gy,0);
			gx=LOWORD(lparam);
			gy=HIWORD(lparam);
			DrawRECT(hWnd,sx,gx,sy,gy,0);
			drawprv=TRUE;
		}
		break;

	case WM_PAINT:
		CldDraw(hWnd);
		break;

	case WM_SIZE:
		MakeScrollInfo(hWnd);InvalidateRect(hWnd,NULL,TRUE);break;
    case WM_CREATE:
		MakeScrollInfo(hWnd);return(0);

	case WM_VSCROLL:
	case WM_HSCROLL:
		WMScroll(hWnd,msg,wparam,lparam);return(0);

	case WM_CLOSE:
	case WM_DESTROY:
//		DestroyWindow(hWnd);
		return(0);
	default:
		break;
    }
    return DefWindowProc( hWnd, msg, wparam, lparam );
}

//*****************************************************************************************
//　スクロールおよびウインドウ生成。恐らくもういじらないほうがいい
//*****************************************************************************************

// set scroll info -----------------------------------------------------------

void MakeScrollInfo(HWND hWnd)
{
	int wx = MYCHILDWIDTH;
	int wy = MYCHILDHEIGHT;

	//GetClientRect(hWnd, &rc);

	//vertical scroll bar----------------------------------------------
	si_y.cbSize = sizeof(SCROLLINFO);
    si_y.fMask = SIF_POS | SIF_RANGE | SIF_PAGE | SIF_DISABLENOSCROLL;
    si_y.nMin = 0;
	if(bmp_loaded[radio]==0)
	    si_y.nMax = bmp_info[radio].hed.biHeight;
	else
		si_y.nMax = 0;
    si_y.nPage = wy - GetSystemMetrics(SM_CYHSCROLL);
    si_y.nPos = 0;
    SetScrollInfo(hWnd, SB_VERT, &si_y, TRUE);
	
	//holizonal scroll bar --------------------------------------------
	si_x.cbSize = sizeof(SCROLLINFO);
    si_x.fMask = SIF_POS | SIF_RANGE | SIF_PAGE | SIF_DISABLENOSCROLL;
    si_x.nMin = 0;
	if(bmp_loaded[radio]==0)
	    si_x.nMax = bmp_info[radio].hed.biWidth;
	else
		si_x.nMax = 0;
    si_x.nPage = wx - GetSystemMetrics(SM_CXHSCROLL);
    si_x.nPos = 0;
    SetScrollInfo(hWnd, SB_HORZ, &si_x, TRUE);

/*	char msg[256];
	sprintf(msg,"高さ：%d/幅：%d",bmp_info[radio].hed.biHeight,bmp_info[radio].hed.biWidth);
	MessageBox(ghwnd,msg,"",MB_OK);*/
}

// message on wm_scroll -------------------------------------------------------

void WMScroll(HWND hwnd,UINT msg,DWORD wp,DWORD lp)
{
	BOOL red=TRUE;
	int ddd=3;

	int dx,dy;


//----------------------------------------------------------------
/*	switch (LOWORD(wp)) {
                case SB_LINEUP:			MessageBox(ghwnd,"SB_LINEUP","",MB_OK);break;//ちっちゃいボタン（上）
                case SB_LINEDOWN:		MessageBox(ghwnd,"SB_LINEDOWN","",MB_OK);break;//ちっちゃいボタン（下）
                case SB_PAGEUP:			MessageBox(ghwnd,"SB_PAGEUP","",MB_OK);break;//空白部分（上）
                case SB_PAGEDOWN:		MessageBox(ghwnd,"SB_PAGEDOWN","",MB_OK);break;//空白部分（下）
                case SB_THUMBTRACK:		MessageBox(ghwnd,"SB_THUMBTRACK","",MB_OK);break;//ドラッグ（動かし中）
				case SB_THUMBPOSITION:	MessageBox(ghwnd,"SB_THUMBPOSITION","",MB_OK);break;//ドラッグ（ボタン離した）
                default:dy = 0;break;
            }*/
//----------------------------------------------------------------

	switch(msg){
	case WM_VSCROLL:
		switch (LOWORD(wp)) {
                case SB_LINEUP:  dy = -1 * ddd;break;
                case SB_LINEDOWN:dy =  1 * ddd;break;
                case SB_PAGEUP:  dy = -1 * si_y.nPage;break;
                case SB_PAGEDOWN:dy =      si_y.nPage;break;
              //case SB_THUMBTRACK:
				case SB_THUMBPOSITION: dy = HIWORD(wp) - si_y.nPos;break;
                default:dy = 0;break;
            }
            dy = max(-1 * si_y.nPos, min(dy, si_y.nMax - si_y.nPos));
			if(dy>0){
				if( int(si_y.nPos + si_y.nPage + dy) > si_y.nMax)dy=si_y.nMax - si_y.nPage - si_y.nPos;
			}
            if (dy != 0) {
                si_y.nPos += dy;
                SetScrollInfo(hwnd, SB_VERT, &si_y, TRUE);
    //            ScrollWindow(hwnd, 0, -dy, NULL, NULL);
      //          UpdateWindow(hwnd);
				InvalidateRect(hwnd,NULL,TRUE);
            }
            break;
	case WM_HSCROLL:
		switch (LOWORD(wp)) {
                case SB_LINEUP:	 dx = -1 * ddd;break;
                case SB_LINEDOWN:dx =  1 * ddd;break;//ココ
                case SB_PAGEUP:  dx = -1 * si_x.nPage;break;
                case SB_PAGEDOWN:dx =      si_x.nPage;break;//ココ
				case SB_THUMBPOSITION:dx = HIWORD(wp) - si_x.nPos;break;
                default:dx = 0;break;
            }
            dx = max(-1 * si_x.nPos, min(dx, si_x.nMax - si_x.nPos));
			if(dx>0){
				if( int(si_x.nPos + si_x.nPage + dx) > si_x.nMax)dx=si_x.nMax - si_x.nPage - si_x.nPos;
			}
            if (dx != 0) {
                si_x.nPos += dx;
                SetScrollInfo(hwnd, SB_HORZ, &si_x, TRUE);
      //          ScrollWindow(hwnd, -dx, 0, NULL, NULL);
     //           UpdateWindow(hwnd);
				InvalidateRect(hwnd,NULL,TRUE);
            }
            break;
	}

	
/*	char cmsg[256];
	sprintf(cmsg,"h:%d / w:%d",si_y.nPos,si_x.nPos);
	MessageBox(ghwnd,cmsg,"",MB_OK);*/
}

// window 生成----------------------------------------------------------------------------

HWND CreateChildWindow(HWND hdlg)
{
	// Register the window class
    WNDCLASSEX wc;
	wc.cbSize		= sizeof(WNDCLASSEX);
	wc.style		= CS_CLASSDC;
	wc.lpfnWndProc	= CldMsgProc;
	wc.cbClsExtra	= 0L;
	wc.cbWndExtra	= 0L; 
    wc.hInstance	= GetModuleHandle(NULL);
	wc.hIcon		= NULL;
	wc.hCursor		= NULL;
	wc.hbrBackground= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName	= NULL;
	wc.lpszClassName= "child";
    wc.hIconSm		= NULL;
    RegisterClassEx( &wc );

    // Create the application's window
    HWND hWnd = CreateWindow(
		"child",
		"child",
		WS_CHILD | WS_VSCROLL | WS_HSCROLL,
		5,
		5,
		MYCHILDWIDTH,
		MYCHILDHEIGHT,
		hdlg,
		NULL,
		wc.hInstance,
		NULL);
	hwc = hWnd;
	return(hWnd);
}
