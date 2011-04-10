
// ���o�[�W�����ƐV�o�[�W�����̏�����
void Exchange(GOBJECT* pNew, CHARACTERINFO2* pOld, BOOL isOldToNew);

//***************************************************
//�@�p�~���ꂽ�֐��̑���
//***************************************************

void Furimuki(DWORD oid);
void InitializeMySound();
void PlayMySound(DWORD soundno);
void LoadAndSetKatiSerif(DWORD tid);
void UnloadMySound();
void DrawFront(GOBJECT *pdat,BOOL fr);
void AddPowerGauge(GOBJECT *pdat,double plus);
void InitializeGCDandBMP(GOBJECT *pdat,DWORD color);
void UnloadMyBitmaps(GOBJECT *pdat);

#define MAXMYSOUND 32//sound31.wav�܂ł�ǂ݂���
/*
struct MYATTACKINFO
{
	DWORD hit;		//�q�b�g���B���̃t���O(HITINFO_�`)�̘_���a���w��
	DWORD guard;	//�K�[�h���B���̃t���O(GUARDINFO_�`)�̘_���a���w��
	DWORD objid;	//�U�����s�����I�u�W�F�N�g��ID
	DWORD id;		//�L�����N�^���ŏ���ɒ�`����
	double pg_hit;	//�U���q�b�g���̃Q�[�W������
	double pg_guard;//�U���K�[�h���ꂽ�Ƃ��̃Q�[�W������
	DWORD damage;	//�_���[�W
	DWORD kezuri;	//���
	BOOL muki;		//���肪�̂��������
	//��ATTACKINFO�ƑS������ ���p�~���ꂽ�����o
	DWORD dur;
};*/
void StartAttack(DWORD oid,MYATTACKINFO *ainfo);

ATTACKINFO* ResisterAttackInfo(DWORD oid,MYATTACKINFO *mainfo);
void GetMyAttackInfo(DWORD oid, const ATTACKINFO *ainfo,
							 MYATTACKINFO* mainfo, BOOL IsMyObject);

//***************************************************
//  ������ѓ���
//***************************************************
/*
struct BULLETINFO_A
{
	double x;
	double y;
	BOOL muki;
	MYATTACKINFO atk;
	int dur;//��������

	DWORD type;//���ɂ���t���O���w��
	int *cell_run;
	int *cell_dis;
	DWORD spd_run;
	DWORD spd_dis;
	double vx;
	double vy;
	double ax;
	double ay;

	DWORD state;//��ԁB������0���Ə���
	DWORD cid;

	LPVOID info_b;
};
*/
#define BULLETA_VSHUMAN		0x00000001//�G�L�����N�^�[�ɓ�����
#define BULLETA_VSBULLET	0x00000002//�G��ѓ���Ƒ��E����
#define BULLETA_DONOTDIE	0x00000004//�Փˌ�����ł��Ȃ��Ŕ��ł���
#define BULLETA_DRAWBACK	0x00000008//�l���������ɕ`�悷��
#define BULLETA_DRAWMIDDLE	0x00000040//�L�����N�^�[�E�q�b�g�}�[�N�Ԃɕ`��
#define BULLETA_XJIMENN		0x00010000//�n�ʂɗ���������Ə��ł���
#define BULLETA_XGAMENGAI	0x00020000//��ʊO�ɒB����Ə���
#define BULLETA_JIMAESYORI	0x10000000//info_b�����o���L���ł���A���������O�ōs��
#define BULLETA_DISPZAHYO	0x00000010//���W�w��̓f�B�X�v���C��̍��W�B�X�e�[�W��̕\���ʒu�ɍ��E����Ȃ�
#define BULLETA_DONOTSTOP	0x00000020//���K����stop�G�t�F�N�g���������Ă��Ă��`�悪�~�܂�Ȃ�

void GoBulletA(GOBJECT *pdat,BULLETINFO_A *bulinfo);//������ѓ����
DWORD BulletACallBack(DWORD msg,LPVOID dat,DWORD prm);//������ѓ���̃��b�Z�[�W�����֐�
DWORD JimaeBulletACallBack(DWORD msg,LPVOID dat,DWORD prm);//������ѓ���i�����s���^�C�v�j�̃��b�Z�[�W�����֐�

struct STRUCT_BULLETA//������ѓ���p�f�[�^
{
	int *cell_run;
	int *cell_dis;
	int num_run;
	int num_dis;
	DWORD spd_run;
	DWORD spd_dis;
	int dur;//��������
	BULLETINFO_A *pbinfo_a;//���ł�����state��0�ɂ���
	DWORD poid;//���ˌ��I�u�W�F�N�g
	DWORD flags;//�w�肳�ꂽ�t���O
	WORD ObjectID;
};

//��ѓ���̏��
#define BULLETASTATE_RUN	1//���Ē�
#define BULLETASTATE_RUN2	2//�U���͂������Ȃ����Ē�
#define BULLETASTATE_DIS	3//���Œ�

#define MYGOBJMSG_BULLETHIT	(GOBJMSG_USERDEFINE | 0xB1E0)//��ѓ���q�b�g�����惁�b�Z�[�W

typedef struct tagBULLETATKINFO
{
	GOBJECT* pbul;
	BOOL hit;
} BULLETATKINFO;
