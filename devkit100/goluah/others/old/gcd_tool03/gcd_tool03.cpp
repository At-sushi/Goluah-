// gcd_tool03.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"

#include <vector>
#include "gcd_tool03.h"
#include "gcd_tool03Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Cgcd_tool03App

BEGIN_MESSAGE_MAP(Cgcd_tool03App, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// Cgcd_tool03App コンストラクション

Cgcd_tool03App::Cgcd_tool03App()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}


// 唯一の Cgcd_tool03App オブジェクトです。

Cgcd_tool03App theApp;


// Cgcd_tool03App 初期化

BOOL Cgcd_tool03App::InitInstance()
{
	// アプリケーション　マニフェストが　visual スタイルを有効にするために、
	// ComCtl32.dll バージョン 6　以降の使用を指定する場合は、
	// Windows XP に　InitCommonControls() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();


	Cgcd_tool03Dlg dlg;
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




/*-------------------------------------------------------------
	ファイルロード
---------------------------------------------------------------*/
void Cgcd_tool03App::Load(char *filename)
{
	Cgcd_tool03Dlg *pdlg = dynamic_cast<Cgcd_tool03Dlg*>(m_pMainWnd);
	if(!pdlg)return;
	pdlg->UpdateData(TRUE);

	//GCDファイルのロード
	switch(m_gcd.Load(filename,TRUE,TRUE))
	{
	case 0:break;//正常終了
	case 1:pdlg->m_str_status="ファイルオーオープン失敗";pdlg->UpdateData(FALSE);return;
	case 3:pdlg->m_str_status="ヴァージョンチェック失敗";pdlg->UpdateData(FALSE);return;
	default:pdlg->m_str_status="ファイル読み込み失敗";pdlg->UpdateData(FALSE);return;
	}

	//イメージのロード
	CString str_path(filename);
	int slapos = str_path.ReverseFind('\\');
	str_path = str_path.Left( slapos+1 );

	CString str_img;
	for(int i=0;i<GCDMAX_IMAGES;i++){
		str_img.Format("%simage%d.bmp",str_path,i+1);
		m_img[i].Load(str_img);
	}

	pdlg->m_str_status="ファイルをロードしました。";
	pdlg->UpdateData(FALSE);
}


/*-------------------------------------------------------------
	空矩形消去処理
---------------------------------------------------------------*/
void Cgcd_tool03App::DeleteEmptyRect()
{
	if(!m_gcd.valid()){
		m_pMainWnd->MessageBox("ファイル読み込まれてないよ","",MB_OK);
		return;
	}
	Cgcd_tool03Dlg *pdlg = dynamic_cast<Cgcd_tool03Dlg*>(m_pMainWnd);
	if(!pdlg)return;
	pdlg->UpdateData(TRUE);

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
			if(!m_img[pr->bmpno].IsNull() ){
				//ビットマップを調べる。全部同じ色だったら空とみなす。
				col = m_img[pr->bmpno].GetPixel( pr->r.left,pr->r.top );//左上の色。コレと比較する
				for(int j=pr->r.top;j<pr->r.bottom;j++){
					for(int k=pr->r.left;k<pr->r.right;k++)
					{
						if(col != m_img[pr->bmpno].GetPixel(k,j)){
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

	pdlg->m_str_status.Format("%d個の矩形が削除されました",deletenum);
	pdlg->UpdateData(FALSE);
}


/*-------------------------------------------------------------
	空セル消去処理
---------------------------------------------------------------*/
void Cgcd_tool03App::DeleteEmptyCell()
{
	if(!m_gcd.valid()){
		m_pMainWnd->MessageBox("ファイル読み込まれてないよ","",MB_OK);
		return;
	}
	Cgcd_tool03Dlg *pdlg = dynamic_cast<Cgcd_tool03Dlg*>(m_pMainWnd);
	if(!pdlg)return;
	pdlg->UpdateData(TRUE);

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

	pdlg->m_str_status.Format("%d個のセルが削除されました",deletenum);
	pdlg->UpdateData(FALSE);
}

/*-------------------------------------------------------------
	矩形並べ替え処理
---------------------------------------------------------------*/
void Cgcd_tool03App::SortRects()
{
	if(!m_gcd.valid()){
		m_pMainWnd->MessageBox("ファイル読み込まれてないよ","",MB_OK);
		return;
	}
	Cgcd_tool03Dlg *pdlg = dynamic_cast<Cgcd_tool03Dlg*>(m_pMainWnd);
	if(!pdlg)return;
	pdlg->UpdateData(TRUE);

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
			org_ref[i] = array.size();
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

	pdlg->m_str_status.Format("矩形並べ替え処理完了");
	pdlg->UpdateData(FALSE);
}


/*-------------------------------------------------------------
	セル並べ替え処理
---------------------------------------------------------------*/
void Cgcd_tool03App::SortCells(BOOL over100)
{
	if(!m_gcd.valid()){
		m_pMainWnd->MessageBox("ファイル読み込まれてないよ","",MB_OK);
		return;
	}
	Cgcd_tool03Dlg *pdlg = dynamic_cast<Cgcd_tool03Dlg*>(m_pMainWnd);
	if(!pdlg)return;
	pdlg->UpdateData(TRUE);

	
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

	pdlg->m_str_status.Format("セル並べ替え処理完了");
	pdlg->UpdateData(FALSE);
}


/*-------------------------------------------------------------
	空セル挿入
---------------------------------------------------------------*/
BOOL Cgcd_tool03App::InsertEmptyCell(UINT index)
{
	if(!m_gcd.valid()){
		m_pMainWnd->MessageBox("ファイル読み込まれてないよ","",MB_OK);
		return FALSE;
	}

	//index以降を全てずらす
	for(UINT i=GCDMAX_CELLS-1;i>index;i--){
		*m_gcd.GetCell(i) = *m_gcd.GetCell(i-1);
		*m_gcd.GetHantei(i) = *m_gcd.GetHantei(i-1);
		memcpy( m_gcd.GetName(i) , m_gcd.GetHantei(i-1) , sizeof(char)*GCD_MAXDEFINELENGTH);
	}

	m_gcd.GCDSetEmptyCell2( m_gcd.GetCell(index) , m_gcd.GetHantei(index) );
	ZeroMemory( m_gcd.GetName(index) , sizeof(char)*GCD_MAXDEFINELENGTH);

	return TRUE;
}


/*-------------------------------------------------------------
	レシオ1.0
---------------------------------------------------------------*/
void Cgcd_tool03App::ForceRatio()
{
	if(!m_gcd.valid()){
		m_pMainWnd->MessageBox("ファイル読み込まれてないよ","",MB_OK);
		return;
	}

	GCD_CELL2 *c;
	UINT cnt =0;

	for(UINT i=0;i<GCDMAX_CELLS;i++){
		c = m_gcd.GetCell(i);
		for(UINT j=0;j<GCD_CELLGROUPNUM;j++){
			if(c->cell[j].alpha_ratio==0)cnt++;
			c->cell[j].alpha_ratio	=1.0f;
			c->cell[j].red_ratio	=1.0f;
			c->cell[j].green_ratio	=1.0f;
			c->cell[j].blue_ratio	=1.0f;
		}
	}

	Cgcd_tool03Dlg *pdlg = dynamic_cast<Cgcd_tool03Dlg*>(m_pMainWnd);
	if(!pdlg)return;
	pdlg->UpdateData(TRUE);
	pdlg->m_str_status.Format("%d個のcellを救出",cnt);
	pdlg->UpdateData(FALSE);

	return ;
}
