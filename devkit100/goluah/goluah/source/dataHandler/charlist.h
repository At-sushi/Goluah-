
/*!
*	@file
*	@brief �L�����N�^�[�Ǘ�
*	�f�B���N�g������L�����N�^�[�̃��X�g���쐬���ĊǗ�����B
*/

#pragma once

#include "gobject.h"//needs CHARACTER_LOAD_OPTION

/*!
*	@ingroup global
*/
/*@{*/

typedef std::vector<CHARACTER_LOAD_OPTION> CharOptionList;	//!< �L�����N�^�[�I�v�V�����z��(STL)


/*!
*	@brief �L�����N�^�[��optset.txt���
*/
struct FAVORITE_OPTION
{
	TCHAR name[32];	//!< �I�v�V������
	DWORD opt;		//!< �I�v�V�����l
};
typedef std::vector<FAVORITE_OPTION> FavoriteOptionList;	//!< �L�����N�^�[�I�v�V�����Z�b�g�z��(STL)

/*! @brief �L�����N�^�[�̏�� */
struct CCL_CHARACTERINFO
{
	TCHAR dir[64];				//!< �ǂ̃f�B���N�g���ɂ��邩
	TCHAR name[32];				//!< ���O�͂Ȃ�Ƃ����̂�
	DWORD ver;					//!< dll�̃o�[�W����

	DWORD caps;					//!< �L�����N�^�[�̔\�́i�X�g�[���[��/�s�A�l�b�g���[�N�Ή�/��Ή����j
	
	//DWORD byteCheck;			//�l�b�g���[�N�ΐ펞�̃`�F�b�N
	CharOptionList options;		//!< �I�v�V��������
	DWORD maxpoint;				//!< �I�v�V�����|�C���g�ő�l
	DWORD previous_option;		//!< �O��I�����ꂽ�I�v�V����
	FavoriteOptionList fav_opts;//!< �I�v�V�����Z�b�g(optset.txt����擾)
	UINT  previous_favorite;	//!< �O��I���I�v�V����? ���g�p���ۂ�

	CCL_CHARACTERINFO(){previous_option=0;previous_favorite=0;}
	//! �G���[�΍�
	CCL_CHARACTERINFO(const CCL_CHARACTERINFO& par){
		strcpy_s(dir, par.dir);
		strcpy_s(name, par.name);
		ver = par.ver;
		caps = par.caps;
		options = par.options;
		maxpoint = par.maxpoint;
		previous_option = par.previous_option;
		fav_opts = par.fav_opts;
		previous_favorite = par.previous_favorite;
	}
};
typedef std::vector<CCL_CHARACTERINFO> CCLCharacterInfoList;	//!< �L�����N�^�[���z��(STL)


/*!	@brief �F�����s�L�����N�^�[�̏�� */
struct CCL_DAMEINFO
{
	TCHAR dir[64];				//!< �ǂ̃f�B���N�g���ɂ��邩
	TCHAR name[32];				//!< ���O�͂Ȃ�Ƃ����̂�
	DWORD dame;					//!< �ʖڂȗ��R
	DWORD ver;					//!< dll�̃o�[�W����
};
typedef std::vector<CCL_DAMEINFO> CCLDameInfoList;	//!< �F�����s�L�����N�^�[�z��(STL)

#define CCL_DAME_NODLL		1//!< �L�����N�^�[�F�����s���RID , action.dll�̓ǂݍ��݂Ɏ��s
#define CCL_DAME_CANTGETFP	2//!< �L�����N�^�[�F�����s���RID , �֐��|�C���^�擾�Ɏ��s
#define CCL_DAME_FFAIL		3//!< �L�����N�^�[�F�����s���RID , �֐���FALSE��Ԃ��Ă���
#define CCL_DAME_OLDDLL		4//!< �L�����N�^�[�F�����s���RID , �o�[�W�����`�F�b�N�Ɏ��s
#define CCL_DAME_NEWDLL		5//!< �L�����N�^�[�F�����s���RID , �o�[�W�����`�F�b�N�Ɏ��s(2)



/*! @brief �����O�i�L�����N�^�[�f�B���N�g���j��� */
struct CCL_RINGINFO
{
	TCHAR name[64];							//!< ���O=�f�B���N�g����
	std::vector<DWORD> ring2serialIndex;	//!< ����ɕ��񂾏��Ԃł̃C���f�b�N�X�ɕϊ�
};
typedef std::vector<CCL_RINGINFO> CCLRingInfoList;	//!< �L�����N�^�[�����O���X�g(STL)



class CCOptionSelecter;
class CCSimpleOptionSelecter;

/*! @brief �L�����N�^�[���X�g�Ǘ��N���X */
class CCharacterList
{
public:
	CCharacterList();
	~CCharacterList(){Destroy();}
	
	void Initialize();
	void Destroy();

	//�L�����N�^�̏��擾�n
	int GetCharacterCount();				//!< �������ꂽ�S�L�����N�^�[����Ԃ��B
	TCHAR* GetCharacterDir(UINT index);		//!< �L�����N�^�[�̃f�B���N�g���p�X��Ԃ�
	TCHAR* GetCharacterDir(UINT index,int ring);
	TCHAR* GetCharacterName(UINT index);		//!< �L�����N�^�[�̖��O��Ԃ�
	DWORD GetCharacterVer(UINT index);		//!< �L�����N�^�[�̃o�[�W������Ԃ�
	int   FindCharacter(TCHAR *name);		//!< ���O�Ō����i�Ȃ������ꍇ-1�j
	DWORD GetCaps(UINT index);				//!< �L�����N�^�[��caps�t�B�[���h���擾����

	//�����O���擾�n
	int GetRingNum();						//!< �����O�̐���Ԃ�
	TCHAR* GetRingName(UINT index);			//!< �����O�̖��O�i�f�B���N�g�����j��Ԃ�
	int GetCharacterCountRing(UINT index);	//!< �w��C���f�b�N�X�̃����O�Ō������ꂽ�L�����N�^�[����Ԃ�
	DWORD RingIndex2SerialIndex(UINT ring,UINT index);//!< �����O���ł̃C���f�b�N�X���A�S�̂ł̃C���f�b�N�X�ɕϊ�����

	//�F�����s�L�����N�^�[�̏��
	int GetDameCharCount();					//!< �F�����s�L�����N�^�[�̐���Ԃ�
	TCHAR* GetDameCharDir(UINT index);		//!< �F�����s�L�����N�^�[�̃f�B���N�g����Ԃ�
	TCHAR* GetDameCharName(UINT index);		//!< �F�����s�L�����N�^�[�̖��O��Ԃ�
	DWORD GetDameCharReas(UINT index);		//!< �F�����s�L�����N�^�[�̔F�����s���R��Ԃ�
	DWORD GetDameCharVer(UINT index);		//!< �F�����s�L�����N�^�[�̃o�[�W������Ԃ�

/*	//�l�g���[�N�֌W
	BOOL GetNetCharacterInfo(NETMESSAGE_INDICATE_CONNECT *dat);
	BOOL CheckNetCharacterList(NETMESSAGE_INDICATE_CONNECT *dat);
	void InitializeNetCharacterIDList();
	void SetNetCharacterIDList(NETMESSAGE_BODY_CHARACTER_ID* dat);
	DWORD GetRandomNetChar();
	DWORD GetIndexFromNetID(DWORD netid,BOOL exitWhenError=FALSE);
	DWORD GetByteCheck(DWORD index){return(infolist[index].byteCheck);}
	DWORD GetNetIDFromIndex(DWORD index);*/

	//�I�v�V�����֌W
	CCOptionSelecter* GetOptionSelecter(DWORD cindex);
	void SetPreviousOption(DWORD index,DWORD opt){infolist[index].previous_option=opt;}
	DWORD GetRandomOption(DWORD index);
	void  CorrectOption(UINT cindex,DWORD *opt);
	void LoadFavoriteOptions(TCHAR* dir,FavoriteOptionList& list);
	CCSimpleOptionSelecter* GetSimpleOptionSelecter(DWORD cindex);

private:
	//Initialize����g�p�����
	void InitializeRingList();				//!< �f�B���N�g�����������A�����O�̃f�B���N�g�������X�g���\�z����
	void InitializeRing(DWORD ringindex);	//!< �f�B���N�g������������j
	BOOL VerifyCharacterDir(TCHAR *dir,DWORD ringindex);//!< �L�����N�^�f�B���N�g���̐����������؂���

private:
	CCLCharacterInfoList infolist;	//!< �L�����N�^���X�g
	CCLRingInfoList ringlist;		//!< �����O���X�g
	CCLDameInfoList damelist;		//!< �f�B���N�g���͂���������verify�Ɏ��s�������X�g
};



class CTOptionSelecterBase;	//!< �I�v�V�����I���x�[�X�N���X
class CTOptionSelecter;		//!< �I�v�V�����I���N���X�iVS���[�h���j
class COptionSelecter;		//!< �I�v�V�����I���N���X�i�X�g�[���[���[�h���j

/*!
*	@brief �I�v�V�����I���f�[�^�Ǘ��N���X
*	@sa CTOptionSelecter
*
*	ver0.90x �Ŏg���Ă��Â��N���X�BCTOptionSelecter�͂���������ɂ��B
*/
class CCOptionSelecter
{
friend class CTOptionSelecterBase;
friend class CTOptionSelecter;
friend class CTOptionSelecterStory;
friend class CTSimpleOptionSelecter;
public:
	CCOptionSelecter(CCL_CHARACTERINFO *info,DWORD maxp);
	void Initialize(DWORD ini_opt);
	virtual BOOL HandlePad(DWORD input);
	void Draw();
	BYTE GetFadeOut();
	DWORD GetSettings();
	void SetOffset(int val){offset_x=val;}
	void SetZ(float val){z=val;}

	TCHAR* GetCurrentSetName();						//!< 0:"Free" , 0�`:favorite�ݒ薼
	void ApplyToPreviousSelect();					//!< �O��I���t���O�ɐݒ肷��

protected:
	void TurnOffDependFlags( DWORD flag );			//!< flag�Ɉˑ�����t���O��S��OFF
	void TurnOffExclisiveFlags( DWORD ex_flag );	//!< flag�Ƌ�������t���O��S��OFF
	void SetRandom();								//!< �����_���Z�b�g

	DWORD counter;					//!< �`��p�J�E���^�H
	DWORD state;					//!< ���
	CharOptionList* list;			//!< �I�v�V�������X�g
	DWORD current_selected;			//!< �I�𒆂̍���
	int current_point;				//!< ���ݎc��|�C���g
	BOOL enabled[32];				//!< �e�I�v�V����ON/OFF�t���O
	BOOL commit_enabled;			//!< ����\�t���O
	DWORD maxpoint;					//!< �|�C���g���E�l
	int offset_x;					//!< �`��X�I�t�Z�b�g�l
	float z;						//!< �`��Z�l

	CCL_CHARACTERINFO *m_ref_cinfo;	//!< �L�����N�^���(�Q��)
	DWORD m_current_favorite;		//!< ���݂̐ݒ�A0:Free , 1�`:favolite0�`
};

/*!
*	@brief �P�����ŗp�I�v�V�����I���f�[�^�Ǘ��N���X
*	@sa CTSimpleOptionSelecter
*
*	CTSimpleOptionSelecter�����Ŏg�p
*	CCOptionSelecter�Ƃقړ����ł����A����n���ꕔ�ς���Ă��܂��B
*/
class CCSimpleOptionSelecter : public CCOptionSelecter
{
friend class CTSimpleOptionSelecter;
public:
	CCSimpleOptionSelecter(CCL_CHARACTERINFO *info,DWORD maxp);
	BOOL HandlePad(DWORD input);

	DWORD GetFavorite() { return 	m_current_favorite; }
	DWORD GetState() { return 	state; }
};

/*@}*/