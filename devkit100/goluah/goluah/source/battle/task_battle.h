
/*============================================================================

	�퓬�^�X�N�N���X

	�i�l�b�g���[�N��Ή��Łj

	Goluah!! Copyright (C) 2001-2004 aki, 2014-2015 logger, 2004-2015 At-sushi

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

==============================================================================*/
#pragma once

#include "define_const.h"
#include "battleTaskBase.h"
#include "gauge.h"
#include "efctlist.h"
#include "task_pause.h"

#define MAXEFCT_SIVER 16
#define DEBUGMSGBUFFERSIZE	(1024*4)


/*!
*	@brief �ʏ�Ő퓬�^�X�N
*	@ingroup Battle
*
*	�I�u�W�F�N�g�̊Ǘ��F
*	�I�u�W�F�N�g��ID�́A�z��̃C���f�b�N�X + ���̃C���f�b�N�X�ł̐����ԍ� + �G�t�F�N�g�I�u�W�F�N�g�t���O�@
*	�ɂ�萶�������BID����C���f�b�N�X��������̂ŁAID����̃I�u�W�F�N�g�̎擾��
*	��r�I�����ɏ����ł���E�E�E�Ǝv���B�@�܂��A�����ԍ��𕍉����邱�Ƃɂ��
*	�����������������Ȃ����蓯��ID�����I�u�W�F�N�g����������邱�Ƃ͂Ȃ��B
*
*	�G�t�F�N�g�I�u�W�F�N�g�t���O�������Ă���ꍇ�͂����蔻��Ȃǂ��s��Ȃ�
*/
class CBattleTask : public CBattleTaskBase
{
public:
	CBattleTask();
	virtual ~CBattleTask();

	//�^�X�N�֐�
	virtual void Initialize();
	virtual BOOL Execute(DWORD time);
	virtual void Draw();
	virtual void WndMessage(HWND hWnd,UINT msg,WPARAM wparam, LPARAM lparam);//!< F7�Ń|�[�Y

	//���擾�n
	virtual double GetDisplayCenterX(){return disp_center_x;}
	virtual double GetDisplayCenterY(){return disp_center_y;}
	virtual BOOL   IsNetwork(){return FALSE;}
	virtual BOOL   IsLocal(BYTE dllid){return TRUE;}

	//void SetKatiSerif(DWORD tid,char *serif);
	virtual DWORD MessageFromObject(DWORD oid,DWORD msg,DWORD prm);
	virtual BOOL CatchObject(DWORD eoid,LPVOID cy);
	virtual void AddEffect(DWORD efctid,int prm1,int prm2,int prm3=0);
	virtual BOOL Atari(DWORD a_id, DWORD k_id, MY2DVECTOR &kas_point);
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

	//�f�o�b�O�p ��ԕ\��
	virtual void DrawState();
	virtual void DrawObjectList();
	virtual void DrawCharacterState();
	virtual void DrawCharacterState2();

protected:
	//�S�̃G�t�F�N�g�֘A
	int efct_slowdown,efct_stop,efct_darkbg,efct_nobg;
	int efct_sindo,efct_sindom;
	int efct_hitstop;
	int efct_fadein;
	int efct_flash;

	//���
	DWORD hprecratio[2][MAXNUM_TEAM];	//!<���R��㐧�̂Ƃ��̗͉̑񕜗�
	BOOL  battle_end;		//!<�^�X�N���k����̂Ɏg�p

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

	//!�|�[�Y���̕`��E����
	CTBattlePause* m_pause_task;

	//!�x���U���v���p�J�E���^
	UINT strikercall_counter[2];

	LPDIRECT3DTEXTURE8 tex_fb;

	//! ���v���C�ۑ��t�@�C���f�[�^
	CFile RepFile;

	/*�������i�s�Ǘ��� ------------------------------------------------------*/
protected:
	//T_UpdateStatus������ɕ���
	virtual void T_UpdateStatus_WaitForEndPose();	//!< �����i�s�Ǘ��E�o��|�[�Y�I���҂�
	virtual void T_UpdateStatus_RoundCall();		//!< �����i�s�Ǘ��E���E���h�R�[���I���҂�
	virtual void T_UpdateStatus_Fighting();			//!< �����i�s�Ǘ��E�퓬���
	virtual void T_UpdateStatus_Finished();			//!< �����i�s�Ǘ��EKO�A�L�����N�^�̍ŏI�_�E���I���҂�
	virtual void T_UpdateStatus_WaitForEndWin();	//!< �����i�s�Ǘ��E�����|�[�Y�I���҂�
	virtual void T_UpdateStatus_DoubleKO();			//!< �����i�s�Ǘ��E�_�u��KO�\���I���҂�
	virtual void T_UpdateStatus_TimeOver();			//!< �����i�s�Ǘ��E�^�C���I�[�o�[�\���I���҂�

	virtual void Update_DeadFlag();					//!< ���S�t���O�X�V

	//�����i�s�Ǘ��t���O
	DWORD winner_oid;								//!< �����|�[�Y�I���҂��Ώ�
	BOOL m_winpose_end;								//!< �����|�[�Y�I���E����l���������ĂȂ�
	BOOL m_finaldown_end[2];						//!< �_�E�������t���O
	BOOL m_tojyo_end[2][MAXNUM_TEAM];				//!< �o��|�[�Y�I���t���O�BKOFLike���̃E�F�C�g�ɂ��g�p
	BOOL m_all_dead[2];								//!< ���S�t���O
	BOOL m_dead_one[2][MAXNUM_TEAM];				//!< ���S�t���O
	BOOL m_active_dead[2];							//!< ���S�t���O
	BYTE m_round_winner;							//!<  0/1:�����`�[�� , 2:�_�u��KO


	/*���I�u�W�F�N�g�Ǘ���-----------------------------------------------------*/
public:
	CGObject* GetGObject(DWORD id);					//!< �w��ID�I�u�W�F�N�g�擾
	DWORD CreateGObject();							//!< �I�u�W�F�N�g����
	DWORD CreateGObjectFx();						//!< �I�u�W�F�N�g����(�G�t�F�N�g)
	void DeleteGObject(DWORD oid);					//!< �I�u�W�F�N�g�j��
	void SuicideGObject(DWORD oid);					//!< �I�u�W�F�N�g�j���\��
	CGObject* GetCharacterObject(DWORD j,DWORD i);	//!< �L�����N�^�[�̃I�u�W�F�N�g�擾�i�Q�[�W���ŕK�v�j
protected:
	std::unordered_map< int, CGObject* > p_objects;	//!< �I�u�W�F�N�g����N���X�̃|�C���^
	DWORD object_regindex;							//!< ���ɐ�������I�u�W�F�N�g�̃C���f�b�N�X
	std::unordered_map< int, WORD > object_regno;	//!< ���̃C���f�b�N�X�ł����̃I�u�W�F�N�g����������Ă�����
	std::deque< DWORD > suicide_list;				//!< ���ŃI�u�W�F�N�g���X�g
};

#define BATTLETASK_FXOBJFLAG		0x80000000
#define BATTLETASK_ISNOTFXOBJ(p)	((p->id & BATTLETASK_FXOBJFLAG) ? FALSE : TRUE)

