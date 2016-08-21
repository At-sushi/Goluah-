
/*=======================================================================================

	練習モード キャラセレ

=========================================================================================*/

#include "task_select.h"

/*!
*	@brief PRACTICE モード用キャラセレ
*	@ingroup Tasks
*
*	今はかなり手抜き
*/
class CCharacterSelectPractice : public CCharacterSelect
{
public:
	virtual void Initialize();
	virtual void CreateNextTask();
};


/*!
*	@brief PRACTICE モード用試合条件選択タスク
*	@ingroup Tasks
*
*	プレイヤーに選択させず、固定条件を返すようにした。
*	手抜きクラス。
*
*	本家(1.00)に比べて少し修正が入っているので、今はもう少し作業してます。
*/
class CTConditionSelecterPractice : public CTConditionSelecter
{
public:
	virtual void Initialize();
	virtual BOOL Execute(DWORD time);
	virtual void Draw();

protected:
	void Change(BOOL key);

	static const int m_hp_ratio[5];				//80,100,10,30,50
};