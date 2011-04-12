// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���̃C���N���[�h �t�@�C���A�܂���
// �Q�Ɖ񐔂������A�����܂�ύX����Ȃ��A�v���W�F�N�g��p�̃C���N���[�h �t�@�C��
// ���L�q���܂��B

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

// ��ʓI�Ŗ������Ă����S�� MFC �̌x�����b�Z�[�W�̈ꕔ�̔�\�����������܂��B
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxext.h>         // MFC �̊g������

#include <mmsystem.h>

#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �R���� �R���g���[�� �T�|�[�g
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �R���� �R���g���[�� �T�|�[�g
#endif // _AFX_NO_AFXCMN_SUPPORT

#ifndef MEMLEAK_CK_DEF
#include "define_const.h"
#endif

//�X�N���[���V���b�g�Ŏg��CImage�p
//#define _ATL_NO_DEFAULT_LIBS//���v���ȥ���H//����ς��߂�?
//#include <atlimage.h>

#include <typeinfo>
#include <algorithm>


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

// �f�o�b�O�@�\�͏d���̂�
#ifdef _DEBUG
#undef _DEBUG
#define DOBASHI
#endif // _DEBUG

#include <list>
#include <stack>
#include <vector>

// ���ɖ߂��Ƃ�
#ifdef DOBASHI
#undef DOBASHI
#define _DEBUG
#endif // DOBASHI

#pragma warning( disable:4995 )
#pragma warning( disable:4996 )		// �Ƃ肠����sprintf_s�x���𖳎�
