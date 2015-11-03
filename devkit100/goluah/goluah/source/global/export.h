
/*!
*	@file 
*	@brief DLL�ɑ΂��Ē񋟂���֐��̒�`
*/
#ifndef _EXPORT_H_
#define _EXPORT_H_

#include "gobject.h"		//�֐��|�C���^�Z�b�g�\���̂̒�`���K�v
#include "battleTaskBase.h"	//�퓬�^�X�N

/*!
*	@ingroup battle
*	@brief DLL�ւ̊֐��|�C���^�񋟃N���X
*
*	DLL�ɑ΂��Ē񋟂���֐��|�C���^�܂Ƃ߃N���X
*	�قƂ�ǂ̊֐���static�B
*	ver0.99�̓r������G���[���o�p�Ɂu�Ō�ɌĂяo���ꂽ�֐��v���L������@�\�����܂����B
*/
class CExport{

public:
	CExport();
	void Initialize();			//!< �֐��|�C���^���\���̂ɋl�ߍ���
	static CBattleTaskBase* GetCurrentBattleTask();//!< ���݂̐퓬�^�X�N���擾����
	static char* last_funcname;
	static BOOL  func_in;

	DI_FUNCTIONS_S fpack_s;
	DI_FUNCTIONS_O fpack_o;
	DI_FUNCTIONS_D fpack_d;

	//�� �V�X�e���֘A
	static DWORD  GetKey(DWORD tid,DWORD index);		//!< �L�[����get
	static int    SeekKey(DWORD tid,int index,int numseek,DWORD key);//!< �L�[����get(2)
	static void   SetKatiSerif(DWORD tid,char* serif);	//!< �����䎌�ݒ�
	static void   AddEffect(DWORD eid,int prm1,int prm2,DWORD prm3);//!< �G�t�F�N�g����
	static void   PlaySystemSound(DWORD sid);			//!< wav�Đ�
	static LPVOID LoadMySound(char* filename);			//!< wav�ǂݍ���
	static void   PlayMySound(LPVOID pdsb);				//!< wav�Đ�
	static void   KillMySound(LPVOID pdsb);				//!< wav����
	static DWORD  Message2System(DWORD oid,DWORD msg,DWORD prm);//!< ���b�Z�[�W
	static char*  GetEnemyName(DWORD tid);				//!< �G�̖��O�擾
	static char*  GetCharacterName(DWORD oid);			//!< �w��L�����̖��O�擾
	static DWORD  GetKeyInput(DWORD oid);				//!< �w��L�����̃L�[ID�擾
	static double GetDisplayCenterX();					//!< �f�B�X�v���C���Sget
	static double GetDisplayCenterY();					//!< �f�B�X�v���C���Sget
	static DWORD  GetTeamNum();							//!< ���Ή���
	static DWORD  GetTaisenKeisiki();					//!< �ΐ�`�����擾
	static DWORD  StrikerOK(DWORD tid,BOOL next);		//!< �X�g���C�J�[�₢���킹
	static BYTE	  GetEnemyFace(DWORD tid);				//!< �t�F�C�X�ύX�l(�ʏ�0)���擾
	static DWORD  GetTeamNum2(DWORD tid);				//!< �`�[���l�����擾���܂�
	static DWORD  TeamID2EnemyTeamID(DWORD tid);		//!< ���`�[��ID��G���̃`�[��ID�ɕϊ����܂�
	static BOOL	  IsNetwork();							//!< �l�b�g���[�N�ΐ킩�ǂ���
	static void   PushTag(const char* tag);				//!< �^�O�ݒ�
	static void   PopTag();								//!< �^�O����
	static void   LogError(const char* str);			//!< ���O
	static void   LogWarning(const char* str);			//!< ���O
	static void   LogDebug(const char* str);			//!< ���O
	static void   LogInfo(const char* str);				//!< ���O
	static void   BGMPause(void);
	static void   BGMResume(void);
	static int    GetGameSpeed();						//!< fps�擾
	static DWORD  GetMaxWin();							//!< ���|�C���g���擾

	//���@�I�u�W�F�N�g�֘A
	static DWORD CreateObject();						//!< �I�u�W�F�N�g����
	static void  DeleteObject(DWORD oid);				//!< �I�u�W�F�N�g����
	static BOOL  CatchObject(DWORD eoid,LPVOID cy);		//!< �����Ƃ��ő��������
	static void* GetObjectInfo(DWORD oid);				//!< ���
	static void  AddDamage(DWORD oid,DWORD eoid,int x,int y);//!< (��ɓ����̂Ƃ�)�����I�Ƀ_���[�W��^����
	static void  ActIDChanged(DWORD oid);				//!< �s��ID�ω����̏���������
	static void* GetActiveCharacter(DWORD tid);			//!< �A�N�e�B�u�L�����N�^�[�擾
	static void  SuicideGObject(DWORD oid);				//!< �I�u�W�F�N�g����2
	static DWORD GetMaaiH(DWORD oid,DWORD eoid);		//!< �ԍ���Get(����)
	static DWORD GetMaaiV(DWORD oid,DWORD eoid);		//!< �ԍ���Get(����)
	static DWORD GetCharacterID(DWORD tid,DWORD index);	//!< �L�����N�^�[��ID���Q�b�g
	static DWORD GetComLevel(DWORD oid);				//!< com���x���擾
	static void  SetComReach(DWORD oid,DWORD idx,int length);		//!< com���[�`�w��
	static void  SetComAct(DWORD oid,DWORD aid,int delay);			//!< com�s���J�ڎw��
	static DWORD CreateObjectFx();									//!< �G�t�F�N�g�I�u�W�F�N�g����
	static BOOL  ObjIsLocal(DWORD oid);								//!< ���[�J�����ǂ���
	static void  SetComRange(DWORD oid,DWORD idx);					//!< com��{�����W�w��
	static DWORD GetHitCount(DWORD oid);				//!< �A���Z�q�b�g���i��R���{���j�擾
	static DWORD GetSexyDamage(DWORD oid);				//!< �A���Z�~�σ_���[�W�i��R���{�_���[�W�j�擾

	//���@�`��֘A�̊֐��̒�`
	static LPVOID GetD3D();								//!< IDirect3D* �̎擾
	static LPVOID GetD3DD();							//!< IDirect3DDevice* �̎擾
	static void   LoadCellDat(char* filename,LPVOID cdat,LPVOID rdat,LPVOID hdat);	//!< �Z���f�[�^�ǂݍ��݊֐�
	static LPVOID LoadBmp(char* filename,char* palname);//!< �r�b�g�}�b�v�ǂݍ��݊֐�
	static void   UnloadBmp(LPVOID psuf);				//!< �r�b�g�}�b�v��n���֐�
	static void   CellDraw(LPVOID* ddsa,LPVOID cdat,LPVOID rdat,DWORD cn,int x,int y,float z,int rot,BOOL revx,BOOL revy,DWORD color,float magx,float magy,BOOL shadowed = 1);//!< �Z���`��֐�
	static void   Blt(void* dds,int x,int y,RECT r,double magx,double magy,BOOL revx,BOOL revy,float z,DWORD color);//!< Blt2
	static void   Blt3D(void* dds,RECT src,MYRECT3D dst,DWORD color);//!< Blt3
	static void   SetTransform(BOOL b);					//!< ���W�ϊ��s����Đݒ�
	static void   SetBlendMode(DWORD m);				//!< �������̕��@��ύX
	static void   SetParentMatrix(LPVOID mat,BOOL root,LPVOID matprv);//!< �u�e�v�ϊ��s��ݒ�
	static DWORD  CreateCellDat(char* filename,LPVOID *cdat,LPVOID *rdat,LPVOID *hdat);//!< �Z���f�[�^�ǂݍ��݁Bver0.90�ȍ~�ł�LoadCellDat���炱����ɕύX����ׂ�
	static void   DestroyCellDat(LPVOID *cdat,LPVOID *rdat,LPVOID *hdat);//!< CreateCellDat�Ő������ꂽ�o�b�t�@���N���A
	static LPVOID LoadImage(char* filename, char* palname);//!< �摜�ǂݍ��݊֐��APNG�Ή��ɔ����ǉ��B
	static DWORD  CreateCellDat2(char* filename,LPVOID *cdat,LPVOID *rdat,LPVOID *hdat);//!< �Z���f�[�^�ǂݍ��݁i�Q�j�A���k�`���Ή��ɔ����ǉ��B
};



#endif//_EXPORT_H_