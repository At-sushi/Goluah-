
/*=====================================================================================

	キャラクター定義

	Goluah!! Copyright (C) 2001-2004 aki, 2014-2015 logger, 2004-2015 At-sushi

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

=======================================================================================*/
#include "character.h"


/*--------------------------------------------------------------------------------------
	キャラクター生成処
	通常、いじる必要ありません
----------------------------------------------------------------------------------------*/

CCharacter::CCharacter(CDI_CHARACTERINFO2 *info) : CCharacterBase(info)
{
	CharDirectory = info->dir;	// キャラクターのフォルダを所得

	//aki用エフェクト初期化
	aki3d.Initialize( 
					CGoluahObject::GetD3DDevice() ,
					CharDirectory
					);
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
	InitAnalyzeOptions();

	pdat->hpmax			= 1000;//最大体力値
	pdat->gaugemax		= m_opt_StockableGauge ? 3 : 1;//最大ゲージ
	pdat->gaugetype		= m_opt_StockableGauge ? GOBJFLGA_POWGAUGE1 : GOBJFLGA_POWGAUGE2 ;//ゲージタイプ
}

//DWORD オプション指定を BOOLメンバ変数に分割する
void CCharacter::InitAnalyzeOptions()
{
	chainComboEnabled	 = TRUE ;
	isAutoGuard			 = (option_flags&OPTIONS_AUTO_GUARD) ? TRUE : FALSE ;
	m_opt_ExtraAttack	 = (option_flags&OPTIONS_EXTRA_ATTACK) ? TRUE : FALSE ;
	m_opt_GuardCancel	 = TRUE ;
	m_opt_GCutterPlus	 = (option_flags&OPTIONS_G_CUTTER_PLUS) ? TRUE : FALSE ;
	m_opt_ChibasiriPlus	 = (option_flags&OPTIONS_CHIBASIRI_PLUS) ? TRUE : FALSE ;
	m_opt_2ndStep		 = (option_flags&OPTIONS_2ND_STEP) ? TRUE : FALSE ;
	m_opt_StockableGauge = (option_flags&OPTIONS_STOCKABLE_GAUGE) ? TRUE : FALSE ;
	m_opt_minigunReloaded= (option_flags&OPTIONS_MINIGUN_RELOADED) ? TRUE : FALSE ;
}

/*--------------------------------------------------------------------------------------
	メッセージ処理
	ベースクラスで拾っていないメッセージを拾いたい場合は、ココに処理を追加します
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Message(DWORD msg,LPVOID dat,DWORD prm)
{
	GOBJECT* pobjdat;

	switch(msg){
	//飛道具ヒット・ガード
	case GOBJMSG_CHIBAHIT:		AddPowerGauge(0.10);break;
	case GOBJMSG_CHIBAGUARD:	AddPowerGauge(0.05);break;
	case GOBJMSG_NAGE://投げ、敵側動作
		{
			pobjdat = GetInfo(prm);
			if(pobjdat==NULL)return(0);
			return(eact_nage(pobjdat));
		}
		break;
	case GOBJMSG_GCUTTERUP:
		{
			pobjdat = GetInfo(prm);
			if(pobjdat==NULL)return(0);
			dact_futtobi2(pobjdat);
			if(pobjdat->vy>0)pobjdat->muteki = TRUE;
		}
		break;
	case GOBJMSG_STRIKER://支援攻撃要請
		if(pdat->aid == ACTID_TAIKICYU){
			RemoveProperty( GOBJFLG_TOBETARGET );//ターゲットにはならない...なんでこれ外すんだっけ？(忘)
			ChangeAction( ACTID_STRIKER_IN );
			return(WAIT_A_SEC);
		}
		return(0);
	default:
		return(CCharacterBase::Message(msg,dat,prm));//キャラクター基本クラスに処理を任せる
	}
	return(TRUE);
}


/*--------------------------------------------------------------------------------------
	GOBJMSG_TOUCHB
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
	if(!hit)dp*=0.2;
	AddPowerGauge(dp);

	if(IsLocalCom())
	{
		switch(pdat->aid)
		{
		case ACTID_ATT_SA:
			{
				if(ComLevelCk(3)){
					SetComAct(ACTID_ATT_SB,5);
				}
			}
			break;
		case ACTID_ATT_SB:
			{
				if(ComLevelCk(2)){
					SetComAct(ACTID_ATT_SB2,6);
				}
			}
			break;
		case ACTID_ATT_SC:
			{
				if(ComLevelCk(5) && pdat->gauge>=1.0f)
				{
					SetComAct(ACTID_MINIGUN,5);
				}
				else if(ComLevelCk(3)){
					if(ChainCheck(CHAIN_SC3)){
						SetComAct(ACTID_ATT_SC3,12);
					}
				}
			}
			break;

		case ACTID_ATT_SB2:
			{
				if(ComLevelCk(3)){
					if(ChainCheck(CHAIN_SC3)){
						SetComAct(ACTID_ATT_SC3,18);
					}
					else if(ChainCheck(CHAIN_CB)){
						SetComAct(ACTID_ATT_CB,18);
					}
				}
			}
			break;
		case ACTID_ATT_SC2:
			{
				if(ComLevelCk(3)){
					SetComAct(ACTID_ATT_SB,5);
				}
			}
			break;
		case ACTID_ATT_SC3:
			{
				if(ComLevelCk(5) && pdat->gauge>=1.0f){
					SetComAct(ACTID_MINIGUN,5);
				}
				else if(ComLevelCk(5) && ChainCheck(CHAIN_CB)){
					SetComAct(ACTID_ATT_CB,8);
				}
				else if(ComLevelCk(3) && ChainCheck(CHAIN_CC)){
					SetComAct(ACTID_ATT_CC,8);
				}
			}
			break;

		case ACTID_ATT_CA:
			{
			}
			break;
		case ACTID_ATT_CB:
			{
				if(ComLevelCk(3) && ChainCheck(CHAIN_CC)){
					SetComAct(ACTID_ATT_CC,6);
				}
				else if(ComLevelCk(3) && ChainCheck(CHAIN_SC3)){
					SetComAct(ACTID_ATT_SC3,6);
				}
			}
			break;
		case ACTID_ATT_CC:
			{
				if(ComLevelCk(4)){
					SetComAct(ACTID_GENOCIDE3G,8);
				}
			}
			break;

		case ACTID_GENOCIDE1J:
		case ACTID_GENOCIDE2J:
		case ACTID_GENOCIDE3J:
			{
				if(m_opt_GCutterPlus)
				{
					if(ComLevelCk(3))
					{
						if(rand()%10<7){
							SetComAct(ACTID_GENOCIDE_UP,20);
						}
						else{
							SetComAct(ACTID_GENOCIDE_DOWN,20);
						}
					}
				}
			}
			break;
		}
	}

	return CCharacterBase::TouchB(info,hit);
}

/*--------------------------------------------------------------------------------------
	GOBJMSG_TOUCHC メッセージ 処理
----------------------------------------------------------------------------------------*/
DWORD CCharacter::TouchC(ATTACKINFO *info,DWORD tc_eid)
{
	//屈強AQ防止
	if(info==&aif[ATTACKINFO_CROUCH_C])
	{
		GOBJECT *pedat = GetInfo(tc_eid);
		if(pedat)
		{
			if(pedat->aid==ACTID_FUTTOBI2)
			{
				return TOUCHC_CANCEL;
			}
		}
	}

	return CCharacterBase::TouchC(info,tc_eid);
}

/*--------------------------------------------------------------------------------------
	コマンド判定 処理
	GOBJMSG_COMMAND メッセージに対応します。現在の行動によって複数の関数に
	処理が分かれています。適切なところに判定処理を追加してください。
----------------------------------------------------------------------------------------*/
BOOL CCharacter::Command_Hissatu(DWORD key)
{
	//必殺技コマンド判定
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_KUCYU){//空中必殺技のコマンド判定
	}
	else//地上必殺技のコマンド判定
	{
		//ミニガン
		if(com426(45) && (key & KEYSTA_BC2))
		{
			if(pdat->gauge >= 1.0f){
				pdat->aid = ACTID_MINIGUN;
				return TRUE;
			}
		}

		//G・カッター
		if(com623(30)){
			if(key & KEYSTA_BC2){pdat->aid = ACTID_GENOCIDE3G;return TRUE;}
			else if(key & KEYSTA_BB2){pdat->aid = ACTID_GENOCIDE2G;return TRUE;}
			else if(key & KEYSTA_BA2){pdat->aid = ACTID_GENOCIDE1G;return TRUE;}
		}

		//地走り
		if(com236(30)){
			if((key & KEYSTA_BC2) && bul_chiba->isReady()){
				pdat->aid = ACTID_CHIBA3;return TRUE;
			}
			else if((key & KEYSTA_BB2) && bul_chiba->isReady()){
				pdat->aid = ACTID_CHIBA2;return TRUE;
			}
			else if((key & KEYSTA_BA2) && bul_chiba->isReady()){
				pdat->aid = ACTID_CHIBA1;return TRUE;
			}
		}
	}

	return CCharacterBase::Command_Hissatu(key);
}

BOOL CCharacter::Command_Normal(DWORD key)
{
	//ダッシュコマンド
	if(com66i(30) && (key&KEYSTA_FOWORD2)){pdat->aid=ACTID_DASHF;return TRUE;}
	if(com44i(30) && (key&KEYSTA_BACK2)  ){pdat->aid=ACTID_DASHB;return TRUE;}

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
		//if(key & KEYSTA_BACK && pdat->aid==ACTID_DASHB)return TRUE;//ダッシュ継続
		pdat->aid = ACTID_WALKB;
	}
	else{//ニュートラルポーズ
		pdat->aid = ACTID_NEUTRAL;
	}
	return FALSE;
}

BOOL CCharacter::Command_OnNormal(DWORD key)
{
	if(pdat->aid & ACTID_KUCYU)////ジャンプ動作中
	{
		//2段ジャンプ
		if((key & KEYSTA_UP2) && m_2nd_step_flag)
		{
			pdat->aid = ACTID_2ND_STEP;
			return TRUE;
		}
		//2段ジャンプ分岐
		if(pdat->aid == ACTID_2ND_STEP){
			if(key & KEYSTA_FOWORD){//前ジャンプ
				pdat->aid = ACTID_JAMPF;
				return TRUE;
			}
			else if(key & KEYSTA_BACK){//後ジャンプ
				pdat->aid = ACTID_JAMPB;
				return TRUE;
			}
		}
	}
	else{//地上動作中
		//投げ
		if((key & KEYSTA_FOWORD) && (key & KEYSTA_BC2) &&
			NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE))
		{
				pdat->aid = ACTID_NAGE1;
				return TRUE;
		}

		//レバー前入れ強
		if((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD) && m_opt_ExtraAttack){
			ChangeAction(  ACTID_ATT_SC3 );return TRUE;
		}
	}

	return CCharacterBase::Command_OnNormal(key);
}

BOOL CCharacter::Command_OnAttacking(DWORD key)
{
	if(m_opt_ExtraAttack)
	{
		if(pdat->aid == ACTID_ATT_SB){
			if((key & KEYSTA_BB2) && !(key & KEYSTA_DOWN)){
				ChangeAction(  ACTID_ATT_SB2 );return TRUE;
			}
		}
		
		if(!(pdat->aid & ACTID_KUCYU))
		{
			if((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD)){
				if(ChainCombo(CHAIN_SC3)){
					ChangeAction(  ACTID_ATT_SC3 );return TRUE;
				}
			}
		}

		if(pdat->aid == ACTID_ATT_SC){
			if((key & KEYSTA_BC2) && !(key & KEYSTA_DOWN)){
				ChangeAction(  ACTID_ATT_SC2 );return TRUE;
			}
		}
	}

	return CCharacterBase::Command_OnAttacking(key);
}

//喰らい・ガード中
BOOL CCharacter::Command_OnSystem(DWORD key)
{
	if(pdat->aid & ACTID_GUARD){
		if(!(pdat->aid & ACTID_KUCYU) && m_opt_GuardCancel)
		{
			//地上ガード中
			//ガードキャンセル G・カッター
			if(com623(30) && pdat->gauge>0.3){
				if(key & KEYSTA_BC2){pdat->aid = ACTID_GCANCEL3;return TRUE;}
				else if(key & KEYSTA_BB2){pdat->aid = ACTID_GCANCEL2;return TRUE;}
				else if(key & KEYSTA_BA2){pdat->aid = ACTID_GCANCEL1;return TRUE;}
			}
		}
	}

	return CCharacterBase::Command_OnSystem(key);
}

//必殺技中
BOOL CCharacter::Command_OnHissatuAttacking(DWORD key)
{
	if(m_opt_GCutterPlus){
		if(pdat->aid==ACTID_GENOCIDE1J || pdat->aid==ACTID_GENOCIDE2J || pdat->aid==ACTID_GENOCIDE3J )
		{
			if((key & (KEYSTA_BC2|KEYSTA_BB2|KEYSTA_BA2)) && (pdat->counter>6) )
			{
				if(key&KEYSTA_DOWN){
					ChangeAction(ACTID_GENOCIDE_DOWN);
					return TRUE;
				}
				else if(key&KEYSTA_UP){
					ChangeAction(ACTID_GENOCIDE_UP);
					return TRUE;
				}
			}
		}
	}
	//ミニガン - リローデッド
	if(pdat->aid==ACTID_MINIGUN && m_opt_minigunReloaded){
		if(com426(45) && (key & KEYSTA_BC2))
		{
			if(pdat->gauge >= 1.0f){
				pdat->counter = 0;
				return TRUE;
			}
		}
	}

	return CCharacterBase::Command_OnHissatuAttacking(key);
}

/*--------------------------------------------------------------------------------------
	コンピュータコマンド
----------------------------------------------------------------------------------------*/

DWORD CCharacter::CommandCOM(DWORD wid)
{
	if(wid==ACTID_MINIGUN && pdat->gauge<1.0f)return FALSE;//ゲージチェック

	if(wid==ACTID_MINIGUN && pdat->aid==ACTID_MINIGUN && m_opt_minigunReloaded)
	{
		return TRUE;
	}
	if(wid==ACTID_CHIBA1 || wid==ACTID_CHIBA2 || wid==ACTID_CHIBA3)
	{
		if(!bul_chiba->isReady())return FALSE;
	}

	return CCharacterBase::CommandCOM(wid);
}


DWORD CCharacter::CmdCom_OnNormal(DWORD wid)
{
	//投げ
	if(!(pdat->aid&ACTID_KUCYU) && !(pdat->aid&ACTID_ATTACK))
	{
		if (wid==ACTID_NAGE1)
		{
			if(NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE)) {
				pdat->aid = ACTID_NAGE1;
				return TRUE;
			}
			else return FALSE;
		}
	}

	if(!(pdat->aid&ACTID_KUCYU) && (m_opt_ExtraAttack))
	{
		//特殊入力系
		if(wid==ACTID_ATT_SB2)
		{
			return (pdat->aid==ACTID_ATT_SB) ? TRUE : FALSE;
		}
		if(wid==ACTID_ATT_SC2)
		{
			return (pdat->aid==ACTID_ATT_SC) ? TRUE : FALSE;
		}

		//チェーンコンボ判定(追加)
		if(GetGObject()->aid & ACTID_ATTACK)
		{
			switch(wid)
			{
			case ACTID_ATT_SC3	:return ChainCombo(CHAIN_SC3);break;
			}
		}
	}

	return CCharacterBase::CmdCom_OnNormal(wid);
}

/*--------------------------------------------------------------------------------------
	GOBJMSG_ACTION メッセージ 処理
	新しい行動を追加した場合は、ここに処理を追加します。
	行動の前後に共通処理を入れたい場合は PreAction(),PostAction() をオーバーライドしてください
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Action()
{
	switch(GetGObject()->aid){
	case ACTID_ATT_SB2		:act_att_sb2();		break;
	case ACTID_ATT_SC2		:act_att_sc2();		break;
	case ACTID_ATT_SC3		:act_att_sc3();		ChainCombo(CHAIN_SC3);break;
	case ACTID_LANDING		:act_landing();		break;
	case ACTID_GENOCIDE1G	:act_genocide1g();	break;
	case ACTID_GENOCIDE1J	:act_genocide1j();	break;
	case ACTID_GENOCIDE2G	:act_genocide2g();	break;
	case ACTID_GENOCIDE2J	:act_genocide2j();	break;
	case ACTID_GENOCIDE3G	:act_genocide3g();	break;
	case ACTID_GENOCIDE3J	:act_genocide3j();	break;
	case ACTID_LANDINGG		:act_landingg();	break;
	case ACTID_DASHF		:act_dashf();		break;
	case ACTID_DASHB		:act_dashb();		break;
	case ACTID_CHIBA1		:act_chiba1();		break;
	case ACTID_CHIBA2		:act_chiba2();		break;
	case ACTID_CHIBA3		:act_chiba3();		break;
	case ACTID_MINIGUN		:act_minigun();		break;
	case ACTID_MINIGUN2		:act_minigun2();	break;
	case ACTID_GCANCEL1		:act_gcancel1();	break;
	case ACTID_GCANCEL2		:act_gcancel2();	break;
	case ACTID_GCANCEL3		:act_gcancel3();	break;
	case ACTID_2ND_STEP		:act_2nd_step();	break;
	case ACTID_GENOCIDE_UP	:act_genocide_up();	break;
	case ACTID_GENOCIDE_DOWN:act_genocide_down();break;
	case ACTID_NAGE1		:act_nage();		break;
	case ACTID_STRIKER_IN	:act_striker_in();	break;
	case ACTID_STRIKER_ATK1	:act_striker_atk1();break;
	case ACTID_STRIKER_ATK2	:act_striker_atk2();break;
	case ACTID_STRIKER_OUT	:act_striker_out();	break;
	default:return CCharacterBase::Action();
	}
	return(TRUE);
}

void CCharacter::PreAction()
{
	if(!(pdat->aid&ACTID_KUCYU)){//2段ジャンプフラグクリア
		m_2nd_step_flag = m_opt_2ndStep;
	}

	if( pdat->aid==ACTID_MINIGUN && m_opt_minigunReloaded && pdat->counter>220 && pdat->counter<225)
	{
		if(ComLevelCk(5) && rand()%2)
		{
			SetComAct(ACTID_MINIGUN,5);
		}
	}

	CCharacterBase::PreAction();
}

/*--------------------------------------------------------------------------------------
	攻撃力情報初期化
----------------------------------------------------------------------------------------*/
void CCharacter::InitAttackInfo()
{
	ZeroMemory(aif,sizeof(aif));
	int i;

	//・通常攻撃

	i=ATTACKINFO_STAND_A;//立ち弱攻撃
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1  | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_WEAK;//ゲージ増加時に使用するID
	aif[i].damage	=30;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_STAND_B;//立ち中攻撃
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2  | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=40;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_STAND_C;//立ち強攻撃
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=45;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_CROUCH_A;//しゃがみ弱攻撃
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1  | GUARDINFO_XJAMP | GUARDINFO_XSTAND;//ガード情報
	aif[i].id		=ATTACK_WEAK;//ゲージ増加時に使用するID
	aif[i].damage	=30;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_CROUCH_B;//しゃがみ中攻撃
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2  | GUARDINFO_XJAMP | GUARDINFO_XSTAND;//ガード情報
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=40;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_CROUCH_C;//しゃがみ強攻撃
	aif[i].hit		=HITINFO_FUTTOBI2 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=55;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_JAMP_A;//ジャンプ弱攻撃
	aif[i].hit		=HITINFO_REACT1 | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1  | GUARDINFO_XCROUCH;//ガード情報
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
	aif[i].damage	=55;//ダメージ
	aif[i].kezuri	=0;//削り

	//・追加入力技

	i=ATTACKINFO_STAND_B2;//立ち中、追加入力技
	aif[i].hit		=HITINFO_REACT3A | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV2 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=30;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_STAND_C2;//立ち強、追加入力技
	aif[i].hit		=HITINFO_TATAKI2B | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH ;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=60;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_STAND_C3;//立ち強、レバー前入れ
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=50;//ダメージ
	aif[i].kezuri	=0;//削り

	//・ジェノサイド・アレ

	i=ATTACKINFO_GENOCIDE1;
	aif[i].hit		=HITINFO_FUTTOBI | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
	aif[i].damage	=65;//ダメージ
	aif[i].kezuri	=5;//削り
	
	i=ATTACKINFO_GENOCIDE2;
	aif[i].hit		=HITINFO_FUTTOBI | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 ;//ガード情報
	aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
	aif[i].damage	=70;//ダメージ
	aif[i].kezuri	=5;//削り
	
	i=ATTACKINFO_GENOCIDE3;
	aif[i].hit		=HITINFO_FUTTOBI | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 ;//ガード情報
	aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
	aif[i].damage	=75;//ダメージ
	aif[i].kezuri	=5;//削り

	//・地走り（各飛道具クラスにコピーされる）

	i=ATTACKINFO_CHIBA1;
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 | HITINFO_EFCTBURN;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
	aif[i].damage	=50;//ダメージ
	aif[i].kezuri	=5;//削り

	i=ATTACKINFO_CHIBA2;
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3  | HITINFO_EFCTBURN;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
	aif[i].damage	=60;//ダメージ
	aif[i].kezuri	=5;//削り
	
	i=ATTACKINFO_CHIBA3;
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3  | HITINFO_EFCTBURN;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
	aif[i].damage	=70;//ダメージ
	aif[i].kezuri	=5;//削り

	//・ミニガン
	i=ATTACKINFO_MINIGUN;
	aif[i].hit		=HITINFO_REACT3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=0;//ゲージ増加時に使用するID
	aif[i].damage	=15;//ダメージ
	aif[i].kezuri	=1;//削り

	//・ジェノサイド、追加入力

	i=ATTACKINFO_GENOCIDE_UP;
	aif[i].hit		=HITINFO_USER(USERHIT_GCUTTERUP) | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH ;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=50;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_GENOCIDE_DOWN;
	aif[i].hit		=HITINFO_TATAKI1B | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH ;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=100;//ダメージ
	aif[i].kezuri	=5;//削り

	//・投げ
	i = ATTACKINFO_NAGE;
	aif[i].id		= 0;
	aif[i].damage	= 80;
	aif[i].hit		= HITINFO_MARK3 | HITINFO_SNDHIT3;

	//・支援攻撃

	i=ATTACKINFO_STRIKER1;
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=0;//ゲージ増加時に使用するID
	aif[i].damage	=30;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_STRIKER2;
	aif[i].hit		=HITINFO_REACT3A | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=0;//ゲージ増加時に使用するID
	aif[i].damage	=30;//ダメージ
	aif[i].kezuri	=0;//削り
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
	//歩きやジャンプなどの行動
	CCharacterBase::InitWazInfo();

	waz.dashf = ACTID_DASHF;
	waz.dashb = ACTID_DASHB;

	//pdat->winfo へ行動IDを追加する
	waz.att_short[0] = ACTID_ATT_SA;
	waz.att_short[1] = ACTID_ATT_CA;
	waz.att_short[2] = ACTID_ATT_SC;
	waz.att_short[3] = ACTID_NAGE1;
	waz.att_middle[0] = ACTID_ATT_SB;
	waz.att_middle[1] = ACTID_ATT_SB2;
	waz.att_middle[2] = ACTID_ATT_SC2;
	waz.att_middle[3] = ACTID_ATT_SC3;
	waz.att_long[0] = ACTID_ATT_SB2;
	waz.att_long[1] = ACTID_ATT_CB;
	waz.att_long[2] = ACTID_ATT_CC;
	waz.att_long[3] = ACTID_ATT_SC2;
	waz.att_long[4] = ACTID_ATT_SC3;
	waz.att_long[5] = ACTID_GENOCIDE2G;
	waz.att_tai[0] = ACTID_GENOCIDE1G;
	waz.att_tai[1] = ACTID_ATT_SC;
	waz.att_bullet[0] = ACTID_CHIBA1;
	waz.att_bullet[1] = ACTID_CHIBA2;
	waz.att_bullet[2] = ACTID_CHIBA3;

	//リーチ設定
	SetComReach( 0,MAAI_SHORT	);
	SetComReach( 1,MAAI_MIDDLE );
	SetComReach( 2,MAAI_LONG );
}

/*--------------------------------------------------------------------------------------
	COMレベル判定
----------------------------------------------------------------------------------------*/
BOOL CCharacter::ComLevelCk(UINT level)
{
	int crnt_level = GetComLevel();
	int v = rand()%100;
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
