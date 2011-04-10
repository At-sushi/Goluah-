
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
	{ OPTIONS_CHAIN_COMBO, 0, 0, "CHAIN COMBO", 5 } //記入例（消してもOK）
};


//**************************************************************************************
//  Character Information
//　ここにキャラクターの名前を記述します
//**************************************************************************************

CCharacterInfo CharInfo("キャラの名前",		// キャラクターの名前（最大31バイト）
						CDI_VERSION,		// キャラクターDLLインターフェイスのバージョン
						option,				// オプション構造体へのポインタ
						sizeof(option) / sizeof(CHARACTER_LOAD_OPTION), // オプションの数
						20,					// オプション最大ポイント数
						FALSE				// ネット対戦対応かどうか 将来の拡張のために用意されている。現在はFALSEを渡さなければならない（笑）
						DTB_VERSION);		// DLLテンプレートベースクラスのバージョン

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
}


/*--------------------------------------------------------------------------------------
	メッセージ処理
	ベースクラスで拾っていないメッセージを拾いたい場合は、ココに処理を追加します
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Message(DWORD msg,LPVOID dat,DWORD prm)
{
	switch(msg){
	case 0:return(0);//WARNINGよけ。消してもいい
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
BOOL CCharacter::Command_Hissatu(DWORD keyinfo)
{
	//必殺技コマンド判定
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_KUCYU){//空中必殺技のコマンド判定
	}
	else{//地上必殺技のコマンド判定
	}

	return CCharacterBase::Command_Hissatu(keyinfo);
}

BOOL CCharacter::Command_Normal(DWORD keyinfo)
{
	//GOBJECT* pdat = GetGObject();
	//ダッシュコマンド例
	//if(com66(30)){pdat->aid=ACTID_DASH;return TRUE;}
	//if(com44(30)){pdat->aid=ACTID_DASHB;return TRUE;}

	//キー入力で継続するタイプのダッシュの場合
	//if(keyinfo & KEYSTA_FOWORD && pdat->aid==ACTID_DASH)return TRUE;
	//if(keyinfo & KEYSTA_BACK && pdat->aid==ACTID_DASHB)return TRUE;

	return CCharacterBase::Command_Normal(keyinfo);
}

BOOL CCharacter::Command_OnNormal(DWORD keyinfo)
{
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_KUCYU){////ジャンプ動作中
	}
	else{//地上動作中
		//投げコマンド例
		//if (keyinfo & KEYSTA_FOWORD && keyinfo & KEYSTA_BC2 &&
		//	NageHantei(40) && ObjCatch(pdat->eid, GOBJMSG_NAGE)) {
		//		pdat->aid = ACTID_THROW;
		//		return TRUE;
		//}
	}

	return CCharacterBase::Command_OnNormal(keyinfo);
}

/*--------------------------------------------------------------------------------------
	GOBJMSG_ACTION メッセージ 処理
	新しい行動を追加した場合は、ここに処理を追加します。
	行動の前後に共通処理を入れたい場合は PreAction(),PostAction() をオーバーライドしてください
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Action()
{
	switch(GetGObject()->aid){
	//追加した行動への遷移をここに記述する
	//case ACTID_	:act_();break;//例

	case 0:return(0);//WARNINGよけ。消してもいい

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

/*	//設定例

	int i;

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
	aif[i].hit		=HITINFO_FUTTOBI | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 | HITINFO_EFCTSINDO;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=50;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_CROUCH_A;//しゃがみ弱攻撃
	aif[i].hit		=HITINFO_REACT1 | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1 ;//ヒット情報
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
	aif[i].id		=ATTACK_STRONG | ATTACK_EREAL;//ゲージ増加時に使用するID
	aif[i].damage	=50;//ダメージ
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
	aif[i].damage	=50;//ダメージ
	aif[i].kezuri	=0;//削り
*/
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
	CCharacterBase::InitWazInfo();//これで、歩きとかジャンプとかは設定される。あと、ジャンプ攻撃も。

	//pdat->winfo（もしくはwaz） へ行動IDを追加する
}
