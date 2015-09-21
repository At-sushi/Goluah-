/*
2011/10/29	timeover�̃{�C�X�ɑΉ�
            �����̓Ɨ�
            ���E�X�g���C�J�[�ŌĂԕ��������E�t��
*/
/*============================================================================

    �퓬�^�X�N�N���X

    �i�l�b�g���[�N��Ή��Łj

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
    �\�z
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
    �j��
------------------------------------------------------------------------------*/
CBattleTask::~CBattleTask()
{
    Terminate();

    if (debugmsgbuff)
        free(debugmsgbuff);
    RELEASE(tex_fb);
}

/*===========================================================================

    �^�X�N�@������

=============================================================================*/
void CBattleTask::Initialize()
{
    CTNowLoading* now_loading = new CTNowLoading;
    g_system.AddTask(now_loading);//Now Loading�\���^�X�N�J�n

    char filename[256];
    int i,j;

    if (!g_config.IsFullScreen())
        AfxGetApp()->DoWaitCursor(1);
    // ���I�z�񏉊���
    p_objects.clear();
	object_regno.clear();
    suicide_list.resize(0);

    CBattleTaskBase::Initialize();

    //�R���s���[�^����t���O�t��
    double hp_ratio=1.0;
    for(i=0;i<3;i++){
        for(j=0;j<2;j++){
            if((charobjid[j][i]&0x0000FFFF)<(int)p_objects.size() && charobjid[j][i]!=0){
                if(GetGObject( charobjid[j][i] )!=NULL)
                {
                    if(g_battleinfo.GetCharacterIsCOM(j,i))
                    {
                        GetGObject( charobjid[j][i] )->data.objtype |= GOBJFLG_COMPUTER;
                        //�̗͂̍ő�l = DLL���ݒ肵���l x ��Փx�ݒ�ɂ��␳�l
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

    // ���v���C�t�@�C���쐬
    /*time_t crnt_time;
    time(&crnt_time);
    struct tm* crnt_time_l = localtime(&crnt_time);
    sprintf(filename,"%s\\%d%s%d%s%d%s%d%s%d%s%d.%s",
        "system\\replay\\",
        crnt_time_l->tm_year + 1900,				//�N
        (crnt_time_l->tm_mon + 1)<10 ? "0" : "",
        crnt_time_l->tm_mon + 1,					//��
        crnt_time_l->tm_mday<10 ? "0" : "",
        crnt_time_l->tm_mday,						//��
        crnt_time_l->tm_hour<10 ? "0" : "",
        crnt_time_l->tm_hour,						//��
        crnt_time_l->tm_min<10 ? "0" : "",
        crnt_time_l->tm_min,						//��
        crnt_time_l->tm_sec<10 ? "0" : "",
        crnt_time_l->tm_sec,						//�b
        "grd"										//�g���q
    );
    RepFile.Open(filename,CFile::modeWrite | CFile::modeCreate);*/

    StartRound();

    //bgm
    BOOL story_bgm_on = FALSE;
    if(g_battleinfo.GetStoryBGM()){//�X�g�[���[�Ŏw�肳�ꂽ�ꍇ
        if(!g_sound.BGMPlay(g_battleinfo.GetStoryBGM()))
        {
            // .stage/bgm/����T���Ă݂�
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
        //�X�e�[�W�f�B���N�g����bgm�̍Đ������݂�
        sprintf(filename,"%s\\bgm",
            g_stagelist.GetStageDir(g_battleinfo.GetStage()));
        if(!g_sound.BGMPlay(filename))
        {
            //�X�e�[�W���Ɠ����bgm�Đ������݂�
            sprintf(filename,"stage\\bgm\\%s",g_stagelist.GetStageDir(g_battleinfo.GetStage()));
            if(!g_sound.BGMPlay(filename)){
                gbl.PlayRandomBGM("stage\\bgm");
            }
        }
    }

    AfxGetApp()->DoWaitCursor(-1);
    g_system.RemoveTask('LOAD');//NowLoading �\���^�X�N����
}


/*----------------------------------------------------------------
    �I�u�W�F�N�g���X�g������
------------------------------------------------------------------*/
void CBattleTask::InitializeObjectList()
{
    g_system.PushSysTag(__FUNCTION__);

    for(int i=0;i<(int)object_regno.size();i++){
        object_regno[i]=0;
    }
    object_regindex=0;

    //�I�u�W�F�N�g0�쐬
    DWORD obj0=
        CreateGObject();
    GetGObjectInfo(0)->tid = TEAM_SYSTEM;
    GetGObjectInfo(0)->y = -9999;

    g_system.PopSysTag();
}



/*----------------------------------------------------------------
    �p�����[�^���Z�b�g
------------------------------------------------------------------*/
void CBattleTask::InitializeParameters()
{
    round=0;//�Ȃ��Ȃ�StartRound��++����邩��B
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
    ���̑��N���X��������
    �Q�[�W�E�G�t�F�N�g
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
    ���E���h����i�߂�
    �p�����[�^�������Z�b�g����
------------------------------------------------------------------*/
void CBattleTask::StartRound()
{
    int i,j;
    round++;
    char filename[256];
    BOOL call_round=TRUE;

    //�����I������
    switch( g_battleinfo.GetBattleType() ){
    case TAISENKEISIKI_GOCYAMAZE:	
        disp_center_x=0;
        limittime=g_battleinfo.GetLimitTime();
        //���������|�C���g���ȏ�Ȃ�Ώ�����ʂ�
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
    case TAISENKEISIKI_JYUNBAN://���E���h�I�� = �����I��
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



    //�u���E���hX�v�T�E���h���[�h
    if(call_round){
        sprintf(filename,".\\system\\sound\\round%d.wav",round);
        dsb_round = g_sound.CreateDSB(filename);
    }

    if(round!=1){//�u���E���h���ς��������ł���v�I�u�W�F�N�g��S������
        for(i=1;i<(int)p_objects.size();i++){
            if(p_objects[i]!=NULL){
                if(!p_objects[i]->Message(GOBJMSG_CNGROUND)){
                    DELETECLASS(p_objects[i]);
                }
            }
        }
    }

    //�L�����N�^�[�̏�Ԃ�ݒ肷��
    DWORD k;
    GOBJECT	  *pdat;
    for(j=0;j<2;j++){
        for(i=0;i<(int)g_battleinfo.GetNumTeam(j);i++){	
            k=charobjid[j][i];
            if(k!=0){
                pdat = &(GetGObject(k)->data);
                //�u�G�L�����N�^�v��id��ݒ�i�K���j
                if(pdat->tid==TEAM_PLAYER1)
                    pdat->eid = charobjid[TEAM_PLAYER2][0];
                else
                    pdat->eid = charobjid[TEAM_PLAYER1][0];
                //�ʒu�ݒ�
                pdat->y = 0;
                pdat->x = (150 + 50*i) *(j==0 ? -1 : 1);
                pdat->muki = (j==0 ? FALSE : TRUE);
    
                if(pdat != NULL){
                    pdat->hp = pdat->hpmax;		//HP��
                    pdat->aid = ACTID_NEUTRAL;	//�s��ID
                    GetGObject(pdat->id)->ActionIDChanged(TRUE,TRUE);
                }
                if(g_battleinfo.GetCharacterIsCOM(j,i)){//�R���s���[�^����t���O�t��
                        GetGObject(k)->data.objtype |= GOBJFLG_COMPUTER;
                }
                GetGObject(k)->Message(GOBJMSG_CNGROUND);//���E���h�J�n�̃��b�Z�[�W���M
            }
        }
    }
    
    bf_state = BFSTATE_WAITFORENDPOSE;
    bf_counter=0;
    
    //�G�t�F�N�g���ׂĖ���
    efct_slowdown=0;
    efct_stop=0;
    efct_darkbg=0;
    efct_nobg=0;
    efct_sindo=0;
    efct_sindom=0;
    efct_hitstop=0;
    efct_flash=0;

    CGObject *pobj;

    //�x���U���v���J�E���^���Z�b�g
    strikercall_counter[0] = strikercall_counter[1] = 0;

    //�o��|�[�Y�ڍs�ݒ�
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

    // �t�F�[�h�A�E�g�J�n
    RELEASE(tex_fb);
    tex_fb = g_draw.GetFrontBufferCopy();
    efct_fadein = 20;
}



/*===========================================================================

    �^�X�N�@�j��

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

    �^�X�N�@���s

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

        //�G�t�F�N�g�E�X���[
        if(efct_slowdown>0){
            efct_slowdown--;
            if(efct_slowdown%2==0){
                g_system.PopSysTag();
                return battle_end ? FALSE : TRUE;
            }
        }
        efct_sindo--;

        //�X�g�b�v����
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
        T_KasanariHantei();//�d�Ȃ蔻��
        T_Sousai();//��ѓ���̑��E
        T_AtariHantei();//�����蔻��
        T_ChangeTarget();//�^�[�Q�b�g�ύX����
        T_UpdateStatus();//������ԍX�V

        if(battle_end){
            g_system.PopSysTag();
            return FALSE;
        }
    }

    g_input.KeyLock(FALSE);

    //�I�u�W�F�N�g���X�g�����B���ł��������Ă���I�u�W�F�N�g�����ł�����
    for(auto i : suicide_list){
        if(i!=0){
            DeleteGObject(i);
        }
    }
    suicide_list.resize(0);

    //�L�[���͌����X�V
    UpdateKeyInputDirections();

    //�w�i�ÃG�t�F�N�g
    efct_darkbg--;
    //�w�i�Ȃ��G�t�F�N�g
    efct_nobg--;
    //�t�F�[�h�C��
    efct_fadein--;
    //�t���b�V��
    efct_flash--;

    //�B��L�����o��
    if(rand()%(50*60*3000) - bf_counter % 30==0)
        if(rand()%(50*60*3000)==0)
            AddEffect(EFCTID_HATTEN,640, 5);//Hatten

    g_system.PopSysTag();
    
    return battle_end ? FALSE : TRUE;
}



/*----------------------------------------------------------------
    COMMAND���b�Z�[�W���I�u�W�F�N�g�ɑ��M
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
    ACTION���b�Z�[�W���I�u�W�F�N�g�ɑ��M
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

            // �b��
            ////RepFile.Write(&p_objects[i]->data, sizeof(GOBJECT));
            //RepFile.Write(&p_objects[i]->data.aid, sizeof(DWORD));
        }
    }

    g_system.PopSysTag();
}


/*----------------------------------------------------------------
    �d�Ȃ蔻�菈��
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

    //�d�Ȃ蔻��
    for(i=0;i<(int)p_objects.size();i++){
        if(p_objects[i]!=NULL){//�I�u�W�F�N�g�����݂���
            pdat1 = &(p_objects[i]->data);
            if(pdat1->objtype & GOBJFLG_KASANARI){//�I�u�W�F�N�g�͏d�Ȃ蔻����s��
                if(pdat1->kasanari){//�d�Ȃ蔻��ON
                    if(pdat1->phdat!=NULL){
                        if(pdat1->pcdat!=NULL){
                            if( ((GCD_CELL2_070*)pdat1->pcdat)[0].cell[0].flag==700 ){
                                magmode1 = 1;//�d�S���S
                            }
                            else{
                                if(pdat1->pcdat[pdat1->cnow].flag & GCDCELL2_ROT_BASEPOINT){
                                    magmode1 = 0;//�������S
                                }
                                else magmode1=1;
                            }
                            // �ύX�F��������݂̂ɑ΂��Ĕ���i������ƌy���Ȃ邩�ȁj
                            for(j=i+1;j<(int)p_objects.size();j++){//** ���̑S�ẴI�u�W�F�N�g�ɑ΂��� **
                                if(/*i!=j*/TRUE){//�����ȊO��
                                    if(p_objects[j]!=NULL){
                                        pdat2 = &(p_objects[j]->data);
                                        if(pdat1->tid != pdat2->tid){
                                            if(pdat2->objtype & GOBJFLG_KASANARI){//�I�u�W�F�N�g�͍U������炤
                                                if(pdat2->kasanari){//�d�Ȃ蔻��ON
                                                    if(pdat2->phdat!=NULL){
                                                        if(pdat2->pcdat!=NULL)
                                                        {
                                                            if( ((GCD_CELL2_070*)pdat2->pcdat)[0].cell[0].flag==700 ){
                                                                magmode2 = 1;//�d�S���S
                                                            }
                                                            else{
                                                                if(pdat2->pcdat[pdat2->cnow].flag & GCDCELL2_ROT_BASEPOINT){
                                                                    magmode2 = 0;//�������S
                                                                }
                                                                else magmode2=1;
                                                            }
                                                            //�����蔻����s��
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
                                                            if(kas_yes){//��ȏ�̋�`���Փ˂���
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

    //��ʒ��S���o��
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
        if(disp_center_x < new_disp_center_x)disp_center_x = new_disp_center_x;//��������
    }
    if(disp_center_x < new_disp_center_x){
        if(disp_center_x < new_disp_center_x-50)disp_center_x+=16;
        else if(disp_center_x < new_disp_center_x-20)disp_center_x+=8;
        else disp_center_x+=4;
        if(disp_center_x > new_disp_center_x)disp_center_x = new_disp_center_x;//��������
    }
    if(disp_center_x>320)disp_center_x=320;
    else if(disp_center_x<-320)disp_center_x=-320;

    //��ʊO�ɍs��������Ă邨���b�Z�[�W
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

    //x�����X�e�[�W�[�N���b�v����
    for(i=0;i<(int)p_objects.size();i++){
        if(p_objects[i]!=NULL){//�I�u�W�F�N�g�����݂���
            pdat1 = &(p_objects[i]->data);
            if(pdat1->objtype & GOBJFLG_CLIPX){//X��������
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
    �U�����菈��
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
            if(p_objects[i]!=NULL){//�I�u�W�F�N�g�����݂���
                pdat1 = &(p_objects[i]->data);
                if((pdat1->tid==TEAM_PLAYER1 || pdat1->tid==TEAM_PLAYER2) && BATTLETASK_ISNOTFXOBJ(pdat1)){
                    if(pdat1->objtype & GOBJFLG_ATTACK){//�I�u�W�F�N�g�͍U�����s��
                        if(pdat1->kougeki){//�U����ON
                            if(pdat1->phdat!=NULL){
                                if(pdat1->pcdat!=NULL)
                                {
                                    if( ((GCD_CELL2_070*)pdat1->pcdat)[0].cell[0].flag==700 ){
                                        magmode1 = 1;//�d�S���S
                                    }
                                    else{
                                        if(pdat1->pcdat[pdat1->cnow].flag & GCDCELL2_ROT_BASEPOINT){
                                            magmode1 = 0;//�������S
                                        }
                                        else magmode1=1;
                                    }
                                    for(j=0;j<(int)p_objects.size();j++){//** ���̑S�ẴI�u�W�F�N�g�ɑ΂��� **
                                        if(i!=j){//�����ȊO��
                                            if(p_objects[j]!=NULL){
                                                pdat2 = &(p_objects[j]->data);
                                                if((pdat2->tid==TEAM_PLAYER1 || pdat2->tid==TEAM_PLAYER2)  && BATTLETASK_ISNOTFXOBJ(pdat2)){
                                                    if(pdat1->tid != pdat2->tid){
                                                        if(TRUE/*pdat2->counter!=0*/){
                                                            if(pdat2->objtype & GOBJFLG_KURAI){//�I�u�W�F�N�g�͍U������炤
                                                                if(!pdat2->muteki){//���G���OFF
                                                                    if(pdat2->phdat!=NULL){
                                                                        if(pdat2->pcdat!=NULL)
                                                                        {
                                                                            if( ((GCD_CELL2_070*)pdat2->pcdat)[0].cell[0].flag==700 ){
                                                                                magmode2 = 1;//�d�S���S
                                                                                }
                                                                                else{
                                                                                    if(pdat2->pcdat[pdat2->cnow].flag & GCDCELL2_ROT_BASEPOINT){
                                                                                        magmode2 = 0;//�������S
                                                                                    }
                                                                                    else magmode2=1;
                                                                            }
                                                                            //�����蔻����s��
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
                                                                            if(num_kas>0){//��ȏ�̋�`���Փ˂���
                                                                                kas_point2.x=0;
                                                                                kas_point2.y=0;
                                                                                for(k=0;k<num_kas;k++){
                                                                                    kas_point2.x += kas_point[k].x;
                                                                                    kas_point2.y += kas_point[k].y;
                                                                                }
                                                                                kas_point2.x /= num_kas;
                                                                                kas_point2.y /= num_kas;
                                                                                if (Atari(p_objects[i]->data.id, p_objects[j]->data.id, kas_point2))
                                                                                    kurai_list.push_back(pdat2->id);// ��ł��
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
    else{//���Α����瓖���蔻������
        for(i=(int)p_objects.size()-1;i>=0;i--){
            if(p_objects[i]!=NULL){//�I�u�W�F�N�g�����݂���
                pdat1 = &(p_objects[i]->data);
                if((pdat1->tid==TEAM_PLAYER1 || pdat1->tid==TEAM_PLAYER2) && BATTLETASK_ISNOTFXOBJ(pdat1)){
                    if(pdat1->objtype & GOBJFLG_ATTACK){//�I�u�W�F�N�g�͍U�����s��
                        if(pdat1->kougeki){//�U����ON
                            if(pdat1->phdat!=NULL){
                                if(pdat1->pcdat!=NULL)
                                {
                                    if( ((GCD_CELL2_070*)pdat1->pcdat)[0].cell[0].flag==700 ){
                                        magmode1 = 1;//�d�S���S
                                    }
                                    else{
                                        if(pdat1->pcdat[pdat1->cnow].flag & GCDCELL2_ROT_BASEPOINT){
                                            magmode1 = 0;//�������S
                                        }
                                        else magmode1=1;
                                    }
                                    for(j=0;j<(int)p_objects.size();j++){//** ���̑S�ẴI�u�W�F�N�g�ɑ΂��� **
                                        if(i!=j){//�����ȊO��
                                            if(p_objects[j]!=NULL){
                                                pdat2 = &(p_objects[j]->data);
                                                if((pdat2->tid==TEAM_PLAYER1 || pdat2->tid==TEAM_PLAYER2)  && BATTLETASK_ISNOTFXOBJ(pdat2) ){
                                                    if(pdat1->tid != pdat2->tid){
                                                        if(TRUE/*pdat2->counter!=0*/){
                                                            if(pdat2->objtype & GOBJFLG_KURAI){//�I�u�W�F�N�g�͍U������炤
                                                                if(!pdat2->muteki){//���G���OFF
                                                                    if(pdat2->phdat!=NULL){
                                                                        if(pdat2->pcdat!=NULL)
                                                                        {
                                                                            if( ((GCD_CELL2_070*)pdat2->pcdat)[0].cell[0].flag==700 ){
                                                                                magmode2 = 1;//�d�S���S
                                                                                }
                                                                                else{
                                                                                    if(pdat2->pcdat[pdat2->cnow].flag & GCDCELL2_ROT_BASEPOINT){
                                                                                        magmode2 = 0;//�������S
                                                                                    }
                                                                                    else magmode2=1;
                                                                            }
                                                                            //�����蔻����s��
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
                                                                            if(num_kas>0){//��ȏ�̋�`���Փ˂���
                                                                                kas_point2.x=0;
                                                                                kas_point2.y=0;
                                                                                for(k=0;k<num_kas;k++){
                                                                                    kas_point2.x += kas_point[k].x;
                                                                                    kas_point2.y += kas_point[k].y;
                                                                                }
                                                                                kas_point2.x /= num_kas;
                                                                                kas_point2.y /= num_kas;
                                                                                if (Atari(p_objects[i]->data.id, p_objects[j]->data.id, kas_point2))
                                                                                    kurai_list.push_back(pdat2->id);// ��ł��
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

    //�������Ƃ��A�s�����X�g�b�v���Ċi�D�����̂�1�񂾂�action()������
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
    �򓹋�̔��菈��
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
        if(p_objects[i]!=NULL){//�I�u�W�F�N�g�����݂���
            pdat1 = &(p_objects[i]->data);
            if((pdat1->tid==TEAM_PLAYER1 || pdat1->tid==TEAM_PLAYER2)  && BATTLETASK_ISNOTFXOBJ(pdat1)){
                if(pdat1->objtype & GOBJFLG_ZBULLET){//�I�u�W�F�N�g�͔�ѓ����������
                    if(pdat1->kougeki){//�U���͂������Ă��Ȃ�
                        if(pdat1->phdat!=NULL){
                            if(pdat1->pcdat!=NULL)
                            {
                                if( ((GCD_CELL2_070*)pdat1->pcdat)[0].cell[0].flag==700 ){
                                    magmode1 = 1;//�d�S���S
                                }
                                else{
                                    if(pdat1->pcdat[pdat1->cnow].flag & GCDCELL2_ROT_BASEPOINT){
                                        magmode1 = 0;//�������S
                                    }
                                    else magmode1=1;
                                }
                                for(j=i+1;j<(int)p_objects.size();j++){//** ���̑S�ẴI�u�W�F�N�g�ɑ΂��� **
                                    if(p_objects[j]!=NULL){
                                        pdat2 = &(p_objects[j]->data);
                                        if((pdat2->tid==TEAM_PLAYER1 || pdat2->tid==TEAM_PLAYER2)  && BATTLETASK_ISNOTFXOBJ(pdat2)){
                                            if(pdat1->tid != pdat2->tid){
                                                if(pdat2->objtype & GOBJFLG_ZBULLET){//�I�u�W�F�N�g�͔�ѓ����������
                                                    if(pdat2->kougeki ){//�U���͂������Ă��Ȃ�
                                                        if(pdat2->phdat!=NULL){
                                                            if(pdat2->pcdat!=NULL)
                                                            {
                                                                if( ((GCD_CELL2_070*)pdat2->pcdat)[0].cell[0].flag==700 ){
                                                                    magmode2 = 1;//�d�S���S
                                                                }
                                                                else{
                                                                    if(pdat2->pcdat[pdat2->cnow].flag & GCDCELL2_ROT_BASEPOINT){
                                                                        magmode2 = 0;//�������S
                                                                    }
                                                                    else magmode2=1;
                                                                }
                                                                //�����蔻����s��
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
                                                                if(num_kas>0){//��ȏ�̋�`���Փ˂���
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
    �^�[�Q�b�g�ύX����
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
            if((pdat->objtype & GOBJFLG_NEEDTARGET) && BATTLETASK_ISNOTFXOBJ(pdat)){//�^�[�Q�b�g���K�v
                minimum_distance=9999*9999;
                for(j=0;j<(int)p_objects.size();j++){
                    if(j!=i){
                        if(p_objects[j]!=NULL){
                            pedat=&(p_objects[j]->data);
                            if((pdat->tid!=pedat->tid) && BATTLETASK_ISNOTFXOBJ(pedat)){//�`�[�����Ⴄ
                                if(pedat->objtype & GOBJFLG_TOBETARGET){//�^�[�Q�b�g�ɂȂ��
                                    if(pedat->hp > 0){//�܂������Ă���
                                        //�����𑪂�
                                        if( (pedat->x-pdat->x)*(pedat->x-pdat->x) < minimum_distance){//�߂�
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

    �`�揈��

==================================================================*/
void CBattleTask::Draw()
{
    if(m_pause_task)return;

    g_system.PushSysTag(__FUNCTION__);

    if (efct_flash>0){	// ��ʃt���b�V��
        g_draw.d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, RGB(255,255,255), 0.0f, 0);
        g_system.PopSysTag();
        return;		// �`�悵�Ȃ�
    }
    SetTransform(TRUE);
    g_draw.EnableZ();

    //�`��p���X�g����
    DWORD i;
    std::vector<CGObject*> objlist;
    for(i=0;i<(int)p_objects.size();i++){
        if(p_objects[i]!=NULL){
            objlist.push_back(p_objects[i]);
        }
    }
    std::sort(objlist.begin(),objlist.end(),CGObject::ZCompare);//z�\�[�g

    //�w�i�X�e�[�W�`��
    if(g_system.sw_showbg && !(efct_nobg>0)){//�i���w�i�Ȃ��G�t�F�N�g�j
        for(i=0;i<objlist.size();i++){
            objlist[i]->Message(GOBJMSG_DRAWBACK);
        }
    }
    
    g_draw.d3ddev->Clear(0,NULL,D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,0,1.0f,0);// clear z buffer
    if (!g_draw.StencilEnable())
        g_draw.d3ddev->Clear(0,NULL,D3DCLEAR_ZBUFFER,0,1.0f,0);// clear z buffer

    //�w�i�Â�����G�t�F�N�g
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

    //�ʏ�ʒu�`��
    for(i=0;i<objlist.size();i++){
        objlist[i]->Message(GOBJMSG_DRAW);
    }

    // �e
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
        //�q�b�g�J�E���g�ƃ_���[�W
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

                    // �\���p�̐����A�j��
                    if (pobj->sexydamage > pobj->sexydamage_anim)
                    {
                        pobj->sexydamage_anim += pobj->sexydamage_haba;
                        if (pobj->sexydamage < pobj->sexydamage_anim)
                            pobj->sexydamage_anim = pobj->sexydamage;
                    }
                    else if  (pobj->sexydamage < pobj->sexydamage_anim)
                    {
                        // �܂��������낤���ǁc
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

                    // �\���p�̐����A�j��
                    if (pobj->sexydamage > pobj->sexydamage_anim)
                    {
                        pobj->sexydamage_anim += pobj->sexydamage_haba;
                        if (pobj->sexydamage < pobj->sexydamage_anim)
                            pobj->sexydamage_anim = pobj->sexydamage;
                    }
                    else if  (pobj->sexydamage < pobj->sexydamage_anim)
                    {
                        // �܂��������낤���ǁc
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

    if(g_system.sw_rects){//�����蔻��\��
        for(i=0;i<(int)p_objects.size();i++){
            if(p_objects[i]!=NULL){//�I�u�W�F�N�g�����݂���
                pdat = &(p_objects[i]->data);
                if(pdat->tid==TEAM_PLAYER1 || pdat->tid==TEAM_PLAYER2){
                    if(pdat->pcdat!=NULL && pdat->phdat!=NULL){
                        show_kas=FALSE;
                        show_atr=FALSE;
                        show_atk=FALSE;
                        if(pdat->objtype & GOBJFLG_ATTACK){//�I�u�W�F�N�g�͍U�����s��
                            if(TRUE){//pdat->kougeki){//�U����ON
                                show_atk=TRUE;
                            }
                        }
                        if(pdat->objtype & GOBJFLG_ZBULLET){//��ѓ����
                            show_atk=TRUE;
                        }
                        if(pdat->objtype & GOBJFLG_KURAI){//�I�u�W�F�N�g�͍U������炤
                            if(TRUE){//!pdat->muteki){//���G�ł͂Ȃ�
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


    //�O�i�X�e�[�W�`��
    if(!(!g_system.sw_showbg && g_config.IsDebugMode())){
        for(i=0;i<objlist.size();i++){
            objlist[i]->Message(GOBJMSG_DRAWFRONT);
        }
    }

    // �t�F�[�h�A�E�g����
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
// �I�u�W�F�N�g���X�g�Ǘ��n
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

    g_system.Log("�I�u�W�F�N�g���ő吔�𒴂��Ă��܂�\n",SYSLOG_WARNING);
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

    //�[���E�I�u�W�F�N�g
    if(oid==0){
        g_system.LogWarning("%s �[���E�I�u�W�F�N�g�폜����");
        g_system.PopSysTag();
        return;
    }

    CGObject *pgobj = GetGObject(oid);
    if(pgobj==NULL){
        g_system.PopSysTag();
        return;
    }

    //�L�����N�^�[�E�X�e�[�W�͏������܂���
    for(int j=0;j<2;j++){
        for(int i=0;i<3;i++)
        {
            if(charobjid[j][i]==oid){
                g_system.LogWarning("%s �L�����N�^�[�I�u�W�F�N�g�폜����(%d,%d)",__FUNCTION__,j,i);
                g_system.PopSysTag();
                return;
            }
        }
    }
    if(stgobjid==oid){
        g_system.LogWarning("%s �X�e�[�W�I�u�W�F�N�g�폜����",__FUNCTION__);
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

    //�ϐ�����
    CGObject *pdat = GetGObject(oid);
    if(pdat==NULL){
        g_system.LogWarning("%s msg=%08X ,�I�u�W�F�N�gNULL",__FUNCTION__,msg);
        g_system.PopSysTag();
        return(0);
    }
    
    BOOL striker_front=FALSE;
    DWORD team = pdat->data.tid;	//�����`�[��
    DWORD team2 = team==TEAM_PLAYER1 ? TEAM_PLAYER2 : TEAM_PLAYER1;	//�G�`�[��

    UINT cidx=0;
    for(int i=0;i<MAXNUM_TEAM;i++){
        if(oid == charobjid[team][i]){
            cidx = i;
        }
    }


    switch(msg){
    //���o��I��
    case MSGOBJ2SYS_TOJYOEND:
        //�����ΐ�łȂ���΁A��A�N�e�B�u�L�����N�^�[�ɑҋ@���b�Z�[�W���M
        if(g_battleinfo.GetBattleType() != TAISENKEISIKI_GOCYAMAZE){
            if(oid != charobjid[team][active_character[team]]){
                GetGObject(oid)->Message(GOBJMSG_TAIKI,0);
            }
        }//���̏����������čs��
    //������o��|�[�Y�I��
    case MSGOBJ2SYS_KOUTAIEND:
        m_tojyo_end[team][cidx]=TRUE;
        break;
    //���ŏI�_�E���I��
    case MSGOBJ2SYS_FINALDOWNEND:
        m_finaldown_end[team]=TRUE;
        break;
    //�������|�[�Y�I��
    case MSGOBJ2SYS_WINPOSEEND:
        if(oid == winner_oid){
            m_winpose_end=TRUE;
        }
        if(g_battleinfo.GetBattleType()==TAISENKEISIKI_JYUNBAN){//KOFLike�̏ꍇ�A�j���[�g�����ɖ߂��Ă��
            GetGObject(oid)->data.aid = ACTID_NEUTRAL;
        }
        break;
    //�����v��(�L�����N�^�[1)
    case MSGOBJ2SYS_KOUTAI1:
        striker_front = TRUE;//���Ƃ́��Ɠ��l�̏����i�ϐ����͋C�ɂ��Ȃ��Łj
    //�����v��(�L�����N�^�[2)
    case MSGOBJ2SYS_KOUTAI2:
        {
            if(g_battleinfo.GetBattleType()!=TAISENKEISIKI_KOUTAI){//�ΐ�`���`�F�b�N
                g_system.PopSysTag();
                return(FALSE);
            }
            if(bf_state != BFSTATE_FIGHTING){//������ԃ`�F�b�N
                g_system.PopSysTag();
                return(FALSE);
            }

            //�u���́v�L�����N�^�[�擾
            DWORD next_act;
            switch(cidx){	//HP��face1�̔z�u�ύX�ɔ����A���E���t��
                case 0:next_act= striker_front ? 2 : 1 ;break;
                case 1:next_act= striker_front ? 0 : 2 ;break;
                case 2:next_act= striker_front ? 1 : 0 ;break;
                default:
                    g_system.LogWarning("%s msg=%08X ,���s(cidx=%d)",__FUNCTION__,msg,cidx);
                    g_system.PopSysTag();
                    return(FALSE);
            }
            if(charobjid[team][next_act]==0){
                g_system.PopSysTag();
                return FALSE;
            }
            pdat=(CGObject*)GetGObject( charobjid[team][next_act] );
            if(!pdat){
                g_system.LogWarning("%s msg=%08X ,���s(�I�u�W�F�N�gNULL)",__FUNCTION__,msg,cidx);
                g_system.PopSysTag();
                return(FALSE);
            }

            //��チ�b�Z�[�W���M
            if(pdat->Message(GOBJMSG_KOUTAI,charobjid[team][cidx]))
            {
                hprecratio[team][cidx]*=1.8;		//HP�񕜃C���^�[�o����
                active_character[team]=next_act;//"�A�N�e�B�u" �L�����N�^�[�X�V
                g_system.PopSysTag();
                return(TRUE);					//����
            }
            else g_system.Log("�����v�����ہB��㎸�s�B\n",SYSLOG_DEBUG);
            g_system.PopSysTag();
            return(FALSE);
        }
        break;

    //���X�g���C�J�[�v��(�L�����N�^�[1)
    case MSGOBJ2SYS_STRIKER1:
        striker_front=TRUE;//���Ƃ́��Ɠ��l�̏���
    //���X�g���C�J�[�v��(�L�����N�^�[2)
    case MSGOBJ2SYS_STRIKER2:
        {
            //�`�F�b�N
            {
                BOOL striker_ck = TRUE;
                if(g_battleinfo.GetBattleType()==TAISENKEISIKI_GOCYAMAZE){	striker_ck=FALSE; }//�ΐ�`���`�F�b�N
                if(bf_state != BFSTATE_FIGHTING){ striker_ck=FALSE; }//������ԃ`�F�b�N
                if(strikercount[team]==0){ striker_ck=FALSE; }//�X�g���C�J�[�|�C���g�`�F�b�N
    
                if(!striker_ck){
                    g_system.PopSysTag();
                    return FALSE;
                }
            }
            
            //�u���́v�L�����N�^�[�擾
            DWORD next_act;
            switch(cidx){	//HP��face1�̔z�u�ύX�ɔ����A���E���t��
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

            if(pdat->Message(GOBJMSG_STRIKER,charobjid[team][cidx]))//�X�g���C�J�[�v���₢���킹
            {
                //OK
                strikercount[team]--;
                g_system.PopSysTag();
                return(TRUE);
            }
            else gbl.ods("���X�g���C�J�[�p��");
        }
        break;

    default:
        g_system.LogWarning("%s msg=%08X ,��������ĂȂ����b�Z�[�W",__FUNCTION__,msg);
    }

    g_system.PopSysTag();
    return(0);
}

BOOL CBattleTask::CatchObject(DWORD eoid,LPVOID cy)
{
    if(cy==NULL)return(FALSE);

    CGObject *peobj = (CGObject*)GetGObject(eoid);
    if(peobj==NULL)return(FALSE);

    if(!(peobj->data.objtype & GOBJFLG_NAGERARE))return(FALSE);//���肪�������t���O�������Ă��Ȃ������玸�s
    if(!peobj->data.nagerare)return(FALSE);
    if(peobj->nage_muteki_cnt>0)return FALSE;
    if(peobj->data.counter==0)return(FALSE);//���肪�s���J�ڒ��ゾ������~�߂Ă���
    if(peobj->data.aid & ACTID_GUARD)return(FALSE);//�K�[�h�����A�ꉞ�_�����Ă��Ƃɂ��Ă���
    if(peobj->data.aid & ACTID_NAGE)return(FALSE);//���������A�ꉞ�_�����Ă��Ƃɂ��Ă���
    if(peobj->data.aid & ACTID_INOUT)return(FALSE);//���or�X�g���C�J�[�U����

    if(!(peobj->data.aid&ACTID_KURAI)){
        peobj->hitcount=0;
    }

    //����
    peobj->data.aid = ACTID_NAGERARE;
    peobj->ActionIDChanged(TRUE,TRUE);
    peobj->data.muteki=TRUE;//��炢����OFF
    peobj->data.kasanari=FALSE;//�d�Ȃ蔻��OFF
    peobj->data.nagerare=FALSE;//������ꔻ��OFF
    peobj->cy = *(CATCHYOU*)cy;

    return(TRUE);
}

CGObject* CBattleTask::GetGObject(DWORD oid)
{
    DWORD id1 = oid & 0x0000FFFF;			//�z��C���f�b�N�X
    DWORD id2 = (oid >> 16) & 0x00007FFF;	//�����J�E���g

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
        g_system.LogWarning("%s �͈̓`�F�b�N�G���[(team=%d,index=%d)",__FUNCTION__,j,i);
        return 0;
    }

    return( GetGObject(charobjid[j][i]) );
}



void CBattleTask::AddEffect(DWORD efctid,int prm1,int prm2,int prm3)
{
    g_system.PushSysTag(__FUNCTION__);

    switch(efctid){
    //CBattleTask���S���G�t�F�N�g
    case EFCTID_STOP:
        efct_stop = prm1;
        break;
    case EFCTID_DARK:
        efct_darkbg = prm1;
        break;
    case EFCTID_SINDO:
        efct_sindo = prm2;
        efct_sindom= prm1;

/*		// �U���G�t�F�N�g�Ɏc��������
        // �R�}�������邱�Ƃ�����iPart37��>>705�j�̂Ŗ߂��Ă����܂�
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
    default://���Ƃ�CEffectList�ɔC����yo
        cp_efctlist->AddEffect(efctid,prm1,prm2,prm3);
    }

    g_system.PopSysTag();
}



//****************************************************************
//  Atari - ��`�Փˎ��̏���
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
    
    //�Ƃ肠���������������Ƃ�ʒm
    if(attacker->Message(GOBJMSG_TOUCHC,k_id)==TOUCHC_CANCEL)
    {
        g_system.PopSysTag();
        return FALSE;
    }

    //���̍U������ۑ�(atk2)
    ATTACKINFO2 tmpatkinfo = higaisya->data.atk2;
    {
        //�������_���[�W�����R�s�[
        higaisya->data.atk2.info1 = attacker->data.atk;
        higaisya->data.atk2.oid = a_id;
        //�t���O��
        higaisya->data.atk2.flags = 0;
        //��ʒ[�ő���������߂����ǂ���
        if(higaisya->data.objtype & GOBJFLG_CLIPX){
            if(attacker->data.objtype & GOBJFLG_HANSAYOU){
                higaisya->data.atk2.flags |= ATKINFO2_ATTACKERBACK;
            }
        }
        //�̂�����̌���
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

    //�R���s���[�^����Ȃ�΁A�L�[������Ă��
    BOOL cool_guard = FALSE;
    if(higaisya->data.objtype & GOBJFLG_COMPUTER)
    {
        g_input.EnableComKey();
        DWORD comgrdkey = 0;
        if(rand()%100 < higaisya->comguard){
            comgrdkey = KEYSTA_BACK;
        }
        
        cool_guard = (rand()%8 < higaisya->com_level) ? TRUE : FALSE;

        //�㉺�i����
        if(cool_guard)
        {
            if(attacker->data.atk->guard & GUARDINFO_XSTAND)
            {
                comgrdkey |= KEYSTA_DOWN;
            }
        }
        //�K��
        else
        {
            if(rand()%2 == 0){
                comgrdkey |= KEYSTA_DOWN;
            }
        }
        g_input.SetComKey(comgrdkey);
    }

    DWORD res = higaisya->Message(GOBJMSG_TOUCHA,a_id);//�������݂�
    g_input.DisableComKey();

    GOBJECT		*pdat  =&(higaisya->data);
    ATTACKINFO  *aif = attacker->data.atk;

    // ���ł��ꂻ���Ȃ���킹��
    if (res & 0x20000000 && bf_state==BFSTATE_FIGHTING && aif->kezuri >= pdat->hp)
        res |= 0x10000000;

    double dmkanwa;
    DWORD i;
    BOOL actidchanged = FALSE;

    if(res & 0x10000000){//������
        dmkanwa=1.0;//�_���[�W�ɘa��
        //hit count
        if(pdat->aid&ACTID_KURAI){
            higaisya->hitcount++;
            if(higaisya->data.id == charobjid[higaisya->data.tid][active_character[higaisya->data.tid]]){//�A���q�b�g�\��
                if(higaisya->hitcount==2)
                    bf_hitdisp[pdat->tid]=0;
                else bf_hitdisp[pdat->tid]=30;
            }
        }
        else{
            higaisya->hitcount=1;
            if(higaisya->data.id == charobjid[higaisya->data.tid][active_character[higaisya->data.tid]]){//�A���q�b�g�\��
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

            if(higaisya->hitcount == 2)		// �\���p���ݒ�
                higaisya->sexydamage_anim = higaisya->sexydamage;
            else							// ���ݒ�
            {
                higaisya->sexydamage_haba = abs((int)higaisya->sexydamage - (int)higaisya->sexydamage_anim) / 8;
                if (higaisya->sexydamage_haba == 0)
                    higaisya->sexydamage_haba = 1;
            }
        }

        switch(aif->hit & 0x000F0000){//�q�b�g�}�[�N�`��
        case 0:break;
        case HITINFO_MARK1:AddEffect(EFCTID_MARK1,(int)kas_point.x,(int)kas_point.y);break;
        case HITINFO_MARK2:AddEffect(EFCTID_MARK2,(int)kas_point.x,(int)kas_point.y);break;
        case HITINFO_MARK3:AddEffect(EFCTID_MARK3,(int)kas_point.x,(int)kas_point.y);break;
        case HITINFO_MARK4:AddEffect(EFCTID_MARK4,(int)kas_point.x,(int)kas_point.y);break;
        }
        switch(aif->hit & 0x00F00000){//���ʉ�
        case 0:break;
        case HITINFO_SNDHIT1:g_system.PlaySystemSound(SYSTEMSOUND_HIT1);break;
        case HITINFO_SNDHIT2:g_system.PlaySystemSound(SYSTEMSOUND_HIT2);break;
        case HITINFO_SNDHIT3:g_system.PlaySystemSound(SYSTEMSOUND_HIT3);break;
        case HITINFO_SNDSHK1:g_system.PlaySystemSound(SYSTEMSOUND_SHOCK1);break;
        case HITINFO_SNDSHK2:g_system.PlaySystemSound(SYSTEMSOUND_SHOCK2);break;
        }
        switch(aif->hit & 0x0F000000){//�q�b�g�X�g�b�v
        case 0:break;
        case HITINFO_SIV1:HitStop( 4,k_id);break;
        case HITINFO_SIV2:HitStop( 7,k_id);break;
        case HITINFO_SIV3:HitStop(13,k_id);break;
        case HITINFO_STOP:HitStop(40,k_id);break;
        }
        int vib_type = 0;		// �U���̋���[0..2]
        if(!(res & 0x40000000)){
            switch(aif->hit & 0x0000000F){
            case HITINFO_REACT1://�ʏ��U��
                switch(res & 0x0000000F){
                case 1:pdat->aid=ACTID_DAMAGE1;break;//����
                case 2:pdat->aid=ACTID_DAMAGEC1;break;//���Ⴊ��
                case 3:pdat->aid=ACTID_DAMAGEJ1;break;//��
                }
                vib_type = 0;
                break;
            case HITINFO_REACT2://�ʏ풆�U��
                switch(res & 0x0000000F){
                case 1:pdat->aid=ACTID_DAMAGE2;break;//����
                case 2:pdat->aid=ACTID_DAMAGEC2;break;//���Ⴊ��
                case 3:pdat->aid=ACTID_DAMAGEJ2;break;//��
                }
                vib_type = 1;
                break;
            case HITINFO_REACT3://�ʏ틭�U��
                switch(res & 0x0000000F){
                case 1:pdat->aid=ACTID_DAMAGE3;break;//����
                case 2:pdat->aid=ACTID_DAMAGEC3;break;//���Ⴊ��
                case 3:pdat->aid=ACTID_DAMAGEJ3;break;//��
                }
                vib_type = 2;
                break;
            case HITINFO_DOWN://�������Ȃǂł̓]�|
                pdat->aid=ACTID_DOWN;
                vib_type = 1;
                break;
            case HITINFO_FUTTOBI://�Ԃ����
                pdat->aid=ACTID_FUTTOBI;
                vib_type = 2;
                break;
            case HITINFO_FUTTOBI2://�Ԃ����2
                pdat->aid=ACTID_FUTTOBI2;
                vib_type = 2;
                break;
            case HITINFO_REACT1A://�ʏ��U���A�̂�����Ȃ�
                switch(res & 0x0000000F){
                case 1:pdat->aid=ACTID_DAMAGE1A;break;//����
                case 2:pdat->aid=ACTID_DAMAGEC1A;break;//���Ⴊ��
                case 3:pdat->aid=ACTID_DAMAGEJ1;break;//�󒆁i�́A���ʁj
                }
                vib_type = 0;
                break;
            case HITINFO_REACT2A://�ʏ풆�U���A�̂�����Ȃ�
                switch(res & 0x0000000F){
                case 1:pdat->aid=ACTID_DAMAGE2A;break;//����
                case 2:pdat->aid=ACTID_DAMAGEC2A;break;//���Ⴊ��
                case 3:pdat->aid=ACTID_DAMAGEJ2;break;//�󒆁i�́A���ʁj
                }
                vib_type = 1;
                break;
            case HITINFO_REACT3A://�ʏ틭�U���A�̂�����Ȃ�
                switch(res & 0x0000000F){
                case 1:pdat->aid=ACTID_DAMAGE3A;break;//����
                case 2:pdat->aid=ACTID_DAMAGEC3A;break;//���Ⴊ��
                case 3:pdat->aid=ACTID_DAMAGEJ3;break;//�󒆁i�́A���ʁj
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
            case HITINFO_USERDEFINE://���[�U�[��`��炢
                switch(res & 0x0000000F){
                case 1://����
                case 2:pdat->aid=ACTID_USERHIT1;break;//���Ⴊ��
                case 3:pdat->aid=ACTID_USERHIT2;break;//��
                }
                higaisya->userhit_attacker = a_id;
                higaisya->userhit_id = (aif->hit&HITINFO_USERIDMASK)>>4;
                vib_type = 1;
                break;
            case HITINFO_USERDEFINE2://���[�U�[��`��炢�i�Q�j
                pdat->aid=ACTID_USERHIT2;//��ɋ�
                higaisya->userhit_attacker = a_id;
                higaisya->userhit_id = (aif->hit&HITINFO_USERIDMASK)>>4;
                vib_type = 1;
                break;
            }
        }

        if(pdat->hp<=0){//���S
            if(g_battleinfo.GetBattleType()==TAISENKEISIKI_GOCYAMAZE)
                pdat->aid=ACTID_FINALDOWN;
            else
                pdat->aid=ACTID_KAITENFINISH;
            if(aif->hit & HITINFO_EFCTBURN )AddEffect(EFCTID_BURN,0,0,k_id);
            if(aif->hit & HITINFO_EFCTBURN_B )AddEffect(EFCTID_BURN_B,0,0,k_id);
            if(aif->hit & HITINFO_EFCTBURN_G )AddEffect(EFCTID_BURN_G,0,0,k_id);
            AddEffect(EFCTID_SINDO,2,20);
        }

        //�G�t�F�N�g
        if((aif->hit & 0x0000F000) & HITINFO_EFCTSINDO)AddEffect(EFCTID_SINDO,2,20);
        if((aif->hit & 0x0000F000) & HITINFO_EFCTBURN  )AddEffect(EFCTID_BURN  ,0,0,k_id);
        if((aif->hit & 0x0000F000) & HITINFO_EFCTBURN_B)AddEffect(EFCTID_BURN_B,0,0,k_id);
        if((aif->hit & 0x0000F000) & HITINFO_EFCTBURN_G)AddEffect(EFCTID_BURN_G,0,0,k_id);
        if (aif->hit & HITINFO_EFCTFLASH) AddEffect(EFCTID_FLASH,4,0);

        //�t�B�[�h�o�b�N�i�b��j
        g_input.StartForce( g_battleinfo.GetKeyAssign((attacker->dll_id - 1) / MAXNUM_TEAM, (attacker->dll_id - 1) % MAXNUM_TEAM), vib_type );
        g_input.StartForce( g_battleinfo.GetKeyAssign((higaisya->dll_id - 1) / MAXNUM_TEAM, (higaisya->dll_id - 1) % MAXNUM_TEAM), vib_type );

        //�������Ƃ��A�s�����X�g�b�v���Ċi�D�����̂�1�񂾂�action()������
        if(!(res & 0x40000000)){
            actidchanged = TRUE;
        }
        //�U���������������Ƃ��U��������ɒʒm
        attacker->Message(GOBJMSG_TOUCHB,TRUE);
    }
    else if(res & 0x20000000){//�K�[�h����
        if(bf_state==BFSTATE_FIGHTING)pdat->hp -= aif->kezuri;//���
        if(aif->kezuri==0)AddEffect(EFCTID_MARKG,(int)kas_point.x,(int)kas_point.y);//�K�[�h�}�[�N�`��
        else AddEffect(EFCTID_MARKG2,(int)kas_point.x,(int)kas_point.y);
        switch(aif->guard & 0x0F000000){//�q�b�g�X�g�b�v
        case 0:break;
        case GUARDINFO_SIV1:HitStop( 4,k_id);break;
        case GUARDINFO_SIV2:HitStop( 7,k_id);break;
        case GUARDINFO_SIV3:HitStop(13,k_id);break;
        case GUARDINFO_STOP:HitStop(40,k_id);break;
        }
        g_system.PlaySystemSound(SYSTEMSOUND_GUARD);//���ʉ�
        if(!(res & 0x40000000)){
            switch(aif->guard & 0x0000000F){
            case GUARDINFO_REACT1://��d��
                switch(res & 0x0000000F){
                case 1:pdat->aid=ACTID_GUARD1;break;//����
                case 2:pdat->aid=ACTID_GUARDC1;break;//���Ⴊ��
                case 3:pdat->aid=ACTID_GUARDJ1;break;//��
                }
                break;
            case GUARDINFO_REACT2://���d��
                switch(res & 0x0000000F){
                case 1:pdat->aid=ACTID_GUARD2;break;//����
                case 2:pdat->aid=ACTID_GUARDC2;break;//���Ⴊ��
                case 3:pdat->aid=ACTID_GUARDJ2;break;//��
                }
                break;
            case GUARDINFO_REACT3://���d��
                switch(res & 0x0000000F){
                case 1:pdat->aid=ACTID_GUARD3;break;//����
                case 2:pdat->aid=ACTID_GUARDC3;break;//���Ⴊ��
                case 3:pdat->aid=ACTID_GUARDJ3;break;//��
                }
                break;
            case GUARDINFO_USERDEFINE://���[�U�[��`�d��
                switch(res & 0x0000000F){
                case 1://����
                case 2:pdat->aid=ACTID_USERGUARD1;break;//���Ⴊ��
                case 3:pdat->aid=ACTID_USERGUARD2;break;//�󒆁i�́A���ʁj
                }
                higaisya->userhit_attacker = a_id;
                higaisya->userhit_id = (aif->hit&GUARDINFO_USERIDMASK)>>4;
                break;
            }
        }
        switch(aif->hit & 0x0000F000){//�G�t�F�N�g
        case GUARDINFO_EFCTSINDO:
            AddEffect(EFCTID_SINDO,2,20);
            break;
/*		case GUARDINFO_EFCTFLASH:			// �������������̂ŕۗ�
            AddEffect(EFCTID_FLASH,4,0);
            break;
*/		}
    
        if(pdat->hp<=0){//���S
            if(g_battleinfo.GetBattleType()==TAISENKEISIKI_GOCYAMAZE)
                pdat->aid=ACTID_FINALDOWN;
            else
                pdat->aid=ACTID_KAITENFINISH;
        }

        //�������Ƃ��A�s�����X�g�b�v���Ċi�D�����̂�1�񂾂�action()������
        if(!(res & 0x40000000)){
            higaisya->ActionIDChanged(TRUE,TRUE);
            higaisya->Message(GOBJMSG_ACTION);
        }
        //�U���������������Ƃ��U��������ɒʒm
        attacker->Message(GOBJMSG_TOUCHB,FALSE);
    }
    else{
        //�U���𖳎������Ȃ�Ό��̍U���͏��ɖ߂��Ă���
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
    default:sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"�s���H,%d",bf_counter);break;
    }
    sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n");
//	sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"num_char=%d , ",g_battleinfo.GetNumTeam());
//	switch(g_battleinfo.GetBattleType()){
//	case TAISENKEISIKI_GOCYAMAZE	:sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"TAISENKEISIKI_GOCYAMAZE , tl=%d\n",g_battleinfo.GetLimitTime());break;
//	case TAISENKEISIKI_KOUTAI		:sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"TAISENKEISIKI_KOUTAI , tl=%d\n",g_battleinfo.GetLimitTime());break;
//	case TAISENKEISIKI_JYUNBAN	:sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"TAISENKEISIKI_JYUNBAN , tl=%d\n",g_battleinfo.GetLimitTime());break;
//	case KEISIKI_END		:sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"KEISIKI_END , tl=%d\n",g_battleinfo.GetLimitTime());break;
//	default					:sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"�s���H , tl=%d\n",g_battleinfo.GetLimitTime());break;
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
        if(p_objects[i]!=NULL)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"��");
        else sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"��");
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
        //���O
        sprintf(debugmsgbuff,g_charlist.GetCharacterName(g_battleinfo.GetCharacter(j,active_character[j])));
        if(GetGObject( charobjid[j][active_character[j]] ) ==NULL){
            sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n\n �ُ�F�I�u�W�F�N�g��NULL�ł�");
        }
        else{
            pdat = &(GetGObject( charobjid[j][active_character[j]] )->data);
            sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n ID:%d , �`�[��:%d , �G:%d , ���[�U�[ID:%d",pdat->id,pdat->tid,pdat->eid,pdat->uid);
            sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n cell#:%d",pdat->cnow);
            if(pdat->muki)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n ����:<- ");
            else sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n ����:-> ");
            if(pdat->revx)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"X���] ");
            if(pdat->revy)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"Y���] ");
            sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"�g�嗦(%5.2f,%5.2f) ��]:%d",
                pdat->magx,pdat->magy,pdat->rot);
            sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n ���W(%5.2f,%5.2f)",pdat->x,pdat->y);
            if(FALSE){
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n\n �ُ�F�I�u�W�F�N�g- pobjdat_a ��NULL�ł�");
            }
            else{
                //���W
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n ���x(%5.3f,%5.3f) - �ۑ��x(%5.3f,%5.3f)",
                    pdat->vx,pdat->vy,pdat->ax,pdat->ay);
                //�s��ID
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n �s��ID:%d (",pdat->aid);
                if(pdat->aid & ACTID_KUCYU)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"�� ");
                if(pdat->aid & ACTID_SYAGAMI)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"�� ");
                if(pdat->aid & ACTID_ATTACK)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"�U�� ");
                if(pdat->aid & ACTID_HISSATU)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"�K�E ");
                if(pdat->aid & ACTID_SYSTEM)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"�V�X�e�� ");
                if(pdat->aid & ACTID_KURAI)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"��炢 ");
                if(pdat->aid & ACTID_GUARD)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"�K�[�h ");
                if(pdat->aid & ACTID_NAGE)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"���� ");
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"- %d )",pdat->aid&0x00000FFFF);
            }
        }
        //�e�L�X�g�`��
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
        //���O
        sprintf(debugmsgbuff,g_charlist.GetCharacterName(g_battleinfo.GetCharacter(j,active_character[j])));
        if(GetGObject( charobjid[j][active_character[j]] ) ==NULL){
            sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n\n �ُ�F�I�u�W�F�N�g��NULL�ł�");
        }
        else{
            pdat = &(GetGObject( charobjid[j][active_character[j]] )->data);
            if(FALSE){
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n\n �ُ�F�I�u�W�F�N�g- pobjdat_a ��NULL�ł�");
            }
            else{
                //�J�E���^
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n counter=%d",pdat->counter);
                //�̗́E�Q�[�W
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n �̗�:%d/%d  �Q�[�W:%1.4f/%lu.0000",pdat->hp,pdat->hpmax,pdat->gauge,pdat->gaugemax);
                //�e�X�C�b�`
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n �d�Ȃ蔻��");
                if(pdat->kasanari)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"ON");
                else sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"OFF");
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)]," ���G");
                if(pdat->muteki)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"ON");
                else sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"OFF");
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n �U����");
                if(pdat->kougeki)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"ON");
                else sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"OFF");
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)]," �������");
                if(pdat->nagerare)sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"ON");
                else sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"OFF");
            }
            if(FALSE){
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n\n �ُ�F�I�u�W�F�N�g- pobjdat_d ��NULL�ł�");
            }
            else{
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n �F:(%d / %d,%d,%d)",
                    (pdat->color&0xFF000000)/(256*256*256),
                    (pdat->color&0x00FF0000)/(256*256),
                    (pdat->color&0x0000FF00)/(256),
                    (pdat->color&0x000000FF));
                sprintf(&debugmsgbuff[strlen(debugmsgbuff)],"\n z���W:%5.2f",pdat->z);
            }
        }
        //�e�L�X�g�`��
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

    ������Ԃ̍X�V

------------------------------------------------------------------*/
void CBattleTask::T_UpdateStatus()
{
    BattleTaskState prv_state=bf_state;

    switch(bf_state)
    {
        case BFSTATE_WAITFORENDPOSE	:T_UpdateStatus_WaitForEndPose();	break;//�o��|�[�Y�I���҂�
        case BFSTATE_ROUNDCALL		:T_UpdateStatus_RoundCall();		break;//���E���h�R�[���I���҂�
        case BFSTATE_FIGHTING		:T_UpdateStatus_Fighting();			break;//�퓬���
        case BFSTATE_FINISHED		:T_UpdateStatus_Finished();			break;//KO�A�L�����N�^�̍ŏI�_�E���I���҂�
        case BFSTATE_WAITFORENDWIN	:T_UpdateStatus_WaitForEndWin();	break;//�����|�[�Y�I���҂�
        case BFSTATE_DOUBLEKO		:T_UpdateStatus_DoubleKO();			break;//�_�u��KO�\���I���҂�
        case BFSTATE_TIMEOVER		:T_UpdateStatus_TimeOver();			break;//�^�C���I�[�o�[�\���I���҂�
    }

    bf_counter++;
    if(prv_state!=bf_state){
        bf_counter=0;
    }
}


//�o��|�[�Y�I���҂�
void CBattleTask::T_UpdateStatus_WaitForEndPose()
{
    g_system.PushSysTag(__FUNCTION__);

    BOOL next=TRUE;

    //�o��|�[�Y�I���t���O���S��ON�ɂȂ��Ă��邩�H
    for(int i=0;i<2;i++){
        for(int j=0;j<MAXNUM_TEAM;j++){
            if(!m_tojyo_end[i][j]){
                next = FALSE;
            }
        }
    }
    if(bf_counter>600){//�~�ϑ[�u
        next=TRUE;
    }

    if(next){
        bf_state=BFSTATE_ROUNDCALL;
        gbl.ods("WAITFORENDPOSE �� BFSTATE_ROUNDCALL");
    }

    g_system.PopSysTag();
}

//���E���h�R�[���I���҂�
void CBattleTask::T_UpdateStatus_RoundCall()
{
    g_system.PushSysTag(__FUNCTION__);

    if(bf_counter==2){
        if(dsb_round!=NULL)dsb_round->Play(0,0,0);//�u���E���h�`�v
        switch(round){
        case 1:AddEffect(EFCTID_ROUND1,0,-1000);break;
        case 2:AddEffect(EFCTID_ROUND2,0,-1000);break;
        case 3:AddEffect(EFCTID_ROUND3,0,-1000);break;
        case 4:AddEffect(EFCTID_ROUND4,0,-1000);break;
        case 5:AddEffect(EFCTID_ROUND5,0,-1000);break;
        case 6:AddEffect(EFCTID_ROUND6,0,-1000);break;
        }
    }
    if(bf_counter==70 * 1.5)//�u���E���h�`�v���I�����鎞�ԁi��́j
    {
        if(dsb_fight!=NULL)dsb_fight->Play(0,0,0);//�u�t�@�C�g�v
        AddEffect(EFCTID_FIGHT,0,0);
    }
    if(bf_counter>130 * 1.5)//�u�t�@�C�g�v���I�����鎞�ԁi��́j
    {
        bf_state=BFSTATE_FIGHTING;
        gbl.ods("BFSTATE_ROUNDCALL �� BFSTATE_FIGHTING");
    }

    g_system.PopSysTag();
}

//�퓬���
void CBattleTask::T_UpdateStatus_Fighting()
{
    g_system.PushSysTag(__FUNCTION__);

    int i,j;
    CGObject *pobj , *pobj2 , *pobj3;
    int k;

    //�^�C�����~�b�g����
    if(!act_stop && limittime>0)
    {
        if(bf_counter%70==0)limittime--;
        if(limittime==0){

            if(dsb_timeover!=NULL)dsb_timeover->Play(0,0,0);

            AddEffect(EFCTID_TIMEOVER,0,0,0);
            bf_state = BFSTATE_TIMEOVER;
            bf_counter = 0;
            gbl.ods("BFSTATE_FIGHTING �� BFSTATE_TIMEOVER");
            g_system.PopSysTag();
            return;
        }
    }

    //���S�t���O�X�V
    Update_DeadFlag();

    BOOL the_end=FALSE;

    switch(g_battleinfo.GetBattleType())
    {
        case TAISENKEISIKI_GOCYAMAZE:
            {
                //����ł������ʂ���O�ɏo�Ă�����
                for(j=0;j<2;j++){
                    for(i=0;i<(int)g_battleinfo.GetNumTeam(j);i++){
                        if(m_dead_one[j][i] && !m_all_dead[j])
                        {
                            GetGObject(charobjid[j][i])->data.objtype &= ~GOBJFLG_DISPLAYME;
                            GetGObject(charobjid[j][i])->data.objtype &= ~GOBJFLG_CLIPX2;
                        }
                    }
                }
                //���E���h�I������
                if( m_all_dead[0] || m_all_dead[1] )
                {
                    the_end=TRUE;

                    //�_�E���҂��t���O�ݒ�
                    m_finaldown_end[0]=m_all_dead[0] ? FALSE : TRUE;
                    m_finaldown_end[1]=m_all_dead[1] ? FALSE : TRUE;
                }
            }
            break;
        case TAISENKEISIKI_JYUNBAN:
            {
                //���E���h�I������
                if( m_active_dead[0] || m_active_dead[1] )
                {
                    the_end=TRUE;

                    //�_�E���҂��t���O�ݒ�
                    m_finaldown_end[0]=m_active_dead[0] ? FALSE : TRUE;
                    m_finaldown_end[1]=m_active_dead[1] ? FALSE : TRUE;
                }

                else
                {
                    //�x���U���v������
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
                    }//���x���U���v�����肱���܂�
                }
            }
            break;
        case TAISENKEISIKI_KOUTAI:
            {
                //���E���h�I������
                if( m_all_dead[0] || m_all_dead[1] )
                {
                    the_end=TRUE;

                    //�_�E���҂��t���O�ݒ�
                    m_finaldown_end[0]=m_all_dead[0] ? FALSE : TRUE;
                    m_finaldown_end[1]=m_all_dead[1] ? FALSE : TRUE;
                }
                else
                {
                    //��㔻��(���S��)
                    for(j=0;j<2;j++)
                    {
                        if( m_active_dead[j] )
                        {
                            for(i=0;i<(int)g_battleinfo.GetNumTeam(j);i++)//�����Ă�����T���Č�シ��
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
                            if(i==3)g_system.Log("���L�����N�^�[�̌��Ɏ��s\n",SYSLOG_WARNING);
                        }
                    }

                    //��㔻��(������)
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
                                //�������݂�
                                if(pobj3)
                                {
                                    //�{���̓V�X�e�����b�Z�[�W�𔭍s���ׂ���������Ȃ������
                                    if(pobj3->Message(GOBJMSG_KOUTAI,charobjid[j][active_character[j]]))
                                    {
                                        hprecratio[j][active_character[j]]*=1.8;		//HP�񕜃C���^�[�o����
                                        active_character[j]=k;
                                        pobj->Message(GOBJMSG_TAIKI,0);
                                    }
                                }
                            }
                        }
                    }//����㔻��A�����܂�

                    //�x���U���v������
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
                    }//���x���U���v�����肱���܂�
                }
            }
            break;
    }//switch�u���b�N�I��

    if(the_end)
    {
        //�G�t�F�N�g����
        AddEffect(EFCTID_KO,0,-1000);
        efct_slowdown=60;
        if(dsb_ko!=NULL)dsb_ko->Play(0,0,0);
    
        //���E���h���Ґݒ�
        if(!m_finaldown_end[0] && !m_finaldown_end[1]){//�_�u��KO
            m_round_winner=2;
        }
        else{
            m_round_winner = m_finaldown_end[0] ? 0 : 1;
        }

        //��ԕύX
        bf_state = BFSTATE_FINISHED;
        gbl.ods("BFSTATE_FIGHTING �� BFSTATE_FINISHED");
    }
    
    g_system.PopSysTag();
}

//KO�A�L�����N�^�̍ŏI�_�E���I���҂�
void CBattleTask::T_UpdateStatus_Finished()
{
    g_system.PushSysTag(__FUNCTION__);

    int i,j,j2;
    CGObject *pobj;
    BOOL next=FALSE;

    if(m_finaldown_end[0] && m_finaldown_end[1])next=TRUE;//�_�E������
    if(bf_counter<100)next=FALSE;	//������
    if(bf_counter>600)next=TRUE;	//�~�ϑ[�u

    if(next)
    {
        if(m_round_winner==2)//�_�u���c��
        {
            bf_state=BFSTATE_DOUBLEKO;
            gbl.ods("BFSTATE_FINISHED �� BFSTATE_DOUBLEKO");
        }
        else {
            if(m_round_winner==0){j=0;j2=1;}//j�͏�������
            else {j=1;j2=0;}
            wincount[j]++;
            switch(g_battleinfo.GetBattleType()){
                case TAISENKEISIKI_GOCYAMAZE:
                    {
                        for(i=0;i<(int)g_battleinfo.GetNumTeam(j);i++){
                            if(!m_dead_one[j][i]){
                                winner_oid = charobjid[j][i];//���̃L�����N�^�[�̏����|�[�Y�I����҂�
                                GetGObject(winner_oid)->Message(GOBJMSG_DOYOUWIN);
                            }
                        }
                    }
                    break;
                case TAISENKEISIKI_JYUNBAN:
                    {
                        //�����Ă�ق���HP��
                        pobj=GetGObject(charobjid[j][active_character[j]]);
                        if(pobj!=NULL){
                            pobj->data.hp+=150;
                            if(pobj->data.hp > (int)pobj->data.hpmax){
                                pobj->data.hp=pobj->data.hpmax;
                            }
                        }
                        else {
                            g_system.Log("CBattleTask::T_UpdateStatus_Finished KOF�`�� ���҂Ȃ�?",SYSLOG_ERROR);
                            g_system.ReturnTitle();
                        }
                    }//���Ɠ��������𑱍s
                case TAISENKEISIKI_KOUTAI:
                    {
                        for(i=0;i<(int)g_battleinfo.GetNumTeam(j);i++)
                        {
                            if(!m_dead_one[j][i]){
                                if(i==(int)active_character[j]){
                                    winner_oid = charobjid[j][active_character[j]];//���̃L�����N�^�[�̏����|�[�Y�I����҂�
                                    GetGObject(winner_oid)->Message(GOBJMSG_DOYOUWIN);
                                }
                                else if(m_all_dead[j2]){
                                    GetGObject(charobjid[j][i])->Message(GOBJMSG_DOYOUWIN2,charobjid[j][active_character[j]]);//�O������ł��ď����|�[�Y
                                }
                            }
                        }
                    }
                    break;
            }
            bf_state=BFSTATE_WAITFORENDWIN;
            m_winpose_end = FALSE;
            gbl.ods("BFSTATE_FINISHED �� WAITFORENDWIN");
        }
    }

    g_system.PopSysTag();
}

//�����|�[�Y�I���҂�
void CBattleTask::T_UpdateStatus_WaitForEndWin()
{
    g_system.PushSysTag(__FUNCTION__);

    BYTE  lteam;
    BOOL next=FALSE;

    if(m_winpose_end && bf_counter>150)next=TRUE;
    if(g_battleinfo.GetBattleType()==TAISENKEISIKI_GOCYAMAZE && (g_battleinfo.GetAllKey()&0xFFFF0000) ){//�{�^�����͂ŃX�L�b�v
        next=TRUE;
    }
    if(bf_counter>600)next=TRUE;//�~�ϑ[�u

    //����
    if(next)//�I������
    {
        //�����ΐ�̏ꍇ
        if(g_battleinfo.GetBattleType()==TAISENKEISIKI_GOCYAMAZE)
        {
            gbl.ods("WAITFORENDWIN �� StartRound");
            StartRound();
            g_system.PopSysTag();
            return;
        }

        lteam = ( m_round_winner==0 ? 1 : 0 );//�����`�[��

        //�����I���H
        // TAISENKEISIKI_KOUTAI �� �K��TRUE
        // TAISENKEISIKI_JYUNBAN�� FALSE�̏ꍇ������
        if(m_all_dead[lteam])
        {
            wincount[m_round_winner]++;
            StartRound();
            g_system.PopSysTag();
            return;
        }
        
        if(g_battleinfo.GetBattleType()==TAISENKEISIKI_KOUTAI){
            g_system.Log("��CBattleTask::T_UpdateStatus_WaitForEndWin - ��������",SYSLOG_ERROR);
            g_system.ReturnTitle();
        }

        //���̃L�����N�^�[���o��(TAISENKEISIKI_JYUNBAN�̂�)
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
        {//���E���h�J�nwav�X�V
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

//�_�u��KO�\���I���҂�
void CBattleTask::T_UpdateStatus_DoubleKO()
{
/*	if(++bf_counter>200 || (g_input.GetKey(0,0)&0xFFFF0000 || g_input.GetKey(1,0)&0xFFFF0000))
    {
        gbl.ods("DOUBLEKO �� StartRound\n");
        if(!m_all_dead[0]){
            wincount[0]++;
        }
        if(!m_all_dead[1]){
            wincount[1]++;
        }
        StartRound();
    }*/
    gbl.ods("���_�u��KO����������");
    g_system.Log("�_�u��KO����������",SYSLOG_WARNING);
    g_system.ReturnTitle();
}

//�^�C���I�[�o�[�\���I���҂�
void CBattleTask::T_UpdateStatus_TimeOver()
{
    g_system.PushSysTag(__FUNCTION__);

    int i,j;

    double hpwariai[2][MAXNUM_TEAM];

    bf_counter++;
    CGObject *pobj;
    float tmp_num;

    //�������̂͂ǂ������H�i�̗͂̌����Ō���j
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
        hpwariai[j][0]=(hpwariai[j][0]+hpwariai[j][1]+hpwariai[j][2])/tmp_num;//���ς���
    }
    if(hpwariai[0][0] < hpwariai[1][0])j=1;//1p���̕���
    else j=0;//2p���̕���
    winner = j;
    loser  = j==0 ? 1 : 0;


    //���ɐi�ށH
    BOOL do_timeover = FALSE;
    if(bf_counter>1000)
    {
        //�����I�ɐi��
        do_timeover = TRUE;
    }
    else if(bf_counter>310)
    {
        //�󒆂ɕ����Ă���z����������Ȃ�

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


    //�����|�[�Y�҂��ɐi��
    if(do_timeover)
    {
        if(g_battleinfo.GetBattleType()==TAISENKEISIKI_GOCYAMAZE)
        {
            wincount[winner]++;

            j= winner;
            for(i=0;i<(int)g_battleinfo.GetNumTeam(j);i++)
            {
                if(GetGObject(charobjid[j][i])->data.hp>0){
                    winner_oid = charobjid[j][i];//���̃L�����N�^�[�̏����|�[�Y�I����҂�
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
        gbl.ods("BFSTATE_TIMEOVER �� WAITFORENDWIN");
        bf_counter=0;
    }

    g_system.PopSysTag();
}

//���S�t���O�X�V
void CBattleTask::Update_DeadFlag()
{
    int i,j;
    CGObject *pobj;

    //�S�t���O������
    for(j=0;j<2;j++){
        for(i=0;i<MAXNUM_TEAM;i++)
        {
            m_all_dead[j]=TRUE;
            m_dead_one[j][i]=TRUE;
        }
    }

    //�`�F�b�N
    for(j=0;j<2;j++){
        for(i=0;i<(int)g_battleinfo.GetNumTeam(j);i++){
            pobj = GetGObject(charobjid[j][i]);
            if(pobj!=NULL && charobjid[j][i]!=0)
            {
                if( pobj->data.hp > 0 || (pobj->data.aid&ACTID_NAGE))
                {
                    m_dead_one[j][i]=FALSE;
                    m_all_dead[j]=FALSE;
                    //��㐧�̏ꍇ��hp��
                    if(g_battleinfo.GetBattleType()==TAISENKEISIKI_KOUTAI)
                    {
                        if(i!=(int)active_character[j]){//�A�N�e�B�u�łȂ����
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
    �L�[���͌����ݒ�X�V
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
                        if(!(obj->data.objtype&GOBJFLG_COMPUTER))	//�R���s���[�^����ł͂Ȃ�
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
    F7�Ń|�[�Y
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

