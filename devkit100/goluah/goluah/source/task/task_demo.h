/*!
*	@file
*	@brief �X�g�[���[���ԃf�� �^�X�N(ver0.5)
*
*	�����̃X�g�[���[���ԃf���X�N���v�g�̍Đ�
*/
#pragma once

/*!
*	@ingroup Tasks
*/
/*@{*/

//! �X�g�[���[���ԃf���E�P�u���b�N�����
struct DEMODAT
{
	char bitmap[4][64];	//!< �\���r�b�g�}�b�v�i�f�t�H���g�F�Ȃ��j
	BOOL bgmmid;		//!< ���g�p
	char bgm[64];		//!< �Đ�BGM�i�f�t�H���g�F�ύX�����j
	char wavsnd[64];	//!< �Đ��T�E���h�i�f�t�H���g�F�Ȃ��j//������
	char msg[1024];		//!< �\�����b�Z�[�W�i�f�t�H���g�F�Ȃ��j
	int spdmsg;			//!< ���b�Z�[�W�\���X�s�[�h�i�f�t�H���g�F�K���j
	int dur;			//!< �\�����ԁi�f�t�H���g�F������\���ɍ��킹�Ď����I�ɐݒ�j
	DWORD bmpdisp;		//!< ���t���O
	double bgmpos;		//!< BGM�J�n�ʒu
	int num_select;		//!< �I�����̐��i0=�I�����Ȃ��j
	int select_base;	//!< �I��ϐ����V�t�g�����
};

#define DEMOBMPDISP_MOVEL	0x00000001
#define DEMOBMPDISP_MOVER	0x00000002
#define DEMOBMPDISP_MOVEU	0x00000004
#define DEMOBMPDISP_MOVED	0x00000008
#define DEMOBMPDISP_YUREX	0x00000010
#define DEMOBMPDISP_YUREY	0x00000020
#define DEMOBMPDISP_FIXR	0x00000040
#define DEMOBMPDISP_FIXB	0x00000080

//! �X�g�[���[���ԃf��
class CTDemo : public CExclusiveTaskBase
{
public:
	CTDemo();
	~CTDemo();
	void Initialize();
	void Terminate();
	BOOL Execute(DWORD time);
	void Draw();
	int  GetDrawPriority(){return 1000;}
	DWORD GetID(){return 'Demo';}

	void Setup(UINT si,char *fn);
	void SetKeyIndex(UINT ki){m_keyindex=ki;}

private:
	BOOL Execute2();
	void CleanUp();
	int InitDemoDat(char *filepath);
	DWORD GetGyoDemo(char *strin,char *strout,DWORD *susumu);

//�ϐ�
public:
private:
	DEMODAT *demodat;
	char errc[256];
	DWORD counter;
	DWORD counter2;	//�e�L�X�g�����C���N�������g�p
	char fpath[256];
	MYSURFACE *ms;
	MYSURFACE* chr[3];
	DWORD num_demoscenes;
	BOOL demoready;

	UINT  m_keyindex;
	UINT  m_current_story_index;
	char* m_script_filename;
	char* m_current_dir;

	//���Ƃ��Ƃ�static
	int bmpw,bmph;
	DWORD playingdemodat;

	//���Ƃ���auto
	DWORD txtlen;
	BOOL  multibyte_flag;//�e�L�X�g��2�o�C�g�����������ꍇ�A���ɃE�F�C�g�C���^�[�o�������B���Ă�������ǉ����Ȃ�
	int yurex,yurey;
	RECT r_all;
	RECT r_serif;
	int select;
	int select_base;	// �ŏI�I�Ȃ��
};

/*@{*/
