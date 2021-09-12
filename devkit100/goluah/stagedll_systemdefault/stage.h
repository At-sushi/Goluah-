#pragma once

#include <windows.h>
#include <stdio.h>
#include "gobjbase.h"

/*!
*	@brief デフォルトステージクラス
*
*	ステージディレクトリのimage1.bmpをロードし、背景として表示する。
*	最もシンプルな背景。システムデフォルト用。
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
