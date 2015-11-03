#include "StdAfx.h"
#define LPDIRECT3DTEXTURE9 LPVOID

// #define EXPORT extern "C" __declspec(dllexport) 

#include "define_const.h"
#include "define_macro.h"
#include "global.h"

#include "define_gcd.h"
#include "gobject.h"
#include "define_data.h"
#include "define_char.h"
#include "mimikaki.h"
#include "adaptor.h"
// #include "CharacterList.h"


char *g_chardir=NULL;		// 要対応
DI_FUNCTIONS_O *funco=NULL;
DI_FUNCTIONS_S *funcs=NULL;
DI_FUNCTIONS_D *funcd=NULL;
FUNCTIONPACK oldfunc;
// CCharAdaptorList clist;
// 本体への統合に伴いクラスから随時上書きする方式に変更
GOBJECT *pchar[2] = {NULL, NULL};	// それぞれのチームの立場での、味方。
DWORD pnagerare[2];					// 投げられてる香具師（敵）のＩＤ
extern WAZAINFO waz;		// 要対応
extern DWORD ObjID[];

//===========================================================
// char charactername[32] = "(ｱﾀﾞﾌﾟﾀｰ使用)";

//===========================================================
// キャラアダプターでのIDの扱い：
// 下位ワードに旧バージョンでのＩＤ（０か１）が割り振られている
// 自分と同じ側なら自分自身、違う側の番号だったら敵、と言う感じ。
// 上位ワードがONの場合は投げられてる相手を指すみたい

#define GETOBJ(id)		(HIWORD(id) ? ((GOBJECT*)funco->getinfo(pnagerare[LOWORD(id)])) :\
	(pchar[id] ? pchar[id] : ((GOBJECT*)funco->getinfo(pchar[!id]->eid))))
#define GETCLASS(id)	((CCharAdaptor*)GETOBJ(LOWORD(id))->pobjdat_u) // 敵データの時は使わないこと

// FUNCTIONPACK用関数
DWORD GetKey(DWORD id,DWORD nanka) {
	if (HIWORD(id)) return 0;
	return funcs->getkey(GETCLASS(id)->keynumber, nanka);}
void DoFurimuki(DWORD id) {	Furimuki(GETOBJ(id)->id); }
void DoStartAttack(DWORD id, LPVOID atk) {
	StartAttack(GETOBJ(LOWORD(id))->id, (MYATTACKINFO*)atk); }
int SeekKey(DWORD id,int nanka,int hen, DWORD dane) {
	if (HIWORD(id)) return -1;
	return funcs->seekkey(GETCLASS(id)->keynumber, nanka, hen, dane);}
void SetKatiSerif(DWORD id, char* serif) {
	funcs->setkatiserif(GETOBJ(id)->tid, serif); }
void PlayMy(DWORD id, DWORD num) {
	if (!HIWORD(id))
		GETCLASS(id)->PlayMySound(num); }
void DoGoBullet(LPVOID bul) {
	GoBulletA(GETOBJ(((BULLETINFO_A*)bul)->cid), (BULLETINFO_A*)bul); }
void SetWazainfo(DWORD id, LPVOID wazainfo)
{
	OLDWAZAINFO* waza = (OLDWAZAINFO*)wazainfo;

	ZeroMemory(&waz,sizeof(WAZAINFO));
	waz.walkf = waza->walkf;
	waz.walkb = waza->walkb;
	waz.jampf = waza->jampf;
	waz.jampb = waza->jampb;
	waz.dashf = waza->dashf;
	waz.dashb = waza->dashb;
	for(int i=0;i<6;i++){
		waz.att_short[i] = waza->att_short[i];
		waz.att_middle[i] = waza->att_middle[i];
		waz.att_long[i] = waza->att_long[i];
		waz.att_bullet[i] = waza->att_bullet[i];
		waz.att_jamp[i] = waza->att_jamp[i];
		waz.att_tai[i] = waza->att_tai[i];
	}

	GETOBJ(id)->winfo = &waz;
}
void DoDrawFront(DWORD id, BOOL isfront) {
	DrawFront(GETOBJ(id), isfront); }
BOOL DoCatchEnemy(DWORD id, LPVOID ufocatcher) {
	CATCHYOU cy; cy.oid = GETOBJ(id)->id; cy.actmsg = MYGOBJMSG_NAGE;
	if (HIWORD(id)) return FALSE;
	GETCLASS(id)->pfNagerare = ((OLDCATCHYOU*)ufocatcher)->pf_nagerare;
	return funco->objcatch(GETOBJ(id)->eid, &cy);
}
void Enemyname(DWORD id, char* buff) {
	lstrcpy(buff, funco->getcharname(GETOBJ(id)->eid));
	if (lstrcmp(buff, "8頭身") == 0) wsprintf(buff, "８頭身");
}
void GetEnemyDat(DWORD id, LPVOID pdat) {
	CHARACTERINFO2& old = *(CHARACTERINFO2*)pdat;
	pnagerare[id] = GETOBJ(id)->eid;
	old.cid = 0x00010000 | id;
	old.hlib = NULL;
	old.fpack = &oldfunc;
	Exchange((GOBJECT*)funco->getinfo(GETOBJ(id)->eid), &old, FALSE);
}
void GetMaai(DWORD id, DWORD* ph, DWORD* pv, DWORD* peaid) {
	GOBJECT* pedat = (GOBJECT*)funco->getinfo(GETOBJ(id)->eid);
	*ph = funco->getmaai_h(GETOBJ(id)->id, pedat->id);
	*pv = funco->getmaai_v(GETOBJ(id)->id, pedat->id);
	*peaid = pedat->aid;
}
void SetWazainfo2(DWORD id, LPVOID winfo2) { }
void SetCharinfo3(DWORD id, LPVOID pinfo3) {
	CHARACTERINFO3* pcinfo = (CHARACTERINFO3*)pinfo3;
	if (HIWORD(id)) return;
	GETOBJ(id)->hpmax = pcinfo->hpmax;
	GETOBJ(id)->gaugetype = pcinfo->pgauge;
	GETOBJ(id)->gaugemax = pcinfo->pgaugemax;
	GETCLASS(id)->g_blown = pcinfo->ppgauge_hit;
	GETCLASS(id)->g_guard = pcinfo->ppgauge_guard;
}
void AddDamage(DWORD eid, LPVOID atk, int x, int y) {
	GETOBJ(LOWORD(eid))->atk =
		ResisterAttackInfo(GETOBJ(LOWORD(eid))->id, (MYATTACKINFO*)atk);
	funco->adddamage(GETOBJ(LOWORD(eid))->id, GETOBJ(eid)->id, x, y);
	if (!HIWORD(eid))
		GETCLASS(eid)->olddat.hp -= ((MYATTACKINFO*)atk)->damage;}
void ExtWin(DWORD ext) {
	extwin = ext;}
BOOL DoAddPowGauge(DWORD id, double addgauge) {
	if (GETOBJ(id)->gauge + addgauge < 0) return FALSE;
	AddPowerGauge(GETOBJ(id), addgauge);
	if (!HIWORD(id))
		GETCLASS(id)->olddat.gauge = GETOBJ(id)->gauge;return TRUE;}
void AddEffect(DWORD efid, int prm1, int prm2, DWORD prm3) {
	if (efid == OLDEFCTID_CYOHI) ADDEFFECT(EFCTID_CYOHI, prm1, prm2, prm3);
	else ADDEFFECT(efid, prm1, prm2, prm3);}
void DestroyBul(LPVOID pbul) {
	if (HIWORD(((BULLETINFO_A*)pbul)->state) < 256)
		funco->suicide(ObjID[HIWORD(((BULLETINFO_A*)pbul)->state)]);
}
char* GetDirectory(DWORD id) {
	if (HIWORD(id)) return NULL;
	return GETCLASS(id)->m_chardir;}

DWORD gMessage(DWORD msg,LPVOID dat,DWORD prm1)
{
	if(dat==NULL)return(0);
	GOBJECT *pdat = (GOBJECT*)dat;
	CCharAdaptor *pc = (CCharAdaptor*)(pdat->pobjdat_u);

	if(pc!=NULL){
		if(msg==GOBJMSG_DELETE){
			pc->Message(msg,dat,prm1);
			delete(pc);
		}
		return(pc->Message(msg,dat,prm1));
	}

	return(0);
}

//===========================================================
//起動時に呼び出される。
//（゜゜）ｲﾗﾈ
/*EXPORT BOOL CharacterInfo(LPVOID info)
{
	CDI_CHARACTERINFO *pif = (CDI_CHARACTERINFO*)info;

	wsprintf(pif->name, g_charlist.GetCharacterName);
	pif->ver=CDI_VERSION;//バージョン
	return(TRUE);
}*/

// bool verold = false;

DWORD CCharAdaptor::CreateCharacter(PVOID info)
{
	CDI_CHARACTERINFO2 *info2 = (CDI_CHARACTERINFO2*)info;
//	BOOL (*pCharInfo)(LPVOID);
	char buffer[256];
//	CHARACTERINFO cinfo;

	//必要な情報をグローバルに格納
	funco = info2->funco;
	funcs = info2->funcs;
	funcd = info2->funcd;
	g_chardir = info2->dir;
	m_chardir = g_chardir;

	oldfunc.pf_getkey = GetKey;
	oldfunc.pf_furimuki = DoFurimuki;
	oldfunc.pf_startattack = DoStartAttack;
	oldfunc.pf_seekkey = SeekKey;
	oldfunc.pf_setkatiserif = SetKatiSerif;
	oldfunc.pf_playmysound = PlayMy;
	oldfunc.pf_gobulleta = DoGoBullet;
	oldfunc.pf_addeffect = AddEffect;
	oldfunc.pf_setwazainfo = SetWazainfo;
	oldfunc.pf_drawfront = DoDrawFront;
	oldfunc.pf_catchenemy = DoCatchEnemy;
	oldfunc.pf_enemyname = Enemyname;
	oldfunc.pf_getenemydat = GetEnemyDat;
	oldfunc.pf_getmaai = GetMaai;
	oldfunc.pf_playsyssound = funcs->playsyssound;
	oldfunc.pf_setwazainfo2 = SetWazainfo2;
	oldfunc.pf_setcharinfo3 = SetCharinfo3;
	oldfunc.pf_adddamage = AddDamage;
	oldfunc.pf_extwindur = ExtWin;
	oldfunc.pf_addpowgauge = DoAddPowGauge;
	oldfunc.pf_destroybullet = DestroyBul;
	oldfunc.pf_getdirectory = GetDirectory;

	if (!hAction2)
	{
		wsprintf(buffer, "%s\\action", info2->dir);
		hAction2 = LoadLibrary(buffer);
		if(hAction2 == NULL) // 失敗
			return 0;

/*		pCharInfo = (BOOL (*)(LPVOID))
			GetProcAddress(hAction2, "CharacterInfo");
		if (pCharInfo == NULL) return FALSE;

		cinfo.dds_face = NULL;
		cinfo.dds_face2 = NULL;
		strncpy(cinfo.dir, info2->dir, 32);
		cinfo.dir[31] = '\0';
		cinfo.storyindex = cinfo.storynum = 0;
		if (!pCharInfo(&cinfo)) return FALSE;
*/ //		if (strlen(cinfo.name) + strlen(charactername) <= 31)
//			strcat(cinfo.name, charactername);//名前
//		if (cinfo.ver < 400 || cinfo.ver >= 600) verold = true;		// 定義済み
	}

//	if (verold) return 0;
	InitAdaptor(info2);
	return(oid);
}
