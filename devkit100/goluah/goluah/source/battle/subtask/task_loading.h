/*----------------------------------------------------------------------

	���[�h���

------------------------------------------------------------------------*/

#pragma once

#include "dx_draw.h"
#include "task.h"

enum NowLoading_IconItem
{
	NowLoading_DLL,
	NowLoading_Image,
	NowLoading_GCD,
};

/*!
*	@brief �����ONowLoading�N���X
*	@ingroup Battle
*
*	DLL���ɒ񋟂���Ă���, BMP,GCD�ǂݍ��݊֐����Ăяo���ꂽ�Ƃ���
*	�퓬�^�X�N��DLL�����[�h�����Ƃ���Proceed���Ă΂�A�`�悪1�X�e�b�v�����s����B
*	���̂Ƃ����C�����[�v�͉���Ă��Ȃ��̂ŁA�`��̊J�n�ƏI���̏��������O��Proceed�֐����ōs���Ă���B
*/
class CTNowLoading : public CTaskBase
{
public:
	void Initialize();						//Execute�܂���Draw���R�[�������O��1�x�����R�[�������
	BOOL Execute(DWORD time){return(TRUE);}	//���t���[���R�[�������
	void Terminate();						//�^�X�N�̃��X�g����O�����Ƃ��ɃR�[�������i���̒���Adelete�����j
	void Draw(){}							//!<Proceed()���ŁA��W���̎葱���ŕ`�悷��̂ŁA�����ł͕`����s��Ȃ�

	DWORD GetID(){return 'LOAD';}			//0�ȊO��Ԃ��悤�ɂ����ꍇ�A�}�l�[�W����Add�����Ƃ�����ID�����^�X�N��j�����܂��i��ɏ풓�^�X�N�p�j
	int GetDrawPriority(){return -1;}		//!<Proceed()���ŁA��W���̎葱���ŕ`�悷��̂ŁA�`����s��Ȃ�
	
	//!�`���1�X�e�b�v�����i�߂�
	void Proceed(NowLoading_IconItem item);
	
	//!�v���O���X�o�[�\��
	void Progress(NowLoading_IconItem item, float value);

protected:
	void DrawIcon();						//!���[�h�ς݂̃A�C�R����\��

	LPDIRECT3DTEXTURE8 tex_fb;				//!< �t�����g�o�b�t�@���R�s�[�����e�N�X�`��
	LPDIRECT3DTEXTURE8 tex_nowload;			//!< "Now Loading"�\���p�e�N�X�`��
	LPDIRECT3DTEXTURE8 tex_dll;				//!< DLL�A�C�R���\���p�e�N�X�`��
	LPDIRECT3DTEXTURE8 tex_img;				//!< IMG�A�C�R���\���p�e�N�X�`��
	LPDIRECT3DTEXTURE8 tex_gcd;				//!< GCD�A�C�R���\���p�e�N�X�`��
	MYVERTEX3D vb[4];						//!< �e�N�X�`���`��p�̒��_���X�g
	LPDIRECT3DTEXTURE8 tex_AA;				//!< �`�`�\���p�e�N�X�`��
	

	std::vector<NowLoading_IconItem> m_iconlist;
};