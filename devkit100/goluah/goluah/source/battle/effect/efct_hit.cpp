/*===============================================================

	ヒットマーク等

=================================================================*/
#include "stdafx.h"
#include "efct_hit.h"
#include "global.h"
#include "system_cell.h"

/*---------------------------------------------------------------
	ヒットマーク(弱)
-----------------------------------------------------------------*/
void CFxHitmark1::Update()
{
	if(pdat->counter == 1){
		AddEffect(EFCTID_TUBUTUBU2,(int)pdat->x,(int)pdat->y,TUBUTUBUPRM(15,10,5,5,5,4,7,0));
	}
	if(pdat->counter < 6)pdat->cnow=CELL_HITMARK1_4;
	else if(pdat->counter < 8)pdat->cnow=CELL_HITMARK1_3;
	else if(pdat->counter < 10)pdat->cnow=CELL_HITMARK1_2;
	else if(pdat->counter < 12)pdat->cnow=CELL_HITMARK1_1;
	else End();

	if(pdat->counter==0)pdat->color=0xFF000000;
	else if(pdat->counter<12)
	{
		pdat->color=0xFFFFFFFF - (((255/24) * pdat->counter)<<24);
	}
	else pdat->color=0x00FFFFFF;
}

BOOL CFxHitmark1::Draw()
{
	UINT counter = pdat->counter;

	if (counter < 15){//円
		g_draw.DrawCircle((int)pdat->x, (int)pdat->y, (counter + 4) * 5,
			30, 0, 0x00FFCC33 + (((255 - counter*(255 / 15)) * 255 * 255 * 255) & 0xFF000000), TRUE);
	}
	return pdat->counter<12 ? FALSE : TRUE;
}

/*---------------------------------------------------------------
	ヒットマーク(中)
-----------------------------------------------------------------*/
void CFxHitmark2::Update()
{
	if(pdat->counter == 1){
		AddEffect(EFCTID_TUBUTUBU2,(int)pdat->x,(int)pdat->y,TUBUTUBUPRM(15,10,8,5,5,5,9,0));
	}
	if(pdat->counter < 8)pdat->cnow=CELL_HITMARK2_1;
	else if(pdat->counter < 11)pdat->cnow=CELL_HITMARK2_2;
	else if(pdat->counter < 14)pdat->cnow=CELL_HITMARK2_3;
	else if(pdat->counter < 17)pdat->cnow=CELL_HITMARK2_4;
	else End();

	if(pdat->counter==0)pdat->color=0xFF000000;
	else if(pdat->counter < 17){
		pdat->color=0xFFFFFFFF - ((BYTE)(255/30 * pdat->counter))*256*256*256;
	}
	else pdat->color=0x00FFFFFF;
}

/*---------------------------------------------------------------
	ヒットマーク(強)
-----------------------------------------------------------------*/
void CFxHitmark3::Update()
{
	if(pdat->counter == 1){
		AddEffect(EFCTID_TUBUTUBU2,(int)pdat->x,(int)pdat->y,TUBUTUBUPRM(15,12,8,5,5,8,9,0));
	}
	if(pdat->counter < 10)pdat->cnow=CELL_HITMARK3_1;
	else if(pdat->counter < 14)pdat->cnow=CELL_HITMARK3_2;
	else if(pdat->counter < 18)pdat->cnow=CELL_HITMARK3_3;
	else if(pdat->counter < 22)pdat->cnow=CELL_HITMARK3_4;
	else End();

	if(pdat->counter==0)pdat->color=0xFF000000;
	else if(pdat->counter < 22){
		pdat->color=0xFFFFFFFF - (255/44 * pdat->counter)*256*256*256;
	}
	else pdat->color=0x00FFFFF;
}

/*---------------------------------------------------------------
	ヒットマーク(激)
-----------------------------------------------------------------*/
void CFxHitmark4::Update()
{
	if(pdat->counter == 1){
		AddEffect(EFCTID_TUBUTUBU,(int)pdat->x,(int)pdat->y,TUBUTUBUPRM(15,10,8,5,5,5,9,0));
	}
	if(pdat->counter == 1){
		AddEffect(EFCTID_TUBUTUBU2,(int)pdat->x,(int)pdat->y,TUBUTUBUPRM(15,12,8,5,5,4,13,0));
	}
	if(pdat->counter < 8)pdat->cnow=CELL_HITMARK4_1;
	else if(pdat->counter < 11)pdat->cnow=CELL_HITMARK4_2;
	else if(pdat->counter < 14)pdat->cnow=CELL_HITMARK4_3;
	else if(pdat->counter < 17)pdat->cnow=CELL_HITMARK4_4;
	else if(pdat->counter < 20)pdat->cnow=CELL_HITMARK4_5;
	else End();
	if(pdat->counter==0)pdat->color=0xFF000000;
	else {
		pdat->color=0xFFFFFFFF - (255/40 * pdat->counter)*256*256*256;
	}
}


BOOL CFxHitmark2::Draw()
{
	UINT counter = pdat->counter;

	if (counter < 15){//円
		g_draw.DrawCircle((int)pdat->x, (int)pdat->y, (counter + 4) * 5,
			30, 0, 0x00FFCC33 + (((255 - counter*(255 / 15)) * 255 * 255 * 255) & 0xFF000000), TRUE);
	}
	if (counter >= 20)return(TRUE);

	return FALSE;
}

BOOL CFxHitmark3::Draw()
{
	UINT counter = pdat->counter;

	if (counter < 15){//円
		g_draw.DrawCircle((int)pdat->x, (int)pdat->y, (counter + 4) * 5,
			30, 0, 0x00FFCC33 + (((255 - counter*(255 / 15)) * 255 * 255 * 255) & 0xFF000000), TRUE);
	}
	if (counter >= 20)return(TRUE);

	return FALSE;
}

BOOL CFxHitmark4::Draw()
{
	UINT counter = pdat->counter;

	if(counter < 15){//円
		g_draw.DrawCircle((int)pdat->x,(int)pdat->y,(counter+4)*5,
			30,0,0x00FFCC33 + (((255-counter*(255/15))*255*255*255)&0xFF000000),TRUE);
	}
	if(counter>=20)return(TRUE);

	return FALSE;
}

/*---------------------------------------------------------------
	ガードマーク(削りダメージなし)
-----------------------------------------------------------------*/
void CFxGuardmark1::Update()
{
	if(pdat->counter > 12)End();
	pdat->cnow = 0;
}

BOOL CFxGuardmark1::Draw()
{
	DWORD counter = pdat->counter;
	if(counter<12){
		if(pdat->muki){
			g_draw.DrawCircle((int)pdat->x,(int)pdat->y,(counter+3)*5,
				(15-counter/3),0,0xFFFFAA00,TRUE,-80,TRUE);
		}
		else{
			g_draw.DrawCircle((int)pdat->x,(int)pdat->y,(counter+3)*5,
				(15-counter/3),0,0xFFFFAA00,TRUE,80,TRUE);
		}
	}
	return(TRUE);
}

/*---------------------------------------------------------------
	ガードマーク(削りダメージあり)
-----------------------------------------------------------------*/
BOOL CFxGuardmark2::Draw()
{
	DWORD counter = pdat->counter;
	if(counter<12){
		if(pdat->muki){
			g_draw.DrawCircle((int)pdat->x,(int)pdat->y,(counter+3)*5,
				(15-counter/3),0,0xFF5555FF,TRUE,-80,TRUE);
		}
		else{
			g_draw.DrawCircle((int)pdat->x,(int)pdat->y,(counter+3)*5,
				(15-counter/3),0,0xFF5555FF,TRUE,80,TRUE);
		}
	}
	return(TRUE);
}

/*---------------------------------------------------------------
	掴みマーク
-----------------------------------------------------------------*/
void CFxCatchmark::Update()
{
	if(pdat->counter < 2)pdat->cnow=CELL_CATCH1;
	else if(pdat->counter < 3)pdat->cnow=CELL_CATCH2;
	else if(pdat->counter < 4)pdat->cnow=CELL_CATCH3;
	else if(pdat->counter < 5)pdat->cnow=CELL_CATCH4;
	else if(pdat->counter < 10)pdat->cnow=CELL_CATCH5;
	else if(pdat->counter < 15)pdat->cnow=CELL_CATCH6;
	else End();
}


/*---------------------------------------------------------------
	地面衝撃波
-----------------------------------------------------------------*/
void CFxGndCircle::Update()
{
	if(pdat->counter < 3)pdat->cnow=CELL_GNDCIR1;
	else if(pdat->counter < 6)pdat->cnow=CELL_GNDCIR2;
	else if(pdat->counter < 9)pdat->cnow=CELL_GNDCIR3;
	else if(pdat->counter <12)pdat->cnow=CELL_GNDCIR4;
	else End();
	pdat->z=ZZAHYO_EFFECT1;
}

