#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <d3d8.h>
#include <d3dx8.h>
#include "define.h"

#include "define_gcd.h"
#include "function_gcd.h"
#include "function.h"
#include "l_directdraw.h"

// global val.=================================================================

MYSURFACE* dds[12]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
extern HWND ghwnd;
extern CDirectDraw *pddraw;
extern int edit_now;

char datfilepath[256];

// edit data ===================================================================
extern GCD_CELLNAMES ed_hed;
extern GCD_RECT		ed_r[GCDMAX_RECTANGLES];
extern GCD_CELL2	ed_c[GCDMAX_CELLS];
extern GCD_HANTEI	ed_h[GCDMAX_CELLS];

void ShowDatNow()
{
	if(edit_now>1024){
		MessageBox(ghwnd,"over 1024","show data now",MB_OK);
		return;
	}

	char msg[1024];
	sprintf(msg,"");
	sprintf(&msg[strlen(msg)],"");
	sprintf(&msg[strlen(msg)],"edit_now = %d\n\n",edit_now);
	sprintf(&msg[strlen(msg)],"g.c. = %d/%d \n",ed_c[edit_now].gcx,ed_c[edit_now].gcy);
	for(int i=0;i<8;i++){
		sprintf(&msg[strlen(msg)],"r=%d ",ed_c[edit_now].cell[i].cdr);
		sprintf(&msg[strlen(msg)]," dx/dy=%d/%d ",ed_c[edit_now].cell[i].dx,ed_c[edit_now].cell[i].dy);
		sprintf(&msg[strlen(msg)]," mag:%f/%f ",ed_c[edit_now].cell[i].magx,ed_c[edit_now].cell[i].magy);		
		sprintf(&msg[strlen(msg)]," rot:%d ",ed_c[edit_now].cell[i].rot);
		sprintf(&msg[strlen(msg)],"- flag:%d\n",ed_c[edit_now].cell[i].flag);
	}

	MessageBox(ghwnd,msg,"dat now",MB_OK);
}

BOOL BitmapReflesh()
{
	if(pddraw == NULL){
		return(FALSE);
	}

	int i;
	char filepath[256];

	for(i=0;i<GCDMAX_IMAGES;i++){
		pddraw->RelSurface(dds[i]);
		sprintf(filepath,"%s\\image%d.bmp",datfilepath,i+1);
		dds[i] = pddraw->CreateSurfaceFrom256BMP(filepath);
	}
	return(TRUE);
}

void DrawRECT(HWND hwnd,int sx,int gx,int sy,int gy,DWORD type)
{
	HDC hdc = GetDC(hwnd);
	DrawRECT2(hdc,sx,gx,sy,gy,type);
	ReleaseDC(hwnd,hdc);
}

void DrawRECT2(HDC hdc,int sx,int gx,int sy,int gy,DWORD type)
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

void DrawLINE(HWND hwnd,int sx,int gx,int sy,int gy,DWORD type)
{
	HDC hdc = GetDC(hwnd);

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

	ReleaseDC(hwnd,hdc);
}

// draw text

HFONT SetMyFont(LPCTSTR face, int h,int fontwaight=FW_NORMAL)
{
    HFONT hFont;
    hFont = CreateFont(h,    //フォント高さ
        0,                    //文字幅
        0,                    //テキストの角度
        0,                    //ベースラインとｘ軸との角度
        fontwaight,            //フォントの重さ（太さ）
        FALSE,                //イタリック体
        FALSE,                //アンダーライン
        FALSE,                //打ち消し線
        SHIFTJIS_CHARSET,    //文字セット
        OUT_DEFAULT_PRECIS,    //出力精度
        CLIP_DEFAULT_PRECIS,//クリッピング精度
        PROOF_QUALITY,        //出力品質
        FIXED_PITCH | FF_MODERN,//ピッチとファミリー
        face);    //書体名
    return hFont;
}

void DrawBlueText(HDC hdc,RECT r,char *text,int len,DWORD method,int hh)
{
/*	if(config2.halfmode){
		hh/=2;
		r.top/=2;
		r.left/=2;
		r.right/=2;
		r.bottom/=2;
	}*/

	HFONT hfont1 = SetMyFont("ＭＳ Ｐゴシック",hh);
	HFONT hfontold = (HFONT)SelectObject(hdc,hfont1);
	int oldmode = SetBkMode(hdc,TRANSPARENT);

	DWORD oldcolor = SetTextColor(hdc,RGB(50,100,150));
	r.top--;
	r.left--;
	DrawText(hdc,text,len,&r,method);

	SetTextColor(hdc,RGB(100,200,250));
	r.top++;
	r.left++;
	DrawText(hdc,text,len,&r,method);

	SetTextColor(hdc,oldcolor);
	if(oldmode!=0)SetBkMode(hdc,oldmode);
	SelectObject(hdc,hfontold);
	DeleteObject(hfont1);
}
