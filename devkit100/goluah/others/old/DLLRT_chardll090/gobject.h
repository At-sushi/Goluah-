
/*=============================================================================

	GOLUAH �L�����N�^�[�E�X�e�[�WDLL �̃r���h�ɕK�v�Ȋe��\���̂̒�`

===============================================================================*/
#ifndef _GOBJECT_H_
#define _GOBJECT_H_

//GCD�t�H�[�}�b�g��`���K�v
#ifndef _DEFINE_GCD_H_
#include "define_gcd.h"
#endif

//******************************************************************************
//�@�@�U���͏��
//******************************************************************************
struct ATTACKINFO
{
	DWORD hit;		//�q�b�g���B���̃t���O(HITINFO_�`)�̘_���a���w��
	DWORD guard;	//�K�[�h���B���̃t���O(GUARDINFO_�`)�̘_���a���w��
	DWORD id;		//�L�����N�^���ŏ���ɒ�`����

	DWORD damage;	//�_���[�W
	DWORD kezuri;	//���

	BOOL muki;//���肪�̂��������(FALSE�Ȃ�΁A�����������Ă�������Ɠ���)
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
#define HITINFO_TATAKI1A	0x0000000A//�^���@�����o�E���h�L��
#define HITINFO_TATAKI2A	0x0000000B//�^���@�����o�E���h�Ȃ�
#define HITINFO_TATAKI1B	0x0000000C//�΂ߒ@�����o�E���h�L��
#define HITINFO_TATAKI2B	0x0000000D//�΂ߒ@�����o�E���h�Ȃ�

#define HITINFO_MARK1		0x00010000//�q�b�g�����Ƃ��ɂ������Ⴂ�}�[�N��\�����܂�
#define HITINFO_MARK2		0x00020000//�q�b�g�����Ƃ��ɒ����炢�̃}�[�N��\�����܂�
#define HITINFO_MARK3		0x00030000//�q�b�g�����Ƃ��ɂł������}�[�N��\�����܂�
#define HITINFO_MARK4		0x00040000//�q�b�g�����Ƃ��ɂ���ɂł������}�[�N��\�����܂�

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
#define HITINFO_EFCTBURN	0x00002000//�U������炤�ƃL�����N�^�[���R���o��

//guard
#define GUARDINFO_XSTAND	0x10000000//�����K�[�h�ł��܂���
#define GUARDINFO_XCROUCH	0x20000000//���Ⴊ�݃K�[�h�ł��܂���
#define GUARDINFO_XJAMP		0x40000000//�󒆃K�[�h�ł��܂���
#define GUARDINFO_XAUTO		0x80000000//�I�[�g�K�[�h�ł��Ȃ�

#define GUARDINFO_REACT1	0x00000001//��K�[�h
#define GUARDINFO_REACT2	0x00000002//���K�[�h
#define GUARDINFO_REACT3	0x00000003//���K�[�h

#define GUARDINFO_SIV1		0x01000000//�q�b�g�X�g�b�v�A�Z��
#define GUARDINFO_SIV2		0x02000000//�q�b�g�X�g�b�v�A�ӂ�
#define GUARDINFO_SIV3		0x03000000//�q�b�g�X�g�b�v�A����
#define GUARDINFO_STOP		0x04000000//�q�b�g�X�g�b�v�A�߂��ᒷ

#define GUARDINFO_EFCTSINDO	0x00001000//�U�������������Ƃ��A�n�ʂ�U��������


struct ATTACKINFO2
{
	ATTACKINFO *info1;
	DWORD oid;//�U�����s�������c�̃I�u�W�F�N�gID�B��ʒ[�ł̂��������Ƃ��A����������߂��̂Ɏg�p�B
	DWORD flags;//���Ɏ����t���O
};

#define ATKINFO2_RIGHTBACK		0x00000001//�E���ɂ̂�����
#define ATKINFO2_ATTACKERBACK	0x00000002//��ʒ[�܂œ��B������U��������������߂����������Ȃ���΂Ȃ�Ȃ�

//**************************************************************************************
//�@�R���s���[�^���쎞�Ɏg�p����\����
//**************************************************************************************
struct WAZAINFO
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

	DWORD nageid[6];	//�����s��ID

	//����������
	//�n�ォ��n�܂�A���Z
	int renzoku_maai[8];		//���̘A���Z�ɓK�����ԍ���
	DWORD renzoku[8][16][2];	//�A���Z [#][�Z][0:�s��ID�i�Ȃ��ꍇ��0�j/1:���̍s���Ɉڂ�܂ł̎���]
	//�󒆂���n�܂�A���Z
	int arenzoku_maai[8];		//���̘A���Z�ɓK�����ԍ���
	DWORD arenzoku[8][16][2];	//�A���Z [#][�Z][0:�s��ID�i�Ȃ��ꍇ��0�j/1:���̍s���Ɉڂ�܂ł̎���]
};

//ACTION ID===============================================================================
//�s��ID�́A�����ɒ�`���Ă���l�Ƃ��Ԃ�Ȃ��悤�ɐݒ肷�邱��
//						          //xxxx 0000 00xx 0xxx | xxxx xxxx xxxx xxxx
#define ACTID_KUCYU		0x00010000//0000 0000 0000 0001 | 0000 0000 0000 0000
#define ACTID_SYAGAMI	0x00020000//0000 0000 0000 0010 | 0000 0000 0000 0000
#define ACTID_ATTACK	0x00100000//0000 0000 0001 0000 | 0000 0000 0000 0000
#define ACTID_HISSATU	0x00200000//0000 0000 0010 0000 | 0000 0000 0000 0000
#define ACTID_SYSTEM	0x80000000//1000 0000 0000 0000 | 0000 0000 0000 0000
#define ACTID_KURAI		0x40000000//0100 0000 0000 0000 | 0000 0000 0000 0000
#define ACTID_GUARD		0x20000000//0010 0000 0000 0000 | 0000 0000 0000 0000
#define ACTID_NAGE		0x00040000//0000 0000 0000 0100
#define ACTID_INOUT		0x10000000//0001 0000 0000 0000

#define ACTID_NEUTRAL	1				//�j���[�g�����|�[�Y
#define ACTID_TOJYO		2				//�o��
#define ACTID_SYORI		3				//����
#define ACTID_CROUCH	ACTID_SYAGAMI	//���Ⴊ�݃|�[�Y
#define ACTID_RAKKA		ACTID_KUCYU		//����

//default action id�@=======================================================================
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
#define ACTID_FINALBOUND	(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0008)//�o�E���h(���S)
#define ACTID_FINALDOWN2	(ACTID_SYSTEM | ACTID_KURAI | 0x0005)				 //��
#define ACTID_FUTTOBI2		(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0009)//�ӂ��Ƃ�2
#define ACTID_NAGERARE		(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | ACTID_NAGE | 0x0001)//������ꒆ
#define ACTID_DAMAGE1A		(ACTID_SYSTEM | ACTID_KURAI | 0x0006)				 //������炢�i���j�A�̂�����Ȃ�
#define ACTID_DAMAGE2A		(ACTID_SYSTEM | ACTID_KURAI | 0x0007)				 //������炢�i���j�A�̂�����Ȃ�
#define ACTID_DAMAGE3A		(ACTID_SYSTEM | ACTID_KURAI | 0x0008)				 //������炢�i���j�A�̂�����Ȃ�
#define ACTID_DAMAGEC1A		(ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0004)//���Ⴊ�݋�炢�i���j�A�̂�����Ȃ�
#define ACTID_DAMAGEC2A		(ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0005)//���Ⴊ�݋�炢�i���j�A�̂�����Ȃ�
#define ACTID_DAMAGEC3A		(ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0006)//���Ⴊ�݋�炢�i��j�A�̂�����Ȃ�
#define ACTID_TATAKITUKE1A	(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000A)  //�^���@����������(�o�E���h�L��)
#define ACTID_TATAKITUKE2A	(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000B)  //�^���@����������(�o�E���h����)
#define ACTID_TATAKITUKE1B	(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000C)  //�΂ߒ@����������(�o�E���h�L��)
#define ACTID_TATAKITUKE2B	(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000D)  //�΂ߒ@����������(�o�E���h����)
#define ACTID_KAITENFINISH	(ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000E)  //��]���ĉ�ʊO�ɐ������ł���

#define ACTID_GUARD1	(ACTID_SYSTEM | ACTID_GUARD | 0x0001)				 //�����K�[�h�i���j
#define ACTID_GUARD2	(ACTID_SYSTEM | ACTID_GUARD | 0x0002)				 //�����K�[�h�i���j
#define ACTID_GUARD3	(ACTID_SYSTEM | ACTID_GUARD | 0x0003)				 //�����K�[�h�i��j
#define ACTID_GUARDC1	(ACTID_SYSTEM | ACTID_GUARD | ACTID_SYAGAMI | 0x0001)//���Ⴊ�݃K�[�h�i���j
#define ACTID_GUARDC2	(ACTID_SYSTEM | ACTID_GUARD | ACTID_SYAGAMI | 0x0002)//���Ⴊ�݃K�[�h�i���j
#define ACTID_GUARDC3	(ACTID_SYSTEM | ACTID_GUARD | ACTID_SYAGAMI | 0x0003)//���Ⴊ�݃K�[�h�i��j
#define ACTID_GUARDJ1	(ACTID_SYSTEM | ACTID_GUARD | ACTID_KUCYU   | 0x0001)//�󒆃K�[�h�i���j
#define ACTID_GUARDJ2	(ACTID_SYSTEM | ACTID_GUARD | ACTID_KUCYU   | 0x0002)//�󒆃K�[�h�i���j
#define ACTID_GUARDJ3	(ACTID_SYSTEM | ACTID_GUARD | ACTID_KUCYU   | 0x0003)//�󒆃K�[�h�i��j

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
#define DCELL_NAGERARE1		30//�d�S���L��
#define DCELL_KAITENFUTTOBI	31//��]������сA�d�S���L��


//******************************************************************************
//�@�@�I�u�W�F�N�g�f�[�^
//******************************************************************************

typedef DWORD (*PFOBJ_MSGHANDLER)(DWORD,LPVOID,DWORD);
//����1:���b�Z�[�WID�i���Q�Ɓj
//����2:�I�u�W�F�N�g�̃f�[�^(GOBJECT�̃|�C���^)
//����3:�e���b�Z�[�W�ɂ��Ӗ����قȂ�i�قƂ�ǎg���ĂȂ��j
//�߂�l�F�e���b�Z�[�W�ɂ��Ӗ����قȂ�B0���ƃf�t�H���g�������s���B

struct GOBJECT
{
	DWORD id;			//�I�u�W�F�N�gid
	DWORD uid;			//���[�U�[�����蓖�Ă�id
	DWORD tid;			//�`�[��id
	DWORD eid;			//�G��id
	PFOBJ_MSGHANDLER msghandler;//���b�Z�[�W�����֐�
	DWORD objtype;		//�I�u�W�F�N�g�̎�����
	
	double x,y;			//�ʒu
	double vx,vy;		//���x
	double ax,ay;		//�����x
	
	DWORD counter;		//�J�E���^
	DWORD aid;			//�s��ID
	DWORD cnow;			//�\���Z��
	int hp;				//�̗�
	DWORD hpmax;		//�̗͂̍ő�l�B�f�t�H���g��1024
	double gauge;		//�Q�[�W
	DWORD gaugemax;		//�Q�[�W�̃}�b�N�X�l 1-9�̒l���w��B�^�C�v2�̃Q�[�W�͊֌W�Ȃ�
	DWORD gaugetype;	//�ǂ̃p���[�Q�[�W���g�p���邩�B���̃t���O�B�f�t�H���g�̓Q�[�W����

	BOOL muki;			//�����iFALSE�͉E�����j
	BOOL revx;			//x���]
	BOOL revy;			//y���]
	int rot;			//��]
	float magx,magy;	//�g�嗦
	float z;			//z���W
	DWORD color;		//�F�i���O�`��̏ꍇ�����̐F�ŕ`�悵�Ă��������j
	DWORD alphamode;	//�������@�iGBLEND_�`�j�t���O

	BOOL kougeki;		//�U����on/off
	BOOL kasanari;		//�d�Ȃ蔻��on/off
	BOOL muteki;		//���Gon/off
	BOOL nagerare;		//������ꔻ��on/off

	MYSURFACE **pmsarr; //�r�b�g�}�b�v�̔z��
	GCD_RECT *prdat;	//��`���z��
	GCD_CELL2 *pcdat;	//�Z�����z��
	GCD_HANTEI *phdat;	//�d�Ȃ�E������E��炢����z��

	ATTACKINFO *atk;	//���܎����������Ă���U�����
	ATTACKINFO2 atk2;	//���̃I�u�W�F�N�g����������U�����
	WAZAINFO  *winfo;	//�R���s���[�^�p�Z���

	LPVOID pobjdat_u;	//���[�U�[��`�f�[�^
};

//team id
#define TEAM_PLAYER1		0
#define TEAM_PLAYER2		1
#define TEAM_STAGE			2
#define TEAM_SYSTEM			3

//objtype						0xF1000FEF
#define GOBJFLG_KURAI			0x00000001//�U������炤
#define GOBJFLG_NAGERARE		0x00000002//��������
#define GOBJFLG_ATTACK			0x00000004//�U�����s��
#define GOBJFLG_KASANARI		0x00000008//�d�Ȃ蔻������
#define GOBJFLG_TOBETARGET		0x10000000//�^�[�Q�b�g�ɂȂ邱�Ƃ��ł���B
#define GOBJFLG_NEEDTARGET		0x20000000//�^�[�Q�b�g(eid)���K�v�ł���B
#define GOBJFLG_COMPUTER		0x40000000//�R���s���[�^�ɐ��䂳����B�ʏ�̃L�����N�^�[�ɂ̓V�X�e���ɂ���Ă��̃t���O���t�������
#define GOBJFLG_ZCHARACTER		0x80000000//�L�����N�^�[�u�����v - ���̂Ƃ�����Ɏg���Ă��Ȃ�
#define GOBJFLG_ZBULLET			0x01000000//��ѓ���u�����v- ���̔�ѓ���Ƒ��E��������
#define GOBJFLG_DISPZAHYO		0x00000020//���W�w��̓f�B�X�v���C��̍��W�ł���A�X�e�[�W��̕\���ʒu�ɍ��E����Ȃ�
#define GOBJFLG_DONOTSTOP		0x00000040//���K����stop�G�t�F�N�g���������Ă��Ă�GOBJMSG_ACTION���ʒm�����
#define GOBJFLG_HANSAYOU		0x00000080//�U�������I�u�W�F�N�g����ʒ[�܂œ��B����Ɖ����߂����i���b�Z�[�W���󂯎�邱�Ƃ��ł���j
#define GOBJFLG_CLIPX			0x00000100//�I�u�W�F�N�g��x�����Ɋւ��ăX�e�[�W(-640�`640)�O�ɍs�����Ƃ͂ł��Ȃ�
#define GOBJFLG_CLIPX2			0x00000200//�I�u�W�F�N�g��x�����Ɋւ��ĉ�ʊO�ɏo�邱�Ƃ͂��Ȃ�
#define GOBJFLG_DISPLAYME		0x00000400//�X�e�[�W��Ńf�B�X�v���C���S���Z�o����Ƃ��ɂ��̃L�����N�^�[�̍��W���Q�Ƃ���
#define GOBJFLG_TAIKI			0x00000800//�L�����N�^�[�͑ҋ@���ł���

//�W���I�ȃL�����N�^�[�����t���O
#define GOBJTYPE_CHARACTER2	( GOBJFLG_KURAI | GOBJFLG_NAGERARE | GOBJFLG_ATTACK | GOBJFLG_KASANARI | GOBJFLG_CLIPX | GOBJFLG_CLIPX2 | GOBJFLG_DISPLAYME)
#define GOBJTYPE_CHARACTER	( GOBJTYPE_CHARACTER2 | GOBJFLG_TOBETARGET | GOBJFLG_NEEDTARGET | GOBJFLG_ZCHARACTER | GOBJFLG_HANSAYOU | GOBJFLG_CLIPX)

//�W���I�Ȕ�ѓ���̃t���O
#define GOBJTYPE_BULLET		(GOBJFLG_ATTACK | GOBJFLG_ZBULLET)

//���̑�
#define GOBJTYPE_DISPLAY	( GOBJFLG_CLIPX | GOBJFLG_CLIPX2 | GOBJFLG_DISPLAYME )
#define GOBJTYPE_NODISPLAY	( ~( GOBJFLG_CLIPX | GOBJFLG_CLIPX2 | GOBJFLG_DISPLAYME ) )
#define GOBJTYPE_DEAD		( ~(GOBJFLG_TOBETARGET | GOBJFLG_NEEDTARGET | GOBJFLG_CLIPX | GOBJFLG_CLIPX2 | GOBJFLG_DISPLAYME) )

//gaugetype
#define GOBJFLGA_POWGAUGE1	0x00000001//9�܂ŃX�g�b�N�\�ȃQ�[�W
#define GOBJFLGA_POWGAUGE2	0x00000002//�X�g�b�N�ł��Ȃ��Q�[�W

//�������̏ꍇ�̍������@
#define GBLEND_HANTOUMEI		(0)//�f�t�H���g
#define GBLEND_KASAN			(1)
#define GBLEND_INV				(3)

//  �`���b�Z�[�W�`  ======================================================================
//msghandler �̑�2�����̈Ӗ�===================================================================
//��ȃ��b�Z�[�W
#define GOBJMSG_DELETE			   1//�I�u�W�F�N�g���V�X�e���̐���𗣂��Ƃ��ɌĂ΂��
#define GOBJMSG_ACTION			   2//1�t���[����1��Ă΂��
#define GOBJMSG_COMMAND			   3//�R�}���h���������K�v������Ƃ��ɌĂ΂��
#define GOBJMSG_COMMANDCOM		   4//�R���s���[�^�R�}���h���������Ƃ��ɌĂ΂��
#define GOBJMSG_TOUCHA			   5//�U�����������Ƃ��ɌĂ΂��B
#define GOBJMSG_TOUCHB			   6//�����̍U��������ɓ����������ɌĂ΂��B
#define GOBJMSG_TOUCHC			   7//�����̍U��������ɓ��������Ƃ� (����̏�Ԃ��ω�����O�ɌĂ΂��)�B
#define GOBJMSG_CNGAID			   8//�s��ID���ω������Ƃ��ɌĂ΂��
//�`��n���b�Z�[�W
#define GOBJMSG_DRAW			 101//�`����s���ۂɌĂ΂��
#define GOBJMSG_DRAWBACK		 102//�w�i�X�e�[�W�ɕ`��
#define GOBJMSG_DRAWFRONT		 103//�O�i�X�e�[�W�ɕ`��
//�o��E���Ȃǃ��b�Z�[�W
#define GOBJMSG_DOTOJYO			 201//�o��|�[�Y�Ɉڍs���A�I�������MSGOBJ2SYS_TOJYOEND���b�Z�[�W��Ԃ�
#define GOBJMSG_DOTIMEOVERLOSE	 202//�^�C���I�[�o�[�ŕ�����
#define GOBJMSG_DOYOUWIN		 203//�����|�[�Y�Ɉڍs���A�I�������MSGOBJ2SYS_WINPOSEEND���b�Z�[�W��Ԃ�
#define GOBJMSG_TAIKI			 204//��ʊO�ɑҋ@���Ă��Ă�������
#define GOBJMSG_KOUTAI			 205//�I����B��ʂ̌�����������ł��Ă��������B�i�_���ȂƂ���0��Ԃ��j
#define GOBJMSG_KOUTAI2			 206//�I����B��ʂ̌�����������ł��Ă��������B�I�������MSGOBJ2SYS_KOUTAIEND���b�Z�[�W��Ԃ�
#define GOBJMSG_STRIKER			 207//�X�g���C�J�[�U������炩���Ă��������i�_���ȂƂ���0��Ԃ��j
#define GOBJMSG_DOYOUWIN2		 208//�ҋ@���A���Ԃ̏����ɂ���ă`�[������������
#define GOBJMSG_STRIKERREADY	 209//�X�g���C�J�[�U�����ł��邩�ǂ�����Ԃ�
//���W����n���b�Z�[�W
#define GOBJMSG_KNOCKBACK		 301//�U�������I�u�W�F�N�g����ʒ[�ɒB����
#define GOBJMSG_CLIPX			 302//���W����ʊO�ɐ����Ă��܂��Ă����
//�I�u�W�F�N�g�ԑ��ݍ�p�E���̑�
#define GOBJMSG_CNGTARGET		 401//�^�[�Q�b�g���ς�����Ƃ��ɌĂ΂��
#define GOBJMSG_SOUSAI			 402//����ѓ���ƐڐG
//�Q�[���i�s
#define GOBJMSG_CNGROUND		 502//���E���h���ς�����Ƃ��ɌĂ΂��

//�l�b�g���[�N
#define GOBJMSG_SYNC			 601//�������b�Z�[�W

//���̑�
#define GOBJMSG_USERDEFINE		0x80000000//���[�U�[��`���b�Z�[�W�쐬�p�t���O

//GOBJMSG_TOUCHA ���b�Z�[�W�ɑ΂���߂�l-------------------------------------------------
//0��Ԃ��Ə���ɔ��f���܂�
#define TOUCHA_KURAIS	0x10000001//������炢
#define TOUCHA_KURAIC	0x10000002//����炢
#define TOUCHA_KURAIJ	0x10000003//�󒆋�炢
#define TOUCHA_GUARDS	0x20000001//�����K�[�h
#define TOUCHA_GUARDC	0x20000002//���K�[�h
#define TOUCHA_GUARDJ	0x20000003//�󒆃K�[�h
#define TOUCHA_MUSI		0x50000000//�U���͋��������ǁA��炢������s��Ȃ��i0x40000000�������t���O�j
#define TOUCHA_MUSI2	0x60000000//�U�����K�[�h�������ǁA��炢������s��Ȃ��i0x40000000�������t���O�j
#define TOUCHA_AVOID	0x80000000//�U���������Ȃ�����

//GOBJMSG_TOUCHC ���b�Z�[�W�ɑ΂���߂�l-------------------------------------------------------
#define TOUCHC_OK		0x00000000//�������܂���B���܂܂Œʂ菈�����i�s���܂��B
#define TOUCHC_CANCEL	0x00000001//��`�Փ˂��Ȃ��������Ƃɂ��܂��B����ȍ~�̏����͂Ȃ���܂���B

//GOBJMSG_STRIKERREADY���b�Z�[�W�ɑ΂���߂�l ���邢�́�DI_FUNCTIONS_S.strikerok�̖߂�l-------
#define STRIKERREADY_UNKNOWN		(0)//�s��
#define STRIKERREADY_OK				(1)//�����I�b�P�[
#define STRIKERREADY_NOTREADY		(2)//�ł��Ȃ�
#define STRIKERREADY_NOTSUPPORTED	(3)//�[���A���Ƃ��Ƃ��Ȃ�
#define STRIKERREADY_NOPOINT		(4)//(sys)�X�g���C�J�[�̃|�C���g������Ȃ�

//**************************************************************************
// �V�X�e���֘A�̊֐��̒�`
//**************************************************************************
typedef DWORD  (*PFUNCS_GETKEY)(DWORD,DWORD);			//�L�[����get
typedef int    (*PFUNCS_SEEKKEY)(DWORD,int,int,DWORD);	//�L�[����get(2)
typedef void   (*PFUNCS_SETKATISERIF)(DWORD,char*);		//�����䎌�ݒ�
typedef void   (*PFUNCS_ADDEFFECT)(DWORD,int,int,DWORD);//�G�t�F�N�g����
typedef void   (*PFUNCS_PLAYSYSSOUND)(DWORD);			//wav�Đ�
typedef LPVOID (*PFUNCS_LOADMYSOUND)(char*);			//wav�ǂݍ���
typedef void   (*PFUNCS_PLAYMYSOUND)(LPVOID);			//wav�Đ�
typedef void   (*PFUNCS_KILLMYSOUND)(LPVOID);			//wav����
typedef DWORD  (*PFUNCS_MESSAGE2SYSTEM)(DWORD,DWORD,DWORD);	//���b�Z�[�W
typedef char*  (*PFUNCS_GETENEMYNAME)(DWORD);			//"�G"�̖��O���擾����
typedef double (*PFUNCS_GETDISPCENTERX)();				//���݂̉�ʒ��S�𓾂�
typedef double (*PFUNCS_GETDISPCENTERY)();				//���݂̉�ʒ��S�𓾂�

typedef DWORD  (*PFUNCS_GETTEAMNUM)();					//���Ή������Q�b�g
typedef DWORD  (*PFUNCS_GETTAISENKEISIKI)();			//�ΐ�`�����Q�g
typedef DWORD  (*PFUNCS_STRIKEROK)(DWORD tid,DWORD oid,BOOL next);//�X�g���C�J�[�U�����\���ǂ�����₢�l�߂�

struct DI_FUNCTIONS_S
{
	PFUNCS_GETKEY			getkey;
	PFUNCS_SEEKKEY			seekkey;
	PFUNCS_SETKATISERIF		setkatiserif;
	PFUNCS_ADDEFFECT		addeffect;
	PFUNCS_PLAYSYSSOUND		playsyssound;
	PFUNCS_LOADMYSOUND		loadmysound;
	PFUNCS_PLAYMYSOUND		playmysound;
	PFUNCS_KILLMYSOUND		killmysound;
	PFUNCS_MESSAGE2SYSTEM	msg2system;
	PFUNCS_GETENEMYNAME		getenemyname;
	PFUNCS_GETDISPCENTERX	getdispcenterx;
	PFUNCS_GETDISPCENTERY	getdispcentery;

	PFUNCS_GETTEAMNUM		getteamnum;
	PFUNCS_GETTAISENKEISIKI	gettaisenkeisiki;
//	PFUNCS_STRIKEROK		strikerok;
};

//�L�[���͒�`===========================================================================

#define KEYINPUT_PLAYER1	(0)
#define KEYINPUT_PLAYER2	(1)

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
#define KEYSTA_BA		(0x00010000)//�{�^��1��������Ă�����
#define KEYSTA_BA2		(0x00020000)//�{�^��1���������ꂽ�Ƃ���
#define KEYSTA_BB		(0x00100000)//�{�^��2
#define KEYSTA_BB2		(0x00200000)
#define KEYSTA_BC		(0x01000000)//�{�^��3
#define KEYSTA_BC2		(0x02000000)
#define KEYSTA_BD		(0x10000000)//�{�^��4
#define KEYSTA_BD2		(0x20000000)

#define KEYSTA_ANYKEY	(0x22220000)
#define KEYSTA_BUTTONS	(0x22220000)


//Message Id============================================================================
//�V�X�e�����ւ̃��b�Z�[�W
#define MSGOBJ2SYS_TOJYOEND			1//�o��f�����I������
#define MSGOBJ2SYS_FINALDOWNEND		2//�ŏI�_�E�����I������
#define MSGOBJ2SYS_WINPOSEEND		3//�����f�����I������
#define MSGOBJ2SYS_KOUTAI1			4//���v��(�L�����N�^�[1)
#define MSGOBJ2SYS_KOUTAI2			5//���v��(�L�����N�^�[2)
#define MSGOBJ2SYS_STRIKER1			6//�X�g���C�J�[�v��(�L�����N�^�[1)
#define MSGOBJ2SYS_STRIKER2			7//�X�g���C�J�[�v��(�L�����N�^�[2)
#define MSGOBJ2SYS_KOUTAIEND		8//���I���B�퓬�ł����Ԃł�

//Effect Id ============================================================================
//addeffect�̑������Ɏg����萔�B�������ȍ~�̈Ӗ��̓G�t�F�N�gID�ɂ��قȂ�
							 //�����̈Ӗ�  /�R�����g
#define EFCTID_MARK1		1//x,y,��
#define EFCTID_MARK2		2//x,y,��
#define EFCTID_MARK3		3//x,y,��
#define EFCTID_MARK4		4//x,y,��
#define EFCTID_MARKG		5//x,y,��
#define EFCTID_MARKG2		6//x,y,��
#define EFCTID_HIKARI		7//x,y,-
#define EFCTID_STOP			8//�X�g�b�v���Ă鎞��,-,-
#define EFCTID_DARK			9//�Â��Ȃ��Ă鎞��,-,-  
#define EFCTID_MARKC		10//x,y,-  /�����͂񂾂Ƃ��̃}�[�N
#define EFCTID_GNDCIR		11//x,y,-  /�n�ʂɊۂ��Ռ��g
#define EFCTID_SINDO		12//�U��,��������,-  /�U�����N����
#define EFCTID_NOBG			13//��������,-,-  �w�i�̕`��𖳂��ɂ���
#define EFCTID_CYOHI		14//x,y,�d������ /���K�G�t�F�N�g
#define EFCTID_TUBUTUBU		15//x,y,���̂ق�  /�p�[�e�B�N���U��
#define EFCTID_TUBUTUBU2	16//x,y, ��Ɠ��� /�ł������p�[�e�B�N���U��
#define EFCTID_COMEON		17//x,y,���� /come on my brother
#define EFCTID_BURN			18//-,-,�I�u�W�F�N�gID /�w��I�u�W�F�N�g���R����

//�Ԃԃp�����[�^���͗p�}�N��
#define TUBUTUBUPRM(a,b,c,d,e,f,g,h)	((DWORD)(a*256*256*256)*16+(b*256*256*256)+(c*256*256*16)+(d*256*256)+(e*256*16)+(f*256)+(g*16)+h)
//a:���̐�(0�`15)
//b;���̑��x
//c:���̑��x�̂΂��
//d:�Œᑬ�x
//e:�����ʒu�̂΂��
//f:�����̂΂��
//g:���̍ŒZ����
#define TUBUTUBU_GOUP		0x00000001//������ɕ΂������x����������
#define TUBUTUBU_GODOWN		0x00000002//�����ɕ΂������x����������
#define TUBUTUBU_GOLEFT		0x00000004//��������
#define TUBUTUBU_GORIGHT	0x00000008//�E

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

//�ΐ�`�� PFUNCS_GETTAISENKEISIKI�̖߂�l
#define TAISENKEISIKI_GOCYAMAZE		1
#define TAISENKEISIKI_KOUTAI		2
#define TAISENKEISIKI_JYUNBAN		3

//**************************************************************************
//�@�I�u�W�F�N�g�֘A�̊֐��̒�`
//**************************************************************************
typedef DWORD  (*PFUNCO_CREATE)();						//�I�u�W�F�N�g����
typedef void   (*PFUNCO_DELETE)(DWORD);					//�I�u�W�F�N�g����
typedef BOOL   (*PFUNCO_CATCH)(DWORD,LPVOID);			//�����Ƃ��ő��������
typedef LPVOID (*PFUNCO_GETINFO)(DWORD);				//���
typedef void   (*PFUNCO_ADDDAMAGE)(DWORD,DWORD,int x,int y);//(��ɓ����̂Ƃ�)�����I�Ƀ_���[�W��^����
typedef void   (*PFUNCO_ACTIDCHANGED)(DWORD);			//�s��ID���ω������Ƃ��̏����𖾎��I�ɍs��
typedef void*  (*PFUNCO_GETACTIVECHARACTER)(DWORD);		//���݃A�N�e�B�u�ȃL�����N�^�[�̏��
typedef void   (*PFUNCO_SUICIDE)(DWORD);				//�I�u�W�F�N�g����
typedef DWORD  (*PFUNCO_GETMAAI_H)(DWORD oid,DWORD eoid);//�ԍ�����get(����)
typedef DWORD  (*PFUNCO_GETMAAI_V)(DWORD oid,DWORD eoid);//�ԍ�����get(����)

typedef DWORD  (*PFUNCO_GETCHARACTERID)(DWORD tid,DWORD index);//�L�����N�^�[�̃I�u�W�F�N�gID

struct DI_FUNCTIONS_O
{
	PFUNCO_CREATE			objcreate;		//abolished
	PFUNCO_DELETE			objdelete;		//abolished
	PFUNCO_CATCH			objcatch;		//out of order now
	PFUNCO_GETINFO			getinfo;
	PFUNCO_ADDDAMAGE		adddamage;
	PFUNCO_ACTIDCHANGED		actidchanged;
	PFUNCO_GETACTIVECHARACTER getactivechar;//out of order now
	PFUNCO_SUICIDE			suicide;		//abolished
	PFUNCO_GETMAAI_H		getmaai_h;
	PFUNCO_GETMAAI_V		getmaai_v;

	PFUNCO_GETCHARACTERID	getcharid;
};

//�@��������ށ@========================================================
//catch ��CATCHYOU�\���̂̃|�C���^�����Ď��s���ATRUE���Ԃ�Γ����J�n
//�n�����֐��� ����̍s���֐��ɂȂ�B�\���Z���ɂ̓f�t�H���g�Z���𗘗p����

struct CATCHYOU
{
	DWORD oid;//�͂ރI�u�W�F�N�g
	DWORD actmsg;//�͂񂾃I�u�W�F�N�g���A���܂ꂽ�I�u�W�F�N�g�𑀍삷�邽�߂Ɏg�����b�Z�[�W
};

//**************************************************************************
//�@�`��֘A�̊֐��̒�`
//**************************************************************************

//*�`��n�֐�*
typedef LPVOID (*PFUNCD_GETD3D)();							//IDirect3D* �̎擾
typedef LPVOID (*PFUNCD_GETD3DD)();							//IDirect3DDevice* �̎擾
typedef void   (*PFUNCD_LOADCELLDAT)(char*,LPVOID,LPVOID,LPVOID);//�Z���f�[�^�ǂݍ��݊֐�
typedef LPVOID (*PFUNCD_LOADBMP)(char*,char*);				//�r�b�g�}�b�v�ǂݍ��݊֐�
typedef void   (*PFUNCD_UNLOADBMP)(LPVOID);					//�r�b�g�}�b�v��n���֐�
typedef void   (*PFUNCD_CELLDRAW)(LPVOID*,LPVOID,LPVOID,DWORD,int,int,float,int,BOOL,BOOL,DWORD,float,float);//�Z���`��֐�
typedef void   (*PFUNCD_CKBLT)(void*,int,int,RECT,double,double,BOOL,BOOL,float,DWORD);//Blt2
typedef void   (*PFUNCD_BLT3D)(void*,RECT,MYRECT3D,DWORD);	//Blt3
typedef void   (*PFUNCD_SETTRANSFORM)(BOOL);				//�ϊ��s��ݒ�
typedef void   (*PFUNCD_SETBLEND)(DWORD);					//�u�����h�̎d����ύX
typedef void   (*PFUNCD_SETPARENTMATRIX)(LPVOID,BOOL,LPVOID);//�u�e�v�̕ϊ��s��ݒ�
typedef DWORD  (*PFUNCD_CREATEGCDDAT)(char* filename,LPVOID *cdat,LPVOID *rdat,LPVOID *hdat);//�Z���f�[�^�ǂݍ��݁Bver0.90�ȍ~�ł�LoadCellDat���炱����ɕύX����ׂ�
typedef void   (*PFUNCD_DESTROYGCDDAT)(LPVOID *cdat,LPVOID *rdat,LPVOID *hdat);//CreateCellDat�Ő������ꂽ�o�b�t�@���N���A

struct DI_FUNCTIONS_D
{
	PFUNCD_GETD3D		getd3d;
	PFUNCD_GETD3DD		getd3dd;
	PFUNCD_LOADCELLDAT	loadcelldat;
	PFUNCD_LOADBMP		loadbmp;
	PFUNCD_UNLOADBMP	unloadbmp;
	PFUNCD_CELLDRAW		celldraw;
	PFUNCD_CKBLT		ckblt;
	PFUNCD_BLT3D		blt3d;
	PFUNCD_SETTRANSFORM settrans;
	PFUNCD_SETBLEND		setblend;
	PFUNCD_SETPARENTMATRIX setparentmat;

	PFUNCD_CREATEGCDDAT create_celldat;
	PFUNCD_DESTROYGCDDAT destroy_celldat;
};


//**************************************************************************
//�@�l�b�g���[�N�֘A�̊֐��̒�`
//**************************************************************************
typedef DWORD (*PFUNCN_REGISTER_OBJECT)(BYTE dllID,BYTE userID,BYTE userNo);
typedef void (*PFUNCN_SENDSYNCMESSAGE)(DWORD objid,DWORD size,LPVOID dat);
typedef void (*PFUNCN_SET_SYNCRONIZE)(DWORD objid,BOOL enabled);
//typedef DWORD (*PFUNCN_REGIST_USEREFFECT)(BYTE dllno,BYTE userid,BYTE userno);
//typedef void (*PFUNCN_CREATE_USEREFFECT)(DWORD userefctid,float x,float y,DWORD flags1,DWORD flags2);

struct DI_FUNCTIONS_N
{
	PFUNCN_REGISTER_OBJECT obj_regist;			//��o.objcreate
	PFUNCN_SENDSYNCMESSAGE send_syncmsg;		//out of order now
	PFUNCN_SET_SYNCRONIZE set_syncronize;
//	PFUNCN_REGIST_EFFECT  regist_usereffect;
//	PFUNCN_CREATE_USEREFFECT create_usereffect;
};

//*************************************************************************
//  �W���I��z���W�̒l
//*************************************************************************
#define ZZAHYO_CHARACTER	(0.00f)
#define ZZAHYO_CHARACTER_B	(0.02f)
#define ZZAHYO_CHARACTER_F	(-0.02f)
#define ZZAHYO_BULLET1		(0.08f)
#define ZZAHYO_BULLET2		(0.04f)
#define ZZAHYO_BULLET3		(-0.04f)
#define ZZAHYO_BULLET4		(-0.08f)
#define ZZAHYO_EFFECT1		(0.06f)
#define ZZAHYO_EFFECT2		(-0.06f)
#define ZZAHYO_STAGE		(0.1f)


//**************************************************************************
//  �L�����N�^DLL�̃o�[�W����
//**************************************************************************
#define CDI_VERSION 920

//********************************************************************************
//�@�L�����N�^�[�I�v�V��������`
//********************************************************************************

struct CHARACTER_LOAD_OPTION
{
	DWORD flag;			//�I�v�V������ON�̏ꍇ�ɗ��t���O
	DWORD exclusive;	//�����Ɏw��ł��Ȃ��ق��̃I�v�V����
	DWORD depends;		//�����Ɏw�肳��Ă��Ȃ���΂Ȃ�Ȃ��ق��̃I�v�V����
	char name[64];		//�I�v�V������
	char point;			//�I�v�V��������|�C���g
};

//**************************************************************************
// CharacterInfo�֐��œn�����\����
//**************************************************************************

typedef DWORD (*PFUNC_CHARACTERINFO)(LPVOID);
struct CDI_CHARACTERINFO
{
	DWORD system_version;		//�V�X�e���̃o�[�W�����B�iver0.9�ȑO��0���͂����Ă�Ƃ����܂��j
	char dummy[28];
	char name[32];				//�R�R�ɖ��O���R�s�[���邱��
	DWORD caps;					//�����ɏ���ݒ�iver0.9�ȍ~�j
	LPVOID dummy2[3];
	DWORD ver;					//�R�R�Ƀo�[�W�������R�s�[���邱��
	//ver0.9�ȍ~�Œǉ�
	CHARACTER_LOAD_OPTION options[32];	//�L�����I�����ɐݒ�\�ȃI�v�V����
	DWORD max_option_point;		//�I�v�V�����̍ő�l
};

//caps�ɐݒ肷��t���O
#define CHARACTER_CAPS_NET		0x00000001//�l�b�g���[�N�ΐ�Ŏg�p�\
#define CHARACTER_CAPS_OFFLINE	0x00000002//�����I�t���C�����[�h�ł��g�p�\


//********************************************************************************
//�@CreateCharacter �̈����ɓn�����\����
//********************************************************************************
typedef DWORD (*PFUNC_CREATECHARACTER)(LPVOID);
struct CDI_CHARACTERINFO2
{
	char dir[64];//dll������f�B���N�g��
	DWORD tid;//�`�[��ID(0:1P���A1:2P��)
	DWORD color;//�ǂ̐F�ɂ��邩�ipal?.bmp�j
	DWORD keyinput;//�ǂ�������͂��󂯎�邩
	DI_FUNCTIONS_S *funcs;
	DI_FUNCTIONS_O *funco;
	DI_FUNCTIONS_D *funcd;
	//ver0.9�ȍ~�Œǉ�
	BOOL isRemote;			//�����[�g���䂩�ǂ���
	BYTE dllid;
	DI_FUNCTIONS_N *funcn;	//���[�J�����[�h�ł�NULL
	DWORD options_flag;		//�I�����ꂽ�I�v�V�����t���O
};

//**************************************************************************
// �X�e�[�WDLL�̃o�[�W����
//**************************************************************************

#define SDI_VERSION 750


//**************************************************************************
// StageInfo�֐��œn�����\����
//**************************************************************************

typedef DWORD (*PFUNC_STAGEINFO)(LPVOID);
struct SDI_STAGEINFO
{
	char name[32];
	DWORD ver;
};

//**************************************************************************
// CreateStage�֐��œn�����\����
//**************************************************************************

typedef DWORD (*PFUNC_CREATESTAGE)(LPVOID);
struct SDI_STAGEINFO2
{
	char dir[64];//dll������f�B���N�g��
	DWORD tid;//�`�[��ID(���2(�X�e�[�W))
	DI_FUNCTIONS_S *funcs;
	DI_FUNCTIONS_O *funco;
	DI_FUNCTIONS_D *funcd;
};

#endif//_GOBJECT_H_