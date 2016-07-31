/*=====================================================================================

Goluah!! Copyright (C) 2001-2004 aki, 2004-2016 At-sushi, 2014-2016 logger

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

=======================================================================================*/

/*==============================================================

	�L�����N�^�[��`�w�b�_

================================================================*/
#pragma once

#include <windows.h>
#include <stdio.h>

#include "gobjbase.h"
#include "cell.h"// �ʏ�Aare_editer�Ő��������w�b�_�[�t�@�C����������include���܂�


#define NEEDGAUGE_SA	(0.1f)
#define AIF_NUM			(32)

/*--------------------------------------------------------------
	���C���L�����N�^�[�N���X
----------------------------------------------------------------*/
class CCharacter : public CCharacterBase
{
public:
	CCharacter(CDI_CHARACTERINFO2 *info);
	virtual ~CCharacter();

protected:
	//*********************************************
	//  �I�[�o�[���C�h����֐�
	//*********************************************

	DWORD Message(DWORD msg,LPVOID pd,DWORD prm);
	DWORD Action();
	DWORD TouchA(ATTACKINFO *info,DWORD ta_eid);
	DWORD TouchB(ATTACKINFO *info,BOOL hit);
	BOOL Command_OnNormal(DWORD keyinfo);
	BOOL Command_Normal(DWORD keyinfo);
	BOOL Command_Hissatu(DWORD keyinfo);
	DWORD CommandCOM(DWORD wid);
	DWORD CmdCom_OnSystem(DWORD wid);
	DWORD CmdCom_OnHissatuAttacking(DWORD wid);
	DWORD CmdCom_OnNormal(DWORD wid);
	void InitAttackInfo();//�U���͏��̏�����
	void InitWazInfo();//�Z���̏�����

	//�ʏ퓮��
	void act_neutral();//�j���[�g����
	void act_crouch();//���Ⴊ��
	void act_rakka();//����
	void act_tojyo();//�o��
	void act_win();//����
	void act_walkf();//�O�i
	void act_walkb();//���
	void act_jamps();//�W�����v�\������
	void act_jampc();//�����W�����v
	void act_jampf();//�O�W�����v
	void act_jampb();//��W�����v
	//�ʏ�.�U��
	void act_att_sa();//����
	void act_att_sb();//����
	void act_att_sc();//����
	void act_att_ca();//����
	void act_att_cb();//����
	void act_att_cc();//����
	void act_att_ja();//���
	void act_att_jb();//��
	void act_att_jc();//��
	//���̑�
	void act_taiki();
	void act_koutai_out();
	void act_koutai_in();
	void act_koutai();
	void act_koutai2();
	void act_strikercomeon();

	void act_timeoverlose();//�^�C���I�[�o�[

	//*********************************************
	//�@CCharacter�N���X�Ǝ��̊֐�
	//*********************************************
private:
	void InitParameters();//�p�����[�^������
	void StartAttack(DWORD atk_info_id);
	BOOL com624624(int dt);

	//�U���͏��
	ATTACKINFO aif[AIF_NUM];

	//�ǉ��s��
	void act_dashf();
	void act_dashb();
	void act_nage();
	DWORD act_nagee(GOBJECT *pedat);
	void act_hnage();
	DWORD act_hnagee(GOBJECT *pedat);
	void act_tetu1();
	void act_tetu2();
	void act_tetu3();
	void act_snage();
	DWORD act_snagee(GOBJECT *pedat);
	void act_striker1();
	void act_striker2();
	void act_striker3();
	void act_striker4();

	//�I�v�V�����l
	BOOL opt_heavy_hit;
	BOOL opt_super_armer;
};

extern LPTSTR CharDirectory;

//�s��ID�������ɒǉ�����

#define ACTID_NAGE1				(ACTID_ATTACK | ACTID_NAGE    | 0x0001)
#define ACTID_TETU1				(ACTID_ATTACK | ACTID_HISSATU | 0x0002)
#define ACTID_TETU2				(ACTID_ATTACK | ACTID_HISSATU | 0x0003)
#define ACTID_TETU3				(ACTID_ATTACK | ACTID_HISSATU | 0x0004)
#define ACTID_HNAGE				(ACTID_ATTACK | ACTID_HISSATU | ACTID_NAGE | 0x0005)
#define ACTID_SNAGE				(ACTID_ATTACK | ACTID_HISSATU | ACTID_NAGE | 0x0006)
#define ACTID_DASHF				(0x0007)
#define ACTID_DASHB				(0x0008)
#define ACTID_STRIKER1			(ACTID_INOUT  | 0x0009)//�X�g���C�J�[�U��
#define ACTID_STRIKER2			(ACTID_INOUT  | 0x000A)
#define ACTID_STRIKER3			(ACTID_INOUT  | 0x000B)
#define ACTID_STRIKER4			(ACTID_INOUT  | 0x000C)//���������ƋA���Ă���

//�U���͔ԍ��������ɒ�`����
#define ATTACKINFO_STAND_A		0
#define ATTACKINFO_STAND_B		1
#define ATTACKINFO_STAND_C		2
#define ATTACKINFO_CROUCH_A		3
#define ATTACKINFO_CROUCH_B		4
#define ATTACKINFO_CROUCH_C		5
#define ATTACKINFO_JAMP_A		6
#define ATTACKINFO_JAMP_B		7
#define ATTACKINFO_JAMP_C		8
#define ATTACKINFO_NAGE			9
#define ATTACKINFO_HNAGE1		10
#define ATTACKINFO_HNAGE2		11
#define ATTACKINFO_CHARGE1		12
#define ATTACKINFO_CHARGE2		13
#define ATTACKINFO_CHARGE3		14
#define ATTACKINFO_SNAGE1		15
#define ATTACKINFO_SNAGE2		16


//�U��ID�i�K�v������ΕύX����j
#define ATTACK_WEAK				1
#define ATTACK_MIDDLE			2
#define ATTACK_STRONG			3
#define ATTACK_HISSATU			4

//���[�U�[��`���b�Z�[�W
//�����p���b�Z�[�W
#define GOBJMSG_NAGE1			(GOBJMSG_USERDEFINE | 0x0001)	//�ʏ퓊��
#define GOBJMSG_NAGE2			(GOBJMSG_USERDEFINE | 0x0002)	//�K�E����
#define GOBJMSG_NAGE3			(GOBJMSG_USERDEFINE | 0x0003)	//���K�E����

//�I�v�V������`
#define OPTIONS_CHAIN_COMBO		0x00000001
#define OPTIONS_HEAVY_HIT		0x00000002
#define OPTIONS_SUPER_ARMER		0x00000004
#define OPTIONS_HP_UP			0x00000008
#define OPTIONS_HP_UP2			0x00000010

//�ԍ�����`
#define MAAI_NAGE				(20)
#define MAAI_HNAGE				(35)
#define MAAI_SNAGE				(50)

//wav�ԍ�
#define wave_tojyo				4
#define wave_win				5
#define wave_nage				6
#define wave_charge				7
#define wave_hnage1				8
#define wave_hnage2				9
#define wave_snage1				10
#define wave_snage2				11

//YEAH!!
#define YEAH					TRUE
