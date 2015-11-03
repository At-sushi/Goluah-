#pragma once

#include <windows.h>
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "gobjbase.h"
#include "aki3d.h"

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
	CMesh* back;
};