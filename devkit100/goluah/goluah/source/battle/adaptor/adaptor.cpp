/*
2011/10/29	Marvel�ŋ󒆂ł̌���s��
*/
#include "StdAfx.h"

#define LPDIRECT3DTEXTURE9 LPVOID

#include "define_gcd.h"
#include "gobject.h"
#include "define_data.h"
#include "define_char.h"
#include "mimikaki.h"
#include "adaptor.h"
// #include "CharacterList.h"
// #include "cell.h"

DWORD extwin = 250;		// �Ή��ł���H
extern GOBJECT* pchar[2];
extern DWORD pnagerare[2];

DWORD gMessage(DWORD msg,LPVOID dat,DWORD);

extern TCHAR *g_chardir;
extern DI_FUNCTIONS_S *funcs;
extern DI_FUNCTIONS_O *funco;
extern DI_FUNCTIONS_D *funcd;
extern FUNCTIONPACK oldfunc;
extern HMODULE hAction2;

WAZAINFO waz;//�Z���
ATTACKINFO atkmuki;

DWORD gMessage(DWORD msg,LPVOID dat,DWORD);
void GetEnemyDat(DWORD id, LPVOID pdat);

//**************************************************************************************
//  Create Character
//**************************************************************************************

CCharAdaptor::CCharAdaptor()
{
	hAction2 = NULL;
	m_pchar[0] = m_pchar[1] = NULL;
	m_chardir=NULL;
}

void CCharAdaptor::InitAdaptor(CDI_CHARACTERINFO2 *info)
{
	BOOL (*pCreate)(DWORD,PVOID);

	oid = (*funco->objcreate)();//�I�u�W�F�N�g�𐶐�
	pdat = (GOBJECT*)(*funco->getinfo)(oid);//�f�[�^�ւ̃|�C���^��get
	keynumber = info->keyinput;//���Ԃ���L�[���͂��󂯎�邩

	//�f�[�^��������
	pdat->tid			= info->tid;//�`�[��ID
	pdat->msghandler	= gMessage;//���b�Z�[�W�����֐�
	pdat->objtype		= GOBJTYPE_CHARACTER;//�I�u�W�F�N�g�^�C�v
	pdat->hpmax			= 1000;//�ő�̗͒l
//	pdat->gaugemax		= 1;//�ő�Q�[�W
//	pdat->gaugetype		= GOBJFLGA_POWGAUGE2;//�Q�[�W�^�C�v
	pdat->winfo			= NULL;
	pg_hit = 0.0;
	pg_guard = 0.0;
	g_blown = 0.0;
	g_guard = 0.0;
	pfNagerare = NULL;

	pCreate = (BOOL (*)(DWORD,PVOID))
		GetProcAddress(hAction2, "CreateCharacter");
	if(pCreate == NULL){ // ���s
		funco->suicide(pdat->id);
		oid = 0; return;
	}

	ZeroMemory(&olddat, sizeof(olddat));
	olddat.cid = pdat->tid;
	if (m_pchar[olddat.cid]) {
		olddat.cid = !olddat.cid;
		if (m_pchar[olddat.cid]) {
			funco->suicide(pdat->id);
			oid = 0; return;
		}
	}
	olddat.hlib = hAction2;
	Exchange(pdat, &olddat, FALSE);
	olddat.fpack = &oldfunc;
	pdat->pobjdat_u = this;
	m_pchar[olddat.cid] = pdat;
	memcpy(pchar, m_pchar, sizeof(pchar));
	if (!pCreate(olddat.cid, &olddat)){ // ���s
		funco->suicide(pdat->id);
		oid = 0; return;
	}
	Exchange(pdat, &olddat, TRUE);

	InitializeGCDandBMP(pdat,info->color);//�r�b�g�}�b�v��cell.gcd��ǂݍ���
	InitializeMySound();//�T�E���h�̓ǂݍ���
	InitAttackInfo();//�U���͏��̏�����

	Exchange(pdat, &olddat, FALSE);
	if (olddat.initchar != NULL)
		olddat.initchar();
	Exchange(pdat, &olddat, TRUE);;
}


//**************************************************************************************
//   �V�X�e������̃��b�Z�[�W
//**************************************************************************************

DWORD CCharAdaptor::Message(DWORD msg,LPVOID dat,DWORD prm)
{
	CHARACTERINFO2 objdat;
	GOBJECT *pobjdat;
	DWORD stock;
	int hp;
	BULLETATKINFO* pbulainfo;

	// �L������ID���O���[�o���ϐ��ɑ���
	if (msg != GOBJMSG_DELETE)
	{
		memcpy(pchar, m_pchar, sizeof(pchar));
		memcpy(pnagerare, m_pnagerare, sizeof(pnagerare));
	}

	switch(msg){
	case GOBJMSG_ACTION:
		return(Action());
	case GOBJMSG_COMMAND:
		Command();
		return(TRUE);
	case GOBJMSG_COMMANDCOM:
		return(CommandCOM(prm));
	case GOBJMSG_DRAW://�`��̓V�X�e���ɔC����
		return(FALSE);
	case GOBJMSG_TOUCHA:
		return(TouchA(pdat->atk2.info1));
	case GOBJMSG_TOUCHB:
		return(TouchB(pdat->atk,prm));
	case GOBJMSG_DOTOJYO:
		pdat->aid = ACTID_TOJYO;
		funco->actidchanged(pdat->id);
		return(TRUE);
	case GOBJMSG_DOYOUWIN://����
		pdat->aid = ACTID_SYORI;
		funco->actidchanged(pdat->id);
		return(TRUE);
	case GOBJMSG_TAIKI://�ҋ@���Ƃ����b�Z�[�W
		pdat->aid = ACTID_TAIKI;
		pdat->objtype &= GOBJTYPE_NODISPLAY;//��ʊO�ւ�����悤�ɂ��Ă���
		pdat->objtype |= GOBJFLG_TAIKI;//�ҋ@�t���O�𗧂Ă�
		pdat->objtype &= ~GOBJFLG_TOBETARGET;//�^�[�Q�b�g�ɂ͂Ȃ�Ȃ�
		(*funco->actidchanged)(pdat->id);
		break;
	case GOBJMSG_KOUTAI://��サ�냁�b�Z�[�W
		if(pdat->aid == ACTID_TAIKICYU){
			pdat->aid = ACTID_KOUTAIIN;
			pdat->objtype |= GOBJFLG_TOBETARGET;//�^�[�Q�b�g�Ȃ�
			(*funco->actidchanged)(pdat->id);
			return(YEAH);
		}
		return(0);
		break;
	case GOBJMSG_STRIKER://�x���U���v��
		return(FALSE);//�x���U���͍s���܂���
		break;
	case GOBJMSG_KOUTAI2://��サ�냁�b�Z�[�W2�i�̂�т�Ɠo��|�[�Y�����߂Ăėǂ��j
		pdat->aid = ACTID_KOUTAI;
		pdat->objtype |= GOBJTYPE_DISPLAY;//��ʓ��ɂ���
		pdat->objtype |= GOBJFLG_TOBETARGET;//�^�[�Q�b�g�ɂȂ�
		funco->actidchanged(pdat->id);
		break;
	case GOBJMSG_CLIPX:
		if(pdat->aid == ACTID_KOUTAI)return(TRUE);
		else return(0);
	case GOBJMSG_CNGROUND:
		return(TRUE);//���E���h���ς���Ă������Ȃ�
	case GOBJMSG_CNGAID:
		ActionIDChanged();
		break;
	case MYGOBJMSG_NAGE:
		stock = 0;
		pobjdat = (GOBJECT*)funco->getinfo(prm);
		GetEnemyDat(olddat.cid, &objdat); // �����ɖ|��
		m_pnagerare[olddat.cid] = prm;
		objdat.cid = 0x00010000 | olddat.cid;
		Exchange(pobjdat, &objdat, FALSE);
		Exchange(pdat, &olddat, FALSE);
		if (pfNagerare != NULL)
			stock = pfNagerare(&objdat);
		hp = pobjdat->hp;
		Exchange(pdat, &olddat, TRUE);
		Exchange(pobjdat, &objdat, TRUE);
		pobjdat->hp = ((objdat.hp > hp) ? hp : objdat.hp);
		switch (pobjdat->cnow) {
		case DCELL_NAGERARE2:
			pobjdat->cnow	= DCELL_NAGERARE1;
			pobjdat->rot	= 45;
			break;
		case DCELL_NAGERARE3:
			pobjdat->cnow	= DCELL_NAGERARE1;
			pobjdat->rot	= 90;
			break;
		case DCELL_NAGERARE4:
			pobjdat->cnow	= DCELL_NAGERARE1;
			pobjdat->rot	= 135;
			break;
		case DCELL_NAGERARE5:
			pobjdat->cnow	= DCELL_NAGERARE1;
			pobjdat->rot	= 0;
			break;
		default:
			pobjdat->rot	= 0;
			break;
		}
		if (stock != 0 && (pobjdat->muki == pdat->muki))
		{
			pdat->atk = &atkmuki;
			funco->adddamage(pdat->id, pobjdat->id, 0, 0);
		}
		return stock;
	case MYGOBJMSG_BULLETHIT:
		pbulainfo = (BULLETATKINFO*)prm;// ����
		return(BulletHit(&((STRUCT_BULLETA*)pbulainfo->pbul->pobjdat_u)->
			pbinfo_a->atk, pbulainfo->hit));
	default:
		return(0);//���Ƃ́A�m���
	}
	return(TRUE);
}


CCharAdaptor::~CCharAdaptor()
{
	Exchange(pdat, &olddat, FALSE);
	if (olddat.deletechar != NULL)
		olddat.deletechar();
	UnloadMyBitmaps(pdat);
	UnloadMySound();
	m_pchar[olddat.cid] = NULL;

    if (hAction2)
	{
		FreeLibrary(hAction2);
		hAction2 = NULL;
    }
}

void CCharAdaptor::Command()
{
	DWORD key_now = (*funcs->getkey)(keynumber,0);
	BOOL callstriker=FALSE;

	if(pdat->aid & ACTID_SYSTEM){//�V�X�e����`���쒆�Ȃ̂ŁA��{�I�ɂ͂�����Ȃ�
		Exchange(pdat, &olddat, FALSE);
		if (olddat.command_a != NULL)
			olddat.command_a();
		Exchange(pdat, &olddat, TRUE);
		return;
	}
//	if(pdat->aid == ACTID_TOJYO || pdat->aid==ACTID_SYORI)return;
//	if(pdat->aid & ACTID_NAGE)return;
	if(pdat->aid & ACTID_INOUT)return;

	//�X�g���C�J�[���Ă�
	if( (key_now&KEYSTA_FOWORD) && (key_now&KEYSTA_DOWN) && (key_now&KEYSTA_BD2) ){
		if((*funcs->msg2system)(pdat->id,MSGOBJ2SYS_STRIKER1,0)){
			ADDEFFECT(EFCTID_COMEON,(int)pdat->x,(int)pdat->y-100,0);
			callstriker=TRUE;
		}
	}
	else if( (key_now&KEYSTA_BACK) && (key_now&KEYSTA_DOWN) && (key_now&KEYSTA_BD2) ){
		if((*funcs->msg2system)(pdat->id,MSGOBJ2SYS_STRIKER2,0)){
			ADDEFFECT(EFCTID_COMEON,(int)pdat->x,(int)pdat->y-100,0);
			callstriker=TRUE;
		}
	}


/*	if(pdat->aid & ACTID_HISSATU){//�K�E�Z���i��{�I�ɂ�����ׂ��炸�j
		return;
	}

	//�K�E�Z�R�}���h����
	if(pdat->aid & ACTID_KUCYU){//�󒆕K�E�Z�̃R�}���h����
	}
	else{//�n��K�E�Z�̃R�}���h����
	}

	if(pdat->aid & ACTID_ATTACK){//�U�����쒆�i��{�I�ɂ�����Ȃ��B�`�F�[���R���{�o���Ȃ炱���ł�����j
		if(pdat->aid & ACTID_KUCYU){////�W�����v���쒆
			return;
		}
		else{
			if(key_now & 0x22220000){
				if(key_now & KEYSTA_DOWN){
					if(key_now & KEYSTA_BC2){if(ChainCombo(CHAIN_CC)){pdat->aid = ACTID_ATT_CC;return;}}
					else if(key_now & KEYSTA_BB2){if(ChainCombo(CHAIN_CB)){pdat->aid = ACTID_ATT_CB;return;}}
					else if(key_now & KEYSTA_BA2){if(ChainCombo(CHAIN_CA)){pdat->aid = ACTID_ATT_CA;return;}}
				}
				else{
					if(key_now & KEYSTA_BC2){if(ChainCombo(CHAIN_SC)){pdat->aid = ACTID_ATT_SC;return;}}
					else if(key_now & KEYSTA_BB2){if(ChainCombo(CHAIN_SB)){pdat->aid = ACTID_ATT_SB;return;}}
					else if(key_now & KEYSTA_BA2){if(ChainCombo(CHAIN_SA)){pdat->aid = ACTID_ATT_SA;return;}}
				}
			}
		}
		return;
	}

	if(pdat->aid & ACTID_KUCYU){//�W�����v���쒆
		if(key_now & 0x22220000){
			if(key_now & KEYSTA_BC2){pdat->aid = ACTID_ATT_JC;return;}
			else if(key_now & KEYSTA_BB2){pdat->aid = ACTID_ATT_JB;return;}
			else if(key_now & KEYSTA_BA2){pdat->aid = ACTID_ATT_JA;return;}
		}
		return;
	}

	//��{���쒆
	if(key_now & 0x22220000){
		if(key_now & KEYSTA_DOWN){
			if(key_now & KEYSTA_BC2){pdat->aid = ACTID_ATT_CC;return;}
			else if(key_now & KEYSTA_BB2){pdat->aid = ACTID_ATT_CB;return;}
			else if(key_now & KEYSTA_BA2){pdat->aid = ACTID_ATT_CA;return;}
		}
		else{
			if(key_now & KEYSTA_BC2){
				pdat->aid = ACTID_ATT_SC;return;
			}
			else if(key_now & KEYSTA_BB2){
				pdat->aid = ACTID_ATT_SB;return;
			}
			else if(key_now & KEYSTA_BA2){
				pdat->aid = ACTID_ATT_SA;return;
			}
		}
	}

	if(pdat->aid == ACTID_JAMPS){
		if(key_now & KEYSTA_FOWORD){//�O�W�����v
			pdat->aid = ACTID_JAMPF;
			return;
		}
		else if(key_now & KEYSTA_BACK){//��W�����v
			pdat->aid = ACTID_JAMPB;
			return;
		}
	}

	if(com66(20)){pdat->aid = ACTID_DASH; return;}
	if(com44(20)){pdat->aid = ACTID_DASHB;return;}
*/
	Exchange(pdat, &olddat, FALSE);
	if (olddat.command_a != NULL)
		olddat.command_a();
	Exchange(pdat, &olddat, TRUE);

	//�X�g���C�J�[
	if(callstriker){
		pdat->aid = ACTID_STRIKERCOMEON;
		return;
	}
	//���[����
	else if( (key_now&KEYSTA_FOWORD) && (key_now&KEYSTA_BD2) && !(key_now&KEYSTA_DOWN) && pdat->y == 0 ){
		if((*funcs->msg2system)(pdat->id,MSGOBJ2SYS_KOUTAI1,0)){
			pdat->aid = ACTID_KOUTAIOUT;
			return;
		}
	}
	else if( (key_now&KEYSTA_BACK) && (key_now&KEYSTA_BD2) && !(key_now&KEYSTA_DOWN)  && pdat->y == 0 ){
		if((*funcs->msg2system)(pdat->id,MSGOBJ2SYS_KOUTAI2,0)){
			pdat->aid = ACTID_KOUTAIOUT;
			return;
		}
	}

	if(pdat->aid==ACTID_STRIKERCOMEON){//���͂��Ȃ���΃|�[�Y�L��
		if(!(key_now & (KEYSTA_UP2|KEYSTA_DOWN2|KEYSTA_FOWORD2|KEYSTA_BACK2))){
			return;
		}
	}

}

DWORD CCharAdaptor::CommandCOM(DWORD wid)
{
	BOOL stock = 0;

	if(pdat->aid & ACTID_SYSTEM){
		Exchange(pdat, &olddat, FALSE);
		if (olddat.command_b != NULL)
			stock = olddat.command_b(wid);
		Exchange(pdat, &olddat, TRUE);
		return(stock);
	}
//	if(pdat->aid == ACTID_TOJYO || pdat->aid==ACTID_SYORI)return(FALSE);
//	if(pdat->aid & ACTID_NAGE)return(FALSE);
	if(pdat->aid & ACTID_INOUT)return(FALSE);

	Exchange(pdat, &olddat, FALSE);
	if (olddat.command_b != NULL)
		stock = olddat.command_b(wid);
	Exchange(pdat, &olddat, TRUE);
	return(stock);
/*	if(pdat->aid & ACTID_HISSATU){//�K�E�Z���i��{�I�ɂ�����ׂ��炸�j
		return(FALSE);
	}

	if(pdat->aid & ACTID_KUCYU){
		if(pdat->aid & ACTID_ATTACK){//�󒆍U����
			if(wid&ACTID_HISSATU && wid&ACTID_KUCYU)return(TRUE);//�󒆕K�E�Z�Ȃ�OK
			return(FALSE);//����ȊO�͂���
		}
		else{//�󒆒ʏ퓮�쒆
			if(wid&ACTID_ATTACK && wid&ACTID_KUCYU)return(TRUE);//�󒆒ʏ�Z�Ȃ�OK
			return(FALSE);//����ȊO�͂���
		}
	}
	else{
		if(pdat->aid & ACTID_ATTACK){//�n��U����
			if(wid&ACTID_HISSATU && !(wid&ACTID_KUCYU))return(TRUE);//�n��K�E�Z�Ȃ�OK
			return(FALSE);//����ȊO�͂���
		}
		else{//�n��ʏ퓮��
			if(wid&ACTID_KUCYU && wid&ACTID_ATTACK)return(FALSE);//�󒆍U���͂���
			return(TRUE);//����ȊO�Ȃ�OK
		}
	}*/
}

DWORD CCharAdaptor::Action()
{
	DWORD nowaid = pdat->aid;
	Exchange(pdat, &olddat, FALSE);
	if (olddat.action != NULL)
		olddat.action();
	Exchange(pdat, &olddat, TRUE);

	if(nowaid == ACTID_TOJYO && pdat->aid == ACTID_NEUTRAL)
		(*funcs->msg2system)(pdat->id,MSGOBJ2SYS_TOJYOEND,0);

	if(pdat->aid == ACTID_SYORI)
	{
		if (pdat->counter == 0)
			LoadAndSetKatiSerif(pdat->tid);
		if (pdat->counter == extwin)
			(*funcs->msg2system)(pdat->id,MSGOBJ2SYS_WINPOSEEND,0);
	}

	if(!(pdat->aid & 0xFFFD0000)){
		Furimuki(pdat->id);
		if(pdat->counter > 2)chainlist=0;
	}

	switch(pdat->aid){
	case ACTID_KOUTAI	:act_koutai();break;
	case ACTID_KOUTAI2	:act_koutai2();break;
	case ACTID_TAIKI	:act_taiki();break;
	case ACTID_KOUTAIOUT:act_koutai_out();break;
	case ACTID_KOUTAIIN	:act_koutai_in();break;
	case ACTID_STRIKERCOMEON:act_strikercomeon();break;
	default:return(0);
	}

	if(!(pdat->aid & 0xFFFD0000)){
		Furimuki(pdat->id);
		if(pdat->counter > 2)chainlist=0;
	}
	return(1);
}

DWORD CCharAdaptor::TouchA(ATTACKINFO *info)
{
	DWORD key_now = 0, nowaid = pdat->aid;
	MYATTACKINFO makurai;

	if(pdat->aid & ACTID_NAGE || pdat->aid & ACTID_INOUT)
		return(TOUCHA_AVOID);

	GetMyAttackInfo(pdat->atk2.oid, info, &makurai, FALSE);
	Exchange(pdat, &olddat, FALSE);
	if (olddat.touch_a != NULL)
		key_now = olddat.touch_a(&makurai);
	Exchange(pdat, &olddat, TRUE);

	if (pdat->aid != nowaid)
		funco->actidchanged(pdat->id);

	switch(key_now) {
	case TOUCHA_KURAIS:
	case TOUCHA_KURAIC:
	case TOUCHA_KURAIJ:
		AddPowerGauge(pdat, g_blown);
		break;
	case TOUCHA_GUARDS:
	case TOUCHA_GUARDC:
	case TOUCHA_GUARDJ:
		AddPowerGauge(pdat, g_guard);
		break;
	case 0:
		return(TOUCHA_AVOID);
	}

	return(key_now);//���Ƃ̓f�t�H���g�ɔC���邺
}

DWORD CCharAdaptor::TouchB(ATTACKINFO *info,BOOL hit)
{
	DWORD stock = 0, nowaid = pdat->aid;
	MYATTACKINFO mainfo;

	if (info == &atkmuki)
		return 0;

	GetMyAttackInfo(pdat->id, info, &mainfo, TRUE);
	Exchange(pdat, &olddat, FALSE);
	if (olddat.touch_b != NULL)
		stock = olddat.touch_b(&mainfo, hit);
	Exchange(pdat, &olddat, TRUE);

	//�Q�[�W�̑���
	if(hit) AddPowerGauge(pdat, pg_hit);
	else AddPowerGauge(pdat, pg_guard);

//	if(pdat->gauge > pdat->gaugemax)
//		pdat->gauge = pdat->gaugemax;

	if (pdat->aid != nowaid)
		funco->actidchanged(pdat->id);

	return(stock);
}

void CCharAdaptor::ActionIDChanged()
{
	pdat->z = ZZAHYO_CHARACTER;
}

void CCharAdaptor::movex(double mx)
{
	if(pdat->muki)mx*=-1;
	pdat->x += mx;
}

int CCharAdaptor::zurex(int x)
{
	if(pdat->muki)return(x*-1);
	else return(x);
}

DWORD CCharAdaptor::BulletHit(MYATTACKINFO *info, BOOL hit)
{
	DWORD stock = 0, nowaid = pdat->aid;

	Exchange(pdat, &olddat, FALSE);
	if (olddat.touch_b != NULL)
		stock = olddat.touch_b(info, hit);
	Exchange(pdat, &olddat, TRUE);

	if(hit) AddPowerGauge(pdat, info->pg_hit);
	else AddPowerGauge(pdat, info->pg_guard);

	if (pdat->aid != nowaid)
		funco->actidchanged(pdat->id);

	return(stock);
}
