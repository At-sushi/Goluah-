#pragma once

#include <windows.h>
#include <stdio.h>
#include "gobjbase.h"

/*!
*	@brief �f�t�H���g�X�e�[�W�N���X
*
*	�X�e�[�W�f�B���N�g����image1.bmp�����[�h���A�w�i�Ƃ��ĕ\������B
*	�ł��V���v���Ȕw�i�B�V�X�e���f�t�H���g�p�B
*/

class CStage : public CStageBase
{
public:
	CStage(SDI_STAGEINFO2 *info);
	~CStage();
	void InitializeStage();
protected:
	DWORD DrawBack();
	MYSURFACE* bitmap;
};