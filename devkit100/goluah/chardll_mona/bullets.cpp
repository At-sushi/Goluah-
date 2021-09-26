
/*==============================================================

        飛道具クラス定義

================================================================*/
#include "character.h"

/***************************************************************
        飛道具の初期化(CCharacter)
****************************************************************/
void CCharacter::InitBullets() {
  // 注：飛道具クラスはGOBJMSG_DELETEメッセージで CGoluahObject の
  // gMessageToObject関数内で破棄されるので、deleteしてはいけない。

  pefe_re = new CReiku_Re(this);
  pefe_i = new CReiku_I(this);
  pefe_ku = new CReiku_Ku(this);
  pefe_dash = new CDashWind(this);
  /*	pbul_rocket = new CRocket(this,aif[ATTACKINFO_ROCKET]);
          pbul_eraser99 =		new CEraser99(this,aif[ATTACKINFO_ERASER99]);
          pbul_eraser99->SetRocket( pbul_rocket );*/

  if (m_opt_hadou)
    pbul_hadou = new CHadouKenB(this);
  else
    pbul_hadou = new CHadouKenA(this);
  pbul_hadou->SetHitMessage(GOBJMSG_HADOUHIT, 0);
  pbul_hadou->SetHitMessage(GOBJMSG_HADOUGUARD, 0);

  pbul_goodbye = new CGoodbye(this);
}

/***************************************************************
        「レイク」文字エフェクト
****************************************************************/

CReiku_Base::CReiku_Base(CCharacter *parent) : CBulletBase(parent) {
  RemoveProperty(GOBJFLG_ATTACK);
  base_z = ZZAHYO_EFFECT2;
}

void CReiku_Base::act_run() {
  pdat->cnow = m_reiku_cell;
  pdat->color = 0x00FFFFFF + (((255 - pdat->counter * 25) * 255 * 255 * 255) & 0xFF000000);
  pdat->magx = pdat->magy = (float)(1.0 + 0.05 * pdat->counter);
  if (pdat->counter > 8)
    ChangeAction(CBB_STATE_IDLE);
}

void CReiku_Base::act_bomb() { ChangeAction(CBB_STATE_IDLE); }

//レ
CReiku_Re::CReiku_Re(CCharacter *parent) : CReiku_Base(parent) { m_reiku_cell = CELL_REIKU_RE0; }

//イ
CReiku_I::CReiku_I(CCharacter *parent) : CReiku_Base(parent) { m_reiku_cell = CELL_REIKU_I0; }

//ク
CReiku_Ku::CReiku_Ku(CCharacter *parent) : CReiku_Base(parent) { m_reiku_cell = CELL_REIKU_KU0; }

/***************************************************************
        ダッシュ時風エフェクト
****************************************************************/

CDashWind::CDashWind(CCharacter *parent) : CBulletBase(parent) {
  RemoveProperty(GOBJFLG_ATTACK);
  base_z = ZZAHYO_EFFECT1;
}

void CDashWind::act_run() {
  pdat->cnow = CELL_DASHWIND;
  pdat->color =
      0x00FFFFFF + (((255 - (DWORD)(pdat->counter / 30.0f * 255.0f)) * 255 * 255 * 255) & 0xFF000000);
  movex(pdat->vx);

  if (pdat->counter > 30) {
    ChangeAction(CBB_STATE_IDLE);
  }
}

void CDashWind::act_bomb() { ChangeAction(CBB_STATE_IDLE); }

/***************************************************************
        波動拳（飛ばない）
****************************************************************/

CHadouKenA::CHadouKenA(CCharacter *parent) : CHadouKen(parent) {
  base_z = ZZAHYO_BULLET3;
  AddProperty(GOBJFLG_DONOTSTOP);
}

void CHadouKenA::Hit() {
  ChangeAction(CBB_STATE_BOMB);
  m_hit_count = pdat->counter + 1;
}

void CHadouKenA::act_run() {
  pdat->kougeki = (pdat->counter > 1) ? TRUE : FALSE;
  act_base();
}

void CHadouKenA::act_bomb() {
  pdat->kougeki = FALSE;
  if (pdat->counter < m_hit_count)
    pdat->counter = m_hit_count;
  act_base();
}

void CHadouKenA::act_base() {
  float t = ((float)pdat->counter / m_duration); // 0→1
  if (t > 1.0f) {
    ChangeAction(CBB_STATE_IDLE);
    return;
  }

  cell(CELL_HADOU_A2);

  pdat->magx = 0.25f + t * 1.25f + 0.03f;
  pdat->magy = 1.0f - t + 0.03f;
}

void CHadouKenA::GoA(BOOL muki, double x, double y, float duration, ATTACKINFO *atk) {
  m_duration = duration;
  atkinfo = *atk;

  Go(muki, x, y, 0);
}

/***************************************************************
        波動拳（飛ぶ）
****************************************************************/

CHadouKenB::CHadouKenB(CCharacter *parent) : CHadouKen(parent) { base_z = ZZAHYO_BULLET3; }

void CHadouKenB::act_run() {
  pdat->kougeki = TRUE;
  cell(CELL_HADOU_B1 + (ccounter / 2) % 6);

  movex(pdat->vx);
  if (ccounter > 60)
    ChangeAction(CBB_STATE_BOMB);
}

void CHadouKenB::act_bomb() {
  if (ccounter < 8)
    cell(CELL_HADOU_B7);
  else if (ccounter < 16)
    cell(CELL_HADOU_B8);
  else
    ChangeAction(CBB_STATE_IDLE);

  float dur = 8.0f;
  float t = (ccounter / dur);

  if (t > 1.0f)
    ChangeAction(CBB_STATE_IDLE);
  else {
    DWORD alpha = (DWORD)(255 * (t * 0.8f));
    pdat->color = 0x00FFFFFF | (alpha << 24);

    pdat->magx = 1.0f + t * 0.5f;
    pdat->magy = 1.0f + t * 0.5f;
  }
}

void CHadouKenB::GoB(BOOL muki, double x, double y, double vx, ATTACKINFO *atk) {
  RemoveProperty(GOBJFLG_DONOTSTOP);
  atkinfo = *atk;
  Go(muki, x, y, vx);
}

void CHadouKenB::Hit() {
  CBulletBase::Hit();
  AddProperty(GOBJFLG_DONOTSTOP);
}

/***************************************************************
        グッバイ・フォーエバー
****************************************************************/
CGoodbye::CGoodbye(CCharacter *parent) : CHadouKenA(parent) {}

void CGoodbye::act_base() {
  CHadouKenA::act_base();

  pdat->magx *= 4.0f;
  pdat->magy *= 4.0f;

  pdat->rot = -75;

  cell((ccounter % 2) ? CELL_HADOU_A1 : CELL_HADOU_A2);

  if (ccounter < 6)
    pdat->kougeki = TRUE;
  else
    pdat->kougeki = FALSE;
}

void CGoodbye::Hit() { return; }

void CGoodbye::GoA(BOOL muki, double x, double y, float duration, ATTACKINFO *atk) {
  CHadouKenA::GoA(muki, x, y, duration, atk);
}

/*---------------------------------------------------------------
        ロケット?
-----------------------------------------------------------------*/
/*CRocket::CRocket(CCharacterBase* parent,ATTACKINFO& atk) : CBulletBase(parent)
{
        atkinfo = atk;
        base_z = ZZAHYO_BULLET3;

        part_bomb = aki3d.CreateParticleObject();
        part_bomb->Create( ROCKET_BOMB_NUM, "tex_bomb1.png" );
}

void CRocket::act_run()
{
        RemoveProperty( GOBJFLG_DONOTSTOP );

        if(ccounter==0)pdat->vx = 0;

        cell( CELL_ROCKET );
        pdat->kougeki = TRUE;

        pdat->vx++;
        movex( pdat->vx );

        if(ccounter>200)Bomb();
}

void CRocket::act_bomb()
{
        UINT i;

        if(ccounter==0)
        {
                for(i=0;i<ROCKET_BOMB_NUM;i++)
                {
                        m_info[i].Reset();
                }
        }

        V3d pos;
        float ar = 1.0f/240.0f;

        for(i=0;i<ROCKET_BOMB_NUM;i++)
        {
                m_info[i].Update();

                pos = m_info[i].pos;
                pos.x *= ar;
                pos.y *= ar;
                pos.z *= ar;

                part_bomb->SetPos(i,pos);
                part_bomb->SetWidth(i,m_info[i].width*ar);
                part_bomb->SetAxis(i,Zero);
                part_bomb->SetColor(i, m_info[i].color);
        }

        if(pdat->counter > 30)Hide();
}

void CRocket::PBombInfo::Reset()
{
        pos = Zero;
        vel.x = aki3d.RandomOne2()*8.0f;
        vel.y = aki3d.RandomOne2()*8.0f;
        vel.z = aki3d.RandomOne2()*8.0f;
        width_base = aki3d.RandomOne()*100.0f+300.0f;
        time = 0;
        time_limit = aki3d.RandomOne()*8.0f + 22.0f;
        color.red = 255;
        color.green = 100;
        color.blue= 70;
}

void CRocket::PBombInfo::Update()
{
        time += 1.0f;
        if( time > time_limit ){
                //Reset();
                color.alpha = 0;
                return;
        }
        float t = time/time_limit;

        vel.y -= 0.3f;
        pos   += vel;

        width = width_base * (t*0.5f+0.5f);
        color.alpha = (BYTE)( 255.0f * (1.0f-t) );
}

DWORD CRocket::Draw()
{
        if(pdat->aid==CBB_STATE_BOMB)
        {
                aki3d.SetBlend_Add();
                aki3d.EnableZ( TRUE, FALSE );
                V3d tra((float)pdat->x/240.0f,(float)pdat->y/240.0f,pdat->z);
                V3d sca((pdat->muki ? -1.0f:1.0f) , 1.0f,1.0f);
                D3DXMATRIX l2w = aki3d.CreateMatrix( &sca, NULL, &tra );
                part_bomb->Render(&l2w);
                aki3d.EnableZ();
                aki3d.SetBlend_Normal();
        }
        else if(pdat->aid==CBB_STATE_RUN)
        {
                return 0;
        }

        return TRUE;
}*/

/*---------------------------------------------------------------
        イレイサー"99"
-----------------------------------------------------------------*/

/*CEraser99::CEraser99(CCharacterBase* parent,ATTACKINFO& atk) : CBulletBase(parent)
{
        atkinfo = atk;

        m_part = aki3d.CreateParticleObject();
        m_part->Create( ERASER99_NUM_CONPART, "tex_conpart.png");

        m_part_ray = aki3d.CreateParticleObject();
        m_part_ray->Create( ERASER99_NUM_RAY, "tex_tubu.png");
}

void CEraser99::act_run()
{
        UINT i;

        if(ccounter==0)
        {
                for(i=0;i<ERASER99_NUM_CONPART;i++)
                {
                        m_info[i].Reset();
                }
        }

        float t = (float)ccounter/15.0f;

        ARGB col;
        col.blue = 255;
        col.red = 40;
        col.green = 40;
        col.alpha = (BYTE)( 255.0f*( t<0.25f ? t*4.0f : 1.0f ) );

        float wscale = 1.0f;
        wscale += t*t*6.0f;

        for(i=0;i<ERASER99_NUM_CONPART;i++)
        {
                m_info[i].Update();

                float width;
                if(i<ERASER99_NUM_CONPART/2)width = 10.0f/240.0f;
                else if(i<ERASER99_NUM_CONPART/4*3)width = 20.0f/240.0f;
                else width = 30.0f/240.0f;

                m_part->SetPos(i, m_info[i].pos);
                m_part->SetAxis(i, m_info[i].vel*0.5f);
                m_part->SetWidth(i, width*wscale);
                m_part->SetColor(i,col);
        }

        if( ccounter>15 )Bomb();
}

void CEraser99::act_bomb()
{
        UINT i;

        if(ccounter==0)
        {
                AddEffect( EFCTID_SINDO, 10, 50, 0);

                for(i=0;i<ERASER99_NUM_CONPART;i++)
                {
                        m_info[i].Reset2();
                }

                for(i=0;i<ERASER99_NUM_RAY;i++)
                {
                        m_info_ray[i].Reset();
                }

                obj_rkt->Bomb();
                obj_rkt->SetPos(pdat->x,pdat->y);
        }

        float t = (float)ccounter / 80.0f;

        ARGB col;
        col.blue = 255;
        col.red = 40;
        col.green = 40;
        col.alpha = (BYTE)(250*(1.0f-t));

        for(i=0;i<ROCKET_BOMB_NUM;i++)
        {
                obj_rkt->m_info[i].color.red = col.red;
                obj_rkt->m_info[i].color.green = col.green;
                obj_rkt->m_info[i].color.blue = col.blue;
        }

        for(i=0;i<ERASER99_NUM_CONPART;i++)
        {
                m_info[i].Update2();

                m_part->SetPos(i, m_info[i].pos);
                m_part->SetAxis(i, m_info[i].vel*2.5f);
                m_part->SetWidth(i, 5.0f/240.0f );
                m_part->SetColor(i,col);
        }

        for(i=0;i<ERASER99_NUM_RAY;i++)
        {
                m_info_ray[i].Update(1.0f-t);

                m_part_ray->SetPos(i,m_info_ray[i].pos+m_info_ray[i].ax);
                m_part_ray->SetAxis(i,m_info_ray[i].ax*0.5f);
                m_part_ray->SetWidth(i,m_info_ray[i].width);
                m_part_ray->SetColor(i,m_info_ray[i].col);
        }

        cell( CELL_ERASER_HANTEI );
        pdat->kougeki = (ccounter<40) ? TRUE : FALSE;
        if(ccounter>80)Hide();
}

void CEraser99::ParticleInfo::Reset()
{
        pos.x = aki3d.RandomOne2()*400.0f;
        pos.y = aki3d.RandomOne2()*400.0f;
        pos.z = aki3d.RandomOne2()*200.0f;

        pos /= 240.0f;

        vel = Zero;
}

void CEraser99::ParticleInfo::Update()
{
        V3d acc;
        D3DXVec3Cross(&acc,&pos,&Xaxis);
        acc *= 0.3f/240.0f/D3DXVec3Length(&acc);
        acc -= pos*1.0f/240.0f/D3DXVec3Length(&pos);

        vel += acc;
        vel *= 0.8f;
        pos += vel;

        if(D3DXVec3Length(&pos)<20.0f/240.0f)
        {
                vel *= 2.0f;
        }
}

void CEraser99::ParticleInfo::Reset2()
{
        vel.x = (aki3d.RandomOne2()-0.5f)*8.0f;
        vel.y = (aki3d.RandomOne2()-0.5f)*8.0f;
        vel.z = aki3d.RandomOne2()*8.0f;

        vel/=240.0f;

        pos = Zero;
}

void CEraser99::ParticleInfo::Update2()
{
        pos += vel;
}

DWORD CEraser99::Draw()
{
        if(pdat->aid!=CBB_STATE_IDLE)
        {
                aki3d.SetBlend_Add();
                aki3d.EnableZ( TRUE, FALSE );
                V3d tra((float)pdat->x/240.0f,(float)pdat->y/240.0f,pdat->z);
                V3d sca((pdat->muki ? -1.0f:1.0f) , 1.0f,1.0f);
                D3DXMATRIX l2w = aki3d.CreateMatrix( &sca, NULL, &tra );
                m_part->Render(&l2w);
                if(pdat->aid==CBB_STATE_BOMB)
                {
                        m_part_ray->Render(&l2w);
                }
                aki3d.EnableZ();
                aki3d.SetBlend_Normal();
        }

        return TRUE;
}

void CEraser99::RayPartInfo::Reset()
{
        ax_base.x = (aki3d.RandomOne())*640.0f;
        ax_base.y = (aki3d.RandomOne2())*250.0f;
        ax_base.z =	 aki3d.RandomOne2()*50.0f;

        ax_base /=240.0f;

        pos = Zero;
        pos.x = -70.0f*(aki3d.RandomOne()-0.25f);
        pos/=240.0f;

        width_base = (250.0f + aki3d.RandomOne()*150.0f)/240.0f;

        time = 0;
        time_end = 5.0f+10.0f*aki3d.RandomOne();

        col.blue = 255;
        col.red = 40;
        col.green = 40;
}

void CEraser99::RayPartInfo::Update(float pt)
{
        time += 1.0f;
        if(time > time_end)Reset();
        float t = time/time_end;

        ax = ax_base*(0.5f+t)*pt;
        width = width_base*(0.55f+t)*pt;

        pos += ax*0.05f;

        col.alpha = (BYTE)( 255.0f*(1.0f-t)*pt);
}*/
