
/*===============================================================

	スタッフロール　タスク

=================================================================*/
#pragma once

#include "task.h"
#include "script_story.h"

/*!
*	@brief スタッフロール
*	@ingroup Tasks
*/
class CTStaffRoll : public CExclusiveTaskBase
{
//関数
public:
	CTStaffRoll();
	~CTStaffRoll(){CleanUp();}

	void Initialize();
	BOOL Execute(DWORD time);
	void Terminate();
	void Draw();

	void SetKeyIndex(UINT ki){m_keyindex=ki;}
	void Setup(char *basedir,CStoryElement_Staff* ele_staff);

private:
	void CleanUp();
	int Get1Gyo(char *str);
	void SetTextPath(char *path);

//変数
public:
private:
	DWORD counter;
	DWORD fsize;
	MYSURFACE *ms;//!<背景
	char *credits;//!<流れる文字列
	char *txtpath;
	UINT m_keyindex;

	RECT r;
	int staffindex;
	int staffend;
	int countersr;
};

