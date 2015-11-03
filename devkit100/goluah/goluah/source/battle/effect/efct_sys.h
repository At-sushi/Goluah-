
/*===============================================================

	�V�X�e���G�t�F�N�g

=================================================================*/
#pragma once

#include "efctlist.h"

/*!
*	@brief �`��I�u�W�F�N�g�E�g�b�v���x���`�敨�x�[�X
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
*	@brief �`��I�u�W�F�N�g�E���E���h�\���x�[�X
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
	//t : 0�`1
	void DrawOneCharacter(float x,float y,char c,float t,float t2);
};

/*!
*	@brief �`��I�u�W�F�N�g�EROUND 1
*	@ingroup Battle
*/
class CFxRound1 : public CFxRoundBase
{
public:
	CFxRound1(){round_number='1';}
};

/*!
*	@brief �`��I�u�W�F�N�g�EROUND 2
*	@ingroup Battle
*/
class  CFxRound2 : public CFxRoundBase
{
public:
	CFxRound2(){round_number='2';}
};

/*!
*	@brief �`��I�u�W�F�N�g�EROUND 3
*	@ingroup Battle
*/
class  CFxRound3 : public CFxRoundBase
{
public:
	CFxRound3(){round_number='3';}
};

/*!
*	@brief �`��I�u�W�F�N�g�EROUND 4
*	@ingroup Battle
*/
class  CFxRound4 : public CFxRoundBase
{
public:
	CFxRound4(){round_number='4';}
};

/*!
*	@brief �`��I�u�W�F�N�g�EROUND 5
*	@ingroup Battle
*/
class  CFxRound5 : public CFxRoundBase
{
public:
	CFxRound5(){round_number='5';}
};

/*!
*	@brief �`��I�u�W�F�N�g�EROUND 6
*	@ingroup Battle
*/
class  CFxRound6 : public CFxRoundBase
{
public:
	CFxRound6(){round_number='6';}
};

/*!
*	@brief �`��I�u�W�F�N�g�EK.O.
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
*	@brief �`��I�u�W�F�N�g�EFIGHT
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
		���_�̐����ƕ���
	--------------------------------------------------------------*/
	struct Vert{float x,y;/*Vert();~Vert();*/};		//!< �e���|�����_
	struct Line{Vert *v[2];/*Line();~Line();*/};	//!< �e���|������	
	struct Tri{Line *l[3];/*Tri();~Tri();*/};		//!< �e���|������
	typedef std::vector<Vert*> VertList;
	typedef std::vector<Line*> LineList;
	typedef std::vector<Tri*>  TriList;

	//!�`��p��
	struct Tri2					//���@����2
	{
		MYVERTEX3D		pos[3];	//!< �e���_�̏d�S����̑��΋���
		D3DXVECTOR3		ctr;	//!< �d�S
		D3DXVECTOR3		vel;	//!< �d�S�̈ړ����x
		D3DXMATRIX		matr;	//!< 1�t�����̉�]�����}�g���N�X
		D3DXMATRIX		mat;	//!< ���݂̉�]�}�g���N�X

		//Tri2();~Tri2();
	};
	typedef std::vector<Tri2*> Tri2List;
	Tri2List list;

	static void Create_F(VertList& vlist,LineList& llist,TriList& tlist,float offset);
	static void Create_I(VertList& vlist,LineList& llist,TriList& tlist,float offset);
	static void Create_G(VertList& vlist,LineList& llist,TriList& tlist,float offset);
	static void Create_H(VertList& vlist,LineList& llist,TriList& tlist,float offset);
	static void Create_T(VertList& vlist,LineList& llist,TriList& tlist,float offset);
	static void Divide(VertList& vlist,LineList& llist,TriList& tlist);							//!< �΂�΂玖��
	static void CreateTri2(VertList& vlist,LineList& llist,TriList& tlist,Tri2List& list_out);	//!< �e���|�����̐���_�E������A�{�ԗp������
};

/*!
*	@brief �`��I�u�W�F�N�g�ETIMEOVER
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
