
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
	char* GetStoryName(UINT index);					//!< �w��C���f�b�N�X�̃X�g�[���[�����擾����
	char* GetStoryBrief(UINT index);				//!< �w��C���f�b�N�X�̃X�g�[���[�T�v���擾����
	char* GetStoryDir(UINT index);					//!< �w��C���f�b�N�X�̃X�g�[���[�f�B���N�g�����擾����
	void GetStoryIconPath(UINT index,char* dst);	//!< �w��C���f�b�N�X�̃X�g�[���[�A�C�R���t�@�C�������擾����
	void GetStoryPreviewPath(UINT index,char* dst);	//!< �w��C���f�b�N�X�̃X�g�[���[�v���r���[�t�@�C�������擾����
	CStoryInfo* GetStoryInfo(UINT index);			//!< �w��C���f�b�N�X�̃X�g�[���[�����擾

	int   Ring2Index(UINT ridx,UINT idx);			//!< �f�B���N�g���C���f�b�N�X+�f�B���N�g�����C���f�b�N�X����A�S�̒ʂ��ԍ����擾
	UINT  GetRingNum();								//!< �f�B���N�g�����擾
	int   GetRingCount(UINT idx);					//!< �w��C���f�b�N�X�̃f�B���N�g�����ɂ���X�g�[���[���擾

	int   GetErrorCount();							//!< �ǂݍ��݂Ɏ��s�����X�g�[���[���擾
	char* GetErrorStr(UINT idx);					//!< �ǂݍ��ݎ��s���R�擾
	char* GetErrorDir(UINT idx);					//!< �ǂݍ��ݎ��s�f�B���N�g��


	/*
		@brief �X�g�[���[���X�g�v�f��`
	*/
	struct CStoryInfo
	{
		char dir[128];					//!< ���s�t�@�C������̑��΃p�X
		char name[64];
		char icon[64];
		char preview[64];
		char brief[512];

		//�L�����N�^��`
		int cnum;						//!< char�w�萔
		int characters[MAXNUM_TEAM];	//!< char�w��@(-1:userselect -2:�w��Ȃ�)
		int color[MAXNUM_TEAM];			//!< �F�w��
		DWORD option[MAXNUM_TEAM];		//!< �I�v�V�������ڎw��(userselect�ȊO�ŗL��)
		StoryOptType opttype[MAXNUM_TEAM];//!< �I�v�V�����w��t���O(userselect�ȊO�ŗL��)

		void Clear();
		void Setup(CStoryElement_Settings* settings,char* dir1,char* dir2);
		void SetDir(char *s,char *s2);
		void SetName(char *s);
		void SetBrief(char *s);
		void SetIcon(char *s);
		void SetPreview(char *s);

		CStoryInfo* Clone();
	};
	typedef std::vector<CStoryInfo> CStoryInfoList;

protected:
	//!�f�B���N�g�����X�g�v�f
	struct CStoryRingInfo
	{
		char dir[64];
		std::vector<DWORD> ring2serial;//!< �����O���ł̃C���f�b�N�X���S�̒ʂ��ԍ��C���f�b�N�X�ϊ�
	};
	typedef std::vector<CStoryRingInfo> CStoryRingInfoList;


	CStoryInfoList     list;
	CStoryInfoList     dlist;//!< ���؎��s���X�g�Bbrief �Ƀ_�����R������B
	CStoryRingInfoList rlist;

protected:

	//�X�N���v�g���ߌ�̗v�f���X�g����̊e������o��
	static char* GetName(SScriptElementList& scr);
	static char* GetBrief(SScriptElementList& scr);
	static int*  GetCharacters(SScriptElementList& scr);
	static char* GetFirstError(SScriptElementList& scr);
	static CStoryElement_Settings* GetSettingsFromScriptList(SScriptElementList& scr);
};



/*@}*/