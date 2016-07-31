/*=====================================================================================

Goluah!! Copyright (C) 2001-2004 aki, 2004-2016 At-sushi, 2014-2016 logger

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

=======================================================================================*/

/*==============================================================

	飛び道具

================================================================*/
#include "character.h"

void CCharacter::InitBullets()
{
	BULLETINFO_A nak1,nak2//wind,
		;//fola[3],folb[3],folc[3],fold[3],fole[3];
	int nakcr[17],nakcd[2],//windcr[4],windcd[2],
		folacr[3],folbcr[2],folccr[4],foldcr[2],folecr[5],folcd[2];

	//旧CCharacterコンストラクタでやってたの
	nak1.muki=pdat->muki;
	nak1.x	=pdat->x-100;
	nak1.y	=0;
	nak1.vx	=0;
	nak1.vy =0;
	nak1.ax =0;
	nak1.ay =0;
	nak1.dur	=58;
	nak2.muki=pdat->muki;
	nak2.x	=pdat->x+100;
	nak2.y	=0;
	nak2.vx	=0;
	nak2.vy =0;
	nak2.ax =0;
	nak2.ay =0;
	nak2.dur	=58;

	nak1.cid	=pdat->id;
	nak1.type	=0;
	nakcr[ 0]=CELL_NAKAMA01;
	nakcr[ 1]=CELL_NAKAMA02;
	nakcr[ 2]=CELL_NAKAMA03;
	nakcr[ 3]=CELL_NAKAMA04;
	nakcr[ 4]=CELL_NAKAMA05;
	nakcr[ 5]=CELL_NAKAMA06;
	nakcr[ 6]=CELL_NAKAMA07;
	nakcr[ 7]=CELL_NAKAMA08;
	nakcr[ 8]=CELL_NAKAMA09;
	nakcr[ 9]=CELL_NAKAMA10;
	nakcr[10]=CELL_NAKAMA11;
	nakcr[11]=CELL_NAKAMA12;
	nakcr[12]=CELL_NAKAMA13;
	nakcr[13]=CELL_NAKAMA14;
	nakcr[14]=CELL_NAKAMA15;
	nakcr[15]=CELL_NAKAMA16;
	nakcr[16]=-1;
	nakcd[0]=-1;
	nakcd[1]=-1;
	nak1.cell_run=nakcr;
	nak1.cell_dis=nakcd;
	nak1.spd_run	=4;
	nak1.spd_dis	=8;
	nak1.vx=0;
	nak1.vy=0;
	nak1.dur=64+8;
	nak1.state = 0;
	nak1.info_b= NULL;

	nak2 = nak1;
	clabul_nak1 = new CClassicalBullet(this,NULL,&nak1);
	clabul_nak2 = new CClassicalBullet(this,NULL,&nak2);

/*	wind.cid	=pdat->id;
	wind.type	=0;
	windcr[0]=CELL_KAZE1;
	windcr[1]=CELL_KAZE2;
	windcr[2]=CELL_KAZE3;
	windcr[3]=-1;
	windcd[0]=-1;
	windcd[1]=-1;
	wind.cell_run=windcr;
	wind.cell_dis=windcd;
	wind.spd_run	=5;
	wind.spd_dis	=4;
	wind.vx=1;
	wind.vy=0;
	wind.ax=wind.ay=0;
	wind.dur=14;
	wind.state = 0;
	wind.info_b= NULL;
	wind.y	=0;
	wind.vx	=1;
	wind.dur=14;
	clabul_wind = new CClassicalBullet(this,NULL,&wind);*/
	clabul_wind = new CDashWind(this);

	folcd[0] = folcd[1] = -1;

	//くるくる
	fola[0].cid	=pdat->id;
	fola[0].type	=BULLETA_VSHUMAN | BULLETA_VSBULLET | BULLETA_DONOTDIE;
	folacr[0]=CELL_FOLLOW11;
	folacr[1]=CELL_FOLLOW12;
	folacr[2]=-1;
	fola[0].cell_run=folacr;
	fola[0].cell_dis=folcd;
	fola[0].spd_run	=5;
	fola[0].spd_dis	=4;
	fola[0].state = 0;
	fola[0].info_b= NULL;

	fola[1] = fola[2] = fola[0];

	//バイク（？）
	folb[0].cid	=pdat->id;
	folb[0].type	=BULLETA_VSHUMAN | BULLETA_DONOTDIE;
	folbcr[0]=CELL_FOLLOW2;
	folbcr[1]=-1;
	folb[0].cell_run=folbcr;
	folb[0].cell_dis=folcd;
	folb[0].spd_run	=5;
	folb[0].spd_dis	=4;
	folb[0].state = 0;
	folb[0].info_b= NULL;

	folb[1] = folb[2] = folb[0];

	//走る
	folc[0].cid	=pdat->id;
	folc[0].type	=BULLETA_VSHUMAN | BULLETA_DONOTDIE;
	folccr[0]=CELL_FOLLOW31;
	folccr[1]=CELL_FOLLOW32;
	folccr[2]=CELL_FOLLOW33;
	folccr[3]=-1;
	folc[0].cell_run=folccr;
	folc[0].cell_dis=folcd;
	folc[0].spd_run	=5;
	folc[0].spd_dis	=4;
	folc[0].state = 0;
	folc[0].info_b= NULL;

	folc[1] = folc[2] = folc[0];

	//ｽﾞｻｰ
	fold[0].cid	=pdat->id;
	fold[0].type	=BULLETA_VSHUMAN | BULLETA_VSBULLET | BULLETA_DONOTDIE;
	foldcr[0]=CELL_FOLLOW4;
	foldcr[1]=-1;
	fold[0].cell_run=foldcr;
	fold[0].cell_dis=folcd;
	fold[0].spd_run	=5;
	fold[0].spd_dis	=4;
	fold[0].state = 0;
	fold[0].info_b= NULL;

	fold[1] = fold[2] = fold[0];

	//歩く
	fole[0].cid	=pdat->id;
	fole[0].type	=BULLETA_VSHUMAN | BULLETA_DONOTDIE;
	folecr[0]=CELL_FOLLOW51;
	folecr[1]=CELL_FOLLOW52;
	folecr[2]=CELL_FOLLOW53;
	folecr[3]=CELL_FOLLOW54;
	folecr[4]=-1;
	fole[0].cell_run=folecr;
	fole[0].cell_dis=folcd;
	fole[0].spd_run	=5;
	fole[0].spd_dis	=4;
	fole[0].state = 0;
	fole[0].info_b= NULL;

	fole[1] = fole[2] = fole[0];
	
	int foldur = 270;
	int foldmg = 30;
	for(int i=0;i<3;i++)
	{
		fola[i].muki=pdat->muki;
	//	fola[i].x	=pdat->x + zurex(-500);
	//	fola[i].y	=0;
		fola[i].vx	=6;
		fola[i].vy=fola[i].ax=fola[i].ay=0;
		fola[i].dur	=foldur;
		fola[i].atk.id		= ATTACK_FRIENDS;
		fola[i].atk.damage	= foldmg;
		fola[i].atk.dur		= foldur;
		fola[i].atk.hit		= HITINFO_MARK4 | HITINFO_REACT3 | HITINFO_SNDHIT3 | GUARDINFO_EFCTSINDO;
		fola[i].atk.guard	= GUARDINFO_REACT3;
		fola[i].atk.kezuri	= 5;
		fola[i].atk.muki	= pdat->muki;
		fola[i].atk.pg_guard=-0.05;
		fola[i].atk.pg_hit	=-0.05;//ainfo.pg_guard*2;
		clabul_fola[i] = new CClassicalBullet(this,NULL,&fola[i]);

		folb[i].muki=pdat->muki;
	//	folb[i].x	=pdat->x + zurex(-500);
	//	folb[i].y	=0;
		folb[i].vx	=10.5;
		folb[i].vy=folb[i].ax=folb[i].ay=0;
		folb[i].dur	=foldur;
		folb[i].atk.id		= ATTACK_FRIENDS;
		folb[i].atk.damage	= foldmg;
		folb[i].atk.dur		= foldur;
		folb[i].atk.hit		= HITINFO_MARK4 | HITINFO_REACT3 | HITINFO_SNDHIT3 | GUARDINFO_EFCTSINDO;
		folb[i].atk.guard	= GUARDINFO_REACT3;
		folb[i].atk.kezuri	= 5;
		folb[i].atk.muki	= pdat->muki;
		folb[i].atk.pg_guard=-0.05;
		folb[i].atk.pg_hit	=-0.05;//ainfo.pg_guard*2;
		clabul_folb[i] = new CClassicalBullet(this,NULL,&folb[i]);

		folc[i].muki=pdat->muki;
	//	folc[i].x	=pdat->x + zurex(-500);
//		folc[i].y	=0;
		folc[i].vx	=7.5;
		folc[i].vy=folc[i].ax=folc[i].ay=0;
		folc[i].dur	=foldur;
		folc[i].atk.id		= ATTACK_FRIENDS;
		folc[i].atk.damage	= foldmg;
		folc[i].atk.dur		= foldur;
		folc[i].atk.hit		= HITINFO_MARK4 | HITINFO_REACT3 | HITINFO_SNDHIT3 | GUARDINFO_EFCTSINDO;
		folc[i].atk.guard	= GUARDINFO_REACT3;
		folc[i].atk.kezuri	= 5;
		folc[i].atk.muki	= pdat->muki;
		folc[i].atk.pg_guard=-0.05;
		folc[i].atk.pg_hit	=-0.05;//ainfo.pg_guard*2;
		clabul_folc[i] = new CClassicalBullet(this,NULL,&folc[i]);

		fold[i].muki=pdat->muki;
		fold[i].vx	=9;
		fold[i].vy=fold[i].ax=fold[i].ay=0;
		fold[i].dur	=foldur;
		fold[i].atk.id		= ATTACK_FRIENDS;
		fold[i].atk.damage	= foldmg;
		fold[i].atk.dur		= foldur;
		fold[i].atk.hit		= HITINFO_MARK4 | HITINFO_REACT3 | HITINFO_SNDHIT3 | GUARDINFO_EFCTSINDO;
		fold[i].atk.guard	= GUARDINFO_REACT3;
		fold[i].atk.kezuri	= 5;
		fold[i].atk.muki	= pdat->muki;
		fold[i].atk.pg_guard=-0.05;
		fold[i].atk.pg_hit	=-0.05;//ainfo.pg_guard*2;
		clabul_fold[i] = new CClassicalBullet(this,NULL,&fold[i]);

		fole[i].muki=pdat->muki;
		fole[i].vx	=6.5;
		fole[i].vy=fole[i].ax=fole[i].ay=0;
		fole[i].dur	=foldur;
		fole[i].atk.id		= ATTACK_FRIENDS;
		fole[i].atk.damage	= foldmg;
		fole[i].atk.dur		= foldur;
		fole[i].atk.hit		= HITINFO_MARK4 | HITINFO_REACT3 | HITINFO_SNDHIT3 | GUARDINFO_EFCTSINDO;
		fole[i].atk.guard	= GUARDINFO_REACT3;
		fole[i].atk.kezuri	= 5;
		fole[i].atk.muki	= pdat->muki;
		fole[i].atk.pg_guard	=-0.05;
		fole[i].atk.pg_hit		=-0.05;//ainfo.pg_guard*2;
		clabul_fole[i] = new CClassicalBullet(this,NULL,&fole[i]);
	}
	bul_heart = new CHeart(this);

	if(opt_exfriend)
	{
		clabul_folx[0] = new CClassicalBullet(this,NULL,&fola[0]);
		clabul_folx[1] = new CClassicalBullet(this,NULL,&folb[0]);
		clabul_folx[2] = new CClassicalBullet(this,NULL,&folc[0]);
		clabul_folx[3] = new CClassicalBullet(this,NULL,&fold[0]);
		clabul_folx[4] = new CClassicalBullet(this,NULL,&fole[0]);
		clabul_folx[5] = new CClassicalBullet(this,NULL,&fola[0]);
	}
	else
	{
		for(int i=0;i<6;i++)
			clabul_folx[i] = NULL;
	}
}

BOOL CCharacter::EscapeOK()
{
	for(int i=0;i<3;i++)
	{
		if(fola[i].state!=0)return FALSE;
		if(folb[i].state!=0)return FALSE;
		if(folc[i].state!=0)return FALSE;
		if(fold[i].state!=0)return FALSE;
		if(fole[i].state!=0)return FALSE;
	}
	return TRUE;
}


/***************************************************************
	ダッシュ時風エフェクト
****************************************************************/

CDashWind::CDashWind(CCharacter *parent) : CBulletBase(parent)
{
	RemoveProperty(GOBJFLG_ATTACK);
	base_z = ZZAHYO_EFFECT1;
}
	
void CDashWind::act_run()
{
	pdat->color= 0x00FFFFFF + (((255-(DWORD)(pdat->counter/30.0f*255.0f))*255*255*255)&0xFF000000);
	movex(pdat->vx);

	pdat->cnow = CELL_KAZE1 + (pdat->counter/11);

	if(pdat->counter>30){
		ChangeAction(CBB_STATE_IDLE);
	}
}

void CDashWind::act_bomb()
{
	ChangeAction(CBB_STATE_IDLE);
}


/***************************************************************
	ハート
****************************************************************/

CHeart::CHeart(CCharacter *parent) : CBulletBase(parent)
{
	RemoveProperty(GOBJFLG_ATTACK);
	base_z = ZZAHYO_EFFECT1;
}
	
void CHeart::act_run()
{
	pdat->cnow=CELL_HEART;
	pdat->color= 0x00FFFFFF + (((255-pdat->counter*25)*255*255*255)&0xFF000000);
	pdat->magx=pdat->magy=(float)(1.0f+0.03f*pdat->counter);

	if (pdat->counter==0){
		movex(-20);
	}
	pdat->y = 3*pdat->counter;

	if(pdat->counter>8){
		ChangeAction(CBB_STATE_IDLE);
	}
}

void CHeart::act_bomb()
{
	ChangeAction(CBB_STATE_IDLE);
}
