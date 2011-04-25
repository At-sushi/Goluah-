
/*============================================================================

	戦闘タスククラス

	（ネットワーク対応版）

==============================================================================*/

#include "StdAfx.h"

#include "define_macro.h"
#include "global.h"
#include ".\task_battle_net.h"
#include "task_loading.h"
#include "gcdhandler.h"


/*----------------------------------------------------------------------------
	構築
------------------------------------------------------------------------------*/
CBattleTaskNet::CBattleTaskNet(void)
{

	cp_gauge = NULL;
	cp_efctlist = NULL;
}

/*----------------------------------------------------------------------------
	破棄
------------------------------------------------------------------------------*/
CBattleTaskNet::~CBattleTaskNet(void)
{
	Terminate();
	g_play.SetCallBack(NULL);
}

/*===========================================================================

	タスク　初期化

=============================================================================*/
void CBattleTaskNet::Initialize()
{
	int i,j;

	// 動的配列初期化
	p_objects.resize(OBJECTS_MEMINCRATE);
	object_regno.resize(OBJECTS_MEMINCRATE);
	suicide_list.resize(0);

	m_crnt_dllid = 0;

	for(i=0;i<(int)p_objects.size();i++){
		p_objects[i]=NULL;
	}

	g_battleinfo.Initialize();
	g_battleinfo.SetBattleType( TAISENKEISIKI_GOCYAMAZE );
	g_battleinfo.SetLimitTime(-1);
	g_battleinfo.AddCharacter(TEAM_PLAYER1,
			g_charlist.FindCharacter("おにぎり"),
			1,
			g_play.IsHost() ? 0 : CASSIGN_NONE,
			/*g_charlist.GetRandomOption( g_charlist.FindCharacter("おにぎり") )*/0
			);
	g_battleinfo.AddCharacter(TEAM_PLAYER2,
			g_charlist.FindCharacter("おにぎり"),
			2,
			g_play.IsHost() ? 1 : 0,
			/*g_charlist.GetRandomOption( g_charlist.FindCharacter("おにぎり") )*/0
			);
	g_battleinfo.SetStage(0);

	CBattleTaskBase::Initialize();

	StartRound();

	// 受信時のコールバック関数を設定
	g_play.SetCallBack(DPlayCallBack);

	// フレームレート固定
	g_system.SetMaxFPS(50);
}


/*----------------------------------------------------------------
	オブジェクトリスト初期化
------------------------------------------------------------------*/
void CBattleTaskNet::InitializeObjectList()
{
	g_system.PushSysTag(__FUNCTION__);

	for(int i=0;i<(int)object_regno.size();i++){
		object_regno[i]=0;
	}
	for(int i=0;i<MAXNUM_TEAM*2+2;i++){
		object_regindex[i]=0;
	}

	//オブジェクト0作成
	DWORD obj0=
		CreateGObject();
	GetGObjectInfo(0)->tid = TEAM_SYSTEM;
	GetGObjectInfo(0)->y = -9999;

	g_system.PopSysTag();
}



/*----------------------------------------------------------------
	パラメータリセット
------------------------------------------------------------------*/
void CBattleTaskNet::InitializeParameters()
{
	round=0;//なぜならStartRoundで++されるから。
	wincount[0] = wincount[1]=0;
	strikercount[0] = strikercount[1] = g_config.GetStrikerCount();
	active_character[0] = active_character[1] =0;
	bf_state = BFSTATE_WAITFORENDPOSE;
	disp_center_x=0;
	disp_center_y=-(double)tan(D3DXToRadian(40)) * 240.0;
	battle_end = FALSE;

	int i,j;
	for(i=0;i<MAXNUM_TEAM;i++){
		for(j=0;j<2;j++){
			charobjid[i][j]=0;
			hprecratio[j][i]=5;
		}
	}
}


/*----------------------------------------------------------------
	その他クラスを初期化
	ゲージ・エフェクト
------------------------------------------------------------------*/
void CBattleTaskNet::InitializeSubTasks()
{
	g_system.PushSysTag(__FUNCTION__);

	cp_gauge = new CGauge;
	cp_efctlist = new CEffectList;

	cp_gauge->Initialize();
	cp_efctlist->Initialize();

	g_system.PopSysTag();
}


/*----------------------------------------------------------------
	ラウンド数を進める
	パラメータ等をリセットする
------------------------------------------------------------------*/
void CBattleTaskNet::StartRound()
{
	int i,j;
	round++;
	char filename[256];
	BOOL call_round=TRUE;

	//キャラクターの状態を設定する
	DWORD k;
	GOBJECT	  *pdat;
	for(j=0;j<2;j++){
		for(i=0;i<(int)g_battleinfo.GetNumTeam(j);i++){	
			k=charobjid[j][i];
			if(k!=0){
				pdat = &(GetGObject(k)->data);
				//「敵キャラクタ」のidを設定（適当）
				if(pdat->tid==TEAM_PLAYER1)
					pdat->eid = charobjid[TEAM_PLAYER2][0];
				else
					pdat->eid = charobjid[TEAM_PLAYER1][0];
				//位置設定
				pdat->y = 0;
				pdat->x = (150 + 50*i) *(j==0 ? -1 : 1);
				pdat->muki = (j==0 ? FALSE : TRUE);
	
				if(pdat != NULL){
					pdat->hp = pdat->hpmax;		//HP回復
					pdat->aid = ACTID_NEUTRAL;	//行動ID
					GetGObject(pdat->id)->ActionIDChanged(TRUE,TRUE);
				}
				if(g_battleinfo.GetCharacterIsCOM(j,i)){//コンピュータ操作フラグ付加
						GetGObject(k)->data.objtype |= GOBJFLG_COMPUTER;
				}
				GetGObject(k)->Message(GOBJMSG_CNGROUND);//ラウンド開始のメッセージ送信
			}
		}
	}
	
//	bf_state = BFSTATE_WAITFORENDPOSE;
	bf_state = BFSTATE_FIGHTING;
	bf_counter=0;
	
	//エフェクトすべて無効
	efct_slowdown=0;
	efct_stop=0;
	efct_darkbg=0;
	efct_nobg=0;
	efct_sindo=0;
	efct_sindom=0;
	efct_hitstop=0;
	efct_flash=0;
}



/*===========================================================================

	タスク　破棄

=============================================================================*/
void CBattleTaskNet::TerminateObjectList()
{
	g_system.PushSysTag(__FUNCTION__);

	for(UINT i=0;i<(int)p_objects.size();i++){
		DELETECLASS(p_objects[i]);
	}

	g_system.PopSysTag();
}

void CBattleTaskNet::TerminateDestroySubTasks()
{
	g_system.PushSysTag(__FUNCTION__);

	if(cp_gauge)cp_gauge->CleanUp();
	if(cp_efctlist)cp_efctlist->Destroy();

	DELETECLASS( cp_gauge );
	DELETECLASS( cp_efctlist );

	// フレームレートを戻す
	g_system.UpdateFrameRate();

	g_system.PopSysTag();
}


/*===========================================================================

	タスク　実行

=============================================================================*/
BOOL CBattleTaskNet::Execute(DWORD time)
{
	static char execute_tag[256];
	sprintf(execute_tag,"%s (%s)",__FUNCTION__,GetBattleStateString());
	g_system.PushSysTag(execute_tag);

/*	if(m_pause_task){
		g_system.PopSysTag();
		return TRUE;
	}
*/
	// 接続が切れたら中断
	if(!g_play.IsOnline()){
		g_system.PopSysTag();
		return FALSE;
	}

	//local vals
	int i;
	
	act_stop=FALSE;
	g_input.KeyLock( bf_state==BFSTATE_FIGHTING ? FALSE : TRUE );

	if(TRUE){

		//エフェクト・スロー
		if(efct_slowdown>0){
			efct_slowdown--;
			if(efct_slowdown%2==0){
				g_system.PopSysTag();
				return battle_end ? FALSE : TRUE;
			}
		}
		efct_sindo--;

		//ストップ処理
		if(efct_stop>0){
			efct_stop--;
			act_stop=TRUE;
		}
		if(efct_hitstop){
			efct_hitstop--;
			act_stop=TRUE;
		}

		T_Command();//command
		T_Action(act_stop);//action
		T_KasanariHantei();//重なり判定
		T_Sousai();//飛び道具の相殺
		T_AtariHantei();//当たり判定
		T_ChangeTarget();//ターゲット変更処理
		T_UpdateStatus();//試合状態更新

		if(battle_end){
			g_system.PopSysTag();
			return FALSE;
		}
	}

	g_input.KeyLock(FALSE);

	// 2秒おきにHP情報を送信
	if (GetGObjectInfo(0)->counter % 100 == 0 &&
		g_play.IsHost()){
			for (int i = 0; i <= 1; i++){
				struct Sync2Mes s2m;

				s2m.msgid = GNETMSG_SYNC2;
				s2m.pid = i*MAXNUM_TEAM;
				s2m.hp = GetCharacterInfo(i, 0)->hp;
				s2m.gauge = GetCharacterInfo(i, 0)->gauge;
				g_play.SendMsg(DPNID_ALL_PLAYERS_GROUP, s2m, sizeof(s2m), 200,
								DPNSEND_NOLOOPBACK | DPNSEND_NONSEQUENTIAL);
			}
	}

	//オブジェクトリスト処理。消滅したがっているオブジェクトを消滅させる
	for(i=0;i<(int)suicide_list.size();i++){
		if(suicide_list[i]!=0){
			DeleteGObject(suicide_list[i]);
		}
	}
	suicide_list.resize(0);

	//キー入力向き更新
	UpdateKeyInputDirections();

	//背景暗エフェクト
	efct_darkbg--;
	//背景なしエフェクト
	efct_nobg--;
	//フェードイン
	efct_fadein--;
	//フラッシュ
	efct_flash--;

	//隠れキャラ出現
	if(rand()%(50*60*3000) - bf_counter % 30==0)
		if(rand()%(50*60*3000)==0)
			AddEffect(EFCTID_HATTEN,640, 5);//Hatten

	g_system.PopSysTag();
	
	return battle_end ? FALSE : TRUE;
}



/*----------------------------------------------------------------
	COMMANDメッセージをオブジェクトに送信
------------------------------------------------------------------*/
void CBattleTaskNet::T_Command()
{
	g_system.PushSysTag(__FUNCTION__);
	
	// リモート側を処理しないようにする必要有り（もうした）
	int i;
	for(i=0;i<(int)p_objects.size();i++){
		if(p_objects[i]!=NULL && 
			(p_objects[i]->data.id & BATTLETASK_FXOBJFLAG || IsLocal(p_objects[i]->dll_id)))
			p_objects[i]->Message(GOBJMSG_COMMAND);
	}

	if (!g_play.IsHost())
	{
		// キー入力情報をホストに送信（クライアント）
		struct InputMes imes;

		imes.msgid = GNETMSG_INPUT;
		imes.key = g_input.GetKeyEx(0, 0);
		g_play.SendMsg(DPNID_ALL_PLAYERS_GROUP, imes, sizeof(imes), 20, DPNSEND_NOLOOPBACK);
	}
	else
	{
		// 遅延ハンディ付きで自分のを記録（ホスト）
		g_input.SetKeyNet(0, g_input.GetKey(0, 50 * 1/20*2));
	}

	g_system.PopSysTag();
}


/*----------------------------------------------------------------
	ACTIONメッセージをオブジェクトに送信
------------------------------------------------------------------*/
void CBattleTaskNet::T_Action(BOOL stop)
{
	g_system.PushSysTag(__FUNCTION__);

	int i;
	for(i=0;i<(int)p_objects.size();i++){
		if(p_objects[i]!=NULL && 
			(p_objects[i]->data.id & BATTLETASK_FXOBJFLAG || IsLocal(p_objects[i]->dll_id) ||
			p_objects[i]->dll_id == 0 || p_objects[i]->dll_id == 7)){
			if(!stop)
				p_objects[i]->Message(GOBJMSG_ACTION);
			else if(p_objects[i]->data.objtype & GOBJFLG_DONOTSTOP ||
					p_objects[i]->data.nonstop)
				p_objects[i]->Message(GOBJMSG_ACTION);

			if (BATTLETASK_ISNOTFXOBJ((&p_objects[i]->data)) &&
				p_objects[i]->dll_id != 0 && p_objects[i]->dll_id != 7){	// システム、背景ではない。
				if (p_objects[i]->data.counter % 15 == 1){
					struct TestSyncMes tsm;

					tsm.msgid = GNETMSG_TESTSYNC;
					tsm.id = p_objects[i]->data.id;
					tsm.x = p_objects[i]->data.x;
					tsm.y = p_objects[i]->data.y;
					tsm.aid = p_objects[i]->data.aid;
					// tsm.cnow = p_objects[i]->data.cnow;
					tsm.counter = p_objects[i]->data.counter;
					tsm.muki = p_objects[i]->data.muki;
					g_play.SendMsg(DPNID_ALL_PLAYERS_GROUP, tsm, sizeof(tsm), 300, DPNSEND_NOLOOPBACK);
				}
			}
		}
	}

	if (g_play.IsHost() && GetGObjectInfo(0)->counter % 15 != 1) {	// アクションだけ（将来的に１ＰＣの全キャラ分を一括する予定 もうした）
		struct ActionMes am;

		am.msgid = GNETMSG_ACTION;
		am.isStop = (act_stop == TRUE);
		// am.aid = p_objects[i]->data.aid;
		g_play.SendMsg(DPNID_ALL_PLAYERS_GROUP, am, sizeof(am), 20, DPNSEND_NOLOOPBACK | DPNSEND_NONSEQUENTIAL);
	}

	g_system.PopSysTag();
}


/*----------------------------------------------------------------
	重なり判定処理
------------------------------------------------------------------*/
void CBattleTaskNet::T_KasanariHantei()
{
	g_system.PushSysTag(__FUNCTION__);

	int i,j,k,l;

	GOBJECT *pdat1 = NULL,*pdat2 = NULL;
	BOOL kas_yes = FALSE;

	GCD_CELL2  c_a,c_k;
	GCD_HANTEI h_1,h_2;
	BOOL revx1,revx2;
	UINT magmode1,magmode2;

	//重なり判定
	for(i=0;i<(int)p_objects.size();i++){
		if(p_objects[i]!=NULL && IsLocal(p_objects[i]->dll_id)){//オブジェクトが存在し、且つリモートでない
			pdat1 = &(p_objects[i]->data);
			if(pdat1->objtype & GOBJFLG_KASANARI){//オブジェクトは重なり判定を行う
				if(pdat1->kasanari){//重なり判定ON
					if(pdat1->phdat!=NULL){
						if(pdat1->pcdat!=NULL){
							if( ((GCD_CELL2_070*)pdat1->pcdat)[0].cell[0].flag==700 ){
								magmode1 = 1;//重心中心
							}
							else{
								if(pdat1->pcdat[pdat1->cnow].flag & GCDCELL2_ROT_BASEPOINT){
									magmode1 = 0;//足元中心
								}
								else magmode1=1;
							}
							for(j=i+1/*0*/;j<(int)p_objects.size();j++){//** 他の全てのオブジェクトに対して **
								// ローカル同士は自分より後のみに対して判定
								if(i<j || !IsLocal(p_objects[j]->dll_id)){//自分以外に
									if(p_objects[j]!=NULL){
										pdat2 = &(p_objects[j]->data);
										if(pdat1->tid != pdat2->tid){
											if(pdat2->objtype & GOBJFLG_KASANARI){//オブジェクトは攻撃を喰らう
												if(pdat2->kasanari){//重なり判定ON
													if(pdat2->phdat!=NULL){
														if(pdat2->pcdat!=NULL)
														{
															if( ((GCD_CELL2_070*)pdat2->pcdat)[0].cell[0].flag==700 ){
																magmode2 = 1;//重心中心
															}
															else{
																if(pdat2->pcdat[pdat2->cnow].flag & GCDCELL2_ROT_BASEPOINT){
																	magmode2 = 0;//足元中心
																}
																else magmode2=1;
															}
															//当たり判定を行う
															kas_yes=FALSE;
															h_1 = (pdat1->phdat[ pdat1->cnow ]);
															h_2 = (pdat2->phdat[ pdat2->cnow ]);
															/*if ( ((GCD_CELL2_070*)pdat2->pcdat)[0].cell[0].flag==700 )
															{
																CGCDHandler::GCDConvCell_070_090( &( ((GCD_CELL2_070*)pdat1->pcdat)[ pdat1->cnow ]), &c_a);
																CGCDHandler::GCDConvCell_070_090( &( ((GCD_CELL2_070*)pdat2->pcdat)[ pdat2->cnow ]), &c_k);
															}
															else
															{*/
																c_a = (pdat1->pcdat[ pdat1->cnow ]);
																c_k = (pdat2->pcdat[ pdat2->cnow ]);
															/*}*/
															if(pdat1->muki && !pdat1->revx)revx1=TRUE;
															else if(!pdat1->muki && pdat1->revx)revx1=TRUE;
															else revx1=FALSE;
															if(pdat2->muki && !pdat2->revx)revx2=TRUE;
															else if(!pdat2->muki && pdat2->revx)revx2=TRUE;
															else revx2=FALSE;
															for(k=0;k<3;k++){
																for(l=0;l<3;l++){
																	if(gbl.Syototu2RECTs(h_1.kas[k],h_2.kas[l],NULL,
																		c_a.gcx,c_a.gcy,revx1,pdat1->revy,pdat1->rot,pdat1->x,pdat1->y,pdat1->magx,pdat1->magy,
																		c_k.gcx,c_k.gcy,revx2,pdat2->revy,pdat2->rot,pdat2->x,pdat2->y,pdat2->magx,pdat2->magy,magmode1,magmode2))
																	{
																		k=l=3;
																		kas_yes=TRUE;
																	}
																}
															}
															if(kas_yes){//一つ以上の矩形が衝突した
																if(pdat1->x > pdat2->x){
																	pdat1->x += 3;
																	pdat2->x -= 3;
																}
																else{
																	pdat1->x -= 3;
																	pdat2->x += 3;
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	//画面中心を出す
	j=0;
	int new_disp_center_x=0;
	for(i=0;i<(int)p_objects.size();i++){
		if(p_objects[i]!=NULL){
			if((p_objects[i]->data.objtype & GOBJFLG_DISPLAYME) )//&& !(p_objects[i]->data.objtype & GOBJFLG_COMPUTER))
			{
				new_disp_center_x += (int)p_objects[i]->data.x;
				j++;
			}
		}
	}
	if(j!=0)new_disp_center_x/=j;
	if(disp_center_x > new_disp_center_x){
		if(disp_center_x > new_disp_center_x+50)disp_center_x-=16;
		else if(disp_center_x > new_disp_center_x+20)disp_center_x-=8;
		else disp_center_x-=4;
		if(disp_center_x < new_disp_center_x)disp_center_x = new_disp_center_x;//いきすぎ
	}
	if(disp_center_x < new_disp_center_x){
		if(disp_center_x < new_disp_center_x-50)disp_center_x+=16;
		else if(disp_center_x < new_disp_center_x-20)disp_center_x+=8;
		else disp_center_x+=4;
		if(disp_center_x > new_disp_center_x)disp_center_x = new_disp_center_x;//いきすぎ
	}
	if(disp_center_x>320)disp_center_x=320;
	else if(disp_center_x<-320)disp_center_x=-320;

	//画面外に行っちゃってるおメッセージ
	int gamengai;
	for(i=0;i<(int)p_objects.size();i++){
		if(p_objects[i]!=NULL && IsLocal(p_objects[i]->dll_id)){
			if(p_objects[i]->data.objtype & GOBJFLG_CLIPX2){
				if(p_objects[i]->data.x > disp_center_x+305){
					gamengai = (int)(disp_center_x+305 - p_objects[i]->data.x);
					p_objects[i]->Message(GOBJMSG_CLIPX,(DWORD)(&gamengai));
				}
				if(p_objects[i]->data.x < disp_center_x-305){
					gamengai = (int)(disp_center_x-305 - p_objects[i]->data.x);
					p_objects[i]->Message(GOBJMSG_CLIPX,(DWORD)(&gamengai));
				}
			}
		}
	}

	//x方向ステージ端クリップ処理
	for(i=0;i<(int)p_objects.size();i++){
		if(p_objects[i]!=NULL){//オブジェクトが存在する
			pdat1 = &(p_objects[i]->data);
			if(pdat1->objtype & GOBJFLG_CLIPX){//X方向限定
				if(pdat1->x < -620){
					gamengai= (int)(-620 - pdat1->x);
					p_objects[i]->Message(GOBJMSG_CLIPX,(DWORD)(&gamengai));
				}
				if(pdat1->x >  620){
					gamengai = (int)(620 - pdat1->x);
					p_objects[i]->Message(GOBJMSG_CLIPX,(DWORD)(&gamengai));
				}
			}
		}
	}

	g_system.PopSysTag();
}


/*----------------------------------------------------------------
	攻撃判定処理
------------------------------------------------------------------*/
void CBattleTaskNet::T_AtariHantei()
{
	g_system.PushSysTag(__FUNCTION__);

	static BOOL hantaihantei=FALSE;
	hantaihantei = !hantaihantei;

	int num_kas;
	MY2DVECTOR kas_point[3*6],kas_point2;

	GOBJECT *pdat1,*pdat2;
	GCD_CELL2  c_a,c_k;
	GCD_HANTEI h_a,h_k;
	BOOL revx1,revx2;
	UINT magmode1,magmode2;

	int i,j,k,l;
	if(!hantaihantei){
		for(i=0;i<(int)p_objects.size();i++){
			if(p_objects[i]!=NULL){//オブジェクトが存在する
				pdat1 = &(p_objects[i]->data);
				if((pdat1->tid==TEAM_PLAYER1 || pdat1->tid==TEAM_PLAYER2) && BATTLETASK_ISNOTFXOBJ(pdat1)){
					if(pdat1->objtype & GOBJFLG_ATTACK){//オブジェクトは攻撃を行う
						if(pdat1->kougeki){//攻撃力ON
							if(pdat1->phdat!=NULL){
								if(pdat1->pcdat!=NULL)
								{
									if( ((GCD_CELL2_070*)pdat1->pcdat)[0].cell[0].flag==700 ){
										magmode1 = 1;//重心中心
									}
									else{
										if(pdat1->pcdat[pdat1->cnow].flag & GCDCELL2_ROT_BASEPOINT){
											magmode1 = 0;//足元中心
										}
										else magmode1=1;
									}
									for(j=0;j<(int)p_objects.size();j++){//** 他の全てのオブジェクトに対して **
										if(i!=j){//自分以外に
											if(p_objects[j]!=NULL && IsLocal(p_objects[j]->dll_id)){
												pdat2 = &(p_objects[j]->data);
												if((pdat2->tid==TEAM_PLAYER1 || pdat2->tid==TEAM_PLAYER2)  && BATTLETASK_ISNOTFXOBJ(pdat2)){
													if(pdat1->tid != pdat2->tid){
														if(TRUE/*pdat2->counter!=0*/){
															if(pdat2->objtype & GOBJFLG_KURAI){//オブジェクトは攻撃を喰らう
																if(!pdat2->muteki){//無敵状態OFF
																	if(pdat2->phdat!=NULL){
																		if(pdat2->pcdat!=NULL)
																		{
																			if( ((GCD_CELL2_070*)pdat2->pcdat)[0].cell[0].flag==700 ){
																				magmode2 = 1;//重心中心
																				}
																				else{
																					if(pdat2->pcdat[pdat2->cnow].flag & GCDCELL2_ROT_BASEPOINT){
																						magmode2 = 0;//足元中心
																					}
																					else magmode2=1;
																			}
																			//当たり判定を行う
																			num_kas=0;
																			h_a = (pdat1->phdat[ pdat1->cnow ]);
																			h_k = (pdat2->phdat[ pdat2->cnow ]);
																			/*if ( ((GCD_CELL2_070*)pdat2->pcdat)[0].cell[0].flag==700 )
																			{
																				CGCDHandler::GCDConvCell_070_090( &( ((GCD_CELL2_070*)pdat1->pcdat)[ pdat1->cnow ]), &c_a);
																				CGCDHandler::GCDConvCell_070_090( &( ((GCD_CELL2_070*)pdat2->pcdat)[ pdat2->cnow ]), &c_k);
																			}
																			else
																			{*/
																				c_a = (pdat1->pcdat[ pdat1->cnow ]);
																				c_k = (pdat2->pcdat[ pdat2->cnow ]);
																			/*}*/
																			if(pdat1->muki && !pdat1->revx)revx1=TRUE;
																			else if(!pdat1->muki && pdat1->revx)revx1=TRUE;
																			else revx1=FALSE;
																			if(pdat2->muki && !pdat2->revx)revx2=TRUE;
																			else if(!pdat2->muki && pdat2->revx)revx2=TRUE;
																			else revx2=FALSE;
																			for(k=0;k<3;k++){
																				for(l=0;l<3;l++){
																					if(gbl.Syototu2RECTs(h_a.attack[k],h_k.kas[l],&kas_point[num_kas],
																						c_a.gcx,c_a.gcy,revx1,pdat1->revy,pdat1->rot,pdat1->x,pdat1->y,pdat1->magx,pdat1->magy,
																						c_k.gcx,c_k.gcy,revx2,pdat2->revy,pdat2->rot,pdat2->x,pdat2->y,pdat2->magx,pdat2->magy,magmode1,magmode2))
																					{
																						num_kas++;
																					}
																					if(gbl.Syototu2RECTs(h_a.attack[k],h_k.kurai[l],&kas_point[num_kas],
																						c_a.gcx,c_a.gcy,revx1,pdat1->revy,pdat1->rot,pdat1->x,pdat1->y,pdat1->magx,pdat1->magy,
																						c_k.gcx,c_k.gcy,revx2,pdat2->revy,pdat2->rot,pdat2->x,pdat2->y,pdat2->magx,pdat2->magy,magmode1,magmode2))
																					{
																						num_kas++;
																					}
																				}
																			}
																			if(num_kas>0){//一つ以上の矩形が衝突した
																				kas_point2.x=0;
																				kas_point2.y=0;
																				for(k=0;k<num_kas;k++){
																					kas_point2.x += kas_point[k].x;
																					kas_point2.y += kas_point[k].y;
																				}
																				kas_point2.x /= num_kas;
																				kas_point2.y /= num_kas;
																				Atari(p_objects[i]->data.id,p_objects[j]->data.id,kas_point2);
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else{//反対側から当たり判定をやる
		for(i=(int)p_objects.size()-1;i>=0;i--){
			if(p_objects[i]!=NULL){//オブジェクトが存在する
				pdat1 = &(p_objects[i]->data);
				if((pdat1->tid==TEAM_PLAYER1 || pdat1->tid==TEAM_PLAYER2) && BATTLETASK_ISNOTFXOBJ(pdat1)){
					if(pdat1->objtype & GOBJFLG_ATTACK){//オブジェクトは攻撃を行う
						if(pdat1->kougeki){//攻撃力ON
							if(pdat1->phdat!=NULL){
								if(pdat1->pcdat!=NULL)
								{
									if( ((GCD_CELL2_070*)pdat1->pcdat)[0].cell[0].flag==700 ){
										magmode1 = 1;//重心中心
									}
									else{
										if(pdat1->pcdat[pdat1->cnow].flag & GCDCELL2_ROT_BASEPOINT){
											magmode1 = 0;//足元中心
										}
										else magmode1=1;
									}
									for(j=0;j<(int)p_objects.size();j++){//** 他の全てのオブジェクトに対して **
										if(i!=j){//自分以外に
											if(p_objects[j]!=NULL && IsLocal(p_objects[j]->dll_id)){
												pdat2 = &(p_objects[j]->data);
												if((pdat2->tid==TEAM_PLAYER1 || pdat2->tid==TEAM_PLAYER2)  && BATTLETASK_ISNOTFXOBJ(pdat2) ){
													if(pdat1->tid != pdat2->tid){
														if(TRUE/*pdat2->counter!=0*/){
															if(pdat2->objtype & GOBJFLG_KURAI){//オブジェクトは攻撃を喰らう
																if(!pdat2->muteki){//無敵状態OFF
																	if(pdat2->phdat!=NULL){
																		if(pdat2->pcdat!=NULL)
																		{
																			if( ((GCD_CELL2_070*)pdat2->pcdat)[0].cell[0].flag==700 ){
																				magmode2 = 1;//重心中心
																				}
																				else{
																					if(pdat2->pcdat[pdat2->cnow].flag & GCDCELL2_ROT_BASEPOINT){
																						magmode2 = 0;//足元中心
																					}
																					else magmode2=1;
																			}
																			//当たり判定を行う
																			num_kas=0;
																			h_a = (pdat1->phdat[ pdat1->cnow ]);
																			h_k = (pdat2->phdat[ pdat2->cnow ]);
																			/*if ( ((GCD_CELL2_070*)pdat2->pcdat)[0].cell[0].flag==700 )
																			{
																				CGCDHandler::GCDConvCell_070_090( &( ((GCD_CELL2_070*)pdat1->pcdat)[ pdat1->cnow ]), &c_a);
																				CGCDHandler::GCDConvCell_070_090( &( ((GCD_CELL2_070*)pdat2->pcdat)[ pdat2->cnow ]), &c_k);
																			}
																			else
																			{*/
																				c_a = (pdat1->pcdat[ pdat1->cnow ]);
																				c_k = (pdat2->pcdat[ pdat2->cnow ]);
																			/*}*/
																			if(pdat1->muki && !pdat1->revx)revx1=TRUE;
																			else if(!pdat1->muki && pdat1->revx)revx1=TRUE;
																			else revx1=FALSE;
																			if(pdat2->muki && !pdat2->revx)revx2=TRUE;
																			else if(!pdat2->muki && pdat2->revx)revx2=TRUE;
																			else revx2=FALSE;
																			for(k=0;k<3;k++){
																				for(l=0;l<3;l++){
																					if(gbl.Syototu2RECTs(h_a.attack[k],h_k.kas[l],&kas_point[num_kas],
																						c_a.gcx,c_a.gcy,revx1,pdat1->revy,pdat1->rot,pdat1->x,pdat1->y,pdat1->magx,pdat1->magy,
																						c_k.gcx,c_k.gcy,revx2,pdat2->revy,pdat2->rot,pdat2->x,pdat2->y,pdat2->magx,pdat2->magy,magmode1,magmode2))
																					{
																						num_kas++;
																					}
																					if(gbl.Syototu2RECTs(h_a.attack[k],h_k.kurai[l],&kas_point[num_kas],
																						c_a.gcx,c_a.gcy,revx1,pdat1->revy,pdat1->rot,pdat1->x,pdat1->y,pdat1->magx,pdat1->magy,
																						c_k.gcx,c_k.gcy,revx2,pdat2->revy,pdat2->rot,pdat2->x,pdat2->y,pdat2->magx,pdat2->magy,magmode1,magmode2))
																					{
																						num_kas++;
																					}
																				}
																			}
																			if(num_kas>0){//一つ以上の矩形が衝突した
																				kas_point2.x=0;
																				kas_point2.y=0;
																				for(k=0;k<num_kas;k++){
																					kas_point2.x += kas_point[k].x;
																					kas_point2.y += kas_point[k].y;
																				}
																				kas_point2.x /= num_kas;
																				kas_point2.y /= num_kas;
																				Atari(p_objects[i]->data.id,p_objects[j]->data.id,kas_point2);
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	g_system.PopSysTag();
}



/*----------------------------------------------------------------
	飛道具の判定処理
------------------------------------------------------------------*/
void CBattleTaskNet::T_Sousai()
{
}



/*----------------------------------------------------------------
	ターゲット変更処理
------------------------------------------------------------------*/
void CBattleTaskNet::T_ChangeTarget()
{
}



/*================================================================

	描画処理

==================================================================*/
void CBattleTaskNet::Draw()
{
//	if(m_pause_task)return;

	g_system.PushSysTag(__FUNCTION__);

	if (efct_flash>0){	// 画面フラッシュ
		g_draw.d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, RGB(255,255,255), 0.0f, 0);
		g_system.PopSysTag();
		return;		// 描画しない
	}
	SetTransform(TRUE);
	g_draw.EnableZ();

	//描画用リスト準備
	DWORD i;
	std::vector<CGObject*> objlist;
	for(i=0;i<(int)p_objects.size();i++){
		if(p_objects[i]!=NULL){
			objlist.push_back(p_objects[i]);
		}
	}
	std::sort(objlist.begin(),objlist.end(),CGObject::ZCompare);//zソート

	//背景ステージ描画
	if(g_system.sw_showbg && !(efct_nobg>0)){//（←背景なしエフェクト）
		for(i=0;i<objlist.size();i++){
			objlist[i]->Message(GOBJMSG_DRAWBACK);
		}
	}
	
	g_draw.d3ddev->Clear(0,NULL,D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,0,1.0f,0);// clear z buffer
	if (!g_draw.StencilEnable())
		g_draw.d3ddev->Clear(0,NULL,D3DCLEAR_ZBUFFER,0,1.0f,0);// clear z buffer

	//背景暗くするエフェクト
	MYVERTEX3D vb[4];
	D3DXMATRIX mati;
	if(efct_darkbg>0){
		vb[0].color = vb[1].color = vb[2].color = vb[3].color = 0xAA000000;
		vb[0].z = vb[1].z = vb[2].z = vb[3].z = -1;
		vb[0].tu = vb[0].tv = vb[1].tu = vb[1].tv = vb[2].tu = vb[2].tv = vb[3].tu = vb[3].tv = 0;
		vb[0].x =    0; vb[0].y =  0;
		vb[1].x =  2.5; vb[1].y =  0;
		vb[2].x =    0; vb[2].y =  2;
		vb[3].x =  2.5; vb[3].y =  2;
		g_draw.d3ddev->SetTexture(0,NULL);
		D3DXMatrixIdentity(&mati);
		SetTransform(FALSE);
		g_draw.d3ddev->SetTransform(D3DTS_WORLD,&mati);
		g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
		g_draw.d3ddev->Clear(0,NULL,D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,0,1.0f,0);// clear z buffer
		SetTransform(TRUE);
	}

	if (!g_draw.StencilEnable())
		g_draw.d3ddev->Clear(0,NULL,D3DCLEAR_ZBUFFER,0,1.0f,0);// clear z buffer

	//通常位置描画
	for(i=0;i<objlist.size();i++){
		objlist[i]->Message(GOBJMSG_DRAW);
	}

	// 影
	if (g_draw.StencilEnable())
	{
		g_draw.d3ddev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL);
		g_draw.SetTransform(FALSE);
		g_draw.ReduceColor(0x66, true);
		g_draw.SetTransform(TRUE);
		g_draw.d3ddev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	}

	g_draw.d3ddev->Clear(0,NULL,D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,0,1.0f,0);// clear z buffer
	if (!g_draw.StencilEnable())
		g_draw.d3ddev->Clear(0,NULL,D3DCLEAR_ZBUFFER,0,1.0f,0);// clear z buffer
	
	int hitdispx;
	CGObject *pobj;
	if(!(!g_system.sw_showbg && g_config.IsDebugMode())){
		//ヒットカウントとダメージ
		SetTransform(FALSE);
		pobj=(CGObject*)GetActiveCharacter(0);
		if(pobj!=NULL){
			if(pobj->hitcount>1){
				hitdispx=640-bf_hitdisp[0]*20;
				if(hitdispx<550)hitdispx=550;
				if(bf_hitdisp[0]<120){
					float scale = (float)(30 - (int)bf_hitdisp[0]) / 15.0f;

					if (scale < 1.0f) scale = 1.0f;
					DrawNumber(pobj->hitcount,hitdispx,120,TRUE,0.0f,scale,scale);
					DrawNumber2(pobj->sexydamage_anim,hitdispx-25,145,0.0f);

					// 表示用の数字アニメ
					if (pobj->sexydamage > pobj->sexydamage_anim)
					{
						pobj->sexydamage_anim += pobj->sexydamage_haba;
						if (pobj->sexydamage < pobj->sexydamage_anim)
							pobj->sexydamage_anim = pobj->sexydamage;
					}
					else if  (pobj->sexydamage < pobj->sexydamage_anim)
					{
						// まあ無いだろうけど…
						pobj->sexydamage_anim -= pobj->sexydamage_haba;
						if (pobj->sexydamage > pobj->sexydamage_anim)
							pobj->sexydamage_anim = pobj->sexydamage;
					}
				}
			}
		}
		pobj=(CGObject*)GetActiveCharacter(1);
		if(pobj!=NULL){
			if(pobj->hitcount>1){
				hitdispx=bf_hitdisp[1]*20-120;
				if(hitdispx>5)hitdispx=5;
				if(bf_hitdisp[1]<120){
					float scale = (float)(30 - (int)bf_hitdisp[1]) / 15.0f;

					if (scale < 1.0f) scale = 1.0f;
					DrawNumber(pobj->hitcount,hitdispx,120,TRUE,0.0f,scale,scale);
					DrawNumber2(pobj->sexydamage_anim,hitdispx+25,145,0.0f);

					// 表示用の数字アニメ
					if (pobj->sexydamage > pobj->sexydamage_anim)
					{
						pobj->sexydamage_anim += pobj->sexydamage_haba;
						if (pobj->sexydamage < pobj->sexydamage_anim)
							pobj->sexydamage_anim = pobj->sexydamage;
					}
					else if  (pobj->sexydamage < pobj->sexydamage_anim)
					{
						// まあ無いだろうけど…
						pobj->sexydamage_anim -= pobj->sexydamage_haba;
						if (pobj->sexydamage > pobj->sexydamage_anim)
							pobj->sexydamage_anim = pobj->sexydamage;
					}
				}
			}
		}
	}
	bf_hitdisp[0]++;
	bf_hitdisp[1]++;
	SetTransform(TRUE);


	BOOL show_kas;
	BOOL show_atr;
	BOOL show_atk;
	BOOL reverse_x;

	GOBJECT *pdat;

	if(g_system.sw_rects){//当たり判定表示
		for(i=0;i<(int)p_objects.size();i++){
			if(p_objects[i]!=NULL){//オブジェクトが存在する
				pdat = &(p_objects[i]->data);
				if(pdat->tid==TEAM_PLAYER1 || pdat->tid==TEAM_PLAYER2){
					if(pdat->pcdat!=NULL && pdat->phdat!=NULL){
						show_kas=FALSE;
						show_atr=FALSE;
						show_atk=FALSE;
						if(pdat->objtype & GOBJFLG_ATTACK){//オブジェクトは攻撃を行う
							if(TRUE){//pdat->kougeki){//攻撃力ON
								show_atk=TRUE;
							}
						}
						if(pdat->objtype & GOBJFLG_ZBULLET){//飛び道具属性
							show_atk=TRUE;
						}
						if(pdat->objtype & GOBJFLG_KURAI){//オブジェクトは攻撃を喰らう
							if(TRUE){//!pdat->muteki){//無敵ではない
								show_kas=TRUE;
								show_atr=TRUE;
							}
						}
						if(show_kas || show_atr || show_atk){
							reverse_x=FALSE;
							if(pdat->revx)reverse_x=!reverse_x;
							if(pdat->muki)reverse_x=!reverse_x;
							g_draw.HanteiDraw(pdat->pcdat,
								pdat->phdat,
								pdat->cnow,
								show_atr,
								show_kas,
								show_atk,
								(int)pdat->x,
								(int)pdat->y,
								0,
								pdat->rot,
								reverse_x,
								pdat->revy,
								pdat->magx,
								pdat->magy);
						}
					}
				}
			}
		}
	}


	//前景ステージ描画
	if(!(!g_system.sw_showbg && g_config.IsDebugMode())){
		for(i=0;i<objlist.size();i++){
			objlist[i]->Message(GOBJMSG_DRAWFRONT);
		}
	}

	// フェードアウト効果
/*	if (tex_fb)
	{
		if (efct_fadein > 0)
		{
			float ar = 320.0f/240.0f;

			vb[0].color = 
			vb[1].color = 
			vb[2].color = 
			vb[3].color = ((int)((float)efct_fadein / 20.0f * 0xFF) * 0x01000000) | 0x00FFFFFF;

			vb[0].x =  0.0f*ar;
			vb[1].x =  0.0f*ar;
			vb[2].x =  2.0f*ar;
			vb[3].x =  2.0f*ar;

			vb[0].y =  0.0f;
			vb[1].y =  2.0f;
			vb[2].y =  0.0f;
			vb[3].y =  2.0f;

			vb[0].z = 
			vb[1].z = 
			vb[2].z = 
			vb[3].z = 0.0f;

			vb[0].tu = 0.0f;
			vb[1].tu = 0.0f;
			vb[2].tu = 1.0f;
			vb[3].tu = 1.0f;
			
			vb[0].tv = 0.0f;
			vb[1].tv = 1.0f;
			vb[2].tv = 0.0f;
			vb[3].tv = 1.0f;

			D3DXMatrixIdentity(&mati);
			SetTransform(FALSE);
			g_draw.d3ddev->SetTransform(D3DTS_WORLD,&mati);
			g_draw.d3ddev->SetTexture(0,tex_fb);
			g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vb,sizeof(MYVERTEX3D));
			SetTransform(TRUE);
		}
		else
			RELEASE(tex_fb);
	}
*/
/*	if(debugmsgbuff!=NULL){
		switch(g_system.sw_showstates%5){
		case 1:DrawCharacterState();break;
		case 2:DrawCharacterState2();break;
		case 3:DrawState();break;
		case 4:DrawObjectList();break;
		}
	}
*/
	g_system.PopSysTag();
}

HRESULT CALLBACK CBattleTaskNet::DPlayCallBack(PVOID UserCont, DWORD mtype, PVOID pmes)
{
	CBattleTaskNet* pClass = dynamic_cast<CBattleTaskNet*>(g_system.GetCurrentMainTask());

	return pClass->DPlayMessage(UserCont, mtype, pmes);
}

HRESULT CBattleTaskNet::DPlayMessage(PVOID UserCont, DWORD mtype, PVOID pmes)
{
	switch (mtype)
	{
	case DPN_MSGID_RECEIVE:
		{
			PDPNMSG_RECEIVE pMsg = (PDPNMSG_RECEIVE)pmes;

			switch (*pMsg->pReceiveData)
			{
			case GNETMSG_TESTSYNC:
			{
				struct TestSyncMes* pmes = (struct TestSyncMes*)pMsg->pReceiveData;
				CGObject* pobj = GetGObject(pmes->id);

				if (pMsg->dwReceiveDataSize >= sizeof(struct TestSyncMes))
				{
					if (pobj->data.aid != pmes->aid){
						pobj->data.aid = pmes->aid;
						pobj->ActionIDChanged(FALSE, TRUE);
					}
					while (pobj->data.counter < pmes->counter)
						pobj->Message(GOBJMSG_ACTION);
					pobj->data.x = pmes->x;
					pobj->data.y = pmes->y;
					// pobj->data.cnow = pmes->cnow;
					pobj->data.muki = pmes->muki;
				}
				break;
			}

			case GNETMSG_SYNC2:
				{
						struct Sync2Mes* pmes = (struct Sync2Mes*)pMsg->pReceiveData;
						GOBJECT* pdat = GetCharacterInfo(pmes->pid / MAXNUM_TEAM, pmes->pid % MAXNUM_TEAM);

						pdat->hp = pmes->hp;
						pdat->gauge = pmes->gauge;
						break;
				}

			case GNETMSG_ACTION:
				{
					if (pMsg->dwReceiveDataSize >= sizeof(struct ActionMes) && !g_play.IsHost())
					{
						struct ActionMes* pmes = (struct ActionMes*)pMsg->pReceiveData;

						int i;
						for(i=0;i<(int)p_objects.size();i++){
							CGObject* pobj = p_objects[i];

	/*						if (pobj->data.aid != pmes->aid){
								pobj->data.aid = pmes->aid;
								pobj->ActionIDChanged(FALSE, TRUE);
							}
	*/						if ( pobj && BATTLETASK_ISNOTFXOBJ((&pobj->data)) &&
								 pobj->dll_id != 0 && pobj->dll_id != 7 &&
									(!pmes->isStop || pobj->data.objtype & GOBJFLG_DONOTSTOP || pobj->data.nonstop) )
								pobj->Message(GOBJMSG_ACTION);
						}
					}
					break;
				}

			case GNETMSG_HIT:
				{
					if (pMsg->dwReceiveDataSize >= sizeof(struct HitMes))
					{
						struct HitMes* pmes = (struct HitMes*)pMsg->pReceiveData;
						CGObject* attacker = GetGObject(pmes->oid);

						if (IsLocal(attacker->dll_id))
							attacker->Message(GOBJMSG_TOUCHB,pmes->hit);
					}
					break;
				}

			case GNETMSG_INPUT:
				{
					if (pMsg->dwReceiveDataSize >= sizeof(struct InputMes))
					{
						struct InputMes* pmes = (struct InputMes*)pMsg->pReceiveData;

						g_input.SetKeyNet(1, pmes->key);
					}
				}
				break;
			}
			break;
		}
	}

	return DPN_OK;
}



//********************************************************************************
// オブジェクトリスト管理系
//********************************************************************************

DWORD CBattleTaskNet::CreateGObject()
{
	g_system.PushSysTag(__FUNCTION__);

	for(DWORD i=0;i<(int)p_objects.size();i++){
		if(p_objects[i]==NULL && object_regno[i] == 0){
			p_objects[i] = new CGObject( object_regindex[m_crnt_dllid]++ | ((m_crnt_dllid<<16) & 0xFFFF0000) );

			if (i == p_objects.size() - 1)
			{
				// 最大値なので、配列を広げる
				p_objects.resize( p_objects.size() + OBJECTS_MEMINCRATE );
				object_regno.resize( object_regno.size() + OBJECTS_MEMINCRATE );
			}

			g_system.PopSysTag();
			return(p_objects[i]->data.id);
		}
	}

	g_system.Log("オブジェクトが最大数を超えています\n",SYSLOG_WARNING);
	g_system.PopSysTag();
	return(0);
}

DWORD CBattleTaskNet::CreateGObjectFx()
{
	g_system.PushSysTag(__FUNCTION__);

	// ちょっと修正がいるよ
	for(DWORD i=0;i<(int)p_objects.size();i++){
		if(p_objects[i]==NULL){
			p_objects[i] = new CGObject( i | ((object_regno[i]<<16) & 0xFFFF0000) );

			if (i == p_objects.size() - 1)
			{
				// 最大値なので、配列を広げる
				p_objects.resize( p_objects.size() + OBJECTS_MEMINCRATE );
				object_regno.resize( object_regno.size() + OBJECTS_MEMINCRATE );
			}

			p_objects[i]->data.id |= BATTLETASK_FXOBJFLAG;
			g_system.PopSysTag();
			return(p_objects[i]->data.id);
		}
	}

	g_system.Log("オブジェクトが最大数を超えています\n",SYSLOG_WARNING);
	g_system.PopSysTag();
	return( 0 );
}

void CBattleTaskNet::DeleteGObject(DWORD oid)
{
	g_system.PushSysTag(__FUNCTION__);

	//ゼロ・オブジェクト
	if(oid==0){
		g_system.LogWarning("%s ゼロ・オブジェクト削除未遂");
		g_system.PopSysTag();
		return;
	}

	CGObject *pgobj = GetGObject(oid);
	if(pgobj==NULL){
		g_system.PopSysTag();
		return;
	}

	//キャラクター・ステージは消えられません
	for(int j=0;j<2;j++){
		for(int i=0;i<3;i++)
		{
			if(charobjid[j][i]==oid){
				g_system.LogWarning("%s キャラクターオブジェクト削除未遂(%d,%d)",__FUNCTION__,j,i);
				g_system.PopSysTag();
				return;
			}
		}
	}
	if(stgobjid==oid){
		g_system.LogWarning("%s ステージオブジェクト削除未遂",__FUNCTION__);
		g_system.PopSysTag();
		return;
	}

	DELETECLASS(pgobj);
	p_objects[oid&0x0000FFFF]=NULL;
	object_regno[oid&0x0000FFFF]++;

	g_system.PopSysTag();
}

void CBattleTaskNet::SuicideGObject(DWORD oid)
{
	if(GetGObject(oid)==NULL)return;

	suicide_list.push_back(oid);
}

DWORD CBattleTaskNet::MessageFromObject(DWORD oid,DWORD msg,DWORD prm)
{
	return(0);
}

BOOL CBattleTaskNet::CatchObject(DWORD eoid,LPVOID cy)
{
	if(cy==NULL)return(FALSE);

	CGObject *peobj = (CGObject*)GetGObject(eoid);
	if(peobj==NULL)return(FALSE);

	if(!(peobj->data.objtype & GOBJFLG_NAGERARE))return(FALSE);//相手が投げられフラグを持っていなかったら失敗
	if(!peobj->data.nagerare)return(FALSE);
	if(peobj->nage_muteki_cnt>0)return FALSE;
	if(peobj->data.counter==0)return(FALSE);//相手が行動遷移直後だったら止めておく
	if(peobj->data.aid & ACTID_GUARD)return(FALSE);//ガード中も、一応ダメってことにしておく
	if(peobj->data.aid & ACTID_NAGE)return(FALSE);//投げ中も、一応ダメってことにしておく
	if(peobj->data.aid & ACTID_INOUT)return(FALSE);//交代orストライカー攻撃中

	if(!(peobj->data.aid&ACTID_KURAI)){
		peobj->hitcount=0;
	}

	//成功
	peobj->data.aid = ACTID_NAGERARE;
	peobj->ActionIDChanged(TRUE,TRUE);
	peobj->data.muteki=TRUE;//喰らい判定OFF
	peobj->data.kasanari=FALSE;//重なり判定OFF
	peobj->data.nagerare=FALSE;//投げられ判定OFF
	peobj->cy = *(CATCHYOU*)cy;

	return(TRUE);
}

CGObject* CBattleTaskNet::GetGObject(DWORD oid)
{
	DWORD id1 = oid & 0x0000FFFF;			//配列インデックス
	DWORD id2 = (oid >> 16) & 0x00007FFF;	//生成カウント

	if(oid & BATTLETASK_FXOBJFLAG){
		if(id1 >= (int)p_objects.size())
			return(NULL);
		if(p_objects[id1]==0){return(NULL);}

		if(id2 == object_regno[id1]){
			return( p_objects[id1] );
		}
	}
	else{
		for (int i = 0; i < (int)p_objects.size(); i++){
			if (p_objects[i] && p_objects[i]->data.id == oid)
				return( p_objects[i] );
		}
	}
	return(NULL);
}

CGObject* CBattleTaskNet::GetCharacterObject(DWORD j,DWORD i)
{
	if(j>=2 || i>=MAXNUM_TEAM){
		g_system.LogWarning("%s 範囲チェックエラー(team=%d,index=%d)",__FUNCTION__,j,i);
		return 0;
	}

	return( GetGObject(charobjid[j][i]) );
}



void CBattleTaskNet::AddEffect(DWORD efctid,int prm1,int prm2,int prm3)
{
}



//****************************************************************
//  Atari - 矩形衝突時の処理
//****************************************************************

void CBattleTaskNet::Atari(DWORD a_id,DWORD k_id,MY2DVECTOR &kas_point)
{
	g_system.PushSysTag(__FUNCTION__);

	CGObject *attacker=GetGObject(a_id);
	CGObject *higaisya=GetGObject(k_id);

	if(attacker==NULL || higaisya==NULL){
		g_system.PopSysTag();
		return;
	}
	
	//とりあえず当たったことを通知
	if(attacker->Message(GOBJMSG_TOUCHC,k_id)==TOUCHC_CANCEL)
	{
		g_system.PopSysTag();
		return;
	}

	//元の攻撃情報を保存(atk2)
	ATTACKINFO2 tmpatkinfo = higaisya->data.atk2;
	{
		//喰らったダメージ情報をコピー
		higaisya->data.atk2.info1 = attacker->data.atk;
		higaisya->data.atk2.oid = a_id;
		//フラグ立
		higaisya->data.atk2.flags = 0;
		//画面端で相手を押し戻すかどうか
		if(higaisya->data.objtype & GOBJFLG_CLIPX){
			if(attacker->data.objtype & GOBJFLG_HANSAYOU){
				higaisya->data.atk2.flags |= ATKINFO2_ATTACKERBACK;
			}
		}
		//のけぞりの向き
		if(attacker->data.muki){
			if(attacker->data.atk->muki){
				higaisya->data.atk2.flags |= ATKINFO2_RIGHTBACK;
			}
		}
		else{
			if(!(attacker->data.atk->muki)){
				higaisya->data.atk2.flags |= ATKINFO2_RIGHTBACK;
			}
		}
	}

	//コンピュータ制御ならば、キーをいれてやる
	BOOL cool_guard = FALSE;
	if(higaisya->data.objtype & GOBJFLG_COMPUTER)
	{
		g_input.EnableComKey();
		DWORD comgrdkey = 0;
		if(rand()%100 < higaisya->comguard){
			comgrdkey = KEYSTA_BACK;
		}
		
		cool_guard = (rand()%8 < higaisya->com_level) ? TRUE : FALSE;

		//上下段判定
		if(cool_guard)
		{
			if(attacker->data.atk->guard & GUARDINFO_XSTAND)
			{
				comgrdkey |= KEYSTA_DOWN;
			}
		}
		//適当
		else
		{
			if(rand()%2 == 0){
				comgrdkey |= KEYSTA_DOWN;
			}
		}
		g_input.SetComKey(comgrdkey);
	}

	DWORD res = higaisya->Message(GOBJMSG_TOUCHA,a_id);//反応をみる
	g_input.DisableComKey();

	GOBJECT		*pdat  =&(higaisya->data);
	ATTACKINFO  *aif = attacker->data.atk;

	// 削りでやられそうなら喰らわせる
	if (res & 0x20000000 && bf_state==BFSTATE_FIGHTING && aif->kezuri >= pdat->hp)
		res |= 0x10000000;

	double dmkanwa;
	DWORD i;

	if(res & 0x10000000){//喰らった
		dmkanwa=1.0;//ダメージ緩和量
		//hit count
		if(pdat->aid&ACTID_KURAI){
			higaisya->hitcount++;
			if(higaisya->data.id == charobjid[higaisya->data.tid][active_character[higaisya->data.tid]]){//連続ヒット表示
				if(higaisya->hitcount==2)
					bf_hitdisp[pdat->tid]=0;
				else bf_hitdisp[pdat->tid]=30;
			}
		}
		else{
			higaisya->hitcount=1;
			if(higaisya->data.id == charobjid[higaisya->data.tid][active_character[higaisya->data.tid]]){//連続ヒット表示
				bf_hitdisp[pdat->tid]=0;
			}
		}
		for(i=1;i<higaisya->hitcount;i++){
			dmkanwa*=RDMGKANWA;
		}
		if(dmkanwa<DMGKANWA_MINIMUM)dmkanwa=DMGKANWA_MINIMUM;

		if(bf_state==BFSTATE_FIGHTING)pdat->hp -= (DWORD)(pdat->atk2.info1->damage * dmkanwa);
		if(higaisya->hitcount == 1)
			higaisya->sexydamage  = (DWORD)(pdat->atk2.info1->damage * dmkanwa);
		else if(higaisya->hitcount>=2){
			higaisya->sexydamage += (DWORD)(pdat->atk2.info1->damage * dmkanwa);

			if(higaisya->hitcount == 2)		// 表示用も設定
				higaisya->sexydamage_anim = higaisya->sexydamage;
			else							// 幅設定
			{
				higaisya->sexydamage_haba = abs((int)higaisya->sexydamage - (int)higaisya->sexydamage_anim) / 8;
				if (higaisya->sexydamage_haba == 0)
					higaisya->sexydamage_haba = 1;
			}
		}

		switch(aif->hit & 0x000F0000){//ヒットマーク描画
		case 0:break;
		case HITINFO_MARK1:AddEffect(EFCTID_MARK1,(int)kas_point.x,(int)kas_point.y);break;
		case HITINFO_MARK2:AddEffect(EFCTID_MARK2,(int)kas_point.x,(int)kas_point.y);break;
		case HITINFO_MARK3:AddEffect(EFCTID_MARK3,(int)kas_point.x,(int)kas_point.y);break;
		case HITINFO_MARK4:AddEffect(EFCTID_MARK4,(int)kas_point.x,(int)kas_point.y);break;
		}
		switch(aif->hit & 0x00F00000){//効果音
		case 0:break;
		case HITINFO_SNDHIT1:g_system.PlaySystemSound(SYSTEMSOUND_HIT1);break;
		case HITINFO_SNDHIT2:g_system.PlaySystemSound(SYSTEMSOUND_HIT2);break;
		case HITINFO_SNDHIT3:g_system.PlaySystemSound(SYSTEMSOUND_HIT3);break;
		case HITINFO_SNDSHK1:g_system.PlaySystemSound(SYSTEMSOUND_SHOCK1);break;
		case HITINFO_SNDSHK2:g_system.PlaySystemSound(SYSTEMSOUND_SHOCK2);break;
		}
		switch(aif->hit & 0x0F000000){//ヒットストップ
		case 0:break;
		case HITINFO_SIV1:HitStop( 5,k_id);break;
		case HITINFO_SIV2:HitStop(10,k_id);break;
		case HITINFO_SIV3:HitStop(15,k_id);break;
		case HITINFO_STOP:HitStop(40,k_id);break;
		}
		int vib_type = 0;		// 振動の強さ[0..2]
		if(!(res & 0x40000000)){
			switch(aif->hit & 0x0000000F){
			case HITINFO_REACT1://通常弱攻撃
				switch(res & 0x0000000F){
				case 1:pdat->aid=ACTID_DAMAGE1;break;//立ち
				case 2:pdat->aid=ACTID_DAMAGEC1;break;//しゃがみ
				case 3:pdat->aid=ACTID_DAMAGEJ1;break;//空中
				}
				vib_type = 0;
				break;
			case HITINFO_REACT2://通常中攻撃
				switch(res & 0x0000000F){
				case 1:pdat->aid=ACTID_DAMAGE2;break;//立ち
				case 2:pdat->aid=ACTID_DAMAGEC2;break;//しゃがみ
				case 3:pdat->aid=ACTID_DAMAGEJ2;break;//空中
				}
				vib_type = 1;
				break;
			case HITINFO_REACT3://通常強攻撃
				switch(res & 0x0000000F){
				case 1:pdat->aid=ACTID_DAMAGE3;break;//立ち
				case 2:pdat->aid=ACTID_DAMAGEC3;break;//しゃがみ
				case 3:pdat->aid=ACTID_DAMAGEJ3;break;//空中
				}
				vib_type = 2;
				break;
			case HITINFO_DOWN://足払いなどでの転倒
				pdat->aid=ACTID_DOWN;
				vib_type = 1;
				break;
			case HITINFO_FUTTOBI://ぶっ飛び
				pdat->aid=ACTID_FUTTOBI;
				vib_type = 2;
				break;
			case HITINFO_FUTTOBI2://ぶっ飛び2
				pdat->aid=ACTID_FUTTOBI2;
				vib_type = 2;
				break;
			case HITINFO_REACT1A://通常弱攻撃、のけぞりなし
				switch(res & 0x0000000F){
				case 1:pdat->aid=ACTID_DAMAGE1A;break;//立ち
				case 2:pdat->aid=ACTID_DAMAGEC1A;break;//しゃがみ
				case 3:pdat->aid=ACTID_DAMAGEJ1;break;//空中（は、普通）
				}
				vib_type = 0;
				break;
			case HITINFO_REACT2A://通常中攻撃、のけぞりなし
				switch(res & 0x0000000F){
				case 1:pdat->aid=ACTID_DAMAGE2A;break;//立ち
				case 2:pdat->aid=ACTID_DAMAGEC2A;break;//しゃがみ
				case 3:pdat->aid=ACTID_DAMAGEJ2;break;//空中（は、普通）
				}
				vib_type = 1;
				break;
			case HITINFO_REACT3A://通常強攻撃、のけぞりなし
				switch(res & 0x0000000F){
				case 1:pdat->aid=ACTID_DAMAGE3A;break;//立ち
				case 2:pdat->aid=ACTID_DAMAGEC3A;break;//しゃがみ
				case 3:pdat->aid=ACTID_DAMAGEJ3;break;//空中（は、普通）
				}
				vib_type = 2;
				break;
			case HITINFO_TATAKI1A:
				pdat->aid=ACTID_TATAKITUKE1A;
				vib_type = 2;
				break;
			case HITINFO_TATAKI2A:
				pdat->aid=ACTID_TATAKITUKE2A;
				vib_type = 2;
				break;
			case HITINFO_TATAKI1B:
				pdat->aid=ACTID_TATAKITUKE1B;
				vib_type = 2;
				break;
			case HITINFO_TATAKI2B:
				pdat->aid=ACTID_TATAKITUKE2B;
				vib_type = 2;
				break;
			case HITINFO_USERDEFINE://ユーザー定義喰らい
				switch(res & 0x0000000F){
				case 1://立ち
				case 2:pdat->aid=ACTID_USERHIT1;break;//しゃがみ
				case 3:pdat->aid=ACTID_USERHIT2;break;//空中
				}
				higaisya->userhit_attacker = a_id;
				higaisya->userhit_id = (aif->hit&HITINFO_USERIDMASK)>>4;
				vib_type = 1;
				break;
			case HITINFO_USERDEFINE2://ユーザー定義喰らい（２）
				pdat->aid=ACTID_USERHIT2;//常に空中
				higaisya->userhit_attacker = a_id;
				higaisya->userhit_id = (aif->hit&HITINFO_USERIDMASK)>>4;
				vib_type = 1;
				break;
			}
		}

		if(pdat->hp<=0){//死亡
			if(g_battleinfo.GetBattleType()==TAISENKEISIKI_GOCYAMAZE)
				pdat->aid=ACTID_FINALDOWN;
			else
				pdat->aid=ACTID_KAITENFINISH;
			if(aif->hit & HITINFO_EFCTBURN )AddEffect(EFCTID_BURN,0,0,k_id);
			if(aif->hit & HITINFO_EFCTBURN_B )AddEffect(EFCTID_BURN_B,0,0,k_id);
			if(aif->hit & HITINFO_EFCTBURN_G )AddEffect(EFCTID_BURN_G,0,0,k_id);
		}

		//エフェクト
		if((aif->hit & 0x0000F000) & HITINFO_EFCTSINDO)AddEffect(EFCTID_SINDO,2,20);
		if((aif->hit & 0x0000F000) & HITINFO_EFCTBURN  )AddEffect(EFCTID_BURN  ,0,0,k_id);
		if((aif->hit & 0x0000F000) & HITINFO_EFCTBURN_B)AddEffect(EFCTID_BURN_B,0,0,k_id);
		if((aif->hit & 0x0000F000) & HITINFO_EFCTBURN_G)AddEffect(EFCTID_BURN_G,0,0,k_id);
		if (aif->hit & HITINFO_EFCTFLASH) AddEffect(EFCTID_FLASH,4,0);

		//フィードバック（暫定）
		g_input.StartForce( g_battleinfo.GetKeyAssign((attacker->dll_id - 1) / MAXNUM_TEAM, (attacker->dll_id - 1) % MAXNUM_TEAM), vib_type );
		g_input.StartForce( g_battleinfo.GetKeyAssign((higaisya->dll_id - 1) / MAXNUM_TEAM, (higaisya->dll_id - 1) % MAXNUM_TEAM), vib_type );

		//喰らったとき、行動がストップして格好悪いので1回だけaction()させる
		if(!(res & 0x40000000)){
			higaisya->ActionIDChanged(TRUE,TRUE);
			higaisya->Message(GOBJMSG_ACTION);
		}
		//攻撃が当たったことを攻撃したやつに通知
		attacker->Message(GOBJMSG_TOUCHB,TRUE);
/*		struct HitMes hmes;

		hmes.msgid = GNETMSG_HIT;
		hmes.oid = a_id;
//		hmes.aid = aif->id;
		hmes.hit = TRUE;
		g_play.SendMsg(DPNID_ALL_PLAYERS_GROUP, hmes, sizeof(hmes), 10000,
			DPNSEND_GUARANTEED | DPNSEND_PRIORITY_HIGH | DPNSEND_NOLOOPBACK | DPNSEND_NONSEQUENTIAL);	// 送り先限定した方がよさそう
*/

		// HP情報を送信
		if (g_play.IsHost()){
					struct SyncHPMes s2m;

					s2m.msgid = GNETMSG_SYNCHP;
					s2m.pid = higaisya->dll_id;
					s2m.hp = higaisya->data.hp;
					g_play.SendMsg(DPNID_ALL_PLAYERS_GROUP, s2m, sizeof(s2m), 200,
									DPNSEND_NOLOOPBACK | DPNSEND_NONSEQUENTIAL);
		}
	}
	else if(res & 0x20000000){//ガードした
		if(bf_state==BFSTATE_FIGHTING)pdat->hp -= aif->kezuri;//削り
		if(aif->kezuri==0)AddEffect(EFCTID_MARKG,(int)kas_point.x,(int)kas_point.y);//ガードマーク描画
		else AddEffect(EFCTID_MARKG2,(int)kas_point.x,(int)kas_point.y);
		switch(aif->guard & 0x0F000000){//ヒットストップ
		case 0:break;
		case GUARDINFO_SIV1:HitStop( 5,k_id);break;
		case GUARDINFO_SIV2:HitStop(10,k_id);break;
		case GUARDINFO_SIV3:HitStop(15,k_id);break;
		case GUARDINFO_STOP:HitStop(40,k_id);break;
		}
		g_system.PlaySystemSound(SYSTEMSOUND_GUARD);//効果音
		if(!(res & 0x40000000)){
			switch(aif->guard & 0x0000000F){
			case GUARDINFO_REACT1://弱硬直
				switch(res & 0x0000000F){
				case 1:pdat->aid=ACTID_GUARD1;break;//立ち
				case 2:pdat->aid=ACTID_GUARDC1;break;//しゃがみ
				case 3:pdat->aid=ACTID_GUARDJ1;break;//空中
				}
				break;
			case GUARDINFO_REACT2://中硬直
				switch(res & 0x0000000F){
				case 1:pdat->aid=ACTID_GUARD2;break;//立ち
				case 2:pdat->aid=ACTID_GUARDC2;break;//しゃがみ
				case 3:pdat->aid=ACTID_GUARDJ2;break;//空中
				}
				break;
			case GUARDINFO_REACT3://強硬直
				switch(res & 0x0000000F){
				case 1:pdat->aid=ACTID_GUARD3;break;//立ち
				case 2:pdat->aid=ACTID_GUARDC3;break;//しゃがみ
				case 3:pdat->aid=ACTID_GUARDJ3;break;//空中
				}
				break;
			case GUARDINFO_USERDEFINE://ユーザー定義硬直
				switch(res & 0x0000000F){
				case 1://立ち
				case 2:pdat->aid=ACTID_USERGUARD1;break;//しゃがみ
				case 3:pdat->aid=ACTID_USERGUARD2;break;//空中（は、普通）
				}
				higaisya->userhit_attacker = a_id;
				higaisya->userhit_id = (aif->hit&GUARDINFO_USERIDMASK)>>4;
				break;
			}
		}
		switch(aif->hit & 0x0000F000){//エフェクト
		case GUARDINFO_EFCTSINDO:
			AddEffect(EFCTID_SINDO,2,20);
			break;
		case GUARDINFO_EFCTFLASH:
			AddEffect(EFCTID_FLASH,4,0);
			break;
		}
	
		if(pdat->hp<=0){//死亡
			if(g_battleinfo.GetBattleType()==TAISENKEISIKI_GOCYAMAZE)
				pdat->aid=ACTID_FINALDOWN;
			else
				pdat->aid=ACTID_KAITENFINISH;
		}

		//喰らったとき、行動がストップして格好悪いので1回だけaction()させる
		if(!(res & 0x40000000)){
			higaisya->ActionIDChanged(TRUE,TRUE);
			higaisya->Message(GOBJMSG_ACTION);
		}
		//攻撃が当たったことを攻撃したやつに通知
		attacker->Message(GOBJMSG_TOUCHB,FALSE);
/*		struct HitMes hmes;

		hmes.msgid = GNETMSG_HIT;
		hmes.oid = a_id;
		hmes.hit = FALSE;
		g_play.SendMsg(DPNID_ALL_PLAYERS_GROUP, hmes, sizeof(hmes), 10000,
			DPNSEND_GUARANTEED | DPNSEND_PRIORITY_HIGH | DPNSEND_NOLOOPBACK | DPNSEND_NONSEQUENTIAL);	// 送り先限定した方がよさそう
*/

		if (g_play.IsHost()){
			// HP情報を送信
					struct SyncHPMes s2m;

					s2m.msgid = GNETMSG_SYNCHP;
					s2m.pid = higaisya->dll_id;
					s2m.hp = higaisya->data.hp;
					g_play.SendMsg(DPNID_ALL_PLAYERS_GROUP, s2m, sizeof(s2m), 200,
									DPNSEND_NOLOOPBACK | DPNSEND_NONSEQUENTIAL);
		}
	}
	else{
		//攻撃を無視したならば元の攻撃力情報に戻しておく
		higaisya->data.atk2 = tmpatkinfo;
	}
	
	g_system.PopSysTag();
}

//**********************************************************************

void CBattleTaskNet::HitStop(DWORD len,DWORD oid)
{
	if(GetGObject(oid)!=NULL)
		GetGObject(oid)->siver=len;
	if(efct_hitstop < (int)len)
		efct_hitstop=len;
}



void CBattleTaskNet::SetTransform(BOOL b)
{
	float dy_sindo=0;
	if(efct_sindo > 0){
		if( (timeGetTime()/30)%2==0 )dy_sindo = efct_sindom/240.0f;
		else dy_sindo = efct_sindom/240.0f*(-1);
	}

	if(b){
		g_draw.camera_x = (float)GetDisplayCenterX()/320.0f  * ASPECTRATIO;
		g_draw.camera_y = -(float)tan(D3DXToRadian(40)) + dy_sindo;
		g_draw.ResetTransformMatrix();
		g_draw.ResetParentMatrix();
	}
	else{
		g_draw.camera_x = 1.0f*ASPECTRATIO;
		g_draw.camera_y = 1.0f;
		g_draw.ResetTransformMatrix();
		g_draw.ResetParentMatrix();
		g_draw.d3ddev->SetTransform(D3DTS_WORLD,&(g_draw.matparent));
	}
}

/*----------------------------------------------------------------

	試合状態の更新

------------------------------------------------------------------*/
void CBattleTaskNet::T_UpdateStatus()
{
}

/*-------------------------------------------------------------------------
	キー入力向き設定更新
---------------------------------------------------------------------------*/
void CBattleTaskNet::UpdateKeyInputDirections()
{
	BOOL flag;

	for(int k=0;k<MAXNUM_KEYI;k++){
		for(int i=0;i<2;i++){
			for(UINT j=0;j<g_battleinfo.GetNumTeam(i);j++)
			{

				if(g_battleinfo.GetBattleType()==TAISENKEISIKI_GOCYAMAZE) flag=TRUE;
				else if(j==GetActiveCharacterID(i)) flag=TRUE;
				else flag=FALSE;

				if(GetCDICharacterInfo(i,j)->keyinput==k && flag)
				{
					CGObject* obj = GetGObject( charobjid[i][j] );
					if(obj){
						if(!(obj->data.objtype&GOBJFLG_COMPUTER))	//コンピュータ制御ではない
						{
							g_muki[k] = obj->data.muki;
						}
					}
				}
			}
		}
	}
}


/*-------------------------------------------------------------------------
	F7でポーズ
---------------------------------------------------------------------------*/
void CBattleTaskNet::WndMessage(HWND hWnd,UINT msg,WPARAM wparam, LPARAM lparam)
{
	if(msg==WM_KEYDOWN){
/*		if(wparam==VK_F7){
			if(m_pause_task)
			{
				m_pause_task->Kill();
				m_pause_task = NULL;
			}
			else
			{
				m_pause_task = new CTBattlePause;
				g_system.AddTask( m_pause_task );
			}
		}
		*/
		if(wparam=='K'){
			for (int i = 0; i <= 1; i++){
				CGObject* pdat = this->GetCharacterObject(i,0);

				if (pdat->data.aid == ACTID_FINALDOWN2){
					pdat->data.hp = pdat->data.hpmax;
					pdat->data.aid = ACTID_OKIAGARI;
					pdat->ActionIDChanged(TRUE, TRUE);
				}
			}
		}
	}
}