/*!
*	@file
*	@brief デバッグメニウ
*/
#pragma once

#include "task.h"

/*!
*	@defgroup Debug
*	@brief デバッグ用いろいろ？
*/

/*!
*	@brief デバッグ用簡易タイトル画面
*	@ingroup Debug
*
*	デバッグビルドの場合、タイトル画面の代わりにこれが出ます
*	タイトル画面戻りの操作もこれに戻ります。
*/
class CDebugMenu : public CExclusiveTaskBase
{
public:
	CDebugMenu(){m_selected=0;m_loop_exec=FALSE;}
	void Initialize(){Activate(0);}
	void Terminate(){Inactivate(0);}

	void Activate(DWORD ptid);
	BOOL Inactivate(DWORD ntid);

	BOOL Execute(DWORD time);
	void Draw();
	DWORD GetID(){return('debg');}

protected:
	UINT m_cnt;
	std::vector<TCHAR*> namelist;
	UINT m_selected;

	UINT m_last_selected;
	BOOL m_loop_exec;
};