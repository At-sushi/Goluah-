
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


// �V�O�l�C�`��
const BYTE cmp_sig[] = _T("GCDC");

//**********************************************************************************
//  load
//**********************************************************************************

#ifdef GCD_EDITER

/*!
*	�u�t�@�C�����J���v�_�C�A���O��񎦂��ăt�@�C�����[�h
*/
BOOL CGCDHandler::GCDLoadDlg(GCD_CELL2 *pcdat,GCD_RECT *prdat,GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames)
{
	char filepath[256]=_T("");
	char filename[64];
	OPENFILENAME ofn;
	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = ghwnd;
	ofn.lpstrFilter = _T("�f�[�^(*.gcd)\0*.gcd\0���ׂ�(*.*)\0*.*\0\0");
	ofn.lpstrFile = filepath;
	ofn.nMaxFile =256;
	ofn.lpstrFileTitle = filename;
	ofn.nMaxFileTitle = 64;
	ofn.Flags = OFN_FILEMUSTEXIST;
	ofn.lpstrTitle = _T("�J��");

	if(!GetOpenFileName(&ofn))return(FALSE);

	switch(GCDLoad(filepath,pcdat,prdat,phdat,pfhnames,GCD_VERSION)){
	case 0://����
		return(TRUE);
	default:MessageBox(ghwnd,_T("�Ǎ��Ɏ��s���܂���"),_T(""),MB_OK);
	}
	
	return(FALSE);
}

#endif


/*-----------------------------------------------------------------------
	�ǂݍ���
	�t�@�C���̃o�[�W�����E�o�b�t�@�̃o�[�W�����w��Ɋ�Â��ĉ\�Ȍ���
	�K�؂ȕϊ����{�����[�h�֐��ɏ������܂킷�B

	��0�Ő���I��
-------------------------------------------------------------------------*/

int CGCDHandler::GCDLoad(char *filename,LPVOID pcdat,LPVOID prdat,LPVOID phdat,LPVOID pfhnames,DWORD version)
{
	//�t�@�C���I�[�v��
	HANDLE hFile = CreateFile(filename,
		GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		return(1);
	}

	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//�t�@�C���擪�Ɉړ�
	//�w�b�_���̓ǂݏo��
	DWORD br;
	DWORD buff[2];
	DWORD ret=ReadFile(hFile,buff,8,&br,NULL);
	if(!ret || br!=8){
		CloseHandle(hFile);
		return(2);
	}
	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//�t�@�C���擪�Ɉړ�
	
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

	//�o�[�W������񖄂ߍ���
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
	0.90 �ǂݍ���
	�t�@�C������̒��ڃ��[�h�̓e���|�����̃o�b�t�@�֍s���A
	�S�Ẵv���Z�X���I��������w��o�b�t�@�ւ̃R�s�[���s��

	0.90�ȊO�̃o�b�t�@�ւ̕ϊ��͍s��Ȃ�

	��0�Ő���I��
-------------------------------------------------------------------------*/
int CGCDHandler::GCDLoad090(HANDLE hFile,LPVOID pcdat,LPVOID prdat,LPVOID phdat,LPVOID pfhnames,DWORD buff_version)
{
	if(buff_version<900)return 8;

	//�e���|�����p
	GCD_FILEHEADER_090	fhed;
	GCD_CELLNAMES_090	fh_names;
	GCD_RECT_090		*dm_rdat = new GCD_RECT_090[GCDMAX_RECTANGLES];
	GCD_CELL2_090		*dm_cdat = new GCD_CELL2_090[GCDMAX_CELLS];
	GCD_HANTEI_090		*dm_hdat = new GCD_HANTEI_090[GCDMAX_CELLS];

	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//�t�@�C���擪�Ɉړ�
	DWORD ret = 0;

	try{
		//�w�b�_���̓ǂݏo��
		DWORD br;
		DWORD ret=ReadFile(hFile,&fhed,sizeof(GCD_FILEHEADER_090),&br,NULL);
		if(!ret || br!=sizeof(GCD_FILEHEADER_090)){
			ret=2;throw;
		}

		//�Z������ǂݏo��
		ret=ReadFile(hFile,&fh_names,sizeof(GCD_CELLNAMES_090),&br,NULL);
		if(!ret || br!=sizeof(GCD_CELLNAMES_090)){
			ret=4;throw;
		}

		//��`
		ret=ReadFile(hFile,dm_rdat,sizeof(GCD_RECT_090)*GCDMAX_RECTANGLES,&br,NULL);
		if(!ret || br!=sizeof(GCD_RECT_090)*GCDMAX_RECTANGLES){
			ret=5;throw;
		}
#		ifndef GCD_EDITER
			//�i�s�󋵕\��
			CTNowLoading* task = dynamic_cast<CTNowLoading*>( g_system.FindTask('LOAD') );
			if(task)task->Progress(NowLoading_GCD, 0.5f);		/* �ʂ͓K�� */
#		endif
		//�b�d�k�k
		ret=ReadFile(hFile,dm_cdat,sizeof(GCD_CELL2_090)*GCDMAX_CELLS,&br,NULL);
		if(!ret || br!=sizeof(GCD_CELL2_090)*GCDMAX_CELLS){
			ret=6;throw;
		}
		//�����蔻��
		ret=ReadFile(hFile,dm_hdat,sizeof(GCD_HANTEI_090)*GCDMAX_CELLS,&br,NULL);
		if(!ret || br!=sizeof(GCD_HANTEI_090)*GCDMAX_CELLS){
			ret=7;throw;
		}
	}
	catch(...){
		ODS(_T("��GCDHandler : ver0.90 GCD load failed"));
	}

	if(ret==0){
		//�����E�n���ꂽ�o�b�t�@�փf�[�^���R�s�[
		if(pfhnames!=NULL){
			memcpy(pfhnames,&fh_names,sizeof(GCD_CELLNAMES));
		}
		if(prdat!=NULL)memcpy(prdat,dm_rdat,sizeof(GCD_RECT_090)*GCDMAX_RECTANGLES);
		if(pcdat!=NULL)memcpy(pcdat,dm_cdat,sizeof(GCD_CELL2_090)*GCDMAX_CELLS);
		if(phdat!=NULL)memcpy(phdat,dm_hdat,sizeof(GCD_HANTEI_090)*GCDMAX_CELLS);

		ODS(_T("��GCDHandler : ver0.90 GCD load comlete\n"));
	}
	delete [] dm_rdat;
	delete [] dm_cdat;
	delete [] dm_hdat;

	//�o�[�W������񖄂ߍ���
	if(pcdat != NULL) ((GCD_CELL2_090*)pcdat)[0].cell[0].flag = 900;
	
	return(0);
}

/*-----------------------------------------------------------------------
	GCDver0.70�ǂݍ���
	�t�@�C������̒��ڃ��[�h�̓e���|�����̃o�b�t�@�֍s���A
	�S�Ẵv���Z�X���I��������w��o�b�t�@�ւ̃R�s�[���s��
	0.70�܂���0.90�`���̃o�b�t�@�ɓǂݍ��߂�

	��0�Ő���I��
-------------------------------------------------------------------------*/
int CGCDHandler::GCDLoad070(HANDLE hFile,LPVOID pcdat,LPVOID prdat,LPVOID phdat,LPVOID pfhnames,DWORD buff_version)
{
	if(buff_version<700)return 8;

	//�e���|�����p
	GCD_FILEHEADER_070	fhed;
	GCD_CELLNAMES_070	fh_names;
	GCD_RECT_070		*dm_rdat = new GCD_RECT_070[GCDMAX_RECTANGLES];
	GCD_CELL2_070		*dm_cdat = new GCD_CELL2_070[GCDMAX_CELLS];
	GCD_HANTEI_070		*dm_hdat = new GCD_HANTEI_070[GCDMAX_CELLS];

	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//�t�@�C���擪�Ɉړ�
	DWORD ret = 0;

	try{
		//�w�b�_���̓ǂݏo��
		DWORD br;
		DWORD ret=ReadFile(hFile,&fhed,sizeof(GCD_FILEHEADER_070),&br,NULL);
		if(!ret || br!=sizeof(GCD_FILEHEADER_070)){
			ret=2;throw;
		}

		//�Z������ǂݏo��
		ret=ReadFile(hFile,&fh_names,sizeof(GCD_CELLNAMES_070),&br,NULL);
		if(!ret || br!=sizeof(GCD_CELLNAMES_070)){
			ret=4;throw;
		}

		//��`
		ret=ReadFile(hFile,dm_rdat,sizeof(GCD_RECT_070)*GCDMAX_RECTANGLES,&br,NULL);
		if(!ret || br!=sizeof(GCD_RECT_070)*GCDMAX_RECTANGLES){
			ret=5;throw;
		}
		//�b�d�k�k
		ret=ReadFile(hFile,dm_cdat,sizeof(GCD_CELL2_070)*GCDMAX_CELLS,&br,NULL);
		if(!ret || br!=sizeof(GCD_CELL2_070)*GCDMAX_CELLS){
			ret=6;throw;
		}
		//�����蔻��
		ret=ReadFile(hFile,dm_hdat,sizeof(GCD_HANTEI_070)*GCDMAX_CELLS,&br,NULL);
		if(!ret || br!=sizeof(GCD_HANTEI_070)*GCDMAX_CELLS){
			ret=7;throw;
		}
	}
	catch(...){
		ODS(_T("��GCDHandler : ver0.70 GCD load failed\n"));
	}

	if(ret==0){//�����E�n���ꂽ�o�b�t�@�փf�[�^���R�s�[
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
					ODS(_T("��GCDHandler : ver0.70��ver0.90 GCD load comlete"));

					//�o�[�W������񖄂ߍ���
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
					ODS(_T("��GCDHandler : ver0.70 GCD load comlete\n"));

					//�o�[�W������񖄂ߍ���
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
	�ǂݍ���
	�t�@�C���̃o�[�W�����E�o�b�t�@�̃o�[�W�����w��Ɋ�Â��ĉ\�Ȍ���
	�K�؂ȕϊ����{�����[�h�֐��ɏ������܂킷�B

	��0�Ő���I��
-------------------------------------------------------------------------*/
int CGCDHandler::GCDLoadCompressed(char *filename,LPVOID pcdat,LPVOID prdat,LPVOID phdat,LPVOID pfhnames,DWORD version)
{
	BYTE* Buffer = NULL;
	DWORD size = 0;
	GCD_COMP_FILEHEADER fhead;
	GCD_COMP_DATAHEADER dhead;
#ifdef GCD_EDITER
	DWORD CRCSum;
#endif

	//�t�@�C���I�[�v��
	HANDLE hFile = CreateFile(filename,
		GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		return(1);
	}

	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//�t�@�C���擪�Ɉړ�

	//�w�b�_���̓ǂݏo��
	DWORD br;
	DWORD ret=ReadFile(hFile,&fhead,sizeof(fhead),&br,NULL);
	if(!ret || br!=sizeof(fhead)){
		CloseHandle(hFile);
		return(2);
	}

	// �w�b�_�`�F�b�N
	if ( memcmp(fhead.signature, cmp_sig, 4) != 0 || fhead.ver != version || fhead.flags & GCDFILE_COMP_USING_FILTER )
	{
		CloseHandle(hFile);
		return 9;
	}

	// �Z����
	if (pfhnames)
	{
		ZeroMemory(pfhnames, sizeof(GCD_CELLNAMES_090));

		if (fhead.flags & GCDFILE_COMP_HAVE_CELLNAMES)
		{
			// �Z��������
			// �f�[�^�w�b�_�ǂݍ���
			ret=ReadFile(hFile,&dhead,sizeof(dhead),&br,NULL);
			if(!ret || br!=sizeof(dhead)){
				CloseHandle(hFile);
				return(4);
			}

			// �ǂݍ���
			Buffer = (BYTE*)malloc(dhead.DataSize);
			ret=ReadFile(hFile,Buffer,dhead.DataSize,&br,NULL);
			if(!ret || br!=dhead.DataSize){
				CloseHandle(hFile);
				if (Buffer) free(Buffer);
				return(4);
			}

			// �W�J
			size = sizeof(GCD_CELLNAMES_090);
			uncompress((BYTE*)pfhnames, &size, Buffer, dhead.DataSize);
			if (Buffer) free(Buffer);

#ifdef GCD_EDITER
			CRCSum = crc32(0L, Z_NULL, 0);
			for (int i = 0; i < GCDMAX_CELLS; i++)
				CRCSum = crc32( CRCSum, (BYTE*)((GCD_CELLNAMES_090*)pfhnames)->name[i], strlen(((GCD_CELLNAMES_090*)pfhnames)->name[i]) + 1 );

			if (CRCSum != dhead.CRCCheckSum)
			{
				if ( MessageBox(NULL, _T("CRC�G���[�i�Z����`���j�F\n���̃f�[�^�͔j�����Ă���\��������܂��B\n\n�����𑱍s���܂����H"),
							_T("�閶�捡������肪�Ƃ�"), MB_YESNO | MB_ICONWARNING) == IDNO )
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
		// �Z�������邯�ǃf�[�^����ĂȂ���
		// �f�[�^�w�b�_�ǂݍ���
		ret=ReadFile(hFile,&dhead,sizeof(dhead),&br,NULL);
		if(!ret || br!=sizeof(dhead)){
			CloseHandle(hFile);
			return(4);
		}

		// ������΂�
		SetFilePointer(hFile, dhead.DataSize, NULL, FILE_CURRENT);
	}

	// ��`
	ZeroMemory(prdat, sizeof(GCD_RECT_090) * GCDMAX_RECTANGLES);

	// �f�[�^�w�b�_�ǂݍ���
	ret=ReadFile(hFile,&dhead,sizeof(dhead),&br,NULL);
	if(!ret || br!=sizeof(dhead)){
		CloseHandle(hFile);
		return(5);
	}

	if (prdat)
	{
		// �ǂݍ���
		Buffer = (BYTE*)malloc(dhead.DataSize);
		ret=ReadFile(hFile,Buffer,dhead.DataSize,&br,NULL);
		if(!ret || br!=dhead.DataSize){
			CloseHandle(hFile);
			if (Buffer) free(Buffer);
			return(5);
		}

		// �W�J
		size = sizeof(GCD_RECT_090) * GCDMAX_RECTANGLES;
		uncompress((BYTE*)prdat, &size, Buffer, dhead.DataSize);
		if (Buffer) free(Buffer);

#		ifdef GCD_EDITER
			CRCSum = crc32( crc32(0L, Z_NULL, 0), (BYTE*)prdat, sizeof(GCD_RECT_090) * GCDMAX_RECTANGLES );
			if (CRCSum != dhead.CRCCheckSum)
			{
				if ( MessageBox(NULL, _T("CRC�G���[�i��`�f�[�^�j�F\n���̃f�[�^�͔j�����Ă���\��������܂��B\n\n�����𑱍s���܂����H"),
							_T("�閶�捡������肪�Ƃ�"), MB_YESNO | MB_ICONWARNING) == IDNO )
				{
					CloseHandle(hFile);
					return 5;
				}
			}
#		endif
	}
	else
		SetFilePointer(hFile, dhead.DataSize, NULL, FILE_CURRENT);

	// �Z��
	// ���ɂ������ĂȂ��Z��
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

	// �f�[�^�w�b�_�ǂݍ���
	ret=ReadFile(hFile,&dhead,sizeof(dhead),&br,NULL);
	if(!ret || br!=sizeof(dhead)){
		CloseHandle(hFile);
		return(6);
	}

	if (pcdat)
	{
		// �ǂݍ���
		Buffer = (BYTE*)malloc(dhead.DataSize);
		ret=ReadFile(hFile,Buffer,dhead.DataSize,&br,NULL);
		if(!ret || br!=dhead.DataSize){
			CloseHandle(hFile);
			if (Buffer) free(Buffer);
			return(6);
		}

		// �W�J
		size = sizeof(GCD_CELL2_090) * GCDMAX_CELLS;
		uncompress((BYTE*)pcdat, &size, Buffer, dhead.DataSize);
		if (Buffer) free(Buffer);

#		ifdef GCD_EDITER
			CRCSum = crc32( crc32(0L, Z_NULL, 0), (BYTE*)pcdat, sizeof(GCD_CELL2_090) * GCDMAX_CELLS );
			if (CRCSum != dhead.CRCCheckSum)
			{
				if ( MessageBox(NULL, _T("CRC�G���[�i�Z���f�[�^�j�F\n���̃f�[�^�͔j�����Ă���\��������܂��B\n\n�����𑱍s���܂����H"),
							_T("�閶�捡������肪�Ƃ�"), MB_YESNO | MB_ICONWARNING) == IDNO )
				{
					CloseHandle(hFile);
					return 6;
				}
			}
#		endif
	}
	else
		SetFilePointer(hFile, dhead.DataSize, NULL, FILE_CURRENT);

	// �����蔻��
	ZeroMemory(phdat, sizeof(GCD_HANTEI_090) * GCDMAX_CELLS);

	// �f�[�^�w�b�_�ǂݍ���
	ret=ReadFile(hFile,&dhead,sizeof(dhead),&br,NULL);
	if(!ret || br!=sizeof(dhead)){
		CloseHandle(hFile);
		return(7);
	}

	if (phdat)
	{
		// �ǂݍ���
		Buffer = (BYTE*)malloc(dhead.DataSize);
		ret=ReadFile(hFile,Buffer,dhead.DataSize,&br,NULL);
		if(!ret || br!=dhead.DataSize){
			CloseHandle(hFile);
			if (Buffer) free(Buffer);
			return(7);
		}

		// �W�J
		size = sizeof(GCD_HANTEI_090) * GCDMAX_CELLS;
		uncompress((BYTE*)phdat, &size, Buffer, dhead.DataSize);
		if (Buffer) free(Buffer);

#		ifdef GCD_EDITER
			CRCSum = crc32( crc32(0L, Z_NULL, 0), (BYTE*)phdat, sizeof(GCD_HANTEI_090) * GCDMAX_CELLS );
			if (CRCSum != dhead.CRCCheckSum)
			{
				if ( MessageBox(NULL, _T("CRC�G���[�i�����蔻��f�[�^�j�F\n���̃f�[�^�͔j�����Ă���\��������܂��B\n\n�����𑱍s���܂����H"),
							_T("�閶�捡������肪�Ƃ�"), MB_YESNO | MB_ICONWARNING) == IDNO )
				{
					CloseHandle(hFile);
					return 7;
				}
			}
#		endif
	}
	else
		SetFilePointer(hFile, dhead.DataSize, NULL, FILE_CURRENT);

	//�o�[�W������񖄂ߍ���
	if(pcdat != NULL) ((GCD_CELL2_090*)pcdat)[0].cell[0].flag = version;
	
	CloseHandle(hFile);

	return 0;
}


//**********************************************************************************
//  save
//**********************************************************************************

#ifdef GCD_EDITER


/*-----------------------------------------------------------------------
	�u�t�@�C����ۑ��v�_�C�A���O��񎦂��ăt�@�C����ۑ�
-------------------------------------------------------------------------*/
BOOL CGCDHandler::GCDSaveDlg(GCD_CELL2 *pcdat,GCD_RECT *prdat,GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames)
{
	char filepath[256]=_T("");
	char filename[64];
	OPENFILENAME ofn;
	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = ghwnd;
	ofn.lpstrFilter = _T("GCD0.90�`��(*.gcd)\0*.gcd\0���kGCD0.90�`��(*.gcm)\0*.gcm\0GCD0.70�`��(*.gcd)\0*.gcd\0���ׂẴt�@�C��(*.*)\0*.*\0\0");
	ofn.lpstrDefExt = _T("gcd");
	ofn.lpstrFile = filepath;
	ofn.nMaxFile =256;
	ofn.lpstrFileTitle = filename;
	ofn.nMaxFileTitle = 64;
	ofn.Flags = OFN_OVERWRITEPROMPT;
	ofn.lpstrTitle = _T("�ۑ�");

	if(!GetSaveFileName(&ofn))return(FALSE);

	//�ۑ�
	int ret;
	switch(ofn.nFilterIndex){
	case 3://0.70�t�H�[�}�b�g�ŕۑ�
		ret = GCDSave070(filepath,pcdat,prdat,phdat,pfhnames);break;
	case 2://���k���ĕۑ�
		if (strcmp(filepath + strlen(filepath) - 4, _T(".gcm")) == 0)
		{
			ret = GCDSaveCompressed(filepath,pcdat,prdat,phdat,pfhnames);break;
		}
	default:
		ret = GCDSave(filepath,pcdat,prdat,phdat,pfhnames);
	}

	//�G���[���b�Z�[�W�\��
	switch(ret){
	case 0:return(TRUE);
	case 1:MessageBox(ghwnd,_T("�t�@�C���I�[�v���Ɏ��s"),_T("�ۑ��Ɏ��s"),MB_OK);break;
	case 2:MessageBox(ghwnd,_T("�������ݎ��s(2)"),_T("�ۑ��Ɏ��s"),MB_OK);break;
	case 3:MessageBox(ghwnd,_T("�������ݎ��s(3)"),_T("�ۑ��Ɏ��s"),MB_OK);break;
	case 4:MessageBox(ghwnd,_T("�������ݎ��s(4)"),_T("�ۑ��Ɏ��s"),MB_OK);break;
	case 5:MessageBox(ghwnd,_T("�������ݎ��s(5)"),_T("�ۑ��Ɏ��s"),MB_OK);break;
	case 6:MessageBox(ghwnd,_T("�������ݎ��s(6)"),_T("�ۑ��Ɏ��s"),MB_OK);break;
	}
	return(FALSE);
}


/*-----------------------------------------------------------------------
	�w��t�@�C���֎w��o�b�t�@�̃f�[�^��ۑ�
	��0������I��
-------------------------------------------------------------------------*/
int CGCDHandler::GCDSave(char *filename,GCD_CELL2 *pcdat,GCD_RECT *prdat,GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames)
{
	GCD_FILEHEADER	fhed;
	GCD_CELLNAMES	fh_names;

	HANDLE hFile = CreateFile(filename,
		GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		return(1);
	}
	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//�t�@�C���擪�Ɉړ�

	//�t�@�C���w�b�_�̏���
	fhed.size = sizeof(GCD_FILEHEADER);
	fhed.ver  = GCD_VERSION;
	//�w�b�_���̏�������
	DWORD br;
	DWORD ret=WriteFile(hFile,&fhed,sizeof(GCD_FILEHEADER),&br,NULL);
	if(!ret || br!=sizeof(GCD_FILEHEADER)){
		CloseHandle(hFile);
		return(2);
	}

	//�Z�����̏���
	if(pfhnames!=NULL)memcpy(&fh_names,pfhnames,sizeof(GCD_CELLNAMES));
	else ZeroMemory(&fh_names,sizeof(GCD_CELLNAMES));
	//�Z�����̏�������
	ret=WriteFile(hFile,&fh_names,sizeof(GCD_CELLNAMES),&br,NULL);
	if(!ret || br!=sizeof(GCD_CELLNAMES)){
		CloseHandle(hFile);
		return(3);
	}

	//��`
	ret=WriteFile(hFile,prdat,sizeof(GCD_RECT)*GCDMAX_RECTANGLES,&br,NULL);
	if(!ret || br!=sizeof(GCD_RECT)*GCDMAX_RECTANGLES){
		CloseHandle(hFile);
		return(4);
	}
	//�b�d�k�k
	ret=WriteFile(hFile,pcdat,sizeof(GCD_CELL2)*GCDMAX_CELLS,&br,NULL);
	if(!ret || br!=sizeof(GCD_CELL2)*GCDMAX_CELLS){
		CloseHandle(hFile);
		return(5);
	}
	//�����蔻��
	ret=WriteFile(hFile,phdat,sizeof(GCD_HANTEI)*GCDMAX_CELLS,&br,NULL);
	if(!ret || br!=sizeof(GCD_HANTEI)*GCDMAX_CELLS){
		CloseHandle(hFile);
		return(6);
	}

	CloseHandle(hFile);

	return(0);
}

/*-----------------------------------------------------------------------
	�w��t�@�C���֎w��o�b�t�@�̃f�[�^��ver0.70�`���ɕϊ����ĕۑ�
	��0������I��
-------------------------------------------------------------------------*/
int CGCDHandler::GCDSave070(char *filename,GCD_CELL2 *pcdat090,GCD_RECT *prdat,GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames)
{
	//�`���ϊ��iCELL�j
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
	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//�t�@�C���擪�Ɉړ�

	//�t�@�C���w�b�_�̏���
	fhed.size = sizeof(GCD_FILEHEADER);
	fhed.ver  = 700;
	//�w�b�_���̏�������
	DWORD br;
	DWORD ret=WriteFile(hFile,&fhed,sizeof(GCD_FILEHEADER),&br,NULL);
	if(!ret || br!=sizeof(GCD_FILEHEADER)){
		delete [] pcdat070;
		CloseHandle(hFile);
		return(2);
	}

	//�Z�����̏���
	if(pfhnames!=NULL)memcpy(&fh_names,pfhnames,sizeof(GCD_CELLNAMES));
	else ZeroMemory(&pfhnames,sizeof(GCD_CELLNAMES));
	//�Z�����̏�������
	ret=WriteFile(hFile,&fh_names,sizeof(GCD_CELLNAMES),&br,NULL);
	if(!ret || br!=sizeof(GCD_CELLNAMES)){
		delete [] pcdat070;
		CloseHandle(hFile);
		return(3);
	}

	//��`
	ret=WriteFile(hFile,prdat,sizeof(GCD_RECT)*GCDMAX_RECTANGLES,&br,NULL);
	if(!ret || br!=sizeof(GCD_RECT)*GCDMAX_RECTANGLES){
		delete [] pcdat070;
		CloseHandle(hFile);
		return(4);
	}
	//�b�d�k�k
	ret=WriteFile(hFile,pcdat070,sizeof(GCD_CELL2_070)*GCDMAX_CELLS,&br,NULL);
	if(!ret || br!=sizeof(GCD_CELL2_070)*GCDMAX_CELLS){
		delete [] pcdat070;
		CloseHandle(hFile);
		return(5);
	}
	//�����蔻��
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
	��`���ۑ�
	�u�t�@�C����ۑ��v�_�C�A���O��񎦂��A�I�����ꂽ�t�@�C���Ƀf�[�^��ۑ�
-------------------------------------------------------------------------*/
BOOL CGCDHandler::GCDSaveHeader(GCD_CELLNAMES *pfhnames)
{
	//�ۑ�����t�@�C����I��������
	char filepath[256]=_T("");
	char filename[64];
	OPENFILENAME ofn;
	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = ghwnd;
	ofn.lpstrFilter = _T("C/C++�w�b�_�t�@�C��(*.h)\0*.h\0���ׂ�(*.*)\0*.*\0\0");
	ofn.lpstrFile = filepath;
	ofn.nMaxFile =256;
	ofn.lpstrFileTitle = filename;
	ofn.nMaxFileTitle = 64;
	ofn.Flags = 0;
	ofn.lpstrTitle = _T("�ۑ�");

	if(!GetOpenFileName(&ofn))return(FALSE);

	//�g���q�𖳗����.h��
	int len=(int)strlen(filepath);
	if(len>4){
		if(!( filepath[len-4]=='.' &&
			(filepath[len-3]=='h' || filepath[len-3]=='H') ))
		{
			filepath[len] = '.';
			filepath[len+1] = 'h';
		}
	}

	//�ۑ�
	HANDLE hFile = CreateFile(filepath,
		GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		MessageBox(ghwnd,_T("̧�ق��J���Ȃ�����"),_T("SaveFile()"),MB_OK);
		return(FALSE);
	}

	char write[256];
	DWORD br;
	DWORD ret;
	for(DWORD i=0;i<GCDMAX_CELLS;i++){
		if(strlen(pfhnames->name[i]) != 0){
			sprintf(write,_T("#define CELL_%s %d\n"),pfhnames->name[i],i);
			ret=WriteFile(hFile,write,(DWORD)sizeof(char)*strlen(write),&br,NULL);
		}
	}

	CloseHandle(hFile);
	return(TRUE);
}

/*-----------------------------------------------------------------------
	�w��t�@�C���֎w��o�b�t�@�̃f�[�^�����k���ĕۑ�
	��0������I��
-------------------------------------------------------------------------*/
int CGCDHandler::GCDSaveCompressed(char *filename,GCD_CELL2 *pcdat,GCD_RECT *prdat,GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames)
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
	SetFilePointer(hFile,0,NULL,FILE_BEGIN);//�t�@�C���擪�Ɉړ�

	// �t�@�C���w�b�_��������
	memcpy(fhed.signature, cmp_sig, 4);
	fhed.ver = GCD_VERSION;
	fhed.flags = 0;
	if (pfhnames) // �Z��������
		fhed.flags |= GCDFILE_COMP_HAVE_CELLNAMES;

	DWORD br;
	DWORD ret=WriteFile(hFile,&fhed,sizeof(GCD_COMP_FILEHEADER),&br,NULL);
	if(!ret || br!=sizeof(GCD_COMP_FILEHEADER)){
		CloseHandle(hFile);
		return(2);
	}

	// �Z����
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

		// CRC�l�v�Z
		DWORD crc32_names = crc32(0L, Z_NULL, 0);

		for (int i = 0; i < GCDMAX_CELLS; i++)
			crc32_names = crc32(crc32_names, (BYTE*)pfhnames->name[i], strlen(pfhnames->name[i]) + 1);

		// �f�[�^�w�b�_�p��
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

	// ��`
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

		// �f�[�^�w�b�_�p��
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

	// �b�d�k�k
	{
		int num_cells = GCDMAX_CELLS;
		GCD_CELL cell_void[GCD_CELLGROUPNUM];

		// ���ɂ������ĂȂ��Z��
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

		// �f�[�^�w�b�_�p��
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

	// �����蔻��
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

		// �f�[�^�w�b�_�p��
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
	�����f�[�^��K�p
	�S��0�������l�ł͂Ȃ��̂ł�₱�����B�g�嗦�̂�1�������l�B
-------------------------------------------------------------------------*/
void CGCDHandler::GCDSetInitialData(GCD_CELL2 *pcdat,GCD_RECT *prdat,GCD_HANTEI *phdat,GCD_CELLNAMES *pfhnames)
{
	//��U�S���N���A
	if(pfhnames!=NULL){
		ZeroMemory(pfhnames,sizeof(GCD_CELLNAMES));
	}
	ZeroMemory(prdat,sizeof(GCD_RECT)*GCDMAX_RECTANGLES);
	ZeroMemory(pcdat,sizeof(GCD_CELL2)*GCDMAX_CELLS);
	ZeroMemory(phdat,sizeof(GCD_HANTEI)*GCDMAX_CELLS);

	//�g�嗦�E�F���Z�b�g
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
	�w��Z���E����@������ł��邩�ǂ����𒲂ׂ�
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
	��`���u��v���ǂ����𒲂ׂ�
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
	�Z���E����@�f�[�^�N���A
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
	��`�f�[�^�N���A
-------------------------------------------------------------------------*/
void CGCDHandler::GCDSetEmptyRect(GCD_RECT *pgcdr)
{
	ZeroMemory(pgcdr,sizeof(GCD_RECT));
}





/*-----------------------------------------------------------------------
	�Z���f�[�^�ϊ� 0.70��0.90
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
	�Z���f�[�^�ϊ� 0.90��0.70
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
	��������͑S�� static �Ȃ̂ŃC���X�^���X���Ȃ��Ă��g����
	�ȉ��A�f�[�^�Ǘ���
************************************************************************************/

#ifndef DELETE_ARRAY
#define DELETE_ARRAY(ptr)	if(ptr){delete [] ptr; ptr=NULL;}
#define SAFEDELETE(ptr)	if(ptr){delete ptr; ptr=NULL;}
#endif

/*-----------------------------------------------------------------------
	�\�z
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
	�ǂݍ���
	0������I��
-------------------------------------------------------------------------*/
int CGCDHandler::Load(char *filename,BOOL hload,BOOL nload)
{
	Destroy();

	m_cdat  = new GCD_CELL2[GCDMAX_CELLS];
	m_rdat  = new GCD_RECT[GCDMAX_RECTANGLES];
	m_hdat  = new GCD_HANTEI[GCDMAX_CELLS];
	m_names = new GCD_CELLNAMES;

	int ret = GCDLoadCompressed(filename,m_cdat,m_rdat,m_hdat,m_names);
	if(ret!=0)	//���s���
	{
		ret = GCDLoad(filename,m_cdat,m_rdat,m_hdat,m_names);
		if(ret!=0)Destroy();	//�@�@�܁@�@���@�@���@�@�s�@�@��
	}

	return ret;
}


/*-----------------------------------------------------------------------
	�f�[�^�j��
-------------------------------------------------------------------------*/
void CGCDHandler::Destroy()
{
	DELETE_ARRAY(m_cdat);
	DELETE_ARRAY(m_rdat);
	DELETE_ARRAY(m_hdat);
	SAFEDELETE(m_names);
}


/*-----------------------------------------------------------------------
	�f�[�^�����k����
	����Ƃ����Ă���̕��ɂ����̃f�[�^������ăo�b�t�@��Z�����邾��
-------------------------------------------------------------------------*/
void CGCDHandler::Diet()
{
	//�܂��[
}



/*-----------------------------------------------------------------------
	�f�[�^�ۑ�
-------------------------------------------------------------------------*/
#ifdef GCD_EDITER
BOOL CGCDHandler::Save()
{
	if(!valid()){
		MessageBox(ghwnd,_T("�f�[�^���ǂݍ��܂�ĂȂ���"),_T(""),MB_OK);
		return FALSE;
	}

	return GCDSaveDlg(m_cdat,m_rdat,m_hdat,m_names);
}
#endif//GCD_EDITER


