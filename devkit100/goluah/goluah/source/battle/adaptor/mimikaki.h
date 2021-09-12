
// 旧バージョンと新バージョンの情報交換
void Exchange(GOBJECT* pNew, CHARACTERINFO2* pOld, BOOL isOldToNew);

//***************************************************
//　廃止された関数の代わり
//***************************************************

void Furimuki(DWORD oid);
void InitializeMySound();
void PlayMySound(DWORD soundno);
void LoadAndSetKatiSerif(DWORD tid);
void UnloadMySound();
void DrawFront(GOBJECT *pdat,BOOL fr);
void AddPowerGauge(GOBJECT *pdat,double plus);
void InitializeGCDandBMP(GOBJECT *pdat,DWORD color);
void UnloadMyBitmaps(GOBJECT *pdat);

#define MAXMYSOUND 32//sound31.wavまでを読みこむ
/*
struct MYATTACKINFO
{
	DWORD hit;		//ヒット情報。下のフラグ(HITINFO_～)の論理和を指定
	DWORD guard;	//ガード情報。下のフラグ(GUARDINFO_～)の論理和を指艇
	DWORD objid;	//攻撃を行ったオブジェクトのID
	DWORD id;		//キャラクタ側で勝手に定義して
	double pg_hit;	//攻撃ヒット時のゲージ増加量
	double pg_guard;//攻撃ガードされたときのゲージ増加量
	DWORD damage;	//ダメージ
	DWORD kezuri;	//削り
	BOOL muki;		//相手がのけぞる向き
	//↑ATTACKINFOと全く同じ ↓廃止されたメンバ
	DWORD dur;
};*/
void StartAttack(DWORD oid,MYATTACKINFO *ainfo);

ATTACKINFO* ResisterAttackInfo(DWORD oid,MYATTACKINFO *mainfo);
void GetMyAttackInfo(DWORD oid, const ATTACKINFO *ainfo,
							 MYATTACKINFO* mainfo, BOOL IsMyObject);

//***************************************************
//  自動飛び道具
//***************************************************
/*
struct BULLETINFO_A
{
	double x;
	double y;
	BOOL muki;
	MYATTACKINFO atk;
	int dur;//持続時間

	DWORD type;//下にあるフラグを指定
	int *cell_run;
	int *cell_dis;
	DWORD spd_run;
	DWORD spd_dis;
	double vx;
	double vy;
	double ax;
	double ay;

	DWORD state;//状態。ここが0だと消滅
	DWORD cid;

	LPVOID info_b;
};
*/
#define BULLETA_VSHUMAN		0x00000001//敵キャラクターに当たる
#define BULLETA_VSBULLET	0x00000002//敵飛び道具と相殺する
#define BULLETA_DONOTDIE	0x00000004//衝突後も消滅しないで飛んでいく
#define BULLETA_DRAWBACK	0x00000008//人物よりも後ろに描画する
#define BULLETA_DRAWMIDDLE	0x00000040//キャラクター・ヒットマーク間に描画
#define BULLETA_XJIMENN		0x00010000//地面に落っこちると消滅する
#define BULLETA_XGAMENGAI	0x00020000//画面外に達すると消滅
#define BULLETA_JIMAESYORI	0x10000000//info_bメンバが有効であり、処理を自前で行う
#define BULLETA_DISPZAHYO	0x00000010//座標指定はディスプレイ上の座標。ステージ上の表示位置に左右されない
#define BULLETA_DONOTSTOP	0x00000020//超必等でstopエフェクトがかかっていても描画が止まらない

void GoBulletA(GOBJECT *pdat,BULLETINFO_A *bulinfo);//自動飛び道具発射
DWORD BulletACallBack(DWORD msg,LPVOID dat,DWORD prm);//自動飛び道具のメッセージ処理関数
DWORD JimaeBulletACallBack(DWORD msg,LPVOID dat,DWORD prm);//自動飛び道具（自立行動タイプ）のメッセージ処理関数

struct STRUCT_BULLETA//自動飛び道具用データ
{
	int *cell_run;
	int *cell_dis;
	int num_run;
	int num_dis;
	DWORD spd_run;
	DWORD spd_dis;
	int dur;//持続時間
	BULLETINFO_A *pbinfo_a;//消滅したらstateを0にする
	DWORD poid;//発射元オブジェクト
	DWORD flags;//指定されたフラグ
	WORD ObjectID;
};

//飛び道具の状態
#define BULLETASTATE_RUN	1//飛翔中
#define BULLETASTATE_RUN2	2//攻撃力を持たない飛翔中
#define BULLETASTATE_DIS	3//消滅中

#define MYGOBJMSG_BULLETHIT	(GOBJMSG_USERDEFINE | 0xB1E0)//飛び道具ヒットしたよメッセージ

typedef struct tagBULLETATKINFO
{
	GOBJECT* pbul;
	BOOL hit;
} BULLETATKINFO;
