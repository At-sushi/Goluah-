
/*========================================================================

	�X�g�[���[�Ǘ�

==========================================================================*/

#include "stdafx.h"

#include "storylist.h"
#include "global.h"

static char afo[] = "afo";

/*************************************************************************

	�X�g�[���[���X�g�N���X

**************************************************************************/

/*!
*	������
*	�L�����N�^�[��X�e�[�W�̑��݃`�F�b�N���s���邽�߁A
*	�L�����N�^�[�E�X�e�[�W�̃��X�g�������̌�Ɏ��s���邱��
*/
void CStoryList::Initialize()
{
	//�O�̂��߁A���X�g�N���A
	list.clear();
	dlist.clear();


	HANDLE hFind;
	WIN32_FIND_DATA fd;


	//�f�B���N�g���̈ꗗ�쐬
	CStoryRingInfo ringitem;
	hFind = FindFirstFile(".\\story\\*.*", &fd);
	if(hFind != INVALID_HANDLE_VALUE) {//�f�B���N�g�������݂���ꍇ
		do {
			if(strcmp(fd.cFileName,".")==0 || strcmp(fd.cFileName,"..")==0);//�A��
			else if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //�f�B���N�g��ʹ��
			{
				sprintf(ringitem.dir,fd.cFileName);
				rlist.push_back(ringitem);
			}
		} while(FindNextFile(hFind, &fd));
		FindClose(hFind);
	}

	//�e�f�B���N�g���̒��̃X�g�[���[������
	CStoryInfo sitem;
	SScriptElementList scr_list;
	char *filename= new char [MAX_PATH];
	CStoryRingInfoList::iterator i = rlist.begin();
	CStoryRingInfoList::iterator ie= rlist.end();
	for(;i!=ie;i++)
	{
		sprintf(filename,".\\story\\%s\\*.*",i->dir);
		hFind = FindFirstFile(filename, &fd);
		if(hFind != INVALID_HANDLE_VALUE) {//�f�B���N�g�������݂���ꍇ
			do {
				if(strcmp(fd.cFileName,".")==0 || strcmp(fd.cFileName,"..")==0);//�A��
				else if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				{
					//�f�B���N�g��ʹ��
					sitem.SetDir(i->dir,fd.cFileName);
					sprintf(filename,".\\story\\%s\\%s\\",i->dir,fd.cFileName);
					if(CGoluahStoryScript::CreateScriptElementList(scr_list,filename))//�ǂݍ���OK
					{
						sitem.Setup( GetSettingsFromScriptList(scr_list), i->dir,fd.cFileName);
						i->ring2serial.push_back(list.size());
						list.push_back(sitem);
					}
					else{
						sitem.SetBrief( GetFirstError(scr_list) );
						dlist.push_back(sitem);
					}
				}
			} while(FindNextFile(hFind, &fd));
			FindClose(hFind);
		}
	}
	delete [] filename;
	CGoluahStoryScript::DestroyScriptElementList(scr_list);
}


/*------------------------------------------------------------------------
	Get�Ȃ񂽂�n
--------------------------------------------------------------------------*/

//�S�X�g�[���[�����擾
UINT CStoryList::GetAllStoryNum()
{
	return list.size();
}

//�w��C���f�b�N�X�̃X�g�[���[���J�n����
void CStoryList::StartStory(UINT index)
{
	gbl.ods("��CStoryList::StartStory * �܂�\n");
}

//�w��C���f�b�N�X�̃X�g�[���[�����擾����
char* CStoryList::GetStoryName(UINT index)
{
	if(index>=list.size())return afo;
	return list[index].name;
}

//�w��C���f�b�N�X�̃X�g�[���[�����擾����
char* CStoryList::GetStoryDir(UINT index)
{
	if(index>=list.size())return afo;
	return list[index].dir;
}

//�w��C���f�b�N�X�̃X�g�[���[�T�v���擾����
char* CStoryList::GetStoryBrief(UINT index)
{
	if(index>=list.size())return afo;
	return list[index].brief;
}

//�w��C���f�b�N�X�̃X�g�[���[�����擾����
CStoryList::CStoryInfo* CStoryList::GetStoryInfo(UINT index)
{
	if(index>=list.size())return NULL;
	return &(list[index]);
}

//�f�B���N�g���C���f�b�N�X+�f�B���N�g�����C���f�b�N�X����A�S�̒ʂ��ԍ����擾
int CStoryList::Ring2Index(UINT ridx,UINT idx)
{
	if(ridx>=rlist.size())return 0;
	if(idx>=rlist[ridx].ring2serial.size())return 0;

	return rlist[ridx].ring2serial[idx];
}

//�f�B���N�g�����擾
UINT CStoryList::GetRingNum()
{
	return rlist.size();
}

//�w��C���f�b�N�X�̃f�B���N�g�����ɂ���X�g�[���[���擾
int CStoryList::GetRingCount(UINT idx)
{
	if(idx>=rlist.size())return 0;
	return rlist[idx].ring2serial.size();
}

//�ǂݍ��݂Ɏ��s�����X�g�[���[���擾
int   CStoryList::GetErrorCount()
{
	return dlist.size();
}

//�ǂݍ��ݎ��s���R�擾
char* CStoryList::GetErrorStr(UINT idx)
{
	if(idx>=dlist.size())return afo;
	return dlist[idx].brief;
}

//�ǂݍ��ݎ��s�f�B���N�g��
char* CStoryList::GetErrorDir(UINT idx)
{
	if(idx>=dlist.size())return afo;
	return dlist[idx].dir;
}

/*------------------------------------------------------------------------
	�X�N���v�g�v�f���X�g����A�e����擾
--------------------------------------------------------------------------*/
//���O�擾
char* CStoryList::GetName(SScriptElementList& scr)
{
	CStoryElement_Settings* set = GetSettingsFromScriptList(scr);
	return set->title;
}

//�T�v�擾
char* CStoryList::GetBrief(SScriptElementList& scr)
{
	CStoryElement_Settings* set = GetSettingsFromScriptList(scr);
	return set->brief;
}

//�L�����N�^�擾
int* CStoryList::GetCharacters(SScriptElementList& scr)
{
	CStoryElement_Settings* set = GetSettingsFromScriptList(scr);
	return set->characters;
}

//�G���[������擾
char* CStoryList::GetFirstError(SScriptElementList& scr)
{
	SScriptElementList::iterator i =scr.begin();
	SScriptElementList::iterator ie=scr.end();
	for(;i!=ie;i++)
	{
		if((*i)->GetError()){
			return (*i)->GetError();
		}
	}
	return NULL;
}

//!settings�v�f�擾
CStoryElement_Settings* CStoryList::GetSettingsFromScriptList(SScriptElementList& scr)
{
	CStoryElement_Settings* ret;

	SScriptElementList::iterator i =scr.begin();
	SScriptElementList::iterator ie=scr.end();
	for(;i!=ie;i++)
	{
		ret = dynamic_cast<CStoryElement_Settings*>( *i );
		if(ret){
			return ret;
		}
	}
	throw;//�����
}


/*!
*	�w��C���f�b�N�X�̃A�C�R���t�@�C�������擾����
*/
void CStoryList::GetStoryIconPath(UINT index,char* dst)
{
	if(!dst)return;
	if(index>=list.size()){
		sprintf(dst,"aho");
		return;
	}

	sprintf(dst,"%s\\%s",list[index].dir,list[index].icon);
}

/*!
*	�w��C���f�b�N�X�̃v���r���[�t�@�C�������擾����
*/
void CStoryList::GetStoryPreviewPath(UINT index,char* dst)
{
	if(!dst)return;
	if(index>=list.size()){
		sprintf(dst,"aho");
		return;
	}

	sprintf(dst,"%s\\%s",list[index].dir,list[index].preview);
}


/*------------------------------------------------------------------------
	���X�g�v�f���\�b�h
--------------------------------------------------------------------------*/
void CStoryList::CStoryInfo::Clear()
{
	SetDir(NULL,NULL);
	SetName(NULL);
	SetBrief(NULL);
	SetIcon(NULL);
	SetPreview(NULL);
}

void CStoryList::CStoryInfo::SetDir(char *s,char *s2)
{
	if(!s){
		strcpy(dir,"");
		return;
	}

	sprintf(dir,".\\story\\%s\\%s",s,s2);
}

void CStoryList::CStoryInfo::SetName(char *s)
{
	if(!s){
		strcpy(name,"untitled");
		return;
	}

	strcpy(name,s);
}

void CStoryList::CStoryInfo::SetBrief(char *s)
{
	if(!s){
		strcpy(brief,"- no information -");
		return;
	}

	if(strlen(s)>500){
		memcpy(brief,s,500);
		int pos = 500;
		brief[pos] = '.';
		pos++;
		brief[pos] = '.';
		pos++;
		brief[pos] = '.';
		pos++;
		brief[pos] = '\0';
	}
	else strcpy(brief,s);
}

void CStoryList::CStoryInfo::SetIcon(char *s)
{
	if(!s){
		strcpy(icon,"");
		return;
	}
	strcpy(icon,s);
}

void CStoryList::CStoryInfo::SetPreview(char *s)
{
	if(!s){
		strcpy(preview,"");
		return;
	}
	strcpy(preview,s);
}

void CStoryList::CStoryInfo::Setup(CStoryElement_Settings *settings,char *dir1,char* dir2)
{
	SetDir(dir1,dir2);
	SetName(settings->title);
	SetBrief(settings->brief);
	SetIcon(settings->icon);
	SetPreview(settings->preview);
	
	cnum = settings->cnum;

	for(int i=0;i<MAXNUM_TEAM;i++)
	{
		characters[i]	=settings->characters[i];
		color[i]		=settings->color[i];
		option[i]		=settings->option[i];
		opttype[i]		=settings->opttype[i];
	}
}

CStoryList::CStoryInfo* CStoryList::CStoryInfo::Clone()
{
	CStoryInfo* ret = new CStoryInfo;
	*ret = *this;
	return ret;
}


