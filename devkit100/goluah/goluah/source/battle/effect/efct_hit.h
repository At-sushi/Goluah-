/*===============================================================

	�q�b�g�}�[�N��

=================================================================*/
#pragma once
#include "efctlist.h"


/*!
*	@brief �G�t�F�N�g�`��I�u�W�F�N�g�E�q�b�g�}�[�N(��)
*	@ingroup Battle
*/
class CFxHitmark1 : public CSystemFxBase
{
public:
	void Update();
	BOOL Draw();
};

/*!
*	@brief �G�t�F�N�g�`��I�u�W�F�N�g�E�q�b�g�}�[�N(��)
*	@ingroup Battle
*/
class CFxHitmark2 : public CSystemFxBase
{
public:
	void Update();
	BOOL Draw();
};

/*!
*	@brief �G�t�F�N�g�`��I�u�W�F�N�g�E�q�b�g�}�[�N(��)
*	@ingroup Battle
*/
class CFxHitmark3 : public CSystemFxBase
{
public:
	void Update();
	BOOL Draw();
};

/*!
*	@brief �G�t�F�N�g�`��I�u�W�F�N�g�E�q�b�g�}�[�N(��)
*	@ingroup Battle
*/
class CFxHitmark4 : public CSystemFxBase
{
public:
	void Update();
	BOOL Draw();
};

/*!
*	@brief �G�t�F�N�g�`��I�u�W�F�N�g�E�K�[�h�}�[�N(���_���[�W�Ȃ�)
*	@ingroup Battle
*/
class CFxGuardmark1 : public CSystemFxBase
{
public:
	virtual ~CFxGuardmark1(){}

	void Update();
	virtual BOOL Draw();
};

/*!
*	@brief �G�t�F�N�g�`��I�u�W�F�N�g�E�K�[�h�}�[�N(���_���[�W����)
*	@ingroup Battle
*/
class CFxGuardmark2 : public CFxGuardmark1
{
public:
	BOOL Draw();
};


/*!
*	@brief �G�t�F�N�g�`��I�u�W�F�N�g�E�͂݃}�[�N
*	@ingroup Battle
*/
class CFxCatchmark : public CSystemFxBase
{
public:
	void Update();
};

/*!
*	@brief �G�t�F�N�g�`��I�u�W�F�N�g�E�n�ʏՌ��g
*	@ingroup Battle
*/
class CFxGndCircle : public CSystemFxBase
{
public:
	void Update();
};

