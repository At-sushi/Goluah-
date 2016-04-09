
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
	void Setup(TCHAR *basedir,CStoryElement_Staff* ele_staff);

private:
	void CleanUp();
	int Get1Gyo(TCHAR *str);
	void SetTextPath(TCHAR *path);

//変数
public:
private:
	DWORD counter;
	DWORD fsize;
	MYSURFACE *ms;//!<背景
	TCHAR *credits;//!<流れる文字列
	TCHAR *txtpath;
	UINT m_keyindex;

	RECT r;
	int staffindex;
	int staffend;
	int countersr;
};

