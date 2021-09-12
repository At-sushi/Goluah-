
/*!
*	@file
*	@brief DLLインターフェース定義
*	GOLUAH キャラクター・ステージDLL のビルドに必要な各種構造体の定義
*/
/*!
*	@defgroup GObject
*	@brief システム⇔DLLインターフェース定義
*	システム(本体実行ファイル)と、DLLの間で交換される各種構造体・定数の定義です。
*	DLL製作者はこれらの定義を変更することはできません。
*/
/*!
*	@ingroup GObject
*/
/*@{*/
#pragma once


//GCDフォーマット定義が必要
#include "define_gcd.h"
#define _GOBJECT_H_

/*!
*	@brief 攻撃力情報構造体
*	@ingroup GObject
*
*	キャラクター間、キャラクター⇔システム間でやり取りする。
*	主にポインタ渡しで扱うので、DLLは構造体の実体を破棄されないような
*	適切な場所に保持すること。
*/
struct ATTACKINFO
{
	DWORD hit;		//!< ヒット情報 @sa HITINFO_FLAGS
	DWORD guard;	//!< ガード情報 @sa GUARDINFO_FLAGS
	DWORD id;		//!< DLL側で自由に使用できる識別ID。システム側では関与しません。

	DWORD damage;	//!< ダメージ値
	DWORD kezuri;	//!< 削り値

	BOOL muki;		//!< 相手がのけぞる向き(FALSEならば、自分が向いている方向と同じ)
};

//hit

/*!
*	@brief ヒット情報フラグ
*	@ingroup GObject
*	@sa ATTACKINFO
*
*	ATTACKINFO::hit に設定します。攻撃が当たったときの各種設定。
*/
enum HITINFO_FLAGS
{
	//占有ビット					0x0FFFFFFF

	HITINFO_REACT1			=	0x00000001,//!< 普通の弱のけぞり
	HITINFO_REACT2			=	0x00000002,//!< 普通の中のけぞり
	HITINFO_REACT3			=	0x00000003,//!< 普通の強のけぞり
	HITINFO_DOWN			=	0x00000004,//!< ダウソする（転ぶ）
	HITINFO_FUTTOBI			=	0x00000005,//!< 吹っ飛ぶ
	HITINFO_FUTTOBI2		=	0x00000006,//!< 吹っ飛ぶ（ちょっと上のほうへ）
	HITINFO_REACT1A			=	0x00000007,//!< 弱、のけぞりなし
	HITINFO_REACT2A			=	0x00000008,//!< 中、のけぞりなし
	HITINFO_REACT3A			=	0x00000009,//!< 強、のけぞりなし
	HITINFO_TATAKI1A		=	0x0000000A,//!< 真下叩きつけバウンド有り
	HITINFO_TATAKI2A		=	0x0000000B,//!< 真下叩きつけバウンドなし
	HITINFO_TATAKI1B		=	0x0000000C,//!< 斜め叩きつけバウンド有り
	HITINFO_TATAKI2B		=	0x0000000D,//!< 斜め叩きつけバウンドなし
	HITINFO_USERDEFINE		=	0x0000000E,//!< ユーザー定義の喰らい行動(地上/空中自動判別)
	HITINFO_USERDEFINE2		=	0x0000000F,//!< ユーザー定義の喰らい行動(強制的に空中に浮かせる)

	HITINFO_MARK1			=	0x00010000,//!< ヒットしたときにちっちゃいマークを表示します
	HITINFO_MARK2			=	0x00020000,//!< ヒットしたときに中ぐらいのマークを表示します
	HITINFO_MARK3			=	0x00030000,//!< ヒットしたときにでっかいマークを表示します
	HITINFO_MARK4			=	0x00040000,//!< ヒットしたときにさらにでっかいマークを表示します

	HITINFO_SNDHIT1			=	0x00100000,//!< ヒット音弱
	HITINFO_SNDHIT2			=	0x00200000,//!< ヒット音中
	HITINFO_SNDHIT3			=	0x00300000,//!< ヒット音強
	HITINFO_SNDSHK1			=	0x00400000,//!< 電撃音1
	HITINFO_SNDSHK2			=	0x00500000,//!< 電撃音2

	HITINFO_SIV1			=	0x01000000,//!< ヒットストップ、短い
	HITINFO_SIV2			=	0x02000000,//!< ヒットストップ、ふつう
	HITINFO_SIV3			=	0x03000000,//!< ヒットストップ、長い
	HITINFO_STOP			=	0x04000000,//!< ヒットストップ、めちゃ長

	HITINFO_EFCTSINDO		=	0x00001000,//!< 攻撃が当たったとき、地面を振動させる
	HITINFO_EFCTBURN		=	0x00002000,//!< 攻撃を喰らうとキャラクターが燃え出す(赤)
	HITINFO_EFCTBURN_B		=	0x00004000,//!< 攻撃を喰らうとキャラクターが燃え出す(青)
	HITINFO_EFCTBURN_G		=	0x00008000,//!< 攻撃を喰らうとキャラクターが燃え出す(緑)

	HITINFO_USERIDMASK		=	0x00000FF0,//!< ユーザー定義ヒットID
};

/*!
*	@brief ガード情報フラグ
*	@ingroup GObject
*	@sa ATTACKINFO
*
*	ATTACKINFO::guard に設定します。攻撃が当ガードされたときの各種設定。
*/
enum GUARDINFO_FLAGS
{
	GUARDINFO_XSTAND		=	0x10000000,//!< 立ちガードできません
	GUARDINFO_XCROUCH		=	0x20000000,//!< しゃがみガードできません
	GUARDINFO_XJAMP			=	0x40000000,//!< 空中ガードできません
	GUARDINFO_XAUTO			=	0x80000000,//!< オートガードできない

	GUARDINFO_REACT1		=	0x00000001,//!< 弱ガード
	GUARDINFO_REACT2		=	0x00000002,//!< 中ガード
	GUARDINFO_REACT3		=	0x00000003,//!< 強ガード
	GUARDINFO_USERDEFINE	=	0x00000004,//!< ユーザー定義ガード行動

	GUARDINFO_SIV1			=	0x01000000,//!< ヒットストップ、短い
	GUARDINFO_SIV2			=	0x02000000,//!< ヒットストップ、ふつう
	GUARDINFO_SIV3			=	0x03000000,//!< ヒットストップ、長い
	GUARDINFO_STOP			=	0x04000000,//!< ヒットストップ、めちゃ長

	GUARDINFO_EFCTSINDO		=	0x00001000,//!< 攻撃が当たったとき、地面を振動させる

	GUARDINFO_USERIDMASK	=	0x00000FF0,//!< ユーザー定義ガードID
};

//ユーザー定義フラグ設定用マクロ
#define HITINFO_USER(id)		(((id)<<4) | HITINFO_USERDEFINE)		//!< IDは0～255で与えること
#define HITINFO_USER2(id)		(((id)<<4) | HITINFO_USERDEFINE2)		//!< IDは0～255で与えること
#define GUARDINFO_USER(id)		(((id)<<4) | GUARDINFO_USERDEFINE)		//!< IDは0～255で与えること

/*!
*	@brief 攻撃力情報構造体（喰らい側）
*	@ingroup GObject
*
*	オブジェクトがどういった攻撃を受けたのかを定義する。
*	info1メンバは相手側の構造体へのポインタがそのまま渡ってくるのでいじらないこと。
*/
struct ATTACKINFO2
{
	ATTACKINFO *info1;		//!< 攻撃情報
	DWORD oid;				//!< 攻撃を行ったヤツのオブジェクトID
	DWORD flags;			//!< フラグ @sa ATKINFO2_FLAGS
};

/*!
*	@brief 攻撃力情報フラグ（喰らい側）
*	@ingroup GObject
*	@sa ATTACKINFO2
*
*	攻撃のヒット時にシステムがこれらのフラグをGOBJECT::atk2::flagsに割り当てます
*/
enum
{
	ATKINFO2_RIGHTBACK		=	0x00000001,//!< 右側にのけぞる
	ATKINFO2_ATTACKERBACK	=	0x00000002,//!< 画面端まで到達したら攻撃したやつを押し戻す処理をしなければならない
};

/*!
*	@brief コンピュータ動作時に使用する構造体
*	@ingroup GObject
*
*	DLLが、システム側へ、キャラクターの持つ技の行動IDを通知するために使用される。
*	オブジェクトがコンピュータ制御である場合、それぞれの行動IDの用途から、
*	システムが行動遷移の候補を選択し、行動遷移の可/不可をGOBJMSG_COMMANDCOMメッセージで
*	DLL側に確認して可であったら、オブジェクトはその行動へ遷移する。
*/
struct WAZAINFO
{
	DWORD walkf;			//!< 前進
	DWORD walkb;			//!< 後退
	DWORD jampf;			//!< まゑジャンプ
	DWORD jampb;			//!< 裏ジャンプ
	DWORD dashf;			//!< まゑダッシュ
	DWORD dashb;			//!< 裏ダッシュ

	DWORD att_short[6];		//!< 短リーチ攻撃
	DWORD att_middle[6];	//!< 中リーチ攻撃
	DWORD att_long[6];		//!< 長リーチ攻撃
	DWORD att_jamp[6];		//!< 空中で出す攻撃
	DWORD att_tai[6];		//!< 対空に使える攻撃
	DWORD att_bullet[6];	//!< 飛び道具など

	DWORD nageid[6];		//!< 投げ行動ID

	//＜未使用＞
	int		not_used1[8];			//!<未使用
	DWORD	not_used2[8][16][2];	//!<未使用
	int		not_used3[8];			//!<未使用
	DWORD	not_used4[8][16][2];	//!<未使用
};

//ACTION ID===============================================================================

/*!
*	@brief 行動種別フラグ
*	@ingroup GObject
*	@sa GOBJECT::aid
*
*	行動IDに対して設定する各種別フラグ定義。
*	コマンド判定・ガード判定等でこれらのフラグが参照されるので、
*	新規行動IDを定義する場合は適切なフラグを加えなければなりません。
*/
enum ACTION_FLAGS
{
	//占有フラグ							//xxxx 0000 00xx 0xxx | user flags...
	ACTID_KUCYU			=	0x00010000,	//!<空中行動
	ACTID_SYAGAMI		=	0x00020000,	//!<しゃがみ行動
	ACTID_ATTACK		=	0x00100000,	//!<攻撃行動
	ACTID_HISSATU		=	0x00200000,	//!<必殺技
	ACTID_SYSTEM		=	0x80000000,	//!<システム系
	ACTID_KURAI			=	0x40000000,	//!<喰らい行動
	ACTID_GUARD			=	0x20000000,	//!<ガード行動
	ACTID_NAGE			=	0x00040000,	//!<投げ/投げられ 行動
	ACTID_INOUT			=	0x10000000,	//!<交代/支援攻撃 行動
};

/*!
*	@brief 行動種別フラグ
*	@ingroup GObject
*	@sa GOBJECT::aid
*
*	喰らい・ガード・やられ　や、ダメージ・ガードからの復帰行動等
*	システム側で定義する必要のある行動の行動ID。
*	DLL作者が新規に作成する行動IDは、ここに定義している値とかぶらないようにすること。
*/
enum DEFAULT_ACTION_IDs
{
	ACTID_NEUTRAL		= 1,					//!< ニュートラルポーズ (ダメージ・ガード行動からの復帰行動)
	ACTID_TOJYO			= 2,					//!< 登場 (旧バージョンとの互換用)
	ACTID_SYORI			= 3,					//!< 勝利 (旧バージョンとの互換用)
	ACTID_CROUCH		= ACTID_SYAGAMI,		//!< しゃがみポーズ (ダメージ・ガード行動からの復帰行動)
	ACTID_RAKKA			= ACTID_KUCYU,			//!< 落下 (ダメージ・ガード行動からの復帰行動)

	ACTID_DAMAGE1		= (ACTID_SYSTEM | ACTID_KURAI | 0x0001),				//!< 立ち喰らい（小）
	ACTID_DAMAGE2		= (ACTID_SYSTEM | ACTID_KURAI | 0x0002),				//!< 立ち喰らい（中）
	ACTID_DAMAGE3		= (ACTID_SYSTEM | ACTID_KURAI | 0x0003),				//!< 立ち喰らい（大）
	ACTID_DAMAGEC1		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0001),//!< しゃがみ喰らい（小）
	ACTID_DAMAGEC2		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0002),//!< しゃがみ喰らい（中）
	ACTID_DAMAGEC3		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0003),//!< しゃがみ喰らい（大）
	ACTID_DAMAGEJ1		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU   | 0x0001),//!< 空中喰らい（小）
	ACTID_DAMAGEJ2		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU   | 0x0002),//!< 空中喰らい（中）
	ACTID_DAMAGEJ3		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU   | 0x0003),//!< 空中喰らい（大）
	ACTID_DOWN			= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0004),	//!< 足払いで転び
	ACTID_DOWN2			= (ACTID_SYSTEM | ACTID_KURAI | 0x0004),				//!< ダウソ
	ACTID_OKIAGARI		= (ACTID_SYSTEM | 0x0001),								//!< おきあがり
	ACTID_FINALDOWN		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0005),	//!< 死
	ACTID_FUTTOBI		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0006),	//!< ふっとび
	ACTID_BOUND			= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0007),	//!< バウンド
	ACTID_FINALBOUND	= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0008),	//!< バウンド(死亡)
	ACTID_FINALDOWN2	= (ACTID_SYSTEM | ACTID_KURAI | 0x0005),				//!< 氏
	ACTID_FUTTOBI2		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0009),	//!< ふっとび2
	ACTID_NAGERARE		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | ACTID_NAGE | 0x0001),//!< 投げられ中
	ACTID_DAMAGE1A		= (ACTID_SYSTEM | ACTID_KURAI | 0x0006),				//!< 立ち喰らい（小）、のけぞりなし
	ACTID_DAMAGE2A		= (ACTID_SYSTEM | ACTID_KURAI | 0x0007),				//!< 立ち喰らい（小）、のけぞりなし
	ACTID_DAMAGE3A		= (ACTID_SYSTEM | ACTID_KURAI | 0x0008),				//!< 立ち喰らい（小）、のけぞりなし
	ACTID_DAMAGEC1A		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0004),//!< しゃがみ喰らい（小）、のけぞりなし
	ACTID_DAMAGEC2A		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0005),//!< しゃがみ喰らい（中）、のけぞりなし
	ACTID_DAMAGEC3A		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0006),//!< しゃがみ喰らい（大）、のけぞりなし
	ACTID_TATAKITUKE1A	= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000A),	//!< 真下叩きつけ落下中(バウンド有り)
	ACTID_TATAKITUKE2A	= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000B),	//!< 真下叩きつけ落下中(バウンド無し)
	ACTID_TATAKITUKE1B	= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000C),	//!< 斜め叩きつけ落下中(バウンド有り)
	ACTID_TATAKITUKE2B	= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000D),	//!< 斜め叩きつけ落下中(バウンド無し)
	ACTID_KAITENFINISH	= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000E),	//!< 回転して画面外に吹っ飛んでいく
	ACTID_USERHIT1		= (ACTID_SYSTEM | ACTID_KURAI | 0x0010),				//!< ユーザー定義喰らい行動(地上)
	ACTID_USERHIT2		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x0011),	//!< ユーザー定義喰らい行動(空中)

	ACTID_GUARD1		= (ACTID_SYSTEM | ACTID_GUARD | 0x0001),				//!< 立ちガード（小）
	ACTID_GUARD2		= (ACTID_SYSTEM | ACTID_GUARD | 0x0002),				//!< 立ちガード（中）
	ACTID_GUARD3		= (ACTID_SYSTEM | ACTID_GUARD | 0x0003),				//!< 立ちガード（大）
	ACTID_GUARDC1		= (ACTID_SYSTEM | ACTID_GUARD | ACTID_SYAGAMI | 0x0001),//!< しゃがみガード（小）
	ACTID_GUARDC2		= (ACTID_SYSTEM | ACTID_GUARD | ACTID_SYAGAMI | 0x0002),//!< しゃがみガード（中）
	ACTID_GUARDC3		= (ACTID_SYSTEM | ACTID_GUARD | ACTID_SYAGAMI | 0x0003),//!< しゃがみガード（大）
	ACTID_GUARDJ1		= (ACTID_SYSTEM | ACTID_GUARD | ACTID_KUCYU   | 0x0001),//!< 空中ガード（小）
	ACTID_GUARDJ2		= (ACTID_SYSTEM | ACTID_GUARD | ACTID_KUCYU   | 0x0002),//!< 空中ガード（中）
	ACTID_GUARDJ3		= (ACTID_SYSTEM | ACTID_GUARD | ACTID_KUCYU   | 0x0003),//!< 空中ガード（大）
	ACTID_USERGUARD1	= (ACTID_SYSTEM | ACTID_GUARD | 0x0010),				//!< ユーザー定義ガード行動(地上)
	ACTID_USERGUARD2	= (ACTID_SYSTEM | ACTID_GUARD | ACTID_KUCYU | 0x0011),	//!< ユーザー定義ガード行動(空中)
};

//デフォルトcell id=====================================================================

/*!
*	@brief デフォルトcell ID
*	@ingroup GObject
*	@sa GOBJECT::cnow
*
*	キャラクターに喰らいやガード行動などのデフォルト行動を行わせるために、
*	システムはこれらのセルを利用する。
*	また、投げなどで喰らい側の行動を攻撃側が記述する場合にもこれらの値が利用される。
*	キャラクターはこれらの番号に適切なセルを割り当てておかなければならない。
*	また、将来の拡張のためにDLL作者が自由に画像を割り当てることができるセルは100番以降とする。
*/
enum DEFAULT_CELL_IDs
{
	DCELL_DAMAGES1		= 1,//!< 立ち喰らいポーズ1
	DCELL_DAMAGES2		= 2,//!< 立ち喰らいポーズ2
	DCELL_DAMAGES3		= 3,//!< 立ち喰らいポーズ3
	DCELL_DAMAGES4		= 4,//!< 立ち喰らいポーズ4
	DCELL_DAMAGEC1		= 5,//!< しゃがみ喰らいポーズ1
	DCELL_DAMAGEC2		= 6,//!< しゃがみ喰らいポーズ2
	DCELL_DAMAGEC3		= 7,//!< しゃがみ喰らいポーズ3
	DCELL_DAMAGEC4		= 8,//!< しゃがみ喰らいポーズ4
	DCELL_GUARDS1		= 9,//!< 立ちガードポーズ1
	DCELL_GUARDS2		=10,//!< 立ちガードポーズ2
	DCELL_GUARDS3		=11,//!< 立ちガードポーズ3
	DCELL_GUARDS4		=12,//!< 立ちガードポーズ4
	DCELL_GUARDC1		=13,//!< しゃがみガードポーズ1
	DCELL_GUARDC2		=14,//!< しゃがみガードポーズ2
	DCELL_GUARDC3		=15,//!< しゃがみガードポーズ3
	DCELL_GUARDC4		=16,//!< しゃがみガードポーズ4
	DCELL_DAMAGEJ		=17,//!< 空中喰らいポーズ
	DCELL_RAKKA1		=18,//!< 落下1
	DCELL_RAKKA2		=19,//!< 落下2
	DCELL_GUARDJ1		=20,//!< 空中ガードポーズ1
	DCELL_GUARDJ2		=21,//!< 空中ガードポーズ2
	DCELL_GUARDJ3		=22,//!< 空中ガードポーズ3
	DCELL_BOUND			=23,//!< 落下してバウンド
	DCELL_DOWN			=24,//!< ダウン
	DCELL_OKIAGARI1		=25,//!< 起き上がりモーション1
	DCELL_OKIAGARI2		=26,//!< 起き上がりモーション2
	DCELL_OKIAGARI3		=27,//!< 起き上がりモーション3
	DCELL_OKIAGARI4		=28,//!< 起き上がりモーション4
	DCELL_OKIAGARI5		=29,//!< 起き上がりモーション5
	DCELL_NAGERARE1		=30,//!< 重心が有効
	DCELL_KAITENFUTTOBI	=31,//!< 回転吹っ飛び、重心が有効
	DCELL_END			=99,//!< ここまで
};

/*!
*	@brief オブジェクト用メッセージ処理関数型定義
*	@ingroup GObject
*	@sa GOBJECT
*	@sa GOBJECT_MESSAGE
*
*	引数1:メッセージID（GOBJMSG_～）
*	引数2:オブジェクトのデータ(GOBJECTのポインタ)
*	引数3:各メッセージにより意味が異なる（ほとんど使ってない）
*	戻り値：各メッセージにより意味が異なる。0だとデフォルト処理を行う。
*/
typedef DWORD (*PFOBJ_MSGHANDLER)(DWORD,LPVOID,DWORD);


/*!
*	@brief オブジェクトデータ
*	@ingroup GObject
*
*	オブジェクトの保持するデータ。重要。
*	試合中、あたり判定や描画はこの、オブジェクトを単位として行われる。
*	DLLがシステムへオブジェクトの生成を要求すると、この構造体が生成される。
*/
struct GOBJECT
{
	DWORD id;			//!< オブジェクトid
	DWORD uid;			//!< ユーザーが割り当てるid
	DWORD tid;			//!< チームid
	DWORD eid;			//!< 敵のid
	PFOBJ_MSGHANDLER msghandler;//!< メッセージ処理関数
	DWORD objtype;		//!< オブジェクトの持つ性質
	
	double x,y;			//!< 位置
	double vx,vy;		//!< 速度
	double ax,ay;		//!< 加速度
	
	DWORD counter;		//!< カウンタ
	DWORD aid;			//!< 行動ID
	DWORD cnow;			//!< 表示セル
	int hp;				//!< 体力
	DWORD hpmax;		//!< 体力の最大値。デフォルトは1024
	double gauge;		//!< ゲージ
	DWORD gaugemax;		//!< ゲージのマックス値 1-9の値を指定。タイプ2のゲージは関係なし
	DWORD gaugetype;	//!< どのパワーゲージを使用するか。下のフラグ。デフォルトはゲージ無し

	BOOL muki;			//!< 向き（FALSEは右向き）
	BOOL revx;			//!< x反転
	BOOL revy;			//!< y反転
	int rot;			//!< 回転
	float magx,magy;	//!< 拡大率
	float z;			//!< z座標
	DWORD color;		//!< 色（自前描画の場合もこの色で描画してください）
	DWORD alphamode;	//!< 合成方法（GBLEND_～）フラグ ※非推奨項目

	BOOL kougeki;		//!< 攻撃力on/off
	BOOL kasanari;		//!< 重なり判定on/off
	BOOL muteki;		//!< 無敵on/off
	BOOL nagerare;		//!< 投げられ判定on/off

	MYSURFACE **pmsarr; //!< ビットマップの配列
	GCD_RECT *prdat;	//!< 矩形情報配列
	GCD_CELL2 *pcdat;	//!< セル情報配列
	GCD_HANTEI *phdat;	//!< 重なり・当たり・喰らい判定配列

	ATTACKINFO *atk;	//!< いま自分が持っている攻撃情報
	ATTACKINFO2 atk2;	//!< 他のオブジェクトから喰らった攻撃情報
	WAZAINFO  *winfo;	//!< コンピュータ用技情報

	LPVOID pobjdat_u;	//!< ユーザー定義データ
};

/*!
*	@brief チームID
*	@ingroup GObject
*	@sa GOBJECT::tid
*
*	キャラクターの場合、TEAM_PLAYER1 か、TEAM_PLAYER2かどちらかである。
*	ver0.9以降、PLAYERが増えたので語弊があるが...
*/
enum TEAM_IDs
{
	TEAM_PLAYER1			=0,
	TEAM_PLAYER2			=1,
	TEAM_STAGE				=2,
	TEAM_SYSTEM				=3,
};

/*!
*	@brief オブジェクトプロパティ
*	@ingroup GObject
*	@sa GOBJECT::objtype
*
*	オブジェクトの動作にかかわる各種設定のためのフラグ類。
*	システムはあたり判定やデフォルト動作、試合進行管理等の処理で
*	これらのフラグを参照する。
*/
enum GOBJECT_PROPERTIES
{
	//占有ビット				= 0xF1000FEF
	GOBJFLG_KURAI			= 0x00000001,//!< 攻撃を喰らう
	GOBJFLG_NAGERARE		= 0x00000002,//!< 投げられる
	GOBJFLG_ATTACK			= 0x00000004,//!< 攻撃を行う
	GOBJFLG_KASANARI		= 0x00000008,//!< 重なり判定をやる
	GOBJFLG_TOBETARGET		= 0x10000000,//!< ターゲットになることができる。
	GOBJFLG_NEEDTARGET		= 0x20000000,//!< ターゲット(eid)が必要である。
	GOBJFLG_COMPUTER		= 0x40000000,//!< コンピュータに制御させる。通常のキャラクターにはシステムによってこのフラグが付加される
	GOBJFLG_ZCHARACTER		= 0x80000000,//!< キャラクター「属性」 - 今のところ特に使っていない
	GOBJFLG_ZBULLET			= 0x01000000,//!< 飛び道具「属性」- 他の飛び道具と相殺処理する
	GOBJFLG_DISPZAHYO		= 0x00000020,//!< 座標指定はディスプレイ上の座標であり、ステージ上の表示位置に左右されない
	GOBJFLG_DONOTSTOP		= 0x00000040,//!< 超必等でstopエフェクトがかかっていてもGOBJMSG_ACTIONが通知される
	GOBJFLG_HANSAYOU		= 0x00000080,//!< 攻撃したオブジェクトが画面端まで到達すると押し戻される（メッセージを受け取ることができる）
	GOBJFLG_CLIPX			= 0x00000100,//!< オブジェクトはx方向に関してステージ(-640～640)外に行くことはできない
	GOBJFLG_CLIPX2			= 0x00000200,//!< オブジェクトはx方向に関して画面外に出ることはしない
	GOBJFLG_DISPLAYME		= 0x00000400,//!< ステージ上でディスプレイ中心を算出するときにこのキャラクターの座標を参照する
	GOBJFLG_TAIKI			= 0x00000800,//!< キャラクターは待機中である
};

//!標準的なキャラクターが持つフラグ
#define GOBJTYPE_CHARACTER2	( GOBJFLG_KURAI | GOBJFLG_NAGERARE | GOBJFLG_ATTACK | GOBJFLG_KASANARI | GOBJFLG_CLIPX | GOBJFLG_CLIPX2 | GOBJFLG_DISPLAYME)
//!標準的なキャラクターが持つフラグ
#define GOBJTYPE_CHARACTER	( GOBJTYPE_CHARACTER2 | GOBJFLG_TOBETARGET | GOBJFLG_NEEDTARGET | GOBJFLG_ZCHARACTER | GOBJFLG_HANSAYOU | GOBJFLG_CLIPX)

//!標準的な飛び道具のフラグ
#define GOBJTYPE_BULLET		(GOBJFLG_ATTACK | GOBJFLG_ZBULLET)

//その他
#define GOBJTYPE_DISPLAY	( GOBJFLG_CLIPX | GOBJFLG_CLIPX2 | GOBJFLG_DISPLAYME )
#define GOBJTYPE_NODISPLAY	( ~( GOBJFLG_CLIPX | GOBJFLG_CLIPX2 | GOBJFLG_DISPLAYME ) )
#define GOBJTYPE_DEAD		( ~(GOBJFLG_TOBETARGET | GOBJFLG_NEEDTARGET | GOBJFLG_CLIPX | GOBJFLG_CLIPX2 | GOBJFLG_DISPLAYME) )

/*!
*	@brief ゲージ描画タイプ
*	@ingroup GObject
*	@sa GOBJECT::gaugetype
*
*	ゲージはDLLのCreateCharacterの戻り値でオブジェクトIDの指定された
*	「キャラクターオブジェクト」に対してしか描画は行われない。
*	また、ここでのゲージの指定はゲージの最大値には影響を与えないことに注意すること。
*	ゲージの上限・下限の管理はDLL側で適切に処理する必要がある。
*/
enum GOBJECT_GAUGE_TYPE
{
	GOBJFLGA_POWGAUGE_NONE		= 0x00000000,//!< ゲージなし(デフォルト)
	GOBJFLGA_POWGAUGE1			= 0x00000001,//!< 9個までストック可能なゲージ
	GOBJFLGA_POWGAUGE2			= 0x00000002,//!< ストックできないゲージ
};

/*!
*	@brief αブレンドタイプ
*	@ingroup GObject
*	@sa DI_FUNCTIONS_D::setblend
*	@sa GOBJECT::alphamode
*
*	Direct3Dのレンダーステートを操作してα合成のタイプを変更する。
*	DI_FUNCTIONS_D::setblendで値をGBLEND_HANTOUMEI以外に設定した場合は
*	描画後にGBLEND_HANTOUMEIに設定を戻しておく必要がある。
*	尚、IDirect3DDeviceを取得して使用すればこの設定方法に頼らなくても
*	α合成モードを変更することができる。ただしその場合も描画後に設定を元に戻しておくこと。
*/
enum GBLENDMODES
{
	GBLEND_HANTOUMEI			= (0),//!< α合成・通常
	GBLEND_KASAN				= (1),//!< α合成・加算合成
	GBLEND_INV					= (3),//!< α合成・Dest色反転
};

//  ～メッセージ～  ======================================================================
/*!
*	@brief オブジェクトメッセージID
*	@ingroup GObject
*	@sa GOBJECT::msghandler
*
*	msghandlerに渡される第2引数(イベント種別)の意味。
*	オブジェクトはこれらのメッセージIDに対応した適切な処理を行って
*	各メッセージごとに定義された特有の戻り値を返すか、
*	あるいはシステムにデフォルトの動作を行わせるために0を返さなければならない。
*/
enum GOBJECT_MESSAGE
{
	//主なメッセージ
	GOBJMSG_DELETE				=   1,//!< オブジェクトがシステムの制御を離れるときに呼ばれる
	GOBJMSG_ACTION				=   2,//!< 1フレームに1回呼ばれる
	GOBJMSG_COMMAND				=   3,//!< コマンド判定をする必要があるときに呼ばれる
	GOBJMSG_COMMANDCOM			=   4,//!< コンピュータコマンド判定をするときに呼ばれる
	GOBJMSG_TOUCHA				=   5,//!< 攻撃を喰らったときに呼ばれる。
	GOBJMSG_TOUCHB				=   6,//!< 自分の攻撃が相手に当たった時に呼ばれる。
	GOBJMSG_TOUCHC				=   7,//!< 自分の攻撃が相手に当たったとき (相手の状態が変化する前に呼ばれる)。
	GOBJMSG_CNGAID				=   8,//!< 行動IDが変化したときに呼ばれる
	GOBJMSG_COMTHINK			=	9,//!< COMの動作思考をする時に呼ばれる
	//描画系メッセージ
	GOBJMSG_DRAW				= 101,//!< 描画を行う際に呼ばれる
	GOBJMSG_DRAWBACK			= 102,//!< 背景ステージに描画
	GOBJMSG_DRAWFRONT			= 103,//!< 前景ステージに描画
	//登場・交代などメッセージ
	GOBJMSG_DOTOJYO				= 201,//!< 登場ポーズに移行し、終わったらMSGOBJ2SYS_TOJYOENDメッセージを返せ
	GOBJMSG_DOTIMEOVERLOSE		= 202,//!< タイムオーバーで負けた
	GOBJMSG_DOYOUWIN			= 203,//!< 勝利ポーズに移行し、終わったらMSGOBJ2SYS_WINPOSEENDメッセージを返せ
	GOBJMSG_TAIKI				= 204,//!< 画面外に待機していてください
	GOBJMSG_KOUTAI				= 205,//!< 選手交代。画面の向こうから飛んできてください。（ダメなときは0を返す）
	GOBJMSG_KOUTAI2				= 206,//!< 選手交代。画面の向こうから飛んできてください。終わったらMSGOBJ2SYS_KOUTAIENDメッセージを返せ
	GOBJMSG_STRIKER				= 207,//!< ストライカー攻撃をやらかしてください（ダメなときは0を返す）
	GOBJMSG_DOYOUWIN2			= 208,//!< 待機中、仲間の勝利によってチームが勝利した
	GOBJMSG_STRIKERREADY		= 209,//!< ストライカー攻撃ができるかどうかを返せ
	//座標操作系メッセージ
	GOBJMSG_KNOCKBACK			= 301,//!< 攻撃したオブジェクトが画面端に達した
	GOBJMSG_CLIPX				= 302,//!< 座標が画面外に逝ってしまっているよ
	//オブジェクト間相互作用・その他
	GOBJMSG_CNGTARGET			= 401,//!< ターゲットが変わったときに呼ばれる
	GOBJMSG_SOUSAI				= 402,//!< 他飛び道具と接触
	//ゲーム進行
	GOBJMSG_CNGROUND			= 502,//!< ラウンドが変わったときに呼ばれる
	//ネットワーク
	GOBJMSG_SYNC				= 601,//!< 同期メッセージ. 未使用
	//その他
	GOBJMSG_USERDEFINE			=0x80000000,//!< ユーザー定義メッセージ作成用フラグ
};


/*!
*	@brief GOBJMSG_TOUCHA メッセージに対する戻り値
*	@ingroup GObject
*	@sa GOBJECT_MESSAGE
*
*	GOBJMSG_TOUCHA(攻撃喰らい)に対する戻り値としてこれらの値を使用する。
*	通常はTOUCHA_DEFAULT (=0) を返し、システムにデフォルトのガード判定を行わせるべきである。
*/
enum GOBJMSG_TOUCHA_RETURNS
{
	TOUCHA_DEFAULT				= 0x00000000,//!< デフォルトのガード判定を行う
	TOUCHA_KURAIS				= 0x10000001,//!< 立ち喰らい
	TOUCHA_KURAIC				= 0x10000002,//!< 屈喰らい
	TOUCHA_KURAIJ				= 0x10000003,//!< 空中喰らい
	TOUCHA_GUARDS				= 0x20000001,//!< 立ちガード
	TOUCHA_GUARDC				= 0x20000002,//!< 屈ガード
	TOUCHA_GUARDJ				= 0x20000003,//!< 空中ガード
	TOUCHA_MUSI					= 0x50000000,//!< 攻撃は喰らったけど、喰らい動作を行わない
	TOUCHA_MUSI2				= 0x60000000,//!< 攻撃をガードしたけど、喰らい動作を行わない
	TOUCHA_AVOID				= 0x80000000,//!< 攻撃をうけなかった

	//memo : 0x10000000をヒット、0x20000000をガード、0x40000000を無視フラグ として使用
};

/*!
*	@brief GOBJMSG_TOUCHCメッセージに対する戻り値
*	@ingroup GObject
*	@sa GOBJECT_MESSAGE
*
*	GOBJMSG_TOUCHC(攻撃ヒット)に対する戻り値としてこれらの値を使用する。
*	判定矩形の衝突が検出され、喰らい側にGOBJMSG_TOUCHAメッセージが送信される前にこのメッセージ処理を行う。
*	TOUCHC_CANCELを返せば衝突をなかったことにして続く全ての処理をキャンセルすることができる。
*/
enum GOBJMSG_TOUCHC_RETURNS
{
	TOUCHC_OK					= 0x00000000,//!< デフォルト。通常通り処理が進行します。
	TOUCHC_CANCEL				= 0x00000001,//!< 矩形衝突をなかったことにします。それ以降の処理はなされません。
};

/*!
*	@brief 支援攻撃問い合わせ
*	@ingroup GObject
*	@sa GOBJECT_MESSAGE
*
*	GOBJMSG_STRIKERREADYメッセージに対する戻り値 あるいはDI_FUNCTIONS_S::strikerokの戻り値として使用する。
*	支援攻撃が可能か否かを問い合わせるためのものであるが、
*	各キャラクターがメッセージへの応答をきちんと実装していないのであまり意味はない(ほとんどの場合「不明」が返る)
*/
enum GOBJMSG_STRIKERREADY_RETURNS
{
	STRIKERREADY_UNKNOWN		= (0),//!< 不明
	STRIKERREADY_OK				= (1),//!< 準備オッケー
	STRIKERREADY_NOTREADY		= (2),//!< できない
	STRIKERREADY_NOTSUPPORTED	= (3),//!< つーか、もともとしない
	STRIKERREADY_NOPOINT		= (4),//!< (sys)ストライカーのポイントが足りない
};

//**************************************************************************
// システム関連の関数の定義
//**************************************************************************
typedef DWORD	(*PFUNCS_GETKEY)(DWORD,DWORD);			//!< キー入力get
typedef int		(*PFUNCS_SEEKKEY)(DWORD,int,int,DWORD);	//!< キー入力検索
typedef void	(*PFUNCS_SETKATISERIF)(DWORD,char*);	//!< 勝ち台詞設定
typedef void	(*PFUNCS_ADDEFFECT)(DWORD,int,int,DWORD);//!< エフェクト発生
typedef void	(*PFUNCS_PLAYSYSSOUND)(DWORD);			//!< システムwav再生
typedef LPVOID	(*PFUNCS_LOADMYSOUND)(char*);			//!< wav読み込み
typedef void	(*PFUNCS_PLAYMYSOUND)(LPVOID);			//!< wav再生
typedef void	(*PFUNCS_KILLMYSOUND)(LPVOID);			//!< wav消去
typedef DWORD	(*PFUNCS_MESSAGE2SYSTEM)(DWORD,DWORD,DWORD);	//!< メッセージ
typedef char*	(*PFUNCS_GETENEMYNAME)(DWORD);			//!< "敵"の名前を取得する
typedef double	(*PFUNCS_GETDISPCENTERX)();				//!< 現在の画面中心(X)を得る
typedef double	(*PFUNCS_GETDISPCENTERY)();				//!< 現在の画面中心(Y)を得る

typedef DWORD	(*PFUNCS_GETTEAMNUM)();					//!< 何対何かをゲット ※ 1P側人数が返ります。非推奨関数
typedef DWORD	(*PFUNCS_GETTAISENKEISIKI)();			//!< 対戦形式をゲト
//typedef DWORD	(*PFUNCS_STRIKEROK)(DWORD tid,DWORD oid,BOOL next);//ストライカー攻撃が可能かどうかを問い詰める

typedef BYTE	(*PFUNCS_GETENEMYFACE)(DWORD tid);		//!< フェイス変更値(通常0)を取得
typedef DWORD	(*PFUNCS_GETTEAMNUM2)(DWORD tid);		//!< チーム人数を取得します
typedef DWORD	(*PFUNCS_TID2ETID)(DWORD tid);			//!< チームID変換
typedef BOOL	(*PFUNCS_IS_NETWORK)();					//!< ネット対戦かどうかを取得します
typedef void	(*PFUNCS_PUSHDLLEXECTAG)(const char*);	//!< 実行個所のタグ付けを行う(エラーログ時に出力)
typedef void	(*PFUNCS_POPDLLEXECTAG)();				//!< 実行個所のタグをひとつ削除する
typedef void	(*PFUNCS_LOGERROR)(const char*);		//!< エラーログを記録する
typedef void	(*PFUNCS_LOGWARNING)(const char*);		//!< 警告ログを記録する
typedef void	(*PFUNCS_LOGDEBUG)(const char*);		//!< デバッグログを記録する
typedef void	(*PFUNCS_LOGINFO)(const char*);			//!< 情報ログを記録する

/*!
*	@brief システム関数ポインタセット
*	@ingroup GObject
*
*	DLLがシステム側の関数を使用するために、キャラクター生成時にシステムからDLLに対して渡される。
*	システム(?)に関する関数が入ってる。
*	CGoluahObject継承クラスの場合、これらの関数は規定クラスにほぼ同名の関数から利用できる。(See also でリンク)
*/
struct DI_FUNCTIONS_S
{
	PFUNCS_GETKEY			getkey;						//!< キー入力get	 @sa CGoluahObject::GetKey
	PFUNCS_SEEKKEY			seekkey;					//!< キー入力検索  @sa CGoluahObject::SeekKey
	PFUNCS_SETKATISERIF		setkatiserif;				//!< 勝ち台詞設定  @sa CGoluahObject::SetKatiSerif
	PFUNCS_ADDEFFECT		addeffect;					//!< エフェクト発生  @sa CGoluahObject::AddEffect
	PFUNCS_PLAYSYSSOUND		playsyssound;				//!< システムwav再生 @sa CGoluahObject::PlaySysSound
	PFUNCS_LOADMYSOUND		loadmysound;				//!< wav読み込み @sa CGoluahObject::LoadMySound
	PFUNCS_PLAYMYSOUND		playmysound;				//!< wav再生 @sa CGoluahObject::PlayMySound
	PFUNCS_KILLMYSOUND		killmysound;				//!< wav消去 @sa CGoluahObject::KillMySound
	PFUNCS_MESSAGE2SYSTEM	msg2system;					//!< メッセージ送信 @sa MESSAGE_OBJ2SYS  @sa CGoluahObject::Message2System
	PFUNCS_GETENEMYNAME		getenemyname;				//!< "敵"の名前を取得する @sa CGoluahObject::GetEnemyName
	PFUNCS_GETDISPCENTERX	getdispcenterx;				//!< 現在の画面中心(X)を得る @sa CGoluahObject::GetDisplayCenter_X
	PFUNCS_GETDISPCENTERY	getdispcentery;				//!< 現在の画面中心(Y)を得る @sa CGoluahObject::GetDisplayCenter_Y
	PFUNCS_GETTEAMNUM		getteamnum;					//!< 何対何かをゲット ※ 1P側人数が返ります。非推奨関数 @sa CGoluahObject::GetTeamNum
	PFUNCS_GETTAISENKEISIKI	gettaisenkeisiki;			//!< 対戦形式をゲト @sa CGoluahObject::GetTaisenKeisiki
	PFUNCS_GETENEMYFACE		getenemyface;				//!< フェイス変更値(通常0)を取得 @sa CGoluahObject::GetEnemyFace
	PFUNCS_GETTEAMNUM2		getteamnum2;				//!< チーム人数を取得します @sa CGoluahObject::GetTeamNum2
	PFUNCS_TID2ETID			tid2etid;					//!< チームID変換 @sa CGoluahObject::TID2EnemyTID
	PFUNCS_IS_NETWORK		is_net;						//!< ネット対戦かどうかを取得します @sa CGoluahObject::IsNetwork
//	PFUNCS_STRIKEROK		strikerok;					//ストライカー攻撃が可能かどうかを問い詰める
	PFUNCS_PUSHDLLEXECTAG	pushtag;					//!< 実行個所のタグ付けを行う(エラーログ時に出力) @sa CGoluahObject::PushTag
	PFUNCS_POPDLLEXECTAG	poptag;						//!< 実行個所のタグをひとつ削除する @sa CGoluahObject::PopTag
	PFUNCS_LOGERROR			logerror;					//!< エラーログを記録する @sa CGoluahObject::LogError
	PFUNCS_LOGWARNING		logwarning;					//!< 警告ログを記録する @sa CGoluahObject::LogWarning
	PFUNCS_LOGDEBUG			logdebug;					//!< デバッグログを記録する @sa CGoluahObject::LogDebug
	PFUNCS_LOGINFO			loginfo;					//!< 情報ログを記録する @sa CGoluahObject::LogInfo
};

//キー入力定義===========================================================================

/*!
*	@brief キー入力値
*	@ingroup GObject
*
*	DI_FUNCTIONS_S::getkey や、DI_FUNCTIONS_S::seekkeyでこれらの値を使用します。
*	左右はキャラクターの向き(GOBJECT::muki)に影響を受けます.
*	「押されている状態」は押しっぱなしでもフラグがたちます。
*	「今押されたところ」は押した瞬間(1フレ)だけフラグがたちます。
*/
enum GKEYSTATES
{
	KEYSTA_LEFT			= (0x00000100),//!< 内部使用
	KEYSTA_LEFT2		= (0x00000200),//!< 内部使用
	KEYSTA_RIGHT		= (0x00001000),//!< 内部使用
	KEYSTA_RIGHT2		= (0x00002000),//!< 内部使用
	KEYSTA_ALEFT		= (0x00000400),//!< 内部使用
	KEYSTA_ALEFT2		= (0x00000800),//!< 内部使用
	KEYSTA_ARIGHT		= (0x00004000),//!< 内部使用
	KEYSTA_ARIGHT2		= (0x00008000),//!< 内部使用

	KEYSTA_UP			= (0x00000001),	//!< ↑入力(押されている状態)
	KEYSTA_UP2			= (0x00000002),	//!< ↑入力(今押されたところ)
	KEYSTA_DOWN			= (0x00000010),	//!< ↓入力(押されている状態)
	KEYSTA_DOWN2		= (0x00000020),	//!< ↓入力(今押されたところ)
	KEYSTA_FOWORD		= KEYSTA_RIGHT,	//!< 前方入力(押されている状態)
	KEYSTA_FOWORD2		= KEYSTA_RIGHT2,//!< 前方入力(今押されたところ)
	KEYSTA_BACK			= KEYSTA_LEFT,	//!< 後方入力(押されている状態)
	KEYSTA_BACK2		= KEYSTA_LEFT2,	//!< 後方入力(今押されたところ)
	KEYSTA_BA			= (0x00010000),	//!< ボタン1(押されている状態)
	KEYSTA_BA2			= (0x00020000),	//!< ボタン1(今押されたところ)
	KEYSTA_BB			= (0x00100000),	//!< ボタン2(押されている状態)
	KEYSTA_BB2			= (0x00200000),	//!< ボタン2(今押されたところ)
	KEYSTA_BC			= (0x01000000),	//!< ボタン3(押されている状態)
	KEYSTA_BC2			= (0x02000000),	//!< ボタン3(今押されたところ)
	KEYSTA_BD			= (0x10000000),	//!< ボタン4(押されている状態)
	KEYSTA_BD2			= (0x20000000),	//!< ボタン4(今押されたところ)

	KEYSTA_BUTTONS		= (0x22220000),	//!< ボタンのいずれかが押された
};
#define KEYSTA_ANYKEY	KEYSTA_BUTTONS

//Message Id============================================================================
/*!
*	@brief オブジェクトからシステムへのメッセージ
*	@ingroup GObject
*	@sa DI_FUNCTIONS_S::msg2system
*
*	試合の進行にかかわるイベントメッセージや、システムに対する要求を送信するときに使用します。
*/
enum MESSAGE_OBJ2SYS
{
	MSGOBJ2SYS_TOJYOEND			= 1,//!< 登場デモが終了した
	MSGOBJ2SYS_FINALDOWNEND		= 2,//!< 最終ダウンが終了した
	MSGOBJ2SYS_WINPOSEEND		= 3,//!< 勝利デモが終了した
	MSGOBJ2SYS_KOUTAI1			= 4,//!< 交代要請(キャラクター1)
	MSGOBJ2SYS_KOUTAI2			= 5,//!< 交代要請(キャラクター2)
	MSGOBJ2SYS_STRIKER1			= 6,//!< ストライカー要請(キャラクター1)
	MSGOBJ2SYS_STRIKER2			= 7,//!< ストライカー要請(キャラクター2)
	MSGOBJ2SYS_KOUTAIEND		= 8,//!< 交代終了。戦闘できる状態です
};

//Effect Id ============================================================================
/*!
*	@brief システムエフェクトID
*	@ingroup GObject
*	@sa DI_FUNCTIONS_S::addeffect
*
*	システムが持つ演出のID。
*/
enum SYSTEM_EFFECT_IDs
{
								     //コメント  (引数の意味)
	EFCTID_MARK1				=  1,//!<ヒットマーク・弱  (x,y,向)
	EFCTID_MARK2				=  2,//!<ヒットマーク・中  (x,y,向)
	EFCTID_MARK3				=  3,//!<ヒットマーク・強  (x,y,向)
	EFCTID_MARK4				=  4,//!<ヒットマーク・激  (x,y,向)
	EFCTID_MARKG				=  5,//!<ガードマーク・削りなし (x,y,向)
	EFCTID_MARKG2				=  6,//!<ガードマーク・削りあり (x,y,向)
	EFCTID_HIKARI				=  7,//!<光？ (x,y,0)
	EFCTID_STOP					=  8,//!<ストップ (ストップしてる時間,0,0)
	EFCTID_DARK					=  9,//!<暗くする (時間,0,0)  
	EFCTID_MARKC				= 10,//!<相手を掴んだとき推奨エフェクト (x,y,0)
	EFCTID_GNDCIR				= 11,//!<丸い衝撃波  (x,y,0)
	EFCTID_SINDO				= 12,//!<振動が起こる  (振幅,持続時間,0)
	EFCTID_NOBG					= 13,//!<背景の描画を無しにする  (持続時間,0,0) 
	EFCTID_CYOHI				= 14,//!<超必発動時推奨エフェクト (x,y,硬化時間)
	EFCTID_TUBUTUBU				= 15,//!<パーティクル散乱・小  (x,y,粒粒パラメータ) @sa TUBUTUBUPRM
	EFCTID_TUBUTUBU2			= 16,//!<パーティクル散乱・大  (x,y,粒粒パラメータ) @sa TUBUTUBUPRM
	EFCTID_COMEON				= 17,//!<支援を呼ぶとき推奨エフェクト  (x,y,向)
	EFCTID_BURN					= 18,//!<燃える・赤  (-,-,オブジェクトID)
	EFCTID_BURN_B				= 19,//!<燃える・青  (-,-,オブジェクトID)
	EFCTID_BURN_G				= 20,//!<燃える・緑  (-,-,オブジェクトID)
	EFCTID_GCANCEL				= 21,//!<ガードキャンセル時推奨エフェクト  (x,y,向)
	EFCTID_SUPERARMER			= 22,//!<スーパーアーマー時推奨エフェクト  (x,y,向)
};

//つぶつぶパラメータ入力用マクロ
#define TUBUTUBUPRM(a,b,c,d,e,f,g,h)	((DWORD)(a*256*256*256)*16+(b*256*256*256)+(c*256*256*16)+(d*256*256)+(e*256*16)+(f*256)+(g*16)+h)
//a:粒の数(0～15)
//b;粒の速度
//c:粒の速度のばらつき
//d:最低速度
//e:初期位置のばらつき
//f:寿命のばらつき
//g:粒の最短寿命
#define TUBUTUBU_GOUP				0x00000001//!< 上方向に偏った速度を持たせる
#define TUBUTUBU_GODOWN				0x00000002//!< 下向に偏った速度を持たせる
#define TUBUTUBU_GOLEFT				0x00000004//!< 左方向に
#define TUBUTUBU_GORIGHT			0x00000008//!< 右

//Sound ID =============================================================================
/*!
*	@brief システムエフェクトID
*	@ingroup GObject
*	@sa DI_FUNCTIONS_S::playsyssound
*
*	システム側で持っている効果音のID。
*/
enum SYSTEM_SOUND_IDs
{
	SYSTEMSOUND_HIT1			= 0,//!< hit1.wav ヒット音
	SYSTEMSOUND_HIT2			= 1,//!< hit2.wav
	SYSTEMSOUND_HIT3			= 2,//!< hit3.wav
	SYSTEMSOUND_SHOCK1			= 3,//!< shock1.wav 使おうと思えば使えるが、未使用
	SYSTEMSOUND_SHOCK2			= 4,//!< shock2.wav 使おうと思えば使えるが、未使用
	SYSTEMSOUND_GUARD			= 5,//!< guard.wav ガード音
	SYSTEMSOUND_CATCH			= 6,//!< catch.wav 掴み音
	SYSTEMSOUND_CYOHI			= 7,//!< cyohi.wav 超必発動時の効果音
	SYSTEMSOUND_CHARGE			= 8,//!< charge.wav ゲージが1本溜まったときの効果音
};

/*!
*	@brief 対戦形式
*	@ingroup GObject
*	@sa DI_FUNCTIONS_S::gettaisenkeisiki
*/
enum TAISENKEISIKI
{
	TAISENKEISIKI_GOCYAMAZE		=1,	//!< 同時対戦
	TAISENKEISIKI_KOUTAI		=2,	//!< 交代あり
	TAISENKEISIKI_JYUNBAN		=3,	//!< 交代なし
};

//**************************************************************************
//　オブジェクト関連の関数の定義
//**************************************************************************
typedef DWORD  (*PFUNCO_CREATE)();										//!< オブジェクト生成
typedef void   (*PFUNCO_DELETE)(DWORD);									//!< オブジェクト消滅 非推奨
typedef BOOL   (*PFUNCO_CATCH)(DWORD,LPVOID);							//!< 投げとかで相手をつかむ
typedef LPVOID (*PFUNCO_GETINFO)(DWORD);								//!< GOBJECT構造体取得
typedef void   (*PFUNCO_ADDDAMAGE)(DWORD,DWORD,int x,int y);			//!< (主に投げのとき)強制的にダメージを与える
typedef void   (*PFUNCO_ACTIDCHANGED)(DWORD);							//!< 行動IDが変化したときの処理を明示的に行う
typedef void*  (*PFUNCO_GETACTIVECHARACTER)(DWORD);						//!< 現在アクティブな※キャラクターの情報
typedef void   (*PFUNCO_SUICIDE)(DWORD);								//!< オブジェクト消滅 Fxオブジェクト以外は非推奨
typedef DWORD  (*PFUNCO_GETMAAI_H)(DWORD oid,DWORD eoid);				//!< 間合いをget(水平)
typedef DWORD  (*PFUNCO_GETMAAI_V)(DWORD oid,DWORD eoid);				//!< 間合いをget(垂直)
typedef DWORD  (*PFUNCO_GETCHARACTERID)(DWORD tid,DWORD index);			//!< キャラクターのオブジェクトID取得

typedef DWORD  (*PFUNCO_GETCOMLEVEL)(DWORD oid);						//!< COMレベル取得
typedef void   (*PFUNCO_SETCOMREACH)(DWORD oid,DWORD idx,int length);	//!< COM判定のリーチ長を設定 idx=0:短 , 1:中 , 2:長
typedef void   (*PFUNCO_SETCOMACT)(DWORD oid,DWORD aid,int delay);		//!< COM行動制御. aid:遷移行動ID(0は抑制) ,delay:発動遅延
typedef DWORD  (*PFUNCO_CREATE_FX)();									//!< エフェクト用オブジェクト生成
typedef BOOL   (*PFUNCO_IS_LOCAL)(DWORD oid);							//!< ローカルかどうか

typedef void   (*PFUNCO_DACTION)(GOBJECT* pdat);						//!< 指定オブジェクトにデフォルト動作をさせる
typedef void   (*PFUNCO_SETCOMRANGE)(DWORD oid,DWORD idx);				//!< COM判定のリーチ長を設定 idx=0:短 , 1:中 , 2:長
typedef char*	(*PFUNCO_GETCHARNAME)(DWORD);							//!< 指定キャラの名前を取得する
typedef DWORD	(*PFUNCO_GETKEYINPUT)(DWORD);							//!< 指定キャラのキーIDを取得する


//※アクティブ : 対戦形式にもよりますが、基本的に画面に出て戦ってるキャラクターを返すよう努力します
//				あいまいな定義です

#define COMIDX_NEAR		0
#define COMIDX_MIDDLE	1
#define COMIDX_LONG		2

/*!
*	@brief オブジェクト関数ポインタセット
*	@ingroup GObject
*
*	DLLがシステム側の関数を使用するために、キャラクター生成時にシステムからDLLに対して渡される。
*	オブジェクトに関する関数が入ってる。
*	CGoluahObject継承クラスの場合、これらの関数は規定クラスにほぼ同名の関数から利用できる。(See also でリンク)
*/
struct DI_FUNCTIONS_O
{
	PFUNCO_CREATE			objcreate;		//!< オブジェクト生成
	PFUNCO_DELETE			objdelete;		//!< オブジェクト消滅 非推奨
	PFUNCO_CATCH			objcatch;		//!< 投げとかで相手をつかむ  @sa CGoluahObject::ObjCatch
	PFUNCO_GETINFO			getinfo;		//!< GOBJECT構造体取得  @sa CGoluahObject::GetInfo
	PFUNCO_ADDDAMAGE		adddamage;		//!< (主に投げのとき)強制的にダメージを与える  @sa CGoluahObject::AddDamage
	PFUNCO_ACTIDCHANGED		actidchanged;	//!< 行動IDが変化したときの処理を明示的に行う  
	PFUNCO_GETACTIVECHARACTER getactivechar;//!< 現在アクティブなキャラクターの情報  @sa CGoluahObject::GetActiveCharacter
	PFUNCO_SUICIDE			suicide;		//!< オブジェクト消滅 エフェクトオブジェクト以外は非推奨  @sa CGoluahObject::Suicide
	PFUNCO_GETMAAI_H		getmaai_h;		//!< 間合いをget(水平)  @sa CGoluahObject::GetMaai_H
	PFUNCO_GETMAAI_V		getmaai_v;		//!< 間合いをget(垂直)  @sa CGoluahObject::GetMaai_V
	PFUNCO_GETCHARACTERID	getcharid;		//!< キャラクターのオブジェクトID取得  @sa CGoluahObject::GetCharacterID

	PFUNCO_GETCOMLEVEL		getcomlevel;	//!< COMレベル取得  @sa CGoluahObject::GetComLevel
	PFUNCO_SETCOMREACH		setcomreach;	//!< COM判定のリーチ長を設定  @sa CGoluahObject::SetComReach
	PFUNCO_SETCOMACT		setcomact;		//!< COM行動制御. aid:遷移行動ID(0は抑制) ,delay:発動遅延  @sa CGoluahObject::SetComAct
	PFUNCO_CREATE_FX		objcreate_fx;	//!< エフェクト用オブジェクト生成
	PFUNCO_IS_LOCAL			obj_is_local;	//!< ローカルなオブジェクトかどうか  @sa CGoluahObject::IsLocal

	PFUNCO_DACTION			dact_damages1;	//!< HITINFO_REACT1 を立ち状態で喰らったときの行動
	PFUNCO_DACTION			dact_damages2;	//!< HITINFO_REACT2 を立ち状態で喰らったときの行動
	PFUNCO_DACTION			dact_damages3;	//!< HITINFO_REACT3 を立ち状態で喰らったときの行動
	PFUNCO_DACTION			dact_damagec1;	//!< HITINFO_REACT1 をしゃがみ状態で喰らったときの行動
	PFUNCO_DACTION			dact_damagec2;	//!< HITINFO_REACT2 をしゃがみ状態で喰らったときの行動
	PFUNCO_DACTION			dact_damagec3;	//!< HITINFO_REACT3 をしゃがみ状態で喰らったときの行動
	PFUNCO_DACTION			dact_damagej1;	//!< HITINFO_REACT1 or HITINFO_REACT1A を空中状態で喰らったときの行動
	PFUNCO_DACTION			dact_damagej2;	//!< HITINFO_REACT2 or HITINFO_REACT2A を空中状態で喰らったときの行動
	PFUNCO_DACTION			dact_damagej3;	//!< HITINFO_REACT3 or HITINFO_REACT3A を空中状態で喰らったときの行動
	PFUNCO_DACTION			dact_damages1a;	//!< HITINFO_REACT1A を立ち状態で喰らったときの行動
	PFUNCO_DACTION			dact_damages2a;	//!< HITINFO_REACT2A を立ち状態で喰らったときの行動
	PFUNCO_DACTION			dact_damages3a;	//!< HITINFO_REACT3A を立ち状態で喰らったときの行動
	PFUNCO_DACTION			dact_damagec1a;	//!< HITINFO_REACT1A をしゃがみ状態で喰らったときの行動
	PFUNCO_DACTION			dact_damagec2a;	//!< HITINFO_REACT2A をしゃがみ状態で喰らったときの行動
	PFUNCO_DACTION			dact_damagec3a;	//!< HITINFO_REACT3A をしゃがみ状態で喰らったときの行動
	PFUNCO_DACTION			dact_guards1;	//!< HITINFO_REACT1 or HITINFO_REACT1A を立ち状態でガードしたときの行動
	PFUNCO_DACTION			dact_guards2;	//!< HITINFO_REACT2 or HITINFO_REACT2A を立ち状態でガードしたときの行動
	PFUNCO_DACTION			dact_guards3;	//!< HITINFO_REACT3 or HITINFO_REACT3A を立ち状態でガードしたときの行動
	PFUNCO_DACTION			dact_guardc1;	//!< HITINFO_REACT1 or HITINFO_REACT1A をしゃがみ状態でガードしたときの行動
	PFUNCO_DACTION			dact_guardc2;	//!< HITINFO_REACT2 or HITINFO_REACT2A をしゃがみ状態でガードしたときの行動
	PFUNCO_DACTION			dact_guardc3;	//!< HITINFO_REACT3 or HITINFO_REACT3A をしゃがみ状態でガードしたときの行動
	PFUNCO_DACTION			dact_guardj1;	//!< HITINFO_REACT1 or HITINFO_REACT1A を空中状態でガードしたときの行動
	PFUNCO_DACTION			dact_guardj2;	//!< HITINFO_REACT2 or HITINFO_REACT2A を空中状態でガードしたときの行動
	PFUNCO_DACTION			dact_guardj3;	//!< HITINFO_REACT3 or HITINFO_REACT3A を空中状態でガードしたときの行動
	PFUNCO_DACTION			dact_down;		//!< HITINFO_DOWN を喰らったときの行動
	PFUNCO_DACTION			dact_down2;		//!< ダウン状態
	PFUNCO_DACTION			dact_okiagari;	//!< 標準起き上がり
	PFUNCO_DACTION			dact_futtobi;	//!< HITINFO_FUTTOBI  を喰らったときの行動
	PFUNCO_DACTION			dact_futtobi2;	//!< HITINFO_FUTTOBI2 を喰らったときの行動
	PFUNCO_DACTION			dact_tatakituke1a;	//!< HITINFO_TATAKI1A を喰らったときの行動
	PFUNCO_DACTION			dact_tatakituke1b;	//!< HITINFO_TATAKI1B を喰らったときの行動
	PFUNCO_DACTION			dact_tatakituke2a;	//!< HITINFO_TATAKI2A を喰らったときの行動
	PFUNCO_DACTION			dact_tatakituke2b;	//!< HITINFO_TATAKI2B を喰らったときの行動

	PFUNCO_SETCOMRANGE		setcomrange;	//!< comが主体とするレンジ設定 @sa CGoluahObject::SetComRange
	PFUNCO_GETCHARNAME		getcharname;	//!< 指定キャラの名前を取得
	PFUNCO_GETKEYINPUT		getkeyinput;	//!< 指定キャラのキーIDを取得
};

/*!
*	@brief 相手つかみ情報
*	@ingroup GObject
*	@sa DI_FUNCTIONS_O::objcatch
*
*	オブジェクト関数objcatch にCATCHYOU構造体のポインタを入れて実行し、TRUEが返れば投げ開始
*	渡した関数が 相手の行動関数になる。表示セルにはデフォルトセルを利用する
*
*	投げられ状態に移行したオブジェクトにはGOBJMSG_ACTIONが送信されなくなる。
*	その代わりに投げたオブジェクトに対してここで指定したユーザーメッセージが、投げた側の
*	オブジェクトに対して送信される。そのメッセージに対して0以外(デフォルト行動IDのいずれか)
*	を返したときに、オブジェクトは投げられ状態から開放される。投げ側は指定したメッセージIDに対応する
*	適切な処理を行うこと。
*/
struct CATCHYOU
{
	DWORD oid;		//!< 掴むオブジェクト
	DWORD actmsg;	//!< 掴んだオブジェクトが、つかまれたオブジェクトを操作するために使うユーザー定義メッセージ
};

//**************************************************************************
//　描画関連の関数の定義
//**************************************************************************

//*描画系関数*
typedef LPVOID (*PFUNCD_GETD3D)();							//!< IDirect3D* の取得
typedef LPVOID (*PFUNCD_GETD3DD)();							//!< IDirect3DDevice* の取得
typedef void   (*PFUNCD_LOADCELLDAT)(char*,LPVOID,LPVOID,LPVOID);//!< GCDデータ読み込み関数
typedef LPVOID (*PFUNCD_LOADBMP)(char*,char*);				//!< ビットマップ読み込み関数
typedef void   (*PFUNCD_UNLOADBMP)(LPVOID);					//!< ビットマップ後始末関数
typedef void   (*PFUNCD_CELLDRAW)(LPVOID*,LPVOID,LPVOID,DWORD,int,int,float,int,BOOL,BOOL,DWORD,float,float);//!< セル描画関数
typedef void   (*PFUNCD_CKBLT)(void*,int,int,RECT,double,double,BOOL,BOOL,float,DWORD);//!< Blt2
typedef void   (*PFUNCD_BLT3D)(void*,RECT,MYRECT3D,DWORD);	//!< Blt3
typedef void   (*PFUNCD_SETTRANSFORM)(BOOL);				//!< 変換行列設定
typedef void   (*PFUNCD_SETBLEND)(DWORD);					//!< ブレンドの仕方を変更
typedef void   (*PFUNCD_SETPARENTMATRIX)(LPVOID,BOOL,LPVOID);//!< 「親」の変換行列設定
typedef DWORD  (*PFUNCD_CREATEGCDDAT)(char* filename,LPVOID *cdat,LPVOID *rdat,LPVOID *hdat);//!< セルデータ読み込み。ver0.90以降ではLoadCellDatからこちらに変更するべき
typedef void   (*PFUNCD_DESTROYGCDDAT)(LPVOID *cdat,LPVOID *rdat,LPVOID *hdat);//!< CreateCellDatで生成されたバッファをクリア
typedef LPVOID (*PFUNCD_LOADIMAGE)(char*,char*);			//!< 画像読み込み関数、PNG対応に伴い追加。
typedef DWORD  (*PFUNCD_CREATEGCDDAT2)(char* filename,LPVOID *cdat,LPVOID *rdat,LPVOID *hdat);//!< セルデータ読み込み。ver0.90以降ではLoadCellDatからこちらに変更するべき

/*!
*	@brief 描画系関数ポインタセット
*	@ingroup GObject
*
*	DLLがシステム側の関数を使用するために、キャラクター生成時にシステムからDLLに対して渡される。
*	描画に関する関数が入ってる。
*	CGoluahObject継承クラスの場合、これらの関数は規定クラスにほぼ同名の関数から利用できる。(See also でリンク)
*/
struct DI_FUNCTIONS_D
{
	PFUNCD_GETD3D		getd3d;				//!< IDirect3D* の取得  @sa CGoluahObject::GetD3D
	PFUNCD_GETD3DD		getd3dd;			//!< IDirect3DDevice* の取得  @sa CGoluahObject::GetD3DDevice
	PFUNCD_LOADCELLDAT	loadcelldat;		//!< GCDデータ読み込み関数  @sa CGoluahObject::LoadCellData
	PFUNCD_LOADBMP		loadbmp;			//!< ビットマップ読み込み関数  @sa CGoluahObject::LoadBitmap
	PFUNCD_UNLOADBMP	unloadbmp;			//!< ビットマップ後始末関数  @sa CGoluahObject::UnloadBitMap
	PFUNCD_CELLDRAW		celldraw;			//!< セル描画関数  @sa CGoluahObject::CellDraw
	PFUNCD_CKBLT		ckblt;				//!< ビットマップ描画(1)  @sa CGoluahObject::CkBlt
	PFUNCD_BLT3D		blt3d;				//!< ビットマップ描画(2)  @sa CGoluahObject::Blt3D
	PFUNCD_SETTRANSFORM settrans;			//!< 座標変換行列設定  @sa CGoluahObject::SetTransform
	PFUNCD_SETBLEND		setblend;			//!< αブレンドの仕方を変更  @sa CGoluahObject::SetBlend
	PFUNCD_SETPARENTMATRIX setparentmat;	//!< 「親」座標変換行列設定  @sa CGoluahObject::SetParentMatrix

	PFUNCD_CREATEGCDDAT create_celldat;		//!< セルデータ読み込み。バージョン自動判別  @sa CGoluahObject::CreateCellData
	PFUNCD_DESTROYGCDDAT destroy_celldat;	//!< create_celldatで生成されたバッファを破棄  @sa CGoluahObject::DestroyCellData
	PFUNCD_LOADIMAGE	loadimage;			//!< 画像読み込み関数、PNG対応に伴い追加。  @sa CGoluahObject::LoadImage
	PFUNCD_CREATEGCDDAT2 create_celldat2;	//!< セルデータ読み込み（２）。バージョン、拡張子自動判別  @sa CGoluahObject::CreateCellData2
};


//*************************************************************************
//  標準的なz座標の値
//*************************************************************************
#define ZZAHYO_CHARACTER	(0.00f)
#define ZZAHYO_CHARACTER_B	(0.02f)
#define ZZAHYO_CHARACTER_F	(-0.02f)
#define ZZAHYO_BULLET1		(0.08f)
#define ZZAHYO_BULLET2		(0.04f)
#define ZZAHYO_BULLET3		(-0.04f)
#define ZZAHYO_BULLET4		(-0.08f)
#define ZZAHYO_EFFECT1		(0.06f)
#define ZZAHYO_EFFECT2		(-0.06f)
#define ZZAHYO_STAGE		(0.1f)


//**************************************************************************
//  キャラクタDLLのバージョン
//**************************************************************************
#define CDI_VERSION 1060

//********************************************************************************
//　キャラクターオプション情報定義
//********************************************************************************
/*!
*	@brief キャラクターオプション定義構造体
*	@ingroup GObject
*/
struct CHARACTER_LOAD_OPTION
{
	DWORD flag;			//!< オプションがONの場合に立つフラグ
	DWORD exclusive;	//!< 同時に指定できないほかのオプション
	DWORD depends;		//!< 同時に指定されていなければならないほかのオプション
	char name[64];		//!< オプション名
	char point;			//!< オプション消費ポイント
};

#define CHARACTER_LOAD_OPTION_FACENUMBERMASK (0xF0000000)//!< ここのビットをface変更のために使用する
#define CHARACTER_LOAD_OPTION_FACE_A		 (0x10000000)//!< face2a , face3a
#define CHARACTER_LOAD_OPTION_FACE_B		 (0x20000000)//!< face2b , face3b
#define CHARACTER_LOAD_OPTION_FACE_C		 (0x30000000)//!< face2c , face3c 以下,定義はしないけどp(0xF)まで同様

//**************************************************************************
// CharacterInfo関数で渡される構造体
//**************************************************************************

typedef DWORD (*PFUNC_CHARACTERINFO)(LPVOID);
/*!
	@brief CharacterInfo関数で渡される構造体
	@ingroup GObject

	システム側からDLL側に渡される。
	DLLはこの構造体に適切な値を設定してCharacterInfo関数の制御を返す。

	なんか、書類を渡されて「ここに必要事項を記入して返してね」みたいなノリ。
*/
struct CDI_CHARACTERINFO
{
	DWORD system_version;				//!< システムのバージョン。（ver0.9以前は0がはいってるとおもます）
	char dummy[28];						//!< 未使用
	char name[32];						//!< ココに名前をコピーすること
	DWORD caps;							//!< ここに情報を設定（ver0.9以降）
	LPVOID dummy2[3];					//!< 未使用
	DWORD ver;							//!< ココにバージョンをコピーすること
	//ver0.9以降で追加
	CHARACTER_LOAD_OPTION options[32];	//!< キャラ選択時に設定可能なオプション
	DWORD max_option_point;				//!< オプションポイントの最大値
};

//capsに設定するフラグ
#define CHARACTER_CAPS_NET		0x00000001//!< ネットワーク対戦で使用可能
#define CHARACTER_CAPS_OFFLINE	0x00000002//!< かつ、オフラインモードでも使用可能


//********************************************************************************
//　CreateCharacter の引数に渡される構造体
//********************************************************************************
typedef DWORD (*PFUNC_CREATECHARACTER)(LPVOID);
/*!
*	@brief CreateCharacter の引数に渡される構造体
*	@ingroup GObject
*
*	この情報は、キャラクター生成後試合中ずっと必要になるモノが詰まっているので、
*	DLLは適切な場所にこれらの情報のコピーを作成して管理すること。
*/
struct CDI_CHARACTERINFO2
{
	char dir[64];					//!< dllがあるディレクトリ
	DWORD tid;						//!< チームID(0:1P側、1:2P側)
	DWORD color;					//!< どの色にするか（pal?.bmp）
	DWORD keyinput;					//!< どこから入力を受け取るか
	DI_FUNCTIONS_S *funcs;			//!< システム関数ポインタセット(システム)
	DI_FUNCTIONS_O *funco;			//!< システム関数ポインタセット(オブジェクト)
	DI_FUNCTIONS_D *funcd;			//!< システム関数ポインタセット(描画)
	//ver0.9以降で追加
	BYTE dllid;						//!< DLLのID
	DWORD options_flag;				//!< 選択されたオプションフラグ
};

//**************************************************************************
// ステージDLLのバージョン
//**************************************************************************

#define SDI_VERSION 1060//無視してるかも・・・


//**************************************************************************
// StageInfo関数で渡される構造体
//**************************************************************************

typedef DWORD (*PFUNC_STAGEINFO)(LPVOID);
/*!
*	@brief StageInfo関数で渡される構造体
*	@ingroup GObject
*	@sa CDI_CHARACTERINFO
*
*	ステージDLLはPFUNC_STAGEINFOの形式、名前「StageInfo」で外部にエクスポートした
*	関数でこの構造体のポインタを受け取る。
*	nameメンバはシステムによって自動的にステージディレクトリのname.txtがロードされるので、
*	DLLはこの関数内でverメンバに適切な値（SDI_VERSION）を設定し、処理を返すこと。
*/
struct SDI_STAGEINFO
{
	char name[32];	//!< ステージ名
	DWORD ver;		//!< バージョン
};

//**************************************************************************
// CreateStage関数で渡される構造体
//**************************************************************************

typedef DWORD (*PFUNC_CREATESTAGE)(LPVOID);
/*!
*	@brief CreateStage関数で渡される構造体
*	@ingroup GObject
*	@sa CDI_CHARACTERINFO2
*
*	基本的にはCDI_CHARACTERINFO2と同じである。
*	ステージDLLはPFUNC_STAGEINFOの形式、名前「CreateStage」で外部にエクスポートした
*	関数でこの構造体のポインタを受け取る。
*/
struct SDI_STAGEINFO2
{
	char dir[64];	//dllがあるディレクトリ
	DWORD tid;		//チームID(常に2(ステージ))
	DI_FUNCTIONS_S *funcs;
	DI_FUNCTIONS_O *funco;
	DI_FUNCTIONS_D *funcd;
};


/*@}*/
