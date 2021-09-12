/*!
*	@file
*	@brief ストーリースクリプト操作
*/
/*!
*	@ingroup Script
*/
/*@{*/
#pragma once

#include "define_const.h"
#include "define_macro.h"
#include "script_loader.h"

class CStoryScriptElement;
typedef std::vector<CStoryScriptElement*> SScriptElementList;

/*!
*	@brief ストーリースクリプト読み込み読み込み
*	@ingroup Script
*/
class CGoluahStoryScript
{
public:
	static BOOL CreateScriptElementList(SScriptElementList& list,char *filename);
	static void DestroyScriptElementList(SScriptElementList& list);
};


/*!
*	@brief ストーリー要素ベース
*	@ingroup Script
*/
class CStoryScriptElement
{
public:
	CStoryScriptElement();
	virtual ~CStoryScriptElement();

	enum CStoryElement_Types
	{
		GSE_ST_ERROR	,
		GSE_ST_STAFF	,
		GSE_ST_VS		,
		GSE_ST_DEMO		,
		GSE_ST_SETTINGS ,
		GSE_ST_SELECT	,
	};

	virtual CStoryElement_Types GetType()=0;	//!<dynamic_castすればいいだけかもネ
	virtual void FeedLine(char *str)=0;			//!<次の定義が見つかったらFALSE
	virtual BOOL Close(){return TRUE;}			//!<次の#が見つかったらコール
	virtual char* GetError(){return m_error;}	//!<エラー取得

protected:
	char *m_error;
	void Error(char *str);
	void ErrorF(char *str,char *name);	//!<副文字列指定
	void ErrorF(char *str,int i);		//!<整数指定
};


/*!
*	@brief ブロック外エラー要素
*	@ingroup Script
*/
class CStoryElement_Error : public CStoryScriptElement
{
public:
	CStoryElement_Error(char *err);
	
	CStoryElement_Types GetType(){return GSE_ST_ERROR;}
	void FeedLine(char *str){}
	BOOL Close(){return TRUE;}
};


/*!
*	@brief #settings要素
*	@ingroup Script
*/
class CStoryElement_Settings : public CStoryScriptElement
{
public:
	CStoryElement_Settings();
	~CStoryElement_Settings();

	CStoryElement_Types GetType(){return GSE_ST_SETTINGS;}
	void FeedLine(char *str);
	BOOL Close();

public:
	//全般
	float ver;						//!<スクリプトのバージョン（-1:未設定）
	char* title;					//!<ストーリータイトル
	char* brief;					//!<ストーリー概要
	char* icon;						//!<アイコンファイル名
	char* preview;					//!<プレビューファイル名
	int continue_num;				//!<コンティニュー可能回数(マイナスで∞)
	int result_send;				//!<試合結果送信対象(マイナスで対象無し)
	int result_key;					//!<試合結果送信時に付属させるキー番号

	//キャラクタ定義
	int cnum;						//!<char指定数
	int characters[MAXNUM_TEAM];	//!<char指定　(-1:userselect -2:指定なし)
	int color[MAXNUM_TEAM];			//!<色指定
	DWORD option[MAXNUM_TEAM];		//!<オプション直接指定(userselect以外で有効)
	StoryOptType opttype[MAXNUM_TEAM];	//!<オプション指定の種類(userselect以外で有効)

protected:
	void Read_char(char *str);
	BOOL multiline_flag;
};


/*!
*	@brief #staff要素
*	@ingroup Script
*/
class CStoryElement_Staff : public CStoryScriptElement
{
public:
	CStoryElement_Staff();
	~CStoryElement_Staff();

	CStoryElement_Types GetType(){return GSE_ST_STAFF;}
	void FeedLine(char *str);

public:
	char *m_filename;				//!<NULLの場合 system/staff.txt
};


/*!
*	@brief #vs要
*	@ingroup Script
*/
class CStoryElement_VS : public CStoryScriptElement
{
public:
	CStoryElement_VS(CStoryElement_Settings* sets);
	~CStoryElement_VS();

	CStoryElement_Types GetType(){return GSE_ST_VS;}
	void FeedLine(char *str);
	BOOL Close();

//	void  SetupBattleInfo();

public:
	//キャラクタ関連
	int num[2];							//!<2チームのキャラクタ数
	int characters[2][MAXNUM_TEAM];		//!<-1:ランダム -2:char1 -3:char2 -4:char3 -5>none
	int color[2][MAXNUM_TEAM];			//!<-1:ランダム
	DWORD option[2][MAXNUM_TEAM];		//!<オプション直接指定
	BOOL is_random_opt[2][MAXNUM_TEAM];	//!<ランダム指定フラグ

	//COMレベル関連
	int level[2][MAXNUM_TEAM];				//!<レベル指定
	BOOL is_related_level[2][MAXNUM_TEAM];	//!<相対レベル指定フラグ

	//ステージ・試合条件等
	int type;						//!<試合形式
	int stage;						//!<ステージ -1:ランダム
	int limit_time;					//!<制限時間(マイナスでSETINGS値継承)
	char *bgm_filename;				//!<BGM指定(NULLで指定無し)
	int bgm_startpos;				//!<BGM再生開始位置(0で通常再生)

	//試合後設定
	char *text_win;					//!<勝利台詞指定
	char *text_lose;				//!<負けたときの、敵の勝利台詞指定
	BOOL show_win;					//!<勝利画面に移行するか？
	BOOL nogameover;				//!<負けてもゲームオーバーしない
	int sel_shift;					//!<nogameover時、選択変数のシフト量

protected:
	void Read_friend(char *str,int team=0);
	void Read_enemy(char *str);
	void Read_stage(char *str);

	//スクリプト解釈中に使用
	CStoryElement_Settings* m_sets;	//!<#settings項目 %char1-3使用時に定義がされているかどうかチェックするため
	UINT multiline_flag;			//!<複数行フラグ。 0:無効 1:text_winに追加する 2:text_loseに追加する
};


/*!
*	@brief #demo要素
*	@ingroup Script
*/
class CStoryElement_Demo : public CStoryScriptElement
{
public:
	CStoryElement_Demo();
	~CStoryElement_Demo();

	CStoryElement_Types GetType(){return GSE_ST_DEMO;}
	void FeedLine(char *str);
	BOOL Close();

public:
	char *m_filename;		//!<NULL→未設定→エラー

	/*
	//引数リスト保管・取得
	struct STRING_PEAR{char *str,*alias;};
	typedef std::list<STRING_PEAR> aliaslist;
	aliaslist args;
	CAliasList* CreateAliasClass();
	*/
};


/*!
*	@brief #sselect要素
*	@ingroup Script
*/
class CStoryElement_Select : public CStoryScriptElement
{
public:
	CStoryElement_Select(int selectnum)	{ m_selectnum = selectnum; }
	~CStoryElement_Select(){}

	CStoryElement_Types GetType(){return GSE_ST_SELECT;}
	void FeedLine(char *str){}
	int GetNum()	{ return m_selectnum; }

public:
	int m_selectnum;

	/*
	//引数リスト保管・取得
	struct STRING_PEAR{char *str,*alias;};
	typedef std::list<STRING_PEAR> aliaslist;
	aliaslist args;
	CAliasList* CreateAliasClass();
	*/
};



/*@}*/
