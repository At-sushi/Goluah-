/*=====================================================================================

	�L�����N�^�[��`

=======================================================================================*/
#include "character.h"

#if 0

//**************************************************************************************
//  Option Settings
//�@�����ɁA�L�����N�^�[�ɐݒ�ł���I�v�V������w�肵�܂�
//**************************************************************************************

CHARACTER_LOAD_OPTION option[] = {
	// �L�����N�^�[�̃I�v�V��������ɏ����Ă��������B�i�ő�32�j
	// �Ȃ��A�I�v�V�����ƃI�v�V�����̊Ԃ̓R���}(,)�ŋ�؂�Ă��������B

	// �L���e�F
	// �@{ �t���O, ���������߼��,�ˑ������߼��, �I�v�V������, ���|�C���g }
	{ OPTIONS_CHAIN_COMBO		,0, 0, "Chain Combo"	, 5}, //�L���i����Ă�OK�j
	{ OPTIONS_EXTRA_ATTACK		,0, 0, "Extra Attack"	, 3},
	{ OPTIONS_AUTO_GUARD		,0, 0, "Auto Guard"		, 5},
	{ OPTIONS_GUARD_CANCEL		,0, OPTIONS_AUTO_GUARD, "Guard Cancel"	, 6},
	{ OPTIONS_G_CUTTER_PLUS		,0, 0, "G.Cutter +"		, 4},
	{ OPTIONS_CHIBASIRI_PLUS	,0, 0, "Chibasiri +"	, 3},
	{ OPTIONS_2ND_STEP			,0, 0, "2nd-Step"		, 3},
	{ OPTIONS_STOCKABLE_GAUGE	,0, 0, "Stockable Gauge", 4},
	{ OPTIONS_MINIGUN_RELOADED	,0,OPTIONS_STOCKABLE_GAUGE,"Minigun -Reloaded-",3},
	{ OPTIONS_MORARAEL_MODE		,0,0,"TEST",0},
};


//**************************************************************************************
//  Character Information
//�@�����ɃL�����N�^�[�̖��O��L�q���܂�
//**************************************************************************************

CCharacterInfo CharInfo("�������[",		// �L�����N�^�[�̖��O�i�ő�31�o�C�g�j
						CDI_VERSION,		// �L�����N�^�[DLL�C���^�[�t�F�C�X�̃o�[�W����
						option,				// �I�v�V�����\���̂ւ̃|�C���^
						sizeof(option) / sizeof(CHARACTER_LOAD_OPTION), // �I�v�V�����̐�
						18,					// �I�v�V�����ő�|�C���g��
						FALSE);				// �l�b�g�ΐ�Ή����ǂ��� �����̊g���̂��߂ɗp�ӂ���Ă���B���݂�0��n���Ȃ���΂Ȃ�Ȃ��i�΁j

char* CharDirectory = NULL;

//�ݒ�}�N���BCCharacterInfo�ϐ������ƁA�L�����N�^�[�N���X����w�肵�܂�
SET_CHARACTERINFO(CharInfo, CCharacter)

#endif //0


/*--------------------------------------------------------------------------------------
	�L�����N�^�[������
	�ʏ�A������K�v����܂���
----------------------------------------------------------------------------------------*/

CCharacter::CCharacter(CDI_CHARACTERINFO2 *info) : CCharacterBase(info)
{
	CharDirectory = info->dir;	// �L�����N�^�[�̃t�H���_���

	//aki�p�G�t�F�N�g����
	aki3d.Initialize( 
					CGoluahObject::GetD3DDevice() ,
					CharDirectory
					);
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

	pdat->hpmax			= 1000;//�ő�̗͒l
	pdat->gaugemax		= m_opt_StockableGauge ? 3 : 1;//�ő�Q�[�W
	pdat->gaugetype		= m_opt_StockableGauge ? GOBJFLGA_POWGAUGE1 : GOBJFLGA_POWGAUGE2 ;//�Q�[�W�^�C�v
}

//DWORD �I�v�V�����w��� BOOL�����o�ϐ��ɕ�������
void CCharacter::InitAnalyzeOptions()
{
	chainComboEnabled	 = TRUE ;
	isAutoGuard			 = (option_flags&OPTIONS_AUTO_GUARD) ? TRUE : FALSE ;
	m_opt_ExtraAttack	 = (option_flags&OPTIONS_EXTRA_ATTACK) ? TRUE : FALSE ;
	m_opt_GuardCancel	 = (option_flags&OPTIONS_GUARD_CANCEL) ? TRUE : FALSE ;
	m_opt_GCutterPlus	 = (option_flags&OPTIONS_G_CUTTER_PLUS) ? TRUE : FALSE ;
	m_opt_ChibasiriPlus	 = (option_flags&OPTIONS_CHIBASIRI_PLUS) ? TRUE : FALSE ;
	m_opt_2ndStep		 = (option_flags&OPTIONS_2ND_STEP) ? TRUE : FALSE ;
	m_opt_StockableGauge = (option_flags&OPTIONS_STOCKABLE_GAUGE) ? TRUE : FALSE ;
	m_opt_minigunReloaded= (option_flags&OPTIONS_MINIGUN_RELOADED) ? TRUE : FALSE ;
}

/*--------------------------------------------------------------------------------------
	���b�Z�[�W����
	�x�[�X�N���X�ŏE��Ă��Ȃ����b�Z�[�W��E�������ꍇ�́A�R�R�ɏ�����ǉ���܂�
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Message(DWORD msg,LPVOID dat,DWORD prm)
{
	GOBJECT* pobjdat;

	switch(msg){
	//�򓹋�q�b�g�E�K�[�h
	case GOBJMSG_CHIBAHIT:		AddPowerGauge(0.10);break;
	case GOBJMSG_CHIBAGUARD:	AddPowerGauge(0.05);break;
	case GOBJMSG_NAGE://�����A�G������
		{
			pobjdat = GetInfo(prm);
			if(pobjdat==NULL)return(0);
			return(eact_nage(pobjdat));
		}
		break;
	case GOBJMSG_GCUTTERUP:
		{
			pobjdat = GetInfo(prm);
			if(pobjdat==NULL)return(0);
			dact_futtobi2(pobjdat);
			if(pobjdat->vy>0)pobjdat->muteki = TRUE;
		}
		break;
	case GOBJMSG_STRIKER://�x���U���v��
		if(pdat->aid == ACTID_TAIKICYU){
			RemoveProperty( GOBJFLG_TOBETARGET );//�^�[�Q�b�g�ɂ͂Ȃ�Ȃ�...�Ȃ�ł���O���񂾂���H(�Y)
			ChangeAction( ACTID_STRIKER_IN );
			return(WAIT_A_SEC);
		}
		return(0);
	default:
		return(CCharacterBase::Message(msg,dat,prm));//�L�����N�^�[��{�N���X�ɏ�����C����
	}
	return(TRUE);
}


/*--------------------------------------------------------------------------------------
	GOBJMSG_TOUCHB
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
	if(!hit)dp*=0.2;
	AddPowerGauge(dp);

	if(IsLocalCom())
	{
		switch(pdat->aid)
		{
		case ACTID_ATT_SA:
			{
				if(ComLevelCk(3)){
					SetComAct(ACTID_ATT_SB,5);
				}
			}
			break;
		case ACTID_ATT_SB:
			{
				if(ComLevelCk(2)){
					SetComAct(ACTID_ATT_SB2,6);
				}
			}
			break;
		case ACTID_ATT_SC:
			{
				if(ComLevelCk(5) && pdat->gauge>=1.0f)
				{
					SetComAct(ACTID_MINIGUN,10);
				}
				else if(ComLevelCk(3)){
					if(ChainCheck(CHAIN_SC3)){
						SetComAct(ACTID_ATT_SC3,12);
					}
				}
			}
			break;

		case ACTID_ATT_SB2:
			{
				if(ComLevelCk(3)){
					if(ChainCheck(CHAIN_SC3)){
						SetComAct(ACTID_ATT_SC3,18);
					}
					else if(ChainCheck(CHAIN_CB)){
						SetComAct(ACTID_ATT_CB,18);
					}
				}
			}
			break;
		case ACTID_ATT_SC2:
			{
				if(ComLevelCk(3)){
					SetComAct(ACTID_ATT_SB,5);
				}
			}
			break;
		case ACTID_ATT_SC3:
			{
				if(ComLevelCk(5) && pdat->gauge>=1.0f){
					SetComAct(ACTID_MINIGUN,10);
				}
				else if(ComLevelCk(5) && ChainCheck(CHAIN_CB)){
					SetComAct(ACTID_ATT_CB,8);
				}
				else if(ComLevelCk(3) && ChainCheck(CHAIN_CC)){
					SetComAct(ACTID_ATT_CC,8);
				}
			}
			break;

		case ACTID_ATT_CA:
			{
			}
			break;
		case ACTID_ATT_CB:
			{
				if(ComLevelCk(3) && ChainCheck(CHAIN_CC)){
					SetComAct(ACTID_ATT_CC,6);
				}
				else if(ComLevelCk(3) && ChainCheck(CHAIN_SC3)){
					SetComAct(ACTID_ATT_SC3,6);
				}
			}
			break;
		case ACTID_ATT_CC:
			{
				if(ComLevelCk(4)){
					SetComAct(ACTID_GENOCIDE3G,8);
				}
			}
			break;

		case ACTID_GENOCIDE1J:
		case ACTID_GENOCIDE2J:
		case ACTID_GENOCIDE3J:
			{
				if(m_opt_GCutterPlus)
				{
					if(ComLevelCk(3))
					{
						if(rand()%10<7){
							SetComAct(ACTID_GENOCIDE_UP,20);
						}
						else{
							SetComAct(ACTID_GENOCIDE_DOWN,20);
						}
					}
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
	//��AQ�h�~
	if(info==&aif[ATTACKINFO_CROUCH_C])
	{
		GOBJECT *pedat = GetInfo(tc_eid);
		if(pedat)
		{
			if(pedat->aid==ACTID_FUTTOBI2)
			{
				return TOUCHC_CANCEL;
			}
		}
	}

	return CCharacterBase::TouchC(info,tc_eid);
}

/*--------------------------------------------------------------------------------------
	�R�}���h���� ����
	GOBJMSG_COMMAND ���b�Z�[�W�ɑΉ����܂��B���݂̍s���ɂ��ĕ����̊֐���
	������������Ă��܂��B�K�؂ȂƂ���ɔ��菈����ǉ���Ă��������B
----------------------------------------------------------------------------------------*/
BOOL CCharacter::Command_Hissatu(DWORD key)
{
	//�K�E�Z�R�}���h����
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_KUCYU){//�󒆕K�E�Z�̃R�}���h����
	}
	else//�n��K�E�Z�̃R�}���h����
	{
		//�~�j�K��
		if(com426(45) && (key & KEYSTA_BC2))
		{
			if(pdat->gauge >= 1.0f){
				pdat->aid = ACTID_MINIGUN;
				return TRUE;
			}
		}

		//G�E�J�b�^�[
		if(com623(30)){
			if(key & KEYSTA_BC2){pdat->aid = ACTID_GENOCIDE3G;return TRUE;}
			else if(key & KEYSTA_BB2){pdat->aid = ACTID_GENOCIDE2G;return TRUE;}
			else if(key & KEYSTA_BA2){pdat->aid = ACTID_GENOCIDE1G;return TRUE;}
		}

		//�n����
		if(com236(30)){
			if((key & KEYSTA_BC2) && bul_chiba->isReady()){
				pdat->aid = ACTID_CHIBA3;return TRUE;
			}
			else if((key & KEYSTA_BB2) && bul_chiba->isReady()){
				pdat->aid = ACTID_CHIBA2;return TRUE;
			}
			else if((key & KEYSTA_BA2) && bul_chiba->isReady()){
				pdat->aid = ACTID_CHIBA1;return TRUE;
			}
		}
	}

	return CCharacterBase::Command_Hissatu(key);
}

BOOL CCharacter::Command_Normal(DWORD key)
{
	//�_�b�V���R�}���h
	if(com66i(30) && (key&KEYSTA_FOWORD2)){pdat->aid=ACTID_DASHF;return TRUE;}
	if(com44i(30) && (key&KEYSTA_BACK2)  ){pdat->aid=ACTID_DASHB;return TRUE;}

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
		//if(key & KEYSTA_BACK && pdat->aid==ACTID_DASHB)return TRUE;//�_�b�V���p��
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
	else{//�n�㓮�쒆
		//����
		if((key & KEYSTA_FOWORD) && (key & KEYSTA_BC2) &&
			NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE))
		{
				pdat->aid = ACTID_NAGE1;
				return TRUE;
		}

		//���o�[�O��ꋭ
		if((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD) && m_opt_ExtraAttack){
			ChangeAction(  ACTID_ATT_SC3 );return TRUE;
		}
	}

	return CCharacterBase::Command_OnNormal(key);
}

BOOL CCharacter::Command_OnAttacking(DWORD key)
{
	if(m_opt_ExtraAttack)
	{
		if(pdat->aid == ACTID_ATT_SB){
			if(key & KEYSTA_BB2){
				ChangeAction(  ACTID_ATT_SB2 );return TRUE;
			}
		}

		if(pdat->aid == ACTID_ATT_SC){
			if(key & KEYSTA_BC2){
				ChangeAction(  ACTID_ATT_SC2 );return TRUE;
			}
		}
		
		if(!(pdat->aid & ACTID_KUCYU))
		{
			if((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD)){
				if(ChainCombo(CHAIN_SC3)){
					ChangeAction(  ACTID_ATT_SC3 );return TRUE;
				}
			}
		}
	}

	return CCharacterBase::Command_OnAttacking(key);
}

//��炢�E�K�[�h��
BOOL CCharacter::Command_OnSystem(DWORD key)
{
	if(pdat->aid & ACTID_GUARD){
		if(!(pdat->aid & ACTID_KUCYU) && m_opt_GuardCancel)
		{
			//�n��K�[�h��
			//�K�[�h�L�����Z�� G�E�J�b�^�[
			if(com623(30) && pdat->gauge>0.3){
				if(key & KEYSTA_BC2){pdat->aid = ACTID_GCANCEL3;return TRUE;}
				else if(key & KEYSTA_BB2){pdat->aid = ACTID_GCANCEL2;return TRUE;}
				else if(key & KEYSTA_BA2){pdat->aid = ACTID_GCANCEL1;return TRUE;}
			}
		}
	}

	return CCharacterBase::Command_OnSystem(key);
}

//�K�E�Z��
BOOL CCharacter::Command_OnHissatuAttacking(DWORD key)
{
	if(m_opt_GCutterPlus){
		if(pdat->aid==ACTID_GENOCIDE1J || pdat->aid==ACTID_GENOCIDE2J || pdat->aid==ACTID_GENOCIDE3J )
		{
			if((key & (KEYSTA_BC2|KEYSTA_BB2|KEYSTA_BA2)) && (pdat->counter>6) )
			{
				if(key&KEYSTA_DOWN){
					ChangeAction(ACTID_GENOCIDE_DOWN);
					return TRUE;
				}
				else if(key&KEYSTA_UP){
					ChangeAction(ACTID_GENOCIDE_UP);
					return TRUE;
				}
			}
		}
	}
	//�~�j�K�� - �����[�f�b�h
	if(pdat->aid==ACTID_MINIGUN && m_opt_minigunReloaded){
		if(com426(45) && (key & KEYSTA_BC2))
		{
			if(pdat->gauge >= 1.0f){
				pdat->counter = 0;
				return TRUE;
			}
		}
	}

	return CCharacterBase::Command_OnHissatuAttacking(key);
}

/*--------------------------------------------------------------------------------------
	�R���s���[�^�R�}���h
----------------------------------------------------------------------------------------*/

DWORD CCharacter::CommandCOM(DWORD wid)
{
	if(wid==ACTID_MINIGUN && pdat->gauge<1.0f)return FALSE;//�Q�[�W�`�F�b�N

	if(wid==ACTID_MINIGUN && pdat->aid==ACTID_MINIGUN && m_opt_minigunReloaded)
	{
		return TRUE;
	}
	if(wid==ACTID_CHIBA1 || wid==ACTID_CHIBA2 || wid==ACTID_CHIBA3)
	{
		if(!bul_chiba->isReady())return FALSE;
	}

	return CCharacterBase::CommandCOM(wid);
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

	if(!(pdat->aid&ACTID_KUCYU) && (m_opt_ExtraAttack))
	{
		//����͌n
		if(wid==ACTID_ATT_SB2)
		{
			return (pdat->aid==ACTID_ATT_SB) ? TRUE : FALSE;
		}
		if(wid==ACTID_ATT_SC2)
		{
			return (pdat->aid==ACTID_ATT_SC) ? TRUE : FALSE;
		}

		//�`�F�[���R���{����(�ǉ�)
		if(GetGObject()->aid & ACTID_ATTACK)
		{
			switch(wid)
			{
			case ACTID_ATT_SC3	:return ChainCombo(CHAIN_SC3);break;
			}
		}
	}

	return CCharacterBase::CmdCom_OnNormal(wid);
}

/*--------------------------------------------------------------------------------------
	GOBJMSG_ACTION ���b�Z�[�W ����
	�V�����s����ǉ�����ꍇ�́A�����ɏ�����ǉ���܂��B
	�s���̑O��ɋ��ʏ������ꂽ���ꍇ�� PreAction(),PostAction() ��I�[�o�[���C�h���Ă�������
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Action()
{
	switch(GetGObject()->aid){
	case ACTID_ATT_SB2		:act_att_sb2();		break;
	case ACTID_ATT_SC2		:act_att_sc2();		break;
	case ACTID_ATT_SC3		:act_att_sc3();		ChainCombo(CHAIN_SC3);break;
	case ACTID_LANDING		:act_landing();		break;
	case ACTID_GENOCIDE1G	:act_genocide1g();	break;
	case ACTID_GENOCIDE1J	:act_genocide1j();	break;
	case ACTID_GENOCIDE2G	:act_genocide2g();	break;
	case ACTID_GENOCIDE2J	:act_genocide2j();	break;
	case ACTID_GENOCIDE3G	:act_genocide3g();	break;
	case ACTID_GENOCIDE3J	:act_genocide3j();	break;
	case ACTID_LANDINGG		:act_landingg();	break;
	case ACTID_DASHF		:act_dashf();		break;
	case ACTID_DASHB		:act_dashb();		break;
	case ACTID_CHIBA1		:act_chiba1();		break;
	case ACTID_CHIBA2		:act_chiba2();		break;
	case ACTID_CHIBA3		:act_chiba3();		break;
	case ACTID_MINIGUN		:act_minigun();		break;
	case ACTID_MINIGUN2		:act_minigun2();	break;
	case ACTID_GCANCEL1		:act_gcancel1();	break;
	case ACTID_GCANCEL2		:act_gcancel2();	break;
	case ACTID_GCANCEL3		:act_gcancel3();	break;
	case ACTID_2ND_STEP		:act_2nd_step();	break;
	case ACTID_GENOCIDE_UP	:act_genocide_up();	break;
	case ACTID_GENOCIDE_DOWN:act_genocide_down();break;
	case ACTID_NAGE1		:act_nage();		break;
	case ACTID_STRIKER_IN	:act_striker_in();	break;
	case ACTID_STRIKER_ATK1	:act_striker_atk1();break;
	case ACTID_STRIKER_ATK2	:act_striker_atk2();break;
	case ACTID_STRIKER_OUT	:act_striker_out();	break;
	default:return CCharacterBase::Action();
	}
	return(TRUE);
}

void CCharacter::PreAction()
{
	if(!(pdat->aid&ACTID_KUCYU)){//2�i�W�����v�t���O�N���A
		m_2nd_step_flag = m_opt_2ndStep;
	}

	if( pdat->aid==ACTID_MINIGUN && m_opt_minigunReloaded && pdat->counter>220 && pdat->counter<225)
	{
		if(ComLevelCk(5) && rand()%2)
		{
			SetComAct(ACTID_MINIGUN,5);
		}
	}

	CCharacterBase::PreAction();
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
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_STAND_B;//�������U��
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2  | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=40;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_STAND_C;//�������U��
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=45;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_CROUCH_A;//���Ⴊ�ݎ�U��
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1  | GUARDINFO_XJAMP | GUARDINFO_XSTAND;//�K�[�h���
	aif[i].id		=ATTACK_WEAK;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_CROUCH_B;//���Ⴊ�ݒ��U��
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2  | GUARDINFO_XJAMP | GUARDINFO_XSTAND;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=40;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_CROUCH_C;//���Ⴊ�݋��U��
	aif[i].hit		=HITINFO_FUTTOBI2 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=55;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_JAMP_A;//�W�����v��U��
	aif[i].hit		=HITINFO_REACT1 | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1  | GUARDINFO_XCROUCH;//�K�[�h���
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
	aif[i].damage	=55;//�_���[�W
	aif[i].kezuri	=0;//���

	//�E�ǉ��͋Z

	i=ATTACKINFO_STAND_B2;//�������A�ǉ��͋Z
	aif[i].hit		=HITINFO_REACT3A | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV2 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_STAND_C2;//�������A�ǉ��͋Z
	aif[i].hit		=HITINFO_TATAKI2B | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH ;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=60;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_STAND_C3;//�������A���o�[�O���
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=0;//���

	//�E�W�F�m�T�C�h�E�A��

	i=ATTACKINFO_GENOCIDE1;
	aif[i].hit		=HITINFO_FUTTOBI | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=65;//�_���[�W
	aif[i].kezuri	=5;//���
	
	i=ATTACKINFO_GENOCIDE2;
	aif[i].hit		=HITINFO_FUTTOBI | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 ;//�K�[�h���
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=70;//�_���[�W
	aif[i].kezuri	=5;//���
	
	i=ATTACKINFO_GENOCIDE3;
	aif[i].hit		=HITINFO_FUTTOBI | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 ;//�K�[�h���
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=75;//�_���[�W
	aif[i].kezuri	=5;//���

	//�E�n����i�e�򓹋�N���X�ɃR�s�[�����j

	i=ATTACKINFO_CHIBA1;
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 | HITINFO_EFCTBURN;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=5;//���

	i=ATTACKINFO_CHIBA2;
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3  | HITINFO_EFCTBURN;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=60;//�_���[�W
	aif[i].kezuri	=5;//���
	
	i=ATTACKINFO_CHIBA3;
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3  | HITINFO_EFCTBURN;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=70;//�_���[�W
	aif[i].kezuri	=5;//���

	//�E�~�j�K��
	i=ATTACKINFO_MINIGUN;
	aif[i].hit		=HITINFO_REACT3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=0;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=15;//�_���[�W
	aif[i].kezuri	=1;//���

	//�E�W�F�m�T�C�h�A�ǉ���

	i=ATTACKINFO_GENOCIDE_UP;
	aif[i].hit		=HITINFO_USER(USERHIT_GCUTTERUP) | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH ;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_GENOCIDE_DOWN;
	aif[i].hit		=HITINFO_TATAKI1B | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH ;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=100;//�_���[�W
	aif[i].kezuri	=5;//���

	//�E����
	i = ATTACKINFO_NAGE;
	aif[i].id		= 0;
	aif[i].damage	= 80;
	aif[i].hit		= HITINFO_MARK3 | HITINFO_SNDHIT3;

	//�E�x���U��

	i=ATTACKINFO_STRIKER1;
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=0;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_STRIKER2;
	aif[i].hit		=HITINFO_REACT3A | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=0;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
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
	//����W�����v�Ȃǂ̍s��
	CCharacterBase::InitWazInfo();

	waz.dashf = ACTID_DASHF;
	waz.dashb = ACTID_DASHB;

	//pdat->winfo �֍s��ID��ǉ����
	waz.att_short[0] = ACTID_ATT_SA;
	waz.att_short[1] = ACTID_ATT_CA;
	waz.att_short[2] = ACTID_ATT_SC;
	waz.att_short[3] = ACTID_NAGE1;
	waz.att_middle[0] = ACTID_ATT_SB;
	waz.att_middle[1] = ACTID_ATT_SB2;
	waz.att_middle[2] = ACTID_ATT_SC2;
	waz.att_middle[3] = ACTID_ATT_SC3;
	waz.att_long[0] = ACTID_ATT_SB2;
	waz.att_long[1] = ACTID_ATT_CB;
	waz.att_long[2] = ACTID_ATT_CC;
	waz.att_long[3] = ACTID_ATT_SC2;
	waz.att_long[4] = ACTID_ATT_SC3;
	waz.att_long[5] = ACTID_GENOCIDE2G;
	waz.att_tai[0] = ACTID_GENOCIDE1G;
	waz.att_tai[1] = ACTID_ATT_SC;
	waz.att_bullet[0] = ACTID_CHIBA1;
	waz.att_bullet[1] = ACTID_CHIBA2;
	waz.att_bullet[2] = ACTID_CHIBA3;

	//���[�`�ݒ�
	SetComReach( 0,MAAI_SHORT	);
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
