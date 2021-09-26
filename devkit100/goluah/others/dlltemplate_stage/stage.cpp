

#include "stage.h"
#include <math.h>

SET_STAGEINFO(CStage)

/*!
 *	@brief コンストラクタ
 */
CStage::CStage(SDI_STAGEINFO2 *info) : CStageBase(info) {
  //変数値の初期化等
}

/*!
 *	@brief デストラクタ
 */
CStage::~CStage() {
  //動的確保オブジェクトの破棄処理等
}

/*!
 *	@brief 初期化
 */
void CStage::InitializeStage() {
  //イメージのロード等、初期化処理全般
}

/*!
 *	@brief 描画
 */
DWORD CStage::DrawBack() {
  //描画処理

  return FALSE; //通常、デフォルト描画は行わない
}
