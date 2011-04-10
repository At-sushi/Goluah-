#include "..\goluah_config\dlgpage1.h"
// DlgPage1.cpp : 実装ファイル
//

#include "stdafx.h"
#include "are_editer.h"
#include "DlgPage1.h"


// CDlgPage1 ダイアログ

IMPLEMENT_DYNAMIC(CDlgPage1, CMyDialogTab)
CDlgPage1::CDlgPage1(CWnd* pParent /*=NULL*/)
	: CMyDialogTab(CDlgPage1::IDD, pParent)
	, m_bDefaltBMP(FALSE)
{
	SetupDefaultFilenames();
}

void CDlgPage1::SetupDefaultFilenames()
{
	m_str_image[0]=(_T("image1.png"));
	m_str_image[1]=(_T("image2.png"));
	m_str_image[2]=(_T("image3.png"));
	m_str_image[3]=(_T("image4.png"));
	m_str_image[4]=(_T("image5.png"));
	m_str_image[5]=(_T("image6.png"));
	m_str_image[6]=(_T("image7.png"));
	m_str_image[7]=(_T("image8.png"));
	m_str_image[8]=(_T("image9.png"));
	m_str_image[9]=(_T("image10.png"));
	m_str_image[10]=(_T("image11.png"));
	m_str_image[11]=(_T("image12.png"));
}

CDlgPage1::~CDlgPage1()
{
}

void CDlgPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_bDefaltBMP);
	DDX_Text(pDX, IDC_EDIT1, m_str_image[0]);
	DDX_Text(pDX, IDC_EDIT2, m_str_image[1]);
	DDX_Text(pDX, IDC_EDIT3, m_str_image[2]);
	DDX_Text(pDX, IDC_EDIT4, m_str_image[3]);
	DDX_Text(pDX, IDC_EDIT5, m_str_image[4]);
	DDX_Text(pDX, IDC_EDIT6, m_str_image[5]);
	DDX_Text(pDX, IDC_EDIT7, m_str_image[6]);
	DDX_Text(pDX, IDC_EDIT8, m_str_image[7]);
	DDX_Text(pDX, IDC_EDIT9, m_str_image[8]);
	DDX_Text(pDX, IDC_EDIT10, m_str_image[9]);
	DDX_Text(pDX, IDC_EDIT11, m_str_image[10]);
	DDX_Text(pDX, IDC_EDIT12, m_str_image[11]);
}


BEGIN_MESSAGE_MAP(CDlgPage1, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_REF1, OnBnClickedButtonRef1)
	ON_BN_CLICKED(IDC_BUTTON_REF2, OnBnClickedButtonRef2)
	ON_BN_CLICKED(IDC_BUTTON_REF3, OnBnClickedButtonRef3)
	ON_BN_CLICKED(IDC_BUTTON_REF4, OnBnClickedButtonRef4)
	ON_BN_CLICKED(IDC_BUTTON_REF5, OnBnClickedButtonRef5)
	ON_BN_CLICKED(IDC_BUTTON_REF6, OnBnClickedButtonRef6)
	ON_BN_CLICKED(IDC_BUTTON_REF7, OnBnClickedButtonRef7)
	ON_BN_CLICKED(IDC_BUTTON_REF8, OnBnClickedButtonRef8)
	ON_BN_CLICKED(IDC_BUTTON_REF9, OnBnClickedButtonRef9)
	ON_BN_CLICKED(IDC_BUTTON_REF10, OnBnClickedButtonRef10)
	ON_BN_CLICKED(IDC_BUTTON_REF11, OnBnClickedButtonRef11)
	ON_BN_CLICKED(IDC_BUTTON_REF12, OnBnClickedButtonRef12)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_HEADER, OnBnClickedButtonLoadHeader)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_HEADER, OnBnClickedButtonSaveHeader)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnBnClickedButtonSave)
//	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgPage1 メッセージ ハンドラ

void CDlgPage1::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO : ここにメッセージ ハンドラ コードを追加します。
	// 描画メッセージで CDialog::OnPaint() を呼び出さないでください。
}

/*---------------------------------------------------------
	初期化
-----------------------------------------------------------*/
BOOL CDlgPage1::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO :  ここに初期化を追加してください

	m_bDefaltBMP = TRUE;
	UpdateData(FALSE);//メンバ変数の値をコントロールに通知

	OnBnClickedCheck1();//エディットボックス・参照ボタン無効化
	GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);//★暫定

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*----------------------------------------------------------
	imageファイル参照（ボタン）
------------------------------------------------------------*/
void CDlgPage1::OnBnClickedButtonRef1()
{
	OnBnClickedButtonRef(0);
}

void CDlgPage1::OnBnClickedButtonRef2()
{
	OnBnClickedButtonRef(1);
}

void CDlgPage1::OnBnClickedButtonRef3()
{
	OnBnClickedButtonRef(2);
}

void CDlgPage1::OnBnClickedButtonRef4()
{
	OnBnClickedButtonRef(3);
}

void CDlgPage1::OnBnClickedButtonRef5()
{
	OnBnClickedButtonRef(4);
}

void CDlgPage1::OnBnClickedButtonRef6()
{
	OnBnClickedButtonRef(5);
}

void CDlgPage1::OnBnClickedButtonRef7()
{
	OnBnClickedButtonRef(6);
}

void CDlgPage1::OnBnClickedButtonRef8()
{
	OnBnClickedButtonRef(7);
}

void CDlgPage1::OnBnClickedButtonRef9()
{
	OnBnClickedButtonRef(8);
}

void CDlgPage1::OnBnClickedButtonRef10()
{
	OnBnClickedButtonRef(9);
}

void CDlgPage1::OnBnClickedButtonRef11()
{
	OnBnClickedButtonRef(10);
}

void CDlgPage1::OnBnClickedButtonRef12()
{
	OnBnClickedButtonRef(11);
}

//↑ボタンの処理本体
void CDlgPage1::OnBnClickedButtonRef(UINT idx)
{
}


/*----------------------------------------------------------
	「デフォルトのファイル名を使用」チェックボックス変更時処理
------------------------------------------------------------*/
void CDlgPage1::OnBnClickedCheck1()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	UpdateData(TRUE);

	BOOL b = m_bDefaltBMP ? FALSE : TRUE;

	GetDlgItem(IDC_EDIT1)->EnableWindow(b);
	GetDlgItem(IDC_EDIT2)->EnableWindow(b);
	GetDlgItem(IDC_EDIT3)->EnableWindow(b);
	GetDlgItem(IDC_EDIT4)->EnableWindow(b);
	GetDlgItem(IDC_EDIT5)->EnableWindow(b);
	GetDlgItem(IDC_EDIT6)->EnableWindow(b);
	GetDlgItem(IDC_EDIT7)->EnableWindow(b);
	GetDlgItem(IDC_EDIT8)->EnableWindow(b);
	GetDlgItem(IDC_EDIT9)->EnableWindow(b);
	GetDlgItem(IDC_EDIT10)->EnableWindow(b);
	GetDlgItem(IDC_EDIT11)->EnableWindow(b);
	GetDlgItem(IDC_EDIT12)->EnableWindow(b);
	
	GetDlgItem(IDC_BUTTON_REF1)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON_REF2)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON_REF3)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON_REF4)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON_REF5)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON_REF6)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON_REF7)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON_REF8)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON_REF9)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON_REF10)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON_REF11)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON_REF12)->EnableWindow(b);
}


/*---------------------------------------------------------
	ファイルのロード（ボタン）
-----------------------------------------------------------*/
void CDlgPage1::OnBnClickedButtonLoad()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	CFileDialog dlg( TRUE, "gcd", "*.gcd; *.gcm", 
        OFN_FILEMUSTEXIST,"GCDファイル (*.gcd, *.gcm)|*.gcd; *.gcm|すべて(*.*)|*.*||",  this);

    if( dlg.DoModal() == IDOK)
    {
		theApp.LoadGCD( dlg.GetPathName() );
    }
	else theApp.SetStatus("保存処理キャンセル");
}

/*----------------------------------------------------------
	.h ファイルのロード
------------------------------------------------------------*/
void CDlgPage1::OnBnClickedButtonLoadHeader()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	CFileDialog dlg( TRUE, "h", "*.h", 
        OFN_FILEMUSTEXIST,"C/C++ヘッダーファイル (*.h)|*.h|すべてのファイル(*.*)|*.*||",  this);
	if( dlg.DoModal() != IDOK)return;

	CString str;

	CFile file;
	if(!file.Open( dlg.GetPathName() ,CFile::modeRead )){
		str.Format(".h ファイルオープンに失敗 (%s)",dlg.GetPathName());
		theApp.SetStatus(str);
		return;
	}
	UINT fsize = (UINT)file.GetLength();
	char* buff = new char[fsize+1];
	file.Read(buff,fsize);
	buff[fsize]='\0';
	file.Close();

	char* s = buff;
	char* send = buff+fsize;
	char  strbuff[64];
	int   tmpnum;

	int oldnum = 0;
	int newnum = 0;
	int errnum = 0;

	//現在の定義名をクリア
	CGCDHandler *gcd = theApp.GetGCD();
	for(UINT i=0;i<GCDMAX_CELLS;i++)
	{
		if(strlen( gcd->m_names->name[i] )>0)
		{
			oldnum++;
			strcpy(gcd->m_names->name[i],"");
		}
	}

	//取り込み...
	while(s < send)
	{
		if(*s=='#')
		{
			int scaret =sscanf(s,"#define CELL_%s %d",strbuff,&tmpnum); 
			if(2==scaret && tmpnum>=0 && tmpnum<GCDMAX_CELLS)
			{
				strcpy(gcd->m_names->name[tmpnum],strbuff);
				newnum++;
			}
			else
			{
				errnum++;
			}
		}

		s++;
	}
	delete [] buff; 

	if(errnum==0){
		str.Format("定義名数:%d→%d",oldnum,newnum);
	}
	else str.Format("定義名数:%d→%d , エラー数:%d",oldnum,newnum,errnum);

	theApp.SetStatus(str);
}

/*----------------------------------------------------------
	.h ファイルの保存
------------------------------------------------------------*/
void CDlgPage1::OnBnClickedButtonSaveHeader()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	CFileDialog dlg( FALSE, "h", "*.h", 
        OFN_OVERWRITEPROMPT,"C/C++ヘッダーファイル (*.h)|*.h|すべてのファイル(*.*)|*.*||",  this);

	CString str;

	if( dlg.DoModal() == IDOK)
    {
		CFile file;
		if(!file.Open( dlg.GetPathName() ,CFile::modeCreate | CFile::modeWrite )){
			str.Format(".h 保存処理: ファイルオープンに失敗 (%s)",dlg.GetPathName());
			theApp.SetStatus(str);
			return;
		}
		char crlf[2] = {13,10};

		UINT rnum=0;

		CGCDHandler *gcd = theApp.GetGCD();
		for(UINT i=0;i<GCDMAX_CELLS;i++)
		{
			if(strlen( gcd->m_names->name[i] )>0)
			{
				str.Format("#define CELL_%s %d",gcd->m_names->name[i],i);
				char *c = str.GetBuffer();
				file.Write(c,(UINT)strlen(c));
				file.Write(crlf,2);
				rnum++;
			}
		}
		file.Close();
		str.Format(".h 保存処理完了. 定義数 %d",rnum);
		theApp.SetStatus(str);
	}
	else theApp.SetStatus(".h 保存処理キャンセル");
}


/*----------------------------------------------------------
	ファイルのセーブ（ボタン）
------------------------------------------------------------*/
void CDlgPage1::OnBnClickedButtonSave()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	if(! theApp.GetGCD()->Save() ){
		theApp.SetStatus("ファイル保存失敗？");
	}
	else{
		theApp.SetStatus("ファイルを保存しました");
	}
}


void CDlgPage1::OnEnChangeEdit1()
{
	// TODO :  これが RICHEDIT コントロールの場合、まず、CMyDialogTab::OnInitDialog() 関数をオーバーライドして、
	// OR 状態の ENM_CORRECTTEXT フラグをマスクに入れて、
	// CRichEditCtrl().SetEventMask() を呼び出さない限り、
	// コントロールは、この通知を送信しません。

	// TODO :  ここにコントロール通知ハンドラ コードを追加してください。
}
