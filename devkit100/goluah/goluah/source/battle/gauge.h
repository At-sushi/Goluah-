/*
2011/10/29	gauge.cpp�̕ύX�ɔ����ύX
*/
/*===================================================================

	�̗̓Q�[�W
	�L�����N�^��E�����J�E���g�EFPS
	�p���[�Q�[�W

	������A�퓬���̃V�X�e�����`��֌W

=====================================================================*/
#pragma once

#include "define_gcd.h"
class CBattleTaskBase;

/*!
*	@brief �������̉�ʏ�̕\���n�S��
*	@ingroup Battle
*	
*	�̗̓Q�[�W�A�p���[�Q�[�W + ��ʏ�̕\���n�S�ʁB�i��A�����J�E���g�A�e�o�r�j���s���B
*/
class CGauge
{
public:
	CGauge();
	~CGauge(){CleanUp();}

	void CleanUp();
	void Initialize();
	void Initialize(DWORD num);
	void ActivateGauge(DWORD t,DWORD n);

	//�퓬�I�u�W�F�N�g�Ƃ��Ă̓���
	static DWORD GaugeObjectMessage(DWORD msg,LPVOID pdat,DWORD prm);
	void Action();
	void Draw();

protected:
	void DrawHPGauge1();		//!< �����፬���̏ꍇ
	void DrawHPGauge2();		//!< ����ȊO
	void DrawPowerGauge1();		//!< �����፬���̏ꍇ
	void DrawPowerGauge2();		//!< ����ȊO
	void DrawPowerGaugeType1(BOOL player,int pindex,int x,int y);			//!< �p���[�Q�[�W�`��(�X�g�b�N)
	void DrawPowerGaugeType2(BOOL player,int pindex,int x,int y,BOOL pow);	//!< �p���[�Q�[�W�`��(0-100%)
	void DrawHPwaku1();	//�@HP�gcoop
	void DrawHPwaku2();	//	HP�gmarvel kof
	void DrawHPG1();	//	DrawHPGauge1()�̑�֕i
	void DrawHPG2();	//	DrawHPGauge2()�̑�֕i

//*�ϐ�*
protected:
	CBattleTaskBase* battleTask;

	DWORD oid;					//!< �Q�[�W�́A�퓬�I�u�W�F�N�g�Ƃ��ẴI�u�W�F�N�gID
	DWORD num_teammember;		//!< 1�`�[�����l��
	DWORD activegauge[2];		//!< �ǂ̃v���C���[�����ݑΐ킵�Ă���̂��H0,1,2
	DWORD flip_counter;
	MYSURFACE *dds_face[2][MAXNUM_TEAM];	//!< ����������
	BYTE dll_id;				//!< DLLID

	//�̗̓Q�[�W
	int gauge_prv[2][MAXNUM_TEAM];		//!< �H������������킶�팸���Ă����̂Ɏg�p
	int gauge_prv2[2][MAXNUM_TEAM];		//!< �}�Ɍ��炷�p

	//�p���[�Q�[�W
	double pgauge_prv[2][MAXNUM_TEAM];	//!< �������O�ɕ\�����Ă��Q�[�W��
	DWORD numprv[2][MAXNUM_TEAM];			//!< �������O�̃Q�[�W�X�g�b�N���Bcharge���Ɏg�p
	BOOL pg_draw[2][MAXNUM_TEAM];			//!< �p���[�Q�[�W��`�悷�邩�ǂ���
	int pg_dx[2][MAXNUM_TEAM];			//!< �p���[�Q�[�W�̂��炵���B�Q�[�W�̌������Ɏg�p
};