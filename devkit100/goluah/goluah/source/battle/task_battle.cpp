/*
2011/10/29	timeoverのボイスに対応
            数字の独立
            交代・ストライカーで呼ぶ方向を左右逆に
*/
/*============================================================================

    戦闘タスククラス

    （ネットワーク非対応版）

	Goluah!! Copyright (C) 2001-2004 aki, 2014-2015 logger, 2004-2015 At-sushi

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

==============================================================================*/

#include "stdafx.h"

#include "define_macro.h"
#include "global.h"
#include "task_battle.h"
#include "task_loading.h"
#include "gcdhandler.h"

/*----------------------------------------------------------------------------
    構築
------------------------------------------------------------------------------*/
CBattleTask::CBattleTask()
{	
    debugmsgbuff = NULL;
    if(g_config.IsDebugMode()){
        debugmsgbuff = (char*)malloc( DEBUGMSGBUFFERSIZE );
    }

    cp_gauge = NULL;
    cp_efctlist = NULL;
    m_pause_task = NULL;
    efct_fadein = 0;
    tex_fb = NULL;
    score[0] = score[1] = 0;
}

/*----------------------------------------------------------------------------
    破棄
------------------------------------------------------------------------------*/
CBattleTask::~CBattleTask()
{
    Terminate();

    if (debugmsgbuff)
        free(debugmsgbuff);
    RELEASE(tex_fb);
}

/*===========================================================================

    タスク　初期化

=============================================================================*/
void CBattleTask::Initialize()
{
    CTNowLoading* now_loading = new CTNowLoading;
    g_system.AddTask(now_loading);//Now Loading表示タスク開始

    char filename[256];
    int i,j;

    if (!g_config.IsFullScreen())
        AfxGetApp()->DoWaitCursor(1);
    // 動的配列初期化
    p_objects.clear();
	object_regno.clear();
    suicide_list.resize(0);

    CBattleTaskBase::Initialize();

    //コンピュータ制御フラグ付加
    double hp_ratio=1.0;
    for(i=0;i<3;i++){
        for(j=0;j<2;j++){
            if((charobjid[j][i]&0x0000FFFF)<(int)p_objects.size() && charobjid[j][i]!=0){
                if(GetGObject( charobjid[j][i] )!=NULL)
                {
                    if(g_battleinfo.GetCharacterIsCOM(j,i))
                    {
                        GetGObject( charobjid[j][i] )->data.objtype |= GOBJFLG_COMPUTER;
                        //体力の最大値 = DLLが設定した値 x 難易度設定による補正値
                        switch(GetGObject( charobjid[j][i] )->com_level){
                        case DIFF_VERYEASY	:hp_ratio=0.75;	break;
                        case DIFF_EASY		:hp_ratio=0.875;break;
                        case DIFF_HARD		:hp_ratio=1.1;	break;
                        case DIFF_VERYHARD	:hp_ratio=1.2;	break;
                        case DIFF_SUPERHARD	:hp_ratio=1.3;	break;
                        case DIFF_ULTRAHARD	:hp_ratio=1.4;	break;
                        case DIFF_LIMITERCUT:hp_ratio=1.5;	break;
                        default:hp_ratio=1.0;break;
                        }
                        GetGObject( charobjid[j][i] )->data.hpmax = (DWORD)( GetGObject( charobjid[j][i] )->data.hpmax * hp_ratio );
                    }
                    GetGObject( charobjid[j][i] )->data.hp = GetGObject( charobjid[j][i] )->data.hpmax ;
                }
            }
        }
    }

    // リプレイファイル作成
    /*time_t crnt_time;
    time(&crnt_time);
    struct tm* crnt_time_l = localtime(&crnt_time);
    sprintf(filename,"%s\\%d%s%d%s%d%s%d%s%d%s%d.%s",
        "system\\replay\\",
        crnt_time_l->tm_year + 1900,				//年
        (crnt_time_l->tm_mon + 1)<10 ? "0" : "",
        crnt_time_l->tm_mon + 1,					//月
        crnt_time_l->tm_mday<10 ? "0" : "",
        crnt_time_l->tm_mday,						//日
        crnt_time_l->tm_hour<10 ? "0" : "",
        crnt_time_l->tm_hour,						//時
        crnt_time_l->tm_min<10 ? "0" : "",
        crnt_time_l->tm_min,						//分
        crnt_time_l->tm_sec<10 ? "0" : "",
        crnt_time_l->tm_sec,						//秒
        "grd"										//拡張子
    );
    RepFile.Open(filename,CFile::modeWrite | CFile::modeCreate);*/

    StartRound();

    //bgm
    BOOL story_bgm_on = FALSE;
    if(g_battleinfo.GetStoryBGM()){//ストーリーで指定された場合
        if(!g_sound.BGMPlay(g_battleinfo.GetStoryBGM()))
        {
            // .stage/bgm/から探してみる
            CString tmpcstr(g_battleinfo.GetStoryBGM());
            int yen_pos = tmpcstr.ReverseFind('\\');
            if(yen_pos>0){
                tmpcstr = tmpcstr.Right( tmpcstr.GetLength()-yen_pos-1 );
                tmpcstr = "stage/bgm/" + tmpcstr;
                gbl.ods((LPCTSTR)tmpcstr);
                story_bgm_on = g_sound.BGMPlay((LPCTSTR)tmpcstr);
            }
        }
        else story_bgm_on = TRUE;
    }
    if(!story_bgm_on)
    {
        //ステージディレクトリのbgmの再生を試みる
        sprintf(filename,"%s\\bgm",
            g_stagelist.GetStageDir(g_battleinfo.GetStage()));
        if(!g_sound.BGMPlay(filename))
        {
            //ステージ名と同一のbgm再生を試みる
            sprintf(filename,"stage\\bgm\\%s",g_stagelist.GetStageDir(g_battleinfo.GetStage()));
            if(!g_sound.BGMPlay(filename)){
                gbl.PlayRandomBGM("stage\\bgm");
            }
        }
    }

    AfxGetApp()->DoWaitCursor(-1);
    g_system.RemoveTask('LOAD');//NowLoading 表示タスク除去
}


/*----------------------------------------------------------------
    オブジェクトリスト初期化
------------------------------------------------------------------*/
void CBattleTask::InitializeObjectList()
{
    g_system.PushSysTag(__FUNCTION__);

    for(int i=0;i<(int)object_regno.size();i++){
        object_regno[i]=0;
    }
    object_regindex=0;

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
void CBattleTask::InitializeParameters()
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
            hprecratio[j][i]=6;
        }
    }
}


/*----------------------------------------------------------------
    その他クラスを初期化
    ゲージ・エフェクト
------------------------------------------------------------------*/
void CBattleTask::InitializeSubTasks()
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
void CBattleTask::StartRound()
{
    int i,j;
    round++;
    char filename[256];
    BOOL call_round=TRUE;

    //試合終了判定
    switch( g_battleinfo.GetBattleType() ){
    case TAISENKEISIKI_GOCYAMAZE:	
        disp_center_x=0;
        limittime=g_battleinfo.GetLimitTime();
        //勝ちが先取ポイント数以上ならば勝利画面へ
        if(wincount[0]>=g_config.GetMaxPoint()){
            g_battleresult.Initialize(0);
            battle_end = TRUE;
            return;
        }
        else if(wincount[1]>=g_config.GetMaxPoint()){			
            g_battleresult.Initialize(1);
            battle_end = TRUE;
            return;
        }
        for(j=0;j<2;j++){
            for(i=0;i<(int)g_battleinfo.GetNumTeam(j);i++){		
                if(GetGObject(charobjid[j][i])!=NULL)
                    GetGObject(charobjid[j][i])->data.objtype |= GOBJTYPE_DISPLAY;
            }
        }
        break;
    case TAISENKEISIKI_JYUNBAN://ラウンド終了 = 試合終了
    case TAISENKEISIKI_KOUTAI:
        disp_center_x=0;
        limittime=-1;
        if(round!=1)
        {
            g_battleresult.Initialize( m_all_dead[1] ? 0 : 1 );
            battle_end = TRUE;
            return;
        }
        break;
    }



    //「ラウンドX」サウンドロード
    if(call_round){
        sprintf(filename,".\\system\\sound\\round%d.wav",round);
        dsb_round = g_sound.CreateDSB(filename);
    }

    if(round!=1){//「ラウンドが変わったら消滅する」オブジェクトを全部消す
        for(i=1;i<(int)p_objects.size();i++){
            if(p_objects[i]!=NULL){
                if(!p_objects[i]->Message(GOBJMSG_CNGROUND)){
                    DELETECLASS(p_objects[i]);
                }
            }
        }
    }

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
    
    bf_state = BFSTATE_WAITFORENDPOSE;
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

    CGObject *pobj;

    //支援攻撃要請カウンタリセット
    strikercall_counter[0] = strikercall_counter[1] = 0;

    //登場ポーズ移行設定
    if(round==1){
        for(j=0;j<2;j++){
            for(i=0;i<(int)g_battleinfo.GetNumTeam(j);i++){
                pobj = (CGObject*)GetGObject( charobjid[j][i] );
                if(pobj!=NULL){
                    pobj->Message(GOBJMSG_DOTOJYO);
                    pobj->data.objtype |= GOBJFLG_DISPLAYME;
                }
                m_tojyo_end[j][i] = FALSE;
            }
            for(;i<MAXNUM_TEAM;i++){
                m_tojyo_end[j][i] = TRUE;
            }
        }
    }
    else{
        for(j=0;j<2;j++){
            for(i=0;i<MAXNUM_TEAM;i++){
                m_tojyo_end[j][i] = TRUE;
            }
        }
    }

    // フェードアウト開始
    RELEASE(tex_fb);
    tex_fb = g_draw.GetFrontBufferCopy();
    efct_fadein = 20;
}



/*===========================================================================

    タスク　破棄

=============================================================================*/
void CBattleTask::TerminateObjectList()
{
    g_system.PushSysTag(__FUNCTION__);

    for(UINT i=0;i<(int)p_objects.size();i++){
        DELETECLASS(p_objects[i]);
    }

    g_system.PopSysTag();
}

void CBattleTask::TerminateDestroySubTasks()
{
    g_system.PushSysTag(__FUNCTION__);

    if(cp_gauge)cp_gauge->CleanUp();
    if(cp_efctlist)cp_efctlist->Destroy();

    DELETECLASS( cp_gauge );
    DELETECLASS( cp_efctlist );

    g_system.PopSysTag();
}


/*===========================================================================

    タスク　実行

=============================================================================*/
BOOL CBattleTask::Execute(DWORD time)
{
    static char execute_tag[256];
    sprintf(execute_tag,"%s (%s)",__FUNCTION__,GetBattleStateString());
    g_system.PushSysTag(execute_tag);

    if(m_pause_task){
        g_system.PopSysTag();
        return TRUE;
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

    //オブジェクトリスト処理。消滅したがっているオブジェクトを消滅させる
    for(auto i : suicide_list){
        if(i!=0){
            DeleteGObject(i);
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
void CBattleTask::T_Command()
{
    g_system.PushSysTag(__FUNCTION__);
    
    int i;
    for(i=0;i<(int)p_objects.size();i++){
        if(p_objects[i]!=NULL)
            p_objects[i]->Message(GOBJMSG_COMMAND);
    }

    g_system.PopSysTag();
}


/*----------------------------------------------------------------
    ACTIONメッセージをオブジェクトに送信
------------------------------------------------------------------*/
void CBattleTask::T_Action(BOOL stop)
{
    g_system.PushSysTag(__FUNCTION__);

    int i;
    for(i=0;i<(int)p_objects.size();i++){
        if(p_objects[i]!=NULL){
            if(!stop)
                p_objects[i]->Message(GOBJMSG_ACTION);
            else if(p_objects[i]->data.objtype & GOBJFLG_DONOTSTOP ||
                    p_objects[i]->data.nonstop)
                p_objects[i]->Message(GOBJMSG_ACTION);

            // 暫定
            ////RepFile.Write(&p_objects[i]->data, sizeof(GOBJECT));
            //RepFile.Write(&p_objects[i]->data.aid, sizeof(DWORD));
        }
    }

    g_system.PopSysTag();
}


/*----------------------------------------------------------------
    重なり判定処理
------------------------------------------------------------------*/
void CBattleTask::T_KasanariHantei()
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
        if(p_objects[i]!=NULL){//オブジェクトが存在する
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
                            // 変更：自分より後のみに対して判定（ちょっと軽くなるかな）
                            for(j=i+1;j<(int)p_objects.size();j++){//** 他の全てのオブジェクトに対して **
                                if(/*i!=j*/TRUE){//自分以外に
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
        if(p_objects[i]!=NULL){
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
void CBattleTask::T_AtariHantei()
{
    g_system.PushSysTag(__FUNCTION__);

    static BOOL hantaihantei=FALSE;
    hantaihantei = !hantaihantei;
    static std::vector<DWORD> kurai_list;

    int num_kas;
    MY2DVECTOR kas_point[3*6],kas_point2;

    GOBJECT *pdat1,*pdat2;
    GCD_CELL2  c_a,c_k;
    GCD_HANTEI h_a,h_k;
    BOOL revx1,revx2;
    UINT magmode1,magmode2;

    ASSERT(kurai_list.empty());

    int i, j, k, l;
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
                                            if(p_objects[j]!=NULL){
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
                                                                                if (Atari(p_objects[i]->data.id, p_objects[j]->data.id, kas_point2))
                                                                                    kurai_list.push_back(pdat2->id);// 後でやる
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
                                            if(p_objects[j]!=NULL){
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
                                                                                if (Atari(p_objects[i]->data.id, p_objects[j]->data.id, kas_point2))
                                                                                    kurai_list.push_back(pdat2->id);// 後でやる
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

    //喰らったとき、行動がストップして格好悪いので1回だけaction()させる
    CGObject* objkurai;
    for (i = 0; i<(int)kurai_list.size(); i++){
        if (objkurai = GetGObject(kurai_list[i]))
        {
            objkurai->ActionIDChanged(TRUE, TRUE);
            objkurai->Message(GOBJMSG_ACTION);
            GetGObject(objkurai->data.atk2.oid)->Message(GOBJMSG_TOUCHB, TRUE);
        }
        else break;
    }
    kurai_list.clear();

    g_system.PopSysTag();
}



/*----------------------------------------------------------------
    飛道具の判定処理
------------------------------------------------------------------*/
void CBattleTask::T_Sousai()
{
    g_system.PushSysTag(__FUNCTION__);

    int i,j,k,l;
    int num_kas;
    MY2DVECTOR kas_point[3*6];
    GOBJECT *pdat1,*pdat2;
    GCD_CELL2  c_a,c_k;
    GCD_HANTEI h_a,h_k;
    BOOL revx1,revx2;
    UINT magmode1,magmode2;

    for(i=1;i<(int)p_objects.size()-1;i++){
        if(p_objects[i]!=NULL){//オブジェクトが存在する
            pdat1 = &(p_objects[i]->data);
            if((pdat1->tid==TEAM_PLAYER1 || pdat1->tid==TEAM_PLAYER2)  && BATTLETASK_ISNOTFXOBJ(pdat1)){
                if(pdat1->objtype & GOBJFLG_ZBULLET){//オブジェクトは飛び道具属性を持つ
                    if(pdat1->kougeki){//攻撃力を失っていない
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
                                for(j=i+1;j<(int)p_objects.size();j++){//** 他の全てのオブジェクトに対して **
                                    if(p_objects[j]!=NULL){
                                        pdat2 = &(p_objects[j]->data);
                                        if((pdat2->tid==TEAM_PLAYER1 || pdat2->tid==TEAM_PLAYER2)  && BATTLETASK_ISNOTFXOBJ(pdat2)){
                                            if(pdat1->tid != pdat2->tid){
                                                if(pdat2->objtype & GOBJFLG_ZBULLET){//オブジェクトは飛び道具属性を持つ
                                                    if(pdat2->kougeki ){//攻撃力を失っていない
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
                                                                        if(gbl.Syototu2RECTs(h_a.attack[k],h_k.attack[l],&kas_point[num_kas],
                                                                            c_a.gcx,c_a.gcy,revx1,pdat1->revy,pdat1->rot,pdat1->x,pdat1->y,pdat1->magx,pdat1->magy,
                                                                            c_k.gcx,c_k.gcy,revx2,pdat2->revy,pdat2->rot,pdat2->x,pdat2->y,pdat2->magx,pdat2->magy,magmode1,magmode2))
                                                                        {
                                                                            num_kas++;
                                                                        }
                                                                    }
                                                                }
                                                                if(num_kas>0){//一つ以上の矩形が衝突した
                                                                    p_objects[i]->Message(GOBJMSG_SOUSAI,0);
                                                                    p_objects[j]->Message(GOBJMSG_SOUSAI,0);
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
    ターゲット変更処理
------------------------------------------------------------------*/
void CBattleTask::T_ChangeTarget()
{
    g_system.PushSysTag(__FUNCTION__);

    double minimum_distance;
    int i,j;
    static int change_target_index=0;
    DWORD newtarget;

    GOBJECT *pdat,*pedat;

    newtarget=0;
    change_target_index += 64;
    if(change_target_index >= (int)p_objects.size())
        change_target_index = 0;
    for(i=change_target_index;i<min(change_target_index+64, (int)p_objects.size());i++){
        if(p_objects[i]!=NULL){
            pdat=&(p_objects[i]->data);
            if((pdat->objtype & GOBJFLG_NEEDTARGET) && BATTLETASK_ISNOTFXOBJ(pdat)){//ターゲットが必要
                minimum_distance=9999*9999;
                for(j=0;j<(int)p_objects.size();j++){
                    if(j!=i){
                        if(p_objects[j]!=NULL){
                            pedat=&(p_objects[j]->data);
                            if((pdat->tid!=pedat->tid) && BATTLETASK_ISNOTFXOBJ(pedat)){//チームが違う
                                if(pedat->objtype & GOBJFLG_TOBETARGET){//ターゲットになれる
                                    if(pedat->hp > 0){//まだ生きている
                                        //距離を測る
                                        if( (pedat->x-pdat->x)*(pedat->x-pdat->x) < minimum_distance){//近い
                                            minimum_distance = (pedat->x-pdat->x)*(pedat->x-pdat->x);
                                            newtarget=j;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if(p_objects[newtarget]->data.id != pdat->eid && newtarget!=0){
                    p_objects[i]->Message(GOBJMSG_CNGTARGET,p_objects[newtarget]->data.id);
                }
            }
        }
    }

    g_system.PopSysTag();
}



/*================================================================

    描画処理

==================================================================*/
void CBattleTask::Draw()
{
    if(m_pause_task)return;

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
                    DrawNumber5(pobj->hitcount,hitdispx,120,TRUE,0.0f,scale,scale);
                    DrawNumber6(pobj->sexydamage_anim,hitdispx+25,145,0.0f);

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
    if (tex_fb)
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

    if(debugmsgbuff!=NULL){
        switch(g_system.sw_showstates%5){
        case 1:DrawCharacterState();break;
        case 2:DrawCharacterState2();break;
        case 3:DrawState();break;
        case 4:DrawObjectList();break;
        }
    }

    g_system.PopSysTag();
}



//********************************************************************************
// オブジェクトリスト管理系
//********************************************************************************

DWORD CBattleTask::CreateGObject()
{
    g_system.PushSysTag(__FUNCTION__);

    for(DWORD i=0;i<(int)p_objects.size() + 1;i++){
        if(p_objects[i]==NULL){
            p_objects[i] = new CGObject( i | ((object_regno[i]<<16) & 0xFFFF0000) );

            g_system.PopSysTag();
            return(p_objects[i]->data.id);
        }
    }

    g_system.Log("オブジェクトが最大数を超えています\n",SYSLOG_WARNING);
    g_system.PopSysTag();
    return(0);
}

DWORD CBattleTask::CreateGObjectFx()
{
    g_system.PushSysTag(__FUNCTION__);

    DWORD ret = CreateGObject();
    if(ret == 0){
        g_system.PopSysTag();
        return 0;
    }

    CGObject* gobj = GetGObject(ret);
    gobj->data.id |= BATTLETASK_FXOBJFLAG;

    g_system.PopSysTag();
    return( gobj->data.id );
}

void CBattleTask::DeleteGObject(DWORD oid)
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

void CBattleTask::SuicideGObject(DWORD oid)
{
    if(GetGObject(oid)==NULL)return;

    suicide_list.push_back(oid);
}

DWORD CBattleTask::MessageFromObject(DWORD oid,DWORD msg,DWORD prm)
{
    g_system.PushSysTag(__FUNCTION__);

    //変数準備
    CGObject *pdat = GetGObject(oid);
    if(pdat==NULL){
        g_system.LogWarning("%s msg=%08X ,オブジェクトNULL",__FUNCTION__,msg);
        g_system.PopSysTag();
        return(0);
    }
    
    BOOL striker_front=FALSE;
    DWORD team = pdat->data.tid;	//所属チーム
    DWORD team2 = team==TEAM_PLAYER1 ? TEAM_PLAYER2 : TEAM_PLAYER1;	//敵チーム

    UINT cidx=0;
    for(int i=0;i<MAXNUM_TEAM;i++){
        if(oid == charobjid[team][i]){
            cidx = i;
        }
    }


    switch(msg){
    //■登場終了
    case MSGOBJ2SYS_TOJYOEND:
        //同時対戦でなければ、非アクティブキャラクターに待機メッセージ発信
        if(g_battleinfo.GetBattleType() != TAISENKEISIKI_GOCYAMAZE){
            if(oid != charobjid[team][active_character[team]]){
                GetGObject(oid)->Message(GOBJMSG_TAIKI,0);
            }
        }//↓の処理も続けて行う
    //■交代後登場ポーズ終了
    case MSGOBJ2SYS_KOUTAIEND:
        m_tojyo_end[team][cidx]=TRUE;
        break;
    //■最終ダウン終了
    case MSGOBJ2SYS_FINALDOWNEND:
        m_finaldown_end[team]=TRUE;
        break;
    //■勝利ポーズ終了
    case MSGOBJ2SYS_WINPOSEEND:
        if(oid == winner_oid){
            m_winpose_end=TRUE;
        }
        if(g_battleinfo.GetBattleType()==TAISENKEISIKI_JYUNBAN){//KOFLikeの場合、ニュートラルに戻してやる
            GetGObject(oid)->data.aid = ACTID_NEUTRAL;
        }
        break;
    //■交代要請(キャラクター1)
    case MSGOBJ2SYS_KOUTAI1:
        striker_front = TRUE;//あとは↓と同様の処理（変数名は気にしないで）
    //■交代要請(キャラクター2)
    case MSGOBJ2SYS_KOUTAI2:
        {
            if(g_battleinfo.GetBattleType()!=TAISENKEISIKI_KOUTAI){//対戦形式チェック
                g_system.PopSysTag();
                return(FALSE);
            }
            if(bf_state != BFSTATE_FIGHTING){//試合状態チェック
                g_system.PopSysTag();
                return(FALSE);
            }

            //「次の」キャラクター取得
            DWORD next_act;
            switch(cidx){	//HPとface1の配置変更に伴い、左右を逆に
                case 0:next_act= striker_front ? 2 : 1 ;break;
                case 1:next_act= striker_front ? 0 : 2 ;break;
                case 2:next_act= striker_front ? 1 : 0 ;break;
                default:
                    g_system.LogWarning("%s msg=%08X ,失敗(cidx=%d)",__FUNCTION__,msg,cidx);
                    g_system.PopSysTag();
                    return(FALSE);
            }
            if(charobjid[team][next_act]==0){
                g_system.PopSysTag();
                return FALSE;
            }
            pdat=(CGObject*)GetGObject( charobjid[team][next_act] );
            if(!pdat){
                g_system.LogWarning("%s msg=%08X ,失敗(オブジェクトNULL)",__FUNCTION__,msg,cidx);
                g_system.PopSysTag();
                return(FALSE);
            }

            //交代メッセージ送信
            if(pdat->Message(GOBJMSG_KOUTAI,charobjid[team][cidx]))
            {
                hprecratio[team][cidx]*=1.8;		//HP回復インターバル増
                active_character[team]=next_act;//"アクティブ" キャラクター更新
                g_system.PopSysTag();
                return(TRUE);					//成功
            }
            else g_system.Log("◆交代要請拒否。交代失敗。\n",SYSLOG_DEBUG);
            g_system.PopSysTag();
            return(FALSE);
        }
        break;

    //■ストライカー要請(キャラクター1)
    case MSGOBJ2SYS_STRIKER1:
        striker_front=TRUE;//あとは↓と同様の処理
    //■ストライカー要請(キャラクター2)
    case MSGOBJ2SYS_STRIKER2:
        {
            //チェック
            {
                BOOL striker_ck = TRUE;
                if(g_battleinfo.GetBattleType()==TAISENKEISIKI_GOCYAMAZE){	striker_ck=FALSE; }//対戦形式チェック
                if(bf_state != BFSTATE_FIGHTING){ striker_ck=FALSE; }//試合状態チェック
                if(strikercount[team]==0){ striker_ck=FALSE; }//ストライカーポイントチェック
    
                if(!striker_ck){
                    g_system.PopSysTag();
                    return FALSE;
                }
            }
            
            //「次の」キャラクター取得
            DWORD next_act;
            switch(cidx){	//HPとface1の配置変更に伴い、左右を逆に
                case 0:next_act= striker_front ? 2 : 1 ;break;
                case 1:next_act= striker_front ? 0 : 2 ;break;
                case 2:next_act= striker_front ? 1 : 0 ;break;
                default:
                    g_system.PopSysTag();
                    return(FALSE);
            }
            if(charobjid[team][next_act]==0){
                g_system.PopSysTag();
                return FALSE;
            }
            pdat=(CGObject*)GetGObject( charobjid[team][next_act] );
            if(!pdat){
                g_system.PopSysTag();
                return(FALSE);
            }

            if(pdat->Message(GOBJMSG_STRIKER,charobjid[team][cidx]))//ストライカー要請問い合わせ
            {
                //OK
                strikercount[team]--;
                g_system.PopSysTag();
                return(TRUE);
            }
            else gbl.ods("★ストライカー却下");
        }
        break;

    default:
        g_system.LogWarning("%s msg=%08X ,実装されてないメッセージ",__FUNCTION__,msg);
    }

    g_system.PopSysTag();
    return(0);
}

BOOL CBattleTask::CatchObject(DWORD eoid,LPVOID cy)
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

CGObject* CBattleTask::GetGObject(DWORD oid)
{
    DWORD id1 = oid & 0x0000FFFF;			//配列インデックス
    DWORD id2 = (oid >> 16) & 0x00007FFF;	//生成カウント

    if(id1 >= (int)p_objects.size())
        return(NULL);
    if(p_objects[id1]==0){return(NULL);}

    if(id2 == object_regno[id1]){
        return( p_objects[id1] );
    }
    return(NULL);
}

CGObject* CBattleTask::GetCharacterObject(DWORD j,DWORD i)
{
    if(j>=2 || i>=MAXNUM_TEAM){
        g_system.LogWarning("%s 範囲チェックエラー(team=%d,index=%d)",__FUNCTION__,j,i);
        return 0;
    }

    return( GetGObject(charobjid[j][i]) );
}



void CBattleTask::AddEffect(DWORD efctid,int prm1,int prm2,int prm3)
{
    g_system.PushSysTag(__FUNCTION__);

    switch(efctid){
    //CBattleTaskが担うエフェクト
    case EFCTID_STOP:
        efct_stop = prm1;
        break;
    case EFCTID_DARK:
        efct_darkbg = prm1;
        break;
    case EFCTID_SINDO:
        efct_sindo = prm2;
        efct_sindom= prm1;

/*		// 振動エフェクトに残像をつけた
        // コマ落ちすることがある（Part37の>>705）ので戻しておきます
        RELEASE(tex_fb);
        tex_fb = g_draw.GetFrontBufferCopy();
        efct_fadein = min(prm2, 8);*/
        break;
    case EFCTID_NOBG:
        efct_nobg = prm1;
        break;
    case EFCTID_CYOHI:
        cp_efctlist->AddEffect(EFCTID_CYOHILIGHT,prm1,prm2,0);
        g_system.PlaySystemSound(SYSTEMSOUND_CYOHI);
        efct_stop = efct_darkbg = 30;
        break;
    case EFCTID_FLASH:
        efct_flash = prm1;
        break;
    default://あとはCEffectListに任せるyo
        cp_efctlist->AddEffect(efctid,prm1,prm2,prm3);
    }

    g_system.PopSysTag();
}



//****************************************************************
//  Atari - 矩形衝突時の処理
//****************************************************************

BOOL CBattleTask::Atari(DWORD a_id, DWORD k_id, MY2DVECTOR &kas_point)
{
    g_system.PushSysTag(__FUNCTION__);

    CGObject *attacker=GetGObject(a_id);
    CGObject *higaisya=GetGObject(k_id);

    if(attacker==NULL || higaisya==NULL){
        g_system.PopSysTag();
        return FALSE;
    }
    
    //とりあえず当たったことを通知
    if(attacker->Message(GOBJMSG_TOUCHC,k_id)==TOUCHC_CANCEL)
    {
        g_system.PopSysTag();
        return FALSE;
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
    BOOL actidchanged = FALSE;

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
        case HITINFO_SIV1:HitStop( 4,k_id);break;
        case HITINFO_SIV2:HitStop( 7,k_id);break;
        case HITINFO_SIV3:HitStop(13,k_id);break;
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
            AddEffect(EFCTID_SINDO,2,20);
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
            actidchanged = TRUE;
        }
        //攻撃が当たったことを攻撃したやつに通知
        attacker->Message(GOBJMSG_TOUCHB,TRUE);
    }
    else if(res & 0x20000000){//ガードした
        if(bf_state==BFSTATE_FIGHTING)pdat->hp -= aif->kezuri;//削り
        if(aif->kezuri==0)AddEffect(EFCTID_MARKG,(int)kas_point.x,(int)kas_point.y);//ガードマーク描画
        else AddEffect(EFCTID_MARKG2,(int)kas_point.x,(int)kas_point.y);
        switch(aif->guard & 0x0F000000){//ヒットストップ
        case 0:break;
        case GUARDINFO_SIV1:HitStop( 4,k_id);break;
        case GUARDINFO_SIV2:HitStop( 7,k_id);break;
        case GUARDINFO_SIV3:HitStop(13,k_id);break;
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
/*		case GUARDINFO_EFCTFLASH:			// 挙動が怪しいので保留
            AddEffect(EFCTID_FLASH,4,0);
            break;
*/		}
    
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
    }
    else{
        //攻撃を無視したならば元の攻撃力情報に戻しておく
        higaisya->data.atk2 = tmpatkinfo;
    }
    
    g_system.PopSysTag();
    return actidchanged;
}

//****************************************************************
// For Debug
//****************************************************************

void CBattleTask::DrawState()
{
    sprintf(debugmsgbuff,"objectid:\n");
    sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n player1 - %d , %d ,%d",charobjid[0][0],charobjid[0][0],charobjid[0][2]);
    sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n player2 - %d , %d ,%d",charobjid[1][0],charobjid[1][0],charobjid[1][2]);
    sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"bfstate:\n");
    switch(bf_state){
    case BFSTATE_WAITFORENDPOSE:sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"BFSTATE_WAITFORENDPOSE,%d",bf_counter);break;
    case BFSTATE_ROUNDCALL:		sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"BFSTATE_ROUNDCALL,%d",bf_counter);break;
    case BFSTATE_FIGHTING:		sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"BFSTATE_FIGHTING,%d",bf_counter);break;
    case BFSTATE_FINISHED:		sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"BFSTATE_FINISHED,%d",bf_counter);break;
    case BFSTATE_WAITFORENDWIN:	sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"BFSTATE_WAITFORENDWIN,%d",bf_counter);break;
    case BFSTATE_DOUBLEKO:		sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"BFSTATE_DOUBLEKO,%d",bf_counter);break;
    default:sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"不明？,%d",bf_counter);break;
    }
    sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n");
//	sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"num_char=%d , ",g_battleinfo.GetNumTeam());
//	switch(g_battleinfo.GetBattleType()){
//	case TAISENKEISIKI_GOCYAMAZE	:sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"TAISENKEISIKI_GOCYAMAZE , tl=%d\n",g_battleinfo.GetLimitTime());break;
//	case TAISENKEISIKI_KOUTAI		:sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"TAISENKEISIKI_KOUTAI , tl=%d\n",g_battleinfo.GetLimitTime());break;
//	case TAISENKEISIKI_JYUNBAN	:sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"TAISENKEISIKI_JYUNBAN , tl=%d\n",g_battleinfo.GetLimitTime());break;
//	case KEISIKI_END		:sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"KEISIKI_END , tl=%d\n",g_battleinfo.GetLimitTime());break;
//	default					:sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"不明？ , tl=%d\n",g_battleinfo.GetLimitTime());break;
//	}
    sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\ndisplay_x=%d , ",disp_center_x);

    RECT r;
    r.top=0;
    r.bottom=g_DISPLAYHEIGHT;
    r.left=0;
    r.right=g_DISPLAYWIDTH;
    g_draw.DrawBlueText(r,debugmsgbuff,-1,DT_LEFT,1);
}

void CBattleTask::DrawObjectList()
{
    sprintf(debugmsgbuff,"");
    for(int i=0;i<(int)p_objects.size();i++){
        if(i%10==0)sprintf(&debugmsgbuff[strlen(debugmsgbuff)]," ");
        if(i%40==0)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n");
        if(p_objects[i]!=NULL)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"●");
        else sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"○");
    }

    RECT r;
    r.top=0;
    r.bottom=g_DISPLAYHEIGHT;
    r.left=0;
    r.right=g_DISPLAYWIDTH;
    g_draw.DrawBlueText(r,debugmsgbuff,-1,DT_LEFT,1);
}

void CBattleTask::DrawCharacterState()
{
    GOBJECT   *pdat=NULL;

    RECT r;
    DWORD method;

    for(int j=0;j<2;j++){
        ZeroMemory(debugmsgbuff,DEBUGMSGBUFFERSIZE);
        //名前
        sprintf(debugmsgbuff,g_charlist.GetCharacterName(g_battleinfo.GetCharacter(j,active_character[j])));
        if(GetGObject( charobjid[j][active_character[j]] ) ==NULL){
            sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n\n 異常：オブジェクトがNULLです");
        }
        else{
            pdat = &(GetGObject( charobjid[j][active_character[j]] )->data);
            sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n ID:%d , チーム:%d , 敵:%d , ユーザーID:%d",pdat->id,pdat->tid,pdat->eid,pdat->uid);
            sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n cell#:%d",pdat->cnow);
            if(pdat->muki)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n 向き:<- ");
            else sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n 向き:-> ");
            if(pdat->revx)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"X反転 ");
            if(pdat->revy)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"Y反転 ");
            sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"拡大率(%5.2f,%5.2f) 回転:%d",
                pdat->magx,pdat->magy,pdat->rot);
            sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n 座標(%5.2f,%5.2f)",pdat->x,pdat->y);
            if(FALSE){
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n\n 異常：オブジェクト- pobjdat_a がNULLです");
            }
            else{
                //座標
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n 速度(%5.3f,%5.3f) - ｶﾛ速度(%5.3f,%5.3f)",
                    pdat->vx,pdat->vy,pdat->ax,pdat->ay);
                //行動ID
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n 行動ID:%d (",pdat->aid);
                if(pdat->aid & ACTID_KUCYU)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"空中 ");
                if(pdat->aid & ACTID_SYAGAMI)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"屈 ");
                if(pdat->aid & ACTID_ATTACK)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"攻撃 ");
                if(pdat->aid & ACTID_HISSATU)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"必殺 ");
                if(pdat->aid & ACTID_SYSTEM)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"システム ");
                if(pdat->aid & ACTID_KURAI)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"喰らい ");
                if(pdat->aid & ACTID_GUARD)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"ガード ");
                if(pdat->aid & ACTID_NAGE)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"投げ ");
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"- %d )",pdat->aid&0x00000FFFF);
            }
        }
        //テキスト描画
        r.top=0;
        r.bottom=g_DISPLAYHEIGHT;
        if(j==0){
            r.left=0;
            r.right=g_DISPLAYWIDTH/2;
            method = DT_LEFT;
        }
        else{
            r.left=g_DISPLAYWIDTH/2;
            r.right=g_DISPLAYWIDTH;
            method = DT_RIGHT;
        }
        g_draw.DrawBlueText(r,debugmsgbuff,-1,method,1);
    }
}

void CBattleTask::DrawCharacterState2()
{
    GOBJECT   *pdat=NULL;

    RECT r;
    DWORD method;

    for(int j=0;j<2;j++){
        ZeroMemory(debugmsgbuff,DEBUGMSGBUFFERSIZE);
        //名前
        sprintf(debugmsgbuff,g_charlist.GetCharacterName(g_battleinfo.GetCharacter(j,active_character[j])));
        if(GetGObject( charobjid[j][active_character[j]] ) ==NULL){
            sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n\n 異常：オブジェクトがNULLです");
        }
        else{
            pdat = &(GetGObject( charobjid[j][active_character[j]] )->data);
            if(FALSE){
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n\n 異常：オブジェクト- pobjdat_a がNULLです");
            }
            else{
                //カウンタ
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n counter=%d",pdat->counter);
                //体力・ゲージ
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n 体力:%d/%d  ゲージ:%1.4f/%lu.0000",pdat->hp,pdat->hpmax,pdat->gauge,pdat->gaugemax);
                //各スイッチ
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n 重なり判定");
                if(pdat->kasanari)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"ON");
                else sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"OFF");
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)]," 無敵");
                if(pdat->muteki)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"ON");
                else sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"OFF");
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n 攻撃力");
                if(pdat->kougeki)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"ON");
                else sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"OFF");
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)]," 投げられ");
                if(pdat->nagerare)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"ON");
                else sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"OFF");
            }
            if(FALSE){
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n\n 異常：オブジェクト- pobjdat_d がNULLです");
            }
            else{
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n 色:(%d / %d,%d,%d)",
                    (pdat->color&0xFF000000)/(256*256*256),
                    (pdat->color&0x00FF0000)/(256*256),
                    (pdat->color&0x0000FF00)/(256),
                    (pdat->color&0x000000FF));
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n z座標:%5.2f",pdat->z);
            }
        }
        //テキスト描画
        r.top=0;
        r.bottom=g_DISPLAYHEIGHT;
        if(j==0){
            r.left=0;
            r.right=g_DISPLAYWIDTH/2;
            method = DT_LEFT;
        }
        else{
            r.left=g_DISPLAYWIDTH/2;
            r.right=g_DISPLAYWIDTH;
            method = DT_RIGHT;
        }
        g_draw.DrawBlueText(r,debugmsgbuff,-1,method,1);
    }
}

//**********************************************************************

void CBattleTask::HitStop(DWORD len,DWORD oid)
{
    if(GetGObject(oid)!=NULL)
        GetGObject(oid)->siver=len;
    if(efct_hitstop < (int)len)
        efct_hitstop=len;
}



void CBattleTask::SetTransform(BOOL b)
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
void CBattleTask::T_UpdateStatus()
{
    BattleTaskState prv_state=bf_state;

    switch(bf_state)
    {
        case BFSTATE_WAITFORENDPOSE	:T_UpdateStatus_WaitForEndPose();	break;//登場ポーズ終了待ち
        case BFSTATE_ROUNDCALL		:T_UpdateStatus_RoundCall();		break;//ラウンドコール終了待ち
        case BFSTATE_FIGHTING		:T_UpdateStatus_Fighting();			break;//戦闘状態
        case BFSTATE_FINISHED		:T_UpdateStatus_Finished();			break;//KO、キャラクタの最終ダウン終了待ち
        case BFSTATE_WAITFORENDWIN	:T_UpdateStatus_WaitForEndWin();	break;//勝利ポーズ終了待ち
        case BFSTATE_DOUBLEKO		:T_UpdateStatus_DoubleKO();			break;//ダブルKO表示終了待ち
        case BFSTATE_TIMEOVER		:T_UpdateStatus_TimeOver();			break;//タイムオーバー表示終了待ち
    }

    bf_counter++;
    if(prv_state!=bf_state){
        bf_counter=0;
    }
}


//登場ポーズ終了待ち
void CBattleTask::T_UpdateStatus_WaitForEndPose()
{
    g_system.PushSysTag(__FUNCTION__);

    BOOL next=TRUE;

    //登場ポーズ終了フラグが全部ONになっているか？
    for(int i=0;i<2;i++){
        for(int j=0;j<MAXNUM_TEAM;j++){
            if(!m_tojyo_end[i][j]){
                next = FALSE;
            }
        }
    }
    if(bf_counter>600){//救済措置
        next=TRUE;
    }

    if(next){
        bf_state=BFSTATE_ROUNDCALL;
        gbl.ods("WAITFORENDPOSE → BFSTATE_ROUNDCALL");
    }

    g_system.PopSysTag();
}

//ラウンドコール終了待ち
void CBattleTask::T_UpdateStatus_RoundCall()
{
    g_system.PushSysTag(__FUNCTION__);

    if(bf_counter==2){
        if(dsb_round!=NULL)dsb_round->Play(0,0,0);//「ラウンド～」
        switch(round){
        case 1:AddEffect(EFCTID_ROUND1,0,-1000);break;
        case 2:AddEffect(EFCTID_ROUND2,0,-1000);break;
        case 3:AddEffect(EFCTID_ROUND3,0,-1000);break;
        case 4:AddEffect(EFCTID_ROUND4,0,-1000);break;
        case 5:AddEffect(EFCTID_ROUND5,0,-1000);break;
        case 6:AddEffect(EFCTID_ROUND6,0,-1000);break;
        }
    }
    if(bf_counter==70 * 1.5)//「ラウンド～」が終了する時間（大体）
    {
        if(dsb_fight!=NULL)dsb_fight->Play(0,0,0);//「ファイト」
        AddEffect(EFCTID_FIGHT,0,0);
    }
    if(bf_counter>130 * 1.5)//「ファイト」が終了する時間（大体）
    {
        bf_state=BFSTATE_FIGHTING;
        gbl.ods("BFSTATE_ROUNDCALL → BFSTATE_FIGHTING");
    }

    g_system.PopSysTag();
}

//戦闘状態
void CBattleTask::T_UpdateStatus_Fighting()
{
    g_system.PushSysTag(__FUNCTION__);

    int i,j;
    CGObject *pobj , *pobj2 , *pobj3;
    int k;

    //タイムリミット判定
    if(!act_stop && limittime>0)
    {
        if(bf_counter%70==0)limittime--;
        if(limittime==0){

            if(dsb_timeover!=NULL)dsb_timeover->Play(0,0,0);

            AddEffect(EFCTID_TIMEOVER,0,0,0);
            bf_state = BFSTATE_TIMEOVER;
            bf_counter = 0;
            gbl.ods("BFSTATE_FIGHTING → BFSTATE_TIMEOVER");
            g_system.PopSysTag();
            return;
        }
    }

    //死亡フラグ更新
    Update_DeadFlag();

    BOOL the_end=FALSE;

    switch(g_battleinfo.GetBattleType())
    {
        case TAISENKEISIKI_GOCYAMAZE:
            {
                //死んでいたら画面から外に出てもいい
                for(j=0;j<2;j++){
                    for(i=0;i<(int)g_battleinfo.GetNumTeam(j);i++){
                        if(m_dead_one[j][i] && !m_all_dead[j])
                        {
                            GetGObject(charobjid[j][i])->data.objtype &= ~GOBJFLG_DISPLAYME;
                            GetGObject(charobjid[j][i])->data.objtype &= ~GOBJFLG_CLIPX2;
                        }
                    }
                }
                //ラウンド終了判定
                if( m_all_dead[0] || m_all_dead[1] )
                {
                    the_end=TRUE;

                    //ダウン待ちフラグ設定
                    m_finaldown_end[0]=m_all_dead[0] ? FALSE : TRUE;
                    m_finaldown_end[1]=m_all_dead[1] ? FALSE : TRUE;
                }
            }
            break;
        case TAISENKEISIKI_JYUNBAN:
            {
                //ラウンド終了判定
                if( m_active_dead[0] || m_active_dead[1] )
                {
                    the_end=TRUE;

                    //ダウン待ちフラグ設定
                    m_finaldown_end[0]=m_active_dead[0] ? FALSE : TRUE;
                    m_finaldown_end[1]=m_active_dead[1] ? FALSE : TRUE;
                }

                else
                {
                    //支援攻撃要請判定
                    for(j=0;j<2;j++)
                    {
                        pobj = GetGObject(  charobjid[j][active_character[j]]  );

                        if( (pobj->data.objtype&GOBJFLG_COMPUTER) &&
                            !(pobj->data.aid&ACTID_NAGE) &&
                            !(pobj->data.aid&ACTID_SYSTEM) &&
                            !(pobj->data.aid&ACTID_KURAI) &&
                            !(pobj->data.aid&ACTID_GUARD) &&
                            !(pobj->data.aid&ACTID_INOUT) && 
                            (g_battleinfo.GetNumTeam(j) >= 2)
                            )
                        {
                            strikercall_counter[j]++;

                            if( (strikercall_counter[j]>700) && ((rand()%20)==0) )
                            {
                                if(MessageFromObject(
                                            pobj->data.id, 
                                            (strikercall_counter[j]%2==0) ? MSGOBJ2SYS_STRIKER1 : MSGOBJ2SYS_STRIKER2 ,
                                            0
                                            ))
                                {
                                    strikercall_counter[j] = 0;
                                    AddEffect(EFCTID_COMEON,(int)pobj->data.x,(int)pobj->data.y-100,pobj->data.muki);
                                }
                                else
                                {
                                    strikercall_counter[j] /= 2;
                                }
                            }
                        }
                    }//←支援攻撃要請判定ここまで
                }
            }
            break;
        case TAISENKEISIKI_KOUTAI:
            {
                //ラウンド終了判定
                if( m_all_dead[0] || m_all_dead[1] )
                {
                    the_end=TRUE;

                    //ダウン待ちフラグ設定
                    m_finaldown_end[0]=m_all_dead[0] ? FALSE : TRUE;
                    m_finaldown_end[1]=m_all_dead[1] ? FALSE : TRUE;
                }
                else
                {
                    //交代判定(死亡時)
                    for(j=0;j<2;j++)
                    {
                        if( m_active_dead[j] )
                        {
                            for(i=0;i<(int)g_battleinfo.GetNumTeam(j);i++)//生きているやつを探して交代する
                            {
                                pobj = GetGObject( charobjid[j][i] );
                                if(pobj!=NULL)
                                {
                                    if(pobj->Message(GOBJMSG_KOUTAI,charobjid[j][active_character[j]]))
                                    {
                                        active_character[j]=i;
                                        i=4;
                                    }
                                }
                            }
                            if(i==3)g_system.Log("◆キャラクターの交代に失敗\n",SYSLOG_WARNING);
                        }
                    }

                    //交代判定(生存時)
                    for(j=0;j<2;j++)
                    {
                        pobj = GetGObject(  charobjid[j][active_character[j]]  );
                        if( pobj->data.hp < (int)pobj->data.hpmax/3 && pobj->data.hp>0)
                        {
                            if( (pobj->data.objtype&GOBJFLG_COMPUTER) &&
                                !(pobj->data.aid&ACTID_ATTACK) &&
                                !(pobj->data.aid&ACTID_KUCYU) &&
                                !(pobj->data.aid&ACTID_HISSATU) &&
                                !(pobj->data.aid&ACTID_NAGE) &&
                                !(pobj->data.aid&ACTID_SYSTEM) &&
                                !(pobj->data.aid&ACTID_KURAI) &&
                                !(pobj->data.aid&ACTID_GUARD) &&
                                !(pobj->data.aid&ACTID_INOUT) && 
                                ( rand()%((pobj->data.counter<19)?(20-pobj->data.counter):2)==0 ) && 
                                (g_battleinfo.GetNumTeam(j) >= 2)
                              )
                            {
                                pobj3 = NULL;
                                for (i = 0; i<(int)g_battleinfo.GetNumTeam(j); i++)
                                {
                                    if(i!=active_character[j])
                                    {
                                        pobj2 = GetGObject(  charobjid[j][i]  );
                                        if(pobj2)
                                        {
                                            if(	((pobj2->data.hp > pobj->data.hp*3) && pobj2->data.hp>300)  )
                                            {
                                                if(!pobj3){
                                                    pobj3=pobj2;
                                                    k=i;
                                                }
                                                else if(pobj3->data.hp/(float)pobj3->data.hpmax > pobj2->data.hp/(float)pobj2->data.hpmax)
                                                {
                                                    pobj3=pobj2;
                                                    k=i;
                                                }
                                            }
                                        }
                                    }
                                }
                                //交代を試みる
                                if(pobj3)
                                {
                                    //本来はシステムメッセージを発行すべきかもしれないが･･･
                                    if(pobj3->Message(GOBJMSG_KOUTAI,charobjid[j][active_character[j]]))
                                    {
                                        hprecratio[j][active_character[j]]*=1.8;		//HP回復インターバル増
                                        active_character[j]=k;
                                        pobj->Message(GOBJMSG_TAIKI,0);
                                    }
                                }
                            }
                        }
                    }//←交代判定、ここまで

                    //支援攻撃要請判定
                    for(j=0;j<2;j++)
                    {
                        pobj = GetGObject(  charobjid[j][active_character[j]]  );

                        if( (pobj->data.objtype&GOBJFLG_COMPUTER) &&
                            !(pobj->data.aid&ACTID_NAGE) &&
                            !(pobj->data.aid&ACTID_SYSTEM) &&
                            !(pobj->data.aid&ACTID_KURAI) &&
                            !(pobj->data.aid&ACTID_GUARD) &&
                            !(pobj->data.aid&ACTID_INOUT) && 
                            (g_battleinfo.GetNumTeam(j) >= 2)
                            )
                        {
                            strikercall_counter[j]++;

                            if( (strikercall_counter[j]>1200) && ((rand()%20)==0) )
                            {
                                if(MessageFromObject(
                                            pobj->data.id, 
                                            (strikercall_counter[j]%2==0) ? MSGOBJ2SYS_STRIKER1 : MSGOBJ2SYS_STRIKER2 ,
                                            0
                                            ))
                                {
                                    strikercall_counter[j] = 0;
                                    AddEffect(EFCTID_COMEON,(int)pobj->data.x,(int)pobj->data.y-100,pobj->data.muki);
                                }
                                else
                                {
                                    strikercall_counter[j] /= 2;
                                }
                            }
                        }
                    }//←支援攻撃要請判定ここまで
                }
            }
            break;
    }//switchブロック終了

    if(the_end)
    {
        //エフェクト発動
        AddEffect(EFCTID_KO,0,-1000);
        efct_slowdown=60;
        if(dsb_ko!=NULL)dsb_ko->Play(0,0,0);
    
        //ラウンド勝者設定
        if(!m_finaldown_end[0] && !m_finaldown_end[1]){//ダブルKO
            m_round_winner=2;
        }
        else{
            m_round_winner = m_finaldown_end[0] ? 0 : 1;
        }

        //状態変更
        bf_state = BFSTATE_FINISHED;
        gbl.ods("BFSTATE_FIGHTING → BFSTATE_FINISHED");
    }
    
    g_system.PopSysTag();
}

//KO、キャラクタの最終ダウン終了待ち
void CBattleTask::T_UpdateStatus_Finished()
{
    g_system.PushSysTag(__FUNCTION__);

    int i,j,j2;
    CGObject *pobj;
    BOOL next=FALSE;

    if(m_finaldown_end[0] && m_finaldown_end[1])next=TRUE;//ダウン完了
    if(bf_counter<100)next=FALSE;	//早すぎ
    if(bf_counter>600)next=TRUE;	//救済措置

    if(next)
    {
        if(m_round_winner==2)//ダブル慶応
        {
            bf_state=BFSTATE_DOUBLEKO;
            gbl.ods("BFSTATE_FINISHED → BFSTATE_DOUBLEKO");
        }
        else {
            if(m_round_winner==0){j=0;j2=1;}//jは勝った方
            else {j=1;j2=0;}
            wincount[j]++;
            switch(g_battleinfo.GetBattleType()){
                case TAISENKEISIKI_GOCYAMAZE:
                    {
                        for(i=0;i<(int)g_battleinfo.GetNumTeam(j);i++){
                            if(!m_dead_one[j][i]){
                                winner_oid = charobjid[j][i];//このキャラクターの勝利ポーズ終了を待つ
                                GetGObject(winner_oid)->Message(GOBJMSG_DOYOUWIN);
                            }
                        }
                    }
                    break;
                case TAISENKEISIKI_JYUNBAN:
                    {
                        //生きてるほうはHP回復
                        pobj=GetGObject(charobjid[j][active_character[j]]);
                        if(pobj!=NULL){
                            pobj->data.hp+=150;
                            if(pobj->data.hp > (int)pobj->data.hpmax){
                                pobj->data.hp=pobj->data.hpmax;
                            }
                        }
                        else {
                            g_system.Log("CBattleTask::T_UpdateStatus_Finished KOF形式 勝者なし?",SYSLOG_ERROR);
                            g_system.ReturnTitle();
                        }
                    }//↓と同じ処理を続行
                case TAISENKEISIKI_KOUTAI:
                    {
                        for(i=0;i<(int)g_battleinfo.GetNumTeam(j);i++)
                        {
                            if(!m_dead_one[j][i]){
                                if(i==(int)active_character[j]){
                                    winner_oid = charobjid[j][active_character[j]];//このキャラクターの勝利ポーズ終了を待つ
                                    GetGObject(winner_oid)->Message(GOBJMSG_DOYOUWIN);
                                }
                                else if(m_all_dead[j2]){
                                    GetGObject(charobjid[j][i])->Message(GOBJMSG_DOYOUWIN2,charobjid[j][active_character[j]]);//外から飛んできて勝利ポーズ
                                }
                            }
                        }
                    }
                    break;
            }
            bf_state=BFSTATE_WAITFORENDWIN;
            m_winpose_end = FALSE;
            gbl.ods("BFSTATE_FINISHED → WAITFORENDWIN");
        }
    }

    g_system.PopSysTag();
}

//勝利ポーズ終了待ち
void CBattleTask::T_UpdateStatus_WaitForEndWin()
{
    g_system.PushSysTag(__FUNCTION__);

    BYTE  lteam;
    BOOL next=FALSE;

    if(m_winpose_end && bf_counter>150)next=TRUE;
    if(g_battleinfo.GetBattleType()==TAISENKEISIKI_GOCYAMAZE && (g_battleinfo.GetAllKey()&0xFFFF0000) ){//ボタン入力でスキップ
        next=TRUE;
    }
    if(bf_counter>600)next=TRUE;//救済措置

    //次へ
    if(next)//終了判定
    {
        //同時対戦の場合
        if(g_battleinfo.GetBattleType()==TAISENKEISIKI_GOCYAMAZE)
        {
            gbl.ods("WAITFORENDWIN → StartRound");
            StartRound();
            g_system.PopSysTag();
            return;
        }

        lteam = ( m_round_winner==0 ? 1 : 0 );//負けチーム

        //試合終了？
        // TAISENKEISIKI_KOUTAI → 必ずTRUE
        // TAISENKEISIKI_JYUNBAN→ FALSEの場合もあり
        if(m_all_dead[lteam])
        {
            wincount[m_round_winner]++;
            StartRound();
            g_system.PopSysTag();
            return;
        }
        
        if(g_battleinfo.GetBattleType()==TAISENKEISIKI_KOUTAI){
            g_system.Log("★CBattleTask::T_UpdateStatus_WaitForEndWin - おかしい",SYSLOG_ERROR);
            g_system.ReturnTitle();
        }

        //次のキャラクターを出す(TAISENKEISIKI_JYUNBANのみ)
        active_character[lteam]++;
        CGObject *pobj = GetGObject( charobjid[lteam][active_character[lteam]] );
        if(pobj){
            pobj->Message(GOBJMSG_KOUTAI2,0);
        }
        else{
            g_system.LogErr("CBattleTask::T_UpdateStatus_WaitForEndWin object not found, lteam=%d , active=%d",
                lteam,active_character[lteam]);
        }
        m_tojyo_end[lteam][active_character[lteam]]=FALSE;
        round++;
        {//ラウンド開始wav更新
            char *filename = new char[MAX_PATH];
            sprintf(filename,".\\system\\sound\\round%d.wav",round);
            RELEASE(dsb_round);
            dsb_round = g_sound.CreateDSB(filename);
            delete []filename;
        }
        bf_state=BFSTATE_WAITFORENDPOSE;
    }
    g_system.PopSysTag();
}

//ダブルKO表示終了待ち
void CBattleTask::T_UpdateStatus_DoubleKO()
{
/*	if(++bf_counter>200 || (g_input.GetKey(0,0)&0xFFFF0000 || g_input.GetKey(1,0)&0xFFFF0000))
    {
        gbl.ods("DOUBLEKO → StartRound\n");
        if(!m_all_dead[0]){
            wincount[0]++;
        }
        if(!m_all_dead[1]){
            wincount[1]++;
        }
        StartRound();
    }*/
    gbl.ods("★ダブルKO処理未実装");
    g_system.Log("ダブルKO処理未実装",SYSLOG_WARNING);
    g_system.ReturnTitle();
}

//タイムオーバー表示終了待ち
void CBattleTask::T_UpdateStatus_TimeOver()
{
    g_system.PushSysTag(__FUNCTION__);

    int i,j;

    double hpwariai[2][MAXNUM_TEAM];

    bf_counter++;
    CGObject *pobj;
    float tmp_num;

    //勝ったのはどっちか？（体力の減り具合で決定）
    DWORD winner;
    DWORD loser;
    for(j=0;j<2;j++)
    {
        tmp_num = 0.001f;
        for(i=0;i<MAXNUM_TEAM;i++)
        {
            if(charobjid[j][i]!=0)
            {
                pobj=GetGObject(charobjid[j][i]);
                if(pobj!=NULL){
                    hpwariai[j][i] = (double)pobj->data.hp / (double)pobj->data.hpmax;
                    tmp_num += 1.0f;
                }
                else hpwariai[j][i] = 0;
            }
            else hpwariai[j][i] = 0;
        }
        hpwariai[j][0]=(hpwariai[j][0]+hpwariai[j][1]+hpwariai[j][2])/tmp_num;//平均する
    }
    if(hpwariai[0][0] < hpwariai[1][0])j=1;//1p側の負け
    else j=0;//2p側の負け
    winner = j;
    loser  = j==0 ? 1 : 0;


    //次に進む？
    BOOL do_timeover = FALSE;
    if(bf_counter>1000)
    {
        //強制的に進む
        do_timeover = TRUE;
    }
    else if(bf_counter>310)
    {
        //空中に浮いている奴が居たらやらない

        do_timeover = TRUE;
        j = loser;
        for(i=0;i<(int)g_battleinfo.GetNumTeam(j);i++){
            if(GetGObject(charobjid[j][i])->data.hp>0 && GetGObject(charobjid[j][i])->data.y<-5){
                do_timeover = FALSE;
            }
        }

        j = winner;
        for(i=0;i<(int)g_battleinfo.GetNumTeam(j);i++){
            if(GetGObject(charobjid[j][i])->data.hp>0 && GetGObject(charobjid[j][i])->data.y<-5){
                do_timeover = FALSE;
            }
        }
    }


    //勝利ポーズ待ちに進む
    if(do_timeover)
    {
        if(g_battleinfo.GetBattleType()==TAISENKEISIKI_GOCYAMAZE)
        {
            wincount[winner]++;

            j= winner;
            for(i=0;i<(int)g_battleinfo.GetNumTeam(j);i++)
            {
                if(GetGObject(charobjid[j][i])->data.hp>0){
                    winner_oid = charobjid[j][i];//このキャラクターの勝利ポーズ終了を待つ
                    GetGObject(winner_oid)->Message(GOBJMSG_DOYOUWIN);
                }
            }

            j= loser;
            for(i=0;i<(int)g_battleinfo.GetNumTeam(j);i++){
                if(GetGObject(charobjid[j][i])->data.hp>0){
                    GetGObject(charobjid[j][i])->Message(GOBJMSG_DOTIMEOVERLOSE);
                }
            }
        }
        else
        {
            g_system.Log("battle 2417",SYSLOG_ERROR);
            g_system.ReturnTitle();
            g_system.PopSysTag();
            return;
        }
        bf_state=BFSTATE_WAITFORENDWIN;
        m_winpose_end = FALSE;
        gbl.ods("BFSTATE_TIMEOVER → WAITFORENDWIN");
        bf_counter=0;
    }

    g_system.PopSysTag();
}

//死亡フラグ更新
void CBattleTask::Update_DeadFlag()
{
    int i,j;
    CGObject *pobj;

    //全フラグ初期化
    for(j=0;j<2;j++){
        for(i=0;i<MAXNUM_TEAM;i++)
        {
            m_all_dead[j]=TRUE;
            m_dead_one[j][i]=TRUE;
        }
    }

    //チェック
    for(j=0;j<2;j++){
        for(i=0;i<(int)g_battleinfo.GetNumTeam(j);i++){
            pobj = GetGObject(charobjid[j][i]);
            if(pobj!=NULL && charobjid[j][i]!=0)
            {
                if( pobj->data.hp > 0 || (pobj->data.aid&ACTID_NAGE))
                {
                    m_dead_one[j][i]=FALSE;
                    m_all_dead[j]=FALSE;
                    //交代制の場合はhp回復
                    if(g_battleinfo.GetBattleType()==TAISENKEISIKI_KOUTAI)
                    {
                        if(i!=(int)active_character[j]){//アクティブでなければ
                            if ((bf_counter%hprecratio[j][i]) == 0){
                                pobj->data.hp++;
                                if (pobj->data.hp > (int)pobj->data.hpmax){
                                    pobj->data.hp = pobj->data.hpmax;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    for(j=0;j<2;j++){
        pobj = GetGObject(charobjid[j][active_character[j]]);
        if(pobj!=NULL){
            if( pobj->data.hp<=0 && !(pobj->data.aid&ACTID_NAGE) ){
                m_active_dead[j]=TRUE;
            }
            else m_active_dead[j]=FALSE;
        }
        else m_active_dead[j]=TRUE;
    }
}

/*-------------------------------------------------------------------------
    キー入力向き設定更新
---------------------------------------------------------------------------*/
void CBattleTask::UpdateKeyInputDirections()
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
void CBattleTask::WndMessage(HWND hWnd,UINT msg,WPARAM wparam, LPARAM lparam)
{
    if(msg==WM_KEYDOWN){
        if(wparam==VK_F7){
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
    }
}

