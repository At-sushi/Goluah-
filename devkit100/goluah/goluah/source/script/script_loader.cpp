/*===============================================================

	スクリプトロードクラス　実装

=================================================================*/

#include "stdafx.h"
#include "script_loader.h"


//goluah_plus.dllの名残
CGoluahPlusBase::~CGoluahPlusBase(){}
void CGoluahPlusBase::Destroy(CGoluahPlusBase *ptr){if(ptr)delete ptr;}


/*---------------------------------------------------------------
	インスタンスをゲトー
-----------------------------------------------------------------*/
CScriptLoader* CScriptLoader::Create()
{
	return new CScriptLoaderInstance;
}


/*---------------------------------------------------------------
	ファイルのロード
	UINT include_nest : includeの依存関係が無限ループしないよう、
						最大ネスト数を超えたら処理を中止する。
-----------------------------------------------------------------*/
BOOL CScriptLoaderInstance::LoadFile(char *base_dir,char *filename,UINT include_count)
{
	if(STRINGHANDLER_MAXNESTCOUNT < include_count)return FALSE;

	m_base_dir = base_dir;
	m_filename = filename;

	//ファイル全体を読み込む
	UINT fsize;
	CString *file_path = new CString;
	file_path->Format(_T("%s\\%s"),base_dir,filename);
	BYTE* str_org = LoadWholeFile(file_path,&fsize);
	if(!str_org){
		delete file_path;
		return FALSE;
	}
	delete file_path;

	//行ごとに分割してリストを作成
	if(!DivideLine(str_org,fsize))return FALSE;
	delete [] str_org;

	//コメントを除去
	if(!EliminateComment())return FALSE;

	//include処理
	if(!IncludeCheck(include_count+1))return FALSE;

	//DebugOutput(NULL);

	//成功?
	return (list.size()!=0);
}


/*---------------------------------------------------------------
	別名置換処理
-----------------------------------------------------------------*/
void CScriptLoaderInstance::ReplaceAlias(CAliasList *palist)
{
	if(palist==NULL)return;
	CAliasListInstance *palist_inst = static_cast<CAliasListInstance*>(palist);
	aliaslist& alist = palist_inst->list;

	aliaslist::iterator i  =alist.begin();
	aliaslist::iterator ied=alist.end();

	strlinelist::iterator il;
	strlinelist::iterator iled;

	for(;i!=ied;i++){
		il=list.begin();
		iled=list.begin();
		for(;il!=iled;il++){
			il->str.Replace(i->str,i->alias);
		}
	}
}


/*---------------------------------------------------------------
	行取得（C文字列）
-----------------------------------------------------------------*/
char* CScriptLoaderInstance::GetLine(int index)
{
	strlinelist::iterator il	=list.begin();
	strlinelist::iterator iled	=list.end();

	for(;il!=iled;il++){
		if(il->line_no==index)return il->str.GetBuffer();
	}
	return NULL;
}


/*---------------------------------------------------------------
	行取得（MFC CStringクラス。ATLかも？）
-----------------------------------------------------------------*/
CString CScriptLoaderInstance::GetLineC(int index)
{
	strlinelist::iterator il	=list.begin();
	strlinelist::iterator iled	=list.end();

	for(;il!=iled;il++){
		if(il->line_no==index)return il->str;
	}

	CString dummy;
	return dummy;
}


/*---------------------------------------------------------------
	ファイルロード処理
	戻り値の BYTE* は new[] したものなので、呼び出し元でdelete[]する
-----------------------------------------------------------------*/
BYTE* CScriptLoaderInstance::LoadWholeFile(CString* filename,UINT *size)
{
	CFile file;

	OutputDebugString(filename->GetBuffer());
	if(!file.Open(filename->GetBuffer(),CFile::modeRead))return NULL;

	*size = (UINT)file.GetLength();

	BYTE *ret = new BYTE [ *size +1 ];
	if(ret){
		file.Read(ret,*size);
		ret[*size] = '\0';
	}

	return ret;
}


/*---------------------------------------------------------------
	行ごと分割処理
-----------------------------------------------------------------*/
BOOL CScriptLoaderInstance::DivideLine(BYTE* str_org,UINT size)
{
	BYTE *strbuf = new BYTE[1024];
	if(!strbuf)return FALSE;

	list.clear();//既存のデータを破棄

	BYTE *str_org_end = str_org + size;
	UINT strsize = 0;
	OneStringLine tmpitem;

	while(str_org != str_org_end)
	{
		if( *str_org==0x0d && *(str_org+1)==0x0a ){
			strbuf[strsize]='\0';
			tmpitem.str = strbuf;
			tmpitem.line_no = (int)list.size();
			list.push_back(tmpitem);
			str_org+=2;
			strsize=0;
		}
		else{
			strbuf[strsize] = *str_org;
			strsize++;
			str_org++;
		}
	}
	strbuf[strsize]='\0';
	tmpitem.str = strbuf;
	tmpitem.line_no = (int)list.size();
	list.push_back(tmpitem);
	delete [] strbuf;

	return TRUE;
}


/*---------------------------------------------------------------
	コメント除去処理
-----------------------------------------------------------------*/
BOOL CScriptLoaderInstance::EliminateComment()
{
	strlinelist::iterator il	=list.begin();
	strlinelist::iterator iled	=list.end();

	int compos;

	for(;il!=iled;il++){
		compos = il->str.Find(STRINGHANDLER_COMMENTSTART);
		if(compos>=0){
			il->str = il->str.Left(compos);
		}
	}
	return TRUE;
}


/*---------------------------------------------------------------
	include 処理
-----------------------------------------------------------------*/
BOOL CScriptLoaderInstance::IncludeCheck(UINT include_count)
{
	strlinelist::iterator il	=list.begin();
	strlinelist::iterator iled	=list.end();

	char *tmpstr = new char[MAX_PATH];
	CScriptLoaderInstance subtext;

	for(;il!=iled;il++){
		if(il->str.Find(_T("include"))==0){
			sscanf(il->str,_T("include %s"),tmpstr);
			if(subtext.LoadFile(m_base_dir.GetBuffer(),tmpstr,include_count)){
				int line_no = il->line_no;
				list.erase(il);
				Marge(line_no,subtext);
				//iedが変更された可能性があるので、最初からやり直し
				il	=list.begin();
				iled=list.end();
			}
		}
	}

	delete [] tmpstr;
	return TRUE;
}


/*---------------------------------------------------------------
	マージ。Include処理に使用
-----------------------------------------------------------------*/
void CScriptLoaderInstance::Marge(int index,CScriptLoaderInstance& sh)
{
	strlinelist::iterator il;
	strlinelist::iterator iled;

	//index 以下の行数をずらす
	il	=list.begin();
	iled=list.end();
	for(;il!=iled;il++){
		if(il->line_no > index){
			il->line_no += (int)sh.list.size()-1;
		}
	}

	//shの内容をリストに追加
	il	=sh.list.begin();
	iled=sh.list.end();
	for(;il!=iled;il++){
		il->line_no += index;
		list.push_back(*il);
	}
}



/*---------------------------------------------------------------
	デバッグ用、一覧出力
-----------------------------------------------------------------*/
#ifdef _DEBUG
#include<algorithm>
void CScriptLoaderInstance::DebugOutput(char *filename)
{
	OutputDebugString(_T("\n■CScriptLoaderInstance::DebugOutput - start\n"));

	strlinelist::iterator il;
	strlinelist::iterator iled;
	il	=list.begin();
	iled=list.end();
	std::sort(il,iled,compline);

	CString tmp;

	il	=list.begin();
	iled=list.end();
	for(;il!=iled;il++){
		tmp.Format(_T("line%d : %s\n"),il->line_no,il->str.GetBuffer());
		OutputDebugString(tmp);
	}

	OutputDebugString(_T("\n■CScriptLoaderInstance::DebugOutput - end\n"));
}
#endif


