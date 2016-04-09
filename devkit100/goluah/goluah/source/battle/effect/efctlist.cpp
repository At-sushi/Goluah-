
/*===============================================================

	システムエフェクトオブジェクト管理クラス

=================================================================*/
#include "stdafx.h"

#include "define_const.h"
#include "define_macro.h"
#include "global.h"
#include "battleTaskBase.h"
#include "efctlist.h"

#include "efct_sys.h"		//FIGHT,ROUNDX等
#include "efct_hit.h"		//ヒットマーク等
#include "efct_other.h"		//その他


//インスタンスを2つ以上作らないこと
CEffectList* CEffectList::pinst = NULL;

LPDIRECT3DTEXTURE9 aki3dtexture_circle = NULL;

/*---------------------------------------------------------------
	生成
-----------------------------------------------------------------*/
CEffectList::CEffectList()
{
	if(pinst)throw;
	pinst = this;

	aki3dtexture_circle = aki3d.LoadTexture(_T("circle.png"));
}


/*---------------------------------------------------------------
	破棄
-----------------------------------------------------------------*/
void CEffectList::Destroy()
{
	UnloadTextures();
	pinst = NULL;

	aki3d.UnloadTexture(aki3dtexture_circle);
}


/*---------------------------------------------------------------
	初期化
-----------------------------------------------------------------*/
void CEffectList::Initialize()
{
	battleTask = dynamic_cast<CBattleTaskBase*>( g_system.GetCurrentMainTask() );
	if(!battleTask)throw;//catchしてないけどね

	LoadTextures();
}


/*---------------------------------------------------------------
	テクスチャ読み込み・消去
-----------------------------------------------------------------*/
void CEffectList::LoadTextures()
{
	LPDIRECT3DTEXTURE9 p;
	TCHAR *filename = new TCHAR [128];

	for(int i=0;i<MAXNUM_SYSFXTEXTURES;i++){
		_stprintf(filename,_T(".\\system\\texture\\tex%d.png"),i);
		if(D3D_OK!=D3DXCreateTextureFromFileA(g_draw.d3ddev,filename,&p))p=NULL;
		textures.push_back(p);
		if(!p){
			gbl.ods(_T("CEffectList::LoadTextures - %s 読み込み失敗"),filename);
		}
	}

	delete [] filename;
}

void CEffectList::UnloadTextures()
{
	TextureList::iterator i = textures.begin();
	TextureList::iterator ie= textures.end();
	for(;i!=ie;i++){
		if(*i){
			(*i)->Release();
		}
	}
	textures.clear();
}

/*---------------------------------------------------------------
	エフェクト追加
-----------------------------------------------------------------*/
void CEffectList::AddEffect(DWORD type,int prm1,int prm2,int prm3)
{
	if(!g_config.UsePointSprite()){
		if(type==EFCTID_TUBUTUBU || type==EFCTID_TUBUTUBU2)return;
	}

	//オブジェクト生成
	DWORD objid = battleTask->CreateGObjectFx();
	if(objid==0)return;
	GOBJECT *pdat = (GOBJECT*)battleTask->GetGObjectInfo(objid);

	//基本
	pdat->tid = TEAM_SYSTEM;
	pdat->uid = type;
	pdat->msghandler = EfctObjectMessage;

	pdat->counter = 0;
	pdat->color = 0xFFFFFFFF;

	pdat->x = prm1;
	pdat->y = prm2;
	pdat->muki = prm3;
	if (pdat->uid == EFCTID_GNDCIR)
		pdat->z = ZZAHYO_EFFECT1;
	else
		pdat->z = ZZAHYO_EFFECT2;

	pdat->objtype |= GOBJFLG_DONOTSTOP;
	pdat->phdat = g_system.GetSystemGraphicHantei();
	pdat->pcdat = g_system.GetSystemGraphicCell();
	pdat->prdat = g_system.GetSystemGraphicRect();
	pdat->pmsarr = g_system.GetSystemGraphicSurface();

	CSystemFxBase *fx = NULL;

	switch(pdat->uid){
	case EFCTID_MARK1:		fx=new CFxHitmark1;		break;
	case EFCTID_MARK2:		fx=new CFxHitmark2;		break;
	case EFCTID_MARK3:		fx=new CFxHitmark3;		break;
	case EFCTID_MARK4:		fx=new CFxHitmark4;		break;
	case EFCTID_MARKG2:		fx=new CFxGuardmark1;	break;
	case EFCTID_MARKG:		fx=new CFxGuardmark2;	break;
	case EFCTID_HIKARI:		fx=new CFxCircle;		break;
	case EFCTID_MARKC:		fx=new CFxCatchmark;	break;
	case EFCTID_GNDCIR:		fx=new CFxGndCircle;	break;
	case EFCTID_CYOHILIGHT:	fx=new CFxCyohiLight;	break;
	case EFCTID_ROUND1:		fx=new CFxRound1;		break;
	case EFCTID_ROUND2:		fx=new CFxRound2;		break;
	case EFCTID_ROUND3:		fx=new CFxRound3;		break;
	case EFCTID_ROUND4:		fx=new CFxRound4;		break;
	case EFCTID_ROUND5:		fx=new CFxRound5;		break;
	case EFCTID_ROUND6:		fx=new CFxRound6;		break;
	case EFCTID_KO:			fx=new CFxKO;			break;
	case EFCTID_FIGHT:		fx=new CFxFight;		break;
	case EFCTID_TUBUTUBU:	fx=new CFxPSprite;		break;
	case EFCTID_TUBUTUBU2:	fx=new CFxPSprite2;		break;
	case EFCTID_BURN:		fx=new CFxBurn;			break;
	case EFCTID_BURN_B:		fx=new CFxBurnB;		break;
	case EFCTID_BURN_G:		fx=new CFxBurnG;		break;
	case EFCTID_COMEON:		fx=new CFxComeon;		break;
	case EFCTID_GCANCEL:	fx=new CFxGuardCancel;	break;
	case EFCTID_SUPERARMER:	fx=new CFxSuperArmer;	break;
	case EFCTID_HATTEN:		fx=new CFxHatten;		break;
	case EFCTID_TIMEOVER:	fx=new CFxTimeOver;		break;
	case EFCTID_DOUBLEKO:
	default:
		gbl.ods(_T("CEffectList::AddEffect / エフェクト未実装"));
		return;
	}

	if(!fx){//なに？
		battleTask->DeleteGObject(pdat->id);
		return;
	}
	fx->pefctlist = this;
	fx->battleTask = battleTask;
	fx->pdat = pdat;
	pdat->pobjdat_u = fx;
	if(!fx->Create(prm1,prm2,prm3)){
		battleTask->DeleteGObject(pdat->id);
	}
}


/*---------------------------------------------------------------
	全エフェクト共通オブジェクトメッセージハンドラ
-----------------------------------------------------------------*/
DWORD CEffectList::EfctObjectMessage(DWORD msg,LPVOID dat,DWORD prm1)
{
	GOBJECT* gobj = (GOBJECT*)dat;
	CSystemFxBase *fx = static_cast<CSystemFxBase*>( gobj->pobjdat_u );
	if(!fx)return 0;

	switch(msg){
	case GOBJMSG_ACTION:	fx->Update();break;
	case GOBJMSG_DRAW:		return fx->Draw();
	case GOBJMSG_DRAWFRONT:	return fx->DrawF();
	case GOBJMSG_DELETE:
				delete(fx);
				gobj->pobjdat_u =NULL;
				break;
	default:return(0);
	}
	return(YEAH);
}


/*---------------------------------------------------------------
	エフェクトUpdate動作
	destraoyed モウコネエヨ 
-----------------------------------------------------------------*
void CEffectList::EfctAction(GOBJECT *pdat)
{
	double tmp;

	switch(pdat->uid){
	case EFCTID_MOUKONEYO:
		if(pdat->counter < 1024){
			pdat->cnow=CELL_MOUKONEYO1+pdat->counter%3;
			pdat->x += 6;
		}
		else battleTask->SuicideGObject(pdat->id);
		break;
	case EFCTID_HATTEN:
		if(pdat->counter < 1024){
			pdat->cnow=CELL_HATTEN;
			pdat->x -= 4;
			tmp = ((timeGetTime()/20)*3)/360.0;
			tmp = sin(tmp*2*3.14);
			tmp *= 50;
			pdat->y = (int)(-360+tmp);
		}
		else battleTask->SuicideGObject(pdat->id);
		break;
	}
}*/

CSystemFxBase::CSystemFxBase()
{
	pdat=NULL;
	pefctlist=NULL;
}


CSystemFxBase::~CSystemFxBase()
{
	Destroy();
}

/*---------------------------------------------------------------
	システムエフェクト・別エフェクトの起動
-----------------------------------------------------------------*/
void CSystemFxBase::AddEffect(DWORD type,int prm1,int prm2,int prm3)
{
	pefctlist->AddEffect(type,prm1,prm2,prm3);
}

/*---------------------------------------------------------------
	システムエフェクト・終了
-----------------------------------------------------------------*/
void CSystemFxBase::End()
{
	if(!pefctlist)return;
	pefctlist->battleTask->SuicideGObject(pdat->id);
}