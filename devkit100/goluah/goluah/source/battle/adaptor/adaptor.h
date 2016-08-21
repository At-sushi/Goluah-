
#define YEAH TRUE

/*!
*	@brief キャラアダプタークラス
*	@ingroup Battle
*
*	アダプターから移植してきただけのクラス
*	DLLの中身をほとんどそのまま引用してるので、もう少し本体に統合できる部分があるかも。
*	以前の名残でグローバル変数に依存している部分がいくつかあるので注意
*
*	まだちょっとバグがあるみたい(キャラが途中で硬直する)。
*/
class CCharAdaptor
{
public:
	CCharAdaptor();
	~CCharAdaptor();

	DWORD CreateCharacter(PVOID info);
	void InitAdaptor(CDI_CHARACTERINFO2 *info);		//!<以前のコンストラクタ

	DWORD Message(DWORD msg,LPVOID dat,DWORD prm1);
	DWORD Action();
	void Command();
	DWORD CommandCOM(DWORD wid);
	DWORD TouchA(ATTACKINFO *info);
	DWORD TouchB(ATTACKINFO *info,BOOL hit);
	void InitAttackInfo();
	void ActionIDChanged();

	BOOL com236(int dt);
	BOOL com623(int dt);
	BOOL com214(int dt);
	BOOL com66(int dt);
	BOOL com44(int dt);
	BOOL NageHantei(DWORD maai);//投げの間合い判定
	BOOL com236236(int dt);

	BOOL ChainCombo(DWORD chainid);

	void act_neutral();
	void act_crouch();
	void act_rakka();
	void act_tojyo();
	void act_win();

	void act_walkf();
	void act_walkb();
	void act_jamps();
	void act_jampc();
	void act_jampf();
	void act_jampb();
	void act_rakka2();

	void act_att_sa();
	void act_att_sb();
	void act_att_sc();
	void act_att_smc();
	void act_att_ca();
	void act_att_cb();
	void act_att_cc();
	void act_att_ja();
	void act_att_jb();
	void act_att_jc();

	void act_dash();
	void act_dashb();

	void act_taiki();
	void act_koutai();
	void act_koutai2();
	void act_koutai_out();
	void act_koutai_in();
	void act_strikercomeon();

	void movex(double x);
	int zurex(int x);
public:
	DWORD BulletHit(MYATTACKINFO* info, BOOL hit);
	double pg_hit;
	double pg_guard;
	double g_blown;
	double g_guard;
	PF_ACTNAGERARE pfNagerare;
	CHARACTERINFO2 olddat;
	GOBJECT *pdat;
	DWORD oid;
	DWORD keynumber;

	DWORD chainlist;

	// 都合によりグローバル領域から移ってこられた方々
	HMODULE hAction2;
	GOBJECT *m_pchar[2];	// グローバル変数の方に送るための、写し。
	DWORD m_pnagerare[2];
	TCHAR *m_chardir;		// 要対応
	LPVOID m_mysounds[MAXMYSOUND];		// 要対応
	MYSURFACE* m_mysurface[2][12];
	GCD_CELL2 m_cdat[2][GCDMAX_CELLS];
	GCD_RECT m_rdat[2][GCDMAX_RECTANGLES];
	GCD_HANTEI m_hdat[2][GCDMAX_CELLS];

	void InitializeMySound();
	void PlayMySound(DWORD soundno);
	void UnloadMySound();
	void InitializeGCDandBMP(GOBJECT *pdat,DWORD color);
	void UnloadMyBitmaps(GOBJECT *pdat);
};
extern DWORD extwin;
/*
#define ACTID_WALKF		0x0005//前歩き
#define ACTID_WALKB		0x0006//後ろ歩き
#define ACTID_JAMPS		0x0007//ジャンプ予備動作
#define ACTID_DASH		0x0008
#define ACTID_DASHB		0x0009
#define ACTID_JAMPC		(ACTID_KUCYU | 0x0002)
#define ACTID_JAMPF		(ACTID_KUCYU | 0x0003)
#define ACTID_JAMPB		(ACTID_KUCYU | 0x0004)
#define ACTID_RAKKA2	(ACTID_KUCYU | 0x0005)
*/
#define ACTID_STRIKERCOMEON	(0x000B)//いけー
/*
#define ACTID_ATT_SA	(ACTID_ATTACK | 0x0001)
#define ACTID_ATT_SB	(ACTID_ATTACK | 0x0002)
#define ACTID_ATT_SC	(ACTID_ATTACK | 0x0003)

#define ACTID_ATT_CA	(ACTID_ATTACK | ACTID_SYAGAMI | 0x0001)
#define ACTID_ATT_CB	(ACTID_ATTACK | ACTID_SYAGAMI | 0x0002)
#define ACTID_ATT_CC	(ACTID_ATTACK | ACTID_SYAGAMI | 0x0003)

#define ACTID_ATT_JA	(ACTID_ATTACK | ACTID_KUCYU | 0x0001)
#define ACTID_ATT_JB	(ACTID_ATTACK | ACTID_KUCYU | 0x0002)
#define ACTID_ATT_JC	(ACTID_ATTACK | ACTID_KUCYU | 0x0003)
*/
#define ACTID_KOUTAIOUT	(ACTID_INOUT | 0x0001)//交代。画面外に飛んでく。
#define ACTID_KOUTAIIN	(ACTID_INOUT | 0x0002)//交代。画面内に飛んでくる
#define ACTID_TAIKI		(ACTID_INOUT | 0x0003)//画面外に飛んでって待機
#define ACTID_KOUTAI	(ACTID_INOUT | 0x0004)//画面外から飛んでくる
#define ACTID_TAIKICYU	(ACTID_INOUT | 0x0009)//待機ちゅ—
#define ACTID_KOUTAI2	(ACTID_INOUT | 0x000A)//交代後、ポーズキメ

#define MYGOBJMSG_NAGE	(GOBJMSG_USERDEFINE | 1)
//chain combo
#define CHAIN_SA	0x00000001
#define CHAIN_SB	0x00000002
#define CHAIN_SC	0x00000004
#define CHAIN_CA	0x00000010
#define CHAIN_CB	0x00000020
#define CHAIN_CC	0x00000040


#define ADDEFFECT(a,b,c,d)	funcs->addeffect(a,b,c,d)