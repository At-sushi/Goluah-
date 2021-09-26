/*===============================================================

        その他エフェクト

=================================================================*/
#pragma once
#include "efctlist.h"
#include "aki3d.h"

/*!
 *	@brief エフェクト描画オブジェクト・超必の光
 *	@ingroup Battle
 */
class CFxCyohiLight : public CSystemFxBase {
#define CYOHILIGHT_LINES 30
#define CYOHILIGHT_LIFETIME 30

public:
  CFxCyohiLight();
  void Update();
  BOOL Draw();

protected:
  void InitCyohiLightVertex();

  static BOOL static_initialized;
  static MYVERTEX3D p_vbcl[CYOHILIGHT_LIFETIME][CYOHILIGHT_LINES * 3];
};

/*!
 *	@brief エフェクト描画オブジェクト・単純な ○ の広がり
 *	@ingroup Battle
 */
class CFxCircle : public CSystemFxBase {
public:
  void Update();
  BOOL Draw();
};

#define BIGTUBUTUBUSIZE 16
/*!
 *	@brief エフェクト描画オブジェクト・粒々（大）
 *	@ingroup Battle
 */
class CFxPSprite : public CSystemFxBase {
public:
  CFxPSprite();
  virtual ~CFxPSprite();

  BOOL Create(int prm1, int prm2, int prm3);
  void Update();
  BOOL Draw();

protected:
  BOOL SetParticleDat(int x, int y, DWORD prm);
  virtual void PointSetting();

  float center_x, center_y;
  DWORD num; //粒子数
  MYVERTEX3D vb[16];
  float vx[16], vy[16];
  DWORD life[16];

  CParticle *m_part;
};

/*!
 *	@brief エフェクト描画オブジェクト・粒々（小）
 *	@ingroup Battle
 */
class CFxPSprite2 : public CFxPSprite {
protected:
  void PointSetting();
};

/*!
 *	@brief エフェクト描画オブジェクト・もえー
 *	@ingroup Battle
 */
class CFxBurn : public CSystemFxBase {
#define EFCTBURN_MAXLIFE 30
#define ACTID_EFCTBURN_ATOUCHED 0
#define ACTID_EFCTBURN_DETOUCHED 1
#define NUM_FIRE (32)
#define FIRE_PERTICLESIZE 32

public:
  virtual ~CFxBurn() {}

  BOOL Create(int prm1, int prm2, int prm3);
  void Update();
  BOOL Draw();

protected:
  DWORD tgtobjid;
  DWORD aidprv;
  MYVERTEX3D vb[NUM_FIRE][4];

  virtual DWORD GetColor() { return 0x00FF5522; }

  float cx[NUM_FIRE], cy[NUM_FIRE];
  float ay[NUM_FIRE];
  float vx[NUM_FIRE], vy[NUM_FIRE];
  int life[NUM_FIRE], lifemax[NUM_FIRE];
  float maxsize[NUM_FIRE];
};

/*!
 *	@brief エフェクト描画オブジェクト・青もえ
 *	@ingroup Battle
 */
class CFxBurnB : public CFxBurn {
  // public:
  //	CFxBurnB(){}
  //	virtual ~CFxBurnB(){}
protected:
  virtual DWORD GetColor() { return 0x002211FF; }
};

/*!
 *	@brief エフェクト描画オブジェクト・緑もえ
 *	@ingroup Battle
 */
class CFxBurnG : public CFxBurn {
  // public:
  //	CFxBurnG(){}
  //	virtual ~CFxBurnG(){}
protected:
  virtual DWORD GetColor() { return 0x0022FF11; }
};

/*!
 *	@brief エフェクト描画オブジェクト・"Come On"
 *	@ingroup Battle
 */
class CFxComeon : public CSystemFxBase {
public:
  void Update();
};

/*!
 *	@brief エフェクト描画オブジェクト・ガードキャンセル
 *	@ingroup Battle
 */
class CFxGuardCancel : public CSystemFxBase {
#define GCANCEL_PARTICLE_NUM (32)

public:
  CFxGuardCancel();
  virtual ~CFxGuardCancel();

  virtual BOOL Create(int prm1, int prm2, int prm3);
  virtual void Update();
  virtual BOOL Draw();

protected:
  virtual DWORD GetColor() { return 0x004444FF; }
  virtual BOOL DrawCircle() { return TRUE; }
  CParticle *m_part;

  struct CFxGuardCancel_PartInfo {
    V3d pos;
    V3d vel;
    V3d ax;
    float rspd;
    float wdt;

    void Reset();
    void Update();
  };
  CFxGuardCancel_PartInfo m_info[GCANCEL_PARTICLE_NUM];
};

/*!
 *	@brief エフェクト描画オブジェクト・スーパーアーマー
 *	@ingroup Battle
 */
class CFxSuperArmer : public CFxGuardCancel {
public:
  virtual DWORD GetColor() { return 0x00FF4400; }
  virtual BOOL DrawCircle() { return FALSE; }
  virtual void Update();
};

/*!
 *	@brief エフェクト描画オブジェクト・Hatten ar din
 *	@ingroup Battle
 */
class CFxHatten : public CSystemFxBase {
public:
  virtual void Update();
};
