
#define DLL_VERSION	420

struct CHARACTERINFO
{
	char dir[32];
	char name[32];
	LPDIRECTDRAWSURFACE dds_face;
	LPDIRECTDRAWSURFACE dds_face2;
	int storyindex,storynum;
	DWORD ver;
};

typedef void (*PFUNC_BASIC)(void);

//system > dll function =====================================================================
typedef DWORD (*PFUNC_GETKEY)(DWORD,DWORD);			//キー入力get
typedef void  (*PFUNC_FURIMUKI)(DWORD);				//振り向き処理
typedef void  (*PFUNC_STARTATTACK)(DWORD,LPVOID);	//攻撃開始
typedef int   (*PFUNC_SEEKKEY)(DWORD,int,int,DWORD);//キー入力get
typedef void  (*PFUNC_SETKATISERIF)(DWORD,char*);	//勝ち台詞設定
typedef void  (*PFUNC_PLAYMYSOUND)(DWORD,DWORD);	//wav再生
typedef void  (*PFUNC_GOBULLETA)(LPVOID);			//飛び道具発射
typedef void  (*PFUNC_ADDEFFECT)(DWORD,int,int,DWORD);//エフェクト発生
typedef void  (*PFUNC_SETWAZAINFO)(DWORD,LPVOID);	//コンピュータに使える技を教える
typedef void  (*PFUNC_DRAWFRONT)(DWORD,BOOL);		//自分の方を手前(後)に表示
typedef BOOL  (*PFUNC_CATCHENEMY)(DWORD,LPVOID);	//投げとかで相手をつかむ
typedef void  (*PFUNC_ENEMYNAME)(DWORD,char*);		//相手の名前get
typedef void  (*PFUNC_GETENEMYDAT)(DWORD,LPVOID);	//相手の情報get
typedef void  (*PFUNC_GETMAAI)(DWORD,DWORD*,DWORD*,DWORD*);//間合いをget（cid,水平,垂直,敵状態）
typedef void  (*PFUNC_PLAYSYSSOUND)(DWORD);			//wav再生
typedef void  (*PFUNC_SETWAZAINFO2)(DWORD,LPVOID);	//コンピュータに使える技を教える(2)
typedef void  (*PFUNC_SETCHARACTERINFO3)(DWORD,LPVOID);//CHARACTERINFO3の構造体を渡す
typedef void  (*PFUNC_ADDDAMAGE)(DWORD,LPVOID,int x,int y);		//(主に投げのとき)強制的にダメージを与える
typedef void  (*PFUNC_EXTWINDURATION)(DWORD);		//勝利ポーズの表示時間を延長する
typedef BOOL  (*PFUNC_ADDPOWERGAUGE)(DWORD,double); //パワーゲージを増やす、または減らす。ゲージが足りない場合FALSEが返る
typedef void  (*PFUNC_DESTROYBULLET)(LPVOID);		//飛び道具を消去する
typedef char* (*PFUNC_GETDIRECTORY)(DWORD);		//ディレクトリーゲット

struct FUNCTIONPACK
{
	PFUNC_GETKEY			pf_getkey;
	PFUNC_FURIMUKI			pf_furimuki;
	PFUNC_STARTATTACK		pf_startattack;
	PFUNC_SEEKKEY			pf_seekkey;
	PFUNC_SETKATISERIF		pf_setkatiserif;
	PFUNC_PLAYMYSOUND		pf_playmysound;
	PFUNC_GOBULLETA			pf_gobulleta;
	PFUNC_ADDEFFECT			pf_addeffect;
	PFUNC_SETWAZAINFO		pf_setwazainfo;
	PFUNC_DRAWFRONT			pf_drawfront;
	PFUNC_CATCHENEMY		pf_catchenemy;
	PFUNC_ENEMYNAME			pf_enemyname;
	PFUNC_GETENEMYDAT		pf_getenemydat;
	PFUNC_GETMAAI			pf_getmaai;
	PFUNC_PLAYSYSSOUND		pf_playsyssound;
	PFUNC_SETWAZAINFO2		pf_setwazainfo2;
	PFUNC_SETCHARACTERINFO3	pf_setcharinfo3;
	PFUNC_ADDDAMAGE			pf_adddamage;
	PFUNC_EXTWINDURATION	pf_extwindur;
	PFUNC_ADDPOWERGAUGE		pf_addpowgauge;
	PFUNC_DESTROYBULLET		pf_destroybullet;
	PFUNC_GETDIRECTORY		pf_getdirectory;
};

//Characer Info===============================================================================

//dll > system function
typedef DWORD (*PFUNC_CHARACTERINFO)(LPVOID);
typedef DWORD (*PFUNC_CREATECHAR)(DWORD,LPVOID);
typedef BOOL  (*PFUNC_COMMANDB)(DWORD);
typedef DWORD (*PFUNC_TOUCH)(LPVOID);
typedef DWORD (*PFUNC_TOUCH2)(LPVOID,BOOL);

struct CHARACTERINFO2
{
	HINSTANCE hlib;
	DWORD cid;		//0 or 1
	PFUNC_BASIC		initchar;//ラウンド開始時に呼ばれる関数
	PFUNC_BASIC		command_a;
	PFUNC_COMMANDB	command_b;
	PFUNC_BASIC		action;
	PFUNC_TOUCH		touch_a;//敵の攻撃が当たったときに呼ばれる関数
	PFUNC_TOUCH2	touch_b;//自分の攻撃が当たったときに呼ばれる関数
	PFUNC_BASIC		deletechar;//キャラクター始末時に呼ばれる関数

	int hp;
	double gauge;

	DWORD aid;
	DWORD cnow;
	DWORD counter;
	double x,y,vx,vy;
	BOOL muki;
	BOOL revx;//x方向反転表示
	BOOL revy;//y方向反転表示
	BOOL muteki;//無敵かどうか
	BOOL nagerare;//投げられ判定

	FUNCTIONPACK *fpack;
};

//return value of touch-a
#define TOUCHA_KURAIS	0x10000001//立ち喰らい
#define TOUCHA_KURAIC	0x10000002//屈喰らい
#define TOUCHA_KURAIJ	0x10000003//空中喰らい
#define TOUCHA_GUARDS	0x20000001//立ちガード
#define TOUCHA_GUARDC	0x20000002//屈ガード
#define TOUCHA_GUARDJ	0x20000003//空中ガード
#define TOUCHA_MUSI		0x50000000//攻撃は喰らったけど、デフォルト動作を行わない
#define TOUCHA_MUSI2	0x60000000//攻撃をガードしたけど、デフォルト動作を行わない

//キャラクター情報========================================================================
// pf_setcharinfo3 に渡す。キャラクターの設定を行う。

struct CHARACTERINFO3
{
	DWORD hpmax;//体力の最大値。デフォルトは1024

	DWORD pgauge;//どのパワーゲージを使用するか。下のフラグ。デフォルトはゲージ無し。
	DWORD pgaugemax;//ゲージのマックス値 1-9の値を指定。タイプ2のゲージは関係なし。
	double ppgauge_hit;//相手の攻撃を喰らったときに増加するゲージ量。
	double ppgauge_guard;//相手の攻撃をガードしたときに増加するゲージ量。
};

#define CINFO3_POWGAUGE1	0x00000001//9個までストック可能なゲージ
#define CINFO3_POWGAUGE2	0x00000002//ストックできないゲージ

//ACTION ID===============================================================================
//行動IDは、ここに定義している値とかぶらないように設定すること
//						          //xxx0 0000 00xx 0xxx | xxxx xxxx xxxx xxxx
#define ACTID_KUCYU		0x00010000//0000 0000 0000 0001 | 0000 0000 0000 0000
#define ACTID_SYAGAMI	0x00020000//0000 0000 0000 0010 | 0000 0000 0000 0000
#define ACTID_ATTACK	0x00100000//0000 0000 0001 0000 | 0000 0000 0000 0000
#define ACTID_HISSATU	0x00200000//0000 0000 0010 0000 | 0000 0000 0000 0000
#define ACTID_SYSTEM	0x80000000//1000 0000 0000 0000 | 0000 0000 0000 0000
#define ACTID_KURAI		0x40000000//0100 0000 0000 0000 | 0000 0000 0000 0000
#define ACTID_GUARD		0x20000000//0010 0000 0000 0000 | 0000 0000 0000 0000
#define ACTID_NAGE		0x00040000//0000 0000 0000 0100

#define OLDACTID_NEUTRAL	0				//ニュートラルポーズ
#define OLDACTID_TOJYO		0x0001			//登場
#define OLDACTID_SYORI		0x0002			//勝利
#define ACTID_CROUCH	ACTID_SYAGAMI	//しゃがみポーズ
#define ACTID_RAKKA		ACTID_KUCYU		//落下

//default action id
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
#define ACTID_FINALBOUND	(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0008)//バウンド
#define ACTID_FINALDOWN2	(ACTID_SYSTEM | ACTID_KURAI | 0x0005)				 //氏
#define ACTID_FUTTOBI2		(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0009)//ふっとび2
#define ACTID_NAGERARE		(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | ACTID_NAGE | 0x0001)//投げられ中
#define ACTID_DAMAGE1A		(ACTID_SYSTEM | ACTID_KURAI | 0x0006)				 //立ち喰らい（小）、のけぞりなし
#define ACTID_DAMAGE2A		(ACTID_SYSTEM | ACTID_KURAI | 0x0007)				 //立ち喰らい（小）、のけぞりなし
#define ACTID_DAMAGE3A		(ACTID_SYSTEM | ACTID_KURAI | 0x0008)				 //立ち喰らい（小）、のけぞりなし
#define ACTID_DAMAGEC1A		(ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0004)//しゃがみ喰らい（小）、のけぞりなし
#define ACTID_DAMAGEC2A		(ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0005)//しゃがみ喰らい（中）、のけぞりなし
#define ACTID_DAMAGEC3A		(ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0006)//しゃがみ喰らい（大）、のけぞりなし
#define ACTID_TATAKITUKE1A	(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000A)//真下叩きつけ落下中(バウンド有り)
#define ACTID_TATAKITUKE2A	(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000B)//真下叩きつけ落下中(バウンド無し)
#define ACTID_TATAKITUKE1B	(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000C)//斜め叩きつけ落下中(バウンド有り)
#define ACTID_TATAKITUKE2B	(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000D)//斜め叩きつけ落下中(バウンド無し)

#define ACTID_GUARD1	(ACTID_SYSTEM | ACTID_GUARD | 0x0001)				 //立ちガード（小）
#define ACTID_GUARD2	(ACTID_SYSTEM | ACTID_GUARD | 0x0002)				 //立ちガード（中）
#define ACTID_GUARD3	(ACTID_SYSTEM | ACTID_GUARD | 0x0003)				 //立ちガード（大）
#define ACTID_GUARDC1	(ACTID_SYSTEM | ACTID_GUARD | ACTID_SYAGAMI | 0x0001)//しゃがみガード（小）
#define ACTID_GUARDC2	(ACTID_SYSTEM | ACTID_GUARD | ACTID_SYAGAMI | 0x0002)//しゃがみガード（中）
#define ACTID_GUARDC3	(ACTID_SYSTEM | ACTID_GUARD | ACTID_SYAGAMI | 0x0003)//しゃがみガード（大）
#define ACTID_GUARDJ1	(ACTID_SYSTEM | ACTID_GUARD | ACTID_KUCYU   | 0x0001)//空中ガード（小）
#define ACTID_GUARDJ2	(ACTID_SYSTEM | ACTID_GUARD | ACTID_KUCYU   | 0x0002)//空中ガード（中）
#define ACTID_GUARDJ3	(ACTID_SYSTEM | ACTID_GUARD | ACTID_KUCYU   | 0x0003)//空中ガード（大）

//キー入力定義===========================================================================
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
#define KEYSTA_BA		(0x00010000)//ボタン1
#define KEYSTA_BA2		(0x00020000)
#define KEYSTA_BB		(0x00100000)//ボタン2
#define KEYSTA_BB2		(0x00200000)
#define KEYSTA_BC		(0x01000000)//ボタン3
#define KEYSTA_BC2		(0x02000000)
#define KEYSTA_BD		(0x10000000)//ボタン4
#define KEYSTA_BD2		(0x20000000)

//#define KEYSTA_ANYKEY	(0x22220000)


//攻撃情報=========================================================================

typedef void (*PF_ACTUSERKURAI)(LPVOID pedat);//ユーザー定義喰らい処理のための関数ポインタの型

struct MYATTACKINFO
{
	DWORD hit;		//ヒット情報。下のフラグ(HITINFO_～)の論理和を指定
	DWORD guard;	//ガード情報。下のフラグ(GUARDINFO_～)の論理和を指定
	DWORD id;		//キャラクタ側で勝手に定義して
	double pg_hit;	//攻撃ヒット時のゲージ増加量
	double pg_guard;//攻撃ガードされたときのゲージ増加量

	DWORD damage;	//ダメージ
	DWORD kezuri;	//削り
	DWORD dur;		//持続時間

	BOOL muki;		//相手がのけぞる向き
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

#define HITINFO_MARK1		0x00010000//ヒットしたときにちっちゃいマークを表示します
#define HITINFO_MARK2		0x00020000//ヒットしたときに中ぐらいのマークを表示します
#define HITINFO_MARK3		0x00030000//ヒットしたときにでっかいマークを表示します
#define HITINFO_MARK4		0x00040000//ヒットしたときにさらにでっかいマークを表示します(未実装)

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

//guard
#define GUARDINFO_XSTAND	0x10000000//立ちガードできません
#define GUARDINFO_XCROUCH	0x20000000//しゃがみガードできません
#define GUARDINFO_XJAMP		0x40000000//空中ガードできません

#define GUARDINFO_REACT1	0x00000001//弱ガード
#define GUARDINFO_REACT2	0x00000002//中ガード
#define GUARDINFO_REACT3	0x00000003//強ガード

#define GUARDINFO_SIV1		0x01000000//ヒットストップ、短い
#define GUARDINFO_SIV2		0x02000000//ヒットストップ、ふつう
#define GUARDINFO_SIV3		0x03000000//ヒットストップ、長い
#define GUARDINFO_STOP		0x04000000//ヒットストップ、めちゃ長

#define GUARDINFO_EFCTSINDO	0x00001000//攻撃が当たったとき、地面を振動させる

//相手をつかむ==========================================================================
//pf_catchenemy にCATCHYOU構造体のポインタを入れて実行し、TRUEが返れば投げ開始
//渡した関数が 相手の行動関数になる。表示セルは下のほうにあるデフォルトセルIDを利用できる

//戻り値が0の場合はつかみ続行。それ以外の値ならばつかみ終了時の相手の行動ID（上のほうに定義してあるヤツ）
typedef DWORD (*PF_ACTNAGERARE)(LPVOID pedat);

struct OLDCATCHYOU
{
	PF_ACTNAGERARE pf_nagerare;
};

// 飛び道具情報 ========================================================================
//pf_gobulleta にこの構造体のポインタを渡すと飛び道具発射。エフェクトとしても使える。

//飛び道具に自立行動をさせるのならば必要
typedef void	(*PFUNC_BULAACTION)(LPVOID);//1フレームに1回呼ばれる
typedef DWORD	(*PFUNC_BULAHIT)(LPVOID);//なんかに当たったときに呼ばれる。戻り値が0以外ならその後消滅処理
struct BULLETINFO_B
{
	DWORD id;
	PFUNC_BULAACTION	pf_action;
	PFUNC_BULAHIT		pf_hit;
};

//pf_gobulleta にこの構造体のポインタを渡すと飛び道具発射。エフェクトとしても使える。

struct BULLETINFO_A
{
	double x;
	double y;
	BOOL muki;
	MYATTACKINFO atk;
	int dur;//持続時間

	DWORD cid;//どちらが撃ったものか
	DWORD type;//下にあるフラグを立てるべし
	int *cell_run;
	int *cell_dis;
	DWORD spd_run;
	DWORD spd_dis;
	double vx;
	double vy;
	double ax;
	double ay;

	//システム側で勝手に使う変数。いじる必要なし
	DWORD state;//状態。自動で動いているときは、ここが0だと消滅
	DWORD counter;
	DWORD cnow;
	DWORD cnow2;

	BULLETINFO_B *info_b;
};

#define BULLETA_VSHUMAN		0x00000001//相手キャラクターに当たる
#define BULLETA_VSBULLET	0x00000002//相手の出した飛び道具と当たる
#define BULLETA_DONOTDIE	0x00000004//衝突時も消滅しないで飛んでいく
#define BULLETA_DRAWBACK	0x00000008//人物よりも後ろに描画する
#define BULLETA_XJIMENN		0x00010000//地面に落っこちると消滅する
#define BULLETA_XGAMENGAI	0x00020000//画面外に達すると消滅（未実装）
#define BULLETA_JIMAESYORI	0x10000000//info_bメンバが有効であり、処理を自前で行う（未実装）
#define BULLETA_DISPZAHYO	0x00000010//座標指定はディスプレイ上の座標。ステージ上の表示位置に左右されない
#define BULLETA_DONOTSTOP	0x00000020//超必等でstopエフェクトがかかっていても描画が止まらない

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
#define DCELL_NAGERARE1		30//水平/十字の位置に体の重心がくるように
#define DCELL_NAGERARE2		31//右上
#define DCELL_NAGERARE3		32//真上
#define DCELL_NAGERARE4		33//左上
#define DCELL_NAGERARE5		34//地面に到達

//Effect Id ============================================================================
//pf_addeffectの第一引数に使える定数。第二引数以降の意味はエフェクトIDにより異なる

							 //引数の意味  /コメント
#define EFCTID_MARK1		1//x,y,-
#define EFCTID_MARK2		2//x,y,-
#define EFCTID_MARK3		3//x,y,-
#define EFCTID_MARK4		4//(未)
#define EFCTID_MARKG		5//x,y,-
#define EFCTID_MARKG2		6//x,y,-
#define EFCTID_HIKARI		7//x,y,?
#define EFCTID_STOP			8//ストップしてる時間,-,-
#define EFCTID_DARK			9//暗くなってる時間,-,-  /重いし、あんまり使わないほうがいい
#define EFCTID_MARKC		10//x,y,-  /相手を掴んだときのマーク
#define EFCTID_GNDCIR		11//x,y,-  /地面に丸い衝撃波
#define EFCTID_SINDO		12//振幅,持続時間,-  /振動が起こる
#define EFCTID_NOBG			13//持続時間,-,-  背景の描画を無しにする
#define OLDEFCTID_CYOHI		1000//x,y,硬化時間 /超必エフェクト

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

#define MAXNUM_CHARACTERSOUND 32//キャラクターが使用できる最大のサウンド数

//Computer Action Info ================================================================
//pf_setwazainfo(2)に使用する構造体.コンピュータ動作時に使用する

struct OLDWAZAINFO
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
};

struct WAZAINFO2
{
	DWORD nageid[8];	//投げ行動ID
	OLDCATCHYOU cy[8];		//投げ時に使う構造体

	int renzoku_maai[16];		//↓の連続技に適した間合い
	DWORD renzoku[16][2][16];	//連続技 [16個まで登録可能][0:行動ID（ない場合は0）/1:次の行動に移るまでの時間][1つの連続技につき、16個までのの技]
};
