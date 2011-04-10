#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <ddraw.h>
#include "define.h"

#include "define_gcd.h"
#include "function.h"

//global val.---------------------------------------------------------------------------
DWORD bmp_loaded[GCDMAX_IMAGES]={1,1,1,1,1,1,1,1,1,1,1,1};
BMPINFO256 bmp_info[GCDMAX_IMAGES];
BMPINFO256 bmp_info2[GCDMAX_IMAGES];
PBYTE bmp_bits[GCDMAX_IMAGES]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

// global val.=================================================================
HWND ghwnd;//���C���E�C���h�E�̃n���h��
char datfilepath[256];

//******************************************************************************
// �`��֘A
//******************************************************************************

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

void DrawPos(HDC hdc,int x,int y,BOOL not)
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

//******************************************************************************
//�r�b�g�}�b�v�ǂݍ��݊֘A
//******************************************************************************

//bmp_loaded[] �̏��
//0:����ɓǂݍ��܂�Ă���
//1:�܂��ǂݍ��܂�Ă��Ȃ�
//2:�t�@�C�������݂��Ȃ����A�܂��͊J���Ȃ�����
//3:�t�@�C���̓ǂݏo�����Ɏ��s����
//4:�Ƃ������A�r�b�g�}�b�v�ł͂Ȃ�����
//5:�t�@�C���w�b�_�ɉ��炩�̕s�s����������(256�łȂ��Ƃ�)


BOOL ChangeBmp2(DWORD n)
{
	if(bmp_loaded[n] == 0){
		if(bmp_bits[n] != NULL){
			free(bmp_bits[n]);
			bmp_bits[n]=NULL;
		}
	}

	char filename[256];
	sprintf(filename,"%s\\image%d.bmp",datfilepath,n+1);

	HANDLE hFile = CreateFile(filename,
		GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		bmp_loaded[n] = 2;
//		MessageBox(ghwnd,"̧�ق��J���Ȃ�����","ChangeBmp()",MB_OK);
		return(FALSE);
	}

	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//�O�̂��߃t�@�C���̐擪�Ɉړ�

	BOOL err=FALSE;

	//̧�كw�b�_�̓ǂݏo��
	BITMAPFILEHEADER fileheader;
	DWORD br;
	DWORD ret=ReadFile(hFile,&fileheader,sizeof(BITMAPFILEHEADER),&br,NULL);
	if(!ret || br!=sizeof(BITMAPFILEHEADER)){
		bmp_loaded[n] = 3;
//		MessageBox(ghwnd,"�t�@�C���̓ǂݍ��݂Ɏ��s(1)\n","ChangeBmp()",MB_OK);
		err=TRUE;
	}
	if(fileheader.bfType != 0x4d42){//"BM"
		bmp_loaded[n] = 4;
//		MessageBox(ghwnd,"�[�����̃t�@�C���̓r�b�g�}�b�v�ł͂Ȃ�\n","ChangeBmp()",MB_OK);
		err=TRUE;
	}
	if(err){
		CloseHandle(hFile);
		return(FALSE);
	}

	//BITMAPINFOHEADER�̓ǂݍ���
	ret=ReadFile(hFile,&bmp_info[n].hed,sizeof(BITMAPINFOHEADER),&br,NULL);
	if(!ret || br!=sizeof(BITMAPINFOHEADER)){
//		MessageBox(ghwnd,"�t�@�C���̓ǂݍ��݂Ɏ��s(2)\n","ChangeBmp()",MB_OK);
		bmp_loaded[n] = 3;
		err=TRUE;
	}
	if(bmp_info[n].hed.biSize != sizeof(BITMAPINFOHEADER)){
//		MessageBox(ghwnd,"BITMAPINFOHEADER�̃T�C�Y������Ȃ�\n","ChangeBmp()",MB_OK);
		bmp_loaded[n] = 5;
		err=TRUE;
	}
	if(bmp_info[n].hed.biBitCount != 8){
		bmp_loaded[n]=2;
//		MessageBox(ghwnd,"�[�����̃t�@�C����256�ł͂Ȃ�\n","ChangeBmp()",MB_OK);
		bmp_loaded[n] = 5;
		err=TRUE;
	}
	if(bmp_info[n].hed.biCompression != BI_RGB){
//		MessageBox(ghwnd,"���k���������Ă���炵��\n","ChangeBmp()",MB_OK);
		bmp_loaded[n] = 5;
		err=TRUE;
	}
	if(bmp_info[n].hed.biClrUsed == 0){
		bmp_info[n].hed.biClrUsed=256;
	}
	if(err){
		CloseHandle(hFile);
		return(FALSE);
	}

	//�p���b�g�̓ǂݍ���
	ret=ReadFile(hFile,bmp_info[n].pal,sizeof(RGBQUAD)*bmp_info[n].hed.biClrUsed,&br,NULL);
	if(!ret || br!=sizeof(RGBQUAD)*bmp_info[n].hed.biClrUsed){
//		MessageBox(ghwnd,"�t�@�C���̓ǂݍ��݂Ɏ��s(3)\n","ChangeBmp()",MB_OK);
		bmp_loaded[n] = 3;
		CloseHandle(hFile);
		return(FALSE);
	}
	bmp_info2[n].hed = bmp_info[n].hed;
	for(DWORD i=0;i<255;i++){
		bmp_info2[n].pal[i].rgbBlue =0;
		bmp_info2[n].pal[i].rgbRed  =0;
		bmp_info2[n].pal[i].rgbGreen=0;
		bmp_info2[n].pal[i].rgbReserved =0;
	}
	bmp_info2[n].pal[255].rgbBlue =0xFF;
	bmp_info2[n].pal[255].rgbRed  =0xFF;
	bmp_info2[n].pal[255].rgbGreen=0xFF;
	bmp_info2[n].pal[255].rgbReserved =0;

	bmp_info[n].pal[255].rgbBlue =0;
	bmp_info[n].pal[255].rgbRed  =0;
	bmp_info[n].pal[255].rgbGreen=0;
	bmp_info[n].pal[255].rgbReserved =0;

	//�r�b�g�}�b�v�r�b�g�̃T�C�Y���v�Z����(�r�b�g�}�b�v�̕���4�̔{���Ŋi�[����Ă���炵��)
	DWORD sizeimage = bmp_info[n].hed.biWidth;
	if(bmp_info[n].hed.biWidth%4 != 0)sizeimage +=  ( 4 - bmp_info[n].hed.biWidth%4 );
	sizeimage *= bmp_info[n].hed.biHeight;
	//���������m�ۂ��ăr�b�g�}�b�v�r�b�g��ǂݍ���
	bmp_bits[n] = (PBYTE)malloc(sizeimage);
	ret=ReadFile(hFile,bmp_bits[n],sizeimage,&br,NULL);
	CloseHandle(hFile);

	bmp_loaded[n]=0;

	return(TRUE);
}

BOOL ChangeBmp(DWORD n)
{
	ChangeBmp2(n);
	return(TRUE);
}

BOOL BitmapReflesh()
{
	for(int i=0;i<GCDMAX_IMAGES;i++){
		ChangeBmp(i);
	}
	return(TRUE);
}

