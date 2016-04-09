
/*========================================================================
	
	�X�e�[�W�Ǘ�

	�f�B���N�g������X�e�[�W�̃��X�g���쐬���ĊǗ�����B

==========================================================================*/

#include "stdafx.h"

#include "define_macro.h"
#include "gobject.h"
#include "stagelist.h"


/*-------------------------------------------------------------------------
	�����j��
---------------------------------------------------------------------------*/
CStageList::CStageList()
{
}

void CStageList::Destroy()
{
	infolist.clear();
	ringlist.clear();
	damelist.clear();
}


/*!
*
*	������
*	
*	1) �T�u�f�B���N�g���i�s����RING�Ƃ������ƂɂȂ��Ă���j�̈ꗗ�𐶐�
*	2) �e�f�B���N�g���Ɋւ��ăX�e�[�W������
*		2.5) ���������e�X�e�[�W�f�B���N�g���̐�����������
*	3) �X�e�[�W��1��������Ȃ����������O�̓��X�g����폜
*/
void CStageList::Initialize()
{
	Destroy();

	InitializeRingList();// 1)
	if(ringlist.size()==0)return;

	DWORD index = 0;
	foreach(ringlist,CSL_STAGERINGLIST,i){ 
		InitializeRing(index); // 2) , 2.5)
		index++;
	}

	//3)
/*	std::list<CSL_STAGERINGLIST::iterator> dellist;
	foreach(ringlist,CSL_STAGERINGLIST,ii){ 
		if(i->ring2serial.size() == 0){
			dellist.push_back(ii);
		}
	}
	foreach(dellist,std::list<CSL_STAGERINGLIST::iterator>,iii){
		ringlist.erase(*iii);
	}*/
}


//stage �ȉ��̃T�u�f�B���N�g���������O�Ƃ��ēo�^
void CStageList::InitializeRingList()
{
	HANDLE hFind;
	WIN32_FIND_DATA fd;

	hFind = FindFirstFile(_T(".\\stage\\*.*"), &fd);
	if(hFind == INVALID_HANDLE_VALUE)return;

	CSL_RINGINFO newitem;

	do {
		if(strcmp(fd.cFileName,_T("."))==0 || strcmp(fd.cFileName,_T(".."))==0 || strcmp(fd.cFileName,_T("bgm"))==0);
		else if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			_tcscpy(newitem.name,fd.cFileName);
			ringlist.push_back(newitem);
		}
	} while(FindNextFile(hFind, &fd));

	FindClose(hFind);
}


//�����O�i�T�u�f�B���N�g���j����̌�������
void CStageList::InitializeRing(DWORD index)
{
	HANDLE hFind;
	WIN32_FIND_DATA fd;

	TCHAR ringBaseDir[64];
	_stprintf(ringBaseDir,_T("stage\\%s\\*.*"),ringlist[index].name);
	hFind = FindFirstFile(ringBaseDir, &fd);

	if(hFind == INVALID_HANDLE_VALUE)return;

	TCHAR founddir[64];

	do {
		if(strcmp(fd.cFileName,_T("."))==0 || strcmp(fd.cFileName,_T(".."))==0);
		else if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			_stprintf(founddir , _T("stage\\%s\\%s") , ringlist[index].name , fd.cFileName);
			VerifyStageDir(founddir , index);
		}
	} while(FindNextFile(hFind, &fd));

	FindClose(hFind);
}


//�X�e�[�W����̐������`�F�b�N
BOOL CStageList::VerifyStageDir(TCHAR *dir,DWORD ring)
{
	TCHAR path[256];
	_stprintf(path,_T("%s\\back1.bmp"),dir);

	CSL_DAMEINFO dame;
	ZeroMemory(&dame,sizeof(dame));

	//���O���k�N
	TCHAR sname[256];
	ZeroMemory(sname,sizeof(sname));
	_stprintf(path,_T("%s\\name.txt"),dir);
	HANDLE hFile = CreateFile(path,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		//���s
		_tcscpy(dame.dir,dir);
		dame.damereas=CSL_DAME_NONAME;
		damelist.push_back(dame);
		return(FALSE);
	}
	DWORD br;
	ReadFile(hFile,&sname,sizeof(sname),&br,NULL);
	CloseHandle(hFile);
	if(strlen(sname)==0){
		_stprintf(sname,_T("STAGE%d"),infolist.size()+1);
	}
	
	//DLL����������o�[�W�����`�F�b�N
	_stprintf(path,_T("%s\\stage.dll"),dir);
	HINSTANCE hLib = LoadLibrary(path);
	PFUNC_STAGEINFO pfsinfo=NULL;
	SDI_STAGEINFO sinfo;
	ZeroMemory(&sinfo,sizeof(SDI_STAGEINFO));
	if(hLib!=NULL){
		pfsinfo=(PFUNC_STAGEINFO)GetProcAddress(hLib, _T("StageInfo"));
		if(pfsinfo!=NULL){
			if(pfsinfo(&sinfo)){
				if(sinfo.ver < 680){//DLL���Â�
					_tcscpy(dame.dir,dir);
					dame.damereas=CSL_DAME_OLDDLL;
					dame.ver=sinfo.ver;
					damelist.push_back(dame);
					return(FALSE);
				}
				if(sinfo.ver > SDI_VERSION){//DLL���V����
					_tcscpy(dame.dir,dir);
					dame.damereas=CSL_DAME_NEWDLL;
					dame.ver=sinfo.ver;
					damelist.push_back(dame);
					return(FALSE);
				}
			}
			else{//�֐����s�Ɏ��s
				_tcscpy(dame.dir,dir);
				dame.damereas=CSL_DAME_PROC2;
				damelist.push_back(dame);
				return(FALSE);
			}
		}
		else{//�֐��|�C���^�擾���s
				_tcscpy(dame.dir,dir);
				dame.damereas=CSL_DAME_PROC;
				damelist.push_back(dame);
				return FALSE;
		}

		RELEASEDLL(hLib);
	}

	//�������[
	//��񕡎�
	CSL_STAGEINFO newitem;
	ZeroMemory(&newitem,sizeof(CSL_STAGEINFO));
	_tcscpy(newitem.dir,dir);
	_tcscpy(newitem.name,sname);
	newitem.ver=sinfo.ver;
	//���X�g�ǉ�
	ringlist[ring].ring2serial.push_back( (DWORD)infolist.size() );
	infolist.push_back(newitem);

	return(YEAH);
}

//********************************************************************
// Get�`�n
//********************************************************************

int CStageList::GetStageCount()
{
	return(infolist.size());
}
TCHAR* CStageList::GetStageDir(int index)
{
	return(infolist[index].dir);
}
TCHAR* CStageList::GetStageName(int index)
{
	return(infolist[index].name);
}
int CStageList::GetDameStageCount()
{
	return( damelist.size() );
}
TCHAR* CStageList::GetDameStageDir(int index)
{
	return(damelist[index].dir);
}
int CStageList::FindStage(TCHAR *name)
{
	if(name==NULL)return(-1);

	for(DWORD i=0;i<infolist.size();i++){
		if(strcmp(infolist[i].name,name)==0)return(i);
	}
	return(-1);
}
DWORD CStageList::GetDameStageReason(int index)
{
	return(damelist[index].damereas);
}
DWORD CStageList::GetStageVer(int index)
{
	return(infolist[index].ver);
}
DWORD CStageList::GetDameStageVer(int index)
{
	return(damelist[index].ver);
}

DWORD CStageList::GetStageCountRing(DWORD rindex)
{
	if(rindex > ringlist.size()-1)return 0;
	return ringlist[rindex].ring2serial.size();
}

DWORD CStageList::Ring2Serial(DWORD ring,DWORD index)
{
	if(ring > ringlist.size()-1)return 0;
	if(index > ringlist.at(ring).ring2serial.size()-1)return 0;
	return  ringlist.at(ring).ring2serial.at(index)  ;
}