
/*!
*	@file 
*	@brief DLLに対して提供する関数の定義
*/
#ifndef _EXPORT_H_
#define _EXPORT_H_

#include "gobject.h"		//関数ポインタセット構造体の定義が必要
#include "battleTaskBase.h"	//戦闘タスク

/*!
*	@ingroup battle
*	@brief DLLへの関数ポインタ提供クラス
*
*	DLLに対して提供する関数ポインタまとめクラス
*	ほとんどの関数がstatic。
*	ver0.99の途中からエラー検出用に「最後に呼び出された関数」を記憶する機能がつきました。
*/
class CExport{

public:
	CExport();
	void Initialize();			//!< 関数ポインタを構造体に詰め込む
	static CBattleTaskBase* GetCurrentBattleTask();//!< 現在の戦闘タスクを取得する
	static char* last_funcname;
	static BOOL  func_in;

	DI_FUNCTIONS_S fpack_s;
	DI_FUNCTIONS_O fpack_o;
	DI_FUNCTIONS_D fpack_d;

	//■ システム関連
	static DWORD  GetKey(DWORD tid,DWORD index);		//!< キー入力get
	static int    SeekKey(DWORD tid,int index,int numseek,DWORD key);//!< キー入力get(2)
	static void   SetKatiSerif(DWORD tid,char* serif);	//!< 勝ち台詞設定
	static void   AddEffect(DWORD eid,int prm1,int prm2,DWORD prm3);//!< エフェクト発生
	static void   PlaySystemSound(DWORD sid);			//!< wav再生
	static LPVOID LoadMySound(char* filename);			//!< wav読み込み
	static void   PlayMySound(LPVOID pdsb);				//!< wav再生
	static void   KillMySound(LPVOID pdsb);				//!< wav消去
	static DWORD  Message2System(DWORD oid,DWORD msg,DWORD prm);//!< メッセージ
	static char*  GetEnemyName(DWORD tid);				//!< 敵の名前取得
	static char*  GetCharacterName(DWORD oid);			//!< 指定キャラの名前取得
	static DWORD  GetKeyInput(DWORD oid);				//!< 指定キャラのキーID取得
	static double GetDisplayCenterX();					//!< ディスプレイ中心get
	static double GetDisplayCenterY();					//!< ディスプレイ中心get
	static DWORD  GetTeamNum();							//!< 何対何か
	static DWORD  GetTaisenKeisiki();					//!< 対戦形式を取得
	static DWORD  StrikerOK(DWORD tid,BOOL next);		//!< ストライカー問い合わせ
	static BYTE	  GetEnemyFace(DWORD tid);				//!< フェイス変更値(通常0)を取得
	static DWORD  GetTeamNum2(DWORD tid);				//!< チーム人数を取得します
	static DWORD  TeamID2EnemyTeamID(DWORD tid);		//!< 自チームIDを敵側のチームIDに変換します
	static BOOL	  IsNetwork();							//!< ネットワーク対戦かどうか
	static void   PushTag(const char* tag);				//!< タグ設定
	static void   PopTag();								//!< タグ除去
	static void   LogError(const char* str);			//!< ログ
	static void   LogWarning(const char* str);			//!< ログ
	static void   LogDebug(const char* str);			//!< ログ
	static void   LogInfo(const char* str);				//!< ログ
	static void   BGMPause(void);
	static void   BGMResume(void);
	static int    GetGameSpeed();						//!< fps取得
	static DWORD  GetMaxWin();							//!< 先取ポイント数取得

	//■　オブジェクト関連
	static DWORD CreateObject();						//!< オブジェクト生成
	static void  DeleteObject(DWORD oid);				//!< オブジェクト消滅
	static BOOL  CatchObject(DWORD eoid,LPVOID cy);		//!< 投げとかで相手をつかむ
	static void* GetObjectInfo(DWORD oid);				//!< 情報
	static void  AddDamage(DWORD oid,DWORD eoid,int x,int y);//!< (主に投げのとき)強制的にダメージを与える
	static void  ActIDChanged(DWORD oid);				//!< 行動ID変化時の処理をする
	static void* GetActiveCharacter(DWORD tid);			//!< アクティブキャラクター取得
	static void  SuicideGObject(DWORD oid);				//!< オブジェクト消滅2
	static DWORD GetMaaiH(DWORD oid,DWORD eoid);		//!< 間合いGet(水平)
	static DWORD GetMaaiV(DWORD oid,DWORD eoid);		//!< 間合いGet(垂直)
	static DWORD GetCharacterID(DWORD tid,DWORD index);	//!< キャラクターのIDをゲット
	static DWORD GetComLevel(DWORD oid);				//!< comレベル取得
	static void  SetComReach(DWORD oid,DWORD idx,int length);		//!< comリーチ指定
	static void  SetComAct(DWORD oid,DWORD aid,int delay);			//!< com行動遷移指定
	static DWORD CreateObjectFx();									//!< エフェクトオブジェクト生成
	static BOOL  ObjIsLocal(DWORD oid);								//!< ローカルかどうか
	static void  SetComRange(DWORD oid,DWORD idx);					//!< com基本レンジ指定
	static DWORD GetHitCount(DWORD oid);				//!< 連続技ヒット数（被コンボ数）取得
	static DWORD GetSexyDamage(DWORD oid);				//!< 連続技蓄積ダメージ（被コンボダメージ）取得

	//■　描画関連の関数の定義
	static LPVOID GetD3D();								//!< IDirect3D* の取得
	static LPVOID GetD3DD();							//!< IDirect3DDevice* の取得
	static void   LoadCellDat(char* filename,LPVOID cdat,LPVOID rdat,LPVOID hdat);	//!< セルデータ読み込み関数
	static LPVOID LoadBmp(char* filename,char* palname);//!< ビットマップ読み込み関数
	static void   UnloadBmp(LPVOID psuf);				//!< ビットマップ後始末関数
	static void   CellDraw(LPVOID* ddsa,LPVOID cdat,LPVOID rdat,DWORD cn,int x,int y,float z,int rot,BOOL revx,BOOL revy,DWORD color,float magx,float magy,BOOL shadowed = 1);//!< セル描画関数
	static void   Blt(void* dds,int x,int y,RECT r,double magx,double magy,BOOL revx,BOOL revy,float z,DWORD color);//!< Blt2
	static void   Blt3D(void* dds,RECT src,MYRECT3D dst,DWORD color);//!< Blt3
	static void   SetTransform(BOOL b);					//!< 座標変換行列を再設定
	static void   SetBlendMode(DWORD m);				//!< α合成の方法を変更
	static void   SetParentMatrix(LPVOID mat,BOOL root,LPVOID matprv);//!< 「親」変換行列設定
	static DWORD  CreateCellDat(char* filename,LPVOID *cdat,LPVOID *rdat,LPVOID *hdat);//!< セルデータ読み込み。ver0.90以降ではLoadCellDatからこちらに変更するべき
	static void   DestroyCellDat(LPVOID *cdat,LPVOID *rdat,LPVOID *hdat);//!< CreateCellDatで生成されたバッファをクリア
	static LPVOID LoadImage(char* filename, char* palname);//!< 画像読み込み関数、PNG対応に伴い追加。
	static DWORD  CreateCellDat2(char* filename,LPVOID *cdat,LPVOID *rdat,LPVOID *hdat);//!< セルデータ読み込み（２）、圧縮形式対応に伴い追加。
};



#endif//_EXPORT_H_