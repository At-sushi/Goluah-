#pragma once

#include <windows.h>
#include <stdio.h>
#include "gobjbase.h"

/*!
*	@brief �X�e�[�W�N���X
*/

class CStage : public CStageBase
{
public:
	CStage(SDI_STAGEINFO2 *info);
	~CStage();
	void InitializeStage();
protected:
	DWORD DrawBack();

	//������ʂ��ĕK�v�ƂȂ�ϐ��͂����ɐ錾����
};