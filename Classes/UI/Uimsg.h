#pragma once
#include <string.h>
using namespace std;
#define MSG_UI_ChANGE  "Uichange"
#define MSG_UI_ChANGE_BG_COLOR	"Change_bg_color"
#define MSG_FRIND_RETURN_LIST "Friend_return_stop"
#define MSG_FRIND_PLAY "Friend_play"
#define MSG_FRINDGKMSG_PLAY "Friendckmsg_play"
#define MSG_FRINDGKMSG_ACOUNTPLAY "Friendckmsg_acountplay"
#define MSG_FRIND_BEYOND_ACOUNT_REFRESH "refresh_acountLayer_beyondFriend"
#define MSG_FRIND_CLICKZ "friendclickz"
#define MSG_GAME_NEI "Game_state_inint"
#define MSG_GAME_CLICKIS_Z "Game_clickisz"
#define MSG_GAME_Stop "Game_stop"
#define MSG_GAME_Stopright "Game_stop_right"
#define MSG_LAGUAGEMAIN  "main_languge"
#define MSG_LAGUAGEBig   "big_languge"
#define MSG_LAGUAGESmall "small_languge"
#define MSG_LAGUAGEAchive "achive_languge"
#define MSG_CHAONDJBUY    "chaonengdaojusucess"
#define MSG_CHAONDJBUYN    "chaonengdaojusucessn"
#define MSG_REFRESHPARENTCLICKZ "refreshparentclickz"


const float time_action=0.25f;
const float jesuantime_action=0.15f;
const float achivelistjuli=0.0f;
const float gkfadeout=0.01f;
const float num_Fade=255.0f;
const float Big_sp_juli=10.0f;
//所有按钮右侧间距
const float buton_jiangju=0.95f;//
const float buton_shang=0.95f;//
const float Big_xiao_backscale=0.4f;
const float xiaogk_jg= 0.0f;
const int   maintouchtype=0;
const int   bigtouchtype=128;
const int   smalltouchtype=128;
const int   tunshimaintouch=-128;
const int   touchlevel=-127;

const float chaonengdj_scale=0.75f;
const float chaonengdj_time=0.25f;

const float allfriendlayer_weizi=0.08f;



enum Layertag{

	SceneBg_tag = 5234,
	Starseneui_tag,
	Weiyi_tag=99,
	Starmain_tag=100,  //主界面
	Bigcheckpoint_tag=101, //大关卡选折界面
	Smalllayer_tag=102,//小关卡选折界面
	Gamesetlayer_tag=103,//游戏设置
	Guangkalayer_tag=104,  //关卡信息界面
	Achievelayer_tag,  //成就界面
	ACountlayer_tag,
};

enum UIActionTag {
	UI_Action_Move_Tag = 0x1001,
};

enum Tachuktag{

	NonetWork_tag=1025,


};

struct Uistruct
{
	int parent;
	int current;

};

enum  Datatype
{

	Chaondaoju=10,
	Friendlist,
	Xiaogktable,
	Xiaogkfriendlist,
	Yaoqingfriendlist,
};


enum Gamestoptag{

	Gamecontinu_tag=100,
	Gameother_tag,
	Gamestopxiaoshi_tag,
	chaonengdaojulayer_tag,
	friendlayer_tag,
	golutulayer_tag,
	Gamestopxiaoshi_xiaosi,
};

struct msgSnsdata
{
	string name;
	string pictrue; //url
	string userId;

	int seasonId;
	int sectionId;

	string solutionId; //解法id，由服务器分配
	int star;   //解法评星
	int score;  //解法分数
	int downloadTimes; //下载次数
	int upTimes; //赞次数
	string solutionData;//解法数据， 没有取时 为空
};


