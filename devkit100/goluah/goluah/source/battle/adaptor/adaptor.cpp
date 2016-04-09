/*
2011/10/29	Marvelで空中での交代を不可に
*/
#include "StdAfx.h"

#define LPDIRECT3DTEXTURE9 LPVOID

#include "define_gcd.h"
#include "gobject.h"
#include "define_data.h"
#include "define_char.h"
#include "mimikaki.h"
#include "adaptor.h"
// #include "CharacterList.h"
// #include "cell.h"

DWORD extwin = 250;		// 対応できる？
extern GOBJECT* pchar[2];
extern DWORD pnagerare[2];

DWORD gMessage(DWORD msg,LPVOID dat,DWORD);

extern TCHAR *g_chardir;
extern DI_FUNCTIONS_S *funcs;
extern DI_FUNCTIONS_O *funco;
extern DI_FUNCTIONS_D *funcd;
extern FUNCTIONPACK oldfunc;
extern HMODULE hAction2;

WAZAINFO waz;//技情報
ATTACKINFO atkmuki;

DWORD gMessage(DWORD msg,LPVOID dat,DWORD);
void GetEnemyDat(DWORD id, LPVOID pdat);

//**************************************************************************************
//  Create Character
//**************************************************************************************

CCharAdaptor::CCharAdaptor()
{
	hAction2 = NULL;
	m_pchar[0] = m_pchar[1] = NULL;
	m_chardir=NULL;
}

void CCharAdaptor::InitAdaptor(CDI_CHARACTERINFO2 *info)
{
	BOOL (*pCreate)(DWORD,PVOID);

	oid = (*funco->objcreate)();//オブジェクトを生成
	pdat = (GOBJECT*)(*funco->getinfo)(oid);//データへのポインタをget
	keynumber = info->keyinput;//何番からキー入力を受け取るか

	//データを初期化
	pdat->tid			= info->tid;//チームID
	pdat->msghandler	= gMessage;//メッセージ処理関数
	pdat->objtype		= GOBJTYPE_CHARACTER;//オブジェクトタイプ
	pdat->hpmax			= 1000;//最大体力値
//	pdat->gaugemax		= 1;//最大ゲージ
//	pdat->gaugetype		= GOBJFLGA_POWGAUGE2;//ゲージタイプ
	pdat->winfo			= NULL;
	pg_hit = 0.0;
	pg_guard = 0.0;
	g_blown = 0.0;
	g_guard = 0.0;
	pfNagerare = NULL;

	pCreate = (BOOL (*)(DWORD,PVOID))
		GetProcAddress(hAction2, "CreateCharacter");
	if(pCreate == NULL){ // 失敗
		funco->suicide(pdat->id);
		oid = 0; return;
	}

	ZeroMemory(&olddat, sizeof(olddat));
	olddat.cid = pdat->tid;
	if (m_pchar[olddat.cid]) {
		olddat.cid = !olddat.cid;
		if (m_pchar[olddat.cid]) {
			funco->suicide(pdat->id);
			oid = 0; return;
		}
	}
	olddat.hlib = hAction2;
	Exchange(pdat, &olddat, FALSE);
	olddat.fpack = &oldfunc;
	pdat->pobjdat_u = this;
	m_pchar[olddat.cid] = pdat;
	memcpy(pchar, m_pchar, sizeof(pchar));
	if (!pCreate(olddat.cid, &olddat)){ // 失敗
		funco->suicide(pdat->id);
		oid = 0; return;
	}
	Exchange(pdat, &olddat, TRUE);

	InitializeGCDandBMP(pdat,info->color);//ビットマップとcell.gcdを読み込み
	InitializeMySound();//サウンドの読み込み
	InitAttackInfo();//攻撃力情報の初期化

	Exchange(pdat, &olddat, FALSE);
	if (olddat.initchar != NULL)
		olddat.initchar();
	Exchange(pdat, &olddat, TRUE);;
}


//**************************************************************************************
//   システムからのメッセージ
//**************************************************************************************

DWORD CCharAdaptor::Message(DWORD msg,LPVOID dat,DWORD prm)
{
	CHARACTERINFO2 objdat;
	GOBJECT *pobjdat;
	DWORD stock;
	int hp;
	BULLETATKINFO* pbulainfo;

	// キャラのIDをグローバル変数に送る
	if (msg != GOBJMSG_DELETE)
	{
		memcpy(pchar, m_pchar, sizeof(pchar));
		memcpy(pnagerare, m_pnagerare, sizeof(pnagerare));
	}

	switch(msg){
	case GOBJMSG_ACTION:
		return(Action());
	case GOBJMSG_COMMAND:
		Command();
		return(TRUE);
	case GOBJMSG_COMMANDCOM:
		return(CommandCOM(prm));
	case GOBJMSG_DRAW://描画はシステムに任せる
		return(FALSE);
	case GOBJMSG_TOUCHA:
		return(TouchA(pdat->atk2.info1));
	case GOBJMSG_TOUCHB:
		return(TouchB(pdat->atk,prm));
	case GOBJMSG_DOTOJYO:
		pdat->aid = ACTID_TOJYO;
		funco->actidchanged(pdat->id);
		return(TRUE);
	case GOBJMSG_DOYOUWIN://勝利
		pdat->aid = ACTID_SYORI;
		funco->actidchanged(pdat->id);
		return(TRUE);
	case GOBJMSG_TAIKI://待機しとけメッセージ
		pdat->aid = ACTID_TAIKI;
		pdat->objtype &= GOBJTYPE_NODISPLAY;//画面外へいけるようにしておく
		pdat->objtype |= GOBJFLG_TAIKI;//待機フラグを立てる
		pdat->objtype &= ~GOBJFLG_TOBETARGET;//ターゲットにはならない
		(*funco->actidchanged)(pdat->id);
		break;
	case GOBJMSG_KOUTAI://交代しろメッセージ
		if(pdat->aid == ACTID_TAIKICYU){
			pdat->aid = ACTID_KOUTAIIN;
			pdat->objtype |= GOBJFLG_TOBETARGET;//ターゲットなる
			(*funco->actidchanged)(pdat->id);
			return(YEAH);
		}
		return(0);
		break;
	case GOBJMSG_STRIKER://支援攻撃要請
		return(FALSE);//支援攻撃は行いません
		break;
	case GOBJMSG_KOUTAI2://交代しろメッセージ2（のんびりと登場ポーズを決めてて良し）
		pdat->aid = ACTID_KOUTAI;
		pdat->objtype |= GOBJTYPE_DISPLAY;//画面内にいる
		pdat->objtype |= GOBJFLG_TOBETARGET;//ターゲットになる
		funco->actidchanged(pdat->id);
		break;
	case GOBJMSG_CLIPX:
		if(pdat->aid == ACTID_KOUTAI)return(TRUE);
		else return(0);
	case GOBJMSG_CNGROUND:
		return(TRUE);//ラウンドが変わっても消えない
	case GOBJMSG_CNGAID:
		ActionIDChanged();
		break;
	case MYGOBJMSG_NAGE:
		stock = 0;
		pobjdat = (GOBJECT*)funco->getinfo(prm);
		GetEnemyDat(olddat.cid, &objdat); // 旧式に翻訳
		m_pnagerare[olddat.cid] = prm;
		objdat.cid = 0x00010000 | olddat.cid;
		Exchange(pobjdat, &objdat, FALSE);
		Exchange(pdat, &olddat, FALSE);
		if (pfNagerare != NULL)
			stock = pfNagerare(&objdat);
		hp = pobjdat->hp;
		Exchange(pdat, &olddat, TRUE);
		Exchange(pobjdat, &objdat, TRUE);
		pobjdat->hp = ((objdat.hp > hp) ? hp : objdat.hp);
		switch (pobjdat->cnow) {
		case DCELL_NAGERARE2:
			pobjdat->cnow	= DCELL_NAGERARE1;
			pobjdat->rot	= 45;
			break;
		case DCELL_NAGERARE3:
			pobjdat->cnow	= DCELL_NAGERARE1;
			pobjdat->rot	= 90;
			break;
		case DCELL_NAGERARE4:
			pobjdat->cnow	= DCELL_NAGERARE1;
			pobjdat->rot	= 135;
			break;
		case DCELL_NAGERARE5:
			pobjdat->cnow	= DCELL_NAGERARE1;
			pobjdat->rot	= 0;
			break;
		default:
			pobjdat->rot	= 0;
			break;
		}
		if (stock != 0 && (pobjdat->muki == pdat->muki))
		{
			pdat->atk = &atkmuki;
			funco->adddamage(pdat->id, pobjdat->id, 0, 0);
		}
		return stock;
	case MYGOBJMSG_BULLETHIT:
		pbulainfo = (BULLETATKINFO*)prm;// 強引
		return(BulletHit(&((STRUCT_BULLETA*)pbulainfo->pbul->pobjdat_u)->
			pbinfo_a->atk, pbulainfo->hit));
	default:
		return(0);//あとは、知らん
	}
	return(TRUE);
}


CCharAdaptor::~CCharAdaptor()
{
	Exchange(pdat, &olddat, FALSE);
	if (olddat.deletechar != NULL)
		olddat.deletechar();
	UnloadMyBitmaps(pdat);
	UnloadMySound();
	m_pchar[olddat.cid] = NULL;

    if (hAction2)
	{
		FreeLibrary(hAction2);
		hAction2 = NULL;
    }
}

void CCharAdaptor::Command()
{
	DWORD key_now = (*funcs->getkey)(keynumber,0);
	BOOL callstriker=FALSE;

	if(pdat->aid & ACTID_SYSTEM){//システム定義動作中なので、基本的にはいじらない
		Exchange(pdat, &olddat, FALSE);
		if (olddat.command_a != NULL)
			olddat.command_a();
		Exchange(pdat, &olddat, TRUE);
		return;
	}
//	if(pdat->aid == ACTID_TOJYO || pdat->aid==ACTID_SYORI)return;
//	if(pdat->aid & ACTID_NAGE)return;
	if(pdat->aid & ACTID_INOUT)return;

	//ストライカーを呼ぶ
	if( (key_now&KEYSTA_FOWORD) && (key_now&KEYSTA_DOWN) && (key_now&KEYSTA_BD2) ){
		if((*funcs->msg2system)(pdat->id,MSGOBJ2SYS_STRIKER1,0)){
			ADDEFFECT(EFCTID_COMEON,(int)pdat->x,(int)pdat->y-100,0);
			callstriker=TRUE;
		}
	}
	else if( (key_now&KEYSTA_BACK) && (key_now&KEYSTA_DOWN) && (key_now&KEYSTA_BD2) ){
		if((*funcs->msg2system)(pdat->id,MSGOBJ2SYS_STRIKER2,0)){
			ADDEFFECT(EFCTID_COMEON,(int)pdat->x,(int)pdat->y-100,0);
			callstriker=TRUE;
		}
	}


/*	if(pdat->aid & ACTID_HISSATU){//必殺技中（基本的にいじるべからず）
		return;
	}

	//必殺技コマンド判定
	if(pdat->aid & ACTID_KUCYU){//空中必殺技のコマンド判定
	}
	else{//地上必殺技のコマンド判定
	}

	if(pdat->aid & ACTID_ATTACK){//攻撃動作中（基本的にいじらない。チェーンコンボ出すならここでいじる）
		if(pdat->aid & ACTID_KUCYU){////ジャンプ動作中
			return;
		}
		else{
			if(key_now & 0x22220000){
				if(key_now & KEYSTA_DOWN){
					if(key_now & KEYSTA_BC2){if(ChainCombo(CHAIN_CC)){pdat->aid = ACTID_ATT_CC;return;}}
					else if(key_now & KEYSTA_BB2){if(ChainCombo(CHAIN_CB)){pdat->aid = ACTID_ATT_CB;return;}}
					else if(key_now & KEYSTA_BA2){if(ChainCombo(CHAIN_CA)){pdat->aid = ACTID_ATT_CA;return;}}
				}
				else{
					if(key_now & KEYSTA_BC2){if(ChainCombo(CHAIN_SC)){pdat->aid = ACTID_ATT_SC;return;}}
					else if(key_now & KEYSTA_BB2){if(ChainCombo(CHAIN_SB)){pdat->aid = ACTID_ATT_SB;return;}}
					else if(key_now & KEYSTA_BA2){if(ChainCombo(CHAIN_SA)){pdat->aid = ACTID_ATT_SA;return;}}
				}
			}
		}
		return;
	}

	if(pdat->aid & ACTID_KUCYU){//ジャンプ動作中
		if(key_now & 0x22220000){
			if(key_now & KEYSTA_BC2){pdat->aid = ACTID_ATT_JC;return;}
			else if(key_now & KEYSTA_BB2){pdat->aid = ACTID_ATT_JB;return;}
			else if(key_now & KEYSTA_BA2){pdat->aid = ACTID_ATT_JA;return;}
		}
		return;
	}

	//基本動作中
	if(key_now & 0x22220000){
		if(key_now & KEYSTA_DOWN){
			if(key_now & KEYSTA_BC2){pdat->aid = ACTID_ATT_CC;return;}
			else if(key_now & KEYSTA_BB2){pdat->aid = ACTID_ATT_CB;return;}
			else if(key_now & KEYSTA_BA2){pdat->aid = ACTID_ATT_CA;return;}
		}
		else{
			if(key_now & KEYSTA_BC2){
				pdat->aid = ACTID_ATT_SC;return;
			}
			else if(key_now & KEYSTA_BB2){
				pdat->aid = ACTID_ATT_SB;return;
			}
			else if(key_now & KEYSTA_BA2){
				pdat->aid = ACTID_ATT_SA;return;
			}
		}
	}

	if(pdat->aid == ACTID_JAMPS){
		if(key_now & KEYSTA_FOWORD){//前ジャンプ
			pdat->aid = ACTID_JAMPF;
			return;
		}
		else if(key_now & KEYSTA_BACK){//後ジャンプ
			pdat->aid = ACTID_JAMPB;
			return;
		}
	}

	if(com66(20)){pdat->aid = ACTID_DASH; return;}
	if(com44(20)){pdat->aid = ACTID_DASHB;return;}
*/
	Exchange(pdat, &olddat, FALSE);
	if (olddat.command_a != NULL)
		olddat.command_a();
	Exchange(pdat, &olddat, TRUE);

	//ストライカー
	if(callstriker){
		pdat->aid = ACTID_STRIKERCOMEON;
		return;
	}
	//こーたい
	else if( (key_now&KEYSTA_FOWORD) && (key_now&KEYSTA_BD2) && !(key_now&KEYSTA_DOWN) && pdat->y == 0 ){
		if((*funcs->msg2system)(pdat->id,MSGOBJ2SYS_KOUTAI1,0)){
			pdat->aid = ACTID_KOUTAIOUT;
			return;
		}
	}
	else if( (key_now&KEYSTA_BACK) && (key_now&KEYSTA_BD2) && !(key_now&KEYSTA_DOWN)  && pdat->y == 0 ){
		if((*funcs->msg2system)(pdat->id,MSGOBJ2SYS_KOUTAI2,0)){
			pdat->aid = ACTID_KOUTAIOUT;
			return;
		}
	}

	if(pdat->aid==ACTID_STRIKERCOMEON){//入力がなければポーズキメ
		if(!(key_now & (KEYSTA_UP2|KEYSTA_DOWN2|KEYSTA_FOWORD2|KEYSTA_BACK2))){
			return;
		}
	}

}

DWORD CCharAdaptor::CommandCOM(DWORD wid)
{
	BOOL stock = 0;

	if(pdat->aid & ACTID_SYSTEM){
		Exchange(pdat, &olddat, FALSE);
		if (olddat.command_b != NULL)
			stock = olddat.command_b(wid);
		Exchange(pdat, &olddat, TRUE);
		return(stock);
	}
//	if(pdat->aid == ACTID_TOJYO || pdat->aid==ACTID_SYORI)return(FALSE);
//	if(pdat->aid & ACTID_NAGE)return(FALSE);
	if(pdat->aid & ACTID_INOUT)return(FALSE);

	Exchange(pdat, &olddat, FALSE);
	if (olddat.command_b != NULL)
		stock = olddat.command_b(wid);
	Exchange(pdat, &olddat, TRUE);
	return(stock);
/*	if(pdat->aid & ACTID_HISSATU){//必殺技中（基本的にいじるべからず）
		return(FALSE);
	}

	if(pdat->aid & ACTID_KUCYU){
		if(pdat->aid & ACTID_ATTACK){//空中攻撃中
			if(wid&ACTID_HISSATU && wid&ACTID_KUCYU)return(TRUE);//空中必殺技ならOK
			return(FALSE);//それ以外はだめ
		}
		else{//空中通常動作中
			if(wid&ACTID_ATTACK && wid&ACTID_KUCYU)return(TRUE);//空中通常技ならOK
			return(FALSE);//それ以外はだめ
		}
	}
	else{
		if(pdat->aid & ACTID_ATTACK){//地上攻撃中
			if(wid&ACTID_HISSATU && !(wid&ACTID_KUCYU))return(TRUE);//地上必殺技ならOK
			return(FALSE);//それ以外はだめ
		}
		else{//地上通常動作
			if(wid&ACTID_KUCYU && wid&ACTID_ATTACK)return(FALSE);//空中攻撃はだめ
			return(TRUE);//それ以外ならOK
		}
	}*/
}

DWORD CCharAdaptor::Action()
{
	DWORD nowaid = pdat->aid;
	Exchange(pdat, &olddat, FALSE);
	if (olddat.action != NULL)
		olddat.action();
	Exchange(pdat, &olddat, TRUE);

	if(nowaid == ACTID_TOJYO && pdat->aid == ACTID_NEUTRAL)
		(*funcs->msg2system)(pdat->id,MSGOBJ2SYS_TOJYOEND,0);

	if(pdat->aid == ACTID_SYORI)
	{
		if (pdat->counter == 0)
			LoadAndSetKatiSerif(pdat->tid);
		if (pdat->counter == extwin)
			(*funcs->msg2system)(pdat->id,MSGOBJ2SYS_WINPOSEEND,0);
	}

	if(!(pdat->aid & 0xFFFD0000)){
		Furimuki(pdat->id);
		if(pdat->counter > 2)chainlist=0;
	}

	switch(pdat->aid){
	case ACTID_KOUTAI	:act_koutai();break;
	case ACTID_KOUTAI2	:act_koutai2();break;
	case ACTID_TAIKI	:act_taiki();break;
	case ACTID_KOUTAIOUT:act_koutai_out();break;
	case ACTID_KOUTAIIN	:act_koutai_in();break;
	case ACTID_STRIKERCOMEON:act_strikercomeon();break;
	default:return(0);
	}

	if(!(pdat->aid & 0xFFFD0000)){
		Furimuki(pdat->id);
		if(pdat->counter > 2)chainlist=0;
	}
	return(1);
}

DWORD CCharAdaptor::TouchA(ATTACKINFO *info)
{
	DWORD key_now = 0, nowaid = pdat->aid;
	MYATTACKINFO makurai;

	if(pdat->aid & ACTID_NAGE || pdat->aid & ACTID_INOUT)
		return(TOUCHA_AVOID);

	GetMyAttackInfo(pdat->atk2.oid, info, &makurai, FALSE);
	Exchange(pdat, &olddat, FALSE);
	if (olddat.touch_a != NULL)
		key_now = olddat.touch_a(&makurai);
	Exchange(pdat, &olddat, TRUE);

	if (pdat->aid != nowaid)
		funco->actidchanged(pdat->id);

	switch(key_now) {
	case TOUCHA_KURAIS:
	case TOUCHA_KURAIC:
	case TOUCHA_KURAIJ:
		AddPowerGauge(pdat, g_blown);
		break;
	case TOUCHA_GUARDS:
	case TOUCHA_GUARDC:
	case TOUCHA_GUARDJ:
		AddPowerGauge(pdat, g_guard);
		break;
	case 0:
		return(TOUCHA_AVOID);
	}

	return(key_now);//あとはデフォルトに任せるぜ
}

DWORD CCharAdaptor::TouchB(ATTACKINFO *info,BOOL hit)
{
	DWORD stock = 0, nowaid = pdat->aid;
	MYATTACKINFO mainfo;

	if (info == &atkmuki)
		return 0;

	GetMyAttackInfo(pdat->id, info, &mainfo, TRUE);
	Exchange(pdat, &olddat, FALSE);
	if (olddat.touch_b != NULL)
		stock = olddat.touch_b(&mainfo, hit);
	Exchange(pdat, &olddat, TRUE);

	//ゲージの増加
	if(hit) AddPowerGauge(pdat, pg_hit);
	else AddPowerGauge(pdat, pg_guard);

//	if(pdat->gauge > pdat->gaugemax)
//		pdat->gauge = pdat->gaugemax;

	if (pdat->aid != nowaid)
		funco->actidchanged(pdat->id);

	return(stock);
}

void CCharAdaptor::ActionIDChanged()
{
	pdat->z = ZZAHYO_CHARACTER;
}

void CCharAdaptor::movex(double mx)
{
	if(pdat->muki)mx*=-1;
	pdat->x += mx;
}

int CCharAdaptor::zurex(int x)
{
	if(pdat->muki)return(x*-1);
	else return(x);
}

DWORD CCharAdaptor::BulletHit(MYATTACKINFO *info, BOOL hit)
{
	DWORD stock = 0, nowaid = pdat->aid;

	Exchange(pdat, &olddat, FALSE);
	if (olddat.touch_b != NULL)
		stock = olddat.touch_b(info, hit);
	Exchange(pdat, &olddat, TRUE);

	if(hit) AddPowerGauge(pdat, info->pg_hit);
	else AddPowerGauge(pdat, info->pg_guard);

	if (pdat->aid != nowaid)
		funco->actidchanged(pdat->id);

	return(stock);
}
