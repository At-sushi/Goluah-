
void DrawRECT(HWND hwnd,int sx,int gx,int sy,int gy,DWORD type);
void DrawRECT2(HDC  hdc ,int sx,int gx,int sy,int gy,DWORD type);

void DrawLINE(HWND hwnd,int sx,int gx,int sy,int gy,DWORD type);

void DrawPos(HDC hdc,int x,int y,BOOL not);

BOOL BitmapReflesh();

BOOL ChangeBmp(DWORD n);
BOOL ChangeBmp2(DWORD n);
BOOL ChooseFile(char *path,BOOL dat,BOOL fme=TRUE);

struct BMPINFO256
{
	BITMAPINFOHEADER hed;
	RGBQUAD pal[256];
};