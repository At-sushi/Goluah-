
/*!
*	@file
*	@brief DLL�C���^�[�t�F�[�X��`
*	GOLUAH �L�����N�^�[�E�X�e�[�WDLL �̃r���h�ɕK�v�Ȋe��\���̂̒�`
*/
/*!
*	@defgroup GObject
*	@brief �V�X�e����DLL�C���^�[�t�F�[�X��`
*	�V�X�e��(�{�̎��s�t�@�C��)�ƁADLL�̊ԂŌ��������e��\���́E�萔�̒�`�ł��B
*	DLL����҂͂����̒�`��ύX���邱�Ƃ͂ł��܂���B
*/
/*!
*	@ingroup GObject
*/
/*@{*/
#pragma once


//GCD�t�H�[�}�b�g��`���K�v
#include "define_gcd.h"
#define _GOBJECT_H_

/*!
*	@brief �U���͏��\����
*	@ingroup GObject
*
*	�L�����N�^�[�ԁA�L�����N�^�[�̃V�X�e���Ԃł���肷��B
*	��Ƀ|�C���^�n���ň����̂ŁADLL�͍\���̂̎��̂�j������Ȃ��悤��
*	�K�؂ȏꏊ�ɕێ����邱�ƁB
*/
struct ATTACKINFO
{
	DWORD hit;		//!< �q�b�g��� @sa HITINFO_FLAGS
	DWORD guard;	//!< �K�[�h��� @sa GUARDINFO_FLAGS
	DWORD id;		//!< DLL���Ŏ��R�Ɏg�p�ł��鎯��ID�B�V�X�e�����ł͊֗^���܂���B

	DWORD damage;	//!< �_���[�W�l
	DWORD kezuri;	//!< ���l

	BOOL muki;		//!< ���肪�̂��������(FALSE�Ȃ�΁A�����������Ă�������Ɠ���)
};

//hit

/*!
*	@brief �q�b�g���t���O
*	@ingroup GObject
*	@sa ATTACKINFO
*
*	ATTACKINFO::hit �ɐݒ肵�܂��B�U�������������Ƃ��̊e��ݒ�B
*/
enum HITINFO_FLAGS
{
	//��L�r�b�g					0x0FFFFFFF

	HITINFO_REACT1			=	0x00000001,//!< ���ʂ̎�̂�����
	HITINFO_REACT2			=	0x00000002,//!< ���ʂ̒��̂�����
	HITINFO_REACT3			=	0x00000003,//!< ���ʂ̋��̂�����
	HITINFO_DOWN			=	0x00000004,//!< �_�E�\����i�]�ԁj
	HITINFO_FUTTOBI			=	0x00000005,//!< �������
	HITINFO_FUTTOBI2		=	0x00000006,//!< ������ԁi������Ə�̂ق��ցj
	HITINFO_REACT1A			=	0x00000007,//!< ��A�̂�����Ȃ�
	HITINFO_REACT2A			=	0x00000008,//!< ���A�̂�����Ȃ�
	HITINFO_REACT3A			=	0x00000009,//!< ���A�̂�����Ȃ�
	HITINFO_TATAKI1A		=	0x0000000A,//!< �^���@�����o�E���h�L��
	HITINFO_TATAKI2A		=	0x0000000B,//!< �^���@�����o�E���h�Ȃ�
	HITINFO_TATAKI1B		=	0x0000000C,//!< �΂ߒ@�����o�E���h�L��
	HITINFO_TATAKI2B		=	0x0000000D,//!< �΂ߒ@�����o�E���h�Ȃ�
	HITINFO_USERDEFINE		=	0x0000000E,//!< ���[�U�[��`�̋�炢�s��(�n��/�󒆎�������)
	HITINFO_USERDEFINE2		=	0x0000000F,//!< ���[�U�[��`�̋�炢�s��(�����I�ɋ󒆂ɕ�������)

	HITINFO_MARK1			=	0x00010000,//!< �q�b�g�����Ƃ��ɂ������Ⴂ�}�[�N��\�����܂�
	HITINFO_MARK2			=	0x00020000,//!< �q�b�g�����Ƃ��ɒ����炢�̃}�[�N��\�����܂�
	HITINFO_MARK3			=	0x00030000,//!< �q�b�g�����Ƃ��ɂł������}�[�N��\�����܂�
	HITINFO_MARK4			=	0x00040000,//!< �q�b�g�����Ƃ��ɂ���ɂł������}�[�N��\�����܂�

	HITINFO_SNDHIT1			=	0x00100000,//!< �q�b�g����
	HITINFO_SNDHIT2			=	0x00200000,//!< �q�b�g����
	HITINFO_SNDHIT3			=	0x00300000,//!< �q�b�g����
	HITINFO_SNDSHK1			=	0x00400000,//!< �d����1
	HITINFO_SNDSHK2			=	0x00500000,//!< �d����2

	HITINFO_SIV1			=	0x01000000,//!< �q�b�g�X�g�b�v�A�Z��
	HITINFO_SIV2			=	0x02000000,//!< �q�b�g�X�g�b�v�A�ӂ�
	HITINFO_SIV3			=	0x03000000,//!< �q�b�g�X�g�b�v�A����
	HITINFO_STOP			=	0x04000000,//!< �q�b�g�X�g�b�v�A�߂��ᒷ

	HITINFO_EFCTSINDO		=	0x00001000,//!< �U�������������Ƃ��A�n�ʂ�U��������
	HITINFO_EFCTBURN		=	0x00002000,//!< �U������炤�ƃL�����N�^�[���R���o��(��)
	HITINFO_EFCTBURN_B		=	0x00004000,//!< �U������炤�ƃL�����N�^�[���R���o��(��)
	HITINFO_EFCTBURN_G		=	0x00008000,//!< �U������炤�ƃL�����N�^�[���R���o��(��)

	HITINFO_USERIDMASK		=	0x00000FF0,//!< ���[�U�[��`�q�b�gID
};

/*!
*	@brief �K�[�h���t���O
*	@ingroup GObject
*	@sa ATTACKINFO
*
*	ATTACKINFO::guard �ɐݒ肵�܂��B�U�������K�[�h���ꂽ�Ƃ��̊e��ݒ�B
*/
enum GUARDINFO_FLAGS
{
	GUARDINFO_XSTAND		=	0x10000000,//!< �����K�[�h�ł��܂���
	GUARDINFO_XCROUCH		=	0x20000000,//!< ���Ⴊ�݃K�[�h�ł��܂���
	GUARDINFO_XJAMP			=	0x40000000,//!< �󒆃K�[�h�ł��܂���
	GUARDINFO_XAUTO			=	0x80000000,//!< �I�[�g�K�[�h�ł��Ȃ�

	GUARDINFO_REACT1		=	0x00000001,//!< ��K�[�h
	GUARDINFO_REACT2		=	0x00000002,//!< ���K�[�h
	GUARDINFO_REACT3		=	0x00000003,//!< ���K�[�h
	GUARDINFO_USERDEFINE	=	0x00000004,//!< ���[�U�[��`�K�[�h�s��

	GUARDINFO_SIV1			=	0x01000000,//!< �q�b�g�X�g�b�v�A�Z��
	GUARDINFO_SIV2			=	0x02000000,//!< �q�b�g�X�g�b�v�A�ӂ�
	GUARDINFO_SIV3			=	0x03000000,//!< �q�b�g�X�g�b�v�A����
	GUARDINFO_STOP			=	0x04000000,//!< �q�b�g�X�g�b�v�A�߂��ᒷ

	GUARDINFO_EFCTSINDO		=	0x00001000,//!< �U�������������Ƃ��A�n�ʂ�U��������

	GUARDINFO_USERIDMASK	=	0x00000FF0,//!< ���[�U�[��`�K�[�hID
};

//���[�U�[��`�t���O�ݒ�p�}�N��
#define HITINFO_USER(id)		(((id)<<4) | HITINFO_USERDEFINE)		//!< ID��0�`255�ŗ^���邱��
#define HITINFO_USER2(id)		(((id)<<4) | HITINFO_USERDEFINE2)		//!< ID��0�`255�ŗ^���邱��
#define GUARDINFO_USER(id)		(((id)<<4) | GUARDINFO_USERDEFINE)		//!< ID��0�`255�ŗ^���邱��

/*!
*	@brief �U���͏��\���́i��炢���j
*	@ingroup GObject
*
*	�I�u�W�F�N�g���ǂ��������U�����󂯂��̂����`����B
*	info1�����o�͑��葤�̍\���̂ւ̃|�C���^�����̂܂ܓn���Ă���̂ł�����Ȃ����ƁB
*/
struct ATTACKINFO2
{
	ATTACKINFO *info1;		//!< �U�����
	DWORD oid;				//!< �U�����s�������c�̃I�u�W�F�N�gID
	DWORD flags;			//!< �t���O @sa ATKINFO2_FLAGS
};

/*!
*	@brief �U���͏��t���O�i��炢���j
*	@ingroup GObject
*	@sa ATTACKINFO2
*
*	�U���̃q�b�g���ɃV�X�e���������̃t���O��GOBJECT::atk2::flags�Ɋ��蓖�Ă܂�
*/
enum
{
	ATKINFO2_RIGHTBACK		=	0x00000001,//!< �E���ɂ̂�����
	ATKINFO2_ATTACKERBACK	=	0x00000002,//!< ��ʒ[�܂œ��B������U��������������߂����������Ȃ���΂Ȃ�Ȃ�
};

/*!
*	@brief �R���s���[�^���쎞�Ɏg�p����\����
*	@ingroup GObject
*
*	DLL���A�V�X�e�����ցA�L�����N�^�[�̎��Z�̍s��ID��ʒm���邽�߂Ɏg�p�����B
*	�I�u�W�F�N�g���R���s���[�^����ł���ꍇ�A���ꂼ��̍s��ID�̗p�r����A
*	�V�X�e�����s���J�ڂ̌���I�����A�s���J�ڂ̉�/�s��GOBJMSG_COMMANDCOM���b�Z�[�W��
*	DLL���Ɋm�F���ĉł�������A�I�u�W�F�N�g�͂��̍s���֑J�ڂ���B
*/
struct WAZAINFO
{
	DWORD walkf;			//!< �O�i
	DWORD walkb;			//!< ���
	DWORD jampf;			//!< �܂�W�����v
	DWORD jampb;			//!< ���W�����v
	DWORD dashf;			//!< �܂�_�b�V��
	DWORD dashb;			//!< ���_�b�V��

	DWORD att_short[6];		//!< �Z���[�`�U��
	DWORD att_middle[6];	//!< �����[�`�U��
	DWORD att_long[6];		//!< �����[�`�U��
	DWORD att_jamp[6];		//!< �󒆂ŏo���U��
	DWORD att_tai[6];		//!< �΋�Ɏg����U��
	DWORD att_bullet[6];	//!< ��ѓ���Ȃ�

	DWORD nageid[6];		//!< �����s��ID

	//�����g�p��
	int		not_used1[8];			//!<���g�p
	DWORD	not_used2[8][16][2];	//!<���g�p
	int		not_used3[8];			//!<���g�p
	DWORD	not_used4[8][16][2];	//!<���g�p
};

//ACTION ID===============================================================================

/*!
*	@brief �s����ʃt���O
*	@ingroup GObject
*	@sa GOBJECT::aid
*
*	�s��ID�ɑ΂��Đݒ肷��e��ʃt���O��`�B
*	�R�}���h����E�K�[�h���蓙�ł����̃t���O���Q�Ƃ����̂ŁA
*	�V�K�s��ID���`����ꍇ�͓K�؂ȃt���O�������Ȃ���΂Ȃ�܂���B
*/
enum ACTION_FLAGS
{
	//��L�t���O							//xxxx 0000 00xx 0xxx | user flags...
	ACTID_KUCYU			=	0x00010000,	//!<�󒆍s��
	ACTID_SYAGAMI		=	0x00020000,	//!<���Ⴊ�ݍs��
	ACTID_ATTACK		=	0x00100000,	//!<�U���s��
	ACTID_HISSATU		=	0x00200000,	//!<�K�E�Z
	ACTID_SYSTEM		=	0x80000000,	//!<�V�X�e���n
	ACTID_KURAI			=	0x40000000,	//!<��炢�s��
	ACTID_GUARD			=	0x20000000,	//!<�K�[�h�s��
	ACTID_NAGE			=	0x00040000,	//!<����/������� �s��
	ACTID_INOUT			=	0x10000000,	//!<���/�x���U�� �s��
};

/*!
*	@brief �s����ʃt���O
*	@ingroup GObject
*	@sa GOBJECT::aid
*
*	��炢�E�K�[�h�E����@��A�_���[�W�E�K�[�h����̕��A�s����
*	�V�X�e�����Œ�`����K�v�̂���s���̍s��ID�B
*	DLL��҂��V�K�ɍ쐬����s��ID�́A�����ɒ�`���Ă���l�Ƃ��Ԃ�Ȃ��悤�ɂ��邱�ƁB
*/
enum DEFAULT_ACTION_IDs
{
	ACTID_NEUTRAL		= 1,					//!< �j���[�g�����|�[�Y (�_���[�W�E�K�[�h�s������̕��A�s��)
	ACTID_TOJYO			= 2,					//!< �o�� (���o�[�W�����Ƃ̌݊��p)
	ACTID_SYORI			= 3,					//!< ���� (���o�[�W�����Ƃ̌݊��p)
	ACTID_CROUCH		= ACTID_SYAGAMI,		//!< ���Ⴊ�݃|�[�Y (�_���[�W�E�K�[�h�s������̕��A�s��)
	ACTID_RAKKA			= ACTID_KUCYU,			//!< ���� (�_���[�W�E�K�[�h�s������̕��A�s��)

	ACTID_DAMAGE1		= (ACTID_SYSTEM | ACTID_KURAI | 0x0001),				//!< ������炢�i���j
	ACTID_DAMAGE2		= (ACTID_SYSTEM | ACTID_KURAI | 0x0002),				//!< ������炢�i���j
	ACTID_DAMAGE3		= (ACTID_SYSTEM | ACTID_KURAI | 0x0003),				//!< ������炢�i��j
	ACTID_DAMAGEC1		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0001),//!< ���Ⴊ�݋�炢�i���j
	ACTID_DAMAGEC2		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0002),//!< ���Ⴊ�݋�炢�i���j
	ACTID_DAMAGEC3		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0003),//!< ���Ⴊ�݋�炢�i��j
	ACTID_DAMAGEJ1		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU   | 0x0001),//!< �󒆋�炢�i���j
	ACTID_DAMAGEJ2		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU   | 0x0002),//!< �󒆋�炢�i���j
	ACTID_DAMAGEJ3		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU   | 0x0003),//!< �󒆋�炢�i��j
	ACTID_DOWN			= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0004),	//!< �������œ]��
	ACTID_DOWN2			= (ACTID_SYSTEM | ACTID_KURAI | 0x0004),				//!< �_�E�\
	ACTID_OKIAGARI		= (ACTID_SYSTEM | 0x0001),								//!< ����������
	ACTID_FINALDOWN		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0005),	//!< ��
	ACTID_FUTTOBI		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0006),	//!< �ӂ��Ƃ�
	ACTID_BOUND			= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0007),	//!< �o�E���h
	ACTID_FINALBOUND	= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0008),	//!< �o�E���h(���S)
	ACTID_FINALDOWN2	= (ACTID_SYSTEM | ACTID_KURAI | 0x0005),				//!< ��
	ACTID_FUTTOBI2		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU	| 0x0009),	//!< �ӂ��Ƃ�2
	ACTID_NAGERARE		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | ACTID_NAGE | 0x0001),//!< ������ꒆ
	ACTID_DAMAGE1A		= (ACTID_SYSTEM | ACTID_KURAI | 0x0006),				//!< ������炢�i���j�A�̂�����Ȃ�
	ACTID_DAMAGE2A		= (ACTID_SYSTEM | ACTID_KURAI | 0x0007),				//!< ������炢�i���j�A�̂�����Ȃ�
	ACTID_DAMAGE3A		= (ACTID_SYSTEM | ACTID_KURAI | 0x0008),				//!< ������炢�i���j�A�̂�����Ȃ�
	ACTID_DAMAGEC1A		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0004),//!< ���Ⴊ�݋�炢�i���j�A�̂�����Ȃ�
	ACTID_DAMAGEC2A		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0005),//!< ���Ⴊ�݋�炢�i���j�A�̂�����Ȃ�
	ACTID_DAMAGEC3A		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_SYAGAMI | 0x0006),//!< ���Ⴊ�݋�炢�i��j�A�̂�����Ȃ�
	ACTID_TATAKITUKE1A	= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000A),	//!< �^���@����������(�o�E���h�L��)
	ACTID_TATAKITUKE2A	= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000B),	//!< �^���@����������(�o�E���h����)
	ACTID_TATAKITUKE1B	= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000C),	//!< �΂ߒ@����������(�o�E���h�L��)
	ACTID_TATAKITUKE2B	= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000D),	//!< �΂ߒ@����������(�o�E���h����)
	ACTID_KAITENFINISH	= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x000E),	//!< ��]���ĉ�ʊO�ɐ������ł���
	ACTID_USERHIT1		= (ACTID_SYSTEM | ACTID_KURAI | 0x0010),				//!< ���[�U�[��`��炢�s��(�n��)
	ACTID_USERHIT2		= (ACTID_SYSTEM | ACTID_KURAI | ACTID_KUCYU | 0x0011),	//!< ���[�U�[��`��炢�s��(��)

	ACTID_GUARD1		= (ACTID_SYSTEM | ACTID_GUARD | 0x0001),				//!< �����K�[�h�i���j
	ACTID_GUARD2		= (ACTID_SYSTEM | ACTID_GUARD | 0x0002),				//!< �����K�[�h�i���j
	ACTID_GUARD3		= (ACTID_SYSTEM | ACTID_GUARD | 0x0003),				//!< �����K�[�h�i��j
	ACTID_GUARDC1		= (ACTID_SYSTEM | ACTID_GUARD | ACTID_SYAGAMI | 0x0001),//!< ���Ⴊ�݃K�[�h�i���j
	ACTID_GUARDC2		= (ACTID_SYSTEM | ACTID_GUARD | ACTID_SYAGAMI | 0x0002),//!< ���Ⴊ�݃K�[�h�i���j
	ACTID_GUARDC3		= (ACTID_SYSTEM | ACTID_GUARD | ACTID_SYAGAMI | 0x0003),//!< ���Ⴊ�݃K�[�h�i��j
	ACTID_GUARDJ1		= (ACTID_SYSTEM | ACTID_GUARD | ACTID_KUCYU   | 0x0001),//!< �󒆃K�[�h�i���j
	ACTID_GUARDJ2		= (ACTID_SYSTEM | ACTID_GUARD | ACTID_KUCYU   | 0x0002),//!< �󒆃K�[�h�i���j
	ACTID_GUARDJ3		= (ACTID_SYSTEM | ACTID_GUARD | ACTID_KUCYU   | 0x0003),//!< �󒆃K�[�h�i��j
	ACTID_USERGUARD1	= (ACTID_SYSTEM | ACTID_GUARD | 0x0010),				//!< ���[�U�[��`�K�[�h�s��(�n��)
	ACTID_USERGUARD2	= (ACTID_SYSTEM | ACTID_GUARD | ACTID_KUCYU | 0x0011),	//!< ���[�U�[��`�K�[�h�s��(��)
};

//�f�t�H���gcell id=====================================================================

/*!
*	@brief �f�t�H���gcell ID
*	@ingroup GObject
*	@sa GOBJECT::cnow
*
*	�L�����N�^�[�ɋ�炢��K�[�h�s���Ȃǂ̃f�t�H���g�s�����s�킹�邽�߂ɁA
*	�V�X�e���͂����̃Z���𗘗p����B
*	�܂��A�����Ȃǂŋ�炢���̍s�����U�������L�q����ꍇ�ɂ������̒l�����p�����B
*	�L�����N�^�[�͂����̔ԍ��ɓK�؂ȃZ�������蓖�ĂĂ����Ȃ���΂Ȃ�Ȃ��B
*	�܂��A�����̊g���̂��߂�DLL��҂����R�ɉ摜�����蓖�Ă邱�Ƃ��ł���Z����100�Ԉȍ~�Ƃ���B
*/
enum DEFAULT_CELL_IDs
{
	DCELL_DAMAGES1		= 1,//!< ������炢�|�[�Y1
	DCELL_DAMAGES2		= 2,//!< ������炢�|�[�Y2
	DCELL_DAMAGES3		= 3,//!< ������炢�|�[�Y3
	DCELL_DAMAGES4		= 4,//!< ������炢�|�[�Y4
	DCELL_DAMAGEC1		= 5,//!< ���Ⴊ�݋�炢�|�[�Y1
	DCELL_DAMAGEC2		= 6,//!< ���Ⴊ�݋�炢�|�[�Y2
	DCELL_DAMAGEC3		= 7,//!< ���Ⴊ�݋�炢�|�[�Y3
	DCELL_DAMAGEC4		= 8,//!< ���Ⴊ�݋�炢�|�[�Y4
	DCELL_GUARDS1		= 9,//!< �����K�[�h�|�[�Y1
	DCELL_GUARDS2		=10,//!< �����K�[�h�|�[�Y2
	DCELL_GUARDS3		=11,//!< �����K�[�h�|�[�Y3
	DCELL_GUARDS4		=12,//!< �����K�[�h�|�[�Y4
	DCELL_GUARDC1		=13,//!< ���Ⴊ�݃K�[�h�|�[�Y1
	DCELL_GUARDC2		=14,//!< ���Ⴊ�݃K�[�h�|�[�Y2
	DCELL_GUARDC3		=15,//!< ���Ⴊ�݃K�[�h�|�[�Y3
	DCELL_GUARDC4		=16,//!< ���Ⴊ�݃K�[�h�|�[�Y4
	DCELL_DAMAGEJ		=17,//!< �󒆋�炢�|�[�Y
	DCELL_RAKKA1		=18,//!< ����1
	DCELL_RAKKA2		=19,//!< ����2
	DCELL_GUARDJ1		=20,//!< �󒆃K�[�h�|�[�Y1
	DCELL_GUARDJ2		=21,//!< �󒆃K�[�h�|�[�Y2
	DCELL_GUARDJ3		=22,//!< �󒆃K�[�h�|�[�Y3
	DCELL_BOUND			=23,//!< �������ăo�E���h
	DCELL_DOWN			=24,//!< �_�E��
	DCELL_OKIAGARI1		=25,//!< �N���オ�胂�[�V����1
	DCELL_OKIAGARI2		=26,//!< �N���オ�胂�[�V����2
	DCELL_OKIAGARI3		=27,//!< �N���オ�胂�[�V����3
	DCELL_OKIAGARI4		=28,//!< �N���オ�胂�[�V����4
	DCELL_OKIAGARI5		=29,//!< �N���オ�胂�[�V����5
	DCELL_NAGERARE1		=30,//!< �d�S���L��
	DCELL_KAITENFUTTOBI	=31,//!< ��]������сA�d�S���L��
	DCELL_END			=99,//!< �����܂�
};

/*!
*	@brief �I�u�W�F�N�g�p���b�Z�[�W�����֐��^��`
*	@ingroup GObject
*	@sa GOBJECT
*	@sa GOBJECT_MESSAGE
*
*	����1:���b�Z�[�WID�iGOBJMSG_�`�j
*	����2:�I�u�W�F�N�g�̃f�[�^(GOBJECT�̃|�C���^)
*	����3:�e���b�Z�[�W�ɂ��Ӗ����قȂ�i�قƂ�ǎg���ĂȂ��j
*	�߂�l�F�e���b�Z�[�W�ɂ��Ӗ����قȂ�B0���ƃf�t�H���g�������s���B
*/
typedef DWORD (*PFOBJ_MSGHANDLER)(DWORD,LPVOID,DWORD);


/*!
*	@brief �I�u�W�F�N�g�f�[�^
*	@ingroup GObject
*
*	�I�u�W�F�N�g�̕ێ�����f�[�^�B�d�v�B
*	�������A�����蔻���`��͂��́A�I�u�W�F�N�g��P�ʂƂ��čs����B
*	DLL���V�X�e���փI�u�W�F�N�g�̐�����v������ƁA���̍\���̂����������B
*/
struct GOBJECT
{
	DWORD id;			//!< �I�u�W�F�N�gid
	DWORD uid;			//!< ���[�U�[�����蓖�Ă�id
	DWORD tid;			//!< �`�[��id
	DWORD eid;			//!< �G��id
	PFOBJ_MSGHANDLER msghandler;//!< ���b�Z�[�W�����֐�
	DWORD objtype;		//!< �I�u�W�F�N�g�̎�����
	
	double x,y;			//!< �ʒu
	double vx,vy;		//!< ���x
	double ax,ay;		//!< �����x
	
	DWORD counter;		//!< �J�E���^
	DWORD aid;			//!< �s��ID
	DWORD cnow;			//!< �\���Z��
	int hp;				//!< �̗�
	DWORD hpmax;		//!< �̗͂̍ő�l�B�f�t�H���g��1024
	double gauge;		//!< �Q�[�W
	DWORD gaugemax;		//!< �Q�[�W�̃}�b�N�X�l 1-9�̒l���w��B�^�C�v2�̃Q�[�W�͊֌W�Ȃ�
	DWORD gaugetype;	//!< �ǂ̃p���[�Q�[�W���g�p���邩�B���̃t���O�B�f�t�H���g�̓Q�[�W����

	BOOL muki;			//!< �����iFALSE�͉E�����j
	BOOL revx;			//!< x���]
	BOOL revy;			//!< y���]
	int rot;			//!< ��]
	float magx,magy;	//!< �g�嗦
	float z;			//!< z���W
	DWORD color;		//!< �F�i���O�`��̏ꍇ�����̐F�ŕ`�悵�Ă��������j
	DWORD alphamode;	//!< �������@�iGBLEND_�`�j�t���O ���񐄏�����

	BOOL kougeki;		//!< �U����on/off
	BOOL kasanari;		//!< �d�Ȃ蔻��on/off
	BOOL muteki;		//!< ���Gon/off
	BOOL nagerare;		//!< ������ꔻ��on/off

	MYSURFACE **pmsarr; //!< �r�b�g�}�b�v�̔z��
	GCD_RECT *prdat;	//!< ��`���z��
	GCD_CELL2 *pcdat;	//!< �Z�����z��
	GCD_HANTEI *phdat;	//!< �d�Ȃ�E������E��炢����z��

	ATTACKINFO *atk;	//!< ���܎����������Ă���U�����
	ATTACKINFO2 atk2;	//!< ���̃I�u�W�F�N�g����������U�����
	WAZAINFO  *winfo;	//!< �R���s���[�^�p�Z���

	LPVOID pobjdat_u;	//!< ���[�U�[��`�f�[�^

	BOOL nonstop;		//!< �m���X�g�b�von/off
};

/*!
*	@brief �`�[��ID
*	@ingroup GObject
*	@sa GOBJECT::tid
*
*	�L�����N�^�[�̏ꍇ�ATEAM_PLAYER1 ���ATEAM_PLAYER2���ǂ��炩�ł���B
*	ver0.9�ȍ~�APLAYER���������̂Ōꕾ�����邪...
*/
enum TEAM_IDs
{
	TEAM_PLAYER1			=0,
	TEAM_PLAYER2			=1,
	TEAM_STAGE				=2,
	TEAM_SYSTEM				=3,
};

/*!
*	@brief �I�u�W�F�N�g�v���p�e�B
*	@ingroup GObject
*	@sa GOBJECT::objtype
*
*	�I�u�W�F�N�g�̓���ɂ������e��ݒ�̂��߂̃t���O�ށB
*	�V�X�e���͂����蔻���f�t�H���g����A�����i�s�Ǘ����̏�����
*	�����̃t���O���Q�Ƃ���B
*/
enum GOBJECT_PROPERTIES
{
	//��L�r�b�g				= 0xF1000FEF
	GOBJFLG_KURAI			= 0x00000001,//!< �U������炤
	GOBJFLG_NAGERARE		= 0x00000002,//!< ��������
	GOBJFLG_ATTACK			= 0x00000004,//!< �U�����s��
	GOBJFLG_KASANARI		= 0x00000008,//!< �d�Ȃ蔻������
	GOBJFLG_TOBETARGET		= 0x10000000,//!< �^�[�Q�b�g�ɂȂ邱�Ƃ��ł���B
	GOBJFLG_NEEDTARGET		= 0x20000000,//!< �^�[�Q�b�g(eid)���K�v�ł���B
	GOBJFLG_COMPUTER		= 0x40000000,//!< �R���s���[�^�ɐ��䂳����B�ʏ�̃L�����N�^�[�ɂ̓V�X�e���ɂ���Ă��̃t���O���t�������
	GOBJFLG_ZCHARACTER		= 0x80000000,//!< �L�����N�^�[�u�����v - ���̂Ƃ�����Ɏg���Ă��Ȃ�
	GOBJFLG_ZBULLET			= 0x01000000,//!< ��ѓ���u�����v- ���̔�ѓ���Ƒ��E��������
	GOBJFLG_DISPZAHYO		= 0x00000020,//!< ���W�w��̓f�B�X�v���C��̍��W�ł���A�X�e�[�W��̕\���ʒu�ɍ��E����Ȃ�
	GOBJFLG_DONOTSTOP		= 0x00000040,//!< ���K����stop�G�t�F�N�g���������Ă��Ă�GOBJMSG_ACTION���ʒm�����
	GOBJFLG_HANSAYOU		= 0x00000080,//!< �U�������I�u�W�F�N�g����ʒ[�܂œ��B����Ɖ����߂����i���b�Z�[�W���󂯎�邱�Ƃ��ł���j
	GOBJFLG_CLIPX			= 0x00000100,//!< �I�u�W�F�N�g��x�����Ɋւ��ăX�e�[�W(-640�`640)�O�ɍs�����Ƃ͂ł��Ȃ�
	GOBJFLG_CLIPX2			= 0x00000200,//!< �I�u�W�F�N�g��x�����Ɋւ��ĉ�ʊO�ɏo�邱�Ƃ͂��Ȃ�
	GOBJFLG_DISPLAYME		= 0x00000400,//!< �X�e�[�W��Ńf�B�X�v���C���S���Z�o����Ƃ��ɂ��̃L�����N�^�[�̍��W���Q�Ƃ���
	GOBJFLG_TAIKI			= 0x00000800,//!< �L�����N�^�[�͑ҋ@���ł���
};

//!�W���I�ȃL�����N�^�[�����t���O
#define GOBJTYPE_CHARACTER2	( GOBJFLG_KURAI | GOBJFLG_NAGERARE | GOBJFLG_ATTACK | GOBJFLG_KASANARI | GOBJFLG_CLIPX | GOBJFLG_CLIPX2 | GOBJFLG_DISPLAYME)
//!�W���I�ȃL�����N�^�[�����t���O
#define GOBJTYPE_CHARACTER	( GOBJTYPE_CHARACTER2 | GOBJFLG_TOBETARGET | GOBJFLG_NEEDTARGET | GOBJFLG_ZCHARACTER | GOBJFLG_HANSAYOU | GOBJFLG_CLIPX)

//!�W���I�Ȕ�ѓ���̃t���O
#define GOBJTYPE_BULLET		(GOBJFLG_ATTACK | GOBJFLG_ZBULLET)

//���̑�
#define GOBJTYPE_DISPLAY	( GOBJFLG_CLIPX | GOBJFLG_CLIPX2 | GOBJFLG_DISPLAYME )
#define GOBJTYPE_NODISPLAY	( ~( GOBJFLG_CLIPX | GOBJFLG_CLIPX2 | GOBJFLG_DISPLAYME ) )
#define GOBJTYPE_DEAD		( ~(GOBJFLG_TOBETARGET | GOBJFLG_NEEDTARGET | GOBJFLG_CLIPX | GOBJFLG_CLIPX2 | GOBJFLG_DISPLAYME) )

/*!
*	@brief �Q�[�W�`��^�C�v
*	@ingroup GObject
*	@sa GOBJECT::gaugetype
*
*	�Q�[�W��DLL��CreateCharacter�̖߂�l�ŃI�u�W�F�N�gID�̎w�肳�ꂽ
*	�u�L�����N�^�[�I�u�W�F�N�g�v�ɑ΂��Ă����`��͍s���Ȃ��B
*	�܂��A�����ł̃Q�[�W�̎w��̓Q�[�W�̍ő�l�ɂ͉e����^���Ȃ����Ƃɒ��ӂ��邱�ƁB
*	�Q�[�W�̏���E�����̊Ǘ���DLL���œK�؂ɏ�������K�v������B
*/
enum GOBJECT_GAUGE_TYPE
{
	GOBJFLGA_POWGAUGE_NONE		= 0x00000000,//!< �Q�[�W�Ȃ�(�f�t�H���g)
	GOBJFLGA_POWGAUGE1			= 0x00000001,//!< 9�܂ŃX�g�b�N�\�ȃQ�[�W
	GOBJFLGA_POWGAUGE2			= 0x00000002,//!< �X�g�b�N�ł��Ȃ��Q�[�W
};

/*!
*	@brief ���u�����h�^�C�v
*	@ingroup GObject
*	@sa DI_FUNCTIONS_D::setblend
*	@sa GOBJECT::alphamode
*
*	Direct3D�̃����_�[�X�e�[�g�𑀍삵�ă������̃^�C�v��ύX����B
*	DI_FUNCTIONS_D::setblend�Œl��GBLEND_HANTOUMEI�ȊO�ɐݒ肵���ꍇ��
*	�`����GBLEND_HANTOUMEI�ɐݒ��߂��Ă����K�v������B
*	���AIDirect3DDevice���擾���Ďg�p����΂��̐ݒ���@�ɗ���Ȃ��Ă�
*	���������[�h��ύX���邱�Ƃ��ł���B���������̏ꍇ���`���ɐݒ�����ɖ߂��Ă������ƁB
*/
enum GBLENDMODES
{
	GBLEND_HANTOUMEI			= (0),//!< �������E�ʏ�
	GBLEND_KASAN				= (1),//!< �������E���Z����
	GBLEND_INV					= (3),//!< �������EDest�F���]
};

//  �`���b�Z�[�W�`  ======================================================================
/*!
*	@brief �I�u�W�F�N�g���b�Z�[�WID
*	@ingroup GObject
*	@sa GOBJECT::msghandler
*
*	msghandler�ɓn������2����(�C�x���g���)�̈Ӗ��B
*	�I�u�W�F�N�g�͂����̃��b�Z�[�WID�ɑΉ������K�؂ȏ������s����
*	�e���b�Z�[�W���Ƃɒ�`���ꂽ���L�̖߂�l��Ԃ����A
*	���邢�̓V�X�e���Ƀf�t�H���g�̓�����s�킹�邽�߂�0��Ԃ��Ȃ���΂Ȃ�Ȃ��B
*/
enum GOBJECT_MESSAGE
{
	//��ȃ��b�Z�[�W
	GOBJMSG_DELETE				=   1,//!< �I�u�W�F�N�g���V�X�e���̐���𗣂��Ƃ��ɌĂ΂��
	GOBJMSG_ACTION				=   2,//!< 1�t���[����1��Ă΂��
	GOBJMSG_COMMAND				=   3,//!< �R�}���h���������K�v������Ƃ��ɌĂ΂��
	GOBJMSG_COMMANDCOM			=   4,//!< �R���s���[�^�R�}���h���������Ƃ��ɌĂ΂��
	GOBJMSG_TOUCHA				=   5,//!< �U�����������Ƃ��ɌĂ΂��B
	GOBJMSG_TOUCHB				=   6,//!< �����̍U��������ɓ����������ɌĂ΂��B
	GOBJMSG_TOUCHC				=   7,//!< �����̍U��������ɓ��������Ƃ� (����̏�Ԃ��ω�����O�ɌĂ΂��)�B
	GOBJMSG_CNGAID				=   8,//!< �s��ID���ω������Ƃ��ɌĂ΂��
	GOBJMSG_COMTHINK			=	9,//!< COM�̓���v�l�����鎞�ɌĂ΂��
	//�`��n���b�Z�[�W
	GOBJMSG_DRAW				= 101,//!< �`����s���ۂɌĂ΂��
	GOBJMSG_DRAWBACK			= 102,//!< �w�i�X�e�[�W�ɕ`��
	GOBJMSG_DRAWFRONT			= 103,//!< �O�i�X�e�[�W�ɕ`��
	//�o��E���Ȃǃ��b�Z�[�W
	GOBJMSG_DOTOJYO				= 201,//!< �o��|�[�Y�Ɉڍs���A�I�������MSGOBJ2SYS_TOJYOEND���b�Z�[�W��Ԃ�
	GOBJMSG_DOTIMEOVERLOSE		= 202,//!< �^�C���I�[�o�[�ŕ�����
	GOBJMSG_DOYOUWIN			= 203,//!< �����|�[�Y�Ɉڍs���A�I�������MSGOBJ2SYS_WINPOSEEND���b�Z�[�W��Ԃ�
	GOBJMSG_TAIKI				= 204,//!< ��ʊO�ɑҋ@���Ă��Ă�������
	GOBJMSG_KOUTAI				= 205,//!< �I����B��ʂ̌�����������ł��Ă��������B�i�_���ȂƂ���0��Ԃ��j
	GOBJMSG_KOUTAI2				= 206,//!< �I����B��ʂ̌�����������ł��Ă��������B�I�������MSGOBJ2SYS_KOUTAIEND���b�Z�[�W��Ԃ�
	GOBJMSG_STRIKER				= 207,//!< �X�g���C�J�[�U������炩���Ă��������i�_���ȂƂ���0��Ԃ��j
	GOBJMSG_DOYOUWIN2			= 208,//!< �ҋ@���A���Ԃ̏����ɂ���ă`�[������������
	GOBJMSG_STRIKERREADY		= 209,//!< �X�g���C�J�[�U�����ł��邩�ǂ�����Ԃ�
	//���W����n���b�Z�[�W
	GOBJMSG_KNOCKBACK			= 301,//!< �U�������I�u�W�F�N�g����ʒ[�ɒB����
	GOBJMSG_CLIPX				= 302,//!< ���W����ʊO�ɐ����Ă��܂��Ă����
	//�I�u�W�F�N�g�ԑ��ݍ�p�E���̑�
	GOBJMSG_CNGTARGET			= 401,//!< �^�[�Q�b�g���ς�����Ƃ��ɌĂ΂��
	GOBJMSG_SOUSAI				= 402,//!< ����ѓ���ƐڐG
	//�Q�[���i�s
	GOBJMSG_CNGROUND			= 502,//!< ���E���h���ς�����Ƃ��ɌĂ΂��
	//�l�b�g���[�N
	GOBJMSG_SYNC				= 601,//!< �������b�Z�[�W. ���g�p
	//���̑�
	GOBJMSG_USERDEFINE			=0x80000000,//!< ���[�U�[��`���b�Z�[�W�쐬�p�t���O
};


/*!
*	@brief GOBJMSG_TOUCHA ���b�Z�[�W�ɑ΂���߂�l
*	@ingroup GObject
*	@sa GOBJECT_MESSAGE
*
*	GOBJMSG_TOUCHA(�U����炢)�ɑ΂���߂�l�Ƃ��Ă����̒l���g�p����B
*	�ʏ��TOUCHA_DEFAULT (=0) ��Ԃ��A�V�X�e���Ƀf�t�H���g�̃K�[�h������s�킹��ׂ��ł���B
*/
enum GOBJMSG_TOUCHA_RETURNS
{
	TOUCHA_DEFAULT				= 0x00000000,//!< �f�t�H���g�̃K�[�h������s��
	TOUCHA_KURAIS				= 0x10000001,//!< ������炢
	TOUCHA_KURAIC				= 0x10000002,//!< ����炢
	TOUCHA_KURAIJ				= 0x10000003,//!< �󒆋�炢
	TOUCHA_GUARDS				= 0x20000001,//!< �����K�[�h
	TOUCHA_GUARDC				= 0x20000002,//!< ���K�[�h
	TOUCHA_GUARDJ				= 0x20000003,//!< �󒆃K�[�h
	TOUCHA_MUSI					= 0x50000000,//!< �U���͋��������ǁA��炢������s��Ȃ�
	TOUCHA_MUSI2				= 0x60000000,//!< �U�����K�[�h�������ǁA��炢������s��Ȃ�
	TOUCHA_AVOID				= 0x80000000,//!< �U���������Ȃ�����

	//memo : 0x10000000���q�b�g�A0x20000000���K�[�h�A0x40000000�𖳎��t���O �Ƃ��Ďg�p
};

/*!
*	@brief GOBJMSG_TOUCHC���b�Z�[�W�ɑ΂���߂�l
*	@ingroup GObject
*	@sa GOBJECT_MESSAGE
*
*	GOBJMSG_TOUCHC(�U���q�b�g)�ɑ΂���߂�l�Ƃ��Ă����̒l���g�p����B
*	�����`�̏Փ˂����o����A��炢����GOBJMSG_TOUCHA���b�Z�[�W�����M�����O�ɂ��̃��b�Z�[�W�������s���B
*	TOUCHC_CANCEL��Ԃ��ΏՓ˂��Ȃ��������Ƃɂ��đ����S�Ă̏������L�����Z�����邱�Ƃ��ł���B
*/
enum GOBJMSG_TOUCHC_RETURNS
{
	TOUCHC_OK					= 0x00000000,//!< �f�t�H���g�B�ʏ�ʂ菈�����i�s���܂��B
	TOUCHC_CANCEL				= 0x00000001,//!< ��`�Փ˂��Ȃ��������Ƃɂ��܂��B����ȍ~�̏����͂Ȃ���܂���B
};

/*!
*	@brief �x���U���₢���킹
*	@ingroup GObject
*	@sa GOBJECT_MESSAGE
*
*	GOBJMSG_STRIKERREADY���b�Z�[�W�ɑ΂���߂�l ���邢��DI_FUNCTIONS_S::strikerok�̖߂�l�Ƃ��Ďg�p����B
*	�x���U�����\���ۂ���₢���킹�邽�߂̂��̂ł��邪�A
*	�e�L�����N�^�[�����b�Z�[�W�ւ̉�����������Ǝ������Ă��Ȃ��̂ł��܂�Ӗ��͂Ȃ�(�قƂ�ǂ̏ꍇ�u�s���v���Ԃ�)
*/
enum GOBJMSG_STRIKERREADY_RETURNS
{
	STRIKERREADY_UNKNOWN		= (0),//!< �s��
	STRIKERREADY_OK				= (1),//!< �����I�b�P�[
	STRIKERREADY_NOTREADY		= (2),//!< �ł��Ȃ�
	STRIKERREADY_NOTSUPPORTED	= (3),//!< �[���A���Ƃ��Ƃ��Ȃ�
	STRIKERREADY_NOPOINT		= (4),//!< (sys)�X�g���C�J�[�̃|�C���g������Ȃ�
};

//**************************************************************************
// �V�X�e���֘A�̊֐��̒�`
//**************************************************************************
typedef DWORD	(*PFUNCS_GETKEY)(DWORD,DWORD);			//!< �L�[����get
typedef int		(*PFUNCS_SEEKKEY)(DWORD,int,int,DWORD);	//!< �L�[���͌���
typedef void	(*PFUNCS_SETKATISERIF)(DWORD,char*);	//!< �����䎌�ݒ�
typedef void	(*PFUNCS_ADDEFFECT)(DWORD,int,int,DWORD);//!< �G�t�F�N�g����
typedef void	(*PFUNCS_PLAYSYSSOUND)(DWORD);			//!< �V�X�e��wav�Đ�
typedef LPVOID	(*PFUNCS_LOADMYSOUND)(char*);			//!< wav�ǂݍ���
typedef void	(*PFUNCS_PLAYMYSOUND)(LPVOID);			//!< wav�Đ�
typedef void	(*PFUNCS_KILLMYSOUND)(LPVOID);			//!< wav����
typedef DWORD	(*PFUNCS_MESSAGE2SYSTEM)(DWORD,DWORD,DWORD);	//!< ���b�Z�[�W
typedef char*	(*PFUNCS_GETENEMYNAME)(DWORD);			//!< "�G"�̖��O���擾����
typedef double	(*PFUNCS_GETDISPCENTERX)();				//!< ���݂̉�ʒ��S(X)�𓾂�
typedef double	(*PFUNCS_GETDISPCENTERY)();				//!< ���݂̉�ʒ��S(Y)�𓾂�

typedef DWORD	(*PFUNCS_GETTEAMNUM)();					//!< ���Ή������Q�b�g �� 1P���l�����Ԃ�܂��B�񐄏��֐�
typedef DWORD	(*PFUNCS_GETTAISENKEISIKI)();			//!< �ΐ�`�����Q�g
//typedef DWORD	(*PFUNCS_STRIKEROK)(DWORD tid,DWORD oid,BOOL next);//�X�g���C�J�[�U�����\���ǂ�����₢�l�߂�

typedef BYTE	(*PFUNCS_GETENEMYFACE)(DWORD tid);		//!< �t�F�C�X�ύX�l(�ʏ�0)���擾
typedef DWORD	(*PFUNCS_GETTEAMNUM2)(DWORD tid);		//!< �`�[���l�����擾���܂�
typedef DWORD	(*PFUNCS_TID2ETID)(DWORD tid);			//!< �`�[��ID�ϊ�
typedef BOOL	(*PFUNCS_IS_NETWORK)();					//!< �l�b�g�ΐ킩�ǂ������擾���܂�
typedef void	(*PFUNCS_PUSHDLLEXECTAG)(const char*);	//!< ���s���̃^�O�t�����s��(�G���[���O���ɏo��)
typedef void	(*PFUNCS_POPDLLEXECTAG)();				//!< ���s���̃^�O���ЂƂ폜����
typedef void	(*PFUNCS_LOGERROR)(const char*);		//!< �G���[���O���L�^����
typedef void	(*PFUNCS_LOGWARNING)(const char*);		//!< �x�����O���L�^����
typedef void	(*PFUNCS_LOGDEBUG)(const char*);		//!< �f�o�b�O���O���L�^����
typedef void	(*PFUNCS_LOGINFO)(const char*);			//!< ��񃍃O���L�^����

/*!
*	@brief �V�X�e���֐��|�C���^�Z�b�g
*	@ingroup GObject
*
*	DLL���V�X�e�����̊֐����g�p���邽�߂ɁA�L�����N�^�[�������ɃV�X�e������DLL�ɑ΂��ēn�����B
*	�V�X�e��(?)�Ɋւ���֐��������Ă�B
*	CGoluahObject�p���N���X�̏ꍇ�A�����̊֐��͋K��N���X�ɂقړ����̊֐����痘�p�ł���B(See also �Ń����N)
*/
struct DI_FUNCTIONS_S
{
	PFUNCS_GETKEY			getkey;						//!< �L�[����get	 @sa CGoluahObject::GetKey
	PFUNCS_SEEKKEY			seekkey;					//!< �L�[���͌���  @sa CGoluahObject::SeekKey
	PFUNCS_SETKATISERIF		setkatiserif;				//!< �����䎌�ݒ�  @sa CGoluahObject::SetKatiSerif
	PFUNCS_ADDEFFECT		addeffect;					//!< �G�t�F�N�g����  @sa CGoluahObject::AddEffect
	PFUNCS_PLAYSYSSOUND		playsyssound;				//!< �V�X�e��wav�Đ� @sa CGoluahObject::PlaySysSound
	PFUNCS_LOADMYSOUND		loadmysound;				//!< wav�ǂݍ��� @sa CGoluahObject::LoadMySound
	PFUNCS_PLAYMYSOUND		playmysound;				//!< wav�Đ� @sa CGoluahObject::PlayMySound
	PFUNCS_KILLMYSOUND		killmysound;				//!< wav���� @sa CGoluahObject::KillMySound
	PFUNCS_MESSAGE2SYSTEM	msg2system;					//!< ���b�Z�[�W���M @sa MESSAGE_OBJ2SYS  @sa CGoluahObject::Message2System
	PFUNCS_GETENEMYNAME		getenemyname;				//!< "�G"�̖��O���擾���� @sa CGoluahObject::GetEnemyName
	PFUNCS_GETDISPCENTERX	getdispcenterx;				//!< ���݂̉�ʒ��S(X)�𓾂� @sa CGoluahObject::GetDisplayCenter_X
	PFUNCS_GETDISPCENTERY	getdispcentery;				//!< ���݂̉�ʒ��S(Y)�𓾂� @sa CGoluahObject::GetDisplayCenter_Y
	PFUNCS_GETTEAMNUM		getteamnum;					//!< ���Ή������Q�b�g �� 1P���l�����Ԃ�܂��B�񐄏��֐� @sa CGoluahObject::GetTeamNum
	PFUNCS_GETTAISENKEISIKI	gettaisenkeisiki;			//!< �ΐ�`�����Q�g @sa CGoluahObject::GetTaisenKeisiki
	PFUNCS_GETENEMYFACE		getenemyface;				//!< �t�F�C�X�ύX�l(�ʏ�0)���擾 @sa CGoluahObject::GetEnemyFace
	PFUNCS_GETTEAMNUM2		getteamnum2;				//!< �`�[���l�����擾���܂� @sa CGoluahObject::GetTeamNum2
	PFUNCS_TID2ETID			tid2etid;					//!< �`�[��ID�ϊ� @sa CGoluahObject::TID2EnemyTID
	PFUNCS_IS_NETWORK		is_net;						//!< �l�b�g�ΐ킩�ǂ������擾���܂� @sa CGoluahObject::IsNetwork
//	PFUNCS_STRIKEROK		strikerok;					//�X�g���C�J�[�U�����\���ǂ�����₢�l�߂�
	PFUNCS_PUSHDLLEXECTAG	pushtag;					//!< ���s���̃^�O�t�����s��(�G���[���O���ɏo��) @sa CGoluahObject::PushTag
	PFUNCS_POPDLLEXECTAG	poptag;						//!< ���s���̃^�O���ЂƂ폜���� @sa CGoluahObject::PopTag
	PFUNCS_LOGERROR			logerror;					//!< �G���[���O���L�^���� @sa CGoluahObject::LogError
	PFUNCS_LOGWARNING		logwarning;					//!< �x�����O���L�^���� @sa CGoluahObject::LogWarning
	PFUNCS_LOGDEBUG			logdebug;					//!< �f�o�b�O���O���L�^���� @sa CGoluahObject::LogDebug
	PFUNCS_LOGINFO			loginfo;					//!< ��񃍃O���L�^���� @sa CGoluahObject::LogInfo
};

//�L�[���͒�`===========================================================================

/*!
*	@brief �L�[���͒l
*	@ingroup GObject
*
*	DI_FUNCTIONS_S::getkey ��ADI_FUNCTIONS_S::seekkey�ł����̒l���g�p���܂��B
*	���E�̓L�����N�^�[�̌���(GOBJECT::muki)�ɉe�����󂯂܂�.
*	�u������Ă����ԁv�͉������ςȂ��ł��t���O�������܂��B
*	�u�������ꂽ�Ƃ���v�͉������u��(1�t��)�����t���O�������܂��B
*/
enum GKEYSTATES
{
	KEYSTA_LEFT			= (0x00000100),//!< �����g�p
	KEYSTA_LEFT2		= (0x00000200),//!< �����g�p
	KEYSTA_RIGHT		= (0x00001000),//!< �����g�p
	KEYSTA_RIGHT2		= (0x00002000),//!< �����g�p
	KEYSTA_ALEFT		= (0x00000400),//!< �����g�p
	KEYSTA_ALEFT2		= (0x00000800),//!< �����g�p
	KEYSTA_ARIGHT		= (0x00004000),//!< �����g�p
	KEYSTA_ARIGHT2		= (0x00008000),//!< �����g�p

	KEYSTA_UP			= (0x00000001),	//!< ������(������Ă�����)
	KEYSTA_UP2			= (0x00000002),	//!< ������(�������ꂽ�Ƃ���)
	KEYSTA_DOWN			= (0x00000010),	//!< ������(������Ă�����)
	KEYSTA_DOWN2		= (0x00000020),	//!< ������(�������ꂽ�Ƃ���)
	KEYSTA_FOWORD		= KEYSTA_RIGHT,	//!< �O������(������Ă�����)
	KEYSTA_FOWORD2		= KEYSTA_RIGHT2,//!< �O������(�������ꂽ�Ƃ���)
	KEYSTA_BACK			= KEYSTA_LEFT,	//!< �������(������Ă�����)
	KEYSTA_BACK2		= KEYSTA_LEFT2,	//!< �������(�������ꂽ�Ƃ���)
	KEYSTA_BA			= (0x00010000),	//!< �{�^��1(������Ă�����)
	KEYSTA_BA2			= (0x00020000),	//!< �{�^��1(�������ꂽ�Ƃ���)
	KEYSTA_BB			= (0x00100000),	//!< �{�^��2(������Ă�����)
	KEYSTA_BB2			= (0x00200000),	//!< �{�^��2(�������ꂽ�Ƃ���)
	KEYSTA_BC			= (0x01000000),	//!< �{�^��3(������Ă�����)
	KEYSTA_BC2			= (0x02000000),	//!< �{�^��3(�������ꂽ�Ƃ���)
	KEYSTA_BD			= (0x10000000),	//!< �{�^��4(������Ă�����)
	KEYSTA_BD2			= (0x20000000),	//!< �{�^��4(�������ꂽ�Ƃ���)

	KEYSTA_BUTTONS		= (0x22220000),	//!< �{�^���̂����ꂩ�������ꂽ
};
#define KEYSTA_ANYKEY	KEYSTA_BUTTONS

//Message Id============================================================================
/*!
*	@brief �I�u�W�F�N�g����V�X�e���ւ̃��b�Z�[�W
*	@ingroup GObject
*	@sa DI_FUNCTIONS_S::msg2system
*
*	�����̐i�s�ɂ������C�x���g���b�Z�[�W��A�V�X�e���ɑ΂���v���𑗐M����Ƃ��Ɏg�p���܂��B
*/
enum MESSAGE_OBJ2SYS
{
	MSGOBJ2SYS_TOJYOEND			= 1,//!< �o��f�����I������
	MSGOBJ2SYS_FINALDOWNEND		= 2,//!< �ŏI�_�E�����I������
	MSGOBJ2SYS_WINPOSEEND		= 3,//!< �����f�����I������
	MSGOBJ2SYS_KOUTAI1			= 4,//!< ���v��(�L�����N�^�[1)
	MSGOBJ2SYS_KOUTAI2			= 5,//!< ���v��(�L�����N�^�[2)
	MSGOBJ2SYS_STRIKER1			= 6,//!< �X�g���C�J�[�v��(�L�����N�^�[1)
	MSGOBJ2SYS_STRIKER2			= 7,//!< �X�g���C�J�[�v��(�L�����N�^�[2)
	MSGOBJ2SYS_KOUTAIEND		= 8,//!< ���I���B�퓬�ł����Ԃł�
};

//Effect Id ============================================================================
/*!
*	@brief �V�X�e���G�t�F�N�gID
*	@ingroup GObject
*	@sa DI_FUNCTIONS_S::addeffect
*
*	�V�X�e���������o��ID�B
*/
enum SYSTEM_EFFECT_IDs
{
								     //�R�����g  (�����̈Ӗ�)
	EFCTID_MARK1				=  1,//!<�q�b�g�}�[�N�E��  (x,y,��)
	EFCTID_MARK2				=  2,//!<�q�b�g�}�[�N�E��  (x,y,��)
	EFCTID_MARK3				=  3,//!<�q�b�g�}�[�N�E��  (x,y,��)
	EFCTID_MARK4				=  4,//!<�q�b�g�}�[�N�E��  (x,y,��)
	EFCTID_MARKG				=  5,//!<�K�[�h�}�[�N�E���Ȃ� (x,y,��)
	EFCTID_MARKG2				=  6,//!<�K�[�h�}�[�N�E��肠�� (x,y,��)
	EFCTID_HIKARI				=  7,//!<���H (x,y,0)
	EFCTID_STOP					=  8,//!<�X�g�b�v (�X�g�b�v���Ă鎞��,0,0)
	EFCTID_DARK					=  9,//!<�Â����� (����,0,0)  
	EFCTID_MARKC				= 10,//!<�����͂񂾂Ƃ������G�t�F�N�g (x,y,0)
	EFCTID_GNDCIR				= 11,//!<�ۂ��Ռ��g  (x,y,0)
	EFCTID_SINDO				= 12,//!<�U�����N����  (�U��,��������,0)
	EFCTID_NOBG					= 13,//!<�w�i�̕`��𖳂��ɂ���  (��������,0,0) 
	EFCTID_CYOHI				= 14,//!<���K�����������G�t�F�N�g (x,y,�d������)
	EFCTID_TUBUTUBU				= 15,//!<�p�[�e�B�N���U���E��  (x,y,�����p�����[�^) @sa TUBUTUBUPRM
	EFCTID_TUBUTUBU2			= 16,//!<�p�[�e�B�N���U���E��  (x,y,�����p�����[�^) @sa TUBUTUBUPRM
	EFCTID_COMEON				= 17,//!<�x�����ĂԂƂ������G�t�F�N�g  (x,y,��)
	EFCTID_BURN					= 18,//!<�R����E��  (-,-,�I�u�W�F�N�gID)
	EFCTID_BURN_B				= 19,//!<�R����E��  (-,-,�I�u�W�F�N�gID)
	EFCTID_BURN_G				= 20,//!<�R����E��  (-,-,�I�u�W�F�N�gID)
	EFCTID_GCANCEL				= 21,//!<�K�[�h�L�����Z���������G�t�F�N�g  (x,y,��)
	EFCTID_SUPERARMER			= 22,//!<�X�[�p�[�A�[�}�[�������G�t�F�N�g  (x,y,��)
	EFCTID_HATTEN				= 23,//!<�B���G�t�F�N�g  (x,y)
};

//�Ԃԃp�����[�^���͗p�}�N��
#define TUBUTUBUPRM(a,b,c,d,e,f,g,h)	((DWORD)(a*256*256*256)*16+(b*256*256*256)+(c*256*256*16)+(d*256*256)+(e*256*16)+(f*256)+(g*16)+h)
//a:���̐�(0�`15)
//b;���̑��x
//c:���̑��x�̂΂��
//d:�Œᑬ�x
//e:�����ʒu�̂΂��
//f:�����̂΂��
//g:���̍ŒZ����
#define TUBUTUBU_GOUP				0x00000001//!< ������ɕ΂������x����������
#define TUBUTUBU_GODOWN				0x00000002//!< �����ɕ΂������x����������
#define TUBUTUBU_GOLEFT				0x00000004//!< ��������
#define TUBUTUBU_GORIGHT			0x00000008//!< �E

//Sound ID =============================================================================
/*!
*	@brief �V�X�e���G�t�F�N�gID
*	@ingroup GObject
*	@sa DI_FUNCTIONS_S::playsyssound
*
*	�V�X�e�����Ŏ����Ă�����ʉ���ID�B
*/
enum SYSTEM_SOUND_IDs
{
	SYSTEMSOUND_HIT1			= 0,//!< hit1.wav �q�b�g��
	SYSTEMSOUND_HIT2			= 1,//!< hit2.wav
	SYSTEMSOUND_HIT3			= 2,//!< hit3.wav
	SYSTEMSOUND_SHOCK1			= 3,//!< shock1.wav �g�����Ǝv���Ύg���邪�A���g�p
	SYSTEMSOUND_SHOCK2			= 4,//!< shock2.wav �g�����Ǝv���Ύg���邪�A���g�p
	SYSTEMSOUND_GUARD			= 5,//!< guard.wav �K�[�h��
	SYSTEMSOUND_CATCH			= 6,//!< catch.wav �͂݉�
	SYSTEMSOUND_CYOHI			= 7,//!< cyohi.wav ���K�������̌��ʉ�
	SYSTEMSOUND_CHARGE			= 8,//!< charge.wav �Q�[�W��1�{���܂����Ƃ��̌��ʉ�
};

/*!
*	@brief �ΐ�`��
*	@ingroup GObject
*	@sa DI_FUNCTIONS_S::gettaisenkeisiki
*/
enum TAISENKEISIKI
{
	TAISENKEISIKI_GOCYAMAZE		=1,	//!< �����ΐ�
	TAISENKEISIKI_KOUTAI		=2,	//!< ��゠��
	TAISENKEISIKI_JYUNBAN		=3,	//!< ���Ȃ�
};

//**************************************************************************
//�@�I�u�W�F�N�g�֘A�̊֐��̒�`
//**************************************************************************
typedef DWORD  (*PFUNCO_CREATE)();										//!< �I�u�W�F�N�g����
typedef void   (*PFUNCO_DELETE)(DWORD);									//!< �I�u�W�F�N�g���� �񐄏�
typedef BOOL   (*PFUNCO_CATCH)(DWORD,LPVOID);							//!< �����Ƃ��ő��������
typedef LPVOID (*PFUNCO_GETINFO)(DWORD);								//!< GOBJECT�\���̎擾
typedef void   (*PFUNCO_ADDDAMAGE)(DWORD,DWORD,int x,int y);			//!< (��ɓ����̂Ƃ�)�����I�Ƀ_���[�W��^����
typedef void   (*PFUNCO_ACTIDCHANGED)(DWORD);							//!< �s��ID���ω������Ƃ��̏����𖾎��I�ɍs��
typedef void*  (*PFUNCO_GETACTIVECHARACTER)(DWORD);						//!< ���݃A�N�e�B�u�ȁ��L�����N�^�[�̏��
typedef void   (*PFUNCO_SUICIDE)(DWORD);								//!< �I�u�W�F�N�g���� Fx�I�u�W�F�N�g�ȊO�͔񐄏�
typedef DWORD  (*PFUNCO_GETMAAI_H)(DWORD oid,DWORD eoid);				//!< �ԍ�����get(����)
typedef DWORD  (*PFUNCO_GETMAAI_V)(DWORD oid,DWORD eoid);				//!< �ԍ�����get(����)
typedef DWORD  (*PFUNCO_GETCHARACTERID)(DWORD tid,DWORD index);			//!< �L�����N�^�[�̃I�u�W�F�N�gID�擾

typedef DWORD  (*PFUNCO_GETCOMLEVEL)(DWORD oid);						//!< COM���x���擾
typedef void   (*PFUNCO_SETCOMREACH)(DWORD oid,DWORD idx,int length);	//!< COM����̃��[�`����ݒ� idx=0:�Z , 1:�� , 2:��
typedef void   (*PFUNCO_SETCOMACT)(DWORD oid,DWORD aid,int delay);		//!< COM�s������. aid:�J�ڍs��ID(0�͗}��) ,delay:�����x��
typedef DWORD  (*PFUNCO_CREATE_FX)();									//!< �G�t�F�N�g�p�I�u�W�F�N�g����
typedef BOOL   (*PFUNCO_IS_LOCAL)(DWORD oid);							//!< ���[�J�����ǂ���

typedef void   (*PFUNCO_DACTION)(GOBJECT* pdat);						//!< �w��I�u�W�F�N�g�Ƀf�t�H���g�����������
typedef void   (*PFUNCO_SETCOMRANGE)(DWORD oid,DWORD idx);				//!< COM����̃��[�`����ݒ� idx=0:�Z , 1:�� , 2:��
typedef char*	(*PFUNCO_GETCHARNAME)(DWORD);							//!< �w��L�����̖��O���擾����
typedef DWORD	(*PFUNCO_GETKEYINPUT)(DWORD);							//!< �w��L�����̃L�[ID���擾����


//���A�N�e�B�u : �ΐ�`���ɂ����܂����A��{�I�ɉ�ʂɏo�Đ���Ă�L�����N�^�[��Ԃ��悤�w�͂��܂�
//				�����܂��Ȓ�`�ł�

#define COMIDX_NEAR		0
#define COMIDX_MIDDLE	1
#define COMIDX_LONG		2

/*!
*	@brief �I�u�W�F�N�g�֐��|�C���^�Z�b�g
*	@ingroup GObject
*
*	DLL���V�X�e�����̊֐����g�p���邽�߂ɁA�L�����N�^�[�������ɃV�X�e������DLL�ɑ΂��ēn�����B
*	�I�u�W�F�N�g�Ɋւ���֐��������Ă�B
*	CGoluahObject�p���N���X�̏ꍇ�A�����̊֐��͋K��N���X�ɂقړ����̊֐����痘�p�ł���B(See also �Ń����N)
*/
struct DI_FUNCTIONS_O
{
	PFUNCO_CREATE			objcreate;		//!< �I�u�W�F�N�g����
	PFUNCO_DELETE			objdelete;		//!< �I�u�W�F�N�g���� �񐄏�
	PFUNCO_CATCH			objcatch;		//!< �����Ƃ��ő��������  @sa CGoluahObject::ObjCatch
	PFUNCO_GETINFO			getinfo;		//!< GOBJECT�\���̎擾  @sa CGoluahObject::GetInfo
	PFUNCO_ADDDAMAGE		adddamage;		//!< (��ɓ����̂Ƃ�)�����I�Ƀ_���[�W��^����  @sa CGoluahObject::AddDamage
	PFUNCO_ACTIDCHANGED		actidchanged;	//!< �s��ID���ω������Ƃ��̏����𖾎��I�ɍs��  
	PFUNCO_GETACTIVECHARACTER getactivechar;//!< ���݃A�N�e�B�u�ȃL�����N�^�[�̏��  @sa CGoluahObject::GetActiveCharacter
	PFUNCO_SUICIDE			suicide;		//!< �I�u�W�F�N�g���� �G�t�F�N�g�I�u�W�F�N�g�ȊO�͔񐄏�  @sa CGoluahObject::Suicide
	PFUNCO_GETMAAI_H		getmaai_h;		//!< �ԍ�����get(����)  @sa CGoluahObject::GetMaai_H
	PFUNCO_GETMAAI_V		getmaai_v;		//!< �ԍ�����get(����)  @sa CGoluahObject::GetMaai_V
	PFUNCO_GETCHARACTERID	getcharid;		//!< �L�����N�^�[�̃I�u�W�F�N�gID�擾  @sa CGoluahObject::GetCharacterID

	PFUNCO_GETCOMLEVEL		getcomlevel;	//!< COM���x���擾  @sa CGoluahObject::GetComLevel
	PFUNCO_SETCOMREACH		setcomreach;	//!< COM����̃��[�`����ݒ�  @sa CGoluahObject::SetComReach
	PFUNCO_SETCOMACT		setcomact;		//!< COM�s������. aid:�J�ڍs��ID(0�͗}��) ,delay:�����x��  @sa CGoluahObject::SetComAct
	PFUNCO_CREATE_FX		objcreate_fx;	//!< �G�t�F�N�g�p�I�u�W�F�N�g����
	PFUNCO_IS_LOCAL			obj_is_local;	//!< ���[�J���ȃI�u�W�F�N�g���ǂ���  @sa CGoluahObject::IsLocal

	PFUNCO_DACTION			dact_damages1;	//!< HITINFO_REACT1 �𗧂���Ԃŋ������Ƃ��̍s��
	PFUNCO_DACTION			dact_damages2;	//!< HITINFO_REACT2 �𗧂���Ԃŋ������Ƃ��̍s��
	PFUNCO_DACTION			dact_damages3;	//!< HITINFO_REACT3 �𗧂���Ԃŋ������Ƃ��̍s��
	PFUNCO_DACTION			dact_damagec1;	//!< HITINFO_REACT1 �����Ⴊ�ݏ�Ԃŋ������Ƃ��̍s��
	PFUNCO_DACTION			dact_damagec2;	//!< HITINFO_REACT2 �����Ⴊ�ݏ�Ԃŋ������Ƃ��̍s��
	PFUNCO_DACTION			dact_damagec3;	//!< HITINFO_REACT3 �����Ⴊ�ݏ�Ԃŋ������Ƃ��̍s��
	PFUNCO_DACTION			dact_damagej1;	//!< HITINFO_REACT1 or HITINFO_REACT1A ���󒆏�Ԃŋ������Ƃ��̍s��
	PFUNCO_DACTION			dact_damagej2;	//!< HITINFO_REACT2 or HITINFO_REACT2A ���󒆏�Ԃŋ������Ƃ��̍s��
	PFUNCO_DACTION			dact_damagej3;	//!< HITINFO_REACT3 or HITINFO_REACT3A ���󒆏�Ԃŋ������Ƃ��̍s��
	PFUNCO_DACTION			dact_damages1a;	//!< HITINFO_REACT1A �𗧂���Ԃŋ������Ƃ��̍s��
	PFUNCO_DACTION			dact_damages2a;	//!< HITINFO_REACT2A �𗧂���Ԃŋ������Ƃ��̍s��
	PFUNCO_DACTION			dact_damages3a;	//!< HITINFO_REACT3A �𗧂���Ԃŋ������Ƃ��̍s��
	PFUNCO_DACTION			dact_damagec1a;	//!< HITINFO_REACT1A �����Ⴊ�ݏ�Ԃŋ������Ƃ��̍s��
	PFUNCO_DACTION			dact_damagec2a;	//!< HITINFO_REACT2A �����Ⴊ�ݏ�Ԃŋ������Ƃ��̍s��
	PFUNCO_DACTION			dact_damagec3a;	//!< HITINFO_REACT3A �����Ⴊ�ݏ�Ԃŋ������Ƃ��̍s��
	PFUNCO_DACTION			dact_guards1;	//!< HITINFO_REACT1 or HITINFO_REACT1A �𗧂���ԂŃK�[�h�����Ƃ��̍s��
	PFUNCO_DACTION			dact_guards2;	//!< HITINFO_REACT2 or HITINFO_REACT2A �𗧂���ԂŃK�[�h�����Ƃ��̍s��
	PFUNCO_DACTION			dact_guards3;	//!< HITINFO_REACT3 or HITINFO_REACT3A �𗧂���ԂŃK�[�h�����Ƃ��̍s��
	PFUNCO_DACTION			dact_guardc1;	//!< HITINFO_REACT1 or HITINFO_REACT1A �����Ⴊ�ݏ�ԂŃK�[�h�����Ƃ��̍s��
	PFUNCO_DACTION			dact_guardc2;	//!< HITINFO_REACT2 or HITINFO_REACT2A �����Ⴊ�ݏ�ԂŃK�[�h�����Ƃ��̍s��
	PFUNCO_DACTION			dact_guardc3;	//!< HITINFO_REACT3 or HITINFO_REACT3A �����Ⴊ�ݏ�ԂŃK�[�h�����Ƃ��̍s��
	PFUNCO_DACTION			dact_guardj1;	//!< HITINFO_REACT1 or HITINFO_REACT1A ���󒆏�ԂŃK�[�h�����Ƃ��̍s��
	PFUNCO_DACTION			dact_guardj2;	//!< HITINFO_REACT2 or HITINFO_REACT2A ���󒆏�ԂŃK�[�h�����Ƃ��̍s��
	PFUNCO_DACTION			dact_guardj3;	//!< HITINFO_REACT3 or HITINFO_REACT3A ���󒆏�ԂŃK�[�h�����Ƃ��̍s��
	PFUNCO_DACTION			dact_down;		//!< HITINFO_DOWN ���������Ƃ��̍s��
	PFUNCO_DACTION			dact_down2;		//!< �_�E�����
	PFUNCO_DACTION			dact_okiagari;	//!< �W���N���オ��
	PFUNCO_DACTION			dact_futtobi;	//!< HITINFO_FUTTOBI  ���������Ƃ��̍s��
	PFUNCO_DACTION			dact_futtobi2;	//!< HITINFO_FUTTOBI2 ���������Ƃ��̍s��
	PFUNCO_DACTION			dact_tatakituke1a;	//!< HITINFO_TATAKI1A ���������Ƃ��̍s��
	PFUNCO_DACTION			dact_tatakituke1b;	//!< HITINFO_TATAKI1B ���������Ƃ��̍s��
	PFUNCO_DACTION			dact_tatakituke2a;	//!< HITINFO_TATAKI2A ���������Ƃ��̍s��
	PFUNCO_DACTION			dact_tatakituke2b;	//!< HITINFO_TATAKI2B ���������Ƃ��̍s��

	PFUNCO_SETCOMRANGE		setcomrange;	//!< com����̂Ƃ��郌���W�ݒ� @sa CGoluahObject::SetComRange
	PFUNCO_GETCHARNAME		getcharname;	//!< �w��L�����̖��O���擾
	PFUNCO_GETKEYINPUT		getkeyinput;	//!< �w��L�����̃L�[ID���擾
};

/*!
*	@brief ������ݏ��
*	@ingroup GObject
*	@sa DI_FUNCTIONS_O::objcatch
*
*	�I�u�W�F�N�g�֐�objcatch ��CATCHYOU�\���̂̃|�C���^�����Ď��s���ATRUE���Ԃ�Γ����J�n
*	�n�����֐��� ����̍s���֐��ɂȂ�B�\���Z���ɂ̓f�t�H���g�Z���𗘗p����
*
*	��������ԂɈڍs�����I�u�W�F�N�g�ɂ�GOBJMSG_ACTION�����M����Ȃ��Ȃ�B
*	���̑���ɓ������I�u�W�F�N�g�ɑ΂��Ă����Ŏw�肵�����[�U�[���b�Z�[�W���A����������
*	�I�u�W�F�N�g�ɑ΂��đ��M�����B���̃��b�Z�[�W�ɑ΂���0�ȊO(�f�t�H���g�s��ID�̂����ꂩ)
*	��Ԃ����Ƃ��ɁA�I�u�W�F�N�g�͓�������Ԃ���J�������B�������͎w�肵�����b�Z�[�WID�ɑΉ�����
*	�K�؂ȏ������s�����ƁB
*/
struct CATCHYOU
{
	DWORD oid;		//!< �͂ރI�u�W�F�N�g
	DWORD actmsg;	//!< �͂񂾃I�u�W�F�N�g���A���܂ꂽ�I�u�W�F�N�g�𑀍삷�邽�߂Ɏg�����[�U�[��`���b�Z�[�W
};

//**************************************************************************
//�@�`��֘A�̊֐��̒�`
//**************************************************************************

//*�`��n�֐�*
typedef LPVOID (*PFUNCD_GETD3D)();							//!< IDirect3D* �̎擾
typedef LPVOID (*PFUNCD_GETD3DD)();							//!< IDirect3DDevice* �̎擾
typedef void   (*PFUNCD_LOADCELLDAT)(char*,LPVOID,LPVOID,LPVOID);//!< GCD�f�[�^�ǂݍ��݊֐�
typedef LPVOID (*PFUNCD_LOADBMP)(char*,char*);				//!< �r�b�g�}�b�v�ǂݍ��݊֐�
typedef void   (*PFUNCD_UNLOADBMP)(LPVOID);					//!< �r�b�g�}�b�v��n���֐�
typedef void   (*PFUNCD_CELLDRAW)(LPVOID*,LPVOID,LPVOID,DWORD,int,int,float,int,BOOL,BOOL,DWORD,float,float);//!< �Z���`��֐�
typedef void   (*PFUNCD_CKBLT)(void*,int,int,RECT,double,double,BOOL,BOOL,float,DWORD);//!< Blt2
typedef void   (*PFUNCD_BLT3D)(void*,RECT,MYRECT3D,DWORD);	//!< Blt3
typedef void   (*PFUNCD_SETTRANSFORM)(BOOL);				//!< �ϊ��s��ݒ�
typedef void   (*PFUNCD_SETBLEND)(DWORD);					//!< �u�����h�̎d����ύX
typedef void   (*PFUNCD_SETPARENTMATRIX)(LPVOID,BOOL,LPVOID);//!< �u�e�v�̕ϊ��s��ݒ�
typedef DWORD  (*PFUNCD_CREATEGCDDAT)(char* filename,LPVOID *cdat,LPVOID *rdat,LPVOID *hdat);//!< �Z���f�[�^�ǂݍ��݁Bver0.90�ȍ~�ł�LoadCellDat���炱����ɕύX����ׂ�
typedef void   (*PFUNCD_DESTROYGCDDAT)(LPVOID *cdat,LPVOID *rdat,LPVOID *hdat);//!< CreateCellDat�Ő������ꂽ�o�b�t�@���N���A
typedef LPVOID (*PFUNCD_LOADIMAGE)(char*,char*);			//!< �摜�ǂݍ��݊֐��APNG�Ή��ɔ����ǉ��B
typedef DWORD  (*PFUNCD_CREATEGCDDAT2)(char* filename,LPVOID *cdat,LPVOID *rdat,LPVOID *hdat);//!< �Z���f�[�^�ǂݍ��݁Bver0.90�ȍ~�ł�LoadCellDat���炱����ɕύX����ׂ�

/*!
*	@brief �`��n�֐��|�C���^�Z�b�g
*	@ingroup GObject
*
*	DLL���V�X�e�����̊֐����g�p���邽�߂ɁA�L�����N�^�[�������ɃV�X�e������DLL�ɑ΂��ēn�����B
*	�`��Ɋւ���֐��������Ă�B
*	CGoluahObject�p���N���X�̏ꍇ�A�����̊֐��͋K��N���X�ɂقړ����̊֐����痘�p�ł���B(See also �Ń����N)
*/
struct DI_FUNCTIONS_D
{
	PFUNCD_GETD3D		getd3d;				//!< IDirect3D* �̎擾  @sa CGoluahObject::GetD3D
	PFUNCD_GETD3DD		getd3dd;			//!< IDirect3DDevice* �̎擾  @sa CGoluahObject::GetD3DDevice
	PFUNCD_LOADCELLDAT	loadcelldat;		//!< GCD�f�[�^�ǂݍ��݊֐�  @sa CGoluahObject::LoadCellData
	PFUNCD_LOADBMP		loadbmp;			//!< �r�b�g�}�b�v�ǂݍ��݊֐�  @sa CGoluahObject::LoadBitmap
	PFUNCD_UNLOADBMP	unloadbmp;			//!< �r�b�g�}�b�v��n���֐�  @sa CGoluahObject::UnloadBitMap
	PFUNCD_CELLDRAW		celldraw;			//!< �Z���`��֐�  @sa CGoluahObject::CellDraw
	PFUNCD_CKBLT		ckblt;				//!< �r�b�g�}�b�v�`��(1)  @sa CGoluahObject::CkBlt
	PFUNCD_BLT3D		blt3d;				//!< �r�b�g�}�b�v�`��(2)  @sa CGoluahObject::Blt3D
	PFUNCD_SETTRANSFORM settrans;			//!< ���W�ϊ��s��ݒ�  @sa CGoluahObject::SetTransform
	PFUNCD_SETBLEND		setblend;			//!< ���u�����h�̎d����ύX  @sa CGoluahObject::SetBlend
	PFUNCD_SETPARENTMATRIX setparentmat;	//!< �u�e�v���W�ϊ��s��ݒ�  @sa CGoluahObject::SetParentMatrix

	PFUNCD_CREATEGCDDAT create_celldat;		//!< �Z���f�[�^�ǂݍ��݁B�o�[�W������������  @sa CGoluahObject::CreateCellData
	PFUNCD_DESTROYGCDDAT destroy_celldat;	//!< create_celldat�Ő������ꂽ�o�b�t�@��j��  @sa CGoluahObject::DestroyCellData
	PFUNCD_LOADIMAGE	loadimage;			//!< �摜�ǂݍ��݊֐��APNG�Ή��ɔ����ǉ��B  @sa CGoluahObject::LoadImage
	PFUNCD_CREATEGCDDAT2 create_celldat2;	//!< �Z���f�[�^�ǂݍ��݁i�Q�j�B�o�[�W�����A�g���q��������  @sa CGoluahObject::CreateCellData2
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
#define CDI_VERSION 1100

//********************************************************************************
//�@�L�����N�^�[�I�v�V��������`
//********************************************************************************
/*!
*	@brief �L�����N�^�[�I�v�V������`�\����
*	@ingroup GObject
*/
struct CHARACTER_LOAD_OPTION
{
	DWORD flag;			//!< �I�v�V������ON�̏ꍇ�ɗ��t���O
	DWORD exclusive;	//!< �����Ɏw��ł��Ȃ��ق��̃I�v�V����
	DWORD depends;		//!< �����Ɏw�肳��Ă��Ȃ���΂Ȃ�Ȃ��ق��̃I�v�V����
	char name[64];		//!< �I�v�V������
	char point;			//!< �I�v�V��������|�C���g
};

#define CHARACTER_LOAD_OPTION_FACENUMBERMASK (0xF0000000)//!< �����̃r�b�g��face�ύX�̂��߂Ɏg�p����
#define CHARACTER_LOAD_OPTION_FACE_A		 (0x10000000)//!< face2a , face3a
#define CHARACTER_LOAD_OPTION_FACE_B		 (0x20000000)//!< face2b , face3b
#define CHARACTER_LOAD_OPTION_FACE_C		 (0x30000000)//!< face2c , face3c �ȉ�,��`�͂��Ȃ�����p(0xF)�܂œ��l

//**************************************************************************
// CharacterInfo�֐��œn�����\����
//**************************************************************************

typedef DWORD (*PFUNC_CHARACTERINFO)(LPVOID);
/*!
	@brief CharacterInfo�֐��œn�����\����
	@ingroup GObject

	�V�X�e��������DLL���ɓn�����B
	DLL�͂��̍\���̂ɓK�؂Ȓl��ݒ肵��CharacterInfo�֐��̐����Ԃ��B

	�Ȃ񂩁A���ނ�n����āu�����ɕK�v�������L�����ĕԂ��Ăˁv�݂����ȃm���B
*/
struct CDI_CHARACTERINFO
{
	DWORD system_version;				//!< �V�X�e���̃o�[�W�����B�iver0.9�ȑO��0���͂����Ă�Ƃ����܂��j
	char dummy[28];						//!< ���g�p
	char name[32];						//!< �R�R�ɖ��O���R�s�[���邱��
	DWORD caps;							//!< �����ɏ���ݒ�iver0.9�ȍ~�j
	LPVOID dummy2[3];					//!< ���g�p
	DWORD ver;							//!< �R�R�Ƀo�[�W�������R�s�[���邱��
	//ver0.9�ȍ~�Œǉ�
	CHARACTER_LOAD_OPTION options[32];	//!< �L�����I�����ɐݒ�\�ȃI�v�V����
	DWORD max_option_point;				//!< �I�v�V�����|�C���g�̍ő�l
};

//caps�ɐݒ肷��t���O
#define CHARACTER_CAPS_NET		0x00000001//!< �l�b�g���[�N�ΐ�Ŏg�p�\
#define CHARACTER_CAPS_OFFLINE	0x00000002//!< ���A�I�t���C�����[�h�ł��g�p�\


//********************************************************************************
//�@CreateCharacter �̈����ɓn�����\����
//********************************************************************************
typedef DWORD (*PFUNC_CREATECHARACTER)(LPVOID);
/*!
*	@brief CreateCharacter �̈����ɓn�����\����
*	@ingroup GObject
*
*	���̏��́A�L�����N�^�[�����㎎���������ƕK�v�ɂȂ郂�m���l�܂��Ă���̂ŁA
*	DLL�͓K�؂ȏꏊ�ɂ����̏��̃R�s�[���쐬���ĊǗ����邱�ƁB
*/
struct CDI_CHARACTERINFO2
{
	char dir[64];					//!< dll������f�B���N�g��
	DWORD tid;						//!< �`�[��ID(0:1P���A1:2P��)
	DWORD color;					//!< �ǂ̐F�ɂ��邩�ipal?.bmp�j
	DWORD keyinput;					//!< �ǂ�������͂��󂯎�邩
	DI_FUNCTIONS_S *funcs;			//!< �V�X�e���֐��|�C���^�Z�b�g(�V�X�e��)
	DI_FUNCTIONS_O *funco;			//!< �V�X�e���֐��|�C���^�Z�b�g(�I�u�W�F�N�g)
	DI_FUNCTIONS_D *funcd;			//!< �V�X�e���֐��|�C���^�Z�b�g(�`��)
	//ver0.9�ȍ~�Œǉ�
	BYTE dllid;						//!< DLL��ID
	DWORD options_flag;				//!< �I�����ꂽ�I�v�V�����t���O
};

//**************************************************************************
// �X�e�[�WDLL�̃o�[�W����
//**************************************************************************

#define SDI_VERSION 1100//�������Ă邩���E�E�E


//**************************************************************************
// StageInfo�֐��œn�����\����
//**************************************************************************

typedef DWORD (*PFUNC_STAGEINFO)(LPVOID);
/*!
*	@brief StageInfo�֐��œn�����\����
*	@ingroup GObject
*	@sa CDI_CHARACTERINFO
*
*	�X�e�[�WDLL��PFUNC_STAGEINFO�̌`���A���O�uStageInfo�v�ŊO���ɃG�N�X�|�[�g����
*	�֐��ł��̍\���̂̃|�C���^���󂯎��B
*	name�����o�̓V�X�e���ɂ���Ď����I�ɃX�e�[�W�f�B���N�g����name.txt�����[�h�����̂ŁA
*	DLL�͂��̊֐�����ver�����o�ɓK�؂Ȓl�iSDI_VERSION�j��ݒ肵�A������Ԃ����ƁB
*/
struct SDI_STAGEINFO
{
	char name[32];	//!< �X�e�[�W��
	DWORD ver;		//!< �o�[�W����
};

//**************************************************************************
// CreateStage�֐��œn�����\����
//**************************************************************************

typedef DWORD (*PFUNC_CREATESTAGE)(LPVOID);
/*!
*	@brief CreateStage�֐��œn�����\����
*	@ingroup GObject
*	@sa CDI_CHARACTERINFO2
*
*	��{�I�ɂ�CDI_CHARACTERINFO2�Ɠ����ł���B
*	�X�e�[�WDLL��PFUNC_STAGEINFO�̌`���A���O�uCreateStage�v�ŊO���ɃG�N�X�|�[�g����
*	�֐��ł��̍\���̂̃|�C���^���󂯎��B
*/
struct SDI_STAGEINFO2
{
	char dir[64];	//dll������f�B���N�g��
	DWORD tid;		//�`�[��ID(���2(�X�e�[�W))
	DI_FUNCTIONS_S *funcs;
	DI_FUNCTIONS_O *funco;
	DI_FUNCTIONS_D *funcd;
};


/*@}*/