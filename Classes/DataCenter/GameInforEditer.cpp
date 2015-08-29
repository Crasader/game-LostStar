#include "GameInforEditer.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "cocos2d.h"
#include "Music.h"
#include "GameLayerFactory.h"
#include "VersionControl.h"
#include "AES.h"
#include "LFMessageBox.h"
#include "LFStrings.h"
#include "ServerTime.h"
#include "JuqingDaoJuManager.h"
#include "LevelManager.h"
#include "VersionControl.h"
#include "UserDataManager.h"
#include "AchievementManager.h"
#include "History.h"
#include "CmdHelper.h"

int g_MarketLayerIndex = -1;

using namespace cocos2d;
// #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
// //配置文件的key;
// static const string defaultGameInfoPath = CCFileUtils::sharedFileUtils()->getWritablePath()+"infor.txt";
// string readGameInforPath = defaultGameInfoPath;
// string writeGameInforPath = defaultGameInfoPath;
// static const string defaultSeasonLockInfoPath = "seasonLock.txt";
// string readGameSeasonLockInfor = defaultSeasonLockInfoPath;
// #else
//配置文件的key;
static const string defaultGameInfoPath = "infor/infor.txt";
string writeGameInforPath = "infor.txt";
static const string defaultSeasonLockInfoPath = "infor/seasonLock.txt";
string writeGameSeasonLockInfor = "seasonLock.txt";
//#endif

#if Limit_Section_Max_Play_Num == 1
#define MAX_FREE_PLAY_TIME		5			// 免费玩，最多5次
#define MAX_PLAY_TIME			10			// 收费玩最多10次
#else
#define MAX_FREE_PLAY_TIME		0x7fffffff	// 用最大值，代表不限制次数了
#define MAX_PLAY_TIME			0x7fffffff
#endif
#define PLAY_TIME_REFRESH_GAP	(24*60*60)	// 可玩次数的刷新间隔 一天

#define WAIT_GAP_INC			(10*60)		// 10分钟

// datapool里面回调
void g_SetNeedComment(string url)
{
	GameInforEditer::getGameInfor()->setNeedComment(true,url);
	GameInforEditer::getGameInfor()->saveSessonInforList();
	CCLog("g_SetNeedComment ---> url = [%s]",url.c_str());
}

using namespace std;

string type_string[type_Num] =
{
	"type_Normal",
	"type_Invalid",
	"type_CommingSoon",
	"type_URL",
	"type_Ad",
	"type_Hide_Disable",
};

string ignoreAllLock = "ignoreAllLock";
string isFirstRunApp = "isFirstRunApp";
string clientId = "clientId";
string initLocale = "initLocale";
string lastSettedLocale = "lastSettedLocale";
string needCommentGame = "needCommentGame";
string commentUrl = "commentUrl";
string successNumAfterNeedComment = "mSuccessNumAfterNeedComment";
string starNumPayed = "starNumPayed";

string sectionPlayData_array = "sectionPlayData_array";
string backgroundSoundOpen = "backgroundSoundOpen";
string effctiveSoundOpen = "effctiveSoundOpen";
string seasonInfor_array = "seasonInfor_array";
string sectionInfor_array = "sectionInfor_array";
string season_type = "season_type"; // 用于描述该season的类型，比如 正常, 无效 , 广告等
string section_type = "section_type" ;// 用于描述该section的类型，比如 正常, 无效 , 广告等
string seasonId = "seasonId";
string sectionId = "sectionId";
string isPlayed = "isPlayed";
string locked = "locked";
string score = "score";
string avgScore = "avgScore";
string minScore = "minScore";
string extraData = "data";

string bUsedSuperTool = "bUsedSuperTool";
string bUsedHelpMap = "bUsedHelpMap";
string fTimeCost = "fTimeCost";
string fFirstCost = "fFirstCost";
string uClickedTimes = "uClickedTimes";
string iRankingScore = "iRankingScore";

string keyItemMedalNum = "keyItemMedalNum";		// 关键元素勋章
string keyTableMedalNum = "keyTableMedalNum";	// 关键台面勋章

// 价格数据保存
string money_symble = "money_symble";

string one_msg_price_1 = "one_msg_price_1";
string need_msg_count_1 = "need_msg_count_1";
string key_price_1 = "key_price_1";
string shortcode_price_1 = "shortcode_price_1";

string one_msg_price_2 = "one_msg_price_2";
string need_msg_count_2 = "need_msg_count_2";
string key_price_2 = "key_price_2";
string shortcode_price_2 = "shortcode_price_2";
string phoneNum = "phoneNum";
string payHistory = "payHistory";



ItemType getItemType(string str)
{
	for (int i=0;i<type_Num;i++)
	{
		if (type_string[i] == str)
		{
			return (ItemType)i;
		}
	}
	CCAssert(false,"非法的类型");

	return type_Normal;
}

GameInforEditer* GameInforEditer::gameInforEditer;

GameInforEditer::GameInforEditer()
{
	m_iToatalUsedSuperToolCount = 0;
	m_iOneTimesOpenBoxTimes = 0;
	m_iTenTimesOpenBoxTimes = 0;
	mIsFirstRunApp = true;
	mStarNumPayed = 0;
	starNumBuyed = 0;
	openBoxNum = 0;
	mIgnoreAllLock = false;
	mHelpNumLeft = 0;
	lastPlayGameTime = 0;
    m_lPlayTotalTime = 0;
    m_lCurPlayTime = 0;
	mLastWaitGap = 0;
	m_bHasPayOfficialSolution = false;
	m_iADDelayTime = 3* 60; //3分钟展示一次广告
	memset(buyHistory,0,sizeof(buyHistory));
	for (int i=0;i<MAX_PAY_HISTORY;i++)
	{
		buyHistory[i] = '0';
	}
}

void GameInforEditer::destroy()
{
	if (gameInforEditer != NULL)
	{
		delete gameInforEditer;
		gameInforEditer = NULL;
	}
}

void GameInforEditer::initData()
{
	/*
	 * 玩家使用社交平台登录过，那么就会把默认数据绑定到第一次登录的账号info_facebook_123.txt，此后将不再使用默认的文件info.txt
	 * 之后用其他账号登录，又会拷贝一份default，保存在一个新的文件info_facebook_456.txt
	*/
	{
		string platform, id;
		bool bHasPlatformID = History::getHistory()->getLastUserPlatformAndID(platform, id);
		if (bHasPlatformID)
		{
			string temp_writeGameInforPath = "infor";
			string temp_readGameSeasonLockInfor = "seasonLock";
			writeGameInforPath = temp_writeGameInforPath + "_" + platform + "_" + id + ".txt";
			writeGameSeasonLockInfor = temp_readGameSeasonLockInfor + "_" + platform + "_" + id + ".txt";

			//第一次登录，那么绑定到之前的数据
			string oldNoneUserGameInforFile = CCFileUtils::sharedFileUtils()->getWritablePath()+defaultGameInfoPath.substr(defaultGameInfoPath.find_last_of("/")+1);
			string oldNoneUserLockSeasonFile = CCFileUtils::sharedFileUtils()->getWritablePath()+defaultSeasonLockInfoPath.substr(defaultSeasonLockInfoPath.find_last_of("/")+1);
			if (CCFileUtils::sharedFileUtils()->isFileExist(oldNoneUserLockSeasonFile) && CCFileUtils::sharedFileUtils()->isFileExist(oldNoneUserGameInforFile))
			{
				int ret = rename(oldNoneUserGameInforFile.c_str(), (CCFileUtils::sharedFileUtils()->getWritablePath()+writeGameInforPath).c_str())
					   || rename(oldNoneUserLockSeasonFile.c_str(), (CCFileUtils::sharedFileUtils()->getWritablePath()+writeGameSeasonLockInfor).c_str());
				if (0 == ret){
					CCLog("%s --->SUCCESS rename file [%s] to [%s]", __FUNCTION__, oldNoneUserGameInforFile.c_str(), writeGameInforPath.c_str());
				}else {
					CCLog("%s --->FAILED rename file [%s] to [%s]", __FUNCTION__, oldNoneUserGameInforFile.c_str(), writeGameInforPath.c_str());
					CCAssert(false, "");
				}
			}
		}
	}




	// 只有win32不加密
	encrypt = (CC_PLATFORM_WIN32 != CC_TARGET_PLATFORM);

	if (encrypt)
	{
		getAESKey(m_AES_key, encrypt);
	}
	
	
	mInitLocale = ResManager::getManager()->getDiviceLanguage();

	mNeedCommentGame = false;
	mCommentUrl = "";
	mSuccessNumAfterNeedComment = 0;

	mSeasonInfors.clear();

	fileUtiles = LFFileUtiles::getLFFileUtiles();
	fileUtiles->copyFileFromResToDocIfNotExist(defaultGameInfoPath, encrypt?m_AES_key:NULL, writeGameInforPath);
	fileUtiles->copyFileFromResToDocIfNotExist(defaultSeasonLockInfoPath, encrypt?m_AES_key:NULL, writeGameSeasonLockInfor);

	initLockSeason();
	bool sec = initGameInfor(writeGameInforPath);
	if (!sec)
	{
		CCLog("\n======GameInforEditer construct failed !");
	}

    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GameInforEditer::updateTime), this, 1, false);
    
}

void GameInforEditer::getAESKey(char *key, bool bEncrypt /*=true*/)
{
	if (bEncrypt)
	{
		string _id = "camellia";
		string code = "Cla"+_id;
		char tmp[128];
		memset(tmp,0,sizeof(tmp));
		int length = code.length();
		strncpy(tmp,code.c_str(),128>length?length:128);

		key[0] = tmp[0];
		key[1] = tmp[126];
		key[2] = tmp[7];
		key[3] = tmp[5];
		key[4] = tmp[20];
		key[5] = tmp[4];
		key[6] = tmp[6];
		key[7] = tmp[50];
		key[8] = tmp[12];
		key[9] = tmp[3];
		key[10] =tmp[100];
		key[11] =tmp[16];
		key[12] =tmp[3];
		key[13] =tmp[30];
		key[14] =tmp[13];
		key[15] =tmp[5];

		for (int i = 0;i<sizeof(key);i++)
		{
			CCLog("m_AES_key[%d-%c]",key[i],key[i]);
		}
	}
	else
	{
		memset(key,0,sizeof(key));
	}
}

void GameInforEditer::updateTime(float dt)
{
    m_lPlayTotalTime++;
    m_lCurPlayTime++;
//    CCLog("m_lPlayTotalTime is %ld", m_lPlayTotalTime);
}


GameInforEditer::~GameInforEditer()
{
	mSeasonInfors.clear();
}
LanguageType GameInforEditer::getInitLocale()
{
	return mInitLocale;
}
int GameInforEditer::getClientId()
{
	return mClientId;
}
void GameInforEditer::setClientId(int _id)
{
	CCAssert(_id != 0,"This Method can just called by first Time get the ClientId");

	// 如果已经是分配了Id的了,那么 就应该 服务器返回的数据也是这个Id,不用修改
//	if (mClientId != 0)
//	{
//		CCAssert(mClientId == _id ,"We have already getted the clientId,you can not change it!");
//
//		return;
//	}

	mClientId = _id;

	saveSessonInforList();
}
GameInforEditer* GameInforEditer::getGameInfor()
{
	if (gameInforEditer == NULL)
	{
		gameInforEditer = new GameInforEditer();
		//gameInforEditer->initData();  由外部调用初始化函数
	}
	return gameInforEditer;
}
//在iPad上调用coco2dx的读写时不需要相对前缀的。
//但是直接使用fileOpen等是需要路径的;
//在windows上，都是需要路径的;
bool GameInforEditer::initGameInfor(string fileFullPath)
{
	Json::Reader reader;
	Json::Value root;
	unsigned long size = 0;
	CCLog("%s Loading gameInfor->",fileFullPath.c_str());

	string doc_str = "";
	if (encrypt)
	{
		AES aes;
		bool suc = aes.ReadFileAndDecryptText(
			fileUtiles->getDocFilePath(fileFullPath).c_str(),
			m_AES_key,doc_str);
		if (!suc)
		{
			CCLog("%s \n=======readInforFile---->Load gameInfor-> %s  fail !", __FUNCTION__, fileFullPath.c_str());
			return false;
		}
	}
	else
	{
		const char* doc = fileUtiles->readDocFile(fileFullPath,&size);
		//文件读取失败就返回NULL;
		if (size<=0)
		{
			CCLog("%s \n=======readInforFile---->Load gameInfor-> %s  fail !", __FUNCTION__, fileFullPath.c_str());
			return false;
		}
		doc_str = string(doc, size);
		delete[] doc;
	}
	

	bool parseOK = parseInforString(doc_str);
	if (!parseOK)
	{
		CCLog("%s", doc_str.c_str());
		LFMessageBox::alert(LFStrings::getValue("cuowu"),LFStrings::getValue("game_infor_error"));
		CCDirector::sharedDirector()->end();
		return false;
	}

	initLastClickVec();

	return true;
}

void GameInforEditer::initLastClickVec()
{
	m_lastClickId.clear();
	std::pair<int, int> temp;
	for (int i = 0; i < GameLayerFactory::getInstance()->getValidSeasonNum(); i++)
	{
		temp.first = i+1,
			temp.second = getLastPlayedSectionIdBySeasonId(temp.first);
		m_lastClickId.push_back(temp);
	}
}

bool GameInforEditer::parseInforString(string str)
{
	Json::Reader reader;
	Json::Value root;
	//解析不成功就返回空;
	if (!reader.parse(str, root))
	{
		CCLog("\n=======parseInforString---->Parse seasonInfo failed !");
		return false;
	}
    
	//解析isFirstRunApp
	{
		Json::Value _isFirstRunApp = root[isFirstRunApp];
		mIsFirstRunApp = _isFirstRunApp.asBool();
	}
	//超能道具使用个数
	{
		if (!root["ToatalUsedSuperToolCount"].isNull())
		{
			m_iToatalUsedSuperToolCount = root["ToatalUsedSuperToolCount"].asInt();
		}
	}
	//开一次宝箱的次数
	{
		if (!root["OneTimesOpenBoxTimes"].isNull())
		{
			m_iOneTimesOpenBoxTimes = root["OneTimesOpenBoxTimes"].asInt();
		}
	}
	//开十次宝箱的次数
	{
		if (!root["TenTimesOpenBoxTimes"].isNull())
		{
			m_iTenTimesOpenBoxTimes = root["TenTimesOpenBoxTimes"].asInt();
		}
	}
	// 评论
	{
		if (!root[needCommentGame].isNull())
		{
			mNeedCommentGame = root[needCommentGame].asBool();
		}else
		{
			mNeedCommentGame = false;
		}

		if (mNeedCommentGame && !root[commentUrl].isNull())
		{
			mCommentUrl = root[commentUrl].asString();
		}else
		{
			mCommentUrl = "";
		}

		if (mNeedCommentGame && !root[successNumAfterNeedComment].isNull())
		{
			mSuccessNumAfterNeedComment = root[successNumAfterNeedComment].asInt();
			if (mSuccessNumAfterNeedComment < 0)
				mSuccessNumAfterNeedComment = 0;
		}else
		{
			mSuccessNumAfterNeedComment = 0;
		}
	}
	{
		Json::Value _id = root[clientId];
		if (!_id.isNull())
		{
			mClientId = _id.asInt();
		}else
		{
			mClientId = 0;
		}
	}
	{
		Json::Value _userId = root["userId_report"];
		if (!_userId.isNull())
		{
			mUserId = _userId.asInt();
		}else
		{
			mUserId = 0;
		}
		Json::Value _userName = root["userName_report"];
		if (!_userName.isNull())
		{
			mUserName = _userName.asString();
		}else
		{
			mUserName.clear();
		}
	}

	{
		Json::Value _payed = root[starNumPayed];
		if (!_payed.isNull())
		{
			mStarNumPayed = _payed.asInt();
		}else
		{
			mStarNumPayed = 0;
		}
	}

	{
		Json::Value _buyed = root["starNumBuyed"];
		if (!_buyed.isNull())
		{
			starNumBuyed = _buyed.asInt();
		}else
		{
			starNumBuyed = 0;
		}
	}
	{
		Json::Value _openBoxNum = root["sOpenBoxNum"];
		if (!_openBoxNum.isNull())
		{
			openBoxNum = _openBoxNum.asInt();
		}else
		{
			openBoxNum = 0;
		}
	}

	{
		Json::Value _lastPlayGameTime = root["lastPlayGameTime"];
		if (!_lastPlayGameTime.isNull())
		{
			lastPlayGameTime = _lastPlayGameTime.asUInt();

			// 超过一天了，那么gap就reset
			if (ServerTime::getTime() - lastPlayGameTime > 24 * 60 * 60) 
			{
				mLastWaitGap = 0;
			}

		}else
		{
			lastPlayGameTime = 0;
		}
	}
    
    {
        Json::Value _lastPlayGameTime = root["PlayTotalTime"];
		if (!_lastPlayGameTime.isNull())
		{
            m_lPlayTotalTime = root["PlayTotalTime"].asDouble();
        }
    }

	{
		Json::Value _num = root["mHelpNumLeft"];
		if (!_num.isNull())
		{
			mHelpNumLeft = _num.asInt();
		}else
		{
			mHelpNumLeft = 0;
		}
	}

	{
		if (!root[phoneNum].isNull())
		{
			mPhoneNumber = root[phoneNum].asString();
		}
	}

	// 支付的历史记录
	{
		if (!root[payHistory].isNull())
		{
			string hist = root[payHistory].asString();
			if (hist.length() > MAX_PAY_HISTORY)
			{
				CCLog("Error : buy history[%s] is biger than season num ,this is bug!",hist.c_str());
				hist = ""; // 直接清空，不要让bug影响
			}
			strncpy(buyHistory,hist.c_str(),MAX_PAY_HISTORY);
		}
	}
	{
		if (!root["HasPayOfficialSolution"].isNull())
		{
			m_bHasPayOfficialSolution = root["HasPayOfficialSolution"].asBool();
		}
	}

	// 解析地区和语言设置
	{
		// 有存值，只存支持的版本，不存放其他值
		if ( !root[initLocale].isNull() )
		{
			mInitLocale = (LanguageType)root[initLocale].asInt();
			if (mInitLocale == kLanguageChinese)
			{
				mInitLocale = kLanguageEnglish;
			}
		}else
		{
			mInitLocale = ResManager::getManager()->getDiviceLanguage();
		}

		// 如果有设置的保存，那么就用上次的设置
		if ( !root[lastSettedLocale].isNull())
		{
			LanguageType l = (LanguageType)root[lastSettedLocale].asInt();
			ResManager::getManager()->setLanguageType(l);
		}
	}
	// 解析是否要忽略所有的锁
	{
		Json::Value _ignoreAllLock = root[ignoreAllLock];
		mIgnoreAllLock = _ignoreAllLock.asBool();
	}
	//声音开关
	{
		{
			if (!root[backgroundSoundOpen].isNull())
			{
				bool backOn = root[backgroundSoundOpen].asBool();
				Music::sharedMusicPlayer()->setBackgroundMute(!backOn);
			}
			if (!root[effctiveSoundOpen].isNull())
			{
				bool effectOn = root[effctiveSoundOpen].asBool();
				Music::sharedMusicPlayer()->setEffectiveMute(!effectOn);
			}
		}
	}
	{// 解析关卡玩耍的信息
		m_mapSection2PlayTime.clear();
		Json::Value _sectionData_array = root[sectionPlayData_array];
		if(!_sectionData_array.isNull() && _sectionData_array.isArray())
		{
			int dataNum = _sectionData_array.size();
			for (int i=0;i<dataNum;i++)
			{
				Json::Value dataInfor =  _sectionData_array[i];

				SectionPlayData data;
				data.sectionId = dataInfor["sectionId"].asInt();
				data.seasonId = dataInfor["seasonId"].asInt();
				data.playedTime = dataInfor["playedTime"].asInt();
				data.buyedTimeLimite = dataInfor["buyedTimeLimite"].asBool();
				data.lastPlayTime = dataInfor["lastPlayTime"].asUInt();
				data.usedHelp = dataInfor["usedHelp"].isNull()?false:dataInfor["usedHelp"].asBool();

				m_mapSection2PlayTime.insert(pair<string,SectionPlayData>(getMapKey(data.seasonId,data.sectionId),data));
			}
		}
	}

	// 解析超能道具
	{
		//"SuperToolArray" : [
		//	{
		//		"type" = 0,
		//		"count" = 1
		//	}
		//]
		mMapSuperToolType2Count.clear();
		Json::Value _sectionData_array = root["SuperToolArray"];
		if(!_sectionData_array.isNull() && _sectionData_array.isArray())
		{
			int dataNum = _sectionData_array.size();
			for (int i=0;i<dataNum;i++)
			{
				Json::Value dataInfor =  _sectionData_array[i];
				int type = 0;//类型现在默认都是0 //dataInfor["type"].asInt();
				int count = 0;
                if (dataInfor["count"].isNumeric())
                {
                    count = dataInfor["count"].asInt();
                }
				mMapSuperToolType2Count.insert(pair<int,int>(type,count));
			}
		}
	}

	// 购买season列表
	{
		mBuyedSeasonIds.clear();
		Json::Value _buyedSeasonHistory_array = root["mBuyedSeasonIds"];
		if(!_buyedSeasonHistory_array.isNull() && _buyedSeasonHistory_array.isArray())
		{
			int dataNum = _buyedSeasonHistory_array.size();
			for (int i=0;i<dataNum;i++)
			{
				Json::Value oneSeason =  _buyedSeasonHistory_array[i];
				int season_id = oneSeason["id"].asInt();
				addSeasonBuyed(season_id);
			}
		}
	}
	//ad 展示时间
	{
		if ( !root["AD_display_delayTime"].isNull() )
		{
			m_iADDelayTime = root["AD_display_delayTime"].asInt();
		}
	}

	//解析seasonInfor_array
	{
		Json::Value _seasonInfor_array = root[seasonInfor_array];

		if(!_seasonInfor_array.isNull() && _seasonInfor_array.isArray())
		{
			int seasonInforNum = _seasonInfor_array.size();
			for (int i=0;i<seasonInforNum;i++)
			{
				Json::Value seasonInfor =  _seasonInfor_array[i];

				SeasonInfor* season_infor = new SeasonInfor();

				season_infor->seasonId = seasonInfor[seasonId].asInt();
				season_infor->isPlayed = seasonInfor[isPlayed].asBool();
				season_infor->locked = seasonInfor[locked].asBool();
				season_infor->score = seasonInfor[score].asInt();
				season_infor->avgScore = seasonInfor[avgScore].asInt();
				season_infor->minScore = seasonInfor[minScore].asInt();
				if (!seasonInfor[season_type].isNull())
				{
					season_infor->seasonType = getItemType(seasonInfor[season_type].asString());
					if (season_infor->seasonType == type_URL && !seasonInfor[extraData].isNull())
					{

						//{
						//	void *vp = static_cast<void*>(new std::string("it's easy to break stuff like this!"));
						//	// Then, in the function that's using the UserEvent:
						//	// Cast it back to a string pointer.
						//	std::string *sp = static_cast<std::string*>(vp);
						//	// You could use 'sp' directly, or this, which does a copy.
						//	std::string s = *sp;
						//	// Don't forget to destroy the memory that you've allocated.
						//	delete sp;
						//}
						season_infor->data = static_cast<void*>(new std::string(seasonInfor[extraData].asString()));
					}
				}

				{//解析每一个Season下面的section的信息，并加入到season_infor中
					Json::Value _sectionInfor_array = seasonInfor[sectionInfor_array];
					if(!_sectionInfor_array.isNull() && _sectionInfor_array.isArray())
					{
						int sectionInforNum = _sectionInfor_array.size();
						for (int j=0;j<sectionInforNum;j++)
						{
							Json::Value sectionInfor =  _sectionInfor_array[j];

							SectionInfor* section_infor = new SectionInfor();
							section_infor->sectionId = sectionInfor[sectionId].asInt();
							section_infor->seasonId = season_infor->seasonId;
							section_infor->isPlayed = sectionInfor[isPlayed].asBool();
							section_infor->locked = sectionInfor[locked].asBool();
							section_infor->iStarNum = sectionInfor[score].asInt();
							section_infor->avgScore = sectionInfor[avgScore].asInt();
							section_infor->minScore = sectionInfor[minScore].asInt();

                            if (!sectionInfor[bUsedSuperTool].isNull())
                            {
                                section_infor->bUseSuperTool = sectionInfor[bUsedSuperTool].asBool();
							}
                            if (!sectionInfor[bUsedHelpMap].isNull())
                            {
                                section_infor->bUseHelpMap = sectionInfor[bUsedHelpMap].asBool();
                            }
                            if (!sectionInfor[fTimeCost].isNull())
                            {
                                section_infor->fTimeCost = sectionInfor[fTimeCost].asDouble();
                            }
                            if (!sectionInfor[fFirstCost].isNull())
                            {
                                section_infor->fFirstCost = sectionInfor[fFirstCost].asDouble();
							}
							if (!sectionInfor[uClickedTimes].isNull())
							{
								section_infor->effectiveClickNum = sectionInfor[uClickedTimes].asUInt();
							}
							if (!sectionInfor[iRankingScore].isNull())
							{
								section_infor->iRankingScore = sectionInfor[iRankingScore].asInt();
							}

							if (!sectionInfor["isSuccessed"].isNull())
							{
								section_infor->isSuccessed = sectionInfor["isSuccessed"].asBool();
							}else
							{
								section_infor->isSuccessed = (section_infor->iStarNum > 0);
							}

							if (!sectionInfor["earliestCanPlayTime"].isNull())
							{
								section_infor->earliestCanPlayTime = sectionInfor["earliestCanPlayTime"].asUInt();
							}

							if (!sectionInfor[keyItemMedalNum].isNull())
								section_infor->keyItemMedalNum = sectionInfor[keyItemMedalNum].asInt();
							if (!sectionInfor[keyTableMedalNum].isNull())
								section_infor->keyTableMedalNum = sectionInfor[keyTableMedalNum].asInt();

							if (!sectionInfor[section_type].isNull())
							{
								section_infor->sectionType = getItemType(sectionInfor[section_type].asString());
							}

							//加入到season_infor中
							season_infor->addSectionInfor(section_infor);
						}
					}

				}

				addSeasonInfor(season_infor);
			}
		}
	}
	return true;
}

/**
 *	返回的只是游戏保存的游戏信息，这个list的大小不能代替Season的数量
 *	Season的数量要通过GameLayerFactory获取
 */
list<SeasonInfor*> GameInforEditer::getSessonInforList()
{
	return mSeasonInfors;
}
//
int GameInforEditer::getLastPlayedSeasonId()
{
	int ret = 1;

	list<SeasonInfor*>::iterator it = mSeasonInfors.begin();
	for (;it!=mSeasonInfors.end();it++)
	{
		if ((*it)->isPlayed && (*it)->seasonId > ret)
		{
			ret = (*it)->seasonId;
		}
	}

	return ret;
}
int GameInforEditer::getLastPlayedSectionId()
{
	int ret = 1;

	SeasonInfor *pSeason = getSeasonInfor(getLastPlayedSeasonId());

	list<SectionInfor*> pSections = pSeason->getAllSectionInfors();
	for (list<SectionInfor*>::iterator it=pSections.begin(); it!=pSections.end(); ++it)
	{
		if ((*it)->isPlayed && (*it)->sectionId > ret)
		{
			ret = (*it)->sectionId;
		}
	}

	return ret;
}

int GameInforEditer::getLastPlayedSectionIdBySeasonId(int seasonId)
{
	int ret = 1;

	SeasonInfor *pSeason = getSeasonInfor(seasonId);
	if (!pSeason)
	{
		return ret;
	}

	list<SectionInfor*> pSections = pSeason->getAllSectionInfors();
	for (list<SectionInfor*>::iterator it=pSections.begin(); it!=pSections.end(); ++it)
	{
		if ((*it)->isPlayed && (*it)->sectionId > ret)
		{
			ret = (*it)->sectionId;
		}
	}

	return ret;
}

//
int GameInforEditer::getLastUnlockedSeasonId()
{
	int ret = 1;

	list<SeasonInfor*>::iterator it = mSeasonInfors.begin();
	for (;it!=mSeasonInfors.end();it++)
	{
		if ((*it)->locked == false && (*it)->seasonId > ret)
		{
			ret = (*it)->seasonId;
		}
	}

	return ret;
}

bool GameInforEditer::saveSessonInforList()
{
	string seasonInforString = getSeasonInfoListString();

	if (encrypt)
	{
		AES aes;
		return aes.saveFileAndEncryptText(
			fileUtiles->getDocFilePath(writeGameInforPath).c_str(),
			m_AES_key,seasonInforString);
	}
	bool sec = false;
	sec = fileUtiles->writeToDocFile(writeGameInforPath,seasonInforString);
	return sec;
}

string GameInforEditer::getSeasonInfoListString()
{
	Json::Value root;
	
	//生成isFirstRunApp的信息
	{
		root[isFirstRunApp] = Json::Value(mIsFirstRunApp);
	}
	{
		if (mNeedCommentGame && mCommentUrl != "")
		{
			root[needCommentGame] = Json::Value(mNeedCommentGame);
			root[commentUrl] = Json::Value(mCommentUrl);
			root[successNumAfterNeedComment] = Json::Value(mSuccessNumAfterNeedComment);
		}else
		{
			root[needCommentGame] = Json::Value(false);
		}
	}

	{
		root[clientId] = Json::Value(mClientId);
		root["userId_report"] = Json::Value(mUserId);
		root["userName_report"] = Json::Value(mUserName.c_str());
	}
	{
		root[starNumPayed] = Json::Value(mStarNumPayed);
	}
	{
		root["starNumBuyed"] = Json::Value(starNumBuyed);
	}
	{
		root["sOpenBoxNum"] = Json::Value(openBoxNum);
	}
	{
		root["mHelpNumLeft"] = Json::Value(mHelpNumLeft);
	}
	{
		root[phoneNum] = Json::Value(mPhoneNumber);
	}
	//超能道具使用个数
	{
		root["ToatalUsedSuperToolCount"] = Json::Value(m_iToatalUsedSuperToolCount);
	}
	//开一次宝箱的次数
	{
		root["OneTimesOpenBoxTimes"] = Json::Value(m_iOneTimesOpenBoxTimes);
	}
	//开十次宝箱的次数
	{
		root["TenTimesOpenBoxTimes"] = Json::Value(m_iTenTimesOpenBoxTimes);
	}
	// 支付的历史记录
	{
		string his = string(buyHistory);
		root[payHistory] = Json::Value(his);
		CCLog("============>payHistory = %s",his.c_str());
	}

	{
		root["HasPayOfficialSolution"] = Json::Value(m_bHasPayOfficialSolution);
	}

	//生成ignoreAllLock的信息
	{
		root[ignoreAllLock] = Json::Value(mIgnoreAllLock);
	}

	// 解析地区和语言设置
	{
		root[initLocale] = Json::Value(mInitLocale);
		LanguageType settedLocale = ResManager::getManager()->getLanguageType();
		root[lastSettedLocale] = Json::Value(settedLocale);
	}

	//声音开关
	{
		root[backgroundSoundOpen] = Json::Value( !(Music::sharedMusicPlayer()->isBackgroundMute()));
		root[effctiveSoundOpen] = Json::Value( !(Music::sharedMusicPlayer()->isEffectiveMute()));
	}
	//广告时间
	{
		root["AD_display_delayTime"] = Json::Value((int)m_iADDelayTime);
	}

	{
		root["lastPlayGameTime"] = Json::Value((unsigned int)lastPlayGameTime);
	}
    
    {
        root["PlayTotalTime"] = Json::Value((double)m_lPlayTotalTime);
    }

	// 生成完的记录
	{
		map<string,SectionPlayData>::iterator it;
		for (it = m_mapSection2PlayTime.begin(); it!=m_mapSection2PlayTime.end(); it++)
		{
			Json::Value _seasonPlayData;
			_seasonPlayData["sectionId"] = Json::Value(it->second.sectionId);
			_seasonPlayData["seasonId"] = Json::Value(it->second.seasonId);
			_seasonPlayData["playedTime"] = Json::Value(it->second.playedTime);
			_seasonPlayData["usedHelp"] = Json::Value(it->second.usedHelp);
			_seasonPlayData["buyedTimeLimite"] = Json::Value(it->second.buyedTimeLimite);
			_seasonPlayData["lastPlayTime"] = Json::Value((unsigned int)it->second.lastPlayTime);
			
			root[sectionPlayData_array].append(_seasonPlayData);
		}
	}

	{
		// 购买season列表
		for (int i=0,count = mBuyedSeasonIds.size(); i<count; i++)
		{
			Json::Value _oneSeason;
			_oneSeason["id"] = Json::Value(mBuyedSeasonIds[i]);
			root["mBuyedSeasonIds"].append(_oneSeason);
		}
	}

	// 保存超能道具
	{
		//"SuperToolArray" : [
		//	{
		//		"type" = 0,
		//		"count" = 1
		//	}
		//]
		map<int,int>::iterator it;
		for (it = mMapSuperToolType2Count.begin(); it!=mMapSuperToolType2Count.end(); it++)
		{
			Json::Value _item;
			_item["type"] = Json::Value(it->first);
			_item["count"] = Json::Value(it->second);

			root["SuperToolArray"].append(_item);
		}
	}

	//生成seasonInfor_array
	{
		//不要这样写，这样的话没有用，只能root[seasonInfor_array]这样写
		//Json::Value seasonArray = root[seasonInfor_array];
		std::list<SeasonInfor*>::iterator it;
		for (it=mSeasonInfors.begin(); it!=mSeasonInfors.end(); ++it)
		{
			Json::Value _oneSeasonInfor;
			SeasonInfor* _seasonInfor = (*it);
			_oneSeasonInfor[seasonId] = Json::Value(_seasonInfor->seasonId);
			_oneSeasonInfor[season_type] = Json::Value(type_string[_seasonInfor->seasonType]);
			_oneSeasonInfor[isPlayed] = Json::Value(_seasonInfor->isPlayed);
			_oneSeasonInfor[locked] = Json::Value(_seasonInfor->locked);
			_oneSeasonInfor[score] = Json::Value(_seasonInfor->score);
			_oneSeasonInfor[avgScore] = Json::Value(_seasonInfor->avgScore);
			_oneSeasonInfor[minScore] = Json::Value(_seasonInfor->minScore);

			if (_seasonInfor->seasonType == type_URL && _seasonInfor->data!=NULL)
			{
				std::string *sp = static_cast<std::string*>(_seasonInfor->data);
				std::string s = *sp;
				_oneSeasonInfor[extraData] = Json::Value(s);
				//{
				//	void *vp = static_cast<void*>(new std::string("it's easy to break stuff like this!"));
				//	// Then, in the function that's using the UserEvent:
				//	// Cast it back to a string pointer.
				//	std::string *sp = static_cast<std::string*>(vp);
				//	// You could use 'sp' directly, or this, which does a copy.
				//	std::string s = *sp;
				//	// Don't forget to destroy the memory that you've allocated.
				//	delete sp;
				//}
			}


			//不要这样写，这样的话没有用，只能_oneSeasonInfor[sectionInfor_array]这样写
			//Json::Value sectionArray = _oneSeasonInfor[sectionInfor_array];
			//保持Season下面的section：sectionInfor_array
			{
				list<SectionInfor*> sectionList = _seasonInfor->getAllSectionInfors();
				std::list<SectionInfor*>::iterator it_section;
				for (it_section=sectionList.begin(); it_section!=sectionList.end(); ++it_section)
				{
					Json::Value _oneSectionInSeason;

					SectionInfor* _sectionInfor = (*it_section);

					//_oneSectionInSeason[seasonId] = Json::Value(_sectionInfor->seasonId);//seasonId 就不用保存了
					_oneSectionInSeason[sectionId] = Json::Value(_sectionInfor->sectionId);
					_oneSectionInSeason[section_type] = Json::Value(type_string[_sectionInfor->sectionType]);
					_oneSectionInSeason[isPlayed] = Json::Value(_sectionInfor->isPlayed);
					_oneSectionInSeason["isSuccessed"] = Json::Value(_sectionInfor->isSuccessed);
					_oneSectionInSeason[locked] = Json::Value(_sectionInfor->locked);
					_oneSectionInSeason["earliestCanPlayTime"] = Json::Value(_sectionInfor->earliestCanPlayTime);
					_oneSectionInSeason[score] = Json::Value(_sectionInfor->iStarNum);
					_oneSectionInSeason[avgScore] = Json::Value(_sectionInfor->avgScore);
					_oneSectionInSeason[minScore] = Json::Value(_sectionInfor->minScore);
					_oneSectionInSeason[keyItemMedalNum] = Json::Value(_sectionInfor->keyItemMedalNum);
					_oneSectionInSeason[keyTableMedalNum] = Json::Value(_sectionInfor->keyTableMedalNum);
					_oneSectionInSeason[bUsedSuperTool] = Json::Value(_sectionInfor->bUseSuperTool);
					_oneSectionInSeason[bUsedHelpMap] = Json::Value(_sectionInfor->bUseHelpMap);
					_oneSectionInSeason[fTimeCost] = Json::Value(_sectionInfor->fTimeCost);
					_oneSectionInSeason[fFirstCost] = Json::Value(_sectionInfor->fFirstCost);
					_oneSectionInSeason[uClickedTimes] = Json::Value(_sectionInfor->effectiveClickNum);
					_oneSectionInSeason[iRankingScore] = Json::Value(_sectionInfor->iRankingScore);
					//sectionArray.append(_oneSectionInSeason);
					_oneSeasonInfor[sectionInfor_array].append(_oneSectionInSeason);
				}
			}

			//加入一个SeasonInfor数据
			//seasonArray.append(_oneSeasonInfor);
			root[seasonInfor_array].append(_oneSeasonInfor);
		}
	}

	//得到配置文本
	Json::StyledWriter styled_writer;

	return styled_writer.write(root);
}

bool GameInforEditer::isFirstRunMe()
{
	return mIsFirstRunApp;
}
bool GameInforEditer::isIgnoreAllLock()
{
	return mIgnoreAllLock;
}

void GameInforEditer::firstRunOverApp()
{
	mIsFirstRunApp = false;
	saveSessonInforList();
}
void GameInforEditer::makeNew()
{
	mIsFirstRunApp = true;
	saveSessonInforList();
}

SectionInfor* GameInforEditer::getSectionInfor(int seasonId,int sectionId)
{
	SeasonInfor* _seasonInfor = getSeasonInfor(seasonId);
	if (_seasonInfor == NULL)
	{
		CCLog("\nGameInforEditer::getSectionInfor---->seasonInfor is null !");
		return NULL;
	}

	list<SectionInfor*> sectionsInSeason = _seasonInfor->getAllSectionInfors();
	list<SectionInfor*>::iterator it;
	for (it=sectionsInSeason.begin(); it!=sectionsInSeason.end(); ++it)
	{
		SectionInfor* _sectionInfo = (*it);
		if (_sectionInfo->sectionId == sectionId)
		{
			return _sectionInfo;
		}
	}

	CCLog("\nGameInforEditer::getSectionInfor--->there is no SectionInfor whitch seasonId = %d,sectionId = %d ",seasonId,sectionId);
	return NULL;
}
bool GameInforEditer::needComments()
{
	return mNeedCommentGame;
}
string GameInforEditer::getCommentUrl()
{
	return mCommentUrl;
}
void GameInforEditer::setNeedComment(bool need,string url)
{
	mNeedCommentGame = need;
	mCommentUrl = url;

	CCLog("GameInforEditer::setNeedComment need = %s,url = %s",need?"true":"false",url.c_str());
}

void GameInforEditer::setSuccessNumAfterNeedComments(int num)
{
	mSuccessNumAfterNeedComment = num;
	if (mSuccessNumAfterNeedComment < 0)
		mSuccessNumAfterNeedComment = 0;
}
int GameInforEditer::getSuccessNumAfterNeedComments()
{
	return mSuccessNumAfterNeedComment;
}

//// 保存和获取支付信息
//void GameInforEditer::setPayInfor(SPCmd_QueryPayChannelInfoRsp infor)
//{
//	if (infor.err_code != Success)
//	{
//		return;
//	}
//	payInfor = infor;
//	//saveSessonInforList();
//}
//bool GameInforEditer::getPayInfor(SPCmd_QueryPayChannelInfoRsp& out_infor)
//{
//	if (payInfor.err_code == Success)
//	{
//		out_infor = payInfor;
//		return true;
//	}
//
//	return false;
//}

string GameInforEditer::getPhoneNumber()
{
	// 每次都查询一次
	string ret = "9090990";
	if (ret != "" && ret != mPhoneNumber)
	{
		mPhoneNumber = ret;
		saveSessonInforList();
	}

	return mPhoneNumber;
}

void GameInforEditer::setUserInputPhoneNumber(string number)
{
	if (number != "" && number != mPhoneNumber)
	{
		mPhoneNumber = number;
		saveSessonInforList();
	}
}

void GameInforEditer::addBuySeasonHistory(int seasonId)
{
	if (seasonId < 1 && seasonId > GameLayerFactory::getInstance()->getConfigSeasonNum(type_Normal))
	{
		CCAssert(false,"异常");
		return;
	}

	if (GameLayerFactory::getInstance()->getConfigSeasonNum(type_Normal) > MAX_PAY_HISTORY)
	{
		CCAssert(false,"不支持，太多的关了");
		return;
	}
	buyHistory[seasonId-1] = '1';
	saveSessonInforList();
}
void GameInforEditer::addBuyAllHistory()
{
	int seasonNum = GameLayerFactory::getInstance()->getConfigSeasonNum(type_Normal);
	if (seasonNum > MAX_PAY_HISTORY)
	{
		CCAssert(false,"不支持这么多Season");
		return;
	}
	
	// 全部都买了
	for (int i = 0;i<seasonNum;i++)
	{
		buyHistory[i] = '1';
	}
	saveSessonInforList();
}

bool GameInforEditer::isWholeSeasonSuccessed(int seasonId)
{
	SeasonInfor* seasonInfor = getSeasonInfor(seasonId);
	if (seasonInfor == NULL)
		return false;

	int success_num = 0;
	list<SectionInfor*> sections = seasonInfor->getNormalSectionInfors();
	list<SectionInfor*>::iterator it = sections.begin();
	for (;it != sections.end(); it++)
	{
		// 任何一关没成功就是false,或者是锁住的，就是false
		if (!(*it)->isSuccessed || (*it)->locked) 
		{
			return false;
		}
		success_num ++;
	}

	return (success_num >= GameLayerFactory::getInstance()->getNomalSectionNumBySeasonId(seasonId));
}
bool GameInforEditer::canUnclock(int seasonId, bool needSpecialItem)
{
	if (seasonId < 0 || seasonId > GameLayerFactory::getInstance()->getConfigSeasonNum())
		return false;

	// 如果忽略了锁，那么就认为是不需要再解锁了
	if (isIgnoreAllLock())
		return false;

#if NOT_AutoUnlockSeason == 1
	// 不提供自动解锁，那么就都不能解锁，直接返回false
	return false;
#endif

	// 用星星的数量限制解锁
#if StarNum_Unlock_Season == 1
	{
		int unlockScore = getScoreForUnlock(seasonId);
		int totalScore =  getTotalScore();
		// 如果分数还不够，直接返回false
		if (totalScore < unlockScore)
			return false;
	}
#endif

	// 如果这一关已经解锁了，就返回false
	SeasonInfor* infor =  getSeasonInfor(seasonId);
	if (infor && infor->locked == false)
		return false;

	// 上一章的最后一关完毕就解锁下一章
#if LAST_SEASON_UNLOCK_NEW_SEASON == 1  
	{
		int section_id = GameLayerFactory::getInstance()->getNomalSectionNumBySeasonId(seasonId-1);
//		SectionInfor* section = getSectionInfor(seasonId-1,section_id);
		// 如果上一章还没完成，就不能解锁
		if (!isWholeSeasonSuccessed(seasonId-1))
		{
			return false;
		}
	}
#endif

#if	COLLECTION_ALL_SPECIAL_ITEM == 1
	{
		if (needSpecialItem){
		//就算集齐了  也要播放解锁动画， 所以全部返回false，看完解锁动画后才解锁
			return false;
		}else {
			vector<JuqingDaoJu> vec = JuqingDaoJuManager::getManager()->getAllItemsInSeason(seasonId-1);
			for (int i=0; i<vec.size(); ++i)
			{
				if (vec.at(i).state == DJS_NotGetted)
					return false;
			}
		}
	}
#endif

	return true;
}

void GameInforEditer::addSeasonBuyed(int seasonId)
{
	for (int i=0,count = mBuyedSeasonIds.size(); i<count; i++)
	{
		if (mBuyedSeasonIds[i] == seasonId)
		{
			return;
		}
	}

	mBuyedSeasonIds.push_back(seasonId);
}
bool GameInforEditer::isBuyedSeason(int seasonId)
{
	for (int i=0,count = mBuyedSeasonIds.size(); i<count; i++)
	{
		if (mBuyedSeasonIds[i] == seasonId)
		{
			return true;
		}
	}

	return false;
}

void GameInforEditer::unLockSeasonForce(int seasonId)
{
	if (seasonId < 0 || seasonId > GameLayerFactory::getInstance()->getConfigSeasonNum())
		return;

	SeasonInfor* unlockSeasonInfo = getSeasonInfor(seasonId);
	SectionInfor* unlockSectionInfo = getSectionInfor(seasonId,1);
	if (unlockSeasonInfo == NULL)
	{//如果没有配置数据，那么就创建一个插入进入
		unlockSeasonInfo = new SeasonInfor(seasonId,false,false);
		addSeasonInfor(unlockSeasonInfo);
	}else
	{
		unlockSeasonInfo->locked = false;
	}

	if (unlockSectionInfo == NULL)
	{//如果下一节没有配置
		unlockSectionInfo = new SectionInfor(seasonId,1,false,false);
		unlockSeasonInfo->addSectionInfor(unlockSectionInfo);
	}else
	{
		unlockSectionInfo->locked = false;
	}
}

void GameInforEditer::unLockSecretStageBySeason(int seasonId)
{
	// 1.先获取seasoninfo
	SeasonInfor* season = getSeasonInfor(seasonId);
	CCAssert(season!=NULL,"");

	// 2.再添加每一个section都解锁的
	int sectionNum = GameLayerFactory::getInstance()->getAllSectionNumBySeasonId(seasonId);
	int icount = getSecretStageNumBySeasonId(seasonId);
	for (int i = sectionNum - icount; i < sectionNum; i++)
	{
		SectionInfor* sectionInfor = GameInforEditer::getGameInfor()->getSectionInfor(seasonId, i+1);
		if (sectionInfor == NULL)
		{
			sectionInfor = new SectionInfor(seasonId,i+1,true,false);
			sectionInfor->iStarNum = 0;
			sectionInfor->isPlayed = false;
			sectionInfor->locked = true;
			sectionInfor->isSuccessed = false;
			season->addSectionInfor(sectionInfor);
		}
	}
	// 3. 保存
	saveSessonInforList();
}

int GameInforEditer::getTotalScoreBeforeSeasonId(int seasonId)
{
	int ret = 0;

	std::list<SeasonInfor*>::iterator it;
	for (it = mSeasonInfors.begin(); it != mSeasonInfors.end(); ++it)
	{
		SeasonInfor* _seasonInfor = (*it);
		if (_seasonInfor->seasonId > seasonId)
		{
			continue;
		}
		if (_seasonInfor->seasonId == seasonId)
		{
			ret += _seasonInfor->getMyNormalSectionScore();
		}
		else
		{
			ret += _seasonInfor->getMyAllSectionScore();
		}
	}

	return ret;
}

int GameInforEditer::getSecretStageNumBySeasonId(int seasonId)
{
	int ret = 0;
	for (size_t i = 0; i < m_lLockedSeason.size(); i++)
	{
		if (m_lLockedSeason.at(i).first == seasonId)
		{
			++ret;
		}
	}

	return ret;
}

void GameInforEditer::unLockSeasonAndItsSectionsUseMoney(int seasonId)
{
	addSeasonBuyed(seasonId);

	// 1.先解锁season
	unLockSeasonForce(seasonId);

	SeasonInfor* season = getSeasonInfor(seasonId);
	CCAssert(season!=NULL,"");

	// 2.再添加每一个section都解锁的，都buyTimeLimit的标记
	int sectionNum = GameLayerFactory::getInstance()->getAllSectionNumBySeasonId(seasonId);
	for (int i=0;i<sectionNum;i++)
	{
		SectionInfor* sectionInfor = GameInforEditer::getGameInfor()->getSectionInfor(seasonId,i+1);
		if (sectionInfor == NULL)
		{
			sectionInfor = new SectionInfor(seasonId,i+1,true,false);
			sectionInfor->iStarNum = 0;
			sectionInfor->isSuccessed = false;
			season->addSectionInfor(sectionInfor);
		}else
		{
			sectionInfor->locked = false;
		}

		// 设置buyTimeLimite标记
		setSectionTimeLimiteBuyed(seasonId,i+1);
	}

	// 3. 保存
	saveSessonInforList();
}

int GameInforEditer::getScoreForUnlock(int seasonId)
{
	CCAssert(StarNum_Unlock_Season == 1,"是用星星的个数来限制解锁的情况才会用这个函数");

	int needStarNum[] = {
		0,40,80,120,160
	};
	if (seasonId < 1 || seasonId > sizeof(needStarNum)/sizeof(int))
	{
		CCLog("waring GameInforEditer::getScoreForUnlock seasonId must >= 1 and <= %d . current seasonId = %d", sizeof(needStarNum)/sizeof(int),seasonId);
		return -1;
	}

	return needStarNum[seasonId-1];
}

int GameInforEditer::getTotalFreeScore()
{
	int total = getTotalScore();
	CCAssert(total >= mStarNumPayed,"不可能消费的星星比得到的还多");
	return total - mStarNumPayed;
}

int GameInforEditer::getTotalScore(bool bAll/*=true*/)
{
	int ret = 0;

	std::list<SeasonInfor*>::iterator it;
	for (it=mSeasonInfors.begin(); it!=mSeasonInfors.end(); ++it)
	{
		SeasonInfor* _seasonInfor = (*it);
		ret += _seasonInfor->score;
	}

	if (bAll)
	{
		ret += starNumBuyed;
	}
	return ret;
}
SeasonInfor* GameInforEditer::getSeasonInfor(int seasonId)
{
	std::list<SeasonInfor*>::iterator it;
	for (it=mSeasonInfors.begin(); it!=mSeasonInfors.end(); ++it)
	{
		SeasonInfor* _seasonInfor = (*it);
		if (_seasonInfor->seasonId == seasonId)
		{
			return _seasonInfor;
		}
	}
	CCLog("\nGameInforEditer::getSeasonInfor--->there is no SeasonInfor whitch seasonId = %d",seasonId);
	return NULL;
}

void GameInforEditer::addSeasonInfor(SeasonInfor* infor)
{
	mSeasonInfors.insert(mSeasonInfors.end(),infor);
}



string GameInforEditer::getMapKey(int seasonId,int sectionId)
{
	char temp[16];
	sprintf(temp,"%d_%d",seasonId,sectionId);
	return temp;
}
//void GameInforEditer::decSectionPlayTime(int seasonId,int sectionId)
//{
//	string key = getMapKey(seasonId,sectionId);
//	map<string,SectionPlayData>::iterator it = m_mapSection2PlayTime.find(key);
//	if (it == m_mapSection2PlayTime.end())
//	{
//		CCAssert(false,"没有找到");
//		return;
//	}
//	// 扣次数
//	it->second.playedTime --;
//	if (it->second.playedTime < 0)
//		it->second.playedTime = 0;
//}
void GameInforEditer::incSectionPlayTime(int seasonId,int sectionId)
{
	string key = getMapKey(seasonId,sectionId);
	map<string,SectionPlayData>::iterator it = m_mapSection2PlayTime.find(key);
	if (it == m_mapSection2PlayTime.end())
	{
		SectionPlayData playTime;
		playTime.seasonId = seasonId;
		playTime.sectionId = sectionId;
		playTime.playedTime = 0;
		playTime.lastPlayTime = ServerTime::getTime();
		m_mapSection2PlayTime.insert(pair<string,SectionPlayData>(key,playTime));

		it = m_mapSection2PlayTime.find(key);
		CCAssert(it != m_mapSection2PlayTime.end(),"");
	}

	// 间隔大于一天就清空记录
	refreshPlayedTimeToday(seasonId,sectionId);
	it->second.playedTime ++;
	it->second.lastPlayTime = ServerTime::getTime();

	// 更新最后一次玩的时间
	lastPlayGameTime = ServerTime::getTime();

	// 校验
	if (!it->second.buyedTimeLimite)
		CCAssert(it->second.playedTime <= MAX_PLAY_TIME,"没有卖的最多只能玩10次");
}

int GameInforEditer::refreshPlayedTimeToday(int seasonId,int sectionId)
{
	string key = getMapKey(seasonId,sectionId);
	map<string,SectionPlayData>::iterator it = m_mapSection2PlayTime.find(key);
	if (it == m_mapSection2PlayTime.end())
	{
		return 0;
	}
	else
	{
		// 变更时间
		//if ( abs(it->second.lastPlayTime - ServerTime::getTime()) >= PLAY_TIME_REFRESH_GAP )
		if ( ServerTime::getDistance(it->second.lastPlayTime,ServerTime::getTime() ) >= PLAY_TIME_REFRESH_GAP )
		{
			it->second.playedTime = 0;
		}
	}

	return it->second.playedTime;
}
bool GameInforEditer::isSectionTimeLimiteBuyed(int seasonId,int sectionId)
{
	SectionPlayData* data = getSectionPlayData(seasonId,sectionId);
	if (data!=NULL && data->buyedTimeLimite)
	{
		return true;
	}
	return false;
}
void GameInforEditer::setSectionTimeLimiteBuyed(int seasonId,int sectionId)
{
	string key = getMapKey(seasonId,sectionId);
	map<string,SectionPlayData>::iterator it = m_mapSection2PlayTime.find(key);
	if (it == m_mapSection2PlayTime.end())
	{
		SectionPlayData playTime;
		playTime.seasonId = seasonId;
		playTime.sectionId = sectionId;
		playTime.playedTime = 0;
		playTime.buyedTimeLimite = true;
		m_mapSection2PlayTime.insert(pair<string,SectionPlayData>(key,playTime));

		it = m_mapSection2PlayTime.find(key);
		CCAssert(it != m_mapSection2PlayTime.end(),"");
	}

	it->second.buyedTimeLimite = true;
}

void GameInforEditer::setSectionHelpUsed(int seasonId,int sectionId)
{
	string key = getMapKey(seasonId,sectionId);
	map<string,SectionPlayData>::iterator it = m_mapSection2PlayTime.find(key);
	if (it == m_mapSection2PlayTime.end())
	{
		SectionPlayData playTime;
		playTime.seasonId = seasonId;
		playTime.sectionId = sectionId;
		playTime.playedTime = 0;
		playTime.buyedTimeLimite = false;
		playTime.usedHelp = true;
		m_mapSection2PlayTime.insert(pair<string,SectionPlayData>(key,playTime));

		it = m_mapSection2PlayTime.find(key);
		CCAssert(it != m_mapSection2PlayTime.end(),"");
	}

	it->second.usedHelp = true;
}

const SectionPlayData* GameInforEditer::getReadOnlySectionPlayData(int seasonId,int sectionId)
{
	return getSectionPlayData(seasonId,sectionId);
}

SectionPlayData* GameInforEditer::getSectionPlayData(int seasonId,int sectionId)
{
	string key = getMapKey(seasonId,sectionId);
	map<string,SectionPlayData>::iterator it = m_mapSection2PlayTime.find(key);
	if (it == m_mapSection2PlayTime.end())
	{
		return NULL;
	}
	return &it->second;
}

int GameInforEditer::getBuySectionTimeLimiteCostMoney()
{
	return 1; // 一元
}
int GameInforEditer::getSectionPlayToCostStarNum(int seasonId,int sectionId)
{
	//return 10000;
	// 购买过了就不限制了
	if (isSectionTimeLimiteBuyed(seasonId,sectionId))
	{
		return 0;
	}
	
	// 看免费的还有没
	int playedNum = refreshPlayedTimeToday(seasonId,sectionId);
	// 注意这里的playedNum是已经玩了的次数，playedNum+1才是这次玩所在的次数

	if (playedNum < MAX_FREE_PLAY_TIME)
	{
		return 0;
	}

	if (playedNum >= 5 && playedNum < 8) // 已经玩了5次了，那么再玩就一块钱了
	{
		return 1;
	}

	if (playedNum >= 8 && playedNum < 10 )
	{
		return 2;
	}
	
	CCAssert(false,"这不可能走过来");
	return 0;
}
int GameInforEditer::getSectionFreePlayTimeLeft(int seasonId,int sectionId)
{
	if (isSectionTimeLimiteBuyed(seasonId,sectionId))
	{
		CCAssert(false,"为了避免歧义，购买过的就不应该调用这个方法，因为购买过的返回的应该是无穷大");
	}

	//
	return MAX_FREE_PLAY_TIME - refreshPlayedTimeToday(seasonId,sectionId);
}
int GameInforEditer::getSectionPlayTimeLeft(int seasonId,int sectionId)
{
	if (isSectionTimeLimiteBuyed(seasonId,sectionId))
	{
		CCAssert(false,"为了避免歧义，购买过的就不应该调用这个方法，因为购买过的返回的应该是无穷大");
	}

	//
	return MAX_PLAY_TIME - refreshPlayedTimeToday(seasonId,sectionId);
}
int GameInforEditer::getHelpTimeLeft()
{
	return mHelpNumLeft;
}
void GameInforEditer::subMyStarNum(size_t toSub)
{
	if (getTotalFreeScore() >= (int)toSub)
	{
		mStarNumPayed += toSub;
		CCLog("%s-->tosub = %d",__FUNCTION__,toSub);
	}
	else
	{
		CCAssert(false,"个数不够，不该调用");
	}
	
}

void GameInforEditer::subHelpNum(size_t toSub)
{
	if (mHelpNumLeft >= (int)toSub)
	{
		mHelpNumLeft -= toSub;
	}
	else
	{
		CCAssert(false,"个数不够，不该调用");
	}
}

void GameInforEditer::addHelpNum(size_t toAdd)
{
	mHelpNumLeft += toAdd;

	GameInforEditer::getGameInfor()->saveSessonInforList();
}

void GameInforEditer::addBuyedStarNum(size_t num)
{
	starNumBuyed += num;

	GameInforEditer::getGameInfor()->saveSessonInforList();
}

void GameInforEditer::addSuperTool(size_t count, int type/* = 0*/)
{
	map<int,int>::iterator it = mMapSuperToolType2Count.find(type);
	if (it == mMapSuperToolType2Count.end())
	{
		mMapSuperToolType2Count.insert(pair<int,int>(type,count));
	}else
	{
		it->second += count;
	}

	GameInforEditer::getGameInfor()->saveSessonInforList();
}
void GameInforEditer::subSuperTool(size_t count, int type/* = 0*/)
{
	map<int,int>::iterator it = mMapSuperToolType2Count.find(type);
	if (it == mMapSuperToolType2Count.end() || it->second < (int)count)
	{
		CCAssert(false,"没有找到，或者个数不够");
		return;
	}
	it->second -= count;
	
	//超能道具使用个数
	m_iToatalUsedSuperToolCount += count;

	//当前关卡使用超能道具
	SectionInfor* info = getSectionInfor(LevelManager::getManager()->getLastSeasonId(), LevelManager::getManager()->getLastSectionId());
	if (info)
	{
		info->bUseSuperTool = true;
	}

	GameInforEditer::getGameInfor()->saveSessonInforList();

	//analytics
	//统计超能道具的使用情况
	CmdHelper::getInstance()->getAnalyticsInstance()->onUse("超能道具", 1, "Unknown");
}
int GameInforEditer::getSuperToolCount(int type/* = 0*/)
{
	map<int,int>::iterator it = mMapSuperToolType2Count.find(type);
	if (it == mMapSuperToolType2Count.end())
	{
		return 0;
	}
	return it->second;
}

void GameInforEditer::startSectionTimeCounter(int seasonId,int sectionId)
{
	SectionInfor* infor = getSectionInfor(seasonId,sectionId);
	if (infor == NULL)
	{
		CCAssert(false,"都没有信息，不能计时，应该先解锁之后才可以计时");
		return;
	}

	// 时间等待
	int timeToWait = getTimeToWaitForFirstPlay(seasonId,sectionId);
	infor->earliestCanPlayTime = timeToWait;
	infor->earliestCanPlayTime += ServerTime::getTime();
}

// 获取某一关解锁还需要等待的时间(秒)
int GameInforEditer::getTimeToWaitForFirstPlay(int seasonId,int sectionId)
{
	if (isBuyedSeason(seasonId))
	{
		return 0;
	}

	// 如果已经指定过可玩时间的话，那么只要时间是到的就有了
	SectionInfor* section_infor = getSectionInfor(seasonId,sectionId);
	if (section_infor != NULL && section_infor->earliestCanPlayTime > 0)
	{
		size_t curTime = ServerTime::getTime();

		return (curTime > section_infor->earliestCanPlayTime) ? 0 : (section_infor->earliestCanPlayTime - curTime); 
	}

	int gap = GameLayerFactory::getInstance()->getSectionIndex(seasonId,sectionId) - 11 + 1;
	if (gap > 0)
	{
		return gap * WAIT_GAP_INC;
	}
	
	return 0;

	//int curentNeedWaitGap = mLastWaitGap + WAIT_GAP_INC;
	//int time = lastPlayGameTime - ServerTime::getTime(); //返回的就是秒数
	//int waitTime = curentNeedWaitGap - time;
	//if (waitTime < 0)
	//	waitTime = 0;
	//
	//return waitTime;
}
// 获取某一个解锁需要的的星星数量
int GameInforEditer::getStarCostForFirstPlay(int seasonId,int sectionId)
{
	// 玩过了，就是0
	SectionInfor* section = getSectionInfor(seasonId,sectionId);
	if (section != NULL && section->isPlayed)
	{
		return 0;
	}

	if (isBuyedSeason(seasonId))
	{// 买过的，就直接返回0
		return 0;
	}
	

	if (seasonId == 1 && sectionId <= 11)
	{
		return 0;
	}

	if (seasonId == 1 && sectionId > 11)
	{
		return 5;
	}

	return 8;
}

string GameInforEditer::getSeasonName(int seasonId)
{
	CCString str;

	if (seasonId > 0 && seasonId < 6)
	{
		str.initWithFormat("seasonName%d", seasonId);
		str = LFStrings::getValue(str.getCString());
	}
	else
	{
		CCAssert(false, "season id  必须是 1-5");
	}

	return str.getCString();
}

// int GameInforEditer::getUserId()
// {
// 	return mUserId;
// }
// void GameInforEditer::setUserId(int id)
// {
// 	if (mUserId == 0)
// 	{
// 		mUserId = id;
// 		//save
// 		saveSessonInforList();
// 	}
// 	else
// 	{
// 		CCLog("Error: %s ---> already have user id, old=%d, cur set id=%d", __FUNCTION__, mUserId, id);
// //		CCAssert(false, "already have user id");
// 	}
// }
string GameInforEditer::getUserName()
{
	return mUserName;
}
void GameInforEditer::setUserName(string str, bool bOverWrite/* = false*/)
{
	if (mUserName.empty() || bOverWrite)
	{
		mUserName = str;
		//save
		saveSessonInforList();
	}
	else
	{
		CCLog("Error: %s ---> already have user name, old=%s, cur set name=%s", __FUNCTION__, mUserName.c_str(), str.c_str());
//		CCAssert(false, "already have user name");
	}
}

//CastInfo GameInforEditer::getOneToBeSaveToServerPayInfoFromFile(int index)
//{
//    char name[128] = {};
//    CastInfo info;
//    
//    sprintf(name, "info_%02d_fee", index);
//    info.fee = CCUserDefault::sharedUserDefault()->getIntegerForKey(name);
//    
//    sprintf(name, "info_%02d_id", index);
//    info.item_id = CCUserDefault::sharedUserDefault()->getIntegerForKey(name);
//    
//    sprintf(name, "info_%02d_time", index);
//    info.time = atol(CCUserDefault::sharedUserDefault()->getStringForKey(name).c_str());
//    
//    return info;
//}
//
//vector<CastInfo> GameInforEditer::getAllToBeSaveToServerPayInfoFromFile()
//{
//    vector<CastInfo> catinfoList;
//    
//    int index = 0;
//    char name[128] = {};
//    while (true)
//    {
//        sprintf(name, "info_%02d_fee", index);
//        if (CCUserDefault::sharedUserDefault()->getIntegerForKey(name, 0) == 0)
//        {
//            break;
//        }
//        index++;
//    }
//    
//    for (int i = 0; i < index; ++i)
//    {
//        catinfoList.push_back(getOneToBeSaveToServerPayInfoFromFile(i));
//    }
//    
//    return catinfoList;
//}
//
//void GameInforEditer::addToBeSaveToServerPayInfoToFile(BuyItem itemInfo)
//{
//    int index = 0;
//    char name[128] = {};
//    while (true)
//    {
//        sprintf(name, "info_%02d_fee", index);
//        if (CCUserDefault::sharedUserDefault()->getIntegerForKey(name, 0) == 0)
//        {
//            break;
//        }
//        index++;
//    }
//    int iCost = 0;
//    sprintf(name, "info_%02d_fee", index);
//    
//    if (itemInfo.item == BuyItem::itemtype_Star_6 || itemInfo.item == BuyItem::itemtype_SuperTools_x8)
//    {
//        iCost = 6;
//    }
//    else if (itemInfo.item == BuyItem::itemtype_Star_12)
//    {
//        iCost = 12;
//    }
//    else if (itemInfo.item == BuyItem::itemtype_Star_18)
//    {
//        iCost = 18;
//    }
//    else if (itemInfo.item == BuyItem::itemtype_Star_25)
//    {
//        iCost = 25;
//    }
//    else if (itemInfo.item == BuyItem::itemtype_Star_30)
//    {
//        iCost = 30;
//    }
//    else
//    {
//        iCost = 1;
//    }
//    CCUserDefault::sharedUserDefault()->setIntegerForKey(name, iCost);
//    sprintf(name, "info_%02d_id", index);
//    CCUserDefault::sharedUserDefault()->setIntegerForKey(name, itemInfo.item);
//    
//    cc_timeval curTime;
//	CCTime::gettimeofdayCocos2d(&curTime, NULL);
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//	time_t timep;
//	time(&timep);
//	curTime.tv_sec = timep;
//#endif
//    
//    sprintf(name, "info_%02d_time", index);
//    CCUserDefault::sharedUserDefault()->setStringForKey(name, CCString::createWithFormat("%ld", curTime.tv_sec)->getCString());
//    
//    CCUserDefault::sharedUserDefault()->flush();
//}

void GameInforEditer::initLockSeason()
{
	m_lLockedSeason.clear();

	Json::Reader reader;
	Json::Value root;
	unsigned long size = 0;
	CCLog("%s Loading LockSeasonInfor->",writeGameSeasonLockInfor.c_str());

	string doc_str = "";
	if (encrypt)
	{
		AES aes;
		bool suc = aes.ReadFileAndDecryptText(
			fileUtiles->getDocFilePath(writeGameSeasonLockInfor).c_str(),
			m_AES_key,doc_str);
		if (!suc)
		{
			CCLog("%s \n=======readInforFile---->Load gameInfor-> %s fail !", __FUNCTION__, writeGameSeasonLockInfor.c_str());
			return ;
		}
	}
	else
	{
		const char* doc = fileUtiles->readDocFile(writeGameSeasonLockInfor,&size);
		//文件读取失败就返回NULL;
		if (size<=0)
		{
			CCLog("%s \n=======readInforFile---->Load gameInfor-> %s fail !", __FUNCTION__, writeGameSeasonLockInfor.c_str());
			return ;
		}
		doc_str = string(doc, size);
		delete[] doc;
	}

	//解析不成功就返回空;
	if (doc_str.size() > 0 && !reader.parse(doc_str, root))
	{
		CCLog("Error : %s --> parse json error!", __FUNCTION__);
	}
	else
	{
		if (!root.isNull() && !root["lock"].isNull()) {
			root = root["lock"];
			if (root.isArray())
			{
				int size = root.size();
				for (int i = 0 ; i < size;i++)
				{
					Json::Value value = root[i];
					int season = value["season"].asInt();
					int section = value["section"].asInt();
					bool locked = value["locked"].asBool();
					
					if (locked)
					{
						m_lLockedSeason.push_back(std::pair<int, int>(season, section));
						//m_lLockedSeason.push_back(section + ( season - 1 ) * GameLayerFactory::getInstance()->getSectionNumBySeasonId(season));
					}
					else
					{
						m_lUnLockSeason.push_back(std::pair<int, int>(season, section));
						//m_lUnLockSeason.push_back(section + ( season - 1 ) * GameLayerFactory::getInstance()->getSectionNumBySeasonId(season));
					}
				}
			}
		}
	}
}
bool GameInforEditer::isChallengeSection(int seasonid, int sectionid)
{
	std::pair<int, int> info;
	for (size_t i = 0; i < m_lLockedSeason.size(); i++)
	{
		info = m_lLockedSeason.at(i);
		if (seasonid == info.first && sectionid == info.second)
		{
			return true;
		}
	}
	return false;
}

bool GameInforEditer::getSecretStageCanUnLocked(int seasonid)
{
	int iTotalScore = getTotalScoreBeforeSeasonId(seasonid);
	if (iTotalScore < GameLayerFactory::getInstance()->getConfigSeasonInfo(seasonid)->iDeblockNum)
	{
		return false;
	}

	return true;
}

bool GameInforEditer::getIsInUnLockedList(int seasonid, int sectionid)
{
	std::pair<int, int> info;
	for (size_t i = 0; i < m_lUnLockSeason.size(); i++)
	{
		info = m_lUnLockSeason.at(i);
		if (seasonid == info.first && sectionid == info.second)
		{
			return true;
		}
	}
	return false;
}

int GameInforEditer::getMyPerfectpassCount()
{
    int iMaxSeasonNum = GameLayerFactory::getInstance()->getValidSeasonNum();   
	//3星通关数
	int perfect_pass = 0;
	for (int i=1; i<= iMaxSeasonNum; ++i)
	{
		SeasonInfor *info = GameInforEditer::getGameInfor()->getSeasonInfor(i);
		if (! info) continue; 
		int sectionnum = GameLayerFactory::getInstance()->getAllSectionNumBySeasonId(i);
		for (int section=1; section<=sectionnum; ++section)
		{
			SectionInfor *secInfo = info->getSectionInfor(section);
			if (secInfo && secInfo->isPlayed && secInfo->iStarNum==3)
			{
				perfect_pass += 1;
			}
		}
	}
	return perfect_pass;
}

int GameInforEditer::getMyEvaluateCount()
{
	return AchievementManager::getInstance()->getAchievementList().size();
}

void GameInforEditer::correctData()
{
	//getSecretStageCanUnLocked
	int lastSeasonId = this->getLastPlayedSeasonId();
	int lastSectionId = this->getLastPlayedSectionId();
	
	int lastSectionIdx = GameLayerFactory::getInstance()->getSectionIndex(lastSeasonId, lastSectionId);
	for (int i=0; i<lastSectionIdx; ++i)
	{
		int curSeasonId, curSectionId;
		pair<int,int> temp = GameLayerFactory::getInstance()->getLevelFormat(i);
		curSeasonId = temp.first;
		curSectionId = temp.second;

		//是挑战关卡，且还不能解锁，就continue
		bool bIsChallengeSection = false;
		if (this->isChallengeSection(curSeasonId, curSectionId)){
			bIsChallengeSection = true;
			if (!this->getSecretStageCanUnLocked(curSeasonId)) continue;
		}

		SectionInfor* pSection = this->getSectionInfor(curSeasonId, curSectionId);
		if (pSection == nullptr)
		{
			SeasonInfor* pSeason = this->getSeasonInfor(curSeasonId);
			if (pSeason == nullptr){
				pSeason = new SeasonInfor();
				pSeason->seasonId = curSeasonId;
				pSeason->score = 0;
				pSeason->avgScore = 0;
				pSeason->minScore = 0;
				pSeason->seasonType = type_Normal;
				GameInforEditer::getGameInfor()->addSeasonInfor(pSeason);
			}
			pSeason->isPlayed = true;
			pSeason->locked = false;
			pSection = new SectionInfor();
			pSection->seasonId = curSeasonId;
			pSection->sectionId = curSectionId;
			pSection->isPlayed = bIsChallengeSection ? false : true;
			pSection->locked = false;
			pSection->iStarNum = bIsChallengeSection ? 0 : 1;
			pSection->iRankingScore = bIsChallengeSection ? 0 : 20;
			pSection->avgScore = 0;
			pSection->minScore = 0;
			pSection->isSuccessed = bIsChallengeSection ? false : true;
			pSection->sectionType = type_Normal;
			pSeason->addSectionInfor(pSection);
			pSeason->score += pSection->iStarNum;
		}
		else if (pSection->locked || !pSection->isPlayed || pSection->iStarNum==0 || pSection->iRankingScore<20)
		{
			SeasonInfor* pSeason = this->getSeasonInfor(curSeasonId);
			pSeason->score -= pSection->iStarNum;
			pSection->locked = false;
			if (! bIsChallengeSection)
			{
				pSection->isPlayed = true;
				pSection->iStarNum = 1;
				pSection->iRankingScore = 20;
				pSection->isSuccessed = true;
			}
			pSeason->score += pSection->iStarNum;
		}
	}

	// 如果lastSectionIdx issuccess 且 next不是最后一个，那么要让下一个解锁
	SectionInfor* pLastSection = this->getSectionInfor(lastSeasonId, lastSectionId);
	int maxSectionIdx = GameLayerFactory::getInstance()->getTotalSectionNum()-1;
	if (pLastSection->isSuccessed && lastSectionIdx < maxSectionIdx)
	{
		bool bIsChallengeSection = true;
		int curSectionIdx = lastSectionIdx+1;
		do 
		{
			int curSeasonId, curSectionId;
			pair<int,int> temp = GameLayerFactory::getInstance()->getLevelFormat(curSectionIdx);
			curSeasonId = temp.first;
			curSectionId = temp.second;
			bIsChallengeSection = this->isChallengeSection(curSeasonId, curSectionId);

			//下一关是挑战关，且不能解锁，那么就找到下一个不是挑战关卡的，解锁它
			if (bIsChallengeSection && !this->getSecretStageCanUnLocked(curSeasonId)){
				continue;
			}

			SectionInfor* pSection = this->getSectionInfor(curSeasonId, curSectionId);
			if (pSection == nullptr)
			{
				SeasonInfor* pSeason = this->getSeasonInfor(curSeasonId);
				if (pSeason == nullptr){
					pSeason = new SeasonInfor();
					pSeason->seasonId = curSeasonId;
					pSeason->isPlayed = true;
					pSeason->locked = false;
					pSeason->score = 0;
					pSeason->avgScore = 0;
					pSeason->minScore = 0;
					pSeason->seasonType = type_Normal;
					GameInforEditer::getGameInfor()->addSeasonInfor(pSeason);
				}
				pSeason->isPlayed = true;
				pSeason->locked = false;
				pSection = new SectionInfor();
				pSection->seasonId = curSeasonId;
				pSection->sectionId = curSectionId;
				pSection->isPlayed = false;
				pSection->locked = false;
				pSeason->addSectionInfor(pSection);
			}
			else if (pSection->locked)
			{
				pSection->isPlayed = false;
				pSection->locked = false;
			}

		} while (++curSectionIdx <= maxSectionIdx && bIsChallengeSection);
	}

	for (list<SeasonInfor*>::iterator it = mSeasonInfors.begin(); it!=mSeasonInfors.end(); ++it)
	{
		(*it)->resetScoreByMySections();
	}
}