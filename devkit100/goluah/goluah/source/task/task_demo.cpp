
/*===============================================================

	�X�g�[���[���ԃf�� �^�X�N
	ver0.5

=================================================================*/

#include "global.h"
#include "task_demo.h"

#define DEMO050MAX_DEMOSCENE 64

//*************************************************************************

extern int story_selectflag;		// �b��ݒu�A���̒u�����script_story.cpp�B

CTDemo::CTDemo()
{
	demodat=NULL;
	ms=NULL;
	m_script_filename = NULL;
	chr[0] = chr[1] = chr[2] = NULL;
	select = -1;
	select_base = 0;
}

CTDemo::~CTDemo()
{
	CleanUp();
}

void CTDemo::CleanUp()
{
	FREEMALM(demodat);
	RELSURFACE(ms);
}

void CTDemo::Setup(UINT storyindex,char *demofilename)
{
	m_current_story_index = storyindex;
	m_script_filename = demofilename;
	m_current_dir = g_storylist.GetStoryDir(m_current_story_index);
}

//**************************************************************************************

void CTDemo::Initialize()
{
	demodat = (DEMODAT*)malloc(sizeof(DEMODAT)*DEMO050MAX_DEMOSCENE);
	if(demodat==NULL){
		sprintf(errc,"�ُ�F�������m�ۂɎ��s�H");
		demoready=FALSE;
		return;
	}
	ZeroMemory(demodat,sizeof(DEMODAT)*DEMO050MAX_DEMOSCENE);
	counter=0;
	counter2=0;

	char *filename = new char [MAX_PATH];
	sprintf(filename,"%s\\%s",m_current_dir,m_script_filename);
	num_demoscenes = InitDemoDat(filename);
	delete [] filename;
	if(num_demoscenes==0){
		demoready=FALSE;
		return;
	}
	demoready=TRUE;

	playingdemodat=0;
	txtlen = 0;
	multibyte_flag = FALSE;

	g_sound.BGMStop();
	//�f���f�[�^�̎d�l�Ɋ�Â��ăr�b�g�}�b�v�Ȃǂ����[�h
	sprintf(fpath,"%s\\",m_current_dir);
	sprintf(&fpath[strlen(fpath)],demodat[playingdemodat].bitmap[0]);
	RELSURFACE(ms);
	ms = g_draw.CreateSurfaceFrom256BMP(fpath);
	if(ms != NULL){//�T�[�t�F�C�X�̕��^����
		bmpw = (DWORD)ms->wg;
		bmph = (DWORD)ms->hg;
		if(g_config.IsHalfMode()){bmpw*=2;bmph*=2;}
	}
	if(strlen(demodat[playingdemodat].bgm) != 0){
		sprintf(fpath,"%s\\",m_current_dir);
		sprintf(&fpath[strlen(fpath)],demodat[playingdemodat].bgm);
		g_sound.BGMSeekAndPlay(fpath,FALSE,demodat[playingdemodat].bgmpos);
	}
	for (int i = 1; i < 4; i++)
	{
		sprintf(fpath,"%s\\",m_current_dir);
		sprintf(&fpath[strlen(fpath)],demodat[playingdemodat].bitmap[i]);
		RELSURFACE(chr[i - 1]);
		chr[i - 1] = g_draw.CreateSurfaceFrom256BMP(fpath);
	}
}

void CTDemo::Terminate()
{
	if(select>=0)
	story_selectflag += select_base + select;		// �I���t���O���X�V

	CleanUp();
	g_sound.BGMStop();
}

#define DEMODISPX 500
#define DEMODISPY 250

BOOL CTDemo::Execute(DWORD time)
{
	if(!demoready){
		return Execute2();
	}

	yurex=0;
	yurey=0;

	//�w�i�`��p�����[�^
	r_all.top=r_all.left=0;
	r_all.right=bmpw;
	r_all.bottom=bmph;
	if(demodat[playingdemodat].bmpdisp & 0x0000000F){
		if(demodat[playingdemodat].bmpdisp & DEMOBMPDISP_MOVER){//���X�N���[��
			r_all.left = counter;
			r_all.right = counter + DEMODISPX;
			if(r_all.right > bmpw){
				r_all.left -= r_all.right - bmpw;
				r_all.right = bmpw;
			}
		}
		if(demodat[playingdemodat].bmpdisp & DEMOBMPDISP_MOVEL){//���X�N���[��
			r_all.right = bmpw - counter;
			r_all.left = r_all.right - DEMODISPX;
			if(r_all.left < 0){
				r_all.left = 0;
				r_all.right = DEMODISPX;
			}
		}
		if(demodat[playingdemodat].bmpdisp & DEMOBMPDISP_MOVED){//���X�N���[��
			r_all.top = counter;
			r_all.bottom = r_all.top + DEMODISPY;
			if(r_all.bottom > bmph){
				r_all.top = bmph-DEMODISPY;
				r_all.bottom = bmph;
			}
		}
		if(demodat[playingdemodat].bmpdisp & DEMOBMPDISP_MOVEU){//���X�N���[��
			r_all.bottom = bmph - counter;
			r_all.top = r_all.bottom - DEMODISPY;
			if(r_all.top <0){
				r_all.top = 0;
				r_all.bottom = DEMODISPY;
			}
		}
	}
	if(demodat[playingdemodat].bmpdisp & DEMOBMPDISP_FIXR){//�E�[fix
		r_all.left = bmpw-DEMODISPX;
		r_all.right = bmpw;
	}
	if(demodat[playingdemodat].bmpdisp & DEMOBMPDISP_FIXB){//���[fix
		r_all.bottom = bmph;
		r_all.top = bmph - DEMODISPY;
	}
	if(demodat[playingdemodat].bmpdisp & DEMOBMPDISP_YUREX){//x�����h��
		yurex = (counter/2%2)*2;
	}
	if(demodat[playingdemodat].bmpdisp & DEMOBMPDISP_YUREY){//y�����h��
		yurey = (counter/2%2)*2;
	}
	r_all.left += yurex;
	r_all.top +=yurey;
	if(r_all.bottom-r_all.top >DEMODISPY)r_all.bottom = r_all.top+DEMODISPY;
	if(r_all.right - r_all.left >DEMODISPX)r_all.right = r_all.left+DEMODISPX;

	//������`��p�����[�^
	if(demodat[playingdemodat].spdmsg <= 0){
		txtlen = strlen(demodat[playingdemodat].msg);
	}
	else if((int)counter2 >= demodat[playingdemodat].spdmsg){
		counter2 -= demodat[playingdemodat].spdmsg;
		if(!multibyte_flag){
			multibyte_flag = (demodat[playingdemodat].msg[txtlen] & 0x80) ? TRUE : FALSE;
			txtlen += multibyte_flag ? 2 : 1;
		}
		else multibyte_flag=FALSE;
	}
	if(txtlen > strlen(demodat[playingdemodat].msg))
		{txtlen=strlen(demodat[playingdemodat].msg);}

	//���̃V�[���Ɉړ����邩�H
	if(txtlen >= strlen(demodat[playingdemodat].msg)){
		demodat[playingdemodat].dur--;
		if(demodat[playingdemodat].dur <= 0){//���̃V�[���Ɉړ�
			counter = counter2 = 1;
			txtlen = 0;
			multibyte_flag = FALSE;
			playingdemodat++;
			if(playingdemodat >= num_demoscenes){//�f���I��
				playingdemodat--;
				return FALSE;
			}
			//�r�b�g�}�b�v�̕ύX
			if(strlen(demodat[playingdemodat].bitmap[0]) > 0){
				sprintf(fpath,"%s\\",m_current_dir);
				sprintf(&fpath[strlen(fpath)],demodat[playingdemodat].bitmap[0]);
				RELSURFACE(ms);
				ms = g_draw.CreateSurfaceFrom256BMP(fpath);
				if(ms != NULL){//�T�[�t�F�C�X�̕��^����
					bmpw = (DWORD)ms->wg;
					bmph = (DWORD)ms->hg;
				}
			}
			//�a�f�l�̕ύX
			if(strlen(demodat[playingdemodat].bgm) != 0){
				sprintf(fpath,"%s\\",m_current_dir);
				sprintf(&fpath[strlen(fpath)],demodat[playingdemodat].bgm);
				g_sound.BGMSeekAndPlay(fpath,FALSE,demodat[playingdemodat].bgmpos);
			}
			// �I�������Z�b�g
			if (demodat[playingdemodat].num_select > 0){
				select = 0;
			}
		}
	}
	counter++;
	counter2++;
	
	// �I��������
	if (demodat[playingdemodat].num_select > 0)
	{
		select_base = demodat[playingdemodat].select_base;

		if (g_input.GetKey(m_keyindex,0) & KEYSTA_DOWN2)
		{
			select = (select + 1) % demodat[playingdemodat].num_select;
		}
		else if (g_input.GetKey(m_keyindex,0) & KEYSTA_UP2)
		{
			select--;
			if (select < 0)
				select = demodat[playingdemodat].num_select - 1;
		}
	}

	if(counter > 1 && g_input.GetKey(m_keyindex,0) & KEYSTA_BUTTONS){//�����I�ɏI��
//		return FALSE;
		// �P�Ɉꕶ��΂������ɕς��Ă݂�
		if(txtlen < strlen(demodat[playingdemodat].msg))
			{ txtlen=strlen(demodat[playingdemodat].msg); }
		else
			demodat[playingdemodat].dur = 0;
	}

	if(counter > 1 && g_input.GetKey(m_keyindex,0) & KEYSTA_BD && demodat[playingdemodat].num_select <= 0){//�I�����łȂ����D�ő�����
		if(txtlen < strlen(demodat[playingdemodat].msg))
			{ txtlen=strlen(demodat[playingdemodat].msg); }
		else
			demodat[playingdemodat].dur = 0;
	}

	return TRUE;
}

BOOL CTDemo::Execute2()
{
	//���݂����Ăˁ[�炵��
/*	RECT r;
	r.left = r.top =0;
	r.right = 640;
	r.bottom = 480;
	g_draw.DrawBlueText(r,errc,-1,DT_CENTER | DT_VCENTER,3);

	if(g_input.GetKey(m_keyindex,0) & 0x22220000){//�����I�ɏI��
		return FALSE;
	}*/

	return TRUE;
}

void CTDemo::Draw()
{
	g_draw.SetTransform(FALSE);

	//�w�i��`��
	if(ms != NULL)g_draw.CheckBlt(ms,70,90,r_all);
	for (int i = 0; i < 3; i++)
		if(chr[i] != NULL)g_draw.CheckBlt(chr[i],70+100+200*(i-1),90/*DEMODISPY - (int)(chr[i]->hg / 2)*/,r_all);

	//�������`��
	r_serif.top=370;
	r_serif.bottom=470;
	r_serif.left=30;
	r_serif.right=630;
	g_draw.DrawBlueText(r_serif,demodat[playingdemodat].msg,txtlen,DT_LEFT|DT_WORDBREAK,3);

	if (demodat[playingdemodat].num_select > 0)	// �I�����L��
	{
		char tmp[8];

		r_serif.left = 0;
		tmp[0] = '\0';

		for (int i = 0; i < select; i++)
			strncat(tmp, "\n", 8 - 1);
		strncat(tmp, "��", 8 - 1);
		g_draw.DrawRedText(r_serif, tmp, lstrlen(tmp), DT_LEFT, 3);
	}

	if(g_config.IsDebugMode())
	{
		//�p�����[�^�\��
		char *outs = new char[256];
		sprintf(outs,"scene=%d\n txtlen=%d , dur=%d / spd=%d\n",playingdemodat,txtlen,demodat[playingdemodat].dur,demodat[playingdemodat].spdmsg);
		sprintf(&outs[strlen(outs)]," bmp w/h = %d/%d\n",bmpw,bmph);
		sprintf(&outs[strlen(outs)]," select = %d , base = %d , selectflag = %d\n",select,demodat[playingdemodat].select_base,story_selectflag);
		r_serif.top=20;
		g_draw.DrawBlueText(r_serif,outs,-1,DT_LEFT,1);
		delete [] outs;
	}
}

//======================================================================================

//����I���F�V�[����
//�G���[�F0
int CTDemo::InitDemoDat(char *filepath)
{
	HANDLE hFile = CreateFile(filepath,
		GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE){
		sprintf(errc,"�t�@�C���̃I�[�v���Ɏ��s\n%s",filepath);
		return(0);
	}

	char *gotfile;
	DWORD fsize = GetFileSize(hFile,NULL);
	if(fsize==0){
		CloseHandle(hFile);
		sprintf(errc,"�t�@�C���T�C�Y0\n%s",filepath);
		return(0);
	}
	gotfile = (char*)malloc(fsize+2);
	ZeroMemory(gotfile,fsize+2);


	gbl.ods("���f��[%s/%dbyte]���[�h�J�n",filepath,fsize);

	DWORD br;
	DWORD ret=ReadFile(hFile,gotfile,fsize,&br,NULL);
	CloseHandle(hFile);

	gotfile[fsize]=13;//���s�R�[�h�t��
	gotfile[fsize+1]=10;
	fsize+=2;

#define RSTRBUFFERSIZE 1024

	//��s�Â|��
	DWORD strpos=0;
	DWORD susumu;
	int rval;
	char *rstr = (char*)malloc(RSTRBUFFERSIZE);
	BOOL ext=FALSE,ext2=FALSE;
	BOOL find;
	int numevents=0;

	//�Ƃ肠�����ŏ��̈��������
	find=FALSE;
	while(strpos < fsize && !find){
		rval = GetGyoDemo(&gotfile[strpos],rstr,&susumu);
		strpos+=susumu;
		if(rval == 1)find = TRUE;
	}
	if(!find){
		sprintf(errc,"���e����\n%s",filepath);
		free(gotfile);
		free(rstr);
		return(0);
	}

	char spdtmp[5];
	ZeroMemory(spdtmp,sizeof(spdtmp));

	while(find){
		find=FALSE;
		if(rval==1){//set defaults
			ZeroMemory(&demodat[numevents],sizeof(DEMODAT));
			demodat[numevents].dur = -1;
			demodat[numevents].bmpdisp = 0;
			demodat[numevents].msg[0] = '\0';
			demodat[numevents].num_select = 0;
			demodat[numevents].select_base = 0;
			while(strpos < fsize && !find){
				ZeroMemory(rstr,RSTRBUFFERSIZE);
				rval=GetGyoDemo(&gotfile[strpos],rstr,&susumu);
				switch(rval){
				case 1://#scene
					find=TRUE;
					break;
				case 2://bgm
					sprintf(demodat[numevents].bgm , rstr);
					break;
				case 3://sound
					sprintf(demodat[numevents].wavsnd , rstr);
					break;
				case 4://bitmap
					sprintf(demodat[numevents].bitmap[0] , rstr);
					break;
				case 5://text
					strncat(demodat[numevents].msg , rstr, 1024 - 1);
					strncat(demodat[numevents].msg, "\n", 1024 - 1);
					break;
				case 6://dur
					demodat[numevents].dur = atoi(rstr);
					break;
				case 7://spd
					spdtmp[0] = rstr[0];
					demodat[numevents].spdmsg = atoi(spdtmp);
					break;
				case 8://bmpdisp : yurex
					demodat[numevents].bmpdisp |= DEMOBMPDISP_YUREX;
					break;
				case 9://bmpdisp : yurey
					demodat[numevents].bmpdisp |= DEMOBMPDISP_YUREY;
					break;
				case 10://bmpdisp : mover
					demodat[numevents].bmpdisp |= DEMOBMPDISP_MOVER;
					break;
				case 11://bmpdisp : movel
					demodat[numevents].bmpdisp |= DEMOBMPDISP_MOVEL;
					break;
				case 12://bmpdisp : moveu
					demodat[numevents].bmpdisp |= DEMOBMPDISP_MOVEU;
					break;
				case 13://bmpdisp : moved
					demodat[numevents].bmpdisp |= DEMOBMPDISP_MOVED;
					break;
				case 14://bmpdisp : fixr
					demodat[numevents].bmpdisp |= DEMOBMPDISP_FIXR;
					break;
				case 15://bmpdisp : fixb
					demodat[numevents].bmpdisp |= DEMOBMPDISP_FIXB;
					break;
				case 16://bgmpos
					demodat[numevents].bgmpos = atof(rstr);
					break;
				case 17://bmp-char
				case 18:
				case 19:
					sprintf(demodat[numevents].bitmap[rval - 17 + 1] , rstr);
					break;
				case 20://select
					select = 0;
					demodat[numevents].num_select = atoi(rstr);
					break;
				case 21://select_base
					demodat[numevents].select_base = atoi(rstr);
					break;
				case 22://selectf
					story_selectflag = atoi(rstr);
					break;
				}
				strpos+=susumu;
			}

			// ����ɏ����Ȃ��悤��
			if (demodat[numevents].num_select > 0){
				demodat[numevents].dur = INT_MAX;
				demodat[numevents].spdmsg = 0;
			}
			numevents++;
		}
	}
	
	free(gotfile);
	free(rstr);
	return(numevents);
}

DWORD CTDemo::GetGyoDemo(char *strin,char *strout,DWORD *susumu)
{
	DWORD rval=0;
	DWORD i=0;

	switch(strin[0]){
	case '#'://#scene /1
		if(strin[1]=='s' && strin[2]=='c' && strin[3]=='e' && strin[4]=='n' && strin[5]=='e')
		{rval=1;i=6;}break;
	case 'b'://bgm /2 -- //bmp /4 -- //bgmpos /16
		if(strin[1]=='g' && strin[2]=='m'){
			rval=2;i=3;
			if(strin[3]=='p' && strin[4]=='o' && strin[5]=='s'){
				rval=16;i=6;
			}
		}
		if(strin[1]=='m' && strin[2]=='p'){
			rval=4;i=3;
			if(strin[3]=='-'){
				if(strin[4]=='y' && strin[5]=='u' && strin[6]=='r' && strin[7]=='e'){
					if(strin[8]=='x'){
						rval = 8; i=9;//bmp-yurex
					}
					else if(strin[8]=='y'){
						rval = 9; i=9;//bmp-yurey
					}
					else{
						rval=0; i=8;
					}
				}
				if(strin[4]=='m' && strin[5]=='o' && strin[6]=='v' && strin[7]=='e'){
					if(strin[8]=='r'){
						rval = 10; i=9;//bmp-mover
					}
					else if(strin[8]=='l'){
						rval = 11; i=9;//bmp-movel
					}
					else if(strin[8]=='u'){
						rval = 12; i=9;//bmp-moveu
					}
					else if(strin[8]=='d'){
						rval = 13; i=9;//bmp-moved
					}
					else {
						rval=0; i=8;
					}
				}
				if(strin[4]=='f' && strin[5]=='i' && strin[6]=='x'){
					if(strin[7]=='r'){
						rval = 14; i=8;//bmp-fixr
					}
					else if(strin[7]=='b'){
						rval = 15; i=8;//bmp-fixb
					}
					else {
						rval=0; i=7;
					}
				}
				if(strin[4]=='c' && strin[5]=='h' && strin[6]=='a' && strin[7]=='r'){
					UINT n = strin[8] - '1';

					if (n < 3){
						rval = 17 + n; i = 9;	// bmp-char(1-3)
					}
					else {
						rval=0; i=8;
					}
				}
			}
		}
		break;
	case 's'://sound /3 -- spd /7  select /20 select_base /21 selectf /22
		if(strin[1]=='o' && strin[2]=='u' && strin[3]=='n' && strin[4]=='d'){
			rval=3;i=5;}
		else if(strin[1]=='p' && strin[2]=='d'){
			rval=7;i=3;}
		else if(strin[1]=='e' && strin[2]=='l' && strin[3]=='e' && strin[4]=='c' && strin[5]=='t'){
			if (strncmp(strin + 6, "_base", 5) == 0){
				rval=21;i=11;}
			else if(strin[6]=='f'){
				rval=22; i=7;}
			else{
				rval=20;i=6;}
		}
		break;
	case 't'://text /5
		if(strin[1]=='e' && strin[2]=='x' && strin[3]=='t'){
			rval=5;i=4;}break;
	case 'd'://dur /6
		if(strin[1]=='u' && strin[2]=='r'){
			rval=6;i=3;}break;
	default:
		rval=0;
	}

	if(rval==0 || rval==1 || rval>=8 && rval<16 ){//���s�R�[�h�������Ė߂�
		while(1){
			if(strin[i]==13 && strin[i+1]==10){//���s�R�[�h����
				i+=2;
				*susumu = i;
				return(rval);
			}
			else{i++;}
		}
	}

	//�X�y�[�X���΂�����
	BOOL ext=FALSE;
	while(!ext){
		if(strin[i]==0x20)i++;
		else if(strin[i]==0x81 && strin[i+1]==0x40)i+=2;
		else ext=TRUE;
	}

	if(strin[i]!='='){
		while(1){
			if(strin[i]==13 && strin[i+1]==10){//���s�R�[�h����
				i+=2;
				*susumu = i;
				return(0);
			}
			else{i++;}
		}
	}
	i++;

	//�X�y�[�X���΂�����
	if(rval != 5){
		ext=FALSE;
		while(!ext){
			if(strin[i]==0x20)i++;
			else if(strin[i]==0x81 && strin[i+1]==0x40)i+=2;
			else ext=TRUE;
		}
	}

	DWORD j=0;
	while(1){
		if(strin[i]==13 && strin[i+1]==10){//���s�R�[�h����
			i+=2;
			*susumu = i;
			return(rval);
		}
		else{i++;j++;}
		strout[j-1]=strin[i-1];//�����񂱂ҁ[
		if(rval==5 && j>1023)break;
		else if(rval!=5 && j>64)break;
	}

	while(1){
		if(strin[i]==13 && strin[i+1]==10){//���s�R�[�h����
			i+=2;
			*susumu = i;
			return(0);
		}
		else{i++;}
	}
}

