#include "stdafx.h"

#include "global.h"	//g_muki[g_muki],g_config ���K�v
#include "dx_play.h"

/*!
*	@file
*	@brief DirectPlay
*
*	���v���ƃ����o�֐��̊T�v�Ƃ���ɏ����Ƃ��Ηǂ������Ǝv��
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
}

CDirectPlay::~CDirectPlay()
{
	Destroy();
}

// ������
// ������쐬�������K�v�Ȃ̂ŁA�R���X�g���N�^�Ƃ͕ʂŁB
HRESULT CDirectPlay::Initialize(const char* Name, const char* Port)
{
	HRESULT hr;

	Destroy();

	CoCreateInstance(CLSID_DirectPlay8Peer, NULL, CLSCTX_INPROC_SERVER,
		IID_IDirectPlay8Peer, (void**)&pDPlay);

	// ���΂炭�g����
	if (SUCCEEDED(CoCreateInstance(CLSID_DirectPlay8ThreadPool, NULL, CLSCTX_INPROC_SERVER,
		IID_IDirectPlay8ThreadPool, (void**)&pThreadPool)))
	{
		pThreadPool->Initialize(this, G_OnMessage, 0);
		pThreadPool->SetThreadCount((DWORD)-1, 0, 0);
	}

	// ����͊ȗ���
	if (!pDPlay || !pThreadPool)
	{
		MessageBox(NULL, "�Ȃ񂩖������ۂ�", "CDirectPlay �G���[", MB_OK | MB_ICONERROR);
		return DD_FALSE;
	}

	hr = pDPlay->Initialize(this, G_OnMessage, DPNINITIALIZE_HINT_LANSESSION);
	// �A�h���X���ۂ����̏���
	if ( FAILED(hr = CoCreateInstance(CLSID_DirectPlay8Address, NULL, CLSCTX_INPROC_SERVER,
		 IID_IDirectPlay8Address, (void**)&pMyAddr)) )
		 return hr;
	pMyAddr->SetSP(&CLSID_DP8SP_TCPIP);
	// �|�[�g�ݒ�
	if (Port[0] != '\0')
	{
		DWORD DPort = atoi(Port);
		hr = pMyAddr->AddComponent(
			DPNA_KEY_PORT, &DPort, sizeof(DWORD), DPNA_DATATYPE_DWORD);
	}

	// ���O�Ƃ��ݒ�
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

// ����
HRESULT CDirectPlay::Host(void)
{
	DPN_APPLICATION_DESC dpdesc;
	HRESULT hr;

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
	}

	return hr;
}

// �N���C�A���g�Ƃ��Đڑ�
HRESULT CDirectPlay::Connect(const char* IP, DWORD Port)
{
	HRESULT hr;

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
	}
	
	return hr;
}

// ���͂����A�f�X�g���C���f�X�g���C�I�I�@�@�@�|�O�R�P��|
HRESULT CDirectPlay::Destroy()
{
	HRESULT hr = S_OK;

	m_isonline = FALSE;

	if (pDPlay) hr = pDPlay->Close(0);
	RELEASE(pHostAddr);
	RELEASE(pMyAddr);

	if (pThreadPool) hr = pThreadPool->Close(0);
	RELEASE(pThreadPool);
	RELEASE(pDPlay);

	return hr;
}

// ==========================================================================================================

// ���b�Z�[�W��M�R�[���o�b�N
HRESULT CALLBACK CDirectPlay::G_OnMessage(PVOID UserCont, DWORD mtype, PVOID pmes)
{
	CDirectPlay* pClass = (CDirectPlay*)UserCont;

	return pClass->OnMessage(UserCont, mtype, pmes);
}

// DirectPlay���b�Z�[�W����
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
				CString data;	// ���O�ɑ��镶��
				wchar_t name[256];

				// ���O�Ɩ{����g�ݗ��Ăđ��M
				GetName(pMsg->dpnidSender, name);
				data = name;
				data += " �� ";
				data += ((char*)pMsg->pReceiveData + 1);	// �ŏ��Ɏ��ʔԍ��������Ă�̂ŁA�����B
			}
			break;
		}
	}

	// �ʂ̃R�[���o�b�N���Ăяo��
	return pRemoteCallBack ? (*pRemoteCallBack)(UserCont, mtype, pmes) : DPN_OK;
}

// ����փf�[�^���M
HRESULT CDirectPlay::SendMsg(DPNID dpnid, BYTE* pData, DWORD size, DWORD timeout, DWORD flags)
{
	DPN_BUFFER_DESC dpbdesc;

	dpbdesc.pBufferData = pData;
	dpbdesc.dwBufferSize = size;

	return pDPlay->SendTo(dpnid, &dpbdesc, 1, timeout, NULL, &AHandle, flags);
}

// �w�肵���v���C���[�̖��O������
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
