#include "define_const.h"
#include "..\resource.h"

/*!
*	@file
*	@brief DirectPlay
*/
#pragma once

/*!
*	@brief DirectPlay�N���X
*	@ingroup DirectX
*
*	aki�����DirectX�N���X�ɂ��₩���č���Ă݂�
*	�Q�������o�[�̊Ǘ����܂��ł��ĂȂ��ł��B
*	NetState�ˑ��Ȃ̂ŁAcommon�Ȃ̂ɖ{�̈ȊO�Ŏg���Ȃ��c
*/
class CDirectPlay
{
public:
	CDirectPlay();
	virtual ~CDirectPlay();

	HRESULT Initialize(const char* Name, const char* Port);//!<������
	HRESULT Destroy();//!<���ڂ��
	BOOL IsReady()	{ return pDPlay != 0; }
	HRESULT Do(DWORD time)	{ return pThreadPool ? pThreadPool->DoWork(time, 0) : S_FALSE; }
	HRESULT SendMsg(DPNID dpnid, BYTE* pData, DWORD size, DWORD timeout, DWORD flags);//!<����փf�[�^���M
	BOOL IsOnline()	{ return m_isonline; }//!<�ڑ������ǂ���
	BOOL IsHost()	{ return m_ishost; }//!<�z�X�g�Ƃ��ĎQ�����Ă邩�ǂ���
	void SetCallBack(PFNDPNMESSAGEHANDLER pCallBack)	{ pRemoteCallBack = pCallBack; }

protected:
	HRESULT OnMessage(PVOID UserCont, DWORD mtype, PVOID pmes);//!<DirectPlay���b�Z�[�W����
	static HRESULT CALLBACK G_OnMessage(PVOID UserCont, DWORD mtype, PVOID pmes);
	wchar_t* GetName(DPNID dpnid, wchar_t* pBuffer);//!<�w�肵���v���C���[�̖��O������

	IDirectPlay8Peer* pDPlay;
	IDirectPlay8Address *pMyAddr, *pHostAddr;
	IDirectPlay8ThreadPool* pThreadPool;
	BOOL m_isonline, m_ishost;
	DPNHANDLE AHandle;
	PFNDPNMESSAGEHANDLER pRemoteCallBack;

public:
	//! ����
	HRESULT Host(void);
	//! �Ȃ�
	HRESULT Connect(const char* IP, DWORD Port);
};


#define GNETMSG_CHAT	0xFF
