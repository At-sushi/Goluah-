
/*==================================================================================
	
	l_directdraw.cpp
	
	CDirectDrawクラスの基本的な部分。
	主に初期化や破棄等の部分を記述

	MFCアプリケーションで使用する場合は「プリコンパイルヘッダーを使用しない」にする

====================================================================================*/


#ifndef GCD_EDITER
#include "stdafx.h"//本体の場合
#include "jpeglib.h"
#include "task_loading.h"
#else
enum NowLoading_IconItem
{
	NowLoading_DLL,
	NowLoading_Image,
	NowLoading_GCD,
};
#endif

#include <setjmp.h>
#include "png.h"
#include "dx_draw.h"
#define HALF_HEIGHT		(g_DISPLAYHEIGHT*0.5f)
#define HALF_HEIGHT2	(240.0f)

/*---------------------------------------------------------------------------------
	エディターの場合の設定
-----------------------------------------------------------------------------------*/
#ifdef GCD_EDITER//■エディタの場合-------------------------------------------------

#ifndef RELEASE
#define RELEASE(a)	if(a!=NULL){a->Release();a=NULL;}
#endif

extern int g_DISPLAYWIDTH;
extern int g_DISPLAYHEIGHT;
#define ASPECTRATIO ((float)g_DISPLAYWIDTH/(float)g_DISPLAYHEIGHT)

class CDummyCfg{
public:
	BOOL		NoAlphaTest(){return FALSE;}
	D3DFORMAT	TexFormat(){return D3DFMT_A1R5G5B5;}
	BOOL		IsHalfMode(){return FALSE;}
	D3DDEVTYPE	DeviceType(){return D3DDEVTYPE_HAL;}
};
CDummyCfg g_config;

#define ODS(a)	OutputDebugString(a)

#else//■Goluah本体の場合-----------------------------------------------------------

#include "define_const.h"
#include "define_macro.h"
#include "global.h"//configが必要

#ifdef _DEBUG
#define ODS(a)	gbl.ods2(a)
#else
#define ODS(a)	((void)0)
#endif

#endif

/*!
*	構築。
*	パラメータのリセットを行うのみ
*/
CDirectDraw::CDirectDraw()
{
	//パラメータリセット
	state = CDDSTATE_NOINITIALIZE;
	dd = NULL;
	d3ddev = NULL;
	//pSprite = NULL;
	pMyVertex = NULL;

	ZeroMemory(ms,sizeof(MYSURFACE)*MAXNUMGOLUAHTEXTURES);
	clearbgcolor = 0;
	ZeroMemory(lpFont,sizeof(LPD3DXFONT)*3);
}

/*!
*	初期化
*	Direct3Dの初期化を行う。
*	フルスクリーンモードの場合、この時点でフルスクリーン状態になる
*
*	@param hwnd アプリケーションのメインウインドウハンドル
*	@param win TRUE:ウインドウモード, FALSE:フルスクリーン
*	@return TRUE:成功, FALSE:失敗
*/
BOOL CDirectDraw::Initialize(HWND hwnd,BOOL win)
{
	Destroy();
	m_is_win = win;
	stencil_enable = false;

	if(!InitDirectDraw(hwnd,win)){
		Destroy();
		return FALSE;
	}

	//最大テクスチャサイズ取得
	D3DCAPS8 cap8;
	if(dd->GetDeviceCaps(D3DADAPTER_DEFAULT,devtypenow,&cap8)!=D3D_OK){
		Destroy();
		return FALSE;
	}
	maxtexturewidth = cap8.MaxTextureWidth;
	maxtextureheight = cap8.MaxTextureHeight;

	// ステート初期化
	InitStates();

	//x,y平面のデータセット
	d3dxplane_x.a=-1.0f;//x平面 (x=0)
	d3dxplane_x.b=0;
	d3dxplane_x.c=0;
	d3dxplane_x.d=0;
	d3dxplane_y.a=0;//y平面 (y=0)
	d3dxplane_y.b=-1.0f;
	d3dxplane_y.c=0;
	d3dxplane_y.d=0;

	//テクスチャに用いるフォーマット
	if (devtypenow == D3DDEVTYPE_HAL)
		texformat=g_config.TexFormat();
	else
		texformat = d3dpp.BackBufferFormat;

	//フォントの作成
	lpFont[0]=CreateMyFont( 8);
	lpFont[1]=CreateMyFont(16);
	lpFont[2]=CreateMyFont(22);
	lpFont[3]=CreateMyFont(32);

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	SetParentMatrix(mat,TRUE);

	center_x=0;
	camera_x=0;
	camera_y=0;
	camera_z=-2.99f;
	camera_zurax=0;
	camera_zuray=0;
	ResetTransformMatrix();

	// スプライト製造
	/*if ( FAILED(D3DXCreateSprite(d3ddev, &pSprite)) )
	{
		Destroy();
		return FALSE;
	}*/

	// 描画用の頂点バッファ
	if ( FAILED(d3ddev->CreateVertexBuffer(sizeof(MYVERTEX3D) * 4, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
											FVF_3DVERTEX, D3DPOOL_DEFAULT, &pMyVertex)) )
	{
		Destroy();
		return FALSE;
	}

	// ガンマランプを設定してみる
/*	D3DGAMMARAMP gr;

	for (int i = 0; i < 256; i++)
	{
		double gout = pow(i / 255.0, 1.0 / 2.2);
		gr.red[i] = gr.green[i] = gr.blue[i] = (WORD)(gout * 0xFFFF);
	}

	d3ddev->SetGammaRamp(D3DSGR_CALIBRATE, &gr);
*/
	return TRUE;
}

void CDirectDraw::InitStates()
{
	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);//カリングなし
    d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);//ライティング無し
	if (devtypenow != D3DDEVTYPE_HAL)
	{
		d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);//点だけ
		d3ddev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);//フラットシェーディング
	}
	else
	{
		//アルファブレンディング有効
		d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
		d3ddev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		d3ddev->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

		//これをやらないとディフューズ色が有効にならない
		d3ddev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		d3ddev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		d3ddev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

		//アルファテスト
		if(!g_config.NoAlphaTest()){
			if(d3ddev->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE ) ==D3D_OK){
				d3ddev->SetRenderState( D3DRS_ALPHAREF, 0x02 );
				d3ddev->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER );
			}
		}

		//ポイントスプライト
		d3ddev->SetRenderState(D3DRS_POINTSPRITEENABLE,TRUE);

		//マルチ
		//d3ddev->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);

		// ステンシル
		if (stencil_enable)
		{
			d3ddev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
			d3ddev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
		}
	}

	d3ddev->SetVertexShader( FVF_3DVERTEX );//とりあえず設定しとく
}


/*!
*	Direct3D初期化部分
*
*	Initializeのうち、Direct3D、Direct3DDeviceの生成部
*
*	@param hwnd アプリケーションのメインウインドウハンドル
*	@param win TRUE:ウインドウモード, FALSE:フルスクリーン
*	@return TRUE:成功, FALSE:失敗
*/
BOOL CDirectDraw::InitDirectDraw(HWND hwnd,BOOL win)
{
	// Direct3D オブジェクトを作成
	dd = Direct3DCreate8(D3D_SDK_VERSION);
    if (NULL == dd){
        MessageBox(hwnd,"Direct3DCreate8に失敗\nDirectXのバージョンが古いと思われ","エラー",MB_OK|MB_ICONSTOP);
        return(FALSE);
    }

	// Direct3D 初期化パラメータの設定
    ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.BackBufferCount = 1;
	d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// デバイスタイプ所得
	devtypenow=g_config.DeviceType();

	//ウインドウモードの場合の初期化
	if(win){
	    // 現在の画面モードを取得
	    D3DDISPLAYMODE d3ddm;
		if( dd->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) != D3D_OK ) {
			MessageBox(hwnd,"GetAdapterDisplayModeに失敗","エラー",MB_OK|MB_ICONSTOP);
			RELEASE(dd);
			return(FALSE);
		}
		// ウインドウ : 現在の画面モードを使用
	    d3dpp.BackBufferFormat = d3ddm.Format;
	    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	    d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = TRUE;
        d3dpp.BackBufferWidth = 0;
        d3dpp.BackBufferHeight = 0;
		
		m_desktop_width = d3ddm.Width;
		m_desktop_height = d3ddm.Height;
	}
	//フルスクリーンの場合の初期化
    else{
		// 使用可能な画面モードを探して選択
		if( dd->CheckDeviceType(D3DADAPTER_DEFAULT, devtypenow, D3DFMT_X8R8G8B8,
								D3DFMT_X8R8G8B8, FALSE) == D3D_OK)
								d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		else if( dd->CheckDeviceType(D3DADAPTER_DEFAULT, devtypenow, D3DFMT_X1R5G5B5,
								D3DFMT_X1R5G5B5, FALSE) == D3D_OK)
								d3dpp.BackBufferFormat = D3DFMT_X1R5G5B5;
		else if( dd->CheckDeviceType(D3DADAPTER_DEFAULT, devtypenow, D3DFMT_R5G6B5,
								D3DFMT_R5G6B5, FALSE) == D3D_OK)
								d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
		else { // ダメだぁ！
			TraceCreateDeviceError(D3DERR_INVALIDCALL,hwnd);
			Destroy();
			return(FALSE);
		}

	    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	    d3dpp.hDeviceWindow = hwnd;
        d3dpp.Windowed = FALSE;
        d3dpp.BackBufferWidth = g_DISPLAYWIDTH;
        d3dpp.BackBufferHeight = g_DISPLAYHEIGHT;

		m_desktop_width = g_DISPLAYWIDTH;
		m_desktop_height = g_DISPLAYHEIGHT;
    }
	// マルチサンプリング
	/*if ( SUCCEEDED(dd->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
				   D3DDEVTYPE_HAL, d3dpp.BackBufferFormat, d3dpp.Windowed,
				   D3DMULTISAMPLE_2_SAMPLES)) &&
		SUCCEEDED(dd->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
				   D3DDEVTYPE_HAL, D3DFMT_D16, d3dpp.Windowed,
				   D3DMULTISAMPLE_2_SAMPLES)) )
		d3dpp.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;*/ // VRAMを倍近く食うみたいなので保留

	// ステンシルが使えそうなら使う
	if ( SUCCEEDED(dd->CheckDeviceFormat(D3DADAPTER_DEFAULT, devtypenow, d3dpp.BackBufferFormat,
										 D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8)) )
	{
		stencil_enable = true;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	}
 
    // デバイスの作成
	DWORD vertexprocessmode=D3DCREATE_HARDWARE_VERTEXPROCESSING;

	if (devtypenow != D3DDEVTYPE_HAL)
		d3dpp.Flags |= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	HRESULT ret_devcreate = dd->CreateDevice(D3DADAPTER_DEFAULT,devtypenow,
		hwnd,vertexprocessmode,&d3dpp,&d3ddev);

	if(D3D_OK != ret_devcreate){//ありゃま
		vertexprocessmode=D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		ret_devcreate = dd->CreateDevice(D3DADAPTER_DEFAULT,devtypenow,
			hwnd,vertexprocessmode,&d3dpp,&d3ddev);// ソフト頂点処理でやり直し。

		if(D3D_OK != ret_devcreate){//やっぱだめか。
			// まともにゲーム出来んので封印
			/*devtypenow = D3DDEVTYPE_REF;
			d3dpp.Flags |= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

			ret_devcreate = dd->CreateDevice(D3DADAPTER_DEFAULT,devtypenow,
				hwnd,vertexprocessmode,&d3dpp,&d3ddev);// REF使用*/

			if(D3D_OK != ret_devcreate){//それでもダメってか？
				TraceCreateDeviceError(ret_devcreate,hwnd);
				return(FALSE);
			}
		}
	}

	return(TRUE);
}

/*!
*	破棄
*/
void CDirectDraw::Destroy()
{
	int i=0;
	DWORD notrelcount = 0;
	
	//フォントの削除
	for(int i=0;i<AKIDX_FONTNUM;i++){
		RELEASE(lpFont[i]);
	}

	//テクスチャの削除
	for(int i=0;i<MAXNUMGOLUAHTEXTURES;i++){
		if(ms[i].valid)notrelcount++;
		RelSurface(&ms[i]);
	}
	RELEASE(pMyVertex);
	//RELEASE(pSprite);
	RELEASE(d3ddev);
	RELEASE(dd);

	#ifndef GCD_EDITER
	if(notrelcount>0){
		gbl.ods("CDirectDraw::CleanDirectDraw [warning] MYSURFACE Remain Count : %d",notrelcount);
	}
	#endif
}


/*!
*	@brief 描画開始
*	毎フレームの描画を開始するときにこの操作を行うこと。
*	あらゆる描画処理は、この関数と、EndDraw の間に行わなければならない。
*
*	@param erbs TRUE:以前の描画内容を消去する
*	@sa clearbgcolor
*	@sa EndDraw
*/
void CDirectDraw::StartDraw(BOOL erbs)
{
	if(erbs){
		if (stencil_enable)
			d3ddev->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,clearbgcolor,1.0f,0);
		else
			d3ddev->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,clearbgcolor,1.0f,0);
	}
	d3ddev->BeginScene();
//	SetTransform(FALSE);
//	ReduceColor(0xCC);
//	SetTransform(TRUE);
}

/*!
*	バックバッファのクリア
*	Zバッファもクリアされる
*/
void CDirectDraw::ClearBackBuffer()
{
	d3ddev->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,clearbgcolor,1.0f,0);
}

/*!
*	描画終了（フリップ）
*	StartDraw からこの関数の呼び出しまでになされた描画内容が実際に表示される。
*
*	@sa StartDraw
*/
void CDirectDraw::EndDraw()
{
	d3ddev->EndScene();
	HRESULT hr = d3ddev->Present(NULL,NULL,NULL,NULL);

	if ( hr == D3DERR_DEVICELOST )
	{
		// DirectDrawデバイス再所得処理
		while (1)
		{
			// こんなところでウィンドウメッセージ所得
			MSG msg;

			if(PeekMessage( &msg, NULL, 0, 0 ,PM_REMOVE)){//message loop
				if (msg.message == WM_QUIT) break;
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			
			// ちょっと長めに
			Sleep(100);

			hr = d3ddev->TestCooperativeLevel();
			if (hr == D3DERR_DEVICENOTRESET)
			{
				ResetDirectDraw();
				break;
			}
		}
	}
}

/*!
*	テキスト描画用フォント生成
*	
*	@param h フォントのサイズ
*	@return フォント
*/
LPD3DXFONT CDirectDraw::CreateMyFont(DWORD h)
{
	HDC      hTextDC = NULL;
    HFONT    hFont = NULL, hOldFont = NULL;
	LPD3DXFONT cf;

    hTextDC = CreateCompatibleDC(NULL);
    hFont = CreateFont( h, 0, 0, 0,
                      FW_REGULAR,
                      FALSE,FALSE,FALSE,
                      SHIFTJIS_CHARSET,
                      OUT_DEFAULT_PRECIS,
                      CLIP_DEFAULT_PRECIS,
                      DEFAULT_QUALITY,
                      DEFAULT_PITCH,
                      "ＭＳ Ｐゴシック"
                      );
    if(!hFont) return(NULL);
    hOldFont = (HFONT)SelectObject(hTextDC, hFont);
    HRESULT ret=D3DXCreateFont( d3ddev, hFont, &cf );

    SelectObject(hTextDC, hOldFont);
    DeleteObject(hFont);

	if(ret!=D3D_OK)return(NULL);
	return(cf);
}

void CDirectDraw::ResetDirectDraw()
{
//	if (pSprite)
//		pSprite->OnLostDevice();
	RELEASE(pMyVertex);

	for (int i = 0; i < AKIDX_FONTNUM; i++)
		if (lpFont[i])
			lpFont[i]->OnLostDevice();

	d3ddev->Reset(&d3dpp);
	InitStates();

//	if (pSprite)
//		pSprite->OnResetDevice();
	d3ddev->CreateVertexBuffer(sizeof(MYVERTEX3D) * 4, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
											FVF_3DVERTEX, D3DPOOL_DEFAULT, &pMyVertex);

	for (int i = 0; i < AKIDX_FONTNUM; i++)
		if (lpFont[i])
			lpFont[i]->OnResetDevice();
}

// ウィンドウサイズ変更処理
/*void CDirectDraw::OnWindowResized(int width, int height)
{
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;

	ResetDirectDraw();
}*/

// スクリーンモード変更
BOOL CDirectDraw::ChangeScreenMode(BOOL win)
{
	if (win && d3dpp.Windowed == FALSE)
	{
		// ウィンドウモードに変更

	    // 現在の画面モードを取得
	    D3DDISPLAYMODE d3ddm;
		if( dd->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) != D3D_OK ) {
			MessageBox(d3dpp.hDeviceWindow,"GetAdapterDisplayModeに失敗","エラー",MB_OK|MB_ICONSTOP);
			RELEASE(dd);
			return(FALSE);
		}
		// ウインドウ : 現在の画面モードを使用
	    d3dpp.BackBufferFormat = d3ddm.Format;
		d3dpp.Windowed = TRUE;

		ResetDirectDraw();
		SetWindowLong(d3dpp.hDeviceWindow, GWL_STYLE, WS_BORDER | WS_CAPTION | WS_SYSMENU);
	}
	else if (!win && d3dpp.Windowed == TRUE)
	{
		// フルスクリーンモードに変更
		SetWindowLong(d3dpp.hDeviceWindow, GWL_STYLE, WS_POPUP);

		// 使用可能な画面モードを探して選択
		if( dd->CheckDeviceType(D3DADAPTER_DEFAULT, devtypenow, D3DFMT_X8R8G8B8,
								D3DFMT_X8R8G8B8, FALSE) == D3D_OK)
								d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		else if( dd->CheckDeviceType(D3DADAPTER_DEFAULT, devtypenow, D3DFMT_X1R5G5B5,
								D3DFMT_X1R5G5B5, FALSE) == D3D_OK)
								d3dpp.BackBufferFormat = D3DFMT_X1R5G5B5;
		else if( dd->CheckDeviceType(D3DADAPTER_DEFAULT, devtypenow, D3DFMT_R5G6B5,
								D3DFMT_R5G6B5, FALSE) == D3D_OK)
								d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
		else { // ダメだぁ！
			TraceCreateDeviceError(D3DERR_INVALIDCALL,d3dpp.hDeviceWindow);
			Destroy();
			return(FALSE);
		}
		d3dpp.Windowed = FALSE;

		ResetDirectDraw();
	}

	return (TRUE);
}


#ifndef _GOBJECT_H_
#define GBLEND_HANTOUMEI		(0)
#define GBLEND_KASAN			(1)
#define GBLEND_INV				(2)
#endif//_GOBJECT_H_

/*!
*	αブレンドのモードを設定する
*	通常の合成、加算合成、反転 が選べる
*/
void CDirectDraw::SetAlphaMode(DWORD alphamode)
{
	switch(alphamode){
	case GBLEND_KASAN://加算合成
		d3ddev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		d3ddev->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
		break;
/*	case GBLEND_GENSAN:
		d3ddev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ZERO);
		d3ddev->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCCOLOR);
		break;
	case GBLEND_JYOSAN:
		d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
		d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		break;
	case GBLEND_NAZO:
		d3ddev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVDESTCOLOR);
		d3ddev->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
		break;*/
	case GBLEND_INV:
		d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
		d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		break;
	case GBLEND_HANTOUMEI:
	default:
		d3ddev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		d3ddev->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
		break;
	}
}

/*!
*	Zテスト・ライトの有効・無効を設定
*
*	@param test TRUE:ｚテストを行う
*	@param write TRUE:ｚバッファーへの書き込みを行う
*/
void CDirectDraw::EnableZ(BOOL test,BOOL write)
{
	if(test)
		d3ddev->SetRenderState(D3DRS_ZFUNC		,D3DCMP_LESSEQUAL);
	else
		d3ddev->SetRenderState(D3DRS_ZFUNC		,D3DCMP_ALWAYS );
	d3ddev->SetRenderState(D3DRS_ZWRITEENABLE	,write);
}


/*!
*	CreateDeviceに失敗した理由をMessageBoxで表示する
*
*	D3DのCreateDeviceが返したリターンコードから、SDKヘルプの解説をコピペしたものに
*	直してメッセージボックス表示する。
*
*	@param ret CreateDeviceが返したリターンコード
*	@param hwnd メッセージボックスの親になるウインドウハンドル
*/
void CDirectDraw::TraceCreateDeviceError(HRESULT ret,HWND hwnd)
{
	char *msg;
	msg=(char*)malloc(1024);
	sprintf(msg,"");

	switch(ret){
	case D3DERR_INVALIDCALL:
		sprintf(&msg[strlen(msg)],"D3DERR_INVALIDCALL:");
		sprintf(&msg[strlen(msg)],"メソッドの呼び出しが無効である。\n");
		sprintf(&msg[strlen(msg)],"　　たとえば、メソッドのパラメータに無効な値が設定されている場合など。");
		sprintf(&msg[strlen(msg)],"\n\n･･･だそうです。");
		sprintf(&msg[strlen(msg)],"\n・ディスプレイの表示色数を変更してください");
		sprintf(&msg[strlen(msg)],"\n・ディスプレイのプロパティ/詳細/でハードウェアアクセラレータ最大にしてください");
		sprintf(&msg[strlen(msg)],"\n・「ファイル名を指定して実行」からdxdiag.exeを実行し、");
		sprintf(&msg[strlen(msg)],"\n\n　　「ディスプレイ」タブでDirect3Dアクセラレータが「使用可能」であることを確かめてください。");
		sprintf(&msg[strlen(msg)],"\n・g_config.exeで「デバイスタイプ」を変更してください");
		break;
	case D3DERR_NOTAVAILABLE:
		sprintf(&msg[strlen(msg)],"D3DERR_NOTAVAILABLE:");
		sprintf(&msg[strlen(msg)],"このデバイスは、照会されたテクニックをサポートしていない。");
		sprintf(&msg[strlen(msg)],"\n\n･･･だそうです。");
		sprintf(&msg[strlen(msg)],"\nグラフィックカードが向いてないみたいなんで、買い換えませう。");
		break;
	case D3DERR_OUTOFVIDEOMEMORY:
		sprintf(&msg[strlen(msg)],"D3DERR_OUTOFVIDEOMEMORY");
		sprintf(&msg[strlen(msg)],"Direct3D が処理を行うのに十分なディスプレイ メモリがない。");
		sprintf(&msg[strlen(msg)],"\n\n･･･だそうです。");
		sprintf(&msg[strlen(msg)],"\n・ディスプレイの解像度と色数をできるだけ減らしてください。");
		sprintf(&msg[strlen(msg)],"\n・g_config.exeで320x240モードにしてみてください");
		sprintf(&msg[strlen(msg)],"\n・PCを買い換えてください");
		break;
	default:
		sprintf(&msg[strlen(msg)],"エラーが特定できませんでした。");
	}

	MessageBox(hwnd,msg,"起動に失敗",MB_OK);
	free(msg);
}








/*==================================================================================
	
	


	ビットマップのロード/アンロード



====================================================================================*/



/*!
*	256ビットマップを独自形式でロード
*
*	256色でなければならないのは、パレットの挿げ替えを行わなければならないため。
*	ビットマップのサイズから適切なテクスチャサイズを計算し、複数に分割してロードする。
*	尚、この関数で生成されたインチキサーフェイスはCDirectDrawクラスがリスト管理し、
*	クラスの破棄時にアンロードされていないサーフェイスを破棄する（最終手段）
*
*	@param filename イメージを読み込むビットマップファイルのファイル名。
*					palnameがNULLでないときは、このファイルのパレットは無視される
*	@param palname パレットを読み込むビットマップファイル名。
*					NULLの場合はfilenameのファイルのパレットが適用される
*	@param dmy 未使用。まだDirectDrawを使っていた頃、VRAMに読むかシステムメモリに読むかのフラグだったもの。
*
*	@return 独自形式インチキサーフェイスのポインタ。失敗した場合はNULL
*/
MYSURFACE* CDirectDraw::CreateSurfaceFrom256BMP(char *filename,char *palname,BOOL dmy)
{
	BOOL bret;
	DWORD i,j;

	//未使用のものを探す
	int e=-1;
	for(i=0;i<MAXNUMGOLUAHTEXTURES;i++){
		if(!ms[i].valid){
			e=i;
			i=MAXNUMGOLUAHTEXTURES;
		}
	}
	if(e<0){
		ODS("warning:テクスチャ数がMAXを超えています\n");
		return(NULL);
	}

	//ビットマップのビットを読み込み
	MYPALLET *bmpbits;
	DWORD bmpwidth,bmpheight;
#ifdef GCD_EDITER
	bret = FALSE;	// 使えない、パス。
#else
	bret = LoadJPEGbits(&bmpbits, &bmpwidth, &bmpheight, filename);
#endif	// GCD_EDITER
	if (!bret)
	{
		bret = Load256PNGbits(&bmpbits,&bmpwidth,&bmpheight,filename,palname);
		if(!bret){
			bret = Load256Bitmapbits(&bmpbits,&bmpwidth,&bmpheight,filename,palname);

			if(!bret){
				return(NULL);
			}
		}
	}
	ms[e].wg = (float)bmpwidth;
	ms[e].hg = (float)bmpheight;

	//ビットマップの分割の仕方を決定
	bret = AssignTextureDiv(&ms[e],bmpwidth,bmpheight);
	if(!bret){
		ODS("AssignTextureDivに失敗\n");
		return(NULL);
	}

	//指定サイズのテクスチャを作成
	LPVOID pointer;
	pointer = malloc( sizeof(LPDIRECT3DTEXTURE8)*(ms[e].xsufnum*ms[e].ysufnum+1));
	ZeroMemory(pointer,sizeof(LPDIRECT3DTEXTURE8)*(ms[e].xsufnum*ms[e].ysufnum+1));
	ms[e].pTex = (LPDIRECT3DTEXTURE8*)pointer;//ポインタを確保する領域を確保
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

	//テクスチャにビットマップをコピー
	DWORD damex,damey;
	for(i=0;i<ms[e].ysufnum;i++){//okasi?
		for(j=0;j<ms[e].xsufnum ;j++){
			//コピー実行
			if(i==ms[e].ysufnum-1){damey=dameyoy;}
			else damey=0;
			if(j==ms[e].xsufnum-1){damex=dameyox;}
			else damex=0;
			CopyBB2TS(bmpbits,bmpwidth,
				ms[e].xsufindx[j],ms[e].ysufindx[i],
				ms[e].pTex[ (i*ms[e].xsufnum+j) ],damex,damey);
		}
	}

	//後始末
	free(bmpbits);
	//成功
	ms[e].valid=TRUE;

	if(g_config.IsHalfMode()){
		ms[e].wg *=2;
		ms[e].hg *=2;
	}

	return(&ms[e]);
}

/*!
*	256色画像を独自形式でロード
*
*	PNG対応に伴い、読み込み処理に柔軟性を求めるために作りました。
*	拡張子を除いたファイル名から、.pngと.bmpの２種類のタイプから自動で読み込みます(PNG優先）。
*	あとは、ほとんどCreateSurfaceFrom256BMPと変わりません。
*
*	@param filename イメージを読み込むファイルの、拡張子を除いたファイル名。
*					palnameがNULLでないときは、このファイルのパレットは無視される
*	@param palname パレットを読み込むファイルの、拡張子を除いたファイル名。
*					NULLの場合はfilenameのファイルのパレットが適用される
*	@param dmy 未使用。まだDirectDrawを使っていた頃、VRAMに読むかシステムメモリに読むかのフラグだったもの。
*
*	@return サーフェイスのポインタ。失敗した場合はNULL
*
*	@sa CreateSurfaceFrom256BMP
*/
MYSURFACE* CDirectDraw::CreateSurfaceFrom256Image(char *filename,char *pallet/*=NULL*/,BOOL dmy/*=TRUE*/)
{
	if (filename)
	{
		char* Buffer = (char*)malloc( strlen(filename) + (4 + 1));

		if (Buffer)
		{
			char* Buffer_pal = NULL;
			MYSURFACE* result;

			if (pallet)
				Buffer_pal = (char*)malloc( strlen(pallet) + (4 + 1));

			// まずはPNG形式で。
			sprintf(Buffer, "%s%s", filename, ".png");
			if (Buffer_pal) sprintf(Buffer_pal, "%s%s", pallet, ".png");

			if ( result = CreateSurfaceFrom256BMP(Buffer, Buffer_pal, dmy) )
			{
				// 成功
				if (Buffer_pal) free(Buffer_pal);
				free(Buffer);
				return result;
			}
			else
			{
				// パレットをBMPに。
				if (Buffer_pal)
				{
					sprintf(Buffer_pal, "%s%s", pallet, ".bmp");

					if ( result = CreateSurfaceFrom256BMP(Buffer, Buffer_pal, dmy) )
					{
						// 成功
						if (Buffer_pal) free(Buffer_pal);
						free(Buffer);
						return result;
					}
				}

				// JPEG形式でやってみる。
				sprintf(Buffer, "%s%s", filename, ".jpg");

				if ( result = CreateSurfaceFrom256BMP(Buffer, NULL, dmy) )
				{
					// 成功
					if (Buffer_pal) free(Buffer_pal);
					free(Buffer);
					return result;
				}

				// 失敗したらビットマップ形式で。
				sprintf(Buffer, "%s%s", filename, ".bmp");
				if (Buffer_pal) sprintf(Buffer_pal, "%s%s", pallet, ".png");

				if ( result = CreateSurfaceFrom256BMP(Buffer, Buffer_pal, dmy) )
				{
					// 成功
					if (Buffer_pal) free(Buffer_pal);
					free(Buffer);
					return result;
				}
				else
				{
					// パレットをBMPに。
					if (Buffer_pal)
					{
						sprintf(Buffer_pal, "%s%s", pallet, ".bmp");

						result = CreateSurfaceFrom256BMP(Buffer, Buffer_pal, dmy);
					}
				}
			}

			if (Buffer_pal) free(Buffer_pal);
			free(Buffer);
			return result;
		}
	}

	return NULL;
}

/*!
*	テクスチャのサーフェースに書き込み
*	CreateSurfaceFrom256BMPの内部関数で、テクスチャーのサーフェイスに
*	ビットマップファイルのイメージをコピーする。
*
*	@param pbb 適用パレットの配列
*	@param bbpitch ソース(ビットマップ)のピッチ
*	@param offset_x ソース(ビットマップ)のコピー位置Xオフセット
*	@param offset_y ソース(ビットマップ)のコピー位置Yオフセット
*	@param damex コピーしちゃダメな部分（テクスチャサイズ > ビットマップの残りのコピー領域 のとき）
*	@param damey コピーしちゃダメな部分（テクスチャサイズ > ビットマップの残りのコピー領域 のとき）
*	@return TRUE:成功, FALSE:残念
*/
BOOL CDirectDraw::CopyBB2TS(MYPALLET *pbb,
							DWORD bbpitch,
							DWORD offset_x,
							DWORD offset_y,
							LPDIRECT3DTEXTURE8 ptex,
							DWORD damex,
							DWORD damey)
{
	if(pbb==NULL)return(FALSE);
	if(ptex==NULL)return(FALSE);

	LPDIRECT3DSURFACE8 psuf=NULL;
	if(D3D_OK != ptex->GetSurfaceLevel(0,&psuf)){
		ODS("CopyBB2TS / GetSurfaceLevelに失敗\n");
		return(FALSE);
	}

	//幅と高さ、フォーマットを取得
	D3DFORMAT fmt;
	DWORD sw,sh;
	D3DSURFACE_DESC dsc;
	if(D3D_OK != psuf->GetDesc(&dsc)){
		ODS("CopyBB2TS / GetDescに失敗\n");
		RELEASE(psuf);
		return(FALSE);
	}
	fmt = dsc.Format;
	sw = dsc.Width;
	sh = dsc.Height;

	if(damex!=0)sw=damex;
	if(damey!=0)sh=damey;

	//サーフェイスのロック
	D3DLOCKED_RECT lr;
	if(D3D_OK != psuf->LockRect(&lr,NULL,0)){
		ODS("CopyBB2TS / LockRectに失敗\n");
		RELEASE(psuf);
		return(FALSE);
	}

	//コピー
	DWORD i,j;
	PBYTE pline;
	int onepixsize;
	for(j=0;j<sh;j++){
		pline =  (PBYTE)lr.pBits + lr.Pitch*j ;
		for(i=0;i<sw;i++){
			switch(fmt){
			case D3DFMT_R5G6B5://これは多分使えない
				onepixsize = CopyOne_R5G6B5(pbb[(j+offset_y)*bbpitch+i+offset_x],pline);
				break;
			case D3DFMT_A8R8G8B8:
			case D3DFMT_X8R8G8B8:
				i=i;
				j=j;
				onepixsize = CopyOne_A8R8G8B8(pbb[(j+offset_y)*bbpitch+i+offset_x],pline);
				break;
			case D3DFMT_A1R5G5B5:
			case D3DFMT_X1R5G5B5:
				onepixsize = CopyOne_A1R5G5B5(pbb[(j+offset_y)*bbpitch+i+offset_x],pline);
				break;
			case D3DFMT_A4R4G4B4:
			case D3DFMT_X4R4G4B4:
				onepixsize = CopyOne_A4R4G4B4(pbb[(j+offset_y)*bbpitch+i+offset_x],pline);
				break;
			case D3DFMT_A8R3G3B2:
				onepixsize = CopyOne_A8R3G3B2(pbb[(j+offset_y)*bbpitch+i+offset_x],pline);
				break;
			default:
				ODS("CopyBB2TS / このフォーマットはコピーできにゃい\n");
				psuf->UnlockRect();
				RELEASE(psuf);
				return(FALSE);
			}
			pline += onepixsize;
		}
	}

	psuf->UnlockRect();	//サーフェイスのアンロック
	RELEASE(psuf);		//GetSurfaceLevelでサーフェイスを取得したときにリファレンスカウントが増えてるから

	return(TRUE);
}


/*!
*	テクスチャのサーフェースへの 1 ピクセルの書き込み処理
*
*	CopyBB2TSの内部関数。パレットの色(ARGB=8888)を、ARGB=0565 の形式に
*	変換して、指定ポインタ位置に書き込む。
*
*	@param src 指定色
*	@param dst 書き込み先
*	@retuen 書き込んだバイト数(常に2)
*/
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

/*!
*	テクスチャのサーフェースへの 1 ピクセルの書き込み処理
*
*	CopyBB2TSの内部関数。パレットの色(ARGB=8888)を、ARGB=1555 の形式に
*	変換して、指定ポインタ位置に書き込む。
*
*	@param src 指定色
*	@param dst 書き込み先
*	@retuen 書き込んだバイト数(常に2)
*/
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

/*!
*	テクスチャのサーフェースへの 1 ピクセルの書き込み処理
*
*	CopyBB2TSの内部関数。パレットの色(ARGB=8888)を、ARGB=4444 の形式に
*	変換して、指定ポインタ位置に書き込む。
*
*	@param src 指定色
*	@param dst 書き込み先
*	@retuen 書き込んだバイト数(常に2)
*/
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

/*!
*	テクスチャのサーフェースへの 1 ピクセルの書き込み処理
*
*	CopyBB2TSの内部関数。パレットの色(ARGB=8888)を、ARGB=8332 の形式に
*	変換して、指定ポインタ位置に書き込む。
*
*	@param src 指定色
*	@param dst 書き込み先
*	@retuen 書き込んだバイト数(常に2)
*/
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

/*!
*	テクスチャのサーフェースへの 1 ピクセルの書き込み処理
*
*	CopyBB2TSの内部関数。パレットの色(ARGB=8888)を、ARGB=8888 の形式に
*	変換して、指定ポインタ位置に書き込む。
*
*	@param src 指定色
*	@param dst 書き込み先
*	@retuen 書き込んだバイト数(常に4)
*/
DWORD CDirectDraw::CopyOne_A8R8G8B8(MYPALLET src,PBYTE dst)
{
	MYPALLET *pdst;
	pdst = (MYPALLET*)dst;

	*pdst = src;

	return(4);
}


/*!
*	指定サイズのビットマップの分割方法決定
*
*	CreateSurfaceFrom256BMPの内部関数。
*	ビットマップのタテヨコを2の累乗で分割するために、どういうサイズで分割するかを計算する。
*
*	@param pmsf [out]インチキサーフェイスのポインタ。分割サイズのパラメータを書き込んで返す
*	@param width [in]元のビットマップの幅
*	@param height [in]元のビットマップの高さ
*	@return TRUE:成功, FALSE:残念
*/
BOOL CDirectDraw::AssignTextureDiv(MYSURFACE *pmsf,DWORD width,DWORD height)
{
	DWORD i;

	DWORD ntx,nty,numsufx=0,numsufy=0;
	BOOL bloop;
	
	//ヨコ方向
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
		if(bmpwidthr==0)bloop=FALSE;//きっちり終わったらそれで終了
		else if(asstexsize <= MINIMUM_TEXSIZE){//最後は細かいから省略だ
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

	//タテ方向
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
		if(bmpheightr==0)bloop=FALSE;//きっちり終わったらそれで終了
		else if(asstexsize <= MINIMUM_TEXSIZE){//最後は細かいから省略だ
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

	return(TRUE);
}


static BOOL GoluahReadFile(HANDLE hFile, LPBYTE bits, DWORD sizeimage, NowLoading_IconItem item, LPDWORD br);

/*!
*	@brief 指定ファイル名のビットマップから、画像ビットを読み込み
*
*	CreateSurfaceFrom256BMPの内部関数。
*	読み込んで生成した配列はmalloc関数で作られてるみたい。freeで始末してください。
*
*	@param pbits [out] 読み込み結果. すでにパレット(ARGB=8888)の配列に変換されてます
*	@param width [out] ビットマップの幅
*	@param height [out] ビットマップの高さ
*	@param bmpfilename [in] ビットマップのファイル名
*	@param palfilename [in] 適用するパレットのファイル名
*	@return TRUE:成功, FALSE:残念
*/
BOOL CDirectDraw::Load256Bitmapbits(MYPALLET **pbits,DWORD *width,DWORD *height,char *bmpfilename,char *palfilename)
{
	BOOL ret2;

	//とりあえずパレットを確保
	MYPALLET pal[256];
	if(palfilename==NULL)palfilename=bmpfilename;
	ret2 = GetPallet(palfilename,pal);
	if(!ret2){
		ret2 = GetPalletPNG(palfilename,pal);

		if(!ret2){
			return(FALSE);
		}
	}

	//ビットマップのビットを取得~
	//ビットマップをロード
	HANDLE hFile;
	DWORD ret,br,err=FALSE;

	if(bmpfilename==NULL){
		ODS("CDirectDraw::Load256Bitmapbits : ファイル名がNULLとはどういうことだ？\n");
	}

	//ファイルのオープン
	hFile=CreateFile(bmpfilename,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		return(NULL);
	}
	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//念のためファイルの先頭に移動

	//ﾌｧｲﾙヘッダの読み出し
	BITMAPFILEHEADER fileheader;
	ret=ReadFile(hFile,&fileheader,sizeof(BITMAPFILEHEADER),&br,NULL);
	if(!ret || br!=sizeof(BITMAPFILEHEADER)){
		ODS("CDirectDraw::Load256Bitmapbits : ファイルの読み込みに失敗(1)\n");
		err=TRUE;
	}
	if(fileheader.bfType != 0x4d42){//"BM"
		ODS("CDirectDraw::Load256Bitmapbits : つーかこのファイルはビットマップではない\n");
		err=TRUE;
	}
	if(err){
		CloseHandle(hFile);
		return(NULL);
	}

	//BITMAPINFOHEADERの読み込み
	BITMAPINFOHEADER infohed;
	ret=ReadFile(hFile,&infohed,sizeof(BITMAPINFOHEADER),&br,NULL);
	if(!ret || br!=sizeof(BITMAPINFOHEADER)){
		ODS("CDirectDraw::Load256Bitmapbits : ファイルの読み込みに失敗(2)\n");
		err=TRUE;
	}
	if(infohed.biSize != sizeof(BITMAPINFOHEADER)){
		ODS("CDirectDraw::Load256Bitmapbits : BITMAPINFOHEADERのサイズが合わない\n");
		err=TRUE;
	}
	if(infohed.biBitCount != 8){
		ODS("CDirectDraw::Load256Bitmapbits : つーかこのファイルは256ではない\n");
		err=TRUE;
	}
	if(infohed.biCompression != BI_RGB){
		ODS("CDirectDraw::Load256Bitmapbits : 圧縮がかかっているらしい\n");
		err=TRUE;
	}
	if(err){
		CloseHandle(hFile);
		return(NULL);
	}
	if(infohed.biClrUsed == 0){//0の場合、256を意味することがあるらしい
		infohed.biClrUsed=256;
	}

	//パレットは読みとばし
	SetFilePointer(hFile,sizeof(RGBQUAD)*infohed.biClrUsed,NULL,FILE_CURRENT);

	//ビットマップビットのサイズを計算する(ビットマップの幅は4の倍数で格納されているらしい)
	DWORD linesize = infohed.biWidth;
	if(infohed.biWidth%4 != 0)linesize +=  ( 4 - infohed.biWidth%4 );
	DWORD sizeimage = linesize * infohed.biHeight;
	//メモリを確保してビットマップビットを読み込む
	LPBYTE bits = (LPBYTE)(malloc(sizeimage));
	ret=GoluahReadFile(hFile,bits,sizeimage,NowLoading_Image,&br);
	CloseHandle(hFile);
	if(!ret || br!=sizeimage){
		OutputDebugString("CDirectDraw::Load256Bitmapbits : ファイルの読み込みに失敗(3)\n");
		free(bits);
		return(NULL);
	}

	//ビットマップの大きさにあわせて新たにメモリ領域を作成する
	LPVOID pnewbits2;
	DWORD ishalf=1;
	if(g_config.IsHalfMode())ishalf=2;
	pnewbits2 = malloc(sizeof(MYPALLET)*infohed.biWidth*infohed.biHeight /ishalf);
	MYPALLET *retbit;
	retbit = (MYPALLET*)pnewbits2;

	//そこにデータをコピー
	LONG i,j;
	PBYTE plinenow;
	DWORD halfcopy=0;
	for(i=infohed.biHeight-1;i>=0;i--){
		plinenow = bits;
		plinenow += linesize*i;
		for(j=0;j<infohed.biWidth;j++){
			if(g_config.IsHalfMode()){//半分しかコピーしない
				if(i%2==0 && j%2==0 && j != infohed.biWidth - 1){
					retbit[halfcopy] = pal[ plinenow[j] ];
					halfcopy++;
				}
			}
			else{
				retbit[(infohed.biHeight-1-i)*infohed.biWidth + j] = pal[ plinenow[j] ];
			}
		}
	}

	//完了
	free(bits);

	if(g_config.IsHalfMode()){
		infohed.biWidth/=2;
		infohed.biHeight/=2;
	}

	*pbits = retbit;
	*width = infohed.biWidth;
	*height = infohed.biHeight;
	return(TRUE);
}

static BOOL GoluahReadFile(HANDLE hFile, LPBYTE bits, DWORD sizeimage, NowLoading_IconItem item, LPDWORD br)
{
	DWORD ret,br2,err=0;

	*br = 0;

	for (int i = 0; i < sizeimage; i += 524288)
	{
		DWORD LoadBytes = min(524288, sizeimage - i);

		ret=ReadFile(hFile,bits + i,LoadBytes,&br2,NULL);
		if(!ret || br2!=LoadBytes){
			return(ret);
		}

		*br += br2;
#		ifndef GCD_EDITER
			//進行状況表示
			CTNowLoading* task = dynamic_cast<CTNowLoading*>( g_system.FindTask('LOAD') );
			if(task)task->Progress(item, (float)((double)i / sizeimage));
#		endif
	}

	return(ret);
}

// 例外処理が2重なので、専用のジャンプ処理を用意。
jmp_buf PngErrJamp;
void PngErrHandler(png_structp Png,png_const_charp message)
{
#ifndef GCD_EDITER
	gbl.ods("PNG読み込みエラー：%s", message);
#endif // GCD_EDITER

//	throw(0x46497743);
	longjmp(PngErrJamp, 0x46497743);
}

#ifndef GCD_EDITER

// プログレス表示用のコールバック
static void png_read_row_callback(png_structp strPNG, png_uint_32 row, int pass)
{
	if (row * strPNG->rowbytes % 524288 < strPNG->rowbytes)
	{
			//進行状況表示
			CTNowLoading* task = dynamic_cast<CTNowLoading*>( g_system.FindTask('LOAD') );
			if(task)task->Progress(NowLoading_Image, (float)((double)row / strPNG->height));
	}
}

// インターレースのある画像用
static void png_read_row_callback_adam7(png_structp strPNG, png_uint_32 row, int pass)
{
	if (row == 0 && pass >= 3)
	{
			//進行状況表示
			CTNowLoading* task = dynamic_cast<CTNowLoading*>( g_system.FindTask('LOAD') );
			if(task)task->Progress(NowLoading_Image, (float)((double)pass*pass / (8*8)));			// 最大pass値の所得方法がないみたいなので･･･
	}
}

#endif	// GCD_EDITER

/*!
*	@brief 指定ファイル名のPNGファイルから、画像ビットを読み込み
*
*	CreateSurfaceFrom256BMPの内部関数、Load256BitmapBitsのPNG版。
*	Load256BitmapBitsと同じく、pbitsはfreeで始末してください。
*
*	@param pbits [out] 読み込み結果. すでにパレット(ARGB=8888)の配列に変換されてます
*	@param width [out] ビットマップの幅
*	@param height [out] ビットマップの高さ
*	@param pngfilename [in] ビットマップのファイル名
*	@param palfilename [in] 適用するパレットのファイル名
*	@return TRUE:成功, FALSE:残念
*	@sa Load256Bitmapbits
*/
BOOL CDirectDraw::Load256PNGbits(MYPALLET **pbits,DWORD *width,DWORD *height,char *pngfilename,char *palfilename)
{
	BOOL ret2;

	//とりあえずパレットを確保
	MYPALLET pal[256];
	if(palfilename==NULL)palfilename=pngfilename;
	ret2 = GetPalletPNG(palfilename,pal);
	if(!ret2){
		ret2 = GetPallet(palfilename,pal);

		if(!ret2){
			return(FALSE);
		}
	}

	//PNGをさっくりロード
	FILE* fp;

	if(pngfilename==NULL){
		ODS("CDirectDraw::Load256PNGbits : ファイル名がNULLとはどういうことだ？\n");
		return(FALSE);
	}

	// PNG構造体？
	png_structp strPNG = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, PngErrHandler, NULL);
	if (!strPNG)
	{
		ODS("CDirectDraw::Load256PNGbits : PNG構造体ﾃﾞｷﾃﾈｴﾖ!!ヽ(`Д´)ﾉｳﾜｧｧﾝ!!\n");
		return FALSE;
	}

	// 情報構造体…って何じゃ。
	png_infop infoPNG = png_create_info_struct(strPNG);
	if (!infoPNG)
	{
		ODS("CDirectDraw::Load256PNGbits : PNG情報構造体ﾃﾞｷﾃﾈｴﾖ!!ヽ(`Д´)ﾉｳﾜｧｧﾝ!!\n");
		png_destroy_read_struct(&strPNG, NULL, NULL);
		return FALSE;
	}

	// 読み込むか。
	fp = fopen(pngfilename, "rb");
	if (!fp)
	{
		png_destroy_read_struct(&strPNG, &infoPNG, NULL);
		return FALSE;
	}

	// tryの外で使うから
	LONG i,j;
	BYTE** Image = NULL;
	MYPALLET *retbit = NULL;

//	try {
	if (!setjmp(PngErrJamp)) {
		// あとはライブラリに任せる
		png_init_io(strPNG, fp);

		// シグネチャ解析
		BYTE sig[8];
		long pos = ftell(fp);

		int len = fread(sig, sizeof(BYTE), 8, fp);
		if ( png_sig_cmp(sig, 0, len) != 0 )
		{
			//ODS("CDirectDraw::Load256PNGbits : つーかこのファイルはPNGではない。\n");
			fclose(fp);
			png_destroy_read_struct(&strPNG, &infoPNG, NULL);
			return FALSE;
		}

		// 先頭に戻す
		fseek(fp, pos, SEEK_SET);

		// PNG情報ゲッツ
		png_read_info(strPNG, infoPNG);

		// イメージヘッダ所得
		int bit_depth = 0, color_type = 0;

		if ( !png_get_IHDR(strPNG, infoPNG, (UINT*)width, (UINT*)height, &bit_depth, &color_type, NULL, NULL, NULL) )
		{
			ODS("CDirectDraw::Load256PNGbits : IHDR読み込み失敗\n");
			fclose(fp);
			png_destroy_read_struct(&strPNG, &infoPNG, NULL);
			return FALSE;
		}

		// パレット付きの256色じゃないとだめぽ
		if (color_type != 3)
		{
			ODS("CDirectDraw::Load256PNGbits : パレットがないよ。\n");
			fclose(fp);
			png_destroy_read_struct(&strPNG, &infoPNG, NULL);
			return FALSE;
		}
		if (bit_depth != 8)
		{
			ODS("CDirectDraw::Load256PNGbits : つーかこのファイルは256ではない。\n");
			fclose(fp);
			png_destroy_read_struct(&strPNG, &infoPNG, NULL);
			return FALSE;
		}

		//ビットマップの大きさにあわせて新たにメモリ領域を作成する
		Image = (BYTE**)malloc(*height * sizeof(BYTE**));
		LPVOID pnewbits2;
		DWORD ishalf=1;
		if(g_config.IsHalfMode())ishalf=2;
		pnewbits2 = malloc(sizeof(MYPALLET)* *width * *height / ishalf);
		retbit = (MYPALLET*)pnewbits2;

		DWORD halfcopy=0;

		// ２次元動的配列
		for (i = 0; i < (LONG)*height; i++)
			Image[i] = (BYTE*)malloc(png_get_rowbytes(strPNG, infoPNG));

#		ifndef GCD_EDITER
			// コールバック設定
			if (png_get_interlace_type(strPNG, infoPNG) == PNG_INTERLACE_ADAM7)
				png_set_read_status_fn(strPNG, png_read_row_callback_adam7);	// ADAM7
			else
				png_set_read_status_fn(strPNG, png_read_row_callback);			// 標準（インターレース無し）
#		endif	// GCD_EDITER

		// 画像読み込み
	
		png_read_image(strPNG, Image);

		//そこにデータをコピー
		for(i=0;i<(LONG)*height;i++){
			for(j=0;j<(LONG)*width;j++){
				if(g_config.IsHalfMode()){//半分しかコピーしない
					if(i%2==0 && j%2==0 && j != *width - 1){
						retbit[halfcopy] = pal[ Image[i][j] ];
						halfcopy++;
					}
				}
				else{
					retbit[i * *width + j] = pal[ Image[i][j] ];
				}
			}
		}
	}
//	catch (int)
	else
	{
		// ケホパホ発生
		BOOL ret = FALSE;

		if (retbit)
		{
			if (Image)
			{
				DWORD halfcopy=0;

				// 無理矢理返す
				for(i=0;i<(LONG)*height;i++){
					for(j=0;j<(LONG)*width;j++){
						if(g_config.IsHalfMode()){//半分しかコピーしない
							if(i%2==0 && j%2==0 && j != *width - 1){
								retbit[halfcopy] = pal[ Image[i][j] ];
								halfcopy++;
							}
						}
						else{
							retbit[i * *width + j] = pal[ Image[i][j] ];
						}
					}
				}

				*pbits = retbit;

				ret = TRUE;
			}
			else
				free(retbit);
		}

		if (Image)
		{
			for (i = 0; i < (LONG)*height; i++)
						{if (Image[i]) free(Image[i]);}

			free(Image);
		}

		fclose(fp);
		png_destroy_read_struct(&strPNG, &infoPNG, NULL);

		if(ret && g_config.IsHalfMode()){
			*width/=2;
			*height/=2;
		}

		return ret;
	}

	//完了
	for (i = 0; i < (LONG)*height; i++)
		free(Image[i]);
	free(Image);

	fclose(fp);
	png_destroy_read_struct(&strPNG, &infoPNG, NULL);

	if(g_config.IsHalfMode()){
		*width/=2;
		*height/=2;
	}

	*pbits = retbit;

	return TRUE;
}

/*!
*	@brief 指定ファイル名のJPEGファイルから、画像ビットを読み込み
*
*	CreateSurfaceFrom256BMPの内部関数、Load256BitmapBitsのJPEG版。
*	Load256BitmapBitsと同じく、pbitsはfreeで始末してください。
*	他の関数と違い、パレットを使わずフルカラーで読み込みます。透明色はなし。
*
*	@param pbits [out] 読み込み結果. すでにパレット(ARGB=8888)の配列に変換されてます
*	@param width [out] ビットマップの幅
*	@param height [out] ビットマップの高さ
*	@param jpegfilename [in] ビットマップのファイル名
*	@return TRUE:成功, FALSE:残念
*	@sa Load256Bitmapbits
*	@sa Load256PNGbits
*/
#ifndef GCD_EDITER
BOOL CDirectDraw::LoadJPEGbits(MYPALLET **pbits,DWORD *width,DWORD *height,char *jpegfilename)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE* fp;
	int i,j;

	// 初期化
	ZeroMemory(&cinfo, sizeof(cinfo));
	ZeroMemory(&jerr, sizeof(jerr));
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	// ファイル読み込み
	fp = fopen(jpegfilename, "rb");
	if (!fp)
	{
		jpeg_destroy_decompress(&cinfo);
		return FALSE;
	}
	// シグネチャ解析
	WORD startsig = 0;
	long pos = ftell(fp);
	int sigr = fread(&startsig, sizeof(WORD), 1, fp);
	if ((sigr != 1) || (startsig != 0xD8FF))
	{
		// 違うっぽ
		jpeg_destroy_decompress(&cinfo);
		fclose(fp);
		return FALSE;
	}
	fseek(fp, pos, SEEK_SET);

	jpeg_stdio_src(&cinfo, fp);

	// ヘッダ読み込み
	jpeg_read_header(&cinfo, TRUE);
	if (!cinfo.saw_JFIF_marker)
	{
		ODS("CDirectDraw::LoadJPEGbits : JFIFマーカーがない。\n");
		jpeg_destroy_decompress(&cinfo);
		fclose(fp);
		return FALSE;
	}

	// 展開開始
	jpeg_start_decompress(&cinfo);

	// 情報とか所得
	*width = cinfo.image_width;
	*height = cinfo.image_height;

	if (cinfo.out_color_components != 3)
	{
		ODS("CDirectDraw::LoadJPEGbits : 対応してない形式が使われている。\n");
		jpeg_destroy_decompress(&cinfo);
		fclose(fp);
		return FALSE;
	}

	//ビットマップの大きさにあわせて新たにメモリ領域を作成する
	JSAMPROW Image;			// 一行だけ
	LPVOID pnewbits2;
	DWORD ishalf=1;
	if(g_config.IsHalfMode())ishalf=2;
	pnewbits2 = malloc(sizeof(MYPALLET)* *width * *height / ishalf);
	MYPALLET* retbit = (MYPALLET*)pnewbits2;

	DWORD halfcopy=0;

	// ２次元動的配列
	Image = (JSAMPROW)malloc(3 * *width);

	//そこにデータをコピー
	for(i=0;i<(LONG)*height;i++){
		jpeg_read_scanlines(&cinfo, &Image, 1);		// 何故かこんな半端なところで読み込み
		for(j=0;j<(LONG)*width;j++){
			if(g_config.IsHalfMode()){//半分しかコピーしない
				if(i%2==0 && j%2==0 && j != *width - 1){
					retbit[halfcopy].red = Image[j*3];
					retbit[halfcopy].green = Image[j*3+1];
					retbit[halfcopy].blue = Image[j*3+2];
					retbit[halfcopy].alpha = 0xFF;
					halfcopy++;
				}
			}
			else{
				int str = i * *width + j;
				retbit[str].red = Image[j*3];
				retbit[str].green = Image[j*3+1];
				retbit[str].blue = Image[j*3+2];
				retbit[str].alpha = 0xFF;
			}
		}
	}

	// 完了
	free(Image);

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(fp);

	if(g_config.IsHalfMode()){
		*width/=2;
		*height/=2;
	}

	*pbits = retbit;

	return TRUE;
}
#endif // GCD_EDITER

/*!
*	@brief 指定ファイル名のビットマップから、パレットだけを読み込み
*
*	CreateSurfaceFrom256BMPの内部で、Load256Bitmapbitsから利用される。
*	α値の設定はビットマップには本来ないものなので、
*	RGBの成分が全て0のものと255番目のパレットは α=0
*	それ以外のパレットは α=255 に設定されて返されます。
*
*	@param filename [in] ファイル名
*	@param pal [out] 読み込み結果を保存するためのパレット配列（256個必要）
*	@return TRUE:成功, FALSE:残念
*/
BOOL CDirectDraw::GetPallet(char *filename,MYPALLET *pal)
{
	HANDLE hFile;
	DWORD ret,br,err=FALSE;

	if(filename==NULL){
		ODS("CDirectDraw::GetPallet : ファイル名がNULLとはどういうことだ？\n");
		return(FALSE);
	}
	if(pal==NULL){
		ODS("CDirectDraw::GetPallet : パレットがNULLとはどういうことだ？\n");
		return(FALSE);
	}

	//ファイルのオープン
	hFile=CreateFile(filename,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		return(NULL);
	}
	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//念のためファイルの先頭に移動

	//ﾌｧｲﾙヘッダの読み出し
	BITMAPFILEHEADER fileheader;
	ret=ReadFile(hFile,&fileheader,sizeof(BITMAPFILEHEADER),&br,NULL);
	if(!ret || br!=sizeof(BITMAPFILEHEADER)){
		ODS("CDirectDraw::GetPallet : ファイルの読み込みに失敗(1)\n");
		err=TRUE;
	}
	if(fileheader.bfType != 0x4d42){//"BM"
		ODS("CDirectDraw::GetPallet : つーかこのファイルはビットマップではない\n");
		err=TRUE;
	}
	if(err){
		CloseHandle(hFile);
		return(FALSE);
	}

	//BITMAPINFOHEADERの読み込み
	BITMAPINFOHEADER infohed;
	ret=ReadFile(hFile,&infohed,sizeof(BITMAPINFOHEADER),&br,NULL);
	if(!ret || br!=sizeof(BITMAPINFOHEADER)){
		ODS("CDirectDraw::GetPallet : ファイルの読み込みに失敗(2)\n");
		err=TRUE;
	}
	if(infohed.biSize != sizeof(BITMAPINFOHEADER)){
		ODS("CDirectDraw::GetPallet : BITMAPINFOHEADERのサイズが合わない\n");
		err=TRUE;
	}
	if(infohed.biBitCount != 8){
		ODS("CDirectDraw::GetPallet : つーかこのファイルは256ではない\n");
		err=TRUE;
	}
	if(infohed.biCompression != BI_RGB){
		ODS("CDirectDraw::GetPallet : 圧縮がかかっているらしい\n");
		err=TRUE;
	}
	if(infohed.biClrUsed == 0){
		infohed.biClrUsed=256;
	}
	if(err){
		CloseHandle(hFile);
		return(FALSE);
	}

	//いよいよパレットの読み込み
	RGBQUAD dpal[256];
	ZeroMemory(dpal,sizeof(RGBQUAD)*256);
	ret=ReadFile(hFile,dpal,sizeof(RGBQUAD)*infohed.biClrUsed,&br,NULL);
	CloseHandle(hFile);
	if(!ret || br!=sizeof(RGBQUAD)*infohed.biClrUsed){
		ODS("CDirectDraw::GetPallet : ファイルの読み込みに失敗(3)\n");
		return(FALSE);
	}
	//255番目の色は強制的に黒（透過色）
	dpal[255].rgbBlue  =0;
	dpal[255].rgbGreen =0;
	dpal[255].rgbRed   =0;

	for(int i=0;i<256;i++){
		if(dpal[i].rgbRed==0 && dpal[i].rgbGreen==0 && dpal[i].rgbBlue==0)pal[i].alpha = 0;//透明
		else pal[i].alpha = 0xFF;//不透明
		pal[i].red = dpal[i].rgbRed;
		pal[i].green = dpal[i].rgbGreen;
		pal[i].blue = dpal[i].rgbBlue;
	}

	return(TRUE);
}

/*!
*	@brief 指定ファイル名のPNGファイルから、パレットだけを読み込み
*
*	GetPalletのPNG版。
*	BMPと違い、α値はファイルからロードします。
*
*	@param filename [in] ファイル名
*	@param pal [out] 読み込み結果を保存するためのパレット配列（256個必要）
*	@return TRUE:成功, FALSE:残念
*	@sa GetPallet 
*/
BOOL CDirectDraw::GetPalletPNG(char *filename,MYPALLET *pal)
{
	FILE* fp;

	if(filename==NULL){
		ODS("CDirectDraw::GetPalletPNG : ファイル名がNULLとはどういうことだ？\n");
		return(FALSE);
	}
	if(pal==NULL){
		ODS("CDirectDraw::GetPalletPNG : パレットがNULLとはどういうことだ？\n");
		return(FALSE);
	}

	// PNG構造体？
	png_structp strPNG = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!strPNG)
	{
		ODS("CDirectDraw::GetPalletPNG : PNG構造体ﾃﾞｷﾃﾈｴﾖ!!ヽ(`Д´)ﾉｳﾜｧｧﾝ!!\n");
		return FALSE;
	}

	// 情報構造体…って何じゃ。
	png_infop infoPNG = png_create_info_struct(strPNG);
	if (!infoPNG)
	{
		ODS("CDirectDraw::GetPalletPNG : PNG情報構造体ﾃﾞｷﾃﾈｴﾖ!!ヽ(`Д´)ﾉｳﾜｧｧﾝ!!\n");
		png_destroy_read_struct(&strPNG, NULL, NULL);
		return FALSE;
	}
	
	// 読み込むか。
	fp = fopen(filename, "rb");
	if (!fp)
	{
		png_destroy_read_struct(&strPNG, &infoPNG, NULL);
		return FALSE;
	}

	// ライブラリに任せる
	png_init_io(strPNG, fp);

	// シグネチャ解析
	BYTE sig[8];
	long pos = ftell(fp);

	int len = fread(sig, sizeof(BYTE), 8, fp);
	if ( png_sig_cmp(sig, 0, len) != 0 )
	{
		//ODS("CDirectDraw::GetPalletPNG : つーかこのファイルはPNGではない。\n");
		fclose(fp);
		png_destroy_read_struct(&strPNG, &infoPNG, NULL);
		return FALSE;
	}

	// 先頭に戻す
	fseek(fp, pos, SEEK_SET);

	// PNG情報ゲッツ
	png_read_info(strPNG, infoPNG);

	// アンド ターン
	int bit_depth = 0, color_type = 0;

	bit_depth = png_get_bit_depth(strPNG, infoPNG);
	color_type = png_get_color_type(strPNG, infoPNG);

	// パレット付きの256色じゃないとだめぽ
	if (color_type != 3)
	{
		ODS("CDirectDraw::GetPalletPNG : パレットがないよ。\n");
		fclose(fp);
		png_destroy_read_struct(&strPNG, &infoPNG, NULL);
		return FALSE;
	}
	if (bit_depth != 8)
	{
		ODS("CDirectDraw::GetPalletPNG : つーかこのファイルは256ではない。\n");
		fclose(fp);
		png_destroy_read_struct(&strPNG, &infoPNG, NULL);
		return FALSE;
	}

	// いよいよパレット所得
	png_colorp Palette;
	int num_palette = 0;

	png_get_PLTE(strPNG, infoPNG, &Palette, &num_palette);

	// α値所得
	png_bytep trans;
	int num_trans = 0;

	png_get_tRNS(strPNG, infoPNG, &trans, &num_trans, NULL);

	// 設定ｶｲｰｼ
	for (int i = 0; i < 256; i++)
	{
		if (i < num_trans)
			pal[i].alpha = trans[i];//透明度設定
		else
			pal[i].alpha = 0xFF;//設定がないので不透明

		if (i < num_palette)	// 色設定
		{
			pal[i].red = Palette[i].red;
			pal[i].green = Palette[i].green;
			pal[i].blue = Palette[i].blue;
		}
		else
			pal[i].red = pal[i].green = pal[i].blue = 0;	// 無い
	}

	// 終了処理
	fclose(fp);
	png_destroy_read_struct(&strPNG, &infoPNG, NULL);

	return TRUE;
}



/*!
*	ロードされた独自形式2Dサーフェイスを開放
*
*	@param s 開放するサーフェイス
*/
void CDirectDraw::RelSurface(MYSURFACE *s)
{
	if(s==NULL)return;

	if(s < &ms[0] || s > &ms[MAXNUMGOLUAHTEXTURES-1])
	{
		#ifndef GCD_EDITER
		g_system.Log("CDirectDraw::RelSurface 確保してないサーフェイスの開放？",SYSLOG_WARNING);
		#endif
		return;
	}

	UINT j,k;

	if(s->valid){
		s->valid=FALSE;
		for(j=0;j<s->ysufnum;j++){
			for(k=0;k<s->xsufnum;k++){
				RELEASE(s->pTex[k*(s->ysufnum) + j]);
			}
		}
	}
	free(s->pTex);
	s->pTex = NULL;
}






/*==================================================================================
	

	
	2D描画・マトリクス関連



====================================================================================*/


/*----------------------------------------------------------------------------------
	独自形式2Dサーフェイス描画
------------------------------------------------------------------------------------*/

/*!
*	@brief CheckBlt2 引数省略簡易版
*
*	@param dds	転送元サーフェイス
*	@param x	転送先X座標
*	@param y	転送先Y座標
*	@param r	転送元矩形
*	@param revx X反転
*	@param revy Y反転
*	@param flag その他動作フラグ
*	@param z	転送先Z座標
*	@param color 32ビット形式頂点カラー。通常 0xFFFFFFFF
*
*	@sa CheckBlt2
*/
void CDirectDraw::CheckBlt(MYSURFACE *dds,int x,int y,RECT r,BOOL revx,BOOL revy,DWORD flag,float z,DWORD color, BOOL drawShadow)
{
	CheckBlt2(dds,x,y,r,1.0,1.0,revx,revy,flag,z,color,drawShadow);
}

/*!
*	@brief サーフェースの矩形転送(描画)
*
*	転送先は通常、常にバックバッファです。（描画ターゲットによる）
*	DirectDrawの頃のBlt関数をそれなりに踏襲して、だんだん引数が増えたり減ったりして今に至る。
*	なんかそんな感じの由緒正しい関数です。よろしくねっ！
*
*	@param dds	転送元サーフェイス
*	@param x	転送先X座標
*	@param y	転送先Y座標
*	@param r	転送元矩形
*	@param magx X拡大率
*	@param magy Y拡大率
*	@param revx X反転
*	@param revy Y反転
*	@param flag その他動作フラグ
*	@param z	転送先Z座標
*	@param color 32ビット形式頂点カラー。通常 0xFFFFFFFF
*/
void CDirectDraw::CheckBlt2(MYSURFACE *dds,int x,int y,RECT r,
			   double magx,double magy,BOOL revx,BOOL revy,DWORD flag,float z,DWORD color, BOOL drawShadow)
{
	DWORD i,j;
	float cut_left,cut_right,cut_top,cut_bottom;//切れてる長さ(ピクセル単位)
	float transx,transy;//一時的に使用

	//if(dds==NULL){return;}
	if(dds < &ms[0] || dds > &ms[MAXNUMGOLUAHTEXTURES-1])return;

	#ifdef AKIDX_DEBUG
	if(magx==0 || magy==0){
		ODS("☆CDirectDraw::CheckBlt2() Warning-magx or magy ==0\n");
	}
	#endif
	//矩形を正しく設定
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

	if(g_config.IsHalfMode()){
		r.left/=2;
		r.right/=2;
		r.top/=2;
		r.bottom/=2;
	//	x/=2;
	//	y/=2;
		magx*=2;
		magy*=2;
	}

	DWORD r_top,r_bottom,r_left,r_right;//warningがウザイ
	r_top=r.top;
	r_bottom=r.bottom;
	r_left=r.left;
	r_right=r.right;

	/*if (devtypenow != D3DDEVTYPE_HAL)
	{
		// 軽量化ver（かなり適当、つーか消えた。）
		MyBlt3DLite(dds, r, x, y, FALSE);
	}
	else*/
	{
		//全てのテクスチャに関して描画するかどうか調べて描画する
		float vl,vr,vt,vb;//各頂点の座標
		float tumin,tumax,tvmin,tvmax;//u,v座標の範囲
		float ar = 640.0f/480.0f;//アスペクト比
		float ar2 = 2.0f/480.0f;
		float centerx = (float)640.0f/2.0f;//x方向画面中心
		MYVERTEX3D* vrtxarr;//頂点配列
		D3DXMATRIX matw;//ワールド座標変換行列
		D3DXMATRIX tmpmat;//テンポラリ行列
		D3DXMATRIX matpres;//プリセット変換行列

		// プリセット準備
		D3DXMatrixIdentity(&matpres);
		//反転処理
		if(revx){
			d3dxplane_x.d=((float)r_right-(float)r_left)*ar2/2.0f;
			D3DXMatrixReflect(&tmpmat,&d3dxplane_x);
			matpres *= tmpmat;
		}
		if(revy){
			d3dxplane_y.d=((float)r_bottom-(float)r_top)*ar2/2.0f;
			D3DXMatrixReflect(&tmpmat,&d3dxplane_y);
			matpres *= tmpmat;
		}
		//拡大縮小
		D3DXMatrixScaling(&tmpmat,(float)magx,(float)magy,1.0f);
		matpres *= tmpmat;
		//移動(座標の単位はfloat変換後のもの）
		D3DXMatrixTranslation(&tmpmat,(float)x/HALF_HEIGHT,(float)y/HALF_HEIGHT,/*z*/0);
		matpres *= tmpmat;
		if(flag & CKBLT_YUREY){//揺れ
			D3DXMatrixTranslation(&tmpmat,0,yurey*ar2,0);
			matpres *= tmpmat;
		}
		//設定された親の変換と合わせる
		matpres *= matparent;

		// pSprite->Begin();
		for(j=0;j<dds->ysufnum;j++){
			for(i=0;i<dds->xsufnum;i++){
				if(dds->pTex[j*dds->xsufnum+i] ==NULL )continue;
				else if(dds->xsufindx[i]+dds->xsufsize[i] < r_left)continue;
				else if(dds->xsufindx[i] > r_right)continue;
				else if(dds->ysufindx[j]+dds->ysufsize[j] < r_top)continue;
				else if(dds->ysufindx[j] > r_bottom)continue;
				else{
					//(0) 左右上下、それぞれ転送しない分を計算
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

					//(1) u,v座標の計算
					tumin = cut_left / (float)dds->xsufsize[i];
					tumax = 1.0f - cut_right/(float)dds->xsufsize[i];
					tvmin = cut_top / (float)dds->ysufsize[j];
					tvmax = 1.0f - cut_bottom/(float)dds->ysufsize[j];

					//(2) 転送座標の計算(以降メモ参照)
					//幅と高さをだす（ピクセル→float変換済座標系）
					//幅
					vl=0;
					vr=dds->xsufsize[i] - (cut_left+cut_right);
					vr= vr*ar2;// - ar;
					//高さ
					vt=0;
					vb=dds->ysufsize[j] - (cut_top+cut_bottom);
					vb=vb*ar2;//-1.0f
					//(3) 頂点配列に座標値を代入
					if ( !pMyVertex || FAILED(pMyVertex->Lock(0, 0, (BYTE**)&vrtxarr, D3DLOCK_DISCARD)) )
						return;

					//左上
					vrtxarr[0].x = vl;
					vrtxarr[0].y = vt;
					vrtxarr[0].z = z;
					//左下
					vrtxarr[1].x = vl;
					vrtxarr[1].y = vb;
					vrtxarr[1].z = z;
					//右上
					vrtxarr[2].x = vr;
					vrtxarr[2].y = vt;
					vrtxarr[2].z = z;
					//右下
					vrtxarr[3].x = vr;
					vrtxarr[3].y = vb;
					vrtxarr[3].z = z;

					//頂点色
					vrtxarr[0].color = color;
					vrtxarr[1].color = color;
					vrtxarr[2].color = color;
					vrtxarr[3].color = color;

					//(3.5) テクスチャ座標設定
					vrtxarr[0].tu = tumin;
					vrtxarr[0].tv = tvmin;
					vrtxarr[1].tu = tumin;
					vrtxarr[1].tv = tvmax;
					vrtxarr[2].tu = tumax;
					vrtxarr[2].tv = tvmin;
					vrtxarr[3].tu = tumax;
					vrtxarr[3].tv = tvmax;

					if (pMyVertex) pMyVertex->Unlock();

					//(4) ワールド座標変換行列用意
					//単位行列
					D3DXMatrixIdentity(&matw);
					//矩形内変換(座標の単位はfloat変換後のもの)
					if(r_left>dds->xsufindx[i])transx=0;
					else transx = (float)( dds->xsufindx[i] - r_left );
					if(r_top>dds->ysufindx[j])transy=0;
					else transy = (float)( dds->ysufindx[j] - r_top );
					D3DXMatrixTranslation(&tmpmat,transx*ar2,transy*ar2,0);
					matw *= tmpmat;
					//プリセットのやつをかける
					matw *= matpres;

					/*RECT r2;

					r2.left = (LONG)cut_left;
					r2.top = (LONG)cut_top;
					r2.right = dds->xsufsize[i] - (LONG)cut_right;
					r2.bottom = dds->ysufsize[j] - (LONG)cut_bottom;*/

					// pSprite->DrawTransform(dds->pTex[j*dds->xsufnum+i], &r2, &matw, color);
					d3ddev->SetTransform(D3DTS_WORLD,&matw);//*設定*
					//(5) 描画
					d3ddev->SetTexture(0,dds->pTex[j*dds->xsufnum+i]);//テクスチャ設定
					d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSU,D3DTADDRESS_CLAMP);
					d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSV,D3DTADDRESS_CLAMP);
					d3ddev->SetStreamSource(0, pMyVertex, sizeof(MYVERTEX3D));
					d3ddev->SetVertexShader( FVF_3DVERTEX );//頂点のフォーマットを指定
					d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);//描画
				}
			}
		}
		// pSprite->End();
	}
}


/*!
*	@brief 独自形式2Dサーフェイス描画　２
*
*	転送位置を3D空間上で指定することができる。転送先は通常、常にバックバッファ（描画ターゲット）です。
*	CheckBlt2よりも引数が少ないけど、MYRECT3Dにいろいろあってこっちのほうが自由度が高い。多分。
*
*	@param dds 転送元サーフェイス
*	@param src 転送元矩形
*	@param dst 転送先矩形
*	@param flag その他動作フラグ
*	@param color 32ビット形式頂点カラー。通常 0xFFFFFFFF
*/
void CDirectDraw::MyBlt3D(MYSURFACE *dds,RECT src,MYRECT3D dst,DWORD flag,DWORD color)
{
	float cut_left,cut_right,cut_top,cut_bottom;//切れてる長さ(ピクセル単位)
	float transx,transy,sclx,scly;//一時的に使用

	if(dds==NULL){return;}

	//矩形を正しく設定
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

	if(g_config.IsHalfMode()){
		src.left/=2;
		src.right/=2;
		src.top/=2;
		src.bottom/=2;
	}

	DWORD i,j;	

	DWORD r_top,r_bottom,r_left,r_right;//warningがウザイ
	r_top=src.top;
	r_bottom=src.bottom;
	r_left=src.left;
	r_right=src.right;

	/*if (devtypenow != D3DDEVTYPE_HAL)
	{
		// 軽量化ver（かなり適当、つーか消えた。）
		MyBlt3DLite(dds, src, (int)(dst.left * HALF_HEIGHT), (int)(dst.top * HALF_HEIGHT), TRUE);
	}
	else*/
	{
		//全てのテクスチャに関して描画するかどうか調べて描画する
		float vl,vr,vt,vb;//各頂点の座標
		float tumin,tumax,tvmin,tvmax;//u,v座標の範囲
		float ar = 640.0f/480.0f;//アスペクト比
		float ar2 = 2.0f/480.0f;
		float centerx = 640.0f/2.0f;//x方向画面中心
		MYVERTEX3D* vrtxarr;//頂点配列
		D3DXMATRIX matw;//ワールド座標変換行列
		D3DXMATRIX tmpmat;//テンポラリ行列
		D3DXMATRIX matpres;//プリセット変換行列

		// プリセット準備
		D3DXMatrixIdentity(&matpres);

		//(0,0)-(1,1)の範囲に入るように縮小
		sclx = 1.0f / (float)(r_right - r_left);
		scly = 1.0f / (float)(r_bottom - r_top);
		D3DXMatrixScaling(&tmpmat,sclx,scly,1.0f);
		matpres *= tmpmat;
		
		//指定されたMYRECT3Dまで拡大&移動
		sclx=dst.right/* * HALF_HEIGHT*/-dst.left/* * HALF_HEIGHT*/;
		scly=dst.bottom/* * HALF_HEIGHT*/-dst.top/* * HALF_HEIGHT*/;
		D3DXMatrixScaling(&tmpmat,sclx,scly,1.0f);
		matpres *= tmpmat;
		
		//移動
		D3DXMatrixTranslation(&tmpmat,dst.left/* * HALF_HEIGHT*/,dst.top/* * HALF_HEIGHT*/,/*dst.z*/0);
		matpres *= tmpmat;
		if(flag & CKBLT_YUREY){//揺れ
			D3DXMatrixTranslation(&tmpmat,0,yurey*ar2,0);
			matpres *= tmpmat;
		}
		//設定された親の変換と合わせる
		matpres *= matparent;

		// pSprite->Begin();
		for(j=0;j<dds->ysufnum;j++){
			for(i=0;i<dds->xsufnum;i++){
				if(dds->pTex[j*dds->xsufnum+i] ==NULL );
				else if(dds->xsufindx[i]+dds->xsufsize[i] < r_left);
				else if(dds->xsufindx[i] > r_right);
				else if(dds->ysufindx[j]+dds->ysufsize[j] < r_top);
				else if(dds->ysufindx[j] > r_bottom);
				else{
					//(0) 左右上下、それぞれ転送しない分を計算(src矩形上)
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

					//(1) u,v座標の計算
					tumin = cut_left / (float)dds->xsufsize[i];
					tumax = 1.0f - cut_right/(float)dds->xsufsize[i];
					tvmin = cut_top / (float)dds->ysufsize[j];
					tvmax = 1.0f - cut_bottom/(float)dds->ysufsize[j];

					//(2) 転送座標の計算
					//幅と高さをだす（ピクセル→float変換済座標系）
					//幅
					vl=0;
					vr=dds->xsufsize[i] - (cut_left+cut_right);
					//高さ
					vt=0;
					vb=dds->ysufsize[j] - (cut_top+cut_bottom);
					//(3) 頂点配列に座標値を代入
					if ( !pMyVertex || FAILED(pMyVertex->Lock(0, 0, (BYTE**)&vrtxarr, D3DLOCK_DISCARD)) )
						return;

					//左上
					vrtxarr[0].x = vl;
					vrtxarr[0].y = vt;
					vrtxarr[0].z = dst.z;
					//左下
					vrtxarr[1].x = vl;
					vrtxarr[1].y = vb;
					vrtxarr[1].z = dst.z;
					//右上
					vrtxarr[2].x = vr;
					vrtxarr[2].y = vt;
					vrtxarr[2].z = dst.z;
					//右下
					vrtxarr[3].x = vr;
					vrtxarr[3].y = vb;
					vrtxarr[3].z = dst.z;

					//頂点色
					vrtxarr[0].color = color;
					vrtxarr[1].color = color;
					vrtxarr[2].color = color;
					vrtxarr[3].color = color;

					//(3.5) テクスチャ座標設定
					vrtxarr[0].tu = tumin;
					vrtxarr[0].tv = tvmin;
					vrtxarr[1].tu = tumin;
					vrtxarr[1].tv = tvmax;
					vrtxarr[2].tu = tumax;
					vrtxarr[2].tv = tvmin;
					vrtxarr[3].tu = tumax;
					vrtxarr[3].tv = tvmax;

					if (pMyVertex) pMyVertex->Unlock();

					//(4) ワールド座標変換行列用意
					//単位行列
					D3DXMatrixIdentity(&matw);

					//矩形内変換
					if(r_left>dds->xsufindx[i])transx=0;
					else transx = (float)( dds->xsufindx[i] - r_left );//pixel単位
					if(r_top>dds->ysufindx[j])transy=0;
					else transy = (float)( dds->ysufindx[j] - r_top );//pixel単位
					D3DXMatrixTranslation(&tmpmat,transx,transy,0);
					matw *= tmpmat;

					//プリセットのやつをかける
					matw *= matpres;

					/*RECT r2;

					r2.left = (LONG)cut_left;
					r2.top = (LONG)cut_top;
					r2.right = dds->xsufsize[i] - (LONG)cut_right;
					r2.bottom = dds->ysufsize[j] - (LONG)cut_bottom;*/

					// pSprite->DrawTransform(dds->pTex[j*dds->xsufnum+i], &r2, &matw, color);
					d3ddev->SetTransform(D3DTS_WORLD,&matw);//*設定*
					//(5) 描画
					d3ddev->SetTexture(0,dds->pTex[j*dds->xsufnum+i]);//テクスチャ設定
					d3ddev->SetStreamSource(0, pMyVertex, sizeof(MYVERTEX3D));
					d3ddev->SetVertexShader( FVF_3DVERTEX );//頂点のフォーマットを指定
					d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSU,D3DTADDRESS_CLAMP);
					d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSV,D3DTADDRESS_CLAMP);
					d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);//描画
				}
			}
		}
		// pSprite->End();
	}
}

/*!
*	@brief Goluah GCD(バージョン自動判別)形式セル描画
*
*	GCDのうち、指定インデックスのCELLを描画します。
*/
void CDirectDraw::CellDraw(MYSURFACE **pbuf,//!< GCDで利用するビットマップ配列
						   LPVOID pcdat,	//!< GCD,CELL (GCD_CELL2)配列
						   LPVOID prdat,	//!< GCD,切り取り矩形 (GCD_RECT)配列
						   DWORD cn,		//!< 描画するセル番号
						   int x,			//!< 描画先基準x
						   int y,			//!< 描画先基準y
						   float z,			//!< 描画先z
						   int rot,			//!< 全体回転角
						   BOOL revx,		//!< 全体反転X
						   BOOL revy,		//!< 全体反転Y
						   DWORD color,		//!< 全体色
						   float magx,		//!< 全体拡大率X
						   float magy,		//!< 全体拡大率Y
						   BOOL shadowed)	//!<影付き
{
	if(!pcdat || cn==0)return;
	GCD_CELL2_070* pc = (GCD_CELL2_070*)pcdat;

	switch(pc->cell[0].flag){
		case 900:
			CellDraw090(pbuf,
						pcdat,
						prdat,
						cn,
						x,
						y,
						z,
						rot,
						revx,
						revy,
						color,
						magx,
						magy,
						shadowed);break;
		case 700:
			CellDraw070(pbuf,
						pcdat,
						prdat,
						cn,
						x,
						y,
						z,
						rot,
						revx,
						revy,
						color,
						magx,
						magy,
						shadowed);break;
		#ifdef AKIDX_DEBUG
		default:
			{
				char tmp[64];
				sprintf(tmp,"DX_Draw : warning , unknown GCD version %X\n",pc->cell[0].flag);
				ODS(tmp);
			}
		#endif
	}
}

/*!
*	@brief Goluah GCD(090)形式セル描画
*
*	CellDraw070とCellDraw090は、内部での座標変換の順番が微妙に入れ替わっているので注意
*/
void CDirectDraw::CellDraw090(MYSURFACE **pbuf,//!< GCDで利用するビットマップ配列
						   LPVOID pcdat,	//!< GCD,CELL (GCD_CELL2_090)配列
						   LPVOID prdat,	//!< GCD,切り取り矩形 (GCD_RECT_090)配列
						   DWORD cn,		//!< 描画するセル番号
						   int x,			//!< 描画先基準x
						   int y,			//!< 描画先基準y
						   float z,			//!< 描画先z
						   int rot,			//!< 全体回転角
						   BOOL revx,		//!< 全体反転X
						   BOOL revy,		//!< 全体反転Y
						   DWORD color,		//!< 全体色
						   float magx,		//!< 全体拡大率X
						   float magy,		//!< 全体拡大率Y
						   BOOL shadowed)	//!<影付き
{
	if(pbuf==NULL || pcdat==NULL || prdat==NULL)return;
	if(cn >= GCDMAX_CELLS)return;

	#ifdef AKIDX_DEBUG
	if(magx==0 || magy==0){
		ODS("☆CDirectDraw::CheckBlt2() Warning-magx or magy ==0\n");
	}
	#endif

	GCD_CELL2_090 *cdat=(GCD_CELL2_090*)pcdat;
	GCD_RECT_090  *rdat=(GCD_RECT_090*)prdat;

	/*if (g_config.IsHalfMode())
	{
		x /= 2;
		y /= 2;
		cdat[cn].gcx /= 2;
		cdat[cn].gcy /= 2;
		for(int i=0;i<8;i++){
			cdat[cn].cell[i].dx /= 2;
			cdat[cn].cell[i].dy /= 2;
		}
	}*/

	if (devtypenow != D3DDEVTYPE_HAL)
	{
		for(int i=0;i<8;i++){
			DWORD rn;

			rn = cdat[cn].cell[i].cdr;
			CheckBlt(pbuf[rdat[rn].bmpno],
				x + cdat[cn].cell[i].dx,y + cdat[cn].cell[i].dy,rdat[rn].r,
				revx,
				revy,
				0,z,color);//描画
		}
	}
	else
	{
		D3DXMATRIX matp,mat,tmt,matprv,matprv2;
		float ar2 = 2.0f/480.0f;;

		//ZW/ZTフラグ操作
		if((cdat[cn].flag & GCDCELL2_DISABLE_ZT) || (cdat[cn].flag & GCDCELL2_DISABLE_ZW))
		{
			BOOL zt = (cdat[cn].flag & GCDCELL2_DISABLE_ZT);
			BOOL zw = (cdat[cn].flag & GCDCELL2_DISABLE_ZW);
			EnableZ(zt,zw);
		}

		//キャラクターの変換行列
		D3DXMatrixIdentity(&matp);
		if(!(cdat[cn].flag & GCDCELL2_SCA_GCENTER))
		{	
			if(cdat[cn].flag & GCDCELL2_ROT_BASEPOINT)
			{
				//スケール：足元中心 回転：足元中心
				D3DXMatrixScaling(&tmt,magx,magy,1.0f);//拡大
				matp *= tmt;
				D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//回転
				matp *= tmt;
				D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2*(-1.0f),(float)(cdat[cn].gcy)*ar2*(-1.0f),0);//重心に移動
				matp *= tmt;
			}
			else{
				//スケール：足元中心 回転：重心中心
				D3DXMatrixScaling(&tmt,magx,magy,1.0f);//拡大
				matp *= tmt;
				D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2*(-1.0f),(float)(cdat[cn].gcy)*ar2*(-1.0f),0);//重心に移動
				matp *= tmt;
				D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//回転
				matp *= tmt;
			}
		}
		else
		{
			if(cdat[cn].flag & GCDCELL2_ROT_BASEPOINT)
			{
				//スケール：重心中心 回転：足元中心
				D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//回転
				matp *= tmt;
				D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2*(-1.0f),(float)(cdat[cn].gcy)*ar2*(-1.0f),0);//重心に移動
				matp *= tmt;
				D3DXMatrixScaling(&tmt,magx,magy,1.0f);//拡大
				matp *= tmt;
			}
			else{
				//スケール：重心中心 回転：重心中心
				D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2*(-1.0f),(float)(cdat[cn].gcy)*ar2*(-1.0f),0);//重心に移動
				matp *= tmt;
				D3DXMatrixScaling(&tmt,magx,magy,1.0f);//拡大
				matp *= tmt;
				D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//回転
				matp *= tmt;
			}
		}
		if(revy){
			d3dxplane_y.d=0;
			D3DXMatrixReflect(&tmt,&d3dxplane_y);//y反転
			matp *= tmt;
		}
		D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2,(float)(cdat[cn].gcy)*ar2,0);//重心に戻す
		matp *= tmt;
		if(revx){
			d3dxplane_x.d=0;
			D3DXMatrixReflect(&tmt,&d3dxplane_x);//x反転
			matp *= tmt;
		}
		D3DXMatrixTranslation(&tmt,(float)x*ar2,(float)y*ar2,0);//表示位置への移動
		matp *= tmt;

		matprv = SetParentMatrix(matp,FALSE,TRUE);//「親」の変換行列として設定

		DWORD rn;
		BOOL c1revx,c1revy;

		for(int i=0;i<8;i++){
			rn = cdat[cn].cell[i].cdr;
			if(rn < GCDMAX_RECTANGLES && rn!=0 && rdat[rn].bmpno<GCDMAX_IMAGES){
				//変換行列を計算
				D3DXMatrixIdentity(&mat);
				D3DXMatrixTranslation(&tmt,(float)(rdat[rn].center_x)*ar2*(-1.0f),(float)(rdat[rn].center_y)*ar2*(-1.0f),0);//重心に移動
				mat *= tmt;
				D3DXMatrixScaling(&tmt,cdat[cn].cell[i].magx,cdat[cn].cell[i].magy,1.0f);//拡大
				mat *= tmt;
				D3DXMatrixRotationZ(&tmt,D3DXToRadian(cdat[cn].cell[i].rot));//回転
				mat *= tmt;
				D3DXMatrixTranslation(&tmt,(float)(rdat[rn].center_x)*ar2,(float)(rdat[rn].center_y)*ar2,0);//重心に戻す
				mat *= tmt;
				D3DXMatrixTranslation(&tmt,(float)(cdat[cn].cell[i].dx)*ar2,(float)(cdat[cn].cell[i].dy)*ar2,0);//表示位置への移動
				mat *= tmt;

				matprv2 = SetParentMatrix(mat,FALSE,TRUE);

				c1revx = cdat[cn].cell[i].flag & GCDCELL_REVERSE_X;
				c1revy = cdat[cn].cell[i].flag & GCDCELL_REVERSE_Y;

				//色を変更(090特有)
				DWORD rcolor_a = (color&0xFF000000)>>24;
				DWORD rcolor_r = (color&0x00FF0000)>>16;
				DWORD rcolor_g = (color&0x0000FF00)>>8;
				DWORD rcolor_b = (color&0x000000FF);
				rcolor_a = (DWORD)(rcolor_a*cdat[cn].cell[i].alpha_ratio);
				rcolor_r = (DWORD)(rcolor_r*cdat[cn].cell[i].red_ratio);
				rcolor_g = (DWORD)(rcolor_g*cdat[cn].cell[i].green_ratio);
				rcolor_b = (DWORD)(rcolor_b*cdat[cn].cell[i].blue_ratio);
				DWORD rcolor = (rcolor_a<<24) | (rcolor_r<<16) | (rcolor_g<<8) | rcolor_b;

				//ブレンディングモード変更(090特有)
				if(cdat[cn].cell[i].flag & GCDCELL_BLEND_ADD){
					SetAlphaMode(GBLEND_KASAN);
				}

			/*	//テクスチャフィルタリングを決定
				if( cdat[cn].cell[i].magx != 1.0f ||
					cdat[cn].cell[i].magy != 1.0f || 
					(cdat[cn].cell[i].rot + rot)%90 != 0)
				{
					d3ddev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
					d3ddev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
				}
				else
				{
					d3ddev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
					d3ddev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
				}*/

				CheckBlt(pbuf[rdat[rn].bmpno],
					0,0,rdat[rn].r,
					c1revx,
					c1revy,
					0,z,rcolor);//描画

				//ブレンディングモードレストア
				if(cdat[cn].cell[i].flag & GCDCELL_BLEND_ADD){
					SetAlphaMode(0);
				}

				// ついでだから影も描くぜ
				if (shadowed)
				{
					D3DXPLANE Plane = D3DXPLANE(0, -1, 0, -0.01);
					D3DXMATRIX matp;

					if (stencil_enable)
					{
						d3ddev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILCAPS_INCRSAT);	// ステンシルに書き込む
						d3ddev->SetRenderState( D3DRS_COLORWRITEENABLE,  FALSE );
						d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
					}
					D3DXMatrixShadow(&mat,
						&D3DXVECTOR4(0,-100,100,1),
						&Plane);
					matp = SetParentMatrix(mat, FALSE);
					CheckBlt(pbuf[rdat[rn].bmpno],
						0,0,rdat[rn].r,
						c1revx,
						c1revy,
						0,z,0xAA000000);//影描画

					d3ddev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILCAPS_KEEP);	// 元に
					d3ddev->SetRenderState( D3DRS_COLORWRITEENABLE,  0xFF );
					d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
				}

				SetParentMatrix(matprv2,TRUE);
			}
		}

		SetParentMatrix(matprv,TRUE);//「親」の変換行列を元に戻す
		EnableZ();

	/*	//テクスチャフィルタを元(?)に戻す
		d3ddev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
		d3ddev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );*/
	}

	/*if (g_config.IsHalfMode())
	{
		cdat[cn].gcx *= 2;
		cdat[cn].gcy *= 2;
		for(int i=0;i<8;i++){
			cdat[cn].cell[i].dx *= 2;
			cdat[cn].cell[i].dy *= 2;
		}
	}*/

	if(cdat[cn].flag & GCDCELL2_LINK){
		CellDraw(pbuf,pcdat,prdat,cn+1,x,y,z,rot,revx,revy,color,magx,magy);
	}
}


/*!
*	@brief Goluah GCD(070)形式セル描画
*
*	CellDraw070とCellDraw090は、内部での座標変換の順番が微妙に入れ替わっているので注意
*/
void CDirectDraw::CellDraw070(
						   MYSURFACE **pbuf,//!< GCDで利用するビットマップ配列
						   LPVOID pcdat,	//!< GCD,CELL (GCD_CELL2_070)配列
						   LPVOID prdat,	//!< GCD,切り取り矩形 (GCD_RECT_070)配列
						   DWORD cn,		//!< 描画するセル番号
						   int x,			//!< 描画先基準x
						   int y,			//!< 描画先基準y
						   float z,			//!< 描画先z
						   int rot,			//!< 全体回転角
						   BOOL revx,		//!< 全体反転X
						   BOOL revy,		//!< 全体反転Y
						   DWORD color,		//!< 全体色
						   float magx,		//!< 全体拡大率X
						   float magy,		//!< 全体拡大率Y
						   BOOL shadowed)	//!<影付き
{
	if(pbuf==NULL || pcdat==NULL || prdat==NULL)return;
	if(cn >= GCDMAX_CELLS)return;

	#ifdef AKIDX_DEBUG
	if(magx==0 || magy==0){
		ODS("☆CDirectDraw::CheckBlt2() Warning-magx or magy ==0\n");
	}
	#endif

	GCD_CELL2_070 *cdat=(GCD_CELL2_070*)pcdat;
	GCD_RECT_070  *rdat=(GCD_RECT_070*)prdat;

	/*if (g_config.IsHalfMode())
	{
		x /= 2;
		y /= 2;
		cdat[cn].gcx /= 2;
		cdat[cn].gcy /= 2;
		for(int i=0;i<8;i++){
			cdat[cn].cell[i].dx /= 2;
			cdat[cn].cell[i].dy /= 2;
		}
	}*/

	if (devtypenow != D3DDEVTYPE_HAL)
	{
		for(int i=0;i<8;i++){
			DWORD rn;

			rn = cdat[cn].cell[i].cdr;
			CheckBlt(pbuf[rdat[rn].bmpno],
				x + cdat[cn].cell[i].dx,y + cdat[cn].cell[i].dy,rdat[rn].r,
				revx,
				revy,
				0,z,color);//描画
		}
	}
	else
	{
		D3DXMATRIX matp,mat,tmt,matprv,matprv2;
		float ar2 = 2.0f/480.0f;

		//キャラクターの変換行列
		D3DXMatrixIdentity(&matp);
		D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2*(-1.0f),(float)(cdat[cn].gcy)*ar2*(-1.0f),0);//重心に移動
		matp *= tmt;
		D3DXMatrixScaling(&tmt,magx,magy,1.0f);//拡大
		matp *= tmt;
		D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//回転
		matp *= tmt;
		if(revy){
			d3dxplane_y.d=0;
			D3DXMatrixReflect(&tmt,&d3dxplane_y);//y反転
			matp *= tmt;
		}
		D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2,(float)(cdat[cn].gcy)*ar2,0);//重心に戻す
		matp *= tmt;
		if(revx){
			d3dxplane_x.d=0;
			D3DXMatrixReflect(&tmt,&d3dxplane_x);//x反転
			matp *= tmt;
		}
		D3DXMatrixTranslation(&tmt,(float)x*ar2,(float)y*ar2,0);//表示位置への移動
		matp *= tmt;

		matprv = SetParentMatrix(matp,FALSE);//「親」の変換行列として設定

		DWORD rn;
		BOOL c1revx,c1revy;

		for(int i=0;i<8;i++){
			rn = cdat[cn].cell[i].cdr;
			if(rn < GCDMAX_RECTANGLES && rn!=0 && rdat[rn].bmpno<GCDMAX_IMAGES){
				//変換行列を計算
				D3DXMatrixIdentity(&mat);
				D3DXMatrixTranslation(&tmt,(float)(rdat[rn].center_x)*ar2*(-1.0f),(float)(rdat[rn].center_y)*ar2*(-1.0f),0);//重心に移動
				mat *= tmt;
				D3DXMatrixScaling(&tmt,cdat[cn].cell[i].magx,cdat[cn].cell[i].magy,1.0f);//拡大
				mat *= tmt;
				D3DXMatrixRotationZ(&tmt,D3DXToRadian(cdat[cn].cell[i].rot));//回転
				mat *= tmt;
				D3DXMatrixTranslation(&tmt,(float)(rdat[rn].center_x*cdat[cn].cell[i].magx)*ar2,(float)(rdat[rn].center_y)*ar2,0);//重心に戻す
				mat *= tmt;
				D3DXMatrixTranslation(&tmt,(float)(cdat[cn].cell[i].dx)*ar2,(float)(cdat[cn].cell[i].dy)*ar2,0);//表示位置への移動
				mat *= tmt;

				matprv2 = SetParentMatrix(mat,FALSE,TRUE);

				c1revx = cdat[cn].cell[i].flag & GCDCELL_REVERSE_X;
				c1revy = cdat[cn].cell[i].flag & GCDCELL_REVERSE_Y;

		/*		//テクスチャフィルタリングを決定
				if( cdat[cn].cell[i].magx != 1.0f ||
					cdat[cn].cell[i].magy != 1.0f || 
					(cdat[cn].cell[i].rot + rot)%90 != 0)
				{
					d3ddev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
					d3ddev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
				}
				else
				{
					d3ddev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
					d3ddev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
				}*/

				CheckBlt(pbuf[rdat[rn].bmpno],
					0,0,rdat[rn].r,
					c1revx,
					c1revy,
					0,z,color);//描画

				// ついでだから影も描くぜ
				if (shadowed)
				{
					D3DXPLANE Plane = D3DXPLANE(0, -1, 0, -0.01);
					D3DXMATRIX matp;

					if (stencil_enable)
					{
						d3ddev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILCAPS_INCRSAT);	// ステンシルに書き込む
						d3ddev->SetRenderState( D3DRS_COLORWRITEENABLE,  FALSE );
						d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
					}
					D3DXMatrixShadow(&mat,
						&D3DXVECTOR4(0,-100,100,1),
						&Plane);
					matp = SetParentMatrix(mat, FALSE);
					CheckBlt(pbuf[rdat[rn].bmpno],
						0,0,rdat[rn].r,
						c1revx,
						c1revy,
						0,z,0xAA000000);//影描画

					d3ddev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILCAPS_KEEP);	// 元に
					d3ddev->SetRenderState( D3DRS_COLORWRITEENABLE,  0xFF );
					d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
				}

				SetParentMatrix(matprv2,TRUE);
			}
		}

		SetParentMatrix(matprv,TRUE);//「親」の変換行列を元に戻す
		EnableZ();
	}

/*	//テクスチャーフィルタを元に戻す
	d3ddev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
	d3ddev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );*/

	/*if (g_config.IsHalfMode())
	{
		cdat[cn].gcx *= 2;
		cdat[cn].gcy *= 2;
		for(int i=0;i<8;i++){
			cdat[cn].cell[i].dx *= 2;
			cdat[cn].cell[i].dy *= 2;
		}
	}*/

	if(cdat[cn].flag & GCDCELL2_LINK){
		CellDraw(pbuf,pcdat,prdat,cn+1,x,y,z,rot,revx,revy,color,magx,magy);
	}
}


#ifdef GCD_EDITER
/*!
*	@brief 矩形エディタ用セル描画
*
*	CellDrawに、現在編集中の矩形を強調表示したり、リンクセルを逆方向に
*	さかのぼったりして描画する機能が追加されている。
*	現在GCD(090)形式のもののみサポート。
*
*	@sa CellDraw
*/
void CDirectDraw::CellDrawED(
						   MYSURFACE **pbuf,
						   LPVOID pcdat,
						   LPVOID prdat,
						   DWORD cn,
						   int x,
						   int y,
						   float z,
						   int rot,
						   BOOL revx,
						   BOOL revy,
						   DWORD color,
						   float magx,
						   float magy,
						   UINT ed_rect,			//!< 編集中の矩形番号
						   BOOL color_modulat,		//!< 強調表示を行う
						   BOOL is_previous_cell,	//!< *さかのぼって描画されたセルかどうか
						   BOOL is_next_cell,		//!< *"次"扱いで描画されたセルかどうか
						   BOOL is_edit_cell)		//!< *現在編集ちゅうのセルかどうか
{
	if(pbuf==NULL || pcdat==NULL || prdat==NULL)return;
	if(cn >= GCDMAX_CELLS)return;

	GCD_CELL2_090 *cdat=(GCD_CELL2_090*)pcdat;
	GCD_RECT_090  *rdat=(GCD_RECT_090*)prdat;

	//前セル描画(ED特有)--------------------------------------------------------------------------------
	if(cn>0 && !is_next_cell && (cdat[cn-1].flag & GCDCELL2_LINK)){
		CellDrawED(pbuf,pcdat,prdat,cn-1,x,y,z,rot,revx,revy,color,magx,magy,ed_rect,color_modulat,TRUE,FALSE,FALSE);
	}//------------------------------------------------------------------------------------------------

	//色準備(ED特有)------------------------------------------------------------------------------------
	DWORD color1 = color;
	DWORD color2_a = (color&0xFF000000)>>24;
	DWORD color2_r = (color&0x00FF0000)>>16;
	DWORD color2_g = (color&0x0000FF00)>>8;
	DWORD color2_b = (color&0x000000FF);
	if(is_edit_cell){
		color2_r = (DWORD)(color2_r*0.7f);
		color2_g = (DWORD)(color2_g*0.7f);
		color2_b = (DWORD)(color2_b*0.7f);
	}
	else{
		color2_r = (DWORD)(color2_r*0.4f);
		color2_g = (DWORD)(color2_g*0.4f);
		color2_b = (DWORD)(color2_b*0.4f);
	}
	DWORD color2 = (color2_a<<24) | (color2_r<<16) | (color2_g<<8) | color2_b;
	//-------------------------------------------------------------------------------------------------

	D3DXMATRIX matp,mat,tmt,matprv,matprv2;
	float ar2 = 1.0f/HALF_HEIGHT;
	
	//ZW/ZTフラグ操作
	if((cdat[cn].flag & GCDCELL2_DISABLE_ZT) || (cdat[cn].flag & GCDCELL2_DISABLE_ZW))
	{
		BOOL zt = (cdat[cn].flag & GCDCELL2_DISABLE_ZT);
		BOOL zw = (cdat[cn].flag & GCDCELL2_DISABLE_ZW);
		EnableZ(zt,zw);
	}

	//キャラクターの変換行列
	D3DXMatrixIdentity(&matp);
	float gcx = (float)(cdat[cn].gcx)*ar2;
	float gcy = (float)(cdat[cn].gcy)*ar2;
	if(!(cdat[cn].flag & GCDCELL2_SCA_GCENTER))
	{	
		if(cdat[cn].flag & GCDCELL2_ROT_BASEPOINT)
		{
			//スケール：足元中心 回転：足元中心
			D3DXMatrixScaling(&tmt,magx,magy,1.0f);//拡大
			gcx *= magx;
			gcy *= magy;
			matp *= tmt;
			D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//回転
			matp *= tmt;
			D3DXMatrixTranslation(&tmt,-gcx,-gcy,0);//重心に移動
			matp *= tmt;
		}
		else{
			//スケール：足元中心 回転：重心中心
			D3DXMatrixScaling(&tmt,magx,magy,1.0f);//拡大
			gcx *= magx;
			gcy *= magy;
			matp *= tmt;
			D3DXMatrixTranslation(&tmt,-gcx,-gcy,0);//重心に移動
			matp *= tmt;
			D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//回転
			matp *= tmt;
		}
	}
	else
	{
		if(cdat[cn].flag & GCDCELL2_ROT_BASEPOINT)
		{
			//スケール：重心中心 回転：足元中心
			D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//回転
			matp *= tmt;
			D3DXMatrixTranslation(&tmt,-gcx,-gcy,0);//重心に移動
			matp *= tmt;
			D3DXMatrixScaling(&tmt,magx,magy,1.0f);//拡大
			matp *= tmt;
		}
		else{
			//スケール：重心中心 回転：重心中心
			D3DXMatrixTranslation(&tmt,-gcx,-gcy,0);//重心に移動
			matp *= tmt;
			D3DXMatrixScaling(&tmt,magx,magy,1.0f);//拡大
			matp *= tmt;
			D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//回転
			matp *= tmt;
		}
	}
	if(revy){
		d3dxplane_y.d=0;
		D3DXMatrixReflect(&tmt,&d3dxplane_y);//y反転
		matp *= tmt;
	}
	D3DXMatrixTranslation(&tmt,gcx,gcy,0);//重心に戻す
	matp *= tmt;
	if(revx){
		d3dxplane_x.d=0;
		D3DXMatrixReflect(&tmt,&d3dxplane_x);//x反転
		matp *= tmt;
	}
	D3DXMatrixTranslation(&tmt,(float)x*ar2,(float)y*ar2,0);//表示位置への移動
	matp *= tmt;

	matprv = SetParentMatrix(matp,FALSE,TRUE);//「親」の変換行列として設定

	DWORD rn;
	BOOL c1revx,c1revy;

	for(int i=0;i<8;i++){
		rn = cdat[cn].cell[i].cdr;
		if(rn < GCDMAX_RECTANGLES && rn!=0 && rdat[rn].bmpno<GCDMAX_IMAGES){
			//変換行列を計算
			D3DXMatrixIdentity(&mat);
			D3DXMatrixTranslation(&tmt,(float)(rdat[rn].center_x)*ar2*(-1.0f),(float)(rdat[rn].center_y)*ar2*(-1.0f),0);//重心に移動
			mat *= tmt;
			D3DXMatrixScaling(&tmt,cdat[cn].cell[i].magx,cdat[cn].cell[i].magy,1.0f);//拡大
			mat *= tmt;
			D3DXMatrixRotationZ(&tmt,D3DXToRadian(cdat[cn].cell[i].rot));//回転
			mat *= tmt;
			D3DXMatrixTranslation(&tmt,(float)(rdat[rn].center_x)*ar2,(float)(rdat[rn].center_y)*ar2,0);//重心に戻す
			mat *= tmt;
			D3DXMatrixTranslation(&tmt,(float)(cdat[cn].cell[i].dx)*ar2,(float)(cdat[cn].cell[i].dy)*ar2,0);//表示位置への移動
			mat *= tmt;

			matprv2 = SetParentMatrix(mat,FALSE,TRUE);

			c1revx = cdat[cn].cell[i].flag & GCDCELL_REVERSE_X;
			c1revy = cdat[cn].cell[i].flag & GCDCELL_REVERSE_Y;

			//色変更(ED特有)--------------------------------------------------------------------
			if(color_modulat)
			{
				if(is_edit_cell){
					if(i==ed_rect)color = color1;
					else color = color2;
				}
				else color = color2;
			}
			//---------------------------------------------------------------------------------

			//色を変更(090特有)
			DWORD rcolor_a = (color&0xFF000000)>>24;
			DWORD rcolor_r = (color&0x00FF0000)>>16;
			DWORD rcolor_g = (color&0x0000FF00)>>8;
			DWORD rcolor_b = (color&0x000000FF);
			rcolor_a = (DWORD)(rcolor_a*cdat[cn].cell[i].alpha_ratio);
			rcolor_r = (DWORD)(rcolor_r*cdat[cn].cell[i].red_ratio);
			rcolor_g = (DWORD)(rcolor_g*cdat[cn].cell[i].green_ratio);
			rcolor_b = (DWORD)(rcolor_b*cdat[cn].cell[i].blue_ratio);
			DWORD rcolor = (rcolor_a<<24) | (rcolor_r<<16) | (rcolor_g<<8) | rcolor_b;

			//ブレンディングモード変更(090特有)
			if(cdat[cn].cell[i].flag & GCDCELL_BLEND_ADD){
				SetAlphaMode(GBLEND_KASAN);
			}
			
			CheckBlt(pbuf[rdat[rn].bmpno],
				0,0,rdat[rn].r,
				c1revx,
				c1revy,
				0,z,rcolor);//描画

			//ブレンディングモードレストア
			if(cdat[cn].cell[i].flag & GCDCELL_BLEND_ADD){
				SetAlphaMode(0);
			}

			SetParentMatrix(matprv2,TRUE);
		}
	}

	SetParentMatrix(matprv,TRUE);//「親」の変換行列を元に戻す

	if(!is_previous_cell){
		color = color1;
		if(cdat[cn].flag & GCDCELL2_LINK){
			CellDrawED(pbuf,pcdat,prdat,cn+1,x,y,z,rot,revx,revy,color,magx,magy,
						ed_rect,
						color_modulat,
						FALSE,
						TRUE,
						FALSE);
		}
	}
}
#endif


/*!
*	@brief Goluah GCD形式あたり判定矩形描画
*
*	あたり判定の四角形を描画する。半透明の四角形によって描画される。
*	引数は基本的にCellDrawとおんなじもんである
*/
void CDirectDraw::HanteiDraw(
						LPVOID pcdat,
						LPVOID phdat,	//!< GCD判定矩形(GCD_HANTEI)配列
						DWORD cn,
						BOOL b_atr,
						BOOL b_kas,
						BOOL b_atk,
						int x,
						int y,
						float z,
						int rot,
						BOOL revx,
						BOOL revy,
						float magx,
						float magy)
{
	if(pcdat==NULL || phdat==NULL)return;
	if(cn >= GCDMAX_CELLS)return;

	GCD_CELL2 *cdat=(GCD_CELL2*)pcdat;
	GCD_HANTEI *hdat=(GCD_HANTEI*)phdat;

	D3DXMATRIX matp,mat,tmt,matprv,matprv2;
	float ar2 = 2.0f/480.0f;

	//キャラクターの変換行列
	D3DXMatrixIdentity(&matp);
	D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2*(-1.0f),(float)(cdat[cn].gcy)*ar2*(-1.0f),0);//重心に移動
	matp *= tmt;
	D3DXMatrixScaling(&tmt,magx,magy,1.0f);//拡大
	matp *= tmt;
	D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//回転
	matp *= tmt;
	if(revy){
		d3dxplane_y.d=0;
		D3DXMatrixReflect(&tmt,&d3dxplane_y);//y反転
		matp *= tmt;
	}
	D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2,(float)(cdat[cn].gcy)*ar2,0);//重心に戻す
	matp *= tmt;
	if(revx){
		d3dxplane_x.d=0;
		D3DXMatrixReflect(&tmt,&d3dxplane_x);//x反転
		matp *= tmt;
	}
	D3DXMatrixTranslation(&tmt,(float)x*ar2,(float)y*ar2,0);//表示位置への移動
	matp *= tmt;

	int i;
	MYVERTEX3D vb[4];
	for(i=0;i<4;i++){
		vb[i].tu = vb[i].tv=0;
		vb[i].z = 0;
	}

	d3ddev->SetRenderState(D3DRS_ZENABLE,FALSE);
	d3ddev->SetTexture(0,NULL);
	d3ddev->SetTransform(D3DTS_WORLD,&matp);

	if(b_atr){//当たり判定（黄色）の描画
		for(i=0;i<3;i++){
			//座標値セット
			vb[0].x = hdat[cn].kurai[i].left / HALF_HEIGHT;
			vb[0].y = hdat[cn].kurai[i].top / HALF_HEIGHT;
			vb[1].x = hdat[cn].kurai[i].right / HALF_HEIGHT;
			vb[1].y = hdat[cn].kurai[i].top / HALF_HEIGHT;
			vb[2].x = hdat[cn].kurai[i].left / HALF_HEIGHT;
			vb[2].y = hdat[cn].kurai[i].bottom / HALF_HEIGHT;
			vb[3].x = hdat[cn].kurai[i].right / HALF_HEIGHT;
			vb[3].y = hdat[cn].kurai[i].bottom / HALF_HEIGHT;
			vb[0].color=
				vb[1].color=
				vb[2].color=
				vb[3].color=0x77FFFF00;
			//描画
			d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
		}
	}

	if(b_kas){//重なり判定（あか）の描画
		for(i=0;i<3;i++){
			//座標値セット
			vb[0].x = hdat[cn].kas[i].left / HALF_HEIGHT;
			vb[0].y = hdat[cn].kas[i].top / HALF_HEIGHT;
			vb[1].x = hdat[cn].kas[i].right / HALF_HEIGHT;
			vb[1].y = hdat[cn].kas[i].top / HALF_HEIGHT;
			vb[2].x = hdat[cn].kas[i].left / HALF_HEIGHT;
			vb[2].y = hdat[cn].kas[i].bottom / HALF_HEIGHT;
			vb[3].x = hdat[cn].kas[i].right / HALF_HEIGHT;
			vb[3].y = hdat[cn].kas[i].bottom / HALF_HEIGHT;
			vb[0].color=
				vb[1].color=
				vb[2].color=
				vb[3].color=0x77FF0000;
			//描画
			d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
		}
	}

	if(b_atk){//攻撃判定（あお）の描画
		for(i=0;i<3;i++){
			//座標値セット
			vb[0].x = hdat[cn].attack[i].left / HALF_HEIGHT;
			vb[0].y = hdat[cn].attack[i].top / HALF_HEIGHT;
			vb[1].x = hdat[cn].attack[i].right / HALF_HEIGHT;
			vb[1].y = hdat[cn].attack[i].top / HALF_HEIGHT;
			vb[2].x = hdat[cn].attack[i].left / HALF_HEIGHT;
			vb[2].y = hdat[cn].attack[i].bottom / HALF_HEIGHT;
			vb[3].x = hdat[cn].attack[i].right / HALF_HEIGHT;
			vb[3].y = hdat[cn].attack[i].bottom / HALF_HEIGHT;
			vb[0].color=
				vb[1].color=
				vb[2].color=
				vb[3].color=0x770000FF;
			//描画
			d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
		}
	}

	d3ddev->SetRenderState(D3DRS_ZENABLE,TRUE);
}


/*!
*	@brief 単純線描画
*/
void CDirectDraw::DrawLine(
						   int sx,		//!< 始点x 
						   int sy,		//!< 始点y
						   int gx,		//!< 終点x
						   int gy,		//!< 終点y	
						   DWORD col	//!< 描画色
						   )
{
	MYVERTEX3D vb[2];
	vb[0].color = col;
	vb[0].tu = vb[0].tv = vb[0].z = 0;
	vb[1] = vb[0];

	vb[0].x = (float)sx / HALF_HEIGHT;
	vb[0].y = (float)sy / HALF_HEIGHT;
	vb[1].x = (float)gx / HALF_HEIGHT;
	vb[1].y = (float)gy / HALF_HEIGHT;

	d3ddev->SetTexture(0,NULL);
	d3ddev->SetTransform(D3DTS_WORLD,&matparent);
	d3ddev->SetVertexShader(FVF_3DVERTEX);
	d3ddev->DrawPrimitiveUP(
		D3DPT_LINELIST,2,vb,sizeof(MYVERTEX3D));
}

/*!
*	@brief GCDあたり判定矩形描画
*
*	ラインにより描画される
*	@sa HanteiDraw
*	@sa CellDraw
*/
void CDirectDraw::HRectDraw(LPVOID pcdat,
							LPVOID phdat,
							DWORD cn,
							int x,
							int y,
							float z,
							int rot,
							BOOL revx,
							BOOL revy,
							float magx,
							float magy)
{
	if(pcdat==NULL || phdat==NULL)return;
	if(cn >= GCDMAX_CELLS)return;

	GCD_CELL2 *cdat=(GCD_CELL2*)pcdat;
	GCD_HANTEI *hdat=(GCD_HANTEI*)phdat;

	D3DXMATRIX matp,mat,tmt,matprv,matprv2;
	float ar2 = 2.0f/480.0f;

	//キャラクターの変換行列
	D3DXMatrixIdentity(&matp);
	D3DXMatrixTranslation(&tmt,0,0,0);//重心に移動
	matp *= tmt;
	/*if (cdat[cn].flag & GCDCELL2_SCA_GCENTER)
	{
		D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2*(-1.0f),(float)(cdat[cn].gcy)*ar2*(-1.0f),0);//重心に移動
		matp *= tmt;
	}*/
	D3DXMatrixScaling(&tmt,magx,magy,1.0f);//拡大
	matp *= tmt;
	/*if (cdat[cn].flag & GCDCELL2_SCA_GCENTER)
	{
		//重心から戻す
		D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2,(float)(cdat[cn].gcy)*ar2,0);
		matp *= tmt;
	}*/
	D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx*magx)*ar2*(-1.0f),(float)(cdat[cn].gcy*magy)*ar2*(-1.0f),0);//重心に移動
	matp *= tmt;
	D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//回転
	matp *= tmt;
	//重心に戻す
	D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx*magx)*ar2,(float)(cdat[cn].gcy*magy)*ar2,0);
	matp *= tmt;
	if(revy){
		d3dxplane_y.d=0;
		D3DXMatrixReflect(&tmt,&d3dxplane_y);//y反転
		matp *= tmt;
	}
	if(revx){
		d3dxplane_x.d=0;
		D3DXMatrixReflect(&tmt,&d3dxplane_x);//x反転
		matp *= tmt;
	}
	D3DXMatrixTranslation(&tmt,(float)x*ar2,(float)y*ar2,0);//表示位置への移動
	matp *= tmt;

	matprv = SetParentMatrix(matp,FALSE);//「親」の変換行列として設定

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

	SetParentMatrix(matprv,TRUE);//「親」の変換行列を元に戻す
}


/*!
*	@brief Goluah GCD形式重心位置マーカー描画
*
*	@sa CellDraw
*/
void CDirectDraw::GCenterDraw(
							LPVOID pcdat,
							DWORD cn,
							int x,
							int y,
							float z,
							int rot,
							BOOL revx,
							BOOL revy,
							float magx,
							float magy)
{
	if(pcdat==NULL)return;
	if(cn >= GCDMAX_CELLS)return;

	GCD_CELL2 *cdat=(GCD_CELL2*)pcdat;

	D3DXMATRIX matp,mat,tmt,matprv,matprv2;
	float ar2 = 2.0f/480.0f;

	//キャラクターの変換行列
	D3DXMatrixIdentity(&matp);
	if(revx){
		d3dxplane_x.d=0;
		D3DXMatrixReflect(&tmt,&d3dxplane_x);//x反転
		matp *= tmt;
	}
	//重心位置への移動
	D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2,(float)(cdat[cn].gcy)*ar2,0);
	matp *= tmt;
	//表示位置への移動
	D3DXMatrixTranslation(&tmt,(float)cdat[cn].gcx*ar2*magx+(x-cdat[cn].gcx)*ar2,(float)cdat[cn].gcy*ar2*magy+(y-cdat[cn].gcy)*ar2,0);
	matp *= tmt;

	matprv = SetParentMatrix(matp,FALSE);//「親」の変換行列として設定

	//Draw
	DrawLine(-20,0,20,0,0xFF00FF00);
	DrawLine(0,-20,0,-20,0xFF00FF00);
	DrawLine(-20, 20, 20, 20,0xFF00FF00);
	DrawLine(-20,-20, 20,-20,0xFF00FF00);
	DrawLine( 20,-20, 20, 20,0xFF00FF00);
	DrawLine(-20,-20,-20, 20,0xFF00FF00);

	SetParentMatrix(matprv,TRUE);//「親」の変換行列を元に戻す
}

//*******************************************************************************
//　変換行列設定関連
//*******************************************************************************


/*!
*	@brief 「親」マトリクスを設定
*
*	CDirectDrawのほとんどの描画関数は、ここで設定されたマトリクスの影響を受ける。
*	このマトリクスは、ローカル → ワールド → カメラ → ビュー 変換のうち、
*	ワールド→ローカル 変換に挿入されるもんである。
*
*	通常は単位行列が指定されている。
*	また、変更した場合には、描画後に元の変換行列に戻しておくこと。
*
*	@param mat 新規に設定する変換マトリクス
*	@param root FALSEの場合、設定されるマトリクスは、旧マトリクス*新マトリクス　の掛け算をしたもんである
*	@param insert TRUEの場合、設定されるマトリクスは、新マトリクス*旧マトリクス　の掛け算をしたもんである
*	@return 関数の実行前に設定されていた古いマトリクス
*/
D3DXMATRIX CDirectDraw::SetParentMatrix(D3DXMATRIX& mat,BOOL root,BOOL insert)
{
	D3DXMATRIX matprv=matparent;

	if(root)ResetParentMatrix();

	if(insert)matparent = mat*matparent;
	else matparent *= mat;

	return(matprv);
}


/*!
*	@brief 「親」マトリクスを設定
*
*	SetParentMatrixとなにがちがうのかな？忘れちゃいました。
*/
void CDirectDraw::SetParentMatrix2(D3DXMATRIX *mat,BOOL root,D3DXMATRIX *matprv)
{
	if(matprv!=NULL)*matprv=matparent;
	if(root){
		matparent = *mat;
	}
	else matparent *= *mat;
}


/*!
*	@brief 「親」マトリクスに単位行列を設定する
*	@sa SetParentMatrix
*/
D3DXMATRIX CDirectDraw::ResetParentMatrix()
{
	D3DXMATRIX matprv=matparent;
	D3DXMatrixIdentity(&matparent);
	return(matprv);
}


// view & projection ------------------------------------------------------------
/*!
*	@brief ワールド→ディスプレイ座標変換マトリクス設定
*
*	カメラとビューの座標変換マトリクスをリセットする
*	D3DTS_VIEW と、D3DTS_PROJECTION のマトリクスが設定される
*
*	@sa camera_x
*	@sa camera_zurax
*	@sa camera_y
*	@sa camera_zuray
*	@sa camera_z
*/
void CDirectDraw::ResetTransformMatrix()
{
	D3DXMATRIX matw,matv,matp;

	D3DXMatrixLookAtRH(&matv,
		&D3DXVECTOR3(camera_x + camera_zurax,camera_y+camera_zuray,camera_z),
		&D3DXVECTOR3(camera_x,camera_y,0),
		&D3DXVECTOR3(0,-3,0));
	d3ddev->SetTransform(D3DTS_VIEW,&matv);

	//projection
	D3DXMatrixPerspectiveFovRH(&matp,
		(float)(2.0*atan2(1.0,3.0)),
		ASPECTRATIO,
		0.1f,
		50.0f);
	d3ddev->SetTransform(D3DTS_PROJECTION,&matp);
}



/*!
*	@brief カメラ、ビュー　マトリックスの設定
*
*	試合のときに描画位置がキャラクターの位置によって移動するけど、
*	それを考慮して描画するかどうか。
*	D3DTS_VIEW と、D3DTS_PROJECTION が影響を受けます。
*	@sa ResetTransformMatrix
*
*	@param b カメラの位置を考慮するか否かみたいなもの
*			FALSE の場合、x:0-640 , y:0-480 のディスプレイ座標系で指定して2D描画すると、
*			そのまんまに表示されるようになる。はず
*/
void CDirectDraw::SetTransform(BOOL b)
{
//	camera_z = -3.0;
	if(b){
		camera_x = (float)center_x/320.0f  * ASPECTRATIO;
		camera_y = -(float)tan(D3DXToRadian(40));
		ResetTransformMatrix();
		ResetParentMatrix();
	}
	else{
		camera_x = 1.0f*ASPECTRATIO;
		camera_y = 1.0f;
		ResetTransformMatrix();
		ResetParentMatrix();
		d3ddev->SetTransform(D3DTS_WORLD,&matparent);
	}
}

/*!
*	@brief ワールド変換マトリクスを指定
*
*	D3DTS_WORLDに指定マトリックスを設定します。
*	たぶん、「D3DTS_WORLD」って綴りを覚えてなかったからこの関数が生まれたんだと思われる。
*/
void CDirectDraw::SetWorldMatrix(D3DXMATRIX *pmat)
{
	d3ddev->SetTransform(D3DTS_WORLD,pmat);
}




#define NUMCIRCLEDIVIDE	40//「円」をいくつの多角形で表現するか。

/*!
*	@brief 円の描画
*/
void CDirectDraw::DrawCircle(int x,			//!< 円の中心位置x
							 int y,			//!< 円の中心位置y
							 int rad,		//!< 円の半径
							 int w,			//!< 線のふとさ
							 float z,		//!< 円のz位置
							 DWORD color,	//!< 円の色
							 BOOL toumei,	//!< 線の内側を透明化するか
							 float rot,		//!< 円の回転角
							 BOOL rot_y,	//!< TRUE:Y軸を軸として回転する,FALSE:X軸を軸として回転する
							 BOOL hosei		//!< 補正をいれるかどうか(なんの?)
							 )
{
	float kakudo;
	float radious = rad/HALF_HEIGHT2;
	float futosa = w/HALF_HEIGHT2;

	if(radious-futosa/2 < 0){
		radious= futosa/2;
	}

	MYVERTEX3D vb[(NUMCIRCLEDIVIDE+1)*2];
	for(int i=0;i<NUMCIRCLEDIVIDE;i++){
		vb[i*2].color = vb[i*2+1].color =color;
		vb[i*2].tu = vb[i*2].tv = vb[i*2+1].tu = vb[i*2+1].tv = 0;
		vb[i*2].z = vb[i*2+1].z = z;
		kakudo = ((2.0f*3.1415926f)/(float)NUMCIRCLEDIVIDE) *i;
		vb[i*2].x   = (radious+futosa/2)*(float)sin(kakudo);
		vb[i*2+1].x = (radious-futosa/2)*(float)sin(kakudo);
		vb[i*2].y   = (radious+futosa/2)*(float)cos(kakudo);
		vb[i*2+1].y = (radious-futosa/2)*(float)cos(kakudo);
		if(toumei)vb[i*2+1].color &= 0x00FFFFFF;
	}
	vb[NUMCIRCLEDIVIDE*2  ] = vb[0];
	vb[NUMCIRCLEDIVIDE*2+1] = vb[1];

	D3DXMATRIX matw,mattrans,matrot,matrh;
	D3DXMatrixTranslation(&mattrans,x/HALF_HEIGHT2,y/HALF_HEIGHT2,0);
	if(rot_y){
		D3DXMatrixRotationY(&matrh,(float)atan2(camera_x - x/HALF_HEIGHT2,-3));
		D3DXMatrixRotationY(&matrot,D3DXToRadian(rot));
	}
	else{ 
		D3DXMatrixRotationX(&matrh,(float)atan2(camera_y - y/HALF_HEIGHT2,-3));
		D3DXMatrixRotationX(&matrot,D3DXToRadian(rot));
	}
	matw = matrh*matrot*mattrans;

	d3ddev->SetRenderState(D3DRS_ZENABLE,FALSE);
	d3ddev->SetTexture(0,NULL);
	d3ddev->SetTransform(D3DTS_WORLD,&matw);
	d3ddev->SetVertexShader( FVF_3DVERTEX );
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,NUMCIRCLEDIVIDE*2,vb,sizeof(MYVERTEX3D));
	d3ddev->SetRenderState(D3DRS_ZENABLE,TRUE);

	return;
}


/*!
*	@brief テキスト描画
*
*	フォントを使って描画します。結構重いっぽい。
*	色はブルー固定らしい。（なんで？）
*/
void CDirectDraw::DrawBlueText(
							RECT& r,		//!< 描画する矩形領域
							char *text,		//!< テキスト文字列
							int len,		//!< テキスト文字数
							DWORD method,	//!< Win32APIのDrawText参照。DT_LEFTとかそういうの
							DWORD size)		//!< 描画サイズ。1～(AKIDX_FONTNUM-1)
{
	if(size<1)return;
	if(size>AKIDX_FONTNUM-1)return;
	if(g_config.IsHalfMode()){
		size--;
		r.left/=2;
		r.right/=2;
		r.top/=2;
		r.bottom/=2;
	}

	lpFont[size]->DrawText(text, len, &r
                    ,method
                    ,0xFF5522FF
                    );
}

//! sa DrawBlueText
void CDirectDraw::DrawRedText(RECT& r,char *text,int len,DWORD method,DWORD size)
{
	if(size<1)return;
	if(size>AKIDX_FONTNUM-1)return;
	if(g_config.IsHalfMode()){
		size--;
		r.left/=2;
		r.right/=2;
		r.top/=2;
		r.bottom/=2;
	}
	lpFont[size]->DrawText(text, len, &r
                    ,method
                    ,0xFFFF2255
                    );
}

void CDirectDraw::ReduceColor(DWORD alpha, bool isShadow /* = false */)
{
	MYVERTEX3D* vb = NULL;
	float ar = 320.0f/240.0f;

	if (alpha == 0xFF)
		return;		// ｲﾗﾈ

	// alpha値で暗くするらしい
	d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
	d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);

	// スクリーン全体を覆うポリゴン
	if ( !pMyVertex || FAILED(pMyVertex->Lock(0, 0, (BYTE**)&vb, D3DLOCK_DISCARD)) )
		return;
	vb[0].color = alpha << 24;
	vb[1].color = alpha << 24;
	vb[2].color = alpha << 24;
	vb[3].color = alpha << 24;
	vb[0].x =  0.0f*ar;
	vb[1].x =  0.0f*ar;
	vb[2].x =  2.0f*ar;
	vb[3].x =  2.0f*ar;
	vb[0].y =  0.0f;
	vb[1].y =  2.0f;
	vb[2].y =  0.0f;
	vb[3].y =  2.0f;
	vb[0].z =  0.4f;
	vb[1].z =  0.4f;
	vb[2].z =  0.4f;
	vb[3].z =  0.4f;
	vb[0].tu = 0.0f;
	vb[1].tu = 0.0f;
	vb[2].tu = 0.0f;
	vb[3].tu = 0.0f;
	vb[0].tv = 0.0f;
	vb[1].tv = 0.0f;
	vb[2].tv = 0.0f;
	vb[3].tv = 0.0f;
	pMyVertex->Unlock();

	if (!isShadow)
		EnableZ(FALSE,FALSE);
	d3ddev->SetStreamSource(0, pMyVertex, sizeof(MYVERTEX3D));
	d3ddev->SetTexture(0, NULL);
	d3ddev->SetVertexShader( FVF_3DVERTEX );
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// 元に戻す
	EnableZ();
	SetAlphaMode(GBLEND_HANTOUMEI);
}



/*----------------------------------------------------------------------------------
	フロントバッファコピー
------------------------------------------------------------------------------------*/
#ifndef GCD_EDITER

extern RECT g_rcClient;

/*!
*	現在のフロントバッファーの内容をコピーして返す。
*	テクスチャーは呼び出し側でReleaseすること。
*	ウィンドウモードの場合、別なウインドウが邪魔してるとそれも写りこむ。
*	戻されるテクスチャーのフォーマットはA1R5G5B5。
*
*	@return 失敗したらNULL
*/
LPDIRECT3DTEXTURE8 CDirectDraw::GetFrontBufferCopy()
{
	//生成するテクスチャーサイズを決定
	UINT tex_width = m_desktop_width;//g_DISPLAYWIDTH;
	UINT tex_height = m_desktop_height;//g_DISPLAYHEIGHT;

	//コピー用テクスチャ生成
	LPDIRECT3DTEXTURE8 ret=NULL;
	if(D3D_OK!=d3ddev->CreateTexture(          
							tex_width,	//UINT Width,
							tex_height,	//UINT Height,
							1,			//UINT Levels(mipmap),
							0,			//DWORD Usage,
							D3DFMT_A8R8G8B8,	//D3DFORMAT Format,コピーするときはこのフォーマットじゃなきゃダメらしい
//							d3dpp.BackBufferFormat,	//D3DFORMAT Format,ひょっとしたらこっちかもしれないので試しに差し替えてみた
							D3DPOOL_SYSTEMMEM ,//D3DPOOL Pool,コピーするときはシステムメモリじゃなきゃダメらしい
							&ret		//IDirect3DTexture9** ppTexture
							))
	{
		return NULL;
	}
	if(!ret)return NULL;

	//テクスチャーからサーフェースを取得する
	//リファレンスカウントが増えるから、Releaseすること
	LPDIRECT3DSURFACE8 surface = NULL;
	if(D3D_OK!=ret->GetSurfaceLevel(0,&surface))
	{
		ret->Release();
		return NULL;
	}

	D3DSURFACE_DESC suf_desc;
	surface->GetDesc(&suf_desc);
	char tekito[256];
	sprintf(tekito,"surface %d,%d\n",suf_desc.Width,suf_desc.Height);
	OutputDebugString(tekito);

	//コピー
	if(D3D_OK!=d3ddev->GetFrontBuffer(surface))//Data(0,surface))
	{
		ret->Release();
		surface->Release();
		return NULL;
	}

	//一旦別テクスチャにコピーしにゃならん。めんどくせー
	LPDIRECT3DTEXTURE8 ret2=NULL;
	LPDIRECT3DSURFACE8 surface2 = NULL;
	do
	{
		//ウィンドウモードの場合、デスクトップ全体のコピーが
		//作られてしまうので、オフセットする必要がある
		UINT offset_x = 0;
		UINT offset_y = 0;
		if(m_is_win)
		{
			offset_x = g_rcClient.left;
			offset_y = g_rcClient.top;
		}


		tex_width = g_DISPLAYWIDTH;
		tex_height = g_DISPLAYHEIGHT;
		//コピー用テクスチャ生成
		
		/*if(D3D_OK!=d3ddev->CreateTexture(          
								tex_width,	//UINT Width,
								tex_height,	//UINT Height,
								1,			//UINT Levels(mipmap),
								0,			//DWORD Usage,
								D3DFMT_A8R8G8B8,	//D3DFORMAT Format,コピーしたフォーマットと同じの
								D3DPOOL_MANAGED ,//D3DPOOL Pool,描画するときはシステムメモリじゃダメらしい
								&ret2		//IDirect3DTexture9** ppTexture
								))*/
		if(D3D_OK!=d3ddev->CreateTexture(          
								tex_width,	//UINT Width,
								tex_height,	//UINT Height,
								1,			//UINT Levels(mipmap),
								0,			//DWORD Usage,
								D3DFMT_A1R5G5B5,	//D3DFORMAT Format,コピーしたフォーマットと同じの
								D3DPOOL_MANAGED ,//D3DPOOL Pool,描画するときはシステムメモリじゃダメらしい
								&ret2		//IDirect3DTexture9** ppTexture
								))
		{
			break;
		}

		//テクスチャーからサーフェースを取得する
		//リファレンスカウントが増えるから、Releaseすること
		if(D3D_OK!=ret2->GetSurfaceLevel(0,&surface2))
		{
			break;
		}

		//2テクスチャをロック。
		D3DLOCKED_RECT lr , lr2;
		if(D3D_OK != surface->LockRect(&lr,NULL,0))
		{
			break;
		}
		if(D3D_OK != surface2->LockRect(&lr2,NULL,0))
		{
			surface->UnlockRect();
			break;
		}

		//コピー（1ピクセルは32ビット）
		for(UINT y=0;y<tex_height;y++)
		{
			//A8R8G8B8→A8R8G8B8
		/*	if(y+offset_y>=m_desktop_height)break;
			DWORD *src = (DWORD*)((BYTE*)lr.pBits + lr.Pitch*(y+offset_y) + offset_x*4);
			DWORD *dst = (DWORD*)((BYTE*)lr2.pBits + lr2.Pitch*y);
			for(UINT x=0;x<tex_width;x++)
			{
				if(x+offset_x>=m_desktop_width)break;
				(*dst) = (*src);
				*dst|=0xFF000000;//ついでにα値も最大にしなきゃだめみたいですよ？
				dst++;
				src++;
			}*/

			//A8R8G8B8→A1R5G5B5
			if(y+offset_y>=m_desktop_height)break;
			DWORD *src = (DWORD*)((BYTE*)lr.pBits + lr.Pitch*(y+offset_y) + offset_x*4);
			WORD *dst = (WORD*)((BYTE*)lr2.pBits + lr2.Pitch*y);
			for(UINT x=0;x<tex_width;x++)
			{
				if(x+offset_x>=m_desktop_width)break;

				WORD r = (BYTE)(((*src) >> 16)&0x000000FF);
				WORD g = (BYTE)(((*src) >> 8 )&0x000000FF);
				WORD b = (BYTE)(((*src)      )&0x000000FF);

				(*dst) = 0x8000 | ((r<<7)&0x7C00) | ((g<<2)&0x03E0) | ((b>>3)&0x001F);
			//	(*dst) <<= 1;
			//	(*dst) |=1;
				dst++;
				src++;
			}

		}

		//テクスチャ案ロック
		surface->UnlockRect();
		surface2->UnlockRect();
	}
	while(0);

	//おっけ～？
	surface->Release();
	ret->Release();

	if(surface2)surface2->Release();
	return ret2;
}

/*!
*	現在のフロントバッファーの内容をコピーして返す。
*	スクリーンショット用。
*	戻されたポインタは呼び出し側でdeleteすること。
*
*	@param wdt [out]コピーしたイメージの幅
*	@param hgt [out]コピーしたイメージの高さ
*	@return 32ビット配列。失敗はNULL。
*/
DWORD* CDirectDraw::GetFrontBufferCopyRaw(UINT *wdt,UINT *hgt)
{
	//生成するテクスチャーサイズを決定
	UINT tex_width = m_desktop_width;//g_DISPLAYWIDTH;
	UINT tex_height = m_desktop_height;//g_DISPLAYHEIGHT;

	//コピー用テクスチャ生成
	LPDIRECT3DTEXTURE8 ret=NULL;
	if(D3D_OK!=d3ddev->CreateTexture(          
							tex_width,	//UINT Width,
							tex_height,	//UINT Height,
							1,			//UINT Levels(mipmap),
							0,			//DWORD Usage,
							D3DFMT_A8R8G8B8,	//D3DFORMAT Format,コピーするときはこのフォーマットじゃなきゃダメらしい
//							d3dpp.BackBufferFormat,	//D3DFORMAT Format,ひょっとしたらこっちかもしれないので試しに差し替えてみた
							D3DPOOL_SYSTEMMEM ,//D3DPOOL Pool,コピーするときはシステムメモリじゃなきゃダメらしい
							&ret		//IDirect3DTexture9** ppTexture
							))
	{
		return NULL;
	}
	if(!ret)return NULL;

	//テクスチャーからサーフェースを取得する
	//リファレンスカウントが増えるから、Releaseすること
	LPDIRECT3DSURFACE8 surface = NULL;
	if(D3D_OK!=ret->GetSurfaceLevel(0,&surface))
	{
		ret->Release();
		return NULL;
	}

	D3DSURFACE_DESC suf_desc;
	surface->GetDesc(&suf_desc);
	char tekito[256];
	sprintf(tekito,"surface %d,%d\n",suf_desc.Width,suf_desc.Height);
	OutputDebugString(tekito);

	//コピー
	if(D3D_OK!=d3ddev->GetFrontBuffer(surface))//Data(0,surface))
	{
		ret->Release();
		surface->Release();
		return NULL;
	}

	//コピー
	DWORD *rret = NULL;
	BOOL fail = TRUE;
	do
	{
		//ウィンドウモードの場合、デスクトップ全体のコピーが
		//作られてしまうので、オフセットする必要がある
		UINT offset_x = 0;
		UINT offset_y = 0;
		if(m_is_win)
		{
			offset_x = g_rcClient.left;
			offset_y = g_rcClient.top;
		}


		tex_width = g_DISPLAYWIDTH;
		tex_height = g_DISPLAYHEIGHT;
		
		*wdt = g_DISPLAYWIDTH;
		*hgt = g_DISPLAYHEIGHT;

		//コピー用メモリ確保
		rret = new DWORD [ tex_width*tex_height ];

		//2テクスチャをロック。
		D3DLOCKED_RECT lr ;
		if(D3D_OK != surface->LockRect(&lr,NULL,0))
		{
			break;
		}

		//コピー（1ピクセルは32ビット）
		for(UINT y=0;y<tex_height;y++)
		{
			//A8R8G8B8→DWORD
			if(y+offset_y>=m_desktop_height)break;
			DWORD *src = (DWORD*)((BYTE*)lr.pBits + lr.Pitch*(y+offset_y) + offset_x*4);
			DWORD *dst = (DWORD*)(rret + y*tex_width);
			for(UINT x=0;x<tex_width;x++)
			{
				if(x+offset_x>=m_desktop_width)break;
				(*dst) = (*src);
				*dst|=0x00000000;//α値？Flag？
				dst++;
				src++;
			}

		}

		//テクスチャ案ロック
		surface->UnlockRect();

		fail = FALSE;
	}
	while(0);

	//おっけ～？
	surface->Release();
	ret->Release();

	if(fail)DELETEARRAY(rret);
	return rret;
}

#endif