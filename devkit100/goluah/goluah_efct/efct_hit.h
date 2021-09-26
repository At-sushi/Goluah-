/*===============================================================

        ヒットマーク等

=================================================================*/
#pragma once
#include "efctlist.h"

/*!
 *	@brief エフェクト描画オブジェクト・ヒットマーク(弱)
 *	@ingroup Battle
 */
class CFxHitmark1 : public CSystemFxBase {
public:
  void Update();
  BOOL Draw();
};

/*!
 *	@brief エフェクト描画オブジェクト・ヒットマーク(中)
 *	@ingroup Battle
 */
class CFxHitmark2 : public CSystemFxBase {
public:
  void Update();
};

/*!
 *	@brief エフェクト描画オブジェクト・ヒットマーク(強)
 *	@ingroup Battle
 */
class CFxHitmark3 : public CSystemFxBase {
public:
  void Update();
};

/*!
 *	@brief エフェクト描画オブジェクト・ヒットマーク(激)
 *	@ingroup Battle
 */
class CFxHitmark4 : public CSystemFxBase {
public:
  void Update();
  BOOL Draw();
};

/*!
 *	@brief エフェクト描画オブジェクト・ガードマーク(削りダメージなし)
 *	@ingroup Battle
 */
class CFxGuardmark1 : public CSystemFxBase {
public:
  virtual ~CFxGuardmark1() {}

  void Update();
  virtual BOOL Draw();
};

/*!
 *	@brief エフェクト描画オブジェクト・ガードマーク(削りダメージあり)
 *	@ingroup Battle
 */
class CFxGuardmark2 : public CFxGuardmark1 {
public:
  BOOL Draw();
};

/*!
 *	@brief エフェクト描画オブジェクト・掴みマーク
 *	@ingroup Battle
 */
class CFxCatchmark : public CSystemFxBase {
public:
  void Update();
};

/*!
 *	@brief エフェクト描画オブジェクト・地面衝撃波
 *	@ingroup Battle
 */
class CFxGndCircle : public CSystemFxBase {
public:
  void Update();
};
