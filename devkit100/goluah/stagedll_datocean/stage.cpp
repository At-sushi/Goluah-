
#include "stage.h"
#include "soul.h"
#include "soul_down.h"
#include "floor.h"
#include "sky.h"

DI_FUNCTIONS_S	*funcs=NULL;
DI_FUNCTIONS_O	*funco=NULL;
DI_FUNCTIONS_D	*funcd=NULL;
LPDIRECT3DDEVICE8 d3ddev=NULL;
char *g_stgdir=NULL;

//メッセージ処理関数・グローバル
DWORD gSTGMessage(DWORD msg,LPVOID pdat,DWORD prm)
{
	if(pdat==NULL)return(NULL);
	GOBJECT* pdat2 = (GOBJECT*)pdat;

	CStage *pstg = (CStage*)pdat2->pobjdat_u;

	if(pstg!=NULL){
		if(msg==GOBJMSG_DELETE)
			delete(pstg);
		return(pstg->Message(msg,pdat,prm));
	}
	return(0);
}

//****************************************************************
//ステージの初期化
//****************************************************************
CStage::CStage(SDI_STAGEINFO2 *info)
{
	funcs = info->funcs;
	funco = info->funco;
	funcd = info->funcd;
	//変数
	d3ddev = (LPDIRECT3DDEVICE8)funcd->getd3dd();
	g_stgdir = info->dir;

	//aki3d
	aki3d.Initialize( d3ddev, g_stgdir );

	//オブジェクト生成
	oid = funco->objcreate();
	pdat = (GOBJECT*)funco->getinfo(oid);
	if(pdat==NULL){
		return;
	}
	pdat->msghandler = gSTGMessage;
	pdat->tid = TEAM_STAGE;
	pdat->pobjdat_u = this;


	objlist.push_back( new CSky );
	objlist.push_back( new CFloor );
	objlist.push_back( new CSoul );
	objlist.push_back( new CSoulDown );


	for(UINT i=0;i<objlist.size();i++)
	{
		objlist[i]->Init();
	}
}


//****************************************************************
//ステージ後始末
//****************************************************************
CStage::~CStage()
{
	for(UINT i=0;i<objlist.size();i++)
	{
		objlist[i]->Release();
		delete objlist[i];
	}
}

//****************************************************************
//メッセージ処理関数
//****************************************************************

DWORD CStage::Message(DWORD msg,LPVOID pdatg,DWORD prm)
{
	if(pdat==NULL)return(0);

	switch(msg){
	case GOBJMSG_DRAWBACK:
		Draw();//描画～
		return(TRUE);
	case GOBJMSG_DRAW:
		return(TRUE);
	case GOBJMSG_CNGROUND:
		return(TRUE);
	case GOBJMSG_ACTION:
		Action();
		return(TRUE);
	default:
		return(0);
	}
}

//*******************************************************
//　背景描画
//*******************************************************
void CStage::Draw()
{
	if(d3ddev==NULL)return;

	for(UINT i=0;i<objlist.size();i++)
	{
		//try{
			objlist[i]->Draw();
		/*}catch(...)
		{
			delete objlist[i];
			objlist.erase( objlist.begin()+i );
			i--;
		}*/
	}
}

//*******************************************************
//　動き
//*******************************************************
void CStage::Action()
{
	for(UINT i=0;i<objlist.size();i++)
	{
		objlist[i]->Update();
	}
}



