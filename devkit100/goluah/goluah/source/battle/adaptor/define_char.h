
#define DLL_VERSION	420

struct CHARACTERINFO
{
	TCHAR dir[32];
	TCHAR name[32];
	LPDIRECTDRAWSURFACE dds_face;
	LPDIRECTDRAWSURFACE dds_face2;
	int storyindex,storynum;
	DWORD ver;
};

typedef void (*PFUNC_BASIC)(void);

//system > dll function =====================================================================
typedef DWORD (*PFUNC_GETKEY)(DWORD,DWORD);			//�L�[����get
typedef void  (*PFUNC_FURIMUKI)(DWORD);				//�U���������
typedef void  (*PFUNC_STARTATTACK)(DWORD,LPVOID);	//�U���J�n
typedef int   (*PFUNC_SEEKKEY)(DWORD,int,int,DWORD);//�L�[����get
typedef void  (*PFUNC_SETKATISERIF)(DWORD,TCHAR*);	//�����䎌�ݒ�
typedef void  (*PFUNC_PLAYMYSOUND)(DWORD,DWORD);	//wav�Đ�
typedef void  (*PFUNC_GOBULLETA)(LPVOID);			//��ѓ����
typedef void  (*PFUNC_ADDEFFECT)(DWORD,int,int,DWORD);//�G�t�F�N�g����
typedef void  (*PFUNC_SETWAZAINFO)(DWORD,LPVOID);	//�R���s���[�^�Ɏg����Z��������
typedef void  (*PFUNC_DRAWFRONT)(DWORD,BOOL);		//�����̕�����O(��)�ɕ\��
typedef BOOL  (*PFUNC_CATCHENEMY)(DWORD,LPVOID);	//�����Ƃ��ő��������
typedef void  (*PFUNC_ENEMYNAME)(DWORD,TCHAR*);		//����̖��Oget
typedef void  (*PFUNC_GETENEMYDAT)(DWORD,LPVOID);	//����̏��get
typedef void  (*PFUNC_GETMAAI)(DWORD,DWORD*,DWORD*,DWORD*);//�ԍ�����get�icid,����,����,�G��ԁj
typedef void  (*PFUNC_PLAYSYSSOUND)(DWORD);			//wav�Đ�
typedef void  (*PFUNC_SETWAZAINFO2)(DWORD,LPVOID);	//�R���s���[�^�Ɏg����Z��������(2)
typedef void  (*PFUNC_SETCHARACTERINFO3)(DWORD,LPVOID);//CHARACTERINFO3�̍\���̂�n��
typedef void  (*PFUNC_ADDDAMAGE)(DWORD,LPVOID,int x,int y);		//(��ɓ����̂Ƃ�)�����I�Ƀ_���[�W��^����
typedef void  (*PFUNC_EXTWINDURATION)(DWORD);		//�����|�[�Y�̕\�����Ԃ���������
typedef BOOL  (*PFUNC_ADDPOWERGAUGE)(DWORD,double); //�p���[�Q�[�W�𑝂₷�A�܂��͌��炷�B�Q�[�W������Ȃ��ꍇFALSE���Ԃ�
typedef void  (*PFUNC_DESTROYBULLET)(LPVOID);		//��ѓ������������
typedef TCHAR* (*PFUNC_GETDIRECTORY)(DWORD);		//�f�B���N�g���[�Q�b�g

struct FUNCTIONPACK
{
	PFUNC_GETKEY			pf_getkey;
	PFUNC_FURIMUKI			pf_furimuki;
	PFUNC_STARTATTACK		pf_startattack;
	PFUNC_SEEKKEY			pf_seekkey;
	PFUNC_SETKATISERIF		pf_setkatiserif;
	PFUNC_PLAYMYSOUND		pf_playmysound;
	PFUNC_GOBULLETA			pf_gobulleta;
	PFUNC_ADDEFFECT			pf_addeffect;
	PFUNC_SETWAZAINFO		pf_setwazainfo;
	PFUNC_DRAWFRONT			pf_drawfront;
	PFUNC_CATCHENEMY		pf_catchenemy;
	PFUNC_ENEMYNAME			pf_enemyname;
	PFUNC_GETENEMYDAT		pf_getenemydat;
	PFUNC_GETMAAI			pf_getmaai;
	PFUNC_PLAYSYSSOUND		pf_playsyssound;
	PFUNC_SETWAZAINFO2		pf_setwazainfo2;
	PFUNC_SETCHARACTERINFO3	pf_setcharinfo3;
	PFUNC_ADDDAMAGE			pf_adddamage;
	PFUNC_EXTWINDURATION	pf_extwindur;
	PFUNC_ADDPOWERGAUGE		pf_addpowgauge;
	PFUNC_DESTROYBULLET		pf_destroybullet;
	PFUNC_GETDIRECTORY		pf_getdirectory;
};

//Characer Info===============================================================================

//dll > system function
typedef DWORD (*PFUNC_CHARACTERINFO)(LPVOID);
typedef DWORD (*PFUNC_CREATECHAR)(DWORD,LPVOID);
typedef BOOL  (*PFUNC_COMMANDB)(DWORD);
typedef DWORD (*PFUNC_TOUCH)(LPVOID);
typedef DWORD (*PFUNC_TOUCH2)(LPVOID,BOOL);

struct CHARACTERINFO2
{
	HINSTANCE hlib;
	DWORD cid;		//0 or 1
	PFUNC_BASIC		initchar;//���E���h�J�n���ɌĂ΂��֐�
	PFUNC_BASIC		command_a;
	PFUNC_COMMANDB	command_b;
	PFUNC_BASIC		action;
	PFUNC_TOUCH		touch_a;//�G�̍U�������������Ƃ��ɌĂ΂��֐�
	PFUNC_TOUCH2	touch_b;//�����̍U�������������Ƃ��ɌĂ΂��֐�
	PFUNC_BASIC		deletechar;//�L�����N�^�[�n�����ɌĂ΂��֐�

	int hp;
	double gauge;

	DWORD aid;
	DWORD cnow;
	DWORD counter;
	double x,y,vx,vy;
	BOOL muki;
	BOOL revx;//x�������]�\��
	BOOL revy;//y�������]�\��
	BOOL muteki;//���G���ǂ���
	BOOL nagerare;//������ꔻ��

	FUNCTIONPACK *fpack;
};

//return value of touch-a
#define TOUCHA_KURAIS	0x10000001//������炢
#define TOUCHA_KURAIC	0x10000002//����炢
#define TOUCHA_KURAIJ	0x10000003//�󒆋�炢
#define TOUCHA_GUARDS	0x20000001//�����K�[�h
#define TOUCHA_GUARDC	0x20000002//���K�[�h
#define TOUCHA_GUARDJ	0x20000003//�󒆃K�[�h
#define TOUCHA_MUSI		0x50000000//�U���͋��������ǁA�f�t�H���g������s��Ȃ�
#define TOUCHA_MUSI2	0x60000000//�U�����K�[�h�������ǁA�f�t�H���g������s��Ȃ�

//�L�����N�^�[���========================================================================
// pf_setcharinfo3 �ɓn���B�L�����N�^�[�̐ݒ���s���B

struct CHARACTERINFO3
{
	DWORD hpmax;//�̗͂̍ő�l�B�f�t�H���g��1024

	DWORD pgauge;//�ǂ̃p���[�Q�[�W���g�p���邩�B���̃t���O�B�f�t�H���g�̓Q�[�W�����B
	DWORD pgaugemax;//�Q�[�W�̃}�b�N�X�l 1-9�̒l���w��B�^�C�v2�̃Q�[�W�͊֌W�Ȃ��B
	double ppgauge_hit;//����̍U�����������Ƃ��ɑ�������Q�[�W�ʁB
	double ppgauge_guard;//����̍U�����K�[�h�����Ƃ��ɑ�������Q�[�W�ʁB
};

#define CINFO3_POWGAUGE1	0x00000001//9�܂ŃX�g�b�N�\�ȃQ�[�W
#define CINFO3_POWGAUGE2	0x00000002//�X�g�b�N�ł��Ȃ��Q�[�W

//ACTION ID===============================================================================
//�s��ID�́A�����ɒ�`���Ă���l�Ƃ��Ԃ�Ȃ��悤�ɐݒ肷�邱��
//						          //xxx0 0000 00xx 0xxx | xxxx xxxx xxxx xxxx
#define ACTID_KUCYU		0x00010000//0000 0000 0000 0001 | 0000 0000 0000 0000
#define ACTID_SYAGAMI	0x00020000//0000 0000 0000 0010 | 0000 0000 0000 0000
#define ACTID_ATTACK	0x00100000//0000 0000 0001 0000 | 0000 0000 0000 0000
#define ACTID_HISSATU	0x00200000//0000 0000 0010 0000 | 0000 0000 0000 0000
#define ACTID_SYSTEM	0x80000000//1000 0000 0000 0000 | 0000 0000 0000 0000
#define ACTID_KURAI		0x40000000//0100 0000 0000 0000 | 0000 0000 0000 0000
#define ACTID_GUARD		0x20000000//0010 0000 0000 0000 | 0000 0000 0000 0000
#define ACTID_NAGE		0x00040000//0000 0000 0000 0100

#define OLDACTID_NEUTRAL	0				//�j���[�g�����|�[�Y
#define OLDACTID_TOJYO		0x0001			//�o��
#define OLDACTID_SYORI		0x0002			//����
#define ACTID_CROUCH	ACTID_SYAGAMI	//���Ⴊ�݃|�[�Y
#define ACTID_RAKKA		ACTID_KUCYU		//����

//default action id
#define ACTID_DAMAGE1		(ACTID_SYSTEM | ACTID_KURAI | 0x0001)				 //������炢�i���j
#define ACTID_DAMAGE2		(ACTID_SYSTEM | ACTID_KURAI | 0x0002)				 //������炢�i���j
#define ACTID_DAMAGE3		(ACTID_SYSTEM | ACTID_KURAI | 0x0003)				 //������炢�i��j
#define ACTID_DAMAGEC1		(ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0001)//���Ⴊ�݋�炢�i���j
#define ACTID_DAMAGEC2		(ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0002)//���Ⴊ�݋�炢�i���j
#define ACTID_DAMAGEC3		(ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0003)//���Ⴊ�݋�炢�i��j
#define ACTID_DAMAGEJ1		(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU   | 0x0001)//�󒆋�炢�i���j
#define ACTID_DAMAGEJ2		(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU   | 0x0002)//�󒆋�炢�i���j
#define ACTID_DAMAGEJ3		(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU   | 0x0003)//�󒆋�炢�i��j
#define ACTID_DOWN			(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0004)//�������œ]��
#define ACTID_DOWN2			(ACTID_SYSTEM | ACTID_KURAI | 0x0004)				 //�_�E�\
#define ACTID_OKIAGARI		(ACTID_SYSTEM | 0x0001)								 //����������
#define ACTID_FINALDOWN		(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0005)//��
#define ACTID_FUTTOBI		(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0006)//�ӂ��Ƃ�
#define ACTID_BOUND			(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0007)//�o�E���h
#define ACTID_FINALBOUND	(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0008)//�o�E���h
#define ACTID_FINALDOWN2	(ACTID_SYSTEM | ACTID_KURAI | 0x0005)				 //��
#define ACTID_FUTTOBI2		(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0009)//�ӂ��Ƃ�2
#define ACTID_NAGERARE		(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | ACTID_NAGE | 0x0001)//������ꒆ
#define ACTID_DAMAGE1A		(ACTID_SYSTEM | ACTID_KURAI | 0x0006)				 //������炢�i���j�A�̂�����Ȃ�
#define ACTID_DAMAGE2A		(ACTID_SYSTEM | ACTID_KURAI | 0x0007)				 //������炢�i���j�A�̂�����Ȃ�
#define ACTID_DAMAGE3A		(ACTID_SYSTEM | ACTID_KURAI | 0x0008)				 //������炢�i���j�A�̂�����Ȃ�
#define ACTID_DAMAGEC1A		(ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0004)//���Ⴊ�݋�炢�i���j�A�̂�����Ȃ�
#define ACTID_DAMAGEC2A		(ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0005)//���Ⴊ�݋�炢�i���j�A�̂�����Ȃ�
#define ACTID_DAMAGEC3A		(ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0006)//���Ⴊ�݋�炢�i��j�A�̂�����Ȃ�
#define ACTID_TATAKITUKE1A	(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000A)//�^���@����������(�o�E���h�L��)
#define ACTID_TATAKITUKE2A	(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000B)//�^���@����������(�o�E���h����)
#define ACTID_TATAKITUKE1B	(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000C)//�΂ߒ@����������(�o�E���h�L��)
#define ACTID_TATAKITUKE2B	(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000D)//�΂ߒ@����������(�o�E���h����)

#define ACTID_GUARD1	(ACTID_SYSTEM | ACTID_GUARD | 0x0001)				 //�����K�[�h�i���j
#define ACTID_GUARD2	(ACTID_SYSTEM | ACTID_GUARD | 0x0002)				 //�����K�[�h�i���j
#define ACTID_GUARD3	(ACTID_SYSTEM | ACTID_GUARD | 0x0003)				 //�����K�[�h�i��j
#define ACTID_GUARDC1	(ACTID_SYSTEM | ACTID_GUARD | ACTID_SYAGAMI | 0x0001)//���Ⴊ�݃K�[�h�i���j
#define ACTID_GUARDC2	(ACTID_SYSTEM | ACTID_GUARD | ACTID_SYAGAMI | 0x0002)//���Ⴊ�݃K�[�h�i���j
#define ACTID_GUARDC3	(ACTID_SYSTEM | ACTID_GUARD | ACTID_SYAGAMI | 0x0003)//���Ⴊ�݃K�[�h�i��j
#define ACTID_GUARDJ1	(ACTID_SYSTEM | ACTID_GUARD | ACTID_KUCYU   | 0x0001)//�󒆃K�[�h�i���j
#define ACTID_GUARDJ2	(ACTID_SYSTEM | ACTID_GUARD | ACTID_KUCYU   | 0x0002)//�󒆃K�[�h�i���j
#define ACTID_GUARDJ3	(ACTID_SYSTEM | ACTID_GUARD | ACTID_KUCYU   | 0x0003)//�󒆃K�[�h�i��j

//�L�[���͒�`===========================================================================
//����͂��܂�g��Ȃ��łق�����
#define KEYSTA_LEFT		(0x00000100)
#define KEYSTA_LEFT2	(0x00000200)
#define KEYSTA_RIGHT	(0x00001000)
#define KEYSTA_RIGHT2	(0x00002000)
#define KEYSTA_ALEFT	(0x00000400)
#define KEYSTA_ALEFT2	(0x00000800)
#define KEYSTA_ARIGHT	(0x00004000)
#define KEYSTA_ARIGHT2	(0x00008000)
//����������g�p�\
#define KEYSTA_UP		(0x00000001)
#define KEYSTA_UP2		(0x00000002)
#define KEYSTA_DOWN		(0x00000010)
#define KEYSTA_DOWN2	(0x00000020)
#define KEYSTA_FOWORD	KEYSTA_RIGHT
#define KEYSTA_FOWORD2	KEYSTA_RIGHT2
#define KEYSTA_BACK		KEYSTA_LEFT
#define KEYSTA_BACK2	KEYSTA_LEFT2
#define KEYSTA_BA		(0x00010000)//�{�^��1
#define KEYSTA_BA2		(0x00020000)
#define KEYSTA_BB		(0x00100000)//�{�^��2
#define KEYSTA_BB2		(0x00200000)
#define KEYSTA_BC		(0x01000000)//�{�^��3
#define KEYSTA_BC2		(0x02000000)
#define KEYSTA_BD		(0x10000000)//�{�^��4
#define KEYSTA_BD2		(0x20000000)

//#define KEYSTA_ANYKEY	(0x22220000)


//�U�����=========================================================================

typedef void (*PF_ACTUSERKURAI)(LPVOID pedat);//���[�U�[��`��炢�����̂��߂̊֐��|�C���^�̌^

struct MYATTACKINFO
{
	DWORD hit;		//�q�b�g���B���̃t���O(HITINFO_�`)�̘_���a���w��
	DWORD guard;	//�K�[�h���B���̃t���O(GUARDINFO_�`)�̘_���a���w��
	DWORD id;		//�L�����N�^���ŏ���ɒ�`����
	double pg_hit;	//�U���q�b�g���̃Q�[�W������
	double pg_guard;//�U���K�[�h���ꂽ�Ƃ��̃Q�[�W������

	DWORD damage;	//�_���[�W
	DWORD kezuri;	//���
	DWORD dur;		//��������

	BOOL muki;		//���肪�̂��������
};

//hit
#define HITINFO_REACT1		0x00000001//���ʂ̎�̂�����
#define HITINFO_REACT2		0x00000002//���ʂ̒��̂�����
#define HITINFO_REACT3		0x00000003//���ʂ̋��̂�����
#define HITINFO_DOWN		0x00000004//�_�E�\����i�]�ԁj
#define HITINFO_FUTTOBI		0x00000005//�������
#define HITINFO_FUTTOBI2	0x00000006//������ԁi������Ə�̂ق��ցj
#define HITINFO_REACT1A		0x00000007//��A�̂�����Ȃ�
#define HITINFO_REACT2A		0x00000008//���A�̂�����Ȃ�
#define HITINFO_REACT3A		0x00000009//���A�̂�����Ȃ�

#define HITINFO_MARK1		0x00010000//�q�b�g�����Ƃ��ɂ������Ⴂ�}�[�N��\�����܂�
#define HITINFO_MARK2		0x00020000//�q�b�g�����Ƃ��ɒ����炢�̃}�[�N��\�����܂�
#define HITINFO_MARK3		0x00030000//�q�b�g�����Ƃ��ɂł������}�[�N��\�����܂�
#define HITINFO_MARK4		0x00040000//�q�b�g�����Ƃ��ɂ���ɂł������}�[�N��\�����܂�(������)

#define HITINFO_SNDHIT1		0x00100000//�q�b�g����
#define HITINFO_SNDHIT2		0x00200000//�q�b�g����
#define HITINFO_SNDHIT3		0x00300000//�q�b�g����
#define HITINFO_SNDSHK1		0x00400000//�d����1
#define HITINFO_SNDSHK2		0x00500000//�d����2

#define HITINFO_SIV1		0x01000000//�q�b�g�X�g�b�v�A�Z��
#define HITINFO_SIV2		0x02000000//�q�b�g�X�g�b�v�A�ӂ�
#define HITINFO_SIV3		0x03000000//�q�b�g�X�g�b�v�A����
#define HITINFO_STOP		0x04000000//�q�b�g�X�g�b�v�A�߂��ᒷ

#define HITINFO_EFCTSINDO	0x00001000//�U�������������Ƃ��A�n�ʂ�U��������

//guard
#define GUARDINFO_XSTAND	0x10000000//�����K�[�h�ł��܂���
#define GUARDINFO_XCROUCH	0x20000000//���Ⴊ�݃K�[�h�ł��܂���
#define GUARDINFO_XJAMP		0x40000000//�󒆃K�[�h�ł��܂���

#define GUARDINFO_REACT1	0x00000001//��K�[�h
#define GUARDINFO_REACT2	0x00000002//���K�[�h
#define GUARDINFO_REACT3	0x00000003//���K�[�h

#define GUARDINFO_SIV1		0x01000000//�q�b�g�X�g�b�v�A�Z��
#define GUARDINFO_SIV2		0x02000000//�q�b�g�X�g�b�v�A�ӂ�
#define GUARDINFO_SIV3		0x03000000//�q�b�g�X�g�b�v�A����
#define GUARDINFO_STOP		0x04000000//�q�b�g�X�g�b�v�A�߂��ᒷ

#define GUARDINFO_EFCTSINDO	0x00001000//�U�������������Ƃ��A�n�ʂ�U��������

//���������==========================================================================
//pf_catchenemy ��CATCHYOU�\���̂̃|�C���^�����Ď��s���ATRUE���Ԃ�Γ����J�n
//�n�����֐��� ����̍s���֐��ɂȂ�B�\���Z���͉��̂ق��ɂ���f�t�H���g�Z��ID�𗘗p�ł���

//�߂�l��0�̏ꍇ�͂��ݑ��s�B����ȊO�̒l�Ȃ�΂��ݏI�����̑���̍s��ID�i��̂ق��ɒ�`���Ă��郄�c�j
typedef DWORD (*PF_ACTNAGERARE)(LPVOID pedat);

struct OLDCATCHYOU
{
	PF_ACTNAGERARE pf_nagerare;
};

// ��ѓ����� ========================================================================
//pf_gobulleta �ɂ��̍\���̂̃|�C���^��n���Ɣ�ѓ���ˁB�G�t�F�N�g�Ƃ��Ă��g����B

//��ѓ���Ɏ����s����������̂Ȃ�ΕK�v
typedef void	(*PFUNC_BULAACTION)(LPVOID);//1�t���[����1��Ă΂��
typedef DWORD	(*PFUNC_BULAHIT)(LPVOID);//�Ȃ񂩂ɓ��������Ƃ��ɌĂ΂��B�߂�l��0�ȊO�Ȃ炻�̌���ŏ���
struct BULLETINFO_B
{
	DWORD id;
	PFUNC_BULAACTION	pf_action;
	PFUNC_BULAHIT		pf_hit;
};

//pf_gobulleta �ɂ��̍\���̂̃|�C���^��n���Ɣ�ѓ���ˁB�G�t�F�N�g�Ƃ��Ă��g����B

struct BULLETINFO_A
{
	double x;
	double y;
	BOOL muki;
	MYATTACKINFO atk;
	int dur;//��������

	DWORD cid;//�ǂ��炪���������̂�
	DWORD type;//���ɂ���t���O�𗧂Ă�ׂ�
	int *cell_run;
	int *cell_dis;
	DWORD spd_run;
	DWORD spd_dis;
	double vx;
	double vy;
	double ax;
	double ay;

	//�V�X�e�����ŏ���Ɏg���ϐ��B������K�v�Ȃ�
	DWORD state;//��ԁB�����œ����Ă���Ƃ��́A������0���Ə���
	DWORD counter;
	DWORD cnow;
	DWORD cnow2;

	BULLETINFO_B *info_b;
};

#define BULLETA_VSHUMAN		0x00000001//����L�����N�^�[�ɓ�����
#define BULLETA_VSBULLET	0x00000002//����̏o������ѓ���Ɠ�����
#define BULLETA_DONOTDIE	0x00000004//�Փˎ������ł��Ȃ��Ŕ��ł���
#define BULLETA_DRAWBACK	0x00000008//�l���������ɕ`�悷��
#define BULLETA_XJIMENN		0x00010000//�n�ʂɗ���������Ə��ł���
#define BULLETA_XGAMENGAI	0x00020000//��ʊO�ɒB����Ə��Łi�������j
#define BULLETA_JIMAESYORI	0x10000000//info_b�����o���L���ł���A���������O�ōs���i�������j
#define BULLETA_DISPZAHYO	0x00000010//���W�w��̓f�B�X�v���C��̍��W�B�X�e�[�W��̕\���ʒu�ɍ��E����Ȃ�
#define BULLETA_DONOTSTOP	0x00000020//���K����stop�G�t�F�N�g���������Ă��Ă��`�悪�~�܂�Ȃ�

//�f�t�H���gcell id=====================================================================
#define DCELL_DAMAGES1		1//������炢�|�[�Y
#define DCELL_DAMAGES2		2
#define DCELL_DAMAGES3		3
#define DCELL_DAMAGES4		4
#define DCELL_DAMAGEC1		5//���Ⴊ�݋�炢�|�[�Y
#define DCELL_DAMAGEC2		6
#define DCELL_DAMAGEC3		7
#define DCELL_DAMAGEC4		8
#define DCELL_GUARDS1		9//�����K�[�h�|�[�Y
#define DCELL_GUARDS2		10
#define DCELL_GUARDS3		11
#define DCELL_GUARDS4		12
#define DCELL_GUARDC1		13//���Ⴊ�݃K�[�h�|�[�Y
#define DCELL_GUARDC2		14
#define DCELL_GUARDC3		15
#define DCELL_GUARDC4		16
#define DCELL_DAMAGEJ		17//�󒆋�炢�|�[�Y
#define DCELL_RAKKA1		18//����1
#define DCELL_RAKKA2		19//����2
#define DCELL_GUARDJ1		20//�󒆃K�[�h�|�[�Y
#define DCELL_GUARDJ2		21
#define DCELL_GUARDJ3		22
#define DCELL_BOUND			23//�������ăo�E���h
#define DCELL_DOWN			24//�_�E��
#define DCELL_OKIAGARI1		25//�N���オ�胂�[�V����
#define DCELL_OKIAGARI2		26
#define DCELL_OKIAGARI3		27
#define DCELL_OKIAGARI4		28
#define DCELL_OKIAGARI5		29
#define DCELL_NAGERARE1		30//����/�\���̈ʒu�ɑ̂̏d�S������悤��
#define DCELL_NAGERARE2		31//�E��
#define DCELL_NAGERARE3		32//�^��
#define DCELL_NAGERARE4		33//����
#define DCELL_NAGERARE5		34//�n�ʂɓ��B

//Effect Id ============================================================================
//pf_addeffect�̑������Ɏg����萔�B�������ȍ~�̈Ӗ��̓G�t�F�N�gID�ɂ��قȂ�

							 //�����̈Ӗ�  /�R�����g
#define EFCTID_MARK1		1//x,y,-
#define EFCTID_MARK2		2//x,y,-
#define EFCTID_MARK3		3//x,y,-
#define EFCTID_MARK4		4//(��)
#define EFCTID_MARKG		5//x,y,-
#define EFCTID_MARKG2		6//x,y,-
#define EFCTID_HIKARI		7//x,y,?
#define EFCTID_STOP			8//�X�g�b�v���Ă鎞��,-,-
#define EFCTID_DARK			9//�Â��Ȃ��Ă鎞��,-,-  /�d�����A����܂�g��Ȃ��ق�������
#define EFCTID_MARKC		10//x,y,-  /�����͂񂾂Ƃ��̃}�[�N
#define EFCTID_GNDCIR		11//x,y,-  /�n�ʂɊۂ��Ռ��g
#define EFCTID_SINDO		12//�U��,��������,-  /�U�����N����
#define EFCTID_NOBG			13//��������,-,-  �w�i�̕`��𖳂��ɂ���
#define OLDEFCTID_CYOHI		1000//x,y,�d������ /���K�G�t�F�N�g

//Sound ID =============================================================================
//pf_playsyssound�Ɏg����萔�B�V�X�e�����Ŏ����Ă�����ʉ��𗘗p�ł���

#define SYSTEMSOUND_HIT1	0//hit1.wav �q�b�g��
#define SYSTEMSOUND_HIT2	1//hit2.wav
#define SYSTEMSOUND_HIT3	2//hit3.wav
#define SYSTEMSOUND_SHOCK1	3//shock1.wav �g�����Ǝv���Ύg���邪�A���g�p
#define SYSTEMSOUND_SHOCK2	4//shock2.wav �g�����Ǝv���Ύg���邪�A���g�p
#define SYSTEMSOUND_GUARD	5//guard.wav �K�[�h��
#define SYSTEMSOUND_CATCH	6//catch.wav �͂݉�
#define SYSTEMSOUND_CYOHI	7//cyohi.wav ���K�������̌��ʉ�
#define SYSTEMSOUND_CHARGE	8//charge.wav �Q�[�W��1�{���܂����Ƃ��̌��ʉ�

#define MAXNUM_CHARACTERSOUND 32//�L�����N�^�[���g�p�ł���ő�̃T�E���h��

//Computer Action Info ================================================================
//pf_setwazainfo(2)�Ɏg�p����\����.�R���s���[�^���쎞�Ɏg�p����

struct OLDWAZAINFO
{
	DWORD walkf;//�O�i
	DWORD walkb;//���
	DWORD jampf;//�܂�W�����v
	DWORD jampb;//���W�����v
	DWORD dashf;//�܂�_�b�V��
	DWORD dashb;//���_�b�V��

	DWORD att_short[6];//�Z���[�`�U��
	DWORD att_middle[6];//�����[�`�U��
	DWORD att_long[6];//�����[�`�U��
	DWORD att_jamp[6];//�󒆂ŏo���U��
	DWORD att_tai[6];//�΋�Ɏg����U��
	DWORD att_bullet[6];//��ѓ���Ȃ�
};

struct WAZAINFO2
{
	DWORD nageid[8];	//�����s��ID
	OLDCATCHYOU cy[8];		//�������Ɏg���\����

	int renzoku_maai[16];		//���̘A���Z�ɓK�����ԍ���
	DWORD renzoku[16][2][16];	//�A���Z [16�܂œo�^�\][0:�s��ID�i�Ȃ��ꍇ��0�j/1:���̍s���Ɉڂ�܂ł̎���][1�̘A���Z�ɂ��A16�܂ł̂̋Z]
};
