
#include "stage.h"


SET_STAGEINFO(CStage)

//****************************************************************
//ステージの初期化
//****************************************************************
CStage::CStage(SDI_STAGEINFO2 *info) : CStageBase(info)
{
}

void CStage::InitializeStage()
{
	InitVrtx();//頂点座標初期化
	LoadTextures();//テクスチャ読込み
}

void CStage::InitVrtx()//頂点座標初期化
{
	int i,j;
	float x0,y0;
	BYTE alpha;
	MYVERTEX3D* vtx_tmp = NULL;
	LPDIRECT3DDEVICE8 d3ddev = GetD3DDevice();
	if(!d3ddev)return;

	vb_moon = NULL;
	vb_city = NULL;
	vb_ground = NULL;
	for(i=0;i<NUMDIV_ROADY;i++)
		vb_road[i] = NULL;
	for(i=0;i<NUMDIV_JCTY;i++)
		vb_jct[i] = NULL;
	vb_gurd = NULL;
	vb_brd1a = NULL;
	vb_brd1b = NULL;
	vb_gurdpoll = NULL;

	//月
	if ( SUCCEEDED(d3ddev->CreateVertexBuffer(sizeof(MYVERTEX3D) * 4, 0, FVF_3DVERTEX, D3DPOOL_MANAGED, &vb_moon)) &&
		 SUCCEEDED(vb_moon->Lock(0, 0, (BYTE**)&vtx_tmp, 0)) )
	{
		vtx_tmp[0].color=vtx_tmp[1].color=
			vtx_tmp[2].color=vtx_tmp[3].color=0xFFFFFFFF;
		vtx_tmp[0].tu=0;
		vtx_tmp[0].tv=0;
		vtx_tmp[1].tu=0;
		vtx_tmp[1].tv=1;
		vtx_tmp[2].tu=1;
		vtx_tmp[2].tv=0;
		vtx_tmp[3].tu=1;
		vtx_tmp[3].tv=1;
		vtx_tmp[0].x=-10;
		vtx_tmp[0].y=-13;
		vtx_tmp[1].x=-10;
		vtx_tmp[1].y=-3;
		vtx_tmp[2].x= 10;
		vtx_tmp[2].y=-13;
		vtx_tmp[3].x= 10;
		vtx_tmp[3].y=-3;
		vtx_tmp[0].z=vtx_tmp[1].z=
			vtx_tmp[2].z=vtx_tmp[3].z=30;

		vb_moon->Unlock();
	}
	
	//街
	if ( SUCCEEDED(d3ddev->CreateVertexBuffer(sizeof(MYVERTEX3D) * 4, 0, FVF_3DVERTEX, D3DPOOL_MANAGED, &vb_city)) &&
		 SUCCEEDED(vb_city->Lock(0, 0, (BYTE**)&vtx_tmp, 0)) )
	{
		vtx_tmp[0].color=vtx_tmp[1].color=
			vtx_tmp[2].color=vtx_tmp[3].color=0xFFFFFFFF;
		vtx_tmp[0].tu=0;
		vtx_tmp[0].tv=0;
		vtx_tmp[1].tu=0;
		vtx_tmp[1].tv=1;
		vtx_tmp[2].tu=1;
		vtx_tmp[2].tv=0;
		vtx_tmp[3].tu=1;
		vtx_tmp[3].tv=1;
		vtx_tmp[0].x=-6;
		vtx_tmp[0].y=-6;
		vtx_tmp[1].x=-6;
		vtx_tmp[1].y= 0;
		vtx_tmp[2].x= 6;
		vtx_tmp[2].y=-6;
		vtx_tmp[3].x= 6;
		vtx_tmp[3].y= 0;
		vtx_tmp[0].z=vtx_tmp[1].z=
			vtx_tmp[2].z=vtx_tmp[3].z=20;

		vb_city->Unlock();
	}

	//地面
	if ( SUCCEEDED(d3ddev->CreateVertexBuffer(sizeof(MYVERTEX3D) * 4, 0, FVF_3DVERTEX, D3DPOOL_MANAGED, &vb_ground)) &&
		SUCCEEDED(vb_ground->Lock(0, 0, (BYTE**)&vtx_tmp, 0)) )
	{
		vtx_tmp[0].color=vtx_tmp[1].color=
			vtx_tmp[2].color=vtx_tmp[3].color=0xFF000000;
		vtx_tmp[0].tu=0;
		vtx_tmp[0].tv=1;
		vtx_tmp[1].tu=0;
		vtx_tmp[1].tv=0;
		vtx_tmp[2].tu=1;
		vtx_tmp[2].tv=1;
		vtx_tmp[3].tu=1;
		vtx_tmp[3].tv=0;
		vtx_tmp[0].x=-10;
		vtx_tmp[0].z=  2;
		vtx_tmp[1].x=-10;
		vtx_tmp[1].z= 20;
		vtx_tmp[2].x= 10;
		vtx_tmp[2].z=  2;
		vtx_tmp[3].x= 10;
		vtx_tmp[3].z= 20;
		vtx_tmp[0].y=vtx_tmp[1].y=
			vtx_tmp[2].y=vtx_tmp[3].y=0.02f;

		vb_ground->Unlock();
	}

	//道路
/*	vb_road[1].color=vb_road[3].color=0xFF000000;
	vb_road[2].color=vb_road[4].color=0xFFFFFFFF;
	vb_road[1].tu=0;
	vb_road[1].tv=10;
	vb_road[2].tu=0;
	vb_road[2].tv=0;
	vb_road[3].tu=1;
	vb_road[3].tv=10;
	vb_road[4].tu=1;
	vb_road[4].tv=0;
	vb_road[1].x=-1;
	vb_road[1].z=20;
	vb_road[2].x=-3.1f;
	vb_road[2].z= 6;
	vb_road[3].x= 1;
	vb_road[3].z=20;
	vb_road[4].x= 3.1f;
	vb_road[4].z= 6;
	vb_road[1].y=vb_road[2].y=
		vb_road[3].y=vb_road[4].y=0;
	vb_road[0].color=0xFF777777;
	vb_road[0].tu=0.5f;
	vb_road[0].tv=(vb_road[1].tv+vb_road[4].tv)*0.5f;
	vb_road[0].x =0;
	vb_road[0].y =0;
	vb_road[0].z =(vb_road[1].z +vb_road[4].z )*0.5f;
	vtx_tmp=vb_road[4];
	vb_road[4]=vb_road[3];
	vb_road[3]=vtx_tmp;*/
/*	for(i=0;i<NUMDIV_ROADY;i++){
		for(j=0;j<=NUMDIV_ROADX;j++){
			alpha=256-(250/NUMDIV_ROADY)*j;
			vb_road[i][j*2].color=
				vb_road[j][j*2+1].color=
					0x00FFFFFF;//+((alpha*256*256*256)&0xFF000000);
			vb_road[i][j*2  ].tu=(1.0f/(float)NUMDIV_ROADX)*j;
			vb_road[i][j*2+1].tu=(1.0f/(float)NUMDIV_ROADX)*(j+1);
			vb_road[i][j*2  ].tv=(float)(i%2);
			vb_road[i][j*2+1].tv=(float)(i%2+1);
			vb_road[i][j*2  ].z=6.0f+(5.0f/(float)NUMDIV_ROADY)*i;
			vb_road[i][j*2+1].z=6.0f+(5.0f/(float)NUMDIV_ROADY)*(i+1);
			x0=-5.0f+(10.0f/(float)NUMDIV_ROADX)*j;
			vb_road[i][j*2  ].x= (1.0f/3.0f-x0)/20.0f*vb_road[i][j*2  ].z+x0;
			vb_road[i][j*2+1].x=-(1.0f/3.0f-x0)/20.0f*vb_road[i][j*2+1].z-x0;
			vb_road[i][j*2  ].y=0;
			vb_road[i][j*2+1].y=0;
		}
	}*/
	for(i=0;i<NUMDIV_ROADY;i++){
		if ( SUCCEEDED(d3ddev->CreateVertexBuffer(sizeof(MYVERTEX3D) * ((NUMDIV_ROADX+1)*2), 0, FVF_3DVERTEX, D3DPOOL_MANAGED, &vb_road[i])) &&
			SUCCEEDED(vb_road[i]->Lock(0, 0, (BYTE**)&vtx_tmp, 0)) )
		{
			for(j=0;j<=NUMDIV_ROADX;j++){
				vtx_tmp[j*2].color=
					vtx_tmp[j*2+1].color=
						0xFFFFFFFF;
				vtx_tmp[j*2].tu=
					vtx_tmp[j*2+1].tu=
						(1.0f/(float)NUMDIV_ROADX)*j;
				vtx_tmp[j*2  ].tv=(float)(i%2)*2;//(1.0f/(float)NUMDIV_ROADY)*(NUMDIV_ROADY-i);
				vtx_tmp[j*2+1].tv=(float)((i+1)%2)*2;//(1.0f/(float)NUMDIV_ROADY)*(NUMDIV_ROADY-i-1);
				vtx_tmp[j*2  ].z=6.0f+(12.0f/(float)NUMDIV_ROADY)*i;
				vtx_tmp[j*2+1].z=6.0f+(12.0f/(float)NUMDIV_ROADY)*(i+1);
				x0=-5.0f+(10.0f/(float)NUMDIV_ROADX)*j;
				vtx_tmp[j*2  ].x=(1.0f/3.0f-x0)/20.0f*vtx_tmp[j*2  ].z+x0;
				vtx_tmp[j*2+1].x=(1.0f/3.0f-x0)/20.0f*vtx_tmp[j*2+1].z+x0;
				vtx_tmp[j*2  ].y=0;
				vtx_tmp[j*2+1].y=0;
			}

			vb_road[i]->Unlock();
		}
	}

	//交差点
/*	vb_jct[1].color=vb_jct[2].color=
		vb_jct[3].color=vb_jct[4].color=0xFFFFFFFF;
	vb_jct[1].tu=0;
	vb_jct[1].tv=1;
	vb_jct[2].tu=0;
	vb_jct[2].tv=0;
	vb_jct[3].tu=1;
	vb_jct[3].tv=1;
	vb_jct[4].tu=1;
	vb_jct[4].tv=0;
	vb_jct[1].x=-4.075f;
	vb_jct[1].z=-0.5;
	vb_jct[2].x=-3.1f;
	vb_jct[2].z= 6;
	vb_jct[3].x= 4.075f;
	vb_jct[3].z=-0.5;	
	vb_jct[4].x= 3.1f;
	vb_jct[4].z= 6;
	vb_jct[1].y=vb_jct[2].y=
		vb_jct[3].y=vb_jct[4].y=0;
	vb_jct[0].color=0xFFFFFFFF;
	vb_jct[0].tu = vb_jct[0].tv = 0.5f;
	vb_jct[0].x = (vb_jct[1].x+vb_jct[4].x)*0.5f;
	vb_jct[0].y = 0;
	vb_jct[0].z = (vb_jct[1].z+vb_jct[4].z)*0.5f;
	vtx_tmp=vb_jct[3];
	vb_jct[3]=vb_jct[4];
	vb_jct[4]=vtx_tmp;*/
	for(i=0;i<NUMDIV_JCTY;i++){
		if ( SUCCEEDED(d3ddev->CreateVertexBuffer(sizeof(MYVERTEX3D) * ((NUMDIV_JCTX+1)*2), 0, FVF_3DVERTEX, D3DPOOL_MANAGED, &vb_jct[i])) &&
			SUCCEEDED(vb_jct[i]->Lock(0, 0, (BYTE**)&vtx_tmp, 0)) )
		{
			for(j=0;j<=NUMDIV_JCTX;j++){
				vtx_tmp[j*2].color=
					vtx_tmp[j*2+1].color=
						0xFFFFFFFF;
				vtx_tmp[j*2].tu=
					vtx_tmp[j*2+1].tu=
						(1.0f/(float)NUMDIV_JCTX)*j;
				vtx_tmp[j*2  ].tv=(1.0f/(float)NUMDIV_JCTY)*(NUMDIV_JCTY-i);
				vtx_tmp[j*2+1].tv=(1.0f/(float)NUMDIV_JCTY)*(NUMDIV_JCTY-i-1);
				vtx_tmp[j*2  ].z=-0.5f+(6.5f/(float)NUMDIV_JCTY)*i;
				vtx_tmp[j*2+1].z=-0.5f+(6.5f/(float)NUMDIV_JCTY)*(i+1);
				x0=-5.0f+(10.0f/(float)NUMDIV_JCTX)*j;
				vtx_tmp[j*2  ].x=(1.0f/3.0f-x0)/20.0f*vtx_tmp[j*2  ].z+x0;
				vtx_tmp[j*2+1].x=(1.0f/3.0f-x0)/20.0f*vtx_tmp[j*2+1].z+x0;
				vtx_tmp[j*2  ].y=0;
				vtx_tmp[j*2+1].y=0;
			}

			vb_jct[i]->Unlock();
		}
	}

	//ガードレール
	if ( SUCCEEDED(d3ddev->CreateVertexBuffer(sizeof(MYVERTEX3D) * ((NUM_GURD+1)*2), 0, FVF_3DVERTEX, D3DPOOL_MANAGED, &vb_gurd)) &&
		SUCCEEDED(vb_gurd->Lock(0, 0, (BYTE**)&vtx_tmp, 0)) )
	{
		for(i=0;i<(NUM_GURD+1)*2;i++){
			vtx_tmp[i].color=0xFFFFFFFF;
			if(i%2==0){
				vtx_tmp[i].y=-0.9f;
				vtx_tmp[i].tv=0;
			}
			else{
				vtx_tmp[i].y=-0.45f;
				vtx_tmp[i].tv=1;
			}
			if((i/2)%2==0)
				vtx_tmp[i].tu=0;
			else
				vtx_tmp[i].tu=1;
		}
		vtx_tmp[0].color=vtx_tmp[1].color=0xFF000000;
		vtx_tmp[0].z=15;
		vtx_tmp[1*2].z=6;
		vtx_tmp[2*2].z=5;
		vtx_tmp[3*2].z=4;
		vtx_tmp[4*2].z=3;
		vtx_tmp[1].z=vtx_tmp[0].z;
		vtx_tmp[3].z=vtx_tmp[2].z;
		vtx_tmp[5].z=vtx_tmp[4].z;
		vtx_tmp[7].z=vtx_tmp[6].z;
		vtx_tmp[9].z=vtx_tmp[8].z;
		for(i=0;i<(NUM_GURD+1)*2;i++){
			vtx_tmp[i].x =	(1.0f/3.0f-5.0f)/20.0f*vtx_tmp[i].z+5.0f;
			vtx_tmp[i].x /= 1.9f;
			vtx_tmp[i].y = vtx_tmp[i].y*(vtx_tmp[i].x/5.0f);
			//(1.0f/3.0f-vb_gurd[i].y)/20.0f*vb_gurd[i].z+vb_gurd[i].y;
	//		if(i%2==1)vb_gurd[i].y /= 2;
		}

		vb_gurd->Unlock();
	}

	//看板支柱
	if ( SUCCEEDED(d3ddev->CreateVertexBuffer(sizeof(MYVERTEX3D) * 4, 0, FVF_3DVERTEX, D3DPOOL_MANAGED, &vb_brd1a)) &&
		SUCCEEDED(vb_brd1a->Lock(0, 0, (BYTE**)&vtx_tmp, 0)) )
	{
		vtx_tmp[0].color=0xFFFFFFFF;
		vtx_tmp[1].color=vtx_tmp[2].color=vtx_tmp[3].color=0xFF777777;
		vtx_tmp[0].tv=0;
		vtx_tmp[0].tu=0;
		vtx_tmp[1].tv=0;
		vtx_tmp[1].tu=1;
		vtx_tmp[2].tv=1;
		vtx_tmp[2].tu=0;
		vtx_tmp[3].tv=1;
		vtx_tmp[3].tu=1;
		vtx_tmp[0].x=-2.5f;
		vtx_tmp[0].y=-3;
		vtx_tmp[1].x=-2.5f+0.2f;
		vtx_tmp[1].y=-3;
		vtx_tmp[2].x=-2.5f;
		vtx_tmp[2].y= 0;
		vtx_tmp[3].x=-2.5f+0.2f;
		vtx_tmp[3].y= 0;
		vtx_tmp[0].z=vtx_tmp[1].z=
			vtx_tmp[2].z=vtx_tmp[3].z=3;

		vb_brd1a->Unlock();
	}
	//看板表
	if ( SUCCEEDED(d3ddev->CreateVertexBuffer(sizeof(MYVERTEX3D) * 4, 0, FVF_3DVERTEX, D3DPOOL_MANAGED, &vb_brd1b)) &&
		SUCCEEDED(vb_brd1b->Lock(0, 0, (BYTE**)&vtx_tmp, 0)) )
	{
		vtx_tmp[0].color=vtx_tmp[1].color=
			vtx_tmp[2].color=vtx_tmp[3].color=0xFF666666;
		vtx_tmp[0].tv=0;
		vtx_tmp[0].tu=0;
		vtx_tmp[1].tv=0;
		vtx_tmp[1].tu=1;
		vtx_tmp[2].tv=1;
		vtx_tmp[2].tu=0;
		vtx_tmp[3].tv=1;
		vtx_tmp[3].tu=1;
		vtx_tmp[0].x=-2.5f+0.2f;
		vtx_tmp[0].y=-3;
		vtx_tmp[1].x=-2.5f+0.2f+1.3f;
		vtx_tmp[1].y=-3;
		vtx_tmp[2].x=-2.5f+0.2f;
		vtx_tmp[2].y=-3+0.9f;
		vtx_tmp[3].x=-2.5f+0.2f+1.3f;
		vtx_tmp[3].y=-3+0.9f;
		vtx_tmp[0].z=vtx_tmp[1].z=
			vtx_tmp[2].z=vtx_tmp[3].z=3;

		vb_brd1b->Unlock();
	}
	
	//ガードレールの柱
	if ( SUCCEEDED(d3ddev->CreateVertexBuffer(sizeof(MYVERTEX3D) * (6*NUM_GURDPOLL), 0, FVF_3DVERTEX, D3DPOOL_MANAGED, &vb_gurdpoll)) &&
		SUCCEEDED(vb_gurdpoll->Lock(0, 0, (BYTE**)&vtx_tmp, 0)) )
	{
		for(i=0;i<NUM_GURDPOLL;i++){
			alpha= (BYTE)( (255/(NUM_GURDPOLL*1.5))*(NUM_GURDPOLL-i) );
			vtx_tmp[i*6+0].color=
				vtx_tmp[i*6+1].color=
				vtx_tmp[i*6+2].color=
				vtx_tmp[i*6+3].color=0xFF000000 + ((alpha*256*256)&0x00FF0000) + ((alpha*256)&0x0000FF00) + ((alpha)&0x000000FF);
			vtx_tmp[i*6+0].z=
				vtx_tmp[i*6+1].z=
				vtx_tmp[i*6+2].z=
				vtx_tmp[i*6+3].z=4.0f+0.65f*i;
			vtx_tmp[i*6+0].tu=vtx_tmp[i*6+0].tv=
				vtx_tmp[i*6+1].tu=vtx_tmp[i*6+1].tv=
				vtx_tmp[i*6+2].tu=vtx_tmp[i*6+2].tv=
				vtx_tmp[i*6+3].tu=vtx_tmp[i*6+3].tv=0;
			vtx_tmp[i*6+0].y=
				vtx_tmp[i*6+3].y=-0.3f + 0.015f*i;
			vtx_tmp[i*6+1].y=
				vtx_tmp[i*6+2].y=0;
			x0 = (1.0f/3.0f-5.0f)/20.0f*vtx_tmp[i*6+0].z+5.0f;
			x0 *= 0.55f;
			vtx_tmp[i*6+0].x=
				vtx_tmp[i*6+1].x=x0+0.10f;
			vtx_tmp[i*6+2].x=
				vtx_tmp[i*6+3].x=x0+0.05f;
			vtx_tmp[i*6+4]=vtx_tmp[i*6+0];
			vtx_tmp[i*6+5]=vtx_tmp[i*6+2];
		}

		vb_gurdpoll->Unlock();
	}

	//信号
	vb_tlight[0].color=vb_tlight[1].color=
		vb_tlight[2].color=vb_tlight[3].color=0xFF666666;
	vb_tlight[0].tv=0;
	vb_tlight[0].tu=0;
	vb_tlight[1].tv=0;
	vb_tlight[1].tu=1;
	vb_tlight[2].tv=1;
	vb_tlight[2].tu=0;
	vb_tlight[3].tv=1;
	vb_tlight[3].tu=1;
	vb_tlight[0].x=-2.4f;
	vb_tlight[0].y=-2.1f;
	vb_tlight[1].x=-2.485f+1.0f;
	vb_tlight[1].y=-2.1f;
	vb_tlight[2].x=-2.4f;
	vb_tlight[2].y=-2.1f+0.6f;
	vb_tlight[3].x=-2.485f+1.0f;
	vb_tlight[3].y=-2.1f+0.6f;
	vb_tlight[0].z=vb_tlight[1].z=
		vb_tlight[2].z=vb_tlight[3].z=3;

	//街灯
	for(i=0;i<NUM_LIGHT;i++){
	//	alpha= (BYTE)( (255/(NUM_GURDPOLL*1.5))*(NUM_GURDPOLL-i) );
		vb_light[i][0].color=
			vb_light[i][1].color=
			vb_light[i][2].color=
			vb_light[i][3].color=0xFFFFFFFF;//0xFF000000 + ((alpha*256*256)&0x00FF0000) + ((alpha*256)&0x0000FF00) + ((alpha)&0x000000FF);
		vb_light[i][0].tu=0;
		vb_light[i][0].tv=0;
		vb_light[i][1].tu=0;
		vb_light[i][1].tv=1;
		vb_light[i][2].tu=1;
		vb_light[i][2].tv=0;
		vb_light[i][3].tu=1;
		vb_light[i][3].tv=1;
		vb_light[i][0].z=
			vb_light[i][1].z=
			vb_light[i][2].z=
			vb_light[i][3].z=4.0f+1.5f*i;
		x0 = (1.0f/3.0f-5.0f)/20.0f*vb_light[i][0].z+5.0f;
		x0 *= 0.64f;
		y0 = -2.8f + 0.26f*i;
		vb_light[i][0].x=x0-0.2f;
		vb_light[i][0].y=y0;
		vb_light[i][1].x=x0-0.2f;
		vb_light[i][1].y=0.0f;
		vb_light[i][2].x=x0+0.2f;
		vb_light[i][2].y=y0;
		vb_light[i][3].x=x0+0.2f;
		vb_light[i][3].y=0.0f;
	}

	//看板２（後ろ向き）
	//看板支柱
	vb_brd2a[0].color=0xFFFFFFFF;
	vb_brd2a[1].color=vb_brd2a[2].color=vb_brd2a[3].color=0xFF777777;
	vb_brd2a[0].tv=0;
	vb_brd2a[0].tu=0;
	vb_brd2a[1].tv=0;
	vb_brd2a[1].tu=1;
	vb_brd2a[2].tv=1;
	vb_brd2a[2].tu=0;
	vb_brd2a[3].tv=1;
	vb_brd2a[3].tu=1;
	vb_brd2a[0].x=-2.5f;
	vb_brd2a[0].y=-3;
	vb_brd2a[1].x=-2.5f+0.2f;
	vb_brd2a[1].y=-3;
	vb_brd2a[2].x=-2.5f;
	vb_brd2a[2].y= 0;
	vb_brd2a[3].x=-2.5f+0.2f;
	vb_brd2a[3].y= 0;
	vb_brd2a[0].z=vb_brd2a[1].z=
		vb_brd2a[2].z=vb_brd2a[3].z=6;
	//看板裏
	vb_brd2b[0].color=vb_brd2b[1].color=
		vb_brd2b[2].color=vb_brd2b[3].color=0xFF666666;
	vb_brd2b[0].tv=0;
	vb_brd2b[0].tu=0;
	vb_brd2b[1].tv=0;
	vb_brd2b[1].tu=1;
	vb_brd2b[2].tv=1;
	vb_brd2b[2].tu=0;
	vb_brd2b[3].tv=1;
	vb_brd2b[3].tu=1;
	vb_brd2b[0].x=-2.5f;
	vb_brd2b[0].y=-3;
	vb_brd2b[1].x=-2.5f+0.9f;
	vb_brd2b[1].y=-3;
	vb_brd2b[2].x=-2.5f;
	vb_brd2b[2].y=-3+0.9f;
	vb_brd2b[3].x=-2.5f+0.9f;
	vb_brd2b[3].y=-3+0.9f;
	vb_brd2b[0].z=vb_brd2b[1].z=
		vb_brd2b[2].z=vb_brd2b[3].z=6;
	for(i=0;i<4;i++){
		vb_brd2a[i].x*=-1;
		vb_brd2b[i].x*=-1;
		vb_brd2a[i].y*=0.7f;
		vb_brd2b[i].y*=0.7f;
	}

	//手前ガードレール
	vb_maegurd[0].color=
		vb_maegurd[1].color=
		vb_maegurd[2].color=
		vb_maegurd[3].color=0xFFFFFFFF;
	vb_maegurd[0].x=vb_maegurd[2].x=-5.0f;
	vb_maegurd[1].x=vb_maegurd[3].x=-3.0f;
	vb_maegurd[0].y=vb_maegurd[1].y=-0.4f;
	vb_maegurd[2].y=vb_maegurd[3].y=-0.15f;
	vb_maegurd[0].z=
		vb_maegurd[1].z=
		vb_maegurd[2].z=
		vb_maegurd[3].z=1.5f;
	vb_maegurd[0].tv=0;
	vb_maegurd[0].tu=0;
	vb_maegurd[1].tv=0;
	vb_maegurd[1].tu=1;
	vb_maegurd[2].tv=1;
	vb_maegurd[2].tu=0;
	vb_maegurd[3].tv=1;
	vb_maegurd[3].tu=1;

	//手前ガードレール支柱
	vb_maegurdpoll[0].color=
		vb_maegurdpoll[1].color=
		vb_maegurdpoll[2].color=
		vb_maegurdpoll[3].color=0xFFEEEEEE;
	vb_maegurdpoll[0].x=vb_maegurdpoll[2].x=-3.3f;
	vb_maegurdpoll[1].x=vb_maegurdpoll[3].x=-3.2f;
	vb_maegurdpoll[0].y=vb_maegurdpoll[1].y=-0.43f;
	vb_maegurdpoll[2].y=vb_maegurdpoll[3].y=-0.0f;
	vb_maegurdpoll[0].z=
		vb_maegurdpoll[1].z=
		vb_maegurdpoll[2].z=
		vb_maegurdpoll[3].z=1.52f;
	vb_maegurdpoll[0].tv=0;
	vb_maegurdpoll[0].tu=0;
	vb_maegurdpoll[1].tv=0;
	vb_maegurdpoll[1].tu=1;
	vb_maegurdpoll[2].tv=1;
	vb_maegurdpoll[2].tu=0;
	vb_maegurdpoll[3].tv=1;
	vb_maegurdpoll[3].tu=1;

	//★
	for(i=0;i<NUM_STARS;i++){
		vb_stars[i].color=0xFFFFFF77;
		vb_stars[i].tu=vb_stars[i].tu=0;
		vb_stars[i].x=-20 + (40.0f/100.0f)*(rand()%100);
		vb_stars[i].y=(-15.0f/100.0f)*(rand()%100);
		vb_stars[i].z=33;
	}

	//お空
	vb_sky[0].z=
		vb_sky[1].z=
		vb_sky[2].z=
		vb_sky[3].z=35;
	vb_sky[0].tu=vb_sky[0].tv=
		vb_sky[1].tu=vb_sky[1].tv=
		vb_sky[2].tu=vb_sky[2].tv=
		vb_sky[3].tu=vb_sky[3].tv=0;
	vb_sky[0].color=
		vb_sky[1].color=0xFF000000;
	vb_sky[2].color=
		vb_sky[3].color=0xFF333333;
	vb_sky[0].x=-20;
	vb_sky[0].y=-20;
	vb_sky[1].x= 20;
	vb_sky[1].y=-20;
	vb_sky[2].x=-20;
	vb_sky[2].y=  0;
	vb_sky[3].x= 20;
	vb_sky[3].y=  0;
}

void CStage::LoadTextures()//テクスチャ読込み
{
	LPDIRECT3DDEVICE8 d3ddev = GetD3DDevice();
	if(!d3ddev)return;

	ptex_moon=NULL;
	ptex_city=NULL;
	ptex_jct=NULL;
	ptex_road=NULL;
	ptex_gurd=NULL;
	ptex_brd1a=NULL;
	ptex_brd1b=NULL;
	ptex_tlight=NULL;

	char *filename=(char*)malloc(256);
	if(filename==NULL)return;

	//月背景
	sprintf(filename,"%s\\tex_moon.png",GetDLLPath());
	D3DXCreateTextureFromFileExA(
		d3ddev,filename,
		0,0,0,0,
		D3DFMT_A1R5G5B5,
		D3DPOOL_MANAGED,D3DX_FILTER_POINT,D3DX_FILTER_POINT,
		0xFF000000,NULL,NULL,
		&ptex_moon);

	//街シルエット
	sprintf(filename,"%s\\tex_city.png",GetDLLPath());
	D3DXCreateTextureFromFileExA(
		d3ddev,filename,
		0,0,0,0,
		D3DFMT_A1R5G5B5,
		D3DPOOL_MANAGED,D3DX_FILTER_POINT,D3DX_FILTER_POINT,
		0xFF000000,NULL,NULL,
		&ptex_city);
	
	//交差点
	sprintf(filename,"%s\\tex_jct.png",GetDLLPath());
	D3DXCreateTextureFromFileExA(
		d3ddev,filename,
		0,0,0,0,
		D3DFMT_X1R5G5B5,
		D3DPOOL_MANAGED,D3DX_FILTER_POINT,D3DX_FILTER_POINT,
		0,NULL,NULL,
		&ptex_jct);
	
	//道路
	sprintf(filename,"%s\\tex_road.bmp",GetDLLPath());
	D3DXCreateTextureFromFileA(
		d3ddev,
		filename,
		&ptex_road);
	
	//ガードレール
	sprintf(filename,"%s\\tex_gurd.bmp",GetDLLPath());
	D3DXCreateTextureFromFileA(
		d3ddev,
		filename,
		&ptex_gurd);

	//看板支柱
	sprintf(filename,"%s\\tex_sicyu.bmp",GetDLLPath());
	D3DXCreateTextureFromFileExA(
		d3ddev,filename,
		0,0,0,0,
		D3DFMT_A1R5G5B5,
		D3DPOOL_MANAGED,D3DX_FILTER_POINT,D3DX_FILTER_POINT,
		0xFF000000,NULL,NULL,
		&ptex_brd1a);
	//看板表
	sprintf(filename,"%s\\tex_bord.png",GetDLLPath());
	D3DXCreateTextureFromFileExA(
		d3ddev,filename,
		0,0,0,0,
		D3DFMT_X1R5G5B5,
		D3DPOOL_MANAGED,D3DX_FILTER_POINT,D3DX_FILTER_POINT,
		0,NULL,NULL,
		&ptex_brd1b);
	
	//信号
	sprintf(filename,"%s\\tex_tlight.bmp",GetDLLPath());
	D3DXCreateTextureFromFileExA(
		d3ddev,filename,
		0,0,0,0,
		D3DFMT_A1R5G5B5,
		D3DPOOL_MANAGED,D3DX_FILTER_POINT,D3DX_FILTER_POINT,
		0xFFFFFFFF,NULL,NULL,
		&ptex_tlight);

	//街灯
	sprintf(filename,"%s\\tex_light.bmp",GetDLLPath());
	D3DXCreateTextureFromFileExA(
		d3ddev,filename,
		0,0,0,0,
		D3DFMT_A1R5G5B5,
		D3DPOOL_MANAGED,D3DX_FILTER_POINT,D3DX_FILTER_POINT,
		0xFFFFFFFF,NULL,NULL,
		&ptex_light);

	//看板裏向き
	sprintf(filename,"%s\\tex_bord2.bmp",GetDLLPath());
	D3DXCreateTextureFromFileExA(
		d3ddev,filename,
		0,0,0,0,
		D3DFMT_A1R5G5B5,
		D3DPOOL_MANAGED,D3DX_FILTER_POINT,D3DX_FILTER_POINT,
		0xFFFFFFFF,NULL,NULL,
		&ptex_brd2);

	free(filename);
}

//****************************************************************
//ステージ後始末
//****************************************************************
CStage::~CStage()
{
	UnloadTextures();//テクスチャ開放
}

void CStage::UnloadTextures()
{
	int i;

	RELEASE(ptex_moon);
	RELEASE(ptex_city);
	RELEASE(ptex_jct);
	RELEASE(ptex_road);
	RELEASE(ptex_gurd);
	RELEASE(ptex_brd1a);
	RELEASE(ptex_brd1b);
	RELEASE(ptex_tlight);
	RELEASE(ptex_light);
	RELEASE(ptex_brd2);

	RELEASE(vb_moon);
	RELEASE(vb_city);
	RELEASE(vb_ground);
	for(i=0;i<NUMDIV_ROADY;i++)
		RELEASE(vb_road[i]);
	for(i=0;i<NUMDIV_JCTY;i++)
		RELEASE(vb_jct[i]);
	RELEASE(vb_gurd);
	RELEASE(vb_brd1a);
	RELEASE(vb_brd1b);
	RELEASE(vb_gurdpoll);
}

//****************************************************************
//メッセージ処理関数
//****************************************************************

void CStage::DrawStage()
{
	LPDIRECT3DDEVICE8 d3ddev = GetD3DDevice();
	if(!d3ddev)return;

	//テクスチャアドレッシングモード-繰り返し
	d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSU,D3DTADDRESS_WRAP);
	d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSV,D3DTADDRESS_WRAP);
	d3ddev->SetVertexShader( FVF_3DVERTEX );

	d3ddev->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(5,0,20),1.0f,0);

	//描画
	DrawJct();
	DrawRoad();
	DrawBacks();

	//テクスチャアドレシングモード - 元に戻す
	d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSU,D3DTADDRESS_CLAMP);
	d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSV,D3DTADDRESS_CLAMP);
}

void CStage::DrawBacks()
{
	LPDIRECT3DDEVICE8 d3ddev = GetD3DDevice();
	if(!d3ddev)return;

	//座標変換-なし
	D3DXMATRIX mati;
	D3DXMatrixIdentity(&mati);
	d3ddev->SetTransform(D3DTS_WORLD,&mati);

	//街
	d3ddev->SetTexture(0,ptex_city);
	d3ddev->SetStreamSource(0, vb_city, sizeof(MYVERTEX3D));
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
	//月
	d3ddev->SetTexture(0,ptex_moon);
	d3ddev->SetStreamSource(0, vb_moon, sizeof(MYVERTEX3D));
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
	//地面
	d3ddev->SetTexture(0,NULL);
	d3ddev->SetStreamSource(0, vb_ground, sizeof(MYVERTEX3D));
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
	//空
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,
		vb_sky,sizeof(MYVERTEX3D));
	//★
	d3ddev->DrawPrimitiveUP(D3DPT_POINTLIST,NUM_STARS,
		vb_stars,sizeof(MYVERTEX3D));

}

void CStage::DrawJct()
{
	LPDIRECT3DDEVICE8 d3ddev = GetD3DDevice();
	if(!d3ddev)return;

	//座標変換・反転
	D3DXMATRIX matrx;
	D3DXPLANE plane_x;
	plane_x.a=1;
	plane_x.b=plane_x.c=plane_x.d=0;
	D3DXMatrixReflect(&matrx,&plane_x);
	d3ddev->SetTransform(D3DTS_WORLD,&matrx);

	//手前ガードレール
	d3ddev->SetTexture(0,ptex_gurd);
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,
		vb_maegurd,sizeof(MYVERTEX3D));
	//手前ガードレール支柱
	d3ddev->SetTexture(0,NULL);
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,
		vb_maegurdpoll,sizeof(MYVERTEX3D));

	//座標変換-なし
	D3DXMATRIX mati;
	D3DXMatrixIdentity(&mati);
	d3ddev->SetTransform(D3DTS_WORLD,&mati);

	//手前ガードレール
	d3ddev->SetTexture(0,ptex_gurd);
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,
		vb_maegurd,sizeof(MYVERTEX3D));
	//手前ガードレール支柱
	d3ddev->SetTexture(0,NULL);
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,
		vb_maegurdpoll,sizeof(MYVERTEX3D));

	//地面
	d3ddev->SetTexture(0,ptex_jct);
	for(int i=0;i<NUMDIV_JCTY;i++)
	{
		d3ddev->SetStreamSource(0, vb_jct[i], sizeof(MYVERTEX3D));
		d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,NUMDIV_JCTX*2);
	}

	//看板１
	d3ddev->SetTexture(0,ptex_brd1a);
	d3ddev->SetStreamSource(0, vb_brd1a, sizeof(MYVERTEX3D));
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
	d3ddev->SetTexture(0,ptex_brd1b);
	d3ddev->SetStreamSource(0, vb_brd1b, sizeof(MYVERTEX3D));
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

	//看板2
	d3ddev->SetTexture(0,ptex_brd1a);
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,
		vb_brd2a,sizeof(MYVERTEX3D));
	d3ddev->SetTexture(0,ptex_brd2);
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,
		vb_brd2b,sizeof(MYVERTEX3D));

	//信号
	d3ddev->SetTexture(0,ptex_tlight);
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,
		vb_tlight,sizeof(MYVERTEX3D));
}

void CStage::DrawRoad()
{
	LPDIRECT3DDEVICE8 d3ddev = GetD3DDevice();
	if(!d3ddev)return;

	//座標変換-なし
	D3DXMATRIX mati;
	D3DXMatrixIdentity(&mati);
	d3ddev->SetTransform(D3DTS_WORLD,&mati);

	//道路
	d3ddev->SetTexture(0,ptex_road);
	for(int i=0;i<NUMDIV_ROADY;i++)
	{
		d3ddev->SetStreamSource(0, vb_road[i], sizeof(MYVERTEX3D));
		d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,NUMDIV_ROADX*2);
	}
	
	//ガードレール
	d3ddev->SetTexture(0,ptex_gurd);
	d3ddev->SetStreamSource(0, vb_gurd, sizeof(MYVERTEX3D));
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,NUM_GURD*2);
	d3ddev->SetTexture(0,NULL);
	d3ddev->SetStreamSource(0, vb_gurdpoll, sizeof(MYVERTEX3D));
	d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST,0,NUM_GURDPOLL*2);
	//街灯
	d3ddev->SetTexture(0,ptex_light);
	for(i=0;i<NUM_LIGHT;i++){
		d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,
			vb_light[i],sizeof(MYVERTEX3D));
	}

	//反対側	
	D3DXMATRIX matrx;
	D3DXPLANE plane_x;
	plane_x.a=1;
	plane_x.b=plane_x.c=plane_x.d=0;
	D3DXMatrixReflect(&matrx,&plane_x);
	d3ddev->SetTransform(D3DTS_WORLD,&matrx);

	//ガードレール
	d3ddev->SetTexture(0,ptex_gurd);
	d3ddev->SetStreamSource(0, vb_gurd, sizeof(MYVERTEX3D));
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,NUM_GURD*2);
	d3ddev->SetTexture(0,NULL);
	d3ddev->SetStreamSource(0, vb_gurdpoll, sizeof(MYVERTEX3D));
	d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST,0,NUM_GURDPOLL*2);
	//街灯
	d3ddev->SetTexture(0,ptex_light);
	for(i=0;i<NUM_LIGHT;i++){
		d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,
			vb_light[i],sizeof(MYVERTEX3D));
	}
}
