/*==============================================================

	�s���֐��i�ʏ퓮��j

================================================================*/
#include "character.h"

//**************************************************************
//�@�@�ʏ퓮��
//**************************************************************

/*--------------------------------------------------------------
	�j���[�g����
----------------------------------------------------------------*/
void CCharacter::act_neutral()
{
	cell(CELL_NEUTRAL);
}

/*--------------------------------------------------------------
	���Ⴊ��
----------------------------------------------------------------*/
void CCharacter::act_crouch()
{
	cell(CELL_CROUCH);
}

/*--------------------------------------------------------------
	����
----------------------------------------------------------------*/
void CCharacter::act_rakka()
{
	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
	cell(CELL_JAMP4);
}

/*--------------------------------------------------------------
	�o��
----------------------------------------------------------------*/
void CCharacter::act_tojyo()
{
	if(ccounter==0)PlayMySound(wave_tojyo);

	if(pdat->counter < 30)pdat->cnow=CELL_TOJYO1;
	else if(pdat->counter < 33)pdat->cnow=CELL_TOJYO2;
	else if(pdat->counter < 60)pdat->cnow=CELL_TOJYO3;
	else if(pdat->counter < 68)pdat->cnow=CELL_TOJYO4;
	else if(pdat->counter < 76)pdat->cnow=CELL_TOJYO5;
	else{
		pdat->aid = ACTID_NEUTRAL;
		Message2System(MSGOBJ2SYS_TOJYOEND,0);
	}
}

/*--------------------------------------------------------------
	����
----------------------------------------------------------------*/
void CCharacter::act_win()
{
	if(ccounter==0)PlayMySound(wave_win);
	if(pdat->counter==0){
		LoadAndSetKatiSerif();
	}
	
	if(ccounter < 4*8*2)
	{
		switch( (ccounter/4)%8 )
		{
			case 0:cell(CELL_WIN1);break;
			case 1:cell(CELL_WIN2);break;
			case 2:cell(CELL_WIN3);break;
			case 3:cell(CELL_WIN4);break;
			case 4:cell(CELL_WIN5);break;
			case 5:cell(CELL_WIN4);break;
			case 6:cell(CELL_WIN3);break;
			case 7:cell(CELL_WIN2);break;
		}
	}
	else{//�s���̍Ō�ɂ̓R����s������
		Message2System(MSGOBJ2SYS_WINPOSEEND,0);
	}
}


/*--------------------------------------------------------------
	�X�g���C�J�[
----------------------------------------------------------------*/
void CCharacter::act_strikercomeon()
{
	if(ccounter==0)PlayMySound(wave_striker_call);
	ChangeAction(ACTID_NEUTRAL);
}


/*--------------------------------------------------------------
	�^�C���I�[�o�[
----------------------------------------------------------------*/
void CCharacter::act_timeoverlose()
{
	if(ccounter<2)cell(CELL_TLOSE1);
	else if(ccounter<4)cell(CELL_TLOSE2);
	else if(ccounter<4)cell(CELL_TLOSE3);
	else if(ccounter<4)cell(CELL_TLOSE4);
	else if(ccounter<4)cell(CELL_TLOSE5);
	else if(ccounter<4)cell(CELL_TLOSE6);
	else{
		cell( CELL_TLOSE2_1 + (ccounter/2)%2 );
	}
}


/*--------------------------------------------------------------
	�O�i
----------------------------------------------------------------*/
void CCharacter::act_walkf()
{
	cell(CELL_WALKF1 + (ccounter/3)%3);
	movex(+3);
}

/*--------------------------------------------------------------
	���
----------------------------------------------------------------*/

//��
#define CELL_WALKB1 CELL_WALKF1
#define CELL_WALKB2 CELL_WALKF2
#define CELL_WALKB3 CELL_WALKF3
#define CELL_WALKB4 CELL_WALKF4
#define CELL_WALKB5 CELL_WALKF5
#define CELL_WALKB6 CELL_WALKF6
#define CELL_WALKB7 CELL_WALKF7
#define CELL_WALKB8 CELL_WALKF8

void CCharacter::act_walkb()
{
	cell(CELL_WALKB1 + (ccounter/3)%3 );
	pdat->revx = TRUE;//��
	movex(-3);
}

/*--------------------------------------------------------------
	�_�b�V��(�O)
----------------------------------------------------------------*/
void CCharacter::act_dashf()
{
	cell(CELL_FDASH1 + (ccounter/3)%6);
	movex(+12);
}

/*--------------------------------------------------------------
	�_�b�V��(��)
----------------------------------------------------------------*/
void CCharacter::act_dashb()
{
	if(pdat->counter == 0){pdat->vy=-12;pdat->vx=-4;}//����x

	if(pdat->counter<3)cell(CELL_BDASH1);
	else if(pdat->counter<6)cell(CELL_BDASH2);
	else if(pdat->counter<9)cell(CELL_BDASH3);
	else if(pdat->counter<12)cell(CELL_BDASH4);
	else if(pdat->counter<15)cell(CELL_BDASH5);
	else if(pdat->counter<18)cell(CELL_BDASH6);

	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

/*--------------------------------------------------------------
	�W�����v�\���
----------------------------------------------------------------*/
void CCharacter::act_jamps()
{
	if(pdat->counter==0)cell(CELL_JAMP1);
	else if(pdat->counter==1)cell(CELL_JAMP2);
	else pdat->aid = ACTID_JAMPC;
}

/*--------------------------------------------------------------
	2�i�W�����v�\���
----------------------------------------------------------------*/
void CCharacter::act_2nd_step()
{
	m_2nd_step_flag = FALSE;

	if(pdat->counter==0)cell(CELL_JAMP1);
	else if(pdat->counter==1)cell(CELL_JAMP2);
	else pdat->aid = ACTID_JAMPC;
}

/*--------------------------------------------------------------
	�����W�����v
----------------------------------------------------------------*/
void CCharacter::act_jampc()
{
	if(pdat->counter == 0){pdat->vy=-17;pdat->vx=0;}//����x

	cell( (pdat->counter<2) ? CELL_JAMP3 : CELL_JAMP4 );
	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

/*--------------------------------------------------------------
	�O�W�����v
----------------------------------------------------------------*/
void CCharacter::act_jampf()
{
	if(pdat->counter == 0){pdat->vy=-17;pdat->vx=6;}//����x
	
	cell( (pdat->counter<2) ? CELL_JAMP3 : CELL_JAMP4 );
	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

/*--------------------------------------------------------------
	��W�����v
----------------------------------------------------------------*/
void CCharacter::act_jampb()
{
	if(pdat->counter == 0){pdat->vy=-17;pdat->vx=-6;}//����x

	cell( (pdat->counter<2) ? CELL_JAMP3 : CELL_JAMP4 );
	JiyuuRakka(1, TRUE, ACTID_NEUTRAL);
}

/*--------------------------------------------------------------
	���n����(��)
----------------------------------------------------------------*/
void CCharacter::act_landing()
{
	if(pdat->counter < 2)cell(CELL_LANDING1);
	else if(pdat->counter < 4)cell(CELL_LANDING2);
	else if(pdat->counter < 6)cell(CELL_LANDING1);
	else ChangeAction(ACTID_NEUTRAL);
}

//***************************************************************
//�@���̑�
//***************************************************************

/*--------------------------------------------------------------
	�ҋ@�s��
----------------------------------------------------------------*/
void CCharacter::act_taiki()
{
	pdat->cnow= CELL_JAMP4;
	CCharacterBase::act_taiki();//�L�����N�^�[��{�N���X�̓���𗬗p
}

/*--------------------------------------------------------------
	��サ�āA��ʊO�ɏo�čs��
----------------------------------------------------------------*/
void CCharacter::act_koutai_out()
{
	pdat->cnow= CELL_JAMP4;
	CCharacterBase::act_koutai_out();//�L�����N�^�[��{�N���X�̓���𗬗p
}

/*--------------------------------------------------------------
	��サ�āA��ʓ�ɓ��Ă���
----------------------------------------------------------------*/
void CCharacter::act_koutai_in()
{
	pdat->cnow= CELL_JAMP4;
	CCharacterBase::act_koutai_in();//�L�����N�^�[��{�N���X�̓���𗬗p
}

/*--------------------------------------------------------------
	���H
	�Ȃ񂾂���H
----------------------------------------------------------------*/
void CCharacter::act_koutai()
{
	pdat->cnow= CELL_JAMP4;
	CCharacterBase::act_koutai();//�L�����N�^�[��{�N���X�̓���𗬗p
}

/*--------------------------------------------------------------
	��サ�āA��ʓ�ɓ��Ă���
	KOF���C�N�ȑΐ�`���̏ꍇ
----------------------------------------------------------------*/
void CCharacter::act_koutai2()
{
/*	{//�s���̍Ō�ɂ͂����s������
		pdat->aid = ACTID_NEUTRAL;
		Message2System(MSGOBJ2SYS_KOUTAIEND,0);
	}*/	
	pdat->cnow= CELL_JAMP4;
	CCharacterBase::act_koutai2();//�L�����N�^�[��{�N���X�̓���𗬗p
}

