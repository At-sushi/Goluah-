
/*=====================================================================================

	キャラクター定義

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
	{ OPTIONS_QUICK_LUNCH_A	, 0, 0, "Quick Lunch - A"	, 5 },
	{ OPTIONS_QUICK_LUNCH_B	, 0, 0, "Quick Lunch - B"	, 5 },
	{ OPTIONS_QUICK_LUNCH_C	, 0, 0, "Quick Lunch - C"	, 5 },
	{ OPTIONS_CRAZY_LUNCH	, 0, OPTIONS_QUICKS, "Crazy Lunch"		,10 },
	{ OPTIONS_2ND_STEP		, 0, 0, "2nd Step"			, 3 },
	{ OPTIONS_REVERSAL		, 0, 0, "Reversal Attack"	, 3 },
	{ OPTIONS_NO_CHAIN_COMBO, 0, 0, "No Chain Combo"	,-5 },
};


//**************************************************************************************
//  Character Information
//　ここにキャラクターの名前を記述します
//**************************************************************************************

CCharacterInfo CharInfo("しぃ",				// キャラクターの名前（最大31バイト）
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
	aki3d.Initialize( GetD3DDevice(), CharDirectory);
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
	//オプション解釈
	m_opt_quickA	= (option_flags&OPTIONS_QUICK_LUNCH_A	) ? TRUE : FALSE;
	m_opt_quickB	= (option_flags&OPTIONS_QUICK_LUNCH_B	) ? TRUE : FALSE;
	m_opt_quickC	= (option_flags&OPTIONS_QUICK_LUNCH_C	) ? TRUE : FALSE;
	m_opt_2nd_step	= (option_flags&OPTIONS_2ND_STEP		) ? TRUE : FALSE;
	m_opt_reversal	= (option_flags&OPTIONS_REVERSAL		) ? TRUE : FALSE;
	m_opt_crazy		= (option_flags&OPTIONS_CRAZY_LUNCH		) ? TRUE : FALSE;

	pdat->hpmax			= 1000;//最大体力値
	pdat->gaugemax		= 1;//最大ゲージ
	pdat->gaugetype		= GOBJFLGA_POWGAUGE2;//ゲージタイプ

	chainComboEnabled	= (option_flags&OPTIONS_NO_CHAIN_COMBO	) ? FALSE : TRUE;
}


/*--------------------------------------------------------------------------------------
	メッセージ処理
	ベースクラスで拾っていないメッセージを拾いたい場合は、ココに処理を追加します
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Message(DWORD msg,LPVOID dat,DWORD prm)
{
	GOBJECT* pobjdat = NULL;
	if(msg & GOBJMSG_USERDEFINE)
	{
		pobjdat = GetInfo(prm);
		if(pobjdat==NULL)return(0);

		switch(msg)
		{
		case GOBJMSG_NAGE:	return eact_nage(pobjdat);
		}
		return TRUE;
	}

	switch(msg){
	case GOBJMSG_STRIKER://支援攻撃要請
		if(pdat->aid == ACTID_TAIKICYU){
			RemoveProperty( GOBJFLG_TOBETARGET );//ターゲットにはならない
			ChangeAction( ACTID_STRIKER_IN );
			return(ROGER);
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

BOOL CCharacter::Command_OnHissatuAttacking(DWORD key)
{
	//アレコレ・タメ中
	if((pdat->aid&ACTID_MOYAMOYA) && pdat->counter>5)
	{
		if(key&(KEYSTA_BC|KEYSTA_BB|KEYSTA_BA) || IsCom())
		{
			//ボタンが押されてれば
			if( com22(20) && (GetKey(0)&KEYSTA_DOWN2) )
			{
				ArekoreCancel();
				return TRUE;
			}
		}
		else
		{
			//ボタンが押されてなければ発射
			ArekoreRelease();
			return TRUE;
		}
	}

	//ライフル、強制発射
	if(pdat->aid==ACTID_POI_RIFLE)
	{
		if( (pdat->counter>30) && 
			( pdat->counter<12+RIFLE_TIMEOUT ) || (IsLocalCom() && ( pdat->counter<12+200 ))
		)
		{
			if(key&KEYSTA_BUTTONS)
			{
				pdat->counter = 12+RIFLE_TIMEOUT;
				pbul_rifle->GetGObject()->counter = RIFLE_TIMEOUT-1;
			}
		}
	}

	return CCharacterBase::Command_OnHissatuAttacking(key);
}

BOOL CCharacter::Command_Hissatu(DWORD key)
{
	//必殺技コマンド判定
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_KUCYU){//空中必殺技のコマンド判定
	}
	else//地上必殺技のコマンド判定
	{
		//ブラッディー・ダッコ
		if(com236236(40))
		{
			if((key & KEYSTA_BC2) && pdat->gauge>=1.0f)
			{
				ChangeAction( ACTID_DAKKO );
				return TRUE;
			}
		}

		//アレコレ
		if(com236(30))
		{
			if(key & KEYSTA_BC2)
			{
				pdat->aid = ACTID_MOYAMOYA3;
				return TRUE;
			}
			else if(key & KEYSTA_BB2)
			{
				pdat->aid = ACTID_MOYAMOYA2;
				return TRUE;
			}
			else if(key & KEYSTA_BA2)
			{
				pdat->aid = ACTID_MOYAMOYA1;
				return TRUE;
			}
		}
	}

	return CCharacterBase::Command_Hissatu(key);
}

BOOL CCharacter::Command_Normal(DWORD key)
{
	//ダッシュ
	if(com66(20)){pdat->aid = ACTID_DASHF;return TRUE;}
	if(com44(20)){pdat->aid = ACTID_DASHB;return TRUE;}

	if(key & KEYSTA_UP){//ジャンプ
		pdat->aid = ACTID_JAMPS;
		nidanjamp=TRUE;
	}
	else if(key & KEYSTA_DOWN){//しゃがみ
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

	return TRUE;
}

BOOL CCharacter::Command_OnNormal(DWORD key)
{
	DWORD key_now = key;

	if(pdat->aid & ACTID_KUCYU)//ジャンプ動作中
	{
		if(nidanjamp && m_opt_2nd_step){//2段ジャンプ
			if(key_now & KEYSTA_UP2){
				pdat->aid = ACTID_2DANJAMPS;
				nidanjamp=FALSE;
				return TRUE;
			}
		}
		if(pdat->aid == ACTID_2DANJAMPS){
			if(key_now & KEYSTA_FOWORD){//前ジャンプ
				pdat->aid = ACTID_2DANJAMPF;
				return TRUE;
			}
			else if(key_now & KEYSTA_BACK){//後ジャンプ
				pdat->aid = ACTID_2DANJAMPB;
				return TRUE;
			}
		}

		//ダッシュ時、でがかりに技を出せないようにする
		if(pdat->aid==ACTID_DASHF)
		{
			if(pdat->counter<10)
			{
				key &= ~KEYSTA_ANYKEY;
			}
		}
	}
	else//地上動作中
	{
		//投げ
		if (key & KEYSTA_FOWORD && key & KEYSTA_BC2 &&
			NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE))
		{
			pdat->aid = ACTID_NAGE1;
			return TRUE;
		}

		//ステッキ（→+C）
		if((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD) && !(key & KEYSTA_DOWN))
		{
			ChangeAction( ACTID_STAFF );
			return TRUE;
		}
	}

	return CCharacterBase::Command_OnNormal(key);
}


BOOL CCharacter::Command_OnAttacking(DWORD key)
{
	if(pdat->aid & ACTID_KUCYU)//ジャンプ動作中
	{
	}
	else//地上
	{
		if((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD) && !(key & KEYSTA_DOWN))
		{
			if(ChainCombo(CHAIN_STAFF))
			{
				ChangeAction( ACTID_STAFF );
				return TRUE;
			}
		}
	}

	return CCharacterBase::Command_OnAttacking(key);
}

BOOL CCharacter::Command_OnSystem(DWORD key)
{
	//リバーサル
	if(pdat->aid==ACTID_OKIAGARI && m_opt_reversal)
	{
		if(ccounter==10  && pdat->gauge>=NEEDGAUGE_REV)
		{
			if(key&KEYSTA_UP)
			{
				ChangeAction( ACTID_REVERSAL );
				return TRUE;
			}
		}
	}
	return CCharacterBase::Command_OnSystem(key);
}

/*--------------------------------------------------------------------------------------
	コマンド判定 処理(COM)
----------------------------------------------------------------------------------------*/

DWORD CCharacter::CommandCOM(DWORD wid)
{
	if(wid==ACTID_DAKKO && pdat->gauge<1.0f)return FALSE;//ゲージチェック
	return CCharacterBase::CommandCOM(wid);
}

DWORD CCharacter::CmdCom_OnNormal(DWORD wid)
{
	//投げ
	if(!(pdat->aid&ACTID_KUCYU) && !(pdat->aid&ACTID_ATTACK))
	{
		if (wid==ACTID_STAFF) return(TRUE);
		if (wid==ACTID_NAGE1)
		{
			if(NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE)) {
				pdat->aid = ACTID_NAGE1;
				return TRUE;
			}
			else return FALSE;
		}
	}
	return CCharacterBase::CmdCom_OnNormal(wid);
}

/*--------------------------------------------------------------------------------------
	GOBJMSG_TOUCHB(攻撃あたり)
----------------------------------------------------------------------------------------*/
DWORD CCharacter::TouchB(ATTACKINFO *info,BOOL hit)
{
	if(info->id == ATTACK_DAKKO)
	{
		if(hit)
		{
			GOBJECT *hit_obj = GetInfo(m_dakko_eid);
			if(hit_obj)
			{
				hit_obj->color = 0xFF000000;
			}
			AddProperty( GOBJFLG_DONOTSTOP );
			pbul_blood->Go(pdat->muki,pdat->x+zurex(100),-80,0,0);
			ChangeAction( ACTID_DAKKO_BACK );
			PlayMySound(wave_dakko_hit);
			AddEffect(EFCTID_FLASH, 4, 0, 0);
		}
		return CCharacterBase::TouchB(info,hit);
	}

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
			case ACTID_ATT_SC:
			case ACTID_ATT_CC:
			{
				if(ComLevelCk(5) && pdat->gauge>=1.0f)
				{
					SetComAct(ACTID_DAKKO,10);
				}
				else if(ComLevelCk(3))
				{
					SetComAct(ACTID_STAFF,5);
				}
			}
			break;
		}
	}

	return CCharacterBase::TouchB(info,hit);
}

/*--------------------------------------------------------------------------------------
	GOBJMSG_TOUCHB(攻撃あたり・前処理)
----------------------------------------------------------------------------------------*/
DWORD CCharacter::TouchC(ATTACKINFO *info,DWORD tc_eid)
{
	if(info->id == ATTACK_DAKKO)
	{
		m_dakko_eid = tc_eid;
	}
	return CCharacterBase::TouchC(info,tc_eid);
}


/*--------------------------------------------------------------------------------------
	GOBJMSG_ACTION メッセージ 処理
	新しい行動を追加した場合は、ここに処理を追加します。
	行動の前後に共通処理を入れたい場合は PreAction(),PostAction() をオーバーライドしてください
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Action()
{
	switch(GetGObject()->aid){
	//追加した行動遷移
	case ACTID_DASHF			:act_dashf();			break;
	case ACTID_DASHB			:act_dashb();			break;
	case ACTID_2DANJAMPS		:act_2danjamps();		break;
	case ACTID_2DANJAMPC		:act_2danjampc();		break;
	case ACTID_2DANJAMPF		:act_2danjampf();		break;
	case ACTID_2DANJAMPB		:act_2danjampb();		break;
	case ACTID_STAFF			:act_staff();			ChainCombo(CHAIN_STAFF);break;
	case ACTID_NAGE1			:act_nage();			break;
	case ACTID_MOYAMOYA1		:act_moyamoya1();		break;
	case ACTID_MOYAMOYA2		:act_moyamoya2();		break;
	case ACTID_MOYAMOYA3		:act_moyamoya3();		break;
	case ACTID_POI_OCYA			:act_poi_ocya();		break;
	case ACTID_POI_JIEN			:act_poi_jien();		break;
	case ACTID_POI_AROE			:act_poi_aroe();		break;
	case ACTID_POI_MISSILE_H	:act_poi_missile_h();	break;
	case ACTID_POI_MISSILE_V	:act_poi_missile_v();	break;
	case ACTID_POI_MOUKONEEYO	:act_poi_moukoneeyo();	break;
	case ACTID_POI_MISSILE_SP	:act_poi_missileSP();	break;
	case ACTID_POI_HANDG		:act_poi_handg();		break;
	case ACTID_POI_CRAYMORE		:act_poi_craymore();	break;
	case ACTID_POI_FIRETHROW	:act_poi_firethrow();	break;
	case ACTID_POI_SHOT			:act_poi_shot();		break;
	case ACTID_POI_RIFLE		:act_poi_rifle();		break;
	case ACTID_POI_ROCKET		:act_poi_rocket();		break;
	case ACTID_POI_LANTERN		:act_poi_lantern();		break;
	case ACTID_POI_HELLFIRE		:act_poi_hellfire();	break;
	case ACTID_POI_HTTPLASER	:act_poi_httplaser();	break;
	case ACTID_POI_ERASER99		:act_poi_eraser99();	break;
	case ACTID_DAKKO			:act_dakko();			break;
	case ACTID_DAKKO_BACK		:act_dakko_back();		break;
	case ACTID_STRIKER_IN		:act_striker_in();		break;
	case ACTID_STRIKER_ATTACK	:act_striker_attack();	break;
	case ACTID_STRIKER_OUT		:act_striker_out();		break;
	case ACTID_REVERSAL			:act_reversal();		break;
	default:
		//基本行動
		return CCharacterBase::Action();
	}

	return(TRUE);
}

void CCharacter::PreAction()
{
	//ダメージ喰らい等で予期せぬ行動遷移が起こった場合、飛び道具の無効化
	if(!(pdat->aid & ACTID_MOYAMOYA))
	{
		if(pbul_moya)pbul_moya->ForceEnd();
	}
	if(pdat->aid!=ACTID_POI_RIFLE)
	{
		pbul_rifle->ForceStop();
	}
	if(pdat->aid!=ACTID_POI_FIRETHROW && pdat->aid!=ACTID_POI_HELLFIRE)
	{
		pbul_firethrower->ForceStop();
	}
	if(pdat->aid!=ACTID_POI_ERASER99)
	{
		pbul_eraser99->ForceStop();
	}

	//COMリバーサル
	if(pdat->aid==ACTID_OKIAGARI && m_opt_reversal)
	{
		if(ccounter==10)
		{
			if( IsLocalCom() && pdat->gauge>=NEEDGAUGE_REV)
			{
				if( GetMaai_H(pdat->id,pdat->eid) < 50 )
				{
					if(rand()%3)
					{
						ChangeAction( ACTID_REVERSAL );
					}
				}
			}
		}
	}

	//COMもやもや
	if( IsLocalCom() && ((pdat->aid&ACTID_MOYAMOYA) && pdat->counter>5))
	{
		UINT maai = GetMaai_H(pdat->id,pdat->eid);
		int t = rand()%50;
		if(t< ((maai<60) ? 6 : 1) )
		{
			ArekoreCancel();
		}
		else if(t<((maai<60) ? 12 : 2))
		{
			ArekoreRelease();
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

	//・立
	i=ATTACKINFO_STAND_A;//立ち弱攻撃
	aif[i].hit		=HITINFO_REACT1 | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1 ;//ヒット情報
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
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=50;//ダメージ
	aif[i].kezuri	=0;//削り

	//・屈

	i=ATTACKINFO_CROUCH_A;//しゃがみ弱攻撃
	aif[i].hit		=HITINFO_REACT1 | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1  | GUARDINFO_XCROUCH | GUARDINFO_XJAMP ;//ガード情報
	aif[i].id		=ATTACK_WEAK;//ゲージ増加時に使用するID
	aif[i].damage	=30;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_CROUCH_B;//しゃがみ中攻撃
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2  | GUARDINFO_XJAMP ;//ガード情報
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=40;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_CROUCH_C;//しゃがみ強攻撃
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_STRONG ;//ゲージ増加時に使用するID
	aif[i].damage	=50;//ダメージ
	aif[i].kezuri	=0;//削り

	//・飛

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
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV1 | HITINFO_MARK2 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_XCROUCH;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=30;//ダメージ
	aif[i].kezuri	=0;//削り

	//・→+C

	i=ATTACKINFO_STAFF;
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=50;//ダメージ
	aif[i].kezuri	=0;//削り

	//・投げ

	i=ATTACKINFO_NAGE;
	aif[i].hit		=HITINFO_REACT3 | HITINFO_MARK3 | HITINFO_SNDHIT3 | HITINFO_EFCTSINDO;//ヒット情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=25;//ダメージ
	aif[i].kezuri	=0;//削り

	//・飛び道具類

	i=ATTACKINFO_OCYA;//お茶
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 ;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=40;//ダメージ
	aif[i].kezuri	=2;//削り
	
	i=ATTACKINFO_JIEN;//ジエン
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 ;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=60;//ダメージ
	aif[i].kezuri	=3;//削り
	
	i=ATTACKINFO_AROE;//アロエ
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=300;//ダメージ
	aif[i].kezuri	=10;//削り
	
	i=ATTACKINFO_MISSILE;//ミサイル
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 | HITINFO_EFCTBURN;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 ;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=60;//ダメージ
	aif[i].kezuri	=6;//削り

	i=ATTACKINFO_MOUKONEEYO;//モウコネエヨ
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 ;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=30;//ダメージ
	aif[i].kezuri	=3;//削り

	i=ATTACKINFO_HANDG;//ハンドグレネード
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 | HITINFO_EFCTBURN;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP ;//ガード情報
	aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
	aif[i].damage	=60;//ダメージ
	aif[i].kezuri	=6;//削り

	i=ATTACKINFO_CRAYMORE;//クレイモア
	aif[i].hit		=HITINFO_REACT2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_XJAMP ;//ガード情報
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=20;//ダメージ
	aif[i].kezuri	=1;//削り

	i=ATTACKINFO_RIFLE;//ライフル
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XSTAND | GUARDINFO_XCROUCH | GUARDINFO_XJAMP ;//ガード情報
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=100;//ダメージ
	aif[i].kezuri	=10;//削り

	i=ATTACKINFO_FIRETHROWER;//火炎放射器
	aif[i].hit		=HITINFO_REACT2 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_XJAMP ;//ガード情報
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=20;//ダメージ
	aif[i].kezuri	=5;//削り

	i=ATTACKINFO_ROCKET;//ロケット
	aif[i].hit		=HITINFO_EFCTSINDO | HITINFO_EFCTBURN | HITINFO_SIV3 | HITINFO_FUTTOBI | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_EFCTSINDO | GUARDINFO_SIV3 | GUARDINFO_REACT3| GUARDINFO_XJAMP ;//ガード情報
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=250;//ダメージ
	aif[i].kezuri	=25;//削り

	i=ATTACKINFO_SHOT;//ショットガン
	aif[i].hit		=HITINFO_REACT2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT2;//ガード情報
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=20;//ダメージ
	aif[i].kezuri	=1;//削り

	i=ATTACKINFO_AGI;//アギ
	aif[i].hit		=HITINFO_SIV3 | HITINFO_REACT3 | HITINFO_MARK4 | HITINFO_SNDHIT3 | HITINFO_EFCTBURN ;//ヒット情報
	aif[i].guard	=GUARDINFO_SIV3 | GUARDINFO_REACT3;//ガード情報
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=50;//ダメージ
	aif[i].kezuri	=3;//削り

	i=ATTACKINFO_HTTPLASER;//レーザー
	aif[i].hit		=HITINFO_EFCTBURN_B | HITINFO_SIV1 | HITINFO_REACT3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_SIV3 | GUARDINFO_REACT1| GUARDINFO_XJAMP ;//ガード情報
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=250;//ダメージ
	aif[i].kezuri	=30;//削り
	
	i=ATTACKINFO_ERASER99;//イレイサー
	aif[i].hit		=HITINFO_EFCTBURN_B | HITINFO_EFCTSINDO | HITINFO_REACT3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3| GUARDINFO_XJAMP | GUARDINFO_XSTAND | GUARDINFO_XCROUCH ;//ガード情報
	aif[i].id		=0;//ゲージ増加時に使用するID
	aif[i].damage	=9999;//ダメージ
	aif[i].kezuri	=99;//削り

	//・ブラッディー・ダッコ

	i=ATTACKINFO_DAKKO;
	aif[i].hit		=HITINFO_REACT3 | HITINFO_MARK3 | HITINFO_SNDHIT3 | HITINFO_STOP;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XSTAND;//ガード情報
	aif[i].id		=ATTACK_DAKKO;//ゲージ増加時に使用するID
	aif[i].damage	=450;//ダメージ
	aif[i].kezuri	=45;//削り

	//・リバーサル

	i=ATTACKINFO_REVERSAL;
	aif[i].hit		=HITINFO_FUTTOBI2 | HITINFO_SIV1 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 | GUARDINFO_XSTAND |  GUARDINFO_XJAMP ;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=25;//ダメージ
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

	//pdat->winfo へ行動IDを追加する
	waz.att_short[0]  = ACTID_ATT_SA;//短リーチ攻撃
	waz.att_short[1]  = ACTID_ATT_CA;
	waz.att_short[2]  = ACTID_ATT_SB;
	waz.att_short[3]  = ACTID_ATT_CB;
	waz.att_short[4]  = ACTID_NAGE1;

	waz.att_middle[0] = ACTID_ATT_SB;//中リーチ攻撃
	waz.att_middle[1] = ACTID_ATT_CB;
	waz.att_middle[2] = ACTID_ATT_SC;
	waz.att_middle[3] = ACTID_ATT_CC;
	waz.att_middle[4] = ACTID_STAFF;
	waz.att_middle[5] = ACTID_DAKKO;

	waz.att_long[0]   = ACTID_ATT_SC;//長リーチ攻撃 
	waz.att_long[1]   = ACTID_ATT_CC;
	waz.att_long[2]   = ACTID_MOYAMOYA1;
	waz.att_long[3]   = ACTID_MOYAMOYA2;
	waz.att_long[4]   = ACTID_DAKKO;

	waz.att_bullet[0] = ACTID_MOYAMOYA1;//飛び道具
	waz.att_bullet[1] = ACTID_MOYAMOYA2;//飛び道具
	waz.att_bullet[2] = ACTID_MOYAMOYA3;//飛び道具

	waz.att_tai[0]    = ACTID_STAFF; //対空

	//リーチ設定
	SetComReach( 0,MAAI_SHORT	);
	SetComReach( 1,MAAI_MIDDLE );
	SetComReach( 2,MAAI_LONG );
}


