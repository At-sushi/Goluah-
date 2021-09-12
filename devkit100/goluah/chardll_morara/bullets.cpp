
/*==============================================================

	飛び道具関係

================================================================*/
#include "character.h"

extern V2d v2Zero;

/*---------------------------------------------------------------
	飛道具初期化(CCharacter)
-----------------------------------------------------------------*/
void CCharacter::InitBullets()
{
	//地走り
	bul_chiba = new CChibasiri(this,m_opt_ChibasiriPlus);

	//G・カッター用軌跡
	efe_track = new CTrack(this);

	//ミニガン
	efe_minigun = new CMiniGunE(this);//エフェクト
	for(UINT i=0;i<NUM_MINIGUN;i++){
		bul_minigun[i] = new CMiniGunB(this);//弾
		bul_minigun[i]->Setup(&aif[ATTACKINFO_MINIGUN],0,0);
	}

	v2Zero.x = v2Zero.y = 0;
}


/****************************************************************
	地走り
*****************************************************************/

CChibasiri::CChibasiri(CCharacterBase *parent,BOOL ext) : CBulletBase(parent)
{
	m_extended = ext;
}

//設定
void CChibasiri::Startup(ATTACKINFO *patkinfo,DWORD hit_msg_id,DWORD grd_msg_id,	 
								BOOL muki,
								double x,
								double y,
								double vx)
{
	atkinfo = *patkinfo;
	SetHitMessage(hit_msg_id,0);
	SetGuardMessage(hit_msg_id,0);

	base_z = ZZAHYO_BULLET3;

	Go(muki,x,y,vx);
}

//飛翔(?)中
void CChibasiri::act_run()
{
	DWORD run_len = m_extended ? 50 : 25 ;

	if(pdat->counter < run_len){
		pdat->atk = &atkinfo;
		pdat->kougeki=TRUE;
		movex( pdat->vx );
	}
	else{
		float cnt2 = (float)(pdat->counter - run_len);
		if(cnt2>10)Hide();

		pdat->kougeki=FALSE;
		pdat->magy = (10.0f-cnt2)/10.0f;
		pdat->magx = 1.0f+cnt2/20.0f;
		movex( 6 );
	}

	pdat->cnow = CELL_BUL_CHIBA1 + (pdat->counter)%4;
}

//敵にヒット後
void CChibasiri::act_bomb()
{
	float cnt2 = (float)pdat->counter ;
	pdat->magx = (5.0f-cnt2)/5.0f;
	pdat->magy = 1.0f+cnt2/10.0f;
	movex( 6 );

	if(cnt2>5)Hide();
}



/****************************************************************
	軌跡
*****************************************************************/

CTrack::CTrack(CCharacterBase *parent) : CBulletBase(parent)
{
	m_num = 42;
	m_step = 6;
	m_width = 50.0f;
	
	RemoveProperty(GOBJFLG_ATTACK);
	base_z = ZZAHYO_EFFECT2;

	m_ts = aki3d.CreateTristripObject();

	Create();
}

/*---------------------------------------------------------------
	生成
-----------------------------------------------------------------*/
void CTrack::Create()
{
	m_ts->Create(m_num,"gctrack.bmp");

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
	破棄
-----------------------------------------------------------------*/
void CTrack::Destroy()
{
	m_ts->Destroy();
	DELETEARRAY(m_pos);
}

/*---------------------------------------------------------------
	ポイント設定
-----------------------------------------------------------------*/
void CTrack::PosUpdate()
{
	if(pdat->aid==CBB_STATE_IDLE)return;
	
	//パラメータ更新
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

	//・位置更新
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

			//ベジェー
			m_pos[i] = v2Zero;
			m_pos[i] += (v0*(it*it*it));
			m_pos[i] += (v1*(3.0f*t*it*it));
			m_pos[i] += (v2*(3.0f*t*t*it));
			m_pos[i] += (v3*(t*t*t));
		
			//ケツ位置更新
			m_tail++;
			if(m_tail>m_num)m_tail=m_num;
			//頭位置更新
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
		V3d pos;
		pos.x = m_pos[i].x;
		pos.y = m_pos[i].y;
		pos.z = pdat->z;
		m_ts->SetPos(i,pos);
	}

	//・幅、色 設定

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
		m_ts->SetWidth(i, wdt * t );
		m_ts->SetColor(i, m_color );
	}
	for(;i<m_tail;i++)
	{
		t = ( 1.0f/(m_tail-m_head) ) * ((m_tail-m_head)-(i-m_head)-1) ;
		m_ts->SetWidth(i, wdt * t);
		ARGB col = m_color;
		col.alpha = (BYTE)(m_color.alpha*t);
		m_ts->SetColor(i,col);
	}
	for(;i<m_num;i++)
	{
		ARGB col;
		col.alpha = 0;
		m_ts->SetWidth(i,  0.0f);
		m_ts->SetColor(i,  col);
	}
}

/*---------------------------------------------------------------
	描画
-----------------------------------------------------------------*/
DWORD CTrack::Draw()
{
	if(pdat->aid!=CBB_STATE_IDLE)m_ts->Render();
	return TRUE;
}

/*---------------------------------------------------------------
	行動関数
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
	//パラメータコピー
	m_width = width;
	m_duration = dur;
	m_stopcount = stop;
	m_inirot = inirot;
	m_rotspd = rotspd;
	m_rotdamp = rotdamp;
	m_offset = offset;
	m_offsetdamp = offset_damp;
	m_offset2 = offset2;

	//内部パラメータリセット
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
	パラメータ設定
-----------------------------------------------------------------*/
void CTrack::SetColor(BYTE a,BYTE r,BYTE g,BYTE b)
{
	m_color.red		= r;
	m_color.green	= g;
	m_color.blue	= b;
	m_color.alpha	= a;
}


/****************************************************************
	ミニガン（弾）
*****************************************************************/
CMiniGunB::CMiniGunB(CCharacterBase *parent) : CBulletBase(parent)
{
}

void CMiniGunB::Setup(
			ATTACKINFO *patkinfo,
			DWORD hit_msg_id,
			DWORD grd_msg_id
			)
{
	atkinfo = *patkinfo;
	SetHitMessage(hit_msg_id,0);
	SetGuardMessage(hit_msg_id,0);
}

void CMiniGunB::act_run()
{
	pdat->kougeki = TRUE;
	cell(CELL_GUNBULLET);
	movex(pdat->vx);
	if(pdat->counter>8)Hide();
}

void CMiniGunB::act_bomb()
{
	Hide();
}

/****************************************************************
	ミニガン（エフェクト）
*****************************************************************/
CMiniGunE::CMiniGunE(CCharacterBase *parent) : CBulletBase(parent)
{
	base_z = ZZAHYO_BULLET3;
}

void CMiniGunE::act_run()
{
	cell(CELL_GUNSPARK1+pdat->counter%4);

	pdat->magx = 1.5f + 1.5f*(aki3d.RandomOne()-0.5f);
	pdat->magy = 1.5f + 1.5f*(aki3d.RandomOne()-0.5f);
	if(pdat->counter>3)Hide();
}

