#pragma once
#include "task_battle.h"

//!ボーナスステージの作りかけ…みたい
class CBonusTask :
	public CBattleTask
{
public:
	CBonusTask(void);
	virtual ~CBonusTask(void);
	virtual void Initialize(void);
};
