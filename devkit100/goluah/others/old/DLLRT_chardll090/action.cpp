#include <windows.h>
#include <stdio.h>

//#define LPDIRECT3DTEXTURE8 LPVOID
#include <d3d8.h>
#include <d3dx8.h>

#include "define_gcd.h"
#include "gobject.h"
#include "gobjbase.h"

#include "character.h"
#include "cell.h"

//*********************************************************
//�@�@�ʏ퓮��
//*********************************************************

void CCharacter::act_neutral()//�j���[�g����
{
	GOBJECT* pdat = GetGObject();
}

void CCharacter::act_crouch()//���Ⴊ��
{
	GOBJECT* pdat = GetGObject();
}

void CCharacter::act_rakka()//����
{
	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
	GOBJECT* pdat = GetGObject();

	//pdat->cnow = CELL_;
}

void CCharacter::act_tojyo()//�o��
{
	GOBJECT* pdat = GetGObject();

/*	{//�s���̍Ō�ɂ͂�����s������
		pdat->aid = ACTID_NEUTRAL;
		Message2System(MSGOBJ2SYS_TOJYOEND,0);
	}*/
}

void CCharacter::act_win()//����
{
	GOBJECT* pdat = GetGObject();

	if(pdat->counter==0){
		LoadAndSetKatiSerif();
	}

	//�s���̍Ō�ɂ̓R�����s������
	//	Message2System(MSGOBJ2SYS_WINPOSEEND,0);
}

void CCharacter::act_walkf()//�O�i
{
	GOBJECT* pdat = GetGObject();
}

void CCharacter::act_walkb()//���
{
	GOBJECT* pdat = GetGObject();
}

void CCharacter::act_jamps()//�W�����v�\������
{	
	GOBJECT* pdat = GetGObject();

//	if(pdat->counter==0)pdat->cnow=CELL_;
//	else if(pdat->counter==1)pdat->cnow=CELL_;
//	else pdat->aid = ACTID_JAMPC;
}

void CCharacter::act_jampc()//�����W�����v
{
	GOBJECT* pdat = GetGObject();

	if(pdat->counter == 0){pdat->vy=-20;pdat->vx=0;}//�������x

//	pdat->cnow = CELL_;
	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

void CCharacter::act_jampf()//�O�W�����v
{
	GOBJECT* pdat = GetGObject();

	if(pdat->counter == 0){pdat->vy=-20;pdat->vx=6;}//�������x

//	pdat->cnow = CELL_;
	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

void CCharacter::act_jampb()//��W�����v
{
	GOBJECT* pdat = GetGObject();

	if(pdat->counter == 0){pdat->vy=-20;pdat->vx=-6;}//�������x

//	pdat->cnow = CELL_;
	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

//*********************************************************
//�ʏ�.�U��
//*********************************************************

void CCharacter::act_att_sa()//����
{
	GOBJECT* pdat = GetGObject();
}

void CCharacter::act_att_sb()//����
{
	GOBJECT* pdat = GetGObject();
}

void CCharacter::act_att_sc()//����
{
	GOBJECT* pdat = GetGObject();
}

void CCharacter::act_att_ca()//����
{
	GOBJECT* pdat = GetGObject();
}

void CCharacter::act_att_cb()//����
{
	GOBJECT* pdat = GetGObject();
}

void CCharacter::act_att_cc()//����
{
	GOBJECT* pdat = GetGObject();
}

void CCharacter::act_att_ja()//���
{
	GOBJECT* pdat = GetGObject();

	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

void CCharacter::act_att_jb()//��
{
	GOBJECT* pdat = GetGObject();

	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

void CCharacter::act_att_jc()//��
{
	GOBJECT* pdat = GetGObject();

	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}


//*********************************************************
//�@���̑�
//*********************************************************
void CCharacter::act_taiki()
{
//	GOBJECT* pdat = GetGObject();

//	if(pdat->counter<2)pdat->cnow=CELL_;
//	else pdat->cnow= CELL_;
	CCharacterBase::act_taiki();//�L�����N�^�[��{�N���X�̓���𗬗p
}

void CCharacter::act_koutai_out()
{
//	GOBJECT* pdat = GetGObject();

//	if(pdat->counter<2)pdat->cnow=CELL_;
//	else pdat->cnow= CELL_;
	CCharacterBase::act_koutai_out();//�L�����N�^�[��{�N���X�̓���𗬗p
}

void CCharacter::act_koutai_in()
{
//	GOBJECT* pdat = GetGObject();

//	pdat->cnow= CELL_;
	CCharacterBase::act_koutai_in();//�L�����N�^�[��{�N���X�̓���𗬗p
}

void CCharacter::act_koutai()
{
//	GOBJECT* pdat = GetGObject();

//	pdat->cnow= CELL_;
	CCharacterBase::act_koutai();//�L�����N�^�[��{�N���X�̓���𗬗p
}

void CCharacter::act_koutai2()
{
//	GOBJECT* pdat = GetGObject();

/*	{//�s���̍Ō�ɂ͂�����s������
		pdat->aid = ACTID_NEUTRAL;
		Message2System(MSGOBJ2SYS_KOUTAIEND,0);
	}*/	
	CCharacterBase::act_koutai2();//�L�����N�^�[��{�N���X�̓���𗬗p
}
