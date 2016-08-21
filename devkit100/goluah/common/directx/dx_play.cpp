#include "stdafx.h"

#include "global.h"	//g_muki[g_muki],g_config が必要
#include "goluah.h"
#include "dx_play.h"

/*!
*	@file
*	@brief DirectPlay
*
*	今思うとメンバ関数の概要とか先に書いとけば良かったと思う
*/

#ifdef _DEBUG
/**************************************************************************** * * DP8Sim simulated service providers * ****************************************************************************/
// {8D3F9E5E-A3BD-475b-9E49-B0E77139143C} 
const GUID CLSID_NETWORKSIMULATOR_DP8SP_TCPIP =
{ 0x8d3f9e5e, 0xa3bd, 0x475b, { 0x9e, 0x49, 0xb0, 0xe7, 0x71, 0x39, 0x14, 0x3c } };
#endif

// {D224CA9F-A3BC-4286-A2E4-383474BAC400}
const GUID g_guidApp =
{ 0xd224ca9f, 0xa3bc, 0x4286, { 0xa2, 0xe4, 0x38, 0x34, 0x74, 0xba, 0xc4, 0x0 } };

CDirectPlay::CDirectPlay()
{
	pDPlay = NULL;
	pMyAddr = NULL;
	pHostAddr = NULL;
	m_isonline = m_ishost = FALSE;
	m_pStateDlg = NULL;
}

CDirectPlay::~CDirectPlay()
{
	Destroy();
}

// 初期化
// 複数回作成消去が必要なので、コンストラクタとは別で。
HRESULT CDirectPlay::Initialize(const TCHAR* Name, const TCHAR* Port)
{
	HRESULT hr;

	Destroy();

	CoCreateInstance(CLSID_DirectPlay8Peer, NULL, CLSCTX_INPROC_SERVER,
		IID_IDirectPlay8Peer, (void**)&pDPlay);

	// しばらく使うよ
	if (SUCCEEDED(CoCreateInstance(CLSID_DirectPlay8ThreadPool, NULL, CLSCTX_INPROC_SERVER,
		IID_IDirectPlay8ThreadPool, (void**)&pThreadPool)))
	{
		pThreadPool->Initialize(this, G_OnMessage, 0);
		pThreadPool->SetThreadCount((DWORD)-1, 0, 0);
	}

	// 今回は簡略化
	if (!pDPlay || !pThreadPool)
	{
		MessageBox(NULL, _T("なんか無理っぽい"), _T("CDirectPlay エラー"), MB_OK | MB_ICONERROR);
		return DD_FALSE;
	}

	hr = pDPlay->Initialize(this, G_OnMessage, DPNINITIALIZE_HINT_LANSESSION);
	// アドレスっぽいもの所得
	if ( FAILED(hr = CoCreateInstance(CLSID_DirectPlay8Address, NULL, CLSCTX_INPROC_SERVER,
		 IID_IDirectPlay8Address, (void**)&pMyAddr)) )
		 return hr;
	pMyAddr->SetSP(&CLSID_DP8SP_TCPIP);
	// ポート設定
	if (Port[0] != '\0')
	{
		DWORD DPort = atoi(Port);
		hr = pMyAddr->AddComponent(
			DPNA_KEY_PORT, &DPort, sizeof(DWORD), DPNA_DATATYPE_DWORD);
	}

	// 名前とか設定
	DPN_PLAYER_INFO PlayerInfo;
	CStringW trans;

	ZeroMemory(&PlayerInfo, sizeof(PlayerInfo));
	PlayerInfo.dwSize = sizeof(DPN_PLAYER_INFO);
	PlayerInfo.dwInfoFlags = DPNINFO_NAME;
	trans = Name;
	PlayerInfo.pwszName = (PWSTR)trans.GetString();
	pDPlay->SetPeerInfo(&PlayerInfo, NULL, NULL, DPNSETPEERINFO_SYNC);

	return hr;
}

// 干す
HRESULT CDirectPlay::Host(void)
{
	DPN_APPLICATION_DESC dpdesc;
	HRESULT hr;
	CWaitCursor wc;

	ZeroMemory(&dpdesc, sizeof(dpdesc));

	dpdesc.dwSize = sizeof(DPN_APPLICATION_DESC);
	dpdesc.dwFlags = 0;
	dpdesc.guidApplication = g_guidApp;
	dpdesc.dwMaxPlayers = 2;

	if ( SUCCEEDED(hr = pDPlay->Host(&dpdesc, &pMyAddr, 1,
						NULL, NULL, NULL,
						DPNHOST_OKTOQUERYFORADDRESSING)) )
	{
		m_isonline = TRUE;
		m_ishost = TRUE;
		m_pStateDlg = new CNetState(NULL, &m_pStateDlg);
	}

	return hr;
}

// クライアントとして接続
HRESULT CDirectPlay::Connect(const TCHAR* IP, DWORD Port)
{
	HRESULT hr;
	CWaitCursor wc;

	if ( FAILED(hr = CoCreateInstance(CLSID_DirectPlay8Address, NULL, CLSCTX_INPROC_SERVER,
		 IID_IDirectPlay8Address, (void**)&pHostAddr)) )
		 return hr;
	pHostAddr->SetSP(&CLSID_DP8SP_TCPIP);

	hr = pHostAddr->AddComponent(DPNA_KEY_HOSTNAME, IP, strlen(IP) + 1, DPNA_DATATYPE_STRING_ANSI);
	hr = pHostAddr->AddComponent(
		DPNA_KEY_PORT, &Port, sizeof(DWORD), DPNA_DATATYPE_DWORD);

	DPN_APPLICATION_DESC dpdesc;

	ZeroMemory(&dpdesc, sizeof(dpdesc));
	dpdesc.dwSize = sizeof(dpdesc);
	dpdesc.guidApplication = g_guidApp;

	if ( SUCCEEDED(hr = pDPlay->Connect(&dpdesc, pHostAddr, pMyAddr,
						   NULL, NULL, NULL, 0, NULL,
						   NULL, NULL, DPNCONNECT_OKTOQUERYFORADDRESSING | DPNCONNECT_SYNC)) )
	{
		m_isonline = TRUE;
		m_ishost = FALSE;
		m_pStateDlg = new CNetState(NULL, &m_pStateDlg);
	}
	
	return hr;
}

// 今はただ、デストロイ＆デストロイ！！　　　－外山恒一－
HRESULT CDirectPlay::Destroy()
{
	HRESULT hr = S_OK;

	m_isonline = FALSE;
	if (m_pStateDlg)
		m_pStateDlg->DestroyWindow();

	if (pDPlay) hr = pDPlay->Close(0);
	RELEASE(pHostAddr);
	RELEASE(pMyAddr);

	if (pThreadPool) hr = pThreadPool->Close(0);
	RELEASE(pThreadPool);
	RELEASE(pDPlay);

	return hr;
}

// ==========================================================================================================

// メッセージ受信コールバック
HRESULT CALLBACK CDirectPlay::G_OnMessage(PVOID UserCont, DWORD mtype, PVOID pmes)
{
	CDirectPlay* pClass = (CDirectPlay*)UserCont;

	return pClass->OnMessage(UserCont, mtype, pmes);
}

// DirectPlayメッセージ処理
HRESULT CDirectPlay::OnMessage(PVOID UserCont, DWORD mtype, PVOID pmes)
{
	switch (mtype)
	{
	case DPN_MSGID_CREATE_PLAYER:
		{
			PDPNMSG_CREATE_PLAYER pMsg = (PDPNMSG_CREATE_PLAYER)pmes;
			break;
		}
	case DPN_MSGID_RECEIVE:
		{
			PDPNMSG_RECEIVE pMsg = (PDPNMSG_RECEIVE)pmes;

			if (*pMsg->pReceiveData == GNETMSG_CHAT)
			{
				CString data;	// ログに送る文章
				wchar_t name[256];

				// 名前と本文を組み立てて送信
				GetName(pMsg->dpnidSender, name);
				data = name;
				data += _T(" ＞ ");
				data += ((TCHAR*)pMsg->pReceiveData + 1);	// 最初に識別番号が入ってるので、抜く。
				m_pStateDlg->WriteLog(data);
			}
			break;
		}
	}

	// 別のコールバックを呼び出す
	return pRemoteCallBack ? (*pRemoteCallBack)(UserCont, mtype, pmes) : DPN_OK;
}

// 相手へデータ送信
HRESULT CDirectPlay::SendMsg(DPNID dpnid, BYTE* pData, DWORD size, DWORD timeout, DWORD flags)
{
	DPN_BUFFER_DESC dpbdesc;

	dpbdesc.pBufferData = pData;
	dpbdesc.dwBufferSize = size;

	return pDPlay->SendTo(dpnid, &dpbdesc, 1, timeout, NULL, &AHandle, flags);
}

// 指定したプレイヤーの名前を所得
wchar_t* CDirectPlay::GetName(DPNID dpnid, wchar_t* pBuffer)
{
	PDPN_PLAYER_INFO pdpi = 0;
	DWORD infosiz = 0;

	pDPlay->GetPeerInfo(dpnid, NULL, &infosiz, 0);
	pdpi = (PDPN_PLAYER_INFO)calloc(infosiz, 1);
	pdpi->dwSize = sizeof(DPN_PLAYER_INFO);
	pDPlay->GetPeerInfo(dpnid, pdpi, &infosiz, 0);

	wcsncpy(pBuffer, pdpi->pwszName, 256);
	free(pdpi);

	return pBuffer;
}
