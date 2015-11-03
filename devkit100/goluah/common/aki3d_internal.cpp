
#include "aki3d_internal.h"

/*******************************************************************

	テクスチャ管理クラス

********************************************************************/

CTexManager *tex_man = NULL;

//テクスチャ読み込み
LPDIRECT3DTEXTURE9 CTexManager::LoadTexture(const char* filename)
{
	if(!filename)return NULL;
	LPDIRECT3DDEVICE9 d3ddev = aki3d.GetD3DDev();
	if(!d3ddev)return NULL;

	LPDIRECT3DTEXTURE9 ret=NULL;

	//既存のものから探す
	for(UINT i=0;i<m_texList.size();i++){
		if( strcmp(m_texNameList[i],filename)==0 )
		{
			m_texRefcntList[i]++;
			return( m_texList[i] );
		}
	}

	//ファイル名準備
	char *path = new char[MAX_PATH];
	if(m_texPath)
		sprintf(path,"%s\\%s",m_texPath,filename);
	else
		strcpy(path,filename);

	OutputDebugString("aki3d , load texture  ");
	OutputDebugString( path );
	OutputDebugString("\n");

	//新規に読み込み
	HRESULT loadret = D3DXCreateTextureFromFileEx(d3ddev, path,
		0,0,0,0,
		D3DFMT_A1R5G5B5,
		D3DPOOL_MANAGED,D3DX_FILTER_POINT,D3DX_FILTER_POINT,
		0,NULL,NULL,&ret);
	delete [] path;

	//リストに追加
	m_texList.push_back(ret);
	char* newTexName = new char [strlen(filename)+1];
	strcpy(newTexName,filename);
	m_texNameList.push_back(newTexName);
	m_texRefcntList.push_back(1);

	return ret;
}

//テクスチャの開放
void CTexManager::UnloadTexture(LPDIRECT3DTEXTURE9 ptex)
{
	if(!ptex)return;

	for(UINT i=0;i<m_texList.size();i++)
	{
		if(m_texList[i]==ptex)
		{
			m_texRefcntList[i]--;
			if(m_texRefcntList[i]==0)
			{
				ptex->Release();
				m_texList.erase( m_texList.begin() + i );
				DELETEARRAY( m_texNameList[i] )
				m_texNameList.erase( m_texNameList.begin() + i );
				m_texRefcntList.erase( m_texRefcntList.begin() + i );
			}
			return;
		}
	}
}

//すべて破棄
void CTexManager::Destroy()
{
	//テクスチャリストの破棄
	while(m_texList.size()!=0){
		std::vector<LPDIRECT3DTEXTURE9>::iterator itex = m_texList.begin();
		RELEASE((*itex));
		m_texList.erase(itex);
	}
	while(m_texNameList.size()!=0){
		std::vector<char*>::iterator iname = m_texNameList.begin();
		DELETEARRAY( (*iname) );
		m_texNameList.erase(iname);
	}
	m_texRefcntList.clear();

	DELETEARRAY(m_texPath);
}

//ベースパス設定
void CTexManager::SetBasePath(const char *path)
{
	if(!path)return;
	if(!m_texPath){
		m_texPath = new char [MAX_PATH];
	}

	strcpy(m_texPath,path);
}



/*******************************************************************

	△ストリップ描画クラス

********************************************************************/

/*------------------------------------------------------------------
	構築
--------------------------------------------------------------------*/
CTristripBody::CTristripBody()
{
	node_num = 0;
	rad = NULL;
	pos = NULL;
	col = NULL;
	vtemp = NULL;
	vsubs = NULL;
	norms = NULL;
	vb = NULL;
	tex = NULL;
}

/*------------------------------------------------------------------
	生成
--------------------------------------------------------------------*/
void CTristripBody::Create(UINT num_node , const char* tex_filename)
{
	Destroy();
	if(num_node<2)return;

	tex = tex_man->LoadTexture(tex_filename);
	node_num = num_node;

	pos = new V3d[node_num];
	col = new ARGB[node_num];
	rad = new float[node_num];
	vb  = new MYVERTEX3D[node_num*2];

	vtemp = new V2d[node_num+1];
	vsubs = vtemp+2;
	norms = vtemp;

	SetupTexCoords();
}

/*------------------------------------------------------------------
	破棄
--------------------------------------------------------------------*/
void CTristripBody::Destroy()
{
	DELETEARRAY(pos);
	DELETEARRAY(col);
	DELETEARRAY(rad);
	DELETEARRAY(vb);
	DELETEARRAY(vtemp);
	vsubs=NULL;
	norms=NULL;

	node_num=0;

	tex_man->UnloadTexture(tex);
	tex=NULL;
}

/*------------------------------------------------------------------
	座標値計算 & 描画
	(2D計算)
--------------------------------------------------------------------*/
void CTristripBody::Render(D3DMATRIX *l2w)
{
	UINT i,j;

	LPDIRECT3DDEVICE9 d3ddev = aki3d.GetD3DDev();
	if(d3ddev==NULL)return;

	//ローカル→ワールドマトリクスを指定されなかった場合は単位行列で代用する
	D3DXMATRIX mati;
	D3DXMatrixIdentity(&mati);
	if(!l2w)l2w = &mati;

	//各ベクトル間でのsubをとり、
	//それを90°回転する( 2Dの90°回転：(x',y')=(y,-x) )
	for(i=0;i<node_num-1;i++)
	{
		vsubs[i].x = (pos[i+1].y - pos[i].y);
		vsubs[i].y = (pos[i].x - pos[i+1].x);
		vsubs[i].Normalize();//ノーマライズしておく
	}

	//各ノードでの法線っぽいものを出す
	norms[0] = vsubs[0];//最初の1点
	i=1;
	j=0;
	for(i=1;i<node_num-1;i++,j++){
		norms[i].x = vsubs[j].x + vsubs[j+1].x;
		norms[i].y = vsubs[j].y + vsubs[j+1].y;
	}
	norms[i]=vsubs[i-1];//最後の1点

	//法線ノーマライズ
	for(i=0;i<node_num;i++){
		norms[i].Normalize();
	}

	for(i=0;i<node_num;i++)
	{
		//色設定
		vb[i*2  ].color = vb[i*2+1].color = *((DWORD*)&col[i]);
		
		//各頂点座標を法線っぽいもの方向に伸ばしつつ描画用の頂点配列に書き込み
		vb[i*2  ].x = pos[i].x + norms[i].x*rad[i]*0.5f;
		vb[i*2+1].x = pos[i].x - norms[i].x*rad[i]*0.5f;
		vb[i*2  ].y = pos[i].y + norms[i].y*rad[i]*0.5f;
		vb[i*2+1].y = pos[i].y - norms[i].y*rad[i]*0.5f;
		vb[i*2  ].z = pos[i].z  ;
		vb[i*2+1].z = pos[i].z  ;
	}

	//描画
	try
	{
		//描画前設定
		d3ddev->SetTexture(0,tex);						//テクスチャー設定
		d3ddev->SetTransform(D3DTS_WORLD,l2w);			//座標変換マトリクス指定
		d3ddev->SetFVF(FVF_3DVERTEX);		//頂点のフォーマットを指定

		//描画
		if(D3D_OK != d3ddev->DrawPrimitiveUP(
								D3DPT_TRIANGLESTRIP,	//プリミティブ種類
								(node_num-1)*2,			//プリミティブ数
								vb,						//頂点配列
								sizeof(MYVERTEX3D)		//頂点ストライド
						))
		{
			aki3d.LogWarning("%s failed to render",__FUNCTION__);
		}
	}
	catch(...)
	{
		zenka=TRUE;
		aki3d.LogWarning("%s catch exception while rendering",__FUNCTION__);
	}
}

/*------------------------------------------------------------------
	テクスチャ座標設定
	u : ストリップの両端で 0～1
	v : ノードの2頂点間で 0～1
--------------------------------------------------------------------*/
void CTristripBody::SetupTexCoords()
{
	for(UINT i=0;i<node_num;i++){
		vb[i*2  ].tv = 0.0f;
		vb[i*2+1].tv = 1.0f;

		vb[i*2  ].tu = (float)i / (float)(node_num-1);
		vb[i*2+1].tu = vb[i*2  ].tu;
	}
}






/*******************************************************************

	パーティクルクラス実体

********************************************************************/


/*------------------------------------------------------------------
	構築
--------------------------------------------------------------------*/
CParticleBody::CParticleBody()
{
	index_arr = NULL;

	pos = NULL;
	col = NULL;
	rad = NULL;
	vb  = NULL;
	index_arr = NULL;
	ax = NULL;

	#ifdef CParticleBody_3D
	tpos= NULL;
	tax= NULL;
	#endif

	tex = NULL;
	zenka = FALSE;
}

/*------------------------------------------------------------------
	生成
--------------------------------------------------------------------*/
void CParticleBody::Create(UINT num_tubu , const char *tex_filename)
{
	Destroy();
	tex = tex_man->LoadTexture(tex_filename);

	num = num_tubu;

	if(num==0)return;

	pos = new V3d[num];
	col = new ARGB[num];
	rad = new float[num];
	ax  = new V3d[num];

	vb  = new MYVERTEX3D[num*4];
	index_arr = new WORD[num*6];

	SetupTexCoords();

	for(UINT i=0;i<num;i++)
	{
		//axisをゼロクリア
		ax[i] = Zero;

		//インデックスデータ準備
		index_arr[i*6+0] = i*4;
		index_arr[i*6+1] = i*4+1;
		index_arr[i*6+2] = i*4+2;
		
		index_arr[i*6+3] = i*4+1;
		index_arr[i*6+4] = i*4+2;
		index_arr[i*6+5] = i*4+3;
	}

	#ifdef CParticleBody_3D
	tpos= new D3DXVECTOR4[num];
	tax= new D3DXVECTOR3[num];
	#endif
}

/*------------------------------------------------------------------
	破棄
--------------------------------------------------------------------*/
void CParticleBody::Destroy()
{
	DELETEARRAY(pos);
	DELETEARRAY(col);
	DELETEARRAY(rad);
	DELETEARRAY(ax);

	DELETEARRAY(vb);
	DELETEARRAY(index_arr);

	num=0;

	tex_man->UnloadTexture( tex );
	tex=NULL;

	#ifdef CParticleBody_3D
	DELETEARRAY(tpos);
	DELETEARRAY(tax)
	#endif
}

/*------------------------------------------------------------------
	座標値計算 & 描画
--------------------------------------------------------------------*/
void CParticleBody::Render(D3DMATRIX *l2w)
{
	if(zenka)return;

	#ifndef CParticleBody_3D

	UINT i;

	LPDIRECT3DDEVICE8 d3ddev = aki3d.GetD3DDev();
	if(d3ddev==NULL)return;

	//ローカル→ワールド変換マトリクスが指定されていない場合単位行列で代用する
	D3DXMATRIX mati;
	D3DXMatrixIdentity(&mati);
	if(!l2w)l2w = &mati;

	for(i=0;i<num;i++){
		//色設定
		vb[i*4  ].color = vb[i*4+1].color = 
		vb[i*4+2].color = vb[i*4+3].color = *((DWORD*)&col[i]);

		//伸ばし方向ベクトルをxy平面上に写したものをを計算
		V2d eax;
		eax.x = ax[i].x;
		eax.y = ax[i].y;
		eax.SetLength(rad[i]*0.5f);//のばす

		//上記ベクトルに垂直なものを計算
		V2d vax = eax;
		vax.Verticalize();

		//posにベクトルをたしつつ描画用の頂点配列に書き込み
		vb[i*4  ].x = pos[i].x + ax[i].x + eax.x + vax.x;
		vb[i*4+1].x = pos[i].x - ax[i].x - eax.x + vax.x;
		vb[i*4+2].x = pos[i].x + ax[i].x + eax.x - vax.x;
		vb[i*4+3].x = pos[i].x - ax[i].x - eax.x - vax.x;

		vb[i*4  ].y = pos[i].y + ax[i].y + eax.y + vax.y;
		vb[i*4+1].y = pos[i].y - ax[i].y - eax.y + vax.y;
		vb[i*4+2].y = pos[i].y + ax[i].y + eax.y - vax.y;
		vb[i*4+3].y = pos[i].y - ax[i].y - eax.y - vax.y;

		vb[i*2  ].z = pos[i].z + ax[i].z;
		vb[i*2+1].z = pos[i].z - ax[i].z;
		vb[i*2+2].z = pos[i].z + ax[i].z;
		vb[i*2+3].z = pos[i].z - ax[i].z;
	}

	//描画
	{
		//描画前設定
		d3ddev->SetTexture(0,tex);						//テクスチャー設定
		d3ddev->SetTransform(D3DTS_WORLD,l2w);			//座標変換マトリクス指定
		d3ddev->SetFVF( FVF_3DVERTEX );		//頂点のフォーマットを指定

		//描画
		d3ddev->DrawIndexedPrimitiveUP(
									D3DPT_TRIANGLELIST,	//D3DPRIMITIVETYPE PrimitiveType,
									0,					//UINT MinVertexIndex,
									num*4,				//UINT NumVertexIndices,
									num*2,				//UINT PrimitiveCount,
									index_arr,			//const void *pIndexData,
									D3DFMT_INDEX16,		//D3DFORMAT IndexDataFormat,
									vb,					//CONST void* pVertexStreamZeroData,
									sizeof(MYVERTEX3D)	//UINT VertexStreamZeroStride
								);
	}

	#else

	//やっぱこっちじゃなきゃだめ？

	UINT i;
	D3DXMATRIX mati;	// 単位行列
	D3DXMATRIX pmat;	// プロジェクション行列
	D3DXMATRIX vmat;	// ビュー行列
	D3DXMATRIX dmat;	// ローカル座標系→ディスプレイ座標系変換マトリクス
	D3DXMATRIX idmat;	// ローカル座標系←ディスプレイ座標系変換マトリクス

	LPDIRECT3DDEVICE9 d3ddev = aki3d.GetD3DDev();
	if(d3ddev==NULL)return;

	D3DXMatrixIdentity(&mati);
	if(!l2w)l2w = &mati;
	d3ddev->GetTransform(D3DTS_PROJECTION,&pmat);
	d3ddev->GetTransform(D3DTS_VIEW,&vmat);
	dmat = (*l2w);
	dmat *= vmat;
	dmat *= pmat;
	D3DXMatrixInverse(&idmat,NULL,&dmat);

	//ポジションとAxisをディスプレイ座標系上に変換する
	for(i=0;i<num;i++)
	{
		D3DXVec3Transform(&tpos[i],&pos[i],&dmat);
		D3DXVec3TransformNormal(&tax[i],&ax[i],&dmat);

		//wでワル
		tpos[i].x /= tpos[i].w;
		tpos[i].y /= tpos[i].w;
		if(tpos[i].w<0.0f){
			tpos[i].x *=-1.0f;
			tpos[i].y *=-1.0f;
		}
	}

	float nc = NEAR_CLIP;//dr->GetNearClip();
	float fc = FAR_CLIP;//dr->GetFarClip();

	float radius;
	for(i=0;i<num;i++){
		//色設定
		vb[i*4  ].color = vb[i*4+1].color = 
		vb[i*4+2].color = vb[i*4+3].color = *((DWORD*)&col[i]);

		//ディスプレイ上での大きさ計算
		if(tpos[i].z!=0)
			radius = rad[i]/tpos[i].z ;	//あってるか？
		else{
			radius = 1.0f;
		}
		V2d vax;
		vax.x = tax[i].x;
		vax.y = tax[i].y;
		vax.SetLength(radius);
		tax[i].x += vax.x;
		tax[i].y += vax.y;
		vax.Verticalize();
		//rad[i]);

		//各頂点座標を肉付けしつつ描画用の頂点配列に書き込み
		vb[i*4  ].x = tpos[i].x + tax[i].x + vax.x;
		vb[i*4+1].x = tpos[i].x - tax[i].x + vax.x;
		vb[i*4+2].x = tpos[i].x + tax[i].x - vax.x;
		vb[i*4+3].x = tpos[i].x - tax[i].x - vax.x;

		vb[i*4  ].y = tpos[i].y + tax[i].y + vax.y;
		vb[i*4+1].y = tpos[i].y - tax[i].y + vax.y;
		vb[i*4+2].y = tpos[i].y + tax[i].y - vax.y;
		vb[i*4+3].y = tpos[i].y - tax[i].y - vax.y;

		vb[i*2  ].z = tpos[i].z / tpos[i].w ;//+ tax[i].z;
		vb[i*2+1].z = tpos[i].z / tpos[i].w ;//- tax[i].z;
		vb[i*2+2].z = tpos[i].z / tpos[i].w ;//+ tax[i].z;
		vb[i*2+3].z = tpos[i].z / tpos[i].w ;//- tax[i].z;
	}

	//描画
	{
		//描画前設定
		d3ddev->SetTexture(0,tex);						//テクスチャー設定
		d3ddev->SetTransform(D3DTS_WORLD,	&mati);		//座標変換マトリクス指定(すでに変換済みなのでやらない)
		d3ddev->SetTransform(D3DTS_VIEW,	&mati);		//座標変換マトリクス指定(すでに変換済みなのでやらない)
		d3ddev->SetTransform(D3DTS_PROJECTION,&mati);	//座標変換マトリクス指定(すでに変換済みなのでやらない)
		d3ddev->SetFVF(FVF_3DVERTEX);		//頂点のフォーマットを指定

		//描画
		try{
		if(D3D_OK!=d3ddev->DrawIndexedPrimitiveUP(
									D3DPT_TRIANGLELIST,	//D3DPRIMITIVETYPE PrimitiveType,
									0,					//UINT MinVertexIndex,
									num*4,				//UINT NumVertexIndices,
									num*2,				//UINT PrimitiveCount,
									index_arr,			//const void *pIndexData,
									D3DFMT_INDEX16,		//D3DFORMAT IndexDataFormat,
									vb,					//CONST void* pVertexStreamZeroData,
									sizeof(MYVERTEX3D)	//UINT VertexStreamZeroStride
								))
		{
			aki3d.LogWarning("%s failed to render",__FUNCTION__);
		}
		}catch(...)
		{
			zenka=TRUE;
			aki3d.LogWarning("%s catch exception while rendering",__FUNCTION__);
		}

		//マトリクス設定のレストア
		d3ddev->SetTransform(D3DTS_VIEW,&vmat);	
		d3ddev->SetTransform(D3DTS_PROJECTION,&pmat);
	}

	#endif//CParticleBody_3D
}

/*------------------------------------------------------------------
	テクスチャ座標設定
--------------------------------------------------------------------*/
void CParticleBody::SetupTexCoords()
{
	for(UINT i=0;i<num;i++)
	{
		vb[i*4  ].tu = 0;
		vb[i*4+1].tu = 0;
		vb[i*4+2].tu = 1;
		vb[i*4+3].tu = 1;
		
		vb[i*4  ].tv = 0;
		vb[i*4+1].tv = 1;
		vb[i*4+2].tv = 0;
		vb[i*4+3].tv = 1;
	}
}



/*******************************************************************

	フラットパーティクル描画クラス実体

********************************************************************/

CFlatBoardsBody::CFlatBoardsBody()
{
	pos = NULL;
	ax1 = NULL;
	ax2 = NULL;
	col = NULL;
	vb  = NULL;
	index_arr = NULL;
	tex = NULL;

	zenka = FALSE;
}

void CFlatBoardsBody::Create(UINT num_node , const char* tex_filename)
{
	Destroy();

	if(num_node==0)return;
	num = num_node;

	pos = new V3d[num];
	ax1 = new V3d[num];
	ax2 = new V3d[num];
	col = new ARGB[num];
	vb  = new MYVERTEX3D[num*5];
	index_arr = new WORD[num*12];

	tex = aki3d.LoadTexture( tex_filename );

	UINT i=0;

	//index
	for(i=0;i<num;i++)
	{
		index_arr[i*12+0] = i*5+0;
		index_arr[i*12+1] = i*5+1;
		index_arr[i*12+2] = i*5+2;

		index_arr[i*12+3] = i*5+0;
		index_arr[i*12+4] = i*5+2;
		index_arr[i*12+5] = i*5+4;
		
		index_arr[i*12+6] = i*5+0;
		index_arr[i*12+7] = i*5+4;
		index_arr[i*12+8] = i*5+3;
		
		index_arr[i*12+9]  = i*5+0;
		index_arr[i*12+10] = i*5+3;
		index_arr[i*12+11] = i*5+1;
	}

	//uv
	for(i=0;i<num;i++)
	{
		vb[i*5+0].tu = 0.5f;
		vb[i*5+0].tv = 0.5f;

		vb[i*5+1].tu = 0.0f;
		vb[i*5+1].tv = 0.0f;

		vb[i*5+2].tu = 0.0f;
		vb[i*5+2].tv = 1.0f;
		
		vb[i*5+3].tu = 1.0f;
		vb[i*5+3].tv = 0.0f;
		
		vb[i*5+4].tu = 1.0f;
		vb[i*5+4].tv = 1.0f;
	}
}

void CFlatBoardsBody::Destroy()
{
	DELETEARRAY(pos);
	DELETEARRAY(ax1);
	DELETEARRAY(ax2);
	DELETEARRAY(col);
	DELETEARRAY(vb);
	DELETEARRAY(index_arr);

	aki3d.UnloadTexture(tex);
	tex = NULL;

	num = 0;
}

void CFlatBoardsBody::Render(D3DMATRIX *l2w)
{
	if(zenka)return;
	UINT i;

	for(i=0;i<num;i++)
	{
		//vertex
		vb[i*5+0].x = pos[i].x ;
		vb[i*5+0].y = pos[i].y ;
		vb[i*5+0].z = pos[i].z ;

		vb[i*5+1].x = pos[i].x + ax2[i].x + ax1[i].x;
		vb[i*5+1].y = pos[i].y + ax2[i].y + ax1[i].y;
		vb[i*5+1].z = pos[i].z + ax2[i].z + ax1[i].z;
		
		vb[i*5+2].x = pos[i].x + ax2[i].x - ax1[i].x;
		vb[i*5+2].y = pos[i].y + ax2[i].y - ax1[i].y;
		vb[i*5+2].z = pos[i].z + ax2[i].z - ax1[i].z;
		
		vb[i*5+3].x = pos[i].x - ax2[i].x + ax1[i].x;
		vb[i*5+3].y = pos[i].y - ax2[i].y + ax1[i].y;
		vb[i*5+3].z = pos[i].z - ax2[i].z + ax1[i].z;
		
		vb[i*5+4].x = pos[i].x - ax2[i].x - ax1[i].x;
		vb[i*5+4].y = pos[i].y - ax2[i].y - ax1[i].y;
		vb[i*5+4].z = pos[i].z - ax2[i].z - ax1[i].z;

		//color setting
		vb[i*5+0].color = col[i].dwcol;
		vb[i*5+1].color = col[i].dwcol;
		vb[i*5+2].color = col[i].dwcol;
		vb[i*5+3].color = col[i].dwcol;
		vb[i*5+4].color = col[i].dwcol;
	}

	LPDIRECT3DDEVICE9 d3ddev = aki3d.GetD3DDev();
	
	D3DXMATRIX mat;
	if(!l2w)
	{
		l2w = D3DXMatrixIdentity(&mat);
	}

	d3ddev->SetTexture(0,tex);						//テクスチャー設定
	d3ddev->SetTransform(D3DTS_WORLD,l2w);			//座標変換マトリクス指定
	d3ddev->SetFVF(FVF_3DVERTEX);		//頂点のフォーマットを指定

	//描画
	try{
	if(D3D_OK!=d3ddev->DrawIndexedPrimitiveUP(
								D3DPT_TRIANGLELIST,	//D3DPRIMITIVETYPE PrimitiveType,
								0,					//UINT MinVertexIndex,
								num*12,				//UINT NumVertexIndices,
								num*4,				//UINT PrimitiveCount,
								index_arr,			//const void *pIndexData,
								D3DFMT_INDEX16,		//D3DFORMAT IndexDataFormat,
								vb,					//CONST void* pVertexStreamZeroData,
								sizeof(MYVERTEX3D)	//UINT VertexStreamZeroStride
							))
		{
			aki3d.LogWarning("%s failed to render",__FUNCTION__);
		}
	}
	catch(...)
	{
		zenka=TRUE;
		aki3d.LogWarning("%s catch exception while rendering",__FUNCTION__);
	}
}






/*******************************************************************

	メッシュクラス実体

********************************************************************/


/*------------------------------------------------------------------
	構築
--------------------------------------------------------------------*/
CMeshBody::CMeshBody()
{
	rad = 1.0f;
	node_num = 0;
	pos = V3d(0, 0, 0);
	rot = V3d(0, 0, 0);
	x_mesh = NULL;
	ldir = V3d(-5.0f, 8.0f, 10.0f);
	amb = 0x00606060;
	isspecular = TRUE;

	mat = NULL;
	tex = NULL;
	zenka = FALSE;
}

/*------------------------------------------------------------------
	生成
--------------------------------------------------------------------*/
void CMeshBody::Create(const char* x_filename)
{
	if(!x_filename)return;
	LPDIRECT3DDEVICE9 d3ddev = aki3d.GetD3DDev();
	if(!d3ddev)return;

	Destroy();
	HRESULT hr;
	LPD3DXBUFFER mtrbuf = NULL;

	// 読み込み
	hr = D3DXLoadMeshFromX((LPSTR)x_filename,
		D3DXMESH_MANAGED,
		d3ddev,
		NULL,
		&mtrbuf,
		NULL,
		&node_num,
		&x_mesh);

	if (FAILED(hr))		// ﾓﾙｽｧ
		return;

	// マテリアル設定
	LPD3DXMATERIAL mater = (LPD3DXMATERIAL)mtrbuf->GetBufferPointer();
	mat = new D3DMATERIAL9[node_num];

	// テクスチャを
	tex = new LPDIRECT3DTEXTURE9[node_num];

	for (int i = 0; i < (int)node_num; i++)
	{
		mat[i] = mater[i].MatD3D;
		mat[i].Ambient = mat[i].Diffuse;
		tex[i] = tex_man->LoadTexture(mater[i].pTextureFilename);
	}

	RELEASE(mtrbuf); // あぼんぬ
}

/*------------------------------------------------------------------
	破棄
--------------------------------------------------------------------*/
void CMeshBody::Destroy()
{
	for (int i = 0; i < (int)node_num; i++)
		tex_man->UnloadTexture(tex[i]);
	DELETEARRAY(tex);
	DELETEARRAY(mat);
	RELEASE(x_mesh);
}

/*------------------------------------------------------------------
	座標値計算 & 描画
	(2D計算)
--------------------------------------------------------------------*/
void CMeshBody::Render(D3DMATRIX *l2w)
{
	if(zenka)return;
	UINT i;

	LPDIRECT3DDEVICE9 d3ddev = aki3d.GetD3DDev();
	if(d3ddev==NULL)return;
	if (x_mesh == NULL) return;

	//ローカル→ワールドマトリクスを指定されなかった場合は単位行列で代用する
	D3DXMATRIX mati = aki3d.CreateMatrix(&V3d(rad, rad * -1.0f, rad), &rot, &pos);	// Y軸を反転する
	if(l2w)mati *= *l2w;

	D3DLIGHT9 light;

	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Specular = light.Diffuse;		// スペキュラ色も設定しとく
	D3DXVec3Normalize((V3d*)&light.Direction, &ldir);
	light.Range = 1000.0f;

	//描画
	try
	{
		D3DMATERIAL9 old;
		
		//描画前設定
		d3ddev->SetTransform(D3DTS_WORLD,&mati);			//座標変換マトリクス指定
		d3ddev->SetFVF(x_mesh->GetFVF());
		d3ddev->GetMaterial(&old);
		d3ddev->SetLight(0, &light);
		d3ddev->LightEnable(0, TRUE);
		d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);
		d3ddev->SetRenderState(D3DRS_SPECULARENABLE, isspecular);
		d3ddev->SetRenderState( D3DRS_AMBIENT, amb );

		//描画
		for (i = 0; i < (int)node_num; i++)
		{
			d3ddev->SetMaterial(&mat[i]);
			d3ddev->SetTexture(0,tex[i]);						//テクスチャー設定
			if(D3D_OK != x_mesh->DrawSubset(i))
			{
				aki3d.LogWarning("%s failed to render",__FUNCTION__);
			}
		}
 
		// 元に戻しとく
		d3ddev->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
		d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
		d3ddev->SetMaterial(&old);
		d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		d3ddev->SetFVF(FVF_3DVERTEX);
	}
	catch(...)
	{
		zenka=TRUE;
		aki3d.LogWarning("%s catch exception while rendering",__FUNCTION__);
	}
}


