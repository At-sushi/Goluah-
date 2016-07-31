/*=====================================================================================

Goluah!! Copyright (C) 2001-2004 aki, 2004-2016 At-sushi, 2014-2016 logger

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

=======================================================================================*/

/*==============================================================

	飛び道具関係

================================================================*/
#include "character.h"


/*---------------------------------------------------------------
	飛道具初期化(CCharacter)
-----------------------------------------------------------------*/
void CCharacter::InitBullets()
{
	efe_wind =  new C2GetWind(this);
	pbul_storm = new CStorm(this,&aif[ATTACKINFO_CYOHI]);
	pbul_toate = new CToate(this,&aif[ATTACKINFO_TOATE]);
}

/***************************************************************
	2ゲットズサー時風
****************************************************************/

C2GetWind::C2GetWind(CCharacterBase *parent) : CBulletBase(parent)
{
	RemoveProperty(GOBJFLG_ATTACK);
	base_z = ZZAHYO_EFFECT1;
}
	
void C2GetWind::act_run()
{
	pdat->cnow = CELL_GETKAZE1 + ccounter/12;
	pdat->color= 0x00FFFFFF + (((255-(DWORD)(pdat->counter/30.0f*255.0f))*255*255*255)&0xFF000000);
	movex(pdat->vx);
	
	if(pdat->counter>30){
		ChangeAction(CBB_STATE_IDLE);
	}
}

void C2GetWind::act_bomb()
{
	ChangeAction(CBB_STATE_IDLE);
}


/***************************************************************
	なんとかストームみたいの
****************************************************************/
CStorm::CStorm(CCharacterBase *parent,ATTACKINFO *aif) : CBulletBase(parent)
{
	for(int i=0;i<STORM_NUM;i++)
	{
		m_strips[i] = aki3d.CreateTristripObject();
		m_strips[i]->Create(STORM_NODE_NUM,"tex_storm.png");
	}

	m_fb = aki3d.CreateFlatBoardsObject();
	m_fb->Create( NUM_GNDSHK,"tex_wind.png");

	atkinfo = *aif;
	m_list.clear();
}

BOOL CStorm::Go(BOOL muki,double x,double y,double vx,double vy)
{
	//CBulletBase::Go(muki,x,y,vx,vy);
	ChangeAction(CBB_STATE_RUN);

	pdat->muki = muki;
	pdat->x = x;
	pdat->y = y;

	m_list.clear();

	for(int i=0;i<STORM_NUM;i++)
	{
		drot[i] = 360.0f/(float)(STORM_NUM) * i;
	}
	for(int i=0;i<NUM_GNDSHK;i++)
	{
		m_info[i].Reset();
	}
	return TRUE;
}

void CStorm::act_run()
{
	float t = ((float)pdat->counter)/13.0f;
	if(t>=1.0f){
		Hide();
		pdat->kougeki = TRUE;
		return;
	}

	//if(pdat->counter%3==0)
	pdat->kougeki = TRUE;

	//あたり判定のため
	cell( CELL_STORM_HANTEI );
	pdat->magy = t*1.5f;

	UINT i;

	for(i=0;i<STORM_NUM;i++)
	{
		UpdateOne( *m_strips[i], t, drot[i]);
	}

	ARGB col;
	col.alpha	= 220;
	col.red		=  90;
	col.green	=  75;
	col.alpha = (BYTE)( 255.0f*(1.0f-t) );

	for(i=0;i<NUM_GNDSHK;i++)
	{
		m_fb->SetPos(i,m_info[i].pos*(0.2f+1.25f*t));
		m_fb->SetAxis1(i,m_info[i].ax1*(0.5f+3.0f*t));
		m_fb->SetAxis2(i,m_info[i].ax2*(0.5f+0.5f*t));
		m_fb->SetColor(i,col);
	}
}

void CStorm::UpdateOne(CTristrip& strip,float t,float rot)
{
	const float r_start =  80.0f;
	const float r_mid   = 160.0f;
	const float r_end	=  40.0f;
	const float height	= 450.0f;
	const float h_mid	= 120.0f;
	const float width	= 60.0f;
	const float t1		= 0.0f;
	const float t2		= 0.7f;

	//deg → rad
	rot = rot/360.0f * 2.0f * 3.1416f;

	ARGB color;
	color.alpha	= 220;
	color.red	=  90;
	color.green	=  75;
	color.blue	= 255;

	for(int i=0;i<STORM_NODE_NUM;i++)
	{
		float s = (float)i/(float)STORM_NODE_NUM;
		float u;
		float t3 = (t2-t1)*t + t1;
		if(s<t3)
		{
			u = s/t3;
			u = 1.0f - (1.0f-u)*(1.0f-u);
		}
		else
		{
			u = (s-t3)/(1.0f-t3);
			u = 1.0f - u*u;
		}
		if(t>0.5f){
			u *= 1.0f - (t-0.5f)*2.0f;
		}

		// h : 高さ
		// r : 半径
		float h = height*t*s;
		float r;
		if(h<h_mid){
			r = h/h_mid;
			r = 1.0f - (1.0f-r)*(1.0f-r);
			r = r*( r_mid - r_start );
			r += r_start;
		}
		else{
			r = (h-h_mid)/(height-h_mid);
			r = r*r;
			r = r*( r_end -r_mid );
			r += r_mid;
		}

		V3d pos;
		pos.x = (float)( pdat->x + r*cos(rot) );
		pos.y = h*-1.0f;
		pos.z = 0.3f*(float)sin(rot);
		pos.x	/= 240.0f;
		pos.y	/= 240.0f;

		strip.SetPos(i,pos);
		strip.SetColor(i,color);
		strip.SetWidth(i, width * u / 240.0f);
	}

	strip.SetWidth(STORM_NODE_NUM-1, 0);
}

DWORD CStorm::Draw()
{
	if(pdat->aid!=CBB_STATE_RUN)return TRUE;

	aki3d.EnableZ(TRUE,FALSE);
	aki3d.SetBlend_Add();

	for(int i=0;i<STORM_NUM;i++)
	{
		m_strips[i]->Render();
	}
	V3d tra((float)pdat->x/240.0f,(float)pdat->y/240.0f,pdat->z);
	D3DXMATRIX l2w = aki3d.CreateMatrix(NULL,NULL,&tra);
	m_fb->Render(&l2w);

	aki3d.SetBlend_Normal();
	aki3d.EnableZ();

	return TRUE;
	//return 0;
}

DWORD CStorm::Message(DWORD msg,LPVOID dat,DWORD prm)
{
	if(msg==GOBJMSG_FUTTOBI){
		GOBJECT* pobjdat = NULL;
		pobjdat = GetInfo(prm);
		if(!pobjdat)return 0;
		return CCharacter::eact_rotfuttobi(pobjdat);
	}

	return(CBulletBase::Message(msg,dat,prm));//キャラクター基本クラスに処理を任せる
}

DWORD CStorm::TouchC(ATTACKINFO *info,DWORD tc_id)
{
	//すでにこいつが攻撃にあたっているかどうか探す
	for(UINT i=0;i<m_list.size();i++)
	{
		if(m_list[i]==tc_id)
		{
			return TOUCHC_CANCEL;//すでにあたったやつにはあたらない
		}
	}

	m_list.push_back(tc_id);
	return TOUCHC_OK;
}

void CStorm::ParticleInfo::Reset()
{
	float rot = aki3d.RandomOne2()*3.1415f;
	float rad = 10.0f;

	pos.x = rad * sinf(rot);
	pos.z = rad * cosf(rot);
	pos.y = 0;

	pos /= 15.0f;

	ax1 = pos/3.0f;
	ax1.y = aki3d.RandomOne()*-0.6f;

	D3DXVec3Cross(&ax2,&ax1,&Yaxis);
}

/***************************************************************
	遠当て
****************************************************************/
CToate::CToate(CCharacterBase *parent,ATTACKINFO *aif) : CBulletBase(parent)
{
	atkinfo = *aif;

	m_particle = aki3d.CreateParticleObject();
	m_particle->Create( TOATE_NUM , "tex_wind.png" );
}

BOOL CToate::Go(BOOL muki,double x,double y,double vx,double vy)
{
	if(! CBulletBase::Go(muki,x,y,vx,vy) )return FALSE;

	m_list.clear();

	for(UINT i=0;i<TOATE_NUM;i++)
	{
		m_info[i].Reset();
	}
	return TRUE;
}

DWORD CToate::Draw()
{
	if(pdat->aid==CBB_STATE_IDLE)
	{
		return TRUE;
	}


	V3d scale( (pdat->muki ? 1.0f : -1.0f) , 1.0f , 1.0f );
	V3d trans( (float)pdat->x/240.0f , (float)pdat->y/240.0f , pdat->z );
	D3DXMATRIX matrix = aki3d.CreateMatrix( &scale, NULL, &trans );

	static BOOL err = FALSE;
	try{

	aki3d.SetBlend_Add();
	aki3d.EnableZ( TRUE, FALSE );
	if(!err)m_particle->Render( &matrix );
	aki3d.SetBlend_Normal();
	aki3d.EnableZ();

	}catch(...){err=TRUE;}

	return TRUE;
}

void CToate::act_run()
{
	if( ccounter==0 ){pdat->kougeki=TRUE;}
	cell( CELL_TOATE_ATK );
	
	const float limit = 8.0f;
	float t = (float)ccounter/limit;
	if(t>1.0f)Hide();

	ARGB col( (BYTE)(200*(1.0f-t)), 90,75,255);
	float width = 40.0f/240.0f*(0.5f+(1.0f-t)*0.5f);

	for(UINT i=0;i<TOATE_NUM;i++)
	{
		m_info[i].Update();

		m_particle->SetPos(i, m_info[i].pos);
		m_particle->SetAxis(i, m_info[i].pos*0.25f );
		m_particle->SetWidth(i, width );
		m_particle->SetColor(i, col );
	}

}

DWORD CToate::TouchC(ATTACKINFO *info,DWORD tc_id)
{
	//すでにこいつが攻撃にあたっているかどうか探す
	for(UINT i=0;i<m_list.size();i++)
	{
		if(m_list[i]==tc_id)
		{
			return TOUCHC_CANCEL;//すでにあたったやつにはあたらない
		}
	}

	m_list.push_back(tc_id);
	return TOUCHC_OK;
}

void CToate::ToateInfo::Reset()
{
	const float radius = 10.0f/240.0f;
	const float bara = 10.0f/240.0f;

/*	pos.x = bara * aki3d.RandomOne();
	pos.z = radius*aki3d.RandomOne2();
	pos.y = radius*aki3d.RandomOne2();

	D3DXVec3Cross(&vel,&pos,&Xaxis);
	vel *= 0.5f;
	vel.x = aki3d.RandomOne()*0.2f/240.0f;

	for(UINT i=0;i<TOATE_NODE_NUM;i++)
	{
		pos_arr[i] = pos;
		pos_arr[i].z /= 100.0f;
	}

	current_index = TOATE_NODE_NUM-1;*/

	pos.x = bara * aki3d.RandomOne();
	pos.z = radius*aki3d.RandomOne2();
	pos.y = radius*aki3d.RandomOne2();
}

void CToate::ToateInfo::Update()
{
/*	float velx = vel.x;
	vel -= pos*0.001f;
	vel.x = velx;*/

/*	D3DXVec3Cross(&vel,&pos,&Xaxis);
	D3DXVec3Normalize(&vel,&vel);
	vel *= 0.05f;
	vel.x = aki3d.RandomOne()*2.0f/240.0f;*/

	V3d vel;
	D3DXVec3Normalize(&vel,&pos);
	pos += vel*( 10.0f/240.0f );

}

