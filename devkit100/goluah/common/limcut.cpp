
#include "stdafx.h"//なければwindows.h


/*-----------------------------------------------------------------------------
	以下、ネットでMACアドレス取得tipsを調べて出てきたサイトにあったコードをこぴぺした

	2004.02.27 aki
-------------------------------------------------------------------------------*/


/******************************************************************************
	Trick Library
		ゼロクリアモジュールヘッダファイル
			Copyright(C) 2000 Wraith.	All rights reserved.
											Coded by Wraith in Jul 17, 2000.
******************************************************************************/

//　Tab幅を４文字に設定して表示させてください。

//	tclear.h


#ifndef		__TrickLibrary_TCLEAR_H__
#define		__TrickLibrary_TCLEAR_H__

#if			defined(__WIN32__) || defined(_WIN32)
#include	<windows.h>
#else
#ifdef		_MSC_VER // compiler is VC.
#include	<memory.h>
#else
#include	<mem.h>
#endif
#endif

///////////////////////////////////////////////////////////////////////////////
//	ゼロクリア関数(テンプレート)
//

template <class T>
inline T & TClear_Core(T &A) {
#if			defined(__WIN32__) || defined(_WIN32)
    ZeroMemory((void *)&A, sizeof(T));
#else
	memset((void *)&A, 0, sizeof(T));
#endif
	return A;
}

template <class T>
inline T & TClear(T &A) {
	//
	//	実装をユーザコーディングでカスタマイズしやすいように
	//	ゼロクリアを行う本体(TClear_Core)をそとに出してます。
	//	カスタマイズするときは必要に応じてTClear_Coreを利用
	//	してください。
	//
	return TClear_Core(A);
}


///////////////////////////////////////////////////////////////////////////////
//	ゼロクリア＆構造体サイズ設定マクロ
//
#define	TClearEx(XXX, YYY)	(*(TClear_Core(XXX), (XXX).YYY = sizeof(XXX), &XXX))


#endif	//	__TrickLibrary_TCLEAR_H__


/******************************************************************************
	Trick Library
		ネットＢＩＯＳモジュールヘッダファイル
			Copyright(C) 2000 Wraith.	All rights reserved.
											Coded by Wraith in Aug 4, 2000.
******************************************************************************/

//　Tab幅を４文字に設定して表示させてください。

//	tnetbios.h


#ifndef		__TrickLibrary_TNETBIOS_H__
#define		__TrickLibrary_TNETBIOS_H__

//#include	"tclear.h"


#if			defined(__WIN32__) || defined(_WIN32)

#include	<nb30.h>
#include	<assert.h>
#include	<string.h>

#pragma comment(lib, "netapi32.lib")
//	↑この指定が有効でない環境では別途リンクを指定してください。

///////////////////////////////////////////////////////////////////////////////
//	ネットＢＩＯＳクラス
//

//
//	このクラスは手抜きをしてます。(^^ゞ
//	将来、きちんとしたものに修正する予定は一切ありませんが、気が向けば
//	それなりものに仕上げることがあるかもしれません。
//


class TNetbios :public NCB {

	public:
	
		UCHAR	ExecuteResult;
	
		//	コンストラクタ

				TNetbios() {
					//	ゼロクリア
					TClear(*this);
				}

		//	メンバ関数

TNetbios &		Clear() {
					return TClear(*this);
				}
TNetbios &		SetLanaNumber(const UCHAR &LanaNumber) {
					ncb_lana_num = LanaNumber;
					return *this;
				}
TNetbios &		SetCallName(const char *CallName) {
					assert(strlen(CallName) < NCBNAMSZ);
					strcpy((char *)ncb_callname, (const char *)CallName);
					return *this;
				}


TNetbios &		SetBuffer(void *Buffer, const int &Size) {
					ncb_buffer = (UCHAR *)Buffer;
					ncb_length = (WORD)Size;
					return *this;
				}
TNetbios &		SetBuffer(PLANA_ENUM Buffer) {
					return SetBuffer(Buffer, sizeof(*Buffer));
				}


TNetbios &		Execute(const UCHAR &Command) {
					ncb_command = Command;
					ExecuteResult = Netbios(this);
					return *this;
				}
TNetbios &		PostCommand(const UCHAR &Command) {
					return Execute(ASYNCH |Command);
				}

};


#endif	//	defined(__WIN32__) || defined(_WIN32)

#endif	//	__TrickLibrary_TNETBIOS_H__

/******************************************************************************
	□■□■ Wraith the Trickster ≫ http://mx1.tiki.ne.jp/~wraith/ □■□■
	■□■□ ～I'll go with heaven's advantage and fool's wisdom.～ ■□■□
******************************************************************************/

DWORD GetLimcutKey()
{

typedef struct _ASTAT_ {
    ADAPTER_STATUS adapt;
    NAME_BUFFER    NameBuff [30];
} ASTAT, * PASTAT;

	ASTAT       Adapter;
	LANA_ENUM   lenum;
	TNetbios    NetbiosAgent;


	//  LANA番号の列挙
/*	sprintf(tekito, "The NCBENUM return code is: 0x%x \n",
			NetbiosAgent
				.Clear()
				.SetBuffer(&lenum)
				.Execute(NCBENUM).ExecuteResult);
	OutputDebugString(tekito);*/
	NetbiosAgent
				.Clear()
				.SetBuffer(&lenum)
				.Execute(NCBENUM);

	//  全てのLANアダプタのステータスを表示
	//for(int i = 0; i < lenum.length; i++) {

	//0番だけ
	int i=0;
	{
		//  LANアダプタのリセット
	/*	sprintf(tekito, "The NCBRESET on LANA %d return code is: 0x%x \n",
				lenum.lana[i],
				NetbiosAgent
					.Clear()
					.SetLanaNumber(lenum.lana[i])
					.Execute(NCBRESET).ExecuteResult);
		OutputDebugString(tekito);*/
		NetbiosAgent
					.Clear()
					.SetLanaNumber(lenum.lana[i])
					.Execute(NCBRESET);

		//  LANアダプタのステータスを取得
	/*	sprintf(tekito, "The NCBASTAT on LANA %d return code is: 0x%x \n",
				lenum.lana[i],
				NetbiosAgent
					.Clear()
					.SetLanaNumber(lenum.lana[i])
					.SetCallName("*")
					.SetBuffer(&Adapter, sizeof(Adapter))
					.Execute(NCBASTAT).ExecuteResult);
		OutputDebugString(tekito);*/
		NetbiosAgent
					.Clear()
					.SetLanaNumber(lenum.lana[i])
					.SetCallName("*")
					.SetBuffer(&Adapter, sizeof(Adapter))
					.Execute(NCBASTAT);

		if (NRC_GOODRET != NetbiosAgent.ExecuteResult) {
			return 0;
		}
		//  ステータスを取得に成功...
			//  MACアドレスの表示
		/*	sprintf(tekito, "The Ethernet Number on LANA %d is:"
						"%02x%02x%02x%02x%02x%02x\n",
					lenum.lana[i],
					Adapter.adapt.adapter_address[0],
					Adapter.adapt.adapter_address[1],
					Adapter.adapt.adapter_address[2],
					Adapter.adapt.adapter_address[3],
					Adapter.adapt.adapter_address[4],
					Adapter.adapt.adapter_address[5]);
			OutputDebugString(tekito);
		}
		else OutputDebugString("The Ethernet Number on LANA %d is ...failed\n");*/
	}

	DWORD ret1 = Adapter.adapt.adapter_address[0]<<8 | Adapter.adapt.adapter_address[1];
	DWORD ret2 = Adapter.adapt.adapter_address[1]<<8 | Adapter.adapt.adapter_address[2];
	DWORD ret3 = Adapter.adapt.adapter_address[3]<<8 | Adapter.adapt.adapter_address[4];

	return( ( ret1 | ret2<<24 ) ^ ret3 );
}