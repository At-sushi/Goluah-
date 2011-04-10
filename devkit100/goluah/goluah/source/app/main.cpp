
/*=============================================================================

	main.cpp

	�G���g���[�|�C���g�E���C���E�C���h�E�̐����ƃ��b�Z�[�W�n���h��
	�E�C���h�E���b�Z�[�W�A���C�����[�v������CSystem�N���X�ɓ�����

===============================================================================*/

//#pragma warning(disable:4786)
#include "stdafx.h"
#include "global.h"
//#include "script_loader.h"

#include "../resource.h"

// function ====================================================================

HWND CreateMyWindow(BOOL win);
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

// �� entry point ***************************************************************
//INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )

/*!
*	@ingroup Application
*	@brief Win32�v���W�F�N�g�������Ƃ��̋�WinMain
*
*	������̃G���g���[�|�C���g�ł��B
*	���݂�CgoluahApp��InitInstance����Ă΂�Ă܂����A
*	���̊֐��𔲂�����A�v���P�[�V�����I���ł��B
*
*	@return �Ӗ��Ȃ��݂����B���0
*/
INT gOldEntryPoint(HINSTANCE hInst)
{
	#if TRUE
	if( CoInitializeEx(NULL, COINIT_MULTITHREADED) != S_OK ){
		if( CoInitializeEx(NULL, NULL) != S_OK ){
			MessageBox(NULL,"CoInitialize�Ɏ��s","�H",MB_OK);
		}
	}
	#else
	if( CoInitialize(NULL) != S_OK ){
		MessageBox(NULL,"CoInitialize�Ɏ��s","�H",MB_OK);
	}
	#endif

	if(g_config.IsHalfMode()){
		g_DISPLAYWIDTH=320;
		g_DISPLAYHEIGHT=240;
	}

	BOOL isWindowMode = ! g_config.IsFullScreen() ;

	//window����
	HWND hwnd = CreateMyWindow(isWindowMode);
	gbl.SetWinCenter(hwnd);
	ShowWindow( hwnd, SW_SHOWDEFAULT );
	UpdateWindow( hwnd );

	//�V�X�e���N���X������
	if(!g_system.Initialize(hwnd)){
		UnregisterClass( AppName , hInst );
		CoUninitialize();
		return(0);
	}

	timeBeginPeriod(1);

	//�Q�[���X�^�[�g�E���C�����[�v
	if(g_system.GameStart()){
		MSG msg;
		while(!g_programexit){
			if(PeekMessage( &msg, NULL, 0, 0 ,PM_REMOVE)){//message loop
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			g_system.MainLoop();
		}
	}

	timeEndPeriod(1);

	g_system.Destroy();
    UnregisterClass( AppName , hInst );
	CoUninitialize();

    return 0;

}


/*!
*	@brief ���C���E�C���h�E�̃E�C���h�E���b�Z�[�W�n���h��
*
*	�E�C���h�E���b�Z�[�W�������B
*	�Q�[�����ŃE�C���h�E���b�Z�[�W���E�������ꍇ�ACTaskBase���p�������N���X�ł����
*	WndMessage���I�[�o�[���C�h���邱�Ƃɂ���Ă����Ɠ������b�Z�[�W���E���܂��B
*/
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	POINT pAdj;
	g_system.WndMessage(hWnd, msg, wparam, lparam);

	switch( msg )
    {
	case WM_KEYDOWN:
		if(wparam==VK_ESCAPE)DestroyWindow(hWnd);
		break;

	case WM_SIZE:
    case WM_CREATE:
	case WM_MOVE:
		pAdj.x = pAdj.y =0 ;
		GetClientRect(hWnd,&g_rcClient);
		ClientToScreen(hWnd,&pAdj);
		g_rcClient.left += pAdj.x;
		g_rcClient.top += pAdj.y;
		g_rcClient.right += pAdj.x;
		g_rcClient.bottom += pAdj.y;
		break;

	case WM_SETCURSOR:
		// �t���X�N���[�����[�h�̎��̓J�[�\��������
		if (g_config.IsFullScreen())
			SetCursor(NULL);
		break;
	/*case WM_SIZE:
		if (wp == SIZE_RESTORED || wp == SIZE_MAXIMIZED)
			g_draw.OnWindowResized(LOWORD(lp), HIWORD(lp));
		break;*/

	case WM_CLOSE:
		DestroyWindow(hWnd);
		return(0);
	case WM_DESTROY:
		PostQuitMessage( 0 );
		g_programexit=TRUE;
		return 0;
	default:
		break;
    }
    return DefWindowProc( hWnd, msg, wparam, lparam );
}


/*!
*	@brief ���C���E�C���h�E�̐���
*	@ingroup Application
*
*	�t���X�N���[���ł����Ă��E�C���h�E�͕K�v�Ȃ̂ŁA�E�C���h�E�͍���܂��B
*	���A�Ȃ񂩖Y�ꂽ���ǎ�������āA�E�C���h�E�X�^�C����g�i�V�ɐݒ肵�Ȃ����
*	�����Ȃ������o�����B
*	���̂��߁A�����ɃE�B���h�E���[�h���ǂ����̃t���O���Ƃ�܂�
*
*	@param win TRUE:�E�B���h�E���[�h, FALSE:�t���X�N���[��
*/
HWND CreateMyWindow(BOOL win)
{
	// Register the window class
    WNDCLASSEX wc;
	wc.cbSize		= sizeof(WNDCLASSEX);
	wc.style		= CS_CLASSDC;
	wc.lpfnWndProc	= MsgProc;
	wc.cbClsExtra	= 0L;
	wc.cbWndExtra	= 0L; 
    wc.hInstance	= GetModuleHandle(NULL);
	wc.hIcon		= LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor		= NULL;
	wc.hbrBackground= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName	= NULL;
	wc.lpszClassName= AppName;
    wc.hIconSm		= NULL;
    RegisterClassEx( &wc );

	DWORD wstyle = WS_POPUP;
	DWORD wsp_x=0,wsp_y=0;
	if(win){
		wstyle=WS_BORDER | WS_CAPTION | WS_SYSMENU/* | WS_THICKFRAME*/ | WS_MINIMIZEBOX /*| WS_MAXIMIZEBOX */;
		wsp_x=GetSystemMetrics(SM_CXDLGFRAME) * 2;
		wsp_y=GetSystemMetrics(SM_CYDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);
	}

    // Create the application's window
    HWND hWnd = CreateWindow(
		AppName,
		AppName,
		wstyle ,
		0,
		0,
		g_DISPLAYWIDTH + wsp_x,
		g_DISPLAYHEIGHT + wsp_y,
		GetDesktopWindow(),
		NULL,
		wc.hInstance,
		NULL);

	return(hWnd);
}



/*!
*	@mainpage
*	"Goluah!!" system program , since 2001.10
*/

