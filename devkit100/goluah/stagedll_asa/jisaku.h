
void InitializeJien();
void CleanupJien();

DWORD gMsgJien(DWORD msg, LPVOID pd, DWORD prm);

#define JISAKUID_PYON 1
#define JISAKUID_OUEN 2
#define JISAKUID_MITUME 3
#define JISAKUID_MITUME2 4

void act_jien_trace(GOBJECT *pdat);
