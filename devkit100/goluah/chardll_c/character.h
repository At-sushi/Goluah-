
/*==============================================================

	�L�����N�^�[��`�w�b�_

================================================================*/
#pragma once

#include <d3d9.h>
#include "d3dx9.h"

#include <windows.h>
#include <stdio.h>
#include <vector>

#include "gobjbase.h"
#include "cell.h"// �ʏ�Aare_editer�Ő��������w�b�_�[�t�@�C����������include���܂�

#include "aki3d.h"

//��ѓ���
class COcya;
class CJien;
class CAroe;
class CMoukoneeyo;
class CMissile;
class CHandg;
class CCraymore;
class CRifle;
class CFireThrower;
class CRocket;
class CShotgun;
class CLantern;
class CAgi;
class CComeon;
class CHttpLaser;
class CEraser99;
class CMoyamoya;
class CMoyamoyaA;
class CMoyamoyaB;
class CMoyamoyaC;
class CBlood;
class CCutin;

#define MISSILE_NUM		(4+4+4)//�~�T�C��,�C���X�^���X��
#define NEEDGAUGE_REV	(0.3)

/*--------------------------------------------------------------
	���C���L�����N�^�[�N���X
----------------------------------------------------------------*/
class CCharacter : public CCharacterBase
{
public:
	CCharacter(CDI_CHARACTERINFO2 *info);
	virtual ~CCharacter();

protected:
	//*********************************************
	//  �I�[�o�[���C�h����֐�
	//*********************************************

	DWORD Message(DWORD msg,LPVOID pd,DWORD prm);
	DWORD Action();
	void PreAction();
	DWORD TouchB(ATTACKINFO *info,BOOL hit);
	DWORD TouchC(ATTACKINFO *info,DWORD tc_eid);
	BOOL Command_OnSystem(DWORD keyinfo);
	BOOL Command_OnNormal(DWORD keyinfo);
	BOOL Command_Normal(DWORD keyinfo);
	BOOL Command_Hissatu(DWORD keyinfo);
	BOOL Command_OnAttacking(DWORD key);
	BOOL Command_OnHissatuAttacking(DWORD key);
	DWORD CommandCOM(DWORD wid);
	DWORD CmdCom_OnNormal(DWORD wid);
	DWORD CmdCom_OnHissatuAttacking(DWORD wid);
	void InitAttackInfo();
	void InitWazInfo();
	void InitBullets();

	//�ʏ퓮��
	void act_neutral();//�j���[�g����
	void act_crouch();//���Ⴊ��
	void act_rakka();//����
	void act_tojyo();//�o��
	void act_win();//����
	void act_timeoverlose();//�^�C���I�[�o�[
	void act_walkf();//�O�i
	void act_walkb();//���
	void act_jamps();//�W�����v�\������
	void act_jampc();//�����W�����v
	void act_jampf();//�O�W�����v
	void act_jampb();//��W�����v
	//�ʏ�.�U��
	void act_att_sa();//����
	void act_att_sb();//����
	void act_att_sc();//����
	void act_att_ca();//����
	void act_att_cb();//����
	void act_att_cc();//����
	void act_att_ja();//���
	void act_att_jb();//��
	void act_att_jc();//��
	//���̑�
	void act_taiki();
	void act_koutai_out();
	void act_koutai_in();
	void act_koutai();
	void act_koutai2();

	//*********************************************
	//�@CCharacter�N���X�Ǝ��̊֐�
	//*********************************************
private:
	void InitParameters();//�p�����[�^������
	void StartAttack(DWORD atk_info_id);

	void ArekoreCancel();		//�A���R�����f
	void ArekoreRelease();		//�A���R������

	//�ǉ��s���֐�
	void act_dashf();
	void act_dashb();
	void act_2danjamps();
	void act_2danjampc();
	void act_2danjampf();
	void act_2danjampb();
	void act_staff();
	void act_nage();
	void act_moyamoya();
	void act_moyamoya1();
	void act_moyamoya2();
	void act_moyamoya3();
	void act_poi_ocya();
	void act_poi_jien();
	void act_poi_aroe();
	void act_poi_missile_h();
	void act_poi_missile_v();
	void act_poi_missile_v2(UINT counter);
	void act_poi_moukoneeyo();
	void act_poi_missileSP();
	void act_poi_handg();
	void act_poi_craymore();
	void act_poi_firethrow();
	void act_poi_shot();
	void act_poi_rifle();
	void act_poi_rocket();
	void act_poi_lantern();
	void act_poi_hellfire();
	void act_poi_httplaser();
	void act_poi_eraser99();
	void act_dakko();
	void act_dakko_back();
	void act_striker_in();
	void act_striker_attack();
	void act_striker_out();
	void act_reversal();

	//�G���s��
	DWORD eact_nage(GOBJECT *pedat);

	BOOL nidanjamp;		//��i�W�����v�t���O
	float m_nage_magx;	//����������̊�g�嗦
	float m_nage_magy;

	//�U���͏��
	ATTACKINFO aif[32];

	//��ѓ���̗�
	COcya*			pbul_ocya;
	CJien*			pbul_jien;
	CAroe*			pbul_aroe;
	CMoukoneeyo*	pbul_moukoneeyo;
	CMissile*		pbul_miss[MISSILE_NUM];//x8
	CHandg*			pbul_handg;
	CCraymore*		pbul_craymore;
	CRifle*			pbul_rifle;
	CFireThrower*	pbul_firethrower;
	CRocket*		pbul_rocket;
	CShotgun*		pbul_shotgun;
	CLantern*		pbul_lantern;
	CComeon*		pbul_comeon;
	CHttpLaser*		pbul_httplaser;
	CEraser99*		pbul_eraser99;
	CMoyamoya*		pbul_moya;
	CMoyamoyaA*		pbul_moyaA;
	CMoyamoyaB*		pbul_moyaB;
	CMoyamoyaC*		pbul_moyaC;
	CBlood*			pbul_blood;
	CCutin*			pbul_cutin;

	//�_�b�R�]����ID
	DWORD m_dakko_eid;

	//�I�v�V�����t���O
	BOOL m_opt_quickA;
	BOOL m_opt_quickB;
	BOOL m_opt_quickC;
	BOOL m_opt_2nd_step;
	BOOL m_opt_reversal;
	BOOL m_opt_crazy;
};

extern LPTSTR CharDirectory;

//�s��ID�������ɒǉ�����
#define ACTID_DASHF					(ACTID_KUCYU  | 0x0001)
#define ACTID_DASHB					(ACTID_KUCYU  | 0x0002)
#define ACTID_2DANJAMPS				(ACTID_KUCYU  | 0x0003)
#define ACTID_2DANJAMPC				(ACTID_KUCYU  | 0x0004)
#define ACTID_2DANJAMPF				(ACTID_KUCYU  | 0x0005)
#define ACTID_2DANJAMPB				(ACTID_KUCYU  | 0x0006)
#define ACTID_STAFF					(ACTID_ATTACK | ACTID_KUCYU   | 0x0007)
#define ACTID_NAGE1					(ACTID_ATTACK | ACTID_NAGE    | 0x0008)
#define ACTID_POI_OCYA				(ACTID_ATTACK | ACTID_HISSATU | 0x0009)
#define ACTID_POI_JIEN				(ACTID_ATTACK | ACTID_HISSATU | 0x000A)
#define ACTID_POI_AROE				(ACTID_ATTACK | ACTID_HISSATU | 0x000B)
#define ACTID_POI_MISSILE_H			(ACTID_ATTACK | ACTID_HISSATU | 0x000C)
#define ACTID_POI_MISSILE_V			(ACTID_ATTACK | ACTID_HISSATU | 0x000D)
//#define ACTID_MOYAMOYA				(ACTID_ATTACK | ACTID_HISSATU | 0x000E)
#define ACTID_POI_MOUKONEEYO		(ACTID_ATTACK | ACTID_HISSATU | 0x000F)
#define ACTID_POI_MISSILE_SP		(ACTID_ATTACK | ACTID_HISSATU | 0x0010)
#define ACTID_POI_HANDG				(ACTID_ATTACK | ACTID_HISSATU | 0x0011)
#define ACTID_POI_CRAYMORE			(ACTID_ATTACK | ACTID_HISSATU | 0x0012)
#define ACTID_POI_FIRETHROW			(ACTID_ATTACK | ACTID_HISSATU | 0x0013)
#define ACTID_POI_SHOT				(ACTID_ATTACK | ACTID_HISSATU | 0x0014)
#define ACTID_POI_RIFLE				(ACTID_ATTACK | ACTID_HISSATU | 0x0015)
#define ACTID_POI_ROCKET			(ACTID_ATTACK | ACTID_HISSATU | 0x0016)
#define ACTID_POI_LANTERN			(ACTID_ATTACK | ACTID_HISSATU | 0x0017)
#define ACTID_POI_HELLFIRE			(ACTID_ATTACK | ACTID_HISSATU | 0x0018)
#define ACTID_POI_HTTPLASER			(ACTID_ATTACK | ACTID_HISSATU | 0x0019)
#define ACTID_POI_ERASER99			(ACTID_ATTACK | ACTID_HISSATU | 0x001A)
#define ACTID_DAKKO					(ACTID_ATTACK | ACTID_HISSATU | 0x001B)
#define ACTID_DAKKO_BACK			(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x001C)
#define ACTID_STRIKER_IN			(ACTID_INOUT  | 0x001D)
#define ACTID_STRIKER_ATTACK		(ACTID_INOUT  | 0x001E)
#define ACTID_STRIKER_OUT			(ACTID_INOUT  | 0x001F)
#define ACTID_REVERSAL				(ACTID_ATTACK | ACTID_KUCYU   | 0x0020)
#define ACTID_MOYAMOYA				(0x00000100)
#define ACTID_MOYAMOYA1				(ACTID_ATTACK | ACTID_HISSATU | ACTID_MOYAMOYA | 0x0021)
#define ACTID_MOYAMOYA2				(ACTID_ATTACK | ACTID_HISSATU | ACTID_MOYAMOYA | 0x0022)
#define ACTID_MOYAMOYA3				(ACTID_ATTACK | ACTID_HISSATU | ACTID_MOYAMOYA | 0x0023)

//�U���͔ԍ��������ɒ�`����
#define ATTACKINFO_STAND_A			0
#define ATTACKINFO_STAND_B			1
#define ATTACKINFO_STAND_C			2
#define ATTACKINFO_CROUCH_A			3
#define ATTACKINFO_CROUCH_B			4
#define ATTACKINFO_CROUCH_C			5
#define ATTACKINFO_JAMP_A			6
#define ATTACKINFO_JAMP_B			7
#define ATTACKINFO_JAMP_C			8
#define ATTACKINFO_STAFF			9
#define ATTACKINFO_NAGE				10
#define ATTACKINFO_OCYA				11
#define ATTACKINFO_JIEN				12
#define ATTACKINFO_AROE				13
#define ATTACKINFO_MISSILE			14
#define ATTACKINFO_MOUKONEEYO		15
#define ATTACKINFO_HANDG			16
#define ATTACKINFO_CRAYMORE_DUMMY	17
#define ATTACKINFO_CRAYMORE			18
#define ATTACKINFO_RIFLE			19
#define ATTACKINFO_FIRETHROWER		20
#define ATTACKINFO_ROCKET			21
#define ATTACKINFO_SHOT				22
#define ATTACKINFO_AGI				23
#define ATTACKINFO_HTTPLASER		24
#define ATTACKINFO_ERASER99			25
#define ATTACKINFO_DAKKO			26
#define ATTACKINFO_REVERSAL			27

//�U��ID�i�K�v������ΕύX����j
#define ATTACK_WEAK					1
#define ATTACK_MIDDLE				2
#define ATTACK_STRONG				3
#define ATTACK_HISSATU				4
#define ATTACK_DAKKO				5

//�`�F�[���R���{�t���O
//#define CHAIN_SA					0x00010000//�x�[�X�Œ�`
//#define CHAIN_SB					0x00020000
//#define CHAIN_SC					0x00040000
//#define CHAIN_CA					0x00080000
//#define CHAIN_CB					0x00100000
//#define CHAIN_CC					0x00200000
#define CHAIN_STAFF					0x00000001//�ǉ���

//���[�U�[��`���b�Z�[�W
#define GOBJMSG_NAGE				(GOBJMSG_USERDEFINE | 0x0001)	//�����p

//�I�v�V������`
#define OPTIONS_QUICK_LUNCH_A		0x00000001
#define OPTIONS_QUICK_LUNCH_B		0x00000002
#define OPTIONS_QUICK_LUNCH_C		0x00000004
#define OPTIONS_CRAZY_LUNCH			0x00000008
#define OPTIONS_2ND_STEP			0x00000010
#define OPTIONS_REVERSAL			0x00000020
#define OPTIONS_NO_CHAIN_COMBO		0x00000040

#define OPTIONS_QUICKS				(OPTIONS_QUICK_LUNCH_A | OPTIONS_QUICK_LUNCH_B | OPTIONS_QUICK_LUNCH_C)

//�}�N����`
#define cell(a)						pdat->cnow=a
#define ccounter					pdat->counter
#define ALLGUARDFLAG				(GUARDINFO_XSTAND | GUARDINFO_XCROUCH | GUARDINFO_XJAMP)
#define CANNOT_GUARD(a)				((a&ALLGUARDFLAG)==0)
#define SetDWAlpha(d,a)				d=(d&0x00FFFFFF)|(a<<24)
#define ROGER						TRUE//���W���[�I						


//�ԍ���
#define MAAI_SHORT				(70)
#define MAAI_MIDDLE				(140)
#define MAAI_LONG				(200)
#define MAAI_NAGE				(20)

//wave �ԍ�
#define wave_tojyo				4
#define wave_win				5
#define wave_staff				6
#define wave_are_kore			7
#define wave_poi_1a				8
#define wave_poi_1b				9
#define wave_poi_1c				10
#define wave_poi_1d				11
#define wave_poi_1e				12
#define wave_poi_2a				13
#define wave_poi_2b				14
#define wave_poi_2c				15
#define wave_poi_2d				16
#define wave_poi_3a				17
#define wave_poi_3b				18
#define wave_poi_3c				19
#define wave_poi_3d				20
#define wave_poi_3e				21
#define wave_dakko				22
#define wave_dakko_hit			23
#define wave_striker_in			24
#define wave_striker_atk		25
#define wave_striker_call		26

#define wave_poi_1f				95
#define wave_poi_2e				96
#define wave_poi_3f1			97
#define wave_poi_3f2			98
#define wave_poi_3f3			99

/*--------------------------------------------------------------
	�򓹋�
----------------------------------------------------------------*/

//������
class COcya : public CBulletBase
{
public:
	COcya(CCharacterBase* parent,ATTACKINFO& atk);
protected:
	void act_run();
	void act_bomb();
};

//���W�T�N�W�G��
class CJien : public CBulletBase
{
public:
	CJien(CCharacterBase* parent,ATTACKINFO& atk);
protected:
	void act_run();
	void act_bomb();
};

//���A���G���[�i
class CAroe : public CBulletBase
{
public:
	CAroe(CCharacterBase* parent,ATTACKINFO& atk);
protected:
	void act_run();
	void act_bomb();
};

//�����E�R�l�G��
class CMoukoneeyo : public CBulletBase
{
public:
	CMoukoneeyo(CCharacterBase* parent,ATTACKINFO& atk);
	BOOL Go(BOOL muki,double x=0,double y=0,double vx=0,double vy = 0);

protected:
	void act_run();
	void act_bomb(){}

	DWORD TouchB(ATTACKINFO *info,BOOL hit);
	DWORD Draw();

	UINT current_index;
	BOOL atk_enabled[4];
	DWORD dispcell;
};

//���~�T�C��
class CMissile : public CBulletBase
{
public:
	CMissile(CCharacterBase* parent,ATTACKINFO& atk);
protected:
	void act_run();
	void act_bomb();
};

//���n���h�O���l�[�h

#define HANDG_NUM1	16
#define HANDG_NUM2	16

class CHandg : public CBulletBase
{
public:
	CHandg(CCharacterBase* parent,ATTACKINFO& atk);
protected:
	void act_run();
	void act_bomb();
	DWORD Draw();
	void Bomb();

	CParticle* m_part1;
	CParticle* m_part2;

	struct HandgInfo1
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
	HandgInfo1 m_info1[HANDG_NUM1];

	struct HandgInfo2 : public HandgInfo1
	{
		V3d acc;
		
		void Reset();
		void Update();
	};
	HandgInfo2 m_info2[HANDG_NUM2];
};

//���N���C���A

#define CRAYMORE_STATE_ATTACK	(0x0001)
#define CRAYMORE_STATE_WAIT		(0x0002)

#define CRAYMORE_NUM			(16)
#define CRAYMORE_TIMEOUT		(500)

class CCraymore : public CBulletBase
{
public:
	CCraymore(CCharacterBase* parent,ATTACKINFO& atk);
protected:
	void  act_run();
	void  act_bomb();
	void  act_wait();
	void  act_atk();

	DWORD Action();
	DWORD TouchC( ATTACKINFO *info,DWORD tc_eid );
	DWORD Draw();

	//�������Ă������Ȃ�
	void Hit(){pdat->kougeki= FALSE;}
	DWORD Sousai(DWORD prm);

	CParticle* m_part;
	ATTACKINFO atk_dummy;

	struct ParticleInfo
	{
		V3d   pos;
		V3d   vel;
		void Reset();
		void Update();
	};
	ParticleInfo m_info[CRAYMORE_NUM];
};

//���V���b�g�K��

#define SHOTGUN_NUM			(16)

class CShotgun : public CBulletBase
{
public:
	CShotgun(CCharacterBase* parent,ATTACKINFO& atk);

protected:
	void  act_run();
	void  act_bomb();
	DWORD Draw();

	//�������Ă������Ȃ�
	void Hit(){pdat->kougeki= FALSE;}
	DWORD Sousai(DWORD prm){return TRUE;}

	CParticle* m_part;

	struct ParticleInfo
	{
		V3d   pos;
		V3d   vel;
		float ax_ratio;

		void Reset();
		void Update();
	};
	ParticleInfo m_info[SHOTGUN_NUM];
};


//�����C�t��

#define RIFLE_TIMEOUT		(500)

class CRifle : public CBulletBase
{
public:
	CRifle(CCharacterBase* parent,ATTACKINFO& atk);
	void  SetKeyIndex(int key){ key_idx=key; }
	void ForceStop();

protected:
	void  act_run();
	void  act_bomb();
	int   key_idx;
};

//���Ή����ˊ�

#define NUM_FIRETHROWER		(64)
#define DUR_FIRETHROW		(100)

class CFireThrower : public CBulletBase
{
public:
	CFireThrower(CCharacterBase* parent,ATTACKINFO& atk);
	void ForceStop();

	DWORD TouchC( ATTACKINFO *info,DWORD tc_eid );

protected:
	DWORD Draw();
	void act_run();
	void act_bomb();

	struct ParticleInfo
	{
		V3d   pos;
		V3d   vel;
		float width;
		ARGB  col;

		float t;
		BOOL created;

		void Init();
		void Reset(double posdelta);
		void Update(BOOL recreate,double posdelta);
	};

	double posdelta;
	double pos_prv;

	ParticleInfo	info[NUM_FIRETHROWER];
	CParticle*		part;

	std::vector<DWORD>	list;

	//�������Ă������Ȃ�
	void Hit(){pdat->kougeki= FALSE;}
	DWORD Sousai(DWORD prm){return TRUE;}
};

//�����P�b�g?
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

//�������^��(�A�M)

#define AGI_NUM				(64)

class CAgi : public CBulletBase
{
public:
	CAgi(CCharacterBase* parent,ATTACKINFO& atk);
public:
	void  act_run();
	void  act_bomb();
	DWORD Draw();

	CParticle* part;

	struct ParticleInfo
	{
		V3d pos;
		V3d vel;
		float time;
		float time_limit;
		float width;
		ARGB color;

		void Reset(GOBJECT *parent);
		void Update(GOBJECT *parent, BOOL recreate);
	};
	ParticleInfo info[AGI_NUM];
};

#define LANTERN_NUM_AGI			(4)//�C�j�V�����C�U�[���ύX���邱��

#define ACT_LANTERN_AGI			(0x00000001)
#define ACT_LANTERN_MAHA_AGI	(0x00000002)
#define ACT_LANTERN_APPEAR		(0x00000003)
#define ACT_LANTERN_DISAPPEAR	(0x00000004)

class CLantern : public CBulletBase
{
public:
	CLantern(CCharacterBase* parent,ATTACKINFO& atk);
	void Summon(){ChangeAction(ACT_LANTERN_APPEAR);}

protected:

	DWORD Action();

	void  act_run();
	void  act_bomb();
	void  act_agi();
	void  act_maha_agi();
	void  act_appear();
	void  act_disappear();

	CAgi*  m_agi[LANTERN_NUM_AGI];
	float aim_x;
};


//��COME ON!
class CComeon : public CBulletBase//CEffectBase
{
public:
	CComeon(CCharacterBase* parent);
protected:
	void act_run();
	void act_bomb();
};


//��http���[�U�[

class CHttpLaser : public CBulletBase
{
public:
	CHttpLaser(CCharacterBase* parent,ATTACKINFO& atk);
protected:
	void act_run();
	void act_bomb();

	DWORD TouchC( ATTACKINFO *info,DWORD tc_eid );
	std::vector<DWORD> list;

	//�������Ă������Ȃ�
	void Hit(){pdat->kougeki= FALSE;}
	DWORD Sousai(DWORD prm){return TRUE;}
};


//���C���C�T�[99

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

	//�������Ă������Ȃ�
	void Hit(){pdat->kougeki= FALSE;}
	DWORD Sousai(DWORD prm){return TRUE;}
	
	//�������q
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

	//���������i���P�b�g���p�j
	CRocket*	obj_rkt;

	//�����{��
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


//���������

enum MoyaType
{
	//A���[�g
	Moya_Ocya,			//
	Moya_Jien,			//
	Moya_Aroe,			//
	Moya_Moukoneeyo,	//
	Moya_MissileH,		//
	Moya_MissileSP,		//

	//B���[�g
	Moya_Handg,			//
	Moya_Craymore,		//
	Moya_Lantern,		//*
	Moya_FireThrow,		//
	Moya_Hellfire,		//*

	//C���[�g
	Moya_Shot,			//
	Moya_Rifle,			//
	Moya_Rocket,		//
	Moya_MissileV,		//
	Moya_HttpLaser,		//*
	Moya_Eraser99,		//*

	Moya_Invalid
};

class CMoyamoya : public CBulletBase
{
public:
	CMoyamoya(CCharacterBase* parent);

	void SetNextType(MoyaType next);
	MoyaType GetType(){ return crnt_type; }

	BOOL Go(BOOL muki,double x=0,double y=0,double vx=0,double vy = 0);
	void ForceEnd();
	void ForceStop();

	DWORD Draw();

	void Quick(){ glow_spd*=1.5f; }

protected:
	void act_run();
	void act_bomb();

	DWORD Type2Cell(MoyaType type);

	float glow;
	float glow_spd;
	float alpha_base;
	MoyaType crnt_type;
	MoyaType next_type;
	BOOL m_2ndFlag;
	BOOL m_force_flag;

	virtual MoyaType GetNextType() = 0;
	virtual MoyaType GetFirstType() = 0;
};

//A���[�g
class CMoyamoyaA : public CMoyamoya
{
public:
	CMoyamoyaA(CCharacterBase* parent):CMoyamoya(parent){crnt_type=GetFirstType();}
protected:
	MoyaType GetFirstType(){return Moya_Ocya;}
	MoyaType GetNextType();
};

//B���[�g
class CMoyamoyaB : public CMoyamoya
{
public:
	CMoyamoyaB(CCharacterBase* parent):CMoyamoya(parent){crnt_type=GetFirstType();}
protected:
	MoyaType GetFirstType(){return Moya_Handg;}
	MoyaType GetNextType();
};

//C���[�g
class CMoyamoyaC : public CMoyamoya
{
public:
	CMoyamoyaC(CCharacterBase* parent):CMoyamoya(parent){crnt_type=GetFirstType();}
protected:
	MoyaType GetFirstType(){return Moya_Shot;}
	MoyaType GetNextType();
};


//���_�b�R��
class CBlood : public CBulletBase
{
public:
	CBlood(CCharacterBase* parent);
protected:
	void act_run();
	void act_bomb();

	DWORD DrawBack();
};

//���_�b�R�E�J�b�g�C��
class CCutin : public CBulletBase
{
public:
	CCutin(CCharacterBase* parent){}
protected:
	void act_run(){Hide();}
	void act_bomb(){Hide();}

	DWORD DrawBack(){return 0;}
};

