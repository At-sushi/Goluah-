#pragma once

#include "define_const.h"
#include "battletaskbase.h"
#include "gauge.h"
#include "efctlist.h"

#define MAXEFCT_SIVER 16
#define DEBUGMSGBUFFERSIZE	(1024*4)
#define OBJECTS_MEMINCRATE	32//!< �I�u�W�F�N�g�z��A�����������l

#include <PshPack1.h>
//! �l�b�g�ɑ��M���郁�b�Z�[�W�̃x�[�X
typedef struct NetMessage{
	BYTE msgid;
	operator BYTE*()	{ return &msgid; }
} NET_MESSAGE;

//!10�t���[�����ƁAACTID�X�V���Ȃǂ̓������b�Z�[�W
#define GNETMSG_TESTSYNC		0
struct TestSyncMes : NetMessage{
	DWORD id;
	float x, y;
	DWORD aid, /*now, */counter;
	bool muki;
};

//!HP�E�Q�[�W����(�v���ǁj
#define GNETMSG_SYNC2			1
struct Sync2Mes : NetMessage{
	BYTE pid;
	int hp;
	float gauge;
};

//!�t���[�����P�i�񂾒ʒm
#define GNETMSG_ACTION			2
struct ActionMes : NetMessage{
	bool isStop;		// �X�g�b�v�����ǂ���
};

//!�U�����������ʒm
#define GNETMSG_HIT				3
struct HitMes : NetMessage{
	DWORD oid/*, aid*/;
	BOOL hit;
};

//!�L�[���͒ʒm
#define GNETMSG_INPUT				4
struct InputMes : NetMessage{
	DWORD key;
};

//!�R�}���h�Z�����ʒm
#define GNETMSG_COMMAND				5
struct CommandMes : NetMessage{
	DWORD aid;
};

//!HP����
#define GNETMSG_SYNCHP				6
struct SyncHPMes : NetMessage{
	BYTE pid;
	int hp;
};
#include <PopPack.h>						// �l�b�g���b�Z�[�W�I���

//!�t���[�����Q�i�񂾒ʒm
#define GNETMSG_ACTION2				7
typedef struct ActionMes Action2Mes;

/*!
*	@brief �l�b�g�ΐ�Ő퓬�^�X�N
*	@ingroup Battle
*	@sa CBattleTask
*	@sa CDirectPlay
*
*	�l�b�g�ΐ�Ɏg���\��̃^�X�N�N���X(�ǂ݂ɂ����c)�ł�
*	��������Ă��镔����CBattleTask�Ǝ��������������ł����A���M����(SendMsg)�̈ʒu���΂�΂�Ȃ̂����Ƃ��������c
*
*	�b��[�u��K�L�[�Ŏ�����HP���񕜂ł��܂�
*/
class CBattleTaskNet :
	public CBattleTaskBase
{
public:
	CBattleTaskNet(void);
	virtual ~CBattleTaskNet(void);
	virtual DWORD GetID(){return 'BtlN';}

	//�^�X�N�֐�
	virtual void Initialize();
	virtual BOOL Execute(DWORD time);
	virtual void Draw();
	virtual void WndMessage(HWND hWnd,UINT msg,WPARAM wparam, LPARAM lparam);//!< F7�Ń|�[�Y
	static  HRESULT CALLBACK DPlayCallBack(PVOID UserCont, DWORD mtype, PVOID pmes);
	HRESULT DPlayMessage(PVOID UserCont, DWORD mtype, PVOID pmes);

	//���擾�n
	virtual double GetDisplayCenterX(){return disp_center_x;}
	virtual double GetDisplayCenterY(){return disp_center_y;}
	virtual BOOL   IsNetwork(){return TRUE;}
	virtual BOOL   IsLocal(BYTE dllid){return g_play.IsHost()/*g_battleinfo.GetKeyAssign((dllid-1)/MAXNUM_TEAM, (dllid-1)%MAXNUM_TEAM) == 0*/;}	// �b��

	//void SetKatiSerif(DWORD tid,char *serif);
	virtual DWORD MessageFromObject(DWORD oid,DWORD msg,DWORD prm);
	virtual BOOL CatchObject(DWORD eoid,LPVOID cy);
	virtual void AddEffect(DWORD efctid,int prm1,int prm2,int prm3=0);
	virtual void Atari(DWORD a_id,DWORD k_id,MY2DVECTOR &kas_point);
	virtual void HitStop(DWORD len,DWORD oid);

	virtual void SetTransform(BOOL b);


protected:
	//Initialize����R�[��
	virtual void InitializeObjectList();
	virtual void InitializeSubTasks();
	virtual void InitializeParameters();

	//Terminate����R�[��
	virtual void TerminateObjectList();
	virtual void TerminateDestroySubTasks();

	//Execute��������
	virtual void T_Command();			//!< COMMAND ���b�Z�[�W�𑗐M����
	virtual void T_Action(BOOL stop);	//!< Action ���b�Z�[�W�𑗐M����
	virtual void T_Sousai();			//!< �򓹋�E�p ��`�����蔻�菈��
	virtual void T_AtariHantei();		//!< �U���p�@�@�@ ��`�����蔻�菈��
	virtual void T_KasanariHantei();	//!< �d�Ȃ蔻��p ��`�����蔻�菈��
//	virtual void T_Draw();				//!< DRAW ���b�Z�[�W�𑗐M����
	virtual void T_ChangeTarget();		//!< �I�u�W�F�N�g�^�[�Q�b�g�ύX����
	virtual void T_UpdateStatus();		//!< �����̏�ԍX�V

	virtual void StartRound();
	virtual void UpdateKeyInputDirections();

protected:
	//�S�̃G�t�F�N�g�֘A
	int efct_slowdown,efct_stop,efct_darkbg,efct_nobg;
	int efct_sindo,efct_sindom;
	int efct_hitstop;
	int efct_fadein;
	int efct_flash;

	//���
	DWORD hprecratio[2][MAXNUM_TEAM];	//!<���R��㐧�̂Ƃ��̗͉̑񕜗�
	BOOL  battle_end;					//!<�^�X�N���k����̂Ɏg�p
	int	  actcount;						//!<���Ɖ���Action�̎��s�������ꂽ��
	bool  hoststop;						//!<�z�X�g����~��

	//!�f�o�b�O�e�L�X�g�\���p�o�b�t�@
	char *debugmsgbuff;

	//!�\�����S
	double disp_center_x,disp_center_y;

	//�N���X
	CGauge		*cp_gauge;
	CEffectList	*cp_efctlist;
	DWORD score[2];			//!< �X�R��

	//�֐��𕪊������̂Ń��[�J�����Ⴂ���Ȃ��Ȃ�������
	BOOL act_stop;


	/*���I�u�W�F�N�g�Ǘ���-----------------------------------------------------*/
public:
	CGObject* GetGObject(DWORD id);					//!< �w��ID�I�u�W�F�N�g�擾
	DWORD CreateGObject();							//!< �I�u�W�F�N�g����
	DWORD CreateGObjectFx();						//!< �I�u�W�F�N�g����(�G�t�F�N�g)
	void DeleteGObject(DWORD oid);					//!< �I�u�W�F�N�g�j��
	void SuicideGObject(DWORD oid);					//!< �I�u�W�F�N�g�j���\��
	CGObject* GetCharacterObject(DWORD j,DWORD i);	//!< �L�����N�^�[�̃I�u�W�F�N�g�擾�i�Q�[�W���ŕK�v�j
protected:
	std::vector< CGObject* > p_objects;				//!< �I�u�W�F�N�g����N���X�̃|�C���^
	DWORD object_regindex[MAXNUM_TEAM*2+2];			//!< ���ɐ�������I�u�W�F�N�g�̃C���f�b�N�X
	std::vector< WORD > object_regno;				//!< ���̃C���f�b�N�X�ł����̃I�u�W�F�N�g����������Ă�����
	std::vector< DWORD > suicide_list;				//!< ���ŃI�u�W�F�N�g���X�g
};

#define BATTLETASK_FXOBJFLAG		0x80000000
#define BATTLETASK_ISNOTFXOBJ(p)	((p->id & BATTLETASK_FXOBJFLAG) ? FALSE : TRUE)

