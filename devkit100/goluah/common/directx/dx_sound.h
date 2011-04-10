
/*!
*	@file
*	@brief DirectSound/DirectShow
*/
#pragma once



/*!
*	@brief DirectSound/DirectShow�N���X
*	@ingroup DirectX
*
*	.wav�t�@�C����ǂݍ��ށB�J���͌Ăяo�����ł�邱�ƁB
*	DirectShow���g���� .mid .mp3 .wma ���̍Đ����s���B����̍Đ����ł����肷��B
*
*	wav�ǂݍ��݂̊֐��͐��N�O�ɗF�l�̂��R�s�y�����̂ŉ��ɂ��悭�킩��܂���B
*	DirectShow�̓T���v���R�[�h���R�s�y�����̂ŉ��ɂ��悭�킩��܂���B
*	�Ȃ񂾁A�킩��Ȃ����Ƃ΂��肶��Ȃ����E�E�E
*/
class CDirectSound
{
public:
	CDirectSound();
	~CDirectSound(){Destroy();}

	BOOL Initialize(HWND hwnd);
	void Destroy();

	BOOL InitDirectSound(HWND hwnd);
	void CleanDirectSound();
	BOOL CreateDirectShow();

	LPDIRECTSOUNDBUFFER CreateDSB(char *filename);

	BOOL PlayVideo(const char *filename);
	BOOL BGMPlay(const char* filename,BOOL loop=TRUE);
	void BGMStop();
	void BGMPause();
	void BGMResume();

	void OnWmgraphnotify();
public:
	BOOL ready;
	HWND hwndmain;

	LPDIRECTSOUND lpds;

	BOOL bgmon,videoon,bgmloop;
	char bgmfilename[256];
	LPDIRECTSOUNDBUFFER dsb_bgm;

	//bgm�n
	IGraphBuilder *pGraph;
	IMediaControl *pMediaControl;
	IMediaEventEx *pEvent;
	IMediaSeeking *pSeek;
	IVideoWindow  *pVidWin;
	IBasicAudio   *pAudio;
	BOOL BGMSeekAndPlay(const char* filename, BOOL loop = 1, double starttime = 0.0);
};


