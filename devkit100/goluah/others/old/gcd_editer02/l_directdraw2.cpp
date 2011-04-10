
//*******************************************************************************
//  l_directdraw2.cpp �r�b�g�}�b�v�̃��[�h�֘A
//*******************************************************************************

#include <windows.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <stdio.h>
#include <string.h>
#include "define.h"

//#include "define_data.h"
//#include "define_system.h"
#include "l_directdraw.h"

extern int g_DISPLAYWIDTH;
extern int g_DISPLAYHEIGHT;

//BOOL outtest=FALSE;

extern CONFIG2 config2;

// �r�b�g�}�b�v��MYSURFACE�Ƃ��ă��[�h *******************************************
MYSURFACE* CDirectDraw::CreateSurfaceFrom256BMP(char *filename,char *palname,BOOL dmy)
{
	BOOL bret;
	DWORD i,j;

	//���g�p�̂��̂�T��
	int e=-1;
	for(i=0;i<MAXNUMGOLUAHTEXTURES;i++){
		if(!ms[i].valid){
			e=i;
			i=MAXNUMGOLUAHTEXTURES;
		}
	}
	if(e<0){
		OutputDebugString("warning:�e�N�X�`������MAX�𒴂��Ă��܂�\n");
		return(NULL);
	}

	//�r�b�g�}�b�v�̃r�b�g��ǂݍ���
	MYPALLET *bmpbits;
	DWORD bmpwidth,bmpheight;
	bret = Load256Bitmapbits(&bmpbits,&bmpwidth,&bmpheight,filename,palname);
	if(!bret){
		OutputDebugString("Load256Bitmapbits�Ɏ��s\n");
		return(NULL);
	}
	ms[e].wg = (float)bmpwidth;
	ms[e].hg = (float)bmpheight;

	//�r�b�g�}�b�v�̕����̎d��������
	bret = AssignTextureDiv(&ms[e],bmpwidth,bmpheight);
	if(!bret){
		OutputDebugString("AssignTextureDiv�Ɏ��s\n");
		return(NULL);
	}

	//�w��T�C�Y�̃e�N�X�`�����쐬
	LPVOID pointer;
	pointer = malloc( sizeof(LPDIRECT3DTEXTURE8)*(ms[e].xsufnum*ms[e].ysufnum+1));
	ZeroMemory(pointer,sizeof(LPDIRECT3DTEXTURE8)*(ms[e].xsufnum*ms[e].ysufnum+1));
	ms[e].pTex = (LPDIRECT3DTEXTURE8*)pointer;//�|�C���^���m�ۂ���̈���m��
	for(i=0;i<ms[e].ysufnum;i++){
		for(j=0;j<ms[e].xsufnum;j++){
			d3ddev->CreateTexture(
				ms[e].xsufsize[j],
				ms[e].ysufsize[i],
				1,//mipmap level
				0,//D3DUSAGE_RENDERTARGET,
				texformat,//D3DFMT_A8R8G8B8,//D3DFMT_R5G6B5,//D3DFMT_R5G6B5,
				D3DPOOL_MANAGED,
				&(ms[e].pTex[i*ms[e].xsufnum + j]));
		}
	}

	DWORD dameyox = bmpwidth%MINIMUM_TEXSIZE;
	DWORD dameyoy = bmpheight%MINIMUM_TEXSIZE;

//	char msg[256];

	//�e�N�X�`���Ƀr�b�g�}�b�v���R�s�[
//	OutputDebugString("���e�N�X�`���Ƀr�b�g�}�b�v���R�s�[\n");
	DWORD damex,damey;
	for(i=0;i<ms[e].ysufnum;i++){//okasi?
		for(j=0;j<ms[e].xsufnum ;j++){
			//�R�s�[���s
//			sprintf(msg,"%d : %d,%d - offset%d/%d ...",i*ms[e].xsufnum + j,i,j,ofx,ofy);
//			OutputDebugString(msg);
			if(i==ms[e].ysufnum-1){damey=dameyoy;}
			else damey=0;
			if(j==ms[e].xsufnum-1){damex=dameyox;}
			else damex=0;
			CopyBB2TS(bmpbits,bmpwidth,
				ms[e].xsufindx[j],ms[e].ysufindx[i],
				ms[e].pTex[ (i*ms[e].xsufnum+j) ],damex,damey);
		}
	}

	//��n��
	free(bmpbits);

	//����
	ms[e].valid=TRUE;
/*
#ifdef _DEBUG
	//test
	OutputDebugString("��CreateSurfaceFromBMP - complete\n");
	char msg[256];
	int numemptytex=0;
	for(i=0;i<MAXNUMGOLUAHTEXTURES;i++)
		if(!ms[i].valid)numemptytex++;
	sprintf(msg,"(ms�c��%d)\n",numemptytex);
	OutputDebugString(msg);
	sprintf(msg,"�e�N�X�`�����F%dx%d (x/ysufnum)\n",ms[e].xsufnum,ms[e].ysufnum);
	OutputDebugString(msg);
	sprintf(msg,"index-x : ");//indexx
	OutputDebugString(msg);
	sprintf(msg,"");
	for(i=0;i<MYSUF_MAXNUMTEX;i++){
		sprintf(&msg[strlen(msg)]," %d,",ms[e].xsufindx[i]);
	}
	sprintf(&msg[strlen(msg)],"\n");
	OutputDebugString(msg);
	sprintf(msg,"index-y : ");//indexy
	OutputDebugString(msg);
	sprintf(msg,"");
	for(i=0;i<MYSUF_MAXNUMTEX;i++){
		sprintf(&msg[strlen(msg)]," %d,",ms[e].ysufindx[i]);
	}
	sprintf(&msg[strlen(msg)],"\n");
	OutputDebugString(msg);
	sprintf(msg,"size-x : ");//size-x
	OutputDebugString(msg);
	sprintf(msg,"");
	for(i=0;i<MYSUF_MAXNUMTEX;i++){
		sprintf(&msg[strlen(msg)]," %d,",ms[e].xsufsize[i]);
	}
	sprintf(&msg[strlen(msg)],"\n");
	OutputDebugString(msg);
	sprintf(msg,"size-y : ");//size-y
	OutputDebugString(msg);
	sprintf(msg,"");
	for(i=0;i<MYSUF_MAXNUMTEX;i++){
		sprintf(&msg[strlen(msg)]," %d,",ms[e].ysufsize[i]);
	}
	sprintf(&msg[strlen(msg)],"\n");
	OutputDebugString(msg);
	
	sprintf(msg,"textures");
	OutputDebugString(msg);
	sprintf(msg,"");
	for(i=0;i<ms[e].xsufnum*ms[e].ysufnum;i++){
		if(i%ms[e].xsufnum==0)sprintf(&msg[strlen(msg)],"\n");
		if(ms[e].pTex[i]!=NULL)sprintf(&msg[strlen(msg)],"��");
		else sprintf(&msg[strlen(msg)],"��");
	}
	sprintf(&msg[strlen(msg)],"\n");
	OutputDebugString(msg);

#endif //��debug//
*/
	return(&ms[e]);
}

//�e�N�X�`���̃T�[�t�F�[�X�ɏ�������*******************************************************************************
BOOL CDirectDraw::CopyBB2TS(MYPALLET *pbb,DWORD bbpitch,DWORD offset_x,DWORD offset_y,LPDIRECT3DTEXTURE8 ptex,DWORD damex,DWORD damey)
{
	if(pbb==NULL)return(FALSE);
	if(ptex==NULL)return(FALSE);

	LPDIRECT3DSURFACE8 psuf=NULL;
	if(D3D_OK != ptex->GetSurfaceLevel(0,&psuf)){
		OutputDebugString("CopyBB2TS / GetSurfaceLevel�Ɏ��s\n");
		return(FALSE);
	}

	//���ƍ����A�t�H�[�}�b�g���擾
	D3DFORMAT fmt;
	DWORD sw,sh;
	D3DSURFACE_DESC dsc;
	if(D3D_OK != psuf->GetDesc(&dsc)){
		OutputDebugString("CopyBB2TS / GetDesc�Ɏ��s\n");
		RELEASE(psuf);
		return(FALSE);
	}
	fmt = dsc.Format;
	sw = dsc.Width;
	sh = dsc.Height;

	if(damex!=0)sw=damex;
	if(damey!=0)sh=damey;

/*	char msg[256];
	sprintf(msg,"surface %dx%d ",sw,sh);
	OutputDebugString(msg);*/

	//�T�[�t�F�C�X�̃��b�N
	D3DLOCKED_RECT lr;
	if(D3D_OK != psuf->LockRect(&lr,NULL,0)){
		OutputDebugString("CopyBB2TS / LockRect�Ɏ��s\n");
		RELEASE(psuf);
		return(FALSE);
	}

	//�R�s�[
	DWORD i,j;
	PBYTE pline;
	int onepixsize;
	for(j=0;j<sh;j++){
		pline =  (PBYTE)lr.pBits + lr.Pitch*j ;
//		sprintf(msg,"line %d\n",j);
//		if(outtest && j>96 )OutputDebugString(msg);
		for(i=0;i<sw;i++){
			switch(fmt){
			case D3DFMT_R5G6B5://����͑����g���Ȃ�
				onepixsize = CopyOne_R5G6B5(pbb[(j+offset_y)*bbpitch+i+offset_x],pline);
				break;
			case D3DFMT_A8R8G8B8:
				i=i;
				j=j;
				onepixsize = CopyOne_A8R8G8B8(pbb[(j+offset_y)*bbpitch+i+offset_x],pline);
				break;
			case D3DFMT_A1R5G5B5:
				onepixsize = CopyOne_A1R5G5B5(pbb[(j+offset_y)*bbpitch+i+offset_x],pline);
				break;
			case D3DFMT_A4R4G4B4:
				onepixsize = CopyOne_A4R4G4B4(pbb[(j+offset_y)*bbpitch+i+offset_x],pline);
				break;
			case D3DFMT_A8R3G3B2:
				onepixsize = CopyOne_A8R3G3B2(pbb[(j+offset_y)*bbpitch+i+offset_x],pline);
				break;
			default:
				OutputDebugString("CopyBB2TS / ���̃t�H�[�}�b�g�̓R�s�[�ł��ɂႢ\n");
				psuf->UnlockRect();
				RELEASE(psuf);
				return(FALSE);
			}
			pline += onepixsize;
//			sprintf(msg," %d,",i);
//			OutputDebugString(msg);
		}
	}
//	sprintf(msg,"... %d/%d\n",i,j);
//	OutputDebugString(msg);

	psuf->UnlockRect();
	RELEASE(psuf);

	return(TRUE);
}

// 1 �s�N�Z���̏������ݏ���********************************************************************************
DWORD CDirectDraw::CopyOne_R5G6B5(MYPALLET src,PBYTE dst)
{
	WORD *pdst;
	pdst = (PWORD)dst;

	WORD r,g,b;
	r = src.red;
	r *= 2*2*2*2*2*2 *2*2;
	r &= 0xF800;
	
	g = src.green;
	g *= 2*2*2;
	g &= 0x07E0;

	b = src.blue;
	b /= 2*2*2;
	b &= 0x001F;

	WORD col = r | g | b ;
	*pdst = col;

	return(2);
}

DWORD CDirectDraw::CopyOne_A1R5G5B5(MYPALLET src,PBYTE dst)
{
	WORD *pdst;
	pdst = (PWORD)dst;

	WORD a,r,g,b;

	if(src.alpha!=0)a=0x8000;
	else    a=0x0000;

	r = src.red;
	r *= 2*2*2*2*2*2 *2;
	r &= 0x7C00;
	
	g = src.green;
	g *= 2*2;
	g &= 0x03E0;

	b = src.blue;
	b /= 2*2*2;
	b &= 0x001F;

	WORD col = a | r | g | b ;
	*pdst = col;

	return(2);
}

DWORD CDirectDraw::CopyOne_A4R4G4B4(MYPALLET src,PBYTE dst)
{
	WORD *pdst;
	pdst = (PWORD)dst;

	WORD a,r,g,b;

	a=src.alpha;
	a *= 2*2*2*2 *2*2*2*2;
	a &= 0xF000;

	r = src.red;
	r *= 2*2*2*2;
	r &= 0x0F00;
	
	g = src.green;
	g &= 0x00F0;

	b = src.blue;
	b /= 2*2*2*2;
	b &= 0x000F;

	WORD col = a | r | g | b ;
	*pdst = col;

	return(2);
}

DWORD CDirectDraw::CopyOne_A8R3G3B2(MYPALLET src,PBYTE dst)
{
	WORD *pdst;
	pdst = (PWORD)dst;

	WORD a,r,g,b;

	a=src.alpha;
	a *= 2*2*2*2 *2*2*2*2;

	r = src.red;
	r &= 0x00080;
	
	g = src.green;
	g /= 2*2*2;
	g &= 0x001C;

	b = src.blue;
	b /= 2*2*2*2 *2*2;
	b &= 0x0003;

	WORD col = a | r | g | b ;
	*pdst = col;

	return(2);
}

DWORD CDirectDraw::CopyOne_A8R8G8B8(MYPALLET src,PBYTE dst)
{
	MYPALLET *pdst;
	pdst = (MYPALLET*)dst;

	*pdst = src;

	return(4);
}

//�������@����*********************************************************************************
BOOL CDirectDraw::AssignTextureDiv(MYSURFACE *pmsf,DWORD width,DWORD height)
{
	DWORD i;

	DWORD ntx,nty,numsufx=0,numsufy=0;
	BOOL bloop;
	
	//���R����
	bloop=TRUE;
	DWORD bmpwidthr = width;
	DWORD asstexsize = maxtexturewidth;
	
	while(bloop){
		ntx = bmpwidthr/asstexsize;
		bmpwidthr = bmpwidthr%asstexsize;
		for(i=0;i<ntx;i++){
			pmsf->xsufsize[numsufx]=asstexsize;
			numsufx++;
			if(numsufx==MYSUF_MAXNUMTEX)return(FALSE);
		}
		if(bmpwidthr==0)bloop=FALSE;//��������I������炻��ŏI��
		else if(asstexsize <= MINIMUM_TEXSIZE){//�Ō�ׂ͍�������ȗ���
			pmsf->xsufsize[numsufx]=asstexsize;
			numsufx++;
			bloop=FALSE;
		}
		asstexsize /= 2;
	}
	pmsf->xsufsize[numsufx]=0;
	pmsf->xsufnum = numsufx;
	pmsf->xsufindx[0]=0;
	for(i=1;i<pmsf->xsufnum;i++){
		pmsf->xsufindx[i] = pmsf->xsufindx[i-1] + pmsf->xsufsize[i-1];
	}

	//�^�e����
	bloop=TRUE;
	DWORD bmpheightr = height;
	asstexsize = maxtextureheight;

	while(bloop){
		nty = bmpheightr/asstexsize;
		bmpheightr= bmpheightr%asstexsize;
		for(i=0;i<nty;i++){
			pmsf->ysufsize[numsufy]=asstexsize;
			numsufy++;
			if(numsufy==MYSUF_MAXNUMTEX)return(FALSE);
		}
		if(bmpheightr==0)bloop=FALSE;//��������I������炻��ŏI��
		else if(asstexsize <= MINIMUM_TEXSIZE){//�Ō�ׂ͍�������ȗ���
			pmsf->ysufsize[numsufy]=MINIMUM_TEXSIZE;
			numsufy++;
			bloop=FALSE;
		}
		asstexsize /= 2;
	}
	pmsf->ysufsize[numsufy]=0;
	pmsf->ysufnum = numsufy;
	pmsf->ysufindx[0]=0;
	for(i=1;i<pmsf->ysufnum;i++){
		pmsf->ysufindx[i] = pmsf->ysufindx[i-1] + pmsf->ysufsize[i-1];
	}

	//test
	/*
#ifdef _DEBUG
	DWORD totalx=0,totaly=0;
	char msg[256];
	sprintf(msg,"��AssignTextureDiv( %d x %d - %d x %d )\n",width,height,pmsf->xsufnum,pmsf->ysufnum);
	OutputDebugString(msg);
	OutputDebugString("x : ");
	for(i=0;i<MYSUF_MAXNUMTEX;i++){
		totalx+=pmsf->xsufsize[i];
		sprintf(msg," %d-%d(%d),",pmsf->xsufindx[i],pmsf->xsufsize[i],totalx);
		OutputDebugString(msg);
		if(pmsf->xsufsize[i]==0)i=MYSUF_MAXNUMTEX;
	}
	OutputDebugString("\n");
	OutputDebugString("y : ");
	for(i=0;i<MYSUF_MAXNUMTEX;i++){
		totaly+=pmsf->ysufsize[i];
		sprintf(msg," %d-%d(%d),",pmsf->xsufindx[i],pmsf->ysufsize[i],totaly);
		OutputDebugString(msg);
		if(pmsf->ysufsize[i]==0)i=MYSUF_MAXNUMTEX;
	}
	OutputDebugString("\n");
#endif
*/
	return(TRUE);
}

//256�r�b�g�}�b�v���t�@�C������ǂݎ��*****************************************************************************
BOOL CDirectDraw::Load256Bitmapbits(MYPALLET **pbits,DWORD *width,DWORD *height,char *bmpfilename,char *palfilename)
{
	BOOL ret2;

	//�Ƃ肠�����p���b�g���m��
	MYPALLET pal[256];
	if(palfilename==NULL)palfilename=bmpfilename;
	ret2 = GetPallet(palfilename,pal);
	if(!ret2){
		OutputDebugString("��CDirectDraw::Load256Bitmapbits �p���b�g�擾�Ɏ��s\n");
		return(FALSE);
	}

	//�r�b�g�}�b�v�̃r�b�g���擾~
	//�r�b�g�}�b�v�����[�h
	HANDLE hFile;
	DWORD ret,br,err=FALSE;

	if(bmpfilename==NULL){
		OutputDebugString("CDirectDraw::Load256Bitmapbits : �t�@�C������NULL�Ƃ͂ǂ��������Ƃ��H\n");
	}

	//�t�@�C���̃I�[�v��
	hFile=CreateFile(bmpfilename,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		OutputDebugString("CDirectDraw::Load256Bitmapbits : ");
		OutputDebugString(bmpfilename);
		OutputDebugString("���J���܂���ł����B\n");
		return(NULL);
	}
	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//�O�̂��߃t�@�C���̐擪�Ɉړ�

	//̧�كw�b�_�̓ǂݏo��
	BITMAPFILEHEADER fileheader;
	ret=ReadFile(hFile,&fileheader,sizeof(BITMAPFILEHEADER),&br,NULL);
	if(!ret || br!=sizeof(BITMAPFILEHEADER)){
		OutputDebugString("CDirectDraw::Load256Bitmapbits : �t�@�C���̓ǂݍ��݂Ɏ��s(1)\n");
		err=TRUE;
	}
	if(fileheader.bfType != 0x4d42){//"BM"
		OutputDebugString("CDirectDraw::Load256Bitmapbits : �[�����̃t�@�C���̓r�b�g�}�b�v�ł͂Ȃ�\n");
		err=TRUE;
	}
	if(err){
		CloseHandle(hFile);
		return(NULL);
	}

	//BITMAPINFOHEADER�̓ǂݍ���
	BITMAPINFOHEADER infohed;
	ret=ReadFile(hFile,&infohed,sizeof(BITMAPINFOHEADER),&br,NULL);
	if(!ret || br!=sizeof(BITMAPINFOHEADER)){
		OutputDebugString("CDirectDraw::Load256Bitmapbits : �t�@�C���̓ǂݍ��݂Ɏ��s(2)\n");
		err=TRUE;
	}
	if(infohed.biSize != sizeof(BITMAPINFOHEADER)){
		OutputDebugString("CDirectDraw::Load256Bitmapbits : BITMAPINFOHEADER�̃T�C�Y������Ȃ�\n");
		err=TRUE;
	}
	if(infohed.biBitCount != 8){
		OutputDebugString("CDirectDraw::Load256Bitmapbits : �[�����̃t�@�C����256�ł͂Ȃ�\n");
		err=TRUE;
	}
	if(infohed.biCompression != BI_RGB){
		OutputDebugString("CDirectDraw::Load256Bitmapbits : ���k���������Ă���炵��\n");
		err=TRUE;
	}
	if(err){
		CloseHandle(hFile);
		return(NULL);
	}

	//�p���b�g�͓ǂ݂Ƃ΂�
	SetFilePointer(hFile,sizeof(RGBQUAD)*infohed.biClrUsed,NULL,FILE_CURRENT);

	if(infohed.biClrUsed == 0){
		infohed.biClrUsed=256;
	}

	//�r�b�g�}�b�v�r�b�g�̃T�C�Y���v�Z����(�r�b�g�}�b�v�̕���4�̔{���Ŋi�[����Ă���炵��)
	DWORD linesize = infohed.biWidth;
	if(infohed.biWidth%4 != 0)linesize +=  ( 4 - infohed.biWidth%4 );
	DWORD sizeimage = linesize * infohed.biHeight;
	//���������m�ۂ��ăr�b�g�}�b�v�r�b�g��ǂݍ���
	LPBYTE bits = (LPBYTE)(malloc(sizeimage));
	ret=ReadFile(hFile,bits,sizeimage,&br,NULL);
	CloseHandle(hFile);
	if(!ret || br!=sizeimage){
		OutputDebugString("CDirectDraw::Load256Bitmapbits : �t�@�C���̓ǂݍ��݂Ɏ��s(3)\n");
		free(bits);
		return(NULL);
	}

	//�r�b�g�}�b�v�̑傫���ɂ��킹�ĐV���Ƀ������̈���쐬����
	LPVOID pnewbits2;
	DWORD ishalf=1;
	if(config2.flag & CONFIG2_HALFMODE)ishalf=2;
	pnewbits2 = malloc(sizeof(MYPALLET)*infohed.biWidth*infohed.biHeight /ishalf);
	MYPALLET *retbit;
	retbit = (MYPALLET*)pnewbits2;

	//�����Ƀf�[�^���R�s�[
	LONG i,j;
	PBYTE plinenow;
	DWORD halfcopy=0;
	for(i=infohed.biHeight-1;i>=0;i--){
		plinenow = bits;
		plinenow += linesize*i;
		for(j=0;j<infohed.biWidth;j++){
			if(config2.flag & CONFIG2_HALFMODE){//���������R�s�[���Ȃ�
				if(i%2==0 && j%2==0){
					retbit[halfcopy] = pal[ plinenow[j] ];
					halfcopy++;
				}
			}
			else{
				retbit[(infohed.biHeight-1-i)*infohed.biWidth + j] = pal[ plinenow[j] ];
			}
		}
	}

	//����
	free(bits);

	if(config2.flag & CONFIG2_HALFMODE){
		infohed.biWidth/=2;
		infohed.biHeight/=2;
	}

	*pbits = retbit;
	*width = infohed.biWidth;
	*height = infohed.biHeight;
	return(TRUE);
}

//256�r�b�g�}�b�v�̃p���b�g�݂̂�ǂݍ���***************************************************************
BOOL CDirectDraw::GetPallet(char *filename,MYPALLET *pal)//256�������t�@�C������A�p���b�g���������o��
{
	HANDLE hFile;
	DWORD ret,br,err=FALSE;

	if(filename==NULL){
		OutputDebugString("CDirectDraw::GetPallet : �t�@�C������NULL�Ƃ͂ǂ��������Ƃ��H\n");
		return(FALSE);
	}
	if(pal==NULL){
		OutputDebugString("CDirectDraw::GetPallet : �p���b�g��NULL�Ƃ͂ǂ��������Ƃ��H\n");
		return(FALSE);
	}

	//�t�@�C���̃I�[�v��
	hFile=CreateFile(filename,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		OutputDebugString("CDirectDraw::GetPallet : ");
		OutputDebugString(filename);
		OutputDebugString("���J���܂���ł����B\n");
		return(NULL);
	}
	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//�O�̂��߃t�@�C���̐擪�Ɉړ�

	//̧�كw�b�_�̓ǂݏo��
	BITMAPFILEHEADER fileheader;
	ret=ReadFile(hFile,&fileheader,sizeof(BITMAPFILEHEADER),&br,NULL);
	if(!ret || br!=sizeof(BITMAPFILEHEADER)){
		OutputDebugString("CDirectDraw::GetPallet : �t�@�C���̓ǂݍ��݂Ɏ��s(1)\n");
		err=TRUE;
	}
	if(fileheader.bfType != 0x4d42){//"BM"
		OutputDebugString("CDirectDraw::GetPallet : �[�����̃t�@�C���̓r�b�g�}�b�v�ł͂Ȃ�\n");
		err=TRUE;
	}
	if(err){
		CloseHandle(hFile);
		return(FALSE);
	}

	//BITMAPINFOHEADER�̓ǂݍ���
	BITMAPINFOHEADER infohed;
	ret=ReadFile(hFile,&infohed,sizeof(BITMAPINFOHEADER),&br,NULL);
	if(!ret || br!=sizeof(BITMAPINFOHEADER)){
		OutputDebugString("CDirectDraw::GetPallet : �t�@�C���̓ǂݍ��݂Ɏ��s(2)\n");
		err=TRUE;
	}
	if(infohed.biSize != sizeof(BITMAPINFOHEADER)){
		OutputDebugString("CDirectDraw::GetPallet : BITMAPINFOHEADER�̃T�C�Y������Ȃ�\n");
		err=TRUE;
	}
	if(infohed.biBitCount != 8){
		OutputDebugString("CDirectDraw::GetPallet : �[�����̃t�@�C����256�ł͂Ȃ�\n");
		err=TRUE;
	}
	if(infohed.biCompression != BI_RGB){
		OutputDebugString("CDirectDraw::GetPallet : ���k���������Ă���炵��\n");
		err=TRUE;
	}
	if(infohed.biClrUsed == 0){
		infohed.biClrUsed=256;
	}
	if(err){
		CloseHandle(hFile);
		return(FALSE);
	}

	//���悢��p���b�g�̓ǂݍ���
	RGBQUAD dpal[256];
	ZeroMemory(dpal,sizeof(RGBQUAD)*256);
	ret=ReadFile(hFile,dpal,sizeof(RGBQUAD)*infohed.biClrUsed,&br,NULL);
	CloseHandle(hFile);
	if(!ret || br!=sizeof(RGBQUAD)*infohed.biClrUsed){
		OutputDebugString("CDirectDraw::GetPallet : �t�@�C���̓ǂݍ��݂Ɏ��s(3)\n");
		return(FALSE);
	}
	//255�Ԗڂ̐F�͋����I�ɍ��i���ߐF�j
	dpal[255].rgbBlue  =0;
	dpal[255].rgbGreen =0;
	dpal[255].rgbRed   =0;

	for(int i=0;i<256;i++){
		if(dpal[i].rgbRed==0 && dpal[i].rgbGreen==0 && dpal[i].rgbBlue==0)pal[i].alpha = 0;//����
		else pal[i].alpha = 0xFF;//�s����
		pal[i].red = dpal[i].rgbRed;
		pal[i].green = dpal[i].rgbGreen;
		pal[i].blue = dpal[i].rgbBlue;

/*		pal[i].alpha = 0;
		pal[i].red = 0;
		pal[i].green = 0xFF;
		pal[i].blue = 0;*/
	}

	return(TRUE);
}

//�J��********************************************************************

void CDirectDraw::RelSurface(MYSURFACE *s)
{
	if(s==NULL)return;

	UINT j,k;

	if(s->valid){
		s->valid=FALSE;
		for(j=0;j<s->ysufnum;j++){
			for(k=0;k<s->xsufnum;k++){
				RELEASE(s->pTex[k*(s->ysufnum) + j]);
			}
		}
	}
}
