// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows ヘッダーから使用されていない部分を除外します。
#endif

// 下で指定された定義の前に対象プラットフォームを指定しなければならない場合、以下の定義を変更してください。
// 異なるプラットフォームに対応する値に関する最新情報については、MSDN を参照してください。
#ifndef WINVER				// Windows 95 および Windows NT 4 以降のバージョンに固有の機能の使用を許可します。
#if _MSC_VER >= 1500
#define WINVER 0x0500		// VC2008以降向け
#else
#define WINVER 0x0400		// これを Windows 98 および Windows 2000 またはそれ以降のバージョン向けに適切な値に変更してください。
#endif // _MSC_VER
#endif

#ifndef _WIN32_WINNT		// Windows NT 4 以降のバージョンに固有の機能の使用を許可します。
#if _MSC_VER >= 1500
#define _WIN32_WINNT 0x0500		// VC2008以降向け
#else
#define _WIN32_WINNT 0x0400		// これを Windows 98 および Windows 2000 またはそれ以降のバージョン向けに適切な値に変更してください。
#endif // _MSC_VER
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_WINDOWS 0x0410 // これを Windows Me またはそれ以降のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_IE			// IE 4.0 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_IE 0x0400	// これを IE 5.0  またはそれ以降のバージョン向けに適切な値に変更してください。
#endif

#define _WIN32_DCOM//? for CoInitializeEx

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 一部の CString コンストラクタは明示的です。

// 一般的で無視しても安全な MFC の警告メッセージの一部の非表示を解除します。
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分

#include <mmsystem.h>

#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT

#ifndef MEMLEAK_CK_DEF
#include "define_const.h"
#endif

//スクリーンショットで使うCImage用
//#define _ATL_NO_DEFAULT_LIBS//大丈夫かな･･･？//やっぱだめぽ?
//#include <atlimage.h>

#include <typeinfo>
#include <algorithm>


#pragma warning( disable : 4005 )
#define DIRECTINPUT_VERSION 0x0800//warning消し

#include <d3d8.h>
#include <d3dx8.h>
#include <dsound.h>
#include <dshow.h>
#include <dinput.h>
#include <InitGuid.h>		// DirectPlayがdxguid.libにもうないので暫定措置
#include <dplay8.h>

#include <stdio.h>
#include <string.h>
#include <time.h>

// デバッグ機能は重いので
#ifdef _DEBUG
#undef _DEBUG
#define DOBASHI
#endif // _DEBUG

#include <list>
#include <stack>
#include <vector>

// 元に戻しとく
#ifdef DOBASHI
#undef DOBASHI
#define _DEBUG
#endif // DOBASHI

#pragma warning( disable:4995 )
#pragma warning( disable:4996 )		// とりあえずsprintf_s警告を無視
