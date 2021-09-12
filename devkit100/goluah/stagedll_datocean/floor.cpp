
/*--------------------------------------------------------------
	ゆか
----------------------------------------------------------------*/
#pragma once
#include "floor.h"

#define OFFSET_Z	(4)
#define BASE_Y		(0.5f)

CFloor::CFloor()
{
	m_1stupdate		= FALSE;
	m_initialized	= FALSE;
	m_1stpass		= FALSE;

	pvb				= NULL;
	pindex			= NULL;
}

void CFloor::Init()
{
	tex = aki3d.LoadTexture("tex_floor.png");
	uf_tex = aki3d.LoadTexture("tex_soul.bmp");

	V2d v2Zero;
	v2Zero.x = v2Zero.y = 0.0f;

	for(UINT i=0;i<NUM_UNDERFLOOR;i++)
	{
		uf[i].pos.x = 1024.0f*( aki3d.RandomOne()-0.5f )*2.0f/240.0f;
		uf[i].pos.y = 10.0f*( aki3d.RandomOne()-0.5f )*2.0f + OFFSET_Z;
		uf[i].vel = v2Zero;
		uf[i].acc = v2Zero;

		uf[i].rad  = 0;
		uf[i].radv = 0.001f;
		uf[i].rada = 0;
		
		uf[i].alpha  = 0.0f;
		uf[i].alphav = 1.0f;
		uf[i].alphaa = 0.01f;

		uf_vb[i*4+0].y = BASE_Y;
		uf_vb[i*4+1].y = BASE_Y;
		uf_vb[i*4+2].y = BASE_Y;
		uf_vb[i*4+3].y = BASE_Y;
		
		uf_vb[i*4+0].tu = 0.0f;
		uf_vb[i*4+1].tu = 0.0f;
		uf_vb[i*4+2].tu = 1.0f;
		uf_vb[i*4+3].tu = 1.0f;
		
		uf_vb[i*4+0].tv = 0.0f;
		uf_vb[i*4+1].tv = 1.0f;
		uf_vb[i*4+2].tv = 0.0f;
		uf_vb[i*4+3].tv = 1.0f;

		uf_idx[i*6+0] = i*4;
		uf_idx[i*6+1] = i*4+1;
		uf_idx[i*6+2] = i*4+2;
		uf_idx[i*6+3] = i*4+1;
		uf_idx[i*6+4] = i*4+2;
		uf_idx[i*6+5] = i*4+3;

		uf[i].rad = 0.5f + 1.5f*aki3d.RandomOne();
		uf[i].alpha = 255;
	}

	LPDIRECT3DDEVICE8 d3ddev = aki3d.GetD3DDev();

	/*
	0 1 2
	3 4 5
	6 7 8
	*/
	MYVERTEX3D* vb = NULL;

	const float u_arr[3] = {1.0f,0.0f,1.0f};
	const float v_arr[3] = {1.0f,0.0f,1.0f};
	const float x_arr[3] = {-1024.0f/240.0f , 0.0f , 1024.0f/240.0f};
	const float z_arr[3] = {OFFSET_Z-10.0f,OFFSET_Z,OFFSET_Z+10.0f};
	
	if ( SUCCEEDED(d3ddev->CreateVertexBuffer(sizeof(MYVERTEX3D) * 9, 0, FVF_3DVERTEX, D3DPOOL_MANAGED, &pvb)) &&
		 SUCCEEDED(pvb->Lock(0, 0, (BYTE**)&vb, 0)) )
	{
		for(UINT i=0;i<9;i++)
		{
			vb[i].color=0xFF000000;
			vb[i].tu = u_arr[i%3];
			vb[i].tv = v_arr[i/3];
			vb[i].x = x_arr[i%3];
			vb[i].y = 0.0f;
			vb[i].z = z_arr[i/3];
		}

		pvb->Unlock();
	}

	/*
	0 1 2
	3 4 5
	6 7 8
	*/
	const WORD index_arr[] = {
		0,3,1,
		3,1,4,
		1,4,2,
		4,2,5,
		3,6,4,
		6,4,7,
		4,7,5,
		7,5,8,
	};
	BYTE* hoge = NULL;

	if ( SUCCEEDED(d3ddev->CreateIndexBuffer(sizeof(index_arr), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &pindex)) &&
		 SUCCEEDED(pindex->Lock(0, 0, &hoge, 0)) )
	{
		memcpy(hoge, index_arr, sizeof(index_arr));
		pindex->Unlock();
	}

	
	m_initialized	= TRUE;
}

void CFloor::Update()
{
	for(UINT i=0;i<NUM_UNDERFLOOR;i++)
	{
		//uf[i].acc.x += (aki3d.RandomOne()-0.5f)*2.0f*0.000001f;
		//uf[i].acc.y += (aki3d.RandomOne()-0.5f)*2.0f*0.000001f;//z

		uf[i].acc.x += (uf[i].pos.x <0 ) ? 0.0001f : -0.0001f;
		uf[i].acc.y += (uf[i].pos.y <OFFSET_Z ) ? 0.0001f : -0.0001f;

		if(uf[i].acc.Length()>0.1f)
		{
			uf[i].acc.SetLength(0.1f);
		}
		if(uf[i].vel.Length()>0.1f)
		{
			uf[i].vel.SetLength(0.1f);
		}

		//uf[i].vel += uf[i].acc;
		uf[i].pos += uf[i].acc;


		ARGB col( (BYTE)uf[i].alpha, 115,255,100 );

		uf_vb[i*4+0].color = col.dwcol;
		uf_vb[i*4+1].color = col.dwcol;
		uf_vb[i*4+2].color = col.dwcol;
		uf_vb[i*4+3].color = col.dwcol;

		uf_vb[i*4+0].x = uf[i].pos.x - uf[i].rad;
		uf_vb[i*4+1].x = uf[i].pos.x - uf[i].rad;
		uf_vb[i*4+2].x = uf[i].pos.x + uf[i].rad;
		uf_vb[i*4+3].x = uf[i].pos.x + uf[i].rad;
		
		uf_vb[i*4+0].z = uf[i].pos.y - uf[i].rad;
		uf_vb[i*4+1].z = uf[i].pos.y + uf[i].rad;
		uf_vb[i*4+2].z = uf[i].pos.y - uf[i].rad;
		uf_vb[i*4+3].z = uf[i].pos.y + uf[i].rad;
	}

	
	m_1stupdate		= TRUE;
}

void CFloor::Draw()
{
	if(!m_1stupdate)return;

	D3DXMATRIX mati;
	D3DXMatrixIdentity(&mati);

	aki3d.SetBlend_Normal();
	LPDIRECT3DDEVICE8 d3ddev = aki3d.GetD3DDev();

	d3ddev->SetTransform(D3DTS_WORLD,	&mati);		//座標変換マトリクス指定
	d3ddev->SetStreamSource(0, pvb, sizeof(MYVERTEX3D));
	d3ddev->SetVertexShader( FVF_3DVERTEX );		//頂点のフォーマットを指定
	d3ddev->SetIndices(pindex, 0);

	aki3d.EnableZ();
	//描画
	d3ddev->SetTexture(0,tex);						//テクスチャー設定
	d3ddev->DrawIndexedPrimitive(
									D3DPT_TRIANGLELIST,	//D3DPRIMITIVETYPE PrimitiveType,
									0,					//UINT MinVertexIndex,
									9,					//UINT NumVertexIndices,
									0,
									8					//UINT PrimitiveCount,
								);
	
	aki3d.EnableZ(TRUE,FALSE);
	aki3d.SetBlend_Add();

	d3ddev->SetTexture(0,uf_tex);						//テクスチャー設定

	static BOOL err = FALSE;
//	try{
		if(!err)d3ddev->DrawIndexedPrimitiveUP(
									D3DPT_TRIANGLELIST,	//D3DPRIMITIVETYPE PrimitiveType,
									0,					//UINT MinVertexIndex,
									NUM_UNDERFLOOR*6,	//UINT NumVertexIndices,
									NUM_UNDERFLOOR*2,	//UINT PrimitiveCount,
									uf_idx,				//const void *pIndexData,
									D3DFMT_INDEX16,		//D3DFORMAT IndexDataFormat,
									uf_vb,					//CONST void* pVertexStreamZeroData,
									sizeof(MYVERTEX3D)	//UINT VertexStreamZeroStride
								);
/*	}catch(...)
	{
		err=TRUE;
	}*/
	m_1stpass = TRUE;

	aki3d.SetBlend_Normal();

}

void CFloor::Release()
{
	RELEASE(pindex);
	RELEASE(pvb);
	aki3d.UnloadTexture(tex);
}

