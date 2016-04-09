
/*========================================================================

	ストーリー管理

==========================================================================*/
#pragma once

#include "define_const.h"
#include "script_story.h"

class CStoryElement_Settings;

/*!
*	@ingroup global
*/
/*@{*/

/*!
*	@brief ストーリーリスト保持
*	Initializeはキャラクターリスト・ステージリストが構築された後に行うこと
*/
class CStoryList
{
public:
	struct CStoryInfo;

	void Initialize();

	UINT  GetAllStoryNum();							//!< 全ストーリー数を取得
	void  StartStory(UINT index);					//!< 指定インデックスのストーリーを開始する
	TCHAR* GetStoryName(UINT index);					//!< 指定インデックスのストーリー名を取得する
	TCHAR* GetStoryBrief(UINT index);				//!< 指定インデックスのストーリー概要を取得する
	TCHAR* GetStoryDir(UINT index);					//!< 指定インデックスのストーリーディレクトリを取得する
	void GetStoryIconPath(UINT index,TCHAR* dst);	//!< 指定インデックスのストーリーアイコンファイル名を取得する
	void GetStoryPreviewPath(UINT index,TCHAR* dst);	//!< 指定インデックスのストーリープレビューファイル名を取得する
	CStoryInfo* GetStoryInfo(UINT index);			//!< 指定インデックスのストーリー情報を取得

	int   Ring2Index(UINT ridx,UINT idx);			//!< ディレクトリインデックス+ディレクトリ内インデックスから、全体通し番号を取得
	UINT  GetRingNum();								//!< ディレクトリ数取得
	int   GetRingCount(UINT idx);					//!< 指定インデックスのディレクトリ内にあるストーリー数取得

	int   GetErrorCount();							//!< 読み込みに失敗したストーリー数取得
	TCHAR* GetErrorStr(UINT idx);					//!< 読み込み失敗理由取得
	TCHAR* GetErrorDir(UINT idx);					//!< 読み込み失敗ディレクトリ


	/*
		@brief ストーリーリスト要素定義
	*/
	struct CStoryInfo
	{
		TCHAR dir[128];					//!< 実行ファイルからの相対パス
		TCHAR name[64];
		TCHAR icon[64];
		TCHAR preview[64];
		TCHAR brief[512];

		//キャラクタ定義
		int cnum;						//!< TCHAR指定数
		int characters[MAXNUM_TEAM];	//!< TCHAR指定　(-1:userselect -2:指定なし)
		int color[MAXNUM_TEAM];			//!< 色指定
		DWORD option[MAXNUM_TEAM];		//!< オプション直接指定(userselect以外で有効)
		StoryOptType opttype[MAXNUM_TEAM];//!< オプション指定フラグ(userselect以外で有効)

		void Clear();
		void Setup(CStoryElement_Settings* settings,TCHAR* dir1,TCHAR* dir2);
		void SetDir(TCHAR *s,TCHAR *s2);
		void SetName(TCHAR *s);
		void SetBrief(TCHAR *s);
		void SetIcon(TCHAR *s);
		void SetPreview(TCHAR *s);

		CStoryInfo* Clone();
	};
	typedef std::vector<CStoryInfo> CStoryInfoList;

protected:
	//!ディレクトリリスト要素
	struct CStoryRingInfo
	{
		TCHAR dir[64];
		std::vector<DWORD> ring2serial;//!< リング内でのインデックス→全体通し番号インデックス変換
	};
	typedef std::vector<CStoryRingInfo> CStoryRingInfoList;


	CStoryInfoList     list;
	CStoryInfoList     dlist;//!< 検証失敗リスト。brief にダメ理由を入れる。
	CStoryRingInfoList rlist;

protected:

	//スクリプト解釈後の要素リストからの各種情報取り出し
	static TCHAR* GetName(SScriptElementList& scr);
	static TCHAR* GetBrief(SScriptElementList& scr);
	static int*  GetCharacters(SScriptElementList& scr);
	static TCHAR* GetFirstError(SScriptElementList& scr);
	static CStoryElement_Settings* GetSettingsFromScriptList(SScriptElementList& scr);
};



/*@}*/