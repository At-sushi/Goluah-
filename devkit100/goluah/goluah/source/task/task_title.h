/*===========================================================

	�^�C�g�����

=============================================================*/
#pragma once
#include "task.h"


/*!
*	@brief �^�C�g�����
*	@ingroup Tasks
*/
class CTitle : public CExclusiveTaskBase
{
public:
	CTitle();
	~CTitle();

	void Initialize(){Activate(0);}
	void Terminate(){Inactivate(0);}
	BOOL Execute(DWORD time);
	void Activate(DWORD ptid);
	BOOL Inactivate(DWORD ntid);
	void Draw();
	DWORD GetID(){return('titl');}

private:
	void MainMenu();
	void Settings();

//�ϐ�
public:
	BOOL  is_error;			//!< �G���[���N�����ă^�C�g���ɖ߂���
private:
	DWORD counter;
	MYSURFACE *dds_back;
	DWORD selectedgamemode;
	DWORD selected_item;	//!< settings �őI������Ă��鍀��
	BOOL  setting_now;		//!< �ݒ胂�[�h�ł���
	BOOL  demomode;			//!< �f���؂�ւ��X�C�b�`�i���[�r�[�^�Q�[���j

	enum {
		TIME_DEMOSTART = 1200//!< �f�����n�܂�܂ł̎���
	};
};


//selectedgamemode�̍���
#define TITLE_STORY				0
#define TITLE_VS				1
#define TITLE_PRACTICE			2
#define TITLE_SETTINGS			3
#define TITLE_OPMOVIE			4
#define TITLE_NETWORK			5//!< ��������ڂ̍Ō�Ƃ݂Ȃ��Ă���

//setting�̍���
#define SETTINGS_DIFFICULTY		0
//#define SETTINGS_TLPERROUND	1
#define SETTINGS_1V1_NUMPOINT	1
#define SETTINGS_NUMSTRIKER		2
//#define SETTINGS_2V2_TYPE		4
//#define SETTINGS_3V3_TYPE		5
#define SETTINGS_GAMESPEED		3
#define SETTINGS_CAMERAMODE		4
#define SETTINGS_GAUGEMODE		5
#define SETTINGS_EXIT			6//!< ��������ڂ̍Ō�Ƃ݂Ȃ��Ă���

