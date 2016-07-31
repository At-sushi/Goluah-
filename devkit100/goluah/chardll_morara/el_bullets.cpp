/*=====================================================================================

Goluah!! Copyright (C) 2001-2004 aki, 2004-2016 At-sushi, 2014-2016 logger

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

=======================================================================================*/

/*==============================================================

	�򓹋�

================================================================*/
#include "el_character.h"

V2d v2Zero;


/***************************************************************
	�򓹋����
****************************************************************/
void CCharacterEL::InitBullets()
{
	pbul_hadou = new CHadou(this);

	for(int i=0;i<EARROW_NUM;i++){
		pbul_earrow[i] = new CEnergyArrow(this);
		pbul_earrow[i]->SetAttackInfo( &aif[ATTACKINFO_EARROW] );
		pbul_earrow[i]->SetHitMessage(GOBJMSG_EARROWHIT, 1);
		pbul_earrow[i]->SetGuardMessage(GOBJMSG_EARROWHIT, 0);
	}

	pefe_track = new Morarael::CTrack(this);
	pefe_track->SetColor(100,255,20,20);

	pbul_judge = new CJudge(this);
	pbul_judge->SetAttackInfo( &aif[ATTACKINFO_JUDGE] );

	pefe_wind = new CDashWind(this);

	v2Zero.x = v2Zero.y = 0;
}


/***************************************************************
	�g���i��΂Ȃ��j
****************************************************************/

CHadou::CHadou(CCharacterBase *parent) : CBulletBase(parent)
{
	base_z = ZZAHYO_BULLET3;
	AddProperty( GOBJFLG_DONOTSTOP );
}

void CHadou::Hit()
{
	ChangeAction(CBB_STATE_BOMB);
	m_hit_count = pdat->counter + 1;
}

void CHadou::act_run()
{
	pdat->kougeki = (pdat->counter > 1) ? TRUE : FALSE;
	act_base();
}

void CHadou::act_bomb()
{
	pdat->kougeki = FALSE;
	if(pdat->counter < m_hit_count)pdat->counter = m_hit_count;
	act_base();
}

void CHadou::act_base()
{
	float t = ((float)pdat->counter/m_duration) ;//0��1
	if(t>1.0f){
		ChangeAction(CBB_STATE_IDLE);
		return;
	}

	cell(CELL_HADOU);

	pdat->magx = 0.25f + t*1.25f + 0.03f;
	pdat->magy = 1.0f-t + 0.03f;

	pdat->magx *= 0.75f;
}

void CHadou::GoA(BOOL muki,double x,double y,float duration,ATTACKINFO *atk)
{
	m_duration = duration;
	atkinfo = *atk;

	Go(muki,x,this->GetParentObj()->y+y,0);
}


/***************************************************************
	�e�g���O���}�g��
****************************************************************/

CEnergyArrow::CEnergyArrow(CCharacterBase *parent) : CBulletBase(parent)
{
	base_z = ZZAHYO_EFFECT2;
	AddProperty( GOBJFLG_NEEDTARGET );//�^�[�Q�b�g���K�v

	m_num =  64;			//�|�C���g��
	m_step =  1;			//1�t����Ԑ��E�E�E������Ԃ���˂�

	//�����p�����[�^
	m_width = 40.0f;		//�����
	m_color.red = 60;		//�F
	m_color.green = 60;
	m_color.blue = 255;
	m_color.alpha = 220;
	m_trackstrength = 2.85f;	//�ǔ�����
	m_maxspeed = 30.0f;		//�ő呬�x
	m_lifetime = 60.0f;

	m_pos = NULL;

	Create();
}


/*--------------------------------------------------------------
	��������
----------------------------------------------------------------*/
void CEnergyArrow::Create()
{
	m_ts = aki3d.CreateTristripObject();
	m_ts->Create(m_num,"morarael\\tex_tetra.png");
	m_pos = new V2d[m_num];
}

/*--------------------------------------------------------------
	�j������
----------------------------------------------------------------*/
void CEnergyArrow::Destroy()
{
	m_ts->Destroy();
	DELETEARRAY(m_pos);
}

/*--------------------------------------------------------------
	�ˏo
----------------------------------------------------------------*/
BOOL CEnergyArrow::Go(BOOL muki,double x,double y,double vx,double vy)
{
	if(!m_pos)return FALSE;
	if(!CBulletBase::Go(muki,x,y,vx,vy))return FALSE;

	for(UINT i=0;i<m_num;i++){//�|�C���g������
		m_pos[i].x = (float)x/ 240.0f;
		m_pos[i].y = (float)y/ 240.0f;
	}

	//��ԂɕK�v�ȃx�N�g��������
	m_ppos.x = (float)x / 240.0f;
	m_ppos.y = (float)y / 240.0f;
	m_pvec.x = m_pvec.y = 0;

	return TRUE;
}
	
/*--------------------------------------------------------------
	���Ē�
----------------------------------------------------------------*/
void CEnergyArrow::act_run()
{
	const UINT start_time = 10;
	float t = ((float)ccounter) / ((float)start_time);
	if(t>1.0f)t=1.0f;

	//�U���͐ݒ�
	if(ccounter<start_time) pdat->kougeki = FALSE;
	else pdat->kougeki = TRUE;

	hitprm = pdat->eid;
	grdprm = pdat->eid;
	//�\���Z���ݒ�
	cell( CELL_EARROW );

	//�G�ǔ�
	float t2 = ((float)ccounter) / m_lifetime;
	if(t2 < 0.3f){
		t2 /= 0.3f;
	}
	else {
		t2 = 1.0f;
	}
	float tstr = m_trackstrength * (1.0f - t2);
	if(tstr<0.2f) tstr = 0.2f;
	EnemyTrack( tstr );


	//�ʒu�ړ�
	pdat->x += pdat->vx;
	pdat->y += pdat->vy;

	pdat->muki = pdat->vx>0 ? FALSE : TRUE;
	pdat->magx = pdat->magy = 1.0f;// - t2;

	if(m_lifetime<ccounter)Bomb();

	UpdateTrack(t,FALSE);
}

/*--------------------------------------------------------------
	�q�b�g��/���������
----------------------------------------------------------------*/
void CEnergyArrow::act_bomb()
{
	const UINT stop_time = 60;
	float t = ((float)ccounter) / ((float)stop_time);

	if(t>1.0f){
		Hide();
		return;
	}

	//�\���Z���ݒ�
	cell( CELL_EARROW );

	//�G�ǔ�
	EnemyTrack( 0.2f );

	
	pdat->magx = pdat->magy = 1.0f - t;

	//�ʒu�ړ�
	pdat->x += pdat->vx;
	pdat->y += pdat->vy;

	pdat->magx = pdat->magy = 0.0f;

	UpdateTrack(t,TRUE);
}


/*--------------------------------------------------------------
	�G�ǔ�����
----------------------------------------------------------------*/
void CEnergyArrow::EnemyTrack(float strength)
{
	GOBJECT *pedat = NULL;
	pedat = GetInfo( pdat->eid );
	if(!pedat || pdat->eid==0)return;

	V2d spd;
	spd.x = (float)pdat->vx;
	spd.y = (float)pdat->vy;

	V2d sub;
	sub.x = (float)(pedat->x - pdat->x );
	sub.y = (float)(pedat->y - pdat->y );
	sub.Normalize();
	sub *= m_trackstrength;

	spd += sub;
	float spdlen = spd.Length();
	if(spdlen>m_maxspeed){
		spd *= m_maxspeed/spdlen; 
	}

	pdat->vx = spd.x;
	pdat->vy = spd.y;
}

/*--------------------------------------------------------------
	�g���C�X�g���b�v�X�V
----------------------------------------------------------------*/
void CEnergyArrow::UpdateTrack(float t,BOOL damp)
{
	UpdateBez();

	UINT i;
	float	t2;
	float	alpha_r = (damp ? (1.0f-t) : 1.0f);
	float	wdt = m_width*( damp ? (1.0f-t) : t )/ 240.0f;

	for(i=0;i<m_num;i++)
	{
		t2 = (float)i/(float)m_num;

		//�|�C���g�ݒ�
		V3d pos;
		pos.x = m_pos[i].x;
		pos.y = m_pos[i].y;
		pos.z = pdat->z;
		m_ts->SetPos(i,pos);
		
		//�F�ݒ�
		ARGB col = m_color;
		col.alpha = (BYTE)( ((float)col.alpha) * alpha_r ); 
		m_ts->SetColor(i,col);

		//�����ݒ�
		m_ts->SetWidth(i, wdt*(1.0f-t2) );
	}
	
	if(damp)m_ts->SetWidth(0,0);
}

void CEnergyArrow::UpdateBez()
{
	//�|�C���g�X�V
	static float vec_ratio = 0.5f;
	UINT i;
	{
		for(i=m_num-1;i>=m_step;i--)
		{
			float t = (1.0f/(float)(m_num-m_step))*(i);
			m_pos[i] = m_pos[i-m_step];
		}
		
		V2d crnt_pos;
		crnt_pos.x = (float)pdat->x/240.0f;
		crnt_pos.y = (float)pdat->y/240.0f;
		V2d crnt_vec = (crnt_pos - (m_ppos +m_pvec))*vec_ratio ;

		V2d v0 = m_ppos;
		V2d v1 = m_ppos + m_pvec;
		V2d v2 = crnt_pos - crnt_vec;
		V2d v3 = crnt_pos;

		for(i=0;i<m_step;i++)
		{
			float it = (1.0f/(float)m_step)*(i+1);
			float t= (1.0f-it);

			//�x�W�F�[
			m_pos[i].x=m_pos[i].y = 0;
			m_pos[i] += (v0*(it*it*it));
			m_pos[i] += (v1*(3.0f*t*it*it));
			m_pos[i] += (v2*(3.0f*t*t*it));
			m_pos[i] += (v3*(t*t*t));
		}
		m_ppos = crnt_pos;
		m_pvec = crnt_vec;
	}
}

/*--------------------------------------------------------------
	�`��
----------------------------------------------------------------*/
DWORD CEnergyArrow::Draw()
{
	if(pdat->aid!=CBB_STATE_IDLE)
	{
		aki3d.SetBlend_Add();
		aki3d.EnableZ(TRUE,FALSE);
		m_ts->Render();
		aki3d.SetBlend_Normal();
		aki3d.EnableZ(TRUE,TRUE);

		if(pdat->magx>0)
			return 0;//�Z���`����s��
		else
			return TRUE;
	}

	return TRUE;//�`��̕K�v�Ȃ�
}



/****************************************************************
	�O��
*****************************************************************/
using namespace Morarael;

CTrack::CTrack(CCharacterBase *parent) : CBulletBase(parent)
{
	m_ts = aki3d.CreateTristripObject();

	m_num = 64;
	m_step = 8;
	m_width = 50.0f;
	
	RemoveProperty(GOBJFLG_ATTACK);
	base_z = ZZAHYO_EFFECT2;

	Create();
}

/*---------------------------------------------------------------
	����
-----------------------------------------------------------------*/
void CTrack::Create()
{
	m_ts->Create(m_num,"gctrack.png");

/*	ARGB	*col = m_ts.GetColAddr();
	float	*rad = m_ts.GetRadAddr();
	for(UINT i=0;i<m_num;i++){
		col[i].alpha = 0;
		rad[i] = m_width/240.0f;
	}*/

	m_ppos = v2Zero;
	m_pvec = v2Zero;
	m_pos  = new V2d[m_num];

	m_width = 25.0f;
	m_color.red		= 255;
	m_color.green	= 255;
	m_color.blue	= 255;
	m_color.alpha	= 255;
}

/*---------------------------------------------------------------
	�j��
-----------------------------------------------------------------*/
void CTrack::Destroy()
{
	m_ts->Destroy();
	DELETEARRAY(m_pos);
}

/*---------------------------------------------------------------
	�|�C���g�ݒ�
-----------------------------------------------------------------*/
void CTrack::PosUpdate()
{
	if(pdat->aid==CBB_STATE_IDLE)return;
	
	//�p�����[�^�X�V
	m_rot += m_rotspd ;
	m_rotspd *= 1.0f-m_rotdamp;
	m_offset *= 1.0f-m_offsetdamp;

	float x = parentX() + m_offset * cosf(m_rot+m_inirot) * ((pdat->muki) ? 1.0f : -1.0f);
	float y = parentY() - m_offset2 + m_offset * sinf(m_rot+m_inirot);

/*	V2d		*pos = m_ts.GetPosAddr();
	ARGB	*col = m_ts.GetColAddr();
	float	*rad = m_ts.GetRadAddr();*/
	float	t;
	UINT i=0;
	
	static float vec_ratio = 0.5f;

	//�E�ʒu�X�V
	{
		for(i=m_num-1;i>=m_step;i--)
		{
			float t = (1.0f/(float)(m_num-m_step))*(i);
			m_pos[i] = m_pos[i-m_step];
		}
		
		V2d crnt_pos;
		crnt_pos.x = x/240.0f;
		crnt_pos.y = y/240.0f;
		V2d crnt_vec = (crnt_pos - (m_ppos +m_pvec))*vec_ratio ;

		V2d v0 = m_ppos;
		V2d v1 = m_ppos + m_pvec;
		V2d v2 = crnt_pos - crnt_vec;
		V2d v3 = crnt_pos;

		for(i=0;i<m_step;i++)
		{
			float it = (1.0f/(float)m_step)*(i+1);
			t= (1.0f-it);

			//�x�W�F�[
			m_pos[i]=v2Zero;
			m_pos[i] += (v0*(it*it*it));
			m_pos[i] += (v1*(3.0f*t*it*it));
			m_pos[i] += (v2*(3.0f*t*t*it));
			m_pos[i] += (v3*(t*t*t));
		
			//�P�c�ʒu�X�V
			m_tail++;
			if(m_tail>m_num)m_tail=m_num;
			//���ʒu�X�V
			if(pdat->aid==CBB_STATE_RUN)m_head = 0;
			else{
				m_head ++;
				if(m_head > m_num*2/3)m_head = m_num*2/3;
			}
		}
		m_ppos = crnt_pos;
		m_pvec = crnt_vec;
	}
	for(i=0;i<m_num;i++){
		V3d v3d;
		v3d.x = m_pos[i].x;
		v3d.y = m_pos[i].y;
		v3d.z = pdat->z;
		m_ts->SetPos(i,v3d);
	}

	//�E���A�F �ݒ�

	float wdt;
	if(pdat->aid==CBB_STATE_RUN){
		wdt=1.0f;
	}
	else {
		wdt = 1.0f - pdat->counter/(float)m_stopcount ;
		if(wdt<0.0f)wdt=0.0f;
	}
	wdt *= m_width/240.0f;

	if(m_tail<2){
		i=0;
	}
	for(i=0;i<m_head;i++)
	{
		t = ( 1.0f/m_head ) * i ;
		m_ts->SetWidth(i, wdt * t);
		m_ts->SetColor(i, m_color);
	}
	for(;i<m_tail;i++)
	{
		t = ( 1.0f/(m_tail-m_head) ) * ((m_tail-m_head)-(i-m_head)-1) ;
		m_ts->SetWidth(i, wdt * t);
		ARGB col = m_color;
		col.alpha = (BYTE)(m_color.alpha*t);
		m_ts->SetColor(i,col);
	}
	for(;i<m_num;i++){
		m_ts->SetWidth(i,0.0f);
		ARGB col;
		col.alpha = 0;
		m_ts->SetColor(i,col);
	}
}

/*---------------------------------------------------------------
	�`��
-----------------------------------------------------------------*/
DWORD CTrack::Draw()
{
	if(pdat->aid!=CBB_STATE_IDLE)
	{
		aki3d.SetBlend_Add();
		m_ts->Render();
		aki3d.SetBlend_Normal();
	}
	return TRUE;
}

/*---------------------------------------------------------------
	�s���֐�
-----------------------------------------------------------------*/
void CTrack::act_run()
{
	PosUpdate();

	if(pdat->counter > m_duration){
		ChangeAction(CBB_STATE_BOMB);
	}
}

void CTrack::act_bomb()
{
	PosUpdate();

	if(pdat->counter > m_stopcount){
		ChangeAction(CBB_STATE_IDLE);
	}
}

/*---------------------------------------------------------------
	Start/Stop
-----------------------------------------------------------------*/
void CTrack::Startup(
				float width,
				DWORD dur,
				DWORD stop,
				float inirot,
				float rotspd,
				float rotdamp,
				float offset,
				float offset_damp,
				float offset2,
				BOOL inv_flag)
{
	//�p�����[�^�R�s�[
	m_width = width;
	m_duration = dur;
	m_stopcount = stop;
	m_inirot = inirot;
	m_rotspd = rotspd;
	m_rotdamp = rotdamp;
	m_offset = offset;
	m_offsetdamp = offset_damp;
	m_offset2 = offset2;

	//�����p�����[�^���Z�b�g
	if(GetParentObj()->muki)inv_flag = !inv_flag;
	pdat->muki = inv_flag;
	m_rot = 0;
	m_ppos.x = (parentX() + offset*cosf(inirot) * (pdat->muki ? 1.0f : -1.0f) )/240.0f ;
	m_ppos.y = (parentY() - m_offset2 + offset*sinf(inirot) )/240.0f;
	m_pvec = v2Zero;
	m_tail = 0;
	m_head = 0;

	ChangeAction(CBB_STATE_RUN);
	PosUpdate();
}

/*---------------------------------------------------------------
	�p�����[�^�ݒ�
-----------------------------------------------------------------*/
void CTrack::SetColor(BYTE a,BYTE r,BYTE g,BYTE b)
{
	m_color.red		= r;
	m_color.green	= g;
	m_color.blue	= b;
	m_color.alpha	= a;
}


/***************************************************************
	�W���b�W�����g
****************************************************************/

CJudge::CJudge(CCharacterBase *parent) : CBulletBase(parent)
{
	m_tex = NULL;
	m_widthorg = 40;
	base_z = ZZAHYO_EFFECT2;
	m_tubu = aki3d.CreateParticleObject();

	Create();
}

void CJudge::Create()
{
	m_tex = aki3d.LoadTexture("morarael\\tex_beam.png");
	m_tubu->Create( JUDGE_TOGENUM, "morarael\\tex_tubu.png" );
}

void CJudge::Destroy()
{
	aki3d.UnloadTexture( m_tex );
}

BOOL CJudge::Go(BOOL muki,double x,double y,double vx,double vy)
{
	//�q�b�g���X�g�N���A
	m_hitlist.clear();

	//�J�n�_�擾
	m_start_x = (float)( GetParentObj()->x );
	m_start_y = (float)( GetParentObj()->y );

	//�Ƃ��Ƃ��p�����[�^���Z�b�g
	for(UINT i=0;i<JUDGE_TOGENUM;i++)
	{
		m_toge[i].Reset((float)x,pdat->z,muki);
		m_toge[i].time = m_toge[i].time_end*aki3d.RandomOne();
	}

	return CBulletBase::Go( muki, x, y, vx, vy);
}

void CJudge::act_run()
{
	if(pdat->counter==0)
	{
		AddEffect(EFCTID_BURN,0,0,pdat->id);
		AddEffect(EFCTID_BURN,0,0,pdat->id);
	}

	pdat->kougeki = TRUE;
	movex( pdat->vx );

	cell( CELL_JUDGE_HANTEI );

	m_width = m_widthorg;

	UpdateToge();

	if(ccounter>=30 || (GetParentObj()->aid!=ACTID_JUDGEMENT))
	{
		Bomb();
	}
}

void CJudge::act_bomb()
{
	float t = (float)ccounter / 20.0f;
	m_width = m_widthorg * (1.0f-t) ;

	UpdateToge();

	if(t>1.0f){
		Hide();
		return;
	}
}

DWORD CJudge::TouchC(ATTACKINFO *info,DWORD tc_eid)
{
	JudgeHitList::iterator i = FindHitInfo( tc_eid );
	if( i==m_hitlist.end() )
	{
		//������Ȃ�������V�K�ɒǉ�����
		JudgeHitInfo newInfo;
		newInfo.oid = tc_eid;
		newInfo.cnt = 1;
		m_hitlist.push_back( newInfo );
		return TOUCHC_OK;
	}

	//�����ɉ�����q�b�g���Ă�ꍇ�͓������ĂȂ����Ƃɂ���
	if( i->cnt >= JUDGE_MAXHIT ){
		return TOUCHC_CANCEL;
	}

	//�q�b�g�J�E���g�𑝂₷
	i->cnt ++;
	return TOUCHC_OK;
}

CJudge::JudgeHitList::iterator CJudge::FindHitInfo(DWORD oid)
{
	JudgeHitList::iterator i = m_hitlist.begin();
	JudgeHitList::iterator ie= m_hitlist.end();
	for(;i!=ie;i++)
	{
		if(i->oid == oid)break;
	}
	return i;
}


DWORD CJudge::Draw()
{
	if(pdat->aid==CBB_STATE_IDLE)
	{
		//�`��̕K�v�Ȃ�
		return TRUE;
	}
	LPDIRECT3DDEVICE9 d3ddev = aki3d.GetD3DDev();
	if(!d3ddev)return TRUE;//�`��ł��Ȃ�

	aki3d.EnableZ(TRUE,FALSE);

	//�Ԃ��r�[���̕`��
	MYVERTEX3D vb[4];
	D3DXMATRIX mat;	
	float m=1.0f;

	if(pdat->muki)m=-1.0f;
	vb[0].color=vb[1].color=vb[2].color=vb[3].color=0x88FFFFFF;
	vb[0].tu=0;
	vb[0].tv=0;
	vb[1].tu=1;
	vb[1].tv=0;
	vb[2].tu=0;
	vb[2].tv=1;
	vb[3].tu=1;
	vb[3].tv=1;

	vb[0].x= (float)( pdat->x + m_width*0.5f );
	vb[1].x= (float)( pdat->x - m_width*0.5f );
	vb[0].y= (float)( pdat->y );
	vb[1].y= (float)( pdat->y );

	vb[2].x= (float)( m_start_x + m_width*0.5f );
	vb[3].x= (float)( m_start_x - m_width*0.5f );
	vb[2].y= (float)( m_start_y - 60.0f);
	vb[3].y= (float)( m_start_y - 60.0f);

	for(int i=0;i<4;i++){
		vb[i].x /= 240.0f;
		vb[i].y /= 240.0f;
		vb[i].z = base_z;
	}

	D3DXMatrixIdentity(&mat);
	aki3d.SetBlend_Add();
	d3ddev->SetTransform(D3DTS_WORLD,&mat);
	d3ddev->SetTexture(0,m_tex);
	
	//�с[��
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
	
	//�n�ʂ𑖂�Ռ��g�̕`��
	m_tubu->Render();

	aki3d.SetBlend_Normal();
	aki3d.EnableZ();

	return(TRUE);//�Z���`��͕K�v�Ȃ�
	//return 0;//�e�X�g�p
}

DWORD CJudge::Sousai(DWORD prm)
{
	return(TRUE);
}

void CJudge::Hit()
{
	//�Ȃɂ����Ȃ�
}

void CJudge::UpdateToge()
{
	const float width = 0.5f;

	for(UINT i=0;i<JUDGE_TOGENUM;i++)
	{
		m_toge[i].time += 1;

		if( m_toge[i].time > m_toge[i].time_end )
		{
			m_toge[i].Reset((float)pdat->x,pdat->z,pdat->muki);
		}

		float t = m_toge[i].time / m_toge[i].time_end;
		ARGB color( 255,255,30,30 );
		color.alpha *= (BYTE)(color.alpha * 2.0f*t*(1.0f-t));

		V3d sub = (m_toge[i].dst - m_toge[i].base)*(0.25f+t);
		V3d pos = m_toge[i].dst*t + m_toge[i].base*(1.0f-t);

		m_tubu->SetPos(i, pos);
		m_tubu->SetAxis(i, sub*0.1f);
		m_tubu->SetWidth(i, width*(1.0f - t*t) );
		m_tubu->SetColor(i, color);
	}
}

void CJudge::TogeParam::Reset(float x,float z,BOOL muki)
{
	time = aki3d.RandomOne();
	time_end = 10.0f + aki3d.RandomOne()*10.0f;

	base.x = x/240.0f;
	base.y = 0;
	base.z = z;

	dst.x = aki3d.RandomOne()*0.8f*(muki ? 1:-1)+x/240.0f;
	dst.y = aki3d.RandomOne()*( -0.8f ) ;
	dst.z = (aki3d.RandomOne()-0.5f)+z;	
}

/***************************************************************
	�_�b�V�������G�t�F�N�g
****************************************************************/

CDashWind::CDashWind(CCharacterBase *parent) : CBulletBase(parent)
{
	RemoveProperty(GOBJFLG_ATTACK);
	base_z = ZZAHYO_EFFECT1;
}
	
void CDashWind::act_run()
{
	pdat->color= 0x00FFFFFF + (((255-(DWORD)(pdat->counter/30.0f*255.0f))*255*255*255)&0xFF000000);
	movex(pdat->vx);
	
	if(pdat->counter<10)		cell(CELL_KAZE1);
	else if(pdat->counter<20)	cell(CELL_KAZE2);
	else						cell(CELL_KAZE3);

	if(pdat->counter>30){
		ChangeAction(CBB_STATE_IDLE);
	}
}

void CDashWind::act_bomb()
{
	ChangeAction(CBB_STATE_IDLE);
}

