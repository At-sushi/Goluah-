#pragma once

#include "task.h"

/*!
* ���̃N���X�́A�܂��قƂ�ǎ�����B
* ��ʂ̃t�F�[�h�A�E�g�^�t�F�[�h�C���Ɏg���\��ł��B
*/
class CFade : public CBackgroundTaskBase
{
public:
	CFade(void);

	//�^�X�N�֐�
	virtual void Initialize();
	virtual BOOL Execute(DWORD time);
	virtual void Draw();
};
