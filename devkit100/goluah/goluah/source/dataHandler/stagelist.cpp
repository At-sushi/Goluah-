
/*========================================================================
	
	ステージ管理

	ディレクトリからステージのリストを作成して管理する。

==========================================================================*/

#include "stdafx.h"

#include "define_macro.h"
#include "gobject.h"
#include "stagelist.h"


/*-------------------------------------------------------------------------
	生成破棄
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
*	初期化
*	
*	1) サブディレクトリ（都合上RINGということになっている）の一覧を生成
*	2) 各ディレクトリに関してステージを検索
*		2.5) 検索した各ステージディレクトリの正当性を検証
*	3) ステージが1個も見つからなかったリングはリストから削除
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


//stage 以下のサブディレクトリをリングとして登録
void CStageList::InitializeRingList()
{
	HANDLE hFind;
	WIN32_FIND_DATA fd;

	hFind = FindFirstFile(".\\stage\\*.*", &fd);
	if(hFind == INVALID_HANDLE_VALUE)return;

	CSL_RINGINFO newitem;

	do {
		if(strcmp(fd.cFileName,".")==0 || strcmp(fd.cFileName,"..")==0 || strcmp(fd.cFileName,"bgm")==0);
		else if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			strcpy(newitem.name,fd.cFileName);
			ringlist.push_back(newitem);
		}
	} while(FindNextFile(hFind, &fd));

	FindClose(hFind);
}


//リング（サブディレクトリ）一つ分の検索処理
void CStageList::InitializeRing(DWORD index)
{
	HANDLE hFind;
	WIN32_FIND_DATA fd;

	char ringBaseDir[64];
	sprintf(ringBaseDir,"stage\\%s\\*.*",ringlist[index].name);
	hFind = FindFirstFile(ringBaseDir, &fd);

	if(hFind == INVALID_HANDLE_VALUE)return;

	char founddir[64];

	do {
		if(strcmp(fd.cFileName,".")==0 || strcmp(fd.cFileName,"..")==0);
		else if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			sprintf(founddir , "stage\\%s\\%s" , ringlist[index].name , fd.cFileName);
			VerifyStageDir(founddir , index);
		}
	} while(FindNextFile(hFind, &fd));

	FindClose(hFind);
}


//ステージ一つ分の正当性チェック
BOOL CStageList::VerifyStageDir(char *dir,DWORD ring)
{
	char path[256];
	sprintf(path,"%s\\back1.bmp",dir);

	CSL_DAMEINFO dame;
	ZeroMemory(&dame,sizeof(dame));

	//名前をヌク
	char sname[256];
	ZeroMemory(sname,sizeof(sname));
	sprintf(path,"%s\\name.txt",dir);
	HANDLE hFile = CreateFile(path,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		//失敗
		strcpy(dame.dir,dir);
		dame.damereas=CSL_DAME_NONAME;
		damelist.push_back(dame);
		return(FALSE);
	}
	DWORD br;
	ReadFile(hFile,&sname,sizeof(sname),&br,NULL);
	CloseHandle(hFile);
	if(strlen(sname)==0){
		sprintf(sname,"STAGE%d",infolist.size()+1);
	}
	
	//DLLがあったらバージョンチェック
	sprintf(path,"%s\\stage.dll",dir);
	HINSTANCE hLib = LoadLibrary(path);
	PFUNC_STAGEINFO pfsinfo=NULL;
	SDI_STAGEINFO sinfo;
	ZeroMemory(&sinfo,sizeof(SDI_STAGEINFO));
	if(hLib!=NULL){
		pfsinfo=(PFUNC_STAGEINFO)GetProcAddress(hLib, "StageInfo");
		if(pfsinfo!=NULL){
			if(pfsinfo(&sinfo)){
				if(sinfo.ver < 680){//DLLが古い
					strcpy(dame.dir,dir);
					dame.damereas=CSL_DAME_OLDDLL;
					dame.ver=sinfo.ver;
					damelist.push_back(dame);
					return(FALSE);
				}
				if(sinfo.ver > SDI_VERSION){//DLLが新しい
					strcpy(dame.dir,dir);
					dame.damereas=CSL_DAME_NEWDLL;
					dame.ver=sinfo.ver;
					damelist.push_back(dame);
					return(FALSE);
				}
			}
			else{//関数実行に失敗
				strcpy(dame.dir,dir);
				dame.damereas=CSL_DAME_PROC2;
				damelist.push_back(dame);
				return(FALSE);
			}
		}
		else{//関数ポインタ取得失敗
				strcpy(dame.dir,dir);
				dame.damereas=CSL_DAME_PROC;
				damelist.push_back(dame);
				return FALSE;
		}

		RELEASEDLL(hLib);
	}

	//おっけー
	//情報複写
	CSL_STAGEINFO newitem;
	ZeroMemory(&newitem,sizeof(CSL_STAGEINFO));
	strcpy(newitem.dir,dir);
	strcpy(newitem.name,sname);
	newitem.ver=sinfo.ver;
	//リスト追加
	ringlist[ring].ring2serial.push_back( (DWORD)infolist.size() );
	infolist.push_back(newitem);

	return(YEAH);
}

//********************************************************************
// Get～系
//********************************************************************

int CStageList::GetStageCount()
{
	return(infolist.size());
}
char* CStageList::GetStageDir(int index)
{
	return(infolist[index].dir);
}
char* CStageList::GetStageName(int index)
{
	return(infolist[index].name);
}
int CStageList::GetDameStageCount()
{
	return( damelist.size() );
}
char* CStageList::GetDameStageDir(int index)
{
	return(damelist[index].dir);
}
int CStageList::FindStage(char *name)
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
