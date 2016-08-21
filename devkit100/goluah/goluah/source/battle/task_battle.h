
/*============================================================================

	戦闘タスククラス

	（ネットワーク非対応版）

	Goluah!!Copyright(C) 2001-2004 aki, 2014-2015 logger, 2004-2015 At-sushi

	This program is free software; you can redistribute it and / or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or(at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110 - 1301 USA.

==============================================================================*/
#pragma once

#include "define_const.h"
#include "battleTaskBase.h"
#include "gauge.h"
#include "efctlist.h"
#include "task_pause.h"

#define MAXEFCT_SIVER 16
#define DEBUGMSGBUFFERSIZE	(1024*4)


/*!
*	@brief 通常版戦闘タスク
*	@ingroup Battle
*
*	オブジェクトの管理：
*	オブジェクトのIDは、配列のインデックス + そのインデックスでの生成番号 + エフェクトオブジェクトフラグ　
*	により生成される。IDからインデックスが分かるので、IDからのオブジェクトの取得が
*	比較的高速に処理できる・・・と思う。　また、生成番号を附加することにより
*	相当試合が長引かない限り同じIDをもつオブジェクトが生成されることはない。
*
*	エフェクトオブジェクトフラグが立っている場合はあたり判定などを行わない
*/
class CBattleTask : public CBattleTaskBase
{
public:
	CBattleTask();
	virtual ~CBattleTask();

	//タスク関数
	virtual void Initialize();
	virtual BOOL Execute(DWORD time);
	virtual void Draw();
	virtual void WndMessage(HWND hWnd,UINT msg,WPARAM wparam, LPARAM lparam);//!< F7でポーズ

	//情報取得系
	virtual double GetDisplayCenterX(){return disp_center_x;}
	virtual double GetDisplayCenterY(){return disp_center_y;}
	virtual BOOL   IsNetwork(){return FALSE;}
	virtual BOOL   IsLocal(BYTE dllid){return TRUE;}

	//void SetKatiSerif(DWORD tid,TCHAR *serif);
	virtual DWORD MessageFromObject(DWORD oid,DWORD msg,DWORD prm);
	virtual BOOL CatchObject(DWORD eoid,LPVOID cy);
	virtual void AddEffect(DWORD efctid,int prm1,int prm2,int prm3=0);
	virtual void Atari(DWORD a_id, DWORD k_id, MY2DVECTOR &kas_point);
	virtual void HitStop(DWORD len,DWORD oid);

	virtual void SetTransform(BOOL b);


protected:
	//Initializeからコール
	virtual void InitializeObjectList();
	virtual void InitializeSubTasks();
	virtual void InitializeParameters();

	//Terminateからコール
	virtual void TerminateObjectList();
	virtual void TerminateDestroySubTasks();

	//Execute部分処理
	virtual void T_Command();			//!< COMMAND メッセージを送信する
	virtual void T_Action(BOOL stop);	//!< Action メッセージを送信する
	virtual void T_Sousai();			//!< 飛道具相殺用 矩形あたり判定処理
	virtual void T_AtariHantei();		//!< 攻撃用　　　 矩形あたり判定処理
	virtual void T_KasanariHantei();	//!< 重なり判定用 矩形あたり判定処理
//	virtual void T_Draw();				//!< DRAW メッセージを送信する
	virtual void T_ChangeTarget();		//!< オブジェクトターゲット変更処理
	virtual void T_UpdateStatus();		//!< 試合の状態更新

	virtual void StartRound();
	virtual void UpdateKeyInputDirections();

	//デバッグ用 状態表示
	virtual void DrawState();
	virtual void DrawObjectList();
	virtual void DrawCharacterState();
	virtual void DrawCharacterState2();

protected:
	//全体エフェクト関連
	int efct_slowdown,efct_stop,efct_darkbg,efct_nobg;
	int efct_sindo,efct_sindom;
	int efct_hitstop;
	int efct_fadein;
	int efct_flash;

	//状態
	DWORD hprecratio[2][MAXNUM_TEAM];	//!<自由交代制のときの体力回復率
	BOOL  battle_end;		//!<タスクをヌけるのに使用
//	DWORD striker_lastcall[2];	//ストライカーを最後に呼んだのはいつか

	//!デバッグテキスト表示用バッファ
	TCHAR *debugmsgbuff;

	//!表示中心
	double disp_center_x,disp_center_y;

	//クラス
	CGauge		*cp_gauge;
	CEffectList	*cp_efctlist;
	DWORD score[2];			//!< スコレ

	//関数を分割したのでローカルじゃいけなくなったもの
	BOOL act_stop;

	//!ポーズ時の描画・操作
	CTBattlePause* m_pause_task;

	//!支援攻撃要請用カウンタ
	UINT strikercall_counter[2];

	LPDIRECT3DTEXTURE9 tex_fb;

	//! リプレイ保存ファイルデータ
	CFile RepFile;

	/*■試合進行管理■ ------------------------------------------------------*/
protected:
	//T_UpdateStatusをさらに分割
	virtual void T_UpdateStatus_WaitForEndPose();	//!< 試合進行管理・登場ポーズ終了待ち
	virtual void T_UpdateStatus_RoundCall();		//!< 試合進行管理・ラウンドコール終了待ち
	virtual void T_UpdateStatus_Fighting();			//!< 試合進行管理・戦闘状態
	virtual void T_UpdateStatus_Finished();			//!< 試合進行管理・KO、キャラクタの最終ダウン終了待ち
	virtual void T_UpdateStatus_WaitForEndWin();	//!< 試合進行管理・勝利ポーズ終了待ち
	virtual void T_UpdateStatus_DoubleKO();			//!< 試合進行管理・ダブルKO表示終了待ち
	virtual void T_UpdateStatus_TimeOver();			//!< 試合進行管理・タイムオーバー表示終了待ち

	virtual void Update_DeadFlag();					//!< 死亡フラグ更新

	//試合進行管理フラグ
	DWORD winner_oid;								//!< 勝利ポーズ終了待ち対象
	BOOL m_winpose_end;								//!< 勝利ポーズ終了・↑一人分しか見てない
	BOOL m_finaldown_end[2];						//!< ダウン完了フラグ
	BOOL m_tojyo_end[2][MAXNUM_TEAM];				//!< 登場ポーズ終了フラグ。KOFLike時のウェイトにも使用
	BOOL m_all_dead[2];								//!< 死亡フラグ
	BOOL m_dead_one[2][MAXNUM_TEAM];				//!< 死亡フラグ
	BOOL m_active_dead[2];							//!< 死亡フラグ
	BYTE m_round_winner;							//!<  0/1:勝利チーム , 2:ダブルKO


	/*■オブジェクト管理■-----------------------------------------------------*/
public:
	CGObject* GetGObject(DWORD id);					//!< 指定IDオブジェクト取得
	DWORD CreateGObject();							//!< オブジェクト生成
	DWORD CreateGObjectFx();						//!< オブジェクト生成(エフェクト)
	void DeleteGObject(DWORD oid);					//!< オブジェクト破棄
	void SuicideGObject(DWORD oid);					//!< オブジェクト破棄予約
	CGObject* GetCharacterObject(DWORD j,DWORD i);	//!< キャラクターのオブジェクト取得（ゲージ等で必要）
protected:
	std::unordered_map< int, CGObject* > p_objects;	//!< オブジェクト操作クラスのポインタ
	DWORD object_regindex;							//!< 次に生成するオブジェクトのインデックス
	std::unordered_map< int, WORD > object_regno;	//!< そのインデックスでいくつのオブジェクトが生成されてきたか
	std::deque< DWORD > suicide_list;				//!< 消滅オブジェクトリスト
};

#define BATTLETASK_FXOBJFLAG		0x80000000
#define BATTLETASK_ISNOTFXOBJ(p)	((p->id & BATTLETASK_FXOBJFLAG) ? FALSE : TRUE)

