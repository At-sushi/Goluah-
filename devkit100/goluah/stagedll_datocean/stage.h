
/*--------------------------------------------------------------
	datの海
----------------------------------------------------------------*/
#pragma once

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <vector>

#include "define_gcd.h"
#include "gobject.h"

#include "aki3d.h"

class CStageObjectBase
{
public:
	virtual ~CStageObjectBase(){}
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Release() = 0;
};

class CStage
{
public:
	CStage(SDI_STAGEINFO2 *info);
	~CStage();
	DWORD Message(DWORD msg,LPVOID pdatg,DWORD prm);
	void Draw();
	void Action();

public:
	DWORD oid;//オブジェクトID
	GOBJECT *pdat;
	std::vector<CStageObjectBase*> objlist;
};


#ifndef RELEASE
#define RELEASE(a) if(a!=NULL){a->Release();a=NULL;}
#endif

#ifndef EXPORT
#define EXPORT extern "C" __declspec(dllexport)
#endif

