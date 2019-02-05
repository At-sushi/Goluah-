
/*==============================================================

	キャラクター定義ヘッダ

================================================================*/
#pragma once

#include <windows.h>
#include <stdio.h>

#include "aki3d.h"
#include "gobjbase.h"
#include "cell.h"// 通常、are_editerで生成したヘッダーファイルをここにincludeします

class CReiku_Re;
class CReiku_I;
class CReiku_Ku;
class CDashWind;
class CHadouKen;
class CGoodbye;
class CRocket;
//class CEraser99;

#define NEEDGAUGE_REV	(0.5)
#define NEEDGAUGE_GC	(0.3)

// エフェクト用オブジェクト作成マクロ
#define CREATE_GOBJ_FX(obj)			{ CGoluahObject* pointer = new obj; if ( pointer && !pointer->GetGObject() )\
										{ delete pointer; }\
									}

/*--------------------------------------------------------------
	メインキャラクタークラス
----------------------------------------------------------------*/

#define MAXNUM_ATTACKINFO	(34)

class CCharacter : public CCharacterBase
{
public:
	CCharacter(CDI_CHARACTERINFO2 *info);
	virtual ~CCharacter();

protected:
	//*********************************************
	//  オーバーライドする関数
	//*********************************************

	DWORD Message(DWORD msg,LPVOID pd,DWORD prm);
	DWORD Action();
	void PreAction();
	BOOL Command_OnHissatuAttacking(DWORD keyinfo);
	BOOL Command_OnNormal(DWORD keyinfo);
	BOOL Command_Normal(DWORD keyinfo);
	BOOL Command_Hissatu(DWORD keyinfo);
	BOOL Command_OnAttacking(DWORD keyinfo);
	BOOL Command_OnSystem(DWORD keyinfo);
	DWORD CommandCOM(DWORD wid);
	DWORD CmdCom_OnNormal(DWORD wid);
	DWORD CmdCom_OnSystem(DWORD wid);
	DWORD TouchA(ATTACKINFO *info,DWORD ta_eid);
	DWORD TouchB(ATTACKINFO *info,BOOL hit);
	DWORD TouchC(ATTACKINFO *info,DWORD tc_eid);

	//通常動作
	void act_neutral();	//ニュートラル
	void act_crouch();	//しゃがみ
	void act_rakka();	//落下
	void act_tojyo();	//登場
	void act_win();		//勝利
	void act_walkf();	//前進
	void act_walkb();	//後退
	void act_jamps();	//ジャンプ予備動作
	void act_jampc();	//垂直ジャンプ
	void act_jampf();	//前ジャンプ
	void act_jampb();	//後ジャンプ
	void act_strikercomeon();//カモ!
	void act_timeoverlose();//タイムオーバー
	//通常.攻撃
	void act_att_sa();	//立弱
	void act_att_sb();	//立中
	void act_att_sc();	//立強
	void act_att_ca();	//屈弱
	void act_att_cb();	//屈中
	void act_att_cc();	//屈強
	void act_att_ja();	//飛弱
	void act_att_jb();	//飛中
	void act_att_jc();	//飛強
	//その他
	void act_taiki();
	void act_koutai_out();
	void act_koutai_in();
	void act_koutai();
	void act_koutai2();

	//初期化関係
	void InitAnalyzeOptions();	//オプションフラグ分割
	void InitParameters();		//パラメータ初期化
	void InitAttackInfo();		//攻撃力情報の初期化
	void InitWazInfo();			//技情報の初期化
	void InitBullets();			//飛道具初期化

	//*********************************************
	//　CCharacterクラス独自の関数
	//*********************************************
private:
	void StartAttack(DWORD atk_info_id);
	void EndAttack();
	BOOL EnemyAction(DWORD eid,DWORD hid);
	BOOL ComLevelCk(UINT level);

	//追加行動関数
	void act_att_sd();		//→中
	void act_att_se();		//→強
	void act_tatumaki1(DWORD atkid);	//竜巻(弱)
	void act_tatumaki2(DWORD atkid);	//竜巻(中)
	void act_tatumaki3(DWORD atkid);	//竜巻(強)
	void act_tatumakir();	//竜巻落下
	void act_landing();		//竜巻着地
	void act_reiku1a();		//レイク(弱)
	void act_reiku1b();
	void act_reiku1c();
	void act_reiku2a();		//レイク(中)
	void act_reiku2b();
	void act_reiku2c();
	void act_reiku3a();		//レイク(強)
	void act_reiku3b();
	void act_reiku3c();
	void act_reiku3cs();	//レイク着地後隙
	void act_dashf();		//前方ダッシュ
	void act_dashb();		//後方ダッシュ
	void act_nage();		//投げ
	void act_hadou1a();		//波動拳（飛ばない系）
	void act_hadou2a();
	void act_hadou3a();
	void act_hadou1b();		//波動拳（飛ぶ系）
	void act_hadou2b();
	void act_hadou3b();
	void act_gcancel1();	//ガードキャンセル
	void act_gcancel2();
	void act_gcancel3();
	void act_gcancel1j();
	void act_gcancel2j();
	void act_gcancel3j();
	void act_2nd_step();	//2段ジャンプ
	void act_ajamp();		//エリアル始動ジャンプ
	void act_goodbye1();	//グッバイ・フォーエバー
	void act_goodbye2();	//グッバイ・フォーエバー
	void act_striker_in();
	void act_striker_dash();
	void act_striker_attack();
	void act_striker_out();

	//敵側行動
	DWORD eact_nage(GOBJECT* pedat);	//投げ
	void eact_aerialpop(GOBJECT* pedat);//エリアル・浮かせヒット
	void eact_aerialhit1(GOBJECT* pedat);//エリアル・空中ヒット
	void eact_aerialhit2(GOBJECT* pedat);//エリアル・空中ヒット
	void eact_aerialhit3(GOBJECT* pedat);//エリアル・空中ヒット

	//攻撃力情報
	ATTACKINFO aif[MAXNUM_ATTACKINFO];


	//飛道具
	CReiku_Re*		pefe_re;	//レイクエフェクト「レ」
	CReiku_I*		pefe_i;		//レイクエフェクト「イ」
	CReiku_Ku*		pefe_ku;	//レイクエフェクト「ク」
	CDashWind*		pefe_dash;	//ダッシュ時風エフェクト
	CHadouKen*		pbul_hadou;	//波動拳
	CGoodbye*		pbul_goodbye;//グッバイF
//	CRocket*		pbul_rocket;
//	CEraser99*		pbul_eraser99;

	//オプション
	BOOL			m_opt_hpp30;	//HP+30%
	BOOL			m_opt_exAttack;	//追加入力技
	BOOL			m_opt_hadou;	//波動拳
	BOOL			m_opt_AAttack;	//エリアル
	BOOL			m_opt_AStep;	//2段ジャンプ
	BOOL			m_opt_gcancel;	//ガードキャンセル

	//動作フラグ
	BOOL			m_2nd_step_flag;//2段ジャンプ可能フラグ
	BOOL			m_aerial;		//エリアル中フラグ
};

extern LPTSTR CharDirectory;

//行動IDをここに追加する
#define ACTID_ATT_SD			(ACTID_ATTACK  | 0x0001)								//立中、レバー前入れ
#define ACTID_ATT_SE			(ACTID_ATTACK  | 0x0002)								//立強、レバー前入れ
#define ACTID_TATUMAKI1			(ACTID_ATTACK  | ACTID_HISSATU | ACTID_KUCYU | 0x0003)	//竜巻（弱）
#define ACTID_TATUMAKI2			(ACTID_ATTACK  | ACTID_HISSATU | ACTID_KUCYU | 0x0004)	//竜巻（中）
#define ACTID_TATUMAKI3			(ACTID_ATTACK  | ACTID_HISSATU | ACTID_KUCYU | 0x0005)	//竜巻（強）
#define ACTID_TATUMAKIR			(ACTID_HISSATU | ACTID_KUCYU   | 0x0006)				//竜巻（落下）
#define ACTID_LANDING			(ACTID_HISSATU | 0x0007)								//竜巻（着地）
#define ACTID_REIKU1A			(ACTID_ATTACK  | ACTID_HISSATU | 0x0008)				//レイク(弱)
#define ACTID_REIKU1B			(ACTID_ATTACK  | ACTID_HISSATU | 0x0009)
#define ACTID_REIKU1C			(ACTID_ATTACK  | ACTID_HISSATU | 0x000A)
#define ACTID_REIKU2A			(ACTID_ATTACK  | ACTID_HISSATU | 0x000B)				//レイク(中)
#define ACTID_REIKU2B			(ACTID_ATTACK  | ACTID_HISSATU | 0x000C)
#define ACTID_REIKU2C			(ACTID_ATTACK  | ACTID_HISSATU | 0x000D)
#define ACTID_REIKU3A			(ACTID_ATTACK  | ACTID_HISSATU | 0x000E)				//レイク(強)
#define ACTID_REIKU3B			(ACTID_ATTACK  | ACTID_HISSATU | 0x000F)
#define ACTID_REIKU3C			(ACTID_ATTACK  | ACTID_HISSATU | ACTID_KUCYU | 0x0010)
#define ACTID_REIKU3CS			(ACTID_HISSATU | 0x0011)								//レイク着地後隙
//#define ACTID_REIKUCOM			(ACTID_ATTACK  | ACTID_HISSATU | 0x0012)				//レイク(COM)
#define ACTID_DASHF				(0x0013)												//前ダッシュ
#define ACTID_DASHB				(0x0014)												//後ダッシュ
#define ACTID_NAGE1				(ACTID_ATTACK  | ACTID_NAGE    | 0x0015)				//投げ
#define ACTID_HADOU1A			(ACTID_ATTACK  | ACTID_HISSATU | 0x0016)				//波動拳A(弱)
#define ACTID_HADOU2A			(ACTID_ATTACK  | ACTID_HISSATU | 0x0017)				//波動拳A(厨)
#define ACTID_HADOU3A			(ACTID_ATTACK  | ACTID_HISSATU | 0x0018)				//波動拳A(強)
#define ACTID_HADOU1B			(ACTID_ATTACK  | ACTID_HISSATU | 0x0019)				//波動拳B(弱)
#define ACTID_HADOU2B			(ACTID_ATTACK  | ACTID_HISSATU | 0x001A)				//波動拳B(厨)
#define ACTID_HADOU3B			(ACTID_ATTACK  | ACTID_HISSATU | 0x001B)				//波動拳B(強)
#define ACTID_GCANCEL1			(ACTID_ATTACK  | ACTID_HISSATU | 0x001C)				//ガーキャン(弱)
#define ACTID_GCANCEL2			(ACTID_ATTACK  | ACTID_HISSATU | 0x001D)				//ガーキャン(中)
#define ACTID_GCANCEL3			(ACTID_ATTACK  | ACTID_HISSATU | 0x001E)				//ガーキャン(強)
#define ACTID_GOODBYE1			(ACTID_ATTACK  | ACTID_HISSATU | 0x001F)				//グッバイ・フォーエバー(始動→前進)
#define ACTID_2ND_STEP			(ACTID_KUCYU   | 0x0020)								//2段ジャンプ発動
#define ACTID_AJAMP				(ACTID_KUCYU   | 0x0021)								//エリアル始動ジャンプ
#define ACTID_GOODBYE2			(ACTID_ATTACK  | ACTID_HISSATU | 0x0022)				//グッバイ・フォーエバー(ヒット→打ち上げ)
#define ACTID_STRIKER_IN		(ACTID_INOUT   | 0x0023)								//支援攻撃・画面内に入ってくる
#define ACTID_STRIKER_DASH		(ACTID_INOUT   | 0x0024)								//支援攻撃・相手のほうにちょっと進む
#define ACTID_STRIKER_ATTACK	(ACTID_INOUT   | 0x0025)								//支援攻撃・攻撃
#define ACTID_STRIKER_OUT		(ACTID_INOUT   | 0x0026)								//支援攻撃・着地、お帰りはこちら
#define ACTID_GCANCEL1J			(ACTID_ATTACK  | ACTID_HISSATU | ACTID_KUCYU | 0x0027)	//ガーキャン(弱) - 発動後行動
#define ACTID_GCANCEL2J			(ACTID_ATTACK  | ACTID_HISSATU | ACTID_KUCYU | 0x0028)	//ガーキャン(中) - 発動後行動
#define ACTID_GCANCEL3J			(ACTID_ATTACK  | ACTID_HISSATU | ACTID_KUCYU | 0x0029)	//ガーキャン(強) - 発動後行動

//攻撃力番号をここに定義する
#define ATTACKINFO_STAND_A		0	//通常技色々
#define ATTACKINFO_STAND_B		1
#define ATTACKINFO_STAND_C		2
#define ATTACKINFO_CROUCH_A		3
#define ATTACKINFO_CROUCH_B		4
#define ATTACKINFO_CROUCH_C		5
#define ATTACKINFO_JAMP_A		6
#define ATTACKINFO_JAMP_B		7
#define ATTACKINFO_JAMP_C		8
#define ATTACKINFO_STAND_D		9	//→中
#define ATTACKINFO_STAND_E		10	//→強
#define ATTACKINFO_TATUMAKI1	11	//竜巻(弱)
#define ATTACKINFO_TATUMAKI2	12	//竜巻(中)
#define ATTACKINFO_TATUMAKI3	13	//竜巻(強)
#define ATTACKINFO_REIKU		14	//レイク(通常)
#define ATTACKINFO_REIKU_FINISH	15	//レイク(フィニッシュ)
#define ATTACKINFO_NAGE			16	//投げ
#define ATTACKINFO_HADOU1A		17	//波動拳（飛ばない系）
#define ATTACKINFO_HADOU2A		18
#define ATTACKINFO_HADOU3A		19
#define ATTACKINFO_HADOU1B		20	//波動拳（飛ぶ系）
#define ATTACKINFO_HADOU2B		21
#define ATTACKINFO_HADOU3B		22
#define ATTACKINFO_GOODBYE1		23 //グッバイ・フォーエバー
#define ATTACKINFO_GOODBYE2		24
#define ATTACKINFO_AERIALPOP	25 //エリアル関係
#define ATTACKINFO_AERIAL_A		26
#define ATTACKINFO_AERIAL_B		27
#define ATTACKINFO_AERIAL_C		28
#define ATTACKINFO_STRIKER_DASH	29
#define ATTACKINFO_STRIKER		30
#define ATTACKINFO_GCANCEL		31
#define ATTACKINFO_ROCKET		32
#define ATTACKINFO_ERASER99		33//いっぱいいっぱい

//攻撃ID（ゲージを増やすときとかに使用）
#define ATTACK_WEAK				1
#define ATTACK_MIDDLE			2
#define ATTACK_STRONG			3
#define ATTACK_HISSATU			4
#define ATTACK_GOODBYE			5//グッバイヒット
#define ATTACK_STRIKER			6//ストライカーダッシュヒット

//チェーンコンボフラグ
//#define CHAIN_SA	0x00010000//ベースで定義
//#define CHAIN_SB	0x00020000
//#define CHAIN_SC	0x00040000
//#define CHAIN_CA	0x00080000
//#define CHAIN_CB	0x00100000
//#define CHAIN_CC	0x00200000
#define CHAIN_SD	0x00000001//追加分
#define CHAIN_SE	0x00000002
#define CHAIN_JA	0x00000010
#define CHAIN_JB	0x00000020
#define CHAIN_JC	0x00000040

#define CHAIN_JAMPS (CHAIN_JA | CHAIN_JB | CHAIN_JC)//2段ジャンプ時一括クリア用

//ユーザーヒット・ID定義
#define USERHIT_FLAG			(0x80)
#define USERHIT_AERIALPOP		(0x01 | USERHIT_FLAG)
#define USERHIT_AERIAL1			(0x02 | USERHIT_FLAG)
#define USERHIT_AERIAL2			(0x03 | USERHIT_FLAG)
#define USERHIT_AERIAL3			(0x04 | USERHIT_FLAG)

//ユーザー定義メッセージ
#define GOBJMSG_NAGE			(GOBJMSG_USERDEFINE | 0x0001)	//投げ用
#define GOBJMSG_HADOUHIT		(GOBJMSG_USERDEFINE | 0x0002)	//波動拳ヒット
#define GOBJMSG_HADOUGUARD		(GOBJMSG_USERDEFINE | 0x0003)	//波動拳ガード
#define GOBJMSG_AERIALPOP		(GOBJMSG_USERDEFINE | USERHIT_AERIALPOP)	//エリアル・浮かせ
#define GOBJMSG_AERIALHIT1		(GOBJMSG_USERDEFINE | USERHIT_AERIAL1)		//エリアル・ヒット
#define GOBJMSG_AERIALHIT2		(GOBJMSG_USERDEFINE | USERHIT_AERIAL2)		//エリアル・ヒット
#define GOBJMSG_AERIALHIT3		(GOBJMSG_USERDEFINE | USERHIT_AERIAL3)		//エリアル・ヒット

//オプション定義
#define OPTIONS_CHAIN_COMBO		0x00000001
#define OPTIONS_EXTRA_ATTACK	0x00000002
#define OPTIONS_AERIAL_ATTACK	0x00000004
#define OPTIONS_AERIAL_STEP		0x00000008
#define OPTIONS_AUTO_GUARD		0x00000010
#define OPTIONS_GUARD_CANCEL	0x00000020
#define OPTIONS_HADOUKEN		0x00000040
#define OPTIONS_HPPLUS30		0x00000080

//間合い
#define MAAI_SHORT				(50)
#define MAAI_MIDDLE				(120)
#define MAAI_LONG				(220)
#define MAAI_NAGE				(20)

//マクロ定義
#define cell(a)		pdat->cnow=a	// "CELL_" がうざい・・・
#define ccounter	pdat->counter
#define ALLGUARDFLAG		(GUARDINFO_XSTAND | GUARDINFO_XCROUCH | GUARDINFO_XJAMP)
#define CANNOT_GUARD(a)		((a&ALLGUARDFLAG)==0)
#define ALL_RIGHT			TRUE//オーライ

//wav番号
#define wave_tojyo			4
#define wave_win			5
#define wave_body_ga		6
#define wave_kurae			7
#define wave_hado1			8
#define wave_hado2			9
#define	wave_tatumaki		10
#define wave_reiku_re		11
#define wave_reiku_i		12
#define wave_reiku_ku		13
#define wave_goodbye		14
#define wave_striker_in		15
#define wave_striker_atk	16
#define wave_striker_call	17

/*-------------------------------------------------------------------
	飛道具等
---------------------------------------------------------------------*/

//■レイク
class CReiku_Base : public CBulletBase
{
public:
	CReiku_Base(CCharacter *parent);
	void act_run();
	void act_bomb();
protected:
	DWORD m_reiku_cell;
};

class CReiku_Re : public CReiku_Base
{
public:CReiku_Re(CCharacter *parent);
};

class CReiku_I : public CReiku_Base
{
public:CReiku_I(CCharacter *parent);
};

class CReiku_Ku : public CReiku_Base
{
public:CReiku_Ku(CCharacter *parent);
};

//■ダッシュ時の風
class CDashWind : public CBulletBase
{
public:
	CDashWind(CCharacter *parent);
	void act_run();
	void act_bomb();
};

//■波動拳（ベース）
class CHadouKen : public CBulletBase
{
public:
	CHadouKen(CCharacter *parent) : CBulletBase(parent){}
	virtual void GoA(BOOL muki,double x,double y,float duration,ATTACKINFO *atk){}
	virtual void GoB(BOOL muki,double x,double y,double vx,ATTACKINFO *atk){}
};

//■波動拳（飛ばない方）
class CHadouKenA : public CHadouKen
{
public:
	CHadouKenA(CCharacter *parent);
	virtual void GoA(BOOL muki,double x,double y,float duration,ATTACKINFO *atk);

protected:
	void act_run();
	void act_bomb();
	virtual void act_base();
	void Hit();

	DWORD m_hit_count;
	float m_duration;
};

//■波動拳（飛ぶ方）
class CHadouKenB : public CHadouKen
{
public:
	CHadouKenB(CCharacter *parent);
	void GoB(BOOL muki,double x,double y, double vx,ATTACKINFO *atk);

protected:
	void act_run();
	void act_bomb();
	void Hit();
};

//■グッバイフォーエバー
class CGoodbye : public CHadouKenA
{
public:
	CGoodbye(CCharacter *parent);
	void GoA(BOOL muki,double x,double y,float duration,ATTACKINFO *atk);

protected:
	void act_base();
	void Hit();

};

//■ロケット?
#define ROCKET_BOMB_NUM		64

class CRocket : public CBulletBase
{
public:
	CRocket(CCharacterBase* parent,ATTACKINFO& atk);
	CParticle* GetParticle();

protected:
friend class CEraser99;

	void act_run();
	void act_bomb();
	DWORD Draw();

	void Hit(){AddProperty( GOBJFLG_DONOTSTOP );CBulletBase::Hit();}

	struct PBombInfo
	{
		V3d   pos;
		V3d   vel;
		float width;
		float width_base;
		float time;
		float time_limit;
		ARGB  color;

		void Reset();
		void Update();
	};
	PBombInfo m_info[ROCKET_BOMB_NUM];
	CParticle* part_bomb;

};

//■イレイサー99

#define ERASER99_NUM_CONPART	(128)
#define ERASER99_NUM_RAY		(64)

class CEraser99 : public CBulletBase
{
public:
	CEraser99(CCharacterBase* parent,ATTACKINFO& atk);

	void SetRocket( CRocket* rkt ){obj_rkt=rkt;}

	void ForceStop(){if(pdat->aid==CBB_STATE_RUN)Hide();}

protected:
	DWORD Draw();
	void act_run();
	void act_bomb();

	//あたっても消えない
	void Hit(){pdat->kougeki= FALSE;}
	DWORD Sousai(DWORD prm){return TRUE;}
	
	//収束粒子
	CParticle* m_part;
	struct ParticleInfo
	{
		V3d pos;
		V3d vel;

		void Reset();
		void Update();
		void Reset2();
		void Update2();
	};
	ParticleInfo m_info[ERASER99_NUM_CONPART];

	//発動時煙（ロケット流用）
	CRocket*	obj_rkt;

	//光線本体
	CParticle* m_part_ray;
	struct RayPartInfo
	{
		V3d pos;
		V3d ax;
		V3d ax_base;
		float width;
		float width_base;
		ARGB col;

		float time;
		float time_end;

		void Reset();
		void Update(float pt);
	};
	RayPartInfo m_info_ray[ERASER99_NUM_RAY];
};

