
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
	{ OPTIONS_CHAIN_COMBO, 0, 0, "CHAIN COMBO", 5 } //�L����i�����Ă�OK�j
};


//**************************************************************************************
//  Character Information
//�@�����ɃL�����N�^�[�̖��O���L�q���܂�
//**************************************************************************************

CCharacterInfo CharInfo("�L�����̖��O",		// �L�����N�^�[�̖��O�i�ő�31�o�C�g�j
						CDI_VERSION,		// �L�����N�^�[DLL�C���^�[�t�F�C�X�̃o�[�W����
						option,				// �I�v�V�����\���̂ւ̃|�C���^
						sizeof(option) / sizeof(CHARACTER_LOAD_OPTION), // �I�v�V�����̐�
						20,					// �I�v�V�����ő�|�C���g��
						FALSE				// �l�b�g�ΐ�Ή����ǂ��� �����̊g���̂��߂ɗp�ӂ���Ă���B���݂�FALSE��n���Ȃ���΂Ȃ�Ȃ��i�΁j
						DTB_VERSION);		// DLL�e���v���[�g�x�[�X�N���X�̃o�[�W����

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
	pdat->hpmax			= 1000;//�ő�̗͒l
	pdat->gaugemax		= 1;//�ő�Q�[�W
	pdat->gaugetype		= GOBJFLGA_POWGAUGE2;//�Q�[�W�^�C�v
}


/*--------------------------------------------------------------------------------------
	���b�Z�[�W����
	�x�[�X�N���X�ŏE���Ă��Ȃ����b�Z�[�W���E�������ꍇ�́A�R�R�ɏ�����ǉ����܂�
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Message(DWORD msg,LPVOID dat,DWORD prm)
{
	switch(msg){
	case 0:return(0);//WARNING�悯�B�����Ă�����
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
BOOL CCharacter::Command_Hissatu(DWORD keyinfo)
{
	//�K�E�Z�R�}���h����
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_KUCYU){//�󒆕K�E�Z�̃R�}���h����
	}
	else{//�n��K�E�Z�̃R�}���h����
	}

	return CCharacterBase::Command_Hissatu(keyinfo);
}

BOOL CCharacter::Command_Normal(DWORD keyinfo)
{
	//GOBJECT* pdat = GetGObject();
	//�_�b�V���R�}���h��
	//if(com66(30)){pdat->aid=ACTID_DASH;return TRUE;}
	//if(com44(30)){pdat->aid=ACTID_DASHB;return TRUE;}

	//�L�[���͂Ōp������^�C�v�̃_�b�V���̏ꍇ
	//if(keyinfo & KEYSTA_FOWORD && pdat->aid==ACTID_DASH)return TRUE;
	//if(keyinfo & KEYSTA_BACK && pdat->aid==ACTID_DASHB)return TRUE;

	return CCharacterBase::Command_Normal(keyinfo);
}

BOOL CCharacter::Command_OnNormal(DWORD keyinfo)
{
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_KUCYU){////�W�����v���쒆
	}
	else{//�n�㓮�쒆
		//�����R�}���h��
		//if (keyinfo & KEYSTA_FOWORD && keyinfo & KEYSTA_BC2 &&
		//	NageHantei(40) && ObjCatch(pdat->eid, GOBJMSG_NAGE)) {
		//		pdat->aid = ACTID_THROW;
		//		return TRUE;
		//}
	}

	return CCharacterBase::Command_OnNormal(keyinfo);
}

/*--------------------------------------------------------------------------------------
	GOBJMSG_ACTION ���b�Z�[�W ����
	�V�����s����ǉ������ꍇ�́A�����ɏ�����ǉ����܂��B
	�s���̑O��ɋ��ʏ�������ꂽ���ꍇ�� PreAction(),PostAction() ���I�[�o�[���C�h���Ă�������
----------------------------------------------------------------------------------------*/
DWORD CCharacter::Action()
{
	switch(GetGObject()->aid){
	//�ǉ������s���ւ̑J�ڂ������ɋL�q����
	//case ACTID_	:act_();break;//��

	case 0:return(0);//WARNING�悯�B�����Ă�����

	default://����ȊO�̓L�����N�^�[��{�N���X�ɔC����
		return CCharacterBase::Action();
	}

	return(TRUE);
}

/*--------------------------------------------------------------------------------------
	�U���͏�񏉊���
----------------------------------------------------------------------------------------*/
void CCharacter::InitAttackInfo()
{
	ZeroMemory(aif,sizeof(aif));

/*	//�ݒ��

	int i;

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
	aif[i].hit		=HITINFO_FUTTOBI | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 | HITINFO_EFCTSINDO;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_CROUCH_A;//���Ⴊ�ݎ�U��
	aif[i].hit		=HITINFO_REACT1 | HITINFO_SIV1 | HITINFO_MARK1 | HITINFO_SNDHIT1 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1  | GUARDINFO_XJAMP | GUARDINFO_XSTAND;//�K�[�h���
	aif[i].id		=ATTACK_WEAK;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=30;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_CROUCH_B;//���Ⴊ�ݒ��U��
	aif[i].hit		=HITINFO_REACT2 | HITINFO_SIV2 | HITINFO_MARK2 | HITINFO_SNDHIT2 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2  | GUARDINFO_XJAMP | GUARDINFO_XSTAND;//�K�[�h���
	aif[i].id		=ATTACK_MIDDLE;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=40;//�_���[�W
	aif[i].kezuri	=0;//���

	i=ATTACKINFO_CROUCH_C;//���Ⴊ�݋��U��
	aif[i].hit		=HITINFO_FUTTOBI2 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//�K�[�h���
	aif[i].id		=ATTACK_STRONG | ATTACK_EREAL;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=0;//���

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
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//�q�b�g���
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH;//�K�[�h���
	aif[i].id		=ATTACK_STRONG;//�Q�[�W�������Ɏg�p����ID
	aif[i].damage	=50;//�_���[�W
	aif[i].kezuri	=0;//���
*/
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
	CCharacterBase::InitWazInfo();//����ŁA�����Ƃ��W�����v�Ƃ��͐ݒ肳���B���ƁA�W�����v�U�����B

	//pdat->winfo�i��������waz�j �֍s��ID��ǉ�����
}
