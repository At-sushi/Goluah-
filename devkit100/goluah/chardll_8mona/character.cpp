/*=====================================================================================

	�L�����N�^�[��`

=======================================================================================*/
#include "character.h"

//**************************************************************************************
//  Option Settings
//�@�����ɁA�L�����N�^�[�ɐݒ�ł���I�v�V������w�肵�܂�
//**************************************************************************************

CHARACTER_LOAD_OPTION option[] = {

	// �@{ �t���O, ���������߼��,�ˑ������߼��, �I�v�V������, ���|�C���g }
	{ OPTIONS_GUARD_CANCEL,		0, 0,	"Guard Cancel"	, 6 } ,
	{ OPTIONS_EXTRA_ATTACK,		0, 0,	"Extra Attacks"	, 6 } ,
	{ OPTIONS_SUPER_ARMER,		0, 0,	"Super Armer"	, 10} ,
	{ OPTIONS_EXTRA_FRIENDS,	0, 0,	"Extra Friends"	, 6 } ,
};


//**************************************************************************************
//  Character Information
//�@�����ɃL�����N�^�[�̖��O��L�q���܂�
//**************************************************************************************

CCharacterInfo CharInfo("8���g���i�[",		// �L�����N�^�[�̖��O�i�ő�31�o�C�g�j
						CDI_VERSION,		// �L�����N�^�[DLL�C���^�[�t�F�C�X�̃o�[�W����
						option,				// �I�v�V�����\���̂ւ̃|�C���^
						sizeof(option) / sizeof(CHARACTER_LOAD_OPTION), // �I�v�V�����̐�
						14,					// �I�v�V�����ő�|�C���g��
						FALSE);				// �l�b�g�ΐ�Ή����ǂ��� �����̊g���̂��߂ɗp�ӂ���Ă���B���݂�0��n���Ȃ���΂Ȃ�Ȃ��i�΁j

char* CharDirectory = NULL;

//�ݒ�}�N���BCCharacterInfo�ϐ������ƁA�L�����N�^�[�N���X����w�肵�܂�
SET_CHARACTERINFO(CharInfo, CCharacter)


/*--------------------------------------------------------------------------------------
	�L�����N�^�[������
	�ʏ�A������K�v����܂���
----------------------------------------------------------------------------------------*/

CCharacter::CCharacter(CDI_CHARACTERINFO2 *info) : CCharacterBase(info)
{
	CharDirectory = info->dir;	// �L�����N�^�[�̃t�H���_���
}

/*--------------------------------------------------------------------------------------
	�L�����N�^�[�j��
	�ʏ�A������K�v����܂���
----------------------------------------------------------------------------------------*/
CCharacter::~CCharacter()
{
}

/*--------------------------------------------------------------------------------------
	�p�����[�^����
	�����ɁA�e��p�����[�^�̏�������ǉ���܂�
----------------------------------------------------------------------------------------*/
void CCharacter::InitParameters()
{
	opt_gcancel		= (option_flags&OPTIONS_GUARD_CANCEL) ? TRUE : FALSE;
	opt_exattack	= (option_flags&OPTIONS_EXTRA_ATTACK) ? TRUE : FALSE;
	opt_ikari		= TRUE;
	opt_super_armer	= (option_flags&OPTIONS_SUPER_ARMER ) ? TRUE : FALSE;
	opt_exfriend	= (option_flags&OPTIONS_EXTRA_FRIENDS) ? TRUE : FALSE;

	pdat->hpmax			= 850;					//�ő�̗͒l
	pdat->gaugemax		= 1;					//�ő�Q�[�W
	pdat->gaugetype		= GOBJFLGA_POWGAUGE2;	//�Q�[�W�^�C�v
}


/*--------------------------------------------------------------------------------------
	���b�Z�[�W����
	�x�[�X�N���X�ŏE��Ă��Ȃ����b�Z�[�W��E�������ꍇ�́A�R�R�ɏ�����ǉ���܂�
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Message(DWORD msg,LPVOID dat,DWORD prm)
{
	GOBJECT* pobjdat;

	switch(msg){
	case GOBJMSG_NAGE://�����A�G������
		{
			pobjdat = GetInfo(prm);
			if(pobjdat==NULL)return(0);
			return(act_nagee(pobjdat));
		}
		break;
	case GOBJMSG_IKARINAGE:
		{
			pobjdat = GetInfo(prm);
			if(pobjdat==NULL)return(0);
			return(act_ikarie(pobjdat));
		}
		break;
	case GOBJMSG_STRIKER://�x���U���v��
		if(pdat->aid == ACTID_TAIKICYU){
			pdat->objtype &= ~GOBJFLG_TOBETARGET;//�^�[�Q�b�g�ɂ͂Ȃ�Ȃ�
			ChangeAction( ACTID_STRIKER1);
			return(YEAH);
		}
		return(0);
	default:
		return(CCharacterBase::Message(msg,dat,prm));//�L�����N�^�[��{�N���X�ɏ�����C����
	}
	return(TRUE);
}


/*--------------------------------------------------------------------------------------
	�R�}���h���� ����
	GOBJMSG_COMMAND ���b�Z�[�W�ɑΉ����܂��B���݂̍s���ɂ��ĕ����̊֐���
	������������Ă��܂��B�K�؂ȂƂ���ɔ��菈����ǉ���Ă��������B
----------------------------------------------------------------------------------------*/

//��炢�E�K�[�h��
BOOL CCharacter::Command_OnSystem(DWORD key)
{
	if((pdat->aid & ACTID_GUARD) && opt_gcancel){
		if(!(pdat->aid & ACTID_KUCYU))
		{
			//�n��K�[�h��
			//�K�[�h�L�����Z��
			if(pdat->gauge>NEEDGAUGE_GC){
				if(key & KEYSTA_BC2){pdat->aid = ACTID_GCANCEL3;return TRUE;}
				else if(key & KEYSTA_BB2){pdat->aid = ACTID_GCANCEL2;return TRUE;}
				else if(key & KEYSTA_BA2){pdat->aid = ACTID_GCANCEL1;return TRUE;}
			}
		}
	}

	return CCharacterBase::Command_OnSystem(key);
}

BOOL CCharacter::Command_Hissatu(DWORD key)
{
	//�K�E�Z�R�}���h����
	if(pdat->aid & ACTID_KUCYU){
	}
	else{
		if( (pdat->gauge>=1.0) && com62426(60) && (key & KEYSTA_BC2) && EscapeOK()){
			pdat->aid = ACTID_ESCAPE;
			return TRUE;
		}
		if(com426(45)){
			if(key & KEYSTA_BC2){
				if(NageHantei(MAAI_HNAGE) && opt_ikari){
					if(ObjCatch(pdat->eid,GOBJMSG_IKARINAGE)){
						pdat->aid = ACTID_IKARI;return TRUE;
					}
				}
			}
		}
		if(com623(30)){
			if(key & KEYSTA_BC2){pdat->aid = ACTID_SAH3;return TRUE;}
			else if(key & KEYSTA_BB2){pdat->aid = ACTID_SAH2;return TRUE;}
			else if(key & KEYSTA_BA2){pdat->aid = ACTID_SAH1;return TRUE;}
		}
		if(com421(30)){
			if(key & KEYSTA_BC2){pdat->aid = ACTID_FIG3;return TRUE;}
			else if(key & KEYSTA_BB2){pdat->aid = ACTID_FIG2;return TRUE;}
			else if(key & KEYSTA_BA2){pdat->aid = ACTID_FIG1;return TRUE;}
		}
	}

	return CCharacterBase::Command_Hissatu(key);
}

BOOL CCharacter::Command_Normal(DWORD key)
{
	//�_�b�V��
	if(com66i(30)){pdat->aid=ACTID_DASHF;return TRUE;}
	if(com44i(30)){pdat->aid=ACTID_DASHB;return TRUE;}


	//��{�ʏ퓮��
	if(key & KEYSTA_UP){
		pdat->aid = ACTID_JAMPS;
		return TRUE;
	}
	if(pdat->aid==ACTID_DASHF || pdat->aid==ACTID_DASHB){
		return TRUE;
	}

	if(key & KEYSTA_DOWN){//���Ⴊ��
		pdat->aid = ACTID_CROUCH;
	}
	else if(key & KEYSTA_FOWORD){//�O��
		pdat->aid = ACTID_WALKF;
	}
	else if(key & KEYSTA_BACK){//����
		pdat->aid = ACTID_WALKB;
	}
	else{//�j���[�g�����|�[�Y
		pdat->aid = ACTID_NEUTRAL;
	}
	return FALSE;
}

BOOL CCharacter::Command_OnNormal(DWORD keyinfo)
{
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_KUCYU){////�W�����v���쒆
	}
	else{//�n�㓮�쒆
		//����
		if (keyinfo & KEYSTA_FOWORD && keyinfo & KEYSTA_BC2 &&
			NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE)) {
				pdat->aid = ACTID_NAGE1;
				return TRUE;
		}

		//�ǉ�Z
		if(opt_exattack)
		{
			DWORD key = keyinfo;

			// �_+C
			if((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD) && (key & KEYSTA_DOWN)){
				ChangeAction(  ACTID_HKICK );return TRUE;
			}

			// ��+C
			else if((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD)){
				ChangeAction(  ACTID_HAND );return TRUE;
			}

			// �_+B
			else if((key & KEYSTA_BB2) && (key & KEYSTA_FOWORD) && (key & KEYSTA_DOWN)){
				ChangeAction(  ACTID_UPPER_STEP );return TRUE;
			}
			// ��+B
			else if((key & KEYSTA_BB2) && (key & KEYSTA_FOWORD)){
				ChangeAction(  ACTID_BAT );return TRUE;
			}
		}
	}

	return CCharacterBase::Command_OnNormal(keyinfo);
}

BOOL CCharacter::Command_OnAttacking(DWORD key)
{	
	if( pdat->aid & ACTID_KUCYU )
	{
	}
	else
	{
		//�ǉ�`�F�[��
		if(opt_exattack)
		{
			// �_+C
			if((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD) && (key & KEYSTA_DOWN) && ChainCombo(CHAIN_HKICK)){
				ChangeAction(  ACTID_HKICK );
				return TRUE;
			}

			// ��+C
			else if((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD) && ChainCombo(CHAIN_HAND)){
				ChangeAction(  ACTID_HAND );return TRUE;
			}

			// �_+B
			else if((key & KEYSTA_BB2) && (key & KEYSTA_FOWORD) && (key & KEYSTA_DOWN) && ChainCombo(CHAIN_UPPER)){
				ChangeAction(  ACTID_UPPER_STEP );
				return TRUE;
			}
			// ��+B
			else if((key & KEYSTA_BB2) && (key & KEYSTA_FOWORD) && ChainCombo(CHAIN_BAT)){
				ChangeAction(  ACTID_BAT );
				return TRUE;
			}
		}
	}

	return CCharacterBase::Command_OnAttacking(key);
}

/*--------------------------------------------------------------------------------------
	�R�}���h���� ����(COM)
----------------------------------------------------------------------------------------*/

DWORD CCharacter::CommandCOM(DWORD wid)
{
	//�Q�[�W�`�F�b�N
	if(wid==ACTID_ESCAPE && (pdat->gauge<1.0f || !EscapeOK()) ){
		return FALSE;
	}
	if(wid==ACTID_GCANCEL1 || wid==ACTID_GCANCEL2 || wid==ACTID_GCANCEL3)
	{
		if(pdat->gauge<NEEDGAUGE_GC)return FALSE;
		if(!((pdat->aid&ACTID_GUARD) && opt_gcancel))return FALSE;
	}

	return CCharacterBase::CommandCOM(wid);
}

DWORD CCharacter::CmdCom_OnSystem(DWORD wid)
{
	//�K�[�L����
	if(wid==ACTID_GCANCEL1 || wid==ACTID_GCANCEL2 || wid==ACTID_GCANCEL3)
	{
		if(pdat->aid&ACTID_KUCYU)
		{
			return FALSE;
		}
		return FALSE;
	}

	return CCharacterBase::CmdCom_OnSystem(wid);
}

DWORD CCharacter::CmdCom_OnHissatuAttacking(DWORD wid)
{
	return CCharacterBase::CmdCom_OnHissatuAttacking(wid);
}

DWORD CCharacter::CmdCom_OnNormal(DWORD wid)
{
	//����
	if (wid==ACTID_NAGE1)
	{
		if(!(pdat->aid&ACTID_KUCYU) && !(pdat->aid&ACTID_ATTACK))
		{
		
			if(NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE)) {
				pdat->aid = ACTID_NAGE1;
				return TRUE;
			}
			else return FALSE;
		}
	}
	//�K�E����
	if(wid==ACTID_IKARI)
	{
		if(!(pdat->aid&ACTID_KUCYU) && !(pdat->aid&ACTID_ATTACK))
		{
			if(NageHantei(MAAI_HNAGE) && opt_ikari)
			if(ObjCatch(pdat->eid, GOBJMSG_IKARINAGE)) {
				pdat->aid = ACTID_NAGE1;
				return TRUE;
			}
		}
		return FALSE;
	}

	if(!(pdat->aid&ACTID_KUCYU) && (opt_exattack))
	{
		//�`�F�[���R���{����(�ǉ�)
		if(GetGObject()->aid & ACTID_ATTACK)
		{
			switch(wid)
			{
			case ACTID_HKICK		:return ChainCombo(CHAIN_HKICK);break;
			case ACTID_HAND			:return ChainCombo(CHAIN_HAND);break;
			case ACTID_UPPER_STEP	:return ChainCombo(CHAIN_UPPER);break;
			case ACTID_BAT			:return ChainCombo(CHAIN_BAT);break;
			}
		}
	}

	return CCharacterBase::CmdCom_OnNormal(wid);
}

/*--------------------------------------------------------------------------------------
	GOBJMSG_TOUCHB(�U��������)
----------------------------------------------------------------------------------------*/
DWORD CCharacter::TouchB(ATTACKINFO *info,BOOL hit)
{
	double dp = 0.0f;
	switch(info->id & 0x0000FFFF){
		case ATTACK_WEAK	:dp=0.02;break;
		case ATTACK_MIDDLE	:dp=0.03;break;
		case ATTACK_STRONG	:dp=0.04;break;
		case ATTACK_HISSATU	:dp=0.05;break;
		case ATTACK_FRIENDS :dp=-0.1;break;
	}
	if(!hit)dp*=0.5;
	AddPowerGauge(dp);

	return CCharacterBase::TouchB(info,hit);
}


/*--------------------------------------------------------------------------------------
	GOBJMSG_TOUCHA(�U����炢)
----------------------------------------------------------------------------------------*/
DWORD CCharacter::TouchA(ATTACKINFO *info,DWORD ta_eid)
{
	//�X�[�p�[�A�[�}�[
	BOOL sa = FALSE;
	if(opt_super_armer && (pdat->gauge>=NEEDGAUGE_SA))
	{
		switch(GetGObject()->aid){

		case ACTID_JAMPS	:sa=TRUE;						break;
		case ACTID_ATT_SA	:if(pdat->counter<2)sa=TRUE;	break;
		case ACTID_ATT_SB	:if(pdat->counter<4)sa=TRUE;	break;
		case ACTID_ATT_SC	:if(pdat->counter<8)sa=TRUE;	break;
		case ACTID_ATT_CA	:if(pdat->counter<2)sa=TRUE;	break;
		case ACTID_ATT_CB	:if(pdat->counter<4)sa=TRUE;	break;
		case ACTID_ATT_CC	:if(pdat->counter<8)sa=TRUE;	break;
		case ACTID_ATT_JA	:if(pdat->counter<2)sa=TRUE;	break;
		case ACTID_ATT_JB	:if(pdat->counter<4)sa=TRUE;	break;
		case ACTID_ATT_JC	:if(pdat->counter<4)sa=TRUE;	break;

		case ACTID_SAH1		:if(pdat->vy<-1)sa=TRUE;		break;
		case ACTID_SAH2		:if(pdat->vy<-1)sa=TRUE;		break;
		case ACTID_SAH3		:if(pdat->vy<-1)sa=TRUE;		break;
		case ACTID_DASHF	:sa=TRUE;						break;
		case ACTID_ESCAPE	:sa=TRUE;						break;

		case ACTID_BAT		:if(pdat->counter<4)sa=TRUE;	break;
		case ACTID_UPPER_STEP:sa=TRUE;						break;
		case ACTID_HAND		:if(pdat->counter<5)sa=TRUE;	break;
		case ACTID_HKICK	:if(pdat->counter<4)sa=TRUE;	break;

		default:return(0);
		}
	}
	isSuperArmer = sa;

	DWORD ret = CCharacterBase::TouchA(info,ta_eid);

	if(sa && (ret&0x10000000)){
		AddPowerGauge(-1.0f*NEEDGAUGE_SA);
	}

	isSuperArmer = FALSE;
	return ret;
}


/*--------------------------------------------------------------------------------------
	GOBJMSG_TOUCHC
----------------------------------------------------------------------------------------*/
DWORD CCharacter::TouchC(ATTACKINFO *info,DWORD tc_eid)
{
	if(TRUE)//info->id & 0xFFFF0000)//�ł��グ�n�̂`�p���
	{
		GOBJECT* pedat = GetInfo(tc_eid);
		if(!pedat){
			dprint("fail1");
			return TOUCHC_OK;
		}
		if(pedat->atk2.info1 < &aif[0] || pedat->atk2.info1 > &aif[AIF_NUM-1]){
			dprint("fail2");
			return TOUCHC_OK;
		}
		if(!(pedat->aid&ACTID_KURAI))
		{
			return TOUCHC_OK;
		}
		DWORD prevhit = (pedat->atk2.info1->id) & 0xFFFF0000;

		if(prevhit || (info->id & 0xFFFF0000) ){
			dprint("crnt=%08X , pre=%08X  %s",info->id & 0xFFFF0000,prevhit,
				(prevhit!=(info->id & 0xFFFF0000)) ? "+" : "");
		}

		switch(info->id & 0xFFFF0000)
		{
		case ATTACK_BAT	:
			{
				switch(prevhit)
				{
				case ATTACK_HKICK1:
				case ATTACK_HKICK2:
				case ATTACK_BAT	:
					return TOUCHC_CANCEL;
				}
			}
			break;
		case ATTACK_HKICK1	:
			{
				switch(prevhit)
				{
				case ATTACK_HKICK1:
				case ATTACK_HKICK2:
					return TOUCHC_CANCEL;
				}
			}
			break;
		case ATTACK_HKICK2	:
			{
				switch(prevhit)
				{
				case ATTACK_HKICK1:
				case ATTACK_HKICK2:
					return TOUCHC_CANCEL;
				}
			}
			break;
		case ATTACK_UPPER	:
			{
				switch(prevhit)
				{
				case ATTACK_HKICK1:
				case ATTACK_HKICK2:
					return TOUCHC_CANCEL;
				}
			}
			break;
		case ATTACK_SAH1	:
			{
				if(prevhit==ATTACK_SAH2)
				{
					return TOUCHC_CANCEL;
				}
			}
			break;
		case ATTACK_SAH2	:
			{
				if(prevhit==ATTACK_SAH1)
				{
					return TOUCHC_CANCEL;
				}
			}
			break;
		}
	}
	return TOUCHC_OK;
}

/*--------------------------------------------------------------------------------------
	GOBJMSG_ACTION ���b�Z�[�W ����
	�V�����s����ǉ�����ꍇ�́A�����ɏ�����ǉ���܂��B
	�s���̑O��ɋ��ʏ������ꂽ���ꍇ�� PreAction(),PostAction() ��I�[�o�[���C�h���Ă�������
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Action()
{
	switch(GetGObject()->aid){
	case ACTID_NAGE1		:act_nage();		break;
	case ACTID_SAH1			:act_sah1();		break;
	case ACTID_SAH2			:act_sah2();		break;
	case ACTID_SAH3			:act_sah3();		break;
	case ACTID_SAHS			:act_sahs();		break;
	case ACTID_FIG1			:act_fig1();		break;
	case ACTID_FIG2			:act_fig2();		break;
	case ACTID_FIG3			:act_fig3();		break;
	case ACTID_GCANCEL1		:act_gcancel1();	break;
	case ACTID_GCANCEL2		:act_gcancel2();	break;
	case ACTID_GCANCEL3		:act_gcancel3();	break;
	case ACTID_DASHF		:act_dashf();		break;
	case ACTID_DASHB		:act_dashb();		break;
	case ACTID_IKARI		:act_ikari();		break;
	case ACTID_ESCAPE		:act_escape();		break;
	case ACTID_STRIKER1		:act_striker1();	break;
	case ACTID_STRIKER2		:act_striker2();	break;
	case ACTID_STRIKER3		:act_striker3();	break;
	case ACTID_STRIKER4		:act_striker4();	break;
	case ACTID_BAT			:act_bat();			ChainCombo(CHAIN_BAT);	break;
	case ACTID_UPPER_STEP	:act_upper_step();	ChainCombo(CHAIN_UPPER);break;
	case ACTID_UPPER_JAMP	:act_upper_jamp();		break;
	case ACTID_UPPER_LANDING:act_upper_landing();	break;
	case ACTID_HAND			:act_hand();	ChainCombo(CHAIN_HAND);	break;
	case ACTID_HKICK		:act_hkick();	ChainCombo(CHAIN_HKICK);break;
	case ACTID_SAH_LANDING	:act_sah_landing();	break;
	default://����ȊO�̓L�����N�^�[��{�N���X�ɔC����
		return CCharacterBase::Action();
	}

	return(TRUE);
}

/*--------------------------------------------------------------------------------------
	�U���͏�񏉊�
----------------------------------------------------------------------------------------*/
void CCharacter::InitAttackInfo()
{
	ZeroMemory(aif,sizeof(aif));

	//�ݒ��

	int i;

	//�ES�U��

	i=ATTACKINFO_STAND_A;//������U��
	aif[i].hit		=HITINFO_MARK1 | HITINFO_REACT1 | HITINFO_SNDHIT1 | HITINFO_SIV1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_WEAK;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_STAND_B;//�������U��
	aif[i].hit		=HITINFO_MARK2 | HITINFO_REACT2 | HITINFO_SNDHIT2 | HITINFO_SIV2 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=40;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_STAND_C;//�������U��
	aif[i].hit		=HITINFO_MARK3 | HITINFO_REACT2 | HITINFO_SNDHIT3 | HITINFO_SIV2;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=0;//���

	//�EC�U��

	i=ATTACKINFO_CROUCH_A;//���Ⴊ�ݎ�U��
	aif[i].hit		=HITINFO_MARK1 | HITINFO_REACT1 | HITINFO_SNDHIT1 | HITINFO_SIV1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_WEAK;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_CROUCH_B;//���Ⴊ�ݒ��U��
	aif[i].hit		=HITINFO_MARK2 | HITINFO_REACT2 | HITINFO_SNDHIT2 | HITINFO_SIV2 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=40;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_CROUCH_C;//���Ⴊ�݋��U��
	aif[i].hit		=HITINFO_MARK3 | HITINFO_REACT3 | HITINFO_SNDHIT3 | HITINFO_SIV3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP | GUARDINFO_XSTAND;//�K�[�h���
	aif[i].id		=ATTACK_STRONG ;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=0;//���

	//�EJ�U��

	i=ATTACKINFO_JAMP_A;//�W�����v��U��
	aif[i].hit		=HITINFO_MARK1 | HITINFO_REACT1 | HITINFO_SNDHIT1 | HITINFO_SIV1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1 | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_WEAK;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_JAMP_B;//�W�����v���U��
	aif[i].hit		=HITINFO_MARK2 | HITINFO_REACT2 | HITINFO_SNDHIT2 | HITINFO_SIV2 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2 | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=40;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_JAMP_C;//�W�����v���U��
	aif[i].hit		=HITINFO_MARK3 | HITINFO_REACT3 | HITINFO_SNDHIT3 | HITINFO_SIV3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=0;//���
	
	//�E���̑�

	i=ATTACKINFO_STRIKER;//�x���U��
	aif[i].hit		=HITINFO_MARK4 | HITINFO_TATAKI2A | HITINFO_SNDHIT3 | HITINFO_SIV3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=70;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_NAGE;//����
	aif[i].hit		=0;//�q�b�g���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=60;//�_���[�W
	aif[i].muki		= TRUE;//����

	//�E�K�E

	i=ATTACKINFO_SAH1A;//�P���[��
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_STRONG | ATTACK_SAH1;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=3;//���
	
	i=ATTACKINFO_SAH1B;//�P���[��
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_STRONG | ATTACK_SAH1;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=3;//���
	
	i=ATTACKINFO_SAH1C;//�P���[��
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_STRONG | ATTACK_SAH1;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=3;//���

	i=ATTACKINFO_SAH2A;//�P���[��
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_STRONG | ATTACK_SAH2;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=3;//���
	
	i=ATTACKINFO_SAH2B;//�P���[��
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_STRONG | ATTACK_SAH2;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=3;//���
	
	i=ATTACKINFO_SAH2C;//�P���[��
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_STRONG | ATTACK_SAH2;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=3;//���

	i=ATTACKINFO_FIG;//�A�C�m�J�^�`
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV2 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=80;//�_���[�W
	aif[i].kezuri	=10;//���

	i=ATTACKINFO_IKARI1;//�{���Ԃ��铊��
	aif[i].hit		=HITINFO_MARK2 | HITINFO_SNDHIT2;//�q�b�g���
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=65;//�_���[�W

	i=ATTACKINFO_IKARI2;//�{���Ԃ��铊��
	aif[i].hit		=HITINFO_MARK3 | HITINFO_SNDHIT3;//�q�b�g���
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=65;//�_���[�W

	i=ATTACKINFO_ACCHI;//������I
	aif[i].hit		=HITINFO_MARK4 | HITINFO_REACT1 | HITINFO_SNDHIT3 | GUARDINFO_EFCTSINDO;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT1 ;//�K�[�h���
	aif[i].id		=ATTACK_FRIENDS;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=20;//�_���[�W
	aif[i].kezuri	=0;//���

	//�EEx Attacks

	i= ATTACKINFO_BAT;
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV2 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG | ATTACK_BAT;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=60;//�_���[�W
	aif[i].kezuri	=0;//���

	i= ATTACKINFO_UPPER;
	aif[i].hit		=HITINFO_MARK3 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV3 | GUARDINFO_XSTAND;//�K�[�h���
	aif[i].id		=ATTACK_STRONG | ATTACK_UPPER;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=60;//�_���[�W
	aif[i].kezuri	=0;//���

	i= ATTACKINFO_HAND;
	aif[i].hit		=HITINFO_MARK2 | HITINFO_REACT2 | HITINFO_SNDHIT3 | HITINFO_SIV1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV1 | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=20;//�_���[�W
	aif[i].kezuri	=0;//���

	i= ATTACKINFO_HKICK1;
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV2 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG | ATTACK_HKICK1;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=60;//�_���[�W
	aif[i].kezuri	=0;//���
	
	i= ATTACKINFO_HKICK2;
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV2 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG | ATTACK_HKICK2;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=60;//�_���[�W
	aif[i].kezuri	=0;//���

	//�E�K�[�h�L�����Z��

	i= ATTACKINFO_GCANCEL1;
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_STOP ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=0;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=60;//�_���[�W
	aif[i].kezuri	=0;//���
	
	i= ATTACKINFO_GCANCEL2;
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_STOP ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=0;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=80;//�_���[�W
	aif[i].kezuri	=0;//���

	i= ATTACKINFO_GCANCEL3;
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_STOP ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=0;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=100;//�_���[�W
	aif[i].kezuri	=0;//���
}

/*--------------------------------------------------------------------------------------
	�U���J�n�����i�ʏ�A����������K�v�͂���܂���j
----------------------------------------------------------------------------------------*/
void CCharacter::StartAttack(DWORD atk_info_id)
{
	GetGObject()->kougeki = TRUE;
	GetGObject()->atk = &aif[atk_info_id];
}

/*--------------------------------------------------------------------------------------
	COM�s���p �Z��� ����
----------------------------------------------------------------------------------------*/
void CCharacter::InitWazInfo()//�R���s���[�^�p�Z���̐ݒ�
{
	CCharacterBase::InitWazInfo();

	waz.dashf = ACTID_DASHF;
	waz.dashb = ACTID_DASHB;

	waz.att_short[0] = ACTID_FIG1;//�Z���[�`�U��
	waz.att_short[1] = ACTID_ATT_CA;
	waz.att_short[2] = ACTID_ATT_CB;
	waz.att_short[3] = ACTID_GCANCEL1;
	waz.att_middle[0] = ACTID_ATT_SB;//�����[�`�U��
	waz.att_middle[1] = ACTID_ATT_CC;
	waz.att_middle[2] = ACTID_FIG2;
	waz.att_middle[3] = ACTID_ESCAPE;
	waz.att_middle[4] = ACTID_UPPER_STEP;
	waz.att_long[0] = ACTID_ATT_SC;//�����[�`�U��
	waz.att_long[1] = ACTID_ATT_SB;
	waz.att_long[2] = ACTID_ATT_CC;
	waz.att_long[3] = ACTID_BAT;
	waz.att_long[4] = ACTID_HAND;
	waz.att_long[5] = ACTID_UPPER_STEP;

	waz.att_jamp[0] = ACTID_ATT_JC;//�W�����v�U��
	waz.att_jamp[1] = ACTID_ATT_JB;
	waz.att_jamp[2] = ACTID_ATT_JA;

	waz.att_tai[0] = ACTID_SAH3;//�΋�
	waz.att_tai[1] = ACTID_SAH2;
	waz.att_tai[2] = ACTID_HKICK;
	waz.att_tai[3] = ACTID_UPPER_STEP;
	
	waz.att_tai[1] = ACTID_FIG1;
	waz.nageid[0] = ACTID_NAGE1;//����
	waz.nageid[1] = ACTID_IKARI;//����
}


/*--------------------------------------------------------------------------------------
	COM���x������
----------------------------------------------------------------------------------------*/
BOOL CCharacter::ComLevelCk(UINT level)
{
	int crnt_level = GetComLevel();
	int v = rand()%100;
	int r;
	const int rat_base = 80;

	if((level==0) || crnt_level==level)
	{
		r = rat_base;
	}
	else if(crnt_level<(int)level)
	{
		r = rat_base*crnt_level/level;
	}
	else
	{
		r = rat_base + (100-rat_base)*(crnt_level-level)/7;
	}

	return (r>=v) ? TRUE : FALSE;
}