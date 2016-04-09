
/*===========================================================

	勝利画面

=============================================================*/
#pragma once

#include "task.h"

/*!
*	@brief 勝利画面タスク
*	@ingroup Tasks
*
*	VSモードタスクや、ストーリーモードタスクがこれを起動する。
*	勝利台詞や面子などはグローバルのg_battleinfoやg_battleresultから取得
*/
class CYouWin : public CExclusiveTaskBase
{
public:
	CYouWin();
	~CYouWin();

	void Initialize();
	BOOL Execute(DWORD time);
	void Terminate();
	void Draw();

	DWORD GetID(){return 'Win';}
	int GetDrawPriority(){return 1000;}

	void SetStoryText(TCHAR *txt);

protected:
	void CleanUp();

//変数
protected:
	DWORD counter;				//!< Execute用カウンタ
	BOOL  m_end_flag;			//!< 終了フラグ
	UINT  m_textlen;			//!< 表示テキスト長
	DWORD wt;					//!< 勝った方のチーム
	MYSURFACE *dds_face[3];		//!< 表示顔

	LPDIRECT3DTEXTURE9 tex_fb;	//!< 試合終了時のフロントバッファのコピー

	int f_x[3];					//!< face2の表示位置
	float f_z[3];				//!< face2の表示位置
	BOOL show_text;				//!< テキスト表示するかどうか
	DWORD jun[3];				//!< 表示の順番

	TCHAR *m_text_from_story;	//!< ストーリー側から指定されたテキスト
	TCHAR *m_text;				//!< m_text_from_storyまたはg_battleresultから取得したテキストのうち、実際に表示するほう。
};

