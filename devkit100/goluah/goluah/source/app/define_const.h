
/*============================================================================

	定数・プリプロセッサ 定義

==============================================================================*/
#ifndef _DEFINE_CONST_H_
#define _DEFINE_CONST_H_

#define GOLUAH_SYSTEM_VERSION	(940)
//定数
#define MAXNUM_TEAM				3		//!< 1チーム最大人数
#define MAXNUM_KEYI				6		//!< キー入力最大数
#define GSCMAX_CHARACTERDLL		6		//!< 一度に読み込まれる可能性のあるDLLの数
#define RDMGKANWA				0.9		//!< ダメージ緩和係数
#define DMGKANWA_MINIMUM		0.5		//!< ダメージが緩和される最小値
#define ASPECTRATIO				(320.0f/240.0f)//!< 画面のタテヨコ率
#define MAXNUM_CHARACTERCOLOR	3		//!< キャラクターのパレット最大数。1-3が有効

//キャラ選択時プレイヤー（キー入力番号）割り当て定数
#define CASSIGN_SPECIFIC	0x80						//!< キー入力番号以外の、特別な意味を持つ定数であるというフラグ
#define CASSIGN_NET			0x40						//!< ネットワークキー(未使用)
#define CASSIGN_COM			(CASSIGN_SPECIFIC|0x01)		//!< コンピュータ
#define CASSIGN_NONE		(CASSIGN_SPECIFIC|0x02)		//!< 割り当てなし
#define CASSIGN_STABLE		(CASSIGN_SPECIFIC|0x03)		//!< 操作できない

//定数その2
#define MAXNUM_CDLL				(MAXNUM_TEAM*2)			//!< キャラクタDLL最大数

//!同prefixのほかのフラグとかぶらないように！
#define CHARACTER_CAPS_NET2		0x10000000


//!ストーリースクリプトでの、オプション指定の種類
enum StoryOptType{
	Opt_Random = 0,		//!< ランダム
	Opt_UserSelect = 1,	//!< ユーザーに選ばせる
	Opt_Fix	= 2			//!< 2進数直接指定で固定された
};

//ビットマップテキスト
#define SYSBMPTXT_PROP		0x00000001//!< プロポーショナル
#define SYSBMPTXT_R2L		0x00000002//!< 右から左へ描画する
#define SYSBMPTXT_ALPHAADD	0x00000004//!< 加算合成で描画する
#define SYSBMPTXT_SHADE		0x00000010//!< 影つき(1)
#define SYSBMPTXT_SHADE_W	0x00000020//!< 真っ白影つき
#define SYSBMPTXT_SHADE_B	0x00000030//!< 真っ黒影つき
#define SYSBMPTXT_SHADEMASK 0x000000F0
#define SYSBMPTXT_DEFAULT	(SYSBMPTXT_PROP)

/*--------------------------------------------------------------------------------
	system private effectid （ユーザーに公開しないもの）
----------------------------------------------------------------------------------*/
#define EFCTID_ROUND1		(0x80000000 | 0x00000001)
#define EFCTID_ROUND2		(0x80000000 | 0x00000002)
#define EFCTID_ROUND3		(0x80000000 | 0x00000003)
#define EFCTID_ROUND4		(0x80000000 | 0x00000004)
#define EFCTID_ROUND5		(0x80000000 | 0x00000005)
#define EFCTID_ROUND6		(0x80000000 | 0x00000006)
#define EFCTID_FIGHT		(0x80000000 | 0x00000007)
#define EFCTID_KO			(0x80000000 | 0x00000008)
#define EFCTID_CYOHILIGHT	(0x80000000 | 0x00000009)//超必の光
#define EFCTID_TIMEOVER		(0x80000000 | 0x0000000A)
#define EFCTID_DOUBLEKO		(0x80000000 | 0x0000000B)
#define EFCTID_HATTEN		(0x80000000 | 0x0000000C)//!<隠しエフェクト  (x,y)




/*--------------------------------------------------------------------------------
	例外処理 ON/OFF
	発生個所を詳しく調べたいときはOFFにしてVCのデバッガがとまるのを待つ
----------------------------------------------------------------------------------*/
#ifndef _DEBUG	//catch

#define _CATCH_WHILE_SCENE					//!< シーンTimer()中に例外をキャッチ
#define _CATCH_HANDLEMESSAGE_EXCEPTIONS		//!< メッセージ処理中の例外をキャッチ
#define _CATCH_WHILE_EXEC					//!< タスクexecute処理中の例外をキャッチ
#define _CATCH_WHILE_RENDER					//!< タスクRender処理中の例外をキャッチ

#endif			//catch


/*--------------------------------------------------------------------------------
	デバッグ処理 ON/OFF
	いみないのも多分ある
----------------------------------------------------------------------------------*/
#define _THREAD_POOL						//!< スレッドプールを使用する

#ifdef _DEBUG//デバッグ用定義↓

#define _DBG_MESSAGES_TO_CONSOLE			//!< LogをOutputDebugStringする
#define _DBG_REPORT_NETMESSAGES
#define _DBG_SHOW_LOADCHARACTER_NET
#define _DBG_NET_INITIDLIST
#define _DBG_NET_RSELECT
#define _DBG_NET_SHOW_STG
#define _DBG_SHOW_LOADSTART_INFORMATION
//#define _DBG_SHOW_SYNCMESSAGE
//#define _DBG_NET_EFFECTLIST
//#define _DBG_COMCOMMAND
//#define _DBG_NET_CK_KEYINPUTNUMBER
//#define _DBG_CK_DLLINDEX
//#define _DBG_REPORT_OBJECT_CREATION
#define _DBG_LOG2FILE_SYNC					//!< データSYNC動作時のログをとる

#endif // デバッグ用定義↑


/*--------------------------------------------------------------------------------
	メモリリークチェック
----------------------------------------------------------------------------------*/
#ifdef _DEBUG
#ifndef CRTDBG_MAP_ALLOC
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define MEMLEAK_CK_DEF
#endif
#endif


//配列外アクセスチェック
#ifdef _DEBUG
#define ARRAYBOUNDARY_DEBUG
#endif




#endif//_DEFINE_CONST_H_
