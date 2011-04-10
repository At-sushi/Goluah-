#include "task_battle.h"

/*!
*	@brief	�f����ʗp�퓬�^�X�N
*	@ingroup Battle
*
*	�^�C�g����ʂł��΂炭�����Ă����Ǝn�܂�f����ʂ̃^�X�N�B
*	�d���΍�̂��߁A�P�`�[���̃L�������������������Ă܂��B
*/
class CBattleTaskDemo :
	public CBattleTask
{
public:
	virtual ~CBattleTaskDemo(void){}

	virtual void Initialize();
	virtual BOOL Execute(DWORD time);

protected:
	virtual void Setting_BattleType();			//!< �ΐ�`����ݒ�
	virtual void Setting_Characters();			//!< �L�����N�^�[��ݒ�
	virtual void Setting_Stage();				//!< �X�e�[�W��ݒ�
	virtual void Setting_Other();				//!< ���̑��H���ԂƂ��B

	enum{
		DEMOBATTLE_MAXTEAM = 2,					//!< �`�[�����Ƃ̍ő�l��
		DEMOBATTLE_LIMITTIME = 3000				//!< �f�����I���܂ł̎���
	};

	void InitRandomCharacters();				//!< m_randchar �ɓK�؂Ȓl��I������
	UINT m_randchar[2][DEMOBATTLE_MAXTEAM];		//!< (�Ȃ�ׂ�)�d�����Ȃ��悤�ɑI�����������_���L����
};
