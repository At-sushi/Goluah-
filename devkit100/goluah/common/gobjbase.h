
/*!=======================================================================================

    @file
    @brief Goluah 基本クラス群

    CCharacterInfo	　キャラクター情報設定クラス
    CGoluahObject	　Goluahオブジェクト基本クラス
    CCharacterBase	　キャラクターオブジェクト基本クラス
    CBulletBase		　飛び道具基本クラス
    CClassicalBullet　旧飛道具構造体互換飛び道具クラス
    CBulletList		　飛道具リストクラス

=========================================================================================*/
#pragma once

/*! 
*	@defgroup GObjectBase
*	@brief Goluah キャラクタ/ステージDLL向けの基礎クラス群
*/

#include "gobject.h"//GOLUAH DLLインターフェイス定義

//ビルド設定

#ifdef COMMONDLL_C100_EXPORTS
    //共通DLLをビルドする場合
#ifndef USE_DIRECT3D_DIRECT
#error
#endif
#define RUNTIME_EXPORT __declspec(dllexport)
#else
    //共通DLLを利用するキャラクタDLLを作成する場合
#ifdef USE_GOLUAH_COMMON_DLL
#pragma comment(lib, "goluah_c100.lib")
#define RUNTIME_BUILD_OFF
#else
    //共通DLLを利用しないキャラクタDLLを作成する場合
#endif
#define RUNTIME_EXPORT
#endif // COMMONDLL_C100_EXPORTS



#if CDI_VERSION < 900
struct CHARACTER_LOAD_OPTION{};
#endif

#if GCD_VERSION < 900
struct GCD_CELL2_070 : public GCD_CELL2 {};
#endif
/*-----------------------------------------------------------------*/


#ifdef _DEBUG
#define dprint	DebugPrintf
#else
#define dprint	DebugPrintfDummy
#endif
extern void RUNTIME_EXPORT DebugPrintf(char* fmt,...);
extern void RUNTIME_EXPORT DebugPrintfDummy(char* fmt,...);

//! サウンド保持用の構造体
/*typedef struct tagMYSOUND
{
    DWORD number; //!< サウンド番号
    LPVOID sound; //!< サウンドへのポインタ
} MYSOUND, *LPMYSOUND;*/

// なぜかこっちにもバージョン振ってます。
#define DTB_VERSION 1100
#define STB_VERSION 1100

/********************************************************
  キャラクター情報設定クラス
  ネット対応：
    コンストラクタ６番目の引数をTRUEに設定してください。
　********************************************************/
class RUNTIME_EXPORT CCharacterInfo  
{
public:
    CCharacterInfo(LPCTSTR CharName = "名無しさん", DWORD CdiVersion = CDI_VERSION, CHARACTER_LOAD_OPTION* Options = NULL,
        int num_Options = 0, DWORD max_option_point = 0, BOOL isNetworkable = FALSE, DWORD DtbVersion = DTB_VERSION);
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
    BOOL m_Error;
};

/*! @brief キャラクター設定マクロ
*	@param cinfo 必要な情報を格納したCCharacterInfoクラスのインスタンス
*	@param Type メインキャラクターとして生成するクラスの型
*	@ingroup GObjectBase
*
*	DLLは、このマクロが生成する関数を外部にエクスポートしなければならない。
*	適切なモジュール定義ファイルを使用すること。
*/
#define SET_CHARACTERINFO(cinfo, Type) \
extern "C" BOOL CharacterInfo(LPVOID info) {\
    return cinfo.DllCharacterInfo(info);\
}\
extern "C" DWORD CreateCharacter(PVOID info) {\
    cinfo.DllCreateCharacter(info);\
    Type* pc = new Type((CDI_CHARACTERINFO2*)info);\
    pc->InitializeCharacter();\
    return(pc->GetObjectID());\
}

/*--------------------------------------------------------------
    Goluahオブジェクト基本クラス
----------------------------------------------------------------*/
class RUNTIME_EXPORT CGoluahObject
{
    friend class CBulletBase;

public:
    CGoluahObject(BOOL is_effect = FALSE);
    virtual ~CGoluahObject();

    static DWORD gMessageToObject(DWORD msg,LPVOID pd,DWORD prm);
    virtual DWORD Message(DWORD msg,LPVOID pd,DWORD prm);

    DWORD GetObjectID();
    GOBJECT* GetGObject();
    char* GetCharDir() { return g_chardir; }
    char* GetDLLPath() { return g_chardir; }//ステージの場合なんだか紛らわしいので
    operator GOBJECT*() { return pdat; }

    static void* operator new(size_t size);
    static void operator delete(void* p, size_t size);

protected:
    virtual DWORD Action();
    virtual void Command();
    virtual DWORD CommandCOM(DWORD wid);
    virtual DWORD TouchA(ATTACKINFO *info,DWORD ta_eid);
    virtual DWORD TouchB(ATTACKINFO *info,BOOL hit);
    virtual DWORD TouchC(ATTACKINFO *info,DWORD tc_eid);
    virtual DWORD Draw();
    virtual DWORD DrawBack();
    virtual DWORD DrawFront();
    virtual void ActionIDChanged();
    virtual BOOL OnChangeRound();
    virtual DWORD ComThink();

    double zurex(double x);
    int zurex(int x);
    void movex(double dx);
    void movex(int dx);

    void AddProperty(DWORD prop);
    void RemoveProperty(DWORD prop);
    void SetProperty(DWORD prop);

    virtual void ChangeAction(DWORD actid);

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
    static char* GetCharacterName(DWORD oid);				//オブジェクトから名前所得
    static double GetDisplayCenter_X();
    static double GetDisplayCenter_Y();
    static DWORD GetTeamNum();
    static DWORD GetTaisenKeisiki();
    static DWORD GetTeamNum2(DWORD tid);
    static BYTE  GetEnemyFace(DWORD tid);
    static DWORD TID2EnemyTID(DWORD tid);
           DWORD EnemyTID();
    static BOOL  IsNetwork();
    static void PushTag(const char* tag);
    static void PopTag();
    static void LogError(const char* fmt,...);
    static void LogWarning(const char* fmt,...);
    static void LogDebug(const char* fmt,...);
    static void LogInfo(const char* fmt,...);
    static void BGMPause();
    static void BGMResume();

    // オブジェクト関連の関数
    BOOL   ObjCatch(DWORD eid,DWORD msg_nage);				//投げとかで相手をつかむ
    static GOBJECT* GetInfo(DWORD oid);						//情報ゲッツ
    void   AddDamage(DWORD eid,int x,int y);				//(主に投げのとき)強制的にダメージを与える
    void   ActIDChanged();									//行動IDが変化したときの処理を明示的に行う
    static GOBJECT*  GetActiveCharacter(DWORD tid);			//現在アクティブなキャラクターの情報
           GOBJECT*  GetActiveEnemy();						//現在アクティブな敵キャラクターの情報
    void   Suicide();										//オブジェクト消滅
    static DWORD  GetMaai_H(DWORD oid, DWORD eoid);			//間合いをget(水平)
    static DWORD  GetMaai_V(DWORD oid, DWORD eoid);			//間合いをget(垂直)
           DWORD  GetMaaiFromMe_H(DWORD eoid)				//!自分からの間合いをget(水平)
                { return GetMaai_H(GetObjectID(), eoid); }
           DWORD  GetMaaiFromMe_V(DWORD eoid)				//!自分からの間合いをget(垂直)
                { return GetMaai_V(GetObjectID(), eoid); }
    static DWORD  GetCharacterID(DWORD tid,DWORD index);	//キャラクターのオブジェクトID
           DWORD  GetComLevel();							//COMレベル取得
           void   SetComReach(DWORD idx,int length);		//COMリーチ設定
           void   SetComAct(DWORD aid,int delay);			//COM行動設定
           void   ComDelay(int delay);						//COM行動抑制
           BOOL	  IsLocal();
           BOOL   IsRemote(){return !IsLocal();}
    static void dact_damages1(GOBJECT *p);
    static void dact_damages2(GOBJECT *p);
    static void dact_damages3(GOBJECT *p);
    static void dact_damagec1(GOBJECT *p);
    static void dact_damagec2(GOBJECT *p);
    static void dact_damagec3(GOBJECT *p);
    static void dact_damagej1(GOBJECT *p);
    static void dact_damagej2(GOBJECT *p);
    static void dact_damagej3(GOBJECT *p);
    static void dact_damages1a(GOBJECT *p);
    static void dact_damages2a(GOBJECT *p);
    static void dact_damages3a(GOBJECT *p);
    static void dact_damagec1a(GOBJECT *p);
    static void dact_damagec2a(GOBJECT *p);
    static void dact_damagec3a(GOBJECT *p);
    static void dact_guards1(GOBJECT *p);
    static void dact_guards2(GOBJECT *p);
    static void dact_guards3(GOBJECT *p);
    static void dact_guardc1(GOBJECT *p);
    static void dact_guardc2(GOBJECT *p);
    static void dact_guardc3(GOBJECT *p);
    static void dact_guardj1(GOBJECT *p);
    static void dact_guardj2(GOBJECT *p);
    static void dact_guardj3(GOBJECT *p);
    static void dact_down(GOBJECT *p);
    static void dact_down2(GOBJECT *p);
    static void dact_okiagari(GOBJECT *p);
    static void dact_futtobi(GOBJECT *p);
    static void dact_futtobi2(GOBJECT *p);
    static void dact_tatakituke1a(GOBJECT *p);
    static void dact_tatakituke1b(GOBJECT *p);
    static void dact_tatakituke2a(GOBJECT *p);
    static void dact_tatakituke2b(GOBJECT *p);

           void   SetComRange(DWORD idx);					//COMレンジ設定
    static DWORD  GetKeyInput(DWORD oid);					//オブジェクトからキーID所得

    //　描画関連の関数の定義
    static LPDIRECT3D8 GetD3D();							//IDirect3D* の取得
    static LPDIRECT3DDEVICE8 GetD3DDevice();				//IDirect3DDevice* の取得
    static void   LoadCellData(char* pathname,GCD_CELL2_070* pCells,GCD_RECT* pRects,GCD_HANTEI* pHanteis);//セルデータ読み込み関数。ver0.90以降では非推奨
    static MYSURFACE* LoadBitmap(char* PathName,char* PalletFileName = NULL);			//ビットマップ読み込み関数
    static void   UnloadBitMap(MYSURFACE* Bitmap);			//ビットマップ後始末関数
    static void   CellDraw(MYSURFACE** pBmps,GCD_CELL2* cdat,GCD_RECT* rdat,		//セル描画関数
        DWORD cnum,int x,int y,float z,int Rotate,BOOL ReverseX,BOOL ReverseY,DWORD Color,float magx,float magy);
    static void	  CellDraw(GOBJECT *objdat);
    static void   CkBlt(MYSURFACE* pBmp,int x1,int y1,RECT bltrect,
        double magx,double magy,BOOL revx,BOOL revy,float z,DWORD color);			//Blt2
    static void   Blt3D(MYSURFACE* pBmp,RECT bltrect,MYRECT3D rect,DWORD color);	//Blt3
    static void   SetTransform(BOOL BonVoyage);				//変換行列設定
    static void   SetBlend(DWORD type);						//ブレンドの仕方を変更
    static void   SetParentMatrix(LPD3DXMATRIX pMatrix,BOOL erase,LPD3DXMATRIX OldMatrix);	//「親」の変換行列設定
    static DWORD  CreateCellData(char* filename,GCD_CELL2 **cdat,GCD_RECT **rdat,GCD_HANTEI **hdat);//セルデータ読み込み。ver0.90以降ではLoadCellDatからこちらに変更するべき
    static void   DestroyCellData(GCD_CELL2 **cdat,GCD_RECT **rdat,GCD_HANTEI **hdat);		//CreateCellDatで生成されたバッファをクリア
    static MYSURFACE* LoadImage(char* PathName, char* PalletFileName);	// 画像読み込み関数、PNG対応に伴い追加。
    static DWORD  CreateCellData2(char* filename,GCD_CELL2 **cdat,GCD_RECT **rdat,GCD_HANTEI **hdat);// セルデータ読み込み（２）、圧縮形式対応に伴い追加。

protected:
    float base_z;
    char *g_chardir;
    GOBJECT *pdat;

private:
    DWORD oid;
    static CGoluahObject* pObjToDelete ;
    static CGoluahObject* pObjDeleting ;

    double velocity;
};

inline DWORD CGoluahObject::GetObjectID(){return(oid);}
inline GOBJECT* CGoluahObject::GetGObject(){return(pdat);}

/*!
*	@brief オブジェクト作成マクロ
*	@param pointer 作成したオブジェクトのアドレスを格納するポインタ
*	@param obj 作成するオブジェクトのクラス名
*	@ingroup GObjectBase
*
*	オブジェクト生成時の推奨マクロです。
*	オブジェクト作成の他、エラー処理などを自動で行ってくれます。
*
*	引数を持つコンストラクタを呼び出す場合、クラス名と同時に設定してください。
*/
#define CREATE_GOBJ(pointer, obj)	{ pointer = new obj; if ( pointer && !pointer->GetGObject() )\
                                        { delete pointer; pointer = NULL; }\
                                    }

/*!
*	@brief エフェクト用オブジェクト作成マクロ
*	@param obj 作成するオブジェクトのクラス名
*	@ingroup GObjectBase
*
*	エフェクトオブジェクト生成時の推奨マクロです。
*	オブジェクト作成の他、エラー処理などを自動で行ってくれます。
*	自動で削除される可能性のあるクラスなので、ポインタ変数への格納は出来ません。
*
*	引数を持つコンストラクタを呼び出す場合、クラス名と同時に設定してください。
*/
#define CREATE_GOBJ_FX(obj)			{ CGoluahObject* pointer = new obj; if ( pointer && !pointer->GetGObject() )\
                                        { delete pointer; }\
                                    }


/**********************************************************************

　キャラクターオブジェクト基本クラス

***********************************************************************/

class RUNTIME_EXPORT CCharacterBase : public CGoluahObject
{
public:
    CCharacterBase(CDI_CHARACTERINFO2* info);
    CCharacterBase(CCharacterBase *parent = NULL);
    virtual void InitializeCharacter();

    virtual ~CCharacterBase();
    
    virtual DWORD Message(DWORD msg,LPVOID pd,DWORD prm);//必要があればオーバーライドします
    DWORD TouchA(ATTACKINFO *info,DWORD ta_eid);

private:
    void CCharacterBaseCreate();//それぞれのCreateで共通の初期化処理

protected:

    virtual DWORD Action();				//通常 必ずオーバーライドします
    virtual void PreAction();			//通常 オーバーライドする必要はありません
    virtual void PostAction();			//通常 オーバーライドする必要はありません
    virtual void Command();				//通常 オーバーライドする必要はありません
    virtual DWORD CommandCOM(DWORD wid);//通常 オーバーライドする必要はありません

    //■初期化処理。必要な部分をオーバーライドして使用します
    virtual void InitParameters()=0;	//オプション・HP等設定処理。通常オーバーライドして使用します
    virtual void InitAttackInfo();		//攻撃力情報の初期化 通常必ずオーバーライドして使用します
    virtual void InitWazInfo();			//技情報の設定 通常必ずオーバーライドして使用します
    virtual void InitBullets(){}		//飛道具初期化処理。必要があればオーバーライドして使用します

    virtual BOOL ChainCombo(DWORD chainid);//チェーンコンボ判定 必要があればオーバーライドします
    virtual BOOL ChainCheck(DWORD chainid);//上記のチェックのみ版

    DWORD GetKeyInput()		{ return keyinput; }

    //■サウンド関係
    // 自前でやる場合はオーバーライドしてください
    // コンストラクタの引数で渡したnumのぶんだけ sound**.wav を使用します
protected:
    virtual void InitMySound(char* path_name=NULL,char* list_name=NULL);
    virtual void ReleaseMySound();
public:
    virtual void PlayMySound(DWORD number);

    //■ビットマップとCELLの読み込み
    //　自前でやる場合はオーバーライドしてください
protected:
    virtual void InitGCDandBMP();
    virtual void ReleaseGCDandBMP();

    //■CCharacterBaseクラスが行う行動の定義
    //通常動作
    virtual void act_neutral()=0;//ニュートラル
    virtual void act_crouch()=0;//しゃがみ
    virtual void act_rakka()=0;//落下
    virtual void act_tojyo()=0;//登場
    virtual void act_win()=0;//勝利
    virtual void act_timeoverlose();//タイムオーバー
    virtual void act_walkf()=0;//前進
    virtual void act_walkb()=0;//後退
    virtual void act_jamps()=0;//ジャンプ予備動作
    virtual void act_jampc()=0;//垂直ジャンプ
    virtual void act_jampf()=0;//前ジャンプ
    virtual void act_jampb()=0;//後ジャンプ
    virtual void act_rakka2();//落下
    //virtual void act_win2_in();//仲間が勝利　画面外から飛んでくる
    //virtual void act_win2_pose();//仲間が勝利　ポーズ
    virtual void act_guards();//落下
    virtual void act_guardc();//落下
    virtual void act_guardj();//落下
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

    // コマンド判定。通常オーバーライドします
    virtual BOOL Command_Hissatu(DWORD keyinfo);			//必殺技コマンド判定
    virtual BOOL Command_Normal(DWORD keyinfo);				//基本動作
    virtual BOOL Command_OnSystem(DWORD keyinfo);			//システム定義動作中
    virtual BOOL Command_OnHissatuAttacking(DWORD keyinfo);	//必殺技中
    virtual BOOL Command_OnAttacking(DWORD keyinfo);		//攻撃動作中
    virtual BOOL Command_OnNormal(DWORD keyinfo);			//基本動作中

    // コンピュータコマンド判定。必要があればオーバーライドします
    virtual DWORD CmdCom_OnSystem(DWORD wid);				//システム定義動作中
    virtual DWORD CmdCom_OnHissatuAttacking(DWORD wid);		//必殺技中
    virtual DWORD CmdCom_OnKucyu(DWORD wid);				//空中動作中
    virtual DWORD CmdCom_OnNormal(DWORD wid);				//基本動作中

    //レバーコマンド判定関数
    BOOL com236(int dt);
    BOOL com623(int dt);
    BOOL com214(int dt);
    BOOL com421(int dt);
    BOOL com66(int dt);
    BOOL com44(int dt);
    BOOL com22(int dt);
    BOOL com66i(int dt);
    BOOL com44i(int dt);
    BOOL com22i(int dt);
    BOOL com426(int dt);
    BOOL com236236(int dt);
    BOOL com2363214(int dt);
    BOOL com62426(int dt);
    BOOL com6426(int dt);

    //COMレベル判定
    BOOL ComLevelCk(UINT level);

    //その他操作
    //通常、オーバーライドする必要はありません
    virtual BOOL Furimuki();		//敵のほうに振り向く動作をします
    virtual void ZFront();			//z座標を手前側に移動します
    virtual void ZBack();			//z座標を奥側に移動します
    virtual void JiyuuRakka(double acc_g,BOOL cyakuchi=FALSE,DWORD toaid=ACTID_NEUTRAL);
    virtual void CyakuchiHantei(DWORD toaid=ACTID_NEUTRAL);//着地の判定
    virtual void LoadAndSetKatiSerif(char* filename=NULL);
    BOOL NageHantei(DWORD maai);	//投げの間合い判定
    void AddPowerGauge(double dp);	//パワーゲージ増加（上限/下限チェックつき）
    DWORD GetKey(DWORD interval)	{ return CGoluahObject::GetKey(keyinput, interval); }
    int SeekKey(int offset, int delay, DWORD keystate);

    //情報取得系
    BOOL IsCom()		{return (pdat->objtype&GOBJFLG_COMPUTER) ? TRUE : FALSE;}
    BOOL IsLocalCom()	{ return (IsCom() && IsLocal()); }

protected:
    //各種動作設定
    BOOL chainComboEnabled;		//チェーンコンボを有効にするかどうか
    DWORD chainlist;			//チェーンコンボ・すでに出した技を保存する変数
    float front_z,back_z;		//ZFront/Backメソッドで指定されるZ座標
    BOOL doStriker;				//支援攻撃をやらかすかどうか
    WAZAINFO waz;				//技情報構造体
    DWORD pal_number;			//システムから指定されたパレット番号
    CCharacterBase *parent_char;//「親」キャラクター
    BOOL isSuperArmer;
    BOOL isAutoGuard;
    DWORD option_flags;

    int base_voice_damage1;
    int base_voice_damage2;
    int base_voice_damage3;
    int base_voice_ko;
    BOOL final_voice_flag;

    MYSURFACE *bitmaps[GCDMAX_IMAGES];
    GCD_RECT *rects;
    GCD_CELL2 *cells;
    GCD_HANTEI *hantei;
    LPVOID* mysounds;
    int* mysound_numbers;
    /*LPMYSOUND mysounds;
    UINT mysound_numbers;*/
    DWORD keyinput;
};

inline int CCharacterBase::SeekKey(int offset, int delay, DWORD keystate) {
    return CGoluahObject::SeekKey(keyinput, offset, delay, keystate);
}

// 派生クラスの行動IDは、このフラグが競合しないように定義すること（普通しないけど）
#define ACTID_CHARACTERBASE (0x8000)

#define ACTID_WALKF		(ACTID_CHARACTERBASE | 0x0001)//前歩き
#define ACTID_WALKB		(ACTID_CHARACTERBASE | 0x0002)//後ろ歩き
#define ACTID_JAMPS		(ACTID_CHARACTERBASE | 0x0003)//ジャンプ予備動作
#define ACTID_JAMPC		(ACTID_CHARACTERBASE | ACTID_KUCYU | 0x0004)
#define ACTID_JAMPF		(ACTID_CHARACTERBASE | ACTID_KUCYU | 0x0005)
#define ACTID_JAMPB		(ACTID_CHARACTERBASE | ACTID_KUCYU | 0x0006)
#define ACTID_RAKKA2	(ACTID_CHARACTERBASE | ACTID_KUCYU | 0x0007)

#define ACTID_STRIKERCOMEON	(ACTID_CHARACTERBASE | 0x0008)//いけー

#define ACTID_ATT_SA	(ACTID_CHARACTERBASE | ACTID_ATTACK | 0x0009)
#define ACTID_ATT_SB	(ACTID_CHARACTERBASE | ACTID_ATTACK | 0x000A)
#define ACTID_ATT_SC	(ACTID_CHARACTERBASE | ACTID_ATTACK | 0x000B)
#define ACTID_TUKAMI	(ACTID_CHARACTERBASE | ACTID_ATTACK | 0x000C)

#define ACTID_ATT_CA	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_SYAGAMI | 0x000C)
#define ACTID_ATT_CB	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_SYAGAMI | 0x000D)
#define ACTID_ATT_CC	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_SYAGAMI | 0x000E)

#define ACTID_ATT_JA	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_KUCYU | 0x000F)
#define ACTID_ATT_JB	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_KUCYU | 0x0010)
#define ACTID_ATT_JC	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_KUCYU | 0x0011)

#define ACTID_KOUTAIOUT	(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0012)//交代。画面外に飛んでく。
#define ACTID_KOUTAIIN	(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0013)//交代。画面内に飛んでくる
#define ACTID_TAIKI		(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0014)//画面外に飛んでって待機
#define ACTID_KOUTAI	(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0015)//画面外から飛んでくる
#define ACTID_TAIKICYU	(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0016)//待機ちゅ―
#define ACTID_KOUTAI2	(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0017)//交代後、ポーズキメ

#define ACTID_TIMEOVERLOSE	(ACTID_CHARACTERBASE | 0x0018)

#define ACTID_SYORI2IN		(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0019)
#define ACTID_SYORI2POSE	(ACTID_CHARACTERBASE | ACTID_INOUT | 0x001A)

#define ACTID_GUARDS	(ACTID_CHARACTERBASE | ACTID_GUARD | 0x0001)
#define ACTID_GUARDC	(ACTID_CHARACTERBASE | ACTID_GUARD | ACTID_SYAGAMI | 0x0001)
#define ACTID_GUARDJ	(ACTID_CHARACTERBASE | ACTID_GUARD | ACTID_KUCYU | 0x0001)

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
　なんかいまいち使い勝手悪そうカモ

　極力試合途中で生成しないでください
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

    virtual ~CBulletBase(){}

    virtual BOOL Go(BOOL muki,double x,double y,double vx,double vy=0);
    virtual void Bomb();//消滅動作に強制移行します
    virtual void Hide();//強制的に待機状態にします
    void SetPos(double x,double y){GetGObject()->x=x;GetGObject()->y=y;}
    virtual DWORD Draw();

    void SetHitMessage(DWORD msgid,DWORD param){hitmsg=msgid;hitprm=param;}
    void SetGuardMessage(DWORD msgid,DWORD param){grdmsg=msgid;grdprm=param;}
    void SetAttackInfo( ATTACKINFO* patk){atkinfo=*patk;}
    BOOL isReady(){return(GetGObject()->aid==CBB_STATE_IDLE);}

    GOBJECT* GetParentObj()				{ return parent_obj; }
    CCharacterBase* GetParentClass()	{ return parent_class; }

private:
    void CBulletBaseCreate();//共通初期化処理

protected:
    //必要があればオーバーライドします
    virtual DWORD Message(DWORD msg,LPVOID pd,DWORD prm);
    virtual DWORD Action();
    virtual DWORD TouchB(ATTACKINFO *info,BOOL hit);
    virtual DWORD Sousai(DWORD prm);	//他飛道具と相殺したときに呼ばれます
    virtual void Hit();					//相手にヒットしたとき呼ばれます(TouchB)
    virtual BOOL OnChangeRound();		//ラウンド変化時処理。消えたくなければTRUEを返す

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
    DWORD hit;		//ヒット情報。下のフラグ(HITINFO_～)の論理和を指定
    DWORD guard;	//ガード情報。下のフラグ(GUARDINFO_～)の論理和を指艇
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
    //!コンストラクタ
    CClassicalBullet( CCharacterBase *parent,	//!< キャラクタクラス
                    BULLETINFO_A *bulinfo		//!< 飛び道具情報
                    );

    //!旧コンストラクタ
    CClassicalBullet( CCharacterBase *parent,	//!< キャラクタクラス
                    CDI_CHARACTERINFO2 *info,	//!< 未使用(多分) NULLでよい
                    BULLETINFO_A *bulinfo,		//!< 飛び道具情報
                    BYTE userID = 0xFF,			//!< 未使用
                    BYTE userNo = 0xFF			//!< 未使用
                    );
    virtual ~CClassicalBullet();
    
    virtual BOOL Go(BOOL muki,double x,double y);
    virtual void Hit();

protected:
    virtual void act_run();
    virtual void act_run2();
    virtual void act_bomb();

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

  これ、消すんでしたっけ？(aki) ＞ あつしさん
　********************************************************/

class RUNTIME_EXPORT CBulletList  
{
public:
    CBulletList();
    CBulletList(CBulletBase* pBullet);
    virtual ~CBulletList();

    virtual void Add(CBulletBase* pBullet);
    /*!
        @brief 飛び道具の発射座標設定
        @param x 発射X座標
        @param y 発射Y座標
        @sa GetNext

        GoまたはGo_Allにて座標を省略した場合に、
        これで設定した座標が使用されます。

        なお、これで設定できるのはリストの最初の要素だけです（笑）。
        後の方に使う場合は、GetNextを使ってください。
    */
    void SetPos(double x, double y)			{ this->x = x; this->y = y; }
    /*!
        @brief 飛び道具の発射速度設定
        @param vx X方向の速度
        @param vy Y方向の速度
        @sa GetNext

        GoまたはGo_Allにてvxとvyを省略した場合に、
        これで設定した速度が使用されます。
        Go_Allを使用する場合はほぼ必須の項目です。

        毎度のことですが、これで設定できるのはリストの最初の要素だけです（笑）。
        後の方に使う場合は、やはりGetNextを使ってください。
    */
    void SetSpeed(double vx, double vy=0)	{ this->vx = vx; this->vy = vy; }
    virtual BOOL Go(BOOL muki);
    virtual BOOL Go(BOOL muki, double x, double y);
    virtual BOOL Go(BOOL muki, double x, double y, double vx, double vy=0);
    virtual void Go_All(BOOL muki, double x, double y);
    virtual void Go_All(BOOL muki);
    /*!
        @brief リストの次の要素を所得
        @return 次の要素を示すCBulletListへのポインタ、無い場合はNULL
        @sa SetPos
        @sa SetSpeed

        呼び出し元の、次の要素を表すCBulletList構造体を所得します。
        次の要素がない場合はNULLを返します。
        要素ごと専用のクラスが無く、CBulletListをそのまま連結させているのは私の手抜きです（笑）。

        主に、リストを順に走査しながらSetPosやSetSpeedを適用するために使います。
        途中のクラスを勝手に解放したりすると（・Ａ・）ｲｸﾅｲ!!
    */
    CBulletList* GetNext()		{ return pNext; }
    /*!
        @brief リストの要素数を所得
        @return リストに含まれている要素の数

        リスト内の、全ての要素数を所得します。
    */
    int GetCount()	{ return ListCount; }
    /*!
        @brief 先頭の飛び道具を所得
        @return リストの先頭に含まれている飛び道具のポインタ
        @sa GetNext

        リストの先頭にある飛び道具を所得します。使うことはあんまりなさそう。
        GetNextを使えば、先頭以外のものも所得出来ます。
    */
    CBulletBase* GetBullet()	{ return m_pBullet; }
    //CBulletList* operator [](int num);

protected:
    double x;
    double y;
    double vx;
    double vy;
    int ListCount;
    CBulletBase* m_pBullet;
    CBulletList* pNext;
};


/********************************************************
    マクロ定義
**********************************************************/

#define ALLGUARDFLAG		(GUARDINFO_XSTAND | GUARDINFO_XCROUCH | GUARDINFO_XJAMP)
#define CANNOT_GUARD(a)		((a&ALLGUARDFLAG)==0)



/********************************************************
    エフェクトベース
**********************************************************/
/*!
*	@brief エフェクトベース
*	@ingroup GObjectBase
*
*	試合中に頻繁に生成・破棄を行っても良いオブジェクト。
*	このクラスにより生成されるオブジェクトは(オブジェクトフラグに関わらず、
*	システム的に)他オブジェクトとのあたり判定が行われない。
*	また、このオブジェクトが他オブジェクトに対して何らかの相互作用
*	（このクラスが生成されることを除く）を行うことはご法度です。
*/
class RUNTIME_EXPORT CEffectBase : public CGoluahObject
{
public:
    CEffectBase();
    virtual ~CEffectBase();

protected:

    /*!
    *	@brief GOBJMSG_ACTION応答処理
    *
    *	CEffectBaseは状態を無視し、カウンタとSetDurationで与えられた値からtimeを計算して
    *	Updateを呼び出します。
    *	カウンタがSetDurationで与えられた値を超えるとオブジェクト消滅の手続きを行い、
    *	クラスは自動的にdeleteされます。
    */
    virtual DWORD Action();

    //!ラウンド変化で消滅
    virtual BOOL OnChangeRound(){return FALSE;}

    /*!
    *	@brief 持続時間の設定
    *	@sa CEffectBase::Update
    *	
    *	通常はコンストラクタ等からこの関数を呼び出し、設定を行ってください。
    */
    void SetDuration(UINT dur);

    /*!
    *	@brief 更新処理
    *	@param time 0～1の値
    *
    *	継承クラスではこの関数をオーバーライドして処理を実装します。
    *	timeはSetDurationで与えた持続時間の間で0.0f～1.0fの値が渡されます。
    *	継承クラスでAction処理を書き換えた場合はこの関数の動作は不定です。
    *	"状態"を複数持ちたい場合はこの関数を使用せず、自前でAction周りの処理を書いてください。
    */
    virtual void Update(float time) = 0;

private:
    float ceffectbase_private_duration;
};


/********************************************************
    ステージベース
**********************************************************/

/*!
*	@brief ステージベースクラス
*	@ingroup GObjectBase
*
*	ステージのベース...といってもCGoluahObjectから追加されたサービスはほとんどない。
*	SET_STAGEINFOマクロを使用してエクスポートする関数を自動生成した場合は
*	試合の開始前にInitializeStageがコールされるので初期化処理はそこに記述すること。
*	描画はDrawBackをオーバーライドして行うこと。
*	何らかの更新処理を行う場合はActionをオーバーライドして行うこと。
*/
class RUNTIME_EXPORT CStageBase : public CGoluahObject
{
public:
    CStageBase(SDI_STAGEINFO2 *info);
    virtual ~CStageBase();

    /*!	@brief 初期化
    *
    *	継承クラスはこの関数をオーバーライドして初期化処理を記述すること。
    */
    virtual void InitializeStage() = 0;

    /*!
    *	@brief 関数ポインタの初期化
    *
    *	CreateStage関数で、クラスのインスタンス生成前に
    *	グローバルの関数ポインタを初期化する必要がある。
    *	この関数がその手続きを行う。
    */
    static void SetupStageDLL(SDI_STAGEINFO2 *info);

    /*!
    *	@brief ライブラリバージョンチェック
    *
    *	ベースクラスのバージョンチェックを行います。
    *	共通ライブラリを使用する場合、これを使うとバージョンのズレを回避できるかも。
    *	マクロが勝手に使うので、ユーザ側で使う必要はありません。
    */
    static BOOL CheckSTB(DWORD Version = STB_VERSION);

protected:
    /*!
    *	ステージは普通、GOBJMSG_DRAWに対応する通常描画は行わない。
    *	かわりにDrawBackをオーバーライドして描画を行うこと
    */
    virtual DWORD Draw(){return TRUE;}
};


/*!
*	@brief 設定用マクロ(ステージ)
*	@param Type ステージとして生成するCStageBase継承クラスの型
*	@ingroup GObjectBase
*
*	DLLは、このマクロが生成する関数を外部にエクスポートしなければならない。
*	適切なモジュール定義ファイルを使用すること。
*/
#define SET_STAGEINFO(Type) \
extern "C" BOOL StageInfo(LPVOID info) {\
    ((SDI_STAGEINFO*)info)->ver = SDI_VERSION;\
    return CStage::CheckSTB();\
}\
extern "C" DWORD CreateStage(PVOID info) {\
    CStage::SetupStageDLL((SDI_STAGEINFO2*)info);\
    Type* pstgobj = new Type((SDI_STAGEINFO2*)info);\
    pstgobj->InitializeStage();\
    return(pstgobj->GetObjectID());\
}

/*!
*	@brief 設定用マクロ(ステージ、バージョンチェック機能付き)
*	@param Type ステージとして生成するCStageBase継承クラスの型
*	@param Ver ライブラリのバージョン
*	@ingroup GObjectBase
*
*	DLLは、このマクロが生成する関数を外部にエクスポートしなければならない。
*	適切なモジュール定義ファイルを使用すること。
*/
#define SET_STAGEINFO_VERCHK(Type, Ver) \
extern "C" BOOL StageInfo(LPVOID info) {\
    ((SDI_STAGEINFO*)info)->ver = SDI_VERSION;\
    return CStage::CheckSTB(Ver);\
}\
extern "C" DWORD CreateStage(PVOID info) {\
    CStage::SetupStageDLL((SDI_STAGEINFO2*)info);\
    Type* pstgobj = new Type((SDI_STAGEINFO2*)info);\
    pstgobj->InitializeStage();\
    return(pstgobj->GetObjectID());\
}

class RUNTIME_EXPORT CShadowEffect : public CEffectBase
{
public:
    CShadowEffect(CCharacterBase* pParent,
                  DWORD color = 0x00888888,
                  DWORD BlendType = GBLEND_HANTOUMEI,
                  UINT Duration = 20);

    virtual ~CShadowEffect(void);

protected:
    void Update(float time);
    DWORD Draw(void);

    DWORD m_color;
    DWORD m_BlendType;
};

/*class CGBitmap
{
public:
    CGBitmap() { pBmp = NULL; }
    CGBitmap(char* FileName, char* PalName = NULL) { CGBitmap(); LoadBitmap(FileName, PalName); }
    void LoadBitmap(char* FileName, char* PalName = NULL) { Abonnu(); pBmp = CGoluahObject::LoadBitmap(FileName, PalName); }
    operator MYSURFACE*() { return pBmp; }
    virtual ~CGBitmap() { Abonnu(); }

private:
    void Abonnu() { if (pBmp) CGoluahObject::UnloadBitMap(pBmp); }
    MYSURFACE* pBmp;
};

class CGCD
{
public:
    CGCD() { pRect = pCell = pHantei = NULL; }
};*/

/*!@}*/
