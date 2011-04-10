
/*===============================================================

	�X�^�b�t���[���@�^�X�N

=================================================================*/
#pragma once

#include "task.h"
#include "script_story.h"

/*!
*	@brief �X�^�b�t���[��
*	@ingroup Tasks
*/
class CTStaffRoll : public CExclusiveTaskBase
{
//�֐�
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

//�ϐ�
public:
private:
	DWORD counter;
	DWORD fsize;
	MYSURFACE *ms;//!<�w�i
	char *credits;//!<����镶����
	char *txtpath;
	UINT m_keyindex;

	RECT r;
	int staffindex;
	int staffend;
	int countersr;
};

