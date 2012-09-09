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
	{ OPTIONS_REVERSAL			,0						,0, "Reversal Attack"	, 5 },
	{ OPTIONS_GCANCEL			,0						,0, "Guard Cancel"		, 5 },
	{ OPTIONS_QUICK_CHARGE		,OPTIONS_STOCKABLE_GAUGE,0, "Quick Charge"		, 7 },
	{ OPTIONS_STOCKABLE_GAUGE	,OPTIONS_QUICK_CHARGE	,0, "Stockable Gauge"	, 7 },
	{ OPTIONS_HEAVY_HIT      ,0            ,0, "Heavy Hit"      , 7 },
	{ OPTIONS_NO_CHAIN_COMBO	,0						,0, "No Chain Comb"		,-5 }
};


//**************************************************************************************
//  Character Information
//�@�����ɃL�����N�^�[�̖��O��L�q���܂�
//**************************************************************************************

CCharacterInfo CharInfo("�M�R",		// �L�����N�^�[�̖��O�i�ő�31�o�C�g�j
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
	//�I�v�V�����l���
	m_opt_reversal	= (option_flags&OPTIONS_REVERSAL		) ? TRUE : FALSE;
	m_opt_gcancel	= (option_flags&OPTIONS_GCANCEL			) ? TRUE : FALSE;
	m_opt_quick_ch	= (option_flags&OPTIONS_QUICK_CHARGE	) ? TRUE : FALSE;
	m_opt_hevy_hit	= (option_flags&OPTIONS_HEAVY_HIT		) ? TRUE : FALSE;
	
	BOOL st_gauge	= (option_flags&OPTIONS_STOCKABLE_GAUGE	) ? TRUE : FALSE;
	chainComboEnabled=(option_flags&OPTIONS_NO_CHAIN_COMBO	) ? FALSE : TRUE;
	BOOL iron		= FALSE;

	pdat->hpmax			= iron ? 1200 : 1000;	//�ő�̗͒l
	pdat->gaugemax		= st_gauge ? 4 : 1;		//�ő�Q�[�W
	pdat->gaugetype		= st_gauge ? GOBJFLGA_POWGAUGE1 : GOBJFLGA_POWGAUGE2;//�Q�[�W�^�C�v
}


/*--------------------------------------------------------------------------------------
	���b�Z�[�W����
	�x�[�X�N���X�ŏE��Ă��Ȃ����b�Z�[�W��E�������ꍇ�́A�R�R�ɏ�����ǉ���܂�
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
		case GOBJMSG_NAGE:			return eact_nage(pobjdat);
		case GOBJMSG_ATEMI_NAGE:	return eact_atemi_nage(pobjdat);
		case GOBJMSG_FUTTOBI:		return eact_rotfuttobi(pobjdat);
		}
		return TRUE;
	}

	switch(msg){
	case GOBJMSG_STRIKER://�x���U���v��
		if(pdat->aid == ACTID_TAIKICYU){
			RemoveProperty( GOBJFLG_TOBETARGET );//�^�[�Q�b�g�ɂ͂Ȃ�Ȃ�...�Ȃ�ł���O���񂾂���H(�Y)
			ChangeAction( ACTID_STRIKER_IN );
			return(YEAH);
		}
		return(0);
	default:
		return(CCharacterBase::Message(msg,dat,prm));//�L�����N�^�[��{�N���X�ɏ�����C����
	}
	return(TRUE);
}


/*--------------------------------------------------------------------------------------
	GOBJMSG_TOUCHA(�U����炢)
----------------------------------------------------------------------------------------*/
DWORD CCharacter::TouchA(ATTACKINFO *info,DWORD eid)
{
	GOBJECT *einfo;

	//���Đg��������
	if(pdat->aid==ACTID_ATEMI1 || pdat->aid==ACTID_ATEMI2)
	{
		DWORD efctid;

		//�G���擾
		einfo = GetInfo(eid);
		if(!einfo){
			OutputDebugString("���g:info�擾���s\n");
			return CCharacterBase::TouchA(info,eid);
		}

	/*	//���肪�L�����N�^�[���H
		if(FALSE){//(einfo->objtype&GOBJFLG_ZCHARACTER)==0){
			OutputDebugString("���g:���肪�L�����N�^�łȂ�\n");
			return CCharacterBase::TouchA(info,eid);
		}

		//���肪������ꔻ����Ă��邩�H
		if((einfo->objtype&GOBJFLG_NAGERARE)==0){
			OutputDebugString("���g:������ꔻ��Ȃ�(1)\n");
			return CCharacterBase::TouchA(info,eid);
		}
		if(!einfo->nagerare){
			OutputDebugString("���g:������ꔻ��Ȃ�(2)\n");
			return CCharacterBase::TouchA(info,eid);
		}*/

		//���l�̗͈͑ȏォ�H
		if(info->kezuri > (DWORD)pdat->hp){
			OutputDebugString("���g:HP�s��\n");
			return TOUCHA_GUARDS;
		}

		efctid = (info->kezuri!=0) ? EFCTID_MARKG2 : EFCTID_MARKG;

		if(pdat->aid==ACTID_ATEMI1)
		{
			//�����K�[�h�ł��Ȃ���͎̂󂯎��Ȃ�
			if(info->guard & GUARDINFO_XSTAND){
				OutputDebugString("���g:�����K�[�h�s�\\n");
				return CCharacterBase::TouchA(info,eid);
			}

			//�����Ă݂�
			if(!ObjCatch(eid, GOBJMSG_ATEMI_NAGE)){
			OutputDebugString("���g:�������s\n");
				return CCharacterBase::TouchA(info,eid);
			}

			//����B�K�[�h�}�[�N��\������
			AddEffect(efctid, (int)(pdat->x+zurex(35)) , -60 , pdat->muki );
		}
		else if(pdat->aid==ACTID_ATEMI2)
		{
			//���Ⴊ�݃K�[�h�ł��Ȃ���͎̂󂯎��Ȃ�
			if(info->guard & GUARDINFO_XCROUCH){
				OutputDebugString("���g:��K�[�h�s�\\n");
				return CCharacterBase::TouchA(info,eid);
			}

			//�����Ă݂�
			if(!ObjCatch(eid, GOBJMSG_ATEMI_NAGE)){
				OutputDebugString("���g:�������s\n");
				return CCharacterBase::TouchA(info,eid);
			}

			//����B�K�[�h�}�[�N��\������
			AddEffect(efctid, (int)(pdat->x+zurex(35)) , -30 , pdat->muki );
		}

		AddEffect(EFCTID_STOP, 8, 0,0);	//�q�b�g�X�g�b�v �G�t�F�N�g
		PlaySysSound(SYSTEMSOUND_GUARD);//�K�[�h��
		pdat->hp -= info->kezuri;		//���K�p
		DrawFront();					//�O�ʕ`��
		ChangeAction(ACTID_ATEMI_NAGE);	//�s���J��

		return TOUCHA_AVOID;
	}
	
	return CCharacterBase::TouchA(info,eid);
}

/*--------------------------------------------------------------------------------------
	GOBJMSG_TOUCHB(�U��������)
----------------------------------------------------------------------------------------*/
DWORD CCharacter::TouchB(ATTACKINFO *info,BOOL hit)
{
	//���s���J��
	if(info->id == ATTACK_STRIKER){
		ChangeAction(ACTID_STRIKER_CHAINSAW);
	}

	double dp = 0.0f;
	switch(info->id){
		case ATTACK_WEAK	:dp=0.02;break;
		case ATTACK_MIDDLE	:dp=0.03;break;
		case ATTACK_STRONG	:dp=0.04;break;
		case ATTACK_HISSATU	:dp=0.05;break;
		case ATTACK_ATEMI	:dp=0.10;break;
	}
	if(!hit)dp*=0.5;
	if(m_opt_quick_ch)dp*=1.5;
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
					SetComAct(ACTID_ATT_SC,5);
				}
			}
			break;
		case ACTID_ATT_SC:
			{
				if(ComLevelCk(3)){
					SetComAct(ACTID_KIRIGET,3);
				}
			}
			break;
		case ACTID_KIRIGET:
			{
				if(ComLevelCk(5) && pdat->gauge>=1.0f)
				{
					SetComAct(ACTID_CYOHI,3);
				}

				if(ComLevelCk(3)){
					SetComAct(ACTID_ATT_CA,5);
				}
			}
			break;
		case ACTID_ATT_CA:
			{
				if(ComLevelCk(3)){
					SetComAct(ACTID_ATT_CB,5);
				}
			}
			break;
		case ACTID_ATT_CB:
			{
				if(ComLevelCk(3)){
					SetComAct(ACTID_ATT_CC,5);
				}
			}
			break;
		case ACTID_ATT_CC:
			{
				DWORD maai = GetMaai_H(pdat->id, pdat->eid);
				if(ComLevelCk(3)){this->
					SetComAct(ACTID_TOATE2,1);
				}
			}
			break;
		}
	}

	return CCharacterBase::TouchB(info,hit);
}

/*--------------------------------------------------------------------------------------
	�R�}���h���� ����
	GOBJMSG_COMMAND ���b�Z�[�W�ɑΉ����܂��B���݂̍s���ɂ��ĕ����̊֐���
	������������Ă��܂��B�K�؂ȂƂ���ɔ��菈����ǉ���Ă��������B
----------------------------------------------------------------------------------------*/
BOOL CCharacter::Command_Hissatu(DWORD key)
{
	if(pdat->aid & ACTID_KUCYU){//�󒆕K�E�Z�̃R�}���h����
	}
	else//�n��K�E�Z�̃R�}���h����
	{
		//���K
		if(com1632143(50)){
			if( (key & KEYSTA_BC2) && pdat->gauge>=1.0 )
			{
				pdat->aid = ACTID_CYOHI;
				return TRUE;
			}
		}

		//���Đg����
		if(com426(30))
		{
			if(key & KEYSTA_BC2){pdat->aid = ACTID_ATEMI3;return TRUE;}
			else if(key & KEYSTA_BB2){pdat->aid = ACTID_ATEMI2;return TRUE;}
			else if(key & KEYSTA_BA2){pdat->aid = ACTID_ATEMI1;return TRUE;}
		}

		//age�^�b�N��
		if(com623(30))
		{
			if(key & KEYSTA_BC2){pdat->aid = ACTID_AGETACKLE3;return TRUE;}
			else if(key & KEYSTA_BB2){pdat->aid = ACTID_AGETACKLE2;return TRUE;}
			else if(key & KEYSTA_BA2){pdat->aid = ACTID_AGETACKLE1;return TRUE;}
		}

		//������
		if(com421(30) && pbul_toate->isReady())
		{
			if(key & KEYSTA_BC2){pdat->aid = ACTID_TOATE3;return TRUE;}
			else if(key & KEYSTA_BB2){pdat->aid = ACTID_TOATE2;return TRUE;}
			else if(key & KEYSTA_BA2){pdat->aid = ACTID_TOATE1;return TRUE;}
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
		pdat->aid = ACTID_WALKB;
	}
	else{//�j���[�g�����|�[�Y
		pdat->aid = ACTID_NEUTRAL;
	}
	return FALSE;
}

BOOL CCharacter::Command_OnNormal(DWORD key)
{
	if(pdat->aid & ACTID_KUCYU){////�W�����v���쒆
	}
	else{//�n�㓮�쒆
		//����
		if (key & KEYSTA_FOWORD && key & KEYSTA_BC2 &&
			NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE)) {
				pdat->aid = ACTID_NAGE1;
				return TRUE;
		}

		//�L���o���Q�b�g�i�_+C�j
		if((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD) && (key & KEYSTA_DOWN)){
			ChangeAction( ACTID_KIRIGET );return TRUE;
		}
		//sage�i�b�N���i��+B�j
		if((key & KEYSTA_BB2) && (key & KEYSTA_FOWORD) && !(key & KEYSTA_DOWN)){
			ChangeAction( ACTID_SAGENA );return TRUE;
		}
	}

	return CCharacterBase::Command_OnNormal(key);
}

BOOL CCharacter::Command_OnAttacking(DWORD key)
{	
	if(!( pdat->aid & ACTID_KUCYU ))
	{
		//�L���o���Q�b�g�i�_+C�j
		if((key & KEYSTA_BC2) && (key & KEYSTA_FOWORD) && (key & KEYSTA_DOWN))
		{
			if(ChainCombo(CHAIN_KIRIGET))
			{
				ChangeAction(  ACTID_KIRIGET );return TRUE;
			}
		}
		//sage�i�b�N���i��+B�j
		if((key & KEYSTA_BB2) && (key & KEYSTA_FOWORD) && !(key & KEYSTA_DOWN))
		{
			if(ChainCombo(CHAIN_SAGENA))
			{
				ChangeAction( ACTID_SAGENA );return TRUE;
			}
		}
	}

	return CCharacterBase::Command_OnAttacking(key);
}

BOOL CCharacter::Command_OnSystem(DWORD key)
{
	if(!(pdat->aid & ACTID_KUCYU))
	{
		//�K�[�L����
		if(m_opt_gcancel)
		{
			if((pdat->aid & ACTID_GUARD) && pdat->gauge>NEEDGAUGE_GC)
			{
				if(key&KEYSTA_BC2)
				{
					ChangeAction( ACTID_GCANCEL3 );
					return TRUE;
				}
				else if(key&KEYSTA_BB2)
				{
					ChangeAction( ACTID_GCANCEL2 );
					return TRUE;
				}
				else if(key&KEYSTA_BA2)
				{
					ChangeAction( ACTID_GCANCEL1 );
					return TRUE;
				}
			}
		}
	}
	//���o�[�T��
	if(pdat->aid==ACTID_OKIAGARI && m_opt_reversal)
	{
		if(ccounter==10 && pdat->gauge>NEEDGAUGE_REV)
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
	if(wid==ACTID_CYOHI && pdat->gauge<1.0f)return FALSE;//�Q�[�W�`�F�b�N

	if(wid==ACTID_GCANCEL1 || wid==ACTID_GCANCEL2 || wid==ACTID_GCANCEL3)
	{
		if(pdat->gauge<NEEDGAUGE_GC)return FALSE;
		if(!((pdat->aid&ACTID_GUARD) && m_opt_gcancel))return FALSE;
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

DWORD CCharacter::CmdCom_OnNormal(DWORD wid)
{
	if(!(pdat->aid&ACTID_KUCYU))
	{
		//����
		if (wid==ACTID_NAGE1 && NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE)) {
			pdat->aid = ACTID_NAGE1;
			return TRUE;
		}
		//�`�F�[���R���{����(�ǉ�)
		if(GetGObject()->aid & ACTID_ATTACK)
		{
			switch(wid)
			{
			case ACTID_KIRIGET	:return ChainCombo(CHAIN_KIRIGET);break;
			case ACTID_SAGENA	:return ChainCombo(CHAIN_SAGENA);break;
			}
		}
	}

	// �΋�Z
	if (wid==ACTID_AGETACKLE1 || wid==ACTID_AGETACKLE2 || wid==ACTID_AGETACKLE3)
		return TRUE;

	return CCharacterBase::CmdCom_OnNormal(wid);
}

/*--------------------------------------------------------------------------------------
	GOBJMSG_ACTION ���b�Z�[�W ����
	�V�����s����ǉ�����ꍇ�́A�����ɏ�����ǉ���܂��B
	�s���̑O��ɋ��ʏ������ꂽ���ꍇ�� PreAction(),PostAction() ��I�[�o�[���C�h���Ă�������
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Action()
{
	switch(GetGObject()->aid)
	{
	case ACTID_NAGE1			:act_nage();			break;
	case ACTID_DASHF			:act_dashf();			break;
	case ACTID_DASHB			:act_dashb();			break;
	case ACTID_AGETACKLE1		:act_agetackle1(ATTACKINFO_AGETACKLE1);		break;
	case ACTID_AGETACKLE2		:act_agetackle2(ATTACKINFO_AGETACKLE2);		break;
	case ACTID_AGETACKLE3		:act_agetackle3(ATTACKINFO_AGETACKLE3);		break;
	case ACTID_AGETACKLESUKI	:act_agetacklesuki();	break;
	case ACTID_KIRIGET			:act_kiriget();			ChainCombo(CHAIN_KIRIGET);break;
	case ACTID_SAGENA			:act_sagena();			ChainCombo(CHAIN_SAGENA);break;
	case ACTID_SAGENASUKI		:act_sagenasuki();		break;
	case ACTID_ATEMI1			:act_atemi1();			break;
	case ACTID_ATEMI2			:act_atemi2();			break;
	case ACTID_ATEMI3			:act_atemi3();			break;
	case ACTID_ATEMI_SUKI		:act_atemi_suki();		break;
	case ACTID_ATEMI_NAGE		:act_atemi_nage();		break;
	case ACTID_CYOHI			:act_cyohi();			break;
	case ACTID_TOATE1			:act_toate1();			break;
	case ACTID_TOATE2			:act_toate2();			break;
	case ACTID_TOATE3			:act_toate3();			break;
	case ACTID_STRIKER_IN		:act_striker_in();		break;
	case ACTID_STRIKER_2GET		:act_striker_2get();	break;
	case ACTID_STRIKER_CHAINSAW	:act_striker_chainsaw();break;
	case ACTID_STRIKER_OUT		:act_striker_out();		break;
	case ACTID_GCANCEL1			:act_gcancel1();		break;
	case ACTID_GCANCEL2			:act_gcancel2();		break;
	case ACTID_GCANCEL3			:act_gcancel3();		break;
	case ACTID_REVERSAL			:act_reversal();		break;
	default://����ȊO�̓L�����N�^�[��{�N���X�ɔC����
		return CCharacterBase::Action();
	}

	return(TRUE);
}

void CCharacter::PreAction()
{
	//COM���o�[�T��
	if(pdat->aid==ACTID_OKIAGARI && m_opt_reversal)
	{
		if(ccounter==10)
		{
			if( IsLocalCom()  && pdat->gauge>NEEDGAUGE_REV)
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

	CCharacterBase::PreAction();
}

/*--------------------------------------------------------------------------------------
	�U���͏�񏉊�
----------------------------------------------------------------------------------------*/
void CCharacter::InitAttackInfo()
{
	ZeroMemory(aif,sizeof(aif));

	int i;

	//�E����

	i=ATTACKINFO_STAND_A;//������U��
	aif[i].hit		=HITINFO_REACT1 | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1 ;//�q�b�g���
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
	aif[i].hit		=HITINFO_MARK2 | HITINFO_REACT2 | HITINFO_SNDHIT3;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=25;//�_���[�W
	aif[i].kezuri	=0;//���

	//�E��

	i=ATTACKINFO_CROUCH_A;//���Ⴊ�ݎ�U��
	aif[i].hit		=HITINFO_REACT1 | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1  | GUARDINFO_XJAMP | GUARDINFO_XSTAND;//�K�[�h���
	aif[i].id		=ATTACK_WEAK;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_CROUCH_B;//���Ⴊ�ݒ��U��
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2  | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=40;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_CROUCH_C;//���Ⴊ�݋��U��
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG ;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=0;//���

	//�E��

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
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=0;//���

	//�E��

	i=ATTACKINFO_NAGE;//�����i�ʏ�j
	aif[i].hit		= HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=80;//�_���[�W
	aif[i].kezuri	=0;//���

	//�L���o���Q�b�g
	i=ATTACKINFO_KIRIGET;//���Ⴊ�݋��U��
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XSTAND | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG ;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=0;//���

	//age�^�b�N��

	i=ATTACKINFO_AGETACKLE1;
	aif[i].hit		= HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1 | GUARDINFO_XJAMP;
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=2;//���

	i=ATTACKINFO_AGETACKLE2;
	aif[i].hit		= HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1;
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=2;//���
	
	i=ATTACKINFO_AGETACKLE3;
	aif[i].hit		= HITINFO_MARK4 | HITINFO_FUTTOBI2 | HITINFO_SNDHIT3 | HITINFO_SIV1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV1;
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=2;//���

	//sage�i�b�N��
	i=ATTACKINFO_SAGENA;//���Ⴊ�݋��U��
	aif[i].hit		=HITINFO_REACT3A | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=40;//�_���[�W
	aif[i].kezuri	=0;//���

	//�E���Đg����

	i=ATTACKINFO_ATEMI_NAGE;//�����i�ʏ�j
	aif[i].hit		= HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].id		=ATTACK_ATEMI;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=90;//�_���[�W
	aif[i].kezuri	=0;//���

	//�E���K

	i=ATTACKINFO_CYOHI;
	aif[i].hit		= HITINFO_MARK4 | HITINFO_USER2(USERHIT_ROTFUTTOBI) | HITINFO_SNDHIT3 | HITINFO_EFCTSINDO;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_XJAMP;
	aif[i].id		=0;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=500;//�_���[�W
	aif[i].kezuri	=50;//���

	
	//�E������

	i=ATTACKINFO_TOATE;
	aif[i].hit		= HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_EFCTSINDO;//�q�b�g���
	aif[i].guard	= GUARDINFO_REACT3 | GUARDINFO_XJAMP;
	aif[i].id		=ATTACK_HISSATU;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=100;//�_���[�W
	aif[i].kezuri	=10;//���

	//�E�X�g���C�J�[

	//�L���o���Q�b�g
	i=ATTACKINFO_STRIKER2GET;
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV3 | GUARDINFO_XSTAND | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRIKER ;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=10;//�_���[�W
	aif[i].kezuri	=0;//���

	//�`�F�C���\�[
	i=ATTACKINFO_STRIKERCHAIN;
	aif[i].hit		=HITINFO_MARK3 | HITINFO_REACT3A | HITINFO_SNDHIT3;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3;//�K�[�h���
	aif[i].id		=0;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=8;//�_���[�W
	aif[i].kezuri	=0;//���

	//�E�K�[�h�L�����Z��
	aif[ATTACKINFO_GCANCEL] = aif[ATTACKINFO_AGETACKLE2];
	i=ATTACKINFO_GCANCEL;
	aif[i].id		=0;//�Q�[�W������Ɏg�p����ID
	aif[i].kezuri	=0;//���
	
	//�E���o�[�T��
	aif[ATTACKINFO_REVERSAL] = aif[ATTACKINFO_AGETACKLE2];
	i=ATTACKINFO_GCANCEL;
	aif[i].id		=0;//�Q�[�W������Ɏg�p����ID
	aif[i].kezuri	=0;//���

	if(m_opt_hevy_hit)
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

	waz.dashf = ACTID_DASHF;
	waz.dashb = ACTID_DASHB;

	waz.att_short[0] = ACTID_ATT_SA;
	waz.att_short[1] = ACTID_ATT_CA;
	waz.att_short[2] = ACTID_ATT_SB;
	waz.att_short[3] = ACTID_CYOHI;
	waz.att_short[4] = ACTID_GCANCEL2;
	waz.att_short[5] = ACTID_ATEMI2;
	
	waz.att_middle[0] = ACTID_ATT_SB;
	waz.att_middle[1] = ACTID_ATT_SC;
	waz.att_middle[2] = ACTID_ATT_CB;
	waz.att_middle[3] = ACTID_ATT_CC;
	waz.att_middle[4] = ACTID_CYOHI;
	waz.att_middle[5] = ACTID_KIRIGET;

	waz.att_long[0] = ACTID_KIRIGET;
	waz.att_long[1] = ACTID_ATT_CC;
	waz.att_long[2] = ACTID_ATT_CB;
	waz.att_long[3] = ACTID_TOATE2;
	waz.att_long[4] = ACTID_TOATE1;

	waz.att_jamp[0] = ACTID_ATT_JA;
	waz.att_jamp[1] = ACTID_ATT_JB;
	waz.att_jamp[2] = ACTID_ATT_JC;

	waz.att_bullet[0] = ACTID_TOATE3;
	
	waz.att_tai[0] = ACTID_AGETACKLE1;
	waz.att_tai[1] = ACTID_ATEMI1;
	waz.att_tai[2] = ACTID_AGETACKLE2;
	waz.att_tai[3] = ACTID_AGETACKLE3;
	waz.att_tai[4] = ACTID_CYOHI;

	waz.nageid[0] = ACTID_NAGE1;

	//���[�`�ݒ�
	SetComReach( 0,MAAI_SHORT	);
	SetComReach( 1,MAAI_MIDDLE );
	SetComReach( 2,MAAI_LONG );
}

/*--------------------------------------------------------------------------------------
	���C�W���O�X�g�[���R�}���h
----------------------------------------------------------------------------------------*/
BOOL CCharacter::com1632143(int dt)
{
	int ofst=0;
	
	ofst=SeekKey(ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	
	ofst=SeekKey(ofst,dt,KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=SeekKey(ofst,dt,KEYSTA_DOWN);
	if(ofst<0)return(FALSE);
	/*ofst=SeekKey(ofst,dt,KEYSTA_DOWN | KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=SeekKey(ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);*/
	ofst=SeekKey(ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);

	ofst=SeekKey(ofst,dt,KEYSTA_DOWN | KEYSTA_BACK);
	if(ofst<0)return(FALSE);

	return(TRUE);
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
