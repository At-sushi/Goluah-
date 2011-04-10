/*=======================================================================================

	�\���n�x�[�X

	��ʏ�\�����x�[�X�N���X

=========================================================================================*/
#pragma once

#include "task.h"
#include "define_gcd.h"
#include "define_const.h"

class CTBeltBase;				//�я�E�C���h�E
class CTWindowBase;				//�P���l�p�`�E�C���h�E
class CTIconRingBase;			//�A�C�R�������O�\��&�I��

/*!
*	@ingroup Tasks
*/
/*@{*/

/*!
*	@brief �I�r��`�敨�x�[�X�N���X
*/
class CTBeltBase : public CTaskBase
{
public:
	CTBeltBase();
	virtual ~CTBeltBase(){}

	virtual void Draw();

	//�`��v���C�I���e�B
	int m_draw_priority;
	virtual int GetDrawPriority(){return m_draw_priority;}
	virtual void SetPriority(int pri){m_draw_priority = pri;}

	//�e�푀��
	void SetLeft(){m_pos=0;}
	void SetRight(){m_pos=1;}
	void SetCenter(){m_pos=2;}
	void SetBaseY(float y){m_base_y=y;}
	void SetText(char *str){strcpy(m_disp_str,str);}
	void FormatText(char *str_format,char *str){sprintf(m_disp_str,str_format,str);}
	void SetAlpha1(BYTE a){m_alpha1=a;}
	void SetAlpha2(BYTE a){m_alpha2=a;}
	
protected:
	BYTE  m_pos;				//!< x�ʒu 0:�E 1:�� 2:���� �i�e�L�X�g�E������ �ɉe���j
	
	char  m_disp_str[128];		//!< �\���e�L�X�g
	BOOL  m_show_text;			//!< �e�L�X�g on/off
	BOOL  m_txtTop;				//!< �e�L�X�g����ɔz�u���邩�A���ɔz�u���邩
	float m_txtLeft;			//!< �e�L�X�g�̍��ʒu
	BOOL  m_txtR2L;				//!< �e�L�X�g�E�����`��

	BYTE  m_alpha1,m_alpha2;	//!< ���A�Z���Ƃ��E�����Ƃ�
	DWORD m_lineCol;			//!< ���C���̃J���[�i���ȊO�j
	DWORD m_bodyCol;			//!< �{�̂̃J���[�i���ȊO�j
	DWORD m_txtCol1,m_txtCol2;	//!< �e�L�X�g�J���[�i���܁j

	float m_lineWidth;			//!< �F�����C������

	//y �ʒu���w��p
	float m_top , m_bottom;

	//y �ʒu�Ԑڎw��
	float m_base_y;				//!< y�ʒu
	float m_height_base;		//!< ���펞����
	float m_ratio;				//!< ������
	void CalcTopBottom();		//!< m_top , m_bottom �Z�o(�`��O�ɃR�[�����邱��)

};


/*!
*	@brief �P���l�p�`�E�C���h�E��`�敨�x�[�X�N���X
*/
class CTWindowBase : public CTaskBase
{
public:
	virtual ~CTWindowBase(){}
	virtual void Initialize();
	virtual BOOL Execute(DWORD time);
	virtual void Draw();

	//�`��v���C�I���e�B
	int m_draw_priority;
	virtual int GetDrawPriority(){return (m_ratio<0.01f) ? -1 : m_draw_priority;}
	virtual void SetPriority(int pri){m_draw_priority = pri;}

	//����
	virtual void Open();
	virtual void Close();
	virtual void SetPos(float x,float y){m_winleft=x;m_wintop=y;}

protected:
	//�E�C���h�E�p�����[�^
	float m_ratio;			//�E�C���h�E�傫��
	float m_winwidth;		//�E�C���h�E�̕��i�W�����j
	float m_winheight;		//�E�C���h�E�̍����i�W�����j
	float m_winleft;		//�E�C���h�E�ʒu
	float m_wintop;			//�E�C���h�E�ʒu
	DWORD m_wincolor;		//�`��Ɏg�����_�F
	float m_winOpenTime;	//�E�B���h�E�I�[�v���A�j���[�V�������x
	float m_winCloseTime;	//�E�B���h�E�N���[�Y�A�j���[�V�������x
	
	//�e�L�X�g�`��B�E�C���h�E�̃T�C�Y�ɍ��킹�ĕ`�悵�Ă�������B
	//������CSystem��DrawBMPText�ƈꏏ�����ǁA�ʒu�̓E�C���h�E���ł̈ʒu
	void DrawText(float x,float y,float z,
					char *str,DWORD col,
					float scaX=1.0f,float scaY=1.0f,DWORD flags=SYSBMPTXT_PROP);

	//�`�掞�Ɏ��ۂɎg���l�B�T�u�N���X�ł�CalcDrawParameters���R�[�����Ă���Draw���邱�ƁB
	virtual void CalcDrawParameters();
	float m_drx1,m_drx2,m_dry1,m_dry2;

private:
	//���
	enum CTWindowBaseState
	{
		state_restore,
		state_show,
		state_hide
	};
	CTWindowBaseState m_state;
	UINT m_counter;
};


/*!
*	@brief �����O��A�C�R���\��&�I�� �x�[�X
*/
class CTIconRingBase : public CTaskBase
{
public:
	virtual ~CTIconRingBase(){}
	void Initialize();
	void Terminate();
	BOOL Execute(DWORD time);
	void Draw();

	//��ԑ���
	virtual void Hide();
	virtual void Restore();

	//�p�����[�^����
	virtual void SetHeight(float h){m_height = h;}
	virtual void SetPos(double basePosX,double basePosY){m_basePosX=basePosX/240.0;m_basePosY=basePosY/240.0;}
	virtual void SetKeyInputIndex(int i);
	virtual int GetKeyInputIndex(){return m_keyindex;}
	virtual UINT GetRing(){return m_selected_ring;}

	//�`��v���C�I���e�B
	virtual void SetPriority(int pri){m_draw_priority = pri;}
	virtual int  GetDrawPriority(){return m_state == CTCRS_HideComplete ? -1 : m_draw_priority;}
	int m_draw_priority;

	//�I�[�o�[���C�h����ׂ��֐�
protected:
	virtual void InitializeSub(){}						//!< Initialize����R�[�������
	virtual void TerminateSub(){}						//!< Terminate ����R�[�������
public:
	virtual UINT GetSelected()=0;						//!< �����O�ԍ�+�C���f�b�N�X ����A�S�̂ł̃C���f�b�N�X���擾����
protected:
	virtual void InitializeIcons()=0;					//!< �\������A�C�R���摜����������
	virtual void DestroyIcons()=0;						//!< �\������A�C�R���摜��j������
	virtual MYSURFACE* GetIcon(UINT ridx,UINT idx)=0;	//!< �\������A�C�R���摜���擾�iDraw����R�[���j
	virtual DWORD GetColor(UINT ridx,UINT idx)=0;		//!< �A�C�R���`�掞�̒��_�F���w�肷��(Draw����R�[��,����0�̂���)
	virtual void OnButtonDown(DWORD key)=0;				//!< ����iA�{�^���j�������ꂽ�Ƃ��̏���
	virtual void OnChangeIndex()=0;						//!< �����O���܂킳��ăC���f�b�N�X���ω������Ƃ��̏���
	virtual void OnChangeRing()=0;						//!< �����O���ύX���ꂽ�Ƃ��̏���
	virtual UINT GetMaxRing()=0;						//!< �����O�̑������擾����
	virtual UINT GetMaxIndex(UINT ridx)=0;				//!< �w�胊���O�������Ă���A�C�R���̐����擾

	//��Ԃ�Update�֐�
	enum CTIconRingState
	{
		CTCRS_NotReady,		//!< �o����
		CTCRS_Ready,		//!< �I���\
		CTCRS_RotateRight,	//!< �E��]��
		CTCRS_RotateLeft,	//!< ����]��
		CTCRS_ChangeUp,		//!< �����O��ύX��
		CTCRS_ChangeDown,	//!< �����O���ύX��
		CTCRS_Hide,			//!< ������
		CTCRS_HideComplete,	//!< ���S�ɏ���
		CTCRS_Restore,		//!< ���A�r��
	};
	virtual void Exec_NotReady();
	virtual void Exec_Ready();
	virtual void Exec_RotateRight();
	virtual void Exec_RotateLeft();
	virtual void Exec_ChangeUp();
	virtual void Exec_ChangeDown();
	virtual void Exec_Hide();
	virtual void Exec_Restore();

	//�ϐ�
	int m_keyindex;					//!< �L�[���̓C���f�b�N�X�B�}�C�i�X�őI�������b�N
	UINT m_sleepCount;				//!< �����O�ύX�E���A��̉��t�����̓L�[��������b�N����
	CTIconRingState m_state;		//!< ���
	DWORD m_selected_ring;			//!< ���ݑI������Ă��郊���O
	DWORD m_selected_index;			//!< ���ݑI������Ă��郊���O���̃C���f�b�N�X
	DWORD m_previous_ring;			//!< �O��I������Ă��������O�i�ύX���̏����A�j���[�V�����ɕK�v�j
	DWORD m_previous_index;			//!< �O��I������Ă��������O���̃C���f�b�N�X

	double m_basePosX,m_basePosY;	//!< ��ʒu
	float m_height;					//!< ����
	
	DWORD  m_counter;				//!< �A�j���[�V�����p�J�E���^
	double m_rotation2;				//!< �ǉ���]�idegree�j
	double m_radius_base;			//!< ����a
	double m_radius_ratio;			//!< ���a�{��
	double m_radius_ratio2;			//!< ���a�{���i�����Ă��������O�j
	BYTE   m_alpha;					//!< ���l
	BYTE   m_alpha2;				//!< ���l�i�����Ă��������O�j
};


/*@}*/