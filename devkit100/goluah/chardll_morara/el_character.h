
/*==============================================================

	キャラクター定義ヘッダ

================================================================*/
#pragma once

#include <list>
#include <windows.h>
#include <stdio.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "gobjbase.h"
#include "el_cell.h"
#include "aki3d.h"

#include "opt.h"

//飛び道具クラス
class CHadou;
class CEnergyArrow;
namespace Morarael
{
class CTrack;//ノーマルモララーとかぶったので
}
class CJudge;
class CDashWind;

//定数定義
#define EARROW_NUM		(4)
#define SOJ_OFFSET		(10)
#define NUM_ATTACKINFO	(32)

#define NEEDGAUGE_TETRA	(0.2)
#define NEEDGAUGE_SOJ	(0.1)
#define NEEDGAUGE_DOJ	(1.0)
#define NEEDGAUGE_MEGID	(1.0)

#define JUDGE_MAXHIT	(3)

/*--------------------------------------------------------------
	メインキャラクタークラス
----------------------------------------------------------------*/
class CCharacterEL : public CCharacterBase
{
public:
	CCharacterEL(CDI_CHARACTERINFO2 *info);
	virtual ~CCharacterEL();

protected:
	//*********************************************
	//  オーバーライドする関数
	//*********************************************

	DWORD Message(DWORD msg,LPVOID pd,DWORD prm);
	DWORD Action();
	void PreAction();
	BOOL Command_OnHissatuAttacking(DWORD key);
	BOOL Command_OnNormal(DWORD keyinfo);
	BOOL Command_Normal(DWORD keyinfo);
	BOOL Command_Hissatu(DWORD keyinfo);
	DWORD CommandCOM(DWORD wid);
	DWORD CmdCom_OnHissatuAttacking(DWORD wid);
	DWORD CmdCom_OnKucyu(DWORD wid);
	DWORD CmdCom_OnNormal(DWORD wid);
	void InitAttackInfo();//攻撃力情報の初期化
	void InitWazInfo();//技情報の初期化
	void InitBullets();//飛道具初期化
	void InitGCDandBMP();//ビットマップロード
	void InitMySound(char* path_name,char* list_name);
	DWORD TouchB(ATTACKINFO *info,BOOL hit);
	DWORD TouchA(ATTACKINFO *info,DWORD ta_eid);
	DWORD ComThink();

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
	void act_timeoverlose();//タイムオーバー
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
	void InitAnalyzeOptions();//オプション分割
	void StartAttack(DWORD atk_info_id);
	BOOL com_nage2(DWORD maai);
	BOOL ComLevelCk(UINT level);

	//追加行動関数
	void act_earrow1();	//エナジーアロー
	void act_earrow2();
	void act_earrow3();
	void act_earrow1j();//エナジーアロー(空中)
	void act_earrow2j();
	void act_earrow3j();
	void act_dashf();	//ダッシュ
	void act_dashb();
	void act_dashfj();	//空中ダッシュ
	void act_dashbj();
	void act_sword1r();	//ソード、具現化
	void act_sword2r();
	void act_sword3r();
	BOOL act_swordRealize();
	void act_sword1a();	//ソード、攻撃
	void act_sword2a();
	void act_sword3a();
	void act_sword1d();	//ソード、消失
	void act_sword2d();
	void act_sword3d();
	void act_nage1();	//投げ
	void act_nage2();
	void act_nageend();
	void act_judgement();//ジャッジメント
	void act_att_scs1();
	void act_att_scs2();

	//敵側行動
	DWORD eact_nage(GOBJECT *pedat);

	//攻撃力情報
	ATTACKINFO aif[NUM_ATTACKINFO];

	//飛道具
	CHadou				*pbul_hadou;
	CEnergyArrow		*pbul_earrow[EARROW_NUM];
	Morarael::CTrack	*pefe_track;
	CJudge				*pbul_judge;
	CDashWind			*pefe_wind;

	//オプション
	BOOL m_opt_tetra;
	BOOL m_opt_sword;
	BOOL m_opt_judge;
	BOOL m_opt_megido;
	BOOL m_opt_regene;
	int  m_regene_rate;
	float m_power_rate;
	BOOL m_opt_attackPlus;
	BOOL m_opt_hp50;
	BOOL m_event_sw;
	BOOL m_1st_update;
};

extern LPTSTR CharDirectory;

//行動IDをここに追加する
#define ACTID_EARROW1			(ACTID_HISSATU | ACTID_ATTACK | 0x0001)//テトラグラマトン（旧名エナジーアロー）
#define ACTID_EARROW2			(ACTID_HISSATU | ACTID_ATTACK | 0x0002)
#define ACTID_EARROW3			(ACTID_HISSATU | ACTID_ATTACK | 0x0003)
#define ACTID_DASHF				(ACTID_KUCYU | 0x0004)//ダッシュ（地上から）
#define ACTID_DASHB				(ACTID_KUCYU | 0x0005)
#define ACTID_EARROW1J			(ACTID_HISSATU | ACTID_ATTACK | ACTID_KUCYU | 0x0006)//空中テトラグラマトン（旧名エナジーアロー）
#define ACTID_EARROW2J			(ACTID_HISSATU | ACTID_ATTACK | ACTID_KUCYU | 0x0007)
#define ACTID_EARROW3J			(ACTID_HISSATU | ACTID_ATTACK | ACTID_KUCYU | 0x0008)
#define ACTID_DASHFJ			(ACTID_KUCYU | 0x0009)//ダッシュ（空中から）
#define ACTID_DASHBJ			(ACTID_KUCYU | 0x000A)
#define ACTID_SWORD1R			(ACTID_HISSATU | ACTID_ATTACK | 0x000B)//ソード(仮)、具現化
#define ACTID_SWORD2R			(ACTID_HISSATU | ACTID_ATTACK | 0x000C)
#define ACTID_SWORD3R			(ACTID_HISSATU | ACTID_ATTACK | 0x000D)
#define ACTID_SWORD1A			(ACTID_HISSATU | ACTID_ATTACK | 0x000E)//ソード(仮)、攻撃
#define ACTID_SWORD2A			(ACTID_HISSATU | ACTID_ATTACK | 0x000F)
#define ACTID_SWORD3A			(ACTID_HISSATU | ACTID_ATTACK | 0x0010)
#define ACTID_SWORD1D			(ACTID_HISSATU | ACTID_ATTACK | 0x0011)//ソード(仮)、消失
#define ACTID_SWORD2D			(ACTID_HISSATU | ACTID_ATTACK | 0x0012)
#define ACTID_SWORD3D			(ACTID_HISSATU | ACTID_ATTACK | 0x0013)
#define ACTID_NAGE1				(ACTID_ATTACK | ACTID_NAGE | 0x0014)				//投げ
#define ACTID_NAGE2				(ACTID_ATTACK | ACTID_NAGE | ACTID_KUCYU | 0x0015)
#define ACTID_NAGEEND			(ACTID_ATTACK | ACTID_NAGE | ACTID_KUCYU | 0x0016)
#define ACTID_JUDGEMENT			(ACTID_HISSATU | ACTID_ATTACK | ACTID_KUCYU | 0x0017)//デイ・オブ・ジャッジメント
#define ACTID_ATT_SCS1			(ACTID_ATTACK | 0x0018)
#define ACTID_ATT_SCS2			(ACTID_ATTACK | 0x0019)

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
#define ATTACKINFO_EARROW		9
#define ATTACKINFO_SWORD1		10
#define ATTACKINFO_SWORD2		11
#define ATTACKINFO_SWORD3		12
#define ATTACKINFO_NAGE			13
#define ATTACKINFO_JUDGE		14


//攻撃ID（必要があれば変更する）
#define ATTACK_WEAK				1
#define ATTACK_MIDDLE			2
#define ATTACK_STRONG			3
#define ATTACK_HISSATU			4
#define ATTACK_EREAL			0x00010000

//ユーザー定義メッセージ
#define GOBJMSG_NAGE			(GOBJMSG_USERDEFINE | 0x0001)	//投げ用
#define GOBJMSG_EARROWHIT		(GOBJMSG_USERDEFINE | 0x0002)	// コンボ用

//マクロ定義
#define cell(a)				pdat->cnow=a
#define ccounter			(pdat->counter)
#define ALLGUARDFLAG		(GUARDINFO_XSTAND | GUARDINFO_XCROUCH | GUARDINFO_XJAMP)
#define CANNOT_GUARD(a)		((a&ALLGUARDFLAG)==0)

//チェーンコンボフラグ
//#define CHAIN_SA	0x00010000 ベースで定義
//#define CHAIN_SB	0x00020000
//#define CHAIN_SC	0x00040000
//#define CHAIN_CA	0x00080000
//#define CHAIN_CB	0x00100000
//#define CHAIN_CC	0x00200000
#define CHAIN_SWORD1	0x00000001//チェーンとしてカウントする
#define CHAIN_SWORD2	0x00000002
#define CHAIN_SWORD3	0x00000004

//間合い
#define MAAI_NAGE		(30)

//wave番号
#define	wave_tojyo			4
#define	wave_win			5
#define	wave_tetra			6
#define wave_sword			7
#define	wave_sword1			8
#define	wave_sword2			9
#define	wave_sword3			10
#define	wave_judge			11


//--------------------------------------------------------------------------------
//	飛道具クラス定義
//----------------------------------------------------------------------------------

class CHadou : public CBulletBase
{
public:
	CHadou(CCharacterBase *parent);
	void GoA(BOOL muki,double x,double y,float duration,ATTACKINFO *atk);
	
protected:
	void act_run();
	void act_bomb();
	virtual void act_base();
	void Hit();

	DWORD m_hit_count;
	float m_duration;
};

class CEnergyArrow : public CBulletBase
{
public:
	CEnergyArrow(CCharacterBase *parent);
	~CEnergyArrow(){Destroy();}

	DWORD Draw();
	BOOL Go(BOOL muki,double x,double y,double vx,double vy);

	void SetAttackInfo( ATTACKINFO *patk ){atkinfo=*patk;}
	
protected:
	void act_run();
	void act_bomb();
	void act_base();

	void Create();
	void Destroy();

	void EnemyTrack(float strength);
	void UpdateBez();
	void UpdateTrack(float t,BOOL damp);

	
	CTristrip* m_ts;			//トライストリップ
	UINT m_num;				//総ポイント数
	UINT m_step;			//1フレで更新するポイント数
	V2d m_ppos;				//前回位置
	V2d m_pvec;				//前回ニセ速度

	V2d *m_pos;				//位置バッファ
	float m_width;			//基準太さ
	ARGB m_color;			//基準色

	float m_maxspeed;		//最大速度
	float m_trackstrength;	//追尾強さ
	float m_lifetime;		//寿命
};

/*--------------------------------------------------------------
	軌跡
----------------------------------------------------------------*/
namespace Morarael
{

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


}//namespace Morarael

/*--------------------------------------------------------------
	ジャッジメント
----------------------------------------------------------------*/
#define JUDGE_TOGENUM	256

class CJudge : public CBulletBase
{
public:
	CJudge(CCharacterBase *parent);
	~CJudge(){Destroy();}

	DWORD TouchC(ATTACKINFO *info,DWORD tc_eid);
	DWORD Draw();
	BOOL Go(BOOL muki,double x,double y=0,double vx=50,double vy=0);

protected:

	void Create();
	void Destroy();

	void Hit();
	DWORD Sousai(DWORD prm);

	void act_run();
	void act_bomb();

	void UpdateToge();

	CParticle* m_tubu;			//パーティクルクラス
	LPDIRECT3DTEXTURE9 m_tex;	//テクスチャ

	//パラメータ
	float m_width;
	float m_widthorg;

	//開始点
	float m_start_x;
	float m_start_y;

	//当たったやつリスト
	struct JudgeHitInfo
	{
		DWORD oid;
		DWORD cnt;
	};
	typedef std::list<JudgeHitInfo> JudgeHitList;
	JudgeHitList m_hitlist;
	JudgeHitList::iterator FindHitInfo(DWORD oid);

	//とげとげパラメータ
	struct TogeParam
	{
		float time;
		float time_end;
		V3d	dst;
		V3d base;
		void Reset(float x,float z,BOOL muki);
	};
	TogeParam m_toge[JUDGE_TOGENUM];
};

//■ダッシュ時の風
class CDashWind : public CBulletBase
{
public:
	CDashWind(CCharacterBase *parent);
	void act_run();
	void act_bomb();
};


