/*!
	@file
	@brief Battle Load Information

	�퓬�^�X�N�J�n�ɕK�v�Ȋe����
	���łɁA�퓬�^�X�N�I�����̏��

*/
#pragma once


#define BLI_NOT_SELECTED	(0xFFFFFFFF)

/*!
	@ingroup global
	@brief �퓬�^�X�N�J�n�ɕK�v�ȏ��ێ��N���X
	�O���[�o����1�������݂��āA�^�X�N�ԂŎ��������󂯓n�����߂̃N���X�B
*/
class CBattleLoadInfo
{
public:
	void Initialize();		//!<�S���ڂ��w�薳���ɐݒ肷��
	void ResolveRandom();	//!<�w�薳�����ڂ������_���w��Ƃ݂Ȃ��A�L���Ȓl�����蓖�Ă�

	//Set�n
	void SetBattleType(BYTE type);								//!<�ΐ�`�����w��iTAISENKEISIKI_�`�j
	void SetLimitTime(int val);									//!<�������Ԑݒ�
	void AddCharacter(  										//!<�L�����N�^�ǉ�
						DWORD team,
						DWORD cindex,
						DWORD color,
						BYTE  key,
						DWORD option);
	void SetStage(DWORD stage_index);							//!<�X�e�[�W��ݒ�
	void SetStoryBGM(char *filepath);							//!<�X�g�[���[����w�肳�ꂽBGM
	void SetComLevel(DWORD team,DWORD cindex,					//!<COM�̃��x��
					int level,BOOL is_related);

	//Get�n
	DWORD GetBattleType();										//!<�ΐ�`�����擾
	int   GetLimitTime();										//!<�������Ԏ擾
	DWORD GetNumTeam(DWORD team);								//!<1�`�[���̐l�����擾
	DWORD GetCharacter(DWORD team,DWORD member_index);			//!<�L�����N�^���擾
	BYTE  GetKeyAssign(DWORD team,DWORD member_index);			
	BOOL  GetCharacterIsCOM(DWORD team,DWORD member_index);		//!<�R���s���[�^���ǂ������擾
	DWORD GetColor(DWORD team,DWORD member_index);				//!<�J���[���擾
	DWORD GetCharacterOption(DWORD team,DWORD member_index);	//!<�I�v�V�������擾
	DWORD GetStage();											//!<�X�e�[�W���擾
	char* GetStoryBGM();										//!<�X�g�[���[�Ŏw�肳�ꂽBGM�擾�i�ݒ肳��Ă��Ȃ��ꍇNULL�j
	Config2_Difficulty GetComLevel(DWORD team,DWORD member_idx);//!<COM���x���擾

	DWORD GetAllKey();//!<�����Ɋւ���Ă�S�v���C���[�L�[���̘͂a���Q�b�g

protected:
	int	  m_limittime;					//!<��������
	DWORD m_type;						//!<�ΐ�`��
	DWORD m_num_team[2];				//!<1�`�[�����l��
	DWORD m_char[2][MAXNUM_TEAM];		//!<�I�����ꂽ�L�����N�^�[
	DWORD m_options[2][MAXNUM_TEAM];	//!<�I�����ꂽ�I�v�V�����ݒ�
	DWORD m_color[2][MAXNUM_TEAM];		//!<�I�����ꂽ�L�����N�^�[�F
	BYTE  m_key_assign[2][MAXNUM_TEAM];	//!<�R���s���[�^���ǂ���
	DWORD m_stage;						//!<�I�����ꂽ�X�e�[�W
	char  m_storybgm[256];				//!<�X�g�[���[����w�肳�ꂽBGM
	Config2_Difficulty m_com_level[2][MAXNUM_TEAM];//!<�R���s���[�^��Փx�Bg_config����擾�����l+�X�g�[���[���[�h�ł̎w��l
};


/*!
	@ingroup global
	@brief �퓬�^�X�N�I�����̏��
	CBattleLoadInfo �Ɠ����悤�Ȉʒu�Â��ŁA�������͏��s��
	��������ӓ��A�I�����̏���ێ�����
*/
class CBattleResultInfo
{
public:
	CBattleResultInfo();

	void Initialize(BYTE wteam);						//!<CBattleLoadInfo��������R�s�[���ď�������
	void SetKatiSerif(DWORD team,char *str);			//!<�����䎌�ݒ�

	BOOL  GetWinner();									//!<�����`�[���Q�g
	UINT  GetWinnerCount(){return m_winner_count;}		//!<�������`�[���l��
	char* GetKatiSerif(DWORD team=3);					//!<�䎌�Q�g
	DWORD GetCharacter(DWORD index);					//!<�L�����N�^�Q�g
	DWORD GetColor(UINT index);							//!<���������̃J���[

protected:
	BYTE  m_winner;					//!<�����`�[��(0or1)
	char  m_serif[2][256];			//!<�䎌

	DWORD m_character[MAXNUM_TEAM];	//!<�L�����N�^
	UINT  m_color[MAXNUM_TEAM];		//!<�J���[
	UINT  m_winner_count;			//!<�������`�[���l��
};

