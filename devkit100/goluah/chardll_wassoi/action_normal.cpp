
/*==============================================================

	行動関数

================================================================*/
#include "character.h"
#include <math.h>

//*********************************************************
//　　通常動作
//*********************************************************

void CCharacter::act_neutral()//ニュートラル
{
	pdat->cnow = CELL_NEUTRAL;
}

void CCharacter::act_crouch()//しゃがみ
{
	pdat->cnow = CELL_CROUCH;
}

void CCharacter::act_rakka()//落下
{
	JiyuuRakka(2,TRUE);
	pdat->cnow = CELL_JAMP1;
}

void CCharacter::act_tojyo()//登場
{
	if(pdat->counter == 0){
		movex(-300);
		PlayMySound(wave_tojyo);
	}

	if(pdat->counter < 50){
		movex(5);
		pdat->cnow = CELL_KORORIN1;
		pdat->rot = pdat->counter * 360 / 49;

		if(pdat->counter < 13)
			pdat->y = -sin(3.1415 / 180.0 * (double)pdat->rot) * 39.0;
		else if(pdat->counter < 25)
			pdat->y = -44 + sin(3.1415 / 180.0 * (double)pdat->rot) * 5.0;
		else if(pdat->counter < 37)
			pdat->y = -44 - sin(3.1415 / 180.0 * (double)pdat->rot) * 5.0;
		else
			pdat->y = sin(3.1415 / 180.0 * (double)pdat->rot) * 39.0;

	}
	else{
		if(pdat->counter == 50)pdat->vy=-10;
		pdat->y += pdat->vy;
		pdat->vy++;
		pdat->cnow = CELL_WALK2;
		if(pdat->y > 0){
			pdat->aid=ACTID_NEUTRAL;
			pdat->y = 0;
			Message2System(MSGOBJ2SYS_TOJYOEND,0);
		}
	}
}

void CCharacter::act_win()//勝利
{
	if(pdat->counter==0){
		PlayMySound(wave_win);
		LoadAndSetKatiSerif();
	}

	DWORD z_end = 30;
	DWORD a_spd = 1;
	DWORD a_end = z_end + a_spd*4*4*2;
	DWORD b_spd = 1;
	DWORD b_end = a_end + b_spd*3*4*2;
	int s_spd = 3;
	DWORD s_len = 5;
	DWORD s_end = b_end + s_len*2 + 2;
	DWORD c_spd = 1;
	DWORD c_end = s_end + c_spd*3*4*2;


	if(pdat->counter < z_end){
		switch((pdat->counter/4)%6){
		case 0:pdat->cnow=CELL_WALK3;pdat->muki=FALSE;break;
		case 1:pdat->cnow=CELL_WALK1;pdat->muki=FALSE;break;
		case 2:pdat->cnow=CELL_WALK2;pdat->muki=FALSE;break;
		case 3:pdat->cnow=CELL_WALK3;pdat->muki=TRUE;break;
		case 4:pdat->cnow=CELL_WALK1;pdat->muki=TRUE;break;
		case 5:pdat->cnow=CELL_WALK2;pdat->muki=TRUE;break;
		}
	}
	else if(pdat->counter < z_end + a_spd*1 ){pdat->cnow=CELL_WINDANCE_A1;pdat->muki=TRUE;}//もにょもにょ
	else if(pdat->counter < z_end + a_spd*2 ){pdat->cnow=CELL_WINDANCE_A2;pdat->muki=TRUE;}
	else if(pdat->counter < z_end + a_spd*3 ){pdat->cnow=CELL_WINDANCE_A3;pdat->muki=TRUE;}
	else if(pdat->counter < z_end + a_spd*4 ){pdat->cnow=CELL_WINDANCE_A4;pdat->muki=TRUE;}
	else if(pdat->counter < z_end + a_spd*5 ){pdat->cnow=CELL_WINDANCE_A3;pdat->muki=TRUE;}
	else if(pdat->counter < z_end + a_spd*6 ){pdat->cnow=CELL_WINDANCE_A2;pdat->muki=TRUE;}
	else if(pdat->counter < z_end + a_spd*7 ){pdat->cnow=CELL_WINDANCE_A1;pdat->muki=TRUE;}
	else if(pdat->counter < z_end + a_spd*8 ){pdat->cnow=CELL_WINDANCE_A2;pdat->muki=TRUE;}
	else if(pdat->counter < z_end + a_spd*9 ){pdat->cnow=CELL_WINDANCE_A3;pdat->muki=TRUE;}
	else if(pdat->counter < z_end + a_spd*10){pdat->cnow=CELL_WINDANCE_A4;pdat->muki=TRUE;}
	else if(pdat->counter < z_end + a_spd*11){pdat->cnow=CELL_WINDANCE_A3;pdat->muki=TRUE;}
	else if(pdat->counter < z_end + a_spd*12){pdat->cnow=CELL_WINDANCE_A2;pdat->muki=TRUE;}
	else if(pdat->counter < z_end + a_spd*13){pdat->cnow=CELL_WINDANCE_A1;pdat->muki=TRUE;}
	else if(pdat->counter < z_end + a_spd*14){pdat->cnow=CELL_WINDANCE_A1;pdat->muki=FALSE;}
	else if(pdat->counter < z_end + a_spd*15){pdat->cnow=CELL_WINDANCE_A2;pdat->muki=FALSE;}
	else if(pdat->counter < z_end + a_spd*16){pdat->cnow=CELL_WINDANCE_A3;pdat->muki=FALSE;}
	else if(pdat->counter < z_end + a_spd*17){pdat->cnow=CELL_WINDANCE_A4;pdat->muki=FALSE;}
	else if(pdat->counter < z_end + a_spd*18){pdat->cnow=CELL_WINDANCE_A3;pdat->muki=FALSE;}
	else if(pdat->counter < z_end + a_spd*19){pdat->cnow=CELL_WINDANCE_A2;pdat->muki=FALSE;}
	else if(pdat->counter < z_end + a_spd*20){pdat->cnow=CELL_WINDANCE_A1;pdat->muki=FALSE;}
	else if(pdat->counter < z_end + a_spd*21){pdat->cnow=CELL_WINDANCE_A2;pdat->muki=FALSE;}
	else if(pdat->counter < z_end + a_spd*22){pdat->cnow=CELL_WINDANCE_A3;pdat->muki=FALSE;}
	else if(pdat->counter < z_end + a_spd*23){pdat->cnow=CELL_WINDANCE_A4;pdat->muki=FALSE;}
	else if(pdat->counter < z_end + a_spd*24){pdat->cnow=CELL_WINDANCE_A3;pdat->muki=FALSE;}
	else if(pdat->counter < z_end + a_spd*25){pdat->cnow=CELL_WINDANCE_A2;pdat->muki=FALSE;}
	else if(pdat->counter < z_end + a_spd*26){pdat->cnow=CELL_WINDANCE_A1;pdat->muki=FALSE;}

	else if(pdat->counter < a_end + b_spd*1 ){pdat->cnow=CELL_WINDANCE_B1;pdat->muki=TRUE;}//もにょもにょ２
	else if(pdat->counter < a_end + b_spd*2 ){pdat->cnow=CELL_WINDANCE_B2;pdat->muki=TRUE;}
	else if(pdat->counter < a_end + b_spd*3 ){pdat->cnow=CELL_WINDANCE_B3;pdat->muki=TRUE;}
	else if(pdat->counter < a_end + b_spd*4 ){pdat->cnow=CELL_WINDANCE_B2;pdat->muki=TRUE;}
	else if(pdat->counter < a_end + b_spd*5 ){pdat->cnow=CELL_WINDANCE_B1;pdat->muki=TRUE;}
	else if(pdat->counter < a_end + b_spd*6 ){pdat->cnow=CELL_WINDANCE_B2;pdat->muki=TRUE;}
	else if(pdat->counter < a_end + b_spd*7 ){pdat->cnow=CELL_WINDANCE_B3;pdat->muki=TRUE;}
	else if(pdat->counter < a_end + b_spd*8 ){pdat->cnow=CELL_WINDANCE_B2;pdat->muki=TRUE;}
	else if(pdat->counter < a_end + b_spd*9 ){pdat->cnow=CELL_WINDANCE_B1;pdat->muki=TRUE;}
	else if(pdat->counter < a_end + b_spd*10){pdat->cnow=CELL_WINDANCE_B1;pdat->muki=FALSE;}
	else if(pdat->counter < a_end + b_spd*11){pdat->cnow=CELL_WINDANCE_B2;pdat->muki=FALSE;}
	else if(pdat->counter < a_end + b_spd*12){pdat->cnow=CELL_WINDANCE_B3;pdat->muki=FALSE;}
	else if(pdat->counter < a_end + b_spd*13){pdat->cnow=CELL_WINDANCE_B2;pdat->muki=FALSE;}
	else if(pdat->counter < a_end + b_spd*14){pdat->cnow=CELL_WINDANCE_B1;pdat->muki=FALSE;}
	else if(pdat->counter < a_end + b_spd*15){pdat->cnow=CELL_WINDANCE_B2;pdat->muki=FALSE;}
	else if(pdat->counter < a_end + b_spd*16){pdat->cnow=CELL_WINDANCE_B3;pdat->muki=FALSE;}
	else if(pdat->counter < a_end + b_spd*17){pdat->cnow=CELL_WINDANCE_B2;pdat->muki=FALSE;}
	else if(pdat->counter < a_end + b_spd*18){pdat->cnow=CELL_WINDANCE_B1;pdat->muki=FALSE;}

	else if(pdat->counter < b_end + 1){pdat->cnow=CELL_WINDANCE_A1;pdat->muki=TRUE;}//すーい
	else if(pdat->counter < b_end + 1 + s_len){pdat->cnow=CELL_WINDANCE_SUI;pdat->muki=TRUE;movex(s_spd);}
	else if(pdat->counter < b_end + 2 + s_len){pdat->cnow=CELL_WINDANCE_A1;pdat->muki=FALSE;}//すーい
	else if(pdat->counter < b_end + 2 + s_len*2){pdat->cnow=CELL_WINDANCE_SUI;pdat->muki=FALSE;movex(-1*s_spd);}

	else if(pdat->counter < s_end + c_spd*1 ){pdat->cnow=CELL_WINDANCE_C1;pdat->muki=TRUE;}//もにょもにょ３
	else if(pdat->counter < s_end + c_spd*2 ){pdat->cnow=CELL_WINDANCE_C2;pdat->muki=TRUE;}
	else if(pdat->counter < s_end + c_spd*3 ){pdat->cnow=CELL_WINDANCE_C3;pdat->muki=TRUE;}
	else if(pdat->counter < s_end + c_spd*4 ){pdat->cnow=CELL_WINDANCE_C2;pdat->muki=TRUE;}
	else if(pdat->counter < s_end + c_spd*5 ){pdat->cnow=CELL_WINDANCE_C1;pdat->muki=TRUE;}
	else if(pdat->counter < s_end + c_spd*6 ){pdat->cnow=CELL_WINDANCE_C2;pdat->muki=TRUE;}
	else if(pdat->counter < s_end + c_spd*7 ){pdat->cnow=CELL_WINDANCE_C3;pdat->muki=TRUE;}
	else if(pdat->counter < s_end + c_spd*8 ){pdat->cnow=CELL_WINDANCE_C2;pdat->muki=TRUE;}
	else if(pdat->counter < s_end + c_spd*9 ){pdat->cnow=CELL_WINDANCE_C1;pdat->muki=TRUE;}
	else if(pdat->counter < s_end + c_spd*10){pdat->cnow=CELL_WINDANCE_C1;pdat->muki=FALSE;}
	else if(pdat->counter < s_end + c_spd*11){pdat->cnow=CELL_WINDANCE_C2;pdat->muki=FALSE;}
	else if(pdat->counter < s_end + c_spd*12){pdat->cnow=CELL_WINDANCE_C3;pdat->muki=FALSE;}
	else if(pdat->counter < s_end + c_spd*13){pdat->cnow=CELL_WINDANCE_C2;pdat->muki=FALSE;}
	else if(pdat->counter < s_end + c_spd*14){pdat->cnow=CELL_WINDANCE_C1;pdat->muki=FALSE;}
	else if(pdat->counter < s_end + c_spd*15){pdat->cnow=CELL_WINDANCE_C2;pdat->muki=FALSE;}
	else if(pdat->counter < s_end + c_spd*16){pdat->cnow=CELL_WINDANCE_C3;pdat->muki=FALSE;}
	else if(pdat->counter < s_end + c_spd*17){pdat->cnow=CELL_WINDANCE_C2;pdat->muki=FALSE;}
	else if(pdat->counter < s_end + c_spd*18){pdat->cnow=CELL_WINDANCE_C1;pdat->muki=FALSE;}

	else if(pdat->counter == c_end){//ぴょーん
		pdat->vy = -6;
		pdat->y += pdat->vy;
		pdat->cnow=CELL_WINDANCE_J1;pdat->muki=TRUE;
	}
	else if(pdat->y < 0){
		pdat->vy += pdat->counter%2;
		pdat->y  += pdat->vy;
		pdat->cnow=CELL_WINDANCE_J1;pdat->muki=TRUE;
	}
	else{
		pdat->y = 0;
		pdat->cnow=CELL_WINDANCE_J2;pdat->muki=TRUE;
	}

	if(pdat->counter == 240)
		Message2System(MSGOBJ2SYS_WINPOSEEND,0);
}

void CCharacter::act_timeoverlose()//タイムオーバー
{
	pdat->cnow = CELL_TLOSE;
}

void CCharacter::act_walkf()//前進
{
	pdat->cnow = CELL_WALK1+(pdat->counter/4)%3;
	movex(3);
}

void CCharacter::act_walkb()//後退
{
	pdat->cnow = CELL_WALKB1+(pdat->counter/4)%3;
	movex(-3);
}

void CCharacter::act_jamps()//ジャンプ予備動作
{
	if(pdat->counter >2)pdat->aid = ACTID_JAMPC;
	pdat->cnow = CELL_JAMP;
}

void CCharacter::act_jampc()//垂直ジャンプ
{
	if(pdat->counter == 0){pdat->vy=-34;pdat->vx=0;}
	pdat->cnow = CELL_JAMP1;
	JiyuuRakka(2,TRUE);
}

void CCharacter::act_jampf()//前ジャンプ
{
	if(pdat->counter == 0){pdat->vy=-34;pdat->vx=6;}
	pdat->cnow = CELL_JAMP1;
	JiyuuRakka(2, TRUE);;
}

void CCharacter::act_jampb()//後ジャンプ
{
	if(pdat->counter == 0){pdat->vy=-34;pdat->vx=-6;}
	pdat->cnow = CELL_JAMP1;
	JiyuuRakka(2,TRUE);
}

//*********************************************************
//　その他
//*********************************************************
void CCharacter::act_taiki()
{
	if(pdat->counter<2)pdat->cnow=CELL_CROUCH;
	else pdat->cnow= CELL_JAMP1;
	CCharacterBase::act_taiki();//キャラクター基本クラスの動作を流用
}

void CCharacter::act_koutai_out()
{
	if(pdat->counter<2)pdat->cnow=CELL_CROUCH;
	else pdat->cnow= CELL_JAMP1;
	CCharacterBase::act_koutai_out();//キャラクター基本クラスの動作を流用
}

void CCharacter::act_koutai_in()
{
	pdat->cnow= CELL_JAMP1;
	CCharacterBase::act_koutai_in();//キャラクター基本クラスの動作を流用
}

void CCharacter::act_koutai()
{
	pdat->cnow= CELL_JAMP1;
	CCharacterBase::act_koutai();//キャラクター基本クラスの動作を流用
}

void CCharacter::act_koutai2()
{
	//CCharacterBase::act_koutai2();//キャラクター基本クラスの動作を流用
	if(pdat->counter < 60){
		switch((pdat->counter/4)%6){
		case 0:pdat->cnow=CELL_WALK3;pdat->muki=FALSE;break;
		case 1:pdat->cnow=CELL_WALK1;pdat->muki=FALSE;break;
		case 2:pdat->cnow=CELL_WALK2;pdat->muki=FALSE;break;
		case 3:pdat->cnow=CELL_WALK3;pdat->muki=TRUE;break;
		case 4:pdat->cnow=CELL_WALK1;pdat->muki=TRUE;break;
		case 5:pdat->cnow=CELL_WALK2;pdat->muki=TRUE;break;
		}
	}
	else{
		pdat->aid = ACTID_NEUTRAL;
		Message2System(MSGOBJ2SYS_KOUTAIEND,0);
	}
}


//*********************************************************
//　　追加動作
//*********************************************************


void CCharacter::act_dashf()
{
	pdat->cnow = CELL_DASHF1+(pdat->counter/4)%4;
	movex(10);
}

void CCharacter::act_dashb()
{
	if(pdat->counter == 0){pdat->vy=-8;pdat->vx=-7;}

	pdat->cnow = CELL_DASHB1;

	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy++;
	if(pdat->y > 0){
		pdat->aid = ACTID_NEUTRAL;
		pdat->y =0;
	}
}

void CCharacter::act_okiagari_foword()
{
	pdat->muteki = TRUE;
	pdat->nagerare = FALSE;

	if(pdat->counter < 25){
		movex(10);
		pdat->cnow = CELL_KORORIN1;
		pdat->rot = pdat->counter * 15;

		if(pdat->counter < 7)
			pdat->y = -sin(3.1415 / 180.0 * (double)pdat->rot) * 39.0;
		else if(pdat->counter < 13)
			pdat->y = -44 + sin(3.1415 / 180.0 * (double)pdat->rot) * 5.0;
		else if(pdat->counter < 19)
			pdat->y = -44 - sin(3.1415 / 180.0 * (double)pdat->rot) * 5.0;
		else
			pdat->y = sin(3.1415 / 180.0 * (double)pdat->rot) * 39.0;

	}
	else{
		if(pdat->counter == 25){
			pdat->vy=-10;
			Furimuki();
			AddPowerGauge(-1.0f*NEEDGAUGE_REV);
		}
		pdat->y += pdat->vy;
		pdat->vy++;
		pdat->cnow = CELL_WALK2;
		if(pdat->y > 0){
			pdat->aid=ACTID_NEUTRAL;
			pdat->y = 0;
		}
	}
}

void CCharacter::act_okiagari_back()
{
	pdat->muteki = TRUE;
	pdat->nagerare = FALSE;

	if(pdat->counter < 25){
		movex(-10);
		pdat->revx = TRUE;
		pdat->cnow = CELL_KORORIN1;
		pdat->rot = pdat->counter * 15;

		if(pdat->counter < 7)
			pdat->y = -sin(3.1415 / 180.0 * (double)pdat->rot) * 39.0;
		else if(pdat->counter < 13)
			pdat->y = -44 + sin(3.1415 / 180.0 * (double)pdat->rot) * 5.0;
		else if(pdat->counter < 19)
			pdat->y = -44 - sin(3.1415 / 180.0 * (double)pdat->rot) * 5.0;
		else
			pdat->y = sin(3.1415 / 180.0 * (double)pdat->rot) * 39.0;

	}
	else{
		if(pdat->counter == 25){
			pdat->vy=-10;
			Furimuki();
			AddPowerGauge(-1.0f*NEEDGAUGE_REV);
			pdat->revx = FALSE;
		}
		pdat->y += pdat->vy;
		pdat->vy++;
		pdat->cnow = CELL_WALK2;
		if(pdat->y > 0){
			pdat->aid=ACTID_NEUTRAL;
			pdat->y = 0;
		}
	}
}

