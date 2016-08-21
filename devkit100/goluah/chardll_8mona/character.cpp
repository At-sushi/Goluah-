/*=====================================================================================

	キャラクター定義

	Goluah!! Copyright (C) 2001-2004 aki, 2004-2016 At-sushi, 2014-2016 logger

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

	// 　{ フラグ, 競合するｵﾌﾟｼｮﾝ,依存するｵﾌﾟｼｮﾝ, オプション名, 消費ポイント }
	{ OPTIONS_EXTRA_ATTACK,		0, 0,	"Extra Attacks"	, 6 } ,
	{ OPTIONS_EXTRA_FRIENDS,	0, 0,	"Extra Friends"	, 6 } ,
};


//**************************************************************************************
//  Character Information
//　ここにキャラクターの名前を記述します
//**************************************************************************************

CCharacterInfo CharInfo("8頭身モナー",		// キャラクターの名前（最大31バイト）
						CDI_VERSION,		// キャラクターDLLインターフェイスのバージョン
						option,				// オプション構造体へのポインタ
						sizeof(option) / sizeof(CHARACTER_LOAD_OPTION), // オプションの数
						6,					// オプション最大ポイント数
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
	opt_gcancel		= TRUE;
	opt_exattack	= (option_flags&OPTIONS_EXTRA_ATTACK) ? TRUE : FALSE;
	opt_ikari		= TRUE;
	opt_super_armer	= FALSE;
	opt_exfriend	= (option_flags&OPTIONS_EXTRA_FRIENDS) ? TRUE : FALSE;

	pdat->hpmax			= 850;					//最大体力値
	pdat->gaugemax		= 1;					//最大ゲージ
	pdat->gaugetype		= GOBJFLGA_POWGAUGE2;	//ゲージタイプ
}


/*--------------------------------------------------------------------------------------
	メッセージ処理
	ベースクラスで拾っていないメッセージを拾いたい場合は、ココに処理を追加します
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Message(DWORD msg,LPVOID dat,DWORD prm)
{
	GOBJECT* pobjdat;

	switch(msg){
	case GOBJMSG_NAGE://投げ、敵側動作
		{
			pobjdat = GetInfo(prm);
			if(pobjdat==NULL)return(0);
			return(act_nagee(pobjdat));
		}
		break;
	case GOBJMSG_IKARINAGE:
		{
			pobjdat = GetInfo(prm);
			if(pobjdat==NULL)return(0);
			return(act_ikarie(pobjdat));
		}
		break;
	case GOBJMSG_STRIKER://支援攻撃要請
		if(pdat->aid == ACTID_TAIKICYU){
			pdat->objtype &= ~GOBJFLG_TOBETARGET;//ターゲットにはならない
			ChangeAction( ACTID_STRIKER1);
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

//喰らい・ガード中
BOOL CCharacter::Command_OnSystem(DWORD key)
{
	if((pdat->aid & ACTID_GUARD) && opt_gcancel){
		if(!(pdat->aid & ACTID_KUCYU))
		{
			//地上ガード中
			//ガードキャンセル
			if(pdat->gauge>NEEDGAUGE_GC){
				if(key & KEYSTA_BC2){pdat->aid = ACTID_GCANCEL3;return TRUE;}
				else if(key & KEYSTA_BB2){pdat->aid = ACTID_GCANCEL2;return TRUE;}
				else if(key & KEYSTA_BA2){pdat->aid = ACTID_GCANCEL1;return TRUE;}
			}
		}
	}

	return CCharacterBase::Command_OnSystem(key);
}

BOOL CCharacter::Command_Hissatu(DWORD key)
{
	//必殺技コマンド判定
	if(pdat->aid & ACTID_KUCYU){
	}
	else{
		if( (pdat->gauge>=1.0) && com62426(60) && (key & KEYSTA_BC2) && EscapeOK()){
			pdat->aid = ACTID_ESCAPE;
			return TRUE;
		}
		if(com426(45)){
			if(key & KEYSTA_BC2){
				if(NageHantei(MAAI_HNAGE) && opt_ikari){
					if(ObjCatch(pdat->eid,GOBJMSG_IKARINAGE)){
						pdat->aid = ACTID_IKARI;return TRUE;
					}
				}
			}
		}
		if(com623(30)){
			if(key & KEYSTA_BC2){pdat->aid = ACTID_SAH3;return TRUE;}
			else if(key & KEYSTA_BB2){pdat->aid = ACTID_SAH2;return TRUE;}
			else if(key & KEYSTA_BA2){pdat->aid = ACTID_SAH1;return TRUE;}
		}
		if(com421(30)){
			if(key & KEYSTA_BC2){pdat->aid = ACTID_FIG3;return TRUE;}
			else if(key & KEYSTA_BB2){pdat->aid = ACTID_FIG2;return TRUE;}
			else if(key & KEYSTA_BA2){pdat->aid = ACTID_FIG1;return TRUE;}
		}
	}

	return CCharacterBase::Command_Hissatu(key);
}

BOOL CCharacter::Command_Normal(DWORD key)
{
	//ダッシュ
	if(com66i(30)){pdat->aid=ACTID_DASHF;return TRUE;}
	if(com44i(30)){pdat->aid=ACTID_DASHB;return TRUE;}


	//基本通常動作
	if(key & KEYSTA_UP){
		pdat->aid = ACTID_JAMPS;
		return TRUE;
	}
	if(pdat->aid==ACTID_DASHF || pdat->aid==ACTID_DASHB){
		return TRUE;
	}

	if(key & KEYSTA_DOWN){//しゃがみ
		pdat->aid = ACTID_CROUCH;
	}
	else if(key & KEYSTA_FOWORD){//前歩き
		pdat->aid = ACTID_WALKF;
	}
	else if(key & KEYSTA_BACK){//後ろ歩き
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
			NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE)) {
				pdat->aid = ACTID_NAGE1;
				return TRUE;
		}

		//追加技
		if(opt_exattack)
		{
			DWORD key = keyinfo;

			// ＼+C
			if((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD) && (key & KEYSTA_DOWN)){
				ChangeAction(  ACTID_HKICK );return TRUE;
			}

			// →+C
			else if((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD)){
				ChangeAction(  ACTID_HAND );return TRUE;
			}

			// ＼+B
			else if((key & KEYSTA_BB2) && (key & KEYSTA_FOWORD) && (key & KEYSTA_DOWN)){
				ChangeAction(  ACTID_UPPER_STEP );return TRUE;
			}
			// →+B
			else if((key & KEYSTA_BB2) && (key & KEYSTA_FOWORD)){
				ChangeAction(  ACTID_BAT );return TRUE;
			}
		}
	}

	return CCharacterBase::Command_OnNormal(keyinfo);
}

BOOL CCharacter::Command_OnAttacking(DWORD key)
{	
	if( pdat->aid & ACTID_KUCYU )
	{
	}
	else
	{
		//追加チェーン
		if(opt_exattack)
		{
			// ＼+C
			if((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD) && (key & KEYSTA_DOWN) && ChainCombo(CHAIN_HKICK)){
				ChangeAction(  ACTID_HKICK );
				return TRUE;
			}

			// →+C
			else if((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD) && ChainCombo(CHAIN_HAND)){
				ChangeAction(  ACTID_HAND );return TRUE;
			}

			// ＼+B
			else if((key & KEYSTA_BB2) && (key & KEYSTA_FOWORD) && (key & KEYSTA_DOWN) && ChainCombo(CHAIN_UPPER)){
				ChangeAction(  ACTID_UPPER_STEP );
				return TRUE;
			}
			// →+B
			else if((key & KEYSTA_BB2) && (key & KEYSTA_FOWORD) && ChainCombo(CHAIN_BAT)){
				ChangeAction(  ACTID_BAT );
				return TRUE;
			}
		}
	}

	return CCharacterBase::Command_OnAttacking(key);
}

/*--------------------------------------------------------------------------------------
	コマンド判定 処理(COM)
----------------------------------------------------------------------------------------*/

DWORD CCharacter::CommandCOM(DWORD wid)
{
	//ゲージチェック
	if(wid==ACTID_ESCAPE && (pdat->gauge<1.0f || !EscapeOK()) ){
		return FALSE;
	}
	if(wid==ACTID_GCANCEL1 || wid==ACTID_GCANCEL2 || wid==ACTID_GCANCEL3)
	{
		if(pdat->gauge<NEEDGAUGE_GC)return FALSE;
		if(!((pdat->aid&ACTID_GUARD) && opt_gcancel))return FALSE;
	}

	return CCharacterBase::CommandCOM(wid);
}

DWORD CCharacter::CmdCom_OnSystem(DWORD wid)
{
	//ガーキャン
	if(wid==ACTID_GCANCEL1 || wid==ACTID_GCANCEL2 || wid==ACTID_GCANCEL3)
	{
		if(pdat->aid&ACTID_KUCYU)
		{
			return FALSE;
		}
		return FALSE;
	}

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
		
			if(NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE)) {
				pdat->aid = ACTID_NAGE1;
				return TRUE;
			}
			else return FALSE;
		}
	}
	//必殺投げ
	if(wid==ACTID_IKARI)
	{
		if(!(pdat->aid&ACTID_KUCYU) && !(pdat->aid&ACTID_ATTACK))
		{
			if(NageHantei(MAAI_HNAGE) && opt_ikari)
			if(ObjCatch(pdat->eid, GOBJMSG_IKARINAGE)) {
				pdat->aid = ACTID_NAGE1;
				return TRUE;
			}
		}
		return FALSE;
	}

	if(!(pdat->aid&ACTID_KUCYU) && (opt_exattack))
	{
		//チェーンコンボ判定(追加)
		if(GetGObject()->aid & ACTID_ATTACK)
		{
			switch(wid)
			{
			case ACTID_HKICK		:return ChainCombo(CHAIN_HKICK);break;
			case ACTID_HAND			:return ChainCombo(CHAIN_HAND);break;
			case ACTID_UPPER_STEP	:return ChainCombo(CHAIN_UPPER);break;
			case ACTID_BAT			:return ChainCombo(CHAIN_BAT);break;
			}
		}
	}

	return CCharacterBase::CmdCom_OnNormal(wid);
}

/*--------------------------------------------------------------------------------------
	GOBJMSG_TOUCHB(攻撃あたり)
----------------------------------------------------------------------------------------*/
DWORD CCharacter::TouchB(ATTACKINFO *info,BOOL hit)
{
	double dp = 0.0f;
	switch(info->id & 0x0000FFFF){
		case ATTACK_WEAK	:dp=0.02;break;
		case ATTACK_MIDDLE	:dp=0.03;break;
		case ATTACK_STRONG	:dp=0.04;break;
		case ATTACK_HISSATU	:dp=0.05;break;
		case ATTACK_FRIENDS :dp=-0.1;break;
	}
	if(!hit)dp*=0.5;
	AddPowerGauge(dp);

	return CCharacterBase::TouchB(info,hit);
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
		case ACTID_ATT_SB	:if(pdat->counter<4)sa=TRUE;	break;
		case ACTID_ATT_SC	:if(pdat->counter<8)sa=TRUE;	break;
		case ACTID_ATT_CA	:if(pdat->counter<2)sa=TRUE;	break;
		case ACTID_ATT_CB	:if(pdat->counter<4)sa=TRUE;	break;
		case ACTID_ATT_CC	:if(pdat->counter<8)sa=TRUE;	break;
		case ACTID_ATT_JA	:if(pdat->counter<2)sa=TRUE;	break;
		case ACTID_ATT_JB	:if(pdat->counter<4)sa=TRUE;	break;
		case ACTID_ATT_JC	:if(pdat->counter<4)sa=TRUE;	break;

		case ACTID_SAH1		:if(pdat->vy<-1)sa=TRUE;		break;
		case ACTID_SAH2		:if(pdat->vy<-1)sa=TRUE;		break;
		case ACTID_SAH3		:if(pdat->vy<-1)sa=TRUE;		break;
		case ACTID_DASHF	:sa=TRUE;						break;
		case ACTID_ESCAPE	:sa=TRUE;						break;

		case ACTID_BAT		:if(pdat->counter<4)sa=TRUE;	break;
		case ACTID_UPPER_STEP:sa=TRUE;						break;
		case ACTID_HAND		:if(pdat->counter<5)sa=TRUE;	break;
		case ACTID_HKICK	:if(pdat->counter<4)sa=TRUE;	break;

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
	GOBJMSG_TOUCHC
----------------------------------------------------------------------------------------*/
DWORD CCharacter::TouchC(ATTACKINFO *info,DWORD tc_eid)
{
	if(TRUE)//info->id & 0xFFFF0000)//打ち上げ系のＡＱ回避
	{
		GOBJECT* pedat = GetInfo(tc_eid);
		if(!pedat){
			dprint("fail1");
			return TOUCHC_OK;
		}
		if(pedat->atk2.info1 < &aif[0] || pedat->atk2.info1 > &aif[AIF_NUM-1]){
			dprint("fail2");
			return TOUCHC_OK;
		}
		if(!(pedat->aid&ACTID_KURAI))
		{
			return TOUCHC_OK;
		}
		DWORD prevhit = (pedat->atk2.info1->id) & 0xFFFF0000;

		if(prevhit || (info->id & 0xFFFF0000) ){
			dprint("crnt=%08X , pre=%08X  %s",info->id & 0xFFFF0000,prevhit,
				(prevhit!=(info->id & 0xFFFF0000)) ? "+" : "");
		}

		switch(info->id & 0xFFFF0000)
		{
		case ATTACK_BAT	:
			{
				switch(prevhit)
				{
				case ATTACK_HKICK1:
				case ATTACK_HKICK2:
				case ATTACK_BAT	:
					return TOUCHC_CANCEL;
				}
			}
			break;
		case ATTACK_HKICK1	:
			{
				switch(prevhit)
				{
				case ATTACK_HKICK1:
				case ATTACK_HKICK2:
					return TOUCHC_CANCEL;
				}
			}
			break;
		case ATTACK_HKICK2	:
			{
				switch(prevhit)
				{
				case ATTACK_HKICK1:
				case ATTACK_HKICK2:
					return TOUCHC_CANCEL;
				}
			}
			break;
		case ATTACK_UPPER	:
			{
				switch(prevhit)
				{
				case ATTACK_HKICK1:
				case ATTACK_HKICK2:
					return TOUCHC_CANCEL;
				}
			}
			break;
		case ATTACK_SAH1	:
			{
				if(prevhit==ATTACK_SAH2)
				{
					return TOUCHC_CANCEL;
				}
			}
			break;
		case ATTACK_SAH2	:
			{
				if(prevhit==ATTACK_SAH1)
				{
					return TOUCHC_CANCEL;
				}
			}
			break;
		}
	}
	return TOUCHC_OK;
}

/*--------------------------------------------------------------------------------------
	GOBJMSG_ACTION メッセージ 処理
	新しい行動を追加した場合は、ここに処理を追加します。
	行動の前後に共通処理を入れたい場合は PreAction(),PostAction() をオーバーライドしてください
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Action()
{
	switch(GetGObject()->aid){
	case ACTID_NAGE1		:act_nage();		break;
	case ACTID_SAH1			:act_sah1();		break;
	case ACTID_SAH2			:act_sah2();		break;
	case ACTID_SAH3			:act_sah3();		break;
	case ACTID_SAHS			:act_sahs();		break;
	case ACTID_FIG1			:act_fig1();		break;
	case ACTID_FIG2			:act_fig2();		break;
	case ACTID_FIG3			:act_fig3();		break;
	case ACTID_GCANCEL1		:act_gcancel1();	break;
	case ACTID_GCANCEL2		:act_gcancel2();	break;
	case ACTID_GCANCEL3		:act_gcancel3();	break;
	case ACTID_DASHF		:act_dashf();		break;
	case ACTID_DASHB		:act_dashb();		break;
	case ACTID_IKARI		:act_ikari();		break;
	case ACTID_ESCAPE		:act_escape();		break;
	case ACTID_STRIKER1		:act_striker1();	break;
	case ACTID_STRIKER2		:act_striker2();	break;
	case ACTID_STRIKER3		:act_striker3();	break;
	case ACTID_STRIKER4		:act_striker4();	break;
	case ACTID_BAT			:act_bat();			ChainCombo(CHAIN_BAT);	break;
	case ACTID_UPPER_STEP	:act_upper_step();	ChainCombo(CHAIN_UPPER);break;
	case ACTID_UPPER_JAMP	:act_upper_jamp();		break;
	case ACTID_UPPER_LANDING:act_upper_landing();	break;
	case ACTID_HAND			:act_hand();	ChainCombo(CHAIN_HAND);	break;
	case ACTID_HKICK		:act_hkick();	ChainCombo(CHAIN_HKICK);break;
	case ACTID_SAH_LANDING	:act_sah_landing();	break;
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

	//設定例

	int i;

	//・S攻撃

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
	aif[i].hit		=HITINFO_MARK3 | HITINFO_REACT2 | HITINFO_SNDHIT3 | HITINFO_SIV2;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=50;//ダメージ
	aif[i].kezuri	=0;//削り

	//・C攻撃

	i=ATTACKINFO_CROUCH_A;//しゃがみ弱攻撃
	aif[i].hit		=HITINFO_MARK1 | HITINFO_REACT1 | HITINFO_SNDHIT1 | HITINFO_SIV1 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_WEAK;//ゲージ増加時に使用するID
	aif[i].damage	=30;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_CROUCH_B;//しゃがみ中攻撃
	aif[i].hit		=HITINFO_MARK2 | HITINFO_REACT2 | HITINFO_SNDHIT2 | HITINFO_SIV2 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=40;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_CROUCH_C;//しゃがみ強攻撃
	aif[i].hit		=HITINFO_MARK3 | HITINFO_REACT3 | HITINFO_SNDHIT3 | HITINFO_SIV3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP | GUARDINFO_XSTAND;//ガード情報
	aif[i].id		=ATTACK_STRONG ;//ゲージ増加時に使用するID
	aif[i].damage	=50;//ダメージ
	aif[i].kezuri	=0;//削り

	//・J攻撃

	i=ATTACKINFO_JAMP_A;//ジャンプ弱攻撃
	aif[i].hit		=HITINFO_MARK1 | HITINFO_REACT1 | HITINFO_SNDHIT1 | HITINFO_SIV1 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1 | GUARDINFO_XCROUCH;//ガード情報
	aif[i].id		=ATTACK_WEAK;//ゲージ増加時に使用するID
	aif[i].damage	=30;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_JAMP_B;//ジャンプ中攻撃
	aif[i].hit		=HITINFO_MARK2 | HITINFO_REACT2 | HITINFO_SNDHIT2 | HITINFO_SIV2 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2 | GUARDINFO_XCROUCH;//ガード情報
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=40;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_JAMP_C;//ジャンプ強攻撃
	aif[i].hit		=HITINFO_MARK3 | HITINFO_REACT3 | HITINFO_SNDHIT3 | HITINFO_SIV3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=50;//ダメージ
	aif[i].kezuri	=0;//削り
	
	//・その他

	i=ATTACKINFO_STRIKER;//支援攻撃
	aif[i].hit		=HITINFO_MARK4 | HITINFO_TATAKI2A | HITINFO_SNDHIT3 | HITINFO_SIV3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=70;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_NAGE;//投げ
	aif[i].hit		=0;//ヒット情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=60;//ダメージ
	aif[i].muki		= TRUE;//反対

	//・必殺

	i=ATTACKINFO_SAH1A;//１さーん
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 | GUARDINFO_XCROUCH;//ガード情報
	aif[i].id		=ATTACK_STRONG | ATTACK_SAH1;//ゲージ増加時に使用するID
	aif[i].damage	=30;//ダメージ
	aif[i].kezuri	=3;//削り
	
	i=ATTACKINFO_SAH1B;//１さーん
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 | GUARDINFO_XCROUCH;//ガード情報
	aif[i].id		=ATTACK_STRONG | ATTACK_SAH1;//ゲージ増加時に使用するID
	aif[i].damage	=30;//ダメージ
	aif[i].kezuri	=3;//削り
	
	i=ATTACKINFO_SAH1C;//１さーん
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 | GUARDINFO_XCROUCH;//ガード情報
	aif[i].id		=ATTACK_STRONG | ATTACK_SAH1;//ゲージ増加時に使用するID
	aif[i].damage	=30;//ダメージ
	aif[i].kezuri	=3;//削り

	i=ATTACKINFO_SAH2A;//１さーん
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 | GUARDINFO_XCROUCH;//ガード情報
	aif[i].id		=ATTACK_STRONG | ATTACK_SAH2;//ゲージ増加時に使用するID
	aif[i].damage	=30;//ダメージ
	aif[i].kezuri	=3;//削り
	
	i=ATTACKINFO_SAH2B;//１さーん
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 | GUARDINFO_XCROUCH;//ガード情報
	aif[i].id		=ATTACK_STRONG | ATTACK_SAH2;//ゲージ増加時に使用するID
	aif[i].damage	=30;//ダメージ
	aif[i].kezuri	=3;//削り
	
	i=ATTACKINFO_SAH2C;//１さーん
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 | GUARDINFO_XCROUCH;//ガード情報
	aif[i].id		=ATTACK_STRONG | ATTACK_SAH2;//ゲージ増加時に使用するID
	aif[i].damage	=30;//ダメージ
	aif[i].kezuri	=3;//削り

	i=ATTACKINFO_FIG;//アイノカタチ
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV2 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
	aif[i].damage	=80;//ダメージ
	aif[i].kezuri	=10;//削り

	i=ATTACKINFO_IKARI1;//怒りをぶつける投げ
	aif[i].hit		=HITINFO_MARK2 | HITINFO_SNDHIT2;//ヒット情報
	aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
	aif[i].damage	=65;//ダメージ

	i=ATTACKINFO_IKARI2;//怒りをぶつける投げ
	aif[i].hit		=HITINFO_MARK3 | HITINFO_SNDHIT3;//ヒット情報
	aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
	aif[i].damage	=65;//ダメージ

	i=ATTACKINFO_ACCHI;//あっち！
	aif[i].hit		=HITINFO_MARK4 | HITINFO_REACT1 | HITINFO_SNDHIT3 | GUARDINFO_EFCTSINDO;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT1 ;//ガード情報
	aif[i].id		=ATTACK_FRIENDS;//ゲージ増加時に使用するID
	aif[i].damage	=20;//ダメージ
	aif[i].kezuri	=0;//削り

	//・Ex Attacks

	i= ATTACKINFO_BAT;
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV2 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_STRONG | ATTACK_BAT;//ゲージ増加時に使用するID
	aif[i].damage	=60;//ダメージ
	aif[i].kezuri	=0;//削り

	i= ATTACKINFO_UPPER;
	aif[i].hit		=HITINFO_MARK3 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV3 | GUARDINFO_XSTAND;//ガード情報
	aif[i].id		=ATTACK_STRONG | ATTACK_UPPER;//ゲージ増加時に使用するID
	aif[i].damage	=60;//ダメージ
	aif[i].kezuri	=0;//削り

	i= ATTACKINFO_HAND;
	aif[i].hit		=HITINFO_MARK2 | HITINFO_REACT2 | HITINFO_SNDHIT3 | HITINFO_SIV1 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV1 | GUARDINFO_XCROUCH;//ガード情報
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=20;//ダメージ
	aif[i].kezuri	=0;//削り

	i= ATTACKINFO_HKICK1;
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV2 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_STRONG | ATTACK_HKICK1;//ゲージ増加時に使用するID
	aif[i].damage	=60;//ダメージ
	aif[i].kezuri	=0;//削り
	
	i= ATTACKINFO_HKICK2;
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV2 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_STRONG | ATTACK_HKICK2;//ゲージ増加時に使用するID
	aif[i].damage	=60;//ダメージ
	aif[i].kezuri	=0;//削り

	//・ガードキャンセル

	i= ATTACKINFO_GCANCEL1;
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_STOP ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=0;//ゲージ増加時に使用するID
	aif[i].damage	=60;//ダメージ
	aif[i].kezuri	=0;//削り
	
	i= ATTACKINFO_GCANCEL2;
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_STOP ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=0;//ゲージ増加時に使用するID
	aif[i].damage	=80;//ダメージ
	aif[i].kezuri	=0;//削り

	i= ATTACKINFO_GCANCEL3;
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_STOP ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=0;//ゲージ増加時に使用するID
	aif[i].damage	=100;//ダメージ
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
	CCharacterBase::InitWazInfo();

	waz.dashf = ACTID_DASHF;
	waz.dashb = ACTID_DASHB;

	waz.att_short[0] = ACTID_FIG1;//短リーチ攻撃
	waz.att_short[1] = ACTID_ATT_CA;
	waz.att_short[2] = ACTID_ATT_CB;
	waz.att_short[3] = ACTID_GCANCEL1;
	waz.att_middle[0] = ACTID_ATT_SB;//中リーチ攻撃
	waz.att_middle[1] = ACTID_ATT_CC;
	waz.att_middle[2] = ACTID_FIG2;
	waz.att_middle[3] = ACTID_ESCAPE;
	waz.att_middle[4] = ACTID_UPPER_STEP;
	waz.att_long[0] = ACTID_ATT_SC;//長リーチ攻撃
	waz.att_long[1] = ACTID_ATT_SB;
	waz.att_long[2] = ACTID_ATT_CC;
	waz.att_long[3] = ACTID_BAT;
	waz.att_long[4] = ACTID_HAND;
	waz.att_long[5] = ACTID_UPPER_STEP;

	waz.att_jamp[0] = ACTID_ATT_JC;//ジャンプ攻撃
	waz.att_jamp[1] = ACTID_ATT_JB;
	waz.att_jamp[2] = ACTID_ATT_JA;

	waz.att_tai[0] = ACTID_SAH3;//対空
	waz.att_tai[1] = ACTID_SAH2;
	waz.att_tai[2] = ACTID_HKICK;
	waz.att_tai[3] = ACTID_UPPER_STEP;
	
	waz.att_tai[1] = ACTID_FIG1;
	waz.nageid[0] = ACTID_NAGE1;//投げ
	waz.nageid[1] = ACTID_IKARI;//投げ
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