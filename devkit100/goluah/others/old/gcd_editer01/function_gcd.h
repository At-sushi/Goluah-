
// load
#ifdef ___GCDEDITER
BOOL GCDLoadDlg(GCD_CELL2 *pcdat, GCD_RECT *prdat, GCD_HANTEI *phdat, GCD_CELLNAMES *pfhnames = NULL);
#endif

int GCDLoad(
    char *filename, GCD_CELL2 *pcdat, GCD_RECT *prdat, GCD_HANTEI *phdat, GCD_CELLNAMES *pfhnames = NULL);

#ifdef ___GCDEDITER
// save
BOOL GCDSaveDlg(GCD_CELL2 *pcdat, GCD_RECT *prdat, GCD_HANTEI *phdat, GCD_CELLNAMES *pfhnames = NULL);
int GCDSave(
    char *filename, GCD_CELL2 *pcdat, GCD_RECT *prdat, GCD_HANTEI *phdat, GCD_CELLNAMES *pfhnames = NULL);
BOOL GCDSaveHeader(GCD_CELLNAMES *pfhnames);

// other
void GCDSetInitialData(GCD_CELL2 *pcdat, GCD_RECT *prdat, GCD_HANTEI *phdat, GCD_CELLNAMES *pfhnames);

BOOL GCDIsEmptyCell2(GCD_CELL2 gcdc, GCD_HANTEI gcdh);
BOOL GCDIsEmptyRect(GCD_RECT gcdr);
void GCDSetEmptyCell2(GCD_CELL2 *pgcdc, GCD_HANTEI *pgcdh);
void GCDSetEmptyRect(GCD_RECT *pgcdr);

#endif
