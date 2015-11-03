
/*==============================================================

	�L�����N�^�[��`�w�b�_

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

//��ѓ���N���X
class CHadou;
class CEnergyArrow;
namespace Morarael
{
class CTrack;//�m�[�}���������[�Ƃ��Ԃ����̂�
}
class CJudge;
class CDashWind;

//�萔��`
#define EARROW_NUM		(4)
#define SOJ_OFFSET		(10)
#define NUM_ATTACKINFO	(32)

#define NEEDGAUGE_TETRA	(0.2)
#define NEEDGAUGE_SOJ	(0.1)
#define NEEDGAUGE_DOJ	(1.0)
#define NEEDGAUGE_MEGID	(1.0)

#define JUDGE_MAXHIT	(3)

/*--------------------------------------------------------------
	���C���L�����N�^�[�N���X
----------------------------------------------------------------*/
class CCharacterEL : public CCharacterBase
{
public:
	CCharacterEL(CDI_CHARACTERINFO2 *info);
	virtual ~CCharacterEL();

protected:
	//*********************************************
	//  �I�[�o�[���C�h����֐�
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
	void InitAttackInfo();//�U���͏��̏�����
	void InitWazInfo();//�Z���̏�����
	void InitBullets();//�򓹋����
	void InitGCDandBMP();//�r�b�g�}�b�v���[�h
	void InitMySound(char* path_name,char* list_name);
	DWORD TouchB(ATTACKINFO *info,BOOL hit);
	DWORD TouchA(ATTACKINFO *info,DWORD ta_eid);
	DWORD ComThink();

	//�ʏ퓮��
	void act_neutral();//�j���[�g����
	void act_crouch();//���Ⴊ��
	void act_rakka();//����
	void act_tojyo();//�o��
	void act_win();//����
	void act_walkf();//�O�i
	void act_walkb();//���
	void act_jamps();//�W�����v�\������
	void act_jampc();//�����W�����v
	void act_jampf();//�O�W�����v
	void act_jampb();//��W�����v
	void act_timeoverlose();//�^�C���I�[�o�[
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
	void InitAnalyzeOptions();//�I�v�V��������
	void StartAttack(DWORD atk_info_id);
	BOOL com_nage2(DWORD maai);
	BOOL ComLevelCk(UINT level);

	//�ǉ��s���֐�
	void act_earrow1();	//�G�i�W�[�A���[
	void act_earrow2();
	void act_earrow3();
	void act_earrow1j();//�G�i�W�[�A���[(��)
	void act_earrow2j();
	void act_earrow3j();
	void act_dashf();	//�_�b�V��
	void act_dashb();
	void act_dashfj();	//�󒆃_�b�V��
	void act_dashbj();
	void act_sword1r();	//�\�[�h�A���
	void act_sword2r();
	void act_sword3r();
	BOOL act_swordRealize();
	void act_sword1a();	//�\�[�h�A�U��
	void act_sword2a();
	void act_sword3a();
	void act_sword1d();	//�\�[�h�A����
	void act_sword2d();
	void act_sword3d();
	void act_nage1();	//����
	void act_nage2();
	void act_nageend();
	void act_judgement();//�W���b�W�����g
	void act_att_scs1();
	void act_att_scs2();

	//�G���s��
	DWORD eact_nage(GOBJECT *pedat);

	//�U���͏��
	ATTACKINFO aif[NUM_ATTACKINFO];

	//�򓹋�
	CHadou				*pbul_hadou;
	CEnergyArrow		*pbul_earrow[EARROW_NUM];
	Morarael::CTrack	*pefe_track;
	CJudge				*pbul_judge;
	CDashWind			*pefe_wind;

	//�I�v�V����
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

//�s��ID�������ɒǉ�����
#define ACTID_EARROW1			(ACTID_HISSATU | ACTID_ATTACK | 0x0001)//�e�g���O���}�g���i�����G�i�W�[�A���[�j
#define ACTID_EARROW2			(ACTID_HISSATU | ACTID_ATTACK | 0x0002)
#define ACTID_EARROW3			(ACTID_HISSATU | ACTID_ATTACK | 0x0003)
#define ACTID_DASHF				(ACTID_KUCYU | 0x0004)//�_�b�V���i�n�ォ��j
#define ACTID_DASHB				(ACTID_KUCYU | 0x0005)
#define ACTID_EARROW1J			(ACTID_HISSATU | ACTID_ATTACK | ACTID_KUCYU | 0x0006)//�󒆃e�g���O���}�g���i�����G�i�W�[�A���[�j
#define ACTID_EARROW2J			(ACTID_HISSATU | ACTID_ATTACK | ACTID_KUCYU | 0x0007)
#define ACTID_EARROW3J			(ACTID_HISSATU | ACTID_ATTACK | ACTID_KUCYU | 0x0008)
#define ACTID_DASHFJ			(ACTID_KUCYU | 0x0009)//�_�b�V���i�󒆂���j
#define ACTID_DASHBJ			(ACTID_KUCYU | 0x000A)
#define ACTID_SWORD1R			(ACTID_HISSATU | ACTID_ATTACK | 0x000B)//�\�[�h(��)�A���
#define ACTID_SWORD2R			(ACTID_HISSATU | ACTID_ATTACK | 0x000C)
#define ACTID_SWORD3R			(ACTID_HISSATU | ACTID_ATTACK | 0x000D)
#define ACTID_SWORD1A			(ACTID_HISSATU | ACTID_ATTACK | 0x000E)//�\�[�h(��)�A�U��
#define ACTID_SWORD2A			(ACTID_HISSATU | ACTID_ATTACK | 0x000F)
#define ACTID_SWORD3A			(ACTID_HISSATU | ACTID_ATTACK | 0x0010)
#define ACTID_SWORD1D			(ACTID_HISSATU | ACTID_ATTACK | 0x0011)//�\�[�h(��)�A����
#define ACTID_SWORD2D			(ACTID_HISSATU | ACTID_ATTACK | 0x0012)
#define ACTID_SWORD3D			(ACTID_HISSATU | ACTID_ATTACK | 0x0013)
#define ACTID_NAGE1				(ACTID_ATTACK | ACTID_NAGE | 0x0014)				//����
#define ACTID_NAGE2				(ACTID_ATTACK | ACTID_NAGE | ACTID_KUCYU | 0x0015)
#define ACTID_NAGEEND			(ACTID_ATTACK | ACTID_NAGE | ACTID_KUCYU | 0x0016)
#define ACTID_JUDGEMENT			(ACTID_HISSATU | ACTID_ATTACK | ACTID_KUCYU | 0x0017)//�f�C�E�I�u�E�W���b�W�����g
#define ACTID_ATT_SCS1			(ACTID_ATTACK | 0x0018)
#define ACTID_ATT_SCS2			(ACTID_ATTACK | 0x0019)

//�U���͔ԍ��������ɒ�`����
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


//�U��ID�i�K�v������ΕύX����j
#define ATTACK_WEAK				1
#define ATTACK_MIDDLE			2
#define ATTACK_STRONG			3
#define ATTACK_HISSATU			4
#define ATTACK_EREAL			0x00010000

//���[�U�[��`���b�Z�[�W
#define GOBJMSG_NAGE			(GOBJMSG_USERDEFINE | 0x0001)	//�����p
#define GOBJMSG_EARROWHIT		(GOBJMSG_USERDEFINE | 0x0002)	// �R���{�p

//�}�N����`
#define cell(a)				pdat->cnow=a
#define ccounter			(pdat->counter)
#define ALLGUARDFLAG		(GUARDINFO_XSTAND | GUARDINFO_XCROUCH | GUARDINFO_XJAMP)
#define CANNOT_GUARD(a)		((a&ALLGUARDFLAG)==0)

//�`�F�[���R���{�t���O
//#define CHAIN_SA	0x00010000 �x�[�X�Œ�`
//#define CHAIN_SB	0x00020000
//#define CHAIN_SC	0x00040000
//#define CHAIN_CA	0x00080000
//#define CHAIN_CB	0x00100000
//#define CHAIN_CC	0x00200000
#define CHAIN_SWORD1	0x00000001//�`�F�[���Ƃ��ăJ�E���g����
#define CHAIN_SWORD2	0x00000002
#define CHAIN_SWORD3	0x00000004

//�ԍ���
#define MAAI_NAGE		(30)

//wave�ԍ�
#define	wave_tojyo			4
#define	wave_win			5
#define	wave_tetra			6
#define wave_sword			7
#define	wave_sword1			8
#define	wave_sword2			9
#define	wave_sword3			10
#define	wave_judge			11


//--------------------------------------------------------------------------------
//	�򓹋�N���X��`
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

	
	CTristrip* m_ts;			//�g���C�X�g���b�v
	UINT m_num;				//���|�C���g��
	UINT m_step;			//1�t���ōX�V����|�C���g��
	V2d m_ppos;				//�O��ʒu
	V2d m_pvec;				//�O��j�Z���x

	V2d *m_pos;				//�ʒu�o�b�t�@
	float m_width;			//�����
	ARGB m_color;			//��F

	float m_maxspeed;		//�ő呬�x
	float m_trackstrength;	//�ǔ�����
	float m_lifetime;		//����
};

/*--------------------------------------------------------------
	�O��
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


}//namespace Morarael

/*--------------------------------------------------------------
	�W���b�W�����g
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

	CParticle* m_tubu;			//�p�[�e�B�N���N���X
	LPDIRECT3DTEXTURE9 m_tex;	//�e�N�X�`��

	//�p�����[�^
	float m_width;
	float m_widthorg;

	//�J�n�_
	float m_start_x;
	float m_start_y;

	//������������X�g
	struct JudgeHitInfo
	{
		DWORD oid;
		DWORD cnt;
	};
	typedef std::list<JudgeHitInfo> JudgeHitList;
	JudgeHitList m_hitlist;
	JudgeHitList::iterator FindHitInfo(DWORD oid);

	//�Ƃ��Ƃ��p�����[�^
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

//���_�b�V�����̕�
class CDashWind : public CBulletBase
{
public:
	CDashWind(CCharacterBase *parent);
	void act_run();
	void act_bomb();
};


