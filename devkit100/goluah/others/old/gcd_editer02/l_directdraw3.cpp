
//*******************************************************************************
//  l_directdraw3.cpp �r�b�g�}�b�v�`��֘A
//*******************************************************************************

#include <windows.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <stdio.h>
#include <string.h>
#include "define.h"

#include "l_directdraw.h"
#include "define_gcd.h"

#ifdef ___GCDEDITER
extern int radio;
#endif

extern int g_DISPLAYWIDTH;
extern int g_DISPLAYHEIGHT;

extern CONFIG2 config2;

void CDirectDraw::CheckBlt(MYSURFACE *dds,int x,int y,RECT r,BOOL revx,BOOL revy,DWORD flag,float z,DWORD color)
{
	CheckBlt2(dds,x,y,r,1.0,1.0,revx,revy,flag,z,color);
}

void CDirectDraw::CheckBlt2(MYSURFACE *dds,int x,int y,RECT r,
			   double magx,double magy,BOOL revx,BOOL revy,DWORD flag,float z,DWORD color)
{
	if(dds==NULL){return;}

	//��`�𐳂����ݒ�
	int dmi;
	if(r.left > r.right){
		dmi = r.left;
		r.left = r.right;
		r.right = dmi;
	}
	if(r.top > r.bottom){
		dmi = r.top;
		r.top = r.bottom;
		r.bottom = dmi;
	}

	if(config2.flag & CONFIG2_HALFMODE){
		r.left/=2;
		r.right/=2;
		r.top/=2;
		r.bottom/=2;
		x/=2;
		y/=2;
	}

	DWORD i,j;	

	DWORD r_top,r_bottom,r_left,r_right;//warning���E�U�C
	r_top=r.top;
	r_bottom=r.bottom;
	r_left=r.left;
	r_right=r.right;

	//�S�Ẵe�N�X�`���Ɋւ��ĕ`�悷�邩�ǂ������ׂĕ`�悷��
	float vl,vr,vt,vb;//�e���_�̍��W
	float tumin,tumax,tvmin,tvmax;//u,v���W�͈̔�
	float ar = (float)g_DISPLAYWIDTH/(float)g_DISPLAYHEIGHT;//�A�X�y�N�g��
	float ar2 = 2.0f/(float)g_DISPLAYHEIGHT;
	float centerx = (float)g_DISPLAYWIDTH/2.0f;//x������ʒ��S
	float cut_left,cut_right,cut_top,cut_bottom;//�؂�Ă钷��(�s�N�Z���P��)
	float transx,transy;//�ꎞ�I�Ɏg�p
	MYVERTEX3D vrtxarr[4];//���_�z��
	D3DXMATRIX matw;//���[���h���W�ϊ��s��
	D3DXMATRIX tmpmat;//�e���|�����s��
	for(j=0;j<dds->ysufnum;j++){
		for(i=0;i<dds->xsufnum;i++){
			if(dds->pTex[j*dds->xsufnum+i] ==NULL );
//				OutputDebugString("CheckBlt:�e�N�X�`���`�F�b�N�Ɏ��s\n");//�e�N�X�`���`�F�b�N
			else if(dds->xsufindx[i]+dds->xsufsize[i] < r_left);
//				OutputDebugString("��");//�͈̓`�F�b�N
			else if(dds->xsufindx[i] > r_right);
//				OutputDebugString("��");//�͈̓`�F�b�N
			else if(dds->ysufindx[j]+dds->ysufsize[j] < r_top);
//				OutputDebugString("��");//�͈̓`�F�b�N
			else if(dds->ysufindx[j] > r_bottom);
//				OutputDebugString("��");//�͈̓`�F�b�N
			else{
				//(0) ���E�㉺�A���ꂼ��]�����Ȃ������v�Z
				if(dds->xsufindx[i] < r_left)cut_left=(float)(r_left-dds->xsufindx[i]);
				else cut_left=0;
				if(dds->xsufindx[i]+dds->xsufsize[i] > r_right)
					cut_right = (float)( dds->xsufindx[i]+dds->xsufsize[i] - r_right);
				else cut_right=0;

				if(dds->ysufindx[j] < r_top)cut_top = (float)( r_top - dds->ysufindx[j] );
				else cut_top=0;
				if(dds->ysufindx[j]+dds->ysufsize[j] > r_bottom)
					cut_bottom = (float)( dds->ysufindx[j]+dds->ysufsize[j] - r_bottom );
				else cut_bottom=0;

				//(1) u,v���W�̌v�Z
				tumin = cut_left / (float)dds->xsufsize[i];
				tumax = 1.0f - cut_right/(float)dds->xsufsize[i];
				tvmin = cut_top / (float)dds->ysufsize[j];
				tvmax = 1.0f - cut_bottom/(float)dds->ysufsize[j];

				//(2) �]�����W�̌v�Z(�ȍ~�����Q��)
				//���ƍ����������i�s�N�Z����float�ϊ��ύ��W�n�j
				//��
				vl=0;
				vr=dds->xsufsize[i] - (cut_left+cut_right);
				vr= vr*ar2;// - ar;
				//����
				vt=0;
				vb=dds->ysufsize[j] - (cut_top+cut_bottom);
				vb=vb*ar2;//-1.0f
				//(3) ���_�z��ɍ��W�l����
				//����
				vrtxarr[0].x = vl;
				vrtxarr[0].y = vt;
				vrtxarr[0].z = z;
				//����
				vrtxarr[1].x = vl;
				vrtxarr[1].y = vb;
				vrtxarr[1].z = z;
				//�E��
				vrtxarr[2].x = vr;
				vrtxarr[2].y = vt;
				vrtxarr[2].z = z;
				//�E��
				vrtxarr[3].x = vr;
				vrtxarr[3].y = vb;
				vrtxarr[3].z = z;

				//���_�F
				vrtxarr[0].color = color;
				vrtxarr[1].color = color;
				vrtxarr[2].color = color;
				vrtxarr[3].color = color;

				//(3.5) �e�N�X�`�����W�ݒ�
				vrtxarr[0].tu = tumin;
				vrtxarr[0].tv = tvmin;
				vrtxarr[1].tu = tumin;
				vrtxarr[1].tv = tvmax;
				vrtxarr[2].tu = tumax;
				vrtxarr[2].tv = tvmin;
				vrtxarr[3].tu = tumax;
				vrtxarr[3].tv = tvmax;

				//(4) ���[���h���W�ϊ��s��p��
				//�P�ʍs��
				D3DXMatrixIdentity(&matw);
				//��`���ϊ�(���W�̒P�ʂ�float�ϊ���̂���)
				if(r_left>dds->xsufindx[i])transx=0;
				else transx = (float)( dds->xsufindx[i] - r_left );
				if(r_top>dds->ysufindx[j])transy=0;
				else transy = (float)( dds->ysufindx[j] - r_top );
				D3DXMatrixTranslation(&tmpmat,transx*ar2,transy*ar2,0);
				matw *= tmpmat;
				//���]����
				if(revx){
					d3dxplane_x.d=((float)r_right-(float)r_left)*ar2/2.0f;
					D3DXMatrixReflect(&tmpmat,&d3dxplane_x);
					matw *= tmpmat;
				}
				if(revy){
					d3dxplane_y.d=((float)r_bottom-(float)r_top)*ar2/2.0f;
					D3DXMatrixReflect(&tmpmat,&d3dxplane_y);
					matw *= tmpmat;
				}
				//�g��k��
				D3DXMatrixScaling(&tmpmat,(float)magx,(float)magy,1.0f);
				matw *= tmpmat;
				//�ړ�(���W�̒P�ʂ�float�ϊ���̂��́j
				D3DXMatrixTranslation(&tmpmat,(float)x*ar2,(float)y*ar2,0);
				matw *= tmpmat;
				if(flag & CKBLT_YUREY){//�h��
					D3DXMatrixTranslation(&tmpmat,0,yurey*ar2,0);
					matw *= tmpmat;
				}
				//�ݒ肳�ꂽ�e�̕ϊ��ƍ��킹��
				matw *= matparent;
				d3ddev->SetTransform(D3DTS_WORLD,&matw);//*�ݒ�*
				//(5) �`��
				d3ddev->SetTexture(0,dds->pTex[j*dds->xsufnum+i]);//�e�N�X�`���ݒ�
				d3ddev->SetVertexShader( FVF_3DVERTEX );//���_�̃t�H�[�}�b�g���w��
				d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vrtxarr,sizeof(MYVERTEX3D));//�`��
			}
		}
	}
}

void CDirectDraw::MyBlt3D(MYSURFACE *dds,RECT src,MYRECT3D dst,DWORD flag,DWORD color)
{
	if(dds==NULL){return;}

	//��`�𐳂����ݒ�
	int dmi;
	if(src.left > src.right){
		dmi = src.left;
		src.left = src.right;
		src.right = dmi;
	}
	if(src.top > src.bottom){
		dmi = src.top;
		src.top = src.bottom;
		src.bottom = dmi;
	}

	if(config2.flag & CONFIG2_HALFMODE){
		src.left/=2;
		src.right/=2;
		src.top/=2;
		src.bottom/=2;
	}

	DWORD i,j;	

	DWORD r_top,r_bottom,r_left,r_right;//warning���E�U�C
	r_top=src.top;
	r_bottom=src.bottom;
	r_left=src.left;
	r_right=src.right;

	//�S�Ẵe�N�X�`���Ɋւ��ĕ`�悷�邩�ǂ������ׂĕ`�悷��
	float vl,vr,vt,vb;//�e���_�̍��W
	float tumin,tumax,tvmin,tvmax;//u,v���W�͈̔�
	float ar = (float)g_DISPLAYWIDTH/(float)g_DISPLAYHEIGHT;//�A�X�y�N�g��
	float ar2 = 2.0f/(float)g_DISPLAYHEIGHT;
	float centerx = (float)g_DISPLAYWIDTH/2.0f;//x������ʒ��S
	float cut_left,cut_right,cut_top,cut_bottom;//�؂�Ă钷��(�s�N�Z���P��)
	float transx,transy,sclx,scly;//�ꎞ�I�Ɏg�p
	MYVERTEX3D vrtxarr[4];//���_�z��
	D3DXMATRIX matw;//���[���h���W�ϊ��s��
	D3DXMATRIX tmpmat;//�e���|�����s��
	for(j=0;j<dds->ysufnum;j++){
		for(i=0;i<dds->xsufnum;i++){
			if(dds->pTex[j*dds->xsufnum+i] ==NULL );
			else if(dds->xsufindx[i]+dds->xsufsize[i] < r_left);
			else if(dds->xsufindx[i] > r_right);
			else if(dds->ysufindx[j]+dds->ysufsize[j] < r_top);
			else if(dds->ysufindx[j] > r_bottom);
			else{
				//(0) ���E�㉺�A���ꂼ��]�����Ȃ������v�Z
				if(dds->xsufindx[i] < r_left)cut_left=(float)(r_left-dds->xsufindx[i]);
				else cut_left=0;
				if(dds->xsufindx[i]+dds->xsufsize[i] > r_right)
					cut_right = (float)( dds->xsufindx[i]+dds->xsufsize[i] - r_right);
				else cut_right=0;

				if(dds->ysufindx[j] < r_top)cut_top = (float)( r_top - dds->ysufindx[j] );
				else cut_top=0;
				if(dds->ysufindx[j]+dds->ysufsize[j] > r_bottom)
					cut_bottom = (float)( dds->ysufindx[j]+dds->ysufsize[j] - r_bottom );
				else cut_bottom=0;

				//(1) u,v���W�̌v�Z
				tumin = cut_left / (float)dds->xsufsize[i];
				tumax = 1.0f - cut_right/(float)dds->xsufsize[i];
				tvmin = cut_top / (float)dds->ysufsize[j];
				tvmax = 1.0f - cut_bottom/(float)dds->ysufsize[j];

				//(2) �]�����W�̌v�Z(�ȍ~�����Q��)
				//���ƍ����������i�s�N�Z����float�ϊ��ύ��W�n�j
				//��
				vl=0;
				vr=dds->xsufsize[i] - (cut_left+cut_right);
				vr= vr*ar2;// - ar;
				//����
				vt=0;
				vb=dds->ysufsize[j] - (cut_top+cut_bottom);
				vb=vb*ar2;//-1.0f
				//(3) ���_�z��ɍ��W�l����
				//����
				vrtxarr[0].x = vl;
				vrtxarr[0].y = vt;
				vrtxarr[0].z = dst.z;
				//����
				vrtxarr[1].x = vl;
				vrtxarr[1].y = vb;
				vrtxarr[1].z = dst.z;
				//�E��
				vrtxarr[2].x = vr;
				vrtxarr[2].y = vt;
				vrtxarr[2].z = dst.z;
				//�E��
				vrtxarr[3].x = vr;
				vrtxarr[3].y = vb;
				vrtxarr[3].z = dst.z;

				//���_�F
				vrtxarr[0].color = color;
				vrtxarr[1].color = color;
				vrtxarr[2].color = color;
				vrtxarr[3].color = color;

				//(3.5) �e�N�X�`�����W�ݒ�
				vrtxarr[0].tu = tumin;
				vrtxarr[0].tv = tvmin;
				vrtxarr[1].tu = tumin;
				vrtxarr[1].tv = tvmax;
				vrtxarr[2].tu = tumax;
				vrtxarr[2].tv = tvmin;
				vrtxarr[3].tu = tumax;
				vrtxarr[3].tv = tvmax;

				//(4) ���[���h���W�ϊ��s��p��
				//�P�ʍs��
				D3DXMatrixIdentity(&matw);
				//��`���ϊ�(���W�̒P�ʂ�float�ϊ���̂���)
				if(r_left>dds->xsufindx[i])transx=0;
				else transx = (float)( dds->xsufindx[i] - r_left );
				if(r_top>dds->ysufindx[j])transy=0;
				else transy = (float)( dds->ysufindx[j] - r_top );
				D3DXMatrixTranslation(&tmpmat,transx*ar2,transy*ar2,0);
				matw *= tmpmat;
				//�w�肳�ꂽMYRECT3D�܂Ŋg��&�ړ�
				sclx=dst.right-dst.left;
				scly=dst.bottom-dst.top;
				D3DXMatrixScaling(&tmpmat,sclx,scly,1.0f);
				matw *= tmpmat;
				D3DXMatrixTranslation(&tmpmat,dst.left,dst.top,0);
				matw *= tmpmat;
				if(flag & CKBLT_YUREY){//�h��
					D3DXMatrixTranslation(&tmpmat,0,yurey*ar2,0);
					matw *= tmpmat;
				}
				//�ݒ肳�ꂽ�e�̕ϊ��ƍ��킹��
				matw *= matparent;
				d3ddev->SetTransform(D3DTS_WORLD,&matw);//*�ݒ�*
				//(5) �`��
				d3ddev->SetTexture(0,dds->pTex[j*dds->xsufnum+i]);//�e�N�X�`���ݒ�
				d3ddev->SetVertexShader( FVF_3DVERTEX );//���_�̃t�H�[�}�b�g���w��
				d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vrtxarr,sizeof(MYVERTEX3D));//�`��
			}
		}
	}
}

void CDirectDraw::CellDraw(MYSURFACE **pbuf,LPVOID pcdat,LPVOID prdat,DWORD cn,int x,int y,float z,int rot,BOOL revx,BOOL revy,DWORD color,float magx,float magy)
{
	if(pbuf==NULL || pcdat==NULL || prdat==NULL)return;
	if(cn >= GCDMAX_CELLS)return;

	GCD_CELL2 *cdat=(GCD_CELL2*)pcdat;
	GCD_RECT  *rdat=(GCD_RECT*)prdat;

	D3DXMATRIX matp,mat,tmt,matprv,matprv2;
	float ar2 = 2.0f/(float)g_DISPLAYHEIGHT;

	//�L�����N�^�[�̕ϊ��s��
	D3DXMatrixIdentity(&matp);
	D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2*(-1.0f),(float)(cdat[cn].gcy)*ar2*(-1.0f),0);//�d�S�Ɉړ�
	matp *= tmt;
	D3DXMatrixScaling(&tmt,magx,magy,1.0f);//�g��
	matp *= tmt;
	D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//��]
	matp *= tmt;
	if(revy){
		d3dxplane_y.d=0;
		D3DXMatrixReflect(&tmt,&d3dxplane_y);//y���]
		matp *= tmt;
	}
	D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2,(float)(cdat[cn].gcy)*ar2,0);//�d�S�ɖ߂�
	matp *= tmt;
	if(revx){
		d3dxplane_x.d=0;
		D3DXMatrixReflect(&tmt,&d3dxplane_x);//x���]
		matp *= tmt;
	}
	D3DXMatrixTranslation(&tmt,(float)x*ar2,(float)y*ar2,0);//�\���ʒu�ւ̈ړ�
	matp *= tmt;

	matprv = SetParentMatrix(matp,FALSE);//�u�e�v�̕ϊ��s��Ƃ��Đݒ�

	DWORD rn;
	BOOL c1revx,c1revy;

	for(int i=0;i<8;i++){
		rn = cdat[cn].cell[i].cdr;
		if(rn < GCDMAX_RECTANGLES){
			//�ϊ��s����v�Z
			D3DXMatrixIdentity(&mat);
			D3DXMatrixTranslation(&tmt,(float)(rdat[rn].center_x)*ar2*(-1.0f),(float)(rdat[rn].center_y)*ar2*(-1.0f),0);//�d�S�Ɉړ�
			mat *= tmt;
			D3DXMatrixScaling(&tmt,cdat[cn].cell[i].magx,cdat[cn].cell[i].magy,1.0f);//�g��
			mat *= tmt;
			D3DXMatrixRotationZ(&tmt,D3DXToRadian(cdat[cn].cell[i].rot));//��]
			mat *= tmt;
			D3DXMatrixTranslation(&tmt,(float)(rdat[rn].center_x)*ar2,(float)(rdat[rn].center_y)*ar2,0);//�d�S�ɖ߂�
			mat *= tmt;
			D3DXMatrixTranslation(&tmt,(float)(cdat[cn].cell[i].dx)*ar2,(float)(cdat[cn].cell[i].dy)*ar2,0);//�\���ʒu�ւ̈ړ�
			mat *= tmt;

			matprv2 = SetParentMatrix(mat,FALSE,TRUE);

			c1revx = cdat[cn].cell[i].flag & GCDCELL_REVERSE_X;
			c1revy = cdat[cn].cell[i].flag & GCDCELL_REVERSE_Y;

			CheckBlt(pbuf[rdat[rn].bmpno],
				0,0,rdat[rn].r,
				c1revx,
				c1revy,
				0,z,color);//�`��

			SetParentMatrix(matprv2,TRUE);
		}
	}

	SetParentMatrix(matprv,TRUE);//�u�e�v�̕ϊ��s������ɖ߂�

	if(cdat[cn].flag & GCDCELL2_LINK)
		CellDraw(pbuf,pcdat,prdat,cn+1,x,y,z,rot,color);
}

#ifdef ___GCDEDITER

void CDirectDraw::CellDrawED(int chcol,MYSURFACE **pbuf,LPVOID pcdat,LPVOID prdat,DWORD cn,int x,int y,float z,int rot,BOOL revx,BOOL revy,float magx,float magy,BOOL linkprv,BOOL linknext)
{
	BOOL edthis = FALSE;
	if(linknext && linkprv)edthis=TRUE;

	if(pbuf==NULL || pcdat==NULL || prdat==NULL)return;
	if(cn >= GCDMAX_CELLS)return;

	GCD_CELL2 *cdat=(GCD_CELL2*)pcdat;
	GCD_RECT  *rdat=(GCD_RECT*)prdat;

	if(cn > 1 && linkprv)
		if(cdat[cn-1].flag & GCDCELL2_LINK)
			CellDrawED(chcol,pbuf,pcdat,prdat,cn-1,x,y,z,rot,revx,revy,magx,magy,TRUE,FALSE);

	D3DXMATRIX matp,mat,tmt,matprv,matprv2;
	float ar2 = 2.0f/(float)g_DISPLAYHEIGHT;

	//�L�����N�^�[�̕ϊ��s��
	D3DXMatrixIdentity(&matp);
	D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2*(-1.0f),(float)(cdat[cn].gcy)*ar2*(-1.0f),0);//�d�S�Ɉړ�
	matp *= tmt;
	D3DXMatrixScaling(&tmt,magx,magy,1.0f);//�g��
	matp *= tmt;
	D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//��]
	matp *= tmt;
	if(revy){
		d3dxplane_y.d=0;
		D3DXMatrixReflect(&tmt,&d3dxplane_y);//y���]
		matp *= tmt;
	}
	//�d�S�ɖ߂�
	D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2,(float)(cdat[cn].gcy)*ar2,0);
	matp *= tmt;
	if(revx){
		d3dxplane_x.d=0;
		D3DXMatrixReflect(&tmt,&d3dxplane_x);//x���]
		matp *= tmt;
	}
	D3DXMatrixTranslation(&tmt,(float)x*ar2,(float)y*ar2,0);//�\���ʒu�ւ̈ړ�
	matp *= tmt;

	matprv = SetParentMatrix(matp,FALSE);//�u�e�v�̕ϊ��s��Ƃ��Đݒ�

	DWORD rn;
	BOOL c1revx,c1revy;
	DWORD bcolor = 0xFFAAAAAA;
	if(!edthis)bcolor=0xFF555555;
	DWORD color;

	for(int i=0;i<8;i++){
		rn = cdat[cn].cell[i].cdr;
		if(rn < GCDMAX_RECTANGLES){
			//�ϊ��s����v�Z
			D3DXMatrixIdentity(&mat);
			//�d�S�Ɉړ�
			D3DXMatrixTranslation(&tmt,(float)(rdat[rn].center_x)*ar2*(-1.0f),(float)(rdat[rn].center_y)*ar2*(-1.0f),0);
			mat *= tmt;
			//�g��
			D3DXMatrixScaling(&tmt,cdat[cn].cell[i].magx,cdat[cn].cell[i].magy,1.0f);
			mat *= tmt;
			//��]
			D3DXMatrixRotationZ(&tmt,D3DXToRadian(cdat[cn].cell[i].rot));
			mat *= tmt;
			//�d�S�ɖ߂�
			D3DXMatrixTranslation(&tmt,(float)(rdat[rn].center_x)*ar2,(float)(rdat[rn].center_y)*ar2,0);
			mat *= tmt;
			//�\���ʒu�ւ̈ړ�
			D3DXMatrixTranslation(&tmt,(float)(cdat[cn].cell[i].dx)*ar2,(float)(cdat[cn].cell[i].dy)*ar2,0);
			mat *= tmt;

			matprv2 = SetParentMatrix(mat,FALSE,TRUE);

			c1revx = cdat[cn].cell[i].flag & GCDCELL_REVERSE_X;
			c1revy = cdat[cn].cell[i].flag & GCDCELL_REVERSE_Y;

			color = bcolor;
			if(edthis && chcol==i)color=0xFFFFFFFF;
			if(radio==1 || radio==2)color = 0xFFFFFFFF;

			CheckBlt(pbuf[rdat[rn].bmpno],
				0,0,rdat[rn].r,
				c1revx,
				c1revy,
				0,z,color);//�`��

			SetParentMatrix(matprv2,TRUE);
		}
	}

	SetParentMatrix(matprv,TRUE);//�u�e�v�̕ϊ��s������ɖ߂�

	if(cn < 1023 && linknext)
		if(cdat[cn].flag & GCDCELL2_LINK)
			CellDrawED(chcol,pbuf,pcdat,prdat,cn+1,x,y,z,rot,revx,revy,magx,magy,FALSE,TRUE);
}
#endif

void CDirectDraw::DrawLine(int sx,int sy,int gx,int gy,DWORD col)
{
	MYVERTEX3D vb[2];
	vb[0].color = col;
	vb[0].tu = vb[0].tv = vb[0].z = 0;
	vb[1] = vb[0];

	vb[0].x = (float)sx / (g_DISPLAYHEIGHT/2.0f);
	vb[0].y = (float)sy / (g_DISPLAYHEIGHT/2.0f);
	vb[1].x = (float)gx / (g_DISPLAYHEIGHT/2.0f);
	vb[1].y = (float)gy / (g_DISPLAYHEIGHT/2.0f);

	d3ddev->SetTexture(0,NULL);
	d3ddev->SetTransform(D3DTS_WORLD,&matparent);
	d3ddev->SetVertexShader(FVF_3DVERTEX);
	d3ddev->DrawPrimitiveUP(
		D3DPT_LINELIST,2,vb,sizeof(MYVERTEX3D));
}

void CDirectDraw::HRectDraw(LPVOID pcdat,LPVOID phdat,DWORD cn,int x,int y,float z,int rot,BOOL revx,BOOL revy,float magx,float magy)
{
	if(pcdat==NULL || phdat==NULL)return;
	if(cn >= GCDMAX_CELLS)return;

	GCD_CELL2 *cdat=(GCD_CELL2*)pcdat;
	GCD_HANTEI *hdat=(GCD_HANTEI*)phdat;

	D3DXMATRIX matp,mat,tmt,matprv,matprv2;
	float ar2 = 2.0f/(float)g_DISPLAYHEIGHT;

	//�L�����N�^�[�̕ϊ��s��
	D3DXMatrixIdentity(&matp);
	D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2*(-1.0f),(float)(cdat[cn].gcy)*ar2*(-1.0f),0);//�d�S�Ɉړ�
	matp *= tmt;
	D3DXMatrixScaling(&tmt,magx,magy,1.0f);//�g��
	matp *= tmt;
	D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//��]
	matp *= tmt;
	if(revy){
		d3dxplane_y.d=0;
		D3DXMatrixReflect(&tmt,&d3dxplane_y);//y���]
		matp *= tmt;
	}
	//�d�S�ɖ߂�
	D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2,(float)(cdat[cn].gcy)*ar2,0);
	matp *= tmt;
	if(revx){
		d3dxplane_x.d=0;
		D3DXMatrixReflect(&tmt,&d3dxplane_x);//x���]
		matp *= tmt;
	}
	D3DXMatrixTranslation(&tmt,(float)x*ar2,(float)y*ar2,0);//�\���ʒu�ւ̈ړ�
	matp *= tmt;

	matprv = SetParentMatrix(matp,FALSE);//�u�e�v�̕ϊ��s��Ƃ��Đݒ�

//	DWORD color;
	RECT r;

	for(int i=0;i<3;i++){
		r=hdat[cn].kas[i];
		DrawLine(r.left,r.top,r.left,r.bottom,0xFFFF0000);
		DrawLine(r.right,r.top,r.right,r.bottom,0xFFFF0000);
		DrawLine(r.left,r.top,r.right,r.top,0xFFFF0000);
		DrawLine(r.left,r.bottom,r.right,r.bottom,0xFFFF0000);
		r=hdat[cn].kurai[i];
		DrawLine(r.left,r.top,r.left,r.bottom,0xFFFFFF00);
		DrawLine(r.right,r.top,r.right,r.bottom,0xFFFFFF00);
		DrawLine(r.left,r.top,r.right,r.top,0xFFFFFF00);
		DrawLine(r.left,r.bottom,r.right,r.bottom,0xFFFFFF00);
		r=hdat[cn].attack[i];
		DrawLine(r.left,r.top,r.left,r.bottom,0xFF0000FF);
		DrawLine(r.right,r.top,r.right,r.bottom,0xFF0000FF);
		DrawLine(r.left,r.top,r.right,r.top,0xFF0000FF);
		DrawLine(r.left,r.bottom,r.right,r.bottom,0xFF0000FF);
	}

	SetParentMatrix(matprv,TRUE);//�u�e�v�̕ϊ��s������ɖ߂�
}

void CDirectDraw::GCenterDraw(LPVOID pcdat,DWORD cn,int x,int y,float z,int rot,BOOL revx,BOOL revy,float magx,float magy)
{
	if(pcdat==NULL)return;
	if(cn >= GCDMAX_CELLS)return;

	GCD_CELL2 *cdat=(GCD_CELL2*)pcdat;

	D3DXMATRIX matp,mat,tmt,matprv,matprv2;
	float ar2 = 2.0f/(float)g_DISPLAYHEIGHT;

	//�L�����N�^�[�̕ϊ��s��
	D3DXMatrixIdentity(&matp);
	if(revx){
		d3dxplane_x.d=0;
		D3DXMatrixReflect(&tmt,&d3dxplane_x);//x���]
		matp *= tmt;
	}
	//�d�S�ʒu�ւ̈ړ�
	D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2,(float)(cdat[cn].gcy)*ar2,0);
	matp *= tmt;
	//�\���ʒu�ւ̈ړ�
	D3DXMatrixTranslation(&tmt,(float)x*ar2,(float)y*ar2,0);
	matp *= tmt;

	matprv = SetParentMatrix(matp,FALSE);//�u�e�v�̕ϊ��s��Ƃ��Đݒ�

	//Draw
	DrawLine(-20,0,20,0,0xFF00FF00);
	DrawLine(0,-20,0,-20,0xFF00FF00);
	DrawLine(-20, 20, 20, 20,0xFF00FF00);
	DrawLine(-20,-20, 20,-20,0xFF00FF00);
	DrawLine( 20,-20, 20, 20,0xFF00FF00);
	DrawLine(-20,-20,-20, 20,0xFF00FF00);

	SetParentMatrix(matprv,TRUE);//�u�e�v�̕ϊ��s������ɖ߂�
}

//*******************************************************************************
//�@�ϊ��s��ݒ�֘A
//*******************************************************************************

// world ------------------------------------------------------------------------
D3DXMATRIX CDirectDraw::SetParentMatrix(D3DXMATRIX mat,BOOL root,BOOL insert)
{
	D3DXMATRIX matprv=matparent;

	if(root)ResetParentMatrix();

	if(insert)matparent = mat*matparent;
	else matparent *= mat;

	return(matprv);
}

D3DXMATRIX CDirectDraw::ResetParentMatrix()
{
	D3DXMATRIX matprv=matparent;
	D3DXMatrixIdentity(&matparent);
	return(matprv);
}

// view & projection ------------------------------------------------------------
void CDirectDraw::ResetTransformMatrix()
{
	D3DXMATRIX matw,matv,matp;

	//view
	D3DXMatrixLookAtRH(&matv,
		&D3DXVECTOR3(camera_x,camera_y,camera_z),
		&D3DXVECTOR3(camera_x,camera_y,0),
		&D3DXVECTOR3(0,-1,0));
	d3ddev->SetTransform(D3DTS_VIEW,&matv);

	//projection
	D3DXMatrixPerspectiveFovRH(&matp,
		(float)(3.1415926/2.0),
		(float)((float)g_DISPLAYWIDTH/(float)g_DISPLAYHEIGHT),
		0.1f,
		100.0f);
	d3ddev->SetTransform(D3DTS_PROJECTION,&matp);
}

// �݊��� ****************************************************************

void CDirectDraw::DrawCircle(int x,int y,int rad,int w)
{
#ifdef _DEBUG
	OutputDebugString("���i�x���jCDirectDraw::DrawCircle���Ă΂�܂�����\n");
#endif
	return;
}
