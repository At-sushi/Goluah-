/*===========================================================

	タイトル画面

=============================================================*/
#pragma once
#include "task.h"


/*!
*	@brief タイトル画面
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

//変数
public:
	BOOL  is_error;			//!< エラーが起こってタイトルに戻った
private:
	DWORD counter;
	MYSURFACE *dds_back;
	DWORD selectedgamemode;
	DWORD selected_item;	//!< settings で選択されている項目
	BOOL  setting_now;		//!< 設定モードである
	BOOL  demomode;			//!< デモ切り替えスイッチ（ムービー／ゲーム）

	enum {
		TIME_DEMOSTART = 1200//!< デモが始まるまでの時間
	};
};


//selectedgamemodeの項目
#define TITLE_STORY				0
#define TITLE_VS				1
#define TITLE_PRACTICE			2
#define TITLE_SETTINGS			3
#define TITLE_OPMOVIE			4
#define TITLE_NETWORK			5//!< これを項目の最後とみなしている

//settingの項目
#define SETTINGS_DIFFICULTY		0
//#define SETTINGS_TLPERROUND	1
#define SETTINGS_1V1_NUMPOINT	1
#define SETTINGS_NUMSTRIKER		2
//#define SETTINGS_2V2_TYPE		4
//#define SETTINGS_3V3_TYPE		5
#define SETTINGS_GAMESPEED		3
#define SETTINGS_EXIT			4//!< これを項目の最後とみなしている

