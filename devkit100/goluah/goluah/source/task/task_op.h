
/*===========================================================

	op.avi�Đ�

=============================================================*/
#pragma once

#include "task.h"
/*!
	@brief �ȂɁH����H
*/
class COpeningMovie : public CExclusiveTaskBase
{
public:
	COpeningMovie();
	~CYouWin();

	void Initialize();
	BOOL Execute(DWORD time);
	void Terminate();
	void Draw();

	DWORD GetID(){return 'Win';}
	int GetDrawPriority(){return 1000;}

	void SetStoryText(char *txt);

protected:
	void CleanUp();

//�ϐ�
protected:
	DWORD counter;				//�J�E���^
	BOOL  m_end_flag;			//�I���t���O
	UINT  m_textlen;			//�\���e�L�X�g��
	DWORD wt;					//���������̃`�[��
	MYSURFACE *dds_face[3];		//�\����

	LPDIRECT3DTEXTURE8 tex_fb;	//�����I�����̃t�����g�o�b�t�@�̃R�s�[

	int f_x[3];					//face2�̕\���ʒu
	float f_z[3];				//face2�̕\���ʒu
	BOOL show_text;				//�e�L�X�g�\�����邩�ǂ���
	DWORD jun[3];				//�\���̏���

	char *m_text_from_story;	//�X�g�[���[������w�肳�ꂽ�e�L�X�g
	char *m_text;				//���܂���g_battleresult����擾�����e�L�X�g�Bdelete�̕K�v�Ȃ��B
};
