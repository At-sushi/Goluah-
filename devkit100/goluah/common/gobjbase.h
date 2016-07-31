/*=====================================================================================

Goluah!! Copyright (C) 2001-2004 aki, 2004-2016 At-sushi, 2014-2016 logger

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

=======================================================================================*/

/*!=======================================================================================

	@file
	@brief Goluah ��{�N���X�Q

	CCharacterInfo	�@�L�����N�^�[���ݒ�N���X
	CGoluahObject	�@Goluah�I�u�W�F�N�g��{�N���X
	CCharacterBase	�@�L�����N�^�[�I�u�W�F�N�g��{�N���X
	CBulletBase		�@��ѓ����{�N���X
	CClassicalBullet�@���򓹋�\���̌݊���ѓ���N���X
	CBulletList		�@�򓹋�X�g�N���X

=========================================================================================*/
#pragma once

/*! 
*	@defgroup GObjectBase
*	@brief Goluah �L�����N�^/�X�e�[�WDLL�����̊�b�N���X�Q
*/

#include "gobject.h"//GOLUAH DLL�C���^�[�t�F�C�X��`

//�r���h�ݒ�

#ifdef COMMONDLL_C100_EXPORTS
	//����DLL���r���h����ꍇ
#ifndef USE_DIRECT3D_DIRECT
#error
#endif
#define RUNTIME_EXPORT __declspec(dllexport)
#else
	//����DLL�𗘗p����L�����N�^DLL���쐬����ꍇ
#ifdef USE_GOLUAH_COMMON_DLL
#pragma comment(lib, "goluah_c100.lib")
#define RUNTIME_BUILD_OFF
#else
	//����DLL�𗘗p���Ȃ��L�����N�^DLL���쐬����ꍇ
#endif
#define RUNTIME_EXPORT
#endif // COMMONDLL_C100_EXPORTS



#if CDI_VERSION < 900
struct CHARACTER_LOAD_OPTION{};
#endif

#if GCD_VERSION < 900
struct GCD_CELL2_070 : public GCD_CELL2 {};
#endif
/*-----------------------------------------------------------------*/


#ifdef _DEBUG
#define dprint	DebugPrintf
#else
#define dprint	DebugPrintfDummy
#endif
extern void RUNTIME_EXPORT DebugPrintf(char* fmt,...);
extern void RUNTIME_EXPORT DebugPrintfDummy(char* fmt,...);

//! �T�E���h�ێ��p�̍\����
/*typedef struct tagMYSOUND
{
	DWORD number; //!< �T�E���h�ԍ�
	LPVOID sound; //!< �T�E���h�ւ̃|�C���^
} MYSOUND, *LPMYSOUND;*/

// �Ȃ����������ɂ��o�[�W�����U���Ă܂��B
#define DTB_VERSION 1100
#define STB_VERSION 1100

/********************************************************
  �L�����N�^�[���ݒ�N���X
  �l�b�g�Ή��F
	�R���X�g���N�^�U�Ԗڂ̈�����TRUE�ɐݒ肵�Ă��������B
�@********************************************************/
class RUNTIME_EXPORT CCharacterInfo  
{
public:
	CCharacterInfo(LPCTSTR CharName = "����������", DWORD CdiVersion = CDI_VERSION, CHARACTER_LOAD_OPTION* Options = NULL,
		int num_Options = 0, DWORD max_option_point = 0, BOOL isNetworkable = FALSE, DWORD DtbVersion = DTB_VERSION);
	virtual ~CCharacterInfo();

	BOOL DllCharacterInfo(LPVOID info);
	void DllCreateCharacter(PVOID info);


private:
	void SetOptionItems(CDI_CHARACTERINFO *pif);
	BOOL m_isNetworkable;
	DWORD version;
	LPTSTR charactername;
	CHARACTER_LOAD_OPTION* m_Options;
	int m_NumOptions;
	DWORD m_MaxPoint;
	BOOL m_Error;
};

/*! @brief �L�����N�^�[�ݒ�}�N��
*	@param cinfo �K�v�ȏ����i�[����CCharacterInfo�N���X�̃C���X�^���X
*	@param Type ���C���L�����N�^�[�Ƃ��Đ�������N���X�̌^
*	@ingroup GObjectBase
*
*	DLL�́A���̃}�N������������֐����O���ɃG�N�X�|�[�g���Ȃ���΂Ȃ�Ȃ��B
*	�K�؂ȃ��W���[����`�t�@�C�����g�p���邱�ƁB
*/
#define SET_CHARACTERINFO(cinfo, Type) \
extern "C" BOOL CharacterInfo(LPVOID info) {\
	return cinfo.DllCharacterInfo(info);\
}\
extern "C" DWORD CreateCharacter(PVOID info) {\
	cinfo.DllCreateCharacter(info);\
	Type* pc = new Type((CDI_CHARACTERINFO2*)info);\
	pc->InitializeCharacter();\
	return(pc->GetObjectID());\
}

/*--------------------------------------------------------------
	Goluah�I�u�W�F�N�g��{�N���X
----------------------------------------------------------------*/
class RUNTIME_EXPORT CGoluahObject
{
	friend class CBulletBase;

public:
	CGoluahObject(BOOL is_effect = FALSE);
	virtual ~CGoluahObject();

	static DWORD gMessageToObject(DWORD msg,LPVOID pd,DWORD prm);
	virtual DWORD Message(DWORD msg,LPVOID pd,DWORD prm);

	DWORD GetObjectID();
	GOBJECT* GetGObject();
	char* GetCharDir() { return g_chardir; }
	char* GetDLLPath() { return g_chardir; }//�X�e�[�W�̏ꍇ�Ȃ񂾂�����킵���̂�
	operator GOBJECT*() { return pdat; }

	static void* operator new(size_t size);
	static void operator delete(void* p, size_t size);

protected:
	virtual DWORD Action();
	virtual void Command();
	virtual DWORD CommandCOM(DWORD wid);
	virtual DWORD TouchA(ATTACKINFO *info,DWORD ta_eid);
	virtual DWORD TouchB(ATTACKINFO *info,BOOL hit);
	virtual DWORD TouchC(ATTACKINFO *info,DWORD tc_eid);
	virtual DWORD Draw();
	virtual DWORD DrawBack();
	virtual DWORD DrawFront();
	virtual void ActionIDChanged();
	virtual BOOL OnChangeRound();
	virtual DWORD ComThink();

	double zurex(double x);
	int zurex(int x);
	void movex(double dx);
	void movex(int dx);

	void AddProperty(DWORD prop);
	void RemoveProperty(DWORD prop);
	void SetProperty(DWORD prop);

	virtual void ChangeAction(DWORD actid);

	// �V�X�e���֘A�̊֐�
	static DWORD GetKey(DWORD keyinput, DWORD interval);
	static int SeekKey(DWORD keyinput, int offset, int delay, DWORD keystate);
	static void SetKatiSerif(DWORD tid, char* serif);
	static void AddEffect(DWORD EffectID, int prm1, int prm2, DWORD prm3);
	static void PlaySysSound(DWORD SoundNo);
	static LPVOID LoadMySound(char* pathname);
	static void PlayMySound(LPVOID psound);
	static void KillMySound(LPVOID psound);
	DWORD Message2System(DWORD mes, DWORD prm);
	static char* GetEnemyName(DWORD tid);
	static char* GetCharacterName(DWORD oid);				//�I�u�W�F�N�g���疼�O����
	static double GetDisplayCenter_X();
	static double GetDisplayCenter_Y();
	static DWORD GetTeamNum();
	static DWORD GetTaisenKeisiki();
	static DWORD GetTeamNum2(DWORD tid);
	static BYTE  GetEnemyFace(DWORD tid);
	static DWORD TID2EnemyTID(DWORD tid);
		   DWORD EnemyTID();
	static BOOL  IsNetwork();
	static void PushTag(const char* tag);
	static void PopTag();
	static void LogError(const char* fmt,...);
	static void LogWarning(const char* fmt,...);
	static void LogDebug(const char* fmt,...);
	static void LogInfo(const char* fmt,...);
	static void BGMPause();
	static void BGMResume();
	static int GetGameSpeed();
	static DWORD GetMaxWin();

	// �I�u�W�F�N�g�֘A�̊֐�
	BOOL   ObjCatch(DWORD eid,DWORD msg_nage);				//�����Ƃ��ő��������
	static GOBJECT* GetInfo(DWORD oid);						//���Q�b�c
	void   AddDamage(DWORD eid,int x,int y);				//(��ɓ����̂Ƃ�)�����I�Ƀ_���[�W��^����
	void   ActIDChanged();									//�s��ID���ω������Ƃ��̏����𖾎��I�ɍs��
	static GOBJECT*  GetActiveCharacter(DWORD tid);			//���݃A�N�e�B�u�ȃL�����N�^�[�̏��
		   GOBJECT*  GetActiveEnemy();						//���݃A�N�e�B�u�ȓG�L�����N�^�[�̏��
	void   Suicide();										//�I�u�W�F�N�g����
	static DWORD  GetMaai_H(DWORD oid, DWORD eoid);			//�ԍ�����get(����)
	static DWORD  GetMaai_V(DWORD oid, DWORD eoid);			//�ԍ�����get(����)
	       DWORD  GetMaaiFromMe_H(DWORD eoid)				//!��������̊ԍ�����get(����)
				{ return GetMaai_H(GetObjectID(), eoid); }
	       DWORD  GetMaaiFromMe_V(DWORD eoid)				//!��������̊ԍ�����get(����)
				{ return GetMaai_V(GetObjectID(), eoid); }
	static DWORD  GetCharacterID(DWORD tid,DWORD index);	//�L�����N�^�[�̃I�u�W�F�N�gID
	       DWORD  GetComLevel();							//COM���x���擾
	       void   SetComReach(DWORD idx,int length);		//COM���[�`�ݒ�
	       void   SetComAct(DWORD aid,int delay);			//COM�s���ݒ�
	       void   ComDelay(int delay);						//COM�s���}��
		   BOOL	  IsLocal();
		   BOOL   IsRemote(){return !IsLocal();}
   	static void dact_damages1(GOBJECT *p);
	static void dact_damages2(GOBJECT *p);
	static void dact_damages3(GOBJECT *p);
	static void dact_damagec1(GOBJECT *p);
	static void dact_damagec2(GOBJECT *p);
	static void dact_damagec3(GOBJECT *p);
	static void dact_damagej1(GOBJECT *p);
	static void dact_damagej2(GOBJECT *p);
	static void dact_damagej3(GOBJECT *p);
	static void dact_damages1a(GOBJECT *p);
	static void dact_damages2a(GOBJECT *p);
	static void dact_damages3a(GOBJECT *p);
	static void dact_damagec1a(GOBJECT *p);
	static void dact_damagec2a(GOBJECT *p);
	static void dact_damagec3a(GOBJECT *p);
	static void dact_guards1(GOBJECT *p);
	static void dact_guards2(GOBJECT *p);
	static void dact_guards3(GOBJECT *p);
	static void dact_guardc1(GOBJECT *p);
	static void dact_guardc2(GOBJECT *p);
	static void dact_guardc3(GOBJECT *p);
	static void dact_guardj1(GOBJECT *p);
	static void dact_guardj2(GOBJECT *p);
	static void dact_guardj3(GOBJECT *p);
	static void dact_down(GOBJECT *p);
	static void dact_down2(GOBJECT *p);
	static void dact_okiagari(GOBJECT *p);
	static void dact_futtobi(GOBJECT *p);
	static void dact_futtobi2(GOBJECT *p);
	static void dact_tatakituke1a(GOBJECT *p);
	static void dact_tatakituke1b(GOBJECT *p);
	static void dact_tatakituke2a(GOBJECT *p);
	static void dact_tatakituke2b(GOBJECT *p);

	       void   SetComRange(DWORD idx);					//COM�����W�ݒ�
	static DWORD  GetKeyInput(DWORD oid);					//�I�u�W�F�N�g����L�[ID����
	static DWORD GetHitCount(DWORD oid);				//!< �A���Z�q�b�g���i��R���{���j�擾
	static DWORD GetSexyDamage(DWORD oid);				//!< �A���Z�~�σ_���[�W�i��R���{�_���[�W�j�擾


	//�@�`��֘A�̊֐��̒�`
	static LPDIRECT3D9 GetD3D();							//IDirect3D* �̎擾
	static LPDIRECT3DDEVICE9 GetD3DDevice();				//IDirect3DDevice* �̎擾
	static void   LoadCellData(char* pathname,GCD_CELL2_070* pCells,GCD_RECT* pRects,GCD_HANTEI* pHanteis);//�Z���f�[�^�ǂݍ��݊֐��Bver0.90�ȍ~�ł͔񐄏�
	static MYSURFACE* LoadBitmap(char* PathName,char* PalletFileName = NULL);			//�r�b�g�}�b�v�ǂݍ��݊֐�
	static void   UnloadBitMap(MYSURFACE* Bitmap);			//�r�b�g�}�b�v��n���֐�
	static void   CellDraw(MYSURFACE** pBmps,GCD_CELL2* cdat,GCD_RECT* rdat,		//�Z���`��֐�
		DWORD cnum,int x,int y,float z,int Rotate,BOOL ReverseX,BOOL ReverseY,DWORD Color,float magx,float magy,BOOL shadowed = 1);
	static void	  CellDraw(GOBJECT *objdat,BOOL shadowed = 1);
	static void   CkBlt(MYSURFACE* pBmp,int x1,int y1,RECT bltrect,
		double magx,double magy,BOOL revx,BOOL revy,float z,DWORD color);			//Blt2
	static void   Blt3D(MYSURFACE* pBmp,RECT bltrect,MYRECT3D rect,DWORD color);	//Blt3
	static void   SetTransform(BOOL BonVoyage);				//�ϊ��s��ݒ�
	static void   SetBlend(DWORD type);						//�u�����h�̎d����ύX
	static void   SetParentMatrix(LPD3DXMATRIX pMatrix,BOOL erase,LPD3DXMATRIX OldMatrix);	//�u�e�v�̕ϊ��s��ݒ�
	static DWORD  CreateCellData(char* filename,GCD_CELL2 **cdat,GCD_RECT **rdat,GCD_HANTEI **hdat);//�Z���f�[�^�ǂݍ��݁Bver0.90�ȍ~�ł�LoadCellDat���炱����ɕύX����ׂ�
	static void   DestroyCellData(GCD_CELL2 **cdat,GCD_RECT **rdat,GCD_HANTEI **hdat);		//CreateCellDat�Ő������ꂽ�o�b�t�@���N���A
	static MYSURFACE* LoadImage(char* PathName, char* PalletFileName);	// �摜�ǂݍ��݊֐��APNG�Ή��ɔ����ǉ��B
	static DWORD  CreateCellData2(char* filename,GCD_CELL2 **cdat,GCD_RECT **rdat,GCD_HANTEI **hdat);// �Z���f�[�^�ǂݍ��݁i�Q�j�A���k�`���Ή��ɔ����ǉ��B

protected:
	float base_z;
	char *g_chardir;
	GOBJECT *pdat;

private:
	DWORD oid;
	static CGoluahObject* pObjToDelete ;
	static CGoluahObject* pObjDeleting ;
};

inline DWORD CGoluahObject::GetObjectID(){return(oid);}
inline GOBJECT* CGoluahObject::GetGObject(){return(pdat);}

/*!
*	@brief �I�u�W�F�N�g�쐬�}�N��
*	@param pointer �쐬�����I�u�W�F�N�g�̃A�h���X���i�[����|�C���^
*	@param obj �쐬����I�u�W�F�N�g�̃N���X��
*	@ingroup GObjectBase
*
*	�I�u�W�F�N�g�������̐����}�N���ł��B
*	�I�u�W�F�N�g�쐬�̑��A�G���[�����Ȃǂ������ōs���Ă���܂��B
*
*	���������R���X�g���N�^���Ăяo���ꍇ�A�N���X���Ɠ����ɐݒ肵�Ă��������B
*/
#define CREATE_GOBJ(pointer, obj)	{ pointer = new obj; if ( pointer && !pointer->GetGObject() )\
										{ delete pointer; pointer = NULL; }\
									}

/*!
*	@brief �G�t�F�N�g�p�I�u�W�F�N�g�쐬�}�N��
*	@param obj �쐬����I�u�W�F�N�g�̃N���X��
*	@ingroup GObjectBase
*
*	�G�t�F�N�g�I�u�W�F�N�g�������̐����}�N���ł��B
*	�I�u�W�F�N�g�쐬�̑��A�G���[�����Ȃǂ������ōs���Ă���܂��B
*	�����ō폜�����\���̂���N���X�Ȃ̂ŁA�|�C���^�ϐ��ւ̊i�[�͏o���܂���B
*
*	���������R���X�g���N�^���Ăяo���ꍇ�A�N���X���Ɠ����ɐݒ肵�Ă��������B
*/
#define CREATE_GOBJ_FX(obj)			{ CGoluahObject* pointer = new obj; if ( pointer && !pointer->GetGObject() )\
										{ delete pointer; }\
									}


/**********************************************************************

�@�L�����N�^�[�I�u�W�F�N�g��{�N���X

***********************************************************************/

class RUNTIME_EXPORT CCharacterBase : public CGoluahObject
{
public:
	CCharacterBase(CDI_CHARACTERINFO2* info);
	CCharacterBase(CCharacterBase *parent = NULL);
	virtual void InitializeCharacter();

	virtual ~CCharacterBase();
	
	virtual DWORD Message(DWORD msg,LPVOID pd,DWORD prm);//�K�v������΃I�[�o�[���C�h���܂�
	DWORD TouchA(ATTACKINFO *info,DWORD ta_eid);

private:
	void CCharacterBaseCreate();//���ꂼ���Create�ŋ��ʂ̏���������

protected:

	virtual DWORD Action();				//�ʏ� �K���I�[�o�[���C�h���܂�
	virtual void PreAction();			//�ʏ� �I�[�o�[���C�h����K�v�͂���܂���
	virtual void PostAction();			//�ʏ� �I�[�o�[���C�h����K�v�͂���܂���
	virtual void Command();				//�ʏ� �I�[�o�[���C�h����K�v�͂���܂���
	virtual DWORD CommandCOM(DWORD wid);//�ʏ� �I�[�o�[���C�h����K�v�͂���܂���

	//�������������B�K�v�ȕ������I�[�o�[���C�h���Ďg�p���܂�
	virtual void InitParameters()=0;	//�I�v�V�����EHP���ݒ菈���B�ʏ�I�[�o�[���C�h���Ďg�p���܂�
	virtual void InitAttackInfo();		//�U���͏��̏����� �ʏ�K���I�[�o�[���C�h���Ďg�p���܂�
	virtual void InitWazInfo();			//�Z���̐ݒ� �ʏ�K���I�[�o�[���C�h���Ďg�p���܂�
	virtual void InitBullets(){}		//�򓹋���������B�K�v������΃I�[�o�[���C�h���Ďg�p���܂�

	virtual BOOL ChainCombo(DWORD chainid);//�`�F�[���R���{���� �K�v������΃I�[�o�[���C�h���܂�
	virtual BOOL ChainCheck(DWORD chainid);//��L�̃`�F�b�N�̂ݔ�

	DWORD GetKeyInput()		{ return keyinput; }

	//���T�E���h�֌W
	// ���O�ł��ꍇ�̓I�[�o�[���C�h���Ă�������
	// �R���X�g���N�^�̈����œn����num�̂Ԃ񂾂� sound**.wav ���g�p���܂�
protected:
	virtual void InitMySound(char* path_name=NULL,char* list_name=NULL);
	virtual void ReleaseMySound();
public:
	virtual void PlayMySound(DWORD number);

	//���r�b�g�}�b�v��CELL�̓ǂݍ���
	//�@���O�ł��ꍇ�̓I�[�o�[���C�h���Ă�������
protected:
	virtual void InitGCDandBMP();
	virtual void ReleaseGCDandBMP();

	//��CCharacterBase�N���X���s���s���̒�`
	//�ʏ퓮��
	virtual void act_neutral()=0;//�j���[�g����
	virtual void act_crouch()=0;//���Ⴊ��
	virtual void act_rakka()=0;//����
	virtual void act_tojyo()=0;//�o��
	virtual void act_win()=0;//����
	virtual void act_timeoverlose();//�^�C���I�[�o�[
	virtual void act_walkf()=0;//�O�i
	virtual void act_walkb()=0;//���
	virtual void act_jamps()=0;//�W�����v�\������
	virtual void act_jampc()=0;//�����W�����v
	virtual void act_jampf()=0;//�O�W�����v
	virtual void act_jampb()=0;//��W�����v
	virtual void act_rakka2();//����
	//virtual void act_win2_in();//���Ԃ������@��ʊO������ł���
	//virtual void act_win2_pose();//���Ԃ������@�|�[�Y
	//�ʏ�.�U��
	virtual void act_att_sa()=0;//����
	virtual void act_att_sb()=0;//����
	virtual void act_att_sc()=0;//����
	virtual void act_att_ca()=0;//����
	virtual void act_att_cb()=0;//����
	virtual void act_att_cc()=0;//����
	virtual void act_att_ja()=0;//���
	virtual void act_att_jb()=0;//��
	virtual void act_att_jc()=0;//��
	//���̑�
	virtual void act_taiki();
	virtual void act_taikicyu();
	virtual void act_koutai_out();
	virtual void act_koutai_in();
	virtual void act_koutai();
	virtual void act_koutai2();
	virtual void act_strikercomeon();

	// �R�}���h����B�ʏ�I�[�o�[���C�h���܂�
	virtual BOOL Command_Hissatu(DWORD keyinfo);			//�K�E�Z�R�}���h����
	virtual BOOL Command_Normal(DWORD keyinfo);				//��{����
	virtual BOOL Command_OnSystem(DWORD keyinfo);			//�V�X�e����`���쒆
	virtual BOOL Command_OnHissatuAttacking(DWORD keyinfo);	//�K�E�Z��
	virtual BOOL Command_OnAttacking(DWORD keyinfo);		//�U�����쒆
	virtual BOOL Command_OnNormal(DWORD keyinfo);			//��{���쒆

	// �R���s���[�^�R�}���h����B�K�v������΃I�[�o�[���C�h���܂�
	virtual DWORD CmdCom_OnSystem(DWORD wid);				//�V�X�e����`���쒆
	virtual DWORD CmdCom_OnHissatuAttacking(DWORD wid);		//�K�E�Z��
	virtual DWORD CmdCom_OnKucyu(DWORD wid);				//�󒆓��쒆
	virtual DWORD CmdCom_OnNormal(DWORD wid);				//��{���쒆

	//���o�[�R�}���h����֐�
	BOOL com236(int dt);
	BOOL com623(int dt);
	BOOL com214(int dt);
	BOOL com421(int dt);
	BOOL com66(int dt);
	BOOL com44(int dt);
	BOOL com22(int dt);
	BOOL com66i(int dt);
	BOOL com44i(int dt);
	BOOL com22i(int dt);
	BOOL com426(int dt);
	BOOL com236236(int dt);
	BOOL com214214(int dt);
	BOOL com2363214(int dt);
	BOOL com62426(int dt);
	BOOL com6426(int dt);
	BOOL com4268(int dt);
	BOOL com2684(int dt);
	BOOL com6842(int dt);
	BOOL com8426(int dt);
	BOOL com4862(int dt);
	BOOL com2486(int dt);
	BOOL com6248(int dt);
	BOOL com8624(int dt);
	//COM���x������
	BOOL ComLevelCk(UINT level);

	//���̑�����
	//�ʏ�A�I�[�o�[���C�h����K�v�͂���܂���
	virtual BOOL Furimuki();		//�G�̂ق��ɐU�������������܂�
	virtual void ZFront();			//z���W����O���Ɉړ����܂�
	virtual void ZBack();			//z���W�������Ɉړ����܂�
	virtual void JiyuuRakka(double acc_g,BOOL cyakuchi=FALSE,DWORD toaid=ACTID_NEUTRAL);
	virtual void CyakuchiHantei(DWORD toaid=ACTID_NEUTRAL);//���n�̔���
	virtual void LoadAndSetKatiSerif(char* filename=NULL);
	BOOL NageHantei(DWORD maai);	//�����̊ԍ�������
	void AddPowerGauge(double dp);	//�p���[�Q�[�W�����i���/�����`�F�b�N���j
	DWORD GetKey(DWORD interval)	{ return CGoluahObject::GetKey(keyinput, interval); }
	int SeekKey(int offset, int delay, DWORD keystate);

	//���擾�n
	BOOL IsCom()		{return (pdat->objtype&GOBJFLG_COMPUTER) ? TRUE : FALSE;}
	BOOL IsLocalCom()	{ return (IsCom() && IsLocal()); }

protected:
	//�e�퓮��ݒ�
	BOOL chainComboEnabled;		//�`�F�[���R���{��L���ɂ��邩�ǂ���
	DWORD chainlist;			//�`�F�[���R���{�E���łɏo�����Z��ۑ�����ϐ�
	float front_z,back_z;		//ZFront/Back���\�b�h�Ŏw�肳���Z���W
	BOOL doStriker;				//�x���U������炩�����ǂ���
	WAZAINFO waz;				//�Z���\����
	DWORD pal_number;			//�V�X�e������w�肳�ꂽ�p���b�g�ԍ�
	CCharacterBase *parent_char;//�u�e�v�L�����N�^�[
	BOOL isSuperArmer;
	BOOL isAutoGuard;
	DWORD option_flags;

	int base_voice_damage1;
	int base_voice_damage2;
	int base_voice_damage3;
	int base_voice_ko;
	BOOL final_voice_flag;

	MYSURFACE *bitmaps[GCDMAX_IMAGES];
	GCD_RECT *rects;
	GCD_CELL2 *cells;
	GCD_HANTEI *hantei;
	LPVOID* mysounds;
	int* mysound_numbers;
	/*LPMYSOUND mysounds;
	UINT mysound_numbers;*/
	DWORD keyinput;
};

inline int CCharacterBase::SeekKey(int offset, int delay, DWORD keystate) {
	return CGoluahObject::SeekKey(keyinput, offset, delay, keystate);
}

// �h���N���X�̍s��ID�́A���̃t���O���������Ȃ��悤�ɒ�`���邱�Ɓi���ʂ��Ȃ����ǁj
#define ACTID_CHARACTERBASE (0x8000)

#define ACTID_WALKF		(ACTID_CHARACTERBASE | 0x0001)//�O����
#define ACTID_WALKB		(ACTID_CHARACTERBASE | 0x0002)//������
#define ACTID_JAMPS		(ACTID_CHARACTERBASE | 0x0003)//�W�����v�\������
#define ACTID_JAMPC		(ACTID_CHARACTERBASE | ACTID_KUCYU | 0x0004)
#define ACTID_JAMPF		(ACTID_CHARACTERBASE | ACTID_KUCYU | 0x0005)
#define ACTID_JAMPB		(ACTID_CHARACTERBASE | ACTID_KUCYU | 0x0006)
#define ACTID_RAKKA2	(ACTID_CHARACTERBASE | ACTID_KUCYU | 0x0007)

#define ACTID_STRIKERCOMEON	(ACTID_CHARACTERBASE | 0x0008)//�����[

#define ACTID_ATT_SA	(ACTID_CHARACTERBASE | ACTID_ATTACK | 0x0009)
#define ACTID_ATT_SB	(ACTID_CHARACTERBASE | ACTID_ATTACK | 0x000A)
#define ACTID_ATT_SC	(ACTID_CHARACTERBASE | ACTID_ATTACK | 0x000B)

#define ACTID_ATT_CA	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_SYAGAMI | 0x000C)
#define ACTID_ATT_CB	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_SYAGAMI | 0x000D)
#define ACTID_ATT_CC	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_SYAGAMI | 0x000E)

#define ACTID_ATT_JA	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_KUCYU | 0x000F)
#define ACTID_ATT_JB	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_KUCYU | 0x0010)
#define ACTID_ATT_JC	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_KUCYU | 0x0011)

#define ACTID_KOUTAIOUT	(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0012)//���B��ʊO�ɔ��ł��B
#define ACTID_KOUTAIIN	(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0013)//���B��ʓ��ɔ��ł���
#define ACTID_TAIKI		(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0014)//��ʊO�ɔ��ł��đҋ@
#define ACTID_KOUTAI	(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0015)//��ʊO������ł���
#define ACTID_TAIKICYU	(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0016)//�ҋ@����\
#define ACTID_KOUTAI2	(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0017)//����A�|�[�Y�L��

#define ACTID_TIMEOVERLOSE	(ACTID_CHARACTERBASE | 0x0018)

#define ACTID_SYORI2IN		(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0019)
#define ACTID_SYORI2POSE	(ACTID_CHARACTERBASE | ACTID_INOUT | 0x001A)

//chain combo
#define CHAIN_SA	0x00010000
#define CHAIN_SB	0x00020000
#define CHAIN_SC	0x00040000
#define CHAIN_CA	0x00080000
#define CHAIN_CB	0x00100000
#define CHAIN_CC	0x00200000

#define ADDEFFECT(a,b,c,d)	funcs->addeffect(a,b,c,d)


/***************************************************************
�@��ѓ����{�N���X
�@�Ȃ񂩂��܂����g�����舫�����J��

�@�ɗ͎����r���Ő������Ȃ��ł�������
****************************************************************/

//�s��ID�̒�`
#define CBB_STATE_FLAG		0x00008000
#define CBB_STATE_IDLE		(CBB_STATE_FLAG | 0x00000100)//act_idle
#define CBB_STATE_RUN		(CBB_STATE_FLAG | 0x00000200)//act_run
#define CBB_STATE_BOMB		(CBB_STATE_FLAG | 0x00000300)//act_bomb
#define CBB_STATE_RUN2		(CBB_STATE_FLAG | 0x00000400)//act_run2

class RUNTIME_EXPORT CBulletBase : public CGoluahObject
{
public:
	CBulletBase(GOBJECT *parent = NULL);		//�R���X�g���N�^(1)
	CBulletBase(CCharacterBase *parent);		//�R���X�g���N�^(2) ����

	virtual ~CBulletBase(){}

	virtual BOOL Go(BOOL muki,double x,double y,double vx,double vy=0);
	virtual void Bomb();//���œ���ɋ����ڍs���܂�
	virtual void Hide();//�����I�ɑҋ@��Ԃɂ��܂�
	void SetPos(double x,double y){GetGObject()->x=x;GetGObject()->y=y;}
	virtual DWORD Draw();

	void SetHitMessage(DWORD msgid,DWORD param){hitmsg=msgid;hitprm=param;}
	void SetGuardMessage(DWORD msgid,DWORD param){grdmsg=msgid;grdprm=param;}
	void SetAttackInfo( ATTACKINFO* patk){atkinfo=*patk;}
	BOOL isReady(){return(GetGObject()->aid==CBB_STATE_IDLE);}

	GOBJECT* GetParentObj()				{ return parent_obj; }
	CCharacterBase* GetParentClass()	{ return parent_class; }

private:
	void CBulletBaseCreate();//���ʏ���������

protected:
	//�K�v������΃I�[�o�[���C�h���܂�
	virtual DWORD Message(DWORD msg,LPVOID pd,DWORD prm);
	virtual DWORD Action();
	virtual DWORD TouchB(ATTACKINFO *info,BOOL hit);
	virtual DWORD Sousai(DWORD prm);	//���򓹋�Ƒ��E�����Ƃ��ɌĂ΂�܂�
	virtual void Hit();					//����Ƀq�b�g�����Ƃ��Ă΂�܂�(TouchB)
	virtual BOOL OnChangeRound();		//���E���h�ω��������B���������Ȃ����TRUE��Ԃ�

	//�s���֐��ł��B�I�[�o�[���C�h���Ďg���܂�
	virtual void act_idle();
	virtual void act_run()=0;
	virtual void act_bomb()=0;
	virtual void act_run2(){}

	//�I�[�o�[���C�h�̕K�v�͑����Ȃ�
	void Suicide();

	//�T�E���h�Đ��B�e�̃T�E���h�@�\�𗬗p���܂�
	//�R���X�g���N�^(2)�ō쐬�����ꍇ�̂ݎg�p�\�ł�
	virtual void PlayMySound(DWORD number);

protected:
	ATTACKINFO atkinfo;//�U���͏��\����

	DWORD hitmsg,hitprm;
	DWORD grdmsg,grdprm;

	GOBJECT *parent_obj;
	CCharacterBase *parent_class;
};


/***************************************************************
�@������ѓ���N���X
�@�́X�̃\�[�X���ė��p���邽�߂ɍ���Ă݂�

�@�l�b�g�Ή��F�l�b�g�̂�
****************************************************************/

struct MYATTACKINFO//���U���͏��\����
{
	DWORD hit;		//�q�b�g���B���̃t���O(HITINFO_�`)�̘_���a���w��
	DWORD guard;	//�K�[�h���B���̃t���O(GUARDINFO_�`)�̘_���a���w��
	DWORD damage;	//�_���[�W
	DWORD kezuri;	//���

	//�ȉ��g���Ă��܂���B�_�~�[�ł�
	DWORD objid;	//�U�����s�����I�u�W�F�N�g��ID
	DWORD id;		//�L�����N�^���ŏ���ɒ�`����
	double pg_hit;	//�U���q�b�g���̃Q�[�W������
	double pg_guard;//�U���K�[�h���ꂽ�Ƃ��̃Q�[�W������
	BOOL muki;		//���肪�̂��������
	DWORD dur;
};

struct BULLETINFO_A//���򓹋�\����
{
	MYATTACKINFO atk;
	int dur;//��������
	DWORD type;//���ɂ���t���O���w��
	int *cell_run;
	int *cell_dis;
	DWORD spd_run;
	DWORD spd_dis;
	double vx, vy;
	double ax, ay;

	//�ȉ��A�g���Ă܂���B�_�~�[�ł��B
	BOOL muki;
	double x;
	double y;
	DWORD state;
	DWORD cid;
	LPVOID info_b;
};

class RUNTIME_EXPORT CClassicalBullet : public CBulletBase
{
public:
	//!�R���X�g���N�^
	CClassicalBullet( CCharacterBase *parent,	//!< �L�����N�^�N���X
					BULLETINFO_A *bulinfo		//!< ��ѓ�����
					);

	//!���R���X�g���N�^
	CClassicalBullet( CCharacterBase *parent,	//!< �L�����N�^�N���X
					CDI_CHARACTERINFO2 *info,	//!< ���g�p(����) NULL�ł悢
					BULLETINFO_A *bulinfo,		//!< ��ѓ�����
					BYTE userID = 0xFF,			//!< ���g�p
					BYTE userNo = 0xFF			//!< ���g�p
					);
	virtual ~CClassicalBullet();
	
	virtual BOOL Go(BOOL muki,double x,double y);
	virtual void Hit();

protected:
	virtual void act_run();
	virtual void act_run2();
	virtual void act_bomb();

	void act_run_base();

	double vx,vy,ax,ay;
	int *pcr;
	int *pcd;
	DWORD array_len_r;
	DWORD array_len_d;
	DWORD spd_r;
	DWORD spd_d;
	DWORD lifedur;
	DWORD flags;
};

//�����ƃT�|�[�g����Ă邩�ǂ����s��
#define BULLETA_VSHUMAN		0x00000001//�G�L�����N�^�[�ɓ�����
#define BULLETA_VSBULLET	0x00000002//�G��ѓ���Ƒ��E����
#define BULLETA_DONOTDIE	0x00000004//�Փˌ�����ł��Ȃ��Ŕ��ł���
#define BULLETA_DRAWBACK	0x00000008//�l���������ɕ`�悷��
#define BULLETA_DRAWMIDDLE	0x00000040//�L�����N�^�[�E�q�b�g�}�[�N�Ԃɕ`��
#define BULLETA_XJIMENN		0x00010000//�n�ʂɗ���������Ə��ł���
//#define BULLETA_XGAMENGAI	0x00020000//��ʊO�ɒB����Ə���
//#define BULLETA_JIMAESYORI	0x10000000//info_b�����o���L���ł���A���������O�ōs��
#define BULLETA_DISPZAHYO	0x00000010//���W�w��̓f�B�X�v���C��̍��W�B�X�e�[�W��̕\���ʒu�ɍ��E����Ȃ�
#define BULLETA_DONOTSTOP	0x00000020//���K����stop�G�t�F�N�g���������Ă��Ă��`�悪�~�܂�Ȃ�

/********************************************************
  ��ѓ���X�g�N���X
  �l�b�g�Ή��F�l�b�g�̂�

  ����A������ł��������H(aki) �� ��������
�@********************************************************/

class RUNTIME_EXPORT CBulletList  
{
public:
	CBulletList();
	CBulletList(CBulletBase* pBullet);
	virtual ~CBulletList();

	virtual void Add(CBulletBase* pBullet);
	/*!
		@brief ��ѓ���̔��ˍ��W�ݒ�
		@param x ����X���W
		@param y ����Y���W
		@sa GetNext

		Go�܂���Go_All�ɂč��W���ȗ������ꍇ�ɁA
		����Őݒ肵�����W���g�p����܂��B

		�Ȃ��A����Őݒ�ł���̂̓��X�g�̍ŏ��̗v�f�����ł��i�΁j�B
		��̕��Ɏg���ꍇ�́AGetNext���g���Ă��������B
	*/
	void SetPos(double x, double y)			{ this->x = x; this->y = y; }
	/*!
		@brief ��ѓ���̔��ˑ��x�ݒ�
		@param vx X�����̑��x
		@param vy Y�����̑��x
		@sa GetNext

		Go�܂���Go_All�ɂ�vx��vy���ȗ������ꍇ�ɁA
		����Őݒ肵�����x���g�p����܂��B
		Go_All���g�p����ꍇ�͂قڕK�{�̍��ڂł��B

		���x�̂��Ƃł����A����Őݒ�ł���̂̓��X�g�̍ŏ��̗v�f�����ł��i�΁j�B
		��̕��Ɏg���ꍇ�́A��͂�GetNext���g���Ă��������B
	*/
	void SetSpeed(double vx, double vy=0)	{ this->vx = vx; this->vy = vy; }
	virtual BOOL Go(BOOL muki);
	virtual BOOL Go(BOOL muki, double x, double y);
	virtual BOOL Go(BOOL muki, double x, double y, double vx, double vy=0);
	virtual void Go_All(BOOL muki, double x, double y);
	virtual void Go_All(BOOL muki);
	/*!
		@brief ���X�g�̎��̗v�f������
		@return ���̗v�f������CBulletList�ւ̃|�C���^�A�����ꍇ��NULL
		@sa SetPos
		@sa SetSpeed

		�Ăяo�����́A���̗v�f��\��CBulletList�\���̂��������܂��B
		���̗v�f���Ȃ��ꍇ��NULL��Ԃ��܂��B
		�v�f���Ɛ�p�̃N���X�������ACBulletList�����̂܂ܘA�������Ă���͎̂��̎蔲���ł��i�΁j�B

		��ɁA���X�g�����ɑ������Ȃ���SetPos��SetSpeed��K�p���邽�߂Ɏg���܂��B
		�r���̃N���X������ɉ�������肷��Ɓi�E�`�E�j��Ų!!
	*/
	CBulletList* GetNext()		{ return pNext; }
	/*!
		@brief ���X�g�̗v�f��������
		@return ���X�g�Ɋ܂܂�Ă���v�f�̐�

		���X�g���́A�S�Ă̗v�f�����������܂��B
	*/
	int GetCount()	{ return ListCount; }
	/*!
		@brief �擪�̔�ѓ��������
		@return ���X�g�̐擪�Ɋ܂܂�Ă����ѓ���̃|�C���^
		@sa GetNext

		���X�g�̐擪�ɂ����ѓ�����������܂��B�g�����Ƃ͂���܂�Ȃ������B
		GetNext���g���΁A�擪�ȊO�̂��̂������o���܂��B
	*/
	CBulletBase* GetBullet()	{ return m_pBullet; }
	//CBulletList* operator [](int num);

protected:
	double x;
	double y;
	double vx;
	double vy;
	int ListCount;
	CBulletBase* m_pBullet;
	CBulletList* pNext;
};


/********************************************************
	�}�N����`
**********************************************************/

#define ALLGUARDFLAG		(GUARDINFO_XSTAND | GUARDINFO_XCROUCH | GUARDINFO_XJAMP)
#define CANNOT_GUARD(a)		((a&ALLGUARDFLAG)==0)



/********************************************************
	�G�t�F�N�g�x�[�X
**********************************************************/
/*!
*	@brief �G�t�F�N�g�x�[�X
*	@ingroup GObjectBase
*
*	�������ɕp�ɂɐ����E�j�����s���Ă��ǂ��I�u�W�F�N�g�B
*	���̃N���X�ɂ�萶�������I�u�W�F�N�g��(�I�u�W�F�N�g�t���O�Ɋւ�炸�A
*	�V�X�e���I��)���I�u�W�F�N�g�Ƃ̂����蔻�肪�s���Ȃ��B
*	�܂��A���̃I�u�W�F�N�g�����I�u�W�F�N�g�ɑ΂��ĉ��炩�̑��ݍ�p
*	�i���̃N���X����������邱�Ƃ������j���s�����Ƃ͂��@�x�ł��B
*/
class RUNTIME_EXPORT CEffectBase : public CGoluahObject
{
public:
	CEffectBase();
	virtual ~CEffectBase();

protected:

	/*!
	*	@brief GOBJMSG_ACTION��������
	*
	*	CEffectBase�͏�Ԃ𖳎����A�J�E���^��SetDuration�ŗ^����ꂽ�l����time���v�Z����
	*	Update���Ăяo���܂��B
	*	�J�E���^��SetDuration�ŗ^����ꂽ�l�𒴂���ƃI�u�W�F�N�g���ł̎葱�����s���A
	*	�N���X�͎����I��delete����܂��B
	*/
	virtual DWORD Action();

	//!���E���h�ω��ŏ���
	virtual BOOL OnChangeRound(){return FALSE;}

	/*!
	*	@brief �������Ԃ̐ݒ�
	*	@sa CEffectBase::Update
	*	
	*	�ʏ�̓R���X�g���N�^�����炱�̊֐����Ăяo���A�ݒ���s���Ă��������B
	*/
	void SetDuration(UINT dur);

	/*!
	*	@brief �X�V����
	*	@param time 0�`1�̒l
	*
	*	�p���N���X�ł͂��̊֐����I�[�o�[���C�h���ď������������܂��B
	*	time��SetDuration�ŗ^�����������Ԃ̊Ԃ�0.0f�`1.0f�̒l���n����܂��B
	*	�p���N���X��Action�����������������ꍇ�͂��̊֐��̓���͕s��ł��B
	*	"���"�𕡐����������ꍇ�͂��̊֐����g�p�����A���O��Action����̏����������Ă��������B
	*/
	virtual void Update(float time) = 0;

private:
	float ceffectbase_private_duration;
};


/********************************************************
	�X�e�[�W�x�[�X
**********************************************************/

/*!
*	@brief �X�e�[�W�x�[�X�N���X
*	@ingroup GObjectBase
*
*	�X�e�[�W�̃x�[�X...�Ƃ����Ă�CGoluahObject����ǉ����ꂽ�T�[�r�X�͂قƂ�ǂȂ��B
*	SET_STAGEINFO�}�N�����g�p���ăG�N�X�|�[�g����֐����������������ꍇ��
*	�����̊J�n�O��InitializeStage���R�[�������̂ŏ����������͂����ɋL�q���邱�ƁB
*	�`���DrawBack���I�[�o�[���C�h���čs�����ƁB
*	���炩�̍X�V�������s���ꍇ��Action���I�[�o�[���C�h���čs�����ƁB
*/
class RUNTIME_EXPORT CStageBase : public CGoluahObject
{
public:
	CStageBase(SDI_STAGEINFO2 *info);
	virtual ~CStageBase();

	/*!	@brief ������
	*
	*	�p���N���X�͂��̊֐����I�[�o�[���C�h���ď������������L�q���邱�ƁB
	*/
	virtual void InitializeStage() = 0;

	/*!
	*	@brief �֐��|�C���^�̏�����
	*
	*	CreateStage�֐��ŁA�N���X�̃C���X�^���X�����O��
	*	�O���[�o���̊֐��|�C���^������������K�v������B
	*	���̊֐������̎葱�����s���B
	*/
	static void SetupStageDLL(SDI_STAGEINFO2 *info);

	/*!
	*	@brief ���C�u�����o�[�W�����`�F�b�N
	*
	*	�x�[�X�N���X�̃o�[�W�����`�F�b�N���s���܂��B
	*	���ʃ��C�u�������g�p����ꍇ�A������g���ƃo�[�W�����̃Y��������ł��邩���B
	*	�}�N��������Ɏg���̂ŁA���[�U���Ŏg���K�v�͂���܂���B
	*/
	static BOOL CheckSTB(DWORD Version = STB_VERSION);

protected:
	/*!
	*	�X�e�[�W�͕��ʁAGOBJMSG_DRAW�ɑΉ�����ʏ�`��͍s��Ȃ��B
	*	������DrawBack���I�[�o�[���C�h���ĕ`����s������
	*/
	virtual DWORD Draw(){return TRUE;}
};


/*!
*	@brief �ݒ�p�}�N��(�X�e�[�W)
*	@param Type �X�e�[�W�Ƃ��Đ�������CStageBase�p���N���X�̌^
*	@ingroup GObjectBase
*
*	DLL�́A���̃}�N������������֐����O���ɃG�N�X�|�[�g���Ȃ���΂Ȃ�Ȃ��B
*	�K�؂ȃ��W���[����`�t�@�C�����g�p���邱�ƁB
*/
#define SET_STAGEINFO(Type) \
extern "C" BOOL StageInfo(LPVOID info) {\
	((SDI_STAGEINFO*)info)->ver = SDI_VERSION;\
	return CStage::CheckSTB();\
}\
extern "C" DWORD CreateStage(PVOID info) {\
	CStage::SetupStageDLL((SDI_STAGEINFO2*)info);\
	Type* pstgobj = new Type((SDI_STAGEINFO2*)info);\
	pstgobj->InitializeStage();\
	return(pstgobj->GetObjectID());\
}

/*!
*	@brief �ݒ�p�}�N��(�X�e�[�W�A�o�[�W�����`�F�b�N�@�\�t��)
*	@param Type �X�e�[�W�Ƃ��Đ�������CStageBase�p���N���X�̌^
*	@param Ver ���C�u�����̃o�[�W����
*	@ingroup GObjectBase
*
*	DLL�́A���̃}�N������������֐����O���ɃG�N�X�|�[�g���Ȃ���΂Ȃ�Ȃ��B
*	�K�؂ȃ��W���[����`�t�@�C�����g�p���邱�ƁB
*/
#define SET_STAGEINFO_VERCHK(Type, Ver) \
extern "C" BOOL StageInfo(LPVOID info) {\
	((SDI_STAGEINFO*)info)->ver = SDI_VERSION;\
	return CStage::CheckSTB(Ver);\
}\
extern "C" DWORD CreateStage(PVOID info) {\
	CStage::SetupStageDLL((SDI_STAGEINFO2*)info);\
	Type* pstgobj = new Type((SDI_STAGEINFO2*)info);\
	pstgobj->InitializeStage();\
	return(pstgobj->GetObjectID());\
}

class RUNTIME_EXPORT CShadowEffect : public CEffectBase
{
public:
	CShadowEffect(CCharacterBase* pParent,
				  DWORD color = 0x00888888,
				  DWORD BlendType = GBLEND_HANTOUMEI,
				  UINT Duration = 20);

	virtual ~CShadowEffect(void);

protected:
	void Update(float time);
	DWORD Draw(void);

	DWORD m_color;
	DWORD m_BlendType;
};

/*class CGBitmap
{
public:
	CGBitmap() { pBmp = NULL; }
	CGBitmap(char* FileName, char* PalName = NULL) { CGBitmap(); LoadBitmap(FileName, PalName); }
	void LoadBitmap(char* FileName, char* PalName = NULL) { Abonnu(); pBmp = CGoluahObject::LoadBitmap(FileName, PalName); }
	operator MYSURFACE*() { return pBmp; }
	virtual ~CGBitmap() { Abonnu(); }

private:
	void Abonnu() { if (pBmp) CGoluahObject::UnloadBitMap(pBmp); }
	MYSURFACE* pBmp;
};

class CGCD
{
public:
	CGCD() { pRect = pCell = pHantei = NULL; }
};*/

/*!@}*/