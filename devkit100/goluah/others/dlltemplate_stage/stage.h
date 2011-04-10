#pragma once

#include <windows.h>
#include <stdio.h>
#include "gobjbase.h"

/*!
*	@brief ステージクラス
*/

class CStage : public CStageBase
{
public:
	CStage(SDI_STAGEINFO2 *info);
	~CStage();
	void InitializeStage();
protected:
	DWORD DrawBack();

	//試合を通して必要となる変数はここに宣言する
};