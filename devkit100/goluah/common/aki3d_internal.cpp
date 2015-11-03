
#include "aki3d_internal.h"

/*******************************************************************

	�e�N�X�`���Ǘ��N���X

********************************************************************/

CTexManager *tex_man = NULL;

//�e�N�X�`���ǂݍ���
LPDIRECT3DTEXTURE9 CTexManager::LoadTexture(const char* filename)
{
	if(!filename)return NULL;
	LPDIRECT3DDEVICE9 d3ddev = aki3d.GetD3DDev();
	if(!d3ddev)return NULL;

	LPDIRECT3DTEXTURE9 ret=NULL;

	//�����̂��̂���T��
	for(UINT i=0;i<m_texList.size();i++){
		if( strcmp(m_texNameList[i],filename)==0 )
		{
			m_texRefcntList[i]++;
			return( m_texList[i] );
		}
	}

	//�t�@�C��������
	char *path = new char[MAX_PATH];
	if(m_texPath)
		sprintf(path,"%s\\%s",m_texPath,filename);
	else
		strcpy(path,filename);

	OutputDebugString("aki3d , load texture  ");
	OutputDebugString( path );
	OutputDebugString("\n");

	//�V�K�ɓǂݍ���
	HRESULT loadret = D3DXCreateTextureFromFileEx(d3ddev, path,
		0,0,0,0,
		D3DFMT_A1R5G5B5,
		D3DPOOL_MANAGED,D3DX_FILTER_POINT,D3DX_FILTER_POINT,
		0,NULL,NULL,&ret);
	delete [] path;

	//���X�g�ɒǉ�
	m_texList.push_back(ret);
	char* newTexName = new char [strlen(filename)+1];
	strcpy(newTexName,filename);
	m_texNameList.push_back(newTexName);
	m_texRefcntList.push_back(1);

	return ret;
}

//�e�N�X�`���̊J��
void CTexManager::UnloadTexture(LPDIRECT3DTEXTURE9 ptex)
{
	if(!ptex)return;

	for(UINT i=0;i<m_texList.size();i++)
	{
		if(m_texList[i]==ptex)
		{
			m_texRefcntList[i]--;
			if(m_texRefcntList[i]==0)
			{
				ptex->Release();
				m_texList.erase( m_texList.begin() + i );
				DELETEARRAY( m_texNameList[i] )
				m_texNameList.erase( m_texNameList.begin() + i );
				m_texRefcntList.erase( m_texRefcntList.begin() + i );
			}
			return;
		}
	}
}

//���ׂĔj��
void CTexManager::Destroy()
{
	//�e�N�X�`�����X�g�̔j��
	while(m_texList.size()!=0){
		std::vector<LPDIRECT3DTEXTURE9>::iterator itex = m_texList.begin();
		RELEASE((*itex));
		m_texList.erase(itex);
	}
	while(m_texNameList.size()!=0){
		std::vector<char*>::iterator iname = m_texNameList.begin();
		DELETEARRAY( (*iname) );
		m_texNameList.erase(iname);
	}
	m_texRefcntList.clear();

	DELETEARRAY(m_texPath);
}

//�x�[�X�p�X�ݒ�
void CTexManager::SetBasePath(const char *path)
{
	if(!path)return;
	if(!m_texPath){
		m_texPath = new char [MAX_PATH];
	}

	strcpy(m_texPath,path);
}



/*******************************************************************

	���X�g���b�v�`��N���X

********************************************************************/

/*------------------------------------------------------------------
	�\�z
--------------------------------------------------------------------*/
CTristripBody::CTristripBody()
{
	node_num = 0;
	rad = NULL;
	pos = NULL;
	col = NULL;
	vtemp = NULL;
	vsubs = NULL;
	norms = NULL;
	vb = NULL;
	tex = NULL;
}

/*------------------------------------------------------------------
	����
--------------------------------------------------------------------*/
void CTristripBody::Create(UINT num_node , const char* tex_filename)
{
	Destroy();
	if(num_node<2)return;

	tex = tex_man->LoadTexture(tex_filename);
	node_num = num_node;

	pos = new V3d[node_num];
	col = new ARGB[node_num];
	rad = new float[node_num];
	vb  = new MYVERTEX3D[node_num*2];

	vtemp = new V2d[node_num+1];
	vsubs = vtemp+2;
	norms = vtemp;

	SetupTexCoords();
}

/*------------------------------------------------------------------
	�j��
--------------------------------------------------------------------*/
void CTristripBody::Destroy()
{
	DELETEARRAY(pos);
	DELETEARRAY(col);
	DELETEARRAY(rad);
	DELETEARRAY(vb);
	DELETEARRAY(vtemp);
	vsubs=NULL;
	norms=NULL;

	node_num=0;

	tex_man->UnloadTexture(tex);
	tex=NULL;
}

/*------------------------------------------------------------------
	���W�l�v�Z & �`��
	(2D�v�Z)
--------------------------------------------------------------------*/
void CTristripBody::Render(D3DMATRIX *l2w)
{
	UINT i,j;

	LPDIRECT3DDEVICE9 d3ddev = aki3d.GetD3DDev();
	if(d3ddev==NULL)return;

	//���[�J�������[���h�}�g���N�X���w�肳��Ȃ������ꍇ�͒P�ʍs��ő�p����
	D3DXMATRIX mati;
	D3DXMatrixIdentity(&mati);
	if(!l2w)l2w = &mati;

	//�e�x�N�g���Ԃł�sub���Ƃ�A
	//�����90����]����( 2D��90����]�F(x',y')=(y,-x) )
	for(i=0;i<node_num-1;i++)
	{
		vsubs[i].x = (pos[i+1].y - pos[i].y);
		vsubs[i].y = (pos[i].x - pos[i+1].x);
		vsubs[i].Normalize();//�m�[�}���C�Y���Ă���
	}

	//�e�m�[�h�ł̖@�����ۂ����̂��o��
	norms[0] = vsubs[0];//�ŏ���1�_
	i=1;
	j=0;
	for(i=1;i<node_num-1;i++,j++){
		norms[i].x = vsubs[j].x + vsubs[j+1].x;
		norms[i].y = vsubs[j].y + vsubs[j+1].y;
	}
	norms[i]=vsubs[i-1];//�Ō��1�_

	//�@���m�[�}���C�Y
	for(i=0;i<node_num;i++){
		norms[i].Normalize();
	}

	for(i=0;i<node_num;i++)
	{
		//�F�ݒ�
		vb[i*2  ].color = vb[i*2+1].color = *((DWORD*)&col[i]);
		
		//�e���_���W��@�����ۂ����̕����ɐL�΂��`��p�̒��_�z��ɏ�������
		vb[i*2  ].x = pos[i].x + norms[i].x*rad[i]*0.5f;
		vb[i*2+1].x = pos[i].x - norms[i].x*rad[i]*0.5f;
		vb[i*2  ].y = pos[i].y + norms[i].y*rad[i]*0.5f;
		vb[i*2+1].y = pos[i].y - norms[i].y*rad[i]*0.5f;
		vb[i*2  ].z = pos[i].z  ;
		vb[i*2+1].z = pos[i].z  ;
	}

	//�`��
	try
	{
		//�`��O�ݒ�
		d3ddev->SetTexture(0,tex);						//�e�N�X�`���[�ݒ�
		d3ddev->SetTransform(D3DTS_WORLD,l2w);			//���W�ϊ��}�g���N�X�w��
		d3ddev->SetFVF(FVF_3DVERTEX);		//���_�̃t�H�[�}�b�g���w��

		//�`��
		if(D3D_OK != d3ddev->DrawPrimitiveUP(
								D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u���
								(node_num-1)*2,			//�v���~�e�B�u��
								vb,						//���_�z��
								sizeof(MYVERTEX3D)		//���_�X�g���C�h
						))
		{
			aki3d.LogWarning("%s failed to render",__FUNCTION__);
		}
	}
	catch(...)
	{
		zenka=TRUE;
		aki3d.LogWarning("%s catch exception while rendering",__FUNCTION__);
	}
}

/*------------------------------------------------------------------
	�e�N�X�`�����W�ݒ�
	u : �X�g���b�v�̗��[�� 0�`1
	v : �m�[�h��2���_�Ԃ� 0�`1
--------------------------------------------------------------------*/
void CTristripBody::SetupTexCoords()
{
	for(UINT i=0;i<node_num;i++){
		vb[i*2  ].tv = 0.0f;
		vb[i*2+1].tv = 1.0f;

		vb[i*2  ].tu = (float)i / (float)(node_num-1);
		vb[i*2+1].tu = vb[i*2  ].tu;
	}
}






/*******************************************************************

	�p�[�e�B�N���N���X����

********************************************************************/


/*------------------------------------------------------------------
	�\�z
--------------------------------------------------------------------*/
CParticleBody::CParticleBody()
{
	index_arr = NULL;

	pos = NULL;
	col = NULL;
	rad = NULL;
	vb  = NULL;
	index_arr = NULL;
	ax = NULL;

	#ifdef CParticleBody_3D
	tpos= NULL;
	tax= NULL;
	#endif

	tex = NULL;
	zenka = FALSE;
}

/*------------------------------------------------------------------
	����
--------------------------------------------------------------------*/
void CParticleBody::Create(UINT num_tubu , const char *tex_filename)
{
	Destroy();
	tex = tex_man->LoadTexture(tex_filename);

	num = num_tubu;

	if(num==0)return;

	pos = new V3d[num];
	col = new ARGB[num];
	rad = new float[num];
	ax  = new V3d[num];

	vb  = new MYVERTEX3D[num*4];
	index_arr = new WORD[num*6];

	SetupTexCoords();

	for(UINT i=0;i<num;i++)
	{
		//axis���[���N���A
		ax[i] = Zero;

		//�C���f�b�N�X�f�[�^����
		index_arr[i*6+0] = i*4;
		index_arr[i*6+1] = i*4+1;
		index_arr[i*6+2] = i*4+2;
		
		index_arr[i*6+3] = i*4+1;
		index_arr[i*6+4] = i*4+2;
		index_arr[i*6+5] = i*4+3;
	}

	#ifdef CParticleBody_3D
	tpos= new D3DXVECTOR4[num];
	tax= new D3DXVECTOR3[num];
	#endif
}

/*------------------------------------------------------------------
	�j��
--------------------------------------------------------------------*/
void CParticleBody::Destroy()
{
	DELETEARRAY(pos);
	DELETEARRAY(col);
	DELETEARRAY(rad);
	DELETEARRAY(ax);

	DELETEARRAY(vb);
	DELETEARRAY(index_arr);

	num=0;

	tex_man->UnloadTexture( tex );
	tex=NULL;

	#ifdef CParticleBody_3D
	DELETEARRAY(tpos);
	DELETEARRAY(tax)
	#endif
}

/*------------------------------------------------------------------
	���W�l�v�Z & �`��
--------------------------------------------------------------------*/
void CParticleBody::Render(D3DMATRIX *l2w)
{
	if(zenka)return;

	#ifndef CParticleBody_3D

	UINT i;

	LPDIRECT3DDEVICE8 d3ddev = aki3d.GetD3DDev();
	if(d3ddev==NULL)return;

	//���[�J�������[���h�ϊ��}�g���N�X���w�肳��Ă��Ȃ��ꍇ�P�ʍs��ő�p����
	D3DXMATRIX mati;
	D3DXMatrixIdentity(&mati);
	if(!l2w)l2w = &mati;

	for(i=0;i<num;i++){
		//�F�ݒ�
		vb[i*4  ].color = vb[i*4+1].color = 
		vb[i*4+2].color = vb[i*4+3].color = *((DWORD*)&col[i]);

		//�L�΂������x�N�g����xy���ʏ�Ɏʂ������̂����v�Z
		V2d eax;
		eax.x = ax[i].x;
		eax.y = ax[i].y;
		eax.SetLength(rad[i]*0.5f);//�̂΂�

		//��L�x�N�g���ɐ����Ȃ��̂��v�Z
		V2d vax = eax;
		vax.Verticalize();

		//pos�Ƀx�N�g���������`��p�̒��_�z��ɏ�������
		vb[i*4  ].x = pos[i].x + ax[i].x + eax.x + vax.x;
		vb[i*4+1].x = pos[i].x - ax[i].x - eax.x + vax.x;
		vb[i*4+2].x = pos[i].x + ax[i].x + eax.x - vax.x;
		vb[i*4+3].x = pos[i].x - ax[i].x - eax.x - vax.x;

		vb[i*4  ].y = pos[i].y + ax[i].y + eax.y + vax.y;
		vb[i*4+1].y = pos[i].y - ax[i].y - eax.y + vax.y;
		vb[i*4+2].y = pos[i].y + ax[i].y + eax.y - vax.y;
		vb[i*4+3].y = pos[i].y - ax[i].y - eax.y - vax.y;

		vb[i*2  ].z = pos[i].z + ax[i].z;
		vb[i*2+1].z = pos[i].z - ax[i].z;
		vb[i*2+2].z = pos[i].z + ax[i].z;
		vb[i*2+3].z = pos[i].z - ax[i].z;
	}

	//�`��
	{
		//�`��O�ݒ�
		d3ddev->SetTexture(0,tex);						//�e�N�X�`���[�ݒ�
		d3ddev->SetTransform(D3DTS_WORLD,l2w);			//���W�ϊ��}�g���N�X�w��
		d3ddev->SetFVF( FVF_3DVERTEX );		//���_�̃t�H�[�}�b�g���w��

		//�`��
		d3ddev->DrawIndexedPrimitiveUP(
									D3DPT_TRIANGLELIST,	//D3DPRIMITIVETYPE PrimitiveType,
									0,					//UINT MinVertexIndex,
									num*4,				//UINT NumVertexIndices,
									num*2,				//UINT PrimitiveCount,
									index_arr,			//const void *pIndexData,
									D3DFMT_INDEX16,		//D3DFORMAT IndexDataFormat,
									vb,					//CONST void* pVertexStreamZeroData,
									sizeof(MYVERTEX3D)	//UINT VertexStreamZeroStride
								);
	}

	#else

	//����ς���������Ȃ��Ⴞ�߁H

	UINT i;
	D3DXMATRIX mati;	// �P�ʍs��
	D3DXMATRIX pmat;	// �v���W�F�N�V�����s��
	D3DXMATRIX vmat;	// �r���[�s��
	D3DXMATRIX dmat;	// ���[�J�����W�n���f�B�X�v���C���W�n�ϊ��}�g���N�X
	D3DXMATRIX idmat;	// ���[�J�����W�n���f�B�X�v���C���W�n�ϊ��}�g���N�X

	LPDIRECT3DDEVICE9 d3ddev = aki3d.GetD3DDev();
	if(d3ddev==NULL)return;

	D3DXMatrixIdentity(&mati);
	if(!l2w)l2w = &mati;
	d3ddev->GetTransform(D3DTS_PROJECTION,&pmat);
	d3ddev->GetTransform(D3DTS_VIEW,&vmat);
	dmat = (*l2w);
	dmat *= vmat;
	dmat *= pmat;
	D3DXMatrixInverse(&idmat,NULL,&dmat);

	//�|�W�V������Axis���f�B�X�v���C���W�n��ɕϊ�����
	for(i=0;i<num;i++)
	{
		D3DXVec3Transform(&tpos[i],&pos[i],&dmat);
		D3DXVec3TransformNormal(&tax[i],&ax[i],&dmat);

		//w�Ń���
		tpos[i].x /= tpos[i].w;
		tpos[i].y /= tpos[i].w;
		if(tpos[i].w<0.0f){
			tpos[i].x *=-1.0f;
			tpos[i].y *=-1.0f;
		}
	}

	float nc = NEAR_CLIP;//dr->GetNearClip();
	float fc = FAR_CLIP;//dr->GetFarClip();

	float radius;
	for(i=0;i<num;i++){
		//�F�ݒ�
		vb[i*4  ].color = vb[i*4+1].color = 
		vb[i*4+2].color = vb[i*4+3].color = *((DWORD*)&col[i]);

		//�f�B�X�v���C��ł̑傫���v�Z
		if(tpos[i].z!=0)
			radius = rad[i]/tpos[i].z ;	//�����Ă邩�H
		else{
			radius = 1.0f;
		}
		V2d vax;
		vax.x = tax[i].x;
		vax.y = tax[i].y;
		vax.SetLength(radius);
		tax[i].x += vax.x;
		tax[i].y += vax.y;
		vax.Verticalize();
		//rad[i]);

		//�e���_���W����t�����`��p�̒��_�z��ɏ�������
		vb[i*4  ].x = tpos[i].x + tax[i].x + vax.x;
		vb[i*4+1].x = tpos[i].x - tax[i].x + vax.x;
		vb[i*4+2].x = tpos[i].x + tax[i].x - vax.x;
		vb[i*4+3].x = tpos[i].x - tax[i].x - vax.x;

		vb[i*4  ].y = tpos[i].y + tax[i].y + vax.y;
		vb[i*4+1].y = tpos[i].y - tax[i].y + vax.y;
		vb[i*4+2].y = tpos[i].y + tax[i].y - vax.y;
		vb[i*4+3].y = tpos[i].y - tax[i].y - vax.y;

		vb[i*2  ].z = tpos[i].z / tpos[i].w ;//+ tax[i].z;
		vb[i*2+1].z = tpos[i].z / tpos[i].w ;//- tax[i].z;
		vb[i*2+2].z = tpos[i].z / tpos[i].w ;//+ tax[i].z;
		vb[i*2+3].z = tpos[i].z / tpos[i].w ;//- tax[i].z;
	}

	//�`��
	{
		//�`��O�ݒ�
		d3ddev->SetTexture(0,tex);						//�e�N�X�`���[�ݒ�
		d3ddev->SetTransform(D3DTS_WORLD,	&mati);		//���W�ϊ��}�g���N�X�w��(���łɕϊ��ς݂Ȃ̂ł��Ȃ�)
		d3ddev->SetTransform(D3DTS_VIEW,	&mati);		//���W�ϊ��}�g���N�X�w��(���łɕϊ��ς݂Ȃ̂ł��Ȃ�)
		d3ddev->SetTransform(D3DTS_PROJECTION,&mati);	//���W�ϊ��}�g���N�X�w��(���łɕϊ��ς݂Ȃ̂ł��Ȃ�)
		d3ddev->SetFVF(FVF_3DVERTEX);		//���_�̃t�H�[�}�b�g���w��

		//�`��
		try{
		if(D3D_OK!=d3ddev->DrawIndexedPrimitiveUP(
									D3DPT_TRIANGLELIST,	//D3DPRIMITIVETYPE PrimitiveType,
									0,					//UINT MinVertexIndex,
									num*4,				//UINT NumVertexIndices,
									num*2,				//UINT PrimitiveCount,
									index_arr,			//const void *pIndexData,
									D3DFMT_INDEX16,		//D3DFORMAT IndexDataFormat,
									vb,					//CONST void* pVertexStreamZeroData,
									sizeof(MYVERTEX3D)	//UINT VertexStreamZeroStride
								))
		{
			aki3d.LogWarning("%s failed to render",__FUNCTION__);
		}
		}catch(...)
		{
			zenka=TRUE;
			aki3d.LogWarning("%s catch exception while rendering",__FUNCTION__);
		}

		//�}�g���N�X�ݒ�̃��X�g�A
		d3ddev->SetTransform(D3DTS_VIEW,&vmat);	
		d3ddev->SetTransform(D3DTS_PROJECTION,&pmat);
	}

	#endif//CParticleBody_3D
}

/*------------------------------------------------------------------
	�e�N�X�`�����W�ݒ�
--------------------------------------------------------------------*/
void CParticleBody::SetupTexCoords()
{
	for(UINT i=0;i<num;i++)
	{
		vb[i*4  ].tu = 0;
		vb[i*4+1].tu = 0;
		vb[i*4+2].tu = 1;
		vb[i*4+3].tu = 1;
		
		vb[i*4  ].tv = 0;
		vb[i*4+1].tv = 1;
		vb[i*4+2].tv = 0;
		vb[i*4+3].tv = 1;
	}
}



/*******************************************************************

	�t���b�g�p�[�e�B�N���`��N���X����

********************************************************************/

CFlatBoardsBody::CFlatBoardsBody()
{
	pos = NULL;
	ax1 = NULL;
	ax2 = NULL;
	col = NULL;
	vb  = NULL;
	index_arr = NULL;
	tex = NULL;

	zenka = FALSE;
}

void CFlatBoardsBody::Create(UINT num_node , const char* tex_filename)
{
	Destroy();

	if(num_node==0)return;
	num = num_node;

	pos = new V3d[num];
	ax1 = new V3d[num];
	ax2 = new V3d[num];
	col = new ARGB[num];
	vb  = new MYVERTEX3D[num*5];
	index_arr = new WORD[num*12];

	tex = aki3d.LoadTexture( tex_filename );

	UINT i=0;

	//index
	for(i=0;i<num;i++)
	{
		index_arr[i*12+0] = i*5+0;
		index_arr[i*12+1] = i*5+1;
		index_arr[i*12+2] = i*5+2;

		index_arr[i*12+3] = i*5+0;
		index_arr[i*12+4] = i*5+2;
		index_arr[i*12+5] = i*5+4;
		
		index_arr[i*12+6] = i*5+0;
		index_arr[i*12+7] = i*5+4;
		index_arr[i*12+8] = i*5+3;
		
		index_arr[i*12+9]  = i*5+0;
		index_arr[i*12+10] = i*5+3;
		index_arr[i*12+11] = i*5+1;
	}

	//uv
	for(i=0;i<num;i++)
	{
		vb[i*5+0].tu = 0.5f;
		vb[i*5+0].tv = 0.5f;

		vb[i*5+1].tu = 0.0f;
		vb[i*5+1].tv = 0.0f;

		vb[i*5+2].tu = 0.0f;
		vb[i*5+2].tv = 1.0f;
		
		vb[i*5+3].tu = 1.0f;
		vb[i*5+3].tv = 0.0f;
		
		vb[i*5+4].tu = 1.0f;
		vb[i*5+4].tv = 1.0f;
	}
}

void CFlatBoardsBody::Destroy()
{
	DELETEARRAY(pos);
	DELETEARRAY(ax1);
	DELETEARRAY(ax2);
	DELETEARRAY(col);
	DELETEARRAY(vb);
	DELETEARRAY(index_arr);

	aki3d.UnloadTexture(tex);
	tex = NULL;

	num = 0;
}

void CFlatBoardsBody::Render(D3DMATRIX *l2w)
{
	if(zenka)return;
	UINT i;

	for(i=0;i<num;i++)
	{
		//vertex
		vb[i*5+0].x = pos[i].x ;
		vb[i*5+0].y = pos[i].y ;
		vb[i*5+0].z = pos[i].z ;

		vb[i*5+1].x = pos[i].x + ax2[i].x + ax1[i].x;
		vb[i*5+1].y = pos[i].y + ax2[i].y + ax1[i].y;
		vb[i*5+1].z = pos[i].z + ax2[i].z + ax1[i].z;
		
		vb[i*5+2].x = pos[i].x + ax2[i].x - ax1[i].x;
		vb[i*5+2].y = pos[i].y + ax2[i].y - ax1[i].y;
		vb[i*5+2].z = pos[i].z + ax2[i].z - ax1[i].z;
		
		vb[i*5+3].x = pos[i].x - ax2[i].x + ax1[i].x;
		vb[i*5+3].y = pos[i].y - ax2[i].y + ax1[i].y;
		vb[i*5+3].z = pos[i].z - ax2[i].z + ax1[i].z;
		
		vb[i*5+4].x = pos[i].x - ax2[i].x - ax1[i].x;
		vb[i*5+4].y = pos[i].y - ax2[i].y - ax1[i].y;
		vb[i*5+4].z = pos[i].z - ax2[i].z - ax1[i].z;

		//color setting
		vb[i*5+0].color = col[i].dwcol;
		vb[i*5+1].color = col[i].dwcol;
		vb[i*5+2].color = col[i].dwcol;
		vb[i*5+3].color = col[i].dwcol;
		vb[i*5+4].color = col[i].dwcol;
	}

	LPDIRECT3DDEVICE9 d3ddev = aki3d.GetD3DDev();
	
	D3DXMATRIX mat;
	if(!l2w)
	{
		l2w = D3DXMatrixIdentity(&mat);
	}

	d3ddev->SetTexture(0,tex);						//�e�N�X�`���[�ݒ�
	d3ddev->SetTransform(D3DTS_WORLD,l2w);			//���W�ϊ��}�g���N�X�w��
	d3ddev->SetFVF(FVF_3DVERTEX);		//���_�̃t�H�[�}�b�g���w��

	//�`��
	try{
	if(D3D_OK!=d3ddev->DrawIndexedPrimitiveUP(
								D3DPT_TRIANGLELIST,	//D3DPRIMITIVETYPE PrimitiveType,
								0,					//UINT MinVertexIndex,
								num*12,				//UINT NumVertexIndices,
								num*4,				//UINT PrimitiveCount,
								index_arr,			//const void *pIndexData,
								D3DFMT_INDEX16,		//D3DFORMAT IndexDataFormat,
								vb,					//CONST void* pVertexStreamZeroData,
								sizeof(MYVERTEX3D)	//UINT VertexStreamZeroStride
							))
		{
			aki3d.LogWarning("%s failed to render",__FUNCTION__);
		}
	}
	catch(...)
	{
		zenka=TRUE;
		aki3d.LogWarning("%s catch exception while rendering",__FUNCTION__);
	}
}






/*******************************************************************

	���b�V���N���X����

********************************************************************/


/*------------------------------------------------------------------
	�\�z
--------------------------------------------------------------------*/
CMeshBody::CMeshBody()
{
	rad = 1.0f;
	node_num = 0;
	pos = V3d(0, 0, 0);
	rot = V3d(0, 0, 0);
	x_mesh = NULL;
	ldir = V3d(-5.0f, 8.0f, 10.0f);
	amb = 0x00606060;
	isspecular = TRUE;

	mat = NULL;
	tex = NULL;
	zenka = FALSE;
}

/*------------------------------------------------------------------
	����
--------------------------------------------------------------------*/
void CMeshBody::Create(const char* x_filename)
{
	if(!x_filename)return;
	LPDIRECT3DDEVICE9 d3ddev = aki3d.GetD3DDev();
	if(!d3ddev)return;

	Destroy();
	HRESULT hr;
	LPD3DXBUFFER mtrbuf = NULL;

	// �ǂݍ���
	hr = D3DXLoadMeshFromX((LPSTR)x_filename,
		D3DXMESH_MANAGED,
		d3ddev,
		NULL,
		&mtrbuf,
		NULL,
		&node_num,
		&x_mesh);

	if (FAILED(hr))		// �ٽ�
		return;

	// �}�e���A���ݒ�
	LPD3DXMATERIAL mater = (LPD3DXMATERIAL)mtrbuf->GetBufferPointer();
	mat = new D3DMATERIAL9[node_num];

	// �e�N�X�`����
	tex = new LPDIRECT3DTEXTURE9[node_num];

	for (int i = 0; i < (int)node_num; i++)
	{
		mat[i] = mater[i].MatD3D;
		mat[i].Ambient = mat[i].Diffuse;
		tex[i] = tex_man->LoadTexture(mater[i].pTextureFilename);
	}

	RELEASE(mtrbuf); // ���ڂ��
}

/*------------------------------------------------------------------
	�j��
--------------------------------------------------------------------*/
void CMeshBody::Destroy()
{
	for (int i = 0; i < (int)node_num; i++)
		tex_man->UnloadTexture(tex[i]);
	DELETEARRAY(tex);
	DELETEARRAY(mat);
	RELEASE(x_mesh);
}

/*------------------------------------------------------------------
	���W�l�v�Z & �`��
	(2D�v�Z)
--------------------------------------------------------------------*/
void CMeshBody::Render(D3DMATRIX *l2w)
{
	if(zenka)return;
	UINT i;

	LPDIRECT3DDEVICE9 d3ddev = aki3d.GetD3DDev();
	if(d3ddev==NULL)return;
	if (x_mesh == NULL) return;

	//���[�J�������[���h�}�g���N�X���w�肳��Ȃ������ꍇ�͒P�ʍs��ő�p����
	D3DXMATRIX mati = aki3d.CreateMatrix(&V3d(rad, rad * -1.0f, rad), &rot, &pos);	// Y���𔽓]����
	if(l2w)mati *= *l2w;

	D3DLIGHT9 light;

	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Specular = light.Diffuse;		// �X�y�L�����F���ݒ肵�Ƃ�
	D3DXVec3Normalize((V3d*)&light.Direction, &ldir);
	light.Range = 1000.0f;

	//�`��
	try
	{
		D3DMATERIAL9 old;
		
		//�`��O�ݒ�
		d3ddev->SetTransform(D3DTS_WORLD,&mati);			//���W�ϊ��}�g���N�X�w��
		d3ddev->SetFVF(x_mesh->GetFVF());
		d3ddev->GetMaterial(&old);
		d3ddev->SetLight(0, &light);
		d3ddev->LightEnable(0, TRUE);
		d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);
		d3ddev->SetRenderState(D3DRS_SPECULARENABLE, isspecular);
		d3ddev->SetRenderState( D3DRS_AMBIENT, amb );

		//�`��
		for (i = 0; i < (int)node_num; i++)
		{
			d3ddev->SetMaterial(&mat[i]);
			d3ddev->SetTexture(0,tex[i]);						//�e�N�X�`���[�ݒ�
			if(D3D_OK != x_mesh->DrawSubset(i))
			{
				aki3d.LogWarning("%s failed to render",__FUNCTION__);
			}
		}
 
		// ���ɖ߂��Ƃ�
		d3ddev->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
		d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
		d3ddev->SetMaterial(&old);
		d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		d3ddev->SetFVF(FVF_3DVERTEX);
	}
	catch(...)
	{
		zenka=TRUE;
		aki3d.LogWarning("%s catch exception while rendering",__FUNCTION__);
	}
}


