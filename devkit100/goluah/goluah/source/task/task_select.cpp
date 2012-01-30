/*=======================================================================================

	vsCOM �܂��� 1Pvs2P �ł̃L�����Z��

=========================================================================================*/

#include "define_macro.h"
#include "define_const.h"
#include "global.h"
#include "task_select.h"
#include "task_battle.h"
#include "task_simpleoptsel.h"

/*=======================================================================================

	�L�����Z���N���X�{��

=========================================================================================*/

//�e�T�u�^�X�N�̕`��v���C�I���e�B
int CCharacterSelect::m_pri_background	= 2000;
int CCharacterSelect::m_pri_face		= 1500;
int CCharacterSelect::m_pri_firstSetting= 1060;
int CCharacterSelect::m_pri_belt		= 1050;
int CCharacterSelect::m_pri_cselecter	= 1040;
int CCharacterSelect::m_pri_oselecter	= 1020;
int CCharacterSelect::m_pri_sselecter	= 1010;

/*-----------------------------------------------------------
	�\�z
-------------------------------------------------------------*/
CCharacterSelect::CCharacterSelect()
{
	//��ɂ�邱�Ɩ���
}

/*----------------------------------------------------------
	����
------------------------------------------------------------*/
void CCharacterSelect::Initialize()
{
	//�p�����[�^���Z�b�g
	charsel_ok[0]=charsel_ok[1]=FALSE;
	stgsel_ok = FALSE;
	num_selected[0]=num_selected[1]=0;
	m_condition_sel_ok = FALSE;
	for(int i=0;i<2;i++){
		for(int j=0;j<MAXNUM_TEAM;j++){
			selected_color[i][j] = 1;
			selected_option[i][j] = 0;
		}
	}

	//BGM�Đ�
	g_sound.BGMPlay(".\\system\\bgm\\choice");

	//���R���f�B�V�����Z���N�^�[�o�^
	CTConditionSelecter *csel = new CTConditionSelecter;
	csel->SetPriority( m_pri_firstSetting );
	g_system.AddTask(csel);

	//���q�[�`��N���X�o�^
	m_bg = new CTCharacterSelectBG();
	m_bg->SetPriority(m_pri_background);
	g_system.AddTask(m_bg);
}


/*----------------------------------------------------------
	�j��
------------------------------------------------------------*/
void CCharacterSelect::Terminate()
{
	g_sound.BGMStop();
}

/*-----------------------------------------------------------
	�X�V����
-------------------------------------------------------------*/
BOOL CCharacterSelect::Execute(DWORD time)
{
	//��step1
	if(!m_condition_sel_ok)return TRUE;//�����ݒ肪�܂��I���Ă��Ȃ�

	//��step2
	if(!charsel_ok[0] || !charsel_ok[1]){
		//�f�Jface�X�V
		m_bface[0]->SetTemporary(m_ring[0]->GetSelected(),selected_color[0][num_selected[0]],OPT2ALT(selected_option[0][num_selected[0]]));
		m_bface[1]->SetTemporary(m_ring[1]->GetSelected(),selected_color[1][num_selected[1]],OPT2ALT(selected_option[1][num_selected[1]]));
		//�I�r�\���X�V
		m_belt[0]->SetRing(m_ring[0]->GetRing());
		m_belt[1]->SetRing(m_ring[1]->GetRing());
		return TRUE;
	}

	//��step3
	if(stgsel_ok)//�X�e�[�W�I���̉��o�X�L�b�v����
	{
		DWORD keystate = 0;

		//�����ɎQ�����S�Ẵv���C���[�̃L�[��Ԃ𑫂�
		int ass_num=0;
		for(int i=0;i<2;i++){
			for(int j=0;j<MAXNUM_TEAM;j++){
				if(!(m_assign[i][j]&CASSIGN_SPECIFIC)){
					keystate |= g_input.GetKey(m_assign[i][j],0);
					ass_num++;
				}
			}
		}
		if(!ass_num){//�S���R���s���[�^�Ȃ�΂��傤���Ȃ�����v���C���[1�̓�͂�󂯎��
			keystate = g_input.GetKey(0,0);
		}

		//�{�^����������Ă����玟�ɐi�񂶂Ⴄ
		if(keystate & KEYSTA_BUTTONS){
			ResolveRandom();
			SetupBattleInfo();
			CreateNextTask();
		}
	}

	return TRUE;
}

void CCharacterSelect::CreateNextTask()
{
	g_system.AddTask( new CBattleTask );
}

/*-----------------------------------------------------------
	�����R���f�B�V�����I�����
-------------------------------------------------------------*/
void CCharacterSelect::OnConditionDecided(CTConditionSelecter *pcsel)
{
	m_condition_sel_ok = TRUE;
	int i,j;

	//���蓖�ď���R�s�[
	memcpy(m_assign , pcsel->m_assign , sizeof(BYTE)*MAXNUM_TEAM*2);
	//�l���J�E���g
	for(i=0;i<2;i++){
		wanted_char_num[i] = 0;
		for(j=0;j<MAXNUM_TEAM;j++){
			if(m_assign[i][j]!=CASSIGN_NONE){
				wanted_char_num[i]++;
			}
		}
	}
	//�\�[�g�i�u���蓖�ĂȂ��v��A���̂ق��Ɉړ��j
	for(i=0;i<2;i++){
		for(j=0;j<MAXNUM_TEAM-1;j++){
			if(m_assign[i][j]==CASSIGN_NONE)
			{
				for(int k=j+1;k<MAXNUM_TEAM;k++)//j�����ŁA���蓖�Ă̂����̂�T��
				{
					if(m_assign[i][k]!=CASSIGN_NONE){//�n�P�[��
						//��
						m_assign[i][j] = m_assign[i][k];
						m_assign[i][k] = CASSIGN_NONE;
						break;
					}
				}
			}
		}
	}

	g_battleinfo.Initialize();					//�ΐ��񏉊�
	g_battleinfo.SetBattleType(pcsel->m_type);	//�ΐ�`����ݒ�
	g_battleinfo.SetLimitTime(
		pcsel->m_limit_time[pcsel->m_limit_time_index]);//������Ԃ�ݒ�

	for(i=0;i<2;i++)
	{
		//���L�����N�^�[�����O�N���X�o�^
		m_ring[i] = new CTCharacterRing();
		m_ring[i]->SetPriority( m_pri_cselecter );
		m_ring[i]->SetPos(320,i==0?160:320);
		m_ring[i]->SetHeight(25);
		g_system.AddTask(m_ring[i]);
		//���L�����N�^�[�����O���E�I�r
		m_belt[i] = new CTCharacterSelectBelt;
		i==0 ? m_belt[i]->SetLeft() : m_belt[i]->SetRight();
		m_belt[i]->SetPriority( m_pri_belt );
		m_belt[i]->SetBaseY(i==0 ? 160.0f : 320.0f);
		g_system.AddTask(m_belt[i]);
		//���f�J��\���N���X�o�^
		m_bface[i] = new CTCharacterBigFace();
		m_bface[i]->SetPriority( m_pri_face );
		m_bface[i]->SetLeftRight(i==0 ? TRUE : FALSE);
		m_bface[i]->SetNum(wanted_char_num[i]);
		g_system.AddTask( m_bface[i] );
		//���I�v�V�����Z���N�^�[�N���X
		m_selecter[i] = new CTSimpleOptionSelecter;
		m_selecter[i]->SetPriority( m_pri_oselecter );
		g_system.AddTask(m_selecter[i]);
	}
	
	//�L�����I��L�[��͊��蓖��
	AssignKeys(0);
	AssignKeys(1);
}

/*-----------------------------------------------------------
	�L�����N�^�I��L�[��͊��蓖��
	+�I�r�N���X��COM,�L�[��͔ԍ��̒ʒm
-------------------------------------------------------------*/
void CCharacterSelect::AssignKeys(int team)
{
	int team2 = AnotherTeam(team);

	//���ɑI���̂̓R���s���[�^
	if(m_assign[team][num_selected[team]]==CASSIGN_COM||m_assign[team][num_selected[team]]==CASSIGN_STABLE)
	{
		m_belt[team]->SetCom(TRUE);
		if(CkAllCom(team))//�`�[�����S��COM�������A
		{
			if(CkAllCom(team2))//���Е�̃`�[����S��COM
			{
				if(team==0 || charsel_ok[team2]){	//�`�[��1 �܂��� ���Е�̃`�[�����I��I���ς�
					m_ring[team]->SetKeyInputIndex(0);
					m_belt[team]->SetKey(0);
					return;
				}
			}

			if(charsel_ok[team2]){//���Е�̃`�[���͑I��I���ς�
				for(int i=0;i<MAXNUM_KEYI;i++)
				{
					//���Е�̃`�[���ɑ��݂���L�[��͔ԍ���蓖��
					if(CkKey(team2,i))
					{
						m_ring[team]->SetKeyInputIndex(i);
						m_belt[team]->SetKey(i);
						return;
					}
				}
			}

			//���Е�̃`�[���̑I��I���܂ł̓��b�N
			m_ring[team]->SetKeyInputIndex(-1);
			m_belt[team]->SetKey(-1);
			return;
		}
		else
		{
			for(int i=0;i<MAXNUM_KEYI;i++)
			{
				//���̃`�[���ɑ��݂���L�[��͔ԍ���蓖��
				if(CkKey(team,i))
				{
					m_ring[team]->SetKeyInputIndex(i);
					m_belt[team]->SetKey(i);
					return;
				}
			}
		}
	}
	//���ɑI���̂̓v���C���[
	else{
		m_belt[team]->SetCom(FALSE);
		m_ring[team]->SetKeyInputIndex( m_assign[team][num_selected[team]] );
		m_belt[team]->SetKey( m_assign[team][num_selected[team]] );
		return;
	}
}

//�w��`�[���Ɏw��L�[��͔ԍ��̃v���C���[�����蓖�Ă��Ă��邩�ǂ���
BOOL CCharacterSelect::CkKey(int team,BYTE ki)
{
	for(int i=0;i<MAXNUM_TEAM;i++){
		if(m_assign[team][i]==ki){
			return TRUE;
		}
	}
	return FALSE;
}

//�w��`�[���̎c��I��A�R���s���[�^�݂̂��ǂ���
BOOL CCharacterSelect::CkAllCom(int team)
{
	for(int i=0;i<wanted_char_num[team];i++){
		if(m_assign[team][i]!=CASSIGN_COM&&m_assign[team][i]!=CASSIGN_STABLE){
			return FALSE;
		}
	}
	return TRUE;
}

void CCharacterSelect::OnCancel()
{
/*	if (charsel_ok[0]){
		num_selected[1]--;
	}
*/	for (int i = 0; i < 2; i++)
	{
		m_ring[i]->Hide();
		m_belt[i]->Show(FALSE);
		m_bface[i]->SetPriority(0);
		m_bface[i]->ResetNum();
//		g_system.RemoveTask(m_selecter[i]->GetID());	// ���̂Ƃ����ʖ���
		for (int j = 0; j < MAXNUM_TEAM; j++)
		{
			m_bface[i]->Set(j, 0, 0);
			m_bface[i]->SetTemporary(j, 0, 0);
		}
	}
	Initialize();
}

/*-----------------------------------------------------------
	�L�����N�^�I�����
-------------------------------------------------------------*/
void CCharacterSelect::OnSelect(CTCharacterRing *pring,int cindex)
{
	//�ǂ̃����O����H
	DWORD team = 3;
	if(pring==m_ring[0])	team = 0;
	if(pring==m_ring[1])	team = 1;
	if(team>1)return;

	selected_char[team][num_selected[team]] = cindex;

	if(cindex>=0){
		m_ring[team]->Hide();
		/* �����`�[����ł̓��L�����I��ɑΉ����悤�Ƃ��ĕۗ����B�J���[�����ɂȂ�B
		if (g_charlist.GetCharacterVer(cindex) < 1000 &&
			g_charlist.GetCharacterVer(cindex) >= 680)*/
			m_ring[team]->AddSelected(cindex);		// �o�[�W����0.6x�`0.8x�̃L������1�`�[�������Q��ɑΉ����ĂȂ�
		ResolveColor(team,num_selected[team]);
		int optnum = m_selecter[team]->SetAndShow(cindex, pring->GetKeyInputIndex() );
		//�I�v�V�����Z���N�^�[�̍����v�Z�E�ʒu�w��
		if(optnum<1)optnum=1;
		float optheight = m_selecter[team]->GetHeight() + 15.0f;//10:�܁[����
		float base_y = team==0 ? 160.0f : 320.0f;
		float top_y = base_y - optheight*( team==0 ? 0.65f : 0.25f );
		float btm_y = base_y + optheight*( team==0 ? 0.25f : 0.65f );
		if(top_y < 0.0f)top_y = 5.0f;
		if(btm_y > 480.0f)btm_y = 475.0f;
		m_selecter[team]->SetPos( team==0 ? 50.0f : 235.0f , top_y+5.0f );
		m_belt[team]->Extend( top_y , btm_y );
		return;
	}

	//�ȉ������_���L��������
	m_bface[team]->Set(cindex,selected_color[team][num_selected[team]],0);//�f�Jface�X�V
	num_selected[team]++;

	if(num_selected[team]==wanted_char_num[team])//�`�[�������o�[��������
	{
		charsel_ok[team] = TRUE;
		m_ring[team]->Hide();
		m_belt[team]->Show(FALSE);
		CheckCharacterSelectOK();
	}
	else{
		AssignKeys(team);//�L�[�Ċ��蓖��
	}
}


/*-----------------------------------------------------------
	�I�v�V�����I�����
-------------------------------------------------------------*/
void CCharacterSelect::OnOptionSelect(CTOptionSelecter *pselecter,DWORD option)
{
	if (pselecter->State() == 0xFFFFFF)
		return;

	//team�H
	DWORD team = 3;
	if(pselecter==m_selecter[0] || pselecter==m_selecter[0]->GetCustomSelecter())team = 0;
	if(pselecter==m_selecter[1] || pselecter==m_selecter[1]->GetCustomSelecter())team = 1;
	if(team>1)return;
	
	selected_option[team][num_selected[team]] = option;
	m_bface[team]->Set(selected_char[team][num_selected[team]],selected_color[team][num_selected[team]],OPT2ALT(option));//�f�Jface�X�V
	num_selected[team]++;

	if(!(m_assign[team][num_selected[team]-1] & CASSIGN_SPECIFIC)){
		pselecter->ApplyToPreviousSelect();
	}

	if(num_selected[team]==wanted_char_num[team])//�`�[�������o�[��������
	{
		charsel_ok[team] = TRUE;
		m_belt[team]->Show(FALSE);
		CheckCharacterSelectOK();
	}
	else//���s
	{
		AssignKeys(team);//�L�[�Ċ��蓖��
		m_ring[team]->Restore();
		m_belt[team]->Restore();
	}
}

/*-----------------------------------------------------------
	�F�ύX������
-------------------------------------------------------------*/
void CCharacterSelect::OnChangeColor(CTCharacterRing *pring)
{
	//�ǂ̃����O����H
	DWORD team = 3;
	if(pring==m_ring[0])	team = 0;
	if(pring==m_ring[1])	team = 1;
	if(team>1)return;

	selected_color[team][num_selected[team]]++;
	if(selected_color[team][num_selected[team]]>3)selected_color[team][num_selected[team]]=1;
}


/*-----------------------------------------------------------
	�C���X�gON/OFF������
-------------------------------------------------------------*/
void CCharacterSelect::OnInstOnOff(CTCharacterRing *pring)
{
	//�ǂ̃����O����H
	DWORD team = 3;
	if(pring==m_ring[0])	team = 0;
	if(pring==m_ring[2])	team = 1;
	if(team>1)return;
}


/*-----------------------------------------------------------
	�L�����N�^�[�I��E�I�v�V�����I��C�x���g���
	�X�e�[�W�I��ɐi�ނ��ǂ����̃`�F�b�N
-------------------------------------------------------------*/
void CCharacterSelect::CheckCharacterSelectOK()
{
/*	if(g_battleinfo.GetCharacterIsCOM(1,0)){//1PvsCOM�̏ꍇ
		//�����O1�𖳌�ɂ��ă����O2�̓�͂�L��ɂ���
		if(charsel_ok[0]){
			m_ring[0]->SetKeyInputIndex(-1);
			m_ring[1]->SetKeyInputIndex(0);
		}
	}*/

	//�X�e�[�W�Z���N�g�̊J�n
	if(charsel_ok[0] && charsel_ok[1]){
		CTStageSelecter *sselect = new CTStageSelecter;
		sselect->SetPriority( m_pri_sselecter );
		//�L�[���蓖��
		int ass_num=0;
		for(int i=0;i<2;i++){
			for(int j=0;j<MAXNUM_TEAM;j++){
				if(!(m_assign[i][j]&CASSIGN_SPECIFIC)){
					sselect->AddKeyInput(m_assign[i][j]);
					ass_num++;
				}
			}
		}
		if(!ass_num){
			sselect->AddKeyInput(0);
		}

		g_system.AddTask( sselect );
		m_bg->ChangeState(TRUE);
	}

	//�L�����Z���E�L�[�Ċ��蓖��
	if(charsel_ok[0]){
		AssignKeys(1);
	}
	else if(charsel_ok[1]){
		AssignKeys(0);
	}
}


/*-----------------------------------------------------------
	�X�e�[�W�I�����
-------------------------------------------------------------*/
void CCharacterSelect::OnStageSelect(CTStageSelecter *spelecter,int sindex)
{
	selected_stage = sindex;
	stgsel_ok = TRUE;
}


/*-----------------------------------------------------------
	�I���A�j���[�V�����J�����ʒm
-------------------------------------------------------------*/
void CCharacterSelect::OnEndAnimComplete(CTaskBase* ptask)
{
	ResolveRandom();
	SetupBattleInfo();

	CreateNextTask();
	return;
}

/*-----------------------------------------------------------
	�`��
-------------------------------------------------------------*/
void CCharacterSelect::Draw()
{
	g_draw.SetTransform(FALSE);
}


/*-----------------------------------------------------------
	�I���O����
-------------------------------------------------------------*/
void CCharacterSelect::SetupBattleInfo()
{
	//�L�����N�^�ݒ�
	for(int i=0;i<2;i++){
		for(int j=0;j<wanted_char_num[i];j++){
			g_battleinfo.AddCharacter(i,
				selected_char[i][j],
				selected_color[i][j],
				m_assign[i][j],
				selected_option[i][j]);
		}
	}

	//�X�e�[�W�ݒ�
	g_battleinfo.SetStage( selected_stage );
}


/*-----------------------------------------------------------
	�����_���w�肳�ꂽ�L�����N�^�[�E�X�e�[�W��
	�L��ȃC���f�b�N�X��蓖�Ă�
-------------------------------------------------------------*/
void CCharacterSelect::ResolveRandom()
{
	srand(timeGetTime());
	BOOL jyufuku;	//�ʃ`�[���̏d��
	BOOL jyufuku2;	//���`�[���̏d��
	int count;

	//�L�����N�^�[�E�����_����
	for(int j=0;j<2;j++){
		for(int i=0;i<wanted_char_num[j];i++)
		{
			if(selected_char[j][i]<0){
				jyufuku = TRUE;
				count = 100;
				while((jyufuku && count>0) || jyufuku2)
				{
					selected_char[j][i] = rand()%g_charlist.GetCharacterCount();
					jyufuku = FALSE;
					jyufuku2 = FALSE;
					for(int k=0;k<2;k++){//�d���`�F�b�N
						for(int l=0;l<wanted_char_num[j];l++){
							if(!(k==j && l==i)){
								if(selected_char[j][i] == selected_char[k][l]){
									jyufuku = TRUE;
									if(j==k)jyufuku2=TRUE;
								}
							}
						}
					}
					count--;//count��0�ɂȂ����d�����ĂĂ��̂܂܂�����Ⴄ
				}//while
				selected_option[j][i] = g_charlist.GetRandomOption(selected_char[j][i]);
				selected_color[j][i] = rand()%MAXNUM_CHARACTERCOLOR+1;
				ResolveColor(j,i);//�J���[�̏d����`�F�b�N
			}
		}
	}

	if(selected_stage<0){
		selected_stage = rand()%g_stagelist.GetStageCount();
	}
}

/*-----------------------------------------------------------
	�J���[���d�����Ȃ��悤�A�K�؂ȃJ���[��I��Ȃ���
-------------------------------------------------------------*/
void CCharacterSelect::ResolveColor(int team,int index)
{
	int team2 = team==0 ? 1 : 0;

	for(int i=0;i<num_selected[team2];i++)
	{
		if(selected_char[team][index]==selected_char[team2][i])//�L�����N�^�[������
		{
			if(selected_color[team][index]==selected_color[team2][i])//�F������
			{
				switch(selected_color[team2][i]){
				case 1:selected_color[team][index]=2;break;
				case 2:selected_color[team][index]=1;break;
				case 3:selected_color[team][index]=1;break;
				}
				return;
			}
		}
	}
}



/*=======================================================================================

	�~�j�烊���O

=========================================================================================*/

DWORD CTCharacterRing::m_instanceCount;
MYSURFACE** CTCharacterRing::m_dds_miniface;

UINT CTCharacterRing::sm_selected_ring1p	=0;
UINT CTCharacterRing::sm_selected_index1p	=0;
UINT CTCharacterRing::sm_selected_ring2p	=0;
UINT CTCharacterRing::sm_selected_index2p	=0;

/*-----------------------------------------------------------
	����
-------------------------------------------------------------*/
void CTCharacterRing::InitializeSub()
{
	//�p�����[�^���Z�b�g
	m_counter = 0;
	m_state = CTCRS_NotReady;
	m_radius_base = 120;
	m_sleepCount = 0;
}

void CTCharacterRing::InitializeIcons()
{
	char *filepath = new char [MAX_PATH];

	//static����I���Ă����l���A
	m_selected_ring = (m_instanceCount==0) ? sm_selected_ring1p : sm_selected_ring2p;
	m_selected_index = (m_instanceCount==0) ? sm_selected_index1p : sm_selected_index2p;

	//�~�j��̓ǂݍ���
	if(m_instanceCount==0){
		m_dds_miniface = new MYSURFACE* [g_charlist.GetCharacterCount()];
		for(int i=0;i<g_charlist.GetCharacterCount();i++){
			m_dds_miniface[i]=NULL;
			sprintf(filepath,"%s\\face1",g_charlist.GetCharacterDir(i));
			m_dds_miniface[i] = g_draw.CreateSurfaceFrom256Image(filepath);
		}
	}
	delete [] filepath;
	m_instanceCount++;
}


/*-----------------------------------------------------------
	�j��
-------------------------------------------------------------*/

void CTCharacterRing::DestroyIcons()
{
	m_instanceCount--;

	//static �ɒl��ۑ�
	if(m_instanceCount==0)
	{
		//�i���Ԃ�j2P
		sm_selected_ring2p = m_selected_ring;
		sm_selected_index2p = m_selected_index;
	}
	else
	{
		//�i���Ԃ�j1P
		sm_selected_ring1p = m_selected_ring;
		sm_selected_index1p = m_selected_index;
	}

	if(m_instanceCount==0){
		for(int i=0;i<g_charlist.GetCharacterCount();i++){
			RELSURFACE(m_dds_miniface[i]);
		}
		delete [] m_dds_miniface;
	}

}

/*-----------------------------------------------------------
	�I��ꂽ�L�����N�^��擾
-------------------------------------------------------------*/
UINT CTCharacterRing::GetSelected()
{
	return g_charlist.RingIndex2SerialIndex(m_selected_ring,m_selected_index);
}


/*-----------------------------------------------------------
	�`��p�̐F��擾
-------------------------------------------------------------*/
DWORD CTCharacterRing::GetColor(UINT ridx,UINT idx)
{
	UINT sindex = g_charlist.RingIndex2SerialIndex(ridx,idx);

	std::list<DWORD>::iterator i=m_selectedlist.begin();
	std::list<DWORD>::iterator ied=m_selectedlist.end();
	for(;i!=ied;i++){
		if(*i==sindex){//��ɑI��ꂽ�L�����N�^�[
			return 0x00888888;
		}
	}
	return 0x00FFFFFF;//�܂��I���Ă��Ȃ��L�����N�^�[
}

/*------------------------------------------------------------
	�w��L�����N�^�[����ɑI���Ă��邩�ǂ����`�F�b�N
--------------------------------------------------------------*/
BOOL CTCharacterRing::IsSelected(DWORD cindex)
{
	std::list<DWORD>::iterator i= m_selectedlist.begin();
	std::list<DWORD>::iterator ied= m_selectedlist.end();
	for(;i!=ied;i++){
		if(*i==cindex)return TRUE;
	}
	return FALSE;
}

/*--------------------------------------------------------------------------------------
	�\������A�C�R���摜��擾�iDraw����R�[���j
----------------------------------------------------------------------------------------*/
MYSURFACE* CTCharacterRing::GetIcon(UINT ridx,UINT idx)
{
	if(!m_dds_miniface)return NULL;

	UINT sindex = g_charlist.RingIndex2SerialIndex(ridx,idx);
	if((int)sindex > g_charlist.GetCharacterCount())return NULL;

	return m_dds_miniface[sindex];
}

/*--------------------------------------------------------------------------------------
	�{�^���������ꂽ�Ƃ��̏���
----------------------------------------------------------------------------------------*/
void CTCharacterRing::OnButtonDown(DWORD key)
{
	//SELECT�^�X�N�Q�b�g
	CCharacterSelectBase* ccselect = dynamic_cast<CCharacterSelectBase*>(g_system.GetCurrentMainTask());
	if(!ccselect)return;

	//����
	if(key & KEYSTA_BA2){//�uA�v����
		if(!IsSelected(GetSelected())){
			ccselect->OnSelect(this,GetSelected());
		}
	}
	else if(key & KEYSTA_BD2){//�����_������
		ccselect->OnSelect(this,-1);
	}
	else if(key & KEYSTA_BB2){//�~ on/off instruction
		//ccselect->OnInstOnOff(this);
		ccselect->OnCancel();	// �Վ�(�L�����Z��)
	}
	else if(key & KEYSTA_BC2){//change color
		ccselect->OnChangeColor(this);
	}
}

/*--------------------------------------------------------------------------------------
	�����O���܂킳��ăC���f�b�N�X���ω������Ƃ��̏���
----------------------------------------------------------------------------------------*/
void CTCharacterRing::OnChangeIndex()
{
	//��ɂ�邱�Ɩ���
}

/*--------------------------------------------------------------------------------------
	�����O���ύX���ꂽ�Ƃ��̏���
----------------------------------------------------------------------------------------*/
void CTCharacterRing::OnChangeRing()
{
	//��ɂ�邱�Ɩ���
}

/*--------------------------------------------------------------------------------------
	�����O�̑�����擾����
----------------------------------------------------------------------------------------*/
UINT CTCharacterRing::GetMaxRing()
{
	return g_charlist.GetRingNum();
}

/*--------------------------------------------------------------------------------------
	�w�胊���O������Ă���A�C�R���̐���擾
----------------------------------------------------------------------------------------*/
UINT CTCharacterRing::GetMaxIndex(UINT ridx)
{
	return g_charlist.GetCharacterCountRing(ridx);
}

/*--------------------------------------------------------------------------------------
	�`��
----------------------------------------------------------------------------------------*/
void CTCharacterRing::Draw()
{
	CTIconRingBase::Draw();

	// �I���Ă�Լ�̖��O��\��
	//g_system.DrawBMPText(m_radius_base, m_height, 0, g_charlist.GetCharacterDir( GetSelected() ), 0xFF4455AA);
}


/*=======================================================================================

	�f�Jface�\���N���X

=========================================================================================*/

/*-----------------------------------------------------------
	����
-------------------------------------------------------------*/
void CTCharacterBigFace::Initialize()
{
	int j;
	
	//�p�����[�^���Z�b�g
	m_counter  = 0;
	m_counter2 = 0;
	m_selected_num = 0;
	for(j=0;j<3;j++){
		m_cindex[j]=0;
		m_color[j]=0;
	}

	m_draw_temporary = TRUE;
}

/*-----------------------------------------------------------
	�j��
-------------------------------------------------------------*/
void CTCharacterBigFace::Terminate()
{
	//��ɂ�邱�Ɩ���
}

/*-----------------------------------------------------------
	��s
-------------------------------------------------------------*/
BOOL CTCharacterBigFace::Execute(DWORD time)
{
	m_counter+=10;//���莞��������J�E���^��Z
	if(m_counter>100)m_counter=100;

	m_counter2+=35;//�I��ύX���o���J�E���^��Z

	return TRUE;
}

/*-----------------------------------------------------------
	�`��
-------------------------------------------------------------*/
void CTCharacterBigFace::Draw()
{
	g_draw.EnableZ(FALSE,FALSE);

	//local val.
	int l;
	DWORD face2_maxx , face2_col;
	RECT r_face;
	MYSURFACE *pddstmp;

	//�`�搔����
	int kmax;
	if(m_draw_temporary)kmax= m_selected_num==m_max_num ? m_max_num : m_selected_num+1;
	else				kmax= m_max_num;

	for(int k=0;k<kmax;k++){
		l=m_selected_num-k;//���̂ق��ɉ�������鐔
		if(m_selected_num==m_max_num)l--;
		if(l==1)l=(int)(80*(m_counter/100.0));
		if(l==2)l=(int)(80+80*(m_counter/100.0));

		pddstmp = gbl.GetBigFace(m_cindex[k],m_color[k],m_alt[k]);
		if(pddstmp){
			face2_col = GetDispColor(k);
			
			r_face.top = 0;
			r_face.left = 0;
			r_face.right = (int)pddstmp->wg;
			r_face.bottom = (int)pddstmp->hg;

			if(k==m_selected_num){
				if(m_counter2 > pddstmp->wg)face2_maxx=(DWORD)pddstmp->wg;
				else face2_maxx = m_counter2;
			}
			else face2_maxx=(DWORD)pddstmp->wg;
			
			if(m_left){//player1
				g_draw.CheckBlt(
					pddstmp,
					face2_maxx-(DWORD)pddstmp->wg - (m_max_num-1)*50 +l,
					240-(DWORD)pddstmp->hg/2,
					r_face,
					FALSE,
					FALSE,
					0,
					0.02f + l*0.008f,
					face2_col);
			}
			else{//player2
				g_draw.CheckBlt(
					pddstmp,
					640-face2_maxx + (m_max_num-1)*80 - l,
					240-(DWORD)pddstmp->hg/2,
					r_face,
					TRUE,
					FALSE,
					0,
					0.02f + + l*0.008f,
					face2_col);
			}
		}
	}

	g_draw.EnableZ();

}

DWORD CTCharacterBigFace::GetDispColor(UINT idx)
{
	return idx==m_selected_num ? 0xFFFFFFFF : 0xFFAAAAAA;
}

/*-----------------------------------------------------------
	�L�����m��
-------------------------------------------------------------*/
void CTCharacterBigFace::Set(int cid,DWORD color,int alt)
{
	if(m_selected_num==m_max_num)return;

	/*
	MYSURFACE **tgt = &m_dds[m_selected_num];
	BOOL needReload = TRUE;
	char *filepath,*palpath;

	if(*tgt!=NULL){
		if(m_cindex[m_selected_num]==cid && m_color[m_selected_num]==color && m_alt[m_selected_num]==alt){
			//�������m����Ƀ��[�h����Ă���
			needReload = FALSE;
		}
	}
	if(cid<0){
		//�����_���w��
		if(*tgt){
			RELSURFACE(*tgt);
		}
		needReload = FALSE;
	}
	if(needReload){
		if(*tgt){
			RELSURFACE(*tgt);
		}
		filepath = new char[MAX_PATH];
		palpath = new char[MAX_PATH];
		char altstr[3]={'\0','\0'};
		if(alt!=0)altstr[0]='a'+alt-1;
		sprintf(filepath,"%s\\face2%s.bmp",g_charlist.GetCharacterDir(cid) ,altstr);
		sprintf(palpath,"%s\\pal%d.bmp",g_charlist.GetCharacterDir(cid),color);
		*tgt = g_draw.CreateSurfaceFrom256BMP(filepath,palpath);
		delete [] filepath;
		delete [] palpath;
	}*/
	m_cindex[m_selected_num] = cid ;
	m_color[m_selected_num] = color;
	m_alt[m_selected_num] = alt;
	m_selected_num ++;
	if(m_selected_num!=m_max_num)m_counter = 0;
	m_counter2 = 0;
}

/*-----------------------------------------------------------
	���I��L������ݒ�
-------------------------------------------------------------*/
void CTCharacterBigFace::SetTemporary(int cid,DWORD color,int alt)
{
	if(m_selected_num==m_max_num)return;

	if(m_cindex[m_selected_num]==cid && m_color[m_selected_num]==color && m_alt[m_selected_num]==alt){
		//�������m����Ƀ��[�h����Ă���E�E�E�n�Y
		return;
	}

	if(cid<0){//�����_���w��
		m_cindex[m_selected_num] = cid ;
		m_color[m_selected_num] = color;
		m_alt[m_selected_num] = alt;
		return;
	}
/*
	MYSURFACE **tgt = &m_dds[m_selected_num];
	char *filepath,*palpath;

	if(m_cindex[m_selected_num]==cid && m_color[m_selected_num]==color && m_alt[m_selected_num]==alt){
		//�������m����Ƀ��[�h����Ă���E�E�E�n�Y
		return;
	}
	if(*tgt){
		RELSURFACE(*tgt);
	}
	if(cid<0){//�����_���w��
		m_cindex[m_selected_num] = cid ;
		m_color[m_selected_num] = color;
		return;
	}
	filepath = new char[MAX_PATH];
	palpath = new char[MAX_PATH];
	char altstr[3]={'\0','\0'};
	if(alt!=0)altstr[0]='a'+alt-1;
	sprintf(filepath,"%s\\face2%s.bmp",g_charlist.GetCharacterDir(cid),altstr );
	sprintf(palpath,"%s\\pal%d.bmp",g_charlist.GetCharacterDir(cid),color);
	*tgt = g_draw.CreateSurfaceFrom256BMP(filepath,palpath);
	delete [] filepath;
	delete [] palpath;
	*/
	m_cindex[m_selected_num] = cid ;
	m_color[m_selected_num] = color;
	m_alt[m_selected_num] = alt;

	m_counter2 = 0;
}

/*=======================================================================================

	�w�i�`��

=========================================================================================*/


/*-----------------------------------------------------------
	����
-------------------------------------------------------------*/
void CTCharacterSelectBG::Initialize()
{
	//�e�N�X�`�����[�h
	D3DXCreateTextureFromFileEx(g_draw.d3ddev,
		"system\\texture\\tex_cs1.png",
		0,0,
		1,0,
		g_draw.texformat,
		D3DPOOL_MANAGED,
		D3DX_FILTER_POINT,
		D3DX_DEFAULT,
		0x000000,
		NULL,
		NULL,
		&ptex_cs1);

	//���_��W�ݒ�
	vbg[0].x = -0.3f;//-(ASPECTRATIO);
	vbg[0].y = -0.2f;//-(1.0f);
	vbg[0].z = 0.2f;
	vbg[1].x = -0.3f;//-(ASPECTRATIO);
	vbg[1].y = (1.0f)*2.2f;
	vbg[1].z = 0.2f;
	vbg[2].x = (ASPECTRATIO*2.2f);
	vbg[2].y = -0.2f;//-(1.0f);
	vbg[2].z = 0.2f;
	vbg[3].x = (ASPECTRATIO)*2.2f;
	vbg[3].y = (1.0f)*2.2f;
	vbg[3].z = 0.2f;

	//�p�����[�^���Z�b�g
	m_state = FALSE;
	m_counter = 0;
}


/*-----------------------------------------------------------
	�j��
-------------------------------------------------------------*/
void CTCharacterSelectBG::Terminate()
{
	RELEASE(ptex_cs1);//�e�N�X�`����n��
}


/*-----------------------------------------------------------
	��s
-------------------------------------------------------------*/
BOOL CTCharacterSelectBG::Execute(DWORD time)
{
	if(!ptex_cs1)return FALSE;

	BYTE bgcolor;//0:�L�����Z���F�@255:�X�e�[�W�Z���N�g�F
	double ratio;
	if(m_state){
		ratio = m_counter/60.0/4.0;
		ratio = 1.0 - ratio;
	}
	else{
		ratio = m_counter/60.0/4.0;
	}
	bgcolor = (BYTE)(255*ratio);

	//�J���[�ݒ�
	vbg[0].color = 0xFF0000FF;
	vbg[0].color |= bgcolor*256*256 + bgcolor*256;
	vbg[1].color = vbg[2].color = 0xFF000000;
	vbg[1].color |= bgcolor*256*256 + bgcolor*256 + bgcolor;
	vbg[2].color |= bgcolor*256*256 + bgcolor*256 + bgcolor;
	vbg[3].color = 0xFFFF0000;
	vbg[3].color |= bgcolor*256 + bgcolor;

	//�e�N�X�`��UV�ݒ�
	float texzurasi= (float)(timeGetTime()%5000) / 5000.0f;
	vbg[0].tu = texzurasi;
	vbg[0].tv = texzurasi;
	vbg[1].tu = texzurasi;
	vbg[1].tv = texzurasi+7.5f;
	vbg[2].tu = texzurasi+5.0f;
	vbg[2].tv = texzurasi;
	vbg[3].tu = texzurasi+5.0f;
	vbg[3].tv = texzurasi+7.5f;

	if(m_counter!=0)m_counter--;
	return TRUE;
}


/*-----------------------------------------------------------
	�`��
-------------------------------------------------------------*/
void CTCharacterSelectBG::Draw()
{
	g_draw.SetTransform(FALSE);
	g_draw.EnableZ(FALSE,FALSE);
	
	g_draw.d3ddev->SetTexture(0,ptex_cs1);
	g_draw.d3ddev->SetVertexShader(FVF_3DVERTEX);
	g_draw.d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSU,D3DTADDRESS_WRAP);
	g_draw.d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSV,D3DTADDRESS_WRAP);
	g_draw.d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,
		2,vbg,sizeof(MYVERTEX3D));
	g_draw.d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSU,D3DTADDRESS_CLAMP);
	g_draw.d3ddev->SetTextureStageState(0,D3DTSS_ADDRESSV,D3DTADDRESS_CLAMP);

	g_draw.EnableZ();
}


/*-----------------------------------------------------------
	���[�h�ύX
-------------------------------------------------------------*/
void CTCharacterSelectBG::ChangeState(BOOL f)
{
	m_state = f;
	m_counter = 60*4 - m_counter;
}



/*=======================================================================================

	�����O�w�i�I�r�\���N���X

=========================================================================================*/


/*-----------------------------------------------------------
	����
-------------------------------------------------------------*/
void CTCharacterSelectBelt::Initialize()
{
	m_state = CTCSBS_NotReady;
	m_ratio = 0;
	m_height_base = 110.0f;

	UpdateText();

	m_txtTop	= m_pos==0 ? TRUE : FALSE;		//�e�L�X�g���ɔz�u���邩�A���ɔz�u���邩
	m_txtLeft	= m_pos==0 ? 50.0f : 590.0f;	//�e�L�X�g�̍��ʒu
	m_txtR2L	= m_pos==0 ? FALSE : TRUE;		//�e�L�X�g�E�����`��

	m_alpha1	= 230;
	m_alpha2	= 20;									//���A�Z���Ƃ��E�����Ƃ�
	m_lineCol	= m_pos==0 ? 0x002200FF : 0x00FF0022;	//���C���̃J���[�i���ȊO�j
	m_bodyCol	= 0x00FFFFFF;							//�{�̂̃J���[�i���ȊO�j
	m_txtCol1	= m_pos==0 ? 0x77220099 : 0x77990022;
	m_txtCol2	= m_pos==0 ? 0xFF8888FF : 0xFFFF8888;	//�e�L�X�g�J���[�i���܁j

	m_lineWidth = 2.0f;			//�F�����C������
}


/*-----------------------------------------------------------
	�j��
-------------------------------------------------------------*/
void CTCharacterSelectBelt::Terminate()
{
}


/*-----------------------------------------------------------
	��s
-------------------------------------------------------------*/
BOOL CTCharacterSelectBelt::Execute(DWORD time)
{
	float animSpd = 0.05f;

	switch(m_state){
	case CTCSBS_NotReady:
		{
			m_ratio += animSpd;
			if(m_ratio > 1.0f){
				m_ratio = 1.0f;
				m_state = CTCSBS_Ready;
			}
			CalcTopBottom();
			m_show_text = TRUE;
		}break;
	case CTCSBS_Ready:
		{
			//no action
		}break;
	case CTCSBS_Change:
		{
			m_ratio -= animSpd*2.0f;
			if(m_ratio < 0.0f){
				m_ratio = 0.0f;
				m_state = CTCSBS_Change2;
			}
			CalcTopBottom();
			m_show_text = TRUE;
		}break;
	case CTCSBS_Change2:
		{
			m_ratio += animSpd*2.0f;
			if(m_ratio > 1.0f){
				m_ratio = 1.0f;
				m_state = CTCSBS_Ready;
			}
			CalcTopBottom();
			m_show_text = TRUE;
			UpdateText();
		}break;
	case CTCSBS_Hide:
		{
			m_ratio -= animSpd;
			if(m_ratio < 0.0f){
				m_ratio = 0.0f;
				m_state = CTCSBS_HideComplete;
			}
			CalcTopBottom();
			m_show_text = TRUE;
		}break;
	case CTCSBS_Show:
		{
			m_ratio += animSpd;
			if(m_ratio > 1.0f){
				m_ratio = 1.0f;
				m_state = CTCSBS_Ready;
			}
			CalcTopBottom();
			m_show_text = TRUE;
		}break;
	case CTCSBS_HideComplete:
		{
			//no action
		}break;
	case CTCSBS_Extend:
		{
			m_ratio -= animSpd*2.0f;
			if(m_ratio < 0.0f){
				m_ratio = 0.0f;
				m_state = CTCSBS_Extend2;
			}
			CalcTopBottom();
			m_show_text = TRUE;
		}break;
	case CTCSBS_Extend2:
		{
			m_ratio += animSpd*2.0f;
			if(m_ratio > 1.0f){
				m_ratio = 1.0f;
				m_state = CTCSBS_Extended;
			}
			m_top    = m_base_y + (m_given_top-m_base_y) * m_ratio;
			m_bottom = m_base_y + (m_given_bottom-m_base_y) * m_ratio;
			m_show_text = FALSE;
		}break;
	case CTCSBS_Extended:
		{
			//no action
		}break;
	case CTCSBS_ExtendedHide:
		{
			m_ratio -= animSpd*2.0f;
			if(m_ratio < 0.0f){
				m_ratio = 0.0f;
				m_state = CTCSBS_HideComplete;
			}
			m_top    = m_base_y + (m_given_top-m_base_y) * m_ratio;
			m_bottom = m_base_y + (m_given_bottom-m_base_y) * m_ratio;
			m_show_text = FALSE;
		}break;
	case CTCSBS_Restore:
		{
			m_ratio -= animSpd*2.0f;
			if(m_ratio < 0.0f){
				m_ratio = 0.0f;
				m_state = CTCSBS_Change2;
			}
			m_top    = m_base_y + (m_given_top-m_base_y) * m_ratio;
			m_bottom = m_base_y + (m_given_bottom-m_base_y) * m_ratio;
			m_show_text = FALSE;
		}break;
	}

	return TRUE;
}

void CTCharacterSelectBelt::UpdateText()
{
	if(m_com){
		if(m_current_key>=0){
			sprintf(m_disp_str,"COM / Player%d",m_current_key+1);
		}
		else{
			sprintf(m_disp_str,"COM / Wait",m_current_key+1);
		}
	}
	else{
		sprintf(m_disp_str,"Player%d",m_current_key+1);
	}

	sprintf(&m_disp_str[strlen(m_disp_str)],": %s",g_charlist.GetRingName(m_ringIndex));
}

/*-----------------------------------------------------------
	���̑� ����
-------------------------------------------------------------*/
void CTCharacterSelectBelt::SetRing(DWORD ringIndex)
{
	if(m_ringIndex==ringIndex)return;

	m_ringIndex = ringIndex;
	m_state = CTCSBS_Change;
}

void CTCharacterSelectBelt::SetCom(BOOL com)
{
	m_com = com;
}

void CTCharacterSelectBelt::SetKey(char idx)
{
	char prev_key = m_current_key;
	m_current_key = idx;

	if(prev_key != m_current_key){
		UpdateText();
	}
}

void CTCharacterSelectBelt::Show(BOOL b)
{
	if(b){
		m_state = CTCSBS_Show;
	}
	else{
		if(m_state==CTCSBS_Extend2 || m_state==CTCSBS_Extended){
			m_state = CTCSBS_ExtendedHide;
		}
		else m_state = CTCSBS_Hide;
	}
}


void CTCharacterSelectBelt::Extend(float top,float bottom)
{
	m_state = CTCSBS_Extend;
	m_given_top = top;
	m_given_bottom = bottom;
}

void CTCharacterSelectBelt::Restore()
{
	m_state = CTCSBS_Restore;
}

/*=======================================================================================

	�I�v�V�����I��N���X

=========================================================================================*/
void CTOptionSelecter::EndSelect()
{
	CCharacterSelect *csel = dynamic_cast<CCharacterSelect*>( g_system.GetCurrentMainTask() );
	if(csel){
		csel->OnOptionSelect(this,m_selecter->GetSettings());
	}
}

/*=======================================================================================

	�X�e�[�W�I��N���X

=========================================================================================*/

DWORD CTStageSelecter::m_previous_selected_ring  =0;
DWORD CTStageSelecter::m_previous_selected_index =0;

/*-----------------------------------------------------------
	����
-------------------------------------------------------------*/
void CTStageSelecter::Initialize()
{
	m_state = CTSSS_NotReady;
	m_counter = 0;
	m_dds = NULL;

	m_selected_ring  = m_previous_selected_ring;
	m_selected_index = m_previous_selected_index;
	
	LoadBMP();

	//�I�r�p�����[�^����
	m_txtTop	= TRUE;				//�e�L�X�g���ɔz�u���邩�A���ɔz�u���邩
	m_txtLeft	= 50.0f;			//�e�L�X�g�̍��ʒu
	m_txtR2L	= FALSE;			//�e�L�X�g�E�����`��
	m_alpha1	= 230;
	m_alpha2	= 150;				//���A�Z���Ƃ��E�����Ƃ�
	m_lineCol	= 0x00555555;		//���C���̃J���[�i���ȊO�j
	m_bodyCol	= 0x00FFFFFF;		//�{�̂̃J���[�i���ȊO�j
	m_txtCol1	= 0xFF555555;
	m_txtCol2	= 0xFF000000;		//�e�L�X�g�J���[�i���܁j
	m_lineWidth = 2.0f;				//�F�����C������
	m_base_y = 240.0f;
	m_height_base = 380.0f;

	UpdateText();
}


/*-----------------------------------------------------------
	�j��
-------------------------------------------------------------*/
void CTStageSelecter::Terminate()
{
	if(m_dds){
		RELSURFACE(m_dds);
	}
}


/*-----------------------------------------------------------
	�X�V
-------------------------------------------------------------*/
BOOL CTStageSelecter::Execute(DWORD time)
{
	CTStageSelecterState prv_state = m_state;
	float tick;

	const float height_max = 0.7f;
	const float width_max = 1.0f;
	const float height_min = 0.01f;

	switch(m_state)
	{
	case CTSSS_NotReady:
		{
			tick = m_counter*0.05f;
			if(tick < 0.5f){
				m_width  = width_max * tick*2.0f;
				m_height = height_min;
			}
			else{
				m_width  = width_max;
				m_height = height_max * (  (tick-0.5f)*2.0f );
			}
			if(tick >= 1.0f){
				m_state = CTSSS_Ready;
				tick = 1.0f;
			}
			m_ratio = tick;
		}break;
	case CTSSS_Ready:
		{
			HandlePad();
			tick = 1.0f;
			m_ratio = tick;
		}break;
	case CTSSS_Change:
		{
			tick = m_counter*0.05f;
			if(tick < 0.5f){
				m_width  = width_max * tick*2.0f;
				m_height = height_min;
			}
			else{
				m_width  = width_max;
				m_height = height_max * (  (tick-0.5f)*2.0f );
			}
			if(tick >= 1.0f){
				m_state = CTSSS_Ready;
				tick = 1.0f;
			}
		}break;
	case CTSSS_Change2:
		{
			tick = m_counter*0.05f;
			if(tick < 0.5f){
				m_width  = width_max * tick*2.0f;
				m_height = height_min;
			}
			else{
				m_width  = width_max;
				m_height = height_max * (  (tick-0.5f)*2.0f );
			}
			if(tick >= 1.0f){
				m_state = CTSSS_Ready;
				tick = 1.0f;
			}
		}break;
	case CTSSS_Hide:
		{
			tick = 1.0f - m_counter*0.035f;
			if(tick < 0.5f){
				m_width  = width_max * tick*2.0f;
				m_height = height_min;
			}
			else{
				m_width  = width_max;
				m_height = height_max * (  (tick-0.5f)*2.0f );
			}
			if(tick<0.0f){
				m_state = CTSSS_HideComplete;
				CCharacterSelectBase* ccselect = dynamic_cast<CCharacterSelectBase*>(g_system.GetCurrentMainTask());
				if(ccselect){
					ccselect->OnEndAnimComplete(this);
				}
				tick = 0.0f;
			}
			m_ratio = tick;
		}break;
	case CTSSS_HideComplete:
		{
			return FALSE;
			tick = 0.0f;
			m_ratio = tick;
		}break;
	}

	if(m_state != prv_state){
		m_counter = 0;
	}
	else m_counter++;
	return TRUE;
}


/*-----------------------------------------------------------
	�`��
-------------------------------------------------------------*/
void CTStageSelecter::Draw()
{
	//�I�r�E�^�C�g���`��
	CTBeltBase::CalcTopBottom();
	CTBeltBase::Draw();

	if(m_dds)//�v���r���[�`��
	{
		RECT rsrc;
		MYRECT3D rdst;
		rsrc.top=rsrc.left=0;
		rsrc.right =(LONG)m_dds->wg;
		rsrc.bottom=(LONG)m_dds->hg;
		rdst.z = 0.0f;
		rdst.left  = (1.0f -0.5f*m_width)*ASPECTRATIO ;
		rdst.right = (1.0f +0.5f*m_width)*ASPECTRATIO ;
		rdst.top   = 1.0f -0.5f*m_height;
		rdst.bottom= 1.0f +0.5f*m_height;
		g_draw.MyBlt3D(m_dds,rsrc,rdst);
	}

	// �X�e�[�W����\��
	if (m_state != CTSSS_NotReady && m_state != CTSSS_Hide)
	{
		RECT r;
		r.left = r.top =0;
		r.right = 640;
		r.bottom = 480 - 90;
		g_draw.DrawBlueText(r, g_stagelist.GetStageName( GetSelection() ), -1, DT_CENTER | DT_BOTTOM, 3);
	}
}



/*-----------------------------------------------------------
	�v���r���[�̓ǂݒ���
-------------------------------------------------------------*/
void CTStageSelecter::LoadBMP()
{
	if(m_dds){
		RELSURFACE(m_dds);
	}

	char *filepath = new char[MAX_PATH];
	sprintf(filepath,"%s\\preview",g_stagelist.GetStageDir( GetSelection() ) );
	m_dds = g_draw.CreateSurfaceFrom256Image(filepath);
	delete [] filepath;
}


/*-----------------------------------------------------------
	�e�L�X�g�X�V
-------------------------------------------------------------*/
void CTStageSelecter::UpdateText()
{
	sprintf(m_disp_str,"%s",
		g_stagelist.GetStageDir( GetSelection() )
		);

	for(UINT i=0;i<strlen(m_disp_str);i++){
		if(m_disp_str[i]=='\\'){
			m_disp_str[i]='/';//������̂ق���������悳��
		}
	}
}


/*-----------------------------------------------------------
	�L�[��͏���
-------------------------------------------------------------*/
void CTStageSelecter::HandlePad()
{
	//SELECT�^�X�N�Q�b�g
	CCharacterSelectBase* ccselect = dynamic_cast<CCharacterSelectBase*>(g_system.GetCurrentMainTask());
	if(!ccselect)return;

	//�L�[��͎擾
	DWORD keystate=0;
	std::vector<DWORD>::iterator ite=m_keyIndexList.begin();
	std::vector<DWORD>::iterator itee=m_keyIndexList.end();
	for(;ite!=itee;ite++){
		keystate |= g_input.GetKey( *ite ,0);
	}
	
	DWORD prvring = m_selected_ring;
	DWORD prvindx = m_selected_index;

	//����
	if(keystate & KEYSTA_BA2){//�uA�v����
		ccselect->OnStageSelect(this,GetSelection());
		m_state = CTSSS_Hide;
	}
	else if(keystate & KEYSTA_BD2){//�����_������
		ccselect->OnStageSelect(this,-1);
		m_state = CTSSS_Hide;
	}
	//���E��
	else if(keystate & KEYSTA_ARIGHT2){//LEFT2){
		if(m_selected_index==0)m_selected_index =g_stagelist.GetStageCountRing(m_selected_ring)-1;
		else m_selected_index--;
		m_state = CTSSS_Change;
		m_counter = 0;
	}
	else if(keystate & KEYSTA_ALEFT2){//RIGHT2){
		m_selected_index++;
		if((int)m_selected_index >= g_stagelist.GetStageCountRing(m_selected_ring))
			m_selected_index=0;
		m_state = CTSSS_Change;
		m_counter = 0;
	}
	//���E��(�����O�ύX)
	else if( keystate & KEYSTA_UP2 ){
		if(m_selected_ring==0)m_selected_ring=g_stagelist.GetRingNum()-1;
		else m_selected_ring--;
		while(g_stagelist.GetStageCountRing(m_selected_ring)==0){
			if(m_selected_ring==0)m_selected_ring=g_stagelist.GetRingNum()-1;
			else m_selected_ring--;
		}
		if(g_stagelist.GetStageCountRing(m_selected_ring)-1 < (int)m_selected_index)
			m_selected_index = g_stagelist.GetStageCountRing(m_selected_ring)-1 ;
		m_state = CTSSS_Change2;
	}
	else if( keystate & KEYSTA_DOWN2 ){
		m_selected_ring++;
		if((int)m_selected_ring > g_stagelist.GetRingNum()-1)m_selected_ring=0;
		while(g_stagelist.GetStageCountRing(m_selected_ring)==0){
			m_selected_ring++;
			if((int)m_selected_ring>g_stagelist.GetRingNum()-1)m_selected_ring=0;
		}
		if(g_stagelist.GetStageCountRing(m_selected_ring)-1 < (int)m_selected_index)
			m_selected_index = g_stagelist.GetStageCountRing(m_selected_ring)-1 ;
		m_state = CTSSS_Change2;
	}
	
	if( prvring != m_selected_ring || prvindx != m_selected_index )
	{
		LoadBMP();
		UpdateText();
	}
}


//�����O+�C���f�b�N�X�@���@�S�̂ł̃C���f�b�N�X
int CTStageSelecter::GetSelection()
{
	return g_stagelist.Ring2Serial( m_selected_ring , m_selected_index );
}





/*=======================================================================================

	�ΐ�E��{�ݒ�ݒ�^�X�N

=========================================================================================*/

BYTE CTConditionSelecter::m_type = TAISENKEISIKI_GOCYAMAZE;

BYTE CTConditionSelecter::m_assign[2][MAXNUM_TEAM] = 
	{
		{0,			  CASSIGN_NONE,CASSIGN_NONE},
		{CASSIGN_COM, CASSIGN_NONE,CASSIGN_NONE}
	};

int CTConditionSelecter::m_limit_time_index = 1;
int CTConditionSelecter::m_limit_time[5] = {40,60,99,120,-1};

/*---------------------------------------------------------------------------------------
	����
-----------------------------------------------------------------------------------------*/
void CTConditionSelecter::Initialize()
{
	Check();

	m_state = CTCoS_Start;
	m_counter = 0;
	m_selected = MAXNUM_TEAM*2;
	m_ratio = 0.0f;

	//�I�r�p�����[�^����
	m_txtTop	= TRUE;				//�e�L�X�g���ɔz�u���邩�A���ɔz�u���邩
	m_txtLeft	= 50.0f;			//�e�L�X�g�̍��ʒu
	m_txtR2L	= FALSE;			//�e�L�X�g�E�����`��
	m_alpha1	= 230;
	m_alpha2	= 150;				//���A�Z���Ƃ��E�����Ƃ�
	m_lineCol	= 0x00555555;		//���C���̃J���[�i���ȊO�j
	m_bodyCol	= 0x00FFFFFF;		//�{�̂̃J���[�i���ȊO�j
	m_txtCol1	= 0xFF555555;
	m_txtCol2	= 0xFF000000;		//�e�L�X�g�J���[�i���܁j
	m_lineWidth = 2.0f;				//�F�����C������
	m_base_y = 240.0f;
	m_height_base = 380.0f;
	sprintf(m_disp_str,"SETTINGS");

	for(int j=0;j<2;j++){
		for(int i=0;i<MAXNUM_TEAM;i++)
		{
			// Stable�͎g��Ȃ�
			if (m_assign[j][i] == CASSIGN_STABLE)
				m_assign[j][i] = CASSIGN_COM;
		}
	}
}


/*---------------------------------------------------------------------------------------
	�I������
-----------------------------------------------------------------------------------------*/
void CTConditionSelecter::Terminate()
{
	CTBeltBase::Terminate();
}


/*---------------------------------------------------------------------------------------
	�X�V
-----------------------------------------------------------------------------------------*/
BOOL CTConditionSelecter::Execute(DWORD time)
{
	CTConditionSelecter_State pstate=m_state;
	const int select_max = MAXNUM_TEAM*2+3;

	switch(m_state)
	{
	case CTCoS_Start://�o����
		{
			float tick = (float)m_counter/20.0f;
			if(tick>1.0f){
				tick=1.0f;
				m_state = CTCoS_Execute;
			}
			m_ratio = tick;
		}break;
	case CTCoS_Execute://��s��
		{
			m_ratio = 1.0f;

			DWORD key = g_input.GetAllKey();
			if(m_selected==select_max-1 && (key&KEYSTA_ALEFT2 || key&KEYSTA_BA2) && m_ok){//�P�e�[�C
				CCharacterSelect* ccselect = dynamic_cast<CCharacterSelect*>(g_system.GetCurrentMainTask());
				ccselect->OnConditionDecided(this);
				m_state = CTCoS_Hide;
			}
			else if( (m_selected==select_max && (key&KEYSTA_ALEFT2 || key&KEYSTA_BA2)) ||
					  key&KEYSTA_BD2 ){//�^�C�g���ɖ߂�
				g_system.ReturnTitle();
				return FALSE;
			}
			else if(key&KEYSTA_ALEFT2 || key&KEYSTA_BA2){//�ύX
				Change(FALSE);
			}
			else if(key&KEYSTA_ARIGHT2 || key&KEYSTA_BB2){//�ύX
				Change(TRUE);
			}
			else if(key&KEYSTA_DOWN2){//�ړ�
				m_selected++;
			}
			else if(key&KEYSTA_UP2){//�ړ�
				m_selected--;
			}
			m_selected = (m_selected+select_max+1) % (select_max+1);
		}break;
	case CTCoS_Hide://�����
		{
			float tick = (float)m_counter/20.0f;
			if(tick>1.0f){
				tick=1.0f;
				m_state = CTCoS_HideComplete;
			}
			m_ratio = 1.0f-tick;
		}break;
	case CTCoS_HideComplete://�������
		{
			m_ratio = 0.0f;
		}break;
	}

	m_counter++;
	if(pstate!=m_state){
		m_counter=0;
	}
	return TRUE;
}

//�ݒ�ύX
void CTConditionSelecter::Change(BOOL key)
{
	//�ΐ�`���ύX
	if(m_selected==0)
	{
		m_type += key ? 1 : -1;
		m_type = (m_type+3-1)%3 +1;
		Check();
		return;
	}
	//������ԕύX
	else if(m_selected==1 && m_type==TAISENKEISIKI_GOCYAMAZE)
	{
		m_limit_time_index += key ? 1 : -1;
		m_limit_time_index = (m_limit_time_index+5)%5 ;
		Check();
		return;
	}

	int sel1 = (m_selected-2)/MAXNUM_TEAM;
	int sel2 = (m_selected-2)%MAXNUM_TEAM;

	if(m_assign[sel1][sel2]&CASSIGN_SPECIFIC){
		switch(m_assign[sel1][sel2]){
		case CASSIGN_COM:
			m_assign[sel1][sel2] = key ? CASSIGN_NONE : MAXNUM_KEYI-1 ;
			break;
		case CASSIGN_NONE:
			m_assign[sel1][sel2] = key ? 0 : CASSIGN_COM ;
			break;
		}
	}

	else{
		//�ʏ�͈͂𒴂��Ă��܂��ꍇ
		if(m_assign[sel1][sel2]==0 && !key){
			m_assign[sel1][sel2] = CASSIGN_NONE;
		}
		else if(m_assign[sel1][sel2]==MAXNUM_KEYI-1 && key){
			m_assign[sel1][sel2] = CASSIGN_COM;
		}
		//����
		else{
			m_assign[sel1][sel2] += key ? 1 : -1;
		}
	}

	Check();
}


/*---------------------------------------------------------------------------------------
	�`��
-----------------------------------------------------------------------------------------*/
void CTConditionSelecter::Draw()
{
	CTBeltBase::CalcTopBottom();
	CTBeltBase::Draw();//�I�r�E�^�C�g���`��

	const float x=100;					//�\�����ʒu
	const float txt_xr=0.65f;			//�e�L�X�g�Ey�g�嗦
	const float txt_yr=0.65f*m_ratio;	//�e�L�X�g�Ex�g�嗦
	float y = 100.0f;
	const float ystep = 24.0f;

	int i,j;

	if(m_ratio<0.01f)return;

	char *tstr = new char[64];

	//�ΐ�`��
	char *typenames[]={
		"Cooperation",
		"Marvel Like",
		"K.O.F. Like"
	};
	sprintf(tstr,"Battle Type : %s",typenames[m_type-1]);
	g_system.DrawBMPTextEx(x,y,0.0f,
				tstr,
				TxtCol(0),txt_xr,txt_yr,SYSBMPTXT_PROP);
	y += ystep;
	//�������
	if(m_type==TAISENKEISIKI_GOCYAMAZE){
		if(m_limit_time[m_limit_time_index]>0)
			sprintf(tstr,"Limit Time : %d",m_limit_time[m_limit_time_index]);
		else
			sprintf(tstr,"Limit Time : -infinity-");
	}
	else
		sprintf(tstr,"Limit Time : -no use-");
	g_system.DrawBMPTextEx(x,y,0.0f,
				tstr,
				TxtCol(1),txt_xr,txt_yr,SYSBMPTXT_PROP);
	y += ystep;
	y += ystep;

	//TEAM*-x : �`
	for(j=0;j<2;j++){
		for(i=0;i<MAXNUM_TEAM;i++)
		{
			sprintf(tstr,"TEAM%d-%d : ",j+1,i+1);
			if(m_assign[j][i]&CASSIGN_SPECIFIC)//���
			{
				switch(m_assign[j][i]){
				case CASSIGN_COM:	sprintf(&tstr[strlen(tstr)],"Computer");break;
				case CASSIGN_NONE:	sprintf(&tstr[strlen(tstr)],"--None--");break;
				default:			sprintf(&tstr[strlen(tstr)],"Error?");
				}
			}
			else{
				sprintf(&tstr[strlen(tstr)],"Player%d",m_assign[j][i]+1);
			}
			g_system.DrawBMPTextEx(x,y,0.0f,
				tstr,TxtCol(j*MAXNUM_TEAM+i+2),txt_xr,txt_yr,SYSBMPTXT_PROP);
			y += ystep;
		}
		y += ystep;
	}

	//"OK"�`��
	g_system.DrawBMPTextEx(640.0f-x,y,0.0f,
				m_ok ? "OK" : m_str_error,
				TxtCol(2*MAXNUM_TEAM+2),txt_xr,txt_yr,SYSBMPTXT_PROP|SYSBMPTXT_R2L);
	y += ystep;
	//"return title"
	g_system.DrawBMPTextEx(640.0f-x,y,0.0f,
				"Return Title",
				TxtCol(2*MAXNUM_TEAM+3),txt_xr,txt_yr,SYSBMPTXT_PROP|SYSBMPTXT_R2L);

	delete [] tstr;
}


/*---------------------------------------------------------------------------------------
	�ݒ�̐������`�F�b�N
-----------------------------------------------------------------------------------------*/
void CTConditionSelecter::Check()
{
	m_ok=TRUE;
	int i,j;

	//���ׂ�NONE�ɂȂ�ĂȂ����H
	BOOL ok;
	for(i=0;i<2;i++){
		ok=FALSE;
		for(j=0;j<MAXNUM_TEAM;j++){
			if(m_assign[i][j]!=CASSIGN_NONE){
				ok=TRUE;
			}
		}
		if(!ok){
			sprintf(m_str_error,"Error , All None");
			m_ok=FALSE;
			return;
		}
	}

	if(m_type==TAISENKEISIKI_GOCYAMAZE){
		//�v���C���[���d�����Ă��Ȃ����H
		int cnt;
		for(int k=0;k<MAXNUM_KEYI;k++)
		{
			cnt=0;
			for(i=0;i<2;i++){
				for(j=0;j<MAXNUM_TEAM;j++){
					if(m_assign[i][j]==k)cnt++;
				}
			}
			if(cnt>1){
				sprintf(m_str_error,"Error , Player%d Duplication",k+1);
				m_ok=FALSE;
				return;
			}
		}
	}
	else{
		//1�v���C���[��2�̃`�[���ɂ܂�����Ĕz�u����Ă��Ȃ����H
		BOOL belong[2];
		for(int k=0;k<MAXNUM_KEYI;k++)
		{
			belong[0]=belong[1]=FALSE;
			for(i=0;i<2;i++){
				for(j=0;j<MAXNUM_TEAM;j++){
					if(m_assign[i][j]==k)belong[i]=TRUE;
				}
			}
			if(belong[0] && belong[1]){
				sprintf(m_str_error,"Error , Player%d is Betrayer",k+1);//���؂�҃n�P�[��
				m_ok=FALSE;
				return;
			}
		}
	}
}








