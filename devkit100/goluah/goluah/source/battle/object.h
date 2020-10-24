
/*============================================================================

	GOBJECT ���������N���X
	���b�Z�[�W���M�ƁA���b�Z�[�W�̃f�t�H���g����

==============================================================================*/
#pragma once

#include "gobject.h"
#include "config.h"
class CBattleTaskBase;

/*!
*	@brief GOBJECT���������N���X
*	@ingroup Battle
*
*	DLL����I�u�W�F�N�g�̐����v�����������Ƃ��ɓ����ł͂��̃N���X������A
*	���̃N���X��GOBJECT�\���̂��Ԃ����B
*	GOBJECT�������₷�����b�v���邱�ƂƁA�e�I�u�W�F�N�g���b�Z�[�W�̃f�t�H���g�������������̂��ړI�B
*	�ꕔ�̃f�t�H���g�s���֐��́Astatic��DLL���Ɍ��J�����B
*
*	�N���X�̐������ɁACBattleTaskBase����"���݂�DLL-ID"���擾����B
*	�܂��ADLL���̊֐��̌Ăяo���O��CBattleTaskBase��"���݂�DLL-ID"���A������ID�ɐݒ肷��B
*	����ɂ��A��{�I�ɂ͂ǂ̃I�u�W�F�N�g���ǂ�DLL�ō��ꂽ�����킩��B
*	(this�|�C���^���Ɠn���Ă������ق������낢��ł��Ă�������)
*
*	���l�ɁA�N���X�̐������� CBattleTaskBase����R���s���[�^���x���l(�Q�[����Փx)���擾����B
*	����ɂ��A�R���s���[�^���x�����L�����N�^�[���Ƃɐݒ�ł���悤�ɂȂ����B
*/
class CGObject
{
public:
	CGObject(DWORD object_id);
	~CGObject();

	DWORD Message(DWORD type,DWORD prm=0);
	DWORD MessageDirect(DWORD type,DWORD prm);
	DWORD TouchA();
	void ActionIDChanged(BOOL ck,BOOL force=FALSE);

	static bool ZCompare(CGObject *obj1,CGObject *obj2){return obj1->data.z > obj2->data.z;}

private:
	//�f�t�H���g����
	void Defmsg_Draw();
	DWORD Defmsg_TouchA();
	void Defmsg_Action();

	//!< �����蔻��p�o�E���f�B���O�{�b�N�X
	RECT boundingAttack[GCDMAX_RECTANGLES], boundingHit[GCDMAX_RECTANGLES], boundingKurai[GCDMAX_RECTANGLES];

public:
	static void dact_damages1(GOBJECT *pdat);
	static void dact_damages2(GOBJECT *pdat);
	static void dact_damages3(GOBJECT *pdat);
	static void dact_damagec1(GOBJECT *pdat);
	static void dact_damagec2(GOBJECT *pdat);
	static void dact_damagec3(GOBJECT *pdat);
	static void dact_damagej1(GOBJECT *pdat);
	static void dact_damagej2(GOBJECT *pdat);
	static void dact_damagej3(GOBJECT *pdat);
	static void dact_damages1a(GOBJECT *pdat);
	static void dact_damages2a(GOBJECT *pdat);
	static void dact_damages3a(GOBJECT *pdat);
	static void dact_damagec1a(GOBJECT *pdat);
	static void dact_damagec2a(GOBJECT *pdat);
	static void dact_damagec3a(GOBJECT *pdat);
	static void dact_guards1(GOBJECT *pdat);
	static void dact_guards2(GOBJECT *pdat);
	static void dact_guards3(GOBJECT *pdat);
	static void dact_guardc1(GOBJECT *pdat);
	static void dact_guardc2(GOBJECT *pdat);
	static void dact_guardc3(GOBJECT *pdat);
	static void dact_guardj1(GOBJECT *pdat);
	static void dact_guardj2(GOBJECT *pdat);
	static void dact_guardj3(GOBJECT *pdat);

	static void dact_down(GOBJECT *pdat);
	static void dact_down2(GOBJECT *pdat);
	static void dact_okiagari(GOBJECT *pdat);
	       void dact_finaldown(GOBJECT *pdat);
	static void dact_futtobi(GOBJECT *pdat);
	static void dact_bound(GOBJECT *pdat);
	static void dact_finalbound(GOBJECT *pdat);
	       void dact_finaldown2(GOBJECT *pdat);
	static void dact_futtobi2(GOBJECT *pdat);
	static void dact_tatakituke1a(GOBJECT *pdat);
	static void dact_tatakituke1b(GOBJECT *pdat);
	static void dact_tatakituke2a(GOBJECT *pdat);
	static void dact_tatakituke2b(GOBJECT *pdat);
	static void dact_tatakituke(GOBJECT *pdat);
	       void dact_kaitenfinish(GOBJECT *pdat);

	//!< �o�E���f�B���O�{�b�N�X�v�Z
	void ComputeBoundingBoxes() {
		const auto phdat = data.phdat;
		if (phdat) {
			for (int i = 0; i < GCDMAX_RECTANGLES; i++) {
				boundingHit[i] = {0, 0, 0, 0};

				// ���̂������v���Ԃ��Windows��API��������
				for (const auto& j : phdat[i].kas)
					UnionRect(boundingHit + i, boundingHit + i, &j);

				boundingKurai[i] = boundingHit[i];

				for (const auto& j : phdat[i].kurai)
					UnionRect(boundingKurai + i, boundingKurai + i, &j);

				boundingAttack[i] = boundingKurai[i];
				for (const auto& j : phdat[i].attack)
					UnionRect(boundingAttack + i, boundingAttack + i, &j);
			}
		}
	}

	const RECT& GetBoundingAttack(int val) const { return boundingAttack[val]; }
	const RECT& GetBoundingHit(int val) const { return boundingHit[val]; }
	const RECT& GetBoundingKurai(int val) const { return boundingKurai[val]; }

private:
	void dact_userhit1();
	void dact_userhit2();
	void dact_userguard1();
	void dact_userguard2();
	
	void ComCommand();								//!< �R���s���[�^�R�}���h����
	static void DmgMovex(GOBJECT *pdat,double mx);	//!< �_���[�W��炢���ʒu�ړ�
	static void Furimuki(GOBJECT *pdat);			//!< �U���������

//�ϐ�
public:
	GOBJECT		data;				//!< �f�[�^
	double		comguard;			//!< �R���s���[�^�K�[�h�m��
	int			siver;				//!< �_���[�W�H������Ƃ��̂��т�i�̂��莝�����ԁj
	CATCHYOU	cy;					//!< ���������
	DWORD		hitcount;			//!< �A���Z�q�b�g��
	DWORD		sexydamage;			//!< �A���Z�~�σ_���[�W
	DWORD		sexydamage_anim;	//!< �A���Z�~�σ_���[�W�i��ʕ\���p�j
	DWORD		sexydamage_haba;	//!< �A���Z�~�σ_���[�W�i��ʕ\���p�̑������j
	
	DWORD		userhit_attacker;	//!< ���[�U�[��`��炢�̏ꍇ�A�U����
	DWORD		userhit_id;			//!< ���[�U�[��`��炢�̏ꍇ�AID

	int			comreach[3];		//!< ���[�`. �Z����
	DWORD		comhint_aid;		//!< DLL����̍s���J�ږ���.�s��ID
	int			comhint_delay;		//!< DLL����̍s���J�ږ���.�f�B���C(�}�C�i�X�l�͖���)

	Config2_Difficulty	com_level;	//!< �R���s���[�^�̃��x��(��Փx)
	BYTE		dll_id;				//!< DLLID
	BYTE		nage_muteki_cnt;	//!< �N���オ��A�K�[�h�㓊�����G�J�E���^

	DWORD		object_id_original;	//!< �I�u�W�F�N�gID�i�`�F�b�N�p�j
	DWORD		comrange;			//!< COM�̊�{�ԍ���

protected:
	int comcounter;					//!< �R���s���[�^����p�J�E���^
	DWORD actid_prv;				//!< �O��s��ID
	CBattleTaskBase *battleTask;	//!< �퓬�^�X�N
	int actloop_count;				//!< �s��ID�̃��[�v��

	DWORD	comhint_counterprev;	//!< �O��̃J�E���^�[�A��r�p�B
	int		comhint_capacity;
};
