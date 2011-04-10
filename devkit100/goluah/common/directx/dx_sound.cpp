
/*!
*	@file
*	@brief DirectSound�N���X
*
*	�E�E�E�Ƃ�����DirectShow���g�p
*
*	.wav�t�@�C����ǂݍ��ށB�J���͎�����(�Ăяo������)���B
*	DirectShow���g���� .mid .mp3 .wma ���̍Đ����s���B����̍Đ����ł����肷��B
*
*/
#include "stdafx.h"

#include "global.h"			//config�I�u�W�F�N�g���K�v
#include "task.h"			//�o�b�N�O���E���h�^�X�N�̍쐬�ɕK�v
#include "dx_sound.h"

#ifndef RELEASE
#define RELEASE(a)	if(a){a->Release();a=NULL;}
#endif

#include "locale.h"


#define WM_GRAPHNOTIFY  WM_APP + 1

/*!
	@brief BGM���[�v�p�E�B���h�E���b�Z�[�W�擾�N���X

	BGM�����[�v�����邽�߂ɃE�C���h�E�̃��b�Z�[�W���E���K�v������
	���̂��߂Ƀo�b�N�O���E���h�^�X�N�Ƃ��ēo�^����N���X
	�O���[�o���� g_sound �� OnWmgraphnotify() ���R�[������
*/
class CBGMLoopMessageListener : public CBackgroundTaskBase
{
public:
	DWORD GetID(){return 'bgmL';}
	void WndMessage(HWND hWnd,UINT msg,WPARAM wparam, LPARAM lparam)
	{
		if(msg==WM_GRAPHNOTIFY){
			g_sound.OnWmgraphnotify();
		}
	}
};



//�����Ə���========================================================
//!����
CDirectSound::CDirectSound()
{
	ready=FALSE;
	hwndmain = NULL;

	bgmon=videoon=FALSE;
	dsb_bgm=NULL;

	pAudio=NULL;
	pVidWin=NULL;
	pSeek=NULL;
	pMediaControl=NULL;
	pEvent=NULL;
    pGraph=NULL;

	lpds = NULL;
}

//!������
BOOL CDirectSound::Initialize(HWND hwnd)
{
	hwndmain = hwnd;

	if(g_config.UseDSound()){
		if(DirectSoundCreate(NULL,&lpds,NULL) != DS_OK){
			CSystem::Log("DirectSoundCreate�Ɏ��s",SYSLOG_ERROR);
			lpds=NULL;
		}
		if(lpds){
			if(lpds->SetCooperativeLevel(hwnd,DSSCL_PRIORITY) != DS_OK){
				CSystem::Log("DSound-SetCooperativeLevel�Ɏ��s",SYSLOG_ERROR);
				lpds->Release();
				lpds=NULL;
			}
		}
	}
	else CSystem::Log("DirectSound (wav�Đ�) ��g�p",SYSLOG_INFO);

	//CoInitialize(NULL);
	g_system.AddTask(new CBGMLoopMessageListener());//BGM���[�v���b�Z�[�W�̎擾�^�X�N

	ready=TRUE;
	return (lpds!=NULL);
}

//!�j��
void CDirectSound::Destroy()
{
	BGMStop();
	RELEASE(lpds);
	//CoUninitialize();

	ready = FALSE;
}


//���̑�=============================================================

/*!
*	wav��ǂݍ����DirectSound�o�b�t�@�[��Ԃ��B�󂯎�����玩����Release���邱�ƁB
*	wav�̃t�H�[�}�b�g��PCM�łȂ��ƃ_���炵���BADPCM�Ƃ��_���������B
*	Windows�A�N�Z�T���̃T�E���h���R�[�_��wav�t�@�C���̌`���������̂ŁA������Q�l�ɁB
*	DirectX5�̍��ɗF�l�̏������R�[�h���R�s�y�����o��������B
*
*	@param filename wav�̃t�@�C����
*	@return ���s��NULL
*/
LPDIRECTSOUNDBUFFER CDirectSound::CreateDSB(char *filename)
{
	if(!g_config.UseDSound())return NULL;
	if(lpds==NULL)return NULL;

	HMMIO			hmfr;
	MMCKINFO		parent,child;
	WAVEFORMATEX	wfmtx;
	LPDIRECTSOUNDBUFFER lpDSBuffer=NULL;

	//�v�`�u�t�@�C�����}���`���f�B�A�h/�n�֐��ŊJ��
	hmfr = mmioOpen(filename,NULL,MMIO_READ | MMIO_ALLOCBUF);
	if(hmfr == NULL){
		return(NULL);
	}

	parent.ckid			=(FOURCC)0;
	parent.cksize		=0;
	parent.fccType		=(FOURCC)0;
	parent.dwDataOffset	=0;
	parent.dwFlags		=0;
	child = parent;

	parent.fccType		= mmioFOURCC('W','A','V','E');
	if(mmioDescend(hmfr,&parent,NULL,MMIO_FINDRIFF)){
		mmioClose(hmfr,0);
		return(NULL);
	}

	child.ckid			= mmioFOURCC('f','m','t',' ');
	if(mmioDescend(hmfr,&child,&parent,0)){
		mmioClose(hmfr,0);
		return(NULL);
	}

	//�v�`�u�t�H�[�}�b�g����ǂݍ���
	if(mmioRead(hmfr,(char*)&wfmtx,sizeof(wfmtx)) != sizeof(wfmtx)){
		mmioClose(hmfr,0);
		return false;
	}

	//���̃t�@�C�����v�`�u���m�F
	if(wfmtx.wFormatTag != WAVE_FORMAT_PCM){
		mmioClose(hmfr,0);
		return false;
	}

	if(mmioAscend(hmfr,&child,0)){
		mmioClose(hmfr,0);
		return false;
	}

	//�`�����N���\���̂ɓǂݍ���
	child.ckid		= mmioFOURCC('d','a','t','a');
	if(mmioDescend(hmfr,&child,&parent,MMIO_FINDCHUNK)){
		mmioClose(hmfr,0);
		return false;
	}

	//���������蓖�ā@�f�[�^��ǂݍ���
	BYTE *pBuffer = new BYTE[child.cksize];
	if((DWORD)mmioRead(hmfr,(char*)pBuffer,child.cksize) != child.cksize){
		mmioClose(hmfr,0);
		delete [] pBuffer;
		return(FALSE);
	}

	//�v�`�u�d�t�@�C�������
	mmioClose(hmfr,0);

	//DirectSound�o�b�t�@���쐬����
	DSBUFFERDESC	dsbdesc;
	PCMWAVEFORMAT	pcmwf;

	memset(&pcmwf,0,sizeof(PCMWAVEFORMAT));
	pcmwf.wf.wFormatTag		= WAVE_FORMAT_PCM;
	pcmwf.wf.nChannels		= wfmtx.nChannels;
	pcmwf.wf.nSamplesPerSec	= wfmtx.nSamplesPerSec;
	pcmwf.wf.nBlockAlign	= wfmtx.nBlockAlign;
	pcmwf.wf.nAvgBytesPerSec= wfmtx.nAvgBytesPerSec;
	pcmwf.wBitsPerSample	= wfmtx.wBitsPerSample;

	memset(&dsbdesc,0,sizeof(DSBUFFERDESC));
	dsbdesc.dwSize			= sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags			= DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN;
	dsbdesc.dwBufferBytes	= child.cksize;
	dsbdesc.lpwfxFormat		= (LPWAVEFORMATEX)&pcmwf;

	if(lpds->CreateSoundBuffer(&dsbdesc,&lpDSBuffer,NULL)!=DS_OK){
		delete [] pBuffer;
		return(NULL);
	}

	//�v�`�u�d�t�@�C���̃f�[�^���o�b�t�@�ɓ]��
	LPVOID				written1,written2;
	DWORD				length1,length2;

	if(lpDSBuffer->Lock(0,child.cksize,&written1,&length1,&written2,&length2,0) == DSERR_BUFFERLOST){
		lpDSBuffer->Restore();
		lpDSBuffer->Lock(0,child.cksize,&written1,&length1,&written2,&length2,0);
	}
	CopyMemory(written1,pBuffer,length1);
	if(written2 != NULL) CopyMemory(written2,pBuffer+length1,length2);
	lpDSBuffer->Unlock(written1,length1,written2,length2);

	//��ƃo�b�t�@���
	delete []pBuffer;

	return(lpDSBuffer);
}

char* GetDSErrCode(HRESULT ret)
{
	switch(ret){
	case S_OK :							return "����";
	case VFW_S_AUDIO_NOT_RENDERED :		return "�I�[�f�B�I �X�g���[�����Đ��ł��Ȃ��B�K�؂ȃ����_����������Ȃ������B ";
	case VFW_S_DUPLICATE_NAME :			return "���O���d�����Ă���t�B���^�̒ǉ����A�ύX��̖��O�Ő��������B ";
	case VFW_S_PARTIAL_RENDER :			return "���̃��[�r�[�ɃT�|�[�g����Ȃ��t�H�[�}�b�g�̃X�g���[�����܂܂�Ă���B ";
	case VFW_S_VIDEO_NOT_RENDERED :		return "�r�f�I �X�g���[�����Đ��ł��Ȃ��B�K�؂ȃ����_����������Ȃ������B ";
	case E_ABORT :						return "���삪���~���ꂽ�B";
	case E_FAIL :						return "���s�B ";
	case E_INVALIDARG :					return "�����������B ";
	case E_OUTOFMEMORY :				return "�������s���B ";
	case E_POINTER :					return "NULL �|�C���^�����B ";
	case VFW_E_CANNOT_CONNECT :			return "�ڑ����m�����钆�ԃt�B���^�̑g�ݍ��킹��������Ȃ������B ";
	case VFW_E_CANNOT_LOAD_SOURCE_FILTER :return "���̃t�@�C���̃\�[�X �t�B���^�����[�h�ł��Ȃ��B ";
	case VFW_E_CANNOT_RENDER :			return "�X�g���[���������_�����O����t�B���^�̑g�ݍ��킹��������Ȃ������B ";
	case VFW_E_INVALID_FILE_FORMAT :	return "�t�@�C�� �t�H�[�}�b�g�������B ";
	case VFW_E_NOT_FOUND :				return "�I�u�W�F�N�g�܂��͖��O��������Ȃ������B ";
	case VFW_E_NOT_IN_GRAPH :			return "�t�B���^ �O���t�ɑ��݂��Ȃ��I�u�W�F�N�g�ɗv�����ꂽ�֐������s�ł��Ȃ��B";
	case VFW_E_UNKNOWN_FILE_TYPE :		return "���̃t�@�C���̃��f�B�A �^�C�v���F������Ȃ��B ";
	case VFW_E_UNSUPPORTED_STREAM :		return "�t�@�C�����Đ��ł��Ȃ��B�t�H�[�}�b�g���T�|�[�g����Ă��Ȃ��B";
	default:return "�s��";
	}
}

/*!
*	@brief ����Đ�
*
*	DirectShow�Ƀt�@�C������n���čĐ����߂����邾���B
*	����̍Đ����́ADirect3D�̃����_�����O�𔭐�������Ƃ�����̂ŁA
*	���C�����[�v�̃����_�����O�J�n�E�I���������~���čs�����ƁB
*
*	@param filename �Đ��t�@�C����
*	@return FALSE�͎c�O�Ȍ��ʂɏI�����
*/
BOOL CDirectSound::PlayVideo(const char *filename)
{
	if(!g_config.UseDShow())return(FALSE);

	BGMStop();
	if(!CreateDirectShow())return(FALSE);

	bgmon=FALSE;
	videoon=TRUE;
	bgmloop=FALSE;
	
	//����ݒ���V�X�e��(?)�̂��̂ɐݒ�. mbstowcs�ɉe��
	setlocale(LC_ALL,"");

	//������ϊ�
	char bgmfilename[256];
	wchar_t ubgmfilename[256];
	sprintf(bgmfilename,"%s",filename);
	mbstowcs( ubgmfilename, bgmfilename, strlen(bgmfilename)+1 );
    // �O���t���\�z����B
	if(pGraph->RenderFile(ubgmfilename, NULL) != S_OK){
		gbl.ods("CDSound::PlayVideo / RenderFile�񎸔s\n");
		BGMStop();
		return(FALSE);
	}

	pVidWin->put_Owner((OAHWND)hwndmain);
	pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
	RECT grc;
	GetClientRect(hwndmain, &grc);
    pVidWin->SetWindowPosition(0, 0, grc.right, grc.bottom);

	pMediaControl->Run();

	return(TRUE);
}

/*!
*	@brief BGM�Đ�
*
*	DirectShow�Ƀt�@�C������n���čĐ����߂����邾���B
*	����̍Đ����́ADirect3D�̃����_�����O�𔭐�������Ƃ�����̂ŁA
*	���C�����[�v�̃����_�����O�J�n�E�I���������~���čs�����ƁB
*
*	@param filename �Đ��t�@�C����
*	@param loop TRUE�Ȃ�΁A�Đ��I����Ƀ��[�v����
*	@return FALSE�͎c�O�Ȍ��ʂɏI�����
*/
BOOL CDirectSound::BGMPlay(const char *filename,BOOL loop)
{
	if(!g_config.UseDShow())return FALSE;

	BGMStop();
	if(!CreateDirectShow())return(FALSE);

	bgmon=TRUE;
	videoon=FALSE;
	bgmloop=loop;

	//����ݒ���V�X�e��(?)�̂��̂ɐݒ�. mbstowcs�ɉe��
	setlocale(LC_ALL,"");

	char *bgmfilename = new char[256];
	wchar_t *ubgmfilename= new wchar_t [256];


	// wma -------------------------------------------------------------
	sprintf(bgmfilename,"%s.wma",filename);
	mbstowcs( ubgmfilename, bgmfilename, strlen(bgmfilename)+1 );
	if(S_OK == pGraph->RenderFile(ubgmfilename, NULL)){
		pMediaControl->Run();
		// �{�����[���������Ă���
		pAudio->put_Volume(-500);
		delete [] bgmfilename;
		delete [] ubgmfilename;
		return(TRUE);
	}

	// mp3 -------------------------------------------------------------
	sprintf(bgmfilename,"%s.mp3",filename);
	mbstowcs( ubgmfilename, bgmfilename, strlen(bgmfilename)+1 );
	if(S_OK == pGraph->RenderFile(ubgmfilename, NULL)){
		pMediaControl->Run();
		// �{�����[���������Ă���
		pAudio->put_Volume(-500);
		delete [] bgmfilename;
		delete [] ubgmfilename;
		return(TRUE);
	}

	// wave ------------------------------------------------------------
	sprintf(bgmfilename,"%s.wav",filename);
	mbstowcs( ubgmfilename, bgmfilename, strlen(bgmfilename)+1 );
	if(S_OK == pGraph->RenderFile(ubgmfilename, NULL)){
		pMediaControl->Run();
		// �{�����[���������Ă���
		pAudio->put_Volume(-500);
		delete [] bgmfilename;
		delete [] ubgmfilename;
		return(TRUE);
	}
	
	// midi ------------------------------------------------------------
	sprintf(bgmfilename,"%s.mid",filename);
	mbstowcs( ubgmfilename, bgmfilename, strlen(bgmfilename)+1 );
	if(S_OK == pGraph->RenderFile(ubgmfilename, NULL)){
		pMediaControl->Run();
		// �{�����[���������Ă���
//		pAudio->put_Volume(-500);
		delete [] bgmfilename;
		delete [] ubgmfilename;
		return(TRUE);
	}

	gbl.ods("CDirectSound::BGMPlay : %s �Đ����s",filename);

	BGMStop();
	delete [] bgmfilename;
	delete [] ubgmfilename;
	return(FALSE);
}

/*!
*	@brief BGM�Đ��I���E�C���h�E���b�Z�[�W�擾
*
*	DirectShow�̓t�@�C���̍Đ��I�����ɃE�C���h�E�ɑ΂��ă��b�Z�[�W�𑗐M����悤�ɂȂ��Ă���B
*	���̊֐���BGM/���� �Đ����I�������Ƃ��̃��b�Z�[�W����M�����Ƃ��ɌĂ΂�A���[�v�����Ȃǂ��s���B
*/
void CDirectSound::OnWmgraphnotify()
{
	if(!g_config.UseDShow())return;

	long evCode, param1, param2;
	LONGLONG zerotime=0;
	HRESULT hr;
	BOOL destroooooy=FALSE;

	if(pEvent==NULL)return;

	while (hr = pEvent->GetEvent(&evCode, &param1, &param2, 0), SUCCEEDED(hr))
	{
		hr = pEvent->FreeEventParams(evCode, param1, param2);
		switch(evCode){
		case EC_COMPLETE:
			gbl.ods("...EC_COMPLETE");
			if(bgmon && bgmloop){
				gbl.ods("case bgm loop");
				pSeek->SetPositions(&zerotime,
					AM_SEEKING_AbsolutePositioning,NULL,AM_SEEKING_NoPositioning);
				pMediaControl->Run();
			}
			else if(videoon){
				destroooooy=TRUE;
				pVidWin->put_Visible(OAFALSE);
				gbl.ods("video stop");
			}
			else {
				destroooooy=TRUE;
				gbl.ods("other case");
			}
			break;
		}
	}
	if(destroooooy)BGMStop();
}

/*!
*	@brief DirectShow����
*
*	DirectShow�̐����E�j���́ABGM�⓮��̍Đ��E��~���ɖ���s���Ă���B
*	�����������猋�\�Ȗ��ʂȂ̂�������Ȃ��B
*/
BOOL CDirectSound::CreateDirectShow()
{
	if(!g_config.UseDShow())return FALSE;

    CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
                        IID_IGraphBuilder, (void **)&pGraph);
	if(pGraph==NULL)return(FALSE);
    pGraph->QueryInterface(IID_IMediaControl, (void **)&pMediaControl);
    pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);
	pEvent->SetNotifyWindow((OAHWND)hwndmain, WM_GRAPHNOTIFY, 0);
	pGraph->QueryInterface(IID_IMediaSeeking, (void **)&pSeek);
	pGraph->QueryInterface(IID_IVideoWindow, (void **)&pVidWin);
	pGraph->QueryInterface(IID_IBasicAudio, (void **)&pAudio);
	return(TRUE);
}

/*!
*	@brief BGM�E���� �̍Đ���~
*/
void CDirectSound::BGMStop()
{
	if(!g_config.UseDShow())return;

	bgmon=FALSE;
	videoon=FALSE;

	if(pMediaControl){
		pMediaControl->Stop();
	}
	if(pVidWin){
		pVidWin->put_Visible(OAFALSE);
	}

	RELEASE(pAudio);
	RELEASE(pVidWin);
	RELEASE(pSeek);
	RELEASE(pEvent);
	RELEASE(pMediaControl);
    RELEASE(pGraph);
}

/*!
*	@brief BGM�E���� �̍Đ��ꎞ��~
*/
void CDirectSound::BGMPause()
{
	if(!g_config.UseDShow())return;

	if (pMediaControl){
		pMediaControl->Pause();
	}
}

/*!
*	@brief BGM�E���� �̈ꎞ��~�ĊJ
*/
void CDirectSound::BGMResume()
{
	if(!g_config.UseDShow())return;

	if (pMediaControl){
		pMediaControl->Run();
	}
}

/*!
*	@brief �J�n�ʒu���w�肵�čĐ�
*
*	���̂���BGMPlay�ɓ������邩��
*/
BOOL CDirectSound::BGMSeekAndPlay(const char* filename, BOOL loop, double starttime)
{
	if (!BGMPlay(filename, loop))
		return FALSE;

	LONGLONG startpos = (LONGLONG)(starttime * 10000.0);
	pSeek->SetPositions(&startpos, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
	
	return TRUE;
}

