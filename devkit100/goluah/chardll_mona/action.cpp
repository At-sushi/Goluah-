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

extern char *g_chardir;
extern DI_FUNCTIONS_S *funcs;
extern DI_FUNCTIONS_O *funco;
extern DI_FUNCTIONS_D *funcd;

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
	JiyuuRakka(ACTID_NEUTRAL,TRUE);

	pdat->cnow = CELL_JAMP1;
}

void CCharacter::act_tojyo()//�o��
{
	if(pdat->counter==0)PlayMySound(4);

	if(pdat->counter < 30)pdat->cnow=CELL_TOJO1;
	else if(pdat->counter < 33)pdat->cnow=CELL_TOJO2;
	else if(pdat->counter < 60)pdat->cnow=CELL_TOJO3;
	else if(pdat->counter < 68)pdat->cnow=CELL_TOJO4;
	else if(pdat->counter < 76)pdat->cnow=CELL_TOJO5;
	else if(pdat->counter < 84)pdat->cnow=CELL_TOJO6;
	else{//�s���̍Ō�ɂ͂�����s������
		pdat->aid = ACTID_NEUTRAL;
		(*funcs->msg2system)(pdat->id,MSGOBJ2SYS_TOJYOEND,0);
	}
}

void CCharacter::act_win()//����
{
	if(pdat->counter==0){
		PlayMySound(5);
		LoadAndSetKatiSerif();
	}
	
	if(pdat->counter==0){
		pdat->vy=-12;
		pdat->y = -1;
	}
	
	if(pdat->y < 0){
		pdat->y += pdat->vy;
		pdat->vy += pdat->counter%2;
		if(pdat->y >= 0){pdat->y=1;pdat->counter=100;}
		pdat->cnow = CELL_WIN1;
	}
	else{
		if(pdat->counter<105)pdat->cnow=CELL_WIN2;
		else pdat->cnow=CELL_WIN3;
	}

	if(pdat->counter==120)
		funcs->msg2system(pdat->id,MSGOBJ2SYS_WINPOSEEND,0);
}

void CCharacter::act_walkf()//�O�i
{
	pdat->cnow = CELL_WALK1+(pdat->counter/3)%2;
	movex(4);
}

void CCharacter::act_walkb()//���
{
	pdat->cnow = CELL_WALKB1+(pdat->counter/3)%2;
	movex(-4);
}

void CCharacter::act_jamps()//�W�����v�\������
{	
	if(pdat->counter >2)pdat->aid = ACTID_JAMPC;
	pdat->cnow = CELL_JAMP;
}

void CCharacter::act_jampc()//�����W�����v
{
	if(pdat->counter == 0){pdat->vy=-20;pdat->vx=0;}
	pdat->cnow = CELL_JAMP1;

	JiyuuRakka(ACTID_NEUTRAL,TRUE);
}

void CCharacter::act_jampf()//�O�W�����v
{
	if(pdat->counter == 0){pdat->vy=-20;pdat->vx=6;}
	pdat->cnow = CELL_JAMP1;

	JiyuuRakka(ACTID_NEUTRAL,TRUE);
}

void CCharacter::act_jampb()//��W�����v
{
	if(pdat->counter == 0){pdat->vy=-20;pdat->vx=-6;}

	pdat->cnow = CELL_JAMP1;
	JiyuuRakka(ACTID_NEUTRAL,TRUE);
}

//*********************************************************
//�ʏ�.�U��
//*********************************************************

void CCharacter::act_att_sa()//����
{
	pdat->cnow = CELL_SA;
	if(pdat->counter > 10){
		pdat->aid = ACTID_NEUTRAL;
	}

	if(pdat->counter == 1)StartAttack(ATTACKINFO_STAND_A);
}

void CCharacter::act_att_sb()//����
{
	if(pdat->counter == 1)StartAttack(ATTACKINFO_STAND_B);
	if(pdat->counter < 19)movex(3);

	if(pdat->counter < 3)pdat->cnow = CELL_SB1;
	else if(pdat->counter < 7)pdat->cnow = CELL_SB2;
	else if(pdat->counter < 15)pdat->cnow = CELL_SB3;
	else if(pdat->counter < 25)pdat->cnow = CELL_SB4;
	else{
		pdat->aid = ACTID_NEUTRAL;
	}
}

void CCharacter::act_att_sc()//����
{
	if(pdat->counter < 4)pdat->cnow = CELL_SC1;
	else if(pdat->counter < 8)pdat->cnow = CELL_SC2;
	else if(pdat->counter < 12)pdat->cnow = CELL_SC3;
	else if(pdat->counter < 15)pdat->cnow = CELL_SC4;
	else if(pdat->counter < 30)pdat->cnow = CELL_SC5;
	else{
		pdat->aid = ACTID_NEUTRAL;
		
	}
	if(pdat->counter < 24)movex(4);

	if(pdat->counter == 8)StartAttack(ATTACKINFO_STAND_C);
	if(pdat->counter ==12)StartAttack(ATTACKINFO_STAND_C2);
}

void CCharacter::act_att_ca()//����
{
	if(pdat->counter == 1)StartAttack(ATTACKINFO_CROUCH_A);

	pdat->cnow = CELL_CA;
	if(pdat->counter > 10){
		pdat->aid = ACTID_CROUCH;
	}
}

void CCharacter::act_att_cb()//����
{
	if(pdat->counter == 1)StartAttack(ATTACKINFO_CROUCH_C);

	if(pdat->counter < 2)pdat->cnow = CELL_CB1;
	else if(pdat->counter < 4)pdat->cnow = CELL_CB2;
	else if(pdat->counter < 6)pdat->cnow = CELL_CB3;
	else if(pdat->counter < 8)pdat->cnow = CELL_CB3;
	else if(pdat->counter <10)pdat->cnow = CELL_CB2;
	else if(pdat->counter <20)pdat->cnow = CELL_CB1;
	else{
		pdat->aid = ACTID_CROUCH;
	}
}

void CCharacter::act_att_cc()//����
{
	if(pdat->counter == 10)StartAttack(ATTACKINFO_CROUCH_C);
	
	if(pdat->counter < 4)pdat->cnow = CELL_CC1;
	else if(pdat->counter < 8)pdat->cnow = CELL_CC2;
	else if(pdat->counter <12)pdat->cnow = CELL_CC3;
	else if(pdat->counter <16)pdat->cnow = CELL_CC3;
	else if(pdat->counter <20)pdat->cnow = CELL_CC2;
	else if(pdat->counter <24)pdat->cnow = CELL_CC1;
	else{
		pdat->aid = ACTID_CROUCH;
	}
}

void CCharacter::act_att_ja()//���
{
	if(pdat->counter == 1)StartAttack(ATTACKINFO_JAMP_A);
	
	pdat->cnow = CELL_JA;
	if(pdat->counter > 20)pdat->aid = ACTID_RAKKA2;

	JiyuuRakka(ACTID_NEUTRAL,TRUE);
}

void CCharacter::act_att_jb()//��
{
	if(pdat->counter == 1)StartAttack(ATTACKINFO_JAMP_B);

	if(pdat->counter < 2)pdat->cnow = CELL_JB1;
	else if(pdat->counter < 4)pdat->cnow = CELL_JB2;
	else if(pdat->counter < 8)pdat->cnow = CELL_JB3;
	else if(pdat->counter <10)pdat->cnow = CELL_JB3;
	else if(pdat->counter <12)pdat->cnow = CELL_JB2;
	else if(pdat->counter <14)pdat->cnow = CELL_JB1;
	else{
		pdat->aid = ACTID_RAKKA2;
	}

	JiyuuRakka(ACTID_NEUTRAL,TRUE);
}

void CCharacter::act_att_jc()//��
{
	if(pdat->counter == 1)StartAttack(ATTACKINFO_JAMP_C);

	if(pdat->counter < 8)pdat->cnow = CELL_JC;
	else pdat->cnow = CELL_JAMP1;
	
	JiyuuRakka(ACTID_NEUTRAL,TRUE);
}


//*********************************************************
//�@���̑�
//*********************************************************
void CCharacter::act_taiki()
{
	if(pdat->counter<2)pdat->cnow=CELL_JAMP1;
	else pdat->cnow= CELL_JAMP1;
	CCharacterBase::act_taiki();//�L�����N�^�[��{�N���X�̓���𗬗p
}

void CCharacter::act_koutai_out()
{
	if(pdat->counter<2)pdat->cnow=CELL_JAMP1;
	else pdat->cnow= CELL_JAMP1;
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
