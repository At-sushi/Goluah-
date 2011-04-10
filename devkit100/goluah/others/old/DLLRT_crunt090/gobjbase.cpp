#include <windows.h>
#include <stdio.h>

//#define LPDIRECT3DTEXTURE8 LPVOID
#include "gobject.h"
#include "gobjbase.h"

DI_FUNCTIONS_S *funcs = NULL;
DI_FUNCTIONS_O *funco = NULL;
DI_FUNCTIONS_D *funcd = NULL;
DI_FUNCTIONS_N *funcn = NULL;

/* **************************************************************************

  CCharacterInfo : �L�����N�^�[���ݒ�N���X

�@�\�t�g���L�����N�^�[��F�����邽�߂ɕK�v�ȏ���ݒ肷��N���X�ł��B
�@�L�����N�^�[DLL���ŁA���̃N���X���O���[�o���錾���A
�@SET_CHARACTERINFO�}�N�����g�p���Ă��������B

 ************************************************************************** */

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CCharacterInfo::CCharacterInfo(LPCTSTR CharName, DWORD CdiVersion, CHARACTER_LOAD_OPTION* Options/* = NULL */,
		int num_Options/* = 0 */, DWORD max_option_point/* = 0 */, BOOL isNetworkable/* = FALSE */)
{
	version = CdiVersion;
	m_MaxPoint = max_option_point;
	m_isNetworkable = isNetworkable;

	if (CharName) {
		int length = lstrlen(CharName);

		if (length >= 32) length = 32 - 1;
		charactername = new char[length + 1];
		if (charactername)
			strncpy(charactername, CharName, length + 1);
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

CCharacterInfo::~CCharacterInfo()
{
	if (charactername)
		delete [] charactername;

	if (m_Options)
		delete [] m_Options;
}

BOOL CCharacterInfo::DllCharacterInfo(LPVOID info)
{
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

void CCharacterInfo::DllCreateCharacter(PVOID info)
{
	CDI_CHARACTERINFO2 *info2 = (CDI_CHARACTERINFO2*)info;

	funco = info2->funco;
	funcs = info2->funcs;
	funcd = info2->funcd;
	if (version >= 900)
		funcn = info2->funcn;
}

void CCharacterInfo::SetOptionItems(CDI_CHARACTERINFO* pif)
{
	pif->max_option_point = m_MaxPoint;

	for (int i = 0; i < m_NumOptions; i++)
		pif->options[i] = m_Options[i];
}

/* **************************************************************************

  CGoluahObject : �I�u�W�F�N�g��{�N���X

�@��Ȗ����̓I�u�W�F�N�g�̍쐬�ƃ��b�Z�[�W�����֐��̐ݒ�B
�@�悭�g�����b�Z�[�W�Ɋւ��Ă͊֐������łɒ�`����Ă���̂ŁA������I�[�o�[���C�h�B
�@����Ȃ�����Message�֐����I�[�o�[���C�h���ă��b�Z�[�W������ǉ�����B

 ************************************************************************** */

unsigned short CGoluahObject::gCreateCounter = 0xFFFF;
int CGoluahObject::m_ObjCount = 0;
CGoluahObject* CGoluahObject::pObjToDelete = NULL;
CGoluahObject* CGoluahObject::pObjDeleting = NULL;

CGoluahObject::CGoluahObject()
{
	CGoluahObjectCreate();
}

CGoluahObject::CGoluahObject(CDI_CHARACTERINFO2 *info,BYTE userID,BYTE userNo)
{
	if(info->funcn == NULL){
		CGoluahObjectCreate();
		return;
	}

	dllID = (BYTE)info->dllid;
	g_chardir = info->dir;

	//�I�u�W�F�N�g�𐶐�
	if (userID == 0xFF && userNo == 0xFF) { // �f�t�H���g����
		oid = (*funcn->obj_regist)(dllID,gCreateCounter / 0x0100,gCreateCounter % 0x0100);
		gCreateCounter--;
		m_ObjCount++;
		m_hasDefaultID = TRUE;
	}
	else {
		oid = (*funcn->obj_regist)(dllID,userID,userNo);
		m_hasDefaultID = FALSE;
	}
	pdat = (GOBJECT*)(*funco->getinfo)(oid);//�f�[�^�ւ̃|�C���^��get
	if(pdat!=NULL){
		pdat->pobjdat_u = this;
		pdat->msghandler	= gMessageToObject;//���b�Z�[�W�����֐�
	}
	base_z				= 0;

	m_network= TRUE;
	m_remote = info->isRemote;
}

void CGoluahObject::CGoluahObjectCreate()
{
	dllID = 0xFF;

	oid = (*funco->objcreate)();//�I�u�W�F�N�g�𐶐�
	pdat = (GOBJECT*)(*funco->getinfo)(oid);//�f�[�^�ւ̃|�C���^��get
	if(pdat!=NULL){
		pdat->pobjdat_u = this;
		pdat->msghandler= gMessageToObject;//���b�Z�[�W�����֐�
	}
	base_z				= 0;

	m_network= FALSE;
	m_remote = FALSE;
	m_hasDefaultID = FALSE;
	g_chardir = "";
}

CGoluahObject::~CGoluahObject()
{
	if (m_hasDefaultID)
	{
		m_ObjCount--;
		if (m_ObjCount == 0)
			gCreateCounter = 0xFFFF;
	}

	if (pObjToDelete == this)
		pObjToDelete = NULL;
	else if (pdat != NULL) {
		pObjDeleting = this;
		funco->objdelete(pdat->id);
	}

#ifdef _DEBUG
	OutputDebugString("DLL CGoluahObject �f�X�g���N�^���܂���\n");
#endif
}

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
	case GOBJMSG_DRAW:		return(Draw());
	case GOBJMSG_DRAWFRONT:	return(DrawFront());
	case GOBJMSG_DRAWBACK:	return(DrawBack());
	default:
		//���Ƃ̃��b�Z�[�W�̓f�t�H���g�����ɔC����
		return(0);
	}

	return(TRUE);
}

DWORD CGoluahObject::gMessageToObject(DWORD msg,LPVOID pd,DWORD prm)
{
	if(pd==NULL)return(0);
	GOBJECT *pdat = (GOBJECT*)pd;
	CGoluahObject *pc = (CGoluahObject*)(pdat->pobjdat_u);

	if(pc!=NULL){
		DWORD result;

		if (pObjDeleting == pc) {	// �蓮������G���[�h�~
			if(msg==GOBJMSG_DELETE) pObjDeleting = NULL;
			return 1;	// �f�t�H���g����͍s���܂���
		}

		result = pc->Message(msg,pd,prm);
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

DWORD CGoluahObject::Action(){return(TRUE);}

void CGoluahObject::Command(){}

DWORD CGoluahObject::CommandCOM(DWORD wid){return(FALSE);}

DWORD CGoluahObject::TouchA(ATTACKINFO *info,DWORD ta_eid)
{
	return(0);//�f�t�H���g�̃K�[�h���菈��
}

DWORD CGoluahObject::TouchB(ATTACKINFO *info,BOOL hit)
{
	pdat->kougeki=FALSE;//�U���̖͂�����
	return(0);
}

DWORD CGoluahObject::TouchC(ATTACKINFO *info,DWORD tc_eid){return(0);}

void CGoluahObject::ActionIDChanged()
{
	pdat->z=base_z;//z���W����l�ɖ߂�
}

double CGoluahObject::zurex(double x)
{
	if(pdat->muki)return(x*-1);
	else return(x);
}

int CGoluahObject::zurex(int x)
{
	if(pdat->muki)return(x*-1);
	else return(x);
}

//�L�����N�^�[���ړ�
void CGoluahObject::movex(double dx)
{
	if(pdat->muki)dx*=-1;
	pdat->x += dx;
}
void CGoluahObject::movex(int dx)
{
	if(pdat->muki)dx*=-1;
	pdat->x += dx;
}

void CGoluahObject::AddProperty(DWORD prop)
{
	pdat->objtype |= prop;
}

void CGoluahObject::RemoveProperty(DWORD prop)
{
	pdat->objtype &= ~(prop);
}

void CGoluahObject::SetProperty(DWORD prop)
{
	pdat->objtype = prop;
}

DWORD CGoluahObject::DrawFront(){return(FALSE);}

DWORD CGoluahObject::DrawBack(){return(FALSE);}

DWORD CGoluahObject::Draw(){return(FALSE);}

void CGoluahObject::ChangeAction(DWORD actid)
{
	if(pdat->aid!=actid){
		pdat->aid=actid;
		funco->actidchanged(pdat->id);
	}
}

// �V�X�e���֘A�̊֐�
DWORD CGoluahObject::GetKey(DWORD keyinput, DWORD interval) {
	return funcs->getkey(keyinput, interval);
}
int CGoluahObject::SeekKey(DWORD keyinput, int offset, int delay, DWORD keystate) {
	return funcs->seekkey(keyinput,  offset,  delay, keystate);
}
void CGoluahObject::SetKatiSerif(DWORD tid, char* serif) {
	funcs->setkatiserif(tid, serif);
}
void CGoluahObject::AddEffect(DWORD EffectID, int prm1, int prm2, DWORD prm3) {
	funcs->addeffect(EffectID, prm1, prm2, prm3);
}
void CGoluahObject::PlaySysSound(DWORD SoundNo) {
	funcs->playsyssound(SoundNo);
}
LPVOID CGoluahObject::LoadMySound(char* pathname) {
	return funcs->loadmysound(pathname);
}
void CGoluahObject::PlayMySound(LPVOID psound) {
	funcs->playmysound(psound);
}
void CGoluahObject::KillMySound(LPVOID psound) {
	funcs->killmysound(psound);
}
DWORD CGoluahObject::Message2System(DWORD mes, DWORD prm) {
	return funcs->msg2system(pdat->id, mes, prm);
}
char* CGoluahObject::GetEnemyName(DWORD tid) {
	return funcs->getenemyname(tid);
}
/*char* CGoluahObject::GetCharacterName(DWORD oid) {
	return funcs->getcharname(oid);
}*/
double CGoluahObject::GetDisplayCenter_X() {
	return funcs->getdispcenterx();
}
double CGoluahObject::GetDisplayCenter_Y() {
	return funcs->getdispcentery();
}
DWORD CGoluahObject::GetTeamNum() {
	return funcs->getteamnum();
}
DWORD CGoluahObject::GetTaisenKeisiki() {
	return funcs->gettaisenkeisiki();
}

// �I�u�W�F�N�g�֘A�̊֐�
BOOL   CGoluahObject::ObjCatch(DWORD eid,DWORD msg_nage) {		//�����Ƃ��ő��������
	CATCHYOU cy;

	cy.oid = pdat->id;
	cy.actmsg = msg_nage;
	return funco->objcatch(eid, &cy);
}
GOBJECT* CGoluahObject::GetInfo(DWORD oid) {					//���
	return (GOBJECT*)funco->getinfo(oid);
}
void   CGoluahObject::AddDamage(DWORD eid,int x,int y) {//(��ɓ����̂Ƃ�)�����I�Ƀ_���[�W��^����
	funco->adddamage(pdat->id, eid, x, y);
}
GOBJECT*  CGoluahObject::GetActiveCharacter(DWORD tid) {		//���݃A�N�e�B�u�ȃL�����N�^�[�̏��
	return (GOBJECT*)funco->getactivechar(tid);
}
void   CGoluahObject::Suicide() {								//�I�u�W�F�N�g����
	funco->suicide(pdat->id);
}
DWORD  CGoluahObject::GetMaai_H(DWORD oid, DWORD eoid) {//�ԍ�����get(����)
	return funco->getmaai_h(oid, eoid);
}
DWORD  CGoluahObject::GetMaai_V(DWORD oid, DWORD eoid) {//�ԍ�����get(����)
	return funco->getmaai_v(oid, eoid);
}
DWORD  CGoluahObject::GetCharacterID(DWORD tid,DWORD index) {//�L�����N�^�[�̃I�u�W�F�N�gID
	return funco->getcharid(tid, index);
}

//�@�`��֘A�̊֐��̒�`
LPDIRECT3D8 CGoluahObject::GetD3D() {							//IDirect3D* �̎擾
	return (LPDIRECT3D8)funcd->getd3d();
}
LPDIRECT3DDEVICE8 CGoluahObject::GetD3DDevice() {				//IDirect3DDevice* �̎擾
	return (LPDIRECT3DDEVICE8)funcd->getd3dd();
}
void   CGoluahObject::LoadCellData(char* pathname,GCD_CELL2_070* pCells,GCD_RECT* pRects,GCD_HANTEI* pHanteis) {//�Z���f�[�^�ǂݍ��݊֐�
	funcd->loadcelldat(pathname, pCells, pRects, pHanteis);
}
MYSURFACE* CGoluahObject::LoadBitmap(char* PathName,char* PalletFileName) {				//�r�b�g�}�b�v�ǂݍ��݊֐�
	return (MYSURFACE*)funcd->loadbmp(PathName, PalletFileName);
}
void   CGoluahObject::UnloadBitMap(MYSURFACE* Bitmap) {					//�r�b�g�}�b�v��n���֐�
	funcd->unloadbmp(Bitmap);
}
void   CGoluahObject::CellDraw(MYSURFACE** pBmps,GCD_CELL2* cdat,GCD_RECT* rdat,
	DWORD cnum,int x,int y,float z,int Rotate,BOOL ReverseX,BOOL ReverseY,DWORD Color,float magx,float magy) {//�Z���`��֐�
	funcd->celldraw((void**)pBmps, cdat, rdat,
	 cnum, x, y, z, Rotate, ReverseX, ReverseY, Color, magx, magy);
}
void   CGoluahObject::CkBlt(MYSURFACE* pBmp,int x1,int y1,RECT bltrect,
	double magx,double magy,BOOL revx,BOOL revy,float z,DWORD color) {//Blt2
	funcd->ckblt(pBmp, x1, y1, bltrect,
	 magx,  magy, revx, revy, z, color);
}
void   CGoluahObject::Blt3D(MYSURFACE* pBmp,RECT bltrect,MYRECT3D rect,DWORD color) {	//Blt3
	funcd->blt3d( pBmp, bltrect, rect, color);
}
void   CGoluahObject::SetTransform(BOOL BonVoyage) {				//�ϊ��s��ݒ�
	funcd->settrans(BonVoyage);
}
void   CGoluahObject::SetBlend(DWORD type) {					//�u�����h�̎d����ύX
	funcd->setblend(type);
}
void   CGoluahObject::SetParentMatrix(LPD3DXMATRIX pMatrix,BOOL erase,LPD3DXMATRIX OldMatrix) {//�u�e�v�̕ϊ��s��ݒ�
	funcd->setparentmat( pMatrix, erase, OldMatrix);
}
DWORD  CGoluahObject::CreateCellData(char* filename,GCD_CELL2 **cdat,GCD_RECT **rdat,GCD_HANTEI **hdat) {//�Z���f�[�^�ǂݍ��݁Bver0.90�ȍ~�ł�LoadCellDat���炱����ɕύX����ׂ�
	return funcd->create_celldat(filename, (void**)cdat, (void**)rdat, (void**)hdat);
}
void   CGoluahObject::DestroyCellData(GCD_CELL2 **cdat,GCD_RECT **rdat,GCD_HANTEI **hdat) {//CreateCellDat�Ő������ꂽ�o�b�t�@���N���A
	funcd->destroy_celldat( (void**)cdat, (void**)rdat, (void**)hdat);
}

//�@�l�b�g���[�N�֘A�̊֐��̒�`
void CGoluahObject::SendSynchronizedMessage(DWORD size,LPVOID dat) {
	if (funcn) funcn->send_syncmsg( pdat->id, size, dat);
}
void CGoluahObject::SetSynchronize(BOOL enabled) {
	if (funcn) funcn->set_syncronize( pdat->id, enabled);
}

/* **************************************************************************

 CCharacterBase: �L�����N�^�[�̊�{�N���X

�@ |�@�@�@�@�@�@ �@ |�@ |�@�@�@�@�@�@________________________________________________
�@ |�@�@�@�@�@�@ �@ |�@ |�Q�Q�Q�Q_����������������������||������
�@ |�@�@�@�@�@�@ �@ |�@ |�P�P�P �^|�@�@�@�@�@ �@ �@ �@ �@ �@ �@ �@ ||
�@ |�@�@�@�@�@�@ �@ |�@ |�@�@�@�^ �^|TTTTTT�@�@ TTTTTTTTTT||TTTTT
�@ |�@ �@ �@ �@ �^�_�@|�@ �^|�^|�^|^^^^^^ |�O�O| ^^^^^^^^^^^||^^^^^^^
�@ |�@ �@ �@ �^�@ �^�@|�^�^ �^ �^|
�@ |�@�@�@�^�@ �^ |�Q|�^|�^|�^|�^|
�@ |�@ �^�@ �^�@�@|��|�^ �^�^ �^
�@ |�^�@ �^.�@ �Q.|�P|�^|�^|�^�@�@�@�@�@�@�@�@�@�@�@�@�ȁQ��
�^|�_�^ �@�^�@�^ �@|�^ �^�@�@�@�@�@�@�@�@�@�@ �@�@�@�i�Q�Q_�j
�^|�@ �@ �^�@�^�@ �^�R�@�@�@�@�@�@�@�@�@�@�@�@�@�@/�k�@�Ձ@�l�l��
�@ |�@�@�@|�P|�@ |�@|�R�^l�@�@�@�@�@�@�@�@�@�@�@�@ �M/���R
�@ |�@�@�@|�@ |�^|�@|__|�^�@�@�@�ȁQ�ȁ@�@�@�@�@ /�@/�i�Q�j
�@ |�@�@�@|�^|�@ |�^�@�@�@�@ �i�@�L�́M�j�@�@�@�@�@�i�Q�j�@�@�ȁQ��
�@ |�@�@�@|�@ |�^�@�@�@�@�@ �^/ /�@ ^�P�n߁@�@�@�@�@�@�@�@�i�M�@�@�@�j
�@ |�@�@�@|�^�@�@�@�@�@�@�@ �/�P�P_�R�@�@�@�@�@�@�@�@�@���k�k�@�Ձ@�l
�@ |�@ �^�@�@�@�@�@�@�@�@�@/�Q�m�i�Q�j�@�@�@�@�@�@�@�@�@ ��|�Q�Q_|
�@ |�^�@�@�@�@�@�@�@�@�@�@�i�Q_�j�@�@�@�@�@�@�@�@�@�@�@ �i�Q� �R�@�R
�^�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�i�Q�j

 ************************************************************************** */

CCharacterBase::CCharacterBase(CCharacterBase *parent/* = NULL */) : CGoluahObject()
{
	parent_char = parent;

	if(parent!=NULL){
		keyinput = parent->keyinput;//���Ԃ���L�[���͂��󂯎�邩
		pal_number = parent->pal_number;//�p���b�g�ԍ�
		GetGObject()->tid		= parent->GetGObject()->tid;//�`�[��ID
		g_chardir = parent->g_chardir;
	}
	else {
		keyinput = 0;//���Ԃ���L�[���͂��󂯎�邩
		pal_number = 0;//�p���b�g�ԍ�
		GetGObject()->tid		= 0;//�`�[��ID
	}

	this->CCharacterBaseCreate();
}

CCharacterBase::CCharacterBase(CDI_CHARACTERINFO2 *info, BYTE userID, BYTE userNo)
					: CGoluahObject(info,userID,userNo)
{
	parent_char = NULL;

	GetGObject()->tid		= info->tid;//�`�[��ID
	keyinput		= info->keyinput;//���Ԃ���L�[���͂��󂯎�邩
	pal_number		= info->color;//�p���b�g�ԍ�
	g_chardir		= info->dir;

	this->CCharacterBaseCreate();
}

void CCharacterBase::CCharacterBaseCreate()
{
	GOBJECT* pdat = GetGObject();

	for(int i=0;i<GCDMAX_IMAGES;i++)bitmaps[i]=NULL;
	cells=NULL;
	hantei=NULL;
	rects=NULL;
	for(i=0;i<CBASE_NUMMYSOUND;i++)mysounds[i]=NULL;

	pdat->objtype	= GOBJTYPE_CHARACTER;//�I�u�W�F�N�g�^�C�v
	pdat->winfo		= &(this->waz);
	ZeroMemory(pdat->winfo,sizeof(WAZAINFO));

	base_z	=ZZAHYO_CHARACTER;
	front_z	=ZZAHYO_CHARACTER_F;
	back_z	=ZZAHYO_CHARACTER_B;

	chainComboEnabled = TRUE;	//�`�F�[���R���{�L��
	isSuperArmer = FALSE;		//�X�[�p�[�A�[�}�[����
	isAutoGuard = FALSE;		//�I�[�g�K�[�h����
}

void CCharacterBase::InitAll()
{
	InitAttackInfo();
	InitGCDandBMP();
	InitWazInfo();
	InitMySound();
}

CCharacterBase::~CCharacterBase()
{
	ReleaseGCDandBMP();
	ReleaseMySound();
}

DWORD CCharacterBase::Message(DWORD msg,LPVOID pd,DWORD prm)
{
	switch(msg){
	case GOBJMSG_DOTOJYO://�o��
		ChangeAction( ACTID_TOJYO );return(TRUE);
	case GOBJMSG_DOYOUWIN://����
		ChangeAction( ACTID_SYORI );return(TRUE);
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
	case GOBJMSG_KOUTAI://��サ�냁�b�Z�[�W
		if(GetGObject()->aid == ACTID_TAIKICYU){
			ChangeAction( ACTID_KOUTAIIN );
			return(TRUE);
		}
		return(0);
	case GOBJMSG_KOUTAI2://��サ�냁�b�Z�[�W2�i�̂�т�Ɠo��|�[�Y�����߂Ăėǂ��j
		ChangeAction( ACTID_KOUTAI );
		break;
	case GOBJMSG_CLIPX:
		if(GetGObject()->aid == ACTID_KOUTAI)return(TRUE);//��㎞�̓N���b�v���������Ȃ�
		else return(0);
	case GOBJMSG_CNGROUND:
		return(TRUE);//���E���h���ς���Ă������Ȃ�
	default:
		//���Ƃ̃��b�Z�[�W�̓I�u�W�F�N�g��{�N���X�ɔC����
		return(CGoluahObject::Message(msg,pd,prm));
	}
	return(0);
}

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
	case ACTID_STRIKERCOMEON:this->act_strikercomeon();break;
	default:return(0);
	}
	return(TRUE);
}

void CCharacterBase::PreAction()
{
	if(!(GetGObject()->aid & 0xFFFD0000)){
		Furimuki();
		if(GetGObject()->counter > 2)chainlist=0;
	}
}

void CCharacterBase::PostAction()
{
	if(!(GetGObject()->aid & 0xFFFD0000)){
		Furimuki();
		if(GetGObject()->counter > 2)chainlist=0;
	}
}

void CCharacterBase::Command()
{
	DWORD key_now = (*funcs->getkey)(keyinput,0);
	BOOL callstriker=FALSE;
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_SYSTEM){//�V�X�e����`���쒆�Ȃ̂ŁA��{�I�ɂ͂�����Ȃ�
		Command_OnSystem(key_now);
		return;
	}
	if(pdat->aid == ACTID_TOJYO || pdat->aid==ACTID_SYORI)return;
	if(pdat->aid & ACTID_NAGE)return;
	if(pdat->aid & ACTID_INOUT)return;

	//�X�g���C�J�[���Ă�
	if( (key_now&KEYSTA_FOWORD) && (key_now&KEYSTA_DOWN) && (key_now&KEYSTA_BD2) ){
		if((*funcs->msg2system)(pdat->id,MSGOBJ2SYS_STRIKER1,0)){
			ADDEFFECT(EFCTID_COMEON,(int)pdat->x,(int)pdat->y-100,pdat->muki);
			callstriker=TRUE;
		}
	}
	else if( (key_now&KEYSTA_BACK) && (key_now&KEYSTA_DOWN) && (key_now&KEYSTA_BD2) ){
		if((*funcs->msg2system)(pdat->id,MSGOBJ2SYS_STRIKER2,0)){
			ADDEFFECT(EFCTID_COMEON,(int)pdat->x,(int)pdat->y-100,pdat->muki);
			callstriker=TRUE;
		}
	}

	if(pdat->aid & ACTID_HISSATU){//�K�E�Z���i��{�I�ɂ�����ׂ��炸�j
		Command_OnHissatuAttacking(key_now);
		return;
	}

	//�K�E�Z�R�}���h����
	if (Command_Hissatu(key_now))
		return;

	if(pdat->aid & ACTID_ATTACK){//�U�����쒆�i��{�I�ɂ�����Ȃ��B�`�F�[���R���{�o���Ȃ炱���ł�����j
		Command_OnAttacking(key_now);
		return;
	}

	//��{���쒆
	if (Command_OnNormal(key_now) || pdat->aid & ACTID_KUCYU)
		return;

	//�X�g���C�J�[
	if(callstriker){
		pdat->aid = ACTID_STRIKERCOMEON;
		return;
	}
	//���[����
	else if( (key_now&KEYSTA_FOWORD) && (key_now&KEYSTA_BD2) && !(key_now&KEYSTA_DOWN) ){
		if((*funcs->msg2system)(pdat->id,MSGOBJ2SYS_KOUTAI1,0)){
			pdat->aid = ACTID_KOUTAIOUT;
			return;
		}
	}
	else if( (key_now&KEYSTA_BACK) && (key_now&KEYSTA_BD2) && !(key_now&KEYSTA_DOWN) ){
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

	Command_Normal(key_now);
}

BOOL CCharacterBase::Command_Hissatu(DWORD keyinfo)
{
	return FALSE;
}

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
	else if(keyinfo & KEYSTA_BACK){//������
		pdat->aid = ACTID_WALKB;
	}
	else{//�j���[�g�����|�[�Y
		pdat->aid = ACTID_NEUTRAL;
	}

	return TRUE;
}

BOOL CCharacterBase::Command_OnSystem(DWORD keyinfo)
{
	return FALSE;
}

BOOL CCharacterBase::Command_OnHissatuAttacking(DWORD keyinfo)
{
	return FALSE;
}

BOOL CCharacterBase::Command_OnAttacking(DWORD keyinfo)
{
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_KUCYU){////�W�����v���쒆
		return FALSE;
	}
	else{
		if(chainComboEnabled && keyinfo & 0x22220000){
			if(keyinfo & KEYSTA_DOWN){
				if(keyinfo & KEYSTA_BC2){if(ChainCombo(CHAIN_CC)){pdat->aid = ACTID_ATT_CC;return TRUE;}}
				else if(keyinfo & KEYSTA_BB2){if(ChainCombo(CHAIN_CB)){pdat->aid = ACTID_ATT_CB;return TRUE;}}
				else if(keyinfo & KEYSTA_BA2){if(ChainCombo(CHAIN_CA)){pdat->aid = ACTID_ATT_CA;return TRUE;}}
			}
			else{
				if(keyinfo & KEYSTA_BC2){if(ChainCombo(CHAIN_SC)){pdat->aid = ACTID_ATT_SC;return TRUE;}}
				else if(keyinfo & KEYSTA_BB2){if(ChainCombo(CHAIN_SB)){pdat->aid = ACTID_ATT_SB;return TRUE;}}
				else if(keyinfo & KEYSTA_BA2){if(ChainCombo(CHAIN_SA)){pdat->aid = ACTID_ATT_SA;return TRUE;}}
			}
		}
	}

	return FALSE;
}

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
		else if(keyinfo & KEYSTA_BACK){//��W�����v
			pdat->aid = ACTID_JAMPB;
			return TRUE;
		}
	}

	return FALSE;
}

DWORD CCharacterBase::TouchA(ATTACKINFO *info,DWORD ta_eid)
{
	GOBJECT* pdat = GetGObject();

	//�X�[�p�[�A�[�}�[�����B�U���𖳎�
	if(isSuperArmer){
		if(pdat->hp>0)
			return TOUCHA_MUSI;
	}
	//�I�[�g�K�[�h�����B�U���E��炢���ȊO�͖������K�[�h
	if(isAutoGuard){
		if(pdat->aid & ACTID_ATTACK)return 0;
		if(pdat->aid & ACTID_KURAI)return 0;
		if(pdat->aid & ACTID_KUCYU)return TOUCHA_GUARDJ;
		if(pdat->aid & ACTID_SYAGAMI)return TOUCHA_GUARDC;
		return TOUCHA_GUARDS;
	}
	return CGoluahObject::TouchA(info,ta_eid);
}

DWORD CCharacterBase::CommandCOM(DWORD wid)
{
	GOBJECT* pdat = GetGObject();

	if(pdat->aid & ACTID_SYSTEM)return(CmdCom_OnSystem(wid));
	if(pdat->aid == ACTID_TOJYO || pdat->aid==ACTID_SYORI)return(FALSE);
	if(pdat->aid & ACTID_NAGE)return(FALSE);
	if(pdat->aid & ACTID_INOUT)return(FALSE);
	
	if(pdat->aid & ACTID_HISSATU){//�K�E�Z���i��{�I�ɂ�����ׂ��炸�j
		return(CmdCom_OnHissatuAttacking(wid));
	}

	if(pdat->aid & ACTID_KUCYU){
		return(CmdCom_OnKucyu(wid));
	}
	else{
		return(CmdCom_OnNormal(wid));
	}
}

DWORD CCharacterBase::CmdCom_OnSystem(DWORD wid)
{
	return(FALSE);
}

DWORD CCharacterBase::CmdCom_OnHissatuAttacking(DWORD wid)
{
	return(FALSE);
}

DWORD CCharacterBase::CmdCom_OnKucyu(DWORD wid)
{
	if(GetGObject()->aid & ACTID_ATTACK){//�󒆍U����
		if(wid&ACTID_NAGE)return(FALSE);
		if(wid&ACTID_HISSATU && wid&ACTID_KUCYU)return(TRUE);//�󒆕K�E�Z�Ȃ�OK
		return(FALSE);//����ȊO�͂���
	}
	else{//�󒆒ʏ퓮�쒆
		if(wid&ACTID_ATTACK && wid&ACTID_KUCYU)return(TRUE);//�󒆒ʏ�Z�Ȃ�OK
		return(FALSE);//����ȊO�͂���
	}
}

DWORD CCharacterBase::CmdCom_OnNormal(DWORD wid)
{
	if(GetGObject()->aid & ACTID_ATTACK){//�n��U����
		if(wid&ACTID_NAGE)return(FALSE);
		if(wid&ACTID_HISSATU && !(wid&ACTID_KUCYU))return(TRUE);//�n��K�E�Z�Ȃ�OK
		return(FALSE);//����ȊO�͂���
	}
	else{//�n��ʏ퓮��
		if(wid&ACTID_KUCYU && wid&ACTID_ATTACK)return(FALSE);//�󒆍U���͂���
		return(TRUE);//����ȊO�Ȃ�OK
	}
}

void CCharacterBase::InitAttackInfo(){}

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

	//"image?.bmp" ��ǂݍ���
	sprintf(palname,"%s\\pal%d.bmp",g_chardir,pal_number);
	for(i=0;i<GCDMAX_IMAGES;i++){
		sprintf(filename,"%s\\image%d.bmp",g_chardir,i+1);
		bitmaps[i] = (MYSURFACE*) ( (*funcd->loadbmp)(filename,palname) );
	}

	//"cell.gcd"��ǂݍ���
	hantei = (GCD_HANTEI*)calloc(GCDMAX_CELLS, sizeof(GCD_HANTEI));
	rects = (GCD_RECT*)calloc(GCDMAX_RECTANGLES, sizeof(GCD_RECT));
	cells = (GCD_CELL2*)calloc(GCDMAX_CELLS, sizeof(GCD_CELL2_070));
	sprintf(filename,"%s\\cell.gcd",g_chardir);
	(*funcd->loadcelldat)(filename,cells,rects,hantei);
	if (((GCD_CELL2_070*)cells)[0].cell[0].flag >= 900)
	{	// �Z���̃o�[�W������0.90�ȏ�
		if(cells!=NULL){free(cells);cells=NULL;}
		if(hantei!=NULL){free(hantei);hantei=NULL;}
		if(rects!=NULL){free(rects);rects=NULL;}

		funcd->create_celldat(filename, (void**)&cells, (void**)&rects, (void**)&hantei);
	}

	//pdat�ɓ���Ă���
	pdat->phdat			= hantei;
	pdat->pmsarr		= bitmaps;
	pdat->pcdat			= cells;
	pdat->prdat			= rects;
}

void CCharacterBase::InitMySound()
{
	//sound1.wav �` sound32.wav��ǂݍ���
	char *filename = new char [256];
	for(int i=0;i<CBASE_NUMMYSOUND;i++){
		sprintf(filename,"%s\\sound%d.wav",g_chardir,i+1);
		mysounds[i] = (*funcs->loadmysound)(filename);
	}
	delete [] filename ;
}

void CCharacterBase::PlayMySound(DWORD number)
{
	if(number>CBASE_NUMMYSOUND)return;
	if(mysounds[number]!=NULL){
		funcs->playmysound(mysounds[number]);
	}
}

void CCharacterBase::ReleaseGCDandBMP()
{
	for(int i=0;i<GCDMAX_IMAGES;i++){
		if(bitmaps[i]!=NULL){
			funcd->unloadbmp(bitmaps[i]);
		}
	}
	if(cells!=NULL) {
		if (cells[0].cell[0].flag >= 900)
			funcd->destroy_celldat((void**)&cells, (void**)&rects, (void**)&hantei);
		else {
			free(cells);
			if(hantei!=NULL)free(hantei);
			if(rects!=NULL)free(rects);
		}
	}
	else {
		if(hantei!=NULL)free(hantei);
		if(rects!=NULL)free(rects);
	}
}

void CCharacterBase::ReleaseMySound()
{
	for(int i=0;i<CBASE_NUMMYSOUND;i++){
		if(mysounds[i]!=NULL){
			funcs->killmysound(mysounds[i]);
		}
	}
}

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

void CCharacterBase::CyakuchiHantei(DWORD toaid)
{
	GOBJECT* pdat = GetGObject();

	if(pdat->y > 0){
		pdat->y=0;
		pdat->aid=toaid;
		Furimuki();
	}
}

void CCharacterBase::ZBack(){GetGObject()->z=back_z;}
void CCharacterBase::ZFront(){GetGObject()->z=front_z;}

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

BOOL CCharacterBase::ChainCombo(DWORD chainid)
{
	if(!chainComboEnabled)return FALSE;

	if(chainlist & chainid)return(FALSE);

	chainlist |= chainid;
	return(TRUE);
}

void CCharacterBase::InitWazInfo()
{
	waz.walkf = ACTID_WALKF;
	waz.walkb = ACTID_WALKB;
	waz.jampf = ACTID_JAMPF;
	waz.jampb = ACTID_JAMPB;
	waz.att_short[0] = ACTID_ATT_SA;//�Z���[�`�U��
	waz.att_short[1] = ACTID_ATT_CA;
	waz.att_short[2] = ACTID_ATT_CB;
	waz.att_middle[0] = ACTID_ATT_SB;//�����[�`�U��
	waz.att_middle[1] = ACTID_ATT_CC;
	waz.att_long[0] = ACTID_ATT_SC;//�����[�`�U��
	waz.att_jamp[0] = ACTID_ATT_JC;//�W�����v�U��
	waz.att_jamp[1] = ACTID_ATT_JB;
	waz.att_jamp[2] = ACTID_ATT_JA;
}

void CCharacterBase::AddPowerGauge(double dp)
{
	GOBJECT* pdat = GetGObject();

	pdat->gauge += dp;
	if(pdat->gauge<0)pdat->gauge = 0;
	else if(pdat->gauge > pdat->gaugemax)pdat->gauge = pdat->gaugemax;
}


//���_��
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

//���_�����_��
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

//�����_
BOOL CCharacterBase::com623(int dt)//����[���[���܂��
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

//���^��
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

//�����^
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

//���^���_��
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

//����
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

//����
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

//����
BOOL CCharacterBase::com22(int dt)//�n���n���A�^�b�N�H�R�}���h
{
	int ofst=0;

	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN2|KEYSTA_DOWN);
	if(ofst<0)return(FALSE);
	ofst+=1;
	ofst=(*funcs->seekkey)(keyinput,ofst,dt,KEYSTA_DOWN2|KEYSTA_DOWN);
	if(ofst<0)return(FALSE);

	return(TRUE);
}

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


//  ���_���_���^��
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

//���_���^���^���_��
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

//�����^���_��
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

//*******************************************************************
//�@��{�I�ȍs���֐� �g���܂킵�Ă�������
//*******************************************************************

void CCharacterBase::act_koutai_in()
{
	/*
	 * ��サ�ĉ�ʓ��ɂ͂����Ă���(1)
	 * �o��|�[�Y�͂��Ȃ��B
	 */

	GOBJECT* pdat = GetGObject();

	if(pdat->counter==0){
		pdat->objtype |= GOBJTYPE_DISPLAY;//��ʓ��ɂ���
	}

	GOBJECT *pedat;
	if(pdat->counter==0){
		if(pdat->tid==TEAM_PLAYER1)
			pedat = (GOBJECT*)funco->getactivechar(TEAM_PLAYER2);
		else
			pedat = (GOBJECT*)funco->getactivechar(TEAM_PLAYER1);
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

void CCharacterBase::act_koutai_out()
{
	/*
	 * ��ʊO�ɂ҂�[����ĂłĂ��đҋ@����
	 */

	GOBJECT* pdat = GetGObject();

	if(pdat->counter==0){
		pdat->objtype &= GOBJTYPE_NODISPLAY;//��ʊO�ւ�����悤�ɂ��Ă���
		pdat->objtype |= GOBJFLG_TAIKI;//�ҋ@�t���O�𗧂Ă�
		RemoveProperty( GOBJFLG_TOBETARGET );//�^�[�Q�b�g�ɂ͂Ȃ�Ȃ�
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

void CCharacterBase::act_taiki()//��ʊO�ɔ��ł��đҋ@����
{
	GOBJECT* pdat = GetGObject();

	if(pdat->counter==0){
		pdat->objtype &= GOBJTYPE_NODISPLAY;//��ʊO�ւ�����悤�ɂ��Ă���
		AddProperty( GOBJFLG_TAIKI );//�ҋ@�t���O�𗧂Ă�
		RemoveProperty( GOBJFLG_TOBETARGET );//�^�[�Q�b�g�ɂ͂Ȃ�Ȃ�
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

void CCharacterBase::act_koutai2()//�o��|�[�Y�E�L��
{
	if(GetGObject()->counter<150)act_tojyo();

	else{
		GetGObject()->aid = ACTID_NEUTRAL;
		(*funcs->msg2system)(GetGObject()->id,MSGOBJ2SYS_KOUTAIEND,0);
	}
}

void CCharacterBase::act_koutai()
{
	GOBJECT* pdat = GetGObject();

	pdat->objtype |= GOBJTYPE_DISPLAY;//��ʓ��ɂ���

	GOBJECT *pedat;
	if(pdat->counter==0){
		if(pdat->tid==TEAM_PLAYER1)
			pedat = (GOBJECT*)funco->getactivechar(TEAM_PLAYER2);
		else
			pedat = (GOBJECT*)funco->getactivechar(TEAM_PLAYER1);
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
		pdat->aid = ACTID_KOUTAI2;//�o��|�[�Y
	}
}

void CCharacterBase::act_taikicyu(){}//�ҋ@��

void CCharacterBase::act_strikercomeon()
{
	GetGObject()->aid=ACTID_NEUTRAL;
}

void CCharacterBase::act_rakka2(){act_rakka();}

//**********************************************************
//�@�����䎌�擾����
//**********************************************************

int GetRandNum(int num);
BOOL GetWinSerif(char *getmsg,char *enename,char *filepath);

void CCharacterBase::LoadAndSetKatiSerif()
{
	char katiserifu[256];
	char *enemyname;
	char filepath[256];
	GOBJECT* pdat = GetGObject();

	ZeroMemory(katiserifu,sizeof(katiserifu));
	enemyname = funcs->getenemyname(pdat->tid);
	sprintf(filepath,"%s\\serifu.txt",g_chardir);

	if(GetWinSerif(katiserifu,enemyname,filepath)){
		funcs->setkatiserif(pdat->tid,katiserifu);
	}
	else funcs->setkatiserif(pdat->tid,"�Z���t�̎擾�Ɏ��s");
}

BOOL GetWinSerif(char *getmsg,char *enename,char *filepath)
{
	sprintf(getmsg,"(�䎌�̐ݒ肪��������܂���ł���)");

	//�t�@�C���̃I�[�v��
	HANDLE hFile = CreateFile(filepath,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE)return(FALSE);

	//�̈�m��
	DWORD fsiz = GetFileSize(hFile,NULL);
	char *buf = (char*)malloc(fsiz);
	ZeroMemory(buf,sizeof(buf));

	//�t�@�C���̓ǂݍ���
	DWORD br;
	ReadFile(hFile,buf,fsiz,&br,NULL);
	CloseHandle(hFile);

	//���̃L�����N�^�[�Ɋ���U��ꂽ�䎌���������邩������
	DWORD numser=0,numserr=0,pnow=0;
	BOOL loop=TRUE,loop2;
	char tmpname[64];
	DWORD tmpnamelen;
	DWORD sstartpos[16],sstartposr[16];
	while(pnow < fsiz && loop){
		//�u#�v������
		loop2=TRUE;
		while(pnow < fsiz && loop2){
			//�s���i�܂��̓t�@�C���擪�j�̂�
			if(pnow==0){
				if(buf[pnow]=='#'){
					loop2=FALSE;
				}
			}
			else if(pnow>1){
				if(buf[pnow-2]==0x0d && buf[pnow-1]==0x0a){
					if(buf[pnow]=='#'){
						loop2=FALSE;
					}
				}
			}
			pnow++;
		}
		if(!loop2){//#����������Ă���Ȃ��
			//���̌�̉��s�܂ł𖼑O�Ƃ��ċL�^
			tmpnamelen=0;
			ZeroMemory(tmpname,sizeof(tmpname));
			loop2=TRUE;
			while(pnow < fsiz && loop2){
				tmpname[tmpnamelen] = buf[pnow];
				tmpnamelen++;
				if(pnow < fsiz-2){//���s����
					if(buf[pnow+1]==0x0d && buf[pnow+2]==0x0a){//���̎��͉��s������
						loop2=FALSE;
						pnow+=3;
					}
					else pnow++;//���̎������s�ł͂Ȃ�
				}
				else pnow++;//�t�@�C���̏I���̂ق��Ȃ̂ŉ��s���肪�ł��Ȃ�
			}
			//���O���r
			if(strcmp(tmpname,"random")==0 && numserr<16){//�����_���䎌
				sstartposr[numserr]=pnow;
				numserr++;
			}
			if(strcmp(tmpname,enename)==0 && numser<16){//�L�����ŗL�䎌
				sstartpos[numser]=pnow;
				numser++;
			}
		}
	}

	if(numser==0 && numserr==0)return(FALSE);//����䎌��������Ȃ�����

	//���������䎌�̂����A�����_����1�̑䎌��I��
	if(numser != 0)pnow = sstartpos[GetRandNum(numser)];
	else pnow = sstartposr[GetRandNum(numserr)];//�����_���䎌


	//���̌�̉��s�܂ł������䎌�Ƃ��ăR�s�[
	int tmpcnt=0;
	DWORD retlen=0;
	while(pnow < fsiz){
		getmsg[retlen] = buf[pnow];
		retlen++;
		if(pnow < fsiz-2){//���s����
			if(buf[pnow+1]==0x0d && buf[pnow+2]==0x0a){//���̎��͉��s������
				if(pnow == fsiz-2){//�����̔����ǉ�
					getmsg[retlen] = 0;
					free(buf);
					return(TRUE);
				}//�����܂�
				if(buf[pnow+3]=='#' || tmpcnt>=2){//���b�Z�[�W�ɗL���ȉ��s���H
					getmsg[retlen] = 0;
					free(buf);
					return(TRUE);
				}
				else{//���s�͂Q�܂Ńo�b�t�@����
					getmsg[retlen++] = buf[++pnow];
					getmsg[retlen++] = buf[++pnow];
					tmpcnt++;
				}
			}
		}
		if(pnow < fsiz-1){
			if(buf[pnow+1]=='#'){
				getmsg[retlen] = 0;
				free(buf);
				return(TRUE);
			}
		}
		pnow++;
	}
	getmsg[retlen] = 0;
	free(buf);
	return(TRUE);

}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
		srand(GetTickCount());

	return TRUE;
}

inline int GetRandNum(int num)//�����_���i���o�[����
{
	return(rand()%num);
}

/* ********************************************************************************

�@CBulletBase : �򓹋��{�N���X

�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ ..,,,,,,..........................�@�@�@�@�\�@�@�@-�@�@�\�@�@�@(��;:)
�]�@�@�@�@�@�@�@�@�@�@ �@�@/'" �@�@�@�@�@�@�@�@ �@�@�@�@�@�@ �@"'�R.�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�|�@�@�\
�@�@�@�\�@�@ �@�@�@�@�@ /"�@�@�@�@�@| �@ �@�@�@ /�@�@�@ �@�@�@�@�@"�R,�i'';:�܁܁R��,..
�@�@�@�@�@�@�@�@�@�@�@�@i�@�@�@���@�@|�P�P�P/�@�@���@�@�@�@�@�@�@ �ց@ �@�@�@�@�@�@;;:: �l�@�@�@(��::;
�@�@�]�@�] �@ -�@�@ �@ �@|�@ �@�@�@�@ i �@�@..::/�@�@�@�@�@�B�@�@�@;:::�^�@ :: ::�@:�@::�܁P""�܁S
�@�@�@�@�@�@�@�@�@�@�@ �@!.�@ �@�@�@�@|�@:::::/|�@�Q _�B�@�@::::::::: /:::�@;�@":�P�@�@:::�@�@:::::::::::)
�@�@-�@�@�@�@�@�\�@�@�@�R�@�@�@ �@�@�@ l�R__ɘ��@�@�@,,..:;::::::::::�^:'�i: : :�@;;:;:�@�@�@�@�@�@��:;;��)
�@�@�@�@�@�@�@�@�@�@�@�@ �@�_�@�@�@�@�@ ���Q/ .......::::::;;:;::::::�^;:/:::: : :�m;;::�@ :;�@:�@�@....:::::�@�@�@..... �@::::�
-�@�@ �@ �@ �@�@�@ �@�@�@ �@�@�P�P�P�P�P�P�P�P'�P""�P" "�@�P�P�P"'"" �P�P�P"�@''''�P''�P

 ******************************************************************************** */

CBulletBase::CBulletBase(GOBJECT *parent/* = NULL */)
{
	parent_obj = parent;
	parent_class = NULL;
	m_isForNetwork = FALSE;

	CBulletBaseCreate();
}

CBulletBase::CBulletBase(CCharacterBase *parent) : CGoluahObject()
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

	m_isForNetwork = FALSE;
	CBulletBaseCreate();
}

CBulletBase::CBulletBase(CCharacterBase *parent,CDI_CHARACTERINFO2 *info,BYTE userID,BYTE userNo)
				: CGoluahObject(info,userID,userNo)
{
	if(parent!=NULL){
		parent_class = parent;
		parent_obj = parent->GetGObject();
		g_chardir = info->dir;
	}
	else {
		parent_class=NULL;
		parent_obj=NULL;
	}
	m_isForNetwork = TRUE;
	CBulletBaseCreate();
}

void CBulletBase::CBulletBaseCreate()
{
	GOBJECT* pdat = GetGObject();

	if(parent_obj!=NULL){
		pdat->tid = parent_obj->tid;//�`�[��ID
		pdat->pmsarr = parent_obj->pmsarr;//gcd�͐e�Ɠ������̂��g����悤�ɂ��Ă���
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
}

DWORD CBulletBase::Message(DWORD msg,LPVOID pd,DWORD prm)
{
	switch(msg){
	case GOBJMSG_SOUSAI:return( Sousai(prm) );
	case GOBJMSG_CNGROUND:Suicide();return(TRUE);
	default:
		//���Ƃ̃��b�Z�[�W�̓I�u�W�F�N�g��{�N���X�ɔC����
		return(CGoluahObject::Message(msg,pd,prm));
	}
	return(0);
}

DWORD CBulletBase::Draw()//idle���͕`�悵�Ȃ�
{
	if(GetGObject()->aid==CBB_STATE_IDLE)return TRUE;
	return 0;
}

DWORD CBulletBase::TouchB(ATTACKINFO *info,BOOL hit)
{
	Hit();
	if(parent_class){
		if(hit && hitmsg!=0){
			parent_class->Message(hitmsg,parent_obj,hitprm);
		}
		else if(grdprm!=0){
			parent_class->Message(grdmsg,parent_obj,grdprm);
		}
	}
	return(TRUE);
}

DWORD CBulletBase::Sousai(DWORD prm)
{
	GetGObject()->kougeki=FALSE;//�U���͖�����
	GetGObject()->aid = CBB_STATE_BOMB;
	return(TRUE);
}

void CBulletBase::Suicide()
{
	if(m_isForNetwork){
		Hide();
		return;
	}
	funco->suicide(GetGObject()->id);
}

//�T�E���h�Đ� �����e������΁A������̂ق��ɏ������ۓ���
void CBulletBase::PlayMySound(DWORD number)
{
	if(parent_class!=NULL)parent_class->PlayMySound(number);
}

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

void CBulletBase::Bomb()
{
	ChangeAction(CBB_STATE_BOMB);
}

void CBulletBase::Hide()
{
	ChangeAction(CBB_STATE_IDLE);
}

void CBulletBase::act_idle()
{
	GOBJECT* pdat = GetGObject();

	if(GetGObject()->counter==0 && IsNetwork()){
		(*funcn->set_syncronize)(pdat->id,FALSE);
	}

	pdat->x = 99999;
	pdat->y = 99999;
	pdat->kougeki = FALSE;
}

/* ********************************************************************************

�@CClassicalBullet : �����򓹋�N���X

 ******************************************************************************** */

CClassicalBullet::CClassicalBullet( CCharacterBase *parent,CDI_CHARACTERINFO2 *info,BULLETINFO_A *bulinfo,
								   BYTE userID,BYTE userNo) : CBulletBase(parent,info,userID,userNo)
{
	array_len_r = 0;
	array_len_d = 0;

	//�z��̒������擾����
	while(bulinfo->cell_run[array_len_r]>0)array_len_r++;
	while(bulinfo->cell_dis[array_len_d]>0)array_len_d++;

	//������m��&�z����R�s�[����
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

	//���U�����z�񂩂�V�U�����z��ɕϊ�����
	atkinfo.damage = bulinfo->atk.damage;
	atkinfo.kezuri = bulinfo->atk.kezuri;
	atkinfo.guard = bulinfo->atk.guard;
	atkinfo.hit = bulinfo->atk.hit;
	atkinfo.muki = FALSE;//���\���̂Ƃ͂�����ƈӖ����Ⴄ

	//�p�����[�^�R�s�[
	ax = bulinfo->ax;
	ay = bulinfo->ay;
	vx = bulinfo->vx;
	vy = bulinfo->vy;
	spd_r = bulinfo->spd_run;
	spd_d = bulinfo->spd_dis;
	lifedur = bulinfo->dur;
	flags = bulinfo->type;

	pState = &bulinfo->state;

	//�t���O���� 
	if( !(flags&BULLETA_VSHUMAN) )RemoveProperty(GOBJFLG_ATTACK);
	if( !(flags&BULLETA_VSBULLET) )RemoveProperty(GOBJFLG_ZBULLET);
	if( flags&BULLETA_DRAWBACK )base_z=ZZAHYO_BULLET3;
	if( flags&BULLETA_DRAWMIDDLE )base_z=ZZAHYO_BULLET2;
	if( flags&BULLETA_DISPZAHYO )AddProperty(GOBJFLG_DISPZAHYO);
	if( flags&BULLETA_DONOTSTOP )AddProperty(GOBJFLG_DONOTSTOP);
}

CClassicalBullet::~CClassicalBullet()
{
	if(pcr)delete [] pcr;
	if(pcd)delete [] pcd;
}


BOOL CClassicalBullet::Go(BOOL muki,double x,double y)
{	
	BOOL result = CBulletBase::Go(muki,x,y,vx,vy);

	if (result)
		*pState = 1;
	return result;
}

void CClassicalBullet::Hit()
{
	if(flags&BULLETA_DONOTDIE)ChangeAction(CBB_STATE_RUN2);
	else ChangeAction(CBB_STATE_BOMB);
}

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

void CClassicalBullet::act_run2()
{
	act_run();
	GetGObject()->kougeki=FALSE;
}

void CClassicalBullet::act_bomb()
{
	GetGObject()->kougeki = FALSE;

	if(spd_d==0 || pcd==NULL)return;
	DWORD index = GetGObject()->counter / spd_d;
	if(index >= array_len_d || array_len_d==0){
		*pState = 0;
		Hide();
		return;
	}
	GetGObject()->cnow = pcd[index];
}

/* **************************************************************************

  CBulletList : ��ѓ���X�g�N���X

�@�l�b�g�ΐ�p�̔�ѓ���I�u�W�F�N�g�����X�g�Ǘ����邱�Ƃ��ł��܂��B
�@��x�ɑ�ʔ��˂����ѓ����A�A�˂������Ƃ��Ɏg�p���Ă��������B
�@�ӂ��̔�ѓ���ł��A�����ɏ�Q���o��ꍇ�Ɏg�p����ƕ֗��ł��B
�@CGoluahObject�̔h���N���X�ł͂Ȃ��̂ŁA���I��������K�v�͂���܂���B
�@�i�r���Ń��X�g���N���A���邱�Ƃ͂ł��܂���̂ł����ӂ��������B�j

 ************************************************************************** */

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

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

CBulletList::CBulletList(CBulletBase *pBullet)
{
	if (pBullet && pBullet->IsForNetwork()){
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

CBulletList::~CBulletList()
{
	if (pNext)
		delete pNext;
}

void CBulletList::Add(CBulletBase *pBullet)
{
	if (pBullet && pBullet->IsForNetwork())
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

void CBulletList::Go_All(BOOL muki, double x, double y)
{
	if (m_pBullet)
		m_pBullet->Go(muki, x, y, vx, vy);

	if (pNext)
		pNext->Go_All(muki, x, y);
}

void CBulletList::Go_All(BOOL muki)
{
	if (m_pBullet)
		m_pBullet->Go(muki, x, y, vx, vy);

	if (pNext)
		pNext->Go_All(muki);
}
