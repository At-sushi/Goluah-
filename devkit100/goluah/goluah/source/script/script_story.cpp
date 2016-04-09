
/*!
	@file
	@brief �X�g�[���[�X�N���v�g����

=================================================================*/

#include "stdafx.h"
#include "define_macro.h"
#include "script_story.h"

#include "global.h"


/*!
*	@brief �X�g�[���[�X�N���v�g�̓ǂݍ��ݎ���O��`
*
*	�X�g�[���[�X�N���v�g���[�h���ɖ�肪�N�������Ƃ��ɁA
*	�A�N�Z�X�ᔽ�Ƃ��Ƌ�ʂ��邽�߂ɂ��̃N���X�𓊂��Ă�݂����B
*/
class CStoryScriptException
{
public:
	CStoryScriptException(){}
	CStoryScriptException(CStoryScriptException& e){}//!< �R�s�[�R���X�g���N�^
};

int story_selectflag = 0;		// �b��ݒu�A�u���ꏊ�͍����B

/*---------------------------------------------------------------
	�X�g�[���[�X�N���v�g�̓ǂݍ���
-----------------------------------------------------------------*/
BOOL CGoluahStoryScript::CreateScriptElementList(SScriptElementList& list,char *location)
{
	CScriptLoader *loader = CScriptLoader::Create();
	char *tmpstr = new char[128];

	DestroyScriptElementList(list);

	BOOL ret =TRUE;
	CStoryElement_Settings *settings=NULL;

	try{
		if(!loader->LoadFile(location,_T("entry.txt"))){
			list.push_back(new CStoryElement_Error(_T("�t�@�C���̓ǂݍ��ݎ��s")));
			throw CStoryScriptException();
		}
		
		int line_index = 0;
		int line_max = loader->GetLineNum();
		while(line_index < line_max)
		{
			char *linestr = loader->GetLine(line_index);
			if(*linestr=='#')//�u���b�N��`�J�n
			{
				linestr++;
				sscanf(linestr,_T("%s"),tmpstr);
				if(list.size()==0){
					if(strcmp(tmpstr,_T("settings"))!=0){
						list.push_back(new CStoryElement_Error(_T("settings�u���b�N�̑O�ɑ��u���b�N���`���邱�Ƃ͂ł��܂���")));
						throw CStoryScriptException();
					}
				}
				else{
					if(list[list.size()-1]->Close()==FALSE){
						throw CStoryScriptException();
					}
				}
				{//�e�u���b�N�̓ǂݍ���
					if(strcmp(tmpstr,_T("vs"))==0){
						list.push_back(new CStoryElement_VS(settings));//#vs
					}
					else if(strcmp(tmpstr,_T("staff"))==0){
						list.push_back(new CStoryElement_Staff);//#staff
					}
					else if(strcmp(tmpstr,_T("demo"))==0){
						list.push_back(new CStoryElement_Demo);//#demo
					}
					else if(strcmp(tmpstr,_T("settings"))==0){
						settings = new CStoryElement_Settings;
						list.push_back(settings);//#settings
					}
					else if(strcmp(tmpstr,_T("select"))==0){
						int num = 0;

						if (settings->ver < 1.1f) {
							list.push_back(new CStoryElement_Error(_T("�o�[�W�����ݒ肪�Â��ł�")));
							throw CStoryScriptException();
						}
						sscanf(linestr, _T("select %d"), &num);
						list.push_back( new CStoryElement_Select(num) );
					}
					else if(strcmp(tmpstr,_T("end_select"))==0){

						if (settings->ver < 1.1f) {
							list.push_back(new CStoryElement_Error(_T("�o�[�W�����ݒ肪�Â��ł�")));
							throw CStoryScriptException();
						}
						list.push_back( new CStoryElement_Select(-1) );
					}
					else{//#����ȊO�H
						char *errstr = new char[256];
						sprintf(errstr,_T("���m�̃u���b�N�� : %s"),tmpstr);
						list.push_back(new CStoryElement_Error(errstr));
						delete [] errstr;
						throw CStoryScriptException();
					}
				}
			}
			else if(list.size()!=0){
				list[list.size()-1]->FeedLine(linestr);
			}
			line_index++;
		}
	}
	catch(CStoryScriptException e){
		e;
		gbl.ods(_T("catch CGoluahStoryScript::CreateScriptElementList\n"));
		ret=FALSE;
	}

	delete [] tmpstr;
	SAFEDELETE(loader);

	return ret;
}

/*---------------------------------------------------------------
	�X�g�[���[�X�N���v�g�̔j��
-----------------------------------------------------------------*/
void CGoluahStoryScript::DestroyScriptElementList(SScriptElementList& scr)
{
	SScriptElementList::iterator i =scr.begin();
	SScriptElementList::iterator ie=scr.end();
	for(;i!=ie;i++)
	{
		delete (*i);
	}
	scr.clear();
}


/*---------------------------------------------------------------
	�X�g�[���[ �u���b�N ��b��`
-----------------------------------------------------------------*/
CStoryScriptElement::CStoryScriptElement()
{
	m_error = NULL;
}

CStoryScriptElement::~CStoryScriptElement()
{
	DELETEARRAY(m_error);
}

void CStoryScriptElement::Error(char *str)
{
	if(!m_error){
		m_error = new char[128];
	}
	sprintf(m_error,str);
}

void CStoryScriptElement::ErrorF(char *str,char *sub_str)
{
	if(!m_error){
		m_error = new char[128];
	}
	if(sub_str)
		sprintf(m_error,str,sub_str);
	else
		sprintf(m_error,str,_T(""));
}

void CStoryScriptElement::ErrorF(char *str,int val)
{
	if(!m_error){
		m_error = new char[128];
	}
	sprintf(m_error,str,val);
}

// #if�p�A���u���B
// �l�����Z�Ə�������̓X�^�b�N�A���ʂƘ_���a�Ȃǂ͍ċA�������g���Ǝv���B
void ParseIf(char *str)
{
}

/*---------------------------------------------------------------
	�X�g�[���[ �u���b�N�O�G���[�v�f
-----------------------------------------------------------------*/
CStoryElement_Error::CStoryElement_Error(char *err)
{
	Error(err);
}

/*---------------------------------------------------------------
	�X�g�[���[ #settings �G�������g
-----------------------------------------------------------------*/
CStoryElement_Settings::CStoryElement_Settings()
{
	//�S��
	ver=-1;							//�X�N���v�g�̃o�[�W�����i�}�C�i�X�l:���ݒ�j
	title=NULL;						//�X�g�[���[�^�C�g��
	brief=NULL;						//�X�g�[���[�T�v
	icon=NULL;						//�A�C�R��
	preview=NULL;					//�v���r���[
	continue_num=-1;				//�R���e�B�j���[�\��
	result_send=-1;					//�������ʑ��M�Ώ�

	//�L�����N�^��`
	cnum=0;							//char�w�萔
	for(int i=0;i<MAXNUM_TEAM;i++)
	{
		characters[i]=-2;			//char�w��(-1:userselect -2:�w��Ȃ�)
		color[i]=-1;				//�F�w��
		option[i]=0;				//�I�v�V�������ڎw��(userselect�ȊO�ŗL��)
		opttype[i]=Opt_UserSelect;	//�����_���w��t���O(userselect�ȊO�ŗL��)
	}

	//�X�N���v�g���ߎ��Ɏg�p
	multiline_flag=FALSE;
}

CStoryElement_Settings::~CStoryElement_Settings()
{
	DELETEARRAY(title);
	DELETEARRAY(brief);
	DELETEARRAY(icon);
	DELETEARRAY(preview);
}

void CStoryElement_Settings::FeedLine(char *str)
{
	//�������C���ɂ킽���`
	if(multiline_flag){
		if(gbl.strcheck(str,_T("brief_end")))multiline_flag=FALSE;
		else brief = gbl.MergeString(brief,str);
		return;
	}

	//�e���`�ǂݍ���
	if(gbl.strcheck(str,_T("ver"))){
		sscanf(str,_T("%s %f"),DMY_STR,&ver);
	}
	else if(gbl.strcheck(str,_T("title"))){
		DELETEARRAY(title);
		title=new char[64];
		sscanf(str,_T("%s %s"),DMY_STR,title);
	}
	else if(gbl.strcheck(str,_T("result"))){
		char *tmp = new char [64];
		sscanf(str,_T("%s %s %d"),DMY_STR,tmp,result_key);
		result_send = g_charlist.FindCharacter(tmp);
		if(result_send<0){
			ErrorF(_T("�L�����N�^�u%s�v�����܂���"),tmp);	
		}
		delete [] tmp;
		if(m_error)throw CStoryScriptException();
	}
	else if(gbl.strcheck(str,_T("continue"))){
		sscanf(str,_T("%s %d"),DMY_STR,&continue_num);
	}
	else if(gbl.strcheck(str,_T("brief_begin"))){
		multiline_flag=TRUE;
	}
	else if(gbl.strcheck(str,_T("char"))){
		Read_char(str);
	}
	else if(gbl.strcheck(str,_T("icon"))){
		DELETEARRAY(icon);
		icon=new char[64];
		sscanf(str,_T("%s %s"),DMY_STR,icon);
	}
	else if(gbl.strcheck(str,_T("preview"))){
		DELETEARRAY(preview);
		preview=new char[64];
		sscanf(str,_T("%s %s"),DMY_STR,preview);
	}
/*	else{//��s����
		char *tmp = new char [256];
		sscanf(str,_T("%s %s"),DMY_STR,tmp);
		ErrorF(_T("(settings) unknown option %s"),tmp);
		delete [] tmp;
	}*/
}

BOOL CStoryElement_Settings::Close()
{
	if(m_error)return FALSE;

	if(ver>1.1f || ver<0.0f){
		Error(_T("�o�[�W�������قȂ邩�A�܂���ver��`������܂���B"));
		return FALSE;
	}

	//�f�t�H�l�ݒ�
	if(!title){
		title=new char[32];
		sprintf(title,_T("untitled"));
	}
	if(!brief){
		brief=new char[32];
		sprintf(brief,_T("- no information -"));
	}

	//�^�C�g����'_'���X�y�[�X�Œu��
	for(UINT i=0;i<strlen(title);i++){
		if(title[i]=='_'){
			title[i]=' ';
		}
	}

	return TRUE;
}


//char �I�v�V�������
void CStoryElement_Settings::Read_char(char *str)
{
	//scan
	char** tmp_str = gbl.CreateTemporaryStrBuff(3);
	sscanf(str,_T("%s %s %s %s"),
		DMY_STR,
		tmp_str[0],//�L�����N�^
		tmp_str[1],//�F
		tmp_str[2] //�I�v�V����
		);

	try{
		if(cnum==3){
			Error(_T("char�w�肪�������܂�"));
			throw CStoryScriptException();
		}

		//�L�����N�^
		if(strcmp(tmp_str[0],_T("%userselect"))==0){
			characters[cnum] = -1;
		}
		else{
			characters[cnum] = g_charlist.FindCharacter(tmp_str[0]);
			if(characters[cnum]<0){
				ErrorF(_T("�L�����N�^�[�u%s�v�����܂���"),tmp_str[0]);
				throw CStoryScriptException();
			}
		}

		if(characters[cnum] != -1)
		{
			//�F�w��
			sscanf(tmp_str[1],_T("%d"),&color[cnum]);
			if(color[cnum]<1 || color[cnum]>3){
				ErrorF(_T("�L�����N�^�̃J���[��1-3�ȊO���w�肳��܂���(%s)"),tmp_str[1]);
				throw CStoryScriptException();
			}

			//�I�v�V�����w��
			if(strcmp(tmp_str[2],_T("%random"))==0){
				opttype[cnum] = Opt_Random;
			}
			else if(strcmp(tmp_str[2],_T("%userselect"))==0){
				opttype[cnum] = Opt_UserSelect;
			}
			else{
				option[cnum] = 0x00000000;
				for(int b=0;b<32;b++){
					if(tmp_str[2][b]=='1'){
						option[cnum] &= 0x00000001;
					}
					else if(tmp_str[2][b]!='0'){
						ErrorF(_T("2�i���I�v�V�����w��ǂݍ��݃G���[(%s)"),tmp_str[2]);
						throw CStoryScriptException();
					}
					if(b!=31)option[cnum] <<= 1;
				}
				opttype[cnum] = Opt_Fix;
			}
		}

		//OK
		cnum++;
	}
	catch(CStoryScriptException e){
		gbl.ods(_T("catch CStoryElement_Settings::Read_char\n"));
		gbl.DeleteTemporaryStrBuff(tmp_str);
		throw e;
	}
	gbl.DeleteTemporaryStrBuff(tmp_str);
}

/*---------------------------------------------------------------
	�X�g�[���[ #vs �G�������g
-----------------------------------------------------------------*/

//���\�z
CStoryElement_VS::CStoryElement_VS(CStoryElement_Settings* sets)
{
	m_sets = sets;
	multiline_flag = 0;

	//�L�����N�^�֘A���Z�b�g
	num[0] = num[1] = 0;
	for(int i=0;i<2;i++)for(int j=0;j<3;j++)
	{
		characters[i][j] = -5;//none
		color[i][j] = 0;
		option[i][j] = 0;
		is_random_opt[i][j] = TRUE;
		is_related_level[i][j] = TRUE;
	}
	
	//�����ݒ�֘A
	stage = -1;						// -1:�����_��
	type = TAISENKEISIKI_GOCYAMAZE;	// �����ΐ�
	show_win = TRUE;
	bgm_filename = NULL;
	bgm_startpos = 0;
	limit_time = -1;
	sel_shift = 0;

	//������֘A
	text_win = text_lose = NULL;
	show_win = TRUE;
	nogameover = FALSE;
}

//����n��
CStoryElement_VS::~CStoryElement_VS()
{
	DELETEARRAY(text_win);
	DELETEARRAY(text_lose);
	DELETEARRAY(bgm_filename);
}

//���X�N���v�g���
void CStoryElement_VS::FeedLine(char *str)
{
	//�������C���ɂ킽���`
	if(multiline_flag){
		if(multiline_flag==1){//�����e�L�X�g
			if(gbl.strcheck(str,_T("wintext_end")))multiline_flag=0;
			else text_win = gbl.MergeString(text_win,str);
		}
		else if(multiline_flag==2){//�����e�L�X�g
			if(gbl.strcheck(str,_T("losetext_end")))multiline_flag=0;
			else text_lose = gbl.MergeString(text_lose,str);
		}
		return;
	}

	//���̑��̎��ʎq
	if(gbl.strcheck(str,_T("type"))){
		sscanf(str,_T("%s %d"),DMY_STR,&type);
		return;
	}
	else if(gbl.strcheck(str,_T("friend"))){
		Read_friend(str);
		return;
	}
	else if(gbl.strcheck(str,_T("enemy"))){
		Read_enemy(str);
		return;
	}
	else if(gbl.strcheck(str,_T("stage"))){
		Read_stage(str);
		return;
	}
	else if(gbl.strcheck(str,_T("timelimit"))){
		sscanf(str,_T("%s %d"),DMY_STR,&limit_time);
		return;
	}
	else if(gbl.strcheck(str,_T("nowin"))){
		show_win = FALSE;
		return;
	}
	else if(gbl.strcheck(str,_T("wintext_begin"))){
		multiline_flag = 1;
		return;
	}
	else if(gbl.strcheck(str,_T("losetext_begin"))){
		multiline_flag = 2;
		return;
	}
	else if(gbl.strcheck(str,_T("bgm"))){
		DELETEARRAY( bgm_filename );
		bgm_filename = new char [128];
		if (m_sets->ver >= 1.1)
			sscanf(str,_T("%s %s %d"),DMY_STR,bgm_filename,bgm_startpos);
		else
			sscanf(str,_T("%s %s"),DMY_STR,bgm_filename);
		return;
	}
	else if(gbl.strcheck(str,_T("nogameover"))){
		nogameover = TRUE;
		return;
	}
	else if(gbl.strcheck(str,_T("sbase"))){
		sscanf(str,_T("%s %d"),DMY_STR,&sel_shift);
		return;
	}
/*	else{//��s����
		Error(_T("#vs�u���b�N���ŉ��ߕs�\�ȃI�v�V�������w�肳��܂���"));
	}*/
}

//��friend �I�v�V�������
void CStoryElement_VS::Read_friend(char *str,int team)
{
	//scan
	char** tmp_str = gbl.CreateTemporaryStrBuff(4);
	sscanf(str,_T("%s %s %s %s %s"),
		DMY_STR,
		tmp_str[0],//�L�����N�^
		tmp_str[1],//�F
		tmp_str[2],//�I�v�V����
		tmp_str[3] //COM���x��
		);

	BOOL ret = FALSE;
	try{
		if(num[team]==3){
			ErrorF(_T("%s�w�肪�������܂�"),team==0?_T("friend"):_T("enemy"));
			throw CStoryScriptException();
		}

		//�L�����N�^
		if(strcmp(tmp_str[0],_T("%random"))==0){
			characters[team][num[team]] = -1;
		}
		else if(strcmp(tmp_str[0],_T("%char1"))==0){
			if(m_sets->cnum<1){
				Error(_T("char�̒�`���s�����Ă��܂�(1)"));
				throw CStoryScriptException();
			}
			characters[team][num[team]] = -2;
		}
		else if(strcmp(tmp_str[0],_T("%char2"))==0){
			if(m_sets->cnum<2){
				Error(_T("char�̒�`���s�����Ă��܂�(2)"));
				throw CStoryScriptException();
			}
			characters[team][num[team]] = -3;
		}
		else if(strcmp(tmp_str[0],_T("%char3"))==0){
			if(m_sets->cnum<3){
				Error(_T("char�̒�`���s�����Ă��܂�(3)"));
				throw CStoryScriptException();
			}
			characters[team][num[team]] = -4;
		}
		else{
			characters[team][num[team]] = g_charlist.FindCharacter(tmp_str[0]);
			if(characters[team][num[team]]<0){
				ErrorF(_T("absence of�u%s�v"),tmp_str[0]);
				throw CStoryScriptException();
			}
		}

		if(characters[team][num[team]]>=0 || characters[team][num[team]]==-1)//char1-3�łȂ��Ƃ�
		{
			//�F�w��
			if(strcmp(tmp_str[1],_T("%auto"))==0){
				color[team][num[team]] = -1;
			}
			else{
				sscanf(tmp_str[1],_T("%d"),&color[team][num[team]]);
				if(color[team][num[team]]<1 || color[team][num[team]]>3)
				{
					ErrorF(_T("(vs) invalid color (%s)"),tmp_str[1]);
					throw CStoryScriptException();
				}
			}

			//�I�v�V�����w��
			if(strcmp(tmp_str[2],_T("%random"))==0){
				is_random_opt[team][num[team]] = TRUE;
			}
			else{
				option[team][num[team]] = 0x00000000;
				if(strlen(tmp_str[2])!=32){
					ErrorF(_T("2�i���I�v�V�����w��ǂݍ��݃G���[1(%s)"),tmp_str[2]);
					throw CStoryScriptException();
				}
				for(int b=0;b<32;b++)
				{
					option[team][num[team]] <<= 1;
					if(tmp_str[2][b]=='1'){
						option[team][num[team]] |= 0x00000001;
					}
					else if(tmp_str[2][b]!='0'){
						ErrorF(_T("2�i���I�v�V�����w��ǂݍ��݃G���[(%s)"),tmp_str[2]);
						throw CStoryScriptException();
					}
				}
			}
		}

		//���x���w��
		if(tmp_str[3][0] == '+' || tmp_str[3][0] == '-'){
			is_related_level[team][num[team]] = TRUE;
		}
		sscanf(tmp_str[3],_T("%d"),&level[team][num[team]]);

		//OK
		ret = TRUE;
		num[team]++;
	}
	catch(CStoryScriptException e){
		gbl.ods(_T("catch CStoryElement_VS::Read_friend"));
		gbl.DeleteTemporaryStrBuff(tmp_str);
		throw e;
	}

	gbl.DeleteTemporaryStrBuff(tmp_str);
}

//��enemy��`�ǂݍ���
void CStoryElement_VS::Read_enemy(char *str)
{
	//�قƂ�ǂ���Ȃ�
	Read_friend(str,1);
}

//��stage��`�ǂݍ���
void CStoryElement_VS::Read_stage(char *str)
{
	char** tmp_str = gbl.CreateTemporaryStrBuff(1);

	sscanf(str,_T("%s %s"),
		DMY_STR,
		tmp_str[0]
		);

	if(strcmp(tmp_str[0],_T("%random"))==0){
		stage = -1;
	}
	else{
		stage = g_stagelist.FindStage(tmp_str[0]);
		if(stage<0){
			ErrorF(_T("�X�e�[�W�u%s�v������܂���"),tmp_str[0]);
			gbl.DeleteTemporaryStrBuff(tmp_str);
			throw CStoryScriptException();
		}
	}
	gbl.DeleteTemporaryStrBuff(tmp_str);
}

//���u���b�N�I��������
BOOL CStoryElement_VS::Close()
{
	if(m_error)return FALSE;
	char temp[128];

	//friend�w�肪����Ȃ��ꍇ
	if(num[0]==0){
		if(m_sets->cnum==0){
			Error(_T("friend�w�肪����܂���Bsettings��char�w�肪�K�v�ł�"));
			return FALSE;
		}
		else{
			FeedLine(_T("friend %char1 - - +0"));//char1-3�̏ꍇ�A�J���[�E�I�v�V�����t�킪���������
			for(int j=1;j<m_sets->cnum;j++){
				sprintf(temp,_T("friend %%char%d - - +0"),j+1);
				FeedLine(temp);
			}
		}
	}

	//enemy�w�肪����Ȃ��ꍇ
	if(num[1]==0){
		for(int i=0;i<num[0];i++){
			FeedLine(_T("enemy %random 1 %random +0"));
		}
	}

	return TRUE;
}


/*---------------------------------------------------------------
	�X�g�[���[ #demo �G�������g
-----------------------------------------------------------------*/

CStoryElement_Demo::CStoryElement_Demo()
{
	m_filename = NULL;
}

CStoryElement_Demo::~CStoryElement_Demo()
{
	DELETEARRAY(m_filename);
}

void CStoryElement_Demo::FeedLine(char *str)
{
	char **tmp = gbl.CreateTemporaryStrBuff(2);
	sscanf(str,_T("%s %s"),
		tmp[0],
		tmp[1]
		);

	if(strcmp(tmp[0],_T("file"))==0){
		DELETEARRAY(m_filename);
		m_filename = new char [strlen(tmp[1])+1];
		strcpy(m_filename,tmp[1]);
	}

	gbl.DeleteTemporaryStrBuff(tmp);
	if(m_error)throw CStoryScriptException();
	return;
}

BOOL CStoryElement_Demo::Close()
{
	if(m_error)return FALSE;

	if(!m_filename){
		Error(_T("#demo�Ƀt�@�C�����w�肪����܂���"));
		return FALSE;
	}
	return TRUE;
}

/*---------------------------------------------------------------
	�X�g�[���[ #staff �G�������g
-----------------------------------------------------------------*/
CStoryElement_Staff::CStoryElement_Staff()
{
	m_filename=NULL;
}

CStoryElement_Staff::~CStoryElement_Staff()
{
	DELETEARRAY(m_filename);
}

void CStoryElement_Staff::FeedLine(char *str)
{
	char **tmp = gbl.CreateTemporaryStrBuff(2);
	sscanf(str,_T("%s %s"),
		tmp[0],
		tmp[1]
		);

	if(strcmp(tmp[0],_T("file"))==0){
		DELETEARRAY(m_filename);
		m_filename = new char [strlen(tmp[1])+1];
		strcpy(m_filename,tmp[1]);
	}

	gbl.DeleteTemporaryStrBuff(tmp);
	if(m_error)throw CStoryScriptException();
	return;
}
