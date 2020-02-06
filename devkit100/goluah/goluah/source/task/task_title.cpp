﻿/*===========================================================

	タイトル画面

=============================================================*/
#include "define_const.h"
#include "define_macro.h"
#include "global.h"
#include "task_title.h"
#include "task_opening.h"

/*-----------------------------------------------------------
	生成・破棄
-------------------------------------------------------------*/
CTitle::CTitle()
{
	dds_back=NULL;
	is_error = FALSE;
	selectedgamemode=0;
	demomode = TRUE;
}

CTitle::~CTitle()
{
	RELSURFACE(dds_back);
}


/*==========================================================

	タスク関係

============================================================*/
void CTitle::Activate(DWORD taskId)
{
	g_sound.BGMStop();

	//変数の初期化
	counter=0;
	selected_item=0;
	setting_now=FALSE;

	//読み込みとかなんかそういうもの
	dds_back = g_draw.CreateSurfaceFrom256Image(_T(".\\system\\logo"));

	//BGM再生
	g_sound.BGMPlay(_T(".\\system\\bgm\\title"));
}

BOOL CTitle::Inactivate(DWORD taskId)
{
	g_sound.BGMStop();
	RELSURFACE(dds_back);
	return TRUE;
}

BOOL CTitle::Execute(DWORD time)
{
	counter++;
	if(counter < 20)return TRUE;//イキナリ決定されてしまうのを防止

	if(setting_now){
		Settings();
	}
	else{
		// デモ
		if(counter == TIME_DEMOSTART){
			BOOL demobattle = FALSE;

			if(!demomode)
			{
				CTOpening* pTask = new CTOpening;

				// ムービー表示
				demobattle = FALSE;
				g_system.AddTask(pTask);
			}

			return TRUE;
		}

		MainMenu();
	}

	return TRUE;
}




/*-----------------------------------------------------------
	描画処理
-------------------------------------------------------------*/

#define CELLDRAW1(x,y,id) g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),id,x,y,-0.02f,0,FALSE,FALSE,0xFF8888FF);
#define CELLDRAW2(x,y,id) g_draw.CellDraw(g_system.GetSystemGraphicSurface(),g_system.GetSystemGraphicCell(),g_system.GetSystemGraphicRect(),id,x,y,-0.02f,0,FALSE,FALSE,0xFFFFFFFF);

void CTitle::Draw()
{
	g_draw.ResetParentMatrix();
	g_draw.ClearBackBuffer();
	g_draw.SetTransform(FALSE);
	g_draw.ResetTransformMatrix();

	RECT r;
	r.top=0;
	r.left=0;
	r.right=640;r.bottom=480;

	DWORD top_y = 300,diff_x=320+120;
	DWORD step_y = 30;

	TCHAR msg[128];

	DWORD text_color;
	DWORD text1_color = 0xFF4455AA;//選択されているときの色
	DWORD text2_color = 0xFF99AADD;//選択されていないときの色

	if(!setting_now){
		g_draw.CheckBlt(dds_back,0,0,r,FALSE,FALSE,0,0,/*g_system.IsErrorTitle() ? 0xFFFF8888 : */0xFFFFFFFF);//logo
		
		if (g_system.IsErrorTitle())
		{
			r.top=20;
			g_draw.DrawRedText(r, _T("エラー発生ゲーム中止、ｽﾏｿ。"), -1, DT_CENTER, 2);
		}

		//storymode
		if(selectedgamemode==TITLE_STORY)text_color = text1_color;
		else text_color = text2_color;
		g_system.DrawBMPText(320,top_y,0,_T("STORY MODE"),text_color);

		top_y += step_y;

		//player1 vs computer
		if(selectedgamemode==TITLE_VS)text_color = text1_color;
		else text_color = text2_color;
		g_system.DrawBMPText(320,top_y,0,_T("VS MODE"),text_color);

		top_y += step_y;

		//practice
		if(selectedgamemode==TITLE_PRACTICE)text_color = text1_color;
		else text_color = text2_color;
		g_system.DrawBMPText(320,top_y,0,_T("PRACTICE"),text_color);

		top_y += step_y;

		//settings
		if(selectedgamemode==TITLE_SETTINGS)text_color = text1_color;
		else text_color = text2_color;
		g_system.DrawBMPText(320,top_y,0,_T("SETTINGS"),text_color);

		top_y += step_y;

		//op movie
		if (selectedgamemode==TITLE_OPMOVIE)text_color = text1_color;
		else text_color = text2_color;
		g_system.DrawBMPText(320,top_y,0,_T("OPENING MOVIE"),text_color);

		top_y += step_y;

		//network
		/*if (selectedgamemode==TITLE_NETWORK)text_color = text1_color;
		else text_color = text2_color;
		g_system.DrawBMPText(320,top_y,0,_T("NETWORK MODE"),text_color);*/
	}
	else{
		g_draw.CheckBlt(dds_back,0,0,r,FALSE,FALSE,0,0,0xFF333333);//logo

		r.top=10;
		g_draw.DrawBlueText(r,_T("SETTINGS"),-1,DT_CENTER,2);
		
		r.top+=30;

		_tcscpy(msg,_T("難易度："));
		switch(g_config.GetDifficulty()){
		case DIFF_VERYEASY	:_tcscpy(&msg[strlen(msg)],_T("牛鮭定食"));break;
		case DIFF_EASY		:_tcscpy(&msg[strlen(msg)],_T("150円引き"));break;
		case DIFF_NORMAL	:_tcscpy(&msg[strlen(msg)],_T("標準"));break;
		case DIFF_HARD		:_tcscpy(&msg[strlen(msg)],_T("パパ特盛り"));break;
		case DIFF_VERYHARD	:_tcscpy(&msg[strlen(msg)],_T("素人にはお勧めできない"));break;
		case DIFF_SUPERHARD	:_tcscpy(&msg[strlen(msg)],_T("殺伐"));break;
		case DIFF_ULTRAHARD	:_tcscpy(&msg[strlen(msg)],_T("刺すか刺されるか"));break;
		case DIFF_LIMITERCUT:_tcscpy(&msg[strlen(msg)],_T("ぶち切れ"));break;
		default:_tcscpy(&msg[strlen(msg)],_T("未知の難易度？"));
		}
		if(selected_item==SETTINGS_DIFFICULTY)
			g_draw.DrawRedText(r,msg,-1,DT_CENTER,2);
		else
			g_draw.DrawBlueText(r,msg,-1,DT_CENTER,2);

		r.top+=20;
		_tcscpy(msg,_T("先取ポイント数："));
		_stprintf(&msg[strlen(msg)],_T("%d"),g_config.GetMaxPoint());
		if(selected_item==SETTINGS_1V1_NUMPOINT)
			g_draw.DrawRedText(r,msg,-1,DT_CENTER,2);
		else
			g_draw.DrawBlueText(r,msg,-1,DT_CENTER,2);

		r.top+=20;
		_tcscpy(msg,_T("支援攻撃数："));
		_stprintf(&msg[strlen(msg)],_T("%d"),g_config.GetStrikerCount());
		if(selected_item==SETTINGS_NUMSTRIKER)
			g_draw.DrawRedText(r,msg,-1,DT_CENTER,2);
		else
			g_draw.DrawBlueText(r,msg,-1,DT_CENTER,2);

		r.top+=20;
		_tcscpy(msg,_T("ゲーム速度："));
		switch(g_config.GetGameSpeed()){
		case GSPEED_SLOW	:_tcscpy(&msg[strlen(msg)],_T("遅い(40fps)"));break;
		case GSPEED_OLD		:_tcscpy(&msg[strlen(msg)],_T("旧標準(50fps)"));break;
		case GSPEED_NEW		:_tcscpy(&msg[strlen(msg)],_T("標準(60fps)"));break;
		case GSPEED_30x2	:_tcscpy(&msg[strlen(msg)],_T("コマ飛ばし(30fps、2倍速)"));break;
//		case GSPEED_25x2	:_tcscpy(&msg[strlen(msg)],_T("コマ飛ばし－標準(25fps、2倍)"));break;
		default:_tcscpy(&msg[strlen(msg)],_T("ﾜｶﾝﾈ（とりあえず50fpsで動作します）"));
		}
		if(selected_item==SETTINGS_GAMESPEED)
			g_draw.DrawRedText(r,msg,-1,DT_CENTER,2);
		else
			g_draw.DrawBlueText(r,msg,-1,DT_CENTER,2);

		r.top += 20;
		_tcscpy(msg, _T("カメラ上下："));
		switch (g_config.GetCameraMode()){
		case CAMERA_OLD:_tcscpy(&msg[strlen(msg)], _T("しない")); break;
		case CAMERA_UPPER:_tcscpy(&msg[strlen(msg)], _T("上優先")); break;
		case CAMERA_NEW:_tcscpy(&msg[strlen(msg)], _T("下優先")); break;
		default:_tcscpy(&msg[strlen(msg)], _T("ﾜｶﾝﾈ"));
		}
		if (selected_item == SETTINGS_CAMERAMODE)
			g_draw.DrawRedText(r, msg, -1, DT_CENTER, 2);
		else
			g_draw.DrawBlueText(r, msg, -1, DT_CENTER, 2);

		r.top += 20;
		_tcscpy(msg, _T("HPゲージ："));
		switch (g_config.GetGaugeMode()){
		case GAUGE_1000:_tcscpy(&msg[strlen(msg)], _T("細め(～1.21)")); break;
		case GAUGE_1220:_tcscpy(&msg[strlen(msg)], _T("太め(1.22～)")); break;
		default:_tcscpy(&msg[strlen(msg)], _T("ﾜｶﾝﾈ"));
		}
		if (selected_item == SETTINGS_GAUGEMODE)
			g_draw.DrawRedText(r, msg, -1, DT_CENTER, 2);
		else
			g_draw.DrawBlueText(r, msg, -1, DT_CENTER, 2);

		r.top += 30;

		r.top+=40;
		if(selected_item==SETTINGS_EXIT)
			g_draw.DrawRedText(r,_T("exit"),-1,DT_CENTER,2);
		else
			g_draw.DrawBlueText(r,_T("exit"),-1,DT_CENTER,2);
	}
}


/*-----------------------------------------------------------
	メインメニュー
-------------------------------------------------------------*/
void CTitle::MainMenu()
{
	DWORD keystate = g_input.GetAllKey();
	UINT  dec_index;//決定キーを押したプレイヤー
	for(int k=0;k<MAXNUM_KEYI;k++){
		if(g_input.GetKey(k,0) & KEYSTA_ANYKEY){
			dec_index = k;
			break;
		}
	}

	//ケテーイ
	if(keystate & KEYSTA_ANYKEY){
		counter=0;
		if(selectedgamemode==TITLE_STORY){//story mode
			g_system.PlaySystemSound(SYSTEMSOUND_HIT3);
			return;
		}
		else if(selectedgamemode==TITLE_VS)//vs mode
		{
			g_system.PlaySystemSound(SYSTEMSOUND_HIT3);
			return;
		}
		else if(selectedgamemode==TITLE_PRACTICE)//practice
		{
			g_system.PlaySystemSound(SYSTEMSOUND_HIT3);
			return;
		}
		else if(selectedgamemode==TITLE_SETTINGS){//settings
			setting_now=TRUE;
			return;
		}
		else if(selectedgamemode==TITLE_NETWORK){//network
			/*if (!g_play.IsConnected())
				g_play.InitConnection();*/
			return;
		}
		else{//op
			g_system.AddTask(new CTOpening);
			return;
		}
	}

	//↓移動
	if(keystate & KEYSTA_DOWN){
		selectedgamemode++;
		if(selectedgamemode>TITLE_OPMOVIE)selectedgamemode=0;
		g_system.PlaySystemSound(SYSTEMSOUND_HIT1);
		counter = 0;
	}
	//↑移動
	if(keystate & KEYSTA_UP){
		if(selectedgamemode==0)selectedgamemode=TITLE_OPMOVIE;
		else selectedgamemode--;
		g_system.PlaySystemSound(SYSTEMSOUND_HIT1);
		counter = 0;
	}
}



/*-----------------------------------------------------------
	「SETTINGS」
-------------------------------------------------------------*/
void CTitle::Settings()
{
	BOOL exitsetting=FALSE;
	DWORD difficulty=g_config.GetDifficulty();
	int timelimit_per_round = g_config.GetTimeLimit();
	DWORD point_max=g_config.GetMaxPoint();
	int num_striker = g_config.GetStrikerCount();
	DWORD game_speed = g_config.GetGameSpeed();
	DWORD camera_mode = g_config.GetCameraMode();
	DWORD gauge_mode = g_config.GetGaugeMode();

	if(g_input.GetKey(0,0) & KEYSTA_DOWN2 || g_input.GetKey(1,0) & KEYSTA_DOWN2){//移動
		selected_item++;
		if(selected_item>SETTINGS_EXIT)selected_item=0;
		g_system.PlaySystemSound(SYSTEMSOUND_HIT1);
	}
	if(g_input.GetKey(0,0) & KEYSTA_UP2 || g_input.GetKey(1,0) & KEYSTA_UP2){//移動
		if(selected_item==0)selected_item=SETTINGS_EXIT;
		else selected_item--;
		g_system.PlaySystemSound(SYSTEMSOUND_HIT1);
	}

	switch(selected_item){
	case SETTINGS_DIFFICULTY://難易度変更
		if(g_input.GetKey(0,0) & KEYSTA_ALEFT2 || g_input.GetKey(1,0) & KEYSTA_ALEFT2){//移動
			difficulty--;
			//if(difficulty>DIFF_END)difficulty=0;
		}
		if(g_input.GetKey(0,0) & KEYSTA_ARIGHT2 || g_input.GetKey(1,0) & KEYSTA_ARIGHT2){//移動
			difficulty++;
			//if(difficulty==DIFF_END)difficulty--;
		}
		g_config.SetDifficulty((Config2_Difficulty)(difficulty%8));
		break;
	case SETTINGS_1V1_NUMPOINT://必要ポイント先取数
		if(g_input.GetKey(0,0) & KEYSTA_ARIGHT2 || g_input.GetKey(1,0) & KEYSTA_ARIGHT2){//移動
			point_max++;
			if(point_max==4)point_max=1;
		}
		if(g_input.GetKey(0,0) & KEYSTA_ALEFT2 || g_input.GetKey(1,0) & KEYSTA_ALEFT2){//移動
			point_max--;
			if(point_max==0)point_max=3;
		}
		g_config.SetMaxPoint(point_max);
		break;
	case SETTINGS_NUMSTRIKER://ストライカー数
		if(g_input.GetKey(0,0) & KEYSTA_ARIGHT2 || g_input.GetKey(1,0) & KEYSTA_ARIGHT2){//移動
			num_striker++;
			if(num_striker==9)num_striker=0;
		}
		if(g_input.GetKey(0,0) & KEYSTA_ALEFT2 || g_input.GetKey(1,0) & KEYSTA_ALEFT2){//移動
			num_striker--;
			if(num_striker==0)num_striker=8;
		}
		g_config.SetStrikerCount(num_striker);
		break;
	case SETTINGS_GAMESPEED://ゲーム速度
		if(g_input.GetKey(0,0) & KEYSTA_ARIGHT2 || g_input.GetKey(1,0) & KEYSTA_ARIGHT2){//移動
			game_speed++;
			if(game_speed==4)game_speed=0;
		}
		if(g_input.GetKey(0,0) & KEYSTA_ALEFT2 || g_input.GetKey(1,0) & KEYSTA_ALEFT2){//移動
			game_speed--;
			if(game_speed>3)game_speed=3;
		}
		g_config.SetGameSpeed((Config2_GameSpeed)game_speed);
		g_system.UpdateFrameRate();
		break;
	case SETTINGS_CAMERAMODE://カメラモード
		if (g_input.GetKey(0, 0) & KEYSTA_ARIGHT2 || g_input.GetKey(1, 0) & KEYSTA_ARIGHT2){//移動
			camera_mode++;
			if (camera_mode == 3)camera_mode = 0;
		}
		if (g_input.GetKey(0, 0) & KEYSTA_ALEFT2 || g_input.GetKey(1, 0) & KEYSTA_ALEFT2){//移動
			camera_mode--;
			if (camera_mode > 2)camera_mode = 2;
		}
		g_config.SetCameraMode((Config2_CameraMode)camera_mode);
		break;
	case SETTINGS_GAUGEMODE://HPゲージモード
		if (g_input.GetKey(0, 0) & KEYSTA_ARIGHT2 || g_input.GetKey(1, 0) & KEYSTA_ARIGHT2){//移動
			gauge_mode++;
			if (gauge_mode == 2)gauge_mode = 0;
		}
		if (g_input.GetKey(0, 0) & KEYSTA_ALEFT2 || g_input.GetKey(1, 0) & KEYSTA_ALEFT2){//移動
			gauge_mode--;
			if (gauge_mode > 1)gauge_mode = 1;
		}
		g_config.SetGaugeMode((Config2_GaugeMode)gauge_mode);
		break;
	case SETTINGS_EXIT:
		if((g_input.GetKey(0,0)|g_input.GetKey(1,0)) & (KEYSTA_RIGHT2|KEYSTA_LEFT2|KEYSTA_ANYKEY )){
			setting_now=FALSE;
		}
		break;
	}
}
