

#include "stage.h"
#include <math.h>

#include "jisaku.h"
#include "aki3d.h"

char *g_stgdir;

SET_STAGEINFO(CStage)


#ifdef USE_GOLUAH_COMMON_DLL
DI_FUNCTIONS_S *funcs = NULL;
DI_FUNCTIONS_O *funco = NULL;
DI_FUNCTIONS_D *funcd = NULL;
#endif // USE_GOLUAH_COMMON_DLL

/*!
*	@brief コンストラクタ
*/
CStage::CStage(SDI_STAGEINFO2 *info)
			: CStageBase(info)
{
	ptex_jimen = NULL;
	vb_gnd = NULL;
	g_stgdir = info->dir;
	aki3d.Initialize(GetD3DDevice(),info->dir);

	moya = NULL;
	ZeroMemory(&src, sizeof(src));
	ZeroMemory(&dst, sizeof(dst));

#ifdef USE_GOLUAH_COMMON_DLL
	funco = info->funco;
	funcs = info->funcs;
	funcd = info->funcd;
#endif // USE_GOLUAH_COMMON_DLL
}

/*!
*	@brief デストラクタ
*/
CStage::~CStage()
{
	UnloadBitMap(pms_back);
	RELEASE(vb_gnd);
	RELEASE(ptex_jimen);
}

/*!
*	@brief 初期化
*/
void CStage::InitializeStage()
{
	d3ddev = GetD3DDevice();

	LoadTextures();//テクスチャ読込み
	InitVrtx();//頂点座標初期化

	InitializeJien();

	moya = aki3d.CreateParticleObject();
	moya->Create(MOYA_NUM,"moya.png");

	for(int i=0;i<MOYA_NUM;i++)
	{
		moyainfo[i].Init();
	}
}

void CStage::InitVrtx()//頂点座標の初期化
{
	MYVERTEX3D* pv = NULL;

	if (d3ddev)
		d3ddev->CreateVertexBuffer(sizeof(MYVERTEX3D) * 4, 0, FVF_3DVERTEX, D3DPOOL_MANAGED, &vb_gnd);

	if ( vb_gnd && SUCCEEDED(vb_gnd->Lock(0, 0, (BYTE**)&pv, 0)) )
	{
		pv[0].y=pv[1].y=0.0f;
		pv[2].y=pv[3].y=0.0f;
		pv[0].x = -(-1.0f/-3.0f+2.0f);
		pv[0].z = -1.0f;
		pv[2].x =  dst.left;
		pv[2].z =  dst.z;
		pv[1].x =  (-1.0f/-3.0f+2.0f);
		pv[1].z = -1.0f;
		pv[3].x =  dst.right;
		pv[3].z =  dst.z;
		pv[0].tu=0;//テクスチャ座標
		pv[0].tv=0;
		pv[1].tu=0;
		pv[1].tv=3;
		pv[2].tu=3;
		pv[2].tv=0;
		pv[3].tu=3;
		pv[3].tv=3;
		pv[0].color=pv[1].color=pv[2].color=pv[3].color=pdat->color;

		vb_gnd->Unlock();
	}
}

void CStage::LoadTextures()//テクスチャの読み込み
{
	char fn[256];

	sprintf(fn,"%s\\back",GetDLLPath());
	pms_back=LoadImage(fn,NULL);

	ptex_jimen=NULL;

	if(d3ddev!=NULL){
		sprintf(fn,"%s\\ground.png",GetDLLPath());
		D3DXCreateTextureFromFile(d3ddev,fn,&ptex_jimen);
	}

	//転送元・ビットマップ全体
	src.left = src.top = 0;
	src.right = (long)pms_back->wg;
	src.bottom= (long)pms_back->hg;

	//転送先
	if(pms_back->wg!=640)
		dst.z = (float)(960.0*4.0-3.0*pms_back->wg)/(float)(pms_back->wg-640.0);
	dst.z += 0.03f;
	dst.left  =-(dst.z/3.0f+2.0f)*(640.0f/480.0f);
	dst.right = (dst.z/3.0f+2.0f)*(640.0f/480.0f);
	dst.top   = -(1.0f+dst.z/3.0f)*2.0f;
	dst.bottom= 0;
}


/*!
*	@brief 描画
*/
DWORD CStage::DrawBack()
{
	//背景-------------------------------------------------------------

	SetTransform(TRUE);

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	SetParentMatrix(&mat,TRUE,NULL);
	SetTransform(TRUE);
	Blt3D(pms_back,src,dst,pdat->color);

	//地面---------------------------------------------------------------
	if(ptex_jimen!=NULL){
		D3DXMatrixIdentity(&mat);
		d3ddev->SetTransform(D3DTS_WORLD,&mat);
		d3ddev->SetStreamSource(0, vb_gnd, sizeof(MYVERTEX3D));
		d3ddev->SetVertexShader(FVF_3DVERTEX);
		d3ddev->SetTexture(0,ptex_jimen);
		d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSU,D3DTADDRESS_MIRROR);
		d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSV,D3DTADDRESS_MIRROR);
		d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
		d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSU,D3DTADDRESS_CLAMP);//元に戻しとく
		d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSV,D3DTADDRESS_CLAMP);//元に戻しとく
	}

	//もや---------------------------------------------------------
	aki3d.EnableZ(FALSE,FALSE);
	aki3d.SetBlend_Add();
	moya->Render(NULL);
	aki3d.SetBlend_Normal();
	aki3d.EnableZ();

	return FALSE;//通常、デフォルト描画は行わない
}


DWORD CStage::Action()
{
	for(int i=0;i<MOYA_NUM;i++)
	{
		moyainfo[i].Update();

		moya->SetPos(i,V3d(moyainfo[i].x,moyainfo[i].width*-0.40f,moyainfo[i].z));
		moya->SetAxis(i,Zero);//V3d(moyainfo[i].axlen,0.0f,0.0f));
		moya->SetColorDW(i,0x11FFFFFF);
		moya->SetWidth(i,moyainfo[i].width);
	}
	return TRUE;
}


void CStage::MoyaInfo::Init()
{
	x = aki3d.RandomOne2()*1536.0f/240.0f;
	z = aki3d.RandomOne()*2.0f;
	width = (aki3d.RandomOne()*300.0f + 300.0f)/240.0f;
	axlen = (aki3d.RandomOne()*10.0f + 10.0f)/240.0f;
	
	vx = (aki3d.RandomOne()*0.2f+0.2f)/240.0f;
}

void CStage::MoyaInfo::Update()
{
	x+=vx;

	if(x+width+axlen > 1536.0f/240.0f)
	{
		x = -1536.0f/240.0f - width - axlen;
	}
}
