/*=======================================================================================

	Goluah ���{�N���X�Q

	CCharacterInfo	�@�L�����N�^�[�����ݒ��N���X
	CGoluahObject	�@Goluah�I�u�W�F�N�g���{�N���X
	CCharacterBase	�@�L�����N�^�[�I�u�W�F�N�g���{�N���X
	CBulletBase		�@���ѓ������{�N���X
	CClassicalBullet�@���򓹋��\���̌݊����ѓ����N���X
	CBulletList		�@�򓹋�X�g�N���X

=========================================================================================*/

#include <windows.h>
#include <stdio.h>

#ifdef USE_DIRECT3D_DIRECT
#include "d3dx8.h"
#endif

#include "gobjbase.h"

#ifndef RUNTIME_BUILD_OFF

DI_FUNCTIONS_S *funcs = NULL;
DI_FUNCTIONS_O *funco = NULL;
DI_FUNCTIONS_D *funcd = NULL;

#ifdef _DEBUG
size_t num_allocs = 0;
#endif

//�f�t�H���g�̐ݒ��t�@�C���p�X
#define MYSOUND_LIST		"list.txt"
#define MYSOUND_PATH		"sound"
#define WIN_SERIFU_TXT		"serifu.txt"

#include <vector>
//#include <algorithm>

//global func
BOOL File2Mem(char* filename,char** buff,UINT *len);
int GetRandNum(int num);

//macro
#define DELETE_ARRAY(p)		if((p)){ delete [] (p); (p)=NULL; }

// �Œ����������̂����o�[�W����
#define DTB_LEASTVER	1060
#define STB_LEASTVER	1060

/*! **************************************************************************

	@class CCharacterInfo
	@brief �L�����N�^�[�����ݒ��N���X
	@ingroup GObjectBase

�@�\�t�g���L�����N�^�[���F�����邽�߂ɕK�v�ȏ������ݒ肷���N���X�ł��B
�@�L�����N�^�[DLL���ŁA���̃N���X���O���[�o���錾���A
�@SET_CHARACTERINFO�}�N�����g�p���Ă��������B
*/
/* ************************************************************************** */

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
/*!
	@brief �\�z
	@param CharName �L�����N�^�[�̖��O�i�ő�31�o�C�g�j
	@param CdiVersion �L�����N�^�[DLL�C���^�[�t�F�C�X�̃o�[�W����
	@param Options �I�v�V�����\���̂ւ̃|�C���^
	@param num_Options �I�v�V�����̐�
	@param max_option_point �I�v�V�����ő��|�C���g��
	@param isNetworkable �l�b�g�ΐ��Ή����ǂ���
						 �����̊g���̂��߂ɗp�ӂ����Ă����B���݂�FALSE���n���Ȃ����΂Ȃ��Ȃ��i�΁j
	@param DtbVersion DLL�e���v���[�g�x�[�X�N���X�̃o�[�W����

	�L�����N�^�[�̏������ݒ肵�ăN���X���\�z���܂��B
	���������́A���ɖ��肪�Ȃ����΃}�N���C���ɂ��Ă��܂��܂��傤�R(߁��)ɁB
*/
CCharacterInfo::CCharacterInfo(LPCTSTR CharName/* = "����������" */, DWORD CdiVersion/* = CDI_VERSION */, CHARACTER_LOAD_OPTION* Options/* = NULL */,
		int num_Options/* = 0 */, DWORD max_option_point/* = 0 */, BOOL isNetworkable/* = FALSE */, DWORD DtbVersion/* = DTB_VERSION */)
{
	version = DtbVersion;
	if(version > DTB_VERSION || version < DTB_LEASTVER){ 
#		ifdef _DEBUG
			char dbgmsg[256];
			sprintf(dbgmsg,"CCharacterInfo:DTB�o�[�W�����Ⴂ(%d!=%d)\n",version,DTB_VERSION);
			OutputDebugString(dbgmsg);
#		endif
		m_Error=TRUE;
		return;
	}
	version = CdiVersion;

	m_Error = FALSE;

	m_MaxPoint = max_option_point;
	m_isNetworkable = isNetworkable;

	if (CharName) {
		int length = lstrlen(CharName);

		if (length >= 32) length = 32 - 1;
		charactername = new char[length + 1];
		if (charactername)
		{
			strncpy(charactername, CharName, length);
			charactername[length] = '\0';
		}
	}
	else charactername = NULL;

	if (Options) {
		m_Options = new CHARACTER_LOAD_OPTION[num_Options];
		if (m_Options) {
			for (int i = 0; i < num_Options; i++)
				m_Options[i] = *(Options + i);
			m_NumOptions = num_Options;
		}
		else m_NumOptions = 0;
	}
	else {
		m_NumOptions = 0;
		m_Options = NULL;
	}
}

//! ���ڂ���
CCharacterInfo::~CCharacterInfo()
{
	if (charactername)
		delete [] charactername;

	if (m_Options)
		delete [] m_Options;
}

/*!
	@brief CharacterInfo����

	�}�N�������Ăяo�����A�{�̂ɃL�����̏����𑗐M���Ă܂��B
	�ӎ����Ďg���K�v�͂����܂薳���ł��B
*/
BOOL CCharacterInfo::DllCharacterInfo(LPVOID info)
{
	if(m_Error)return FALSE;

	CDI_CHARACTERINFO *pif = (CDI_CHARACTERINFO*)info;
	if (charactername)
		lstrcpy(pif->name,charactername);//���O
	pif->ver=version;//�o�[�W����

	if(pif->system_version!=0){
		if (m_isNetworkable)
			pif->caps |= CHARACTER_CAPS_NET;
		SetOptionItems(pif);
	}

	return(TRUE);
}

/*!
	@brief CreateaCharacter����

	�}�N�������Ăяo�����A���������Ə��������܂��B
	�N���X�\�z�̓}�N���������Ă邵�B
	�������A�ӎ����Ďg���K�v�͖����ł��B
*/
void CCharacterInfo::DllCreateCharacter(PVOID info)
{
	CDI_CHARACTERINFO2 *info2 = (CDI_CHARACTERINFO2*)info;

	funco = info2->funco;
	funcs = info2->funcs;
	funcd = info2->funcd;
}

/*!
	@brief �I�v�V�������M����

	�I�v�V�����������\���̂��炳�΂��āA�{�̂ɑ��M���܂��B
	�����܂��A�ӎ����Ďg���K�v�͖����ł��B
*/
void CCharacterInfo::SetOptionItems(CDI_CHARACTERINFO* pif)
{
	pif->max_option_point = m_MaxPoint;

	for (int i = 0; i < m_NumOptions; i++)
		pif->options[i] = m_Options[i];
}

/*! **************************************************************************

	@class CGoluahObject
	@brief DLL�����I�u�W�F�N�g���{�N���X
	@ingroup GObjectBase

�@	���Ȗ����̓I�u�W�F�N�g�̍쐬�ƃ��b�Z�[�W�����֐��̐ݒ��B
�@	�悭�g�����b�Z�[�W�Ɋւ��Ă͊֐������łɒ��`�����Ă����̂ŁA�������I�[�o�[���C�h�B
�@	�����Ȃ�����Message�֐����I�[�o�[���C�h���ă��b�Z�[�W�������ǉ������B

	�j���̓I�u�W�F�N�g���b�Z�[�W�AGOBJMSG_DELETE�̎��M���ɍs���Ă����̂ŁA
	�����ȊO�̏ꏊ�Ōp���N���X��delete���Ă͂����܂����B
	�܂��A���b�Z�[�W�����֐�(GOBJECT.msghandler)���X�^�e�B�b�N��gMessageToObject�֐��A
	���[�U�[�f�[�^(GOBJECT.pobjdat_u)���A�N���X�̃|�C���^�Ƃ��Ďg�p�����Ă����̂ŁA
	�p���N���X���ł������̃f�[�^���ύX���Ȃ��ł��������B
*/
/* ************************************************************************** */

CGoluahObject* CGoluahObject::pObjToDelete = NULL;
CGoluahObject* CGoluahObject::pObjDeleting = NULL;

/*!
	@brief �\�z
	@param is_effect �G�t�F�N�g�I�u�W�F�N�g���ǂ����B�G�t�F�N�g�I�u�W�F�N�g�͂����蔻�����s���܂����B
					���̏����́A�I�u�W�F�N�g�t���O�ł͂Ȃ��A�V�X�e�������̃I�u�W�F�N�g�Ǘ��@�\�ɂ����ď��������܂��B
					�������ɕύX���邱�Ƃ͂ł��܂����B
*/
CGoluahObject::CGoluahObject(BOOL is_effect/* = FALSE */)
{
	if(is_effect)
	{
		oid = (*funco->objcreate_fx)();//�G�t�F�N�g�p�I�u�W�F�N�g�𐶐�
	}
	else
	{
		oid = (*funco->objcreate)();//�I�u�W�F�N�g�𐶐�
	}

	pdat = (GOBJECT*)(*funco->getinfo)(oid);//�f�[�^�ւ̃|�C���^��get
	if(pdat!=NULL){
		pdat->pobjdat_u = this;
		pdat->msghandler= gMessageToObject;//���b�Z�[�W�����֐�
	}
	base_z				= 0;
	g_chardir = "";//?�@�caki���Ͽ�A�����̏������ł��B
}


/*!
	@brief �j��

	�j���̓��b�Z�[�W�����֐��� GOBJMSG_DELETE�����M�����Ƃ��ɍs�������̂ŁA
	�����ȊO�̏ꏊ��delete���Ă͂����܂����B
*/
CGoluahObject::~CGoluahObject()
{
	if (pObjToDelete == this)
		pObjToDelete = NULL;
	else if (pdat != NULL) {
		pObjDeleting = this;
		funco->objdelete(pdat->id);
	}
}

/*!
	@brief ���{���b�Z�[�W�����֐�

	�������̊��{�I�ȃ��b�Z�[�W�Ɋւ��āA���z�֐��ɏ������U�蕪���܂��B
*/
DWORD CGoluahObject::Message(DWORD msg,LPVOID pd,DWORD prm)
{
	switch(msg){
	case GOBJMSG_ACTION:	return(Action());
	case GOBJMSG_CNGAID:	ActionIDChanged();return(TRUE);
	case GOBJMSG_COMMAND:	Command();return(TRUE);
	case GOBJMSG_COMMANDCOM:return(CommandCOM(prm));
	case GOBJMSG_TOUCHA:	return(TouchA(pdat->atk2.info1,prm));
	case GOBJMSG_TOUCHB:	return(TouchB(pdat->atk,prm));
	case GOBJMSG_TOUCHC:	return(TouchC(pdat->atk,prm));
	case GOBJMSG_COMTHINK:	return(ComThink());
	case GOBJMSG_DRAW:		return(Draw());
	case GOBJMSG_DRAWFRONT:	return(DrawFront());
	case GOBJMSG_DRAWBACK:	return(DrawBack());
	case GOBJMSG_CNGROUND:	return(OnChangeRound());
	default:
		//���Ƃ̃��b�Z�[�W�̓f�t�H���g�����ɔC����
		return(0);
	}

	return(TRUE);
}


/*!
	@brief ���ʃ��b�Z�[�W�����֐�

	�S�Ă�CGoluahObject�p���N���X�̃I�u�W�F�N�g�́A���b�Z�[�W�����ɂ��̊֐����g���܂��B
	�܂��AGOBJECT.pobjdat_u �̓N���X�̃|�C���^�Ƃ��Ďg�p�������̂ŁA�p���N���X��
	���̒l���ύX���Ă͂����܂����B
*/
DWORD CGoluahObject::gMessageToObject(DWORD msg,LPVOID pd,DWORD prm)
{
	if(pd==NULL)return(0);
	GOBJECT *pdat = (GOBJECT*)pd;
	CGoluahObject *pc = (CGoluahObject*)(pdat->pobjdat_u);

	if(pc!=NULL){
		DWORD result;

		if (pObjDeleting == pc) {	// �蓮�������G���[�h�~
			if(msg==GOBJMSG_DELETE) pObjDeleting = NULL;
			return 1;	// �f�t�H���g�����͍s���܂���
		}

		// ���b�Z�[�W������
		result = pc->Message(msg,pd,prm);

		// �^�E�������X�g�b�p�[�@�������������܂������B�B�B�i�P�[�P�j��د
		pdat->id = pc->oid;
		pdat->pobjdat_u = pc;
		pdat->msghandler = gMessageToObject;
		pc->pdat = pdat;

		if(msg==GOBJMSG_DELETE){
			#ifdef _DEBUG
				OutputDebugString("DLL GOBJMSG_DELETE���b�Z�[�W���܂���\n");
			#endif
			pObjToDelete = pc;
			delete(pc);
		}
		return(result);
	}

	return(0);
}


/*!
	@brief GOBJMSG_ACTION���b�Z�[�W�����֐�

	���{�I��1�t����1���Ăяo�����܂��B
	�p�����āA���̂Ȃ��i�̂����ɕ��򂵂��֐��j�ŃL�����N�^�[�����ѓ���̃A�j���[�V�������L�q���܂��B
*/
DWORD CGoluahObject::Action(){return(TRUE);}

/*!
	@brief GOBJMSG_COMMAND���b�Z�[�W�����֐�

	�R�}���h���菈�����s�����߂ɁA���{�I��1�t����1���Ăяo�����܂��B
	�p�����āA���̂Ȃ��i�̂����ɕ��򂵂��֐��j�ŃR�}���h�������������܂��B
*/
void CGoluahObject::Command(){}

/*!
	@brief GOBJMSG_COMMANDCOM���b�Z�[�W�����֐�

	�R���s���[�^�̍s���J�ڂ̉E�s�𔻒肷�邽�߂ɌĂяo�����܂��B
	���݂̍s�������Awid�ւ̍s���J�ڂ��ł�����0�ȊO���A�s�ł�����0���Ԃ��悤�ȏ������L�q���Ă��������B

	@param wid �Z�̍s��ID
	@return 0:�񋖉�, 0�ȊO:����
*/
DWORD CGoluahObject::CommandCOM(DWORD wid){return(FALSE);}

/*!
	@brief GOBJMSG_TOUCHA���b�Z�[�W�����֐�
	@sa GOBJMSG_TOUCHA_RETURNS

	�����̍U���������ɂ��������Ƃ��ɌĂ΂��܂��B
	�L�[���͂⌻�݂Ƃ��Ă����s���Ȃǂ��画�f���A�K�[�h�E���炢���̓K�؂�
	���^�[���R�[�h���Ԃ��悤�ȏ������L�q���Ă��������B

	@param info ���Ă��ꂽ�U���̏���
	@param ta_eid �U���҂̃I�u�W�F�N�gID
	@return TOUCHA_�`�̃��^�[���R�[�h
*/
DWORD CGoluahObject::TouchA(ATTACKINFO *info,DWORD ta_eid)
{
	return(0);//�f�t�H���g�̃K�[�h���菈��
}

/*!
	@brief GOBJMSG_TOUCHB���b�Z�[�W�����֐�

	�����̍U���������ɂ��������Ƃ��ɌĂ΂��܂��B
	�ʏ��͂����ŃQ�[�W�̑����������������܂��B
	CGoluahObject::TouchB �͍U���̖͂��������s���܂��B

	@param info ���Ă��U���̏���
	@param hit TRUE:�q�b�g����, FALSE:�K�[�h���ꂽ
	@return ����0
*/
DWORD CGoluahObject::TouchB(ATTACKINFO *info,BOOL hit)
{
	pdat->kougeki=FALSE;//�U���̖͂�����
	return(0);
}

/*!
	@brief GOBJMSG_TOUCHC���b�Z�[�W�����֐�
	@sa GOBJMSG_TOUCHC_RETURNS

	�����̍U���������ɓ������A�Ȃ������葤��GOBJMSG_TOUCHA�����M�������O�ɌĂяo�����܂��B
	�ʏ��͂��̃��b�Z�[�W�����������K�v�͂����܂����B

	@param info ���Ă��U���̏���
	@param tc_eid ���Q�҂̃I�u�W�F�N�gID
	@return TOUCHC_�`�̃��^�[���R�[�h
*/
DWORD CGoluahObject::TouchC(ATTACKINFO *info,DWORD tc_eid){return(0);}

/*!
	@brief GOBJMSG_TOUCHC���b�Z�[�W�����֐�

	�L�����N�^�[��COM�����ŁACOM�����������肷�鎞�ɌĂяo�����܂��B
	���̊֐����g���āA���[�U�[�Ǝ��̎v�l���[�`�����g�ނ��Ƃ��o���܂��B

	@return �ڍs���铮����ACTID
*/
DWORD CGoluahObject::ComThink() { return 0; }


/*-----------------------------------------------------------------------------
	���̂���������
-------------------------------------------------------------------------------*/

/*!
	@brief GOBJMSG_CNGAID���b�Z�[�W����

	�s��ID�ω����̏����������ɋL�q���܂��B
	CGoluahObject::ActionIDChanged �́Az�l(GOBJECT::z)���A�z�ʒu(CGoluahObject::base_z)��
	�߂��܂��B
*/
void CGoluahObject::ActionIDChanged()
{
	pdat->z=base_z;//z���W����l�ɖ߂�
}

/*!
	@brief GOBJMSG_CNGROUND���b�Z�[�W�����֐�

	�������E���h�i�s���b�Z�[�W�B
	���̃��b�Z�[�W�̉����Ƃ���0���Ԃ��ƁA���̃I�u�W�F�N�g�͏��������܂��B
	�ʏ���TRUE���Ԃ��A���������Ȃ��悤�ɂ��Ă��������B

	@return FALSE:�I�u�W�F�N�g�̏����ATRUE:���̃��E���h�֎����z��
*/
BOOL CGoluahObject::OnChangeRound()
{
	return(TRUE);
}

/*!
	@brief �I�u�W�F�N�g�̌����𔻒肵��x�I�t�Z�b�g�l���쐬

	�I�t�Z�b�g�ʂ̐����ł��B
	�Ⴆ�΁A���ѓ������L�����N�^�[�̊���W����100"�O"�̈ʒu����
	�J�n�������ꍇ�AGOBJECT::muki�̒l���Q�Ƃ��� +��- �𔻒肵�Ȃ����΂Ȃ��܂��񂪁A
	���̊֐��͂������l�������l���Ԃ��܂��B
	�q�b�g�}�[�N�̔����ʒu�I�t�Z�b�g�Ȃǂɂ��g�p���܂��B

	@param x �I�t�Z�b�g��(+�͑O���A-�͌���)
	@return �������l�������I�t�Z�b�g��
*/
double CGoluahObject::zurex(double x)
{
	if(pdat->muki)return(x*-1);
	else return(x);
}

/*!
	@brief �I�u�W�F�N�g�̌����𔻒肵��x�I�t�Z�b�g�l���쐬

	double�ł̂��̂� int �ɃI�[�o�[���[�h�������̂ł��B
	double�ł��Q�Ƃ��Ă��������B
*/
int CGoluahObject::zurex(int x)
{
	if(pdat->muki)return(x*-1);
	else return(x);
}

/*!
	@brief �I�u�W�F�N�g�̌����𔻒肵��X�����ړ�

	�L�����N�^�[���E�������Ă����Ƃ��ƁA�L�����N�^�[�����������Ă����Ƃ��ł́A
	�L�����N�^�[���ړ������Ƃ���x�l�ɉ������ׂ��l�̕������قȂ��܂��B
	���̊֐���GOBJECT::muki���l�����ăL�����N�^�[��x�ʒu(GOBJECT::x)���ړ����܂��B

	@param dx �ړ���(+�͑O���A-�͌���)
*/
void CGoluahObject::movex(double dx)
{
	if(pdat->muki)dx*=-1;
	pdat->x += dx;
}

/*!
	@brief �I�u�W�F�N�g�̌����𔻒肵��X�����ړ�

	double�ł̂��̂� int �ɃI�[�o�[���[�h�������̂ł��B
	double�ł��Q�Ƃ��Ă��������B
*/
void CGoluahObject::movex(int dx)
{
	if(pdat->muki)dx*=-1;
	pdat->x += dx;
}

/*!
	@brief �I�u�W�F�N�g�v���p�e�B�̒ǉ�
	@sa GOBJECT_PROPERTIES

	GOBJECT::objtype�Ɏw���t���O���ǉ����܂��B���Ƃ��Ɛݒ肳���Ă����t���O�͕ێ������܂��B
	( pdat->objtype |= prop; )
	GOBJECT::objtype���ꊇ�Ŏw�肵�����ꍇ��CGoluahObject::SetProperty���g�p���Ă��������B

	@param prop �ǉ������I�u�W�F�N�g�v���p�e�B�t���O
*/
void CGoluahObject::AddProperty(DWORD prop)
{
	pdat->objtype |= prop;
}

/*!
	@brief �I�u�W�F�N�g�v���p�e�B�̍폜
	@sa GOBJECT_PROPERTIES

	GOBJECT::objtype�����w���t���O���������܂��B�w�肳�ꂽ�t���O�ȊO�̃t���O�͕ێ������܂��B
	( pdat->objtype &= ~prop; )

	@param prop �폜�����I�u�W�F�N�g�v���p�e�B�t���O
*/
void CGoluahObject::RemoveProperty(DWORD prop)
{
	pdat->objtype &= ~(prop);
}

/*!
	@brief �I�u�W�F�N�g�v���p�e�B�̑S�ݒ�
	@sa GOBJECT_PROPERTIES

	GOBJECT::objtype�Ɏw���l���ݒ肵�܂��B���X�̃t���O�͑S�Ď������܂��B
	( pdat->objtype = prop; )

	@param prop �ݒ肷���I�u�W�F�N�g�v���p�e�B�t���O
*/
void CGoluahObject::SetProperty(DWORD prop)
{
	pdat->objtype = prop;
}

/*!
	@brief GOBJMSG_DRAWFRONT���b�Z�[�W�֐�
	@sa GOBJECT_MESSAGE

	GOBJMSG_DRAWFRONT���b�Z�[�W�ɑΉ����鏈���ł��B
	�ʏ��͂����ł͕`�揈�����s���܂����BCGoluahObject::DrawFront�͏���FALSE�i�`�����s���Ȃ��j
	���Ԃ��܂��B

	@return TRUE:�f�t�H���g�̃Z���`�����s�킹��,FALSE:�f�t�H���g�`�����s���Ȃ�
*/
DWORD CGoluahObject::DrawFront(){return(FALSE);}

/*!
	@brief GOBJMSG_DRAWBACK���b�Z�[�W�֐�
	@sa GOBJECT_MESSAGE

	GOBJMSG_DRAWBACK���b�Z�[�W�ɑΉ����鏈���ł��B
	�ʏ��͂����ł͕`�揈�����s���܂����B�X�e�[�W�I�u�W�F�N�g�͂����ŕ`�揈�����s���܂��B
	CGoluahObject::DrawFront�͏���FALSE�i�`�����s���Ȃ��j���Ԃ��܂��B

	@return TRUE:�f�t�H���g�̃Z���`�����s�킹��,FALSE:�f�t�H���g�`�����s���Ȃ�
*/
DWORD CGoluahObject::DrawBack(){return(FALSE);}

/*!
	@brief GOBJMSG_DRAW���b�Z�[�W����
	@sa GOBJECT_MESSAGE

	GOBJMSG_DRAWBACK���b�Z�[�W�ɑΉ����鏈���ł��B
	CGoluahObject::Draw�͏���FALSE(�f�t�H���g�̕`�����s��)���Ԃ��܂��B
	�ʏ��̃I�u�W�F�N�g�́AGCD�̃Z���`�����s�����߁A���̊֐����I�[�o�[���C�h�����K�v�͂����܂����B
	���ɃG�t�F�N�g����D3D�𒼐ڂ������ĕ`�悵�����ꍇ�A���̊֐��̏��������������܂��B

	@return FALSE:�f�t�H���g�̃Z���`�����s�킹��,TRUE:�f�t�H���g�`�����s���Ȃ�
*/
DWORD CGoluahObject::Draw(){return(FALSE);}

/*!
	@brief �s��ID�ύX����

	�s��ID���w���l�ɃZ�b�g���āA�p�����[�^�̃��Z�b�g���s���܂��B

	GOBJMSG_ACTION �� GOBJMSG_COMMAND ���b�Z�[�W�������ɍs��ID(GOBJECT.aid)���ύX�����ꍇ�́A
	�V�X�e�����ŕύX�����m���A�p�����[�^�̃��Z�b�g���s���� GOBJMSG_CNGAID���b�Z�[�W �����M�����܂��B
	�����ȊO�̏ꏊ�ōs��ID���ύX�����ꍇ�A�p�����[�^�̃��Z�b�g�͖����I�ɍs���Ȃ����΂Ȃ��܂����B

	�s��ID�̕ύX�́A�l�𒼐ڑ��������̂ł͂Ȃ��A���̊֐����g���čs���̂����S�ł��B

	@param actid �ύX�����V�����s��ID�l
*/
void CGoluahObject::ChangeAction(DWORD actid)
{
	if(pdat->aid!=actid){
		pdat->aid=actid;
		funco->actidchanged(pdat->id);
	}
}

/*!
	@brief new����

	delete�������Ǝ��Ȃ̂ŁA���łɂ��������B
*/
void* CGoluahObject::operator new(size_t size)
{
#	ifdef _DEBUG
		void* result = malloc(size);

		if (result)
			num_allocs += size;

		return result;
#	else
		return malloc(size);
#	endif //_DEBUG
}

/*!
	@brief delete����

	�A�T�[�V�������E�U�C�̂ŁA���O�ŏ����܂����B
*/
void CGoluahObject::operator delete(void* p, size_t size)
{
#		ifdef _DEBUG
			num_allocs -= size;
#		endif

	free(p);
}


/*-----------------------------------------------------------------------------
	�V�X�e���֘A�̊֐�
-------------------------------------------------------------------------------*/

/*!
	@brief �L�[���͎擾
	@sa GKEYSTATES
	@sa DI_FUNCTIONS_S::getkey
	@param keyinput �L�[���̓C���f�b�N�X�l(CreateCharacter���Ɏ擾�����l)
	@param interval ���t���[���O�̓��͂��擾���邩�H(�ő�255) 0�Ō��݃t���[���̓��͂��擾�B
	@retuen �L�[���̓R�[�h�BKEYSTA_�`�̒��`���Q��

	�V�X�e�����ێ������L�[���͂̃��O�����A�w���t���[�������ߋ��̒l���擾���܂��B
	�ʏ���0(����)�̓��͂��擾���܂��B
*/
DWORD CGoluahObject::GetKey(DWORD keyinput, DWORD interval) {
	return funcs->getkey(keyinput, interval);
}
/*!
	@brief �L�[���͌���
	@sa GKEYSTATES
	@sa DI_FUNCTIONS_S::seekkey
	@param keyinput �L�[���̓C���f�b�N�X�l(CreateCharacter���Ɏ擾�����l)
	@param offset ���t���[���O���猟�����J�n���邩
	@param delay ���t���[���O�܂Ō����ΏۂƂ��邩
	@return ���������ꍇ�A���݃t���[�����牽�t���O�����Ԃ��܂��B�������Ȃ������ꍇ�}�C�i�X�l

	�V�X�e���̕ێ������L�[���̓��O�����w���L�[���͂��������܂�.
	�R�}���h�������s���Ƃ��Ɏg�p���܂��B
*/
int CGoluahObject::SeekKey(DWORD keyinput, int offset, int delay, DWORD keystate) {
	return funcs->seekkey(keyinput,  offset,  delay, keystate);
}
/*!
	@brief �����䎌�ݒ�
	@sa DI_FUNCTIONS_S::setkatiserif
	@param tid �ǂ����̃`�[����
	@param serif �ݒ肷���䎌������

	�V�X�e���ɏ����䎌���������ʒm���Đݒ肵�܂��B
	�ʏ��́A�O���e�L�X�g�����ݒ����ǂݍ����Őݒ肷���ACCharacterBase::LoadAndSetKatiSerif
	���g�p���܂��B
*/
void CGoluahObject::SetKatiSerif(DWORD tid, char* serif) {
	funcs->setkatiserif(tid, serif);
}
/*!
	@brief �V�X�e���G�t�F�N�g����
	@sa DI_FUNCTIONS_S::addeffect
	@sa SYSTEM_EFFECT_IDs
	@param EffectID �G�t�F�N�g��ID(EFCTID_�`)
	@param prm1 EffectID�ɂ�����
	@param prm2 EffectID�ɂ�����
	@param prm3 EffectID�ɂ�����

	�V�X�e���G�t�F�N�g�𔭐������܂��B
	���ꂼ���̃G�t�F�N�g��ID�ƈ����̈Ӗ���SYSTEM_EFFECT_IDs���Q�ƁB
*/
void CGoluahObject::AddEffect(DWORD EffectID, int prm1, int prm2, DWORD prm3) {
	funcs->addeffect(EffectID, prm1, prm2, prm3);
}
/*!
	@brief �V�X�e���T�E���h�Đ�
	@sa DI_FUNCTIONS_S::playsyssound
	@sa SYSTEM_SOUND_IDs
	@param SoundNo �V�X�e���T�E���h��ID

	�V�X�e���ŗp�ӂ����Ă����T�E���h���Đ������B
	������SYSTEM_SOUND_IDs���Q�Ƃ̂��ƁB
*/
void CGoluahObject::PlaySysSound(DWORD SoundNo) {
	funcs->playsyssound(SoundNo);
}
/*!
	@brief wav�T�E���h���[�h
	@param pathname �t�@�C����(�{�̎��s�t�@�C�������̑���)
	@return �����ł����΃T�E���h�ւ̃|�C���^�A���s�ł�����NULL
	@sa DI_FUNCTIONS_S::loadmysound
	@sa CGoluahObject::PlayMySound
	@sa CGoluahObject::KillMySound

	wav�T�E���h�����[�h�����B
	�߂��l�ŗ^�����ꂽ�|�C���^�́A�K��CGoluahObject::KillMySound�Ŕj�����邱�ƁB
*/
LPVOID CGoluahObject::LoadMySound(char* pathname) {
	return funcs->loadmysound(pathname);
}
/*!
	@brief wav�T�E���h�Đ�
	@param psound �T�E���h�̃|�C���^
	@sa DI_FUNCTIONS_S::playmysound
	@sa CGoluahObject::LoadMySound

	LoadMySound�Ń��[�h����wav�T�E���h���Đ������B
*/
void CGoluahObject::PlayMySound(LPVOID psound) {
	funcs->playmysound(psound);
}
/*!
	@brief wav�T�E���h�j��
	@param psound �T�E���h�̃|�C���^
	@sa DI_FUNCTIONS_S::killmysound
	@sa CGoluahObject::LoadMySound

	LoadMySound�Ń��[�h����wav�T�E���h���A�����[�h����
*/
void CGoluahObject::KillMySound(LPVOID psound) {
	funcs->killmysound(psound);
}

/*!
	@brief �V�X�e���փ��b�Z�[�W���M
	@param mes ���b�Z�[�WID
	@param prm �p�����[�^
	@return ���b�Z�[�WID�ɑΉ��������^�[���R�[�h
	@sa DI_FUNCTIONS_S::msg2system
	@sa MESSAGE_OBJ2SYS

	�V�X�e���փ��b�Z�[�W�𑗐M���܂��B���M�ł��郁�b�Z�[�W�̒l�Ƃ��ꂼ���̈Ӗ��Ɋւ��Ă�MESSAGE_OBJ2SYS���Q�ƁB
	�����i�s�Ɋ֌W�����C�x���g���A�x���U���A���㓙�̗v���Ȃǂ������ɂ������܂��B
*/
DWORD CGoluahObject::Message2System(DWORD mes, DWORD prm) {
	return funcs->msg2system(pdat->id, mes, prm);
}

/*!
	@brief �G�̖��O���擾����
	@param tid �������̃`�[��ID
	@return �G�̖��O������
	@sa DI_FUNCTIONS_S::getenemyname

	�擾�ł����G�̖��O�́A�ΐ��`���������ΐ��̏ꍇ�s���ł��B
	�����ȊO�̏ꍇ�͍Ō���KO���ꂽ�L�����N�^�[�̖��O���擾�����܂��B
*/
char* CGoluahObject::GetEnemyName(DWORD tid) {
	return funcs->getenemyname(tid);
}

/*!
	@brief �I�u�W�F�N�g���L�L�����̖��O���擾����
	@param oid �I�u�W�F�N�gID
	@return �w���L�����̖��O������
	@sa DI_FUNCTIONS_O::getcharname
*/
char* CGoluahObject::GetCharacterName(DWORD oid) {
	return funco->getcharname(oid);
}

/*!
	@brief �I�u�W�F�N�g���L�L�����̃L�[���̓C���f�b�N�X�l���擾����
	@param oid �I�u�W�F�N�gID
	@return �w���L�����̃L�[���̓C���f�b�N�X�l
	@sa DI_FUNCTIONS_O::getkeyinput
*/
DWORD CGoluahObject::GetKeyInput(DWORD oid) {
	return funco->getkeyinput(oid);
}


/*!
	@brief ���݂̕\�����S�ʒuX���擾
	@return �\�����SX���W
	@sa DI_FUNCTIONS_S::etdispcenterx

	�X�e�[�W�̕\���ʒu�̓L�����N�^�[�̈ړ��ɂ����ĕω����܂��B
	���̊֐��̓X�e�[�W�̌��݂̕\�����S�ʒu��X���W���擾���܂��B
*/
double CGoluahObject::GetDisplayCenter_X() {
	return funcs->getdispcenterx();
}
/*!
	@brief ���݂̕\�����S�ʒuY���擾
	@return �\�����SY���W
	@sa DI_FUNCTIONS_S::etdispcentery

	�X�e�[�W�̕\���ʒu�̓L�����N�^�[�̈ړ��ɂ����ĕω����܂��B
	���̊֐��̓X�e�[�W�̌��݂̕\�����S�ʒu��X���W���擾���܂��B
*/
double CGoluahObject::GetDisplayCenter_Y() {
	return funcs->getdispcentery();
}
/*!
	@brief �`�[���̐l�����擾
	@return TEAM_PLAYER1���̃`�[���l��
	@sa CGoluahObject::GetTeamNum2
	@sa DI_FUNCTIONS_S::getteamnum

	�ȑO��1P/2P���̐l�������ꂾ�����̂ł��̊֐����g�p���܂������A���݂�
	�Ⴄ�̂ŁA���̊֐��̎g�p�͔����Ă��������B
	���֋@�\�Ƃ���CGoluahObject::GetTeamNum2���g�p���܂��B
*/
DWORD CGoluahObject::GetTeamNum() {
	return funcs->getteamnum();
}
/*!
	@brief �����`���擾
	@return TAISENKEISIKI_�`�̒l
	@sa TAISENKEISIKI
	@sa DI_FUNCTIONS_S::gettaisenkeisiki

	���݂̎����`�����擾�����B
*/
DWORD CGoluahObject::GetTaisenKeisiki() {
	return funcs->gettaisenkeisiki();
}
/*!
	@brief �G��face�ύX�l���擾����
	@param tid �������̃`�[��ID
	@return face�ύX�l(�ʏ�0)
	@sa DI_FUNCTIONS_S::getenemyface

	�G�̃`�[���ɕ����̃L�����N�^�[�����݂����ꍇ�A�ǂ̃L�����N�^�[���I�������邩��
	CGoluahObject::GetEnemyName�Ɠ��l�ł��B
	���̊֐����Ԃ��l�͒ʏ�0�ł��B�������[/�������G���̂悤�ɁA�I�v�V�����ݒ��ɂ�����
	face2,3.bmp �̃C���[�W���ω������L�����N�^�[�̏ꍇ�A�I�v�V�����l���Q�Ƃ���
	�ǂ̃C���[�W���g�p�����Ă��邩���Ԃ��܂��B
	�����䎌�ݒ莞�Ȃǂɗ��p���܂��B
*/
BYTE CGoluahObject::GetEnemyFace(DWORD tid) {
	return funcs->getenemyface(tid);
}
/*!
	@brief �`�[���̐l�����擾
	@param tid �l�����擾�����`�[���̃`�[��ID
	@return �w���`�[���̐l��
	@sa DI_FUNCTIONS_S::getteamnum2

	�w���`�[���̐l�����擾���܂��B
	tid�ɂ͒ʏ펩���̃`�[��ID(GOBJECT::tid)���w�肵�܂��B
	�G�`�[���Ɋւ��Ē��ׂ����ꍇ��CGoluahObject::EnemyTID���g�p���܂��B
*/
DWORD CGoluahObject::GetTeamNum2(DWORD tid)
{
	return funcs->getteamnum2(tid);
}

/*!
	@brief �`�[��ID���G���̃`�[��ID�ɕϊ�
	@param tid �������`�[��ID
	@return �G���`�[��ID
	@sa DI_FUNCTIONS_S::tid2etid

	�`�[��ID���G���̃`�[��ID�ɕϊ����܂��B

*/
DWORD CGoluahObject::TID2EnemyTID(DWORD tid)
{
	return funcs->tid2etid(tid);
}
/*!
	@brief �G���̃`�[��ID���擾
	@return �G�̃`�[��ID

	�����̃`�[��ID(GOBJECT::tid)��CGoluahObject::TID2EnemyTID(DWORD tid)�ɂ�������
	�G���̃`�[��ID���悵�܂��B
*/
DWORD CGoluahObject::EnemyTID()
{
	return TID2EnemyTID(pdat->tid);
}
/*!
	@brief �l�b�g���[�N���[�h���ǂ������擾����
	@return ���݂͏���FALSE
	@sa DI_FUNCTIONS_S::is_net

	���݂͏���FALSE���Ԃ��܂�
*/
BOOL CGoluahObject::IsNetwork(){
	return funcs->is_net();
}

/*!
*	@brief �^�O�̃Z�b�g
*	@param tag �G���[�������Ƀ��O�ɏo�͂����镶����
*	@sa DI_FUNCTIONS_S::pushtag
*
*	���O�������A���̊֐��ɂ����Đݒ肳�ꂽ�����񂪃G���[���O�ɒǉ������܂��B
*	���O�𓊂����o�O�̔����������ȏꏊ�� PushTag , PopTag �ň͂ނ��Ƃɂ����āA
*	�G���[�������̓����ɖ𗧂��������Ȃ��B
*	�ݒ肵���^�O�̓X�^�b�N���ɕێ��������̂ŁA���̊֐��𕡐������s���邱�Ƃ��\�ł��B
*	�V�X�e���͂��̊֐��œn���ꂽ�������̃R�s�[�͍쐬���Ȃ��̂ŁA
*	���̊֐��Őݒ肷�镶�����͔j�������Ȃ��ꏊ�Ɋm�ۂ��Ă��������B
*/
void CGoluahObject::PushTag(const char* tag)
{
	funcs->pushtag( tag );
}

/*!
*	@brief �^�O�̏���
*	@sa CGoluahObject::PushTag
*	@sa DI_FUNCTIONS_S::poptag
*
*	�Ō���Push���ꂽ�^�O���폜���܂�
*/
void CGoluahObject::PopTag()
{
	funcs->poptag();
}

/*!
*	@brief ���O�o��([error]) + ����
*/
void CGoluahObject::LogError(const char* fmt,...)
{
	va_list args;
	va_start(args, fmt);
	char buffer[512];
	vsprintf(buffer, fmt, args);
	funcs->logerror( buffer );
	va_end(args);
}

/*!
*	@brief ���O�o��([warning]) + ����
*/
void CGoluahObject::LogWarning(const char* fmt,...)
{
	va_list args;
	va_start(args, fmt);
	char buffer[512];
	vsprintf(buffer, fmt, args);
	funcs->logwarning( buffer );
	va_end(args);
}

/*!
*	@brief ���O�o��([debug]) + ����
*/
void CGoluahObject::LogDebug(const char* fmt,...)
{
	va_list args;
	va_start(args, fmt);
	char buffer[512];
	vsprintf(buffer, fmt, args);
	funcs->logdebug( buffer );
	va_end(args);
}

/*!
*	@brief ���O�o��([info]) + ����
*/
void CGoluahObject::LogInfo(const char* fmt,...)
{
	va_list args;
	va_start(args, fmt);
	char buffer[512];
	vsprintf(buffer, fmt, args);
	funcs->loginfo( buffer );
	va_end(args);
}


/*-----------------------------------------------------------------------------
	�I�u�W�F�N�g�֘A�̊֐�
-------------------------------------------------------------------------------*/
/*!
	@brief �����Ƃ��ő���������
	@param eid ���݂����I�u�W�F�N�gID
	@param msg_nage ���������������Ƃ��A���葤�̍s���𐧌䂷�邽�߂Ɏg�p���郁�b�Z�[�WID
	@return TRUE:����
	@sa DI_FUNCTIONS_O::objcatch
	@sa CATCHYOU

	���������ꍇ�͑��肪�����������ԂɂȂ��̂ŁA���������s�����K�؂ɏ������Ȃ���
	�����i�s�Ɉُ����������̂Œ��ӂ��Ă��������B
*/
BOOL   CGoluahObject::ObjCatch(DWORD eid,DWORD msg_nage) {
	CATCHYOU cy;

	cy.oid = pdat->id;
	cy.actmsg = msg_nage;
	return funco->objcatch(eid, &cy);
}
/*!
	@brief �I�u�W�F�N�g����(GOBJECT)�擾
	@param oid �擾�����I�u�W�F�N�g�̃I�u�W�F�N�gID
	@return �w���I�u�W�F�N�g��GOBJECT�\����
	@sa DI_FUNCTIONS_O::getinfo

	�w��ID�̃I�u�W�F�N�g�̃I�u�W�F�N�g�������擾���܂��B
*/
GOBJECT* CGoluahObject::GetInfo(DWORD oid) {
	return (GOBJECT*)funco->getinfo(oid);
}

/*!
	@brief �����I�Ƀ_���[�W���^����
	@param eid �_���[�W���󂯂��I�u�W�F�N�g��ID
	@param x �U�����q�b�g�������Ƃɂ���x���W
	@param y �U�����q�b�g�������Ƃɂ���y���W
	@sa DI_FUNCTIONS_O::adddamage

	���ɓ����̂Ƃ��A�������Ԃ̃I�u�W�F�N�g�ɂ͂����蔻�肪�s�����Ă��Ȃ��̂ŁA
	���̊֐��ő����Ƀ_���[�W���^���܂��B�U�������͎��I�u�W�F�N�g�ɐݒ肳�ꂽ���̂��g�p�������̂ŁA
	���ʂɍU���������Ƃ����l�AStartAttack���g�p���čU���͏������ݒ肵�܂��B
*/
void   CGoluahObject::AddDamage(DWORD eid,int x,int y) {
	funco->adddamage(pdat->id, eid, x, y);
}
/*!
	@brief �s��ID���ω������Ƃ��̏����𖾎��I�ɍs��
	@sa DI_FUNCTIONS_O::actidchanged
	@sa ChangeAction

	GOBJMSG_ACTION �� GOBJMSG_COMMAND ���b�Z�[�W�������ɍs��ID(GOBJECT.aid)���ύX�����ꍇ�́A
	�V�X�e�����ŕύX�����m���A�p�����[�^�̃��Z�b�g���s���� GOBJMSG_CNGAID���b�Z�[�W �����M�����܂��B
	�����ȊO�̏ꏊ�ōs��ID���ύX�����ꍇ�A�p�����[�^�̃��Z�b�g�͖����I�ɍs���Ȃ����΂Ȃ��܂����B

	�s��ID���ύX�������ł��̊֐����Ăяo�����Ƃɂ����A���L�̂悤�ȏ������s���܂��B
	�ꉞ�P�̂ł����`�A���ʂɎg���Ȃ�ChangeAction�̎g�p���������߂��܂��B
*/
void   CGoluahObject::ActIDChanged() {
	funco->actidchanged(pdat->id);
}
/*!
	@brief ���݃A�N�e�B�u�ȃL�����N�^�[�̏������擾
	@param tid �`�[��
	@return �w���`�[���̌��݃L�����N�^�[�̃I�u�W�F�N�g����
	@sa DI_FUNCTIONS_O::getactivechar

	"�A�N�e�B�u�ȃL�����N�^�["�́A�ΐ��`���ɂ������܂����A
	���{�I�ɉ��ʂɏo�Đ����Ă��L�����N�^�[���Ԃ��A���ۂ��H�����܂��Ȓ��`�ł��B
*/
GOBJECT*  CGoluahObject::GetActiveCharacter(DWORD tid) {
	return (GOBJECT*)funco->getactivechar(tid);
}
/*!
	@brief ���݃A�N�e�B�u�ȓG�L�����N�^�[�̏���
	@return �G�`�[���̌��݃L�����N�^�[�̃I�u�W�F�N�g����

	CGoluahObject::GetActiveCharacter �Q��
*/
GOBJECT*  CGoluahObject::GetActiveEnemy() {
	return GetActiveCharacter( EnemyTID() );
}
/*!
	@brief �I�u�W�F�N�g����
	@sa DI_FUNCTIONS_O::suicide

	�I�u�W�F�N�g���j�����܂��B
	���ӁF�u�G�t�F�N�g�����v�ł͂Ȃ��I�u�W�F�N�g�����̊֐��Ŕj�������͔̂񐄏��ł��B
*/
void   CGoluahObject::Suicide() {
	funco->suicide(pdat->id);
}

/*!
	@brief �ԍ�����get(����)
	@param oid �I�u�W�F�N�g����1
	@param eoid �I�u�W�F�N�g����2
	@return �w��2�I�u�W�F�N�g�Ԃ̊ԍ���
	@sa DI_FUNCTIONS_O::getmaai_h

	�ԍ����͏d�Ȃ蔻���̋����Ōv�Z�����܂��B
	�I�u�W�F�N�g�����]���܂ޏꍇ�A�������l�͓������܂����B
*/
DWORD  CGoluahObject::GetMaai_H(DWORD oid, DWORD eoid) {
	return funco->getmaai_h(oid, eoid);
}
/*!
	@brief �ԍ�����get(����)
	@param oid �I�u�W�F�N�g����1
	@param eoid �I�u�W�F�N�g����2
	@return �w��2�I�u�W�F�N�g�Ԃ̊ԍ���
	@sa DI_FUNCTIONS_O::getmaai_v
*/
DWORD  CGoluahObject::GetMaai_V(DWORD oid, DWORD eoid) {
	return funco->getmaai_v(oid, eoid);
}

/*!
	@brief �L�����N�^�[�̃I�u�W�F�N�gID
	@param tid �`�[��ID
	@param index �`�[�����ł̃C���f�b�N�X(0�`)
	@return �L�����N�^�[�̃I�u�W�F�N�gID
	@sa DI_FUNCTIONS_O::getcharid
*/
DWORD  CGoluahObject::GetCharacterID(DWORD tid,DWORD index) {
	return funco->getcharid(tid, index);
}
/*!
	@brief COM���x���擾
	@return COM�����̏ꍇ�̃��x��(���Փx)
	@sa DI_FUNCTIONS_O::getcomlevel

	COM�̃��x����0�`7�܂ł�8�i�K�ł��B
	�V�X�e���ɂ��鐧���ɉ����āADLL���ŃR���s���[�^�ɘA���Z�����点���ꍇ��
	�Q�l�ɂ��Ă��������B
	�܂��A�A���Z�����点���ꍇ�ɂ� CGoluahObject::SetComAct �𐄏����܂�
*/
DWORD  CGoluahObject::GetComLevel(){
	return funco->getcomlevel(oid);
}
/*!
	@brief COM���[�`�ݒ�
	@param idx COMIDX_NEAR:�Z,COMIDX_MIDDLE:��,COMIDX_LONG:��
	@param length �ݒ肷�郊�[�`��
	@sa DI_FUNCTIONS_O::setcomreach

	COM�����̃��[�`�����ݒ肵�܂��B
	�V�X�e�����ŋZ�����\���̂ɐݒ肳�ꂽ�Z�̂����A�ǂ����I�����邩�̔��f�ɉe�����܂��B
	�ݒ����s���Ȃ��ꍇ��ver0.8�ȑO�Ŏg�p�����Ă����Œ��l���g�p�����܂��B
*/
void   CGoluahObject::SetComReach(DWORD idx,int length){
	funco->setcomreach(oid,idx,length);
}
/*!
	@brief COM�s���ݒ�
	@param aid ���ɋN�����ׂ��s���̍s��ID(0�̏ꍇ�s�����}������)
	@param delay �����x������
	@sa DI_FUNCTIONS_O::setcomact

	COM�����ɋN�����s�����w�肵�܂��B�A���Z�����点���Ƃ��ȂǂɎg�p���܂��B
	���̊֐��Ŏw�肵�������x�����Ԃ̊Ԃ͕ʂ̍s���Ɉڍs���܂����i�K�[�h�E���炢���������j
	�������A�K�[�h���炢���̍U���s�\�ȍs���ɑJ�ڂ����ꍇ�͂��̐ݒ��̓N���A�����܂��B
*/
void   CGoluahObject::SetComAct(DWORD aid,int delay){
	funco->setcomact(oid,aid,delay);
}
/*!
	@brief COM�����s���}��
	@param delay �}�����鎞��
	@sa SetComAct

	COM�̍s���J�ڂ��w�肵�����Ԃ̊ԗ}�����܂��B
	�s���J�ڂ��N�����Ă����U���͂����������܂łɎ��Ԃ̂������Z�̏ꍇ�A
	���̊Ԃɕʂ̋Z���L�����Z�����ďo���Ȃ��悤�Ƀf�B���C���������邽�߂Ɏg�p���܂��B
*/
void   CGoluahObject::ComDelay(int delay){
	SetComAct(0,delay);
}
/*!
	@brief �I�u�W�F�N�g�����[�J���ł��邩�ǂ���
	@return ���݂͏���TRUE

	�������Ƀl�b�g���[�N�ΐ��̋@�\���������ꂽ�ꍇ�A�I�u�W�F�N�g�̂����蔻�蓙�́A
	���̃I�u�W�F�N�g�����[�J���ȃV�X�e���̊Ǘ��̂��̂ł��邩�ǂ����𔻒f�����K�v�������܂��B
*/
BOOL CGoluahObject::IsLocal(){
	return funco->obj_is_local(oid);
}

void CGoluahObject::dact_damages1(GOBJECT *p)		{funco->dact_damages1(p);}
void CGoluahObject::dact_damages2(GOBJECT *p)		{funco->dact_damages2(p);}
void CGoluahObject::dact_damages3(GOBJECT *p)		{funco->dact_damages3(p);}
void CGoluahObject::dact_damagec1(GOBJECT *p)		{funco->dact_damagec1(p);}
void CGoluahObject::dact_damagec2(GOBJECT *p)		{funco->dact_damagec2(p);}
void CGoluahObject::dact_damagec3(GOBJECT *p)		{funco->dact_damagec3(p);}
void CGoluahObject::dact_damagej1(GOBJECT *p)		{funco->dact_damagej1(p);}
void CGoluahObject::dact_damagej2(GOBJECT *p)		{funco->dact_damagej2(p);}
void CGoluahObject::dact_damagej3(GOBJECT *p)		{funco->dact_damagej3(p);}
void CGoluahObject::dact_damages1a(GOBJECT *p)		{funco->dact_damages1a(p);}
void CGoluahObject::dact_damages2a(GOBJECT *p)		{funco->dact_damages2a(p);}
void CGoluahObject::dact_damages3a(GOBJECT *p)		{funco->dact_damages3a(p);}
void CGoluahObject::dact_damagec1a(GOBJECT *p)		{funco->dact_damagec1a(p);}
void CGoluahObject::dact_damagec2a(GOBJECT *p)		{funco->dact_damagec2a(p);}
void CGoluahObject::dact_damagec3a(GOBJECT *p)		{funco->dact_damagec3a(p);}
void CGoluahObject::dact_guards1(GOBJECT *p)		{funco->dact_guards1(p);}
void CGoluahObject::dact_guards2(GOBJECT *p)		{funco->dact_guards2(p);}
void CGoluahObject::dact_guards3(GOBJECT *p)		{funco->dact_guards3(p);}
void CGoluahObject::dact_guardc1(GOBJECT *p)		{funco->dact_guardc1(p);}
void CGoluahObject::dact_guardc2(GOBJECT *p)		{funco->dact_guardc2(p);}
void CGoluahObject::dact_guardc3(GOBJECT *p)		{funco->dact_guardc3(p);}
void CGoluahObject::dact_guardj1(GOBJECT *p)		{funco->dact_guardj1(p);}
void CGoluahObject::dact_guardj2(GOBJECT *p)		{funco->dact_guardj2(p);}
void CGoluahObject::dact_guardj3(GOBJECT *p)		{funco->dact_guardj3(p);}
void CGoluahObject::dact_down(GOBJECT *p)			{funco->dact_down(p);}
void CGoluahObject::dact_down2(GOBJECT *p)			{funco->dact_down2(p);}
void CGoluahObject::dact_okiagari(GOBJECT *p)		{funco->dact_okiagari(p);}
void CGoluahObject::dact_futtobi(GOBJECT *p)		{funco->dact_futtobi(p);}
void CGoluahObject::dact_futtobi2(GOBJECT *p)		{funco->dact_futtobi2(p);}
void CGoluahObject::dact_tatakituke1a(GOBJECT *p)	{funco->dact_tatakituke1a(p);}
void CGoluahObject::dact_tatakituke1b(GOBJECT *p)	{funco->dact_tatakituke2b(p);}
void CGoluahObject::dact_tatakituke2a(GOBJECT *p)	{funco->dact_tatakituke1a(p);}
void CGoluahObject::dact_tatakituke2b(GOBJECT *p)	{funco->dact_tatakituke2b(p);}

/*!
	@brief COM�����W�ݒ�
	@param idx COMIDX_NEAR:�Z,COMIDX_MIDDLE:��,COMIDX_LONG:��
	@sa DI_FUNCTIONS_O::setcomrange

	COM�����Ŋ��{�ƂȂ��ԍ������ݒ肵�܂��B
	�܂��������ł��A�g���Ȃ������ǂ����B
*/
void   CGoluahObject::SetComRange(DWORD idx){
	funco->setcomrange(oid,idx);
}


/*-----------------------------------------------------------------------------
	�`���֘A�̊֐�
-------------------------------------------------------------------------------*/

/*!
	@brief IDirect3D* �̎擾
	@sa DI_FUNCTIONS_D::getd3d

	IDirect3D*���擾���܂��B�擾���Ă��g�����Ȃ����������Ȃ�����...
*/
LPDIRECT3D8 CGoluahObject::GetD3D() {
	return (LPDIRECT3D8)funcd->getd3d();
}
/*!
	@brief IDirect3DDevice* �̎擾
	@sa DI_FUNCTIONS_D::getd3dd

	IDirect3DDevice* ���擾���܂��B
	IDirect3DDevice*�ɂ����ĉ\�ȑ�����DirectX��SDK���Q�Ƃ��Ă��������B
*/
LPDIRECT3DDEVICE8 CGoluahObject::GetD3DDevice() {
	return (LPDIRECT3DDEVICE8)funcd->getd3dd();
}
/*!
	@brief ver0.70�`���Z���f�[�^�ǂݍ���(�񐄏�)
	@sa CGoluahObject::CreateCellData
	@sa DI_FUNCTIONS_D::loadcelldat
	@param pathname [in] �ǂݍ��ރt�@�C����
	@param pCells [out] ���ʂ��i�[����CELL�f�[�^�\���̔z��
	@param pRects [out] ���ʂ��i�[�������`�f�[�^�\���̔z��
	@param pHanteis [out] ���ʂ��i�[���邠���蔻���f�[�^�\���̔z��

	ver0.70�`���Z���f�[�^�����[�h���܂��B���̊֐���DLL�������������Œ蒷�̃������̈���
	���[�h�����f�[�^���������ނ��߁A����GCD�f�[�^�̃f�[�^�����ω������ꍇ�ɂ͑Ή����邱�Ƃ�
	�ł��܂����B���̖����͏o���オ�����L�����N�^�[��GCD�f�[�^�������̐V����GCD�G�f�B�^��
	�ҏW���ꂽ�ꍇ�ɋN�����܂����A�����L�����N�^�[�������Ȃ��̂ł����Γ��ɖ����ł͂Ȃ����������܂����B

	���̊֐��ł�ver0.9�`����GCD�f�[�^���ǂݍ��ނ��Ƃ͂ł��܂����B
	��������CGoluahObject::CreateCellData���g�p���Ă��������B
*/
void   CGoluahObject::LoadCellData(char* pathname,GCD_CELL2_070* pCells,GCD_RECT* pRects,GCD_HANTEI* pHanteis) {
	funcd->loadcelldat(pathname, pCells, pRects, pHanteis);
}
/*!
	@brief �r�b�g�}�b�v�ǂݍ���
	@sa DI_FUNCTIONS_D::loadbmp
	@param PathName	�摜�f�[�^���ǂݍ��ރt�@�C����
	@param PalletFileName �p���b�g�f�[�^���ǂݍ��ރt�@�C����
	@return �ǂݍ��񂾉摜.NULL�͎��s

	8�r�b�g�C���f�b�N�X�J���[�̖����k�r�b�g�}�b�v�����[�h���܂��B
	PathName�̃t�@�C�������p���b�g�͒ʏ햳�������܂��B
	���[�h�����C���[�W�͕K��CGoluahObject::UnloadBitMap�Ŕj�����Ă��������B
*/
MYSURFACE* CGoluahObject::LoadBitmap(char* PathName,char* PalletFileName) {
	return (MYSURFACE*)funcd->loadbmp(PathName, PalletFileName);
}
/*!
	@brief �r�b�g�}�b�v���n��
	@sa DI_FUNCTIONS_D::unloadbmp
	@sa CGoluahObject::LoadBitmap
	@param Bitmap CGoluahObject::LoadBitmap�œǂݍ��񂾃r�b�g�}�b�v

	�ǂݍ��񂾃r�b�g�}�b�v���A�����[�h���܂�
*/
void   CGoluahObject::UnloadBitMap(MYSURFACE* Bitmap) {
	funcd->unloadbmp(Bitmap);
}
/*!
	@brief �Z���`��
	@param pBmps �g�p�����r�b�g�}�b�v�z��
	@param cdat �g�p�����Z���f�[�^�z��
	@param rdat �g�p�������`�f�[�^�z��
	@param cnum �`�悷���Z���ԍ�
	@param x �`����x���W
	@param y �`����y���W
	@param z �`����z���W
	@param Rotate ���]�p(degree)
	@param ReverseX x���]
	@param ReverseY y���]
	@param Color �`���̒��_Diffuse�F
	@param magx x�g�嗦
	@param magy y�g�嗦

	�Z���`�����s���܂��B
	�ʏ��̓I�u�W�F�N�g���b�Z�[�WGOBJMSG_DRAW��FALSE���Ԃ����Ƃɂ����A
	�I�u�W�F�N�g�f�[�^(GOBJECT)���炻�ꂼ���K���ȃp�����[�^���E�����ĕ`�悳���܂����A
	1�I�u�W�F�N�g�ň��x�ɂQ�ȏ��̃Z�����`�悵�����ꍇ�Ȃǂɂ��̊֐��𗘗p���邱�Ƃ��ł��܂��B

	@sa DI_FUNCTIONS_D::celldraw
	@sa CGoluahObject::SetTransform
	@sa CGoluahObject::SetParentMatrix
*/
void   CGoluahObject::CellDraw(MYSURFACE** pBmps,GCD_CELL2* cdat,GCD_RECT* rdat,
	DWORD cnum,int x,int y,float z,int Rotate,BOOL ReverseX,BOOL ReverseY,DWORD Color,float magx,float magy) 
{
	funcd->celldraw((void**)pBmps, cdat, rdat,
	 cnum, x, y, z, Rotate, ReverseX, ReverseY, Color, magx, magy);
}
/*!
	@brief �Z���`��
	@sa CGoluahObject::SetTransform
	@sa CGoluahObject::SetParentMatrix
	
	�ʏ��̓I�u�W�F�N�g���b�Z�[�WGOBJMSG_DRAW��FALSE���Ԃ����Ƃɂ��蓯�l�̕`�悪�s�����܂����A
	1�I�u�W�F�N�g�ň��x�ɂQ�ȏ��̃Z�����`�悵�����ꍇ�Ȃǂɂ��̊֐��𗘗p���邱�Ƃ��ł��܂��B
*/
void   CGoluahObject::CellDraw(GOBJECT *objdat)
{
	funcd->celldraw(
		(void**)objdat->pmsarr,
		objdat->pcdat,
		objdat->prdat,
		objdat->cnow,
		(int)objdat->x,
		(int)objdat->y,
		objdat->z,
		objdat->rot,
		objdat->muki ? (!objdat->revx) : objdat->revx,
		objdat->revy,
		objdat->color,
		objdat->magx,
		objdat->magy
		);
}
/*!
	@brief �r�b�g�}�b�v�`��
	@param pBmp �]�����r�b�g�}�b�v
	@param x1 �]�����̍�����x���W�l
	@param y1 �]�����̍�����y���W�l
	@param bltrect �]�������`
	@param magx x�g�嗦
	@param magy y�g�嗦
	@param revx x���]
	@param revy y���]
	@param z �]����z���W�l
	@param color �`���̒��_Diffuse�F

	CGoluahObject::LoadBitmap�Ń��[�h�����r�b�g�}�b�v���`�悵�܂��B
	�`����z���ɑ΂��Đ����ȕ��ʂɂȂ��܂��B

	@sa DI_FUNCTIONS_D::ckblt
	@sa CGoluahObject::SetTransform
	@sa CGoluahObject::SetBlend
	@sa CGoluahObject::SetParentMatrix
*/
void   CGoluahObject::CkBlt(MYSURFACE* pBmp,int x1,int y1,RECT bltrect,
	double magx,double magy,BOOL revx,BOOL revy,float z,DWORD color)
{
	funcd->ckblt(pBmp, x1, y1, bltrect,
	 magx,  magy, revx, revy, z, color);
}
/*!
	@brief �r�b�g�}�b�v�`��(2)

	@param pBmp �]�����r�b�g�}�b�v
	@param bltrect �]�������`
	@param rect �]�������`
	@param color �`���̒��_Diffuse�F

	@sa DI_FUNCTIONS_D::blt3d
	@sa CGoluahObject::SetTransform
	@sa CGoluahObject::SetBlend
	@sa CGoluahObject::SetParentMatrix

	CGoluahObject::LoadBitmap�Ń��[�h�����r�b�g�}�b�v���`�悵�܂��B
*/
void   CGoluahObject::Blt3D(MYSURFACE* pBmp,RECT bltrect,MYRECT3D rect,DWORD color)
{
	funcd->blt3d( pBmp, bltrect, rect, color);
}
/*!
	@brief �`�掞���W�ϊ��ݒ�
	@sa DI_FUNCTIONS_D::settrans
	@param BonVoyage TRUE:�ʏ�,FALSE:�X�N���[�����W�n�B�����̕ϐ����H

	�ʏ��A�X�e�[�W�̂����`�悳�����͈͂̓L�����N�^�[�̈ړ��ɂ����ĕω����܂����A
	���̊֐���FALSE���w�肷���ƃE�B���h�E�̍�����(0,0)�Ƃ������W�n�ŕ`�悳�����悤�ɂȂ��܂��B
	�ύX�����ꍇ�́A�`������TRUE�ɖ߂��Ă������ƁB
*/
void   CGoluahObject::SetTransform(BOOL BonVoyage) {
	funcd->settrans(BonVoyage);
}
/*!
	@brief ���u�����h�̎d�����ύX
	@sa DI_FUNCTIONS_D::setblend
	@sa GBLENDMODES

	�������̕��@���w���ł��܂��B�r�b�g�}�b�v�`���ɉe�����܂��B
	�ύX�����ꍇ�́A�`������GBLEND_HANTOUMEI�ɖ߂��Ă������ƁB
*/
void   CGoluahObject::SetBlend(DWORD type) {
	funcd->setblend(type);
}
/*!
	@brief �`�掞���W�ϊ��s���ݒ�
	@sa DI_FUNCTIONS_D::setparentmat

	@param pMatrix [in]�ݒ肷���ϊ��s��
	@param erase [in]FALSE�̏ꍇ�A�V�K�ɐݒ肳�����s���͌��̍s����pMatrix�̐�
	@param OldMatrix [out]�֐��̎��s�O�ɐݒ肳���Ă����ϊ��s�񂪕Ԃ����܂�

	���W�ϊ��}�g���N�X���w�肵�܂��B
	�r�b�g�}�b�v�`���E�Z���`���̍ۂɂ������̍��W�ϊ����K�p�������̂ŁA
	���܂��g���Β����@�\�ȕ`�悪�ł����͂��B���F�͕��ʂł����ǂˁB
	�ύX�����ꍇ��OldMatrix�ŕԂ����錳�̕ϊ��s���ɖ߂��܂��B
	�ʏ��͒P�ʍs�񂪐ݒ肳���Ă��܂��B
*/
void   CGoluahObject::SetParentMatrix(LPD3DXMATRIX pMatrix,BOOL erase,LPD3DXMATRIX OldMatrix) {
	funcd->setparentmat( pMatrix, erase, OldMatrix);
}
/*!
	@brief GCD�f�[�^���[�h
	@sa CGoluahObject::DestroyCellData
	@sa DI_FUNCTIONS_D::create_celldat

	����GCD�f�[�^�̍\���̂��ύX�������\���ɑΉ����邽�߁A
	�V�X�e�����Ŕz�񃁃������m�ۂ��Ă����Ƀf�[�^�����[�h���܂��B
	���̂��߁A���̊֐��Ń��[�h�����f�[�^�𓮓I�ɕύX���邱�Ƃ͔񐄏��ƂȂ��܂��B
	�ύX���s���ꍇ�A����GCD�̃t�H�[�}�b�g���ύX�����A
	���[�U�[��GCD�����������Ƃ��Ɍ듮�����N�����\���������܂��B�i�ʂɂ��܂��Ȃ�����?�j
*/
DWORD  CGoluahObject::CreateCellData(char* filename,GCD_CELL2 **cdat,GCD_RECT **rdat,GCD_HANTEI **hdat) {//�Z���f�[�^�ǂݍ��݁Bver0.90�ȍ~�ł�LoadCellDat���炱�����ɕύX�����ׂ�
	return funcd->create_celldat(filename, (void**)cdat, (void**)rdat, (void**)hdat);
}
/*!
	@brief GCD�f�[�^�j��
	@sa CGoluahObject::CreateCellData
	@sa DI_FUNCTIONS_D::destroy_celldat

	CGoluahObject::CreateCellData�Ń��[�h����GCD�f�[�^���j�����܂��B
*/
void   CGoluahObject::DestroyCellData(GCD_CELL2 **cdat,GCD_RECT **rdat,GCD_HANTEI **hdat) {//CreateCellDat�Ő������ꂽ�o�b�t�@���N���A
	funcd->destroy_celldat( (void**)cdat, (void**)rdat, (void**)hdat);
}
/*!
	@brief PNG/BMP�`���摜�ǂݍ���
	@sa DI_FUNCTIONS_D::loadimage
	@sa LoadBitmap
	@param PathName	�摜�f�[�^���ǂݍ��ރt�@�C���́A�g���q���������t�@�C����
	@param PalletFileName �p���b�g�f�[�^���ǂݍ��ރt�@�C���́A�g���q���������t�@�C����
	@return �ǂݍ��񂾉摜.NULL�͎��s

	PNG�Ή��ɔ����A�ǂݍ��ݏ����ɏ_������߂邽�߂ɍ����܂����B
	�g���q���������t�@�C���������A.png��.bmp�̂Q���ނ̃^�C�v���玩���I�����ēǂݍ��݂܂�(PNG�D���j�B
	���Ƃ́A�قƂ���LoadBitmap�ƕς����܂����B
*/
MYSURFACE* CGoluahObject::LoadImage(char* PathName, char* PalletFileName) {
	return (MYSURFACE*)funcd->loadimage(PathName, PalletFileName);
}
/*!
	@brief GCD�f�[�^���[�h�i�Q�j
	@sa CGoluahObject::DestroyCellData
	@sa DI_FUNCTIONS_D::create_celldat2
	@sa CreateCellData

	GCD�̂����k�ɑΉ����A�g���q�����������Ƃ����ǉ����ꂽ�֐��ł��B
	�g���q���������t�@�C���������A.gcm�i���k�j��.gcd�i�񈳏k�j�̂Q���ނ̃^�C�v���玩���I�����ēǂݍ��݂܂�(���k���D���j�B
	�����ȊO�́ACreateCellData�ƕς����܂����B
*/
DWORD  CGoluahObject::CreateCellData2(char* filename,GCD_CELL2 **cdat,GCD_RECT **rdat,GCD_HANTEI **hdat) {//�Z���f�[�^�ǂݍ��݁Bver0.90�ȍ~�ł�LoadCellDat���炱�����ɕύX�����ׂ�
	return funcd->create_celldat2(filename, (void**)cdat, (void**)rdat, (void**)hdat);
}


/*!
	@class CCharacterBase
	@brief �L�����N�^�[���{�N���X
	@ingroup GObjectBase

	DLL�̃��C���L�����N�^�[���쐬���邽�߂̃N���X�ł��B
	GObjectBase�ɁA�L�����N�^�[���g�p�����ł��낤(�Ǝv������)���������̋@�\���ǉ��B

	Init�`() �n�̏����� InitializeCharacter�֐������R�[�������܂��B
	���C���L�����N�^�[�ȊO�𐶐������ꍇ�́Anew�������Ƃ�InitializeCharacter
	���R�[�������K�v�������܂��B

	CBulletBase�̐�������"�e"�N���X�Ƃ��Ă��̃N���X���w�肷���ƁACBulletBase��
	�`�掞�ɂ��̃N���X(�I�u�W�F�N�g)�����[�h�����r�b�g�}�b�v , GCD�f�[�^, �T�E���h����
	���p���܂��B
*/

/*!
	@brief �\�z(1)
	@param info CreateCharacter�̈����ŃV�X�e�������󂯎������\����
*/
CCharacterBase::CCharacterBase(CDI_CHARACTERINFO2 *info) : CGoluahObject(FALSE)
{
	parent_char = NULL;

	GetGObject()->tid	= info->tid;//�`�[��ID
	keyinput			= info->keyinput;//���Ԃ����L�[���͂��󂯎��邩
	pal_number			= info->color;//�p���b�g�ԍ�
	g_chardir			= info->dir;
	option_flags		= info->options_flag;

	CCharacterBaseCreate();
}

/*!
	@brief �\�z(2)
	@param parent ���C���L�����N�^�[�N���X

	���ѓ���̃I�u�W�F�N�g�𐶐����邽�߂Ɏg���܂�
*/
CCharacterBase::CCharacterBase(CCharacterBase *parent/* = NULL */) : CGoluahObject(FALSE)
{
	parent_char = parent;

	if(parent!=NULL){
		keyinput = parent->keyinput;//���Ԃ����L�[���͂��󂯎��邩
		pal_number = parent->pal_number;//�p���b�g�ԍ�
		GetGObject()->tid		= parent->GetGObject()->tid;//�`�[��ID
		g_chardir = parent->g_chardir;
	}
	else {
		keyinput = 0;//���Ԃ����L�[���͂��󂯎��邩
		pal_number = 0;//�p���b�g�ԍ�
		GetGObject()->tid		= 0;//�`�[��ID
	}

	CCharacterBaseCreate();
}

/*!
	@brief �R���X�g���N�^�̋��ʏ���
*/
void CCharacterBase::CCharacterBaseCreate()
{
	mysounds = NULL;
	mysound_numbers = 0;

	GOBJECT* pdat = GetGObject();

	for(int i=0;i<GCDMAX_IMAGES;i++)bitmaps[i]=NULL;
	cells=NULL;
	hantei=NULL;
	rects=NULL;
	mysounds = NULL;

	pdat->objtype	= GOBJTYPE_CHARACTER;//�I�u�W�F�N�g�^�C�v
	pdat->winfo		= &(this->waz);
	ZeroMemory(pdat->winfo,sizeof(WAZAINFO));

	base_z	=ZZAHYO_CHARACTER;
	front_z	=ZZAHYO_CHARACTER_F;
	back_z	=ZZAHYO_CHARACTER_B;

	chainComboEnabled = TRUE;	//�`�F�[���R���{�L��
	isSuperArmer = FALSE;		//�X�[�p�[�A�[�}�[����
	isAutoGuard = FALSE;		//�I�[�g�K�[�h����

	base_voice_damage1 = 1;
	base_voice_damage2 = 2;
	base_voice_damage3 = 3;
	base_voice_ko = 0;
}

/*!
	@brief ������

	CreateCharacter���Ƀ}�N�����玩�����s�������蔤�ɂȂ��Ă��܂��B
	���Ԃ�������ꍇ�̓I�[�o�[���C�h���Ă�������
*/
void CCharacterBase::InitializeCharacter()
{
	InitParameters();
	InitAttackInfo();
	InitGCDandBMP();
	InitWazInfo();
	InitMySound();
	InitBullets();
}

/*!
	@brief �j��
*/
CCharacterBase::~CCharacterBase()
{
	ReleaseGCDandBMP();
	ReleaseMySound();
	if(mysounds) free(mysounds);
}

/*!
	@brief ���b�Z�[�W�����֐�

	���b�Z�[�W�������A�K�؂ȉ��z�֐��ɐU�蕪���܂�
*/
DWORD CCharacterBase::Message(DWORD msg,LPVOID pd,DWORD prm)
{
	switch(msg){
	case GOBJMSG_DOTOJYO://�o��
		ChangeAction( ACTID_TOJYO );return(TRUE);
	case GOBJMSG_DOYOUWIN://����
		ChangeAction( ACTID_SYORI );return(TRUE);
	case GOBJMSG_DOTIMEOVERLOSE:
		ChangeAction( ACTID_TIMEOVERLOSE );return(TRUE);
	case GOBJMSG_TAIKI://�ҋ@���Ƃ����b�Z�[�W
		ChangeAction( ACTID_TAIKI );
		break;
	case GOBJMSG_ACTION:
		{
			DWORD result;

			PreAction();
			result = Action();
			PostAction();
			return result;
		}
	case GOBJMSG_KOUTAI://���サ�냁�b�Z�[�W
		if(GetGObject()->aid == ACTID_TAIKICYU){
			ChangeAction( ACTID_KOUTAIIN );
			return(TRUE);
		}
		return(0);
	case GOBJMSG_KOUTAI2://���サ�냁�b�Z�[�W2�i�̂��т��Ɠo���|�[�Y�����߂Ăėǂ��j
		ChangeAction( ACTID_KOUTAI );
		break;
	case GOBJMSG_CLIPX:
		if(GetGObject()->aid == ACTID_KOUTAI)return(TRUE);//���㎞�̓N���b�v���������Ȃ�
		else return(0);
	default:
		//���Ƃ̃��b�Z�[�W�̓I�u�W�F�N�g���{�N���X�ɔC����
		return(CGoluahObject::Message(msg,pd,prm));
	}
	return(0);
}


/*!
	@brief GOBJMSG_ACTION ���b�Z�[�W����

	CCharacterBase�Œ��`�����Ă������{�s���֏������U�蕪���܂�
*/
DWORD CCharacterBase::Action()
{
	switch(GetGObject()->aid){
	case ACTID_NEUTRAL	:act_neutral();break;//default actions
	case ACTID_CROUCH	:act_crouch();break;
	case ACTID_RAKKA	:act_rakka();break;
	case ACTID_TOJYO	:act_tojyo();break;
	case ACTID_SYORI	:act_win();break;
	case ACTID_WALKF	:act_walkf();break;//normal actions
	case ACTID_WALKB	:act_walkb();break;
	case ACTID_JAMPS	:act_jamps();break;
	case ACTID_JAMPC	:act_jampc();break;
	case ACTID_JAMPF	:act_jampf();break;
	case ACTID_JAMPB	:act_jampb();break;
	case ACTID_RAKKA2	:act_rakka2();break;
	case ACTID_ATT_SA	:act_att_sa();ChainCombo(CHAIN_SA);break;//attack actions
	case ACTID_ATT_SB	:act_att_sb();ChainCombo(CHAIN_SB);break;
	case ACTID_ATT_SC	:act_att_sc();ChainCombo(CHAIN_SC);break;
	case ACTID_ATT_CA	:act_att_ca();ChainCombo(CHAIN_CA);break;
	case ACTID_ATT_CB	:act_att_cb();ChainCombo(CHAIN_CB);break;
	case ACTID_ATT_CC	:act_att_cc();ChainCombo(CHAIN_CC);break;
	case ACTID_ATT_JA	:act_att_ja();break;
	case ACTID_ATT_JB	:act_att_jb();break;
	case ACTID_ATT_JC	:act_att_jc();break;
	case ACTID_TAIKI	:act_taiki();break;
	case ACTID_KOUTAIOUT:act_koutai_out();break;
	case ACTID_KOUTAIIN	:act_koutai_in();break;
	case ACTID_KOUTAI	:act_koutai();break;
	case ACTID_KOUTAI2	:act_koutai2();break;
	case ACTID_TAIKICYU	:act_taikicyu();break;
	case ACTID_STRIKERCOMEON:act_strikercomeon();break;
	case ACTID_TIMEOVERLOSE	:act_timeoverlose();break;
	//case ACTID_SYORI2IN		:act_win2_in();break;
	//case ACTID_SYORI2POSE	:act_win2_pose();break;
	default:return(0);
	}
	return(TRUE);
}


/*!
	@brief GOBJMSG_ACTION ���b�Z�[�W �O����

	�����I�ɍs���Ȃ����΂Ȃ��Ȃ����炩�̏������s���܂�
*/
void CCharacterBase::PreAction()
{
	//�`�F�[���R���{���X�g�N���A
	if(!(GetGObject()->aid & 0xFFFD0000)){
		if(GetGObject()->counter > 2)chainlist=0;
	}

	//�U�������������s���܂��B�����ȊO�̓����ł����肽���ꍇ�̓I�[�o�[���C�h���Ēǉ����Ă�������
	if(pdat->aid==ACTID_NEUTRAL ||
	   pdat->aid==ACTID_CROUCH  ||
	   pdat->aid==ACTID_TOJYO   ||
	   !(pdat->aid & (ACTID_KUCYU | ACTID_ATTACK | ACTID_NAGE | ACTID_HISSATU | ACTID_SYSTEM | ACTID_INOUT))
	   )
	{
		Furimuki();
	}

	//�_���[�W���Đ�
	if(pdat->hp > 0)final_voice_flag = FALSE;
	if(pdat->counter==0 && !final_voice_flag)
	{
		switch(pdat->aid)
		{
		//�����炢
		case ACTID_DAMAGE1	:
		case ACTID_DAMAGEC1	:
		case ACTID_DAMAGE1A	:
		case ACTID_DAMAGEC1A:
		case ACTID_DAMAGEJ1	:
			if(GetRandNum(5)==0)PlayMySound(base_voice_damage1);
			break;
		//�����炢
		case ACTID_DAMAGE2	:
		case ACTID_DAMAGEC2	:
		case ACTID_DAMAGE2A	:
		case ACTID_DAMAGEC2A:
		case ACTID_DAMAGEJ2	:
			if(GetRandNum(4)==0)PlayMySound(base_voice_damage2);
			break;
		//�����炢
		case ACTID_DAMAGE3	:
		case ACTID_DAMAGEC3	:
		case ACTID_DAMAGE3A	:
		case ACTID_DAMAGEC3A:
		case ACTID_DAMAGEJ3	:
			if(GetRandNum(3)==0)PlayMySound(base_voice_damage3);
			break;

		//���̑����炢
		case ACTID_DOWN			:
		case ACTID_FUTTOBI		:
		case ACTID_FUTTOBI2		:
		case ACTID_TATAKITUKE1A	:
		case ACTID_TATAKITUKE2A	:
		case ACTID_TATAKITUKE1B	:
		case ACTID_TATAKITUKE2B	:
			if(GetRandNum(2)==0)PlayMySound(base_voice_damage3);
			break;

		//KO
		case ACTID_FINALDOWN	://��
		case ACTID_FINALDOWN2	://��
		case ACTID_KAITENFINISH	:
			final_voice_flag = TRUE;
			PlayMySound(base_voice_ko);
			break;
		}
	}
}
/*!
	@brief GOBJMSG_ACTION ���b�Z�[�W �㏈��

	�����I�ɍs���Ȃ����΂Ȃ��Ȃ����炩�̏������s���܂�
*/
void CCharacterBase::PostAction()
{
	//�`�F�[���R���{���X�g�N���A
	if(!(GetGObject()->aid & 0xFFFD0000)){
		if(GetGObject()->counter > 2)chainlist=0;
	}

	//�U�������������s���܂��B�����ȊO�̓����ł����肽���ꍇ�̓I�[�o�[���C�h���Ēǉ����Ă�������
	if(pdat->aid==ACTID_NEUTRAL ||
	   pdat->aid==ACTID_CROUCH
	   )
	{
		Furimuki();
	}
}


/*!
	@brief GOBJMSG_COMMAND ���b�Z�[�W����

	�s��ID�ɂ����ACommand�n�̉��z�֐��ɏ������U�蕪���܂�
*/
void CCharacterBase::Command()
{
	DWORD key_now = (*funcs->getkey)(keyinput,0);
	BOOL callstriker=FALSE;
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_SYSTEM){//�V�X�e�����`���쒆�Ȃ̂ŁA���{�I�ɂ͂������Ȃ�
		Command_OnSystem(key_now);
		return;
	}
	if(pdat->aid == ACTID_TOJYO || pdat->aid==ACTID_SYORI)return;
	if(pdat->aid == ACTID_TIMEOVERLOSE)return;
	if(pdat->aid & ACTID_NAGE)return;
	if(pdat->aid & ACTID_INOUT)return;

	//�X�g���C�J�[���Ă�
	if( (key_now&KEYSTA_FOWORD) && (key_now&KEYSTA_DOWN) && (key_now&KEYSTA_BD2) ){
		if(Message2System(MSGOBJ2SYS_STRIKER1,0)){
			ADDEFFECT(EFCTID_COMEON,(int)pdat->x,(int)pdat->y-100,pdat->muki);
			callstriker=TRUE;
		}
	}
	else if( (key_now&KEYSTA_BACK) && (key_now&KEYSTA_DOWN) && (key_now&KEYSTA_BD2) ){
		if(Message2System(MSGOBJ2SYS_STRIKER2,0)){
			ADDEFFECT(EFCTID_COMEON,(int)pdat->x,(int)pdat->y-100,pdat->muki);
			callstriker=TRUE;
		}
	}

	if(pdat->aid & ACTID_HISSATU){//�K�E�Z���i���{�I�ɂ������ׂ��炸�j
		Command_OnHissatuAttacking(key_now);
		return;
	}

	//�K�E�Z�R�}���h����
	if (Command_Hissatu(key_now))
		return;

	if(pdat->aid & ACTID_ATTACK){//�U�����쒆�i���{�I�ɂ������Ȃ��B�`�F�[���R���{�o���Ȃ炱���ł������j
		Command_OnAttacking(key_now);
		return;
	}

	//���{���쒆
	if (Command_OnNormal(key_now) || pdat->aid & ACTID_KUCYU)
		return;

	//�X�g���C�J�[
	if(callstriker){
		pdat->aid = ACTID_STRIKERCOMEON;
		return;
	}
	//���[����
	else if( (key_now&KEYSTA_FOWORD) && (key_now&KEYSTA_BD2) && !(key_now&KEYSTA_DOWN) ){
		if(Message2System(MSGOBJ2SYS_KOUTAI1,0)){
			pdat->aid = ACTID_KOUTAIOUT;
			return;
		}
	}
	else if( (key_now&KEYSTA_BACK) && (key_now&KEYSTA_BD2) && !(key_now&KEYSTA_DOWN) ){
		if(Message2System(MSGOBJ2SYS_KOUTAI2,0)){
			pdat->aid = ACTID_KOUTAIOUT;
			return;
		}
	}

	if(pdat->aid==ACTID_STRIKERCOMEON){//���͂��Ȃ����΃|�[�Y�L��
		if(!(key_now & (KEYSTA_UP2|KEYSTA_DOWN2|KEYSTA_FOWORD2|KEYSTA_BACK2))){
			return;
		}
	}

	Command_Normal(key_now);
}


/*!
	@brief GOBJMSG_COMMAND ���b�Z�[�W ���������i�K�E�Z���j
*/
BOOL CCharacterBase::Command_Hissatu(DWORD keyinfo)
{
	return FALSE;
}


/*!
	@brief GOBJMSG_COMMAND ���b�Z�[�W ���������i�ʏ퓮�쒆�j
*/
BOOL CCharacterBase::Command_Normal(DWORD keyinfo)
{
	GOBJECT* pdat = GetGObject();

	if(keyinfo & KEYSTA_UP){
		pdat->aid = ACTID_JAMPS;
	}
	else if(keyinfo & KEYSTA_DOWN){//���Ⴊ��
		pdat->aid = ACTID_CROUCH;
	}
	else if(keyinfo & KEYSTA_FOWORD){//�O����
		pdat->aid = ACTID_WALKF;
	}
	else if(keyinfo & KEYSTA_BACK){//��������
		pdat->aid = ACTID_WALKB;
	}
	else{//�j���[�g�����|�[�Y
		pdat->aid = ACTID_NEUTRAL;
	}

	return TRUE;
}

/*!
	@brief GOBJMSG_COMMAND ���b�Z�[�W ���������i�V�X�e�����쒆�j
*/
BOOL CCharacterBase::Command_OnSystem(DWORD keyinfo)
{
	return FALSE;
}

/*!
	@brief GOBJMSG_COMMAND ���b�Z�[�W ���������i�K�E�U�����쒆�j
*/
BOOL CCharacterBase::Command_OnHissatuAttacking(DWORD keyinfo)
{
	return FALSE;
}

/*!
	@brief GOBJMSG_COMMAND ���b�Z�[�W ���������i�ʏ��U�����쒆�j
*/
BOOL CCharacterBase::Command_OnAttacking(DWORD keyinfo)
{
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_KUCYU){////�W�����v���쒆
		return FALSE;
	}
	else{
		if(chainComboEnabled && keyinfo & 0x22220000){
			if(keyinfo & KEYSTA_DOWN){
				if(keyinfo & KEYSTA_BC2){if(ChainCombo(CHAIN_CC)){ChangeAction(ACTID_ATT_CC);return TRUE;}}
				else if(keyinfo & KEYSTA_BB2){if(ChainCombo(CHAIN_CB)){ChangeAction(ACTID_ATT_CB);return TRUE;}}
				else if(keyinfo & KEYSTA_BA2){if(ChainCombo(CHAIN_CA)){ChangeAction(ACTID_ATT_CA);return TRUE;}}
			}
			else{
				if(keyinfo & KEYSTA_BC2){if(ChainCombo(CHAIN_SC)){ChangeAction(ACTID_ATT_SC);return TRUE;}}
				else if(keyinfo & KEYSTA_BB2){if(ChainCombo(CHAIN_SB)){ChangeAction(ACTID_ATT_SB);return TRUE;}}
				else if(keyinfo & KEYSTA_BA2){if(ChainCombo(CHAIN_SA)){ChangeAction(ACTID_ATT_SA);return TRUE;}}
			}
		}
	}

	return FALSE;
}

/*!
	@brief GOBJMSG_COMMAND ���b�Z�[�W ���������i�ʏ퓮���j
*/
BOOL CCharacterBase::Command_OnNormal(DWORD keyinfo)
{
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_KUCYU){//�W�����v���쒆
		if(keyinfo & 0x22220000){
			if(keyinfo & KEYSTA_BC2){pdat->aid = ACTID_ATT_JC;return TRUE;}
			else if(keyinfo & KEYSTA_BB2){pdat->aid = ACTID_ATT_JB;return TRUE;}
			else if(keyinfo & KEYSTA_BA2){pdat->aid = ACTID_ATT_JA;return TRUE;}
		}
		return FALSE;
	}

	//�n�㓮�쒆
	if(keyinfo & 0x22220000){
		if(keyinfo & KEYSTA_DOWN){
			if(keyinfo & KEYSTA_BC2){pdat->aid = ACTID_ATT_CC;return TRUE;}
			else if(keyinfo & KEYSTA_BB2){pdat->aid = ACTID_ATT_CB;return TRUE;}
			else if(keyinfo & KEYSTA_BA2){pdat->aid = ACTID_ATT_CA;return TRUE;}
		}
		else{
			if(keyinfo & KEYSTA_BC2){
				pdat->aid = ACTID_ATT_SC;return TRUE;
			}
			else if(keyinfo & KEYSTA_BB2){
				pdat->aid = ACTID_ATT_SB;return TRUE;
			}
			else if(keyinfo & KEYSTA_BA2){
				pdat->aid = ACTID_ATT_SA;return TRUE;
			}
		}
	}

	if(pdat->aid == ACTID_JAMPS){
		if(keyinfo & KEYSTA_FOWORD){//�O�W�����v
			pdat->aid = ACTID_JAMPF;
			return TRUE;
		}
		else if(keyinfo & KEYSTA_BACK){//���W�����v
			pdat->aid = ACTID_JAMPB;
			return TRUE;
		}
	}

	return FALSE;
}


/*!
	@brief GOBJMSG_TOUCHA ���b�Z�[�W ����
*/
DWORD CCharacterBase::TouchA(ATTACKINFO *info,DWORD ta_eid)
{
	GOBJECT* pdat = GetGObject();

	// �ҋ@/�ޔ𒆂͖��������Ⴄ
	if(pdat->aid & ACTID_INOUT)return(TOUCHA_AVOID);
	//�������͖��������Ⴄ
	if(pdat->aid & ACTID_NAGE)return(TOUCHA_AVOID);

	//�X�[�p�[�A�[�}�[�����B�U���𖳎�
	if(isSuperArmer){
		if(pdat->hp>0){
			AddEffect(EFCTID_SUPERARMER,(int)pdat->x,(int)(pdat->y-50.0f),pdat->muki);
			return TOUCHA_MUSI;
		}
	}

	//�I�[�g�K�[�h�����B�U���E���炢���ȊO�͖������K�[�h
	BOOL auto_guard = FALSE;
	if(isAutoGuard)
	{
		if(IsCom())
		{
			auto_guard = (rand()%2) ? TRUE : FALSE;
		}
		else
		{
			DWORD crnt_key = GetKey(0);
			if((crnt_key&KEYSTA_FOWORD)||(crnt_key&KEYSTA_UP))
			{
				auto_guard = FALSE;
			}
			else auto_guard = TRUE;
		}
	}

	if(auto_guard)
	{
		if(CANNOT_GUARD(info->guard) && (pdat->aid!=ACTID_RAKKA))return 0;//�ǂ̃K�[�h���ł��Ȃ��ꍇ�̓K�[�h�ł��Ȃ�
		if(pdat->aid & ACTID_ATTACK)return 0;
		if(pdat->aid & ACTID_KURAI)return 0;
		if(pdat->aid & ACTID_KUCYU)
		{
			//�󒆃K�[�h
			return (info->guard & GUARDINFO_XJAMP) ? 0 : TOUCHA_GUARDJ;
		}
		if((info->guard & GUARDINFO_XCROUCH) && (info->guard & GUARDINFO_XSTAND))
		{
			//�n���K�[�h�s��
			return 0;
		}
		if(pdat->aid & ACTID_SYAGAMI)
		{
			//���K�[�h
			if(info->guard & GUARDINFO_XCROUCH)return TOUCHA_GUARDS;
			return TOUCHA_GUARDC;
		}
		else
		{
			//�����K�[�h
			if(info->guard & GUARDINFO_XSTAND)return TOUCHA_GUARDC;
			return TOUCHA_GUARDS;
		}
	}

	return CGoluahObject::TouchA(info,ta_eid);
}

/*!
	@brief GGOBJMSG_COMMANDCOM ���b�Z�[�W ����
*/
DWORD CCharacterBase::CommandCOM(DWORD wid)
{
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_SYSTEM)return(CmdCom_OnSystem(wid));
	if(pdat->aid == ACTID_TOJYO || pdat->aid==ACTID_SYORI)return(FALSE);
	if(pdat->aid & ACTID_NAGE)return(FALSE);
	if(pdat->aid & ACTID_INOUT)return(FALSE);
	
	if(pdat->aid & ACTID_HISSATU){//�K�E�Z���i���{�I�ɂ������ׂ��炸�j
		return(CmdCom_OnHissatuAttacking(wid));
	}

	if(pdat->aid & ACTID_KUCYU){
		return(CmdCom_OnKucyu(wid));
	}
	else{
		return(CmdCom_OnNormal(wid));
	}
}

/*!
	@brief GGOBJMSG_COMMANDCOM���b�Z�[�W ��������
*/
DWORD CCharacterBase::CmdCom_OnSystem(DWORD wid)
{
	return(FALSE);
}

/*!
	@brief GGOBJMSG_COMMANDCOM���b�Z�[�W ��������
*/
DWORD CCharacterBase::CmdCom_OnHissatuAttacking(DWORD wid)
{
	return(FALSE);
}

/*!
	@brief GGOBJMSG_COMMANDCOM���b�Z�[�W ��������
*/
DWORD CCharacterBase::CmdCom_OnKucyu(DWORD wid)
{
	//�����͌p�����ŏ������Ȃ��Ƃ����Ȃ��̂ŁA�_��
	if(wid&ACTID_NAGE)return(FALSE);

	if(GetGObject()->aid & ACTID_ATTACK){//�󒆍U����
		if(wid&ACTID_HISSATU && wid&ACTID_KUCYU)return(TRUE);//�󒆕K�E�Z�Ȃ�OK
		return(FALSE);//�����ȊO�͂���
	}
	else{//�󒆒ʏ퓮�쒆
		if(wid&ACTID_ATTACK && wid&ACTID_KUCYU)return(TRUE);//�󒆒ʏ��Z�Ȃ�OK
		return(FALSE);//�����ȊO�͂���
	}
}

/*!
	@brief GGOBJMSG_COMMANDCOM���b�Z�[�W ��������
*/
DWORD CCharacterBase::CmdCom_OnNormal(DWORD wid)
{
	//�����͌p�����ŏ������Ȃ��Ƃ����Ȃ��̂ŁA�_��
	if(wid&ACTID_NAGE)return(FALSE);

	if(GetGObject()->aid & ACTID_ATTACK){//�n���U����
		if(wid&ACTID_NAGE)return(FALSE);// �����Z�ł̃L�����Z���͕s��
		if(wid&ACTID_HISSATU && !(wid&ACTID_KUCYU))return(TRUE);//�n���K�E�Z�Ȃ�OK
		if(chainComboEnabled)
		{
			switch(wid)
			{
			case ACTID_ATT_SA:return( ChainCombo(CHAIN_SA) );
			case ACTID_ATT_SB:return( ChainCombo(CHAIN_SB) );
			case ACTID_ATT_SC:return( ChainCombo(CHAIN_SC) );
			case ACTID_ATT_CA:return( ChainCombo(CHAIN_CA) );
			case ACTID_ATT_CB:return( ChainCombo(CHAIN_CB) );
			case ACTID_ATT_CC:return( ChainCombo(CHAIN_CC) );
			}
		}
		return(FALSE);//����
	}
	else{//�n���ʏ퓮��
		if(wid&ACTID_KUCYU && wid&ACTID_ATTACK)return(FALSE);//�󒆍U���͂���
		return(TRUE);//�����ȊO�Ȃ�OK
	}
}

/*!
	@brief �U���͏��񏉊���

	���ۂ̃L�����N�^�[�N���X�ł͂��̊֐����I�[�o�[���C�h����
	�U���͏��������������鏈�����L�q���Ă�������
*/
void CCharacterBase::InitAttackInfo()
{
}


/*!
	@brief GCD/BMP ���[�h

	�L�����N�^�[�t�H���_�� cell.gcd �� image1�`12.bmp �����[�h���܂��B
	�ʂ̃t�@�C�����̃t�@�C�����g�p�������ꍇ�̓I�[�o�[���C�h���ď������ύX���܂��B
*/
void CCharacterBase::InitGCDandBMP()
{
	int i;
	GOBJECT* pdat = GetGObject();
	if(pdat==NULL)return;
	char filename[256],palname[256];

#ifdef _DEBUG
	if(pal_number==0)
		OutputDebugString("�L����DLL [warning] : �p���b�g�ԍ���0�ł��B");
#endif

	//"image?.bmp" ���ǂݍ���
	sprintf(palname,"%s\\pal%d",g_chardir,pal_number);
	for(i=0;i<GCDMAX_IMAGES;i++){
		sprintf(filename,"%s\\image%d",g_chardir,i+1);
		bitmaps[i] = (MYSURFACE*) ( (*funcd->loadimage)(filename,palname) );
	}

	//"cell.gcd"���ǂݍ���
	sprintf(filename,"%s\\cell",g_chardir);
	funcd->create_celldat2(filename, (void**)&cells, (void**)&rects, (void**)&hantei);

	//pdat�ɓ����Ă���
	pdat->phdat			= hantei;
	pdat->pmsarr		= bitmaps;
	pdat->pcdat			= cells;
	pdat->prdat			= rects;
}

/*!
	@brief GCD/BMP �A�����[�h
	@sa InitGCDandBMP
*/
void CCharacterBase::ReleaseGCDandBMP()
{
	for(int i=0;i<GCDMAX_IMAGES;i++){
		if(bitmaps[i]!=NULL){
			funcd->unloadbmp(bitmaps[i]);
		}
	}
	if(cells!=NULL) {
		funcd->destroy_celldat((void**)&cells, (void**)&rects, (void**)&hantei);
	}
	else {
		if(hantei!=NULL)free(hantei);
		if(rects!=NULL)free(rects);
	}
}

/* !
	@brief MYSOUND�\���̂����r����

	@sa CCharacterBase::InitMySound
	�\�[�g�p�AInitMySound�֐������Ŏg�p���Ă܂��B
*/
/*bool mscomp(const MYSOUND& val1, const MYSOUND& val2)
{
	return val1.number < val2.number;
}*/

/*!
	@brief wav�T�E���h�֌W

	@param path_name �T�E���h�̓ǂݍ��݂��s����p�X(NULL�̏ꍇ�A"sound")
	@param list_name �T�E���h�ԍ���wav�t�@�C�����̑Ή����L�q�������`�t�@�C����(NULL�̏ꍇ"list.txt")
*/
void CCharacterBase::InitMySound(char* path_name,char* list_name)
{
	ReleaseMySound();

	if(!path_name)path_name = MYSOUND_PATH;
	if(!list_name)list_name = MYSOUND_LIST;

	unsigned int i;
	char *filename = new char [256];
	char *buff = NULL;
	std::vector<char*>		name_list;
	std::vector<int>	number_list;
	std::vector<LPVOID>	sound_list;
//	std::vector<MYSOUND>	sound_list;

	do
	{
		//�e�L�X�g�����t�@�C�����̃��X�g���擾
		UINT bufflen;
		sprintf(filename, "%s\\%s\\%s",GetCharDir(),path_name,list_name);
		File2Mem(filename, &buff, &bufflen);
		if(!buff)break;

		//�Ă��ۂ����`�ȕϐ�
		char* s = buff;
		char* tmpstr = NULL;
		int tmpno = 0;

		//�t�@�C�����̃��X�g���擾
		while((UINT)(s-buff)<bufflen-1)
		{
			if(*s=='#')
			{
				tmpstr = new char [32];
				s++;
				if(2==sscanf( s,"%d %s",&tmpno,tmpstr) && tmpno >= 0)
				{
					name_list.push_back(tmpstr);
					number_list.push_back(tmpno);

/*					MYSOUND tmpms;

					tmpms.number	= tmpno;
					tmpms.sound		= NULL;
					sound_list.push_back(tmpms);*/
				}
				else
				{
					DELETE_ARRAY(tmpstr);
				}
			}
			else
			{
				s++;
				if(*(s - 1) & 0x80)s++;
			}
		}
		if(name_list.size()==0)break;

		//�T�E���h�����[�h
		int success_num = 0;
		for(i=0;i<name_list.size();i++)
		{
			sprintf(filename,"%s\\%s\\%s.wav",GetCharDir(),path_name,name_list[i]);
			sound_list.push_back( (*funcs->loadmysound)(filename) );
			if(sound_list[i])success_num++;

			/*if(i < sound_list.size())
			{
				sound_list[i].sound = (*funcs->loadmysound)(filename);
				if (sound_list[i].sound) success_num++;
			}*/
		}
		if(success_num==0)break;

		// �f�[�^���\�[�g
//		std::stable_sort< std::vector< MYSOUND >::iterator >(sound_list.begin(), sound_list.end(), mscomp);

		//�z�����p�ӂ��āA���������������ێ����Ă���
		mysounds = new LPVOID [success_num+1];
		mysound_numbers = new int [success_num];
		/*mysounds = (LPMYSOUND)malloc( sizeof(MYSOUND) * success_num);
		mysound_numbers = success_num;*/
		success_num = 0;//�g���܂킵
		for(i=0;i<name_list.size();i++)
		{
			if(sound_list[i])
			{
				mysounds[success_num] = sound_list[i];
				mysound_numbers[success_num] = number_list[i];
				success_num++;
			}

/*			// �T�E���h��NULL�̏ꍇ�̓p�X�B
			if ( (i < sound_list.size()) && sound_list[i].sound )
			{
				if ( ( (i > 0) && (sound_list[i - 1].number == sound_list[i].number) ) )
				{
					// �����ԍ��������̂Ŏ��s�A�z�����P���炷�B
					mysound_numbers--;
					mysounds = (LPMYSOUND)realloc(mysounds, sizeof(MYSOUND) * mysound_numbers);
					KillMySound(sound_list[i].sound);	// �g���Ȃ��̂ł��ڂ���
				}
				else
				{
					mysounds[success_num] = sound_list[i];
					success_num++;
				}
			}*/
		}
		//�I�[���m�p
		mysounds[success_num] = NULL;
	}
	while(0);
	DELETE_ARRAY( buff );
	DELETE_ARRAY( filename );
	for(i=0;i<name_list.size();i++)DELETE_ARRAY( name_list[i] );
}

/*!
	@brief wav�T�E���h�Đ�
	@param number �T�E���h�ԍ�
	@sa InitMySound

	number�Ŏw�肳�ꂽ�ԍ��̃T�E���h���Đ����܂��B
*/
void CCharacterBase::PlayMySound(DWORD number)
{
	if(!mysounds)return;

	UINT i=0;
	while(mysounds[i])
	{
		if(mysound_numbers[i]==number)
		{
			funcs->playmysound(mysounds[i]);
			return;
		}
		i++;
	}

	// �񕪒T�����g���Č���
/*	UINT i=0, j=mysound_numbers-1;
	while(i <= j)
	{
		UINT cent = (i + j) / 2;

		if (mysounds[cent].number < number)
			i = cent + 1;
		else if (mysounds[cent].number > number)
			j = cent - 1;
		else
		{
			// ����
			funcs->playmysound(mysounds[cent].sound);
			return;
		}
	}*/
}

/*!
	@brief �T�E���h�j��
	@sa InitMySound
*/
void CCharacterBase::ReleaseMySound()
{
	if(!mysounds)return;

	UINT i=0;
	while(mysounds[i])
	{
		funcs->killmysound(mysounds[i]);
		i++;
	}
	DELETE_ARRAY( mysounds );
	DELETE_ARRAY( mysound_numbers );

/*	for(UINT i=0; i < mysound_numbers; i++)
	{
		funcs->killmysound(mysounds[i].sound);
	}
	free(mysounds);
	mysounds = NULL;
	mysound_numbers = 0;*/
}

/*!
	@brief ��������

	@param acc_g �d�͉����x(1.0���W��)
	@param cyakuchi y���W��0�ȏ��ɒB�����ۂɁA�s���J�ڂ��s�����ǂ���
	@param toaid y���W��0�ȏ��ɒB�����ۂɑJ�ڂ����s��ID
*/
void CCharacterBase::JiyuuRakka(double acc_g,BOOL cyakuchi,DWORD toaid)
{
	GOBJECT* pdat = GetGObject();

	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy += acc_g;
	if(cyakuchi){
		CyakuchiHantei(toaid);
	}
}

/*!
	@brief ���n���菈��
	@param toaid ���n���Ă����Ɣ��肳�ꂽ�ꍇ�ɑJ�ڂ����s��ID

	y���W��0�ȏ��̂Ƃ��A���n���Ă����Ɣ��肳���܂�
*/
void CCharacterBase::CyakuchiHantei(DWORD toaid)
{
	GOBJECT* pdat = GetGObject();

	if(pdat->y > 0){
		pdat->y=0;
		pdat->aid=toaid;
		Furimuki();
	}
}

/*-----------------------------------------------------------------------------
	Z�ʒu����
-------------------------------------------------------------------------------*/
void CCharacterBase::ZBack(){GetGObject()->z=back_z;}
void CCharacterBase::ZFront(){GetGObject()->z=front_z;}

/*!
	@brief �U������ ����
	@return TRUE:�U�������������s���A�������ύX���ꂽ

	�^�[�Q�b�g�I�u�W�F�N�g����ɂ���x���W�𔻒肵�ăI�u�W�F�N�g�̌������ύX���܂��B
*/
BOOL CCharacterBase::Furimuki()
{
	GOBJECT* pdat = GetGObject();
	BOOL muki_prev = pdat->muki;
	GOBJECT *pedat = (GOBJECT*)(*funco->getinfo)(pdat->eid);

	if(pdat==NULL)return(FALSE);
	if(pedat==NULL)return(FALSE);

	if(pdat->x > pedat->x )
		pdat->muki = TRUE;
	else
		pdat->muki = FALSE;

	if(muki_prev!=pdat->muki)return(TRUE);
	else return(FALSE);
}

/*-----------------------------------------------------------------------------
	�`�F�[���R���{����
-------------------------------------------------------------------------------*/

/*!
	@brief �`�F�[���R���{����

	�w���t���O���܂������Ă��Ȃ����΁A���̃t���O���ǉ�����TRUE���Ԃ��B
	�`�F�[���R���{�ŃL�����Z���ł��Ȃ��ꍇ��FALSE���Ԃ�

	@return TRUE:���̋Z���o���Ă��悵, FALSE:����
*/
BOOL CCharacterBase::ChainCombo(DWORD chainid)
{
	if(!chainComboEnabled)return FALSE;
	if(chainlist & chainid)return(FALSE);

	chainlist |= chainid;
	return(TRUE);
}

/*!
	@brief �`�F�[���R���{����
	@sa ChainCombo

	ChainCombo�Ƃ̈Ⴂ�́A�w���t���O�� ���łɏo�������X�g�ɉ����邩�ǂ����B
	���̊֐��ł͎w���`�F�[���R���{ID�����X�g�ɉ������A�`�F�b�N�݂̂��s���B
*/
BOOL CCharacterBase::ChainCheck(DWORD chainid)
{
	if(!chainComboEnabled)return FALSE;
	if(chainlist & chainid)return(FALSE);

	return(TRUE);
}


/*!
	@brief �Z���񏉊���

	�ʏ��A���̊֐����I�[�o�[���C�h���ċZ���������������܂��B
	CCharacterBase::InitWazInfo ���Ăяo���ƁA�������W�����v���̊��{�I��
	�s���̐ݒ����s���܂��B
*/
void CCharacterBase::InitWazInfo()
{
	waz.walkf = ACTID_WALKF;
	waz.walkb = ACTID_WALKB;
	waz.jampf = ACTID_JAMPF;
	waz.jampb = ACTID_JAMPB;

	waz.att_jamp[0] = ACTID_ATT_JC;//�W�����v�U��
	waz.att_jamp[1] = ACTID_ATT_JB;
	waz.att_jamp[2] = ACTID_ATT_JA;
}

/*!
	@brief �p���[�Q�[�W����
	@param dp ������(�}�C�i�X����)
*/
void CCharacterBase::AddPowerGauge(double dp)
{
	GOBJECT* pdat = GetGObject();

	pdat->gauge += dp;
	if(pdat->gauge<0)pdat->gauge = 0;
	else if(pdat->gauge > pdat->gaugemax)pdat->gauge = pdat->gaugemax;
}

/*-----------------------------------------------------------------------------
	�R�}���h�����֐�
-------------------------------------------------------------------------------*/

/*!
	@brief �R�}���h����(���_��)
	@param dt �R�}���h���t����
	@return TRUE:�R�}���h����
*/
BOOL CCharacterBase::com236(int dt)//�t�����R�}���h
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

/*!
	@brief �R�}���h����(���_�����_��)
	@param dt �R�}���h���t����
	@return TRUE:�R�}���h����
*/
BOOL CCharacterBase::com236236(int dt)//�t�����R�}���hx2
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

/*!
	@brief �R�}���h����(�����_)
	@param dt �R�}���h���t����
	@return TRUE:�R�}���h����
*/
BOOL CCharacterBase::com623(int dt)//�����[�����[���܂���
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

/*!
	@brief �R�}���h����(���^��)
	@param dt �R�}���h���t����
	@return TRUE:�R�}���h����
*/
BOOL CCharacterBase::com214(int dt)//�t�͂ǁ[
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

/*!
	@brief �R�}���h����(�����^)
	@param dt �R�}���h���t����
	@return TRUE:�R�}���h����
*/
BOOL CCharacterBase::com421(int dt)
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_BACK);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

/*!
	@brief �R�}���h����(���^���_��)
	@param dt �R�}���h���t����
	@return TRUE:�R�}���h����
*/
BOOL CCharacterBase::com426(int dt)//�悪
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_BACK);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

/*!
	@brief �R�}���h����(����)
	@param dt �R�}���h���t����
	@return TRUE:�R�}���h����
*/
BOOL CCharacterBase::com66(int dt)//�_�b�V���R�}���h
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD2|KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst+=1;
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD2|KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

BOOL CCharacterBase::com66i(int dt)//�o�b�N�_�b�V���R�}���h
{
	if(!(GetKey(0)&KEYSTA_FOWORD2))return FALSE;
	return com66(dt);
}

/*!
	@brief �R�}���h����(����)
	@param dt �R�}���h���t����
	@return TRUE:�R�}���h����
*/
BOOL CCharacterBase::com44(int dt)//�o�b�N�_�b�V���R�}���h
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_BACK2|KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst+=1;
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_BACK2|KEYSTA_BACK);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

BOOL CCharacterBase::com44i(int dt)//�o�b�N�_�b�V���R�}���h
{
	if(!(GetKey(0)&KEYSTA_BACK2))return FALSE;
	return com44(dt);
}


/*!
	@brief �R�}���h����(����)
	@param dt �R�}���h���t����
	@return TRUE:�R�}���h����
*/
BOOL CCharacterBase::com22(int dt)
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN2|KEYSTA_DOWN);
	if(ofst<0)return(FALSE);
	ofst+=1;
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN2|KEYSTA_DOWN);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

BOOL CCharacterBase::com22i(int dt)
{
	if(!(GetKey(0)&KEYSTA_DOWN2))return FALSE;
	return com22(dt);
}

/*!
	@brief �n�㓊������
	@param maai ���E�ԍ���
	@return TRUE:�I�b�P�[
*/
BOOL CCharacterBase::NageHantei(DWORD maai)//�����̔���
{
	DWORD hm;
	GOBJECT *pedat= (GOBJECT*)(*funco->getinfo)(GetGObject()->eid);
	if(pedat==NULL)return(FALSE);

	if(pedat->aid & ACTID_KUCYU)//���肪�󒆔���
		return(FALSE);

	hm = (*funco->getmaai_h)(GetGObject()->id,pedat->id);
	if(hm > maai)
		return(FALSE);

	return(TRUE);
}


/*!
	@brief �R�}���h����( ���_���_���^��)
	@param dt �R�}���h���t����
	@return TRUE:�R�}���h����
*/
BOOL CCharacterBase::com2363214(int dt)
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_BACK | KEYSTA_DOWN);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD | KEYSTA_DOWN);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

/*!
	@brief �R�}���h����(���_���^���^���_��)
	@param dt �R�}���h���t����
	@return TRUE:�R�}���h����
*/
BOOL CCharacterBase::com62426(int dt)
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

/*!
	@brief �R�}���h����(�����^���_��)
	@param dt �R�}���h���t����
	@return TRUE:�R�}���h����
*/
BOOL CCharacterBase::com6426(int dt)
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN | KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_BACK);
	if(ofst<0)return(FALSE);
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_FOWORD);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

/*!--------------------------------------------------------------------------------------
	@brief COM���x������
	@param level ��ƂȂ郌�x��(���Փx)
	@return �����ɐ��������ꍇ��TRUE
	@sa GetComLevel

	�����̃��x������Ƃ��A�A���Z���o�����邩�ǂ������߂��Ƃ��Ɏg���܂��B
----------------------------------------------------------------------------------------*/
BOOL CCharacterBase::ComLevelCk(UINT level)
{
	int crnt_level = GetComLevel();
	int v = GetRandNum(100);
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


/*-----------------------------------------------------------------------------
	���{�I�ȍs���֐�
	�g���܂킵�Ă�������
-------------------------------------------------------------------------------*/

/*!
	@brief �����s��(in)

	���サ�ĉ��ʓ��ɂ͂����Ă���(1)
	�o���|�[�Y�͂��Ȃ��B
*/
void CCharacterBase::act_koutai_in()
{
	GOBJECT* pdat = GetGObject();

	if(pdat->counter==0){
		pdat->objtype |= GOBJTYPE_DISPLAY;//���ʓ��ɂ���
	}

	GOBJECT *pedat;
	if(pdat->counter==0){
		if(pdat->tid==TEAM_PLAYER1)
			pedat = GetActiveCharacter(TEAM_PLAYER2);
		else
			pedat = GetActiveCharacter(TEAM_PLAYER1);
		if(pedat!=NULL){
			if(pedat->x>0){
				pdat->x = pedat->x -(320+400);
				pdat->muki=FALSE;
				pdat->vx=14;
			}
			else{
				pdat->x = pedat->x +(320+400);
				pdat->muki=TRUE;
				pdat->vx=14;
			}
		}
		else {
			pdat->x=0;
			pdat->vx=0;
		}
		pdat->y = -400;
		pdat->vy= 0;
	}
	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy++;

	if(pdat->y > 0){
		pdat->objtype &= ~GOBJFLG_TAIKI;//�ҋ@�t���O������
		AddProperty( GOBJFLG_TOBETARGET );//�^�[�Q�b�g�ɂȂ�
		pdat->y=0;
		pdat->aid = ACTID_NEUTRAL;
	}
}

/*!
	@brief �����s��(out)

	���ʊO�ɂ҂��[�����ĂłĂ��đҋ@����
*/
void CCharacterBase::act_koutai_out()
{
	GOBJECT* pdat = GetGObject();

	if(pdat->counter==0){
		pdat->objtype &= GOBJTYPE_NODISPLAY;//���ʊO�ւ������悤�ɂ��Ă���
		pdat->objtype |= GOBJFLG_TAIKI;//�ҋ@�t���O�𗧂Ă�
		RemoveProperty( GOBJFLG_TOBETARGET );//�^�[�Q�b�g�ɂ͂Ȃ��Ȃ�
	}

	if(pdat->counter < 2){
		pdat->vx = -12;
		pdat->vy = -16;
	}
	else {
		pdat->revx=TRUE;
		movex(pdat->vx);
		pdat->y += pdat->vy;
		pdat->vy++;
		if(pdat->x > 1500 || pdat->x <-1500)pdat->aid = ACTID_TAIKICYU;
	}
}

/*!
	@brief �����s��(out)
	���ʊO�ɔ����ł��đҋ@����
*/
void CCharacterBase::act_taiki()
{
	GOBJECT* pdat = GetGObject();

	if(pdat->counter==0){
		pdat->objtype &= GOBJTYPE_NODISPLAY;//���ʊO�ւ������悤�ɂ��Ă���
		AddProperty( GOBJFLG_TAIKI );//�ҋ@�t���O�𗧂Ă�
		RemoveProperty( GOBJFLG_TOBETARGET );//�^�[�Q�b�g�ɂ͂Ȃ��Ȃ�
	}

	if(pdat->counter < 2){
		pdat->vx = -12;
		pdat->vy = -20;
	}
	else {
		pdat->revx=TRUE;
		movex(pdat->vx);
		pdat->y += pdat->vy;
		pdat->vy++;
		if(pdat->x > 1500 || pdat->x <-1500)pdat->aid = ACTID_TAIKICYU;
	}
}


/*!
	@brief �����s��(in) �o���|�[�Y�����E�o���|�[�Y
*/
void CCharacterBase::act_koutai2()
{
	act_tojyo();

	if(GetGObject()->counter<150 || GetGObject()->aid==ACTID_NEUTRAL){
		GetGObject()->aid = ACTID_NEUTRAL;
		(*funcs->msg2system)(GetGObject()->id,MSGOBJ2SYS_KOUTAIEND,0);
	}
}

/*!
	@brief �����s��(in) �o���|�[�Y�����E����
*/
void CCharacterBase::act_koutai()
{
	GOBJECT* pdat = GetGObject();

	pdat->objtype |= GOBJTYPE_DISPLAY;//���ʓ��ɂ���

	GOBJECT *pedat;
	if(pdat->counter==0){
		if(pdat->tid==TEAM_PLAYER1)
			pedat = GetActiveCharacter(TEAM_PLAYER2);
		else
			pedat = GetActiveCharacter(TEAM_PLAYER1);
		if(pedat!=NULL){
			if(pedat->x>0){
				pdat->x = pedat->x -(320+400);
				pdat->muki=FALSE;
				pdat->vx=14;
			}
			else{
				pdat->x = pedat->x +(320+400);
				pdat->muki=TRUE;
				pdat->vx=14;
			}
		}
		else {
			pdat->x=0;
			pdat->vx=0;
		}
		pdat->y = -400;
		pdat->vy= 0;
	}
	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy++;

	if(pdat->y > 0){
		pdat->objtype &= ~GOBJFLG_TAIKI;//�ҋ@�t���O������
		AddProperty( GOBJFLG_TOBETARGET );//�^�[�Q�b�g�ɂȂ�
		pdat->y=0;
		pdat->aid = ACTID_KOUTAI2;//�o���|�[�Y
	}
}

/*!
	@brief �ҋ@���s��
*/
void CCharacterBase::act_taikicyu(){}

/*!
	@brief �x���U���Ăяo���s��
*/
void CCharacterBase::act_strikercomeon()
{
	ChangeAction(ACTID_NEUTRAL);
}

/*!
	@brief �^�C���I�[�o�[�����s��
*/
void CCharacterBase::act_timeoverlose()
{
	ChangeAction(ACTID_NEUTRAL);
}

/*!
	@brief ���ԏ������s��
*/
/*void CCharacterBase::act_win2_in()
{
	GOBJECT* pdat = GetGObject();

	pdat->objtype |= GOBJTYPE_DISPLAY;//���ʓ��ɂ���

	GOBJECT *padat;
	if(pdat->counter==0){
		padat = GetActiveCharacter(pdat->tid);
		if(padat!=NULL){
			if(pedat->x>0){
				pdat->muki=padat->muki;
				//if (G
				pdat->x = padat->x - zurex(320+400);
				pdat->x = pedat->x + zurex(320+400);
				pdat->vx=14;
			}
			else{
				pdat->muki=TRUE;
				pdat->vx=14;
			}
		}
		else {
			pdat->x=0;
			pdat->vx=0;
		}
		pdat->y = -400;
		pdat->vy= 0;
	}
	movex(pdat->vx);
	pdat->y += pdat->vy;
	pdat->vy++;

	if(pdat->y > 0){
		pdat->objtype &= ~GOBJFLG_TAIKI;//�ҋ@�t���O������
		AddProperty( GOBJFLG_TOBETARGET );//�^�[�Q�b�g�ɂȂ�
		pdat->y=0;
		pdat->aid = ACTID_KOUTAI2;//�o���|�[�Y
	}
}*/

/*!
	@brief �󒆃_���[�W���A�㗎��
*/
void CCharacterBase::act_rakka2(){act_rakka();}


/*-----------------------------------------------------------------------------
	�����䎌 �擾/�ݒ� ����
-------------------------------------------------------------------------------*/
/*!
	@brief �����䎌�̐ݒ�

	�ݒ��e�L�X�g�t�@�C�������[�h���A�G�̖��O�ƃt�F�C�X�ύX�l�𔻒f����
	�K�؂ȑ䎌���I�����ăV�X�e���ɒʒm���܂��B
	�ʏ폟���|�[�Y�s���ōs���܂��B

	@param filename �����䎌�ݒ��e�L�X�g�t�@�C����(NULL�̏ꍇ"serifu.txt")
*/
void CCharacterBase::LoadAndSetKatiSerif(char* filename)
{
	if(!filename)filename = WIN_SERIFU_TXT;

	char* filepath = new char[256];
	char* tmpname = new char[64];
	char* katiserifu = new char[256];
	char* buff = NULL;
	UINT  bufflen;
	std::vector<char*>	str_points;
	std::vector<char*>	str_points_r;
	do
	{
		//�t�@�C�������������Ƀ��[�h
		sprintf(filepath,"%s\\%s",g_chardir,filename);
		File2Mem(filepath,&buff,&bufflen);
		if(!buff)break;
		
		char* enemyname = GetEnemyName(pdat->tid);
		int   enemyface = GetEnemyFace(pdat->tid);

		//�ݒ������o���A�ݒ��J�n�ʒu���L������
		char* s = buff;
		char* s_point;
		int tmpface;
		while((UINT)(s-buff)<bufflen-1)
		{
			if(*s=='#')
			{
				s++;
				s_point = s;
				int scaret = sscanf( s,"%s %d",tmpname,&tmpface);
				if(scaret == 1)
				{
					tmpface = 0;
				}
				if(scaret>0)
				{
					//�����_���p
					if( strcmp(tmpname,"random")==0 ){
						str_points_r.push_back(s_point);
					}
					//�w���䎌
					else if( tmpface==enemyface && enemyname && strcmp(tmpname,enemyname)==0){
						str_points.push_back(s_point);
					}
				}
			}
			else
			{
				s++;
				if(*s & 0x80)s++;
			}
		}


		//�g�p�����䎌���I��
		if(str_points.size()>0)
		{
			s_point = str_points[ GetRandNum((int)str_points.size()) ];
		}
		else if(str_points_r.size()>0)
		{
			s_point = str_points_r[ GetRandNum((int)str_points_r.size()) ];
		}
		else break;

		//�䎌���ǂݍ���
		s = s_point;
		ZeroMemory(katiserifu,256);
		while(TRUE)
		{
			//���s�����܂Ői��
			if(*s=='\n')
			{
				s++;
				break;
			}
			if((*s==0x0d && *(s+1)==0x0a))
			{
				s+=2;
				break;
			}

			s++;

			//�������Ȃ������H
			if((UINT)(s-buff)>=bufflen-2)
			{
				s=NULL;
				break;
			}
		}
		if(!s)break;

		UINT strcount=0;
		while(TRUE)
		{
			//�R�s�[
			katiserifu[strcount] = *s;
			strcount++;
			if(strcount>254)break;

			s++;

			//����#���������Ă��܂������I��
			if(*s=='#')
			{
				break;
			}

			//�I�[�ɓ��B
			if((UINT)(s-buff)>=bufflen)
			{
				break;
			}
		}
		funcs->setkatiserif(pdat->tid,katiserifu);

	}while(0);

	DELETE_ARRAY(buff);
	DELETE_ARRAY(filepath);
	DELETE_ARRAY(tmpname);
	DELETE_ARRAY(katiserifu);
}

/*!
	@class CBulletBase
	@brief �򓹋����{�N���X
	@ingroup GObjectBase

	CGoluahObject�����ѓ��������Ɋg�������N���X�ł��B
	act_run() : �����ł����Ƃ��̃A�N�V�����@�A�@
	act_bomb() : �q�b�g���A���ł����܂ł̃A�N�V�����@�A�@
	act_run2() : �q�b�g�������ł�����(�U���͂�������)�����ł����A�N�V�����@�A�@
	���������邱�Ƃɂ����ȒP�ɔ��ѓ����N���X�������܂��B

	�U���͂͒ʏ��Aatkinfo�����o�ϐ����g�p�����܂��B

	�R���X�g���N�^��CCharacterBase���e�N���X�Ƃ��Ďw�肷���ƁA
	�`�掞�ɕW���̃r�b�g�}�b�v��GCD�A�T�E���h�������p�ł��܂��B
*/


/*!
	@brief �\�z(�񐄏�)
*/
CBulletBase::CBulletBase(GOBJECT *parent/* = NULL */) : CGoluahObject(FALSE)
{
	parent_obj = parent;
	parent_class = NULL;

	CBulletBaseCreate();
}

/*!
	@brief �\�z(����)
*/
CBulletBase::CBulletBase(CCharacterBase *parent) : CGoluahObject(FALSE)
{
	if(parent!=NULL){
		parent_class = parent;
		parent_obj = parent->GetGObject();
		g_chardir = parent->GetCharDir();
	}
	else {
		parent_class=NULL;
		parent_obj=NULL;
	}
	CBulletBaseCreate();
}


/*!
	@brief �R���X�g���N�^���ʏ���
*/
void CBulletBase::CBulletBaseCreate()
{
	GOBJECT* pdat = GetGObject();

	if(parent_obj!=NULL){
		pdat->tid = parent_obj->tid;//�`�[��ID
		pdat->pmsarr = parent_obj->pmsarr;//gcd�͐e�Ɠ������̂��g�����悤�ɂ��Ă���
		pdat->pcdat = parent_obj->pcdat;
		pdat->prdat = parent_obj->prdat;
		pdat->phdat = parent_obj->phdat;

		pdat->x = parent_obj->x;
		pdat->y = parent_obj->y;
		pdat->muki = parent_obj->muki;

	}

	pdat->aid = CBB_STATE_IDLE;
	pdat->objtype	= GOBJTYPE_BULLET;//�I�u�W�F�N�g�^�C�v
	base_z=ZZAHYO_BULLET1;

	ZeroMemory(&atkinfo,sizeof(ATTACKINFO));
	pdat->atk = &atkinfo;

	hitmsg = 0;
	grdmsg = 0;
}


/*-----------------------------------------------------------------------------
	���b�Z�[�W�����֐�
-------------------------------------------------------------------------------*/
DWORD CBulletBase::Message(DWORD msg,LPVOID pd,DWORD prm)
{
	switch(msg){
	case GOBJMSG_SOUSAI:return( Sousai(prm) );
	default:
		//���Ƃ̃��b�Z�[�W�̓I�u�W�F�N�g���{�N���X�ɔC����
		return(CGoluahObject::Message(msg,pd,prm));
	}
	return(0);
}

/*!
	@brief GOBJMSG_DRAW ���b�Z�[�W����
	idle���͕`�悵�Ȃ��Bidle�ȊO�̓f�t�H���g�̕`�揈���B
*/
DWORD CBulletBase::Draw()
{
	if(GetGObject()->aid==CBB_STATE_IDLE)return TRUE;
	return 0;
}

/*!
	@brief GOBJMSG_TOUCHB ���b�Z�[�W�����i���I�u�W�F�N�g�ւ̏Փˁj
*/
DWORD CBulletBase::TouchB(ATTACKINFO *info,BOOL hit)
{
	Hit();
	if(parent_class){
		if(hit && hitmsg!=0){
			parent_class->Message(hitmsg,parent_obj,hitprm);
		}
		else if(grdmsg!=0){
			parent_class->Message(grdmsg,parent_obj,grdprm);
		}
	}
	return(TRUE);
}

void CBulletBase::Hit()
{
	pdat->kougeki = FALSE;
	Bomb();
}

/*!
	@brief GOBJMSG_SOUSAI ���b�Z�[�W�����i���򓹋��ւ̏Փˁj
*/
DWORD CBulletBase::Sousai(DWORD prm)
{
	GetGObject()->kougeki=FALSE;//�U���͖�����
	GetGObject()->aid = CBB_STATE_BOMB;
	return(TRUE);
}


/*!
	@brief GOBJMSG_CNGROUND ���b�Z�[�W����

	���E���h�ω��BFALSE���Ԃ��Ə����ꂿ�Ⴄ�B
	CBulletBase��TRUE���Ԃ��AHide()���܂��B
*/
BOOL CBulletBase::OnChangeRound()
{
	Hide();
	return TRUE;//�ʏ��A�����Ȃ�
}

/*-----------------------------------------------------------------------------
	���ԑ���
-------------------------------------------------------------------------------*/

//!�I�u�W�F�N�g�j��
void CBulletBase::Suicide()
{
	Hide();
}

//!���ŃA�j���[�V�����ֈڍs
void CBulletBase::Bomb()
{
	ChangeAction(CBB_STATE_BOMB);
}

//!�B��
void CBulletBase::Hide()
{
	ChangeAction(CBB_STATE_IDLE);
}

//!���΂�
BOOL CBulletBase::Go(BOOL muki,double x,double y,double vx,double vy)
{
	GOBJECT* pdat = GetGObject();

	if (pdat->aid == CBB_STATE_IDLE)
	{
		pdat->muki = muki;
		SetPos(x,y);
		pdat->vx = vx;
		pdat->vy = vy;
		ChangeAction(CBB_STATE_RUN);
		return TRUE;
	}

	return FALSE;
}

/*!
	@brief �T�E���h�Đ�
	�����e�N���X�������΁A�������̂ق��ɏ������ۓ���
*/
void CBulletBase::PlayMySound(DWORD number)
{
	if(parent_class!=NULL)parent_class->PlayMySound(number);
}

/*!
	@brief GOBJMSG_ACTION ���b�Z�[�W����
*/
DWORD CBulletBase::Action()
{
	switch(GetGObject()->aid){
	case CBB_STATE_IDLE:	act_idle();break;
	case CBB_STATE_RUN:		act_run();break;
	case CBB_STATE_BOMB:	act_bomb();break;
	case CBB_STATE_RUN2:	act_run2();break;
	}
	return(TRUE);
}

//!�A�C�h��������
void CBulletBase::act_idle()
{
	GOBJECT* pdat = GetGObject();

	pdat->x = 99999;
	pdat->y = 99999;
	pdat->kougeki = FALSE;
}



/*!
	@class CClassicalBullet
	@brief �����򓹋��N���X
	@ingroup GObjectBase

	���򓹋��\���� BULLETINFO_A �����̂܂܎g�p�ł��܂��B���Ԃ��B
	���{�I�ɂ͋��\�[�X�����̈ڐA�p�̃N���X�ŁACBulletBase��
	�p���������ѓ����N���X���V�K�ɍ������ق������Ԃ��������Ȃ��Ǝv���܂��B

	�g�p����BULLETINFO_A�\���̂́A�����o�łȂ����[�J���ϐ��ɂ���������OK�ł��B
	�\���̂��̂��̂��������Ȃ��Ȃ����̂ŁA���̕ӂ̍H�v���K�v�����B
*/

/*!
	@brief �\�z
*/
CClassicalBullet::CClassicalBullet( CCharacterBase *parent,CDI_CHARACTERINFO2 *info,BULLETINFO_A *bulinfo,
								   BYTE userID,BYTE userNo) : CBulletBase(parent)
{
	array_len_r = 0;
	array_len_d = 0;

	//�z���̒������擾����
	while(bulinfo->cell_run[array_len_r]>0)array_len_r++;
	while(bulinfo->cell_dis[array_len_d]>0)array_len_d++;

	//�������m��&�z�����R�s�[����
	DWORD i;
	if(array_len_r==0)pcr=NULL;
	else{
		pcr = new int[array_len_r];
		for(i=0;i<array_len_r;i++){
			pcr[i] = bulinfo->cell_run[i];
		}
	}
	if(array_len_d==0)pcd=NULL;
	else{
		pcd = new int[array_len_d];
		for(i=0;i<array_len_d;i++){
			pcd[i] = bulinfo->cell_dis[i];
		}
	}

	//���U�������z�񂩂��V�U�������z���ɕϊ�����
	atkinfo.damage = bulinfo->atk.damage;
	atkinfo.kezuri = bulinfo->atk.kezuri;
	atkinfo.guard = bulinfo->atk.guard;
	atkinfo.hit = bulinfo->atk.hit;
	atkinfo.muki = FALSE;//���\���̂Ƃ͂������ƈӖ����Ⴄ

	//�p�����[�^�R�s�[
	ax = bulinfo->ax;
	ay = bulinfo->ay;
	vx = bulinfo->vx;
	vy = bulinfo->vy;
	spd_r = bulinfo->spd_run;
	spd_d = bulinfo->spd_dis;
	lifedur = bulinfo->dur;
	flags = bulinfo->type;

	//�t���O���� 
	if( !(flags&BULLETA_VSHUMAN) )RemoveProperty(GOBJFLG_ATTACK);
	if( !(flags&BULLETA_VSBULLET) )RemoveProperty(GOBJFLG_ZBULLET);
	if( flags&BULLETA_DRAWBACK )base_z=ZZAHYO_BULLET3;
	if( flags&BULLETA_DRAWMIDDLE )base_z=ZZAHYO_BULLET2;
	if( flags&BULLETA_DISPZAHYO )AddProperty(GOBJFLG_DISPZAHYO);
	if( flags&BULLETA_DONOTSTOP )AddProperty(GOBJFLG_DONOTSTOP);
}

/*!
	@brief �j��
*/
CClassicalBullet::~CClassicalBullet()
{
	if(pcr)delete [] pcr;
	if(pcd)delete [] pcd;
}


/*!
	@brief ���̑�
*/
BOOL CClassicalBullet::Go(BOOL muki,double x,double y)
{	
	return CBulletBase::Go(muki,x,y,vx,vy);
}

//!�q�b�g������
void CClassicalBullet::Hit()
{
	if(flags&BULLETA_DONOTDIE)ChangeAction(CBB_STATE_RUN2);
	else ChangeAction(CBB_STATE_BOMB);
}

//!���Ď�����
void CClassicalBullet::act_run()
{
	if(spd_r==0 || array_len_r==0 || pcr==NULL)return;
	GOBJECT* pdat = GetGObject();
	DWORD counter2 = pdat->counter%(spd_r * array_len_r);
	DWORD index = counter2 /= spd_r;

	pdat->cnow = pcr[index];

	pdat->vx += ax;
	pdat->vy += ay;
	movex( pdat->vx );
	pdat->y += pdat->vy;

	pdat->kougeki=TRUE;
	if(pdat->counter > lifedur){
		ChangeAction(CBB_STATE_BOMB);
	}
	else if(flags&BULLETA_XJIMENN){
		if(pdat->y>0){
			pdat->y = 0;
			ChangeAction(CBB_STATE_BOMB);
		}
	}
}

//!���Ď�����(�q�b�g��)
void CClassicalBullet::act_run2()
{
	act_run();
	GetGObject()->kougeki=FALSE;
}

//!�q�b�g�㏈��
void CClassicalBullet::act_bomb()
{
	GetGObject()->kougeki = FALSE;

	DWORD index = 0;

	if(!(spd_d==0 || pcd==NULL)){
		index = GetGObject()->counter / spd_d;
	}
	if(index >= array_len_d || array_len_d==0 || (spd_d==0 || pcd==NULL)){
		Hide();
		return;
	}
	GetGObject()->cnow = pcd[index];
}

/*!
	@class CBulletList
	@brief ���ѓ���X�g�N���X
	@ingroup GObjectBase

�@���ѓ����I�u�W�F�N�g�����X�g�Ǘ����邱�Ƃ��ł��܂��B
�@���x�ɑ��ʔ��˂������ѓ������A�A�˂������Ƃ��Ɏg�p���Ă��������B
�@�ӂ��̔��ѓ����ł��A�����ɏ��Q���o���ꍇ�Ɏg�p�����ƕ֗��ł��B
�@CGoluahObject�̔h���N���X�ł͂Ȃ��̂ŁA���I���������K�v�͂����܂����B
�@�i�r���Ń��X�g���N���A���邱�Ƃ͂ł��܂����̂ł����ӂ��������B�j

*/

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
/*!
	@brief �\�z

	�ʏ��͂��������g���Ă��������B
*/
CBulletList::CBulletList()
{
	m_pBullet = NULL;
	pNext = NULL;
	ListCount = 0;
	x = 0;
	y = 0;
	vx = 0;
	vy = 0;
}

/*!
	@brief ���X�g�ɔ��ѓ������P�����č\�z
	@param pBullet ���X�g�ɒǉ�����CBulletBase�ւ̃|�C���^

	�g���@���͂����܂薳���Ǝv���B
*/
CBulletList::CBulletList(CBulletBase *pBullet)
{
	if (pBullet){
		m_pBullet = pBullet;
		ListCount = 1;
	}
	else {
		m_pBullet = NULL;
		ListCount = 0;
	}

	pNext = NULL;
	x = 0;
	y = 0;
	vx = 0;
	vy = 0;
}

/*!
	@brief ���ڂ���

	���X�g���̔��ѓ����Ɏ��͉����܂����B
*/
CBulletList::~CBulletList()
{
	if (pNext)
		delete pNext;
}

/*!
	@brief ���X�g�ɔ��ѓ������ǉ�
	@param pBullet �ǉ�����CBulletBase�ւ̃|�C���^

	���X�g��pBullet���������ѓ������P�ǉ����܂��B
	CCharacterBase::InitBullets���őS���܂Ƃ߂Ă������Ⴄ���Ƃ������߂��܂��B
	�Ȃ����x�ǉ������ƌ��ɖ߂��܂����A�����ӂ��������B
*/
void CBulletList::Add(CBulletBase *pBullet)
{
	if (pBullet)
	{
		if (m_pBullet == NULL) {
			m_pBullet = pBullet;
			if (m_pBullet)
				ListCount++;
		}
		else if (pNext == NULL) {
			pNext = new CBulletList(pBullet);
			if (pNext)
				ListCount++;
		}
		else {
			pNext->Add(pBullet);
			ListCount = pNext->GetCount() + 1;
		}
	}
}

/*!
	@brief ���X�g���̔��ѓ������P����(1)
	@sa CBulletBase::Go
	@return ���˂ɐ��������ꍇ��TRUE�A���s�����ꍇ��FALSE

	���X�g�����甭�ˉ\�Ȕ��ѓ������T���A���˂��܂��B
	���������ˉ\�ȏꍇ�́A���ɓo�^���ꂽ���̂��D�悳���܂��B
*/
BOOL CBulletList::Go(BOOL muki, double x, double y, double vx, double vy)
{
	if (m_pBullet == NULL)
		return FALSE;

	if (m_pBullet->Go(muki, x, y, vx, vy))
		return TRUE;
	else if (pNext)
		return pNext->Go(muki, x, y, vx, vy);
	else
		return FALSE;
}

/*!
	@brief ���X�g���̔��ѓ������P����(2)
	@sa CBulletBase::Go
	@return ���˂ɐ��������ꍇ��TRUE�A���s�����ꍇ��FALSE

	�ڂ�����(1)���Q�ƁB
	�ȗ����ꂽvx��vy�ɂ́ASetSpeed�Őݒ肳�ꂽ���x���g�p�����܂��B
*/
BOOL CBulletList::Go(BOOL muki, double x, double y)
{
	if (m_pBullet == NULL)
		return FALSE;

	if (m_pBullet->Go(muki, x, y, vx, vy))
		return TRUE;
	else if (pNext)
		return pNext->Go(muki, x, y);
	else
		return FALSE;
}

/*!
	@brief ���X�g���̔��ѓ������P����(3)
	@sa CBulletBase::Go
	@return ���˂ɐ��������ꍇ��TRUE�A���s�����ꍇ��FALSE

	�ڂ�����(1)���Q�ƁB
	�ȗ����ꂽx��y�ɂ́ASetPos�Őݒ肳�ꂽ���W���A
	�ȗ����ꂽvx��vy�ɂ́ASetSpeed�Őݒ肳�ꂽ���x�����ꂼ���g�p�����܂��B
*/
BOOL CBulletList::Go(BOOL muki)
{
	if (m_pBullet == NULL)
		return FALSE;

	if (m_pBullet->Go(muki, x, y, vx, vy))
		return TRUE;
	else if (pNext)
		return pNext->Go(muki);
	else
		return FALSE;
}

/*!
	@brief ���X�g���̔��ѓ������S�Ĕ���(1)
	@sa CBulletBase::Go

	���X�g�Ɋ܂܂��Ă����A�S�Ă̔��ѓ����𔭎˂��܂��B
	�ȗ����ꂽvx��vy�ɂ́A�e�X��SetSpeed�Őݒ肳�ꂽ���x���g�p�����܂��B
*/
void CBulletList::Go_All(BOOL muki, double x, double y)
{
	if (m_pBullet)
		m_pBullet->Go(muki, x, y, vx, vy);

	if (pNext)
		pNext->Go_All(muki, x, y);
}

/*!
	@brief ���X�g���̔��ѓ������S�Ĕ���(2)
	@sa CBulletBase::Go

	�ڂ�����(1)���Q�ƁB
	�ȗ����ꂽx��y�ɂ́A�e�X��SetPos�Őݒ肳�ꂽ���W���A
	�ȗ����ꂽvx��vy�ɂ́A�e�X��SetSpeed�Őݒ肳�ꂽ���x�����ꂼ���g�p�����܂��B
*/
void CBulletList::Go_All(BOOL muki)
{
	if (m_pBullet)
		m_pBullet->Go(muki, x, y, vx, vy);

	if (pNext)
		pNext->Go_All(muki);
}

/*
	@brief ���X�g��num�Ԗڂ̗v�f������
	@param num �v�f�̔ԍ�
	@return num�Ԗڂ̗v�f������CBulletList�ւ̃|�C���^�A�����ꍇ��NULL
	@sa GetNext

	�z���Ɠ����悤�Ȋ��o�ilist[3]�̗l�Ȍ`�j�ŁA�����̗v�f���������邱�Ƃ��o���܂��B

	���������s���x���x�����߁A�g�p�͔񐄏��ł��B���X�g���̑����͏o���邾��GetNext���g�p���Ă��������B
*/
/*CBulletList* CBulletList::operator [](int num)
{
	if (num == 0)
		return this;
	else if (num > 0)
	{
		if (pNext)
			return pNext->operator [](num - 1);
		else
			return NULL;
	}

	return NULL;
}
*/
/****************************************************************************

  CEffectBase�N���X

*****************************************************************************/

CEffectBase::CEffectBase() : CGoluahObject(TRUE)
{
	ceffectbase_private_duration = 20;
}

CEffectBase::~CEffectBase()
{
}

DWORD CEffectBase::Action()
{
	float time = pdat->counter / ceffectbase_private_duration;
	if(time>=1.0f)
	{
		Suicide();
	}
	else
	{
		Update( time );
	}
	return TRUE;
}

void CEffectBase::SetDuration(UINT dur)
{
	if(dur==0){dur=1;}
	ceffectbase_private_duration = (float)dur;
}


/* **************************************************************************

  CStageBase�N���X

 ************************************************************************** */

void CStageBase::SetupStageDLL(SDI_STAGEINFO2 *info)
{
	funco = info->funco;
	funcs = info->funcs;
	funcd = info->funcd;
}

BOOL CStageBase::CheckSTB(DWORD Version/* = STB_VERSION */)
{
	if(Version > STB_VERSION || Version < STB_LEASTVER){ 
#		ifdef _DEBUG
			char *dbgmsg = new char[256];
			sprintf(dbgmsg,"CCharacterInfo:STB�o�[�W�����Ⴂ(%d!=%d)\n",Version,STB_VERSION);
			OutputDebugString(dbgmsg);
			delete [] dbgmsg;
#		endif
		return FALSE;
	}

	return TRUE;
}

CStageBase::CStageBase(SDI_STAGEINFO2 *info)
				: CGoluahObject(TRUE)//�����蔻���͍s���Ȃ��B
{
	g_chardir = info->dir;
	pdat->tid = info->tid;
}


CStageBase::~CStageBase()
{
}
/* **************************************************************************

  �c���G�t�F�N�g

 ************************************************************************** */

CShadowEffect::CShadowEffect(CCharacterBase* pParent,
				  DWORD color /* = 0x00888888 */,
				  DWORD BlendType /* = GBLEND_HANTOUMEI */,
				  UINT Duration /* = 20 */)
{
	GOBJECT* ppdat;

	if (!pdat)
		return;

	if (!pParent)
	{
		Suicide();
		return;
	}

	ppdat = pParent->GetGObject();

	pdat->tid = ppdat->tid;
	pdat->objtype = GOBJFLG_DONOTSTOP;

	pdat->pcdat = ppdat->pcdat;
	pdat->prdat = ppdat->prdat;
	pdat->phdat = ppdat->phdat;
	pdat->pmsarr = ppdat->pmsarr;

	pdat->counter = 0;
	pdat->cnow = ppdat->cnow;
	pdat->x = ppdat->x;
	pdat->y = ppdat->y;
	pdat->z = ZZAHYO_EFFECT1;
	pdat->rot = ppdat->rot;
	pdat->revx = ppdat->revx;
	pdat->revy = ppdat->revy;
	pdat->muki = ppdat->muki;
	pdat->magx = ppdat->magx;
	pdat->magy = ppdat->magy;

	// �����o������
	m_color = color;
	m_BlendType = BlendType;
	SetDuration(Duration);
}

CShadowEffect::~CShadowEffect(void)
{
}

void CShadowEffect::Update(float time)
{
	// �R�����Y�̃\�[�X���肨�؂肵�܂����A21�X����335�����T���N�X�R�B
	pdat->color = (m_color & 0x00FFFFFF) | ( (DWORD)(255.0 - time * 20.0 * 25.0) << 24 );
}

DWORD CShadowEffect::Draw(void)
{
	SetBlend(m_BlendType);
	CellDraw(pdat);
	SetBlend(GBLEND_HANTOUMEI);

	return TRUE;
}


/* **************************************************************************

  �O���[�o���֐��@�Ƃ�

 ************************************************************************** */

/*!
	@brief DLL�̃G���g���[�|�C���g�H

	�����_������
*/
BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
#		ifdef _DEBUG
			num_allocs = 0;
#		endif
		srand(GetTickCount());
		break;

	case DLL_PROCESS_DETACH:
#		ifdef _DEBUG
			if (num_allocs > 0)
			{
				DebugPrintf("DLL:���܂����A���������[�N���������Ă܂��B\n�����F%d�o�C�g", (int)num_allocs);
			}
#		endif
		break;
	}

	return TRUE;
}

//!�����_���i���o�[����
inline int GetRandNum(int num)
{
	return(rand()%num);
}

/*!
	@brief �t�@�C���ǂݍ���

	�t�@�C�����ǂݍ����Ń��������ɍڂ��܂��B
	buff �ɂ� new[] �Ő��������̈��̃|�C���^���A���̂ŁA
	�֐������������ꍇ�͌Ăяo������d elete[] ���܂��B

	@param filepath	[in] �ǂݍ��ރt�@�C����
	@param buff		[out] �ǂݍ��݌��̃o�b�t�@
	@param len		[out] �ǂݍ��񂾃o�C�g��
	@return	TRUE:���� , FALSE:�c�O
*/
BOOL File2Mem(char* filepath,char** buff,UINT *len)
{
	*buff = NULL;
	*len = 0;

	//�t�@�C���̃I�[�v��
	HANDLE hFile = CreateFile(filepath,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE)return(FALSE);

	//�̈��m��
	*len = GetFileSize(hFile,NULL);
	*buff = new char [(*len)+1];
	ZeroMemory(*buff,sizeof(char)*((*len)+1));

	//�t�@�C���̓ǂݍ���
	DWORD br;
	ReadFile(hFile,*buff,*len,&br,NULL);
	CloseHandle(hFile);

	return TRUE;
}

/*!
	@brief �f�o�b�O�o��

	�t�H�[�}�b�g��(printf�Ƃ��Ƃ�������)��
	OutputDebugString���s���܂��B
*/
void DebugPrintf(char* fmt,...)
{
	va_list args;
	va_start(args, fmt);

	char buffer[512];
	vsprintf(buffer, fmt, args);

	OutputDebugString(buffer);
	OutputDebugString("\n");

	va_end(args);
}


void DebugPrintfDummy(char* fmt,...){}

#endif//RUNTIME_BUILD_OFF