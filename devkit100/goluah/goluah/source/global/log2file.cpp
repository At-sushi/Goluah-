
/*============================================================================

	�t�@�C���ւ̃��O�ۑ��N���X

==============================================================================*/

#include "stdafx.h"

#include "global.h"		//config , FileExist ���K�v
#include "log2file.h"

//�o�b�t�@�T�C�Y
#define LOG2FILE_BUFFERSIZE (1024*16)		//16KB buffer
#define LOG2FILE_DIR		"system\\log"	//�ۑ��f�B���N�g��
#define LOG2FILE_PREFIX		""				//�t�@�C���̐ړ��q


/*!
*	�\�z
*/
CLog2File::CLog2File()
{
	//�o�b�t�@����
	buf = new char[LOG2FILE_BUFFERSIZE];
	ZeroMemory(buf,LOG2FILE_BUFFERSIZE);
	p = buf;
	filename = new char[256];

	//�ۑ��t�@�C����������
	time_t crnt_time;
	time(&crnt_time);
	struct tm* crnt_time_l = localtime(&crnt_time);
	sprintf(filename,"%s\\%s%d%s%d%s%d%s%d%s%d%s%d.txt",
		LOG2FILE_DIR,LOG2FILE_PREFIX,				//�v���t�B�b�N�X
		crnt_time_l->tm_year + 1900,				//�N
		(crnt_time_l->tm_mon + 1)<10 ? "0" : "",
		crnt_time_l->tm_mon + 1,					//��
		crnt_time_l->tm_mday<10 ? "0" : "",
		crnt_time_l->tm_mday,						//��
		crnt_time_l->tm_hour<10 ? "0" : "",
		crnt_time_l->tm_hour,						//��
		crnt_time_l->tm_min<10 ? "0" : "",
		crnt_time_l->tm_min,						//��
		crnt_time_l->tm_sec<10 ? "0" : "",
		crnt_time_l->tm_sec							//�b
	);
}

/*!
*	�j��
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

		// ����ۂ������炠�ڂ��
		if (fsize == 0)
			DeleteFile(filename);
	}

	delete [] buf;
	delete [] filename;
}


/*!
*	�o�b�t�@���e���t�@�C���֏�������
*	�ʏ�̓o�b�t�@���S���ƂȂ��Ȃ��Ă����Ƃ��Ɏ����I�ɍs����B
*	�d�v�ȃ��O��f�����ꍇ�͖����I�ɍs���B
*/
void CLog2File::Flush()
{
	if(!g_config.UseLog2File())return;

	HANDLE hFile = CreateFile(filename,
		GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		return;
	}
	//�t�@�C���ʒu���t�@�C���I�[�Ɉړ�
	SetFilePointer(hFile,0,NULL,FILE_END);

	//��������
	DWORD wsize = p - buf;
	DWORD br;
	DWORD ret=WriteFile(hFile,buf,wsize,&br,NULL);
	CloseHandle(hFile);

	ZeroMemory(buf,LOG2FILE_BUFFERSIZE);
	p = buf;

	return;
}



/*!
*	���O�ǉ�
*	�o�b�t�@������̂ŁA���ڃt�@�C���֍s�����Ⴄ�킯�ł͂Ȃ��B
*	�K�v�������Flush�𖾎��I�Ɏ��s���邱�ƁB
*
*	@param str �f���o��������B�����ɉ��s�������t������̂Œ��ӁB
*/
void CLog2File::AddLog(char *str)
{
	if(!g_config.UseLog2File())return;

	if(p+256 > buf+LOG2FILE_BUFFERSIZE)Flush();
	size_t strsize = strlen(str);
	memcpy(p,str,strsize);
	p+= strsize;
	//window ���s�R�[�h�}��
	*p++ = 13;
	*p++ = 10;
}


