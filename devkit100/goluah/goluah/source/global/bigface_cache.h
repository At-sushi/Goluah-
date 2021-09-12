
/*!
*	@file
*	@brief デカ顔キャッシュ
*	表示に必要なくなったものもアンロードせずに保持しておいて
*	読み込み回数を軽減する
*/
#pragma once

#include "define_gcd.h"
#include "task.h"

#define BIGFACE_TASKID	'BFCT'

/*!
*	@ingroup global
*	@brief デカ顔取得クラス
*
*	デカ顔(face2.bmp)を表示するときに、いちいちビットマップのロードとアンロードを
*	管理するのが面倒なので、こいつから取得するようにした。
*	要求されたビットマップがすでに読み込まれている場合はロードしないでそれを返す。
*	明示的なアンロードは存在しないので、常時結構な数のビットマップがロードされっぱなし。
*/
class CTBigFaceCache : public CBackgroundTaskBase
{
public:
	~CTBigFaceCache(){Terminate();}
	
	BOOL Execute(DWORD time);			//!< 1フレ更新タイミング
	void Terminate();
	DWORD GetID(){return BIGFACE_TASKID;}
	
	MYSURFACE* Get(int cindex,int col,int alt);
	void Destroy();

protected:
	//! face2ビットマップ情報管理,1枚分
	struct CTextureRef
	{
		MYSURFACE *dds;
		int cindex;
		int col;
		int alt;
		BOOL prev_ref;
		BOOL crnt_ref;

		CTextureRef();
		~CTextureRef();
	};
	typedef std::list<CTextureRef*> texref_list;

	texref_list list;
};
