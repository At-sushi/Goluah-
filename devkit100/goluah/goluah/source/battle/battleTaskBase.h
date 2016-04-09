/*
2011/10/29	timeover�̃{�C�X�ɑΉ�
			�����̕`�ʂɎg��cell�����Ԑ����EHit�EDamage���ŕ�����
*/
/*=========================================================================
	
	�퓬�^�X�N��b

	�l�b�g���[�N�Ή��E�l�b�g���[�N��Ή� �̗����̐퓬�^�X�N�̋��ʕ���

===========================================================================*/
#pragma once

#include "gobject.h"
#include "object.h"
#include "task.h"

struct MY2DVECTOR;

/*!
*	@defgroup Battle
*	@ingroup Tasks
*	@brief �����֌W
*/

/*!
*	@brief ���������ǂ�������Ԃ�
*	@ingroup Battle
*/
enum BattleTaskState
{
	BFSTATE_WAITFORENDPOSE	,//!<�o��|�[�Y�I���҂�
	BFSTATE_ROUNDCALL		,//!<���E���h�R�[���I���҂�
	BFSTATE_FIGHTING		,//!<�퓬���
	BFSTATE_FINISHED		,//!<KO�A�L�����N�^�̍ŏI�_�E���I���҂�
	BFSTATE_WAITFORENDWIN	,//!<�����|�[�Y�I���҂�
	BFSTATE_DOUBLEKO		,//!<�_�u��KO�\���I���҂�
	BFSTATE_TIMEOVER		 //!<�^�C���I�[�o�[�\���I���҂�
};


/*!
*	@brief �퓬�^�X�N�x�[�X�N���X
*	@ingroup Battle
*
*	������l�b�g�ΐ킪��������邩������Ȃ��ƁA���ʑ�����\�Ȍ���ʃN���X�Ɏ������Ă݂��B
*	�p���N���X�ł��ׂ����Ƃ́A�I�u�W�F�N�g�Ǝ����i�s�̊Ǘ��B���Ԃ�B
*	�r������߂�ǂ������Ȃ��āA�����ƕ������Ă��Ȃ����������邪���E
*
*	@sa CBattleTask
*/
class CBattleTaskBase : public CExclusiveTaskBase
{
public:
	CBattleTaskBase();
	virtual ~CBattleTaskBase(){}

	//�^�X�N�n�֐�
	virtual void Initialize();
	virtual void Terminate();
	virtual DWORD GetID(){return 'Btl';}

	//�I�u�W�F�N�g�Ǘ�����
	//�I�u�W�F�N�g�̊Ǘ��̓T�u�N���X�Ŏ�������
	virtual CGObject* GetGObject(DWORD id)=0;					//!< �w��ID�I�u�W�F�N�g�擾
	virtual DWORD CreateGObject()=0;							//!< �I�u�W�F�N�g����
	virtual DWORD CreateGObjectFx()=0;							//!< �I�u�W�F�N�g����(�G�t�F�N�g)
	virtual void DeleteGObject(DWORD oid)=0;					//!< �I�u�W�F�N�g�j��
	virtual void SuicideGObject(DWORD oid)=0;					//!< �I�u�W�F�N�g�j���\��
	virtual CGObject* GetCharacterObject(DWORD j,DWORD i) = 0;	//!< �L�����N�^�[�̃I�u�W�F�N�g�擾�i�Q�[�W���ŕK�v�j

	//�I�u�W�F�N�g�T�[�r�X
	//�I�[�o�[���[�h�̕K�v�����邩�ǂ����킩��Ȃ����͈̂ꉞvirtual��
	virtual DWORD MessageFromObject(DWORD oid,DWORD msg,DWORD prm) = 0;
	virtual BOOL CatchObject(DWORD eoid,LPVOID cy) = 0;
	        void AddDamage(DWORD oid,DWORD eoid,int x,int y);	//!< GetObject�𗘗p����
	        DWORD GetMaai(DWORD oid,DWORD eoid,BOOL v);			//!< GetObject�𗘗p����
	        GOBJECT* GetGObjectInfo(DWORD oid);					//!< GetObject�𗘗p����
	        GOBJECT* GetCharacterInfo(DWORD j,DWORD i);			//!< GetObject�𗘗p����
	virtual void AddEffect(DWORD efctid,int prm1,int prm2,int prm3=0) = 0;

	//�p�����[�^�ݒ�E�擾�n
	DWORD GetWinCount(DWORD tid);								//!< �����J�E���g�����擾
	DWORD GetStrikerCount(DWORD tid);							//!< �c��X�g���C�J�[�����擾
	TCHAR* GetKatiSerif();										//!< �����䎌���擾
	void SetKatiSerif(DWORD tid,TCHAR *serif);					//!< �����䎌��ݒ�
	void* GetActiveCharacterInfo(DWORD tid);					//!< �u�A�N�e�B�u�v�ȃL�����N�^�̎擾
	CDI_CHARACTERINFO2* GetCDICharacterInfo(DWORD tid,DWORD index){return &m_cinfo[tid][index];}
	SDI_STAGEINFO2* GetSDIStageInfo(){return &m_sinfo;}			
	BattleTaskState GetState(){return bf_state;}				//!< ������Ԏ擾
	DWORD GetActiveCharacterID(DWORD team);
	GOBJECT* GetActiveCharacter(DWORD team);					//!< "�A�N�e�B�u"�ȃL�����N�^�[��Ԃ�
	DWORD GetCharacterOID(DWORD t,DWORD m){return charobjid[t][m];}
	virtual double GetDisplayCenterX() = 0;						//!< �\�����SX�擾
	virtual double GetDisplayCenterY() = 0;						//!< �\�����SY�擾
	virtual BOOL IsNetwork() = 0;								//!< �l�b�g���[�N���ǂ���
	virtual BOOL IsLocal(BYTE dllid) = 0;						//!< �w��DLL�����[�J�����ǂ���

	//�`��T�[�r�X
	virtual void SetTransform(BOOL b) = 0;
	void DrawNumber(DWORD num,int x,int y,BOOL hits,float z, float magx = 1.0f, float magy = 1.0f);
	void DrawNumber2(DWORD num,int x,int y,float z);
	void DrawNumber3(double numd,int x,int y,float z);			//!< �Q�[�W1�p �����`��
	void DrawNumber4(double numd,int x,int y,float z);			//!< �Q�[�W2�p �����`��
	void DrawNumber5(DWORD num,int x,int y,BOOL hits,float z, float magx = 1.0f, float magy = 1.0f);
	void DrawNumber6(DWORD num,int x,int y,float z);
	void DrawNumber7(double numd,int x,int y,float z);			//!< �Q�[�W1�p �����`��
	void DrawNumber8(double numd,int x,int y,float z);			//!< �Q�[�W2�p �����`��
	void DrawNumber9(DWORD num,int x,int y,BOOL hits,float z, float magx = 1.0f, float magy = 1.0f);	//���Ԑ���
	void DrawGObject(GOBJECT* pdat);

	//�I�u�W�F�N�g�����ɕK�v
	Config2_Difficulty	GetCurrentComLevel(){return m_current_com_level;}
	BYTE				GetCurrentDLLID(){return m_crnt_dllid;}

	//DLL�G���[����
	virtual void Notify_Exception(CGObject *obj,DWORD msgid,DWORD prm);	//!< DLL����O���N�������Ƃ��̏���
	static const TCHAR* MessageID2String(DWORD id);						//!< �I�u�W�F�N�g���b�Z�[�W��ID���A������ɕϊ�
	const TCHAR* GetBattleStateString();									//!< �������ID���A������ɕϊ����ĕԂ�
	virtual void SetCurrentDLLID(BYTE dllID)	{m_crnt_dllid=dllID;}	//!< CGObject�̐������ɌĂ΂�ADLL��ID��Ԃ�
	virtual void SetCurrentDLLIDSystem()		{m_crnt_dllid=0;}		//!< CGObject��DLL�̊֐����Ăяo���O��ɌĂ΂�A���݂�DLLID��ݒ肷��

protected:
	//Initialize����R�[��
	virtual void InitializeObjectList() = 0;							//!< �I�u�W�F�N�g���X�g�̏�����
	virtual void InitializeDLLLoadInfo();								//!< DLL���[�h���ɓn���\���̂�����������
	virtual void InitializeLoadDLLs();									//!< DLL�����[�h����
	virtual void InitializeSubTasks() = 0;								//!< �������Ɏg���x���N���X����������
	virtual void InitializeParameters() = 0;							//!< �����J�n�O�p�����[�^������

	//Terminate����R�[��
	virtual void TerminateObjectList() = 0;								//!< �I�u�W�F�N�g���X�g�j��
	virtual void TerminateUnloadDLLs();									//!< DLL�A�����[�h
	virtual void TerminateDestroySubTasks() = 0;						//!< �������Ɏg�����x���N���X����j��

	virtual void StartRound()=0;										//!< ���E���h�J�n���p�����[�^���Z�b�g����

	virtual void Atari(DWORD a_id,DWORD k_id,MY2DVECTOR &kas_point)=0;	//!< �U���q�b�g������
	virtual void HitStop(DWORD len,DWORD oid)=0;						//!< �q�b�g�X�g�b�v�G�t�F�N�g����

	//for debug text out
	void DrawCharacterState();
	void DrawState();
	void DrawObjectList();

	//�ϐ�
public:
	int limittime;						//!< ��������(�}�C�i�X�́�)
protected:
	HINSTANCE hlib_c[2][MAXNUM_TEAM];	//!< �L�����N�^�[��DLL �C���X�^���X�n���h��
	HINSTANCE hlib_s;					//!< �X�e�[�W��DLL �C���X�^���X�n���h��

	//�T�E���h
	LPDIRECTSOUNDBUFFER dsb_round;		//!< �T�E���h �u���E���h�`�v
	LPDIRECTSOUNDBUFFER dsb_fight;		//!< �T�E���h �u�t�@�C�g�v
	LPDIRECTSOUNDBUFFER dsb_ko;			//!< �T�E���h �u�j�n�v
	LPDIRECTSOUNDBUFFER dsb_timeover;	//!< �T�E���h �u�^�C���I�[�o�[�v

	//���E���h�ʂ��ĕω����Ȃ��p�����[�^
	CDI_CHARACTERINFO2 m_cinfo[2][MAXNUM_TEAM];	//!< DLL�Ƃ���肷��L�����N�^�[���
	SDI_STAGEINFO2 m_sinfo;						//!< DLL�Ƃ���肷��X�e�[�W���
	DWORD charobjid[2][MAXNUM_TEAM];			//!< �L�����N�^�[�̃I�u�W�F�N�gID
	DWORD active_character[2];					//!< �A�N�e�B�u�ȁi���ݐ퓬���́j�L�����N�^�[��Index
	DWORD stgobjid;								//!< �X�e�[�W�̃I�u�W�F�N�gID

	//�����J�n���Ƀ��Z�b�g
	int round;							//!< ���E���h��
	DWORD wincount[2];					//!< ������
	DWORD strikercount[2];				//!< �x���U���\��

	//���E���h�J�n���Ƀ��Z�b�g
	BattleTaskState bf_state;			//!< �����̏��
	DWORD bf_counter;					//!< ���C���J�E���^
	DWORD bf_hitdisp[2];				//!< �q�b�g���J�E���g�\���J�E���^

	//DLL���[�h���ɐݒ�B
	//DLL���[�h���g_config�̒l�ɌŒ�
	Config2_Difficulty m_current_com_level;//!< CGObject���������Ɏg�p����COM���x��

	/*!
	*	@brief DLL-ID
	*
	*	DLL���[�h���Ɉȉ��̂悤�ɐݒ肵��DLL��Create�֐����Ăяo���B
	*	char1-1:1 , char1-2:2 , char1-3:3 , char2-1:4 , char2-2:5 , char2-3:6 , stage:7
	*	
	*	���̌��CGObject��DLL�̊֐����Ăяo�����тɂ���ID���ACGObject������ID�ɐݒ肷��B
	*	���̊֐��Ăяo�����ŃI�u�W�F�N�g�����������΁A�������ꂽCGObject�͂���ID�����������Ɏ擾����B
	*	�������āA���ׂẴI�u�W�F�N�g��(�ꕔ�̍l���ɂ�����O��������)�ǂ�DLL�Ő������ꂽ���̂�������ł���B
	*	DLL�̊֐����甲�����Ƃ��ɂ́AID��0(system)�Ƃ���B
	*/
	BYTE m_crnt_dllid;
};


