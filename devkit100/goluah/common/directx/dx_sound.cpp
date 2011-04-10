
/*!
*	@file
*	@brief DirectSoundクラス
*
*	・・・といいつつDirectShowも使用
*
*	.wavファイルを読み込む。開放は自分で(呼び出し側が)やる。
*	DirectShowを使って .mid .mp3 .wma 等の再生を行う。動画の再生もできたりする。
*
*/
#include "stdafx.h"

#include "global.h"			//configオブジェクトが必要
#include "task.h"			//バックグラウンドタスクの作成に必要
#include "dx_sound.h"

#ifndef RELEASE
#define RELEASE(a)	if(a){a->Release();a=NULL;}
#endif

#include "locale.h"


#define WM_GRAPHNOTIFY  WM_APP + 1

/*!
	@brief BGMループ用ウィンドウメッセージ取得クラス

	BGMをループさせるためにウインドウのメッセージを拾う必要がある
	そのためにバックグラウンドタスクとして登録するクラス
	グローバルの g_sound の OnWmgraphnotify() をコールする
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



//生成と消滅========================================================
//!生成
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

//!初期化
BOOL CDirectSound::Initialize(HWND hwnd)
{
	hwndmain = hwnd;

	if(g_config.UseDSound()){
		if(DirectSoundCreate(NULL,&lpds,NULL) != DS_OK){
			CSystem::Log("DirectSoundCreateに失敗",SYSLOG_ERROR);
			lpds=NULL;
		}
		if(lpds){
			if(lpds->SetCooperativeLevel(hwnd,DSSCL_PRIORITY) != DS_OK){
				CSystem::Log("DSound-SetCooperativeLevelに失敗",SYSLOG_ERROR);
				lpds->Release();
				lpds=NULL;
			}
		}
	}
	else CSystem::Log("DirectSound (wav再生) 非使用",SYSLOG_INFO);

	//CoInitialize(NULL);
	g_system.AddTask(new CBGMLoopMessageListener());//BGMループメッセージの取得タスク

	ready=TRUE;
	return (lpds!=NULL);
}

//!破棄
void CDirectSound::Destroy()
{
	BGMStop();
	RELEASE(lpds);
	//CoUninitialize();

	ready = FALSE;
}


//その他=============================================================

/*!
*	wavを読み込んでDirectSoundバッファーを返す。受け取ったら自分でReleaseすること。
*	wavのフォーマットはPCMでないとダメらしい。ADPCMとかダメだった。
*	Windowsアクセサリのサウンドレコーダでwavファイルの形式が見れるので、それを参考に。
*	DirectX5の頃に友人の書いたコードをコピペした覚えがある。
*
*	@param filename wavのファイル名
*	@return 失敗はNULL
*/
LPDIRECTSOUNDBUFFER CDirectSound::CreateDSB(char *filename)
{
	if(!g_config.UseDSound())return NULL;
	if(lpds==NULL)return NULL;

	HMMIO			hmfr;
	MMCKINFO		parent,child;
	WAVEFORMATEX	wfmtx;
	LPDIRECTSOUNDBUFFER lpDSBuffer=NULL;

	//ＷＡＶファイルをマルチメディアＩ/Ｏ関数で開く
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

	//ＷＡＶフォーマット情報を読み込む
	if(mmioRead(hmfr,(char*)&wfmtx,sizeof(wfmtx)) != sizeof(wfmtx)){
		mmioClose(hmfr,0);
		return false;
	}

	//このファイルがＷＡＶか確認
	if(wfmtx.wFormatTag != WAVE_FORMAT_PCM){
		mmioClose(hmfr,0);
		return false;
	}

	if(mmioAscend(hmfr,&child,0)){
		mmioClose(hmfr,0);
		return false;
	}

	//チャンクを構造体に読み込む
	child.ckid		= mmioFOURCC('d','a','t','a');
	if(mmioDescend(hmfr,&child,&parent,MMIO_FINDCHUNK)){
		mmioClose(hmfr,0);
		return false;
	}

	//メモリ割り当て　データを読み込む
	BYTE *pBuffer = new BYTE[child.cksize];
	if((DWORD)mmioRead(hmfr,(char*)pBuffer,child.cksize) != child.cksize){
		mmioClose(hmfr,0);
		delete [] pBuffer;
		return(FALSE);
	}

	//ＷＡＶＥファイルを閉じる
	mmioClose(hmfr,0);

	//DirectSoundバッファを作成する
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

	//ＷＡＶＥファイルのデータをバッファに転送
	LPVOID				written1,written2;
	DWORD				length1,length2;

	if(lpDSBuffer->Lock(0,child.cksize,&written1,&length1,&written2,&length2,0) == DSERR_BUFFERLOST){
		lpDSBuffer->Restore();
		lpDSBuffer->Lock(0,child.cksize,&written1,&length1,&written2,&length2,0);
	}
	CopyMemory(written1,pBuffer,length1);
	if(written2 != NULL) CopyMemory(written2,pBuffer+length1,length2);
	lpDSBuffer->Unlock(written1,length1,written2,length2);

	//作業バッファ解放
	delete []pBuffer;

	return(lpDSBuffer);
}

char* GetDSErrCode(HRESULT ret)
{
	switch(ret){
	case S_OK :							return "成功";
	case VFW_S_AUDIO_NOT_RENDERED :		return "オーディオ ストリームを再生できない。適切なレンダラが見つからなかった。 ";
	case VFW_S_DUPLICATE_NAME :			return "名前が重複しているフィルタの追加が、変更後の名前で成功した。 ";
	case VFW_S_PARTIAL_RENDER :			return "このムービーにサポートされないフォーマットのストリームが含まれている。 ";
	case VFW_S_VIDEO_NOT_RENDERED :		return "ビデオ ストリームを再生できない。適切なレンダラが見つからなかった。 ";
	case E_ABORT :						return "操作が中止された。";
	case E_FAIL :						return "失敗。 ";
	case E_INVALIDARG :					return "引数が無効。 ";
	case E_OUTOFMEMORY :				return "メモリ不足。 ";
	case E_POINTER :					return "NULL ポインタ引数。 ";
	case VFW_E_CANNOT_CONNECT :			return "接続を確立する中間フィルタの組み合わせが見つからなかった。 ";
	case VFW_E_CANNOT_LOAD_SOURCE_FILTER :return "このファイルのソース フィルタをロードできない。 ";
	case VFW_E_CANNOT_RENDER :			return "ストリームをレンダリングするフィルタの組み合わせが見つからなかった。 ";
	case VFW_E_INVALID_FILE_FORMAT :	return "ファイル フォーマットが無効。 ";
	case VFW_E_NOT_FOUND :				return "オブジェクトまたは名前が見つからなかった。 ";
	case VFW_E_NOT_IN_GRAPH :			return "フィルタ グラフに存在しないオブジェクトに要求された関数を実行できない。";
	case VFW_E_UNKNOWN_FILE_TYPE :		return "このファイルのメディア タイプが認識されない。 ";
	case VFW_E_UNSUPPORTED_STREAM :		return "ファイルを再生できない。フォーマットがサポートされていない。";
	default:return "不明";
	}
}

/*!
*	@brief 動画再生
*
*	DirectShowにファイル名を渡して再生命令をするだけ。
*	動画の再生中は、Direct3Dのレンダリングを発生させるとちらつくので、
*	メインループのレンダリング開始・終了処理を停止して行うこと。
*
*	@param filename 再生ファイル名
*	@return FALSEは残念な結果に終わった
*/
BOOL CDirectSound::PlayVideo(const char *filename)
{
	if(!g_config.UseDShow())return(FALSE);

	BGMStop();
	if(!CreateDirectShow())return(FALSE);

	bgmon=FALSE;
	videoon=TRUE;
	bgmloop=FALSE;
	
	//言語設定をシステム(?)のものに設定. mbstowcsに影響
	setlocale(LC_ALL,"");

	//文字列変換
	char bgmfilename[256];
	wchar_t ubgmfilename[256];
	sprintf(bgmfilename,"%s",filename);
	mbstowcs( ubgmfilename, bgmfilename, strlen(bgmfilename)+1 );
    // グラフを構築する。
	if(pGraph->RenderFile(ubgmfilename, NULL) != S_OK){
		gbl.ods("CDSound::PlayVideo / RenderFile二失敗\n");
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
*	@brief BGM再生
*
*	DirectShowにファイル名を渡して再生命令をするだけ。
*	動画の再生中は、Direct3Dのレンダリングを発生させるとちらつくので、
*	メインループのレンダリング開始・終了処理を停止して行うこと。
*
*	@param filename 再生ファイル名
*	@param loop TRUEならば、再生終了後にループする
*	@return FALSEは残念な結果に終わった
*/
BOOL CDirectSound::BGMPlay(const char *filename,BOOL loop)
{
	if(!g_config.UseDShow())return FALSE;

	BGMStop();
	if(!CreateDirectShow())return(FALSE);

	bgmon=TRUE;
	videoon=FALSE;
	bgmloop=loop;

	//言語設定をシステム(?)のものに設定. mbstowcsに影響
	setlocale(LC_ALL,"");

	char *bgmfilename = new char[256];
	wchar_t *ubgmfilename= new wchar_t [256];


	// wma -------------------------------------------------------------
	sprintf(bgmfilename,"%s.wma",filename);
	mbstowcs( ubgmfilename, bgmfilename, strlen(bgmfilename)+1 );
	if(S_OK == pGraph->RenderFile(ubgmfilename, NULL)){
		pMediaControl->Run();
		// ボリュームを下げておく
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
		// ボリュームを下げておく
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
		// ボリュームを下げておく
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
		// ボリュームを下げておく
//		pAudio->put_Volume(-500);
		delete [] bgmfilename;
		delete [] ubgmfilename;
		return(TRUE);
	}

	gbl.ods("CDirectSound::BGMPlay : %s 再生失敗",filename);

	BGMStop();
	delete [] bgmfilename;
	delete [] ubgmfilename;
	return(FALSE);
}

/*!
*	@brief BGM再生終了ウインドウメッセージ取得
*
*	DirectShowはファイルの再生終了時にウインドウに対してメッセージを送信するようになっている。
*	この関数はBGM/動画 再生が終了したときのメッセージを受信したときに呼ばれ、ループ処理などを行う。
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
*	@brief DirectShow生成
*
*	DirectShowの生成・破棄は、BGMや動画の再生・停止時に毎回行っている。
*	もしかしたら結構な無駄なのかもしれない。
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
*	@brief BGM・動画 の再生停止
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
*	@brief BGM・動画 の再生一時停止
*/
void CDirectSound::BGMPause()
{
	if(!g_config.UseDShow())return;

	if (pMediaControl){
		pMediaControl->Pause();
	}
}

/*!
*	@brief BGM・動画 の一時停止再開
*/
void CDirectSound::BGMResume()
{
	if(!g_config.UseDShow())return;

	if (pMediaControl){
		pMediaControl->Run();
	}
}

/*!
*	@brief 開始位置を指定して再生
*
*	そのうちBGMPlayに統合するかも
*/
BOOL CDirectSound::BGMSeekAndPlay(const char* filename, BOOL loop, double starttime)
{
	if (!BGMPlay(filename, loop))
		return FALSE;

	LONGLONG startpos = (LONGLONG)(starttime * 10000.0);
	pSeek->SetPositions(&startpos, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
	
	return TRUE;
}

