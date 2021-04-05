
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
    // 記入内容：
    // 　{ フラグ, 競合するｵﾌﾟｼｮﾝ,依存するｵﾌﾟｼｮﾝ, オプション名, 消費ポイント }
    { OPTIONS_EXTRA_ATTACK	, 0, 0,						"Extra Attack"	, 5 },//追加入力技
    { OPTIONS_AERIAL_ATTACK	, 0, 0,						"Aerial Attack"	, 5 },//エリアル・アタック
    { OPTIONS_AUTO_GUARD	, 0, 0,						"Auto Guard"	, /*10*/5 },//オートガード
    { OPTIONS_HADOUKEN, 0, 0, "Hadou-Ken", 5 },//波動拳
};


//**************************************************************************************
//  Character Information
//　ここにキャラクターの名前を記述します
//**************************************************************************************

CCharacterInfo CharInfo("モナー",		// キャラクターの名前（最大31バイト）
                        CDI_VERSION,		// キャラクターDLLインターフェイスのバージョン
                        option,				// オプション構造体へのポインタ
                        sizeof(option) / sizeof(CHARACTER_LOAD_OPTION), // オプションの数
                        10,					// オプション最大ポイント数
                        FALSE);				// ネット対戦対応かどうか 将来の拡張のために用意されている。

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
//	aki3d.Initialize( GetD3DDevice(), CharDirectory);
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

    pdat->hpmax			= m_opt_hpp30 ? 1040 : 800;//最大体力値
    pdat->gaugemax		= 1;//最大ゲージ
    pdat->gaugetype		= GOBJFLGA_POWGAUGE2;//ゲージタイプ
}

//DWORD オプション指定を BOOLメンバ変数に分割する
void CCharacter::InitAnalyzeOptions()
{
    //チェーンコンボ・ON/OFF
    chainComboEnabled = TRUE;

    m_opt_exAttack	= (option_flags&OPTIONS_EXTRA_ATTACK) ? TRUE : FALSE;
    m_opt_hadou = (option_flags&OPTIONS_HADOUKEN) ? TRUE : FALSE;
    m_opt_hpp30 = FALSE;

    m_opt_AAttack	= (option_flags&OPTIONS_AERIAL_ATTACK) ? TRUE : FALSE;
    m_opt_AStep		= FALSE;
    isAutoGuard		= (option_flags&OPTIONS_AUTO_GUARD) ? TRUE : FALSE;
    m_opt_gcancel	= TRUE;
}

/*--------------------------------------------------------------------------------------
    メッセージ処理
    ベースクラスで拾っていないメッセージを拾いたい場合は、ココに処理を追加します
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Message(DWORD msg,LPVOID dat,DWORD prm)
{
    GOBJECT* pobjdat;

    //ユーザー定義ヒット
    if((msg&USERHIT_FLAG) && (msg&GOBJMSG_USERDEFINE))
    {
        return EnemyAction(prm,msg&(~GOBJMSG_USERDEFINE));
    }

    switch(msg){
    case GOBJMSG_NAGE://投げ、敵側動作
        {
            pobjdat = GetInfo(prm);
            if(pobjdat==NULL)return(0);
            return(eact_nage(pobjdat));
        }
        break;
    case GOBJMSG_HADOUHIT:		AddPowerGauge(0.10);break;
    case GOBJMSG_HADOUGUARD:	AddPowerGauge(0.05);break;
    case GOBJMSG_STRIKER://支援攻撃要請
        if(pdat->aid == ACTID_TAIKICYU){
            RemoveProperty( GOBJFLG_TOBETARGET );//ターゲットにはならない...なんでこれ外すんだっけ？(忘)
            ChangeAction( ACTID_STRIKER_IN );
            return(ALL_RIGHT);
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

//必殺技中
BOOL CCharacter::Command_OnHissatuAttacking(DWORD key_now)
{
    //レイク、追加入力コマンド判定
    if (pdat->counter >= 2) {
        if (pdat->aid == ACTID_REIKU1A){
            if (com214(30)){
                if (key_now & 0x22220000){
                    pdat->aid = ACTID_REIKU1B;
                    return TRUE;
                }
            }
        }
        else if (pdat->aid == ACTID_REIKU1B){
            if (com214(30)){
                if (key_now & 0x22220000){
                    pdat->aid = ACTID_REIKU1C;
                    return TRUE;
                }
            }
        }
        else if (pdat->aid == ACTID_REIKU2A){
            if (com214(30)){
                if (key_now & 0x22220000){
                    pdat->aid = ACTID_REIKU2B;
                    return TRUE;
                }
            }
        }
        else if (pdat->aid == ACTID_REIKU2B){
            if (com214(30)){
                if (key_now & 0x22220000){
                    pdat->aid = ACTID_REIKU2C;
                    return TRUE;
                }
            }
        }
        else if (pdat->aid == ACTID_REIKU3A){
            if (com214(30)){
                if (key_now & 0x22220000){
                    pdat->aid = ACTID_REIKU3B;
                    return TRUE;
                }
            }
        }
        else if (pdat->aid == ACTID_REIKU3B){
            if (com214(30)){
                if (key_now & 0x22220000){
                    pdat->aid = ACTID_REIKU3C;
                    return TRUE;
                }
            }
        }
    }

    return CCharacterBase::Command_OnHissatuAttacking(key_now);
}

//必殺技コマンド判定
BOOL CCharacter::Command_Hissatu(DWORD key)
{
    GOBJECT* pdat = GetGObject();

    if(pdat->aid & ACTID_KUCYU){//空中必殺技のコマンド判定
    }
    else//地上必殺技のコマンド判定
    {
        //グッバイフォーエバー
        if(com236236(40) && (key & KEYSTA_BC2) && pdat->gauge>=1.0f)
        {
            ChangeAction(ACTID_GOODBYE1);
            return TRUE;
        }
        //竜巻旋風脚みたいなもの
        if(com623(30)){
            if(key & KEYSTA_BC2){pdat->aid = ACTID_TATUMAKI3;return TRUE;}
            else if(key & KEYSTA_BB2){pdat->aid = ACTID_TATUMAKI2;return TRUE;}
            else if(key & KEYSTA_BA2){pdat->aid = ACTID_TATUMAKI1;return TRUE;}
        }
        //レイク
        if(TRUE){//OptReiku()){
            if(com214(30)){
                if(key & KEYSTA_BC2){pdat->aid = ACTID_REIKU3A;return TRUE;}
                else if(key & KEYSTA_BB2){pdat->aid = ACTID_REIKU2A;return TRUE;}
                else if(key & KEYSTA_BA2){pdat->aid = ACTID_REIKU1A;return TRUE;}
            }
        }
        if(m_opt_hadou)
        {
            //波動拳（飛ぶ）
            if(pbul_hadou->isReady() && com236(30))
            {
                if(key & KEYSTA_BC2){pdat->aid = ACTID_HADOU3B;return TRUE;}
                else if(key & KEYSTA_BB2){pdat->aid = ACTID_HADOU2B;return TRUE;}
                else if(key & KEYSTA_BA2){pdat->aid = ACTID_HADOU1B;return TRUE;}
            }
        }
        else
        {
            //波動拳（飛ばない）
            if(pbul_hadou->isReady() && com236(30)){
                if(key & KEYSTA_BC2){pdat->aid = ACTID_HADOU3A;return TRUE;}
                else if(key & KEYSTA_BB2){pdat->aid = ACTID_HADOU2A;return TRUE;}
                else if(key & KEYSTA_BA2){pdat->aid = ACTID_HADOU1A;return TRUE;}
            }
        }
    }

    return CCharacterBase::Command_Hissatu(key);
}

BOOL CCharacter::Command_Normal(DWORD key)
{
    //ダッシュコマンド
    if(com66i(30)){pdat->aid=ACTID_DASHF;return TRUE;}
    if(com44i(30)){pdat->aid=ACTID_DASHB;return TRUE;}

    //return CCharacterBase::Command_Normal(key);

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
    else//地上動作中
    {
        //投げ
        if (key & KEYSTA_FOWORD && key & KEYSTA_BC2 &&
            NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE)) {
                pdat->aid = ACTID_NAGE1;
                return TRUE;
        }

        if(m_opt_exAttack)
        {
            //レバー前入れ強
            if((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD)){
                ChangeAction(  ACTID_ATT_SE );return TRUE;
            }

            //レバー前入れ中
            if((key & KEYSTA_BB2) && (key & KEYSTA_FOWORD)){
                ChangeAction(  ACTID_ATT_SD );return TRUE;
            }
        }
    }

    return CCharacterBase::Command_OnNormal(key);
}

BOOL CCharacter::Command_OnAttacking(DWORD key)
{	
    if( pdat->aid & ACTID_KUCYU )
    {
        if(m_opt_AAttack)
        {
            //空中チェーン
            if(key & KEYSTA_BC2){if(ChainCombo(CHAIN_JC)){pdat->aid = ACTID_ATT_JC;return TRUE;}}
            else if(key & KEYSTA_BB2){if(ChainCombo(CHAIN_JB)){pdat->aid = ACTID_ATT_JB;return TRUE;}}
            else if(key & KEYSTA_BA2){if(ChainCombo(CHAIN_JA)){pdat->aid = ACTID_ATT_JA;return TRUE;}}

            //2段ジャンプ
            if((key & KEYSTA_UP2) && m_2nd_step_flag)
            {
                pdat->aid = ACTID_2ND_STEP;
                return TRUE;
            }
        }
    }
    else
    {
        if(m_opt_exAttack)
        {
            //レバー前入れ強
            if((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD)){
                if(ChainCombo(CHAIN_SE)){
                    ChangeAction(  ACTID_ATT_SE );return TRUE;
                }
            }

            //レバー前入れ中
            if((key & KEYSTA_BB2) && (key & KEYSTA_FOWORD)){
                if(ChainCombo(CHAIN_SD)){
                    ChangeAction(  ACTID_ATT_SD );return TRUE;
                }
            }
        }

        //特殊キャンセル
        if((pdat->aid==ACTID_ATT_CC) && m_opt_AAttack){
            if(key & KEYSTA_UP){
                pdat->aid = ACTID_AJAMP;
                return TRUE;
            }
        }
    }

    return CCharacterBase::Command_OnAttacking(key);
}


//喰らい・ガード中
BOOL CCharacter::Command_OnSystem(DWORD key)
{
    if((pdat->aid & ACTID_GUARD) && m_opt_gcancel){
        if(!(pdat->aid & ACTID_KUCYU))
        {
            //地上ガード中
            //ガードキャンセル 竜巻旋風脚
            if(com623(30) && pdat->gauge>NEEDGAUGE_GC){
                if(key & KEYSTA_BC2){pdat->aid = ACTID_GCANCEL3;return TRUE;}
                else if(key & KEYSTA_BB2){pdat->aid = ACTID_GCANCEL2;return TRUE;}
                else if(key & KEYSTA_BA2){pdat->aid = ACTID_GCANCEL1;return TRUE;}
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
    if(wid==ACTID_GOODBYE1 && pdat->gauge<1.0f)return FALSE;//ゲージチェック

    if(wid==ACTID_GCANCEL1 || wid==ACTID_GCANCEL2 || wid==ACTID_GCANCEL3)
    {
        if(pdat->gauge<NEEDGAUGE_GC)return FALSE;
        if(!((pdat->aid&ACTID_GUARD) && m_opt_gcancel))return FALSE;
    }

    //レイク
    if(wid==ACTID_REIKU1B)return (pdat->aid==ACTID_REIKU1A) ? TRUE : FALSE;
    if(wid==ACTID_REIKU1C)return (pdat->aid==ACTID_REIKU1B) ? TRUE : FALSE;
    if(wid==ACTID_REIKU2B)return (pdat->aid==ACTID_REIKU2A) ? TRUE : FALSE;
    if(wid==ACTID_REIKU2C)return (pdat->aid==ACTID_REIKU2B) ? TRUE : FALSE;
    if(wid==ACTID_REIKU3B)return (pdat->aid==ACTID_REIKU3A) ? TRUE : FALSE;
    if(wid==ACTID_REIKU3C)return (pdat->aid==ACTID_REIKU3B) ? TRUE : FALSE;

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
        return TRUE;
    }

    return CCharacterBase::CmdCom_OnSystem(wid);
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

    // エリアルジャンプ
    if (wid == ACTID_AJAMP)
    {
        if (pdat->aid == ACTID_ATT_CC && m_opt_AAttack)
            return TRUE;
        else
            return FALSE;
    }

    if(!(pdat->aid&ACTID_KUCYU) && (m_opt_exAttack))
    {
        //チェーンコンボ判定(追加)
        if(GetGObject()->aid & ACTID_ATTACK)
        {
            switch(wid)
            {
            case ACTID_ATT_SE	:return ChainCombo(CHAIN_SE);break;
            case ACTID_ATT_SD	:return ChainCombo(CHAIN_SD);break;
            }
        }
    }

    return CCharacterBase::CmdCom_OnNormal(wid);
}

/*--------------------------------------------------------------------------------------
    GOBJMSG_TOUCHA メッセージ 処理
    ガードポイント追加
----------------------------------------------------------------------------------------*/
DWORD CCharacter::TouchA(ATTACKINFO *info,DWORD ta_eid)
{
    DWORD aid = GetGObject()->aid;
    DWORD cntr = GetGObject()->counter;

    if( CANNOT_GUARD(info->guard) ){//ガード不可
        return(CCharacterBase::TouchA(info,ta_eid));
    }

    //竜巻旋風脚
    if((aid==ACTID_TATUMAKI1) && (cntr<10))return TOUCHA_MUSI2;
    if((aid==ACTID_TATUMAKI2) && (cntr<20))return TOUCHA_MUSI2;
    if((aid==ACTID_TATUMAKI3) && (cntr<30))return TOUCHA_MUSI2;

    //レバー入れ攻撃
    if(aid==ACTID_ATT_SD){
        if(pdat->cnow==CELL_SD2 || pdat->cnow==CELL_SD3)return TOUCHA_MUSI2;
    }
    if(aid==ACTID_ATT_SE){
        if(pdat->cnow==CELL_SE3)return TOUCHA_MUSI2;
    }

    return(CCharacterBase::TouchA(info,ta_eid));
}


/*--------------------------------------------------------------------------------------
    GOBJMSG_TOUCHB メッセージ 処理
    ゲージ増加処理
----------------------------------------------------------------------------------------*/
DWORD CCharacter::TouchB(ATTACKINFO *info,BOOL hit)
{
    //特殊行動遷移
    if(info->id == ATTACK_GOODBYE && hit){
        ChangeAction(ACTID_GOODBYE2);
        cell(CELL_GOODBYE3);
    }
    if(info->id == ATTACK_STRIKER){
        ChangeAction(ACTID_STRIKER_ATTACK);
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
        case ACTID_ATT_SA:
            {
                if(ComLevelCk(4)){
                    SetComAct(ACTID_ATT_SB,3);
                }
            }
            break;
        case ACTID_ATT_SB:{
                if(ComLevelCk(5) && rand()%2){
                    if(ChainCheck(CHAIN_SD))SetComAct(ACTID_ATT_SD,5);
                    else if(ChainCheck(CHAIN_SE))SetComAct(ACTID_ATT_SE,5);
                    else SetComAct(ACTID_REIKU1C,5);
                }
            }
            break;
        case ACTID_ATT_SC:
            {
                if(ComLevelCk(5) && pdat->gauge>=1.0f)
                {
                    SetComAct(ACTID_GOODBYE1,10);
                }
                else if(ComLevelCk(4)){
                    if(ChainCheck(CHAIN_SD))SetComAct(ACTID_ATT_SD,8);
                    else if(ChainCheck(CHAIN_SE))SetComAct(ACTID_ATT_SE,8);
                    else SetComAct(ACTID_REIKU1C,5);
                }
            }
            break;

        case ACTID_ATT_SD:
            {
                if(ComLevelCk(5) && pdat->gauge>=1.0f)
                {
                    SetComAct(ACTID_GOODBYE1,10);
                }
                else if(ComLevelCk(3)){
                    if(ChainCheck(CHAIN_SE))SetComAct(ACTID_ATT_SE,8);
                    else SetComAct(ACTID_REIKU1C,8);
                }
            }
            break;
        case ACTID_ATT_SE:
            {
                if(ComLevelCk(5) && pdat->gauge>=1.0f)
                {
                    SetComAct(ACTID_GOODBYE1,10);
                }
                else if(ComLevelCk(3)){
                    if(ChainCheck(CHAIN_SD))SetComAct(ACTID_ATT_SD,8);
                    else SetComAct(ACTID_REIKU1C,8);
                }
            }
            break;

        case ACTID_ATT_CA:
            {
                if(ComLevelCk(3)){
                    SetComAct(ACTID_ATT_SB,5);
                }
            }
            break;
        case ACTID_ATT_CB:
            {
                if(ComLevelCk(4)){
                    if(ChainCheck(CHAIN_SD))SetComAct(ACTID_ATT_SD,3);
                    else if(ChainCheck(CHAIN_SE))SetComAct(ACTID_ATT_SE,3);
                    else SetComAct(ACTID_REIKU1C,5);
                }
            }
            break;
        case ACTID_ATT_CC:
            {
                if (m_opt_AAttack && ComLevelCk(3)){
                    SetComAct(ACTID_AJAMP, 3);
                }
                else if (ComLevelCk(3))
                    SetComAct(ACTID_REIKU1C,5);
            }
            break;
        case ACTID_ATT_JA:
        {
            if (m_opt_AAttack && ComLevelCk(2)){
                SetComAct(ACTID_ATT_JB, 3);
            }
            if (m_opt_AAttack && ComLevelCk(2)){
                SetComAct(ACTID_ATT_JC, 3);
            }
        }
        break;
        case ACTID_ATT_JB:{
            if (m_opt_AAttack && ComLevelCk(2)){
                SetComAct(ACTID_ATT_JC, 3);
            }
            if (m_opt_AAttack && ComLevelCk(2)){
                SetComAct(ACTID_ATT_JA, 3);
            }
        }
        break;
        case ACTID_ATT_JC:{
            if (m_opt_AAttack && ComLevelCk(2)){
                SetComAct(ACTID_ATT_JA, 3);
            }
            if (m_opt_AAttack && ComLevelCk(2)){
                SetComAct(ACTID_ATT_JB, 3);
            }
        }
        break;

        //レイク
        case ACTID_REIKU1A:
            {
                if(ComLevelCk(1)){
                    SetComAct(ACTID_REIKU1B,16);
                }
            }
            break;
        case ACTID_REIKU1B:
            {
                if(ComLevelCk(1)){
                    SetComAct(ACTID_REIKU1C,16);
                }
            }
            break;
        case ACTID_REIKU2A:
            {
                if(ComLevelCk(1)){
                    SetComAct(ACTID_REIKU2B,16);
                }
            }
            break;
        case ACTID_REIKU2B:
            {
                if(ComLevelCk(1)){
                    SetComAct(ACTID_REIKU2C,16);
                }
            }
            break;
        case ACTID_REIKU3A:
            {
                if(ComLevelCk(1)){
                    SetComAct(ACTID_REIKU3B,16);
                }
            }
            break;
        case ACTID_REIKU3B:
            {
                if(ComLevelCk(1)){
                    SetComAct(ACTID_REIKU3C,16);
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
    GOBJECT *pedat = GetInfo(tc_eid);
    if(!pedat)return CCharacterBase::TouchC(info,tc_eid);

    //屈強→屈強　禁止
    if(info==&aif[ATTACKINFO_CROUCH_C])
    {
        if(pedat->aid==ACTID_FUTTOBI2)
        {
            return TOUCHC_CANCEL;
        }
    }
    //エリアル→地上攻撃 禁止
    if(pedat->aid==ACTID_USERHIT2)
    {
        if(!(pdat->aid&ACTID_KUCYU))
        {
            return TOUCHC_CANCEL;
        }
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
    case ACTID_ATT_SD		:act_att_sd();ChainCombo(CHAIN_SD);	break;
    case ACTID_ATT_SE		:act_att_se();ChainCombo(CHAIN_SE);	break;
    case ACTID_TATUMAKI1	:act_tatumaki1(ATTACKINFO_TATUMAKI1);	break;//竜巻(弱)
    case ACTID_TATUMAKI2	:act_tatumaki2(ATTACKINFO_TATUMAKI2);	break;//竜巻(中)
    case ACTID_TATUMAKI3	:act_tatumaki3(ATTACKINFO_TATUMAKI3);	break;//竜巻(強)
    case ACTID_TATUMAKIR	:act_tatumakir();		break;//竜巻落下
    case ACTID_LANDING		:act_landing();			break;//竜巻着地
    case ACTID_REIKU1A		:act_reiku1a();			break;//レイク(弱)
    case ACTID_REIKU1B		:act_reiku1b();			break;
    case ACTID_REIKU1C		:act_reiku1c();			break;
    case ACTID_REIKU2A		:act_reiku2a();			break;//レイク(中)
    case ACTID_REIKU2B		:act_reiku2b();			break;
    case ACTID_REIKU2C		:act_reiku2c();			break;
    case ACTID_REIKU3A		:act_reiku3a();			break;//レイク(強)
    case ACTID_REIKU3B		:act_reiku3b();			break;
    case ACTID_REIKU3C		:act_reiku3c();			break;
    case ACTID_REIKU3CS		:act_reiku3cs();		break;//レイク(隙)
//	case ACTID_REIKUCOM		:act_reikucom();		break;//レイク(COM)
    case ACTID_DASHF		:act_dashf();			break;//前方ダッシュ
    case ACTID_DASHB		:act_dashb();			break;//後方ダッシュ
    case ACTID_NAGE1		:act_nage();			break;//投げ
    case ACTID_HADOU1A		:act_hadou1a();			break;//波動拳（飛ばない、弱）
    case ACTID_HADOU2A		:act_hadou2a();			break;//波動拳（飛ばない、厨）
    case ACTID_HADOU3A		:act_hadou3a();			break;//波動拳（飛ばない、強）
    case ACTID_HADOU1B		:act_hadou1b();			break;//波動拳（飛ぶ、弱）
    case ACTID_HADOU2B		:act_hadou2b();			break;//波動拳（飛ぶ、厨）
    case ACTID_HADOU3B		:act_hadou3b();			break;//波動拳（飛ぶ、強）
    case ACTID_GCANCEL1		:act_gcancel1();		break;//ガードキャンセル
    case ACTID_GCANCEL2		:act_gcancel2();		break;//ガードキャンセル
    case ACTID_GCANCEL3		:act_gcancel3();		break;//ガードキャンセル
    case ACTID_GCANCEL1J	:act_gcancel1j();		break;//ガードキャンセル
    case ACTID_GCANCEL2J	:act_gcancel2j();		break;//ガードキャンセル
    case ACTID_GCANCEL3J	:act_gcancel3j();		break;//ガードキャンセル
    case ACTID_2ND_STEP		:act_2nd_step();		break;//2段ジャンプ
    case ACTID_ATT_JA		:act_att_ja();ChainCombo(CHAIN_JA);break;//空中でもチェーンコンボをカウント
    case ACTID_ATT_JB		:act_att_jb();ChainCombo(CHAIN_JB);break;//空中でもチェーンコンボをカウント
    case ACTID_ATT_JC		:act_att_jc();ChainCombo(CHAIN_JC);break;//空中でもチェーンコンボをカウント
    case ACTID_AJAMP		:act_ajamp();			break;
    case ACTID_GOODBYE1		:act_goodbye1();		break;//グッバイ・フォーエバー
    case ACTID_GOODBYE2		:act_goodbye2();		break;
    case ACTID_STRIKER_IN	:act_striker_in();		break;//支援攻撃・画面内に入ってくる
    case ACTID_STRIKER_DASH	:act_striker_dash();	break;//支援攻撃・相手のほうにちょっと進む
    case ACTID_STRIKER_ATTACK:act_striker_attack();	break;//支援攻撃・攻撃
    case ACTID_STRIKER_OUT	:act_striker_out();		break;//支援攻撃・終了
    default:return CCharacterBase::Action();
    }
    return(TRUE);
}

void CCharacter::PreAction()
{
    if(!(pdat->aid&ACTID_KUCYU))
    {
        m_2nd_step_flag = m_opt_AStep;//2段ジャンプフラグクリア
        m_aerial = FALSE;			//エリアルフラグクリア
    }

    CCharacterBase::PreAction();
}

/*--------------------------------------------------------------------------------------
    敵側くらい行動
----------------------------------------------------------------------------------------*/
BOOL CCharacter::EnemyAction(DWORD eid,DWORD hid)
{
    GOBJECT* pedat = GetInfo(eid);
    if(!pedat)return FALSE;

    switch(hid)
    {
    case USERHIT_AERIALPOP	:eact_aerialpop(pedat);break;
    case USERHIT_AERIAL1	:eact_aerialhit1(pedat);break;
    case USERHIT_AERIAL2	:eact_aerialhit2(pedat);break;
    case USERHIT_AERIAL3	:eact_aerialhit3(pedat);break;
    default:
        pedat->aid=ACTID_FUTTOBI;
        return FALSE;
    }

    return TRUE;
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
    aif[i].damage	=20;//ダメージ
    aif[i].kezuri	=0;//削り

    i=ATTACKINFO_STAND_B;//立ち中攻撃
    aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2  | GUARDINFO_XJAMP;//ガード情報
    aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
    aif[i].damage	=30;//ダメージ
    aif[i].kezuri	=0;//削り

    i=ATTACKINFO_STAND_C;//立ち強攻撃
    aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
    aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
    aif[i].damage	=40;//ダメージ
    aif[i].kezuri	=0;//削り

    i=ATTACKINFO_CROUCH_A;//しゃがみ弱攻撃
    aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1 ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1  | GUARDINFO_XJAMP | GUARDINFO_XSTAND;//ガード情報
    aif[i].id		=ATTACK_WEAK;//ゲージ増加時に使用するID
    aif[i].damage	=20;//ダメージ
    aif[i].kezuri	=0;//削り

    i=ATTACKINFO_CROUCH_B;//しゃがみ中攻撃
    aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2  | GUARDINFO_XJAMP | GUARDINFO_XSTAND;//ガード情報
    aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
    aif[i].damage	=30;//ダメージ
    aif[i].kezuri	=0;//削り

    i=ATTACKINFO_CROUCH_C;//しゃがみ強攻撃
    aif[i].hit		=HITINFO_FUTTOBI2 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
    aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
    aif[i].damage	=40;//ダメージ
    aif[i].kezuri	=0;//削り

    i=ATTACKINFO_JAMP_A;//ジャンプ弱攻撃
    aif[i].hit		=HITINFO_REACT1 | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1 ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1  | GUARDINFO_XCROUCH;//ガード情報
    aif[i].id		=ATTACK_WEAK;//ゲージ増加時に使用するID
    aif[i].damage	=20;//ダメージ
    aif[i].kezuri	=0;//削り

    i=ATTACKINFO_JAMP_B;//ジャンプ中攻撃
    aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2  | GUARDINFO_XCROUCH;//ガード情報
    aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
    aif[i].damage	=30;//ダメージ
    aif[i].kezuri	=0;//削り

    i=ATTACKINFO_JAMP_C;//ジャンプ強攻撃
    aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH;//ガード情報
    aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
    aif[i].damage	=40;//ダメージ
    aif[i].kezuri	=0;//削り

    //・追加入力技

    i=ATTACKINFO_STAND_D;//立ち中、レバー前入れ
    aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
    aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
    aif[i].damage	=50;//ダメージ
    aif[i].kezuri	=0;//削り

    i=ATTACKINFO_STAND_E;//立ち強、レバー前入れ
    aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
    aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
    aif[i].damage	=50;//ダメージ
    aif[i].kezuri	=0;//削り

    //・竜巻旋風客みたいな技

    i=ATTACKINFO_TATUMAKI1;//(弱)
    aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 | GUARDINFO_XJAMP;//ガード情報
    aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
    aif[i].damage	=40;//ダメージ
    aif[i].kezuri	=4;//削り
    
    i=ATTACKINFO_TATUMAKI2;//(中)
    aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 ;//ガード情報
    aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
    aif[i].damage	=40;//ダメージ
    aif[i].kezuri	=4;//削り

    i=ATTACKINFO_TATUMAKI3;//(強)
    aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 ;//ガード情報
    aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
    aif[i].damage	=40;//ダメージ
    aif[i].kezuri	=4;//削り

    //・レイク

    i= ATTACKINFO_REIKU;//レイク
    aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
    aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
    aif[i].damage	=25;//ダメージ
    aif[i].kezuri	=1;//削り

    i= ATTACKINFO_REIKU_FINISH;//レイク・フィニッシュ
    aif[i].hit		=HITINFO_FUTTOBI | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
    aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
    aif[i].damage	=30;//ダメージ
    aif[i].kezuri	=2;//削り

    //・投げ
    i = ATTACKINFO_NAGE;
    aif[i].id		= 0;
    aif[i].damage	= 60;
    aif[i].hit		= HITINFO_MARK3 | HITINFO_SNDHIT3;

    //・波動拳（各飛道具クラスにコピーされる）

    //飛ばない系
    i=ATTACKINFO_HADOU1A;
    aif[i].hit		=HITINFO_FUTTOBI | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
    aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
    aif[i].damage	=90;//ダメージ
    aif[i].kezuri	=7;//削り

    i=ATTACKINFO_HADOU2A;
    aif[i].hit		=HITINFO_FUTTOBI | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
    aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
    aif[i].damage	=95;//ダメージ
    aif[i].kezuri	=7;//削り
    
    i=ATTACKINFO_HADOU3A;
    aif[i].hit		=HITINFO_FUTTOBI | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
    aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
    aif[i].damage	=100;//ダメージ
    aif[i].kezuri	=7;//削り

    //飛ぶ系
    i=ATTACKINFO_HADOU1B;
    aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
    aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
    aif[i].damage	=50;//ダメージ
    aif[i].kezuri	=5;//削り

    i=ATTACKINFO_HADOU2B;
    aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
    aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
    aif[i].damage	=60;//ダメージ
    aif[i].kezuri	=5;//削り
    
    i=ATTACKINFO_HADOU3B;
    aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
    aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
    aif[i].damage	=70;//ダメージ
    aif[i].kezuri	=5;//削り

    //・グッバイ・フォーエバー

    i= ATTACKINFO_GOODBYE1;
    aif[i].hit		=HITINFO_REACT3A | HITINFO_STOP | HITINFO_MARK4 | HITINFO_SNDHIT3 | HITINFO_EFCTSINDO | HITINFO_EFCTFLASH ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP | GUARDINFO_EFCTSINDO;//ガード情報
    aif[i].id		=ATTACK_GOODBYE;//ゲージ増加時に使用するID
    aif[i].damage	=150;//ダメージ
    aif[i].kezuri	=20;//削り

    i= ATTACKINFO_GOODBYE2;
    aif[i].hit		=HITINFO_FUTTOBI2 | HITINFO_STOP | HITINFO_MARK4 | HITINFO_SNDHIT3  | HITINFO_EFCTSINDO ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP | GUARDINFO_EFCTSINDO;//ガード情報
    aif[i].id		=0;//ゲージ増加時に使用するID
    aif[i].damage	=80;//ダメージ
    aif[i].kezuri	=5;//削り

    //エリアル

    i=ATTACKINFO_AERIALPOP;//浮かせ
    aif[i].hit		=HITINFO_USER(USERHIT_AERIALPOP) | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
    aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
    aif[i].damage	=50;//ダメージ
    aif[i].kezuri	=0;//削り

    i=ATTACKINFO_AERIAL_A;//弱攻撃
    aif[i].hit		=HITINFO_USER(USERHIT_AERIAL1) | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1 ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1  | GUARDINFO_XCROUCH;//ガード情報
    aif[i].id		=ATTACK_WEAK;//ゲージ増加時に使用するID
    aif[i].damage	=20;//ダメージ
    aif[i].kezuri	=0;//削り

    i=ATTACKINFO_AERIAL_B;//ジャンプ中攻撃
    aif[i].hit		=HITINFO_USER(USERHIT_AERIAL2) | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2  | GUARDINFO_XCROUCH;//ガード情報
    aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
    aif[i].damage	=30;//ダメージ
    aif[i].kezuri	=0;//削り

    i=ATTACKINFO_AERIAL_C;//ジャンプ強攻撃
    aif[i].hit		=HITINFO_USER(USERHIT_AERIAL3) | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH;//ガード情報
    aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
    aif[i].damage	=40;//ダメージ
    aif[i].kezuri	=0;//削り

    //ストライカー時

    i=ATTACKINFO_STRIKER_DASH;//ミニダッシュ
    aif[i].hit		=HITINFO_REACT3A | HITINFO_SIV1 | HITINFO_MARK3 | HITINFO_SNDHIT3;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1;//ガード情報
    aif[i].id		=ATTACK_STRIKER;//ゲージ増加時に使用するID
    aif[i].damage	=0;//ダメージ
    aif[i].kezuri	=0;//削り

    i=ATTACKINFO_STRIKER;//旋風脚
    aif[i].hit		=HITINFO_REACT2A | HITINFO_SIV1 | HITINFO_MARK3 | HITINFO_SNDHIT3;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV1;//ガード情報
    aif[i].id		=0;//ゲージ増加時に使用するID
    aif[i].damage	=10;//ダメージ
    aif[i].kezuri	=0;//削り

    //ガードキャンセル用
    aif[ATTACKINFO_GCANCEL] = aif[ATTACKINFO_TATUMAKI2];
    i=ATTACKINFO_GCANCEL;
    aif[i].id		=0;//ゲージ増加時に使用するID
    aif[i].kezuri	=0;//削り

    i=ATTACKINFO_ROCKET;//ロケット
    aif[i].hit		=HITINFO_EFCTSINDO | HITINFO_EFCTBURN | HITINFO_SIV3 | HITINFO_FUTTOBI | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
    aif[i].guard	=GUARDINFO_EFCTSINDO | GUARDINFO_SIV3 | GUARDINFO_REACT3| GUARDINFO_XJAMP ;//ガード情報
    aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
    aif[i].damage	=250;//ダメージ
    aif[i].kezuri	=25;//削り

    i=ATTACKINFO_ERASER99;//イレイサー
    aif[i].hit		=HITINFO_EFCTBURN_B | HITINFO_EFCTSINDO | HITINFO_SIV1 | HITINFO_FUTTOBI | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
    aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1;//ガード情報
    aif[i].id		=0;//ゲージ増加時に使用するID
    aif[i].damage	=0;//ダメージ
    aif[i].kezuri	=0;//削り

    //チェーンコンボOFF時は、少しダメージをおまけしてやる
    if(!chainComboEnabled){
        for(i=0;i<MAXNUM_ATTACKINFO;i++){
            aif[i].damage = (DWORD)(aif[i].damage * 1.2);
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

void CCharacter::EndAttack()
{
    GetGObject()->kougeki = FALSE;
}

/*--------------------------------------------------------------------------------------
    COM行動用 技情報 初期化
----------------------------------------------------------------------------------------*/
void CCharacter::InitWazInfo()//コンピュータ用技情報の設定
{
    CCharacterBase::InitWazInfo();//これで、歩きとかジャンプとかは設定される。あと、ジャンプ攻撃も。

    waz.dashf = ACTID_DASHF;
    waz.dashb = ACTID_DASHB;

    //pdat->winfo へ行動IDを追加する
    waz.att_short[0]  = ACTID_ATT_SA;//短リーチ攻撃
    waz.att_short[1]  = ACTID_ATT_CA;
    waz.att_short[2]  = ACTID_ATT_SB;
    waz.att_short[3]  = ACTID_ATT_CB;
    waz.att_short[4]  = ACTID_GCANCEL2;
    waz.att_short[5]  = ACTID_ATT_SC;
    waz.att_middle[0] = ACTID_ATT_SC;//中リーチ攻撃
    waz.att_middle[1] = ACTID_ATT_CC;
    waz.att_middle[2] = ACTID_ATT_SD;
    waz.att_middle[3] = ACTID_ATT_SE;
    waz.att_middle[4] = ACTID_REIKU1A;
    waz.att_long[0]   = ACTID_ATT_SD;//長リーチ攻撃 
    waz.att_long[1]   = ACTID_ATT_SE;
    waz.att_long[2]   = ACTID_REIKU2A;
    waz.att_long[3]   = ACTID_REIKU3A;
    waz.att_long[4]   = ACTID_GOODBYE1;

    waz.att_tai[0] = ACTID_ATT_CC;
    waz.att_tai[1] = ACTID_TATUMAKI1;
    waz.att_tai[2] = ACTID_TATUMAKI2;
    waz.att_tai[3] = ACTID_TATUMAKI3;

    waz.nageid[0] = ACTID_NAGE1;

    //リーチ設定
    SetComReach( 0,MAAI_SHORT );
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
