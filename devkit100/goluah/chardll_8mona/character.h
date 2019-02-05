
/*==============================================================

	キャラクター定義ヘッダ

================================================================*/
#pragma once

#include <windows.h>
#include <stdio.h>

#include "gobjbase.h"
#include "cell.h"// 通常、are_editerで生成したヘッダーファイルをここにincludeします

class CHeart;
class CDashWind;

#define NEEDGAUGE_GC	(0.3f)
#define NEEDGAUGE_SA	(0.1f)

#define AIF_NUM			(32)

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
	DWORD TouchA(ATTACKINFO *info,DWORD ta_eid);
	DWORD TouchB(ATTACKINFO *info,BOOL hit);
	DWORD TouchC(ATTACKINFO *info,DWORD tc_eid);
	BOOL Command_OnSystem(DWORD key);
	BOOL Command_OnNormal(DWORD keyinfo);
	BOOL Command_Normal(DWORD keyinfo);
	BOOL Command_Hissatu(DWORD keyinfo);
	BOOL Command_OnAttacking(DWORD key);
	DWORD CommandCOM(DWORD wid);
	DWORD CmdCom_OnSystem(DWORD wid);
	DWORD CmdCom_OnHissatuAttacking(DWORD wid);
	DWORD CmdCom_OnNormal(DWORD wid);
	void InitAttackInfo();//攻撃力情報の初期化
	void InitWazInfo();//技情報の初期化
	void InitBullets();//飛び道具の初期化

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
	void act_strikercomeon();//いらっしゃーい
	void act_timeoverlose();//がっかり！
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

	//*********************************************
	//　CCharacterクラス独自の関数
	//*********************************************
private:
	void InitParameters();//パラメータ初期化
	void StartAttack(DWORD atk_info_id);
	void EndAttack(){pdat->kougeki=FALSE;}
	BOOL ComLevelCk(UINT level);

	//追加行動関数
	void act_nage();
	DWORD act_nagee(GOBJECT *pedat);
	void act_sah1();
	void act_sah2();
	void act_sah3();
	void act_sahs();
	void act_fig1();
	void act_fig2();
	void act_fig3();
	void act_gcancel1();
	void act_gcancel2();
	void act_gcancel3();
	void act_dashf();
	void act_dashb();
	void act_ikari();
	DWORD act_ikarie(GOBJECT *pedat);
	void act_escape();
	void act_striker1();
	void act_striker2();
	void act_striker3();
	void act_striker4();
	void act_bat();
	void act_upper_step();
	void act_upper_jamp();
	void act_upper_landing();
	void act_hand();
	void act_hkick();
	void act_sah_landing();

	//攻撃力情報
	ATTACKINFO aif[AIF_NUM];
	
	//飛び道具類
	CClassicalBullet *clabul_nak1;
	CClassicalBullet *clabul_nak2;
//	CClassicalBullet *clabul_wind;
	CClassicalBullet *clabul_fola[3];
	CClassicalBullet *clabul_folb[3];
	CClassicalBullet *clabul_folc[3];
	CClassicalBullet *clabul_fold[3];
	CClassicalBullet *clabul_fole[3];
	CClassicalBullet *clabul_folx[6];
	void GoHeart();
	CHeart *bul_heart;
	CDashWind* clabul_wind;
	BULLETINFO_A fola[3],folb[3],folc[3],fold[3],fole[3];
	BOOL EscapeOK();

	//オプション類
	BOOL opt_gcancel;
	BOOL opt_exattack;
	BOOL opt_ikari;
	BOOL opt_super_armer;
	BOOL opt_exfriend;

	//さぁ
	BOOL sah_sw;
};

extern LPTSTR CharDirectory;

//行動IDをここに追加する

#define ACTID_NAGE1				(ACTID_ATTACK | ACTID_NAGE    | 0x0001)
#define ACTID_SAH1				(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0002)
#define ACTID_SAH2				(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0003)
#define ACTID_SAH3				(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0004)
#define ACTID_SAHS				(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0005)
#define ACTID_FIG1				(ACTID_ATTACK | ACTID_HISSATU | 0x0006)
#define ACTID_FIG2				(ACTID_ATTACK | ACTID_HISSATU | 0x0007)
#define ACTID_FIG3				(ACTID_ATTACK | ACTID_HISSATU | 0x0008)
#define ACTID_GCANCEL1			(ACTID_ATTACK | ACTID_HISSATU | 0x0009)
#define ACTID_GCANCEL2			(ACTID_ATTACK | ACTID_HISSATU | 0x000A)
#define ACTID_GCANCEL3			(ACTID_ATTACK | ACTID_HISSATU | 0x000B)
#define ACTID_DASHF				(0x000C)
#define ACTID_DASHB				(0x000D)
#define ACTID_IKARI				(ACTID_ATTACK | ACTID_HISSATU | ACTID_NAGE  | 0x000E)
#define ACTID_ESCAPE			(ACTID_ATTACK | ACTID_HISSATU | 0x000F)
#define ACTID_STRIKER1			(ACTID_INOUT  | 0x0010)//ストライカー攻撃
#define ACTID_STRIKER2			(ACTID_INOUT  | 0x0011)
#define ACTID_STRIKER3			(ACTID_INOUT  | 0x0012)
#define ACTID_STRIKER4			(ACTID_INOUT  | 0x0013)//すごすごと帰っていく
#define ACTID_BAT				(ACTID_ATTACK | 0x0014)
#define ACTID_UPPER_STEP		(ACTID_ATTACK | 0x0015)
#define ACTID_UPPER_JAMP		(ACTID_ATTACK | ACTID_KUCYU   | 0x0016)
#define ACTID_UPPER_LANDING		(ACTID_ATTACK | 0x0017)
#define ACTID_HAND				(ACTID_ATTACK | 0x0018)
#define ACTID_HKICK				(ACTID_ATTACK | 0x0019)
#define ACTID_SAH_LANDING		(ACTID_ATTACK | ACTID_HISSATU | 0x0020)

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
#define ATTACKINFO_STRIKER		9
#define ATTACKINFO_NAGE			10
#define ATTACKINFO_SAH1A		11
#define ATTACKINFO_SAH1B		12
#define ATTACKINFO_SAH1C		13
#define ATTACKINFO_SAH2A		14
#define ATTACKINFO_SAH2B		15
#define ATTACKINFO_SAH2C		16
#define ATTACKINFO_FIG			17
#define ATTACKINFO_IKARI1		18
#define ATTACKINFO_IKARI2		19
#define ATTACKINFO_ACCHI		20
#define ATTACKINFO_BAT			21
#define ATTACKINFO_UPPER		22
#define ATTACKINFO_HAND			23
#define ATTACKINFO_HKICK1		24
#define ATTACKINFO_HKICK2		25
#define ATTACKINFO_GCANCEL1		26
#define ATTACKINFO_GCANCEL2		27
#define ATTACKINFO_GCANCEL3		28

//攻撃ID（必要があれば変更する）
#define ATTACK_WEAK				0x00000001
#define ATTACK_MIDDLE			0x00000002
#define ATTACK_STRONG			0x00000003
#define ATTACK_HISSATU			0x00000004
#define ATTACK_FRIENDS			0x00000005

#define ATTACK_HKICK1			0x00010000
#define ATTACK_HKICK2			0x00020000
#define ATTACK_UPPER			0x00030000
#define ATTACK_SAH1				0x00040000
#define ATTACK_SAH2				0x00050000
#define ATTACK_BAT				0x00060000

//追加チェーンID
#define CHAIN_HAND				(0x00000001)
#define CHAIN_UPPER				(0x00000002)
#define CHAIN_BAT				(0x00000004)
#define CHAIN_HKICK				(0x00000008)

//ユーザー定義メッセージ
#define GOBJMSG_NAGE			(GOBJMSG_USERDEFINE | 0x0001)	//投げ用
#define GOBJMSG_IKARINAGE		(GOBJMSG_USERDEFINE | 0x0002)	//投げ用

//オプション定義
#define OPTIONS_GUARD_CANCEL	0x00000001
#define OPTIONS_EXTRA_ATTACK	0x00000002
#define OPTIONS_IKARI			0x00000004
#define OPTIONS_SUPER_ARMER		0x00000008
#define OPTIONS_EXTRA_FRIENDS	0x00000010

#define YEAH					TRUE

//間合い
#define MAAI_SHORT				(70)
#define MAAI_MIDDLE				(140)
#define MAAI_LONG				(200)
#define MAAI_NAGE				(20)
#define MAAI_HNAGE				(40)

//wave番号
#define wave_tojyo				4
#define wave_win				5
#define wave_nage				6
#define wave_sah				7
#define wave_fig				8
#define wave_ikari1				9
#define wave_ikari2				10
#define wave_escape				11
#define wave_striker			12
#define wave_break				13


//マクロ定義
#define cell(a)					pdat->cnow=a
#define ccounter				pdat->counter
#define ALLGUARDFLAG			(GUARDINFO_XSTAND | GUARDINFO_XCROUCH | GUARDINFO_XJAMP)
#define CANNOT_GUARD(a)			((a&ALLGUARDFLAG)==0)
#define SetDWAlpha(d,a)			d=(d&0x00FFFFFF)|(a<<24)

/*-------------------------------------------------------------------
	飛道具等
---------------------------------------------------------------------*/

//■ダッシュ時の風
class CDashWind : public CBulletBase
{
public:
	CDashWind(CCharacter *parent);
	void act_run();
	void act_bomb();
};

//■ハートマーク
class CHeart : public CBulletBase
{
public:
	CHeart(CCharacter *parent);
	void act_run();
	void act_bomb();
};
inline void CCharacter::GoHeart(){if(bul_heart)bul_heart->Go(pdat->muki,pdat->x,0,0,0);}