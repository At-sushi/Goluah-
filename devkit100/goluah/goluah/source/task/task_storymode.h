
/*===============================================================

	�X�g�[���[���[�h�^�X�N

=================================================================*/
#pragma once

#include "task.h"
#include "script_story.h"

class CTaskStoryMode;	//�X�g�[���[���[�h�{��
class CTStoryContinue;	//�R���e�B�j���[�^�X�N
class CTStoryGameOver;	//�Q�[���I�[�o�[�^�X�N


/*!
*	@brief �X�g�[���[���[�h�{��
*	@ingroup Tasks
*
*	Activate���ɑO�^�X�N��A�X�g�[���[�N���X�̏�񂩂�
*	���f���ēK�؂Ȏ��̃^�X�N���N������B
*/
class CTaskStoryMode : public CExclusiveTaskBase
{
public:
	CTaskStoryMode();
	~CTaskStoryMode();

	void Setup(UINT storyIndex,UINT keyindex,CStoryList::CStoryInfo* info);

	void Initialize();
	BOOL Execute(DWORD time);
	void Terminate();
	DWORD GetID(){return('StMd');}

	void Activate(DWORD prvTaskID);
	BOOL Inactivate(DWORD nextTaskID){return TRUE;}

protected:
	UINT m_story_index;					//!<�X�g�[���[�ԍ�
	UINT m_keyindex;					//!<�v���C���[�̃L�[���͔ԍ�
	CStoryList::CStoryInfo* m_setting;	//!<�X�g�[���[�����ݒ�
	SScriptElementList scr_list;		//!<�X�N���v�g���X�g
	SScriptElementList::iterator scr_i;	//!<���݂̃X�N���v�g�u���b�N
	int m_continue_num;					//!<�R���e�B�j���[��

	DWORD m_prev_task_id;				//!<�O��^�X�NID
	int selectflag;						//!<�I�����t���O
	bool in_select;						//!<select����ォ�ǂ���

	//�^�X�N�N��
	void StartNextTask();				//!<���̃X�g�[���[�v�f�̊J�n
	void StartVSTask();					//!<����
	void StartDemoTask();				//!<�f��
	void StartStaffTask();				//!<�X�^�b�t���[��
	void StartContinueTask();			//!<�R���e�B�j���[

	/*!
	*	@brief �����_���L�����N�^�[�擾�N���X
	*	@ingroup Tasks
	*
	*	�X�g�[���[���łȂ�ׂ������L�����N�^�[���o�Ȃ��悤�ɂ��邽�߂Ɏg�p�B
	*/
	class CRandomCharacterGetter
	{
	public:
		void Initialize(SScriptElementList& scr_list);
		void Req(UINT num);	//!< ���̎�����,�w�肵�����̃����_���L�����N�^�[���K�v�B����Ȃ��ꍇ�̓V���b�t�����Ȃ���
		UINT GetNext();		//!< ���̃����_���L�����N�^�[���擾����

		int chars[MAXNUM_TEAM];//!< settings�u���b�N�Ŏw�肳�ꂽ�L�����N�^�[

	protected:
		void EraseFromAllList(UINT n);
		typedef std::vector<UINT> CIndexList;
		CIndexList all_list;
		CIndexList crnt_list;
	};
	CRandomCharacterGetter m_RandomCharacterGetter;

	/*!
	*	@brief �����_���X�e�[�W�擾�N���X
	*	@ingroup Tasks
	*
	*	�X�g�[���[���łȂ�ׂ������X�e�[�W���o�Ȃ��悤�ɂ��邽�߂̃N���X�B
	*/
	class CRandomStageGetter
	{
	public:
		void Initialize(SScriptElementList& scr_list);
		UINT GetNext();		//!< ���̃����_���X�e�[�W���擾����

	protected:
		void EraseFromAllList(UINT n);
		void Restore();

		typedef std::vector<UINT> SIndexList;
		SIndexList all_list;
		SIndexList crnt_list;
	};
	CRandomStageGetter m_RandomStageGetter;
};



/*!
*	@brief �X�g�[���[���[�h���E�R���e�B�j���[
*	@ingroup Tasks
*/
class CTStoryContinue : public CExclusiveTaskBase
{
public:
	virtual ~CTStoryContinue(){}
	virtual void Initialize();
	virtual BOOL Execute(DWORD time);
	virtual void Terminate();
	virtual void Draw();

	int remain_cont;
protected:
	void DrawFBC(float t);

	float m_time;
	LPDIRECT3DTEXTURE8	m_texfb;

	BOOL m_yes_selected;
};


/*!
*	@brief �X�g�[���[���[�h���E�Q�[���I�[�o�[
*	@ingroup Tasks
*/
class CTStoryGameOver : public CTStoryContinue
{
public:
	BOOL Execute(DWORD time);
	void Draw();
};
