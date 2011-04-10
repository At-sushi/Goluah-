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
#pragma once

#include "define_const.h"
#include "config.h"	//�p�b�h���̒�`���K�v

/*!
*	@ingroup DirectX
*/
/*@{*/

//!�萔��`
#define DINPUT_MAX_GAMEPAD		NUM_PAD

/*!
*	@brief �L�[���͊Ǘ��N���X�B
*	@ingroup DirectX
*
*	���͂̃��O���Ƃ�A�{�^����������Ă���E���Ȃ��̔��f�����łȂ��A
*	�������ςȂ��̏�ԂȂ̂��A���݃t���[���ŉ����ꂽ���̂Ȃ̂������f�ł���B
*	
*	�L�[�{�[�h�̓��͂ƃp�b�h�iDirectX8�ȏ��DirectInput���g�p�j��
*	�����̓��͂���͂��������ď�������B
*
*	�s���ɂ�萶�̃L�[���͂�Ԃ��Ȃ��ꍇ������
*	�EKeyLock �œ��͂����b�N�����ꍇ
*	�EEnableComKey �ŃR���s���[�^�̃K�[�h�L�[���͂�L���ɂ����ꍇ
*/
class CDirectInput
{
public:
	CDirectInput();
	~CDirectInput(){Destroy();}

public:
	BOOL Initialize(HWND hwnd);
	BOOL InitializePad();//DirectInput������
	void Destroy();

	// ���莞����
	void KeyLog();	//�L�[���̓��O�����B���t���[���ĂԕK�v������
	void RecoverDirectInput(WPARAM wParam,LPARAM lParam);//�E�C���h�E�t�H�[�J�X���A����

	//!���͎擾�֐��̖߂�l��0�ɌŒ肷�邱�Ƃɂ���āA���z�I�ɃL�[���͂��֎~����
	void KeyLock(BOOL lk=TRUE);

	//!�S�L�[���O����������
	void ClearKeyLog();

	//!���z�L�[��DirectInput�L�[�ɕϊ�
	static BYTE VirtKeyToDInputKey(int vKey);

	//!���������ɕK�v�ȃR�[���o�b�N�֐�
	static BOOL FAR CALLBACK EnumGamePad(LPCDIDEVICEINSTANCE lpddi,LPVOID pvref);
	static BOOL FAR CALLBACK EnumAxis(LPCDIDEVICEOBJECTINSTANCE lpddoi,LPVOID pvref);

	//!�t�H�[�X�n��
	void StartForce(DWORD cid, DWORD num);

	// ���L�[���̓f�[�^�擾�n
public:
	DWORD GetKeyEx(DWORD cid,DWORD ofst);
	int SeekKeyEx(DWORD cid,int offset,int num_seek,DWORD key);
	DWORD GetKeyNet(DWORD cid,DWORD ofst);
	int SeekKeyNet(DWORD cid,int offset,int num_seek,DWORD key);
	DWORD SetKeyNet(DWORD index,DWORD key);
	DWORD GetAllKey();
	DWORD GetKey(DWORD cid,DWORD ofst);
	int SeekKey(DWORD cid,int offset,int num_seek,DWORD key);

public:
	// ��COM�K�[�h���p
	void EnableComKey()			{com_grd_enabled=TRUE; }
	void DisableComKey()		{com_grd_enabled=FALSE;}
	void SetComKey(DWORD key)	{com_grd_key=key;}

	// ����������֐�
private:
	DWORD KeyLog2(DWORD cid,int kb,int pad, PBYTE KeyState = NULL);		//!< �L�[���O��������
	void CleanDInput();													//!< DirectInput��n��
	DWORD GetPadState(int pid);											//!< Pad �f�[�^�擾
	void InitializeEffects();											//!< �G�t�F�N�g������

	// ���f�[�^�����o
public:
	int keylognow;										//!< ���O�J�n�ʒu
	DWORD keylog[MAXNUM_KEYI][256+256];					//!< �L�[���͂̃��O
	DWORD netkey[MAXNUM_KEYI][256+256];					//!< �l�b�g�ΐ펞�̃L�[���O�i�p�X���[�h�N���b�N�Ɏg���A���ł͂Ȃ��j

	//�p�b�h(DirectInput)�֘A
	int jsnum;											//!< �F�������p�b�h�̐�
	char gamepadname[DINPUT_MAX_GAMEPAD][256];			//!< �p�b�h�̖��O
	LPDIRECTINPUT8 pdi;									//!< DirectInput�I�u�W�F�N�g
	LPDIRECTINPUTDEVICE8 pdidev[DINPUT_MAX_GAMEPAD];	//!< �f�o�C�X�I�u�W�F�N�g
	LPDIRECTINPUTDEVICE8 pdidev_kb;						//!< �L�[�{�[�h�f�o�C�X���������Ă݂�e�X�g
	LPDIRECTINPUTEFFECT pdieffect[DINPUT_MAX_GAMEPAD][3];		//!< �t�H�[�X�t�B�[�h�o�b�N�̃G�t�F�N�g

	DWORD com_grd_key;		//!< COM�K�[�h�L�[
	BOOL  com_grd_enabled;	//!< COM�K�[�h�L�[���L��

private:
	HWND hwnd;				//!< �E�C���h�E�n���h���i�t�H�[�J�X���Ɋ֌W�j
	BOOL keylocked;			//!< �L�[���b�N�t���O
	UINT input_sleep_time;	//!< ClearKeyLog() ��ɂ�����Ƃ̊ԃL�[���͂𖳌��ɂ���J�E���^
	KEYCONFIG DIKeyState;	//!< DirectInput�p�ɕϊ������A�L�[�R���t�B�O���
};

#define PADUP		0x00000001
#define PADDOWN		0x00000002
#define PADLEFT		0x00000004
#define PADRIGHT	0x00000008
#define PADA		0x00000010
#define PADB		0x00000020
#define PADC		0x00000040
#define PADD		0x00000080


/*@}*/