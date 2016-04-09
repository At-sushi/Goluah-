/*===============================================================

	その他エフェクト

=================================================================*/
#include "stdafx.h"
#include "efct_other.h"
#include "global.h"
#include "system_cell.h"



/****************************************************************
	超必の光
*****************************************************************/

BOOL CFxCyohiLight::static_initialized = FALSE;
MYVERTEX3D CFxCyohiLight::p_vbcl[CYOHILIGHT_LIFETIME][CYOHILIGHT_LINES*3];

CFxCyohiLight::CFxCyohiLight()
{
	if(!static_initialized){
		InitCyohiLightVertex();
		static_initialized =TRUE;
	}
}

void CFxCyohiLight::Update()
{
	if(pdat->counter >= CYOHILIGHT_LIFETIME)End();
}

void CFxCyohiLight::InitCyohiLightVertex()
{
	int i,j;

	float nagasa_wariai;
	float nagasa;
	double rot;
	double rot_prv;
	double sita;//θ

	double ng_s_min=0.2;//この長さ以下でθ一定
	double ng_s_max=1.0;//この長さ以上でθ一定
	double sita_max=30;//θmax

	MYVERTEX3D *pvb[3];

	srand(timeGetTime());

	for(j=0;j<CYOHILIGHT_LINES;j++){
		nagasa_wariai = (float)( 1.0 + (rand()%100)*0.004 );
		rot_prv = (360.0/CYOHILIGHT_LINES)*j;//初期回転角
		for(i=0;i<CYOHILIGHT_LIFETIME;i++){

			pvb[0] = &p_vbcl[i][j*3];
			pvb[1] = &p_vbcl[i][j*3+1];
			pvb[2] = &p_vbcl[i][j*3+2];

			//長さ
			if(i<10)nagasa = 0.8f - 0.03f*i;
			else nagasa = (0.8f - 0.03f*10) + 0.04f*(i-10);
			nagasa *= nagasa_wariai;

			//角度
			if(nagasa > ng_s_max)sita=0;
			else if(nagasa < ng_s_min)sita=sita_max;
			else {
				sita = (sita_max/(ng_s_max-ng_s_min))*nagasa - (ng_s_max*(sita_max/(ng_s_max-ng_s_min)));
			}

			//回転角度（長さから決める）
			rot = rot_prv + (0.8-nagasa)*7;

			//ざひょー
			pvb[0]->z = pvb[1]->z = pvb[2]->z = 0;
			pvb[0]->x = pvb[0]->y = 0;
			pvb[1]->x = nagasa*(float)sin( (rot+sita)/360*2*PI );
			pvb[1]->y = nagasa*(float)cos( (rot+sita)/360*2*PI );
			pvb[2]->x = nagasa*(float)sin( (rot-sita)/360*2*PI );
			pvb[2]->y = nagasa*(float)cos( (rot-sita)/360*2*PI );

			//色
			pvb[0]->color = pvb[1]->color = pvb[2]->color = 0xFF0022FF;
			pvb[0]->color = 0xAAFFFFFF;
			pvb[1]->color &= 0x00FFFFFF;
			pvb[2]->color &= 0x00FFFFFF;

			//UV座標
			pvb[0]->tu = pvb[1]->tu = pvb[2]->tu = 0;
			pvb[0]->tv = pvb[1]->tv = pvb[2]->tv = 0;

			rot_prv = rot;
		}
	}
}

BOOL CFxCyohiLight::Draw()
{
	UINT counter = pdat->counter;
	D3DXMATRIX matw;

	g_draw.SetAlphaMode(GBLEND_KASAN);
	if(counter<CYOHILIGHT_LIFETIME){
		g_draw.d3ddev->SetTexture(0,NULL);
		D3DXMatrixTranslation(&matw,(float)pdat->x/240.0f,(float)pdat->y/240.0f,0);
		g_draw.SetWorldMatrix(&matw);
		g_draw.d3ddev->SetRenderState(D3DRS_ZENABLE,FALSE);
		g_draw.d3ddev->DrawPrimitiveUP(
			D3DPT_TRIANGLELIST,
			CYOHILIGHT_LINES,p_vbcl[counter],sizeof(MYVERTEX3D));
			g_draw.d3ddev->SetRenderState(D3DRS_ZENABLE,TRUE);
	}
	if(counter<20)
	g_draw.DrawCircle((int)pdat->x,(int)pdat->y,//ひろがる
		counter*10,
		(20-counter)*2,0.02f,0xFF0055FF,TRUE);
	g_draw.SetAlphaMode(0);
	return(TRUE);
}

/****************************************************************
	単純な○の広がり
*****************************************************************/

void CFxCircle::Update()
{
	if(pdat->counter > 30)End();
	pdat->cnow=0;
}

BOOL CFxCircle::Draw()
{
	g_draw.DrawCircle((int)pdat->x,(int)pdat->y,pdat->counter*15,(30-pdat->counter)*2);
	return(TRUE);
}

/*****************************************************************
	粒々（ポイントスプライト）
******************************************************************/

CFxPSprite::CFxPSprite()
{
	m_part = aki3d.CreateParticleObject();
}

CFxPSprite::~CFxPSprite()
{
	aki3d.DestroyParticleObject(m_part);
}

BOOL CFxPSprite::Create(int prm1,int prm2,int prm3)
{
	if(SetParticleDat(prm1,prm2,(DWORD)prm3))
	{
		if(num>16)
		{
			g_system.Log(_T("CFxPSprite::Create numが16を超えています"),SYSLOG_WARNING);
			num = 16;
		}

		m_part->Create(num,_T("circle.png"));
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CFxPSprite::Update()
{
	BOOL tmp3=TRUE;
	BYTE tmp2;
	int tmp4;

	for(tmp2=0;tmp2<16;tmp2++){
		vb[tmp2].x += vx[tmp2];
		vb[tmp2].y += vy[tmp2];
		//α値を設定
		tmp4 = (255 / (life[tmp2]+1) )* pdat->counter;
		tmp4 = 255-tmp4;
		vb[tmp2].color &= 0x00FFFFFF;
		vb[tmp2].color |= (((BYTE)tmp4)*255*255*255)&0xFF000000;
		if(pdat->counter < life[tmp2])tmp3=FALSE;
		else vb[tmp2].color=0x00000000;
	}
	if(tmp3)End();
}

BOOL CFxPSprite::Draw()
{
	D3DXMATRIX matw;
	D3DXMatrixTranslation(&matw,center_x,center_y,0);
	g_draw.SetWorldMatrix(&matw);
	
	PointSetting();

	g_draw.SetAlphaMode(GBLEND_KASAN);//合成のモード
	g_draw.EnableZ(TRUE,FALSE);//Z書き込みOFF

	for(UINT i=0;i<num;i++)
	{
		V3d pos;
		pos.x = vb[i].x;
		pos.y = vb[i].y;
		pos.z = vb[i].z;
		m_part->SetPos( i, pos);
		m_part->SetAxis(i,Zero);
		m_part->SetColorDW(i,vb[i].color);
	}
	V3d vtrans;
	vtrans.x = center_x;
	vtrans.y = center_y;
	vtrans.z = pdat->z;
	D3DXMATRIX transm = aki3d.CreateMatrix( NULL, NULL, &vtrans);
	m_part->Render(&matw);

	g_draw.SetAlphaMode(0);//合成のモード
	g_draw.EnableZ();//Z書き込みON(規定値)

	return TRUE;
}

void CFxPSprite::PointSetting()
{
/*	float psize=3.0f;
	g_draw.d3ddev->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&psize));
	g_draw.d3ddev->SetTexture(0,NULL);*/

	for(UINT i=0;i<num;i++)
	{
		m_part->SetWidth( i, 4.0f/240.0f );
	}
}

BOOL CFxPSprite::SetParticleDat(int x,int y,DWORD prm)
{
	DWORD i, j;
	DWORD prme[8];
	for(i=0;i<8;i++){//パラメータを8つに分解
		prme[i] = prm;
		for(j=(7-i);j>0;j--){
			prme[i] /= 16;
		}
		prme[i] &= 0x0000000F;
	}
//a:粒の数(0～15)
//b;粒の速度
//c:粒の速度のばらつき
//d:最低速度
//e:初期位置のばらつき
//f:寿命のばらつき
//g:粒の最短寿命

	double kakudo;
	double sokudo;
	float hsokudo=0;
	num = prme[0]+1;
	if(num>16)return(FALSE);
	for(i=0;i<num;i++){
		kakudo = (3.1416*2.0/100.0)*(rand()%100);
		sokudo = 0.0002*(prme[1])*(rand()%(prme[2]+1)+prme[3]);
		vx[i] = (float)(sokudo*sin(kakudo));
		vy[i] = (float)(sokudo*cos(kakudo));
		vb[i].x = vx[i]*(prme[4]);
		vb[i].y = vy[i]*(prme[4]);
		vb[i].z = 0;
		vb[i].tu = vb[i].tv = 0;
		vb[i].color = 0xAAFF5500;
		life[i] = rand()%(prme[5]+1) +prme[6] +1;
		hsokudo += (float)sokudo;
	}

	hsokudo /= num;
	hsokudo *= 0.4f;
	if(prme[7]&TUBUTUBU_GOUP){
		for(i=0;i<num;i++){
			vy[i] -= hsokudo;
		}
	}
	if(prme[7]&TUBUTUBU_GODOWN){
		for(i=0;i<num;i++){
			vy[i] += hsokudo;
		}
	}
	if(prme[7]&TUBUTUBU_GOLEFT){
		for(i=0;i<num;i++){
			vx[i] -= hsokudo;
		}
	}
	if(prme[7]&TUBUTUBU_GORIGHT){
		for(i=0;i<num;i++){
			vx[i] += hsokudo;
		}
	}

	center_x = (float)x/240.0f;
	center_y = (float)y/240.0f;

	return(TRUE);
}

/*****************************************************************
	粒々（ポイントスプライト）・2
******************************************************************/
void CFxPSprite2::PointSetting()
{
/*	float psize=BIGTUBUTUBUSIZE;
	g_draw.d3ddev->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&psize));
	g_draw.d3ddev->SetTexture(0,GetTexture(SYSFXTEX_CIRCLE));*/

	for(UINT i=0;i<num;i++)
	{
		m_part->SetWidth( i,(BIGTUBUTUBUSIZE)/240.0f );
	}
}

/*****************************************************************
	もえー
******************************************************************/

BOOL CFxBurn::Create(int prm1,int prm2,int prm3)
{
	DWORD tgt = (DWORD)prm3;

	CGObject *pobj=battleTask->GetGObject(tgt);
	if(pobj==NULL)return(FALSE);

	GOBJECT *pdattgt = (GOBJECT*)battleTask->GetGObject(tgt);
	if(tgt==NULL)return(FALSE);

	aidprv = pdattgt->aid;
	tgtobjid = tgt;

	int i;
	int kakudo;
	float sokudo;

	for(i=0;i<NUM_FIRE;i++){
		ay[i] = -0.000002f*(rand()%10);
		kakudo = rand()%360;
		sokudo = 0.0001f + 0.00001f*(rand()%10);
		cx[i] = (float)pdattgt->x/240.0f + sokudo*(float)cos(D3DXToRadian(kakudo));
		cy[i] = ((float)pdattgt->y-50)/240.0f + sokudo*(float)sin(D3DXToRadian(kakudo));
		vb[i][0].z = 
			vb[i][1].z = 
			vb[i][2].z = 
			vb[i][3].z = 0.2f - 0.01f*(rand()%40);
		vx[i] = sokudo*(float)cos(D3DXToRadian(kakudo))*3;
		vy[i] = sokudo*(float)sin(D3DXToRadian(kakudo))*3;
		vb[i][0].tu = 0;
		vb[i][1].tu = 0;
		vb[i][2].tu = 1;
		vb[i][3].tu = 1;
		vb[i][0].tv = 0;
		vb[i][1].tv = 1;
		vb[i][2].tv = 0;
		vb[i][3].tv = 1;
		lifemax[i] = 10 + rand()%20;
		life[i] = rand()%lifemax[i];
		maxsize[i] = (0.025f + 0.001f*(rand()%10))/1.2f;
	}

	return(TRUE);
}


void CFxBurn::Update()
{
	if(pdat==NULL)return;
	
	GOBJECT *ptdat = NULL;
	if(pdat->aid==ACTID_EFCTBURN_ATOUCHED){
		ptdat = (GOBJECT*)battleTask->GetGObject(tgtobjid);
		if(ptdat==NULL){
			 pdat->aid=ACTID_EFCTBURN_DETOUCHED;
			 pdat->counter=0;
		}
		else if(ptdat->aid != aidprv){
			 pdat->aid=ACTID_EFCTBURN_DETOUCHED;
			 pdat->counter=0;
		}
	}
	pdat->objtype &= ~GOBJFLG_DONOTSTOP;

	int i;
	int kakudo;
	float sokudo;
	BYTE alpha;
	DWORD col;

	if(pdat->aid==ACTID_EFCTBURN_ATOUCHED){//キャラクターを黒くする
		ptdat->color = 0xFF775555;
	}

	for(i=0;i<NUM_FIRE;i++){
		vy[i] += ay[i];
		cx[i] += vx[i];
		cy[i] += vy[i];
		life[i]++;
		if(life[i]==lifemax[i] && pdat->aid==ACTID_EFCTBURN_ATOUCHED){
			ay[i] = -0.0002f*(rand()%10);
			kakudo = rand()%360;
			sokudo = 0.01f + 0.001f*(rand()%10);
			cx[i] = (float)ptdat->x/240.0f + sokudo*(float)cos(D3DXToRadian(kakudo))*3;
			cy[i] = ((float)ptdat->y-50)/240.0f + sokudo*(float)sin(D3DXToRadian(kakudo))*3;
			vb[i][0].z = 
				vb[i][1].z = 
				vb[i][2].z = 
				vb[i][3].z = 0.2f - 0.01f*(rand()%40);
			vx[i] = sokudo*(float)cos(D3DXToRadian(kakudo));
			vy[i] = sokudo*(float)sin(D3DXToRadian(kakudo))-sokudo/3;
			lifemax[i] = 10 + rand()%20;
			life[i] = 0;
			maxsize[i] = (0.05f + 0.002f*(rand()%10))/1.2f;
		}
		vb[i][0].x = cx[i] - (maxsize[i]/lifemax[i]*2.0f)*(lifemax[i]*2+life[i]);
		vb[i][1].x = cx[i] - (maxsize[i]/lifemax[i]*2.0f)*(lifemax[i]*2+life[i]);
		vb[i][2].x = cx[i] + (maxsize[i]/lifemax[i]*2.0f)*(lifemax[i]*2+life[i]);
		vb[i][3].x = cx[i] + (maxsize[i]/lifemax[i]*2.0f)*(lifemax[i]*2+life[i]);
		vb[i][0].y = cy[i] - (maxsize[i]/lifemax[i]*2.0f)*(lifemax[i]*2+life[i]);
		vb[i][1].y = cy[i] + (maxsize[i]/lifemax[i]*2.0f)*(lifemax[i]*2+life[i]);
		vb[i][2].y = cy[i] - (maxsize[i]/lifemax[i]*2.0f)*(lifemax[i]*2+life[i]);
		vb[i][3].y = cy[i] + (maxsize[i]/lifemax[i]*2.0f)*(lifemax[i]*2+life[i]);
		alpha = (256/lifemax[i]) * (lifemax[i] - life[i]);
		col = GetColor() | ( (alpha*256*256*256) & 0xFF000000);
		vb[i][0].color = 
			vb[i][1].color = 
			vb[i][2].color = 
			vb[i][3].color = col;
	}

	if(pdat->aid==ACTID_EFCTBURN_DETOUCHED){
		if(pdat->counter > EFCTBURN_MAXLIFE)End();
	}
}


BOOL CFxBurn::Draw()
{
	if(pdat==NULL)return TRUE;

	D3DXMATRIX mati;
	D3DXMatrixIdentity(&mati);
	g_draw.d3ddev->SetTransform(D3DTS_WORLD,&mati);
	g_draw.EnableZ(TRUE,FALSE);

	g_draw.SetAlphaMode(GBLEND_KASAN);
	g_draw.d3ddev->SetRenderState(D3DRS_ZENABLE,FALSE);

	g_draw.d3ddev->SetTexture(0,GetTexture(SYSFXTEX_FIRE));
	for(int i=0;i<NUM_FIRE;i++){
		if(life[i] < lifemax[i]){
			g_draw.d3ddev->DrawPrimitiveUP(
				D3DPT_TRIANGLESTRIP,2,vb[i],sizeof(MYVERTEX3D));
		}
	}
	g_draw.SetAlphaMode(0);
	g_draw.EnableZ();
	g_draw.d3ddev->SetRenderState(D3DRS_ZENABLE,TRUE);

	return TRUE;
}

/*****************************************************************
	"Come On"
******************************************************************/

void CFxComeon::Update()
{
	BYTE tmp2;

	if(pdat->muki){
		pdat->cnow = CELL_COMEONR;
		pdat->revx=TRUE;
		pdat->x++;
	}
	else{
		pdat->cnow = CELL_COMEONL;
		pdat->x--;
	}
	pdat->y--;
	tmp2 = (BYTE)(255 - 5*pdat->counter);
	pdat->color = 0x00FFFFFF + ((tmp2*256*256*256)&0xFF000000);
	if(pdat->counter>48)End();
}


/*****************************************************************
	ガードキャンセル
******************************************************************/

CFxGuardCancel::CFxGuardCancel()
{
	m_part = aki3d.CreateParticleObject();
}

CFxGuardCancel::~CFxGuardCancel()
{
	aki3d.DestroyParticleObject(m_part);
}

BOOL CFxGuardCancel::Create(int prm1,int prm2,int prm3)
{
	m_part->Create(GCANCEL_PARTICLE_NUM,_T("gcancel.png"));

	pdat->x = prm1;
	pdat->y = prm2;
	pdat->z = ZZAHYO_EFFECT1;
	pdat->muki = prm3;

	for(UINT i=0;i<GCANCEL_PARTICLE_NUM;i++)
	{
		m_info[i].Reset();
	}
	return TRUE;
}

void CFxGuardCancel::Update()
{
	float t = (float)pdat->counter/240.0f;
	t = 1.0f-t;
	DWORD col = GetColor() | 0xFF000000;

	for(UINT i=0;i<GCANCEL_PARTICLE_NUM;i++)
	{
		m_info[i].Update();

		m_part->SetWidth(i,m_info[i].wdt*t);
		m_part->SetAxis(i,m_info[i].ax);
		m_part->SetPos(i,m_info[i].pos);
		m_part->SetColorDW(i,col);
	}

	if(pdat->counter>=24)End();
}

BOOL CFxGuardCancel::Draw()
{
	aki3d.SetBlend_Add();
	aki3d.EnableZ(TRUE,FALSE);

	if(pdat->counter < 15){//円
		g_draw.DrawCircle((int)pdat->x,(int)pdat->y,(pdat->counter+4)*5,
			10,0,GetColor() + (((255-pdat->counter*(255/15))*255*255*255)&0xFF000000),TRUE);
	}

	V3d trans;
	trans.x = (float)pdat->x/240.0f;
	trans.y = (float)pdat->y/240.0f;
	trans.z = pdat->z;
	V3d sca;
	sca.x = pdat->muki ? -1.0f : 1.0f;
	sca.y = 1.0f;
	sca.z = 1.0f;
	D3DXMATRIX mat = aki3d.CreateMatrix(NULL,NULL,&trans);
	m_part->Render(&mat);

	aki3d.SetBlend_Normal();
	aki3d.EnableZ();

	return TRUE;
}

void CFxGuardCancel::CFxGuardCancel_PartInfo::Reset()
{
	pos = Zero;
	vel.x = aki3d.RandomOne2()*0.03f;
	vel.y = aki3d.RandomOne2()*0.03f;
	vel.z = 0;
	ax.x = aki3d.RandomOne()*1.0f/240.0f;
	ax.y = aki3d.RandomOne()*1.0f/240.0f;
	ax.z = 0;
	rspd = aki3d.RandomOne2()/PI*0.6f;

	wdt = 6.0f + 20.0f*aki3d.RandomOne();
	wdt /= 240.0f;
}

void CFxGuardCancel::CFxGuardCancel_PartInfo::Update()
{
	pos += vel;
	ax.x = ax.x*cosf(rspd) + ax.y*sinf(rspd);
	ax.y =-ax.x*sinf(rspd) + ax.y*cosf(rspd);
}


/*****************************************************************
	スーパーアーマー
******************************************************************/
void CFxSuperArmer::Update()
{
	float t = (float)pdat->counter/240.0f;
	t = 1.0f-t;
	DWORD col = GetColor() | 0xFF000000;

	for(UINT i=0;i<GCANCEL_PARTICLE_NUM;i++)
	{
		m_info[i].Update();

		m_part->SetWidth(i,m_info[i].wdt*t*0.5f);
		m_part->SetAxis(i,m_info[i].ax);
		m_part->SetPos(i,m_info[i].pos);
		m_part->SetColorDW(i,col);
	}

	if(pdat->counter>=24)End();
}


/*****************************************************************
	隠しエフェクト「Hatten ar din」
******************************************************************/
void CFxHatten::Update()
{
	double tmp;

	if(pdat->counter < 1024){
		pdat->cnow=CELL_HATTEN;
		pdat->x -= 4;
		tmp = ((timeGetTime()/20)*3)/360.0;
		tmp = sin(tmp*2*3.14);
		tmp *= 50;
		pdat->y = (int)(-360+tmp);
	}
	else End();
}
