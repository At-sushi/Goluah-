// gcd_tool03.cpp : �A�v���P�[�V�����̃N���X������`���܂��B
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


// Cgcd_tool03App �R���X�g���N�V����

Cgcd_tool03App::Cgcd_tool03App()
{
	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}


// �B��� Cgcd_tool03App �I�u�W�F�N�g�ł��B

Cgcd_tool03App theApp;


// Cgcd_tool03App ������

BOOL Cgcd_tool03App::InitInstance()
{
	// �A�v���P�[�V�����@�}�j�t�F�X�g���@visual �X�^�C����L���ɂ��邽�߂ɁA
	// ComCtl32.dll �o�[�W���� 6�@�ȍ~�̎g�p���w�肷��ꍇ�́A
	// Windows XP �Ɂ@InitCommonControls() ���K�v�ł��B�����Ȃ���΁A�E�B���h�E�쐬�͂��ׂĎ��s���܂��B
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();


	Cgcd_tool03Dlg dlg;
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




/*-------------------------------------------------------------
	�t�@�C�����[�h
---------------------------------------------------------------*/
void Cgcd_tool03App::Load(char *filename)
{
	Cgcd_tool03Dlg *pdlg = dynamic_cast<Cgcd_tool03Dlg*>(m_pMainWnd);
	if(!pdlg)return;
	pdlg->UpdateData(TRUE);

	//GCD�t�@�C���̃��[�h
	switch(m_gcd.Load(filename,TRUE,TRUE))
	{
	case 0:break;//����I��
	case 1:pdlg->m_str_status="�t�@�C���I�[�I�[�v�����s";pdlg->UpdateData(FALSE);return;
	case 3:pdlg->m_str_status="���@�[�W�����`�F�b�N���s";pdlg->UpdateData(FALSE);return;
	default:pdlg->m_str_status="�t�@�C���ǂݍ��ݎ��s";pdlg->UpdateData(FALSE);return;
	}

	//�C���[�W�̃��[�h
	CString str_path(filename);
	int slapos = str_path.ReverseFind('\\');
	str_path = str_path.Left( slapos+1 );

	CString str_img;
	for(int i=0;i<GCDMAX_IMAGES;i++){
		str_img.Format("%simage%d.bmp",str_path,i+1);
		m_img[i].Load(str_img);
	}

	pdlg->m_str_status="�t�@�C�������[�h���܂����B";
	pdlg->UpdateData(FALSE);
}


/*-------------------------------------------------------------
	���`��������
---------------------------------------------------------------*/
void Cgcd_tool03App::DeleteEmptyRect()
{
	if(!m_gcd.valid()){
		m_pMainWnd->MessageBox("�t�@�C���ǂݍ��܂�ĂȂ���","",MB_OK);
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
				//�r�b�g�}�b�v�𒲂ׂ�B�S�������F���������Ƃ݂Ȃ��B
				col = m_img[pr->bmpno].GetPixel( pr->r.left,pr->r.top );//����̐F�B�R���Ɣ�r����
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

	pdlg->m_str_status.Format("%d�̋�`���폜����܂���",deletenum);
	pdlg->UpdateData(FALSE);
}


/*-------------------------------------------------------------
	��Z����������
---------------------------------------------------------------*/
void Cgcd_tool03App::DeleteEmptyCell()
{
	if(!m_gcd.valid()){
		m_pMainWnd->MessageBox("�t�@�C���ǂݍ��܂�ĂȂ���","",MB_OK);
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

	pdlg->m_str_status.Format("%d�̃Z�����폜����܂���",deletenum);
	pdlg->UpdateData(FALSE);
}

/*-------------------------------------------------------------
	��`���בւ�����
---------------------------------------------------------------*/
void Cgcd_tool03App::SortRects()
{
	if(!m_gcd.valid()){
		m_pMainWnd->MessageBox("�t�@�C���ǂݍ��܂�ĂȂ���","",MB_OK);
		return;
	}
	Cgcd_tool03Dlg *pdlg = dynamic_cast<Cgcd_tool03Dlg*>(m_pMainWnd);
	if(!pdlg)return;
	pdlg->UpdateData(TRUE);

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
			org_ref[i] = array.size();
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

	pdlg->m_str_status.Format("��`���בւ���������");
	pdlg->UpdateData(FALSE);
}


/*-------------------------------------------------------------
	�Z�����בւ�����
---------------------------------------------------------------*/
void Cgcd_tool03App::SortCells(BOOL over100)
{
	if(!m_gcd.valid()){
		m_pMainWnd->MessageBox("�t�@�C���ǂݍ��܂�ĂȂ���","",MB_OK);
		return;
	}
	Cgcd_tool03Dlg *pdlg = dynamic_cast<Cgcd_tool03Dlg*>(m_pMainWnd);
	if(!pdlg)return;
	pdlg->UpdateData(TRUE);

	
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

	pdlg->m_str_status.Format("�Z�����בւ���������");
	pdlg->UpdateData(FALSE);
}


/*-------------------------------------------------------------
	��Z���}��
---------------------------------------------------------------*/
BOOL Cgcd_tool03App::InsertEmptyCell(UINT index)
{
	if(!m_gcd.valid()){
		m_pMainWnd->MessageBox("�t�@�C���ǂݍ��܂�ĂȂ���","",MB_OK);
		return FALSE;
	}

	//index�ȍ~��S�Ă��炷
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
	���V�I1.0
---------------------------------------------------------------*/
void Cgcd_tool03App::ForceRatio()
{
	if(!m_gcd.valid()){
		m_pMainWnd->MessageBox("�t�@�C���ǂݍ��܂�ĂȂ���","",MB_OK);
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
	pdlg->m_str_status.Format("%d��cell���~�o",cnt);
	pdlg->UpdateData(FALSE);

	return ;
}
