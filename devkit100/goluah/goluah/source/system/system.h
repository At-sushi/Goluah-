

/*!
*	@file 
*	@brief �V�X�e���N���X���̒�`
*	���C���V�X�e���B�Q�[���S�ʂ̓���
*/

/*!
*	@defgroup System
*	@brief �Q�[���S�ʂ̓�������
*/

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "system_cell.h"

#include "define_gcd.h"
#include "task.h"


#define NUM_SYSTEMSOUND		32//0-31�܂ł�wav��ǂݍ���

/*!
*	@ingroup System
*	@brief System::Log ���O�̕���
*/
enum SyatemLogCategory
{
	SYSLOG_NONE,		//�Ƃ��ɂȂ���Ȃ�
	SYSLOG_INFO,		//���@�E�E�E���āA�Ȃ�́H
	SYSLOG_ERROR,		//�G���[
	SYSLOG_WARNING,		//�x��
	SYSLOG_ALERT,		//�x��i���������I�j
	SYSLOG_DEBUG		//�f�o�b�O���
};


/*!
*	@ingroup System
*
*	@brief �V�X�e���N���X�B�Q�[���S�ʂ̓���
*
*	���ۂɂ͂���܂�d�����Ă��Ȃ��B
*	DX���̃N���X��������������A���C�����[�v�Ń^�X�N�Ǘ��N���X���ĂԂ��炢�B
*	���Ƃ̓��O�f���o���Ƃ��̎G�p�n�B
*/
class CSystem
{
friend class CExport;//�^�X�N�}�l�[�W�����璼�ڐ퓬�^�X�N�𔲂��o�����߂ɕK�v

public:
	CSystem();							//!< �R���X�g���N�^
	~CSystem(){Destroy();}				//!< �f�X�g���N�^

	//�������E�j��
	BOOL Initialize(HWND hwnd);			//!< ������
	BOOL GameStart();					//!< �Q�[���J�n���ɃG���g���[�|�C���g�i�̕��ʁj����Ă΂��
	void Destroy();						//!< �j���B�f�X�g���N�^�̎�����

	void MainLoop();					//!< ���C�����[�v����Ă΂��B�莞����
	void WndMessage(HWND hWnd,UINT msg, WPARAM wparam, LPARAM lparam);//!< �E�C���h�v���V�[�W������Ă΂��

	//��Ԏ擾
	BOOL IsWindowActive();				//!< �A�v���P�[�V�����E�C���h�E���A�N�e�B�u�ȏ�Ԃ��ǂ���
	BOOL IsReady(){return ready;}		//!< �Q�[�����J�n�ł����Ԃ��ǂ����i�������H
	HWND GetHWnd(){return hwnd;}		//!< �A�v���P�[�V�����E�C���h�E�̃E�C���h�E�n���h�����擾

	//�T�[�r�X
	void PlaySystemSound(DWORD sid);									//!< �T�E���h�Đ�
	double DrawBMPText(double x,double y,float z,char *str,DWORD col);	//!< �r�b�g�}�b�v�e�L�X�g��`��
	double DrawBMPTextEx(double x,double y,float z,char *str,			//!< �r�b�g�}�b�v�e�L�X�g��`��(�g�k�E�v���|�[�V���i���I�v�V������)
		DWORD col,float scaX,float scaY,DWORD flags);
	void ReturnTitle();													//!< �^�C�g����ʂ֖߂�
	void DisableRendering()	{m_render_disabled=TRUE;}					//!< �^�X�N�Ǘ��ŕ`������Ȃ��i����Đ��p�j
	void EnableRendering()	{m_render_disabled=FALSE;}					//!< DisableRendering���A

	//fps
	DWORD GetFPS(){return m_fps;}										//!< ���ۃt���[�����[�g�擾
	DWORD GetMaxFPS(){return (DWORD)(1000.0/m_loopTime);}				//!< �ő�t���[�����[�g�i�����������Ă��Ȃ���Ԃŏo��j�擾
	void SetMaxFPS(float newFPS){m_loopTime = (DWORD)(1000.0/newFPS);}	//!< �t���[�����[�g���~�b�g�ݒ�
	void UpdateFrameRate();												//!< �R���t�B�O����ݒ�

	//�^�X�N
	void AddTask(CTaskBase* task){taskManager.AddTask(task);}			//!< �V�K�^�X�N�ǉ�
	void RemoveTask(DWORD id){taskManager.RemoveTaskByID(id);}			//!< �w��ID�����^�X�N���폜
	CExclusiveTaskBase* GetCurrentMainTask();							//!< ���݂̔r���^�X�N���擾
	CBackgroundTaskBase* FindBGTask(DWORD id);							//!< BG�n�^�X�N�̒�����w��ID�����^�X�N��T��
	CTaskBase*			 FindTask(DWORD id);							//!< �ʏ�n�^�X�N�̒�����w��ID�����^�X�N��T��

	//�V�X�e���O���t�B�b�N���擾
	MYSURFACE** GetSystemGraphicSurface(){return sdds;}					//!< �V�X�e���Ƃ��ĕێ�����Ă���GCD�p�̃r�b�g�}�b�v�z����擾
	GCD_RECT* GetSystemGraphicRect(){return srdat;}						//!< �V�X�e���Ƃ��ĕێ�����Ă���GCD�̋�`�z����擾
	GCD_CELL2* GetSystemGraphicCell(){return scdat;}					//!< �V�X�e���Ƃ��ĕێ�����Ă���GCD�̃Z���z����擾
	GCD_HANTEI* GetSystemGraphicHantei(){return shdat;}					//!< �V�X�e���Ƃ��ĕێ�����Ă���GCD�̂����蔻���`�z����擾

	//�f�o�b�O
	void NotifyExcption();												//!< ��O������ʒm�B���񃁃C�����[�v�Ń^�C�g���߂�i�ݒ�Ɉ˂�j
	BOOL IsErrorTitle()		{return m_error_title;}						//!< �G���[�����Ń^�C�g���ɖ߂����̂��t���O�擾
	static void Log(const char *msg,DWORD category);					//!< ���ނ��Ńt�@�C���փ��O���o��
	static void LogErr(const char *fmt,...);							//!< �t�@�C���փ��O���o��(�G���[���A����t�@�C��FLUSH)
	static void LogWarning(const char *fmt,...);						//!< ����:�x�� �Ń��O���o���B�t�H�[�}�b�g��
	void PushSysTag(const char* tag_str);								//!< �^�O��ݒ�
	void PopSysTag();													//!< �^�O���ЂƂ�Pop�ADLL�^�O�̃N���A
	void ClearSysTag();													//!< �V�X�e���^�O�̃N���A�ADLL�^�O���N���A
	void PushDLLTag(const char* tag_str);								//!< �^�O��ݒ�
	void PopDLLTag();													//!< �^�O���ЂƂ�Pop
	void ClearDLLTag();													//!< DLL�^�O�̃N���A
	void DumpTags();													//!< �^�O���G���[���O�Ƃ��ďo��

protected:
	void InitSystemSound();												//!< �V�X�e���T�E���h�̃��[�h
	void InitSystemGraphics();											//!< �V�X�e���O���t�B�b�N(system/cell.gcd��)�����[�h
public:	void OnWMKeydown(WPARAM wp,LPARAM lp);							//!< WM_KEYDOWN�E�B���h�E���b�Z�[�W����
protected:
	BOOL ShowInformation();												//!< ��������̏��_�C�A���O�̕\��
	void SaveScreenShot();												//!< �X�N���[���V���b�g�ۑ�

	//�f�[�^�����o
public:
	//�X�C�b�`
	BOOL sw_rects;										//!< ���g�p
	BOOL sw_showfps;									//!< ���g�p
	BOOL sw_showbg;										//!< ���g�p
	int  sw_showstates;									//!< ���g�p
	int  sw_slowmode;									//!< ���g�p
protected:
	BOOL ready;											//!< �������������������ǂ���
	HWND hwnd;											//!< �A�v���P�[�V�����E�C���h�E�̃E�C���h�E�n���h��
	DWORD m_fps;										//!< ���݂̃t���[�����[�g
	DWORD m_loopTime;									//!< ���C�����[�v����ms�ŉ񂷂��i�t���[�����[�g����j
	BOOL  m_render_disabled;							//!< �`��J�n/�I�����s��Ȃ��iSirectShow����Đ��p�j

	//�V�X�e���E�O���t�B�b�N
	MYSURFACE *sdds[GCDMAX_IMAGES];						//!< system/image?.bmp�����[�h��������
	GCD_RECT srdat[GCDMAX_RECTANGLES];					//!< system/cellgcd�� ���[�h��������
	GCD_CELL2 scdat[GCDMAX_CELLS];						//!< system/cellgcd�� ���[�h��������
	GCD_HANTEI shdat[GCDMAX_CELLS];						//!< system/cellgcd�� ���[�h��������

	//�V�X�e���E�T�E���h
	LPDIRECTSOUNDBUFFER dsb_efct[2][NUM_SYSTEMSOUND];	//!< �V�X�e���T�E���h
	BOOL b_efctlst[NUM_SYSTEMSOUND];					//!< �V�X�e���T�C���ǂ̂����ǂ���̃T�E���h���Đ��������B�A�����ē���wav���Đ����邽�߂ɕK�v

	CTaskManager taskManager;							//!< �^�X�N�Ǘ��N���X
	BOOL m_return_title_flag;							//!< ReturnTitle�t���O
	BOOL m_error_title;									//!< �G���[�^�C�g����ʃt���O
	BOOL m_first_update_flag;							//!< �N���㏉��t���O
	BOOL m_debug_menu_enabled;							//!< �f�o�b�O���j���[���L��

	static char *logBuffer;								//!< �f�o�b�O���O������t�H�[�}�b�g�p�o�b�t�@
	std::stack<const char*> m_systag;					//!< ��O�������ɓf���o���V�X�e���^�O
	std::stack<const char*> m_dlltag;					//!< ��O�������ɓf���o��DLL�^�O
};


#endif//_SYSTEM_H_