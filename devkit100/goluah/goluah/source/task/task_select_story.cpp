/*=======================================================================================

	�X�g�[���[�Z���N�g

=========================================================================================*/

#include "stdafx.h"
#include "global.h"
#include "task_select_story.h"
#include "task_storymode.h"


/*=======================================================================================

	�X�g�[���[�I���^�X�N
	�{��

=========================================================================================*/

//�`��v���C�I���e�B�萔
int CTStorySelect::m_pri_ring		=1050;
int CTStorySelect::m_pri_paramwin	=1057;
int CTStorySelect::m_pri_brief		=1055;
int CTStorySelect::m_pri_bigface	=1060;
int CTStorySelect::m_pri_preview	=1070;
int CTStorySelect::m_pri_bg			=2000;


/*-----------------------------------------------------------------------
	������
-------------------------------------------------------------------------*/
void CTStorySelect::Initialize()
{
	//�������O�N���X�o�^
	m_ring = new CTStorySelecterRing;
	m_ring->SetPriority(m_pri_ring);
	m_ring->SetKeyInputIndex(m_keyindex);
	g_system.AddTask(m_ring);

	//���w�i�`��N���X�o�^
	m_bg = new CTStorySelectBG();
	m_bg->SetPriority(m_pri_bg);
	g_system.AddTask(m_bg);

	//���f�J��\���N���X�o�^
	m_face = new CTStoryBigFace;
	m_face->SetPriority(m_pri_bigface);
	g_system.AddTask(m_face);

	//���v���r���[�\���N���X�o�^
	m_pv = new CTStorySelectPreview;
	m_pv->SetPriority(m_pri_preview,m_pri_brief);
	g_system.AddTask(m_pv);

	m_paramwin = NULL;

	m_first_update = TRUE;
	m_sselected = FALSE;
	m_complete_flag = FALSE;
	m_sinfo = NULL;

	g_sound.BGMPlay(".\\system\\bgm\\choice",TRUE);
}

/*-----------------------------------------------------------------------
	���s
-------------------------------------------------------------------------*/
BOOL CTStorySelect::Execute(DWORD time)
{
	int i;

	if(m_first_update)
	{
		m_first_update = FALSE;

		//�f�J��N���X�ɐݒ�
		CStoryList::CStoryInfo* sinfo = g_storylist.GetStoryInfo( m_ring->GetSelected() );
		if(sinfo)
		{
			for(i=0;i<MAXNUM_TEAM;i++)
			{
				m_alts[i] = OPT2ALT(sinfo->option[i]);
			}

			m_face->Setup(sinfo->characters,sinfo->color,m_alts,sinfo->cnum);
		}

		//�v���r���[�N���X�ݒ�
		m_pv->Change( m_ring->GetSelected() );
	}

	if(m_sselected){
		m_face->SetDisplayMode(TRUE);
		if(m_paramwin)
			m_face->SetLightIndex( m_paramwin->GetCurrentCharacter() );
		for(i=0;i<m_sinfo->cnum;i++){
			m_face->ChangeCharacter(m_sinfo->characters[i],i);
			m_face->ChangeColor(m_sinfo->color[i],i);
		}
	}
	else{
		m_face->SetDisplayMode(FALSE);
	}

	//�I��������̃E�F�C�g
	if(m_complete_flag){
		if(++m_complete_counter>45 || g_input.GetKey(m_keyindex,0)&KEYSTA_ANYKEY)
		{
			//�X�g�[���[���[�h�^�X�N�N��
			CTaskStoryMode *tsm = new CTaskStoryMode;
			tsm->Setup(m_selected_story_index,m_keyindex,m_sinfo);
			g_system.AddTask( tsm );
		}
		m_face->SetDisplayMode(FALSE);
	}

	return TRUE;
}

/*-----------------------------------------------------------------------
	�j��
-------------------------------------------------------------------------*/
void CTStorySelect::Terminate()
{
	g_sound.BGMStop();
	SAFEDELETE(m_sinfo);
}

/*-----------------------------------------------------------------------
	�X�g�[���[�I��������
-------------------------------------------------------------------------*/
void CTStorySelect::OnSelect(CTStorySelecterRing *ring)
{
	m_ring->Hide();
	m_pv->HideText();

	m_selected_story_index = m_ring->GetSelected();

	//setting���𕡐�
	CStoryList::CStoryInfo* sinfo = g_storylist.GetStoryInfo( m_selected_story_index );
	if(sinfo){
		m_sinfo = sinfo->Clone();
	}
	else g_system.ReturnTitle();

	if(m_sinfo->cnum==0){
		//�p�����[�^�I���̕K�v�Ȃ�
		OnParamSelectComplete();
		return;
	}

	//���p�����[�^�ݒ�E�C���h�E�o�^
	m_paramwin = new CTStoryParamWindow;
	m_paramwin->SetPriority(m_pri_paramwin);
	m_paramwin->SetKeyIndex(m_keyindex);
	g_system.AddTask(m_paramwin);

	m_sselected = TRUE;
}


/*-----------------------------------------------------------------------
	�p�����[�^�I�����I��
-------------------------------------------------------------------------*/
void CTStorySelect::OnParamSelectComplete()
{
	m_complete_flag = TRUE;
	m_complete_counter = 0;
	m_pv->HidePreview();
	if(m_paramwin){
		m_paramwin->Close();
	}
}


/*-----------------------------------------------------------------------
	�X�g�[���[�ύX������
-------------------------------------------------------------------------*/
void CTStorySelect::OnChange(CTStorySelecterRing *ring)
{
	int sindex = ring->GetSelected();

	m_pv->Change( sindex );

	CStoryList::CStoryInfo* sinfo = g_storylist.GetStoryInfo( sindex );
	if(!sinfo)return;//���߂�

	m_face->Setup(sinfo->characters,sinfo->color,m_alts,sinfo->cnum);
}

void CTStorySelect::OnCancel()
{
/*	if (charsel_ok[0]){
		num_selected[1]--;
	}
	for (int i = 0; i < 2; i++)
	{
		m_ring[i]->Hide();
		m_belt[i]->Show(FALSE);
		m_bface[i]->SetPriority(0);
		m_bface[i]->ResetNum();
		for (int j = 0; j < MAXNUM_TEAM; j++)
		{
			m_bface[i]->Set(j, 0, 0);
			m_bface[i]->SetTemporary(j, 0, 0);
		}
	}
	g_system.taskManager.m_ring->Terminate();
	m_bg->Terminate();
	m_face->Terminate();m_pv->Terminate();
	Initialize();*/
}


/*-----------------------------------------------------------------------
	�X�g�[���[�I����̃p�����[�^�ύX
-------------------------------------------------------------------------*/
void CTStorySelect::SelectChangeCharacter(UINT idx,int cindex)
{
	if(m_sinfo->characters[idx]!=cindex)
	{
		m_sinfo->characters[idx] = cindex;
		m_sinfo->option[idx] = 0;
		m_face->ChangeAlt( m_alts[idx],idx );
	}
}

void CTStorySelect::SelectChangeColor(UINT idx,int col)
{
	m_sinfo->color[idx] = col;
}

void CTStorySelect::SelectChangeOption(UINT idx,DWORD opt)
{
	m_sinfo->option[idx] = opt;
	m_alts[idx] = OPT2ALT(opt);
	m_face->ChangeAlt( m_alts[idx],idx );
}

/*=======================================================================================

	�X�g�[���[�I�������O

=========================================================================================*/


/*-----------------------------------------------------------------------
	�����O�ԍ�+�C���f�b�N�X ����A�S�̂ł̃C���f�b�N�X���擾����
-------------------------------------------------------------------------*/
UINT CTStorySelecterRing::GetSelected()
{
	return g_storylist.Ring2Index(m_selected_ring,m_selected_index);
}

/*-----------------------------------------------------------------------
	Initialize����R�[�������
-------------------------------------------------------------------------*/
void CTStorySelecterRing::InitializeSub()
{
	//�p�����[�^���Z�b�g
	m_selected_ring = 0;
	m_selected_index = 0;
	m_counter = 0;
	m_state = CTCRS_NotReady;
	m_radius_base = 120;
	m_sleepCount = 0;

	m_icons = NULL;
	m_belt = new CTStorySelectBelt;
	m_belt->SetLeft();
	m_belt->SetPriority(m_draw_priority+1);
	m_belt->SetRing( GetSelected() ,FALSE);
	m_belt->SetBaseY( 420.0f );
	g_system.AddTask(m_belt);

	SetPos(480,410);
	SetHeight(18);
}


/*-----------------------------------------------------------------------
	�X�V�B�I����Ă���I������
-------------------------------------------------------------------------*/
BOOL CTStorySelecterRing::Execute(DWORD time)
{
	if(GetDrawPriority()<0)return FALSE;//�I��
	return CTIconRingBase::Execute(time);
}


/*-----------------------------------------------------------------------
	�\������A�C�R���摜����������
-------------------------------------------------------------------------*/
void CTStorySelecterRing::InitializeIcons()
{
	UINT num = g_storylist.GetAllStoryNum();
	if(num==0)return;

	char *filepath = new char [MAX_PATH];

	m_icons = new MYSURFACE* [num];
	for(UINT i=0;i<num;i++){
		g_storylist.GetStoryIconPath(i,filepath);
		m_icons[i] = g_draw.CreateSurfaceFrom256BMP(filepath);
	}
	
	delete [] filepath;
}

/*-----------------------------------------------------------------------
	�\������A�C�R���摜��j������
-------------------------------------------------------------------------*/
void CTStorySelecterRing::DestroyIcons()
{
	if(!m_icons)return;
	UINT num = g_storylist.GetAllStoryNum();

	for(UINT i=0;i<num;i++){
		RELSURFACE(	m_icons[i] );
	}
	DELETEARRAY( m_icons );
}

/*-----------------------------------------------------------------------
	�\������A�C�R���摜���擾�iDraw����R�[���j
-------------------------------------------------------------------------*/
MYSURFACE* CTStorySelecterRing::GetIcon(UINT ridx,UINT idx)
{
	int num = g_storylist.Ring2Index(ridx,idx);
	if(num<0)return NULL;

	return m_icons[num];
}

/*-----------------------------------------------------------------------
	�A�C�R���`�掞�̒��_�F���w�肷��(Draw����R�[��,����0�̂���)
-------------------------------------------------------------------------*/
DWORD CTStorySelecterRing::GetColor(UINT ridx,UINT idx)
{
	return 0x00FFFFFF;
}


/*-----------------------------------------------------------------------
	�{�^���������ꂽ�Ƃ��̏���
-------------------------------------------------------------------------*/
void CTStorySelecterRing::OnButtonDown(DWORD key)
{
	//SELECT�^�X�N�Q�b�g
	CTStorySelect* csselect = dynamic_cast<CTStorySelect*>(g_system.GetCurrentMainTask());
	if(!csselect)return;

	if(key==KEYSTA_BA2){
		csselect->OnSelect(this);
	}
	if(key==KEYSTA_BB2){
		csselect->OnCancel();
	}
}

/*-----------------------------------------------------------------------
	�����O���܂킳��ăC���f�b�N�X���ω������Ƃ��̏���
-------------------------------------------------------------------------*/
void CTStorySelecterRing::OnChangeIndex()
{
	m_belt->SetRing( GetSelected() ,FALSE);

	//SELECT�^�X�N�Q�b�g
	CTStorySelect* csselect = dynamic_cast<CTStorySelect*>(g_system.GetCurrentMainTask());
	if(!csselect)return;

	csselect->OnChange(this);
}

/*-----------------------------------------------------------------------
	�����O���ύX���ꂽ�Ƃ��̏���
-------------------------------------------------------------------------*/
void CTStorySelecterRing::OnChangeRing()
{
	m_belt->SetRing( GetSelected() ,TRUE);
	
	//SELECT�^�X�N�Q�b�g
	CTStorySelect* csselect = dynamic_cast<CTStorySelect*>(g_system.GetCurrentMainTask());
	if(!csselect)return;

	csselect->OnChange(this);
}

/*-----------------------------------------------------------------------
	�����O�̑������擾����
-------------------------------------------------------------------------*/
UINT CTStorySelecterRing::GetMaxRing()
{
	return g_storylist.GetRingNum();
}

/*-----------------------------------------------------------------------
	�w�胊���O�������Ă���A�C�R���̐����擾
-------------------------------------------------------------------------*/
UINT CTStorySelecterRing::GetMaxIndex(UINT ridx)
{
	UINT ret = g_storylist.GetRingCount(ridx);
	return g_storylist.GetRingCount(ridx);
}

/*-----------------------------------------------------------------------
	����
-------------------------------------------------------------------------*/
void CTStorySelecterRing::Hide()
{
	CTIconRingBase::Hide();
	m_belt->Show(FALSE);//�I�r������
}


/*-----------------------------------------------------------------------
	�X�g�[���[�����O���I�r�N���X
-------------------------------------------------------------------------*/
void CTStorySelecterRing::CTStorySelectBelt::Initialize()
{
	CTCharacterSelectBelt::Initialize();
	m_txtLeft = 640 - 20;
	m_txtTop = FALSE;
	m_txtR2L = FALSE;
}

void CTStorySelecterRing::CTStorySelectBelt::SetRing( UINT idx ,BOOL real_change)
{
	if(real_change){
		CTCharacterSelectBelt::SetRing(idx);
		return;
	}

	m_ringIndex = idx;
	UpdateText();
}

void CTStorySelecterRing::CTStorySelectBelt::UpdateText()
{
	//m_ringIndex�́A�X�g�[���[�����O�ԍ��ł͂Ȃ��S�̂̒ʂ��ԍ��Ƃ��Ďg�p
	sprintf(m_disp_str,"%s",g_storylist.GetStoryDir(m_ringIndex));
	for(UINT i=0;i<strlen(m_disp_str);i++){
		if(m_disp_str[i]=='\\'){
			m_disp_str[i]='/';//�������̂ق����������悳��
		}
	}}



/*=======================================================================================

	�f�J��\���N���X

=========================================================================================*/

#define CTStoryBigFace_AppearInterval (10)//�o���C���^�[�o��

/*-----------------------------------------------------------------------
	�����ݒ�
-------------------------------------------------------------------------*/
void CTStoryBigFace::Initialize()
{
	m_dark = TRUE;
	m_appear_counter=0;

	m_num = 0;

	for(int i=0;i<MAXNUM_TEAM;i++)
	{
		m_characters[i] = 0;
		m_colors[i] = 0;
	}

	CTCharacterBigFace::Initialize();
	CTCharacterBigFace::SetLeftRight(TRUE);
}

/*-----------------------------------------------------------------------
	���s
-------------------------------------------------------------------------*/
BOOL CTStoryBigFace::Execute(DWORD time)
{
	if(m_appear_counter)
	{
		m_appear_counter --;
		if(m_appear_counter%CTStoryBigFace_AppearInterval==0)
		{
			//�蒅
			CTCharacterBigFace::Set(
						m_cindex[m_selected_num],
						m_color [m_selected_num],
						m_alt[m_selected_num]);
			//�e���|�����[�ɏo��
			m_appear_cnt ++;
			if(m_appear_cnt<=m_num){	
				SetTemporary(m_characters[m_num-m_appear_cnt],m_colors[m_num-m_appear_cnt],m_alt[m_num-m_appear_cnt]);
			}
		}
	}

	return CTCharacterBigFace::Execute(time);
}

/*-----------------------------------------------------------------------
	�L�����ꊇ�w��
-------------------------------------------------------------------------*/
void CTStoryBigFace::Setup(int *chars,int *colors,int *alts,int num)
{
//	CTCharacterBigFace::Initialize();//�p�����[�^���Z�b�g
	m_selected_num = 0;

	m_num = num;
	m_appear_counter = num*CTStoryBigFace_AppearInterval ;

	if(num==0)return;

	for(int i=0;i<num;i++){
		m_characters[i] = chars[num-1-i];
		m_colors[i] = colors[num-1-i];
		m_alt[i] = alts[num-1-i];
	}

	SetNum(m_num);
	SetTemporary(m_characters[num-1],m_colors[num-1],m_alt[num-1]);
	m_appear_cnt = 1;
}

/*-----------------------------------------------------------------------
	�����L�����F�ύX
-------------------------------------------------------------------------*/
void CTStoryBigFace::ChangeCharacter(int cindex,int idx)
{
	m_cindex[idx] = cindex;
}

/*-----------------------------------------------------------------------
	�����L�����ύX
-------------------------------------------------------------------------*/
void CTStoryBigFace::ChangeColor(int color,int idx)
{
	m_color[idx] = color>0 ? color : 1;
}

/*-----------------------------------------------------------------------
	�����L����FaceAlt�ύX
-------------------------------------------------------------------------*/
void CTStoryBigFace::ChangeAlt(int alt,int idx)
{
	m_alt[idx] = alt;
}

/*-----------------------------------------------------------------------
	dark=TRUE �� �I���L�����ȊO���Â��\��
-------------------------------------------------------------------------*/
void CTStoryBigFace::SetDisplayMode(BOOL dark)
{
	dark = TRUE;
}

/*-----------------------------------------------------------------------
	dark=TRUE ���ɖ��邭�\������L�������w��
-------------------------------------------------------------------------*/
void CTStoryBigFace::SetLightIndex(int idx)
{
	m_lighting = idx;
}

/*-----------------------------------------------------------------------
	�`�掞�̕\���F�w��
-------------------------------------------------------------------------*/
DWORD CTStoryBigFace::GetDispColor(UINT idx)
{
	if(!m_dark)return 0xFFFFFFFF;

	else return idx==m_lighting ? 0xFFFFFFFF : 0xFF999999;
}



/*=======================================================================================

	�X�g�[���[�v���r���[�\���N���X

=========================================================================================*/

#define CTStorySelectPreview_Anim (40)

/*-----------------------------------------------------------------------
	������
-------------------------------------------------------------------------*/
void CTStorySelectPreview::Initialize()
{
	//�p�����[�^���Z�b�g
	m_counter		= 0;
	m_crnt_sindex	=-1;
	m_prev_sindex	=-1;
	m_pv			=NULL;
	m_pv_prev		=NULL;

	
	//�T�v�e�L�X�g�`��N���X��ʃ^�X�N�Ƃ��ċN��
	m_brief = new CTStoryBrief;
	m_brief->SetPriority(m_txt_draw_priority);
	g_system.AddTask(m_brief);

	//const?
	m_height = 75.0f;
	y_pos = 150.0f;
}

/*-----------------------------------------------------------------------
	���s
-------------------------------------------------------------------------*/
BOOL CTStorySelectPreview::Execute(DWORD time)
{
	if(m_counter){
		m_counter--;
		if(m_counter==0)m_brief->Set(m_crnt_sindex);
	}
	m_ratio = (float)m_counter / (float)CTStorySelectPreview_Anim;

	return TRUE;
}

/*-----------------------------------------------------------------------
	�j��
-------------------------------------------------------------------------*/
void CTStorySelectPreview::Terminate()
{
	RELSURFACE(m_pv);
	RELSURFACE(m_pv_prev);
}

/*-----------------------------------------------------------------------
	�`��
-------------------------------------------------------------------------*/
void CTStorySelectPreview::Draw()
{
	RECT rsrc;
	MYRECT3D rdst;
	BYTE alpha;
	DWORD col;
	
	rsrc.top=rsrc.left=0;
	rdst.z = 0.0f;

	const float ybase = y_pos/240.0f;
	const float hgt   = m_height/240.0f;
	const float inv_ratio = 1.0f - m_ratio;//0��1
	const float xmgn = 0.1f;

	if(m_counter && m_pv_prev){
		//"�O��"�v���r���[�`��
		rsrc.right =(LONG)m_pv_prev->wg;
		rsrc.bottom=(LONG)m_pv_prev->hg;

		rdst.left  = -xmgn ;
		rdst.right = 2.0f+xmgn ;
		rdst.top   = ybase -hgt*m_ratio;
		rdst.bottom= ybase +hgt*m_ratio;

		rdst.left	*=ASPECTRATIO;
		rdst.right	*=ASPECTRATIO;
		
		alpha = (BYTE)(255 * m_ratio);
		col = 0x00FFFFFF | alpha<<24 ;

		g_draw.MyBlt3D(m_pv_prev,rsrc,rdst,0,col);
	}

	alpha = (BYTE)(255 * inv_ratio);
	col = 0x00FFFFFF | alpha<<24 ;

	if(m_pv)
	{
		//�v���r���[�`��
		rsrc.right =(LONG)m_pv->wg;
		rsrc.bottom=(LONG)m_pv->hg;

		rdst.left  = -xmgn -1.0f*m_ratio ;
		rdst.right = 2.0f + xmgn +0.2f*m_ratio ;
		rdst.top   = ybase -hgt - hgt*m_ratio;
		rdst.bottom= ybase +hgt + hgt*m_ratio;
		
		rdst.left	*=ASPECTRATIO;
		rdst.right	*=ASPECTRATIO;

		g_draw.MyBlt3D(m_pv,rsrc,rdst,0,col);
	}
}

/*-----------------------------------------------------------------------
	�ύX
-------------------------------------------------------------------------*/
void CTStorySelectPreview::Change(UINT sindex)
{
	m_prev_sindex = m_crnt_sindex;
	m_crnt_sindex = sindex;

	m_pv_prev = m_pv;
	char *filename = new char[MAX_PATH];
	g_storylist.GetStoryPreviewPath(m_crnt_sindex,filename);
	m_pv = g_draw.CreateSurfaceFrom256BMP(filename);
	delete [] filename;

	m_counter = CTStorySelectPreview_Anim;
	m_brief->Clear();
}

/*-----------------------------------------------------------------------
	����
-------------------------------------------------------------------------*/
void CTStorySelectPreview::HidePreview()
{
	m_prev_sindex = m_crnt_sindex;
	m_crnt_sindex = -1;

	m_pv_prev = m_pv;
	m_pv = NULL;
	m_counter = CTStorySelectPreview_Anim;
}


/*-----------------------------------------------------------------------
	�X�g�[���[�v���r���[::�T�v�e�L�X�g
	������
-------------------------------------------------------------------------*/
void CTStorySelectPreview::CTStoryBrief::Initialize()
{
	disp_txt = NULL;
	m_sindex = -1;

	m_hideFlag = FALSE;
	m_hideMove=0.0f;
}

/*-----------------------------------------------------------------------
	�X�g�[���[�v���r���[::�T�v�e�L�X�g
	���s
-------------------------------------------------------------------------*/
BOOL CTStorySelectPreview::CTStoryBrief::Execute(DWORD time)
{
	if(m_sindex<0)return TRUE;

	char *str = g_storylist.GetStoryBrief(m_sindex);

	//1�������₷
	if(disp_len < (int)strlen(str))
	{
		int len = (str[disp_len]&0x80) ? 2 : 1;
		for(;len>0;len--){
			disp_txt[disp_len] = str[disp_len];
			disp_len++;
		}
		disp_txt[disp_len] = '\0';
	}

	//�B���Ƃ�����
	if(m_hideFlag){
		m_hideMove+=9.0f;
	}

	return TRUE;
}

//�w��C���f�b�N�X�̃X�g�[���[�T�v�̕`����J�n����
void CTStorySelectPreview::CTStoryBrief::Set(UINT sindex)
{
	m_sindex = sindex;

	char *str = g_storylist.GetStoryBrief(m_sindex);
	DELETEARRAY(disp_txt);
	if(str){
		disp_txt = new char[strlen(str)+1];
		disp_txt[0] = '\0';
	}
	disp_len=0;
}

//�e�L�X�g���N���A���A�`����s��Ȃ�����
void CTStorySelectPreview::CTStoryBrief::Clear()
{
	m_sindex = -1;
	DELETEARRAY(disp_txt);
}

//�B��
void CTStorySelectPreview::CTStoryBrief::Hide()
{
	m_hideFlag=TRUE;
	m_hideMove=0.0f;
}

/*-----------------------------------------------------------------------
	�X�g�[���[�v���r���[::�T�v�e�L�X�g
	�j��
-------------------------------------------------------------------------*/
void CTStorySelectPreview::CTStoryBrief::Terminate()
{
	DELETEARRAY(disp_txt);
}

/*-----------------------------------------------------------------------
	�X�g�[���[�v���r���[::�T�v�e�L�X�g
	�`��
-------------------------------------------------------------------------*/
void CTStorySelectPreview::CTStoryBrief::Draw()
{
	if(m_sindex<0)return;

	if(disp_txt){
		RECT r = {320+(LONG)m_hideMove,265,640+(LONG)m_hideMove,480};
		g_draw.DrawBlueText(
							r,
							disp_txt,
							disp_len,
							DT_LEFT|DT_WORDBREAK,
							2
							);
	}

	//�e�L�X�g
	char* title = g_storylist.GetStoryName(m_sindex);
	if(title){
		g_system.DrawBMPTextEx( 
							620+m_hideMove,
							225,
							0.0f,
							title,
							0xFFFFFFFF,
							1.0f,
							1.0f,
							SYSBMPTXT_PROP | SYSBMPTXT_R2L);
	}
}



/*=======================================================================================

	�X�g�[���[�I����A�p�����[�^�����E�C���h�E

=========================================================================================*/

CTStoryParamWindow* CTStoryParamWindow::m_instance = NULL;

/*--------------------------------------------------------------------------------------
	������
----------------------------------------------------------------------------------------*/
void CTStoryParamWindow::Initialize()
{
	int i;
	m_instance = this;

	CTWindowBase::Initialize();

	//�E�C���h�E�p�����[�^������
	m_winwidth		= 620.0f;
	m_winheight		= 180.0f;
	m_winleft		= 10.0f;
	m_wintop		= 270.0f;
	m_wincolor		= 0xBBFFFFFF;

	//�~�j��̓ǂݍ���(�L�����I��p)
	char *filepath = new char [MAX_PATH];
	m_miniface = new MYSURFACE* [g_charlist.GetCharacterCount()];
	for(i=0;i<g_charlist.GetCharacterCount();i++){
		m_miniface[i]=NULL;
		sprintf(filepath,"%s\\face1",g_charlist.GetCharacterDir(i));
		m_miniface[i] = g_draw.CreateSurfaceFrom256Image(filepath);
	}
	delete [] filepath;

	//�L�����N�^�����X�g�[���[���擾
	//SELECT�^�X�N�Q�b�g
	CTStorySelect* csselect = dynamic_cast<CTStorySelect*>(g_system.GetCurrentMainTask());
	if(!csselect){
		g_system.ReturnTitle();
		return;
	}
	m_sinfo = &csselect->m_sinfo;
	if(!(*m_sinfo)){
		g_system.ReturnTitle();
		return;
	}
	m_num = (*m_sinfo)->cnum;

	//Lock�t���O����
	for(UINT i=0;i<m_num;i++){
		m_lock[i][0] = ((*m_sinfo)->characters[i] < 0) ? FALSE : TRUE;
		if(m_lock[i][0])
		{
			m_lock[i][1] = TRUE;
			m_lock[i][2] = ((*m_sinfo)->opttype[i] == Opt_UserSelect) ? FALSE : TRUE;
		}
		else{
			m_lock[i][1] = FALSE;
			m_lock[i][2] = FALSE;
		}
	}
	
	//�p�����[�^���Z�b�g
	m_crnt_cindex = 0;
	m_crnt_param = 0;
	m_state = state_start;
	OkCheck();

}

/*--------------------------------------------------------------------------------------
	���s
----------------------------------------------------------------------------------------*/
BOOL CTStoryParamWindow::Execute(DWORD time)
{
	CTWindowBase::Execute(time);

	switch(m_state)
	{
	case state_start:	Exec_Start();break;
	case state_exec:	Exec_Exec();break;
	case state_wait:	Exec_Wait();break;
	case state_hide:	return Exec_Hide();
	}
	return TRUE;
}

//�o��������
void CTStoryParamWindow::Exec_Start()
{
	if(m_ratio==1.0f)m_state = state_exec;
}

//���펞����
void CTStoryParamWindow::Exec_Exec()
{
	DWORD keysta = g_input.GetKey(m_keyindex,0);
	
	if(keysta & KEYSTA_BA2){
		switch(m_crnt_param){
		case 0://�L�����N�^�I����
			{
				if(!(m_lock[m_crnt_cindex][m_crnt_param])){
					CCharSelecter* pchsel = new CCharSelecter;
					pchsel->SetPriority( m_draw_priority -1);
					pchsel->SetKeyInputIndex( m_keyindex );
					pchsel->cidx = m_crnt_cindex;
					pchsel->m_miniface = m_miniface;
					for(UINT i=0;i<m_num;i++)
					{
						//���łɑI������Ă���L�����N�^��I���ł��Ȃ�����
						if(i!=m_crnt_cindex && (*m_sinfo)->characters[i]>=0){
							pchsel->AddSelected( (*m_sinfo)->characters[i] );
						}
					}
					pchsel->SetInitialValue((*m_sinfo)->characters[m_crnt_cindex]);
					pchsel->SetPos(320,380);
					pchsel->SetHeight(10);
					g_system.AddTask( pchsel );
					ToWaitMode();
				}
				return;
			}break;
		case 1://�J���[�I����
			{
				if(!(m_lock[m_crnt_cindex][m_crnt_param])
						&& (*m_sinfo)->characters[m_crnt_cindex]>=0)//�L�����N�^���I������Ă��Ȃ��ꍇ�͑I�������Ȃ�
				{
					CColorSelecter* pcosel = new CColorSelecter;
					pcosel->SetPriority( m_draw_priority -1);
					pcosel->m_keyindex = m_keyindex ;
					pcosel->cidx = m_crnt_cindex;
					pcosel->SetPos(
							115.0f+75.0f+150.0f*m_crnt_cindex,
							m_wintop	+55+30*m_crnt_param);
					pcosel->SetInitialValue((*m_sinfo)->color[m_crnt_cindex]);
					g_system.AddTask( pcosel );
					ToWaitMode();
				}
				return;
			}break;
		case 2://�I�v�V�����I����
			{
				if(!(m_lock[m_crnt_cindex][m_crnt_param])
						&& (*m_sinfo)->characters[m_crnt_cindex]>=0)//�L�����N�^���I������Ă��Ȃ��ꍇ�͑I�������Ȃ�
				{
					COptionSelecter* popsel = new COptionSelecter;
					popsel->SetPriority( m_draw_priority -1);
					popsel->SetCIndex(m_crnt_cindex);
					popsel->SetAndShow((*m_sinfo)->characters[m_crnt_cindex],m_keyindex);
					popsel->SetPos(
							50,
							180);
					popsel->SetInitialValue( (*m_sinfo)->option[m_crnt_cindex] );
					g_system.AddTask( popsel );
					ToWaitMode();
				}
				return;
			}break;
		case 3://OK�{�^��
			{
				if(m_ok){
					//SELECT�^�X�N�Q�b�g
					CTStorySelect* csselect = dynamic_cast<CTStorySelect*>(g_system.GetCurrentMainTask());
					if(!csselect)return;
					csselect->OnParamSelectComplete();
					m_state = state_hide;
				}
				return;
			}break;
		}
		return;
	}

	if(keysta & KEYSTA_ALEFT2){			//��
		if(m_crnt_cindex==0)m_crnt_cindex=m_num-1;
		else m_crnt_cindex--;
	}
	else if(keysta & KEYSTA_ARIGHT2){	//��
		m_crnt_cindex++;
		if(m_crnt_cindex==m_num)m_crnt_cindex=0;
	}
	else if(keysta & KEYSTA_UP2){		//��
		if(m_crnt_param==0)m_crnt_param=3;
		else m_crnt_param--;
	}
	else if(keysta & KEYSTA_DOWN2){		//��
		m_crnt_param++;
		if(m_crnt_param==4)m_crnt_param=0;
	}


	OkCheck();
}

//�ҋ@������
void CTStoryParamWindow::Exec_Wait()
{
	//�Ȃ���Ȃ��H
}

//����������
BOOL CTStoryParamWindow::Exec_Hide()
{
	return (GetDrawPriority()<0) ? FALSE : TRUE;
}

void CTStoryParamWindow::ToWaitMode()
{
	m_state = state_wait;
}

/*--------------------------------------------------------------------------------------
	��n��
----------------------------------------------------------------------------------------*/
void CTStoryParamWindow::Terminate()
{
	//�~�j��̔j��
	for(int i=0;i<g_charlist.GetCharacterCount();i++){
		RELSURFACE(m_miniface[i]);
	}
	delete [] m_miniface;

	m_instance = NULL;

	CTWindowBase::Terminate();
}

/*--------------------------------------------------------------------------------------
	�`��
----------------------------------------------------------------------------------------*/
void CTStoryParamWindow::Draw()
{
	//���n�`��
	CTWindowBase::CalcDrawParameters();
	CTWindowBase::Draw();

	//�e�L�X�g�`��
	float x = 10;
	float xstep = 150;
	float ymgn = 20.0f;
	float y[] = {
					ymgn + 20.0f,
					ymgn + 20.0f + 30.0f,
					ymgn + 20.0f + 60.0f,
					ymgn + 20.0f + 100.0f,
				};

	DWORD txtcol = 0xFF888888;

	//Char1-3
	DrawText(x+xstep*1,ymgn,0.0f,"Char1"	,txtcol,0.75f,0.5f,SYSBMPTXT_PROP);
	DrawText(x+xstep*2,ymgn,0.0f,"Char2"	,txtcol,0.75f,0.5f,SYSBMPTXT_PROP);
	DrawText(x+xstep*3,ymgn,0.0f,"char3"	,txtcol,0.75f,0.5f,SYSBMPTXT_PROP);
	//param name
	DrawText(x+xstep-20.0f,y[0],0.0f,"Character"	,txtcol,0.75f,0.5f,SYSBMPTXT_PROP|SYSBMPTXT_R2L);
	DrawText(x+xstep-20.0f,y[1],0.0f,"Color"		,txtcol,0.75f,0.5f,SYSBMPTXT_PROP|SYSBMPTXT_R2L);
	DrawText(x+xstep-20.0f,y[2],0.0f,"Option"		,txtcol,0.75f,0.5f,SYSBMPTXT_PROP|SYSBMPTXT_R2L);
	x+=xstep;

	char *tmp = new char[16];
	int j;
	for(UINT i=0;i<m_num;i++)
	{
		//�L�����N�^�C���f�b�N�X
		j=0;
		if((*m_sinfo)->characters[i]<0){
			DrawText(x,y[j],0.0f,"Not Selected",GetDrawColor(i,j),0.6f,1.0f,SYSBMPTXT_PROP);
		}
		else{
			sprintf(tmp,"<%d>",(*m_sinfo)->characters[i]);
			DrawText(x,y[j],0.0f,tmp,GetDrawColor(i,j),1.0f,1.0f,SYSBMPTXT_PROP);
		}
		//�L�����N�^�J���[
		j=1;
		if((*m_sinfo)->color[i]<0){
			DrawText(x,y[j],0.0f,"Not Selected",GetDrawColor(i,j),0.6f,1.0f,SYSBMPTXT_PROP);
		}
		else{
			sprintf(tmp,"<%d>",(*m_sinfo)->color[i]);
			DrawText(x,y[j],0.0f,tmp,GetDrawColor(i,j),1.0f,1.0f,SYSBMPTXT_PROP);
		}
		//�L�����N�^�I�v�V����
		j=2;
		switch((*m_sinfo)->opttype[i]){
		case Opt_Random:
			{
				DrawText(x,y[j],0.0f,"Random Option",GetDrawColor(i,j),0.5f,1.0f,SYSBMPTXT_PROP);
			}break;
		case Opt_UserSelect:
			{
				sprintf(tmp,"<%8X>",(*m_sinfo)->option[i]);
				DrawText(x,y[j],0.0f,tmp,GetDrawColor(i,j),0.6f,1.0f,SYSBMPTXT_PROP);
			}break;
		case Opt_Fix:
			{
				sprintf(tmp,"<%8X,fix>",(*m_sinfo)->option[i]);
				DrawText(x,y[j],0.0f,tmp,GetDrawColor(i,j),0.75f,1.0f,SYSBMPTXT_PROP);
			}break;
		}

		x+=xstep;
	}
	delete [] tmp;

	//OK
	DrawText(600,y[3],0.0f,
			m_ok ? "OK" : "Cannot Start",
			GetDrawColor(m_crnt_cindex,3),
			m_ok ? 1.5f : 1.0f,
			1.0f,SYSBMPTXT_PROP|SYSBMPTXT_R2L
			);
}

//�`�掞�e�L�X�g�J���[�擾
DWORD CTStoryParamWindow::GetDrawColor(UINT cidx,UINT param_idx)
{
	const DWORD lok1 = 0xFFFF0000;
	const DWORD lok2 = 0xFFFF6666;
	const DWORD blk1 = 0xFF000000;
	const DWORD blk2 = 0xFF666666;
	BOOL selected = (m_crnt_cindex==cidx && m_crnt_param==param_idx);

	if(param_idx==3){
		if(m_ok){
			return selected ? blk1 : blk2;
		}
		else{
			return selected ? lok1 : lok2;
		}
	}

	if(m_lock[cidx][param_idx]){
		return selected ? lok1 : lok2;
	}
	else{
		return selected ? blk1 : blk2;
	}
}


/*--------------------------------------------------------------------------------------
	�I���I������OK���ǂ����`�F�b�N
----------------------------------------------------------------------------------------*/
void CTStoryParamWindow::OkCheck()
{
	m_ok = TRUE;//��
}


/*--------------------------------------------------------------------------------------
	�T�u�^�X�N�I�������A����
----------------------------------------------------------------------------------------*/
void CTStoryParamWindow::EndSub()
{
	if(m_instance){
		m_instance->m_state = state_exec;
	}
}

/*=======================================================================================

	�X�g�[���[�I����A�p�����[�^�����E�C���h�E�@-�@�L�����I���T�u�^�X�N

=========================================================================================*/

void CTStoryParamWindow::CCharSelecter::Terminate()
{
	CTCharacterRing::Terminate();
	CTStoryParamWindow::EndSub();//�e�^�X�N���A
}

BOOL CTStoryParamWindow::CCharSelecter::Execute(DWORD time)
{
	if(m_state==CTCRS_HideComplete)return FALSE;
	return CTCharacterRing::Execute(time);
}

/*--------------------------------------------------------------------------------------
	�{�^���������ꂽ�Ƃ��̏���
----------------------------------------------------------------------------------------*/
void CTStoryParamWindow::CCharSelecter::OnButtonDown(DWORD key)
{
	//����
	if(key & KEYSTA_BA2){//�uA�v����
		if(!IsSelected(GetSelected())){
			Hide();
		}
	}
}


/*--------------------------------------------------------------------------------------
	�I��ύX������
----------------------------------------------------------------------------------------*/
void CTStoryParamWindow::CCharSelecter::Notify()
{
	//SELECT�^�X�N�Q�b�g
	CTStorySelect* csselect = dynamic_cast<CTStorySelect*>(g_system.GetCurrentMainTask());
	if(!csselect){
		return;
	}

	//�ύX��ʒm
	csselect->SelectChangeCharacter(cidx,GetSelected());
}

/*--------------------------------------------------------------------------------------
	�\������A�C�R���摜���擾�iDraw����R�[���j
----------------------------------------------------------------------------------------*/
MYSURFACE* CTStoryParamWindow::CCharSelecter::GetIcon(UINT ridx,UINT idx)
{
	if(!m_miniface)return NULL;//�_����

	int sidx = g_charlist.RingIndex2SerialIndex(ridx,idx);
	if(sidx>g_charlist.GetCharacterCount())return NULL;
	return m_miniface[sidx];
}

/*--------------------------------------------------------------------------------------
	�����l�ݒ�B
	�n�����̂͑S�̂ł̒ʂ��ԍ��Ȃ̂Ō��\�߂�ǂ�����
----------------------------------------------------------------------------------------*/
void CTStoryParamWindow::CCharSelecter::SetInitialValue(int chr)
{
	if(chr<0){
		m_selected_index = m_selected_ring = 0;
	}
	else{
		BOOL find=FALSE;
		for(int r=0;r<g_charlist.GetRingNum();r++){
			for(int i=0;i<g_charlist.GetCharacterCountRing(r);i++){
				if(g_charlist.RingIndex2SerialIndex(r,i)==chr){
					m_selected_ring = r;
					m_selected_index = i;
					find = TRUE;
					break;
				}
			}
			if(find)break;
		}
		if(!find)m_selected_index = m_selected_ring = 0;
	}
	Notify();
}

/*=======================================================================================

	�X�g�[���[�I����A�p�����[�^�����E�C���h�E�@-�@�J���[�I���T�u�^�X�N

=========================================================================================*/
void CTStoryParamWindow::CColorSelecter::Initialize()
{
	CTWindowBase::Initialize();
	Open();

	m_winOpenTime=10.0f;
	m_winCloseTime=10.0f;
	
	m_winwidth = 120.0f;
	m_winheight = 100.0f;

	m_end_flag = FALSE;
}

BOOL CTStoryParamWindow::CColorSelecter::Execute(DWORD time)
{
	DWORD keysta = g_input.GetKey(m_keyindex,0);
	if(m_ratio==1.0f){
		if(keysta&KEYSTA_BA2){
			Close();
			m_end_flag = TRUE;
		}
		if(keysta & KEYSTA_UP2){
			color--;
			if(color==0)color = MAXNUM_CHARACTERCOLOR;
			Notify();
		}
		if(keysta & KEYSTA_DOWN2){
			color++;
			if(color>MAXNUM_CHARACTERCOLOR)color = 1;
			Notify();
		}
	}

	if(m_ratio==0.0f && m_end_flag){
		return FALSE;
	}
	return CTWindowBase::Execute(time);
}

void CTStoryParamWindow::CColorSelecter::Terminate()
{
	CTWindowBase::Terminate();
	CTStoryParamWindow::EndSub();//�e�^�X�N���A
}

void CTStoryParamWindow::CColorSelecter::Draw()
{
	//���n�`��
	CTWindowBase::CalcDrawParameters();
	CTWindowBase::Draw();

	const float ystep = 30;
	float x = 10;
	float y = 10;

	char *tmp = new char[16];
	DWORD col;
	for(int i=1;i<=MAXNUM_CHARACTERCOLOR;i++){
		col = (i==color) ? 0xFF000000 : 0xFF888888;
		sprintf(tmp,"Color%d",i);
		DrawText(x,y,0.0f,tmp,col,1.0f,0.7f);
		y+=ystep;
	}
	delete [] tmp;
}

void CTStoryParamWindow::CColorSelecter::SetInitialValue(int col)
{
	if(col<0)col=1;
	color=col;

	Notify();
}

void CTStoryParamWindow::CColorSelecter::Notify()
{
	//SELECT�^�X�N�Q�b�g
	CTStorySelect* csselect = dynamic_cast<CTStorySelect*>(g_system.GetCurrentMainTask());
	if(!csselect){
		return;
	}

	//�ύX��ʒm
	csselect->SelectChangeColor(cidx,color);
}

/*=======================================================================================

	�X�g�[���[�I����A�p�����[�^�����E�C���h�E�@-�@�I�v�V�����I���T�u�^�X�N

=========================================================================================*/

void CTStoryParamWindow::COptionSelecter::EndSelect()
{
	// �p���łȂ�Ƃ�������
	if (GetSelecter()->GetFavorite() == 0 && GetSelecter()->GetState() != 0xFFFFFFFF && !custom_mode){
		// CUSTOM���I�΂ꂽ�̂ŁAm_customselect�̏������[�h�Ɉڍs
		m_customselect.SetAndShow(m_cindex, m_keyIndex);
		m_customselect.SetPos(offset_x, m_top);
		custom_mode = TRUE;
	}
	else{
		//SELECT�^�X�N�Q�b�g
		CTStorySelect* csselect = dynamic_cast<CTStorySelect*>(g_system.GetCurrentMainTask());
		if(!csselect){
			return;
		}

		//�ύX��ʒm
		if (custom_mode)
			csselect->SelectChangeOption(m_cindex2,m_customselect.GetSelecter()->GetSettings());
		else
			csselect->SelectChangeOption(m_cindex2,m_selecter->GetSettings());

		CTStoryParamWindow::EndSub();//�e�^�X�N���A
	}
}

void CTStoryParamWindow::COptionSelecter::Draw()
{
	//���n�`��F
	DWORD color ;
	color = (DWORD)(m_tick*240);
	color &= 0x000000FF;
	color <<= 24;
	color |= 0x00FFFFFF;

	MYVERTEX3D* vb;

	if ( !g_draw.pMyVertex || FAILED(g_draw.pMyVertex->Lock(0, 0, (BYTE**)&vb, D3DLOCK_DISCARD)) )
		return;

	vb[0].color = color;
	vb[1].color = color;
	vb[2].color = color;
	vb[3].color = color;

	vb[0].z = 0.0f;
	vb[1].z = 0.0f;
	vb[2].z = 0.0f;
	vb[3].z = 0.0f;

	vb[0].x = -0.5f;
	vb[1].x = -0.5f;
	vb[2].x = 2.5f;
	vb[3].x = 2.5f;

	vb[0].y = m_top;
	vb[1].y = m_top + GetHeight();
	vb[2].y = m_top;
	vb[3].y = m_top + GetHeight();

	for(int i=0;i<4;i++)vb[i].y /= 240.0f;
	if (g_draw.pMyVertex) g_draw.pMyVertex->Unlock();

	g_draw.EnableZ(FALSE,FALSE);
	g_draw.SetTransform(FALSE);
	g_draw.d3ddev->SetTexture(0,NULL);
	g_draw.d3ddev->SetStreamSource(0, g_draw.pMyVertex, sizeof(MYVERTEX3D));
	g_draw.d3ddev->SetVertexShader(FVF_3DVERTEX);
	g_draw.d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
	g_draw.EnableZ();

	CTSimpleOptionSelecter::Draw();
}

BOOL CTStoryParamWindow::COptionSelecter::Execute(DWORD time)
{
	float anmSpd = 0.08f;

	// �ȉ�������C���A�悢�q�̓}�l���Ȃ��B
	if (m_customselect.State() == CTOPTSS_Ready)
		customend = false;	// ���̏������I������u�Ԃɗ��Ă邽�߂̃t���O

	if (custom_mode && m_customselect.State() == CTOPTSS_Hide){		// CUSTOM�I�����I������Ƃ�
		if(!customend){
			customend = true;
			EndSelect();
		}

		// ������w�i������
		m_tick -= anmSpd;
		if(m_tick<0){
			m_tick = 0;
		}
	}

	return CTSimpleOptionSelecter::Execute(time);
}