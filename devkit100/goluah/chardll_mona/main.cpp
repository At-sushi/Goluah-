#include <windows.h>
#include <stdio.h>
#include <d3d8.h>

#define EXPORT extern "C" __declspec(dllexport) 

#include "define_gcd.h"
#include "gobject.h"
#include "gobjbase.h"
#include "character.h"

char *g_chardir=NULL;
DI_FUNCTIONS_O *funco=NULL;
DI_FUNCTIONS_S *funcs=NULL;
DI_FUNCTIONS_D *funcd=NULL;
DI_FUNCTIONS_N *funcn=NULL;

extern void SetOptionItems(CDI_CHARACTERINFO *pif);

//===========================================================
extern char charactername[32];

//===========================================================
//起動時に呼び出される。
EXPORT BOOL CharacterInfo(LPVOID info)
{
	CDI_CHARACTERINFO *pif = (CDI_CHARACTERINFO*)info;
	sprintf(pif->name,charactername);//名前
	pif->ver=CDI_VERSION;//バージョン

	if(pif->system_version!=0){
		pif->caps |= CHARACTER_CAPS_NET;
		SetOptionItems(pif);
	}

	return(TRUE);
}

EXPORT DWORD CreateCharacter(PVOID info)
{
	CDI_CHARACTERINFO2 *info2 = (CDI_CHARACTERINFO2*)info;

	funco = info2->funco;
	funcs = info2->funcs;
	funcd = info2->funcd;
	funcn = info2->funcn;
	g_chardir = info2->dir;

	CCharacter* pc = new CCharacter(info2);
	return(pc->GetObjectID());
}
