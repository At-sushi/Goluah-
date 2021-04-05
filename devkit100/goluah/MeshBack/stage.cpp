

#include "stage.h"
#include <math.h>


SET_STAGEINFO(CStage)
#define APPNAME		"MeshBack"


/*!
*	@brief コンストラクタ
*/
CStage::CStage(SDI_STAGEINFO2 *info)
			: CStageBase(info)
{
	//変数値の初期化等
	aki3d.Initialize(GetD3DDevice(), info->dir);
	back = NULL;
}

/*!
*	@brief デストラクタ
*/
CStage::~CStage()
{
	//動的確保オブジェクトの破棄処理等
}

/*!
*	@brief 初期化
*/
void CStage::InitializeStage()
{
	//イメージのロード等、初期化処理全般
	// メッス作る
	back = aki3d.CreateMeshObject();

	// back.xをロード
	char buf[256], buf2[16];

	sprintf(buf, "%s\\back.x", this->GetCharDir());
	back->Create(buf);

	// 設定読み込み
	union {
		int amb;
		BOOL spec;
	};
	double ldir_x = 0.0, ldir_y = 0.0, ldir_z = 0.0;

	sprintf(buf, "%s\\stage.ini", this->GetCharDir());

	amb = GetPrivateProfileInt(APPNAME, "Ambient", 60, buf) & 0xFF;
	back->SetAmbient(amb | (amb<<8) | (amb<<16));

	GetPrivateProfileString(APPNAME, "LightDirect", "-5.0,8.0,10.0", buf2, 16, buf);
	sscanf(buf2, "%lf,%lf,%lf", &ldir_x, &ldir_y, &ldir_z);
	back->SetLightDir(V3d(ldir_x, ldir_y, ldir_z));

	spec = GetPrivateProfileInt(APPNAME, "SpecularEnable", 1, buf);
	back->SetSpecular(spec);
}

/*!
*	@brief 描画
*/
DWORD CStage::DrawBack()
{
	//描画処理
	// メッシュ描画
	back->Render();

	return FALSE;//通常、デフォルト描画は行わない
}
