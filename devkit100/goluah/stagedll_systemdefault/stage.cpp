

#include "stage.h"
#include <math.h>


SET_STAGEINFO(CStage)


/*!
*	@brief コンストラクタ
*/
CStage::CStage(SDI_STAGEINFO2 *info)
			: CStageBase(info)
{
	bitmap = NULL;
}

/*!
*	@brief デストラクタ
*	読み込んだimage1.bmpを破棄する
*/
CStage::~CStage()
{
	UnloadBitMap(bitmap);
}

/*!
*	@brief 初期化
*	image1.bmpを読み込む。
*/
void CStage::InitializeStage()
{
	//image1.bmpをロード
	char filename[256];
	sprintf(filename,"%s\\back1",GetDLLPath());
	bitmap = LoadImage(filename,NULL);
}

/*!
*	@brief 描画
*	image1.bmpをステージ全体に描画する
*/
DWORD CStage::DrawBack()
{
	if(bitmap!=NULL){

		//転送元・ビットマップ全体
		RECT src;
		src.left = src.top = 0;
		src.right = (long)bitmap->wg;
		src.bottom= (long)bitmap->hg;

		//転送先
		MYRECT3D dst;
		if(bitmap->wg!=640)
			dst.z = 640.0f/(bitmap->wg-640)-1.0f;
		dst.z = 0.03f;
		dst.left  =-(dst.z+2.0f)*(640.0f/480.0f);
		dst.right = (dst.z+2.0f)*(640.0f/480.0f);
		dst.top   = (-1.0f - (float)tan(40.0/180.0*3.1415926))*(1.0f+dst.z);
		dst.bottom= ( 1.0f - (float)tan(40.0/180.0*3.1415926))*(1.0f+dst.z);

		//描画
		Blt3D(bitmap,src,dst,pdat->color);
	}

	return FALSE;//デフォルト描画は行わない
}
