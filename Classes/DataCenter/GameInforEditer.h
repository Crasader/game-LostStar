#ifndef __GAME_INFOR_EDITER_H__
#define __GAME_INFOR_EDITER_H__

#include "cocos2d.h"
#include "CommonDefine.h"
#include "LFFileUtiles.h"
#include "ResManager.h"

using namespace cocos2d;
#include "include_json.h"

#include <string>
#include <list>

using namespace std;

class SeasonInfor;
class SectionInfor;


struct SectionPlayData
{
	int seasonId;
	int sectionId;
	int playedTime; // 玩的次数
	bool usedHelp;	// 历史上用过帮助没

	bool buyedTimeLimite; // 购买这个小section，用于去除次数限制，但是如果是lock的那么还是不能玩

	// 上次玩的时间戳
	unsigned long lastPlayTime;

	SectionPlayData()
	{
		sectionId = 0;
		seasonId = 0;
		playedTime = 0;
		buyedTimeLimite = false;
		usedHelp = false;
		lastPlayTime = 0;
	}
};

enum ItemType{
	type_Normal = 0,	// 可以看见，可以点击，就是普通的按钮
	type_Invalid,		// 可以看见，但是永远都不可以点击的按钮，不管锁住与否
	type_CommingSoon,	// type_CommingSoon
	type_URL,			// 是一个URL的连接，点击就会打开URL
	type_Ad,			// 是一个广告
	type_Hide_Disable,	// 不可见，也不处理点击
	type_Num,			// 描述type的个数
};

#define MAX_PAY_HISTORY	10

//一个用于读写(编辑)游戏信息的类，里面存放了关于游戏的信息,比如：
//1，游戏是否第一次打开
//2，某一章（season）是否已经玩过，得分是多少
//3，某一章的哪些小节（section）是否已经玩过，得分是多少
class GameInforEditer : public cocos2d::CCObject
{
public:
	static GameInforEditer* getGameInfor();
	static void destroy();
	void initData();
	void correctData(); //矫正 同步后的数据
	string getSeasonName(int seasonId);

	list<SeasonInfor*> getSessonInforList();						// 获得SeasonInfor的所有数据
	int getLastPlayedSeasonId();									// 最后一个玩过的season，played为true
	int getLastPlayedSectionId();									// 获取最后一个玩过的section， played为true
	int getLastUnlockedSeasonId();									// 最后一个解锁的season(就是一般的游戏season)
	bool saveSessonInforList();										// 把当前的SessonInfor（包括season和season下面的section数据写回文本文件），成功返回true
	bool isFirstRunMe();											// 是否第一次启动我这个程序
	void firstRunOverApp();											// 第一次运行了程序，也就是置标志位，并保存
	void addSeasonInfor(SeasonInfor* infor);						// 加入一个seasonInfor
	SectionInfor* getSectionInfor(int seasonId,int sectionId);		// 【注意NULL】获得seasonId，sectionId的对应的一个Section的信息
	SeasonInfor* getSeasonInfor(int seasonId);						// 【注意NULL】获得该seasonId对应的Season的信息
	void makeNew();
	bool isIgnoreAllLock();											// 是否忽略所有的锁

	bool canUnclock(int seasonId, bool needSpecialItem=true);		// 看这个season是否可以解锁，如果已经是解锁了的，那么就返回false

	void unLockSeasonForce(int seasonId);							// 强制开启某关

	// 用钱解锁season以及他下面的所有section
	void unLockSeasonAndItsSectionsUseMoney(int seasonId);

	// 获得要解锁该season的分数,-1 则无效
	int getScoreForUnlock(int seasonId);

	bool needComments();
	string getCommentUrl();

	void setNeedComment(bool need,string url);
	int getTotalScore(bool bAll = true); //获取所有星星
	int getClientId();
	void setClientId(int _id);
	LanguageType getInitLocale();

//	int getUserId();
//	void setUserId(int id);
	string getUserName();
	void setUserName(string str, bool bOverWrite = false);

	// 获取在收到评论要求时，到现在，已经成功的次数
	int getSuccessNumAfterNeedComments();
	void setSuccessNumAfterNeedComments(int num);

// 	// 保存和获取支付信息
// 	void setPayInfor(SPCmd_QueryPayChannelInfoRsp infor);
// 	bool getPayInfor(SPCmd_QueryPayChannelInfoRsp& out_infor);
	// 电话号码
	string getPhoneNumber();
	void setUserInputPhoneNumber(string number);

	// 只用于yeahMobe的版本
	void addBuySeasonHistory(int seasonId);
	void addBuyAllHistory();

	// 某个season是否全部买过了
	void addSeasonBuyed(int seasonId);
	bool isBuyedSeason(int seasonId);

	// 某个season是否都成功了，每一个section都成功了
	bool isWholeSeasonSuccessed(int seasonId);

	// 增加1次关卡的玩的次数
	void incSectionPlayTime(int seasonId,int sectionId);
	//// 减少一次关卡玩的次数
	//void decSectionPlayTime(int seasonId,int sectionId);
	// 计算如果要玩这个关卡，需要花多少星星
	int getSectionPlayToCostStarNum(int seasonId,int sectionId);
	int getBuySectionTimeLimiteCostMoney();
	// 获得当前关卡还剩几次免费的玩的次数
	int getSectionFreePlayTimeLeft(int seasonId,int sectionId);
	// 获取当关卡还剩下的可玩次数（buyed不要调用）
	int getSectionPlayTimeLeft(int seasonId,int sectionId);
	// 计算这个关卡今天玩了几次了
	int refreshPlayedTimeToday(int seasonId,int sectionId);
	// 购买一个小关卡
	void setSectionTimeLimiteBuyed(int seasonId,int sectionId);
	// 设置已经用过帮助了
	void setSectionHelpUsed(int seasonId,int sectionId);
	// 是否购买过
	bool isSectionTimeLimiteBuyed(int seasonId,int sectionId);
	// 
	const SectionPlayData* getReadOnlySectionPlayData(int seasonId,int sectionId);
	// 获得当前可用的星星个数
	int getTotalFreeScore();
	// 获取剩余的帮助次数
	int getHelpTimeLeft();
	// 扣除星星的个数
	void subMyStarNum(size_t toSub);
	// 扣除剩余的帮助次数
	void subHelpNum(size_t toSub);
	// 添加帮助的可使用次数
	void addHelpNum(size_t toAdd);
	// 超能道具
	void addSuperTool(size_t count, int type = 0);
	void subSuperTool(size_t count, int type = 0);
	int getSuperToolCount(int type = 0);
    unsigned long getToalPlayTime(){return m_lPlayTotalTime;};

	//得到总共使用的超能道具个数
	int getToatalUsedSuperToolCount(){
		return m_iToatalUsedSuperToolCount;
	}
	// 
	void addBuyedStarNum(size_t num);
	//
	int getOpenboxNum(){return openBoxNum;}
	void addOpenboxNum(int num) {
		openBoxNum += num;
		if (num == 10)
		{
			m_iTenTimesOpenBoxTimes++;
		}
		else
		{
			m_iOneTimesOpenBoxTimes++;
		}
		saveSessonInforList();
	}
    unsigned long getCurPlayTime(){return m_lCurPlayTime;};
    void setCurPlayTime(unsigned long time){m_lCurPlayTime = time;};
    void updateTime(float dt);

	//得到开一次宝箱的总数
	int getOneTimesOpenBoxTimes(){return m_iOneTimesOpenBoxTimes;};
	//得到开十次宝箱的总数
	int getTenTimesOpenBoxTimes(){return m_iTenTimesOpenBoxTimes;};

	//是否购买过官方解法
	bool getHasPayOfficialSolution(){return m_bHasPayOfficialSolution;}
	void setHasPayOfficialSolution(bool state){m_bHasPayOfficialSolution = state; saveSessonInforList();}

	//// 触发等待
	//// 用于设置一个标记量，表示有
	//void triggleTimeWait();

	// 开始section的计时
	// 一般是在上一关完毕，解锁下一关的时候调用
	void startSectionTimeCounter(int seasonId,int sectionId);
	// 获取某一关解锁还需要等待的时间(秒)，如果星星足够直接就可以玩
	// 不用调这个函数获取时间，0就是不用等了
	// 如果某关已经开始等了，就会返还还剩余的时间
	int getTimeToWaitForFirstPlay(int seasonId,int sectionId);
	// 获取某一个解锁需要的的星星数量,玩过的话就是0
	int getStarCostForFirstPlay(int seasonId,int sectionId);

	//展示广告间隔时间  20140710 scf 新需求
	protected: int m_iADDelayTime;
	public : int getDisplayADDelayTime(){return m_iADDelayTime;}// seconds
	    
// 	vector<CastInfo> getAllToBeSaveToServerPayInfoFromFile();
//     CastInfo getOneToBeSaveToServerPayInfoFromFile(int index);
//     void addToBeSaveToServerPayInfoToFile(BuyItem itemInfo);

	void initLockSeason();

	//LockedList和UnLockedList都是属于隐藏关卡，
	//LockedList 表示 该关卡的“lock”值为1      “lock”值暂时无用处
	//UnLockedList 表示 该关卡的“lock”值为0   
	//是否是隐藏关卡
	bool isChallengeSection(int seasonid, int sectionid);
	bool getIsInUnLockedList(int seasonid, int sectionid);

	//得到指定章节的隐藏关卡数
	int getSecretStageNumBySeasonId(int seasonId);

	//每章16关完成的时候就必须要调用该函数一次
	//创建隐藏关卡的sectioninfo,保证他们不为NULL
	void unLockSecretStageBySeason(int seasonId);
	//获取隐藏关卡是否能解锁
	bool getSecretStageCanUnLocked(int seasonid);

	//获取当前seasonid之前用于判断是否能解锁隐藏关卡的分数
	int getTotalScoreBeforeSeasonId(int seasonId);

	static void getAESKey(char *key, bool bEncrypt = true);

	int getMyPerfectpassCount();
	int getMyEvaluateCount();
		
	void initLastClickVec();

	int getLastPlayedSectionIdBySeasonId(int seasonId);

	void setLastClickId(int iLastClickSeasonId, int iLastClickSectionId)
	{
		for (size_t i = 0; i < m_lastClickId.size(); i++)
		{
			if (m_lastClickId.at(i).first == iLastClickSeasonId)
			{
				m_lastClickId.at(i).second = iLastClickSectionId;
				break;
			}
		}
	};

	int getLastClickSectionId(int seasonId)
	{
		for (size_t i = 0; i < m_lastClickId.size(); i++)
		{
			if (m_lastClickId.at(i).first == seasonId)
			{
				return m_lastClickId.at(i).second;
			}
		}
		return 1;
	}

private:
	SectionPlayData* getSectionPlayData(int seasonId,int sectionId);
private:
	GameInforEditer();
	~GameInforEditer();
	bool initGameInfor(string fileFullPath);						// 初始化GameInfor的信息
	string getSeasonInfoListString();								//获得seasonInfor数据的字符串描述
	bool parseInforString(string str);								//解析，成功则返回true
	string getMapKey(int seasonId,int sectionId);					// 获取m_mapSection2PlayTime的key

	static GameInforEditer* gameInforEditer;
	//season的数据信息
	list<SeasonInfor*> mSeasonInfors;
	map<string,SectionPlayData> m_mapSection2PlayTime;
	//是不是第一次run我们只个APP，只有显示完毕Guide之后，我们才认为是第一次运行(则即使中途崩溃，下次也还是会引导我们)
	bool mIsFirstRunApp;
	bool mIgnoreAllLock;
	bool mNeedCommentGame;			// 是否需要评论
	string mCommentUrl;				// 评论的链接
	int mSuccessNumAfterNeedComment;
	string inforFilePath_Read;
	string inforFilePath_Write;
	LFFileUtiles* fileUtiles;

	bool encrypt; // 加密为true
	char m_AES_key[16];

	int mClientId;
	int mUserId; //上报排行榜数据用！
	string mUserName;

	int mLastWaitGap;	// 玩某个关卡上次等待的时间
	int starNumBuyed;	// 额外购买的星星数量
	int openBoxNum; //开宝箱的数量、 轮询每第10个必出一个当前章节的道具
	int m_iOneTimesOpenBoxTimes;	//开一次宝箱的数量
	int m_iTenTimesOpenBoxTimes;	//开十次宝箱的数量
	// 付款用掉的星星个数
	int mStarNumPayed;
	// 剩余的帮助次数
	int mHelpNumLeft;
	
	//是否购买过官方解法
	bool m_bHasPayOfficialSolution;

	//超能道具的使用个数（总共）
	int		m_iToatalUsedSuperToolCount;


	vector< std::pair<int, int> >	m_lastClickId;

	vector<int> mBuyedSeasonIds;
    
    
    //一共玩游戏的时间
    unsigned long m_lPlayTotalTime;
    unsigned long m_lCurPlayTime;

	unsigned long lastPlayGameTime; // 上次玩某一关的时间

	map<int,int> mMapSuperToolType2Count; // 超能道具类型和个数的映射

	LanguageType mInitLocale;
//	SPCmd_QueryPayChannelInfoRsp payInfor;
	string mPhoneNumber;
	char buyHistory[MAX_PAY_HISTORY+1];	// 购买的历史，第一关的也在这里面。01111就是全买 01000就是只买了第二章,以"\0"结尾

	std::vector< std::pair<int, int> >		m_lLockedSeason;
	std::vector< std::pair<int, int> >		m_lUnLockSeason;
};

//一个Season的信息，里面有多个SectionInfor
class SectionInfor
{
public:
	int sectionId;
	ItemType sectionType;
	int seasonId;
	bool isPlayed;
	bool locked;
	unsigned int earliestCanPlayTime;	// 最早可玩的时间
	bool isSuccessed;	// 是否成功过了
	int iStarNum;
	float avgScore;//全球的平均得分
	int minScore;//全球的最小得分

	bool bUseSuperTool;		//是否使用超能道具
	bool bUseHelpMap;		//是否使用攻略图
	float fTimeCost;		//过关花费的时间
 	float fFirstCost;		//第一次花费的时间
	unsigned int effectiveClickNum;	//有效点击次数
	int iRankingScore;     //计算出来的分数  ---》 用于排名，服务器同步时优劣比较

	void* data;	// 额外携带的data

	int keyItemMedalNum;			// 关键元素的勋章个数
	int keyTableMedalNum;			// 关键台面的勋章个数

	SectionInfor()
	{
		sectionId = -1;
		seasonId = -1;
		data = NULL;
		sectionType = type_Normal;
		isPlayed = false;
		locked = true;
		iStarNum = 0;
		iRankingScore = 0;
		avgScore = 0;
		minScore = 0;
		earliestCanPlayTime = 0; // 默认就是任何时候都可玩
		isSuccessed = false;
		keyItemMedalNum = 0;
		keyTableMedalNum = 0;
		bUseSuperTool = false;
		bUseHelpMap = false;
		fTimeCost = 0.0f;
        fFirstCost = 0.0f;
		effectiveClickNum = 0;
	}
	SectionInfor(int seasonId,int sectionId,bool isPlayed,bool locked)
	{
		this->sectionId = sectionId;
		this->seasonId = seasonId;
		this->isPlayed = isPlayed;
		this->locked = locked;

		earliestCanPlayTime = 0;
		isSuccessed = false;
		sectionType = type_Normal;
		iStarNum = 0;
		avgScore = 0;
		minScore = 0;
		keyItemMedalNum = 0;
		keyTableMedalNum = 0;
		data = NULL;

		bUseSuperTool = false;
		bUseHelpMap = false;
		fTimeCost = 0.0f;
		fFirstCost = 0.0f;
		effectiveClickNum = 0;
		iRankingScore = 0;
	}

#define USED_SUPER_TOOL_SCORE 2000
	//判断 传入的数据 是否比 this的好
	bool isBetterThanMe(float timeCost, int clickCount, int starNum, bool bUseSuperTool)
	{
// 		if (this->iStarNum > starNum) return false;
// 		else if (this->iStarNum < starNum) return true;
		int score = 0;

		if (bUseSuperTool) {
			starNum = 1;
			score = USED_SUPER_TOOL_SCORE;
		}else{
			score = calculateRankScore(timeCost,clickCount,starNum,bUseSuperTool);
		}

		return this->iRankingScore < score;
	}
	bool isBetterThanMe(int rankingScore)
	{
		return this->iRankingScore < rankingScore;
	}

	//计算评分 (和score字段不一样，score是值星级，老版本遗留，可能会用)
	/*评分公式：
	设定时间上限是2000秒，超过20000秒按20000秒算，
	游戏时间的取值范围：1-20000
	设定步数上限是20步，超过20步按20步算，
	步数的取值范围：1-20
	n:
	若评星为3星，n=100000
	若评星为2星，n=50000
	若评星为1星，n=20000
	计算公式为：（21-步数）*20-游戏时间+n*/
	// warning: 使用的超能道具  score = 2000；
	static int calculateRankScore(float timeCost, int clickCount, int starNum, bool bUserSuperTool)
	{
		if (bUserSuperTool) return USED_SUPER_TOOL_SCORE;
		if (timeCost == 0 || clickCount == 0 || starNum == 0) return 0;

		int time = timeCost > 200 ? 20000 : timeCost*100;
		int click = clickCount > 20 ? 20 : clickCount;
		int n;
		if (starNum >= 3) n = 100000;
		else if (starNum >= 2) n = 50000;
		else n = 2000;
		return (( 21 - click ) * 2000 - time + n);
	}
};


//一个Season的信息，里面有多个SectionInfor
class SeasonInfor
{
public:
	int seasonId;
	ItemType seasonType;
	bool isPlayed;
	bool locked;
	int score;
	float avgScore;//全球的平均得分
	int minScore;	//全球的最小得分
	void* data;	// 额外携带的data

	//构造默认的一个SeasonInfor：seasonId = -1;isPlayed = false;locked = true;score = 0;
	SeasonInfor()
	{
		seasonId = -1;
		seasonType = type_Normal;
		isPlayed = false;
		locked = true;
		score = 0;
		avgScore = 0;
		minScore = 0;
		data = NULL;
		clearSectionList();
	}
	//score = 0;
	SeasonInfor(int seasonId,bool isPlayed,bool locked)
	{
		this->seasonId = seasonId;
		this->isPlayed = isPlayed;
		this->locked = locked;
		this->seasonType = type_Normal;
		score = 0;
		avgScore = 0;
		minScore = 0;
		data = NULL;
		clearSectionList();
	}
	~SeasonInfor()
	{
		clearSectionList();
	}

	void addSectionInfor(SectionInfor* infor)
	{
		sectionInforList.insert(sectionInforList.end(),infor);
	}

	// 获取一个section_id 为指定的Item
	SectionInfor* getSectionInfor(int section_id)
	{
		list<SectionInfor*>::iterator it;
		for (it = sectionInforList.begin(); it!=sectionInforList.end(); it++) {
			if ((*it)->sectionId == section_id)
			{
				return (*it);
			}
		}
		return NULL;
	}

	list<SectionInfor*> getAllSectionInfors()
	{
		return sectionInforList;
	}

	list<SectionInfor*> getNormalSectionInfors()
	{
		list<SectionInfor*> temp;
		list<SectionInfor*>::iterator it;
		for (it = sectionInforList.begin(); it!=sectionInforList.end(); it++) {
			if ((*it)->sectionId <= 16)
			{
				temp.push_back(*it);
			}
			else
			{
				break;
			}
		}
		return temp;
	}

	//得到我下面有记录的section的得分和
	int getMyAllSectionScore()
	{
		int score = 0;
		list<SectionInfor*>::iterator it;
		for (it = sectionInforList.begin(); it!=sectionInforList.end(); it++) {
			score += (*it)->iStarNum;
		}
		return score;
	}

	int getMyNormalSectionScore()
	{
		int score = 0;
		list<SectionInfor*>::iterator it;
		int max = 16;
		for (it = sectionInforList.begin(); it!=sectionInforList.end(); it++) {
			if ((*it)->sectionId > max)
			{
				continue;
			}
			score += (*it)->iStarNum;
		}
		return score;
	}
	void clearSectionList()
	{
		list<SectionInfor*>::iterator it;
		for (it = sectionInforList.begin(); it!=sectionInforList.end(); it++) {
			delete (*it);
		}
		sectionInforList.clear();
	}

	//根据我下面有记录的section的得分和来设置我的season得分
	void resetScoreByMySections()
	{
		this->score = getMyAllSectionScore();
	}
private:
	list<SectionInfor*> sectionInforList; 

};

#endif