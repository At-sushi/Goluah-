// are_editer.cpp : �A�v���P�[�V�����̃N���X������`���܂��B
//

#include "stdafx.h"
#include "are_editer.h"
#include "are_editerDlg.h"
#include "DlgPage1.h"

#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// �B��� CAre_editerApp �I�u�W�F�N�g�ł��B
CAre_editerApp theApp;

extern CDirectDraw g_draw;

struct CHAR64{char name[64];};

/*-------------------------------------------------------------------
// CAre_editerApp �R���X�g���N�V����
---------------------------------------------------------------------*/
CAre_editerApp::CAre_editerApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
	UINT i;

	for(i=0;i<GCDMAX_IMAGES;i++){
		m_bmps[i] = NULL;
	}
	m_pCurrentRectNO=NULL;
	m_pCurrentCellNO=NULL;
}


/*-------------------------------------------------------------------
	���b�Z�[�W�}�b�v
---------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CAre_editerApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/*-------------------------------------------------------------------
// CAre_editerApp ������
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
		// TODO: �_�C�A���O�� <OK> �ŏ����ꂽ���̃R�[�h��
		//       �L�q���Ă��������B
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �_�C�A���O�� <�L�����Z��> �ŏ����ꂽ���̃R�[�h��
		//       �L�q���Ă��������B
	}

	// �_�C�A���O�͕����܂����B�A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n���Ȃ���
	// �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}


/*-------------------------------------------------------------------------
	���ʃ_�C�A���O�x�[�X
---------------------------------------------------------------------------*/
IMPLEMENT_DYNAMIC(CMyDialogTab,CDialog)
CMyDialogTab::CMyDialogTab(UINT nIDTemplate,CWnd* pParentWnd)
	: CDialog(nIDTemplate, pParentWnd)
{
}

/*-------------------------------------------------------------------
	GCD�t�@�C���̃��[�h 0.70/0.90��������
---------------------------------------------------------------------*/
void CAre_editerApp::LoadGCD(CString filename)
{
	CString sta;

	//�u���݃f�B���N�g���v�̍X�V
	m_current_dir = filename.Left( filename.ReverseFind('\\')+1 );
	sta += m_current_dir + CString(" �Ƀf�B���N�g���ړ��B");

	//GCD�t�@�C�����[�h
	if( 0!= m_gcd.Load(filename.GetBuffer(),TRUE,TRUE)){
		sta = "GCD�t�@�C���̓ǂݍ��݂Ɏ��s";
	}
	else {
		int rnum = ReloadBitmaps();//�r�b�g�}�b�v�X�V
		CString sta2;
		sta2.Format(" , %d �̃r�b�g�}�b�v�����[�h.",rnum);
		sta += sta2;
	}

	((CAre_editerDlg*)m_pMainWnd)->SetStatus(sta);
	InvalidateAll();
}

/*-------------------------------------------------------------------
	�r�b�g�}�b�v�t�@�C���̍ă��[�h
---------------------------------------------------------------------*/
int CAre_editerApp::ReloadBitmaps()
{
	UINT i;
	int ret =0;

	ASSERT( m_pMainWnd->IsKindOf(RUNTIME_CLASS(CAre_editerDlg)) );
	CDlgPage1* Dlg1 = ((CDlgPage1*)((CAre_editerDlg*)m_pMainWnd)->GetDialogTab(0));

	//���݂̃r�b�g�}�b�v��j��
	for(i=0;i<GCDMAX_IMAGES;i++){
		g_draw.RelSurface( m_bmps[i] );
		m_images[i].Destroy();
	}

	// �����l�Đݒ�
	Dlg1->SetupDefaultFilenames();
	Dlg1->UpdateData(FALSE);

	//�r�b�g�}�b�v�����[�h
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
					// bmp�ɕς��ēǂݒ���
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
	�ĕ`�悪�K�v
---------------------------------------------------------------------*/
void CAre_editerApp::InvalidateAll()
{
	g_childwnd.Dirty();
	((CAre_editerDlg*)m_pMainWnd)->UpdateItems();
}

/*-------------------------------------------------------------------
	���ݕҏW����CELL�Q�b�g
---------------------------------------------------------------------*/
GCD_RECT* CAre_editerApp::GetCurrentRect()
{
	if(!m_pCurrentRectNO)return NULL;
	return m_gcd.GetRect( *m_pCurrentRectNO );
}


/*-------------------------------------------------------------------
	���ݕҏW����CELL/�����`�Q�b�g
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
	�r�b�g�}�b�v�̃t�@�C�����Q�b�g
---------------------------------------------------------------------*/
CString CAre_editerApp::GetBMPFilename(UINT n)
{
	return ((CAre_editerDlg*)m_pMainWnd)->GetBMPFilename(n);
}


/*-------------------------------------------------------------------
	�r�b�g�}�b�v�̃t�@�C�����Q�b�g
---------------------------------------------------------------------*/
void CAre_editerApp::SetStatus(CString str)
{
	((CAre_editerDlg*)m_pMainWnd)->SetStatus(str);
}


/*-------------------------------------------------------------------
	���ꑀ��
---------------------------------------------------------------------*/
void CAre_editerApp::SpecialCtrl(UINT key)
{
	((CAre_editerDlg*)m_pMainWnd)->SpecialCtrl(key);
}


/*-------------------------------------------------------------
	��Z���}������
---------------------------------------------------------------*/
void CAre_editerApp::InsertEmptyCell(UINT index)
{
	if(!m_gcd.valid()){
		m_pMainWnd->MessageBox("�t�@�C���ǂݍ��܂�ĂȂ���","",MB_OK);
		return ;
	}

	//index�ȍ~��S�Ă��炷
	for(UINT i=GCDMAX_CELLS-1;i>index;i--){
		*m_gcd.GetCell(i) = *m_gcd.GetCell(i-1);
		*m_gcd.GetHantei(i) = *m_gcd.GetHantei(i-1);
		memcpy( m_gcd.GetName(i) , m_gcd.GetName(i-1) , sizeof(char)*GCD_MAXDEFINELENGTH);
	}

	m_gcd.GCDSetEmptyCell2( m_gcd.GetCell(index) , m_gcd.GetHantei(index) );
	ZeroMemory( m_gcd.GetName(index) , sizeof(char)*GCD_MAXDEFINELENGTH);

	InvalidateAll();

	SetStatus("��Z���}������-OK");
	return ;
}

/*-------------------------------------------------------------
	�s���̈���Q�Ƃ��Ă����`������
---------------------------------------------------------------*/
void CAre_editerApp::DeleteInvalidRect()
{
	if(!m_gcd.valid()){
		m_pMainWnd->MessageBox("�t�@�C���ǂݍ��܂�ĂȂ���","",MB_OK);
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
				//�r�b�g�}�b�v�͈̔͊O���Q�Ƃ��Ă���ꍇ�͋�Ƃ݂Ȃ�
				//��

				//�r�b�g�}�b�v�𒲂ׂ�B�S�������F���������Ƃ݂Ȃ��B
				col = m_images[pr->bmpno].GetPixel( pr->r.left,pr->r.top );//����̐F�B�R���Ɣ�r����
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
	str_ret.Format("%d�̋�`���폜����܂���",deletenum);
	m_pMainWnd->MessageBox(str_ret);
}

/*-------------------------------------------------------------
	�ʐ�0�̋�`���������Ĕԍ����l�߂�
---------------------------------------------------------------*/
void CAre_editerApp::DeleteEmptyRect()
{
	if(!m_gcd.valid()){
		m_pMainWnd->MessageBox("�t�@�C���ǂݍ��܂�ĂȂ���","",MB_OK);
		return;
	}

	GCD_RECT empty_rect;				//�u��v�̋�`�f�[�^
	m_gcd.GCDSetEmptyRect(&empty_rect);

	DWORD org_ref[GCDMAX_RECTANGLES];	//���̔ԍ��ƃ\�[�g��̔ԍ��̑Ή�
	std::vector<GCD_RECT> array;		//�Ĕz���̃f�[�^

	int i;
	
	//�Ĕz��f�[�^�\�z
	array.push_back(*m_gcd.GetRect(0));//0�Ԃ́u���`�v�����ŗ\��ς�
	org_ref[0] = 0;
	for(i=1;i<GCDMAX_RECTANGLES;i++)
	{
		if( !m_gcd.IsEmptyRect(i) ){
			org_ref[i] = (DWORD)array.size();
			array.push_back( *m_gcd.GetRect(i) );
		}
		else org_ref[i] = 0;
	}

	//�Ĕz��f�[�^��K�p
	for(i=0;i<(int)array.size();i++)
	{
		*m_gcd.GetRect(i) = array[i];
	}
	for(;i<GCDMAX_RECTANGLES;i++)
	{
		*m_gcd.GetRect(i) = empty_rect;
	}


	//�Z���f�[�^�̋�`�ԍ����X�V
	GCD_CELL2 *pc;
	for(i=0;i<GCDMAX_CELLS;i++){
		pc = m_gcd.GetCell(i);
		for(int j=0;j<GCD_CELLGROUPNUM;j++){
			pc->cell[j].cdr = org_ref[pc->cell[j].cdr];
		}
	}

	m_pMainWnd->MessageBox("��`���בւ���������");
}

/*-------------------------------------------------------------
	���`�݂̂��Q�Ƃ��Ă���Z��������
---------------------------------------------------------------*/
void CAre_editerApp::DeleteInvalidCell()
{
	if(!m_gcd.valid()){
		m_pMainWnd->MessageBox("�t�@�C���ǂݍ��܂�ĂȂ���","",MB_OK);
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
	str_ret.Format("%d�̃Z�����폜����܂���",deletenum);
	m_pMainWnd->MessageBox(str_ret);
}


/*-------------------------------------------------------------
	���`�݂̂��Q�Ƃ��Ă���Z��������
---------------------------------------------------------------*/
void CAre_editerApp::DeleteEmptyCell(BOOL over100)
{
	if(!m_gcd.valid()){
		m_pMainWnd->MessageBox("�t�@�C���ǂݍ��܂�ĂȂ���","",MB_OK);
		return;
	}
	
	GCD_CELL2 empty_cell;			//�u��v�̃Z���f�[�^
	GCD_HANTEI empty_hantei;		//�u��v�̔���f�[�^
	m_gcd.GCDSetEmptyCell2(&empty_cell,&empty_hantei);
	//�u��v�̒�`��
	CHAR64 empty_name;
	ZeroMemory(&empty_name,sizeof(CHAR64));

	std::vector<GCD_CELL2> array;		//�Ĕz���̃f�[�^(cell)
	std::vector<GCD_HANTEI> array2;		//�Ĕz���̃f�[�^(����)
	std::vector<CHAR64> array3;			//�Ĕz���̃f�[�^(��`��)

	int i;
	BOOL displace = FALSE;
	int start_index = over100 ? 100 : 0;
	CHAR64 tmpname;

	//�Ĕz��f�[�^�\�z
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

	//�Ĕz��f�[�^��K�p
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

	m_pMainWnd->MessageBox("�Z�����בւ���������");
}