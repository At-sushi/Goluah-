
/*!	
*	@file
*	@brief CDirectInput
*
*	�L�[���͊Ǘ��N���X�B
*	���͂̃��O���Ƃ�A�{�^����������Ă���E���Ȃ��̔��f�����łȂ��A
*	�������ςȂ��̏�ԂȂ̂��A���݃t���[���ŉ����ꂽ���̂Ȃ̂������f�ł���B
*	
*	�L�[�{�[�h�̓��͂ƃp�b�h�iDirectX8�ȏ��DirectInput���g�p�j��
*	�����̓��͂���͂��������ď�������B
*
*/

#include "stdafx.h"

#include "global.h"	//g_muki[g_muki],g_config ���K�v
#include "dx_input.h"





#define INPUT_RECOVER_TASK_ID 'iptR'
/*!
*	@brief CDirectInput�p�C�x���g�擾�N���X
*	@ingroup DirectX
*
*	CDirectInput�N���X���p�b�h�̕��A�������s�����߂�WM_ACTIVATE���b�Z�[�W���E���K�v������
*	���̂��߂Ƀo�b�N�O���E���h�^�X�N�Ƃ��ēo�^����
*	���A�̍ۂɃO���[�o���� g_input �� RecoverDirectInput() ���R�[������
*
*	�܂��A���t���[�� g_input �� KeyLog() ���R�[�����ăL�[��ԃ��O�𐶐�������
*/
class CInputRecoverListener : public CBackgroundTaskBase
{
public:
	DWORD GetID(){return INPUT_RECOVER_TASK_ID;}

	BOOL Execute(DWORD time)//�L�[��ԃ��O�𐶐�
	{
		g_input.KeyLog();
		return TRUE;
	}

	void WndMessage(HWND hWnd,UINT msg,WPARAM wparam, LPARAM lparam)//!<�p�b�h�̕��A����
	{
		if(msg==WM_ACTIVATE){
			g_input.RecoverDirectInput(wparam,lparam);
		}
	}

	void Initialize(){}
	void Terminate(){}
};


/*!
*	@brief �\�z
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
*	@brief ������
*
*	@param hwnd ���C���E�C���h�E�̃E�C���h�E�n���h���B
*				�p�b�h�̓��͕��A�̂��߂ɕK�v
*	@return TRUE:����, FALSE:�c�O
*/
BOOL CDirectInput::Initialize(HWND hwndg)
{
	CleanDInput();

	hwnd = hwndg;

	//�p�b�h
	jsnum = 0;
	pdi=NULL;
	for(int i=0;i<DINPUT_MAX_GAMEPAD;i++)pdidev[i] = NULL;
	pdidev_kb = NULL;

	ZeroMemory(keylog,sizeof(keylog));//�L�[���̓��O���N���A
	keylognow=0;//�L�[���̓��O�����ʒu
	keylocked = FALSE;

	//WM_ACTIVATE�擾�E���t���[�����O�����^�C�~���O�擾 �^�X�N
	g_system.AddTask( new CInputRecoverListener() );

	input_sleep_time = 0;

	return TRUE;
}

/*!
*	@brief �j��
*/
void CDirectInput::Destroy()
{
	//WM_ACTIVATE�擾�^�X�N����
	g_system.RemoveTask(INPUT_RECOVER_TASK_ID);

	CleanDInput();
	hwnd = NULL;
}


/*!
*	@brief �L�[��ԃ��O����
*
*	1�t�����̓��͂��������A���O�̈ʒu���X�V����B
*	�ǂ̓��͂��ǂ̃v���C���[�Ɋ��蓖�Ă邩��m�邽�߂ɁA�O���[�o����g_config���Q�Ƃ���B
*/
void CDirectInput::KeyLog()
{
	BYTE KeyState[256];
	PBYTE pkbstate = NULL;

	//ClearKeyLog����̖�������
	if(input_sleep_time>0){
		input_sleep_time--;
		return;
	}

	keylognow--;
	if(keylognow<0)keylognow=255;

	if (pdidev_kb)
	{
		// �L�[����
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

		// �l�b�g�L�[�F�O�t���[���̃f�[�^���R�s�[����
		// �d�g�݂��s����Ȃ̂ŉ��ǂ���l�͗v����
		SetKeyNet(p, GetKeyNet(p, 1));
	}
}

/*!
*	@brief �L�[��ԃ��O����(1PLAYER��)
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
*	@brief �L�[���̓��O���N���A
*
*	�L�[���̓��O�̃o�b�t�@�[���[���N���A����B
*	�^�X�N�؂�ւ�蒼��̌듮���h�����Ǝv�������ǁA�������Ă��ꂪ�悭�Ȃ������B
*	(�������ςȂ��ł����Ă��A���̒���Ɂu�������u�ԁv�t���O������)
*	���͂��̑΍���u���Ă݂��B
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
*	@brief �i�O������́j�L�[��Ԃ̎擾
*
*	���̊֐���KeyLock �� COM�K�[�h�L�[���l������B
*	���̓��͂𒼐ڎ�肽���Ƃ���GetKey�𗘗p����B
*/
DWORD CDirectInput::GetKeyEx(
							DWORD cid,	//!< �L�[���̓C���f�b�N�X(PLAYER)
							DWORD ofst)	//!< ���t���O�̓��͂��擾���邩
{
	CExclusiveTaskBase* bt = g_system.GetCurrentMainTask();

	if(keylocked)return(0);
	if(com_grd_enabled){
		return com_grd_key;
	}

	// �b��ݒu
	if (bt && bt->GetID() == 'BtlN' && g_play.IsHost())
		return (GetKeyNet(cid, ofst));

	return(GetKey(cid,ofst));
}

DWORD CDirectInput::SetKeyNet(DWORD index,DWORD key)
{
	DWORD keyold = GetKeyNet(index, 1);

	// ��񗎂����Ă邩���m��Ȃ�����**2�t���O��t����
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
*	@brief ���̃L�[���͂��擾����
*	@sa GetKeyEx
*/
DWORD CDirectInput::GetKey(DWORD cid,DWORD ofst)
{
	if(ofst>255)return(0);
	if(cid>=MAXNUM_KEYI)return(0);

	return(keylog[cid][keylognow+ofst]);
}

/*!
*	@brief ���̃L�[���͂��擾����
*	@sa GetKeyEx
*/
DWORD CDirectInput::GetKeyNet(DWORD cid,DWORD ofst)
{
	if(ofst>255)return(0);
	if(cid>=MAXNUM_KEYI)return(0);

	return(netkey[cid][keylognow+ofst]);
}

/*!
*	@brief �w��L�[���͂����O�̒�����T��
*	@return 0�ȏ�F���������ʒu, �}�C�i�X�F������Ȃ�����
*/
int CDirectInput::SeekKeyEx(DWORD cid,		//!< ���̓C���f�b�N�X(PLAYER)
							int offset,		//!< �����J�n�ʒu
							int num_seek,	//!< �����I���ʒu
							DWORD key		//!< �����L�[
							)
{
	CExclusiveTaskBase* bt = g_system.GetCurrentMainTask();

	if(keylocked)return(-1);
	if(com_grd_enabled)return(-1);

	// �b��ݒu
	if (bt && bt->GetID() == 'BtlN' && g_play.IsHost())
		return (SeekKeyNet(cid, offset, num_seek, key));

	return(SeekKey(cid,offset,num_seek,key));
}

/*!
*	@brief �w��L�[���͂����O�̒�����T��
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
*	@brief �w��L�[���͂����O�̒�����T��
*	@sa SeekKeyEx
*/
int CDirectInput::SeekKeyNet(DWORD cid,int offset,int num_seek,DWORD key)
{
	for(int i=offset;i<num_seek;i++){
		if((GetKeyNet(cid,i)& ((i == 0) ? 0x00001111 : 0x00003333)) == key){	// �b��[�u�i�����_�b�V���悯�j
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
// Direct Input ������
//********************************************************************

BOOL CDirectInput::InitializePad()
{
	if(!g_config.UseDInput()){
		CSystem::Log(_T("DirectInput(�p�b�h) ��g�p"),SYSLOG_INFO);
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

	//DirectInput8�I�u�W�F�N�g�̍쐬
	if(DI_OK != DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&pdi,NULL)){
		if(IDYES == MessageBox(hwnd,_T("�Q�[���p�b�h���g�p�ł��܂���B\n���s���܂����H"),
			_T("DirectInput8Create ���s"),MB_YESNO))
			return(TRUE);
		return(FALSE);
	}
	//�f�o�C�X�̗񋓁E�쐬
	res = pdi->EnumDevices(DI8DEVCLASS_GAMECTRL,EnumGamePad,(LPVOID)&jsnum_detected,DIEDFL_ATTACHEDONLY);
	if(res!=DI_OK){
		g_system.Log(_T("��InitDirectInput:EnumDevice�Ɏ��s\n"),SYSLOG_ERROR);
		isOK=FALSE;
	}

	//�f�o�C�X�̐ݒ�
	for(DWORD i=0;i<jsnum_detected;){
		isOK = TRUE;

		if(DI_OK!=pdidev[i]->SetDataFormat(&c_dfDIJoystick)){//�f�[�^�t�H�[�}�b�g
			g_system.Log(_T("��InitDirectInput:�f�[�^�t�H�[�}�b�g�ݒ�Ɏ��s\n"),SYSLOG_ERROR);
			isOK=FALSE;
		}
		if(DI_OK!=pdidev[i]->SetCooperativeLevel(hwnd,DISCL_EXCLUSIVE | DISCL_FOREGROUND)){//����
			g_system.Log(_T("��InitDirectInput:�������x���ݒ�Ɏ��s\n"),SYSLOG_ERROR);
			isOK=FALSE;
		}
		if(DI_OK!=pdidev[i]->SetProperty(DIPROP_AXISMODE,&dip.diph)){//�����[�h�̐ݒ�
			g_system.Log(_T("��InitDirectInput:�����[�h�ݒ�Ɏ��s\n"),SYSLOG_ERROR);
			isOK=FALSE;
		}
		//����񋓂��Ēl�͈̔͂�ݒ肷��
		if(DI_OK!=pdidev[i]->EnumObjects(EnumAxis,pdidev[i],DIDFT_AXIS)){
			g_system.Log(_T("��InitDirectInput:���l�͈͐ݒ�Ɏ��s\n"),SYSLOG_ERROR);
			isOK=FALSE;
		}
		if(!isOK){
			//�����̐ݒ�Ɏ��s�����炻��͊J�����Ă��܂�
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

	// �L�[�{�[�h�f�o�C�X����
	if ( SUCCEEDED(pdi->CreateDevice(GUID_SysKeyboard, &pdidev_kb, NULL)) )
	{
		isOK = TRUE;

		if ( FAILED(pdidev_kb->SetDataFormat(&c_dfDIKeyboard)) )
		{
			g_system.Log(_T("��InitDirectInput:�f�[�^�t�H�[�}�b�g�ݒ�Ɏ��s\n"),SYSLOG_ERROR);
			isOK=FALSE;
		}

		if ( FAILED(pdidev_kb->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)) )
		{
			g_system.Log(_T("��InitDirectInput:�������x���ݒ�Ɏ��s\n"),SYSLOG_ERROR);
			isOK=FALSE;
		}

		if (isOK)
		{
			// �L�[�ݒ��ϊ�
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

//�Q�[���p�b�h�񋓃R�[���o�b�N�֐�
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
		strcpy(g_input.gamepadname[num],lpddi->tszInstanceName);//���O��ۑ����Ă���
		(*(DWORD*)pvref)++;
	}

	if(*(DWORD*)pvref >= DINPUT_MAX_GAMEPAD){
		return(DIENUM_STOP);//�K�v�ȕ����������炨���܂�
	}

	return(DIENUM_CONTINUE);
}

//���̗񋓁E�l�͈͐ݒ�
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
// Direct Input ��n��
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
//  �p�b�h�f�[�^�̎擾
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
//  WM_ACTIVATE�@���A����
// *******************************************************************
void CDirectInput::RecoverDirectInput(WPARAM wParam,LPARAM lParam)
{
	int i;

	if(pdi != NULL){
		if(wParam == WA_INACTIVE){//��A�N�e�B�����
			for(i=0;i<DINPUT_MAX_GAMEPAD;i++){
				if( pdidev[i] != NULL ){
					pdidev[i]->Unacquire();
				}
			}
			pdidev_kb->Unacquire();
		}
		else{//�A�N�e�B�����
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
// ���z�L�[��DirectInput�L�[�ɕϊ�
//********************************************************************
BYTE CDirectInput::VirtKeyToDInputKey(int vKey)
{
	// �����L�[�i�P�`�X�j
	if (vKey >= '1' && vKey <= '9')
		return vKey - '1' + DIK_1;

	// �t�@���N�V�����L�[�i�e�P�`�e�P�O�j
	if (vKey >= VK_F1 && vKey <= VK_F10)
		return vKey - VK_F1 + DIK_F1;

	// �t�@���N�V�����L�[�i�e�P�P�`�e�P�T�j
	if (vKey >= VK_F11 && vKey <= VK_F15)
		return vKey - VK_F11 + DIK_F11;

	// �e���L�[�i�P�`�R�j
	if (vKey >= VK_NUMPAD1 && vKey <= VK_NUMPAD3)
		return vKey - VK_NUMPAD1 + DIK_NUMPAD1;

	// �e���L�[�i�S�`�U�j
	if (vKey >= VK_NUMPAD4 && vKey <= VK_NUMPAD6)
		return vKey - VK_NUMPAD4 + DIK_NUMPAD4;

	// �e���L�[�i�V�`�X�j
	if (vKey >= VK_NUMPAD7 && vKey <= VK_NUMPAD9)
		return vKey - VK_NUMPAD7 + DIK_NUMPAD7;

	// ���@�����@���̂�����΂�
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

	// �ȉ��AA-Z

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

	return vKey;	// ���������̂łƂ肠�������̂܂ܑ����Ă݂�i�����j�B
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

	// ('A`)���޸�
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

			// ����Ƃ��B
			pdidev[i]->CreateEffect(GUID_ConstantForce, &diEffect, &pdieffect[i][0], NULL);
			pdidev[i]->CreateEffect(GUID_ConstantForce, &diEffect, &pdieffect[i][1], NULL);
			pdidev[i]->CreateEffect(GUID_ConstantForce, &diEffect, &pdieffect[i][2], NULL);
		}
	}
}

// �t�H�[�X�̗͂��B
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



