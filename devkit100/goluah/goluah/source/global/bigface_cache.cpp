
/*=======================================================================
	�f�J��L���b�V��
	�\���ɕK�v�Ȃ��Ȃ������̂��A�����[�h�����ɕێ����Ă�����
	�ǂݍ��݉񐔂��y������
=========================================================================*/
#pragma once

#include "stdafx.h"
#include "global.h"
#include "bigface_cache.h"

#define MAX_BIGFACE_CACHE	(MAXNUM_TEAM*2+3)


/*
	�擾

	@param cindex �L�����N�^�[�C���f�b�N�X�B�}�C�i�X�̏ꍇ�����_���p�̃t�F�C�X��Ԃ�
	@param col �F�ԍ��B�p���b�g������
	@param alt �L�����N�^�[�̃I�v�V�����ɂ�茈�肳���@face2�ύX�l
	@return �w��摜��ǂݍ��񂾃C���`�L�T�[�t�F�C�X
*/
MYSURFACE* CTBigFaceCache::Get(int cindex,int col,int alt)
{
	if(cindex>g_charlist.GetCharacterCount())return NULL;
	if(cindex<0){//�����_���̏ꍇ�݂͂�Ȃ�������
		cindex=-1;
		col=0;//�����_���̏ꍇ�A�J���[�͊֌W�Ȃ�
		alt=0;//alt���֌W�Ȃ�
	}

	//���łɓǂݍ���ł�����̂̒�����T��
	texref_list::iterator i =list.begin();
	texref_list::iterator ie=list.end();
	for(;i!=ie;i++){
		if((*i)->cindex==cindex && (*i)->col==col && (*i)->alt==alt)//�q�b�g
		{
			CTextureRef* hit = *i;
			hit->crnt_ref = TRUE;
			list.erase(i);
			list.push_back(hit);//�����������̂����X�g�̖����Ɉړ�����
			return hit->dds;
		}
	}

	gbl.ods(_T("CTBigFaceCache::Get , miss hit %d,%d,%d"),cindex,col,alt);

	//�Ȃ��ꍇ�͂��傤���Ȃ��̂Ńt�@�C�����烍�[�h����
	MYSURFACE *dds=NULL;
	if(cindex>=0){
		char *filepath = new char[MAX_PATH];
		char *palpath = new char[MAX_PATH];
		char altstr[2]={'\0','\0'};
		if(alt!=0){
			altstr[0]='a'+alt-1;
		}
		sprintf(filepath,_T("%s\\face2%s"),g_charlist.GetCharacterDir(cindex),altstr );
		sprintf(palpath,_T("%s\\pal%d"),g_charlist.GetCharacterDir(cindex),col);
		dds = g_draw.CreateSurfaceFrom256Image(filepath,palpath);
		gbl.ods(_T("CTBigFaceCache : load %s"),filepath);
		delete [] filepath;
		delete [] palpath;
	}
	else{//�����_���p
		dds = g_draw.CreateSurfaceFrom256Image(_T(".\\system\\rand_face"));
	}

	//���X�g�̖����ɒǉ�����
	CTextureRef* nitem = new CTextureRef;
	nitem->dds         = dds;
	nitem->cindex      = cindex;
	nitem->col         = col;
	nitem->alt		   = alt;
	list.push_back(nitem);

	return dds;
}

/*!
*	�S���X�g�j��
*	BG�^�X�N���p�����Ă��邽�߁A�V�X�e���̃V���b�g�_�E�����Ɏ����I�ɌĂ΂��B
*/
void CTBigFaceCache::Terminate()
{
	texref_list::iterator i =list.begin();
	texref_list::iterator ie=list.end();
	for(;i!=ie;i++){
		delete (*i);
	}
	list.clear();
}


/*!
*	�X�V
*	���ۓǂݍ��܂�Ă���r�b�g�}�b�v���́A�ő吔���u�ԓI�ɒ����邱�Ƃ�����
*	�i�����̊ԈႢ�Ŗ��t���[���r�b�g�}�b�v�ǂݍ��݂Ƃ����悤�Ȏ��ԂɂȂ�Ȃ��悤�j
*	���̏ꍇ�A���݃t���[���ŗv���̂Ȃ������r�b�g�}�b�v���폜����B
*/
BOOL CTBigFaceCache::Execute(DWORD time)
{
	texref_list::iterator i,ie;

	//�ő吔�`�F�b�N
	int max_over = list.size() - MAX_BIGFACE_CACHE;

	//���X�g���ő吔�𒴂��Ă���ꍇ�A�Q�Ƃ̂Ȃ��������ڂ��폜����
	if(max_over>0){
		i =list.begin();
		ie=list.end();
		for(;i!=ie;)
		{
			if( !((*i)->crnt_ref || (*i)->prev_ref) ){
				delete (*i);
				i = list.erase(i);
				max_over--;
			}
			else i++;
			if(max_over==0)break;
		}
	}

	//�Q�ƃt���O�X�V
	i =list.begin();
	ie=list.end();
	for(;i!=ie;i++)
	{
		(*i)->prev_ref = (*i)->crnt_ref;
		(*i)->crnt_ref = FALSE;
	}

	return TRUE;
}


/*------------------------------------------------------------------------
	�����N���X�̍\�z�Ɣj��
--------------------------------------------------------------------------*/
CTBigFaceCache::CTextureRef::CTextureRef()
{
	dds=NULL;
	prev_ref=TRUE;
	crnt_ref=TRUE;
}

CTBigFaceCache::CTextureRef::~CTextureRef()
{
	RELSURFACE(dds);
}

