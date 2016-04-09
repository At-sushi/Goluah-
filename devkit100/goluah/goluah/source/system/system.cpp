
/*============================================================================

	�V�X�e���N���X

	���C���V�X�e���B�Q�[���S�ʂ̓���

==============================================================================*/


#include "stdafx.h"
#include "global.h"
#include "system.h"
#include "../resource.h"
#include "task_title.h"
#include "gcdhandler.h"
#include "debug_menu.h"
#include "aki3d.h"
#include "task_opening.h"
#include "png.h"
extern "C"
{
#include "jpeglib.h"
} // extern "C"

/*------------------------------------------------------------------------------
	����
--------------------------------------------------------------------------------*/
CSystem::CSystem()
{
	int i;

	//�T�E���h�̃A�����[�h
	for(i=0;i<NUM_SYSTEMSOUND;i++){
		RELEASE(dsb_efct[0][i]);
		RELEASE(dsb_efct[1][i]);
	}

	//�V�X�e���O���t�B�b�N�j��
	for(i=0;i<GCDMAX_IMAGES;i++)
	{
		g_draw.RelSurface(sdds[i]);
		sdds[i] = NULL;
	}

	//���̑����낢��
	hwnd = NULL;
	ready=FALSE;

	sw_rects=FALSE;
	sw_showfps=FALSE;
	sw_showbg=TRUE;
	sw_showstates=0;
	sw_slowmode=0;

	m_return_title_flag = FALSE;
	m_error_title = FALSE;
	m_first_update_flag = TRUE;

	m_loopTime = 20;

	logBuffer = NULL;
}

/*!
*	������
*	�N����_�C�A���O���\�������O�܂ł̂������̏������B
*	���̊֐��̍Ō�Ń_�C�A���O��\���B
*/
BOOL CSystem::Initialize(HWND hwnd)
{
	srand(timeGetTime());
	this->hwnd = hwnd;
	logBuffer = new char [512];

	// CDirectPlay����
	/*if (!g_play.Initialize(hwnd))
	{
		MessageBox(hwnd,_T("�ʐM�n�̏������Ɏ��s"),_T("�������G���[(2)"),MB_OK | MB_ICONERROR);
		Destroy();
		return FALSE;
	}*/

	//CDirectSound����
	if(g_config.UseDSound()){
		if(!g_sound.Initialize(hwnd)){
			MessageBox(hwnd,_T("���n�̏������Ɏ��s"),_T("�������G���[(1)"),MB_OK | MB_ICONERROR);
			Destroy();
			return FALSE;
		}
	}

	//�V�X�e���T�E���h�ǂݍ���
	InitSystemSound();

	//CDirectInput����
	if(!g_input.Initialize(hwnd)){
		MessageBox(hwnd,_T("���͌n�̏������Ɏ��s"),_T("�������G���[(2)"),MB_OK | MB_ICONERROR);
		Destroy();
		return FALSE;
	}
	if(g_config.UseDInput()){//�p�b�h���g�p����
		if(!g_input.InitializePad()){
			MessageBox(hwnd,_T("�Q�[���p�b�h�̏������Ɏ��s"),_T("�������G���[(2)"),MB_OK | MB_ICONERROR);
			Destroy();
			return FALSE;
		}
	}

	g_exp.Initialize();			//DLL�փG�N�X�|�[�g����֐��|�C���^������

	//���X�g������
	g_charlist.Initialize();	//�L�����N�^���X�g�\�z
	g_stagelist.Initialize();	//�X�e�[�W���X�g�\�z
	g_storylist.Initialize();	//�X�g�[���[���X�g�\�z

	//CDirectDraw����
	if(!g_draw.Initialize(hwnd,!g_config.IsFullScreen())){
		MessageBox(hwnd,_T("�`��n�̏������Ɏ��s"),_T("�������G���[(2)"),MB_OK);
		Destroy();
		return(FALSE);
	}

	//aki3d������
	aki3d.Initialize(g_draw.d3ddev,_T("system\\texture"));

	InitSystemGraphics();		//�V�X�e���O���t�B�b�N�ǂݍ���

	//�_�C�A���O�\��
	if(g_config.SwShowInfo() && !ShowInformation()){
		Destroy();
		return FALSE;
	}

	if(g_charlist.GetCharacterCount()==0){//�L�����N�^�[��������Ȃ�
		MessageBox(hwnd,_T("�L�����N�^�[����̂����܂���B�Q�[�����J�n�ł��܂���B"),_T("�������G���[(2)"),MB_OK);
		Destroy();
		return(FALSE);
	}
	if(g_stagelist.GetStageCount()==0){//�X�e�[�W������Ȃ�
		MessageBox(hwnd,_T("�X�e�[�W���������܂���B�Q�[�����J�n�ł��܂���B"),_T("�������G���[(2)"),MB_OK);
		Destroy();
		return(FALSE);
	}

	ready=TRUE;
	return TRUE;
}



/*!
*	�Q�[���J�n
*	�N����̏��\���_�C�A���O������ꂽ��A���C�����[�v���J�n�����O�ɌĂ΂��B
*	�^�C�g�����A�f�o�b�O���[�h�̏ꍇ�f�o�b�O���j���[�������^�X�N�Ƃ��ĊJ�n����B
*
*	�^�X�N�}�l�[�W�����i�Ȃ�?�j1�t����1�����r���^�X�N���󂯕t���Ȃ��悤�ɂ��Ă���̂ŁA
*	�I�v�V�����ł̋N�����ダ�[�r�[�Đ��̓��C�����[�v�̏��񎞂ɋN�����Ă���B
*/
BOOL CSystem::GameStart()
{
	m_debug_menu_enabled = FALSE;

	UpdateFrameRate();

#ifdef _DEBUG
	m_debug_menu_enabled = TRUE;
#else
	if(/*g_config.IsDebugMode() && */GetKeyState(VK_SHIFT) & 0x8000)m_debug_menu_enabled = TRUE;

	//�Q�[���J�n
	//�����^�X�N�̑}��
	if(m_debug_menu_enabled)AddTask( new CDebugMenu );
	else AddTask( new CTitle );
#endif
	return(TRUE);
}


/*------------------------------------------------------------------------------
	�j��
--------------------------------------------------------------------------------*/
void CSystem::Destroy()
{
	taskManager.Destroy();

	// �V�X�e���T�E���h���
	// �����������������Ȃ��̂łƂ肠���������ŏ����B
	for(int i=0;i<NUM_SYSTEMSOUND;i++){
		for(int j=0;j<2;j++){
			RELEASE(dsb_efct[j][i]);
		}
	}

	g_input.Destroy();
	g_sound.Destroy();
	g_draw.Destroy();
	g_play.Destroy();

	hwnd = NULL;
	ready = FALSE;
	DELETE_ARRAY(logBuffer);
}


/*!
*	���C�����[�v
*	�G���g���[�|�C���g��while���[�v����Ă΂�Ă���
*	FPS���߂��s���A�^�X�N�Ǘ��N���X�̒莞�������Ăяo���B
*/
void CSystem::MainLoop()
{
	//fps���߁B�E�F�C�g��������B
	static DWORD timeprv = timeGetTime();
	DWORD time_now = timeGetTime();
	if( timeprv+m_loopTime > time_now ){
		Sleep( timeprv+m_loopTime - time_now );//����������
		return;
	}
	time_now = timeGetTime();
	DWORD eat_time = time_now - timeprv;
	timeprv = time_now;

	//fps�Z�o
	static DWORD timepass=0;
	static DWORD passCnt=0;
	timepass += eat_time;
	passCnt++;
	if(timepass > 1000){
		m_fps = passCnt;
		timepass=0;
		passCnt=0;
	}

	//���O�����₷�����邽�߂ɁA�}��
	//if(g_config.CfgFullDebugLog()){
	//	g_log2file.AddLog(_T("-----------------------------main loop start"));
	//}

	//���s�^�O�̃N���A
	ClearSysTag();


	//�^�X�N�EExecute
	CExclusiveTaskBase *xtask = taskManager.GetTopExclusiveTask();
	taskManager.Execute(eat_time);

	static DWORD DrawCnt = 0; // �b��
	if (xtask == taskManager.GetTopExclusiveTask() && !m_render_disabled && !IsIconic(this->hwnd) &&
		(m_loopTime > 1 || timepass % (1000 / 60) <= 1) && // �m�[�E�F�C�g���y�ʉ�
		(g_config.GetGameSpeed() != GSPEED_30x2 || DrawCnt % 2 == 0)) // �R�}��΂�����
	{
		//�^�X�N�EDraw
		g_draw.StartDraw(TRUE);
		taskManager.Draw();
		g_draw.EndDraw();
	}
	else if(m_first_update_flag)
	{
		//�C���`�L�������t���O�������
		m_first_update_flag = FALSE;
		if(g_config.SwShowMovieFirst()){
			AddTask(new CTOpening);
		}
	}
	DrawCnt++;
	//else ���C���̃^�X�N���؂�ւ��������͕`����s��Ȃ�

	//�^�C�g����ʖ߂葀��
	if(m_return_title_flag){
		//�w��ID�̃^�X�N�������܂ŃG�N�X�N���[�V�u�^�X�N��pop
		if(m_debug_menu_enabled)
			taskManager.ReturnExclusiveTaskByID('debg');
		else
			taskManager.ReturnExclusiveTaskByID('titl');

		m_return_title_flag = FALSE;
	}

	//�Ȃ񂩂̊ԈႢ�őS���Ȃ��Ȃ���������Ƃ��̕��A�p
	if(taskManager.ExEmpty())
	{
		if(m_debug_menu_enabled)AddTask( new CDebugMenu );
		else AddTask( new CTitle );
	}

	// �b��e�X�g�[�u
	// DirectPlay�ɏ���������
	g_play.Do(m_loopTime / 3);
}



/*!
*	�E�C���h�E���b�Z�[�W����
*
*	���C���E�C���h�E�̃��b�Z�[�W�v���V�[�W������Ă΂��B
*	���C���E�C���h�E�Ɠ������b�Z�[�W���󂯎�邱�Ƃ��ł���
*
*	�����̓^�X�N�Ǘ��N���X�ɂ��܂킵�A������o�R���Ċe�^�X�N�N���X�܂Ŕz�M�����B
*
*	ESC�L�[�ł̃Q�[���I���͂����ōs���Ă���͗l
*/
void CSystem::WndMessage(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp)
{
	if(msg==WM_KEYDOWN){
		OnWMKeydown( wp, lp);//�L�[���͏���
	}

	taskManager.WndMessage(hWnd,msg,wp,lp);
}

/*!
*	�^�C�g����ʖ߂�
*	�֐����ł̓t���O��ۑ����邾���ŁA���ۂ̏����̓��C�����[�v�̒��ōs���Ă���B
*	��Ƀ^�X�N�Ǘ��N���X�̃^�X�N���X�g�̃��[�v�����炱�̊֐����Ă΂�Ă���̂ŁA
*	��������^�X�N�j�����s���Ɗ�Ȃ�����B�����B
*
*	�ʏ�̏����ł��̊֐����Ă΂ꂽ�ꍇ�A�G���[�t���O����������
*	@sa NotifyExcption
*/
void CSystem::ReturnTitle()
{
	m_return_title_flag = TRUE;
	m_error_title = FALSE;
}

/*!
*	��O����
*	�ݒ�ɂ��A���̂܂ܑ��s���邩�A�G���[�t���O�𗧂Ăă^�C�g���ɖ߂�B
*	�G���[�t���O�����ƃ^�C�g����ʂ��Ԃ��ۂ��Ȃ�B
*	@sa ReturnTitle
*/
void CSystem::NotifyExcption()
{
	g_system.DumpTags();

	if(g_config.IgnoreExceptions())return;
	/*MessageBox(hwnd, _T("�@ �ȁQ��\n�@�i�@�L�́M�j �� �ʂ�ۂ��������܂���"),
				_T("�E�z�b�I�����G���[�c�@�@���@�@���@�@�ȁ@�@���@�@��"), MB_OK | MB_ICONERROR);*/
	ReturnTitle();
	m_error_title = TRUE;
}

/*!
*	�V�X�e���I�L�[���͏���
*	��Ƀt�@���N�V�����L�[�̑���ɑΉ��B
*	�f�o�b�O���[�hON�ݒ�̏ꍇ�͂���ɑ��ɂ������̋@�\��S������B
*/
void CSystem::OnWMKeydown(WPARAM wp,LPARAM lp)
{
	switch(wp)
	{
	case VK_F6:SaveScreenShot();break;
	//case VK_F7:sw_pause = !sw_pause;break;//�퓬�^�X�N�ő���
	case VK_F8:ReturnTitle();break;
	}

	if(!g_config.IsDebugMode())return;

	//�ȉ��A�f�o�b�O���[�h�̂Ƃ��̂݌����X�C�b�`
	switch(wp){
		//�J�����ړ�
		case 'O':g_draw.camera_z+=0.2f;break;
		case 'P':g_draw.camera_z-=0.2f;break;
		case 'U':g_draw.camera_zurax+=0.2f;break;
		case 'I':g_draw.camera_zurax-=0.2f;break;
		case 'T':g_draw.camera_zuray+=0.2f;break;
		case 'Y':g_draw.camera_zuray-=0.2f;break;

		//Rectangle
		case 'R':sw_rects = !sw_rects;break;
		//Full throttle
		case 'F':m_loopTime=1;break;

		case VK_F2:
			g_system.sw_showstates++;
			break;
		case VK_F3:
			taskManager.DebugOutputTaskList();
			break;
		case VK_F4:
			switch(m_loopTime){
				case 40:m_loopTime=80;break;
				case 80:m_loopTime=160;break;
				case 160:UpdateFrameRate();break;
				default:m_loopTime=40;
			}
			break;
	}
}



/*------------------------------------------------------------------------------
	�V�X�e���Ŋm�ۂ��Ă���wav���Đ�
--------------------------------------------------------------------------------*/
void CSystem::PlaySystemSound(DWORD sid)
{
	if(sid>=NUM_SYSTEMSOUND)return;
	int i=0;
	if(b_efctlst[sid])i=1;
	if(dsb_efct[i][sid]!=NULL){
		dsb_efct[i][sid]->Stop();
		dsb_efct[i][sid]->SetCurrentPosition(0);
		dsb_efct[i][sid]->Play(0,0,0);
		b_efctlst[sid]=!b_efctlst[sid];
	}
}


/*------------------------------------------------------------------------------
	�V�X�e���ōĐ�����T�E���h�����[�h
--------------------------------------------------------------------------------*/
void CSystem::InitSystemSound()
{
	for(int i=0;i<NUM_SYSTEMSOUND;i++){
		dsb_efct[0][i]=NULL;
		dsb_efct[1][i]=NULL;
		b_efctlst[i]=FALSE;
	}

	dsb_efct[0][ 0] = g_sound.CreateDSB(_T(".\\system\\sound\\hit1.wav"));
	dsb_efct[0][ 1] = g_sound.CreateDSB(_T(".\\system\\sound\\hit2.wav"));
	dsb_efct[0][ 2] = g_sound.CreateDSB(_T(".\\system\\sound\\hit3.wav"));
	dsb_efct[0][ 3] = g_sound.CreateDSB(_T(".\\system\\sound\\shock1.wav"));
	dsb_efct[0][ 4] = g_sound.CreateDSB(_T(".\\system\\sound\\shock2.wav"));
	dsb_efct[0][ 5] = g_sound.CreateDSB(_T(".\\system\\sound\\guard.wav"));
	dsb_efct[0][ 6] = g_sound.CreateDSB(_T(".\\system\\sound\\catch.wav"));
	dsb_efct[0][ 7] = g_sound.CreateDSB(_T(".\\system\\sound\\cyohi.wav"));
	dsb_efct[0][ 8] = g_sound.CreateDSB(_T(".\\system\\sound\\charge.wav"));
	//	dsb_efct[0][ 9] = g_sound.CreateDSB(_T(".\\system\\sound\\ko.wav"));
	//	dsb_efct[0][10] = g_sound.CreateDSB(_T(".\\system\\sound\\fight.wav"));
	dsb_efct[0][ 9] = g_sound.CreateDSB(_T(".\\system\\sound\\select.wav"));
	dsb_efct[0][11] = g_sound.CreateDSB(_T(".\\system\\sound\\change.wav"));
	dsb_efct[0][12] = g_sound.CreateDSB(_T(".\\system\\sound\\ok.wav"));

	for(int i=0;i<NUM_SYSTEMSOUND;i++){
		// DirectSound�̊֐����g���ăR�s�[
		// �蔲���A�{���͐�p�֐�����������ǂ����B
		g_sound.lpds->DuplicateSoundBuffer( dsb_efct[0][i], &dsb_efct[1][i] );
	}
}


/*------------------------------------------------------------------------------
	�V�X�e���Ŏg�p����r�b�g�}�b�v��CELL���̃O���t�B�b�N�����[�h
--------------------------------------------------------------------------------*/
void CSystem::InitSystemGraphics()
{
	char filename[256];
	for(int i=0;i<GCDMAX_IMAGES;i++){
		sprintf(filename,_T(".\\system\\image%d"),i+1);
		sdds[i] = g_draw.CreateSurfaceFrom256Image(filename);
	}
	sprintf(filename,_T(".\\system\\cell2.gcm"));
	sdds[3] = g_draw.CreateSurfaceFrom256BMP(filename);
	sprintf(filename,_T(".\\system\\cell.gcm"));

	CGCDHandler::GCDLoadCompressed(filename,scdat,srdat,shdat);
}


/*------------------------------------------------------------------------------
	�Q�[���J�n����̃C���t�H���[�V�����_�C�A���O�{�b�N�X
--------------------------------------------------------------------------------*/
BOOL CALLBACK DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);

BOOL CSystem::ShowInformation()
{
	if(DialogBox(GetModuleHandle(NULL),
		MAKEINTRESOURCE(IDD_DIALOG1),
		hwnd,
		DialogProc)!=0)return(FALSE);
	return(TRUE);
}

#define ADDLIST(x) SendMessage(GetDlgItem(hwndDlg,IDC_LIST1),LB_ADDSTRING,0,(LPARAM)(LPCTSTR)x)

//! �Q�[���J�n����̃C���t�H���[�V�����_�C�A���O�{�b�N�X�̃��b�Z�[�W�v���V�[�W��
BOOL CALLBACK DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	char *str;
	int i;
	BOOL cannot_start=FALSE;
	if(uMsg == WM_INITDIALOG){
		gbl.SetWinCenter(hwndDlg);
		str = new char[1024*64];
		//config �ǂݍ��݌���
		sprintf(str,_T("���ݒ�t�@�C���ǂݍ���"));
		ADDLIST(str);
		if(!g_config.readok1){
			sprintf(str,_T("config.dat(�L�[�ݒ�)�Ǎ��ݎ��s �f�t�H���g��K�p"));
			ADDLIST(str);
		}
		else{
			sprintf(str,_T("config.dat �ǂݍ���OK"));
			ADDLIST(str);
		}
		if(!g_config.readok2){
			sprintf(str,_T("config2.dat�̓Ǎ��ݎ��s �f�t�H���g��K�p"));
			ADDLIST(str);
		}
		else{
			sprintf(str,_T("config2.dat �ǂݍ���OK"));
			ADDLIST(str);
		}
		ADDLIST(_T(" "));
		//�p�b�h�̌�������
		sprintf(str,_T("���Q�[���p�b�h��������"));
		ADDLIST(str);
		sprintf(str,_T("%d�̃Q�[���p�b�h��������܂���"),g_input.jsnum);
		ADDLIST(str);
		if(g_input.jsnum!=0){
			for(int i=0;i<g_input.jsnum;i++){
				sprintf(str,_T("%d : %s"),i+1,g_input.gamepadname[i]);
				ADDLIST(str);
			}
		}
		ADDLIST(_T(" "));
		//�L�����N�^�[�̌�������
		ADDLIST(_T("���L�����N�^�[��������"));
		for(i=0;i<g_charlist.GetRingNum();i++){
			sprintf(str,_T("%s\\ : %d"),g_charlist.GetRingName(i),g_charlist.GetCharacterCountRing(i));
			ADDLIST(str);
		}
		sprintf(str,_T("�v : %d"),g_charlist.GetCharacterCount());
		ADDLIST(str);

		ADDLIST(_T(" "));
		ADDLIST(_T("(��ͥ)�ǂݍ��݂ɐ��������L�����N�^�[(��ͥ)"));
		sprintf(str,_T("�S:%d��"),g_charlist.GetCharacterCount());
		ADDLIST(str);
		for(i=0;i<g_charlist.GetCharacterCount();i++){
			sprintf(str,_T("%s : %s - ver%4.3f"),
				g_charlist.GetCharacterDir(i),g_charlist.GetCharacterName(i),
				g_charlist.GetCharacterVer(i)/1000.0);
			ADDLIST(str);
		}
		ADDLIST(_T(" "));
		ADDLIST(_T("(��`�)�ǂݍ��݂Ɏ��s�����L�����N�^�[(��`�)"));
		sprintf(str,_T("�S:%d��"),g_charlist.GetDameCharCount());
		ADDLIST(str);
		for(i=0;i<g_charlist.GetDameCharCount();i++){
			sprintf(str,_T("%s : "),g_charlist.GetDameCharDir(i));
			switch(g_charlist.GetDameCharReas(i)){
			case CCL_DAME_NODLL://action.dll�̓ǂݍ��݂Ɏ��s
				sprintf(&str[strlen(str)],_T("dll�̓ǂݍ��݂Ɏ��s���܂���"));
				break;
			case CCL_DAME_CANTGETFP://�֐��|�C���^�擾�Ɏ��s
				sprintf(&str[strlen(str)],_T("�֐��|�C���^�̎擾�Ɏ��s���܂���"));
				break;
			case CCL_DAME_FFAIL://�֐���FALSE��Ԃ��Ă���
				sprintf(&str[strlen(str)],_T("CharacterInfo�֐���FALSE��Ԃ��܂���"));
				break;
			case CCL_DAME_OLDDLL://�o�[�W�����`�F�b�N�Ɏ��s
				sprintf(&str[strlen(str)],_T("ver.%4.3f(��) - %s"),
					(double)g_charlist.GetDameCharVer(i)/1000.0,
					g_charlist.GetDameCharName(i));
				break;
			case CCL_DAME_NEWDLL://�o�[�W�����`�F�b�N�Ɏ��s(2)
				sprintf(&str[strlen(str)],_T("ver.%4.3f(�V) - %s"),
					(double)g_charlist.GetDameCharVer(i)/1000.0,
					g_charlist.GetDameCharName(i));
				break;
			default:
				sprintf(&str[strlen(str)],_T("�G���[�����ł��܂���"));
				break;
			}
			ADDLIST(str);
		}
		ADDLIST(_T(" "));
		//�X�e�[�W�̌�������
		ADDLIST(_T("���X�e�[�W��������"));
		sprintf(str,_T("�S%d�X�e�[�W"),g_stagelist.GetStageCount());
		ADDLIST(str);
		ADDLIST(_T(" "));
		ADDLIST(_T("(`��֥�L)�ǂݍ��݂ɐ��������X�e�[�W(`��֥�L)"));
		for(i=0;i<g_stagelist.GetStageCount();i++){
			sprintf(str,_T("%s - %s"),g_stagelist.GetStageDir(i),g_stagelist.GetStageName(i));
			if(g_stagelist.GetStageVer(i)!=0){
				sprintf(&str[strlen(str)],_T("(ver%4.3f)"),g_stagelist.GetStageVer(i)/1000.0);
			}
			else{
				sprintf(&str[strlen(str)],_T("(DLL�Ȃ�)"));
			}
			ADDLIST(str);
		}
		ADDLIST(_T(" "));
		ADDLIST(_T("(�L��֥`)�ǂݍ��݂Ɏ��s�����X�e�[�W(�L��֥`)"));
		sprintf(str,_T("�S%d�X�e�[�W"),g_stagelist.GetDameStageCount());
		ADDLIST(str);
		for(i=0;i<g_stagelist.GetDameStageCount();i++){
			sprintf(str,_T("%s"),g_stagelist.GetDameStageDir(i));
			switch(g_stagelist.GetDameStageReason(i)){
			case CSL_DAME_NONAME:
				sprintf(&str[strlen(str)],_T("(���O�擾�Ɏ��s)"));
				break;
			case CSL_DAME_PROC:
				sprintf(&str[strlen(str)],_T("(�֐��|�C���^�擾�Ɏ��s)"));
				break;
			case CSL_DAME_PROC2:
				sprintf(&str[strlen(str)],_T("(StageInfo�Ɏ��s)"));
				break;
			case CSL_DAME_OLDDLL:
				sprintf(&str[strlen(str)],_T("(DLL���Â�-%4.3f)"),g_stagelist.GetDameStageVer(i)/1000.0);
				break;
			case CSL_DAME_NEWDLL:
				sprintf(&str[strlen(str)],_T("(DLL���V������-%4.3f)"),g_stagelist.GetDameStageVer(i)/1000.0);
				break;
			}
			ADDLIST(str);
		}
		ADDLIST(_T(" "));
		//�X�g�[���[
		ADDLIST(_T("���X�g�[���[���،���"));
		ADDLIST(_T("���ǂݍ��ݐ�����"));
		for(i=0;i<(int)g_storylist.GetAllStoryNum();i++){
			sprintf(str,_T("%s : %s"),g_storylist.GetStoryDir(i),g_storylist.GetStoryName(i));
			ADDLIST(str);
		}
		ADDLIST(_T("�~�ǂݍ��ݎ��s�~"));
		for(i=0;i<g_storylist.GetErrorCount();i++){
			sprintf(str,_T("%s : %s"),g_storylist.GetErrorDir(i),g_storylist.GetErrorStr(i));
			ADDLIST(str);
		}
		ADDLIST(_T(" "));

		if(g_charlist.GetCharacterCount()==0){//�L�����N�^�[��������Ȃ�
			ADDLIST(_T(" "));
			ADDLIST(_T("�L�����N�^�[����̂����܂���B�Q�[�����J�n�ł��܂���B"));
			ADDLIST(_T(" "));
			cannot_start = TRUE;
		}
		if(g_stagelist.GetStageCount()==0){//�X�e�[�W��������Ȃ�
			ADDLIST(_T(" "));
			ADDLIST(_T("�X�e�[�W���������܂���B�Q�[�����J�n�ł��܂���B"));
			ADDLIST(_T(" "));
			cannot_start = TRUE;
		}

		delete [] str;

		if(cannot_start){
			EnableWindow(GetDlgItem(hwndDlg,IDOK),FALSE);
		}
	/*	if(CDirectPlay::isLobbyLunch){
			EnableWindow(GetDlgItem(hwndDlg,IDC_RADIO1),FALSE);
			EnableWindow(GetDlgItem(hwndDlg,IDC_RADIO2),FALSE);
			EnableWindow(GetDlgItem(hwndDlg,IDC_RADIO3),FALSE);
			EnableWindow(GetDlgItem(hwndDlg,IDC_RADIO4),TRUE);
			SendMessage(GetDlgItem(hwndDlg,IDC_RADIO4),BM_SETCHECK, BST_CHECKED, 0);
		}
		else{
			SendMessage(GetDlgItem(hwndDlg,IDC_RADIO1),BM_SETCHECK, BST_CHECKED, 0);
			EnableWindow(GetDlgItem(hwndDlg,IDC_RADIO4),FALSE);
		}
		if(config.IsDebugMode() || !g_charlist.netOK){//g_input.jsnum==0 || 
			EnableWindow(GetDlgItem(hwndDlg,IDC_RADIO2),FALSE);
			EnableWindow(GetDlgItem(hwndDlg,IDC_RADIO3),FALSE);
		}*/
		else SetFocus(GetDlgItem(hwndDlg,IDOK));
	}

	switch( uMsg ){
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDC_BUTTON1:
		case IDCLOSE:
		case IDCANCEL:
			EndDialog(hwndDlg,1);
			break;
		case IDOK:
		/*	if(SendMessage(GetDlgItem(hwndDlg,IDC_RADIO1),BM_GETCHECK,0,0)==BST_CHECKED)network = NETWORK_NONE;
			else if(SendMessage(GetDlgItem(hwndDlg,IDC_RADIO2),BM_GETCHECK,0,0)==BST_CHECKED)network = NETWORK_HOST;
			else if(SendMessage(GetDlgItem(hwndDlg,IDC_RADIO3),BM_GETCHECK,0,0)==BST_CHECKED)network = NETWORK_CLIENT;*/
			EndDialog(hwndDlg,0);
			break;
		}
	default:return(FALSE);
	}
	return(TRUE);
}



/*------------------------------------------------------------------------------
	���C���E�C���h�E���A�N�e�B�u���ǂ����𒲂ׂ�
--------------------------------------------------------------------------------*/
BOOL CSystem::IsWindowActive()
{
	if( GetForegroundWindow()==hwnd )return(TRUE);
	else return FALSE;
}


/*!
*	�r�b�g�}�b�v�ɂ��e�L�X�g�`��
*	�t�H���g�ɂ��e�L�X�g�`��͏d�����ۂ��̂ŁA�ɗ͂��������g���ׂ��H
*	�A���t�@�x�b�g�啶��(A-Z)�Ɛ����ƋɈꕔ�̋L�������T�|�[�g���Ȃ�
*	�������A���t�@�x�b�g�͑啶���ɕϊ������
*	�T�|�[�g����L��  +-%:/
*/
//CSystem::DrawBMPText �Ŏg�p�B�����R�[�h��Ή�����Z���ԍ��ɕϊ�
int char2cell(char c)
{
	if(c<0x20)return -1;//�\���ł��Ȃ�����
	if(c==0x20)return 0;//�S�p�X�y�[�X
	if(c>0x7E)return -1;//�\���ł��Ȃ�����

	return CELL_BMPTEXT21 + (c - 0x21);
}

#define CELLDRAW(x,y,z,id,col) g_draw.CellDraw(sdds,scdat,srdat,id,x,y,z,0,FALSE,FALSE,col);

/*! �r�b�g�}�b�v�e�L�X�g�`��B
*	�A���t�@�x�b�g�i�啶���j�A�����A+-%�̂�
*/
double CSystem::DrawBMPText(double x,double y,float z,char *str,DWORD col)
{
	return DrawBMPTextEx(x,y,z,str,col,1.0f,1.0f,SYSBMPTXT_DEFAULT);
}

/*!�r�b�g�}�b�v�e�L�X�g�`��Ex
*	�g��k���E�C���`�L�v���|�[�V���i���E�E���e�L�X�g���I���ł���
*/
double CSystem::DrawBMPTextEx(double x,double y,float z,char *str,DWORD col,float scaX,float scaY,DWORD flags)
{
	if(scaX<0.01f)return x;
	BOOL dr = (scaY<0.01f) ? FALSE : TRUE;

	//local val.
	int cno;
	int step = 16;
	RECT *pr;
	ARGB shadecol;

	shadecol.dwcol = col;
	switch(flags & SYSBMPTXT_SHADEMASK)
	{
	case SYSBMPTXT_SHADE://�e��(1)
		{
			shadecol.red /= 2;
			shadecol.green /= 2;
			shadecol.blue /= 2;
		}break;
	case SYSBMPTXT_SHADE_W://�^�����e��
		{
			shadecol.red = 255;
			shadecol.green = 255;
			shadecol.blue = 255;
		}break;
	case SYSBMPTXT_SHADE_B://�^�����e��
		{
			shadecol.red = 0;
			shadecol.green = 0;
			shadecol.blue = 0;
		}break;
	}
	

	if(flags&SYSBMPTXT_ALPHAADD)g_draw.SetAlphaMode(GBLEND_KASAN);

	//�t����
	if(flags & SYSBMPTXT_R2L){
		int len = strlen(str);
		char *tmp = new char [len+1];
		for(int t=0;t<len;t++){
			tmp[len-t-1] = str[t];
		}
		tmp[len] = '\0';
		str = tmp;
		while(*str!='\0'){
			cno = char2cell(*str);
			if(cno>0){
				//step�����߂�
				if(flags&SYSBMPTXT_PROP){
					pr = &srdat[scdat[cno].cell[0].cdr].r;
					step = (int)((pr->right-pr->left)*scaX);
				}
				if(dr){
					if(flags&SYSBMPTXT_SHADEMASK){
						g_draw.CellDraw(sdds,scdat,srdat,cno,(int)x-step+1,(int)y+1,0,0,FALSE,FALSE,shadecol.dwcol,scaX,scaY);
					}
					g_draw.CellDraw(sdds,scdat,srdat,cno,(int)x-step,(int)y,0,0,FALSE,FALSE,col,scaX,scaY);
				}
			}
			else step=(int)(10*scaX);
			if(step<0)step*=-1;
			x-=step+1;
			str++;
		}
		delete [] tmp;
		g_draw.SetAlphaMode(0);
		return x;
	}

	//�ӂ[
	while(*str!='\0'){
		cno = char2cell(*str);
		if(cno>0){
			//step�����߂�
			if(flags&SYSBMPTXT_PROP){
				pr = &srdat[scdat[cno].cell[0].cdr].r;
				step = (int)((pr->right-pr->left)*scaX +1);
			}
			if(dr){
				if(flags&SYSBMPTXT_SHADEMASK){
					g_draw.CellDraw(sdds,scdat,srdat,cno,(int)x+1,(int)y+1,0,0,FALSE,FALSE,shadecol.dwcol,scaX,scaY);
				}
				g_draw.CellDraw(sdds,scdat,srdat,cno,(int)x,(int)y,0,0,FALSE,FALSE,col,scaX,scaY);
			}
		}
		else step=(int)(10*scaX);
		if(step<0)step*=-1;
		x+=step+1;
		str++;
	}

	g_draw.SetAlphaMode(0);
	return x;
}



/*------------------------------------------------------------------
	���݂̃��C���^�X�N�N���X���擾
--------------------------------------------------------------------*/
CExclusiveTaskBase* CSystem::GetCurrentMainTask()
{
	return taskManager.GetTopExclusiveTask();
}

/*------------------------------------------------------------------
	�w��ID�̃o�b�N�O���E���h�^�X�N���擾
--------------------------------------------------------------------*/
CBackgroundTaskBase* CSystem::FindBGTask(DWORD id)
{
	return taskManager.FindBGTask(id);
}

/*------------------------------------------------------------------
	�w��ID�̒ʏ�^�X�N���擾
--------------------------------------------------------------------*/
CTaskBase* CSystem::FindTask(DWORD id)
{
	return taskManager.FindTask(id);
}

/*------------------------------------------------------------------
	���O
--------------------------------------------------------------------*/

char* CSystem::logBuffer = NULL;

void CSystem::Log(const char *log,DWORD flag)
{
	#ifdef _DBG_MESSAGES_TO_CONSOLE
	if(flag!=SYSLOG_DEBUG){
		OutputDebugString(log);
		OutputDebugString(_T("\n"));
	}
	#endif

	if(!g_config.UseLog2File()){
		return;
	}

	BOOL log2file = FALSE;
	switch(flag){
	case SYSLOG_INFO	:
		{
			sprintf(logBuffer,_T("[info]:%s"),log);
			log2file = g_config.UseLog_Info();
		}
		break;
	case SYSLOG_ERROR	:
		{
			sprintf(logBuffer,_T("[error]:%s"),log);
			log2file = TRUE;
		}
		break;
	case SYSLOG_ALERT	:
	case SYSLOG_WARNING	:
		{
			sprintf(logBuffer,_T("[warning]:%s"),log);
			log2file = g_config.UseLog_Warning();
		}
		break;
	case SYSLOG_DEBUG	:
		{
			sprintf(logBuffer,_T("[debug]:%s"),log);
			log2file = g_config.UseLog_Debug();
		}
		break;
	}
	
	if(log2file){
		g_log2file.AddLog(logBuffer);
		if(flag==SYSLOG_ERROR)g_log2file.Flush();
	}
}

void CSystem::LogWarning(const char *format, ...)
{
	strcpy(logBuffer,_T("[warning]:"));

	va_list args;
	va_start(args, format);
	vsprintf(&logBuffer[strlen(logBuffer)], format, args);
	va_end(args);

	OutputDebugString(logBuffer);
	OutputDebugString(_T("\n"));

	if(g_config.UseLog_Warning())g_log2file.AddLog(logBuffer);
}


void CSystem::LogErr(const char *format,...)
{
	strcpy(logBuffer,_T("[error]:"));

	va_list args;
	va_start(args, format);
	vsprintf(&logBuffer[strlen(logBuffer)], format, args);
	va_end(args);

	OutputDebugString(logBuffer);
	OutputDebugString(_T("\n"));

	g_log2file.AddLog(logBuffer);
}

void PngErrHandler(png_structp Png,png_const_charp message);

void CSystem::SaveScreenShot()
{
	//�R�s�[
	UINT width , height;
	DWORD *bits = g_draw.GetFrontBufferCopyRaw(&width,&height);
	if(!bits){
		Log(_T("�X�N���[���V���b�g�̍쐬�Ɏ��s(�t�����g�o�b�t�@�[�R�s�[)"),SYSLOG_WARNING);
		return;
	}

	//�ۑ��t�@�C�������Ђ˂�o��
	char* filename = new char[256];
	time_t crnt_time;
	time(&crnt_time);
	struct tm* crnt_time_l = localtime(&crnt_time);
	sprintf(filename,_T("%s\\%d%s%d%s%d%s%d%s%d%s%d.%s"),
		_T("system\\sshot\\"),
		crnt_time_l->tm_year + 1900,				//�N
		(crnt_time_l->tm_mon + 1)<10 ? _T("0") : _T(""),
		crnt_time_l->tm_mon + 1,					//��
		crnt_time_l->tm_mday<10 ? _T("0") : _T(""),
		crnt_time_l->tm_mday,						//��
		crnt_time_l->tm_hour<10 ? _T("0") : _T(""),
		crnt_time_l->tm_hour,						//��
		crnt_time_l->tm_min<10 ? _T("0") : _T(""),
		crnt_time_l->tm_min,						//��
		crnt_time_l->tm_sec<10 ? _T("0") : _T(""),
		crnt_time_l->tm_sec,						//�b
		g_config.GetSShotFileTypeStr()				//�g���q
	);

/*	���������gdiplus���K�v�ɂȂ�

	CImage img;
	img.Create(width,height,32);
	ARGB col;
	BYTE tmp;
	for(UINT y=0;y<height;y++)
	{
		for(UINT x=0;x<width;x++)
		{
			col.dwcol = *(bits + y*width + x);

			//�ԂƐ��t�炵��
			tmp = col.red;
			col.red = col.blue;
			col.blue = tmp;

			img.SetPixel(x,y,col.dwcol);
		}
	}
	img.Save(filename);//,ImageFormatJPEG);*/

	//�r�b�g�}�b�v�ŕۑ�
	if (g_config.GetSShotImageFormat() == SSHOT_BMP)
	{
		UINT stride = width*3;
		stride += (stride%4) ? (4-stride%4) : 0;

		//�t�@�C���w�b�_
		BITMAPFILEHEADER head;
		head.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		head.bfReserved1 = 0;
		head.bfReserved2 = 0;
		head.bfSize = sizeof(head) + sizeof(BITMAPINFOHEADER) + height * stride;
		head.bfType = 'MB';

		//�r�b�g�}�b�v�w�b�_
		BITMAPINFOHEADER head2;
		head2.biBitCount = 24;		//32;	//32:The high byte in each DWORD is not used
		head2.biClrImportant = 0;	//?
		head2.biClrUsed = 0;		//the bitmap uses the maximum number of colors corresponding to the value of the biBitCount member
		head2.biCompression = BI_RGB;
		head2.biHeight = height;
		head2.biPlanes = 1;
		head2.biSize = sizeof(BITMAPINFOHEADER);
		head2.biSizeImage = height*stride;
		head2.biWidth = width;
		head2.biXPelsPerMeter = 2000;//�K���ł悢�H
		head2.biYPelsPerMeter = 2000;//�K���ł悢�H

		//�t�@�C���J����
		CFile file;
		if(file.Open(filename,CFile::modeWrite | CFile::modeCreate))
		{
			//�w�b�_��������
			file.Write(&head ,sizeof(BITMAPFILEHEADER));
			file.Write(&head2,sizeof(BITMAPINFOHEADER));
			
			#if 1
			{
				//32��24�ϊ��p�o�b�t�@
				BYTE *cbuf = new BYTE[ stride ];
				ZeroMemory(cbuf,stride);

				if (!cbuf)
				{
					gbl.ods2(_T("CSystem::SaveScreenShot : �ϊ��p�o�b�t�@���Ȃ���\n"));
					file.Close();
					CFile::Remove(filename);
					goto SSHOT_FAILED;
				}

				for(UINT y=0;y<height;y++)
				{
					//32��24
					BYTE  *pd = cbuf;
					DWORD *ps = bits + (height-y-1)*width;
					for(UINT x=0;x<width;x++)
					{
						*pd = (BYTE)(( *ps     ) & 0x000000FF);
						pd++;
						*pd = (BYTE)(((*ps)>> 8) & 0x000000FF);
						pd++;
						*pd = (BYTE)(((*ps)>>16) & 0x000000FF);
						pd++;
						ps++;
					}

					//1���C������������
					file.Write( cbuf , stride );
				}

				DELETEARRAY(cbuf);
			}
			#else
			{
				for(UINT y=0;y<height;y++)
				{
					file.Write( bits + (height-y-1)*width , 4*width );
				}
			}
			#endif
			file.Close();
		}
		else
		{
			Log(_T("�X�N���[���V���b�g�̍쐬�Ɏ��s(�t�@�C���I�[�v��)"),SYSLOG_WARNING);
		}
	}
	else if (g_config.GetSShotImageFormat() == SSHOT_PNG)	// PNG�ۑ�
	{
		FILE* fp;

		// PNG�\����
		png_structp strPNG = png_create_write_struct(PNG_LIBPNG_VER_STRING, _T("�P�z�p�z�̖�"), PngErrHandler, NULL);
		if (!strPNG)
		{
			gbl.ods2(_T("CDirectDraw::Load256PNGbits : PNG�\�����޷�ȴ�!!�R(`�D�L)ɳܧ��!!\n"));
			goto SSHOT_FAILED;
		}

		// ���\����
		png_infop infoPNG = png_create_info_struct(strPNG);
		if (!infoPNG)
		{
			gbl.ods2(_T("CDirectDraw::Load256PNGbits : PNG���\�����޷�ȴ�!!�R(`�D�L)ɳܧ��!!\n"));
			png_destroy_write_struct(&strPNG, NULL);
			goto SSHOT_FAILED;
		}

		// �������ݏ���
		fp = fopen(filename, _T("wb"));
		if (!fp)
		{
			png_destroy_write_struct(&strPNG, &infoPNG);
			goto SSHOT_FAILED;
		}

		// ���C�u�����ɔC����
		png_init_io(strPNG, fp);

		// �摜�̏���ݒ�
		png_set_IHDR(strPNG, infoPNG,				// �\���̈ꎮ
					 width,							// ��
					 height,						// ����
					 8,								// �r�b�g�[�x
					 PNG_COLOR_TYPE_RGB,			// �J���[�^�C�v
					 PNG_INTERLACE_NONE,			// �C���^���[�X
					 PNG_COMPRESSION_TYPE_DEFAULT,	// ���k���@�H
					 PNG_FILTER_TYPE_DEFAULT);		// ���΂��̃t�B���^�[�𔲂��ċz���̂͂�߂܂��傤�B

		// �t�@�C���ɂԂ�����
		png_write_info(strPNG, infoPNG);

		{
			UINT stride = width*3;
			stride += (stride%4) ? (4-stride%4) : 0;

			//32��24�ϊ��p�o�b�t�@
			BYTE *cbuf = new BYTE[ stride ];

			if (!cbuf)
			{
				gbl.ods2(_T("CSystem::SaveScreenShot : �ϊ��p�o�b�t�@���Ȃ���\n"));
				fclose(fp);
				png_destroy_write_struct(&strPNG, &infoPNG);
				goto SSHOT_FAILED;
			}

			ZeroMemory(cbuf,stride);

			for(int y=height - 1;y >= 0;y--)
			{
				//32��24
				BYTE  *pd = cbuf;
				DWORD *ps = bits + (height-y-1)*width;
				for(UINT x=0;x<width;x++)
				{
					*pd = (BYTE)(((*ps)>>16) & 0x000000FF);
					pd++;
					*pd = (BYTE)(((*ps)>> 8) & 0x000000FF);
					pd++;
					*pd = (BYTE)(( *ps     ) & 0x000000FF);
					pd++;
					ps++;
				}

				//1���C������������
				png_write_row(strPNG, cbuf);
			}

			DELETEARRAY(cbuf);
		}

		// ����
		png_write_end(strPNG, infoPNG);
		fclose(fp);
		png_destroy_write_struct(&strPNG, &infoPNG);
	}
	else	// JPEG�ۑ�
	{
		struct jpeg_compress_struct cinfo;
		struct jpeg_error_mgr jerr;
		FILE* fp;

		// �\���̏�����
		ZeroMemory(&cinfo, sizeof(cinfo));
		ZeroMemory(&jerr, sizeof(jerr));
		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_compress(&cinfo);

		// �������ݏ���
		fp = fopen(filename, _T("wb"));
		if (!fp)
		{
			jpeg_destroy_compress(&cinfo);
			goto SSHOT_FAILED;
		}

		// ����
		jpeg_stdio_dest(&cinfo, fp);

		// �F�X���ݒ�
		cinfo.image_width = width;
		cinfo.image_height = height;
		cinfo.input_components = 3;
		cinfo.in_color_space = JCS_RGB;
		jpeg_set_defaults(&cinfo);			// ��̓��C�u�����ɔC����

		// �掿��ݒ�
		jpeg_set_quality(&cinfo, 70, TRUE);

		// ���ˏ���
		jpeg_start_compress(&cinfo, TRUE);

		{
			UINT stride = width*3;
			stride += (stride%4) ? (4-stride%4) : 0;

			//32��24�ϊ��p�o�b�t�@
			BYTE *cbuf = new BYTE[ stride ];

			if (!cbuf)
			{
				gbl.ods2(_T("CSystem::SaveScreenShot : �ϊ��p�o�b�t�@���Ȃ���\n"));
				fclose(fp);
				jpeg_destroy_compress(&cinfo);
				goto SSHOT_FAILED;
			}

			// �f�[�^�`�B�Ɏg���|�C���^
			JSAMPROW row = (JSAMPROW)cbuf;

			ZeroMemory(cbuf,stride);

			for(int y=height - 1;y >= 0;y--)
			{
				//32��24
				BYTE  *pd = cbuf;
				DWORD *ps = bits + (height-y-1)*width;
				for(UINT x=0;x<width;x++)
				{
					*pd = (BYTE)(((*ps)>>16) & 0x000000FF);
					pd++;
					*pd = (BYTE)(((*ps)>> 8) & 0x000000FF);
					pd++;
					*pd = (BYTE)(( *ps     ) & 0x000000FF);
					pd++;
					ps++;
				}

				//1���C������������
				jpeg_write_scanlines(&cinfo, &row, 1);
			}

			DELETEARRAY(cbuf);
		}

		// ����
		jpeg_finish_compress(&cinfo);
		jpeg_destroy_compress(&cinfo);
		fclose(fp);
	}

SSHOT_FAILED:
	delete [] filename;
	delete [] bits;
}

// �ݒ�ɏ]���t���[�����[�g��ύX�B
void CSystem::UpdateFrameRate()
{
	switch ( g_config.GetGameSpeed() )
	{
	case GSPEED_SLOW:	m_loopTime = 25; break;
	case GSPEED_OLD:	m_loopTime = 20; break;
	case GSPEED_NEW:	m_loopTime = 1000 / 60; break;
	case GSPEED_30x2:	m_loopTime = 1000 / 60; break;
//	case GSPEED_25x2:	m_loopTime = 20; break;
	}
}


void CSystem::PushSysTag(const char* tag_str)
{
	m_systag.push(tag_str);
}

void CSystem::PopSysTag()
{
	if(m_systag.size()!=0){
		m_systag.pop();
	}
}

void CSystem::ClearSysTag()
{
	while( m_systag.size()!=0 ){
		m_systag.pop();
	}
	ClearDLLTag();
}

void CSystem::PushDLLTag(const char* tag_str)
{
	m_dlltag.push(tag_str);
}

void CSystem::PopDLLTag()
{
	if(m_dlltag.size()!=0){
		m_dlltag.pop();
	}
}

void CSystem::ClearDLLTag()
{
	while( m_dlltag.size()!=0 ){
		m_dlltag.pop();
	}
}

void CSystem::DumpTags()
{
	while( m_dlltag.size()!=0 )
	{
		LogErr(_T("\t<dll(%d)> %s"),
				m_dlltag.size(),
				(m_dlltag.top()) ? (m_dlltag.top()) : _T("(null)")
				);
		m_dlltag.pop();
	}

	while( m_systag.size()!=0 )
	{
		LogErr(_T("\t<sys(%d)> %s"),
				m_systag.size(),
				m_systag.top() ? m_systag.top() : _T("(null)")
				);
		m_systag.pop();
	}
}
