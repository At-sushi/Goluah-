// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。

/*
Goluah!!Copyright(C) 2001 - 2004 aki, 2014 - 2015 logger, 2004 - 2015 At - sushi

This program is free software; you can redistribute it and / or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or(at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110 - 1301 USA.
*/

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
#include <atlstr.h>
#include <atlwin.h>

// さすがに標準APIは使わないといけない
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


#include <mmsystem.h>

#ifndef MEMLEAK_CK_DEF
#include "define_const.h"
#endif

//スクリーンショットで使うCImage用
//#define _ATL_NO_DEFAULT_LIBS//大丈夫かな･･･？//やっぱだめぽ?
//#include <atlimage.h>


// STLヘッダ読み込み
#include <typeinfo>
#include <algorithm>
#include <list>
#include <stack>
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>


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

#pragma warning( disable:4995 )
#pragma warning( disable:4996 )		// とりあえずsprintf_s警告を無視
