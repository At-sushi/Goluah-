#pragma once

#include <windows.h>
#include <stdio.h>
#include <d3d8.h>
#include <d3dx8.h>
#include "gobjbase.h"
#include "aki3d.h"

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
	CMesh* back;
};