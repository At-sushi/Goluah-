
class CCharacter : public CCharacterBase
{
public:
	CCharacter(CDI_CHARACTERINFO2 *info);
	virtual ~CCharacter();

protected:
	//*********************************************
	//  �I�[�o�[���C�h����֐�
	//*********************************************

	DWORD Message(DWORD msg,LPVOID pd,DWORD prm);
	DWORD Action();
	BOOL Command_OnNormal(DWORD keyinfo);
	BOOL Command_Normal(DWORD keyinfo);
	BOOL Command_Hissatu(DWORD keyinfo);
	void InitAttackInfo();//�U���͏��̏�����
	void InitWazInfo();//�Z���̏�����

	//�ʏ퓮��
	void act_neutral();//�j���[�g����
	void act_crouch();//���Ⴊ��
	void act_rakka();//����
	void act_tojyo();//�o��
	void act_win();//����
	void act_walkf();//�O�i
	void act_walkb();//���
	void act_jamps();//�W�����v�\������
	void act_jampc();//�����W�����v
	void act_jampf();//�O�W�����v
	void act_jampb();//��W�����v
	//�ʏ�.�U��
	void act_att_sa();//����
	void act_att_sb();//����
	void act_att_sc();//����
	void act_att_ca();//����
	void act_att_cb();//����
	void act_att_cc();//����
	void act_att_ja();//���
	void act_att_jb();//��
	void act_att_jc();//��
	//���̑�
	void act_taiki();
	void act_koutai_out();
	void act_koutai_in();
	void act_koutai();
	void act_koutai2();

	//*********************************************
	//�@CCharacter�N���X�Ǝ��̊֐�
	//*********************************************
private:
	void InitializeMyCharacter();//�p�����[�^������
	void StartAttack(DWORD atk_info_id);
	void AddPowerGauge(double dp);

	//�ǉ��s���֐�

	//�U���͏��
	ATTACKINFO aif[32];

	// �I�u�W�F�N�g�i���o�[�ݒ�p
	static BYTE gCreateCounter;
	static BYTE gDeleteCounter;
	enum { USERID = 0x01 };
};

extern LPTSTR CharDirectory;

//�s��ID�������ɒǉ�����


//�U���͔ԍ��������ɒ�`����
/* sample
#define ATTACKINFO_STAND_A		0
#define ATTACKINFO_STAND_B		1
#define ATTACKINFO_STAND_C		2
#define ATTACKINFO_CROUCH_A		3
#define ATTACKINFO_CROUCH_B		4
#define ATTACKINFO_CROUCH_C		5
#define ATTACKINFO_JAMP_A		6
#define ATTACKINFO_JAMP_B		7
#define ATTACKINFO_JAMP_C		8
*/


//�U��ID�i�K�v������ΕύX����j
#define ATTACK_WEAK				1
#define ATTACK_MIDDLE			2
#define ATTACK_STRONG			3
#define ATTACK_HISSATU			4
#define ATTACK_EREAL			0x00010000

//���[�U�[��`���b�Z�[�W
#define GOBJMSG_NAGE			(GOBJMSG_USERDEFINE | 0x0001)	//�����p

//�I�v�V������`
#define OPTIONS_CHAIN_COMBO		0x00000001
