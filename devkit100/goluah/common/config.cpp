

#include <d3d9.h>

#include "config.h"

#include <tchar.h>

extern DWORD GetLimcutKey();

CConfig	g_config;

// public =========================================================

CConfig::CConfig()
{
	rawData[0] = rawData[1] = rawData[2] = rawData[3] = 0;
	filepath1=filepath2=NULL;
	ReInitialize();
	limcut_pass = FALSE;
}

CConfig::~CConfig()
{
	SaveConfig2();
}

//*****************************************************************
//  Getなんたら
//*****************************************************************

D3DFORMAT CConfig::TexFormat()
{
	switch(tex_format){
	case CONFIG2_USETEXFMT_A1R5G5B5:return(D3DFMT_A1R5G5B5);
	case CONFIG2_USETEXFMT_A4R4G4B4:return(D3DFMT_A4R4G4B4);
	case CONFIG2_USETEXFMT_A8R3G3B2:return(D3DFMT_A8R3G3B2);
	case CONFIG2_USETEXFMT_A8R8G8B8:return(D3DFMT_A8R8G8B8);
	}
	return(D3DFMT_A1R5G5B5);
}

D3DDEVTYPE CConfig::DeviceType()
{
	switch(device_type){
	case CONFIG2_DEVICE_HAL:return(D3DDEVTYPE_HAL);
	case CONFIG2_DEVICE_REF:return(D3DDEVTYPE_REF);
	case CONFIG2_DEVICE_SW:return(D3DDEVTYPE_SW);
	}
	return(D3DDEVTYPE_HAL);
}

TCHAR* CConfig::GetSShotFileTypeStr()
{
	switch(sshot_format)
	{
	case SSHOT_JPG	:return _T("jpg");
	case SSHOT_PNG	:return _T("png");
	case SSHOT_BMP	:
	default:return _T("bmp");
	}
}

int CConfig::GetGameSpeed2()
{
	switch (g_config.GetGameSpeed()){
	case GSPEED_SLOW:return 40;
	case GSPEED_OLD:return 50;
	case GSPEED_NEW:return 60;
	case GSPEED_30x2:return 60;
		//case GSPEED_25x2	:return 50;
	default:return 50;
	}
}

//*****************************************************************
//  Setなんたら
//*****************************************************************

void CConfig::SetTimeLimit(UINT tl)
{
	if(tl<20)		limit_time=0;
	else if(tl>99)	limit_time=99;
	else			limit_time=tl;
}

void CConfig::SetLimiterCut(BOOL b)
{
#ifndef _DEBUG
	if(limiter_cut_enable)
	{
#endif
		limiter_cut = b ? 1 : 0;
#ifndef _DEBUG
	}
	else limiter_cut = 0; 
#endif
}

void CConfig::SetDeviceType(D3DDEVTYPE dt)
{
	switch(dt){
	case D3DDEVTYPE_HAL:device_type= CONFIG2_DEVICE_HAL;break;
	case D3DDEVTYPE_REF:device_type= CONFIG2_DEVICE_REF;break;
	case D3DDEVTYPE_SW :device_type= CONFIG2_DEVICE_SW ;break;
	}
}

void CConfig::SetTexFormat(D3DFORMAT tf)
{
	switch(tf){
	case D3DFMT_A1R5G5B5:tex_format= CONFIG2_USETEXFMT_A1R5G5B5;break;
	case D3DFMT_A4R4G4B4:tex_format= CONFIG2_USETEXFMT_A4R4G4B4;break;
	case D3DFMT_A8R3G3B2:tex_format= CONFIG2_USETEXFMT_A8R3G3B2;break;
	case D3DFMT_A8R8G8B8:tex_format= CONFIG2_USETEXFMT_A8R8G8B8;break;
	}
}


//*****************************************************************
//  デフォルト設定にする
//*****************************************************************

void CConfig::SetDefaultConfigKB()
{
	//player1 key config
	keycfg.key_up[0]		= VK_UP;
	keycfg.key_down[0]		= VK_DOWN;
	keycfg.key_left[0]		= VK_LEFT;
	keycfg.key_right[0]		= VK_RIGHT;
	keycfg.key_button[0][0]	= 'Z';
	keycfg.key_button[1][0]	= 'X';
	keycfg.key_button[2][0] = 'C';
	keycfg.key_button[3][0] = 'V';
	//player2 key config
	keycfg.key_up[1]		= 'W';
	keycfg.key_down[1]		= 'S';
	keycfg.key_left[1]		= 'A';
	keycfg.key_right[1]		= 'D';
	keycfg.key_button[0][1]	= '1';
	keycfg.key_button[1][1]	= '2';
	keycfg.key_button[2][1] = '3';
	keycfg.key_button[3][1] = '4';
}

void CConfig::SetDefaultConfigPad()
{
	for(int i=0;i<NUM_PAD;i++)
	{
		padcfg.pad_button[0][i] = 0;
		padcfg.pad_button[1][i] = 1;
		padcfg.pad_button[2][i] = 2;
		padcfg.pad_button[3][i] = 3;
	}
}

void CConfig::SetDefaultConfigAss()
{
	inputAssign.kb[0] = 0;
	inputAssign.kb[1] = 1;
	for(int i=0;i<NUM_PAD;i++)
	{
		inputAssign.pad[i]=i;
	}
}

void CConfig::SetDefaultConfig2()
{
	//□描画関係
	SetHalfMode( FALSE );
	SetFullScreen( FALSE );
	SetTexFormat( D3DFMT_A1R5G5B5 );
	SetDeviceType(D3DDEVTYPE_HAL);
	SetHardwereVP( FALSE );
	SetNoAlphaTest( FALSE );

	//□ゲーム設定
	SetDebugMode( FALSE );
	SetShowMovie( TRUE );
	SetShowInfo( TRUE );
	SetTimeLimit( 60 );
	SetMaxPoint( 2 );
	SetDifficulty( DIFF_NORMAL );
	SetStrikerCount( 4 );
	SetLimiterCut( FALSE );
	SetGameSpeed( GSPEED_NEW );
	SetGaugeMode( GAUGE_1220 );

	//□システム設定
	SetUseDInput( TRUE );
	SetUseDSound( TRUE );
	SetUseDShow( TRUE );
	SetUseLog2File( FALSE );
	SetIgnoreExceptions( FALSE );
	SetUseLog2File( TRUE );

	SetUseLog2File( TRUE );
	SetUseLog_Info( FALSE );
	SetUseLog_Warning( FALSE );
	SetUseLog_Debug( FALSE );
	SetSShotImageFormat( SSHOT_PNG );
}

//*****************************************************************
//  設定をファイルから読み込み
//*****************************************************************

void CConfig::ReInitialize()
{
	readok1 = LoadConfig();
	if(!readok1){
		SetDefaultConfigKB();
		SetDefaultConfigPad();
		SetDefaultConfigAss();
	}
	
	readok2 = LoadConfig2();
	if(!readok2)SetDefaultConfig2();
}

BOOL CConfig::LoadConfig()
{
	TCHAR filepath[256];
	if(filepath1!=NULL){
		_tcscpy(filepath,filepath1);
	}
	else _tcscpy(filepath,_T(".\\system\\config.dat"));

	HANDLE hFile = CreateFile(filepath,
		GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		return(FALSE);
	}
	DWORD br;

	//キーボード
	DWORD ret=ReadFile(hFile,&keycfg,sizeof(KEYCONFIG),&br,NULL);
	if(!ret || br!=sizeof(KEYCONFIG)){
		CloseHandle(hFile);
		return(FALSE);
	}

	//パッド
	ret=ReadFile(hFile,&padcfg,sizeof(PADCONFIG),&br,NULL);
	if(!ret || br!=sizeof(PADCONFIG)){
		CloseHandle(hFile);
		return(FALSE);
	}

	//割り当て
	ret=ReadFile(hFile,&inputAssign,sizeof(INPUT2PLAYER),&br,NULL);
	if(!ret || br!=sizeof(INPUT2PLAYER)){
		CloseHandle(hFile);
		return(FALSE);
	}

	CloseHandle(hFile);
	return(TRUE);
}

BOOL CConfig::LoadConfig2()
{	
	TCHAR filepath[256];
	if(filepath2!=NULL){
		_tcscpy(filepath,filepath2);
	}
	else _tcscpy(filepath,_T(".\\system\\config2.dat"));

	HANDLE hFile = CreateFile(filepath,
		GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		return(FALSE);
	}

	DWORD br;
	DWORD ver_ck;

	DWORD ret=ReadFile(hFile,&ver_ck,sizeof(DWORD),&br,NULL);
	if(!ret || br!=sizeof(DWORD)){
		CloseHandle(hFile);
		return(FALSE);
	}

	if( ver_ck!=CONFIG_VERSION ){
		SetFilePointer(hFile, sizeof(rawData), NULL, FILE_CURRENT);
	}
	else{
		ret=ReadFile(hFile,rawData,sizeof(rawData),&br,NULL);
		if(!ret || br!=sizeof(rawData)){
			CloseHandle(hFile);
			return(FALSE);
		}
	}

	DWORD lc_key;
	ret=ReadFile(hFile,&lc_key,sizeof(DWORD),&br,NULL);
	if(!ret || br!=sizeof(DWORD)){
		CloseHandle(hFile);
		return(FALSE);
	}
	CloseHandle(hFile);

	if( ver_ck!=CONFIG_VERSION ){
		SetDefaultConfig2();
	}

	DWORD lc_key2 = GetLimcutKey();
	if( (lc_key^('aki')) == lc_key2 )
	{
		OutputDebugString(_T("キー一致、限界突破許可\n"));
		limcut_pass = TRUE;
		limiter_cut_enable = 1;
	}
	else if( lc_key==lc_key2 )
	{
		OutputDebugString(_T("キー一致\n"));
		limcut_pass = TRUE;
		limiter_cut_enable = 0;
	}
	else
	{
		OutputDebugString(_T("キー不一致\n"));
		limiter_cut = 0;
		limiter_cut_enable = 0;
		limcut_pass = FALSE;
	}

	return TRUE;
}

//*****************************************************************
//  設定をファイルに書き込み
//*****************************************************************

BOOL CConfig::SaveConfig()
{
	TCHAR filepath[256];
	if(filepath1!=NULL){
		_tcscpy(filepath,filepath1);
	}
	else _tcscpy(filepath,_T(".\\system\\config.dat"));

	HANDLE hFile = CreateFile(filepath,
		GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		return FALSE;
	}
	DWORD br;

	//キーボード
	DWORD ret=WriteFile(hFile,&keycfg,sizeof(KEYCONFIG),&br,NULL);
	if(!ret || br!=sizeof(KEYCONFIG)){
		CloseHandle(hFile);
		return FALSE;
	}

	//パッド
	ret=WriteFile(hFile,&padcfg,sizeof(PADCONFIG),&br,NULL);
	if(!ret || br!=sizeof(PADCONFIG)){
		CloseHandle(hFile);
		return FALSE;
	}

	//入力割り当て
	ret=WriteFile(hFile,&inputAssign,sizeof(INPUT2PLAYER),&br,NULL);
	if(!ret || br!=sizeof(INPUT2PLAYER)){
		CloseHandle(hFile);
		return FALSE;
	}

	CloseHandle(hFile);
	return TRUE;
}

BOOL CConfig::SaveConfig2()
{
	TCHAR filepath[256];
	if(filepath2!=NULL){
		_tcscpy(filepath,filepath2);
	}
	else _tcscpy(filepath,_T(".\\system\\config2.dat"));

	HANDLE hFile = CreateFile(filepath,
		GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		return FALSE;
	}
	DWORD br;

	DWORD ver_ck = CONFIG_VERSION;
	DWORD ret=WriteFile(hFile,&ver_ck,sizeof(DWORD),&br,NULL);
	if(!ret || br!=sizeof(DWORD)){
		CloseHandle(hFile);
		return FALSE;
	}

	ret=WriteFile(hFile,rawData,sizeof(rawData),&br,NULL);
	if(!ret || br!=sizeof(rawData)){
		CloseHandle(hFile);
		return FALSE;
	}

	DWORD lc_key = GetLimcutKey();
	if(limiter_cut_enable)
	{
		lc_key ^= ('aki');
	}

	ret=WriteFile(hFile,&lc_key,sizeof(DWORD),&br,NULL);
	if(!ret || br!=sizeof(DWORD)){
		CloseHandle(hFile);
		return FALSE;
	}
	CloseHandle(hFile);
	return TRUE;
}

