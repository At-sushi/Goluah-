
/*==================================================================

	Battle Load Information

	戦闘タスク開始に必要な各種情報
	ついでに、戦闘タスク終了時の情報

====================================================================*/

#include "stdafx.h"
#include "gobject.h"			//対戦形式のdefineが必要
#include "global.h"
#include "battle_load_info.h"

//******************************************************************
//
//						Battle Load Info
//
//******************************************************************

//全項目を指定無しに設定する
void CBattleLoadInfo::Initialize()
{
	m_limittime = -1;							//制限時間 : 無制限
	m_type = TAISENKEISIKI_GOCYAMAZE;			//対戦形式 : ごちゃまぜ
	m_num_team[0] = m_num_team[1] = 0;			//1チーム何人か : 0

	for(int i=0;i<2;i++){
		for(int j=0;j<3;j++){
			m_char[i][j] = BLI_NOT_SELECTED;	//選択されたキャラクター : 未選択
			m_options[i][j] = 0;				//選択されたオプション設定 : 0(なし)
			m_color[i][j] = 0;					//選択されたキャラクター色 : 0
			m_key_assign[i][j] = CASSIGN_NONE;	//キーアサイン・なし
			m_com_level[i][j] = g_config.GetDifficulty();//難易度 : コンフィグから取得
		}
	}
	m_stage = BLI_NOT_SELECTED;					//選択されたステージ : 未選択
	strcpy(m_storybgm,_T(""));						//ストーリーから指定されたBGM : なし

	g_battleresult.SetKatiSerif(0,_T("(台詞が設定されていません)"));
	g_battleresult.SetKatiSerif(1,_T("(台詞が設定されていません)"));
}

//指定無し項目をランダム指定とみなし、有効な値を割り当てる
void CBattleLoadInfo::ResolveRandom()
{
	//キャラクター
	for(int i=0;i<2;i++){
		for(DWORD j=0;j<GetNumTeam(i);j++){
			if(m_char[i][j]==BLI_NOT_SELECTED){
				m_char[i][j] = rand()%g_charlist.GetCharacterCount();
				m_options[i][j] = g_charlist.GetRandomOption(m_char[i][j]);
			}
		}
	}

	//ステージ
	if(m_stage == BLI_NOT_SELECTED){
		m_stage = rand()%g_stagelist.GetStageCount();
	}
}

//*******************************************************************
//	Set～
//*******************************************************************

/*!
*	対戦形式を指定
*	@param type 対戦形式(TAISENKEISIKI_～値)
*/
void CBattleLoadInfo::SetBattleType(BYTE type)
{
	m_type = type;
}

/*!
*	制限時間設定
*	@param val 設定値。マイナスは制限なしを表す。
*/
void CBattleLoadInfo::SetLimitTime(int val)
{
	m_limittime = val;
}

/*/1チームの人数を指定
void CBattleLoadInfo::SetNumTeam(DWORD num)
{m_num_team = num;}
//キャラクタを設定
void CBattleLoadInfo::SetCharacter(DWORD team,DWORD member_index,DWORD character_index)
{m_char[team][member_index] = character_index;}
//コンピュータかどうかを設定
void CBattleLoadInfo::SetCharacterIsCOM(DWORD team,DWORD member_index,BOOL val)
{m_is_com[team][member_index] = val;}
//カラーを設定
void CBattleLoadInfo::SetColor(DWORD team,DWORD member_index,DWORD color)
{m_color[team][member_index] = color;}
//オプションを指定
void CBattleLoadInfo::SetCharacterOption(DWORD team,DWORD member_index,DWORD optval)
{m_options[team][member_index] = optval;}
*/

/*!
*	参加キャラクタ指定
*	呼び出した回数でチームの人数が決まります。
*
*	@param team TEAM_PLAYER1/TEAM_PLAYER2 , というか、0/1
*	@param cindex 設定キャラクターインデックス
*	@param color 設定色番号
*	@param key 使用キー入力番号.または、CASSIGN_COM(コンピュータ制御)
*	@param option 設定オプション
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


//!ステージを設定
void CBattleLoadInfo::SetStage(DWORD stage_index)
{
	m_stage = stage_index;
}

//!ストーリーから指定されたBGM
void CBattleLoadInfo::SetStoryBGM(char *filepath)
{
	if(filepath==NULL)return;
	strcpy(m_storybgm,filepath);
}

/*!
*	COMレベル設定
*
*	@param team チーム
*	@param cindex チーム内の何人目に対して設定するか(0～)
*	@param level 設定レベル
*	@param is_related TRUEの場合、コンフィグからの相対（マイナス値もアリ）
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
//	Get～
//*******************************************************************

//対戦形式を取得
DWORD CBattleLoadInfo::GetBattleType()
{
	return m_type;
}

//制限時間取得
int   CBattleLoadInfo::GetLimitTime()
{
	return m_limittime;
}

//1チームの人数を取得
DWORD CBattleLoadInfo::GetNumTeam(DWORD team)
{
	return m_num_team[team];
}

//キャラクタを取得
DWORD CBattleLoadInfo::GetCharacter(DWORD team,DWORD member_index)
{
	return m_char[team][member_index];
}

//コンピュータかどうかを取得
BYTE CBattleLoadInfo::GetKeyAssign(DWORD team,DWORD member_index)
{
	return m_key_assign[team][member_index];
}


//コンピュータかどうかを取得
BOOL CBattleLoadInfo::GetCharacterIsCOM(DWORD team,DWORD member_index)
{
	return m_key_assign[team][member_index] == CASSIGN_COM;
}

//カラーを取得
DWORD CBattleLoadInfo::GetColor(DWORD team,DWORD member_index)
{
	return m_color[team][member_index];
}

//オプションを取得
DWORD CBattleLoadInfo::GetCharacterOption(DWORD team,DWORD member_index)
{
	return m_options[team][member_index];
}

//COMレベル取得
Config2_Difficulty CBattleLoadInfo::GetComLevel(DWORD team,DWORD member_idx)
{
	return m_com_level[team][member_idx];
}

//ステージを取得
DWORD CBattleLoadInfo::GetStage()
{
	return m_stage;
}

//ストーリーで指定されたBGM取得（設定されていない場合NULL）
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
	sprintf(m_serif[0],_T("(台詞が設定されていません)"));
	sprintf(m_serif[1],_T("(台詞が設定されていません)"));
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
//	Set～
//*******************************************************************

void  CBattleResultInfo::SetKatiSerif(DWORD team,char *str)
{
	sprintf( m_serif[team] , str );
}


//*******************************************************************
//	Get～
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




