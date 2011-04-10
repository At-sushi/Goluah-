/*----------------------------------------------------------------------

	ロード画面

------------------------------------------------------------------------*/

#pragma once

#include "dx_draw.h"
#include "task.h"

enum NowLoading_IconItem
{
	NowLoading_DLL,
	NowLoading_Image,
	NowLoading_GCD,
};

/*!
*	@brief 試合前NowLoadingクラス
*	@ingroup Battle
*
*	DLL側に提供されている, BMP,GCD読み込み関数が呼び出されたときと
*	戦闘タスクがDLLをロードしたときにProceedが呼ばれ、描画が1ステップだけ行われる。
*	このときメインループは廻っていないので、描画の開始と終了の処理を自前でProceed関数中で行っている。
*/
class CTNowLoading : public CTaskBase
{
public:
	void Initialize();						//ExecuteまたはDrawがコールされる前に1度だけコールされる
	BOOL Execute(DWORD time){return(TRUE);}	//毎フレームコールされる
	void Terminate();						//タスクのリストから外されるときにコールされる（その直後、deleteされる）
	void Draw(){}							//!<Proceed()内で、非標準の手続きで描画するので、ここでは描画を行わない

	DWORD GetID(){return 'LOAD';}			//0以外を返すようにした場合、マネージャにAddしたとき同じIDを持つタスクを破棄します（主に常駐タスク用）
	int GetDrawPriority(){return -1;}		//!<Proceed()内で、非標準の手続きで描画するので、描画を行わない
	
	//!描画を1ステップだけ進める
	void Proceed(NowLoading_IconItem item);
	
	//!プログレスバー表示
	void Progress(NowLoading_IconItem item, float value);

protected:
	void DrawIcon();						//!ロード済みのアイコンを表示

	LPDIRECT3DTEXTURE8 tex_fb;				//!< フロントバッファをコピーしたテクスチャ
	LPDIRECT3DTEXTURE8 tex_nowload;			//!< "Now Loading"表示用テクスチャ
	LPDIRECT3DTEXTURE8 tex_dll;				//!< DLLアイコン表示用テクスチャ
	LPDIRECT3DTEXTURE8 tex_img;				//!< IMGアイコン表示用テクスチャ
	LPDIRECT3DTEXTURE8 tex_gcd;				//!< GCDアイコン表示用テクスチャ
	MYVERTEX3D vb[4];						//!< テクスチャ描画用の頂点リスト
	LPDIRECT3DTEXTURE8 tex_AA;				//!< ＡＡ表示用テクスチャ
	

	std::vector<NowLoading_IconItem> m_iconlist;
};