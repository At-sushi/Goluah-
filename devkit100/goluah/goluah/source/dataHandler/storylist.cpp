
/*========================================================================

	ストーリー管理

==========================================================================*/

#include "stdafx.h"

#include "storylist.h"
#include "global.h"

static TCHAR afo[] = _T("afo");

/*************************************************************************

	ストーリーリストクラス

**************************************************************************/

/*!
*	初期化
*	キャラクターやステージの存在チェックが行われるため、
*	キャラクター・ステージのリスト初期化の後に実行すること
*/
void CStoryList::Initialize()
{
	//念のため、リストクリア
	list.clear();
	dlist.clear();


	HANDLE hFind;
	WIN32_FIND_DATA fd;


	//ディレクトリの一覧作成
	CStoryRingInfo ringitem;
	hFind = FindFirstFile(_T(".\\story\\*.*"), &fd);
	if(hFind != INVALID_HANDLE_VALUE) {//ディレクトリが存在する場合
		do {
			if(strcmp(fd.cFileName,_T("."))==0 || strcmp(fd.cFileName,_T(".."))==0);//アレ
			else if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //ディレクトリﾊｹｰﾝ
			{
				_stprintf(ringitem.dir,fd.cFileName);
				rlist.push_back(ringitem);
			}
		} while(FindNextFile(hFind, &fd));
		FindClose(hFind);
	}

	//各ディレクトリの中のストーリーを検索
	CStoryInfo sitem;
	SScriptElementList scr_list;
	TCHAR *filename= new TCHAR [MAX_PATH];
	CStoryRingInfoList::iterator i = rlist.begin();
	CStoryRingInfoList::iterator ie= rlist.end();
	for(;i!=ie;i++)
	{
		_stprintf(filename,_T("story\\%s\\*.*"),i->dir);
		hFind = FindFirstFile(filename, &fd);
		if(hFind != INVALID_HANDLE_VALUE) {//ディレクトリが存在する場合
			do {
				if(strcmp(fd.cFileName,_T("."))==0 || strcmp(fd.cFileName,_T(".."))==0);//アレ
				else if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				{
					//ディレクトリﾊｹｰﾝ
					sitem.SetDir(i->dir,fd.cFileName);
					_stprintf(filename,_T("story\\%s\\%s\\"),i->dir,fd.cFileName);
					if(CGoluahStoryScript::CreateScriptElementList(scr_list,filename))//読み込みOK
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
	Getなんたら系
--------------------------------------------------------------------------*/

//全ストーリー数を取得
UINT CStoryList::GetAllStoryNum()
{
	return list.size();
}

//指定インデックスのストーリーを開始する
void CStoryList::StartStory(UINT index)
{
	gbl.ods(_T("★CStoryList::StartStory * まだ\n"));
}

//指定インデックスのストーリー名を取得する
TCHAR* CStoryList::GetStoryName(UINT index)
{
	if(index>=list.size())return afo;
	return list[index].name;
}

//指定インデックスのストーリー名を取得する
TCHAR* CStoryList::GetStoryDir(UINT index)
{
	if(index>=list.size())return afo;
	return list[index].dir;
}

//指定インデックスのストーリー概要を取得する
TCHAR* CStoryList::GetStoryBrief(UINT index)
{
	if(index>=list.size())return afo;
	return list[index].brief;
}

//指定インデックスのストーリー情報を取得する
CStoryList::CStoryInfo* CStoryList::GetStoryInfo(UINT index)
{
	if(index>=list.size())return NULL;
	return &(list[index]);
}

//ディレクトリインデックス+ディレクトリ内インデックスから、全体通し番号を取得
int CStoryList::Ring2Index(UINT ridx,UINT idx)
{
	if(ridx>=rlist.size())return 0;
	if(idx>=rlist[ridx].ring2serial.size())return 0;

	return rlist[ridx].ring2serial[idx];
}

//ディレクトリ数取得
UINT CStoryList::GetRingNum()
{
	return rlist.size();
}

//指定インデックスのディレクトリ内にあるストーリー数取得
int CStoryList::GetRingCount(UINT idx)
{
	if(idx>=rlist.size())return 0;
	return rlist[idx].ring2serial.size();
}

//読み込みに失敗したストーリー数取得
int   CStoryList::GetErrorCount()
{
	return dlist.size();
}

//読み込み失敗理由取得
TCHAR* CStoryList::GetErrorStr(UINT idx)
{
	if(idx>=dlist.size())return afo;
	return dlist[idx].brief;
}

//読み込み失敗ディレクトリ
TCHAR* CStoryList::GetErrorDir(UINT idx)
{
	if(idx>=dlist.size())return afo;
	return dlist[idx].dir;
}

/*------------------------------------------------------------------------
	スクリプト要素リストから、各種情報取得
--------------------------------------------------------------------------*/
//名前取得
TCHAR* CStoryList::GetName(SScriptElementList& scr)
{
	CStoryElement_Settings* set = GetSettingsFromScriptList(scr);
	return set->title;
}

//概要取得
TCHAR* CStoryList::GetBrief(SScriptElementList& scr)
{
	CStoryElement_Settings* set = GetSettingsFromScriptList(scr);
	return set->brief;
}

//キャラクタ取得
int* CStoryList::GetCharacters(SScriptElementList& scr)
{
	CStoryElement_Settings* set = GetSettingsFromScriptList(scr);
	return set->characters;
}

//エラー文字列取得
TCHAR* CStoryList::GetFirstError(SScriptElementList& scr)
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

//!settings要素取得
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
	throw;//しらん
}


/*!
*	指定インデックスのアイコンファイル名を取得する
*/
void CStoryList::GetStoryIconPath(UINT index,TCHAR* dst)
{
	if(!dst)return;
	if(index>=list.size()){
		_stprintf(dst,_T("aho"));
		return;
	}

	_stprintf(dst,_T("%s\\%s"),list[index].dir,list[index].icon);
}

/*!
*	指定インデックスのプレビューファイル名を取得する
*/
void CStoryList::GetStoryPreviewPath(UINT index,TCHAR* dst)
{
	if(!dst)return;
	if(index>=list.size()){
		_stprintf(dst,_T("aho"));
		return;
	}

	_stprintf(dst,_T("%s\\%s"),list[index].dir,list[index].preview);
}


/*------------------------------------------------------------------------
	リスト要素メソッド
--------------------------------------------------------------------------*/
void CStoryList::CStoryInfo::Clear()
{
	SetDir(NULL,NULL);
	SetName(NULL);
	SetBrief(NULL);
	SetIcon(NULL);
	SetPreview(NULL);
}

void CStoryList::CStoryInfo::SetDir(TCHAR *s,TCHAR *s2)
{
	if(!s){
		_tcscpy(dir,_T(""));
		return;
	}

	_stprintf(dir,_T("story\\%s\\%s"),s,s2);
}

void CStoryList::CStoryInfo::SetName(TCHAR *s)
{
	if(!s){
		_tcscpy(name,_T("untitled"));
		return;
	}

	_tcscpy(name,s);
}

void CStoryList::CStoryInfo::SetBrief(TCHAR *s)
{
	if(!s){
		_tcscpy(brief,_T("- no information -"));
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
	else _tcscpy(brief,s);
}

void CStoryList::CStoryInfo::SetIcon(TCHAR *s)
{
	if(!s){
		_tcscpy(icon,_T(""));
		return;
	}
	_tcscpy(icon,s);
}

void CStoryList::CStoryInfo::SetPreview(TCHAR *s)
{
	if(!s){
		_tcscpy(preview,_T(""));
		return;
	}
	_tcscpy(preview,s);
}

void CStoryList::CStoryInfo::Setup(CStoryElement_Settings *settings,TCHAR *dir1,TCHAR* dir2)
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


