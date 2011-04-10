/*===============================================================

	システムエフェクト

=================================================================*/
#include "stdafx.h"
#include "efct_sys.h"
#include "global.h"
#include "system_cell.h"


/*****************************************************************
	ラウンド表示ベース
******************************************************************/

void CFxRoundBase::Update()
{
	if(pdat->counter > 40 + 12 -1)End();
}

BOOL CFxRoundBase::DrawF()
{
	g_draw.SetTransform(FALSE);

	float t2;
	if(pdat->counter < 20){
		t2 = 10.0f - pdat->counter/2.0f;
	}
	else if(pdat->counter < 40){
		t2 = 1.0f;
	}
	else{
		t2 = 1.0f - (pdat->counter-40)/12.0f ;
	}

	char str[] = "ROUND";
	const float interval = 5;
	const float duration = 6;
	const float y		 = 200;
	const float left	 = 80;
	const float space    = (640-left*2)/6;
	float t;
	float x = left;
	for(UINT i=0;i<5;i++){
		t = (pdat->counter-interval*i) / duration;
		DrawOneCharacter(x,y,str[i],t,t2);
		x += space;
	}

	t = (pdat->counter-interval*i) / duration;
	DrawOneCharacter(x,y,round_number,t,t2);

	g_draw.SetTransform(TRUE);

	return FALSE;
}

void CFxRoundBase::DrawOneCharacter(float x,float y,char c,float t,float t2)
{
	if(t<0.0f)return;
	if(t>1.0f)t=1.0f;

	DWORD col;
	char str[2] = {c,'\0'};
	float tx,ty,scaX,scaY;

	//大体
	const float font_size = 60;

	scaX = t*4.5f;
	scaY = ((0.5f + 0.5f*t)*4.0f )* t2;
	tx = x ;
	ty = y - font_size*t2;

	col = (DWORD)(170*t*t2/4);//alpha
	col <<= 24;
	col |= 0x004411FF;

	g_draw.SetAlphaMode(GBLEND_HANTOUMEI);
//	g_system.DrawBMPTextEx(tx  ,ty  ,0.0f,str,col,scaX,scaY,0);
	g_system.DrawBMPTextEx(tx+t,ty+t,0.0f,str,col,scaX,scaY,0);
	g_system.DrawBMPTextEx(tx+t,ty-t,0.0f,str,col,scaX,scaY,0);
	g_system.DrawBMPTextEx(tx-t,ty+t,0.0f,str,col,scaX,scaY,0);
	g_system.DrawBMPTextEx(tx-t,ty-t,0.0f,str,col,scaX,scaY,0);

	g_draw.EnableZ(FALSE,FALSE);

	col = (DWORD)(128*t*t2);//alpha
	col <<= 24;
	col |= 0x004411FF;

	g_draw.SetAlphaMode(GBLEND_KASAN);
	g_system.DrawBMPTextEx(tx,ty,0.0f,str,col,scaX,scaY,0);

	g_draw.EnableZ(TRUE,TRUE);
	g_draw.SetAlphaMode(0);
}

/*****************************************************************
	KO
******************************************************************/
void CFxKO::Update()
{
	if(pdat->counter > 120)End();

	if(pdat->counter < 30){
		m_t = pdat->counter /30.0f;
	}
	else if(pdat->counter < 90){
		m_t = 1.0f;
	}
	else{
		m_t = 1.0f - (pdat->counter-90) /30.0f;
	}
}

BOOL CFxKO::DrawF()
{
	g_draw.SetTransform(FALSE);

	//LTMマトリクス設定(単位行列)
	D3DXMATRIX mati;
	D3DXMatrixIdentity(&mati);
	g_draw.d3ddev->SetTransform(D3DTS_WORLD,&mati);

	float t;
	float t2 = m_t<0.5f ? m_t*2.0f : 1.0f-(m_t-0.5f)*2.0f;
	float x,y;
	float w,h;
	UINT i;

	MYVERTEX3D v1[4];
	MYVERTEX3D v2[4];
	MYVERTEX3D *v;

	//頂点色設定
	v1[0].color = (DWORD)(128*m_t);
	v1[0].color <<= 24;
	v1[0].color |= 0x00FF3333;
	v1[1].color = v1[2].color = v1[3].color = v1[0].color;
	v2[0].color = (DWORD)(128*t2);
	v2[0].color <<= 24;
	v2[0].color |= 0x00FF3333;
	v2[1].color = v2[2].color = v2[3].color = v2[0].color;

	//テクスチャUV設定
	v1[0].tu = v2[0].tu = 0.0f;
	v1[0].tv = v2[0].tv = 0.0f;
	v1[1].tu = v2[1].tu = 1.0f;
	v1[1].tv = v2[1].tv = 0.0f;
	v1[2].tu = v2[2].tu = 0.0f;
	v1[2].tv = v2[2].tv = 1.0f;
	v1[3].tu = v2[3].tu = 1.0f;
	v1[3].tv = v2[3].tv = 1.0f;

	//z 0.0f
	for(i=0;i<4;i++){
		v1[i].z = v2[i].z = 0.0f;
	}
	
	g_draw.d3ddev->SetTexture(0,GetTexture(SYSFXTEX_KO_K));
	//'K'(半透明)
	v = v1;
	for(i=0;i<2;i++)
	{
		switch(i){
			case 0:t=m_t;break;
			case 2:t=m_t*m_t;break;
		}

		x = (0.5f + 0.15f*t)*ASPECTRATIO;
		y = 1.0f;
		w = (400.0f - 180.0f*t)/240.0f;
		h = (300.0f - 90.0f*t)/240.0f;

		v[0].x = x - w*0.5f;
		v[0].y = y - h*0.5f;
		v[1].x = x + w*0.5f;
		v[1].y = y - h*0.5f;
		v[2].x = x - w*0.5f;
		v[2].y = y + h*0.5f;
		v[3].x = x + w*0.5f;
		v[3].y = y + h*0.5f;
		
		//描画
		g_draw.d3ddev->DrawPrimitiveUP(
					D3DPT_TRIANGLESTRIP,
					2,
					v,
					sizeof(MYVERTEX3D));
	}
	//'K'(加算)
	g_draw.SetAlphaMode(GBLEND_KASAN);
	v = v2;
	for(i=0;i<2;i++)
	{
		switch(i){
			case 0:t = (m_t+m_t*m_t)*0.5f;break;
			case 1:t = m_t*m_t*m_t;break;
		}

		x = (0.5f + 0.15f*t)*ASPECTRATIO;
		y = 1.0f;
		w = (400.0f - 180.0f*t)/240.0f;
		h = (300.0f - 90.0f*t)/240.0f;

		v[0].x = x - w*0.5f;
		v[0].y = y - h*0.5f;
		v[1].x = x + w*0.5f;
		v[1].y = y - h*0.5f;
		v[2].x = x - w*0.5f;
		v[2].y = y + h*0.5f;
		v[3].x = x + w*0.5f;
		v[3].y = y + h*0.5f;
		
		//描画
		g_draw.d3ddev->DrawPrimitiveUP(
					D3DPT_TRIANGLESTRIP,
					2,
					v,
					sizeof(MYVERTEX3D));
	}

	
	g_draw.d3ddev->SetTexture(0,GetTexture(SYSFXTEX_KO_O));
	//'O'(半透明)
	g_draw.SetAlphaMode(0);
	v = v1;
	for(i=0;i<2;i++)
	{
		switch(i){
			case 0:t=m_t;break;
			case 2:t=m_t*m_t;break;
		}

		x = (1.6f - 0.15f*t)*ASPECTRATIO;
		y = 1.0f;
		w = (400.0f - 180.0f*t)/240.0f;
		h = (300.0f - 90.0f*t)/240.0f;

		v[0].x = x - w*0.5f;
		v[0].y = y - h*0.5f;
		v[1].x = x + w*0.5f;
		v[1].y = y - h*0.5f;
		v[2].x = x - w*0.5f;
		v[2].y = y + h*0.5f;
		v[3].x = x + w*0.5f;
		v[3].y = y + h*0.5f;
		
		//描画
		g_draw.d3ddev->DrawPrimitiveUP(
					D3DPT_TRIANGLESTRIP,
					2,
					v,
					sizeof(MYVERTEX3D));
	}
	//'O'(加算)
	g_draw.SetAlphaMode(GBLEND_KASAN);
	v = v2;
	for(i=0;i<2;i++)
	{
		switch(i){
			case 0:t = (m_t+m_t*m_t)*0.5f;break;
			case 1:t = m_t*m_t*m_t;break;
		}

		x = (1.6f - 0.15f*t)*ASPECTRATIO;
		y = 1.0f;
		w = (400.0f - 180.0f*t)/240.0f;
		h = (300.0f - 90.0f*t)/240.0f;

		v[0].x = x - w*0.5f;
		v[0].y = y - h*0.5f;
		v[1].x = x + w*0.5f;
		v[1].y = y - h*0.5f;
		v[2].x = x - w*0.5f;
		v[2].y = y + h*0.5f;
		v[3].x = x + w*0.5f;
		v[3].y = y + h*0.5f;
		
		//描画
		g_draw.d3ddev->DrawPrimitiveUP(
					D3DPT_TRIANGLESTRIP,
					2,
					v,
					sizeof(MYVERTEX3D));
	}

	g_draw.SetAlphaMode(0);
	g_draw.SetTransform(TRUE);
	return FALSE;
}



/*****************************************************************
	FIGHT
******************************************************************/
/*
UINT vcount = 0;
UINT lcount = 0;
UINT tcount = 0;
UINT t2count = 0;

CFxFight::Vert::Vert(){ vcount++; }
CFxFight::Vert::~Vert(){ vcount--; }

CFxFight::Line::Line(){ lcount++; }
CFxFight::Line::~Line(){ lcount--; }

CFxFight::Tri::Tri(){ tcount++; }
CFxFight::Tri::~Tri(){ tcount--; }

CFxFight::Tri2::Tri2(){ t2count++; }
CFxFight::Tri2::~Tri2(){ t2count--; }
*/

/*----------------------------------------------------------------
	生成時処理
------------------------------------------------------------------*/
BOOL CFxFight::Create(int prm1,int prm2,int prm3)
{
	pdat->objtype |= GOBJFLG_DISPZAHYO;

	VertList vlist;
	LineList llist;
	TriList  tlist;

	//リスト生成
	Create_F(vlist,llist,tlist,-20.0f);
	Create_I(vlist,llist,tlist,-10.0f);
	Create_G(vlist,llist,tlist,  0.0f);
	Create_H(vlist,llist,tlist, 10.0f);
	Create_T(vlist,llist,tlist, 20.0f);

	//ばらばら事件
	static UINT ndiv = 32;
	for(UINT i=0;i<ndiv;i++){
		Divide(vlist,llist,tlist);
	}

	//描画用リストに変換
	CreateTri2(vlist,llist,tlist,list);

	//リスト破棄
	VertList::iterator iv = vlist.begin();
	VertList::iterator ive= vlist.end();
	for(;iv!=ive;iv++){
		delete (*iv);
	}
	LineList::iterator il = llist.begin();
	LineList::iterator ile= llist.end();
	for(;il!=ile;il++){
		delete (*il);
	}
	TriList::iterator it = tlist.begin();
	TriList::iterator ite= tlist.end();
	for(;it!=ite;it++){
		delete (*it);
	}

	//gbl.ods("★FxFigft v:%d , l:%d , t:%d",vcount,lcount,tcount);

	return TRUE;
}


//頂点設定マクロ
#define DEFVERT(a,b,c)	{					\
	v[a]->x = b;							\
	v[a]->y = c;							\
}

//トライアングル設定マクロ
#define DEFTRI(a,b,c)	{					\
	t=new Tri; tlist.push_back(t);			\
	l1 = new Line;	llist.push_back(l1);	\
	l2 = new Line;	llist.push_back(l2);	\
	l3 = new Line;	llist.push_back(l3);	\
	t->l[0] = l1;							\
	t->l[1] = l2;							\
	t->l[2] = l3;							\
	l1->v[0]=v[a];							\
	l1->v[1]=v[b];							\
	l2->v[0]=v[b];							\
	l2->v[1]=v[c];							\
	l3->v[0]=v[c];							\
	l3->v[1]=v[a]; 							\
}

/*----------------------------------------------------------------
	文字「F」生成
------------------------------------------------------------------*/
void CFxFight::Create_F(VertList& vlist,LineList& llist,TriList& tlist,float offset)
{
	const UINT vnum =10;
	Vert **v = new Vert* [vnum];
	for(UINT i=0;i<vnum;i++){
		v[i] = new Vert;
		vlist.push_back(v[i]);
	}

	const float left = 0.0f;
	const float top = 0.0f;
	const float right = 9.0f;
	const float bottom = 9.0f;
	DEFVERT( 0,		left,		top		);
	DEFVERT( 1,		right,		v[0]->y	);
	DEFVERT( 2,		v[1]->x,	2.0f	);
	DEFVERT( 3,		3.0f,		v[2]->y	);
	DEFVERT( 4,		v[3]->x,	3.0f	);
	DEFVERT( 5,		right,		v[4]->y	);
	DEFVERT( 6,		right,		6.0f	);	
	DEFVERT( 7,		v[3]->x,	v[6]->y	);
	DEFVERT( 8,		v[3]->x,	bottom	);
	DEFVERT( 9,		v[0]->x,	bottom	);

	Tri *t;
	Line *l1,*l2,*l3;

	for(UINT i=0;i<vnum;i++){
		v[i]->x += offset;
		v[i]->y -= 4.5f;
	}

	DEFTRI(0,1,3);
	DEFTRI(1,2,3);
	DEFTRI(0,3,9);
	DEFTRI(3,9,8);
	DEFTRI(4,5,6);
	DEFTRI(4,7,6);

	delete [] v;
}


/*----------------------------------------------------------------
	文字「I」生成
------------------------------------------------------------------*/
void CFxFight::Create_I(VertList& vlist,LineList& llist,TriList& tlist,float offset)
{
	Vert *v[4];
	for(UINT i=0;i<4;i++){
		v[i] = new Vert;
		vlist.push_back(v[i]);
	}

	v[0]->x = 3.0f;
	v[0]->y = 0.0f;
	
	v[1]->x = 6.0f;
	v[1]->y = 0.0f;
	
	v[2]->x = 3.0f;
	v[2]->y = 9.0f;
	
	v[3]->x = 6.0f;
	v[3]->y = 9.0f;

	Tri *t;
	Line *l1,*l2,*l3;

	for(UINT i=0;i<4;i++){
		v[i]->x += offset;
		v[i]->y -= 4.5f;
	}

	//▲
	t  = new Tri;	tlist.push_back(t);
	l1 = new Line;	llist.push_back(l1);
	l2 = new Line;	llist.push_back(l2);
	l3 = new Line;	llist.push_back(l3);
	t->l[0] = l1;
	t->l[1] = l2;
	t->l[2] = l3;

	l1->v[0] = v[0];
	l1->v[1] = v[1];

	l2->v[0] = v[1];
	l2->v[1] = v[2];

	l3->v[0] = v[2];
	l3->v[1] = v[0];

	//▲
	t  = new Tri;	tlist.push_back(t);
	l1 = new Line;	llist.push_back(l1);
	l2 = new Line;	llist.push_back(l2);
	l3 = new Line;	llist.push_back(l3);
	t->l[0] = l1;
	t->l[1] = l2;
	t->l[2] = l3;

	l1->v[0] = v[1];
	l1->v[1] = v[2];

	l2->v[0] = v[2];
	l2->v[1] = v[3];

	l3->v[0] = v[3];
	l3->v[1] = v[1];

	//delete [] v;
}


/*----------------------------------------------------------------
	文字「G」生成
------------------------------------------------------------------*/
void CFxFight::Create_G(VertList& vlist,LineList& llist,TriList& tlist,float offset)
{
	const UINT vnum =14;
	Vert **v = new Vert* [vnum];
	for(UINT i=0;i<vnum;i++){
		v[i] = new Vert;
		vlist.push_back(v[i]);
	}

	const float left = 0.0f;
	const float top = 0.0f;
	const float right = 9.0f;
	const float bottom = 9.0f;
	DEFVERT( 0,	9,	0);
	DEFVERT( 1,	9,	2);
	DEFVERT( 2,	2,	2);
	DEFVERT( 3,	0,	0);
	DEFVERT( 4,	0,	9);
	DEFVERT( 5,	2,	7);
	DEFVERT( 6,	9,	9);	
	DEFVERT( 7,	7,	7);
	DEFVERT( 8,	9,	6);
	DEFVERT( 9,	7,	6);
	DEFVERT(10,	9,	4);
	DEFVERT(11,	7,	4);
	DEFVERT(12,	5,	6);
	DEFVERT(13,	5,	4);

	Tri *t;
	Line *l1,*l2,*l3;

	for(UINT i=0;i<vnum;i++){
		v[i]->x += offset;
		v[i]->y -= 4.5f;
	}

	DEFTRI(0,1,2);
	DEFTRI(0,2,3);
	DEFTRI(2,3,4);
	DEFTRI(2,4,5);
	DEFTRI(4,5,6);
	DEFTRI(5,6,7);
	DEFTRI(6,7,8);
	DEFTRI(7,8,9);
	DEFTRI(8,9,10);
	DEFTRI(9,10,11);
	DEFTRI(10,11,12);
	DEFTRI(11,12,13);

	delete [] v;
}


/*----------------------------------------------------------------
	文字「H」生成
------------------------------------------------------------------*/
void CFxFight::Create_H(VertList& vlist,LineList& llist,TriList& tlist,float offset)
{
	const UINT vnum =14;
	Vert **v = new Vert* [vnum];
	for(UINT i=0;i<vnum;i++){
		v[i] = new Vert;
		vlist.push_back(v[i]);
	}

	const float left = 0.0f;
	const float top = 0.0f;
	const float right = 9.0f;
	const float bottom = 9.0f;
	DEFVERT( 0,	0,	0);
	DEFVERT( 1,	3,	0);
	DEFVERT( 2,	0,	9);
	DEFVERT( 3,	3,	9);
	DEFVERT( 4,	3,	3.5);
	DEFVERT( 5,	3,	4.5);
	DEFVERT( 6,	6,	3.5);	
	DEFVERT( 7,	6,	4.5);
	DEFVERT( 8,	6,	0);
	DEFVERT( 9,	9,	0);
	DEFVERT(10,	6,	9);
	DEFVERT(11,	9,	9);
	DEFVERT(12,	0,	4);
	DEFVERT(13,	9,	4);

	Tri *t;
	Line *l1,*l2,*l3;

	for(UINT i=0;i<vnum;i++){
		v[i]->x += offset;
		v[i]->y -= 4.5f;
	}

	DEFTRI(0,1,12);
	DEFTRI(1,12,3);
	DEFTRI(12,3,2);
	DEFTRI(6,4,5);
	DEFTRI(7,5,6);
	DEFTRI(8,9,13);
	DEFTRI(8,10,13);
	DEFTRI(10,11,13);

	delete [] v;
}


/*----------------------------------------------------------------
	文字「T」生成
------------------------------------------------------------------*/
void CFxFight::Create_T(VertList& vlist,LineList& llist,TriList& tlist,float offset)
{
	const UINT vnum =9;
	Vert **v = new Vert* [vnum];
	for(UINT i=0;i<vnum;i++){
		v[i] = new Vert;
		vlist.push_back(v[i]);
	}

	const float left = 0.0f;
	const float top = 0.0f;
	const float right = 9.0f;
	const float bottom = 9.0f;
	DEFVERT( 0,	0,	0);
	DEFVERT( 1,	0,	3);
	DEFVERT( 2,	4.5,0);
	DEFVERT( 3,	9,	0);
	DEFVERT( 4,	9,	3);
	DEFVERT( 5,	3,	3);
	DEFVERT( 6,	6,	3);	
	DEFVERT( 7,	3,	9);
	DEFVERT( 8,	6,	9);

	Tri *t;
	Line *l1,*l2,*l3;

	for(UINT i=0;i<vnum;i++){
		v[i]->x += offset;
		v[i]->y -= 4.5f;
	}

	DEFTRI(0,1,2);
	DEFTRI(1,2,4);
	DEFTRI(2,3,4);
	DEFTRI(6,7,5);
	DEFTRI(7,8,6);

	delete [] v;
}

/*----------------------------------------------------------------
	頂点分割処理
------------------------------------------------------------------*/
void CFxFight::Divide(VertList& vlist,LineList& llist,TriList& tlist)
{
	UINT i;
	float ratio;
	float inv_ratio;

	//適当なライン(1)を選ぶ
	Line *l1 = llist[ rand()%llist.size() ];

	//ラインに結び付けられた△を見つける
	Tri *t_old = NULL;
	UINT l1_index;
	TriList::iterator it = tlist.begin();
	TriList::iterator ite= tlist.end();
	for(;it!=ite;it++){
		for(i=0;i<3;i++){
			if((*it)->l[i]==l1){
				t_old = (*it);
				l1_index = i;
				break;
			}
		}
	}
	if(!t_old){
		gbl.ods("CFxFight::Divide t_old not found");
		return;
	}

	//選択された△に結び付けられたもう一つのライン(2)を探す
	UINT l2_index = (l1_index + rand()%2 + 1)%3;
	Line *l2 = t_old->l[l2_index];

	//分割をおこなわないライン(0)を選択
	UINT l0_index = 3;
	switch(l1_index){
		case 0:
			switch(l2_index){
				case 1:l0_index = 2;break;
				case 2:l0_index = 1;break;
			}
			break;
		case 1:
			switch(l2_index){
				case 0:l0_index = 2;break;
				case 2:l0_index = 0;break;
			}
			break;
		case 2:
			switch(l2_index){
				case 0:l0_index = 1;break;
				case 1:l0_index = 0;break;
			}
			break;
	}
	if(l0_index==3){
		gbl.ods("CFxFight::Divide line0 not found");
		return;
	}
	Line *l0 = t_old->l[l0_index];

	//2ラインが共通に持つ頂点(v0)
	Vert *v0;
	if(l1->v[0]==l2->v[0])v0=l2->v[0];
	else if(l1->v[0]==l2->v[1])v0=l2->v[1];
	else if(l1->v[1]==l2->v[0])v0=l2->v[0];
	else if(l1->v[1]==l2->v[1])v0=l2->v[1];
	else{
		gbl.ods2("CFxFight::Divide v0 not found\n");
		return;
	}

	//2ラインで重複していない頂点(v31,v32)
	Vert *v31;
	if(l1->v[0]==v0)		v31 = l1->v[1];
	else if(l1->v[1]==v0)	v31 = l1->v[0];
	else{
		gbl.ods2("CFxFight::Divide v31 not found\n");
		return;
	}
	Vert *v32;
	if(l2->v[0]==v0)		v32 = l2->v[1];
	else if(l2->v[1]==v0)	v32 = l2->v[0];
	else{
		gbl.ods2("CFxFight::Divide v32 not found\n");
		return;
	}

	//ライン(1)を分割する（v1生成　ライン(1)：v0-v1 , 生成ライン(3)：v1-v31）
	ratio = ((rand()%99999)/100000.0f)*0.8f + 0.1f;
	inv_ratio = 1.0f-ratio;
	Vert *v1 = new Vert;
	v1->x = l1->v[0]->x*ratio + l1->v[1]->x*inv_ratio;
	v1->y = l1->v[0]->y*ratio + l1->v[1]->y*inv_ratio;
	if(l1->v[0]==v31)	l1->v[0] = v1;
	else				l1->v[1] = v1;
	Line *l3 = new Line;
	l3->v[0] = v1;
	l3->v[1] = v31;

	//ライン(2)を分割する（v2生成　ライン(2)：v0-v2 , 生成ライン(4)：v2-v32）
	ratio = ((rand()%99999)/100000.0f)*0.8f + 0.1f;
	inv_ratio = 1.0f-ratio;
	Vert *v2 = new Vert;
	v2->x = l2->v[0]->x*ratio + l2->v[1]->x*inv_ratio;
	v2->y = l2->v[0]->y*ratio + l2->v[1]->y*inv_ratio;
	if(l2->v[0]==v32)	l2->v[0] = v2;
	else				l2->v[1] = v2;
	Line *l4 = new Line;
	l4->v[0] = v2;
	l4->v[1] = v32;

	//ライン(5)生成　（v1-v2）
	Line *l51 = new Line;
	l51->v[0] = v1;
	l51->v[1] = v2;
	Line *l52 = new Line;
	l52->v[0] = l51->v[0];
	l52->v[1] = l51->v[1];

	//ライン(6)生成　（v1-v32 or v2-v31）
	Line *l61 = new Line;
	BOOL left = rand()%2==0 ? TRUE : FALSE;
	if(left){
		l61->v[0] = v1;
		l61->v[1] = v32;
	}
	else{
		l61->v[0] = v2;
		l61->v[1] = v31;
	}
	Line *l62 = new Line;
	l62->v[0] = l61->v[0];
	l62->v[1] = l61->v[1];

	//△old変更（ライン 1,2,0 → 1,2,5）
	if(t_old->l[0]==l0)		t_old->l[0] = l51;
	else if(t_old->l[1]==l0)t_old->l[1] = l51;
	else if(t_old->l[2]==l0)t_old->l[2] = l51;
	else{
		gbl.ods2("CFxFight::Divide line0 not found(2)\n");
		return;
	}

	//△生成（ライン 5,6, 4or3 ）
	Tri *t_new1 = new Tri;
	t_new1->l[0] = left ? l4 : l3;
	t_new1->l[1] = l52;
	t_new1->l[2] = l61;

	//△生成（ライン 6,0, 3or4）
	Tri *t_new2 = new Tri;
	t_new2->l[0] = left ? l3 : l4;
	t_new2->l[1] = l62;
	t_new2->l[2] = l0;

	//新規に生成されたモノをリストに突っ込む
	vlist.push_back(v1);
	vlist.push_back(v2);
	llist.push_back(l3);
	llist.push_back(l4);
	llist.push_back(l51);
	llist.push_back(l52);
	llist.push_back(l61);
	llist.push_back(l62);
	tlist.push_back(t_new1);
	tlist.push_back(t_new2);
}

/*----------------------------------------------------------------
	テンポラリ△リストから、本番用の△リストを作る
------------------------------------------------------------------*/
void CFxFight::CreateTri2(VertList& vlist,LineList& llist,TriList& tlist,Tri2List& list_out)
{
	Tri  *t;
	Vert *v1,*v2,*v3;
	Tri2 *t2;
	D3DXVECTOR3 ax,tr;

	TriList::iterator it = tlist.begin();
	TriList::iterator ite= tlist.end();
	for(;it!=ite;it++)
	{
		t = (*it);
		v1 = t->l[0]->v[0];
		v2 = t->l[0]->v[1];
		if(t->l[0]->v[0]==t->l[1]->v[0]){
			v3 = t->l[1]->v[1];
		}
		else{
			if(t->l[0]->v[1]==t->l[1]->v[0]){
				v3 = t->l[1]->v[1];
			}
			else{
				v3 = t->l[1]->v[0];
			}
		}

		t2 = new Tri2;
		list_out.push_back(t2);

		t2->ctr.x = ( v1->x + v2->x + v3->x ) / 3.0f;
		t2->ctr.y = ( v1->y + v2->y + v3->y ) / 3.0f;
		t2->ctr.z = 0.0f;

		t2->pos[0].x = v1->x - t2->ctr.x;
		t2->pos[0].y = v1->y - t2->ctr.y;
		t2->pos[0].z = 0.0f;

		t2->pos[1].x = v2->x - t2->ctr.x;
		t2->pos[1].y = v2->y - t2->ctr.y;
		t2->pos[1].z = 0.0f;
		
		t2->pos[2].x = v3->x - t2->ctr.x;
		t2->pos[2].y = v3->y - t2->ctr.y;
		t2->pos[2].z = 0.0f;

		ax.x = (gbl.RandomOne()+0.01f) - 0.5f;
		ax.y = (gbl.RandomOne()+0.01f) - 0.5f;
		ax.z = (gbl.RandomOne()+0.01f) - 0.5f;

		D3DXMatrixRotationAxis( &t2->matr , &ax , gbl.RandomOne()*0.25f + 0.1f );
		D3DXMatrixIdentity( &t2->mat );

		t2->vel.x = t2->ctr.x * (gbl.RandomOne()+0.5f) * 0.030f;
		t2->vel.y = t2->ctr.y * (gbl.RandomOne()+0.5f) * 0.080f;
		t2->vel.z = (gbl.RandomOne()-0.5f)*0.5f;
	}
}

/*----------------------------------------------------------------
	更新
------------------------------------------------------------------*/
void CFxFight::Update()
{
	const float dur1 = 5;
	const float dur3 = 20;
	const float dur2 = 25;
	float t;
	m_sca = 0.05f;

	if(pdat->counter > dur1+dur2+dur3){
		End();
		return;
	}
	if(pdat->counter < dur1){
		//色更新
		t = pdat->counter / dur1;
		m_alpha = (DWORD)(255*t);
		m_red = 0xFF;
		m_green = 0x88;
		m_blue = 0x22;
		m_sca = 0.05f + (1.0f-t)*0.035f;
		return;
	}
	if(pdat->counter < dur1+dur3){
		//色更新
		t = (pdat->counter-dur1)/dur3 ;
		m_alpha = 255;
		m_red = 0xFF;
		m_green = (DWORD)(0x88*(1.0f-t));
		m_blue = (DWORD)(0x22*(1.0f-t));
		return;
	}

	//色更新
	t = (pdat->counter-dur1-dur3)/dur2 ;
	m_alpha = (DWORD)(255*(1.0f-t));
	m_red   = 255;
	m_green = (DWORD)(255*t);
	m_blue  = (DWORD)(255*t);

	//位置･回転更新
	Tri2List::iterator i = list.begin();
	Tri2List::iterator ie= list.end();
	for(;i!=ie;i++)
	{
		(*i)->mat *= (*i)->matr ;
		(*i)->ctr += (*i)->vel;
	}
}


/*----------------------------------------------------------------
	描画
------------------------------------------------------------------*/
BOOL CFxFight::DrawF()
{
	g_draw.SetTransform(FALSE);
	g_draw.d3ddev->SetTexture(0,NULL);				//テクスチャ無し
	g_draw.d3ddev->SetVertexShader( FVF_3DVERTEX );	//頂点のフォーマットを指定

	DWORD color = 0;
	color |= m_alpha << 24;
	color |= m_red	 << 16;
	color |= m_green << 8;
	color |= m_blue;

	D3DXMATRIX mat,matt;

	D3DXMATRIX matb , matbs , matbs2 , matbt;
	D3DXMatrixScaling(&matbs,m_sca,m_sca,m_sca);
	D3DXMatrixTranslation(&matbt,1.15f,0.90f,0);
	matb = matbs*matbt;

	Tri2List::iterator i = list.begin();
	Tri2List::iterator ie= list.end();
	for(;i!=ie;i++)
	{
		Tri2 *t = (*i);

		//色を設定
		t->pos[0].color = color;
		t->pos[1].color = color;
		t->pos[2].color = color;

		//マトリクス設定
		D3DXMatrixTranslation(&matt , (*i)->ctr.x , (*i)->ctr.y , (*i)->ctr.z );
		mat = (*i)->mat * matt * matb;
		g_draw.d3ddev->SetTransform(D3DTS_WORLD,&mat);
		
		//描画
		g_draw.d3ddev->DrawPrimitiveUP(
					D3DPT_TRIANGLESTRIP,
					1,
					t->pos,
					sizeof(MYVERTEX3D));
	}

	g_draw.SetTransform(TRUE);

	return TRUE;
}

/*----------------------------------------------------------------
	破棄
------------------------------------------------------------------*/
void CFxFight::Destroy()
{
	//リストを破棄
	Tri2List::iterator i = list.begin();
	Tri2List::iterator ie= list.end();
	for(;i!=ie;i++)
	{
		delete (*i);
	}
	list.clear();

//	gbl.ods("★CFxFight t2:%d",t2count);
}


/*****************************************************************
	TIMEOVER
******************************************************************/

void CFxTimeOver::Update()
{
	if(pdat->counter > 150 + 24 -1)End();
}

BOOL CFxTimeOver::DrawF()
{
	g_draw.SetTransform(FALSE);

	float t2;
	if(pdat->counter < 150){
		t2 = 1.0f;
	}
	else{
		t2 = 1.0f - (pdat->counter-150)/24.0f ;
	}

	char str[] = "TIME OVER";
	const float interval = 3;
	const float duration = 6;
	const float y		 = 200;
	const float left	 = 20;
	const float space    = (640-left*2)/9;
	float t;
	float x = left;
	for(UINT i=0;i<9;i++){
		t = (pdat->counter-interval*i) / duration;
		DrawOneCharacter(x,y,str[i],t,t2);
		x += space;
	}

	g_draw.SetTransform(TRUE);

	return FALSE;
}

void CFxTimeOver::DrawOneCharacter(float x,float y,char c,float t,float t2)
{
	if(t<0.0f)return;
	if(t>1.0f)t=1.0f;

	DWORD col;
	char str[2] = {c,'\0'};
	float tx,ty,scaX,scaY;

	//大体
	const float font_size = 60;

	scaX = t*3.2f;
	scaY = ((0.5f + 0.5f*t)*3.0f )* t2;
	tx = x ;
	ty = y - font_size*t2;

	col = (DWORD)(220*t*t2/4);//alpha
	col <<= 24;
	col |= 0x00FFFFFF;

	g_draw.SetAlphaMode(GBLEND_HANTOUMEI);
//	g_system.DrawBMPTextEx(tx  ,ty  ,0.0f,str,col,scaX,scaY,0);
	g_system.DrawBMPTextEx(tx+t,ty+t,0.0f,str,col,scaX,scaY,0);
	g_system.DrawBMPTextEx(tx+t,ty-t,0.0f,str,col,scaX,scaY,0);
	g_system.DrawBMPTextEx(tx-t,ty+t,0.0f,str,col,scaX,scaY,0);
	g_system.DrawBMPTextEx(tx-t,ty-t,0.0f,str,col,scaX,scaY,0);

	g_draw.EnableZ(FALSE,FALSE);

	col = (DWORD)(128*t*t2);//alpha
	col <<= 24;
	col |= 0x004411FF;

	g_draw.SetAlphaMode(GBLEND_KASAN);
	g_system.DrawBMPTextEx(tx,ty,0.0f,str,col,scaX,scaY,0);

	g_draw.EnableZ(TRUE,TRUE);
	g_draw.SetAlphaMode(0);
}
