
/*========================================================
	
	������`

==========================================================*/
#pragma once
#include "aki3d.h"

/*--------------------------------------------------------
	�e�N�X�`���Ǘ�
----------------------------------------------------------*/
class CTexManager
{
public:
	CTexManager(){ m_texPath=NULL; }
	~CTexManager(){Destroy();}
	void SetBasePath(const char *path);

	LPDIRECT3DTEXTURE8 LoadTexture(const char *filename);
	void UnloadTexture(LPDIRECT3DTEXTURE8 ptex);

protected:
	void Destroy();

	char*							m_texPath;
	std::vector<LPDIRECT3DTEXTURE8>	m_texList;
	std::vector<char*>				m_texNameList;
	std::vector<UINT>				m_texRefcntList;
};

extern CTexManager* tex_man;


/*--------------------------------------------------------
	T�X�g���b�v����
----------------------------------------------------------*/
class CTristripBody : public CTristrip
{
public:
	CTristripBody();
	~CTristripBody(){Destroy();}

	void Create(UINT num_node , const char* tex_filename);
	void Destroy();
	void Render(D3DMATRIX *ltm = NULL);

	void SetWidth(UINT index,float value)	{if(index<node_num)rad[index]=value*2.0f;}
	void SetPos(UINT index,V3d& value)		{if(index<node_num)pos[index]=value;}
	void SetColor(UINT index,ARGB value)	{if(index<node_num)col[index]=value;}
	void SetColorDW(UINT index,DWORD value)	{if(index<node_num)col[index].dwcol=value;}

protected:
	virtual void SetupTexCoords();	//Create���Ƀe�N�X�`�����W��ݒ肷��B

	UINT				node_num;	//�m�[�h(��)��
	float				*rad;		//�߂̑���
	V3d					*pos;		//�߂̈ʒu
	ARGB				*col;		//�߂̂̐F

	V2d					*vtemp;		//�e���|�������_�o�b�t�@�B�ȉ��̃o�b�t�@�͂�����I�t�Z�b�g���Ďg���̂ŁAdelete���Ȃ�����
	V2d					*vsubs;		//�e�m�[�h�Ԃ�sub�x�N�g���Ɛ����ȃx�N�g��
	V2d					*norms;		//�e�m�[�h�̖@���݂����Ȃ���
	MYVERTEX3D			*vb;		//�`��p���_�o�b�t�@

	LPDIRECT3DTEXTURE8	tex;		//�e�N�X�`��

	BOOL				zenka;		//�O��
};

/*--------------------------------------------------------
	�p�[�e�B�N�� ����
----------------------------------------------------------*/
#define CParticleBody_3D
class CParticleBody : public CParticle
{
public:
	CParticleBody();
	~CParticleBody(){Destroy();}

	void Create(UINT num_node , const char* tex_filename);
	void Destroy();
	void Render(D3DMATRIX *ltm = NULL);

	void SetWidth(UINT index,float value)	{if(index<num)rad[index]=value;}
	void SetPos(UINT index,V3d& value)		{if(index<num)pos[index]=value;}
	void SetColor(UINT index,ARGB value)	{if(index<num)col[index]=value;}
	void SetColorDW(UINT index,DWORD value)	{if(index<num)col[index].dwcol=value;}
	void SetAxis(UINT index,V3d& value)		{if(index<num)ax[index] =value;}

protected:
	virtual void SetupTexCoords();	//Create���Ƀe�N�X�`�����W��ݒ肷��B

	UINT				num;		//���q��
	V3d					*pos;		//�ʒu
	ARGB				*col;		//�F
	float				*rad;		//�傫��
	V3d					*ax;		//�̂΂���

	MYVERTEX3D			*vb;		//�`��p���_�o�b�t�@
	WORD				*index_arr;	//���_�C���f�b�N�X���X�g

	LPDIRECT3DTEXTURE8	tex;		//�e�N�X�`��
	
	#ifdef CParticleBody_3D
	D3DXVECTOR4			*tpos;		//���W�ϊ��ςݒ��_
	D3DXVECTOR3			*tax;		//���W�ϊ��ς݃x�N�g��
	#endif
	
	BOOL				zenka;		//�O��
};


/*--------------------------------------------------------
	�t���b�g�p�[�e�B�N�� ����
----------------------------------------------------------*/
class CFlatBoardsBody : public CFlatBoards
{
public:
	CFlatBoardsBody();
	~CFlatBoardsBody(){Destroy();}
	
	void Create(UINT num_node , const char* tex_filename);
	void Destroy();
	void Render(D3DMATRIX *ltm = NULL);

	void	SetPos(UINT index,V3d value)		{if(index<num)pos[index]=value;}
	void	SetAxis1(UINT index,V3d value)		{if(index<num)ax1[index]=value;}
	void	SetAxis2(UINT index,V3d value)		{if(index<num)ax2[index]=value;}
	void	SetColor(UINT index,ARGB value)		{if(index<num)col[index]=value;}
	void	SetColorDW(UINT index,DWORD value)	{if(index<num)col[index].dwcol=value;}

protected:
	UINT		num;		//���q��
	V3d			*pos;		//�ʒu
	V3d			*ax1;		//�W�N1
	V3d			*ax2;		//�W�N2
	ARGB		*col;		//�F
	MYVERTEX3D	*vb;		//�`��p���_�o�b�t�@
	WORD		*index_arr;	//���_�C���f�b�N�X���X�g
	LPDIRECT3DTEXTURE8 tex;	//�e�N�X�`��
	
	BOOL		zenka;		//�O��
};


/*--------------------------------------------------------
	���b�V������
----------------------------------------------------------*/
class CMeshBody : public CMesh
{
public:
	CMeshBody();
	~CMeshBody(){Destroy();}

	void Create(const char* x_filename);
	void Destroy();
	void Render(D3DMATRIX *ltm = NULL);

	void SetWidth(float value)	{rad=value;}
	void SetPos(V3d& value)		{pos=value;}
	void SetRot(V3d& value)		{rot=value;}
	/*void SetColor(UINT index,ARGB value)	{if(index<node_num)col[index]=value;}
	void SetColorDW(UINT index,DWORD value)	{if(index<node_num)col[index].dwcol=value;}*/
	void SetLightDir(V3d& value)		{ldir=value;}
	void SetAmbient(D3DCOLOR value)		{amb=value;}
	void SetSpecular(BOOL value)		{isspecular=value;}

protected:
	DWORD				node_num;	//�m�[�h(��)��
	float				rad;		//�߂̑���
	V3d					pos;		//�߂̈ʒu
	V3d					rot;		//��]
	/*ARGB				*col;		//�߂̂̐F

	V2d					*vtemp;		//�e���|�������_�o�b�t�@�B�ȉ��̃o�b�t�@�͂�����I�t�Z�b�g���Ďg���̂ŁAdelete���Ȃ�����
	V2d					*vsubs;		//�e�m�[�h�Ԃ�sub�x�N�g���Ɛ����ȃx�N�g��
	V2d					*norms;		//�e�m�[�h�̖@���݂����Ȃ���*/
	ID3DXMesh			*x_mesh;	//�`��p���_�o�b�t�@
	V3d					ldir;		//���C�g�̌���
	D3DCOLOR			amb;		//����
	BOOL				isspecular;	//���ˌ����g�����ǂ���

	D3DMATERIAL8		*mat;		//�}�e���A���i�z��j
	LPDIRECT3DTEXTURE8	*tex;		//�e�N�X�`���i�z��j

	BOOL				zenka;		//�O��
};

