
/*===============================================================

	�I�[�v�j���O���[�r�[�Đ��@�^�X�N

=================================================================*/
#pragma once

#include "task.h"

/*!
*	@brief �I�[�v�j���O���[�r�[�Đ�
*	@ingroup Tasks
*/
class CTOpening : public CExclusiveTaskBase
{
public:
	CTOpening();
	~CTOpening();

	void Initialize();
	BOOL Execute(DWORD time);
	void Terminate();
	DWORD GetID(){return 'OpMv';}
	BOOL IsOK() { return opok; }

private:
	BOOL opok;
	UINT counter;
	BOOL IMEOld;
};