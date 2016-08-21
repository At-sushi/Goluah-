
/*===============================================================

	オープニングムービー再生　タスク

=================================================================*/
#pragma once

#include "global.h"
#include "task_opening.h"


//**********************************************************************

CTOpening::CTOpening()
{
	opok=TRUE;
}

CTOpening::~CTOpening()
{}

void CTOpening::Initialize()
{
	g_system.DisableRendering();

	// フルスクリーン時対策
	g_draw.d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	g_draw.UpdateD3DPP();

	opok = g_sound.PlayVideo(_T(".\\system\\op.avi"));

	counter = 0;
}

void CTOpening::Terminate()
{
	g_draw.d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_draw.UpdateD3DPP();

	g_system.EnableRendering();
	g_sound.BGMStop();
}

BOOL CTOpening::Execute(DWORD time)
{
	counter ++;
	if(counter<20)return TRUE;

	if(opok==FALSE){
		return FALSE;
	}
	if(g_input.GetAllKey() & KEYSTA_ANYKEY){
		return FALSE;
	}
	if(!g_sound.videoon){
		opok=FALSE;
		return FALSE;
	}
	return TRUE;
}

