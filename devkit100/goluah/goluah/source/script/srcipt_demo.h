
/*!
*	@file
*	@brief ストーリー中間デモスクリプト操作(未使用)
*
*	現在使われておりませんっぽい。
*	ver0.8までのデモスクリプトを作り直そうとしてたみたい。
*/
/*! @ingroup script*/
/*@{*/
#pragma once

#include "define_macro.h"
#include "script_loader.h"

class CDemoScriptElement;
typedef std::vector<CDemoScriptElement*> DScriptElementList;

/*!
*	@brief ストーリーデモ読み込み？
*
*	デモスクリプトを作り直そうとしてたみたい。
*	中身なし
*/
#include "task_demo.h"

class CGoluahDemoScript : public CTDemo
{
public:
	/*!
	*	@brief 読み込み関数
	*
	*	デモスクリプトを作り直そうとしてたみたい。
	*	中身なし
	*/
	static BOOL CreateScriptElementList(SScriptElementList& list,char *filename,CAliasList *args);
};

/*@}*/