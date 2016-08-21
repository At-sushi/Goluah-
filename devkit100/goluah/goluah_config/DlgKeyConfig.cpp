// DlgKeyConfig.cpp : 実装ファイル
//

#include "stdafx.h"
#include "goluah_config.h"
#include "DlgKeyConfig.h"
#include "DlgKBKeyGet.h"

CKeyCode2Str keycode;

// CDlgKeyConfig ダイアログ

IMPLEMENT_DYNAMIC(CDlgKeyConfig, CDialog)
CDlgKeyConfig::CDlgKeyConfig(CWnd* pParent , KEYCONFIG& cfg)
	: CDialog(CDlgKeyConfig::IDD, pParent)
	, sstr_1_1(_T(""))
	, sstr_1_2(_T(""))
	, sstr_1_3(_T(""))
	, sstr_1_4(_T(""))
	, sstr_1_5(_T(""))
	, sstr_1_6(_T(""))
	, sstr_1_7(_T(""))
	, sstr_1_8(_T(""))
	, sstr_2_1(_T(""))
	, sstr_2_2(_T(""))
	, sstr_2_3(_T(""))
	, sstr_2_4(_T(""))
	, sstr_2_5(_T(""))
	, sstr_2_6(_T(""))
	, sstr_2_7(_T(""))
	, sstr_2_8(_T(""))
{
	m_cfg = cfg;
}

CDlgKeyConfig::~CDlgKeyConfig()
{
}

void CDlgKeyConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEXT_1_1, sstr_1_1);
	DDX_Text(pDX, IDC_TEXT_1_2, sstr_1_2);
	DDX_Text(pDX, IDC_TEXT_1_3, sstr_1_3);
	DDX_Text(pDX, IDC_TEXT_1_4, sstr_1_4);
	DDX_Text(pDX, IDC_TEXT_1_5, sstr_1_5);
	DDX_Text(pDX, IDC_TEXT_1_6, sstr_1_6);
	DDX_Text(pDX, IDC_TEXT_1_7, sstr_1_7);
	DDX_Text(pDX, IDC_TEXT_1_8, sstr_1_8);
	DDX_Text(pDX, IDC_TEXT_2_1, sstr_2_1);
	DDX_Text(pDX, IDC_TEXT_2_2, sstr_2_2);
	DDX_Text(pDX, IDC_TEXT_2_3, sstr_2_3);
	DDX_Text(pDX, IDC_TEXT_2_4, sstr_2_4);
	DDX_Text(pDX, IDC_TEXT_2_5, sstr_2_5);
	DDX_Text(pDX, IDC_TEXT_2_6, sstr_2_6);
	DDX_Text(pDX, IDC_TEXT_2_7, sstr_2_7);
	DDX_Text(pDX, IDC_TEXT_2_8, sstr_2_8);
}


BEGIN_MESSAGE_MAP(CDlgKeyConfig, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_1_1, &CDlgKeyConfig::OnBnClickedButtonSetkey11)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_1_2, &CDlgKeyConfig::OnBnClickedButtonSetkey12)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_1_3, &CDlgKeyConfig::OnBnClickedButtonSetkey13)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_1_4, &CDlgKeyConfig::OnBnClickedButtonSetkey14)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_1_5, &CDlgKeyConfig::OnBnClickedButtonSetkey15)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_1_6, &CDlgKeyConfig::OnBnClickedButtonSetkey16)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_1_7, &CDlgKeyConfig::OnBnClickedButtonSetkey17)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_1_8, &CDlgKeyConfig::OnBnClickedButtonSetkey18)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_2_1, &CDlgKeyConfig::OnBnClickedButtonSetkey21)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_2_2, &CDlgKeyConfig::OnBnClickedButtonSetkey22)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_2_3, &CDlgKeyConfig::OnBnClickedButtonSetkey23)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_2_4, &CDlgKeyConfig::OnBnClickedButtonSetkey24)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_2_6, &CDlgKeyConfig::OnBnClickedButtonSetkey26)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_2_7, &CDlgKeyConfig::OnBnClickedButtonSetkey27)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_2_8, &CDlgKeyConfig::OnBnClickedButtonSetkey28)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY_2_9, &CDlgKeyConfig::OnBnClickedButtonSetkey29)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CDlgKeyConfig::OnBnClickedButtonDefault)
	ON_BN_CLICKED(IDOK, &CDlgKeyConfig::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CDlgKeyConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO :  ここに初期化を追加してください
	UpdateNames();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// CDlgKeyConfig メッセージ ハンドラ
/*-----------------------------------------------------------------
	キー設定(1P)
-------------------------------------------------------------------*/
//←
void CDlgKeyConfig::OnBnClickedButtonSetkey11()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_left[0] = ret;
	UpdateNames();
}

//→
void CDlgKeyConfig::OnBnClickedButtonSetkey12()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_right[0] = ret;
	UpdateNames();
}

//↑
void CDlgKeyConfig::OnBnClickedButtonSetkey13()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_up[0] = ret;
	UpdateNames();
}

//↓
void CDlgKeyConfig::OnBnClickedButtonSetkey14()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_down[0] = ret;
	UpdateNames();
}

//ボタンA
void CDlgKeyConfig::OnBnClickedButtonSetkey15()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_button[0][0] = ret;
	UpdateNames();
}

//ボタンB
void CDlgKeyConfig::OnBnClickedButtonSetkey16()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_button[1][0] = ret;
	UpdateNames();
}

//ボタンC
void CDlgKeyConfig::OnBnClickedButtonSetkey17()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_button[2][0] = ret;
	UpdateNames();
}

//ボタンD
void CDlgKeyConfig::OnBnClickedButtonSetkey18()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_button[3][0] = ret;
	UpdateNames();
}

/*-----------------------------------------------------------------
	キー設定(2P)
-------------------------------------------------------------------*/
//←
void CDlgKeyConfig::OnBnClickedButtonSetkey21()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_left[1] = ret;
	UpdateNames();
}

//→
void CDlgKeyConfig::OnBnClickedButtonSetkey22()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_right[1] = ret;
	UpdateNames();
}

//↑
void CDlgKeyConfig::OnBnClickedButtonSetkey23()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_up[1] = ret;
	UpdateNames();
}

//↓
void CDlgKeyConfig::OnBnClickedButtonSetkey24()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_down[1] = ret;
	UpdateNames();
}

//ボタンA
void CDlgKeyConfig::OnBnClickedButtonSetkey26()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_button[0][1] = ret;
	UpdateNames();
}

//ボタンB
void CDlgKeyConfig::OnBnClickedButtonSetkey27()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_button[1][1] = ret;
	UpdateNames();
}

//ボタンC
void CDlgKeyConfig::OnBnClickedButtonSetkey28()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_button[2][1] = ret;
	UpdateNames();
}

//ボタンD
void CDlgKeyConfig::OnBnClickedButtonSetkey29()
{
	CDlgKBKeyGet dlg(this);
	int ret = (int)dlg.DoModal();

	if(ret==0)return;
	m_cfg.key_button[3][1] = ret;
	UpdateNames();
}

/*-----------------------------------------------------------------
	表示更新
-------------------------------------------------------------------*/
void CDlgKeyConfig::UpdateNames()
{
	//sstr_1_3.Format("↑ : %s(%d)", keycode.GetStr( VK_CAPITAL ),m_cfg.key_up[0]);

	sstr_1_3.Format("↑ : %s", keycode.GetStr( m_cfg.key_up[0] ));
	sstr_1_4.Format("↓ : %s", keycode.GetStr( m_cfg.key_down[0] ));
	sstr_1_1.Format("← : %s", keycode.GetStr( m_cfg.key_left[0] ));
	sstr_1_2.Format("→ : %s", keycode.GetStr( m_cfg.key_right[0] ));
	sstr_1_5.Format("A(弱) : %s", keycode.GetStr( m_cfg.key_button[0][0] ));
	sstr_1_6.Format("B(中) : %s", keycode.GetStr( m_cfg.key_button[1][0] ));
	sstr_1_7.Format("C(強) : %s", keycode.GetStr( m_cfg.key_button[2][0] ));
	sstr_1_8.Format("D(特) : %s", keycode.GetStr( m_cfg.key_button[3][0] ));
	
	sstr_2_3.Format("↑ : %s", keycode.GetStr( m_cfg.key_up[1] ));
	sstr_2_4.Format("↓ : %s", keycode.GetStr( m_cfg.key_down[1] ));
	sstr_2_1.Format("← : %s", keycode.GetStr( m_cfg.key_left[1] ));
	sstr_2_2.Format("→ : %s", keycode.GetStr( m_cfg.key_right[1] ));
	sstr_2_5.Format("A(弱) : %s", keycode.GetStr( m_cfg.key_button[0][1] ));
	sstr_2_6.Format("B(中) : %s", keycode.GetStr( m_cfg.key_button[1][1] ));
	sstr_2_7.Format("C(強) : %s", keycode.GetStr( m_cfg.key_button[2][1] ));
	sstr_2_8.Format("D(特) : %s", keycode.GetStr( m_cfg.key_button[3][1] ));

	UpdateData(FALSE);
}

/*-----------------------------------------------------------------
	キーコード → 文字変換
-------------------------------------------------------------------*/
CKeyCode2Str::CKeyCode2Str()
{
	//0x00-0x07
	while(m_list.size()<0x07+1)
	{
		Add("?");
	}

	Add("Back Space");	//8
	Add("Tab");		//9
	Add("?");			//a
	Add("?");			//b

	Add("Clear");		//c
	Add("Enter");		//d
	
	Add("?");			//e
	Add("?");			//f

	Add("SHIFT");
	Add("CONTROL");
	Add("Alt"); 
	Add("PAUSE");
	Add("Caps Lock");
	Add("かな");
	
	Add("?");			//0x16

	Add("JUNJA");
	Add("FINAL");
	Add("KANJI");

	Add("?");			//0x1a

	Add("Esc"); 
	Add("変換");
	Add("無変換"); 
	Add("ACCEPT");
	Add("モード変更");
	Add("スペース");
	Add("Page Up");
	Add("Page Down");
	Add("End");
	Add("Home");
	Add("←");
	Add("↑");
	Add("→");
	Add("↓");
	Add("Select");
	Add("Print");
	Add("Execute"); 
	Add("Print Scr");
	Add("Insert");
	Add("Delete");
	Add("Help");

	Add("0");
	Add("1");
	Add("2");
	Add("3");
	Add("4");
	Add("5");
	Add("6");
	Add("7");
	Add("8");
	Add("9");

	
	if(m_list.size()!='9'+1)OutputDebugString("CKeyCode2Str : チェック失敗 at 0x39\n");
	while(m_list.size()<0x40+1)
	{
		Add("?");
	}
	
	Add("A");
	Add("B");
	Add("C");
	Add("D");
	Add("E");
	Add("F");
	Add("G");
	Add("H");
	Add("I");
	Add("J");
	Add("K");
	Add("L");
	Add("M");
	Add("N");
	Add("O");
	Add("P");
	Add("Q");
	Add("R");
	Add("S");
	Add("T");
	Add("U");
	Add("V");
	Add("W");
	Add("X");
	Add("Y");
	Add("Z");

	Add("左Win");
	Add("右Win");
	Add("アプリ"); 
	Add("?");
	Add("SLEEP");

	if(m_list.size()!=0x5F+1)OutputDebugString("CKeyCode2Str : チェック失敗 at 0x5F\n");
	
	Add("テンキー0");
	Add("テンキー1");
	Add("テンキー2");
	Add("テンキー3");
	Add("テンキー4");
	Add("テンキー5");
	Add("テンキー6");
	Add("テンキー7");
	Add("テンキー8");
	Add("テンキー9");

	Add("テンキー[ * ]");
	Add("テンキー[ + ]");
	Add("テンキー[Enter]");
	Add("テンキー[ - ]");
	Add("テンキー[ . ]");
	Add("テンキー[ / ]");

	Add("F1");
	Add("F2");
	Add("F3");
	Add("F4");
	Add("F5<丶｀∀´>");
	Add("F6");
	Add("F7");
	Add("F8");
	Add("F9");
	Add("F10");
	Add("F11");
	Add("F12");
	Add("F13");
	Add("F14");
	Add("F15");
	Add("F16");
	Add("F17");
	Add("F18");
	Add("F19");
	Add("F20");
	Add("F21");
	Add("F22");
	Add("F23");
	Add("F24");

	if(m_list.size()!=VK_F24+1)OutputDebugString("CKeyCode2Str : チェック失敗 at 0x88\n");
	while(m_list.size()<0x8F+1)
	{
		Add("?");
	}

	Add("Num Lock");
	Add("Scroll Lock");
	Add("OEM1");
	Add("OEM2");
	Add("OEM3");
	Add("OEM4");
	Add("OEM5");

	if(m_list.size()!=0x96+1)OutputDebugString("CKeyCode2Str : チェック失敗 at 0x96\n");
	while(m_list.size()<0x9F+1)
	{
		Add("?");
	}

	Add("左Shift"); 
	Add("右Shift") ;
	Add("左Ctrl");
	Add("右Ctrl") ;
	Add("左Alt");
	Add("右Alt");

	if(m_list.size()!=0xA5+1)OutputDebugString("CKeyCode2Str : チェック失敗 at 0xA5\n");

	Add("ブラウザ[戻る]");
	Add("ブラウザ[進む]");
	Add("ブラウザ[田代砲（笑）]");
	Add("ブラウザ[ブレイク]");
	Add("ブラウザ[検索]");
	Add("ブラウザ[お気に入り]");
	Add("ブラウザ[ホームページ]");
	Add("メディア[ミュート]");
	Add("メディア[音量sage]");
	Add("メディア[音量age]");
	Add("メディア[次のトラック]");
	Add("メディア[前のトラック]");
	Add("メディア[停止]");
	Add("メディア[再生／一時停止]");
	Add("メール");
	Add("メディア選択");
	Add("アプリ起動１");
	Add("アプリ起動２");

	if(m_list.size()!=0xB7+1)OutputDebugString("CKeyCode2Str : チェック失敗 at 0xA5\n");
}

const char* CKeyCode2Str::GetStr(BYTE code)
{
	if(code >= m_list.size())
	{
		static char buffer[32]; // メンバにすんのﾏﾝﾄﾞｸｾ
		
		sprintf(buffer, "不明(%d番)", code);
		return buffer;
	}

	return m_list[code];
}

void CKeyCode2Str::Add(const char* str)
{
	char *tekito = new char [strlen(str)+1];
	strcpy(tekito,str);
	m_list.push_back(tekito);
}

CKeyCode2Str::~CKeyCode2Str()
{
	for(UINT i=0;i<m_list.size();i++)
	{
		delete [] m_list[i];
	}
}
void CDlgKeyConfig::OnBnClickedButtonDefault()
{
	g_config.SetDefaultConfigKB();
	m_cfg = g_config.keycfg;
	UpdateNames();
}

void CDlgKeyConfig::OnBnClickedOk()
{
	g_config.keycfg = m_cfg;
	OnOK();
}

void CDlgKeyConfig::OnCancel()
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	OnBnClickedOk();
	//CDialog::OnCancel();
}
