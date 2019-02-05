
/*==============================================================

	�L�����N�^�[��`�w�b�_

================================================================*/
#pragma once

#include <windows.h>
#include <stdio.h>

#include "aki3d.h"
#include "gobjbase.h"
#include "cell.h"// �ʏ�Aare_editer�Ő��������w�b�_�[�t�@�C����������include���܂�

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

// �G�t�F�N�g�p�I�u�W�F�N�g�쐬�}�N��
#define CREATE_GOBJ_FX(obj)			{ CGoluahObject* pointer = new obj; if ( pointer && !pointer->GetGObject() )\
										{ delete pointer; }\
									}

/*--------------------------------------------------------------
	���C���L�����N�^�[�N���X
----------------------------------------------------------------*/

#define MAXNUM_ATTACKINFO	(34)

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

	//�ʏ퓮��
	void act_neutral();	//�j���[�g����
	void act_crouch();	//���Ⴊ��
	void act_rakka();	//����
	void act_tojyo();	//�o��
	void act_win();		//����
	void act_walkf();	//�O�i
	void act_walkb();	//���
	void act_jamps();	//�W�����v�\������
	void act_jampc();	//�����W�����v
	void act_jampf();	//�O�W�����v
	void act_jampb();	//��W�����v
	void act_strikercomeon();//�J��!
	void act_timeoverlose();//�^�C���I�[�o�[
	//�ʏ�.�U��
	void act_att_sa();	//����
	void act_att_sb();	//����
	void act_att_sc();	//����
	void act_att_ca();	//����
	void act_att_cb();	//����
	void act_att_cc();	//����
	void act_att_ja();	//���
	void act_att_jb();	//��
	void act_att_jc();	//��
	//���̑�
	void act_taiki();
	void act_koutai_out();
	void act_koutai_in();
	void act_koutai();
	void act_koutai2();

	//�������֌W
	void InitAnalyzeOptions();	//�I�v�V�����t���O����
	void InitParameters();		//�p�����[�^������
	void InitAttackInfo();		//�U���͏��̏�����
	void InitWazInfo();			//�Z���̏�����
	void InitBullets();			//�򓹋����

	//*********************************************
	//�@CCharacter�N���X�Ǝ��̊֐�
	//*********************************************
private:
	void StartAttack(DWORD atk_info_id);
	void EndAttack();
	BOOL EnemyAction(DWORD eid,DWORD hid);
	BOOL ComLevelCk(UINT level);

	//�ǉ��s���֐�
	void act_att_sd();		//����
	void act_att_se();		//����
	void act_tatumaki1(DWORD atkid);	//����(��)
	void act_tatumaki2(DWORD atkid);	//����(��)
	void act_tatumaki3(DWORD atkid);	//����(��)
	void act_tatumakir();	//��������
	void act_landing();		//�������n
	void act_reiku1a();		//���C�N(��)
	void act_reiku1b();
	void act_reiku1c();
	void act_reiku2a();		//���C�N(��)
	void act_reiku2b();
	void act_reiku2c();
	void act_reiku3a();		//���C�N(��)
	void act_reiku3b();
	void act_reiku3c();
	void act_reiku3cs();	//���C�N���n�㌄
	void act_dashf();		//�O���_�b�V��
	void act_dashb();		//����_�b�V��
	void act_nage();		//����
	void act_hadou1a();		//�g�����i��΂Ȃ��n�j
	void act_hadou2a();
	void act_hadou3a();
	void act_hadou1b();		//�g�����i��Ԍn�j
	void act_hadou2b();
	void act_hadou3b();
	void act_gcancel1();	//�K�[�h�L�����Z��
	void act_gcancel2();
	void act_gcancel3();
	void act_gcancel1j();
	void act_gcancel2j();
	void act_gcancel3j();
	void act_2nd_step();	//2�i�W�����v
	void act_ajamp();		//�G���A���n���W�����v
	void act_goodbye1();	//�O�b�o�C�E�t�H�[�G�o�[
	void act_goodbye2();	//�O�b�o�C�E�t�H�[�G�o�[
	void act_striker_in();
	void act_striker_dash();
	void act_striker_attack();
	void act_striker_out();

	//�G���s��
	DWORD eact_nage(GOBJECT* pedat);	//����
	void eact_aerialpop(GOBJECT* pedat);//�G���A���E�������q�b�g
	void eact_aerialhit1(GOBJECT* pedat);//�G���A���E�󒆃q�b�g
	void eact_aerialhit2(GOBJECT* pedat);//�G���A���E�󒆃q�b�g
	void eact_aerialhit3(GOBJECT* pedat);//�G���A���E�󒆃q�b�g

	//�U���͏��
	ATTACKINFO aif[MAXNUM_ATTACKINFO];


	//�򓹋�
	CReiku_Re*		pefe_re;	//���C�N�G�t�F�N�g�u���v
	CReiku_I*		pefe_i;		//���C�N�G�t�F�N�g�u�C�v
	CReiku_Ku*		pefe_ku;	//���C�N�G�t�F�N�g�u�N�v
	CDashWind*		pefe_dash;	//�_�b�V�������G�t�F�N�g
	CHadouKen*		pbul_hadou;	//�g����
	CGoodbye*		pbul_goodbye;//�O�b�o�CF
//	CRocket*		pbul_rocket;
//	CEraser99*		pbul_eraser99;

	//�I�v�V����
	BOOL			m_opt_hpp30;	//HP+30%
	BOOL			m_opt_exAttack;	//�ǉ����͋Z
	BOOL			m_opt_hadou;	//�g����
	BOOL			m_opt_AAttack;	//�G���A��
	BOOL			m_opt_AStep;	//2�i�W�����v
	BOOL			m_opt_gcancel;	//�K�[�h�L�����Z��

	//����t���O
	BOOL			m_2nd_step_flag;//2�i�W�����v�\�t���O
	BOOL			m_aerial;		//�G���A�����t���O
};

extern LPTSTR CharDirectory;

//�s��ID�������ɒǉ�����
#define ACTID_ATT_SD			(ACTID_ATTACK  | 0x0001)								//�����A���o�[�O����
#define ACTID_ATT_SE			(ACTID_ATTACK  | 0x0002)								//�����A���o�[�O����
#define ACTID_TATUMAKI1			(ACTID_ATTACK  | ACTID_HISSATU | ACTID_KUCYU | 0x0003)	//�����i��j
#define ACTID_TATUMAKI2			(ACTID_ATTACK  | ACTID_HISSATU | ACTID_KUCYU | 0x0004)	//�����i���j
#define ACTID_TATUMAKI3			(ACTID_ATTACK  | ACTID_HISSATU | ACTID_KUCYU | 0x0005)	//�����i���j
#define ACTID_TATUMAKIR			(ACTID_HISSATU | ACTID_KUCYU   | 0x0006)				//�����i�����j
#define ACTID_LANDING			(ACTID_HISSATU | 0x0007)								//�����i���n�j
#define ACTID_REIKU1A			(ACTID_ATTACK  | ACTID_HISSATU | 0x0008)				//���C�N(��)
#define ACTID_REIKU1B			(ACTID_ATTACK  | ACTID_HISSATU | 0x0009)
#define ACTID_REIKU1C			(ACTID_ATTACK  | ACTID_HISSATU | 0x000A)
#define ACTID_REIKU2A			(ACTID_ATTACK  | ACTID_HISSATU | 0x000B)				//���C�N(��)
#define ACTID_REIKU2B			(ACTID_ATTACK  | ACTID_HISSATU | 0x000C)
#define ACTID_REIKU2C			(ACTID_ATTACK  | ACTID_HISSATU | 0x000D)
#define ACTID_REIKU3A			(ACTID_ATTACK  | ACTID_HISSATU | 0x000E)				//���C�N(��)
#define ACTID_REIKU3B			(ACTID_ATTACK  | ACTID_HISSATU | 0x000F)
#define ACTID_REIKU3C			(ACTID_ATTACK  | ACTID_HISSATU | ACTID_KUCYU | 0x0010)
#define ACTID_REIKU3CS			(ACTID_HISSATU | 0x0011)								//���C�N���n�㌄
//#define ACTID_REIKUCOM			(ACTID_ATTACK  | ACTID_HISSATU | 0x0012)				//���C�N(COM)
#define ACTID_DASHF				(0x0013)												//�O�_�b�V��
#define ACTID_DASHB				(0x0014)												//��_�b�V��
#define ACTID_NAGE1				(ACTID_ATTACK  | ACTID_NAGE    | 0x0015)				//����
#define ACTID_HADOU1A			(ACTID_ATTACK  | ACTID_HISSATU | 0x0016)				//�g����A(��)
#define ACTID_HADOU2A			(ACTID_ATTACK  | ACTID_HISSATU | 0x0017)				//�g����A(�~)
#define ACTID_HADOU3A			(ACTID_ATTACK  | ACTID_HISSATU | 0x0018)				//�g����A(��)
#define ACTID_HADOU1B			(ACTID_ATTACK  | ACTID_HISSATU | 0x0019)				//�g����B(��)
#define ACTID_HADOU2B			(ACTID_ATTACK  | ACTID_HISSATU | 0x001A)				//�g����B(�~)
#define ACTID_HADOU3B			(ACTID_ATTACK  | ACTID_HISSATU | 0x001B)				//�g����B(��)
#define ACTID_GCANCEL1			(ACTID_ATTACK  | ACTID_HISSATU | 0x001C)				//�K�[�L����(��)
#define ACTID_GCANCEL2			(ACTID_ATTACK  | ACTID_HISSATU | 0x001D)				//�K�[�L����(��)
#define ACTID_GCANCEL3			(ACTID_ATTACK  | ACTID_HISSATU | 0x001E)				//�K�[�L����(��)
#define ACTID_GOODBYE1			(ACTID_ATTACK  | ACTID_HISSATU | 0x001F)				//�O�b�o�C�E�t�H�[�G�o�[(�n�����O�i)
#define ACTID_2ND_STEP			(ACTID_KUCYU   | 0x0020)								//2�i�W�����v����
#define ACTID_AJAMP				(ACTID_KUCYU   | 0x0021)								//�G���A���n���W�����v
#define ACTID_GOODBYE2			(ACTID_ATTACK  | ACTID_HISSATU | 0x0022)				//�O�b�o�C�E�t�H�[�G�o�[(�q�b�g���ł��グ)
#define ACTID_STRIKER_IN		(ACTID_INOUT   | 0x0023)								//�x���U���E��ʓ��ɓ����Ă���
#define ACTID_STRIKER_DASH		(ACTID_INOUT   | 0x0024)								//�x���U���E����̂ق��ɂ�����Ɛi��
#define ACTID_STRIKER_ATTACK	(ACTID_INOUT   | 0x0025)								//�x���U���E�U��
#define ACTID_STRIKER_OUT		(ACTID_INOUT   | 0x0026)								//�x���U���E���n�A���A��͂�����
#define ACTID_GCANCEL1J			(ACTID_ATTACK  | ACTID_HISSATU | ACTID_KUCYU | 0x0027)	//�K�[�L����(��) - ������s��
#define ACTID_GCANCEL2J			(ACTID_ATTACK  | ACTID_HISSATU | ACTID_KUCYU | 0x0028)	//�K�[�L����(��) - ������s��
#define ACTID_GCANCEL3J			(ACTID_ATTACK  | ACTID_HISSATU | ACTID_KUCYU | 0x0029)	//�K�[�L����(��) - ������s��

//�U���͔ԍ��������ɒ�`����
#define ATTACKINFO_STAND_A		0	//�ʏ�Z�F�X
#define ATTACKINFO_STAND_B		1
#define ATTACKINFO_STAND_C		2
#define ATTACKINFO_CROUCH_A		3
#define ATTACKINFO_CROUCH_B		4
#define ATTACKINFO_CROUCH_C		5
#define ATTACKINFO_JAMP_A		6
#define ATTACKINFO_JAMP_B		7
#define ATTACKINFO_JAMP_C		8
#define ATTACKINFO_STAND_D		9	//����
#define ATTACKINFO_STAND_E		10	//����
#define ATTACKINFO_TATUMAKI1	11	//����(��)
#define ATTACKINFO_TATUMAKI2	12	//����(��)
#define ATTACKINFO_TATUMAKI3	13	//����(��)
#define ATTACKINFO_REIKU		14	//���C�N(�ʏ�)
#define ATTACKINFO_REIKU_FINISH	15	//���C�N(�t�B�j�b�V��)
#define ATTACKINFO_NAGE			16	//����
#define ATTACKINFO_HADOU1A		17	//�g�����i��΂Ȃ��n�j
#define ATTACKINFO_HADOU2A		18
#define ATTACKINFO_HADOU3A		19
#define ATTACKINFO_HADOU1B		20	//�g�����i��Ԍn�j
#define ATTACKINFO_HADOU2B		21
#define ATTACKINFO_HADOU3B		22
#define ATTACKINFO_GOODBYE1		23 //�O�b�o�C�E�t�H�[�G�o�[
#define ATTACKINFO_GOODBYE2		24
#define ATTACKINFO_AERIALPOP	25 //�G���A���֌W
#define ATTACKINFO_AERIAL_A		26
#define ATTACKINFO_AERIAL_B		27
#define ATTACKINFO_AERIAL_C		28
#define ATTACKINFO_STRIKER_DASH	29
#define ATTACKINFO_STRIKER		30
#define ATTACKINFO_GCANCEL		31
#define ATTACKINFO_ROCKET		32
#define ATTACKINFO_ERASER99		33//�����ς������ς�

//�U��ID�i�Q�[�W�𑝂₷�Ƃ��Ƃ��Ɏg�p�j
#define ATTACK_WEAK				1
#define ATTACK_MIDDLE			2
#define ATTACK_STRONG			3
#define ATTACK_HISSATU			4
#define ATTACK_GOODBYE			5//�O�b�o�C�q�b�g
#define ATTACK_STRIKER			6//�X�g���C�J�[�_�b�V���q�b�g

//�`�F�[���R���{�t���O
//#define CHAIN_SA	0x00010000//�x�[�X�Œ�`
//#define CHAIN_SB	0x00020000
//#define CHAIN_SC	0x00040000
//#define CHAIN_CA	0x00080000
//#define CHAIN_CB	0x00100000
//#define CHAIN_CC	0x00200000
#define CHAIN_SD	0x00000001//�ǉ���
#define CHAIN_SE	0x00000002
#define CHAIN_JA	0x00000010
#define CHAIN_JB	0x00000020
#define CHAIN_JC	0x00000040

#define CHAIN_JAMPS (CHAIN_JA | CHAIN_JB | CHAIN_JC)//2�i�W�����v���ꊇ�N���A�p

//���[�U�[�q�b�g�EID��`
#define USERHIT_FLAG			(0x80)
#define USERHIT_AERIALPOP		(0x01 | USERHIT_FLAG)
#define USERHIT_AERIAL1			(0x02 | USERHIT_FLAG)
#define USERHIT_AERIAL2			(0x03 | USERHIT_FLAG)
#define USERHIT_AERIAL3			(0x04 | USERHIT_FLAG)

//���[�U�[��`���b�Z�[�W
#define GOBJMSG_NAGE			(GOBJMSG_USERDEFINE | 0x0001)	//�����p
#define GOBJMSG_HADOUHIT		(GOBJMSG_USERDEFINE | 0x0002)	//�g�����q�b�g
#define GOBJMSG_HADOUGUARD		(GOBJMSG_USERDEFINE | 0x0003)	//�g�����K�[�h
#define GOBJMSG_AERIALPOP		(GOBJMSG_USERDEFINE | USERHIT_AERIALPOP)	//�G���A���E������
#define GOBJMSG_AERIALHIT1		(GOBJMSG_USERDEFINE | USERHIT_AERIAL1)		//�G���A���E�q�b�g
#define GOBJMSG_AERIALHIT2		(GOBJMSG_USERDEFINE | USERHIT_AERIAL2)		//�G���A���E�q�b�g
#define GOBJMSG_AERIALHIT3		(GOBJMSG_USERDEFINE | USERHIT_AERIAL3)		//�G���A���E�q�b�g

//�I�v�V������`
#define OPTIONS_CHAIN_COMBO		0x00000001
#define OPTIONS_EXTRA_ATTACK	0x00000002
#define OPTIONS_AERIAL_ATTACK	0x00000004
#define OPTIONS_AERIAL_STEP		0x00000008
#define OPTIONS_AUTO_GUARD		0x00000010
#define OPTIONS_GUARD_CANCEL	0x00000020
#define OPTIONS_HADOUKEN		0x00000040
#define OPTIONS_HPPLUS30		0x00000080

//�ԍ���
#define MAAI_SHORT				(50)
#define MAAI_MIDDLE				(120)
#define MAAI_LONG				(220)
#define MAAI_NAGE				(20)

//�}�N����`
#define cell(a)		pdat->cnow=a	// "CELL_" ���������E�E�E
#define ccounter	pdat->counter
#define ALLGUARDFLAG		(GUARDINFO_XSTAND | GUARDINFO_XCROUCH | GUARDINFO_XJAMP)
#define CANNOT_GUARD(a)		((a&ALLGUARDFLAG)==0)
#define ALL_RIGHT			TRUE//�I�[���C

//wav�ԍ�
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
	�򓹋
---------------------------------------------------------------------*/

//�����C�N
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

//���_�b�V�����̕�
class CDashWind : public CBulletBase
{
public:
	CDashWind(CCharacter *parent);
	void act_run();
	void act_bomb();
};

//���g�����i�x�[�X�j
class CHadouKen : public CBulletBase
{
public:
	CHadouKen(CCharacter *parent) : CBulletBase(parent){}
	virtual void GoA(BOOL muki,double x,double y,float duration,ATTACKINFO *atk){}
	virtual void GoB(BOOL muki,double x,double y,double vx,ATTACKINFO *atk){}
};

//���g�����i��΂Ȃ����j
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

//���g�����i��ԕ��j
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

//���O�b�o�C�t�H�[�G�o�[
class CGoodbye : public CHadouKenA
{
public:
	CGoodbye(CCharacter *parent);
	void GoA(BOOL muki,double x,double y,float duration,ATTACKINFO *atk);

protected:
	void act_base();
	void Hit();

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

