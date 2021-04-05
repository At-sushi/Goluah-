
/*!
*	@file
*	@brief GCD形式データ取り扱い定義
*
*	Goluah!!本体と、矩形エディタ（are_editer）から参照されます
*	GCD_EDITERの定義の有無によって切り替えます。
*/
#pragma once

#include "define_gcd.h"


/*!
*	@ingroup global
*/
/*@{*/

//---------------------------------------------------------------------
/*!
	@brief GCD形式データ取り扱いクラス

	主にstaticで普通の関数として使用する。
	矩形エディタの場合はこのクラスのインスタンスをデータのセットとして扱う。
*/
class CGCDHandler
{
// スタティック----------------------------------------------------------
//■読み込み
public:

#ifdef GCD_EDITER
static BOOL GCDLoadDlg(GCD_CELL2 *pcdat,GCD_RECT *prdat,
				GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames=NULL);
#endif//GCD_EDITER

//!GCDファイルを読み込み
static int GCDLoad(char *filename,				//!< [in] 読み込むファイル名
				   LPVOID pcdat,				//!< [out] セルデータ読込先バッファ
				   LPVOID prdat,				//!< [out] 切り取り矩形データ読込先バッファ 
				   LPVOID phdat,				//!< [out] あたり判定矩形読込先バッファ
				   LPVOID pfhnames=NULL,		//!< [out] セル定義名読込先バッファ
				   DWORD version=GCD_VERSION	//!< [in] 指定された読込先バッファがどのバージョンの構造体で用意されているか
				   );
//!バージョン0.70のファイルを読み込み
static int GCDLoad070(HANDLE hFile,				//!< [in] 読み込むファイル
				  LPVOID pcdat,					//!< [out] セルデータ読込先バッファ
				   LPVOID prdat,				//!< [out] 切り取り矩形データ読込先バッファ 
				   LPVOID phdat,				//!< [out] あたり判定矩形読込先バッファ
				   LPVOID pfhnames=NULL,		//!< [out] セル定義名読込先バッファ
				   DWORD version=GCD_VERSION	//!< [in] 指定された読込先バッファがどのバージョンの構造体で用意されているか
				  );
//!バージョン0.90のファイルを読み込み
static int GCDLoad090(HANDLE hFile,
				  LPVOID pcdat,					//!< [out] セルデータ読込先バッファ
				  LPVOID prdat,					//!< [out] 切り取り矩形データ読込先バッファ 
				  LPVOID phdat,					//!< [out] あたり判定矩形読込先バッファ
				  LPVOID pfhnames=NULL,			//!< [out] セル定義名読込先バッファ
				  DWORD version=GCD_VERSION		//!< [in] 指定された読込先バッファがどのバージョンの構造体で用意されているか
				  );
//!圧縮済みファイルを読み込み
static int GCDLoadCompressed(char *filename,		//!< [in] 読み込むファイル名
				   LPVOID pcdat,				//!< [out] セルデータ読込先バッファ
				   LPVOID prdat,				//!< [out] 切り取り矩形データ読込先バッファ 
				   LPVOID phdat,				//!< [out] あたり判定矩形読込先バッファ
				   LPVOID pfhnames=NULL,		//!< [out] セル定義名読込先バッファ
				   DWORD version=GCD_VERSION	//!< [in] 指定された読込先バッファがどのバージョンの構造体で用意されているか
				   );

//■保存 （保存に渡すバッファは常に最新のバージョンであることが仮定される）
public:
#ifdef GCD_EDITER
static BOOL GCDSaveDlg(GCD_CELL2 *pcdat,GCD_RECT *prdat,
				GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames=NULL);
static int GCDSave(char *filename,GCD_CELL2 *pcdat,GCD_RECT *prdat,
			 GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames=NULL);
static int GCDSave070(char *filename,GCD_CELL2 *pcdat,GCD_RECT *prdat,
			 GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames=NULL);
static BOOL GCDSaveHeader(GCD_CELLNAMES *pfhnames);
static int GCDSaveCompressed(char *filename,GCD_CELL2 *pcdat,GCD_RECT *prdat,
			 GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames=NULL);
#endif//GCD_EDITER

//■その他ユーティリティ
#ifdef GCD_EDITER
static void GCDSetInitialData(GCD_CELL2 *pcdat,GCD_RECT *prdat,
					   GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames);
#endif//GCD_EDITER
static BOOL GCDIsEmptyCell2(GCD_CELL2& gcdc,GCD_HANTEI& gcdh);
static BOOL GCDIsEmptyRect(GCD_RECT& gcdr);
static void GCDSetEmptyCell2(GCD_CELL2 *pgcdc,GCD_HANTEI *pgcdh);
static void GCDSetEmptyRect(GCD_RECT *pgcdr);
static void GCDConvCell_070_090(GCD_CELL2_070 *src,GCD_CELL2 *dst);
static void GCDConvCell_090_070(GCD_CELL2 *src,GCD_CELL2_070 *dst);


// ダイナミック（？）---------------------------------------------------
public:
	CGCDHandler();
	~CGCDHandler(){Destroy();}

	int  Load(char *filename,BOOL hload,BOOL nload=FALSE);
	void Destroy();
	void Diet();
	
	BOOL valid(){return m_cdat&&m_rdat ? TRUE : FALSE;}
	BOOL IsEmptyRect(DWORD index){return GCDIsEmptyRect(m_rdat[index]);}
	BOOL IsEmptyCell(DWORD index){return GCDIsEmptyCell2(m_cdat[index],m_hdat[index]);}
	GCD_RECT*  GetRect(DWORD index){return &m_rdat[index];}
	GCD_CELL2* GetCell(DWORD index){return &m_cdat[index];}
	GCD_HANTEI* GetHantei(DWORD index){return &m_hdat[index];}
	char* GetName(DWORD index){return m_names->name[index];}

	#ifdef GCD_EDITER
	BOOL Save();
	#endif//GCD_EDITER

	GCD_CELL2		*m_cdat;	//!< セルデータ
	GCD_RECT		*m_rdat;	//!< 矩形データ
	GCD_HANTEI		*m_hdat;	//!< あたり判定データ
	GCD_CELLNAMES	*m_names;	//!< 定義名
};

/*@}*/
