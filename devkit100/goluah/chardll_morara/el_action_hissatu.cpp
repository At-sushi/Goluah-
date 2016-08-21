/*=====================================================================================

Goluah!! Copyright (C) 2001-2004 aki, 2004-2016 At-sushi, 2014-2016 logger

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

=======================================================================================*/

/*==============================================================

	行動関数(必殺技)

================================================================*/
#include "el_character.h"

#ifndef PI
#define PI (3.141592f)
#endif

/*--------------------------------------------------------------
	エナジーアロー（仮）
----------------------------------------------------------------*/
void CCharacterEL::act_earrow1()
{
	if(ccounter==0)PlayMySound(wave_tetra);

	if(ccounter<1)		cell( CELL_GEA1 );
	else if(ccounter<2)	cell( CELL_GEA2 );
	else if(ccounter<3)	cell( CELL_GEA3 );
	else if(ccounter<4)	cell( CELL_GEA4 );
	else if(ccounter<5)	cell( CELL_GEA5 );
	else if(ccounter<10)cell( CELL_GEA6 );
	else if(ccounter<15)cell( CELL_GEA5 );
	else if(ccounter<20)cell( CELL_GEA4 );
	else if(ccounter<25)cell( CELL_GEA3 );
	else if(ccounter<30)cell( CELL_GEA2 );
	else if(ccounter<35)cell( CELL_GEA1 );
	else ChangeAction( ACTID_NEUTRAL );

	if(ccounter==5)
	{
		AddPowerGauge( -1 * NEEDGAUGE_TETRA );

		V2d v[] = 
		{
			{  2.0f , - 8.0f},
			{ -6.0f , -10.0f},
			{-20.0f , - 5.0f},
			{-10.0f ,  -8.0f}
		};
		const float vmag = 2.8f;

		for(int i=0;i<EARROW_NUM;i++)
		{
			pbul_earrow[i]->Go(
					TRUE,
					pdat->x,
					pdat->y - 50,
					v[i].x * vmag * (pdat->muki ? -1.0f : 1.0f),
					v[i].y * vmag);
		}
	}
}

void CCharacterEL::act_earrow2()
{
	act_earrow1();
}

void CCharacterEL::act_earrow3()
{
	act_earrow1();
}

/*--------------------------------------------------------------
	エナジーアロー（仮）空中版
----------------------------------------------------------------*/
void CCharacterEL::act_earrow1j()
{
	pdat->vy = pdat->vx = 0;

	if(ccounter==0)PlayMySound(wave_tetra);
	if(ccounter<1)		cell( CELL_JC1 );
	else if(ccounter<2)	cell( CELL_JC2 );
	else if(ccounter<3)	cell( CELL_JC3 );
	else if(ccounter<4)	cell( CELL_JC4 );
	else if(ccounter<5)	cell( CELL_JC5 );
	else if(ccounter<10)cell( CELL_JC6 );
	else if(ccounter<15)cell( CELL_JC5 );
	else if(ccounter<20)cell( CELL_JC4 );
	else if(ccounter<25)cell( CELL_JC3 );
	else if(ccounter<30)cell( CELL_JC2 );
	else if(ccounter<35)cell( CELL_JC1 );
	else ChangeAction( ACTID_RAKKA2 );

	if(ccounter==5)
	{
		AddPowerGauge( -1 * NEEDGAUGE_TETRA );

		V2d v[] = 
		{
			{  2.0f , - 8.0f},
			{ -6.0f , -10.0f},
			{-20.0f , - 5.0f},
			{-10.0f ,  -8.0f}
		};
		const float vmag = 2.8f;

		for(int i=0;i<EARROW_NUM;i++)
		{
			pbul_earrow[i]->Go(
					TRUE,
					pdat->x,
					pdat->y - 50,
					v[i].x * vmag * (pdat->muki ? -1.0f : 1.0f),
					v[i].y * vmag);
		}
	}
}

void CCharacterEL::act_earrow2j()
{
	act_earrow1j();
}

void CCharacterEL::act_earrow3j()
{
	act_earrow1j();
}


/*--------------------------------------------------------------
	ソード・オブ・ジャスティス(共通)
----------------------------------------------------------------*/
BOOL CCharacterEL::act_swordRealize()
{
	if(ccounter==0)PlayMySound(wave_sword);
	if(ccounter==0)AddPowerGauge( -1 * NEEDGAUGE_SOJ );

	const UINT anmspd = m_event_sw ? 1 : 2;
	if(ccounter<anmspd)			cell( CELL_SWORD0_1 );
	else if(ccounter<anmspd*2)	cell( CELL_SWORD0_2 );
	else if(ccounter<anmspd*3)	cell( CELL_SWORD0_3 );
	else if(ccounter<anmspd*4)	cell( CELL_SWORD0_4 );
	else if(ccounter<anmspd*5)	cell( CELL_SWORD0_5 );
	else if(ccounter<anmspd*6)	cell( CELL_SWORD0_6 );
	else if(ccounter<anmspd*7)	cell( CELL_SWORD0_7 );
	else if(ccounter<anmspd*8)	cell( CELL_SWORD0_8 );
	else return TRUE;

	return FALSE;
}

/*--------------------------------------------------------------
	ソード・オブ・ジャスティス(弱)
----------------------------------------------------------------*/

//具現化
void CCharacterEL::act_sword1r()
{
	if(act_swordRealize())ChangeAction( ACTID_SWORD1A );
}

//攻撃
void CCharacterEL::act_sword1a()
{
	if(ccounter==0)PlayMySound(wave_sword1);
	if(ccounter==0)
	{
		StartAttack( ATTACKINFO_SWORD1 );
		if(pefe_track)
		{
			pefe_track->Startup(
				40.0f,
				3,
				5,
				PI,
				0.0f,
				0.1f,
				20.0f,
				-0.30f,
				60.0f);
		}
	}

	const UINT anmspd = 3;
	if(ccounter<anmspd)			cell( CELL_SWORD1_1 );
	else if(ccounter<anmspd*2)	cell( CELL_SWORD1_2 );
	else if(ccounter<anmspd*3)	cell( CELL_SWORD1_3 );
	else ChangeAction(ACTID_SWORD1D );
}

//消失
void CCharacterEL::act_sword1d()
{
	const UINT anmspd = m_event_sw ? 2 : 4;
	if(ccounter<anmspd+SOJ_OFFSET)			cell( CELL_SWORD5_1 );
	else if(ccounter<anmspd*2+SOJ_OFFSET)	cell( CELL_SWORD5_2 );
	else if(ccounter<anmspd*3+SOJ_OFFSET)	cell( CELL_SWORD5_3 );
	else if(ccounter<anmspd*4+SOJ_OFFSET)	cell( CELL_SWORD5_4 );
	else if(ccounter<anmspd*5+SOJ_OFFSET)	cell( CELL_SWORD5_5 );
	else if(ccounter<anmspd*6+SOJ_OFFSET)	cell( CELL_SWORD5_6 );
	else if(ccounter<anmspd*7+SOJ_OFFSET)	cell( CELL_SWORD5_7 );
	else if(ccounter<anmspd*8+SOJ_OFFSET)	cell( CELL_SWORD5_8 );
	else if(ccounter<anmspd*9+SOJ_OFFSET)	cell( CELL_SWORD5_9 );
	else ChangeAction( ACTID_NEUTRAL );
}

/*--------------------------------------------------------------
	ソード・オブ・ジャスティス(中)
----------------------------------------------------------------*/

//具現化
void CCharacterEL::act_sword2r()
{
	if(act_swordRealize())ChangeAction( ACTID_SWORD2A );
}

//攻撃
void CCharacterEL::act_sword2a()
{
	if(ccounter==0)PlayMySound(wave_sword2);
	if(ccounter==0)
	{
		StartAttack( ATTACKINFO_SWORD2 );
		if(pefe_track)
		{
			pefe_track->Startup(
				20.0f,
				2,
				8,
				PI*0.25f,
				0.8f,
				0.1f,
				200.0f,
				0.01f,
				80.0f);
		}
	}

	const UINT anmspd = 3;
	if(ccounter<anmspd)			cell( CELL_SWORD2_1 );
	else if(ccounter<anmspd*2)	cell( CELL_SWORD2_2 );
	else if(ccounter<anmspd*3)	cell( CELL_SWORD2_3 );
	else ChangeAction(ACTID_SWORD2D );
}

//消失
void CCharacterEL::act_sword2d()
{
	act_sword1d();//弱と同じ消え方
}

/*--------------------------------------------------------------
	ソード・オブ・ジャスティス(強)
----------------------------------------------------------------*/

//具現化
void CCharacterEL::act_sword3r()
{
	if(act_swordRealize())ChangeAction( ACTID_SWORD3A );
}

//攻撃
void CCharacterEL::act_sword3a()
{
	if(ccounter==0)PlayMySound(wave_sword3);
	if(ccounter==0)
	{
		StartAttack( ATTACKINFO_SWORD3 );
		if(pefe_track){
			pefe_track->Startup(
				20.0f,
				2,
				8,
				PI*0.25f,
				0.8f,
				0.1f,
				80.0f,
				-0.10f,
				50.0f,
				TRUE);
		}
	}

	const UINT anmspd = 3;
	if(ccounter<anmspd)			cell( CELL_SWORD3_1 );
	else if(ccounter<anmspd*2)	cell( CELL_SWORD3_2 );
	else if(ccounter<anmspd*3)	cell( CELL_SWORD3_3 );
	else ChangeAction(ACTID_SWORD3D );
}

//消失
void CCharacterEL::act_sword3d()
{
	const UINT anmspd = m_event_sw ? 2 : 4;
	if(ccounter<anmspd+SOJ_OFFSET)			cell( CELL_SWORD4_1 );
	else if(ccounter<anmspd*2+SOJ_OFFSET)	cell( CELL_SWORD4_2 );
	else if(ccounter<anmspd*3+SOJ_OFFSET)	cell( CELL_SWORD4_3 );
	else if(ccounter<anmspd*4+SOJ_OFFSET)	cell( CELL_SWORD4_4 );
	else if(ccounter<anmspd*5+SOJ_OFFSET)	cell( CELL_SWORD4_5 );
	else if(ccounter<anmspd*6+SOJ_OFFSET)	cell( CELL_SWORD4_6 );
	else if(ccounter<anmspd*7+SOJ_OFFSET)	cell( CELL_SWORD4_7 );
	else if(ccounter<anmspd*8+SOJ_OFFSET)	cell( CELL_SWORD4_8 );
	else ChangeAction( ACTID_NEUTRAL );
}

/*--------------------------------------------------------------
	ジャッジメント
----------------------------------------------------------------*/
void CCharacterEL::act_judgement()
{
	if(ccounter==0)PlayMySound(wave_judge);
	if(pdat->counter < 1)pdat->cnow = CELL_JUDGE1;
	else if(pdat->counter < 4)pdat->cnow = CELL_JUDGE2;
	else if(pdat->counter < 60){
		if(pdat->counter%2==0)pdat->cnow = CELL_JUDGE3;
		else pdat->cnow = CELL_JUDGE4;
	}
	else{
		pdat->aid = ACTID_RAKKA2;
	}

	if(pdat->counter==0)
	{
		AddPowerGauge( -1 * NEEDGAUGE_DOJ );
		AddEffect(EFCTID_CYOHI,(int)pdat->x,(int)pdat->y-50,30);
	}

	if(pdat->counter==5)
	{
		if(pbul_judge)pbul_judge->Go(pdat->muki,pdat->x + zurex(-320));
	}
}

