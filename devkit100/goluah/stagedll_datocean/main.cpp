
#include "stage.h"


#define EXPORT extern "C" __declspec(dllexport) 

#include "stage.h"

// StageInfo ================================================
//�N�����ɌĂяo�����B
EXPORT BOOL StageInfo(LPVOID info)
{
	SDI_STAGEINFO *pif = (SDI_STAGEINFO*)info;
	pif->ver = SDI_VERSION;
	return(TRUE);
}

// CreateStage ================================================
//���[�h���ɌĂяo�����B
EXPORT DWORD CreateStage(PVOID info)
{
	CStage *pstg = new CStage((SDI_STAGEINFO2*)info);
	return(pstg->oid);
}
