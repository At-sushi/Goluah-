
/*!
*	@file
*	@brief グローバル関数 グローバル変数　など
*
*	実体は global.cpp にて定義する
*/
#pragma once

/*!
*	@defgroup global
*	@brief グローバルな関数とか変数とか
*
*	主なものはキャラクターのリスト、ステージのリスト、ストーリーのリスト、
*	config.exeの設定、試合開始前後の設定など。
*/

/*------------------------------------------------------------------------------
	グローバルオブジェクト(クラス)
--------------------------------------------------------------------------------*/

#include "stdafx.h"
#include "dx_draw.h"
#include "dx_input.h"
#include "dx_sound.h"
#include "dx_play.h"

#include "system.h"

class AkiGlobal;
#include "config.h"
#include "log2file.h"
#include "export.h"
#include "storylist.h"
#include "stagelist.h"
#include "charlist.h"
#include "battle_load_info.h"

extern CDirectDraw			g_draw;			//!< 描画オブジェクト
extern CDirectInput			g_input;		//!< 入力オブジェクト
extern CDirectSound			g_sound;		//!< 音系オブジェクト
extern CDirectPlay			g_play;			//!< 通信オブジェクト　消滅

extern CSystem				g_system;		//!< システムオブジェクト

extern AkiGlobal			gbl;			//!< グローバル関数の入れ物
extern CLog2File			g_log2file;		//!< ファイルへログ保存クラス
extern CExport				g_exp;			//!< DLLへの関数提供クラス
extern CStoryList			g_storylist;	//!< ストーリーリスト管理クラス
extern CStageList			g_stagelist;	//!< ステージリスト管理クラス
extern CCharacterList		g_charlist;		//!< キャラクターリスト管理クラス
extern CBattleLoadInfo		g_battleinfo;	//!< 試合情報クラス
extern CBattleResultInfo	g_battleresult;	//!< 試合結果情報クラス


/*------------------------------------------------------------------------------
	グローバル変数 extern宣言
--------------------------------------------------------------------------------*/
extern TCHAR AppName[];			//!< アプリケーション名
extern BOOL g_programexit;		//!< メインループ終了フラグ
extern RECT g_rcClient;			//!< window modeのときに必要(?)
extern int g_DISPLAYWIDTH;		//!< 生成ウインドウサイズ（幅）
extern int g_DISPLAYHEIGHT;		//!< 生成ウインドウサイズ（高さ）
extern BOOL g_muki[MAXNUM_KEYI];//!< キー入力の向き


/*------------------------------------------------------------------------------
	グローバル関数
--------------------------------------------------------------------------------*/

class AkiGlobal;

struct MY2DVECTOR;
#include "bigface_cache.h"

/*!
*	@ingroup global
*	@brief 雑用いろいろクラス
*
*	CSystem同様、雑用が主な仕事です。
*	グローバル関数でもいいけど、VCのコード補完がうまい具合に動かないので
*	ここに入れているというのもある。gbl. と打ったらコード補完発動。
*/
class AkiGlobal{
public:
	AkiGlobal();
	~AkiGlobal();

	//!指定ウインドウをディスプレイの中心へ持っていく
	static int SetWinCenter(HWND hWnd);

	//!ファイルがあるかどうかを調べる
	static BOOL FileExist(TCHAR *path);

	/*!
		@brief 矩形の衝突判定
		回転がある場合時々0除算を行うみたい。
		あんまりちゃんと動作確認してません
	*/
	static BOOL Syototu2RECTs(RECT r1,RECT r2,MY2DVECTOR *point,
			int gx1,int gy1,BOOL rx1,BOOL ry1,int rot1,double tx1,double ty1,double mx1,double my1,
			int gx2,int gy2,BOOL rx2,BOOL ry2,int rot2,double tx2,double ty2,double mx2,double my2,
			UINT magmode1,UINT magmode2);

	/*!
		@brief 2文字列を結合して新しいバッファを返す
		戻り値はnew[]で確保したバッファ。str1はdelete[]されます。
	*/
	static TCHAR* MergeString(TCHAR* str1,const TCHAR* str2);

	/*!
		@brief ダミー用TCHARバッファを取得。
		主にスクリプトテキストのロード時に使われています。怪しい機能。
	*/
	TCHAR*  GetDummyString();

	TCHAR** CreateTemporaryStrBuff(UINT num,UINT max_len=128);	//!< テンポラリ文字列配列の破棄
	void   DeleteTemporaryStrBuff(TCHAR** buf);					//!< テンポラリ文字列配列の破棄

	//!指定バッファの先頭に、指定文字列が入っているかどうかチェック
	BOOL strcheck(const TCHAR* buf,const TCHAR* str);

	//!デカ顔イメージ(face2.bmp)取得
	MYSURFACE* GetBigFace(int cindex,int col,int alt);

	//!ブランクアイコン取得
	MYSURFACE* GetBlankIcon();

	//!実数ランダム(0〜1)
	float RandomOne();

	//!指定ディレクトリ下からランダムなBGM再生
	void PlayRandomBGM(TCHAR *dir);

	
	void ods(const TCHAR *format, ...);	//!OpuputDebugStringする。改行を付加
	void ods2(const TCHAR *format, ...);	//!OpuputDebugStringする。改行を付加しない

protected:
	//文字列関係
	TCHAR** m_dummybuf;
	UINT m_dummy_next;

	std::list<TCHAR**> m_tempstrlist;

	TCHAR** CreateStringArray(UINT num,UINT max_len);
	void   DeleteStringArray(TCHAR** pstrarr);

	TCHAR *m_ods_buffer;

	//!ブランクアイコン
	MYSURFACE* m_blankIcon;
};

//!2Dベクトル構造体。矩形衝突判定関数で使用
struct MY2DVECTOR {double x;double y;};


/*!
*	@ingroup global
*	@brief ファイルアクセス支援クラス？
*
*	ファイルへのアクセスを簡略化したくて作ったクラス。
*	あとから作ったのであんまり使われていないし、むしろMFCのCFileを使ったほうが良い。
*/
class AkiFile
{
public:
	AkiFile();
	~AkiFile(){Destroy();}
	BOOL Load(TCHAR *filename);
	BOOL Save(TCHAR *filename);

	void	Set(DWORD size,BYTE* ptr=NULL);

	DWORD	m_size;
	BYTE*	m_buffer;

protected:
	void Destroy();//!< Set時の旧メモリ破棄,デストラクタ　共通操作
};


#ifndef _aki3d_h_
//!ARGB ←→ DWORD 変換クラス
struct ARGB
{
	union
	{
		DWORD dwcol;
		struct
		{
			BYTE blue;
			BYTE green;
			BYTE red;
			BYTE alpha;
		};
	};
};
#define ARGB_STRUCT
#endif
