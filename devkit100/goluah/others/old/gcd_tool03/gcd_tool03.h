// gcd_tool03.h : PROJECT_NAME �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include <atlimage.h>
#include "resource.h"		// ���C�� �V���{��
#include "gcdhandler.h"


// Cgcd_tool03App:
// ���̃N���X�̎����ɂ��ẮAgcd_tool03.cpp ���Q�Ƃ��Ă��������B
//

class Cgcd_tool03App : public CWinApp
{
public:
	Cgcd_tool03App();

// �I�[�o�[���C�h
	public:
	virtual BOOL InitInstance();

// ����
	DECLARE_MESSAGE_MAP()


public:
	CGCDHandler m_gcd;
	CImage m_img[GCDMAX_IMAGES];

	void Load(char *filename);
	void Save(){m_gcd.Save();}
	void DeleteEmptyRect();
	void SortRects();
	void DeleteEmptyCell();
	void SortCells(BOOL over100);
	BOOL InsertEmptyCell(UINT index);
	void ForceRatio();

	struct CHAR64
	{
		char name[64];
	};
};

extern Cgcd_tool03App theApp;
