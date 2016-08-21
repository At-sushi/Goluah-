// are_editer.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "are_editer.h"
#include "are_editerDlg.h"
#include "DlgPage1.h"

#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 唯一の CAre_editerApp オブジェクトです。
CAre_editerApp theApp;

extern CDirectDraw g_draw;

struct CHAR64{char name[64];};

/*-------------------------------------------------------------------
// CAre_editerApp コンストラクション
---------------------------------------------------------------------*/
CAre_editerApp::CAre_editerApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
	UINT i;

	for(i=0;i<GCDMAX_IMAGES;i++){
		m_bmps[i] = NULL;
	}
	m_pCurrentRectNO=NULL;
	m_pCurrentCellNO=NULL;
}


/*-------------------------------------------------------------------
	メッセージマップ
---------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CAre_editerApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/*-------------------------------------------------------------------
// CAre_editerApp 初期化
---------------------------------------------------------------------*/
BOOL CAre_editerApp::InitInstance()
{
	CWinApp::InitInstance();

	AfxEnableControlContainer();


	CAre_editerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ダイアログが <OK> で消された時のコードを
		//       記述してください。
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ダイアログが <キャンセル> で消された時のコードを
		//       記述してください。
	}

	// ダイアログは閉じられました。アプリケーションのメッセージ ポンプを開始しないで
	// アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}


/*-------------------------------------------------------------------------
	共通ダイアログベース
---------------------------------------------------------------------------*/
IMPLEMENT_DYNAMIC(CMyDialogTab,CDialog)
CMyDialogTab::CMyDialogTab(UINT nIDTemplate,CWnd* pParentWnd)
	: CDialog(nIDTemplate, pParentWnd)
{
}

/*-------------------------------------------------------------------
	GCDファイルのロード 0.70/0.90自動判別
---------------------------------------------------------------------*/
void CAre_editerApp::LoadGCD(CString filename)
{
	CString sta;

	//「現在ディレクトリ」の更新
	m_current_dir = filename.Left( filename.ReverseFind('\\')+1 );
	sta += m_current_dir + CString(" にディレクトリ移動。");

	//GCDファイルロード
	if( 0!= m_gcd.Load(filename.GetBuffer(),TRUE,TRUE)){
		sta = "GCDファイルの読み込みに失敗";
	}
	else {
		int rnum = ReloadBitmaps();//ビットマップ更新
		CString sta2;
		sta2.Format(" , %d のビットマップをロード.",rnum);
		sta += sta2;
	}

	((CAre_editerDlg*)m_pMainWnd)->SetStatus(sta);
	InvalidateAll();
}

/*-------------------------------------------------------------------
	ビットマップファイルの再ロード
---------------------------------------------------------------------*/
int CAre_editerApp::ReloadBitmaps()
{
	UINT i;
	int ret =0;

	ASSERT( m_pMainWnd->IsKindOf(RUNTIME_CLASS(CAre_editerDlg)) );
	CDlgPage1* Dlg1 = ((CDlgPage1*)((CAre_editerDlg*)m_pMainWnd)->GetDialogTab(0));

	//現在のビットマップを破棄
	for(i=0;i<GCDMAX_IMAGES;i++){
		g_draw.RelSurface( m_bmps[i] );
		m_images[i].Destroy();
	}

	// 初期値再設定
	Dlg1->SetupDefaultFilenames();
	Dlg1->UpdateData(FALSE);

	//ビットマップをロード
	CString filename;
	for(i=0;i<GCDMAX_IMAGES;i++){
		filename = m_current_dir + GetBMPFilename(i);
		OutputDebugString(filename.GetBuffer());
		m_bmps[i] = g_draw.CreateSurfaceFrom256BMP(filename.GetBuffer());
		if(m_bmps[i]){
			OutputDebugString("...ok\n");
			ret++;
			m_images[i].Load(filename.GetBuffer());
		}
		else
		{
			{
				LPTSTR nganga = Dlg1->m_str_image[i].GetBuffer();

				if ( lstrcmp(nganga + lstrlen(nganga) - 4, _T(".png")) == 0)
				{
					// bmpに変えて読み直し
					lstrcpy( nganga + lstrlen(nganga) - 4, _T(".bmp") );
					Dlg1->m_str_image[i].ReleaseBuffer();

					i--;
					Dlg1->UpdateData(FALSE);
					continue;
				}

				Dlg1->m_str_image[i].ReleaseBuffer();
			}
			OutputDebugString("...failed\n");
		}
	}

	return ret;
}


/*-------------------------------------------------------------------
	再描画が必要
---------------------------------------------------------------------*/
void CAre_editerApp::InvalidateAll()
{
	g_childwnd.Dirty();
	((CAre_editerDlg*)m_pMainWnd)->UpdateItems();
}

/*-------------------------------------------------------------------
	現在編集中のCELLゲット
---------------------------------------------------------------------*/
GCD_RECT* CAre_editerApp::GetCurrentRect()
{
	if(!m_pCurrentRectNO)return NULL;
	return m_gcd.GetRect( *m_pCurrentRectNO );
}


/*-------------------------------------------------------------------
	現在編集中のCELL/判定矩形ゲット
---------------------------------------------------------------------*/
GCD_CELL2* CAre_editerApp::GetCurrentCell()
{
	if(!m_pCurrentCellNO)return NULL;
	return m_gcd.GetCell( *m_pCurrentCellNO );
}

GCD_HANTEI* CAre_editerApp::GetCurrentHantei()
{
	if(!m_pCurrentCellNO)return NULL;
	return m_gcd.GetHantei( *m_pCurrentCellNO );
}


/*-------------------------------------------------------------------
	ビットマップのファイル名ゲット
---------------------------------------------------------------------*/
CString CAre_editerApp::GetBMPFilename(UINT n)
{
	return ((CAre_editerDlg*)m_pMainWnd)->GetBMPFilename(n);
}


/*-------------------------------------------------------------------
	ビットマップのファイル名ゲット
---------------------------------------------------------------------*/
void CAre_editerApp::SetStatus(CString str)
{
	((CAre_editerDlg*)m_pMainWnd)->SetStatus(str);
}


/*-------------------------------------------------------------------
	特殊操作
---------------------------------------------------------------------*/
void CAre_editerApp::SpecialCtrl(UINT key)
{
	((CAre_editerDlg*)m_pMainWnd)->SpecialCtrl(key);
}


/*-------------------------------------------------------------
	空セル挿入処理
---------------------------------------------------------------*/
void CAre_editerApp::InsertEmptyCell(UINT index)
{
	if(!m_gcd.valid()){
		m_pMainWnd->MessageBox("ファイル読み込まれてないよ","",MB_OK);
		return ;
	}

	//index以降を全てずらす
	for(UINT i=GCDMAX_CELLS-1;i>index;i--){
		*m_gcd.GetCell(i) = *m_gcd.GetCell(i-1);
		*m_gcd.GetHantei(i) = *m_gcd.GetHantei(i-1);
		memcpy( m_gcd.GetName(i) , m_gcd.GetName(i-1) , sizeof(char)*GCD_MAXDEFINELENGTH);
	}

	m_gcd.GCDSetEmptyCell2( m_gcd.GetCell(index) , m_gcd.GetHantei(index) );
	ZeroMemory( m_gcd.GetName(index) , sizeof(char)*GCD_MAXDEFINELENGTH);

	InvalidateAll();

	SetStatus("空セル挿入処理-OK");
	return ;
}

/*-------------------------------------------------------------
	不正領域を参照している矩形を消去
---------------------------------------------------------------*/
void CAre_editerApp::DeleteInvalidRect()
{
	if(!m_gcd.valid()){
		m_pMainWnd->MessageBox("ファイル読み込まれてないよ","",MB_OK);
		return;
	}

	GCD_RECT *pr;
	COLORREF col;

	DWORD deletenum = 0;
	BOOL isEmpty ;

	int i;
	for(i=0;i<GCDMAX_RECTANGLES;i++)
	{
		isEmpty = TRUE;
		if( !m_gcd.IsEmptyRect(i) ){
			pr = m_gcd.GetRect(i);
			if(!m_images[pr->bmpno].IsNull() )
			{
				//ビットマップの範囲外を参照している場合は空とみなす
				//★

				//ビットマップを調べる。全部同じ色だったら空とみなす。
				col = m_images[pr->bmpno].GetPixel( pr->r.left,pr->r.top );//左上の色。コレと比較する
				for(int j=pr->r.top;j<pr->r.bottom;j++){
					for(int k=pr->r.left;k<pr->r.right;k++)
					{
						if(col != m_images[pr->bmpno].GetPixel(k,j)){
							isEmpty = FALSE;
							break;
						}
					}
					if(!isEmpty)break;
				}
			}
			if(isEmpty){
				deletenum++;
				m_gcd.GCDSetEmptyRect(pr); 
			}
		}
	}

	CString str_ret;
	str_ret.Format("%d個の矩形が削除されました",deletenum);
	m_pMainWnd->MessageBox(str_ret);
}

/*-------------------------------------------------------------
	面積0の矩形を消去して番号を詰める
---------------------------------------------------------------*/
void CAre_editerApp::DeleteEmptyRect()
{
	if(!m_gcd.valid()){
		m_pMainWnd->MessageBox("ファイル読み込まれてないよ","",MB_OK);
		return;
	}

	GCD_RECT empty_rect;				//「空」の矩形データ
	m_gcd.GCDSetEmptyRect(&empty_rect);

	DWORD org_ref[GCDMAX_RECTANGLES];	//元の番号とソート後の番号の対応
	std::vector<GCD_RECT> array;		//再配列後のデータ

	int i;
	
	//再配列データ構築
	array.push_back(*m_gcd.GetRect(0));//0番は「空矩形」扱いで予約済み
	org_ref[0] = 0;
	for(i=1;i<GCDMAX_RECTANGLES;i++)
	{
		if( !m_gcd.IsEmptyRect(i) ){
			org_ref[i] = (DWORD)array.size();
			array.push_back( *m_gcd.GetRect(i) );
		}
		else org_ref[i] = 0;
	}

	//再配列データを適用
	for(i=0;i<(int)array.size();i++)
	{
		*m_gcd.GetRect(i) = array[i];
	}
	for(;i<GCDMAX_RECTANGLES;i++)
	{
		*m_gcd.GetRect(i) = empty_rect;
	}


	//セルデータの矩形番号を更新
	GCD_CELL2 *pc;
	for(i=0;i<GCDMAX_CELLS;i++){
		pc = m_gcd.GetCell(i);
		for(int j=0;j<GCD_CELLGROUPNUM;j++){
			pc->cell[j].cdr = org_ref[pc->cell[j].cdr];
		}
	}

	m_pMainWnd->MessageBox("矩形並べ替え処理完了");
}

/*-------------------------------------------------------------
	空矩形のみを参照しているセルを消去
---------------------------------------------------------------*/
void CAre_editerApp::DeleteInvalidCell()
{
	if(!m_gcd.valid()){
		m_pMainWnd->MessageBox("ファイル読み込まれてないよ","",MB_OK);
		return;
	}

	GCD_CELL2 *pc;
	DWORD deletenum = 0;
	BOOL isEmpty ;

	int i;
	for(i=0;i<GCDMAX_RECTANGLES;i++)
	{
		isEmpty = TRUE;
		if( !m_gcd.IsEmptyCell(i) ){
			pc = m_gcd.GetCell(i);
			for(int j=0;j<GCD_CELLGROUPNUM;j++){
				if(pc->cell[j].cdr!=0){
					isEmpty = FALSE;
					break;
				}
			}
			if(isEmpty){
				deletenum++;
				m_gcd.GCDSetEmptyCell2(pc,m_gcd.GetHantei(i));
				ZeroMemory( m_gcd.GetName(i),GCD_MAXDEFINELENGTH );
			}
		}
	}

	CString str_ret;
	str_ret.Format("%d個のセルが削除されました",deletenum);
	m_pMainWnd->MessageBox(str_ret);
}


/*-------------------------------------------------------------
	空矩形のみを参照しているセルを消去
---------------------------------------------------------------*/
void CAre_editerApp::DeleteEmptyCell(BOOL over100)
{
	if(!m_gcd.valid()){
		m_pMainWnd->MessageBox("ファイル読み込まれてないよ","",MB_OK);
		return;
	}
	
	GCD_CELL2 empty_cell;			//「空」のセルデータ
	GCD_HANTEI empty_hantei;		//「空」の判定データ
	m_gcd.GCDSetEmptyCell2(&empty_cell,&empty_hantei);
	//「空」の定義名
	CHAR64 empty_name;
	ZeroMemory(&empty_name,sizeof(CHAR64));

	std::vector<GCD_CELL2> array;		//再配列後のデータ(cell)
	std::vector<GCD_HANTEI> array2;		//再配列後のデータ(判定)
	std::vector<CHAR64> array3;			//再配列後のデータ(定義名)

	int i;
	BOOL displace = FALSE;
	int start_index = over100 ? 100 : 0;
	CHAR64 tmpname;

	//再配列データ構築
	for(i=0;i<start_index;i++){
		array.push_back( *m_gcd.GetCell(i) );
		array2.push_back( *m_gcd.GetHantei(i) );
		memcpy(&tmpname,m_gcd.GetName(i),64);
		array3.push_back(tmpname);
		
	}
	for(;i<GCDMAX_CELLS;i++)
	{
		if( !m_gcd.IsEmptyCell(i) ){
			array.push_back( *m_gcd.GetCell(i) );
			array2.push_back( *m_gcd.GetHantei(i) );
			memcpy(&tmpname,m_gcd.GetName(i),64);
			array3.push_back(tmpname);
		}
	}

	//再配列データを適用
	for(i=0;i<(int)array.size();i++)
	{
		*m_gcd.GetCell(i) = array[i];
		*m_gcd.GetHantei(i) = array2[i];
		memcpy(m_gcd.GetName(i),&(array3[i]),64);
	}
	for(;i<GCDMAX_RECTANGLES;i++)
	{
		*m_gcd.GetCell(i) = empty_cell;
		*m_gcd.GetHantei(i) = empty_hantei;
		memcpy(m_gcd.GetName(i),&empty_name,64);
	}

	m_pMainWnd->MessageBox("セル並べ替え処理完了");
}