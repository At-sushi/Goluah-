#include "define_const.h"
#include "..\resource.h"
#include "NetState.h"

/*!
*	@file
*	@brief DirectPlay
*/
#pragma once

/*!
*	@brief DirectPlayクラス
*	@ingroup DirectX
*
*	akiさんのDirectXクラスにあやかって作ってみた
*	参加メンバーの管理がまだできてないです。
*	NetState依存なので、commonなのに本体以外で使えない…
*/
class CDirectPlay
{
public:
	CDirectPlay();
	virtual ~CDirectPlay();

	HRESULT Initialize(const TCHAR* Name, const TCHAR* Port);//!<初期化
	HRESULT Destroy();//!<あぼんぬ
	BOOL IsReady()	{ return pDPlay != 0; }
	HRESULT Do(DWORD time)	{ return pThreadPool ? pThreadPool->DoWork(time, 0) : S_FALSE; }
	HRESULT SendMsg(DPNID dpnid, BYTE* pData, DWORD size, DWORD timeout, DWORD flags);//!<相手へデータ送信
	BOOL IsOnline()	{ return m_isonline; }//!<接続中かどうか
	BOOL IsHost()	{ return m_ishost; }//!<ホストとして参加してるかどうか
	void SetCallBack(PFNDPNMESSAGEHANDLER pCallBack)	{ pRemoteCallBack = pCallBack; }

protected:
	HRESULT OnMessage(PVOID UserCont, DWORD mtype, PVOID pmes);//!<DirectPlayメッセージ処理
	static HRESULT CALLBACK G_OnMessage(PVOID UserCont, DWORD mtype, PVOID pmes);
	wchar_t* GetName(DPNID dpnid, wchar_t* pBuffer);//!<指定したプレイヤーの名前を所得

	IDirectPlay8Peer* pDPlay;
	IDirectPlay8Address *pMyAddr, *pHostAddr;
	IDirectPlay8ThreadPool* pThreadPool;
	CNetState* m_pStateDlg;
	BOOL m_isonline, m_ishost;
	DPNHANDLE AHandle;
	PFNDPNMESSAGEHANDLER pRemoteCallBack;

public:
	//! 干す
	HRESULT Host(void);
	//! つなぐ
	HRESULT Connect(const TCHAR* IP, DWORD Port);
};


#define GNETMSG_CHAT	0xFF
