// Goluah!! ����DLL�p�C���^�[�t�F�[�X�Ƃ����̂ق�

// �v����ɏ������z�֐��Ȗ󂾂�
#define IF(a)	virtual a = 0

class ISystem
{
	//�T�[�r�X
	IF(double DrawBMPText(double x,double y,float z,char *str,DWORD col));	//!< �r�b�g�}�b�v�e�L�X�g��`��
	IF(double DrawBMPTextEx(double x,double y,float z,char *str,			//!< �r�b�g�}�b�v�e�L�X�g��`��(�g�k�E�v���|�[�V���i���I�v�V������)
		DWORD col,float scaX,float scaY,DWORD flags));

	//fps
	IF(DWORD GetFPS());														//!< ���ۃt���[�����[�g�擾

	//�^�X�N
	IF(void AddTask(CTaskBase* task));										//!< �V�K�^�X�N�ǉ�
	IF(CExclusiveTaskBase* GetCurrentMainTask());							//!< ���݂̔r���^�X�N���擾

	//�V�X�e���O���t�B�b�N���擾
	IF(MYSURFACE** GetSystemGraphicSurface());								//!< �V�X�e���Ƃ��ĕێ�����Ă���GCD�p�̃r�b�g�}�b�v�z����擾
	IF(GCD_RECT* GetSystemGraphicRect());									//!< �V�X�e���Ƃ��ĕێ�����Ă���GCD�̋�`�z����擾
	IF(GCD_CELL2* GetSystemGraphicCell());									//!< �V�X�e���Ƃ��ĕێ�����Ă���GCD�̃Z���z����擾
	IF(GCD_HANTEI* GetSystemGraphicHantei());								//!< �V�X�e���Ƃ��ĕێ�����Ă���GCD�̂����蔻���`�z����擾
};

class IDraw
{

	//�����̑��`��
	//���C���`��
	IF(void DrawLine(int sx,int sy,int gx,int gy,DWORD col));
	//�o�b�N�o�b�t�@�̃N���A
	IF(void ClearBackBuffer());
	//�~�̕`��
	IF(void DrawCircle(int x,int y,int rad,int w,
		float z=0,DWORD color=0xFF55AAFF,BOOL toumei=FALSE,float rot=0,BOOL rot_y=TRUE,BOOL hosei=TRUE));
	//�Ȃɂ���H
	IF(void Draw3DText());
	//�e�L�X�g�`��
	IF(void DrawBlueText(RECT& r,char *text,int len,DWORD method,DWORD size));
	IF(void DrawRedText(RECT& r,char *text,int len,DWORD method,DWORD size));

	//���֗��֐��H
	IF(void SetAlphaMode(DWORD alphamode));//���u�����h�̃��[�h��ݒ�
	IF(void EnableZ(BOOL test=TRUE,BOOL write=TRUE));//Z�e�X�g�E���C�g�̗L���E������ݒ�

	//�����W�ϊ��}�g���N�X�֘A
	//for directx8
	IF(void ResetTransformMatrix());//view & projection
	IF(void SetTransform(BOOL b));
	IF(void SetWorldMatrix(D3DXMATRIX *pmat));
};

__interface IBattleTask
{
};
