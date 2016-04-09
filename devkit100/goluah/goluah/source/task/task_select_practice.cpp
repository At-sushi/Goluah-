/*=======================================================================================

	���K���[�h �L�����Z��

=========================================================================================*/

#include "stdafx.h"
#include "global.h"
#include "task_select_practice.h"
#include "task_practice.h"

void CCharacterSelectPractice::Initialize()
{
	//�p�����[�^���Z�b�g
	charsel_ok[0]=charsel_ok[1]=FALSE;
	optsel_ok[0] = optsel_ok[1] = TRUE;
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
	g_sound.BGMPlay(_T(".\\system\\bgm\\choice"));

	//���R���f�B�V�����Z���N�^�[�o�^
	CTConditionSelecter *csel = new CTConditionSelecterPractice;
	csel->SetPriority( m_pri_firstSetting );
	g_system.AddTask(csel);

	//���q�[�`��N���X�o�^
	m_bg = new CTCharacterSelectBG();
	m_bg->SetPriority(m_pri_background);
	g_system.AddTask(m_bg);
}


void CCharacterSelectPractice::CreateNextTask()
{
	g_system.AddTask( new CBattlePractice );
}


/*!
*	�b��.
*/

const int CTConditionSelecterPractice::m_hp_ratio[5] = {80,100,10,30,50};

void CTConditionSelecterPractice::Initialize()
{
	CTConditionSelecter::Initialize();

	for(int j=0;j<2;j++){
		for(int i=0;i<MAXNUM_TEAM;i++)
		{
			// �b��[�u
			if (m_assign[j][i] == CASSIGN_COM)
				m_assign[j][i] = CASSIGN_STABLE;
		}
	}
}

BOOL CTConditionSelecterPractice::Execute(DWORD time)
{
/*	static BYTE m_type;						//gobject.h , TAISENKEISIKI_�`�Q�� BYTE�ő����
	static BYTE m_assign[2][MAXNUM_TEAM];	//���蓖��
	static int m_limit_time_index;			//���̃C���f�b�N�X
	static int m_limit_time[5];				//45,60,99,120,��  ,  const	

	m_limit_time_index = 4;
	m_type = TAISENKEISIKI_GOCYAMAZE;
	m_assign[0][0] = 0;
	m_assign[0][1] = CASSIGN_NONE;
	m_assign[0][2] = CASSIGN_NONE;
	m_assign[1][0] = CASSIGN_COM;
	m_assign[1][1] = CASSIGN_NONE;
	m_assign[1][2] = CASSIGN_NONE;

	CCharacterSelect* ccselect = dynamic_cast<CCharacterSelect*>(g_system.GetCurrentMainTask());
	ccselect->OnConditionDecided(this);*/

	// �㏑��
	if (m_state == CTCoS_Hide && m_counter == 0){
		g_battleinfo.SetLimitTime(m_hp_ratio[m_limit_time_index]);

	}

	return CTConditionSelecter::Execute(time);
}


/*---------------------------------------------------------------------------------------
	�`��
-----------------------------------------------------------------------------------------*/
void CTConditionSelecterPractice::Draw()
{
	CTBeltBase::CalcTopBottom();
	CTBeltBase::Draw();//�I�r�E�^�C�g���`��

	const float x=40.0f;					//�\�����ʒu
	const float txt_xr=1.00f;			//�e�L�X�g�Ey�g�嗦
	const float txt_yr=1.00f*m_ratio;	//�e�L�X�g�Ex�g�嗦
	float y = 100.0f;
	const float ystep = 35.0f;
	float shiftX=0.0f;	//TEAM2���s�p

	int i,j;

	if(m_ratio<0.01f)return;

	char *tstr = new char[64];

	//�ΐ�`��
	char *typenames[]={
		_T("Cooperation"),
		_T("Marvel Like"),
		_T("K.O.F. Like")
	};
	sprintf(tstr,_T("Mode : %s"),typenames[m_type-1]);
	g_system.DrawBMPTextEx(x,y,0.0f,
				tstr,
				TxtCol(0),txt_xr,txt_yr,SYSBMPTXT_PROP);

	//HP����
	sprintf(tstr,_T("HP : %d%%"),m_hp_ratio[m_limit_time_index]);
	g_system.DrawBMPTextEx(x+300,y,0.0f,
				tstr,
				TxtCol(1),txt_xr,txt_yr,SYSBMPTXT_PROP);
	y += ystep;
	y += ystep;

	sprintf(tstr,_T("TEAM1"));
	g_system.DrawBMPTextEx(x,y,0.0f,tstr,0xFF5237FF,txt_xr,txt_yr,SYSBMPTXT_PROP);
	sprintf(tstr,_T("TEAM2"));
	g_system.DrawBMPTextEx(x+300.0f,y,0.0f,tstr,0xFFFF3752,txt_xr,txt_yr,SYSBMPTXT_PROP);
	y += ystep;

	//TEAM*-x : �`
	for(j=0;j<2;j++){
		for(i=0;i<MAXNUM_TEAM;i++)
		{
			sprintf(tstr,_T("%d : "),i+1);
			if(m_assign[j][i]&CASSIGN_SPECIFIC)//����
			{
				switch(m_assign[j][i]){
				case CASSIGN_COM:	sprintf(&tstr[strlen(tstr)],_T("Computer"));break;
				case CASSIGN_NONE:	sprintf(&tstr[strlen(tstr)],_T("--None--"));break;
				case CASSIGN_STABLE:	sprintf(&tstr[strlen(tstr)],_T("Stop"));break;
				default:			sprintf(&tstr[strlen(tstr)],_T("Error?"));
				}
			}
			else{
				sprintf(&tstr[strlen(tstr)],_T("Player%d"),m_assign[j][i]+1);
			}
			if(j==0)	//TEAM1
				shiftX=40.0f;
			else	//TEAM2
			{
				y=100.0f + (i+3)*ystep;
				shiftX=340.0f;
			}
			g_system.DrawBMPTextEx(x+shiftX,y,0.0f,
				tstr,TxtCol(j*MAXNUM_TEAM+i+2),txt_xr,txt_yr,SYSBMPTXT_PROP);
			y += ystep;
		}
		y += ystep;
	}

	//"OK"�`��
	g_system.DrawBMPTextEx(640.0f-x,y,0.0f,
				m_ok ? _T("OK") : m_str_error,
				TxtCol(2*MAXNUM_TEAM+2),txt_xr,txt_yr,SYSBMPTXT_PROP|SYSBMPTXT_R2L);
	y += ystep;
	//"return title"
	g_system.DrawBMPTextEx(640.0f-x,y,0.0f,
				_T("Return Title"),
				TxtCol(2*MAXNUM_TEAM+3),txt_xr,txt_yr,SYSBMPTXT_PROP|SYSBMPTXT_R2L);

	delete [] tstr;
}

void CTConditionSelecterPractice::Change(BOOL key)
{
	if(m_selected>1)
	{
		int sel1 = (m_selected-2)/MAXNUM_TEAM;
		int sel2 = (m_selected-2)%MAXNUM_TEAM;

		// Stable�������g�p��
		if(m_assign[sel1][sel2]&CASSIGN_SPECIFIC){
			switch(m_assign[sel1][sel2]){
			case CASSIGN_COM:
				m_assign[sel1][sel2] = key ? CASSIGN_STABLE : MAXNUM_KEYI-1 ;
				break;
			case CASSIGN_NONE:
				m_assign[sel1][sel2] = key ? 0 : CASSIGN_STABLE ;
				break;
			case CASSIGN_STABLE:
				m_assign[sel1][sel2] = key ? CASSIGN_NONE : CASSIGN_COM ;
				break;
			}
			Check();
			return;
		}
	}

	// ��L�ύX�����Ȃ������ꍇ
	CTConditionSelecter::Change(key);
}
