
// �r�b�g�}�b�v�p�\���� ********************************************
/*#define MYSUF_MAXNUMTEX	(16)
struct MYSURFACE
{
	BOOL valid;
	DWORD xsufsize[MYSUF_MAXNUMTEX];//�e�N�X�`���̕��̔z��
	DWORD ysufsize[MYSUF_MAXNUMTEX];//�e�N�X�`���̍����̔z��
	DWORD xsufindx[MYSUF_MAXNUMTEX];//MYSURFACE���ł�left���W
	DWORD ysufindx[MYSUF_MAXNUMTEX];//MYSURFACE���ł�top���W
	DWORD xsufnum;
	DWORD ysufnum;
	LPDIRECT3DTEXTURE9 *pTex;		//�e�N�X�`���z��Bd3d8.h��g�ݍ��܂Ȃ��ꍇ��LPVOID�^�ɁB
	float w;						//�r�b�g�}�b�v�̕�
	float h;						//�r�b�g�}�b�v�̍���
	float w2;						//�r�b�g�}�b�v�̕��i320x240���[�h�ł��ω������j
	float h2;						//�r�b�g�}�b�v�̍����i320x240���[�h�ł��ω������j
};

struct MYRECT3D
{
	float top;
	float bottom;
	float left;
	float right;
	float z;
};
*/
// �L�����N�^�[�֌W�̍\���� ***************************************

#define MAXNUM_RECTANGLES	(1024)
#define MAXNUM_CELLS		(1024)

struct CHARDAT_RECT
{
	DWORD bmpno;	//�g�p����r�b�g�}�b�v�̔ԍ�(0����)
	RECT r;			//�؂����`
};

struct CHARDAT_CELL
{
	DWORD cdr;		//CHARDAT_RECT�\���̂̔ԍ�
	int dx;			//���炵��x
	int dy;			//���炵��y
	DWORD flag;		//���t���O
	double magx;	//�g�嗦x
	double magy;	//�g�嗦y
	DWORD res;		//�H�Y�ꂽ
};

#define CHARDAT_CELL_REVERSE_X	0x00000001//x���]
#define CHARDAT_CELL_REVERSE_Y	0x00000002//y���]

struct CHARDAT_CELL2
{
	RECT attack;			//�U����`
	RECT kas;				//�d�Ȃ��`
	RECT kurai;				//���炢��`
	CHARDAT_CELL cell[8];	//CHARDAT_CELL�z��
};

struct CHARACTERINFOS
{
	MYSURFACE* dds[12];						//�ǂݍ��񂾃r�b�g�}�b�v
	CHARDAT_RECT  rdat[MAXNUM_RECTANGLES];	//
	CHARDAT_CELL2 cdat[MAXNUM_CELLS];		//
	BOOL iscomputerplayer;					//�R���s���[�^���ǂ���
};



















// �ȉ��L�����N�^�E�X�e�[�W�̐���Ƃ͂���܂�֌W�Ȃ� *******************************************************
struct CHARDAT_FILEHEADER2//�t�@�C���擪
{
	DWORD size;			//�\���̃T�C�Y
	DWORD num_rects;	//�����Ă���CHARDAT_RECT�\���̂̐�
	DWORD num_cells;	//�����Ă���CHARDAT_CELL2�\���̂̐�
	DWORD flag;			//�Y�ꂽ
};

struct CHARDAT_FILEHEADER//��Ԗ�(�Ȃ����Ƃ�����)
{
//	char path[256];
//	char filename[64];
	char name[MAXNUM_CELLS][64];//�Z���ɂ������O
};

#define MAX_IMAGES	12

// ���t�H�[�}�b�g ******************************************
struct DATOLD_FILEHEADER
{
	char path[4][256];
	char name[MAXNUM_CELLS][64];
};

struct DATOLD_CELL
{
	DWORD cdr[3];
	int dx[3];
	int dy[3];
	DWORD flag[3];

	RECT attack[3];
	RECT kas[3];
	RECT kurai[3];
};


