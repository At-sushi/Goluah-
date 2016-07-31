/*=====================================================================================

Goluah!! Copyright (C) 2001-2004 aki, 2004-2016 At-sushi, 2014-2016 logger

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

=======================================================================================*/

#include "gobject.h"

//オプション定義(モララエル)
#define OPTIONS_CHAIN_COMBO_EL		0x00010000
#define OPTIONS_TETRAGRAMMATON		0x00020000
#define OPTIONS_SWORDOFJUSTICE		0x00040000
#define OPTIONS_DAYOFJUDGEMENT		0x00080000
#define OPTIONS_MEGIDORAON			0x00100000
#define OPTIONS_REGENERATOR			0x00200000
#define OPTIONS_ENERGY_SOURCE		0x00400000
#define OPTIONS_ENERGY_SOURCE2		0x00800000
#define OPTIONS_ATTACK30			0x01000000
#define OPTIONS_HP50				0x02000000
#define OPTIONS_AUTOGUARD			0x04000000

#define OPTIONS_MORARAEL_ALL		0x07FF0000//上記すべての組み合わせ

//オプション定義
#define OPTIONS_CHAIN_COMBO			(0x00000001)
#define OPTIONS_EXTRA_ATTACK		(0x00000002)
#define OPTIONS_AUTO_GUARD			(0x00000004)
#define OPTIONS_GUARD_CANCEL		(0x00000008)
#define OPTIONS_G_CUTTER_PLUS		(0x00000010)
#define OPTIONS_CHIBASIRI_PLUS		(0x00000020)
#define OPTIONS_2ND_STEP			(0x00000040)
#define OPTIONS_STOCKABLE_GAUGE		(0x00000080)
#define OPTIONS_MINIGUN_RELOADED	(0x00000100)

#define OPTIONS_MORARA_ALL			(0x000001FF)//上記すべての組み合わせ

#define OPTIONS_MORARAEL_MODE		(0x08000000 | CHARACTER_LOAD_OPTION_FACE_A | OPTIONS_MORARAEL_ALL)

