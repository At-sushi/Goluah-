
/*=============================================================================

    main.cpp

    エントリーポイント・メインウインドウの生成とメッセージハンドラ
    ウインドウメッセージ、メインループ処理をCSystemクラスに投げる

===============================================================================*/

//#pragma warning(disable:4786)
#include "stdafx.h"
#include "global.h"
//#include "script_loader.h"

#include "../resource.h"

// function ====================================================================

HWND CreateMyWindow(BOOL win);
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

// ★ entry point ***************************************************************
/*!
*	@ingroup Application
*	@brief Win32プロジェクトだったときの旧WinMain（旧じゃなくなりました）
*
*	事実上のエントリーポイントです。
*	現在はCgoluahAppのInitInstanceから呼ばれてますが、
*	この関数を抜けたらアプリケーション終了です。
*
*/
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
    #if TRUE
    if( CoInitializeEx(NULL, COINIT_MULTITHREADED) != S_OK ){
        if( CoInitializeEx(NULL, NULL) != S_OK ){
            MessageBox(NULL,"CoInitializeに失敗","？",MB_OK);
        }
    }
    #else
    if( CoInitialize(NULL) != S_OK ){
        MessageBox(NULL,"CoInitializeに失敗","？",MB_OK);
    }
    #endif

    if(g_config.IsHalfMode()){
        g_DISPLAYWIDTH=320;
        g_DISPLAYHEIGHT=240;
    }

    BOOL isWindowMode = ! g_config.IsFullScreen() ;

    //window生成
    HWND hwnd = CreateMyWindow(isWindowMode);
    gbl.SetWinCenter(hwnd);
    ShowWindow( hwnd, SW_SHOWDEFAULT );
    UpdateWindow( hwnd );

    //システムクラス初期化
    if(!g_system.Initialize(hwnd)){
        UnregisterClass( AppName , hInst );
        CoUninitialize();
        return(0);
    }

    timeBeginPeriod(1);

    //ゲームスタート・メインループ
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
*	@brief メインウインドウのウインドウメッセージハンドラ
*
*	ウインドウメッセージを処理。
*	ゲーム中でウインドウメッセージを拾いたい場合、CTaskBaseを継承したクラスであれば
*	WndMessageをオーバーライドすることによってここと同じメッセージが拾えます。
*/
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
    POINT pAdj;
    g_system.WndMessage(hWnd, msg, wparam, lparam);

    switch( msg )
    {
    case WM_KEYDOWN:
        if (wparam == VK_ESCAPE) {
            const auto taskID = g_system.GetCurrentMainTask()->GetID();
            if (taskID == 'titl' || taskID == 'debg')
                DestroyWindow(hWnd);
            else
                g_system.ReturnTitle();
        }
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
        // フルスクリーンモードの時はカーソルを消す
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
*	@brief メインウインドウの生成
*	@ingroup Application
*
*	フルスクリーンであってもウインドウは必要なので、ウインドウは作られます。
*	が、なんか忘れたけど事情があって、ウインドウスタイルを枠ナシに設定しなければ
*	いけなかった覚えが。
*	そのため、引数にウィンドウモードかどうかのフラグをとります
*
*	@param win TRUE:ウィンドウモード, FALSE:フルスクリーン
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

