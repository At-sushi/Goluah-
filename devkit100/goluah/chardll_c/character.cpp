
/*=====================================================================================

	�L�����N�^�[��`

=======================================================================================*/
#include "character.h"

//**************************************************************************************
//  Option Settings
//�@�����ɁA�L�����N�^�[�ɐݒ�ł���I�v�V�������w�肵�܂�
//**************************************************************************************

CHARACTER_LOAD_OPTION option[] = {
	// �L�����N�^�[�̃I�v�V�����������ɏ����Ă��������B�i�ő�32�j
	// �Ȃ��A�I�v�V�����ƃI�v�V�����̊Ԃ̓R���}(,)�ŋ�؂��Ă��������B

	// �L�����e�F
	// �@{ �t���O, ���������߼��,�ˑ������߼��, �I�v�V������, ����|�C���g }
	{ OPTIONS_QUICK_LUNCH_A	, 0, 0, "Quick Lunch - A"	, 5 },
	{ OPTIONS_QUICK_LUNCH_B	, 0, 0, "Quick Lunch - B"	, 5 },
	{ OPTIONS_QUICK_LUNCH_C	, 0, 0, "Quick Lunch - C"	, 5 },
	{ OPTIONS_CRAZY_LUNCH	, 0, OPTIONS_QUICKS, "Crazy Lunch"		,10 },
	{ OPTIONS_2ND_STEP		, 0, 0, "2nd Step"			, 3 },
	{ OPTIONS_REVERSAL		, 0, 0, "Reversal Attack"	, 3 },
	{ OPTIONS_NO_CHAIN_COMBO, 0, 0, "No Chain Combo"	,-5 },
};


//**************************************************************************************
//  Character Information
//�@�����ɃL�����N�^�[�̖��O���L�q���܂�
//**************************************************************************************

CCharacterInfo CharInfo("����",				// �L�����N�^�[�̖��O�i�ő�31�o�C�g�j
						CDI_VERSION,		// �L�����N�^�[DLL�C���^�[�t�F�C�X�̃o�[�W����
						option,				// �I�v�V�����\���̂ւ̃|�C���^
						sizeof(option) / sizeof(CHARACTER_LOAD_OPTION), // �I�v�V�����̐�
						20,					// �I�v�V�����ő�|�C���g��
						FALSE);				// �l�b�g�ΐ�Ή����ǂ��� �����̊g���̂��߂ɗp�ӂ���Ă���B���݂�0��n���Ȃ���΂Ȃ�Ȃ��i�΁j

char* CharDirectory = NULL;

//�ݒ�}�N���BCCharacterInfo�ϐ������ƁA�L�����N�^�[�N���X�����w�肵�܂�
SET_CHARACTERINFO(CharInfo, CCharacter)


/*--------------------------------------------------------------------------------------
	�L�����N�^�[������
	�ʏ�A������K�v����܂���
----------------------------------------------------------------------------------------*/

CCharacter::CCharacter(CDI_CHARACTERINFO2 *info) : CCharacterBase(info)
{
	CharDirectory = info->dir;	// �L�����N�^�[�̃t�H���_������
	aki3d.Initialize( GetD3DDevice(), CharDirectory);
}

/*--------------------------------------------------------------------------------------
	�L�����N�^�[�j������
	�ʏ�A������K�v����܂���
----------------------------------------------------------------------------------------*/
CCharacter::~CCharacter()
{
}

/*--------------------------------------------------------------------------------------
	�p�����[�^������
	�����ɁA�e��p�����[�^�̏�����������ǉ����܂�
----------------------------------------------------------------------------------------*/
void CCharacter::InitParameters()
{
	//�I�v�V��������
	m_opt_quickA	= (option_flags&OPTIONS_QUICK_LUNCH_A	) ? TRUE : FALSE;
	m_opt_quickB	= (option_flags&OPTIONS_QUICK_LUNCH_B	) ? TRUE : FALSE;
	m_opt_quickC	= (option_flags&OPTIONS_QUICK_LUNCH_C	) ? TRUE : FALSE;
	m_opt_2nd_step	= (option_flags&OPTIONS_2ND_STEP		) ? TRUE : FALSE;
	m_opt_reversal	= (option_flags&OPTIONS_REVERSAL		) ? TRUE : FALSE;
	m_opt_crazy		= (option_flags&OPTIONS_CRAZY_LUNCH		) ? TRUE : FALSE;

	pdat->hpmax			= 1000;//�ő�̗͒l
	pdat->gaugemax		= 1;//�ő�Q�[�W
	pdat->gaugetype		= GOBJFLGA_POWGAUGE2;//�Q�[�W�^�C�v

	chainComboEnabled	= (option_flags&OPTIONS_NO_CHAIN_COMBO	) ? FALSE : TRUE;
}


/*--------------------------------------------------------------------------------------
	���b�Z�[�W����
	�x�[�X�N���X�ŏE���Ă��Ȃ����b�Z�[�W���E�������ꍇ�́A�R�R�ɏ�����ǉ����܂�
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Message(DWORD msg,LPVOID dat,DWORD prm)
{
	GOBJECT* pobjdat = NULL;
	if(msg & GOBJMSG_USERDEFINE)
	{
		pobjdat = GetInfo(prm);
		if(pobjdat==NULL)return(0);

		switch(msg)
		{
		case GOBJMSG_NAGE:	return eact_nage(pobjdat);
		}
		return TRUE;
	}

	switch(msg){
	case GOBJMSG_STRIKER://�x���U���v��
		if(pdat->aid == ACTID_TAIKICYU){
			RemoveProperty( GOBJFLG_TOBETARGET );//�^�[�Q�b�g�ɂ͂Ȃ�Ȃ�
			ChangeAction( ACTID_STRIKER_IN );
			return(ROGER);
		}
		return(0);
	default:
		return(CCharacterBase::Message(msg,dat,prm));//�L�����N�^�[��{�N���X�ɏ�����C����
	}
	return(TRUE);
}


/*--------------------------------------------------------------------------------------
	�R�}���h���� ����
	GOBJMSG_COMMAND ���b�Z�[�W�ɑΉ����܂��B���݂̍s���ɂ���ĕ����̊֐���
	������������Ă��܂��B�K�؂ȂƂ���ɔ��菈����ǉ����Ă��������B
----------------------------------------------------------------------------------------*/

BOOL CCharacter::Command_OnHissatuAttacking(DWORD key)
{
	//�A���R���E�^����
	if((pdat->aid&ACTID_MOYAMOYA) && pdat->counter>5)
	{
		if(key&(KEYSTA_BC|KEYSTA_BB|KEYSTA_BA) || IsCom())
		{
			//�{�^����������Ă��
			if( com22(20) && (GetKey(0)&KEYSTA_DOWN2) )
			{
				ArekoreCancel();
				return TRUE;
			}
		}
		else
		{
			//�{�^����������ĂȂ���Δ���
			ArekoreRelease();
			return TRUE;
		}
	}

	//���C�t���A��������
	if(pdat->aid==ACTID_POI_RIFLE)
	{
		if( (pdat->counter>30) && 
			( pdat->counter<12+RIFLE_TIMEOUT ) || (IsLocalCom() && ( pdat->counter<12+200 ))
		)
		{
			if(key&KEYSTA_BUTTONS)
			{
				pdat->counter = 12+RIFLE_TIMEOUT;
				pbul_rifle->GetGObject()->counter = RIFLE_TIMEOUT-1;
			}
		}
	}

	return CCharacterBase::Command_OnHissatuAttacking(key);
}

BOOL CCharacter::Command_Hissatu(DWORD key)
{
	//�K�E�Z�R�}���h����
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_KUCYU){//�󒆕K�E�Z�̃R�}���h����
	}
	else//�n��K�E�Z�̃R�}���h����
	{
		//�u���b�f�B�[�E�_�b�R
		if(com236236(40))
		{
			if((key & KEYSTA_BC2) && pdat->gauge>=1.0f)
			{
				ChangeAction( ACTID_DAKKO );
				return TRUE;
			}
		}

		//�A���R��
		if(com236(30))
		{
			if(key & KEYSTA_BC2)
			{
				pdat->aid = ACTID_MOYAMOYA3;
				return TRUE;
			}
			else if(key & KEYSTA_BB2)
			{
				pdat->aid = ACTID_MOYAMOYA2;
				return TRUE;
			}
			else if(key & KEYSTA_BA2)
			{
				pdat->aid = ACTID_MOYAMOYA1;
				return TRUE;
			}
		}
	}

	return CCharacterBase::Command_Hissatu(key);
}

BOOL CCharacter::Command_Normal(DWORD key)
{
	//�_�b�V��
	if(com66(20)){pdat->aid = ACTID_DASHF;return TRUE;}
	if(com44(20)){pdat->aid = ACTID_DASHB;return TRUE;}

	if(key & KEYSTA_UP){//�W�����v
		pdat->aid = ACTID_JAMPS;
		nidanjamp=TRUE;
	}
	else if(key & KEYSTA_DOWN){//���Ⴊ��
		pdat->aid = ACTID_CROUCH;
	}
	else if(key & KEYSTA_FOWORD){//�O����
		pdat->aid = ACTID_WALKF;
	}
	else if(key & KEYSTA_BACK){//������
		pdat->aid = ACTID_WALKB;
	}
	else{//�j���[�g�����|�[�Y
		pdat->aid = ACTID_NEUTRAL;
	}

	return TRUE;
}

BOOL CCharacter::Command_OnNormal(DWORD key)
{
	DWORD key_now = key;

	if(pdat->aid & ACTID_KUCYU)//�W�����v���쒆
	{
		if(nidanjamp && m_opt_2nd_step){//2�i�W�����v
			if(key_now & KEYSTA_UP2){
				pdat->aid = ACTID_2DANJAMPS;
				nidanjamp=FALSE;
				return TRUE;
			}
		}
		if(pdat->aid == ACTID_2DANJAMPS){
			if(key_now & KEYSTA_FOWORD){//�O�W�����v
				pdat->aid = ACTID_2DANJAMPF;
				return TRUE;
			}
			else if(key_now & KEYSTA_BACK){//��W�����v
				pdat->aid = ACTID_2DANJAMPB;
				return TRUE;
			}
		}

		//�_�b�V�����A�ł�����ɋZ���o���Ȃ��悤�ɂ���
		if(pdat->aid==ACTID_DASHF)
		{
			if(pdat->counter<10)
			{
				key &= ~KEYSTA_ANYKEY;
			}
		}
	}
	else//�n�㓮�쒆
	{
		//����
		if (key & KEYSTA_FOWORD && key & KEYSTA_BC2 &&
			NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE))
		{
			pdat->aid = ACTID_NAGE1;
			return TRUE;
		}

		//�X�e�b�L�i��+C�j
		if((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD) && !(key & KEYSTA_DOWN))
		{
			ChangeAction( ACTID_STAFF );
			return TRUE;
		}
	}

	return CCharacterBase::Command_OnNormal(key);
}


BOOL CCharacter::Command_OnAttacking(DWORD key)
{
	if(pdat->aid & ACTID_KUCYU)//�W�����v���쒆
	{
	}
	else//�n��
	{
		if((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD) && !(key & KEYSTA_DOWN))
		{
			if(ChainCombo(CHAIN_STAFF))
			{
				ChangeAction( ACTID_STAFF );
				return TRUE;
			}
		}
	}

	return CCharacterBase::Command_OnAttacking(key);
}

BOOL CCharacter::Command_OnSystem(DWORD key)
{
	//���o�[�T��
	if(pdat->aid==ACTID_OKIAGARI && m_opt_reversal)
	{
		if(ccounter==10  && pdat->gauge>=NEEDGAUGE_REV)
		{
			if(key&KEYSTA_UP)
			{
				ChangeAction( ACTID_REVERSAL );
				return TRUE;
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
	if(wid==ACTID_DAKKO && pdat->gauge<1.0f)return FALSE;//�Q�[�W�`�F�b�N
	return CCharacterBase::CommandCOM(wid);
}

DWORD CCharacter::CmdCom_OnNormal(DWORD wid)
{
	//����
	if(!(pdat->aid&ACTID_KUCYU) && !(pdat->aid&ACTID_ATTACK))
	{
		if (wid==ACTID_STAFF) return(TRUE);
		if (wid==ACTID_NAGE1)
		{
			if(NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE)) {
				pdat->aid = ACTID_NAGE1;
				return TRUE;
			}
			else return FALSE;
		}
	}
	return CCharacterBase::CmdCom_OnNormal(wid);
}

/*--------------------------------------------------------------------------------------
	GOBJMSG_TOUCHB(�U��������)
----------------------------------------------------------------------------------------*/
DWORD CCharacter::TouchB(ATTACKINFO *info,BOOL hit)
{
	if(info->id == ATTACK_DAKKO)
	{
		if(hit)
		{
			GOBJECT *hit_obj = GetInfo(m_dakko_eid);
			if(hit_obj)
			{
				hit_obj->color = 0xFF000000;
			}
			AddProperty( GOBJFLG_DONOTSTOP );
			pbul_blood->Go(pdat->muki,pdat->x+zurex(100),-80,0,0);
			ChangeAction( ACTID_DAKKO_BACK );
			PlayMySound(wave_dakko_hit);
			AddEffect(EFCTID_FLASH, 4, 0, 0);
		}
		return CCharacterBase::TouchB(info,hit);
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
			case ACTID_ATT_SC:
			case ACTID_ATT_CC:
			{
				if(ComLevelCk(5) && pdat->gauge>=1.0f)
				{
					SetComAct(ACTID_DAKKO,10);
				}
				else if(ComLevelCk(3))
				{
					SetComAct(ACTID_STAFF,5);
				}
			}
			break;
		}
	}

	return CCharacterBase::TouchB(info,hit);
}

/*--------------------------------------------------------------------------------------
	GOBJMSG_TOUCHB(�U��������E�O����)
----------------------------------------------------------------------------------------*/
DWORD CCharacter::TouchC(ATTACKINFO *info,DWORD tc_eid)
{
	if(info->id == ATTACK_DAKKO)
	{
		m_dakko_eid = tc_eid;
	}
	return CCharacterBase::TouchC(info,tc_eid);
}


/*--------------------------------------------------------------------------------------
	GOBJMSG_ACTION ���b�Z�[�W ����
	�V�����s����ǉ������ꍇ�́A�����ɏ�����ǉ����܂��B
	�s���̑O��ɋ��ʏ�������ꂽ���ꍇ�� PreAction(),PostAction() ���I�[�o�[���C�h���Ă�������
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Action()
{
	switch(GetGObject()->aid){
	//�ǉ������s���J��
	case ACTID_DASHF			:act_dashf();			break;
	case ACTID_DASHB			:act_dashb();			break;
	case ACTID_2DANJAMPS		:act_2danjamps();		break;
	case ACTID_2DANJAMPC		:act_2danjampc();		break;
	case ACTID_2DANJAMPF		:act_2danjampf();		break;
	case ACTID_2DANJAMPB		:act_2danjampb();		break;
	case ACTID_STAFF			:act_staff();			ChainCombo(CHAIN_STAFF);break;
	case ACTID_NAGE1			:act_nage();			break;
	case ACTID_MOYAMOYA1		:act_moyamoya1();		break;
	case ACTID_MOYAMOYA2		:act_moyamoya2();		break;
	case ACTID_MOYAMOYA3		:act_moyamoya3();		break;
	case ACTID_POI_OCYA			:act_poi_ocya();		break;
	case ACTID_POI_JIEN			:act_poi_jien();		break;
	case ACTID_POI_AROE			:act_poi_aroe();		break;
	case ACTID_POI_MISSILE_H	:act_poi_missile_h();	break;
	case ACTID_POI_MISSILE_V	:act_poi_missile_v();	break;
	case ACTID_POI_MOUKONEEYO	:act_poi_moukoneeyo();	break;
	case ACTID_POI_MISSILE_SP	:act_poi_missileSP();	break;
	case ACTID_POI_HANDG		:act_poi_handg();		break;
	case ACTID_POI_CRAYMORE		:act_poi_craymore();	break;
	case ACTID_POI_FIRETHROW	:act_poi_firethrow();	break;
	case ACTID_POI_SHOT			:act_poi_shot();		break;
	case ACTID_POI_RIFLE		:act_poi_rifle();		break;
	case ACTID_POI_ROCKET		:act_poi_rocket();		break;
	case ACTID_POI_LANTERN		:act_poi_lantern();		break;
	case ACTID_POI_HELLFIRE		:act_poi_hellfire();	break;
	case ACTID_POI_HTTPLASER	:act_poi_httplaser();	break;
	case ACTID_POI_ERASER99		:act_poi_eraser99();	break;
	case ACTID_DAKKO			:act_dakko();			break;
	case ACTID_DAKKO_BACK		:act_dakko_back();		break;
	case ACTID_STRIKER_IN		:act_striker_in();		break;
	case ACTID_STRIKER_ATTACK	:act_striker_attack();	break;
	case ACTID_STRIKER_OUT		:act_striker_out();		break;
	case ACTID_REVERSAL			:act_reversal();		break;
	default:
		//��{�s��
		return CCharacterBase::Action();
	}

	return(TRUE);
}

void CCharacter::PreAction()
{
	//�_���[�W��炢���ŗ\�����ʍs���J�ڂ��N�������ꍇ�A��ѓ���̖�����
	if(!(pdat->aid & ACTID_MOYAMOYA))
	{
		if(pbul_moya)pbul_moya->ForceEnd();
	}
	if(pdat->aid!=ACTID_POI_RIFLE)
	{
		pbul_rifle->ForceStop();
	}
	if(pdat->aid!=ACTID_POI_FIRETHROW && pdat->aid!=ACTID_POI_HELLFIRE)
	{
		pbul_firethrower->ForceStop();
	}
	if(pdat->aid!=ACTID_POI_ERASER99)
	{
		pbul_eraser99->ForceStop();
	}

	//COM���o�[�T��
	if(pdat->aid==ACTID_OKIAGARI && m_opt_reversal)
	{
		if(ccounter==10)
		{
			if( IsLocalCom() && pdat->gauge>=NEEDGAUGE_REV)
			{
				if( GetMaai_H(pdat->id,pdat->eid) < 50 )
				{
					if(rand()%3)
					{
						ChangeAction( ACTID_REVERSAL );
					}
				}
			}
		}
	}

	//COM�������
	if( IsLocalCom() && ((pdat->aid&ACTID_MOYAMOYA) && pdat->counter>5))
	{
		UINT maai = GetMaai_H(pdat->id,pdat->eid);
		int t = rand()%50;
		if(t< ((maai<60) ? 6 : 1) )
		{
			ArekoreCancel();
		}
		else if(t<((maai<60) ? 12 : 2))
		{
			ArekoreRelease();
		}
	}

	CCharacterBase::PreAction();
}

/*--------------------------------------------------------------------------------------
	�U���͏�񏉊���
----------------------------------------------------------------------------------------*/
void CCharacter::InitAttackInfo()
{
	ZeroMemory(aif,sizeof(aif));

	int i;

	//�E��
	i=ATTACKINFO_STAND_A;//������U��
	aif[i].hit		=HITINFO_REACT1 | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1  | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_WEAK;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_STAND_B;//�������U��
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2  | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=40;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_STAND_C;//�������U��
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=0;//���

	//�E��

	i=ATTACKINFO_CROUCH_A;//���Ⴊ�ݎ�U��
	aif[i].hit		=HITINFO_REACT1 | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1  | GUARDINFO_XCROUCH | GUARDINFO_XJAMP ;//�K�[�h���
	aif[i].id		=ATTACK_WEAK;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_CROUCH_B;//���Ⴊ�ݒ��U��
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2  | GUARDINFO_XJAMP ;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=40;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_CROUCH_C;//���Ⴊ�݋��U��
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG ;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=0;//���

	//�E��

	i=ATTACKINFO_JAMP_A;//�W�����v��U��
	aif[i].hit		=HITINFO_REACT1 | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1  | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_WEAK;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_JAMP_B;//�W�����v���U��
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2  | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=40;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_JAMP_C;//�W�����v���U��
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV1 | HITINFO_MARK2 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=0;//���

	//�E��+C

	i=ATTACKINFO_STAFF;
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=0;//���

	//�E����

	i=ATTACKINFO_NAGE;
	aif[i].hit		=HITINFO_REACT3 | HITINFO_MARK3 | HITINFO_SNDHIT3 | HITINFO_EFCTSINDO;//�q�b�g���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=25;//�_���[�W
	aif[i].kezuri	=0;//���

	//�E��ѓ����

	i=ATTACKINFO_OCYA;//����
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 ;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=40;//�_���[�W
	aif[i].kezuri	=2;//���
	
	i=ATTACKINFO_JIEN;//�W�G��
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 ;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=60;//�_���[�W
	aif[i].kezuri	=3;//���
	
	i=ATTACKINFO_AROE;//�A���G
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=300;//�_���[�W
	aif[i].kezuri	=10;//���
	
	i=ATTACKINFO_MISSILE;//�~�T�C��
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 | HITINFO_EFCTBURN;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 ;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=60;//�_���[�W
	aif[i].kezuri	=6;//���

	i=ATTACKINFO_MOUKONEEYO;//���E�R�l�G��
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 ;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=3;//���

	i=ATTACKINFO_HANDG;//�n���h�O���l�[�h
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 | HITINFO_EFCTBURN;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP ;//�K�[�h���
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=60;//�_���[�W
	aif[i].kezuri	=6;//���

	i=ATTACKINFO_CRAYMORE;//�N���C���A
	aif[i].hit		=HITINFO_REACT2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_XJAMP ;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=20;//�_���[�W
	aif[i].kezuri	=1;//���

	i=ATTACKINFO_RIFLE;//���C�t��
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XSTAND | GUARDINFO_XCROUCH | GUARDINFO_XJAMP ;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=100;//�_���[�W
	aif[i].kezuri	=10;//���

	i=ATTACKINFO_FIRETHROWER;//�Ή����ˊ�
	aif[i].hit		=HITINFO_REACT2 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_XJAMP ;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=20;//�_���[�W
	aif[i].kezuri	=5;//���

	i=ATTACKINFO_ROCKET;//���P�b�g
	aif[i].hit		=HITINFO_EFCTSINDO | HITINFO_EFCTBURN | HITINFO_SIV3 | HITINFO_FUTTOBI | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_EFCTSINDO | GUARDINFO_SIV3 | GUARDINFO_REACT3| GUARDINFO_XJAMP ;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=250;//�_���[�W
	aif[i].kezuri	=25;//���

	i=ATTACKINFO_SHOT;//�V���b�g�K��
	aif[i].hit		=HITINFO_REACT2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT2;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=20;//�_���[�W
	aif[i].kezuri	=1;//���

	i=ATTACKINFO_AGI;//�A�M
	aif[i].hit		=HITINFO_SIV3 | HITINFO_REACT3 | HITINFO_MARK4 | HITINFO_SNDHIT3 | HITINFO_EFCTBURN ;//�q�b�g���
	aif[i].guard	=GUARDINFO_SIV3 | GUARDINFO_REACT3;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=3;//���

	i=ATTACKINFO_HTTPLASER;//���[�U�[
	aif[i].hit		=HITINFO_EFCTBURN_B | HITINFO_SIV1 | HITINFO_REACT3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_SIV3 | GUARDINFO_REACT1| GUARDINFO_XJAMP ;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=250;//�_���[�W
	aif[i].kezuri	=30;//���
	
	i=ATTACKINFO_ERASER99;//�C���C�T�[
	aif[i].hit		=HITINFO_EFCTBURN_B | HITINFO_EFCTSINDO | HITINFO_REACT3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3| GUARDINFO_XJAMP | GUARDINFO_XSTAND | GUARDINFO_XCROUCH ;//�K�[�h���
	aif[i].id		=0;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=9999;//�_���[�W
	aif[i].kezuri	=99;//���

	//�E�u���b�f�B�[�E�_�b�R

	i=ATTACKINFO_DAKKO;
	aif[i].hit		=HITINFO_REACT3 | HITINFO_MARK3 | HITINFO_SNDHIT3 | HITINFO_STOP;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XSTAND;//�K�[�h���
	aif[i].id		=ATTACK_DAKKO;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=450;//�_���[�W
	aif[i].kezuri	=45;//���

	//�E���o�[�T��

	i=ATTACKINFO_REVERSAL;
	aif[i].hit		=HITINFO_FUTTOBI2 | HITINFO_SIV1 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 | GUARDINFO_XSTAND |  GUARDINFO_XJAMP ;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=25;//�_���[�W
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
	COM�s���p �Z��� ������
----------------------------------------------------------------------------------------*/
void CCharacter::InitWazInfo()//�R���s���[�^�p�Z���̐ݒ�
{
	CCharacterBase::InitWazInfo();

	waz.dashf = ACTID_DASHF;
	waz.dashb = ACTID_DASHB;

	//pdat->winfo �֍s��ID��ǉ�����
	waz.att_short[0]  = ACTID_ATT_SA;//�Z���[�`�U��
	waz.att_short[1]  = ACTID_ATT_CA;
	waz.att_short[2]  = ACTID_ATT_SB;
	waz.att_short[3]  = ACTID_ATT_CB;
	waz.att_short[4]  = ACTID_NAGE1;

	waz.att_middle[0] = ACTID_ATT_SB;//�����[�`�U��
	waz.att_middle[1] = ACTID_ATT_CB;
	waz.att_middle[2] = ACTID_ATT_SC;
	waz.att_middle[3] = ACTID_ATT_CC;
	waz.att_middle[4] = ACTID_STAFF;
	waz.att_middle[5] = ACTID_DAKKO;

	waz.att_long[0]   = ACTID_ATT_SC;//�����[�`�U�� 
	waz.att_long[1]   = ACTID_ATT_CC;
	waz.att_long[2]   = ACTID_MOYAMOYA1;
	waz.att_long[3]   = ACTID_MOYAMOYA2;
	waz.att_long[4]   = ACTID_DAKKO;

	waz.att_bullet[0] = ACTID_MOYAMOYA1;//��ѓ���
	waz.att_bullet[1] = ACTID_MOYAMOYA2;//��ѓ���
	waz.att_bullet[2] = ACTID_MOYAMOYA3;//��ѓ���

	waz.att_tai[0]    = ACTID_STAFF; //�΋�

	//���[�`�ݒ�
	SetComReach( 0,MAAI_SHORT	);
	SetComReach( 1,MAAI_MIDDLE );
	SetComReach( 2,MAAI_LONG );
}


