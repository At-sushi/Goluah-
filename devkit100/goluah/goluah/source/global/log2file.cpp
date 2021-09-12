
/*============================================================================

	ファイルへのログ保存クラス

==============================================================================*/

#include "stdafx.h"

#include "global.h"		//config , FileExist が必要
#include "log2file.h"

//バッファサイズ
#define LOG2FILE_BUFFERSIZE (1024*16)		//16KB buffer
#define LOG2FILE_DIR		"system\\log"	//保存ディレクトリ
#define LOG2FILE_PREFIX		""				//ファイルの接頭子


/*!
*	構築
*/
CLog2File::CLog2File()
{
	//バッファ生成
	buf = new char[LOG2FILE_BUFFERSIZE];
	ZeroMemory(buf,LOG2FILE_BUFFERSIZE);
	p = buf;
	filename = new char[256];

	//保存ファイル名を決定
	time_t crnt_time;
	time(&crnt_time);
	struct tm* crnt_time_l = localtime(&crnt_time);
	sprintf(filename,"%s\\%s%d%s%d%s%d%s%d%s%d%s%d.txt",
		LOG2FILE_DIR,LOG2FILE_PREFIX,				//プレフィックス
		crnt_time_l->tm_year + 1900,				//年
		(crnt_time_l->tm_mon + 1)<10 ? "0" : "",
		crnt_time_l->tm_mon + 1,					//月
		crnt_time_l->tm_mday<10 ? "0" : "",
		crnt_time_l->tm_mday,						//日
		crnt_time_l->tm_hour<10 ? "0" : "",
		crnt_time_l->tm_hour,						//時
		crnt_time_l->tm_min<10 ? "0" : "",
		crnt_time_l->tm_min,						//分
		crnt_time_l->tm_sec<10 ? "0" : "",
		crnt_time_l->tm_sec							//秒
	);
}

/*!
*	破棄
*/
CLog2File::~CLog2File()
{
	if(strlen(buf)!=0)Flush();

	HANDLE hFile = CreateFile(filename,
		GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD fsize = GetFileSize(hFile, NULL);

		CloseHandle(hFile);

		// 空っぽだったらあぼんぬ
		if (fsize == 0)
			DeleteFile(filename);
	}

	delete [] buf;
	delete [] filename;
}


/*!
*	バッファ内容をファイルへ書き込み
*	通常はバッファが心もとなくなってきたときに自動的に行われる。
*	重要なログを吐いた場合は明示的に行う。
*/
void CLog2File::Flush()
{
	if(!g_config.UseLog2File())return;

	HANDLE hFile = CreateFile(filename,
		GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		return;
	}
	//ファイル位置をファイル終端に移動
	SetFilePointer(hFile,0,NULL,FILE_END);

	//書き込み
	DWORD wsize = p - buf;
	DWORD br;
	DWORD ret=WriteFile(hFile,buf,wsize,&br,NULL);
	CloseHandle(hFile);

	ZeroMemory(buf,LOG2FILE_BUFFERSIZE);
	p = buf;

	return;
}



/*!
*	ログ追加
*	バッファがあるので、直接ファイルへ行っちゃうわけではない。
*	必要があればFlushを明示的に実行すること。
*
*	@param str 吐き出す文字列。末尾に改行を自動付加するので注意。
*/
void CLog2File::AddLog(char *str)
{
	if(!g_config.UseLog2File())return;

	if(p+256 > buf+LOG2FILE_BUFFERSIZE)Flush();
	size_t strsize = strlen(str);
	memcpy(p,str,strsize);
	p+= strsize;
	//window 改行コード挿入
	*p++ = 13;
	*p++ = 10;
}


