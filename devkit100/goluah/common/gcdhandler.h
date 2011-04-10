
/*!
*	@file
*	@brief GCD�`���f�[�^��舵����`
*
*	Goluah!!�{�̂ƁA��`�G�f�B�^�iare_editer�j����Q�Ƃ���܂�
*	GCD_EDITER�̒�`�̗L���ɂ���Đ؂�ւ��܂��B
*/
#pragma once

#include "define_gcd.h"


/*!
*	@ingroup global
*/
/*@{*/

//---------------------------------------------------------------------
/*!
	@brief GCD�`���f�[�^��舵���N���X

	���static�ŕ��ʂ̊֐��Ƃ��Ďg�p����B
	��`�G�f�B�^�̏ꍇ�͂��̃N���X�̃C���X�^���X���f�[�^�̃Z�b�g�Ƃ��Ĉ����B
*/
class CGCDHandler
{
// �X�^�e�B�b�N----------------------------------------------------------
//���ǂݍ���
public:

#ifdef GCD_EDITER
static BOOL GCDLoadDlg(GCD_CELL2 *pcdat,GCD_RECT *prdat,
				GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames=NULL);
#endif//GCD_EDITER

//!GCD�t�@�C����ǂݍ���
static int GCDLoad(char *filename,				//!< [in] �ǂݍ��ރt�@�C����
				   LPVOID pcdat,				//!< [out] �Z���f�[�^�Ǎ���o�b�t�@
				   LPVOID prdat,				//!< [out] �؂����`�f�[�^�Ǎ���o�b�t�@ 
				   LPVOID phdat,				//!< [out] �����蔻���`�Ǎ���o�b�t�@
				   LPVOID pfhnames=NULL,		//!< [out] �Z����`���Ǎ���o�b�t�@
				   DWORD version=GCD_VERSION	//!< [in] �w�肳�ꂽ�Ǎ���o�b�t�@���ǂ̃o�[�W�����̍\���̂ŗp�ӂ���Ă��邩
				   );
//!�o�[�W����0.70�̃t�@�C����ǂݍ���
static int GCDLoad070(HANDLE hFile,				//!< [in] �ǂݍ��ރt�@�C��
				  LPVOID pcdat,					//!< [out] �Z���f�[�^�Ǎ���o�b�t�@
				   LPVOID prdat,				//!< [out] �؂����`�f�[�^�Ǎ���o�b�t�@ 
				   LPVOID phdat,				//!< [out] �����蔻���`�Ǎ���o�b�t�@
				   LPVOID pfhnames=NULL,		//!< [out] �Z����`���Ǎ���o�b�t�@
				   DWORD version=GCD_VERSION	//!< [in] �w�肳�ꂽ�Ǎ���o�b�t�@���ǂ̃o�[�W�����̍\���̂ŗp�ӂ���Ă��邩
				  );
//!�o�[�W����0.90�̃t�@�C����ǂݍ���
static int GCDLoad090(HANDLE hFile,
				  LPVOID pcdat,					//!< [out] �Z���f�[�^�Ǎ���o�b�t�@
				  LPVOID prdat,					//!< [out] �؂����`�f�[�^�Ǎ���o�b�t�@ 
				  LPVOID phdat,					//!< [out] �����蔻���`�Ǎ���o�b�t�@
				  LPVOID pfhnames=NULL,			//!< [out] �Z����`���Ǎ���o�b�t�@
				  DWORD version=GCD_VERSION		//!< [in] �w�肳�ꂽ�Ǎ���o�b�t�@���ǂ̃o�[�W�����̍\���̂ŗp�ӂ���Ă��邩
				  );
//!���k�ς݃t�@�C����ǂݍ���
static int GCDLoadCompressed(char *filename,		//!< [in] �ǂݍ��ރt�@�C����
				   LPVOID pcdat,				//!< [out] �Z���f�[�^�Ǎ���o�b�t�@
				   LPVOID prdat,				//!< [out] �؂����`�f�[�^�Ǎ���o�b�t�@ 
				   LPVOID phdat,				//!< [out] �����蔻���`�Ǎ���o�b�t�@
				   LPVOID pfhnames=NULL,		//!< [out] �Z����`���Ǎ���o�b�t�@
				   DWORD version=GCD_VERSION	//!< [in] �w�肳�ꂽ�Ǎ���o�b�t�@���ǂ̃o�[�W�����̍\���̂ŗp�ӂ���Ă��邩
				   );

//���ۑ� �i�ۑ��ɓn���o�b�t�@�͏�ɍŐV�̃o�[�W�����ł��邱�Ƃ����肳���j
public:
#ifdef GCD_EDITER
static BOOL GCDSaveDlg(GCD_CELL2 *pcdat,GCD_RECT *prdat,
				GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames=NULL);
static int GCDSave(char *filename,GCD_CELL2 *pcdat,GCD_RECT *prdat,
			 GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames=NULL);
static int GCDSave070(char *filename,GCD_CELL2 *pcdat,GCD_RECT *prdat,
			 GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames=NULL);
static BOOL GCDSaveHeader(GCD_CELLNAMES *pfhnames);
static int GCDSaveCompressed(char *filename,GCD_CELL2 *pcdat,GCD_RECT *prdat,
			 GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames=NULL);
#endif//GCD_EDITER

//�����̑����[�e�B���e�B
#ifdef GCD_EDITER
static void GCDSetInitialData(GCD_CELL2 *pcdat,GCD_RECT *prdat,
					   GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames);
#endif//GCD_EDITER
static BOOL GCDIsEmptyCell2(GCD_CELL2& gcdc,GCD_HANTEI& gcdh);
static BOOL GCDIsEmptyRect(GCD_RECT& gcdr);
static void GCDSetEmptyCell2(GCD_CELL2 *pgcdc,GCD_HANTEI *pgcdh);
static void GCDSetEmptyRect(GCD_RECT *pgcdr);
static void GCDConvCell_070_090(GCD_CELL2_070 *src,GCD_CELL2 *dst);
static void GCDConvCell_090_070(GCD_CELL2 *src,GCD_CELL2_070 *dst);


// �_�C�i�~�b�N�i�H�j---------------------------------------------------
public:
	CGCDHandler();
	~CGCDHandler(){Destroy();}

	int  Load(char *filename,BOOL hload,BOOL nload=FALSE);
	void Destroy();
	void Diet();
	
	BOOL valid(){return m_cdat&&m_rdat ? TRUE : FALSE;}
	BOOL IsEmptyRect(DWORD index){return GCDIsEmptyRect(m_rdat[index]);}
	BOOL IsEmptyCell(DWORD index){return GCDIsEmptyCell2(m_cdat[index],m_hdat[index]);}
	GCD_RECT*  GetRect(DWORD index){return &m_rdat[index];}
	GCD_CELL2* GetCell(DWORD index){return &m_cdat[index];}
	GCD_HANTEI* GetHantei(DWORD index){return &m_hdat[index];}
	char* GetName(DWORD index){return m_names->name[index];}

	#ifdef GCD_EDITER
	BOOL Save();
	#endif//GCD_EDITER

	GCD_CELL2		*m_cdat;	//!< �Z���f�[�^
	GCD_RECT		*m_rdat;	//!< ��`�f�[�^
	GCD_HANTEI		*m_hdat;	//!< �����蔻��f�[�^
	GCD_CELLNAMES	*m_names;	//!< ��`��
};

/*@}*/