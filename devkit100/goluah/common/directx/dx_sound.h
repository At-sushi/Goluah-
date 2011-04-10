
/*!
*	@file
*	@brief DirectSound/DirectShow
*/
#pragma once



/*!
*	@brief DirectSound/DirectShowクラス
*	@ingroup DirectX
*
*	.wavファイルを読み込む。開放は呼び出し側でやること。
*	DirectShowを使って .mid .mp3 .wma 等の再生を行う。動画の再生もできたりする。
*
*	wav読み込みの関数は数年前に友人のをコピペしたので俺にもよくわかりません。
*	DirectShowはサンプルコードをコピペしたので俺にもよくわかりません。
*	なんだ、わからないことばかりじゃないか・・・
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

	//bgm系
	IGraphBuilder *pGraph;
	IMediaControl *pMediaControl;
	IMediaEventEx *pEvent;
	IMediaSeeking *pSeek;
	IVideoWindow  *pVidWin;
	IBasicAudio   *pAudio;
	BOOL BGMSeekAndPlay(const char* filename, BOOL loop = 1, double starttime = 0.0);
};


