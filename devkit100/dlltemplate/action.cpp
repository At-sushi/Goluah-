
/*==============================================================

	�s���֐�

================================================================*/
#include "character.h"

//*********************************************************
//�@�@�ʏ퓮��
//*********************************************************

void CCharacter::act_neutral()//�j���[�g����
{
}

void CCharacter::act_crouch()//���Ⴊ��
{
}

void CCharacter::act_rakka()//����
{
	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);

	//pdat->cnow = CELL_;
}

void CCharacter::act_tojyo()//�o��
{
/*	{//�s���̍Ō�ɂ͂�����s������
		pdat->aid = ACTID_NEUTRAL;
		Message2System(MSGOBJ2SYS_TOJYOEND,0);
	}*/
}

void CCharacter::act_win()//����
{
	if(pdat->counter==0){
		LoadAndSetKatiSerif();
	}

	//�s���̍Ō�ɂ̓R�����s������
	//	Message2System(MSGOBJ2SYS_WINPOSEEND,0);
}

void CCharacter::act_walkf()//�O�i
{
}

void CCharacter::act_walkb()//���
{
}

void CCharacter::act_jamps()//�W�����v�\������
{
//	if(pdat->counter==0)pdat->cnow=CELL_;
//	else if(pdat->counter==1)pdat->cnow=CELL_;
//	else pdat->aid = ACTID_JAMPC;
}

void CCharacter::act_jampc()//�����W�����v
{
	if(pdat->counter == 0){pdat->vy=-20;pdat->vx=0;}//�������x

//	pdat->cnow = CELL_;
	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

void CCharacter::act_jampf()//�O�W�����v
{
	if(pdat->counter == 0){pdat->vy=-20;pdat->vx=6;}//�������x

//	pdat->cnow = CELL_;
	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

void CCharacter::act_jampb()//��W�����v
{
	if(pdat->counter == 0){pdat->vy=-20;pdat->vx=-6;}//�������x

//	pdat->cnow = CELL_;
	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

//*********************************************************
//�ʏ�.�U��
//*********************************************************

void CCharacter::act_att_sa()//����
{
}

void CCharacter::act_att_sb()//����
{
}

void CCharacter::act_att_sc()//����
{
}

void CCharacter::act_att_ca()//����
{
}

void CCharacter::act_att_cb()//����
{
}

void CCharacter::act_att_cc()//����
{
}

void CCharacter::act_att_ja()//���
{
	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

void CCharacter::act_att_jb()//��
{
	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

void CCharacter::act_att_jc()//��
{
	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}


//*********************************************************
//�@���̑�
//*********************************************************
void CCharacter::act_taiki()
{
//	if(pdat->counter<2)pdat->cnow=CELL_;
//	else pdat->cnow= CELL_;
	CCharacterBase::act_taiki();//�L�����N�^�[��{�N���X�̓���𗬗p
}

void CCharacter::act_koutai_out()
{
//	if(pdat->counter<2)pdat->cnow=CELL_;
//	else pdat->cnow= CELL_;
	CCharacterBase::act_koutai_out();//�L�����N�^�[��{�N���X�̓���𗬗p
}

void CCharacter::act_koutai_in()
{
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
/*	{//�s���̍Ō�ɂ͂�����s������
		pdat->aid = ACTID_NEUTRAL;
		Message2System(MSGOBJ2SYS_KOUTAIEND,0);
	}*/	
	CCharacterBase::act_koutai2();//�L�����N�^�[��{�N���X�̓���𗬗p
}

