#include "AchievementManager.h"
#include "GameScenseDefine.h"
#include "CustomeSprite.h"
#include "GameInforEditer.h"
#include "AchievementNotification.h"
#include "LFUtile.h"
#include "Runtime.h"
#include "LFShareKit.h"
#include "VersionControl.h"
#include "Debug.h"
#include "LevelManager.h"
#include "History.h"

static string default_AchievementManagerFile = "infor/achievement.txt";
static string AchievementManagerFile = "achievement.txt";

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
const std::string configAchievementFile = "achievement_config";
#else
const std::string configAchievementFile = "config/achievement_config";
#endif

#define AchievementManagerRoot  "Achievements"
#define AchievementName "name"
#define AchievementID  "ID" 
#define AchievementFileName  "filename" 
#define AchievementDescribe  "describe" 
#define AchievementSeasonID "SeasonID"
#define AchievementSectionID "SectionID"

using namespace LF;

Achievement::Achievement(string name)
{
	this->iseasonid = 0;
	this->isectionid = 0;
	this->id = AchievementManager::getInstance()->getAchievementIdByName(name);
	Achievement a = AchievementManager::getInstance()->getAchievementDefaultInfo(id);
	this->name = a.name;
	this->filename = a.filename;
	this->describe = a.describe;
}

AchievementManager* AchievementManager::instance;

AchievementManager::AchievementManager()
{
	m_bHaveNew = false;
	mSeasonId = 0;
	mSectionId = 0;
	mRestartClickedNum = 0;
	mAlarmClockBiNum = 0;
	mLastClickedTag = -1;
	mTimeForLastClick = 0;
	mTimeForRedStarWithoutCollection = 0;
	mIsRedStarCollected = true;
	mRedStar = NULL;
	mRedStarBody = NULL;
	mIsRedStarCreatedInScreen = true;
	mIsRedStarFlyOutScreen = false;
	mIsRedStarOnceInScreen = false;
	mBoilerUsedNum = 0;
	mMatorUsedTime = 0;
	mHandlableMotorNum = 0;
	mTotalMotorNum = 0;
	mRestitutionBall24Num = 0;
	mUsedRestitutionBall24Num = 0;
	mWorld = NULL;
	mTimerDesBombNum = 0;
	mUsedTimerDesBombNum = 0;
	mTotalNomalShapeNum = 0;
	mClickedNormalShape = 0;
	mRedStarHorizontalFlyTime = 0;
	m_CurrentMaxRedStarHorizontalFlyTime = 0;
	mLastRedStarAngle = 0;
	mRedStarHighSpeedRotatedAngle = 0;
	mIsRedStarInRope = false;
	mIsRedStarStableAfterHorizontalFly = false;
	mRedStarStableTimeAfterHorizontalFly = 0.0f;
	mRedStarStableTimeAfterFlyOutScreen = 0.0f;
	mRedstarContactTableTime = 0;
	mIsFirstContactTableStatic = true;
	mContactTable = NULL;
    
	ownAchievementsVec.clear();
	m_pFileUtiles = LFFileUtiles::getLFFileUtiles();
}


void AchievementManager::initData()
{
	this->initAllDefaultAchievements();

	// 对于 要编辑的文件 都需要拷贝到doc 下面，以后的操作都用Doc下面的文件
	/*
	 * 玩家使用社交平台登录过，那么就会把默认数据绑定到第一次登录的账号info_facebook_123.txt，此后将不再使用默认的文件info.txt
	 * 之后用其他账号登录，又会拷贝一份default，保存在一个新的文件info_facebook_456.txt
	*/
	{
		string platform, id;
		bool bHasPlatformID = History::getHistory()->getLastUserPlatformAndID(platform, id);
		if (bHasPlatformID)
		{
			string temp_filePath = "achievement";
			AchievementManagerFile = temp_filePath + "_" + platform + "_" + id + ".txt";

			//第一次登录，那么绑定到之前的数据
			string oldNoneUserAchieveFile = CCFileUtils::sharedFileUtils()->getWritablePath()+default_AchievementManagerFile.substr(default_AchievementManagerFile.find_last_of("/")+1);
			if (CCFileUtils::sharedFileUtils()->isFileExist(oldNoneUserAchieveFile))
			{
				if (0 == rename(oldNoneUserAchieveFile.c_str(), (CCFileUtils::sharedFileUtils()->getWritablePath()+AchievementManagerFile).c_str())){
					CCLog("%s --->SUCCESS rename file [%s] to [%s]", __FUNCTION__, oldNoneUserAchieveFile.c_str(), AchievementManagerFile.c_str());
				}else {
					CCLog("%s --->FAILED rename file [%s] to [%s]", __FUNCTION__, oldNoneUserAchieveFile.c_str(), AchievementManagerFile.c_str());
					CCAssert(false, "");
				}
			}
		}
	}
	m_pFileUtiles->copyFileFromResToDocIfNotExist(default_AchievementManagerFile, NULL, AchievementManagerFile);

	if (!parseAchievementFile())
	{
		//CCAssert(false,"解析成就数据出错");
		CCMessageBox(CCString::createWithFormat("parse %s error",AchievementManagerFile.c_str())->getCString(), "error");
	}

}

void AchievementManager::initAllDefaultAchievements()
{
    //所有的成就，提交时按照这个顺序来判断是否完成这个
    m_allDefalutAchievementVec.clear();

	string fullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename((configAchievementFile+"_"+ResManager::getManager()->getLanguagePathString()).c_str());
	fullpath = 	fullpath.substr(0, fullpath.find_last_of("/")) + ".txt";
	unsigned long size;
	const char* achievementDoc = m_pFileUtiles->readResourceFile(fullpath.c_str(), &size);
	if (achievementDoc == NULL)
	{
		CCMessageBox("AchievementManager::parseAchievementFile read file Error !", "tishi");
		return;
	}
	if (size <= 0)
	{
		CC_SAFE_DELETE(achievementDoc);
		return;
	}
	string docStr((const char*)achievementDoc, size);
	CC_SAFE_DELETE(achievementDoc);

	Json::Reader reader;
	Json::Value root;
	//解析不成功就返回空;
	if (!reader.parse(docStr, root))
	{
		CCLog("AchievementManager::parseAchievementFile %s parse Error!", docStr.c_str());
		return;
	}
	if (!root[AchievementManagerRoot].isNull())
	{
		Achievement a;
		Json::Value arrayValue = root[AchievementManagerRoot];
		for (size_t i = 0; i < arrayValue.size(); ++i)
		{
			a.id = arrayValue[i][AchievementID].asInt();
			a.name = arrayValue[i][AchievementName].asString();
			a.filename = arrayValue[i][AchievementFileName].asString();
			a.describe = arrayValue[i][AchievementDescribe].asString();
			m_allDefalutAchievementVec.push_back(a);
		}
	}
}

int AchievementManager::getAchievementIdByName(string name)
{
	std::vector<string> allDefalutAchievementNamesVec;

	allDefalutAchievementNamesVec.push_back("Suspended_In_The_Air");
	allDefalutAchievementNamesVec.push_back("HighSpeed_Rotating");
	allDefalutAchievementNamesVec.push_back("Where_Am_I");
	allDefalutAchievementNamesVec.push_back("Perfect_Arc");
	allDefalutAchievementNamesVec.push_back("Standing_On_A_Leg");
	allDefalutAchievementNamesVec.push_back("How_Can_Be");
	allDefalutAchievementNamesVec.push_back("Waiting_For_Chance");
	allDefalutAchievementNamesVec.push_back("Just_Waiting");
	allDefalutAchievementNamesVec.push_back("Age_Of_Steam");
	allDefalutAchievementNamesVec.push_back("Precise_Anticipation");//Precise_Control
    
	allDefalutAchievementNamesVec.push_back("Hate_Winter");
	allDefalutAchievementNamesVec.push_back("Only_Love");
	allDefalutAchievementNamesVec.push_back("Love_Pinball");
	allDefalutAchievementNamesVec.push_back("Go_For_Perfect");
	allDefalutAchievementNamesVec.push_back("Air_Hit");
//	allDefalutAchievementNamesVec.push_back("Impatient");  //去掉，现在没有时钟了
	allDefalutAchievementNamesVec.push_back("Regression_Planet");
	allDefalutAchievementNamesVec.push_back("Stand_Steady");
	allDefalutAchievementNamesVec.push_back("Big_Clear");
	allDefalutAchievementNamesVec.push_back("Stars_Full");
    
	allDefalutAchievementNamesVec.push_back("Stars_Many");
	allDefalutAchievementNamesVec.push_back("Stars_Little");
	allDefalutAchievementNamesVec.push_back("Wow_Bomb");
	allDefalutAchievementNamesVec.push_back("Motor_Experts");
	allDefalutAchievementNamesVec.push_back("Industrial_Age");
	allDefalutAchievementNamesVec.push_back("Most_Cold");
	allDefalutAchievementNamesVec.push_back("Conquered_The_Outer_Space");
	allDefalutAchievementNamesVec.push_back("So_Dangerous");
	allDefalutAchievementNamesVec.push_back("Persistence");

	for (size_t i = 0; i < allDefalutAchievementNamesVec.size(); i++)
	{
		if (name == allDefalutAchievementNamesVec.at(i))
		{
			return i+1;
		}
	}
	return 0;
}

// Achievement AchievementManager::getAchievementDefaultInfo(string name)
// {
// 	Achievement a;
// 	for (size_t i = 0; i < m_allDefalutAchievementVec.size(); i++)
// 	{
// 		a = m_allDefalutAchievementVec.at(i);
// 		if (name == a.name)
// 		{
// 			return a;
// 		}
// 	}
// 	return a;
// }

Achievement AchievementManager::getAchievementDefaultInfo(int iAchievementid)
{
	Achievement a;
	for (size_t i = 0; i < m_allDefalutAchievementVec.size(); i++)
	{
		a = m_allDefalutAchievementVec.at(i);
		if (iAchievementid == a.id)
		{
			return a;
		}
	}
	return a;
}

void AchievementManager::clearData(int currentSeationId,int currentSectionId)
{
	int tempRestartClickedNum = mRestartClickedNum;	// 先保留

	mRestartClickedNum = 0;
	mAlarmClockBiNum = 0;
	mTimeForLastClick = 0;
	mLastClickedTag = -1;
	mTimeForRedStarWithoutCollection = 0;
	mIsRedStarCollected = true;
	mRedStar = NULL;
	mRedStarBody = NULL;
	mIsRedStarCreatedInScreen = true;
	mIsRedStarFlyOutScreen = false;
	mIsRedStarOnceInScreen = false;
	mBoilerUsedNum = 0;
	mMatorUsedTime = 0;
	mHandlableMotorNum = 0;
	mTotalMotorNum = 0;
	mRestitutionBall24Num = 0;
	mUsedRestitutionBall24Num = 0;
	mWorld = NULL;
	mTimerDesBombNum = 0;
	mUsedTimerDesBombNum = 0;
	mTotalNomalShapeNum = 0;
	mClickedNormalShape = 0;
	mRedStarHorizontalFlyTime = 0;
	m_CurrentMaxRedStarHorizontalFlyTime = 0;
	mLastRedStarAngle = 0;
	mRedStarHighSpeedRotatedAngle = 0;
	mIsRedStarInRope = false;
	mIsRedStarStableAfterHorizontalFly = false;
	mRedStarStableTimeAfterHorizontalFly = 0.0f;
	mRedStarStableTimeAfterFlyOutScreen = 0.0f;
	mRedstarContactTableTime = 0;
	mIsFirstContactTableStatic = true;

	
	if (currentSeationId == mSeasonId && currentSectionId == mSectionId)
	{// 上次进入游戏的关卡 与这次进入的关卡 一样 那么就不清除restart的点击次数
		// 疑问：如果点击一次restart 之后，就又回到外面，等会又进入这个关卡，怎么处理，目前都是保留
		mRestartClickedNum = tempRestartClickedNum;
	}
	mSeasonId = currentSeationId;
	mSectionId = currentSectionId;
}
AchievementManager* AchievementManager::getInstance()
{
	if (instance == NULL)
		instance = new AchievementManager();
	return instance;
}
void AchievementManager::destroy()
{
	if (instance != NULL){
		delete instance;
		instance = NULL;
	}
}

void AchievementManager::reloadAchievementDefaultInfo()
{
	initAllDefaultAchievements();

	for (size_t i = 0; i < ownAchievementsVec.size(); i++)
	{
		Achievement& temp = ownAchievementsVec.at(i);
		Achievement a = getAchievementDefaultInfo(temp.id);
		temp.name = a.name;
		temp.describe = a.describe;
		temp.filename = a.filename;
	}
}

bool AchievementManager::isAnySpriteInArea(CCNode* exceptedNode,CCPoint pos,float r)
{
	b2Vec2 b2Pos = b2Vec2(pos.x / PTM_RATIO,pos.y / PTM_RATIO);
	float32 b2Len= r /PTM_RATIO;

	minDistanceFromPolygon(NULL,0,b2Vec2(0,0));

	//
	for (b2Body* b = mWorld->GetBodyList(); b; b = b->GetNext())
	{
		CCNode* node = (CCNode*) b->GetUserData();
		if (node != NULL && node!=exceptedNode) 
		{
			b2Fixture* fixture = b->GetFixtureList();
			for (; fixture!=NULL; fixture=fixture->GetNext())
			{
				b2Shape* shape = (b2Shape*)fixture->GetShape();

				// 是 台面 单独处理
				if (node->getTag() == Tag_Box_Table || 
					node->getTag() == Tag_Box_Table_Ineffective)
				{
					if( minDistanceFromShape(shape,b2Pos,b) <= b2Len )
					{
						return true;
					}
				}
				else
				{	// 一般的用规矩的物体
					b2Transform transform;
					transform.Set(b->GetPosition(), b->GetAngle());
					b2AABB aabb;
					shape->ComputeAABB(&aabb,transform,0);
					if ( b2Distance(b2Pos,aabb.GetCenter()) <= b2Len )
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}
void AchievementManager::test()
{
// 	addAchievement(Achievement("Suspended_In_The_Air"));
// 	addAchievement(Achievement("HighSpeed_Rotating"));
// 	addAchievement(Achievement("Where_Am_I"));
// 	addAchievement(Achievement("Perfect_Arc"));
// 	addAchievement(Achievement("Standing_On_A_Leg"));
// 	addAchievement(Achievement("How_Can_Be"));
// 	addAchievement(Achievement("Waiting_For_Chance"));
// 	addAchievement(Achievement("Just_Waiting"));
// 	addAchievement(Achievement("Age_Of_Steam"));
// 	addAchievement(Achievement("Precise_Anticipation"));//Precise_Control
// 
// 	addAchievement(Achievement("Hate_Winter"));
// 	addAchievement(Achievement("Only_Love"));
// 	addAchievement(Achievement("Love_Pinball"));
// 	addAchievement(Achievement("Go_For_Perfect"));
// 	addAchievement(Achievement("Air_Hit"));
// 	addAchievement(Achievement("Impatient"));
// 	addAchievement(Achievement("Regression_Planet"));
// 	addAchievement(Achievement("Stand_Steady"));
// 	addAchievement(Achievement("Big_Clear"));
// 	addAchievement(Achievement("Stars_Full"));
// 
// 	addAchievement(Achievement("Stars_Many"));
// 	addAchievement(Achievement("Stars_Little"));
// 	addAchievement(Achievement("Wow_Bomb"));
// 	addAchievement(Achievement("Motor_Experts"));
// 	addAchievement(Achievement("Industrial_Age"));
// 	addAchievement(Achievement("Most_Cold"));
// 	addAchievement(Achievement("Conquered_The_Outer_Space"));
// 	addAchievement(Achievement("So_Dangerous"));
// 	addAchievement(Achievement("Persistence"));



}

// 注意 如果是没有内容，那么也要最好的写上{}来预防错误
bool AchievementManager::parseAchievementFile()
{
	unsigned long size;
	const char* achievementDoc = m_pFileUtiles->readDocFile(AchievementManagerFile,&size);
	if (achievementDoc == NULL)
	{
		CCLog("AchievementManager::parseAchievementFile read file Error !");
		return false;
	}
	if (size <= 0)
	{
		return true;
	}
	string docStr(achievementDoc, size);

	Json::Reader reader;
	Json::Value root;
	//解析不成功就返回空;
	if (!reader.parse(docStr, root))
	{
		CCLog("AchievementManager::parseAchievementFile %s parse Error!", docStr.c_str());
		CC_SAFE_DELETE(achievementDoc);
		return false;
	}
	
	if (!root[AchievementManagerRoot].isNull())
	{
		int num = root[AchievementManagerRoot].size();
		for (int i=0;i<num;i++)
		{
			Achievement a;
			
            a.iseasonid = root[AchievementManagerRoot][i][AchievementSeasonID].asInt();
            a.isectionid = root[AchievementManagerRoot][i][AchievementSectionID].asInt();
			a.id = root[AchievementManagerRoot][i][AchievementID].asInt();

			Achievement temp = getAchievementDefaultInfo(a.id);
			a.filename = temp.filename;
			a.describe = temp.describe;
			a.name = temp.name;
			ownAchievementsVec.insert(ownAchievementsVec.end(),a);
		}
	}
	CC_SAFE_DELETE(achievementDoc);
	return true;
}

string AchievementManager::genAchievementString()
{
	Json::Value root;
	for(size_t i=0;i<ownAchievementsVec.size();i++)
	{
		Json::Value _oneAchievement;
        _oneAchievement[AchievementSeasonID] = Json::Value(ownAchievementsVec[i].iseasonid);
        _oneAchievement[AchievementSectionID] = Json::Value(ownAchievementsVec[i].isectionid);
		_oneAchievement[AchievementName] = Json::Value(ownAchievementsVec[i].name);
		_oneAchievement[AchievementID] = Json::Value(ownAchievementsVec[i].id);
		root[AchievementManagerRoot].append(_oneAchievement);
	}
	
	//得到配置文本
	Json::StyledWriter styled_writer;
	return styled_writer.write(root);
}
void AchievementManager::save()
{
	string seasonInforString = genAchievementString();
	bool sec = false;
	sec = m_pFileUtiles->writeToDocFile(AchievementManagerFile,seasonInforString);

#ifdef DEBUG_ENABLE_ACHIEVE_LOG
	if (sec)
	{
		CCLog("save Achievemen Success !");
	}else
	{
		CCLog("save Achievemen Error !");
	}
#endif
	
}

size_t AchievementManager::getSaveToServerData()
{
    size_t b = 0;
    for (size_t i = 0; i < m_allDefalutAchievementVec.size() && i < 32; ++i)
    {
        if (alreadyHaveAchievement(m_allDefalutAchievementVec.at(i)))
        {
            b |= (1 << i);
        }
    }
    
    return b;
}

void AchievementManager::saveServerDataToFile(size_t value)
{
    
    for (int i = m_allDefalutAchievementVec.size()-1; i >= 0; i--)
    {
        if (value >> i & 0x01)
        {
            addAchievement(m_allDefalutAchievementVec.at(i));
        }
    }
}

//
bool AchievementManager::alreadyHaveAchievement(Achievement a)
{
	for (int i=ownAchievementsVec.size() - 1 ;i >= 0 ;i--)
	{
		if (ownAchievementsVec[i] == a)
		{
			return true;
		}
	}
	return false;
}
bool AchievementManager::addAchievement(Achievement a)
{
#ifdef DEBUG_ENABLE_ACHIEVE_LOG
	CCLog("AchievementManager try to add  Achievement [%s] ",a.name.c_str());
#endif


	// free版本只能完成平稳着陆；追求完美；不爱过冬；好险；这4个
	//addAchievement(Achievement("Go_For_Perfect"));
	//addAchievement(Achievement("Stand_Steady"));
	//addAchievement(Achievement("So_Dangerous"));
	//addAchievement(Achievement("Hate_Winter"));
#if (Version_Lite == 1)
	if (a.id != getAchievementIdByName("Go_For_Perfect") &&
		a.id != getAchievementIdByName("Stand_Steady") &&
		a.id != getAchievementIdByName("So_Dangerous") &&
		a.id != getAchievementIdByName("Hate_Winter"))
	{
		return true;
	}
#endif


	// 测试
	//AchievementNotification::currentLayer()->showAchievement(a);

	// 这里应该每次都上报，不能根据当前是否存放有记录而区别是否要上报
	// 因为用户可能在没有登录时得了成就，那么以后登录了，就不会再的成就了，所以要每次都上报
    LFShareKit::getKit()->reportAchievementToGameCenter(a.name, 100);
    
	bool alreadyHave = alreadyHaveAchievement(a);
	if (alreadyHave)
		return true;
	Achievement temp = getAchievementDefaultInfo(a.id);
	a.filename = temp.filename;
	a.describe = temp.describe;
	a.name = temp.name;
    a.iseasonid = LevelManager::getManager()->getLastSeasonId();
    a.isectionid = LevelManager::getManager()->getLastSectionId();
	ownAchievementsVec.insert(ownAchievementsVec.end(),a);
	save();
	setHaveNew(true);
#ifdef DEBUG_ENABLE_ACHIEVE_LOG
	CCLog("AchievementManager addAchievement successful : [ name = %s]",a.name.c_str());
#endif

	if (AchievementNotification::currentLayer())
	{
        if (a.iseasonid >= 1 && a.isectionid >= 1)
        {
            AchievementNotification::currentLayer()->showAchievement(a);
        }
	}

	return true;
}

//
bool AchievementManager::isInScreen(CCNode* node)
{
	if (node == NULL)
		return false;

	if (node->getPositionX() > 0 &&
		node->getPositionX() < 1024 &&
		node->getPositionY() > 0 &&
		node->getPositionY() < 768)
	{
		return true;
	}

	return false;
}
//
void AchievementManager::setRedStarInAir()
{
	mIsRedStarCollected = false;
	mTimeForRedStarWithoutCollection = 0;
	mRedStarHorizontalFlyTime = 0;
}
//
void AchievementManager::setRedStarCollectioned()
{
	mIsRedStarCollected = true;
	mTimeForRedStarWithoutCollection = 0;
	mRedStarHorizontalFlyTime = 0;
}
//
void AchievementManager::tick(float t)
{
	mTimeForLastClick +=t;

	if (!mIsRedStarCollected && mRedStarBody)
	{
		mTimeForRedStarWithoutCollection +=t;
		//星星3秒不碰到任何东西
		if (mTimeForRedStarWithoutCollection >= 6.0f)
		{
			addAchievement(Achievement("Suspended_In_The_Air"));
			// 防止不断的加入
			mTimeForRedStarWithoutCollection = 0;
		}

		//CCLog("m_CurrentMaxRedStarHorizontalFlyTime = %f",m_CurrentMaxRedStarHorizontalFlyTime);

		// 水平 无障碍的飞行
		if (abs(mRedStarBody->GetLinearVelocity().x) > 8.0f && abs(mRedStarBody->GetLinearVelocity().x) > abs(mRedStarBody->GetLinearVelocity().y))
		{
			mRedStarHorizontalFlyTime += t;

			if (m_CurrentMaxRedStarHorizontalFlyTime < mRedStarHorizontalFlyTime)
			{
				m_CurrentMaxRedStarHorizontalFlyTime = mRedStarHorizontalFlyTime;
			}
		}else
		{
			mRedStarHorizontalFlyTime = 0;
		}
	}

	if (mRedStar)
	{
		float rotateSpeed = abs(mLastRedStarAngle - mRedStar->getRotation());
		// 转动速度超过10.0f
		if ( rotateSpeed >= 7.0f )
		{
			mRedStarHighSpeedRotatedAngle += rotateSpeed;
		}else
		{
			mRedStarHighSpeedRotatedAngle = 0;
		}
		//CCLog("rotateSpeed = %f , angle = %f",rotateSpeed,mRedStarHighSpeedRotatedAngle);

		//星星在空中旋转3600度
		if (mRedStarHighSpeedRotatedAngle >= 2500)
		{
			addAchievement(Achievement("HighSpeed_Rotating"));
			// 防止不断的加入
			mRedStarHighSpeedRotatedAngle = 0;
		}
		// 如果红星在往外飞，那么就代表飞出了屏幕，而不是着落入屏幕
		// 绳子不链接的情况才算
		if ( !mIsRedStarInRope &&  mRedStar->getPositionY() >= 780 && mRedStarBody->GetLinearVelocity().y > 0)
		{
			mIsRedStarFlyOutScreen = true;
		}

		// 记录红星是否曾经在屏幕里面过
		if (!mIsRedStarOnceInScreen)
		{
			if (isInScreen(mRedStar))
			{
				mIsRedStarOnceInScreen = true;
			}
		}

		// 如果红星飞到外面，还在往上飞，那么就是代表飞到外面过
		// 那么就等待他回来稳定
		if(mIsRedStarFlyOutScreen)
		{
			//CCLog("mRedStarBody->GetLinearVelocity().x = %f",abs(mRedStarBody->GetLinearVelocity().x));
			//CCLog("mRedStarBody->GetLinearVelocity().y = %f",abs(mRedStarBody->GetLinearVelocity().y));
			if (isRedStarStabled())
			{
				mRedStarStableTimeAfterFlyOutScreen += t;

				// 停止了有0.3秒，就认为稳定了
				if (mRedStarStableTimeAfterFlyOutScreen >= 0.3f)
				{
					addAchievement(Achievement("Where_Am_I"));
					// 防止不断的加入
					mIsRedStarFlyOutScreen = false;
				}
			}else
			{
				mRedStarStableTimeAfterFlyOutScreen = 0;
			}
		}
		// 如果平飞了有一秒了，那就等待静止的情况
		if (m_CurrentMaxRedStarHorizontalFlyTime >= 1.8f )
		{
			//CCLog("mRedStarBody->GetLinearVelocity().x = %f",abs(mRedStarBody->GetLinearVelocity().x));
			//CCLog("mRedStarBody->GetLinearVelocity().y = %f",abs(mRedStarBody->GetLinearVelocity().y));
			if (isRedStarStabled())
			{
				mRedStarStableTimeAfterHorizontalFly += t;

				// 停止了有0.3秒，就认为稳定了
				if (mRedStarStableTimeAfterHorizontalFly >= 0.3f)
				{
					addAchievement(Achievement("Perfect_Arc"));

					// 防止不断的加入
					m_CurrentMaxRedStarHorizontalFlyTime = 0;
				}
			}else
			{
				mRedStarStableTimeAfterHorizontalFly = 0;
			}
		}

		// 更新角度
		mLastRedStarAngle = mRedStar->getRotation();
	}
}
bool AchievementManager::isRedStarStabled()
{
	if (mRedStar == NULL || mRedStarBody == NULL)
	{
		return false;
	}

	if ( abs(mRedStarBody->GetLinearVelocity().x) <= 0.1f && abs(mRedStarBody->GetLinearVelocity().y) <= 1.0f )
	{
		return true;
	}

	return false;
}

//

//=================== 游戏中回调 =====================//

void AchievementManager::setRedStar(CCNode* redStar,b2Body* body,bool isThrow)
{
	mRedStar = redStar;
	mRedStarBody = body;
	mIsRedStarCreatedInScreen = isInScreen(redStar);
	
	mLastRedStarAngle = redStar!=NULL ? redStar->getRotation() : 0;

	if (isThrow && redStar)
	{// 是抛出的，就触发一下
		setRedStarInAir();
	}
}
//
void AchievementManager::restartItemClicked()
{
	mRestartClickedNum ++;

	if (mRestartClickedNum >= 10)
	{
		AchievementNotification::currentLayer()->stopMonitoring();
		Achievement a;
		a.name = "Persistence";	// 坚持不懈
		addAchievement(a);
		// 防止不断加入
		mRestartClickedNum = 1;
	}
}
//
bool AchievementManager::isDangerousEffetive(CCNode* redStar,CCNode* table)
{
	if (!mWorld && !redStar)
		return false;

	float32 b2Len = 4.0f;
	b2Vec2 b2RedStarPos = b2Vec2(redStar->getPositionX()/PTM_RATIO,redStar->getPositionY()/PTM_RATIO);

	for (b2Body* b = mWorld->GetBodyList(); b; b = b->GetNext())
	{   
		CCNode* node = (CCNode*)b->GetUserData();
		if (node == NULL || node == redStar || node == table)
			continue;
		
		b2Fixture* fixture = b->GetFixtureList();
		for (; fixture!=NULL; fixture=fixture->GetNext())
		{
			b2Shape* shape = (b2Shape*)fixture->GetShape();

			float tempDis = minDistanceFromShape(shape,b2RedStarPos,b);
#ifdef DEBUG_ENABLE_ACHIEVE_LOG
			CCLog("tempDis = %f , b2Len = %f",tempDis,b2Len);
#endif
			if( tempDis <= b2Len )
			{
				return false;
			}
		}
	}

	return true;

}

//
void AchievementManager::sucessed(CCSprite* redStar,CCSprite* table,b2Body* tableBody)
{
	int angle = abs(redStar->getRotation() - table->getRotation());
	int shift = 5;	// 偏移是5度

	// 当两个夹角为 （360/5）度的倍数时，就是刚好平直
	angle %= 72;	// 归一化

	// 有夹角，就是不是平放的，也就是一个角着地
	// 但是得需要红星在桌面上面
	if( abs(angle) > shift && abs(72 - angle) > shift  && redStar->getPositionY() > table->getPositionY() + 30 )
	{
		addAchievement(Achievement("Standing_On_A_Leg"));
	}

	float dis_x = abs(redStar->getPositionX() - table->getPositionX());
	b2PolygonShape* shape = (b2PolygonShape* )tableBody->GetFixtureList()->GetShape();
	float tableWidth = abs(shape->GetVertex(0).x - shape->GetVertex(1).x) * PTM_RATIO;
	// 刚好放在上面的情况
	float criticalValue = tableWidth/2 - redStar->getContentSize().width/2 + 6.0f;
	// 红星在台面外面成功
	if(dis_x >= criticalValue + 28)
	{
		addAchievement(Achievement("How_Can_Be"));
	}

	// 刚好在边上
	if (dis_x <= (criticalValue + 8) && dis_x >= criticalValue - 3 && isDangerousEffetive(redStar,table))
	{
		addAchievement(Achievement("So_Dangerous"));
	}

	// 在，仅有不可控马达的关卡，点击炸弹通关
	if ( (mLastClickedTag == Tag_Box_Bomb || mLastClickedTag == Tag_Box_Bomb_Hard) &&
		 (mHandlableMotorNum <=0 && mTotalMotorNum >= 1) )
	{
		addAchievement(Achievement("Waiting_For_Chance"));
	}
	
	// 3秒以上无操作
	if (mTimeForLastClick >= (6.0f + RedstarStayOnTable_TimeToSuc) )
	{
		addAchievement(Achievement("Just_Waiting"));
	}
	// 使用锅炉的次数大于10次
	if (mBoilerUsedNum >= 10)
	{
		addAchievement(Achievement("Age_Of_Steam"));
	}

	// 对于有可控马达的关卡，不点击马达，通关
	if (mHandlableMotorNum >= 1 && mMatorUsedTime <= 1)
	{
		addAchievement(Achievement("Precise_Anticipation"));//Precise_Control
	}

	// 在有减速炸弹的关卡中，不使用它，而过关了
	if (mTimerDesBombNum > 0 && mUsedTimerDesBombNum <=0)
	{
		addAchievement(Achievement("Hate_Winter"));
	}
	// 把游戏里面的所有可点击的木块，全部点掉。而且至少点击了5个
	if (mClickedNormalShape >= mTotalNomalShapeNum && mClickedNormalShape >= 5)
	{
		addAchievement(Achievement("Only_Love"));
	}

	// 还剩余4个弹球没有用
	if (mRestitutionBall24Num - mUsedRestitutionBall24Num >= 4)
	{
		addAchievement(Achievement("Love_Pinball"));
	}
	
	GameScore score = Runtime::getRuntime()->getGameScore();
	// 再重复玩得到3颗星，成绩比以前更好，那么急
	if (score.isABetterScore && score.getStarNum() == 3 && mRestartClickedNum >= 1)
	{
		addAchievement(Achievement("Go_For_Perfect"));
	}

}
//
void AchievementManager::effctiveClick(CCNode* node,b2Body* body)
{
	mTimeForLastClick = 0;
	if (node)
	{
		mLastClickedTag = node->getTag();
		
		if(mLastClickedTag == Tag_Box_Boiler)
			mBoilerUsedNum ++;
		if(mLastClickedTag == Tag_Anchor_Mator)
			mMatorUsedTime ++;
		if(mLastClickedTag == Tag_Timer_Dec_Bomb)
			mUsedTimerDesBombNum ++;
		if(mLastClickedTag == Tag_Box_can_Destroied)
			mClickedNormalShape ++;

		// 是否使用弹球
		{
			CustomeSprite* sprite = (CustomeSprite*)node;
			if (node->getTag() == Tag_Box_can_Destroied &&
				(sprite->getFileName().find("marbles.png") != string::npos ) )
			{
				mUsedRestitutionBall24Num ++;
			}
		}

		// 悬空击中，只处理24的物体
		{
			CustomeSprite* sprite = (CustomeSprite*)node;
			float minV = 2;
			float32 bodyV = 0;
			if (body)
			{
				bodyV = body->GetLinearVelocity().Length();

#ifdef DEBUG_ENABLE_ACHIEVE_LOG
				CCLog("bodyV = %f",bodyV);
#endif
			}
			if (node->getTag() == Tag_Box_can_Destroied &&
					(sprite->getFileName().find("marbles.png") != string::npos || 
					sprite->getFileName().find("circle24.png") != string::npos || 
					sprite->getFileName().find("box24.png") != string::npos) 
				)
			{
				if (!isAnySpriteInArea(node,node->getPosition(),72) && bodyV > minV)
				{
					addAchievement(Achievement("Air_Hit"));
				}
			}

			if (
				sprite->getFileName().find("circle_bomb24.png") != string::npos || 
				sprite->getFileName().find("wormhole_bomb_24.png") != string::npos || 
				sprite->getFileName().find("bomb24_no.png") != string::npos || //
				(sprite->getFileName().find("timerdec.png") != string::npos && sprite->getScale() == 0.5f) )
			{
				if (!isAnySpriteInArea(node,node->getPosition(),72) && body && bodyV > minV)
				{
					addAchievement(Achievement("Air_Hit"));
				}
			}
		}


	}else
	{
		mLastClickedTag = -1;
	}
}
//
void AchievementManager::alarmClockBi()
{
	mAlarmClockBiNum ++;
	if (mAlarmClockBiNum >= 5)
	{
		addAchievement(Achievement("Impatient"));
	}
}
//
void AchievementManager::redStarFlyOutOffScreenAndFailed()
{
	AchievementNotification::currentLayer()->stopMonitoring();

	addAchievement(Achievement("Regression_Planet"));
}
void AchievementManager::redStarContactTable(CCNode*table)
{
	mRedstarContactTableTime = 0;
	mIsFirstContactTableStatic = true;
	mContactTable = table;
}
void AchievementManager::redStarOnTable(b2Vec2 shift,float dt)
{
	if (abs(shift.x) <= 0.01 && abs(shift.y) <= 0.01) 
	{
		mRedstarContactTableTime += dt;

		if (mIsFirstContactTableStatic && mRedstarContactTableTime >= 0.3f && isDangerousEffetive(mRedStar,mContactTable))
		{
			// 排除1-7
			if ( mSeasonId != 1 && mSectionId != 7)
			{
				addAchievement(Achievement("Stand_Steady"));
			}
			// 防止不断的加入
			mIsFirstContactTableStatic = false;
		}
	}else
	{
		mRedstarContactTableTime = 0;
		mIsFirstContactTableStatic = false;
	}
}
//void AchievementManager::redStarOnTableMove()
//{
//	mRedstarContactTableTime = 0.0f;
//}
//void AchievementManager::redStarOnTableStatic(float dt)
//{
//	mRedstarContactTableTime += dt;
//
//	if (mRedstarContactTableTime >= RedstarStayOnTable_TimeToSuc/3.0f)
//	{
//		addAchievement(Achievement("Stand_Steady"));
//
//		mRedstarContactTableTime = 0;
//	}
//}

void AchievementManager::beginContact(CCNode* node1,CCNode* node2)
{
	if (node1->getTag() == Tag_Box_RedStar || node2->getTag() == Tag_Box_RedStar)
	{
		setRedStarCollectioned();

		//((CCSprite*)mRedStar)->setColor(ccGREEN);
	}
}
void AchievementManager::endContact(CCNode* node1,CCNode* node2)
{
	if (node1->getTag() == Tag_Box_RedStar || node2->getTag() == Tag_Box_RedStar)
	{
		// 没有悬挂在绳子上，才算滞空
		if (!mIsRedStarInRope)
		{
			setRedStarInAir();
		}

		//((CCSprite*)mRedStar)->setColor(ccWHITE);
	}
}
void AchievementManager::PostSolve(CCNode* node1,CCNode* node2)
{
	if (node1->getTag() == Tag_Box_RedStar || node2->getTag() == Tag_Box_RedStar)
	{
		setRedStarCollectioned();

		//((CCSprite*)mRedStar)->setColor(ccBLACK);
	}
}
void AchievementManager::createdARope(CCNode* hungedShape)
{
	if (hungedShape->getTag() == Tag_Box_RedStar)
	{
		mIsRedStarInRope = true;
	}
}
void AchievementManager::ropeCuted(CCNode* hungedShape)
{
	if (hungedShape->getTag() == Tag_Box_RedStar)
	{
		mIsRedStarInRope = false;
		setRedStarInAir();
	}
}
void AchievementManager::createdOneRestitutionBall24()
{
	mRestitutionBall24Num++;
}	

void AchievementManager::createdOneMortoShape(bool handable)
{
	if (handable)
	{
		mHandlableMotorNum ++;
	}
	mTotalMotorNum ++;
}
void AchievementManager::setWorld(b2World* world)
{
	mWorld = world;
}

void AchievementManager::createdATimeDecBomb()
{
	mTimerDesBombNum ++;
}
void AchievementManager::blackHoleAbsorption(const vector<CCNode*> absorptions)
{
	// 黑洞一次性吸收5个以上
	if (absorptions.size() >= 5)
	{
		addAchievement(Achievement("Big_Clear"));
	}
}

void AchievementManager::createdOneNormalShape()
{
	mTotalNomalShapeNum ++;
}

void AchievementManager::congratulationed()
{
	int score = 0;
	list<SeasonInfor*> seasonList = GameInforEditer::getGameInfor()->getSessonInforList();
	list<SeasonInfor*>::iterator it;
	for (it = seasonList.begin();it!=seasonList.end();it++)
	{
		score += (*it)->score;
	}

	//收集300颗星星
	if (score >= 300)
	{
		addAchievement(Achievement("Stars_Full"));
		return;
	}
	//收集240颗星星
	if (score >= 240)
	{
		addAchievement(Achievement("Stars_Many"));
		return;
	}
	//收集180颗星星
	if (score >= 180)
	{
		addAchievement(Achievement("Stars_Little"));
		return;
	}
}

void AchievementManager::seasonFinished(int seasonId)
{
	switch(seasonId)
	{
	case 1:
		addAchievement(Achievement("Wow_Bomb"));
		break;
	case 2:
		addAchievement(Achievement("Motor_Experts"));
		break;
	case 3:
		addAchievement(Achievement("Industrial_Age"));
		break;
	case 4:
		addAchievement(Achievement("Most_Cold"));
		break;
	case 5:		// 是游戏完毕之后回调的
		addAchievement(Achievement("Conquered_The_Outer_Space"));
		break;
	}
}
