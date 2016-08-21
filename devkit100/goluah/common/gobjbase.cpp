/*=====================================================================================

Goluah!! Copyright (C) 2001-2004 aki, 2004-2016 At-sushi, 2014-2016 logger

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

=======================================================================================*/

/*=======================================================================================

	Goluah 基本クラス群

	CCharacterInfo	　キャラクター情報設定クラス
	CGoluahObject	　Goluahオブジェクト基本クラス
	CCharacterBase	　キャラクターオブジェクト基本クラス
	CBulletBase		　飛び道具基本クラス
	CClassicalBullet　旧飛道具構造体互換飛び道具クラス
	CBulletList		　飛道具リストクラス

=========================================================================================*/

#include <windows.h>
#include <stdio.h>

#ifdef USE_DIRECT3D_DIRECT
#include "d3dx9.h"
#endif

#include "gobjbase.h"

#ifndef RUNTIME_BUILD_OFF

DI_FUNCTIONS_S *funcs = NULL;
DI_FUNCTIONS_O *funco = NULL;
DI_FUNCTIONS_D *funcd = NULL;

#ifdef _DEBUG
size_t num_allocs = 0;
#endif

//デフォルトの設定ファイルパス
#define MYSOUND_LIST		"list.txt"
#define MYSOUND_PATH		"sound"
#define WIN_SERIFU_TXT		"serifu.txt"

#include <vector>
//#include <algorithm>

//global func
BOOL File2Mem(char* filename,char** buff,UINT *len);
int GetRandNum(int num);

//macro
#define DELETE_ARRAY(p)		if((p)){ delete [] (p); (p)=NULL; }

// 最低限交換性のあるバージョン
#define DTB_LEASTVER	1060
#define STB_LEASTVER	1060

/*! **************************************************************************

	@class CCharacterInfo
	@brief キャラクター情報設定クラス
	@ingroup GObjectBase

　ソフトがキャラクターを認識するために必要な情報を設定するクラスです。
　キャラクターDLL内で、このクラスをグローバル宣言し、
　SET_CHARACTERINFOマクロを使用してください。
*/
/* ************************************************************************** */

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
/*!
	@brief 構築
	@param CharName キャラクターの名前（最大31バイト）
	@param CdiVersion キャラクターDLLインターフェイスのバージョン
	@param Options オプション構造体へのポインタ
	@param num_Options オプションの数
	@param max_option_point オプション最大ポイント数
	@param isNetworkable ネット対戦対応かどうか
						 将来の拡張のために用意されている。現在はFALSEを渡さなければならない（笑）
	@param DtbVersion DLLテンプレートベースクラスのバージョン

	キャラクターの情報を設定してクラスを構築します。
	作った後は、特に問題がなければマクロ任せにしてしまいましょうヽ(ﾟ∀ﾟ)ﾉ。
*/
CCharacterInfo::CCharacterInfo(LPCTSTR CharName/* = "名無しさん" */, DWORD CdiVersion/* = CDI_VERSION */, CHARACTER_LOAD_OPTION* Options/* = NULL */,
		int num_Options/* = 0 */, DWORD max_option_point/* = 0 */, BOOL isNetworkable/* = FALSE */, DWORD DtbVersion/* = DTB_VERSION */)
{
	version = DtbVersion;
	if(version > DTB_VERSION || version < DTB_LEASTVER){ 
#		ifdef _DEBUG
			char dbgmsg[256];
			sprintf(dbgmsg,"CCharacterInfo:DTBバージョン違い(%d!=%d)\n",version,DTB_VERSION);
			OutputDebugString(dbgmsg);
#		endif
		m_Error=TRUE;
		return;
	}
	version = CdiVersion;

	m_Error = FALSE;

	m_MaxPoint = max_option_point;
	m_isNetworkable = isNetworkable;

	if (CharName) {
		int length = lstrlen(CharName);

		if (length >= 32) length = 32 - 1;
		charactername = new char[length + 1];
		if (charactername)
		{
			strncpy(charactername, CharName, length);
			charactername[length] = '\0';
		}
	}
	else charactername = NULL;

	if (Options) {
		m_Options = new CHARACTER_LOAD_OPTION[num_Options];
		if (m_Options) {
			for (int i = 0; i < num_Options; i++)
				m_Options[i] = *(Options + i);
			m_NumOptions = num_Options;
		}
		else m_NumOptions = 0;
	}
	else {
		m_NumOptions = 0;
		m_Options = NULL;
	}
}

//! あぼんぬ
CCharacterInfo::~CCharacterInfo()
{
	if (charactername)
		delete [] charactername;

	if (m_Options)
		delete [] m_Options;
}

/*!
	@brief CharacterInfo処理

	マクロから呼び出され、本体にキャラの情報を送信してます。
	意識して使う必要はあんまり無いです。
*/
BOOL CCharacterInfo::DllCharacterInfo(LPVOID info)
{
	if(m_Error)return FALSE;

	CDI_CHARACTERINFO *pif = (CDI_CHARACTERINFO*)info;
	if (charactername)
		lstrcpy(pif->name,charactername);//名前
	pif->ver=version;//バージョン

	if(pif->system_version!=0){
		if (m_isNetworkable)
			pif->caps |= CHARACTER_CAPS_NET;
		SetOptionItems(pif);
	}

	return(TRUE);
}

/*!
	@brief CreateaCharacter処理

	マクロから呼び出され、さっくりと初期化します。
	クラス構築はマクロがやってるし。
	これも、意識して使う必要は無いです。
*/
void CCharacterInfo::DllCreateCharacter(PVOID info)
{
	CDI_CHARACTERINFO2 *info2 = (CDI_CHARACTERINFO2*)info;

	funco = info2->funco;
	funcs = info2->funcs;
	funcd = info2->funcd;
}

/*!
	@brief オプション送信処理

	オプション情報を構造体からさばいて、本体に送信します。
	これまた、意識して使う必要は無いです。
*/
void CCharacterInfo::SetOptionItems(CDI_CHARACTERINFO* pif)
{
	pif->max_option_point = m_MaxPoint;

	for (int i = 0; i < m_NumOptions; i++)
		pif->options[i] = m_Options[i];
}

/*! **************************************************************************

	@class CGoluahObject
	@brief DLL向けオブジェクト基本クラス
	@ingroup GObjectBase

　	主な役割はオブジェクトの作成とメッセージ処理関数の設定。
　	よく使うメッセージに関しては関数がすでに定義されているので、それをオーバーライド。
　	足りない分はMessage関数をオーバーライドしてメッセージ処理を追加する。

	破棄はオブジェクトメッセージ、GOBJMSG_DELETEの受信時に行っているので、
	それ以外の場所で継承クラスをdeleteしてはいけません。
	また、メッセージ処理関数(GOBJECT.msghandler)がスタティックのgMessageToObject関数、
	ユーザーデータ(GOBJECT.pobjdat_u)が、クラスのポインタとして使用されているので、
	継承クラス側でこれらのデータを変更しないでください。
*/
/* ************************************************************************** */

CGoluahObject* CGoluahObject::pObjToDelete = NULL;
CGoluahObject* CGoluahObject::pObjDeleting = NULL;

/*!
	@brief 構築
	@param is_effect エフェクトオブジェクトかどうか。エフェクトオブジェクトはあたり判定を行いません。
					この処理は、オブジェクトフラグではなく、システム内部のオブジェクト管理機構によって処理されます。
					生成後に変更することはできません。
*/
CGoluahObject::CGoluahObject(BOOL is_effect/* = FALSE */)
{
	if(is_effect)
	{
		oid = (*funco->objcreate_fx)();//エフェクト用オブジェクトを生成
	}
	else
	{
		oid = (*funco->objcreate)();//オブジェクトを生成
	}

	pdat = (GOBJECT*)(*funco->getinfo)(oid);//データへのポインタをget
	if(pdat!=NULL){
		pdat->pobjdat_u = this;
		pdat->msghandler= gMessageToObject;//メッセージ処理関数
	}
	base_z				= 0;
	g_chardir = "";//?　…aki氏ｽﾏｿ、ただの初期化です。
}


/*!
	@brief 破棄

	破棄はメッセージ処理関数が GOBJMSG_DELETEを受信したときに行われるので、
	それ以外の場所でdeleteしてはいけません。
*/
CGoluahObject::~CGoluahObject()
{
	if (pObjToDelete == this)
		pObjToDelete = NULL;
	else if (pdat != NULL) {
		pObjDeleting = this;
		funco->objdelete(pdat->id);
	}
}

/*!
	@brief 基本メッセージ処理関数

	いくつかの基本的なメッセージに関して、仮想関数に処理を振り分けます。
*/
DWORD CGoluahObject::Message(DWORD msg,LPVOID pd,DWORD prm)
{
	switch(msg){
	case GOBJMSG_ACTION:	return(Action());
	case GOBJMSG_CNGAID:	ActionIDChanged();return(TRUE);
	case GOBJMSG_COMMAND:	Command();return(TRUE);
	case GOBJMSG_COMMANDCOM:return(CommandCOM(prm));
	case GOBJMSG_TOUCHA:	return(TouchA(pdat->atk2.info1,prm));
	case GOBJMSG_TOUCHB:	return(TouchB(pdat->atk,prm));
	case GOBJMSG_TOUCHC:	return(TouchC(pdat->atk,prm));
	case GOBJMSG_COMTHINK:	return(ComThink());
	case GOBJMSG_DRAW:		return(Draw());
	case GOBJMSG_DRAWFRONT:	return(DrawFront());
	case GOBJMSG_DRAWBACK:	return(DrawBack());
	case GOBJMSG_CNGROUND:	return(OnChangeRound());
	default:
		//あとのメッセージはデフォルト処理に任せる
		return(0);
	}

	return(TRUE);
}


/*!
	@brief 共通メッセージ処理関数

	全てのCGoluahObject継承クラスのオブジェクトは、メッセージ処理にこの関数を使います。
	また、GOBJECT.pobjdat_u はクラスのポインタとして使用されるので、継承クラスで
	この値を変更してはいけません。
*/
DWORD CGoluahObject::gMessageToObject(DWORD msg,LPVOID pd,DWORD prm)
{
	if(pd==NULL)return(0);
	GOBJECT *pdat = (GOBJECT*)pd;
	CGoluahObject *pc = (CGoluahObject*)(pdat->pobjdat_u);

	if(pc!=NULL){
		DWORD result;

		if (pObjDeleting == pc) {	// 手動解放時エラー防止
			if(msg==GOBJMSG_DELETE) pObjDeleting = NULL;
			return 1;	// デフォルト動作は行いません
		}

		// メッセージを処理
		result = pc->Message(msg,pd,prm);

		// 真・改ざんストッパー　書き換えられませんよ。。。（￣ー￣）ﾆﾔﾘｯ
		pdat->id = pc->oid;
		pdat->pobjdat_u = pc;
		pdat->msghandler = gMessageToObject;
		pc->pdat = pdat;

		if(msg==GOBJMSG_DELETE){
			#ifdef _DEBUG
				OutputDebugString("DLL GOBJMSG_DELETEメッセージ来ました\n");
			#endif
			pObjToDelete = pc;
			delete(pc);
		}
		return(result);
	}

	return(0);
}


/*!
	@brief GOBJMSG_ACTIONメッセージ処理関数

	基本的に1フレに1回呼び出されます。
	継承して、このなか（のさらに分岐した関数）でキャラクターや飛び道具等のアニメーションを記述します。
*/
DWORD CGoluahObject::Action(){return(TRUE);}

/*!
	@brief GOBJMSG_COMMANDメッセージ処理関数

	コマンド判定処理を行うために、基本的に1フレに1回呼び出されます。
	継承して、このなか（のさらに分岐した関数）でコマンド処理を実装します。
*/
void CGoluahObject::Command(){}

/*!
	@brief GOBJMSG_COMMANDCOMメッセージ処理関数

	コンピュータの行動遷移の可・不可を判定するために呼び出されます。
	現在の行動から、widへの行動遷移が可であれば0以外を、不可であれば0を返すような処理を記述してください。

	@param wid 技の行動ID
	@return 0:非許可, 0以外:許可
*/
DWORD CGoluahObject::CommandCOM(DWORD wid){return(FALSE);}

/*!
	@brief GOBJMSG_TOUCHAメッセージ処理関数
	@sa GOBJMSG_TOUCHA_RETURNS

	相手の攻撃が自分にあたったときに呼ばれます。
	キー入力や現在とっている行動などから判断し、ガード・喰らい等の適切な
	リターンコードを返すような処理を記述してください。

	@param info 当てられた攻撃の情報
	@param ta_eid 攻撃者のオブジェクトID
	@return TOUCHA_～のリターンコード
*/
DWORD CGoluahObject::TouchA(ATTACKINFO *info,DWORD ta_eid)
{
	return(0);//デフォルトのガード判定処理
}

/*!
	@brief GOBJMSG_TOUCHBメッセージ処理関数

	自分の攻撃が相手にあたったときに呼ばれます。
	通常はここでゲージの増加処理を実装します。
	CGoluahObject::TouchB は攻撃力の無効化を行います。

	@param info 当てた攻撃の情報
	@param hit TRUE:ヒットした, FALSE:ガードされた
	@return 常に0
*/
DWORD CGoluahObject::TouchB(ATTACKINFO *info,BOOL hit)
{
	pdat->kougeki=FALSE;//攻撃力の無効化
	return(0);
}

/*!
	@brief GOBJMSG_TOUCHCメッセージ処理関数
	@sa GOBJMSG_TOUCHC_RETURNS

	自分の攻撃が相手に当たり、なおかつ相手側にGOBJMSG_TOUCHAが送信される前に呼び出されます。
	通常はこのメッセージを処理する必要はありません。

	@param info 当てた攻撃の情報
	@param tc_eid 被害者のオブジェクトID
	@return TOUCHC_～のリターンコード
*/
DWORD CGoluahObject::TouchC(ATTACKINFO *info,DWORD tc_eid){return(0);}

/*!
	@brief GOBJMSG_TOUCHCメッセージ処理関数

	キャラクターがCOM操作で、COMが動作を決定する時に呼び出されます。
	この関数を使って、ユーザー独自の思考ルーチンを組むことが出来ます。

	@return 移行する動作のACTID
*/
DWORD CGoluahObject::ComThink() { return 0; }


/*-----------------------------------------------------------------------------
	そのたもろもろ
-------------------------------------------------------------------------------*/

/*!
	@brief GOBJMSG_CNGAIDメッセージ処理

	行動ID変化時の処理をここに記述します。
	CGoluahObject::ActionIDChanged は、z値(GOBJECT::z)を、基準z位置(CGoluahObject::base_z)に
	戻します。
*/
void CGoluahObject::ActionIDChanged()
{
	pdat->z=base_z;//z座標を基準値に戻す
}

/*!
	@brief GOBJMSG_CNGROUNDメッセージ処理関数

	試合ラウンド進行メッセージ。
	このメッセージの応答として0を返すと、そのオブジェクトは消去されます。
	通常はTRUEを返し、消去されないようにしてください。

	@return FALSE:オブジェクトの消去、TRUE:次のラウンドへ持ち越し
*/
BOOL CGoluahObject::OnChangeRound()
{
	return(TRUE);
}

/*!
	@brief オブジェクトの向きを判定してxオフセット値を作成

	オフセット量の生成です。
	例えば、飛び道具をキャラクターの基準座標から100"前"の位置から
	開始したい場合、GOBJECT::mukiの値を参照して +か- を判定しなければなりませんが、
	この関数はそれを考慮した値を返します。
	ヒットマークの発生位置オフセットなどにも使用します。

	@param x オフセット量(+は前方、-は後方)
	@return 向きを考慮したオフセット量
*/
double CGoluahObject::zurex(double x)
{
	if(pdat->muki)return(x*-1);
	else return(x);
}

/*!
	@brief オブジェクトの向きを判定してxオフセット値を作成

	double版のものを int にオーバーロードしたものです。
	double版を参照してください。
*/
int CGoluahObject::zurex(int x)
{
	if(pdat->muki)return(x*-1);
	else return(x);
}

/*!
	@brief オブジェクトの向きを判定してX方向移動

	キャラクターが右を向いているときと、キャラクターが左を向いているときでは、
	キャラクターを移動するときにx値に加えるべき値の符合が異なります。
	この関数はGOBJECT::mukiを考慮してキャラクターのx位置(GOBJECT::x)を移動します。

	@param dx 移動量(+は前方、-は後方)
*/
void CGoluahObject::movex(double dx)
{
	if(pdat->muki)dx*=-1;
	pdat->x += dx;
}

/*!
	@brief オブジェクトの向きを判定してX方向移動

	double版のものを int にオーバーロードしたものです。
	double版を参照してください。
*/
void CGoluahObject::movex(int dx)
{
	if(pdat->muki)dx*=-1;
	pdat->x += dx;
}

/*!
	@brief オブジェクトプロパティの追加
	@sa GOBJECT_PROPERTIES

	GOBJECT::objtypeに指定フラグを追加します。もともと設定されてあるフラグは保持されます。
	( pdat->objtype |= prop; )
	GOBJECT::objtypeを一括で指定したい場合はCGoluahObject::SetPropertyを使用してください。

	@param prop 追加するオブジェクトプロパティフラグ
*/
void CGoluahObject::AddProperty(DWORD prop)
{
	pdat->objtype |= prop;
}

/*!
	@brief オブジェクトプロパティの削除
	@sa GOBJECT_PROPERTIES

	GOBJECT::objtypeから指定フラグを除去します。指定されたフラグ以外のフラグは保持されます。
	( pdat->objtype &= ~prop; )

	@param prop 削除するオブジェクトプロパティフラグ
*/
void CGoluahObject::RemoveProperty(DWORD prop)
{
	pdat->objtype &= ~(prop);
}

/*!
	@brief オブジェクトプロパティの全設定
	@sa GOBJECT_PROPERTIES

	GOBJECT::objtypeに指定値を設定します。元々のフラグは全て失われます。
	( pdat->objtype = prop; )

	@param prop 設定するオブジェクトプロパティフラグ
*/
void CGoluahObject::SetProperty(DWORD prop)
{
	pdat->objtype = prop;
}

/*!
	@brief GOBJMSG_DRAWFRONTメッセージ関数
	@sa GOBJECT_MESSAGE

	GOBJMSG_DRAWFRONTメッセージに対応する処理です。
	通常はここでは描画処理を行いません。CGoluahObject::DrawFrontは常にFALSE（描画を行わない）
	を返します。

	@return TRUE:デフォルトのセル描画を行わせる,FALSE:デフォルト描画を行わない
*/
DWORD CGoluahObject::DrawFront(){return(FALSE);}

/*!
	@brief GOBJMSG_DRAWBACKメッセージ関数
	@sa GOBJECT_MESSAGE

	GOBJMSG_DRAWBACKメッセージに対応する処理です。
	通常はここでは描画処理を行いません。ステージオブジェクトはここで描画処理を行います。
	CGoluahObject::DrawFrontは常にFALSE（描画を行わない）を返します。

	@return TRUE:デフォルトのセル描画を行わせる,FALSE:デフォルト描画を行わない
*/
DWORD CGoluahObject::DrawBack(){return(FALSE);}

/*!
	@brief GOBJMSG_DRAWメッセージ処理
	@sa GOBJECT_MESSAGE

	GOBJMSG_DRAWBACKメッセージに対応する処理です。
	CGoluahObject::Drawは常にFALSE(デフォルトの描画を行う)を返します。
	通常のオブジェクトは、GCDのセル描画を行うため、この関数をオーバーライドする必要はありません。
	主にエフェクト等でD3Dを直接たたいて描画したい場合、この関数の処理を書き換えます。

	@return FALSE:デフォルトのセル描画を行わせる,TRUE:デフォルト描画を行わない
*/
DWORD CGoluahObject::Draw(){return(FALSE);}

/*!
	@brief 行動ID変更処理

	行動IDを指定値にセットして、パラメータのリセットを行います。

	GOBJMSG_ACTION と GOBJMSG_COMMAND メッセージ処理中に行動ID(GOBJECT.aid)を変更した場合は、
	システム側で変更を検知し、パラメータのリセットを行って GOBJMSG_CNGAIDメッセージ が送信されます。
	それ以外の場所で行動IDを変更した場合、パラメータのリセットは明示的に行わなければなりません。

	行動IDの変更は、値を直接代入するのではなく、この関数を使って行うのが安全です。

	@param actid 変更する新しい行動ID値
*/
void CGoluahObject::ChangeAction(DWORD actid)
{
	if(pdat->aid!=actid){
		pdat->aid=actid;
		funco->actidchanged(pdat->id);
	}
}

/*!
	@brief new処理

	delete処理が独自なので、ついでにこっちも。
*/
void* CGoluahObject::operator new(size_t size)
{
#	ifdef _DEBUG
		void* result = malloc(size);

		if (result)
			num_allocs += size;

		return result;
#	else
		return malloc(size);
#	endif //_DEBUG
}

/*!
	@brief delete処理

	アサーションがウザイので、自前で書きました。
*/
void CGoluahObject::operator delete(void* p, size_t size)
{
#		ifdef _DEBUG
			num_allocs -= size;
#		endif

	free(p);
}


/*-----------------------------------------------------------------------------
	システム関連の関数
-------------------------------------------------------------------------------*/

/*!
	@brief キー入力取得
	@sa GKEYSTATES
	@sa DI_FUNCTIONS_S::getkey
	@param keyinput キー入力インデックス値(CreateCharacter時に取得した値)
	@param interval 何フレーム前の入力を取得するか？(最大255) 0で現在フレームの入力を取得。
	@retuen キー入力コード。KEYSTA_～の定義を参照

	システムが保持するキー入力のログから、指定フレーム数分過去の値を取得します。
	通常は0(現在)の入力を取得します。
*/
DWORD CGoluahObject::GetKey(DWORD keyinput, DWORD interval) {
	return funcs->getkey(keyinput, interval);
}
/*!
	@brief キー入力検索
	@sa GKEYSTATES
	@sa DI_FUNCTIONS_S::seekkey
	@param keyinput キー入力インデックス値(CreateCharacter時に取得した値)
	@param offset 何フレーム前から検索を開始するか
	@param delay 何フレーム前まで検索対象とするか
	@return 見つかった場合、現在フレームから何フレ前かを返します。見つからなかった場合マイナス値

	システムの保持するキー入力ログから指定キー入力を検索します.
	コマンド判定を行うときに使用します。
*/
int CGoluahObject::SeekKey(DWORD keyinput, int offset, int delay, DWORD keystate) {
	return funcs->seekkey(keyinput,  offset,  delay, keystate);
}
/*!
	@brief 勝利台詞設定
	@sa DI_FUNCTIONS_S::setkatiserif
	@param tid どちらのチームか
	@param serif 設定する台詞文字列

	システムに勝利台詞文字列を通知して設定します。
	通常は、外部テキストから設定を読み込んで設定する、CCharacterBase::LoadAndSetKatiSerif
	を使用します。
*/
void CGoluahObject::SetKatiSerif(DWORD tid, char* serif) {
	funcs->setkatiserif(tid, serif);
}
/*!
	@brief システムエフェクト発生
	@sa DI_FUNCTIONS_S::addeffect
	@sa SYSTEM_EFFECT_IDs
	@param EffectID エフェクトのID(EFCTID_～)
	@param prm1 EffectIDにより可変
	@param prm2 EffectIDにより可変
	@param prm3 EffectIDにより可変

	システムエフェクトを発生させます。
	それぞれのエフェクトのIDと引数の意味はSYSTEM_EFFECT_IDsを参照。
*/
void CGoluahObject::AddEffect(DWORD EffectID, int prm1, int prm2, DWORD prm3) {
	funcs->addeffect(EffectID, prm1, prm2, prm3);
}
/*!
	@brief システムサウンド再生
	@sa DI_FUNCTIONS_S::playsyssound
	@sa SYSTEM_SOUND_IDs
	@param SoundNo システムサウンドのID

	システムで用意されているサウンドを再生する。
	引数はSYSTEM_SOUND_IDsを参照のこと。
*/
void CGoluahObject::PlaySysSound(DWORD SoundNo) {
	funcs->playsyssound(SoundNo);
}
/*!
	@brief wavサウンドロード
	@param pathname ファイル名(本体実行ファイルからの相対)
	@return 成功であればサウンドへのポインタ、失敗であればNULL
	@sa DI_FUNCTIONS_S::loadmysound
	@sa CGoluahObject::PlayMySound
	@sa CGoluahObject::KillMySound

	wavサウンドをロードする。
	戻り値で与えられたポインタは、必ずCGoluahObject::KillMySoundで破棄すること。
*/
LPVOID CGoluahObject::LoadMySound(char* pathname) {
	return funcs->loadmysound(pathname);
}
/*!
	@brief wavサウンド再生
	@param psound サウンドのポインタ
	@sa DI_FUNCTIONS_S::playmysound
	@sa CGoluahObject::LoadMySound

	LoadMySoundでロードしたwavサウンドを再生する。
*/
void CGoluahObject::PlayMySound(LPVOID psound) {
	funcs->playmysound(psound);
}
/*!
	@brief wavサウンド破棄
	@param psound サウンドのポインタ
	@sa DI_FUNCTIONS_S::killmysound
	@sa CGoluahObject::LoadMySound

	LoadMySoundでロードしたwavサウンドをアンロードする
*/
void CGoluahObject::KillMySound(LPVOID psound) {
	funcs->killmysound(psound);
}

/*!
	@brief システムへメッセージ送信
	@param mes メッセージID
	@param prm パラメータ
	@return メッセージIDに対応したリターンコード
	@sa DI_FUNCTIONS_S::msg2system
	@sa MESSAGE_OBJ2SYS

	システムへメッセージを送信します。送信できるメッセージの値とそれぞれの意味に関してはMESSAGE_OBJ2SYSを参照。
	試合進行に関係するイベントや、支援攻撃、交代等の要請などがこれにあたります。
*/
DWORD CGoluahObject::Message2System(DWORD mes, DWORD prm) {
	return funcs->msg2system(pdat->id, mes, prm);
}

/*!
	@brief 敵の名前を取得する
	@param tid 自分側のチームID
	@return 敵の名前文字列
	@sa DI_FUNCTIONS_S::getenemyname

	取得できる敵の名前は、対戦形式が同時対戦の場合不定です。
	それ以外の場合は最後にKOされたキャラクターの名前が取得されます。
*/
char* CGoluahObject::GetEnemyName(DWORD tid) {
	return funcs->getenemyname(tid);
}

/*!
	@brief オブジェクト所有キャラの名前を取得する
	@param oid オブジェクトID
	@return 指定キャラの名前文字列
	@sa DI_FUNCTIONS_O::getcharname
*/
char* CGoluahObject::GetCharacterName(DWORD oid) {
	return funco->getcharname(oid);
}

/*!
	@brief オブジェクト所有キャラのキー入力インデックス値を取得する
	@param oid オブジェクトID
	@return 指定キャラのキー入力インデックス値
	@sa DI_FUNCTIONS_O::getkeyinput
*/
DWORD CGoluahObject::GetKeyInput(DWORD oid) {
	return funco->getkeyinput(oid);
}


/*!
	@brief 現在の表示中心位置Xを取得
	@return 表示中心X座標
	@sa DI_FUNCTIONS_S::etdispcenterx

	ステージの表示位置はキャラクターの移動によって変化します。
	この関数はステージの現在の表示中心位置のX座標を取得します。
*/
double CGoluahObject::GetDisplayCenter_X() {
	return funcs->getdispcenterx();
}
/*!
	@brief 現在の表示中心位置Yを取得
	@return 表示中心Y座標
	@sa DI_FUNCTIONS_S::etdispcentery

	ステージの表示位置はキャラクターの移動によって変化します。
	この関数はステージの現在の表示中心位置のY座標を取得します。
*/
double CGoluahObject::GetDisplayCenter_Y() {
	return funcs->getdispcentery();
}
/*!
	@brief チームの人数を取得
	@return TEAM_PLAYER1側のチーム人数
	@sa CGoluahObject::GetTeamNum2
	@sa DI_FUNCTIONS_S::getteamnum

	以前は1P/2P側の人数が同一だったのでこの関数を使用しましたが、現在は
	違うので、この関数の使用は避けてください。
	代替機能としてCGoluahObject::GetTeamNum2を使用します。
*/
DWORD CGoluahObject::GetTeamNum() {
	return funcs->getteamnum();
}
/*!
	@brief 試合形式取得
	@return TAISENKEISIKI_～の値
	@sa TAISENKEISIKI
	@sa DI_FUNCTIONS_S::gettaisenkeisiki

	現在の試合形式を取得する。
*/
DWORD CGoluahObject::GetTaisenKeisiki() {
	return funcs->gettaisenkeisiki();
}
/*!
	@brief 敵のface変更値を取得する
	@param tid 自分側のチームID
	@return face変更値(通常0)
	@sa DI_FUNCTIONS_S::getenemyface

	敵のチームに複数のキャラクターが存在する場合、どのキャラクターが選択されるかは
	CGoluahObject::GetEnemyNameと同様です。
	この関数が返す値は通常0です。モララー/モララエルのように、オプション設定によって
	face2,3.bmp のイメージが変化するキャラクターの場合、オプション値を参照して
	どのイメージが使用されているかを返します。
	勝利台詞設定時などに利用します。
*/
BYTE CGoluahObject::GetEnemyFace(DWORD tid) {
	return funcs->getenemyface(tid);
}
/*!
	@brief チームの人数を取得
	@param tid 人数を取得するチームのチームID
	@return 指定チームの人数
	@sa DI_FUNCTIONS_S::getteamnum2

	指定チームの人数を取得します。
	tidには通常自分のチームID(GOBJECT::tid)を指定します。
	敵チームに関して調べたい場合はCGoluahObject::EnemyTIDを使用します。
*/
DWORD CGoluahObject::GetTeamNum2(DWORD tid)
{
	return funcs->getteamnum2(tid);
}

/*!
	@brief チームIDを敵側のチームIDに変換
	@param tid 味方側チームID
	@return 敵側チームID
	@sa DI_FUNCTIONS_S::tid2etid

	チームIDを敵側のチームIDに変換します。

*/
DWORD CGoluahObject::TID2EnemyTID(DWORD tid)
{
	return funcs->tid2etid(tid);
}
/*!
	@brief 敵側のチームIDを取得
	@return 敵のチームID

	自分のチームID(GOBJECT::tid)をCGoluahObject::TID2EnemyTID(DWORD tid)につっこんで
	敵側のチームIDを取します。
*/
DWORD CGoluahObject::EnemyTID()
{
	return TID2EnemyTID(pdat->tid);
}
/*!
	@brief ネットワークモードかどうかを取得する
	@return 現在は常にFALSE
	@sa DI_FUNCTIONS_S::is_net

	現在は常にFALSEが返ります
*/
BOOL CGoluahObject::IsNetwork(){
	return funcs->is_net();
}

/*!
*	@brief タグのセット
*	@param tag エラー発生時にログに出力される文字列
*	@sa DI_FUNCTIONS_S::pushtag
*
*	例外発生時、この関数によって設定された文字列がエラーログに追加されます。
*	例外を投げるバグの発生しそうな場所を PushTag , PopTag で囲むことによって、
*	エラー発生個所の特定に役立つかもしれない。
*	設定したタグはスタック式に保持されるので、この関数を複数回実行することも可能です。
*	システムはこの関数で渡された文字列のコピーは作成しないので、
*	この関数で設定する文字列は破棄されない場所に確保してください。
*/
void CGoluahObject::PushTag(const char* tag)
{
	funcs->pushtag( tag );
}

/*!
*	@brief タグの除去
*	@sa CGoluahObject::PushTag
*	@sa DI_FUNCTIONS_S::poptag
*
*	最後にPushされたタグを削除します
*/
void CGoluahObject::PopTag()
{
	funcs->poptag();
}

/*!
*	@brief ログ出力([error]) + 書式
*/
void CGoluahObject::LogError(const char* fmt,...)
{
	va_list args;
	va_start(args, fmt);
	char buffer[512];
	vsprintf(buffer, fmt, args);
	funcs->logerror( buffer );
	va_end(args);
}

/*!
*	@brief ログ出力([warning]) + 書式
*/
void CGoluahObject::LogWarning(const char* fmt,...)
{
	va_list args;
	va_start(args, fmt);
	char buffer[512];
	vsprintf(buffer, fmt, args);
	funcs->logwarning( buffer );
	va_end(args);
}

/*!
*	@brief ログ出力([debug]) + 書式
*/
void CGoluahObject::LogDebug(const char* fmt,...)
{
	va_list args;
	va_start(args, fmt);
	char buffer[512];
	vsprintf(buffer, fmt, args);
	funcs->logdebug( buffer );
	va_end(args);
}

/*!
*	@brief ログ出力([info]) + 書式
*/
void CGoluahObject::LogInfo(const char* fmt,...)
{
	va_list args;
	va_start(args, fmt);
	char buffer[512];
	vsprintf(buffer, fmt, args);
	funcs->loginfo( buffer );
	va_end(args);
}

/*!
* @brief BGMの一時停止
* @sa CGoluahObject::BGMResume
* @sa DI_FUNCTIONS_S::bgm_pause
*
* BGMの再生を一時停止します。BGMResumeで再開します。
* 独自BGMなどの演出に。
*/
void CGoluahObject::BGMPause()
{
	funcs->bgm_pause();
}

/*!
* @brief BGMの再開
* @sa CGoluahObject::BGMPause
* @sa DI_FUNCTIONS_S::bgm_resume
*
* BGMPauseで停止したBGMの再生を再開します。
*/
void CGoluahObject::BGMResume()
{
	funcs->bgm_resume();
}

/*!
* @brief fpsの取得
* @sa CGoluahObject::GetGameSpeed
* @sa DI_FUNCTIONS_S::getgamespeed
*
* fpsをint型で取得します。
* 正確な実時間が必要なときに。
*/
int CGoluahObject::GetGameSpeed()
{
	return funcs->getgamespeed();
}

/*!
* @brief  先取ポイント数の取得
* @sa CGoluahObject::GetMaxWin
* @sa DI_FUNCTIONS_S::getmaxwin
*
* 何本先取制なのか取得します。
*/
DWORD CGoluahObject::GetMaxWin()
{
	return funcs->getmaxwin();
}

/*-----------------------------------------------------------------------------
	オブジェクト関連の関数
-------------------------------------------------------------------------------*/
/*!
	@brief 投げとかで相手をつかむ
	@param eid つかみたいオブジェクトID
	@param msg_nage 投げが成功したとき、相手側の行動を制御するために使用するメッセージID
	@return TRUE:成功
	@sa DI_FUNCTIONS_O::objcatch
	@sa CATCHYOU

	成功した場合は相手が投げられ状態になるので、投げられ行動を適切に処理しないと
	試合進行に異常をきたすので注意してください。
*/
BOOL   CGoluahObject::ObjCatch(DWORD eid,DWORD msg_nage) {
	CATCHYOU cy;

	cy.oid = pdat->id;
	cy.actmsg = msg_nage;
	return funco->objcatch(eid, &cy);
}
/*!
	@brief オブジェクト情報(GOBJECT)取得
	@param oid 取得するオブジェクトのオブジェクトID
	@return 指定オブジェクトのGOBJECT構造体
	@sa DI_FUNCTIONS_O::getinfo

	指定IDのオブジェクトのオブジェクト情報を取得します。
*/
GOBJECT* CGoluahObject::GetInfo(DWORD oid) {
	return (GOBJECT*)funco->getinfo(oid);
}

/*!
	@brief 強制的にダメージを与える
	@param eid ダメージを受けるオブジェクトのID
	@param x 攻撃がヒットしたことにするx座標
	@param y 攻撃がヒットしたことにするy座標
	@sa DI_FUNCTIONS_O::adddamage

	主に投げのとき、投げ状態のオブジェクトにはあたり判定が行われていないので、
	この関数で相手にダメージを与えます。攻撃情報は自オブジェクトに設定されたものが使用されるので、
	普通に攻撃をするとき同様、StartAttackを使用して攻撃力情報を設定します。
*/
void   CGoluahObject::AddDamage(DWORD eid,int x,int y) {
	funco->adddamage(pdat->id, eid, x, y);
}
/*!
	@brief 行動IDが変化したときの処理を明示的に行う
	@sa DI_FUNCTIONS_O::actidchanged
	@sa ChangeAction

	GOBJMSG_ACTION と GOBJMSG_COMMAND メッセージ処理中に行動ID(GOBJECT.aid)を変更した場合は、
	システム側で変更を検知し、パラメータのリセットを行って GOBJMSG_CNGAIDメッセージ が送信されます。
	それ以外の場所で行動IDを変更した場合、パラメータのリセットは明示的に行わなければなりません。

	行動IDを変更した後でこの関数を呼び出すことにより、上記のような処理を行います。
	一応単体でも定義、普通に使うならChangeActionの使用をおすすめします。
*/
void   CGoluahObject::ActIDChanged() {
	funco->actidchanged(pdat->id);
}
/*!
	@brief 現在アクティブなキャラクターの情報を取得
	@param tid チーム
	@return 指定チームの現在キャラクターのオブジェクト情報
	@sa DI_FUNCTIONS_O::getactivechar

	"アクティブなキャラクター"は、対戦形式にもよりますが、
	基本的に画面に出て戦ってるキャラクターが返る、っぽい？あいまいな定義です。
*/
GOBJECT*  CGoluahObject::GetActiveCharacter(DWORD tid) {
	return (GOBJECT*)funco->getactivechar(tid);
}
/*!
	@brief 現在アクティブな敵キャラクターの情報
	@return 敵チームの現在キャラクターのオブジェクト情報

	CGoluahObject::GetActiveCharacter 参照
*/
GOBJECT*  CGoluahObject::GetActiveEnemy() {
	return GetActiveCharacter( EnemyTID() );
}
/*!
	@brief オブジェクト消滅
	@sa DI_FUNCTIONS_O::suicide

	オブジェクトを破棄します。
	注意：「エフェクト扱い」ではないオブジェクトをこの関数で破棄するのは非推奨です。
*/
void   CGoluahObject::Suicide() {
	funco->suicide(pdat->id);
}

/*!
	@brief 間合いをget(水平)
	@param oid オブジェクトその1
	@param eoid オブジェクトその2
	@return 指定2オブジェクト間の間合い
	@sa DI_FUNCTIONS_O::getmaai_h

	間合いは重なり判定の距離で計算されます。
	オブジェクトが回転を含む場合、正しい値は得られません。
*/
DWORD  CGoluahObject::GetMaai_H(DWORD oid, DWORD eoid) {
	return funco->getmaai_h(oid, eoid);
}
/*!
	@brief 間合いをget(垂直)
	@param oid オブジェクトその1
	@param eoid オブジェクトその2
	@return 指定2オブジェクト間の間合い
	@sa DI_FUNCTIONS_O::getmaai_v
*/
DWORD  CGoluahObject::GetMaai_V(DWORD oid, DWORD eoid) {
	return funco->getmaai_v(oid, eoid);
}

/*!
	@brief キャラクターのオブジェクトID
	@param tid チームID
	@param index チーム内でのインデックス(0～)
	@return キャラクターのオブジェクトID
	@sa DI_FUNCTIONS_O::getcharid
*/
DWORD  CGoluahObject::GetCharacterID(DWORD tid,DWORD index) {
	return funco->getcharid(tid, index);
}
/*!
	@brief COMレベル取得
	@return COM制御の場合のレベル(難易度)
	@sa DI_FUNCTIONS_O::getcomlevel

	COMのレベルは0～7までの8段階です。
	システムによる制御に加えて、DLL側でコンピュータに連続技をやらせる場合に
	参考にしてください。
	また、連続技をやらせる場合には CGoluahObject::SetComAct を推奨します
*/
DWORD  CGoluahObject::GetComLevel(){
	return funco->getcomlevel(oid);
}
/*!
	@brief COMリーチ設定
	@param idx COMIDX_NEAR:短,COMIDX_MIDDLE:中,COMIDX_LONG:長
	@param length 設定するリーチ長
	@sa DI_FUNCTIONS_O::setcomreach

	COM判定のリーチ長を設定します。
	システム側で技情報構造体に設定された技のうち、どれを選択するかの判断に影響します。
	設定を行わない場合はver0.8以前で使用されていた固定値が使用されます。
*/
void   CGoluahObject::SetComReach(DWORD idx,int length){
	funco->setcomreach(oid,idx,length);
}
/*!
	@brief COM行動設定
	@param aid 次に起こすべき行動の行動ID(0の場合行動を抑制する)
	@param delay 発動遅延時間
	@sa DI_FUNCTIONS_O::setcomact

	COMが次に起こす行動を指定します。連続技をやらせるときなどに使用します。
	この関数で指定した発動遅延時間の間は別の行動に移行しません（ガード・喰らい等を除く）
	ただし、ガードくらい等の攻撃不能な行動に遷移した場合はこの設定はクリアされます。
*/
void   CGoluahObject::SetComAct(DWORD aid,int delay){
	funco->setcomact(oid,aid,delay);
}
/*!
	@brief COM自動行動抑制
	@param delay 抑制する時間
	@sa SetComAct

	COMの行動遷移を指定した時間の間抑制します。
	行動遷移が起こってから攻撃力が発生するまでに時間のかかる技の場合、
	その間に別の技をキャンセルして出さないようにディレイを持たせるために使用します。
*/
void   CGoluahObject::ComDelay(int delay){
	SetComAct(0,delay);
}
/*!
	@brief オブジェクトがローカルであるかどうか
	@return 現在は常にTRUE

	もし仮にネットワーク対戦の機能が実装された場合、オブジェクトのあたり判定等は、
	そのオブジェクトがローカルなシステムの管理のものであるかどうかを判断する必要があります。
*/
BOOL CGoluahObject::IsLocal(){
	return funco->obj_is_local(oid);
}

void CGoluahObject::dact_damages1(GOBJECT *p)		{funco->dact_damages1(p);}
void CGoluahObject::dact_damages2(GOBJECT *p)		{funco->dact_damages2(p);}
void CGoluahObject::dact_damages3(GOBJECT *p)		{funco->dact_damages3(p);}
void CGoluahObject::dact_damagec1(GOBJECT *p)		{funco->dact_damagec1(p);}
void CGoluahObject::dact_damagec2(GOBJECT *p)		{funco->dact_damagec2(p);}
void CGoluahObject::dact_damagec3(GOBJECT *p)		{funco->dact_damagec3(p);}
void CGoluahObject::dact_damagej1(GOBJECT *p)		{funco->dact_damagej1(p);}
void CGoluahObject::dact_damagej2(GOBJECT *p)		{funco->dact_damagej2(p);}
void CGoluahObject::dact_damagej3(GOBJECT *p)		{funco->dact_damagej3(p);}
void CGoluahObject::dact_damages1a(GOBJECT *p)		{funco->dact_damages1a(p);}
void CGoluahObject::dact_damages2a(GOBJECT *p)		{funco->dact_damages2a(p);}
void CGoluahObject::dact_damages3a(GOBJECT *p)		{funco->dact_damages3a(p);}
void CGoluahObject::dact_damagec1a(GOBJECT *p)		{funco->dact_damagec1a(p);}
void CGoluahObject::dact_damagec2a(GOBJECT *p)		{funco->dact_damagec2a(p);}
void CGoluahObject::dact_damagec3a(GOBJECT *p)		{funco->dact_damagec3a(p);}
void CGoluahObject::dact_guards1(GOBJECT *p)		{funco->dact_guards1(p);}
void CGoluahObject::dact_guards2(GOBJECT *p)		{funco->dact_guards2(p);}
void CGoluahObject::dact_guards3(GOBJECT *p)		{funco->dact_guards3(p);}
void CGoluahObject::dact_guardc1(GOBJECT *p)		{funco->dact_guardc1(p);}
void CGoluahObject::dact_guardc2(GOBJECT *p)		{funco->dact_guardc2(p);}
void CGoluahObject::dact_guardc3(GOBJECT *p)		{funco->dact_guardc3(p);}
void CGoluahObject::dact_guardj1(GOBJECT *p)		{funco->dact_guardj1(p);}
void CGoluahObject::dact_guardj2(GOBJECT *p)		{funco->dact_guardj2(p);}
void CGoluahObject::dact_guardj3(GOBJECT *p)		{funco->dact_guardj3(p);}
void CGoluahObject::dact_down(GOBJECT *p)			{funco->dact_down(p);}
void CGoluahObject::dact_down2(GOBJECT *p)			{funco->dact_down2(p);}
void CGoluahObject::dact_okiagari(GOBJECT *p)		{funco->dact_okiagari(p);}
void CGoluahObject::dact_futtobi(GOBJECT *p)		{funco->dact_futtobi(p);}
void CGoluahObject::dact_futtobi2(GOBJECT *p)		{funco->dact_futtobi2(p);}
void CGoluahObject::dact_tatakituke1a(GOBJECT *p)	{funco->dact_tatakituke1a(p);}
void CGoluahObject::dact_tatakituke1b(GOBJECT *p)	{funco->dact_tatakituke2b(p);}
void CGoluahObject::dact_tatakituke2a(GOBJECT *p)	{funco->dact_tatakituke1a(p);}
void CGoluahObject::dact_tatakituke2b(GOBJECT *p)	{funco->dact_tatakituke2b(p);}

/*!
	@brief COMレンジ設定
	@param idx COMIDX_NEAR:短,COMIDX_MIDDLE:中,COMIDX_LONG:長
	@sa DI_FUNCTIONS_O::setcomrange

	COM判定で基本となる間合いを設定します。
	まだ試験中です、使わない方が良さげ。
*/
void   CGoluahObject::SetComRange(DWORD idx){
	funco->setcomrange(oid,idx);
}

/*!
	@brief 連続技ヒット数（被コンボ数）取得
	@param oid オブジェクトID
	@return 指定オブジェクトのhitcount
	@sa DI_FUNCTIONS_O::gethitcount

	指定IDのオブジェクトの連続技ヒット数（被コンボ数）を取得します。
	コンボ数は加害者ではなくて被害者が持っています。
*/
DWORD CGoluahObject::GetHitCount(DWORD oid) {
	return funco->gethitcount(oid);
}
/*!
	@brief 連続技蓄積ダメージ（被コンボダメージ）取得
	@param oid オブジェクトID
	@return 指定オブジェクトのsexydamage
	@sa DI_FUNCTIONS_O::getsexydamage

	指定IDのオブジェクトの連続技蓄積ダメージ（被コンボダメージ）を取得します。
	これも被害者側。
*/
DWORD CGoluahObject::GetSexyDamage(DWORD oid) {
	return funco->getsexydamage(oid);
}

/*-----------------------------------------------------------------------------
	描画関連の関数
-------------------------------------------------------------------------------*/

/*!
	@brief IDirect3D* の取得
	@sa DI_FUNCTIONS_D::getd3d

	IDirect3D*を取得します。取得しても使い道ないかもしれないけど...
*/
LPDIRECT3D9 CGoluahObject::GetD3D() {
	return (LPDIRECT3D9)funcd->getd3d();
}
/*!
	@brief IDirect3DDevice* の取得
	@sa DI_FUNCTIONS_D::getd3dd

	IDirect3DDevice* を取得します。
	IDirect3DDevice*によって可能な操作はDirectXのSDKを参照してください。
*/
LPDIRECT3DDEVICE9 CGoluahObject::GetD3DDevice() {
	return (LPDIRECT3DDEVICE9)funcd->getd3dd();
}
/*!
	@brief ver0.70形式セルデータ読み込み(非推奨)
	@sa CGoluahObject::CreateCellData
	@sa DI_FUNCTIONS_D::loadcelldat
	@param pathname [in] 読み込むファイル名
	@param pCells [out] 結果を格納するCELLデータ構造体配列
	@param pRects [out] 結果を格納する矩形データ構造体配列
	@param pHanteis [out] 結果を格納するあたり判定データ構造体配列

	ver0.70形式セルデータをロードします。この関数はDLL側が所持する固定長のメモリ領域へ
	ロードしたデータを書き込むため、将来GCDデータのデータ長が変化した場合には対応することが
	できません。この問題は出来上がったキャラクターのGCDデータが将来の新しいGCDエディタで
	編集された場合に起こりますが、換装キャラクターを許さないのであれば特に問題ではないかもしれません。

	この関数ではver0.9形式のGCDデータを読み込むことはできません。
	かわりにCGoluahObject::CreateCellDataを使用してください。
*/
void   CGoluahObject::LoadCellData(char* pathname,GCD_CELL2_070* pCells,GCD_RECT* pRects,GCD_HANTEI* pHanteis) {
	funcd->loadcelldat(pathname, pCells, pRects, pHanteis);
}
/*!
	@brief ビットマップ読み込み
	@sa DI_FUNCTIONS_D::loadbmp
	@param PathName	画像データを読み込むファイル名
	@param PalletFileName パレットデータを読み込むファイル名
	@return 読み込んだ画像.NULLは失敗

	8ビットインデックスカラーの無圧縮ビットマップをロードします。
	PathNameのファイルが持つパレットは通常無視されます。
	ロードしたイメージは必ずCGoluahObject::UnloadBitMapで破棄してください。
*/
MYSURFACE* CGoluahObject::LoadBitmap(char* PathName,char* PalletFileName) {
	return (MYSURFACE*)funcd->loadbmp(PathName, PalletFileName);
}
/*!
	@brief ビットマップ後始末
	@sa DI_FUNCTIONS_D::unloadbmp
	@sa CGoluahObject::LoadBitmap
	@param Bitmap CGoluahObject::LoadBitmapで読み込んだビットマップ

	読み込んだビットマップをアンロードします
*/
void   CGoluahObject::UnloadBitMap(MYSURFACE* Bitmap) {
	funcd->unloadbmp(Bitmap);
}
/*!
	@brief セル描画
	@param pBmps 使用するビットマップ配列
	@param cdat 使用するセルデータ配列
	@param rdat 使用する矩形データ配列
	@param cnum 描画するセル番号
	@param x 描画先x座標
	@param y 描画先y座標
	@param z 描画先z座標
	@param Rotate 回転角(degree)
	@param ReverseX x反転
	@param ReverseY y反転
	@param Color 描画の頂点Diffuse色
	@param magx x拡大率
	@param magy y拡大率
	@param shadowed 影(省略可)

	セル描画を行います。
	通常はオブジェクトメッセージGOBJMSG_DRAWでFALSEを返すことにより、
	オブジェクトデータ(GOBJECT)からそれぞれ適当なパラメータが拾われて描画されますが、
	1オブジェクトで一度に２つ以上のセルを描画したい場合などにこの関数を利用することができます。

	@sa DI_FUNCTIONS_D::celldraw
	@sa CGoluahObject::SetTransform
	@sa CGoluahObject::SetParentMatrix
*/
void   CGoluahObject::CellDraw(MYSURFACE** pBmps,GCD_CELL2* cdat,GCD_RECT* rdat,
	DWORD cnum,int x,int y,float z,int Rotate,BOOL ReverseX,BOOL ReverseY,DWORD Color,float magx,float magy,BOOL shadowed) 
{
	funcd->celldraw((void**)pBmps, cdat, rdat,
	 cnum, x, y, z, Rotate, ReverseX, ReverseY, Color, magx, magy, shadowed);
}
/*!
	@brief セル描画
	@sa CGoluahObject::SetTransform
	@sa CGoluahObject::SetParentMatrix
	
	通常はオブジェクトメッセージGOBJMSG_DRAWでFALSEを返すことにより同様の描画が行われますが、
	1オブジェクトで一度に２つ以上のセルを描画したい場合などにこの関数を利用することができます。
*/
void   CGoluahObject::CellDraw(GOBJECT *objdat,BOOL shadowed)
{
	funcd->celldraw(
		(void**)objdat->pmsarr,
		objdat->pcdat,
		objdat->prdat,
		objdat->cnow,
		(int)objdat->x,
		(int)objdat->y,
		objdat->z,
		objdat->rot,
		objdat->muki ? (!objdat->revx) : objdat->revx,
		objdat->revy,
		objdat->color,
		objdat->magx,
		objdat->magy,
		shadowed
		);
}
/*!
	@brief ビットマップ描画
	@param pBmp 転送元ビットマップ
	@param x1 転送先の左上のx座標値
	@param y1 転送先の左上のy座標値
	@param bltrect 転送元矩形
	@param magx x拡大率
	@param magy y拡大率
	@param revx x反転
	@param revy y反転
	@param z 転送先z座標値
	@param color 描画の頂点Diffuse色

	CGoluahObject::LoadBitmapでロードしたビットマップを描画します。
	描画はz軸に対して垂直な平面になります。

	@sa DI_FUNCTIONS_D::ckblt
	@sa CGoluahObject::SetTransform
	@sa CGoluahObject::SetBlend
	@sa CGoluahObject::SetParentMatrix
*/
void   CGoluahObject::CkBlt(MYSURFACE* pBmp,int x1,int y1,RECT bltrect,
	double magx,double magy,BOOL revx,BOOL revy,float z,DWORD color)
{
	funcd->ckblt(pBmp, x1, y1, bltrect,
	 magx,  magy, revx, revy, z, color);
}
/*!
	@brief ビットマップ描画(2)

	@param pBmp 転送元ビットマップ
	@param bltrect 転送元矩形
	@param rect 転送先矩形
	@param color 描画の頂点Diffuse色

	@sa DI_FUNCTIONS_D::blt3d
	@sa CGoluahObject::SetTransform
	@sa CGoluahObject::SetBlend
	@sa CGoluahObject::SetParentMatrix

	CGoluahObject::LoadBitmapでロードしたビットマップを描画します。
*/
void   CGoluahObject::Blt3D(MYSURFACE* pBmp,RECT bltrect,MYRECT3D rect,DWORD color)
{
	funcd->blt3d( pBmp, bltrect, rect, color);
}
/*!
	@brief 描画時座標変換設定
	@sa DI_FUNCTIONS_D::settrans
	@param BonVoyage TRUE:通常,FALSE:スクリーン座標系。何この変数名？

	通常、ステージのうち描画される範囲はキャラクターの移動によって変化しますが、
	この関数でFALSEを指定するとウィンドウの左上を(0,0)とした座標系で描画されるようになります。
	変更した場合は、描画後にTRUEに戻しておくこと。
*/
void   CGoluahObject::SetTransform(BOOL BonVoyage) {
	funcd->settrans(BonVoyage);
}
/*!
	@brief αブレンドの仕方を変更
	@sa DI_FUNCTIONS_D::setblend
	@sa GBLENDMODES

	α合成の方法を指定できます。ビットマップ描画に影響します。
	変更した場合は、描画後にGBLEND_HANTOUMEIに戻しておくこと。
*/
void   CGoluahObject::SetBlend(DWORD type) {
	funcd->setblend(type);
}
/*!
	@brief 描画時座標変換行列設定
	@sa DI_FUNCTIONS_D::setparentmat

	@param pMatrix [in]設定する変換行列
	@param erase [in]FALSEの場合、新規に設定される行列は元の行列とpMatrixの積
	@param OldMatrix [out]関数の実行前に設定されていた変換行列が返されます

	座標変換マトリクスを指定します。
	ビットマップ描画・セル描画の際にこれらの座標変換が適用されるので、
	うまく使えば超多機能な描画ができるはず。所詮は平面ですけどね。
	変更した場合はOldMatrixで返される元の変換行列に戻します。
	通常は単位行列が設定されています。
*/
void   CGoluahObject::SetParentMatrix(LPD3DXMATRIX pMatrix,BOOL erase,LPD3DXMATRIX OldMatrix) {
	funcd->setparentmat( pMatrix, erase, OldMatrix);
}
/*!
	@brief GCDデータロード
	@sa CGoluahObject::DestroyCellData
	@sa DI_FUNCTIONS_D::create_celldat

	将来GCDデータの構造体が変更される可能性に対応するため、
	システム側で配列メモリを確保してそこにデータをロードします。
	そのため、この関数でロードしたデータを動的に変更することは非推奨となります。
	変更を行う場合、将来GCDのフォーマットが変更され、
	ユーザーがGCDをいじったときに誤動作を起こす可能性があります。（別にかまわないかも?）
*/
DWORD  CGoluahObject::CreateCellData(char* filename,GCD_CELL2 **cdat,GCD_RECT **rdat,GCD_HANTEI **hdat) {//セルデータ読み込み。ver0.90以降ではLoadCellDatからこちらに変更するべき
	return funcd->create_celldat(filename, (void**)cdat, (void**)rdat, (void**)hdat);
}
/*!
	@brief GCDデータ破棄
	@sa CGoluahObject::CreateCellData
	@sa DI_FUNCTIONS_D::destroy_celldat

	CGoluahObject::CreateCellDataでロードしたGCDデータを破棄します。
*/
void   CGoluahObject::DestroyCellData(GCD_CELL2 **cdat,GCD_RECT **rdat,GCD_HANTEI **hdat) {//CreateCellDatで生成されたバッファをクリア
	funcd->destroy_celldat( (void**)cdat, (void**)rdat, (void**)hdat);
}
/*!
	@brief PNG/BMP形式画像読み込み
	@sa DI_FUNCTIONS_D::loadimage
	@sa LoadBitmap
	@param PathName	画像データを読み込むファイルの、拡張子を除いたファイル名
	@param PalletFileName パレットデータを読み込むファイルの、拡張子を除いたファイル名
	@return 読み込んだ画像.NULLは失敗

	PNG対応に伴い、読み込み処理に柔軟性を求めるために作りました。
	拡張子を除いたファイル名から、.pngと.bmpの２種類のタイプから自動選択して読み込みます(PNG優先）。
	あとは、ほとんどLoadBitmapと変わりません。
*/
MYSURFACE* CGoluahObject::LoadImage(char* PathName, char* PalletFileName) {
	return (MYSURFACE*)funcd->loadimage(PathName, PalletFileName);
}
/*!
	@brief GCDデータロード（２）
	@sa CGoluahObject::DestroyCellData
	@sa DI_FUNCTIONS_D::create_celldat2
	@sa CreateCellData

	GCDのが圧縮に対応し、拡張子が増えたことから追加された関数です。
	拡張子を除いたファイル名から、.gcm（圧縮）と.gcd（非圧縮）の２種類のタイプから自動選択して読み込みます(圧縮側優先）。
	それ以外は、CreateCellDataと変わりません。
*/
DWORD  CGoluahObject::CreateCellData2(char* filename,GCD_CELL2 **cdat,GCD_RECT **rdat,GCD_HANTEI **hdat) {//セルデータ読み込み。ver0.90以降ではLoadCellDatからこちらに変更するべき
	return funcd->create_celldat2(filename, (void**)cdat, (void**)rdat, (void**)hdat);
}


/*!
	@class CCharacterBase
	@brief キャラクター基本クラス
	@ingroup GObjectBase

	DLLのメインキャラクターを作成するためのクラスです。
	GObjectBaseに、キャラクターが使用するであろう(と思われる)もろもろの機能を追加。

	Init～() 系の処理は InitializeCharacter関数からコールされます。
	メインキャラクター以外を生成する場合は、newしたあとにInitializeCharacter
	をコールする必要があります。

	CBulletBaseの生成時に"親"クラスとしてこのクラスを指定すると、CBulletBaseは
	描画時にこのクラス(オブジェクト)がロードしたビットマップ , GCDデータ, サウンド等を
	利用します。
*/

/*!
	@brief 構築(1)
	@param info CreateCharacterの引数でシステムから受け取った構造体
*/
CCharacterBase::CCharacterBase(CDI_CHARACTERINFO2 *info) : CGoluahObject(FALSE)
{
	parent_char = NULL;

	GetGObject()->tid	= info->tid;//チームID
	keyinput			= info->keyinput;//何番からキー入力を受け取るか
	pal_number			= info->color;//パレット番号
	g_chardir			= info->dir;
	option_flags		= info->options_flag;

	CCharacterBaseCreate();
}

/*!
	@brief 構築(2)
	@param parent メインキャラクタークラス

	飛び道具等のオブジェクトを生成するために使います
*/
CCharacterBase::CCharacterBase(CCharacterBase *parent/* = NULL */) : CGoluahObject(FALSE)
{
	parent_char = parent;

	if(parent!=NULL){
		keyinput = parent->keyinput;//何番からキー入力を受け取るか
		pal_number = parent->pal_number;//パレット番号
		GetGObject()->tid		= parent->GetGObject()->tid;//チームID
		g_chardir = parent->g_chardir;
	}
	else {
		keyinput = 0;//何番からキー入力を受け取るか
		pal_number = 0;//パレット番号
		GetGObject()->tid		= 0;//チームID
	}

	CCharacterBaseCreate();
}

/*!
	@brief コンストラクタの共通処理
*/
void CCharacterBase::CCharacterBaseCreate()
{
	mysounds = NULL;
	mysound_numbers = 0;

	GOBJECT* pdat = GetGObject();

	for(int i=0;i<GCDMAX_IMAGES;i++)bitmaps[i]=NULL;
	cells=NULL;
	hantei=NULL;
	rects=NULL;
	mysounds = NULL;

	pdat->objtype	= GOBJTYPE_CHARACTER;//オブジェクトタイプ
	pdat->winfo		= &(this->waz);
	ZeroMemory(pdat->winfo,sizeof(WAZAINFO));

	base_z	=ZZAHYO_CHARACTER;
	front_z	=ZZAHYO_CHARACTER_F;
	back_z	=ZZAHYO_CHARACTER_B;

	chainComboEnabled = TRUE;	//チェーンコンボ有効
	isSuperArmer = FALSE;		//スーパーアーマー無効
	isAutoGuard = FALSE;		//オートガード無効

	base_voice_damage1 = 1;
	base_voice_damage2 = 2;
	base_voice_damage3 = 3;
	base_voice_ko = 0;
}

/*!
	@brief 初期化

	CreateCharacter時にマクロから自動実行される手筈になっています。
	順番が具合悪い場合はオーバーライドしてください
*/
void CCharacterBase::InitializeCharacter()
{
	InitParameters();
	InitAttackInfo();
	InitGCDandBMP();
	InitWazInfo();
	InitMySound();
	InitBullets();
}

/*!
	@brief 破棄
*/
CCharacterBase::~CCharacterBase()
{
	ReleaseGCDandBMP();
	ReleaseMySound();
	if(mysounds) free(mysounds);
}

/*!
	@brief メッセージ処理関数

	メッセージ処理を、適切な仮想関数に振り分けます
*/
DWORD CCharacterBase::Message(DWORD msg,LPVOID pd,DWORD prm)
{
	switch(msg){
	case GOBJMSG_DOTOJYO://登場
		ChangeAction( ACTID_TOJYO );return(TRUE);
	case GOBJMSG_DOYOUWIN://勝利
		ChangeAction( ACTID_SYORI );return(TRUE);
	case GOBJMSG_DOTIMEOVERLOSE:
		ChangeAction( ACTID_TIMEOVERLOSE );return(TRUE);
	case GOBJMSG_TAIKI://待機しとけメッセージ
		ChangeAction( ACTID_TAIKI );
		break;
	case GOBJMSG_ACTION:
		{
			DWORD result;

			PreAction();
			result = Action();
			PostAction();
			return result;
		}
	case GOBJMSG_KOUTAI://交代しろメッセージ
		if(GetGObject()->aid == ACTID_TAIKICYU){
			ChangeAction( ACTID_KOUTAIIN );
			return(TRUE);
		}
		return(0);
	case GOBJMSG_KOUTAI2://交代しろメッセージ2（のんびりと登場ポーズを決めてて良し）
		ChangeAction( ACTID_KOUTAI );
		break;
	case GOBJMSG_CLIPX:
		if(GetGObject()->aid == ACTID_KOUTAI)return(TRUE);//交代時はクリップ処理をしない
		else return(0);
	default:
		//あとのメッセージはオブジェクト基本クラスに任せる
		return(CGoluahObject::Message(msg,pd,prm));
	}
	return(0);
}


/*!
	@brief GOBJMSG_ACTION メッセージ処理

	CCharacterBaseで定義されている基本行動へ処理を振り分けます
*/
DWORD CCharacterBase::Action()
{
	switch(GetGObject()->aid){
	case ACTID_NEUTRAL	:act_neutral();break;//default actions
	case ACTID_CROUCH	:act_crouch();break;
	case ACTID_RAKKA	:act_rakka();break;
	case ACTID_TOJYO	:act_tojyo();break;
	case ACTID_SYORI	:act_win();break;
	case ACTID_WALKF	:act_walkf();break;//normal actions
	case ACTID_WALKB	:act_walkb();break;
	case ACTID_JAMPS	:act_jamps();break;
	case ACTID_JAMPC	:act_jampc();break;
	case ACTID_JAMPF	:act_jampf();break;
	case ACTID_JAMPB	:act_jampb();break;
	case ACTID_RAKKA2	:act_rakka2();break;
	case ACTID_ATT_SA	:act_att_sa();ChainCombo(CHAIN_SA);break;//attack actions
	case ACTID_ATT_SB	:act_att_sb();ChainCombo(CHAIN_SB);break;
	case ACTID_ATT_SC	:act_att_sc();ChainCombo(CHAIN_SC);break;
	case ACTID_ATT_CA	:act_att_ca();ChainCombo(CHAIN_CA);break;
	case ACTID_ATT_CB	:act_att_cb();ChainCombo(CHAIN_CB);break;
	case ACTID_ATT_CC	:act_att_cc();ChainCombo(CHAIN_CC);break;
	case ACTID_ATT_JA	:act_att_ja();break;
	case ACTID_ATT_JB	:act_att_jb();break;
	case ACTID_ATT_JC	:act_att_jc();break;
	case ACTID_TAIKI	:act_taiki();break;
	case ACTID_KOUTAIOUT:act_koutai_out();break;
	case ACTID_KOUTAIIN	:act_koutai_in();break;
	case ACTID_KOUTAI	:act_koutai();break;
	case ACTID_KOUTAI2	:act_koutai2();break;
	case ACTID_TAIKICYU	:act_taikicyu();break;
	case ACTID_STRIKERCOMEON:act_strikercomeon();break;
	case ACTID_TIMEOVERLOSE	:act_timeoverlose();break;
	//case ACTID_SYORI2IN		:act_win2_in();break;
	//case ACTID_SYORI2POSE	:act_win2_pose();break;
	default:return(0);
	}
	return(TRUE);
}


/*!
	@brief GOBJMSG_ACTION メッセージ 前処理

	定期的に行わなければならない何らかの処理を行います
*/
void CCharacterBase::PreAction()
{
	//チェーンコンボリストクリア
	if(!(GetGObject()->aid & 0xFFFD0000)){
		if(GetGObject()->counter > 2)chainlist=0;
	}

	//振り向き処理を行います。これ以外の動作でもやりたい場合はオーバーライドして追加してください
	if(pdat->aid==ACTID_NEUTRAL ||
	   pdat->aid==ACTID_CROUCH  ||
	   pdat->aid==ACTID_TOJYO   ||
	   !(pdat->aid & (ACTID_KUCYU | ACTID_ATTACK | ACTID_NAGE | ACTID_HISSATU | ACTID_SYSTEM | ACTID_INOUT))
	   )
	{
		Furimuki();
	}

	//ダメージ声再生
	if(pdat->hp > 0)final_voice_flag = FALSE;
	if(pdat->counter==0 && !final_voice_flag)
	{
		switch(pdat->aid)
		{
		//小喰らい
		case ACTID_DAMAGE1	:
		case ACTID_DAMAGEC1	:
		case ACTID_DAMAGE1A	:
		case ACTID_DAMAGEC1A:
		case ACTID_DAMAGEJ1	:
			if(GetRandNum(5)==0)PlayMySound(base_voice_damage1);
			break;
		//中喰らい
		case ACTID_DAMAGE2	:
		case ACTID_DAMAGEC2	:
		case ACTID_DAMAGE2A	:
		case ACTID_DAMAGEC2A:
		case ACTID_DAMAGEJ2	:
			if(GetRandNum(4)==0)PlayMySound(base_voice_damage2);
			break;
		//強喰らい
		case ACTID_DAMAGE3	:
		case ACTID_DAMAGEC3	:
		case ACTID_DAMAGE3A	:
		case ACTID_DAMAGEC3A:
		case ACTID_DAMAGEJ3	:
			if(GetRandNum(3)==0)PlayMySound(base_voice_damage3);
			break;

		//その他喰らい
		case ACTID_DOWN			:
		case ACTID_FUTTOBI		:
		case ACTID_FUTTOBI2		:
		case ACTID_TATAKITUKE1A	:
		case ACTID_TATAKITUKE2A	:
		case ACTID_TATAKITUKE1B	:
		case ACTID_TATAKITUKE2B	:
			if(GetRandNum(2)==0)PlayMySound(base_voice_damage3);
			break;

		//KO
		case ACTID_FINALDOWN	://死
		case ACTID_FINALDOWN2	://氏
		case ACTID_KAITENFINISH	:
			final_voice_flag = TRUE;
			PlayMySound(base_voice_ko);
			break;
		}
	}
}
/*!
	@brief GOBJMSG_ACTION メッセージ 後処理

	定期的に行わなければならない何らかの処理を行います
*/
void CCharacterBase::PostAction()
{
	//チェーンコンボリストクリア
	if(!(GetGObject()->aid & 0xFFFD0000)){
		if(GetGObject()->counter > 2)chainlist=0;
	}

	//振り向き処理を行います。これ以外の動作でもやりたい場合はオーバーライドして追加してください
	if(pdat->aid==ACTID_NEUTRAL ||
	   pdat->aid==ACTID_CROUCH
	   )
	{
		Furimuki();
	}
}


/*!
	@brief GOBJMSG_COMMAND メッセージ処理

	行動IDにより、Command系の仮想関数に処理を振り分けます
*/
void CCharacterBase::Command()
{
	DWORD key_now = (*funcs->getkey)(keyinput,0);
	BOOL callstriker=FALSE;
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_SYSTEM){//システム定義動作中なので、基本的にはいじらない
		Command_OnSystem(key_now);
		return;
	}
	if(pdat->aid == ACTID_TOJYO || pdat->aid==ACTID_SYORI)return;
	if(pdat->aid == ACTID_TIMEOVERLOSE)return;
	if(pdat->aid & ACTID_NAGE)return;
	if(pdat->aid & ACTID_INOUT)return;

	//ストライカーを呼ぶ
	if( (key_now&KEYSTA_FOWORD) && (key_now&KEYSTA_DOWN) && (key_now&KEYSTA_BD2) ){
		if(Message2System(MSGOBJ2SYS_STRIKER1,0)){
			ADDEFFECT(EFCTID_COMEON,(int)pdat->x,(int)pdat->y-100,pdat->muki);
			callstriker=TRUE;
		}
	}
	else if( (key_now&KEYSTA_BACK) && (key_now&KEYSTA_DOWN) && (key_now&KEYSTA_BD2) ){
		if(Message2System(MSGOBJ2SYS_STRIKER2,0)){
			ADDEFFECT(EFCTID_COMEON,(int)pdat->x,(int)pdat->y-100,pdat->muki);
			callstriker=TRUE;
		}
	}

	if(pdat->aid & ACTID_HISSATU){//必殺技中（基本的にいじるべからず）
		Command_OnHissatuAttacking(key_now);
		return;
	}

	//必殺技コマンド判定
	if (Command_Hissatu(key_now))
		return;

	if(pdat->aid & ACTID_ATTACK){//攻撃動作中（基本的にいじらない。チェーンコンボ出すならここでいじる）
		Command_OnAttacking(key_now);
		return;
	}

	//基本動作中
	if (Command_OnNormal(key_now) || pdat->aid & ACTID_KUCYU)
		return;

	//ストライカー
	if(callstriker){
		pdat->aid = ACTID_STRIKERCOMEON;
		return;
	}
	//こーたい
	else if( (key_now&KEYSTA_FOWORD) && (key_now&KEYSTA_BD2) && !(key_now&KEYSTA_DOWN) ){
		if(Message2System(MSGOBJ2SYS_KOUTAI1,0)){
			pdat->aid = ACTID_KOUTAIOUT;
			return;
		}
	}
	else if( (key_now&KEYSTA_BACK) && (key_now&KEYSTA_BD2) && !(key_now&KEYSTA_DOWN) ){
		if(Message2System(MSGOBJ2SYS_KOUTAI2,0)){
			pdat->aid = ACTID_KOUTAIOUT;
			return;
		}
	}

	if(pdat->aid==ACTID_STRIKERCOMEON){//入力がなければポーズキメ
		if(!(key_now & (KEYSTA_UP2|KEYSTA_DOWN2|KEYSTA_FOWORD2|KEYSTA_BACK2))){
			return;
		}
	}

	Command_Normal(key_now);
}


/*!
	@brief GOBJMSG_COMMAND メッセージ 部分処理（必殺技中）
*/
BOOL CCharacterBase::Command_Hissatu(DWORD keyinfo)
{
	return FALSE;
}


/*!
	@brief GOBJMSG_COMMAND メッセージ 部分処理（通常動作中）
*/
BOOL CCharacterBase::Command_Normal(DWORD keyinfo)
{
	GOBJECT* pdat = GetGObject();

	if(keyinfo & KEYSTA_UP){
		pdat->aid = ACTID_JAMPS;
	}
	else if(keyinfo & KEYSTA_DOWN){//しゃがみ
		pdat->aid = ACTID_CROUCH;
	}
	else if(keyinfo & KEYSTA_FOWORD){//前歩き
		pdat->aid = ACTID_WALKF;
	}
	else if(keyinfo & KEYSTA_BACK){//後ろ歩き
		pdat->aid = ACTID_WALKB;
	}
	else{//ニュートラルポーズ
		pdat->aid = ACTID_NEUTRAL;
	}

	return TRUE;
}

/*!
	@brief GOBJMSG_COMMAND メッセージ 部分処理（システム動作中）
*/
BOOL CCharacterBase::Command_OnSystem(DWORD keyinfo)
{
	return FALSE;
}

/*!
	@brief GOBJMSG_COMMAND メッセージ 部分処理（必殺攻撃動作中）
*/
BOOL CCharacterBase::Command_OnHissatuAttacking(DWORD keyinfo)
{
	return FALSE;
}

/*!
	@brief GOBJMSG_COMMAND メッセージ 部分処理（通常攻撃動作中）
*/
BOOL CCharacterBase::Command_OnAttacking(DWORD keyinfo)
{
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_KUCYU){////ジャンプ動作中
		return FALSE;
	}
	else{
		if(chainComboEnabled && keyinfo & 0x22220000){
			if(keyinfo & KEYSTA_DOWN){
				if(keyinfo & KEYSTA_BC2){if(ChainCombo(CHAIN_CC)){ChangeAction(ACTID_ATT_CC);return TRUE;}}
				else if(keyinfo & KEYSTA_BB2){if(ChainCombo(CHAIN_CB)){ChangeAction(ACTID_ATT_CB);return TRUE;}}
				else if(keyinfo & KEYSTA_BA2){if(ChainCombo(CHAIN_CA)){ChangeAction(ACTID_ATT_CA);return TRUE;}}
			}
			else{
				if(keyinfo & KEYSTA_BC2){if(ChainCombo(CHAIN_SC)){ChangeAction(ACTID_ATT_SC);return TRUE;}}
				else if(keyinfo & KEYSTA_BB2){if(ChainCombo(CHAIN_SB)){ChangeAction(ACTID_ATT_SB);return TRUE;}}
				else if(keyinfo & KEYSTA_BA2){if(ChainCombo(CHAIN_SA)){ChangeAction(ACTID_ATT_SA);return TRUE;}}
			}
		}
	}

	return FALSE;
}

/*!
	@brief GOBJMSG_COMMAND メッセージ 部分処理（通常動作）
*/
BOOL CCharacterBase::Command_OnNormal(DWORD keyinfo)
{
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_KUCYU){//ジャンプ動作中
		if(keyinfo & 0x22220000){
			if(keyinfo & KEYSTA_BC2){pdat->aid = ACTID_ATT_JC;return TRUE;}
			else if(keyinfo & KEYSTA_BB2){pdat->aid = ACTID_ATT_JB;return TRUE;}
			else if(keyinfo & KEYSTA_BA2){pdat->aid = ACTID_ATT_JA;return TRUE;}
		}
		return FALSE;
	}

	//地上動作中
	if(keyinfo & 0x22220000){
		if(keyinfo & KEYSTA_DOWN){
			if(keyinfo & KEYSTA_BC2){pdat->aid = ACTID_ATT_CC;return TRUE;}
			else if(keyinfo & KEYSTA_BB2){pdat->aid = ACTID_ATT_CB;return TRUE;}
			else if(keyinfo & KEYSTA_BA2){pdat->aid = ACTID_ATT_CA;return TRUE;}
		}
		else{
			if(keyinfo & KEYSTA_BC2){
				pdat->aid = ACTID_ATT_SC;return TRUE;
			}
			else if(keyinfo & KEYSTA_BB2){
				pdat->aid = ACTID_ATT_SB;return TRUE;
			}
			else if(keyinfo & KEYSTA_BA2){
				pdat->aid = ACTID_ATT_SA;return TRUE;
			}
		}
	}

	if(pdat->aid == ACTID_JAMPS){
		if(keyinfo & KEYSTA_FOWORD){//前ジャンプ
			pdat->aid = ACTID_JAMPF;
			return TRUE;
		}
		else if(keyinfo & KEYSTA_BACK){//後ジャンプ
			pdat->aid = ACTID_JAMPB;
			return TRUE;
		}
	}

	return FALSE;
}


/*!
	@brief GOBJMSG_TOUCHA メッセージ 処理
*/
DWORD CCharacterBase::TouchA(ATTACKINFO *info,DWORD ta_eid)
{
	GOBJECT* pdat = GetGObject();

	// 待機/退避中は無視しちゃう
	if(pdat->aid & ACTID_INOUT)return(TOUCHA_AVOID);
	//投げ中は無視しちゃう
	if(pdat->aid & ACTID_NAGE)return(TOUCHA_AVOID);

	//スーパーアーマー処理。攻撃を無視
	if(isSuperArmer){
		if(pdat->hp>0){
			AddEffect(EFCTID_SUPERARMER,(int)pdat->x,(int)(pdat->y-50.0f),pdat->muki);
			return TOUCHA_MUSI;
		}
	}

	//オートガード処理。攻撃・喰らい中以外は無条件ガード
	BOOL auto_guard = FALSE;
	if(isAutoGuard)
	{
		if(IsCom())
		{
			auto_guard = (rand()%2) ? TRUE : FALSE;
		}
		else
		{
			DWORD crnt_key = GetKey(0);
			if((crnt_key&KEYSTA_FOWORD)||(crnt_key&KEYSTA_UP))
			{
				auto_guard = FALSE;
			}
			else auto_guard = TRUE;
		}
	}

	if(auto_guard)
	{
		if(CANNOT_GUARD(info->guard) && (pdat->aid!=ACTID_RAKKA))return 0;//どのガードもできない場合はガードできない
		if(pdat->aid & ACTID_ATTACK)return 0;
		if(pdat->aid & ACTID_KURAI)return 0;
		if(pdat->aid & ACTID_KUCYU)
		{
			//空中ガード
			return (info->guard & GUARDINFO_XJAMP) ? 0 : TOUCHA_GUARDJ;
		}
		if((info->guard & GUARDINFO_XCROUCH) && (info->guard & GUARDINFO_XSTAND))
		{
			//地上ガード不可
			return 0;
		}
		if(pdat->aid & ACTID_SYAGAMI)
		{
			//屈ガード
			if(info->guard & GUARDINFO_XCROUCH)return TOUCHA_GUARDS;
			return TOUCHA_GUARDC;
		}
		else
		{
			//立ちガード
			if(info->guard & GUARDINFO_XSTAND)return TOUCHA_GUARDC;
			return TOUCHA_GUARDS;
		}
	}

	return CGoluahObject::TouchA(info,ta_eid);
}

/*!
	@brief GGOBJMSG_COMMANDCOM メッセージ 処理
*/
DWORD CCharacterBase::CommandCOM(DWORD wid)
{
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_SYSTEM)return(CmdCom_OnSystem(wid));
	if(pdat->aid == ACTID_TOJYO || pdat->aid==ACTID_SYORI)return(FALSE);
	if(pdat->aid & ACTID_NAGE)return(FALSE);
	if(pdat->aid & ACTID_INOUT)return(FALSE);
	
	if(pdat->aid & ACTID_HISSATU){//必殺技中（基本的にいじるべからず）
		return(CmdCom_OnHissatuAttacking(wid));
	}

	if(pdat->aid & ACTID_KUCYU){
		return(CmdCom_OnKucyu(wid));
	}
	else{
		return(CmdCom_OnNormal(wid));
	}
}

/*!
	@brief GGOBJMSG_COMMANDCOMメッセージ 部分処理
*/
DWORD CCharacterBase::CmdCom_OnSystem(DWORD wid)
{
	return(FALSE);
}

/*!
	@brief GGOBJMSG_COMMANDCOMメッセージ 部分処理
*/
DWORD CCharacterBase::CmdCom_OnHissatuAttacking(DWORD wid)
{
	return(FALSE);
}

/*!
	@brief GGOBJMSG_COMMANDCOMメッセージ 部分処理
*/
DWORD CCharacterBase::CmdCom_OnKucyu(DWORD wid)
{
	//投げは継承側で処理しないといけないので、ダメ
	if(wid&ACTID_NAGE)return(FALSE);

	if(GetGObject()->aid & ACTID_ATTACK){//空中攻撃中
		if(wid&ACTID_HISSATU && wid&ACTID_KUCYU)return(TRUE);//空中必殺技ならOK
		return(FALSE);//それ以外はだめ
	}
	else{//空中通常動作中
		if(wid&ACTID_ATTACK && wid&ACTID_KUCYU)return(TRUE);//空中通常技ならOK
		return(FALSE);//それ以外はだめ
	}
}

/*!
	@brief GGOBJMSG_COMMANDCOMメッセージ 部分処理
*/
DWORD CCharacterBase::CmdCom_OnNormal(DWORD wid)
{
	//投げは継承側で処理しないといけないので、ダメ
	if(wid&ACTID_NAGE)return(FALSE);

	if(GetGObject()->aid & ACTID_ATTACK){//地上攻撃中
		if(wid&ACTID_NAGE)return(FALSE);// 投げ技でのキャンセルは不可
		if(wid&ACTID_HISSATU && !(wid&ACTID_KUCYU))return(TRUE);//地上必殺技ならOK
		if(chainComboEnabled)
		{
			switch(wid)
			{
			case ACTID_ATT_SA:return( ChainCombo(CHAIN_SA) );
			case ACTID_ATT_SB:return( ChainCombo(CHAIN_SB) );
			case ACTID_ATT_SC:return( ChainCombo(CHAIN_SC) );
			case ACTID_ATT_CA:return( ChainCombo(CHAIN_CA) );
			case ACTID_ATT_CB:return( ChainCombo(CHAIN_CB) );
			case ACTID_ATT_CC:return( ChainCombo(CHAIN_CC) );
			}
		}
		return(FALSE);//だめ
	}
	else{//地上通常動作
		if(wid&ACTID_KUCYU && wid&ACTID_ATTACK)return(FALSE);//空中攻撃はだめ
		return(TRUE);//それ以外ならOK
	}
}

/*!
	@brief 攻撃力情報初期化

	実際のキャラクタークラスではこの関数をオーバーライドして
	攻撃力情報を初期化する処理を記述してください
*/
void CCharacterBase::InitAttackInfo()
{
}


/*!
	@brief GCD/BMP ロード

	キャラクターフォルダの cell.gcd と image1～12.bmp をロードします。
	別のファイル名のファイルを使用したい場合はオーバーライドして処理を変更します。
*/
void CCharacterBase::InitGCDandBMP()
{
	int i;
	GOBJECT* pdat = GetGObject();
	if(pdat==NULL)return;
	char filename[256],palname[256];

#ifdef _DEBUG
	if(pal_number==0)
		OutputDebugString("キャラDLL [warning] : パレット番号が0です。");
#endif

	//"image?.bmp" を読み込む
	sprintf(palname,"%s\\pal%d",g_chardir,pal_number);
	for(i=0;i<GCDMAX_IMAGES;i++){
		sprintf(filename,"%s\\image%d",g_chardir,i+1);
		bitmaps[i] = (MYSURFACE*) ( (*funcd->loadimage)(filename,palname) );
	}

	//"cell.gcd"を読み込み
	sprintf(filename,"%s\\cell",g_chardir);
	funcd->create_celldat2(filename, (void**)&cells, (void**)&rects, (void**)&hantei);

	//pdatに入れておく
	pdat->phdat			= hantei;
	pdat->pmsarr		= bitmaps;
	pdat->pcdat			= cells;
	pdat->prdat			= rects;
}

/*!
	@brief GCD/BMP アンロード
	@sa InitGCDandBMP
*/
void CCharacterBase::ReleaseGCDandBMP()
{
	for(int i=0;i<GCDMAX_IMAGES;i++){
		if(bitmaps[i]!=NULL){
			funcd->unloadbmp(bitmaps[i]);
		}
	}
	if(cells!=NULL) {
		funcd->destroy_celldat((void**)&cells, (void**)&rects, (void**)&hantei);
	}
	else {
		if(hantei!=NULL)free(hantei);
		if(rects!=NULL)free(rects);
	}
}

/* !
	@brief MYSOUND構造体を比較する

	@sa CCharacterBase::InitMySound
	ソート用、InitMySound関数内部で使用してます。
*/
/*bool mscomp(const MYSOUND& val1, const MYSOUND& val2)
{
	return val1.number < val2.number;
}*/

/*!
	@brief wavサウンド関係

	@param path_name サウンドの読み込みを行う基準パス(NULLの場合、"sound")
	@param list_name サウンド番号とwavファイル名の対応を記述した定義ファイル名(NULLの場合"list.txt")
*/
void CCharacterBase::InitMySound(char* path_name,char* list_name)
{
	ReleaseMySound();

	if(!path_name)path_name = MYSOUND_PATH;
	if(!list_name)list_name = MYSOUND_LIST;

	unsigned int i;
	char *filename = new char [256];
	char *buff = NULL;
	std::vector<char*>		name_list;
	std::vector<int>	number_list;
	std::vector<LPVOID>	sound_list;
//	std::vector<MYSOUND>	sound_list;

	do
	{
		//テキストからファイル名のリストを取得
		UINT bufflen;
		sprintf(filename, "%s\\%s\\%s",GetCharDir(),path_name,list_name);
		File2Mem(filename, &buff, &bufflen);
		if(!buff)break;

		//てんぽらり～な変数
		char* s = buff;
		char* tmpstr = NULL;
		int tmpno = 0;

		//ファイル名のリストを取得
		while((UINT)(s-buff)<bufflen-1)
		{
			if(*s=='#')
			{
				tmpstr = new char [32];
				s++;
				if(2==sscanf( s,"%d %s",&tmpno,tmpstr) && tmpno >= 0)
				{
					name_list.push_back(tmpstr);
					number_list.push_back(tmpno);

/*					MYSOUND tmpms;

					tmpms.number	= tmpno;
					tmpms.sound		= NULL;
					sound_list.push_back(tmpms);*/
				}
				else
				{
					DELETE_ARRAY(tmpstr);
				}
			}
			else
			{
				s++;
				if(*(s - 1) & 0x80)s++;
			}
		}
		if(name_list.size()==0)break;

		//サウンドをロード
		int success_num = 0;
		for(i=0;i<name_list.size();i++)
		{
			sprintf(filename,"%s\\%s\\%s.wav",GetCharDir(),path_name,name_list[i]);
			sound_list.push_back( (*funcs->loadmysound)(filename) );
			if(sound_list[i])success_num++;

			/*if(i < sound_list.size())
			{
				sound_list[i].sound = (*funcs->loadmysound)(filename);
				if (sound_list[i].sound) success_num++;
			}*/
		}
		if(success_num==0)break;

		// データをソート
//		std::stable_sort< std::vector< MYSOUND >::iterator >(sound_list.begin(), sound_list.end(), mscomp);

		//配列を用意して、成功したやつだけ保持しておく
		mysounds = new LPVOID [success_num+1];
		mysound_numbers = new int [success_num];
		/*mysounds = (LPMYSOUND)malloc( sizeof(MYSOUND) * success_num);
		mysound_numbers = success_num;*/
		success_num = 0;//使いまわし
		for(i=0;i<name_list.size();i++)
		{
			if(sound_list[i])
			{
				mysounds[success_num] = sound_list[i];
				mysound_numbers[success_num] = number_list[i];
				success_num++;
			}

/*			// サウンドがNULLの場合はパス。
			if ( (i < sound_list.size()) && sound_list[i].sound )
			{
				if ( ( (i > 0) && (sound_list[i - 1].number == sound_list[i].number) ) )
				{
					// 同じ番号があるので失敗、配列を１減らす。
					mysound_numbers--;
					mysounds = (LPMYSOUND)realloc(mysounds, sizeof(MYSOUND) * mysound_numbers);
					KillMySound(sound_list[i].sound);	// 使わないのであぼんぬ
				}
				else
				{
					mysounds[success_num] = sound_list[i];
					success_num++;
				}
			}*/
		}
		//終端検知用
		mysounds[success_num] = NULL;
	}
	while(0);
	DELETE_ARRAY( buff );
	DELETE_ARRAY( filename );
	for(i=0;i<name_list.size();i++)DELETE_ARRAY( name_list[i] );
}

/*!
	@brief wavサウンド再生
	@param number サウンド番号
	@sa InitMySound

	numberで指定された番号のサウンドを再生します。
*/
void CCharacterBase::PlayMySound(DWORD number)
{
	if(!mysounds)return;

	UINT i=0;
	while(mysounds[i])
	{
		if(mysound_numbers[i]==number)
		{
			funcs->playmysound(mysounds[i]);
			return;
		}
		i++;
	}

	// 二分探索を使って検索
/*	UINT i=0, j=mysound_numbers-1;
	while(i <= j)
	{
		UINT cent = (i + j) / 2;

		if (mysounds[cent].number < number)
			i = cent + 1;
		else if (mysounds[cent].number > number)
			j = cent - 1;
		else
		{
			// 発見
			funcs->playmysound(mysounds[cent].sound);
			return;
		}
	}*/
}

/*!
	@brief サウンド破棄
	@sa InitMySound
*/
void CCharacterBase::ReleaseMySound()
{
	if(!mysounds)return;

	UINT i=0;
	while(mysounds[i])
	{
		funcs->killmysound(mysounds[i]);
		i++;
	}
	DELETE_ARRAY( mysounds );
	DELETE_ARRAY( mysound_numbers );

/*	for(UINT i=0; i < mysound_numbers; i++)
	{
		funcs->killmysound(mysounds[i].sound);
	}
	free(mysounds);
	mysounds = NULL;
	mysound_numbers = 0;*/
}

/*!
	@brief 落下処理

	@param acc_g 重力加速度(1.0が標準)
	@param cyakuchi y座標が0以上に達した際に、行動遷移を行うかどうか
	@param toaid y座標が0以上に達した際に遷移する行動ID
*/
void CCharacterBase::JiyuuRakka(double acc_g,BOOL cyakuchi,DWORD toaid)
{
	GOBJECT* pdat = GetGObject();

	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy += acc_g;
	if(cyakuchi){
		CyakuchiHantei(toaid);
	}
}

/*!
	@brief 着地判定処理
	@param toaid 着地していると判定された場合に遷移する行動ID

	y座標が0以上のとき、着地していると判定されます
*/
void CCharacterBase::CyakuchiHantei(DWORD toaid)
{
	GOBJECT* pdat = GetGObject();

	if(pdat->y > 0){
		pdat->y=0;
		pdat->aid=toaid;
		Furimuki();
	}
}

/*-----------------------------------------------------------------------------
	Z位置操作
-------------------------------------------------------------------------------*/
void CCharacterBase::ZBack(){GetGObject()->z=back_z;}
void CCharacterBase::ZFront(){GetGObject()->z=front_z;}

/*!
	@brief 振り向き 処理
	@return TRUE:振り向き処理を行い、向きが変更された

	ターゲットオブジェクトを基準にしてx座標を判定してオブジェクトの向きを変更します。
*/
BOOL CCharacterBase::Furimuki()
{
	GOBJECT* pdat = GetGObject();
	BOOL muki_prev = pdat->muki;
	GOBJECT *pedat = (GOBJECT*)(*funco->getinfo)(pdat->eid);

	if(pdat==NULL)return(FALSE);
	if(pedat==NULL)return(FALSE);

	if(pdat->x > pedat->x )
		pdat->muki = TRUE;
	else
		pdat->muki = FALSE;

	if(muki_prev!=pdat->muki)return(TRUE);
	else return(FALSE);
}

/*-----------------------------------------------------------------------------
	チェーンコンボ判定
-------------------------------------------------------------------------------*/

/*!
	@brief チェーンコンボ判定

	指定フラグがまだ立っていなければ、そのフラグを追加してTRUEを返す。
	チェーンコンボでキャンセルできない場合はFALSEを返す

	@return TRUE:その技を出してもよし, FALSE:だめ
*/
BOOL CCharacterBase::ChainCombo(DWORD chainid)
{
	if(!chainComboEnabled)return FALSE;
	if(chainlist & chainid)return(FALSE);

	chainlist |= chainid;
	return(TRUE);
}

/*!
	@brief チェーンコンボ判定
	@sa ChainCombo

	ChainComboとの違いは、指定フラグを すでに出したリストに加えるかどうか。
	この関数では指定チェーンコンボIDをリストに加えず、チェックのみを行う。
*/
BOOL CCharacterBase::ChainCheck(DWORD chainid)
{
	if(!chainComboEnabled)return FALSE;
	if(chainlist & chainid)return(FALSE);

	return(TRUE);
}


/*!
	@brief 技情報初期化

	通常、この関数をオーバーライドして技情報を初期化します。
	CCharacterBase::InitWazInfo を呼び出すと、歩きやジャンプ等の基本的な
	行動の設定を行います。
*/
void CCharacterBase::InitWazInfo()
{
	waz.walkf = ACTID_WALKF;
	waz.walkb = ACTID_WALKB;
	waz.jampf = ACTID_JAMPF;
	waz.jampb = ACTID_JAMPB;

	waz.att_jamp[0] = ACTID_ATT_JC;//ジャンプ攻撃
	waz.att_jamp[1] = ACTID_ATT_JB;
	waz.att_jamp[2] = ACTID_ATT_JA;
}

/*!
	@brief パワーゲージ増加
	@param dp 増加量(マイナスも可)
*/
void CCharacterBase::AddPowerGauge(double dp)
{
	GOBJECT* pdat = GetGObject();

	pdat->gauge += dp;
	if(pdat->gauge<0)pdat->gauge = 0;
	else if(pdat->gauge > pdat->gaugemax)pdat->gauge = pdat->gaugemax;
}

/*-----------------------------------------------------------------------------
	コマンド判定関数
-------------------------------------------------------------------------------*/

/*!
	@brief コマンド判定(↓＼→)
	@param dt コマンド受付時間
	@return TRUE:コマンド成立
*/
BOOL CCharacterBase::com236(int dt)//葉同権コマンド
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

/*!
	@brief コマンド判定(↓＼→↓＼→)
	@param dt コマンド受付時間
	@return TRUE:コマンド成立
*/
BOOL CCharacterBase::com236236(int dt)//葉同権コマンドx2
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

/*!
	@brief コマンド判定(→↓＼)
	@param dt コマンド受付時間
	@return TRUE:コマンド成立
*/
BOOL CCharacterBase::com623(int dt)//しょーりゅーこまんど
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

/*!
	@brief コマンド判定(↓／←)
	@param dt コマンド受付時間
	@return TRUE:コマンド成立
*/
BOOL CCharacterBase::com214(int dt)//逆はどー
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

/*!
@brief コマンド判定(↓／←↓／←)
@param dt コマンド受付時間
@return TRUE:コマンド成立
*/
BOOL CCharacterBase::com214214(int dt)//逆はどーx2
{
	int ofst = 0;

	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_BACK);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_DOWN | KEYSTA_BACK);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_DOWN);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_BACK);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_DOWN | KEYSTA_BACK);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_DOWN);
	if (ofst<0)return(FALSE);

	return(TRUE);
}
/*!
	@brief コマンド判定(←↓／)
	@param dt コマンド受付時間
	@return TRUE:コマンド成立
*/
BOOL CCharacterBase::com421(int dt)
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_BACK);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

/*!
	@brief コマンド判定(←／↓＼→)
	@param dt コマンド受付時間
	@return TRUE:コマンド成立
*/
BOOL CCharacterBase::com426(int dt)//よが
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_BACK);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

/*!
	@brief コマンド判定(→→)
	@param dt コマンド受付時間
	@return TRUE:コマンド成立
*/
BOOL CCharacterBase::com66(int dt)//ダッシュコマンド
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD2|KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst+=1;
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD2|KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

BOOL CCharacterBase::com66i(int dt)//ダッシュコマンド
{
	if(!(GetKey(0)&KEYSTA_FOWORD2))return FALSE;
	return com66(dt);
}

/*!
	@brief コマンド判定(←←)
	@param dt コマンド受付時間
	@return TRUE:コマンド成立
*/
BOOL CCharacterBase::com44(int dt)//バックダッシュコマンド
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_BACK2|KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst+=1;
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_BACK2|KEYSTA_BACK);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

BOOL CCharacterBase::com44i(int dt)//バックダッシュコマンド
{
	if(!(GetKey(0)&KEYSTA_BACK2))return FALSE;
	return com44(dt);
}


/*!
	@brief コマンド判定(↓↓)
	@param dt コマンド受付時間
	@return TRUE:コマンド成立
*/
BOOL CCharacterBase::com22(int dt)
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN2|KEYSTA_DOWN);
	if(ofst<0)return(FALSE);
	ofst+=1;
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN2|KEYSTA_DOWN);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

BOOL CCharacterBase::com22i(int dt)
{
	if(!(GetKey(0)&KEYSTA_DOWN2))return FALSE;
	return com22(dt);
}

/*!
	@brief 地上投げ判定
	@param maai 限界間合い
	@return TRUE:オッケー
*/
BOOL CCharacterBase::NageHantei(DWORD maai)//投げの判定
{
	DWORD hm;
	GOBJECT *pedat= (GOBJECT*)(*funco->getinfo)(GetGObject()->eid);
	if(pedat==NULL)return(FALSE);

	if(pedat->aid & ACTID_KUCYU)//相手が空中判定
		return(FALSE);

	hm = (*funco->getmaai_h)(GetGObject()->id,pedat->id);
	if(hm > maai)
		return(FALSE);

	return(TRUE);
}


/*!
	@brief コマンド判定( ↓＼→＼↓／←)
	@param dt コマンド受付時間
	@return TRUE:コマンド成立
*/
BOOL CCharacterBase::com2363214(int dt)
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_BACK | KEYSTA_DOWN);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD | KEYSTA_DOWN);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

/*!
	@brief コマンド判定(→＼↓／←／↓＼→)
	@param dt コマンド受付時間
	@return TRUE:コマンド成立
*/
BOOL CCharacterBase::com62426(int dt)
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

/*!
	@brief コマンド判定(→←／↓＼→)
	@param dt コマンド受付時間
	@return TRUE:コマンド成立
*/
BOOL CCharacterBase::com6426(int dt)
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

/*!
	@brief コマンド判定(←↓→↑)
	@param dt コマンド受付時間
	@return TRUE:コマンド成立
	com4268～com8624はぁゃなみのソースからお借りしました。感謝。
*/
BOOL CCharacterBase::com4268(int dt)
{
	int ofst = 0;

	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_UP);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_FOWORD);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_FOWORD | KEYSTA_DOWN);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_DOWN);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_DOWN | KEYSTA_BACK);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_BACK);
	if (ofst<0)return(FALSE);

	return(TRUE);
}

/*!
	@brief コマンド判定(↓→↑←)
	@param dt コマンド受付時間
	@return TRUE:コマンド成立
*/
BOOL CCharacterBase::com2684(int dt)
{
	int ofst = 0;

	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_BACK);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_UP);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_FOWORD);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_FOWORD | KEYSTA_DOWN);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_DOWN);
	if (ofst<0)return(FALSE);

	return(TRUE);
}

/*!
	@brief コマンド判定(→↑←↓)
	@param dt コマンド受付時間
	@return TRUE:コマンド成立
*/
BOOL CCharacterBase::com6842(int dt)
{
	int ofst = 0;

	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_DOWN);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_DOWN | KEYSTA_BACK);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_BACK);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_UP);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_FOWORD);
	if (ofst<0)return(FALSE);

	return(TRUE);
}


/*!
	@brief コマンド判定(↑←↓→)
	@param dt コマンド受付時間
	@return TRUE:コマンド成立
*/
BOOL CCharacterBase::com8426(int dt)
{
	int ofst = 0;

	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_FOWORD);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_FOWORD | KEYSTA_DOWN);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_DOWN);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_DOWN | KEYSTA_BACK);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_BACK);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_UP);
	if (ofst<0)return(FALSE);

	return(TRUE);
}
/*!
	@brief コマンド判定(←↑→↓)
	@param dt コマンド受付時間
	@return TRUE:コマンド成立
*/
BOOL CCharacterBase::com4862(int dt)
{
	int ofst = 0;

	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_DOWN);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_FOWORD | KEYSTA_DOWN);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_FOWORD);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_UP);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_BACK);
	if (ofst<0)return(FALSE);

	return(TRUE);
}

/*!
	@brief コマンド判定(↓←↑→)
	@param dt コマンド受付時間
	@return TRUE:コマンド成立
*/
BOOL CCharacterBase::com2486(int dt)
{
	int ofst = 0;

	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_FOWORD);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_UP);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_BACK);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_DOWN | KEYSTA_BACK);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_DOWN);
	if (ofst<0)return(FALSE);

	return(TRUE);
}

/*!
	@brief コマンド判定(→↓←↑)
	@param dt コマンド受付時間
	@return TRUE:コマンド成立
*/
BOOL CCharacterBase::com6248(int dt)
{
	int ofst = 0;

	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_UP);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_BACK);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_DOWN | KEYSTA_BACK);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_DOWN);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_FOWORD | KEYSTA_DOWN);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_FOWORD);
	if (ofst<0)return(FALSE);

	return(TRUE);
}
/*!
	@brief コマンド判定(↑→↓←)
	@param dt コマンド受付時間
	@return TRUE:コマンド成立
*/
BOOL CCharacterBase::com8624(int dt)
{
	int ofst = 0;

	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_BACK);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_DOWN | KEYSTA_BACK);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_DOWN);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_FOWORD | KEYSTA_DOWN);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_FOWORD);
	if (ofst<0)return(FALSE);
	ofst = (*funcs->seekkey)(keyinput, ofst, dt, KEYSTA_UP);
	if (ofst<0)return(FALSE);

	return(TRUE);
}
/*!--------------------------------------------------------------------------------------
	@brief COMレベル判定
	@param level 基準となるレベル(難易度)
	@return 判定に成功した場合はTRUE
	@sa GetComLevel

	特定のレベルを基準とし、連続技を出させるかどうか決めるときに使います。
----------------------------------------------------------------------------------------*/
BOOL CCharacterBase::ComLevelCk(UINT level)
{
	int crnt_level = GetComLevel();
	int v = GetRandNum(100);
	int r;
	const int rat_base = 80;

	if((level==0) || crnt_level==level)
	{
		r = rat_base;
	}
	else if(crnt_level<(int)level)
	{
		r = rat_base*crnt_level/level;
	}
	else
	{
		r = rat_base + (100-rat_base)*(crnt_level-level)/7;
	}

	return (r>=v) ? TRUE : FALSE;
}


/*-----------------------------------------------------------------------------
	基本的な行動関数
	使いまわしてください
-------------------------------------------------------------------------------*/

/*!
	@brief 交代行動(in)

	交代して画面内にはいってくる(1)
	登場ポーズはしない。
*/
void CCharacterBase::act_koutai_in()
{
	GOBJECT* pdat = GetGObject();

	if(pdat->counter==0){
		pdat->objtype |= GOBJTYPE_DISPLAY;//画面内にいる
	}

	GOBJECT *pedat;
	if(pdat->counter==0){
		if(pdat->tid==TEAM_PLAYER1)
			pedat = GetActiveCharacter(TEAM_PLAYER2);
		else
			pedat = GetActiveCharacter(TEAM_PLAYER1);
		if(pedat!=NULL){
			if(pedat->x>0){
				pdat->x = pedat->x -(320+400);
				pdat->muki=FALSE;
				pdat->vx=14;
			}
			else{
				pdat->x = pedat->x +(320+400);
				pdat->muki=TRUE;
				pdat->vx=14;
			}
		}
		else {
			pdat->x=0;
			pdat->vx=0;
		}
		pdat->y = -400;
		pdat->vy= 0;
	}
	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy++;

	if(pdat->y > 0){
		pdat->objtype &= ~GOBJFLG_TAIKI;//待機フラグを消す
		AddProperty( GOBJFLG_TOBETARGET );//ターゲットになる
		pdat->y=0;
		pdat->aid = ACTID_NEUTRAL;
	}
}

/*!
	@brief 交代行動(out)

	画面外にぴょーんってでてって待機する
*/
void CCharacterBase::act_koutai_out()
{
	GOBJECT* pdat = GetGObject();

	if(pdat->counter==0){
		pdat->objtype &= GOBJTYPE_NODISPLAY;//画面外へいけるようにしておく
		pdat->objtype |= GOBJFLG_TAIKI;//待機フラグを立てる
		RemoveProperty( GOBJFLG_TOBETARGET );//ターゲットにはならない
	}

	if(pdat->counter < 2){
		pdat->vx = -12;
		pdat->vy = -16;
	}
	else {
		pdat->revx=TRUE;
		movex(pdat->vx);
		pdat->y += pdat->vy;
		pdat->vy++;
		if(pdat->x > 1500 || pdat->x <-1500)pdat->aid = ACTID_TAIKICYU;
	}
}

/*!
	@brief 交代行動(out)
	画面外に飛んでって待機する
*/
void CCharacterBase::act_taiki()
{
	GOBJECT* pdat = GetGObject();

	if(pdat->counter==0){
		pdat->objtype &= GOBJTYPE_NODISPLAY;//画面外へいけるようにしておく
		AddProperty( GOBJFLG_TAIKI );//待機フラグを立てる
		RemoveProperty( GOBJFLG_TOBETARGET );//ターゲットにはならない
	}

	if(pdat->counter < 2){
		pdat->vx = -12;
		pdat->vy = -20;
	}
	else {
		pdat->revx=TRUE;
		movex(pdat->vx);
		pdat->y += pdat->vy;
		pdat->vy++;
		if(pdat->x > 1500 || pdat->x <-1500)pdat->aid = ACTID_TAIKICYU;
	}
}


/*!
	@brief 交代行動(in) 登場ポーズあり・登場ポーズ
*/
void CCharacterBase::act_koutai2()
{
	act_tojyo();

	if(GetGObject()->counter<150 || GetGObject()->aid==ACTID_NEUTRAL){
		GetGObject()->aid = ACTID_NEUTRAL;
		(*funcs->msg2system)(GetGObject()->id,MSGOBJ2SYS_KOUTAIEND,0);
	}
}

/*!
	@brief 交代行動(in) 登場ポーズあり・発動
*/
void CCharacterBase::act_koutai()
{
	GOBJECT* pdat = GetGObject();

	pdat->objtype |= GOBJTYPE_DISPLAY;//画面内にいる

	GOBJECT *pedat;
	if(pdat->counter==0){
		if(pdat->tid==TEAM_PLAYER1)
			pedat = GetActiveCharacter(TEAM_PLAYER2);
		else
			pedat = GetActiveCharacter(TEAM_PLAYER1);
		if(pedat!=NULL){
			if(pedat->x>0){
				pdat->x = pedat->x -(320+400);
				pdat->muki=FALSE;
				pdat->vx=14;
			}
			else{
				pdat->x = pedat->x +(320+400);
				pdat->muki=TRUE;
				pdat->vx=14;
			}
		}
		else {
			pdat->x=0;
			pdat->vx=0;
		}
		pdat->y = -400;
		pdat->vy= 0;
	}
	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy++;

	if(pdat->y > 0){
		pdat->objtype &= ~GOBJFLG_TAIKI;//待機フラグを消す
		AddProperty( GOBJFLG_TOBETARGET );//ターゲットになる
		pdat->y=0;
		pdat->aid = ACTID_KOUTAI2;//登場ポーズ
	}
}

/*!
	@brief 待機中行動
*/
void CCharacterBase::act_taikicyu(){}

/*!
	@brief 支援攻撃呼び出し行動
*/
void CCharacterBase::act_strikercomeon()
{
	ChangeAction(ACTID_NEUTRAL);
}

/*!
	@brief タイムオーバー負け行動
*/
void CCharacterBase::act_timeoverlose()
{
	ChangeAction(ACTID_NEUTRAL);
}

/*!
	@brief 仲間勝利時行動
*/
/*void CCharacterBase::act_win2_in()
{
	GOBJECT* pdat = GetGObject();

	pdat->objtype |= GOBJTYPE_DISPLAY;//画面内にいる

	GOBJECT *padat;
	if(pdat->counter==0){
		padat = GetActiveCharacter(pdat->tid);
		if(padat!=NULL){
			if(pedat->x>0){
				pdat->muki=padat->muki;
				//if (G
				pdat->x = padat->x - zurex(320+400);
				pdat->x = pedat->x + zurex(320+400);
				pdat->vx=14;
			}
			else{
				pdat->muki=TRUE;
				pdat->vx=14;
			}
		}
		else {
			pdat->x=0;
			pdat->vx=0;
		}
		pdat->y = -400;
		pdat->vy= 0;
	}
	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy++;

	if(pdat->y > 0){
		pdat->objtype &= ~GOBJFLG_TAIKI;//待機フラグを消す
		AddProperty( GOBJFLG_TOBETARGET );//ターゲットになる
		pdat->y=0;
		pdat->aid = ACTID_KOUTAI2;//登場ポーズ
	}
}*/

/*!
	@brief 空中ダメージ復帰後落下
*/
void CCharacterBase::act_rakka2(){act_rakka();}


/*-----------------------------------------------------------------------------
	勝利台詞 取得/設定 処理
-------------------------------------------------------------------------------*/
/*!
	@brief 勝利台詞の設定

	設定テキストファイルをロードし、敵の名前とフェイス変更値を判断して
	適切な台詞を選択してシステムに通知します。
	通常勝ちポーズ行動で行います。

	@param filename 勝利台詞設定テキストファイル名(NULLの場合"serifu.txt")
*/
void CCharacterBase::LoadAndSetKatiSerif(char* filename)
{
	if(!filename)filename = WIN_SERIFU_TXT;

	char* filepath = new char[256];
	char* tmpname = new char[64];
	char* katiserifu = new char[256];
	char* buff = NULL;
	UINT  bufflen;
	std::vector<char*>	str_points;
	std::vector<char*>	str_points_r;
	do
	{
		//ファイルをメモリ上にロード
		sprintf(filepath,"%s\\%s",g_chardir,filename);
		File2Mem(filepath,&buff,&bufflen);
		if(!buff)break;
		
		char* enemyname = GetEnemyName(pdat->tid);
		int   enemyface = GetEnemyFace(pdat->tid);

		//設定を検出し、設定開始位置を記憶する
		char* s = buff;
		char* s_point;
		int tmpface;
		while((UINT)(s-buff)<bufflen-1)
		{
			if(*s=='#')
			{
				s++;
				s_point = s;
				int scaret = sscanf( s,"%s %d",tmpname,&tmpface);
				if(scaret == 1)
				{
					tmpface = 0;
				}
				if(scaret>0)
				{
					//ランダム用
					if( strcmp(tmpname,"random")==0 ){
						str_points_r.push_back(s_point);
					}
					//指定台詞
					else if( tmpface==enemyface && enemyname && strcmp(tmpname,enemyname)==0){
						str_points.push_back(s_point);
					}
				}
			}
			else
			{
				s++;
				if(*s & 0x80)s++;
			}
		}


		//使用する台詞を選択
		if(str_points.size()>0)
		{
			s_point = str_points[ GetRandNum((int)str_points.size()) ];
		}
		else if(str_points_r.size()>0)
		{
			s_point = str_points_r[ GetRandNum((int)str_points_r.size()) ];
		}
		else break;

		//台詞を読み込み
		s = s_point;
		ZeroMemory(katiserifu,256);
		while(TRUE)
		{
			//改行するまで進む
			if(*s=='\n')
			{
				s++;
				break;
			}
			if((*s==0x0d && *(s+1)==0x0a))
			{
				s+=2;
				break;
			}

			s++;

			//見つからなかった？
			if((UINT)(s-buff)>=bufflen-2)
			{
				s=NULL;
				break;
			}
		}
		if(!s)break;

		UINT strcount=0;
		while(TRUE)
		{
			//コピー
			katiserifu[strcount] = *s;
			strcount++;
			if(strcount>254)break;

			s++;

			//次の#が見つかってしまったら終了
			if(*s=='#')
			{
				break;
			}

			//終端に到達
			if((UINT)(s-buff)>=bufflen)
			{
				break;
			}
		}
		funcs->setkatiserif(pdat->tid,katiserifu);

	}while(0);

	DELETE_ARRAY(buff);
	DELETE_ARRAY(filepath);
	DELETE_ARRAY(tmpname);
	DELETE_ARRAY(katiserifu);
}

/*!
	@class CBulletBase
	@brief 飛道具基本クラス
	@ingroup GObjectBase

	CGoluahObjectを飛び道具向けに拡張したクラスです。
	act_run() : 飛んでいるときのアクション　、　
	act_bomb() : ヒット後、消滅するまでのアクション　、　
	act_run2() : ヒット後も消滅せずに(攻撃力を失って)飛んでいくアクション　、　
	を実装することにより簡単に飛び道具クラスが作れます。

	攻撃力は通常、atkinfoメンバ変数が使用されます。

	コンストラクタでCCharacterBaseを親クラスとして指定すると、
	描画時に標準のビットマップとGCD、サウンド等が利用できます。
*/


/*!
	@brief 構築(非推奨)
*/
CBulletBase::CBulletBase(GOBJECT *parent/* = NULL */) : CGoluahObject(FALSE)
{
	parent_obj = parent;
	parent_class = NULL;

	CBulletBaseCreate();
}

/*!
	@brief 構築(推奨)
*/
CBulletBase::CBulletBase(CCharacterBase *parent) : CGoluahObject(FALSE)
{
	if(parent!=NULL){
		parent_class = parent;
		parent_obj = parent->GetGObject();
		g_chardir = parent->GetCharDir();
	}
	else {
		parent_class=NULL;
		parent_obj=NULL;
	}
	CBulletBaseCreate();
}


/*!
	@brief コンストラクタ共通処理
*/
void CBulletBase::CBulletBaseCreate()
{
	GOBJECT* pdat = GetGObject();

	if(parent_obj!=NULL){
		pdat->tid = parent_obj->tid;//チームID
		pdat->pmsarr = parent_obj->pmsarr;//gcdは親と同じものを使えるようにしておく
		pdat->pcdat = parent_obj->pcdat;
		pdat->prdat = parent_obj->prdat;
		pdat->phdat = parent_obj->phdat;

		pdat->x = parent_obj->x;
		pdat->y = parent_obj->y;
		pdat->muki = parent_obj->muki;

	}

	pdat->aid = CBB_STATE_IDLE;
	pdat->objtype	= GOBJTYPE_BULLET;//オブジェクトタイプ
	base_z=ZZAHYO_BULLET1;

	ZeroMemory(&atkinfo,sizeof(ATTACKINFO));
	pdat->atk = &atkinfo;

	hitmsg = 0;
	grdmsg = 0;
}


/*-----------------------------------------------------------------------------
	メッセージ処理関数
-------------------------------------------------------------------------------*/
DWORD CBulletBase::Message(DWORD msg,LPVOID pd,DWORD prm)
{
	switch(msg){
	case GOBJMSG_SOUSAI:return( Sousai(prm) );
	default:
		//あとのメッセージはオブジェクト基本クラスに任せる
		return(CGoluahObject::Message(msg,pd,prm));
	}
	return(0);
}

/*!
	@brief GOBJMSG_DRAW メッセージ処理
	idle時は描画しない。idle以外はデフォルトの描画処理。
*/
DWORD CBulletBase::Draw()
{
	if(GetGObject()->aid==CBB_STATE_IDLE)return TRUE;
	return 0;
}

/*!
	@brief GOBJMSG_TOUCHB メッセージ処理（他オブジェクトへの衝突）
*/
DWORD CBulletBase::TouchB(ATTACKINFO *info,BOOL hit)
{
	Hit();
	if(parent_class){
		if(hit){
			if (hitmsg!=0)
				parent_class->Message(hitmsg,parent_obj,hitprm);
			else
				parent_class->TouchB(info, hit);
		}
		else if(grdmsg!=0){
			parent_class->Message(grdmsg,parent_obj,grdprm);
		}
		else
			parent_class->TouchB(info, hit);
	}
	return(TRUE);
}

void CBulletBase::Hit()
{
	pdat->kougeki = FALSE;
	Bomb();
}

/*!
	@brief GOBJMSG_SOUSAI メッセージ処理（他飛道具への衝突）
*/
DWORD CBulletBase::Sousai(DWORD prm)
{
	GetGObject()->kougeki=FALSE;//攻撃力無効化
	GetGObject()->aid = CBB_STATE_BOMB;
	return(TRUE);
}


/*!
	@brief GOBJMSG_CNGROUND メッセージ処理

	ラウンド変化。FALSEを返すと消されちゃう。
	CBulletBaseはTRUEを返し、Hide()します。
*/
BOOL CBulletBase::OnChangeRound()
{
	Hide();
	return TRUE;//通常、消えない
}

/*-----------------------------------------------------------------------------
	状態操作
-------------------------------------------------------------------------------*/

//!オブジェクト破棄
void CBulletBase::Suicide()
{
	Hide();
}

//!消滅アニメーションへ移行
void CBulletBase::Bomb()
{
	ChangeAction(CBB_STATE_BOMB);
}

//!隠す
void CBulletBase::Hide()
{
	ChangeAction(CBB_STATE_IDLE);
}

//!飛ばす
BOOL CBulletBase::Go(BOOL muki,double x,double y,double vx,double vy)
{
	GOBJECT* pdat = GetGObject();

	if (pdat->aid == CBB_STATE_IDLE)
	{
		pdat->muki = muki;
		SetPos(x,y);
		pdat->vx = vx;
		pdat->vy = vy;
		ChangeAction(CBB_STATE_RUN);
		return TRUE;
	}

	return FALSE;
}

/*!
	@brief サウンド再生
	もし親クラスがあれば、そちらのほうに処理を丸投げ
*/
void CBulletBase::PlayMySound(DWORD number)
{
	if(parent_class!=NULL)parent_class->PlayMySound(number);
}

/*!
	@brief GOBJMSG_ACTION メッセージ処理
*/
DWORD CBulletBase::Action()
{
	switch(GetGObject()->aid){
	case CBB_STATE_IDLE:	act_idle();break;
	case CBB_STATE_RUN:		act_run();break;
	case CBB_STATE_BOMB:	act_bomb();break;
	case CBB_STATE_RUN2:	act_run2();break;
	}
	return(TRUE);
}

//!アイドル時処理
void CBulletBase::act_idle()
{
	GOBJECT* pdat = GetGObject();

	pdat->x = 99999;
	pdat->y = 99999;
	pdat->kougeki = FALSE;
}



/*!
	@class CClassicalBullet
	@brief 自動飛道具クラス
	@ingroup GObjectBase

	旧飛道具構造体 BULLETINFO_A をそのまま使用できます。たぶん。
	基本的には旧ソースからの移植用のクラスで、CBulletBaseを
	継承した飛び道具クラスを新規に作ったほうが手間がかからないと思います。

	使用するBULLETINFO_A構造体は、メンバでなくローカル変数にしちゃってOKです。
	構造体そのものをいじらなくなったので、その辺の工夫が必要かも。
*/

/*!
	@brief 構築
*/
CClassicalBullet::CClassicalBullet( CCharacterBase *parent,CDI_CHARACTERINFO2 *info,BULLETINFO_A *bulinfo,
								   BYTE userID,BYTE userNo) : CBulletBase(parent)
{
	array_len_r = 0;
	array_len_d = 0;

	//配列の長さを取得する
	while(bulinfo->cell_run[array_len_r]>0)array_len_r++;
	while(bulinfo->cell_dis[array_len_d]>0)array_len_d++;

	//メモり確保&配列をコピーする
	DWORD i;
	if(array_len_r==0)pcr=NULL;
	else{
		pcr = new int[array_len_r];
		for(i=0;i<array_len_r;i++){
			pcr[i] = bulinfo->cell_run[i];
		}
	}
	if(array_len_d==0)pcd=NULL;
	else{
		pcd = new int[array_len_d];
		for(i=0;i<array_len_d;i++){
			pcd[i] = bulinfo->cell_dis[i];
		}
	}

	//旧攻撃情報配列から新攻撃情報配列に変換する
	atkinfo.damage = bulinfo->atk.damage;
	atkinfo.kezuri = bulinfo->atk.kezuri;
	atkinfo.guard = bulinfo->atk.guard;
	atkinfo.hit = bulinfo->atk.hit;
	atkinfo.muki = FALSE;//旧構造体とはちょっと意味が違う

	//パラメータコピー
	ax = bulinfo->ax;
	ay = bulinfo->ay;
	vx = bulinfo->vx;
	vy = bulinfo->vy;
	spd_r = bulinfo->spd_run;
	spd_d = bulinfo->spd_dis;
	lifedur = bulinfo->dur;
	flags = bulinfo->type;

	//フラグ処理 
	if( !(flags&BULLETA_VSHUMAN) )RemoveProperty(GOBJFLG_ATTACK);
	if( !(flags&BULLETA_VSBULLET) )RemoveProperty(GOBJFLG_ZBULLET);
	if( flags&BULLETA_DRAWBACK )base_z=ZZAHYO_BULLET3;
	if( flags&BULLETA_DRAWMIDDLE )base_z=ZZAHYO_BULLET2;
	if( flags&BULLETA_DISPZAHYO )AddProperty(GOBJFLG_DISPZAHYO);
	if( flags&BULLETA_DONOTSTOP )AddProperty(GOBJFLG_DONOTSTOP);
}

/*!
	@brief 破棄
*/
CClassicalBullet::~CClassicalBullet()
{
	if(pcr)delete [] pcr;
	if(pcd)delete [] pcd;
}


/*!
	@brief その他
*/
BOOL CClassicalBullet::Go(BOOL muki,double x,double y)
{	
	return CBulletBase::Go(muki,x,y,vx,vy);
}

//!ヒット時処理
void CClassicalBullet::Hit()
{
	if(flags&BULLETA_DONOTDIE)ChangeAction(CBB_STATE_RUN2);
	else ChangeAction(CBB_STATE_BOMB);
}

//!飛翔時処理
void CClassicalBullet::act_run()
{
	if(spd_r==0 || array_len_r==0 || pcr==NULL)return;
	GOBJECT* pdat = GetGObject();
	DWORD counter2 = pdat->counter%(spd_r * array_len_r);
	DWORD index = counter2 /= spd_r;

	pdat->cnow = pcr[index];

	pdat->vx += ax;
	pdat->vy += ay;
	movex( pdat->vx );
	pdat->y += pdat->vy;

	pdat->kougeki=TRUE;
	if(pdat->counter > lifedur){
		ChangeAction(CBB_STATE_BOMB);
	}
	else if(flags&BULLETA_XJIMENN){
		if(pdat->y>0){
			pdat->y = 0;
			ChangeAction(CBB_STATE_BOMB);
		}
	}
}

//!飛翔時処理(ヒット後)
void CClassicalBullet::act_run2()
{
	act_run();
	GetGObject()->kougeki=FALSE;
}

//!ヒット後処理
void CClassicalBullet::act_bomb()
{
	GetGObject()->kougeki = FALSE;

	DWORD index = 0;

	if(!(spd_d==0 || pcd==NULL)){
		index = GetGObject()->counter / spd_d;
	}
	if(index >= array_len_d || array_len_d==0 || (spd_d==0 || pcd==NULL)){
		Hide();
		return;
	}
	GetGObject()->cnow = pcd[index];
}

/*!
	@class CBulletList
	@brief 飛び道具リストクラス
	@ingroup GObjectBase

　飛び道具オブジェクトをリスト管理することができます。
　一度に大量発射する飛び道具や、連射したいときに使用してください。
　ふつうの飛び道具でも、動きに障害が出る場合に使用すると便利です。
　CGoluahObjectの派生クラスではないので、動的生成する必要はありません。
　（途中でリストをクリアすることはできませんのでご注意ください。）

*/

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
/*!
	@brief 構築

	通常はこちらを使ってください。
*/
CBulletList::CBulletList()
{
	m_pBullet = NULL;
	pNext = NULL;
	ListCount = 0;
	x = 0;
	y = 0;
	vx = 0;
	vy = 0;
}

/*!
	@brief リストに飛び道具を１つ加えて構築
	@param pBullet リストに追加するCBulletBaseへのポインタ

	使う機会はあんまり無いと思う。
*/
CBulletList::CBulletList(CBulletBase *pBullet)
{
	if (pBullet){
		m_pBullet = pBullet;
		ListCount = 1;
	}
	else {
		m_pBullet = NULL;
		ListCount = 0;
	}

	pNext = NULL;
	x = 0;
	y = 0;
	vx = 0;
	vy = 0;
}

/*!
	@brief あぼんぬ

	リスト内の飛び道具に手は加えません。
*/
CBulletList::~CBulletList()
{
	if (pNext)
		delete pNext;
}

/*!
	@brief リストに飛び道具を追加
	@param pBullet 追加するCBulletBaseへのポインタ

	リストにpBulletが示す飛び道具を１つ追加します。
	CCharacterBase::InitBullets内で全部まとめてやっちゃうことをお勧めします。
	なお一度追加すると元に戻せません、ご注意ください。
*/
void CBulletList::Add(CBulletBase *pBullet)
{
	if (pBullet)
	{
		if (m_pBullet == NULL) {
			m_pBullet = pBullet;
			if (m_pBullet)
				ListCount++;
		}
		else if (pNext == NULL) {
			pNext = new CBulletList(pBullet);
			if (pNext)
				ListCount++;
		}
		else {
			pNext->Add(pBullet);
			ListCount = pNext->GetCount() + 1;
		}
	}
}

/*!
	@brief リスト内の飛び道具を１つ発射(1)
	@sa CBulletBase::Go
	@return 発射に成功した場合はTRUE、失敗した場合はFALSE

	リスト内から発射可能な飛び道具を探し、発射します。
	複数が発射可能な場合は、先に登録されたものが優先されます。
*/
BOOL CBulletList::Go(BOOL muki, double x, double y, double vx, double vy)
{
	if (m_pBullet == NULL)
		return FALSE;

	if (m_pBullet->Go(muki, x, y, vx, vy))
		return TRUE;
	else if (pNext)
		return pNext->Go(muki, x, y, vx, vy);
	else
		return FALSE;
}

/*!
	@brief リスト内の飛び道具を１つ発射(2)
	@sa CBulletBase::Go
	@return 発射に成功した場合はTRUE、失敗した場合はFALSE

	詳しくは(1)を参照。
	省略されたvxとvyには、SetSpeedで設定された速度が使用されます。
*/
BOOL CBulletList::Go(BOOL muki, double x, double y)
{
	if (m_pBullet == NULL)
		return FALSE;

	if (m_pBullet->Go(muki, x, y, vx, vy))
		return TRUE;
	else if (pNext)
		return pNext->Go(muki, x, y);
	else
		return FALSE;
}

/*!
	@brief リスト内の飛び道具を１つ発射(3)
	@sa CBulletBase::Go
	@return 発射に成功した場合はTRUE、失敗した場合はFALSE

	詳しくは(1)を参照。
	省略されたxとyには、SetPosで設定された座標が、
	省略されたvxとvyには、SetSpeedで設定された速度がそれぞれ使用されます。
*/
BOOL CBulletList::Go(BOOL muki)
{
	if (m_pBullet == NULL)
		return FALSE;

	if (m_pBullet->Go(muki, x, y, vx, vy))
		return TRUE;
	else if (pNext)
		return pNext->Go(muki);
	else
		return FALSE;
}

/*!
	@brief リスト内の飛び道具を全て発射(1)
	@sa CBulletBase::Go

	リストに含まれている、全ての飛び道具を発射します。
	省略されたvxとvyには、各々のSetSpeedで設定された速度が使用されます。
*/
void CBulletList::Go_All(BOOL muki, double x, double y)
{
	if (m_pBullet)
		m_pBullet->Go(muki, x, y, vx, vy);

	if (pNext)
		pNext->Go_All(muki, x, y);
}

/*!
	@brief リスト内の飛び道具を全て発射(2)
	@sa CBulletBase::Go

	詳しくは(1)を参照。
	省略されたxとyには、各々のSetPosで設定された座標が、
	省略されたvxとvyには、各々のSetSpeedで設定された速度がそれぞれ使用されます。
*/
void CBulletList::Go_All(BOOL muki)
{
	if (m_pBullet)
		m_pBullet->Go(muki, x, y, vx, vy);

	if (pNext)
		pNext->Go_All(muki);
}

/*
	@brief リストのnum番目の要素を所得
	@param num 要素の番号
	@return num番目の要素を示すCBulletListへのポインタ、無い場合はNULL
	@sa GetNext

	配列と同じような感覚（list[3]の様な形）で、特定の要素を所得することが出来ます。

	ただし実行速度が遅いため、使用は非推奨です。リスト内の走査は出来るだけGetNextを使用してください。
*/
/*CBulletList* CBulletList::operator [](int num)
{
	if (num == 0)
		return this;
	else if (num > 0)
	{
		if (pNext)
			return pNext->operator [](num - 1);
		else
			return NULL;
	}

	return NULL;
}
*/
/****************************************************************************

  CEffectBaseクラス

*****************************************************************************/

CEffectBase::CEffectBase() : CGoluahObject(TRUE)
{
	ceffectbase_private_duration = 20;
}

CEffectBase::~CEffectBase()
{
}

DWORD CEffectBase::Action()
{
	float time = pdat->counter / ceffectbase_private_duration;
	if(time>=1.0f)
	{
		Suicide();
	}
	else
	{
		Update( time );
	}
	return TRUE;
}

void CEffectBase::SetDuration(UINT dur)
{
	if(dur==0){dur=1;}
	ceffectbase_private_duration = (float)dur;
}


/* **************************************************************************

  CStageBaseクラス

 ************************************************************************** */

void CStageBase::SetupStageDLL(SDI_STAGEINFO2 *info)
{
	funco = info->funco;
	funcs = info->funcs;
	funcd = info->funcd;
}

BOOL CStageBase::CheckSTB(DWORD Version/* = STB_VERSION */)
{
	if(Version > STB_VERSION || Version < STB_LEASTVER){ 
#		ifdef _DEBUG
			char *dbgmsg = new char[256];
			sprintf(dbgmsg,"CCharacterInfo:STBバージョン違い(%d!=%d)\n",Version,STB_VERSION);
			OutputDebugString(dbgmsg);
			delete [] dbgmsg;
#		endif
		return FALSE;
	}

	return TRUE;
}

CStageBase::CStageBase(SDI_STAGEINFO2 *info)
				: CGoluahObject(TRUE)//あたり判定は行わない。
{
	g_chardir = info->dir;
	pdat->tid = info->tid;
}


CStageBase::~CStageBase()
{
}
/* **************************************************************************

  残像エフェクト

 ************************************************************************** */

CShadowEffect::CShadowEffect(CCharacterBase* pParent,
				  DWORD color /* = 0x00888888 */,
				  DWORD BlendType /* = GBLEND_HANTOUMEI */,
				  UINT Duration /* = 20 */)
{
	GOBJECT* ppdat;

	if (!pdat)
		return;

	if (!pParent)
	{
		Suicide();
		return;
	}

	ppdat = pParent->GetGObject();

	pdat->tid = ppdat->tid;
	pdat->objtype = GOBJFLG_DONOTSTOP;

	pdat->pcdat = ppdat->pcdat;
	pdat->prdat = ppdat->prdat;
	pdat->phdat = ppdat->phdat;
	pdat->pmsarr = ppdat->pmsarr;

	pdat->counter = 0;
	pdat->cnow = ppdat->cnow;
	pdat->x = ppdat->x;
	pdat->y = ppdat->y;
	pdat->z = ZZAHYO_EFFECT1;
	pdat->rot = ppdat->rot;
	pdat->revx = ppdat->revx;
	pdat->revy = ppdat->revy;
	pdat->muki = ppdat->muki;
	pdat->magx = ppdat->magx;
	pdat->magy = ppdat->magy;

	// メンバ初期化
	m_color = color;
	m_BlendType = BlendType;
	SetDuration(Duration);
}

CShadowEffect::~CShadowEffect(void)
{
}

void CShadowEffect::Update(float time)
{
	// コリンズのソースよりお借りしました、21スレの335さんサンクスコ。
	pdat->color = (m_color & 0x00FFFFFF) | ( (DWORD)(255.0 - time * 20.0 * 25.0) << 24 );
}

DWORD CShadowEffect::Draw(void)
{
	SetBlend(m_BlendType);
	CellDraw(pdat);
	SetBlend(GBLEND_HANTOUMEI);

	return TRUE;
}


/* **************************************************************************

  グローバル関数　とか

 ************************************************************************** */

/*!
	@brief DLLのエントリーポイント？

	ランダム種ぽ
*/
BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
#		ifdef _DEBUG
			num_allocs = 0;
#		endif
		srand(GetTickCount());
		break;

	case DLL_PROCESS_DETACH:
#		ifdef _DEBUG
			if (num_allocs > 0)
			{
				DebugPrintf("DLL:おまいら、メモリリークが発生してます。\n総数：%dバイト", (int)num_allocs);
			}
#		endif
		break;
	}

	return TRUE;
}

//!ランダムナンバー発生
inline int GetRandNum(int num)
{
	return(rand()%num);
}

/*!
	@brief ファイル読み込み

	ファイルを読み込んでメモリ上に載せます。
	buff には new[] で生成した領域のポインタが帰るので、
	関数が成功した場合は呼び出し元でd elete[] します。

	@param filepath	[in] 読み込むファイル名
	@param buff		[out] 読み込み後のバッファ
	@param len		[out] 読み込んだバイト数
	@return	TRUE:成功 , FALSE:残念
*/
BOOL File2Mem(char* filepath,char** buff,UINT *len)
{
	*buff = NULL;
	*len = 0;

	//ファイルのオープン
	HANDLE hFile = CreateFile(filepath,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE)return(FALSE);

	//領域確保
	*len = GetFileSize(hFile,NULL);
	*buff = new char [(*len)+1];
	ZeroMemory(*buff,sizeof(char)*((*len)+1));

	//ファイルの読み込み
	DWORD br;
	ReadFile(hFile,*buff,*len,&br,NULL);
	CloseHandle(hFile);

	return TRUE;
}

/*!
	@brief デバッグ出力

	フォーマットつき(printfとかといっしょ)で
	OutputDebugStringを行います。
*/
void DebugPrintf(char* fmt,...)
{
	va_list args;
	va_start(args, fmt);

	char buffer[512];
	vsprintf(buffer, fmt, args);

	OutputDebugString(buffer);
	OutputDebugString("\n");

	va_end(args);
}


void DebugPrintfDummy(char* fmt,...){}

#endif//RUNTIME_BUILD_OFF