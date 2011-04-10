

#include "stage.h"
#include <math.h>


SET_STAGEINFO(CStage)


/*!
*	@brief �R���X�g���N�^
*/
CStage::CStage(SDI_STAGEINFO2 *info)
			: CStageBase(info)
{
	ptex_jimen=NULL;
	ptex_fens=NULL;
	ptex_buil1a=NULL;
	ptex_sky=NULL;
	ptex_brdura=NULL;
	ptex_buil2a=NULL;
	ptex_bg1=NULL;
	ptex_bg2=NULL;
	for (int i = 0; i < 10; i++)
		vb_buil1d[i] = NULL;

	d3ddev = NULL;
}

/*!
*	@brief �f�X�g���N�^
*/
CStage::~CStage()
{
	//unload textures
	RELEASE(ptex_jimen);
	RELEASE(ptex_fens);
	RELEASE(ptex_buil1a);
	RELEASE(ptex_brdura);
	RELEASE(ptex_sky);
	RELEASE(ptex_buil2a);
	RELEASE(ptex_bg1);
	RELEASE(ptex_bg2);
	for (int i = 0; i < 10; i++)
		RELEASE(vb_buil1d[i]);
}

/*!
*	@brief ������
*/
void CStage::InitializeStage()
{
	d3ddev = GetD3DDevice();

	InitVrtx();//���_���W������
	LoadTextures();//�e�N�X�`���Ǎ���
}


void CStage::InitVrtx()//���_���W������
{
	int i,j;
	MYVERTEX3D* vtx_tmp = NULL;

	//�n��
	vb_jimen[0].color=vb_jimen[2].color=0xFFFFFFFF;
	vb_jimen[1].color=vb_jimen[3].color=0xFF333333;
	vb_jimen[0].tu=-10;
	vb_jimen[0].tv=-3;
	vb_jimen[1].tu=vb_jimen[0].tu;
	vb_jimen[1].tv=3;
	vb_jimen[2].tu=10;
	vb_jimen[2].tv=vb_jimen[0].tv;
	vb_jimen[3].tu=vb_jimen[2].tu;
	vb_jimen[3].tv=vb_jimen[1].tv;
	vb_jimen[0].x=vb_jimen[1].x=(640.0f/240.0f+0.5f)*(-1.0f);
	vb_jimen[2].x=vb_jimen[3].x=(640.0f/240.0f+0.5f);
	vb_jimen[0].y=vb_jimen[1].y=
		vb_jimen[2].y=vb_jimen[3].y=0;
	vb_jimen[0].z=vb_jimen[2].z=0.3f;
	vb_jimen[1].z=vb_jimen[3].z=-1.0f;

	//�t�F���X
	vb_fens[0].color=vb_fens[1].color=
		vb_fens[2].color=vb_fens[3].color=0xFFFFFFFF;
	vb_fens[0].tu=-20;
	vb_fens[0].tv=-3;
	vb_fens[1].tu=vb_fens[0].tu;
	vb_fens[1].tv=3;
	vb_fens[2].tu=20;
	vb_fens[2].tv=vb_fens[0].tv;
	vb_fens[3].tu=vb_fens[2].tu;
	vb_fens[3].tv=vb_fens[1].tv;
	vb_fens[0].x=vb_fens[1].x=(640.0f/240.0f+0.5f)*(-1.0f);
	vb_fens[2].x=vb_fens[3].x=(640.0f/240.0f+0.5f);
	vb_fens[0].z=vb_fens[1].z=
		vb_fens[2].z=vb_fens[3].z=0.3f;
	vb_fens[0].y=vb_fens[2].y=-0.6f;
	vb_fens[1].y=vb_fens[3].y=0.0f;

	//�t�F���X�肷��
	vb_fens2[0][0].color=vb_fens2[0][1].color=
		vb_fens2[0][2].color=vb_fens2[0][3].color=0xFF442277;
	vb_fens2[0][0].tu=-20;
	vb_fens2[0][0].tv=-3;
	vb_fens2[0][1].tu=vb_fens2[0][0].tu;
	vb_fens2[0][1].tv=3;
	vb_fens2[0][2].tu=20;
	vb_fens2[0][2].tv=vb_fens2[0][0].tv;
	vb_fens2[0][3].tu=vb_fens2[0][2].tu;
	vb_fens2[0][3].tv=vb_fens2[0][1].tv;
	vb_fens2[0][0].x=vb_fens2[0][1].x=(640.0f/240.0f+0.5f)*(-1.0f);
	vb_fens2[0][2].x=vb_fens2[0][3].x=(640.0f/240.0f+0.5f);
	vb_fens2[0][0].z=vb_fens2[0][1].z=
		vb_fens2[0][2].z=vb_fens2[0][3].z=0.3f;
	vb_fens2[0][0].y=vb_fens2[0][2].y=-0.63f;
	vb_fens2[0][1].y=vb_fens2[0][3].y=-0.6f;
	//�t�F���X�肷��(��)
	vb_fens2[1][0].color=vb_fens2[1][1].color=
		vb_fens2[1][2].color=vb_fens2[1][3].color=0xFF442277;
	vb_fens2[1][0].tu=-20;
	vb_fens2[1][0].tv=-3;
	vb_fens2[1][1].tu=vb_fens2[1][0].tu;
	vb_fens2[1][1].tv=3;
	vb_fens2[1][2].tu=20;
	vb_fens2[1][2].tv=vb_fens2[1][0].tv;
	vb_fens2[1][3].tu=vb_fens2[1][2].tu;
	vb_fens2[1][3].tv=vb_fens2[1][1].tv;
	vb_fens2[1][0].x=vb_fens2[1][1].x=(640.0f/240.0f+0.5f)*(-1.0f);
	vb_fens2[1][2].x=vb_fens2[1][3].x=(640.0f/240.0f+0.5f);
	vb_fens2[1][0].z=vb_fens2[1][1].z=
		vb_fens2[1][2].z=vb_fens2[1][3].z=0.3f;
	vb_fens2[1][0].y=vb_fens2[1][2].y=-0.1f;
	vb_fens2[1][1].y=vb_fens2[1][3].y= 0.0f;

	//�r���i�E�j�A����
	vb_buil1a[0][0].color=vb_buil1a[0][1].color=0xFFFFFFFF;
	vb_buil1a[0][2].color=vb_buil1a[0][3].color=0xFFAAAAAA;
	vb_buil1a[0][0].tu=0;
	vb_buil1a[0][0].tv=0;
	vb_buil1a[0][1].tu=0;
	vb_buil1a[0][1].tv=1;
	vb_buil1a[0][2].tu=1;
	vb_buil1a[0][2].tv=0;
	vb_buil1a[0][3].tu=1;
	vb_buil1a[0][3].tv=1;
	vb_buil1a[0][0].x=vb_buil1a[0][1].x=
		vb_buil1a[0][2].x=vb_buil1a[0][3].x=-0.5f;
	vb_buil1a[0][0].y=vb_buil1a[0][2].y=-0.4f;
	vb_buil1a[0][1].y=vb_buil1a[0][3].y=1.2f;
	vb_buil1a[0][0].z=vb_buil1a[0][1].z=1.5f;
	vb_buil1a[0][2].z=vb_buil1a[0][3].z=0.5f;

	//�r���i�E�j�A�w��
	vb_buil1a[1][0].color=vb_buil1a[1][1].color=0xFF9999AA;
	vb_buil1a[1][2].color=vb_buil1a[1][3].color=0xFF9999AA;
	vb_buil1a[1][0].tu=0;
	vb_buil1a[1][0].tv=0;
	vb_buil1a[1][1].tu=0;
	vb_buil1a[1][1].tv=1;
	vb_buil1a[1][2].tu=1.5;
	vb_buil1a[1][2].tv=0;
	vb_buil1a[1][3].tu=1.5;
	vb_buil1a[1][3].tv=1;
	vb_buil1a[1][0].z=vb_buil1a[1][1].z=
		vb_buil1a[1][2].z=vb_buil1a[1][3].z=0.5f;
	vb_buil1a[1][0].y=vb_buil1a[1][2].y=-0.4f;
	vb_buil1a[1][1].y=vb_buil1a[1][3].y=1.2f;
	vb_buil1a[1][0].x=vb_buil1a[1][1].x=-0.5f;
	vb_buil1a[1][2].x=vb_buil1a[1][3].x=1.5f;

	//�r���V��
	vb_buil1b[0].color=vb_buil1b[1].color=0xFF555599;
	vb_buil1b[2].color=vb_buil1b[3].color=0xFF555599;
	vb_buil1b[0].tu=0;
	vb_buil1b[0].tv=0;
	vb_buil1b[1].tu=0;
	vb_buil1b[1].tv=10;
	vb_buil1b[2].tu=15;
	vb_buil1b[2].tv=0;
	vb_buil1b[3].tu=15;
	vb_buil1b[3].tv=10;
	vb_buil1b[0].y=vb_buil1b[1].y=
		vb_buil1b[2].y=vb_buil1b[3].y=vb_buil1a[1][0].y;
	vb_buil1b[0].z=vb_buil1b[2].z=	vb_buil1a[0][0].z;
	vb_buil1b[1].z=vb_buil1b[3].z=	vb_buil1a[1][0].z;
	vb_buil1b[0].x=vb_buil1b[1].x=	vb_buil1a[1][0].x;
	vb_buil1b[2].x=vb_buil1b[3].x=	vb_buil1a[1][2].x;

	//�r���i�E�j�A�Ŕw��
	vb_buil1c[0].color=vb_buil1c[1].color=0xFFEEEEEE;
	vb_buil1c[2].color=vb_buil1c[3].color=0xFFEEEEEE;
	vb_buil1c[0].tu=0;
	vb_buil1c[0].tv=0;
	vb_buil1c[1].tu=0;
	vb_buil1c[1].tv=8;
	vb_buil1c[2].tu=8;
	vb_buil1c[2].tv=0;
	vb_buil1c[3].tu=8;
	vb_buil1c[3].tv=8;
	vb_buil1c[0].z=vb_buil1c[1].z=
		vb_buil1c[2].z=vb_buil1c[3].z=1.5f;
	vb_buil1c[0].y=vb_buil1c[2].y=-1.8f;
	vb_buil1c[1].y=vb_buil1c[3].y=-0.5f;
	vb_buil1c[0].x=vb_buil1c[1].x=-0.5f;
	vb_buil1c[2].x=vb_buil1c[3].x= 1.5f;

	float x1=vb_buil1c[0].x + 0.1f;
	float x2=vb_buil1c[3].x - 0.1f;
	float y1=vb_buil1c[0].y + 0.1f;
	float y2=vb_buil1c[3].y - 0.1f;
	float y3=vb_buil1b[0].y;
	float z1=vb_buil1c[0].z - 0.02f;
	float z2=vb_buil1a[1][0].z + 0.4f;
	float futosa1=0.06f;
	float futosa2=0.03f;

	//�Ŕ̎x��
	MYVERTEX3D vb_buil1d_tmp[10][4];
	for(i=0;i<10;i++){
		for(j=0;j<4;j++){
			vb_buil1d_tmp[i][j].color=0xFF000022;
			vb_buil1d_tmp[i][j].z=z1;
		}
	}
	//�@
	vb_buil1d_tmp[0][0].x=vb_buil1d_tmp[0][1].x=x1;
	vb_buil1d_tmp[0][2].x=vb_buil1d_tmp[0][3].x=x1+futosa1;
	vb_buil1d_tmp[0][0].y=vb_buil1d_tmp[0][2].y=y1;
	vb_buil1d_tmp[0][1].y=vb_buil1d_tmp[0][3].y=y3;
	//�A
	vb_buil1d_tmp[1][0].x=vb_buil1d_tmp[1][1].x=x2;
	vb_buil1d_tmp[1][2].x=vb_buil1d_tmp[1][3].x=x2-futosa1;
	vb_buil1d_tmp[1][0].y=vb_buil1d_tmp[1][2].y=y1;
	vb_buil1d_tmp[1][1].y=vb_buil1d_tmp[1][3].y=y3;
	//�B
	vb_buil1d_tmp[2][0].x=vb_buil1d_tmp[2][1].x=x1;
	vb_buil1d_tmp[2][1].x+=futosa2;
	vb_buil1d_tmp[2][2].x=vb_buil1d_tmp[2][3].x=x2;
	vb_buil1d_tmp[2][2].x-=futosa2;
	vb_buil1d_tmp[2][0].y=vb_buil1d_tmp[2][1].y=y1;
	vb_buil1d_tmp[2][2].y=vb_buil1d_tmp[2][3].y=y2;
	//�C
	vb_buil1d_tmp[3][0].x=vb_buil1d_tmp[3][1].x=x2;
	vb_buil1d_tmp[3][1].x-=futosa2;
	vb_buil1d_tmp[3][2].x=vb_buil1d_tmp[3][3].x=x1;
	vb_buil1d_tmp[3][2].x+=futosa2;
	vb_buil1d_tmp[3][0].y=vb_buil1d_tmp[3][1].y=y1;
	vb_buil1d_tmp[3][2].y=vb_buil1d_tmp[3][3].y=y2;
	i=4;//�D
	vb_buil1d_tmp[i][0].x=vb_buil1d_tmp[i][1].x=x2;
	vb_buil1d_tmp[i][1].x-=futosa2;
	vb_buil1d_tmp[i][2].x=vb_buil1d_tmp[i][3].x=x1;
	vb_buil1d_tmp[i][2].x+=futosa2;
	vb_buil1d_tmp[i][0].y=vb_buil1d_tmp[i][1].y=y1;
	vb_buil1d_tmp[i][2].y=vb_buil1d_tmp[i][3].y=y3;
	i=5;//�E
	vb_buil1d_tmp[i][0].x=vb_buil1d_tmp[i][1].x=x1;
	vb_buil1d_tmp[i][1].x+=futosa2;
	vb_buil1d_tmp[i][2].x=vb_buil1d_tmp[i][3].x=x2;
	vb_buil1d_tmp[i][2].x-=futosa2;
	vb_buil1d_tmp[i][0].y=vb_buil1d_tmp[i][1].y=y1;
	vb_buil1d_tmp[i][2].y=vb_buil1d_tmp[i][3].y=y3;
	i=6;//�F
	vb_buil1d_tmp[i][0].x=vb_buil1d_tmp[i][1].x=x1;
	vb_buil1d_tmp[i][2].x=vb_buil1d_tmp[i][3].x=x2;
	vb_buil1d_tmp[i][0].y=vb_buil1d_tmp[i][2].y=y1;
	vb_buil1d_tmp[i][1].y=vb_buil1d_tmp[i][3].y=y1-futosa1;
	i=7;//�G
	vb_buil1d_tmp[i][0].x=vb_buil1d_tmp[i][1].x=x1;
	vb_buil1d_tmp[i][2].x=vb_buil1d_tmp[i][3].x=x2;
	vb_buil1d_tmp[i][0].y=vb_buil1d_tmp[i][2].y=y2;
	vb_buil1d_tmp[i][1].y=vb_buil1d_tmp[i][3].y=y2-futosa1;
	i=8;//�H
	vb_buil1d_tmp[i][0].x=vb_buil1d_tmp[i][1].x=x1;
	vb_buil1d_tmp[i][2].x=vb_buil1d_tmp[i][3].x=x1+futosa1;
	vb_buil1d_tmp[i][0].y=vb_buil1d_tmp[i][2].y=y1;
	vb_buil1d_tmp[i][1].y=vb_buil1d_tmp[i][3].y=y3;
	vb_buil1d_tmp[i][1].z=vb_buil1d_tmp[i][3].z=z2;
	i=9;//�I
	vb_buil1d_tmp[i][0].x=vb_buil1d_tmp[i][1].x=x2;
	vb_buil1d_tmp[i][2].x=vb_buil1d_tmp[i][3].x=x2-futosa1;
	vb_buil1d_tmp[i][0].y=vb_buil1d_tmp[i][2].y=y1;
	vb_buil1d_tmp[i][1].y=vb_buil1d_tmp[i][3].y=y3;
	vb_buil1d_tmp[i][1].z=vb_buil1d_tmp[i][3].z=z2;

	for(i=0;i<10;i++){
		if ( SUCCEEDED(d3ddev->CreateVertexBuffer(sizeof(MYVERTEX3D) * 4, 0, FVF_3DVERTEX,
						D3DPOOL_MANAGED, &vb_buil1d[i])) &&
			SUCCEEDED(vb_buil1d[i]->Lock(0, 0, (BYTE**)&vtx_tmp, 0)) )
		{
			memcpy(vtx_tmp, vb_buil1d_tmp[i], sizeof(vb_buil1d_tmp[i]));
			vb_buil1d[i]->Unlock();
		}
	}

	//�r���Q�w��
	vb_buil2b[0].color=vb_buil2b[1].color=0xFF551199;
	vb_buil2b[2].color=vb_buil2b[3].color=0xFF220033;
	vb_buil2b[0].tu=0;
	vb_buil2b[0].tv=0;
	vb_buil2b[1].tu=0;
	vb_buil2b[1].tv=2;
	vb_buil2b[2].tu=1;
	vb_buil2b[2].tv=0;
	vb_buil2b[3].tu=1;
	vb_buil2b[3].tv=2;
	vb_buil2b[0].z=vb_buil2b[1].z=
		vb_buil2b[2].z=vb_buil2b[3].z=0.7f;
	vb_buil2b[0].y=vb_buil2b[2].y=-3.0f;
	vb_buil2b[1].y=vb_buil2b[3].y= 1.2f;
	vb_buil2b[0].x=vb_buil2b[1].x=-0.5f;
	vb_buil2b[2].x=vb_buil2b[3].x=-2.5f;
	//�r���Q����
	vb_buil2a[0].color=vb_buil2a[1].color=0xFFFFFFFF;
	vb_buil2a[2].color=vb_buil2a[3].color=0xFFAAAAAA;
	vb_buil2a[0].tu=0;
	vb_buil2a[0].tv=0;
	vb_buil2a[1].tu=0;
	vb_buil2a[1].tv=2;
	vb_buil2a[2].tu=1;
	vb_buil2a[2].tv=0;
	vb_buil2a[3].tu=1;
	vb_buil2a[3].tv=2;
	vb_buil2a[0].x=vb_buil2a[1].x=
		vb_buil2a[2].x=vb_buil2a[3].x=-0.5f;
	vb_buil2a[0].y=vb_buil2a[2].y=-3.0f;
	vb_buil2a[1].y=vb_buil2a[3].y= 1.2f;
	vb_buil2a[0].z=vb_buil2a[1].z=1.5f;
	vb_buil2a[2].z=vb_buil2a[3].z=0.7f;

	//��
	vb_sky[0].color=vb_sky[1].color=
		vb_sky[2].color=vb_sky[3].color=0xFFFFFFFF;
	vb_sky[0].tu=0;
	vb_sky[0].tv=0;
	vb_sky[1].tu=vb_sky[0].tu;
	vb_sky[1].tv=1;
	vb_sky[2].tu=1;
	vb_sky[2].tv=vb_sky[0].tv;
	vb_sky[3].tu=vb_sky[2].tu;
	vb_sky[3].tv=vb_sky[1].tv;
	vb_sky[0].x=vb_sky[1].x=-18.0f;
	vb_sky[2].x=vb_sky[3].x= 18.0f;
	vb_sky[0].z=vb_sky[1].z=
		vb_sky[2].z=vb_sky[3].z=20.0f;
	vb_sky[0].y=vb_sky[2].y=-9.0f;
	vb_sky[1].y=vb_sky[3].y= 9.0f;

	//�w�i�P
	vb_bg1[0].color=vb_bg1[1].color=
		vb_bg1[2].color=vb_bg1[3].color=0xFFFFFFFF;
	vb_bg1[0].tu=0;
	vb_bg1[0].tv=0;
	vb_bg1[1].tu=vb_bg1[0].tu;
	vb_bg1[1].tv=1;
	vb_bg1[2].tu=1;
	vb_bg1[2].tv=vb_bg1[0].tv;
	vb_bg1[3].tu=vb_bg1[2].tu;
	vb_bg1[3].tv=vb_bg1[1].tv;
	vb_bg1[0].x=vb_bg1[1].x=-10.0f;
	vb_bg1[2].x=vb_bg1[3].x= 10.0f;
	vb_bg1[0].z=vb_bg1[1].z=
		vb_bg1[2].z=vb_bg1[3].z=12.0f;
	vb_bg1[0].y=vb_bg1[2].y=-2.5f;
	vb_bg1[1].y=vb_bg1[3].y= 3.5f;

	//�w�i�Q
	vb_bg2[0].color=vb_bg2[1].color=
		vb_bg2[2].color=vb_bg2[3].color=0xFFFFFFFF;
	vb_bg2[0].tu=0;
	vb_bg2[0].tv=0;
	vb_bg2[1].tu=vb_bg2[0].tu;
	vb_bg2[1].tv=1;
	vb_bg2[2].tu=1;
	vb_bg2[2].tv=vb_bg2[0].tv;
	vb_bg2[3].tu=vb_bg2[2].tu;
	vb_bg2[3].tv=vb_bg2[1].tv;
	vb_bg2[0].x=vb_bg2[1].x=-8.0f;
	vb_bg2[2].x=vb_bg2[3].x= 8.0f;
	vb_bg2[0].z=vb_bg2[1].z=
		vb_bg2[2].z=vb_bg2[3].z=8.0f;
	vb_bg2[0].y=vb_bg2[2].y=-1.5f;
	vb_bg2[1].y=vb_bg2[3].y= 2.5f;
}

void CStage::LoadTextures()//�e�N�X�`���Ǎ���
{
	if(d3ddev==NULL)return;

	char* filename=(char*)malloc(256);
	if(filename==NULL)return;

	//�n��
	sprintf(filename,"%s\\tex_jimen.bmp",GetDLLPath());
	D3DXCreateTextureFromFileA(
		d3ddev,
		filename,
		&ptex_jimen);
	sprintf(filename,"%s\\tex_fens.bmp",GetDLLPath());
	D3DXCreateTextureFromFileExA(
		d3ddev,filename,
		0,0,0,0,
		D3DFMT_A1R5G5B5,
		D3DPOOL_MANAGED,D3DX_FILTER_POINT,D3DX_FILTER_POINT,
		0xFF000000,NULL,NULL,
		&ptex_fens);
	
	//�r��1
	sprintf(filename,"%s\\tex_buil1a.bmp",GetDLLPath());
	D3DXCreateTextureFromFileExA(
		d3ddev,filename,
		0,0,0,0,
		D3DFMT_X1R5G5B5,
		D3DPOOL_MANAGED,D3DX_FILTER_POINT,D3DX_FILTER_POINT,
		0,NULL,NULL,
		&ptex_buil1a);
	//�r���P�Ŕ�
	sprintf(filename,"%s\\tex_brdura.bmp",GetDLLPath());
	D3DXCreateTextureFromFileA(
		d3ddev,
		filename,
		&ptex_brdura);
	
	//�r��2��
	sprintf(filename,"%s\\tex_buil2a.bmp",GetDLLPath());
	D3DXCreateTextureFromFileExA(
		d3ddev,filename,
		0,0,0,0,
		D3DFMT_X1R5G5B5,
		D3DPOOL_MANAGED,D3DX_FILTER_POINT,D3DX_FILTER_POINT,
		0,NULL,NULL,
		&ptex_buil2a);

	//��
	sprintf(filename,"%s\\tex_sky.png",GetDLLPath());
	D3DXCreateTextureFromFileExA(
		d3ddev,filename,
		0,0,0,0,
		D3DFMT_X1R5G5B5,
		D3DPOOL_MANAGED,D3DX_FILTER_POINT,D3DX_FILTER_POINT,
		0,NULL,NULL,
		&ptex_sky);
	//�w�i�P
	sprintf(filename,"%s\\tex_bg1.png",GetDLLPath());
	D3DXCreateTextureFromFileExA(
		d3ddev,filename,
		0,0,0,0,
		D3DFMT_A1R5G5B5,
		D3DPOOL_MANAGED,D3DX_FILTER_POINT,D3DX_FILTER_POINT,
		0xFF000000,NULL,NULL,
		&ptex_bg1);
	//�w�i�Q
	sprintf(filename,"%s\\tex_bg2.png",GetDLLPath());
	D3DXCreateTextureFromFileExA(
		d3ddev,filename,
		0,0,0,0,
		D3DFMT_A1R5G5B5,
		D3DPOOL_MANAGED,D3DX_FILTER_POINT,D3DX_FILTER_POINT,
		0xFF000000,NULL,NULL,
		&ptex_bg2);

	free(filename);
}


/*!
*	@brief �`��
*/
DWORD CStage::DrawBack()
{
	if(d3ddev==NULL)return FALSE;

	//�e�N�X�`���A�h���b�V���O���[�h-�J��Ԃ�
	d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSU,D3DTADDRESS_WRAP);
	d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSV,D3DTADDRESS_WRAP);

	DrawJimen();//�n�ʁH
	DrawBuilding1();//�r��(�E)
	DrawBuilding2();//�r��(��)
	DrawBackGround();//���w�i

	//�e�N�X�`���A�h���V���O���[�h-���̂܂�
	d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSU,D3DTADDRESS_CLAMP);
	d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSV,D3DTADDRESS_CLAMP);

	return FALSE;//�ʏ�A�f�t�H���g�`��͍s��Ȃ�
}


//*****************************************************
//�@�n��
//*****************************************************

void CStage::DrawJimen()
{
	//���W�ϊ�-�Ȃ�
	D3DXMATRIX mati;
	D3DXMatrixIdentity(&mati);
	d3ddev->SetTransform(D3DTS_WORLD,&mati);

	//�n��
	d3ddev->SetTexture(0,ptex_jimen);
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,
		vb_jimen,sizeof(MYVERTEX3D));

	//�t�F���X
	d3ddev->SetTexture(0,ptex_fens);
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,
		vb_fens,sizeof(MYVERTEX3D));
	d3ddev->SetTexture(0,NULL);
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,
		vb_fens2[0],sizeof(MYVERTEX3D));
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,
		vb_fens2[1],sizeof(MYVERTEX3D));
}

//*****************************************************
//�@�r���i�E�j
//*****************************************************

void CStage::DrawBuilding1()
{
	int i;

	d3ddev->SetTexture(0,ptex_buil1a);

	//�r���S�̂̂��炵
	D3DXMATRIX mat_buil1trans;
	D3DXMatrixTranslation(&mat_buil1trans,1.5f ,0.0f ,0.0f );

	d3ddev->SetTransform(D3DTS_WORLD,&mat_buil1trans);

	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,//�ǁi���j
		vb_buil1a[0],sizeof(MYVERTEX3D));
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,//�ǁi�w�ʁj
		vb_buil1a[1],sizeof(MYVERTEX3D));
	d3ddev->SetTexture(0,ptex_jimen);
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,//�V��
		vb_buil1b,sizeof(MYVERTEX3D));
	d3ddev->SetTexture(0,ptex_brdura);
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,//�Ŕ�
		vb_buil1c,sizeof(MYVERTEX3D));
	d3ddev->SetTexture(0,NULL);
	for(i=0;i<10;i++){
		d3ddev->SetStreamSource(0, vb_buil1d[i], sizeof(MYVERTEX3D));
		d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);//�Ŕ�����
	}
}

//*****************************************************
//�@�r���i���j
//*****************************************************

void CStage::DrawBuilding2()
{
	//�r���S�̂̂��炵
	D3DXMATRIX mat_buil2trans;
	D3DXMatrixTranslation(&mat_buil2trans,-0.5f ,0.0f ,0.0f );
	d3ddev->SetTransform(D3DTS_WORLD,&mat_buil2trans);

	d3ddev->SetTexture(0,ptex_buil2a);
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,//��
		vb_buil2a,sizeof(MYVERTEX3D));
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,//�w��
		vb_buil2b,sizeof(MYVERTEX3D));
}
//*****************************************************
//�@�w�i
//*****************************************************

void CStage::DrawBackGround()
{
	//���W�ϊ�-�Ȃ�
	D3DXMATRIX mati;
	D3DXMatrixIdentity(&mati);
	d3ddev->SetTransform(D3DTS_WORLD,&mati);

	//�w�i�Q
	d3ddev->SetTexture(0,ptex_bg2);
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,
		vb_bg2,sizeof(MYVERTEX3D));

	//�w�i�P
	d3ddev->SetTexture(0,ptex_bg1);
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,
		vb_bg1,sizeof(MYVERTEX3D));

	//����
	d3ddev->SetTexture(0,ptex_sky);
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,
		vb_sky,sizeof(MYVERTEX3D));
}


