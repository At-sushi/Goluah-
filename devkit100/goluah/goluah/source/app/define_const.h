
/*============================================================================

	�萔�E�v���v���Z�b�T ��`

==============================================================================*/
#ifndef _DEFINE_CONST_H_
#define _DEFINE_CONST_H_

#define GOLUAH_SYSTEM_VERSION	(940)
//�萔
#define MAXNUM_TEAM				3		//!< 1�`�[���ő�l��
#define MAXNUM_KEYI				6		//!< �L�[���͍ő吔
#define GSCMAX_CHARACTERDLL		6		//!< ��x�ɓǂݍ��܂��\���̂���DLL�̐�
#define RDMGKANWA				0.9		//!< �_���[�W�ɘa�W��
#define DMGKANWA_MINIMUM		0.5		//!< �_���[�W���ɘa�����ŏ��l
#define ASPECTRATIO				(320.0f/240.0f)//!< ��ʂ̃^�e���R��
#define MAXNUM_CHARACTERCOLOR	3		//!< �L�����N�^�[�̃p���b�g�ő吔�B1-3���L��

//�L�����I�����v���C���[�i�L�[���͔ԍ��j���蓖�Ē萔
#define CASSIGN_SPECIFIC	0x80						//!< �L�[���͔ԍ��ȊO�́A���ʂȈӖ������萔�ł���Ƃ����t���O
#define CASSIGN_NET			0x40						//!< �l�b�g���[�N�L�[(���g�p)
#define CASSIGN_COM			(CASSIGN_SPECIFIC|0x01)		//!< �R���s���[�^
#define CASSIGN_NONE		(CASSIGN_SPECIFIC|0x02)		//!< ���蓖�ĂȂ�
#define CASSIGN_STABLE		(CASSIGN_SPECIFIC|0x03)		//!< ����ł��Ȃ�

//�萔����2
#define MAXNUM_CDLL				(MAXNUM_TEAM*2)			//!< �L�����N�^DLL�ő吔

//!��prefix�̂ق��̃t���O�Ƃ��Ԃ�Ȃ��悤�ɁI
#define CHARACTER_CAPS_NET2		0x10000000


//!�X�g�[���[�X�N���v�g�ł́A�I�v�V�����w��̎��
enum StoryOptType{
	Opt_Random = 0,		//!< �����_��
	Opt_UserSelect = 1,	//!< ���[�U�[�ɑI�΂���
	Opt_Fix	= 2			//!< 2�i�����ڎw��ŌŒ肳�ꂽ
};

//�r�b�g�}�b�v�e�L�X�g
#define SYSBMPTXT_PROP		0x00000001//!< �v���|�[�V���i��
#define SYSBMPTXT_R2L		0x00000002//!< �E���獶�֕`�悷��
#define SYSBMPTXT_ALPHAADD	0x00000004//!< ���Z�����ŕ`�悷��
#define SYSBMPTXT_SHADE		0x00000010//!< �e��(1)
#define SYSBMPTXT_SHADE_W	0x00000020//!< �^�����e��
#define SYSBMPTXT_SHADE_B	0x00000030//!< �^�����e��
#define SYSBMPTXT_SHADEMASK 0x000000F0
#define SYSBMPTXT_DEFAULT	(SYSBMPTXT_PROP)

/*--------------------------------------------------------------------------------
	system private effectid �i���[�U�[�Ɍ��J���Ȃ����́j
----------------------------------------------------------------------------------*/
#define EFCTID_ROUND1		(0x80000000 | 0x00000001)
#define EFCTID_ROUND2		(0x80000000 | 0x00000002)
#define EFCTID_ROUND3		(0x80000000 | 0x00000003)
#define EFCTID_ROUND4		(0x80000000 | 0x00000004)
#define EFCTID_ROUND5		(0x80000000 | 0x00000005)
#define EFCTID_ROUND6		(0x80000000 | 0x00000006)
#define EFCTID_FIGHT		(0x80000000 | 0x00000007)
#define EFCTID_KO			(0x80000000 | 0x00000008)
#define EFCTID_CYOHILIGHT	(0x80000000 | 0x00000009)//���K�̌�
#define EFCTID_TIMEOVER		(0x80000000 | 0x0000000A)
#define EFCTID_DOUBLEKO		(0x80000000 | 0x0000000B)
#define EFCTID_HATTEN		(0x80000000 | 0x0000000C)//!<�B���G�t�F�N�g  (x,y)




/*--------------------------------------------------------------------------------
	��O���� ON/OFF
	���������ڂ������ׂ����Ƃ���OFF�ɂ���VC�̃f�o�b�K���Ƃ܂�̂�҂�
----------------------------------------------------------------------------------*/
#ifndef _DEBUG	//catch

#define _CATCH_WHILE_SCENE					//!< �V�[��Timer()���ɗ�O���L���b�`
#define _CATCH_HANDLEMESSAGE_EXCEPTIONS		//!< ���b�Z�[�W�������̗�O���L���b�`
#define _CATCH_WHILE_EXEC					//!< �^�X�Nexecute�������̗�O���L���b�`
#define _CATCH_WHILE_RENDER					//!< �^�X�NRender�������̗�O���L���b�`

#endif			//catch


/*--------------------------------------------------------------------------------
	�f�o�b�O���� ON/OFF
	���݂Ȃ��̂���������
----------------------------------------------------------------------------------*/
#define _THREAD_POOL						//!< �X���b�h�v�[�����g�p����

#ifdef _DEBUG//�f�o�b�O�p��`��

#define _DBG_MESSAGES_TO_CONSOLE			//!< Log��OutputDebugString����
#define _DBG_REPORT_NETMESSAGES
#define _DBG_SHOW_LOADCHARACTER_NET
#define _DBG_NET_INITIDLIST
#define _DBG_NET_RSELECT
#define _DBG_NET_SHOW_STG
#define _DBG_SHOW_LOADSTART_INFORMATION
//#define _DBG_SHOW_SYNCMESSAGE
//#define _DBG_NET_EFFECTLIST
//#define _DBG_COMCOMMAND
//#define _DBG_NET_CK_KEYINPUTNUMBER
//#define _DBG_CK_DLLINDEX
//#define _DBG_REPORT_OBJECT_CREATION
#define _DBG_LOG2FILE_SYNC					//!< �f�[�^SYNC���쎞�̃��O���Ƃ�

#endif // �f�o�b�O�p��`��


/*--------------------------------------------------------------------------------
	���������[�N�`�F�b�N
----------------------------------------------------------------------------------*/
#ifdef _DEBUG
#ifndef CRTDBG_MAP_ALLOC
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define MEMLEAK_CK_DEF
#endif
#endif


//�z��O�A�N�Z�X�`�F�b�N
#ifdef _DEBUG
#define ARRAYBOUNDARY_DEBUG
#endif




#endif//_DEFINE_CONST_H_
