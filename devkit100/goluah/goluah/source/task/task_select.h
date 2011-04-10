

/*=======================================================================================

	vsCOM �܂��� 1Pvs2P �ł̃L�����Z��

=========================================================================================*/
#pragma once

#include "define_const.h"
#include "task_items.h"
#include "charlist.h"
#include "task_optselecter.h"

class CTCharacterRing;
class CTCharacterBigFace;
class CTCharacterSelectBG;
class CTCharacterSelectBelt;
class CTOptionSelecter;
class CTSimpleOptionSelecter;
class CTStageSelecter;
class CTConditionSelecter;

/*!
*	@brief �L�����Z���N���X�E�x�[�X
*	@ingroup Tasks
*
*	�L�����N�^�[�����O���痘�p���鋤�ʃC���^�[�t�F�C�X���`�����肵���������݂����B
*/
class CCharacterSelectBase : public CExclusiveTaskBase
{
friend class CTCharacterRing;

public:
	virtual ~CCharacterSelectBase(){}
	virtual void OnSelect(CTCharacterRing *pring,int cindex)=0;	//!< �L�����N�^�I���������B�}�C�i�X�̓����_������
	virtual void OnChangeColor(CTCharacterRing *pring)=0;		//!< �F�ύX������
	virtual void OnInstOnOff(CTCharacterRing *pring)=0;			//!< �C���X�gON/OFF������
	virtual void OnOptionSelect(CTOptionSelecter *pselecter,DWORD option)=0;	//!< �I�v�V�����I��������
	virtual void OnStageSelect(CTStageSelecter *spelecter,int sindex)=0;		//!< �X�e�[�W�I��������
	virtual void OnEndAnimComplete(CTaskBase* ptask)=0;			//!< �I���A�j���[�V����������ʒm
	virtual void OnCancel() = 0;
};

/*!
*	@brief �L�����Z���N���X�E�ΐ�p
*	@ingroup Tasks
*/
class CCharacterSelect : public CCharacterSelectBase
{
public:
	CCharacterSelect();
	virtual ~CCharacterSelect(){Terminate();}

	void Initialize();
	BOOL Execute(DWORD time);
	void Terminate();
	void Draw();
	int GetDrawPriority(){return 100;}

	//�L�����N�^�[�����O����̏���
	void OnSelect(CTCharacterRing *pring,int cindex);	//!< �L�����N�^�I��������
	void OnChangeColor(CTCharacterRing *pring);			//!< �F�ύX������
	void OnInstOnOff(CTCharacterRing *pring);			//!< �C���X�gON/OFF������
	void OnOptionSelect(CTOptionSelecter *pselecter,DWORD option);	//!< �I�v�V�����I��������
	void OnStageSelect(CTStageSelecter *spelecter,int sindex);		//!< �X�e�[�W�I��������
	void OnEndAnimComplete(CTaskBase* ptask);						//!< �I���A�j���[�V����������ʒm
	void OnCancel();

	//!ConditionSelecter����̏���
	virtual void OnConditionDecided(CTConditionSelecter *pcsel);

protected:
	void ResolveColor(int team,int index);				//!< �F�̏d������������
	void ResolveRandom();								//!< �d��������ă����_���w�����������
	void SetupBattleInfo();								//!< g_battleinfo�ɑI�����ꂽ���ڂ�ݒ肷��
	//!�L�����N�^�[�I���E�I�v�V�����I���C�x���g��ɃX�e�[�W�I���ɐi�ނ��ǂ����̃`�F�b�N
	void CheckCharacterSelectOK();
	
	//!�p���N���X�p
	virtual void CreateNextTask();

//�ϐ�
protected:

	//�I�����ꂽ���
	BYTE m_assign[2][MAXNUM_TEAM];			//!< �v���C�����蓖��
	int selected_char[2][MAXNUM_TEAM];		//!< �L�����N�^
	int selected_color[2][MAXNUM_TEAM];		//!< �L�����N�^�E�F
	DWORD selected_option[2][MAXNUM_TEAM];	//!< �L�����N�^�E�I�v�V����
	int selected_stage;						//!< �X�e�[�W

	//�i�s�t���O��
	BOOL m_condition_sel_ok;			//!< �v���C�����蓖�Ă͏I��������H
	int wanted_char_num[2];				//!< ���l�I�яo���΂����̂��H
	BOOL charsel_ok[2];					//!< �K�v�l���I�яo���ꂽ���H
	BOOL stgsel_ok;						//!< �X�e�[�W�͊��ɑI�����ꂽ���H
	int num_selected[2];				//!< ���l�I�񂾂��H

	//sub tasks
	CTCharacterSelectBG *m_bg;			//!< �w�i
	CTCharacterRing* m_ring[2];			//!< �����O
	CTCharacterBigFace* m_bface[2];		//!< �f�Jface
	CTCharacterSelectBelt* m_belt[2];	//!< �I�r
	CTSimpleOptionSelecter* m_selecter[2];	//!< �I�v�V�����Z���N�^

	//�`��v���C�I���e�B�ݒ�
	static int m_pri_background;
	static int m_pri_face;
	static int m_pri_belt;
	static int m_pri_cselecter;
	static int m_pri_oselecter;
	static int m_pri_sselecter;
	static int m_pri_firstSetting;

	//!�L�����I���L�[���͊��蓖��
	void AssignKeys(int team);
	BOOL CkKey(int team,BYTE ki);	//!< �`�F�b�N
	BOOL CkAllCom(int team);		//!< �`�F�b�N

	//!�G�`�[���擾
	int AnotherTeam(int team){return team==0 ? 1 : 0;}
};


/*!
*	@brief VS���[�h���L�����N�^�[�I�������O�N���X
*	@ingroup Tasks
*/
class CTCharacterRing : public CTIconRingBase
{
public:
	virtual ~CTCharacterRing(){}

	void AddSelected(DWORD sel)				//!<�I���ς݃L�����N�^�[���X�g�ǉ�
		{m_selectedlist.push_back(sel);}

	//�������z�֐��̎���
protected:
	void InitializeSub();					//!<Initialize����R�[�������
public:
	UINT GetSelected();						//!<�����O�ԍ�+�C���f�b�N�X ����A�S�̂ł̃C���f�b�N�X���擾����
protected:
	virtual void InitializeIcons();					//!<�\������A�C�R���摜����������
	virtual void DestroyIcons();					//!<�\������A�C�R���摜��j������
	virtual MYSURFACE* GetIcon(UINT ridx,UINT idx);	//!<�\������A�C�R���摜���擾�iDraw����R�[���j
	virtual DWORD GetColor(UINT ridx,UINT idx);		//!<�A�C�R���`�掞�̒��_�F���w�肷��(Draw����R�[��,����0�̂���)
	virtual void OnButtonDown(DWORD key);			//!<����iA�{�^���j�������ꂽ�Ƃ��̏���
	virtual void OnChangeIndex();					//!<�����O���܂킳��ăC���f�b�N�X���ω������Ƃ��̏���
	virtual void OnChangeRing();					//!<�����O���ύX���ꂽ�Ƃ��̏���
	virtual UINT GetMaxRing();						//!<�����O�̑������擾����
	virtual UINT GetMaxIndex(UINT ridx);			//!<�w�胊���O�������Ă���A�C�R���̐����擾
	void Draw();									//!<�`��

	//�ǉ����ꂽ�����o
	BOOL IsSelected(DWORD cindex);			//!<�w��L�����N�^�[�����ɑI������Ă��邩�ǂ����`�F�b�N�i�FGet�p�j

	static MYSURFACE** m_dds_miniface;		//!<�L�����N�^�~�j��C���[�W
	static DWORD m_instanceCount;			//!<���𕡐��̃����O�ŋ��L���邽�߂̎Q�ƃJ�E���g
	std::list<DWORD> m_selectedlist;		//!<�I���ς݃L�����N�^�[�̃��X�g

private:
	static UINT sm_selected_ring1p;
	static UINT sm_selected_index1p;
	static UINT sm_selected_ring2p;
	static UINT sm_selected_index2p;
};



class CTCharacterSelectBelt2;
/*!
*	@brief VS���[�h�I�����@�I�v�V�����Z���N�^�[�E�^�X�N
*	@ingroup Tasks
*/
class CTOptionSelecter : public CTOptionSelecterBase
{
public:
	virtual void ApplyToPreviousSelect(){m_selecter->ApplyToPreviousSelect();}
protected:
	virtual void EndSelect();
};


/*!
*	@brief VS���[�h�I�����@�L�����N�^�[�E�f�J�t�F�C�X�\��
*	@ingroup Tasks
*/
class CTCharacterBigFace : public CTaskBase
{
public:
	virtual ~CTCharacterBigFace(){}
	virtual void Initialize();
	virtual void Terminate();
	virtual BOOL Execute(DWORD time);
	virtual void Draw();

	virtual int GetDrawPriority(){return m_draw_priority;}
	virtual void SetPriority(int pri){m_draw_priority = pri;}

	void SetLeftRight(BOOL left){m_left=left;}
	void SetNum(DWORD num){m_max_num = num;}
	void Set(int cid,DWORD color,int alt);
	void SetTemporary(int cid,DWORD color,int alt);
	void ResetNum()		{ m_selected_num = 0; }

protected:
	int m_draw_priority;		//!<�`��v���C�I���e�B
	BOOL m_left;				//!<�������A�E�����H
	UINT m_counter;				//!<���ɉ������̂ɕK�v
	UINT m_counter2;			//!<�I�����ω������Ƃ��ɕK�v
	UINT m_selected_num;		//!<�I���ς݃L�����̐�
	DWORD m_max_num;			//!<�ő�l��

	int m_cindex[MAXNUM_TEAM];	//!<���ݓǂݍ��܂�Ă�����̂��A�ǂ̃L�����N�^�[�̂��̂�
	DWORD m_color[MAXNUM_TEAM];	//!<���ݓǂݍ��܂�Ă�����̂��A�ǂ̐F�̂��̂�
	int m_alt[MAXNUM_TEAM];		//!<�I�v�V�����ɂ��ύX

	//�X�g�[���[�I���̂ق��Ōp�������Ƃ��p�B
	virtual DWORD GetDispColor(UINT idx);	//!<�`��F���擾����
	BOOL m_draw_temporary;					//!<�e���|�����I���L�����N�^��`�悵�Ȃ�
};


/*!
*	@brief VS���[�h�I�����̔w�i�`��
*	@ingroup Tasks
*/
class CTCharacterSelectBG : public CTaskBase
{
public:
	virtual ~CTCharacterSelectBG(){}
	void Initialize();
	void Terminate();
	BOOL Execute(DWORD time);
	void Draw();
	int GetDrawPriority(){return m_draw_priority;}

	void SetPriority(int pri){m_draw_priority = pri;}
	void ChangeState(BOOL f);
protected:
	int m_draw_priority;	//!<�`��v���C�I���e�B
	BOOL  m_state;			//!<TRUE:�L�����Z���F / FALSE:�X�e�[�W�Z���N�g�F
	DWORD m_counter;		//!<�J�E���^
	MYVERTEX3D vbg[4];		//!<���_
	LPDIRECT3DTEXTURE8 ptex_cs1;//!<�w�i�p�e�N�X�`��
};

/*---------------------------------------------------
	�I�r
-----------------------------------------------------*/

/*!
*	@brief CTCharacterSelectBelt�̏��
*	@ingroup Tasks
*/
enum CTCharacterSelectBeltState
{
	CTCSBS_NotReady,
	CTCSBS_Ready,
	CTCSBS_Change,
	CTCSBS_Change2,
	CTCSBS_Hide,
	CTCSBS_Show,
	CTCSBS_HideComplete,
	CTCSBS_Extend,			//�g���O�L�k
	CTCSBS_Extend2,			//�g����
	CTCSBS_Extended,
	CTCSBS_ExtendedHide,	//�g��������Hide��
	CTCSBS_Restore,			//�g��������ʏ퓮���
};

/*!
*	@brief �я�̕`�敨�E�x�[�X
*	@ingroup Tasks
*
*	�L�����I���Ƃ��ŁA�я�̕`�敨���������������̂łȂ�ƂȂ�����Ă݂��B
*	����܂���ʓI�Ɏg���ĂȂ���������Ȃ��B
*/
class CTCharacterSelectBelt : public CTBeltBase
{
public:
	virtual ~CTCharacterSelectBelt(){}
	virtual void Initialize();
	virtual void Terminate();
	virtual BOOL Execute(DWORD time);
	//void Draw();

	virtual int GetDrawPriority(){return m_state==CTCSBS_HideComplete ? -1 : m_draw_priority;}

	//����E�ݒ�
	virtual void SetRing(DWORD ringIndex);
	virtual void SetCom(BOOL com);
	virtual void SetKey(char idx);

	virtual void Show(BOOL b);
	virtual void Extend(float top,float bottom);	//!<�̈�g��
	virtual void Restore();							//!<�ʏ�̏�Ԃɖ߂�

protected:
	virtual void UpdateText();

	CTCharacterSelectBeltState m_state;
	DWORD m_ringIndex;						//!<�����O���\���p
	BOOL  m_com;							//!<�R���s���[�^���ǂ���
	char  m_current_key;					//!<���ݗL���ȃL�[���͔ԍ�
	
	float m_given_top , m_given_bottom;
};


/*---------------------------------------------------
	�X�e�[�W�Z���N�^�[�E�^�X�N
-----------------------------------------------------*/

/*!
*	@brief CTStageSelecter ���
*	@ingroup Tasks
*/
enum CTStageSelecterState
{
	CTSSS_NotReady,
	CTSSS_Ready,
	CTSSS_Change,		//���E�ύX���A�j��
	CTSSS_Change2,		//�㉺�ύX���A�j��
	CTSSS_Hide,			//�������A�j��
	CTSSS_HideComplete, //�I��
};

/*!
*	@brief VS���[�h���@�X�e�[�W�I��
*	@ingroup Tasks
*/
class CTStageSelecter : public CTBeltBase
{
public:
	void Initialize();
	void Terminate();
	BOOL Execute(DWORD time);
	void Draw();

	//�`��v���C�I���e�B
	int GetDrawPriority(){return m_state==CTCSBS_HideComplete ? -1 : m_draw_priority;}

	static DWORD m_previous_selected_ring;
	static DWORD m_previous_selected_index;

	//����
	void AddKeyInput(DWORD ki){m_keyIndexList.push_back(ki);}

protected:
	void LoadBMP();
	int GetSelection();
	void HandlePad();
	void UpdateText();

	CTStageSelecterState m_state;
	DWORD m_counter;
	DWORD m_selected_ring , m_selected_index;
	std::vector<DWORD> m_keyIndexList;		//!<�ǂ̃L�[���͂��Q�Ƃ��邩

	MYSURFACE *m_dds;

	//�`�掞�p�����[�^
	float m_width , m_height;
};




/*!
*	@brief VS���[�h���@�ΐ�R���f�B�V�����I���^�X�N
*	@ingroup Tasks
*/
class CTConditionSelecter : public CTBeltBase
{
public:
	friend class CCharacterSelectBase;
	friend class CCharacterSelect;

	enum CTConditionSelecter_State
	{
		CTCoS_Start,		//�o����
		CTCoS_Execute,		//���s��
		CTCoS_Hide,			//������
		CTCoS_HideComplete,	//��������
	};

public:
	virtual ~CTConditionSelecter(){}
	virtual void Initialize();
	virtual void Terminate();
	virtual BOOL Execute(DWORD time);
	virtual void Draw();

	//�`��v���C�I���e�B
	int GetDrawPriority(){return m_state==CTCoS_HideComplete ? -1 : m_draw_priority;}

protected:
	virtual DWORD TxtCol(int idx){return idx==m_selected ? 0xFF000000 : 0xFF999999;}
	virtual void  Change(BOOL key);

	CTConditionSelecter_State m_state;
	DWORD m_counter;
	int   m_selected;

	static BYTE m_type;						//gobject.h , TAISENKEISIKI_�`�Q�� BYTE�ő����
	static BYTE m_assign[2][MAXNUM_TEAM];	//���蓖��
	static int m_limit_time_index;			//���̃C���f�b�N�X
	static int m_limit_time[5];				//45,60,99,120,��  ,  const

	BOOL m_ok;
	char m_str_error[128];

	void Check();//!< ���蓖�Ă̐��������`�F�b�N����
};


