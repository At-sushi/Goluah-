
/*=============================================================================

	GOLUAH キャラクター・ステージDLL のビルドに必要な各種構造体の定義

===============================================================================*/
#ifndef _GOBJECT_H_
#define _GOBJECT_H_

//GCDフォーマット定義が必要
#ifndef _DEFINE_GCD_H_
#include "define_gcd.h"
#endif

//******************************************************************************
//　　攻撃力情報
//******************************************************************************
struct ATTACKINFO
{
	DWORD hit;		//ヒット情報。下のフラグ(HITINFO_〜)の論理和を指定
	DWORD guard;	//ガード情報。下のフラグ(GUARDINFO_〜)の論理和を指艇
	DWORD id;		//キャラクタ側で勝手に定義して

	DWORD damage;	//ダメージ
	DWORD kezuri;	//削り

	BOOL muki;//相手がのけぞる向き(FALSEならば、自分が向いている方向と同じ)
};

//hit
#define HITINFO_REACT1		0x00000001//普通の弱のけぞり
#define HITINFO_REACT2		0x00000002//普通の中のけぞり
#define HITINFO_REACT3		0x00000003//普通の強のけぞり
#define HITINFO_DOWN		0x00000004//ダウソする（転ぶ）
#define HITINFO_FUTTOBI		0x00000005//吹っ飛ぶ
#define HITINFO_FUTTOBI2	0x00000006//吹っ飛ぶ（ちょっと上のほうへ）
#define HITINFO_REACT1A		0x00000007//弱、のけぞりなし
#define HITINFO_REACT2A		0x00000008//中、のけぞりなし
#define HITINFO_REACT3A		0x00000009//強、のけぞりなし
#define HITINFO_TATAKI1A	0x0000000A//真下叩きつけバウンド有り
#define HITINFO_TATAKI2A	0x0000000B//真下叩きつけバウンドなし
#define HITINFO_TATAKI1B	0x0000000C//斜め叩きつけバウンド有り
#define HITINFO_TATAKI2B	0x0000000D//斜め叩きつけバウンドなし

#define HITINFO_MARK1		0x00010000//ヒットしたときにちっちゃいマークを表示します
#define HITINFO_MARK2		0x00020000//ヒットしたときに中ぐらいのマークを表示します
#define HITINFO_MARK3		0x00030000//ヒットしたときにでっかいマークを表示します
#define HITINFO_MARK4		0x00040000//ヒットしたときにさらにでっかいマークを表示します

#define HITINFO_SNDHIT1		0x00100000//ヒット音弱
#define HITINFO_SNDHIT2		0x00200000//ヒット音中
#define HITINFO_SNDHIT3		0x00300000//ヒット音強
#define HITINFO_SNDSHK1		0x00400000//電撃音1
#define HITINFO_SNDSHK2		0x00500000//電撃音2

#define HITINFO_SIV1		0x01000000//ヒットストップ、短い
#define HITINFO_SIV2		0x02000000//ヒットストップ、ふつう
#define HITINFO_SIV3		0x03000000//ヒットストップ、長い
#define HITINFO_STOP		0x04000000//ヒットストップ、めちゃ長

#define HITINFO_EFCTSINDO	0x00001000//攻撃が当たったとき、地面を振動させる
#define HITINFO_EFCTBURN	0x00002000//攻撃を喰らうとキャラクターが燃え出す

//guard
#define GUARDINFO_XSTAND	0x10000000//立ちガードできません
#define GUARDINFO_XCROUCH	0x20000000//しゃがみガードできません
#define GUARDINFO_XJAMP		0x40000000//空中ガードできません
#define GUARDINFO_XAUTO		0x80000000//オートガードできない

#define GUARDINFO_REACT1	0x00000001//弱ガード
#define GUARDINFO_REACT2	0x00000002//中ガード
#define GUARDINFO_REACT3	0x00000003//強ガード

#define GUARDINFO_SIV1		0x01000000//ヒットストップ、短い
#define GUARDINFO_SIV2		0x02000000//ヒットストップ、ふつう
#define GUARDINFO_SIV3		0x03000000//ヒットストップ、長い
#define GUARDINFO_STOP		0x04000000//ヒットストップ、めちゃ長

#define GUARDINFO_EFCTSINDO	0x00001000//攻撃が当たったとき、地面を振動させる


struct ATTACKINFO2
{
	ATTACKINFO *info1;
	DWORD oid;//攻撃を行ったヤツのオブジェクトID。画面端でのけぞったとき、相手を押し戻すのに使用。
	DWORD flags;//下に示すフラグ
};

#define ATKINFO2_RIGHTBACK		0x00000001//右側にのけぞる
#define ATKINFO2_ATTACKERBACK	0x00000002//画面端まで到達したら攻撃したやつを押し戻す処理をしなければならない

//**************************************************************************************
//　コンピュータ動作時に使用する構造体
//**************************************************************************************
struct WAZAINFO
{
	DWORD walkf;//前進
	DWORD walkb;//後退
	DWORD jampf;//まゑジャンプ
	DWORD jampb;//裏ジャンプ
	DWORD dashf;//まゑダッシュ
	DWORD dashb;//裏ダッシュ

	DWORD att_short[6];//短リーチ攻撃
	DWORD att_middle[6];//中リーチ攻撃
	DWORD att_long[6];//長リーチ攻撃
	DWORD att_jamp[6];//空中で出す攻撃
	DWORD att_tai[6];//対空に使える攻撃
	DWORD att_bullet[6];//飛び道具など

	DWORD nageid[6];	//投げ行動ID

	//＜未実装＞
	//地上から始まる連続技
	int renzoku_maai[8];		//↓の連続技に適した間合い
	DWORD renzoku[8][16][2];	//連続技 [#][技][0:行動ID（ない場合は0）/1:次の行動に移るまでの時間]
	//空中から始まる連続技
	int arenzoku_maai[8];		//↓の連続技に適した間合い
	DWORD arenzoku[8][16][2];	//連続技 [#][技][0:行動ID（ない場合は0）/1:次の行動に移るまでの時間]
};

//ACTION ID===============================================================================
//行動IDは、ここに定義している値とかぶらないように設定すること
//						          //xxxx 0000 00xx 0xxx | xxxx xxxx xxxx xxxx
#define ACTID_KUCYU		0x00010000//0000 0000 0000 0001 | 0000 0000 0000 0000
#define ACTID_SYAGAMI	0x00020000//0000 0000 0000 0010 | 0000 0000 0000 0000
#define ACTID_ATTACK	0x00100000//0000 0000 0001 0000 | 0000 0000 0000 0000
#define ACTID_HISSATU	0x00200000//0000 0000 0010 0000 | 0000 0000 0000 0000
#define ACTID_SYSTEM	0x80000000//1000 0000 0000 0000 | 0000 0000 0000 0000
#define ACTID_KURAI		0x40000000//0100 0000 0000 0000 | 0000 0000 0000 0000
#define ACTID_GUARD		0x20000000//0010 0000 0000 0000 | 0000 0000 0000 0000
#define ACTID_NAGE		0x00040000//0000 0000 0000 0100
#define ACTID_INOUT		0x10000000//0001 0000 0000 0000

#define ACTID_NEUTRAL	1				//ニュートラルポーズ
#define ACTID_TOJYO		2				//登場
#define ACTID_SYORI		3				//勝利
#define ACTID_CROUCH	ACTID_SYAGAMI	//しゃがみポーズ
#define ACTID_RAKKA		ACTID_KUCYU		//落下

//default action id　=======================================================================
#define ACTID_DAMAGE1		(ACTID_SYSTEM | ACTID_KURAI | 0x0001)				 //立ち喰らい（小）
#define ACTID_DAMAGE2		(ACTID_SYSTEM | ACTID_KURAI | 0x0002)				 //立ち喰らい（中）
#define ACTID_DAMAGE3		(ACTID_SYSTEM | ACTID_KURAI | 0x0003)				 //立ち喰らい（大）
#define ACTID_DAMAGEC1		(ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0001)//しゃがみ喰らい（小）
#define ACTID_DAMAGEC2		(ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0002)//しゃがみ喰らい（中）
#define ACTID_DAMAGEC3		(ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0003)//しゃがみ喰らい（大）
#define ACTID_DAMAGEJ1		(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU   | 0x0001)//空中喰らい（小）
#define ACTID_DAMAGEJ2		(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU   | 0x0002)//空中喰らい（中）
#define ACTID_DAMAGEJ3		(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU   | 0x0003)//空中喰らい（大）
#define ACTID_DOWN			(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0004)//足払いで転び
#define ACTID_DOWN2			(ACTID_SYSTEM | ACTID_KURAI | 0x0004)				 //ダウソ
#define ACTID_OKIAGARI		(ACTID_SYSTEM | 0x0001)								 //おきあがり
#define ACTID_FINALDOWN		(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0005)//死
#define ACTID_FUTTOBI		(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0006)//ふっとび
#define ACTID_BOUND			(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0007)//バウンド
#define ACTID_FINALBOUND	(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0008)//バウンド(死亡)
#define ACTID_FINALDOWN2	(ACTID_SYSTEM | ACTID_KURAI | 0x0005)				 //氏
#define ACTID_FUTTOBI2		(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0009)//ふっとび2
#define ACTID_NAGERARE		(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | ACTID_NAGE | 0x0001)//投げられ中
#define ACTID_DAMAGE1A		(ACTID_SYSTEM | ACTID_KURAI | 0x0006)				 //立ち喰らい（小）、のけぞりなし
#define ACTID_DAMAGE2A		(ACTID_SYSTEM | ACTID_KURAI | 0x0007)				 //立ち喰らい（小）、のけぞりなし
#define ACTID_DAMAGE3A		(ACTID_SYSTEM | ACTID_KURAI | 0x0008)				 //立ち喰らい（小）、のけぞりなし
#define ACTID_DAMAGEC1A		(ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0004)//しゃがみ喰らい（小）、のけぞりなし
#define ACTID_DAMAGEC2A		(ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0005)//しゃがみ喰らい（中）、のけぞりなし
#define ACTID_DAMAGEC3A		(ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0006)//しゃがみ喰らい（大）、のけぞりなし
#define ACTID_TATAKITUKE1A	(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000A)  //真下叩きつけ落下中(バウンド有り)
#define ACTID_TATAKITUKE2A	(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000B)  //真下叩きつけ落下中(バウンド無し)
#define ACTID_TATAKITUKE1B	(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000C)  //斜め叩きつけ落下中(バウンド有り)
#define ACTID_TATAKITUKE2B	(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000D)  //斜め叩きつけ落下中(バウンド無し)
#define ACTID_KAITENFINISH	(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000E)  //回転して画面外に吹っ飛んでいく

#define ACTID_GUARD1	(ACTID_SYSTEM | ACTID_GUARD | 0x0001)				 //立ちガード（小）
#define ACTID_GUARD2	(ACTID_SYSTEM | ACTID_GUARD | 0x0002)				 //立ちガード（中）
#define ACTID_GUARD3	(ACTID_SYSTEM | ACTID_GUARD | 0x0003)				 //立ちガード（大）
#define ACTID_GUARDC1	(ACTID_SYSTEM | ACTID_GUARD | ACTID_SYAGAMI | 0x0001)//しゃがみガード（小）
#define ACTID_GUARDC2	(ACTID_SYSTEM | ACTID_GUARD | ACTID_SYAGAMI | 0x0002)//しゃがみガード（中）
#define ACTID_GUARDC3	(ACTID_SYSTEM | ACTID_GUARD | ACTID_SYAGAMI | 0x0003)//しゃがみガード（大）
#define ACTID_GUARDJ1	(ACTID_SYSTEM | ACTID_GUARD | ACTID_KUCYU   | 0x0001)//空中ガード（小）
#define ACTID_GUARDJ2	(ACTID_SYSTEM | ACTID_GUARD | ACTID_KUCYU   | 0x0002)//空中ガード（中）
#define ACTID_GUARDJ3	(ACTID_SYSTEM | ACTID_GUARD | ACTID_KUCYU   | 0x0003)//空中ガード（大）

//デフォルトcell id=====================================================================
#define DCELL_DAMAGES1		1//立ち喰らいポーズ
#define DCELL_DAMAGES2		2
#define DCELL_DAMAGES3		3
#define DCELL_DAMAGES4		4
#define DCELL_DAMAGEC1		5//しゃがみ喰らいポーズ
#define DCELL_DAMAGEC2		6
#define DCELL_DAMAGEC3		7
#define DCELL_DAMAGEC4		8
#define DCELL_GUARDS1		9//立ちガードポーズ
#define DCELL_GUARDS2		10
#define DCELL_GUARDS3		11
#define DCELL_GUARDS4		12
#define DCELL_GUARDC1		13//しゃがみガードポーズ
#define DCELL_GUARDC2		14
#define DCELL_GUARDC3		15
#define DCELL_GUARDC4		16
#define DCELL_DAMAGEJ		17//空中喰らいポーズ
#define DCELL_RAKKA1		18//落下1
#define DCELL_RAKKA2		19//落下2
#define DCELL_GUARDJ1		20//空中ガードポーズ
#define DCELL_GUARDJ2		21
#define DCELL_GUARDJ3		22
#define DCELL_BOUND			23//落下してバウンド
#define DCELL_DOWN			24//ダウン
#define DCELL_OKIAGARI1		25//起き上がりモーション
#define DCELL_OKIAGARI2		26
#define DCELL_OKIAGARI3		27
#define DCELL_OKIAGARI4		28
#define DCELL_OKIAGARI5		29
#define DCELL_NAGERARE1		30//重心が有効
#define DCELL_KAITENFUTTOBI	31//回転吹っ飛び、重心が有効


//******************************************************************************
//　　オブジェクトデータ
//******************************************************************************

typedef DWORD (*PFOBJ_MSGHANDLER)(DWORD,LPVOID,DWORD);
//引数1:メッセージID（下参照）
//引数2:オブジェクトのデータ(GOBJECTのポインタ)
//引数3:各メッセージにより意味が異なる（ほとんど使ってない）
//戻り値：各メッセージにより意味が異なる。0だとデフォルト処理を行う。

struct GOBJECT
{
	DWORD id;			//オブジェクトid
	DWORD uid;			//ユーザーが割り当てるid
	DWORD tid;			//チームid
	DWORD eid;			//敵のid
	PFOBJ_MSGHANDLER msghandler;//メッセージ処理関数
	DWORD objtype;		//オブジェクトの持つ性質
	
	double x,y;			//位置
	double vx,vy;		//速度
	double ax,ay;		//加速度
	
	DWORD counter;		//カウンタ
	DWORD aid;			//行動ID
	DWORD cnow;			//表示セル
	int hp;				//体力
	DWORD hpmax;		//体力の最大値。デフォルトは1024
	double gauge;		//ゲージ
	DWORD gaugemax;		//ゲージのマックス値 1-9の値を指定。タイプ2のゲージは関係なし
	DWORD gaugetype;	//どのパワーゲージを使用するか。下のフラグ。デフォルトはゲージ無し

	BOOL muki;			//向き（FALSEは右向き）
	BOOL revx;			//x反転
	BOOL revy;			//y反転
	int rot;			//回転
	float magx,magy;	//拡大率
	float z;			//z座標
	DWORD color;		//色（自前描画の場合もこの色で描画してください）
	DWORD alphamode;	//合成方法（GBLEND_〜）フラグ

	BOOL kougeki;		//攻撃力on/off
	BOOL kasanari;		//重なり判定on/off
	BOOL muteki;		//無敵on/off
	BOOL nagerare;		//投げられ判定on/off

	MYSURFACE **pmsarr; //ビットマップの配列
	GCD_RECT *prdat;	//矩形情報配列
	GCD_CELL2 *pcdat;	//セル情報配列
	GCD_HANTEI *phdat;	//重なり・当たり・喰らい判定配列

	ATTACKINFO *atk;	//いま自分が持っている攻撃情報
	ATTACKINFO2 atk2;	//他のオブジェクトから喰らった攻撃情報
	WAZAINFO  *winfo;	//コンピュータ用技情報

	LPVOID pobjdat_u;	//ユーザー定義データ
};

//team id
#define TEAM_PLAYER1		0
#define TEAM_PLAYER2		1
#define TEAM_STAGE			2
#define TEAM_SYSTEM			3

//objtype						0xF1000FEF
#define GOBJFLG_KURAI			0x00000001//攻撃を喰らう
#define GOBJFLG_NAGERARE		0x00000002//投げられる
#define GOBJFLG_ATTACK			0x00000004//攻撃を行う
#define GOBJFLG_KASANARI		0x00000008//重なり判定をやる
#define GOBJFLG_TOBETARGET		0x10000000//ターゲットになることができる。
#define GOBJFLG_NEEDTARGET		0x20000000//ターゲット(eid)が必要である。
#define GOBJFLG_COMPUTER		0x40000000//コンピュータに制御させる。通常のキャラクターにはシステムによってこのフラグが付加される
#define GOBJFLG_ZCHARACTER		0x80000000//キャラクター「属性」 - 今のところ特に使っていない
#define GOBJFLG_ZBULLET			0x01000000//飛び道具「属性」- 他の飛び道具と相殺処理する
#define GOBJFLG_DISPZAHYO		0x00000020//座標指定はディスプレイ上の座標であり、ステージ上の表示位置に左右されない
#define GOBJFLG_DONOTSTOP		0x00000040//超必等でstopエフェクトがかかっていてもGOBJMSG_ACTIONが通知される
#define GOBJFLG_HANSAYOU		0x00000080//攻撃したオブジェクトが画面端まで到達すると押し戻される（メッセージを受け取ることができる）
#define GOBJFLG_CLIPX			0x00000100//オブジェクトはx方向に関してステージ(-640〜640)外に行くことはできない
#define GOBJFLG_CLIPX2			0x00000200//オブジェクトはx方向に関して画面外に出ることはしない
#define GOBJFLG_DISPLAYME		0x00000400//ステージ上でディスプレイ中心を算出するときにこのキャラクターの座標を参照する
#define GOBJFLG_TAIKI			0x00000800//キャラクターは待機中である

//標準的なキャラクターが持つフラグ
#define GOBJTYPE_CHARACTER2	( GOBJFLG_KURAI | GOBJFLG_NAGERARE | GOBJFLG_ATTACK | GOBJFLG_KASANARI | GOBJFLG_CLIPX | GOBJFLG_CLIPX2 | GOBJFLG_DISPLAYME)
#define GOBJTYPE_CHARACTER	( GOBJTYPE_CHARACTER2 | GOBJFLG_TOBETARGET | GOBJFLG_NEEDTARGET | GOBJFLG_ZCHARACTER | GOBJFLG_HANSAYOU | GOBJFLG_CLIPX)

//標準的な飛び道具のフラグ
#define GOBJTYPE_BULLET		(GOBJFLG_ATTACK | GOBJFLG_ZBULLET)

//その他
#define GOBJTYPE_DISPLAY	( GOBJFLG_CLIPX | GOBJFLG_CLIPX2 | GOBJFLG_DISPLAYME )
#define GOBJTYPE_NODISPLAY	( ~( GOBJFLG_CLIPX | GOBJFLG_CLIPX2 | GOBJFLG_DISPLAYME ) )
#define GOBJTYPE_DEAD		( ~(GOBJFLG_TOBETARGET | GOBJFLG_NEEDTARGET | GOBJFLG_CLIPX | GOBJFLG_CLIPX2 | GOBJFLG_DISPLAYME) )

//gaugetype
#define GOBJFLGA_POWGAUGE1	0x00000001//9個までストック可能なゲージ
#define GOBJFLGA_POWGAUGE2	0x00000002//ストックできないゲージ

//半透明の場合の合成方法
#define GBLEND_HANTOUMEI		(0)//デフォルト
#define GBLEND_KASAN			(1)
#define GBLEND_INV				(3)

//  〜メッセージ〜  ======================================================================
//msghandler の第2引数の意味===================================================================
//主なメッセージ
#define GOBJMSG_DELETE			   1//オブジェクトがシステムの制御を離れるときに呼ばれる
#define GOBJMSG_ACTION			   2//1フレームに1回呼ばれる
#define GOBJMSG_COMMAND			   3//コマンド判定をする必要があるときに呼ばれる
#define GOBJMSG_COMMANDCOM		   4//コンピュータコマンド判定をするときに呼ばれる
#define GOBJMSG_TOUCHA			   5//攻撃を喰らったときに呼ばれる。
#define GOBJMSG_TOUCHB			   6//自分の攻撃が相手に当たった時に呼ばれる。
#define GOBJMSG_TOUCHC			   7//自分の攻撃が相手に当たったとき (相手の状態が変化する前に呼ばれる)。
#define GOBJMSG_CNGAID			   8//行動IDが変化したときに呼ばれる
//描画系メッセージ
#define GOBJMSG_DRAW			 101//描画を行う際に呼ばれる
#define GOBJMSG_DRAWBACK		 102//背景ステージに描画
#define GOBJMSG_DRAWFRONT		 103//前景ステージに描画
//登場・交代などメッセージ
#define GOBJMSG_DOTOJYO			 201//登場ポーズに移行し、終わったらMSGOBJ2SYS_TOJYOENDメッセージを返せ
#define GOBJMSG_DOTIMEOVERLOSE	 202//タイムオーバーで負けた
#define GOBJMSG_DOYOUWIN		 203//勝利ポーズに移行し、終わったらMSGOBJ2SYS_WINPOSEENDメッセージを返せ
#define GOBJMSG_TAIKI			 204//画面外に待機していてください
#define GOBJMSG_KOUTAI			 205//選手交代。画面の向こうから飛んできてください。（ダメなときは0を返す）
#define GOBJMSG_KOUTAI2			 206//選手交代。画面の向こうから飛んできてください。終わったらMSGOBJ2SYS_KOUTAIENDメッセージを返せ
#define GOBJMSG_STRIKER			 207//ストライカー攻撃をやらかしてください（ダメなときは0を返す）
#define GOBJMSG_DOYOUWIN2		 208//待機中、仲間の勝利によってチームが勝利した
#define GOBJMSG_STRIKERREADY	 209//ストライカー攻撃ができるかどうかを返せ
//座標操作系メッセージ
#define GOBJMSG_KNOCKBACK		 301//攻撃したオブジェクトが画面端に達した
#define GOBJMSG_CLIPX			 302//座標が画面外に逝ってしまっているよ
//オブジェクト間相互作用・その他
#define GOBJMSG_CNGTARGET		 401//ターゲットが変わったときに呼ばれる
#define GOBJMSG_SOUSAI			 402//他飛び道具と接触
//ゲーム進行
#define GOBJMSG_CNGROUND		 502//ラウンドが変わったときに呼ばれる

//ネットワーク
#define GOBJMSG_SYNC			 601//同期メッセージ

//その他
#define GOBJMSG_USERDEFINE		0x80000000//ユーザー定義メッセージ作成用フラグ

//GOBJMSG_TOUCHA メッセージに対する戻り値-------------------------------------------------
//0を返すと勝手に判断します
#define TOUCHA_KURAIS	0x10000001//立ち喰らい
#define TOUCHA_KURAIC	0x10000002//屈喰らい
#define TOUCHA_KURAIJ	0x10000003//空中喰らい
#define TOUCHA_GUARDS	0x20000001//立ちガード
#define TOUCHA_GUARDC	0x20000002//屈ガード
#define TOUCHA_GUARDJ	0x20000003//空中ガード
#define TOUCHA_MUSI		0x50000000//攻撃は喰らったけど、喰らい動作を行わない（0x40000000が無視フラグ）
#define TOUCHA_MUSI2	0x60000000//攻撃をガードしたけど、喰らい動作を行わない（0x40000000が無視フラグ）
#define TOUCHA_AVOID	0x80000000//攻撃をうけなかった

//GOBJMSG_TOUCHC メッセージに対する戻り値-------------------------------------------------------
#define TOUCHC_OK		0x00000000//何もしません。いままで通り処理が進行します。
#define TOUCHC_CANCEL	0x00000001//矩形衝突をなかったことにします。それ以降の処理はなされません。

//GOBJMSG_STRIKERREADYメッセージに対する戻り値 あるいは↓DI_FUNCTIONS_S.strikerokの戻り値-------
#define STRIKERREADY_UNKNOWN		(0)//不明
#define STRIKERREADY_OK				(1)//準備オッケー
#define STRIKERREADY_NOTREADY		(2)//できない
#define STRIKERREADY_NOTSUPPORTED	(3)//つーか、もともとしない
#define STRIKERREADY_NOPOINT		(4)//(sys)ストライカーのポイントが足りない

//**************************************************************************
// システム関連の関数の定義
//**************************************************************************
typedef DWORD  (*PFUNCS_GETKEY)(DWORD,DWORD);			//キー入力get
typedef int    (*PFUNCS_SEEKKEY)(DWORD,int,int,DWORD);	//キー入力get(2)
typedef void   (*PFUNCS_SETKATISERIF)(DWORD,char*);		//勝ち台詞設定
typedef void   (*PFUNCS_ADDEFFECT)(DWORD,int,int,DWORD);//エフェクト発生
typedef void   (*PFUNCS_PLAYSYSSOUND)(DWORD);			//wav再生
typedef LPVOID (*PFUNCS_LOADMYSOUND)(char*);			//wav読み込み
typedef void   (*PFUNCS_PLAYMYSOUND)(LPVOID);			//wav再生
typedef void   (*PFUNCS_KILLMYSOUND)(LPVOID);			//wav消去
typedef DWORD  (*PFUNCS_MESSAGE2SYSTEM)(DWORD,DWORD,DWORD);	//メッセージ
typedef char*  (*PFUNCS_GETENEMYNAME)(DWORD);			//"敵"の名前を取得する
typedef double (*PFUNCS_GETDISPCENTERX)();				//現在の画面中心を得る
typedef double (*PFUNCS_GETDISPCENTERY)();				//現在の画面中心を得る

typedef DWORD  (*PFUNCS_GETTEAMNUM)();					//何対何かをゲット
typedef DWORD  (*PFUNCS_GETTAISENKEISIKI)();			//対戦形式をゲト
typedef DWORD  (*PFUNCS_STRIKEROK)(DWORD tid,DWORD oid,BOOL next);//ストライカー攻撃が可能かどうかを問い詰める

struct DI_FUNCTIONS_S
{
	PFUNCS_GETKEY			getkey;
	PFUNCS_SEEKKEY			seekkey;
	PFUNCS_SETKATISERIF		setkatiserif;
	PFUNCS_ADDEFFECT		addeffect;
	PFUNCS_PLAYSYSSOUND		playsyssound;
	PFUNCS_LOADMYSOUND		loadmysound;
	PFUNCS_PLAYMYSOUND		playmysound;
	PFUNCS_KILLMYSOUND		killmysound;
	PFUNCS_MESSAGE2SYSTEM	msg2system;
	PFUNCS_GETENEMYNAME		getenemyname;
	PFUNCS_GETDISPCENTERX	getdispcenterx;
	PFUNCS_GETDISPCENTERY	getdispcentery;

	PFUNCS_GETTEAMNUM		getteamnum;
	PFUNCS_GETTAISENKEISIKI	gettaisenkeisiki;
//	PFUNCS_STRIKEROK		strikerok;
};

//キー入力定義===========================================================================

#define KEYINPUT_PLAYER1	(0)
#define KEYINPUT_PLAYER2	(1)

//これはあまり使わないでほしい↓
#define KEYSTA_LEFT		(0x00000100)
#define KEYSTA_LEFT2	(0x00000200)
#define KEYSTA_RIGHT	(0x00001000)
#define KEYSTA_RIGHT2	(0x00002000)
#define KEYSTA_ALEFT	(0x00000400)
#define KEYSTA_ALEFT2	(0x00000800)
#define KEYSTA_ARIGHT	(0x00004000)
#define KEYSTA_ARIGHT2	(0x00008000)
//↓ここから使用可能
#define KEYSTA_UP		(0x00000001)
#define KEYSTA_UP2		(0x00000002)
#define KEYSTA_DOWN		(0x00000010)
#define KEYSTA_DOWN2	(0x00000020)
#define KEYSTA_FOWORD	KEYSTA_RIGHT
#define KEYSTA_FOWORD2	KEYSTA_RIGHT2
#define KEYSTA_BACK		KEYSTA_LEFT
#define KEYSTA_BACK2	KEYSTA_LEFT2
#define KEYSTA_BA		(0x00010000)//ボタン1が押されている状態
#define KEYSTA_BA2		(0x00020000)//ボタン1が今押されたところ
#define KEYSTA_BB		(0x00100000)//ボタン2
#define KEYSTA_BB2		(0x00200000)
#define KEYSTA_BC		(0x01000000)//ボタン3
#define KEYSTA_BC2		(0x02000000)
#define KEYSTA_BD		(0x10000000)//ボタン4
#define KEYSTA_BD2		(0x20000000)

#define KEYSTA_ANYKEY	(0x22220000)
#define KEYSTA_BUTTONS	(0x22220000)


//Message Id============================================================================
//システム側へのメッセージ
#define MSGOBJ2SYS_TOJYOEND			1//登場デモが終了した
#define MSGOBJ2SYS_FINALDOWNEND		2//最終ダウンが終了した
#define MSGOBJ2SYS_WINPOSEEND		3//勝利デモが終了した
#define MSGOBJ2SYS_KOUTAI1			4//交代要請(キャラクター1)
#define MSGOBJ2SYS_KOUTAI2			5//交代要請(キャラクター2)
#define MSGOBJ2SYS_STRIKER1			6//ストライカー要請(キャラクター1)
#define MSGOBJ2SYS_STRIKER2			7//ストライカー要請(キャラクター2)
#define MSGOBJ2SYS_KOUTAIEND		8//交代終了。戦闘できる状態です

//Effect Id ============================================================================
//addeffectの第一引数に使える定数。第二引数以降の意味はエフェクトIDにより異なる
							 //引数の意味  /コメント
#define EFCTID_MARK1		1//x,y,向
#define EFCTID_MARK2		2//x,y,向
#define EFCTID_MARK3		3//x,y,向
#define EFCTID_MARK4		4//x,y,向
#define EFCTID_MARKG		5//x,y,向
#define EFCTID_MARKG2		6//x,y,向
#define EFCTID_HIKARI		7//x,y,-
#define EFCTID_STOP			8//ストップしてる時間,-,-
#define EFCTID_DARK			9//暗くなってる時間,-,-  
#define EFCTID_MARKC		10//x,y,-  /相手を掴んだときのマーク
#define EFCTID_GNDCIR		11//x,y,-  /地面に丸い衝撃波
#define EFCTID_SINDO		12//振幅,持続時間,-  /振動が起こる
#define EFCTID_NOBG			13//持続時間,-,-  背景の描画を無しにする
#define EFCTID_CYOHI		14//x,y,硬化時間 /超必エフェクト
#define EFCTID_TUBUTUBU		15//x,y,↓のほう  /パーティクル散乱
#define EFCTID_TUBUTUBU2	16//x,y, 上と同じ /でっかいパーティクル散乱
#define EFCTID_COMEON		17//x,y,向き /come on my brother
#define EFCTID_BURN			18//-,-,オブジェクトID /指定オブジェクトが燃える

//つぶつぶパラメータ入力用マクロ
#define TUBUTUBUPRM(a,b,c,d,e,f,g,h)	((DWORD)(a*256*256*256)*16+(b*256*256*256)+(c*256*256*16)+(d*256*256)+(e*256*16)+(f*256)+(g*16)+h)
//a:粒の数(0〜15)
//b;粒の速度
//c:粒の速度のばらつき
//d:最低速度
//e:初期位置のばらつき
//f:寿命のばらつき
//g:粒の最短寿命
#define TUBUTUBU_GOUP		0x00000001//上方向に偏った速度を持たせる
#define TUBUTUBU_GODOWN		0x00000002//下向に偏った速度を持たせる
#define TUBUTUBU_GOLEFT		0x00000004//左方向に
#define TUBUTUBU_GORIGHT	0x00000008//右

//Sound ID =============================================================================
//pf_playsyssoundに使える定数。システム側で持っている効果音を利用できる

#define SYSTEMSOUND_HIT1	0//hit1.wav ヒット音
#define SYSTEMSOUND_HIT2	1//hit2.wav
#define SYSTEMSOUND_HIT3	2//hit3.wav
#define SYSTEMSOUND_SHOCK1	3//shock1.wav 使おうと思えば使えるが、未使用
#define SYSTEMSOUND_SHOCK2	4//shock2.wav 使おうと思えば使えるが、未使用
#define SYSTEMSOUND_GUARD	5//guard.wav ガード音
#define SYSTEMSOUND_CATCH	6//catch.wav 掴み音
#define SYSTEMSOUND_CYOHI	7//cyohi.wav 超必発動時の効果音
#define SYSTEMSOUND_CHARGE	8//charge.wav ゲージが1本溜まったときの効果音

//対戦形式 PFUNCS_GETTAISENKEISIKIの戻り値
#define TAISENKEISIKI_GOCYAMAZE		1
#define TAISENKEISIKI_KOUTAI		2
#define TAISENKEISIKI_JYUNBAN		3

//**************************************************************************
//　オブジェクト関連の関数の定義
//**************************************************************************
typedef DWORD  (*PFUNCO_CREATE)();						//オブジェクト生成
typedef void   (*PFUNCO_DELETE)(DWORD);					//オブジェクト消滅
typedef BOOL   (*PFUNCO_CATCH)(DWORD,LPVOID);			//投げとかで相手をつかむ
typedef LPVOID (*PFUNCO_GETINFO)(DWORD);				//情報
typedef void   (*PFUNCO_ADDDAMAGE)(DWORD,DWORD,int x,int y);//(主に投げのとき)強制的にダメージを与える
typedef void   (*PFUNCO_ACTIDCHANGED)(DWORD);			//行動IDが変化したときの処理を明示的に行う
typedef void*  (*PFUNCO_GETACTIVECHARACTER)(DWORD);		//現在アクティブなキャラクターの情報
typedef void   (*PFUNCO_SUICIDE)(DWORD);				//オブジェクト消滅
typedef DWORD  (*PFUNCO_GETMAAI_H)(DWORD oid,DWORD eoid);//間合いをget(水平)
typedef DWORD  (*PFUNCO_GETMAAI_V)(DWORD oid,DWORD eoid);//間合いをget(垂直)

typedef DWORD  (*PFUNCO_GETCHARACTERID)(DWORD tid,DWORD index);//キャラクターのオブジェクトID

struct DI_FUNCTIONS_O
{
	PFUNCO_CREATE			objcreate;		//abolished
	PFUNCO_DELETE			objdelete;		//abolished
	PFUNCO_CATCH			objcatch;		//out of order now
	PFUNCO_GETINFO			getinfo;
	PFUNCO_ADDDAMAGE		adddamage;
	PFUNCO_ACTIDCHANGED		actidchanged;
	PFUNCO_GETACTIVECHARACTER getactivechar;//out of order now
	PFUNCO_SUICIDE			suicide;		//abolished
	PFUNCO_GETMAAI_H		getmaai_h;
	PFUNCO_GETMAAI_V		getmaai_v;

	PFUNCO_GETCHARACTERID	getcharid;
};

//　相手をつかむ　========================================================
//catch にCATCHYOU構造体のポインタを入れて実行し、TRUEが返れば投げ開始
//渡した関数が 相手の行動関数になる。表示セルにはデフォルトセルを利用する

struct CATCHYOU
{
	DWORD oid;//掴むオブジェクト
	DWORD actmsg;//掴んだオブジェクトが、つかまれたオブジェクトを操作するために使うメッセージ
};

//**************************************************************************
//　描画関連の関数の定義
//**************************************************************************

//*描画系関数*
typedef LPVOID (*PFUNCD_GETD3D)();							//IDirect3D* の取得
typedef LPVOID (*PFUNCD_GETD3DD)();							//IDirect3DDevice* の取得
typedef void   (*PFUNCD_LOADCELLDAT)(char*,LPVOID,LPVOID,LPVOID);//セルデータ読み込み関数
typedef LPVOID (*PFUNCD_LOADBMP)(char*,char*);				//ビットマップ読み込み関数
typedef void   (*PFUNCD_UNLOADBMP)(LPVOID);					//ビットマップ後始末関数
typedef void   (*PFUNCD_CELLDRAW)(LPVOID*,LPVOID,LPVOID,DWORD,int,int,float,int,BOOL,BOOL,DWORD,float,float);//セル描画関数
typedef void   (*PFUNCD_CKBLT)(void*,int,int,RECT,double,double,BOOL,BOOL,float,DWORD);//Blt2
typedef void   (*PFUNCD_BLT3D)(void*,RECT,MYRECT3D,DWORD);	//Blt3
typedef void   (*PFUNCD_SETTRANSFORM)(BOOL);				//変換行列設定
typedef void   (*PFUNCD_SETBLEND)(DWORD);					//ブレンドの仕方を変更
typedef void   (*PFUNCD_SETPARENTMATRIX)(LPVOID,BOOL,LPVOID);//「親」の変換行列設定
typedef DWORD  (*PFUNCD_CREATEGCDDAT)(char* filename,LPVOID *cdat,LPVOID *rdat,LPVOID *hdat);//セルデータ読み込み。ver0.90以降ではLoadCellDatからこちらに変更するべき
typedef void   (*PFUNCD_DESTROYGCDDAT)(LPVOID *cdat,LPVOID *rdat,LPVOID *hdat);//CreateCellDatで生成されたバッファをクリア

struct DI_FUNCTIONS_D
{
	PFUNCD_GETD3D		getd3d;
	PFUNCD_GETD3DD		getd3dd;
	PFUNCD_LOADCELLDAT	loadcelldat;
	PFUNCD_LOADBMP		loadbmp;
	PFUNCD_UNLOADBMP	unloadbmp;
	PFUNCD_CELLDRAW		celldraw;
	PFUNCD_CKBLT		ckblt;
	PFUNCD_BLT3D		blt3d;
	PFUNCD_SETTRANSFORM settrans;
	PFUNCD_SETBLEND		setblend;
	PFUNCD_SETPARENTMATRIX setparentmat;

	PFUNCD_CREATEGCDDAT create_celldat;
	PFUNCD_DESTROYGCDDAT destroy_celldat;
};


//**************************************************************************
//　ネットワーク関連の関数の定義
//**************************************************************************
typedef DWORD (*PFUNCN_REGISTER_OBJECT)(BYTE dllID,BYTE userID,BYTE userNo);
typedef void (*PFUNCN_SENDSYNCMESSAGE)(DWORD objid,DWORD size,LPVOID dat);
typedef void (*PFUNCN_SET_SYNCRONIZE)(DWORD objid,BOOL enabled);
//typedef DWORD (*PFUNCN_REGIST_USEREFFECT)(BYTE dllno,BYTE userid,BYTE userno);
//typedef void (*PFUNCN_CREATE_USEREFFECT)(DWORD userefctid,float x,float y,DWORD flags1,DWORD flags2);

struct DI_FUNCTIONS_N
{
	PFUNCN_REGISTER_OBJECT obj_regist;			//→o.objcreate
	PFUNCN_SENDSYNCMESSAGE send_syncmsg;		//out of order now
	PFUNCN_SET_SYNCRONIZE set_syncronize;
//	PFUNCN_REGIST_EFFECT  regist_usereffect;
//	PFUNCN_CREATE_USEREFFECT create_usereffect;
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
#define CDI_VERSION 920

//********************************************************************************
//　キャラクターオプション情報定義
//********************************************************************************

struct CHARACTER_LOAD_OPTION
{
	DWORD flag;			//オプションがONの場合に立つフラグ
	DWORD exclusive;	//同時に指定できないほかのオプション
	DWORD depends;		//同時に指定されていなければならないほかのオプション
	char name[64];		//オプション名
	char point;			//オプション消費ポイント
};

//**************************************************************************
// CharacterInfo関数で渡される構造体
//**************************************************************************

typedef DWORD (*PFUNC_CHARACTERINFO)(LPVOID);
struct CDI_CHARACTERINFO
{
	DWORD system_version;		//システムのバージョン。（ver0.9以前は0がはいってるとおもます）
	char dummy[28];
	char name[32];				//ココに名前をコピーすること
	DWORD caps;					//ここに情報を設定（ver0.9以降）
	LPVOID dummy2[3];
	DWORD ver;					//ココにバージョンをコピーすること
	//ver0.9以降で追加
	CHARACTER_LOAD_OPTION options[32];	//キャラ選択時に設定可能なオプション
	DWORD max_option_point;		//オプションの最大値
};

//capsに設定するフラグ
#define CHARACTER_CAPS_NET		0x00000001//ネットワーク対戦で使用可能
#define CHARACTER_CAPS_OFFLINE	0x00000002//↑かつオフラインモードでも使用可能


//********************************************************************************
//　CreateCharacter の引数に渡される構造体
//********************************************************************************
typedef DWORD (*PFUNC_CREATECHARACTER)(LPVOID);
struct CDI_CHARACTERINFO2
{
	char dir[64];//dllがあるディレクトリ
	DWORD tid;//チームID(0:1P側、1:2P側)
	DWORD color;//どの色にするか（pal?.bmp）
	DWORD keyinput;//どこから入力を受け取るか
	DI_FUNCTIONS_S *funcs;
	DI_FUNCTIONS_O *funco;
	DI_FUNCTIONS_D *funcd;
	//ver0.9以降で追加
	BOOL isRemote;			//リモート制御かどうか
	BYTE dllid;
	DI_FUNCTIONS_N *funcn;	//ローカルモードではNULL
	DWORD options_flag;		//選択されたオプションフラグ
};

//**************************************************************************
// ステージDLLのバージョン
//**************************************************************************

#define SDI_VERSION 750


//**************************************************************************
// StageInfo関数で渡される構造体
//**************************************************************************

typedef DWORD (*PFUNC_STAGEINFO)(LPVOID);
struct SDI_STAGEINFO
{
	char name[32];
	DWORD ver;
};

//**************************************************************************
// CreateStage関数で渡される構造体
//**************************************************************************

typedef DWORD (*PFUNC_CREATESTAGE)(LPVOID);
struct SDI_STAGEINFO2
{
	char dir[64];//dllがあるディレクトリ
	DWORD tid;//チームID(常に2(ステージ))
	DI_FUNCTIONS_S *funcs;
	DI_FUNCTIONS_O *funco;
	DI_FUNCTIONS_D *funcd;
};

#endif//_GOBJECT_H_