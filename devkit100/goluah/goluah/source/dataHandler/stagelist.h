
/*========================================================================
	
	ステージ管理

	ディレクトリからステージのリストを作成して管理する。

==========================================================================*/
#pragma once

/*!
*	@ingroup global
*/
/*@{*/


/*!
*	@brief ステージの情報 
*/
struct CSL_STAGEINFO
{
	TCHAR dir[64];				//!< どのディレクトリにあるか
	TCHAR name[32];				//!< 名前はなんというのか
	DWORD ver;					//!< DLLが返したバージョン
};
typedef std::vector<CSL_STAGEINFO> CSL_STAGEINFOLIST;	//!< ステージ情報配列(STL)



/*!
*	@brief 認証失敗ステージの情報
*/
struct CSL_DAMEINFO
{
	TCHAR dir[64];				//!< どのディレクトリにあるか
	DWORD damereas;				//!< 駄目な理由
	DWORD ver;					//!< DLLが返したバージョン
};
typedef std::vector<CSL_DAMEINFO> CSL_DAMEINFOLIST;	//!< 認証失敗ステージ配列(STL)

#define CSL_DAME_NONAME		1	//!< ステージ認識失敗理由ID、名前が見つからなかった
#define CSL_DAME_PROC		2	//!< ステージ認識失敗理由ID、関数ポインタ取得に失敗
#define CSL_DAME_PROC2		3	//!< ステージ認識失敗理由ID、StageInfo関数に失敗
#define CSL_DAME_OLDDLL		4	//!< ステージ認識失敗理由ID、DLLのバージョンが古い
#define CSL_DAME_NEWDLL		5	//!< ステージ認識失敗理由ID、DLLのバージョンが新しい



/*!
*	@brief ステージディレクトリリスト（ステージリング）
*/
struct CSL_RINGINFO
{
	TCHAR name[32];						//!< リング名(=表示名)
	std::vector<DWORD> ring2serial;		//!< リング内インデックス→全体インデックス
};
typedef std::vector<CSL_RINGINFO> CSL_STAGERINGLIST;	//!< ステージリング配列(STL)




/*!
*	@brief ステージリスト管理クラス本体
*
*	グローバルにひとつだけインスタンスがある。
*	ステージに関する情報まとめ。
*/
class CStageList
{
public:
	CStageList();
	~CStageList(){Destroy();}

	void Initialize();				//!< ディレクトリを検索し、リストを初期化
	void Destroy();					//!< リストを破棄

	int GetStageCount();			//!< 検索されたステージ数を返す
	TCHAR* GetStageDir(int index);	//!< ステージのディレクトリを返す
	TCHAR* GetStageName(int index);	//!< ステージの名前を返す
	DWORD GetStageVer(int index);	//!< ステージのバージョンを返す
	int FindStage(TCHAR *name);		//!< 名前で検索（なかった場合-1）

	int GetDameStageCount();						//!< 検索に失敗したステージ数を返す
	TCHAR* GetDameStageDir(int index);				//!< 失敗理由を文字列で返す
	DWORD GetDameStageReason(int index);			//!< 検索失敗理由を理由ID値で返す
	DWORD GetDameStageVer(int index);				//!< 検索失敗ステージ・バージョンを返す

	DWORD Ring2Serial(DWORD ring,DWORD rindex);		//!< リング内でのインデックスを全体でのインデックスに変換
	DWORD GetStageCountRing(DWORD rindex);			//!< 指定リングにステージがいくつあるか返す
	DWORD GetRingNum(){return ringlist.size();}		//!< リングの個数を返す

private:
	void InitializeRingList();						//!< Initializeから呼ばれる
	void InitializeRing(DWORD index);				//!< Initializeから呼ばれる
	BOOL VerifyStageDir(TCHAR *dir,DWORD ring);		//!< Initializeから呼ばれる

private:
	CSL_STAGEINFOLIST infolist;		//!< ステージ情報リスト
	CSL_STAGERINGLIST ringlist;		//!< ステージリングリスト
	CSL_DAMEINFOLIST damelist;		//!< ディレクトリはあったけど認識に失敗したリスト
};

/*@}*/


