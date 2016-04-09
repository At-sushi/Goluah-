/*
2011/10/29	timeoverのボイスに対応
			数字の描写に使うcellを時間制限・Hit・Damage等で分けた
*/
/*=========================================================================
	
	戦闘タスク基礎

	ネットワーク対応・ネットワーク非対応 の両方の戦闘タスクの共通部分

===========================================================================*/
#pragma once

#include "gobject.h"
#include "object.h"
#include "task.h"

struct MY2DVECTOR;

/*!
*	@defgroup Battle
*	@ingroup Tasks
*	@brief 試合関係
*/

/*!
*	@brief 試合が今どういう状態か
*	@ingroup Battle
*/
enum BattleTaskState
{
	BFSTATE_WAITFORENDPOSE	,//!<登場ポーズ終了待ち
	BFSTATE_ROUNDCALL		,//!<ラウンドコール終了待ち
	BFSTATE_FIGHTING		,//!<戦闘状態
	BFSTATE_FINISHED		,//!<KO、キャラクタの最終ダウン終了待ち
	BFSTATE_WAITFORENDWIN	,//!<勝利ポーズ終了待ち
	BFSTATE_DOUBLEKO		,//!<ダブルKO表示終了待ち
	BFSTATE_TIMEOVER		 //!<タイムオーバー表示終了待ち
};


/*!
*	@brief 戦闘タスクベースクラス
*	@ingroup Battle
*
*	万が一ネット対戦が実装されるかもしれないと、共通操作を可能な限り別クラスに実装してみた。
*	継承クラスでやるべきことは、オブジェクトと試合進行の管理。たぶん。
*	途中からめんどくさくなって、ちゃんと分けられていない部分もあるが･･・
*
*	@sa CBattleTask
*/
class CBattleTaskBase : public CExclusiveTaskBase
{
public:
	CBattleTaskBase();
	virtual ~CBattleTaskBase(){}

	//タスク系関数
	virtual void Initialize();
	virtual void Terminate();
	virtual DWORD GetID(){return 'Btl';}

	//オブジェクト管理部分
	//オブジェクトの管理はサブクラスで実装する
	virtual CGObject* GetGObject(DWORD id)=0;					//!< 指定IDオブジェクト取得
	virtual DWORD CreateGObject()=0;							//!< オブジェクト生成
	virtual DWORD CreateGObjectFx()=0;							//!< オブジェクト生成(エフェクト)
	virtual void DeleteGObject(DWORD oid)=0;					//!< オブジェクト破棄
	virtual void SuicideGObject(DWORD oid)=0;					//!< オブジェクト破棄予約
	virtual CGObject* GetCharacterObject(DWORD j,DWORD i) = 0;	//!< キャラクターのオブジェクト取得（ゲージ等で必要）

	//オブジェクトサービス
	//オーバーロードの必要があるかどうかわからないものは一応virtualに
	virtual DWORD MessageFromObject(DWORD oid,DWORD msg,DWORD prm) = 0;
	virtual BOOL CatchObject(DWORD eoid,LPVOID cy) = 0;
	        void AddDamage(DWORD oid,DWORD eoid,int x,int y);	//!< GetObjectを利用する
	        DWORD GetMaai(DWORD oid,DWORD eoid,BOOL v);			//!< GetObjectを利用する
	        GOBJECT* GetGObjectInfo(DWORD oid);					//!< GetObjectを利用する
	        GOBJECT* GetCharacterInfo(DWORD j,DWORD i);			//!< GetObjectを利用する
	virtual void AddEffect(DWORD efctid,int prm1,int prm2,int prm3=0) = 0;

	//パラメータ設定・取得系
	DWORD GetWinCount(DWORD tid);								//!< 勝利カウント数を取得
	DWORD GetStrikerCount(DWORD tid);							//!< 残りストライカー数を取得
	TCHAR* GetKatiSerif();										//!< 勝利台詞を取得
	void SetKatiSerif(DWORD tid,TCHAR *serif);					//!< 勝利台詞を設定
	void* GetActiveCharacterInfo(DWORD tid);					//!< 「アクティブ」なキャラクタの取得
	CDI_CHARACTERINFO2* GetCDICharacterInfo(DWORD tid,DWORD index){return &m_cinfo[tid][index];}
	SDI_STAGEINFO2* GetSDIStageInfo(){return &m_sinfo;}			
	BattleTaskState GetState(){return bf_state;}				//!< 試合状態取得
	DWORD GetActiveCharacterID(DWORD team);
	GOBJECT* GetActiveCharacter(DWORD team);					//!< "アクティブ"なキャラクターを返す
	DWORD GetCharacterOID(DWORD t,DWORD m){return charobjid[t][m];}
	virtual double GetDisplayCenterX() = 0;						//!< 表示中心X取得
	virtual double GetDisplayCenterY() = 0;						//!< 表示中心Y取得
	virtual BOOL IsNetwork() = 0;								//!< ネットワークかどうか
	virtual BOOL IsLocal(BYTE dllid) = 0;						//!< 指定DLLがローカルかどうか

	//描画サービス
	virtual void SetTransform(BOOL b) = 0;
	void DrawNumber(DWORD num,int x,int y,BOOL hits,float z, float magx = 1.0f, float magy = 1.0f);
	void DrawNumber2(DWORD num,int x,int y,float z);
	void DrawNumber3(double numd,int x,int y,float z);			//!< ゲージ1用 数字描画
	void DrawNumber4(double numd,int x,int y,float z);			//!< ゲージ2用 数字描画
	void DrawNumber5(DWORD num,int x,int y,BOOL hits,float z, float magx = 1.0f, float magy = 1.0f);
	void DrawNumber6(DWORD num,int x,int y,float z);
	void DrawNumber7(double numd,int x,int y,float z);			//!< ゲージ1用 数字描画
	void DrawNumber8(double numd,int x,int y,float z);			//!< ゲージ2用 数字描画
	void DrawNumber9(DWORD num,int x,int y,BOOL hits,float z, float magx = 1.0f, float magy = 1.0f);	//時間制限
	void DrawGObject(GOBJECT* pdat);

	//オブジェクト生成に必要
	Config2_Difficulty	GetCurrentComLevel(){return m_current_com_level;}
	BYTE				GetCurrentDLLID(){return m_crnt_dllid;}

	//DLLエラー処理
	virtual void Notify_Exception(CGObject *obj,DWORD msgid,DWORD prm);	//!< DLLが例外を起こしたときの処理
	static const TCHAR* MessageID2String(DWORD id);						//!< オブジェクトメッセージのIDを、文字列に変換
	const TCHAR* GetBattleStateString();									//!< 試合状態IDを、文字列に変換して返す
	virtual void SetCurrentDLLID(BYTE dllID)	{m_crnt_dllid=dllID;}	//!< CGObjectの生成時に呼ばれ、DLLのIDを返す
	virtual void SetCurrentDLLIDSystem()		{m_crnt_dllid=0;}		//!< CGObjectがDLLの関数を呼び出す前後に呼ばれ、現在のDLLIDを設定する

protected:
	//Initializeからコール
	virtual void InitializeObjectList() = 0;							//!< オブジェクトリストの初期化
	virtual void InitializeDLLLoadInfo();								//!< DLLロード時に渡す構造体を初期化する
	virtual void InitializeLoadDLLs();									//!< DLLをロードする
	virtual void InitializeSubTasks() = 0;								//!< 試合時に使う支援クラス等を初期化
	virtual void InitializeParameters() = 0;							//!< 試合開始前パラメータ初期化

	//Terminateからコール
	virtual void TerminateObjectList() = 0;								//!< オブジェクトリスト破棄
	virtual void TerminateUnloadDLLs();									//!< DLLアンロード
	virtual void TerminateDestroySubTasks() = 0;						//!< 試合時に使った支援クラス等を破棄

	virtual void StartRound()=0;										//!< ラウンド開始時パラメータリセット処理

	virtual void Atari(DWORD a_id,DWORD k_id,MY2DVECTOR &kas_point)=0;	//!< 攻撃ヒット時処理
	virtual void HitStop(DWORD len,DWORD oid)=0;						//!< ヒットストップエフェクト処理

	//for debug text out
	void DrawCharacterState();
	void DrawState();
	void DrawObjectList();

	//変数
public:
	int limittime;						//!< 制限時間(マイナスは∞)
protected:
	HINSTANCE hlib_c[2][MAXNUM_TEAM];	//!< キャラクターのDLL インスタンスハンドル
	HINSTANCE hlib_s;					//!< ステージのDLL インスタンスハンドル

	//サウンド
	LPDIRECTSOUNDBUFFER dsb_round;		//!< サウンド 「ラウンド～」
	LPDIRECTSOUNDBUFFER dsb_fight;		//!< サウンド 「ファイト」
	LPDIRECTSOUNDBUFFER dsb_ko;			//!< サウンド 「ＫＯ」
	LPDIRECTSOUNDBUFFER dsb_timeover;	//!< サウンド 「タイムオーバー」

	//ラウンド通して変化しないパラメータ
	CDI_CHARACTERINFO2 m_cinfo[2][MAXNUM_TEAM];	//!< DLLとやり取りするキャラクター情報
	SDI_STAGEINFO2 m_sinfo;						//!< DLLとやり取りするステージ情報
	DWORD charobjid[2][MAXNUM_TEAM];			//!< キャラクターのオブジェクトID
	DWORD active_character[2];					//!< アクティブな（現在戦闘中の）キャラクターのIndex
	DWORD stgobjid;								//!< ステージのオブジェクトID

	//試合開始時にリセット
	int round;							//!< ラウンド数
	DWORD wincount[2];					//!< 勝利数
	DWORD strikercount[2];				//!< 支援攻撃可能数

	//ラウンド開始時にリセット
	BattleTaskState bf_state;			//!< 試合の状態
	DWORD bf_counter;					//!< メインカウンタ
	DWORD bf_hitdisp[2];				//!< ヒット数カウント表示カウンタ

	//DLLロード時に設定。
	//DLLロード後はg_configの値に固定
	Config2_Difficulty m_current_com_level;//!< CGObjectが生成時に使用するCOMレベル

	/*!
	*	@brief DLL-ID
	*
	*	DLLロード時に以下のように設定してDLLのCreate関数を呼び出す。
	*	char1-1:1 , char1-2:2 , char1-3:3 , char2-1:4 , char2-2:5 , char2-3:6 , stage:7
	*	
	*	その後はCGObjectがDLLの関数を呼び出すたびにこのIDを、CGObjectがもつIDに設定する。
	*	その関数呼び出し内でオブジェクトが生成されれば、生成されたCGObjectはこのIDを初期化時に取得する。
	*	こうして、すべてのオブジェクトが(一部の考えにくい例外を除いて)どのDLLで生成されたものかが特定できる。
	*	DLLの関数から抜けたときには、IDは0(system)とする。
	*/
	BYTE m_crnt_dllid;
};


