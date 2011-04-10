
/*!
*	@file
*	@brief �f�J��L���b�V��
*	�\���ɕK�v�Ȃ��Ȃ������̂��A�����[�h�����ɕێ����Ă�����
*	�ǂݍ��݉񐔂��y������
*/
#pragma once

#include "define_gcd.h"
#include "task.h"

#define BIGFACE_TASKID	'BFCT'

/*!
*	@ingroup global
*	@brief �f�J��擾�N���X
*
*	�f�J��(face2.bmp)��\������Ƃ��ɁA���������r�b�g�}�b�v�̃��[�h�ƃA�����[�h��
*	�Ǘ�����̂��ʓ|�Ȃ̂ŁA��������擾����悤�ɂ����B
*	�v�����ꂽ�r�b�g�}�b�v�����łɓǂݍ��܂�Ă���ꍇ�̓��[�h���Ȃ��ł����Ԃ��B
*	�����I�ȃA�����[�h�͑��݂��Ȃ��̂ŁA�펞���\�Ȑ��̃r�b�g�}�b�v�����[�h������ςȂ��B
*/
class CTBigFaceCache : public CBackgroundTaskBase
{
public:
	~CTBigFaceCache(){Terminate();}
	
	BOOL Execute(DWORD time);			//!< 1�t���X�V�^�C�~���O
	void Terminate();
	DWORD GetID(){return BIGFACE_TASKID;}
	
	MYSURFACE* Get(int cindex,int col,int alt);
	void Destroy();

protected:
	//! face2�r�b�g�}�b�v���Ǘ�,1����
	struct CTextureRef
	{
		MYSURFACE *dds;
		int cindex;
		int col;
		int alt;
		BOOL prev_ref;
		BOOL crnt_ref;

		CTextureRef();
		~CTextureRef();
	};
	typedef std::list<CTextureRef*> texref_list;

	texref_list list;
};