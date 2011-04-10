/*----------------------------------------------------------------------

	�|�[�Y���

------------------------------------------------------------------------*/

#include "task.h"
#include "dx_draw.h"

/*!
*	@brief �������|�[�Y�����Ƃ��̑��쥕`����s��
*	@ingroup Battle
*/
class CTBattlePause : public CTaskBase
{
public:
	void Initialize();						//Execute�܂���Draw���R�[�������O��1�x�����R�[�������
	void Terminate();						//�^�X�N�̃��X�g����O�����Ƃ��ɃR�[�������i���̒���Adelete�����j
	BOOL Execute(DWORD time);				//���t���[���R�[�������
	void Draw();							//�`�掞�ɃR�[�������
	int GetDrawPriority(){return 500;}		//�`��v���C�I���e�B�B�Ⴂ�قǎ�O�Ɂi��Ɂj�`��B�}�C�i�X�Ȃ�Ε\�����Ȃ�

	void Kill()	{m_kill_flag=TRUE;}
	void ChangeInst(UINT i);

protected:
	LPDIRECT3DTEXTURE8 tex_fb;				//!< �t�����g�o�b�t�@���R�s�[�����e�N�X�`��
	LPDIRECT3DTEXTURE8 tex_pause;			//!< "Pause Press F7 Key" �\���p�e�N�X�`��
	MYSURFACE* ms_inst[2];					//!< 1P/2P �C���X�gbmp

	UINT m_counter;
	UINT m_face_counter[2];
	UINT m_face_idx[2];
	BOOL m_kill_flag;
	BOOL m_inst_on[2];
};

