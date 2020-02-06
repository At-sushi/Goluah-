/*----------------------------------------------------------------------

	ポーズ画面

------------------------------------------------------------------------*/

#include "task.h"
#include "dx_draw.h"

/*!
*	@brief 試合をポーズしたときの操作・描画を行う
*	@ingroup Battle
*/
class CTBattlePause : public CTaskBase
{
public:
	void Initialize();						//ExecuteまたはDrawがコールされる前に1度だけコールされる
	void Terminate();						//タスクのリストから外されるときにコールされる（その直後、deleteされる）
	BOOL Execute(DWORD time);				//毎フレームコールされる
	void Draw();							//描画時にコールされる
	int GetDrawPriority(){return 500;}		//描画プライオリティ。低いほど手前に（後に）描画。マイナスならば表示しない

	void Kill()	{m_kill_flag=TRUE;}

protected:
	LPDIRECT3DTEXTURE9 tex_fb;				//!< フロントバッファをコピーしたテクスチャ
	LPDIRECT3DTEXTURE9 tex_pause;			//!< "Pause Press F7 Key" 表示用テクスチャ

	UINT m_counter;
	UINT m_face_counter[2];
	UINT m_face_idx[2];
	BOOL m_kill_flag;
	BOOL m_inst_on[2];
	int m_shiftY;
};

