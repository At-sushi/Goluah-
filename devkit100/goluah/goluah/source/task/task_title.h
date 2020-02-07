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
enum {
    TITLE_STORY = 0,
    TITLE_VS,
    TITLE_PRACTICE,
    TITLE_SETTINGS,
    TITLE_END,

    //未対応
    TITLE_NETWORK
};

//settingの項目
enum {
    SETTINGS_DIFFICULTY = 0,
    SETTINGS_1V1_NUMPOINT,
    SETTINGS_GAMESPEED,
    SETTINGS_EXIT//!< これを項目の最後とみなしている
};

