
/*=====================================================================================

	�L�����N�^�[����

	Goluah!! Copyright (C) 2001-2004 aki, 2014-2015 logger, 2004-2015 At-sushi

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

=======================================================================================*/

#include "character.h"		//�������[
extern CCharacterBase* CreateMorarael(CDI_CHARACTERINFO2* info);	//�������G��


CHARACTER_LOAD_OPTION option[] = {
	// �L�����N�^�[�̃I�v�V�����������ɏ����Ă��������B�i�ő�32�j
	// �Ȃ��A�I�v�V�����ƃI�v�V�����̊Ԃ̓R���}(,)�ŋ�؂��Ă��������B

	// �L�����e�F
	// �@{ �t���O, ���������߼��,�ˑ������߼��, �I�v�V������, ����|�C���g }
	{ OPTIONS_EXTRA_ATTACK		,OPTIONS_MORARAEL_MODE, 0, "Extra Attack"	, 3},
	{ OPTIONS_AUTO_GUARD		,OPTIONS_MORARAEL_MODE, 0, "Auto Guard"		, 5},
	{ OPTIONS_G_CUTTER_PLUS		,OPTIONS_MORARAEL_MODE, 0, "G.Cutter +"		, 4},
	{ OPTIONS_CHIBASIRI_PLUS	,OPTIONS_MORARAEL_MODE, 0, "Chibasiri +"	, 3},
	{ OPTIONS_2ND_STEP			,OPTIONS_MORARAEL_MODE, 0, "2nd-Step"		, 3},
	{ OPTIONS_STOCKABLE_GAUGE	,OPTIONS_MORARAEL_MODE, 0, "Stockable Gauge", 4},
	{ OPTIONS_MINIGUN_RELOADED	,OPTIONS_MORARAEL_MODE, OPTIONS_STOCKABLE_GAUGE,"Minigun -Reloaded-",6},
	{ OPTIONS_MORARAEL_MODE		,OPTIONS_MORARA_ALL   , 0, "Morarael-Mode",99},
};


//**************************************************************************************
//  Character Information
//�@�����ɃL�����N�^�[�̖��O���L�q���܂�
//**************************************************************************************

CCharacterInfo CharInfo("�������[",		// �L�����N�^�[�̖��O�i�ő�31�o�C�g�j
						CDI_VERSION,		// �L�����N�^�[DLL�C���^�[�t�F�C�X�̃o�[�W����
						option,				// �I�v�V�����\���̂ւ̃|�C���^
						sizeof(option) / sizeof(CHARACTER_LOAD_OPTION), // �I�v�V�����̐�
						15,					// �I�v�V�����ő�|�C���g��
						FALSE);				// �l�b�g�ΐ�Ή����ǂ��� �����̊g���̂��߂ɗp�ӂ���Ă���B���݂�0��n���Ȃ���΂Ȃ�Ȃ��i�΁j

char* CharDirectory = NULL;



//�L�����N�^�[�o�^�����i�ʏ�ʂ�j
extern "C" BOOL CharacterInfo(LPVOID info) 
{
	return CharInfo.DllCharacterInfo(info);
}

//�L�����N�^�[���������i�؂�ւ��j
extern "C" DWORD CreateCharacter(PVOID info) 
{
	CharInfo.DllCreateCharacter(info);
	CDI_CHARACTERINFO2 *cinfo = (CDI_CHARACTERINFO2*)info;
	
	CCharacterBase *pc;
	if(cinfo->options_flag&OPTIONS_MORARAEL_MODE){
		//�������G�����[�h
		pc = CreateMorarael((CDI_CHARACTERINFO2*)info);
	}
	else{
		//�������[���[�h
		pc = new CCharacter((CDI_CHARACTERINFO2*)info);
	}
	pc->InitializeCharacter();
	return(pc->GetObjectID());
}


