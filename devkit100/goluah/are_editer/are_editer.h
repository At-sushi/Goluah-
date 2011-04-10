// are_editer.h : PROJECT_NAME �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "dx_draw.h"
#include "gcdhandler.h"
#include "resource.h"		// ���C�� �V���{��
#include <atlimage.h>

/*-------------------------------------------------------------------------
// CAre_editerApp:
// ���̃N���X�̎����ɂ��ẮAare_editer.cpp ���Q�Ƃ��Ă��������B
//�E�E�E���Ƃ�
---------------------------------------------------------------------------*/
class CAre_editerApp : public CWinApp
{
public:
	CAre_editerApp();

	int ReloadBitmaps();

	void LoadGCD(CString filename);
//	void SaveGCD070(CString filename);
//	void SaveGCD090(CString filename);

	GCD_RECT* GetCurrentRect();
	GCD_CELL2* GetCurrentCell();
	GCD_HANTEI* GetCurrentHantei();
	CGCDHandler* GetGCD(){return &m_gcd;}

	void InvalidateAll();
	void SetStatus(CString str);
	void SpecialCtrl(UINT key);

	MYSURFACE*  GetBitmap(UINT no){return m_bmps[no];}
	MYSURFACE** GetBitmaps(){return m_bmps;}
	CImage& GetImage(UINT no){return m_images[no];}
	CString GetBMPFilename(UINT n);

	void SetCurrentRectNOPointer(DWORD* p){m_pCurrentRectNO=p;}
	void SetCurrentCellNOPointer(DWORD* p){m_pCurrentCellNO=p;}
	UINT GetCurrentCellNO(){return *m_pCurrentCellNO;}
	void SetCurrentCellRectNOPointer(DWORD* p){m_pCurrentCellRectNO=p;}
	UINT GetCurrentCellRectNO(){return *m_pCurrentCellRectNO;}

	//r-tools
	void InsertEmptyRect(UINT index);	//�w��ʒu�ɋ�̋�`��}��
	void DeleteInvalidRect();			//�r�b�g�}�b�v��̉����Ȃ��̈���Q�Ƃ��Ă����`����������
	void DeleteEmptyRect();				//��̋�`���������Ĕԍ����l�߂�

	//c-tools
	void InsertEmptyCell(UINT index);	//�w��ʒu�ɋ󂫃Z����}��
	void DeleteInvalidCell();			//��̋�`�����Q�Ƃ��Ă��Ȃ��Z������������
	void DeleteEmptyCell(BOOL under100);//���CELL���������Ĕԍ����l�߂�

protected:
	CGCDHandler m_gcd;
	MYSURFACE* m_bmps[GCDMAX_IMAGES];
	CImage m_images[GCDMAX_IMAGES];
	CString m_current_dir;
	DWORD *m_pCurrentRectNO;
	DWORD *m_pCurrentCellNO;
	DWORD *m_pCurrentCellRectNO;

// �I�[�o�[���C�h
	public:
	virtual BOOL InitInstance();

// ����
	DECLARE_MESSAGE_MAP()
};

extern CAre_editerApp theApp;



/*-------------------------------------------------------------------------
	���ʃ_�C�A���O�x�[�X
---------------------------------------------------------------------------*/
class CMyDialogTab : public CDialog
{
public:

	DECLARE_DYNAMIC(CMyDialogTab)//�Ȃ񂶂Ⴑ���H

	CMyDialogTab(UINT nIDTemplate,CWnd* pParentWnd = NULL); 
	virtual void UpdateItems()=0;
};



