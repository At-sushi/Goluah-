#include "task_battle.h"

/*!
*	@brief	デモ画面用戦闘タスク
*	@ingroup Battle
*
*	タイトル画面でしばらく放っておくと始まるデモ画面のタスク。
*	重さ対策のため、１チームのキャラ数を少し制限してます。
*/
class CBattleTaskDemo :
	public CBattleTask
{
public:
	virtual ~CBattleTaskDemo(void){}

	virtual void Initialize();
	virtual BOOL Execute(DWORD time);

protected:
	virtual void Setting_BattleType();			//!< 対戦形式を設定
	virtual void Setting_Characters();			//!< キャラクターを設定
	virtual void Setting_Stage();				//!< ステージを設定
	virtual void Setting_Other();				//!< その他？時間とか。

	enum{
		DEMOBATTLE_MAXTEAM = 2,					//!< チームごとの最大人数
		DEMOBATTLE_LIMITTIME = 3000				//!< デモが終わるまでの時間
	};

	void InitRandomCharacters();				//!< m_randchar に適切な値を選択する
	UINT m_randchar[2][DEMOBATTLE_MAXTEAM];		//!< (なるべく)重複しないように選択したランダムキャラ
};
