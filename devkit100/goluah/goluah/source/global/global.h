
/*!
*	@file
*	@brief �O���[�o���֐� �O���[�o���ϐ��@�Ȃ�
*
*	���̂� global.cpp �ɂĒ�`����
*/
#pragma once

/*!
*	@defgroup global
*	@brief �O���[�o���Ȋ֐��Ƃ��ϐ��Ƃ�
*
*	��Ȃ��̂̓L�����N�^�[�̃��X�g�A�X�e�[�W�̃��X�g�A�X�g�[���[�̃��X�g�A
*	config.exe�̐ݒ�A�����J�n�O��̐ݒ�ȂǁB
*/

/*------------------------------------------------------------------------------
	�O���[�o���I�u�W�F�N�g(�N���X)
--------------------------------------------------------------------------------*/

#include "stdafx.h"
#include "dx_draw.h"
#include "dx_input.h"
#include "dx_sound.h"
#include "dx_play.h"

#include "system.h"

class AkiGlobal;
#include "config.h"
#include "log2file.h"
#include "export.h"
#include "storylist.h"
#include "stagelist.h"
#include "charlist.h"
#include "battle_load_info.h"

extern CDirectDraw			g_draw;			//!< �`��I�u�W�F�N�g
extern CDirectInput			g_input;		//!< ���̓I�u�W�F�N�g
extern CDirectSound			g_sound;		//!< ���n�I�u�W�F�N�g
extern CDirectPlay			g_play;			//!< �ʐM�I�u�W�F�N�g�@����

extern CSystem				g_system;		//!< �V�X�e���I�u�W�F�N�g

extern AkiGlobal			gbl;			//!< �O���[�o���֐��̓��ꕨ
extern CLog2File			g_log2file;		//!< �t�@�C���փ��O�ۑ��N���X
extern CExport				g_exp;			//!< DLL�ւ̊֐��񋟃N���X
extern CStoryList			g_storylist;	//!< �X�g�[���[���X�g�Ǘ��N���X
extern CStageList			g_stagelist;	//!< �X�e�[�W���X�g�Ǘ��N���X
extern CCharacterList		g_charlist;		//!< �L�����N�^�[���X�g�Ǘ��N���X
extern CBattleLoadInfo		g_battleinfo;	//!< �������N���X
extern CBattleResultInfo	g_battleresult;	//!< �������ʏ��N���X


/*------------------------------------------------------------------------------
	�O���[�o���ϐ� extern�錾
--------------------------------------------------------------------------------*/
extern char AppName[];			//!< �A�v���P�[�V������
extern BOOL g_programexit;		//!< ���C�����[�v�I���t���O
extern RECT g_rcClient;			//!< window mode�̂Ƃ��ɕK�v(?)
extern int g_DISPLAYWIDTH;		//!< �����E�C���h�E�T�C�Y�i���j
extern int g_DISPLAYHEIGHT;		//!< �����E�C���h�E�T�C�Y�i�����j
extern BOOL g_muki[MAXNUM_KEYI];//!< �L�[���͂̌���


/*------------------------------------------------------------------------------
	�O���[�o���֐�
--------------------------------------------------------------------------------*/

class AkiGlobal;

struct MY2DVECTOR;
#include "bigface_cache.h"

/*!
*	@ingroup global
*	@brief �G�p���낢��N���X
*
*	CSystem���l�A�G�p����Ȏd���ł��B
*	�O���[�o���֐��ł��������ǁAVC�̃R�[�h�⊮�����܂���ɓ����Ȃ��̂�
*	�����ɓ���Ă���Ƃ����̂�����Bgbl. �Ƒł�����R�[�h�⊮�����B
*/
class AkiGlobal{
public:
	AkiGlobal();
	~AkiGlobal();

	//!�w��E�C���h�E���f�B�X�v���C�̒��S�֎����Ă���
	static int SetWinCenter(HWND hWnd);

	//!�t�@�C�������邩�ǂ����𒲂ׂ�
	static BOOL FileExist(char *path);

	/*!
		@brief ��`�̏Փ˔���
		��]������ꍇ���X0���Z���s���݂����B
		����܂肿���Ɠ���m�F���Ă܂���
	*/
	static BOOL Syototu2RECTs(RECT r1,RECT r2,MY2DVECTOR *point,
			int gx1,int gy1,BOOL rx1,BOOL ry1,int rot1,double tx1,double ty1,double mx1,double my1,
			int gx2,int gy2,BOOL rx2,BOOL ry2,int rot2,double tx2,double ty2,double mx2,double my2,
			UINT magmode1,UINT magmode2);

	/*!
		@brief 2��������������ĐV�����o�b�t�@��Ԃ�
		�߂�l��new[]�Ŋm�ۂ����o�b�t�@�Bstr1��delete[]����܂��B
	*/
	static char* MergeString(char* str1,const char* str2);

	/*!
		@brief �_�~�[�pchar�o�b�t�@���擾�B
		��ɃX�N���v�g�e�L�X�g�̃��[�h���Ɏg���Ă��܂��B�������@�\�B
	*/
	char*  GetDummyString();

	char** CreateTemporaryStrBuff(UINT num,UINT max_len=128);	//!< �e���|����������z��̔j��
	void   DeleteTemporaryStrBuff(char** buf);					//!< �e���|����������z��̔j��

	//!�w��o�b�t�@�̐擪�ɁA�w�蕶���񂪓����Ă��邩�ǂ����`�F�b�N
	BOOL strcheck(const char* buf,const char* str);

	//!�f�J��C���[�W(face2.bmp)�擾
	MYSURFACE* GetBigFace(int cindex,int col,int alt);

	//!�u�����N�A�C�R���擾
	MYSURFACE* GetBlankIcon();

	//!���������_��(0�`1)
	float RandomOne();

	//!�w��f�B���N�g�������烉���_����BGM�Đ�
	void PlayRandomBGM(char *dir);

	
	void ods(const char *format, ...);	//!OpuputDebugString����B���s��t��
	void ods2(const char *format, ...);	//!OpuputDebugString����B���s��t�����Ȃ�

protected:
	//������֌W
	char** m_dummybuf;
	UINT m_dummy_next;

	std::list<char**> m_tempstrlist;

	char** CreateStringArray(UINT num,UINT max_len);
	void   DeleteStringArray(char** pstrarr);

	char *m_ods_buffer;

	//!�u�����N�A�C�R��
	MYSURFACE* m_blankIcon;
};

//!2D�x�N�g���\���́B��`�Փ˔���֐��Ŏg�p
struct MY2DVECTOR {double x;double y;};


/*!
*	@ingroup global
*	@brief �t�@�C���A�N�Z�X�x���N���X�H
*
*	�t�@�C���ւ̃A�N�Z�X���ȗ����������č�����N���X�B
*	���Ƃ��������̂ł���܂�g���Ă��Ȃ����A�ނ���MFC��CFile���g�����ق����ǂ��B
*/
class AkiFile
{
public:
	AkiFile();
	~AkiFile(){Destroy();}
	BOOL Load(char *filename);
	BOOL Save(char *filename);

	void	Set(DWORD size,BYTE* ptr=NULL);

	DWORD	m_size;
	BYTE*	m_buffer;

protected:
	void Destroy();//!< Set���̋��������j��,�f�X�g���N�^�@���ʑ���
};


#ifndef _aki3d_h_
//!ARGB ���� DWORD �ϊ��N���X
struct ARGB
{
	union
	{
		DWORD dwcol;
		struct
		{
			BYTE blue;
			BYTE green;
			BYTE red;
			BYTE alpha;
		};
	};
};
#define ARGB_STRUCT
#endif
