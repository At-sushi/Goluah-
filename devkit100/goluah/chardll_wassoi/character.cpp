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
	{ OPTIONS_AUTO_GUARD	, 0, 0, "Auto Guard"	, 8 },
	{ OPTIONS_GUARD_CANCEL	, 0, 0, "Guard Cancel"	, 5 },
	{ OPTIONS_KOROKORO		, 0, 0, "KoroKoro"		, 5 },
	{ OPTIONS_REGENERATOR	, 0, 0, "Regenerator"	, 8 },
};


//**************************************************************************************
//  Character Information
//�@�����ɃL�����N�^�[�̖��O��L�q���܂�
//**************************************************************************************

CCharacterInfo CharInfo("���ɂ���",			// �L�����N�^�[�̖��O�i�ő�31�o�C�g�j
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
	pdat->hpmax			= 1000;//�ő�̗͒l
	pdat->gaugemax		= 1;//�ő�Q�[�W
	pdat->gaugetype		= GOBJFLGA_POWGAUGE2;//�Q�[�W�^�C�v

	option_flag = CCharacterBase::option_flags;

	//�I��I�v�V����
	//HP+
	if(FALSE){
		pdat->hpmax = (int)(pdat->hpmax*1.2);
	}
	//auto guard
	if(option_flag & OPTIONS_AUTO_GUARD){
		isAutoGuard = TRUE;
	}

	hp_to_regenerate = 0;
	ZeroMemory(m_blur, sizeof(m_blur));
	m_blur_target = 0;
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
	case GOBJMSG_FESTIVAL_NORI_HIT://�C�ۃq�b�g
		if(pdat->hp > 0 && !(pdat->aid&ACTID_SYSTEM)){
			ChangeAction(ACTID_FESTIVALDASH);
		}
		break;
	case GOBJMSG_STRIKER://�x���U���v��
		if(pdat->aid == ACTID_TAIKICYU){
			pdat->objtype &= ~GOBJFLG_TOBETARGET;//�^�[�Q�b�g�ɂ͂Ȃ�Ȃ�
			ChangeAction( ACTID_STRIKER1);
			return(TRUE);
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
	if(pdat->aid==ACTID_OKIAGARI && pdat->counter==2 && 
		pdat->gauge>=NEEDGAUGE_REV && OptKoroKoro())
	{
		if(key & KEYSTA_FOWORD)
		{
			ChangeAction(ACTID_OKIAGARI_FOWORD);
			pdat->muteki = TRUE;
			pdat->nagerare = FALSE;
			return TRUE;
		}
		else if(key & KEYSTA_BACK)
		{
			ChangeAction(ACTID_OKIAGARI_BACK);
			pdat->muteki = TRUE;
			pdat->nagerare = FALSE;
			return TRUE;
		}
	}

	if((pdat->aid & ACTID_GUARD) && OptGuardCancel()){
		if(!(pdat->aid & ACTID_KUCYU))
		{
			//�n��K�[�h��
			//�K�[�h�L�����Z��
			if(pdat->gauge>=NEEDGAUGE_GC){
				if(key & KEYSTA_BC2){pdat->aid = ACTID_GCANCEL3;return TRUE;}
				else if(key & KEYSTA_BB2){pdat->aid = ACTID_GCANCEL2;return TRUE;}
				else if(key & KEYSTA_BA2){pdat->aid = ACTID_GCANCEL1;return TRUE;}
			}
		}
	}

	return CCharacterBase::Command_OnSystem(key);
}

BOOL CCharacter::Command_Hissatu(DWORD key_now)
{
	//�K�E�Z�R�}���h����
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_KUCYU){//�󒆕K�E�Z�̃R�}���h����
	}
	else{//�n��K�E�Z�̃R�}���h����
		if(pdat->gauge==1.0 && com2363214(60) && (key_now & KEYSTA_BC2)){//���ɂ���t�F�X�e�B�o��
			pdat->aid = ACTID_FESTIVAL;return TRUE;
		}
		if(com623(30)){//�����p
			if(key_now & KEYSTA_BC2){pdat->aid = ACTID_WASSYORYUKEN3;return TRUE;}
			else if(key_now & KEYSTA_BB2){pdat->aid = ACTID_WASSYORYUKEN2;return TRUE;}
			else if(key_now & KEYSTA_BA2){pdat->aid = ACTID_WASSYORYUKEN1;return TRUE;}
		}
		if(com214(30)){//�����`
			if(key_now & KEYSTA_BC2){pdat->aid = ACTID_SYARA3;return TRUE;}
			else if(key_now & KEYSTA_BB2){pdat->aid = ACTID_SYARA2;return TRUE;}
			else if(key_now & KEYSTA_BA2){pdat->aid = ACTID_SYARA1;return TRUE;}
		}
		if(b_nori && b_nori->isReady() && com236(30)){//�C��
			if(key_now & KEYSTA_BC2){pdat->aid = ACTID_HADOU3;return TRUE;}
			else if(key_now & KEYSTA_BB2){pdat->aid = ACTID_HADOU2;return TRUE;}
			else if(key_now & KEYSTA_BA2){pdat->aid = ACTID_HADOU1;return TRUE;}
		}
	}

	return CCharacterBase::Command_Hissatu(key_now);
}

BOOL CCharacter::Command_Normal(DWORD key)
{
	//�_�b�V���R�}���h��
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
		//if(key & KEYSTA_BACK && pdat->aid==ACTID_DASHB)return TRUE;//�_�b�V���p��
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

	if(pdat->aid & ACTID_KUCYU){//�W�����v���쒆
	}
	else{//�n�㓮�쒆
		//����
		if (keyinfo & KEYSTA_FOWORD && keyinfo & KEYSTA_BC2 &&
			NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE)) {
				pdat->aid = ACTID_NAGE1;
				return TRUE;
		}

		// ��+B
		if((keyinfo&KEYSTA_FOWORD) && (keyinfo&KEYSTA_BB2))
		{
			ChangeAction(ACTID_ATT_SMC);
			return TRUE;
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
		// ��+B
		if((key & KEYSTA_BB2) && (key & KEYSTA_FOWORD) && ChainCombo(CHAIN_SMC)){
			ChangeAction(ACTID_ATT_SMC);
			return TRUE;
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
	if(wid==ACTID_FESTIVAL && pdat->gauge<1.0f)return FALSE;

	if(wid==ACTID_GCANCEL1 || wid==ACTID_GCANCEL2 || wid==ACTID_GCANCEL3)
	{
		if(!(pdat->aid & ACTID_GUARD))
		{
			return FALSE;
		}
	}

	return CCharacterBase::CommandCOM(wid);
}

DWORD CCharacter::CmdCom_OnSystem(DWORD wid)
{
	//�K�[�L����
	if(wid==ACTID_GCANCEL1 || wid==ACTID_GCANCEL2 || wid==ACTID_GCANCEL3)
	{
		if((pdat->aid & ACTID_GUARD) && !(pdat->aid&ACTID_KUCYU) && pdat->gauge>=NEEDGAUGE_GC && OptGuardCancel())
		{
			return TRUE;
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
			case ACTID_ATT_SMC	:return ChainCombo(CHAIN_SMC);break;
			}
		}
	}

	// �΋�Z
	if (wid==ACTID_WASSYORYUKEN1 || wid==ACTID_WASSYORYUKEN2 || wid==ACTID_WASSYORYUKEN3)
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
	switch(GetGObject()->aid){
	case ACTID_ATT_SMC			:act_att_smc(); ChainCombo(CHAIN_SMC); break;
	case ACTID_WASSYORYUKEN1	:act_wassoi1();			break;
	case ACTID_WASSYORYUKEN2	:act_wassoi2();			break;
	case ACTID_WASSYORYUKEN3	:act_wassoi3();			break;
	case ACTID_WASSYORYUKENF	:act_wassoif();			break;
	case ACTID_HADOU1			:act_hadou1();			break;
	case ACTID_HADOU2			:act_hadou2();			break;
	case ACTID_HADOU3			:act_hadou3();			break;
	case ACTID_DASHF			:act_dashf();			break;
	case ACTID_DASHB			:act_dashb();			break;
	case ACTID_SYARA1			:act_syara1();			break;
	case ACTID_SYARA2			:act_syara2();			break;
	case ACTID_SYARA3			:act_syara3();			break;
	case ACTID_SYARAF			:act_syaraf();			break;
	case ACTID_FESTIVAL			:act_festival();		break;
	case ACTID_FESTIVALDASH		:act_festivaldash();	break;
	case ACTID_FESTIVALSUKI		:act_festivalsuki();	break;
	case ACTID_FESTIVALRUSH		:act_festivalrush();	break;
	case ACTID_FESTIVALFINISH	:act_festivalfinish();	break;
	case ACTID_GCANCEL1			:act_gcancel1();		break;
	case ACTID_GCANCEL2			:act_gcancel2();		break;
	case ACTID_GCANCEL3			:act_gcancel3();		break;
	case ACTID_NAGE1			:act_nage();			break;
	case ACTID_OKIAGARI_FOWORD	:act_okiagari_foword();	break;
	case ACTID_OKIAGARI_BACK	:act_okiagari_back();	break;
	case ACTID_STRIKER1			:act_striker1();		break;
	case ACTID_STRIKER2			:act_striker2();		break;
	case ACTID_STRIKER3			:act_striker3();		break;
	case ACTID_STRIKER4			:act_striker4();		break;
	default://����ȊO�̓L�����N�^�[��{�N���X�ɔC����
		return CCharacterBase::Action();
	}

	return(TRUE);
}

void CCharacter::PreAction()
{
	//HP�̉�
	if(OptRegenerator() && pdat->hp>5 && hp_to_regenerate>0)
	{
		int regene_rate = 10;

		if(
				!(pdat->aid & ACTID_ATTACK) && 
				!(pdat->aid & ACTID_SYSTEM) && 
				!(pdat->aid & ACTID_KUCYU)  && 
				(pdat->aid != ACTID_TOJYO) && 
				(pdat->aid != ACTID_SYORI) 
		)
		{
			if((pdat->counter%regene_rate)==0){
				pdat->hp += 1;
				hp_to_regenerate--;
			}
			if(pdat->hp > (int)pdat->hpmax)pdat->hp = pdat->hpmax;
		}
		else
		{
			if((pdat->counter%(regene_rate*3))==0){
				pdat->hp += 1;
				hp_to_regenerate--;
			}
			if(pdat->hp > (int)pdat->hpmax)pdat->hp = pdat->hpmax;
		}
	}

	// �u���[�ǉ�
	m_blur[m_blur_target].cnum = pdat->cnow;
	m_blur[m_blur_target].x = pdat->x;
	m_blur[m_blur_target].y = pdat->y;
	m_blur[m_blur_target].z = pdat->z;
	m_blur[m_blur_target].rot = pdat->rot;
	m_blur[m_blur_target].revx = pdat->revx != pdat->muki;
	m_blur[m_blur_target].revy = pdat->revy;
	m_blur[m_blur_target].color = 0x00FFFFFF;
	m_blur[m_blur_target].magx = pdat->magx;
	m_blur[m_blur_target].magy = pdat->magy;
	m_blur_target = (m_blur_target + 1) % NUM_BLURS;

	CCharacterBase::PreAction();
}

DWORD CCharacter::Draw()
{
/*	for (int i = m_blur_target; i < m_blur_target+NUM_BLURS; i++)
	{
		int t = i % NUM_BLURS;

		CellDraw(bitmaps, cells, rects,
				 m_blur[t].cnum,
				 m_blur[t].x, m_blur[t].y,
				 m_blur[t].z,
				 m_blur[t].rot,
				 m_blur[t].revx, m_blur[t].revy,
				 m_blur[t].color | (0xFF * (i-m_blur_target+1) / NUM_BLURS << 24),
				 m_blur[t].magx, m_blur[t].magy);
	}

	return TRUE;
*/
	return FALSE;
}

/*----------------------------------------------------------------
	TOUCHB
------------------------------------------------------------------*/
DWORD CCharacter::TouchB(ATTACKINFO *info,BOOL hit)
{
	CCharacterBase::TouchB(info,hit);

	//�Q�[�W�̑���
	switch(info->id){
	case ATTACK_WEAK:
		if(hit)pdat->gauge += 0.02;
		else   pdat->gauge += 0.01;
		break;
	case ATTACK_MIDDLE:
		if(hit)pdat->gauge += 0.04;
		else   pdat->gauge += 0.02;
		break;
	case ATTACK_STRONG:
		if(hit)pdat->gauge += 0.06;
		else   pdat->gauge += 0.03;
		break;
	case ATTACK_HISSATU:
		if(hit)pdat->gauge += 0.08;
		else   pdat->gauge += 0.04;
		break;
	}
	if(pdat->gauge > pdat->gaugemax)
		pdat->gauge = pdat->gaugemax;

	if(info->id == ATTACK_SCDASH){
		if(hit)ChangeAction( ACTID_FESTIVALRUSH );
		else ChangeAction( ACTID_FESTIVALSUKI );
	}

	if(IsLocalCom())
	{
		switch(pdat->aid)
		{
		case ACTID_ATT_SA:
			{
				if(ComLevelCk(3)){
					SetComAct(ACTID_ATT_SB,3);
				}
				if(ComLevelCk(3)){
					SetComAct(ACTID_ATT_SC,5);
				}
			}
			break;

		case ACTID_ATT_SB:
			{
				if(ComLevelCk(3)){
					SetComAct(ACTID_ATT_SC,3);
				}
			}
			break;

		case ACTID_ATT_SC:
		case ACTID_ATT_SMC:
			{
				if(ComLevelCk(5) && pdat->gauge >= 1.0){
					SetComAct(ACTID_FESTIVAL,5);
				}
				if(ComLevelCk(3)){
					SetComAct(ACTID_SYARA2,5);
				}
				if(ComLevelCk(3)){
					SetComAct(ACTID_HADOU2,5);
				}
				if(ComLevelCk(3)){
					SetComAct(ACTID_ATT_CC,5);
				}
			}
			break;

		case ACTID_ATT_CA:
			{
				if(ComLevelCk(3)){
					SetComAct(ACTID_ATT_CC,3);
				}
			}
			break;

		case ACTID_ATT_CB:
			{
				if(ComLevelCk(3)){
					SetComAct(ACTID_ATT_CC,3);
				}
			}
			break;

		case ACTID_ATT_CC:
			{
				if(ComLevelCk(5) && pdat->gauge >= 1.0){
					SetComAct(ACTID_FESTIVAL,5);
				}
				if(ComLevelCk(3)){
					SetComAct(ACTID_SYARA2,5);
				}
				if(ComLevelCk(3)){
					SetComAct(ACTID_HADOU2,5);
				}
			}
			break;
		}
	}

	return(0);
}

//!GOBJMSG_TOUCHA�ɑΉ�
DWORD CCharacter::TouchA(ATTACKINFO *info,DWORD ta_eid)
{
	//���K���̓X�[�p�[�A�[�}�[
	if(pdat->aid==ACTID_FESTIVALDASH ||
		pdat->aid==ACTID_FESTIVALRUSH ||
		pdat->aid==ACTID_FESTIVALFINISH )
	{
		isSuperArmer = TRUE;
	}

	DWORD ret = CCharacterBase::TouchA(info,ta_eid);

	if(ret & 0x10000000)//�����
	{
		hp_to_regenerate /= 2;
		hp_to_regenerate += info->damage/2;
	}

	isSuperArmer = FALSE;
	return ret;
}

/*--------------------------------------------------------------------------------------
	�U���͏�񏉊�
----------------------------------------------------------------------------------------*/
void CCharacter::InitAttackInfo()
{
	ZeroMemory(aif,sizeof(aif));
	int i;

	i=ATTACKINFO_STAND_A;//������U��
	aif[i].hit		=HITINFO_MARK1 | HITINFO_REACT1 | HITINFO_SNDHIT1 | HITINFO_SIV1;
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1 | GUARDINFO_XJAMP;
	aif[i].id		=ATTACK_WEAK;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_STAND_B;//�������U��
	aif[i].hit		= HITINFO_MARK2 | HITINFO_REACT2 | HITINFO_SNDHIT2 | HITINFO_SIV2;
	aif[i].guard	= GUARDINFO_REACT2 | GUARDINFO_SIV2 | GUARDINFO_XJAMP;
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=45;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_STAND_C;//�������U��
	aif[i].hit		=HITINFO_MARK3 | HITINFO_REACT3 | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=60;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_CROUCH_A;//���Ⴊ�ݎ�U��
	aif[i].hit		= HITINFO_MARK1 | HITINFO_REACT1 | HITINFO_SNDHIT1 | HITINFO_SIV1;
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1 | GUARDINFO_XJAMP;
	aif[i].id		=ATTACK_WEAK;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_CROUCH_B;//���Ⴊ�ݒ��U��
	aif[i].hit		= HITINFO_MARK2 | HITINFO_REACT2 | HITINFO_SNDHIT2 | HITINFO_SIV2;
	aif[i].guard	= GUARDINFO_REACT2 | GUARDINFO_SIV2 | GUARDINFO_XJAMP;
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=45;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_CROUCH_C;//���Ⴊ�݋��U��
	aif[i].hit		=HITINFO_MARK3 | HITINFO_REACT3 | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP | GUARDINFO_XSTAND;//���Ⴊ�݃K�[�h�s��
	aif[i].id		=ATTACK_STRONG ;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=60;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_JAMP_A;//�W�����v��U��
	aif[i].hit		=HITINFO_MARK1 | HITINFO_REACT1 | HITINFO_SNDHIT1 | HITINFO_SIV1;
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1 | GUARDINFO_XCROUCH;
	aif[i].id		=ATTACK_WEAK;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_JAMP_B;//�W�����v���U��
	aif[i].hit		= HITINFO_MARK2 | HITINFO_REACT2 | HITINFO_SNDHIT2 | HITINFO_SIV2;
	aif[i].guard	= GUARDINFO_REACT2 | GUARDINFO_SIV2 | GUARDINFO_XCROUCH;
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=40;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_JAMP_C;//�W�����v���U��
	aif[i].hit		=HITINFO_MARK3 | HITINFO_REACT3 | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH;
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=0;//���

	i= ATTACKINFO_STAND_MB;//��+B
	aif[i].hit		= HITINFO_MARK3 | HITINFO_REACT3 | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	= GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP | GUARDINFO_XCROUCH | GUARDINFO_XAUTO;
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=0;//���

	i= ATTACKINFO_DIVE1; //���V���[�C�_�C���i��j
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 ;
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=70;//�_���[�W
	aif[i].kezuri	=8;//���

	i= ATTACKINFO_DIVE2; //���V���[�C�_�C���i���j
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 ;
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=75;//�_���[�W
	aif[i].kezuri	=9;//���

	i= ATTACKINFO_DIVE3; //���V���[�C�_�C���i���j
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 ;
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=80;//�_���[�W
	aif[i].kezuri	=10;//���

	i= ATTACKINFO_SYARA1; //�����`�i��j
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=80;//�_���[�W
	aif[i].kezuri	=10;//���

	i= ATTACKINFO_SYARA2; //�����`�i���j
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=90;//�_���[�W
	aif[i].kezuri	=10;//���

	i= ATTACKINFO_SYARA3; //�����`�i���j
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=100;//�_���[�W
	aif[i].kezuri	=10;//���

	i= ATTACKINFO_FESTA_D	; //�t�F�X�e�B�o���i�_�b�V���q�b�g���j
	aif[i].hit		=HITINFO_MARK4 | HITINFO_REACT2A | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;
	aif[i].id		=ATTACK_SCDASH;//��
	aif[i].damage	=60;//�_���[�W
	aif[i].kezuri	=10;//���

	i= ATTACKINFO_FESTA	; //�t�F�X�e�B�o���i���b�V�����j
	aif[i].hit		=HITINFO_MARK3 | HITINFO_REACT3A | HITINFO_SNDHIT3 | HITINFO_SIV1;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3;
	aif[i].id		=ATTACK_CYOHI;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=20;//�_���[�W
	aif[i].kezuri	=5;//���

	i= ATTACKINFO_FESTA_F; //�t�F�X�e�B�o���i�t�B�j�b�V���j
	aif[i].hit		=HITINFO_MARK4 | HITINFO_FUTTOBI | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 ;
	aif[i].id		=ATTACK_CYOHI;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=500;//�_���[�W
	aif[i].kezuri	=10;//���

	i=ATTACKINFO_NAGE;//����
	aif[i].hit		=HITINFO_MARK3 | HITINFO_REACT3 | HITINFO_SNDHIT3 | HITINFO_SIV3;
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;
	aif[i].id		=ATTACK_STRONG;//�Q�[�W������Ɏg�p����ID
	aif[i].damage	=60;//�_���[�W
	aif[i].kezuri	=0;//���
	aif[i].muki		=TRUE;

	if(OptHeavyHit())
	{
		for(i=0;i<AIF_NUM;i++)
		{
			aif[i].damage = (DWORD)(aif[i].damage*1.2f);
			aif[i].kezuri = (DWORD)(aif[i].kezuri*1.2f);
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
	waz.walkf = ACTID_WALKF;
	waz.walkb = ACTID_WALKB;
	waz.jampf = ACTID_JAMPF;
	waz.jampb = ACTID_JAMPB;
	waz.dashf = ACTID_DASHF;
	waz.dashb = ACTID_DASHB;
	waz.att_short[0] = ACTID_ATT_SA;//�Z���[�`�U��
	waz.att_short[1] = ACTID_ATT_CA;
	waz.att_short[2] = ACTID_ATT_CB;
	waz.att_short[3] = ACTID_WASSYORYUKEN2;
	waz.att_short[4] = ACTID_ATT_SMC;
	waz.att_short[5] = ACTID_GCANCEL1;
	waz.att_middle[0] = ACTID_SYARA1;//�����[�`�U��
	waz.att_middle[1] = ACTID_ATT_CC;
	waz.att_middle[2] = ACTID_SYARA2;
	waz.att_middle[3] = ACTID_ATT_SB;
	waz.att_long[0] = ACTID_ATT_SC;//�����[�`�U��
	waz.att_long[1] = ACTID_SYARA3;
	waz.att_long[2] = ACTID_FESTIVAL;
	waz.att_jamp[0] = ACTID_ATT_JC;//�W�����v�U��
	waz.att_jamp[1] = ACTID_ATT_JB;
	waz.att_jamp[2] = ACTID_ATT_JA;
	waz.att_tai[0] = ACTID_WASSYORYUKEN1;//�΋�
	waz.att_tai[1] = ACTID_WASSYORYUKEN3;
	waz.att_bullet[0] = ACTID_HADOU2;//��ѓ���

	waz.nageid[0] = ACTID_NAGE1;//����

	//���[�`�ݒ�
	SetComReach( 0,MAAI_SHORT );
	SetComReach( 1,MAAI_MIDDLE );
	SetComReach( 2,MAAI_LONG );
}

/*--------------------------------------------------------------------------------------
	COM�s�� �v�l���[�`����`
----------------------------------------------------------------------------------------*/
DWORD CCharacter::ComThink()
{
	GOBJECT *pedat= GetInfo(GetGObject()->eid);
	if(pedat==NULL)return 0;

	// �s���w��
	int val_walkf = 0;
	int val_walkb = 0;
	int val_jumpf = 0;
	int val_jumpb = 0;
	int val_dashf = 0;
	int val_dashb = 0;
	int val_att_sa = 0;
	int val_att_sb = 0;
	int val_att_sc = 0;
	int val_att_smc = 0;
	int val_att_ca = 0;
	int val_att_cb = 0;
	int val_att_cc = 0;
	int val_att_ja = 0;
	int val_att_jb = 0;
	int val_att_jc = 0;
	int val_nage = 0;
	int val_nori = 0;
	int val_dive = 0;
	int val_syara = 0;
	int val_festival = 0;

	// �ʒu�֌W
	int maai = (int)abs(pedat->x - pdat->x);

	if (pdat->aid & ACTID_KUCYU)
	{
		// ��
		if (maai < MAAI_MIDDLE)
		{
			val_att_ja += 5;
			val_att_jb += 6;
			val_att_jc += 7;
		}
	}
	else
	{
		// �n��
		if (pedat->aid & ACTID_KUCYU)
		{
			// �΋�
			if (maai < MAAI_MIDDLE)
			{
				val_dashf += 5;
				val_walkb += 1;
				val_dashb += 4;
				val_dive += 6;
			}
			else
			{
				// ��ɈӖ����������Ȃ�
				val_walkf += 2;
				val_walkb += 4;
				val_nori += 3;
			}
		}
		else
		{
			// �n���
			if (maai < MAAI_SHORT)
			{
				// �ߋ���
				val_walkb += 1;
				val_dashb += 3;
				val_jumpb += 2;
				val_att_sa += 5;
				val_att_ca += 5;
				val_att_cb += 4;
				val_nage += 5;
				val_dive += 6;
				if (pedat->aid & ACTID_SYAGAMI)
					val_att_smc += 2;
				if (pedat->aid & ACTID_ATTACK){
					val_walkb += 2;
					val_dashb += 2;
				}
			}
			else if (maai < MAAI_MIDDLE)
			{
				// ������
				val_walkf += 4;
				val_dashf += 3;
				val_walkb += 4;
				val_dashb += 3;
				val_jumpf += 2;
				val_jumpb += 3;
				val_att_sb += 5;
				val_att_cc += 4;
				val_syara += 6;
				if (pedat->aid & ACTID_SYAGAMI)
					val_att_smc += 3;
			}
			else if (maai < MAAI_LONG)
			{
				// ������
				val_walkf += 4;
				val_dashf += 5;
				val_walkb += 4;
				val_dashb += 3;
				val_jumpf += 5;
				val_jumpb += 3;
				val_att_sc += 5;
				val_syara += 7;
				val_festival += 4;
				val_nori += 4;
			}
			else
			{
				// ��������
				val_walkf += 4;
				val_dashf += 6;
				val_walkb += 2;
				val_jumpf += 5;
				val_nori += 7;
				val_festival += 7;
			}
		}
	}

	// �����_���v�f
	int diff = max(7-GetComLevel(), 2);		// �덷
	val_walkf += rand() % diff;
	val_walkb += rand() % diff;
	val_jumpf += rand() % diff;
	val_jumpb += rand() % diff;
	val_dashf += rand() % diff;
	val_dashb += rand() % diff;
	val_att_sa += rand() % diff;
	val_att_sb += rand() % diff;
	val_att_sc += rand() % diff;
	val_att_smc += rand() % diff;
	val_att_ca += rand() % diff;
	val_att_cb += rand() % diff;
	val_att_cc += rand() % diff;
	val_att_ja += rand() % diff;
	val_att_jb += rand() % diff;
	val_att_jc += rand() % diff;
	val_nage += rand() % diff;
	val_nori += rand() % diff;
	val_dive += rand() % diff;
	val_syara += rand() % diff;
	val_festival += rand() % diff;

	// ������s������
	int highest = 0;
	DWORD now_aid = 0;

	if (val_walkf > highest)	{highest = val_walkf; now_aid = ACTID_WALKF;}
	if (val_walkb > highest)
	{
		highest = val_walkb;
		now_aid = ACTID_WALKB;
	}
	if (val_jumpf > highest)
	{
		highest = val_jumpf;
		now_aid = ACTID_JAMPF;
	}
	if (val_jumpb > highest)
	{
		highest = val_jumpb;
		now_aid = ACTID_JAMPB;
	}
	if (val_dashf > highest)
	{
		highest = val_dashf;
		now_aid = ACTID_DASHF;
	}
	if (val_dashb > highest)
	{
		highest = val_dashb;
		now_aid = ACTID_DASHB;
	}
	if (val_att_sa > highest)
	{
		highest = val_att_sa;
		now_aid = ACTID_ATT_SA;
	}
	if (val_att_sb > highest)
	{
		highest = val_att_sb;
		now_aid = ACTID_ATT_SB;
	}
	if (val_att_sc > highest)
	{
		highest = val_att_sc;
		now_aid = ACTID_ATT_SC;
	}
	if (val_att_ca > highest)
	{
		highest = val_att_ca;
		now_aid = ACTID_ATT_CA;
	}
	if (val_att_cb > highest)
	{
		highest = val_att_cb;
		now_aid = ACTID_ATT_CB;
	}
	if (val_att_cc > highest)
	{
		highest = val_att_cc;
		now_aid = ACTID_ATT_CC;
	}
	if (val_att_ja > highest)
	{
		highest = val_att_ja;
		now_aid = ACTID_ATT_JA;
	}
	if (val_att_jb > highest)
	{
		highest = val_att_jb;
		now_aid = ACTID_ATT_JB;
	}
	if (val_att_jc > highest)
	{
		highest = val_att_jc;
		now_aid = ACTID_ATT_JC;
	}
	if (val_nage > highest)
	{
		highest = val_nage;
		now_aid = ACTID_NAGE1;
	}
	if (val_nori > highest)
	{
		highest = val_nori;
		now_aid = ACTID_HADOU2;
	}
	if (val_dive > highest)
	{
		highest = val_dive;
		now_aid = ACTID_WASSYORYUKEN3;
	}
	if (val_syara > highest)
	{
		highest = val_syara;
		now_aid = ACTID_SYARA3;
	}
	if (val_festival > highest)
	{
		highest = val_festival;
		now_aid = ACTID_FESTIVAL;
	}

	return now_aid;
}
