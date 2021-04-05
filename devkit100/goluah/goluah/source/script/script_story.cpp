﻿
/*!
	@file
	@brief ストーリースクリプト操作

=================================================================*/

#include "stdafx.h"
#include "define_macro.h"
#include "script_story.h"

#include "global.h"


/*!
*	@brief ストーリースクリプトの読み込み時例外定義
*
*	ストーリースクリプトロード時に問題が起こったときに、
*	アクセス違反とかと区別するためにこのクラスを投げてるみたい。
*/
class CStoryScriptException
{
public:
	CStoryScriptException(){}
	CStoryScriptException(CStoryScriptException& e){}//!< コピーコンストラクタ
};

int story_selectflag = 0;		// 暫定設置、置き場所模索中。

/*---------------------------------------------------------------
	ストーリースクリプトの読み込み
-----------------------------------------------------------------*/
BOOL CGoluahStoryScript::CreateScriptElementList(SScriptElementList& list,char *location)
{
	CScriptLoader *loader = CScriptLoader::Create();
	char *tmpstr = new char[128];

	DestroyScriptElementList(list);

	BOOL ret =TRUE;
	CStoryElement_Settings *settings=NULL;

	try{
		if(!loader->LoadFile(location,"entry.txt")){
			list.push_back(new CStoryElement_Error("ファイルの読み込み失敗"));
			throw CStoryScriptException();
		}
		
		int line_index = 0;
		int line_max = loader->GetLineNum();
		while(line_index < line_max)
		{
			char *linestr = loader->GetLine(line_index);
			if(*linestr=='#')//ブロック定義開始
			{
				linestr++;
				sscanf(linestr,"%s",tmpstr);
				if(list.size()==0){
					if(strcmp(tmpstr,"settings")!=0){
						list.push_back(new CStoryElement_Error("settingsブロックの前に他ブロックを定義することはできません"));
						throw CStoryScriptException();
					}
				}
				else{
					if(list[list.size()-1]->Close()==FALSE){
						throw CStoryScriptException();
					}
				}
				{//各ブロックの読み込み
					if(strcmp(tmpstr,"vs")==0){
						list.push_back(new CStoryElement_VS(settings));//#vs
					}
					else if(strcmp(tmpstr,"staff")==0){
						list.push_back(new CStoryElement_Staff);//#staff
					}
					else if(strcmp(tmpstr,"demo")==0){
						list.push_back(new CStoryElement_Demo);//#demo
					}
					else if(strcmp(tmpstr,"settings")==0){
						settings = new CStoryElement_Settings;
						list.push_back(settings);//#settings
					}
					else if(strcmp(tmpstr,"select")==0){
						int num = 0;

						if (settings->ver < 1.1f) {
							list.push_back(new CStoryElement_Error("バージョン設定が古いです"));
							throw CStoryScriptException();
						}
						sscanf(linestr, "select %d", &num);
						list.push_back( new CStoryElement_Select(num) );
					}
					else if(strcmp(tmpstr,"end_select")==0){

						if (settings->ver < 1.1f) {
							list.push_back(new CStoryElement_Error("バージョン設定が古いです"));
							throw CStoryScriptException();
						}
						list.push_back( new CStoryElement_Select(-1) );
					}
					else{//#それ以外？
						char *errstr = new char[256];
						sprintf(errstr,"未知のブロック名 : %s",tmpstr);
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
		gbl.ods("catch CGoluahStoryScript::CreateScriptElementList\n");
		ret=FALSE;
	}

	delete [] tmpstr;
	SAFEDELETE(loader);

	return ret;
}

/*---------------------------------------------------------------
	ストーリースクリプトの破棄
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
	ストーリー ブロック 基礎定義
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
		sprintf(m_error,str,"");
}

void CStoryScriptElement::ErrorF(char *str,int val)
{
	if(!m_error){
		m_error = new char[128];
	}
	sprintf(m_error,str,val);
}

// #if用、仮置き。
// 四則演算と条件判定はスタック、括弧と論理和などは再帰処理を使うと思われ。
void ParseIf(char *str)
{
}

/*---------------------------------------------------------------
	ストーリー ブロック外エラー要素
-----------------------------------------------------------------*/
CStoryElement_Error::CStoryElement_Error(char *err)
{
	Error(err);
}

/*---------------------------------------------------------------
	ストーリー #settings エレメント
-----------------------------------------------------------------*/
CStoryElement_Settings::CStoryElement_Settings()
{
	//全般
	ver=-1;							//スクリプトのバージョン（マイナス値:未設定）
	title=NULL;						//ストーリータイトル
	brief=NULL;						//ストーリー概要
	icon=NULL;						//アイコン
	preview=NULL;					//プレビュー
	continue_num=-1;				//コンティニュー可能回数
	result_send=-1;					//試合結果送信対象

	//キャラクタ定義
	cnum=0;							//char指定数
	for(int i=0;i<MAXNUM_TEAM;i++)
	{
		characters[i]=-2;			//char指定(-1:userselect -2:指定なし)
		color[i]=-1;				//色指定
		option[i]=0;				//オプション直接指定(userselect以外で有効)
		opttype[i]=Opt_UserSelect;	//ランダム指定フラグ(userselect以外で有効)
	}

	//スクリプト解釈時に使用
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
	//複数ラインにわたる定義
	if(multiline_flag){
		if(gbl.strcheck(str,"brief_end"))multiline_flag=FALSE;
		else brief = gbl.MergeString(brief,str);
		return;
	}

	//各種定義読み込み
	if(gbl.strcheck(str,"ver")){
		sscanf(str,"%s %f",DMY_STR,&ver);
	}
	else if(gbl.strcheck(str,"title")){
		DELETEARRAY(title);
		title=new char[64];
		sscanf(str,"%s %s",DMY_STR,title);
	}
	else if(gbl.strcheck(str,"result")){
		char *tmp = new char [64];
		sscanf(str,"%s %s %d",DMY_STR,tmp,result_key);
		result_send = g_charlist.FindCharacter(tmp);
		if(result_send<0){
			ErrorF("キャラクタ「%s」がいません",tmp);	
		}
		delete [] tmp;
		if(m_error)throw CStoryScriptException();
	}
	else if(gbl.strcheck(str,"continue")){
		sscanf(str,"%s %d",DMY_STR,&continue_num);
	}
	else if(gbl.strcheck(str,"brief_begin")){
		multiline_flag=TRUE;
	}
	else if(gbl.strcheck(str,"char")){
		Read_char(str);
	}
	else if(gbl.strcheck(str,"icon")){
		DELETEARRAY(icon);
		icon=new char[64];
		sscanf(str,"%s %s",DMY_STR,icon);
	}
	else if(gbl.strcheck(str,"preview")){
		DELETEARRAY(preview);
		preview=new char[64];
		sscanf(str,"%s %s",DMY_STR,preview);
	}
/*	else{//空行かも
		char *tmp = new char [256];
		sscanf(str,"%s %s",DMY_STR,tmp);
		ErrorF("(settings) unknown option %s",tmp);
		delete [] tmp;
	}*/
}

BOOL CStoryElement_Settings::Close()
{
	if(m_error)return FALSE;

	if(ver>1.1f || ver<0.0f){
		Error("バージョンが異なるか、またはver定義がありません。");
		return FALSE;
	}

	//デフォ値設定
	if(!title){
		title=new char[32];
		sprintf(title,"untitled");
	}
	if(!brief){
		brief=new char[32];
		sprintf(brief,"- no information -");
	}

	//タイトルの'_'をスペースで置換
	for(UINT i=0;i<strlen(title);i++){
		if(title[i]=='_'){
			title[i]=' ';
		}
	}

	return TRUE;
}


//char オプション介錯
void CStoryElement_Settings::Read_char(char *str)
{
	//scan
	char** tmp_str = gbl.CreateTemporaryStrBuff(3);
	sscanf(str,"%s %s %s %s",
		DMY_STR,
		tmp_str[0],//キャラクタ
		tmp_str[1],//色
		tmp_str[2] //オプション
		);

	try{
		if(cnum==3){
			Error("char指定が多すぎます");
			throw CStoryScriptException();
		}

		//キャラクタ
		if(strcmp(tmp_str[0],"%userselect")==0){
			characters[cnum] = -1;
		}
		else{
			characters[cnum] = g_charlist.FindCharacter(tmp_str[0]);
			if(characters[cnum]<0){
				ErrorF("キャラクター「%s」がいません",tmp_str[0]);
				throw CStoryScriptException();
			}
		}

		if(characters[cnum] != -1)
		{
			//色指定
			sscanf(tmp_str[1],"%d",&color[cnum]);
			if(color[cnum]<1 || color[cnum]>3){
				ErrorF("キャラクタのカラーに1-3以外が指定されました(%s)",tmp_str[1]);
				throw CStoryScriptException();
			}

			//オプション指定
			if(strcmp(tmp_str[2],"%random")==0){
				opttype[cnum] = Opt_Random;
			}
			else if(strcmp(tmp_str[2],"%userselect")==0){
				opttype[cnum] = Opt_UserSelect;
			}
			else{
				option[cnum] = 0x00000000;
				for(int b=0;b<32;b++){
					if(tmp_str[2][b]=='1'){
						option[cnum] &= 0x00000001;
					}
					else if(tmp_str[2][b]!='0'){
						ErrorF("2進数オプション指定読み込みエラー(%s)",tmp_str[2]);
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
		gbl.ods("catch CStoryElement_Settings::Read_char\n");
		gbl.DeleteTemporaryStrBuff(tmp_str);
		throw e;
	}
	gbl.DeleteTemporaryStrBuff(tmp_str);
}

/*---------------------------------------------------------------
	ストーリー #vs エレメント
-----------------------------------------------------------------*/

//■構築
CStoryElement_VS::CStoryElement_VS(CStoryElement_Settings* sets)
{
	m_sets = sets;
	multiline_flag = 0;

	//キャラクタ関連リセット
	num[0] = num[1] = 0;
	for(int i=0;i<2;i++)for(int j=0;j<3;j++)
	{
		characters[i][j] = -5;//none
		color[i][j] = 0;
		option[i][j] = 0;
		is_random_opt[i][j] = TRUE;
		is_related_level[i][j] = TRUE;
	}
	
	//試合設定関連
	stage = -1;						// -1:ランダム
	type = TAISENKEISIKI_GOCYAMAZE;	// 同時対戦
	show_win = TRUE;
	bgm_filename = NULL;
	bgm_startpos = 0;
	limit_time = -1;
	sel_shift = 0;

	//試合後関連
	text_win = text_lose = NULL;
	show_win = TRUE;
	nogameover = FALSE;
}

//■後始末
CStoryElement_VS::~CStoryElement_VS()
{
	DELETEARRAY(text_win);
	DELETEARRAY(text_lose);
	DELETEARRAY(bgm_filename);
}

//■スクリプト介錯
void CStoryElement_VS::FeedLine(char *str)
{
	//複数ラインにわたる定義
	if(multiline_flag){
		if(multiline_flag==1){//勝利テキスト
			if(gbl.strcheck(str,"wintext_end"))multiline_flag=0;
			else text_win = gbl.MergeString(text_win,str);
		}
		else if(multiline_flag==2){//負けテキスト
			if(gbl.strcheck(str,"losetext_end"))multiline_flag=0;
			else text_lose = gbl.MergeString(text_lose,str);
		}
		return;
	}

	//その他の識別子
	if(gbl.strcheck(str,"type")){
		sscanf(str,"%s %d",DMY_STR,&type);
		return;
	}
	else if(gbl.strcheck(str,"friend")){
		Read_friend(str);
		return;
	}
	else if(gbl.strcheck(str,"enemy")){
		Read_enemy(str);
		return;
	}
	else if(gbl.strcheck(str,"stage")){
		Read_stage(str);
		return;
	}
	else if(gbl.strcheck(str,"timelimit")){
		sscanf(str,"%s %d",DMY_STR,&limit_time);
		return;
	}
	else if(gbl.strcheck(str,"nowin")){
		show_win = FALSE;
		return;
	}
	else if(gbl.strcheck(str,"wintext_begin")){
		multiline_flag = 1;
		return;
	}
	else if(gbl.strcheck(str,"losetext_begin")){
		multiline_flag = 2;
		return;
	}
	else if(gbl.strcheck(str,"bgm")){
		DELETEARRAY( bgm_filename );
		bgm_filename = new char [128];
		if (m_sets->ver >= 1.1)
			sscanf(str,"%s %s %d",DMY_STR,bgm_filename,bgm_startpos);
		else
			sscanf(str,"%s %s",DMY_STR,bgm_filename);
		return;
	}
	else if(gbl.strcheck(str,"nogameover")){
		nogameover = TRUE;
		return;
	}
	else if(gbl.strcheck(str,"sbase")){
		sscanf(str,"%s %d",DMY_STR,&sel_shift);
		return;
	}
/*	else{//空行かも
		Error("#vsブロック内で解釈不可能なオプションが指定されました");
	}*/
}

//■friend オプション介錯
void CStoryElement_VS::Read_friend(char *str,int team)
{
	//scan
	char** tmp_str = gbl.CreateTemporaryStrBuff(4);
	sscanf(str,"%s %s %s %s %s",
		DMY_STR,
		tmp_str[0],//キャラクタ
		tmp_str[1],//色
		tmp_str[2],//オプション
		tmp_str[3] //COMレベル
		);

	BOOL ret = FALSE;
	try{
		if(num[team]==3){
			ErrorF("%s指定が多すぎます",team==0?"friend":"enemy");
			throw CStoryScriptException();
		}

		//キャラクタ
		if(strcmp(tmp_str[0],"%random")==0){
			characters[team][num[team]] = -1;
		}
		else if(strcmp(tmp_str[0],"%char1")==0){
			if(m_sets->cnum<1){
				Error("charの定義が不足しています(1)");
				throw CStoryScriptException();
			}
			characters[team][num[team]] = -2;
		}
		else if(strcmp(tmp_str[0],"%char2")==0){
			if(m_sets->cnum<2){
				Error("charの定義が不足しています(2)");
				throw CStoryScriptException();
			}
			characters[team][num[team]] = -3;
		}
		else if(strcmp(tmp_str[0],"%char3")==0){
			if(m_sets->cnum<3){
				Error("charの定義が不足しています(3)");
				throw CStoryScriptException();
			}
			characters[team][num[team]] = -4;
		}
		else{
			characters[team][num[team]] = g_charlist.FindCharacter(tmp_str[0]);
			if(characters[team][num[team]]<0){
				ErrorF("absence of「%s」",tmp_str[0]);
				throw CStoryScriptException();
			}
		}

		if(characters[team][num[team]]>=0 || characters[team][num[team]]==-1)//char1-3でないとき
		{
			//色指定
			if(strcmp(tmp_str[1],"%auto")==0){
				color[team][num[team]] = -1;
			}
			else{
				sscanf(tmp_str[1],"%d",&color[team][num[team]]);
				if(color[team][num[team]]<1 || color[team][num[team]]>3)
				{
					ErrorF("(vs) invalid color (%s)",tmp_str[1]);
					throw CStoryScriptException();
				}
			}

			//オプション指定
			if(strcmp(tmp_str[2],"%random")==0){
				is_random_opt[team][num[team]] = TRUE;
			}
			else{
				option[team][num[team]] = 0x00000000;
				if(strlen(tmp_str[2])!=32){
					ErrorF("2進数オプション指定読み込みエラー1(%s)",tmp_str[2]);
					throw CStoryScriptException();
				}
				for(int b=0;b<32;b++)
				{
					option[team][num[team]] <<= 1;
					if(tmp_str[2][b]=='1'){
						option[team][num[team]] |= 0x00000001;
					}
					else if(tmp_str[2][b]!='0'){
						ErrorF("2進数オプション指定読み込みエラー(%s)",tmp_str[2]);
						throw CStoryScriptException();
					}
				}
			}
		}

		//レベル指定
		if(tmp_str[3][0] == '+' || tmp_str[3][0] == '-'){
			is_related_level[team][num[team]] = TRUE;
		}
		sscanf(tmp_str[3],"%d",&level[team][num[team]]);

		//OK
		ret = TRUE;
		num[team]++;
	}
	catch(CStoryScriptException e){
		gbl.ods("catch CStoryElement_VS::Read_friend");
		gbl.DeleteTemporaryStrBuff(tmp_str);
		throw e;
	}

	gbl.DeleteTemporaryStrBuff(tmp_str);
}

//■enemy定義読み込み
void CStoryElement_VS::Read_enemy(char *str)
{
	//ほとんどおんなじ
	Read_friend(str,1);
}

//■stage定義読み込み
void CStoryElement_VS::Read_stage(char *str)
{
	char** tmp_str = gbl.CreateTemporaryStrBuff(1);

	sscanf(str,"%s %s",
		DMY_STR,
		tmp_str[0]
		);

	if(strcmp(tmp_str[0],"%random")==0){
		stage = -1;
	}
	else{
		stage = g_stagelist.FindStage(tmp_str[0]);
		if(stage<0){
			ErrorF("ステージ「%s」がありません",tmp_str[0]);
			gbl.DeleteTemporaryStrBuff(tmp_str);
			throw CStoryScriptException();
		}
	}
	gbl.DeleteTemporaryStrBuff(tmp_str);
}

//■ブロック終了時処理
BOOL CStoryElement_VS::Close()
{
	if(m_error)return FALSE;
	char temp[128];

	//friend指定が一つもない場合
	if(num[0]==0){
		if(m_sets->cnum==0){
			Error("friend指定がありません。settingsでchar指定が必要です");
			return FALSE;
		}
		else{
			FeedLine("friend %char1 - - +0");//char1-3の場合、カラー・オプション師弟が無視される
			for(int j=1;j<m_sets->cnum;j++){
				sprintf(temp,"friend %%char%d - - +0",j+1);
				FeedLine(temp);
			}
		}
	}

	//enemy指定が一つもない場合
	if(num[1]==0){
		for(int i=0;i<num[0];i++){
			FeedLine("enemy %random 1 %random +0");
		}
	}

	return TRUE;
}


/*---------------------------------------------------------------
	ストーリー #demo エレメント
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
	sscanf(str,"%s %s",
		tmp[0],
		tmp[1]
		);

	if(strcmp(tmp[0],"file")==0){
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
		Error("#demoにファイル名指定がありません");
		return FALSE;
	}
	return TRUE;
}

/*---------------------------------------------------------------
	ストーリー #staff エレメント
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
	sscanf(str,"%s %s",
		tmp[0],
		tmp[1]
		);

	if(strcmp(tmp[0],"file")==0){
		DELETEARRAY(m_filename);
		m_filename = new char [strlen(tmp[1])+1];
		strcpy(m_filename,tmp[1]);
	}

	gbl.DeleteTemporaryStrBuff(tmp);
	if(m_error)throw CStoryScriptException();
	return;
}
