/*!
*	@file
*	@brief �^�X�N(?)�Ǘ��E��`
*/
#pragma once




/*!
*	@defgroup Tasks
*	@brief �^�X�N? =�Q�[���̃V�[���ƍl���Ă��������B
*
*	CTaskBase���p�������N���X�́A���C�����[�v����Ă΂��X�V�E�`�揈���֐��������Ă��܂��B
*	�V�X�e���͂��̃N���X�̃��X�g�������Ă��܂��B
*	�^�C�g���E�L�����Z���E���� �Ȃǂ̃Q�[���̏�Ԃ̕ύX�́A
*	�����^�X�N�N���X�̐؂�ւ��ɂ���čs���܂��B
*/

/*! 
*	@ingroup Tasks
*	@brief	��{�^�X�N
*
*	�EExecute��FALSE��Ԃ��Ɣj�������
*	�E�r���^�X�N���ύX���ꂽ�Ƃ��A�j�������
*/
class CTaskBase
{
public:
	virtual ~CTaskBase(){}
	virtual void Initialize(){}					//!< Execute�܂���Draw���R�[�������O��1�x�����R�[�������
	virtual BOOL Execute(DWORD time)
						{return(TRUE);}			//!< ���t���[���R�[�������
	virtual void Terminate(){}					//!< �^�X�N�̃��X�g����O�����Ƃ��ɃR�[�������i���̒���Adelete�����j
	virtual void Draw(){}						//!< �`�掞�ɃR�[�������
	virtual void WndMessage(					//!< ���C���E�C���h�E�̃��b�Z�[�W�Ɠ������m���󂯎�邱�Ƃ��ł���
					HWND hWnd,
					UINT msg, 
					WPARAM wparam, 
					LPARAM lparam){}
	virtual DWORD GetID(){return 0;}			//!< 0�ȊO��Ԃ��悤�ɂ����ꍇ�A�}�l�[�W���ɓ���ID�����^�X�N��Add���ꂽ�Ƃ��j�������
	virtual int GetDrawPriority(){return -1;}	//!< �`��v���C�I���e�B�B�Ⴂ�قǎ�O�Ɂi��Ɂj�`��B�}�C�i�X�Ȃ�Ε\�����Ȃ�

	static bool CompByDrawPriority(CTaskBase* arg1,CTaskBase* arg2)	//!< �`��v���C�I���e�B�Ń\�[�g���邽�߂̔�r���Z
		{return arg1->GetDrawPriority() > arg2->GetDrawPriority() ;}
};


/*! 
*	@ingroup Tasks
*	@brief �r���^�X�N
*
*	�E���̔r���^�X�N�ƈꏏ�ɂ͓���(Execute)���Ȃ�
*	�E���̔r���^�X�N���ǉ����ꂽ�ꍇ�AInactivate���R�[������A������FALSE��Ԃ���
*		�j�������BTRUE��Ԃ���Execute�AWndMessage���R�[������Ȃ���ԂɂȂ�A
*		�V�K�̔r���^�X�N���S�Ĕj�����ꂽ�Ƃ���Activate���Ă΂�A�������ĊJ����B
*/
class CExclusiveTaskBase : public CTaskBase
{
public:
	virtual ~CExclusiveTaskBase(){}
	virtual void Activate(DWORD prvTaskID){}				//!< Execute���ĊJ�����Ƃ��ɌĂ΂��
	virtual BOOL Inactivate(DWORD nextTaskID){return FALSE;}//!< ���̔r���^�X�N���J�n�����Ƃ��ɌĂ΂��
	
	virtual int GetDrawPriority(){return 0;}				//!< �`��v���C�I���e�B�擾���\�b�h
};



/*!
*	@ingroup Tasks
*	@brief �풓�^�X�N
*
*	�E��{�^�X�N�ƈႢ�A�r���^�X�N���ύX����Ă��j������Ȃ�
*	�EEnabled�łȂ��Ƃ��ɂ� Execute , WndMessage ���R�[�����Ȃ�
*/
class CBackgroundTaskBase : public CTaskBase
{
public:
	virtual ~CBackgroundTaskBase(){}
	CBackgroundTaskBase(){m_isEnabled=TRUE;}

	BOOL IsEnabled(){return m_isEnabled;}
	void Enable(){m_isEnabled = TRUE;}
	void Disable(){m_isEnabled = FALSE;}

protected:
	BOOL m_isEnabled;
};




/*!
*	@ingroup System
*	@brief �^�X�N�Ǘ��N���X
*
*	CSystem�̓����ɕێ������BCSystem�ȊO����͂���ɃA�N�Z�X����K�v�͂Ȃ��͂�
*	�^�X�N�p���N���X�̃��X�g���Ǘ����A�`��A�X�V�A�E�B���h�E���b�Z�[�W���̔z�M���s���B
*
*	���s���ɗ�O���N�������Ƃ��A�ǂ̃N���X����O���N�������̂������O�ɓf���o���B
*	���̍ۂɎ��s���^��񂩂�N���X�����擾���Ă���̂ŁA�R���p�C���̍ۂɂ�
*	���s���^���(RTTI�ƕ\�L�����ꍇ������)��ON�ɂ��邱�ƁB
*/

typedef std::list<CTaskBase*> TaskList;
typedef std::stack<CExclusiveTaskBase*> ExTaskStack;

class CTaskManager
{
public:
	CTaskManager();
	~CTaskManager(){Destroy();}

	void Destroy();

	void AddTask(CTaskBase *newTask);			//!< �^�X�N�ǉ�
	void RemoveTaskByID(DWORD id);				//!< �w��ID�����^�X�N�̏����@�����FExclusive�^�X�N�̓`�F�b�N���Ȃ�
	void ReturnExclusiveTaskByID(DWORD id);		//!< �w��ID�̔r���^�X�N�܂�Terminate/pop����
	CExclusiveTaskBase* GetTopExclusiveTask();	//!< �ŏ�ʂɂ���G�N�X�N���[�V�u�^�X�N���Q�g
	CBackgroundTaskBase* FindBGTask(DWORD id);	//!< �w��ID�����풓�^�X�N�Q�b�g
	CTaskBase* FindTask(DWORD id);				//!< �w��ID�����ʏ�^�X�N�Q�b�g

	void Execute(DWORD time);					//!< CSystem����R�[������A�e�^�X�N�̓��֐����R�[������
	void Draw();								//!< CSystem����R�[������A�e�^�X�N���v���C�I���e�B���ɕ`�悷��
	void WndMessage(							//!< CSystem����R�[������A�e�^�X�N�̓��֐����R�[������
			HWND hWnd,
			UINT msg,
			WPARAM wparam,
			LPARAM lparam);
	BOOL ExEmpty();								//!< �r���^�X�N���S���Ȃ��Ȃ�����������ǂ���

	//�f�o�b�O
	void DebugOutputTaskList();					//!< ���݃��X�g�ɕێ�����Ă���N���X�̃N���X�����f�o�b�O�o�͂���

protected:
	void CleanupAllSubTasks();					//!< �ʏ�^�X�N��S��Terminate , delete����
	void SortTask(TaskList *ptgt);				//!< �^�X�N��`��v���C�I���e�B���ɕ��ׂ�

	TaskList tasks;								//!< ���ݓ��삿�イ�̃^�X�N���X�g
	TaskList bg_tasks;							//!< �풓�^�X�N���X�g
	ExTaskStack ex_stack;						//!< �r���^�X�N�̃X�^�b�N�Btop�������s���Ȃ�

	CExclusiveTaskBase* exNext;					//!< ���݃t���[����Add���ꂽ�r���^�X�N
};


