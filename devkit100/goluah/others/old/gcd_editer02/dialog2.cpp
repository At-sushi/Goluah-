#include <windows.h>
#include <stdio.h>
#include <d3d8.h>
#include <d3dx8.h>

#include "define.h"

#include "define_gcd.h"
#include "function_gcd.h"
#include "function.h"
#include "l_directdraw.h"
#include "resource.h"

extern GCD_CELLNAMES ed_hed;
extern GCD_RECT		ed_r[GCDMAX_RECTANGLES];
extern GCD_CELL2	ed_c[GCDMAX_CELLS];
extern GCD_HANTEI	ed_h[GCDMAX_CELLS];

extern int radio,radio2,radioview;
extern int edit_now;

BOOL CALLBACK DialogProc2(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	char str[256];

	switch( uMsg ){
	case WM_INITDIALOG:
		sprintf(str,"%f",ed_c[edit_now].cell[radio2].magx);
		SetDlgItemText(hwndDlg,IDC_EDIT1,str);
		sprintf(str,"%f",ed_c[edit_now].cell[radio2].magy);
		SetDlgItemText(hwndDlg,IDC_EDIT3,str);
		sprintf(str,"%d",ed_c[edit_now].cell[radio2].rot);
		SetDlgItemText(hwndDlg,IDC_EDIT4,str);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDC_BUTTON1://clear		
			sprintf(str,"1");
			SetDlgItemText(hwndDlg,IDC_EDIT1,str);
			sprintf(str,"1");
			SetDlgItemText(hwndDlg,IDC_EDIT3,str);
			sprintf(str,"0");
			SetDlgItemText(hwndDlg,IDC_EDIT4,str);
			break;
		case IDOK://OK
			GetDlgItemText(hwndDlg,IDC_EDIT1,str,256);
			ed_c[edit_now].cell[radio2].magx = (float)atof(str);
			GetDlgItemText(hwndDlg,IDC_EDIT3,str,256);
			ed_c[edit_now].cell[radio2].magy = (float)atof(str);
			GetDlgItemText(hwndDlg,IDC_EDIT4,str,256);
			ed_c[edit_now].cell[radio2].rot  = atoi(str);
			EndDialog(hwndDlg,0);
			break;
		case IDCANCEL:EndDialog(hwndDlg,0);break;//ƒLƒƒƒ“ƒZƒ‹‚³‚ê‚½
		}
	default:return(FALSE);
	}
	return(TRUE);
}