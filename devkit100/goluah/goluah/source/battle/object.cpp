/*============================================================================

	GOBJECT 内部処理クラス
	メッセージ送信と、メッセージのデフォルト処理

==============================================================================*/
#include "stdafx.h"

#include "gobject.h"
#include "global.h"
#include "object.h"


//===========================================================

CGObject::CGObject(DWORD object_id)
{
	battleTask = dynamic_cast<CBattleTaskBase*>( g_system.GetCurrentMainTask() );
	if(!battleTask)throw;

	ZeroMemory(&data ,sizeof(GOBJECT));
	ActionIDChanged(TRUE,TRUE);

	//パラメータを全てリセット
	comcounter=0;
	comguard = 0;
	ZeroMemory(&cy,sizeof(CATCHYOU));
	hitcount=0;//連続技ヒット数
	sexydamage=0;//連続技蓄積ダメージ
	sexydamage_anim=0;//連続技蓄積ダメージ
	sexydamage_haba=0;//連続技蓄積ダメージ
	siver=0;

	com_level = battleTask->GetCurrentComLevel();
	dll_id = battleTask->GetCurrentDLLID();

	comhint_aid   = 0;
	comhint_delay = -1;

	comreach[0] = 130;
	comreach[1] = 260;
	comreach[2] = 390;

	nage_muteki_cnt = 0;
	data.id = object_id;
	object_id_original = object_id;

	actloop_count = 0;
	comhint_counterprev = 0;
	comhint_capacity = 20;
	comrange = COMIDX_NEAR;

	userhit_attacker = 0;
	userhit_id = 0;
	ZeroMemory(&cy ,sizeof(cy));
}

CGObject::~CGObject()
{
	MessageDirect(GOBJMSG_DELETE,0);
	if((data.id&0x7FFFFFFF) != (object_id_original&0x7FFFFFFF)){
		g_system.LogErr( "object id changed %08X > %08X , dll_id = %d",
			object_id_original , data.id , dll_id );
	}
}

DWORD CGObject::MessageDirect(DWORD type,DWORD prm)
{
	try
	{
		CExport::last_funcname = "none";

		if(data.msghandler!=NULL)
		{
			battleTask->SetCurrentDLLID(dll_id);
			DWORD ret = (*data.msghandler)(type,&data,prm);
			battleTask->SetCurrentDLLIDSystem();
			g_system.ClearDLLTag();
			return ret;
		}
		else return 0;
	}
	catch(...)
	{
		battleTask->Notify_Exception( this,type,prm );
		throw;
	}
	return 0;
}


DWORD CGObject::Message(DWORD type,DWORD prm)
{
	int siverdx=0;
	CGObject *pobj=NULL;
	DWORD ret =0;

	//前処理
	switch(type){
	case GOBJMSG_COMMAND:
		if(data.objtype & GOBJFLG_TAIKI)//待機中はコマンド処理をしない
			return(0);
		break;
	case GOBJMSG_ACTION:
		ActionIDChanged(TRUE);
		if(data.aid==ACTID_OKIAGARI)nage_muteki_cnt = 25;
		else if(data.aid&ACTID_GUARD)nage_muteki_cnt = 25;
		else if(nage_muteki_cnt)nage_muteki_cnt--;
		break;
	case GOBJMSG_DRAW:
		if(siver>0){
			if(siver%2==0){
				data.x-=5;
				siverdx = 5;
			}
			else {
				data.x+=5;
				siverdx = -5;
			}
			siver--;
		}
		break;
	}

	//コンピュータ制御
	if(type==GOBJMSG_COMMAND){
		if(data.objtype&GOBJFLG_COMPUTER){
			ActionIDChanged(TRUE);
			ComCommand();
			ActionIDChanged(FALSE);
			return(0);
		}
	}
	if(type==GOBJMSG_ACTION && data.aid == ACTID_NAGERARE)//投げられ中である
	{
		pobj = (CGObject*)battleTask->GetGObject(cy.oid);
		if(pobj==NULL)//異常
		{
			g_system.Log("CGObject::Message 投げられ中に投げ側オブジェクト取得失敗",SYSLOG_WARNING);
			data.aid=ACTID_FUTTOBI;
			ActionIDChanged(TRUE,TRUE);
			return(0);
		}
		ret=pobj->Message(cy.actmsg,data.id);
		if(ret!=0){//投げから開放される
			data.aid=ret;
			if(data.hp<=0){//死亡
				if( g_battleinfo.GetBattleType() ==TAISENKEISIKI_GOCYAMAZE)
					data.aid=ACTID_FINALDOWN;
				else
					data.aid=ACTID_KAITENFINISH;
			}
			ActionIDChanged(TRUE,TRUE);
			Message(GOBJMSG_ACTION);
			return(0);
		}
		data.counter++;
		return(0);
	}

	if(type==GOBJMSG_TOUCHB)
	{
		comcounter = rand()%3+1;
	}
	ret = MessageDirect(type,prm);

	//デフォルト処理
	if(ret==0){
		switch(type){//メッセージによってデフォルトの動作が異なるッ
		case GOBJMSG_DRAW://セル描画
			Defmsg_Draw();
			break;
		case GOBJMSG_TOUCHA:
			return(Defmsg_TouchA());
		case GOBJMSG_TOUCHB://攻撃が当たった（攻撃力のoff）
			data.kougeki = FALSE;
			break;
		case GOBJMSG_ACTION:
			Defmsg_Action();
			break;
		case GOBJMSG_CLIPX:
			if(data.objtype & GOBJFLG_CLIPX2){
				data.x += *((int*)prm);
				if((data.aid&ACTID_KURAI || data.aid&ACTID_GUARD) && !(data.aid&ACTID_KUCYU)){//敵を押し戻す
					if(data.atk2.flags&ATKINFO2_ATTACKERBACK){
						pobj = battleTask->GetGObject( data.atk2.oid );
						if(pobj!=NULL){
							pobj->Message(GOBJMSG_KNOCKBACK,prm);
						}
					}
				}
			}
			break;
		case GOBJMSG_KNOCKBACK:
			data.x+=*((int*)prm);
			break;
		case GOBJMSG_CNGTARGET:
			data.eid = prm;
			break;
		}
	}

	//後処理
	switch(type){
	case GOBJMSG_COMMAND:
		ActionIDChanged(FALSE);
		break;
	case GOBJMSG_ACTION:
		data.counter++;
		ActionIDChanged(FALSE);
		if (actid_prv != data.aid)
		{
			if (actloop_count < 100)
			{
				actloop_count++;
				Message(GOBJMSG_ACTION);
			}
#ifdef _DEBUG
			else OutputDebugString("WARNING: ACTIDが変化し続け、無限ループを起こしています。\n");
#endif
			actloop_count = 0;
		}
		break;
	case GOBJMSG_DRAW:
		data.x += siverdx;
		break;
	}

	return(ret);
}

// デフォルトメッセージ処理 *********************************************************

void CGObject::Defmsg_Draw()//描画。cellを描画するのみ
{
	BOOL rev_x=FALSE;
	if(data.revx)rev_x=!rev_x;
	if(data.muki)rev_x=!rev_x;

	//描画の設定
	if(data.alphamode!=0)g_draw.SetAlphaMode(data.alphamode);
	if(data.objtype & GOBJFLG_DISPZAHYO)battleTask->SetTransform(FALSE);

	BOOL shadow = FALSE;

	if (data.y < 10 && data.y > -500 && abs(data.x) <= 640.0)
		shadow = TRUE;

	g_draw.CellDraw(data.pmsarr,data.pcdat,data.prdat,
		data.cnow,
		(g_draw.devtypenow == D3DDEVTYPE_HAL || data.objtype & GOBJFLG_DISPZAHYO) ? (int)data.x : (int)data.x - ((int)battleTask->GetDisplayCenterX() - g_DISPLAYWIDTH / 2),
		(g_draw.devtypenow == D3DDEVTYPE_HAL || data.objtype & GOBJFLG_DISPZAHYO) ? (int)data.y : (int)data.y + 440,data.z,
		data.rot,rev_x,data.revy,data.color,data.magx,data.magy,shadow);

	//描画の設定・元に戻しておく
	if(data.objtype & GOBJFLG_DISPZAHYO)battleTask->SetTransform(TRUE);
	if(data.alphamode!=0)g_draw.SetAlphaMode(0);
}

DWORD CGObject::Defmsg_TouchA()//相手の攻撃に当たったときの処理
{
	//喰らった攻撃情報→ data.atk2
	ATTACKINFO *info = data.atk2.info1;

	DWORD keyinput = CExport::GetKeyInput(data.id);//オブジェクトidをgetkeyinputに代入して指定キャラのキー入力インデックス値を取得
	DWORD key_now = g_input.GetKeyEx(keyinput,0);	//直った・・・？

	// 待機/退避中は無視しちゃう
	if(data.aid & ACTID_INOUT)return(TOUCHA_AVOID);
	//投げ中は無視しちゃう
	if(data.aid & ACTID_NAGE)return(TOUCHA_AVOID);

	//AQ防止?
	if(data.aid==ACTID_DOWN2)return(TOUCHA_AVOID);
	//喰らい、またはガード中
	if(data.aid & ACTID_KURAI || data.aid & ACTID_GUARD){
		if(data.aid & ACTID_KUCYU){////ジャンプ動作中
			if(data.aid & ACTID_GUARD)return(TOUCHA_GUARDJ);
			else return(TOUCHA_KURAIJ);
		}
		else if(data.aid & ACTID_SYAGAMI){//しゃがみ中
			if(!(info->guard & GUARDINFO_XCROUCH)){
				if(data.aid & ACTID_GUARD)return(TOUCHA_GUARDC);
				else return(TOUCHA_KURAIC);
			}
		}
		else{//立ち
			if(!(info->guard & GUARDINFO_XSTAND)){
				if(data.aid & ACTID_GUARD)return(TOUCHA_GUARDS);
				else return(TOUCHA_KURAIS);
			}
		}
	}

	//攻撃動作中（無条件に喰らう）
	if((data.aid & ACTID_ATTACK) || (data.aid & ACTID_KURAI)){
		if(data.aid & ACTID_KUCYU){//ジャンプ動作中
			return(TOUCHA_KURAIJ);
		}
		else if(data.aid & ACTID_SYAGAMI){//しゃがみ中
			return(TOUCHA_KURAIC);
		}
		else return(TOUCHA_KURAIS);
	}

	//基本動作中（レバー（裏）が入ってればガード）
	if(data.aid & ACTID_KUCYU){//ジャンプ動作中
		if((data.aid==ACTID_RAKKA) && (key_now & KEYSTA_BACK)){
			return(TOUCHA_GUARDJ);
		}
		if((key_now & KEYSTA_BACK) && !(info->guard & GUARDINFO_XJAMP)){
			return(TOUCHA_GUARDJ);
		}
		else return(TOUCHA_KURAIJ);
	}
	if(key_now & KEYSTA_BACK){
		if(key_now & KEYSTA_DOWN){
			if(info->guard & GUARDINFO_XCROUCH){
				return(TOUCHA_KURAIC);
			}
			else {
				return(TOUCHA_GUARDC);
			}
		}
		else{
			if(info->guard & GUARDINFO_XSTAND){
				return(TOUCHA_KURAIS);
			}
			else {
				return(TOUCHA_GUARDS);
			}
		}
	}
	else{
		if(data.aid & ACTID_SYAGAMI)return(TOUCHA_KURAIC);
		else return(TOUCHA_KURAIS);
	}

	return(TOUCHA_AVOID);
}

void CGObject::ActionIDChanged(BOOL ck,BOOL force)//行動IDが変わったときの処理
{
	if(!force){
		if(ck){actid_prv=data.aid;return;}
		if(actid_prv == data.aid)return;
	}

	data.revx = data.revy = FALSE;
	data.rot=0;
	data.magx = data.magy = 1.0f;

	data.counter = 0;
	data.kasanari= TRUE;
	data.kougeki	= FALSE;
	data.muteki	= FALSE;
	data.nagerare= TRUE;
	data.nonstop = FALSE;

	data.color = 0xFFFFFFFF;

	MessageDirect(GOBJMSG_CNGAID,0);
}

//******************************************************************
//  デフォルト喰らい動作
//******************************************************************

//======================================================================================
void CGObject::DmgMovex(GOBJECT *pdat,double mx)
{
	if (mx < 0)
	{
		if(pdat->atk2.flags & ATKINFO2_RIGHTBACK){
			pdat->x -= mx;
		}
		else{
			pdat->x += mx;
		}
	}
}

void CGObject::Furimuki(GOBJECT *pdat)
{
	CBattleTaskBase* bTask = dynamic_cast<CBattleTaskBase*>( g_system.GetCurrentMainTask() );
	GOBJECT *pedat = (GOBJECT*)bTask->GetGObjectInfo(pdat->eid);
	if(pedat==NULL)return;

	if(pdat->x > pedat->x)pdat->muki=TRUE;
	else pdat->muki=FALSE;
}

void CGObject::Defmsg_Action()
{
	if(data.aid & ACTID_GUARD)data.nagerare =FALSE;

	switch(data.aid){
	case ACTID_DAMAGE1		:dact_damages1(&data);break;//立ち喰らい（小）
	case ACTID_DAMAGE2		:dact_damages2(&data);break;//立ち喰らい（中）
	case ACTID_DAMAGE3		:dact_damages3(&data);break;//立ち喰らい（大）
	case ACTID_DAMAGEC1		:dact_damagec1(&data);break;//しゃがみ喰らい（小）
	case ACTID_DAMAGEC2		:dact_damagec2(&data);break;//しゃがみ喰らい（中）
	case ACTID_DAMAGEC3		:dact_damagec3(&data);break;//しゃがみ喰らい（大）
	case ACTID_DAMAGEJ1		:dact_damagej1(&data);break;//空中喰らい（小）
	case ACTID_DAMAGEJ2		:dact_damagej2(&data);break;//空中喰らい（中）
	case ACTID_DAMAGEJ3		:dact_damagej3(&data);break;//空中喰らい（大）
	case ACTID_DOWN			:dact_down(&data);break;//転倒
	case ACTID_DOWN2		:dact_down2(&data);break;//寝てる
	case ACTID_FINALDOWN	:dact_finaldown(&data);break;//起き上がり
	case ACTID_FUTTOBI		:dact_futtobi(&data);break;//ふっとび
	case ACTID_BOUND		:dact_bound(&data);break;//バウンド
	case ACTID_OKIAGARI		:dact_okiagari(&data);break;//おきあがり
	case ACTID_FINALBOUND	:dact_finalbound(&data);break;//氏にバウンド
	case ACTID_FINALDOWN2	:dact_finaldown2(&data);break;
	case ACTID_FUTTOBI2		:dact_futtobi2(&data);break;//ぶっとび2
	case ACTID_DAMAGE1A		:dact_damages1a(&data);break;//立ち喰らい（小）、のけぞりなし
	case ACTID_DAMAGE2A		:dact_damages2a(&data);break;//立ち喰らい（中）、のけぞりなし
	case ACTID_DAMAGE3A		:dact_damages3a(&data);break;//立ち喰らい（大）、のけぞりなし
	case ACTID_DAMAGEC1A	:dact_damagec1a(&data);break;//しゃがみ喰らい（小）、のけぞりなし
	case ACTID_DAMAGEC2A	:dact_damagec2a(&data);break;//しゃがみ喰らい（中）、のけぞりなし
	case ACTID_DAMAGEC3A	:dact_damagec3a(&data);break;//しゃがみ喰らい（大）、のけぞりなし
	case ACTID_TATAKITUKE1A	:dact_tatakituke1a(&data);break;//真下叩きつけ、バウンドあり
	case ACTID_TATAKITUKE2A	:dact_tatakituke2a(&data);break;//　　〃　　　、〃　　　なし
	case ACTID_TATAKITUKE1B	:dact_tatakituke1b(&data);break;//斜め叩きつけ、バウンドあり
	case ACTID_TATAKITUKE2B	:dact_tatakituke2b(&data);break;//　　〃　　　、〃　　　なし
	case ACTID_KAITENFINISH	:dact_kaitenfinish(&data);break;//回転フィニッシュ
	case ACTID_USERHIT1		:dact_userhit1();break;//ユーザー定義喰らい(地上)
	case ACTID_USERHIT2		:dact_userhit2();break;//ユーザー定義喰らい(空中)

	case ACTID_GUARD1		:dact_guards1(&data);break;//立ちガード（小）
	case ACTID_GUARD2		:dact_guards2(&data);break;//立ちガード（中）
	case ACTID_GUARD3		:dact_guards3(&data);break;//立ちガード（大）
	case ACTID_GUARDC1		:dact_guardc1(&data);break;//しゃがみガード（小）
	case ACTID_GUARDC2		:dact_guardc2(&data);break;//しゃがみガード（中）
	case ACTID_GUARDC3		:dact_guardc3(&data);break;//しゃがみガード（大）
	case ACTID_GUARDJ1		:dact_guardj1(&data);break;//空中ガード（小）
	case ACTID_GUARDJ2		:dact_guardj2(&data);break;//空中ガード（中）
	case ACTID_GUARDJ3		:dact_guardj3(&data);break;//空中ガード（大）
	case ACTID_USERGUARD1	:dact_userguard1();break;//ユーザー定義ガード(地上)
	case ACTID_USERGUARD2	:dact_userguard2();break;//ユーザー定義ガード(空中)
	default:return;
	}
}

//喰らい動作======================================================================================
void CGObject::dact_damages1(GOBJECT* pdat)//立ち喰らい(弱)
{
	if(pdat->counter==0){
		pdat->vx=-3;
	}
	DmgMovex(pdat,pdat->vx / 2);
	pdat->vx+=((pdat->counter%3)/2);
	if(pdat->vx >2){
		pdat->vx=0;
		pdat->aid = ACTID_NEUTRAL;
	}

	if(pdat->counter < 2){
		pdat->cnow = DCELL_DAMAGES1;}
	else if(pdat->counter < 4){
		pdat->cnow = DCELL_DAMAGES2;}
	else if(pdat->counter < 6){
		pdat->cnow = DCELL_DAMAGES3;}
	else if(pdat->counter < 8){
		pdat->cnow = DCELL_DAMAGES4;}
}

void CGObject::dact_damages2(GOBJECT* pdat)//立ち喰らい(中)
{
	if(pdat->counter==0){
		pdat->vx=-5;
	}
	DmgMovex(pdat,pdat->vx);
	pdat->vx+=((pdat->counter%3)/2);
	if(pdat->vx >2){
		pdat->vx=0;
		pdat->aid = ACTID_NEUTRAL;
	}

	if(pdat->counter < 6){
		pdat->cnow = DCELL_DAMAGES1;}
	else if(pdat->counter < 8){
		pdat->cnow = DCELL_DAMAGES2;}
	else if(pdat->counter < 10){
		pdat->cnow = DCELL_DAMAGES3;}
	else if(pdat->counter < 12){
		pdat->cnow = DCELL_DAMAGES4;}
}

void CGObject::dact_damages3(GOBJECT* pdat)//立ち喰らい(強)
{
	if(pdat->counter==0){
		pdat->vx=-7;
	}
	DmgMovex(pdat,pdat->vx);
	pdat->vx+=((pdat->counter%3)/2);
	if(pdat->vx >2){
		pdat->vx=0;
		pdat->aid = ACTID_NEUTRAL;
	}

	if(pdat->counter < 8){
		pdat->cnow = DCELL_DAMAGES1;}
	else if(pdat->counter < 12){
		pdat->cnow = DCELL_DAMAGES2;}
	else if(pdat->counter < 16){
		pdat->cnow = DCELL_DAMAGES3;}
	else if(pdat->counter < 18){
		pdat->cnow = DCELL_DAMAGES4;}
}

void CGObject::dact_damagec1(GOBJECT* pdat)//しゃがみ喰らい(弱)
{
	if(pdat->counter==0){
		pdat->vx=-3;
	}
	DmgMovex(pdat,pdat->vx / 2);
	pdat->vx+=((pdat->counter%3)/2);
	if(pdat->vx >2){
		pdat->vx=0;
		pdat->aid = ACTID_CROUCH;
	}

	if(pdat->counter < 2){
		pdat->cnow = DCELL_DAMAGEC1;}
	else if(pdat->counter < 4){
		pdat->cnow = DCELL_DAMAGEC2;}
	else if(pdat->counter < 6){
		pdat->cnow = DCELL_DAMAGEC3;}
	else if(pdat->counter < 8){
		pdat->cnow = DCELL_DAMAGEC4;}
}

void CGObject::dact_damagec2(GOBJECT* pdat)//しゃがみ喰らい(中)
{
	if(pdat->counter==0){
		pdat->vx=-5;
	}
	DmgMovex(pdat,pdat->vx);
	pdat->vx+=((pdat->counter%3)/2);
	if(pdat->vx >2){
		pdat->vx=0;
		pdat->aid = ACTID_CROUCH;
	}

	if(pdat->counter < 6){
		pdat->cnow = DCELL_DAMAGEC1;}
	else if(pdat->counter < 8){
		pdat->cnow = DCELL_DAMAGEC2;}
	else if(pdat->counter < 10){
		pdat->cnow = DCELL_DAMAGEC3;}
	else if(pdat->counter < 12){
		pdat->cnow = DCELL_DAMAGEC4;}
	else pdat->aid = ACTID_CROUCH;
}

void CGObject::dact_damagec3(GOBJECT* pdat)//しゃがみ喰らい(強)
{
	if(pdat->counter==0){
		pdat->vx=-7;
	}
	DmgMovex(pdat,pdat->vx);
	pdat->vx+=((pdat->counter%3)/2);
	if(pdat->vx >2){
		pdat->vx=0;
		pdat->aid = ACTID_CROUCH;
	}

	if(pdat->counter < 8){
		pdat->cnow = DCELL_DAMAGEC1;}
	else if(pdat->counter < 12){
		pdat->cnow = DCELL_DAMAGEC2;}
	else if(pdat->counter < 16){
		pdat->cnow = DCELL_DAMAGEC3;}
	else if(pdat->counter < 18){
		pdat->cnow = DCELL_DAMAGEC4;}
}

void CGObject::dact_damages1a(GOBJECT* pdat)//立ち喰らい(弱)、のけぞりなし
{
	if(pdat->counter==0){
		pdat->vx=-5;
	}
//	DmgMovex(pdat,pdat->vx);
	pdat->vx+=((pdat->counter%3)/2);
	if(pdat->vx >0){
		pdat->vx=0;
		pdat->aid = ACTID_NEUTRAL;
	}

	if(pdat->counter < 2){
		pdat->cnow = DCELL_DAMAGES1;}
	else if(pdat->counter < 4){
		pdat->cnow = DCELL_DAMAGES2;}
	else if(pdat->counter < 6){
		pdat->cnow = DCELL_DAMAGES3;}
	else if(pdat->counter < 8){
		pdat->cnow = DCELL_DAMAGES4;}
}

void CGObject::dact_damages2a(GOBJECT* pdat)//立ち喰らい(中)、のけぞりなし
{
	if(pdat->counter==0){
		pdat->vx=-7;
	}
//	DmgMovex(pdat,pdat->vx);
	pdat->vx+=((pdat->counter%3)/2);
	if(pdat->vx >0){
		pdat->vx=0;
		pdat->aid = ACTID_NEUTRAL;
	}

	if(pdat->counter < 6){
		pdat->cnow = DCELL_DAMAGES1;}
	else if(pdat->counter < 8){
		pdat->cnow = DCELL_DAMAGES2;}
	else if(pdat->counter < 10){
		pdat->cnow = DCELL_DAMAGES3;}
	else if(pdat->counter < 12){
		pdat->cnow = DCELL_DAMAGES4;}
}

void CGObject::dact_damages3a(GOBJECT* pdat)//立ち喰らい(強)、のけぞりなし
{
	if(pdat->counter==0){
		pdat->vx=-9;
	}
//	DmgMovex(pdat,pdat->vx);
	pdat->vx+=((pdat->counter%3)/2);
	if(pdat->vx >0){
		pdat->vx=0;
		pdat->aid = ACTID_NEUTRAL;
	}

	if(pdat->counter < 8){
		pdat->cnow = DCELL_DAMAGES1;}
	else if(pdat->counter < 12){
		pdat->cnow = DCELL_DAMAGES2;}
	else if(pdat->counter < 16){
		pdat->cnow = DCELL_DAMAGES3;}
	else if(pdat->counter < 18){
		pdat->cnow = DCELL_DAMAGES4;}
}

void CGObject::dact_damagec1a(GOBJECT* pdat)//しゃがみ喰らい(弱)、のけぞりなし
{
	if(pdat->counter==0){
		pdat->vx=-5;
	}
//	DmgMovex(pdat,pdat->vx);
	pdat->vx+=((pdat->counter%3)/2);
	if(pdat->vx >0){
		pdat->vx=0;
		pdat->aid = ACTID_CROUCH;
	}

	if(pdat->counter < 2){
		pdat->cnow = DCELL_DAMAGEC1;}
	else if(pdat->counter < 4){
		pdat->cnow = DCELL_DAMAGEC2;}
	else if(pdat->counter < 6){
		pdat->cnow = DCELL_DAMAGEC3;}
	else if(pdat->counter < 8){
		pdat->cnow = DCELL_DAMAGEC4;}
}

void CGObject::dact_damagec2a(GOBJECT* pdat)//しゃがみ喰らい(中)、のけぞりなし
{
	if(pdat->counter==0){
		pdat->vx=-7;
	}
//	DmgMovex(pdat,pdat->vx);
	pdat->vx+=((pdat->counter%3)/2);
	if(pdat->vx >0){
		pdat->vx=0;
		pdat->aid = ACTID_CROUCH;
	}

	if(pdat->counter < 6){
		pdat->cnow = DCELL_DAMAGEC1;}
	else if(pdat->counter < 8){
		pdat->cnow = DCELL_DAMAGEC2;}
	else if(pdat->counter < 10){
		pdat->cnow = DCELL_DAMAGEC3;}
	else if(pdat->counter < 12){
		pdat->cnow = DCELL_DAMAGEC4;}
	else pdat->aid = ACTID_CROUCH;
}

void CGObject::dact_damagec3a(GOBJECT* pdat)//しゃがみ喰らい(強)、のけぞりなし
{
	if(pdat->counter==0){
		pdat->vx=-9;
	}
//	DmgMovex(pdat,pdat->vx);
	pdat->vx+=((pdat->counter%3)/2);
	if(pdat->vx >0){
		pdat->vx=0;
		pdat->aid = ACTID_CROUCH;
	}

	if(pdat->counter < 8){
		pdat->cnow = DCELL_DAMAGEC1;}
	else if(pdat->counter < 12){
		pdat->cnow = DCELL_DAMAGEC2;}
	else if(pdat->counter < 16){
		pdat->cnow = DCELL_DAMAGEC3;}
	else if(pdat->counter < 18){
		pdat->cnow = DCELL_DAMAGEC4;}
}

void CGObject::dact_damagej1(GOBJECT* pdat)//空中喰らい(弱)
{
	if(pdat->counter==0){
		pdat->vx=-4;
		pdat->vy=-9;
	}
	DmgMovex(pdat,pdat->vx);
	pdat->y+=pdat->vy;

	if(pdat->counter < 12){pdat->cnow = DCELL_DAMAGEJ;}
	else {
		if(pdat->atk2.flags & ATKINFO2_RIGHTBACK)pdat->muki = TRUE;
		else pdat->muki =FALSE;
		pdat->aid = ACTID_RAKKA;
	}
}

void CGObject::dact_damagej2(GOBJECT* pdat)//空中喰らい(中)
{
	if(pdat->counter==0){
		pdat->vx=-6;
		pdat->vy=-9;
	}
	DmgMovex(pdat,pdat->vx);
	pdat->y+=pdat->vy;

	if(pdat->counter < 6){pdat->cnow = DCELL_DAMAGEJ;}
	else if(pdat->counter < 9){pdat->cnow = DCELL_DAMAGEJ;}
	else if(pdat->counter < 15){pdat->cnow = DCELL_DAMAGEJ;}
	else {
		if(pdat->atk2.flags & ATKINFO2_RIGHTBACK)pdat->muki = TRUE;
		else pdat->muki =FALSE;
		pdat->aid = ACTID_RAKKA;
	}
}

void CGObject::dact_damagej3(GOBJECT* pdat)//空中喰らい(強)
{
	if(pdat->counter==0){
		pdat->vx=-7;
		pdat->vy=-9;
	}
	DmgMovex(pdat,pdat->vx);
	pdat->y+=pdat->vy;
	pdat->vx+=(pdat->counter%3)/3;

	if(pdat->counter < 8){pdat->cnow = DCELL_DAMAGEJ;}
	else if(pdat->counter < 14){pdat->cnow = DCELL_DAMAGEJ;}
	else if(pdat->counter < 18){pdat->cnow = DCELL_DAMAGEJ;}
	else {
		if(pdat->atk2.flags & ATKINFO2_RIGHTBACK)pdat->muki = TRUE;
		else pdat->muki =FALSE;
		pdat->aid = ACTID_RAKKA;
	}
}

void CGObject::dact_down(GOBJECT* pdat)//転倒
{
	pdat->muteki=TRUE;
	pdat->nagerare=FALSE;

	if(pdat->counter==0){
		pdat->vx=-7;
		pdat->vy=-12;
	}
	DmgMovex(pdat,pdat->vx);
	pdat->y+=pdat->vy;
	pdat->vy+=1+pdat->counter%2;

	if(pdat->counter < 8){pdat->cnow = DCELL_DAMAGEJ;}
	else if(pdat->counter < 12){pdat->cnow = DCELL_RAKKA1;}
	else if(pdat->counter < 16){pdat->cnow = DCELL_RAKKA2;}

	if(pdat->y > 0){
		pdat->y=0;
		pdat->aid=ACTID_DOWN2;
	}
}

void CGObject::dact_down2(GOBJECT* pdat)//ダウン
{
	pdat->muteki=TRUE;
	pdat->nagerare=FALSE;

	if(pdat->vy > 15)pdat->aid=ACTID_BOUND;

	pdat->cnow=DCELL_DOWN;
	if(pdat->counter > 25)pdat->aid = ACTID_OKIAGARI;
}

void CGObject::dact_finaldown(GOBJECT* pdat)//氏に
{
	pdat->muteki=TRUE;
	pdat->nagerare=FALSE;

	if(pdat->counter==0){
		pdat->vx=-2;
		pdat->vy=-18;
	}
	DmgMovex(pdat,pdat->vx);
	pdat->y+=pdat->vy;
	pdat->vy+=1;

	if(pdat->counter < 8){pdat->cnow = DCELL_DAMAGEJ;}
	else if(pdat->counter < 12){pdat->cnow = DCELL_RAKKA1;}
	else if(pdat->counter < 16){pdat->cnow = DCELL_RAKKA2;}

	if(pdat->y > 0){
		pdat->y=0;
		pdat->cnow = DCELL_DOWN;
		pdat->aid=ACTID_FINALBOUND;
	}
}

void CGObject::dact_futtobi(GOBJECT* pdat)//ふっとび
{
	if(pdat->counter==0){
		pdat->vx=-12;
		pdat->vy=-20;
	}
	DmgMovex(pdat,pdat->vx);
	pdat->y+=pdat->vy;
	pdat->vy+=1+pdat->counter%2;

	if(pdat->counter < 8){pdat->cnow = DCELL_DAMAGEJ;}
	else if(pdat->counter < 12){pdat->cnow = DCELL_RAKKA1;}
	else if(pdat->counter < 16){pdat->cnow = DCELL_RAKKA2;}

	if(pdat->y > 0){
		pdat->y=0;
		pdat->aid=ACTID_DOWN2;
	}
}

void CGObject::dact_futtobi2(GOBJECT* pdat)//ふっとび(エリアル風味)
{
	if(pdat->counter==0){
		DmgMovex(pdat, -30);
		pdat->vx=-3;
		pdat->vy=-25;
	}
	DmgMovex(pdat,pdat->vx);
	pdat->y+=pdat->vy;
	pdat->vy+=1+pdat->counter%2;

	if(pdat->counter < 8){pdat->cnow = DCELL_DAMAGEJ;}
	else if(pdat->counter < 12){pdat->cnow = DCELL_RAKKA1;}
	else if(pdat->counter < 16){pdat->cnow = DCELL_RAKKA2;}

	if(pdat->y > 0){
		pdat->y=0;
		pdat->aid=ACTID_DOWN2;
	}
}

void CGObject::dact_bound(GOBJECT* pdat)//バウンド
{
	pdat->muteki=TRUE;
	pdat->nagerare=FALSE;

	if(pdat->counter==0){
		pdat->vx=-7;
		pdat->vy=-10;
	}
	DmgMovex(pdat,pdat->vx);
	pdat->y+=pdat->vy;
	pdat->vy+=1+pdat->counter%2;

	pdat->cnow = DCELL_BOUND;

	if(pdat->y > 0){
		pdat->y=0;
		pdat->aid=ACTID_DOWN2;
	}
}

void CGObject::dact_okiagari(GOBJECT* pdat)//おきあがり
{
	pdat->muteki=TRUE;
	pdat->nagerare=FALSE;

	if(pdat->counter < 4)pdat->cnow=DCELL_OKIAGARI1;
	else if(pdat->counter < 8)pdat->cnow=DCELL_OKIAGARI2;
	else if(pdat->counter < 12)pdat->cnow=DCELL_OKIAGARI3;
	else if(pdat->counter < 16)pdat->cnow=DCELL_OKIAGARI4;
	else if(pdat->counter < 20)pdat->cnow=DCELL_OKIAGARI5;
	else{
		Furimuki(pdat);
		pdat->aid=ACTID_NEUTRAL;
	}
	
}

void CGObject::dact_finalbound(GOBJECT* pdat)//バウンド
{
	pdat->muteki=TRUE;
	pdat->nagerare=FALSE;

	if(pdat->counter==0){
		pdat->vx=-1;
		pdat->vy=-10;
	}
	DmgMovex(pdat,pdat->vx);
	pdat->y+=pdat->vy;
	pdat->vy+=1+pdat->counter%2;

	pdat->cnow = DCELL_BOUND;

	if(pdat->y > 0){
		pdat->y=0;
		pdat->aid=ACTID_FINALDOWN2;
	}
}

void CGObject::dact_finaldown2(GOBJECT* pdat)
{
	pdat->muteki=TRUE;
	pdat->vy=0;
	pdat->cnow=DCELL_DOWN;

	if(pdat->counter == 120){
		battleTask->MessageFromObject(pdat->id,MSGOBJ2SYS_FINALDOWNEND,0);
	}
}

void CGObject::dact_tatakituke(GOBJECT* pdat)//タタキツケ
{
	DmgMovex(pdat,pdat->vx);
	pdat->y+=pdat->vy;

	pdat->cnow = DCELL_DAMAGEJ;
	pdat->revy = TRUE;
}

void CGObject::dact_tatakituke1a(GOBJECT* pdat)
{
	if(pdat->counter==0){
		pdat->vx=0;
		pdat->vy=24;
	}
	dact_tatakituke(pdat);
	if(pdat->y > 0){
		pdat->y=0;
		pdat->aid=ACTID_BOUND;
	}
}

void CGObject::dact_tatakituke2a(GOBJECT* pdat)
{
	if(pdat->counter==0){
		pdat->vx=0;
		pdat->vy=24;
	}
	dact_tatakituke(pdat);
	if(pdat->y > 0){
		pdat->y=0;
		pdat->aid=ACTID_DOWN;
	}
}

void CGObject::dact_tatakituke1b(GOBJECT* pdat)
{
	if(pdat->counter==0){
		pdat->vx=-15;
		pdat->vy=24;
	}
	dact_tatakituke(pdat);
	if(pdat->y > 0){
		pdat->y=0;
		pdat->aid=ACTID_BOUND;
	}
}

void CGObject::dact_tatakituke2b(GOBJECT* pdat)
{
	if(pdat->counter==0){
		pdat->vx=-15;
		pdat->vy=24;
	}
	dact_tatakituke(pdat);
	if(pdat->y > 0){
		pdat->y=0;
		pdat->aid=ACTID_DOWN;
	}
}

void CGObject::dact_kaitenfinish(GOBJECT* pdat)//回転してぶっとぶ
{
	if(TRUE)//pdat->counter==0){
	{
		pdat->vx = -20;
		pdat->vy = -20;
	}
	if(pdat->counter==30)pdat->objtype &= GOBJTYPE_DEAD;

	pdat->cnow = DCELL_KAITENFUTTOBI;
	pdat->rot = (pdat->counter*60)%360;
	DmgMovex(pdat,pdat->vx);
	pdat->y += pdat->vy;

	if(pdat->counter==120){
		battleTask->MessageFromObject(pdat->id,MSGOBJ2SYS_FINALDOWNEND,0);
	}
}

//ガード動作================================================================================

void CGObject::dact_guards1(GOBJECT* pdat)//立ちガード(弱)
{
	if(pdat->counter < 2){
		pdat->cnow = DCELL_GUARDS1;DmgMovex(pdat,-2);}
	else if(pdat->counter < 4){
		pdat->cnow = DCELL_GUARDS2;DmgMovex(pdat,-1);}
	else if(pdat->counter < 6){
		pdat->cnow = DCELL_GUARDS3;DmgMovex(pdat,-1);}
	else if(pdat->counter < 8){
		pdat->cnow = DCELL_GUARDS4;DmgMovex(pdat,0);}
	else pdat->aid = ACTID_NEUTRAL;
}

void CGObject::dact_guards2(GOBJECT* pdat)//立ちガード(中)
{
	if(pdat->counter < 4){
		pdat->cnow = DCELL_GUARDS1;DmgMovex(pdat,-2);}
	else if(pdat->counter < 6){
		pdat->cnow = DCELL_GUARDS2;DmgMovex(pdat,-1);}
	else if(pdat->counter < 8){
		pdat->cnow = DCELL_GUARDS3;DmgMovex(pdat,-1);}
	else if(pdat->counter < 12){
		pdat->cnow = DCELL_GUARDS4;DmgMovex(pdat,0);}
	else pdat->aid = ACTID_NEUTRAL;
}

void CGObject::dact_guards3(GOBJECT* pdat)//立ちガード(強)
{
	if(pdat->counter < 4){
		pdat->cnow = DCELL_GUARDS1;DmgMovex(pdat,-2);}
	else if(pdat->counter < 8){
		pdat->cnow = DCELL_GUARDS2;DmgMovex(pdat,-1);}
	else if(pdat->counter < 12){
		pdat->cnow = DCELL_GUARDS3;DmgMovex(pdat,-1);}
	else if(pdat->counter < 16){
		pdat->cnow = DCELL_GUARDS4;DmgMovex(pdat,0);}
	else pdat->aid = ACTID_NEUTRAL;
}

void CGObject::dact_guardc1(GOBJECT* pdat)//しゃがみガード(弱)
{
	if(pdat->counter < 2){
		pdat->cnow = DCELL_GUARDC1;DmgMovex(pdat,-2);}
	else if(pdat->counter < 4){
		pdat->cnow = DCELL_GUARDC2;DmgMovex(pdat,-1);}
	else if(pdat->counter < 6){
		pdat->cnow = DCELL_GUARDC3;DmgMovex(pdat,-1);}
	else if(pdat->counter < 8){
		pdat->cnow = DCELL_GUARDC4;DmgMovex(pdat,0);}
	else pdat->aid = ACTID_CROUCH;
}

void CGObject::dact_guardc2(GOBJECT* pdat)//しゃがみガード(中)
{
	if(pdat->counter < 4){
		pdat->cnow = DCELL_GUARDC1;DmgMovex(pdat,-2);}
	else if(pdat->counter < 6){
		pdat->cnow = DCELL_GUARDC2;DmgMovex(pdat,-1);}
	else if(pdat->counter < 8){
		pdat->cnow = DCELL_GUARDC3;DmgMovex(pdat,-1);}
	else if(pdat->counter < 12){
		pdat->cnow = DCELL_GUARDC4;DmgMovex(pdat,0);}
	else pdat->aid = ACTID_CROUCH;
}

void CGObject::dact_guardc3(GOBJECT* pdat)//しゃがみガード(強)
{
	if(pdat->counter < 4){
		pdat->cnow = DCELL_GUARDC1;DmgMovex(pdat,-2);}
	else if(pdat->counter < 8){
		pdat->cnow = DCELL_GUARDC2;DmgMovex(pdat,-1);}
	else if(pdat->counter < 12){
		pdat->cnow = DCELL_GUARDC3;DmgMovex(pdat,-1);}
	else if(pdat->counter < 16){
		pdat->cnow = DCELL_GUARDC4;DmgMovex(pdat,0);}
	else pdat->aid = ACTID_CROUCH;
}

void CGObject::dact_guardj1(GOBJECT* pdat)//空中ガード(弱)
{
	if(pdat->counter < 2){
		pdat->cnow = DCELL_GUARDJ1;DmgMovex(pdat,-2);}
	else if(pdat->counter < 4){
		pdat->cnow = DCELL_GUARDJ2;DmgMovex(pdat,-1);}
	else if(pdat->counter < 8){
		pdat->cnow = DCELL_GUARDJ3;DmgMovex(pdat,0);}
	else {
		if(pdat->atk2.flags & ATKINFO2_RIGHTBACK)pdat->muki = TRUE;
		else pdat->muki =FALSE;
		pdat->aid = ACTID_RAKKA;
	}
}

void CGObject::dact_guardj2(GOBJECT* pdat)//空中ガード(中)
{
	if(pdat->counter < 3){
		pdat->cnow = DCELL_GUARDJ1;DmgMovex(pdat,-2);}
	else if(pdat->counter < 6){
		pdat->cnow = DCELL_GUARDJ2;DmgMovex(pdat,-1);}
	else if(pdat->counter < 9){
		pdat->cnow = DCELL_GUARDJ3;DmgMovex(pdat,0);}
	else {
		if(pdat->atk2.flags & ATKINFO2_RIGHTBACK)pdat->muki = TRUE;
		else pdat->muki =FALSE;
		pdat->aid = ACTID_RAKKA;
	}
}

void CGObject::dact_guardj3(GOBJECT* pdat)//空中ガード(強)
{
	if(pdat->counter < 4){
		pdat->cnow = DCELL_GUARDJ1;DmgMovex(pdat,-2);}
	else if(pdat->counter < 8){
		pdat->cnow = DCELL_GUARDJ2;DmgMovex(pdat,-1);}
	else if(pdat->counter < 12){
		pdat->cnow = DCELL_GUARDJ3;DmgMovex(pdat,0);}
	else {
		if(pdat->atk2.flags & ATKINFO2_RIGHTBACK)pdat->muki = TRUE;
		else pdat->muki =FALSE;
		pdat->aid = ACTID_RAKKA;
	}
}

void CGObject::dact_userhit1()
{
	CGObject *pobj = (CGObject*)battleTask->GetGObject(userhit_attacker);
	if(pobj==NULL){//異常
		data.aid=ACTID_FUTTOBI;
		ActionIDChanged(TRUE,TRUE);
		return;
	}
	pobj->Message(GOBJMSG_USERDEFINE|userhit_id,data.id);
}

void CGObject::dact_userhit2()
{
	CGObject *pobj = (CGObject*)battleTask->GetGObject(userhit_attacker);
	if(pobj==NULL){//異常
		data.aid=ACTID_FUTTOBI;
		ActionIDChanged(TRUE,TRUE);
		return;
	}
	pobj->Message(GOBJMSG_USERDEFINE|userhit_id,data.id);
}

void CGObject::dact_userguard1()
{
	CGObject *pobj = (CGObject*)battleTask->GetGObject(userhit_attacker);
	if(pobj==NULL){//異常
		data.aid=ACTID_NEUTRAL;
		ActionIDChanged(TRUE,TRUE);
		return;
	}
	pobj->Message(GOBJMSG_USERDEFINE|userhit_id,data.id);
}

void CGObject::dact_userguard2()
{
	CGObject *pobj = (CGObject*)battleTask->GetGObject(userhit_attacker);
	if(pobj==NULL){//異常
		data.aid=ACTID_NEUTRAL;
		ActionIDChanged(TRUE,TRUE);
		return;
	}
	pobj->Message(GOBJMSG_USERDEFINE|userhit_id,data.id);
}


//******************************************************************
//  コンピュータコマンド処理
//******************************************************************

inline DWORD choiranw(DWORD *waz)//6つのDWORD配列のうち、0でないものをランダムに選択
{
	int r=timeGetTime()%6;
	if(waz[r]!=0)return(waz[r]);

	if(rand()%3==0)if(waz[0]!=0)return(waz[0]);
	if(rand()%3==0)if(waz[1]!=0)return(waz[1]);
	if(rand()%3==0)if(waz[2]!=0)return(waz[2]);
	if(rand()%3==0)if(waz[3]!=0)return(waz[3]);
	if(rand()%3==0)if(waz[4]!=0)return(waz[4]);
	if(rand()%3==0)if(waz[5]!=0)return(waz[5]);

	if(rand()%2==0)if(waz[0]!=0)return(waz[0]);
	if(rand()%2==0)if(waz[1]!=0)return(waz[1]);
	if(rand()%2==0)if(waz[2]!=0)return(waz[2]);
	if(rand()%2==0)if(waz[3]!=0)return(waz[3]);
	if(rand()%2==0)if(waz[4]!=0)return(waz[4]);
	if(rand()%2==0)if(waz[5]!=0)return(waz[5]);

	if(waz[0]!=0)return(waz[0]);
	if(waz[1]!=0)return(waz[1]);
	if(waz[2]!=0)return(waz[2]);
	if(waz[3]!=0)return(waz[3]);
	if(waz[4]!=0)return(waz[4]);
	if(waz[5]!=0)return(waz[5]);

	return(0);
}

void CGObject::ComCommand()
{
	int IsStopping = (data.counter == comhint_counterprev);
	DWORD wnext=0;//次に出したい技

	comhint_counterprev = data.counter;
	if(data.winfo==NULL)return;
	if(battleTask->GetState() != BFSTATE_FIGHTING && battleTask->GetState()!= BFSTATE_FIGHTING )return;
	if(data.msghandler == NULL)return;

	double guardratio;//ガードする確率
	switch(com_level){
	case DIFF_VERYEASY	:guardratio=0.2;break;
	case DIFF_EASY		:guardratio=0.4;break;
	case DIFF_HARD		:guardratio=0.8;break;
	case DIFF_VERYHARD	:guardratio=1.5;break;
	case DIFF_SUPERHARD	:guardratio=2.0;break;
	case DIFF_ULTRAHARD	:guardratio=2.5;break;
	case DIFF_LIMITERCUT:guardratio=3.0;break;
	default:guardratio=0.8;break;
	}

	//喰らい･ガード動作になったらユーザー指定をクリア
	if((data.aid & ACTID_KURAI) || (data.aid & ACTID_GUARD))
	{
		comhint_delay = -1;
	}

	//ユーザー指定行動
	if(comhint_delay>=0 && !IsStopping)
	{
		comhint_delay--;
		if(comhint_delay<=0)
		{
			//遷移
			if( comhint_aid!=0 )
			{
				if (MessageDirect(GOBJMSG_COMMANDCOM,comhint_aid))
				{
					comcounter=20;
					comguard=30*guardratio;
					data.aid=comhint_aid;
					comhint_delay = -1;
					comhint_capacity = 20;
				}
				else if (comhint_capacity > 0)
				{
					// 出来るまで粘る
					comhint_delay = 0;
					comhint_capacity--;
				}
				else
					comhint_capacity = 20;
			}
			return;
		}
		else return;
	}

	comcounter--;
	if(comcounter > 0)return;

	DWORD thinkret = MessageDirect(GOBJMSG_COMTHINK, 0);
	if (thinkret != 0)
	{
		// ユーザー指定思考ルーチン
		if ( MessageDirect(GOBJMSG_COMMANDCOM, thinkret) )
		{
			data.aid = thinkret;

			// 念のためこっちも設定
			comcounter=10;
			comguard=50*guardratio;
		}

		return;
	}

	int maai;
	GOBJECT *pedat;
	pedat = (GOBJECT*)battleTask->GetGObjectInfo(data.eid);
	if(pedat!=NULL){
		maai = (int)(data.x - pedat->x);//相手との間合い
		if(maai<0)maai*=(-1);
	}
	else return;

	if(data.aid & ACTID_KUCYU){//ジャンプ中の場合
		if( (maai<=comreach[1] && abs(data.y - pedat->y) <= comreach[1])/* && rand()%5!=0*/ ){//80%の確率で攻撃を出す
			wnext = choiranw(data.winfo->att_jamp);
			if(wnext!=0){
				if(MessageDirect(GOBJMSG_COMMANDCOM,wnext))data.aid=wnext;
			}
			comguard=30*guardratio;
			comcounter=40;
		}
		else{//防御を固める
			comguard=60*guardratio;
			comcounter=30;
		}
	}
	else{//地上
		if(maai > comreach[2]){//case "E"
			#ifdef _DBG_COMCOMMAND
			gbl.ods("ComCommand : 超遠距離");
			#endif
			switch(rand()%4){
			case 0://飛び道具を放つ
				wnext=choiranw(data.winfo->att_bullet);
				if(wnext!=0){
					if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
						comcounter=30;
						comguard=90*guardratio;
						data.aid=wnext;
						return;
					}
				}
				break;
			case 1://前進
				wnext=data.winfo->walkf;
				if(wnext!=0){
					if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
						comcounter=30;
						comguard=70*guardratio;
						data.aid=wnext;
						return;
					}
				}
				break;
			case 2://前ダッシュ
				wnext=data.winfo->dashf;
				if(wnext!=0){
					if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
						comcounter=25;
						comguard=30*guardratio;
						data.aid=wnext;
						return;
					}
				}
				break;
			default:
				wnext=ACTID_NEUTRAL;
				if(wnext!=0){
					if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
						comcounter=20;
						comguard=80*guardratio;
						data.aid=wnext;
						return;
					}
				}
			}
		}
		else if(maai > comreach[1]){//case "D"
			#ifdef _DBG_COMCOMMAND
			gbl.ods("ComCommand : 遠距離");
			#endif
			switch(rand()%8){
			case 0://前進
				wnext=data.winfo->walkf;
				if(wnext!=0){
					if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
						comcounter=10;
						comguard=60*guardratio;
						data.aid=wnext;
						return;
					}
				}
				break;
			case 1://前ジャンプ
				wnext=data.winfo->jampf;
				if(wnext!=0){
					if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
						comcounter=30;
						comguard=30*guardratio;
						data.aid=wnext;
						return;
					}
				}
				break;
			case 2:
			case 3://長リーチ攻撃
				wnext=choiranw(data.winfo->att_long);
				if(wnext!=0){
					if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
						comcounter=15;
						comguard=20*guardratio;
						data.aid=wnext;
						return;
					}
				}
				break;
			case 4://飛び道具
				wnext=choiranw(data.winfo->att_bullet);
				if(wnext!=0){
					if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
						comcounter=30;
						comguard=50*guardratio;
						data.aid=wnext;
						return;
					}
				}
				break;
			case 5://ダッシュ
				{
					wnext=data.winfo->dashf;
					if(wnext!=0){
						if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
							comcounter=20;
							comguard=30*guardratio;
							data.aid=wnext;
							return;
						}
					}
				}
				break;
			case 6://前ジャンプ（飛び込み）
				if (pedat->aid & ACTID_ATTACK)
				{
					wnext=data.winfo->jampf;
					if(wnext!=0){
						if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
							comcounter=15;
							comguard=30*guardratio;
							data.aid=wnext;
							return;
						}
					}
				}
				break;
			default://ニュートラル
				wnext=ACTID_NEUTRAL;
				if(wnext!=0){
					if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
						comcounter=30;
						comguard=80*guardratio;
						data.aid=wnext;
						return;
					}
				}
			}
		}
		else if(pedat!=NULL && pedat->y < -50){//case "C"
			#ifdef _DBG_COMCOMMAND
			gbl.ods("ComCommand : 敵空中");
			#endif
			switch(rand()%5){
			case 0://前進
				wnext=data.winfo->walkf;
				if(wnext!=0){
					if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
						comcounter=10;
						comguard=60*guardratio;
						data.aid=wnext;
						return;
					}
				}
				break;
			case 1://前ジャンプ
				wnext=data.winfo->jampf;
				if(wnext!=0){
					if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
						comcounter=15;
						comguard=30*guardratio;
						data.aid=wnext;
						return;
					}
				}
				break;
			case 2:
			case 3://対空技
				wnext=choiranw(data.winfo->att_tai);
				if(wnext!=0){
					if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
						comcounter=10;
						comguard=30*guardratio;
						data.aid=wnext;
						return;
					}
				}
				break;
			default://ニュートラル
				wnext=ACTID_NEUTRAL;
				if(wnext!=0){
					if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
						comcounter=5;
						comguard=70*guardratio;
						data.aid=wnext;
						return;
					}
				}
			}
		}
		else if(maai>comreach[0]){//case "B"
			#ifdef _DBG_COMCOMMAND
			gbl.ods("ComCommand : 中距離");
			#endif
			switch(rand()%6){
			case 0://前進
				if ( com_level <= DIFF_EASY || !(pedat->aid & ACTID_ATTACK) )
				{
					wnext=data.winfo->walkf;
					if(wnext!=0){
						if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
							comcounter=5;
							comguard=60*guardratio;
							data.aid=wnext;
							return;
						}
					}
				}
				break;
			/*case 1://前ジャンプ
				wnext=data.winfo->jampf;
				if(wnext!=0){
					if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
						comcounter=20;
						comguard=70*guardratio;
						data.aid=wnext;
						return;
					}
				}
				break;*/
			case 2:
			case 3://中リーチ攻撃
				wnext=choiranw(data.winfo->att_middle);
				if(wnext!=0){
					if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
						comcounter=10;
						comguard=70*guardratio;
						data.aid=wnext;
						return;
					}
				}
				break;
			case 4://ダッシュ
				if ( com_level <= DIFF_EASY || !(pedat->aid & ACTID_ATTACK) )
				{
					wnext=data.winfo->dashf;
					if(wnext!=0){
						if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
							comcounter=10;
							comguard=2*guardratio;
							data.aid=wnext;
							return;
						}
					}
				}
				break;
			case 5://後退
				if(pedat->aid & ACTID_ATTACK){
					wnext=data.winfo->walkb;
					if(wnext!=0){
						if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
							comcounter=5;
							comguard=500*guardratio;
							data.aid=wnext;
							return;
						}
					}
				}
				break;
			/*case 6://後退
				wnext=data.winfo->walkb;
				if(wnext!=0){
					if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
						comcounter=10;
						comguard=500*guardratio;
						data.aid=wnext;
						return;
					}
				}
				break;
			case 7://後ジャンプ
				wnext=data.winfo->jampb;
				if(wnext!=0){
					if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
						comcounter=20;
						comguard=70*guardratio;
						data.aid=wnext;
						return;
					}
				}
				break;*/
			default://ニュートラル
				wnext=ACTID_NEUTRAL;
				if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
					comcounter=30;
					comguard=70*guardratio;
					data.aid=wnext;
					return;
				}
			}
		}
		else{//case "A"
			#ifdef _DBG_COMCOMMAND
			gbl.ods("ComCommand : 近距離");
			#endif
			switch(rand()%7){
			case 0://前進
				if(com_level<=DIFF_NORMAL)
				{
					wnext=data.winfo->walkf;
					if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
						comcounter=3;
						comguard=70*guardratio;
						data.aid=wnext;
						return;
					}
				}
				else {//投げ
					wnext=choiranw(data.winfo->nageid);
					if(wnext!=0){
						if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
							comcounter=25;
							comguard=30*guardratio;
							data.aid=wnext;
							return;
						}
					}
				}
				break;
			case 1:
				if(com_level>=DIFF_NORMAL){
					wnext=choiranw(data.winfo->nageid);
					if(wnext!=0){
						if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
							comcounter=25;
							comguard=30*guardratio;
							data.aid=wnext;
							return;
						}
					}
				}
				break;
			case 2:
				if(com_level>=DIFF_SUPERHARD){
					wnext=choiranw(data.winfo->nageid);
					if(wnext!=0){
						if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
							comcounter=25;
							comguard=30*guardratio;
							data.aid=wnext;
							return;
						}
					}
				}
				break;
			case 3:
			case 5://短リーチ攻撃
				wnext=choiranw(data.winfo->att_short);
				if(wnext!=0){
					if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
						comcounter=5;
						comguard=30*guardratio;
						data.aid=wnext;
						return;
					}
				}
				break;
			case 4://後退
				if (pedat->aid & ACTID_ATTACK)
				{
					wnext=data.winfo->walkb;
					if(wnext!=0){
						if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
							comcounter=5;
							comguard=500*guardratio;
							data.aid=wnext;
							return;
						}
					}
				}
				break;
			/*case 4://後ダッシュ
				if(com_level>=DIFF_NORMAL){
					wnext=data.winfo->dashb;
					if(wnext!=0){
						if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
							comcounter=20;
							comguard=500*guardratio;
							data.aid=wnext;
							return;
						}
					}
				}
				break;
			case 5://後ジャンプ
				if(com_level>=DIFF_EASY){
					wnext=data.winfo->jampb;
					if(wnext!=0){
						if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
							comcounter=20;
							comguard=70*guardratio;
							data.aid=wnext;
							return;
						}
					}
				}
				break;*/
			default://ニュートラル
				wnext=ACTID_NEUTRAL;
				if(com_level>=DIFF_VERYHARD){
					wnext=choiranw(data.winfo->nageid);
					if(wnext!=0){
						if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
							comcounter=25;
							comguard=30*guardratio;
							data.aid=wnext;
							return;
						}
					}
				}
				else if(MessageDirect(GOBJMSG_COMMANDCOM,wnext)){
					comcounter=5;
					comguard=90*guardratio;
					data.aid=wnext;
					return;
				}
			}
		}
	}
}

