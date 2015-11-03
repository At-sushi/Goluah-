
/*!
*	@file
*   @brief config.dat , config2.dat �̃f�[�^�Ǘ���`
*
*	���̃t�@�C����goluah!!�{�̂�config.exe�̃v���W�F�N�g����Q�Ƃ���܂��B
*/
#pragma once

#define NUM_PAD			(6)
#define CONFIG_VERSION	(1200)

/*!
*	@ingroup global
*	@brief �L�[�R���t�B�M�����[�V�����f�[�^
*/
struct KEYCONFIG  //.\\system\\config.dat
{
	int key_up[2];
	int key_down[2];
	int key_left[2];
	int key_right[2];
	int key_button[4][2];
};

/*!
*	@ingroup global
*	@brief �p�b�h�R���t�B�M�����[�V�����f�[�^
*/
struct PADCONFIG
{
	//!�{�^���̊��蓖�ā@[A-D][PAD]
	int pad_button[4][NUM_PAD];
};

/*!
*	@ingroup global
*	@brief ���́��v���C���[���蓖�ď��
*/
struct INPUT2PLAYER
{
	UINT kb[2];
	UINT pad[NUM_PAD];
};

/*!
*	@ingroup global
*	@brief �g�p�e�N�X�`���t�H�[�}�b�gID
*/
enum Config2_TexFormat
{
	CONFIG2_USETEXFMT_A1R5G5B5 = 0,
	CONFIG2_USETEXFMT_A4R4G4B4 = 1,
	CONFIG2_USETEXFMT_A8R3G3B2 = 2,
	CONFIG2_USETEXFMT_A8R8G8B8 = 3,
};

/*!
*	@ingroup global
*	@brief �g�pD3D�f�o�C�X�^�C�vID
*/
enum Config2_DevType
{
	CONFIG2_DEVICE_HAL	= 0,
	CONFIG2_DEVICE_REF	= 1,
	CONFIG2_DEVICE_SW	= 2,
};

/*!
*	@ingroup global
*	@brief ��������ID
*/
enum Config2_LimitTime
{
	CONFIG2_TIMELIMITINF	=0,
	CONFIG2_TIMELINIT60		=1,
	CONFIG2_TIMELIMIT99		=2,
};

/*!
*	@ingroup global
*	@brief ��ՓxID
*/
enum Config2_Difficulty
{
	DIFF_VERYEASY		=	0,
	DIFF_EASY			=	1,
	DIFF_NORMAL			=	2,
	DIFF_HARD			=	3,
	DIFF_VERYHARD		=	4,
	DIFF_SUPERHARD		=	5,
	DIFF_ULTRAHARD		=	6,
	DIFF_LIMITERCUT		=	7,
};

/*!
*	@ingroup global
*	@brief �X�N���[���V���b�g�ۑ����̃t�H�[�}�b�g
*/
enum Config2_SShotFormat
{
	SSHOT_JPG			= 0,
	SSHOT_PNG			= 1,
	SSHOT_BMP			= 2,
};

/*!
*	@ingroup global
*	@brief �Q�[�����x
*/
enum Config2_GameSpeed
{
	GSPEED_SLOW			= 0,
	GSPEED_OLD			= 1,
	GSPEED_NEW			= 2,
	GSPEED_30x2			= 3,
//	GSPEED_25x2			= 4,	// ���������ƍő�T�C�Y�𒴂���
};

/*!
*	@ingroup global
*	@brief �J�����㉺���邩���Ȃ���
*/
enum Config2_CameraMode
{
	CAMERA_OLD = 0,
	CAMERA_UPPER = 1,
	CAMERA_NEW = 2,
};

/*!
*	@ingroup global
*	@brief HP�Q�[�W�̃��[�h
*/
enum Config2_GaugeMode
{
	GAUGE_1000 = 0,
	GAUGE_1220 = 1,
};

/*!
*	@brief config.dat , config2.dat �̃f�[�^�Ǘ��N���X
*	@ingroup global
*
*	�R���X�g���N�^�� config.dat,config2.dat ��ǂݍ��ށB
*	�f�X�g���N�^�ł� config2.dat �̂ݕۑ����s��(config.dat�͖{�̎��s�t�@�C���ł͕ύX����Ȃ�����)
*
*	�O���[�o���� g_config ���g�p���Ă���Γ��ɓǂݍ��݂��ӎ����Ȃ��Ă�
*	�ݒ���擾���邱�Ƃ��ł���B
*/
class CConfig
{
public:
	CConfig();
	~CConfig();
	BOOL SaveConfig();
	BOOL SaveConfig2();
	void ReInitialize();
	void SetDefaultConfigKB();	//!< �L�[�{�[�h�E�L�[
	void SetDefaultConfigPad();	//!< �p�b�h�{�^��
	void SetDefaultConfigAss();	//!< ���͊��蓖��
	void SetDefaultConfig2();	//!< �V�X�e���ݒ�

	//���l�̎擾

	//���`��֌W
	BOOL		IsHalfMode()			{return halfmode ? TRUE : FALSE;}
	BOOL		IsFullScreen()			{return full_screen ? TRUE : FALSE;}
	D3DFORMAT	TexFormat();
	D3DDEVTYPE	DeviceType();
	BOOL		HardwereVP()			{return hardwear_vp ? TRUE : FALSE;}
	BOOL		NoAlphaTest()			{return no_alpha_test ? TRUE : FALSE;}

	//���Q�[���ݒ�
	BOOL		IsDebugMode()			{return debug_mode ? TRUE : FALSE;}
	BOOL		SwShowMovieFirst()		{return show_movie_1st ? TRUE : FALSE;}
	BOOL		SwShowInfo()			{return show_info_1st ? TRUE : FALSE;}
	UINT		GetTimeLimit()			{return limit_time;}
	DWORD		GetMaxPoint()			{return max_point+1;}
	Config2_Difficulty GetDifficulty()	{return (Config2_Difficulty)difficulty;}
	DWORD		GetStrikerCount()		{return striker;}
	BOOL		IsLimiterCut()			{return limiter_cut ? TRUE : FALSE;}
	Config2_GameSpeed GetGameSpeed()	{return (Config2_GameSpeed)game_speed;}
	Config2_CameraMode GetCameraMode()	{ return (Config2_CameraMode)camera_mode; }
	int GetGameSpeed2();	//fps��Ԃ��܂�
	Config2_GaugeMode GetGaugeMode()	{ return (Config2_GaugeMode)gauge_mode; }

	//���V�X�e���ݒ�
	BOOL		UseDInput()				{return no_dinput ? FALSE : TRUE;}
	BOOL		UseDSound()				{return no_dsound ? FALSE : TRUE;}
	BOOL		UseDShow()				{return no_dshow ? FALSE : TRUE;}
	BOOL		IgnoreExceptions()		{return ignore_exceptions ? TRUE : FALSE;}

	//�����O
	BOOL UseLog2File()					{return log2file ? TRUE : FALSE;}
	BOOL UseLog_Info()					{return log_info ? TRUE : FALSE;}
	BOOL UseLog_Warning()				{return log_warning ? TRUE : FALSE;}
	BOOL UseLog_Debug()					{return log_debug ? TRUE : FALSE;}

	Config2_SShotFormat GetSShotImageFormat()
										{return (Config2_SShotFormat)sshot_format;}
	char* GetSShotFileTypeStr();

	//���폜���ꂽ����
	BOOL		UsePointSprite()		{return TRUE;}

	//���l�̐ݒ�

	//���`��֌W
	void SetHalfMode(BOOL b)			{halfmode = b ? 1:0;}
	void SetFullScreen(BOOL b)			{full_screen = b ? 1:0;}
	void SetTexFormat(D3DFORMAT tf);
	void SetDeviceType(D3DDEVTYPE dt);
	void SetHardwereVP(BOOL b)			{hardwear_vp = b ? 1:0;}
	void SetNoAlphaTest(BOOL b)			{no_alpha_test = b ? 1:0;}

	//���Q�[���ݒ�
	void SetDebugMode(BOOL b)			{debug_mode= b ? 1:0;}
	void SetShowMovie(BOOL b)			{show_movie_1st= b ? 1:0;}
	void SetShowInfo(BOOL b)			{show_info_1st= b ? 1:0;}
	void SetTimeLimit(UINT tl);
	void SetMaxPoint(DWORD mp)			{max_point = mp-1;}
	void SetDifficulty(Config2_Difficulty diff)	{difficulty=diff;}
	void SetStrikerCount(DWORD sc)		{striker=sc;}
	void SetLimiterCut(BOOL b);
	void EnableLimiterCut()				{limiter_cut_enable=1;}
	void SetGameSpeed(Config2_GameSpeed spd) { game_speed = spd; }
	void SetCameraMode(Config2_CameraMode mode) { camera_mode = mode; }
	void SetGaugeMode(Config2_GaugeMode b)		{ gauge_mode = b ? 1 : 0; }

	//���V�X�e���ݒ�
	void SetUseDInput(BOOL b)			{no_dinput = b ? 0:1;}
	void SetUseDSound(BOOL b)			{no_dsound = b ? 0:1;}
	void SetUseDShow(BOOL b)			{no_dshow = b ? 0:1;}
	void SetIgnoreExceptions(BOOL b)	{ignore_exceptions = b ? 1:0;}

	//�����O
	void SetUseLog2File(BOOL b)			{log2file = b ? 1:0;}
	void SetUseLog_Info(BOOL b)			{log_info = b ? 1:0;}
	void SetUseLog_Warning(BOOL b)		{log_warning = b ? 1:0;}
	void SetUseLog_Debug(BOOL b)		{log_debug = b ? 1:0;}
	void SetSShotImageFormat(Config2_SShotFormat fmt)
										{sshot_format = (unsigned int)fmt;}

private:
	BOOL LoadConfig();
	BOOL LoadConfig2();

public:
	BOOL readok1,readok2;
	KEYCONFIG keycfg;
	PADCONFIG padcfg;
	INPUT2PLAYER inputAssign;
	char *filepath1;
	char *filepath2;

	//�f�[�^
	union
	{
		struct
		{
			//�`��ݒ�
			unsigned int halfmode			: 1;
			unsigned int full_screen		: 1;
			unsigned int tex_format			: 2;//��enum
			unsigned int device_type		: 2;//��enum
			unsigned int hardwear_vp		: 1;
			unsigned int no_alpha_test		: 1;

			//�Q�[���ݒ�
			unsigned int debug_mode			: 1;
			unsigned int show_info_1st		: 1;
			unsigned int show_movie_1st		: 1;
			unsigned int limit_time			: 7;//20-99 , 0=��
			unsigned int max_point			: 2;//1-3
			unsigned int difficulty			: 3;//��enum
			unsigned int striker			: 3;//0-7
			unsigned int limiter_cut		: 1;
			unsigned int limiter_cut_enable : 1;
			unsigned int game_speed			: 2;//��enum
			unsigned int camera_mode		: 2;
			unsigned int gauge_mode			: 1;

			//�V�X�e���ݒ�
			unsigned int no_dinput			: 1;
			unsigned int no_dsound			: 1;
			unsigned int no_dshow			: 1;
			unsigned int log2file			: 1;
			unsigned int ignore_exceptions	: 1;
			unsigned int sshot_format		: 2;
			unsigned int log_info			: 1;
			unsigned int log_debug			: 1;
			unsigned int log_warning		: 1;
		};
		DWORD rawData[7];	//���I�v�V�����̐��H
	};
	BOOL  limcut_pass;
};

extern CConfig				g_config;		//�R���t�B���O

/*@}*/