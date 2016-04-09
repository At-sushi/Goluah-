
/*==================================================================

	Battle Load Information

	�퓬�^�X�N�J�n�ɕK�v�Ȋe����
	���łɁA�퓬�^�X�N�I�����̏��

====================================================================*/

#include "stdafx.h"
#include "gobject.h"			//�ΐ�`����define���K�v
#include "global.h"
#include "battle_load_info.h"

//******************************************************************
//
//						Battle Load Info
//
//******************************************************************

//�S���ڂ��w�薳���ɐݒ肷��
void CBattleLoadInfo::Initialize()
{
	m_limittime = -1;							//�������� : ������
	m_type = TAISENKEISIKI_GOCYAMAZE;			//�ΐ�`�� : ������܂�
	m_num_team[0] = m_num_team[1] = 0;			//1�`�[�����l�� : 0

	for(int i=0;i<2;i++){
		for(int j=0;j<3;j++){
			m_char[i][j] = BLI_NOT_SELECTED;	//�I�����ꂽ�L�����N�^�[ : ���I��
			m_options[i][j] = 0;				//�I�����ꂽ�I�v�V�����ݒ� : 0(�Ȃ�)
			m_color[i][j] = 0;					//�I�����ꂽ�L�����N�^�[�F : 0
			m_key_assign[i][j] = CASSIGN_NONE;	//�L�[�A�T�C���E�Ȃ�
			m_com_level[i][j] = g_config.GetDifficulty();//��Փx : �R���t�B�O����擾
		}
	}
	m_stage = BLI_NOT_SELECTED;					//�I�����ꂽ�X�e�[�W : ���I��
	strcpy(m_storybgm,_T(""));						//�X�g�[���[����w�肳�ꂽBGM : �Ȃ�

	g_battleresult.SetKatiSerif(0,_T("(�䎌���ݒ肳��Ă��܂���)"));
	g_battleresult.SetKatiSerif(1,_T("(�䎌���ݒ肳��Ă��܂���)"));
}

//�w�薳�����ڂ������_���w��Ƃ݂Ȃ��A�L���Ȓl�����蓖�Ă�
void CBattleLoadInfo::ResolveRandom()
{
	//�L�����N�^�[
	for(int i=0;i<2;i++){
		for(DWORD j=0;j<GetNumTeam(i);j++){
			if(m_char[i][j]==BLI_NOT_SELECTED){
				m_char[i][j] = rand()%g_charlist.GetCharacterCount();
				m_options[i][j] = g_charlist.GetRandomOption(m_char[i][j]);
			}
		}
	}

	//�X�e�[�W
	if(m_stage == BLI_NOT_SELECTED){
		m_stage = rand()%g_stagelist.GetStageCount();
	}
}

//*******************************************************************
//	Set�`
//*******************************************************************

/*!
*	�ΐ�`�����w��
*	@param type �ΐ�`��(TAISENKEISIKI_�`�l)
*/
void CBattleLoadInfo::SetBattleType(BYTE type)
{
	m_type = type;
}

/*!
*	�������Ԑݒ�
*	@param val �ݒ�l�B�}�C�i�X�͐����Ȃ���\���B
*/
void CBattleLoadInfo::SetLimitTime(int val)
{
	m_limittime = val;
}

/*/1�`�[���̐l�����w��
void CBattleLoadInfo::SetNumTeam(DWORD num)
{m_num_team = num;}
//�L�����N�^��ݒ�
void CBattleLoadInfo::SetCharacter(DWORD team,DWORD member_index,DWORD character_index)
{m_char[team][member_index] = character_index;}
//�R���s���[�^���ǂ�����ݒ�
void CBattleLoadInfo::SetCharacterIsCOM(DWORD team,DWORD member_index,BOOL val)
{m_is_com[team][member_index] = val;}
//�J���[��ݒ�
void CBattleLoadInfo::SetColor(DWORD team,DWORD member_index,DWORD color)
{m_color[team][member_index] = color;}
//�I�v�V�������w��
void CBattleLoadInfo::SetCharacterOption(DWORD team,DWORD member_index,DWORD optval)
{m_options[team][member_index] = optval;}
*/

/*!
*	�Q���L�����N�^�w��
*	�Ăяo�����񐔂Ń`�[���̐l�������܂�܂��B
*
*	@param team TEAM_PLAYER1/TEAM_PLAYER2 , �Ƃ������A0/1
*	@param cindex �ݒ�L�����N�^�[�C���f�b�N�X
*	@param color �ݒ�F�ԍ�
*	@param key �g�p�L�[���͔ԍ�.�܂��́ACASSIGN_COM(�R���s���[�^����)
*	@param option �ݒ�I�v�V����
*/
void CBattleLoadInfo::AddCharacter( 
						DWORD team,
						DWORD cindex,
						DWORD color,
						BYTE  key,
						DWORD option)
{
	if(team>1)return;
	if(m_num_team[team]>=3)return;

	m_char[team][m_num_team[team]] = cindex;
	m_color[team][m_num_team[team]] = color;
	m_key_assign[team][m_num_team[team]] = key;
	m_options[team][m_num_team[team]] = option;

	m_num_team[team]++;
}


//!�X�e�[�W��ݒ�
void CBattleLoadInfo::SetStage(DWORD stage_index)
{
	m_stage = stage_index;
}

//!�X�g�[���[����w�肳�ꂽBGM
void CBattleLoadInfo::SetStoryBGM(char *filepath)
{
	if(filepath==NULL)return;
	strcpy(m_storybgm,filepath);
}

/*!
*	COM���x���ݒ�
*
*	@param team �`�[��
*	@param cindex �`�[�����̉��l�ڂɑ΂��Đݒ肷�邩(0�`)
*	@param level �ݒ背�x��
*	@param is_related TRUE�̏ꍇ�A�R���t�B�O����̑��΁i�}�C�i�X�l���A���j
*/
void CBattleLoadInfo::SetComLevel(DWORD team,DWORD cindex,int level,BOOL is_related)
{
	int min = DIFF_VERYEASY;
	int max = DIFF_LIMITERCUT;

	if(is_related)
	{
		int lvl = m_com_level[team][cindex] + level;
		if(lvl<min)	lvl=min;
		if(lvl>max) lvl=max;

		m_com_level[team][cindex] = (Config2_Difficulty)lvl;
	}
	else
	{
		if(level<min) level=min;
		if(level>max) level=max;

		m_com_level[team][cindex] = (Config2_Difficulty)level;
	}
}

//*******************************************************************
//	Get�`
//*******************************************************************

//�ΐ�`�����擾
DWORD CBattleLoadInfo::GetBattleType()
{
	return m_type;
}

//�������Ԏ擾
int   CBattleLoadInfo::GetLimitTime()
{
	return m_limittime;
}

//1�`�[���̐l�����擾
DWORD CBattleLoadInfo::GetNumTeam(DWORD team)
{
	return m_num_team[team];
}

//�L�����N�^���擾
DWORD CBattleLoadInfo::GetCharacter(DWORD team,DWORD member_index)
{
	return m_char[team][member_index];
}

//�R���s���[�^���ǂ������擾
BYTE CBattleLoadInfo::GetKeyAssign(DWORD team,DWORD member_index)
{
	return m_key_assign[team][member_index];
}


//�R���s���[�^���ǂ������擾
BOOL CBattleLoadInfo::GetCharacterIsCOM(DWORD team,DWORD member_index)
{
	return m_key_assign[team][member_index] == CASSIGN_COM;
}

//�J���[���擾
DWORD CBattleLoadInfo::GetColor(DWORD team,DWORD member_index)
{
	return m_color[team][member_index];
}

//�I�v�V�������擾
DWORD CBattleLoadInfo::GetCharacterOption(DWORD team,DWORD member_index)
{
	return m_options[team][member_index];
}

//COM���x���擾
Config2_Difficulty CBattleLoadInfo::GetComLevel(DWORD team,DWORD member_idx)
{
	return m_com_level[team][member_idx];
}

//�X�e�[�W���擾
DWORD CBattleLoadInfo::GetStage()
{
	return m_stage;
}

//�X�g�[���[�Ŏw�肳�ꂽBGM�擾�i�ݒ肳��Ă��Ȃ��ꍇNULL�j
char* CBattleLoadInfo::GetStoryBGM()
{
	if(strlen(m_storybgm)==0)return NULL;
	else return m_storybgm;
}



//*******************************************************************
//	survice
//*******************************************************************

DWORD CBattleLoadInfo::GetAllKey()
{
	DWORD ret=0;
	int cnt=0;

	for(int k=0;k<MAXNUM_KEYI;k++){
		for(int j=0;j<2;j++){
			for(int i=0;i<MAXNUM_TEAM;i++){
				if(m_key_assign[j][i]==k){
					ret |= g_input.GetKey(k,0);
					cnt++;
				}
			}
		}
	}
	if(cnt==0)ret |= g_input.GetKey(0,0);

	return TRUE;
}


//******************************************************************
//
//						Battle Result Info
//
//******************************************************************

CBattleResultInfo::CBattleResultInfo()
{
	sprintf(m_serif[0],_T("(�䎌���ݒ肳��Ă��܂���)"));
	sprintf(m_serif[1],_T("(�䎌���ݒ肳��Ă��܂���)"));
}

void CBattleResultInfo::Initialize(BYTE wteam)
{
	m_winner = wteam;
	m_winner_count=g_battleinfo.GetNumTeam(m_winner);

	for(UINT i=0;i<m_winner_count;i++)
	{
		m_character[i] = g_battleinfo.GetCharacter(m_winner,i);
		m_color[i] = g_battleinfo.GetColor(m_winner,i);
	}
}


//*******************************************************************
//	Set�`
//*******************************************************************

void  CBattleResultInfo::SetKatiSerif(DWORD team,char *str)
{
	sprintf( m_serif[team] , str );
}


//*******************************************************************
//	Get�`
//*******************************************************************
BOOL  CBattleResultInfo::GetWinner()
{
	return m_winner == 0 ? FALSE : TRUE;
}

char* CBattleResultInfo::GetKatiSerif(DWORD team)
{
	return m_serif[m_winner];
}

DWORD CBattleResultInfo::GetCharacter(DWORD idx)
{
	return m_character[idx];
}

DWORD CBattleResultInfo::GetColor(UINT idx)
{
	return m_color[idx];
}




