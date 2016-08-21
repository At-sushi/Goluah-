

#include "stage.h"
#include <math.h>


SET_STAGEINFO(CStage)

#define WATERHEIGHT -0.05f

const float g_jimenheightarr[15][16]=//地面の高さ
{
	9,6,1,1,1,1,1,1,1,1,1,1,1,1,4,9,
	9,6,1,1,1,1,1,1,1,1,1,1,1,1,6,9,
	9,6,1,0,1,0,0,0,0,1,0,1,0,0,6,9,
	9,6,1,0,0,0,1,0,0,0,0,1,1,0,6,9,
	9,6,1,0,1,0,0,0,0,0,1,0,0,0,6,9,
	9,6,1,0,0,0,0,1,0,0,0,0,1,0,6,9,
	9,6,1,0,0,0,0,0,0,0,1,1,0,0,6,9,
	9,6,1,0,2,1,2,0,0,2,2,1,0,0,6,9,
	9,6,1,0,2,4,3,2,0,0,0,2,0,0,6,9,
	9,6,1,0,0,2,0,0,0,0,0,0,1,0,6,9,
	9,6,1,0,0,0,0,0,0,9,0,0,0,0,6,9,
	9,6,2,1,1,0,0,0,0,0,1,1,2,2,6,9,
	9,6,3,2,3,1,1,0,0,1,1,3,3,3,4,9,
	9,6,5,5,6,6,5,5,6,6,6,5,5,6,6,9,
	9,8,9,8,9,8,9,8,8,9,8,9,8,9,8,9
};

/*!
*	@brief コンストラクタ
*/
CStage::CStage(SDI_STAGEINFO2 *info)
			: CStageBase(info)
{
	int i;

	ptex_water = NULL;
	ptex_sky = NULL;

	for (i = 0; i < NUM_JIMEN_Z; i++)
		vb_jimen[i] = NULL;
	vb_sky = NULL;
}

/*!
*	@brief デストラクタ
*/
CStage::~CStage()
{
	int i;

	RELEASE(ptex_water);
	RELEASE(ptex_sky);

	for (i = 0; i < NUM_JIMEN_Z; i++)
		RELEASE(vb_jimen[i]);
	RELEASE(vb_sky);
}

/*!
*	@brief 初期化
*/
void CStage::InitializeStage()
{
	InitVrtx();//頂点座標初期化
	LoadTextures();//テクスチャ読込み
}


void CStage::InitVrtx()//頂点座標初期化
{
	int i,j;
	MYVERTEX3D* vtx_tmp = NULL;
	LPDIRECT3DDEVICE9 d3ddev = GetD3DDevice();

	//水面
	vb_water[0].color=
		vb_water[1].color=
		vb_water[2].color=
		vb_water[3].color=0xAAFFFFFF;
	vb_water[0].y=
		vb_water[1].y=
		vb_water[2].y=
		vb_water[3].y=WATERHEIGHT;
	vb_water[0].x=-50;
	vb_water[0].z=50;
	vb_water[1].x=50;
	vb_water[1].z=50;
	vb_water[2].x=-50;
	vb_water[2].z=0.5;
	vb_water[3].x=50;
	vb_water[3].z=0.5;
	vb_water[0].tu=0;
	vb_water[0].tv=0;
	vb_water[1].tu=30;
	vb_water[1].tv=0;
	vb_water[2].tu=0;
	vb_water[2].tv=30;
	vb_water[3].tu=30;
	vb_water[3].tv=30;
	

	srand(timeGetTime());
//	DWORD tmp;

	//地面
	MYVERTEX3D hogehoge[NUM_JIMEN_Z][(NUM_JIMEN_X+1)*2];
		for(i=0;i<NUM_JIMEN_Z;i++){
			for(j=0;j<NUM_JIMEN_X+1;j++){
				if(i==0){
					hogehoge[i][j*2  ].z= -5.0f + ((40.0f)/(float)NUM_JIMEN_Z)*i;
					hogehoge[i][j*2  ].x= -20.0f + (100.0f/(float)NUM_JIMEN_X)*j;
					hogehoge[i][j*2  ].y= (-0.3f)*g_jimenheightarr[i][j] - 0.005f*(rand()%5);
					hogehoge[i][j*2  ].color=0xFFFFFFFF;
				}
				else 
					hogehoge[i][j*2]=hogehoge[i-1][j*2+1];
				hogehoge[i][j*2+1].z= -5.0f + ((55.0f)/(float)NUM_JIMEN_Z)*(i+1);
				hogehoge[i][j*2+1].x= -20.0f + (40.0f/(float)NUM_JIMEN_X)*j;
				hogehoge[i][j*2+1].y= (-0.3f)*g_jimenheightarr[i][j] - 0.005f*(rand()%5);
				hogehoge[i][j*2+1].color=0xFFFFFFFF;

				if(j!=0){
					if(g_jimenheightarr[i][j-1] > g_jimenheightarr[i][j]){
						hogehoge[i][j*2  ].color=0xFFAAAAAA;
						hogehoge[i][j*2+1].color=0xFFAAAAAA;
					}
				}
				if(i==0){
					hogehoge[i][j*2  ].color=0xFFAAAAAA;
					hogehoge[i][j*2+1].color=0xFFAAAAAA;
				}

					
				hogehoge[i][j*2  ].tu= (float)(j%2);
				hogehoge[i][j*2  ].tv= (float)(i%2);
				hogehoge[i][j*2+1].tu= (float)(j%2);
				hogehoge[i][j*2+1].tv= (float)((i+1)%2);
				//テクスチャ座標
				/* no texture */
			}

			if ( SUCCEEDED(d3ddev->CreateVertexBuffer(sizeof(MYVERTEX3D) * ((NUM_JIMEN_X+1)*2), 0, FVF_3DVERTEX,
				D3DPOOL_MANAGED, &vb_jimen[i], NULL)) &&
				SUCCEEDED(vb_jimen[i]->Lock(0, 0, (void**)&vtx_tmp, 0)) )
			{
				memcpy(vtx_tmp, hogehoge[i], sizeof(hogehoge[i]));
				vb_jimen[i]->Unlock();
			}
		}

	//空
		if (SUCCEEDED(d3ddev->CreateVertexBuffer(sizeof(MYVERTEX3D) * 4, 0, FVF_3DVERTEX, D3DPOOL_MANAGED, &vb_sky, NULL)) &&
		 SUCCEEDED(vb_sky->Lock(0, 0, (void**)&vtx_tmp, 0)) )
	{
		vtx_tmp[0].color=
			vtx_tmp[1].color=
			vtx_tmp[2].color=
			vtx_tmp[3].color=0xFFFFFFFF;
		vtx_tmp[0].z=
			vtx_tmp[1].z=
			vtx_tmp[2].z=
			vtx_tmp[3].z=45.0f;
		vtx_tmp[0].x=-30;
		vtx_tmp[0].y=-20;
		vtx_tmp[1].x=30;
		vtx_tmp[1].y=-20;
		vtx_tmp[2].x=-30;
		vtx_tmp[2].y=0;
		vtx_tmp[3].x=30;
		vtx_tmp[3].y=0;
		vtx_tmp[0].tu=0;
		vtx_tmp[0].tv=0;
		vtx_tmp[1].tu=1;
		vtx_tmp[1].tv=0;
		vtx_tmp[2].tu=0;
		vtx_tmp[2].tv=1;
		vtx_tmp[3].tu=1;
		vtx_tmp[3].tv=1;

		vb_sky->Unlock();
	}
}

void CStage::LoadTextures()//テクスチャ読込み
{
	LPDIRECT3DDEVICE9 d3ddev = GetD3DDevice();

	char *filename=(char*)malloc(256);
	if(filename==NULL)return;
	
	//水面
	ptex_water=NULL;
	sprintf(filename,"%s\\tex_water.bmp",GetDLLPath());
	D3DXCreateTextureFromFileA(
		d3ddev,
		filename,
		&ptex_water);
	//空
	ptex_sky=NULL;
	sprintf(filename,"%s\\tex_sky.png",GetDLLPath());
	D3DXCreateTextureFromFileExA(
		d3ddev,filename,
		0,0,0,0,
		D3DFMT_X1R5G5B5,
		D3DPOOL_MANAGED,D3DX_FILTER_POINT,D3DX_FILTER_POINT,
		0,NULL,NULL,
		&ptex_sky);

	free(filename);
}

/*!
*	@brief 描画
*/
DWORD CStage::DrawBack()
{
	LPDIRECT3DDEVICE9 d3ddev = GetD3DDevice();
	if(d3ddev)
	{
		//テクスチャアドレッシングモード-繰り返し
		d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

		d3ddev->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(5,0,20),1.0f,0);

		//描画
		DrawJimen();
		DrawWater();
		DrawSky();

		//テクスチャアドレシングモード - 元に戻す
		d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	}

	return FALSE;//通常、デフォルト描画は行わない
}


void CStage::DrawWater()
{
	LPDIRECT3DDEVICE9 d3ddev = GetD3DDevice();

	//座標変換-なし
	D3DXMATRIX mati;
	D3DXMatrixIdentity(&mati);
	d3ddev->SetTransform(D3DTS_WORLD,&mati);

	d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
	
	//水面
	d3ddev->SetTexture(0,ptex_water);
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,
		vb_water,sizeof(MYVERTEX3D));

	d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}

void CStage::DrawSky()
{
	LPDIRECT3DDEVICE9 d3ddev = GetD3DDevice();

	//座標変換-なし
	D3DXMATRIX mati;
	D3DXMatrixIdentity(&mati);
	d3ddev->SetTransform(D3DTS_WORLD,&mati);

	d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
	
	//水面
	d3ddev->SetTexture(0,ptex_sky);
	d3ddev->SetStreamSource(0, vb_sky,0,sizeof(MYVERTEX3D));
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

	d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}

void CStage::DrawJimen()
{
	LPDIRECT3DDEVICE9 d3ddev = GetD3DDevice();

	//座標変換-なし
	D3DXMATRIX mati;
	D3DXMatrixIdentity(&mati);
	d3ddev->SetTransform(D3DTS_WORLD,&mati);

	//地面
	d3ddev->SetTexture(0,NULL);
	for(int i=0;i<NUM_JIMEN_Z;i++){
		d3ddev->SetStreamSource(0, vb_jimen[i],0,sizeof(MYVERTEX3D));
		d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,NUM_JIMEN_X*2);
	}
}

//*******************************************************
//　動き
//*******************************************************
DWORD CStage::Action()
{
	for(int i=0;i<4;i++){
		vb_water[i].tu +=0.0005f;
		vb_water[i].tv +=0.0002f;
	}
	return TRUE;
}