/*!
	@file
	@brief Battle Load Information

	戦闘タスク開始に必要な各種情報
	ついでに、戦闘タスク終了時の情報

*/
#pragma once


#define BLI_NOT_SELECTED	(0xFFFFFFFF)

/*!
	@ingroup global
	@brief 戦闘タスク開始に必要な情報保持クラス
	グローバルに1つだけ存在して、タスク間で試合情報を受け渡すためのクラス。
*/
class CBattleLoadInfo
{
public:
	void Initialize();		//!<全項目を指定無しに設定する
	void ResolveRandom();	//!<指定無し項目をランダム指定とみなし、有効な値を割り当てる

	//Set系
	void SetBattleType(BYTE type);								//!<対戦形式を指定（TAISENKEISIKI_～）
	void SetLimitTime(int val);									//!<制限時間設定
	void AddCharacter(  										//!<キャラクタ追加
						DWORD team,
						DWORD cindex,
						DWORD color,
						BYTE  key,
						DWORD option);
	void SetStage(DWORD stage_index);							//!<ステージを設定
	void SetStoryBGM(char *filepath);							//!<ストーリーから指定されたBGM
	void SetComLevel(DWORD team,DWORD cindex,					//!<COMのレベル
					int level,BOOL is_related);

	//Get系
	DWORD GetBattleType();										//!<対戦形式を取得
	int   GetLimitTime();										//!<制限時間取得
	DWORD GetNumTeam(DWORD team);								//!<1チームの人数を取得
	DWORD GetCharacter(DWORD team,DWORD member_index);			//!<キャラクタを取得
	BYTE  GetKeyAssign(DWORD team,DWORD member_index);			
	BOOL  GetCharacterIsCOM(DWORD team,DWORD member_index);		//!<コンピュータかどうかを取得
	DWORD GetColor(DWORD team,DWORD member_index);				//!<カラーを取得
	DWORD GetCharacterOption(DWORD team,DWORD member_index);	//!<オプションを取得
	DWORD GetStage();											//!<ステージを取得
	char* GetStoryBGM();										//!<ストーリーで指定されたBGM取得（設定されていない場合NULL）
	Config2_Difficulty GetComLevel(DWORD team,DWORD member_idx);//!<COMレベル取得

	DWORD GetAllKey();//!<試合に関わってる全プレイヤーキー入力の和をゲット

protected:
	int	  m_limittime;					//!<制限時間
	DWORD m_type;						//!<対戦形式
	DWORD m_num_team[2];				//!<1チーム何人か
	DWORD m_char[2][MAXNUM_TEAM];		//!<選択されたキャラクター
	DWORD m_options[2][MAXNUM_TEAM];	//!<選択されたオプション設定
	DWORD m_color[2][MAXNUM_TEAM];		//!<選択されたキャラクター色
	BYTE  m_key_assign[2][MAXNUM_TEAM];	//!<コンピュータかどうか
	DWORD m_stage;						//!<選択されたステージ
	char  m_storybgm[256];				//!<ストーリーから指定されたBGM
	Config2_Difficulty m_com_level[2][MAXNUM_TEAM];//!<コンピュータ難易度。g_configから取得した値+ストーリーモードでの指定値
};


/*!
	@ingroup global
	@brief 戦闘タスク終了時の情報
	CBattleLoadInfo と同じような位置づけで、こっちは勝敗や
	勝利せりふ等、終了時の情報を保持する
*/
class CBattleResultInfo
{
public:
	CBattleResultInfo();

	void Initialize(BYTE wteam);						//!<CBattleLoadInfoから情報をコピーして準備する
	void SetKatiSerif(DWORD team,char *str);			//!<勝利台詞設定

	BOOL  GetWinner();									//!<勝利チームゲト
	UINT  GetWinnerCount(){return m_winner_count;}		//!<勝利側チーム人数
	char* GetKatiSerif(DWORD team=3);					//!<台詞ゲト
	DWORD GetCharacter(DWORD index);					//!<キャラクタゲト
	DWORD GetColor(UINT index);							//!<勝った方のカラー

protected:
	BYTE  m_winner;					//!<勝利チーム(0or1)
	char  m_serif[2][256];			//!<台詞

	DWORD m_character[MAXNUM_TEAM];	//!<キャラクタ
	UINT  m_color[MAXNUM_TEAM];		//!<カラー
	UINT  m_winner_count;			//!<勝利側チーム人数
};

