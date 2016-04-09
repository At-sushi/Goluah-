
/*!	
*	@file
*	@brief CDirectInput
*
*	キー入力管理クラス。
*	入力のログをとり、ボタンが押されている・いないの判断だけでなく、
*	押しっぱなしの状態なのか、現在フレームで押されたものなのかも判断できる。
*	
*	キーボードの入力とパッド（DirectX8以上のDirectInputを使用）の
*	両方の入力を入力を合成して処理する。
*
*/

#include "stdafx.h"

#include "global.h"	//g_muki[g_muki],g_config が必要
#include "dx_input.h"





#define INPUT_RECOVER_TASK_ID 'iptR'
/*!
*	@brief CDirectInput用イベント取得クラス
*	@ingroup DirectX
*
*	CDirectInputクラスがパッドの復帰処理を行うためにWM_ACTIVATEメッセージを拾う必要がある
*	そのためにバックグラウンドタスクとして登録する
*	復帰の際にグローバルの g_input の RecoverDirectInput() をコールする
*
*	また、毎フレーム g_input の KeyLog() をコールしてキー状態ログを生成させる
*/
class CInputRecoverListener : public CBackgroundTaskBase
{
public:
	DWORD GetID(){return INPUT_RECOVER_TASK_ID;}

	BOOL Execute(DWORD time)//キー状態ログを生成
	{
		g_input.KeyLog();
		return TRUE;
	}

	void WndMessage(HWND hWnd,UINT msg,WPARAM wparam, LPARAM lparam)//!<パッドの復帰処理
	{
		if(msg==WM_ACTIVATE){
			g_input.RecoverDirectInput(wparam,lparam);
		}
	}

	void Initialize(){}
	void Terminate(){}
};


/*!
*	@brief 構築
*/
CDirectInput::CDirectInput()
{
	keylognow = 0;
	jsnum = 0;
	pdi = NULL;
	ZeroMemory( pdidev , sizeof(LPDIRECTINPUTDEVICE8)*DINPUT_MAX_GAMEPAD );
	hwnd = NULL;
	com_grd_enabled = FALSE;
	pdidev_kb = NULL;
	ZeroMemory( &DIKeyState , sizeof(DIKeyState) );
	ZeroMemory( pdieffect , sizeof(LPDIRECTINPUTEFFECT)*DINPUT_MAX_GAMEPAD*3 );
}


/*!
*	@brief 初期化
*
*	@param hwnd メインウインドウのウインドウハンドル。
*				パッドの入力復帰のために必要
*	@return TRUE:成功, FALSE:残念
*/
BOOL CDirectInput::Initialize(HWND hwndg)
{
	CleanDInput();

	hwnd = hwndg;

	//パッド
	jsnum = 0;
	pdi=NULL;
	for(int i=0;i<DINPUT_MAX_GAMEPAD;i++)pdidev[i] = NULL;
	pdidev_kb = NULL;

	ZeroMemory(keylog,sizeof(keylog));//キー入力ログをクリア
	keylognow=0;//キー入力ログ初期位置
	keylocked = FALSE;

	//WM_ACTIVATE取得・毎フレームログ生成タイミング取得 タスク
	g_system.AddTask( new CInputRecoverListener() );

	input_sleep_time = 0;

	return TRUE;
}

/*!
*	@brief 破棄
*/
void CDirectInput::Destroy()
{
	//WM_ACTIVATE取得タスク除去
	g_system.RemoveTask(INPUT_RECOVER_TASK_ID);

	CleanDInput();
	hwnd = NULL;
}


/*!
*	@brief キー状態ログ生成
*
*	1フレ分の入力を処理し、ログの位置を更新する。
*	どの入力をどのプレイヤーに割り当てるかを知るために、グローバルのg_configを参照する。
*/
void CDirectInput::KeyLog()
{
	BYTE KeyState[256];
	PBYTE pkbstate = NULL;

	//ClearKeyLog直後の無効時間
	if(input_sleep_time>0){
		input_sleep_time--;
		return;
	}

	keylognow--;
	if(keylognow<0)keylognow=255;

	if (pdidev_kb)
	{
		// キー所得
		HRESULT ret = pdidev_kb->GetDeviceState(256, KeyState);

		if ( SUCCEEDED(ret) )
			pkbstate = KeyState;
		else if (ret == DIERR_INPUTLOST)
			pdidev_kb->Acquire();
	}

	UINT p;
	for(p=0;p<MAXNUM_KEYI;p++)
	{
		int kb,pad;
		if(g_config.inputAssign.kb[0]==p)kb=0;
		else if(g_config.inputAssign.kb[1]==p)kb=1;
		else kb = -1;
		if(g_config.inputAssign.pad[0]==p)pad=0;
		else if(g_config.inputAssign.pad[1]==p)pad=1;
		else if(g_config.inputAssign.pad[2]==p)pad=2;
		else if(g_config.inputAssign.pad[3]==p)pad=3;
		else if(g_config.inputAssign.pad[4]==p)pad=4;
		else if(g_config.inputAssign.pad[5]==p)pad=5;
		else pad = -1;
		//player1
		keylog[p][keylognow] = KeyLog2(p,kb,pad,pkbstate);
		keylog[p][keylognow+256] = keylog[p][keylognow];

		// ネットキー：前フレームのデータをコピーする
		// 仕組みが不安定なので改良する人は要注意
		SetKeyNet(p, GetKeyNet(p, 1));
	}
}

/*!
*	@brief キー状態ログ生成(1PLAYER分)
*	@sa KeyLog
*/
DWORD CDirectInput::KeyLog2(DWORD cid,int kb,int pad, PBYTE KeyState /* = NULL */)
{
	DWORD keyold=GetKey(cid,1);
	DWORD keystate=0;
	
	DWORD padk = GetPadState(pad);
	BOOL kb_up, kb_down, kb_left, kb_right, kb_b1, kb_b2, kb_b3, kb_b4;

	if (KeyState)
	{
		kb_up		= kb<0 ? FALSE : KeyState[ (BYTE)DIKeyState.key_up[kb] ] & 0x80;
		kb_down		= kb<0 ? FALSE : KeyState[ (BYTE)DIKeyState.key_down[kb] ] & 0x80;
		kb_left		= kb<0 ? FALSE : KeyState[ (BYTE)DIKeyState.key_left[kb] ] & 0x80;
		kb_right	= kb<0 ? FALSE : KeyState[ (BYTE)DIKeyState.key_right[kb] ] & 0x80;
		kb_b1		= kb<0 ? FALSE : KeyState[ (BYTE)DIKeyState.key_button[0][kb] ] & 0x80;
		kb_b2		= kb<0 ? FALSE : KeyState[ (BYTE)DIKeyState.key_button[1][kb] ] & 0x80;
		kb_b3		= kb<0 ? FALSE : KeyState[ (BYTE)DIKeyState.key_button[2][kb] ] & 0x80;
		kb_b4		= kb<0 ? FALSE : KeyState[ (BYTE)DIKeyState.key_button[3][kb] ] & 0x80;
	}
	else
	{
		kb_up		= kb<0 ? FALSE : (GetKeyState(g_config.keycfg.key_up[kb]) & 0x8000);
		kb_down		= kb<0 ? FALSE : (GetKeyState(g_config.keycfg.key_down[kb]) & 0x8000);
		kb_left		= kb<0 ? FALSE : (GetKeyState(g_config.keycfg.key_left[kb]) & 0x8000);
		kb_right	= kb<0 ? FALSE : (GetKeyState(g_config.keycfg.key_right[kb]) & 0x8000);
		kb_b1		= kb<0 ? FALSE : (GetKeyState(g_config.keycfg.key_button[0][kb]) & 0x8000);
		kb_b2		= kb<0 ? FALSE : (GetKeyState(g_config.keycfg.key_button[1][kb]) & 0x8000);
		kb_b3		= kb<0 ? FALSE : (GetKeyState(g_config.keycfg.key_button[2][kb]) & 0x8000);
		kb_b4		= kb<0 ? FALSE : (GetKeyState(g_config.keycfg.key_button[3][kb]) & 0x8000);
	}

	if(g_system.IsWindowActive()){
		//up*down
		if(kb_up || (padk & PADUP)){//VK_UP
			keystate |= KEYSTA_UP;
			if((keyold & KEYSTA_UP));
			else keystate |= KEYSTA_UP2;
		}
		else if(kb_down || (padk & PADDOWN)){//VK_DOWN
			keystate |= KEYSTA_DOWN;
			if(!(keyold & KEYSTA_DOWN))keystate |= KEYSTA_DOWN2;
		}
		//left*right
		if(kb_left || (padk & PADLEFT)){//VK_LEFT
			keystate |= KEYSTA_ALEFT;
			if(!(keyold & KEYSTA_ALEFT))keystate |= KEYSTA_ALEFT2;
		}
		else if(kb_right || (padk & PADRIGHT)){//VK_RIGHT
			keystate |= KEYSTA_ARIGHT;
			if(!(keyold & KEYSTA_ARIGHT))keystate |= KEYSTA_ARIGHT2;
		}
		if(g_muki[cid]){
			if(keystate & KEYSTA_ALEFT  )keystate |= KEYSTA_RIGHT;
			if(keystate & KEYSTA_ALEFT2 )keystate |= KEYSTA_RIGHT2;
			if(keystate & KEYSTA_ARIGHT )keystate |= KEYSTA_LEFT;
			if(keystate & KEYSTA_ARIGHT2)keystate |= KEYSTA_LEFT2;
		}
		else{
			if(keystate & KEYSTA_ALEFT  )keystate |= KEYSTA_LEFT;
			if(keystate & KEYSTA_ALEFT2 )keystate |= KEYSTA_LEFT2;
			if(keystate & KEYSTA_ARIGHT )keystate |= KEYSTA_RIGHT;
			if(keystate & KEYSTA_ARIGHT2)keystate |= KEYSTA_RIGHT2;
		}
		//button
		if(kb_b1 || (padk&PADA)){//button-a
			keystate |= KEYSTA_BA;
			if(!(keyold & KEYSTA_BA))keystate |= KEYSTA_BA2;
		}
		if(kb_b2 || (padk&PADB)){//button-b
			keystate |= KEYSTA_BB;
			if(!(keyold & KEYSTA_BB))keystate |= KEYSTA_BB2;
		}
		if(kb_b3 || (padk&PADC)){//button-c
			keystate |= KEYSTA_BC;
			if(!(keyold & KEYSTA_BC))keystate |= KEYSTA_BC2;
		}
		if(kb_b4 || (padk&PADD)){//button-d
			keystate |= KEYSTA_BD;
			if(!(keyold & KEYSTA_BD))keystate |= KEYSTA_BD2;
		}
	}
	
	return(keystate);
}



/*!
*	@brief キー入力ログをクリア
*
*	キー入力ログのバッファーをゼロクリアする。
*	タスク切り替わり直後の誤動作を防ごうと思ったけど、かえってそれがよくなかった。
*	(おしっぱなしであっても、この直後に「押した瞬間」フラグが立つ)
*	今はその対策を講じてみた。
*/
void CDirectInput::ClearKeyLog()
{
	DWORD prvkey;

	for(UINT i=0;i<MAXNUM_KEYI;i++)
	{
		prvkey = keylog[i][keylognow];
		prvkey &= ~(KEYSTA_UP2|KEYSTA_DOWN2|KEYSTA_ALEFT2|KEYSTA_ARIGHT2|KEYSTA_ARIGHT2|KEYSTA_BUTTONS|
					KEYSTA_FOWORD2|KEYSTA_BACK2);

		ZeroMemory(keylog[i],sizeof(DWORD)*(256+256));
		keylognow = 0;

		keylog[i][keylognow] = prvkey;
		keylog[i][keylognow+256] = prvkey;
	}

	input_sleep_time = 20;
}

/*!
*	@brief （外部からの）キー状態の取得
*
*	この関数はKeyLock と COMガードキーを考慮する。
*	生の入力を直接取りたいときはGetKeyを利用する。
*/
DWORD CDirectInput::GetKeyEx(
							DWORD cid,	//!< キー入力インデックス(PLAYER)
							DWORD ofst)	//!< 何フレ前の入力を取得するか
{
	CExclusiveTaskBase* bt = g_system.GetCurrentMainTask();

	if(keylocked)return(0);
	if(com_grd_enabled){
		return com_grd_key;
	}

	// 暫定設置
	if (bt && bt->GetID() == 'BtlN' && g_play.IsHost())
		return (GetKeyNet(cid, ofst));

	return(GetKey(cid,ofst));
}

DWORD CDirectInput::SetKeyNet(DWORD index,DWORD key)
{
	DWORD keyold = GetKeyNet(index, 1);

	// 情報落ちしてるかも知れないから**2フラグを付ける
	if (key & KEYSTA_UP && !(keyold & KEYSTA_UP))
		key |= KEYSTA_UP2;
	if (key & KEYSTA_DOWN && !(keyold & KEYSTA_DOWN))
		key |= KEYSTA_DOWN2;
	if (key & KEYSTA_FOWORD && !(keyold & KEYSTA_FOWORD))
		key |= KEYSTA_FOWORD2;
	if (key & KEYSTA_BACK && !(keyold & KEYSTA_BACK))
		key |= KEYSTA_BACK2;
	if (key & KEYSTA_BA && !(keyold & KEYSTA_BA))
		key |= KEYSTA_BA2;
	if (key & KEYSTA_BB && !(keyold & KEYSTA_BB))
		key |= KEYSTA_BB2;
	if (key & KEYSTA_BC && !(keyold & KEYSTA_BC))
		key |= KEYSTA_BC2;
	if (key & KEYSTA_BD && !(keyold & KEYSTA_BD))
		key |= KEYSTA_BD2;

	return netkey[index][keylognow] = key;
}

/*!
*	@brief 生のキー入力を取得する
*	@sa GetKeyEx
*/
DWORD CDirectInput::GetKey(DWORD cid,DWORD ofst)
{
	if(ofst>255)return(0);
	if(cid>=MAXNUM_KEYI)return(0);

	return(keylog[cid][keylognow+ofst]);
}

/*!
*	@brief 生のキー入力を取得する
*	@sa GetKeyEx
*/
DWORD CDirectInput::GetKeyNet(DWORD cid,DWORD ofst)
{
	if(ofst>255)return(0);
	if(cid>=MAXNUM_KEYI)return(0);

	return(netkey[cid][keylognow+ofst]);
}

/*!
*	@brief 指定キー入力をログの中から探す
*	@return 0以上：見つかった位置, マイナス：見つからなかった
*/
int CDirectInput::SeekKeyEx(DWORD cid,		//!< 入力インデックス(PLAYER)
							int offset,		//!< 検索開始位置
							int num_seek,	//!< 検索終了位置
							DWORD key		//!< 検索キー
							)
{
	CExclusiveTaskBase* bt = g_system.GetCurrentMainTask();

	if(keylocked)return(-1);
	if(com_grd_enabled)return(-1);

	// 暫定設置
	if (bt && bt->GetID() == 'BtlN' && g_play.IsHost())
		return (SeekKeyNet(cid, offset, num_seek, key));

	return(SeekKey(cid,offset,num_seek,key));
}

/*!
*	@brief 指定キー入力をログの中から探す
*	@sa SeekKeyEx
*/
int CDirectInput::SeekKey(DWORD cid,int offset,int num_seek,DWORD key)
{
	for(int i=offset;i<num_seek;i++){
		if((GetKey(cid,i)&0x00003333) == key){
			return(i);
		}
	}
	return(-1);
}

/*!
*	@brief 指定キー入力をログの中から探す
*	@sa SeekKeyEx
*/
int CDirectInput::SeekKeyNet(DWORD cid,int offset,int num_seek,DWORD key)
{
	for(int i=offset;i<num_seek;i++){
		if((GetKeyNet(cid,i)& ((i == 0) ? 0x00001111 : 0x00003333)) == key){	// 暫定措置（自動ダッシュよけ）
			return(i);
		}
	}
	return(-1);
}

void CDirectInput::KeyLock(BOOL lk)
{
	keylocked=lk;
}

DWORD CDirectInput::GetAllKey()
{
	DWORD ret =0;
	for(int i=0;i<DINPUT_MAX_GAMEPAD;i++){
		ret |= GetKey(i,0);
	}
	return ret;
}

//********************************************************************
// Direct Input 初期化
//********************************************************************

BOOL CDirectInput::InitializePad()
{
	if(!g_config.UseDInput()){
		CSystem::Log(_T("DirectInput(パッド) 非使用"),SYSLOG_INFO);
		return TRUE;
	}

	if(hwnd==NULL)return FALSE;

	HRESULT res;
	DIPROPDWORD dip;
	ZeroMemory(&dip,sizeof(DIPROPDWORD));
	dip.diph.dwSize = sizeof(DIPROPDWORD);
	dip.diph.dwHeaderSize = sizeof(dip.diph);
	dip.diph.dwObj = 0;
	dip.diph.dwHow = DIPH_DEVICE;
	dip.dwData = DIPROPAXISMODE_ABS;

	DWORD jsnum_detected=0;
	BOOL isOK;

	//DirectInput8オブジェクトの作成
	if(DI_OK != DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&pdi,NULL)){
		if(IDYES == MessageBox(hwnd,_T("ゲームパッドが使用できません。\n続行しますか？"),
			_T("DirectInput8Create 失敗"),MB_YESNO))
			return(TRUE);
		return(FALSE);
	}
	//デバイスの列挙・作成
	res = pdi->EnumDevices(DI8DEVCLASS_GAMECTRL,EnumGamePad,(LPVOID)&jsnum_detected,DIEDFL_ATTACHEDONLY);
	if(res!=DI_OK){
		g_system.Log(_T("▲InitDirectInput:EnumDeviceに失敗\n"),SYSLOG_ERROR);
		isOK=FALSE;
	}

	//デバイスの設定
	for(DWORD i=0;i<jsnum_detected;){
		isOK = TRUE;

		if(DI_OK!=pdidev[i]->SetDataFormat(&c_dfDIJoystick)){//データフォーマット
			g_system.Log(_T("▲InitDirectInput:データフォーマット設定に失敗\n"),SYSLOG_ERROR);
			isOK=FALSE;
		}
		if(DI_OK!=pdidev[i]->SetCooperativeLevel(hwnd,DISCL_EXCLUSIVE | DISCL_FOREGROUND)){//協調
			g_system.Log(_T("▲InitDirectInput:協調レベル設定に失敗\n"),SYSLOG_ERROR);
			isOK=FALSE;
		}
		if(DI_OK!=pdidev[i]->SetProperty(DIPROP_AXISMODE,&dip.diph)){//軸モードの設定
			g_system.Log(_T("▲InitDirectInput:軸モード設定に失敗\n"),SYSLOG_ERROR);
			isOK=FALSE;
		}
		//軸を列挙して値の範囲を設定する
		if(DI_OK!=pdidev[i]->EnumObjects(EnumAxis,pdidev[i],DIDFT_AXIS)){
			g_system.Log(_T("▲InitDirectInput:軸値範囲設定に失敗\n"),SYSLOG_ERROR);
			isOK=FALSE;
		}
		if(!isOK){
			//何かの設定に失敗したらそれは開放してしまう
			RELEASE(pdidev[i]);
			if(i!=jsnum_detected-1){
				pdidev[i] = pdidev[i+1];
				jsnum_detected--;
			}
		}
		else {
			pdidev[i]->Acquire();
			i++;
		}
	}

	// キーボードデバイス所得
	if ( SUCCEEDED(pdi->CreateDevice(GUID_SysKeyboard, &pdidev_kb, NULL)) )
	{
		isOK = TRUE;

		if ( FAILED(pdidev_kb->SetDataFormat(&c_dfDIKeyboard)) )
		{
			g_system.Log(_T("▲InitDirectInput:データフォーマット設定に失敗\n"),SYSLOG_ERROR);
			isOK=FALSE;
		}

		if ( FAILED(pdidev_kb->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)) )
		{
			g_system.Log(_T("▲InitDirectInput:協調レベル設定に失敗\n"),SYSLOG_ERROR);
			isOK=FALSE;
		}

		if (isOK)
		{
			// キー設定を変換
			for (int i = 0; i < 2; i++)
			{
				DIKeyState.key_up[i] = VirtKeyToDInputKey(g_config.keycfg.key_up[i]);
				DIKeyState.key_down[i] = VirtKeyToDInputKey(g_config.keycfg.key_down[i]);
				DIKeyState.key_left[i] = VirtKeyToDInputKey(g_config.keycfg.key_left[i]);
				DIKeyState.key_right[i] = VirtKeyToDInputKey(g_config.keycfg.key_right[i]);
				for (int j = 0; j < 4; j++)
					DIKeyState.key_button[j][i] = VirtKeyToDInputKey(g_config.keycfg.key_button[j][i]);
			}

			pdidev_kb->Acquire();
		}
		else
			RELEASE(pdidev_kb);
	}

	InitializeEffects();
	jsnum=jsnum_detected;
	return(TRUE);
}

//ゲームパッド列挙コールバック関数
BOOL FAR CALLBACK CDirectInput::EnumGamePad(LPCDIDEVICEINSTANCE lpddi,LPVOID pvref)
{
/*	typedef struct _GUID
	{
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
	} GUID;*/

	static std::vector<GUID> guid_list;
	for(UINT i=0;i<guid_list.size();i++)
	{
		if( guid_list[i] == lpddi->guidInstance )return DIENUM_CONTINUE;
	}
	guid_list.push_back(lpddi->guidInstance);

	gbl.ods(_T("Pad %s / %s , %X-%X-%X-(%X-%X-%X-%X-%X-%X-%X-%X) , %X-%X-%X-(%X-%X-%X-%X-%X-%X-%X-%X)"),
		lpddi->tszProductName,lpddi->tszInstanceName,
		lpddi->guidInstance.Data1 , lpddi->guidInstance.Data2 , lpddi->guidInstance.Data3 , 
		lpddi->guidInstance.Data4[0] , lpddi->guidInstance.Data4[1] , lpddi->guidInstance.Data4[2] , lpddi->guidInstance.Data4[3],
		lpddi->guidInstance.Data4[4] , lpddi->guidInstance.Data4[5] , lpddi->guidInstance.Data4[6] , lpddi->guidInstance.Data4[7],
		lpddi->guidProduct.Data4[0] , lpddi->guidProduct.Data4[1] , lpddi->guidProduct.Data4[2] , lpddi->guidProduct.Data4[3],
		lpddi->guidProduct.Data4[4] , lpddi->guidProduct.Data4[5] , lpddi->guidProduct.Data4[6] , lpddi->guidProduct.Data4[7]);

	DWORD num = *(DWORD*)pvref;

	if( DI_OK != g_input.pdi->CreateDevice(lpddi->guidInstance,&(g_input.pdidev[num]),NULL) )
		return(DIENUM_CONTINUE);
	else{
		strcpy(g_input.gamepadname[num],lpddi->tszInstanceName);//名前を保存しておく
		(*(DWORD*)pvref)++;
	}

	if(*(DWORD*)pvref >= DINPUT_MAX_GAMEPAD){
		return(DIENUM_STOP);//必要な分見つかったらおしまい
	}

	return(DIENUM_CONTINUE);
}

//軸の列挙・値範囲設定
BOOL FAR CALLBACK CDirectInput::EnumAxis(LPCDIDEVICEOBJECTINSTANCE lpddoi,LPVOID pvref)
{
	DIPROPRANGE dip;
	ZeroMemory(&dip,sizeof(DIPROPRANGE));
	dip.diph.dwSize = sizeof(dip);
	dip.diph.dwHeaderSize = sizeof(dip.diph);
	dip.diph.dwObj = lpddoi->dwType;
	dip.diph.dwHow = DIPH_BYID;
	dip.lMax = +1000;
	dip.lMin = -1000;

	((LPDIRECTINPUTDEVICE)pvref)->SetProperty(DIPROP_RANGE,&dip.diph);
	return(DIENUM_CONTINUE);
}

//********************************************************************
// Direct Input 後始末
// *******************************************************************
void CDirectInput::CleanDInput()
{
	for(int i=0;i<DINPUT_MAX_GAMEPAD;i++){
		for (int j = 0; j < 3; j++)
		{
			if (pdieffect[i][j])
			{
				pdieffect[i][j]->Unload();
				RELEASE(pdieffect[i][j]);
			}
		}

		if(pdidev[i])pdidev[i]->Unacquire();
		RELEASE(pdidev[i]);
	}
	if (pdidev_kb) pdidev_kb->Unacquire();
	RELEASE(pdidev_kb);
	RELEASE(pdi);
}

//***************************************************************************
//  パッドデータの取得
//*****************************************************************************
DWORD CDirectInput::GetPadState(int cid)
{
	HRESULT ret;
	DWORD padstate=0;

	if(cid > DINPUT_MAX_GAMEPAD || cid<0)return(0);

	DIJOYSTATE dijs;
	int i;
	if( pdidev[cid] !=NULL ){
		pdidev[cid]->Poll();
		ret = pdidev[cid]->GetDeviceState(sizeof(DIJOYSTATE),&dijs);
		if(ret == DI_OK){
			if( dijs.lX < -500 ) padstate |= PADLEFT;
			else if( dijs.lX > 500 ) padstate |= PADRIGHT;
			if( dijs.lY < -500 ) padstate |= PADUP;
			else if( dijs.lY > 500 ) padstate |= PADDOWN;
			
			for(i=0;i<12;i++){
				if(dijs.rgbButtons[i] & 0x80){
					if( i==g_config.padcfg.pad_button[0][cid] ) padstate |= PADA;
					else if( i==g_config.padcfg.pad_button[1][cid] ) padstate |= PADB;
					else if( i==g_config.padcfg.pad_button[2][cid] ) padstate |= PADC;
					else if( i==g_config.padcfg.pad_button[3][cid] ) padstate |= PADD;
					/*else if( i==4 ) SendMessage(hwnd, WM_KEYDOWN, VK_F7, 0L);
					else if( i==5 ) SendMessage(hwnd, WM_KEYDOWN, VK_F8, 0L);*/
				}
			}
		}
		else if(ret == DIERR_INPUTLOST){
			pdidev[cid]->Acquire();
		}
	}
	return(padstate);
}

//********************************************************************
//  WM_ACTIVATE　復帰処理
// *******************************************************************
void CDirectInput::RecoverDirectInput(WPARAM wParam,LPARAM lParam)
{
	int i;

	if(pdi != NULL){
		if(wParam == WA_INACTIVE){//非アクティヴ状態
			for(i=0;i<DINPUT_MAX_GAMEPAD;i++){
				if( pdidev[i] != NULL ){
					pdidev[i]->Unacquire();
				}
			}
			pdidev_kb->Unacquire();
		}
		else{//アクティヴ状態
			for(i=0;i<DINPUT_MAX_GAMEPAD;i++){
				if( pdidev[i] != NULL ){
					pdidev[i]->Acquire();
				}
			}
			pdidev_kb->Acquire();
		}
	}
}

//********************************************************************
// 仮想キーをDirectInputキーに変換
//********************************************************************
BYTE CDirectInput::VirtKeyToDInputKey(int vKey)
{
	// 数字キー（１～９）
	if (vKey >= '1' && vKey <= '9')
		return vKey - '1' + DIK_1;

	// ファンクションキー（Ｆ１～Ｆ１０）
	if (vKey >= VK_F1 && vKey <= VK_F10)
		return vKey - VK_F1 + DIK_F1;

	// ファンクションキー（Ｆ１１～Ｆ１５）
	if (vKey >= VK_F11 && vKey <= VK_F15)
		return vKey - VK_F11 + DIK_F11;

	// テンキー（１～３）
	if (vKey >= VK_NUMPAD1 && vKey <= VK_NUMPAD3)
		return vKey - VK_NUMPAD1 + DIK_NUMPAD1;

	// テンキー（４～６）
	if (vKey >= VK_NUMPAD4 && vKey <= VK_NUMPAD6)
		return vKey - VK_NUMPAD4 + DIK_NUMPAD4;

	// テンキー（７～９）
	if (vKey >= VK_NUMPAD7 && vKey <= VK_NUMPAD9)
		return vKey - VK_NUMPAD7 + DIK_NUMPAD7;

	// ヴァラヴァラのやつをさばく
	switch (vKey)
	{
	case VK_ESCAPE:
		return DIK_ESCAPE;

	case '0':
		return DIK_0;

	case VK_OEM_MINUS:
		return DIK_MINUS;

/*	case VK_OEM_NEC_EQUAL:
		return DIK_EQUALS;*/

	case VK_BACK:
		return DIK_BACK;

	case VK_TAB:
		return DIK_TAB;

	case VK_OEM_4:
		return DIK_LBRACKET;

	case VK_OEM_6:
		return DIK_RBRACKET;

	case VK_RETURN:
		return DIK_RETURN;

	case VK_NUMPAD0:
		return DIK_NUMPAD0;

	case VK_LCONTROL:
		return DIK_LCONTROL;

	case VK_RCONTROL:
		return DIK_RCONTROL;

	case VK_OEM_1:
		return DIK_SEMICOLON;

	case VK_OEM_PLUS:
		return DIK_COLON;

	case VK_OEM_7:
		return DIK_APOSTROPHE;

	case VK_OEM_3:
		return DIK_GRAVE;

	case VK_LSHIFT:
		return DIK_LSHIFT;

	case VK_OEM_5:
		return DIK_BACKSLASH;

	case VK_OEM_COMMA:
		return DIK_COMMA;

	case VK_OEM_PERIOD:
		return DIK_PERIOD;

	case VK_OEM_2:
		return DIK_SLASH;

	case VK_RSHIFT:
		return DIK_RSHIFT;

	case VK_LMENU:
		return DIK_LMENU;

	case VK_RMENU:
		return DIK_RMENU;

	case VK_SPACE:
		return DIK_SPACE;

	case VK_CAPITAL:
		return DIK_CAPITAL;

	case VK_NUMLOCK:
		return DIK_NUMLOCK;

	case VK_SCROLL:
		return DIK_SCROLL;

	case VK_SUBTRACT:
		return DIK_SUBTRACT;

	case VK_ADD:
		return DIK_ADD;

	case VK_DECIMAL:
		return DIK_DECIMAL;

	case VK_OEM_102:
		return DIK_OEM_102;

	case VK_UP:
		return DIK_UP;

	case VK_DOWN:
		return DIK_DOWN;

	case VK_LEFT:
		return DIK_LEFT;

	case VK_RIGHT:
		return DIK_RIGHT;

	case VK_KANA:
		return DIK_KANA;

	case VK_KANJI:
		return DIK_KANJI;

	case VK_CONVERT:
		return DIK_CONVERT;

	case VK_NONCONVERT:
		return DIK_NOCONVERT;

	case VK_OEM_NEC_EQUAL:
		return DIK_NUMPADEQUALS;

	case VK_OEM_AX:
		return DIK_AX;

	case VK_PAUSE:
		return DIK_PAUSE;

	case VK_HOME:
		return DIK_HOME;

	case VK_PRIOR:
		return DIK_PRIOR;

	case VK_END:
		return DIK_END;

	case VK_NEXT:
		return DIK_NEXT;

	case VK_INSERT:
		return DIK_INSERT;

	case VK_DELETE:
		return DIK_DELETE;

	case VK_APPS:
		return DIK_APPS;

	case VK_SLEEP:
		return DIK_SLEEP;

	case VK_LWIN:
		return DIK_LWIN;

	case VK_RWIN:
		return DIK_RWIN;

	case VK_SEPARATOR:
		return DIK_NUMPADENTER;

	// 以下、A-Z

	case 'Q':
		return DIK_Q;

	case 'W':
		return DIK_W;

	case 'E':
		return DIK_E;

	case 'R':
		return DIK_R;

	case 'T':
		return DIK_T;

	case 'Y':
		return DIK_Y;

	case 'U':
		return DIK_U;

	case 'I':
		return DIK_I;

	case 'O':
		return DIK_O;

	case 'P':
		return DIK_P;

	case 'A':
		return DIK_A;

	case 'S':
		return DIK_S;

	case 'D':
		return DIK_D;

	case 'F':
		return DIK_F;

	case 'G':
		return DIK_G;

	case 'H':
		return DIK_H;

	case 'J':
		return DIK_J;

	case 'K':
		return DIK_K;

	case 'L':
		return DIK_L;

	case 'Z':
		return DIK_Z;

	case 'X':
		return DIK_X;

	case 'C':
		return DIK_C;

	case 'V':
		return DIK_V;

	case 'B':
		return DIK_B;

	case 'N':
		return DIK_N;

	case 'M':
		return DIK_M;
	}

	return vKey;	// 無かったのでとりあえずそのまま送ってみる（おい）。
}

void CDirectInput::InitializeEffects()
{
	enum { DE_AXES = 1 };
	static DWORD Axes[DE_AXES] = { DIJOFS_Y };
	static LONG Direction[DE_AXES] = { -1 };

	DIEFFECT diEffect;
	DICONSTANTFORCE diConstantForce;
	static DIENVELOPE diEnvelope;

	diEnvelope.dwSize = sizeof(diEnvelope);

	// ('A`)ﾏﾝﾄﾞｸｾ
	ZeroMemory(&diEffect, sizeof(diEffect));
	diEffect.dwSize = sizeof(diEffect);
	diEffect.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
	diEffect.dwSamplePeriod = (DWORD)(1.00 * DI_SECONDS);
	diEffect.dwTriggerButton = DIEB_NOTRIGGER;
	diEffect.cAxes = DE_AXES;
	diEffect.dwGain = DI_FFNOMINALMAX;
	diEffect.rgdwAxes = Axes;
	diEffect.rglDirection = Direction;
	diEffect.lpEnvelope = &diEnvelope;
	diEffect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
	diEffect.lpvTypeSpecificParams = &diConstantForce;
	diEffect.dwStartDelay = 0;

	for (int i = 0; i < DINPUT_MAX_GAMEPAD; i++)
	{
		if (pdidev[i])
		{
			diConstantForce.lMagnitude = 3000;
			diEnvelope.dwAttackLevel = 4000;
			diEnvelope.dwAttackTime = (DWORD)(0.05 * DI_SECONDS);
			diEnvelope.dwFadeLevel = 2000;
			diEnvelope.dwFadeTime = (DWORD)(0.05 * DI_SECONDS);
			diEffect.dwDuration = (DWORD)(0.0625 * DI_SECONDS * pow(2.0, i));

			// 作っとこ。
			pdidev[i]->CreateEffect(GUID_ConstantForce, &diEffect, &pdieffect[i][0], NULL);
			pdidev[i]->CreateEffect(GUID_ConstantForce, &diEffect, &pdieffect[i][1], NULL);
			pdidev[i]->CreateEffect(GUID_ConstantForce, &diEffect, &pdieffect[i][2], NULL);
		}
	}
}

// フォースの力だ。
void CDirectInput::StartForce(DWORD cid, DWORD num)
{
	if(num>=3) return;
	if(cid>=MAXNUM_KEYI) return;
	DICONSTANTFORCE diConstantForce;

	if (pdieffect[ g_config.inputAssign.pad[cid] ][num])
	{
		DIEFFECT diEffect;

		ZeroMemory( &diEffect, sizeof(diEffect) );
		diEffect.dwSize                = sizeof(DIEFFECT);
		diEffect.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
		diEffect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
		diEffect.lpvTypeSpecificParams = &diConstantForce;
		diEffect.dwStartDelay = 0;
		diConstantForce.lMagnitude = 3000;

		pdieffect[ g_config.inputAssign.pad[cid] ][num]->SetParameters(&diEffect, DIEP_TYPESPECIFICPARAMS | DIEP_START);
	}
}



