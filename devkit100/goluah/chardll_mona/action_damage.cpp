/*=====================================================================================

Goluah!! Copyright (C) 2001-2004 aki, 2004-2016 At-sushi, 2014-2016 logger

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

=======================================================================================*/

/*==============================================================

	行動関数（敵側行動・投げ以外）

================================================================*/
#include "character.h"

#define emovex(dx)	pedat->x+=(pedat->muki) ? -dx:dx

/*--------------------------------------------------------------
	エリアル・浮かせヒット
----------------------------------------------------------------*/
void CCharacter::eact_aerialpop(GOBJECT* pedat)
{
	if(pedat->counter==0){
		pedat->vx=-2;
		pedat->vy=-3;
	}
	emovex(pedat->vx);
	pedat->y+=pedat->vy;
	pedat->vy += pedat->vy<0 ? 0.4 : 1.0;

	if(pedat->counter < 8){pedat->cnow = DCELL_DAMAGEJ;}
	else if(pedat->counter < 12){pedat->cnow = DCELL_RAKKA1;}
	else if(pedat->counter < 16){pedat->cnow = DCELL_RAKKA2;}

	if(pedat->y > 0){
		pedat->y=0;
		pedat->aid=ACTID_DOWN2;
	}
}

/*--------------------------------------------------------------
	エリアル・空中ヒット
----------------------------------------------------------------*/
void CCharacter::eact_aerialhit1(GOBJECT* pedat)
{
	if(!(pedat->aid&ACTID_KUCYU)){
		pedat->aid = ACTID_DAMAGE1;
		return;
	}

	if(pedat->counter==0){
		pedat->vy-=0.7;
		if(pedat->vy>0)pedat->vy=-1.5;
	}
	emovex(pedat->vx);
	pedat->y+=pedat->vy;
	pedat->vy += pedat->vy<3 ? 0.4 : 1.0;

	if(pedat->counter < 8){pedat->cnow = DCELL_DAMAGEJ;}
	else if(pedat->counter < 12){pedat->cnow = DCELL_RAKKA1;}
	else if(pedat->counter < 16){pedat->cnow = DCELL_RAKKA2;}

	if(pedat->y > 0){
		pedat->y=0;
		pedat->aid=ACTID_DOWN2;
	}
}

void CCharacter::eact_aerialhit2(GOBJECT* pedat)
{
	if(!(pedat->aid&ACTID_KUCYU)){
		pedat->aid = ACTID_DAMAGE2;
		return;
	}

	if(pedat->counter==0){
		pedat->vy-=0.7;
		if(pedat->vy>0)pedat->vy=-1.5;
	}
	emovex(pedat->vx);
	pedat->y+=pedat->vy;
	pedat->vy += pedat->vy<3 ? 0.4 : 1.0;

	if(pedat->counter < 8){pedat->cnow = DCELL_DAMAGEJ;}
	else if(pedat->counter < 12){pedat->cnow = DCELL_RAKKA1;}
	else if(pedat->counter < 16){pedat->cnow = DCELL_RAKKA2;}

	if(pedat->y > 0){
		pedat->y=0;
		pedat->aid=ACTID_DOWN2;
	}
}

void CCharacter::eact_aerialhit3(GOBJECT* pedat)
{
	if(!(pedat->aid&ACTID_KUCYU)){
		pedat->aid = ACTID_DAMAGE3;
		return;
	}

	if(pedat->counter==0){
		pedat->vy-=0.7;
		if(pedat->vy>0)pedat->vy=-1.5;
	}
	emovex(pedat->vx);
	pedat->y+=pedat->vy;
	pedat->vy += pedat->vy<3 ? 0.4 : 1.0;

	if(pedat->counter < 8){pedat->cnow = DCELL_DAMAGEJ;}
	else if(pedat->counter < 12){pedat->cnow = DCELL_RAKKA1;}
	else if(pedat->counter < 16){pedat->cnow = DCELL_RAKKA2;}

	if(pedat->y > 0){
		pedat->y=0;
		pedat->aid=ACTID_DOWN2;
	}
}