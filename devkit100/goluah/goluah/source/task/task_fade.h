#pragma once

#include "task.h"

/*!
* このクラスは、まだほとんど手つかず。
* 画面のフェードアウト／フェードインに使う予定です。
*/
class CFade : public CBackgroundTaskBase
{
public:
	CFade(void);

	//タスク関数
	virtual void Initialize();
	virtual BOOL Execute(DWORD time);
	virtual void Draw();
};
