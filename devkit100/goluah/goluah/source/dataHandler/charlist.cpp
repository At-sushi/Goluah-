
/*========================================================================

	�L�����N�^�[�Ǘ�

	�f�B���N�g������L�����N�^�[�̃��X�g���쐬���ĊǗ�����B

==========================================================================*/

#include "stdafx.h"

#include "define_const.h"//needs GOLUAH_SYSTEM_VERSION
#include "define_macro.h"//needs foreach
#include "global.h"	//needs g_system
#include "charlist.h"


/*-------------------------------------------------------------------------
	�\�z
---------------------------------------------------------------------------*/
CCharacterList::CCharacterList()
{
}


/*-------------------------------------------------------------------------
	�j��
---------------------------------------------------------------------------*/
void CCharacterList::Destroy()
{
	infolist.clear();
	damelist.clear();
	ringlist.clear();
}


/*-------------------------------------------------------------------------

	������

	1) "�����O" �ɑ�������f�B���N�g���̈ꗗ�𐶐�
	2) �e�f�B���N�g���Ɋւ��ăL�����N�^�[������
		2.5) ���������e�L�����N�^�[�f�B���N�g���̐�����������
	3) �L�����N�^��1�̂�������Ȃ����������O�̓��X�g����폜
	4) Favorite�I�v�V�����̕s���𐳂�

---------------------------------------------------------------------------*/
void CCharacterList::Initialize()
{
	Destroy();

	InitializeRingList();// 1)
	if(ringlist.size()==0)return;

	DWORD index = 0;
	foreach(ringlist,CCLRingInfoList,ii){ 
		InitializeRing(index); // 2) , 2.5)
		index++;
	}

	//3)
	CCLRingInfoList::iterator ite = ringlist.begin();
	CCLRingInfoList::iterator itee= ringlist.end();
	while (ite!=itee){
		if(ite->ring2serialIndex.size() == 0){
			ite = ringlist.erase(ite);
			itee= ringlist.end();
		}
		else
			ite++;
	}

	//4)
	UINT i,j;
	for(i=0;i<infolist.size();i++){
		for(j=0;j<infolist[i].fav_opts.size();j++)
		{
			CorrectOption(i,&infolist[i].fav_opts[j].opt);
		}
	}
}

//�����O�f�B���N�g���ꗗ�̐���
void CCharacterList::InitializeRingList()
{
	HANDLE hFind;
	WIN32_FIND_DATA fd;

	hFind = FindFirstFile(".\\char\\*.*", &fd);
	CCL_RINGINFO ringitem;

	if(hFind != INVALID_HANDLE_VALUE) {//�f�B���N�g�������݂���ꍇ
		do {
			if(strcmp(fd.cFileName,".")==0 || strcmp(fd.cFileName,"..")==0);//�A��
			else if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //�f�B���N�g��ʹ��
			{
				strcpy(ringitem.name,fd.cFileName);
				ringlist.push_back(ringitem);
			}
		} while(FindNextFile(hFind, &fd));
		FindClose(hFind);
	}
}

//�w��C���f�b�N�X�̃����O��������
void CCharacterList::InitializeRing(DWORD index)
{
	HANDLE hFind;
	WIN32_FIND_DATA fd;
	char ringdir[32];
	char chardir[64];

	sprintf(ringdir,"char\\%s\\*.*",ringlist.at(index).name);
	hFind = FindFirstFile(ringdir, &fd);
	sprintf(ringdir,"char\\%s\\",ringlist.at(index).name);

	if(hFind != INVALID_HANDLE_VALUE) {//�f�B���N�g�������݂���ꍇ
		do {
			if(strcmp(fd.cFileName,".")==0 || strcmp(fd.cFileName,"..")==0);//�A��
			else if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //�f�B���N�g��ʹ��
			{
				sprintf(chardir,"%s%s",ringdir,fd.cFileName);
				VerifyCharacterDir(chardir,index);//����
			}
		} while(FindNextFile(hFind, &fd));
		FindClose(hFind);
	}
}

//�L�����N�^�[�f�B���N�g���̌���
BOOL CCharacterList::VerifyCharacterDir(char *dir,DWORD ringindex)
{
	char path[256];
	sprintf(path,"%s\\action.dll",dir);
	CDI_CHARACTERINFO info;
	ZeroMemory(&info,sizeof(CDI_CHARACTERINFO));
	info.system_version = GOLUAH_SYSTEM_VERSION;

	CCL_DAMEINFO dameinfo;

	sprintf(dameinfo.name,"");

	HINSTANCE hLib = LoadLibrary(path);
	if(hLib == NULL){		//DLL�ǂݍ��ݎ��s
		strcpy(dameinfo.dir,dir);
		dameinfo.dame = CCL_DAME_NODLL;
		damelist.push_back(dameinfo);
		return(FALSE);
	}
	PFUNC_CHARACTERINFO pfunc;
	pfunc = (PFUNC_CHARACTERINFO)GetProcAddress(hLib, "CharacterInfo");
	if(pfunc==NULL){		//�֐��|�C���^�擾���s
		FreeLibrary(hLib);
		strcpy(dameinfo.dir,dir);
		dameinfo.dame = CCL_DAME_CANTGETFP;
		damelist.push_back(dameinfo);
		return(FALSE);
	}
	if(!(*pfunc)(&info)){	//CharacterInfo��FALSE��Ԃ�����_��
		FreeLibrary(hLib);
		strcpy(dameinfo.dir,dir);
		dameinfo.dame = CCL_DAME_FFAIL;
		damelist.push_back(dameinfo);
		return(FALSE);
	}
	if((info.ver > 800 && info.ver<1000) ||
	   info.ver < 400 /*680*/){		//�o�[�W�����̃`�F�b�N�i�Â��ق��j
		strcpy(dameinfo.dir,dir);
		strcpy(dameinfo.name,info.name);
		dameinfo.dame = CCL_DAME_OLDDLL;
		dameinfo.ver = info.ver;
		damelist.push_back(dameinfo);
		FreeLibrary(hLib);
		return(FALSE);
	}
	if(info.ver > CDI_VERSION){//�o�[�W�����̃`�F�b�N�i�V�����ق��j
		strcpy(dameinfo.dir,dir);
		strcpy(dameinfo.name,info.name);
		dameinfo.dame = CCL_DAME_NEWDLL;
		dameinfo.ver = info.ver;
		FreeLibrary(hLib);
		damelist.push_back(dameinfo);
		return(FALSE);
	}
	FreeLibrary(hLib);
	//�\���̂փf�[�^�R�s�[
	CCL_CHARACTERINFO newitem;
	ZeroMemory(&newitem,sizeof(CCL_CHARACTERINFO));
	strcpy_s(newitem.dir  , dir);						// ��������strcpy_s�Ή��A���āc
	strcpy(newitem.name , info.name);
	newitem.ver = info.ver;
	newitem.caps = info.caps;

	//�I�v�V�������ڂ��R�s�[����
	CHARACTER_LOAD_OPTION opt;
	for(DWORD i=0;i<32;i++){
		if(strlen(info.options[i].name)!=0){
			memcpy(&opt,&info.options[i],sizeof(CHARACTER_LOAD_OPTION));
			newitem.options.push_back(opt);
		}
	}
	newitem.maxpoint = info.max_option_point;

	//favoriteopt.txt ��ǂݍ���
	LoadFavoriteOptions( dir , newitem.fav_opts );

	//���X�g�ɒǉ�����
	ringlist.at(ringindex).ring2serialIndex.push_back(infolist.size());//�w�胊���O�ɃC���f�b�N�X�ǉ�
	infolist.push_back(newitem);//�ꗗ�ɒǉ�

	return(YEAH);
}

//********************************************************************
// Get�`�n
//********************************************************************

int CCharacterList::GetCharacterCount()
{
	return infolist.size();
}

int CCharacterList::GetCharacterCountRing(UINT ring)
{
	if(ringlist.size()-1 < ring){
		g_system.LogWarning("%s index�s��(%d/%d)",__FUNCTION__,ring,ringlist.size());
		return 0;
	}

	return ringlist.at(ring).ring2serialIndex.size();
}

char* CCharacterList::GetCharacterDir(UINT index)
{
	static char *error_return = "error";
	if(index>=infolist.size()){
		g_system.LogWarning("%s index�s��(%d/%d)",__FUNCTION__,index,infolist.size());
		return error_return;
	}

	return  infolist.at(index).dir;
}


char* CCharacterList::GetCharacterDir(UINT index,int ring)
{
	if(ring<0)return GetCharacterDir(index);
	return  GetCharacterDir( RingIndex2SerialIndex(ring,index) );
}

char* CCharacterList::GetCharacterName(UINT index)
{
	static char *error_return = "error";
	if(index>=infolist.size()){
		g_system.LogWarning("%s index�s��(%d/%d)",__FUNCTION__,index,infolist.size());
		return error_return;
	}

	return  infolist.at(index).name;
}

DWORD CCharacterList::GetCharacterVer(UINT index)
{
	if(index>=infolist.size()){
		g_system.LogWarning("%s index�s��(%d/%d)",__FUNCTION__,index,infolist.size());
		return 0;
	}
	return  infolist.at(index).ver;
}

int CCharacterList::GetDameCharCount()
{
	return  damelist.size();
}

char* CCharacterList::GetDameCharDir(UINT index)
{
	return damelist.at(index).dir;
}
char* CCharacterList::GetDameCharName(UINT index)
{
	return(damelist[index].name);
}
DWORD CCharacterList::GetDameCharReas(UINT index)
{
	return(damelist[index].dame);
}
DWORD CCharacterList::GetDameCharVer(UINT index)
{
	return(damelist[index].ver);
}
DWORD CCharacterList::RingIndex2SerialIndex(UINT ring,UINT index)
{
	if(ring > ringlist.size()-1){
		g_system.LogWarning("%s ring�s��(%d/%d)",__FUNCTION__,ring,ringlist.size());
		return 0;
	}
	if(ringlist.at(ring).ring2serialIndex.size()<=index){
		g_system.LogWarning("%s index�s��(%d/%d)",__FUNCTION__,index,infolist.size());
		return 0;
	}

	return  ringlist.at(ring).ring2serialIndex.at(index)  ;
}

int CCharacterList::FindCharacter(char *name)
{
	if(name==NULL)return(-1);

	for(DWORD i=0;i<infolist.size();i++){
		if(strcmp(infolist[i].name,name)==0)return(i);
	}
	return(-1);
}



/*�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`

						�I�v�V�����n�H

  �`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`*/
CCOptionSelecter* CCharacterList::GetOptionSelecter(DWORD cindex)
{
	if(cindex>infolist.size())return(NULL);
	return new CCOptionSelecter( 
		&infolist[cindex],
		infolist[cindex].maxpoint);
}


/*�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`

						�I�v�V�����n�H

  �`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`*/
CCSimpleOptionSelecter* CCharacterList::GetSimpleOptionSelecter(DWORD cindex)
{
	if(cindex>infolist.size())return(NULL);
	return new CCSimpleOptionSelecter( 
		&infolist[cindex],
		infolist[cindex].maxpoint);
}

DWORD CCharacterList::GetRandomOption(DWORD index)
{
	if(index >= infolist.size())return 0;
	if( infolist[index].options.size()==0 )return 0;

	DWORD point_now = infolist[index].maxpoint;
	DWORD ret=0;

	std::list<CHARACTER_LOAD_OPTION> dlist;
	std::list<CHARACTER_LOAD_OPTION>::iterator i;
	CharOptionList::iterator i2;
	for(i2=infolist[index].options.begin();i2!=infolist[index].options.end();i2++){
		dlist.push_back(*i2);
	}

	DWORD breaker=0;
	while( point_now!=0 && dlist.size()!=0){
		for(i=dlist.begin();i!=dlist.end();i++){
			if(i->point > (int)point_now){//�|�C���g�`�F�b�N
				dlist.erase(i);
				break;
			}
			else if(ret & i->exclusive){//�����`�F�b�N
				dlist.erase(i);
				break;
			}
			else if(rand()%4==0){
				if( (i->depends&ret)==i->depends ){//�ˑ��`�F�b�N
					point_now -= i->point;
					ret |= i->flag;
					dlist.erase(i);
					break;
				}
			}
		}
		if(breaker++>1000)break;
	}
	return ret;
}


char* CCharacterList::GetRingName(UINT index)
{
	if( ringlist.size()-1 < index )return NULL;
	return ringlist.at(index).name;
}

int CCharacterList::GetRingNum()
{
	return ringlist.size();
}

//�s���ȃI�v�V���������O����
void CCharacterList::CorrectOption(UINT cindex,DWORD *opt_org)
{
	if(cindex >= infolist.size()){
		*opt_org = 0;
		return;
	}

	DWORD ret = *opt_org;
	DWORD crnt_flag;
	DWORD crnt_depend;
	DWORD crnt_exclusive;

	BOOL changed=TRUE;
	BOOL fail;
	CharOptionList::iterator i,ie;
	CharOptionList* optlist = &infolist[cindex].options;

	while(changed)
	{
		changed = FALSE;
		crnt_flag =0x00000001;
		for(int b=0;b<32;b++){
			if(ret&crnt_flag)
			{
				fail=FALSE;
				//���݃`�F�b�N
				{
					fail = TRUE;
					i =optlist->begin();
					ie=optlist->end();
					for(;i!=ie;i++){
						if( i->flag & crnt_flag ){//== crnt_flag ){
							fail=FALSE;
							crnt_exclusive = i->exclusive;
							crnt_depend = i->depends;
							break;
						}
					}
				}
				//�ˑ��`�F�b�N
				if(!fail){
					if((ret&crnt_depend) != crnt_depend){
						fail=TRUE;
					}
				}
				//�r���`�F�b�N
				if(!fail){
					if(ret&crnt_exclusive){
						fail=TRUE;
					}
				}

				if(fail){
					ret &= ~crnt_flag;
					changed = TRUE;
				}
			}
			crnt_flag <<= 1;//1�r�b�g�V�t�g
		}
	}

	*opt_org = ret;
}


/*----------------------------------------------------------------
	optset.txt �̓ǂݍ���
------------------------------------------------------------------*/
void CCharacterList::LoadFavoriteOptions(char* dir,FavoriteOptionList& list)
{
	char *path = new char[MAX_PATH];
	sprintf( path, "%s\\optset.txt",dir);

	AkiFile file;
	if(! file.Load( path ) ){
		delete [] path;
		return;
	}

	char *optstr= new char [128];

	FAVORITE_OPTION opt;
	DWORD i=0,j=0;
	int ssret;
	std::vector<DWORD> optlist;
	while(i<file.m_size-2)
	{
		if(file.m_buffer[i] == '#')
		{
			ssret = sscanf((const char*)(&file.m_buffer[i+1]),"%s %s",opt.name,optstr);
			if(ssret==EOF)break;
			else if(ssret==2 && strlen(optstr)==32)
			{
				opt.opt = 0;
				for(int b=0;b<32;b++){
					if(optstr[b]=='1'){
						opt.opt |= 0x00000001;
					}
					else if(optstr[b]!='0'){
						gbl.ods("2�i���I�v�V�����w��ǂݍ��݃G���[(%s)",optstr);
						break;
					}
					if(b!=31)opt.opt <<= 1;
				}
				if(opt.opt!=0)
				{
					list.push_back(opt);
				}
			}
		}
		i++;
	}

	delete [] optstr;
	delete [] path;
}


/*****************************************************************
	�L�����I���I�v�V�����Z���N�^�[
******************************************************************/

CCOptionSelecter::CCOptionSelecter(CCL_CHARACTERINFO *cinfo,DWORD maxp)
{
	m_ref_cinfo = cinfo;
	CharOptionList* col = &(cinfo->options);
	m_current_favorite = cinfo->previous_favorite;

	DWORD ini_opt = m_current_favorite==0 ? cinfo->previous_option : cinfo->fav_opts[m_current_favorite-1].opt;

	list = col;
	maxpoint = maxp;
	state = 0;
	counter = 0;
	current_point = maxp;
	current_selected = 0;
	for(int i=0;i<32;i++){
		enabled[i] = FALSE;
	}
	commit_enabled = TRUE;

	offset_x = 0;
	z = -0.2f;

	Initialize(ini_opt);
	m_current_favorite = cinfo->previous_favorite;
}

//�w��l�������l�Ƃ��Đݒ肷��
void CCOptionSelecter::Initialize(DWORD ini_opt)
{
	current_point = maxpoint;
	for(int i=0;i<32;i++){
		enabled[i] = FALSE;
	}

	//�����l�ݒ�
	DWORD k=0;
	for(CharOptionList::iterator ite=list->begin();ite!=list->end();ite++,k++){
		if(ite->flag & ini_opt)
		{
			enabled[k] = TRUE;
			current_point -= ite->point;

			//gbl.ods("... %s %d %d %X",ite->name,ite->point,current_point,ite->flag & ini_opt);
		}
	}
	m_current_favorite = 0;//free
}

//�p�b�h���͂���������
BOOL CCOptionSelecter::HandlePad(DWORD inputIndex)
{
	counter++;
	CharOptionList::iterator list2=list->begin();

	DWORD input = g_input.GetKey(inputIndex,0);

	//�������͏���
	if(input & KEYSTA_UP2){
		current_selected--;
		if(current_selected>list->size())current_selected = list->size();
	}
	else if(input & KEYSTA_DOWN2){
		current_selected++;
		if(current_selected>list->size())current_selected = 0;
	}

	DWORD curr_flag = GetSettings();

	//���E���������������ςŔ�������悤�ɂ���
	DWORD key1 = g_input.GetKey(inputIndex,1);
	DWORD key2 = g_input.GetKey(inputIndex,2);
	DWORD key3 = g_input.GetKey(inputIndex,3);
	BOOL leftON  = (input&KEYSTA_ALEFT) && (key1&KEYSTA_ALEFT) && (key2&KEYSTA_ALEFT) && (key3&KEYSTA_ALEFT2);
	BOOL rightON = (input&KEYSTA_ARIGHT) && (key1&KEYSTA_ARIGHT) && (key2&KEYSTA_ARIGHT) && (key3&KEYSTA_ARIGHT2);

	//B���͏���(optset�ύX)
	if(input & KEYSTA_BB2)
	{
		m_current_favorite++;
		if(m_current_favorite == m_ref_cinfo->fav_opts.size()+1){
			m_current_favorite = 0;
		}
		else
		{
			DWORD current_favorite = m_current_favorite;
			Initialize( m_ref_cinfo->fav_opts[m_current_favorite-1].opt );
			m_current_favorite = current_favorite;
		}
		//point over check
		if(current_point<0)
			commit_enabled=FALSE;
		else
			commit_enabled=TRUE;

		//Free�ȊO�̏ꍇ�͂n�j�փJ�[�\���ړ�
		if(m_current_favorite!=0){
			current_selected = list->size();
		}
	}
	//optset�ݒ肩��㉺�L�[����������Free
	if(m_current_favorite!=0){
		if(current_selected != list->size())
		{
			m_current_favorite=0;
		}
	}

	//C���͏���(�����_��)
	if( input&(KEYSTA_BC2) )
	{
		SetRandom();
		//point over check
		if(current_point<0)
			commit_enabled=FALSE;
		else
			commit_enabled=TRUE;
		current_selected = list->size();//OK�ʒu�ɃJ�[�\���ړ�
	}

	// ON/OFF
	if(input&(KEYSTA_BA2) || leftON || rightON)
	{
		if(current_selected==list->size())
		{
			if((commit_enabled||g_config.IsLimiterCut()) && input&(KEYSTA_BA2)){//A���͏���(����)
				state=1;
				counter=0;
				return TRUE;
			}
		}
		//turn off
		else if(enabled[current_selected]){
			current_point += list2[current_selected].point;
			enabled[current_selected] = FALSE;
			TurnOffDependFlags(list2[current_selected].flag);
		}
		//turn on
		else{
			if((list2[current_selected].depends & curr_flag)==list2[current_selected].depends){
				current_point -= list2[current_selected].point;
				enabled[current_selected] = TRUE;
				TurnOffExclisiveFlags(list2[current_selected].exclusive);
			}
		}
		//point over check
		if(current_point<0)
			commit_enabled=FALSE;
		else
			commit_enabled=TRUE;
	}

	if( input&(KEYSTA_BD2) )
	{
		this->state=0xFFFFFFFF;
		return TRUE;
	}
	return FALSE;
}

//flag�Ɉˑ�����t���O��S��OFF
void CCOptionSelecter::TurnOffDependFlags( DWORD flag )
{
	DWORD next_flag=0;
	DWORD k=0;
	for(CharOptionList::iterator ite=list->begin();ite!=list->end();ite++,k++){
		if(ite->depends & flag){
			if(enabled[k] ){
				enabled[k] = FALSE;
				current_point += ite->point;
				next_flag |= ite->flag;
			}
		}
	}
	if(next_flag)TurnOffDependFlags( next_flag );
}

//flag�Ƌ�������t���O��S��OFF
void CCOptionSelecter::TurnOffExclisiveFlags( DWORD ex_flag )
{
	DWORD k=0;
	DWORD next_flag=0;
	for(CharOptionList::iterator ite=list->begin();ite!=list->end();ite++,k++){
		if(ite->flag & ex_flag){
			if(enabled[k] ){
				enabled[k] = FALSE;
				current_point += ite->point;
				next_flag |= ite->flag;
			}
		}
	}
	if(next_flag)TurnOffDependFlags( next_flag );
}

//�`�悷��BStartDraw���������Ă��ԂŌĂяo������
void CCOptionSelecter::Draw()
{
	int y = 240-list->size()*16;
	int off_x2 = 20;

	char *tmp_str;
	tmp_str = new char[128];

	DWORD ex_flag =0;
	DWORD k=0;
	CharOptionList::iterator ite;
	for(ite=list->begin();ite!=list->end();ite++){
		if(enabled[k]){
			ex_flag |= ite->exclusive;
		}
		k++;
	}

	sprintf(tmp_str,"=--OPTIONS-- POINT:%d",current_point);
	g_system.DrawBMPText(offset_x,y,z,tmp_str,0xFFBBBBBB);

	y+=32;

	k=0;
	DWORD color;
	DWORD setting_now = GetSettings();
	BOOL not_available;
	for(ite=list->begin();ite!=list->end();ite++){
		not_available = FALSE;
		//�F�͉��F�H
		if(ite->flag & ex_flag){//�����t���O�A��
			if(k!=current_selected)color=0xFFBBBB00;
			else color=0xFFFFFF00;
			not_available = TRUE;
		}
		else if(enabled[k]){//�L�����
			if(k!=current_selected)color=0xFF0000BB;
			else color=0xFF0000FF;
		}
		else {//�������
			if((ite->depends&setting_now)!=ite->depends){//depends�t���O������Ȃ�
				if(k!=current_selected)color=0xFFBBBB00;
				else color=0xFFFFFF00;
				not_available = TRUE;
			}
			else if(current_point<0 || current_point < ite->point){//ON�ɂ���ƃ|�C���g����Ȃ�
				if(k!=current_selected)color=0xFFBB0000;
				else color=0xFFFF0000;
			}
			else{//ON�ɂ��Ă��|�C���g�͑����
				if(k!=current_selected)color=0xFFBBBBBB;
				else color=0xFFFFFFFF;
			}
		}
		//�`��
		g_system.DrawBMPText(offset_x+off_x2,y,z,ite->name,color);
		if(not_available)
			g_system.DrawBMPText(offset_x+320,y,z,"N/A",color);
		else if(enabled[k])
			g_system.DrawBMPText(offset_x+320,y,z,"ON",color);
		else
			g_system.DrawBMPText(offset_x+320,y,z,"OFF",color);
		y+=32;
		k++;
	}

	//--OK--
	if(commit_enabled){
		if(current_selected!=list->size())
			g_system.DrawBMPText(offset_x+150,y,z,"--OK--",0xFFBBBBBB);
		else
			g_system.DrawBMPText(offset_x+150,y,z,"--OK--",0xFFFFFFFF);
	}
	else{
		if(current_selected!=list->size())
			g_system.DrawBMPText(offset_x+150,y,z,"--OVER--",0xFFBB0000);
		else
			g_system.DrawBMPText(offset_x+150,y,z,"--OVER--",0xFFFF0000);
	}

	delete [] tmp_str;
}

//�t�F�[�h�A�E�g���l���擾����
BYTE CCOptionSelecter::GetFadeOut()
{
	DWORD tmp = counter*4;
	if(state==0){
		if(tmp>255)return 255;
		return (BYTE)tmp;
	}
	else if(state==1){
		if(tmp>255)return 0;
		return (BYTE)(255-tmp);
	}
	return 0;
}

DWORD CCOptionSelecter::GetSettings()
{
	DWORD ret = 0;
	DWORD k=0;
	CharOptionList::iterator ite;
	for(ite=list->begin();ite!=list->end();ite++,k++){
		if(enabled[k]){
			ret |= ite->flag;
		}
	}
	return ret;
}

void CCOptionSelecter::SetRandom()
{
	if (m_ref_cinfo->fav_opts.size()!= 0)
	{
		//favorite�ݒ�ł̃����_��
		m_current_favorite = (rand()%m_ref_cinfo->fav_opts.size());
		DWORD current_favorite = m_current_favorite;
		Initialize( m_ref_cinfo->fav_opts[m_current_favorite].opt );
		m_current_favorite = current_favorite;
		return;
	}

	//Free�����_��
	int k;
	for(k=0;k<32;k++){
		enabled[k]=FALSE;
	}
	current_point = maxpoint;

	std::list<CHARACTER_LOAD_OPTION> dlist;
	CharOptionList::iterator i2;
	for(i2=list->begin();i2!=list->end();i2++){
		dlist.push_back(*i2);
	}

	//srand(timeGetTime());

	DWORD ret=0;
	DWORD breaker=0;
	std::list<CHARACTER_LOAD_OPTION>::iterator i;
	while( current_point>0 && dlist.size()!=0)
	{
		for(i=dlist.begin();i!=dlist.end();i++)
		{
			if(i->point > current_point){//�|�C���g�`�F�b�N
				dlist.erase(i);
				break;
			}
			else if(ret & i->exclusive){//�����`�F�b�N
				dlist.erase(i);
				break;
			}
			else if(rand()%4==0 )
			{
				if( (i->depends&ret)==i->depends )//�ˑ��`�F�b�N
				{
					//�t���O���Z
					current_point -= i->point;
					ret |= i->flag;
					//gbl.ods("%s %d %d %X",i->name,i->point,current_point,i->flag);
					dlist.erase(i);


					break;
				}
			}
			else if(i->point<=0 && rand()%3)
			{
				dlist.erase(i);
				break;
			}
		}
		if(breaker++>1000){
			gbl.ods("CCOptionSelecter::SetRandom �����u���[�N\n");
			break;
		}
	}

	//�l�K�e�B�u�I�v�V�����΍�
	dlist.clear();
	for(i2=list->begin();i2!=list->end();i2++){
		dlist.push_back(*i2);
	}
	for(i=dlist.begin();i!=dlist.end();i++)
	{
		if(ret&i->flag)continue;
		if(i->point <= current_point && i->point>0){//�|�C���g�`�F�b�N
			if(ret & i->exclusive){//�����`�F�b�N
			}
			else if( (i->depends&ret)==i->depends )//�ˑ��`�F�b�N
			{
				//�t���O���Z
				current_point -= i->point;
				ret |= i->flag;
				//gbl.ods("%s %d %d %X",i->name,i->point,current_point,i->flag);
			}
		}
	}

	Initialize(ret);
	//return ret;
}

//0:"Free" , 0�`:favorite�ݒ薼
char* CCOptionSelecter::GetCurrentSetName()
{
	if(m_current_favorite==0)return "Free";

	return m_ref_cinfo->fav_opts[ m_current_favorite-1 ].name;
}

//�O��I���t���O�ɐݒ肷��
void CCOptionSelecter::ApplyToPreviousSelect()
{
	m_ref_cinfo->previous_favorite = m_current_favorite;
	m_ref_cinfo->previous_option = GetSettings();
}


/*****************************************************************
	�L�����I���I�v�V�����Z���N�^�[�i�P�����Łj
******************************************************************/

CCSimpleOptionSelecter::CCSimpleOptionSelecter(CCL_CHARACTERINFO *cinfo,DWORD maxp)
: CCOptionSelecter(cinfo, maxp)
{

}

//�p�b�h���͂���������
BOOL CCSimpleOptionSelecter::HandlePad(DWORD inputIndex)
{
	counter++;
	CharOptionList::iterator list2=list->begin();

	DWORD input = g_input.GetKey(inputIndex,0);

	//�������͏���
	if(input & KEYSTA_UP2){
		m_current_favorite--;
		if(m_current_favorite == 0xFFFFFFFF){
			m_current_favorite = m_ref_cinfo->fav_opts.size();
		}
		if(m_current_favorite != 0)
		{
			DWORD current_favorite = m_current_favorite;
			Initialize( m_ref_cinfo->fav_opts[m_current_favorite-1].opt );
			m_current_favorite = current_favorite;
		}
		//point over check
		if(current_point<0)
			commit_enabled=FALSE;
		else
			commit_enabled=TRUE;
	}
	else if(input & (KEYSTA_DOWN2 | KEYSTA_BB2)){				// B�ł�����\��
		m_current_favorite++;
		if(m_current_favorite == m_ref_cinfo->fav_opts.size()+1){
			m_current_favorite = 0;
		}
		else
		{
			DWORD current_favorite = m_current_favorite;
			Initialize( m_ref_cinfo->fav_opts[m_current_favorite-1].opt );
			m_current_favorite = current_favorite;
		}
		//point over check
		if(current_point<0)
			commit_enabled=FALSE;
		else
			commit_enabled=TRUE;
	}

	DWORD curr_flag = GetSettings();

	DWORD key1 = g_input.GetKey(inputIndex,1);
	DWORD key2 = g_input.GetKey(inputIndex,2);
	DWORD key3 = g_input.GetKey(inputIndex,3);
	BOOL leftON  = (input&KEYSTA_ALEFT) && (key1&KEYSTA_ALEFT) && (key2&KEYSTA_ALEFT) && (key3&KEYSTA_ALEFT2);
	BOOL rightON = (input&KEYSTA_ARIGHT) && (key1&KEYSTA_ARIGHT) && (key2&KEYSTA_ARIGHT) && (key3&KEYSTA_ARIGHT2);

	//B���͏���(optset�ύX)
	if(input & KEYSTA_BB2)
	{
	}
	//optset�ݒ肩��㉺�L�[����������Free
	/*if(m_current_favorite!=0){
		if(current_selected != list->size())
		{
			m_current_favorite=0;
		}
	}*/

	//C���͏���(�����_��)
	if( input&(KEYSTA_BC2) && !m_ref_cinfo->fav_opts.empty() )
	{
/*		SetRandom();
		current_selected = list->size();//OK�ʒu�ɃJ�[�\���ړ�
*/		m_current_favorite = (rand() % m_ref_cinfo->fav_opts.size()) +1;

		DWORD current_favorite = m_current_favorite;
		Initialize( m_ref_cinfo->fav_opts[m_current_favorite-1].opt );
		m_current_favorite = current_favorite;
		//point over check
		if(current_point<0)
			commit_enabled=FALSE;
		else
			commit_enabled=TRUE;
	}

	// ON/OFF
	if(input&(KEYSTA_BA2))
	{
		if((commit_enabled||g_config.IsLimiterCut()) && input&(KEYSTA_BA2)){//A���͏���(����)
			state=1;
			counter=0;
			return TRUE;
		}
	}

	if( input&(KEYSTA_BD2) )
	{
		this->state=0xFFFFFFFF;
		return TRUE;
	}
	return FALSE;
}


