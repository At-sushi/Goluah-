// Goluah キャラクターDLL用ランタイム
#ifndef CDI_VERSION
#include "gobject.h"
#endif

#ifdef CRUNT090_EXPORTS
#define RUNTIME_EXPORT __declspec(dllexport)
#else
#pragma comment(lib, "crunt090.lib")
#define RUNTIME_EXPORT
#endif // CRUNT090_EXPORTS

#ifndef _D3D8_H_
typedef LPVOID LPDIRECT3D8;
typedef LPVOID LPDIRECT3DDEVICE8;
#endif // _D3D8_H_
#ifndef __D3DX8MATH_H__
typedef LPVOID LPD3DXMATRIX;
#endif
#if CDI_VERSION < 900
struct CHARACTER_LOAD_OPTION{};
#endif
#if GCD_VERSION < 900
struct GCD_CELL2_070 : public GCD_CELL2 {};
#endif

/********************************************************
  キャラクター情報設定クラス
  ネット対応：
	コンストラクタ６番目の引数をTRUEに設定してください。
　********************************************************/

class RUNTIME_EXPORT CCharacterInfo  
{
public:
	CCharacterInfo(LPCTSTR CharName, DWORD CdiVersion, CHARACTER_LOAD_OPTION* Options = NULL,
		int num_Options = 0, DWORD max_option_point = 0, BOOL isNetworkable = FALSE);
	virtual ~CCharacterInfo();

	BOOL DllCharacterInfo(LPVOID info);
	void DllCreateCharacter(PVOID info);

private:
	void SetOptionItems(CDI_CHARACTERINFO *pif);
	BOOL m_isNetworkable;
	DWORD version;
	LPTSTR charactername;
	CHARACTER_LOAD_OPTION* m_Options;
	int m_NumOptions;
	DWORD m_MaxPoint;
};

// 設定用マクロ
#define SET_CHARACTERINFO(cinfo, Type) \
extern "C" BOOL CharacterInfo(LPVOID info) {\
	return cinfo.DllCharacterInfo(info);\
}\
extern "C" DWORD CreateCharacter(PVOID info) {\
	cinfo.DllCreateCharacter(info);\
	Type* pc = new Type((CDI_CHARACTERINFO2*)info);\
	return(pc->GetObjectID());\
}

/********************************************************
  Goluahオブジェクト基本クラス
  ネット対応：
	引数あるほうのコンストラクタで構築してください
	デフォルトでは、オブジェクトID 0xXXXXFFFFから下って作成するので
	ユーザーＩＤに0xF*あたりは指定しないでください
　********************************************************/

class RUNTIME_EXPORT CGoluahObject
{
public:
	CGoluahObject();
	CGoluahObject(CDI_CHARACTERINFO2 *info,BYTE userID = 0xFF,BYTE userNo = 0xFF);
	virtual ~CGoluahObject();
	DWORD GetObjectID();
	GOBJECT* GetGObject();
	char* GetCharDir() { return g_chardir; }
	static DWORD gMessageToObject(DWORD msg,LPVOID pd,DWORD prm);
	virtual DWORD Message(DWORD msg,LPVOID pd,DWORD prm);
private:
	void CGoluahObjectCreate();//非ネットワーク初期化処理

protected:
	virtual DWORD Action();//GOBJMSG_ACTIONに対応
	virtual void Command();//GOBJMSG_COMMANDに対応
	virtual DWORD CommandCOM(DWORD wid);//GOBJMSG_COMMANDCOMに対応
	virtual DWORD TouchA(ATTACKINFO *info,DWORD ta_eid);//GOBJMSG_TOUCHAに対応
	virtual DWORD TouchB(ATTACKINFO *info,BOOL hit);//GOBJMSG_TOUCHBに対応
	virtual DWORD TouchC(ATTACKINFO *info,DWORD tc_eid);//GOBJMSG_TOUCHCに対応
	virtual DWORD Draw();//GOBJMSG_DRAWに対応
	virtual DWORD DrawBack();//GOBJMSG_DRAWBACKに対応
	virtual DWORD DrawFront();//GOBJMSG_DRAWFRONTに対応
	virtual void ActionIDChanged();//GOBJMSG_CNGAIDに対応

	double zurex(double x);
	int zurex(int x);
	void movex(double dx);
	void movex(int dx);

	void AddProperty(DWORD prop);
	void RemoveProperty(DWORD prop);
	void SetProperty(DWORD prop);

	void ChangeAction(DWORD actid);

	BOOL IsRemote(){return m_remote;}
	BOOL IsNetwork(){return m_network;}

	// システム関連の関数
	static DWORD GetKey(DWORD keyinput, DWORD interval);
	static int SeekKey(DWORD keyinput, int offset, int delay, DWORD keystate);
	static void SetKatiSerif(DWORD tid, char* serif);
	static void AddEffect(DWORD EffectID, int prm1, int prm2, DWORD prm3);
	static void PlaySysSound(DWORD SoundNo);
	static LPVOID LoadMySound(char* pathname);
	static void PlayMySound(LPVOID psound);
	static void KillMySound(LPVOID psound);
	DWORD Message2System(DWORD mes, DWORD prm);
	static char* GetEnemyName(DWORD tid);
//	static char* GetCharacterName(DWORD oid);
	static double GetDisplayCenter_X();
	static double GetDisplayCenter_Y();
	static DWORD GetTeamNum();
	static DWORD GetTaisenKeisiki();

	// オブジェクト関連の関数
	BOOL   ObjCatch(DWORD eid,DWORD msg_nage);			//投げとかで相手をつかむ
	static GOBJECT* GetInfo(DWORD oid);					//情報ゲッツ
	void   AddDamage(DWORD eid,int x,int y);			//(主に投げのとき)強制的にダメージを与える
	static GOBJECT*  GetActiveCharacter(DWORD tid);		//現在アクティブなキャラクターの情報
	void   Suicide();									//オブジェクト消滅
	static DWORD  GetMaai_H(DWORD oid, DWORD eoid);		//間合いをget(水平)
	static DWORD  GetMaai_V(DWORD oid, DWORD eoid);		//間合いをget(垂直)
	static DWORD  GetCharacterID(DWORD tid,DWORD index);//キャラクターのオブジェクトID

	//　描画関連の関数の定義
	static LPDIRECT3D8 GetD3D();							//IDirect3D* の取得
	static LPDIRECT3DDEVICE8 GetD3DDevice();				//IDirect3DDevice* の取得
	static void   LoadCellData(char* pathname,GCD_CELL2_070* pCells,GCD_RECT* pRects,GCD_HANTEI* pHanteis);//セルデータ読み込み関数
	static MYSURFACE* LoadBitmap(char* PathName,char* PalletFileName);				//ビットマップ読み込み関数
	static void   UnloadBitMap(MYSURFACE* Bitmap);			//ビットマップ後始末関数
	static void   CellDraw(MYSURFACE** pBmps,GCD_CELL2* cdat,GCD_RECT* rdat,		//セル描画関数
		DWORD cnum,int x,int y,float z,int Rotate,BOOL ReverseX,BOOL ReverseY,DWORD Color,float magx,float magy);
	static void   CkBlt(MYSURFACE* pBmp,int x1,int y1,RECT bltrect,
		double magx,double magy,BOOL revx,BOOL revy,float z,DWORD color);			//Blt2
	static void   Blt3D(MYSURFACE* pBmp,RECT bltrect,MYRECT3D rect,DWORD color);	//Blt3
	static void   SetTransform(BOOL BonVoyage);				//変換行列設定
	static void   SetBlend(DWORD type);						//ブレンドの仕方を変更
	static void   SetParentMatrix(LPD3DXMATRIX pMatrix,BOOL erase,LPD3DXMATRIX OldMatrix);	//「親」の変換行列設定
	static DWORD  CreateCellData(char* filename,GCD_CELL2 **cdat,GCD_RECT **rdat,GCD_HANTEI **hdat);//セルデータ読み込み。ver0.90以降ではLoadCellDatからこちらに変更するべき
	static void   DestroyCellData(GCD_CELL2 **cdat,GCD_RECT **rdat,GCD_HANTEI **hdat);		//CreateCellDatで生成されたバッファをクリア

	//　ネットワーク関連の関数の定義
	void SendSynchronizedMessage(DWORD size,LPVOID dat);	// ネットワーク同期メッセージ送信
	void SetSynchronize(BOOL enabled);						// ネットワーク情報通信のON/OFF

protected:
	float base_z;
	BYTE dllID;

	BOOL m_remote;
	BOOL m_network;
	char *g_chardir;

private:
	BOOL m_hasDefaultID;
	DWORD oid;
	GOBJECT *pdat;

	static unsigned short gCreateCounter;
	static int m_ObjCount;
	static CGoluahObject* pObjToDelete;
	static CGoluahObject* pObjDeleting;
};

inline DWORD CGoluahObject::GetObjectID(){return(oid);}
inline GOBJECT* CGoluahObject::GetGObject(){return(pdat);}

/**********************************************************************
　キャラクターオブジェクト基本クラス
　ネット：
		２番目のコンストラクタを使ってください。
  *********************************************************************/

#define CBASE_NUMMYSOUND	(32)//デフォルトで32番までのwavを読み込みます

class RUNTIME_EXPORT CCharacterBase : public CGoluahObject
{
public:
	CCharacterBase(CCharacterBase *parent = NULL);
	CCharacterBase(CDI_CHARACTERINFO2 *info,BYTE userID = 0xFF,BYTE userNo = 0xFF);
	virtual ~CCharacterBase();
	virtual DWORD Message(DWORD msg,LPVOID pd,DWORD prm);//必要があればオーバーライドします
	DWORD TouchA(ATTACKINFO *info,DWORD ta_eid);

private:
	void CCharacterBaseCreate();//それぞれのコンストラクタで共通の初期化処理

protected:

	virtual DWORD Action();//通常 必ずオーバーライドします
	virtual void PreAction();//通常 オーバーライドする必要はありません
	virtual void PostAction();//通常 オーバーライドする必要はありません
	virtual void Command();//通常 オーバーライドする必要はありません
	virtual DWORD CommandCOM(DWORD wid);//通常 オーバーライドする必要はありません

	void InitAll();
	void AddPowerGauge(double dp);

	virtual void InitAttackInfo();//攻撃力情報の初期化 通常必ずオーバーライドして使用します
	virtual void InitWazInfo();//技情報の設定 通常必ずオーバーライドして使用します
	virtual BOOL ChainCombo(DWORD chainid);//チェーンコンボ判定 必要があればオーバーライドします

	DWORD GetKeyInput()		{ return keyinput; }
	//サウンド関係。自前でやる場合はオーバーロードしてください
	virtual void InitMySound();
	virtual void ReleaseMySound();

public:
	virtual void PlayMySound(DWORD number);

protected:
	//ビットマップとCELLの読み込み。自前でやる場合はオーバーロードしてください
	virtual void InitGCDandBMP();
	virtual void ReleaseGCDandBMP();

	//CCharacterBaseクラスが行う行動の定義
	//通常動作
	virtual void act_neutral()=0;//ニュートラル
	virtual void act_crouch()=0;//しゃがみ
	virtual void act_rakka()=0;//落下
	virtual void act_tojyo()=0;//登場
	virtual void act_win()=0;//勝利
	virtual void act_walkf()=0;//前進
	virtual void act_walkb()=0;//後退
	virtual void act_jamps()=0;//ジャンプ予備動作
	virtual void act_jampc()=0;//垂直ジャンプ
	virtual void act_jampf()=0;//前ジャンプ
	virtual void act_jampb()=0;//後ジャンプ
	virtual void act_rakka2();//落下
	//通常.攻撃
	virtual void act_att_sa()=0;//立弱
	virtual void act_att_sb()=0;//立中
	virtual void act_att_sc()=0;//立強
	virtual void act_att_ca()=0;//屈弱
	virtual void act_att_cb()=0;//屈中
	virtual void act_att_cc()=0;//屈強
	virtual void act_att_ja()=0;//飛弱
	virtual void act_att_jb()=0;//飛中
	virtual void act_att_jc()=0;//飛強
	//その他
	virtual void act_taiki();
	virtual void act_taikicyu();
	virtual void act_koutai_out();
	virtual void act_koutai_in();
	virtual void act_koutai();
	virtual void act_koutai2();
	virtual void act_strikercomeon();

	//通常、オーバーライドする必要はありません
	virtual BOOL Furimuki();//敵のほうに振り向く動作をします
	virtual void ZFront();//z座標を手前側に移動します
	virtual void ZBack();//z座標を奥側に移動します
	virtual void JiyuuRakka(double acc_g,BOOL cyakuchi=FALSE,DWORD toaid=ACTID_NEUTRAL);
	virtual void CyakuchiHantei(DWORD toaid=ACTID_NEUTRAL);//着地の判定
	virtual void LoadAndSetKatiSerif();

	// コマンド判定。通常 必ずオーバーライドします
	virtual BOOL Command_Hissatu(DWORD keyinfo);//必殺技コマンド判定
	virtual BOOL Command_Normal(DWORD keyinfo);//基本動作
	virtual BOOL Command_OnSystem(DWORD keyinfo);//システム定義動作中
	virtual BOOL Command_OnHissatuAttacking(DWORD keyinfo);//必殺技中
	virtual BOOL Command_OnAttacking(DWORD keyinfo);//攻撃動作中
	virtual BOOL Command_OnNormal(DWORD keyinfo);//基本動作中

	// コンピュータコマンド判定。必要があればオーバーライドします
	virtual DWORD CmdCom_OnSystem(DWORD wid);//システム定義動作中
	virtual DWORD CmdCom_OnHissatuAttacking(DWORD wid);//必殺技中
	virtual DWORD CmdCom_OnKucyu(DWORD wid);//空中動作中
	virtual DWORD CmdCom_OnNormal(DWORD wid);//基本動作中

	//レバーコマンド判定関数
	BOOL com236(int dt);
	BOOL com623(int dt);
	BOOL com214(int dt);
	BOOL com421(int dt);
	BOOL com66(int dt);
	BOOL com44(int dt);
	BOOL com22(int dt);
	BOOL com426(int dt);
	BOOL com236236(int dt);
	BOOL com2363214(int dt);
	BOOL com62426(int dt);
	BOOL com6426(int dt);

	BOOL NageHantei(DWORD maai);//投げの間合い判定

	DWORD GetKey(DWORD interval)	{ return CGoluahObject::GetKey(keyinput, interval); }
	int SeekKey(int offset, int delay, DWORD keystate);

protected:
	BOOL chainComboEnabled;//チェーンコンボを有効にするかどうか
	DWORD chainlist;//チェーンコンボ・すでに出した技を保存する変数
	float front_z,back_z;//ZFront/Backメソッドで指定されるZ座標
	BOOL doStriker;//支援攻撃をやらかすかどうか
	WAZAINFO waz;//技情報構造体
	DWORD pal_number;//システムから指定されたパレット番号
	CCharacterBase *parent_char;//「親」キャラクター
	BOOL isSuperArmer;
	BOOL isAutoGuard;

private:
	MYSURFACE *bitmaps[GCDMAX_IMAGES];
	GCD_RECT *rects;
	GCD_CELL2 *cells;
	GCD_HANTEI *hantei;
	LPVOID mysounds[CBASE_NUMMYSOUND];
	DWORD keyinput;
};

inline int CCharacterBase::SeekKey(int offset, int delay, DWORD keystate) {
	return CGoluahObject::SeekKey(keyinput, offset, delay, keystate);
}

// 派生クラスの行動IDには、このフラグを使用しないこと
#define ACTID_CHARACTERBASE (0x8000)

#define ACTID_WALKF		(ACTID_CHARACTERBASE | 0x0005)//前歩き
#define ACTID_WALKB		(ACTID_CHARACTERBASE | 0x0006)//後ろ歩き
#define ACTID_JAMPS		(ACTID_CHARACTERBASE | 0x0007)//ジャンプ予備動作
#define ACTID_JAMPC		(ACTID_CHARACTERBASE | ACTID_KUCYU | 0x0002)
#define ACTID_JAMPF		(ACTID_CHARACTERBASE | ACTID_KUCYU | 0x0003)
#define ACTID_JAMPB		(ACTID_CHARACTERBASE | ACTID_KUCYU | 0x0004)
#define ACTID_RAKKA2	(ACTID_CHARACTERBASE | ACTID_KUCYU | 0x0005)

#define ACTID_STRIKERCOMEON	(ACTID_CHARACTERBASE | 0x000B)//いけー

#define ACTID_ATT_SA	(ACTID_CHARACTERBASE | ACTID_ATTACK | 0x0001)
#define ACTID_ATT_SB	(ACTID_CHARACTERBASE | ACTID_ATTACK | 0x0002)
#define ACTID_ATT_SC	(ACTID_CHARACTERBASE | ACTID_ATTACK | 0x0003)

#define ACTID_ATT_CA	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_SYAGAMI | 0x0001)
#define ACTID_ATT_CB	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_SYAGAMI | 0x0002)
#define ACTID_ATT_CC	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_SYAGAMI | 0x0003)

#define ACTID_ATT_JA	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_KUCYU | 0x0001)
#define ACTID_ATT_JB	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_KUCYU | 0x0002)
#define ACTID_ATT_JC	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_KUCYU | 0x0003)

#define ACTID_KOUTAIOUT	(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0001)//交代。画面外に飛んでく。
#define ACTID_KOUTAIIN	(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0002)//交代。画面内に飛んでくる
#define ACTID_TAIKI		(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0003)//画面外に飛んでって待機
#define ACTID_KOUTAI	(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0004)//画面外から飛んでくる
#define ACTID_TAIKICYU	(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0009)//待機ちゅ—
#define ACTID_KOUTAI2	(ACTID_CHARACTERBASE | ACTID_INOUT | 0x000A)//交代後、ポーズキメ

//chain combo
#define CHAIN_SA	0x00010000
#define CHAIN_SB	0x00020000
#define CHAIN_SC	0x00040000
#define CHAIN_CA	0x00080000
#define CHAIN_CB	0x00100000
#define CHAIN_CC	0x00200000

#define ADDEFFECT(a,b,c,d)	funcs->addeffect(a,b,c,d)


/***************************************************************
　飛び道具基本クラス
　なんかいまいち使い勝手悪そう

　極力試合途中で生成しないでください（する場合はネット非対応となります）

  ネット：コンストラクタ(3)で構築します。
****************************************************************/

//行動IDの定義
#define CBB_STATE_FLAG		0x00008000
#define CBB_STATE_IDLE		(CBB_STATE_FLAG | 0x00000100)//act_idle
#define CBB_STATE_RUN		(CBB_STATE_FLAG | 0x00000200)//act_run
#define CBB_STATE_BOMB		(CBB_STATE_FLAG | 0x00000300)//act_bomb
#define CBB_STATE_RUN2		(CBB_STATE_FLAG | 0x00000400)//act_run2

class RUNTIME_EXPORT CBulletBase : public CGoluahObject
{
public:
	CBulletBase(GOBJECT *parent = NULL);		//コンストラクタ(1)
	CBulletBase(CCharacterBase *parent);		//コンストラクタ(2) 推奨
	CBulletBase(CCharacterBase *parent,CDI_CHARACTERINFO2 *info,
		BYTE userID = 0xFF,BYTE userNo = 0xFF);	//コンストラクタ(3) ネットワーク用

	virtual ~CBulletBase(){}

	virtual BOOL Go(BOOL muki,double x,double y,double vx,double vy=0);
	virtual void Bomb();//消滅動作に強制移行します
	virtual void Hide();//強制的に待機状態にします
	void SetPos(double x,double y){GetGObject()->x=x;GetGObject()->y=y;}
	virtual DWORD Draw();

	void SetHitMessage(DWORD msgid,DWORD param){hitmsg=msgid;hitprm=param;}
	void SetGuardMessage(DWORD msgid,DWORD param){grdmsg=msgid;grdprm=param;}
	BOOL isReady(){return(GetGObject()->aid==CBB_STATE_IDLE);}

	GOBJECT* GetParentObj()				{ return parent_obj; }
	CCharacterBase* GetParentClass()	{ return parent_class; }
	BOOL IsForNetwork()					{ return m_isForNetwork; }

private:
	void CBulletBaseCreate();//共通初期化処理

protected:
	//必要があればオーバーライドします
	virtual DWORD Message(DWORD msg,LPVOID pd,DWORD prm);
	virtual DWORD Action();
	virtual DWORD TouchB(ATTACKINFO *info,BOOL hit);
	virtual DWORD Sousai(DWORD prm);//他飛道具と相殺したときに呼ばれます
	virtual void Hit(){}//相手にヒットしたとき呼ばれます(TouchB)

	//行動関数です。オーバーライドして使います
	virtual void act_idle();
	virtual void act_run()=0;
	virtual void act_bomb()=0;
	virtual void act_run2(){}

	//オーバーライドの必要は多分ない
	void Suicide();

	//サウンド再生。親のサウンド機能を流用します
	//コンストラクタ(2)で作成した場合のみ使用可能です
	virtual void PlayMySound(DWORD number);

protected:
	ATTACKINFO atkinfo;//攻撃力情報構造体

	DWORD hitmsg,hitprm;
	DWORD grdmsg,grdprm;

private:
	BOOL m_isForNetwork;
	GOBJECT *parent_obj;
	CCharacterBase *parent_class;
};


/***************************************************************
　自動飛び道具クラス
　昔々のソースを再利用するために作ってみた

　ネット対応：ネットのみ
****************************************************************/

struct MYATTACKINFO//旧攻撃力情報構造体
{
	DWORD hit;		//ヒット情報。下のフラグ(HITINFO_〜)の論理和を指定
	DWORD guard;	//ガード情報。下のフラグ(GUARDINFO_〜)の論理和を指艇
	DWORD damage;	//ダメージ
	DWORD kezuri;	//削り

	//以下使われていません。ダミーです
	DWORD objid;	//攻撃を行ったオブジェクトのID
	DWORD id;		//キャラクタ側で勝手に定義して
	double pg_hit;	//攻撃ヒット時のゲージ増加量
	double pg_guard;//攻撃ガードされたときのゲージ増加量
	BOOL muki;		//相手がのけぞる向き
	DWORD dur;
};

struct BULLETINFO_A//旧飛道具構造体
{
	MYATTACKINFO atk;
	int dur;//持続時間
	DWORD type;//下にあるフラグを指定
	int *cell_run;
	int *cell_dis;
	DWORD spd_run;
	DWORD spd_dis;
	double vx, vy;
	double ax, ay;

	//以下、使われてません。ダミーです。
	BOOL muki;
	double x;
	double y;
	DWORD state;
	DWORD cid;
	LPVOID info_b;
};

class RUNTIME_EXPORT CClassicalBullet : public CBulletBase
{
public:
	CClassicalBullet( CCharacterBase *parent,CDI_CHARACTERINFO2 *info,BULLETINFO_A *bulinfo,
							BYTE userID = 0xFF,BYTE userNo = 0xFF);
	virtual ~CClassicalBullet();
	
	BOOL Go(BOOL muki,double x,double y);
	virtual void Hit();

protected:
	virtual void act_run();
	virtual void act_run2();
	virtual void act_bomb();

private:
	DWORD* pState;
	void act_run_base();

	double vx,vy,ax,ay;
	int *pcr;
	int *pcd;
	DWORD array_len_r;
	DWORD array_len_d;
	DWORD spd_r;
	DWORD spd_d;
	DWORD lifedur;
	DWORD flags;
};

//ちゃんとサポートされてるかどうか不明
#define BULLETA_VSHUMAN		0x00000001//敵キャラクターに当たる
#define BULLETA_VSBULLET	0x00000002//敵飛び道具と相殺する
#define BULLETA_DONOTDIE	0x00000004//衝突後も消滅しないで飛んでいく
#define BULLETA_DRAWBACK	0x00000008//人物よりも後ろに描画する
#define BULLETA_DRAWMIDDLE	0x00000040//キャラクター・ヒットマーク間に描画
#define BULLETA_XJIMENN		0x00010000//地面に落っこちると消滅する
//#define BULLETA_XGAMENGAI	0x00020000//画面外に達すると消滅
//#define BULLETA_JIMAESYORI	0x10000000//info_bメンバが有効であり、処理を自前で行う
#define BULLETA_DISPZAHYO	0x00000010//座標指定はディスプレイ上の座標。ステージ上の表示位置に左右されない
#define BULLETA_DONOTSTOP	0x00000020//超必等でstopエフェクトがかかっていても描画が止まらない

/********************************************************
  飛び道具リストクラス
  ネット対応：ネットのみ
　********************************************************/

class RUNTIME_EXPORT CBulletList  
{
public:
	CBulletList();
	CBulletList(CBulletBase* pBullet);
	virtual ~CBulletList();

	virtual void Add(CBulletBase* pBullet);
	void SetPos(double x, double y)			{ this->x = x; this->y = y; }
	void SetSpeed(double vx, double vy=0)	{ this->vx = vx; this->vy = vy; }
	virtual BOOL Go(BOOL muki);
	virtual BOOL Go(BOOL muki, double x, double y);
	virtual BOOL Go(BOOL muki, double x, double y, double vx, double vy=0);
	virtual void Go_All(BOOL muki, double x, double y);
	virtual void Go_All(BOOL muki);
	CBulletList* GetNext()		{ return pNext; }
	int GetCount()	{ return ListCount; }

protected:
	CBulletBase* GetBullet()	{ return m_pBullet; }

private:
	double x;
	double y;
	double vx;
	double vy;
	int ListCount;
	CBulletBase* m_pBullet;
	CBulletList* pNext;
};

