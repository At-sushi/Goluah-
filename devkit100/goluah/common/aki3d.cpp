
/*========================================================
	
	���ʒ�`

==========================================================*/

#include "aki3d.h"
#include "aki3d_internal.h"

std::vector<CTristripBody*> g_tstrip_list;
std::vector<CParticleBody*> g_particle_list;
std::vector<CFlatBoardsBody*> g_flatboards_list;
std::vector<CMeshBody*> g_mesh_list;

Aki3d aki3d;


/**********************************************************

	akieffect

***********************************************************/

/*---------------------------------------------------------
	�\�z
-----------------------------------------------------------*/
void Aki3d::Initialize(
			 LPDIRECT3DDEVICE9 d3d_device,
			 const char* tex_path,
			 WarningReportFunctionPtr pwf
			 )
{
	if(d3ddev)return;

	//Destroy();
	d3ddev = d3d_device;
	warning_report = pwf;

	tex_man = new CTexManager;
	tex_man->SetBasePath( tex_path );

	//���̏�����
	Zero.x = Zero.y = Zero.z = 0;
	Xaxis = Zero;
	Xaxis.x = 1.0f;
	Yaxis = Zero;
	Yaxis.y = 1.0f;
	Zaxis = Zero;
	Zaxis.z = 1.0f;

	// ���O�ɃX�^�b�N�p�z��𗿗����Ă���
	/*g_tstrip_list.clear();
	g_particle_list.clear();
	g_flatboards_list.clear();
	g_mesh_list.clear();*/
}

/*---------------------------------------------------------
	0�`1 ���������_��
-----------------------------------------------------------*/
float Aki3d::RandomOne()
{
	return ((float)(rand()%1000))*0.001f;
}

/*---------------------------------------------------------
	-1�`1 ���������_��
-----------------------------------------------------------*/
float Aki3d::RandomOne2()
{
	return RandomOne()*2.0f-1.0f;
}

/*---------------------------------------------------------
	0�`1 �ϊ��n
-----------------------------------------------------------*/
float Aki3d::zo_tri(float t,float mid)
{
	if(t<mid)return t/mid;
	else return 1.0f-(t-mid)/(1.0f-mid);
}


/*---------------------------------------------------------
	�W�N�Ƃ�
-----------------------------------------------------------*/
V3d Xaxis;
V3d Yaxis;
V3d Zaxis;
V3d Zero;

/*---------------------------------------------------------
	�T�[�r�X
-----------------------------------------------------------*/

//��Z�o�b�t�@ ON/OFF
void Aki3d::EnableZ(BOOL t,BOOL w)
{
	if(t)
		d3ddev->SetRenderState(D3DRS_ZFUNC		,D3DCMP_LESSEQUAL);
	else
		d3ddev->SetRenderState(D3DRS_ZFUNC		,D3DCMP_ALWAYS );
	d3ddev->SetRenderState(D3DRS_ZWRITEENABLE	,w);
}

//�����������[�h�ݒ�A���Z����
void Aki3d::SetBlend_Add()
{
	d3ddev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	d3ddev->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
}

//�����������[�h�ݒ�A�ʏ피����
void Aki3d::SetBlend_Normal()
{
	d3ddev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	d3ddev->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
}

//�����������[�h�ݒ�A���]
void Aki3d::SetBlend_Nega()
{
	d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
	d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
}



/*---------------------------------------------------------
	�I�u�W�F�N�g�̎擾
-----------------------------------------------------------*/
CTristrip* Aki3d::CreateTristripObject()
{
	OutputDebugString(_T("Aki3d::CreateTristripObject\n"));

	if(!tex_man)return NULL;

	g_tstrip_list.resize(g_tstrip_list.size() + 1, new CTristripBody);
	return g_tstrip_list[g_tstrip_list.size() - 1];
}

CParticle* Aki3d::CreateParticleObject()
{
	if(!tex_man)return NULL;

	g_particle_list.resize(g_particle_list.size() + 1, new CParticleBody);
	return g_particle_list[g_particle_list.size() - 1];
}

CFlatBoards* Aki3d::CreateFlatBoardsObject()
{
	if(!tex_man)return NULL;

	g_flatboards_list.resize(g_flatboards_list.size() + 1, new CFlatBoardsBody);
	return g_flatboards_list[g_flatboards_list.size() - 1];
}

CMesh* Aki3d::CreateMeshObject()
{
	OutputDebugString(_T("Aki3d::CreateMeshObject\n"));

	if(!tex_man)return NULL;

	g_mesh_list.resize(g_mesh_list.size() + 1, new CMeshBody);
	return g_mesh_list[g_mesh_list.size() - 1];
}

/*---------------------------------------------------------
	�I�u�W�F�N�g�̔j��
-----------------------------------------------------------*/
void Aki3d::DestroyTristripObject(CTristrip* obj)
{
	std::vector<CTristripBody*>::iterator i  = g_tstrip_list.begin();
	std::vector<CTristripBody*>::iterator ie = g_tstrip_list.end();

	for(;i!=ie;i++)
	{
		if((*i)==obj)
		{
			delete (*i);
			g_tstrip_list.erase(i);
			return;
		}
	}

	OutputDebugString(_T("[���o�C] Aki3d::DestroyTristripObject , �����炸\n"));
}

void Aki3d::DestroyParticleObject(CParticle* obj)
{
	std::vector<CParticleBody*>::iterator i  = g_particle_list.begin();
	std::vector<CParticleBody*>::iterator ie = g_particle_list.end();

	for(;i!=ie;i++)
	{
		if((*i)==obj)
		{
			delete (*i);
			g_particle_list.erase(i);
			return;
		}
	}

	OutputDebugString(_T("[���o�C] Aki3d::DestroyParticleObject , �����炸\n"));
}

void Aki3d::DestroyFlatBoardsObject(CFlatBoards* obj)
{
	std::vector<CFlatBoardsBody*>::iterator i  = g_flatboards_list.begin();
	std::vector<CFlatBoardsBody*>::iterator ie = g_flatboards_list.end();

	for(;i!=ie;i++)
	{
		if((*i)==obj)
		{
			delete (*i);
			g_flatboards_list.erase(i);
			return;
		}
	}

	OutputDebugString(_T("[���o�C] Aki3d::DestroyFlatBoardsObject , �����炸\n"));
}

void Aki3d::DestroyMeshObject(CMesh* obj)
{
	std::vector<CMeshBody*>::iterator i  = g_mesh_list.begin();
	std::vector<CMeshBody*>::iterator ie = g_mesh_list.end();

	for(;i!=ie;i++)
	{
		if((*i)==obj)
		{
			delete (*i);
			g_mesh_list.erase(i);
			return;
		}
	}

	OutputDebugString(_T("[���o�C] Aki3d::DestroyMeshObject , �����炸\n"));
}

/*---------------------------------------------------------
	�}�g���b�N�X����
-----------------------------------------------------------*/
D3DXMATRIX mat;

D3DXMATRIX& Aki3d::CreateMatrix( V3d* scale, V3d* rotation, V3d* trans )
{
	D3DXMATRIX tempra;

	//�X�P�[��
	if(scale)
	{
		D3DXMatrixScaling( &mat, scale->x, scale->y, scale->z );
	}
	else
	{
		D3DXMatrixIdentity( &mat );
	}

	//��]�i�x�N�g���̒��������W�A���̉�]�p�Ƃ���j
	if(rotation)
	{
		mat*= *D3DXMatrixRotationAxis( &tempra, rotation, D3DXVec3Length(rotation) );
	}

	//�ړ�
	if(trans)
	{
		mat*= *D3DXMatrixTranslation( &tempra, trans->x, trans->y, trans->z );
	}

	return mat;
}


/*---------------------------------------------------------
	��n��
-----------------------------------------------------------*/
void Aki3d::Destroy()
{
	for (UINT i=0;i<g_tstrip_list.size();i++)
		delete g_tstrip_list[i];
	g_tstrip_list.clear();

	
	for (UINT i=0;i<g_particle_list.size();i++)
		delete g_particle_list[i];
	g_particle_list.clear();

	for (UINT i=0;i<g_flatboards_list.size();i++)
		delete g_flatboards_list[i];
	g_flatboards_list.clear();

	for (UINT i=0;i<g_mesh_list.size();i++)
		delete g_mesh_list[i];
	g_mesh_list.clear();

	SAFEDELETE( tex_man );
}

/*---------------------------------------------------------
	�e�N�X�`���[
-----------------------------------------------------------*/
LPDIRECT3DTEXTURE9 Aki3d::LoadTexture(const char *filename)
{
	if(!tex_man)return NULL;
	return tex_man->LoadTexture(filename);
}

void Aki3d::UnloadTexture(LPDIRECT3DTEXTURE9 ptex)
{
	if(!tex_man)return;
	tex_man->UnloadTexture(ptex);
}

/*---------------------------------------------------------
	�x�����O
-----------------------------------------------------------*/
void Aki3d::LogWarning(const char* fmt,...)
{
	if(!warning_report)return;

	va_list args;
	va_start(args, fmt);
	static char buffer[128];
	vsprintf(buffer, fmt, args);
	warning_report( buffer );
	va_end(args);
}

/*------------------------------------------------------------------
	V2D�N���X�֌W
--------------------------------------------------------------------*/

//����1��
void V2d::Normalize()
{
	float len = sqrtf(x*x + y*y);
	if(len>0.000001f){
		x /= len;
		y /= len;
	}
	else {//�K��
		y = 1.0f;
		x = 0.0f;
	}
}

//������
void V2d::Verticalize()
{
	float tmp = y;
	y = x;
	x = -tmp;
}

//�����擾
float V2d::Length()
{
	return sqrtf(x*x+y*y);
}

//�����ݒ�
void V2d::SetLength(float len)
{
	Normalize();
	x*=len;
	y*=len;
}

//���Z�q�֌W
V2d& V2d::operator=(const V2d& v)
{
	x = v.x;
	y = v.y;
	return(*this);
}

V2d V2d::operator+(const V2d& v)
{
	V2d ret;
	ret.x = x + v.x;
	ret.y = y + v.y;
	return(ret);
}

V2d V2d::operator-(const V2d& v)
{
	V2d ret;
	ret.x = x - v.x;
	ret.y = y - v.y;
	return(ret);
}

V2d& V2d::operator+=(const V2d& v)
{
	x += v.x;
	y += v.y;
	return(*this);
}

V2d& V2d::operator-=(const V2d& v)
{
	x -= v.x;
	y -= v.y;
	return(*this);
}

V2d V2d::operator*(const float& d)
{
	V2d ret = *this;
	ret.x =x*d;
	ret.y =y*d;
	return(ret);
}

V2d& V2d::operator*=(const float& d)
{
	x *= d;
	y *= d;
	return(*this);
}

V2d V2d::operator/(const float& d)
{
	V2d ret = *this;
	if(d!=0){
		ret.x =x/d;
		ret.y =y/d;
	}
	return(ret);
}
