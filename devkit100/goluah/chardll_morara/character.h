/*=====================================================================================

Goluah!! Copyright (C) 2001-2004 aki, 2004-2016 At-sushi, 2014-2016 logger

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

=======================================================================================*/

/*==============================================================

	�L�����N�^�[��`�w�b�_

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

//�򓹋�N���X�ꗗ
class CChibasiri;
class CTrack;
class CMiniGunE;
class CMiniGunB;

#define NUM_MINIGUN (3)

#define NEEDGAUGE_REV	(0.3)
#define NEEDGAUGE_GC	(0.5)

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

	//����������
	void InitParameters();		//�p�����[�^������
	void InitAnalyzeOptions();	//�I�v�V�����t���O����
	void InitBullets();			//�򓹋��������
	void InitAttackInfo();		//�U���͏��̏�����
	void InitWazInfo();			//�Z���̏�����

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

	//*********************************************
	//�@CCharacter�N���X�Ǝ��̊֐�
	//*********************************************
private:

	void StartAttack(DWORD atk_info_id);
	BOOL ComLevelCk(UINT level);

	//�ǉ��s���֐�
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
	
	//�G���s��
	DWORD eact_nage(GOBJECT *pedat);

	//�U���͏��
	ATTACKINFO aif[32];

	//�򓹋�
	CChibasiri *bul_chiba;
	CTrack *efe_track;
	CMiniGunB *bul_minigun[NUM_MINIGUN];
	CMiniGunE *efe_minigun;

	//�I�v�V�����t���O
	BOOL m_opt_ExtraAttack;
	BOOL m_opt_GuardCancel;
	BOOL m_opt_GCutterPlus;
	BOOL m_opt_ChibasiriPlus;
	BOOL m_opt_2ndStep;
	BOOL m_opt_StockableGauge;
	BOOL m_opt_minigunReloaded;

	//��������t���O
	BOOL m_2nd_step_flag;
};

extern LPTSTR CharDirectory;

//�s��ID�������ɒǉ�����
#define ACTID_LANDING		(ACTID_HISSATU | 0x0001)								//���n���̌�(�s���ł��Ȃ��悤�ɁA�K�E�Z����)
#define ACTID_ATT_SB2		(ACTID_ATTACK | 0x0002)									//�����A�ǉ����͋Z
#define ACTID_ATT_SC2		(ACTID_ATTACK | ACTID_KUCYU | 0x0003)					//�����A�ǉ����͋Z
#define ACTID_ATT_SC3		(ACTID_ATTACK | 0x0004)									//�����A���o�[�O����
#define ACTID_GENOCIDE1G	(ACTID_ATTACK | ACTID_HISSATU | 0x0005)					//�W�F�m�T�C�h��n��
#define ACTID_GENOCIDE1J	(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0006)	//�W�F�m�T�C�h���
#define ACTID_GENOCIDE2G	(ACTID_ATTACK | ACTID_HISSATU | 0x0007)					//�W�F�m�T�C�h���n��
#define ACTID_GENOCIDE2J	(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0008)	//�W�F�m�T�C�h����
#define ACTID_GENOCIDE3G	(ACTID_ATTACK | ACTID_HISSATU | 0x0009)					//�W�F�m�T�C�h���n��
#define ACTID_GENOCIDE3J	(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x000A)	//�W�F�m�T�C�h����
#define ACTID_LANDINGG		(ACTID_HISSATU | 0x000B)								//�W�F�m�T�C�h���n���̌�
#define ACTID_DASHF			(0x000C)												//�_�b�V���i�O�j
#define ACTID_DASHB			(ACTID_KUCYU |0x000D)									//�_�b�V���i��j
#define ACTID_CHIBA1		(ACTID_ATTACK | ACTID_HISSATU | 0x000E)					//�n����E��
#define ACTID_CHIBA2		(ACTID_ATTACK | ACTID_HISSATU | 0x000F)					//�n����E�~
#define ACTID_CHIBA3		(ACTID_ATTACK | ACTID_HISSATU | 0x0010)					//�n����E��
#define ACTID_MINIGUN		(ACTID_ATTACK | ACTID_HISSATU | 0x0011)					//�~�j�K��
#define ACTID_MINIGUN2		(ACTID_ATTACK | ACTID_HISSATU | 0x0012)					//�~�j�K���E��
#define ACTID_GCANCEL1		(ACTID_ATTACK | ACTID_HISSATU | 0x0013)					//�K�[�h�L�����Z�������E��
#define ACTID_GCANCEL2		(ACTID_ATTACK | ACTID_HISSATU | 0x0014)					//�K�[�h�L�����Z�������E��
#define ACTID_GCANCEL3		(ACTID_ATTACK | ACTID_HISSATU | 0x0015)					//�K�[�h�L�����Z�������E��
#define ACTID_2ND_STEP		(ACTID_KUCYU |0x0016)									//2�i�W�����v����
#define ACTID_GENOCIDE_UP	(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0017)	//�W�F�m�T�C�h�ǉ����́�
#define ACTID_GENOCIDE_DOWN	(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0018)	//�W�F�m�T�C�h�ǉ����́�
#define ACTID_NAGE1			(ACTID_ATTACK | ACTID_NAGE | 0x0019)					//����
#define ACTID_STRIKER_IN	(ACTID_INOUT  | 0x001A)
#define ACTID_STRIKER_ATK1	(ACTID_INOUT  | 0x001B)
#define ACTID_STRIKER_ATK2	(ACTID_INOUT  | 0x001C)
#define ACTID_STRIKER_OUT	(ACTID_INOUT  | 0x001D)

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
#define ATTACKINFO_STAND_B2		9	//�������ǉ����͋Z
#define ATTACKINFO_STAND_C2		10	//�������ǉ����͋Z
#define ATTACKINFO_STAND_C3		11	//������������͋Z
#define ATTACKINFO_GENOCIDE1	12	//�W�F�m�T�C�h�E��
#define ATTACKINFO_GENOCIDE2	13	//�W�F�m�T�C�h�E�~
#define ATTACKINFO_GENOCIDE3	14	//�W�F�m�T�C�h�E��
#define ATTACKINFO_CHIBA1		15	//�n����E��
#define ATTACKINFO_CHIBA2		16	//�n����E�~
#define ATTACKINFO_CHIBA3		17	//�n����E��
#define ATTACKINFO_MINIGUN		18	//�~�j�K��
#define ATTACKINFO_GENOCIDE_UP	19	//�W�F�m�T�C�h�E�ǉ���
#define ATTACKINFO_GENOCIDE_DOWN 20	//�W�F�m�T�C�h�E�ǉ���
#define ATTACKINFO_NAGE			21	//����
#define ATTACKINFO_STRIKER1		22	//�X�g���C�J�[�p1
#define ATTACKINFO_STRIKER2		23	//�X�g���C�J�[�p2

//�U��ID�i�K�v������ΕύX����j
#define ATTACK_WEAK				1
#define ATTACK_MIDDLE			2
#define ATTACK_STRONG			3
#define ATTACK_HISSATU			4

//�`�F�[���R���{�t���O
//#define CHAIN_SA	0x00010000 �x�[�X�Œ�`
//#define CHAIN_SB	0x00020000
//#define CHAIN_SC	0x00040000
//#define CHAIN_CA	0x00080000
//#define CHAIN_CB	0x00100000
//#define CHAIN_CC	0x00200000
#define CHAIN_SC3	0x00000001//�ǉ���

//���[�U�[�q�b�gID
#define USERHIT_GCUTTERUP		0x81

//���[�U�[��`���b�Z�[�W
#define GOBJMSG_NAGE			(GOBJMSG_USERDEFINE | 0x0001)				//�����p
#define GOBJMSG_CHIBAHIT		(GOBJMSG_USERDEFINE | 0x0002)				//�n����q�b�g
#define GOBJMSG_CHIBAGUARD		(GOBJMSG_USERDEFINE | 0x0003)				//�n����K�[�h
#define GOBJMSG_GCUTTERUP		(GOBJMSG_USERDEFINE | USERHIT_GCUTTERUP)	//G�J�b�^�[(��)�E�q�b�g

//aki�p�}�N����`
#define cell(a)					pdat->cnow=a
#define ccounter				pdat->counter
#define PI						(3.14159265f)
#define WAIT_A_SEC				TRUE//�܂��Ă�A������

//�ԍ���
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
	�n����
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
	�O��
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
	void act_run();	//���ۂɂ͉������Ȃ�
	void act_bomb();//���ۂɂ͉������Ȃ�

	void PosUpdate();//�V�K���_�ǉ�

	CTristrip* m_ts;

	//�p�����[�^
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

	//�����p�����[�^
	float m_rot;
	UINT m_num,m_step;
	V2d  m_ppos,m_pvec;
	V2d *m_pos;
	UINT m_head;
	UINT m_tail;
};


/*--------------------------------------------------------------
	�~�j�K��
----------------------------------------------------------------*/
//�e
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

//�G�t�F�N�g
class CMiniGunE : public CBulletBase
{
public:
	CMiniGunE(CCharacterBase *parent);

protected:
	void act_run();
	void act_bomb(){}
};


