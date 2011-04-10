/*!
*	@file
*	@brief ストーリー中間デモ タスク(ver0.5)
*
*	旧来のストーリー中間デモスクリプトの再生
*/
#pragma once

/*!
*	@ingroup Tasks
*/
/*@{*/

//! ストーリー中間デモ・１ブロック分情報
struct DEMODAT
{
	char bitmap[4][64];	//!< 表示ビットマップ（デフォルト：なし）
	BOOL bgmmid;		//!< 未使用
	char bgm[64];		//!< 再生BGM（デフォルト：変更無し）
	char wavsnd[64];	//!< 再生サウンド（デフォルト：なし）//未実装
	char msg[1024];		//!< 表示メッセージ（デフォルト：なし）
	int spdmsg;			//!< メッセージ表示スピード（デフォルト：適当）
	int dur;			//!< 表示時間（デフォルト：文字列表示に合わせて自動的に設定）
	DWORD bmpdisp;		//!< ↓フラグ
	double bgmpos;		//!< BGM開始位置
	int num_select;		//!< 選択肢の数（0=選択肢なし）
	int select_base;	//!< 選択変数をシフトする量
};

#define DEMOBMPDISP_MOVEL	0x00000001
#define DEMOBMPDISP_MOVER	0x00000002
#define DEMOBMPDISP_MOVEU	0x00000004
#define DEMOBMPDISP_MOVED	0x00000008
#define DEMOBMPDISP_YUREX	0x00000010
#define DEMOBMPDISP_YUREY	0x00000020
#define DEMOBMPDISP_FIXR	0x00000040
#define DEMOBMPDISP_FIXB	0x00000080

//! ストーリー中間デモ
class CTDemo : public CExclusiveTaskBase
{
public:
	CTDemo();
	~CTDemo();
	void Initialize();
	void Terminate();
	BOOL Execute(DWORD time);
	void Draw();
	int  GetDrawPriority(){return 1000;}
	DWORD GetID(){return 'Demo';}

	void Setup(UINT si,char *fn);
	void SetKeyIndex(UINT ki){m_keyindex=ki;}

private:
	BOOL Execute2();
	void CleanUp();
	int InitDemoDat(char *filepath);
	DWORD GetGyoDemo(char *strin,char *strout,DWORD *susumu);

//変数
public:
private:
	DEMODAT *demodat;
	char errc[256];
	DWORD counter;
	DWORD counter2;	//テキスト長さインクリメント用
	char fpath[256];
	MYSURFACE *ms;
	MYSURFACE* chr[3];
	DWORD num_demoscenes;
	BOOL demoready;

	UINT  m_keyindex;
	UINT  m_current_story_index;
	char* m_script_filename;
	char* m_current_dir;

	//もともとがstatic
	int bmpw,bmph;
	DWORD playingdemodat;

	//もともとauto
	DWORD txtlen;
	BOOL  multibyte_flag;//テキストが2バイト文字だった場合、次にウェイトインターバルが到達しても文字を追加しない
	int yurex,yurey;
	RECT r_all;
	RECT r_serif;
	int select;
	int select_base;	// 最終的なやつ
};

/*@{*/
