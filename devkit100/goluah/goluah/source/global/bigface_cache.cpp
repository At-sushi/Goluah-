
/*=======================================================================
	デカ顔キャッシュ
	表示に必要なくなったものもアンロードせずに保持しておいて
	読み込み回数を軽減する
=========================================================================*/
#pragma once

#include "stdafx.h"
#include "global.h"
#include "bigface_cache.h"

#define MAX_BIGFACE_CACHE	(MAXNUM_TEAM*2+3)


/*
	取得

	@param cindex キャラクターインデックス。マイナスの場合ランダム用のフェイスを返す
	@param col 色番号。パレットを決定
	@param alt キャラクターのオプションにより決定される　face2変更値
	@return 指定画像を読み込んだインチキサーフェイス
*/
MYSURFACE* CTBigFaceCache::Get(int cindex,int col,int alt)
{
	if(cindex>g_charlist.GetCharacterCount())return NULL;
	if(cindex<0){//ランダムの場合はみんないっしょ
		cindex=-1;
		col=0;//ランダムの場合、カラーは関係ない
		alt=0;//altも関係ない
	}

	//すでに読み込んであるものの中から探す
	texref_list::iterator i =list.begin();
	texref_list::iterator ie=list.end();
	for(;i!=ie;i++){
		if((*i)->cindex==cindex && (*i)->col==col && (*i)->alt==alt)//ヒット
		{
			CTextureRef* hit = *i;
			hit->crnt_ref = TRUE;
			list.erase(i);
			list.push_back(hit);//見つかったものをリストの末尾に移動する
			return hit->dds;
		}
	}

	gbl.ods(_T("CTBigFaceCache::Get , miss hit %d,%d,%d"),cindex,col,alt);

	//ない場合はしょうがないのでファイルからロードする
	MYSURFACE *dds=NULL;
	if(cindex>=0){
		char *filepath = new char[MAX_PATH];
		char *palpath = new char[MAX_PATH];
		char altstr[2]={'\0','\0'};
		if(alt!=0){
			altstr[0]='a'+alt-1;
		}
		sprintf(filepath,_T("%s\\face2%s"),g_charlist.GetCharacterDir(cindex),altstr );
		sprintf(palpath,_T("%s\\pal%d"),g_charlist.GetCharacterDir(cindex),col);
		dds = g_draw.CreateSurfaceFrom256Image(filepath,palpath);
		gbl.ods(_T("CTBigFaceCache : load %s"),filepath);
		delete [] filepath;
		delete [] palpath;
	}
	else{//ランダム用
		dds = g_draw.CreateSurfaceFrom256Image(_T(".\\system\\rand_face"));
	}

	//リストの末尾に追加する
	CTextureRef* nitem = new CTextureRef;
	nitem->dds         = dds;
	nitem->cindex      = cindex;
	nitem->col         = col;
	nitem->alt		   = alt;
	list.push_back(nitem);

	return dds;
}

/*!
*	全リスト破棄
*	BGタスクを継承しているため、システムのシャットダウン時に自動的に呼ばれる。
*/
void CTBigFaceCache::Terminate()
{
	texref_list::iterator i =list.begin();
	texref_list::iterator ie=list.end();
	for(;i!=ie;i++){
		delete (*i);
	}
	list.clear();
}


/*!
*	更新
*	実際読み込まれているビットマップ数は、最大数を瞬間的に超えることがある
*	（何かの間違いで毎フレームビットマップ読み込みというような事態にならないよう）
*	その場合、現在フレームで要求のなかったビットマップを削除する。
*/
BOOL CTBigFaceCache::Execute(DWORD time)
{
	texref_list::iterator i,ie;

	//最大数チェック
	int max_over = list.size() - MAX_BIGFACE_CACHE;

	//リストが最大数を超えている場合、参照のなかった項目を削除する
	if(max_over>0){
		i =list.begin();
		ie=list.end();
		for(;i!=ie;)
		{
			if( !((*i)->crnt_ref || (*i)->prev_ref) ){
				delete (*i);
				i = list.erase(i);
				max_over--;
			}
			else i++;
			if(max_over==0)break;
		}
	}

	//参照フラグ更新
	i =list.begin();
	ie=list.end();
	for(;i!=ie;i++)
	{
		(*i)->prev_ref = (*i)->crnt_ref;
		(*i)->crnt_ref = FALSE;
	}

	return TRUE;
}


/*------------------------------------------------------------------------
	内部クラスの構築と破棄
--------------------------------------------------------------------------*/
CTBigFaceCache::CTextureRef::CTextureRef()
{
	dds=NULL;
	prev_ref=TRUE;
	crnt_ref=TRUE;
}

CTBigFaceCache::CTextureRef::~CTextureRef()
{
	RELSURFACE(dds);
}

