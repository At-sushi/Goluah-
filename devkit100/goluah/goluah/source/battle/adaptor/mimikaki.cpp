
#include "StdAfx.h"

#include "define_const.h"
#include "define_macro.h"
#include "global.h"
#include "task_loading.h"

#include "define_gcd.h"
#include "gobject.h"
#include "define_data.h"
#include "define_char.h"
#include "mimikaki.h"
#include "adaptor.h"

extern TCHAR *g_chardir;
extern DI_FUNCTIONS_S *funcs;
extern DI_FUNCTIONS_O *funco;
extern DI_FUNCTIONS_D *funcd;

// LPVOID g_mysounds[MAXMYSOUND];		// メンバ領域に移動

//****************************************************************
//  情報交換関数
//****************************************************************

void Exchange(GOBJECT* pNew, CHARACTERINFO2* pOld, BOOL isOldToNew)
{
	if (isOldToNew)
	{
		// 旧バージョンから新バージョンへ
		if (pOld->aid == OLDACTID_NEUTRAL)
			pNew->aid = ACTID_NEUTRAL;
		else if (pOld->aid == OLDACTID_TOJYO)
			pNew->aid = ACTID_TOJYO;
		else if (pOld->aid == OLDACTID_SYORI)
			pNew->aid = ACTID_SYORI;
		else
			pNew->aid = pOld->aid;

		pNew->cnow		= pOld->cnow;
		pNew->counter	= pOld->counter;
		pNew->x			= pOld->x;
		pNew->y			= pOld->y;
		pNew->vx		= pOld->vx;
		pNew->vy		= pOld->vy;
		pNew->muki		= pOld->muki;
		pNew->revx		= pOld->revx;
		pNew->revy		= pOld->revy;
		pNew->muteki	= pOld->muteki;
		pNew->nagerare	= pOld->nagerare;
		pNew->hp		= pOld->hp;
	}
	else
	{
		// 新バージョンから旧バージョンへ
		if (pNew->aid == ACTID_NEUTRAL)
			pOld->aid = OLDACTID_NEUTRAL;
		else if (pNew->aid == ACTID_TOJYO)
			pOld->aid = OLDACTID_TOJYO;
		else if (pNew->aid == ACTID_SYORI)
			pOld->aid = OLDACTID_SYORI;
		else
			pOld->aid = pNew->aid;

		pOld->cnow		= pNew->cnow;
		pOld->counter	= pNew->counter;
		pOld->x			= pNew->x;
		pOld->y			= pNew->y;
		pOld->vx		= pNew->vx;
		pOld->vy		= pNew->vy;
		pOld->muki		= pNew->muki;
		pOld->revx		= pNew->revx;
		pOld->revy		= pNew->revy;
		pOld->muteki	= pNew->muteki;
		pOld->nagerare	= pNew->nagerare;
		pOld->hp		= pNew->hp;
		pOld->gauge		= pNew->gauge;
	}
}

//****************************************************************
//  いろいろ
//****************************************************************

void Furimuki(DWORD oid)
{
	GOBJECT *pdat  = (GOBJECT*)(*funco->getinfo)(oid);
	GOBJECT *pedat = (GOBJECT*)(*funco->getinfo)(pdat->eid);

	if(pdat==NULL)return;
	if(pedat==NULL)return;

	if(pdat->x > pedat->x )
		pdat->muki = TRUE;
	else
		pdat->muki = FALSE;
}

void CCharAdaptor::InitializeMySound()
{
//	static BOOL isfirst=TRUE;
	int i;
//	if(isfirst){
		for(i=0;i<MAXMYSOUND;i++){
			m_mysounds[i]=NULL;
		}
//	}
//	isfirst=FALSE;

	TCHAR filename[256];
	for(i=0;i<MAXMYSOUND;i++){
		if(m_mysounds[i]==NULL){
			_stprintf(filename,_T("%s\\sound%d.wav"),g_chardir,i);
			m_mysounds[i] = (*funcs->loadmysound)(filename);
		}
	}
}

void CCharAdaptor::PlayMySound(DWORD soundno)
{
	if(m_mysounds[soundno]!=NULL){
		(*funcs->playmysound)(m_mysounds[soundno]);
	}
}

void CCharAdaptor::UnloadMySound()
{
	for(int i=0;i<MAXMYSOUND;i++){
		if(m_mysounds[i]!=NULL){
			(*funcs->killmysound)(m_mysounds[i]);
			m_mysounds[i]=NULL;
		}
	}
}

// pdat側で解放してるからこっちは大丈夫？
// やっぱダメ
/* MYSURFACE* g_mysurface[2][12];
GCD_CELL2 g_cdat[2][GCDMAX_CELLS];
GCD_RECT g_rdat[2][GCDMAX_RECTANGLES];
GCD_HANTEI g_hdat[2][GCDMAX_CELLS];		*/

void CCharAdaptor::InitializeGCDandBMP(GOBJECT *pdat,DWORD color)
{
	int i,j;
	HANDLE hFile;
	DWORD br;
	CHARDAT_FILEHEADER2 cfh;
//	CHARDAT_FILEHEADER* muda;
	CHARDAT_RECT* crect = NULL;
	CHARDAT_CELL2* ccell = NULL;

	if(pdat==NULL)return;
	j = pdat->tid;

	TCHAR filename[256],palname[256];
	_stprintf(palname,_T("%s\\pal%d.bmp"),m_chardir,color);
	for(i=0;i<12;i++){
		_stprintf(filename,_T("%s\\image%d.bmp"),m_chardir,i+1);
		m_mysurface[j][i] = (MYSURFACE*) ( (*funcd->loadbmp)(filename,palname) );//image1-12.bmp
	}
	// pdatに入れる
	pdat->pmsarr		= m_mysurface[j];

	_stprintf(filename,_T("%s\\cell.gcd"),m_chardir);
	hFile = CreateFile(filename,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE) return;
	if(!ReadFile(hFile,&cfh,sizeof(cfh),&br,NULL)){CloseHandle(hFile);return;}
	if (br < sizeof(cfh)) {
		pdat->phdat = NULL;
		pdat->pcdat = NULL;
		pdat->prdat = NULL;
		CloseHandle(hFile);
		return;
	}
	if(cfh.size == sizeof(GCD_FILEHEADER)) { // 最新バージョン
		CloseHandle(hFile);
		(*funcd->create_celldat)(filename,(LPVOID*)&pdat->pcdat,(LPVOID*)&pdat->prdat,(LPVOID*)&pdat->phdat);//cell.gcd
	}
	else {
		if (cfh.flag & 0x0001) { // ヘッダあり
			// ファイルヘッダ部分を飛ばす
			SetFilePointer(hFile, sizeof(CHARDAT_FILEHEADER), NULL, FILE_CURRENT);
		}
		crect = new CHARDAT_RECT[cfh.num_rects];
		if(!ReadFile(hFile,crect,sizeof(CHARDAT_RECT) * cfh.num_rects,&br,NULL)){CloseHandle(hFile);return;}
		if (br < sizeof(CHARDAT_RECT) * cfh.num_rects) {
			pdat->phdat = NULL;
			pdat->pcdat = NULL;
			pdat->prdat = NULL;
			CloseHandle(hFile);
			delete [] crect;
			return;
		}
		ZeroMemory(m_rdat[j], sizeof(m_rdat[j]));
		for (i = 0; i < (int)cfh.num_rects; i++) {
			m_rdat[j][i].bmpno = crect[i].bmpno;
			m_rdat[j][i].r = crect[i].r;
			m_rdat[j][i].center_x = 0;
			m_rdat[j][i].center_y = 0;
		}
		delete [] crect;
		ccell = new CHARDAT_CELL2[cfh.num_cells];
		if(!ReadFile(hFile,ccell,sizeof(CHARDAT_CELL2) * cfh.num_cells,&br,NULL)){CloseHandle(hFile);return;}
		if (br < sizeof(CHARDAT_CELL2) * cfh.num_cells) {
			pdat->phdat = NULL;
			pdat->pcdat = NULL;
			pdat->prdat = m_rdat[j];
			CloseHandle(hFile);
			delete [] ccell;
			return;
		}
		ZeroMemory(m_cdat[j], sizeof(m_cdat[j]));
		ZeroMemory(m_hdat[j], sizeof(m_hdat[j]));
		for (i = 0; i < (int)cfh.num_cells; i++) {
			for (int k = 0; k < 8; k++) {
				m_cdat[j][i].cell[k].cdr = ccell[i].cell[k].cdr;
				m_cdat[j][i].cell[k].dx = ccell[i].cell[k].dx;
				m_cdat[j][i].cell[k].dy = ccell[i].cell[k].dy;
				m_cdat[j][i].cell[k].flag = ccell[i].cell[k].flag;
				m_cdat[j][i].cell[k].magx = (float)ccell[i].cell[k].magx;
				if (m_cdat[j][i].cell[k].magx == 0.0)
					m_cdat[j][i].cell[k].magx = 1.0;
				m_cdat[j][i].cell[k].magy = (float)ccell[i].cell[k].magy;
				if (m_cdat[j][i].cell[k].magy == 0.0)
					m_cdat[j][i].cell[k].magy = 1.0;
				m_cdat[j][i].cell[k].rot = 0;

				// 最新版増加分
				m_cdat[j][i].cell[k].red_ratio = 1;
				m_cdat[j][i].cell[k].green_ratio = 1;
				m_cdat[j][i].cell[k].blue_ratio = 1;
				m_cdat[j][i].cell[k].alpha_ratio = 1;
			}
			m_cdat[j][i].flag = 0;
			m_cdat[j][i].gcx = 0;
			m_cdat[j][i].gcy = 0;
			m_hdat[j][i].attack[0] = ccell[i].attack;
			m_hdat[j][i].kas[0] = ccell[i].kas;
			m_hdat[j][i].kurai[0] = ccell[i].kurai;
		}
		delete [] ccell;
		CloseHandle(hFile);

		// 最後の締め
		m_cdat[j][0].cell[0].flag = GCD_VERSION;

		//pdatに入れておく
		pdat->phdat			= m_hdat[j];
		pdat->pcdat			= m_cdat[j];
		pdat->prdat			= m_rdat[j];

		//NowLoadingを進める
		CTNowLoading* task = dynamic_cast<CTNowLoading*>( g_system.FindTask('LOAD') );
		if(task)task->Proceed(NowLoading_GCD);
	}
}

void CCharAdaptor::UnloadMyBitmaps(GOBJECT *pdat)
{
	if(pdat==NULL)return;
	if(pdat->pmsarr==NULL)return;
	for(int i=0;i<12;i++){
		(*funcd->unloadbmp)(pdat->pmsarr[i]);
	}

	// 管轄外だけどセルの解放もここで。（最新版のcellのみ）
	if ((pdat->prdat != NULL && pdat->prdat != m_rdat[pdat->tid]) ||
		(pdat->pcdat != NULL && pdat->pcdat != m_cdat[pdat->tid]) ||
		(pdat->phdat != NULL && pdat->phdat != m_hdat[pdat->tid])   )
			funcd->destroy_celldat((LPVOID*)&pdat->pcdat, (LPVOID*)&pdat->prdat, (LPVOID*)&pdat->phdat);
}

//こぴーぺ
int GetRandNum(int num)//ランダムナンバー発生
{
	srand(timeGetTime());
	return(rand()%num);
}

BOOL GetWinSerif(TCHAR *getmsg,TCHAR *enename,TCHAR *filepath)
{
//	BY_HANDLE_FILE_INFORMATION FileInformation;
	_stprintf(getmsg,_T("(台詞の設定が検索されませんでした)"));

	//ファイルのオープン
	HANDLE hFile = CreateFile(filepath,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE)return(FALSE);
//	if (GetFileInformationByHandle(hFile, &FileInformation) &&
//		(FileInformation.ftLastAccessTime.dwLowDateTime > timeGetTime()))
	//領域確保
	DWORD fsiz = GetFileSize(hFile,NULL);
	TCHAR *buf = (TCHAR*)malloc(fsiz);
	ZeroMemory(buf,sizeof(buf));

	//ファイルの読み込み
	DWORD br;
	ReadFile(hFile,buf,fsiz,&br,NULL);
	CloseHandle(hFile);

	//そのキャラクターに割り振られた台詞がいくつあるかを検索
	DWORD numser=0,numserr=0,pnow=0;
	BOOL loop=TRUE,loop2;
	TCHAR tmpname[64];
	DWORD tmpnamelen;
	DWORD sstartpos[16],sstartposr[16];
	while(pnow < fsiz && loop){
		//「#」を検索
		loop2=TRUE;
		while(pnow < fsiz && loop2){
			//行頭（またはファイル先頭）のみ
			if(pnow==0){
				if(buf[pnow]=='#'){
					loop2=FALSE;
				}
			}
			else if(pnow>1){
				if(buf[pnow-2]==0x0d && buf[pnow-1]==0x0a){
					if(buf[pnow]=='#'){
						loop2=FALSE;
					}
				}
			}
			pnow++;
		}
		if(!loop2){//#が発見されているならば
			//その後の改行までを名前として記録
			tmpnamelen=0;
			ZeroMemory(tmpname,sizeof(tmpname));
			loop2=TRUE;
			while(pnow < fsiz && loop2){
				tmpname[tmpnamelen] = buf[pnow];
				tmpnamelen++;
				if(pnow < fsiz-2){//改行判定
					if(buf[pnow+1]==0x0d && buf[pnow+2]==0x0a){//その次は改行だった
						loop2=FALSE;
						pnow+=3;
					}
					else pnow++;//その次も改行ではない
				}
				else pnow++;//ファイルの終わりのほうなので改行判定ができない
			}
			//名前を比較
			if(strcmp(tmpname,_T("random"))==0 && numserr<16){//ランダム台詞
				sstartposr[numserr]=pnow;
				numserr++;
			}
			if(strcmp(tmpname,enename)==0 && numser<16){//キャラ固有台詞
				sstartpos[numser]=pnow;
				numser++;
			}
		}
	}

	if(numser==0 && numserr==0)return(FALSE);//一つも台詞が見つからなかった

	//見つかった台詞のうち、ランダムに1つの台詞を選択
	if(numser != 0)pnow = sstartpos[GetRandNum(numser)];
	else pnow = sstartposr[GetRandNum(numserr)];//ランダム台詞


	//その後の改行までを勝ち台詞としてコピー
	int tmpcnt=0;
	DWORD retlen=0;
	while(pnow < fsiz){
		getmsg[retlen] = buf[pnow];
		retlen++;
		if(pnow < fsiz-2){//改行判定
			if(buf[pnow+1]==0x0d && buf[pnow+2]==0x0a){//その次は改行だった
				if(pnow == fsiz-2){//ここの判定を追加
					getmsg[retlen] = 0;
					free(buf);
					return(TRUE);
				}//ここまで
				if(buf[pnow+3]=='#' || tmpcnt>=2){//メッセージに有効な改行か？
					getmsg[retlen] = 0;
					free(buf);
					return(TRUE);
				}
				else{//改行は２個までバッファする
					getmsg[retlen++] = buf[++pnow];
					getmsg[retlen++] = buf[++pnow];
					tmpcnt++;
				}
			}
		}
		if(pnow < fsiz-1){
			if(buf[pnow+1]=='#'){
				getmsg[retlen] = 0;
				free(buf);
				return(TRUE);
			}
		}
		pnow++;
	}
	getmsg[retlen] = 0;
	free(buf);
	return(TRUE);

}

void LoadAndSetKatiSerif(DWORD tid)//テキストから勝利時のセリフを読み込んで設定する
{
	TCHAR katiserifu[256];
	TCHAR *enemyname;
	TCHAR filepath[256];

	ZeroMemory(katiserifu,sizeof(katiserifu));
	enemyname = funcs->getenemyname(tid);
	_stprintf(filepath,_T("%s\\serifu.txt"),g_chardir);

	if(GetWinSerif(katiserifu,enemyname,filepath)){
		funcs->setkatiserif(tid,katiserifu);
	}
//	else funcs->setkatiserif(tid,_T("セリフの取得に失敗"));
}

void DrawFront(GOBJECT *pdat,BOOL fr)
{
	if (fr)
		pdat->z = ZZAHYO_CHARACTER_F;
	else
		pdat->z = ZZAHYO_CHARACTER_B;
}

void AddPowerGauge(GOBJECT *pdat,double plus)
{
	pdat->gauge += plus;
	if(pdat->gauge > pdat->gaugemax)
		pdat->gauge = pdat->gaugemax;
	if(pdat->gauge < 0)
		pdat->gauge = 0;
}

#define MAXNUM_MYATTACKINFO 50
ATTACKINFO my_ainfo[MAXNUM_MYATTACKINFO];
DWORD mai_index=0;

void StartAttack(DWORD oid,MYATTACKINFO *ainfo)
{
	GOBJECT *pdat = (GOBJECT*)(*funco->getinfo)(oid);
	if(pdat==NULL)return;

	ATTACKINFO *ainfo2 = ResisterAttackInfo(oid,ainfo);

	pdat->atk = ainfo2;
	pdat->kougeki = TRUE;
}

ATTACKINFO* ResisterAttackInfo(DWORD oid,MYATTACKINFO *mainfo)
{
	CCharAdaptor* pCharacter;
	DWORD index = mai_index;
	mai_index++;
	if(mai_index>=MAXNUM_MYATTACKINFO)mai_index=0;

	my_ainfo[index].damage	= mainfo->damage;
	my_ainfo[index].kezuri	= mainfo->kezuri;
	my_ainfo[index].hit		= mainfo->hit;
	my_ainfo[index].guard	= mainfo->guard;
	my_ainfo[index].id		= mainfo->id;
	my_ainfo[index].muki	= mainfo->muki;

	GOBJECT *pdat = (GOBJECT*)(*funco->getinfo)(oid);
	if(pdat==NULL)return(&my_ainfo[index]);
	if(pdat->muki != my_ainfo[index].muki)my_ainfo[index].muki = TRUE;
	else my_ainfo[index].muki = FALSE;

	pCharacter = (CCharAdaptor*)pdat->pobjdat_u;
	if (pCharacter == NULL)
		return &my_ainfo[index];
	pCharacter->pg_hit		= mainfo->pg_hit;
	pCharacter->pg_guard	= mainfo->pg_guard;

	return(&my_ainfo[index]);
}

// ATTACKINFO構造体を旧式に変換
void GetMyAttackInfo(DWORD oid, const ATTACKINFO *ainfo,
							 MYATTACKINFO* mainfo, BOOL IsMyObject)
{
	GOBJECT* pdat;

	if (!mainfo || !ainfo)
		return;

	mainfo->id		= ainfo->id;
	mainfo->damage	= ainfo->damage;
	mainfo->hit		= ainfo->hit;
	mainfo->guard	= ainfo->guard;
	mainfo->kezuri	= ainfo->kezuri;
	mainfo->dur		= 0;

	pdat = (GOBJECT*)funco->getinfo(oid);
	if (pdat)
	{
		if (ainfo->muki == FALSE)
			mainfo->muki = pdat->muki;
		else
			mainfo->muki = !pdat->muki;

		if (IsMyObject)
		{
			CCharAdaptor* pCharacter = (CCharAdaptor*)pdat->pobjdat_u;

			if (pCharacter)
			{
				mainfo->pg_hit = pCharacter->pg_hit;
				mainfo->pg_guard = pCharacter->pg_guard;
			}
			else
				mainfo->pg_hit = mainfo->pg_guard = 0.0;
		}
		else
			mainfo->pg_hit = mainfo->pg_guard = 0.0;
	}
	else
	{
		mainfo->muki = FALSE;
		mainfo->pg_hit = mainfo->pg_guard = 0.0;
	}
}

//****************************************************************
//  簡易飛び道具
//****************************************************************
void GoBulletA(GOBJECT *pdat,BULLETINFO_A *bulinfo)
{
	DWORD oid = (*funco->objcreate)();
	GOBJECT *pbdat = (GOBJECT*)(*funco->getinfo)(oid);
	STRUCT_BULLETA *pdatu = (STRUCT_BULLETA*)malloc(sizeof(STRUCT_BULLETA));

	if(pdatu==NULL)return;
	if(pbdat==NULL) { free(pdatu); return; }

	pdatu->pbinfo_a	= bulinfo;

	pbdat->tid = pdat->tid;
	pbdat->uid = bulinfo->type;
	if(pbdat->uid & BULLETA_VSHUMAN)
		pbdat->objtype |= GOBJFLG_ATTACK;
	if(pbdat->uid & BULLETA_VSBULLET)
		pbdat->objtype |= GOBJFLG_ZBULLET;
	if(pbdat->uid & BULLETA_DISPZAHYO)
		pbdat->objtype |= GOBJFLG_DISPZAHYO;
	if(pbdat->uid & BULLETA_DONOTSTOP)
		pbdat->objtype |= GOBJFLG_DONOTSTOP;
	if(pbdat->uid & BULLETA_JIMAESYORI)
	{
		pbdat->msghandler = JimaeBulletACallBack;
		pdatu->ObjectID = 0xFFFF;
	}
	else
	{
		pbdat->msghandler = BulletACallBack;
		bulinfo->counter = 0;
		pdatu->cell_dis = bulinfo->cell_dis;
		pdatu->cell_run = bulinfo->cell_run;
		pdatu->num_run	= 0;
		while(pdatu->cell_run[pdatu->num_run]!=-1){
			pdatu->num_run++;
		}
		pdatu->num_dis	= 0;
		while(pdatu->cell_dis[pdatu->num_dis]!=-1){
			pdatu->num_dis++;
		}
		pdatu->pbinfo_a->state = 1;
	}
	pbdat->cnow = 0;
	pbdat->counter = 0;
	pbdat->aid = BULLETASTATE_RUN;
	pbdat->atk = ResisterAttackInfo(pdat->id,&(bulinfo->atk));
	pbdat->phdat = pdat->phdat;

	pbdat->pcdat = pdat->pcdat;
	pbdat->prdat = pdat->prdat;
	pbdat->pmsarr = pdat->pmsarr;

	pdatu->spd_dis  = bulinfo->spd_dis+1;
	pdatu->spd_run  = bulinfo->spd_run+1;
	pdatu->dur		= bulinfo->dur;
	pdatu->poid		= pdat->id;
	pdatu->flags	= bulinfo->type;
	pbdat->pobjdat_u = pdatu;
}

DWORD BulletACallBack(DWORD msg,LPVOID dat,DWORD prm)
{
	GOBJECT *pdat = (GOBJECT*)dat;
	if(pdat==NULL)return(0);
	STRUCT_BULLETA *pbdat = (STRUCT_BULLETA*)pdat->pobjdat_u;
	if(pbdat==NULL)return(0);
	BULLETINFO_A* pbinfo_a = pbdat->pbinfo_a;
	if(pbinfo_a==NULL)return(0);

	if (msg != GOBJMSG_DELETE)
	{
		pdat->muki = pbinfo_a->muki;
		pdat->x = pbinfo_a->x;
		pdat->y = pbinfo_a->y;
		pdat->ax = pbinfo_a->ax;
		pdat->ay = pbinfo_a->ay;
		pdat->vx = pbinfo_a->vx;
		pdat->vy = pbinfo_a->vy;
	}

	switch(msg){
	case GOBJMSG_ACTION://アクショソ
		switch(pdat->aid){
		case BULLETASTATE_RUN:
			pdat->kougeki=TRUE;//攻撃を行う
		case BULLETASTATE_RUN2:
			if(pdat->aid==BULLETASTATE_RUN2)pdat->kougeki=FALSE;//攻撃を行わない
			//前に進む
			if(pdat->muki)pdat->x -= pdat->vx;
			else pdat->x += pdat->vx;
			pdat->y += pdat->vy;
			//加速する
			pdat->vx += pdat->ax;
			pdat->vy += pdat->ay;
			//表示セルを変更する
			if(pbdat->spd_run !=0  &&  pbdat->num_run!=0)
				pdat->cnow = pbdat->cell_run[ (pdat->counter / pbdat->spd_run)%pbdat->num_run ];
			//持続時間
			if(pdat->counter > (DWORD)pbdat->dur){
				pdat->aid = BULLETASTATE_DIS;
			}
			//画面外
			if(pdat->uid & BULLETA_XGAMENGAI && (pdat->x > 700 || pdat->x < -700)){
				pdat->aid = BULLETASTATE_DIS;
			}
			//地面に到達
			if(pdat->uid & BULLETA_XJIMENN){
				if(pdat->y > 0){
					pdat->aid = BULLETASTATE_DIS;
				}
			}
			break;
		case BULLETASTATE_DIS:
			pdat->kougeki=FALSE;//攻撃を行わない
			//表示セルを変更する
			if(pbdat->spd_dis !=0  &&  pbdat->num_dis!=0)
				pdat->cnow = pbdat->cell_dis[ (pdat->counter / pbdat->spd_dis)%pbdat->num_dis ];
			//完全消滅
			if(pdat->counter / pbdat->spd_dis > (DWORD)pbdat->num_dis){
				(*funco->suicide)(pdat->id);
			}
			break;
		default:
			pdat->aid = BULLETASTATE_DIS;
		}
		pbinfo_a->counter++;
		//手動操作による消去
		if(pbinfo_a->state == 0){
			funco->suicide(pdat->id);
		}
		//z座標
		pdat->z = ZZAHYO_BULLET4;
		if(pbdat->flags & BULLETA_DRAWBACK)pdat->z = ZZAHYO_BULLET2;
		if(pbdat->flags & BULLETA_DRAWMIDDLE)pdat->z = ZZAHYO_BULLET3;
		break;
	case GOBJMSG_SOUSAI://他の飛び道具と当たった
		if(pdat->uid & BULLETA_DONOTDIE)
			pdat->aid = BULLETASTATE_RUN2;
		else
			pdat->aid = BULLETASTATE_DIS;
		pdat->kougeki=FALSE;//攻撃を行わない
		break;
	case GOBJMSG_TOUCHB://敵に当たった
	{
		GOBJECT* ppdat = (GOBJECT*)(*funco->getinfo)(pbdat->poid);

		if(ppdat!=NULL){
			if(ppdat->msghandler!=NULL){
				BULLETATKINFO BulAtkInfo;

				BulAtkInfo.pbul = pdat;
				BulAtkInfo.hit = prm;
				(*ppdat->msghandler)(MYGOBJMSG_BULLETHIT,ppdat, (DWORD)&BulAtkInfo);//やったー当たったよー
			}
		}
		if(pdat->uid & BULLETA_DONOTDIE)
			pdat->aid = BULLETASTATE_RUN2;
		else
			pdat->aid = BULLETASTATE_DIS;
		pdat->kougeki=FALSE;//攻撃を行わない
		break;
	}
	case GOBJMSG_DELETE://消滅
		if (funco->getinfo(pbdat->poid) != NULL)
			pbdat->pbinfo_a->state = 0;
		free(pdat->pobjdat_u);
		return(TRUE);
	case GOBJMSG_DRAW://描画はシステムに任せる
	default:
		return(0);
	}

	pbinfo_a->muki = pdat->muki;
	pbinfo_a->x = pdat->x;
	pbinfo_a->y = pdat->y;
	pbinfo_a->ax = pdat->ax;
	pbinfo_a->ay = pdat->ay;
	pbinfo_a->vx = pdat->vx;
	pbinfo_a->vy = pdat->vy;

	return(TRUE);
}

DWORD ObjID[256] = {0};

DWORD JimaeBulletACallBack(DWORD msg,LPVOID dat,DWORD prm)
{
	GOBJECT *pdat = (GOBJECT*)dat;
	if(pdat==NULL)return(0);
	STRUCT_BULLETA *pbdat = (STRUCT_BULLETA*)pdat->pobjdat_u;
	if(pbdat==NULL)return(0);
	BULLETINFO_A* pbinfo_a = pbdat->pbinfo_a;
	if(pbinfo_a==NULL)return(0);
	BULLETINFO_B* pbinfo_b = pbdat->pbinfo_a->info_b;
	if(pbinfo_b==NULL)return(0);

	switch (msg)
	{
	case GOBJMSG_ACTION:
		if (pdat->counter == 0)
		{
			pdat->kougeki = TRUE;
			for (WORD i = 0; i < 256; i++)
				if (ObjID[i] == 0)
				{
					pbdat->ObjectID = i;
					ObjID[i] = pdat->id;
					break;
				}

			pbinfo_a->state &= 0xFFFF;
			pbinfo_a->state |= (pbdat->ObjectID * 0x00010000);
		}

		if (pbinfo_b->pf_action)
			pbinfo_b->pf_action(pbinfo_a);
		break;
	case GOBJMSG_SOUSAI://他の飛び道具と当たった
		if (pbinfo_b->pf_hit && pbinfo_b->pf_hit(pbinfo_a))
			funco->suicide(pdat->id);
		pdat->kougeki=FALSE;//攻撃を行わない
		break;
	case GOBJMSG_TOUCHB:
	{
		GOBJECT* ppdat = (GOBJECT*)(*funco->getinfo)(pbdat->poid);

		if(ppdat!=NULL){
			if(ppdat->msghandler!=NULL){
				BULLETATKINFO BulAtkInfo;

				BulAtkInfo.pbul = pdat;
				BulAtkInfo.hit = prm;
				(*ppdat->msghandler)(MYGOBJMSG_BULLETHIT,ppdat, (DWORD)&BulAtkInfo);//やったー当たったよー
			}
		}
		if (pbinfo_b->pf_hit && pbinfo_b->pf_hit(pbinfo_a))
			funco->suicide(pdat->id);
		pdat->kougeki=FALSE;//攻撃を行わない
		break;
	}
	case GOBJMSG_DELETE://消滅
		if (pbdat->ObjectID < 256)
			ObjID[pbdat->ObjectID] = 0;
		free(pdat->pobjdat_u);
		return(TRUE);
	case GOBJMSG_DRAW://描画はシステムに任せる
	default:
		return(0);
	}

	pbinfo_a->state &= 0xFFFF;
	pbinfo_a->state |= (pbdat->ObjectID * 0x00010000);

	pdat->muki = pbinfo_a->muki;
	pdat->x = pbinfo_a->x;
	pdat->y = pbinfo_a->y;
	pdat->ax = pbinfo_a->ax;
	pdat->ay = pbinfo_a->ay;
	pdat->vx = pbinfo_a->vx;
	pdat->vy = pbinfo_a->vy;
	pdat->cnow = pbinfo_a->cnow;

	return TRUE;
}
