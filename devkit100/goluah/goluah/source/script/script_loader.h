/*!
*	@file
*	@brief スクリプト関連クラス定義
*/
#pragma once
/*!
*	@defgroup Script
*	@brief ストーリー/ストーリー中間デモ スクリプト
*/
/*!
*	@ingroup Script
*/
/*@{*/

class CScriptLoader;
class CAliasList;

//□golush_plus.dllの名残
/*
#ifdef GOLUAH_PLUS_DLL
#define DLLEXINPORT __declspec(dllexport)
#else
#define DLLEXINPORT __declspec(dllimport)
#endif*/
#define DLLEXINPORT
/*!
*	@brief 名残。意味なし。
*	@ingroup Script
*
*	以前CStringを使ってるところをMFC使用のDLLとして分割してたときの名残っぽい。
*	今は本体プロジェクトごとMFCプロジェクトになっちゃったから意味なし。
*/
class DLLEXINPORT CGoluahPlusBase
{
public:
	virtual ~CGoluahPlusBase();
	static void Destroy(CGoluahPlusBase* ptr);
};

/*!
*	@brief スクリプトロードクラス
*	@ingroup Script
*
*	ストーリーのスクリプトを読み込むクラスみたい。
*	実際にはこいつは空っぽで、CScriptLoaderInstance が機能を実装してる。
*	DLL化してたときの名残
*/
class DLLEXINPORT CScriptLoader : public CGoluahPlusBase
{
public:
	static CScriptLoader* Create();
	virtual ~CScriptLoader(){}

	virtual BOOL LoadFile(TCHAR *base_dir,TCHAR *filename,UINT include_count=0) = 0;
	virtual void ReplaceAlias(CAliasList *alist) = 0;
	virtual TCHAR*   GetLine(int index) = 0;
	virtual UINT	GetLineNum() = 0;
};

/*!
*	@brief 別名リストクラス
*	@ingroup Script
*
*	スクリプトのテキストで、文字列置換をするのに使おうとしてたような気がする。
*	実際使ってたっけ？
*/
class DLLEXINPORT CAliasList : public CGoluahPlusBase
{
public:
	static CAliasList* Create();
	virtual ~CAliasList(){}

	virtual BOOL LoadFile(TCHAR* filename) = 0;
	virtual void AddAlias(TCHAR* str,TCHAR* stralias) = 0;
};




#include <list>
#include <vector>

/*!
*	@brief CStringHandlerが内部的にもつ行リストの一行分
*	@ingroup Script
*/
struct OneStringLine
{
	int line_no;
	CString str;
};
typedef std::vector<OneStringLine> strlinelist;


#define STRINGHANDLER_MAXNESTCOUNT	10		//include最大ネスト数
#define STRINGHANDLER_COMMENTSTART	_T("//")	//コメント識別文字列

/*!
*	@brief CScriptLoaderクラス本体
*	@ingroup Script
*/
class CScriptLoaderInstance : public CScriptLoader
{
public:
	/*!
	@brief ファイルを読み込む

	処理内容：
	 1.ファイル全体のロード
	 2.行ごとに分割
	 3.コメント除去
	 4.include処理
	*/
	BOOL LoadFile(TCHAR *base_dir,TCHAR *filename,UINT include_count=0);

	//!別名置換処理
	void ReplaceAlias(CAliasList *alist);

	//!指定行取得
	TCHAR*   GetLine(int index);
	CString GetLineC(int index);
	UINT	GetLineNum(){return (UINT)list.size();}

#ifdef _DEBUG
	/*!
		@brief デバッグ用。
		保持しているリストをファイルに出力する*/
	void DebugOutput(TCHAR *filename);

	//!ソート用比較関数
	static bool compline(OneStringLine line1,OneStringLine line2)
					{return line1.line_no<line2.line_no;}
#endif

protected:
	//LoadFile内部処理
	BYTE* LoadWholeFile(CString *filename,UINT *size);	//!< ファイルロード処理
	BOOL  DivideLine(BYTE* str_org,UINT size);			//!< 行ごと分割処理してリストを構築
	BOOL  EliminateComment();							//!< コメント除去処理
	BOOL  IncludeCheck(UINT include_count);				//!< include 処理

	//!マージ。Include処理時に使用
	void Marge(int index,CScriptLoaderInstance& sh);

	CString m_base_dir;
	CString m_filename;
	strlinelist list;
};



//! 別名定義
struct StringPear
{
	CString str;
	CString alias;

	StringPear();
	~StringPear();
};
typedef std::list<StringPear> aliaslist;



//! 別名リストクラス実体
class CAliasListInstance : public CAliasList
{
public:
	void Load(TCHAR *filename);
	aliaslist list;
};

/*@}*/