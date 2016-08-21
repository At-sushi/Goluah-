
/*===================================================================

	体力ゲージ
	キャラクタ顔・勝利カウント・FPS
	パワーゲージ

	・・・等、戦闘時のシステム情報描画関係

=====================================================================*/
#pragma once

#include "define_gcd.h"
#include "gobject.h"
class CBattleTaskBase;

/*!
*	@brief 試合中の画面上の表示系全般
*	@ingroup Battle
*	
*	体力ゲージ、パワーゲージ + 画面上の表示系全般。（顔、勝利カウント、ＦＰＳ）を行う。
*/
class CGauge
{
public:
	CGauge(DI_FUNCTIONS_S* pfs, DI_FUNCTIONS_O* pfo, DI_FUNCTIONS_D* pfd);
	~CGauge(){CleanUp();}

	void CleanUp();
	void Initialize();
	void Initialize(DWORD num);
	void ActivateGauge(DWORD t,DWORD n);

	//戦闘オブジェクトとしての動作
	static DWORD GaugeObjectMessage(DWORD msg,LPVOID pdat,DWORD prm);
	void Action();
	void Draw();

protected:
	void DrawHPGauge1();		//!< ごちゃ混ぜの場合
	void DrawHPGauge2();		//!< それ以外
	void DrawPowerGauge1();		//!< ごちゃ混ぜの場合
	void DrawPowerGauge2();		//!< それ以外
	void DrawPowerGaugeType1(BOOL player,int pindex,int x,int y);			//!< パワーゲージ描画(ストック)
	void DrawPowerGaugeType2(BOOL player,int pindex,int x,int y,BOOL pow);	//!< パワーゲージ描画(0-100%)

//*変数*
protected:
	CBattleTaskBase* battleTask;

	DWORD oid;					//!< ゲージの、戦闘オブジェクトとしてのオブジェクトID
	DWORD num_teammember;		//!< 1チーム何人か
	DWORD activegauge[2];		//!< どのプレイヤーが現在対戦しているのか？0,1,2
	DWORD flip_counter;
	MYSURFACE *dds_face[2][MAXNUM_TEAM];	//!< ちっこい顔
	BYTE dll_id;				//!< DLLID

	//体力ゲージ
	int gauge_prv[2][MAXNUM_TEAM];		//!< 食らった分がじわじわ減っていくのに使用
	int gauge_prv2[2][MAXNUM_TEAM];		//!< 急に減らす用

	//パワーゲージ
	double pgauge_prv[2][MAXNUM_TEAM];	//!< いっこ前に表示してたゲージ量
	DWORD numprv[2][MAXNUM_TEAM];			//!< いっこ前のゲージストック数。charge音に使用
	BOOL pg_draw[2][MAXNUM_TEAM];			//!< パワーゲージを描画するかどうか
	int pg_dx[2][MAXNUM_TEAM];			//!< パワーゲージのずらし幅。ゲージの交換時に使用

	// ファンクション
	DI_FUNCTIONS_S* funcs;
	DI_FUNCTIONS_O* funco;
	DI_FUNCTIONS_D* funcd;
};


