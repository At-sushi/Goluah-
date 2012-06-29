/*=====================================================================================

	�L�����N�^�[��`

=======================================================================================*/
#include "character.h"

//**************************************************************************************
//  Option Settings
//�@�����ɁA�L�����N�^�[�ɐݒ�ł���I�v�V������w�肵�܂�
//**************************************************************************************

CHARACTER_LOAD_OPTION option[] = {
	// �L���e�F
	// �@{ �t���O, ���������߼��,�ˑ������߼��, �I�v�V������, ���|�C���g }
	{ OPTIONS_CHAIN_COMBO	, 0, 0,						"Chain Combo"	, /*10*/5 },//�`�F�C���E�R���{
	{ OPTIONS_EXTRA_ATTACK	, 0, 0,						"Extra Attack"	, 5 },//�ǉ��͋Z
	{ OPTIONS_AERIAL_ATTACK	, 0, 0,						"Aerial Attack"	, 5 },//�G���A���E�A�^�b�N
	{ OPTIONS_AERIAL_STEP	, 0, OPTIONS_AERIAL_ATTACK,	"Aerial Step"	, 5 },//2�i�W�����v
	{ OPTIONS_AUTO_GUARD	, 0, 0,						"Auto Guard"	, /*10*/5 },//�I�[�g�K�[�h
	{ OPTIONS_GUARD_CANCEL	, 0, OPTIONS_AUTO_GUARD,	"Guard Cancel"	, 5 },//�K�[�h�L�����Z��
	{ OPTIONS_HADOUKEN		, 0, 0,						"Hadou-Ken"		, 5 },//�g����
};


//**************************************************************************************
//  Character Information
//�@�����ɃL�����N�^�[�̖��O��L�q���܂�
//**************************************************************************************

CCharacterInfo CharInfo("���i�[",		// �L�����N�^�[�̖��O�i�ő�31�o�C�g�j
						CDI_VERSION,		// �L�����N�^�[DLL�C���^�[�t�F�C�X�̃o�[�W����
						option,				// �I�v�V�����\���̂ւ̃|�C���^
						sizeof(option) / sizeof(CHARACTER_LOAD_OPTION), // �I�v�V�����̐�
						20,					// �I�v�V�����ő�|�C���g��
						FALSE);				// �l�b�g�ΐ�Ή����ǂ��� �����̊g���̂��߂ɗp�ӂ���Ă���B

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
//	aki3d.Initialize( GetD3DDevice(), CharDirectory);
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
	InitAnalyzeOptions();

	pdat->hpmax			= m_opt_hpp30 ? 1040 : 800;//�ő�̗͒l
	pdat->gaugemax		= 1;//�ő�Q�[�W
	pdat->gaugetype		= GOBJFLGA_POWGAUGE2;//�Q�[�W�^�C�v
}

//DWORD �I�v�V�����w��� BOOL�����o�ϐ��ɕ�������
void CCharacter::InitAnalyzeOptions()
{
	//�`�F�[���R���{�EON/OFF
	if(option_flags&OPTIONS_CHAIN_COMBO)	chainComboEnabled = TRUE;
	else									chainComboEnabled = FALSE;

	m_opt_exAttack	= (option_flags&OPTIONS_EXTRA_ATTACK) ? TRUE : FALSE;
	m_opt_hadou		= (option_flags&OPTIONS_HADOUKEN) ? TRUE : FALSE;
	m_opt_hpp30		= FALSE;

	m_opt_AAttack	= (option_flags&OPTIONS_AERIAL_ATTACK) ? TRUE : FALSE;
	m_opt_AStep		= (option_flags&OPTIONS_AERIAL_STEP) ? TRUE : FALSE;
	isAutoGuard		= (option_flags&OPTIONS_AUTO_GUARD) ? TRUE : FALSE;
	m_opt_gcancel	= (option_flags&OPTIONS_GUARD_CANCEL) ? TRUE : FALSE;
}

/*--------------------------------------------------------------------------------------
	���b�Z�[�W����
	�x�[�X�N���X�ŏE��Ă��Ȃ����b�Z�[�W��E�������ꍇ�́A�R�R�ɏ�����ǉ���܂�
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Message(DWORD msg,LPVOID dat,DWORD prm)
{
	GOBJECT* pobjdat;

	//���[�U�[��`�q�b�g
	if((msg&USERHIT_FLAG) && (msg&GOBJMSG_USERDEFINE))
	{
		return EnemyAction(prm,msg&(~GOBJMSG_USERDEFINE));
	}

	switch(msg){
	case GOBJMSG_NAGE://�����A�G������
		{
			pobjdat = GetInfo(prm);
			if(pobjdat==NULL)return(0);
			return(eact_nage(pobjdat));
		}
		break;
	case GOBJMSG_HADOUHIT:		AddPowerGauge(0.10);break;
	case GOBJMSG_HADOUGUARD:	AddPowerGauge(0.05);break;
	case GOBJMSG_STRIKER://�x���U���v��
		if(pdat->aid == ACTID_TAIKICYU){
			RemoveProperty( GOBJFLG_TOBETARGET );//�^�[�Q�b�g�ɂ͂Ȃ�Ȃ�...�Ȃ�ł���O���񂾂���H(�Y)
			ChangeAction( ACTID_STRIKER_IN );
			return(ALL_RIGHT);
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

//�K�E�Z��
BOOL CCharacter::Command_OnHissatuAttacking(DWORD key_now)
{
	//���C�N�A�ǉ��̓R�}���h����
	if(pdat->aid==ACTID_REIKU1A){
		if(com214(30)){
			if(key_now & 0x22220000){
				pdat->aid=ACTID_REIKU1B;
				return TRUE;
			}
		}
	}
	else if(pdat->aid==ACTID_REIKU1B){
		if(com214(30)){
			if(key_now & 0x22220000){
				pdat->aid=ACTID_REIKU1C;
				return TRUE;
			}
		}
	}
	else if(pdat->aid==ACTID_REIKU2A){
		if(com214(30)){
			if(key_now & 0x22220000){
				pdat->aid=ACTID_REIKU2B;
				return TRUE;
			}
		}
	}
	else if(pdat->aid==ACTID_REIKU2B){
		if(com214(30)){
			if(key_now & 0x22220000){
				pdat->aid=ACTID_REIKU2C;
				return TRUE;
			}
		}
	}
	else if(pdat->aid==ACTID_REIKU3A){
		if(com214(30)){
			if(key_now & 0x22220000){
				pdat->aid=ACTID_REIKU3B;
				return TRUE;
			}
		}
	}
	else if(pdat->aid==ACTID_REIKU3B){
		if(com214(30)){
			if(key_now & 0x22220000){
				pdat->aid=ACTID_REIKU3C;
				return TRUE;
			}
		}
	}

	return CCharacterBase::Command_OnHissatuAttacking(key_now);
}

//�K�E�Z�R�}���h����
BOOL CCharacter::Command_Hissatu(DWORD key)
{
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_KUCYU){//�󒆕K�E�Z�̃R�}���h����
	}
	else//�n��K�E�Z�̃R�}���h����
	{
		//�O�b�o�C�t�H�[�G�o�[
		if(com236236(40) && (key & KEYSTA_BC2) && pdat->gauge>=1.0f)
		{
			ChangeAction(ACTID_GOODBYE1);
			return TRUE;
		}
		//������r�݂����Ȃ��
		if(com623(30)){
			if(key & KEYSTA_BC2){pdat->aid = ACTID_TATUMAKI3;return TRUE;}
			else if(key & KEYSTA_BB2){pdat->aid = ACTID_TATUMAKI2;return TRUE;}
			else if(key & KEYSTA_BA2){pdat->aid = ACTID_TATUMAKI1;return TRUE;}
		}
		//���C�N
		if(TRUE){//OptReiku()){
			if(com214(30)){
				if(key & KEYSTA_BC2){pdat->aid = ACTID_REIKU3A;return TRUE;}
				else if(key & KEYSTA_BB2){pdat->aid = ACTID_REIKU2A;return TRUE;}
				else if(key & KEYSTA_BA2){pdat->aid = ACTID_REIKU1A;return TRUE;}
			}
		}
		if(m_opt_hadou)
		{
			//�g�����i��ԁj
			if(pbul_hadou->isReady() && com236(30))
			{
				if(key & KEYSTA_BC2){pdat->aid = ACTID_HADOU3B;return TRUE;}
				else if(key & KEYSTA_BB2){pdat->aid = ACTID_HADOU2B;return TRUE;}
				else if(key & KEYSTA_BA2){pdat->aid = ACTID_HADOU1B;return TRUE;}
			}
		}
		else
		{
			//�g�����i��΂Ȃ��j
			if(pbul_hadou->isReady() && com236(30)){
				if(key & KEYSTA_BC2){pdat->aid = ACTID_HADOU3A;return TRUE;}
				else if(key & KEYSTA_BB2){pdat->aid = ACTID_HADOU2A;return TRUE;}
				else if(key & KEYSTA_BA2){pdat->aid = ACTID_HADOU1A;return TRUE;}
			}
		}
	}

	return CCharacterBase::Command_Hissatu(key);
}

BOOL CCharacter::Command_Normal(DWORD key)
{
	//�_�b�V���R�}���h
	if(com66i(30)){pdat->aid=ACTID_DASHF;return TRUE;}
	if(com44i(30)){pdat->aid=ACTID_DASHB;return TRUE;}

	//return CCharacterBase::Command_Normal(key);

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

BOOL CCharacter::Command_OnNormal(DWORD key)
{
	if(pdat->aid & ACTID_KUCYU)////�W�����v���쒆
	{
		//2�i�W�����v
		if((key & KEYSTA_UP2) && m_2nd_step_flag)
		{
			pdat->aid = ACTID_2ND_STEP;
			return TRUE;
		}
		//2�i�W�����v����
		if(pdat->aid == ACTID_2ND_STEP){
			if(key & KEYSTA_FOWORD){//�O�W�����v
				pdat->aid = ACTID_JAMPF;
				return TRUE;
			}
			else if(key & KEYSTA_BACK){//��W�����v
				pdat->aid = ACTID_JAMPB;
				return TRUE;
			}
		}
	}
	else//�n�㓮�쒆
	{
		//����
		if (key & KEYSTA_FOWORD && key & KEYSTA_BC2 &&
			NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE)) {
				pdat->aid = ACTID_NAGE1;
				return TRUE;
		}

		if(m_opt_exAttack)
		{
			//���o�[�O��ꋭ
			if((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD)){
				ChangeAction(  ACTID_ATT_SE );return TRUE;
			}

			//���o�[�O��ꒆ
			if((key & KEYSTA_BB2) && (key & KEYSTA_FOWORD)){
				ChangeAction(  ACTID_ATT_SD );return TRUE;
			}
		}
	}

	return CCharacterBase::Command_OnNormal(key);
}

BOOL CCharacter::Command_OnAttacking(DWORD key)
{	
	if( pdat->aid & ACTID_KUCYU )
	{
		if(m_opt_AAttack)
		{
			//�󒆃`�F�[��
			if(key & KEYSTA_BC2){if(ChainCombo(CHAIN_JC)){pdat->aid = ACTID_ATT_JC;return TRUE;}}
			else if(key & KEYSTA_BB2){if(ChainCombo(CHAIN_JB)){pdat->aid = ACTID_ATT_JB;return TRUE;}}
			else if(key & KEYSTA_BA2){if(ChainCombo(CHAIN_JA)){pdat->aid = ACTID_ATT_JA;return TRUE;}}

			//2�i�W�����v
			if((key & KEYSTA_UP2) && m_2nd_step_flag)
			{
				pdat->aid = ACTID_2ND_STEP;
				return TRUE;
			}
		}
	}
	else
	{
		if(m_opt_exAttack)
		{
			//���o�[�O��ꋭ
			if((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD)){
				if(ChainCombo(CHAIN_SE)){
					ChangeAction(  ACTID_ATT_SE );return TRUE;
				}
			}

			//���o�[�O��ꒆ
			if((key & KEYSTA_BB2) && (key & KEYSTA_FOWORD)){
				if(ChainCombo(CHAIN_SD)){
					ChangeAction(  ACTID_ATT_SD );return TRUE;
				}
			}
		}

		//���L�����Z��
		if((pdat->aid==ACTID_ATT_CC) && m_opt_AAttack){
			if(key & KEYSTA_UP){
				pdat->aid = ACTID_AJAMP;
				return TRUE;
			}
		}
	}

	return CCharacterBase::Command_OnAttacking(key);
}


//��炢�E�K�[�h��
BOOL CCharacter::Command_OnSystem(DWORD key)
{
	if((pdat->aid & ACTID_GUARD) && m_opt_gcancel){
		if(!(pdat->aid & ACTID_KUCYU))
		{
			//�n��K�[�h��
			//�K�[�h�L�����Z�� ������r
			if(com623(30) && pdat->gauge>NEEDGAUGE_GC){
				if(key & KEYSTA_BC2){pdat->aid = ACTID_GCANCEL3;return TRUE;}
				else if(key & KEYSTA_BB2){pdat->aid = ACTID_GCANCEL2;return TRUE;}
				else if(key & KEYSTA_BA2){pdat->aid = ACTID_GCANCEL1;return TRUE;}
			}
		}
	}

	return CCharacterBase::Command_OnSystem(key);
}

/*--------------------------------------------------------------------------------------
	�R�}���h���� ����(COM)
----------------------------------------------------------------------------------------*/

DWORD CCharacter::CommandCOM(DWORD wid)
{
	if(wid==ACTID_GOODBYE1 && pdat->gauge<1.0f)return FALSE;//�Q�[�W�`�F�b�N

	if(wid==ACTID_GCANCEL1 || wid==ACTID_GCANCEL2 || wid==ACTID_GCANCEL3)
	{
		if(pdat->gauge<NEEDGAUGE_GC)return FALSE;
		if(!((pdat->aid&ACTID_GUARD) && m_opt_gcancel))return FALSE;
	}

	//���C�N
	if(wid==ACTID_REIKU1B)return (pdat->aid==ACTID_REIKU1A) ? TRUE : FALSE;
	if(wid==ACTID_REIKU1C)return (pdat->aid==ACTID_REIKU1B) ? TRUE : FALSE;
	if(wid==ACTID_REIKU2B)return (pdat->aid==ACTID_REIKU2A) ? TRUE : FALSE;
	if(wid==ACTID_REIKU2C)return (pdat->aid==ACTID_REIKU2B) ? TRUE : FALSE;
	if(wid==ACTID_REIKU3B)return (pdat->aid==ACTID_REIKU3A) ? TRUE : FALSE;
	if(wid==ACTID_REIKU3C)return (pdat->aid==ACTID_REIKU3B) ? TRUE : FALSE;

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
		return TRUE;
	}

	return CCharacterBase::CmdCom_OnSystem(wid);
}

DWORD CCharacter::CmdCom_OnNormal(DWORD wid)
{
	//����
	if(!(pdat->aid&ACTID_KUCYU) && !(pdat->aid&ACTID_ATTACK))
	{
		if (wid==ACTID_NAGE1)
		{
			if(NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE)) {
				pdat->aid = ACTID_NAGE1;
				return TRUE;
			}
			else return FALSE;
		}
	}

	// �G���A���W�����v
	if (wid == ACTID_AJAMP)
	{
		if (pdat->aid == ACTID_ATT_CC && m_opt_AAttack)
			return TRUE;
		else
			return FALSE;
	}

	if(!(pdat->aid&ACTID_KUCYU) && (m_opt_exAttack))
	{
		//�`�F�[���R���{����(�ǉ�)
		if(GetGObject()->aid & ACTID_ATTACK)
		{
			switch(wid)
			{
			case ACTID_ATT_SE	:return ChainCombo(CHAIN_SE);break;
			case ACTID_ATT_SD	:return ChainCombo(CHAIN_SD);break;
			}
		}
	}

	return CCharacterBase::CmdCom_OnNormal(wid);
}

/*--------------------------------------------------------------------------------------
	GOBJMSG_TOUCHA ���b�Z�[�W ����
	�K�[�h�|�C���g�ǉ�
----------------------------------------------------------------------------------------*/
DWORD CCharacter::TouchA(ATTACKINFO *info,DWORD ta_eid)
{
	DWORD aid = GetGObject()->aid;
	DWORD cntr = GetGObject()->counter;

	if( CANNOT_GUARD(info->guard) ){//�K�[�h�s��
		return(CCharacterBase::TouchA(info,ta_eid));
	}

	//������r
	if((aid==ACTID_TATUMAKI1) && (cntr<10))return TOUCHA_MUSI2;
	if((aid==ACTID_TATUMAKI2) && (cntr<20))return TOUCHA_MUSI2;
	if((aid==ACTID_TATUMAKI3) && (cntr<30))return TOUCHA_MUSI2;

	//���o�[���U��
	if(aid==ACTID_ATT_SD){
		if(pdat->cnow==CELL_SD2 || pdat->cnow==CELL_SD3)return TOUCHA_MUSI2;
	}
	if(aid==ACTID_ATT_SE){
		if(pdat->cnow==CELL_SE3)return TOUCHA_MUSI2;
	}

	return(CCharacterBase::TouchA(info,ta_eid));
}


/*--------------------------------------------------------------------------------------
	GOBJMSG_TOUCHB ���b�Z�[�W ����
	�Q�[�W�������
----------------------------------------------------------------------------------------*/
DWORD CCharacter::TouchB(ATTACKINFO *info,BOOL hit)
{
	//���s���J��
	if(info->id == ATTACK_GOODBYE && hit){
		ChangeAction(ACTID_GOODBYE2);
		cell(CELL_GOODBYE3);
	}
	if(info->id == ATTACK_STRIKER){
		ChangeAction(ACTID_STRIKER_ATTACK);
	}

	double dp = 0.0f;
	switch(info->id){
		case ATTACK_WEAK	:dp=0.02;break;
		case ATTACK_MIDDLE	:dp=0.03;break;
		case ATTACK_STRONG	:dp=0.04;break;
		case ATTACK_HISSATU	:dp=0.05;break;
	}
	if(!hit)dp*=0.2;
	AddPowerGauge(dp);

	if(IsLocalCom())
	{
		switch(pdat->aid)
		{
		case ACTID_ATT_SA:
			{
				if(ComLevelCk(4)){
					SetComAct(ACTID_ATT_SB,3);
				}
			}
			break;
		case ACTID_ATT_SB:{
				if(ComLevelCk(5) && rand()%2){
					if(ChainCheck(CHAIN_SD))SetComAct(ACTID_ATT_SD,5);
					else if(ChainCheck(CHAIN_SE))SetComAct(ACTID_ATT_SE,5);
					else SetComAct(ACTID_REIKU1C,5);
				}
			}
			break;
		case ACTID_ATT_SC:
			{
				if(ComLevelCk(5) && pdat->gauge>=1.0f)
				{
					SetComAct(ACTID_GOODBYE1,10);
				}
				else if(ComLevelCk(4)){
					if(ChainCheck(CHAIN_SD))SetComAct(ACTID_ATT_SD,8);
					else if(ChainCheck(CHAIN_SE))SetComAct(ACTID_ATT_SE,8);
					else SetComAct(ACTID_REIKU1C,5);
				}
			}
			break;

		case ACTID_ATT_SD:
			{
				if(ComLevelCk(5) && pdat->gauge>=1.0f)
				{
					SetComAct(ACTID_GOODBYE1,10);
				}
				else if(ComLevelCk(3)){
					if(ChainCheck(CHAIN_SE))SetComAct(ACTID_ATT_SE,8);
					else SetComAct(ACTID_REIKU1C,8);
				}
			}
			break;
		case ACTID_ATT_SE:
			{
				if(ComLevelCk(5) && pdat->gauge>=1.0f)
				{
					SetComAct(ACTID_GOODBYE1,10);
				}
				else if(ComLevelCk(3)){
					if(ChainCheck(CHAIN_SD))SetComAct(ACTID_ATT_SD,8);
					else SetComAct(ACTID_REIKU1C,8);
				}
			}
			break;

		case ACTID_ATT_CA:
			{
				if(ComLevelCk(3)){
					SetComAct(ACTID_ATT_SB,5);
				}
			}
			break;
		case ACTID_ATT_CB:
			{
				if(ComLevelCk(4)){
					if(ChainCheck(CHAIN_SD))SetComAct(ACTID_ATT_SD,3);
					else if(ChainCheck(CHAIN_SE))SetComAct(ACTID_ATT_SE,3);
					else SetComAct(ACTID_REIKU1C,5);
				}
			}
			break;
		case ACTID_ATT_CC:
			{
				if (m_opt_AAttack && ComLevelCk(3))
					SetComAct(ACTID_AJAMP, 3);
			}
			break;
		//���C�N
		case ACTID_REIKU1A:
			{
				if(ComLevelCk(1)){
					SetComAct(ACTID_REIKU1B,16);
				}
			}
			break;
		case ACTID_REIKU1B:
			{
				if(ComLevelCk(1)){
					SetComAct(ACTID_REIKU1C,16);
				}
			}
			break;
		case ACTID_REIKU2A:
			{
				if(ComLevelCk(1)){
					SetComAct(ACTID_REIKU2B,16);
				}
			}
			break;
		case ACTID_REIKU2B:
			{
				if(ComLevelCk(1)){
					SetComAct(ACTID_REIKU2C,16);
				}
			}
			break;
		case ACTID_REIKU3A:
			{
				if(ComLevelCk(1)){
					SetComAct(ACTID_REIKU3B,16);
				}
			}
			break;
		case ACTID_REIKU3B:
			{
				if(ComLevelCk(1)){
					SetComAct(ACTID_REIKU3C,16);
				}
			}
			break;
		}
	}

	return CCharacterBase::TouchB(info,hit);
}


/*--------------------------------------------------------------------------------------
	GOBJMSG_TOUCHC ���b�Z�[�W ����
----------------------------------------------------------------------------------------*/
DWORD CCharacter::TouchC(ATTACKINFO *info,DWORD tc_eid)
{
	GOBJECT *pedat = GetInfo(tc_eid);
	if(!pedat)return CCharacterBase::TouchC(info,tc_eid);

	//���@�֎~
	if(info==&aif[ATTACKINFO_CROUCH_C])
	{
		if(pedat->aid==ACTID_FUTTOBI2)
		{
			return TOUCHC_CANCEL;
		}
	}
	//�G���A�����n��U�� �֎~
	if(pedat->aid==ACTID_USERHIT2)
	{
		if(!(pdat->aid&ACTID_KUCYU))
		{
			return TOUCHC_CANCEL;
		}
	}

	return CCharacterBase::TouchC(info,tc_eid);
}

/*--------------------------------------------------------------------------------------
	GOBJMSG_ACTION ���b�Z�[�W ����
	�V�����s����ǉ�����ꍇ�́A�����ɏ�����ǉ���܂��B
	�s���̑O��ɋ��ʏ������ꂽ���ꍇ�� PreAction(),PostAction() ��I�[�o�[���C�h���Ă�������
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Action()
{
	switch(GetGObject()->aid){
	case ACTID_ATT_SD		:act_att_sd();ChainCombo(CHAIN_SD);	break;
	case ACTID_ATT_SE		:act_att_se();ChainCombo(CHAIN_SE);	break;
	case ACTID_TATUMAKI1	:act_tatumaki1(ATTACKINFO_TATUMAKI1);	break;//����(��)
	case ACTID_TATUMAKI2	:act_tatumaki2(ATTACKINFO_TATUMAKI2);	break;//����(��)
	case ACTID_TATUMAKI3	:act_tatumaki3(ATTACKINFO_TATUMAKI3);	break;//����(��)
	case ACTID_TATUMAKIR	:act_tatumakir();		break;//��������
	case ACTID_LANDING		:act_landing();			break;//�������n
	case ACTID_REIKU1A		:act_reiku1a();			break;//���C�N(��)
	case ACTID_REIKU1B		:act_reiku1b();			break;
	case ACTID_REIKU1C		:act_reiku1c();			break;
	case ACTID_REIKU2A		:act_reiku2a();			break;//���C�N(��)
	case ACTID_REIKU2B		:act_reiku2b();			break;
	case ACTID_REIKU2C		:act_reiku2c();			break;
	case ACTID_REIKU3A		:act_reiku3a();			break;//���C�N(��)
	case ACTID_REIKU3B		:act_reiku3b();			break;
	case ACTID_REIKU3C		:act_reiku3c();			break;
	case ACTID_REIKU3CS		:act_reiku3cs();		break;//���C�N(��)
//	case ACTID_REIKUCOM		:act_reikucom();		break;//���C�N(COM)
	case ACTID_DASHF		:act_dashf();			break;//�O��_�b�V��
	case ACTID_DASHB		:act_dashb();			break;//���_�b�V��
	case ACTID_NAGE1		:act_nage();			break;//����
	case ACTID_HADOU1A		:act_hadou1a();			break;//�g�����i��΂Ȃ��A��j
	case ACTID_HADOU2A		:act_hadou2a();			break;//�g�����i��΂Ȃ��A�~�j
	case ACTID_HADOU3A		:act_hadou3a();			break;//�g�����i��΂Ȃ��A���j
	case ACTID_HADOU1B		:act_hadou1b();			break;//�g�����i��ԁA��j
	case ACTID_HADOU2B		:act_hadou2b();			break;//�g�����i��ԁA�~�j
	case ACTID_HADOU3B		:act_hadou3b();			break;//�g�����i��ԁA���j
	case ACTID_GCANCEL1		:act_gcancel1();		break;//�K�[�h�L�����Z��
	case ACTID_GCANCEL2		:act_gcancel2();		break;//�K�[�h�L�����Z��
	case ACTID_GCANCEL3		:act_gcancel3();		break;//�K�[�h�L�����Z��
	case ACTID_GCANCEL1J	:act_gcancel1j();		break;//�K�[�h�L�����Z��
	case ACTID_GCANCEL2J	:act_gcancel2j();		break;//�K�[�h�L�����Z��
	case ACTID_GCANCEL3J	:act_gcancel3j();		break;//�K�[�h�L�����Z��
	case ACTID_2ND_STEP		:act_2nd_step();		break;//2�i�W�����v
	case ACTID_ATT_JA		:act_att_ja();ChainCombo(CHAIN_JA);break;//�󒆂ł�`�F�[���R���{��J�E���g
	case ACTID_ATT_JB		:act_att_jb();ChainCombo(CHAIN_JB);break;//�󒆂ł�`�F�[���R���{��J�E���g
	case ACTID_ATT_JC		:act_att_jc();ChainCombo(CHAIN_JC);break;//�󒆂ł�`�F�[���R���{��J�E���g
	case ACTID_AJAMP		:act_ajamp();			break;
	case ACTID_GOODBYE1		:act_goodbye1();		break;//�O�b�o�C�E�t�H�[�G�o�[
	case ACTID_GOODBYE2		:act_goodbye2();		break;
	case ACTID_STRIKER_IN	:act_striker_in();		break;//�x���U���E��ʓ�ɓ��Ă���
	case ACTID_STRIKER_DASH	:act_striker_dash();	break;//�x���U���E����̂ق��ɂ����Ɛi��
	case ACTID_STRIKER_ATTACK:act_striker_attack();	break;//�x���U���E�U��
	case ACTID_STRIKER_OUT	:act_striker_out();		break;//�x���U���E�I��
	default:return CCharacterBase::Action();
	}
	return(TRUE);
}

void CCharacter::PreAction()
{
	if(!(pdat->aid&ACTID_KUCYU))
	{
		m_2nd_step_flag = m_opt_AStep;//2�i�W�����v�t���O�N���A
		m_aerial = FALSE;			//�G���A���t���O�N���A
	}

	CCharacterBase::PreAction();
}

/*--------------------------------------------------------------------------------------
	�G�����炢�s��
----------------------------------------------------------------------------------------*/
BOOL CCharacter::EnemyAction(DWORD eid,DWORD hid)
{
	GOBJECT* pedat = GetInfo(eid);
	if(!pedat)return FALSE;

	switch(hid)
	{
	case USERHIT_AERIALPOP	:eact_aerialpop(pedat);break;
	case USERHIT_AERIAL1	:eact_aerialhit1(pedat);break;
	case USERHIT_AERIAL2	:eact_aerialhit2(pedat);break;
	case USERHIT_AERIAL3	:eact_aerialhit3(pedat);break;
	default:
		pedat->aid=ACTID_FUTTOBI;
		return FALSE;
	}

	return TRUE;
}

/*--------------------------------------------------------------------------------------
	�U���͏�񏉊�
----------------------------------------------------------------------------------------*/
void CCharacter::InitAttackInfo()
{
	ZeroMemory(aif,sizeof(aif));
	int i;

	//�E�ʏ�U��

	i=ATTACKINFO_STAND_A;//������U��
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1  | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_WEAK;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=20;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_STAND_B;//�������U��
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2  | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_STAND_C;//�������U��
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=40;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_CROUCH_A;//���Ⴊ�ݎ�U��
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1  | GUARDINFO_XJAMP | GUARDINFO_XSTAND;//�K�[�h���
	aif[i].id		=ATTACK_WEAK;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=20;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_CROUCH_B;//���Ⴊ�ݒ��U��
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2  | GUARDINFO_XJAMP | GUARDINFO_XSTAND;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_CROUCH_C;//���Ⴊ�݋��U��
	aif[i].hit		=HITINFO_FUTTOBI2 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=40;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_JAMP_A;//�W�����v��U��
	aif[i].hit		=HITINFO_REACT1 | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1  | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_WEAK;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=20;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_JAMP_B;//�W�����v���U��
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2  | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_JAMP_C;//�W�����v���U��
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=40;//�_���[�W
	aif[i].kezuri	=0;//���

	//�E�ǉ��͋Z

	i=ATTACKINFO_STAND_D;//�������A���o�[�O���
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_STAND_E;//�������A���o�[�O���
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=0;//���

	//�E������q�݂����ȋZ

	i=ATTACKINFO_TATUMAKI1;//(��)
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=40;//�_���[�W
	aif[i].kezuri	=4;//���
	
	i=ATTACKINFO_TATUMAKI2;//(��)
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 ;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=40;//�_���[�W
	aif[i].kezuri	=4;//���

	i=ATTACKINFO_TATUMAKI3;//(��)
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 ;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=40;//�_���[�W
	aif[i].kezuri	=4;//���

	//�E���C�N

	i= ATTACKINFO_REIKU;//���C�N
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=25;//�_���[�W
	aif[i].kezuri	=1;//���

	i= ATTACKINFO_REIKU_FINISH;//���C�N�E�t�B�j�b�V��
	aif[i].hit		=HITINFO_FUTTOBI | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=2;//���

	//�E����
	i = ATTACKINFO_NAGE;
	aif[i].id		= 0;
	aif[i].damage	= 60;
	aif[i].hit		= HITINFO_MARK3 | HITINFO_SNDHIT3;

	//�E�g�����i�e�򓹋�N���X�ɃR�s�[�����j

	//��΂Ȃ��n
	i=ATTACKINFO_HADOU1A;
	aif[i].hit		=HITINFO_FUTTOBI | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=90;//�_���[�W
	aif[i].kezuri	=7;//���

	i=ATTACKINFO_HADOU2A;
	aif[i].hit		=HITINFO_FUTTOBI | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=95;//�_���[�W
	aif[i].kezuri	=7;//���
	
	i=ATTACKINFO_HADOU3A;
	aif[i].hit		=HITINFO_FUTTOBI | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=100;//�_���[�W
	aif[i].kezuri	=7;//���

	//��Ԍn
	i=ATTACKINFO_HADOU1B;
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=5;//���

	i=ATTACKINFO_HADOU2B;
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=60;//�_���[�W
	aif[i].kezuri	=5;//���
	
	i=ATTACKINFO_HADOU3B;
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=70;//�_���[�W
	aif[i].kezuri	=5;//���

	//�E�O�b�o�C�E�t�H�[�G�o�[

	i= ATTACKINFO_GOODBYE1;
	aif[i].hit		=HITINFO_REACT3A | HITINFO_STOP | HITINFO_MARK4 | HITINFO_SNDHIT3 | HITINFO_EFCTSINDO | HITINFO_EFCTFLASH ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP | GUARDINFO_EFCTSINDO;//�K�[�h���
	aif[i].id		=ATTACK_GOODBYE;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=150;//�_���[�W
	aif[i].kezuri	=20;//���

	i= ATTACKINFO_GOODBYE2;
	aif[i].hit		=HITINFO_FUTTOBI2 | HITINFO_STOP | HITINFO_MARK4 | HITINFO_SNDHIT3  | HITINFO_EFCTSINDO ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP | GUARDINFO_EFCTSINDO;//�K�[�h���
	aif[i].id		=0;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=80;//�_���[�W
	aif[i].kezuri	=5;//���

	//�G���A��

	i=ATTACKINFO_AERIALPOP;//������
	aif[i].hit		=HITINFO_USER(USERHIT_AERIALPOP) | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_AERIAL_A;//��U��
	aif[i].hit		=HITINFO_USER(USERHIT_AERIAL1) | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1  | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_WEAK;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=20;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_AERIAL_B;//�W�����v���U��
	aif[i].hit		=HITINFO_USER(USERHIT_AERIAL2) | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2  | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_AERIAL_C;//�W�����v���U��
	aif[i].hit		=HITINFO_USER(USERHIT_AERIAL3) | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=40;//�_���[�W
	aif[i].kezuri	=0;//���

	//�X�g���C�J�[��

	i=ATTACKINFO_STRIKER_DASH;//�~�j�_�b�V��
	aif[i].hit		=HITINFO_REACT3A | HITINFO_SIV1 | HITINFO_MARK3 | HITINFO_SNDHIT3;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1;//�K�[�h���
	aif[i].id		=ATTACK_STRIKER;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=0;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_STRIKER;//��r
	aif[i].hit		=HITINFO_REACT2A | HITINFO_SIV1 | HITINFO_MARK3 | HITINFO_SNDHIT3;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV1;//�K�[�h���
	aif[i].id		=0;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=10;//�_���[�W
	aif[i].kezuri	=0;//���

	//�K�[�h�L�����Z���p
	aif[ATTACKINFO_GCANCEL] = aif[ATTACKINFO_TATUMAKI2];
	i=ATTACKINFO_GCANCEL;
	aif[i].id		=0;//�Q�[�W������Ɏg�p����ID
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_ROCKET;//���P�b�g
	aif[i].hit		=HITINFO_EFCTSINDO | HITINFO_EFCTBURN | HITINFO_SIV3 | HITINFO_FUTTOBI | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_EFCTSINDO | GUARDINFO_SIV3 | GUARDINFO_REACT3| GUARDINFO_XJAMP ;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=250;//�_���[�W
	aif[i].kezuri	=25;//���

	i=ATTACKINFO_ERASER99;//�C���C�T�[
	aif[i].hit		=HITINFO_EFCTBURN_B | HITINFO_EFCTSINDO | HITINFO_SIV1 | HITINFO_FUTTOBI | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1;//�K�[�h���
	aif[i].id		=0;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=0;//�_���[�W
	aif[i].kezuri	=0;//���

	//�`�F�[���R���{OFF���́A�����_���[�W��܂����Ă��
	if(!chainComboEnabled){
		for(i=0;i<MAXNUM_ATTACKINFO;i++){
			aif[i].damage = (DWORD)(aif[i].damage * 1.2);
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

void CCharacter::EndAttack()
{
	GetGObject()->kougeki = FALSE;
}

/*--------------------------------------------------------------------------------------
	COM�s���p �Z��� ����
----------------------------------------------------------------------------------------*/
void CCharacter::InitWazInfo()//�R���s���[�^�p�Z���̐ݒ�
{
	CCharacterBase::InitWazInfo();//����ŁA���Ƃ��W�����v�Ƃ��͐ݒ肳���B���ƁA�W�����v�U����B

	waz.dashf = ACTID_DASHF;
	waz.dashb = ACTID_DASHB;

	//pdat->winfo �֍s��ID��ǉ����
	waz.att_short[0]  = ACTID_ATT_SA;//�Z���[�`�U��
	waz.att_short[1]  = ACTID_ATT_CA;
	waz.att_short[2]  = ACTID_ATT_SB;
	waz.att_short[3]  = ACTID_ATT_CB;
	waz.att_short[4]  = ACTID_GCANCEL2;
	waz.att_short[5]  = ACTID_ATT_SC;
	waz.att_middle[0] = ACTID_ATT_SC;//�����[�`�U��
	waz.att_middle[1] = ACTID_ATT_CC;
	waz.att_middle[2] = ACTID_ATT_SD;
	waz.att_middle[3] = ACTID_ATT_SE;
	waz.att_middle[4] = ACTID_REIKU1A;
	waz.att_long[0]   = ACTID_ATT_SD;//�����[�`�U�� 
	waz.att_long[1]   = ACTID_ATT_SE;
	waz.att_long[2]   = ACTID_REIKU2A;
	waz.att_long[3]   = ACTID_REIKU3A;
	waz.att_long[4]   = ACTID_GOODBYE1;

	waz.att_tai[0] = ACTID_TATUMAKI1;
	waz.att_tai[1] = ACTID_TATUMAKI2;
	waz.att_tai[2] = ACTID_TATUMAKI3;

	waz.nageid[0] = ACTID_NAGE1;

	//���[�`�ݒ�
	SetComReach( 0,MAAI_SHORT );
	SetComReach( 1,MAAI_MIDDLE );
	SetComReach( 2,MAAI_LONG );
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