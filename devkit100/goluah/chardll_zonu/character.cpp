/*=====================================================================================

	�L�����N�^�[��`

=======================================================================================*/
#include "character.h"

//**************************************************************************************
//  Option Settings
//�@�����ɁA�L�����N�^�[�ɐݒ�ł���I�v�V������w�肵�܂�
//**************************************************************************************

CHARACTER_LOAD_OPTION option[] = {
	// �L�����N�^�[�̃I�v�V��������ɏ����Ă��������B�i�ő�32�j
	// �Ȃ��A�I�v�V�����ƃI�v�V�����̊Ԃ̓R���}(,)�ŋ�؂�Ă��������B

	// �L���e�F
	// �@{ �t���O, ���������߼��,�ˑ������߼��, �I�v�V������, ���|�C���g }
	{ OPTIONS_CHAIN_COMBO  ,0          , 0, "Chain Combo"  , 8 } ,
	{ OPTIONS_SUPER_ARMER	,0					, 0, "Super Armer"	, 10} ,
	{ OPTIONS_HP_UP			,0					, 0, "HP+20%"		, 8 } ,
	{ OPTIONS_HP_UP2		,0	, OPTIONS_HP_UP	, "HP+20%"			, 8 } ,
};


//**************************************************************************************
//  Character Information
//�@�����ɃL�����N�^�[�̖��O��L�q���܂�
//**************************************************************************************

CCharacterInfo CharInfo("����",				// �L�����N�^�[�̖��O�i�ő�31�o�C�g�j
						CDI_VERSION,		// �L�����N�^�[DLL�C���^�[�t�F�C�X�̃o�[�W����
						option,				// �I�v�V�����\���̂ւ̃|�C���^
						sizeof(option) / sizeof(CHARACTER_LOAD_OPTION), // �I�v�V�����̐�
						20,					// �I�v�V�����ő�|�C���g��
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
	chainComboEnabled	= (option_flags&OPTIONS_CHAIN_COMBO) ? TRUE : FALSE;
	opt_heavy_hit		= FALSE;
	opt_super_armer		= (option_flags&OPTIONS_SUPER_ARMER ) ? TRUE : FALSE;

	//�ő�̗͒l
	pdat->hpmax			= (option_flags&OPTIONS_HP_UP ) ? 1080 : 900 ;
	pdat->hpmax			= (option_flags&OPTIONS_HP_UP2) ? 1260 : pdat->hpmax;

	pdat->gaugemax		= 1;//�ő�Q�[�W
	pdat->gaugetype		= GOBJFLGA_POWGAUGE2;//�Q�[�W�^�C�v
}


/*--------------------------------------------------------------------------------------
	���b�Z�[�W����
	�x�[�X�N���X�ŏE��Ă��Ȃ����b�Z�[�W��E�������ꍇ�́A�R�R�ɏ�����ǉ���܂�
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Message(DWORD msg,LPVOID dat,DWORD prm)
{
	GOBJECT *pobjdat;

	switch(msg){
	case GOBJMSG_NAGE1://�����A�G������
		pobjdat = GetInfo(prm);
		if(pobjdat==NULL)return(0);
		return(act_nagee(pobjdat));
	case GOBJMSG_NAGE2://�K�����A�G������
		pobjdat = GetInfo(prm);
		if(pobjdat==NULL)return(0);
		return(act_hnagee(pobjdat));
	case GOBJMSG_NAGE3://�������A�G������
		pobjdat = GetInfo(prm);
		if(pobjdat==NULL)return(0);
		return(act_snagee(pobjdat));
	case GOBJMSG_STRIKER://�x���U���v��
		if(pdat->aid == ACTID_TAIKICYU)
		{
			pdat->objtype &= ~GOBJFLG_TOBETARGET;//�^�[�Q�b�g�ɂ͂Ȃ�Ȃ�
			ChangeAction( ACTID_STRIKER1 );
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
BOOL CCharacter::Command_Hissatu(DWORD key_now)
{
	//�K�E�Z�R�}���h����
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_KUCYU){//�󒆕K�E�Z�̃R�}���h����
	}
	else//�n��K�E�Z�̃R�}���h����
	{
		if(com624624(60) && pdat->gauge>=1.0){
			if(key_now & KEYSTA_BC2){
				if(NageHantei(MAAI_SNAGE)){
					if(ObjCatch(pdat->eid,GOBJMSG_NAGE3)){pdat->aid = ACTID_SNAGE;return TRUE;}
				}
			}
		}
		if(com214(30)){
			if(key_now & KEYSTA_BC2){pdat->aid = ACTID_TETU3;return TRUE;}
			else if(key_now & KEYSTA_BB2){pdat->aid = ACTID_TETU2;return TRUE;}
			else if(key_now & KEYSTA_BA2){pdat->aid = ACTID_TETU1;return TRUE;}
		}
		if(com426(45)){
			if(key_now & KEYSTA_BC2){
				if(NageHantei(MAAI_HNAGE)){
					if(ObjCatch(pdat->eid,GOBJMSG_NAGE2)){pdat->aid = ACTID_HNAGE;return TRUE;}
				}
			}
		}
	}

	return CCharacterBase::Command_Hissatu(key_now);
}

BOOL CCharacter::Command_Normal(DWORD key)
{
	//�_�b�V���R�}���h
	if(com66i(30)){pdat->aid=ACTID_DASHF;return TRUE;}
	if(com44i(30)){pdat->aid=ACTID_DASHB;return TRUE;}

	//return CCharacterBase::Command_Normal(key);

	//��{�ʏ퓮��
	if(key & KEYSTA_UP){
		pdat->aid = ACTID_JAMPS;
	}
	else if(key & KEYSTA_DOWN){//���Ⴊ��
		pdat->aid = ACTID_CROUCH;
	}
	else if(key & KEYSTA_FOWORD){//�O��
		if(key & KEYSTA_FOWORD && pdat->aid==ACTID_DASHF)return TRUE;//�_�b�V���p��
		pdat->aid = ACTID_WALKF;
	}
	else if(key & KEYSTA_BACK){//����
		if(key & KEYSTA_BACK && pdat->aid==ACTID_DASHB)return TRUE;//�_�b�V���p��
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
			NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE1)) {
				pdat->aid = ACTID_NAGE1;
				return TRUE;
		}
	}

	return CCharacterBase::Command_OnNormal(keyinfo);
}

/*--------------------------------------------------------------------------------------
	���_���^�����_���^��
----------------------------------------------------------------------------------------*/
BOOL CCharacter::com624624(int dt)
{
	int ofst=0;

	ofst=SeekKey(ofst,dt,KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=SeekKey(ofst,dt,KEYSTA_DOWN | KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=SeekKey(ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=SeekKey(ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=SeekKey(ofst,dt,KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=SeekKey(ofst,dt,KEYSTA_DOWN | KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=SeekKey(ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=SeekKey(ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

/*--------------------------------------------------------------------------------------
	�R�}���h���� ����(COM)
----------------------------------------------------------------------------------------*/

DWORD CCharacter::CommandCOM(DWORD wid)
{
	return CCharacterBase::CommandCOM(wid);
}

DWORD CCharacter::CmdCom_OnSystem(DWORD wid)
{
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
		
			if(NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE1)) {
				pdat->aid = ACTID_NAGE1;
				return TRUE;
			}
			else return FALSE;
		}
	}
	//�K�E����
	if(wid==ACTID_HNAGE)
	{
		if(!(pdat->aid&ACTID_KUCYU) && !(pdat->aid&ACTID_ATTACK))
		{
			if(NageHantei(MAAI_HNAGE))
			if(ObjCatch(pdat->eid, GOBJMSG_NAGE2)) {
				pdat->aid = ACTID_HNAGE;
				return TRUE;
			}
		}
		return FALSE;
	}
	if(wid==ACTID_SNAGE)
	{
		if(!(pdat->aid&ACTID_KUCYU) && !(pdat->aid&ACTID_ATTACK) && pdat->gauge>=1.0)
		{
			if(NageHantei(MAAI_SNAGE))
			if(ObjCatch(pdat->eid, GOBJMSG_NAGE3)) {
				pdat->aid = ACTID_SNAGE;
				return TRUE;
			}
		}
		return FALSE;
	}

	return CCharacterBase::CmdCom_OnNormal(wid);
}

/*--------------------------------------------------------------------------------------
	GOBJMSG_TOUCHB(�U��������)
----------------------------------------------------------------------------------------*/
DWORD CCharacter::TouchB(ATTACKINFO *info,BOOL hit)
{
	double dp = 0.0f;
	switch(info->id){
		case ATTACK_WEAK	:dp=0.02;break;
		case ATTACK_MIDDLE	:dp=0.03;break;
		case ATTACK_STRONG	:dp=0.04;break;
		case ATTACK_HISSATU	:dp=0.05;break;
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
		case ACTID_ATT_SB	:if(pdat->counter<15)sa=TRUE;	break;
		case ACTID_ATT_SC	:if(pdat->counter<25)sa=TRUE;	break;
		case ACTID_ATT_CA	:if(pdat->counter<3)sa=TRUE;	break;
		case ACTID_ATT_CB	:if(pdat->counter<15)sa=TRUE;	break;
		case ACTID_ATT_CC	:if(pdat->counter<15)sa=TRUE;	break;
		case ACTID_ATT_JA	:if(pdat->counter<2)sa=TRUE;	break;
		case ACTID_ATT_JB	:if(pdat->counter<7)sa=TRUE;	break;
		case ACTID_ATT_JC	:if(pdat->counter<10)sa=TRUE;	break;

		case ACTID_TETU1	:
		case ACTID_TETU2	:
		case ACTID_TETU3	:if(pdat->counter<12)sa=TRUE;	break;
		case ACTID_DASHF	:sa=TRUE;						break;
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
	GOBJMSG_ACTION ���b�Z�[�W ����
	�V�����s����ǉ�����ꍇ�́A�����ɏ�����ǉ���܂��B
	�s���̑O��ɋ��ʏ������ꂽ���ꍇ�� PreAction(),PostAction() ��I�[�o�[���C�h���Ă�������
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Action()
{
	switch(GetGObject()->aid){
	case ACTID_NAGE1		:act_nage();	break;
	case ACTID_TETU1		:act_tetu1();	break;
	case ACTID_TETU2		:act_tetu2();	break;
	case ACTID_TETU3		:act_tetu3();	break;
	case ACTID_HNAGE		:act_hnage();	break;
	case ACTID_SNAGE		:act_snage();	break;
	case ACTID_DASHF		:act_dashf();	break;
	case ACTID_DASHB		:act_dashb();	break;
	case ACTID_STRIKER1		:act_striker1();break;
	case ACTID_STRIKER2		:act_striker2();break;
	case ACTID_STRIKER3		:act_striker3();break;
	case ACTID_STRIKER4		:act_striker4();break;
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

	int i;

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
	aif[i].hit		=HITINFO_MARK3 | HITINFO_REACT1 | HITINFO_SNDHIT3 | HITINFO_SIV1;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=20;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_CROUCH_A;//���Ⴊ�ݎ�U��
	aif[i].hit		=HITINFO_MARK1 | HITINFO_REACT1 | HITINFO_SNDHIT1 | HITINFO_SIV1;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_WEAK;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_CROUCH_B;//���Ⴊ�ݒ��U��
	aif[i].hit		=HITINFO_MARK2 | HITINFO_REACT2 | HITINFO_SNDHIT2 | HITINFO_SIV2;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=45;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_CROUCH_C;//���Ⴊ�݋��U��
	aif[i].hit		=HITINFO_MARK3 | HITINFO_REACT3 | HITINFO_SNDHIT3 | HITINFO_SIV3;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP | GUARDINFO_XSTAND;//�K�[�h���
	aif[i].id		=ATTACK_STRONG ;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_JAMP_A;//�W�����v��U��
	aif[i].hit		=HITINFO_MARK1 | HITINFO_REACT1 | HITINFO_SNDHIT1 | HITINFO_SIV1;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1 | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_WEAK;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_JAMP_B;//�W�����v���U��
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2  | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=40;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_JAMP_C;//�W�����v���U��
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_NAGE;//����
	aif[i].hit		=HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=80;//�_���[�W

	i=ATTACKINFO_HNAGE1;//�K�E����(1)
	aif[i].hit		=HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=60;//�_���[�W

	i=ATTACKINFO_HNAGE2;//�K�E����(2)
	aif[i].hit		=HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=80;//�_���[�W

	//�̓�����

	i=ATTACKINFO_CHARGE1;
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=100;//�_���[�W
	aif[i].kezuri	=10;//���
	
	i=ATTACKINFO_CHARGE2;
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=120;//�_���[�W
	aif[i].kezuri	=12;//���
	
	i=ATTACKINFO_CHARGE3;
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=140;//�_���[�W
	aif[i].kezuri	=14;//���

	//������
	
	i=ATTACKINFO_SNAGE1;//������(1)
	aif[i].hit		=HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].id		=0;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=60;//�_���[�W

	i=ATTACKINFO_SNAGE2;//������(2)
	aif[i].hit		=HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].id		=0;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=240;//�_���[�W

	if(opt_heavy_hit)
	{
		for(int i=0;i<AIF_NUM;i++)
		{
			aif[i].damage = (DWORD)(aif[i].damage*1.2);
			aif[i].kezuri*= 2;
		}
	}
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

	waz.walkf = ACTID_WALKF;
	waz.walkb = ACTID_WALKB;
	waz.jampf = ACTID_JAMPF;
	waz.jampb = ACTID_JAMPB;

	waz.dashf = ACTID_DASHF;
	waz.dashb = ACTID_DASHB;

	waz.att_short[0] = ACTID_ATT_SA;//�Z���[�`�U��
	waz.att_short[1] = ACTID_ATT_CA;
	waz.att_short[2] = ACTID_ATT_CB;
	waz.att_short[3] = ACTID_TETU1;
	waz.att_middle[0] = ACTID_ATT_SB;//�����[�`�U��
	waz.att_middle[1] = ACTID_ATT_CC;
	waz.att_middle[2] = ACTID_TETU3;
	waz.att_long[0] = ACTID_ATT_SC;//�����[�`�U��
	waz.att_jamp[0] = ACTID_ATT_JC;//�W�����v�U��
	waz.att_jamp[1] = ACTID_ATT_JB;
	waz.att_jamp[2] = ACTID_ATT_JA;
	waz.att_tai[0] = ACTID_ATT_CC;
	waz.nageid[0] = ACTID_NAGE1;//����
	waz.nageid[1] = ACTID_HNAGE;
	waz.nageid[2] = ACTID_SNAGE;
}
