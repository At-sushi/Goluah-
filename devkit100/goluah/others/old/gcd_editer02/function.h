
void DrawRECT(HWND hwnd,int sx,int gx,int sy,int gy,DWORD type);
void DrawRECT2(HDC  hdc ,int sx,int gx,int sy,int gy,DWORD type);

void DrawLINE(HWND hwnd,int sx,int gx,int sy,int gy,DWORD type);

BOOL BitmapReflesh();

struct BMPINFO256
{
	BITMAPINFOHEADER hed;
	RGBQUAD pal[256];
};

void DrawBlueText(HDC hdc,RECT r,char *text,int len,DWORD method,int hh);

void ShowDatNow();
