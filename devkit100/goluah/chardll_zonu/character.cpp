
/*=====================================================================================

	キャラクター定義

	Goluah!! Copyright (C) 2001-2004 aki, 2014-2015 logger, 2004-2015 At-sushi

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

=======================================================================================*/
#include "character.h"

//**************************************************************************************
//  Option Settings
//　ここに、キャラクターに設定できるオプションを指定します
//**************************************************************************************

CHARACTER_LOAD_OPTION option[] = {
	// キャラクターのオプションをここに書いてください。（最大32個）
	// なお、オプションとオプションの間はコンマ(,)で区切ってください。

	// 記入内容：
	// 　{ フラグ, 競合するｵﾌﾟｼｮﾝ,依存するｵﾌﾟｼｮﾝ, オプション名, 消費ポイント }
	{ OPTIONS_CHAIN_COMBO	,0					, 0, "Chain Combo"	, 8 } ,
	{ OPTIONS_SUPER_ARMER	,0					, 0, "Super Armor"	, 10} ,
	{ OPTIONS_HP_UP			,0					, 0, "HP+20%"		, 8 } ,
};


//**************************************************************************************
//  Character Information
//　ここにキャラクターの名前を記述します
//**************************************************************************************

CCharacterInfo CharInfo("ぞぬ",				// キャラクターの名前（最大31バイト）
						CDI_VERSION,		// キャラクターDLLインターフェイスのバージョン
						option,				// オプション構造体へのポインタ
						sizeof(option) / sizeof(CHARACTER_LOAD_OPTION), // オプションの数
						20,					// オプション最大ポイント数
						FALSE);				// ネット対戦対応かどうか 将来の拡張のために用意されている。現在は0を渡さなければならない（笑）

char* CharDirectory = NULL;

//設定マクロ。CCharacterInfo変数名↑と、キャラクタークラス名を指定します
SET_CHARACTERINFO(CharInfo, CCharacter)


/*--------------------------------------------------------------------------------------
	キャラクター生成処
	通常、いじる必要ありません
----------------------------------------------------------------------------------------*/

CCharacter::CCharacter(CDI_CHARACTERINFO2 *info) : CCharacterBase(info)
{
	CharDirectory = info->dir;	// キャラクターのフォルダを所得
}

/*--------------------------------------------------------------------------------------
	キャラクター破棄処理
	通常、いじる必要ありません
----------------------------------------------------------------------------------------*/
CCharacter::~CCharacter()
{
}

/*--------------------------------------------------------------------------------------
	パラメータ初期化
	ここに、各種パラメータの初期化処理を追加します
----------------------------------------------------------------------------------------*/
void CCharacter::InitParameters()
{
	chainComboEnabled	= (option_flags&OPTIONS_CHAIN_COMBO) ? TRUE : FALSE;
	opt_heavy_hit		= FALSE;
	opt_super_armer		= (option_flags&OPTIONS_SUPER_ARMER ) ? TRUE : FALSE;

	//最大体力値
	pdat->hpmax			= (option_flags&OPTIONS_HP_UP ) ? 1080 : 900 ;

	pdat->gaugemax		= 1;//最大ゲージ
	pdat->gaugetype		= GOBJFLGA_POWGAUGE2;//ゲージタイプ
}


/*--------------------------------------------------------------------------------------
	メッセージ処理
	ベースクラスで拾っていないメッセージを拾いたい場合は、ココに処理を追加します
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Message(DWORD msg,LPVOID dat,DWORD prm)
{
	GOBJECT *pobjdat;

	switch(msg){
	case GOBJMSG_NAGE1://投げ、敵側動作
		pobjdat = GetInfo(prm);
		if(pobjdat==NULL)return(0);
		return(act_nagee(pobjdat));
	case GOBJMSG_NAGE2://必投げ、敵側動作
		pobjdat = GetInfo(prm);
		if(pobjdat==NULL)return(0);
		return(act_hnagee(pobjdat));
	case GOBJMSG_NAGE3://超投げ、敵側動作
		pobjdat = GetInfo(prm);
		if(pobjdat==NULL)return(0);
		return(act_snagee(pobjdat));
	case GOBJMSG_STRIKER://支援攻撃要請
		if(pdat->aid == ACTID_TAIKICYU)
		{
			pdat->objtype &= ~GOBJFLG_TOBETARGET;//ターゲットにはならない
			ChangeAction( ACTID_STRIKER1 );
			return(YEAH);
		}
		return(0);
	default:
		return(CCharacterBase::Message(msg,dat,prm));//キャラクター基本クラスに処理を任せる
	}
	return(TRUE);
}


/*--------------------------------------------------------------------------------------
	コマンド判定 処理
	GOBJMSG_COMMAND メッセージに対応します。現在の行動によって複数の関数に
	処理が分かれています。適切なところに判定処理を追加してください。
----------------------------------------------------------------------------------------*/
BOOL CCharacter::Command_Hissatu(DWORD key_now)
{
	//必殺技コマンド判定
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_KUCYU){//空中必殺技のコマンド判定
	}
	else//地上必殺技のコマンド判定
	{
		if(com624624(60) && pdat->gauge>=1.0){
			if(key_now & KEYSTA_BC2){
				if(NageHantei(MAAI_SNAGE)){
					if(ObjCatch(pdat->eid,GOBJMSG_NAGE3)){pdat->aid = ACTID_SNAGE;return TRUE;}
				}
			}
		}
		if(com214(30)){
			if(key_now & KEYSTA_BC2){pdat->aid = ACTID_TETU3;return TRUE;}
			else if(key_now & KEYSTA_BB2){pdat->aid = ACTID_TETU2;return TRUE;}
			else if(key_now & KEYSTA_BA2){pdat->aid = ACTID_TETU1;return TRUE;}
		}
		if(com426(45)){
			if(key_now & KEYSTA_BC2){
				if(NageHantei(MAAI_HNAGE)){
					if(ObjCatch(pdat->eid,GOBJMSG_NAGE2)){pdat->aid = ACTID_HNAGE;return TRUE;}
				}
			}
		}
	}

	return CCharacterBase::Command_Hissatu(key_now);
}

BOOL CCharacter::Command_Normal(DWORD key)
{
	//ダッシュコマンド
	if(com66i(30)){pdat->aid=ACTID_DASHF;return TRUE;}
	if(com44i(30)){pdat->aid=ACTID_DASHB;return TRUE;}

	//return CCharacterBase::Command_Normal(key);

	//基本通常動作
	if(key & KEYSTA_UP){
		pdat->aid = ACTID_JAMPS;
	}
	else if(key & KEYSTA_DOWN){//しゃがみ
		pdat->aid = ACTID_CROUCH;
	}
	else if(key & KEYSTA_FOWORD){//前歩き
		if(key & KEYSTA_FOWORD && pdat->aid==ACTID_DASHF)return TRUE;//ダッシュ継続
		pdat->aid = ACTID_WALKF;
	}
	else if(key & KEYSTA_BACK){//後ろ歩き
		if(key & KEYSTA_BACK && pdat->aid==ACTID_DASHB)return TRUE;//ダッシュ継続
		pdat->aid = ACTID_WALKB;
	}
	else{//ニュートラルポーズ
		pdat->aid = ACTID_NEUTRAL;
	}
	return FALSE;
}

BOOL CCharacter::Command_OnNormal(DWORD keyinfo)
{
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_KUCYU){////ジャンプ動作中
	}
	else{//地上動作中
		//投げ
		if (keyinfo & KEYSTA_FOWORD && keyinfo & KEYSTA_BC2 &&
			NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE1)) {
				pdat->aid = ACTID_NAGE1;
				return TRUE;
		}
	}

	return CCharacterBase::Command_OnNormal(keyinfo);
}

/*--------------------------------------------------------------------------------------
	→＼↓／←→＼↓／←
----------------------------------------------------------------------------------------*/
BOOL CCharacter::com624624(int dt)
{
	int ofst=0;

	ofst=SeekKey(ofst,dt,KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=SeekKey(ofst,dt,KEYSTA_DOWN | KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=SeekKey(ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=SeekKey(ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=SeekKey(ofst,dt,KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=SeekKey(ofst,dt,KEYSTA_DOWN | KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=SeekKey(ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=SeekKey(ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

/*--------------------------------------------------------------------------------------
	コマンド判定 処理(COM)
----------------------------------------------------------------------------------------*/

DWORD CCharacter::CommandCOM(DWORD wid)
{
	return CCharacterBase::CommandCOM(wid);
}

DWORD CCharacter::CmdCom_OnSystem(DWORD wid)
{
	return CCharacterBase::CmdCom_OnSystem(wid);
}

DWORD CCharacter::CmdCom_OnHissatuAttacking(DWORD wid)
{
	return CCharacterBase::CmdCom_OnHissatuAttacking(wid);
}

DWORD CCharacter::CmdCom_OnNormal(DWORD wid)
{
	//投げ
	if (wid==ACTID_NAGE1)
	{
		if(!(pdat->aid&ACTID_KUCYU) && !(pdat->aid&ACTID_ATTACK))
		{
		
			if(NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE1)) {
				pdat->aid = ACTID_NAGE1;
				return TRUE;
			}
			else return FALSE;
		}
	}
	//必殺投げ
	if(wid==ACTID_HNAGE)
	{
		if(!(pdat->aid&ACTID_KUCYU) && !(pdat->aid&ACTID_ATTACK))
		{
			if(NageHantei(MAAI_HNAGE))
			if(ObjCatch(pdat->eid, GOBJMSG_NAGE2)) {
				pdat->aid = ACTID_HNAGE;
				return TRUE;
			}
		}
		return FALSE;
	}
	if(wid==ACTID_SNAGE)
	{
		if(!(pdat->aid&ACTID_KUCYU) && !(pdat->aid&ACTID_ATTACK) && pdat->gauge>=1.0)
		{
			if(NageHantei(MAAI_SNAGE))
			if(ObjCatch(pdat->eid, GOBJMSG_NAGE3)) {
				pdat->aid = ACTID_SNAGE;
				return TRUE;
			}
		}
		return FALSE;
	}

	return CCharacterBase::CmdCom_OnNormal(wid);
}

/*--------------------------------------------------------------------------------------
	GOBJMSG_TOUCHB(攻撃あたり)
----------------------------------------------------------------------------------------*/
DWORD CCharacter::TouchB(ATTACKINFO *info,BOOL hit)
{
	double dp = 0.0f;
	switch(info->id){
		case ATTACK_WEAK	:dp=0.02;break;
		case ATTACK_MIDDLE	:dp=0.03;break;
		case ATTACK_STRONG	:dp=0.04;break;
		case ATTACK_HISSATU	:dp=0.05;break;
	}
	if(!hit)dp*=0.5;
	AddPowerGauge(dp);


	if (IsLocalCom())
	{
		switch (pdat->aid)
		{
		case ACTID_ATT_SA:
		{
			if (ComLevelCk(3)){
				SetComAct(ACTID_ATT_SB, 5);
			}
		}
		break;
		case ACTID_ATT_SB:
		{
			if (ComLevelCk(2)){
				SetComAct(ACTID_ATT_SC, 5);
			}
		}
		break;
		case ACTID_ATT_CA:
		{
			if (ComLevelCk(3)){
				SetComAct(ACTID_ATT_CB, 5);
			}
		}
		break;
		case ACTID_ATT_CB:
		{
			if (ComLevelCk(3)){
				SetComAct(ACTID_ATT_CC, 5);
			}
		}
		break;
		case ACTID_ATT_SC:
		case ACTID_ATT_CC:
		{
			if (ComLevelCk(3))
			{
				SetComAct(ACTID_TETU2, 5);
			}
		}
		break;
		}
	}

	return CCharacterBase::TouchB(info, hit);
}

/*--------------------------------------------------------------------------------------
	GOBJMSG_TOUCHA(攻撃喰らい)
----------------------------------------------------------------------------------------*/
DWORD CCharacter::TouchA(ATTACKINFO *info,DWORD ta_eid)
{
	//スーパーアーマー
	BOOL sa = FALSE;
	if(opt_super_armer && (pdat->gauge>=NEEDGAUGE_SA))
	{
		switch(GetGObject()->aid){
		case ACTID_JAMPS	:sa=TRUE;						break;
		case ACTID_ATT_SA	:if(pdat->counter<2)sa=TRUE;	break;
		case ACTID_ATT_SB	:if(pdat->counter<15)sa=TRUE;	break;
		case ACTID_ATT_SC	:if(pdat->counter<25)sa=TRUE;	break;
		case ACTID_ATT_CA	:if(pdat->counter<3)sa=TRUE;	break;
		case ACTID_ATT_CB	:if(pdat->counter<15)sa=TRUE;	break;
		case ACTID_ATT_CC	:if(pdat->counter<15)sa=TRUE;	break;
		case ACTID_ATT_JA	:if(pdat->counter<2)sa=TRUE;	break;
		case ACTID_ATT_JB	:if(pdat->counter<7)sa=TRUE;	break;
		case ACTID_ATT_JC	:if(pdat->counter<10)sa=TRUE;	break;

		case ACTID_TETU1	:
		case ACTID_TETU2	:
		case ACTID_TETU3	:if(pdat->counter<12)sa=TRUE;	break;
		case ACTID_DASHF	:sa=TRUE;						break;
		default:return(0);
		}
	}
	isSuperArmer = sa;

	DWORD ret = CCharacterBase::TouchA(info,ta_eid);

	if(sa && (ret&0x10000000)){
		AddPowerGauge(-1.0f*NEEDGAUGE_SA);
	}

	isSuperArmer = FALSE;
	return ret;
}


/*--------------------------------------------------------------------------------------
	GOBJMSG_ACTION メッセージ 処理
	新しい行動を追加した場合は、ここに処理を追加します。
	行動の前後に共通処理を入れたい場合は PreAction(),PostAction() をオーバーライドしてください
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Action()
{
	switch(GetGObject()->aid){
	case ACTID_NAGE1		:act_nage();	break;
	case ACTID_TETU1		:act_tetu1();	break;
	case ACTID_TETU2		:act_tetu2();	break;
	case ACTID_TETU3		:act_tetu3();	break;
	case ACTID_HNAGE		:act_hnage();	break;
	case ACTID_SNAGE		:act_snage();	break;
	case ACTID_DASHF		:act_dashf();	break;
	case ACTID_DASHB		:act_dashb();	break;
	case ACTID_STRIKER1		:act_striker1();break;
	case ACTID_STRIKER2		:act_striker2();break;
	case ACTID_STRIKER3		:act_striker3();break;
	case ACTID_STRIKER4		:act_striker4();break;
	default://それ以外はキャラクター基本クラスに任せる
		return CCharacterBase::Action();
	}

	return(TRUE);
}

/*--------------------------------------------------------------------------------------
	攻撃力情報初期化
----------------------------------------------------------------------------------------*/
void CCharacter::InitAttackInfo()
{
	ZeroMemory(aif,sizeof(aif));

	int i;

	i=ATTACKINFO_STAND_A;//立ち弱攻撃
	aif[i].hit		=HITINFO_MARK1 | HITINFO_REACT1 | HITINFO_SNDHIT1 | HITINFO_SIV1 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_WEAK;//ゲージ増加時に使用するID
	aif[i].damage	=30;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_STAND_B;//立ち中攻撃
	aif[i].hit		=HITINFO_MARK2 | HITINFO_REACT2 | HITINFO_SNDHIT2 | HITINFO_SIV2 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=40;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_STAND_C;//立ち強攻撃
	aif[i].hit		=HITINFO_MARK3 | HITINFO_REACT1 | HITINFO_SNDHIT3 | HITINFO_SIV1;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=20;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_CROUCH_A;//しゃがみ弱攻撃
	aif[i].hit		=HITINFO_MARK1 | HITINFO_REACT1 | HITINFO_SNDHIT1 | HITINFO_SIV1;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_WEAK;//ゲージ増加時に使用するID
	aif[i].damage	=30;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_CROUCH_B;//しゃがみ中攻撃
	aif[i].hit		=HITINFO_MARK2 | HITINFO_REACT2 | HITINFO_SNDHIT2 | HITINFO_SIV2;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=45;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_CROUCH_C;//しゃがみ強攻撃
	aif[i].hit		=HITINFO_MARK3 | HITINFO_REACT3 | HITINFO_SNDHIT3 | HITINFO_SIV3;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP | GUARDINFO_XSTAND;//ガード情報
	aif[i].id		=ATTACK_STRONG ;//ゲージ増加時に使用するID
	aif[i].damage	=50;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_JAMP_A;//ジャンプ弱攻撃
	aif[i].hit		=HITINFO_MARK1 | HITINFO_REACT1 | HITINFO_SNDHIT1 | HITINFO_SIV1;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1 | GUARDINFO_XCROUCH;//ガード情報
	aif[i].id		=ATTACK_WEAK;//ゲージ増加時に使用するID
	aif[i].damage	=30;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_JAMP_B;//ジャンプ中攻撃
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2  | GUARDINFO_XCROUCH;//ガード情報
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=40;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_JAMP_C;//ジャンプ強攻撃
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=50;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_NAGE;//投げ
	aif[i].hit		=HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=80;//ダメージ

	i=ATTACKINFO_HNAGE1;//必殺投げ(1)
	aif[i].hit		=HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
	aif[i].damage	=60;//ダメージ

	i=ATTACKINFO_HNAGE2;//必殺投げ(2)
	aif[i].hit		=HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
	aif[i].damage	=80;//ダメージ

	//体当たり

	i=ATTACKINFO_CHARGE1;
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
	aif[i].damage	=100;//ダメージ
	aif[i].kezuri	=10;//削り
	
	i=ATTACKINFO_CHARGE2;
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
	aif[i].damage	=120;//ダメージ
	aif[i].kezuri	=12;//削り
	
	i=ATTACKINFO_CHARGE3;
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
	aif[i].damage	=140;//ダメージ
	aif[i].kezuri	=14;//削り

	//超投げ
	
	i=ATTACKINFO_SNAGE1;//超投げ(1)
	aif[i].hit		=HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].id		=0;//ゲージ増加時に使用するID
	aif[i].damage	=60;//ダメージ

	i=ATTACKINFO_SNAGE2;//超投げ(2)
	aif[i].hit		=HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].id		=0;//ゲージ増加時に使用するID
	aif[i].damage	=240;//ダメージ

	if(opt_heavy_hit)
	{
		for(int i=0;i<AIF_NUM;i++)
		{
			aif[i].damage = (DWORD)(aif[i].damage*1.2);
			aif[i].kezuri*= 2;
		}
	}
}

/*--------------------------------------------------------------------------------------
	攻撃開始処理（通常、書き換える必要はありません）
----------------------------------------------------------------------------------------*/
void CCharacter::StartAttack(DWORD atk_info_id)
{
	GetGObject()->kougeki = TRUE;
	GetGObject()->atk = &aif[atk_info_id];
}

/*--------------------------------------------------------------------------------------
	COM行動用 技情報 初期化
----------------------------------------------------------------------------------------*/
void CCharacter::InitWazInfo()//コンピュータ用技情報の設定
{
	CCharacterBase::InitWazInfo();

	waz.walkf = ACTID_WALKF;
	waz.walkb = ACTID_WALKB;
	waz.jampf = ACTID_JAMPF;
	waz.jampb = ACTID_JAMPB;

	waz.dashf = ACTID_DASHF;
	waz.dashb = ACTID_DASHB;

	waz.att_short[0] = ACTID_ATT_SA;//短リーチ攻撃
	waz.att_short[1] = ACTID_ATT_CA;
	waz.att_short[2] = ACTID_ATT_CB;
	waz.att_short[3] = ACTID_TETU1;
	waz.att_middle[0] = ACTID_ATT_SB;//中リーチ攻撃
	waz.att_middle[1] = ACTID_ATT_CC;
	waz.att_middle[2] = ACTID_TETU3;
	waz.att_long[0] = ACTID_ATT_SC;//長リーチ攻撃
	waz.att_jamp[0] = ACTID_ATT_JC;//ジャンプ攻撃
	waz.att_jamp[1] = ACTID_ATT_JB;
	waz.att_jamp[2] = ACTID_ATT_JA;
	waz.att_tai[0] = ACTID_ATT_CC;
	waz.nageid[0] = ACTID_NAGE1;//投げ
	waz.nageid[1] = ACTID_HNAGE;
	waz.nageid[2] = ACTID_SNAGE;
}
