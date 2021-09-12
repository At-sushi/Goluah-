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
#include "resource.h"

// global val.=================================================================
HWND hwcld=NULL;
HWND ghwnd=NULL;

int radio =0;//エディットモード（オフセット/判定）
int radio2=0;//セル#2
int edit_now;
int radioview=-1;

CDirectDraw *pddraw;
BOOL testmirrorx=FALSE,testmirrory=FALSE;//左右反転テスト
BOOL tobasilink=FALSE;

// edit data ===================================================================
GCD_CELLNAMES ed_hed;
GCD_RECT	ed_r[GCDMAX_RECTANGLES];
GCD_CELL2	ed_c[GCDMAX_CELLS];
GCD_HANTEI	ed_h[GCDMAX_CELLS];

GCD_CELL2 ed_copy;//コピーのための一時データ置き場
GCD_HANTEI ed_copy2;

// function ====================================================================
HWND CreateChildWindow(HWND hdlg);
void CldReset();
void CldRedraw();
RECT GetRECTNow();
void CldMove();

BOOL CALLBACK DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
BOOL CALLBACK DialogProc2(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
BOOL DlgCommand(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
void DlgDraw(HWND hwndDlg);
void DlgRedraw();

void EditChanged();
void EditChanged2();
void Edit2Changed();
void Edit2Changed2();
void RadioChanged();
void FindBlank(BOOL pre);
void FindFind(BOOL pre);
BOOL FindRect(BOOL fwd,int num);

void SetKasanari(DWORD n);
void SetKurai(DWORD n);
void SetKougeki(DWORD n);
void CheckChanged();

int SetWinCenter(HWND hWnd);
// entry point *****************************************************************
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	GCDSetInitialData(ed_c,ed_r,ed_h,&ed_hed);

	DialogBox(hInst,
		MAKEINTRESOURCE(IDD_DIALOG1),
		GetDesktopWindow(),
		DialogProc);

	delete(pddraw);
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
		SendMessage(GetDlgItem(ghwnd,IDC_RADIO1), BM_SETCHECK, BST_CHECKED, 0);
		SendMessage(GetDlgItem(ghwnd,IDC_RADIOSEL1), BM_SETCHECK, BST_CHECKED, 0);
		SendMessage(GetDlgItem(ghwnd,IDC_RADIO_VIEW4), BM_SETCHECK, BST_CHECKED, 0);
		pddraw->clearbgcolor = 0xFFF0F0F0;//でふぉの背景色
		SendMessage(GetDlgItem(ghwnd,IDC_RADIO_BG1), BM_SETCHECK, BST_CHECKED, 0);
		RadioChanged();
		EditChanged();
		SetWinCenter(hwndDlg);
		break;
	case WM_COMMAND:
		return(DlgCommand(hwndDlg,uMsg,wParam,lParam));break;
	case WM_PAINT:
		DlgDraw(hwndDlg);break;
	case WM_KEYDOWN:
		break;
	case WM_MOVE:
		CldMove();
		break;
	default:return(FALSE);
	}
	return(TRUE);
}

// command ---------------------------------------------------------------------
BOOL DlgCommand(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	char msgout[256];

	GCD_CELL fcell;

	switch(LOWORD(wParam)){
	case IDC_BUTTON_LOAD:
		if(GCDLoadDlg(ed_c,ed_r,ed_h,&ed_hed))
			BitmapReflesh();
		CldReset();break;
	case IDC_BUTTON_SAVE:GCDSaveDlg(ed_c,ed_r,ed_h,&ed_hed);break;
	case IDC_BUTTON_SAVE2:GCDSaveHeader(&ed_hed);break;

	//edit_now移動
	case IDC_BUTTON_P:
		if(edit_now==1 || edit_now==0){
			edit_now=0;
			break;
		}
		edit_now--;
		while( tobasilink && (ed_c[edit_now-1].flag&GCDCELL2_LINK) ){
			edit_now--;
			if(edit_now < 2)break;
		}
		if(edit_now < 0)edit_now=0;
		EditChanged();
		break;
	case IDC_BUTTON_N:
		edit_now++;
		while( tobasilink && (ed_c[edit_now-1].flag&GCDCELL2_LINK) ){
			edit_now++;
			if(edit_now > GCDMAX_RECTANGLES-1)break;
		}
		if(edit_now > GCDMAX_RECTANGLES-1)edit_now=GCDMAX_RECTANGLES;
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

	//cell# 変更
	case IDC_RADIO1:
	case IDC_RADIO2:
	case IDC_RADIO3:
	case IDC_RADIO4:
	case IDC_RADIO5:
	case IDC_RADIO6:
	case IDC_RADIOSEL1:
	case IDC_RADIOSEL2:
	case IDC_RADIOSEL3:
	case IDC_RADIOSEL4:
	case IDC_RADIOSEL5:
	case IDC_RADIOSEL6:
	case IDC_RADIOSEL7:
	case IDC_RADIOSEL8:
	case IDC_RADIO_VIEW1:
	case IDC_RADIO_VIEW2:
	case IDC_RADIO_VIEW3:
	case IDC_RADIO_VIEW4:
	case IDC_RADIO_VIEW5:
	case IDC_RADIO_VIEW6:
		RadioChanged();break;

	//判定セットボタン
	case IDC_BUTTON_KASANARI0://重なり判定
		SetKasanari(0);break;
	case IDC_BUTTON_KASANARI1:
		SetKasanari(1);break;
	case IDC_BUTTON_KASANARI2:
		SetKasanari(2);break;
	case IDC_BUTTON_KURAI0://喰らい判定
		SetKurai(0);break;
	case IDC_BUTTON_KURAI1:
		SetKurai(1);break;
	case IDC_BUTTON_KURAI2:
		SetKurai(2);break;
	case IDC_BUTTON_KOUGEKI0://攻撃判定
		SetKougeki(0);break;
	case IDC_BUTTON_KOUGEKI1:
		SetKougeki(1);break;
	case IDC_BUTTON_KOUGEKI2:
		SetKougeki(2);break;

	//edit_now#直接指定 / セル定義名変更
	case IDC_EDIT1:
		EditChanged2();break;
	case IDC_EDIT2:
		Edit2Changed2();break;

	//セル入れ替え処理
	case IDC_BUTTONFLIP1:
		fcell = ed_c[edit_now].cell[0];
		ed_c[edit_now].cell[0] = ed_c[edit_now].cell[1];
		ed_c[edit_now].cell[1] =fcell;
		CldRedraw();
		break;
	case IDC_BUTTONFLIP2:
		fcell = ed_c[edit_now].cell[1];
		ed_c[edit_now].cell[1] = ed_c[edit_now].cell[2];
		ed_c[edit_now].cell[2] =fcell;
		CldRedraw();
		break;
	case IDC_BUTTONFLIP3:
		fcell = ed_c[edit_now].cell[2];
		ed_c[edit_now].cell[2] = ed_c[edit_now].cell[3];
		ed_c[edit_now].cell[3] =fcell;
		CldRedraw();
		break;
	case IDC_BUTTONFLIP4:
		fcell = ed_c[edit_now].cell[3];
		ed_c[edit_now].cell[3] = ed_c[edit_now].cell[4];
		ed_c[edit_now].cell[4] =fcell;
		CldRedraw();
		break;
	case IDC_BUTTONFLIP5:
		fcell = ed_c[edit_now].cell[4];
		ed_c[edit_now].cell[4] = ed_c[edit_now].cell[5];
		ed_c[edit_now].cell[5] =fcell;
		CldRedraw();
		break;
	case IDC_BUTTONFLIP6:
		fcell = ed_c[edit_now].cell[5];
		ed_c[edit_now].cell[5] = ed_c[edit_now].cell[6];
		ed_c[edit_now].cell[6] =fcell;
		CldRedraw();
		break;
	case IDC_BUTTONFLIP7:
		fcell = ed_c[edit_now].cell[6];
		ed_c[edit_now].cell[6] = ed_c[edit_now].cell[7];
		ed_c[edit_now].cell[7] =fcell;
		CldRedraw();
		break;

	//チェックボックス変更処理
	case IDC_CHECK1:
	case IDC_CHECK2:
	case IDC_TESTCHECK1:
	case IDC_TESTCHECK2:
	case IDC_CHECK_LINK:
	case IDC_TOBASILINK:
		CheckChanged();break;

	//背景色指定
	case IDC_RADIO_BG1:pddraw->clearbgcolor = 0xFFF0F0F0;CldRedraw();break;
	case IDC_RADIO_BG2:pddraw->clearbgcolor = 0xFF0F0F0F;CldRedraw();break;
	case IDC_RADIO_BG3:pddraw->clearbgcolor = 0xFFF000F0;CldRedraw();break;
	case IDC_RADIO_BG4:pddraw->clearbgcolor = 0xFF00F0F0;CldRedraw();break;

	//矩形番号変更処理
	case IDC_BUTTON_BN  :if(FindRect(TRUE , 1))DlgRedraw();break;
	case IDC_BUTTON_BN10:if(FindRect(TRUE ,10))DlgRedraw();break;
	case IDC_BUTTON_BP  :if(FindRect(FALSE, 1))DlgRedraw();break;
	case IDC_BUTTON_BP10:if(FindRect(FALSE,10))DlgRedraw();break;

	//「消去」ボタン
	case IDC_BUTTON_ERASE:
		sprintf(msgout,"%d番のデータを消去します。\nイイデスカ？",edit_now);
		if(MessageBox(hwndDlg,msgout,"データ消去",MB_YESNO)==IDYES){
			GCDSetEmptyCell2(&ed_c[edit_now],&ed_h[edit_now]);
			DlgRedraw();
		}break;

	//「コピー」「貼り付け」ボタン
	case IDC_BUTTON_COPY:
		ed_copy = ed_c[edit_now];
		ed_copy2 = ed_h[edit_now];
//		MessageBox(hwndDlg,"コピー","しますた",MB_OK);
		break;
	case IDC_BUTTON_PASTE:
		if(MessageBox(hwndDlg,"コピーしたデータを貼り付けます。\nイイデスカ？",
													"paste",MB_YESNO)==IDYES){
			ed_c[edit_now] = ed_copy;
			ed_h[edit_now] = ed_copy2;
			DlgRedraw();
//			MessageBox(hwndDlg,"コピー","実行しますた",MB_OK);
		}
		break;

	//判定矩形のリセット
	case IDC_BUTTON_RESETHANTEI:
		if(MessageBox(hwndDlg,"判定の矩形を全部クリアします。\nOK?","確認",MB_YESNO)==IDYES){
			ZeroMemory(&ed_h[edit_now],sizeof(GCD_HANTEI));
			CldRedraw();
		}
		break;

	case IDC_BUTTON_MAG://回転角と拡大率の数値指定
		DialogBox(GetModuleHandle(NULL),
			MAKEINTRESOURCE(IDD_DIALOG2),
			hwndDlg,
			DialogProc2);
		CldRedraw();
		break;

	case IDCANCEL:EndDialog(hwndDlg,0);break;
	default:return(FALSE);
	}
	return(TRUE);
}

void FindBlank(BOOL pre)//空っぽのデータを探す
{
	int tmp = edit_now;

	int a;
	if(pre)a=-1;
	else   a= 1;

	tmp+=a;

	while(tmp < GCDMAX_RECTANGLES && tmp>=0){
		if(GCDIsEmptyCell2(ed_c[tmp],ed_h[tmp])){//見つかった
			edit_now=tmp;
			return;
		}
		else tmp+=a;
	}
	MessageBox(ghwnd,"ありません","FindBlank()",MB_OK);
}

void FindFind(BOOL pre)//空っぽではないデータを探す
{
	int tmp = edit_now;

	int a;
	if(pre)a=-1;
	else   a= 1;

	tmp+=a;

	while(tmp < GCDMAX_RECTANGLES && tmp>=0){
		if(!GCDIsEmptyCell2(ed_c[tmp],ed_h[tmp])){//見つかった
			edit_now=tmp;
			return;
		}
		else tmp+=a;
	}
	MessageBox(ghwnd,"ありません","FindFind()",MB_OK);
}

void RadioChanged()
{
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO1), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio=0;//移動
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO2), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio=1;//判定矩形
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO3), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio=2;//重心設定
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO4), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio=3;//回転
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO5), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio=4;//拡大縮小x
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO6), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio=5;//拡大縮小y

	if(radio!=1){ 
		EnableWindow(GetDlgItem(ghwnd,IDC_BUTTON_KASANARI0),FALSE);
		EnableWindow(GetDlgItem(ghwnd,IDC_BUTTON_KASANARI1),FALSE);
		EnableWindow(GetDlgItem(ghwnd,IDC_BUTTON_KASANARI2),FALSE);
		EnableWindow(GetDlgItem(ghwnd,IDC_BUTTON_KURAI0),FALSE);
		EnableWindow(GetDlgItem(ghwnd,IDC_BUTTON_KURAI1),FALSE);
		EnableWindow(GetDlgItem(ghwnd,IDC_BUTTON_KURAI2),FALSE);
		EnableWindow(GetDlgItem(ghwnd,IDC_BUTTON_KOUGEKI0),FALSE);
		EnableWindow(GetDlgItem(ghwnd,IDC_BUTTON_KOUGEKI1),FALSE);
		EnableWindow(GetDlgItem(ghwnd,IDC_BUTTON_KOUGEKI2),FALSE);
	}
	else{
		EnableWindow(GetDlgItem(ghwnd,IDC_BUTTON_KASANARI0),TRUE);
		EnableWindow(GetDlgItem(ghwnd,IDC_BUTTON_KASANARI1),TRUE);
		EnableWindow(GetDlgItem(ghwnd,IDC_BUTTON_KASANARI2),TRUE);
		EnableWindow(GetDlgItem(ghwnd,IDC_BUTTON_KURAI0),TRUE);
		EnableWindow(GetDlgItem(ghwnd,IDC_BUTTON_KURAI1),TRUE);
		EnableWindow(GetDlgItem(ghwnd,IDC_BUTTON_KURAI2),TRUE);
		EnableWindow(GetDlgItem(ghwnd,IDC_BUTTON_KOUGEKI0),TRUE);
		EnableWindow(GetDlgItem(ghwnd,IDC_BUTTON_KOUGEKI1),TRUE);
		EnableWindow(GetDlgItem(ghwnd,IDC_BUTTON_KOUGEKI2),TRUE);
	}

	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIOSEL1), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio2=0;
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIOSEL2), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio2=1;
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIOSEL3), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio2=2;
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIOSEL4), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio2=3;
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIOSEL5), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio2=4;
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIOSEL6), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio2=5;
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIOSEL7), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio2=6;
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIOSEL8), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radio2=7;

	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO_VIEW1), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radioview=0;//25%
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO_VIEW2), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radioview=1;//50%
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO_VIEW3), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radioview=2;//75%
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO_VIEW4), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radioview=3;//100%
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO_VIEW5), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radioview=4;//200%
	if(SendMessage(GetDlgItem(ghwnd,IDC_RADIO_VIEW6), BM_GETCHECK, 0L, 0L)==BST_CHECKED)radioview=5;//400%

	DlgRedraw();
	CldReset();
}

void EditChanged()
{
	char str[256];
	itoa(edit_now,str,10);
	SetDlgItemText(ghwnd,IDC_EDIT1,str);

	Edit2Changed();

	DlgRedraw();
}

void EditChanged2()
{
	char str[256];
	GetDlgItemText(ghwnd,IDC_EDIT1,str,256);

	edit_now = atoi(str);
	if(edit_now < 0)edit_now=0;
	if(edit_now > GCDMAX_RECTANGLES-1)edit_now=GCDMAX_RECTANGLES;

	DlgRedraw();
}

void Edit2Changed()
{
	char str[256];
	memcpy(str,ed_hed.name[edit_now],sizeof(char)*64);
	SetDlgItemText(ghwnd,IDC_EDIT2,str);
}

void Edit2Changed2()
{
	char str[256];
	GetDlgItemText(ghwnd,IDC_EDIT2,str,256);
	sprintf(ed_hed.name[edit_now],str);
}

BOOL FindRect(BOOL fwd,int num)
{
	int tmp = ed_c[edit_now].cell[radio2].cdr;
	
	int a;
	if(fwd)a=1;
	else a=-1;

	int found=0;

	tmp+=a;

	while(tmp < GCDMAX_RECTANGLES && tmp>=0){
		if(tmp==0){
			ed_c[edit_now].cell[radio2].cdr=0;
			return(TRUE);
		}
		if(!GCDIsEmptyRect(ed_r[tmp])){//見つかった
			found++;
			if(num==found){
				ed_c[edit_now].cell[radio2].cdr=tmp;
				return(TRUE);
			}
		}
		tmp+=a;
	}
	return(FALSE);
}

void SetKasanari(DWORD n)
{
	RECT r = GetRECTNow();
	double mag=1.0;
	switch(radioview){
	case 0:mag=4;break;//25%
	case 1:mag=2;break;//50%
	case 2:mag=1.5;break;//75%
	case 4:mag=0.5;break;//200%
	case 5:mag=0.25;break;//400%
	default:mag=1.0;
	}
	r.top = (int)(r.top*mag);
	r.bottom = (int)(r.bottom*mag);
	r.left = (int)(r.left*mag);
	r.right = (int)(r.right*mag);

	ed_h[edit_now].kas[n]=r;
	DlgRedraw();
}
void SetKurai(DWORD n)
{
	RECT r = GetRECTNow();
	
	double mag=1.0;
	switch(radioview){
	case 0:mag=4;break;//25%
	case 1:mag=2;break;//50%
	case 2:mag=1.5;break;//75%
	case 4:mag=0.5;break;//200%
	case 5:mag=0.25;break;//400%
	default:mag=1.0;
	}
	r.top = (int)(r.top*mag);
	r.bottom = (int)(r.bottom*mag);
	r.left = (int)(r.left*mag);
	r.right = (int)(r.right*mag);

	ed_h[edit_now].kurai[n] = r;
	DlgRedraw();
}
void SetKougeki(DWORD n)
{
	RECT r=GetRECTNow();

	double mag=1.0;
	switch(radioview){
	case 0:mag=4;break;//25%
	case 1:mag=2;break;//50%
	case 2:mag=1.5;break;//75%
	case 4:mag=0.5;break;//200%
	case 5:mag=0.25;break;//400%
	default:mag=1.0;
	}
	r.top = (int)(r.top*mag);
	r.bottom = (int)(r.bottom*mag);
	r.left = (int)(r.left*mag);
	r.right = (int)(r.right*mag);

	ed_h[edit_now].attack[n]=r;
	DlgRedraw();
}

void CheckChanged()
{
	//x方向反転
	if(SendMessage(GetDlgItem(ghwnd,IDC_CHECK1), BM_GETCHECK, 0L, 0L)==BST_CHECKED)
		ed_c[edit_now].cell[radio2].flag |= 0x00000001;
	else
		ed_c[edit_now].cell[radio2].flag &= 0xFFFFFFFE;
	//y方向反転
	if(SendMessage(GetDlgItem(ghwnd,IDC_CHECK2), BM_GETCHECK, 0L, 0L)==BST_CHECKED)
		ed_c[edit_now].cell[radio2].flag |= 0x00000002;
	else
		ed_c[edit_now].cell[radio2].flag &= 0xFFFFFFFD;

	//test mirror
	//x
	if(SendMessage(GetDlgItem(ghwnd,IDC_TESTCHECK1), BM_GETCHECK, 0L, 0L)==BST_CHECKED)
		testmirrorx=TRUE;
	else
		testmirrorx=FALSE;
	//y
	if(SendMessage(GetDlgItem(ghwnd,IDC_TESTCHECK2), BM_GETCHECK, 0L, 0L)==BST_CHECKED)
		testmirrory=TRUE;
	else
		testmirrory=FALSE;

	//link
	if(SendMessage(GetDlgItem(ghwnd,IDC_CHECK_LINK), BM_GETCHECK, 0L, 0L)==BST_CHECKED)
		ed_c[edit_now].flag |= GCDCELL2_LINK;
	else
		ed_c[edit_now].flag &= (!GCDCELL2_LINK);

	//リンクセル飛ばし
	if(SendMessage(GetDlgItem(ghwnd,IDC_TOBASILINK), BM_GETCHECK, 0L, 0L)==BST_CHECKED)
		tobasilink=TRUE;
	else
		tobasilink=FALSE;

	DlgRedraw();
}

// draw dialog box--------------------------------------------------------------
void DlgDraw(HWND hwndDlg)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwndDlg, &ps);

	char msgout[256];
	sprintf(msgout,"%d",ed_c[edit_now].cell[radio2].cdr);

	RECT r;
	r.top=380;r.bottom=r.top+50;
	r.left=190;r.right=r.left+50;

	int oldmode = SetROP2(hdc,TRANSPARENT);
	DrawText(hdc,msgout,-1,&r,DT_LEFT);
	SetROP2(hdc,oldmode);

	EndPaint(hwndDlg, &ps);
}

void DlgRedraw()
{
	RECT r;
	r.top=380;r.bottom=r.top+50;
	r.left=190;r.right=r.left+50;
	InvalidateRect(ghwnd,&r,TRUE);
	CldRedraw();

	//x方向反転
	if(ed_c[edit_now].cell[radio2].flag & 0x00000001)
		SendMessage(GetDlgItem(ghwnd,IDC_CHECK1), BM_SETCHECK, BST_CHECKED, 0L);
	else
		SendMessage(GetDlgItem(ghwnd,IDC_CHECK1), BM_SETCHECK, BST_UNCHECKED, 0L);
	//ｙ方向反転
	if(ed_c[edit_now].cell[radio2].flag & 0x00000002)
		SendMessage(GetDlgItem(ghwnd,IDC_CHECK2), BM_SETCHECK, BST_CHECKED, 0L);
	else
		SendMessage(GetDlgItem(ghwnd,IDC_CHECK2), BM_SETCHECK, BST_UNCHECKED, 0L);
	//test mirror-x
	if(testmirrorx)
		SendMessage(GetDlgItem(ghwnd,IDC_TESTCHECK1), BM_SETCHECK, BST_CHECKED, 0L);
	else
		SendMessage(GetDlgItem(ghwnd,IDC_TESTCHECK1), BM_SETCHECK, BST_UNCHECKED, 0L);
	if(testmirrory)
		SendMessage(GetDlgItem(ghwnd,IDC_TESTCHECK2), BM_SETCHECK, BST_CHECKED, 0L);
	else
		SendMessage(GetDlgItem(ghwnd,IDC_TESTCHECK2), BM_SETCHECK, BST_UNCHECKED, 0L);
	//link
	if(ed_c[edit_now].flag & GCDCELL2_LINK)
		SendMessage(GetDlgItem(ghwnd,IDC_CHECK_LINK), BM_SETCHECK, BST_CHECKED, 0L);
	else
		SendMessage(GetDlgItem(ghwnd,IDC_CHECK_LINK), BM_SETCHECK, BST_UNCHECKED, 0L);
}

int SetWinCenter(HWND hWnd)
{
    HWND hDeskWnd;
    RECT deskrc, rc;
    int x, y;
    
    hDeskWnd = GetDesktopWindow();
    GetWindowRect(hDeskWnd, (LPRECT)&deskrc);
    GetWindowRect(hWnd, (LPRECT)&rc);
    x = (deskrc.right - (rc.right-rc.left)) / 2;
    y = (deskrc.bottom - (rc.bottom-rc.top)) / 2;
    SetWindowPos(hWnd, HWND_TOP, x, y, (rc.right-rc.left), (rc.bottom-rc.top),SWP_SHOWWINDOW); 
    return 0;
}
