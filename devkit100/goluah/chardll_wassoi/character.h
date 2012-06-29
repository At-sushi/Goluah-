/*==============================================================

	�L�����N�^�[��`�w�b�_

================================================================*/
#pragma once

#include <windows.h>
#include <stdio.h>

#include <d3d8.h>
#include <d3dx8.h>

#include "gobjbase.h"
#include "cell.h"// �ʏ�Aare_editer�Ő��������w�b�_�[�t�@�C�������include���܂�

//�I�v�V������`
#define OPTIONS_HEAVY_HIT		0x00000001
#define OPTIONS_AUTO_GUARD		0x00000002
#define OPTIONS_GUARD_CANCEL	0x00000004
#define OPTIONS_KOROKORO		0x00000008
#define OPTIONS_HPUP			0x00000010
#define OPTIONS_REGENERATOR		0x00000020

#define AIF_NUM					(32)
#define NEEDGAUGE_GC			(0.5f)
#define NEEDGAUGE_REV			(0.2f)
#define NUM_BLURS				(3)

class CWassoiLight;

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
	DWORD TouchA(ATTACKINFO *info,DWORD ta_eid);
	DWORD TouchB(ATTACKINFO *info,BOOL hit);
	BOOL Command_OnSystem(DWORD key);
	BOOL Command_OnNormal(DWORD keyinfo);
	BOOL Command_Normal(DWORD keyinfo);
	BOOL Command_Hissatu(DWORD keyinfo);
	BOOL Command_OnAttacking(DWORD key);
	DWORD CommandCOM(DWORD wid);
	DWORD CmdCom_OnSystem(DWORD wid);
	DWORD CmdCom_OnNormal(DWORD wid);
	void InitAttackInfo();//�U���͏��̏���
	void InitWazInfo();//�Z���̏���
	void InitBullets();
	DWORD ComThink();
	DWORD Draw();

	//�ʏ퓮��
	void act_neutral();//�j���[�g����
	void act_crouch();//���Ⴊ��
	void act_rakka();//����
	void act_tojyo();//�o��
	void act_win();//����
	void act_walkf();//�O�i
	void act_walkb();//���
	void act_jamps();//�W�����v�\���
	void act_jampc();//�����W�����v
	void act_jampf();//�O�W�����v
	void act_jampb();//��W�����v
	//�ʏ�.�U��
	void act_att_sa();//����
	void act_att_sb();//����
	void act_att_sc();//����
	void act_att_ca();//���
	void act_att_cb();//��
	void act_att_cc();//��
	void act_att_ja();//���
	void act_att_jb();//��
	void act_att_jc();//��
	//���̑�
	void act_taiki();
	void act_koutai_out();
	void act_koutai_in();
	void act_koutai();
	void act_koutai2();

	void act_timeoverlose();//�^�C���I�[�o�[

	//*********************************************
	//�@CCharacter�N���X�Ǝ��̊֐�
	//*********************************************
private:
	void InitParameters();//�p�����[�^����
	void StartAttack(DWORD atk_info_id);

	//�ǉ�s���֐�
	void act_dashf();
	void act_dashb();
	void act_att_smc();
	void act_wassoi1();
	void act_wassoi2();
	void act_wassoi3();
	void act_wassoif();
	void act_hadou1();
	void act_hadou2();
	void act_hadou3();
	void act_syara1();
	void act_syara2();
	void act_syara3();
	void act_syaraf();
	void act_festival();
	void act_festivaldash();
	void act_festivalsuki();
	void act_festivalrush();
	void act_festivalfinish();
	void act_gcancel1();
	void act_gcancel2();
	void act_gcancel3();
	void act_nage();
	DWORD act_nagee(GOBJECT *pedat);
	void act_okiagari_foword();
	void act_okiagari_back();
	void act_striker1();
	void act_striker2();
	void act_striker3();
	void act_striker4();

	//�U���͏��
	ATTACKINFO aif[AIF_NUM];
	
	//�򓹋�
	CClassicalBullet	*b_nori;
	CClassicalBullet	*b_nori2;
	CWassoiLight		*b_wlight;

	//options
	DWORD option_flag;
	BOOL OptHeavyHit()		{return FALSE;}
	BOOL OptGuardCancel()	{return (option_flag&OPTIONS_GUARD_CANCEL)	? TRUE : FALSE;}
	BOOL OptKoroKoro()		{return (option_flag&OPTIONS_KOROKORO)		? TRUE : FALSE;}
	BOOL OptRegenerator()	{return (option_flag&OPTIONS_REGENERATOR)	? TRUE : FALSE;}

	DWORD hp_to_regenerate;

	// �u���[
	struct BLUR {
		DWORD cnum;
		int x, y;
		float z;
		int rot;
		BOOL revx, revy;
		DWORD color;
		float magx, magy;
	} m_blur[NUM_BLURS];
	int m_blur_target;
};

extern LPTSTR CharDirectory;

//�s��ID
#define ACTID_ATT_SMC			(ACTID_ATTACK | 0x0001)
#define ACTID_WASSYORYUKEN1		(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0002)
#define ACTID_WASSYORYUKEN2		(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0003)
#define ACTID_WASSYORYUKEN3		(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0004)
#define ACTID_WASSYORYUKENF		(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0005)
#define ACTID_HADOU1			(ACTID_ATTACK | ACTID_HISSATU | 0x0006)
#define ACTID_HADOU2			(ACTID_ATTACK | ACTID_HISSATU | 0x0007)
#define ACTID_HADOU3			(ACTID_ATTACK | ACTID_HISSATU | 0x0008)
#define ACTID_DASHF				(0x0009)
#define ACTID_DASHB				(ACTID_KUCYU  | 0x000A)
#define ACTID_SYARA1			(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x000B)
#define ACTID_SYARA2			(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x000C)
#define ACTID_SYARA3			(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x000D)
#define ACTID_SYARAF			(ACTID_ATTACK | ACTID_HISSATU | 0x000E)
#define ACTID_FESTIVAL			(ACTID_ATTACK | ACTID_HISSATU | 0x000F)
#define ACTID_FESTIVALDASH		(ACTID_ATTACK | ACTID_HISSATU | 0x0010)
#define ACTID_FESTIVALSUKI		(ACTID_ATTACK | ACTID_HISSATU | 0x0011)
#define ACTID_FESTIVALRUSH		(ACTID_ATTACK | ACTID_HISSATU | 0x0012)
#define ACTID_FESTIVALFINISH	(ACTID_ATTACK | ACTID_HISSATU | ACTID_KUCYU | 0x0013)
#define ACTID_GCANCEL1			(ACTID_ATTACK | ACTID_HISSATU | 0x0014)
#define ACTID_GCANCEL2			(ACTID_ATTACK | ACTID_HISSATU | 0x0015)
#define ACTID_GCANCEL3			(ACTID_ATTACK | ACTID_HISSATU | 0x0016)
#define ACTID_NAGE1				(ACTID_ATTACK | ACTID_NAGE    | 0x0017)
#define ACTID_OKIAGARI_FOWORD	(ACTID_SYSTEM | 0x0018)
#define ACTID_OKIAGARI_BACK		(ACTID_SYSTEM | 0x0019)
#define ACTID_STRIKER1			(ACTID_INOUT  | 0x001A)
#define ACTID_STRIKER2			(ACTID_INOUT  | 0x001B)
#define ACTID_STRIKER3			(ACTID_INOUT  | 0x001C)
#define ACTID_STRIKER4			(ACTID_INOUT  | 0x001D)

//�U���͔ԍ�
#define ATTACKINFO_STAND_A		0
#define ATTACKINFO_STAND_B		1
#define ATTACKINFO_STAND_C		2
#define ATTACKINFO_CROUCH_A		3
#define ATTACKINFO_CROUCH_B		4
#define ATTACKINFO_CROUCH_C		5
#define ATTACKINFO_JAMP_A		6
#define ATTACKINFO_JAMP_B		7
#define ATTACKINFO_JAMP_C		8
#define ATTACKINFO_STAND_MB		9
#define ATTACKINFO_DIVE1		10
#define ATTACKINFO_DIVE2		11
#define ATTACKINFO_DIVE3		12
#define ATTACKINFO_SYARA1		13
#define ATTACKINFO_SYARA2		14
#define ATTACKINFO_SYARA3		15
#define ATTACKINFO_FESTA_D		16
#define ATTACKINFO_FESTA		17
#define ATTACKINFO_FESTA_F		18
#define ATTACKINFO_NAGE			19

//�U��ID�i�K�v������ΕύX����j
#define ATTACK_WEAK				1
#define ATTACK_MIDDLE			2
#define ATTACK_STRONG			3
#define ATTACK_HISSATU			4
#define ATTACK_CYOHI			5
#define ATTACK_SCDASH			6

//���[�U�[��`���b�Z�[�W
#define GOBJMSG_NAGE				(GOBJMSG_USERDEFINE | 0x0001)	//�����p
#define GOBJMSG_FESTIVAL_NORI_HIT	(GOBJMSG_USERDEFINE | 0x0002)	//�C�ۃq�b�g

//�ԍ���
#define MAAI_SHORT				(50)
#define MAAI_MIDDLE				(120)
#define MAAI_LONG				(220)
#define MAAI_NAGE				(20)

//wave�i���o�[
#define wave_tojyo				4
#define wave_win				5
#define wave_nage				6
#define wave_dive				7
#define wave_nori				8
#define wave_syara				9
#define wave_fdash				10
#define wave_frush				11
#define wave_ffinish			12

//Chain
#define CHAIN_SMC				0x00000001


//���
#define WLIGHT_NUM	16
class CWassoiLight : public CBulletBase
{
public:
	CWassoiLight(CCharacterBase* parent);
	void SetTarget(DWORD oid){tgt_oid = oid;}

protected:
	void act_run();
	void act_bomb();
	DWORD Draw();

	DWORD tgt_oid;

	struct CWassoiLightInfo
	{
		BOOL isCreated;
		UINT counter;
		float x,y;
		float sca;

		void Start(float tgt_x,float tgt_y);
		void Update();
		void Draw(LPDIRECT3DDEVICE8 dev);
	};

	CWassoiLightInfo info[WLIGHT_NUM];
};

