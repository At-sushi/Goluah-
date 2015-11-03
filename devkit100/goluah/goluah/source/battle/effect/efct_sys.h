
/*===============================================================

	システムエフェクト

=================================================================*/
#pragma once

#include "efctlist.h"

/*!
*	@brief 描画オブジェクト・トップレベル描画物ベース
*	@ingroup Battle
*/
class CFxFrontDispBase : public CSystemFxBase
{
public:
	virtual ~CFxFrontDispBase(){}

	virtual BOOL Create(int prm1,int prm2,int prm3)
						{pdat->objtype |= GOBJFLG_DISPZAHYO;return TRUE;}
	virtual BOOL Draw(){return TRUE;}
	virtual BOOL DrawF(){return TRUE;}
};

/*!
*	@brief 描画オブジェクト・ラウンド表示ベース
*	@ingroup Battle
*/
class CFxRoundBase : public CFxFrontDispBase
{
public:
	virtual ~CFxRoundBase(){}

	void Update();
	BOOL DrawF();

protected:
	char round_number;
	//t : 0～1
	void DrawOneCharacter(float x,float y,char c,float t,float t2);
};

/*!
*	@brief 描画オブジェクト・ROUND 1
*	@ingroup Battle
*/
class CFxRound1 : public CFxRoundBase
{
public:
	CFxRound1(){round_number='1';}
};

/*!
*	@brief 描画オブジェクト・ROUND 2
*	@ingroup Battle
*/
class  CFxRound2 : public CFxRoundBase
{
public:
	CFxRound2(){round_number='2';}
};

/*!
*	@brief 描画オブジェクト・ROUND 3
*	@ingroup Battle
*/
class  CFxRound3 : public CFxRoundBase
{
public:
	CFxRound3(){round_number='3';}
};

/*!
*	@brief 描画オブジェクト・ROUND 4
*	@ingroup Battle
*/
class  CFxRound4 : public CFxRoundBase
{
public:
	CFxRound4(){round_number='4';}
};

/*!
*	@brief 描画オブジェクト・ROUND 5
*	@ingroup Battle
*/
class  CFxRound5 : public CFxRoundBase
{
public:
	CFxRound5(){round_number='5';}
};

/*!
*	@brief 描画オブジェクト・ROUND 6
*	@ingroup Battle
*/
class  CFxRound6 : public CFxRoundBase
{
public:
	CFxRound6(){round_number='6';}
};

/*!
*	@brief 描画オブジェクト・K.O.
*	@ingroup Battle
*/
class  CFxKO : public CFxFrontDispBase
{
public:
	void Update();
	BOOL DrawF();

protected:
	float m_t;
	void DrawOne(float cx,float cy,float t,
					LPDIRECT3DTEXTURE9 tex);
};

/*!
*	@brief 描画オブジェクト・FIGHT
*	@ingroup Battle
*/
class  CFxFight : public CFxFrontDispBase
{
public:
	virtual ~CFxFight(){Destroy();}

	BOOL Create(int prm1,int prm2,int prm3);
	void Update();
	BOOL DrawF();
	void Destroy();

protected:
	DWORD m_alpha;
	DWORD m_red;
	DWORD m_green;
	DWORD m_blue;
	float m_sca;

	/*------------------------------------------------------------
		頂点の生成と分割
	--------------------------------------------------------------*/
	struct Vert{float x,y;/*Vert();~Vert();*/};		//!< テンポラリ点
	struct Line{Vert *v[2];/*Line();~Line();*/};	//!< テンポラリ線	
	struct Tri{Line *l[3];/*Tri();~Tri();*/};		//!< テンポラリ△
	typedef std::vector<Vert*> VertList;
	typedef std::vector<Line*> LineList;
	typedef std::vector<Tri*>  TriList;

	//!描画用△
	struct Tri2					//△　その2
	{
		MYVERTEX3D		pos[3];	//!< 各頂点の重心からの相対距離
		D3DXVECTOR3		ctr;	//!< 重心
		D3DXVECTOR3		vel;	//!< 重心の移動速度
		D3DXMATRIX		matr;	//!< 1フレ分の回転をもつマトリクス
		D3DXMATRIX		mat;	//!< 現在の回転マトリクス

		//Tri2();~Tri2();
	};
	typedef std::vector<Tri2*> Tri2List;
	Tri2List list;

	static void Create_F(VertList& vlist,LineList& llist,TriList& tlist,float offset);
	static void Create_I(VertList& vlist,LineList& llist,TriList& tlist,float offset);
	static void Create_G(VertList& vlist,LineList& llist,TriList& tlist,float offset);
	static void Create_H(VertList& vlist,LineList& llist,TriList& tlist,float offset);
	static void Create_T(VertList& vlist,LineList& llist,TriList& tlist,float offset);
	static void Divide(VertList& vlist,LineList& llist,TriList& tlist);							//!< ばらばら事件
	static void CreateTri2(VertList& vlist,LineList& llist,TriList& tlist,Tri2List& list_out);	//!< テンポラリの線･点・△から、本番用△生成
};

/*!
*	@brief 描画オブジェクト・TIMEOVER
*	@ingroup Battle
*/
class CFxTimeOver : public CFxFrontDispBase
{
public:
	virtual ~CFxTimeOver(){}

	void Update();
	BOOL DrawF();

protected:
	void DrawOneCharacter(float x,float y,char c,float t,float t2);
};
