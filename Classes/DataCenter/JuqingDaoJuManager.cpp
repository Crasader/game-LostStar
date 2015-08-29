#include "JuqingDaoJuManager.h"
#include "include_json.h"
#include "LFFileUtiles.h"
#include <algorithm> 
#include "GameLayerFactory.h"
#include "AES.h"
#include "UserDataManager.h"


JuqingDaoJuManager* JuqingDaoJuManager::mInstance = NULL;
#define JSON_FILE  "config/juqing_daoju.txt"


inline bool sortByIdAsc(const JuqingDaoJu& a,const JuqingDaoJu& b)
{
	return a.daojuId < b.daojuId;
}


JuqingDaoJuManager::JuqingDaoJuManager(void)
{
	{
		bool success;
		string jsonDoc = LFFileUtiles::getLFFileUtiles()->getResourceFile(JSON_FILE,success);
		if(!success)
		{
			CCAssert(false,"");
			return;
		}

		Json::Reader reader;
		Json::Value root;

		//解析不成功就返回空;
		if (!reader.parse(jsonDoc, root))
		{
			CCAssert(false,"History::initFromJsonString--->parse json file error!");
			return;
		}

		Json::Value _ITEM_LIST = root["item_list"];
		int temp_id[128];
		memset(temp_id,0,sizeof(temp_id));
		for (int i=0,itemCount=_ITEM_LIST.size(); i<itemCount; i++)
		{
			Json::Value ONE_ITEM = _ITEM_LIST[i];
			JuqingDaoJu item;

			string str1 = "daoju_";
			//string str2 = "daoju_meibaibian_";
			string str2 = "daoju_";

			item.daojuId = ONE_ITEM["daojuId"].asInt();
			item.sectionId = ONE_ITEM["sectionId"].asInt();
			float posx = ONE_ITEM["point_x"].asDouble();
			float posy = ONE_ITEM["point_y"].asDouble();
			item.posInGame = b2Vec2(posx,posy);
			item.fileName_Origin = ONE_ITEM["fileName"].asString();
			item.fileName = str1 + item.fileName_Origin;
			item.fileName_WithOutWhiteBoarder = str2 + item.fileName_Origin;
			item.daojuName = ONE_ITEM["daojuName"].asString();
			item.describe = ONE_ITEM["daojuDescribe"].asString();
//			item.state = (DaoJuState)ONE_ITEM["state"].asInt();

			// 简单校验
			temp_id[item.daojuId] ++;
			if (temp_id[item.daojuId] >= 2)
			{
				CCLog("Error : id error,daojuId = %d",item.daojuId);
				CCAssert(false,"重复的Id");
			}

			// 临时放进去，还得排序
			items.push_back(item);
		}
	}

	// 排序
	std::sort(items.begin(),items.end(),sortByIdAsc);

	// 检测section是否配置错误
	int seasonNum = GameLayerFactory::getInstance()->getConfigSeasonNum(type_Normal);
	CCAssert((int)items.size() >= ITEM_NUM_IN_EACH_SEASON*seasonNum,"个数要够");
	int section_id[128];
	for (int i=0;i<seasonNum;i++)
	{
		memset(section_id,0,sizeof(section_id));
		for (int j=0;j<ITEM_NUM_IN_EACH_SEASON;j++)
		{
			int index = i*ITEM_NUM_IN_EACH_SEASON +j;

			section_id[ items[index].sectionId ] ++;

			if ( section_id[ items[index].sectionId ] >= 2)
			{
				CCLog("seasonId = %d,sectionId = %d",i+1,items[index].sectionId );
				CCAssert(false,"当前章节的当前Section已经配置过一个Item了");
			}
		}
	}


	encrypt = (CC_PLATFORM_WIN32 != CC_TARGET_PLATFORM);
	GameInforEditer::getGameInfor()->getAESKey(m_AES_key, encrypt);
	//
	vector<int> vec_ = UserDataManager::getInstance()->getSpecialItemState();
	if (vec_.size() != items.size())
	{
		CCLog("Error: %s the size of vec_ != items ", __FUNCTION__);
		return;
	}
	for (size_t i=0; i<items.size(); ++i)
	{
		items.at(i).state = (DaoJuState)vec_.at(i);
	}

}

JuqingDaoJuManager* JuqingDaoJuManager::getManager()
{
	if (mInstance == NULL)
	{
		mInstance = new JuqingDaoJuManager();
	}
	return mInstance;
}

vector<JuqingDaoJu> JuqingDaoJuManager::getAllGettedItems()
{
	vector<JuqingDaoJu> ret;

	for (int i=0,count=items.size(); i<count; i++)
	{
		if (items[i].state != DJS_NotGetted)
		{
			ret.push_back(items[i]);
		}
	}

	return ret;
}

JuqingDaoJu JuqingDaoJuManager::getDaoJuById(int id)
{
	for (size_t i=0; i<items.size(); ++i)
	{
		if (items.at(i).daojuId == id)
			return items.at(i);
	}

	CCAssert(false, "传入的id不在 1-20之间");
	return JuqingDaoJu();
}

vector<JuqingDaoJu> JuqingDaoJuManager::getAllItemsInSeason(int seasonId)
{
	vector<JuqingDaoJu> ret;

	for (int i=0,count=items.size(); i<count; i++)
	{
		if (isItemBelongToSection(items[i].daojuId,seasonId))
		{
			ret.push_back(items[i]);
		}
	}

	return ret;
}

bool JuqingDaoJuManager::canUnlockedInSeason(int seasonId){
//	int j;
// 	for(j=0;j<4;++j){
// 		if(!isItemGettedInSection(seasonId,(seasonId-1)*ITEM_NUM_IN_EACH_SEASON+1+j)){
// 			return false;
// 		}
// 	}

	vector<JuqingDaoJu> vec = getAllItemsInSeason(seasonId);
	if (vec.size() < 4) return false;

	for (size_t i=0; i<vec.size(); ++i)
	{
		if (vec.at(i).state == DJS_NotGetted) return false;
	}

	return true;
}

bool JuqingDaoJuManager::isCollectAllInSeason(int seasonId)
{
	return (getAllItemsInSeason(seasonId).size() == ITEM_NUM_IN_EACH_SEASON);
}

vector<JuqingDaoJu> JuqingDaoJuManager::getItemInSeason(int seasonId)
{
	vector<JuqingDaoJu> ret;

	int index = (seasonId-1)*ITEM_NUM_IN_EACH_SEASON;
	for (int i=index; i<ITEM_NUM_IN_EACH_SEASON*seasonId; i++)
	{
		ret.push_back(items[i]);
	}

	return ret;
}

int JuqingDaoJuManager::howManyItemBuyedInSeason(int seasonId)
{
	int ret = 0;

	vector<JuqingDaoJu> temp = getItemInSeason(seasonId);
	for (int i=0,count=temp.size(); i<count; i++)
	{
		if (temp[i].state == DJS_GettedByBuy)
		{
			ret ++;
		}
	}

	return ret;
}


bool JuqingDaoJuManager::getItemInSection(int seasonId,int sectionId,JuqingDaoJu& outData)
{
	bool found = false;
	vector<JuqingDaoJu> itemInSeason = getAllItemsInSeason(seasonId);

	for (int i=0,count = itemInSeason.size(); i<count; i++)
	{
		if (itemInSeason[i].sectionId == sectionId)
		{
			outData = itemInSeason[i];
			found = true;
		}
	}
	return found;
}
bool JuqingDaoJuManager::isItemConfigInSection(int seasonId,int sectionId)
{
	JuqingDaoJu temp;
	return getItemInSection(seasonId,sectionId,temp);
}

bool JuqingDaoJuManager::isItemGettedInSection(int seasonId,int sectionId)
{
	JuqingDaoJu temp;
	if (!getItemInSection(seasonId,sectionId,temp))
	{
		CCAssert(false,"这关没有配置道具，不知道返回什么，外界也不应该调用");
		return false;
	}
	return temp.state != DJS_NotGetted;
}
void JuqingDaoJuManager::setItemIsGettedState(int seasonId,int sectionId,DaoJuState stat)
{
	for (int i=0,count=items.size(); i<count; i++)
	{
		if (isItemBelongToSection(items[i].daojuId,seasonId) && items[i].sectionId == sectionId)
		{
			items[i].state = stat;
			UserDataManager::getInstance()->updateSpecialItemState(items[i].daojuId, stat);
			//saveData();
		}
	}
}

void JuqingDaoJuManager::setItemIsGettedState(int daojuId,DaoJuState stat)
{
	for (int i=0,count=items.size(); i<count; i++)
	{
		if (daojuId == items.at(i).daojuId)
		{
			items[i].state = stat;
			UserDataManager::getInstance()->updateSpecialItemState(daojuId, stat);
			//saveData();
		}
	}
}

bool JuqingDaoJuManager::saveData()
{
	string seasonInforString = getDaoJuInfoStr();

	LFFileUtiles *fileUtiles = LFFileUtiles::getLFFileUtiles();

	if (encrypt)
	{
		AES aes;
		return aes.saveFileAndEncryptText(
			fileUtiles->getDocFilePath(JSON_FILE).c_str(),
			m_AES_key,seasonInforString);
	}
	bool sec = false;
	sec = fileUtiles->writeToDocFile(JSON_FILE,seasonInforString);

	return sec;
}

string JuqingDaoJuManager::getDaoJuInfoStr()
{
	Json::Value root;

	Json::Value itemList;
	for (int i=0,itemCount=items.size(); i<itemCount; i++)
	{
		JuqingDaoJu item = items.at(i);
		Json::Value oneItem;

		oneItem["daojuId"] = Json::Value(item.daojuId);
		oneItem["point_x"] = Json::Value(item.posInGame.x);
		oneItem["point_y"] = Json::Value(item.posInGame.y);
		oneItem["sectionId"] = Json::Value(item.sectionId);
		oneItem["daojuName"] = Json::Value(item.daojuName.c_str());
		oneItem["daojuDescribe"] = Json::Value(item.describe.c_str());
		oneItem["fileName"] = Json::Value(item.fileName_Origin.c_str());
//		oneItem["state"] = Json::Value(item.state);

		itemList.append(oneItem);
	}
	root["item_list"] = itemList;

	//得到配置文本
	Json::StyledWriter styled_writer;

	return styled_writer.write(root); 
}

void JuqingDaoJuManager::reset()
{
    delete mInstance;
    mInstance = NULL;
    JuqingDaoJuManager::getManager();
}

size_t JuqingDaoJuManager::getSaveToServerData()
{
    size_t ret = 0;
    for (size_t i = 0; i < items.size(); ++i)
    {
        if (items.at(i).state != DJS_NotGetted)
        {
            ret |= (1 << i);
        }
    }
    
    return ret;
}

void JuqingDaoJuManager::saveServerDataToFile(size_t value)
{
    for (int i = items.size()-1; i >= 0; i--)
    {
        if (((value >> i) & 0x1) == 1)
        {
            CCLog("i is %d", i);
            setItemIsGettedState(i+1, DJS_GettedByGame);
        }
    }
    
}

vector<JuqingDaoJu> JuqingDaoJuManager::getAllItems()
{
	return items;
}


JuqingDaoJuManager::~JuqingDaoJuManager(void)
{
    items.clear();
}
