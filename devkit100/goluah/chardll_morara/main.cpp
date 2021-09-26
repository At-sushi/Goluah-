
/*=====================================================================================

        キャラクター生成

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

#include "character.h"                                           //モララー
extern CCharacterBase *CreateMorarael(CDI_CHARACTERINFO2 *info); //モララエル

CHARACTER_LOAD_OPTION option[] = {
    // キャラクターのオプションをここに書いてください。（最大32個）
    // なお、オプションとオプションの間はコンマ(,)で区切ってください。

    // 記入内容：
    // 　{ フラグ, 競合するｵﾌﾟｼｮﾝ,依存するｵﾌﾟｼｮﾝ, オプション名, 消費ポイント }
    {OPTIONS_EXTRA_ATTACK, OPTIONS_MORARAEL_MODE, 0, "Extra Attack", 3},
    {OPTIONS_AUTO_GUARD, OPTIONS_MORARAEL_MODE, 0, "Auto Guard", 5},
    {OPTIONS_G_CUTTER_PLUS, OPTIONS_MORARAEL_MODE, 0, "G.Cutter +", 4},
    {OPTIONS_CHIBASIRI_PLUS, OPTIONS_MORARAEL_MODE, 0, "Chibasiri +", 3},
    {OPTIONS_2ND_STEP, OPTIONS_MORARAEL_MODE, 0, "2nd-Step", 3},
    {OPTIONS_STOCKABLE_GAUGE, OPTIONS_MORARAEL_MODE, 0, "Stockable Gauge", 4},
    {OPTIONS_MINIGUN_RELOADED, OPTIONS_MORARAEL_MODE, OPTIONS_STOCKABLE_GAUGE, "Minigun -Reloaded-", 6},
    {OPTIONS_MORARAEL_MODE, OPTIONS_MORARA_ALL, 0, "Morarael-Mode", 99},
};

//**************************************************************************************
//  Character Information
//　ここにキャラクターの名前を記述します
//**************************************************************************************

CCharacterInfo CharInfo(
    "モララー",  // キャラクターの名前（最大31バイト）
    CDI_VERSION, // キャラクターDLLインターフェイスのバージョン
    option,      // オプション構造体へのポインタ
    sizeof(option) / sizeof(CHARACTER_LOAD_OPTION), // オプションの数
    15,                                             // オプション最大ポイント数
    FALSE); // ネット対戦対応かどうか 将来の拡張のために用意されている。現在は0を渡さなければならない（笑）

char *CharDirectory = NULL;

//キャラクター登録処理（通常通り）
extern "C" BOOL CharacterInfo(LPVOID info) { return CharInfo.DllCharacterInfo(info); }

//キャラクター生成処理（切り替え）
extern "C" DWORD CreateCharacter(PVOID info) {
  CharInfo.DllCreateCharacter(info);
  CDI_CHARACTERINFO2 *cinfo = (CDI_CHARACTERINFO2 *)info;

  CCharacterBase *pc;
  if (cinfo->options_flag & OPTIONS_MORARAEL_MODE) {
    //モララエルモード
    pc = CreateMorarael((CDI_CHARACTERINFO2 *)info);
  } else {
    //モララーモード
    pc = new CCharacter((CDI_CHARACTERINFO2 *)info);
  }
  pc->InitializeCharacter();
  return (pc->GetObjectID());
}
