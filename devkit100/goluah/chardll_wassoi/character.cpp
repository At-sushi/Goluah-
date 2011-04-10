
/*=====================================================================================

	キャラクター定義

=======================================================================================*/
#include "character.h"

//**************************************************************************************
//  Option Settings
//　ここに、キャラクターに設定できるオプションを指定します
//**************************************************************************************

CHARACTER_LOAD_OPTION option[] = {

	// 　{ フラグ, 競合するｵﾌﾟｼｮﾝ,依存するｵﾌﾟｼｮﾝ, オプション名, 消費ポイント }
	{ OPTIONS_HEAVY_HIT		, 0, 0, "Heavy Hit"		, 5 },
	{ OPTIONS_AUTO_GUARD	, 0, 0, "Auto Guard"	, 8 },
	{ OPTIONS_GUARD_CANCEL	, 0, 0, "Guard Cancel"	, 5 },
	{ OPTIONS_KOROKORO		, 0, 0, "KoroKoro"		, 5 },
	{ OPTIONS_HPUP			, 0, 0, "HP+20%"		, 8 },
	{ OPTIONS_REGENERATOR	, 0, 0, "Regenerator"	, 8 },
};


//**************************************************************************************
//  Character Information
//　ここにキャラクターの名前を記述します
//**************************************************************************************

CCharacterInfo CharInfo("おにぎり",			// キャラクターの名前（最大31バイト）
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
	pdat->hpmax			= 1000;//最大体力値
	pdat->gaugemax		= 1;//最大ゲージ
	pdat->gaugetype		= GOBJFLGA_POWGAUGE2;//ゲージタイプ

	option_flag = CCharacterBase::option_flags;

	//選択オプション
	//HP+
	if(option_flag & OPTIONS_HPUP){
		pdat->hpmax = (int)(pdat->hpmax*1.2);
	}
	//auto guard
	if(option_flag & OPTIONS_AUTO_GUARD){
		isAutoGuard = TRUE;
	}

	hp_to_regenerate = 0;
	ZeroMemory(m_blur, sizeof(m_blur));
	m_blur_target = 0;
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
	case GOBJMSG_FESTIVAL_NORI_HIT://海苔ヒット
		if(pdat->hp > 0 && !(pdat->aid&ACTID_SYSTEM)){
			ChangeAction(ACTID_FESTIVALDASH);
		}
		break;
	case GOBJMSG_STRIKER://支援攻撃要請
		if(pdat->aid == ACTID_TAIKICYU){
			pdat->objtype &= ~GOBJFLG_TOBETARGET;//ターゲットにはならない
			ChangeAction( ACTID_STRIKER1);
			return(TRUE);
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
	if(pdat->aid==ACTID_OKIAGARI && pdat->counter==2 && 
		pdat->gauge>=NEEDGAUGE_REV && OptKoroKoro())
	{
		if(key & KEYSTA_FOWORD)
		{
			ChangeAction(ACTID_OKIAGARI_FOWORD);
			pdat->muteki = TRUE;
			pdat->nagerare = FALSE;
			return TRUE;
		}
		else if(key & KEYSTA_BACK)
		{
			ChangeAction(ACTID_OKIAGARI_BACK);
			pdat->muteki = TRUE;
			pdat->nagerare = FALSE;
			return TRUE;
		}
	}

	if((pdat->aid & ACTID_GUARD) && OptGuardCancel()){
		if(!(pdat->aid & ACTID_KUCYU))
		{
			//地上ガード中
			//ガードキャンセル
			if(pdat->gauge>=NEEDGAUGE_GC){
				if(key & KEYSTA_BC2){pdat->aid = ACTID_GCANCEL3;return TRUE;}
				else if(key & KEYSTA_BB2){pdat->aid = ACTID_GCANCEL2;return TRUE;}
				else if(key & KEYSTA_BA2){pdat->aid = ACTID_GCANCEL1;return TRUE;}
			}
		}
	}

	return CCharacterBase::Command_OnSystem(key);
}

BOOL CCharacter::Command_Hissatu(DWORD key_now)
{
	//必殺技コマンド判定
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_KUCYU){//空中必殺技のコマンド判定
	}
	else{//地上必殺技のコマンド判定
		if(pdat->gauge==1.0 && com2363214(60) && (key_now & KEYSTA_BC2)){//おにぎりフェスティバル
			pdat->aid = ACTID_FESTIVAL;return TRUE;
		}
		if(com623(30)){//商流縣
			if(key_now & KEYSTA_BC2){pdat->aid = ACTID_WASSYORYUKEN3;return TRUE;}
			else if(key_now & KEYSTA_BB2){pdat->aid = ACTID_WASSYORYUKEN2;return TRUE;}
			else if(key_now & KEYSTA_BA2){pdat->aid = ACTID_WASSYORYUKEN1;return TRUE;}
		}
		if(com214(30)){//しゃら～
			if(key_now & KEYSTA_BC2){pdat->aid = ACTID_SYARA3;return TRUE;}
			else if(key_now & KEYSTA_BB2){pdat->aid = ACTID_SYARA2;return TRUE;}
			else if(key_now & KEYSTA_BA2){pdat->aid = ACTID_SYARA1;return TRUE;}
		}
		if(b_nori && b_nori->isReady() && com236(30)){//海苔
			if(key_now & KEYSTA_BC2){pdat->aid = ACTID_HADOU3;return TRUE;}
			else if(key_now & KEYSTA_BB2){pdat->aid = ACTID_HADOU2;return TRUE;}
			else if(key_now & KEYSTA_BA2){pdat->aid = ACTID_HADOU1;return TRUE;}
		}
	}

	return CCharacterBase::Command_Hissatu(key_now);
}

BOOL CCharacter::Command_Normal(DWORD key)
{
	//ダッシュコマンド例
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
		//if(key & KEYSTA_BACK && pdat->aid==ACTID_DASHB)return TRUE;//ダッシュ継続
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

	if(pdat->aid & ACTID_KUCYU){//ジャンプ動作中
	}
	else{//地上動作中
		//投げ
		if (keyinfo & KEYSTA_FOWORD && keyinfo & KEYSTA_BC2 &&
			NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE)) {
				pdat->aid = ACTID_NAGE1;
				return TRUE;
		}

		// →+B
		if((keyinfo&KEYSTA_FOWORD) && (keyinfo&KEYSTA_BB2))
		{
			ChangeAction(ACTID_ATT_SMC);
			return TRUE;
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
		// →+B
		if((key & KEYSTA_BB2) && (key & KEYSTA_FOWORD) && ChainCombo(CHAIN_SMC)){
			ChangeAction(ACTID_ATT_SMC);
			return TRUE;
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
	if(wid==ACTID_FESTIVAL && pdat->gauge<1.0f)return FALSE;

	if(wid==ACTID_GCANCEL1 || wid==ACTID_GCANCEL2 || wid==ACTID_GCANCEL3)
	{
		if(!(pdat->aid & ACTID_GUARD))
		{
			return FALSE;
		}
	}

	return CCharacterBase::CommandCOM(wid);
}

DWORD CCharacter::CmdCom_OnSystem(DWORD wid)
{
	//ガーキャン
	if(wid==ACTID_GCANCEL1 || wid==ACTID_GCANCEL2 || wid==ACTID_GCANCEL3)
	{
		if((pdat->aid & ACTID_GUARD) && !(pdat->aid&ACTID_KUCYU) && pdat->gauge>=NEEDGAUGE_GC && OptGuardCancel())
		{
			return TRUE;
		}
		return FALSE;
	}

	return CCharacterBase::CmdCom_OnSystem(wid);
}

DWORD CCharacter::CmdCom_OnNormal(DWORD wid)
{
	if(!(pdat->aid&ACTID_KUCYU))
	{
		//投げ
		if (wid==ACTID_NAGE1 && NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE)) {
			pdat->aid = ACTID_NAGE1;
			return TRUE;
		}

		//チェーンコンボ判定(追加)
		if(GetGObject()->aid & ACTID_ATTACK)
		{
			switch(wid)
			{
			case ACTID_ATT_SMC	:return ChainCombo(CHAIN_SMC);break;
			}
		}
	}

	// 対空技
	if (wid==ACTID_WASSYORYUKEN1 || wid==ACTID_WASSYORYUKEN2 || wid==ACTID_WASSYORYUKEN3)
		return TRUE;

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
	case ACTID_ATT_SMC			:act_att_smc(); ChainCombo(CHAIN_SMC); break;
	case ACTID_WASSYORYUKEN1	:act_wassoi1();			break;
	case ACTID_WASSYORYUKEN2	:act_wassoi2();			break;
	case ACTID_WASSYORYUKEN3	:act_wassoi3();			break;
	case ACTID_WASSYORYUKENF	:act_wassoif();			break;
	case ACTID_HADOU1			:act_hadou1();			break;
	case ACTID_HADOU2			:act_hadou2();			break;
	case ACTID_HADOU3			:act_hadou3();			break;
	case ACTID_DASHF			:act_dashf();			break;
	case ACTID_DASHB			:act_dashb();			break;
	case ACTID_SYARA1			:act_syara1();			break;
	case ACTID_SYARA2			:act_syara2();			break;
	case ACTID_SYARA3			:act_syara3();			break;
	case ACTID_SYARAF			:act_syaraf();			break;
	case ACTID_FESTIVAL			:act_festival();		break;
	case ACTID_FESTIVALDASH		:act_festivaldash();	break;
	case ACTID_FESTIVALSUKI		:act_festivalsuki();	break;
	case ACTID_FESTIVALRUSH		:act_festivalrush();	break;
	case ACTID_FESTIVALFINISH	:act_festivalfinish();	break;
	case ACTID_GCANCEL1			:act_gcancel1();		break;
	case ACTID_GCANCEL2			:act_gcancel2();		break;
	case ACTID_GCANCEL3			:act_gcancel3();		break;
	case ACTID_NAGE1			:act_nage();			break;
	case ACTID_OKIAGARI_FOWORD	:act_okiagari_foword();	break;
	case ACTID_OKIAGARI_BACK	:act_okiagari_back();	break;
	case ACTID_STRIKER1			:act_striker1();		break;
	case ACTID_STRIKER2			:act_striker2();		break;
	case ACTID_STRIKER3			:act_striker3();		break;
	case ACTID_STRIKER4			:act_striker4();		break;
	default://それ以外はキャラクター基本クラスに任せる
		return CCharacterBase::Action();
	}

	return(TRUE);
}

void CCharacter::PreAction()
{
	//HPの回復
	if(OptRegenerator() && pdat->hp>5 && hp_to_regenerate>0)
	{
		int regene_rate = 10;

		if(
				!(pdat->aid & ACTID_ATTACK) && 
				!(pdat->aid & ACTID_SYSTEM) && 
				!(pdat->aid & ACTID_KUCYU)  && 
				(pdat->aid != ACTID_TOJYO) && 
				(pdat->aid != ACTID_SYORI) 
		)
		{
			if((pdat->counter%regene_rate)==0){
				pdat->hp += 1;
				hp_to_regenerate--;
			}
			if(pdat->hp > (int)pdat->hpmax)pdat->hp = pdat->hpmax;
		}
		else
		{
			if((pdat->counter%(regene_rate*3))==0){
				pdat->hp += 1;
				hp_to_regenerate--;
			}
			if(pdat->hp > (int)pdat->hpmax)pdat->hp = pdat->hpmax;
		}
	}

	// ブラー追加
	m_blur[m_blur_target].cnum = pdat->cnow;
	m_blur[m_blur_target].x = pdat->x;
	m_blur[m_blur_target].y = pdat->y;
	m_blur[m_blur_target].z = pdat->z;
	m_blur[m_blur_target].rot = pdat->rot;
	m_blur[m_blur_target].revx = pdat->revx != pdat->muki;
	m_blur[m_blur_target].revy = pdat->revy;
	m_blur[m_blur_target].color = 0x00FFFFFF;
	m_blur[m_blur_target].magx = pdat->magx;
	m_blur[m_blur_target].magy = pdat->magy;
	m_blur_target = (m_blur_target + 1) % NUM_BLURS;

	CCharacterBase::PreAction();
}

DWORD CCharacter::Draw()
{
/*	for (int i = m_blur_target; i < m_blur_target+NUM_BLURS; i++)
	{
		int t = i % NUM_BLURS;

		CellDraw(bitmaps, cells, rects,
				 m_blur[t].cnum,
				 m_blur[t].x, m_blur[t].y,
				 m_blur[t].z,
				 m_blur[t].rot,
				 m_blur[t].revx, m_blur[t].revy,
				 m_blur[t].color | (0xFF * (i-m_blur_target+1) / NUM_BLURS << 24),
				 m_blur[t].magx, m_blur[t].magy);
	}

	return TRUE;
*/
	return FALSE;
}

/*----------------------------------------------------------------
	TOUCHB
------------------------------------------------------------------*/
DWORD CCharacter::TouchB(ATTACKINFO *info,BOOL hit)
{
	CCharacterBase::TouchB(info,hit);

	//ゲージの増加
	switch(info->id){
	case ATTACK_WEAK:
		if(hit)pdat->gauge += 0.02;
		else   pdat->gauge += 0.01;
		break;
	case ATTACK_MIDDLE:
		if(hit)pdat->gauge += 0.04;
		else   pdat->gauge += 0.02;
		break;
	case ATTACK_STRONG:
		if(hit)pdat->gauge += 0.06;
		else   pdat->gauge += 0.03;
		break;
	case ATTACK_HISSATU:
		if(hit)pdat->gauge += 0.08;
		else   pdat->gauge += 0.04;
		break;
	}
	if(pdat->gauge > pdat->gaugemax)
		pdat->gauge = pdat->gaugemax;

	if(info->id == ATTACK_SCDASH){
		if(hit)ChangeAction( ACTID_FESTIVALRUSH );
		else ChangeAction( ACTID_FESTIVALSUKI );
	}

	if(IsLocalCom())
	{
		switch(pdat->aid)
		{
		case ACTID_ATT_SA:
			{
				if(ComLevelCk(3)){
					SetComAct(ACTID_ATT_SB,3);
				}
				if(ComLevelCk(3)){
					SetComAct(ACTID_ATT_SC,5);
				}
			}
			break;

		case ACTID_ATT_SB:
			{
				if(ComLevelCk(3)){
					SetComAct(ACTID_ATT_SC,3);
				}
			}
			break;

		case ACTID_ATT_SC:
		case ACTID_ATT_SMC:
			{
				if(ComLevelCk(5) && pdat->gauge >= 1.0){
					SetComAct(ACTID_FESTIVAL,5);
				}
				if(ComLevelCk(3)){
					SetComAct(ACTID_SYARA2,5);
				}
				if(ComLevelCk(3)){
					SetComAct(ACTID_HADOU2,5);
				}
				if(ComLevelCk(3)){
					SetComAct(ACTID_ATT_CC,5);
				}
			}
			break;

		case ACTID_ATT_CA:
			{
				if(ComLevelCk(3)){
					SetComAct(ACTID_ATT_CC,3);
				}
			}
			break;

		case ACTID_ATT_CB:
			{
				if(ComLevelCk(3)){
					SetComAct(ACTID_ATT_CC,3);
				}
			}
			break;

		case ACTID_ATT_CC:
			{
				if(ComLevelCk(5) && pdat->gauge >= 1.0){
					SetComAct(ACTID_FESTIVAL,5);
				}
				if(ComLevelCk(3)){
					SetComAct(ACTID_SYARA2,5);
				}
				if(ComLevelCk(3)){
					SetComAct(ACTID_HADOU2,5);
				}
			}
			break;
		}
	}

	return(0);
}

//!GOBJMSG_TOUCHAに対応
DWORD CCharacter::TouchA(ATTACKINFO *info,DWORD ta_eid)
{
	//超必中はスーパーアーマー
	if(pdat->aid==ACTID_FESTIVALDASH ||
		pdat->aid==ACTID_FESTIVALRUSH ||
		pdat->aid==ACTID_FESTIVALFINISH )
	{
		isSuperArmer = TRUE;
	}

	DWORD ret = CCharacterBase::TouchA(info,ta_eid);

	if(ret & 0x10000000)//喰らった
	{
		hp_to_regenerate /= 2;
		hp_to_regenerate += info->damage/2;
	}

	isSuperArmer = FALSE;
	return ret;
}

/*--------------------------------------------------------------------------------------
	攻撃力情報初期化
----------------------------------------------------------------------------------------*/
void CCharacter::InitAttackInfo()
{
	ZeroMemory(aif,sizeof(aif));
	int i;

	i=ATTACKINFO_STAND_A;//立ち弱攻撃
	aif[i].hit		=HITINFO_MARK1 | HITINFO_REACT1 | HITINFO_SNDHIT1 | HITINFO_SIV1;
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1 | GUARDINFO_XJAMP;
	aif[i].id		=ATTACK_WEAK;//ゲージ増加時に使用するID
	aif[i].damage	=30;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_STAND_B;//立ち中攻撃
	aif[i].hit		= HITINFO_MARK2 | HITINFO_REACT2 | HITINFO_SNDHIT2 | HITINFO_SIV2;
	aif[i].guard	= GUARDINFO_REACT2 | GUARDINFO_SIV2 | GUARDINFO_XJAMP;
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=45;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_STAND_C;//立ち強攻撃
	aif[i].hit		=HITINFO_MARK3 | HITINFO_REACT3 | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=60;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_CROUCH_A;//しゃがみ弱攻撃
	aif[i].hit		= HITINFO_MARK1 | HITINFO_REACT1 | HITINFO_SNDHIT1 | HITINFO_SIV1;
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1 | GUARDINFO_XJAMP;
	aif[i].id		=ATTACK_WEAK;//ゲージ増加時に使用するID
	aif[i].damage	=30;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_CROUCH_B;//しゃがみ中攻撃
	aif[i].hit		= HITINFO_MARK2 | HITINFO_REACT2 | HITINFO_SNDHIT2 | HITINFO_SIV2;
	aif[i].guard	= GUARDINFO_REACT2 | GUARDINFO_SIV2 | GUARDINFO_XJAMP;
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=45;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_CROUCH_C;//しゃがみ強攻撃
	aif[i].hit		=HITINFO_MARK3 | HITINFO_REACT3 | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP | GUARDINFO_XSTAND;//しゃがみガード不可
	aif[i].id		=ATTACK_STRONG ;//ゲージ増加時に使用するID
	aif[i].damage	=60;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_JAMP_A;//ジャンプ弱攻撃
	aif[i].hit		=HITINFO_MARK1 | HITINFO_REACT1 | HITINFO_SNDHIT1 | HITINFO_SIV1;
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1 | GUARDINFO_XCROUCH;
	aif[i].id		=ATTACK_WEAK;//ゲージ増加時に使用するID
	aif[i].damage	=30;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_JAMP_B;//ジャンプ中攻撃
	aif[i].hit		= HITINFO_MARK2 | HITINFO_REACT2 | HITINFO_SNDHIT2 | HITINFO_SIV2;
	aif[i].guard	= GUARDINFO_REACT2 | GUARDINFO_SIV2 | GUARDINFO_XCROUCH;
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=40;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_JAMP_C;//ジャンプ強攻撃
	aif[i].hit		=HITINFO_MARK3 | HITINFO_REACT3 | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH;
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=50;//ダメージ
	aif[i].kezuri	=0;//削り

	i= ATTACKINFO_STAND_MB;//→+B
	aif[i].hit		= HITINFO_MARK3 | HITINFO_REACT3 | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	= GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP | GUARDINFO_XCROUCH | GUARDINFO_XAUTO;
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=50;//ダメージ
	aif[i].kezuri	=0;//削り

	i= ATTACKINFO_DIVE1; //ワショーイダイヴ（弱）
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 ;
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=70;//ダメージ
	aif[i].kezuri	=8;//削り

	i= ATTACKINFO_DIVE2; //ワショーイダイヴ（中）
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 ;
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=75;//ダメージ
	aif[i].kezuri	=9;//削り

	i= ATTACKINFO_DIVE3; //ワショーイダイヴ（強）
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 ;
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=80;//ダメージ
	aif[i].kezuri	=10;//削り

	i= ATTACKINFO_SYARA1; //しゃら～（弱）
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=80;//ダメージ
	aif[i].kezuri	=10;//削り

	i= ATTACKINFO_SYARA2; //しゃら～（中）
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=90;//ダメージ
	aif[i].kezuri	=10;//削り

	i= ATTACKINFO_SYARA3; //しゃら～（強）
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=100;//ダメージ
	aif[i].kezuri	=10;//削り

	i= ATTACKINFO_FESTA_D	; //フェスティバル（ダッシュヒット時）
	aif[i].hit		=HITINFO_MARK4 | HITINFO_REACT2A | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;
	aif[i].id		=ATTACK_SCDASH;//★
	aif[i].damage	=60;//ダメージ
	aif[i].kezuri	=10;//削り

	i= ATTACKINFO_FESTA	; //フェスティバル（ラッシュ時）
	aif[i].hit		=HITINFO_MARK3 | HITINFO_REACT3A | HITINFO_SNDHIT3 | HITINFO_SIV1;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3;
	aif[i].id		=ATTACK_CYOHI;//ゲージ増加時に使用するID
	aif[i].damage	=20;//ダメージ
	aif[i].kezuri	=5;//削り

	i= ATTACKINFO_FESTA_F; //フェスティバル（フィニッシュ）
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 ;
	aif[i].id		=ATTACK_CYOHI;//ゲージ増加時に使用するID
	aif[i].damage	=500;//ダメージ
	aif[i].kezuri	=10;//削り

	i=ATTACKINFO_NAGE;//投げ
	aif[i].hit		=HITINFO_MARK3 | HITINFO_REACT3 | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=60;//ダメージ
	aif[i].kezuri	=0;//削り
	aif[i].muki		=TRUE;

	if(OptHeavyHit())
	{
		for(i=0;i<AIF_NUM;i++)
		{
			aif[i].damage = (DWORD)(aif[i].damage*1.2f);
			aif[i].kezuri = (DWORD)(aif[i].kezuri*1.2f);
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
	waz.walkf = ACTID_WALKF;
	waz.walkb = ACTID_WALKB;
	waz.jampf = ACTID_JAMPF;
	waz.jampb = ACTID_JAMPB;
	waz.dashf = ACTID_DASHF;
	waz.dashb = ACTID_DASHB;
	waz.att_short[0] = ACTID_ATT_SA;//短リーチ攻撃
	waz.att_short[1] = ACTID_ATT_CA;
	waz.att_short[2] = ACTID_ATT_CB;
	waz.att_short[3] = ACTID_WASSYORYUKEN2;
	waz.att_short[4] = ACTID_ATT_SMC;
	waz.att_short[5] = ACTID_GCANCEL1;
	waz.att_middle[0] = ACTID_SYARA1;//中リーチ攻撃
	waz.att_middle[1] = ACTID_ATT_CC;
	waz.att_middle[2] = ACTID_SYARA2;
	waz.att_middle[3] = ACTID_ATT_SB;
	waz.att_long[0] = ACTID_ATT_SC;//長リーチ攻撃
	waz.att_long[1] = ACTID_SYARA3;
	waz.att_long[2] = ACTID_FESTIVAL;
	waz.att_jamp[0] = ACTID_ATT_JC;//ジャンプ攻撃
	waz.att_jamp[1] = ACTID_ATT_JB;
	waz.att_jamp[2] = ACTID_ATT_JA;
	waz.att_tai[0] = ACTID_WASSYORYUKEN1;//対空
	waz.att_tai[1] = ACTID_WASSYORYUKEN3;
	waz.att_bullet[0] = ACTID_HADOU2;//飛び道具

	waz.nageid[0] = ACTID_NAGE1;//投げ

	//リーチ設定
	SetComReach( 0,MAAI_SHORT );
	SetComReach( 1,MAAI_MIDDLE );
	SetComReach( 2,MAAI_LONG );
}

/*--------------------------------------------------------------------------------------
	COM行動 思考ルーチン定義
----------------------------------------------------------------------------------------*/
DWORD CCharacter::ComThink()
{
	GOBJECT *pedat= GetInfo(GetGObject()->eid);
	if(pedat==NULL)return 0;

	// 行動指数
	int val_walkf = 0;
	int val_walkb = 0;
	int val_jumpf = 0;
	int val_jumpb = 0;
	int val_dashf = 0;
	int val_dashb = 0;
	int val_att_sa = 0;
	int val_att_sb = 0;
	int val_att_sc = 0;
	int val_att_smc = 0;
	int val_att_ca = 0;
	int val_att_cb = 0;
	int val_att_cc = 0;
	int val_att_ja = 0;
	int val_att_jb = 0;
	int val_att_jc = 0;
	int val_nage = 0;
	int val_nori = 0;
	int val_dive = 0;
	int val_syara = 0;
	int val_festival = 0;

	// 位置関係
	int maai = (int)abs(pedat->x - pdat->x);

	if (pdat->aid & ACTID_KUCYU)
	{
		// 空中
		if (maai < MAAI_MIDDLE)
		{
			val_att_ja += 5;
			val_att_jb += 6;
			val_att_jc += 7;
		}
	}
	else
	{
		// 地上
		if (pedat->aid & ACTID_KUCYU)
		{
			// 対空
			if (maai < MAAI_MIDDLE)
			{
				val_dashf += 5;
				val_walkb += 1;
				val_dashb += 4;
				val_dive += 6;
			}
			else
			{
				// 特に意味が感じられない
				val_walkf += 2;
				val_walkb += 4;
				val_nori += 3;
			}
		}
		else
		{
			// 地上戦
			if (maai < MAAI_SHORT)
			{
				// 近距離
				val_walkb += 1;
				val_dashb += 3;
				val_jumpb += 2;
				val_att_sa += 5;
				val_att_ca += 5;
				val_att_cb += 4;
				val_nage += 5;
				val_dive += 6;
				if (pedat->aid & ACTID_SYAGAMI)
					val_att_smc += 2;
				if (pedat->aid & ACTID_ATTACK){
					val_walkb += 2;
					val_dashb += 2;
				}
			}
			else if (maai < MAAI_MIDDLE)
			{
				// 中距離
				val_walkf += 4;
				val_dashf += 3;
				val_walkb += 4;
				val_dashb += 3;
				val_jumpf += 2;
				val_jumpb += 3;
				val_att_sb += 5;
				val_att_cc += 4;
				val_syara += 6;
				if (pedat->aid & ACTID_SYAGAMI)
					val_att_smc += 3;
			}
			else if (maai < MAAI_LONG)
			{
				// 長距離
				val_walkf += 4;
				val_dashf += 5;
				val_walkb += 4;
				val_dashb += 3;
				val_jumpf += 5;
				val_jumpb += 3;
				val_att_sc += 5;
				val_syara += 7;
				val_festival += 4;
				val_nori += 4;
			}
			else
			{
				// 超長距離
				val_walkf += 4;
				val_dashf += 6;
				val_walkb += 2;
				val_jumpf += 5;
				val_nori += 7;
				val_festival += 7;
			}
		}
	}

	// ランダム要素
	int diff = max(7-GetComLevel(), 2);		// 誤差
	val_walkf += rand() % diff;
	val_walkb += rand() % diff;
	val_jumpf += rand() % diff;
	val_jumpb += rand() % diff;
	val_dashf += rand() % diff;
	val_dashb += rand() % diff;
	val_att_sa += rand() % diff;
	val_att_sb += rand() % diff;
	val_att_sc += rand() % diff;
	val_att_smc += rand() % diff;
	val_att_ca += rand() % diff;
	val_att_cb += rand() % diff;
	val_att_cc += rand() % diff;
	val_att_ja += rand() % diff;
	val_att_jb += rand() % diff;
	val_att_jc += rand() % diff;
	val_nage += rand() % diff;
	val_nori += rand() % diff;
	val_dive += rand() % diff;
	val_syara += rand() % diff;
	val_festival += rand() % diff;

	// 無理矢理行動決定
	int highest = 0;
	DWORD now_aid = 0;

	if (val_walkf > highest)	{highest = val_walkf; now_aid = ACTID_WALKF;}
	if (val_walkb > highest)
	{
		highest = val_walkb;
		now_aid = ACTID_WALKB;
	}
	if (val_jumpf > highest)
	{
		highest = val_jumpf;
		now_aid = ACTID_JAMPF;
	}
	if (val_jumpb > highest)
	{
		highest = val_jumpb;
		now_aid = ACTID_JAMPB;
	}
	if (val_dashf > highest)
	{
		highest = val_dashf;
		now_aid = ACTID_DASHF;
	}
	if (val_dashb > highest)
	{
		highest = val_dashb;
		now_aid = ACTID_DASHB;
	}
	if (val_att_sa > highest)
	{
		highest = val_att_sa;
		now_aid = ACTID_ATT_SA;
	}
	if (val_att_sb > highest)
	{
		highest = val_att_sb;
		now_aid = ACTID_ATT_SB;
	}
	if (val_att_sc > highest)
	{
		highest = val_att_sc;
		now_aid = ACTID_ATT_SC;
	}
	if (val_att_ca > highest)
	{
		highest = val_att_ca;
		now_aid = ACTID_ATT_CA;
	}
	if (val_att_cb > highest)
	{
		highest = val_att_cb;
		now_aid = ACTID_ATT_CB;
	}
	if (val_att_cc > highest)
	{
		highest = val_att_cc;
		now_aid = ACTID_ATT_CC;
	}
	if (val_att_ja > highest)
	{
		highest = val_att_ja;
		now_aid = ACTID_ATT_JA;
	}
	if (val_att_jb > highest)
	{
		highest = val_att_jb;
		now_aid = ACTID_ATT_JB;
	}
	if (val_att_jc > highest)
	{
		highest = val_att_jc;
		now_aid = ACTID_ATT_JC;
	}
	if (val_nage > highest)
	{
		highest = val_nage;
		now_aid = ACTID_NAGE1;
	}
	if (val_nori > highest)
	{
		highest = val_nori;
		now_aid = ACTID_HADOU2;
	}
	if (val_dive > highest)
	{
		highest = val_dive;
		now_aid = ACTID_WASSYORYUKEN3;
	}
	if (val_syara > highest)
	{
		highest = val_syara;
		now_aid = ACTID_SYARA3;
	}
	if (val_festival > highest)
	{
		highest = val_festival;
		now_aid = ACTID_FESTIVAL;
	}

	return now_aid;
}
