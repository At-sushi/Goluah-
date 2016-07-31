/*=====================================================================================

Goluah!! Copyright (C) 2001-2004 aki, 2004-2016 At-sushi, 2014-2016 logger

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

=======================================================================================*/

/*==============================================================

	キャラクター定義ヘッダ

================================================================*/
#pragma once 

#include <windows.h>
#include <stdio.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "gobjbase.h"
#include "cell.h"
#include "aki3d.h"

#include "opt.h"

//飛道具クラス一覧
class CChibasiri;
class CTrack;
class CMiniGunE;
class CMiniGunB;

#define NUM_MINIGUN (3)

#define NEEDGAUGE_REV	(0.3)
#define NEEDGAUGE_GC	(0.5)

/*--------------------------------------------------------------
	メインキャラクタークラス
----------------------------------------------------------------*/
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
	BOOL Command_OnNormal(DWORD keyinfo);
	BOOL Command_Normal(DWORD keyinfo);
	BOOL Command_Hissatu(DWORD keyinfo);
	BOOL Command_OnAttacking(DWORD keyinfo);
	BOOL Command_OnSystem(DWORD key);
	BOOL Command_OnHissatuAttacking(DWORD key);
	DWORD CommandCOM(DWORD wid);
	DWORD CmdCom_OnNormal(DWORD wid);
	DWORD TouchB(ATTACKINFO *info,BOOL hit);
	DWORD TouchC(ATTACKINFO *info,DWORD tc_eid);

	//初期化処理
	void InitParameters();		//パラメータ初期化
	void InitAnalyzeOptions();	//オプションフラグ分割
	void InitBullets();			//飛道具初期化処理
	void InitAttackInfo();		//攻撃力情報の初期化
	void InitWazInfo();			//技情報の初期化

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

	//*********************************************
	//　CCharacterクラス独自の関数
	//*********************************************
private:

	void StartAttack(DWORD atk_info_id);
	BOOL ComLevelCk(UINT level);

	//追加行動関数
	void act_att_sb2();
	void act_att_sc2();
	void act_att_sc3();
	void act_landing();
	void act_genocide1g();
	void act_genocide1j();
	void act_genocide2g();
	void act_genocide2j();
	void act_genocide3g();
	void act_genocide3j();
	void act_landingg();
	void act_dashf();
	void act_dashb();
	void act_chiba1();
	void act_chiba2();
	void act_chiba3();
	void act_minigun();
	void act_minigun2();
	void act_gcancel1();
	void act_gcancel2();
	void act_gcancel3();
	void act_2nd_step();
	void act_genocide_up();
	void act_genocide_down();
	void act_nage();
	void act_striker_in();
	void act_striker_atk1();
	void act_striker_atk2();
	void act_striker_out();
	
	//敵側行動
	DWORD eact_nage(GOBJECT *pedat);

	//攻撃力情報
	ATTACKINFO aif[32];

	//飛道具
	CChibasiri *bul_chiba;
	CTrack *efe_track;
	CMiniGunB *bul_minigun[NUM_MINIGUN];
	CMiniGunE *efe_minigun;

	//オプションフラグ
	BOOL m_opt_ExtraAttack;
	BOOL m_opt_GuardCancel;
	BOOL m_opt_GCutterPlus;
	BOOL m_opt_ChibasiriPlus;
	BOOL m_opt_2ndStep;
	BOOL m_opt_StockableGauge;
	BOOL m_opt_minigunReloaded;

	//内部動作フラグ
	BOOL m_2nd_step_flag;
};

extern LPTSTR CharDirectory;

//行動IDをここに追加する
#define ACTID_LANDING		(ACTID_HISSATU | 0x0001)								//着地時の隙(行動できないように、必殺技扱い)
#define ACTID_ATT_SB2		(ACTID_ATTACK | 0x0002)									//立中、追加入力技
#define ACTID_ATT_SC2		(ACTID_ATTACK | ACTID_KUCYU | 0x0003)					//立強、追加入力技
#define ACTID_ATT_SC3		(ACTID_ATTACK | 0x0004)									//立強、レバー前入れ
#define ACTID_GENOCIDE1G	(ACTID_ATTACK | ACTID_HISSATU | 0x0005)					//ジェノサイド弱地上
#define ACTID_GENOCIDE1J	(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0006)	//ジェノサイド弱空中
#define ACTID_GENOCIDE2G	(ACTID_ATTACK | ACTID_HISSATU | 0x0007)					//ジェノサイド中地上
#define ACTID_GENOCIDE2J	(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0008)	//ジェノサイド中空中
#define ACTID_GENOCIDE3G	(ACTID_ATTACK | ACTID_HISSATU | 0x0009)					//ジェノサイド強地上
#define ACTID_GENOCIDE3J	(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x000A)	//ジェノサイド強空中
#define ACTID_LANDINGG		(ACTID_HISSATU | 0x000B)								//ジェノサイド着地時の隙
#define ACTID_DASHF			(0x000C)												//ダッシュ（前）
#define ACTID_DASHB			(ACTID_KUCYU |0x000D)									//ダッシュ（後）
#define ACTID_CHIBA1		(ACTID_ATTACK | ACTID_HISSATU | 0x000E)					//地走り・弱
#define ACTID_CHIBA2		(ACTID_ATTACK | ACTID_HISSATU | 0x000F)					//地走り・厨
#define ACTID_CHIBA3		(ACTID_ATTACK | ACTID_HISSATU | 0x0010)					//地走り・強
#define ACTID_MINIGUN		(ACTID_ATTACK | ACTID_HISSATU | 0x0011)					//ミニガン
#define ACTID_MINIGUN2		(ACTID_ATTACK | ACTID_HISSATU | 0x0012)					//ミニガン・隙
#define ACTID_GCANCEL1		(ACTID_ATTACK | ACTID_HISSATU | 0x0013)					//ガードキャンセル発動・弱
#define ACTID_GCANCEL2		(ACTID_ATTACK | ACTID_HISSATU | 0x0014)					//ガードキャンセル発動・中
#define ACTID_GCANCEL3		(ACTID_ATTACK | ACTID_HISSATU | 0x0015)					//ガードキャンセル発動・強
#define ACTID_2ND_STEP		(ACTID_KUCYU |0x0016)									//2段ジャンプ発動
#define ACTID_GENOCIDE_UP	(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0017)	//ジェノサイド追加入力↑
#define ACTID_GENOCIDE_DOWN	(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0018)	//ジェノサイド追加入力↓
#define ACTID_NAGE1			(ACTID_ATTACK | ACTID_NAGE | 0x0019)					//投げ
#define ACTID_STRIKER_IN	(ACTID_INOUT  | 0x001A)
#define ACTID_STRIKER_ATK1	(ACTID_INOUT  | 0x001B)
#define ACTID_STRIKER_ATK2	(ACTID_INOUT  | 0x001C)
#define ACTID_STRIKER_OUT	(ACTID_INOUT  | 0x001D)

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
#define ATTACKINFO_STAND_B2		9	//立ち中追加入力技
#define ATTACKINFO_STAND_C2		10	//立ち強追加入力技
#define ATTACKINFO_STAND_C3		11	//立ち強特殊入力技
#define ATTACKINFO_GENOCIDE1	12	//ジェノサイド・弱
#define ATTACKINFO_GENOCIDE2	13	//ジェノサイド・厨
#define ATTACKINFO_GENOCIDE3	14	//ジェノサイド・強
#define ATTACKINFO_CHIBA1		15	//地走り・弱
#define ATTACKINFO_CHIBA2		16	//地走り・厨
#define ATTACKINFO_CHIBA3		17	//地走り・強
#define ATTACKINFO_MINIGUN		18	//ミニガン
#define ATTACKINFO_GENOCIDE_UP	19	//ジェノサイド・追加↑
#define ATTACKINFO_GENOCIDE_DOWN 20	//ジェノサイド・追加↓
#define ATTACKINFO_NAGE			21	//投げ
#define ATTACKINFO_STRIKER1		22	//ストライカー用1
#define ATTACKINFO_STRIKER2		23	//ストライカー用2

//攻撃ID（必要があれば変更する）
#define ATTACK_WEAK				1
#define ATTACK_MIDDLE			2
#define ATTACK_STRONG			3
#define ATTACK_HISSATU			4

//チェーンコンボフラグ
//#define CHAIN_SA	0x00010000 ベースで定義
//#define CHAIN_SB	0x00020000
//#define CHAIN_SC	0x00040000
//#define CHAIN_CA	0x00080000
//#define CHAIN_CB	0x00100000
//#define CHAIN_CC	0x00200000
#define CHAIN_SC3	0x00000001//追加分

//ユーザーヒットID
#define USERHIT_GCUTTERUP		0x81

//ユーザー定義メッセージ
#define GOBJMSG_NAGE			(GOBJMSG_USERDEFINE | 0x0001)				//投げ用
#define GOBJMSG_CHIBAHIT		(GOBJMSG_USERDEFINE | 0x0002)				//地走りヒット
#define GOBJMSG_CHIBAGUARD		(GOBJMSG_USERDEFINE | 0x0003)				//地走りガード
#define GOBJMSG_GCUTTERUP		(GOBJMSG_USERDEFINE | USERHIT_GCUTTERUP)	//Gカッター(上)・ヒット

//aki用マクロ定義
#define cell(a)					pdat->cnow=a
#define ccounter				pdat->counter
#define PI						(3.14159265f)
#define WAIT_A_SEC				TRUE//まってろ、今逝く

//間合い
#define MAAI_NAGE				(20)
#define MAAI_SHORT				(50)
#define MAAI_MIDDLE				(120)
#define MAAI_LONG				(220)

#define wave_tojyo				4
#define	wave_win				5
#define	wave_kakato				6
#define	wave_punch				7
#define wave_gcut				8
#define wave_gcut_up			9
#define wave_gcut_down			10
#define wave_chiwo				11
#define wave_gun				12
#define wave_gunfire			13
#define wave_gunfire2			14
#define wave_striker_in			15
#define wave_striker_atk		16
#define wave_striker_call		17

/*--------------------------------------------------------------
	地走り
----------------------------------------------------------------*/
class CChibasiri : public CBulletBase
{
public:
	CChibasiri(CCharacterBase *parent,BOOL ext);
	void Startup(
			ATTACKINFO *patkinfo,
			DWORD hit_msg_id,
			DWORD grd_msg_id,
			BOOL muki,
			double x,
			double y,
			double vx
			);
	BOOL m_extended;

protected:
	void act_run();
	void act_bomb();
};


/*--------------------------------------------------------------
	軌跡
----------------------------------------------------------------*/
class CTrack : public CBulletBase
{
public:
	CTrack(CCharacterBase *parent);
	virtual ~CTrack(){Destroy();}

	void Create();
	void Destroy();

	DWORD Draw();

	void Startup(
			float width,
			DWORD dur,
			DWORD stop,
			float inirot,
			float rotspd,
			float rotdump,
			float offset,
			float offset_damp,
			float offset2,
			BOOL inv_flag = FALSE
			);

	void SetColor(BYTE a,BYTE r,BYTE g,BYTE b);

	float parentX(){ return (float)GetParentObj()->x; }
	float parentY(){ return (float)GetParentObj()->y; }

protected:
	void act_run();	//実際には何もしない
	void act_bomb();//実際には何もしない

	void PosUpdate();//新規頂点追加

	CTristrip* m_ts;

	//パラメータ
	float m_width;
	ARGB m_color;
	float m_inirot;
	float m_rotspd;
	float m_rotdamp;
	float m_offset;
	float m_offsetdamp;
	float m_offset2;
	DWORD m_duration;
	DWORD m_stopcount;

	//内部パラメータ
	float m_rot;
	UINT m_num,m_step;
	V2d  m_ppos,m_pvec;
	V2d *m_pos;
	UINT m_head;
	UINT m_tail;
};


/*--------------------------------------------------------------
	ミニガン
----------------------------------------------------------------*/
//弾
class CMiniGunB : public CBulletBase
{
public:
	CMiniGunB(CCharacterBase *parent);
	void Setup(
			ATTACKINFO *patkinfo,
			DWORD hit_msg_id,
			DWORD grd_msg_id
			);
protected:
	void act_run();
	void act_bomb();
};

//エフェクト
class CMiniGunE : public CBulletBase
{
public:
	CMiniGunE(CCharacterBase *parent);

protected:
	void act_run();
	void act_bomb(){}
};


