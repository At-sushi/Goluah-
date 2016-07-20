
/*==============================================================

	�s���֐�

================================================================*/
#include "character.h"

//*********************************************************
//�@�@�ʏ퓮��
//*********************************************************

void CCharacter::act_neutral()//�j���[�g����
{
	pdat->cnow = CELL_NEUTRAL;
}

void CCharacter::act_crouch()//���Ⴊ��
{
	pdat->cnow = CELL_CROUCH;
}

void CCharacter::act_rakka()//����
{
	pdat->cnow = CELL_JAMP1;

	JiyuuRakka(2, TRUE, ACTID_NEUTRAL);
}

void CCharacter::act_tojyo()//�o��
{
	if(pdat->counter==0)PlayMySound(wave_tojyo);

	DWORD spd=3;
	DWORD cyc=spd*8;
	if(pdat->counter < spd*1)pdat->cnow = CELL_TOJYO1;
	else if(pdat->counter < spd*2)pdat->cnow = CELL_TOJYO2;
	else if(pdat->counter < spd*3)pdat->cnow = CELL_TOJYO3;
	else if(pdat->counter < spd*4)pdat->cnow = CELL_TOJYO4;
	else if(pdat->counter < spd*5){pdat->cnow = CELL_TOJYO5;}
	else if(pdat->counter < spd*6)pdat->cnow = CELL_TOJYO4;
	else if(pdat->counter < spd*7)pdat->cnow = CELL_TOJYO3;
	else if(pdat->counter < spd*8)pdat->cnow = CELL_TOJYO2;
	else if(pdat->counter < cyc + spd*1)pdat->cnow = CELL_TOJYO1;
	else if(pdat->counter < cyc + spd*2)pdat->cnow = CELL_TOJYO2;
	else if(pdat->counter < cyc + spd*3)pdat->cnow = CELL_TOJYO3;
	else if(pdat->counter < cyc + spd*4)pdat->cnow = CELL_TOJYO4;
	else if(pdat->counter < cyc + spd*5){pdat->cnow = CELL_TOJYO5;}
	else if(pdat->counter < cyc + spd*6)pdat->cnow = CELL_TOJYO4;
	else if(pdat->counter < cyc + spd*7)pdat->cnow = CELL_TOJYO3;
	else if(pdat->counter < cyc + spd*8)pdat->cnow = CELL_TOJYO2;
	else if(pdat->counter < cyc*2 + spd*1)pdat->cnow = CELL_TOJYO1;
	else if(pdat->counter < cyc*2 + spd*2)pdat->cnow = CELL_TOJYO2;
	else if(pdat->counter < cyc*2 + spd*3)pdat->cnow = CELL_TOJYO3;
	else if(pdat->counter < cyc*2 + spd*4)pdat->cnow = CELL_TOJYO4;
	else if(pdat->counter < cyc*2 + spd*5)pdat->cnow = CELL_TOJYO5;
	else if(pdat->counter < cyc*2 + spd*6)pdat->cnow = CELL_TOJYO4;
	else if(pdat->counter < cyc*2 + spd*7)pdat->cnow = CELL_TOJYO3;
	else if(pdat->counter < cyc*2 + spd*8+20)pdat->cnow = CELL_TOJYO2;
	else{
		pdat->aid=ACTID_NEUTRAL;
		Message2System(MSGOBJ2SYS_TOJYOEND,0);
	}

	if(pdat->counter == spd*5)AddEffect(EFCTID_MARK2,(int)pdat->x + zurex(70),-70,0);
	if(pdat->counter == spd*5 +cyc)AddEffect(EFCTID_MARK2,(int)pdat->x + zurex(70),-70,0);
	if(pdat->counter == spd*5 +cyc*2)AddEffect(EFCTID_MARK2,(int)pdat->x + zurex(70),-70,0);
}

void CCharacter::act_win()//����
{
	if(pdat->counter==0){
		PlayMySound(wave_win);
		LoadAndSetKatiSerif();
	}

	DWORD spd=2;
	if(pdat->counter < spd*1)pdat->cnow = CELL_WIN1;
	else if(pdat->counter < spd*2)pdat->cnow = CELL_WIN2;
	else if(pdat->counter < spd*3)pdat->cnow = CELL_WIN3;
	else if(pdat->counter < spd*4)pdat->cnow = CELL_WIN4;
	else if(pdat->counter < spd*5)pdat->cnow = CELL_WIN5;
	else pdat->cnow = CELL_WIN6;

	if(pdat->counter == 120)
		Message2System(MSGOBJ2SYS_WINPOSEEND,0);
}

void CCharacter::act_timeoverlose()//�^�C���I�[�o�[
{
	pdat->cnow = CELL_TLOSE;
}

void CCharacter::act_walkf()//�O�i
{
	pdat->cnow = CELL_WALKF1+(pdat->counter/4)%3;
	movex(3);
}

void CCharacter::act_walkb()//���
{
	pdat->cnow = CELL_WALKB1+(pdat->counter/4)%3;
	movex(-3);
}

void CCharacter::act_jamps()//�W�����v�\������
{
	if(pdat->counter <1)pdat->cnow = CELL_JAMPS1;
	else if(pdat->counter <2)pdat->cnow = CELL_JAMPS2;
	else pdat->aid = ACTID_JAMPC;
}

void CCharacter::act_jampc()//�����W�����v
{
	if(pdat->counter == 0){pdat->vy=-27;pdat->vx=0;}

	if(pdat->vy < 0)pdat->cnow = CELL_JAMP1;
	else pdat->cnow = CELL_JAMP2;

	pdat->y += pdat->vy;
	pdat->vy += 1 + pdat->counter % 3;
	if(pdat->y > 0){
		pdat->aid = ACTID_NEUTRAL;
		pdat->y =0;
		Furimuki();
	}
}

void CCharacter::act_jampf()//�O�W�����v
{
	if(pdat->counter == 0){pdat->vy=-27;pdat->vx=4;}

	if(pdat->vy < 0)pdat->cnow = CELL_JAMP1;
	else pdat->cnow = CELL_JAMP2;

	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy += 1 + pdat->counter % 3;
	if(pdat->y > 0){
		pdat->aid = ACTID_NEUTRAL;
		pdat->y =0;
		Furimuki();
	}
}

void CCharacter::act_jampb()//��W�����v
{
	if(pdat->counter == 0){pdat->vy=-27;pdat->vx=-4;}

	if(pdat->vy < 0)pdat->cnow = CELL_JAMP1;
	else pdat->cnow = CELL_JAMP2;

	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy += 1+pdat->counter%3;
	if(pdat->y > 0){
		pdat->aid = ACTID_NEUTRAL;
		pdat->y =0;
		Furimuki();
	}
}


//*********************************************************
//�@���̑�
//*********************************************************

void CCharacter::act_taiki()
{
	pdat->cnow= CELL_JAMP1;
	CCharacterBase::act_taiki();//�L�����N�^�[��{�N���X�̓���𗬗p
}

void CCharacter::act_koutai_out()
{
	pdat->cnow= CELL_JAMP1;
	CCharacterBase::act_koutai_out();//�L�����N�^�[��{�N���X�̓���𗬗p
}

void CCharacter::act_koutai_in()
{
	pdat->cnow= CELL_JAMP1;
	CCharacterBase::act_koutai_in();//�L�����N�^�[��{�N���X�̓���𗬗p
}

void CCharacter::act_koutai()
{
	pdat->cnow= CELL_JAMP1;
	CCharacterBase::act_koutai();//�L�����N�^�[��{�N���X�̓���𗬗p
}

void CCharacter::act_koutai2()
{
	CCharacterBase::act_koutai2();//�L�����N�^�[��{�N���X�̓���𗬗p
}

//*********************************************************
//�@�ǉ��s��
//*********************************************************

void CCharacter::act_dashf()
{
	pdat->cnow = CELL_DASHF1+(pdat->counter/3)%3;
	movex(5);
}

void CCharacter::act_dashb()
{
	pdat->cnow = CELL_DASHB1+(pdat->counter/3)%3;
	movex(-5);
}

void CCharacter::act_strikercomeon()
{
	DWORD spd=2;
	if(pdat->counter < spd*1)pdat->cnow = CELL_WIN1;
	else if(pdat->counter < spd*2)pdat->cnow = CELL_WIN2;
	else if(pdat->counter < spd*3)pdat->cnow = CELL_WIN3;
	else if(pdat->counter < spd*4)pdat->cnow = CELL_WIN4;
	else if(pdat->counter < spd*5)pdat->cnow = CELL_WIN5;
	else if(pdat->counter < spd*5+10)pdat->cnow = CELL_WIN6;
	else pdat->aid=ACTID_NEUTRAL;
}

