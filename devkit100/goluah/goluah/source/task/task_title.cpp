/*===========================================================

	�^�C�g�����

=============================================================*/
#include "define_const.h"
#include "define_macro.h"
#include "global.h"
#include "task_title.h"
#include "task_vsmode.h"
#include "task_select_story.h"
#include "task_opening.h"
#include "task_select_practice.h"
#include "task_demobattle.h"		// ����

/*-----------------------------------------------------------
	�����E�j��
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

	�^�X�N�֌W

============================================================*/
void CTitle::Activate(DWORD taskId)
{
	g_sound.BGMStop();

	//�ϐ��̏�����
	counter=0;
	selected_item=0;
	setting_now=FALSE;

	//�ǂݍ��݂Ƃ��Ȃ񂩂�����������
	dds_back = g_draw.CreateSurfaceFrom256Image(".\\system\\logo");

	//BGM�Đ�
	g_sound.BGMPlay(".\\system\\bgm\\title");
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
	if(counter < 10)return TRUE;//�C�L�i�����肳��Ă��܂��̂�h�~

	if(setting_now){
		Settings();
	}
	else{
		// �f��
		if(counter == TIME_DEMOSTART){
			BOOL demobattle = TRUE;

			if(!demomode)
			{
				CTOpening* pTask = new CTOpening;

				// ���[�r�[�\��
				demobattle = FALSE;
				g_system.AddTask(pTask);
				if( !pTask->IsOK() )
					demobattle = TRUE; // �_���|
			}

			if(demobattle){
				// �ΐ�f��
				g_system.AddTask(new CBattleTaskDemo);
			}
			demomode = !demomode; // �t���O���]

			return TRUE;
		}

		MainMenu();
	}

	return TRUE;
}




/*-----------------------------------------------------------
	�`�揈��
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

	char msg[128];

	DWORD text_color;
	DWORD text1_color = 0xFF4455AA;//�I������Ă���Ƃ��̐F
	DWORD text2_color = 0xFF99AADD;//�I������Ă��Ȃ��Ƃ��̐F

	if(!setting_now){
		g_draw.CheckBlt(dds_back,0,0,r,FALSE,FALSE,0,0,/*g_system.IsErrorTitle() ? 0xFFFF8888 : */0xFFFFFFFF);//logo
		
		if (g_system.IsErrorTitle())
		{
			r.top=20;
			g_draw.DrawRedText(r, "�G���[�����Q�[�����~�A�Ͽ�B", -1, DT_CENTER, 2);
		}

		//storymode
		if(selectedgamemode==TITLE_STORY)text_color = text1_color;
		else text_color = text2_color;
		g_system.DrawBMPText(320,top_y,0,"STORY MODE",text_color);

		top_y += step_y;

		//player1 vs computer
		if(selectedgamemode==TITLE_VS)text_color = text1_color;
		else text_color = text2_color;
		g_system.DrawBMPText(320,top_y,0,"VS MODE",text_color);

		top_y += step_y;

		//practice
		if(selectedgamemode==TITLE_PRACTICE)text_color = text1_color;
		else text_color = text2_color;
		g_system.DrawBMPText(320,top_y,0,"PRACTICE",text_color);

		top_y += step_y;

		//settings
		if(selectedgamemode==TITLE_SETTINGS)text_color = text1_color;
		else text_color = text2_color;
		g_system.DrawBMPText(320,top_y,0,"SETTINGS",text_color);

		top_y += step_y;

		//op movie
		if (selectedgamemode==TITLE_OPMOVIE)text_color = text1_color;
		else text_color = text2_color;
		g_system.DrawBMPText(320,top_y,0,"OPENING MOVIE",text_color);

		top_y += step_y;

		//network
		/*if (selectedgamemode==TITLE_NETWORK)text_color = text1_color;
		else text_color = text2_color;
		g_system.DrawBMPText(320,top_y,0,"NETWORK MODE",text_color);*/
	}
	else{
		g_draw.CheckBlt(dds_back,0,0,r,FALSE,FALSE,0,0,0xFF333333);//logo

		r.top=10;
		g_draw.DrawBlueText(r,"SETTINGS",-1,DT_CENTER,1);
		
		r.top+=30;

		strcpy(msg,"��Փx�F");
		switch(g_config.GetDifficulty()){
		case DIFF_VERYEASY	:strcpy(&msg[strlen(msg)],"������H");break;
		case DIFF_EASY		:strcpy(&msg[strlen(msg)],"150�~����");break;
		case DIFF_NORMAL	:strcpy(&msg[strlen(msg)],"�W��");break;
		case DIFF_HARD		:strcpy(&msg[strlen(msg)],"�p�p������");break;
		case DIFF_VERYHARD	:strcpy(&msg[strlen(msg)],"�f�l�ɂ͂����߂ł��Ȃ�");break;
		case DIFF_SUPERHARD	:strcpy(&msg[strlen(msg)],"�E��");break;
		case DIFF_ULTRAHARD	:strcpy(&msg[strlen(msg)],"�h�����h����邩");break;
		case DIFF_LIMITERCUT:strcpy(&msg[strlen(msg)],"�Ԃ��؂�");break;
		default:strcpy(&msg[strlen(msg)],"���m�̓�Փx�H");
		}
		if(selected_item==SETTINGS_DIFFICULTY)
			g_draw.DrawRedText(r,msg,-1,DT_CENTER,1);
		else
			g_draw.DrawBlueText(r,msg,-1,DT_CENTER,1);

		r.top+=20;
		strcpy(msg,"���|�C���g���F");
		sprintf(&msg[strlen(msg)],"%d",g_config.GetMaxPoint());
		if(selected_item==SETTINGS_1V1_NUMPOINT)
			g_draw.DrawRedText(r,msg,-1,DT_CENTER,1);
		else
			g_draw.DrawBlueText(r,msg,-1,DT_CENTER,1);

		r.top+=20;
		strcpy(msg,"�x���U�����F");
		sprintf(&msg[strlen(msg)],"%d",g_config.GetStrikerCount());
		if(selected_item==SETTINGS_NUMSTRIKER)
			g_draw.DrawRedText(r,msg,-1,DT_CENTER,1);
		else
			g_draw.DrawBlueText(r,msg,-1,DT_CENTER,1);

		r.top+=20;
		strcpy(msg,"�Q�[�����x�F");
		switch(g_config.GetGameSpeed()){
		case GSPEED_SLOW	:strcpy(&msg[strlen(msg)],"�x��(40fps)");break;
		case GSPEED_OLD		:strcpy(&msg[strlen(msg)],"�W��(50fps)");break;
		case GSPEED_NEW		:strcpy(&msg[strlen(msg)],"������Ƒ���(60fps)");break;
		case GSPEED_30x2	:strcpy(&msg[strlen(msg)],"�R�}��΂�(30fps�A2�{)");break;
//		case GSPEED_25x2	:strcpy(&msg[strlen(msg)],"�R�}��΂��|�W��(25fps�A2�{)");break;
		default:strcpy(&msg[strlen(msg)],"ܶ�ȁi�Ƃ肠����50fps�œ��삵�܂��j");
		}
		if(selected_item==SETTINGS_GAMESPEED)
			g_draw.DrawRedText(r,msg,-1,DT_CENTER,1);
		else
			g_draw.DrawBlueText(r,msg,-1,DT_CENTER,1);

		r.top+=30;

		r.top+=40;
		if(selected_item==SETTINGS_EXIT)
			g_draw.DrawRedText(r,"exit",-1,DT_CENTER,1);
		else
			g_draw.DrawBlueText(r,"exit",-1,DT_CENTER,1);
	}
}


/*-----------------------------------------------------------
	���C�����j���[
-------------------------------------------------------------*/
void CTitle::MainMenu()
{
	CTaskVsMode *vsTask;
	CTStorySelect *sselect;

	DWORD keystate = g_input.GetAllKey();
	UINT  dec_index;//����L�[���������v���C���[
	for(int k=0;k<MAXNUM_KEYI;k++){
		if(g_input.GetKey(k,0) & KEYSTA_ANYKEY){
			dec_index = k;
			break;
		}
	}

	//�P�e�[�C
	if(keystate & KEYSTA_ANYKEY){
		counter=0;
		if(selectedgamemode==TITLE_STORY){//story mode
			if(g_storylist.GetAllStoryNum()<=0){
				g_system.PlaySystemSound(SYSTEMSOUND_GUARD);
				return;
			}
			g_system.PlaySystemSound(SYSTEMSOUND_HIT3);
			sselect = new CTStorySelect;
			sselect->SetKeyIndex(dec_index);
			g_system.AddTask(sselect);
			return;
		}
		else if(selectedgamemode==TITLE_VS)//vs mode
		{
			g_system.PlaySystemSound(SYSTEMSOUND_HIT3);
			vsTask = new CTaskVsMode;
			g_system.AddTask( vsTask );
			return;
		}
		else if(selectedgamemode==TITLE_PRACTICE)//practice
		{
			g_system.PlaySystemSound(SYSTEMSOUND_HIT3);
			CCharacterSelectPractice* pmode = new CCharacterSelectPractice;
			g_system.AddTask( pmode );
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

	//���ړ�
	if(keystate & KEYSTA_DOWN){
		selectedgamemode++;
		if(selectedgamemode>TITLE_OPMOVIE)selectedgamemode=0;
		g_system.PlaySystemSound(SYSTEMSOUND_HIT1);
		counter = 0;
	}
	//���ړ�
	if(keystate & KEYSTA_UP){
		if(selectedgamemode==0)selectedgamemode=TITLE_OPMOVIE;
		else selectedgamemode--;
		g_system.PlaySystemSound(SYSTEMSOUND_HIT1);
		counter = 0;
	}
}



/*-----------------------------------------------------------
	�uSETTINGS�v
-------------------------------------------------------------*/
void CTitle::Settings()
{
	BOOL exitsetting=FALSE;
	DWORD difficulty=g_config.GetDifficulty();
	int timelimit_per_round = g_config.GetTimeLimit();
	DWORD point_max=g_config.GetMaxPoint();
	int num_striker = g_config.GetStrikerCount();
	DWORD game_speed = g_config.GetGameSpeed();

	if(g_input.GetKey(0,0) & KEYSTA_DOWN2 || g_input.GetKey(1,0) & KEYSTA_DOWN2){//�ړ�
		selected_item++;
		if(selected_item>SETTINGS_EXIT)selected_item=0;
		g_system.PlaySystemSound(SYSTEMSOUND_HIT1);
	}
	if(g_input.GetKey(0,0) & KEYSTA_UP2 || g_input.GetKey(1,0) & KEYSTA_UP2){//�ړ�
		if(selected_item==0)selected_item=SETTINGS_EXIT;
		else selected_item--;
		g_system.PlaySystemSound(SYSTEMSOUND_HIT1);
	}

	switch(selected_item){
	case SETTINGS_DIFFICULTY://��Փx�ύX
		if(g_input.GetKey(0,0) & KEYSTA_ALEFT2 || g_input.GetKey(1,0) & KEYSTA_ALEFT2){//�ړ�
			difficulty--;
			//if(difficulty>DIFF_END)difficulty=0;
		}
		if(g_input.GetKey(0,0) & KEYSTA_ARIGHT2 || g_input.GetKey(1,0) & KEYSTA_ARIGHT2){//�ړ�
			difficulty++;
			//if(difficulty==DIFF_END)difficulty--;
		}
		g_config.SetDifficulty((Config2_Difficulty)(difficulty%8));
		break;
	case SETTINGS_1V1_NUMPOINT://�K�v�|�C���g��搔
		if(g_input.GetKey(0,0) & KEYSTA_ARIGHT2 || g_input.GetKey(1,0) & KEYSTA_ARIGHT2){//�ړ�
			point_max++;
			if(point_max==4)point_max=1;
		}
		if(g_input.GetKey(0,0) & KEYSTA_ALEFT2 || g_input.GetKey(1,0) & KEYSTA_ALEFT2){//�ړ�
			point_max--;
			if(point_max==0)point_max=3;
		}
		g_config.SetMaxPoint(point_max);
		break;
	case SETTINGS_NUMSTRIKER://�X�g���C�J�[��
		if(g_input.GetKey(0,0) & KEYSTA_ARIGHT2 || g_input.GetKey(1,0) & KEYSTA_ARIGHT2){//�ړ�
			num_striker++;
			if(num_striker==9)num_striker=0;
		}
		if(g_input.GetKey(0,0) & KEYSTA_ALEFT2 || g_input.GetKey(1,0) & KEYSTA_ALEFT2){//�ړ�
			num_striker--;
			if(num_striker==0)num_striker=8;
		}
		g_config.SetStrikerCount(num_striker);
		break;
	case SETTINGS_GAMESPEED://�Q�[�����x
		if(g_input.GetKey(0,0) & KEYSTA_ARIGHT2 || g_input.GetKey(1,0) & KEYSTA_ARIGHT2){//�ړ�
			game_speed++;
			if(game_speed==4)game_speed=0;
		}
		if(g_input.GetKey(0,0) & KEYSTA_ALEFT2 || g_input.GetKey(1,0) & KEYSTA_ALEFT2){//�ړ�
			game_speed--;
			if(game_speed>3)game_speed=3;
		}
		g_config.SetGameSpeed((Config2_GameSpeed)game_speed);
		g_system.UpdateFrameRate();
		break;
	case SETTINGS_EXIT:
		if((g_input.GetKey(0,0)|g_input.GetKey(1,0)) & (KEYSTA_RIGHT2|KEYSTA_LEFT2|KEYSTA_ANYKEY )){
			setting_now=FALSE;
		}
		break;
	}
}
