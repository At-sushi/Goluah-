
#ifndef GCD_EDITER
#include "stdafx.h"
#include "task_loading.h"
#endif

#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <zlib.h>

#include "gcdhandler.h"

#ifdef  GCD_EDITER
extern HWND ghwnd;
#define ODS(a)	OutputDebugString(a)
#else
#include "global.h"
#ifdef _DEBUG
#define ODS(a) gbl.ods(a)
#else
#define ODS(a)	((void)0)
#endif
#endif//GCD_EDITER


// シグネイチャ
const BYTE cmp_sig[] = _T("GCDC");

//**********************************************************************************
//  load
//**********************************************************************************

#ifdef GCD_EDITER

/*!
*	「ファイルを開く」ダイアログを提示してファイルロード
*/
BOOL CGCDHandler::GCDLoadDlg(GCD_CELL2 *pcdat,GCD_RECT *prdat,GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames)
{
	TCHAR filepath[256]=_T("");
	TCHAR filename[64];
	OPENFILENAME ofn;
	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = ghwnd;
	ofn.lpstrFilter = _T("データ(*.gcd)\0*.gcd\0すべて(*.*)\0*.*\0\0");
	ofn.lpstrFile = filepath;
	ofn.nMaxFile =256;
	ofn.lpstrFileTitle = filename;
	ofn.nMaxFileTitle = 64;
	ofn.Flags = OFN_FILEMUSTEXIST;
	ofn.lpstrTitle = _T("開く");

	if(!GetOpenFileName(&ofn))return(FALSE);

	switch(GCDLoad(filepath,pcdat,prdat,phdat,pfhnames,GCD_VERSION)){
	case 0://成功
		return(TRUE);
	default:MessageBox(ghwnd,_T("読込に失敗しました"),_T(""),MB_OK);
	}
	
	return(FALSE);
}

#endif


/*-----------------------------------------------------------------------
	読み込み
	ファイルのバージョン・バッファのバージョン指定に基づいて可能な限り
	適切な変換を施すロード関数に処理をまわす。

	戻0で正常終了
-------------------------------------------------------------------------*/

int CGCDHandler::GCDLoad(TCHAR *filename,LPVOID pcdat,LPVOID prdat,LPVOID phdat,LPVOID pfhnames,DWORD version)
{
	//ファイルオープン
	HANDLE hFile = CreateFile(filename,
		GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		return(1);
	}

	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//ファイル先頭に移動
	//ヘッダ部の読み出し
	DWORD br;
	DWORD buff[2];
	DWORD ret=ReadFile(hFile,buff,8,&br,NULL);
	if(!ret || br!=8){
		CloseHandle(hFile);
		return(2);
	}
	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//ファイル先頭に移動
	
	if (buff[0] != 8){
		CloseHandle(hFile);
		return(9);
	}

	int ret2;
	switch(buff[1]){
		case 900:ret2=GCDLoad090(hFile,pcdat,prdat,phdat,pfhnames,version);break;
		case 700:ret2=GCDLoad070(hFile,pcdat,prdat,phdat,pfhnames,version);break;
		default:ret2 = 9;
	}
	CloseHandle(hFile);

	//バージョン情報埋め込み
	if(ret2==0){
		switch(version){
		case 900:
			{
				GCD_CELL2_090 *pc090 = (GCD_CELL2_090*)pcdat;
				pc090->cell[0].flag = 900;
			}break;
		case 700:
			{
				GCD_CELL2_070 *pc070 = (GCD_CELL2_070*)pcdat;
				pc070->cell[0].flag = 700;
			}break;
		}
	}

	return ret2;
}


/*-----------------------------------------------------------------------
	0.90 読み込み
	ファイルからの直接ロードはテンポラリのバッファへ行い、
	全てのプロセスが終了したら指定バッファへのコピーを行う

	0.90以外のバッファへの変換は行わない

	戻0で正常終了
-------------------------------------------------------------------------*/
int CGCDHandler::GCDLoad090(HANDLE hFile,LPVOID pcdat,LPVOID prdat,LPVOID phdat,LPVOID pfhnames,DWORD buff_version)
{
	if(buff_version<900)return 8;

	//テンポラリ用
	GCD_FILEHEADER_090	fhed;
	GCD_CELLNAMES_090	fh_names;
	GCD_RECT_090		*dm_rdat = new GCD_RECT_090[GCDMAX_RECTANGLES];
	GCD_CELL2_090		*dm_cdat = new GCD_CELL2_090[GCDMAX_CELLS];
	GCD_HANTEI_090		*dm_hdat = new GCD_HANTEI_090[GCDMAX_CELLS];

	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//ファイル先頭に移動
	DWORD ret = 0;

	try{
		//ヘッダ部の読み出し
		DWORD br;
		DWORD ret=ReadFile(hFile,&fhed,sizeof(GCD_FILEHEADER_090),&br,NULL);
		if(!ret || br!=sizeof(GCD_FILEHEADER_090)){
			ret=2;throw;
		}

		//セル名を読み出し
		ret=ReadFile(hFile,&fh_names,sizeof(GCD_CELLNAMES_090),&br,NULL);
		if(!ret || br!=sizeof(GCD_CELLNAMES_090)){
			ret=4;throw;
		}

		//矩形
		ret=ReadFile(hFile,dm_rdat,sizeof(GCD_RECT_090)*GCDMAX_RECTANGLES,&br,NULL);
		if(!ret || br!=sizeof(GCD_RECT_090)*GCDMAX_RECTANGLES){
			ret=5;throw;
		}
#		ifndef GCD_EDITER
			//進行状況表示
			CTNowLoading* task = dynamic_cast<CTNowLoading*>( g_system.FindTask('LOAD') );
			if(task)task->Progress(NowLoading_GCD, 0.5f);		/* 量は適当 */
#		endif
		//ＣＥＬＬ
		ret=ReadFile(hFile,dm_cdat,sizeof(GCD_CELL2_090)*GCDMAX_CELLS,&br,NULL);
		if(!ret || br!=sizeof(GCD_CELL2_090)*GCDMAX_CELLS){
			ret=6;throw;
		}
		//あたり判定
		ret=ReadFile(hFile,dm_hdat,sizeof(GCD_HANTEI_090)*GCDMAX_CELLS,&br,NULL);
		if(!ret || br!=sizeof(GCD_HANTEI_090)*GCDMAX_CELLS){
			ret=7;throw;
		}
	}
	catch(...){
		ODS(_T("□GCDHandler : ver0.90 GCD load failed"));
	}

	if(ret==0){
		//成功・渡されたバッファへデータをコピー
		if(pfhnames!=NULL){
			memcpy(pfhnames,&fh_names,sizeof(GCD_CELLNAMES));
		}
		if(prdat!=NULL)memcpy(prdat,dm_rdat,sizeof(GCD_RECT_090)*GCDMAX_RECTANGLES);
		if(pcdat!=NULL)memcpy(pcdat,dm_cdat,sizeof(GCD_CELL2_090)*GCDMAX_CELLS);
		if(phdat!=NULL)memcpy(phdat,dm_hdat,sizeof(GCD_HANTEI_090)*GCDMAX_CELLS);

		ODS(_T("□GCDHandler : ver0.90 GCD load comlete\n"));
	}
	delete [] dm_rdat;
	delete [] dm_cdat;
	delete [] dm_hdat;

	//バージョン情報埋め込み
	if(pcdat != NULL) ((GCD_CELL2_090*)pcdat)[0].cell[0].flag = 900;
	
	return(0);
}

/*-----------------------------------------------------------------------
	GCDver0.70読み込み
	ファイルからの直接ロードはテンポラリのバッファへ行い、
	全てのプロセスが終了したら指定バッファへのコピーを行う
	0.70または0.90形式のバッファに読み込める

	戻0で正常終了
-------------------------------------------------------------------------*/
int CGCDHandler::GCDLoad070(HANDLE hFile,LPVOID pcdat,LPVOID prdat,LPVOID phdat,LPVOID pfhnames,DWORD buff_version)
{
	if(buff_version<700)return 8;

	//テンポラリ用
	GCD_FILEHEADER_070	fhed;
	GCD_CELLNAMES_070	fh_names;
	GCD_RECT_070		*dm_rdat = new GCD_RECT_070[GCDMAX_RECTANGLES];
	GCD_CELL2_070		*dm_cdat = new GCD_CELL2_070[GCDMAX_CELLS];
	GCD_HANTEI_070		*dm_hdat = new GCD_HANTEI_070[GCDMAX_CELLS];

	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//ファイル先頭に移動
	DWORD ret = 0;

	try{
		//ヘッダ部の読み出し
		DWORD br;
		DWORD ret=ReadFile(hFile,&fhed,sizeof(GCD_FILEHEADER_070),&br,NULL);
		if(!ret || br!=sizeof(GCD_FILEHEADER_070)){
			ret=2;throw;
		}

		//セル名を読み出し
		ret=ReadFile(hFile,&fh_names,sizeof(GCD_CELLNAMES_070),&br,NULL);
		if(!ret || br!=sizeof(GCD_CELLNAMES_070)){
			ret=4;throw;
		}

		//矩形
		ret=ReadFile(hFile,dm_rdat,sizeof(GCD_RECT_070)*GCDMAX_RECTANGLES,&br,NULL);
		if(!ret || br!=sizeof(GCD_RECT_070)*GCDMAX_RECTANGLES){
			ret=5;throw;
		}
		//ＣＥＬＬ
		ret=ReadFile(hFile,dm_cdat,sizeof(GCD_CELL2_070)*GCDMAX_CELLS,&br,NULL);
		if(!ret || br!=sizeof(GCD_CELL2_070)*GCDMAX_CELLS){
			ret=6;throw;
		}
		//あたり判定
		ret=ReadFile(hFile,dm_hdat,sizeof(GCD_HANTEI_070)*GCDMAX_CELLS,&br,NULL);
		if(!ret || br!=sizeof(GCD_HANTEI_070)*GCDMAX_CELLS){
			ret=7;throw;
		}
	}
	catch(...){
		ODS(_T("□GCDHandler : ver0.70 GCD load failed\n"));
	}

	if(ret==0){//成功・渡されたバッファへデータをコピー
		switch(buff_version){
			case 900:
				{
					if(pfhnames!=NULL){
						memcpy(pfhnames,&fh_names,sizeof(GCD_CELLNAMES_090));
					}
					if(prdat!=NULL)memcpy(prdat,dm_rdat,sizeof(GCD_RECT_090)*GCDMAX_RECTANGLES);
					if(pcdat!=NULL){
						GCD_CELL2_090 *pcdat090 = (GCD_CELL2_090*)pcdat;
						for(int i=0;i<GCDMAX_CELLS;i++){
							GCDConvCell_070_090(&dm_cdat[i],&pcdat090[i]);
						}
					}
					if(phdat!=NULL)memcpy(phdat,dm_hdat,sizeof(GCD_HANTEI_090)*GCDMAX_CELLS);
					ODS(_T("□GCDHandler : ver0.70→ver0.90 GCD load comlete"));

					//バージョン情報埋め込み
					if(pcdat != NULL) ((GCD_CELL2_090*)pcdat)[0].cell[0].flag = 900;
				}break;
			case 700:
				{
					if(pfhnames!=NULL){
						memcpy(pfhnames,&fh_names,sizeof(GCD_CELLNAMES_070));
					}
					if(prdat!=NULL)memcpy(prdat,dm_rdat,sizeof(GCD_RECT_070)*GCDMAX_RECTANGLES);
					if(pcdat!=NULL)memcpy(pcdat,dm_cdat,sizeof(GCD_CELL2_070)*GCDMAX_CELLS);
					if(phdat!=NULL)memcpy(phdat,dm_hdat,sizeof(GCD_HANTEI_070)*GCDMAX_CELLS);
					ODS(_T("□GCDHandler : ver0.70 GCD load comlete\n"));

					//バージョン情報埋め込み
					if(pcdat != NULL) ((GCD_CELL2_070*)pcdat)[0].cell[0].flag = 700;
				}break;
		}
	}
	delete [] dm_rdat;
	delete [] dm_cdat;
	delete [] dm_hdat;
	
	return(0);
}

/*-----------------------------------------------------------------------
	読み込み
	ファイルのバージョン・バッファのバージョン指定に基づいて可能な限り
	適切な変換を施すロード関数に処理をまわす。

	戻0で正常終了
-------------------------------------------------------------------------*/
int CGCDHandler::GCDLoadCompressed(TCHAR *filename,LPVOID pcdat,LPVOID prdat,LPVOID phdat,LPVOID pfhnames,DWORD version)
{
	BYTE* Buffer = NULL;
	DWORD size = 0;
	GCD_COMP_FILEHEADER fhead;
	GCD_COMP_DATAHEADER dhead;
#ifdef GCD_EDITER
	DWORD CRCSum;
#endif

	//ファイルオープン
	HANDLE hFile = CreateFile(filename,
		GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		return(1);
	}

	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//ファイル先頭に移動

	//ヘッダ部の読み出し
	DWORD br;
	DWORD ret=ReadFile(hFile,&fhead,sizeof(fhead),&br,NULL);
	if(!ret || br!=sizeof(fhead)){
		CloseHandle(hFile);
		return(2);
	}

	// ヘッダチェック
	if ( memcmp(fhead.signature, cmp_sig, 4) != 0 || fhead.ver != version || fhead.flags & GCDFILE_COMP_USING_FILTER )
	{
		CloseHandle(hFile);
		return 9;
	}

	// セル名
	if (pfhnames)
	{
		ZeroMemory(pfhnames, sizeof(GCD_CELLNAMES_090));

		if (fhead.flags & GCDFILE_COMP_HAVE_CELLNAMES)
		{
			// セル名あり
			// データヘッダ読み込み
			ret=ReadFile(hFile,&dhead,sizeof(dhead),&br,NULL);
			if(!ret || br!=sizeof(dhead)){
				CloseHandle(hFile);
				return(4);
			}

			// 読み込み
			Buffer = (BYTE*)malloc(dhead.DataSize);
			ret=ReadFile(hFile,Buffer,dhead.DataSize,&br,NULL);
			if(!ret || br!=dhead.DataSize){
				CloseHandle(hFile);
				if (Buffer) free(Buffer);
				return(4);
			}

			// 展開
			size = sizeof(GCD_CELLNAMES_090);
			uncompress((BYTE*)pfhnames, &size, Buffer, dhead.DataSize);
			if (Buffer) free(Buffer);

#ifdef GCD_EDITER
			CRCSum = crc32(0L, Z_NULL, 0);
			for (int i = 0; i < GCDMAX_CELLS; i++)
				CRCSum = crc32( CRCSum, (BYTE*)((GCD_CELLNAMES_090*)pfhnames)->name[i], strlen(((GCD_CELLNAMES_090*)pfhnames)->name[i]) + 1 );

			if (CRCSum != dhead.CRCCheckSum)
			{
				if ( MessageBox(NULL, _T("CRCエラー（セル定義名）：\nこのデータは破損している可能性があります。\n\n処理を続行しますか？"),
							_T("夜霧よ今夜もありがとう"), MB_YESNO | MB_ICONWARNING) == IDNO )
				{
					CloseHandle(hFile);
					return 4;
				}
			}
#endif
		}
	}
	else if (fhead.flags & GCDFILE_COMP_HAVE_CELLNAMES)
	{
		// セル名あるけどデータ取ってないよ
		// データヘッダ読み込み
		ret=ReadFile(hFile,&dhead,sizeof(dhead),&br,NULL);
		if(!ret || br!=sizeof(dhead)){
			CloseHandle(hFile);
			return(4);
		}

		// すっ飛ばす
		SetFilePointer(hFile, dhead.DataSize, NULL, FILE_CURRENT);
	}

	// 矩形
	ZeroMemory(prdat, sizeof(GCD_RECT_090) * GCDMAX_RECTANGLES);

	// データヘッダ読み込み
	ret=ReadFile(hFile,&dhead,sizeof(dhead),&br,NULL);
	if(!ret || br!=sizeof(dhead)){
		CloseHandle(hFile);
		return(5);
	}

	if (prdat)
	{
		// 読み込み
		Buffer = (BYTE*)malloc(dhead.DataSize);
		ret=ReadFile(hFile,Buffer,dhead.DataSize,&br,NULL);
		if(!ret || br!=dhead.DataSize){
			CloseHandle(hFile);
			if (Buffer) free(Buffer);
			return(5);
		}

		// 展開
		size = sizeof(GCD_RECT_090) * GCDMAX_RECTANGLES;
		uncompress((BYTE*)prdat, &size, Buffer, dhead.DataSize);
		if (Buffer) free(Buffer);

#		ifdef GCD_EDITER
			CRCSum = crc32( crc32(0L, Z_NULL, 0), (BYTE*)prdat, sizeof(GCD_RECT_090) * GCDMAX_RECTANGLES );
			if (CRCSum != dhead.CRCCheckSum)
			{
				if ( MessageBox(NULL, _T("CRCエラー（矩形データ）：\nこのデータは破損している可能性があります。\n\n処理を続行しますか？"),
							_T("夜霧よ今夜もありがとう"), MB_YESNO | MB_ICONWARNING) == IDNO )
				{
					CloseHandle(hFile);
					return 5;
				}
			}
#		endif
	}
	else
		SetFilePointer(hFile, dhead.DataSize, NULL, FILE_CURRENT);

	// セル
	// 何にも入ってないセル
	GCD_CELL cell_void[GCD_CELLGROUPNUM];
	for (int i = 0; i < GCD_CELLGROUPNUM; i++)
	{
		cell_void[i].cdr = 0;
		cell_void[i].dx = 0;
		cell_void[i].dy = 0;
		cell_void[i].flag = 0;
		cell_void[i].magx = 1.0f;
		cell_void[i].magy = 1.0f;
		cell_void[i].rot = 0;
		cell_void[i].red_ratio = 1.0f;
		cell_void[i].green_ratio = 1.0f;
		cell_void[i].blue_ratio = 1.0f;
		cell_void[i].alpha_ratio = 1.0f;
	}

	ZeroMemory(pcdat, sizeof(GCD_CELL2_090) * GCDMAX_CELLS);
	for (int i = 0; i < GCDMAX_CELLS; i++)
		for (int j = 0; j < GCD_CELLGROUPNUM; j++)
			((GCD_CELL2_090*)pcdat)[i].cell[j] = cell_void[j];

	// データヘッダ読み込み
	ret=ReadFile(hFile,&dhead,sizeof(dhead),&br,NULL);
	if(!ret || br!=sizeof(dhead)){
		CloseHandle(hFile);
		return(6);
	}

	if (pcdat)
	{
		// 読み込み
		Buffer = (BYTE*)malloc(dhead.DataSize);
		ret=ReadFile(hFile,Buffer,dhead.DataSize,&br,NULL);
		if(!ret || br!=dhead.DataSize){
			CloseHandle(hFile);
			if (Buffer) free(Buffer);
			return(6);
		}

		// 展開
		size = sizeof(GCD_CELL2_090) * GCDMAX_CELLS;
		uncompress((BYTE*)pcdat, &size, Buffer, dhead.DataSize);
		if (Buffer) free(Buffer);

#		ifdef GCD_EDITER
			CRCSum = crc32( crc32(0L, Z_NULL, 0), (BYTE*)pcdat, sizeof(GCD_CELL2_090) * GCDMAX_CELLS );
			if (CRCSum != dhead.CRCCheckSum)
			{
				if ( MessageBox(NULL, _T("CRCエラー（セルデータ）：\nこのデータは破損している可能性があります。\n\n処理を続行しますか？"),
							_T("夜霧よ今夜もありがとう"), MB_YESNO | MB_ICONWARNING) == IDNO )
				{
					CloseHandle(hFile);
					return 6;
				}
			}
#		endif
	}
	else
		SetFilePointer(hFile, dhead.DataSize, NULL, FILE_CURRENT);

	// あたり判定
	ZeroMemory(phdat, sizeof(GCD_HANTEI_090) * GCDMAX_CELLS);

	// データヘッダ読み込み
	ret=ReadFile(hFile,&dhead,sizeof(dhead),&br,NULL);
	if(!ret || br!=sizeof(dhead)){
		CloseHandle(hFile);
		return(7);
	}

	if (phdat)
	{
		// 読み込み
		Buffer = (BYTE*)malloc(dhead.DataSize);
		ret=ReadFile(hFile,Buffer,dhead.DataSize,&br,NULL);
		if(!ret || br!=dhead.DataSize){
			CloseHandle(hFile);
			if (Buffer) free(Buffer);
			return(7);
		}

		// 展開
		size = sizeof(GCD_HANTEI_090) * GCDMAX_CELLS;
		uncompress((BYTE*)phdat, &size, Buffer, dhead.DataSize);
		if (Buffer) free(Buffer);

#		ifdef GCD_EDITER
			CRCSum = crc32( crc32(0L, Z_NULL, 0), (BYTE*)phdat, sizeof(GCD_HANTEI_090) * GCDMAX_CELLS );
			if (CRCSum != dhead.CRCCheckSum)
			{
				if ( MessageBox(NULL, _T("CRCエラー（当たり判定データ）：\nこのデータは破損している可能性があります。\n\n処理を続行しますか？"),
							_T("夜霧よ今夜もありがとう"), MB_YESNO | MB_ICONWARNING) == IDNO )
				{
					CloseHandle(hFile);
					return 7;
				}
			}
#		endif
	}
	else
		SetFilePointer(hFile, dhead.DataSize, NULL, FILE_CURRENT);

	//バージョン情報埋め込み
	if(pcdat != NULL) ((GCD_CELL2_090*)pcdat)[0].cell[0].flag = version;
	
	CloseHandle(hFile);

	return 0;
}


//**********************************************************************************
//  save
//**********************************************************************************

#ifdef GCD_EDITER


/*-----------------------------------------------------------------------
	「ファイルを保存」ダイアログを提示してファイルを保存
-------------------------------------------------------------------------*/
BOOL CGCDHandler::GCDSaveDlg(GCD_CELL2 *pcdat,GCD_RECT *prdat,GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames)
{
	TCHAR filepath[256]=_T("");
	TCHAR filename[64];
	OPENFILENAME ofn;
	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = ghwnd;
	ofn.lpstrFilter = _T("GCD0.90形式(*.gcd)\0*.gcd\0圧縮GCD0.90形式(*.gcm)\0*.gcm\0GCD0.70形式(*.gcd)\0*.gcd\0すべてのファイル(*.*)\0*.*\0\0");
	ofn.lpstrDefExt = _T("gcd");
	ofn.lpstrFile = filepath;
	ofn.nMaxFile =256;
	ofn.lpstrFileTitle = filename;
	ofn.nMaxFileTitle = 64;
	ofn.Flags = OFN_OVERWRITEPROMPT;
	ofn.lpstrTitle = _T("保存");

	if(!GetSaveFileName(&ofn))return(FALSE);

	//保存
	int ret;
	switch(ofn.nFilterIndex){
	case 3://0.70フォーマットで保存
		ret = GCDSave070(filepath,pcdat,prdat,phdat,pfhnames);break;
	case 2://圧縮して保存
		if (strcmp(filepath + strlen(filepath) - 4, _T(".gcm")) == 0)
		{
			ret = GCDSaveCompressed(filepath,pcdat,prdat,phdat,pfhnames);break;
		}
	default:
		ret = GCDSave(filepath,pcdat,prdat,phdat,pfhnames);
	}

	//エラーメッセージ表示
	switch(ret){
	case 0:return(TRUE);
	case 1:MessageBox(ghwnd,_T("ファイルオープンに失敗"),_T("保存に失敗"),MB_OK);break;
	case 2:MessageBox(ghwnd,_T("書き込み失敗(2)"),_T("保存に失敗"),MB_OK);break;
	case 3:MessageBox(ghwnd,_T("書き込み失敗(3)"),_T("保存に失敗"),MB_OK);break;
	case 4:MessageBox(ghwnd,_T("書き込み失敗(4)"),_T("保存に失敗"),MB_OK);break;
	case 5:MessageBox(ghwnd,_T("書き込み失敗(5)"),_T("保存に失敗"),MB_OK);break;
	case 6:MessageBox(ghwnd,_T("書き込み失敗(6)"),_T("保存に失敗"),MB_OK);break;
	}
	return(FALSE);
}


/*-----------------------------------------------------------------------
	指定ファイルへ指定バッファのデータを保存
	戻0が正常終了
-------------------------------------------------------------------------*/
int CGCDHandler::GCDSave(TCHAR *filename,GCD_CELL2 *pcdat,GCD_RECT *prdat,GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames)
{
	GCD_FILEHEADER	fhed;
	GCD_CELLNAMES	fh_names;

	HANDLE hFile = CreateFile(filename,
		GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		return(1);
	}
	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//ファイル先頭に移動

	//ファイルヘッダの準備
	fhed.size = sizeof(GCD_FILEHEADER);
	fhed.ver  = GCD_VERSION;
	//ヘッダ部の書き込み
	DWORD br;
	DWORD ret=WriteFile(hFile,&fhed,sizeof(GCD_FILEHEADER),&br,NULL);
	if(!ret || br!=sizeof(GCD_FILEHEADER)){
		CloseHandle(hFile);
		return(2);
	}

	//セル名の準備
	if(pfhnames!=NULL)memcpy(&fh_names,pfhnames,sizeof(GCD_CELLNAMES));
	else ZeroMemory(&fh_names,sizeof(GCD_CELLNAMES));
	//セル名の書き込み
	ret=WriteFile(hFile,&fh_names,sizeof(GCD_CELLNAMES),&br,NULL);
	if(!ret || br!=sizeof(GCD_CELLNAMES)){
		CloseHandle(hFile);
		return(3);
	}

	//矩形
	ret=WriteFile(hFile,prdat,sizeof(GCD_RECT)*GCDMAX_RECTANGLES,&br,NULL);
	if(!ret || br!=sizeof(GCD_RECT)*GCDMAX_RECTANGLES){
		CloseHandle(hFile);
		return(4);
	}
	//ＣＥＬＬ
	ret=WriteFile(hFile,pcdat,sizeof(GCD_CELL2)*GCDMAX_CELLS,&br,NULL);
	if(!ret || br!=sizeof(GCD_CELL2)*GCDMAX_CELLS){
		CloseHandle(hFile);
		return(5);
	}
	//あたり判定
	ret=WriteFile(hFile,phdat,sizeof(GCD_HANTEI)*GCDMAX_CELLS,&br,NULL);
	if(!ret || br!=sizeof(GCD_HANTEI)*GCDMAX_CELLS){
		CloseHandle(hFile);
		return(6);
	}

	CloseHandle(hFile);

	return(0);
}

/*-----------------------------------------------------------------------
	指定ファイルへ指定バッファのデータをver0.70形式に変換して保存
	戻0が正常終了
-------------------------------------------------------------------------*/
int CGCDHandler::GCDSave070(TCHAR *filename,GCD_CELL2 *pcdat090,GCD_RECT *prdat,GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames)
{
	//形式変換（CELL）
	GCD_CELL2_070 *pcdat070 = new GCD_CELL2_070[GCDMAX_CELLS];
	for(int i=0;i<GCDMAX_CELLS;i++){
		GCDConvCell_090_070(&pcdat090[i],&pcdat070[i]);
	}

	GCD_FILEHEADER	fhed;
	GCD_CELLNAMES	fh_names;

	HANDLE hFile = CreateFile(filename,
		GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		delete [] pcdat070;
		return(1);
	}
	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//ファイル先頭に移動

	//ファイルヘッダの準備
	fhed.size = sizeof(GCD_FILEHEADER);
	fhed.ver  = 700;
	//ヘッダ部の書き込み
	DWORD br;
	DWORD ret=WriteFile(hFile,&fhed,sizeof(GCD_FILEHEADER),&br,NULL);
	if(!ret || br!=sizeof(GCD_FILEHEADER)){
		delete [] pcdat070;
		CloseHandle(hFile);
		return(2);
	}

	//セル名の準備
	if(pfhnames!=NULL)memcpy(&fh_names,pfhnames,sizeof(GCD_CELLNAMES));
	else ZeroMemory(&pfhnames,sizeof(GCD_CELLNAMES));
	//セル名の書き込み
	ret=WriteFile(hFile,&fh_names,sizeof(GCD_CELLNAMES),&br,NULL);
	if(!ret || br!=sizeof(GCD_CELLNAMES)){
		delete [] pcdat070;
		CloseHandle(hFile);
		return(3);
	}

	//矩形
	ret=WriteFile(hFile,prdat,sizeof(GCD_RECT)*GCDMAX_RECTANGLES,&br,NULL);
	if(!ret || br!=sizeof(GCD_RECT)*GCDMAX_RECTANGLES){
		delete [] pcdat070;
		CloseHandle(hFile);
		return(4);
	}
	//ＣＥＬＬ
	ret=WriteFile(hFile,pcdat070,sizeof(GCD_CELL2_070)*GCDMAX_CELLS,&br,NULL);
	if(!ret || br!=sizeof(GCD_CELL2_070)*GCDMAX_CELLS){
		delete [] pcdat070;
		CloseHandle(hFile);
		return(5);
	}
	//あたり判定
	ret=WriteFile(hFile,phdat,sizeof(GCD_HANTEI)*GCDMAX_CELLS,&br,NULL);
	if(!ret || br!=sizeof(GCD_HANTEI)*GCDMAX_CELLS){
		delete [] pcdat070;
		CloseHandle(hFile);
		return(6);
	}

	delete [] pcdat070;
	CloseHandle(hFile);

	return(0);
}


/*-----------------------------------------------------------------------
	定義名保存
	「ファイルを保存」ダイアログを提示し、選択されたファイルにデータを保存
-------------------------------------------------------------------------*/
BOOL CGCDHandler::GCDSaveHeader(GCD_CELLNAMES *pfhnames)
{
	//保存するファイルを選択させる
	TCHAR filepath[256]=_T("");
	TCHAR filename[64];
	OPENFILENAME ofn;
	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = ghwnd;
	ofn.lpstrFilter = _T("C/C++ヘッダファイル(*.h)\0*.h\0すべて(*.*)\0*.*\0\0");
	ofn.lpstrFile = filepath;
	ofn.nMaxFile =256;
	ofn.lpstrFileTitle = filename;
	ofn.nMaxFileTitle = 64;
	ofn.Flags = 0;
	ofn.lpstrTitle = _T("保存");

	if(!GetOpenFileName(&ofn))return(FALSE);

	//拡張子を無理やり.hに
	int len=(int)strlen(filepath);
	if(len>4){
		if(!( filepath[len-4]=='.' &&
			(filepath[len-3]=='h' || filepath[len-3]=='H') ))
		{
			filepath[len] = '.';
			filepath[len+1] = 'h';
		}
	}

	//保存
	HANDLE hFile = CreateFile(filepath,
		GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		MessageBox(ghwnd,_T("ﾌｧｲﾙが開けなかった"),_T("SaveFile()"),MB_OK);
		return(FALSE);
	}

	TCHAR write[256];
	DWORD br;
	DWORD ret;
	for(DWORD i=0;i<GCDMAX_CELLS;i++){
		if(strlen(pfhnames->name[i]) != 0){
			_stprintf(write,_T("#define CELL_%s %d\n"),pfhnames->name[i],i);
			ret=WriteFile(hFile,write,(DWORD)sizeof(TCHAR)*strlen(write),&br,NULL);
		}
	}

	CloseHandle(hFile);
	return(TRUE);
}

/*-----------------------------------------------------------------------
	指定ファイルへ指定バッファのデータを圧縮して保存
	戻0が正常終了
-------------------------------------------------------------------------*/
int CGCDHandler::GCDSaveCompressed(TCHAR *filename,GCD_CELL2 *pcdat,GCD_RECT *prdat,GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames)
{
	GCD_COMP_FILEHEADER	fhed;
	GCD_COMP_DATAHEADER dhed;

	BYTE* Buffer = NULL;
	DWORD size = 0;
	DWORD BufSize = 0;

	HANDLE hFile = CreateFile(filename,
		GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		return(1);
	}
	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//ファイル先頭に移動

	// ファイルヘッダ書き込み
	memcpy(fhed.signature, cmp_sig, 4);
	fhed.ver = GCD_VERSION;
	fhed.flags = 0;
	if (pfhnames) // セル名あり
		fhed.flags |= GCDFILE_COMP_HAVE_CELLNAMES;

	DWORD br;
	DWORD ret=WriteFile(hFile,&fhed,sizeof(GCD_COMP_FILEHEADER),&br,NULL);
	if(!ret || br!=sizeof(GCD_COMP_FILEHEADER)){
		CloseHandle(hFile);
		return(2);
	}

	// セル名
	if (pfhnames)
	{
		int num_names = GCDMAX_CELLS;

		while ( (num_names > 0) && (strlen(pfhnames->name[num_names - 1]) == 0) )
			num_names--;

		size = num_names * GCD_MAXDEFINELENGTH;
		BufSize = size * 2;
		Buffer = (BYTE*)malloc(BufSize);
		if (!Buffer)
		{
			CloseHandle(hFile);
			return(3);
		}

		compress(Buffer, &BufSize, (BYTE*)pfhnames, size);

		// CRC値計算
		DWORD crc32_names = crc32(0L, Z_NULL, 0);

		for (int i = 0; i < GCDMAX_CELLS; i++)
			crc32_names = crc32(crc32_names, (BYTE*)pfhnames->name[i], strlen(pfhnames->name[i]) + 1);

		// データヘッダ用意
		dhed.DataSize = BufSize;
		dhed.CRCCheckSum = crc32_names;

		ret=WriteFile(hFile,&dhed,sizeof(GCD_COMP_DATAHEADER),&br,NULL);
		if(!ret || br!=sizeof(GCD_COMP_DATAHEADER)){
			if (Buffer) { free(Buffer); Buffer = NULL; }
			CloseHandle(hFile);
			return(3);
		}

		ret=WriteFile(hFile,Buffer,BufSize,&br,NULL);
		if(!ret || br!=BufSize){
			if (Buffer) { free(Buffer); Buffer = NULL; }
			CloseHandle(hFile);
			return(3);
		}

		if (Buffer) { free(Buffer); Buffer = NULL; }
	}

	// 矩形
	{
		int num_rects = GCDMAX_RECTANGLES;

		while (
			num_rects > 0 &&
			prdat[num_rects - 1].bmpno == 0 &&
			prdat[num_rects - 1].r.left == 0 &&
			prdat[num_rects - 1].r.top == 0 &&
			prdat[num_rects - 1].r.right == 0 &&
			prdat[num_rects - 1].r.bottom == 0 &&
			prdat[num_rects - 1].center_x == 0 &&
			prdat[num_rects - 1].center_y == 0
			)
			num_rects--;

		size = num_rects * sizeof(GCD_RECT);
		BufSize = size * 2;
		Buffer = (BYTE*)malloc(BufSize);
		if (!Buffer)
		{
			CloseHandle(hFile);
			return(4);
		}

		compress(Buffer, &BufSize, (BYTE*)prdat, size);

		// データヘッダ用意
		dhed.DataSize = BufSize;
		dhed.CRCCheckSum = crc32( crc32(0L, Z_NULL, 0), (BYTE*)prdat, sizeof(GCD_RECT) * GCDMAX_RECTANGLES );

		ret=WriteFile(hFile,&dhed,sizeof(GCD_COMP_DATAHEADER),&br,NULL);
		if(!ret || br!=sizeof(GCD_COMP_DATAHEADER)){
			if (Buffer) { free(Buffer); Buffer = NULL; }
			CloseHandle(hFile);
			return(4);
		}

		ret=WriteFile(hFile,Buffer,BufSize,&br,NULL);
		if(!ret || br!=BufSize){
			if (Buffer) { free(Buffer); Buffer = NULL; }
			CloseHandle(hFile);
			return(4);
		}

		if (Buffer) { free(Buffer); Buffer = NULL; }
	}

	// ＣＥＬＬ
	{
		int num_cells = GCDMAX_CELLS;
		GCD_CELL cell_void[GCD_CELLGROUPNUM];

		// 何にも入ってないセル
		for (int i = 0; i < GCD_CELLGROUPNUM; i++)
		{

			cell_void[i].cdr = 0;
			cell_void[i].dx = 0;
			cell_void[i].dy = 0;
			cell_void[i].flag = 0;
			cell_void[i].magx = 1.0f;
			cell_void[i].magy = 1.0f;
			cell_void[i].rot = 0;
			cell_void[i].red_ratio = 1.0f;
			cell_void[i].green_ratio = 1.0f;
			cell_void[i].blue_ratio = 1.0f;
			cell_void[i].alpha_ratio = 1.0f;
		}

		while (
			num_cells > 0 &&
			pcdat[num_cells - 1].gcx == 0 &&
			pcdat[num_cells - 1].gcy == 0 &&
			pcdat[num_cells - 1].flag == 0
			)
		{
			if ( memcmp(pcdat[num_cells - 1].cell, cell_void, sizeof(GCD_CELL) * GCD_CELLGROUPNUM) != 0 )
				break;

			num_cells--;
		}

		size = num_cells * sizeof(GCD_CELL2);
		BufSize = size * 2;
		Buffer = (BYTE*)malloc(BufSize);
		if (!Buffer)
		{
			CloseHandle(hFile);
			return(5);
		}

		compress(Buffer, &BufSize, (BYTE*)pcdat, size);

		// データヘッダ用意
		dhed.DataSize = BufSize;
		dhed.CRCCheckSum = crc32( crc32(0L, Z_NULL, 0), (BYTE*)pcdat, sizeof(GCD_CELL2) * GCDMAX_CELLS );

		ret=WriteFile(hFile,&dhed,sizeof(GCD_COMP_DATAHEADER),&br,NULL);
		if(!ret || br!=sizeof(GCD_COMP_DATAHEADER)){
			if (Buffer) { free(Buffer); Buffer = NULL; }
			CloseHandle(hFile);
			return(5);
		}

		ret=WriteFile(hFile,Buffer,BufSize,&br,NULL);
		if(!ret || br!=BufSize){
			if (Buffer) { free(Buffer); Buffer = NULL; }
			CloseHandle(hFile);
			return(5);
		}

		if (Buffer) { free(Buffer); Buffer = NULL; }
	}

	// あたり判定
	{
		int num_hantei = GCDMAX_CELLS;
		const RECT hantei_void = { 0, 0, 0, 0 };

		while (
			num_hantei > 0 &&
			memcmp(&phdat[num_hantei - 1].attack[0], &hantei_void, sizeof(RECT)) == 0 &&
			memcmp(&phdat[num_hantei - 1].attack[1], &hantei_void, sizeof(RECT)) == 0 &&
			memcmp(&phdat[num_hantei - 1].attack[2], &hantei_void, sizeof(RECT)) == 0 &&
			memcmp(&phdat[num_hantei - 1].kas[0], &hantei_void, sizeof(RECT)) == 0 &&
			memcmp(&phdat[num_hantei - 1].kas[1], &hantei_void, sizeof(RECT)) == 0 &&
			memcmp(&phdat[num_hantei - 1].kas[2], &hantei_void, sizeof(RECT)) == 0 &&
			memcmp(&phdat[num_hantei - 1].kurai[0], &hantei_void, sizeof(RECT)) == 0 &&
			memcmp(&phdat[num_hantei - 1].kurai[1], &hantei_void, sizeof(RECT)) == 0 &&
			memcmp(&phdat[num_hantei - 1].kurai[2], &hantei_void, sizeof(RECT)) == 0
			)
			num_hantei--;

		size = num_hantei * sizeof(GCD_HANTEI);
		BufSize = size * 2;
		Buffer = (BYTE*)malloc(BufSize);
		if (!Buffer)
		{
			CloseHandle(hFile);
			return(6);
		}

		compress(Buffer, &BufSize, (BYTE*)phdat, size);

		// データヘッダ用意
		dhed.DataSize = BufSize;
		dhed.CRCCheckSum = crc32( crc32(0L, Z_NULL, 0), (BYTE*)phdat, sizeof(GCD_HANTEI) * GCDMAX_CELLS );

		ret=WriteFile(hFile,&dhed,sizeof(GCD_COMP_DATAHEADER),&br,NULL);
		if(!ret || br!=sizeof(GCD_COMP_DATAHEADER)){
			if (Buffer) { free(Buffer); Buffer = NULL; }
			CloseHandle(hFile);
			return(6);
		}

		ret=WriteFile(hFile,Buffer,BufSize,&br,NULL);
		if(!ret || br!=BufSize){
			if (Buffer) { free(Buffer); Buffer = NULL; }
			CloseHandle(hFile);
			return(6);
		}

		if (Buffer) { free(Buffer); Buffer = NULL; }
	}

	CloseHandle(hFile);

	return 0;
}

//**********************************************************************************
//  other
//**********************************************************************************

/*-----------------------------------------------------------------------
	初期データを適用
	全部0が初期値ではないのでややこしい。拡大率のみ1が初期値。
-------------------------------------------------------------------------*/
void CGCDHandler::GCDSetInitialData(GCD_CELL2 *pcdat,GCD_RECT *prdat,GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames)
{
	//一旦全部クリア
	if(pfhnames!=NULL){
		ZeroMemory(pfhnames,sizeof(GCD_CELLNAMES));
	}
	ZeroMemory(prdat,sizeof(GCD_RECT)*GCDMAX_RECTANGLES);
	ZeroMemory(pcdat,sizeof(GCD_CELL2)*GCDMAX_CELLS);
	ZeroMemory(phdat,sizeof(GCD_HANTEI)*GCDMAX_CELLS);

	//拡大率・色率セット
	int i,j;
	for(i=0;i<GCDMAX_CELLS;i++){
		for(j=0;j<8;j++){
			pcdat[i].cell[j].magx = pcdat[i].cell[j].magy = 1.0;
			pcdat[i].cell[j].alpha_ratio = 1.0f;
			pcdat[i].cell[j].red_ratio = 1.0f;
			pcdat[i].cell[j].blue_ratio = 1.0f;
			pcdat[i].cell[j].green_ratio = 1.0f;
		}
	}
}
#endif

/*-----------------------------------------------------------------------
	指定セル・判定　がからであるかどうかを調べる
-------------------------------------------------------------------------*/
BOOL CGCDHandler::GCDIsEmptyCell2(GCD_CELL2& gcdc,GCD_HANTEI& gcdh)
{
	int i;
	
	for(i=0;i<8;i++){
		if(gcdc.cell[i].cdr != 0)return(FALSE);
		if(gcdc.cell[i].dx != 0)return(FALSE);
		if(gcdc.cell[i].dy != 0)return(FALSE);
		if(gcdc.cell[i].flag != 0)return(FALSE);
		if(gcdc.cell[i].magx != 1.0)return(FALSE);
		if(gcdc.cell[i].magy != 1.0)return(FALSE);
		if(gcdc.cell[i].rot != 0)return(FALSE);
		if(gcdc.cell[i].red_ratio != 1.0f)return(FALSE);
		if(gcdc.cell[i].blue_ratio != 1.0f)return(FALSE);
		if(gcdc.cell[i].green_ratio != 1.0f)return(FALSE);
		if(gcdc.cell[i].alpha_ratio != 1.0f)return(FALSE);
	}
	if(gcdc.gcx != 0)return(FALSE);
	if(gcdc.gcy != 0)return(FALSE);
	if(gcdc.flag != 0)return(FALSE);

	for(i=0;i<3;i++){
		if(gcdh.attack[i].bottom != 0)return(FALSE);
		if(gcdh.attack[i].top    != 0)return(FALSE);
		if(gcdh.attack[i].left   != 0)return(FALSE);
		if(gcdh.attack[i].right  != 0)return(FALSE);
		if(gcdh.kas[i].bottom != 0)return(FALSE);
		if(gcdh.kas[i].top    != 0)return(FALSE);
		if(gcdh.kas[i].left   != 0)return(FALSE);
		if(gcdh.kas[i].right  != 0)return(FALSE);
		if(gcdh.kurai[i].bottom != 0)return(FALSE);
		if(gcdh.kurai[i].top    != 0)return(FALSE);
		if(gcdh.kurai[i].left   != 0)return(FALSE);
		if(gcdh.kurai[i].right  != 0)return(FALSE);
	}

	return(TRUE);
}


/*-----------------------------------------------------------------------
	矩形が「空」かどうかを調べる
-------------------------------------------------------------------------*/
BOOL CGCDHandler::GCDIsEmptyRect(GCD_RECT& gcdr)
{
	if(gcdr.bmpno != 0)return(FALSE);
	if(gcdr.center_x != 0)return(FALSE);
	if(gcdr.center_y != 0)return(FALSE);
	if(gcdr.r.bottom != 0)return(FALSE);
	if(gcdr.r.top != 0)return(FALSE);
	if(gcdr.r.left != 0)return(FALSE);
	if(gcdr.r.right != 0)return(FALSE);
	return(TRUE);
}


/*-----------------------------------------------------------------------
	セル・判定　データクリア
-------------------------------------------------------------------------*/
void CGCDHandler::GCDSetEmptyCell2(GCD_CELL2 *pgcdc,GCD_HANTEI *pgcdh)
{
	for(int i=0;i<8;i++){
		pgcdc->cell[i].cdr=0;
		pgcdc->cell[i].dx=0;
		pgcdc->cell[i].dy=0;
		pgcdc->cell[i].flag=0;
		pgcdc->cell[i].magx=1.0f;
		pgcdc->cell[i].magy=1.0f;
		pgcdc->cell[i].rot=0;
		pgcdc->cell[i].red_ratio	= 1.0f;
		pgcdc->cell[i].green_ratio	= 1.0f;
		pgcdc->cell[i].blue_ratio	= 1.0f;
		pgcdc->cell[i].alpha_ratio	= 1.0f;
	}
	pgcdc->flag=0;
	pgcdc->gcx = pgcdc->gcy = 0;

	ZeroMemory(pgcdh,sizeof(GCD_HANTEI));
}


/*-----------------------------------------------------------------------
	矩形データクリア
-------------------------------------------------------------------------*/
void CGCDHandler::GCDSetEmptyRect(GCD_RECT *pgcdr)
{
	ZeroMemory(pgcdr,sizeof(GCD_RECT));
}





/*-----------------------------------------------------------------------
	セルデータ変換 0.70→0.90
-------------------------------------------------------------------------*/
void CGCDHandler::GCDConvCell_070_090(GCD_CELL2_070 *src,GCD_CELL2 *dst)
{
	GCD_HANTEI dmy;
	GCDSetEmptyCell2(dst,&dmy);

	GCD_CELL_070	*src2;
	GCD_CELL		*dst2;

	for(int i=0;i<8;i++)
	{
		src2 = &src->cell[i];
		dst2 = &dst->cell[i];

		dst2->cdr	=src2->cdr;
		dst2->dx	=src2->dx;
		dst2->dy	=src2->dy;
		dst2->flag	=src2->flag;
		dst2->magx	=src2->magx;
		dst2->magy	=src2->magy;
		dst2->rot	=src2->rot;

		dst2->red_ratio		=1.0f;
		dst2->green_ratio	=1.0f;
		dst2->blue_ratio	=1.0f;
		dst2->alpha_ratio	=1.0f;
	}
	dst->flag = src->flag;
	dst->gcx = src->gcx;
	dst->gcy = src->gcy;
}


/*-----------------------------------------------------------------------
	セルデータ変換 0.90→0.70
-------------------------------------------------------------------------*/
void CGCDHandler::GCDConvCell_090_070(GCD_CELL2 *src,GCD_CELL2_070 *dst)
{
	GCD_CELL_070	*dst2;
	GCD_CELL		*src2;

	for(int i=0;i<8;i++)
	{
		src2 = &src->cell[i];
		dst2 = &dst->cell[i];

		dst2->cdr	=src2->cdr;
		dst2->dx	=src2->dx;
		dst2->dy	=src2->dy;
		dst2->flag	=src2->flag;
		dst2->magx	=src2->magx;
		dst2->magy	=src2->magy;
		dst2->rot	=src2->rot;
	}
	dst->flag = src->flag;
	dst->gcx = src->gcx;
	dst->gcy = src->gcy;
}



/**********************************************************************************
	ここより上は全部 static なのでインスタンスがなくても使える
	以下、データ管理部
************************************************************************************/

#ifndef DELETE_ARRAY
#define DELETE_ARRAY(ptr)	if(ptr){delete [] ptr; ptr=NULL;}
#define SAFEDELETE(ptr)	if(ptr){delete ptr; ptr=NULL;}
#endif

/*-----------------------------------------------------------------------
	構築
-------------------------------------------------------------------------*/
CGCDHandler::CGCDHandler()
{
	m_cdat	=NULL;
	m_rdat	=NULL;
	m_hdat	=NULL;
	m_names	=NULL;

	#ifdef GCD_EDITER
	m_cdat  = new GCD_CELL2[GCDMAX_CELLS];
	m_rdat  = new GCD_RECT[GCDMAX_RECTANGLES];
	m_hdat  = new GCD_HANTEI[GCDMAX_CELLS];
	m_names = new GCD_CELLNAMES;
	GCDSetInitialData(m_cdat,m_rdat,m_hdat,m_names);
	#endif
}



/*-----------------------------------------------------------------------
	読み込み
	0が正常終了
-------------------------------------------------------------------------*/
int CGCDHandler::Load(TCHAR *filename,BOOL hload,BOOL nload)
{
	Destroy();

	m_cdat  = new GCD_CELL2[GCDMAX_CELLS];
	m_rdat  = new GCD_RECT[GCDMAX_RECTANGLES];
	m_hdat  = new GCD_HANTEI[GCDMAX_CELLS];
	m_names = new GCD_CELLNAMES;

	int ret = GCDLoadCompressed(filename,m_cdat,m_rdat,m_hdat,m_names);
	if(ret!=0)	//失敗･･･
	{
		ret = GCDLoad(filename,m_cdat,m_rdat,m_hdat,m_names);
		if(ret!=0)Destroy();	//　　ま　　た　　失　　敗　　か
	}

	return ret;
}


/*-----------------------------------------------------------------------
	データ破棄
-------------------------------------------------------------------------*/
void CGCDHandler::Destroy()
{
	DELETE_ARRAY(m_cdat);
	DELETE_ARRAY(m_rdat);
	DELETE_ARRAY(m_hdat);
	SAFEDELETE(m_names);
}


/*-----------------------------------------------------------------------
	データを圧縮する
	･･･といっても後の方にある空のデータを削ってバッファを短くするだけ
-------------------------------------------------------------------------*/
void CGCDHandler::Diet()
{
	//まだー
}



/*-----------------------------------------------------------------------
	データ保存
-------------------------------------------------------------------------*/
#ifdef GCD_EDITER
BOOL CGCDHandler::Save()
{
	if(!valid()){
		MessageBox(ghwnd,_T("データが読み込まれてないよ"),_T(""),MB_OK);
		return FALSE;
	}

	return GCDSaveDlg(m_cdat,m_rdat,m_hdat,m_names);
}
#endif//GCD_EDITER


