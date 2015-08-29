
/*
 * 1. 存放每一个season的社交用户信息，每进入一个season都应该与服务器同步当前数据
 * 2. 存放所有好友信息
 * 如：全服最高分，好友列表，好友分数，好友解法
 * 警告：玩家登录了社交平台，与游戏服务器同步数据后方可使用，否则数据为空
*/
#pragma once

#include <string>
#include <vector>
#include <map>

using std::vector;
using std::map;
using std::string;


//测试使用 --- 本地虚拟数据 （模拟用户登录后，获取了社交平台的信息）
#define _USE_VIRTURE_USER_INFOR_ 0
#if _USE_VIRTURE_USER_INFOR_ == 1
extern const char* _VIRTUE_USER_ID_;
extern const char* _VIRTUE_USER_NAME_;
extern const char* _VIRTUE_USER_PICTURE_;
extern bool _VIRTUE_IsLogin;
#endif


struct TaskUser
{
	string userId;
	string name;
	string pictrue; //url
};

struct TaskSolution
{
	string userId;

	int seasonId;
	int sectionId;

	string solutionId; //解法id，由服务器分配

	int star;   //解法评星
	int score;  //解法分数
	int downloadTimes; //下载次数
	int upTimes; //赞次数
	string solutionData;//解法数据， 没有取时 为空   (后缀是.db的，就是本地文件绝对路径，而不是data)

	TaskSolution(){
		seasonId = 0;
		sectionId = 0;
		star = 0;
		score = 0;
		downloadTimes = 0;
		upTimes = 0;
	}
};

struct TaskInfor
{
	int seasonId;
	int sectionId;

	int maxScore;
	int avgScore;
	int playerCount;

	vector<TaskSolution> users;

	TaskInfor(){
		seasonId = 0;
		sectionId = 0;
		maxScore = 0;
		avgScore = 0;
		playerCount = 0;
	}
};




class SocialInfoManager
{
protected:
	SocialInfoManager();
	void initLocalTemplateData(); //模拟的测试数据
	static SocialInfoManager* _instanceSocialManager;
public:
	static SocialInfoManager* getInstance();
	bool initLocalData();
	void resetLocalData();


	//获取一个关卡的完整信息
	const TaskInfor* getSectionInfor(int seasonId, int sectionId);


	//获取一个关卡的 全服最高分
	int getSectionMaxScore(int seasonId, int sectionId);
	//获取一个关卡的全服平局分
	int getSectionAvgScore(int seasonId, int sectionId);


	// 设置用户 和朋友的信息
	void setFriendsAndMeInfor(const vector<TaskUser>& vec);
	//
	const TaskUser* getUserInfor(string userId);
	vector<TaskUser> getFriendsExceptMe();
	vector<TaskUser> getFriendsAndMe();
	vector<TaskUser> getFriendsAndMe_WithoutVirtualUser(); //除去本地配置的官方好友，主要用于cmdhelper请求服务器数据时使用
// 	//获取好友进度
// 	std::pair<unsigned int, unsigned int> getFriendProgress(string userId); // 返回seasonId sectionId
	/*获取进展到当前season的friend
	 * @param  seasonId 需要查询的season
	 * return 进展到当前season的好友列表， key:userID  value:sectionID
	*/
	map<string,unsigned int> getFriendProgressBySeason(unsigned int seasonId);
	vector<string> getFriendProgressBySection(unsigned int seasonId, unsigned int sectionId); //获取进度在当前section的好友


	//关卡解法信息
	const TaskSolution* getSolutionInfor(int seasonId, int sectionId, string userId);
	TaskSolution* getSolutionInfor(string solutionId);
	void setSolutionData(string solutionId, const string& data);
	string getSolutionData(int seasonId, int sectionId, string userId);//本地没有数据时 string为空
	bool isOfficialOrVirtualSolutionId(string solutionId);


	//设置一个season的关卡信息（好友排行列表、全服平均分）
	void setSectionSocialInfor(const vector<TaskInfor>& list);
	//更新玩家自己完成一关后的数据
	void setSelfSectionScore(unsigned int seasonId, unsigned int sectionId, int score, int star);
protected:
	string getSolutionData(string solutionId);//本地没有数据时 string为空
	void setSeasonInfor(int seasonId, const vector<TaskInfor>& list); //结构中存在指针，所以接口内部会拷贝一份后重新设置指针地址
	bool isOfficalUserID(string userId); //判断是否官方账号的id

protected:
	vector<TaskUser> mUserList;
	vector<string> mVirtualUserList;   //每位玩家都会配置几个官方账号作为好友 (已加入mUserList，这里列出来是为了方便删除的时候擦找)
	vector< std::pair<int, vector<TaskInfor> > > mAllTask; //  seasonId --- vector<TaskInfor>

	bool m_bHasInitFriendsAndMe;
};