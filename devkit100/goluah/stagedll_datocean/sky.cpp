/*--------------------------------------------------------------
	そら
----------------------------------------------------------------*/
#include "sky.h"

void CSky::Init()
{
	tex = aki3d.LoadTexture("tex_sky.bmp");

	for(UINT i=0;i<8;i++)
	{
		ARGB col(64,115,255,100);

		sky_vb[i].color = col.dwcol;
		sky_vb[i].z = 10.0f;

		sky_vb[i].x = ((i%4)/2) ? 2048.0f : -2048.0f;
		sky_vb[i].y = ((i%4)%2) ? 0.0f : -5.5f;

		sky_vb[i].x /= 240.0f;

		sky_vb[i].tv = ((i%4)%2) ? 1.0f : 0.0f;
	}

	time = 0;
}

void CSky::Update()
{
	time++;
	UINT time2 = time%2048;
	float t = (float)time2 / 2048.0f;

	float u1 = t;
	float u2 = t+3.0f;

	for(UINT i=0;i<4;i++)
	{
		sky_vb[i  ].tu = (i/2==1) ? u1 : u2;
		sky_vb[i+4].tu = (i/2==1) ? u2 : u1;
	}
}

void CSky::Draw()
{
	LPDIRECT3DDEVICE8 d3ddev = aki3d.GetD3DDev();

	const WORD sky_idx[] = 
	{
		0,1,2,
		1,2,3,
		4,5,6,
		5,6,7,
	};

	//マトリクース
	D3DXMATRIX mati;
	D3DXMatrixIdentity(&mati);
	d3ddev->SetTransform(D3DTS_WORLD,	&mati);		//座標変換マトリクス指定
	d3ddev->SetVertexShader( FVF_3DVERTEX );		//頂点のフォーマットを指定

	//テクスチャアドレッシングモード-繰り返し
	

	aki3d.EnableZ(FALSE,FALSE);
	aki3d.SetBlend_Add();
	
	d3ddev->SetTexture(0,tex);							//テクスチャー設定
	d3ddev->DrawIndexedPrimitiveUP(
									D3DPT_TRIANGLELIST,	//D3DPRIMITIVETYPE PrimitiveType,
									0,					//UINT MinVertexIndex,
									12,					//UINT NumVertexIndices,
									4,					//UINT PrimitiveCount,
									sky_idx,				//const void *pIndexData,
									D3DFMT_INDEX16,		//D3DFORMAT IndexDataFormat,
									sky_vb,					//CONST void* pVertexStreamZeroData,
									sizeof(MYVERTEX3D)	//UINT VertexStreamZeroStride
								);
	aki3d.SetBlend_Normal();
}

void CSky::Release()
{
	aki3d.UnloadTexture(tex);
}

