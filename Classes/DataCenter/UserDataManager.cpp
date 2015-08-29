#include "UserDataManager.h"
#include "GameInforEditer.h"
#include "AchievementManager.h"
#include "JuqingDaoJuManager.h"
#include "History.h"
#include "GameLayerFactory.h"

static UserDataManager* _userDataManager = NULL;

UserDataManager* UserDataManager::getInstance()
{
	if (!_userDataManager)
		_userDataManager = new UserDataManager();

	return _userDataManager;
}
void UserDataManager::destroy()
{
	if (_userDataManager){
		delete _userDataManager;
		_userDataManager = NULL;
	}
}

UserDataManager::UserDataManager()
{
	m_pDb = NULL;  
}

void UserDataManager::initData()
{
	string path = CCFileUtils::sharedFileUtils()->getWritablePath()+"data.db"; 

	{
		string platform,id;
		if (History::getHistory()->getLastUserPlatformAndID(platform, id))
		{
			string temp_path = path.substr(0, path.find_last_of("."));
			path = temp_path + "_" + platform + "_" + id + ".db";

			//第一次登录，那么绑定到之前的数据
			string oldNoneUserPath = CCFileUtils::sharedFileUtils()->getWritablePath()+"data.db";
			if (CCFileUtils::sharedFileUtils()->isFileExist(oldNoneUserPath))
			{
				if (0 == rename(oldNoneUserPath.c_str(), path.c_str())){
					CCLog("%s --->SUCCESS rename file [%s] to [%s]", __FUNCTION__, oldNoneUserPath.c_str(), path.c_str());
				}else {
					CCLog("%s --->FAILED rename file [%s] to [%s]", __FUNCTION__, oldNoneUserPath.c_str(), path.c_str());
					CCAssert(false, "");
				}
			}
		}
	}




	CCString sql;  
	int result;  
	result = sqlite3_open(path.c_str(), &m_pDb);  
	if(result != SQLITE_OK)  
	{
		CCLog("open database failed,  number%d",result);  
		m_pDb = NULL;
		return;
	}

	// help map
	sql = "CREATE TABLE help_map(ID INTEGER PRIMARY KEY,count INTEGER DEFAULT 0)";
	result = sqlite3_exec(m_pDb, sql.getCString(), NULL,NULL,NULL); 
	if(result != SQLITE_OK)  
		CCLog("create table failed, table is exist"); 
	for (int i=0; i< GameLayerFactory::getInstance()->getTotalSectionNum(); ++i)
	{
		sql.initWithFormat("insert into help_map values(%d, 0)", i+1);  
		result = sqlite3_exec(m_pDb,sql.getCString(),NULL,NULL,NULL);  
// 		if(result != SQLITE_OK)  
// 			CCLog("insert data failed!"); 
	}

	// special tool
	sql = "CREATE TABLE special_tool(ID INTEGER PRIMARY KEY,state INTEGER DEFAULT 0,count INTEGER DEFAULT 0)";
	result = sqlite3_exec(m_pDb, sql.getCString(), NULL,NULL,NULL);
	if(result != SQLITE_OK)  
		CCLog("create table failed, table is exist");  
	for (int i=0; i<20; ++i)
	{
		sql.initWithFormat("insert into special_tool values(%d, 0, 0)", i+1);  
		result = sqlite3_exec(m_pDb,sql.getCString(),NULL,NULL,NULL);  
// 		if(result != SQLITE_OK)  
// 			CCLog("insert data failed!"); 
	}

	//select
	char **re;  
	int r,c;  

	{
		//第3个参数是查询结果，它依然一维数组（不要以为是二维数组，更不要以为是三维数组）。
		//它内存布局是：第一行是字段名称，后面是紧接着是每个字段的值
		// col个数据之后才是表数据，之前的是列名字
		result = sqlite3_get_table(m_pDb,"select * from help_map",&re,&r,&c,NULL);  
		CCLog("help_map: row is %d,column is %d",r,c); 

		m_helpMap.clear();
		for (int i=0; i< GameLayerFactory::getInstance()->getTotalSectionNum(); ++i)
		{
//			CCLog("row %d ---> id = %s, count = %s", i+1, re[c + i*c], re[c + i*c +1]);
			int val = CCString(re[c + i*c +1]).intValue();
			m_helpMap.push_back(val);
		}
		sqlite3_free_table(re); 
	}

	{
		result = sqlite3_get_table(m_pDb,"select * from special_tool",&re,&r,&c,NULL);  
		CCLog("special_tool row is %d,column is %d",r,c);  

		m_specialItemState.clear();
		m_specialItem.clear();
		for (int i=0; i< 20; ++i)
		{
//			CCLog("row %d ---> id = %s, state = %s, count = %s", i+1, re[c + i*c], re[c + i*c +1], re[c + i*c + 2]);			
			int val = CCString(re[c + i*c +1]).intValue();
			m_specialItemState.push_back(val);
			val = CCString(re[c + i*c +2]).intValue();
			m_specialItem.push_back(val);
		}
		sqlite3_free_table(re); 
	}
 
}

UserDataManager::~UserDataManager()
{
	sqlite3_close(m_pDb); 
}

vector<IllustrationInfo> UserDataManager::getIllustration()
{
	vector<IllustrationInfo> vec;

	int id = GameInforEditer::getGameInfor()->getLastPlayedSeasonId();
	/*SectionInfor *infor = GameInforEditer::getGameInfor()->getSectionInfor(id, PerGuanKaCount);
	if (! infor || ! infor->isSuccessed)
		id -= 1;
*/
	SeasonInfor* seasoninfor = GameInforEditer::getGameInfor()->getSeasonInfor(id+1);
	if (!seasoninfor || seasoninfor->locked )
	{
		id -= 1;
	}

	for (int i=1; i<=id; ++i)
	{
		IllustrationInfo temp;
		temp.fileName = getIllustrationFileStr(i);
		temp.seasonId = i;
		vec.push_back(temp);
	}

	return vec;
}

size_t UserDataManager::getIllstrationSaveToServerData()
{
    size_t ret = 0;
    size_t iCount = getIllustration().size();
    for (size_t i = 0; i < 5; ++i)
    {
        if (i < iCount)
        {
            ret |= (1 << i);
        }
        else
        {
            ret |= (0 << i);
        }
    }
    
    return ret;
}

//void UserDataManager::saveServerDataToFile(size_t value)
//{
//    for (int i = 5; i >= 0; i--)
//    {
//        if (value >> i & 0x01)
//        {
//            
//        }
//    }
//}

string UserDataManager::getIllustrationFileStr(int id)
{
	CCString temp;
	temp.initWithFormat(ResManager::getManager()->getSharedFilePath(g_collectlayerPath+"chatu_%d_biankuang.png").c_str(), id);

	return temp.getCString();
}

vector<string> UserDataManager::getAchievements()
{
	vector<string> ret;

	vector<Achievement> vec = AchievementManager::getInstance()->getAchievementList();
	for (size_t i=0; i<vec.size(); ++i)
	{
		// achiev_***.png
		//string lableFileName = "achiev_" + vec.at(i).name /*+ "_zh"*/ + ".png";
		string lableFileName = vec.at(i).filename;
		ret.push_back(lableFileName);
	}

	return ret;
}


//攻略图   没有就是false
vector<int> UserDataManager::getAllHelpMapsState()
{
	return m_helpMap;
}
int UserDataManager::getOneSectionMapState(int section)
{
	return m_helpMap.at(section - 1);
}
void UserDataManager::addOneSectionMap(int section, int addNum)
{
	if (m_pDb)
	{
		CCString sql;
		int result;
		sql.initWithFormat("UPDATE help_map SET count=%d WHERE ID=%d", m_helpMap.at(section-1)+addNum, section);
		result = sqlite3_exec(m_pDb, sql.getCString(), NULL,NULL,NULL);
		if(result != SQLITE_OK)  
			CCLog("update table help_map failed");
		else 
			m_helpMap.at(section-1) += addNum;
	}
}

//特殊 剧情道具
vector<int> UserDataManager::getSpecialItemNum()
{
	return m_specialItem;
}
vector<int> UserDataManager::getSpecialItemState()
{
	return m_specialItemState;
}
int UserDataManager::getSpecialItemNum(int daojuId)
{
	return m_specialItem.at(daojuId - 1);
}
void UserDataManager::addSpecialItemNum(int daojuId, int addNum)
{
	if (m_pDb)
	{
		CCString sql;
		int result;
		sql.initWithFormat("UPDATE special_tool SET count=%d WHERE ID=%d", m_specialItem.at(daojuId-1)+addNum, daojuId);
		result = sqlite3_exec(m_pDb, sql.getCString(), NULL,NULL,NULL);
		if(result != SQLITE_OK)  
			CCLog("update table special_tool failed");
		else 
		{
			m_specialItem.at(daojuId-1) += addNum;
			if (m_specialItemState.at(daojuId-1) == DJS_NotGetted)
				updateSpecialItemState(daojuId, DJS_GettedByBuy);
		}
	}
}
void UserDataManager::updateSpecialItemState(int daojuId, int state)
{
	if (m_pDb)
	{
		CCString sql;
		int result;
		sql.initWithFormat("UPDATE special_tool SET state=%d WHERE ID=%d", state, daojuId);
		result = sqlite3_exec(m_pDb, sql.getCString(), NULL,NULL,NULL);
		if(result != SQLITE_OK)  
			CCLog("update table special_tool failed");
		else 
			m_specialItemState.at(daojuId-1) = state;
	}
}