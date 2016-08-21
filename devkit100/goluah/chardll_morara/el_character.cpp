/*=====================================================================================

Goluah!! Copyright (C) 2001-2004 aki, 2004-2016 At-sushi, 2014-2016 logger

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

=======================================================================================*/

/*=====================================================================================

	キャラクター定義

=======================================================================================*/
#include "el_character.h"

//**************************************************************************************
//  Option Settings
//　ここに、キャラクターに設定できるオプションを指定します
//**************************************************************************************

#if 0
/*
CHARACTER_LOAD_OPTION option[] = {
	// キャラクターのオプションをここに書いてください。（最大32個）
	// なお、オプションとオプションの間はコンマ(,)で区切ってください。

	// 記入内容：
	// 　{ フラグ, 競合するｵﾌﾟｼｮﾝ,依存するｵﾌﾟｼｮﾝ, オプション名, 消費ポイント }
	{ OPTIONS_CHAIN_COMBO		,0, 0, "CHAIN COMBO",		5 } , 
	{ OPTIONS_TETRAGRAMMATON	,0, 0, "TertaGrammaton",	7 } ,
	{ OPTIONS_SWORDOFJUSTICE	,0, 0, "Sword Of Justice",	5 } ,
	{ OPTIONS_DAYOFJUDGEMENT	,0, 0, "Day Of Judgement",	4 } ,
	{ OPTIONS_MEGIDORAON		,0, 0, "Megidoraon",		4 } ,
	{ OPTIONS_ENERGY_SOURCE		,0, 0, "Enegy+",			5 } ,
	{ OPTIONS_ENERGY_SOURCE2	,0, OPTIONS_ENERGY_SOURCE, "Enegy++",	5 } ,
	{ OPTIONS_ATTACK30			,0, 0, "Attack +30%",		7 } ,
	{ OPTIONS_AUTOGUARD			,0, 0, "Auto Guard",		5 } ,
//	{ OPTIONS_REGENERATOR		,OPTIONS_HP50, 0, "Regenerator",		15 } ,
	{ OPTIONS_HP50				,0, 0, "HP -50%",			-15 } ,
};

//**************************************************************************************
//  Character Information
//　ここにキャラクターの名前を記述します
//**************************************************************************************

CCharacterInfo CharInfo("モララエル",		// キャラクターの名前（最大31バイト）
						CDI_VERSION,		// キャラクターDLLインターフェイスのバージョン
						option,				// オプション構造体へのポインタ
						sizeof(option) / sizeof(CHARACTER_LOAD_OPTION), // オプションの数
						20,					// オプション最大ポイント数
						FALSE);				// ネット対戦対応かどうか 将来の拡張のために用意されている。現在は0を渡さなければならない（笑）

char* CharDirectory = NULL;

//設定マクロ。CCharacterInfo変数名↑と、キャラクタークラス名を指定します
SET_CHARACTERINFO(CharInfo, CCharacter)
*/
#endif


CCharacterBase* CreateMorarael(CDI_CHARACTERINFO2 *info)
{
	return new CCharacterEL(info);
}


/*--------------------------------------------------------------------------------------
	キャラクター生成処
	通常、いじる必要ありません
----------------------------------------------------------------------------------------*/

CCharacterEL::CCharacterEL(CDI_CHARACTERINFO2 *info) : CCharacterBase(info)
{
	CharDirectory = info->dir;	// キャラクターのフォルダを所得

	//aki用エフェクト初期化
	aki3d.Initialize( 
					CGoluahObject::GetD3DDevice() ,
					CharDirectory
					);
#	ifdef USE_GOLUAH_COMMON_DLL
		srand(GetTickCount());
#	endif // USE_GOLUAH_COMMON_DLL
}

/*--------------------------------------------------------------------------------------
	キャラクター破棄処理
	通常、いじる必要ありません
----------------------------------------------------------------------------------------*/
CCharacterEL::~CCharacterEL()
{
}

/*--------------------------------------------------------------------------------------
	パラメータ初期化
	ここに、各種パラメータの初期化処理を追加します
----------------------------------------------------------------------------------------*/
void CCharacterEL::InitParameters()
{
	InitAnalyzeOptions();

	pdat->hpmax			= 1400;//最大体力値
	pdat->gaugemax		= 4;//最大ゲージ
	pdat->gaugetype		= GOBJFLGA_POWGAUGE1;//ゲージタイプ
}

void CCharacterEL::InitAnalyzeOptions()
{
/*	chainComboEnabled	= (option_flags&OPTIONS_CHAIN_COMBO) ? TRUE : FALSE ;
	m_opt_tetra			= (option_flags&OPTIONS_TETRAGRAMMATON) ? TRUE : FALSE ;
	m_opt_sword			= (option_flags&OPTIONS_SWORDOFJUSTICE) ? TRUE : FALSE ;
	m_opt_judge			= (option_flags&OPTIONS_DAYOFJUDGEMENT) ? TRUE : FALSE ;
	m_opt_megido		= (option_flags&OPTIONS_MEGIDORAON) ? TRUE : FALSE ;
//	m_opt_regene		= (option_flags&OPTIONS_REGENERATOR) ? TRUE : FALSE ;
	m_opt_regene		= FALSE;

	m_power_rate = 0.0005f;
	if(option_flags&OPTIONS_ENERGY_SOURCE ) m_power_rate += 0.0005f;
	if(option_flags&OPTIONS_ENERGY_SOURCE2) m_power_rate += 0.0005f;

	m_opt_attackPlus	= (option_flags&OPTIONS_ATTACK30) ? TRUE : FALSE ;
	m_opt_hp50			= (option_flags&OPTIONS_HP50) ? TRUE : FALSE ;

	isAutoGuard			= (option_flags&OPTIONS_AUTOGUARD) ? TRUE : FALSE ;

	#ifdef _DEBUG
	char *dbgmsg = new char[256];
	sprintf(dbgmsg,"morarael: chainComboEnabled %s\n", chainComboEnabled ? "TRUE" : "FALSE");
	OutputDebugString(dbgmsg);
	sprintf(dbgmsg,"morarael: tetragrammaton %s\n", m_opt_tetra ? "TRUE" : "FALSE");
	OutputDebugString(dbgmsg);
	sprintf(dbgmsg,"morarael: sword %s\n", m_opt_sword ? "TRUE" : "FALSE");
	OutputDebugString(dbgmsg);
	sprintf(dbgmsg,"morarael: judge %s\n", m_opt_judge ? "TRUE" : "FALSE");
	OutputDebugString(dbgmsg);
	sprintf(dbgmsg,"morarael: megido %s\n", m_opt_megido ? "TRUE" : "FALSE");
	OutputDebugString(dbgmsg);
	sprintf(dbgmsg,"morarael: regene %s\n", m_opt_regene ? "TRUE" : "FALSE");
	OutputDebugString(dbgmsg);
	sprintf(dbgmsg,"morarael: power1 %s\n", (option_flags&OPTIONS_ENERGY_SOURCE ) ? "TRUE" : "FALSE");
	OutputDebugString(dbgmsg);
	sprintf(dbgmsg,"morarael: power2 %s\n", (option_flags&OPTIONS_ENERGY_SOURCE2) ? "TRUE" : "FALSE");
	OutputDebugString(dbgmsg);
	sprintf(dbgmsg,"morarael: attackPlus %s\n", m_opt_attackPlus ? "TRUE" : "FALSE");
	OutputDebugString(dbgmsg);
	sprintf(dbgmsg,"morarael: hp50 %s\n", m_opt_hp50 ? "TRUE" : "FALSE");
	OutputDebugString(dbgmsg);
	delete [] dbgmsg;
	#endif
	*/

	m_opt_tetra		=TRUE;
	m_opt_sword		=TRUE;
	m_opt_judge		=TRUE;
	m_opt_megido	=TRUE;
	m_opt_regene	=FALSE;
	m_regene_rate	=4;
	m_power_rate	=0.002f;
	m_opt_attackPlus=FALSE;
	m_opt_hp50		=FALSE;
	m_event_sw = FALSE;
	m_1st_update = TRUE;

}


/*-----------------------------------------------------------------------------
	GCD/BMP ロード
-------------------------------------------------------------------------------*/
void CCharacterEL::InitGCDandBMP()
{
	int i;
	GOBJECT* pdat = GetGObject();
	if(pdat==NULL)return;
	char filename[256],palname[256];

#ifdef _DEBUG
	if(pal_number==0)
		OutputDebugString("キャラDLL [warning] : パレット番号が0です。");
#endif

	//"morarael/image?.bmp" を読み込む
	sprintf(palname,"%s\\morarael\\pal%d",g_chardir,pal_number);
	for(i=0;i<GCDMAX_IMAGES;i++){
		sprintf(filename,"%s\\morarael\\image%d",g_chardir,i+1);
		bitmaps[i] = (MYSURFACE*) LoadImage(filename,palname) ;
	}
		
	//システム側で自動判別します
	sprintf(filename,"%s\\morarael\\cell",g_chardir);
	CreateCellData2(filename, &cells, &rects, &hantei);

	//pdatに入れておく
	pdat->phdat			= hantei;
	pdat->pmsarr		= bitmaps;
	pdat->pcdat			= cells;
	pdat->prdat			= rects;
}


void CCharacterEL::InitMySound(char* path_name,char* list_name)
{
	CCharacterBase::InitMySound("morarael\\sound");
}


/*--------------------------------------------------------------------------------------
	メッセージ処理
	ベースクラスで拾っていないメッセージを拾いたい場合は、ココに処理を追加します
----------------------------------------------------------------------------------------*/
DWORD CCharacterEL::Message(DWORD msg,LPVOID dat,DWORD prm)
{
	switch(msg){
	case GOBJMSG_NAGE:
		{
			GOBJECT* pobjdat = GetInfo(prm);
			if(pobjdat==NULL)return(0);
			return(eact_nage(pobjdat));
		}
		break;
	case GOBJMSG_EARROWHIT:
		if(IsLocalCom() && ComLevelCk(3) && pdat->gauge >= 1.0)
		{
			GOBJECT* pedat = GetInfo(prm);

			if ( pedat && !(pedat->aid & ACTID_KUCYU) )
			{
				if (pdat->aid & ACTID_KUCYU) SetComAct(ACTID_JUDGEMENT,1);
			}
		}
		break;
	default:
		return(CCharacterBase::Message(msg,dat,prm));//キャラクター基本クラスに処理を任せる
	}
	return(TRUE);
}


/*--------------------------------------------------------------------------------------
	コマンド判定 処理
	GOBJMSG_COMMAND メッセージに対応します。現在の行動によって複数の関数に
	処理が分かれています。適切なところに判定処理を追加してください。
----------------------------------------------------------------------------------------*/
BOOL CCharacterEL::Command_Hissatu(DWORD key)
{
	//必殺技コマンド判定
	GOBJECT* pdat = GetGObject();

	int i;
	
	//エナジー・アローが出せる常態か
	BOOL earrow_ok;
	earrow_ok = TRUE;
	for(i=0;i<EARROW_NUM;i++){
		if(!pbul_earrow[i]->isReady()){
			earrow_ok = FALSE;
			break;
		}
	}
	if(!m_opt_tetra || pdat->gauge<NEEDGAUGE_TETRA )earrow_ok=FALSE;


	if(pdat->aid & ACTID_KUCYU)//空中必殺技のコマンド判定
	{
		//デイ・オブ・ジャッジメント
		if(com6426(50) && m_opt_judge && (pdat->gauge>=NEEDGAUGE_DOJ) )
		{
			if(key & KEYSTA_BC2){
				pdat->aid = ACTID_JUDGEMENT;
				return TRUE;
			}
		}
		//テトラグラマトン（空中）
		if(earrow_ok)
		{
			if(com236(30)){
				if(key & KEYSTA_BC2){
					pdat->aid = ACTID_EARROW3J;return TRUE;
				}
				else if(key & KEYSTA_BB2){
					pdat->aid = ACTID_EARROW2J;return TRUE;
				}
				else if(key & KEYSTA_BA2){
					pdat->aid = ACTID_EARROW1J;return TRUE;
				}
			}
		}
	}
	else//地上必殺技のコマンド判定
	{
		//ソードオブジャスティス
		if(com214(30) && m_opt_sword && pdat->gauge>=NEEDGAUGE_SOJ){
			if(key & KEYSTA_BC2){
				pdat->aid = ACTID_SWORD3R;return TRUE;
			}
			else if(key & KEYSTA_BB2){
				pdat->aid = ACTID_SWORD2R;return TRUE;
			}
			else if(key & KEYSTA_BA2){
				pdat->aid = ACTID_SWORD1R;return TRUE;
			}
		}

		//エナジーアロー
		if(earrow_ok)
		{
			if(com236(30)){
				if(key & KEYSTA_BC2){
					pdat->aid = ACTID_EARROW3;return TRUE;
				}
				else if(key & KEYSTA_BB2){
					pdat->aid = ACTID_EARROW2;return TRUE;
				}
				else if(key & KEYSTA_BA2){
					pdat->aid = ACTID_EARROW1;return TRUE;
				}
			}
		}
	}

	return CCharacterBase::Command_Hissatu(key);
}

BOOL CCharacterEL::Command_OnHissatuAttacking(DWORD key)
{
	if(!(pdat->aid & ACTID_KUCYU))
	{
		//ソードオブジャスティス キャンセル 
		if(com214(30) && m_opt_sword && pdat->gauge>=NEEDGAUGE_SOJ)
		{
			//if(pdat->counter<SOJ_OFFSET)
			{
				//強→弱or中
				if(pdat->aid==ACTID_SWORD3D)
				{
					if((key & KEYSTA_BB2) && ChainCombo(CHAIN_SWORD2) ){
						pdat->aid = ACTID_SWORD2A;
						AddPowerGauge( -1 * NEEDGAUGE_SOJ );
						return TRUE;
					}
					else if((key & KEYSTA_BA2) && ChainCombo(CHAIN_SWORD1)){
						pdat->aid = ACTID_SWORD1A;
						AddPowerGauge( -1 * NEEDGAUGE_SOJ );
						return TRUE;
					}
				}

				//中→弱or強
				if(pdat->aid==ACTID_SWORD2D)
				{
					if((key & KEYSTA_BC2) && ChainCombo(CHAIN_SWORD3)){
						pdat->aid = ACTID_SWORD3A;
						AddPowerGauge( -1 * NEEDGAUGE_SOJ );
						return TRUE;
					}
					else if((key & KEYSTA_BA2) && ChainCombo(CHAIN_SWORD1)){
						pdat->aid = ACTID_SWORD1A;
						AddPowerGauge( -1 * NEEDGAUGE_SOJ );
						return TRUE;
					}
				}

				//弱→中or強
				if(pdat->aid==ACTID_SWORD1D)
				{
					if((key & KEYSTA_BC2) && ChainCombo(CHAIN_SWORD3)){
						pdat->aid = ACTID_SWORD3A;
						AddPowerGauge( -1 * NEEDGAUGE_SOJ );
						return TRUE;
					}
					else if((key & KEYSTA_BB2) && ChainCombo(CHAIN_SWORD2)){
						pdat->aid = ACTID_SWORD2A;
						AddPowerGauge( -1 * NEEDGAUGE_SOJ );
						return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
}

BOOL CCharacterEL::Command_Normal(DWORD key)
{
	//ダッシュ
	BOOL mkprv = pdat->muki;
	if(com66(30) )
	{
		Furimuki();
		pdat->aid= (pdat->muki==mkprv) ? ACTID_DASHF : ACTID_DASHB;
		return TRUE;
	}
	if(com44(30) )
	{
		Furimuki();
		pdat->aid= (pdat->muki==mkprv) ? ACTID_DASHB : ACTID_DASHF;
		return TRUE;
	}

	return CCharacterBase::Command_Normal(key);
}

BOOL CCharacterEL::Command_OnNormal(DWORD key)
{
	GOBJECT* pdat = GetGObject();

	BOOL mkprv;

	if(pdat->aid & ACTID_KUCYU)//ジャンプ動作中
	{
		//投げ（空中）
		if(key & KEYSTA_BC2){
			if((pdat->y<-50) && (key & KEYSTA_FOWORD) && com_nage2(40))
			{
				if(ObjCatch(pdat->eid,GOBJMSG_NAGE))
				{
					pdat->aid = ACTID_NAGE2;
					return TRUE;
				}
			}
		}

		//空中ダッシュ
		mkprv = pdat->muki;
		if(pdat->y<-50 && com66(30) && (key&KEYSTA_FOWORD2) )
		{
			Furimuki();
			pdat->aid= (pdat->muki==mkprv) ? ACTID_DASHFJ : ACTID_DASHBJ;
			return TRUE;
		}
		if(pdat->y<-50 && com44(30) && (key&KEYSTA_BACK2) )
		{
			Furimuki();
			pdat->aid= (pdat->muki==mkprv) ? ACTID_DASHBJ : ACTID_DASHFJ;
			return TRUE;
		}
	}
	else{//地上動作中
		//投げ（地上）
		if (key & KEYSTA_FOWORD && key & KEYSTA_BC2 &&
			NageHantei(40) && ObjCatch(pdat->eid, GOBJMSG_NAGE))
		{
				pdat->aid = ACTID_NAGE1;
				return TRUE;
		}
	}

	if(CCharacterBase::Command_OnNormal(key))return TRUE;//通常攻撃の判定

	//ダッシュ継続判定
	if(pdat->aid & ACTID_KUCYU)
	{
		if(pdat->aid==ACTID_DASHF){
			if(!(key&KEYSTA_FOWORD))ChangeAction( ACTID_RAKKA2 );
			return TRUE;
		}
		if(pdat->aid==ACTID_DASHB){
			if(!(key&KEYSTA_BACK))ChangeAction( ACTID_RAKKA2 );
			return TRUE;
		}
		if(pdat->aid==ACTID_DASHFJ){
			if(!(key&KEYSTA_FOWORD) && pdat->counter>5)ChangeAction( ACTID_RAKKA2 );
			return TRUE;
		}
		if(pdat->aid==ACTID_DASHBJ){
			if(!(key&KEYSTA_BACK) && pdat->counter>5)ChangeAction( ACTID_RAKKA2 );
			return TRUE;
		}
	}

	return FALSE;
}

//空中投げコマンド判定
BOOL CCharacterEL::com_nage2(DWORD maai)
{
	DWORD hm;
	GOBJECT *pedat= GetInfo(pdat->eid);
	if(pedat==NULL)return(FALSE);

	if(!(pedat->aid & ACTID_KUCYU))//相手が空中判定でない
		return(FALSE);

	hm = GetMaai_H(pdat->id,pedat->id);
	if(hm > maai)
		return(FALSE);
	DWORD vm = GetMaai_V(pdat->id,pedat->id);
	if(vm > maai)
		return(FALSE);

	return(TRUE);
}

/*--------------------------------------------------------------------------------------
	COMMAND_COM
----------------------------------------------------------------------------------------*/

DWORD CCharacterEL::CommandCOM(DWORD wid)
{
	if(wid==ACTID_JUDGEMENT && pdat->gauge<1.0f)return FALSE;//ゲージチェック
	return CCharacterBase::CommandCOM(wid);
}


DWORD CCharacterEL::CmdCom_OnHissatuAttacking(DWORD wid)
{
	//ソードオブジャスティスキャンセル
	{
		if(wid==ACTID_SWORD3A && ChainCombo(CHAIN_SWORD3))
		{
			if(pdat->aid==ACTID_SWORD2D || pdat->aid==ACTID_SWORD1D)
			{
				if(pdat->gauge<NEEDGAUGE_SOJ || !m_opt_sword)return FALSE;
				AddPowerGauge( -1 * NEEDGAUGE_SOJ );
				return TRUE;
			}
		}
		if(wid==ACTID_SWORD2A && ChainCombo(CHAIN_SWORD2))
		{
			if(pdat->aid==ACTID_SWORD3D || pdat->aid==ACTID_SWORD1D)
			{
				if(pdat->gauge<NEEDGAUGE_SOJ || !m_opt_sword)return FALSE;
				AddPowerGauge( -1 * NEEDGAUGE_SOJ );
				return TRUE;
			}
		}
		if(wid==ACTID_SWORD1A && ChainCombo(CHAIN_SWORD1))
		{
			if(pdat->aid==ACTID_SWORD3D || pdat->aid==ACTID_SWORD2D)
			{
				if(pdat->gauge<NEEDGAUGE_SOJ || !m_opt_sword)return FALSE;
				AddPowerGauge( -1 * NEEDGAUGE_SOJ );
				return TRUE;
			}
		}
	}

	return CCharacterBase::CmdCom_OnHissatuAttacking(wid);
}

DWORD CCharacterEL::CmdCom_OnNormal(DWORD wid)
{
	// ジャッジメントは空中用なので、ダメ。
	if(wid==ACTID_JUDGEMENT)
		return FALSE;

	//テトラグラマトン
	BOOL earrow_ok;
	earrow_ok = TRUE;
	for(UINT i=0;i<EARROW_NUM;i++){
		if(!pbul_earrow[i]->isReady()){
			earrow_ok = FALSE;
			break;
		}
	}
	if(!m_opt_tetra || pdat->gauge<NEEDGAUGE_TETRA )earrow_ok=FALSE;
	if(wid==ACTID_EARROW1)if(pdat->gauge<NEEDGAUGE_TETRA || !earrow_ok)return FALSE;

	//ソードオブジャスティス
	if(wid==ACTID_SWORD1R || wid==ACTID_SWORD2R || wid==ACTID_SWORD3R)
	{
		if(pdat->gauge<NEEDGAUGE_SOJ || !m_opt_sword)return FALSE;
		return TRUE;
	}
	
	//ソードオブジャスティス（キャンセル）
	if(wid==ACTID_SWORD1A || wid==ACTID_SWORD2A || wid==ACTID_SWORD3A)
	{
		return FALSE;
	}

	//投げ
	if(!(pdat->aid&ACTID_KUCYU) && !(pdat->aid&ACTID_ATTACK))
	{
		if (wid==ACTID_NAGE1)
		{
			if(NageHantei(MAAI_NAGE) && ObjCatch(pdat->eid, GOBJMSG_NAGE)) {
				return TRUE;
			}
			return FALSE;
		}
	}

	// 空中ダッシュ
	if (wid == ACTID_DASHFJ || wid == ACTID_DASHBJ)
		return FALSE;

	return CCharacterBase::CmdCom_OnNormal(wid);
}


DWORD CCharacterEL::CmdCom_OnKucyu(DWORD wid)
{
	if(wid==ACTID_EARROW1J)
	{
		//テトラグラマトン
		BOOL earrow_ok;
		earrow_ok = TRUE;
		for(UINT i=0;i<EARROW_NUM;i++){
			if(!pbul_earrow[i]->isReady()){
				earrow_ok = FALSE;
				break;
			}
		}
		if(!m_opt_tetra || pdat->gauge<NEEDGAUGE_TETRA )earrow_ok=FALSE;
		if(pdat->gauge<NEEDGAUGE_TETRA || !earrow_ok)return FALSE;
	}

	//空中投げ
	if(!(pdat->aid&ACTID_ATTACK))
	{
		if (wid==ACTID_NAGE2)
		{
			if(com_nage2(40) && ObjCatch(pdat->eid, GOBJMSG_NAGE)) {
				return TRUE;
			}
			return FALSE;
		}

		// 空中ダッシュ
		if (wid == ACTID_DASHFJ || wid == ACTID_DASHBJ)
			return TRUE;
	}

	return CCharacterBase::CmdCom_OnKucyu(wid);
}

/*--------------------------------------------------------------------------------------
	GOBJMSG_ACTION メッセージ 処理
	新しい行動を追加した場合は、ここに処理を追加します。
	行動の前後に共通処理を入れたい場合は PreAction(),PostAction() をオーバーライドしてください
----------------------------------------------------------------------------------------*/
DWORD CCharacterEL::Action()
{
	// COM空中ダッシュ
	/*if (IsLocalCom() && pdat->aid & ACTID_KUCYU && !(pdat->aid & ACTID_ATTACK) && pdat->counter % 10 == 0)
	{
		GOBJECT* pedat = GetInfo(pdat->eid);

		if (pedat && abs(pdat->x - pedat->x) >= 300.0 && rand() % 6 == 0)
			SetComAct(ACTID_DASHFJ, 1);
	}*/

	switch(GetGObject()->aid){
	case ACTID_EARROW1:		act_earrow1();	break;
	case ACTID_EARROW2:		act_earrow2();	break;
	case ACTID_EARROW3:		act_earrow3();	break;
	case ACTID_EARROW1J:	act_earrow1j();	break;
	case ACTID_EARROW2J:	act_earrow2j();	break;
	case ACTID_EARROW3J:	act_earrow3j();	break;
	case ACTID_DASHF:		act_dashf();	break;
	case ACTID_DASHB:		act_dashb();	break;
	case ACTID_DASHFJ:		act_dashfj();	break;
	case ACTID_DASHBJ:		act_dashbj();	break;
	case ACTID_SWORD1R:		act_sword1r();	break;
	case ACTID_SWORD2R:		act_sword2r();	break;
	case ACTID_SWORD3R:		act_sword3r();	break;
	case ACTID_SWORD1A:		act_sword1a();	ChainCombo(CHAIN_SWORD1);break;
	case ACTID_SWORD2A:		act_sword2a();	ChainCombo(CHAIN_SWORD2);break;
	case ACTID_SWORD3A:		act_sword3a();	ChainCombo(CHAIN_SWORD3);break;
	case ACTID_SWORD1D:		act_sword1d();	break;
	case ACTID_SWORD2D:		act_sword2d();	break;
	case ACTID_SWORD3D:		act_sword3d();	break;
	case ACTID_NAGE1:		act_nage1();	break;
	case ACTID_NAGE2:		act_nage2();	break;
	case ACTID_NAGEEND:		act_nageend();	break;
	case ACTID_JUDGEMENT:	act_judgement();break;
	case ACTID_ATT_SCS1:	act_att_scs1();	break;
	case ACTID_ATT_SCS2:	act_att_scs2();	break;
	default://それ以外はキャラクター基本クラスに任せる
		return CCharacterBase::Action();
	}

	return(TRUE);
}

void CCharacterEL::PreAction()
{
	if(m_1st_update)
	{
		if(IsLocalCom() && GetTeamNum2(pdat->tid)==1 && GetComLevel()>6)
		{
			m_event_sw = TRUE; // 倒すとポイント限界突破選択可能
			m_opt_regene = TRUE; // HP自動回復
			int enemy_num = GetTeamNum2( EnemyTID() );

			m_regene_rate = 5;
			isAutoGuard = TRUE;

			float atkrate = 1.0f;

			switch(GetTaisenKeisiki())
			{
			case TAISENKEISIKI_GOCYAMAZE:
				m_power_rate *= 1.0f+((float)enemy_num*0.25f);
				atkrate = 1.0f+((float)enemy_num*0.25f);
				pdat->hpmax	= (DWORD)(pdat->hpmax*atkrate);
				pdat->hp	= (DWORD)(pdat->hp*atkrate);
				if(enemy_num>1)isSuperArmer = TRUE;
				m_regene_rate = 1;
				break;
			case TAISENKEISIKI_KOUTAI	:
				atkrate = 1.0f+((float)enemy_num*0.12f) ;
				pdat->hpmax	= (DWORD)(pdat->hpmax*atkrate);
				pdat->hp	= (DWORD)(pdat->hp*atkrate);
				break;
			case TAISENKEISIKI_JYUNBAN	:
				atkrate = 1.0f+((float)enemy_num*0.12f) ;
				pdat->hpmax	= (DWORD)(pdat->hpmax*atkrate);
				pdat->hp	= (DWORD)(pdat->hp*atkrate);
				m_regene_rate = 8;
				break;
			}
			for(int i=0;i<NUM_ATTACKINFO;i++){
				aif[i].damage = (DWORD)(aif[i].damage * atkrate);
			}
		}
		m_1st_update = FALSE;
	}

	//HPの回復
	if(m_opt_regene && pdat->hp>5)
	{
		if(
				!(pdat->aid & ACTID_ATTACK) && 
				!(pdat->aid & ACTID_SYSTEM) && 
				!(pdat->aid & ACTID_KUCYU)  && 
				(pdat->aid != ACTID_TOJYO) && 
				(pdat->aid != ACTID_SYORI) 
		)
		{
			if((pdat->counter%m_regene_rate)==0)pdat->hp += 1;
			if(pdat->hp > (int)pdat->hpmax)pdat->hp = pdat->hpmax;
		}
		else
		{
			if((pdat->counter%(m_regene_rate*3))==0)pdat->hp += 1;
			if(pdat->hp > (int)pdat->hpmax)pdat->hp = pdat->hpmax;
		}
	}
	if(pdat->hp<=0 && m_event_sw)
	{
		Message2System( 'aki','LCut');
		m_event_sw = FALSE;
	}

	//ゲージの増加
	if(
		!(pdat->aid & ACTID_ATTACK)	&& //攻撃中でない
		(pdat->aid != ACTID_DASHFJ)	&& //空中ダッシュ中でない
		(pdat->aid != ACTID_DASHBJ)	&&
		(pdat->hp >0 )					//死んでない
		)
	{
		AddPowerGauge( m_power_rate );
	}

	CCharacterBase::PreAction();
}

/*--------------------------------------------------------------------------------------
	攻撃力情報初期化
----------------------------------------------------------------------------------------*/
void CCharacterEL::InitAttackInfo()
{
	ZeroMemory(aif,sizeof(aif));

	//設定例

	int i;

	//・通常攻撃（立）

	i=ATTACKINFO_STAND_A;//立ち弱攻撃
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3;//ガード情報
	aif[i].id		=ATTACK_WEAK;//ゲージ増加時に使用するID
	aif[i].damage	=60;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_STAND_B;//立ち中攻撃
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2;//ガード情報
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=80;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_STAND_C;//立ち強攻撃
	aif[i].hit		=HITINFO_FUTTOBI2 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 | HITINFO_EFCTSINDO;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=100;//ダメージ
	aif[i].kezuri	=0;//削り

	//・通常攻撃(屈)

	i=ATTACKINFO_CROUCH_A;//しゃがみ弱攻撃
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT1 | GUARDINFO_SIV1  | GUARDINFO_XSTAND ;//ガード情報
	aif[i].id		=ATTACK_WEAK;//ゲージ増加時に使用するID
	aif[i].damage	=50;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_CROUCH_B;//しゃがみ中攻撃
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT2 | GUARDINFO_SIV2;//ガード情報
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=70;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_CROUCH_C;//しゃがみ強攻撃
	aif[i].hit		=HITINFO_FUTTOBI2 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 | HITINFO_EFCTSINDO;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=90;//ダメージ
	aif[i].kezuri	=0;//削り

	//・通常攻撃（飛）

	i=ATTACKINFO_JAMP_A;//ジャンプ弱攻撃
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 ;//ガード情報
	aif[i].id		=ATTACK_WEAK;//ゲージ増加時に使用するID
	aif[i].damage	=60;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_JAMP_B;//ジャンプ中攻撃
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3  | GUARDINFO_XCROUCH;//ガード情報
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=80;//ダメージ
	aif[i].kezuri	=0;//削り

	i=ATTACKINFO_JAMP_C;//ジャンプ強攻撃
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=120;//ダメージ
	aif[i].kezuri	=0;//削り

	//・テトラグラマトン（エナジーアロー）

	i=ATTACKINFO_EARROW;
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK3 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3  | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_MIDDLE;//ゲージ増加時に使用するID
	aif[i].damage	=50;//ダメージ
	aif[i].kezuri	=3;//削り

	//・ソード・オブ・ジャスティス
	i=ATTACKINFO_SWORD1;
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 | HITINFO_EFCTSINDO;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
	aif[i].damage	=110;//ダメージ
	aif[i].kezuri	=10;//削り
	
	i=ATTACKINFO_SWORD2;
	aif[i].hit		=HITINFO_FUTTOBI2 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 | HITINFO_EFCTSINDO;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XSTAND | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
	aif[i].damage	=110;//ダメージ
	aif[i].kezuri	=10;//削り

	i=ATTACKINFO_SWORD3;
	aif[i].hit		=HITINFO_TATAKI1A | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 | HITINFO_EFCTSINDO;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
	aif[i].damage	=110;//ダメージ
	aif[i].kezuri	=10;//削り

	//・投げ
	i=ATTACKINFO_NAGE;
	aif[i].hit		=HITINFO_REACT3 | HITINFO_SIV3 | HITINFO_MARK4 | HITINFO_SNDHIT3 ;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_SIV3 | GUARDINFO_XCROUCH;//ガード情報
	aif[i].id		=ATTACK_STRONG;//ゲージ増加時に使用するID
	aif[i].damage	=120;//ダメージ
	aif[i].kezuri	=0;//削り

	//・ジャッジメント
	i=ATTACKINFO_JUDGE;
	aif[i].hit		=HITINFO_FUTTOBI2 | HITINFO_MARK4 | HITINFO_SNDHIT3 | HITINFO_EFCTSINDO | HITINFO_EFCTBURN;//ヒット情報
	aif[i].guard	=GUARDINFO_REACT3 | GUARDINFO_XJAMP;//ガード情報
	aif[i].id		=ATTACK_HISSATU;//ゲージ増加時に使用するID
	aif[i].damage	=250;//ダメージ
	aif[i].kezuri	=22;//削り

	//攻撃力+
	if(m_opt_attackPlus){
		for(i=0;i<NUM_ATTACKINFO;i++){
			aif[i].damage = (DWORD)(aif[i].damage * 1.3);
		}
	}
}

/*--------------------------------------------------------------------------------------
	攻撃開始処理（通常、書き換える必要はありません）
----------------------------------------------------------------------------------------*/
void CCharacterEL::StartAttack(DWORD atk_info_id)
{
	GetGObject()->kougeki = TRUE;
	GetGObject()->atk = &aif[atk_info_id];
}

/*--------------------------------------------------------------------------------------
	COM行動用 技情報 初期化
----------------------------------------------------------------------------------------*/
void CCharacterEL::InitWazInfo()//コンピュータ用技情報の設定
{
	CCharacterBase::InitWazInfo();

	
	waz.dashf = ACTID_DASHF;
	waz.dashb = ACTID_DASHB;

	//短リーチ攻撃
	waz.att_short[0] = ACTID_ATT_SA;
	waz.att_short[1] = ACTID_ATT_SB;
	waz.att_short[2] = ACTID_SWORD2A;
	waz.att_short[3] = ACTID_SWORD3A;
	waz.att_short[4] = ACTID_ATT_CA;
	waz.att_short[5] = ACTID_ATT_SC;

	//中間リーチ攻撃
	waz.att_middle[0] = ACTID_ATT_SC;
	waz.att_middle[1] = ACTID_EARROW1;
	waz.att_middle[2] = ACTID_SWORD1R;
	waz.att_middle[3] = ACTID_SWORD3R;
	waz.att_middle[4] = ACTID_SWORD2A;
	waz.att_middle[5] = ACTID_SWORD3A;

	//長リーチ攻撃
//	waz.att_long[0] = ACTID_ATT_SB;
	waz.att_long[0] = ACTID_EARROW1;
	waz.att_long[1] = ACTID_SWORD1A;
	waz.att_long[2] = ACTID_SWORD3A;

	//空中攻撃
	waz.att_jamp[0] = ACTID_ATT_JA;
	waz.att_jamp[1] = ACTID_ATT_JB;
	waz.att_jamp[2] = ACTID_ATT_JC;
	waz.att_jamp[3] = ACTID_EARROW1J;
	waz.att_jamp[4] = ACTID_JUDGEMENT;
	waz.att_jamp[5] = ACTID_NAGE2;

	//飛道具
	waz.att_bullet[0] = ACTID_EARROW1;

	//対空
	waz.att_tai[0] = ACTID_ATT_SC;
	waz.att_tai[1] = ACTID_ATT_CC;
	waz.att_tai[2] = ACTID_SWORD2A;
	waz.att_tai[3] = ACTID_SWORD3A;

	//投げの方にも
	waz.nageid[0] = ACTID_NAGE1;
}



/*--------------------------------------------------------------------------------------
	GOBJMSG_TOUCHB メッセージ 処理
	ゲージ増加処理
----------------------------------------------------------------------------------------*/
DWORD CCharacterEL::TouchB(ATTACKINFO *info,BOOL hit)
{
	if(IsLocalCom())
	{
		switch(pdat->aid)
		{
		case ACTID_ATT_SA:
			{
				if(ComLevelCk(1)){
					SetComAct(ACTID_ATT_SB,3);
				}

				if(ComLevelCk(1)){
					SetComAct(ACTID_ATT_SC,5);
				}

				if(ComLevelCk(1)){
					SetComAct(ACTID_ATT_CC,3);
				}

				if(ComLevelCk(1)){
					SetComAct(ACTID_SWORD1R,5);
				}
			}
			break;
		case ACTID_ATT_SB:
			{
				if(ComLevelCk(1)){
					SetComAct(ACTID_ATT_SC,5);
				}

				if(ComLevelCk(1)){
					SetComAct(ACTID_ATT_CC,3);
				}

				if(ComLevelCk(1)){
					SetComAct(ACTID_SWORD1R,5);
				}
			}
			break;
		case ACTID_ATT_SC:
			{
				if(ComLevelCk(1))
				{
					GOBJECT* pedat = GetInfo(pdat->eid);

					if ( pedat->y >= -50 ){
						SetComAct(ACTID_ATT_CC,1);
					}
					else{
						SetComAct(ACTID_SWORD3R,m_event_sw ? 10 : 5);
					}
				}
			}
			break;
		case ACTID_ATT_CA:
			{
				if(ComLevelCk(1)){
					SetComAct(ACTID_ATT_CB,3);
				}

				if(ComLevelCk(1)){
					SetComAct(ACTID_ATT_SC,5);
				}

				if(ComLevelCk(1)){
					SetComAct(ACTID_ATT_CC,3);
				}

				if(ComLevelCk(1)){
					SetComAct(ACTID_SWORD1R,5);
				}
			}
			break;
		case ACTID_ATT_CB:
			{
				if(ComLevelCk(1)){
					SetComAct(ACTID_ATT_SC,5);
				}

				if(ComLevelCk(1)){
					SetComAct(ACTID_ATT_CC,3);
				}

				if(ComLevelCk(1)){
					SetComAct(ACTID_SWORD1R,5);
				}
			}
			break;
		case ACTID_ATT_CC:
			{
				if(ComLevelCk(1)){
					SetComAct(ACTID_SWORD3R,m_event_sw ? 20 : 10);
				}
			}
			break;
		case ACTID_ATT_JA:
		case ACTID_ATT_JB:
		case ACTID_ATT_JC:
			{
				if(ComLevelCk(3) && pdat->gauge >= 1.0)
				{
					GOBJECT* pedat = GetInfo(pdat->eid);

					if ( !(pedat->aid & ACTID_KUCYU) )
					{
						if (pdat->aid & ACTID_KUCYU) SetComAct(ACTID_JUDGEMENT,1);
					}
					else{
						SetComAct(ACTID_EARROW1J,5);
					}
				}
				else if(ComLevelCk(1)){
					SetComAct(ACTID_EARROW1J,5);
				}
			}
			break;

		case ACTID_SWORD1A:
			{
				if(ComLevelCk(1)){
					int maai = GetMaai_H(pdat->id, pdat->eid);

					if (maai < 50)
						SetComAct(ACTID_SWORD2A,10);
					else
						SetComAct(ACTID_SWORD3A,10);
				}
			}
			break;
		case ACTID_SWORD2A:
			{
				if(ComLevelCk(1)){
					SetComAct(ACTID_SWORD3A,10);
				}
			}
			break;
		}
	}

	return CCharacterBase::TouchB(info, hit);
}

/*--------------------------------------------------------------------------------------
	COMレベル判定
----------------------------------------------------------------------------------------*/
BOOL CCharacterEL::ComLevelCk(UINT level)
{
	int crnt_level = GetComLevel();
	int v = rand()%100;
	int r;
	const int rat_base = 80;

	if((level==0) || crnt_level==level)
	{
		r = rat_base;
	}
	else if(crnt_level<(int)level)
	{
		r = rat_base*crnt_level/level;
	}
	else
	{
		r = rat_base + (100-rat_base)*(crnt_level-level)/7;
	}

	return (r>=v) ? TRUE : FALSE;
}

DWORD CCharacterEL::TouchA(ATTACKINFO *info,DWORD ta_eid)
{
	if (m_event_sw)
	{
		// 強制ガードしたり、しなかったり。
		if (rand() % 3 == 0)
			isAutoGuard = TRUE;
		else
			isAutoGuard = FALSE;
	}

	return CCharacterBase::TouchA(info, ta_eid);
}

/*--------------------------------------------------------------------------------------
	COM行動 思考ルーチン定義
----------------------------------------------------------------------------------------*/
#define MAAI_SHORT 130
#define MAAI_MIDDLE 260
#define MAAI_LONG 390

DWORD CCharacterEL::ComThink()
{
	/*GOBJECT *pedat= GetInfo(GetGObject()->eid);
	if(pedat==NULL)return 0;

	// 行動指数
	int val_walkf = 0;
	int val_walkb = 0;
	int val_jumpf = 0;
	int val_jumpb = 0;
	int val_dashf = 0;
	int val_dashb = 0;
	int val_dashfj = 0;
	int val_dashbj = 0;
	int val_att_sa = 0;
	int val_att_sb = 0;
	int val_att_sc = 0;
	int val_att_ca = 0;
	int val_att_cb = 0;
	int val_att_cc = 0;
	int val_att_ja = 0;
	int val_att_jb = 0;
	int val_att_jc = 0;
	int val_nage = 0;
	int val_tetra = 0;
	int val_sword1 = 0;
	int val_sword2 = 0;
	int val_sword3 = 0;
	int val_sword1a = 0;
	int val_sword2a = 0;
	int val_sword3a = 0;
	int val_judgement = 0;

	// 位置関係
	int maai = (int)abs(pedat->x - pdat->x);

	if (pdat->aid & ACTID_KUCYU)
	{
		// 空中
		if (pedat->aid & ACTID_KUCYU)
		{
			// 空中戦
			if (maai < MAAI_SHORT)
			{
				val_dashbj += 4;
				val_att_ja += 5;
				val_att_jb += 6;
				val_att_jc += 7;
			}

			val_tetra += 6;
			val_dashfj += 2;
			if (pedat->y > pdat->y && !pedat->muteki)
				val_judgement += 4;
		}
		else
		{
			// 対地
			if (maai < MAAI_SHORT)
			{
				val_att_ja += 3;
				val_att_jb += 4;
				val_att_jc += 5;
			}
			val_dashbj += 1;
			val_dashfj += 3;
			val_tetra += 7;
			if (!pedat->muteki)
				val_judgement += 7;
		}
	}
	else
	{
		// 地上
		if (pedat->aid & ACTID_KUCYU)
		{
			// 対空
			if (maai < MAAI_SHORT)
			{
				val_walkb += 1;
				val_walkf += 3;
				val_dashb += 3;
				val_sword2a += 6;
				val_sword3a += 6;
				val_sword3 += 6;
				val_att_sc += 3;
				val_att_cc += 5;
			}
			else
			{
				// 特に意味が感じられない
				val_walkf += 2;
				val_walkb += 4;
				val_tetra += 3;
			}
		}
		else
		{
			// 地上戦
			if (maai < MAAI_SHORT)
			{
				// 近距離
				val_walkb += 4;
				val_dashb += 4;
				val_jumpb += 3;
				val_att_sa += 5;
				val_att_sb += 4;
				val_att_ca += 5;
				val_att_sc += 4;
				val_nage += 5;
				val_sword1a += 6;
				val_sword2a += 6;
				val_sword3a += 5;
				val_tetra += 2;
			}
			else if (maai < MAAI_MIDDLE)
			{
				// 中距離
				val_walkf += 3;
				val_dashf += 3;
				val_walkb += 3;
				val_dashb += 3;
				val_jumpf += 2;
				val_jumpb += 3;
				val_att_sc += 7;
				val_sword1a += 6;
				val_sword2a += 2;
				val_sword3a += 5;
				val_sword1 += 7;
				val_sword2 += 3;
				val_sword3 += 6;
				val_tetra += 3;
			}
			else if (maai < MAAI_LONG)
			{
				// 長距離
				val_walkf += 5;
				val_dashf += 5;
				val_dashb += 1;
				val_jumpf += 5;
				val_jumpb += 3;
				val_att_sc += 5;
				val_tetra += 6;
				val_sword1a += 2;
				val_sword3a += 2;
			}
			else
			{
				// 超長距離
				val_walkf += 4;
				val_dashf += 6;
				val_jumpf += 5;
				val_tetra += 6;
			}
		}
	}

	// ランダム要素
	val_walkf += rand() % 5;
	val_walkb += rand() % 5;
	val_jumpf += rand() % 5;
	val_jumpb += rand() % 5;
	val_dashf += rand() % 5;
	val_dashb += rand() % 5;
	val_dashfj += rand() % 5;
	val_dashbj += rand() % 5;
	val_att_sa += rand() % 5;
	val_att_sb += rand() % 5;
	val_att_sc += rand() % 5;
	val_att_ca += rand() % 5;
	val_att_cb += rand() % 5;
	val_att_cc += rand() % 5;
	val_att_ja += rand() % 5;
	val_att_jb += rand() % 5;
	val_att_jc += rand() % 5;
	val_nage += rand() % 5;
	val_tetra += rand() % 5;
	val_sword1 += rand() % 5;
	val_sword2 += rand() % 5;
	val_sword3 += rand() % 5;
	val_sword1a += rand() % 5;
	val_sword2a += rand() % 5;
	val_sword3a += rand() % 5;
	val_judgement += rand() % 5;

	// 無理矢理行動決定
	int highest = 0;
	DWORD now_aid = 0;

	if (pdat->tid == TEAM_PLAYER1)
	{
	if (val_walkf > highest)
	{
		highest = val_walkf;
		now_aid = ACTID_WALKF;
	}
	if (val_walkb > highest)
	{
		highest = val_walkb;
		now_aid = ACTID_WALKB;
	}
	if (val_jumpf > highest)
	{
		highest = val_jumpf;
		now_aid = ACTID_JAMPF;
	}
	if (val_jumpb > highest)
	{
		highest = val_jumpb;
		now_aid = ACTID_JAMPB;
	}
	if (val_dashf > highest)
	{
		highest = val_dashf;
		now_aid = ACTID_DASHF;
	}
	if (val_dashb > highest)
	{
		highest = val_dashb;
		now_aid = ACTID_DASHB;
	}
	if (val_dashfj > highest)
	{
		highest = val_dashfj;
		now_aid = ACTID_DASHFJ;
	}
	if (val_dashbj > highest)
	{
		highest = val_dashbj;
		now_aid = ACTID_DASHBJ;
	}
	if (val_att_sa > highest)
	{
		highest = val_att_sa;
		now_aid = ACTID_ATT_SA;
	}
	if (val_att_sb > highest)
	{
		highest = val_att_sb;
		now_aid = ACTID_ATT_SB;
	}
	if (val_att_sc > highest)
	{
		highest = val_att_sc;
		now_aid = ACTID_ATT_SC;
	}
	if (val_att_ca > highest)
	{
		highest = val_att_ca;
		now_aid = ACTID_ATT_CA;
	}
	if (val_att_cb > highest)
	{
		highest = val_att_cb;
		now_aid = ACTID_ATT_CB;
	}
	if (val_att_cc > highest)
	{
		highest = val_att_cc;
		now_aid = ACTID_ATT_CC;
	}
	if (val_att_ja > highest)
	{
		highest = val_att_ja;
		now_aid = ACTID_ATT_JA;
	}
	if (val_att_jb > highest)
	{
		highest = val_att_jb;
		now_aid = ACTID_ATT_JB;
	}
	if (val_att_jc > highest)
	{
		highest = val_att_jc;
		now_aid = ACTID_ATT_JC;
	}
	if (val_nage > highest)
	{
		highest = val_nage;
		now_aid = ACTID_NAGE1;
	}
	if (val_tetra > highest)
	{
		highest = val_tetra;
		now_aid = ACTID_EARROW1;
	}
	if (val_sword1 > highest)
	{
		highest = val_sword1;
		now_aid = ACTID_SWORD1R;
	}
	if (val_sword2 > highest)
	{
		highest = val_sword2;
		now_aid = ACTID_SWORD2R;
	}
	if (val_sword3 > highest)
	{
		highest = val_sword3;
		now_aid = ACTID_SWORD3R;
	}
	if (val_sword1a > highest)
	{
		highest = val_sword1a;
		now_aid = ACTID_SWORD1A;
	}
	if (val_sword2a > highest)
	{
		highest = val_sword2a;
		now_aid = ACTID_SWORD2A;
	}
	if (val_sword3a > highest)
	{
		highest = val_sword3a;
		now_aid = ACTID_SWORD3A;
	}
	if (val_judgement > highest)
	{
		highest = val_judgement;
		now_aid = ACTID_JUDGEMENT;
	}
	}*/

	return 0;
}
