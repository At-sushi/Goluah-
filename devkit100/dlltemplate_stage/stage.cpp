

#include "stage.h"
#include <math.h>


SET_STAGEINFO_VERCHK(CStage, STB_VERSION)


/*!
*	@brief �R���X�g���N�^
*/
CStage::CStage(SDI_STAGEINFO2 *info)
			: CStageBase(info)
{
	//�ϐ��l�̏�������
}

/*!
*	@brief �f�X�g���N�^
*/
CStage::~CStage()
{
	//���I�m�ۃI�u�W�F�N�g�̔j��������
}

/*!
*	@brief ������
*/
void CStage::InitializeStage()
{
	//�C���[�W�̃��[�h���A�����������S��
}

/*!
*	@brief �`��
*/
DWORD CStage::DrawBack()
{
	//�`�揈��

	return FALSE;//�ʏ�A�f�t�H���g�`��͍s��Ȃ�
}
