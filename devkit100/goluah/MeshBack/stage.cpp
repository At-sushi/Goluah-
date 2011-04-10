

#include "stage.h"
#include <math.h>


SET_STAGEINFO(CStage)
#define APPNAME		"MeshBack"


/*!
*	@brief �R���X�g���N�^
*/
CStage::CStage(SDI_STAGEINFO2 *info)
			: CStageBase(info)
{
	//�ϐ��l�̏�������
	aki3d.Initialize(GetD3DDevice(), info->dir);
	back = NULL;
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
	// ���b�X���
	back = aki3d.CreateMeshObject();

	// back.x�����[�h
	char buf[256], buf2[16];

	sprintf(buf, "%s\\back.x", this->GetCharDir());
	back->Create(buf);

	// �ݒ�ǂݍ���
	union {
		int amb;
		BOOL spec;
	};
	double ldir_x = 0.0, ldir_y = 0.0, ldir_z = 0.0;

	sprintf(buf, "%s\\stage.ini", this->GetCharDir());

	amb = GetPrivateProfileInt(APPNAME, "Ambient", 60, buf) & 0xFF;
	back->SetAmbient(amb | (amb<<8) | (amb<<16));

	GetPrivateProfileString(APPNAME, "LightDirect", "-5.0,8.0,10.0", buf2, 16, buf);
	sscanf(buf2, "%lf,%lf,%lf", &ldir_x, &ldir_y, &ldir_z);
	back->SetLightDir(V3d(ldir_x, ldir_y, ldir_z));

	spec = GetPrivateProfileInt(APPNAME, "SpecularEnable", 1, buf);
	back->SetSpecular(spec);
}

/*!
*	@brief �`��
*/
DWORD CStage::DrawBack()
{
	//�`�揈��
	// ���b�V���`��
	back->Render();

	return FALSE;//�ʏ�A�f�t�H���g�`��͍s��Ȃ�
}
