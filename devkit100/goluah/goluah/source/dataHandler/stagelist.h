
/*========================================================================
	
	�X�e�[�W�Ǘ�

	�f�B���N�g������X�e�[�W�̃��X�g���쐬���ĊǗ�����B

==========================================================================*/
#pragma once

/*!
*	@ingroup global
*/
/*@{*/


/*!
*	@brief �X�e�[�W�̏�� 
*/
struct CSL_STAGEINFO
{
	char dir[64];				//!< �ǂ̃f�B���N�g���ɂ��邩
	char name[32];				//!< ���O�͂Ȃ�Ƃ����̂�
	DWORD ver;					//!< DLL���Ԃ����o�[�W����
};
typedef std::vector<CSL_STAGEINFO> CSL_STAGEINFOLIST;	//!< �X�e�[�W���z��(STL)



/*!
*	@brief �F�؎��s�X�e�[�W�̏��
*/
struct CSL_DAMEINFO
{
	char dir[64];				//!< �ǂ̃f�B���N�g���ɂ��邩
	DWORD damereas;				//!< �ʖڂȗ��R
	DWORD ver;					//!< DLL���Ԃ����o�[�W����
};
typedef std::vector<CSL_DAMEINFO> CSL_DAMEINFOLIST;	//!< �F�؎��s�X�e�[�W�z��(STL)

#define CSL_DAME_NONAME		1	//!< �X�e�[�W�F�����s���RID�A���O��������Ȃ�����
#define CSL_DAME_PROC		2	//!< �X�e�[�W�F�����s���RID�A�֐��|�C���^�擾�Ɏ��s
#define CSL_DAME_PROC2		3	//!< �X�e�[�W�F�����s���RID�AStageInfo�֐��Ɏ��s
#define CSL_DAME_OLDDLL		4	//!< �X�e�[�W�F�����s���RID�ADLL�̃o�[�W�������Â�
#define CSL_DAME_NEWDLL		5	//!< �X�e�[�W�F�����s���RID�ADLL�̃o�[�W�������V����



/*!
*	@brief �X�e�[�W�f�B���N�g�����X�g�i�X�e�[�W�����O�j
*/
struct CSL_RINGINFO
{
	char name[32];						//!< �����O��(=�\����)
	std::vector<DWORD> ring2serial;		//!< �����O���C���f�b�N�X���S�̃C���f�b�N�X
};
typedef std::vector<CSL_RINGINFO> CSL_STAGERINGLIST;	//!< �X�e�[�W�����O�z��(STL)




/*!
*	@brief �X�e�[�W���X�g�Ǘ��N���X�{��
*
*	�O���[�o���ɂЂƂ����C���X�^���X������B
*	�X�e�[�W�Ɋւ�����܂Ƃ߁B
*/
class CStageList
{
public:
	CStageList();
	~CStageList(){Destroy();}

	void Initialize();				//!< �f�B���N�g�����������A���X�g��������
	void Destroy();					//!< ���X�g��j��

	int GetStageCount();			//!< �������ꂽ�X�e�[�W����Ԃ�
	char* GetStageDir(int index);	//!< �X�e�[�W�̃f�B���N�g����Ԃ�
	char* GetStageName(int index);	//!< �X�e�[�W�̖��O��Ԃ�
	DWORD GetStageVer(int index);	//!< �X�e�[�W�̃o�[�W������Ԃ�
	int FindStage(char *name);		//!< ���O�Ō����i�Ȃ������ꍇ-1�j

	int GetDameStageCount();						//!< �����Ɏ��s�����X�e�[�W����Ԃ�
	char* GetDameStageDir(int index);				//!< ���s���R�𕶎���ŕԂ�
	DWORD GetDameStageReason(int index);			//!< �������s���R�𗝗RID�l�ŕԂ�
	DWORD GetDameStageVer(int index);				//!< �������s�X�e�[�W�E�o�[�W������Ԃ�

	DWORD Ring2Serial(DWORD ring,DWORD rindex);		//!< �����O���ł̃C���f�b�N�X��S�̂ł̃C���f�b�N�X�ɕϊ�
	DWORD GetStageCountRing(DWORD rindex);			//!< �w�胊���O�ɃX�e�[�W���������邩�Ԃ�
	DWORD GetRingNum(){return ringlist.size();}		//!< �����O�̌���Ԃ�

private:
	void InitializeRingList();						//!< Initialize����Ă΂��
	void InitializeRing(DWORD index);				//!< Initialize����Ă΂��
	BOOL VerifyStageDir(char *dir,DWORD ring);		//!< Initialize����Ă΂��

private:
	CSL_STAGEINFOLIST infolist;		//!< �X�e�[�W��񃊃X�g
	CSL_STAGERINGLIST ringlist;		//!< �X�e�[�W�����O���X�g
	CSL_DAMEINFOLIST damelist;		//!< �f�B���N�g���͂��������ǔF���Ɏ��s�������X�g
};

/*@}*/


