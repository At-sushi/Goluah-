/*================================================================================

	DLLに対して提供する関数の定義

==================================================================================*/

#include "stdafx.h"

#include "define_macro.h"
#include "global.h"
#include "export.h"
#include "gcdhandler.h"
#include "task_loading.h"

//*********************************************************************************
// Initialize
//*********************************************************************************

char* CExport::last_funcname = "none";
BOOL  CExport::func_in = FALSE;

#define FUNC_IN		last_funcname=__FUNCTION__;func_in=TRUE
#define FUNC_OUT	func_in=FALSE

CExport::CExport()
{
	ZeroMemory( &fpack_s , sizeof(DI_FUNCTIONS_S) );
	ZeroMemory( &fpack_o , sizeof(DI_FUNCTIONS_O) );
	ZeroMemory( &fpack_d , sizeof(DI_FUNCTIONS_D) );
}

void CExport::Initialize()
{
	//system
	fpack_s.getkey			= CExport::GetKey;
	fpack_s.seekkey			= CExport::SeekKey;
	fpack_s.setkatiserif	= CExport::SetKatiSerif;
	fpack_s.addeffect		= CExport::AddEffect;
	fpack_s.playsyssound	= CExport::PlaySystemSound;
	fpack_s.loadmysound		= CExport::LoadMySound;
	fpack_s.playmysound		= CExport::PlayMySound;
	fpack_s.killmysound		= CExport::KillMySound;
	fpack_s.msg2system		= CExport::Message2System;
	fpack_s.getenemyname	= CExport::GetEnemyName;
	fpack_s.getdispcenterx	= CExport::GetDisplayCenterX;
	fpack_s.getdispcentery	= CExport::GetDisplayCenterY;
	fpack_s.getteamnum		= CExport::GetTeamNum;
	fpack_s.gettaisenkeisiki= CExport::GetTaisenKeisiki;
//	fpack_s.strikerok		= CExport::StrikerOK;
	fpack_s.getenemyface	= CExport::GetEnemyFace;
	fpack_s.getteamnum2		= CExport::GetTeamNum2;
	fpack_s.tid2etid		= CExport::TeamID2EnemyTeamID;
	fpack_s.is_net			= CExport::IsNetwork;
	fpack_s.pushtag			= CExport::PushTag;
	fpack_s.poptag			= CExport::PopTag;
	fpack_s.logerror		= CExport::LogError;
	fpack_s.logwarning		= CExport::LogWarning;
	fpack_s.logdebug		= CExport::LogDebug;
	fpack_s.loginfo			= CExport::LogInfo;
	fpack_s.bgm_pause		= CExport::BGMPause;
	fpack_s.bgm_resume		= CExport::BGMResume;

	//object
	fpack_o.objcreate		= CExport::CreateObject;
	fpack_o.objdelete		= CExport::DeleteObject;
	fpack_o.objcatch		= CExport::CatchObject;
	fpack_o.getinfo			= CExport::GetObjectInfo;
	fpack_o.adddamage		= CExport::AddDamage;
	fpack_o.actidchanged	= CExport::ActIDChanged;
	fpack_o.getactivechar	= CExport::GetActiveCharacter;
	fpack_o.suicide			= CExport::SuicideGObject;
	fpack_o.getmaai_h		= CExport::GetMaaiH;
	fpack_o.getmaai_v		= CExport::GetMaaiV;
	fpack_o.getcharid		= CExport::GetCharacterID;
	fpack_o.getcomlevel		= CExport::GetComLevel;
	fpack_o.setcomreach		= CExport::SetComReach;
	fpack_o.setcomact		= CExport::SetComAct;
	fpack_o.objcreate_fx	= CExport::CreateObjectFx;
	fpack_o.obj_is_local	= CExport::ObjIsLocal;

	//object, default actions
	fpack_o.dact_damages1	= CGObject::dact_damages1;
	fpack_o.dact_damages2	= CGObject::dact_damages2;
	fpack_o.dact_damages3	= CGObject::dact_damages3;
	fpack_o.dact_damagec1	= CGObject::dact_damagec1;
	fpack_o.dact_damagec2	= CGObject::dact_damagec2;
	fpack_o.dact_damagec3	= CGObject::dact_damagec3;
	fpack_o.dact_damagej1	= CGObject::dact_damagej1;
	fpack_o.dact_damagej2	= CGObject::dact_damagej2;
	fpack_o.dact_damagej3	= CGObject::dact_damagej3;
	fpack_o.dact_damages1a	= CGObject::dact_damages1a;
	fpack_o.dact_damages2a	= CGObject::dact_damages2a;
	fpack_o.dact_damages3a	= CGObject::dact_damages3a;
	fpack_o.dact_damagec1a	= CGObject::dact_damagec1a;
	fpack_o.dact_damagec2a	= CGObject::dact_damagec2a;
	fpack_o.dact_damagec3a	= CGObject::dact_damagec3a;
	fpack_o.dact_guards1	= CGObject::dact_guards1;
	fpack_o.dact_guards2	= CGObject::dact_guards2;
	fpack_o.dact_guards3	= CGObject::dact_guards3;
	fpack_o.dact_guardc1	= CGObject::dact_guardc1;
	fpack_o.dact_guardc2	= CGObject::dact_guardc2;
	fpack_o.dact_guardc3	= CGObject::dact_guardc3;
	fpack_o.dact_guardj1	= CGObject::dact_guardj1;
	fpack_o.dact_guardj2	= CGObject::dact_guardj2;
	fpack_o.dact_guardj3	= CGObject::dact_guardj3;
	fpack_o.dact_down		= CGObject::dact_down;
	fpack_o.dact_down2		= CGObject::dact_down2;
	fpack_o.dact_okiagari	= CGObject::dact_okiagari;
	fpack_o.dact_futtobi	= CGObject::dact_futtobi;
	fpack_o.dact_futtobi2	= CGObject::dact_futtobi2;
	fpack_o.dact_tatakituke1a	= CGObject::dact_tatakituke1a;
	fpack_o.dact_tatakituke1b	= CGObject::dact_tatakituke1b;
	fpack_o.dact_tatakituke2a	= CGObject::dact_tatakituke2a;
	fpack_o.dact_tatakituke2b	= CGObject::dact_tatakituke2b;

	fpack_o.setcomrange		= CExport::SetComRange;
	fpack_o.getcharname		= CExport::GetCharacterName;
	fpack_o.getkeyinput		= CExport::GetKeyInput;

	//draw
	fpack_d.getd3d			= CExport::GetD3D;
	fpack_d.getd3dd			= CExport::GetD3DD;
	fpack_d.loadcelldat		= CExport::LoadCellDat;
	fpack_d.loadbmp			= CExport::LoadBmp;
	fpack_d.unloadbmp		= CExport::UnloadBmp;
	fpack_d.celldraw		= CExport::CellDraw;
	fpack_d.ckblt			= CExport::Blt;
	fpack_d.blt3d			= CExport::Blt3D;
	fpack_d.settrans		= CExport::SetTransform;
	fpack_d.setblend		= CExport::SetBlendMode;
	fpack_d.setparentmat	= CExport::SetParentMatrix;
	fpack_d.create_celldat	= CExport::CreateCellDat;
	fpack_d.destroy_celldat = CExport::DestroyCellDat;
	fpack_d.loadimage		= CExport::LoadImage;
	fpack_d.create_celldat2	= CExport::CreateCellDat2;
}

//*********************************************************************************
// 現在の戦闘タスク取得
//*********************************************************************************
CBattleTaskBase* CExport::GetCurrentBattleTask()
{
	CExclusiveTaskBase* pTopTask = g_system.taskManager.GetTopExclusiveTask();
	return( dynamic_cast<CBattleTaskBase*>(pTopTask) );
}


//*********************************************************************************
// システム関連
//*********************************************************************************

DWORD CExport::GetKey(DWORD tid,DWORD index)
{
	FUNC_IN;
	DWORD ret = g_input.GetKeyEx(tid,index);
	FUNC_OUT;
	return(ret);
}
int   CExport::SeekKey(DWORD tid,int index,int numseek,DWORD key)
{
	FUNC_IN;
	int ret =g_input.SeekKeyEx(tid,index,numseek,key); 
	FUNC_OUT;
	return(ret);
}
void  CExport::SetKatiSerif(DWORD tid,char* serif)
{
	FUNC_IN;
	GetCurrentBattleTask()->SetKatiSerif(tid,serif);
	FUNC_OUT;
}
void  CExport::AddEffect(DWORD eid,int prm1,int prm2,DWORD prm3)
{	
	FUNC_IN;
	GetCurrentBattleTask()->AddEffect(eid,prm1,prm2,prm3);
	FUNC_OUT;
}
void  CExport::PlaySystemSound(DWORD sid)
{
	FUNC_IN;
	g_system.PlaySystemSound(sid);
	FUNC_OUT;
}
typedef struct _TWIN_SOUNDS
{
	LPDIRECTSOUNDBUFFER pSound[2];
	int num;
} TWIN_SOUNDS;
LPVOID CExport::LoadMySound(char* filename)
{
	FUNC_IN;
	LPVOID ret = NULL;
	TWIN_SOUNDS* pSounds = (TWIN_SOUNDS*)malloc(sizeof(TWIN_SOUNDS));

	if (pSounds)
	{
		pSounds->pSound[0] = g_sound.CreateDSB(filename) ;
		// ２個目はコピペ
		pSounds->pSound[1] = NULL;
		g_sound.lpds->DuplicateSoundBuffer(pSounds->pSound[0], &pSounds->pSound[1]) ;
		pSounds->num = 0;

		ret = (LPVOID)pSounds;
	}
	FUNC_OUT;
	return ret;
}
void   CExport::PlayMySound(LPVOID pdsb)
{
	FUNC_IN;
	if(pdsb!=NULL)
	{
		TWIN_SOUNDS* pSounds = (TWIN_SOUNDS*)pdsb;

		if (pSounds->pSound[ pSounds->num ])
		{
			pSounds->pSound[ pSounds->num ]->Stop();
			pSounds->pSound[ pSounds->num ]->SetCurrentPosition(0);
			pSounds->pSound[ pSounds->num ]->Play(0,0,0);
		}

		if (pSounds->pSound[ !pSounds->num ])
			pSounds->num = !pSounds->num;
	}
	FUNC_OUT;
}
void   CExport::KillMySound(LPVOID pdsb)
{
	FUNC_IN;
	if(pdsb!=NULL)
	{
		TWIN_SOUNDS* pSounds = (TWIN_SOUNDS*)pdsb;

		if (pSounds->pSound[0])
			pSounds->pSound[0]->Release();
		if (pSounds->pSound[1])
			pSounds->pSound[1]->Release();

		free(pdsb);
	}
	FUNC_OUT;
}
DWORD CExport::Message2System(DWORD oid,DWORD msg,DWORD prm)
{
	FUNC_IN;
	if(msg==('aki') && prm==('LCut'))
	{
		g_config.EnableLimiterCut();
		FUNC_OUT;
		return TRUE;
	}
	DWORD ret = GetCurrentBattleTask()->MessageFromObject(oid,msg,prm);
	FUNC_OUT;
	return ret;
}
char* CExport::GetEnemyName(DWORD tid)
{
	FUNC_IN;
	DWORD eid=TEAM_PLAYER1;
	if(tid==TEAM_PLAYER1)eid=TEAM_PLAYER2;
	char* ret = g_charlist.GetCharacterName(g_battleinfo.GetCharacter(eid,GetCurrentBattleTask()->GetActiveCharacterID(eid)));
	FUNC_OUT;
	return ret;
}
char* CExport::GetCharacterName(DWORD oid)
{
	FUNC_IN;
	CGObject* pobj = GetCurrentBattleTask()->GetGObject(oid);
	char* ret = NULL;
	if (pobj) ret = g_charlist.GetCharacterName(g_battleinfo.GetCharacter( (pobj->dll_id - 1) / MAXNUM_TEAM, (pobj->dll_id - 1) % MAXNUM_TEAM ));
	FUNC_OUT;
	return ret;
}
DWORD CExport::GetKeyInput(DWORD oid)
{
	FUNC_IN;
	CGObject* pobj = GetCurrentBattleTask()->GetGObject(oid);
	DWORD ret = 0;
	if (pobj) ret = g_battleinfo.GetKeyAssign( (pobj->dll_id - 1) / MAXNUM_TEAM, (pobj->dll_id - 1) % MAXNUM_TEAM );
	FUNC_OUT;
	return ret;
}
double CExport::GetDisplayCenterX()
{
	FUNC_IN;
	double ret = GetCurrentBattleTask()->GetDisplayCenterX();
	FUNC_OUT;
	return ret;
}
double CExport::GetDisplayCenterY()
{
	FUNC_IN;
	double ret =GetCurrentBattleTask()->GetDisplayCenterY();
	FUNC_OUT;
	return ret;
}
DWORD CExport::GetTeamNum()
{
	FUNC_IN;
	DWORD ret = max(GetTeamNum2(TEAM_PLAYER1), GetTeamNum2(TEAM_PLAYER2));
	FUNC_OUT;
	return ret;
}
DWORD CExport::GetTaisenKeisiki()
{
	FUNC_IN;
	DWORD ret = g_battleinfo.GetBattleType();
	FUNC_OUT;
	return ret;
}
/*
DWORD CExport::StrikerOK(DWORD tid,BOOL next)
{
	BOOL striker_front=next;

	if(bf_state != BFSTATE_FIGHTING)return(STRIKEREADY_NOPOINT);
	if(strikercount[pdat->data.tid]<=0)return(STRIKEREADY_NOPOINT);

	return(0);
}
*/
BYTE  CExport::GetEnemyFace(DWORD tid)
{
	FUNC_IN;
	DWORD eid=TEAM_PLAYER1;
	if(tid==TEAM_PLAYER1)eid=TEAM_PLAYER2;
	BYTE ret = (BYTE)( OPT2ALT(g_battleinfo.GetCharacterOption(eid,GetCurrentBattleTask()->GetActiveCharacterID(eid))) );
	FUNC_OUT;
	return ret;
}
DWORD CExport::GetTeamNum2(DWORD tid)
{
	FUNC_IN;
	DWORD ret = g_battleinfo.GetNumTeam(tid);
	FUNC_OUT;
	return ret;
}
DWORD CExport::TeamID2EnemyTeamID(DWORD tid)
{
	FUNC_IN;
	DWORD ret = (tid==TEAM_PLAYER1) ? TEAM_PLAYER2 : TEAM_PLAYER1;
	FUNC_OUT;
	return ret;
}
BOOL CExport::IsNetwork()
{
	FUNC_IN;
	BOOL ret = GetCurrentBattleTask()->IsNetwork();
	FUNC_OUT;
	return ret;
}
void CExport::PushTag(const char* tag)
{
	FUNC_IN;
	g_system.PushDLLTag( tag );
	FUNC_OUT;
}
void CExport::PopTag()
{
	FUNC_IN;
	g_system.PopDLLTag();
	FUNC_OUT;
}
void CExport::LogError(const char* str)
{
	FUNC_IN;
	g_system.Log( str,SYSLOG_ERROR );
	FUNC_OUT;
}
void CExport::LogWarning(const char* str)
{
	FUNC_IN;
	g_system.Log( str,SYSLOG_WARNING );
	FUNC_OUT;
}
void CExport::LogDebug(const char* str)
{
	FUNC_IN;
	g_system.Log( str,SYSLOG_DEBUG );
	FUNC_OUT;
}
void CExport::LogInfo(const char* str)
{
	FUNC_IN;
	g_system.Log( str,SYSLOG_INFO );
	FUNC_OUT;
}

//*********************************************************************************
//　オブジェクト関連
//*********************************************************************************

DWORD CExport::CreateObject()
{
	FUNC_IN;
	DWORD ret = GetCurrentBattleTask()->CreateGObject();
	FUNC_OUT;
	return ret;
}
void  CExport::DeleteObject(DWORD oid)
{
	FUNC_IN;
	GetCurrentBattleTask()->DeleteGObject(oid);
	FUNC_OUT;
}
BOOL  CExport::CatchObject(DWORD eoid,LPVOID cy)
{
	FUNC_IN;
	BOOL ret = GetCurrentBattleTask()->CatchObject(eoid,cy);
	FUNC_OUT;
	return ret;
}
LPVOID  CExport::GetObjectInfo(DWORD oid)
{
	FUNC_IN;
	LPVOID ret = GetCurrentBattleTask()->GetGObjectInfo(oid);
	FUNC_OUT;
	return ret;
}
void  CExport::AddDamage(DWORD oid,DWORD eoid,int x,int y)
{
	FUNC_IN;
	GetCurrentBattleTask()->AddDamage(oid,eoid,x,y);
	FUNC_OUT;
}
void  CExport::ActIDChanged(DWORD oid)
{
	FUNC_IN;
	CGObject *pg=GetCurrentBattleTask()->GetGObject(oid);
	if(pg!=NULL){
		pg->ActionIDChanged(TRUE,TRUE);
	}
	FUNC_OUT;
}
void* CExport::GetActiveCharacter(DWORD tid)
{
	FUNC_IN;
	void* ret = (GOBJECT*)(GetCurrentBattleTask()->GetActiveCharacter(tid));
	FUNC_OUT;
	return ret;
}
void  CExport::SuicideGObject(DWORD oid)
{
	FUNC_IN;
	GetCurrentBattleTask()->SuicideGObject(oid);
	FUNC_OUT;
}
DWORD CExport::GetMaaiH(DWORD oid,DWORD eoid)
{
	FUNC_IN;
	DWORD ret = GetCurrentBattleTask()->GetMaai(oid,eoid,FALSE);
	FUNC_OUT;
	return ret;
}
DWORD CExport::GetMaaiV(DWORD oid,DWORD eoid)
{
	FUNC_IN;
	DWORD ret = GetCurrentBattleTask()->GetMaai(oid,eoid,TRUE);
	FUNC_OUT;
	return ret;
}
DWORD CExport::GetCharacterID(DWORD tid,DWORD index)
{
	FUNC_IN;
	DWORD ret = GetCurrentBattleTask()->GetCharacterOID(tid,index);
	FUNC_OUT;
	return ret;
}
DWORD CExport::GetComLevel(DWORD oid)
{
	FUNC_IN;
	CGObject *pg=GetCurrentBattleTask()->GetGObject(oid);
	DWORD ret;
	if(pg!=NULL){
		ret= pg->com_level;
	}
	else ret = 0;
	FUNC_OUT;
	return ret;
}
void CExport::SetComReach(DWORD oid,DWORD idx,int length)
{
	FUNC_IN;
	if(idx>2){
		FUNC_OUT;
		return;
	}

	CGObject *pg=GetCurrentBattleTask()->GetGObject(oid);
	if(pg!=NULL){
		pg->comreach[idx] = length;
	}
	FUNC_OUT;
}
void CExport::SetComAct(DWORD oid,DWORD aid,int delay)
{
	FUNC_IN;
	CGObject *pg=GetCurrentBattleTask()->GetGObject(oid);
	if(pg!=NULL){
		pg->comhint_aid = aid;
		pg->comhint_delay = delay;
	}
	FUNC_OUT;
}

DWORD CExport::CreateObjectFx()
{
	FUNC_IN;
	DWORD ret = GetCurrentBattleTask()->CreateGObjectFx();
	FUNC_OUT;
	return ret;
}
	
BOOL CExport::ObjIsLocal(DWORD oid)
{
	FUNC_IN;
	CBattleTaskBase *btask = GetCurrentBattleTask();
	CGObject *obj = btask->GetGObject(oid);
	BOOL ret;
	if(obj)
	{
		ret = btask->IsLocal( obj->dll_id );
	}
	else ret = FALSE;
	FUNC_OUT;
	return ret;
}

void CExport::SetComRange(DWORD oid,DWORD idx)
{
	FUNC_IN;
	if(idx>2){
		FUNC_OUT;
		return;
	}

	CGObject *pg=GetCurrentBattleTask()->GetGObject(oid);
	if(pg!=NULL){
		pg->comrange = idx;
	}
	FUNC_OUT;
}

//*********************************************************************************
//　描画関連の関数の定義
//*********************************************************************************

LPVOID CExport::GetD3D()
{
	FUNC_IN;
	LPVOID ret = g_draw.dd;
	FUNC_OUT;
	return ret;
}
LPVOID CExport::GetD3DD()
{
	FUNC_IN;
	LPVOID ret = g_draw.d3ddev;
	FUNC_OUT;
	return ret;
}
void   CExport::LoadCellDat(char* filename,LPVOID cdat,LPVOID rdat,LPVOID hdat)
{
	FUNC_IN;
	CGCDHandler::GCDLoad(filename,(GCD_CELL2*)cdat,(GCD_RECT*)rdat,(GCD_HANTEI*)hdat,NULL,700);

	//NowLoadingを進める
	CTNowLoading* task = dynamic_cast<CTNowLoading*>( g_system.FindTask('LOAD') );
	if(task)task->Proceed(NowLoading_GCD);

	FUNC_OUT;
}
LPVOID CExport::LoadBmp(char* filename,char* palname)
{
	FUNC_IN;
	LPVOID ret = g_draw.CreateSurfaceFrom256BMP(filename,palname);
	gbl.ods("CExport::LoadBmp : %s - %s , %s",
			ret ? "success" : "failed",
			filename ? filename : "NULL",
			palname ? palname : "NULL"
			);
	if(ret)
	{
		//NowLoadingを進める
		CTNowLoading* task = dynamic_cast<CTNowLoading*>( g_system.FindTask('LOAD') );
		if(task)task->Proceed(NowLoading_Image);
	}
	FUNC_OUT;
	return ret;
}
void   CExport::UnloadBmp(LPVOID psuf)
{
	FUNC_IN;
	g_draw.RelSurface((MYSURFACE*)psuf);
	FUNC_OUT;
}
void   CExport::CellDraw(LPVOID* ddsa,LPVOID cdat,LPVOID rdat,DWORD cn,int x,int y,float z,int rot,BOOL revx,BOOL revy,DWORD color,float magx,float magy)
{
	FUNC_IN;

	if (g_draw.devtypenow != D3DDEVTYPE_HAL)
	{
		CBattleTaskBase *battleTask = GetCurrentBattleTask();

		if (battleTask)
		{
			BOOL shadow = FALSE;

			if (y < 10 && y > -500 && abs(x) <= 640.0)
				shadow = TRUE;
			g_draw.CellDraw((MYSURFACE**)ddsa,cdat,rdat,cn,x - ((int)battleTask->GetDisplayCenterX() - g_DISPLAYWIDTH / 2),y + 440,z,rot,revx,revy,color,magx,magy,shadow);
			FUNC_OUT;
			return;
		}
	}

	g_draw.CellDraw((MYSURFACE**)ddsa,cdat,rdat,cn,x,y,z,rot,revx,revy,color,magx,magy,TRUE);

	FUNC_OUT;
}
void   CExport::Blt(void* dds,int x,int y,RECT r,double magx,double magy,BOOL revx,BOOL revy,float z,DWORD color)
{
	FUNC_IN;

	if (g_draw.devtypenow != D3DDEVTYPE_HAL)
	{
		CBattleTaskBase *battleTask = GetCurrentBattleTask();

		if (battleTask)
		{
			g_draw.CheckBlt2((MYSURFACE*)dds,x - ((int)battleTask->GetDisplayCenterX() - g_DISPLAYWIDTH / 2),y + 440,r,magx,magy,revx,revy,0,z,color);
			FUNC_OUT;
			return;
		}
	}
	
	g_draw.CheckBlt2((MYSURFACE*)dds,x,y,r,magx,magy,revx,revy,0,z,color);

	FUNC_OUT;
}
void   CExport::Blt3D(void* dds,RECT src,MYRECT3D dst,DWORD color)
{
	FUNC_IN;

	if (g_draw.devtypenow != D3DDEVTYPE_HAL)
	{
		CBattleTaskBase *battleTask = GetCurrentBattleTask();

		if (battleTask)
		{
			dst.left	-= ((float)battleTask->GetDisplayCenterX() - g_DISPLAYWIDTH / 2) / (g_DISPLAYHEIGHT / 2);
			dst.right	-= ((float)battleTask->GetDisplayCenterX() - g_DISPLAYWIDTH / 2) / (g_DISPLAYHEIGHT / 2);
			dst.top		+= 400 / (g_DISPLAYHEIGHT / 3);
			dst.bottom	+= 400 / (g_DISPLAYHEIGHT / 3);
		}
	}

	g_draw.MyBlt3D((MYSURFACE*)dds,src,dst,0,color);
	FUNC_OUT;
}
void   CExport::SetTransform(BOOL b)//座標変換行列を再設定
{
	FUNC_IN;
	GetCurrentBattleTask()->SetTransform(b);
	FUNC_OUT;
}
void   CExport::SetBlendMode(DWORD m)//α合成の方法を変更
{
	FUNC_IN;
	g_draw.SetAlphaMode(m);
	FUNC_OUT;
}
void   CExport::SetParentMatrix(LPVOID mat,BOOL root,LPVOID matprv)//「親」変換行列設定
{
	FUNC_IN;
	g_draw.SetParentMatrix2((D3DXMATRIX*)mat,root,(D3DXMATRIX*)matprv);
	FUNC_OUT;
}
DWORD  CExport::CreateCellDat(char* filename,LPVOID *cdat,LPVOID *rdat,LPVOID *hdat)
{
	FUNC_IN;
	DWORD ret = GCD_VERSION;

	*cdat = new GCD_CELL2[GCDMAX_CELLS];
	*rdat = new GCD_RECT[GCDMAX_RECTANGLES];
	*hdat = new GCD_HANTEI[GCDMAX_CELLS];
	if(0 != CGCDHandler::GCDLoad(filename,(GCD_CELL2*)(*cdat),(GCD_RECT*)(*rdat),(GCD_HANTEI*)(*hdat),NULL)){
		DELETE_ARRAY( *cdat );
		DELETE_ARRAY( *rdat );
		DELETE_ARRAY( *hdat );

		//0.70でリトライ
		*cdat = new GCD_CELL2_070[GCDMAX_CELLS];
		*rdat = new GCD_RECT_070[GCDMAX_RECTANGLES];
		*hdat = new GCD_HANTEI_070[GCDMAX_CELLS];
		if(0 != CGCDHandler::GCDLoad(filename,(GCD_CELL2*)(*cdat),(GCD_RECT*)(*rdat),(GCD_HANTEI*)(*hdat),NULL,700)){
			DELETE_ARRAY( *cdat );
			DELETE_ARRAY( *rdat );
			DELETE_ARRAY( *hdat );
			return 0;
		}
		ret= 700;
	}

	//NowLoadingを進める
	CTNowLoading* task = dynamic_cast<CTNowLoading*>( g_system.FindTask('LOAD') );
	if(task)task->Proceed(NowLoading_GCD);

	FUNC_OUT;
	return ret;
}
void  CExport::DestroyCellDat(LPVOID *cdat,LPVOID *rdat,LPVOID *hdat)
{
	FUNC_IN;
	DELETE_ARRAY( *cdat );
	DELETE_ARRAY( *rdat );
	DELETE_ARRAY( *hdat );
	FUNC_OUT;
}
LPVOID  CExport::LoadImage(char* filename, char* palname)
{
	LPVOID ret;

	FUNC_IN;
	ret = g_draw.CreateSurfaceFrom256Image(filename, palname);
	gbl.ods("CExport::LoadImage : %s - %s , %s",
			ret ? "success" : "failed",
			filename ? filename : "NULL",
			palname ? palname : "NULL"
			);
	if(ret)
	{
		//NowLoadingを進める
		CTNowLoading* task = dynamic_cast<CTNowLoading*>( g_system.FindTask('LOAD') );
		if(task)task->Proceed(NowLoading_Image);
	}
	FUNC_OUT;

	return ret;
}
DWORD  CExport::CreateCellDat2(char* filename,LPVOID *cdat,LPVOID *rdat,LPVOID *hdat)
{
	FUNC_IN;
	DWORD ret = GCD_VERSION;
	if (!filename) return 0;
	char* Buffer = (char*)malloc( strlen(filename) + (4 + 1) );

	if (!Buffer) return 0;

	*cdat = new GCD_CELL2[GCDMAX_CELLS];
	*rdat = new GCD_RECT[GCDMAX_RECTANGLES];
	*hdat = new GCD_HANTEI[GCDMAX_CELLS];
	sprintf(Buffer, "%s%s", filename, ".gcm");
	if(0 != CGCDHandler::GCDLoadCompressed(Buffer,(GCD_CELL2*)(*cdat),(GCD_RECT*)(*rdat),(GCD_HANTEI*)(*hdat),NULL) &&
		0 != CGCDHandler::GCDLoad(Buffer,(GCD_CELL2*)(*cdat),(GCD_RECT*)(*rdat),(GCD_HANTEI*)(*hdat),NULL)){
		sprintf(Buffer, "%s%s", filename, ".gcd");

		if(0 != CGCDHandler::GCDLoadCompressed(Buffer,(GCD_CELL2*)(*cdat),(GCD_RECT*)(*rdat),(GCD_HANTEI*)(*hdat),NULL) &&
			0 != CGCDHandler::GCDLoad(Buffer,(GCD_CELL2*)(*cdat),(GCD_RECT*)(*rdat),(GCD_HANTEI*)(*hdat),NULL)){
			DELETE_ARRAY( *cdat );
			DELETE_ARRAY( *rdat );
			DELETE_ARRAY( *hdat );

			//0.70でリトライ
			*cdat = new GCD_CELL2_070[GCDMAX_CELLS];
			*rdat = new GCD_RECT_070[GCDMAX_RECTANGLES];
			*hdat = new GCD_HANTEI_070[GCDMAX_CELLS];
			if(0 != CGCDHandler::GCDLoad(Buffer,(GCD_CELL2*)(*cdat),(GCD_RECT*)(*rdat),(GCD_HANTEI*)(*hdat),NULL,700)){
				DELETE_ARRAY( *cdat );
				DELETE_ARRAY( *rdat );
				DELETE_ARRAY( *hdat );
				FREEMALM(Buffer);
				return 0;
			}
			ret= 700;
		}
	}

	//NowLoadingを進める
	CTNowLoading* task = dynamic_cast<CTNowLoading*>( g_system.FindTask('LOAD') );
	if(task)task->Proceed(NowLoading_GCD);

	FREEMALM(Buffer);
	FUNC_OUT;
	return ret;
}
void CExport::BGMPause(void)
{
g_sound.BGMPause();
}
void CExport::BGMResume(void)
{
g_sound.BGMResume();
}



