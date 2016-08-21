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

#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>
#include <stdio.h>
#include <vector>

#include "gobjbase.h"
#include "cell.h"
#include "aki3d.h"

class C2GetWind;
class CStorm;
class CToate;

#define AIF_NUM			(32)
#define NEEDGAUGE_GC	(0.5)
#define NEEDGAUGE_REV	(0.3)

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
	void PreAction();
	DWORD Action();
	DWORD TouchA(ATTACKINFO *info,DWORD taid);
	DWORD TouchB(ATTACKINFO *info,BOOL hit);
	BOOL Command_OnNormal(DWORD keyinfo);
	BOOL Command_Normal(DWORD keyinfo);
	BOOL Command_Hissatu(DWORD keyinfo);
	BOOL Command_OnAttacking(DWORD key);
	BOOL Command_OnSystem(DWORD keyinfo);
	DWORD CommandCOM(DWORD wid);
	DWORD CmdCom_OnSystem(DWORD wid);
	DWORD CmdCom_OnNormal(DWORD wid);

	void InitAttackInfo();		//攻撃力情報の初期化
	void InitWazInfo();			//技情報の初期化
	void InitBullets();			//飛道具初期化処理

	//通常動作
	void act_neutral();//ニュートラル
	void act_crouch();//しゃがみ
	void act_rakka();//落下
	void act_tojyo();//登場
	void act_win();//勝利
	void act_walkf();//前進
	void act_walkb();//後退
	void act_jamps();//ジャンプ予備動作
	void act_jampc();//垂直ジャンプ
	void act_jampf();//前ジャンプ
	void act_jampb();//後ジャンプ
	//通常.攻撃
	void act_att_sa();//立弱
	void act_att_sb();//立中
	void act_att_sc();//立強
	void act_att_ca();//屈弱
	void act_att_cb();//屈中
	void act_att_cc();//屈強
	void act_att_ja();//飛弱
	void act_att_jb();//飛中
	void act_att_jc();//飛強
	//その他
	void act_taiki();
	void act_koutai_out();
	void act_koutai_in();
	void act_koutai();
	void act_koutai2();
	void act_timeoverlose();
	void act_strikercomeon();

	//*********************************************
	//　CCharacterクラス独自の関数
	//*********************************************
private:
	void InitParameters();//パラメータ初期化
	void StartAttack(DWORD atk_info_id);

	BOOL com1632143(int dt);
	BOOL ComLevelCk(UINT level);

	//追加行動関数
	void act_nage();
	void act_dashf();
	void act_dashb();
	void act_agetackle1(DWORD atkid);
	void act_agetackle2(DWORD atkid);
	void act_agetackle3(DWORD atkid);
	void act_agetacklesuki();
	void act_kiriget();
	void act_sagena();
	void act_sagenasuki();
	void act_atemi1();
	void act_atemi2();
	void act_atemi3();
	void act_atemi_suki();
	void act_atemi_nage();
	void act_cyohi();
	void act_toate1();
	void act_toate2();
	void act_toate3();
	void act_striker_in();
	void act_striker_2get();
	void act_striker_chainsaw();
	void act_striker_out();
	void act_gcancel1();
	void act_gcancel2();
	void act_gcancel3();
	void act_reversal();

	DWORD eact_nage(GOBJECT *pedat);
	DWORD eact_atemi_nage(GOBJECT *pedat);
public:
	static DWORD eact_rotfuttobi(GOBJECT *pedat);

protected:
	//攻撃力情報
	ATTACKINFO aif[AIF_NUM];

	//飛び道具/エフェクト
	C2GetWind	*efe_wind;
	CStorm		*pbul_storm;
	CToate		*pbul_toate;

	//オプションフラグ
	BOOL		m_opt_reversal;
	BOOL		m_opt_gcancel;
	BOOL		m_opt_quick_ch;
	BOOL		m_opt_hevy_hit;
};

extern LPTSTR CharDirectory;

//行動IDをここに追加する
#define ACTID_NAGE1				(ACTID_NAGE  | 0x0001)
#define ACTID_DASHF				(0x1002)
#define ACTID_DASHB				(ACTID_KUCYU  | 0x0003)
#define ACTID_AGETACKLE1		(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0004)
#define ACTID_AGETACKLE2		(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0005)
#define ACTID_AGETACKLE3		(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0006)
#define ACTID_AGETACKLESUKI		(ACTID_ATTACK | ACTID_HISSATU | 0x0007)
#define ACTID_KIRIGET			(ACTID_ATTACK | 0x0008)
#define ACTID_SAGENA			(ACTID_ATTACK | ACTID_HISSATU | 0x0009)
#define ACTID_SAGENASUKI		(ACTID_ATTACK | 0x000A)
#define ACTID_ATEMI1			(ACTID_ATTACK | ACTID_HISSATU | 0x000B)
#define ACTID_ATEMI2			(ACTID_ATTACK | ACTID_HISSATU | 0x000C)
#define ACTID_ATEMI3			(ACTID_ATTACK | ACTID_HISSATU | 0x000D)
#define ACTID_ATEMI_SUKI		(ACTID_ATTACK | ACTID_HISSATU | 0x000E)
#define ACTID_ATEMI_NAGE		(ACTID_ATTACK | ACTID_HISSATU | ACTID_NAGE | 0x000F)
#define ACTID_CYOHI				(ACTID_ATTACK | ACTID_HISSATU | 0x0010)
#define ACTID_TOATE1			(ACTID_ATTACK | ACTID_HISSATU | 0x0011)
#define ACTID_TOATE2			(ACTID_ATTACK | ACTID_HISSATU | 0x0012)
#define ACTID_TOATE3			(ACTID_ATTACK | ACTID_HISSATU | 0x0013)
#define ACTID_STRIKER_IN		(ACTID_INOUT  | 0x0014)
#define ACTID_STRIKER_2GET		(ACTID_INOUT  | 0x0015)
#define ACTID_STRIKER_CHAINSAW	(ACTID_INOUT  | 0x0016)
#define ACTID_STRIKER_OUT		(ACTID_INOUT  | 0x0017)
#define ACTID_GCANCEL1			(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0018)
#define ACTID_GCANCEL2			(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0019)
#define ACTID_GCANCEL3			(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x001A)
#define ACTID_REVERSAL			(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x001B)

//攻撃力番号をここに定義する
#define ATTACKINFO_STAND_A		0
#define ATTACKINFO_STAND_B		1
#define ATTACKINFO_STAND_C		2
#define ATTACKINFO_CROUCH_A		3
#define ATTACKINFO_CROUCH_B		4
#define ATTACKINFO_CROUCH_C		5
#define ATTACKINFO_JAMP_A		6
#define ATTACKINFO_JAMP_B		7
#define ATTACKINFO_JAMP_C		8
#define ATTACKINFO_NAGE			9
#define ATTACKINFO_KIRIGET		10
#define ATTACKINFO_AGETACKLE1	11
#define ATTACKINFO_AGETACKLE2	12
#define ATTACKINFO_AGETACKLE3	13
#define ATTACKINFO_SAGENA		14
#define ATTACKINFO_ATEMI_NAGE	15
#define ATTACKINFO_CYOHI		16
#define ATTACKINFO_TOATE		17
#define ATTACKINFO_STRIKER2GET	18
#define ATTACKINFO_STRIKERCHAIN	19
#define ATTACKINFO_GCANCEL		20
#define ATTACKINFO_REVERSAL		21

//攻撃ID（必要があれば変更する）
#define ATTACK_WEAK				1
#define ATTACK_MIDDLE			2
#define ATTACK_STRONG			3
#define ATTACK_HISSATU			4
#define ATTACK_ATEMI			5
#define ATTACK_STRIKER			6

//チェーンコンボフラグ
//#define CHAIN_SA				0x00010000//ベースで定義
//#define CHAIN_SB				0x00020000
//#define CHAIN_SC				0x00040000
//#define CHAIN_CA				0x00080000
//#define CHAIN_CB				0x00100000
//#define CHAIN_CC				0x00200000
#define CHAIN_KIRIGET			0x00000001//追加分
#define CHAIN_SAGENA			0x00000002


//ユーザーヒット
#define USERHIT_FLAG			(0x80)
#define USERHIT_ROTFUTTOBI		(USERHIT_FLAG | 0x01)//超必用・回転吹っ飛び

//ユーザー定義メッセージ
#define GOBJMSG_NAGE			(GOBJMSG_USERDEFINE | 0x0001)	//投げ用
#define GOBJMSG_ATEMI_NAGE		(GOBJMSG_USERDEFINE | 0x0002)	//当て身投げ投げ用
#define GOBJMSG_FUTTOBI			(GOBJMSG_USERDEFINE | USERHIT_ROTFUTTOBI)//超必用・回転吹っ飛び

//オプション定義
#define OPTIONS_REVERSAL		0x00000001
#define OPTIONS_GCANCEL			0x00000002
#define OPTIONS_QUICK_CHARGE	0x00000004
#define OPTIONS_STOCKABLE_GAUGE	0x00000008
#define OPTIONS_HEAVY_HIT		0x00000010
#define OPTIONS_IRON_SKIN		0x00000020
#define OPTIONS_NO_CHAIN_COMBO	0x00000040

//間合い
#define MAAI_SHORT				(50)
#define MAAI_MIDDLE				(150)
#define MAAI_LONG				(250)
#define MAAI_NAGE				(20)

//マクロ定義
#define cell(a)					pdat->cnow=a
#define ccounter				pdat->counter
#define ALLGUARDFLAG			(GUARDINFO_XSTAND | GUARDINFO_XCROUCH | GUARDINFO_XJAMP)
#define CANNOT_GUARD(a)			((a&ALLGUARDFLAG)==0)
#define YEAH					TRUE//まかせろ

//wav 番号
#define wave_tojyo					4
#define wave_win					5
#define	wave_kiriget				6
#define	wave_sage_na				7
#define	wave_toate					8
#define	wave_atemi					9
#define	wave_atemi_nage				10
#define	wave_age_ta					11
#define	wave_storm1					12
#define	wave_storm2					13
#define	wave_striker_in				14
#define	wave_striker_atk			15
#define	wave_striker_call			16


/*-------------------------------------------------------------------
	飛道具等
---------------------------------------------------------------------*/

//■2get時の風
class C2GetWind : public CBulletBase
{
public:
	C2GetWind(CCharacterBase *parent);
	void act_run();
	void act_bomb();
};

//■なんとかストームみたいの
#define STORM_NUM		8
#define STORM_NODE_NUM	32
#define NUM_GNDSHK		32
class CStorm : public CBulletBase
{
public:
	CStorm(CCharacterBase *parent,ATTACKINFO *aif);
	BOOL Go(BOOL muki,double x,double y,double vx=0,double vy=0);
	DWORD Draw();
	DWORD Message(DWORD msg,LPVOID dat,DWORD prm);
	DWORD TouchC(ATTACKINFO *info,DWORD tc_id);

protected:
	CTristrip*	m_strips[STORM_NUM];

	float		drot[STORM_NUM];
	std::vector<DWORD> m_list;

	void UpdateOne(CTristrip& strip,float t,float rot);
	void act_run();
	void act_bomb(){}

	//あたっても消えない
	void Hit(){}
	DWORD Sousai(DWORD prm){return TRUE;}


	CFlatBoards*	m_fb;
	struct ParticleInfo
	{
		V3d pos;
		V3d ax1;
		V3d ax2;

		void Reset();
	};
	ParticleInfo m_info[NUM_GNDSHK];
};

//■遠当て
#define TOATE_NUM			64

class CToate : public CBulletBase
{
public:
	CToate(CCharacterBase *parent,ATTACKINFO *aif);
	BOOL Go(BOOL muki,double x,double y,double vx=0,double vy=0);
	DWORD Draw();
	DWORD TouchC(ATTACKINFO *info,DWORD tc_id);

protected:
	CParticle* m_particle;

	void act_run();
	void act_bomb(){}

	struct ToateInfo
	{
		V3d pos;

		void Reset();
		void Update();
	};
	ToateInfo m_info[TOATE_NUM];

	//あたっても消えない
	void Hit(){}
	DWORD Sousai(DWORD prm){return TRUE;}
	std::vector<DWORD> m_list;
};

