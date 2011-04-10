#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "define.h"

#define DISPLAYWIDTH	640
#define DISPLAYHEIGHT	480

#include "define_gcd.h"
#include "function_gcd.h"
#include "resource.h"
#include "function.h"

// edit data ===================================================================
GCD_CELLNAMES ed_hed;
GCD_RECT ed_r[GCDMAX_RECTANGLES];
GCD_CELL2 ed_c[GCDMAX_CELLS];
GCD_HANTEI ed_h[GCDMAX_CELLS];

// global val.=================================================================
HWND hwcld=NULL;
extern HWND ghwnd;

HINSTANCE ghinst=NULL;
extern PBYTE bmp_bits[GCDMAX_IMAGES];

int radio=0;
int radio2=0;
int edit_now;

// function ====================================================================
//�q�E�C���h�E
HWND CreateChildWindow(HWND hdlg);
void CldReset();
void CldRedraw();
RECT GetRECTNow();

//���C���_�C�A���O
BOOL CALLBACK DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
BOOL DlgCommand(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
void DlgDraw(HWND hwndDlg);
void DlgRedraw();

void EditChanged();
void EditChanged2();
void RadioChanged();
void RadioChanged2();
void FindBlank(BOOL pre);
void FindFind(BOOL pre);
void OnKettei();

#define ZeroMemory2(a)	ZeroMemory(a,sizeof(a))

// entry point *****************************************************************
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	ghinst=hInst;

	GCDSetInitialData(ed_c,ed_r,ed_h,&ed_hed);
	edit_now = 1;

	DialogBox(hInst,
		MAKEINTRESOURCE(IDD_DIALOG1),
		GetDesktopWindow(),
		DialogProc);

	for(int i=0;i<GCDMAX_IMAGES;i++){//�r�b�g�}�b�v�̃��������J��
		if(bmp_bits[i]!=NULL){
			free(bmp_bits[i]);
		}
	}

    return 0;
}

// dialog box proc. ------------------------------------------------------------
BOOL CALLBACK DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch( uMsg ){
	case WM_INITDIALOG:
		ghwnd = hwndDlg;
		hwcld = CreateChildWindow(hwndDlg);
		ShowWindow(hwcld,SW_SHOW);
		SendMessage(GetDlgItem(ghwnd,IDC_RADIO1), BM_SETCHECK, BST_CHECKED, 0L);
		SendMessage(GetDlgItem(ghwnd,IDC_RADIO_E1), BM_SETCHECK, BST_CHECKED, 0L);
		break;
	case WM_COMMAND:
		return(DlgCommand(hwndDlg,uMsg,wParam,lParam));break;
	case WM_PAINT:
		DlgDraw(hwndDlg);break;
	case WM_DESTROY:break;
	default:return(FALSE);
	}
	return(TRUE);
}

// command ---------------------------------------------------------------------
BOOL DlgCommand(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	char msgout[256];

	switch(LOWORD(wParam)){
	case IDC_BUTTON_LOAD:
		if(GCDLoadDlg(ed_c,ed_r,ed_h,&ed_hed))
			BitmapReflesh();
		CldReset();
		break;
	case IDC_BUTTON_SAVE:
		GCDSaveDlg(ed_c,ed_r,ed_h,&ed_hed);
		break;

	//edit_now �ړ�
	case IDC_BUTTON_P:
		edit_now--;if(edit_now < 0)edit_now=0;
		EditChanged();
		break;
	case IDC_BUTTON_N:
		edit_now++;if(edit_now > GCDMAX_RECTANGLES-1)edit_now=GCDMAX_RECTANGLES;
		EditChanged();
		break;
	case IDC_BUTTON_P10:
		edit_now-=10;if(edit_now < 0)edit_now=0;
		EditChanged();
		break;
	case IDC_BUTTON_N10:
		edit_now+=10;if(edit_now > GCDMAX_RECTANGLES-1)edit_now=GCDMAX_RECTANGLES;
		EditChanged();
		break;
	case IDC_BUTTON_PB:
		FindBlank(TRUE);EditChanged();
		break;
	case IDC_BUTTON_NB:
		FindBlank(FALSE);EditChanged();
		break;
	case IDC_BUTTON_PF:
		FindFind(TRUE);EditChanged();
		break;
	case IDC_BUTTON_NF:
		FindFind(FALSE);EditChanged();
		break;
	case IDC_EDIT1://���ڎw��
		EditChanged2();break;

	//�r�b�g�}�b�v�ύX
	case IDC_RADIO1:
	case IDC_RADIO2:
	case IDC_RADIO3:
	case IDC_RADIO4:
	case IDC_RADIO5:
	case IDC_RADIO6:
	case IDC_RADIO7:
	case IDC_RADIO8:
	case IDC_RADIO9:
	case IDC_RADIO10:
	case IDC_RADIO11:
	case IDC_RADIO12:RadioChanged();break;

	case IDC_RADIO_E1:
	case IDC_RADIO_E2:RadioChanged2();break;

	//�{�^��
	case IDC_BUTTON_KETTEI:
		OnKettei();break;
	case IDC_BUTTON_ERASE:
		sprintf(msgout,"%d�Ԃ̋�`���������܂��B\n��낵���ł����H",edit_now);
		if(MessageBox(hwndDlg,msgout,"��`����",MB_YESNO)==IDYES){
			ZeroMemory(&ed_r[edit_now],sizeof(GCD_RECT));
			DlgRedraw();
		}break;

	case IDCANCEL:
		EndDialog(hwndDlg,0);break;
	default:return(FALSE);
	}
	return(TRUE);
}

void FindBlank(BOOL pre)//����ۂ̃f�[�^��T��
{
	int tmp = edit_now;

	int a;
	if(pre)a=-1;
	else   a= 1;

	tmp+=a;

	while(tmp < GCDMAX_RECTANGLES && tmp>=1){
		if(ed_r[tmp].bmpno==0 &&
			ed_r[tmp].r.left==0 &&
			ed_r[tmp].r.right==0 &&
			ed_r[tmp].r.top==0 &&
			ed_r[tmp].r.bottom==0){//��������
			edit_now=tmp;
			return;
		}
		else tmp+=a;
	}
	MessageBox(ghwnd,"����܂���","FindBlank()",MB_OK);
}

void FindFind(BOOL pre)//����ۂł͂Ȃ��f�[�^��T��
{
	int tmp = edit_now;

	int a;
	if(pre)a=-1;
	else   a= 1;

	tmp+=a;

	while(tmp < GCDMAX_RECTANGLES && tmp>=1){
		if(ed_r[tmp].bmpno!=0 ||
			ed_r[tmp].r.left!=0 ||
			ed_r[tmp].r.right!=0 ||
			ed_r[tmp].r.top!=0 ||
			ed_r[tmp].r.bottom!=0){//��������
			edit_now=tmp;
			return;
		}
		else tmp+=a;
	}
	MessageBox(ghwnd,"����܂���","FindFind()",MB_OK);
}

void RadioChanged()
{
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO1), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio=0;
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO2), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio=1;
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO3), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio=2;
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO4), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio=3;
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO5), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio=4;
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO6), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio=5;
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO7), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio=6;
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO8), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio=7;
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO9), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio=8;
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO10), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio=9;
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO11), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio=10;
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO12), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio=11;

	DlgRedraw();
	CldReset();
}

void RadioChanged2()
{
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO_E1), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio2=0;
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO_E2), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio2=1;

	DlgRedraw();
	CldReset();
}

void EditChanged()//edit_now�ԍ��̕ύX
{
	char str[256];
	itoa(edit_now,str,10);
	SetDlgItemText(ghwnd,IDC_EDIT1,str);

	DlgRedraw();
}

void EditChanged2()//edit_now�ړ�-���ڔԍ��w��
{
	char str[256];
	GetDlgItemText(ghwnd,IDC_EDIT1,str,256);

	edit_now = atoi(str);
	if(edit_now < 0)edit_now=0;
	if(edit_now > GCDMAX_RECTANGLES-1)edit_now=GCDMAX_RECTANGLES;

	DlgRedraw();
}

void OnKettei()//����{�^��
{
	RECT rn = GetRECTNow();
	ed_r[edit_now].bmpno=radio;
	ed_r[edit_now].r=rn;

	DlgRedraw();
}

// draw dialog box--------------------------------------------------------------
void DlgDraw(HWND hwndDlg)
{
	PAINTSTRUCT ps;
	HDC hdc;

	char msgout[256];
	if(ed_r[edit_now].bmpno == 0
		&& ed_r[edit_now].r.left==ed_r[edit_now].r.right
		&& ed_r[edit_now].r.top==ed_r[edit_now].r.bottom){
		sprintf(msgout,"����`");
	}
	else
		sprintf(msgout,"��`�ς�\nbmp=%d , x=%d�`%d (%d) , y=%d�`%d (%d)\ncenter : %d/%d",
			ed_r[edit_now].bmpno,
			ed_r[edit_now].r.left,ed_r[edit_now].r.right,
			ed_r[edit_now].r.right - ed_r[edit_now].r.left,
			ed_r[edit_now].r.top,ed_r[edit_now].r.bottom,
			ed_r[edit_now].r.bottom - ed_r[edit_now].r.top,
			ed_r[edit_now].center_x,ed_r[edit_now].center_y);

	RECT r;
	r.top=410;
	r.bottom=500;
	r.left=0;
	r.right=510;

	hdc = BeginPaint(hwndDlg, &ps);
	int oldmode = SetROP2(hdc,TRANSPARENT);//�e�L�X�g�w�i�𓧉߂ɂ���
	DrawText(hdc,msgout,-1,&r,DT_LEFT);
	SetROP2(hdc,oldmode);
	EndPaint(hwndDlg, &ps);
}

void DlgRedraw()
{
	RECT r;
	r.top=410;
	r.bottom=500;
	r.left=0;
	r.right=510;
	InvalidateRect(ghwnd,&r,TRUE);
	CldRedraw();
}