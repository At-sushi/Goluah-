
/*============================================================================

	システムクラス

	メインシステム。ゲーム全般の統括

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
	生成
--------------------------------------------------------------------------------*/
CSystem::CSystem()
{
	int i;

	//サウンドのアンロード
	for(i=0;i<NUM_SYSTEMSOUND;i++){
		RELEASE(dsb_efct[0][i]);
		RELEASE(dsb_efct[1][i]);
	}

	//システムグラフィック破棄
	for(i=0;i<GCDMAX_IMAGES;i++)
	{
		g_draw.RelSurface(sdds[i]);
		sdds[i] = NULL;
	}

	//その他いろいろ
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
*	初期化
*	起動後ダイアログが表示される前までのもろもろの初期化。
*	この関数の最後でダイアログを表示。
*/
BOOL CSystem::Initialize(HWND hwnd)
{
	srand(timeGetTime());
	this->hwnd = hwnd;
	logBuffer = new TCHAR [512];

	// CDirectPlay生成
	/*if (!g_play.Initialize(hwnd))
	{
		MessageBox(hwnd,_T("通信系の初期化に失敗"),_T("初期化エラー(2)"),MB_OK | MB_ICONERROR);
		Destroy();
		return FALSE;
	}*/

	//CDirectSound生成
	if(g_config.UseDSound()){
		if(!g_sound.Initialize(hwnd)){
			MessageBox(hwnd,_T("音系の初期化に失敗"),_T("初期化エラー(1)"),MB_OK | MB_ICONERROR);
			Destroy();
			return FALSE;
		}
	}

	//システムサウンド読み込み
	InitSystemSound();

	//CDirectInput生成
	if(!g_input.Initialize(hwnd)){
		MessageBox(hwnd,_T("入力系の初期化に失敗"),_T("初期化エラー(2)"),MB_OK | MB_ICONERROR);
		Destroy();
		return FALSE;
	}
	if(g_config.UseDInput()){//パッドを使用する
		if(!g_input.InitializePad()){
			MessageBox(hwnd,_T("ゲームパッドの初期化に失敗"),_T("初期化エラー(2)"),MB_OK | MB_ICONERROR);
			Destroy();
			return FALSE;
		}
	}

	//リスト初期化
	g_stagelist.Initialize();	//ステージリスト構築

	//CDirectDraw生成
	if(!g_draw.Initialize(hwnd,!g_config.IsFullScreen())){
		MessageBox(hwnd,_T("描画系の初期化に失敗"),_T("初期化エラー(2)"),MB_OK);
		Destroy();
		return(FALSE);
	}

	//aki3d初期化
	aki3d.Initialize(g_draw.d3ddev,_T("system\\texture"));

	InitSystemGraphics();		//システムグラフィック読み込み

	//ダイアログ表示
	if(g_config.SwShowInfo() && !ShowInformation()){
		Destroy();
		return FALSE;
	}

	if(g_stagelist.GetStageCount()==0){//ステージが一個もない
		MessageBox(hwnd,_T("ステージが一つもありません。ゲームを開始できません。"),_T("初期化エラー(2)"),MB_OK);
		Destroy();
		return(FALSE);
	}

	ready=TRUE;
	return TRUE;
}



/*!
*	ゲーム開始
*	起動後の情報表示ダイアログが閉じられた後、メインループが開始される前に呼ばれる。
*	タイトルか、デバッグモードの場合デバッグメニューを初期タスクとして開始する。
*
*	タスクマネージャが（なぜ?）1フレに1つしか排他タスクを受け付けないようにしているので、
*	オプションでの起動直後ムービー再生はメインループの初回時に起動している。
*/
BOOL CSystem::GameStart()
{
	m_debug_menu_enabled = FALSE;

	UpdateFrameRate();

#ifdef _DEBUG
	m_debug_menu_enabled = TRUE;
#else
	if(/*g_config.IsDebugMode() && */GetKeyState(VK_SHIFT) & 0x8000)m_debug_menu_enabled = TRUE;

	//ゲーム開始
	//初期タスクの挿入
	if(m_debug_menu_enabled)AddTask( new CDebugMenu );
	else AddTask( new CTitle );
#endif
	return(TRUE);
}


/*------------------------------------------------------------------------------
	破棄
--------------------------------------------------------------------------------*/
void CSystem::Destroy()
{
	taskManager.Destroy();

	// システムサウンド解放
	// 解放処理が見あたらないのでとりあえずここで処理。
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
*	メインループ
*	エントリーポイントのwhileループから呼ばれている
*	FPS調節を行い、タスク管理クラスの定時処理を呼び出す。
*/
void CSystem::MainLoop()
{
	//fps調節。ウェイトをかける。
	static DWORD timeprv = timeGetTime();
	DWORD time_now = timeGetTime();
	if( timeprv+m_loopTime > time_now ){
		Sleep( timeprv+m_loopTime - time_now );//おすそ分け
		return;
	}
	time_now = timeGetTime();
	DWORD eat_time = time_now - timeprv;
	timeprv = time_now;

	//fps算出
	static DWORD timepass=0;
	static DWORD passCnt=0;
	timepass += eat_time;
	passCnt++;
	if(timepass > 1000){
		m_fps = passCnt;
		timepass=0;
		passCnt=0;
	}

	//ログを見やすくするために、挿入
	//if(g_config.CfgFullDebugLog()){
	//	g_log2file.AddLog(_T("-----------------------------main loop start"));
	//}

	//実行タグのクリア
	ClearSysTag();


	//タスク・Execute
	CExclusiveTaskBase *xtask = taskManager.GetTopExclusiveTask();
	taskManager.Execute(eat_time);

	static DWORD DrawCnt = 0; // 暫定
	if (xtask == taskManager.GetTopExclusiveTask() && !m_render_disabled && !IsIconic(this->hwnd) &&
		(m_loopTime > 1 || timepass % (1000 / 60) <= 1) && // ノーウェイト時軽量化
		(g_config.GetGameSpeed() != GSPEED_30x2 || DrawCnt % 2 == 0)) // コマ飛ばし処理
	{
		//タスク・Draw
		g_draw.StartDraw(TRUE);
		taskManager.Draw();
		g_draw.EndDraw();
	}
	else if(m_first_update_flag)
	{
		//インチキくさいフラグだが･･･
		m_first_update_flag = FALSE;
		if(g_config.SwShowMovieFirst()){
			AddTask(new CTOpening);
		}
	}
	DrawCnt++;
	//else メインのタスクが切り替わった直後は描画を行わない

	//タイトル画面戻り操作
	if(m_return_title_flag){
		//指定IDのタスクが現れるまでエクスクルーシブタスクをpop
		if(m_debug_menu_enabled)
			taskManager.ReturnExclusiveTaskByID('debg');
		else
			taskManager.ReturnExclusiveTaskByID('titl');

		m_return_title_flag = FALSE;
	}

	//なんかの間違いで全部なくなっちゃったときの復帰用
	if(taskManager.ExEmpty())
	{
		if(m_debug_menu_enabled)AddTask( new CDebugMenu );
		else AddTask( new CTitle );
	}

	// 暫定テスト措置
	// DirectPlayに処理させる
	g_play.Do(m_loopTime / 3);
}



/*!
*	ウインドウメッセージ処理
*
*	メインウインドウのメッセージプロシージャから呼ばれる。
*	メインウインドウと同じメッセージを受け取ることができる
*
*	処理はタスク管理クラスにもまわし、それを経由して各タスククラスまで配信される。
*
*	ESCキーでのゲーム終了はここで行っている模様
*/
void CSystem::WndMessage(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp)
{
	if(msg==WM_KEYDOWN){
		OnWMKeydown( wp, lp);//キー入力処理
	}

	taskManager.WndMessage(hWnd,msg,wp,lp);
}

/*!
*	タイトル画面戻り
*	関数内ではフラグを保存するだけで、実際の処理はメインループの中で行っている。
*	主にタスク管理クラスのタスクリストのループ内からこの関数が呼ばれているので、
*	そこからタスク破棄を行うと危ないから。多分。
*
*	通常の処理でこの関数が呼ばれた場合、エラーフラグを除去する
*	@sa NotifyExcption
*/
void CSystem::ReturnTitle()
{
	m_return_title_flag = TRUE;
	m_error_title = FALSE;
}

/*!
*	例外発生
*	設定により、そのまま続行するか、エラーフラグを立ててタイトルに戻る。
*	エラーフラグがたつとタイトル画面が赤っぽくなる。
*	@sa ReturnTitle
*/
void CSystem::NotifyExcption()
{
	g_system.DumpTags();

	if(g_config.IgnoreExceptions())return;
	/*MessageBox(hwnd, _T("　 ∧＿∧\n　（　´∀｀） ＜ ぬるぽが発生しますた"),
				_T("ウホッ！いいエラー…　　直　　さ　　な　　い　　か"), MB_OK | MB_ICONERROR);*/
	ReturnTitle();
	m_error_title = TRUE;
}

/*!
*	システム的キー入力処理
*	主にファンクションキーの操作に対応。
*	デバッグモードON設定の場合はさらに他にいくつかの機能を担当する。
*/
void CSystem::OnWMKeydown(WPARAM wp,LPARAM lp)
{
	switch(wp)
	{
	case VK_F6:SaveScreenShot();break;
	//case VK_F7:sw_pause = !sw_pause;break;//戦闘タスクで操作
	case VK_F8:ReturnTitle();break;
	}

	if(!g_config.IsDebugMode())return;

	//以下、デバッグモードのときのみ効くスイッチ
	switch(wp){
		//カメラ移動
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
	システムで確保しているwavを再生
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
	システムで再生するサウンドをロード
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
		// DirectSoundの関数を使ってコピー
		// 手抜き、本当は専用関数作った方が良さげ。
		g_sound.lpds->DuplicateSoundBuffer( dsb_efct[0][i], &dsb_efct[1][i] );
	}
}


/*------------------------------------------------------------------------------
	システムで使用するビットマップやCELL等のグラフィックをロード
--------------------------------------------------------------------------------*/
void CSystem::InitSystemGraphics()
{
	TCHAR filename[256];
	for(int i=0;i<GCDMAX_IMAGES;i++){
		_stprintf(filename,_T(".\\system\\image%d"),i+1);
		sdds[i] = g_draw.CreateSurfaceFrom256Image(filename);
	}
	_stprintf(filename,_T(".\\system\\cell2.gcm"));
	sdds[3] = g_draw.CreateSurfaceFrom256BMP(filename);
	_stprintf(filename,_T(".\\system\\cell.gcm"));

	CGCDHandler::GCDLoadCompressed(filename,scdat,srdat,shdat);
}


/*------------------------------------------------------------------------------
	ゲーム開始直後のインフォメーションダイアログボックス
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

//! ゲーム開始直後のインフォメーションダイアログボックスのメッセージプロシージャ
BOOL CALLBACK DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	TCHAR *str;
	int i;
	BOOL cannot_start=FALSE;
	if(uMsg == WM_INITDIALOG){
		gbl.SetWinCenter(hwndDlg);
		str = new TCHAR[1024*64];
		//config 読み込み結果
		_stprintf(str,_T("■設定ファイル読み込み"));
		ADDLIST(str);
		if(!g_config.readok1){
			_stprintf(str,_T("config.dat(キー設定)読込み失敗 デフォルトを適用"));
			ADDLIST(str);
		}
		else{
			_stprintf(str,_T("config.dat 読み込みOK"));
			ADDLIST(str);
		}
		if(!g_config.readok2){
			_stprintf(str,_T("config2.datの読込み失敗 デフォルトを適用"));
			ADDLIST(str);
		}
		else{
			_stprintf(str,_T("config2.dat 読み込みOK"));
			ADDLIST(str);
		}
		ADDLIST(_T(" "));
		//パッドの検索結果
		_stprintf(str,_T("■ゲームパッド検索結果"));
		ADDLIST(str);
		_stprintf(str,_T("%d個のゲームパッドが見つかりました"),g_input.jsnum);
		ADDLIST(str);
		if(g_input.jsnum!=0){
			for(int i=0;i<g_input.jsnum;i++){
				_stprintf(str,_T("%d : %s"),i+1,g_input.gamepadname[i]);
				ADDLIST(str);
			}
		}
		ADDLIST(_T(" "));
		//ステージの検索結果
		ADDLIST(_T("■ステージ検索結果"));
		_stprintf(str,_T("全%dステージ"),g_stagelist.GetStageCount());
		ADDLIST(str);
		ADDLIST(_T(" "));
		ADDLIST(_T("(`･ω･´)読み込みに成功したステージ(`･ω･´)"));
		for(i=0;i<g_stagelist.GetStageCount();i++){
			_stprintf(str,_T("%s - %s"),g_stagelist.GetStageDir(i),g_stagelist.GetStageName(i));
			if(g_stagelist.GetStageVer(i)!=0){
				_stprintf(&str[strlen(str)],_T("(ver%4.3f)"),g_stagelist.GetStageVer(i)/1000.0);
			}
			else{
				_stprintf(&str[strlen(str)],_T("(DLLなし)"));
			}
			ADDLIST(str);
		}
		ADDLIST(_T(" "));
		ADDLIST(_T("(´･ω･`)読み込みに失敗したステージ(´･ω･`)"));
		_stprintf(str,_T("全%dステージ"),g_stagelist.GetDameStageCount());
		ADDLIST(str);
		for(i=0;i<g_stagelist.GetDameStageCount();i++){
			_stprintf(str,_T("%s"),g_stagelist.GetDameStageDir(i));
			switch(g_stagelist.GetDameStageReason(i)){
			case CSL_DAME_NONAME:
				_stprintf(&str[strlen(str)],_T("(名前取得に失敗)"));
				break;
			case CSL_DAME_PROC:
				_stprintf(&str[strlen(str)],_T("(関数ポインタ取得に失敗)"));
				break;
			case CSL_DAME_PROC2:
				_stprintf(&str[strlen(str)],_T("(StageInfoに失敗)"));
				break;
			case CSL_DAME_OLDDLL:
				_stprintf(&str[strlen(str)],_T("(DLLが古い-%4.3f)"),g_stagelist.GetDameStageVer(i)/1000.0);
				break;
			case CSL_DAME_NEWDLL:
				_stprintf(&str[strlen(str)],_T("(DLLが新しすぎ-%4.3f)"),g_stagelist.GetDameStageVer(i)/1000.0);
				break;
			}
			ADDLIST(str);
		}
		ADDLIST(_T(" "));

		if(g_stagelist.GetStageCount()==0){//ステージが一個もいない
			ADDLIST(_T(" "));
			ADDLIST(_T("ステージが一つもありません。ゲームを開始できません。"));
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
	メインウインドウがアクティブかどうかを調べる
--------------------------------------------------------------------------------*/
BOOL CSystem::IsWindowActive()
{
	if( GetForegroundWindow()==hwnd )return(TRUE);
	else return FALSE;
}


/*!
*	ビットマップによるテキスト描画
*	フォントによるテキスト描画は重いっぽいので、極力こっちを使うべき？
*	アルファベット大文字(A-Z)と数字と極一部の記号しかサポートしない
*	小文字アルファベットは大文字に変換される
*	サポートする記号  +-%:/
*/
//CSystem::DrawBMPText で使用。文字コードを対応するセル番号に変換
int char2cell(TCHAR c)
{
	if(c<0x20)return -1;//表示できない文字
	if(c==0x20)return 0;//全角スペース
	if(c>0x7E)return -1;//表示できない文字

	return CELL_BMPTEXT21 + (c - 0x21);
}

#define CELLDRAW(x,y,z,id,col) g_draw.CellDraw(sdds,scdat,srdat,id,x,y,z,0,FALSE,FALSE,col);

/*! ビットマップテキスト描画。
*	アルファベット（大文字）、数字、+-%のみ
*/
double CSystem::DrawBMPText(double x,double y,float z,TCHAR *str,DWORD col)
{
	return DrawBMPTextEx(x,y,z,str,col,1.0f,1.0f,SYSBMPTXT_DEFAULT);
}

/*!ビットマップテキスト描画Ex
*	拡大縮小・インチキプロポーショナル・右左テキストが選択できる
*/
double CSystem::DrawBMPTextEx(double x,double y,float z,TCHAR *str,DWORD col,float scaX,float scaY,DWORD flags)
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
	case SYSBMPTXT_SHADE://影つき(1)
		{
			shadecol.red /= 2;
			shadecol.green /= 2;
			shadecol.blue /= 2;
		}break;
	case SYSBMPTXT_SHADE_W://真っ白影つき
		{
			shadecol.red = 255;
			shadecol.green = 255;
			shadecol.blue = 255;
		}break;
	case SYSBMPTXT_SHADE_B://真っ黒影つき
		{
			shadecol.red = 0;
			shadecol.green = 0;
			shadecol.blue = 0;
		}break;
	}
	

	if(flags&SYSBMPTXT_ALPHAADD)g_draw.SetAlphaMode(GBLEND_KASAN);

	//逆向き
	if(flags & SYSBMPTXT_R2L){
		int len = strlen(str);
		TCHAR *tmp = new TCHAR [len+1];
		for(int t=0;t<len;t++){
			tmp[len-t-1] = str[t];
		}
		tmp[len] = '\0';
		str = tmp;
		while(*str!='\0'){
			cno = char2cell(*str);
			if(cno>0){
				//stepを求める
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

	//ふつー
	while(*str!='\0'){
		cno = char2cell(*str);
		if(cno>0){
			//stepを求める
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
	現在のメインタスククラスを取得
--------------------------------------------------------------------*/
CExclusiveTaskBase* CSystem::GetCurrentMainTask()
{
	return taskManager.GetTopExclusiveTask();
}

/*------------------------------------------------------------------
	指定IDのバックグラウンドタスクを取得
--------------------------------------------------------------------*/
CBackgroundTaskBase* CSystem::FindBGTask(DWORD id)
{
	return taskManager.FindBGTask(id);
}

/*------------------------------------------------------------------
	指定IDの通常タスクを取得
--------------------------------------------------------------------*/
CTaskBase* CSystem::FindTask(DWORD id)
{
	return taskManager.FindTask(id);
}

/*------------------------------------------------------------------
	ログ
--------------------------------------------------------------------*/

TCHAR* CSystem::logBuffer = NULL;

void CSystem::Log(const TCHAR *log,DWORD flag)
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
			_stprintf(logBuffer,_T("[info]:%s"),log);
			log2file = g_config.UseLog_Info();
		}
		break;
	case SYSLOG_ERROR	:
		{
			_stprintf(logBuffer,_T("[error]:%s"),log);
			log2file = TRUE;
		}
		break;
	case SYSLOG_ALERT	:
	case SYSLOG_WARNING	:
		{
			_stprintf(logBuffer,_T("[warning]:%s"),log);
			log2file = g_config.UseLog_Warning();
		}
		break;
	case SYSLOG_DEBUG	:
		{
			_stprintf(logBuffer,_T("[debug]:%s"),log);
			log2file = g_config.UseLog_Debug();
		}
		break;
	}
	
	if(log2file){
		g_log2file.AddLog(logBuffer);
		if(flag==SYSLOG_ERROR)g_log2file.Flush();
	}
}

void CSystem::LogWarning(const TCHAR *format, ...)
{
	_tcscpy(logBuffer,_T("[warning]:"));

	va_list args;
	va_start(args, format);
	vsprintf(&logBuffer[strlen(logBuffer)], format, args);
	va_end(args);

	OutputDebugString(logBuffer);
	OutputDebugString(_T("\n"));

	if(g_config.UseLog_Warning())g_log2file.AddLog(logBuffer);
}


void CSystem::LogErr(const TCHAR *format,...)
{
	_tcscpy(logBuffer,_T("[error]:"));

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
	//コピー
	UINT width , height;
	DWORD *bits = g_draw.GetFrontBufferCopyRaw(&width,&height);
	if(!bits){
		Log(_T("スクリーンショットの作成に失敗(フロントバッファーコピー)"),SYSLOG_WARNING);
		return;
	}

	//保存ファイル名をひねり出す
	TCHAR* filename = new TCHAR[256];
	time_t crnt_time;
	time(&crnt_time);
	struct tm* crnt_time_l = localtime(&crnt_time);
	_stprintf(filename,_T("%s\\%d%s%d%s%d%s%d%s%d%s%d.%s"),
		_T("system\\sshot\\"),
		crnt_time_l->tm_year + 1900,				//年
		(crnt_time_l->tm_mon + 1)<10 ? _T("0") : _T(""),
		crnt_time_l->tm_mon + 1,					//月
		crnt_time_l->tm_mday<10 ? _T("0") : _T(""),
		crnt_time_l->tm_mday,						//日
		crnt_time_l->tm_hour<10 ? _T("0") : _T(""),
		crnt_time_l->tm_hour,						//時
		crnt_time_l->tm_min<10 ? _T("0") : _T(""),
		crnt_time_l->tm_min,						//分
		crnt_time_l->tm_sec<10 ? _T("0") : _T(""),
		crnt_time_l->tm_sec,						//秒
		g_config.GetSShotFileTypeStr()				//拡張子
	);

/*	これをやるとgdiplusが必要になる

	CImage img;
	img.Create(width,height,32);
	ARGB col;
	BYTE tmp;
	for(UINT y=0;y<height;y++)
	{
		for(UINT x=0;x<width;x++)
		{
			col.dwcol = *(bits + y*width + x);

			//赤と青が逆らしい
			tmp = col.red;
			col.red = col.blue;
			col.blue = tmp;

			img.SetPixel(x,y,col.dwcol);
		}
	}
	img.Save(filename);//,ImageFormatJPEG);*/

	//ビットマップで保存
	if (g_config.GetSShotImageFormat() == SSHOT_BMP)
	{
		UINT stride = width*3;
		stride += (stride%4) ? (4-stride%4) : 0;

		//ファイルヘッダ
		BITMAPFILEHEADER head;
		head.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		head.bfReserved1 = 0;
		head.bfReserved2 = 0;
		head.bfSize = sizeof(head) + sizeof(BITMAPINFOHEADER) + height * stride;
		head.bfType = 'MB';

		//ビットマップヘッダ
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
		head2.biXPelsPerMeter = 2000;//適当でよい？
		head2.biYPelsPerMeter = 2000;//適当でよい？

		//ファイル開ける
		CFile file;
		if(file.Open(filename,CFile::modeWrite | CFile::modeCreate))
		{
			//ヘッダ書き込み
			file.Write(&head ,sizeof(BITMAPFILEHEADER));
			file.Write(&head2,sizeof(BITMAPINFOHEADER));
			
			#if 1
			{
				//32→24変換用バッファ
				BYTE *cbuf = new BYTE[ stride ];
				ZeroMemory(cbuf,stride);

				if (!cbuf)
				{
					gbl.ods2(_T("CSystem::SaveScreenShot : 変換用バッファがないぽ\n"));
					file.Close();
					CFile::Remove(filename);
					goto SSHOT_FAILED;
				}

				for(UINT y=0;y<height;y++)
				{
					//32→24
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

					//1ライン分書き込み
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
			Log(_T("スクリーンショットの作成に失敗(ファイルオープン)"),SYSLOG_WARNING);
		}
	}
	else if (g_config.GetSShotImageFormat() == SSHOT_PNG)	// PNG保存
	{
		FILE* fp;

		// PNG構造体
		png_structp strPNG = png_create_write_struct(PNG_LIBPNG_VER_STRING, _T("ケホパホの夜"), PngErrHandler, NULL);
		if (!strPNG)
		{
			gbl.ods2(_T("CDirectDraw::Load256PNGbits : PNG構造体ﾃﾞｷﾃﾈｴﾖ!!ヽ(`Д´)ﾉｳﾜｧｧﾝ!!\n"));
			goto SSHOT_FAILED;
		}

		// 情報構造体
		png_infop infoPNG = png_create_info_struct(strPNG);
		if (!infoPNG)
		{
			gbl.ods2(_T("CDirectDraw::Load256PNGbits : PNG情報構造体ﾃﾞｷﾃﾈｴﾖ!!ヽ(`Д´)ﾉｳﾜｧｧﾝ!!\n"));
			png_destroy_write_struct(&strPNG, NULL);
			goto SSHOT_FAILED;
		}

		// 書き込み準備
		fp = fopen(filename, _T("wb"));
		if (!fp)
		{
			png_destroy_write_struct(&strPNG, &infoPNG);
			goto SSHOT_FAILED;
		}

		// ライブラリに任せる
		png_init_io(strPNG, fp);

		// 画像の情報を設定
		png_set_IHDR(strPNG, infoPNG,				// 構造体一式
					 width,							// 幅
					 height,						// 高さ
					 8,								// ビット深度
					 PNG_COLOR_TYPE_RGB,			// カラータイプ
					 PNG_INTERLACE_NONE,			// インタレース
					 PNG_COMPRESSION_TYPE_DEFAULT,	// 圧縮方法？
					 PNG_FILTER_TYPE_DEFAULT);		// たばこのフィルターを抜いて吸うのはやめましょう。

		// ファイルにぶち込む
		png_write_info(strPNG, infoPNG);

		{
			UINT stride = width*3;
			stride += (stride%4) ? (4-stride%4) : 0;

			//32→24変換用バッファ
			BYTE *cbuf = new BYTE[ stride ];

			if (!cbuf)
			{
				gbl.ods2(_T("CSystem::SaveScreenShot : 変換用バッファがないぽ\n"));
				fclose(fp);
				png_destroy_write_struct(&strPNG, &infoPNG);
				goto SSHOT_FAILED;
			}

			ZeroMemory(cbuf,stride);

			for(int y=height - 1;y >= 0;y--)
			{
				//32→24
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

				//1ライン分書き込み
				png_write_row(strPNG, cbuf);
			}

			DELETEARRAY(cbuf);
		}

		// 完了
		png_write_end(strPNG, infoPNG);
		fclose(fp);
		png_destroy_write_struct(&strPNG, &infoPNG);
	}
	else	// JPEG保存
	{
		struct jpeg_compress_struct cinfo;
		struct jpeg_error_mgr jerr;
		FILE* fp;

		// 構造体初期化
		ZeroMemory(&cinfo, sizeof(cinfo));
		ZeroMemory(&jerr, sizeof(jerr));
		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_compress(&cinfo);

		// 書き込み準備
		fp = fopen(filename, _T("wb"));
		if (!fp)
		{
			jpeg_destroy_compress(&cinfo);
			goto SSHOT_FAILED;
		}

		// 合体
		jpeg_stdio_dest(&cinfo, fp);

		// 色々情報設定
		cinfo.image_width = width;
		cinfo.image_height = height;
		cinfo.input_components = 3;
		cinfo.in_color_space = JCS_RGB;
		jpeg_set_defaults(&cinfo);			// 後はライブラリに任せる

		// 画質を設定
		jpeg_set_quality(&cinfo, 70, TRUE);

		// 発射準備
		jpeg_start_compress(&cinfo, TRUE);

		{
			UINT stride = width*3;
			stride += (stride%4) ? (4-stride%4) : 0;

			//32→24変換用バッファ
			BYTE *cbuf = new BYTE[ stride ];

			if (!cbuf)
			{
				gbl.ods2(_T("CSystem::SaveScreenShot : 変換用バッファがないぽ\n"));
				fclose(fp);
				jpeg_destroy_compress(&cinfo);
				goto SSHOT_FAILED;
			}

			// データ伝達に使うポインタ
			JSAMPROW row = (JSAMPROW)cbuf;

			ZeroMemory(cbuf,stride);

			for(int y=height - 1;y >= 0;y--)
			{
				//32→24
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

				//1ライン分書き込み
				jpeg_write_scanlines(&cinfo, &row, 1);
			}

			DELETEARRAY(cbuf);
		}

		// 完了
		jpeg_finish_compress(&cinfo);
		jpeg_destroy_compress(&cinfo);
		fclose(fp);
	}

SSHOT_FAILED:
	delete [] filename;
	delete [] bits;
}

// 設定に従いフレームレートを変更。
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


void CSystem::PushSysTag(const TCHAR* tag_str)
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

void CSystem::PushDLLTag(const TCHAR* tag_str)
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
