
/*==============================================================================

	�O���[�o���֐�
	�O���[�o���ϐ��@�Ȃ�

================================================================================*/

#include "stdafx.h"
#include "global.h"
#include "bigface_cache.h"

/*------------------------------------------------------------------------------
	�O���[�o���I�u�W�F�N�g(�N���X)
--------------------------------------------------------------------------------*/

CDirectDraw				g_draw;			//�`��I�u�W�F�N�g
CDirectInput			g_input;		//���̓I�u�W�F�N�g
CDirectSound			g_sound;		//���n�I�u�W�F�N�g
CDirectPlay				g_play;			//�ʐM�I�u�W�F�N�g
	
CSystem					g_system;		//�V�X�e���I�u�W�F�N�g

AkiGlobal				gbl;			//�O���[�o���֐��̓��ꕨ
CLog2File				g_log2file;		//�t�@�C���փ��O�ۑ��N���X
CExport					g_exp;			//DLL�ւ̊֐��񋟃N���X
CStoryList				g_storylist;	//�X�g�[���[�Ǘ��N���X
CStageList				g_stagelist;	//�X�e�[�W���X�g�Ǘ��N���X
CCharacterList			g_charlist;		//�L�����N�^�[���X�g�Ǘ��N���X
CBattleLoadInfo			g_battleinfo;	//�������N���X
CBattleResultInfo		g_battleresult;	//�������ʏ��N���X

/*------------------------------------------------------------------------------
	�O���[�o���ϐ�
--------------------------------------------------------------------------------*/
TCHAR AppName[]=_T("Goluah!! (߄D�)");		//�A�v���P�[�V������
BOOL g_programexit=FALSE;				//���C�����[�v�I���t���O
RECT g_rcClient;						//window mode�̂Ƃ��ɕK�v(?)
int g_DISPLAYWIDTH=640;					//�����E�C���h�E�T�C�Y�i���j
int g_DISPLAYHEIGHT=480;				//�����E�C���h�E�T�C�Y�i�����j
BOOL g_muki[MAXNUM_KEYI];				//�L�[���͂̌���




/*==============================================================================

	�O���[�o���֐��@(gbl.�`)

================================================================================*/


/*
	�E�C���h�E���f�B�X�v���C���S�ɂ����Ă����B
	�\�[�X��web�Œ��ׂĂǂ�������R�s�����L��������B���N�O�ɁB

	@param hWnd ����Ώۂ̃E�C���h�E�n���h��
	@return �Ӗ��Ȃ��݂����B���0�B
*/
int AkiGlobal::SetWinCenter(HWND hWnd)
{
    HWND hDeskWnd;
    RECT deskrc, rc;
    int x, y;
    
    hDeskWnd = GetDesktopWindow();
    GetWindowRect(hDeskWnd, (LPRECT)&deskrc);
    GetWindowRect(hWnd, (LPRECT)&rc);
    x = (deskrc.right - (rc.right-rc.left)) / 2;
    y = (deskrc.bottom - (rc.bottom-rc.top)) / 2;
    SetWindowPos(hWnd, HWND_TOP, x, y, (rc.right-rc.left), (rc.bottom-rc.top),SWP_SHOWWINDOW); 
    return 0;
}



/*
	�w��t�@�C�������邩�ǂ������ׂ�

	@param path �{�̎��s�t�@�C������Ƃ������΃p�X�B���Ԃ�B
	@return TRUE:����� , FALSE:�Ȃ���
*/
BOOL AkiGlobal::FileExist(TCHAR *path)
{
	HANDLE hFile = CreateFile(path,
		GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	if(hFile==INVALID_HANDLE_VALUE){
		return(FALSE);
	}
	else {
		CloseHandle(hFile);
		return(TRUE);
	}
}



/*------------------------------------------------------------------------------
	��`�̏Փˌ��o

	Syototu2RECTs	: �֐��{��
	Syototu2RECTsA	: ��]�����̏ꍇ�̋�`�̏Փˌ��o
	Syototu2RECTsB	: ��]�̂���ꍇ�̋�`�̏Փˌ��o
	getmiddle		: 4�̒l�̂������Ԃ̓��������
	GetKas_LINEvsRECT: ���Ƌ�`�̏Փˌ��o
	g2dt_rot , g2dt_tns , g2dt_mag , g2dt_rvx , g2dt_rvy : 2D���W�ϊ�

--------------------------------------------------------------------------------*/

//!2D���W�ϊ�
inline void g2dt_rot(MY2DVECTOR *v,int rot)//��]
{
	double ox = v->x;
	double oy = v->y;
	v->x = ox*cos(D3DXToRadian(rot)) - oy*sin(D3DXToRadian(rot));
	v->y = oy*cos(D3DXToRadian(rot)) + ox*sin(D3DXToRadian(rot));
}
inline void g2dt_tns(MY2DVECTOR *v,double tx,double ty)//!< ���s�ړ�
	{v->x += tx;v->y += ty;}
inline void g2dt_mag(MY2DVECTOR *v,double mx,double my)//!< �g��
	{v->x *= mx;v->y *= my;}
inline void g2dt_rvx(MY2DVECTOR *v){v->x *= (-1);}
inline void g2dt_rvy(MY2DVECTOR *v){v->y *= (-1);}

/*!
	@brief ��]�����̏ꍇ�̋�`�̏Փˌ��o
	@sa AkiGlobal::Syototu2RECTs
	@param r1 1P�������`
	@param r2 2P�������`
	@param point [out] �Փ˃|�C���g���󂯎�邽�߂̃|�C���^
	@param gx1 1P���d�S(x)
	@param gy1 1P���d�S(y)
	@param rx1 1P��X���]�t���O
	@param ry1 1P��Y���]�t���O
	@param tx1 1P�����s�ړ���X
	@param ty1 1P�����s�ړ���Y
	@param mx1 1P��X�g�嗦
	@param my1 1P��Y�g�嗦
	@param gx2 2P���d�S(x)
	@param gy2 2P���d�S(y)
	@param rx2 2P��X���]�t���O
	@param ry2 2P��Y���]�t���O
	@param tx2 2P�����s�ړ���X
	@param ty2 2P�����s�ړ���Y
	@param mx2 2P��X�g�嗦
	@param my2 2P��Y�g�嗦
	@param magmode1 1P���g�僂�[�h
	@param magmode2 2P���g�僂�[�h
*/
BOOL Syototu2RECTsA(RECT r1,RECT r2,MY2DVECTOR *point,
		int gx1,int gy1,BOOL rx1,BOOL ry1,double tx1,double ty1,double mx1,double my1,
		int gx2,int gy2,BOOL rx2,BOOL ry2,double tx2,double ty2,double mx2,double my2,UINT magmode1,UINT magmode2);

/*!
	@brief ��]�����̏ꍇ�̋�`�̏Փˌ��o
	��`�ʐς�1P��<2P���ɂȂ�悤�ɂ��邱��

	@sa AkiGlobal::Syototu2RECTs
	@param r1 1P�������`
	@param r2 2P�������`
	@param point [out] �Փ˃|�C���g���󂯎�邽�߂̃|�C���^
	@param gx1 1P���d�S(x)
	@param gy1 1P���d�S(y)
	@param rx1 1P��X���]�t���O
	@param ry1 1P��Y���]�t���O
	@param rot1 1P����]�p(degree)
	@param tx1 1P�����s�ړ���X
	@param ty1 1P�����s�ړ���Y
	@param mx1 1P��X�g�嗦
	@param my1 1P��Y�g�嗦
	@param gx2 2P���d�S(x)
	@param gy2 2P���d�S(y)
	@param rx2 2P��X���]�t���O
	@param ry2 2P��Y���]�t���O
	@param rot2 2P����]�p(degree)
	@param tx2 2P�����s�ړ���X
	@param ty2 2P�����s�ړ���Y
	@param mx2 2P��X�g�嗦
	@param my2 2P��Y�g�嗦
	@param magmode1 1P���g�僂�[�h
	@param magmode2 2P���g�僂�[�h
*/
BOOL Syototu2RECTsB(RECT r1,RECT r2,MY2DVECTOR *point,
		int gx1,int gy1,BOOL rx1,BOOL ry1,int rot1,double tx1,double ty1,double mx1,double my1,
		int gx2,int gy2,BOOL rx2,BOOL ry2,int rot2,double tx2,double ty2,double mx2,double my2,UINT magmode1,UINT magmode2);
void getmiddle(int a,int b,int c,int d,long *m1,long *m2);
DWORD GetKas_LINEvsRECT(MY2DVECTOR *kaspoint,MY2DVECTOR &line_start,MY2DVECTOR &line_end,MY2DVECTOR rect_pos[4]);

/*!
	@brief ��`�̏Փˌ��o

	���ۂ̏����͉�]�̗L���� Syototu2RECTsA �� Syototu2RECTsB �ɐU���Ă���B

	@sa AkiGlobal::Syototu2RECTs
	@param r1 1P�������`
	@param r2 2P�������`
	@param point [out] �Փ˃|�C���g���󂯎�邽�߂̃|�C���^
	@param gx1 1P���d�S(x)
	@param gy1 1P���d�S(y)
	@param rx1 1P��X���]�t���O
	@param ry1 1P��Y���]�t���O
	@param rot1 1P����]�p(degree)
	@param tx1 1P�����s�ړ���X
	@param ty1 1P�����s�ړ���Y
	@param mx1 1P��X�g�嗦
	@param my1 1P��Y�g�嗦
	@param gx2 2P���d�S(x)
	@param gy2 2P���d�S(y)
	@param rx2 2P��X���]�t���O
	@param ry2 2P��Y���]�t���O
	@param rot2 2P����]�p(degree)
	@param tx2 2P�����s�ړ���X
	@param ty2 2P�����s�ړ���Y
	@param mx2 2P��X�g�嗦
	@param my2 2P��Y�g�嗦
	@param magmode1 1P���g�僂�[�h
	@param magmode2 2P���g�僂�[�h
*/
BOOL AkiGlobal::Syototu2RECTs(RECT r1,RECT r2,MY2DVECTOR *point,
		int gx1,int gy1,BOOL rx1,BOOL ry1,int rot1,double tx1,double ty1,double mx1,double my1,
		int gx2,int gy2,BOOL rx2,BOOL ry2,int rot2,double tx2,double ty2,double mx2,double my2,
		UINT magmode1,UINT magmode2
		)
{
	//�ʐς�0�Ȃ��O����
	if(r1.left==r1.right || r1.top==r1.bottom)return(FALSE);
	if(r2.left==r2.right || r2.top==r2.bottom)return(FALSE);
	if(mx1==0 || my1==0 || mx2==0 || my2==0)return(FALSE);

	try{//���܂�0���Z��O���N�����炵��

		double menseki[2];

		if(rot1%360==0 && rot2%360==0){
			return(Syototu2RECTsA(r1,r2,point,gx2,gy2,rx1,ry1,tx1,ty1,mx1,my1,gx2,gy2,rx2,ry2,tx2,ty2,mx2,my2,magmode1,magmode2));
		}
		else{
			//�X�O�x�ɂȂ��Ă��܂��Ƃ��낢��܂����̂ŁA�������܂���
			if((rot1+rot2)%90==0)rot1+=1;
			//�ʐϔ�r
			menseki[0]=(r1.right - r1.left)*(r1.bottom - r1.top);
			menseki[0]= menseki[0]*menseki[0];
			menseki[1]=(r2.right - r2.left)*(r2.bottom - r2.top);
			menseki[1]= menseki[1]*menseki[1];
			if(menseki[0] < menseki[1])
				return(Syototu2RECTsB(r1,r2,point,gx1,gy1,rx1,ry1,rot1,tx1,ty1,mx1,my1,gx2,gy2,rx2,ry2,rot2,tx2,ty2,mx2,my2,magmode1,magmode2));
			else
				return(Syototu2RECTsB(r2,r1,point,gx2,gy2,rx2,ry2,rot2,tx2,ty2,mx2,my2,gx1,gy1,rx1,ry1,rot1,tx1,ty1,mx1,my1,magmode1,magmode2));
		}
	}catch(...){
		return FALSE;
	}
}

//��]���Ȃ��ꍇ�A2�̋�`�̏Փ˂����o����
BOOL Syototu2RECTsA(RECT r1,RECT r2,MY2DVECTOR *point,
		int gx1,int gy1,BOOL rx1,BOOL ry1,double tx1,double ty1,double mx1,double my1,
		int gx2,int gy2,BOOL rx2,BOOL ry2,double tx2,double ty2,double mx2,double my2,
		UINT magmode1,UINT magmode2)
{
	if(magmode1==1)
	{
		// x/y�g��
		r1.left		=(long)(r1.left  *mx1);
		r1.right	=(long)(r1.right *mx1);
		r1.top		=(long)(r1.top   *my1);
		r1.bottom	=(long)(r1.bottom*my1);
	}
	if(magmode2==1)
	{
		r2.left		=(long)(r2.left  *mx2);
		r2.right	=(long)(r2.right *mx2);
		r2.top		=(long)(r2.top   *my2);
		r2.bottom	=(long)(r2.bottom*my2);
	}

	//�d�S�Ɏ����Ă���
	r1.left		-= gx1;
	r1.right	-= gx1;
	r1.top		-= gy1;
	r1.bottom	-= gy1;
	r2.left		-= gx2;
	r2.right	-= gx2;
	r2.top		-= gy2;
	r2.bottom	-= gy2;

	//y���]
	if(ry1){
		r1.top*=-1;
		r1.bottom*=-1;
	}
	if(ry2){
		r2.top*=-1;
		r2.bottom*=-1;
	}

	
	if(magmode1==0)
	{
		// x/y�g��
		r1.left		=(long)(r1.left  *mx1);
		r1.right	=(long)(r1.right *mx1);
		r1.top		=(long)(r1.top   *my1);
		r1.bottom	=(long)(r1.bottom*my1);
	}
	if(magmode2==0)
	{
		r2.left		=(long)(r2.left  *mx2);
		r2.right	=(long)(r2.right *mx2);
		r2.top		=(long)(r2.top   *my2);
		r2.bottom	=(long)(r2.bottom*my2);
	}

	//�d�S���猳�ɖ߂�
	r1.left		+= gx1;
	r1.right	+= gx1;
	r1.top		+= gy1;
	r1.bottom	+= gy1;
	r2.left		+= gx2;
	r2.right	+= gx2;
	r2.top		+= gy2;
	r2.bottom	+= gy2;

	// x���]
	if(rx1){
		r1.left*=-1;
		r1.right*=-1;
	}
	if(rx2){
		r2.left*=-1;
		r2.right*=-1;
	}

	//�ړ�
	r1.left		= (long)(r1.left  +tx1);
	r1.right	= (long)(r1.right +tx1);
	r1.top		= (long)(r1.top   +ty1);
	r1.bottom	= (long)(r1.bottom+ty1);
	r2.left		= (long)(r2.left  +tx2);
	r2.right	= (long)(r2.right +tx2);
	r2.top		= (long)(r2.top   +ty2);
	r2.bottom	= (long)(r2.bottom+ty2);

	//�l�𐳂�������
	int tmp;
	if(r1.left > r1.right){tmp=r1.left;r1.left=r1.right;r1.right=tmp;}
	if(r2.left > r2.right){tmp=r2.left;r2.left=r2.right;r2.right=tmp;}
	if(r1.top > r1.bottom){tmp=r1.top;r1.top=r1.bottom;r1.bottom=tmp;}
	if(r2.top > r2.bottom){tmp=r2.top;r2.top=r2.bottom;r2.bottom=tmp;}

	if(r1.right < r2.left)return(FALSE);
	if(r2.right < r1.left)return(FALSE);
	if(r1.bottom < r2.top)return(FALSE);
	if(r2.bottom < r1.top)return(FALSE);

	RECT r3;
	if(point!=NULL){
		getmiddle(r1.left,r1.right,r2.left,r2.right,&(r3.left),&(r3.right));
		getmiddle(r1.top,r2.top,r1.bottom,r2.bottom,&(r3.top),&(r3.bottom));

		point->x = (r3.left + r3.right)/2;
		point->y = (r3.top + r3.bottom)/2;
	}

	return(TRUE);
}

void getmiddle(int a,int b,int c,int d,long *m1,long *m2)//!< 4�̒l�̂������Ԃ̓��������
{
	if(a>b && a>c && a>d){//a���ő�
		if(b<c && b<d){//b���ŏ�
			if(c<d){*m1=c;*m2=d;}
			else{*m1=d;*m2=c;}
		}
		else if(c<d){//c���ŏ�
			if(b<d){*m1=b;*m2=d;}
			else{*m1=d;*m2=b;}
		}
		else{//d���ŏ�
			if(b<c){*m1=b;*m2=c;}
			else{*m1=c;*m2=b;}
		}
	}
	else if(b>c && b>d){//b���ő�
		if(a<c && a<d){//a���ŏ�
			if(c<d){*m1=c;*m2=d;}
			else{*m1=d;*m2=c;}
		}
		else if(c<d){//c���ŏ�
			if(a<d){*m1=a;*m2=d;}
			else{*m1=d;*m2=a;}
		}
		else{//d���ŏ�
			if(a<c){*m1=a;*m2=c;}
			else{*m1=c;*m2=a;}
		}
	}
	else if(c>d){//c���ő�
		if(b<a && b<d){//b���ŏ�
			if(a<d){*m1=a;*m2=d;}
			else{*m1=d;*m2=a;}
		}
		else if(a<d){//a���ŏ�
			if(b<d){*m1=b;*m2=d;}
			else{*m1=d;*m2=b;}
		}
		else{//d���ŏ�
			if(b<a){*m1=b;*m2=a;}
			else{*m1=a;*m2=b;}
		}
	}
	else {//d���ő�
		if(b<c && b<a){//b���ŏ�
			if(c<d){*m1=c;*m2=a;}
			else{*m1=a;*m2=c;}
		}
		else if(c<a){//c���ŏ�
			if(b<a){*m1=b;*m2=a;}
			else{*m1=a;*m2=b;}
		}
		else{//a���ŏ�
			if(b<c){*m1=b;*m2=c;}
			else{*m1=c;*m2=b;}
		}
	}
}

//��]�������ƍl�����ċ�`�̏Փ˂𔻒肷�� / r1�̂ق���r2��肿������
BOOL Syototu2RECTsB(RECT r1,RECT r2,MY2DVECTOR *point,
		int gx1,int gy1,BOOL rx1,BOOL ry1,int rot1,double tx1,double ty1,double mx1,double my1,
		int gx2,int gy2,BOOL rx2,BOOL ry2,int rot2,double tx2,double ty2,double mx2,double my2,
		UINT magmode1,UINT magmode2)
{
	if (!point) return FALSE;
	
	//top<bottom , left<right��ۏ�
	int tmp;
	if(r2.left > r2.right){tmp=r2.left;r2.left=r2.right;r2.right=tmp;}
	if(r2.top > r2.bottom){tmp=r2.top;r2.top=r2.bottom;r2.bottom=tmp;}

	MY2DVECTOR mv[2][4];
	//����
	mv[0][0].x = r1.left;
	mv[0][0].y = r1.top;
	mv[1][0].x = r2.left;
	mv[1][0].y = r2.top;
	//�E��
	mv[0][1].x = r1.right;
	mv[0][1].y = r1.top;
	mv[1][1].x = r2.right;
	mv[1][1].y = r2.top;
	//����
	mv[0][2].x = r1.left;
	mv[0][2].y = r1.bottom;
	mv[1][2].x = r2.left;
	mv[1][2].y = r2.bottom;
	//�E��
	mv[0][3].x = r1.right;
	mv[0][3].y = r1.bottom;
	mv[1][3].x = r2.right;
	mv[1][3].y = r2.bottom;

	//r1��r2�̍��W�n�ɕϊ�����
	int i;
	for(i=0;i<4;i++){
		if(magmode1==1){
			g2dt_mag(&mv[0][i],mx1,my1);//�g��i�������S�j
		}
		g2dt_tns(&mv[0][i],gx1,gx1);//�d�S���W�Ɏ����Ă���
		if(ry1)g2dt_rvy(&mv[0][i]);//y���]
		if(magmode1==0){
			g2dt_mag(&mv[0][i],mx1,my1);//�g��i�d�S���S�j
		}
		g2dt_rot(&mv[0][i],rot1);//��]
		g2dt_tns(&mv[0][i],-gx1,-gx1);//�d�S�߂�
		if(rx1)g2dt_rvx(&mv[0][i]);//x���]
		g2dt_tns(&mv[0][i],tx1,ty1);

		//here , the world
		
		g2dt_tns(&mv[0][i],-tx2,-ty2);
		if(rx2)g2dt_rvx(&mv[0][i]);//x���]
		g2dt_tns(&mv[0][i],gx2,gx2);//�d�S�߂�
		g2dt_rot(&mv[0][i],-rot2);//��]
		if(magmode2==0){
			g2dt_mag(&mv[0][i],1.0/mx2,1.0/my2);//�g��(�d�S���S)
		}
		if(ry2)g2dt_rvy(&mv[0][i]);//y���]
		g2dt_tns(&mv[0][i],-gx2,-gx2);//�d�S���W�Ɏ����Ă���
		if(magmode2==1){
			g2dt_mag(&mv[0][i],1.0/mx2,1.0/my2);//�g��
		}
	}

	int num_kas_line=0;//����������C���̐�
	MY2DVECTOR kas_points[8];//��������_�̃��X�g�B�����ő�8�_
	//4�ӓ��m�̌������o���b�I
	num_kas_line+=GetKas_LINEvsRECT(
		&kas_points[num_kas_line],
		mv[0][0],mv[0][1],
		mv[1]
		);
	num_kas_line+=GetKas_LINEvsRECT(
		&kas_points[num_kas_line],
		mv[0][1],mv[0][2],
		mv[1]
		);
	num_kas_line+=GetKas_LINEvsRECT(
		&kas_points[num_kas_line],
		mv[0][2],mv[0][3],
		mv[1]
		);
	num_kas_line+=GetKas_LINEvsRECT(
		&kas_points[num_kas_line],
		mv[0][3],mv[0][0],
		mv[1]
		);

	if(num_kas_line >0){//������Ă�D�D�D�D�E�D
		point->x =0;
		point->y =0;
		for(tmp=0;tmp<num_kas_line;tmp++){
			point->x += kas_points[tmp].x;
			point->y += kas_points[tmp].y;
		}
		point->x /= num_kas_line;
		point->y /= num_kas_line;
		//�Փ˂����_�����[���h���W�n�ɕϊ�����
		//here , the world
		g2dt_tns(point,gx2,gx2);//�d�S���W�Ɏ����Ă���
		if(ry2)g2dt_rvy(point);//y���]
		g2dt_mag(point,mx2,my2);//�g��
		g2dt_rot(point,rot2);//��]
		g2dt_tns(point,-gx2,-gx2);//�d�S�߂�
		if(rx2)g2dt_rvx(point);//x���]
		g2dt_tns(point,tx2,ty2);

		return(TRUE);
	}

	//����̋�`����������̋�`�̓����ɂ��邩�H
	if( mv[0][0].x > mv[1][2].x && mv[0][0].x < mv[1][1].x ){
		if( mv[0][0].y > mv[1][1].y && mv[0][0].y < mv[1][2].y ){
			if( mv[0][1].x > mv[1][2].x && mv[0][1].x < mv[1][1].x ){
				if( mv[0][1].y > mv[1][1].y && mv[0][1].y < mv[1][2].y ){
					if( mv[0][2].x > mv[1][2].x && mv[0][2].x < mv[1][1].x ){
						if( mv[0][2].y > mv[1][1].y && mv[0][2].y < mv[1][2].y ){
							if( mv[0][3].x > mv[1][2].x && mv[0][3].x < mv[1][1].x ){
								if( mv[0][3].y > mv[1][1].y && mv[0][3].y < mv[1][2].y ){
									//�����ɂ���D�D�D�D�D�b�D�D�D�D
									point->x=0;
									point->y=0;
									for(tmp=0;tmp<4;tmp++){
										point->x+=mv[0][tmp].x;
										point->y+=mv[0][tmp].y;
									}
									point->x/=4;
									point->y/=4;
									//�Փ˂����_�����[���h���W�n�ɕϊ�����
									g2dt_tns(point,gx2,gx2);//�d�S���W�Ɏ����Ă���
									if(ry2)g2dt_rvy(point);//y���]
									g2dt_mag(point,mx2,my2);//�g��
									g2dt_rot(point,rot2);//��]
									g2dt_tns(point,-gx2,-gx2);//�d�S�߂�
									if(rx2)g2dt_rvx(point);//x���]
									g2dt_tns(point,tx2,ty2);

									return(TRUE);
								}
							}
						}
					}
				}
			}
		}
	}
	
	return(FALSE);//�Փ˂͂���܂���ł���
}


//!�����Ƌ�`�̊e�ӂƂ̌����𒲂ׂ�
DWORD GetKas_LINEvsRECT(MY2DVECTOR *kaspoint,
						MY2DVECTOR &line_start,MY2DVECTOR &line_end,
						MY2DVECTOR rect_pos[4])
{
	double a,b;//�����̕����� ax+by+1=0
	double xmin,xmax,ymin,ymax;//�����Ɗe�ӂ������\��������͈�
	double x[7],y[7];//�Ă�ۂ��
	DWORD num_kas=0;

	//�܂��͒����̕����������߂�
	x[3]=line_start.x;
	y[3]=line_start.y;
	x[4]=line_end.x;
	y[4]=line_end.y;
	if(x[3]==0 || x[4]*y[3]-x[3]*y[4]==0)return(0);//0�ł̏��Z�͂ł��Ȃ���
	b = (x[3]-x[4])/(x[4]*y[3]-x[3]*y[4]);
	a = (-1.0/x[3])*(1.0 + b*y[3] );

	if(a==0 || b==0)return(0);//���Ȃ�ʓ|�Ȃ̂ť��

	//���R���Ƃ̂܂����
	xmin = __min(x[3],x[4]);
	xmax = __max(x[3],x[4]);
	if(xmin<rect_pos[1].x){
		if(xmax>rect_pos[0].x){
			x[5] = __max(xmin,rect_pos[0].x);
			x[6] = __min(xmax,rect_pos[1].x);
			y[5] = -(a/b)*x[5] - 1.0/b ;
			y[6] = -(a/b)*x[6] - 1.0/b ;
			y[1] = rect_pos[1].y;
			if( (y[5]-y[1])*(y[6]-y[1]) <0 ){//�����
				//��������_�̌v�Z
				y[0]=y[1];
				x[0]=-(b*y[0]+1)/a;
				kaspoint[num_kas].x=x[0];
				kaspoint[num_kas].y=y[0];
				num_kas++;
			}
			y[1] = rect_pos[2].y;
			if( (y[5]-y[1])*(y[6]-y[1]) <0 ){//�����
				//��������_�̌v�Z
				y[0]=y[1];
				x[0]=-(b*y[0]+1)/a;
				kaspoint[num_kas].x=x[0];
				kaspoint[num_kas].y=y[0];
				num_kas++;
			}
		}
	}

	//�^�e���Ƃ̌����
	ymin = __min(y[3],y[4]);
	ymax = __max(y[3],y[4]);
	if(ymin<rect_pos[2].y){
		if(ymax>rect_pos[1].y){
			y[5] = __max(ymin,rect_pos[1].y);
			y[6] = __min(ymax,rect_pos[2].y);
			x[5] = -(b*y[5]+1.0)/a ;
			x[6] = -(b*y[6]+1.0)/a ;
			x[1] = rect_pos[1].x;
			if( (x[5]-x[1])*(x[6]-x[1]) <0 ){//�����
				//��������_�̌v�Z
				x[0]=x[1];
				y[0]= -(a*x[0]+1.0)/b ;
				kaspoint[num_kas].x=x[0];
				kaspoint[num_kas].y=y[0];
				num_kas++;
			}
			x[1] = rect_pos[2].x;
			if( (x[5]-x[1])*(x[6]-x[1]) <0 ){//�����
				//��������_�̌v�Z
				x[0]=x[1];
				y[0]= -(a*x[0]+1.0)/b ;
				kaspoint[num_kas].x=x[0];
				kaspoint[num_kas].y=y[0];
				num_kas++;
			}
		}
	}

	return(num_kas);
}


//2������𑫂��ĐV�����o�b�t�@��Ԃ�
//�߂�l��new[]�Ŋm�ۂ����o�b�t�@�Bstr1��delete[]����܂��B
TCHAR* AkiGlobal::MergeString(TCHAR* str1,const TCHAR* str2)
{
	UINT len=0;
	
	if(str1){
		len += strlen(str1);
	}
	
	if(str2){
		len += strlen(str2);
	}
	else return str1;

	TCHAR* ret = new TCHAR [len+1];
	if(!ret)return NULL;

	if(str1){
		_stprintf(ret,_T("%s%s"),str1,str2);
	}
	else _stprintf(ret,_T("%s"),str2);

	DELETEARRAY(str1);

	return ret;
}


/*
	@brief �w��f�B���N�g�������烉���_����BGM��I�����čĐ�
	@param dir �w��f�B���N�g���B�{�̎��s�t�@�C������
*/
void AkiGlobal::PlayRandomBGM(TCHAR *dir)
{
	if(!g_config.UseDShow())return;

	//�t�@�C�������X�g
	std::vector<TCHAR*> filelist;
	std::vector<TCHAR*>::iterator ite;
	std::vector<TCHAR*>::iterator itee;

	int numstr;	//������
	TCHAR filepath[MAX_PATH];//�e���|�����p�X

	//�w��f�B���N�g�����̃t�@�C���������X�g�Ɏ��W�i�g���q�ʂ��j
	HANDLE hFind;
	WIN32_FIND_DATA fd;
	_stprintf(filepath,_T("%s\\*.*"),dir);
	hFind = FindFirstFile( filepath , &fd);
	if(hFind == INVALID_HANDLE_VALUE)return;//���s
	do {
		if(strcmp(fd.cFileName,_T("."))==0 || strcmp(fd.cFileName,_T(".."))==0 );
		else if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			numstr = strlen( fd.cFileName );
			TCHAR *str = new TCHAR [numstr+1];
			str[numstr]='\0';

			//�ӂ�����߂����ҁ[
			while(numstr>0)
			{
				numstr--;
				str[numstr] = fd.cFileName[numstr];
			}

			//�g���q�폜
			numstr = strlen( fd.cFileName );
			while(numstr>0)
			{
				numstr--;
				if(fd.cFileName[numstr]=='.')
				{
					str[numstr]='\0';
					break;
				}
			}

			filelist.push_back(str);
		}
	} while(FindNextFile(hFind, &fd));
	FindClose(hFind);

	//�t�@�C�����ЂƂ��Ȃ�
	if(filelist.size()==0){
		gbl.ods(_T("AkiGlobal::PlayRandomBGM : no files\n"));
		return;
	}

	//�V���b�t��
	ite = filelist.begin();
	itee= filelist.end();
	std::random_shuffle(ite,itee);

	//�Đ������݂�
	ite = filelist.begin();
	itee= filelist.end();
	for(;ite!=itee;ite++)
	{
		_stprintf( filepath, _T("%s\\%s"),dir,*ite);
		if(g_sound.BGMPlay( filepath ))
		{
			break;//�Đ��ɐ���������I��
		}
		gbl.ods2(_T("AkiGlobal::PlayRandomBGM : %s ...failed"),filepath);
	}
//	delete [] filepath;

	//�t�@�C�����̃��X�g��j��
	ite = filelist.begin();
	itee= filelist.end();
	for(;ite!=itee;ite++)
	{
		delete [] (*ite);
	}
}

/*=========================================================================
	��static
===========================================================================*/

#define MAX_DUMMY_STRING	16

//�m��
AkiGlobal::AkiGlobal()
{
	m_dummybuf = CreateStringArray(MAX_DUMMY_STRING,128);
	m_dummy_next = 0;

	g_system.AddTask(new CTBigFaceCache);
	m_blankIcon = NULL;

	m_ods_buffer = new TCHAR [1024*8];
}

//�J��
AkiGlobal::~AkiGlobal()
{
	DeleteStringArray(m_dummybuf);
	//RELSURFACE(m_blankIcon);//g_draw�j�����Ɏ����I�ɏ����邩�炢����

	DELETEARRAY(m_ods_buffer);
}



/*----------------------------------------------------------------
	�����񃆁[�e�B���e�B
------------------------------------------------------------------*/

//�X�N���v�g�ǂݍ���_tscanf�Ŏg���_�~�[�pTCHAR�o�b�t�@���擾�B
TCHAR*  AkiGlobal::GetDummyString()
{
	m_dummy_next++;
	return m_dummybuf[ m_dummy_next%MAX_DUMMY_STRING ];
}


//�e���|����������z��̐����E�j��
TCHAR** AkiGlobal::CreateTemporaryStrBuff(UINT num,UINT max_len)
{
	return CreateStringArray(num,max_len);
}

void   AkiGlobal::DeleteTemporaryStrBuff(TCHAR** buf)
{
	DeleteStringArray(buf);
}


//�w��o�b�t�@�̐擪�ɁA�w�蕶���񂪓����Ă��邩�ǂ����`�F�b�N
BOOL AkiGlobal::strcheck(const TCHAR* buf,const TCHAR* str)
{
	int len = strlen(str);
	while(len>0){
		if(*buf!=*str)return FALSE;
		buf++;
		str++;
		len--;
	}
	return TRUE;
}

//2����TCHAR�z��m��
TCHAR** AkiGlobal::CreateStringArray(UINT num,UINT max_len)
{
	if(num==0 || max_len==0)return NULL;

	TCHAR** ret = new TCHAR* [num+1];
	ret[num]=NULL;

	for(UINT i=0;i<num;i++){
		ret[i] = new TCHAR[max_len];
	}

	return ret;
}

//2����TCHAR�z��j��
void   AkiGlobal::DeleteStringArray(TCHAR** pstrarr)
{
	if(!pstrarr)return;

	TCHAR** tmp = pstrarr;
	while(*tmp!=NULL){
		delete [] *tmp;
		tmp++;
	}

	delete [] pstrarr;
}



/*----------------------------------------------------------------
	�f�J��C���[�W�擾
------------------------------------------------------------------*/
MYSURFACE* AkiGlobal::GetBigFace(int cindex,int col,int alt)
{
	CBackgroundTaskBase *bgt = g_system.FindBGTask( BIGFACE_TASKID );
	CTBigFaceCache *bfc = dynamic_cast<CTBigFaceCache*>(bgt);

	if(!bfc)return NULL;
	return bfc->Get(cindex,col,alt);
}


/*----------------------------------------------------------------
	�u�����N�p�A�C�R���擾
------------------------------------------------------------------*/
MYSURFACE* AkiGlobal::GetBlankIcon()
{
	if(m_blankIcon==(MYSURFACE*)0xFFFFFFFF){//���łɈ�x�ǂݍ��݂Ɏ��s����
		return NULL;
	}

	if(!m_blankIcon){
		m_blankIcon = g_draw.CreateSurfaceFrom256Image(_T(".\\system\\blank_icon"));
		if(m_blankIcon==NULL){
			m_blankIcon=(MYSURFACE*)0xFFFFFFFF;
			return NULL;
		}
	}

	return m_blankIcon;
}

/*----------------------------------------------------------------
	���� 0�`1 �����_��
------------------------------------------------------------------*/
float AkiGlobal::RandomOne()
{
	UINT  div1 = 9999;
	float div2 = 9999.0f;
	return ((float)(rand()%div1))/div2 ;
}

/*----------------------------------------------------------------
	�t�@�C���ۂ��Ɠǂݍ���
------------------------------------------------------------------*/

AkiFile::AkiFile()
{
	m_buffer = NULL;
	m_size = 0;
}

void AkiFile::Destroy()
{
	DELETEARRAY(m_buffer);
	m_size = 0;
}

BOOL AkiFile::Load(TCHAR *filename)
{
	Destroy();
	if(!filename)return FALSE;

	//�I�[�v�\
	HANDLE hFile = CreateFile(filename,
		GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		return(FALSE);
	}

	//�t�@�C���T�C�Y�擾�E�������m��
	DWORD size = GetFileSize(hFile,NULL);
	if(size==0)return FALSE;
	m_buffer = new BYTE [size+1] ;

	//�ǂݍ���
	DWORD br;
	DWORD ret=ReadFile(hFile,m_buffer,size,&br,NULL);
	CloseHandle(hFile);
	if(!ret || br!=size)
	{
		delete [] m_buffer;
		return(FALSE);
	}

	m_buffer[size] = '\0';//������Ƃ��Ĉ����Ƃ��̂��߂̕֋X

	m_size = size;
	return TRUE;
}


BOOL AkiFile::Save(TCHAR *filename)
{
	if(!filename)return FALSE;
	if(!m_buffer)return FALSE;
	if(m_size==0)return FALSE;

	//�I�[�v�\
	HANDLE hFile = CreateFile(filename,
		GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		return(FALSE);
	}

	//��������
	DWORD br;
	DWORD ret=ReadFile(hFile,m_buffer,m_size,&br,NULL);
	CloseHandle(hFile);
	
	if(!ret)return FALSE;
	if(br != m_size)return FALSE;
	return TRUE;
}


void AkiFile::Set(DWORD size,BYTE* ptr)
{
	Destroy();

	m_size = size;
	m_buffer = new BYTE [ m_size ];

	if(ptr)
	{
		memcpy(m_buffer,ptr,m_size);
	}
}


/*----------------------------------------------------------------
	Output Debug String
------------------------------------------------------------------*/
void AkiGlobal::ods(const TCHAR *format, ...)
{
	va_list args;
	va_start(args, format);

	vsprintf(m_ods_buffer, format, args);
	OutputDebugString(m_ods_buffer);
	OutputDebugString(_T("\n"));

	g_system.Log(m_ods_buffer,SYSLOG_DEBUG);

	va_end(args);
}

void AkiGlobal::ods2(const TCHAR *format, ...)
{
	va_list args;
	va_start(args, format);

	vsprintf(m_ods_buffer, format, args);
	OutputDebugString(m_ods_buffer);
	
	g_system.Log(m_ods_buffer,SYSLOG_DEBUG);

	va_end(args);
}

