/*!
*	@file
*	@brief �X�g�[���[�X�N���v�g����
*/
/*!
*	@ingroup Script
*/
/*@{*/
#pragma once

#include "define_const.h"
#include "define_macro.h"
#include "script_loader.h"

class CStoryScriptElement;
typedef std::vector<CStoryScriptElement*> SScriptElementList;

/*!
*	@brief �X�g�[���[�X�N���v�g�ǂݍ��ݓǂݍ���
*	@ingroup Script
*/
class CGoluahStoryScript
{
public:
	static BOOL CreateScriptElementList(SScriptElementList& list,TCHAR *filename);
	static void DestroyScriptElementList(SScriptElementList& list);
};


/*!
*	@brief �X�g�[���[�v�f�x�[�X
*	@ingroup Script
*/
class CStoryScriptElement
{
public:
	CStoryScriptElement();
	virtual ~CStoryScriptElement();

	enum CStoryElement_Types
	{
		GSE_ST_ERROR	,
		GSE_ST_STAFF	,
		GSE_ST_VS		,
		GSE_ST_DEMO		,
		GSE_ST_SETTINGS ,
		GSE_ST_SELECT	,
	};

	virtual CStoryElement_Types GetType()=0;	//!<dynamic_cast����΂������������l
	virtual void FeedLine(TCHAR *str)=0;			//!<���̒�`������������FALSE
	virtual BOOL Close(){return TRUE;}			//!<����#������������R�[��
	virtual TCHAR* GetError(){return m_error;}	//!<�G���[�擾

protected:
	TCHAR *m_error;
	void Error(TCHAR *str);
	void ErrorF(TCHAR *str,TCHAR *name);	//!<��������w��
	void ErrorF(TCHAR *str,int i);		//!<�����w��
};


/*!
*	@brief �u���b�N�O�G���[�v�f
*	@ingroup Script
*/
class CStoryElement_Error : public CStoryScriptElement
{
public:
	CStoryElement_Error(TCHAR *err);
	
	CStoryElement_Types GetType(){return GSE_ST_ERROR;}
	void FeedLine(TCHAR *str){}
	BOOL Close(){return TRUE;}
};


/*!
*	@brief #settings�v�f
*	@ingroup Script
*/
class CStoryElement_Settings : public CStoryScriptElement
{
public:
	CStoryElement_Settings();
	~CStoryElement_Settings();

	CStoryElement_Types GetType(){return GSE_ST_SETTINGS;}
	void FeedLine(TCHAR *str);
	BOOL Close();

public:
	//�S��
	float ver;						//!<�X�N���v�g�̃o�[�W�����i-1:���ݒ�j
	TCHAR* title;					//!<�X�g�[���[�^�C�g��
	TCHAR* brief;					//!<�X�g�[���[�T�v
	TCHAR* icon;						//!<�A�C�R���t�@�C����
	TCHAR* preview;					//!<�v���r���[�t�@�C����
	int continue_num;				//!<�R���e�B�j���[�\��(�}�C�i�X�Ł�)
	int result_send;				//!<�������ʑ��M�Ώ�(�}�C�i�X�őΏۖ���)
	int result_key;					//!<�������ʑ��M���ɕt��������L�[�ԍ�

	//�L�����N�^��`
	int cnum;						//!<TCHAR�w�萔
	int characters[MAXNUM_TEAM];	//!<TCHAR�w��@(-1:userselect -2:�w��Ȃ�)
	int color[MAXNUM_TEAM];			//!<�F�w��
	DWORD option[MAXNUM_TEAM];		//!<�I�v�V�������ڎw��(userselect�ȊO�ŗL��)
	StoryOptType opttype[MAXNUM_TEAM];	//!<�I�v�V�����w��̎��(userselect�ȊO�ŗL��)

protected:
	void Read_char(TCHAR *str);
	BOOL multiline_flag;
};


/*!
*	@brief #staff�v�f
*	@ingroup Script
*/
class CStoryElement_Staff : public CStoryScriptElement
{
public:
	CStoryElement_Staff();
	~CStoryElement_Staff();

	CStoryElement_Types GetType(){return GSE_ST_STAFF;}
	void FeedLine(TCHAR *str);

public:
	TCHAR *m_filename;				//!<NULL�̏ꍇ system/staff.txt
};


/*!
*	@brief #vs�v
*	@ingroup Script
*/
class CStoryElement_VS : public CStoryScriptElement
{
public:
	CStoryElement_VS(CStoryElement_Settings* sets);
	~CStoryElement_VS();

	CStoryElement_Types GetType(){return GSE_ST_VS;}
	void FeedLine(TCHAR *str);
	BOOL Close();

//	void  SetupBattleInfo();

public:
	//�L�����N�^�֘A
	int num[2];							//!<2�`�[���̃L�����N�^��
	int characters[2][MAXNUM_TEAM];		//!<-1:�����_�� -2:char1 -3:char2 -4:char3 -5>none
	int color[2][MAXNUM_TEAM];			//!<-1:�����_��
	DWORD option[2][MAXNUM_TEAM];		//!<�I�v�V�������ڎw��
	BOOL is_random_opt[2][MAXNUM_TEAM];	//!<�����_���w��t���O

	//COM���x���֘A
	int level[2][MAXNUM_TEAM];				//!<���x���w��
	BOOL is_related_level[2][MAXNUM_TEAM];	//!<���΃��x���w��t���O

	//�X�e�[�W�E����������
	int type;						//!<�����`��
	int stage;						//!<�X�e�[�W -1:�����_��
	int limit_time;					//!<��������(�}�C�i�X��SETINGS�l�p��)
	TCHAR *bgm_filename;				//!<BGM�w��(NULL�Ŏw�薳��)
	int bgm_startpos;				//!<BGM�Đ��J�n�ʒu(0�Œʏ�Đ�)

	//������ݒ�
	TCHAR *text_win;					//!<�����䎌�w��
	TCHAR *text_lose;				//!<�������Ƃ��́A�G�̏����䎌�w��
	BOOL show_win;					//!<������ʂɈڍs���邩�H
	BOOL nogameover;				//!<�����Ă��Q�[���I�[�o�[���Ȃ�
	int sel_shift;					//!<nogameover���A�I��ϐ��̃V�t�g��

protected:
	void Read_friend(TCHAR *str,int team=0);
	void Read_enemy(TCHAR *str);
	void Read_stage(TCHAR *str);

	//�X�N���v�g���ߒ��Ɏg�p
	CStoryElement_Settings* m_sets;	//!<#settings���� %char1-3�g�p���ɒ�`������Ă��邩�ǂ����`�F�b�N���邽��
	UINT multiline_flag;			//!<�����s�t���O�B 0:���� 1:text_win�ɒǉ����� 2:text_lose�ɒǉ�����
};


/*!
*	@brief #demo�v�f
*	@ingroup Script
*/
class CStoryElement_Demo : public CStoryScriptElement
{
public:
	CStoryElement_Demo();
	~CStoryElement_Demo();

	CStoryElement_Types GetType(){return GSE_ST_DEMO;}
	void FeedLine(TCHAR *str);
	BOOL Close();

public:
	TCHAR *m_filename;		//!<NULL�����ݒ聨�G���[

	/*
	//�������X�g�ۊǁE�擾
	struct STRING_PEAR{TCHAR *str,*alias;};
	typedef std::list<STRING_PEAR> aliaslist;
	aliaslist args;
	CAliasList* CreateAliasClass();
	*/
};


/*!
*	@brief #sselect�v�f
*	@ingroup Script
*/
class CStoryElement_Select : public CStoryScriptElement
{
public:
	CStoryElement_Select(int selectnum)	{ m_selectnum = selectnum; }
	~CStoryElement_Select(){}

	CStoryElement_Types GetType(){return GSE_ST_SELECT;}
	void FeedLine(TCHAR *str){}
	int GetNum()	{ return m_selectnum; }

public:
	int m_selectnum;

	/*
	//�������X�g�ۊǁE�擾
	struct STRING_PEAR{TCHAR *str,*alias;};
	typedef std::list<STRING_PEAR> aliaslist;
	aliaslist args;
	CAliasList* CreateAliasClass();
	*/
};



/*@}*/