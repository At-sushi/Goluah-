
/*===============================================================

	ストーリーモードタスク

=================================================================*/
#pragma once

#include "task.h"
#include "script_story.h"

class CTaskStoryMode;	//ストーリーモード本体
class CTStoryContinue;	//コンティニュータスク
class CTStoryGameOver;	//ゲームオーバータスク


/*!
*	@brief ストーリーモード本体
*	@ingroup Tasks
*
*	Activate時に前タスクや、ストーリークラスの情報から
*	判断して適切な次のタスクを起動する。
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
	UINT m_story_index;					//!<ストーリー番号
	UINT m_keyindex;					//!<プレイヤーのキー入力番号
	CStoryList::CStoryInfo* m_setting;	//!<ストーリー初期設定
	SScriptElementList scr_list;		//!<スクリプトリスト
	SScriptElementList::iterator scr_i;	//!<現在のスクリプトブロック
	int m_continue_num;					//!<コンティニュー回数

	DWORD m_prev_task_id;				//!<前回タスクID
	int selectflag;						//!<選択肢フラグ
	bool in_select;						//!<select分岐後かどうか

	//タスク起動
	void StartNextTask();				//!<次のストーリー要素の開始
	void StartVSTask();					//!<試合
	void StartDemoTask();				//!<デモ
	void StartStaffTask();				//!<スタッフロール
	void StartContinueTask();			//!<コンティニュー

	/*!
	*	@brief ランダムキャラクター取得クラス
	*	@ingroup Tasks
	*
	*	ストーリー中でなるべく同じキャラクターが出ないようにするために使用。
	*/
	class CRandomCharacterGetter
	{
	public:
		void Initialize(SScriptElementList& scr_list);
		void Req(UINT num);	//!< 次の試合で,指定した数のランダムキャラクターが必要。足りない場合はシャッフルしなおす
		UINT GetNext();		//!< 次のランダムキャラクターを取得する

		int chars[MAXNUM_TEAM];//!< settingsブロックで指定されたキャラクター

	protected:
		void EraseFromAllList(UINT n);
		typedef std::vector<UINT> CIndexList;
		CIndexList all_list;
		CIndexList crnt_list;
	};
	CRandomCharacterGetter m_RandomCharacterGetter;

	/*!
	*	@brief ランダムステージ取得クラス
	*	@ingroup Tasks
	*
	*	ストーリー中でなるべく同じステージが出ないようにするためのクラス。
	*/
	class CRandomStageGetter
	{
	public:
		void Initialize(SScriptElementList& scr_list);
		UINT GetNext();		//!< 次のランダムステージを取得する

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
*	@brief ストーリーモード時・コンティニュー
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
*	@brief ストーリーモード時・ゲームオーバー
*	@ingroup Tasks
*/
class CTStoryGameOver : public CTStoryContinue
{
public:
	BOOL Execute(DWORD time);
	void Draw();
};
