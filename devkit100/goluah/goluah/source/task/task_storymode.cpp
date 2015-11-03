/*===============================================================

	�X�g�[���[���[�h�^�X�N

=================================================================*/

#include "stdafx.h"

#include "global.h"
#include "task_storymode.h"
#include "task_win.h"
#include "task_battle.h"
#include "task_staffroll.h"
#include "task_demo.h"


/*===============================================================

	�X�g�[���[���[�h�{��

=================================================================*/

extern int story_selectflag;		// �b��ݒu�A���̒u�����script_story.cpp�B

/*----------------------------------------------------------------
	�\�z
------------------------------------------------------------------*/
CTaskStoryMode::CTaskStoryMode()
{
	m_setting = NULL;
}

/*----------------------------------------------------------------
	�폜
------------------------------------------------------------------*/
CTaskStoryMode::~CTaskStoryMode()
{
	SAFEDELETE(m_setting);
	CGoluahStoryScript::DestroyScriptElementList(scr_list);
}

/*----------------------------------------------------------------
	�J�n���ݒ�
------------------------------------------------------------------*/
void CTaskStoryMode::Setup(UINT storyIndex,UINT keyindex,CStoryList::CStoryInfo* info)
{
	SAFEDELETE(m_setting);
	m_setting = info->Clone();

	for(int i=0;i<m_setting->cnum;i++)
	{
		//�r���E�ˑ��I�v�V�����ŁA�s���ɂȂ��Ă�����̂𐳂�
		g_charlist.CorrectOption( m_setting->characters[i],&m_setting->option[i] );
	}

	m_story_index = storyIndex;
	m_keyindex = keyindex;
	in_select = false;
}

/*----------------------------------------------------------------
	������
------------------------------------------------------------------*/
void CTaskStoryMode::Initialize()
{
	//�X�N���v�g�̓ǂݍ���(entry.txt�͎����ł������炵��)
//	char *filename = new char[MAX_PATH];
//	sprintf(filename,"%s\\entry.txt",g_storylist.GetStoryDir(m_story_index));
	BOOL ret = CGoluahStoryScript::CreateScriptElementList(
											scr_list,
											g_storylist.GetStoryDir(m_story_index));//filename);
//	delete [] filename;
	if(!ret){
		g_system.ReturnTitle();
		return;
	}

	//vs�u���b�N�ŁA�L�����N�^�[�̃I�v�V�����̕s���𒼂�
	CStoryElement_VS	*ele_vs;
	SScriptElementList::iterator i =scr_list.begin();
	SScriptElementList::iterator ie=scr_list.begin();
	for(;i!=ie;i++){
		ele_vs = dynamic_cast<CStoryElement_VS*>( *i );
		if(ele_vs){
			for(int t=0;t<2;t++){
				for(int j=0;j<ele_vs->num[t];j++){
					if(ele_vs->characters[t][j]>=0 && !ele_vs->is_random_opt[t][j])
					{
						g_charlist.CorrectOption(
										ele_vs->characters[t][j],
										&ele_vs->option[t][j]);
					}
				}
			}
		}
	}

	scr_i = scr_list.begin();

	//�R���e�B�j���[�񐔂��E��
	CStoryElement_Settings* pset=dynamic_cast<CStoryElement_Settings*>(*scr_i);
	if(!pset){
		g_system.ReturnTitle();
		return;
	}
	m_continue_num = pset->continue_num;

	//setting�u���b�N���΂�
	while(dynamic_cast<CStoryElement_Settings*>(*scr_i))scr_i++;

	//�����_���L�����N�^�[�E�X�e�[�W�I���N���X��������
	for(UINT n=0;n<MAXNUM_TEAM;n++)
	{
		m_RandomCharacterGetter.chars[n] = m_setting->characters[n];
	}
	m_RandomCharacterGetter.Initialize(scr_list);
	m_RandomStageGetter.Initialize(scr_list);

	m_prev_task_id = 'strt';
}

/*----------------------------------------------------------------
	���s
------------------------------------------------------------------*/
BOOL CTaskStoryMode::Execute(DWORD time)
{
	CStoryElement_Staff	*ele_staff	= dynamic_cast<CStoryElement_Staff*>(*scr_i);
	CStoryElement_VS	*ele_vs		= dynamic_cast<CStoryElement_VS*>(*scr_i);
	CStoryElement_Demo	*ele_demo	= dynamic_cast<CStoryElement_Demo*>(*scr_i);

	if(!ele_staff && !ele_vs && !ele_demo){
		//scr_i == scr_list.end() ?
		return FALSE;
	}

	switch(m_prev_task_id)
	{
	//#vs�u���b�N�̏I��
	case 'Btl':
		{
			if(!ele_vs)return FALSE;
			if(ele_vs->show_win || (g_battleresult.GetWinner()/*==1*/ && !ele_vs->nogameover)){
				//������ʂ�
				CYouWin *t_win = new CYouWin;
				if(g_battleresult.GetWinner() && ele_vs->text_lose)
				{
					//�����e�L�X�g�ݒ�
					t_win->SetStoryText(ele_vs->text_lose);
				}
				if(!g_battleresult.GetWinner() && ele_vs->text_win)
				{
					//�����e�L�X�g�ݒ�
					t_win->SetStoryText(ele_vs->text_win);
				}
				g_system.AddTask( t_win );
				return TRUE;
			}
			scr_i++;
			StartNextTask();
		}
		break;
	case 'Win':
		{
			if(!ele_vs)return FALSE;

			if (ele_vs->nogameover)	// nogameover�Ȃ珈�����p�X
			{
				story_selectflag += ele_vs->sel_shift + g_battleresult.GetWinner();
			} else {
				//����������Ă�
				if(g_battleresult.GetWinner()){
					if(m_continue_num<0){
						//�����R���e�B�j���[
						StartContinueTask();
						return TRUE;
					}
					if(m_continue_num != 0){
						//�R���e�B�j���[
						m_continue_num--;
						StartContinueTask();
						return TRUE;
					}
					//�Q�[���I�[�o�[
					CTStoryGameOver *go = new CTStoryGameOver;
					g_system.AddTask(go);
					return TRUE;
				}
			}
			//������
			scr_i++;
			StartNextTask();
		}
		break;
	//#demo�u���b�N�̏I��
	case 'Demo':
		{
			if(!ele_demo)return FALSE;
			scr_i++;
			StartNextTask();
		}
		break;
	//#staff�u���b�N�̏I��
	case 'Staf':
		{
			if(!ele_staff)return FALSE;
			scr_i++;
			StartNextTask();
		}
		break;
	//�ŏ��̃u���b�N
	case 'strt':
		{
			StartNextTask();
		}
		break;
	//�R���e�B�j���[��ʂ���߂��Ă���
	case 'Cntn':
		{
			if(!ele_vs)return FALSE;
			StartVSTask();
		}
		break;
	//�Q�[���I�[�o�[�H
	default:
		g_system.ReturnTitle();
	}
	return TRUE;
}

/*----------------------------------------------------------------
	�I��
------------------------------------------------------------------*/
void CTaskStoryMode::Terminate()
{
	//���ɂ�邱�ƂȂ�
}

/*----------------------------------------------------------------
	
------------------------------------------------------------------*/
void CTaskStoryMode::Activate(DWORD prvTaskID)
{
	m_prev_task_id = prvTaskID;
}

/*----------------------------------------------------------------
	�^�X�N�N��
------------------------------------------------------------------*/
void CTaskStoryMode::StartNextTask()
{
	// #select
	//      ���̃C�e���[�^�̎d�l���Ă�����ƕs�ւ����
	while (scr_i != scr_list.end() && (*scr_i)->GetType() == CStoryScriptElement::CStoryElement_Types::GSE_ST_SELECT){
		CStoryElement_Select *ele_select	= dynamic_cast<CStoryElement_Select*>(*scr_i);

		scr_i++;	// �������g�͂������������̂Ŏ���
		// �������f�ɍ���Ȃ��������΂�
		if (ele_select){
			if (ele_select->GetNum() == story_selectflag && !in_select)
				in_select = true;					// ������
			else if (ele_select->GetNum() == -1)
				in_select = false;					// ������
			else {
				// ����select���܂Ŕ�΂�
				while (scr_i != scr_list.end() && (*scr_i)->GetType() != CStoryScriptElement::CStoryElement_Types::GSE_ST_SELECT)
					scr_i++;
			}
		}
	}

	CStoryElement_Staff	*ele_staff	= dynamic_cast<CStoryElement_Staff*>(*scr_i);
	CStoryElement_VS	*ele_vs		= dynamic_cast<CStoryElement_VS*>(*scr_i);
	CStoryElement_Demo	*ele_demo	= dynamic_cast<CStoryElement_Demo*>(*scr_i);

	if(ele_staff)StartStaffTask();
	else if(ele_vs)StartVSTask();
	else if(ele_demo)StartDemoTask();
	else g_system.ReturnTitle();
}

//����
void CTaskStoryMode::StartVSTask()
{
	CStoryElement_VS *ele_vs = dynamic_cast<CStoryElement_VS*>(*scr_i);
	if(!ele_vs){
		g_system.ReturnTitle();
		return;
	}

	int t,j,n;

	//char1-3�w�����������
	for(t=0;t<2;t++){
		for(j=0;j<ele_vs->num[t];j++)
		{
			if(ele_vs->characters[t][j]<0)
			{
				switch(ele_vs->characters[t][j]){
				case -2:n=0;break;//char1
				case -3:n=1;break;//char2
				case -4:n=2;break;//char3
				default:n=3;
				}
				if(n<3){
					ele_vs->characters[t][j] = m_setting->characters[n];
					ele_vs->option[t][j] = m_setting->option[n];
					ele_vs->color[t][j] = m_setting->color[n];
				}
			}
		}
	}

	//�����_���̐l���𐔂��A�����_���L�����擾�N���X�ɒʒm
	UINT rdm_cnum=0;
	for(t=0;t<2;t++){
		for(j=0;j<ele_vs->num[t];j++){
			if(ele_vs->characters[t][j]==-1){
				rdm_cnum++;
			}
		}
	}
	m_RandomCharacterGetter.Req(rdm_cnum);

	//�����_���L�����N�^�[����
	for(t=0;t<2;t++){
		for(j=0;j<ele_vs->num[t];j++)
		{
			if(ele_vs->characters[t][j]<0){
				ele_vs->characters[t][j] = m_RandomCharacterGetter.GetNext();
				ele_vs->option[t][j] = g_charlist.GetRandomOption( ele_vs->characters[t][j] );
			}
		}
	}

	int t2,j2,c;

	//�F��auto�w�����������
	for(t=0;t<2;t++){
		for(j=0;j<ele_vs->num[t];j++)
		{
			if(ele_vs->color[t][j]<0)
			{
				c=1;
				//���łɌ��肵�Ă���Ƃ͂��Ԃ�Ȃ��悤�ɂ���
				for(t2=0;t2<2;t2++){
					for(j2=0;j2<ele_vs->num[t2];j2++)
					{
						if(t!=t2 && j!=j2){
							if(c==ele_vs->color[t2][j2] &&								//�F����
								ele_vs->characters[t][j]==ele_vs->characters[t2][j2])	//�L��������
							{
								c++;
								t2=0;
								j2=0;
							}
						}
						if(c==3)break;//���߂�
					}
					if(c==3)break;//���߂�
				}
				ele_vs->color[t][j]=c;
			}
		}
	}

	//�����_���X�e�[�W����������
	if(ele_vs->stage < 0){
		ele_vs->stage = m_RandomStageGetter.GetNext();
	}

	BOOL com;
	BYTE key;

	//�������N���X�ɏ��ݒ�
	g_battleinfo.Initialize();
	for(t=0;t<2;t++){
		com = t==0 ? FALSE : TRUE;
		for(j=0;j<ele_vs->num[t];j++)
		{
			key = com ? CASSIGN_COM : m_keyindex;
			if(ele_vs->type==TAISENKEISIKI_GOCYAMAZE && j!=0){//�����ΐ�̏ꍇ�̓`�[��1�ł�COM����
				key = CASSIGN_COM;
			}
			g_battleinfo.AddCharacter(
							t,
							ele_vs->characters[t][j],
							ele_vs->color[t][j],
							key,
							ele_vs->option[t][j]);
			//com���x����ݒ�
			g_battleinfo.SetComLevel( t,j,ele_vs->level[t][j],ele_vs->is_related_level[t][j]);
		}
	}

	char *tmpbgmname = new char[256];
	sprintf(tmpbgmname,"%s\\%s",g_storylist.GetStoryDir(m_story_index),ele_vs->bgm_filename);
	g_battleinfo.SetStoryBGM( tmpbgmname );				//BGM�ݒ�
	delete [] tmpbgmname;

	g_battleinfo.SetBattleType( ele_vs->type );			//�����`��
	g_battleinfo.SetLimitTime( ele_vs->limit_time );	//��������
	g_battleinfo.SetStage( ele_vs->stage );				//�X�e�[�W

	//�����^�X�N�N��
	g_system.AddTask( new CBattleTask );
}

//�f��
void CTaskStoryMode::StartDemoTask()
{
	CStoryElement_Demo	*ele_demo	= dynamic_cast<CStoryElement_Demo*>(*scr_i);
	if(!ele_demo){
		g_system.ReturnTitle();
		return;
	}

	CTDemo *tdemo = new CTDemo;
	tdemo->SetKeyIndex(m_keyindex);
	tdemo->Setup(m_story_index,ele_demo->m_filename);
	g_system.AddTask( tdemo );
}

//�X�^�b�t���[��
void CTaskStoryMode::StartStaffTask()
{
	CStoryElement_Staff	*ele_staff	= dynamic_cast<CStoryElement_Staff*>(*scr_i);

	CTStaffRoll *tstaff = new CTStaffRoll;
	tstaff->SetKeyIndex(m_keyindex);
	tstaff->Setup( g_storylist.GetStoryDir(m_story_index) , ele_staff );
	g_system.AddTask( tstaff );
}

//�R���e�B�j���[
void CTaskStoryMode::StartContinueTask()
{
	CTStoryContinue *con = new CTStoryContinue;
	con->remain_cont = m_continue_num;
	g_system.AddTask(con);
}


/*===============================================================

	�����_�� �L�����N�^�[�E�X�e�[�W �擾�N���X

=================================================================*/

/*----------------------------------------------------------------
	�����_���L�����N�^�[�N���X
------------------------------------------------------------------*/
void CTaskStoryMode::CRandomCharacterGetter::Initialize(SScriptElementList& scr_list)
{
	//�S�L�����N�^�[����荞��
	UINT rseed = rand()%g_charlist.GetCharacterCount();//�C���`�L�����_���V�[�h
	UINT n = g_charlist.GetCharacterCount();
	UINT nn = g_charlist.GetCharacterCount();
	while(n!=0){
		n--;

		BOOL in=TRUE;
		for(int k=0;k<MAXNUM_TEAM;k++)
		{
			if(chars[k]==(n+rseed)%nn)in=FALSE;
		}
		if(in)all_list.push_back((n+rseed)%nn);
	}

	int t,j;

	gbl.ods("������ �����_���L�����N�^�[all(%d) ������",all_list.size());

	//�X�g�[���[�X�N���v�g�Ŗ����I�Ɏw�肳��Ă���L�����N�^�[�̓��X�g���珜��
	CStoryElement_VS *ele_vs;
	CStoryElement_Settings *ele_set;
	SScriptElementList::iterator i,ie;
	i =scr_list.begin();
	ie=scr_list.end();
	for(;i!=ie;i++){
		ele_vs = dynamic_cast<CStoryElement_VS*>( *i );
		if(ele_vs){
			for(t=0;t<2;t++){
				for(j=0;j<ele_vs->num[t];j++){
					if(ele_vs->characters[t][j]>=0){
						EraseFromAllList( ele_vs->characters[t][j] );
					}
				}
			}
		}
		ele_set = dynamic_cast<CStoryElement_Settings*>( *i );
		if(ele_set){
			for(t=0;t<ele_set->cnum;t++){
				if(ele_set->characters[t]>=0){
					EraseFromAllList( ele_set->characters[t] );
				}
			}
		}
	}
	gbl.ods("������ �����_���L�����N�^�[all(%d) ������",all_list.size());

	//���݃��X�g�𐶐��i�T�C�Y��0�Ȃ̂ŕK����荞�ݔ����j
	Req(1);
}

//���̎�����,�w�肵�����̃����_���L�����N�^�[���K�v�B
//����Ȃ��ꍇ�́A�S�̃��X�g����荞��ŃV���b�t�����Ȃ���
void CTaskStoryMode::CRandomCharacterGetter::Req(UINT num)
{
	if(crnt_list.size() > num)return;

	crnt_list.clear();

	if(all_list.size() < num)//���߂�
	{
		//�S�̃��X�g��H���Ă�����Ȃ�
		//�d�����Ȃ��̂łƂ肠�����S�L�����N�^�[����荞��
		gbl.ods("������ �����_���L�����N�^�[�s��������(%d/%d) ������",all_list.size(),num);
		UINT n = g_charlist.GetCharacterCount();
		while(n!=0){
			n--;
			all_list.push_back(n);
		}
	}

	//�S�̃��X�g����荞��
	CIndexList::iterator i,ie;
	i = all_list.begin();
	ie= all_list.end();
	while(i!=ie){
		crnt_list.push_back(*i);
		i++;
	}

	//�V���b�t��
	srand(timeGetTime());
	UINT tempra;
	UINT tempra1;
	UINT tempra2;
	for(UINT n=0;n<crnt_list.size();n++)
	{
		tempra1 = rand()%crnt_list.size();
		tempra2 = rand()%crnt_list.size();

		tempra = crnt_list[tempra1];
		crnt_list[tempra1] = crnt_list[tempra2];
		crnt_list[tempra2] = tempra;
	}

	gbl.ods("������ �����_���L�����N�^�[���X�g���Z�b�g(%d) ������",crnt_list.size());
}

//���̃����_���L�����N�^�[���擾����
UINT CTaskStoryMode::CRandomCharacterGetter::GetNext()
{
	if(crnt_list.size()==0){
		g_system.Log("CTaskStoryMode::CRandomCharacterGetter::GetNext() , list size zero\n",SYSLOG_WARNING);
		Req(1);
	}

	CIndexList::iterator nxt = crnt_list.begin();
	UINT ret = *nxt;
	crnt_list.erase(nxt);

	gbl.ods("CTaskStoryMode::CRandomCharacterGetter::GetNext() - %d",ret);

	return ret;
}

//�w�肳�ꂽ�L�����N�^�[��S�̃��X�g���珜�O����
void CTaskStoryMode::CRandomCharacterGetter::EraseFromAllList(UINT n)
{
	CIndexList::iterator i =all_list.begin();
	CIndexList::iterator ie=all_list.end();
	for(;i!=ie;i++){
		if( *i==n ){
			all_list.erase(i);
			return;
		}
	}
}


/*----------------------------------------------------------------
	�����_���X�e�[�W�N���X
------------------------------------------------------------------*/
void CTaskStoryMode::CRandomStageGetter::Initialize(SScriptElementList& scr_list)
{
	//�S�X�e�[�W����荞��
	UINT nrand = rand()%g_stagelist.GetStageCount();
	UINT n = g_stagelist.GetStageCount();
	UINT nn = g_stagelist.GetStageCount();
	while(n!=0){
		n--;
		all_list.push_back((n+nrand)%nn);
	}

	//�X�g�[���[�X�N���v�g�Ŗ����I�Ɏw�肳��Ă���X�e�[�W�̓��X�g���珜��
	CStoryElement_VS *ele_vs;
	SScriptElementList::iterator i,ie;
	i =scr_list.begin();
	ie=scr_list.end();
	for(;i!=ie;i++){
		ele_vs = dynamic_cast<CStoryElement_VS*>( *i );
		if(ele_vs){
			if(ele_vs->stage>=0)EraseFromAllList( ele_vs->stage );
		}
	}

	//���݃��X�g�𐶐��i�T�C�Y��0�Ȃ̂ŕK����荞�ݔ����j
	Restore();
}

//�S�̃��X�g���A���݃��X�g�Ɏ�荞��
void CTaskStoryMode::CRandomStageGetter::Restore()
{
	SIndexList::iterator i,ie;
	i = all_list.begin();
	ie= all_list.end();
	while(i!=ie){
		crnt_list.push_back(*i);
		i++;
	}

	//�V���b�t��
	srand(timeGetTime());
	UINT tempra;
	UINT tempra1;
	UINT tempra2;
	for(UINT n=0;n<crnt_list.size();n++)
	{
		tempra1 = rand()%crnt_list.size();
		tempra2 = rand()%crnt_list.size();

		tempra = crnt_list[tempra1];
		crnt_list[tempra1] = crnt_list[tempra2];
		crnt_list[tempra2] = tempra;
	}
	
	gbl.ods("������ �����_���X�e�[�W���X�g���Z�b�g(%d) ������",crnt_list.size());
}

//���̃����_���X�e�[�W���擾����
UINT CTaskStoryMode::CRandomStageGetter::GetNext()
{
	if(crnt_list.size()==0){
		Restore();
	}

	SIndexList::iterator nxt = crnt_list.begin();
	UINT ret = *nxt;
	crnt_list.erase(nxt);

	return ret;
}

//�w�肳�ꂽ�X�e�[�W��S�̃��X�g���珜�O����
void CTaskStoryMode::CRandomStageGetter::EraseFromAllList(UINT n)
{
	SIndexList::iterator i =all_list.begin();
	SIndexList::iterator ie=all_list.end();
	for(;i!=ie;i++){
		if( *i==n ){
			all_list.erase(i);
			return;
		}
	}
}



/*===============================================================

	�R���e�B�j���[�^�X�N

=================================================================*/

void CTStoryContinue::Initialize()
{
	m_texfb = g_draw.GetFrontBufferCopy();
	m_time = 0;
	m_yes_selected = TRUE;
}

BOOL CTStoryContinue::Execute(DWORD time)
{
	m_time+=1.0f;

	if(m_time>30.0f)
	{
		if(g_input.GetAllKey() & KEYSTA_ANYKEY)
		{
			if(m_yes_selected)
				g_system.AddTask( new CBattleTask );//�O�̐ݒ�̂܂܍Đ�
			else
				g_system.ReturnTitle();
		}
		else if((g_input.GetAllKey() & KEYSTA_UP2)||(g_input.GetAllKey() & KEYSTA_DOWN2))
		{
			m_yes_selected = !m_yes_selected ;
		}
	}
	return TRUE;
}

void CTStoryContinue::Terminate()
{
	RELEASE(m_texfb);
}

void CTStoryContinue::Draw()
{
	g_draw.SetTransform(FALSE);

	float t = 1.0f - m_time/30.0f;
	if(t<0.2f)t = 0.2f;

	DrawFBC( t );

	t = m_time/30.0f;
	if(t>1.0f)t = 1.0f;

	DWORD c = (DWORD)(t*255.0f);
	DWORD col = 0xFF000000 | (c<<16) | (c<<8) | c;

	char *tmpstr = new char[256];
	sprintf(tmpstr,"continue? (%d)",remain_cont);
	g_system.DrawBMPText(50,230,0.0f,tmpstr,col);
	delete [] tmpstr;

	c/=2;
	col = 0xFF000000 | (c<<16) | (c<<8) | c;
	g_system.DrawBMPText(350,280,0.0f,"Yes",col);
	g_system.DrawBMPText(350,310,0.0f,"No",col);
	
	c*=2;
	col = 0xFF000000 | (c<<16) | (c<<8) | c;
	if(m_yes_selected)
		g_system.DrawBMPText(348,278,0.0f,"Yes",col);
	else
		g_system.DrawBMPText(348,308,0.0f,"No",col);
}

void CTStoryContinue::DrawFBC(float t)
{
	DWORD c = (DWORD)(t*255.0f);

	MYVERTEX3D* vb;

	if ( !g_draw.pMyVertex || FAILED(g_draw.pMyVertex->Lock(0, 0, (void**)&vb, D3DLOCK_DISCARD)) )
		return;

	vb[0].z = 0.0f;
	vb[1].z = 0.0f;
	vb[2].z = 0.0f;
	vb[3].z = 0.0f;

	vb[0].tu = 0.0f;
	vb[1].tu = 0.0f;
	vb[2].tu = 1.0f;
	vb[3].tu = 1.0f;
	
	vb[0].tv = 0.0f;
	vb[1].tv = 1.0f;
	vb[2].tv = 0.0f;
	vb[3].tv = 1.0f;
	
	vb[0].color = 
	vb[1].color = 
	vb[2].color = 
	vb[3].color = 0xFF000000 | (c<<16) | (c<<8) | c;

	vb[0].x =  0.0f;
	vb[1].x =  0.0f;
	vb[2].x =  2.0f*320.0f/240.0f;
	vb[3].x =  2.0f*320.0f/240.0f;

	vb[0].y =  0.0f;
	vb[1].y =  2.0f;
	vb[2].y =  0.0f;
	vb[3].y =  2.0f;

	if (g_draw.pMyVertex) g_draw.pMyVertex->Unlock();

	D3DXMATRIX mati;
	D3DXMatrixIdentity(&mati);
	g_draw.EnableZ(FALSE,FALSE);
	g_draw.SetTransform(FALSE);
	g_draw.d3ddev->SetStreamSource(0, g_draw.pMyVertex, 0, sizeof(MYVERTEX3D));
	g_draw.d3ddev->SetFVF(FVF_3DVERTEX);
	g_draw.d3ddev->SetTexture(0,m_texfb);
	g_draw.d3ddev->SetTransform(D3DTS_WORLD,&mati);
	g_draw.d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
}

/*===============================================================

	�Q�[���I�[�o�[�^�X�N

=================================================================*/

BOOL CTStoryGameOver::Execute(DWORD time)
{
	m_time += 1.0f;

	if((g_input.GetAllKey() & KEYSTA_ANYKEY) && m_time>60.0f){
		g_system.ReturnTitle();
	}
	return TRUE;
}

void CTStoryGameOver::Draw()
{
	g_draw.SetTransform(FALSE);

	float t = 1.0f - m_time/200.0f;
	if(t<0.1f)t=0.1f;
	DrawFBC(t);

	if(m_time>20.0f)
	{
		t = (m_time-20.0f)/60.0f;
		if(t>1.0f)t=1.0f;
		DWORD c = (DWORD)(t*255.0f);
		DWORD col = 0xFF000000 | (c<<16) | (c<<8) | c;
		g_system.DrawBMPText(50,230,0.0f,"game over!",col);
	}
}

