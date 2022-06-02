
/*=====================================================================================

        キャラクター定義

        Goluah!! Copyright (C) 2001-2004 aki, 2014-2015 logger, 2004-2015 At-sushi

        This program is free software; you can redistribute it and/or modify it under the terms of the GNU
General Public License as published by the Free Software Foundation; either version 2 of the License, or (at
your option) any later version.

        This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
License for more details.

        You should have received a copy of the GNU General Public License along with this program; if not,
write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

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
    {OPTIONS_REVERSAL, 0, 0, "Reversal Attack", 5},
    {OPTIONS_QUICK_CHARGE, OPTIONS_STOCKABLE_GAUGE, 0, "Quick Charge", 7},
    {OPTIONS_STOCKABLE_GAUGE, OPTIONS_QUICK_CHARGE, 0, "Stockable Gauge", 7},
};

//**************************************************************************************
//  Character Information
//　ここにキャラクターの名前を記述します
//**************************************************************************************

CCharacterInfo CharInfo(
    "ギコ",      // キャラクターの名前（最大31バイト）
    CDI_VERSION, // キャラクターDLLインターフェイスのバージョン
    option,      // オプション構造体へのポインタ
    sizeof(option) / sizeof(CHARACTER_LOAD_OPTION), // オプションの数
    15,                                             // オプション最大ポイント数
    FALSE); // ネット対戦対応かどうか 将来の拡張のために用意されている。現在は0を渡さなければならない（笑）

char *CharDirectory = NULL;

//設定マクロ。CCharacterInfo変数名↑と、キャラクタークラス名を指定します
SET_CHARACTERINFO(CharInfo, CCharacter)

/*--------------------------------------------------------------------------------------
        キャラクター生成処
        通常、いじる必要ありません
----------------------------------------------------------------------------------------*/

CCharacter::CCharacter(CDI_CHARACTERINFO2 *info) : CCharacterBase(info) {
  CharDirectory = info->dir; // キャラクターのフォルダを所得

  // aki用エフェクト初期化
  aki3d.Initialize(CGoluahObject::GetD3DDevice(), CharDirectory);
}

/*--------------------------------------------------------------------------------------
        キャラクター破棄処理
        通常、いじる必要ありません
----------------------------------------------------------------------------------------*/
CCharacter::~CCharacter() {}

/*--------------------------------------------------------------------------------------
        パラメータ初期化
        ここに、各種パラメータの初期化処理を追加します
----------------------------------------------------------------------------------------*/
void CCharacter::InitParameters() {
  //オプション値解釈
  m_opt_reversal = (option_flags & OPTIONS_REVERSAL) ? TRUE : FALSE;
  m_opt_gcancel = TRUE;
  m_opt_quick_ch = (option_flags & OPTIONS_QUICK_CHARGE) ? TRUE : FALSE;
  m_opt_hevy_hit = (option_flags & OPTIONS_HEAVY_HIT) ? TRUE : FALSE;

  BOOL st_gauge = (option_flags & OPTIONS_STOCKABLE_GAUGE) ? TRUE : FALSE;
  chainComboEnabled = TRUE;
  BOOL iron = FALSE;

  pdat->hpmax = iron ? 1200 : 1000;                                     //最大体力値
  pdat->gaugemax = st_gauge ? 4 : 1;                                    //最大ゲージ
  pdat->gaugetype = st_gauge ? GOBJFLGA_POWGAUGE1 : GOBJFLGA_POWGAUGE2; //ゲージタイプ
}

/*--------------------------------------------------------------------------------------
        メッセージ処理
        ベースクラスで拾っていないメッセージを拾いたい場合は、ココに処理を追加します
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Message(DWORD msg, LPVOID dat, DWORD prm) {
  GOBJECT *pobjdat = NULL;
  if (msg & GOBJMSG_USERDEFINE) {
    pobjdat = GetInfo(prm);
    if (pobjdat == NULL)
      return (0);

    switch (msg) {
    case GOBJMSG_NAGE:
      return eact_nage(pobjdat);
    case GOBJMSG_ATEMI_NAGE:
      return eact_atemi_nage(pobjdat);
    case GOBJMSG_FUTTOBI:
      return eact_rotfuttobi(pobjdat);
    }
    return TRUE;
  }

  switch (msg) {
  case GOBJMSG_STRIKER: //支援攻撃要請
    if (pdat->aid == ACTID_TAIKICYU) {
      RemoveProperty(GOBJFLG_TOBETARGET); //ターゲットにはならない...なんでこれ外すんだっけ？(忘)
      ChangeAction(ACTID_STRIKER_IN);
      return (YEAH);
    }
    return (0);
  default:
    return (CCharacterBase::Message(msg, dat, prm)); //キャラクター基本クラスに処理を任せる
  }
  return (TRUE);
}

/*--------------------------------------------------------------------------------------
        GOBJMSG_TOUCHA(攻撃喰らい)
----------------------------------------------------------------------------------------*/
DWORD CCharacter::TouchA(ATTACKINFO *info, DWORD eid) {
  GOBJECT *einfo;

  //当て身投げ処理
  if (pdat->aid == ACTID_ATEMI1 || pdat->aid == ACTID_ATEMI2) {
    DWORD efctid;

    //敵情報取得
    einfo = GetInfo(eid);
    if (!einfo) {
      OutputDebugString("当身:info取得失敗\n");
      return CCharacterBase::TouchA(info, eid);
    }

    /*	//相手がキャラクターか？
            if(FALSE){//(einfo->objtype&GOBJFLG_ZCHARACTER)==0){
                    OutputDebugString("当身:相手がキャラクタでない\n");
                    return CCharacterBase::TouchA(info,eid);
            }

            //相手が投げられ判定を持っているか？
            if((einfo->objtype&GOBJFLG_NAGERARE)==0){
                    OutputDebugString("当身:投げられ判定なし(1)\n");
                    return CCharacterBase::TouchA(info,eid);
            }
            if(!einfo->nagerare){
                    OutputDebugString("当身:投げられ判定なし(2)\n");
                    return CCharacterBase::TouchA(info,eid);
            }*/

    //削り値は体力以上か？
    if (info->kezuri > (DWORD)pdat->hp) {
      OutputDebugString("当身:HP不足\n");
      return TOUCHA_GUARDS;
    }

    efctid = (info->kezuri != 0) ? EFCTID_MARKG2 : EFCTID_MARKG;

    if (pdat->aid == ACTID_ATEMI1) {
      //立ちガードできないものは受け取れない
      if (info->guard & GUARDINFO_XSTAND) {
        OutputDebugString("当身:立ちガード不能\n");
        return CCharacterBase::TouchA(info, eid);
      }

      //投げてみる
      if (!ObjCatch(eid, GOBJMSG_ATEMI_NAGE)) {
        OutputDebugString("当身:投げ失敗\n");
        return CCharacterBase::TouchA(info, eid);
      }

      //成功。ガードマークを表示する
      AddEffect(efctid, (int)(pdat->x + zurex(35)), -60, pdat->muki);
    } else if (pdat->aid == ACTID_ATEMI2) {
      //しゃがみガードできないものは受け取れない
      if (info->guard & GUARDINFO_XCROUCH) {
        OutputDebugString("当身:屈ガード不能\n");
        return CCharacterBase::TouchA(info, eid);
      }

      //投げてみる
      if (!ObjCatch(eid, GOBJMSG_ATEMI_NAGE)) {
        OutputDebugString("当身:投げ失敗\n");
        return CCharacterBase::TouchA(info, eid);
      }

      //成功。ガードマークを表示する
      AddEffect(efctid, (int)(pdat->x + zurex(35)), -30, pdat->muki);
    }

    AddEffect(EFCTID_STOP, 8, 0, 0); //ヒットストップ エフェクト
    PlaySysSound(SYSTEMSOUND_GUARD); //ガード音
    pdat->hp -= info->kezuri;        //削り適用
    DrawFront();                     //前面描画
    ChangeAction(ACTID_ATEMI_NAGE);  //行動遷移

    return TOUCHA_AVOID;
  }

  return CCharacterBase::TouchA(info, eid);
}

/*--------------------------------------------------------------------------------------
        GOBJMSG_TOUCHB(攻撃あたり)
----------------------------------------------------------------------------------------*/
DWORD CCharacter::TouchB(ATTACKINFO *info, BOOL hit) {
  //特殊行動遷移
  if (info->id == ATTACK_STRIKER) {
    ChangeAction(ACTID_STRIKER_CHAINSAW);
  }

  double dp = 0.0f;
  switch (info->id) {
  case ATTACK_WEAK:
    dp = 0.02;
    break;
  case ATTACK_MIDDLE:
    dp = 0.03;
    break;
  case ATTACK_STRONG:
    dp = 0.04;
    break;
  case ATTACK_HISSATU:
    dp = 0.05;
    break;
  case ATTACK_ATEMI:
    dp = 0.10;
    break;
  }
  if (!hit)
    dp *= 0.5;
  if (m_opt_quick_ch)
    dp *= 1.5;
  AddPowerGauge(dp);

  if (IsLocalCom()) {
    switch (pdat->aid) {
    case ACTID_ATT_SA: {
      if (ComLevelCk(3)) {
        SetComAct(ACTID_ATT_SB, 5);
      }
    } break;
    case ACTID_ATT_SB: {
      if (ComLevelCk(2)) {
        SetComAct(ACTID_ATT_SC, 5);
      }
    } break;
    case ACTID_ATT_SC: {
      if (ComLevelCk(3)) {
        SetComAct(ACTID_KIRIGET, 3);
      }
    } break;
    case ACTID_KIRIGET: {
      if (ComLevelCk(5) && pdat->gauge >= 1.0f) {
        SetComAct(ACTID_CYOHI, 3);
      }

      if (ComLevelCk(3)) {
        SetComAct(ACTID_ATT_CA, 5);
      }
      DWORD maai = GetMaai_H(pdat->id, pdat->eid);
      if (maai > 230 && ComLevelCk(3)) {
        this->SetComAct(ACTID_TOATE2, 1);
      }
    } break;
    case ACTID_ATT_CA: {
      if (ComLevelCk(3)) {
        SetComAct(ACTID_ATT_CB, 5);
      }
    } break;
    case ACTID_ATT_CB: {
      if (ComLevelCk(3)) {
        SetComAct(ACTID_ATT_CC, 5);
      }
    } break;
    case ACTID_ATT_CC: {
      DWORD maai = GetMaai_H(pdat->id, pdat->eid);
      if (maai > 230 && ComLevelCk(3)) {
        this->SetComAct(ACTID_TOATE2, 1);
      }
    } break;
    }
  }

  return CCharacterBase::TouchB(info, hit);
}

/*--------------------------------------------------------------------------------------
        コマンド判定 処理
        GOBJMSG_COMMAND メッセージに対応します。現在の行動によって複数の関数に
        処理が分かれています。適切なところに判定処理を追加してください。
----------------------------------------------------------------------------------------*/
BOOL CCharacter::Command_Hissatu(DWORD key) {
  if (pdat->aid & ACTID_KUCYU) { //空中必殺技のコマンド判定
  } else                         //地上必殺技のコマンド判定
  {
    //超必
    if (com1632143(50)) {
      if ((key & KEYSTA_BC2) && pdat->gauge >= 1.0) {
        pdat->aid = ACTID_CYOHI;
        return TRUE;
      }
    }

    //当て身投げ
    if (com426(30)) {
      if (key & KEYSTA_BC2) {
        pdat->aid = ACTID_ATEMI3;
        return TRUE;
      } else if (key & KEYSTA_BB2) {
        pdat->aid = ACTID_ATEMI2;
        return TRUE;
      } else if (key & KEYSTA_BA2) {
        pdat->aid = ACTID_ATEMI1;
        return TRUE;
      }
    }

    // ageタックル
    if (com623(30)) {
      if (key & KEYSTA_BC2) {
        pdat->aid = ACTID_AGETACKLE3;
        return TRUE;
      } else if (key & KEYSTA_BB2) {
        pdat->aid = ACTID_AGETACKLE2;
        return TRUE;
      } else if (key & KEYSTA_BA2) {
        pdat->aid = ACTID_AGETACKLE1;
        return TRUE;
      }
    }

    //遠当て
    if (com421(30) && pbul_toate->isReady()) {
      if (key & KEYSTA_BC2) {
        pdat->aid = ACTID_TOATE3;
        return TRUE;
      } else if (key & KEYSTA_BB2) {
        pdat->aid = ACTID_TOATE2;
        return TRUE;
      } else if (key & KEYSTA_BA2) {
        pdat->aid = ACTID_TOATE1;
        return TRUE;
      }
    }
  }

  return CCharacterBase::Command_Hissatu(key);
}

BOOL CCharacter::Command_Normal(DWORD key) {
  //ダッシュコマンド
  if (com66i(30) && (key & KEYSTA_FOWORD2)) {
    pdat->aid = ACTID_DASHF;
    return TRUE;
  }
  if (com44i(30) && (key & KEYSTA_BACK2)) {
    pdat->aid = ACTID_DASHB;
    return TRUE;
  }

  // return CCharacterBase::Command_Normal(key);

  //基本通常動作
  if (key & KEYSTA_UP) {
    pdat->aid = ACTID_JAMPS;
  } else if (key & KEYSTA_DOWN) { //しゃがみ
    pdat->aid = ACTID_CROUCH;
  } else if (key & KEYSTA_FOWORD) { //前歩き
    if (key & KEYSTA_FOWORD && pdat->aid == ACTID_DASHF)
      return TRUE; //ダッシュ継続
    pdat->aid = ACTID_WALKF;
  } else if (key & KEYSTA_BACK) { //後ろ歩き
    pdat->aid = ACTID_WALKB;
  } else { //ニュートラルポーズ
    pdat->aid = ACTID_NEUTRAL;
  }
  return FALSE;
}

BOOL CCharacter::Command_OnNormal(DWORD key) {
  if (pdat->aid & ACTID_KUCYU) { ////ジャンプ動作中
  } else {                       //地上動作中
    //投げ
    if (key & KEYSTA_FOWORD && key & KEYSTA_BC2 && NageHantei(MAAI_NAGE) &&
        ObjCatch(pdat->eid, GOBJMSG_NAGE)) {
      pdat->aid = ACTID_NAGE1;
      return TRUE;
    }

    //キリバンゲット（＼+C）
    if ((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD) && (key & KEYSTA_DOWN)) {
      ChangeAction(ACTID_KIRIGET);
      return TRUE;
    }
    // sageナックル（→+B）
    if ((key & KEYSTA_BB2) && (key & KEYSTA_FOWORD) && !(key & KEYSTA_DOWN)) {
      ChangeAction(ACTID_SAGENA);
      return TRUE;
    }
  }

  return CCharacterBase::Command_OnNormal(key);
}

BOOL CCharacter::Command_OnAttacking(DWORD key) {
  if (!(pdat->aid & ACTID_KUCYU)) {
    //キリバンゲット（＼+C）
    if ((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD) && (key & KEYSTA_DOWN)) {
      if (ChainCombo(CHAIN_KIRIGET)) {
        ChangeAction(ACTID_KIRIGET);
        return TRUE;
      }
    }
    // sageナックル（→+B）
    if ((key & KEYSTA_BB2) && (key & KEYSTA_FOWORD) && !(key & KEYSTA_DOWN)) {
      if (ChainCombo(CHAIN_SAGENA)) {
        ChangeAction(ACTID_SAGENA);
        return TRUE;
      }
    }
  }

  return CCharacterBase::Command_OnAttacking(key);
}

BOOL CCharacter::Command_OnSystem(DWORD key) {
  if (!(pdat->aid & ACTID_KUCYU)) {
    //ガーキャン
    if (m_opt_gcancel) {
      if ((pdat->aid & ACTID_GUARD) && pdat->gauge > NEEDGAUGE_GC) {
        if (key & KEYSTA_BC2) {
          ChangeAction(ACTID_GCANCEL3);
          return TRUE;
        } else if (key & KEYSTA_BB2) {
          ChangeAction(ACTID_GCANCEL2);
          return TRUE;
        } else if (key & KEYSTA_BA2) {
          ChangeAction(ACTID_GCANCEL1);
          return TRUE;
        }
      }
    }
  }
  //リバーサル
  if (pdat->aid == ACTID_OKIAGARI && m_opt_reversal) {
    if (ccounter == 10 && pdat->gauge > NEEDGAUGE_REV) {
      if (key & KEYSTA_UP) {
        ChangeAction(ACTID_REVERSAL);
        return TRUE;
      }
    }
  }
  return CCharacterBase::Command_OnSystem(key);
}

/*--------------------------------------------------------------------------------------
        コマンド判定 処理(COM)
----------------------------------------------------------------------------------------*/
DWORD CCharacter::CommandCOM(DWORD wid) {
  if (wid == ACTID_CYOHI && pdat->gauge < 1.0f)
    return FALSE; //ゲージチェック

  if (wid == ACTID_GCANCEL1 || wid == ACTID_GCANCEL2 || wid == ACTID_GCANCEL3) {
    if (pdat->gauge < NEEDGAUGE_GC)
      return FALSE;
    if (!((pdat->aid & ACTID_GUARD) && m_opt_gcancel))
      return FALSE;
  }

  return CCharacterBase::CommandCOM(wid);
}

DWORD CCharacter::CmdCom_OnSystem(DWORD wid) {
  //ガーキャン
  if (wid == ACTID_GCANCEL1 || wid == ACTID_GCANCEL2 || wid == ACTID_GCANCEL3) {
    if (pdat->aid & ACTID_KUCYU) {
      return FALSE;
    }
    return FALSE;
  }

  return CCharacterBase::CmdCom_OnSystem(wid);
}

DWORD CCharacter::CmdCom_OnNormal(DWORD wid) {
  if (!(pdat->aid & ACTID_KUCYU)) {
    //投げ
    if (wid == ACTID_NAGE1 && NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE)) {
      pdat->aid = ACTID_NAGE1;
      return TRUE;
    }
    //チェーンコンボ判定(追加)
    if (GetGObject()->aid & ACTID_ATTACK) {
      switch (wid) {
      case ACTID_KIRIGET:
        return ChainCombo(CHAIN_KIRIGET);
        break;
      case ACTID_SAGENA:
        return ChainCombo(CHAIN_SAGENA);
        break;
      }
    }
  }

  // 対空技
  if (wid == ACTID_AGETACKLE1 || wid == ACTID_AGETACKLE2 || wid == ACTID_AGETACKLE3)
    return TRUE;

  return CCharacterBase::CmdCom_OnNormal(wid);
}

/*--------------------------------------------------------------------------------------
        GOBJMSG_ACTION メッセージ 処理
        新しい行動を追加した場合は、ここに処理を追加します。
        行動の前後に共通処理を入れたい場合は PreAction(),PostAction() をオーバーライドしてください
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Action() {
  switch (GetGObject()->aid) {
  case ACTID_NAGE1:
    act_nage();
    break;
  case ACTID_DASHF:
    act_dashf();
    break;
  case ACTID_DASHB:
    act_dashb();
    break;
  case ACTID_AGETACKLE1:
    act_agetackle1(ATTACKINFO_AGETACKLE1);
    break;
  case ACTID_AGETACKLE2:
    act_agetackle2(ATTACKINFO_AGETACKLE2);
    break;
  case ACTID_AGETACKLE3:
    act_agetackle3(ATTACKINFO_AGETACKLE3);
    break;
  case ACTID_AGETACKLESUKI:
    act_agetacklesuki();
    break;
  case ACTID_KIRIGET:
    act_kiriget();
    ChainCombo(CHAIN_KIRIGET);
    break;
  case ACTID_SAGENA:
    act_sagena();
    ChainCombo(CHAIN_SAGENA);
    break;
  case ACTID_SAGENASUKI:
    act_sagenasuki();
    break;
  case ACTID_ATEMI1:
    act_atemi1();
    break;
  case ACTID_ATEMI2:
    act_atemi2();
    break;
  case ACTID_ATEMI3:
    act_atemi3();
    break;
  case ACTID_ATEMI_SUKI:
    act_atemi_suki();
    break;
  case ACTID_ATEMI_NAGE:
    act_atemi_nage();
    break;
  case ACTID_CYOHI:
    act_cyohi();
    break;
  case ACTID_TOATE1:
    act_toate1();
    break;
  case ACTID_TOATE2:
    act_toate2();
    break;
  case ACTID_TOATE3:
    act_toate3();
    break;
  case ACTID_STRIKER_IN:
    act_striker_in();
    break;
  case ACTID_STRIKER_2GET:
    act_striker_2get();
    break;
  case ACTID_STRIKER_CHAINSAW:
    act_striker_chainsaw();
    break;
  case ACTID_STRIKER_OUT:
    act_striker_out();
    break;
  case ACTID_GCANCEL1:
    act_gcancel1();
    break;
  case ACTID_GCANCEL2:
    act_gcancel2();
    break;
  case ACTID_GCANCEL3:
    act_gcancel3();
    break;
  case ACTID_REVERSAL:
    act_reversal();
    break;
  default: //それ以外はキャラクター基本クラスに任せる
    return CCharacterBase::Action();
  }

  return (TRUE);
}

void CCharacter::PreAction() {
  // COMリバーサル
  if (pdat->aid == ACTID_OKIAGARI && m_opt_reversal) {
    if (ccounter == 10) {
      if (IsLocalCom() && pdat->gauge > NEEDGAUGE_REV) {
        if (GetMaai_H(pdat->id, pdat->eid) < 50) {
          if (rand() % 3) {
            ChangeAction(ACTID_REVERSAL);
          }
        }
      }
    }
  }

  CCharacterBase::PreAction();
}

/*--------------------------------------------------------------------------------------
        攻撃力情報初期化
----------------------------------------------------------------------------------------*/
void CCharacter::InitAttackInfo() {
  ZeroMemory(aif, sizeof(aif));

  int i;

  //・立ち

  i = ATTACKINFO_STAND_A;                                                       //立ち弱攻撃
  aif[i].hit = HITINFO_REACT1 | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1; //ヒット情報
  aif[i].guard = GUARDINFO_REACT1 | GUARDINFO_SIV1 | GUARDINFO_XJAMP;           //ガード情報
  aif[i].id = ATTACK_WEAK; //ゲージ増加時に使用するID
  aif[i].damage = 30;      //ダメージ
  aif[i].kezuri = 0;       //削り

  i = ATTACKINFO_STAND_B;                                                       //立ち中攻撃
  aif[i].hit = HITINFO_REACT2 | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2; //ヒット情報
  aif[i].guard = GUARDINFO_REACT2 | GUARDINFO_SIV2 | GUARDINFO_XJAMP;           //ガード情報
  aif[i].id = ATTACK_MIDDLE; //ゲージ増加時に使用するID
  aif[i].damage = 40;        //ダメージ
  aif[i].kezuri = 0;         //削り

  i = ATTACKINFO_STAND_C;                                        //立ち強攻撃
  aif[i].hit = HITINFO_MARK2 | HITINFO_REACT2 | HITINFO_SNDHIT3; //ヒット情報
  aif[i].guard = GUARDINFO_REACT3 | GUARDINFO_XJAMP;             //ガード情報
  aif[i].id = ATTACK_STRONG;                                     //ゲージ増加時に使用するID
  aif[i].damage = 25;                                            //ダメージ
  aif[i].kezuri = 0;                                             //削り

  //・屈

  i = ATTACKINFO_CROUCH_A;                                                      //しゃがみ弱攻撃
  aif[i].hit = HITINFO_REACT1 | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1; //ヒット情報
  aif[i].guard = GUARDINFO_REACT1 | GUARDINFO_SIV1 | GUARDINFO_XJAMP | GUARDINFO_XSTAND; //ガード情報
  aif[i].id = ATTACK_WEAK; //ゲージ増加時に使用するID
  aif[i].damage = 30;      //ダメージ
  aif[i].kezuri = 0;       //削り

  i = ATTACKINFO_CROUCH_B;                                                      //しゃがみ中攻撃
  aif[i].hit = HITINFO_REACT2 | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2; //ヒット情報
  aif[i].guard = GUARDINFO_REACT2 | GUARDINFO_SIV2 | GUARDINFO_XJAMP;           //ガード情報
  aif[i].id = ATTACK_MIDDLE; //ゲージ増加時に使用するID
  aif[i].damage = 40;        //ダメージ
  aif[i].kezuri = 0;         //削り

  i = ATTACKINFO_CROUCH_C;                                                      //しゃがみ強攻撃
  aif[i].hit = HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3; //ヒット情報
  aif[i].guard = GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;           //ガード情報
  aif[i].id = ATTACK_STRONG; //ゲージ増加時に使用するID
  aif[i].damage = 50;        //ダメージ
  aif[i].kezuri = 0;         //削り

  //・飛

  i = ATTACKINFO_JAMP_A;                                                        //ジャンプ弱攻撃
  aif[i].hit = HITINFO_REACT1 | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1; //ヒット情報
  aif[i].guard = GUARDINFO_REACT1 | GUARDINFO_SIV1 | GUARDINFO_XCROUCH;         //ガード情報
  aif[i].id = ATTACK_WEAK; //ゲージ増加時に使用するID
  aif[i].damage = 30;      //ダメージ
  aif[i].kezuri = 0;       //削り

  i = ATTACKINFO_JAMP_B;                                                        //ジャンプ中攻撃
  aif[i].hit = HITINFO_REACT2 | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2; //ヒット情報
  aif[i].guard = GUARDINFO_REACT2 | GUARDINFO_SIV2 | GUARDINFO_XCROUCH;         //ガード情報
  aif[i].id = ATTACK_MIDDLE; //ゲージ増加時に使用するID
  aif[i].damage = 40;        //ダメージ
  aif[i].kezuri = 0;         //削り

  i = ATTACKINFO_JAMP_C;                                                        //ジャンプ強攻撃
  aif[i].hit = HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3; //ヒット情報
  aif[i].guard = GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH;         //ガード情報
  aif[i].id = ATTACK_STRONG; //ゲージ増加時に使用するID
  aif[i].damage = 50;        //ダメージ
  aif[i].kezuri = 0;         //削り

  //・投

  i = ATTACKINFO_NAGE;                          //投げ（通常）
  aif[i].hit = HITINFO_MARK3 | HITINFO_SNDHIT3; //ヒット情報
  aif[i].id = ATTACK_STRONG;                    //ゲージ増加時に使用するID
  aif[i].damage = 80;                           //ダメージ
  aif[i].kezuri = 0;                            //削り

  //キリバンゲット
  i = ATTACKINFO_KIRIGET;                                                       //しゃがみ強攻撃
  aif[i].hit = HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3; //ヒット情報
  aif[i].guard = GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XSTAND | GUARDINFO_XJAMP; //ガード情報
  aif[i].id = ATTACK_STRONG; //ゲージ増加時に使用するID
  aif[i].damage = 50;        //ダメージ
  aif[i].kezuri = 0;         //削り

  // ageタックル

  i = ATTACKINFO_AGETACKLE1;
  aif[i].hit = HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1; //ヒット情報
  aif[i].guard = GUARDINFO_REACT3 | GUARDINFO_SIV1 | GUARDINFO_XJAMP;
  aif[i].id = ATTACK_HISSATU; //ゲージ増加時に使用するID
  aif[i].damage = 30;         //ダメージ
  aif[i].kezuri = 2;          //削り

  i = ATTACKINFO_AGETACKLE2;
  aif[i].hit = HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1; //ヒット情報
  aif[i].guard = GUARDINFO_REACT3 | GUARDINFO_SIV1;
  aif[i].id = ATTACK_HISSATU; //ゲージ増加時に使用するID
  aif[i].damage = 30;         //ダメージ
  aif[i].kezuri = 2;          //削り

  i = ATTACKINFO_AGETACKLE3;
  aif[i].hit = HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1; //ヒット情報
  aif[i].guard = GUARDINFO_REACT3 | GUARDINFO_SIV1;
  aif[i].id = ATTACK_HISSATU; //ゲージ増加時に使用するID
  aif[i].damage = 30;         //ダメージ
  aif[i].kezuri = 2;          //削り

  // sageナックル
  i = ATTACKINFO_SAGENA;                                                         //しゃがみ強攻撃
  aif[i].hit = HITINFO_REACT3A | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3; //ヒット情報
  aif[i].guard = GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH;          //ガード情報
  aif[i].id = ATTACK_STRONG; //ゲージ増加時に使用するID
  aif[i].damage = 40;        //ダメージ
  aif[i].kezuri = 0;         //削り

  //・当て身投げ

  i = ATTACKINFO_ATEMI_NAGE;                    //投げ（通常）
  aif[i].hit = HITINFO_MARK3 | HITINFO_SNDHIT3; //ヒット情報
  aif[i].id = ATTACK_ATEMI;                     //ゲージ増加時に使用するID
  aif[i].damage = 90;                           //ダメージ
  aif[i].kezuri = 0;                            //削り

  //・超必

  i = ATTACKINFO_CYOHI;
  aif[i].hit =
      HITINFO_MARK4 | HITINFO_USER2(USERHIT_ROTFUTTOBI) | HITINFO_SNDHIT3 | HITINFO_EFCTSINDO; //ヒット情報
  aif[i].guard = GUARDINFO_REACT3 | GUARDINFO_XJAMP;
  aif[i].id = 0;       //ゲージ増加時に使用するID
  aif[i].damage = 500; //ダメージ
  aif[i].kezuri = 50;  //削り

  //・遠当て

  i = ATTACKINFO_TOATE;
  aif[i].hit = HITINFO_MARK4 | HITINFO_SIV3 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_EFCTSINDO; //ヒット情報
  aif[i].guard = GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;
  aif[i].id = ATTACK_HISSATU; //ゲージ増加時に使用するID
  aif[i].damage = 100;        //ダメージ
  aif[i].kezuri = 10;         //削り

  //・ストライカー

  //キリバンゲット
  i = ATTACKINFO_STRIKER2GET;
  aif[i].hit = HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3;          //ヒット情報
  aif[i].guard = GUARDINFO_REACT2 | GUARDINFO_SIV3 | GUARDINFO_XSTAND | GUARDINFO_XJAMP; //ガード情報
  aif[i].id = ATTACK_STRIKER; //ゲージ増加時に使用するID
  aif[i].damage = 10;         //ダメージ
  aif[i].kezuri = 0;          //削り

  //チェインソー
  i = ATTACKINFO_STRIKERCHAIN;
  aif[i].hit = HITINFO_MARK3 | HITINFO_REACT3A | HITINFO_SNDHIT3; //ヒット情報
  aif[i].guard = GUARDINFO_REACT3;                                //ガード情報
  aif[i].id = 0;                                                  //ゲージ増加時に使用するID
  aif[i].damage = 8;                                              //ダメージ
  aif[i].kezuri = 0;                                              //削り

  //・ガードキャンセル
  aif[ATTACKINFO_GCANCEL] = aif[ATTACKINFO_AGETACKLE2];
  i = ATTACKINFO_GCANCEL;
  aif[i].id = 0;     //ゲージ増加時に使用するID
  aif[i].kezuri = 0; //削り

  //・リバーサル
  aif[ATTACKINFO_REVERSAL] = aif[ATTACKINFO_AGETACKLE2];
  i = ATTACKINFO_GCANCEL;
  aif[i].id = 0;     //ゲージ増加時に使用するID
  aif[i].kezuri = 0; //削り

  if (m_opt_hevy_hit) {
    for (int i = 0; i < AIF_NUM; i++) {
      aif[i].damage = (DWORD)(aif[i].damage * 1.2);
      aif[i].kezuri *= 2;
    }
  }
}

/*--------------------------------------------------------------------------------------
        攻撃開始処理（通常、書き換える必要はありません）
----------------------------------------------------------------------------------------*/
void CCharacter::StartAttack(DWORD atk_info_id) {
  GetGObject()->kougeki = TRUE;
  GetGObject()->atk = &aif[atk_info_id];
}

/*--------------------------------------------------------------------------------------
        COM行動用 技情報 初期化
----------------------------------------------------------------------------------------*/
void CCharacter::InitWazInfo() //コンピュータ用技情報の設定
{
  CCharacterBase::InitWazInfo();

  waz.dashf = ACTID_DASHF;
  waz.dashb = ACTID_DASHB;

  waz.att_short[0] = ACTID_ATT_SA;
  waz.att_short[1] = ACTID_ATT_CA;
  waz.att_short[2] = ACTID_ATT_SB;
  waz.att_short[3] = ACTID_CYOHI;
  waz.att_short[4] = ACTID_GCANCEL2;
  waz.att_short[5] = ACTID_ATEMI2;

  waz.att_middle[0] = ACTID_ATT_SB;
  waz.att_middle[1] = ACTID_ATT_SC;
  waz.att_middle[2] = ACTID_ATT_CB;
  waz.att_middle[3] = ACTID_ATT_CC;
  waz.att_middle[4] = ACTID_CYOHI;
  waz.att_middle[5] = ACTID_KIRIGET;

  waz.att_long[0] = ACTID_KIRIGET;
  waz.att_long[1] = ACTID_ATT_CC;
  waz.att_long[2] = ACTID_ATT_CB;
  waz.att_long[3] = ACTID_TOATE2;
  waz.att_long[4] = ACTID_TOATE1;

  waz.att_jamp[0] = ACTID_ATT_JA;
  waz.att_jamp[1] = ACTID_ATT_JB;
  waz.att_jamp[2] = ACTID_ATT_JC;

  waz.att_bullet[0] = ACTID_TOATE3;

  waz.att_tai[0] = ACTID_AGETACKLE1;
  waz.att_tai[1] = ACTID_ATEMI1;
  waz.att_tai[2] = ACTID_AGETACKLE2;
  waz.att_tai[3] = ACTID_AGETACKLE3;
  waz.att_tai[4] = ACTID_CYOHI;

  waz.nageid[0] = ACTID_NAGE1;

  //リーチ設定
  SetComReach(0, MAAI_SHORT);
  SetComReach(1, MAAI_MIDDLE);
  SetComReach(2, MAAI_LONG);
}

/*--------------------------------------------------------------------------------------
        レイジングストームコマンド
----------------------------------------------------------------------------------------*/
BOOL CCharacter::com1632143(int dt) {
  int ofst = 0;

  ofst = SeekKey(ofst, dt, KEYSTA_DOWN | KEYSTA_FOWORD);
  if (ofst < 0)
    return (FALSE);

  ofst = SeekKey(ofst, dt, KEYSTA_BACK);
  if (ofst < 0)
    return (FALSE);
  ofst = SeekKey(ofst, dt, KEYSTA_DOWN);
  if (ofst < 0)
    return (FALSE);
  /*ofst=SeekKey(ofst,dt,KEYSTA_DOWN | KEYSTA_BACK);
  if(ofst<0)return(FALSE);
  ofst=SeekKey(ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
  if(ofst<0)return(FALSE);*/
  ofst = SeekKey(ofst, dt, KEYSTA_FOWORD);
  if (ofst < 0)
    return (FALSE);

  ofst = SeekKey(ofst, dt, KEYSTA_DOWN | KEYSTA_BACK);
  if (ofst < 0)
    return (FALSE);

  return (TRUE);
}

/*--------------------------------------------------------------------------------------
        COMレベル判定
----------------------------------------------------------------------------------------*/
BOOL CCharacter::ComLevelCk(UINT level) {
  int crnt_level = GetComLevel();
  int v = rand() % 100;
  int r;
  const int rat_base = 80;

  if ((level == 0) || crnt_level == level) {
    r = rat_base;
  } else if (crnt_level < (int)level) {
    r = rat_base * crnt_level / level;
  } else {
    r = rat_base + (100 - rat_base) * (crnt_level - level) / 7;
  }

  return (r >= v) ? TRUE : FALSE;
}
