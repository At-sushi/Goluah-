// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���̃C���N���[�h �t�@�C���A�܂���
// �Q�Ɖ񐔂������A�����܂�ύX����Ȃ��A�v���W�F�N�g��p�̃C���N���[�h �t�@�C��
// ���L�q���܂��B

/*
Goluah!!Copyright(C) 2001 - 2004 aki, 2014 - 2015 logger, 2004 - 2015 At - sushi

This program is free software; you can redistribute it and / or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or(at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110 - 1301 USA.
*/

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows �w�b�_�[����g�p����Ă��Ȃ����������O���܂��B
#endif

// ���Ŏw�肳�ꂽ��`�̑O�ɑΏۃv���b�g�t�H�[�����w�肵�Ȃ���΂Ȃ�Ȃ��ꍇ�A�ȉ��̒�`��ύX���Ă��������B
// �قȂ�v���b�g�t�H�[���ɑΉ�����l�Ɋւ���ŐV���ɂ��ẮAMSDN ���Q�Ƃ��Ă��������B
#ifndef WINVER				// Windows 95 ����� Windows NT 4 �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
#if _MSC_VER >= 1500
#define WINVER 0x0500		// VC2008�ȍ~����
#else
#define WINVER 0x0400		// ����� Windows 98 ����� Windows 2000 �܂��͂���ȍ~�̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif // _MSC_VER
#endif

#ifndef _WIN32_WINNT		// Windows NT 4 �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
#if _MSC_VER >= 1500
#define _WIN32_WINNT 0x0500		// VC2008�ȍ~����
#else
#define _WIN32_WINNT 0x0400		// ����� Windows 98 ����� Windows 2000 �܂��͂���ȍ~�̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif // _MSC_VER
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
#define _WIN32_WINDOWS 0x0410 // ����� Windows Me �܂��͂���ȍ~�̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#ifndef _WIN32_IE			// IE 4.0 �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
#define _WIN32_IE 0x0400	// ����� IE 5.0  �܂��͂���ȍ~�̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#define _WIN32_DCOM//? for CoInitializeEx

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// �ꕔ�� CString �R���X�g���N�^�͖����I�ł��B
#include <atlstr.h>
#include <atlwin.h>

// �������ɕW��API�͎g��Ȃ��Ƃ����Ȃ�
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


#include <mmsystem.h>

#ifndef MEMLEAK_CK_DEF
#include "define_const.h"
#endif

//�X�N���[���V���b�g�Ŏg��CImage�p
//#define _ATL_NO_DEFAULT_LIBS//���v���ȥ���H//����ς��߂�?
//#include <atlimage.h>


// STL�w�b�_�ǂݍ���
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
#define DIRECTINPUT_VERSION 0x0800//warning����

#include <d3d8.h>
#include <d3dx8.h>
#include <dsound.h>
#include <dshow.h>
#include <dinput.h>
#include <InitGuid.h>		// DirectPlay��dxguid.lib�ɂ����Ȃ��̂Ŏb��[�u
#include <dplay8.h>

#include <stdio.h>
#include <string.h>
#include <time.h>

#pragma warning( disable:4995 )
#pragma warning( disable:4996 )		// �Ƃ肠����sprintf_s�x���𖳎�
