

#include "stage.h"
#include <math.h>


SET_STAGEINFO(CStage)


/*!
*	@brief �R���X�g���N�^
*/
CStage::CStage(SDI_STAGEINFO2 *info)
			: CStageBase(info)
{
	bitmap = NULL;
}

/*!
*	@brief �f�X�g���N�^
*	�ǂݍ���image1.bmp��j������
*/
CStage::~CStage()
{
	UnloadBitMap(bitmap);
}

/*!
*	@brief ������
*	image1.bmp��ǂݍ��ށB
*/
void CStage::InitializeStage()
{
	//image1.bmp�����[�h
	char filename[256];
	sprintf(filename,"%s\\back1",GetDLLPath());
	bitmap = LoadImage(filename,NULL);
}

/*!
*	@brief �`��
*	image1.bmp���X�e�[�W�S�̂ɕ`�悷��
*/
DWORD CStage::DrawBack()
{
	if(bitmap!=NULL){

		//�]�����E�r�b�g�}�b�v�S��
		RECT src;
		src.left = src.top = 0;
		src.right = (long)bitmap->wg;
		src.bottom= (long)bitmap->hg;

		//�]����
		MYRECT3D dst;
		if(bitmap->wg!=640)
			dst.z = 640.0f/(bitmap->wg-640)-1.0f;
		dst.z = 0.03f;
		dst.left  =-(dst.z+2.0f)*(640.0f/480.0f);
		dst.right = (dst.z+2.0f)*(640.0f/480.0f);
		dst.top   = (-1.0f - (float)tan(40.0/180.0*3.1415926))*(1.0f+dst.z);
		dst.bottom= ( 1.0f - (float)tan(40.0/180.0*3.1415926))*(1.0f+dst.z);

		//�`��
		Blt3D(bitmap,src,dst,pdat->color);
	}

	return FALSE;//�f�t�H���g�`��͍s��Ȃ�
}
