
#define GCD_VERSION (700) // for ver0.7
#define GCDMAX_RECTANGLES (1024)
#define GCDMAX_CELLS (1024)
#define GCDMAX_IMAGES (12)

struct GCD_RECT {
  DWORD bmpno;
  RECT r;
  int center_x, center_y;
};

struct GCD_CELL {
  DWORD cdr;
  int dx;
  int dy;
  DWORD flag;
  float magx;
  float magy;
  int rot;
};

#define GCDCELL_REVERSE_X 0x00000001
#define GCDCELL_REVERSE_Y 0x00000002

struct GCD_CELL2 {
  GCD_CELL cell[8];
  int gcx;
  int gcy;
  DWORD flag;
};

#define GCDCELL2_LINK 0x00000001

struct GCD_HANTEI {
  RECT attack[3];
  RECT kas[3];
  RECT kurai[3];
};

struct GCD_FILEHEADER {
  DWORD size;
  DWORD ver;
};

struct GCD_CELLNAMES {
  char name[GCDMAX_CELLS][64];
};

//*********************************************************************
