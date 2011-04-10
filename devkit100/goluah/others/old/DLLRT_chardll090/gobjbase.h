// Goluah �L�����N�^�[DLL�p�����^�C��
#ifndef CDI_VERSION
#include "gobject.h"
#endif

#ifdef CRUNT090_EXPORTS
#define RUNTIME_EXPORT __declspec(dllexport)
#else
#pragma comment(lib, "crunt090.lib")
#define RUNTIME_EXPORT
#endif // CRUNT090_EXPORTS

#ifndef _D3D8_H_
typedef LPVOID LPDIRECT3D8;
typedef LPVOID LPDIRECT3DDEVICE8;
#endif // _D3D8_H_
#ifndef __D3DX8MATH_H__
typedef LPVOID LPD3DXMATRIX;
#endif
#if CDI_VERSION < 900
struct CHARACTER_LOAD_OPTION{};
#endif
#if GCD_VERSION < 900
struct GCD_CELL2_070 : public GCD_CELL2 {};
#endif

/********************************************************
  �L�����N�^�[���ݒ�N���X
  �l�b�g�Ή��F
	�R���X�g���N�^�U�Ԗڂ̈�����TRUE�ɐݒ肵�Ă��������B
�@********************************************************/

class RUNTIME_EXPORT CCharacterInfo  
{
public:
	CCharacterInfo(LPCTSTR CharName, DWORD CdiVersion, CHARACTER_LOAD_OPTION* Options = NULL,
		int num_Options = 0, DWORD max_option_point = 0, BOOL isNetworkable = FALSE);
	virtual ~CCharacterInfo();

	BOOL DllCharacterInfo(LPVOID info);
	void DllCreateCharacter(PVOID info);

private:
	void SetOptionItems(CDI_CHARACTERINFO *pif);
	BOOL m_isNetworkable;
	DWORD version;
	LPTSTR charactername;
	CHARACTER_LOAD_OPTION* m_Options;
	int m_NumOptions;
	DWORD m_MaxPoint;
};

// �ݒ�p�}�N��
#define SET_CHARACTERINFO(cinfo, Type) \
extern "C" BOOL CharacterInfo(LPVOID info) {\
	return cinfo.DllCharacterInfo(info);\
}\
extern "C" DWORD CreateCharacter(PVOID info) {\
	cinfo.DllCreateCharacter(info);\
	Type* pc = new Type((CDI_CHARACTERINFO2*)info);\
	return(pc->GetObjectID());\
}

/********************************************************
  Goluah�I�u�W�F�N�g��{�N���X
  �l�b�g�Ή��F
	��������ق��̃R���X�g���N�^�ō\�z���Ă�������
	�f�t�H���g�ł́A�I�u�W�F�N�gID 0xXXXXFFFF���牺���č쐬����̂�
	���[�U�[�h�c��0xF*������͎w�肵�Ȃ��ł�������
�@********************************************************/

class RUNTIME_EXPORT CGoluahObject
{
public:
	CGoluahObject();
	CGoluahObject(CDI_CHARACTERINFO2 *info,BYTE userID = 0xFF,BYTE userNo = 0xFF);
	virtual ~CGoluahObject();
	DWORD GetObjectID();
	GOBJECT* GetGObject();
	char* GetCharDir() { return g_chardir; }
	static DWORD gMessageToObject(DWORD msg,LPVOID pd,DWORD prm);
	virtual DWORD Message(DWORD msg,LPVOID pd,DWORD prm);
private:
	void CGoluahObjectCreate();//��l�b�g���[�N����������

protected:
	virtual DWORD Action();//GOBJMSG_ACTION�ɑΉ�
	virtual void Command();//GOBJMSG_COMMAND�ɑΉ�
	virtual DWORD CommandCOM(DWORD wid);//GOBJMSG_COMMANDCOM�ɑΉ�
	virtual DWORD TouchA(ATTACKINFO *info,DWORD ta_eid);//GOBJMSG_TOUCHA�ɑΉ�
	virtual DWORD TouchB(ATTACKINFO *info,BOOL hit);//GOBJMSG_TOUCHB�ɑΉ�
	virtual DWORD TouchC(ATTACKINFO *info,DWORD tc_eid);//GOBJMSG_TOUCHC�ɑΉ�
	virtual DWORD Draw();//GOBJMSG_DRAW�ɑΉ�
	virtual DWORD DrawBack();//GOBJMSG_DRAWBACK�ɑΉ�
	virtual DWORD DrawFront();//GOBJMSG_DRAWFRONT�ɑΉ�
	virtual void ActionIDChanged();//GOBJMSG_CNGAID�ɑΉ�

	double zurex(double x);
	int zurex(int x);
	void movex(double dx);
	void movex(int dx);

	void AddProperty(DWORD prop);
	void RemoveProperty(DWORD prop);
	void SetProperty(DWORD prop);

	void ChangeAction(DWORD actid);

	BOOL IsRemote(){return m_remote;}
	BOOL IsNetwork(){return m_network;}

	// �V�X�e���֘A�̊֐�
	static DWORD GetKey(DWORD keyinput, DWORD interval);
	static int SeekKey(DWORD keyinput, int offset, int delay, DWORD keystate);
	static void SetKatiSerif(DWORD tid, char* serif);
	static void AddEffect(DWORD EffectID, int prm1, int prm2, DWORD prm3);
	static void PlaySysSound(DWORD SoundNo);
	static LPVOID LoadMySound(char* pathname);
	static void PlayMySound(LPVOID psound);
	static void KillMySound(LPVOID psound);
	DWORD Message2System(DWORD mes, DWORD prm);
	static char* GetEnemyName(DWORD tid);
//	static char* GetCharacterName(DWORD oid);
	static double GetDisplayCenter_X();
	static double GetDisplayCenter_Y();
	static DWORD GetTeamNum();
	static DWORD GetTaisenKeisiki();

	// �I�u�W�F�N�g�֘A�̊֐�
	BOOL   ObjCatch(DWORD eid,DWORD msg_nage);			//�����Ƃ��ő��������
	static GOBJECT* GetInfo(DWORD oid);					//���Q�b�c
	void   AddDamage(DWORD eid,int x,int y);			//(��ɓ����̂Ƃ�)�����I�Ƀ_���[�W��^����
	static GOBJECT*  GetActiveCharacter(DWORD tid);		//���݃A�N�e�B�u�ȃL�����N�^�[�̏��
	void   Suicide();									//�I�u�W�F�N�g����
	static DWORD  GetMaai_H(DWORD oid, DWORD eoid);		//�ԍ�����get(����)
	static DWORD  GetMaai_V(DWORD oid, DWORD eoid);		//�ԍ�����get(����)
	static DWORD  GetCharacterID(DWORD tid,DWORD index);//�L�����N�^�[�̃I�u�W�F�N�gID

	//�@�`��֘A�̊֐��̒�`
	static LPDIRECT3D8 GetD3D();							//IDirect3D* �̎擾
	static LPDIRECT3DDEVICE8 GetD3DDevice();				//IDirect3DDevice* �̎擾
	static void   LoadCellData(char* pathname,GCD_CELL2_070* pCells,GCD_RECT* pRects,GCD_HANTEI* pHanteis);//�Z���f�[�^�ǂݍ��݊֐�
	static MYSURFACE* LoadBitmap(char* PathName,char* PalletFileName);				//�r�b�g�}�b�v�ǂݍ��݊֐�
	static void   UnloadBitMap(MYSURFACE* Bitmap);			//�r�b�g�}�b�v��n���֐�
	static void   CellDraw(MYSURFACE** pBmps,GCD_CELL2* cdat,GCD_RECT* rdat,		//�Z���`��֐�
		DWORD cnum,int x,int y,float z,int Rotate,BOOL ReverseX,BOOL ReverseY,DWORD Color,float magx,float magy);
	static void   CkBlt(MYSURFACE* pBmp,int x1,int y1,RECT bltrect,
		double magx,double magy,BOOL revx,BOOL revy,float z,DWORD color);			//Blt2
	static void   Blt3D(MYSURFACE* pBmp,RECT bltrect,MYRECT3D rect,DWORD color);	//Blt3
	static void   SetTransform(BOOL BonVoyage);				//�ϊ��s��ݒ�
	static void   SetBlend(DWORD type);						//�u�����h�̎d����ύX
	static void   SetParentMatrix(LPD3DXMATRIX pMatrix,BOOL erase,LPD3DXMATRIX OldMatrix);	//�u�e�v�̕ϊ��s��ݒ�
	static DWORD  CreateCellData(char* filename,GCD_CELL2 **cdat,GCD_RECT **rdat,GCD_HANTEI **hdat);//�Z���f�[�^�ǂݍ��݁Bver0.90�ȍ~�ł�LoadCellDat���炱����ɕύX����ׂ�
	static void   DestroyCellData(GCD_CELL2 **cdat,GCD_RECT **rdat,GCD_HANTEI **hdat);		//CreateCellDat�Ő������ꂽ�o�b�t�@���N���A

	//�@�l�b�g���[�N�֘A�̊֐��̒�`
	void SendSynchronizedMessage(DWORD size,LPVOID dat);	// �l�b�g���[�N�������b�Z�[�W���M
	void SetSynchronize(BOOL enabled);						// �l�b�g���[�N���ʐM��ON/OFF

protected:
	float base_z;
	BYTE dllID;

	BOOL m_remote;
	BOOL m_network;
	char *g_chardir;

private:
	BOOL m_hasDefaultID;
	DWORD oid;
	GOBJECT *pdat;

	static unsigned short gCreateCounter;
	static int m_ObjCount;
	static CGoluahObject* pObjToDelete;
	static CGoluahObject* pObjDeleting;
};

inline DWORD CGoluahObject::GetObjectID(){return(oid);}
inline GOBJECT* CGoluahObject::GetGObject(){return(pdat);}

/**********************************************************************
�@�L�����N�^�[�I�u�W�F�N�g��{�N���X
�@�l�b�g�F
		�Q�Ԗڂ̃R���X�g���N�^���g���Ă��������B
  *********************************************************************/

#define CBASE_NUMMYSOUND	(32)//�f�t�H���g��32�Ԃ܂ł�wav��ǂݍ��݂܂�

class RUNTIME_EXPORT CCharacterBase : public CGoluahObject
{
public:
	CCharacterBase(CCharacterBase *parent = NULL);
	CCharacterBase(CDI_CHARACTERINFO2 *info,BYTE userID = 0xFF,BYTE userNo = 0xFF);
	virtual ~CCharacterBase();
	virtual DWORD Message(DWORD msg,LPVOID pd,DWORD prm);//�K�v������΃I�[�o�[���C�h���܂�
	DWORD TouchA(ATTACKINFO *info,DWORD ta_eid);

private:
	void CCharacterBaseCreate();//���ꂼ��̃R���X�g���N�^�ŋ��ʂ̏���������

protected:

	virtual DWORD Action();//�ʏ� �K���I�[�o�[���C�h���܂�
	virtual void PreAction();//�ʏ� �I�[�o�[���C�h����K�v�͂���܂���
	virtual void PostAction();//�ʏ� �I�[�o�[���C�h����K�v�͂���܂���
	virtual void Command();//�ʏ� �I�[�o�[���C�h����K�v�͂���܂���
	virtual DWORD CommandCOM(DWORD wid);//�ʏ� �I�[�o�[���C�h����K�v�͂���܂���

	void InitAll();
	void AddPowerGauge(double dp);

	virtual void InitAttackInfo();//�U���͏��̏����� �ʏ�K���I�[�o�[���C�h���Ďg�p���܂�
	virtual void InitWazInfo();//�Z���̐ݒ� �ʏ�K���I�[�o�[���C�h���Ďg�p���܂�
	virtual BOOL ChainCombo(DWORD chainid);//�`�F�[���R���{���� �K�v������΃I�[�o�[���C�h���܂�

	DWORD GetKeyInput()		{ return keyinput; }
	//�T�E���h�֌W�B���O�ł��ꍇ�̓I�[�o�[���[�h���Ă�������
	virtual void InitMySound();
	virtual void ReleaseMySound();

public:
	virtual void PlayMySound(DWORD number);

protected:
	//�r�b�g�}�b�v��CELL�̓ǂݍ��݁B���O�ł��ꍇ�̓I�[�o�[���[�h���Ă�������
	virtual void InitGCDandBMP();
	virtual void ReleaseGCDandBMP();

	//CCharacterBase�N���X���s���s���̒�`
	//�ʏ퓮��
	virtual void act_neutral()=0;//�j���[�g����
	virtual void act_crouch()=0;//���Ⴊ��
	virtual void act_rakka()=0;//����
	virtual void act_tojyo()=0;//�o��
	virtual void act_win()=0;//����
	virtual void act_walkf()=0;//�O�i
	virtual void act_walkb()=0;//���
	virtual void act_jamps()=0;//�W�����v�\������
	virtual void act_jampc()=0;//�����W�����v
	virtual void act_jampf()=0;//�O�W�����v
	virtual void act_jampb()=0;//��W�����v
	virtual void act_rakka2();//����
	//�ʏ�.�U��
	virtual void act_att_sa()=0;//����
	virtual void act_att_sb()=0;//����
	virtual void act_att_sc()=0;//����
	virtual void act_att_ca()=0;//����
	virtual void act_att_cb()=0;//����
	virtual void act_att_cc()=0;//����
	virtual void act_att_ja()=0;//���
	virtual void act_att_jb()=0;//��
	virtual void act_att_jc()=0;//��
	//���̑�
	virtual void act_taiki();
	virtual void act_taikicyu();
	virtual void act_koutai_out();
	virtual void act_koutai_in();
	virtual void act_koutai();
	virtual void act_koutai2();
	virtual void act_strikercomeon();

	//�ʏ�A�I�[�o�[���C�h����K�v�͂���܂���
	virtual BOOL Furimuki();//�G�̂ق��ɐU�������������܂�
	virtual void ZFront();//z���W����O���Ɉړ����܂�
	virtual void ZBack();//z���W�������Ɉړ����܂�
	virtual void JiyuuRakka(double acc_g,BOOL cyakuchi=FALSE,DWORD toaid=ACTID_NEUTRAL);
	virtual void CyakuchiHantei(DWORD toaid=ACTID_NEUTRAL);//���n�̔���
	virtual void LoadAndSetKatiSerif();

	// �R�}���h����B�ʏ� �K���I�[�o�[���C�h���܂�
	virtual BOOL Command_Hissatu(DWORD keyinfo);//�K�E�Z�R�}���h����
	virtual BOOL Command_Normal(DWORD keyinfo);//��{����
	virtual BOOL Command_OnSystem(DWORD keyinfo);//�V�X�e����`���쒆
	virtual BOOL Command_OnHissatuAttacking(DWORD keyinfo);//�K�E�Z��
	virtual BOOL Command_OnAttacking(DWORD keyinfo);//�U�����쒆
	virtual BOOL Command_OnNormal(DWORD keyinfo);//��{���쒆

	// �R���s���[�^�R�}���h����B�K�v������΃I�[�o�[���C�h���܂�
	virtual DWORD CmdCom_OnSystem(DWORD wid);//�V�X�e����`���쒆
	virtual DWORD CmdCom_OnHissatuAttacking(DWORD wid);//�K�E�Z��
	virtual DWORD CmdCom_OnKucyu(DWORD wid);//�󒆓��쒆
	virtual DWORD CmdCom_OnNormal(DWORD wid);//��{���쒆

	//���o�[�R�}���h����֐�
	BOOL com236(int dt);
	BOOL com623(int dt);
	BOOL com214(int dt);
	BOOL com421(int dt);
	BOOL com66(int dt);
	BOOL com44(int dt);
	BOOL com22(int dt);
	BOOL com426(int dt);
	BOOL com236236(int dt);
	BOOL com2363214(int dt);
	BOOL com62426(int dt);
	BOOL com6426(int dt);

	BOOL NageHantei(DWORD maai);//�����̊ԍ�������

	DWORD GetKey(DWORD interval)	{ return CGoluahObject::GetKey(keyinput, interval); }
	int SeekKey(int offset, int delay, DWORD keystate);

protected:
	BOOL chainComboEnabled;//�`�F�[���R���{��L���ɂ��邩�ǂ���
	DWORD chainlist;//�`�F�[���R���{�E���łɏo�����Z��ۑ�����ϐ�
	float front_z,back_z;//ZFront/Back���\�b�h�Ŏw�肳���Z���W
	BOOL doStriker;//�x���U������炩�����ǂ���
	WAZAINFO waz;//�Z���\����
	DWORD pal_number;//�V�X�e������w�肳�ꂽ�p���b�g�ԍ�
	CCharacterBase *parent_char;//�u�e�v�L�����N�^�[
	BOOL isSuperArmer;
	BOOL isAutoGuard;

private:
	MYSURFACE *bitmaps[GCDMAX_IMAGES];
	GCD_RECT *rects;
	GCD_CELL2 *cells;
	GCD_HANTEI *hantei;
	LPVOID mysounds[CBASE_NUMMYSOUND];
	DWORD keyinput;
};

inline int CCharacterBase::SeekKey(int offset, int delay, DWORD keystate) {
	return CGoluahObject::SeekKey(keyinput, offset, delay, keystate);
}

// �h���N���X�̍s��ID�ɂ́A���̃t���O���g�p���Ȃ�����
#define ACTID_CHARACTERBASE (0x8000)

#define ACTID_WALKF		(ACTID_CHARACTERBASE | 0x0005)//�O����
#define ACTID_WALKB		(ACTID_CHARACTERBASE | 0x0006)//������
#define ACTID_JAMPS		(ACTID_CHARACTERBASE | 0x0007)//�W�����v�\������
#define ACTID_JAMPC		(ACTID_CHARACTERBASE | ACTID_KUCYU | 0x0002)
#define ACTID_JAMPF		(ACTID_CHARACTERBASE | ACTID_KUCYU | 0x0003)
#define ACTID_JAMPB		(ACTID_CHARACTERBASE | ACTID_KUCYU | 0x0004)
#define ACTID_RAKKA2	(ACTID_CHARACTERBASE | ACTID_KUCYU | 0x0005)

#define ACTID_STRIKERCOMEON	(ACTID_CHARACTERBASE | 0x000B)//�����[

#define ACTID_ATT_SA	(ACTID_CHARACTERBASE | ACTID_ATTACK | 0x0001)
#define ACTID_ATT_SB	(ACTID_CHARACTERBASE | ACTID_ATTACK | 0x0002)
#define ACTID_ATT_SC	(ACTID_CHARACTERBASE | ACTID_ATTACK | 0x0003)

#define ACTID_ATT_CA	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_SYAGAMI | 0x0001)
#define ACTID_ATT_CB	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_SYAGAMI | 0x0002)
#define ACTID_ATT_CC	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_SYAGAMI | 0x0003)

#define ACTID_ATT_JA	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_KUCYU | 0x0001)
#define ACTID_ATT_JB	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_KUCYU | 0x0002)
#define ACTID_ATT_JC	(ACTID_CHARACTERBASE | ACTID_ATTACK | ACTID_KUCYU | 0x0003)

#define ACTID_KOUTAIOUT	(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0001)//���B��ʊO�ɔ��ł��B
#define ACTID_KOUTAIIN	(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0002)//���B��ʓ��ɔ��ł���
#define ACTID_TAIKI		(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0003)//��ʊO�ɔ��ł��đҋ@
#define ACTID_KOUTAI	(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0004)//��ʊO������ł���
#define ACTID_TAIKICYU	(ACTID_CHARACTERBASE | ACTID_INOUT | 0x0009)//�ҋ@����\
#define ACTID_KOUTAI2	(ACTID_CHARACTERBASE | ACTID_INOUT | 0x000A)//����A�|�[�Y�L��

//chain combo
#define CHAIN_SA	0x00010000
#define CHAIN_SB	0x00020000
#define CHAIN_SC	0x00040000
#define CHAIN_CA	0x00080000
#define CHAIN_CB	0x00100000
#define CHAIN_CC	0x00200000

#define ADDEFFECT(a,b,c,d)	funcs->addeffect(a,b,c,d)


/***************************************************************
�@��ѓ����{�N���X
�@�Ȃ񂩂��܂����g�����舫����

�@�ɗ͎����r���Ő������Ȃ��ł��������i����ꍇ�̓l�b�g��Ή��ƂȂ�܂��j

  �l�b�g�F�R���X�g���N�^(3)�ō\�z���܂��B
****************************************************************/

//�s��ID�̒�`
#define CBB_STATE_FLAG		0x00008000
#define CBB_STATE_IDLE		(CBB_STATE_FLAG | 0x00000100)//act_idle
#define CBB_STATE_RUN		(CBB_STATE_FLAG | 0x00000200)//act_run
#define CBB_STATE_BOMB		(CBB_STATE_FLAG | 0x00000300)//act_bomb
#define CBB_STATE_RUN2		(CBB_STATE_FLAG | 0x00000400)//act_run2

class RUNTIME_EXPORT CBulletBase : public CGoluahObject
{
public:
	CBulletBase(GOBJECT *parent = NULL);		//�R���X�g���N�^(1)
	CBulletBase(CCharacterBase *parent);		//�R���X�g���N�^(2) ����
	CBulletBase(CCharacterBase *parent,CDI_CHARACTERINFO2 *info,
		BYTE userID = 0xFF,BYTE userNo = 0xFF);	//�R���X�g���N�^(3) �l�b�g���[�N�p

	virtual ~CBulletBase(){}

	virtual BOOL Go(BOOL muki,double x,double y,double vx,double vy=0);
	virtual void Bomb();//���œ���ɋ����ڍs���܂�
	virtual void Hide();//�����I�ɑҋ@��Ԃɂ��܂�
	void SetPos(double x,double y){GetGObject()->x=x;GetGObject()->y=y;}
	virtual DWORD Draw();

	void SetHitMessage(DWORD msgid,DWORD param){hitmsg=msgid;hitprm=param;}
	void SetGuardMessage(DWORD msgid,DWORD param){grdmsg=msgid;grdprm=param;}
	BOOL isReady(){return(GetGObject()->aid==CBB_STATE_IDLE);}

	GOBJECT* GetParentObj()				{ return parent_obj; }
	CCharacterBase* GetParentClass()	{ return parent_class; }
	BOOL IsForNetwork()					{ return m_isForNetwork; }

private:
	void CBulletBaseCreate();//���ʏ���������

protected:
	//�K�v������΃I�[�o�[���C�h���܂�
	virtual DWORD Message(DWORD msg,LPVOID pd,DWORD prm);
	virtual DWORD Action();
	virtual DWORD TouchB(ATTACKINFO *info,BOOL hit);
	virtual DWORD Sousai(DWORD prm);//���򓹋�Ƒ��E�����Ƃ��ɌĂ΂�܂�
	virtual void Hit(){}//����Ƀq�b�g�����Ƃ��Ă΂�܂�(TouchB)

	//�s���֐��ł��B�I�[�o�[���C�h���Ďg���܂�
	virtual void act_idle();
	virtual void act_run()=0;
	virtual void act_bomb()=0;
	virtual void act_run2(){}

	//�I�[�o�[���C�h�̕K�v�͑����Ȃ�
	void Suicide();

	//�T�E���h�Đ��B�e�̃T�E���h�@�\�𗬗p���܂�
	//�R���X�g���N�^(2)�ō쐬�����ꍇ�̂ݎg�p�\�ł�
	virtual void PlayMySound(DWORD number);

protected:
	ATTACKINFO atkinfo;//�U���͏��\����

	DWORD hitmsg,hitprm;
	DWORD grdmsg,grdprm;

private:
	BOOL m_isForNetwork;
	GOBJECT *parent_obj;
	CCharacterBase *parent_class;
};


/***************************************************************
�@������ѓ���N���X
�@�́X�̃\�[�X���ė��p���邽�߂ɍ���Ă݂�

�@�l�b�g�Ή��F�l�b�g�̂�
****************************************************************/

struct MYATTACKINFO//���U���͏��\����
{
	DWORD hit;		//�q�b�g���B���̃t���O(HITINFO_�`)�̘_���a���w��
	DWORD guard;	//�K�[�h���B���̃t���O(GUARDINFO_�`)�̘_���a���w��
	DWORD damage;	//�_���[�W
	DWORD kezuri;	//���

	//�ȉ��g���Ă��܂���B�_�~�[�ł�
	DWORD objid;	//�U�����s�����I�u�W�F�N�g��ID
	DWORD id;		//�L�����N�^���ŏ���ɒ�`����
	double pg_hit;	//�U���q�b�g���̃Q�[�W������
	double pg_guard;//�U���K�[�h���ꂽ�Ƃ��̃Q�[�W������
	BOOL muki;		//���肪�̂��������
	DWORD dur;
};

struct BULLETINFO_A//���򓹋�\����
{
	MYATTACKINFO atk;
	int dur;//��������
	DWORD type;//���ɂ���t���O���w��
	int *cell_run;
	int *cell_dis;
	DWORD spd_run;
	DWORD spd_dis;
	double vx, vy;
	double ax, ay;

	//�ȉ��A�g���Ă܂���B�_�~�[�ł��B
	BOOL muki;
	double x;
	double y;
	DWORD state;
	DWORD cid;
	LPVOID info_b;
};

class RUNTIME_EXPORT CClassicalBullet : public CBulletBase
{
public:
	CClassicalBullet( CCharacterBase *parent,CDI_CHARACTERINFO2 *info,BULLETINFO_A *bulinfo,
							BYTE userID = 0xFF,BYTE userNo = 0xFF);
	virtual ~CClassicalBullet();
	
	BOOL Go(BOOL muki,double x,double y);
	virtual void Hit();

protected:
	virtual void act_run();
	virtual void act_run2();
	virtual void act_bomb();

private:
	DWORD* pState;
	void act_run_base();

	double vx,vy,ax,ay;
	int *pcr;
	int *pcd;
	DWORD array_len_r;
	DWORD array_len_d;
	DWORD spd_r;
	DWORD spd_d;
	DWORD lifedur;
	DWORD flags;
};

//�����ƃT�|�[�g����Ă邩�ǂ����s��
#define BULLETA_VSHUMAN		0x00000001//�G�L�����N�^�[�ɓ�����
#define BULLETA_VSBULLET	0x00000002//�G��ѓ���Ƒ��E����
#define BULLETA_DONOTDIE	0x00000004//�Փˌ�����ł��Ȃ��Ŕ��ł���
#define BULLETA_DRAWBACK	0x00000008//�l���������ɕ`�悷��
#define BULLETA_DRAWMIDDLE	0x00000040//�L�����N�^�[�E�q�b�g�}�[�N�Ԃɕ`��
#define BULLETA_XJIMENN		0x00010000//�n�ʂɗ���������Ə��ł���
//#define BULLETA_XGAMENGAI	0x00020000//��ʊO�ɒB����Ə���
//#define BULLETA_JIMAESYORI	0x10000000//info_b�����o���L���ł���A���������O�ōs��
#define BULLETA_DISPZAHYO	0x00000010//���W�w��̓f�B�X�v���C��̍��W�B�X�e�[�W��̕\���ʒu�ɍ��E����Ȃ�
#define BULLETA_DONOTSTOP	0x00000020//���K����stop�G�t�F�N�g���������Ă��Ă��`�悪�~�܂�Ȃ�

/********************************************************
  ��ѓ���X�g�N���X
  �l�b�g�Ή��F�l�b�g�̂�
�@********************************************************/

class RUNTIME_EXPORT CBulletList  
{
public:
	CBulletList();
	CBulletList(CBulletBase* pBullet);
	virtual ~CBulletList();

	virtual void Add(CBulletBase* pBullet);
	void SetPos(double x, double y)			{ this->x = x; this->y = y; }
	void SetSpeed(double vx, double vy=0)	{ this->vx = vx; this->vy = vy; }
	virtual BOOL Go(BOOL muki);
	virtual BOOL Go(BOOL muki, double x, double y);
	virtual BOOL Go(BOOL muki, double x, double y, double vx, double vy=0);
	virtual void Go_All(BOOL muki, double x, double y);
	virtual void Go_All(BOOL muki);
	CBulletList* GetNext()		{ return pNext; }
	int GetCount()	{ return ListCount; }

protected:
	CBulletBase* GetBullet()	{ return m_pBullet; }

private:
	double x;
	double y;
	double vx;
	double vy;
	int ListCount;
	CBulletBase* m_pBullet;
	CBulletList* pNext;
};

