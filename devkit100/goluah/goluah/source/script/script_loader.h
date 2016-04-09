/*!
*	@file
*	@brief �X�N���v�g�֘A�N���X��`
*/
#pragma once
/*!
*	@defgroup Script
*	@brief �X�g�[���[/�X�g�[���[���ԃf�� �X�N���v�g
*/
/*!
*	@ingroup Script
*/
/*@{*/

class CScriptLoader;
class CAliasList;

//��golush_plus.dll�̖��c
/*
#ifdef GOLUAH_PLUS_DLL
#define DLLEXINPORT __declspec(dllexport)
#else
#define DLLEXINPORT __declspec(dllimport)
#endif*/
#define DLLEXINPORT
/*!
*	@brief ���c�B�Ӗ��Ȃ��B
*	@ingroup Script
*
*	�ȑOCString���g���Ă�Ƃ����MFC�g�p��DLL�Ƃ��ĕ������Ă��Ƃ��̖��c���ۂ��B
*	���͖{�̃v���W�F�N�g����MFC�v���W�F�N�g�ɂȂ������������Ӗ��Ȃ��B
*/
class DLLEXINPORT CGoluahPlusBase
{
public:
	virtual ~CGoluahPlusBase();
	static void Destroy(CGoluahPlusBase* ptr);
};

/*!
*	@brief �X�N���v�g���[�h�N���X
*	@ingroup Script
*
*	�X�g�[���[�̃X�N���v�g��ǂݍ��ރN���X�݂����B
*	���ۂɂ͂����͋���ۂŁACScriptLoaderInstance ���@�\���������Ă�B
*	DLL�����Ă��Ƃ��̖��c
*/
class DLLEXINPORT CScriptLoader : public CGoluahPlusBase
{
public:
	static CScriptLoader* Create();
	virtual ~CScriptLoader(){}

	virtual BOOL LoadFile(TCHAR *base_dir,TCHAR *filename,UINT include_count=0) = 0;
	virtual void ReplaceAlias(CAliasList *alist) = 0;
	virtual TCHAR*   GetLine(int index) = 0;
	virtual UINT	GetLineNum() = 0;
};

/*!
*	@brief �ʖ����X�g�N���X
*	@ingroup Script
*
*	�X�N���v�g�̃e�L�X�g�ŁA������u��������̂Ɏg�����Ƃ��Ă��悤�ȋC������B
*	���ێg���Ă������H
*/
class DLLEXINPORT CAliasList : public CGoluahPlusBase
{
public:
	static CAliasList* Create();
	virtual ~CAliasList(){}

	virtual BOOL LoadFile(TCHAR* filename) = 0;
	virtual void AddAlias(TCHAR* str,TCHAR* stralias) = 0;
};




#include <list>
#include <vector>

/*!
*	@brief CStringHandler�������I�ɂ��s���X�g�̈�s��
*	@ingroup Script
*/
struct OneStringLine
{
	int line_no;
	CString str;
};
typedef std::vector<OneStringLine> strlinelist;


#define STRINGHANDLER_MAXNESTCOUNT	10		//include�ő�l�X�g��
#define STRINGHANDLER_COMMENTSTART	_T("//")	//�R�����g���ʕ�����

/*!
*	@brief CScriptLoader�N���X�{��
*	@ingroup Script
*/
class CScriptLoaderInstance : public CScriptLoader
{
public:
	/*!
	@brief �t�@�C����ǂݍ���

	�������e�F
	 1.�t�@�C���S�̂̃��[�h
	 2.�s���Ƃɕ���
	 3.�R�����g����
	 4.include����
	*/
	BOOL LoadFile(TCHAR *base_dir,TCHAR *filename,UINT include_count=0);

	//!�ʖ��u������
	void ReplaceAlias(CAliasList *alist);

	//!�w��s�擾
	TCHAR*   GetLine(int index);
	CString GetLineC(int index);
	UINT	GetLineNum(){return (UINT)list.size();}

#ifdef _DEBUG
	/*!
		@brief �f�o�b�O�p�B
		�ێ����Ă��郊�X�g���t�@�C���ɏo�͂���*/
	void DebugOutput(TCHAR *filename);

	//!�\�[�g�p��r�֐�
	static bool compline(OneStringLine line1,OneStringLine line2)
					{return line1.line_no<line2.line_no;}
#endif

protected:
	//LoadFile��������
	BYTE* LoadWholeFile(CString *filename,UINT *size);	//!< �t�@�C�����[�h����
	BOOL  DivideLine(BYTE* str_org,UINT size);			//!< �s���ƕ����������ă��X�g���\�z
	BOOL  EliminateComment();							//!< �R�����g��������
	BOOL  IncludeCheck(UINT include_count);				//!< include ����

	//!�}�[�W�BInclude�������Ɏg�p
	void Marge(int index,CScriptLoaderInstance& sh);

	CString m_base_dir;
	CString m_filename;
	strlinelist list;
};



//! �ʖ���`
struct StringPear
{
	CString str;
	CString alias;

	StringPear();
	~StringPear();
};
typedef std::list<StringPear> aliaslist;



//! �ʖ����X�g�N���X����
class CAliasListInstance : public CAliasList
{
public:
	void Load(TCHAR *filename);
	aliaslist list;
};

/*@}*/