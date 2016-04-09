
/*=======================================================================================

	�X�g�[���[�Z���N�g

=========================================================================================*/
#pragma once

#include "define_const.h"
#include "task_items.h"
#include "storylist.h"
#include "charlist.h"
#include "task_select.h"
#include "task_simpleoptsel.h"

class CTStorySelect;			//�I���N���X�{��
class CTStorySelecterRing;		//�����O��X�g�[���[�I���N���X
class CTStorySelectPreview;		//�X�g�[���[�v���r���[�N���X
class CTStoryBigFace;			//�X�g�[���[�p�f�J��\���N���X
class CTStorySelectBG;			//�X�g�[���[�p�w�i�`��N���X
class CTStoryParamWindow;		//�p�����[�^������E�C���h�E

/*!
*	@brief �X�g�[���[�Z���N�g��ʁE�{��
*	@ingroup Tasks
*
*	�X�g�[���[�Z���N�g��ʗp�̃^�X�N�N���X�B
*/
class CTStorySelect : public CExclusiveTaskBase
{
public:
	void Initialize();
	BOOL Execute(DWORD time);
	void Terminate();
	
	//!���쎞�ɎQ�Ƃ���L�[�ԍ�
	void SetKeyIndex(UINT ki){m_keyindex=ki;}
	UINT m_keyindex;

	void OnSelect(CTStorySelecterRing *ring);
	void OnChange(CTStorySelecterRing *ring);
	void OnParamSelectComplete();
	void OnCancel();

	//�X�g�[���[�I����̃p�����[�^�ύX
	void SelectChangeCharacter(UINT idx,int cindex);
	void SelectChangeColor(UINT idx,int col);
	void SelectChangeOption(UINT idx,DWORD opt);

	CStoryList::CStoryInfo* m_sinfo;
protected:
	BOOL m_first_update;		//!< ����Update�t���O�B�����O�E�v���r���[�ɒl��ݒ肷�邽�߂Ɏg�p
	BOOL m_sselected;			//!< �X�g�[���[�̑I�������t���O
	BOOL m_complete_flag;		//!< �I�������t���O
	UINT m_complete_counter;	//!< �I��������A�^�X�N�𔲂���܂ł̃E�F�C�g�Ɏg�p
	UINT m_selected_story_index;//!< �I�����ꂽ�X�g�[���[�ԍ�

	int m_alts[MAXNUM_TEAM];

	//�I�u�W�F�N�g
	CTStorySelecterRing		*m_ring;	//!< �����O�N���X
	CTStorySelectBG			*m_bg;		//!< �w�i�`��N���X
	CTStoryBigFace			*m_face;	//!< �f�J��N���X
	CTStorySelectPreview	*m_pv;		//!< �v���r���[
	CTStoryParamWindow		*m_paramwin;//!< �p�����[�^�E�B���h�E

	//�`��v���C�I���e�B�i�萔�j
	static int m_pri_ring;
	static int m_pri_brief;
	static int m_pri_bigface;
	static int m_pri_preview;
	static int m_pri_bg;
	static int m_pri_paramwin;
};


/*!
*	@brief �X�g�[���[�I�������O
*	@ingroup Tasks
*/
class CTStorySelecterRing : public CTIconRingBase
{
public:
	BOOL Execute(DWORD time);
	void Hide();

	//�������z�֐��̎���
protected:
	void InitializeSub();					//!< Initialize����R�[�������
public:
	UINT GetSelected();						//!< �����O�ԍ�+�C���f�b�N�X ����A�S�̂ł̃C���f�b�N�X���擾����
protected:
	void InitializeIcons();					//!< �\������A�C�R���摜����������
	void DestroyIcons();					//!< �\������A�C�R���摜��j������
	MYSURFACE* GetIcon(UINT ridx,UINT idx);	//!< �\������A�C�R���摜���擾�iDraw����R�[���j
	DWORD GetColor(UINT ridx,UINT idx);		//!< �A�C�R���`�掞�̒��_�F���w�肷��(Draw����R�[��,����0�̂���)
	void OnButtonDown(DWORD key);			//!< ����iA�{�^���j�������ꂽ�Ƃ��̏���
	void OnChangeIndex();					//!< �����O���܂킳��ăC���f�b�N�X���ω������Ƃ��̏���
	void OnChangeRing();					//!< �����O���ύX���ꂽ�Ƃ��̏���
	UINT GetMaxRing();						//!< �����O�̑������擾����
	UINT GetMaxIndex(UINT ridx);			//!< �w�胊���O�������Ă���A�C�R���̐����擾

	//�ϐ�
protected:
	MYSURFACE** m_icons;

	//�I�r
protected:
	class CTStorySelectBelt : public CTCharacterSelectBelt
	{
	public:
		void Initialize();
		void SetRing(UINT idx,BOOL real_change);//!< real_change��FALSE�Ȃ�L�k���Ȃ�
	protected:
		void UpdateText();
		
	};
	CTStorySelectBelt* m_belt;
};


/*!
*	@brief �X�g�[���[�p�w�i�`��N���X
*	@ingroup Tasks
*
*	CTCharacterSelectBG����h�����Ă邯�ǉ��ɂ��I�[�o�[���C�h�Ƃ�����ĂȂ��݂����B
*/
class CTStorySelectBG : public CTCharacterSelectBG
{
	//���̂܂�
};


/*!
*	@brief �X�g�[���[�I�����f�J��\���N���X
*/
class CTStoryBigFace : public CTCharacterBigFace
{
public:
	void Initialize();
	BOOL Execute(DWORD time);

	void Setup(int *chars,int *colors,int *alts,int num);	//!< �L�����ꊇ�w��
	void ChangeCharacter(int cindex,int idx);				//!< �����L�����F�ύX
	void ChangeColor(int color,int idx);					//!< �����L�����ύX
	void ChangeAlt(int alt,int idx);						//!< Alt�ύX

	void SetDisplayMode(BOOL dark);	//!< dark=TRUE �� �I���L�����ȊO���Â��\��
	void SetLightIndex(int idx);	//!< dark=TRUE ���ɖ��邭�\������L�������w��

protected:
	BOOL m_dark;					//!< �Â�����t���O
	int  m_lighting;				//!< �Â������Ƃ��ɖ��邭����C���f�b�N�X
	UINT m_appear_counter;			//!< �o���J�E���^. Setup���Ƀ��Z�b�g
	int  m_appear_cnt;				//!< �o����

	int m_num;						//!< �\������ׂ��l��
	int m_characters[MAXNUM_TEAM];	//!< �n���ꂽ�L�����N�^
	int m_colors[MAXNUM_TEAM];		//!< �n���ꂽ�J���[

	DWORD GetDispColor(UINT idx);	//!< �`�掞�\���F�w��
};


/*!
*	@brief �X�g�[���[�v���r���[�\���N���X
*	@ingroup Tasks
*/
class CTStorySelectPreview : public CTaskBase
{
public:
	void Initialize();
	BOOL Execute(DWORD time);
	void Terminate();
	void Draw();

	void Change(UINT sindex);//!< �ύX
	void HideText(){m_brief->Hide();}
	void HidePreview();

	//�`��v���C�I���e�B
	int m_draw_priority;
	int m_txt_draw_priority;
	virtual int GetDrawPriority(){return m_draw_priority;}
	virtual void SetPriority(int pri,int txtpri)
		{m_draw_priority = pri;m_txt_draw_priority=txtpri;}

public:
	//�ϐ�
	UINT m_counter;		//!< �o���E�ύX���A�j���[�V�����p�J�E���^
	int m_crnt_sindex;	//!< �X�g�[���[�ԍ��B�}�C�i�X�͖���
	int m_prev_sindex;	//!< �X�g�[���[�ԍ��B�}�C�i�X�͖���
	float y_pos;
	float m_height;

	//!�v���r���[�r�b�g�}�b�v
	MYSURFACE* m_pv;
	MYSURFACE* m_pv_prev;

	//!�`�掞�p�����[�^
	float m_ratio;


	//!�X�g�[���[�T�v�e�L�X�g�`��p�N���X
	class CTStoryBrief : public CTaskBase
	{
	public:
		void Initialize();
		BOOL Execute(DWORD time);
		void Terminate();
		void Draw();

		//�`��v���C�I���`�[
		int m_draw_priority;
		virtual int GetDrawPriority(){return m_draw_priority;}
		virtual void SetPriority(int pri){m_draw_priority = pri;}

		void Set(UINT sindex);
		void Clear();
		void Hide();//!<�B��

	protected:
		int   disp_len;	//!<������\������
		TCHAR* disp_txt;	//!<�\��������
		int   m_sindex;	//!<�X�g�[���[�ԍ��B�}�C�i�X�͖���
		BOOL  m_hideFlag;
		float m_hideMove;
	};
	CTStoryBrief *m_brief;
};


/*!
*	@brief �X�g�[���[�E�p�����[�^������E�C���h�E
*	@ingroup Tasks
*
*	�X�g�[���[�I����ɁA�L�����N�^�[�Ƃ��I�v�V�����Ƃ��I�ԃE�C���h�E�݂����Ȃ��
*/
class CTStoryParamWindow : public CTWindowBase
{
public:
	void Initialize();
	BOOL Execute(DWORD time);
	void Terminate();
	void Draw();

	//!�`��v���C�I���`�[
	int m_draw_priority;
	virtual int GetDrawPriority(){return m_draw_priority;}
	virtual void SetPriority(int pri){m_draw_priority = pri;}
	
	//!���쎞�ɎQ�Ƃ���L�[�ԍ�
	void SetKeyIndex(UINT ki){m_keyindex=ki;}
	UINT m_keyindex;

	UINT GetCurrentCharacter()			//!<�I���N���X�{�̂��A�f�J��N���X�̉摜�𖾂邭����Ƃ��Ɏg�p
		{return m_crnt_cindex;}

public:
	UINT m_num;							//!<�L������
	UINT m_crnt_cindex;					//!<���݃L����
	UINT m_crnt_param;					//!<�p�����^�i 0:�L���� 1:�J���[ 2:�I�v�V���� / 3:OK�{�^���j
	BOOL  m_lock[MAXNUM_TEAM][3];		//!<�I�����֎~����Ă���p�����[�^
	CStoryList::CStoryInfo** m_sinfo;	//!<�X�g�[���[���iSELECT�^�X�N�{�̂̕ϐ��ւ̎Q�Ɓj

	MYSURFACE** m_miniface;
	DWORD GetDrawColor(UINT cidx,UINT param_idx);	//�`�掞�e�L�X�g�J���[�擾

	BOOL m_ok;
	void OkCheck();
	void ToWaitMode();

	//��Ԃ�Exec�֐�
	enum CTStoryParamWindowState
	{
		state_start,
		state_exec,
		state_wait,
		state_hide,
	};
	void Exec_Start();
	void Exec_Exec();
	void Exec_Wait();
	BOOL Exec_Hide();
	CTStoryParamWindowState m_state;

	/*!
	*	@brief �X�g�[���[�I����E�L�����I���T�u�^�X�N
	*/
	class CCharSelecter : public CTCharacterRing
	{
	protected:
		void InitializeIcons(){}		//!< �A�C�R�������A�Ȃɂ����Ȃ�
		void DestroyIcons(){}			//!< �A�C�R���j���A�Ȃɂ����Ȃ�
		void OnButtonDown(DWORD key);	//!< �{�^�������ꂽ�Ƃ��̏���
		void OnChangeIndex(){Notify();}	//!< �����O���܂킳��ăC���f�b�N�X���ω������Ƃ��̏���
		void OnChangeRing() {Notify();}	//!< �����O���ύX���ꂽ�Ƃ��̏���
		MYSURFACE* GetIcon(UINT ridx,UINT idx);	//!< �\������A�C�R���摜���擾�iDraw����R�[���j
	public:
		MYSURFACE** m_miniface;			//!< �~�j��B�{�̃N���X���炢������
		UINT cidx;						//!< �L�����N�^

		void SetInitialValue(int chr);
		BOOL Execute(DWORD time);		//!< HideComplete���A�^�X�N�k�P
		void Terminate();				//!< �e�^�X�N���A�̂���
	protected:
		void Notify();					//!< ChangeIndex/Ring���ʏ���
	};
	/*!
	*	@brief �X�g�[���[�I����E�F�I���T�u�^�X�N
	*/
	class CColorSelecter : public CTWindowBase
	{
	public:
		void Initialize();
		BOOL Execute(DWORD time);
		void Terminate();
		void Draw();

		void SetInitialValue(int col);
		UINT m_keyindex;				//���쎞�ɎQ�Ƃ���L�[�ԍ�
		UINT cidx;						//�L�����N�^

	protected:
		void Notify();					//�ύX�ʒm
		UINT color;						//�F
		BOOL m_end_flag;				//�I���t���O
	};
	/*!
	*	@brief �X�g�[���[�I����E�I�v�V�����I���T�u�^�X�N
	*/
	class COptionSelecter : public CTSimpleOptionSelecter
	{
	public:
		void Draw();
		void SetCIndex(UINT idx){m_cindex2=idx;}
		BOOL Execute(DWORD time);

	protected:
		void EndSelect();

		UINT m_cindex2;
		bool customend;
	};

	//!�T�u�^�X�N����̏I�����b�Z�[�W�󂯎��p
	public:		static void EndSub();
	protected:	static CTStoryParamWindow *m_instance;//��L�֐����ŃC���X�^���X���擾���邽��
};


