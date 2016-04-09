
/*========================================================================

	�X�g�[���[�Ǘ�

==========================================================================*/
#pragma once

#include "define_const.h"
#include "script_story.h"

class CStoryElement_Settings;

/*!
*	@ingroup global
*/
/*@{*/

/*!
*	@brief �X�g�[���[���X�g�ێ�
*	Initialize�̓L�����N�^�[���X�g�E�X�e�[�W���X�g���\�z���ꂽ��ɍs������
*/
class CStoryList
{
public:
	struct CStoryInfo;

	void Initialize();

	UINT  GetAllStoryNum();							//!< �S�X�g�[���[�����擾
	void  StartStory(UINT index);					//!< �w��C���f�b�N�X�̃X�g�[���[���J�n����
	TCHAR* GetStoryName(UINT index);					//!< �w��C���f�b�N�X�̃X�g�[���[�����擾����
	TCHAR* GetStoryBrief(UINT index);				//!< �w��C���f�b�N�X�̃X�g�[���[�T�v���擾����
	TCHAR* GetStoryDir(UINT index);					//!< �w��C���f�b�N�X�̃X�g�[���[�f�B���N�g�����擾����
	void GetStoryIconPath(UINT index,TCHAR* dst);	//!< �w��C���f�b�N�X�̃X�g�[���[�A�C�R���t�@�C�������擾����
	void GetStoryPreviewPath(UINT index,TCHAR* dst);	//!< �w��C���f�b�N�X�̃X�g�[���[�v���r���[�t�@�C�������擾����
	CStoryInfo* GetStoryInfo(UINT index);			//!< �w��C���f�b�N�X�̃X�g�[���[�����擾

	int   Ring2Index(UINT ridx,UINT idx);			//!< �f�B���N�g���C���f�b�N�X+�f�B���N�g�����C���f�b�N�X����A�S�̒ʂ��ԍ����擾
	UINT  GetRingNum();								//!< �f�B���N�g�����擾
	int   GetRingCount(UINT idx);					//!< �w��C���f�b�N�X�̃f�B���N�g�����ɂ���X�g�[���[���擾

	int   GetErrorCount();							//!< �ǂݍ��݂Ɏ��s�����X�g�[���[���擾
	TCHAR* GetErrorStr(UINT idx);					//!< �ǂݍ��ݎ��s���R�擾
	TCHAR* GetErrorDir(UINT idx);					//!< �ǂݍ��ݎ��s�f�B���N�g��


	/*
		@brief �X�g�[���[���X�g�v�f��`
	*/
	struct CStoryInfo
	{
		TCHAR dir[128];					//!< ���s�t�@�C������̑��΃p�X
		TCHAR name[64];
		TCHAR icon[64];
		TCHAR preview[64];
		TCHAR brief[512];

		//�L�����N�^��`
		int cnum;						//!< TCHAR�w�萔
		int characters[MAXNUM_TEAM];	//!< TCHAR�w��@(-1:userselect -2:�w��Ȃ�)
		int color[MAXNUM_TEAM];			//!< �F�w��
		DWORD option[MAXNUM_TEAM];		//!< �I�v�V�������ڎw��(userselect�ȊO�ŗL��)
		StoryOptType opttype[MAXNUM_TEAM];//!< �I�v�V�����w��t���O(userselect�ȊO�ŗL��)

		void Clear();
		void Setup(CStoryElement_Settings* settings,TCHAR* dir1,TCHAR* dir2);
		void SetDir(TCHAR *s,TCHAR *s2);
		void SetName(TCHAR *s);
		void SetBrief(TCHAR *s);
		void SetIcon(TCHAR *s);
		void SetPreview(TCHAR *s);

		CStoryInfo* Clone();
	};
	typedef std::vector<CStoryInfo> CStoryInfoList;

protected:
	//!�f�B���N�g�����X�g�v�f
	struct CStoryRingInfo
	{
		TCHAR dir[64];
		std::vector<DWORD> ring2serial;//!< �����O���ł̃C���f�b�N�X���S�̒ʂ��ԍ��C���f�b�N�X�ϊ�
	};
	typedef std::vector<CStoryRingInfo> CStoryRingInfoList;


	CStoryInfoList     list;
	CStoryInfoList     dlist;//!< ���؎��s���X�g�Bbrief �Ƀ_�����R������B
	CStoryRingInfoList rlist;

protected:

	//�X�N���v�g���ߌ�̗v�f���X�g����̊e������o��
	static TCHAR* GetName(SScriptElementList& scr);
	static TCHAR* GetBrief(SScriptElementList& scr);
	static int*  GetCharacters(SScriptElementList& scr);
	static TCHAR* GetFirstError(SScriptElementList& scr);
	static CStoryElement_Settings* GetSettingsFromScriptList(SScriptElementList& scr);
};



/*@}*/