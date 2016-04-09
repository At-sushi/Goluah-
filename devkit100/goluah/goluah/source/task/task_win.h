
/*===========================================================

	�������

=============================================================*/
#pragma once

#include "task.h"

/*!
*	@brief ������ʃ^�X�N
*	@ingroup Tasks
*
*	VS���[�h�^�X�N��A�X�g�[���[���[�h�^�X�N��������N������B
*	�����䎌��ʎq�Ȃǂ̓O���[�o����g_battleinfo��g_battleresult����擾
*/
class CYouWin : public CExclusiveTaskBase
{
public:
	CYouWin();
	~CYouWin();

	void Initialize();
	BOOL Execute(DWORD time);
	void Terminate();
	void Draw();

	DWORD GetID(){return 'Win';}
	int GetDrawPriority(){return 1000;}

	void SetStoryText(TCHAR *txt);

protected:
	void CleanUp();

//�ϐ�
protected:
	DWORD counter;				//!< Execute�p�J�E���^
	BOOL  m_end_flag;			//!< �I���t���O
	UINT  m_textlen;			//!< �\���e�L�X�g��
	DWORD wt;					//!< ���������̃`�[��
	MYSURFACE *dds_face[3];		//!< �\����

	LPDIRECT3DTEXTURE9 tex_fb;	//!< �����I�����̃t�����g�o�b�t�@�̃R�s�[

	int f_x[3];					//!< face2�̕\���ʒu
	float f_z[3];				//!< face2�̕\���ʒu
	BOOL show_text;				//!< �e�L�X�g�\�����邩�ǂ���
	DWORD jun[3];				//!< �\���̏���

	TCHAR *m_text_from_story;	//!< �X�g�[���[������w�肳�ꂽ�e�L�X�g
	TCHAR *m_text;				//!< m_text_from_story�܂���g_battleresult����擾�����e�L�X�g�̂����A���ۂɕ\������ق��B
};

