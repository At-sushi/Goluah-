
/*==================================================================================
	
	l_directdraw.cpp
	
	CDirectDraw�N���X�̊�{�I�ȕ����B
	��ɏ�������j�����̕������L�q

	MFC�A�v���P�[�V�����Ŏg�p����ꍇ�́u�v���R���p�C���w�b�_�[���g�p���Ȃ��v�ɂ���

====================================================================================*/


#ifndef GCD_EDITER
#include "stdafx.h"//�{�̂̏ꍇ
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
	�G�f�B�^�[�̏ꍇ�̐ݒ�
-----------------------------------------------------------------------------------*/
#ifdef GCD_EDITER//���G�f�B�^�̏ꍇ-------------------------------------------------

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

#else//��Goluah�{�̂̏ꍇ-----------------------------------------------------------

#include "define_const.h"
#include "define_macro.h"
#include "global.h"//config���K�v

#ifdef _DEBUG
#define ODS(a)	gbl.ods2(a)
#else
#define ODS(a)	((void)0)
#endif

#endif

/*!
*	�\�z�B
*	�p�����[�^�̃��Z�b�g���s���̂�
*/
CDirectDraw::CDirectDraw()
{
	//�p�����[�^���Z�b�g
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
*	������
*	Direct3D�̏��������s���B
*	�t���X�N���[�����[�h�̏ꍇ�A���̎��_�Ńt���X�N���[����ԂɂȂ�
*
*	@param hwnd �A�v���P�[�V�����̃��C���E�C���h�E�n���h��
*	@param win TRUE:�E�C���h�E���[�h, FALSE:�t���X�N���[��
*	@return TRUE:����, FALSE:���s
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

	//�ő�e�N�X�`���T�C�Y�擾
	D3DCAPS8 cap8;
	if(dd->GetDeviceCaps(D3DADAPTER_DEFAULT,devtypenow,&cap8)!=D3D_OK){
		Destroy();
		return FALSE;
	}
	maxtexturewidth = cap8.MaxTextureWidth;
	maxtextureheight = cap8.MaxTextureHeight;

	// �X�e�[�g������
	InitStates();

	//x,y���ʂ̃f�[�^�Z�b�g
	d3dxplane_x.a=-1.0f;//x���� (x=0)
	d3dxplane_x.b=0;
	d3dxplane_x.c=0;
	d3dxplane_x.d=0;
	d3dxplane_y.a=0;//y���� (y=0)
	d3dxplane_y.b=-1.0f;
	d3dxplane_y.c=0;
	d3dxplane_y.d=0;

	//�e�N�X�`���ɗp����t�H�[�}�b�g
	if (devtypenow == D3DDEVTYPE_HAL)
		texformat=g_config.TexFormat();
	else
		texformat = d3dpp.BackBufferFormat;

	//�t�H���g�̍쐬
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

	// �X�v���C�g����
	/*if ( FAILED(D3DXCreateSprite(d3ddev, &pSprite)) )
	{
		Destroy();
		return FALSE;
	}*/

	// �`��p�̒��_�o�b�t�@
	if ( FAILED(d3ddev->CreateVertexBuffer(sizeof(MYVERTEX3D) * 4, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
											FVF_3DVERTEX, D3DPOOL_DEFAULT, &pMyVertex)) )
	{
		Destroy();
		return FALSE;
	}

	// �K���}�����v��ݒ肵�Ă݂�
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
	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);//�J�����O�Ȃ�
    d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);//���C�e�B���O����
	if (devtypenow != D3DDEVTYPE_HAL)
	{
		d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);//�_����
		d3ddev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);//�t���b�g�V�F�[�f�B���O
	}
	else
	{
		//�A���t�@�u�����f�B���O�L��
		d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
		d3ddev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		d3ddev->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

		//��������Ȃ��ƃf�B�t���[�Y�F���L���ɂȂ�Ȃ�
		d3ddev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		d3ddev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		d3ddev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

		//�A���t�@�e�X�g
		if(!g_config.NoAlphaTest()){
			if(d3ddev->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE ) ==D3D_OK){
				d3ddev->SetRenderState( D3DRS_ALPHAREF, 0x02 );
				d3ddev->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER );
			}
		}

		//�|�C���g�X�v���C�g
		d3ddev->SetRenderState(D3DRS_POINTSPRITEENABLE,TRUE);

		//�}���`
		//d3ddev->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);

		// �X�e���V��
		if (stencil_enable)
		{
			d3ddev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
			d3ddev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
		}
	}

	d3ddev->SetVertexShader( FVF_3DVERTEX );//�Ƃ肠�����ݒ肵�Ƃ�
}


/*!
*	Direct3D����������
*
*	Initialize�̂����ADirect3D�ADirect3DDevice�̐�����
*
*	@param hwnd �A�v���P�[�V�����̃��C���E�C���h�E�n���h��
*	@param win TRUE:�E�C���h�E���[�h, FALSE:�t���X�N���[��
*	@return TRUE:����, FALSE:���s
*/
BOOL CDirectDraw::InitDirectDraw(HWND hwnd,BOOL win)
{
	// Direct3D �I�u�W�F�N�g���쐬
	dd = Direct3DCreate8(D3D_SDK_VERSION);
    if (NULL == dd){
        MessageBox(hwnd,"Direct3DCreate8�Ɏ��s\nDirectX�̃o�[�W�������Â��Ǝv���","�G���[",MB_OK|MB_ICONSTOP);
        return(FALSE);
    }

	// Direct3D �������p�����[�^�̐ݒ�
    ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.BackBufferCount = 1;
	d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// �f�o�C�X�^�C�v����
	devtypenow=g_config.DeviceType();

	//�E�C���h�E���[�h�̏ꍇ�̏�����
	if(win){
	    // ���݂̉�ʃ��[�h���擾
	    D3DDISPLAYMODE d3ddm;
		if( dd->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) != D3D_OK ) {
			MessageBox(hwnd,"GetAdapterDisplayMode�Ɏ��s","�G���[",MB_OK|MB_ICONSTOP);
			RELEASE(dd);
			return(FALSE);
		}
		// �E�C���h�E : ���݂̉�ʃ��[�h���g�p
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
	//�t���X�N���[���̏ꍇ�̏�����
    else{
		// �g�p�\�ȉ�ʃ��[�h��T���đI��
		if( dd->CheckDeviceType(D3DADAPTER_DEFAULT, devtypenow, D3DFMT_X8R8G8B8,
								D3DFMT_X8R8G8B8, FALSE) == D3D_OK)
								d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		else if( dd->CheckDeviceType(D3DADAPTER_DEFAULT, devtypenow, D3DFMT_X1R5G5B5,
								D3DFMT_X1R5G5B5, FALSE) == D3D_OK)
								d3dpp.BackBufferFormat = D3DFMT_X1R5G5B5;
		else if( dd->CheckDeviceType(D3DADAPTER_DEFAULT, devtypenow, D3DFMT_R5G6B5,
								D3DFMT_R5G6B5, FALSE) == D3D_OK)
								d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
		else { // �_�������I
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
	// �}���`�T���v�����O
	/*if ( SUCCEEDED(dd->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
				   D3DDEVTYPE_HAL, d3dpp.BackBufferFormat, d3dpp.Windowed,
				   D3DMULTISAMPLE_2_SAMPLES)) &&
		SUCCEEDED(dd->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
				   D3DDEVTYPE_HAL, D3DFMT_D16, d3dpp.Windowed,
				   D3DMULTISAMPLE_2_SAMPLES)) )
		d3dpp.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;*/ // VRAM��{�߂��H���݂����Ȃ̂ŕۗ�

	// �X�e���V�����g�������Ȃ�g��
	if ( SUCCEEDED(dd->CheckDeviceFormat(D3DADAPTER_DEFAULT, devtypenow, d3dpp.BackBufferFormat,
										 D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8)) )
	{
		stencil_enable = true;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	}
 
    // �f�o�C�X�̍쐬
	DWORD vertexprocessmode=D3DCREATE_HARDWARE_VERTEXPROCESSING;

	if (devtypenow != D3DDEVTYPE_HAL)
		d3dpp.Flags |= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	HRESULT ret_devcreate = dd->CreateDevice(D3DADAPTER_DEFAULT,devtypenow,
		hwnd,vertexprocessmode,&d3dpp,&d3ddev);

	if(D3D_OK != ret_devcreate){//������
		vertexprocessmode=D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		ret_devcreate = dd->CreateDevice(D3DADAPTER_DEFAULT,devtypenow,
			hwnd,vertexprocessmode,&d3dpp,&d3ddev);// �\�t�g���_�����ł�蒼���B

		if(D3D_OK != ret_devcreate){//����ς��߂��B
			// �܂Ƃ��ɃQ�[���o����̂ŕ���
			/*devtypenow = D3DDEVTYPE_REF;
			d3dpp.Flags |= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

			ret_devcreate = dd->CreateDevice(D3DADAPTER_DEFAULT,devtypenow,
				hwnd,vertexprocessmode,&d3dpp,&d3ddev);// REF�g�p*/

			if(D3D_OK != ret_devcreate){//����ł��_�����Ă��H
				TraceCreateDeviceError(ret_devcreate,hwnd);
				return(FALSE);
			}
		}
	}

	return(TRUE);
}

/*!
*	�j��
*/
void CDirectDraw::Destroy()
{
	int i=0;
	DWORD notrelcount = 0;
	
	//�t�H���g�̍폜
	for(int i=0;i<AKIDX_FONTNUM;i++){
		RELEASE(lpFont[i]);
	}

	//�e�N�X�`���̍폜
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
*	@brief �`��J�n
*	���t���[���̕`����J�n����Ƃ��ɂ��̑�����s�����ƁB
*	������`�揈���́A���̊֐��ƁAEndDraw �̊Ԃɍs��Ȃ���΂Ȃ�Ȃ��B
*
*	@param erbs TRUE:�ȑO�̕`����e����������
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
*	�o�b�N�o�b�t�@�̃N���A
*	Z�o�b�t�@���N���A�����
*/
void CDirectDraw::ClearBackBuffer()
{
	d3ddev->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,clearbgcolor,1.0f,0);
}

/*!
*	�`��I���i�t���b�v�j
*	StartDraw ���炱�̊֐��̌Ăяo���܂łɂȂ��ꂽ�`����e�����ۂɕ\�������B
*
*	@sa StartDraw
*/
void CDirectDraw::EndDraw()
{
	d3ddev->EndScene();
	HRESULT hr = d3ddev->Present(NULL,NULL,NULL,NULL);

	if ( hr == D3DERR_DEVICELOST )
	{
		// DirectDraw�f�o�C�X�ď�������
		while (1)
		{
			// ����ȂƂ���ŃE�B���h�E���b�Z�[�W����
			MSG msg;

			if(PeekMessage( &msg, NULL, 0, 0 ,PM_REMOVE)){//message loop
				if (msg.message == WM_QUIT) break;
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			
			// ������ƒ��߂�
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
*	�e�L�X�g�`��p�t�H���g����
*	
*	@param h �t�H���g�̃T�C�Y
*	@return �t�H���g
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
                      "�l�r �o�S�V�b�N"
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

// �E�B���h�E�T�C�Y�ύX����
/*void CDirectDraw::OnWindowResized(int width, int height)
{
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;

	ResetDirectDraw();
}*/

// �X�N���[�����[�h�ύX
BOOL CDirectDraw::ChangeScreenMode(BOOL win)
{
	if (win && d3dpp.Windowed == FALSE)
	{
		// �E�B���h�E���[�h�ɕύX

	    // ���݂̉�ʃ��[�h���擾
	    D3DDISPLAYMODE d3ddm;
		if( dd->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) != D3D_OK ) {
			MessageBox(d3dpp.hDeviceWindow,"GetAdapterDisplayMode�Ɏ��s","�G���[",MB_OK|MB_ICONSTOP);
			RELEASE(dd);
			return(FALSE);
		}
		// �E�C���h�E : ���݂̉�ʃ��[�h���g�p
	    d3dpp.BackBufferFormat = d3ddm.Format;
		d3dpp.Windowed = TRUE;

		ResetDirectDraw();
		SetWindowLong(d3dpp.hDeviceWindow, GWL_STYLE, WS_BORDER | WS_CAPTION | WS_SYSMENU);
	}
	else if (!win && d3dpp.Windowed == TRUE)
	{
		// �t���X�N���[�����[�h�ɕύX
		SetWindowLong(d3dpp.hDeviceWindow, GWL_STYLE, WS_POPUP);

		// �g�p�\�ȉ�ʃ��[�h��T���đI��
		if( dd->CheckDeviceType(D3DADAPTER_DEFAULT, devtypenow, D3DFMT_X8R8G8B8,
								D3DFMT_X8R8G8B8, FALSE) == D3D_OK)
								d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		else if( dd->CheckDeviceType(D3DADAPTER_DEFAULT, devtypenow, D3DFMT_X1R5G5B5,
								D3DFMT_X1R5G5B5, FALSE) == D3D_OK)
								d3dpp.BackBufferFormat = D3DFMT_X1R5G5B5;
		else if( dd->CheckDeviceType(D3DADAPTER_DEFAULT, devtypenow, D3DFMT_R5G6B5,
								D3DFMT_R5G6B5, FALSE) == D3D_OK)
								d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
		else { // �_�������I
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
*	���u�����h�̃��[�h��ݒ肷��
*	�ʏ�̍����A���Z�����A���] ���I�ׂ�
*/
void CDirectDraw::SetAlphaMode(DWORD alphamode)
{
	switch(alphamode){
	case GBLEND_KASAN://���Z����
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
*	Z�e�X�g�E���C�g�̗L���E������ݒ�
*
*	@param test TRUE:���e�X�g���s��
*	@param write TRUE:���o�b�t�@�[�ւ̏������݂��s��
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
*	CreateDevice�Ɏ��s�������R��MessageBox�ŕ\������
*
*	D3D��CreateDevice���Ԃ������^�[���R�[�h����ASDK�w���v�̉�����R�s�y�������̂�
*	�����ă��b�Z�[�W�{�b�N�X�\������B
*
*	@param ret CreateDevice���Ԃ������^�[���R�[�h
*	@param hwnd ���b�Z�[�W�{�b�N�X�̐e�ɂȂ�E�C���h�E�n���h��
*/
void CDirectDraw::TraceCreateDeviceError(HRESULT ret,HWND hwnd)
{
	char *msg;
	msg=(char*)malloc(1024);
	sprintf(msg,"");

	switch(ret){
	case D3DERR_INVALIDCALL:
		sprintf(&msg[strlen(msg)],"D3DERR_INVALIDCALL:");
		sprintf(&msg[strlen(msg)],"���\�b�h�̌Ăяo���������ł���B\n");
		sprintf(&msg[strlen(msg)],"�@�@���Ƃ��΁A���\�b�h�̃p�����[�^�ɖ����Ȓl���ݒ肳��Ă���ꍇ�ȂǁB");
		sprintf(&msg[strlen(msg)],"\n\n����������ł��B");
		sprintf(&msg[strlen(msg)],"\n�E�f�B�X�v���C�̕\���F����ύX���Ă�������");
		sprintf(&msg[strlen(msg)],"\n�E�f�B�X�v���C�̃v���p�e�B/�ڍ�/�Ńn�[�h�E�F�A�A�N�Z�����[�^�ő�ɂ��Ă�������");
		sprintf(&msg[strlen(msg)],"\n�E�u�t�@�C�������w�肵�Ď��s�v����dxdiag.exe�����s���A");
		sprintf(&msg[strlen(msg)],"\n\n�@�@�u�f�B�X�v���C�v�^�u��Direct3D�A�N�Z�����[�^���u�g�p�\�v�ł��邱�Ƃ��m���߂Ă��������B");
		sprintf(&msg[strlen(msg)],"\n�Eg_config.exe�Łu�f�o�C�X�^�C�v�v��ύX���Ă�������");
		break;
	case D3DERR_NOTAVAILABLE:
		sprintf(&msg[strlen(msg)],"D3DERR_NOTAVAILABLE:");
		sprintf(&msg[strlen(msg)],"���̃f�o�C�X�́A�Ɖ�ꂽ�e�N�j�b�N���T�|�[�g���Ă��Ȃ��B");
		sprintf(&msg[strlen(msg)],"\n\n����������ł��B");
		sprintf(&msg[strlen(msg)],"\n�O���t�B�b�N�J�[�h�������ĂȂ��݂����Ȃ�ŁA���������܂����B");
		break;
	case D3DERR_OUTOFVIDEOMEMORY:
		sprintf(&msg[strlen(msg)],"D3DERR_OUTOFVIDEOMEMORY");
		sprintf(&msg[strlen(msg)],"Direct3D ���������s���̂ɏ\���ȃf�B�X�v���C ���������Ȃ��B");
		sprintf(&msg[strlen(msg)],"\n\n����������ł��B");
		sprintf(&msg[strlen(msg)],"\n�E�f�B�X�v���C�̉𑜓x�ƐF�����ł��邾�����炵�Ă��������B");
		sprintf(&msg[strlen(msg)],"\n�Eg_config.exe��320x240���[�h�ɂ��Ă݂Ă�������");
		sprintf(&msg[strlen(msg)],"\n�EPC�𔃂������Ă�������");
		break;
	default:
		sprintf(&msg[strlen(msg)],"�G���[������ł��܂���ł����B");
	}

	MessageBox(hwnd,msg,"�N���Ɏ��s",MB_OK);
	free(msg);
}








/*==================================================================================
	
	


	�r�b�g�}�b�v�̃��[�h/�A�����[�h



====================================================================================*/



/*!
*	256�r�b�g�}�b�v��Ǝ��`���Ń��[�h
*
*	256�F�łȂ���΂Ȃ�Ȃ��̂́A�p���b�g�̑}���ւ����s��Ȃ���΂Ȃ�Ȃ����߁B
*	�r�b�g�}�b�v�̃T�C�Y����K�؂ȃe�N�X�`���T�C�Y���v�Z���A�����ɕ������ă��[�h����B
*	���A���̊֐��Ő������ꂽ�C���`�L�T�[�t�F�C�X��CDirectDraw�N���X�����X�g�Ǘ����A
*	�N���X�̔j�����ɃA�����[�h����Ă��Ȃ��T�[�t�F�C�X��j������i�ŏI��i�j
*
*	@param filename �C���[�W��ǂݍ��ރr�b�g�}�b�v�t�@�C���̃t�@�C�����B
*					palname��NULL�łȂ��Ƃ��́A���̃t�@�C���̃p���b�g�͖��������
*	@param palname �p���b�g��ǂݍ��ރr�b�g�}�b�v�t�@�C�����B
*					NULL�̏ꍇ��filename�̃t�@�C���̃p���b�g���K�p�����
*	@param dmy ���g�p�B�܂�DirectDraw���g���Ă������AVRAM�ɓǂނ��V�X�e���������ɓǂނ��̃t���O���������́B
*
*	@return �Ǝ��`���C���`�L�T�[�t�F�C�X�̃|�C���^�B���s�����ꍇ��NULL
*/
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
		ODS("warning:�e�N�X�`������MAX�𒴂��Ă��܂�\n");
		return(NULL);
	}

	//�r�b�g�}�b�v�̃r�b�g��ǂݍ���
	MYPALLET *bmpbits;
	DWORD bmpwidth,bmpheight;
#ifdef GCD_EDITER
	bret = FALSE;	// �g���Ȃ��A�p�X�B
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

	//�r�b�g�}�b�v�̕����̎d��������
	bret = AssignTextureDiv(&ms[e],bmpwidth,bmpheight);
	if(!bret){
		ODS("AssignTextureDiv�Ɏ��s\n");
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

	//�e�N�X�`���Ƀr�b�g�}�b�v���R�s�[
	DWORD damex,damey;
	for(i=0;i<ms[e].ysufnum;i++){//okasi?
		for(j=0;j<ms[e].xsufnum ;j++){
			//�R�s�[���s
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

	if(g_config.IsHalfMode()){
		ms[e].wg *=2;
		ms[e].hg *=2;
	}

	return(&ms[e]);
}

/*!
*	256�F�摜��Ǝ��`���Ń��[�h
*
*	PNG�Ή��ɔ����A�ǂݍ��ݏ����ɏ_������߂邽�߂ɍ��܂����B
*	�g���q���������t�@�C��������A.png��.bmp�̂Q��ނ̃^�C�v���玩���œǂݍ��݂܂�(PNG�D��j�B
*	���Ƃ́A�قƂ��CreateSurfaceFrom256BMP�ƕς��܂���B
*
*	@param filename �C���[�W��ǂݍ��ރt�@�C���́A�g���q���������t�@�C�����B
*					palname��NULL�łȂ��Ƃ��́A���̃t�@�C���̃p���b�g�͖��������
*	@param palname �p���b�g��ǂݍ��ރt�@�C���́A�g���q���������t�@�C�����B
*					NULL�̏ꍇ��filename�̃t�@�C���̃p���b�g���K�p�����
*	@param dmy ���g�p�B�܂�DirectDraw���g���Ă������AVRAM�ɓǂނ��V�X�e���������ɓǂނ��̃t���O���������́B
*
*	@return �T�[�t�F�C�X�̃|�C���^�B���s�����ꍇ��NULL
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

			// �܂���PNG�`���ŁB
			sprintf(Buffer, "%s%s", filename, ".png");
			if (Buffer_pal) sprintf(Buffer_pal, "%s%s", pallet, ".png");

			if ( result = CreateSurfaceFrom256BMP(Buffer, Buffer_pal, dmy) )
			{
				// ����
				if (Buffer_pal) free(Buffer_pal);
				free(Buffer);
				return result;
			}
			else
			{
				// �p���b�g��BMP�ɁB
				if (Buffer_pal)
				{
					sprintf(Buffer_pal, "%s%s", pallet, ".bmp");

					if ( result = CreateSurfaceFrom256BMP(Buffer, Buffer_pal, dmy) )
					{
						// ����
						if (Buffer_pal) free(Buffer_pal);
						free(Buffer);
						return result;
					}
				}

				// JPEG�`���ł���Ă݂�B
				sprintf(Buffer, "%s%s", filename, ".jpg");

				if ( result = CreateSurfaceFrom256BMP(Buffer, NULL, dmy) )
				{
					// ����
					if (Buffer_pal) free(Buffer_pal);
					free(Buffer);
					return result;
				}

				// ���s������r�b�g�}�b�v�`���ŁB
				sprintf(Buffer, "%s%s", filename, ".bmp");
				if (Buffer_pal) sprintf(Buffer_pal, "%s%s", pallet, ".png");

				if ( result = CreateSurfaceFrom256BMP(Buffer, Buffer_pal, dmy) )
				{
					// ����
					if (Buffer_pal) free(Buffer_pal);
					free(Buffer);
					return result;
				}
				else
				{
					// �p���b�g��BMP�ɁB
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
*	�e�N�X�`���̃T�[�t�F�[�X�ɏ�������
*	CreateSurfaceFrom256BMP�̓����֐��ŁA�e�N�X�`���[�̃T�[�t�F�C�X��
*	�r�b�g�}�b�v�t�@�C���̃C���[�W���R�s�[����B
*
*	@param pbb �K�p�p���b�g�̔z��
*	@param bbpitch �\�[�X(�r�b�g�}�b�v)�̃s�b�`
*	@param offset_x �\�[�X(�r�b�g�}�b�v)�̃R�s�[�ʒuX�I�t�Z�b�g
*	@param offset_y �\�[�X(�r�b�g�}�b�v)�̃R�s�[�ʒuY�I�t�Z�b�g
*	@param damex �R�s�[������_���ȕ����i�e�N�X�`���T�C�Y > �r�b�g�}�b�v�̎c��̃R�s�[�̈� �̂Ƃ��j
*	@param damey �R�s�[������_���ȕ����i�e�N�X�`���T�C�Y > �r�b�g�}�b�v�̎c��̃R�s�[�̈� �̂Ƃ��j
*	@return TRUE:����, FALSE:�c�O
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
		ODS("CopyBB2TS / GetSurfaceLevel�Ɏ��s\n");
		return(FALSE);
	}

	//���ƍ����A�t�H�[�}�b�g���擾
	D3DFORMAT fmt;
	DWORD sw,sh;
	D3DSURFACE_DESC dsc;
	if(D3D_OK != psuf->GetDesc(&dsc)){
		ODS("CopyBB2TS / GetDesc�Ɏ��s\n");
		RELEASE(psuf);
		return(FALSE);
	}
	fmt = dsc.Format;
	sw = dsc.Width;
	sh = dsc.Height;

	if(damex!=0)sw=damex;
	if(damey!=0)sh=damey;

	//�T�[�t�F�C�X�̃��b�N
	D3DLOCKED_RECT lr;
	if(D3D_OK != psuf->LockRect(&lr,NULL,0)){
		ODS("CopyBB2TS / LockRect�Ɏ��s\n");
		RELEASE(psuf);
		return(FALSE);
	}

	//�R�s�[
	DWORD i,j;
	PBYTE pline;
	int onepixsize;
	for(j=0;j<sh;j++){
		pline =  (PBYTE)lr.pBits + lr.Pitch*j ;
		for(i=0;i<sw;i++){
			switch(fmt){
			case D3DFMT_R5G6B5://����͑����g���Ȃ�
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
				ODS("CopyBB2TS / ���̃t�H�[�}�b�g�̓R�s�[�ł��ɂႢ\n");
				psuf->UnlockRect();
				RELEASE(psuf);
				return(FALSE);
			}
			pline += onepixsize;
		}
	}

	psuf->UnlockRect();	//�T�[�t�F�C�X�̃A�����b�N
	RELEASE(psuf);		//GetSurfaceLevel�ŃT�[�t�F�C�X���擾�����Ƃ��Ƀ��t�@�����X�J�E���g�������Ă邩��

	return(TRUE);
}


/*!
*	�e�N�X�`���̃T�[�t�F�[�X�ւ� 1 �s�N�Z���̏������ݏ���
*
*	CopyBB2TS�̓����֐��B�p���b�g�̐F(ARGB=8888)���AARGB=0565 �̌`����
*	�ϊ����āA�w��|�C���^�ʒu�ɏ������ށB
*
*	@param src �w��F
*	@param dst �������ݐ�
*	@retuen �������񂾃o�C�g��(���2)
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
*	�e�N�X�`���̃T�[�t�F�[�X�ւ� 1 �s�N�Z���̏������ݏ���
*
*	CopyBB2TS�̓����֐��B�p���b�g�̐F(ARGB=8888)���AARGB=1555 �̌`����
*	�ϊ����āA�w��|�C���^�ʒu�ɏ������ށB
*
*	@param src �w��F
*	@param dst �������ݐ�
*	@retuen �������񂾃o�C�g��(���2)
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
*	�e�N�X�`���̃T�[�t�F�[�X�ւ� 1 �s�N�Z���̏������ݏ���
*
*	CopyBB2TS�̓����֐��B�p���b�g�̐F(ARGB=8888)���AARGB=4444 �̌`����
*	�ϊ����āA�w��|�C���^�ʒu�ɏ������ށB
*
*	@param src �w��F
*	@param dst �������ݐ�
*	@retuen �������񂾃o�C�g��(���2)
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
*	�e�N�X�`���̃T�[�t�F�[�X�ւ� 1 �s�N�Z���̏������ݏ���
*
*	CopyBB2TS�̓����֐��B�p���b�g�̐F(ARGB=8888)���AARGB=8332 �̌`����
*	�ϊ����āA�w��|�C���^�ʒu�ɏ������ށB
*
*	@param src �w��F
*	@param dst �������ݐ�
*	@retuen �������񂾃o�C�g��(���2)
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
*	�e�N�X�`���̃T�[�t�F�[�X�ւ� 1 �s�N�Z���̏������ݏ���
*
*	CopyBB2TS�̓����֐��B�p���b�g�̐F(ARGB=8888)���AARGB=8888 �̌`����
*	�ϊ����āA�w��|�C���^�ʒu�ɏ������ށB
*
*	@param src �w��F
*	@param dst �������ݐ�
*	@retuen �������񂾃o�C�g��(���4)
*/
DWORD CDirectDraw::CopyOne_A8R8G8B8(MYPALLET src,PBYTE dst)
{
	MYPALLET *pdst;
	pdst = (MYPALLET*)dst;

	*pdst = src;

	return(4);
}


/*!
*	�w��T�C�Y�̃r�b�g�}�b�v�̕������@����
*
*	CreateSurfaceFrom256BMP�̓����֐��B
*	�r�b�g�}�b�v�̃^�e���R��2�̗ݏ�ŕ������邽�߂ɁA�ǂ������T�C�Y�ŕ������邩���v�Z����B
*
*	@param pmsf [out]�C���`�L�T�[�t�F�C�X�̃|�C���^�B�����T�C�Y�̃p�����[�^����������ŕԂ�
*	@param width [in]���̃r�b�g�}�b�v�̕�
*	@param height [in]���̃r�b�g�}�b�v�̍���
*	@return TRUE:����, FALSE:�c�O
*/
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

	return(TRUE);
}


static BOOL GoluahReadFile(HANDLE hFile, LPBYTE bits, DWORD sizeimage, NowLoading_IconItem item, LPDWORD br);

/*!
*	@brief �w��t�@�C�����̃r�b�g�}�b�v����A�摜�r�b�g��ǂݍ���
*
*	CreateSurfaceFrom256BMP�̓����֐��B
*	�ǂݍ���Ő��������z���malloc�֐��ō���Ă�݂����Bfree�Ŏn�����Ă��������B
*
*	@param pbits [out] �ǂݍ��݌���. ���łɃp���b�g(ARGB=8888)�̔z��ɕϊ�����Ă܂�
*	@param width [out] �r�b�g�}�b�v�̕�
*	@param height [out] �r�b�g�}�b�v�̍���
*	@param bmpfilename [in] �r�b�g�}�b�v�̃t�@�C����
*	@param palfilename [in] �K�p����p���b�g�̃t�@�C����
*	@return TRUE:����, FALSE:�c�O
*/
BOOL CDirectDraw::Load256Bitmapbits(MYPALLET **pbits,DWORD *width,DWORD *height,char *bmpfilename,char *palfilename)
{
	BOOL ret2;

	//�Ƃ肠�����p���b�g���m��
	MYPALLET pal[256];
	if(palfilename==NULL)palfilename=bmpfilename;
	ret2 = GetPallet(palfilename,pal);
	if(!ret2){
		ret2 = GetPalletPNG(palfilename,pal);

		if(!ret2){
			return(FALSE);
		}
	}

	//�r�b�g�}�b�v�̃r�b�g���擾~
	//�r�b�g�}�b�v�����[�h
	HANDLE hFile;
	DWORD ret,br,err=FALSE;

	if(bmpfilename==NULL){
		ODS("CDirectDraw::Load256Bitmapbits : �t�@�C������NULL�Ƃ͂ǂ��������Ƃ��H\n");
	}

	//�t�@�C���̃I�[�v��
	hFile=CreateFile(bmpfilename,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		return(NULL);
	}
	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//�O�̂��߃t�@�C���̐擪�Ɉړ�

	//̧�كw�b�_�̓ǂݏo��
	BITMAPFILEHEADER fileheader;
	ret=ReadFile(hFile,&fileheader,sizeof(BITMAPFILEHEADER),&br,NULL);
	if(!ret || br!=sizeof(BITMAPFILEHEADER)){
		ODS("CDirectDraw::Load256Bitmapbits : �t�@�C���̓ǂݍ��݂Ɏ��s(1)\n");
		err=TRUE;
	}
	if(fileheader.bfType != 0x4d42){//"BM"
		ODS("CDirectDraw::Load256Bitmapbits : �[�����̃t�@�C���̓r�b�g�}�b�v�ł͂Ȃ�\n");
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
		ODS("CDirectDraw::Load256Bitmapbits : �t�@�C���̓ǂݍ��݂Ɏ��s(2)\n");
		err=TRUE;
	}
	if(infohed.biSize != sizeof(BITMAPINFOHEADER)){
		ODS("CDirectDraw::Load256Bitmapbits : BITMAPINFOHEADER�̃T�C�Y������Ȃ�\n");
		err=TRUE;
	}
	if(infohed.biBitCount != 8){
		ODS("CDirectDraw::Load256Bitmapbits : �[�����̃t�@�C����256�ł͂Ȃ�\n");
		err=TRUE;
	}
	if(infohed.biCompression != BI_RGB){
		ODS("CDirectDraw::Load256Bitmapbits : ���k���������Ă���炵��\n");
		err=TRUE;
	}
	if(err){
		CloseHandle(hFile);
		return(NULL);
	}
	if(infohed.biClrUsed == 0){//0�̏ꍇ�A256���Ӗ����邱�Ƃ�����炵��
		infohed.biClrUsed=256;
	}

	//�p���b�g�͓ǂ݂Ƃ΂�
	SetFilePointer(hFile,sizeof(RGBQUAD)*infohed.biClrUsed,NULL,FILE_CURRENT);

	//�r�b�g�}�b�v�r�b�g�̃T�C�Y���v�Z����(�r�b�g�}�b�v�̕���4�̔{���Ŋi�[����Ă���炵��)
	DWORD linesize = infohed.biWidth;
	if(infohed.biWidth%4 != 0)linesize +=  ( 4 - infohed.biWidth%4 );
	DWORD sizeimage = linesize * infohed.biHeight;
	//���������m�ۂ��ăr�b�g�}�b�v�r�b�g��ǂݍ���
	LPBYTE bits = (LPBYTE)(malloc(sizeimage));
	ret=GoluahReadFile(hFile,bits,sizeimage,NowLoading_Image,&br);
	CloseHandle(hFile);
	if(!ret || br!=sizeimage){
		OutputDebugString("CDirectDraw::Load256Bitmapbits : �t�@�C���̓ǂݍ��݂Ɏ��s(3)\n");
		free(bits);
		return(NULL);
	}

	//�r�b�g�}�b�v�̑傫���ɂ��킹�ĐV���Ƀ������̈���쐬����
	LPVOID pnewbits2;
	DWORD ishalf=1;
	if(g_config.IsHalfMode())ishalf=2;
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
			if(g_config.IsHalfMode()){//���������R�s�[���Ȃ�
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

	//����
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
			//�i�s�󋵕\��
			CTNowLoading* task = dynamic_cast<CTNowLoading*>( g_system.FindTask('LOAD') );
			if(task)task->Progress(item, (float)((double)i / sizeimage));
#		endif
	}

	return(ret);
}

// ��O������2�d�Ȃ̂ŁA��p�̃W�����v������p�ӁB
jmp_buf PngErrJamp;
void PngErrHandler(png_structp Png,png_const_charp message)
{
#ifndef GCD_EDITER
	gbl.ods("PNG�ǂݍ��݃G���[�F%s", message);
#endif // GCD_EDITER

//	throw(0x46497743);
	longjmp(PngErrJamp, 0x46497743);
}

#ifndef GCD_EDITER

// �v���O���X�\���p�̃R�[���o�b�N
static void png_read_row_callback(png_structp strPNG, png_uint_32 row, int pass)
{
	if (row * strPNG->rowbytes % 524288 < strPNG->rowbytes)
	{
			//�i�s�󋵕\��
			CTNowLoading* task = dynamic_cast<CTNowLoading*>( g_system.FindTask('LOAD') );
			if(task)task->Progress(NowLoading_Image, (float)((double)row / strPNG->height));
	}
}

// �C���^�[���[�X�̂���摜�p
static void png_read_row_callback_adam7(png_structp strPNG, png_uint_32 row, int pass)
{
	if (row == 0 && pass >= 3)
	{
			//�i�s�󋵕\��
			CTNowLoading* task = dynamic_cast<CTNowLoading*>( g_system.FindTask('LOAD') );
			if(task)task->Progress(NowLoading_Image, (float)((double)pass*pass / (8*8)));			// �ő�pass�l�̏������@���Ȃ��݂����Ȃ̂ť��
	}
}

#endif	// GCD_EDITER

/*!
*	@brief �w��t�@�C������PNG�t�@�C������A�摜�r�b�g��ǂݍ���
*
*	CreateSurfaceFrom256BMP�̓����֐��ALoad256BitmapBits��PNG�ŁB
*	Load256BitmapBits�Ɠ������Apbits��free�Ŏn�����Ă��������B
*
*	@param pbits [out] �ǂݍ��݌���. ���łɃp���b�g(ARGB=8888)�̔z��ɕϊ�����Ă܂�
*	@param width [out] �r�b�g�}�b�v�̕�
*	@param height [out] �r�b�g�}�b�v�̍���
*	@param pngfilename [in] �r�b�g�}�b�v�̃t�@�C����
*	@param palfilename [in] �K�p����p���b�g�̃t�@�C����
*	@return TRUE:����, FALSE:�c�O
*	@sa Load256Bitmapbits
*/
BOOL CDirectDraw::Load256PNGbits(MYPALLET **pbits,DWORD *width,DWORD *height,char *pngfilename,char *palfilename)
{
	BOOL ret2;

	//�Ƃ肠�����p���b�g���m��
	MYPALLET pal[256];
	if(palfilename==NULL)palfilename=pngfilename;
	ret2 = GetPalletPNG(palfilename,pal);
	if(!ret2){
		ret2 = GetPallet(palfilename,pal);

		if(!ret2){
			return(FALSE);
		}
	}

	//PNG���������胍�[�h
	FILE* fp;

	if(pngfilename==NULL){
		ODS("CDirectDraw::Load256PNGbits : �t�@�C������NULL�Ƃ͂ǂ��������Ƃ��H\n");
		return(FALSE);
	}

	// PNG�\���́H
	png_structp strPNG = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, PngErrHandler, NULL);
	if (!strPNG)
	{
		ODS("CDirectDraw::Load256PNGbits : PNG�\�����޷�ȴ�!!�R(`�D�L)ɳܧ��!!\n");
		return FALSE;
	}

	// ���\���́c���ĉ�����B
	png_infop infoPNG = png_create_info_struct(strPNG);
	if (!infoPNG)
	{
		ODS("CDirectDraw::Load256PNGbits : PNG���\�����޷�ȴ�!!�R(`�D�L)ɳܧ��!!\n");
		png_destroy_read_struct(&strPNG, NULL, NULL);
		return FALSE;
	}

	// �ǂݍ��ނ��B
	fp = fopen(pngfilename, "rb");
	if (!fp)
	{
		png_destroy_read_struct(&strPNG, &infoPNG, NULL);
		return FALSE;
	}

	// try�̊O�Ŏg������
	LONG i,j;
	BYTE** Image = NULL;
	MYPALLET *retbit = NULL;

//	try {
	if (!setjmp(PngErrJamp)) {
		// ���Ƃ̓��C�u�����ɔC����
		png_init_io(strPNG, fp);

		// �V�O�l�`�����
		BYTE sig[8];
		long pos = ftell(fp);

		int len = fread(sig, sizeof(BYTE), 8, fp);
		if ( png_sig_cmp(sig, 0, len) != 0 )
		{
			//ODS("CDirectDraw::Load256PNGbits : �[�����̃t�@�C����PNG�ł͂Ȃ��B\n");
			fclose(fp);
			png_destroy_read_struct(&strPNG, &infoPNG, NULL);
			return FALSE;
		}

		// �擪�ɖ߂�
		fseek(fp, pos, SEEK_SET);

		// PNG���Q�b�c
		png_read_info(strPNG, infoPNG);

		// �C���[�W�w�b�_����
		int bit_depth = 0, color_type = 0;

		if ( !png_get_IHDR(strPNG, infoPNG, (UINT*)width, (UINT*)height, &bit_depth, &color_type, NULL, NULL, NULL) )
		{
			ODS("CDirectDraw::Load256PNGbits : IHDR�ǂݍ��ݎ��s\n");
			fclose(fp);
			png_destroy_read_struct(&strPNG, &infoPNG, NULL);
			return FALSE;
		}

		// �p���b�g�t����256�F����Ȃ��Ƃ��߂�
		if (color_type != 3)
		{
			ODS("CDirectDraw::Load256PNGbits : �p���b�g���Ȃ���B\n");
			fclose(fp);
			png_destroy_read_struct(&strPNG, &infoPNG, NULL);
			return FALSE;
		}
		if (bit_depth != 8)
		{
			ODS("CDirectDraw::Load256PNGbits : �[�����̃t�@�C����256�ł͂Ȃ��B\n");
			fclose(fp);
			png_destroy_read_struct(&strPNG, &infoPNG, NULL);
			return FALSE;
		}

		//�r�b�g�}�b�v�̑傫���ɂ��킹�ĐV���Ƀ������̈���쐬����
		Image = (BYTE**)malloc(*height * sizeof(BYTE**));
		LPVOID pnewbits2;
		DWORD ishalf=1;
		if(g_config.IsHalfMode())ishalf=2;
		pnewbits2 = malloc(sizeof(MYPALLET)* *width * *height / ishalf);
		retbit = (MYPALLET*)pnewbits2;

		DWORD halfcopy=0;

		// �Q�������I�z��
		for (i = 0; i < (LONG)*height; i++)
			Image[i] = (BYTE*)malloc(png_get_rowbytes(strPNG, infoPNG));

#		ifndef GCD_EDITER
			// �R�[���o�b�N�ݒ�
			if (png_get_interlace_type(strPNG, infoPNG) == PNG_INTERLACE_ADAM7)
				png_set_read_status_fn(strPNG, png_read_row_callback_adam7);	// ADAM7
			else
				png_set_read_status_fn(strPNG, png_read_row_callback);			// �W���i�C���^�[���[�X�����j
#		endif	// GCD_EDITER

		// �摜�ǂݍ���
	
		png_read_image(strPNG, Image);

		//�����Ƀf�[�^���R�s�[
		for(i=0;i<(LONG)*height;i++){
			for(j=0;j<(LONG)*width;j++){
				if(g_config.IsHalfMode()){//���������R�s�[���Ȃ�
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
		// �P�z�p�z����
		BOOL ret = FALSE;

		if (retbit)
		{
			if (Image)
			{
				DWORD halfcopy=0;

				// ������Ԃ�
				for(i=0;i<(LONG)*height;i++){
					for(j=0;j<(LONG)*width;j++){
						if(g_config.IsHalfMode()){//���������R�s�[���Ȃ�
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

	//����
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
*	@brief �w��t�@�C������JPEG�t�@�C������A�摜�r�b�g��ǂݍ���
*
*	CreateSurfaceFrom256BMP�̓����֐��ALoad256BitmapBits��JPEG�ŁB
*	Load256BitmapBits�Ɠ������Apbits��free�Ŏn�����Ă��������B
*	���̊֐��ƈႢ�A�p���b�g���g�킸�t���J���[�œǂݍ��݂܂��B�����F�͂Ȃ��B
*
*	@param pbits [out] �ǂݍ��݌���. ���łɃp���b�g(ARGB=8888)�̔z��ɕϊ�����Ă܂�
*	@param width [out] �r�b�g�}�b�v�̕�
*	@param height [out] �r�b�g�}�b�v�̍���
*	@param jpegfilename [in] �r�b�g�}�b�v�̃t�@�C����
*	@return TRUE:����, FALSE:�c�O
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

	// ������
	ZeroMemory(&cinfo, sizeof(cinfo));
	ZeroMemory(&jerr, sizeof(jerr));
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	// �t�@�C���ǂݍ���
	fp = fopen(jpegfilename, "rb");
	if (!fp)
	{
		jpeg_destroy_decompress(&cinfo);
		return FALSE;
	}
	// �V�O�l�`�����
	WORD startsig = 0;
	long pos = ftell(fp);
	int sigr = fread(&startsig, sizeof(WORD), 1, fp);
	if ((sigr != 1) || (startsig != 0xD8FF))
	{
		// �Ⴄ����
		jpeg_destroy_decompress(&cinfo);
		fclose(fp);
		return FALSE;
	}
	fseek(fp, pos, SEEK_SET);

	jpeg_stdio_src(&cinfo, fp);

	// �w�b�_�ǂݍ���
	jpeg_read_header(&cinfo, TRUE);
	if (!cinfo.saw_JFIF_marker)
	{
		ODS("CDirectDraw::LoadJPEGbits : JFIF�}�[�J�[���Ȃ��B\n");
		jpeg_destroy_decompress(&cinfo);
		fclose(fp);
		return FALSE;
	}

	// �W�J�J�n
	jpeg_start_decompress(&cinfo);

	// ���Ƃ�����
	*width = cinfo.image_width;
	*height = cinfo.image_height;

	if (cinfo.out_color_components != 3)
	{
		ODS("CDirectDraw::LoadJPEGbits : �Ή����ĂȂ��`�����g���Ă���B\n");
		jpeg_destroy_decompress(&cinfo);
		fclose(fp);
		return FALSE;
	}

	//�r�b�g�}�b�v�̑傫���ɂ��킹�ĐV���Ƀ������̈���쐬����
	JSAMPROW Image;			// ��s����
	LPVOID pnewbits2;
	DWORD ishalf=1;
	if(g_config.IsHalfMode())ishalf=2;
	pnewbits2 = malloc(sizeof(MYPALLET)* *width * *height / ishalf);
	MYPALLET* retbit = (MYPALLET*)pnewbits2;

	DWORD halfcopy=0;

	// �Q�������I�z��
	Image = (JSAMPROW)malloc(3 * *width);

	//�����Ƀf�[�^���R�s�[
	for(i=0;i<(LONG)*height;i++){
		jpeg_read_scanlines(&cinfo, &Image, 1);		// ���̂�����Ȕ��[�ȂƂ���œǂݍ���
		for(j=0;j<(LONG)*width;j++){
			if(g_config.IsHalfMode()){//���������R�s�[���Ȃ�
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

	// ����
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
*	@brief �w��t�@�C�����̃r�b�g�}�b�v����A�p���b�g������ǂݍ���
*
*	CreateSurfaceFrom256BMP�̓����ŁALoad256Bitmapbits���痘�p�����B
*	���l�̐ݒ�̓r�b�g�}�b�v�ɂ͖{���Ȃ����̂Ȃ̂ŁA
*	RGB�̐������S��0�̂��̂�255�Ԗڂ̃p���b�g�� ��=0
*	����ȊO�̃p���b�g�� ��=255 �ɐݒ肳��ĕԂ���܂��B
*
*	@param filename [in] �t�@�C����
*	@param pal [out] �ǂݍ��݌��ʂ�ۑ����邽�߂̃p���b�g�z��i256�K�v�j
*	@return TRUE:����, FALSE:�c�O
*/
BOOL CDirectDraw::GetPallet(char *filename,MYPALLET *pal)
{
	HANDLE hFile;
	DWORD ret,br,err=FALSE;

	if(filename==NULL){
		ODS("CDirectDraw::GetPallet : �t�@�C������NULL�Ƃ͂ǂ��������Ƃ��H\n");
		return(FALSE);
	}
	if(pal==NULL){
		ODS("CDirectDraw::GetPallet : �p���b�g��NULL�Ƃ͂ǂ��������Ƃ��H\n");
		return(FALSE);
	}

	//�t�@�C���̃I�[�v��
	hFile=CreateFile(filename,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		return(NULL);
	}
	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//�O�̂��߃t�@�C���̐擪�Ɉړ�

	//̧�كw�b�_�̓ǂݏo��
	BITMAPFILEHEADER fileheader;
	ret=ReadFile(hFile,&fileheader,sizeof(BITMAPFILEHEADER),&br,NULL);
	if(!ret || br!=sizeof(BITMAPFILEHEADER)){
		ODS("CDirectDraw::GetPallet : �t�@�C���̓ǂݍ��݂Ɏ��s(1)\n");
		err=TRUE;
	}
	if(fileheader.bfType != 0x4d42){//"BM"
		ODS("CDirectDraw::GetPallet : �[�����̃t�@�C���̓r�b�g�}�b�v�ł͂Ȃ�\n");
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
		ODS("CDirectDraw::GetPallet : �t�@�C���̓ǂݍ��݂Ɏ��s(2)\n");
		err=TRUE;
	}
	if(infohed.biSize != sizeof(BITMAPINFOHEADER)){
		ODS("CDirectDraw::GetPallet : BITMAPINFOHEADER�̃T�C�Y������Ȃ�\n");
		err=TRUE;
	}
	if(infohed.biBitCount != 8){
		ODS("CDirectDraw::GetPallet : �[�����̃t�@�C����256�ł͂Ȃ�\n");
		err=TRUE;
	}
	if(infohed.biCompression != BI_RGB){
		ODS("CDirectDraw::GetPallet : ���k���������Ă���炵��\n");
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
		ODS("CDirectDraw::GetPallet : �t�@�C���̓ǂݍ��݂Ɏ��s(3)\n");
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
	}

	return(TRUE);
}

/*!
*	@brief �w��t�@�C������PNG�t�@�C������A�p���b�g������ǂݍ���
*
*	GetPallet��PNG�ŁB
*	BMP�ƈႢ�A���l�̓t�@�C�����烍�[�h���܂��B
*
*	@param filename [in] �t�@�C����
*	@param pal [out] �ǂݍ��݌��ʂ�ۑ����邽�߂̃p���b�g�z��i256�K�v�j
*	@return TRUE:����, FALSE:�c�O
*	@sa GetPallet 
*/
BOOL CDirectDraw::GetPalletPNG(char *filename,MYPALLET *pal)
{
	FILE* fp;

	if(filename==NULL){
		ODS("CDirectDraw::GetPalletPNG : �t�@�C������NULL�Ƃ͂ǂ��������Ƃ��H\n");
		return(FALSE);
	}
	if(pal==NULL){
		ODS("CDirectDraw::GetPalletPNG : �p���b�g��NULL�Ƃ͂ǂ��������Ƃ��H\n");
		return(FALSE);
	}

	// PNG�\���́H
	png_structp strPNG = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!strPNG)
	{
		ODS("CDirectDraw::GetPalletPNG : PNG�\�����޷�ȴ�!!�R(`�D�L)ɳܧ��!!\n");
		return FALSE;
	}

	// ���\���́c���ĉ�����B
	png_infop infoPNG = png_create_info_struct(strPNG);
	if (!infoPNG)
	{
		ODS("CDirectDraw::GetPalletPNG : PNG���\�����޷�ȴ�!!�R(`�D�L)ɳܧ��!!\n");
		png_destroy_read_struct(&strPNG, NULL, NULL);
		return FALSE;
	}
	
	// �ǂݍ��ނ��B
	fp = fopen(filename, "rb");
	if (!fp)
	{
		png_destroy_read_struct(&strPNG, &infoPNG, NULL);
		return FALSE;
	}

	// ���C�u�����ɔC����
	png_init_io(strPNG, fp);

	// �V�O�l�`�����
	BYTE sig[8];
	long pos = ftell(fp);

	int len = fread(sig, sizeof(BYTE), 8, fp);
	if ( png_sig_cmp(sig, 0, len) != 0 )
	{
		//ODS("CDirectDraw::GetPalletPNG : �[�����̃t�@�C����PNG�ł͂Ȃ��B\n");
		fclose(fp);
		png_destroy_read_struct(&strPNG, &infoPNG, NULL);
		return FALSE;
	}

	// �擪�ɖ߂�
	fseek(fp, pos, SEEK_SET);

	// PNG���Q�b�c
	png_read_info(strPNG, infoPNG);

	// �A���h �^�[��
	int bit_depth = 0, color_type = 0;

	bit_depth = png_get_bit_depth(strPNG, infoPNG);
	color_type = png_get_color_type(strPNG, infoPNG);

	// �p���b�g�t����256�F����Ȃ��Ƃ��߂�
	if (color_type != 3)
	{
		ODS("CDirectDraw::GetPalletPNG : �p���b�g���Ȃ���B\n");
		fclose(fp);
		png_destroy_read_struct(&strPNG, &infoPNG, NULL);
		return FALSE;
	}
	if (bit_depth != 8)
	{
		ODS("CDirectDraw::GetPalletPNG : �[�����̃t�@�C����256�ł͂Ȃ��B\n");
		fclose(fp);
		png_destroy_read_struct(&strPNG, &infoPNG, NULL);
		return FALSE;
	}

	// ���悢��p���b�g����
	png_colorp Palette;
	int num_palette = 0;

	png_get_PLTE(strPNG, infoPNG, &Palette, &num_palette);

	// ���l����
	png_bytep trans;
	int num_trans = 0;

	png_get_tRNS(strPNG, infoPNG, &trans, &num_trans, NULL);

	// �ݒ趲��
	for (int i = 0; i < 256; i++)
	{
		if (i < num_trans)
			pal[i].alpha = trans[i];//�����x�ݒ�
		else
			pal[i].alpha = 0xFF;//�ݒ肪�Ȃ��̂ŕs����

		if (i < num_palette)	// �F�ݒ�
		{
			pal[i].red = Palette[i].red;
			pal[i].green = Palette[i].green;
			pal[i].blue = Palette[i].blue;
		}
		else
			pal[i].red = pal[i].green = pal[i].blue = 0;	// ����
	}

	// �I������
	fclose(fp);
	png_destroy_read_struct(&strPNG, &infoPNG, NULL);

	return TRUE;
}



/*!
*	���[�h���ꂽ�Ǝ��`��2D�T�[�t�F�C�X���J��
*
*	@param s �J������T�[�t�F�C�X
*/
void CDirectDraw::RelSurface(MYSURFACE *s)
{
	if(s==NULL)return;

	if(s < &ms[0] || s > &ms[MAXNUMGOLUAHTEXTURES-1])
	{
		#ifndef GCD_EDITER
		g_system.Log("CDirectDraw::RelSurface �m�ۂ��ĂȂ��T�[�t�F�C�X�̊J���H",SYSLOG_WARNING);
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
	

	
	2D�`��E�}�g���N�X�֘A



====================================================================================*/


/*----------------------------------------------------------------------------------
	�Ǝ��`��2D�T�[�t�F�C�X�`��
------------------------------------------------------------------------------------*/

/*!
*	@brief CheckBlt2 �����ȗ��ȈՔ�
*
*	@param dds	�]�����T�[�t�F�C�X
*	@param x	�]����X���W
*	@param y	�]����Y���W
*	@param r	�]������`
*	@param revx X���]
*	@param revy Y���]
*	@param flag ���̑�����t���O
*	@param z	�]����Z���W
*	@param color 32�r�b�g�`�����_�J���[�B�ʏ� 0xFFFFFFFF
*
*	@sa CheckBlt2
*/
void CDirectDraw::CheckBlt(MYSURFACE *dds,int x,int y,RECT r,BOOL revx,BOOL revy,DWORD flag,float z,DWORD color, BOOL drawShadow)
{
	CheckBlt2(dds,x,y,r,1.0,1.0,revx,revy,flag,z,color,drawShadow);
}

/*!
*	@brief �T�[�t�F�[�X�̋�`�]��(�`��)
*
*	�]����͒ʏ�A��Ƀo�b�N�o�b�t�@�ł��B�i�`��^�[�Q�b�g�ɂ��j
*	DirectDraw�̍���Blt�֐�������Ȃ�ɓ��P���āA���񂾂�������������茸�����肵�č��Ɏ���B
*	�Ȃ񂩂���Ȋ����̗R���������֐��ł��B��낵���˂��I
*
*	@param dds	�]�����T�[�t�F�C�X
*	@param x	�]����X���W
*	@param y	�]����Y���W
*	@param r	�]������`
*	@param magx X�g�嗦
*	@param magy Y�g�嗦
*	@param revx X���]
*	@param revy Y���]
*	@param flag ���̑�����t���O
*	@param z	�]����Z���W
*	@param color 32�r�b�g�`�����_�J���[�B�ʏ� 0xFFFFFFFF
*/
void CDirectDraw::CheckBlt2(MYSURFACE *dds,int x,int y,RECT r,
			   double magx,double magy,BOOL revx,BOOL revy,DWORD flag,float z,DWORD color, BOOL drawShadow)
{
	DWORD i,j;
	float cut_left,cut_right,cut_top,cut_bottom;//�؂�Ă钷��(�s�N�Z���P��)
	float transx,transy;//�ꎞ�I�Ɏg�p

	//if(dds==NULL){return;}
	if(dds < &ms[0] || dds > &ms[MAXNUMGOLUAHTEXTURES-1])return;

	#ifdef AKIDX_DEBUG
	if(magx==0 || magy==0){
		ODS("��CDirectDraw::CheckBlt2() Warning-magx or magy ==0\n");
	}
	#endif
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

	DWORD r_top,r_bottom,r_left,r_right;//warning���E�U�C
	r_top=r.top;
	r_bottom=r.bottom;
	r_left=r.left;
	r_right=r.right;

	/*if (devtypenow != D3DDEVTYPE_HAL)
	{
		// �y�ʉ�ver�i���Ȃ�K���A�[���������B�j
		MyBlt3DLite(dds, r, x, y, FALSE);
	}
	else*/
	{
		//�S�Ẵe�N�X�`���Ɋւ��ĕ`�悷�邩�ǂ������ׂĕ`�悷��
		float vl,vr,vt,vb;//�e���_�̍��W
		float tumin,tumax,tvmin,tvmax;//u,v���W�͈̔�
		float ar = 640.0f/480.0f;//�A�X�y�N�g��
		float ar2 = 2.0f/480.0f;
		float centerx = (float)640.0f/2.0f;//x������ʒ��S
		MYVERTEX3D* vrtxarr;//���_�z��
		D3DXMATRIX matw;//���[���h���W�ϊ��s��
		D3DXMATRIX tmpmat;//�e���|�����s��
		D3DXMATRIX matpres;//�v���Z�b�g�ϊ��s��

		// �v���Z�b�g����
		D3DXMatrixIdentity(&matpres);
		//���]����
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
		//�g��k��
		D3DXMatrixScaling(&tmpmat,(float)magx,(float)magy,1.0f);
		matpres *= tmpmat;
		//�ړ�(���W�̒P�ʂ�float�ϊ���̂��́j
		D3DXMatrixTranslation(&tmpmat,(float)x/HALF_HEIGHT,(float)y/HALF_HEIGHT,/*z*/0);
		matpres *= tmpmat;
		if(flag & CKBLT_YUREY){//�h��
			D3DXMatrixTranslation(&tmpmat,0,yurey*ar2,0);
			matpres *= tmpmat;
		}
		//�ݒ肳�ꂽ�e�̕ϊ��ƍ��킹��
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
					if ( !pMyVertex || FAILED(pMyVertex->Lock(0, 0, (BYTE**)&vrtxarr, D3DLOCK_DISCARD)) )
						return;

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

					if (pMyVertex) pMyVertex->Unlock();

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
					//�v���Z�b�g�̂��������
					matw *= matpres;

					/*RECT r2;

					r2.left = (LONG)cut_left;
					r2.top = (LONG)cut_top;
					r2.right = dds->xsufsize[i] - (LONG)cut_right;
					r2.bottom = dds->ysufsize[j] - (LONG)cut_bottom;*/

					// pSprite->DrawTransform(dds->pTex[j*dds->xsufnum+i], &r2, &matw, color);
					d3ddev->SetTransform(D3DTS_WORLD,&matw);//*�ݒ�*
					//(5) �`��
					d3ddev->SetTexture(0,dds->pTex[j*dds->xsufnum+i]);//�e�N�X�`���ݒ�
					d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSU,D3DTADDRESS_CLAMP);
					d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSV,D3DTADDRESS_CLAMP);
					d3ddev->SetStreamSource(0, pMyVertex, sizeof(MYVERTEX3D));
					d3ddev->SetVertexShader( FVF_3DVERTEX );//���_�̃t�H�[�}�b�g���w��
					d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);//�`��
				}
			}
		}
		// pSprite->End();
	}
}


/*!
*	@brief �Ǝ��`��2D�T�[�t�F�C�X�`��@�Q
*
*	�]���ʒu��3D��ԏ�Ŏw�肷�邱�Ƃ��ł���B�]����͒ʏ�A��Ƀo�b�N�o�b�t�@�i�`��^�[�Q�b�g�j�ł��B
*	CheckBlt2�������������Ȃ����ǁAMYRECT3D�ɂ��낢�날���Ă������̂ق������R�x�������B�����B
*
*	@param dds �]�����T�[�t�F�C�X
*	@param src �]������`
*	@param dst �]�����`
*	@param flag ���̑�����t���O
*	@param color 32�r�b�g�`�����_�J���[�B�ʏ� 0xFFFFFFFF
*/
void CDirectDraw::MyBlt3D(MYSURFACE *dds,RECT src,MYRECT3D dst,DWORD flag,DWORD color)
{
	float cut_left,cut_right,cut_top,cut_bottom;//�؂�Ă钷��(�s�N�Z���P��)
	float transx,transy,sclx,scly;//�ꎞ�I�Ɏg�p

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

	if(g_config.IsHalfMode()){
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

	/*if (devtypenow != D3DDEVTYPE_HAL)
	{
		// �y�ʉ�ver�i���Ȃ�K���A�[���������B�j
		MyBlt3DLite(dds, src, (int)(dst.left * HALF_HEIGHT), (int)(dst.top * HALF_HEIGHT), TRUE);
	}
	else*/
	{
		//�S�Ẵe�N�X�`���Ɋւ��ĕ`�悷�邩�ǂ������ׂĕ`�悷��
		float vl,vr,vt,vb;//�e���_�̍��W
		float tumin,tumax,tvmin,tvmax;//u,v���W�͈̔�
		float ar = 640.0f/480.0f;//�A�X�y�N�g��
		float ar2 = 2.0f/480.0f;
		float centerx = 640.0f/2.0f;//x������ʒ��S
		MYVERTEX3D* vrtxarr;//���_�z��
		D3DXMATRIX matw;//���[���h���W�ϊ��s��
		D3DXMATRIX tmpmat;//�e���|�����s��
		D3DXMATRIX matpres;//�v���Z�b�g�ϊ��s��

		// �v���Z�b�g����
		D3DXMatrixIdentity(&matpres);

		//(0,0)-(1,1)�͈̔͂ɓ���悤�ɏk��
		sclx = 1.0f / (float)(r_right - r_left);
		scly = 1.0f / (float)(r_bottom - r_top);
		D3DXMatrixScaling(&tmpmat,sclx,scly,1.0f);
		matpres *= tmpmat;
		
		//�w�肳�ꂽMYRECT3D�܂Ŋg��&�ړ�
		sclx=dst.right/* * HALF_HEIGHT*/-dst.left/* * HALF_HEIGHT*/;
		scly=dst.bottom/* * HALF_HEIGHT*/-dst.top/* * HALF_HEIGHT*/;
		D3DXMatrixScaling(&tmpmat,sclx,scly,1.0f);
		matpres *= tmpmat;
		
		//�ړ�
		D3DXMatrixTranslation(&tmpmat,dst.left/* * HALF_HEIGHT*/,dst.top/* * HALF_HEIGHT*/,/*dst.z*/0);
		matpres *= tmpmat;
		if(flag & CKBLT_YUREY){//�h��
			D3DXMatrixTranslation(&tmpmat,0,yurey*ar2,0);
			matpres *= tmpmat;
		}
		//�ݒ肳�ꂽ�e�̕ϊ��ƍ��킹��
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
					//(0) ���E�㉺�A���ꂼ��]�����Ȃ������v�Z(src��`��)
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

					//(2) �]�����W�̌v�Z
					//���ƍ����������i�s�N�Z����float�ϊ��ύ��W�n�j
					//��
					vl=0;
					vr=dds->xsufsize[i] - (cut_left+cut_right);
					//����
					vt=0;
					vb=dds->ysufsize[j] - (cut_top+cut_bottom);
					//(3) ���_�z��ɍ��W�l����
					if ( !pMyVertex || FAILED(pMyVertex->Lock(0, 0, (BYTE**)&vrtxarr, D3DLOCK_DISCARD)) )
						return;

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

					if (pMyVertex) pMyVertex->Unlock();

					//(4) ���[���h���W�ϊ��s��p��
					//�P�ʍs��
					D3DXMatrixIdentity(&matw);

					//��`���ϊ�
					if(r_left>dds->xsufindx[i])transx=0;
					else transx = (float)( dds->xsufindx[i] - r_left );//pixel�P��
					if(r_top>dds->ysufindx[j])transy=0;
					else transy = (float)( dds->ysufindx[j] - r_top );//pixel�P��
					D3DXMatrixTranslation(&tmpmat,transx,transy,0);
					matw *= tmpmat;

					//�v���Z�b�g�̂��������
					matw *= matpres;

					/*RECT r2;

					r2.left = (LONG)cut_left;
					r2.top = (LONG)cut_top;
					r2.right = dds->xsufsize[i] - (LONG)cut_right;
					r2.bottom = dds->ysufsize[j] - (LONG)cut_bottom;*/

					// pSprite->DrawTransform(dds->pTex[j*dds->xsufnum+i], &r2, &matw, color);
					d3ddev->SetTransform(D3DTS_WORLD,&matw);//*�ݒ�*
					//(5) �`��
					d3ddev->SetTexture(0,dds->pTex[j*dds->xsufnum+i]);//�e�N�X�`���ݒ�
					d3ddev->SetStreamSource(0, pMyVertex, sizeof(MYVERTEX3D));
					d3ddev->SetVertexShader( FVF_3DVERTEX );//���_�̃t�H�[�}�b�g���w��
					d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSU,D3DTADDRESS_CLAMP);
					d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSV,D3DTADDRESS_CLAMP);
					d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);//�`��
				}
			}
		}
		// pSprite->End();
	}
}

/*!
*	@brief Goluah GCD(�o�[�W������������)�`���Z���`��
*
*	GCD�̂����A�w��C���f�b�N�X��CELL��`�悵�܂��B
*/
void CDirectDraw::CellDraw(MYSURFACE **pbuf,//!< GCD�ŗ��p����r�b�g�}�b�v�z��
						   LPVOID pcdat,	//!< GCD,CELL (GCD_CELL2)�z��
						   LPVOID prdat,	//!< GCD,�؂����` (GCD_RECT)�z��
						   DWORD cn,		//!< �`�悷��Z���ԍ�
						   int x,			//!< �`���x
						   int y,			//!< �`���y
						   float z,			//!< �`���z
						   int rot,			//!< �S�̉�]�p
						   BOOL revx,		//!< �S�̔��]X
						   BOOL revy,		//!< �S�̔��]Y
						   DWORD color,		//!< �S�̐F
						   float magx,		//!< �S�̊g�嗦X
						   float magy,		//!< �S�̊g�嗦Y
						   BOOL shadowed)	//!<�e�t��
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
*	@brief Goluah GCD(090)�`���Z���`��
*
*	CellDraw070��CellDraw090�́A�����ł̍��W�ϊ��̏��Ԃ������ɓ���ւ���Ă���̂Œ���
*/
void CDirectDraw::CellDraw090(MYSURFACE **pbuf,//!< GCD�ŗ��p����r�b�g�}�b�v�z��
						   LPVOID pcdat,	//!< GCD,CELL (GCD_CELL2_090)�z��
						   LPVOID prdat,	//!< GCD,�؂����` (GCD_RECT_090)�z��
						   DWORD cn,		//!< �`�悷��Z���ԍ�
						   int x,			//!< �`���x
						   int y,			//!< �`���y
						   float z,			//!< �`���z
						   int rot,			//!< �S�̉�]�p
						   BOOL revx,		//!< �S�̔��]X
						   BOOL revy,		//!< �S�̔��]Y
						   DWORD color,		//!< �S�̐F
						   float magx,		//!< �S�̊g�嗦X
						   float magy,		//!< �S�̊g�嗦Y
						   BOOL shadowed)	//!<�e�t��
{
	if(pbuf==NULL || pcdat==NULL || prdat==NULL)return;
	if(cn >= GCDMAX_CELLS)return;

	#ifdef AKIDX_DEBUG
	if(magx==0 || magy==0){
		ODS("��CDirectDraw::CheckBlt2() Warning-magx or magy ==0\n");
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
				0,z,color);//�`��
		}
	}
	else
	{
		D3DXMATRIX matp,mat,tmt,matprv,matprv2;
		float ar2 = 2.0f/480.0f;;

		//ZW/ZT�t���O����
		if((cdat[cn].flag & GCDCELL2_DISABLE_ZT) || (cdat[cn].flag & GCDCELL2_DISABLE_ZW))
		{
			BOOL zt = (cdat[cn].flag & GCDCELL2_DISABLE_ZT);
			BOOL zw = (cdat[cn].flag & GCDCELL2_DISABLE_ZW);
			EnableZ(zt,zw);
		}

		//�L�����N�^�[�̕ϊ��s��
		D3DXMatrixIdentity(&matp);
		if(!(cdat[cn].flag & GCDCELL2_SCA_GCENTER))
		{	
			if(cdat[cn].flag & GCDCELL2_ROT_BASEPOINT)
			{
				//�X�P�[���F�������S ��]�F�������S
				D3DXMatrixScaling(&tmt,magx,magy,1.0f);//�g��
				matp *= tmt;
				D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//��]
				matp *= tmt;
				D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2*(-1.0f),(float)(cdat[cn].gcy)*ar2*(-1.0f),0);//�d�S�Ɉړ�
				matp *= tmt;
			}
			else{
				//�X�P�[���F�������S ��]�F�d�S���S
				D3DXMatrixScaling(&tmt,magx,magy,1.0f);//�g��
				matp *= tmt;
				D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2*(-1.0f),(float)(cdat[cn].gcy)*ar2*(-1.0f),0);//�d�S�Ɉړ�
				matp *= tmt;
				D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//��]
				matp *= tmt;
			}
		}
		else
		{
			if(cdat[cn].flag & GCDCELL2_ROT_BASEPOINT)
			{
				//�X�P�[���F�d�S���S ��]�F�������S
				D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//��]
				matp *= tmt;
				D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2*(-1.0f),(float)(cdat[cn].gcy)*ar2*(-1.0f),0);//�d�S�Ɉړ�
				matp *= tmt;
				D3DXMatrixScaling(&tmt,magx,magy,1.0f);//�g��
				matp *= tmt;
			}
			else{
				//�X�P�[���F�d�S���S ��]�F�d�S���S
				D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2*(-1.0f),(float)(cdat[cn].gcy)*ar2*(-1.0f),0);//�d�S�Ɉړ�
				matp *= tmt;
				D3DXMatrixScaling(&tmt,magx,magy,1.0f);//�g��
				matp *= tmt;
				D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//��]
				matp *= tmt;
			}
		}
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

		matprv = SetParentMatrix(matp,FALSE,TRUE);//�u�e�v�̕ϊ��s��Ƃ��Đݒ�

		DWORD rn;
		BOOL c1revx,c1revy;

		for(int i=0;i<8;i++){
			rn = cdat[cn].cell[i].cdr;
			if(rn < GCDMAX_RECTANGLES && rn!=0 && rdat[rn].bmpno<GCDMAX_IMAGES){
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

				//�F��ύX(090���L)
				DWORD rcolor_a = (color&0xFF000000)>>24;
				DWORD rcolor_r = (color&0x00FF0000)>>16;
				DWORD rcolor_g = (color&0x0000FF00)>>8;
				DWORD rcolor_b = (color&0x000000FF);
				rcolor_a = (DWORD)(rcolor_a*cdat[cn].cell[i].alpha_ratio);
				rcolor_r = (DWORD)(rcolor_r*cdat[cn].cell[i].red_ratio);
				rcolor_g = (DWORD)(rcolor_g*cdat[cn].cell[i].green_ratio);
				rcolor_b = (DWORD)(rcolor_b*cdat[cn].cell[i].blue_ratio);
				DWORD rcolor = (rcolor_a<<24) | (rcolor_r<<16) | (rcolor_g<<8) | rcolor_b;

				//�u�����f�B���O���[�h�ύX(090���L)
				if(cdat[cn].cell[i].flag & GCDCELL_BLEND_ADD){
					SetAlphaMode(GBLEND_KASAN);
				}

			/*	//�e�N�X�`���t�B���^�����O������
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
					0,z,rcolor);//�`��

				//�u�����f�B���O���[�h���X�g�A
				if(cdat[cn].cell[i].flag & GCDCELL_BLEND_ADD){
					SetAlphaMode(0);
				}

				// ���ł�����e���`����
				if (shadowed)
				{
					D3DXPLANE Plane = D3DXPLANE(0, -1, 0, -0.01);
					D3DXMATRIX matp;

					if (stencil_enable)
					{
						d3ddev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILCAPS_INCRSAT);	// �X�e���V���ɏ�������
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
						0,z,0xAA000000);//�e�`��

					d3ddev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILCAPS_KEEP);	// ����
					d3ddev->SetRenderState( D3DRS_COLORWRITEENABLE,  0xFF );
					d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
				}

				SetParentMatrix(matprv2,TRUE);
			}
		}

		SetParentMatrix(matprv,TRUE);//�u�e�v�̕ϊ��s������ɖ߂�
		EnableZ();

	/*	//�e�N�X�`���t�B���^����(?)�ɖ߂�
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
*	@brief Goluah GCD(070)�`���Z���`��
*
*	CellDraw070��CellDraw090�́A�����ł̍��W�ϊ��̏��Ԃ������ɓ���ւ���Ă���̂Œ���
*/
void CDirectDraw::CellDraw070(
						   MYSURFACE **pbuf,//!< GCD�ŗ��p����r�b�g�}�b�v�z��
						   LPVOID pcdat,	//!< GCD,CELL (GCD_CELL2_070)�z��
						   LPVOID prdat,	//!< GCD,�؂����` (GCD_RECT_070)�z��
						   DWORD cn,		//!< �`�悷��Z���ԍ�
						   int x,			//!< �`���x
						   int y,			//!< �`���y
						   float z,			//!< �`���z
						   int rot,			//!< �S�̉�]�p
						   BOOL revx,		//!< �S�̔��]X
						   BOOL revy,		//!< �S�̔��]Y
						   DWORD color,		//!< �S�̐F
						   float magx,		//!< �S�̊g�嗦X
						   float magy,		//!< �S�̊g�嗦Y
						   BOOL shadowed)	//!<�e�t��
{
	if(pbuf==NULL || pcdat==NULL || prdat==NULL)return;
	if(cn >= GCDMAX_CELLS)return;

	#ifdef AKIDX_DEBUG
	if(magx==0 || magy==0){
		ODS("��CDirectDraw::CheckBlt2() Warning-magx or magy ==0\n");
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
				0,z,color);//�`��
		}
	}
	else
	{
		D3DXMATRIX matp,mat,tmt,matprv,matprv2;
		float ar2 = 2.0f/480.0f;

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
			if(rn < GCDMAX_RECTANGLES && rn!=0 && rdat[rn].bmpno<GCDMAX_IMAGES){
				//�ϊ��s����v�Z
				D3DXMatrixIdentity(&mat);
				D3DXMatrixTranslation(&tmt,(float)(rdat[rn].center_x)*ar2*(-1.0f),(float)(rdat[rn].center_y)*ar2*(-1.0f),0);//�d�S�Ɉړ�
				mat *= tmt;
				D3DXMatrixScaling(&tmt,cdat[cn].cell[i].magx,cdat[cn].cell[i].magy,1.0f);//�g��
				mat *= tmt;
				D3DXMatrixRotationZ(&tmt,D3DXToRadian(cdat[cn].cell[i].rot));//��]
				mat *= tmt;
				D3DXMatrixTranslation(&tmt,(float)(rdat[rn].center_x*cdat[cn].cell[i].magx)*ar2,(float)(rdat[rn].center_y)*ar2,0);//�d�S�ɖ߂�
				mat *= tmt;
				D3DXMatrixTranslation(&tmt,(float)(cdat[cn].cell[i].dx)*ar2,(float)(cdat[cn].cell[i].dy)*ar2,0);//�\���ʒu�ւ̈ړ�
				mat *= tmt;

				matprv2 = SetParentMatrix(mat,FALSE,TRUE);

				c1revx = cdat[cn].cell[i].flag & GCDCELL_REVERSE_X;
				c1revy = cdat[cn].cell[i].flag & GCDCELL_REVERSE_Y;

		/*		//�e�N�X�`���t�B���^�����O������
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
					0,z,color);//�`��

				// ���ł�����e���`����
				if (shadowed)
				{
					D3DXPLANE Plane = D3DXPLANE(0, -1, 0, -0.01);
					D3DXMATRIX matp;

					if (stencil_enable)
					{
						d3ddev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILCAPS_INCRSAT);	// �X�e���V���ɏ�������
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
						0,z,0xAA000000);//�e�`��

					d3ddev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILCAPS_KEEP);	// ����
					d3ddev->SetRenderState( D3DRS_COLORWRITEENABLE,  0xFF );
					d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
				}

				SetParentMatrix(matprv2,TRUE);
			}
		}

		SetParentMatrix(matprv,TRUE);//�u�e�v�̕ϊ��s������ɖ߂�
		EnableZ();
	}

/*	//�e�N�X�`���[�t�B���^�����ɖ߂�
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
*	@brief ��`�G�f�B�^�p�Z���`��
*
*	CellDraw�ɁA���ݕҏW���̋�`�������\��������A�����N�Z�����t������
*	�����̂ڂ����肵�ĕ`�悷��@�\���ǉ�����Ă���B
*	����GCD(090)�`���̂��̂̂݃T�|�[�g�B
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
						   UINT ed_rect,			//!< �ҏW���̋�`�ԍ�
						   BOOL color_modulat,		//!< �����\�����s��
						   BOOL is_previous_cell,	//!< *�����̂ڂ��ĕ`�悳�ꂽ�Z�����ǂ���
						   BOOL is_next_cell,		//!< *"��"�����ŕ`�悳�ꂽ�Z�����ǂ���
						   BOOL is_edit_cell)		//!< *���ݕҏW���イ�̃Z�����ǂ���
{
	if(pbuf==NULL || pcdat==NULL || prdat==NULL)return;
	if(cn >= GCDMAX_CELLS)return;

	GCD_CELL2_090 *cdat=(GCD_CELL2_090*)pcdat;
	GCD_RECT_090  *rdat=(GCD_RECT_090*)prdat;

	//�O�Z���`��(ED���L)--------------------------------------------------------------------------------
	if(cn>0 && !is_next_cell && (cdat[cn-1].flag & GCDCELL2_LINK)){
		CellDrawED(pbuf,pcdat,prdat,cn-1,x,y,z,rot,revx,revy,color,magx,magy,ed_rect,color_modulat,TRUE,FALSE,FALSE);
	}//------------------------------------------------------------------------------------------------

	//�F����(ED���L)------------------------------------------------------------------------------------
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
	
	//ZW/ZT�t���O����
	if((cdat[cn].flag & GCDCELL2_DISABLE_ZT) || (cdat[cn].flag & GCDCELL2_DISABLE_ZW))
	{
		BOOL zt = (cdat[cn].flag & GCDCELL2_DISABLE_ZT);
		BOOL zw = (cdat[cn].flag & GCDCELL2_DISABLE_ZW);
		EnableZ(zt,zw);
	}

	//�L�����N�^�[�̕ϊ��s��
	D3DXMatrixIdentity(&matp);
	float gcx = (float)(cdat[cn].gcx)*ar2;
	float gcy = (float)(cdat[cn].gcy)*ar2;
	if(!(cdat[cn].flag & GCDCELL2_SCA_GCENTER))
	{	
		if(cdat[cn].flag & GCDCELL2_ROT_BASEPOINT)
		{
			//�X�P�[���F�������S ��]�F�������S
			D3DXMatrixScaling(&tmt,magx,magy,1.0f);//�g��
			gcx *= magx;
			gcy *= magy;
			matp *= tmt;
			D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//��]
			matp *= tmt;
			D3DXMatrixTranslation(&tmt,-gcx,-gcy,0);//�d�S�Ɉړ�
			matp *= tmt;
		}
		else{
			//�X�P�[���F�������S ��]�F�d�S���S
			D3DXMatrixScaling(&tmt,magx,magy,1.0f);//�g��
			gcx *= magx;
			gcy *= magy;
			matp *= tmt;
			D3DXMatrixTranslation(&tmt,-gcx,-gcy,0);//�d�S�Ɉړ�
			matp *= tmt;
			D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//��]
			matp *= tmt;
		}
	}
	else
	{
		if(cdat[cn].flag & GCDCELL2_ROT_BASEPOINT)
		{
			//�X�P�[���F�d�S���S ��]�F�������S
			D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//��]
			matp *= tmt;
			D3DXMatrixTranslation(&tmt,-gcx,-gcy,0);//�d�S�Ɉړ�
			matp *= tmt;
			D3DXMatrixScaling(&tmt,magx,magy,1.0f);//�g��
			matp *= tmt;
		}
		else{
			//�X�P�[���F�d�S���S ��]�F�d�S���S
			D3DXMatrixTranslation(&tmt,-gcx,-gcy,0);//�d�S�Ɉړ�
			matp *= tmt;
			D3DXMatrixScaling(&tmt,magx,magy,1.0f);//�g��
			matp *= tmt;
			D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//��]
			matp *= tmt;
		}
	}
	if(revy){
		d3dxplane_y.d=0;
		D3DXMatrixReflect(&tmt,&d3dxplane_y);//y���]
		matp *= tmt;
	}
	D3DXMatrixTranslation(&tmt,gcx,gcy,0);//�d�S�ɖ߂�
	matp *= tmt;
	if(revx){
		d3dxplane_x.d=0;
		D3DXMatrixReflect(&tmt,&d3dxplane_x);//x���]
		matp *= tmt;
	}
	D3DXMatrixTranslation(&tmt,(float)x*ar2,(float)y*ar2,0);//�\���ʒu�ւ̈ړ�
	matp *= tmt;

	matprv = SetParentMatrix(matp,FALSE,TRUE);//�u�e�v�̕ϊ��s��Ƃ��Đݒ�

	DWORD rn;
	BOOL c1revx,c1revy;

	for(int i=0;i<8;i++){
		rn = cdat[cn].cell[i].cdr;
		if(rn < GCDMAX_RECTANGLES && rn!=0 && rdat[rn].bmpno<GCDMAX_IMAGES){
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

			//�F�ύX(ED���L)--------------------------------------------------------------------
			if(color_modulat)
			{
				if(is_edit_cell){
					if(i==ed_rect)color = color1;
					else color = color2;
				}
				else color = color2;
			}
			//---------------------------------------------------------------------------------

			//�F��ύX(090���L)
			DWORD rcolor_a = (color&0xFF000000)>>24;
			DWORD rcolor_r = (color&0x00FF0000)>>16;
			DWORD rcolor_g = (color&0x0000FF00)>>8;
			DWORD rcolor_b = (color&0x000000FF);
			rcolor_a = (DWORD)(rcolor_a*cdat[cn].cell[i].alpha_ratio);
			rcolor_r = (DWORD)(rcolor_r*cdat[cn].cell[i].red_ratio);
			rcolor_g = (DWORD)(rcolor_g*cdat[cn].cell[i].green_ratio);
			rcolor_b = (DWORD)(rcolor_b*cdat[cn].cell[i].blue_ratio);
			DWORD rcolor = (rcolor_a<<24) | (rcolor_r<<16) | (rcolor_g<<8) | rcolor_b;

			//�u�����f�B���O���[�h�ύX(090���L)
			if(cdat[cn].cell[i].flag & GCDCELL_BLEND_ADD){
				SetAlphaMode(GBLEND_KASAN);
			}
			
			CheckBlt(pbuf[rdat[rn].bmpno],
				0,0,rdat[rn].r,
				c1revx,
				c1revy,
				0,z,rcolor);//�`��

			//�u�����f�B���O���[�h���X�g�A
			if(cdat[cn].cell[i].flag & GCDCELL_BLEND_ADD){
				SetAlphaMode(0);
			}

			SetParentMatrix(matprv2,TRUE);
		}
	}

	SetParentMatrix(matprv,TRUE);//�u�e�v�̕ϊ��s������ɖ߂�

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
*	@brief Goluah GCD�`�������蔻���`�`��
*
*	�����蔻��̎l�p�`��`�悷��B�������̎l�p�`�ɂ���ĕ`�悳���B
*	�����͊�{�I��CellDraw�Ƃ���Ȃ�����ł���
*/
void CDirectDraw::HanteiDraw(
						LPVOID pcdat,
						LPVOID phdat,	//!< GCD�����`(GCD_HANTEI)�z��
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

	int i;
	MYVERTEX3D vb[4];
	for(i=0;i<4;i++){
		vb[i].tu = vb[i].tv=0;
		vb[i].z = 0;
	}

	d3ddev->SetRenderState(D3DRS_ZENABLE,FALSE);
	d3ddev->SetTexture(0,NULL);
	d3ddev->SetTransform(D3DTS_WORLD,&matp);

	if(b_atr){//�����蔻��i���F�j�̕`��
		for(i=0;i<3;i++){
			//���W�l�Z�b�g
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
			//�`��
			d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
		}
	}

	if(b_kas){//�d�Ȃ蔻��i�����j�̕`��
		for(i=0;i<3;i++){
			//���W�l�Z�b�g
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
			//�`��
			d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
		}
	}

	if(b_atk){//�U������i�����j�̕`��
		for(i=0;i<3;i++){
			//���W�l�Z�b�g
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
			//�`��
			d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
		}
	}

	d3ddev->SetRenderState(D3DRS_ZENABLE,TRUE);
}


/*!
*	@brief �P�����`��
*/
void CDirectDraw::DrawLine(
						   int sx,		//!< �n�_x 
						   int sy,		//!< �n�_y
						   int gx,		//!< �I�_x
						   int gy,		//!< �I�_y	
						   DWORD col	//!< �`��F
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
*	@brief GCD�����蔻���`�`��
*
*	���C���ɂ��`�悳���
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

	//�L�����N�^�[�̕ϊ��s��
	D3DXMatrixIdentity(&matp);
	D3DXMatrixTranslation(&tmt,0,0,0);//�d�S�Ɉړ�
	matp *= tmt;
	/*if (cdat[cn].flag & GCDCELL2_SCA_GCENTER)
	{
		D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2*(-1.0f),(float)(cdat[cn].gcy)*ar2*(-1.0f),0);//�d�S�Ɉړ�
		matp *= tmt;
	}*/
	D3DXMatrixScaling(&tmt,magx,magy,1.0f);//�g��
	matp *= tmt;
	/*if (cdat[cn].flag & GCDCELL2_SCA_GCENTER)
	{
		//�d�S����߂�
		D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx)*ar2,(float)(cdat[cn].gcy)*ar2,0);
		matp *= tmt;
	}*/
	D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx*magx)*ar2*(-1.0f),(float)(cdat[cn].gcy*magy)*ar2*(-1.0f),0);//�d�S�Ɉړ�
	matp *= tmt;
	D3DXMatrixRotationZ(&tmt,D3DXToRadian(rot));//��]
	matp *= tmt;
	//�d�S�ɖ߂�
	D3DXMatrixTranslation(&tmt,(float)(cdat[cn].gcx*magx)*ar2,(float)(cdat[cn].gcy*magy)*ar2,0);
	matp *= tmt;
	if(revy){
		d3dxplane_y.d=0;
		D3DXMatrixReflect(&tmt,&d3dxplane_y);//y���]
		matp *= tmt;
	}
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


/*!
*	@brief Goluah GCD�`���d�S�ʒu�}�[�J�[�`��
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
	D3DXMatrixTranslation(&tmt,(float)cdat[cn].gcx*ar2*magx+(x-cdat[cn].gcx)*ar2,(float)cdat[cn].gcy*ar2*magy+(y-cdat[cn].gcy)*ar2,0);
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


/*!
*	@brief �u�e�v�}�g���N�X��ݒ�
*
*	CDirectDraw�̂قƂ�ǂ̕`��֐��́A�����Őݒ肳�ꂽ�}�g���N�X�̉e�����󂯂�B
*	���̃}�g���N�X�́A���[�J�� �� ���[���h �� �J���� �� �r���[ �ϊ��̂����A
*	���[���h�����[�J�� �ϊ��ɑ}����������ł���B
*
*	�ʏ�͒P�ʍs�񂪎w�肳��Ă���B
*	�܂��A�ύX�����ꍇ�ɂ́A�`���Ɍ��̕ϊ��s��ɖ߂��Ă������ƁB
*
*	@param mat �V�K�ɐݒ肷��ϊ��}�g���N�X
*	@param root FALSE�̏ꍇ�A�ݒ肳���}�g���N�X�́A���}�g���N�X*�V�}�g���N�X�@�̊|���Z����������ł���
*	@param insert TRUE�̏ꍇ�A�ݒ肳���}�g���N�X�́A�V�}�g���N�X*���}�g���N�X�@�̊|���Z����������ł���
*	@return �֐��̎��s�O�ɐݒ肳��Ă����Â��}�g���N�X
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
*	@brief �u�e�v�}�g���N�X��ݒ�
*
*	SetParentMatrix�ƂȂɂ��������̂��ȁH�Y�ꂿ�Ⴂ�܂����B
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
*	@brief �u�e�v�}�g���N�X�ɒP�ʍs���ݒ肷��
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
*	@brief ���[���h���f�B�X�v���C���W�ϊ��}�g���N�X�ݒ�
*
*	�J�����ƃr���[�̍��W�ϊ��}�g���N�X�����Z�b�g����
*	D3DTS_VIEW �ƁAD3DTS_PROJECTION �̃}�g���N�X���ݒ肳���
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
*	@brief �J�����A�r���[�@�}�g���b�N�X�̐ݒ�
*
*	�����̂Ƃ��ɕ`��ʒu���L�����N�^�[�̈ʒu�ɂ���Ĉړ����邯�ǁA
*	������l�����ĕ`�悷�邩�ǂ����B
*	D3DTS_VIEW �ƁAD3DTS_PROJECTION ���e�����󂯂܂��B
*	@sa ResetTransformMatrix
*
*	@param b �J�����̈ʒu���l�����邩�ۂ��݂����Ȃ���
*			FALSE �̏ꍇ�Ax:0-640 , y:0-480 �̃f�B�X�v���C���W�n�Ŏw�肵��2D�`�悷��ƁA
*			���̂܂�܂ɕ\�������悤�ɂȂ�B�͂�
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
*	@brief ���[���h�ϊ��}�g���N�X���w��
*
*	D3DTS_WORLD�Ɏw��}�g���b�N�X��ݒ肵�܂��B
*	���Ԃ�A�uD3DTS_WORLD�v���ĒԂ���o���ĂȂ��������炱�̊֐������܂ꂽ�񂾂Ǝv����B
*/
void CDirectDraw::SetWorldMatrix(D3DXMATRIX *pmat)
{
	d3ddev->SetTransform(D3DTS_WORLD,pmat);
}




#define NUMCIRCLEDIVIDE	40//�u�~�v�������̑��p�`�ŕ\�����邩�B

/*!
*	@brief �~�̕`��
*/
void CDirectDraw::DrawCircle(int x,			//!< �~�̒��S�ʒux
							 int y,			//!< �~�̒��S�ʒuy
							 int rad,		//!< �~�̔��a
							 int w,			//!< ���̂ӂƂ�
							 float z,		//!< �~��z�ʒu
							 DWORD color,	//!< �~�̐F
							 BOOL toumei,	//!< ���̓����𓧖������邩
							 float rot,		//!< �~�̉�]�p
							 BOOL rot_y,	//!< TRUE:Y�������Ƃ��ĉ�]����,FALSE:X�������Ƃ��ĉ�]����
							 BOOL hosei		//!< �␳������邩�ǂ���(�Ȃ��?)
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
*	@brief �e�L�X�g�`��
*
*	�t�H���g���g���ĕ`�悵�܂��B���\�d�����ۂ��B
*	�F�̓u���[�Œ�炵���B�i�Ȃ�ŁH�j
*/
void CDirectDraw::DrawBlueText(
							RECT& r,		//!< �`�悷���`�̈�
							char *text,		//!< �e�L�X�g������
							int len,		//!< �e�L�X�g������
							DWORD method,	//!< Win32API��DrawText�Q�ƁBDT_LEFT�Ƃ�����������
							DWORD size)		//!< �`��T�C�Y�B1�`(AKIDX_FONTNUM-1)
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
		return;		// ���

	// alpha�l�ňÂ�����炵��
	d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
	d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);

	// �X�N���[���S�̂𕢂��|���S��
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

	// ���ɖ߂�
	EnableZ();
	SetAlphaMode(GBLEND_HANTOUMEI);
}



/*----------------------------------------------------------------------------------
	�t�����g�o�b�t�@�R�s�[
------------------------------------------------------------------------------------*/
#ifndef GCD_EDITER

extern RECT g_rcClient;

/*!
*	���݂̃t�����g�o�b�t�@�[�̓��e���R�s�[���ĕԂ��B
*	�e�N�X�`���[�͌Ăяo������Release���邱�ƁB
*	�E�B���h�E���[�h�̏ꍇ�A�ʂȃE�C���h�E���ז����Ă�Ƃ�����ʂ肱�ށB
*	�߂����e�N�X�`���[�̃t�H�[�}�b�g��A1R5G5B5�B
*
*	@return ���s������NULL
*/
LPDIRECT3DTEXTURE8 CDirectDraw::GetFrontBufferCopy()
{
	//��������e�N�X�`���[�T�C�Y������
	UINT tex_width = m_desktop_width;//g_DISPLAYWIDTH;
	UINT tex_height = m_desktop_height;//g_DISPLAYHEIGHT;

	//�R�s�[�p�e�N�X�`������
	LPDIRECT3DTEXTURE8 ret=NULL;
	if(D3D_OK!=d3ddev->CreateTexture(          
							tex_width,	//UINT Width,
							tex_height,	//UINT Height,
							1,			//UINT Levels(mipmap),
							0,			//DWORD Usage,
							D3DFMT_A8R8G8B8,	//D3DFORMAT Format,�R�s�[����Ƃ��͂��̃t�H�[�}�b�g����Ȃ���_���炵��
//							d3dpp.BackBufferFormat,	//D3DFORMAT Format,�Ђ���Ƃ����炱������������Ȃ��̂Ŏ����ɍ����ւ��Ă݂�
							D3DPOOL_SYSTEMMEM ,//D3DPOOL Pool,�R�s�[����Ƃ��̓V�X�e������������Ȃ���_���炵��
							&ret		//IDirect3DTexture9** ppTexture
							))
	{
		return NULL;
	}
	if(!ret)return NULL;

	//�e�N�X�`���[����T�[�t�F�[�X���擾����
	//���t�@�����X�J�E���g�������邩��ARelease���邱��
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

	//�R�s�[
	if(D3D_OK!=d3ddev->GetFrontBuffer(surface))//Data(0,surface))
	{
		ret->Release();
		surface->Release();
		return NULL;
	}

	//��U�ʃe�N�X�`���ɃR�s�[���ɂ�Ȃ��B�߂�ǂ����[
	LPDIRECT3DTEXTURE8 ret2=NULL;
	LPDIRECT3DSURFACE8 surface2 = NULL;
	do
	{
		//�E�B���h�E���[�h�̏ꍇ�A�f�X�N�g�b�v�S�̂̃R�s�[��
		//����Ă��܂��̂ŁA�I�t�Z�b�g����K�v������
		UINT offset_x = 0;
		UINT offset_y = 0;
		if(m_is_win)
		{
			offset_x = g_rcClient.left;
			offset_y = g_rcClient.top;
		}


		tex_width = g_DISPLAYWIDTH;
		tex_height = g_DISPLAYHEIGHT;
		//�R�s�[�p�e�N�X�`������
		
		/*if(D3D_OK!=d3ddev->CreateTexture(          
								tex_width,	//UINT Width,
								tex_height,	//UINT Height,
								1,			//UINT Levels(mipmap),
								0,			//DWORD Usage,
								D3DFMT_A8R8G8B8,	//D3DFORMAT Format,�R�s�[�����t�H�[�}�b�g�Ɠ�����
								D3DPOOL_MANAGED ,//D3DPOOL Pool,�`�悷��Ƃ��̓V�X�e������������_���炵��
								&ret2		//IDirect3DTexture9** ppTexture
								))*/
		if(D3D_OK!=d3ddev->CreateTexture(          
								tex_width,	//UINT Width,
								tex_height,	//UINT Height,
								1,			//UINT Levels(mipmap),
								0,			//DWORD Usage,
								D3DFMT_A1R5G5B5,	//D3DFORMAT Format,�R�s�[�����t�H�[�}�b�g�Ɠ�����
								D3DPOOL_MANAGED ,//D3DPOOL Pool,�`�悷��Ƃ��̓V�X�e������������_���炵��
								&ret2		//IDirect3DTexture9** ppTexture
								))
		{
			break;
		}

		//�e�N�X�`���[����T�[�t�F�[�X���擾����
		//���t�@�����X�J�E���g�������邩��ARelease���邱��
		if(D3D_OK!=ret2->GetSurfaceLevel(0,&surface2))
		{
			break;
		}

		//2�e�N�X�`�������b�N�B
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

		//�R�s�[�i1�s�N�Z����32�r�b�g�j
		for(UINT y=0;y<tex_height;y++)
		{
			//A8R8G8B8��A8R8G8B8
		/*	if(y+offset_y>=m_desktop_height)break;
			DWORD *src = (DWORD*)((BYTE*)lr.pBits + lr.Pitch*(y+offset_y) + offset_x*4);
			DWORD *dst = (DWORD*)((BYTE*)lr2.pBits + lr2.Pitch*y);
			for(UINT x=0;x<tex_width;x++)
			{
				if(x+offset_x>=m_desktop_width)break;
				(*dst) = (*src);
				*dst|=0xFF000000;//���łɃ��l���ő�ɂ��Ȃ��Ⴞ�߂݂����ł���H
				dst++;
				src++;
			}*/

			//A8R8G8B8��A1R5G5B5
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

		//�e�N�X�`���ă��b�N
		surface->UnlockRect();
		surface2->UnlockRect();
	}
	while(0);

	//�������`�H
	surface->Release();
	ret->Release();

	if(surface2)surface2->Release();
	return ret2;
}

/*!
*	���݂̃t�����g�o�b�t�@�[�̓��e���R�s�[���ĕԂ��B
*	�X�N���[���V���b�g�p�B
*	�߂��ꂽ�|�C���^�͌Ăяo������delete���邱�ƁB
*
*	@param wdt [out]�R�s�[�����C���[�W�̕�
*	@param hgt [out]�R�s�[�����C���[�W�̍���
*	@return 32�r�b�g�z��B���s��NULL�B
*/
DWORD* CDirectDraw::GetFrontBufferCopyRaw(UINT *wdt,UINT *hgt)
{
	//��������e�N�X�`���[�T�C�Y������
	UINT tex_width = m_desktop_width;//g_DISPLAYWIDTH;
	UINT tex_height = m_desktop_height;//g_DISPLAYHEIGHT;

	//�R�s�[�p�e�N�X�`������
	LPDIRECT3DTEXTURE8 ret=NULL;
	if(D3D_OK!=d3ddev->CreateTexture(          
							tex_width,	//UINT Width,
							tex_height,	//UINT Height,
							1,			//UINT Levels(mipmap),
							0,			//DWORD Usage,
							D3DFMT_A8R8G8B8,	//D3DFORMAT Format,�R�s�[����Ƃ��͂��̃t�H�[�}�b�g����Ȃ���_���炵��
//							d3dpp.BackBufferFormat,	//D3DFORMAT Format,�Ђ���Ƃ����炱������������Ȃ��̂Ŏ����ɍ����ւ��Ă݂�
							D3DPOOL_SYSTEMMEM ,//D3DPOOL Pool,�R�s�[����Ƃ��̓V�X�e������������Ȃ���_���炵��
							&ret		//IDirect3DTexture9** ppTexture
							))
	{
		return NULL;
	}
	if(!ret)return NULL;

	//�e�N�X�`���[����T�[�t�F�[�X���擾����
	//���t�@�����X�J�E���g�������邩��ARelease���邱��
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

	//�R�s�[
	if(D3D_OK!=d3ddev->GetFrontBuffer(surface))//Data(0,surface))
	{
		ret->Release();
		surface->Release();
		return NULL;
	}

	//�R�s�[
	DWORD *rret = NULL;
	BOOL fail = TRUE;
	do
	{
		//�E�B���h�E���[�h�̏ꍇ�A�f�X�N�g�b�v�S�̂̃R�s�[��
		//����Ă��܂��̂ŁA�I�t�Z�b�g����K�v������
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

		//�R�s�[�p�������m��
		rret = new DWORD [ tex_width*tex_height ];

		//2�e�N�X�`�������b�N�B
		D3DLOCKED_RECT lr ;
		if(D3D_OK != surface->LockRect(&lr,NULL,0))
		{
			break;
		}

		//�R�s�[�i1�s�N�Z����32�r�b�g�j
		for(UINT y=0;y<tex_height;y++)
		{
			//A8R8G8B8��DWORD
			if(y+offset_y>=m_desktop_height)break;
			DWORD *src = (DWORD*)((BYTE*)lr.pBits + lr.Pitch*(y+offset_y) + offset_x*4);
			DWORD *dst = (DWORD*)(rret + y*tex_width);
			for(UINT x=0;x<tex_width;x++)
			{
				if(x+offset_x>=m_desktop_width)break;
				(*dst) = (*src);
				*dst|=0x00000000;//���l�HFlag�H
				dst++;
				src++;
			}

		}

		//�e�N�X�`���ă��b�N
		surface->UnlockRect();

		fail = FALSE;
	}
	while(0);

	//�������`�H
	surface->Release();
	ret->Release();

	if(fail)DELETEARRAY(rret);
	return rret;
}

#endif