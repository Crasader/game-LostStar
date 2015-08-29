#include "RankingListManager.h"
#include "GameInforEditer.h"
#include "AchievementManager.h"
#include "UserDataManager.h"
#include "AES.h"
#include "GameLayerFactory.h"

#define Json_Value			"value"
#define Json_Time			"time"
#define Json_Order			"order"
#define Json_Name			"name"
#define Json_ID				"id"

#define	Json_EvaluateOrder			"evaluateOrder"
#define	Json_PerfectPassOrder		"perfectPassOrder"

#define	Json_MyEvaluateOrder		"myevaluateOrder"
#define	Json_MyPerfectPassOrder		"myperfectPassOrder"


const std::string RankingListFileName = "rankinglist.txt";

static RankingListManager* _rankListManager = NULL;

int reorderByVal(const void* data1, const void* data2)
{
	RankList_PerfectPassOrEvaluate* temp1 = (RankList_PerfectPassOrEvaluate*)data1;
	RankList_PerfectPassOrEvaluate* temp2 = (RankList_PerfectPassOrEvaluate*)data2;

	if (temp1->val < temp2->val)
	{
		return 1;
	}
	else if (temp1->val == temp2->val)
	{
		if (temp1->time < temp2->time)
		{
			return 1;
		}
		return- 1;
	}
	return -1;
}

bool reorderByTime(RankList_PerfectPassOrEvaluate data1, RankList_PerfectPassOrEvaluate data2)
{
	if (data1.val == data2.val)
	{
		if (data1.time < data2.time)
		{
			return false;
		}
	}
	return true;
}


RankingListManager* RankingListManager::getInstance()
{
	if (! _rankListManager)
	{
		_rankListManager = new RankingListManager();
	}

	return _rankListManager;
}

RankingListManager::RankingListManager()
{
	_myEvaluateOrder = 0;
	_myPerfectPassOrder = 0;

	string datastring;
	bool bEncrypt = (CC_PLATFORM_WIN32 != CC_TARGET_PLATFORM);
	if (bEncrypt)
	{
		char key[16];
		GameInforEditer::getGameInfor()->getAESKey(key, bEncrypt);

		AES aes;
		aes.ReadFileAndDecryptText((CCFileUtils::sharedFileUtils()->getWritablePath()+RankingListFileName).c_str(), key, datastring);
	}
	else
	{
		unsigned long len = 0;
		unsigned char* data = CCFileUtils::sharedFileUtils()->getFileData(
			(CCFileUtils::sharedFileUtils()->getWritablePath()+RankingListFileName).c_str(),"rb", &len);
		if (data)
		{
			datastring = (char *)data;
			CC_SAFE_DELETE(data);
		}
	}

	unsigned int id = GameInforEditer::getGameInfor()->getClientId();
	RankList_PerfectPassOrEvaluate temp;
	Json::Value root;
	Json::Reader reader;
	//解析不成功就返回空;
	if (!reader.parse(datastring, root))
	{
		CCLog("Error : %s --> parse json error!", __FUNCTION__);
	}
	else
	{	
		if (!root.isNull())
		{
			Json::Value array = root[Json_EvaluateOrder];
			for (size_t i = 0; i < array.size(); ++i)
			{
				temp.val = array[i][Json_Value].asUInt();
				temp.time = array[i][Json_Time].asUInt();
				temp.order = array[i][Json_Order].asUInt();
				temp.name = array[i][Json_Name].asString();
				temp.id = array[i][Json_ID].asUInt();
				_evaluateOrderList.push_back(temp);

				if (temp.id == id)
				{
					_myEvaluateOrder = temp.order;
				}
			}

			array = root[Json_PerfectPassOrder];
			for (size_t i = 0; i < array.size(); ++i)
			{
				temp.val = array[i][Json_Value].asUInt();
				temp.time = array[i][Json_Time].asUInt();
				temp.order = array[i][Json_Order].asUInt();
				temp.name = array[i][Json_Name].asString();
				temp.id = array[i][Json_ID].asUInt();
				_perfectPassOrderList.push_back(temp);

				if (temp.id == id)
				{
					_myPerfectPassOrder = temp.order;
				}
			}
		}
	}

}

RankList_PerfectPassOrEvaluate RankingListManager::getMyPerfectPass()
{
	unsigned int id = GameInforEditer::getGameInfor()->getClientId();
	
	bool bFind = false;
	cc_timeval curTime;
	CCTime::gettimeofdayCocos2d(&curTime, 0);

	RankList_PerfectPassOrEvaluate temp;
	for (size_t i = 0; i < _perfectPassOrderList.size(); i++)
	{
		temp = _perfectPassOrderList.at(i);
		if (temp.id == id)
		{
			_perfectPassOrderList.at(i).val = GameInforEditer::getGameInfor()->getMyEvaluateCount();
			_perfectPassOrderList.at(i).time = curTime.tv_usec;
			//更新数据
			reorderList(_perfectPassOrderList);
			save();
			bFind = true;
			break;
		}
	}

	if (bFind)
	{
		for (size_t i = 0; i < _perfectPassOrderList.size(); i++)
		{
			temp = _perfectPassOrderList.at(i);
			if (temp.id == id)
			{
				_myPerfectPassOrder = _perfectPassOrderList.at(i).order;

				//数据更新后，重新获取一次，这样才是最新的数据
				temp = _perfectPassOrderList.at(i);
				return temp;
			}
		}
	}

	temp.order = _myPerfectPassOrder;
	temp.name = GameInforEditer::getGameInfor()->getUserName();
	temp.val = GameInforEditer::getGameInfor()->getMyEvaluateCount();
	temp.id = GameInforEditer::getGameInfor()->getClientId();
	temp.time = curTime.tv_usec;

	return temp;
}
RankList_PerfectPassOrEvaluate RankingListManager::getMyEvaluate()
{
	unsigned int id = GameInforEditer::getGameInfor()->getClientId();
	
	bool bFind = false;

	cc_timeval curTime;
	CCTime::gettimeofdayCocos2d(&curTime, 0);

	RankList_PerfectPassOrEvaluate temp;
	for (size_t i = 0; i < _evaluateOrderList.size(); i++)
	{
		temp = _evaluateOrderList.at(i);
		if (temp.id == id)
		{
			_evaluateOrderList.at(i).val = GameInforEditer::getGameInfor()->getMyEvaluateCount();
			_evaluateOrderList.at(i).time = curTime.tv_usec;
			//更新数据
			reorderList(_evaluateOrderList);
			save();
			bFind = true;
			break;
		}
	}

	if (bFind)
	{
		for (size_t i = 0; i < _evaluateOrderList.size(); i++)
		{
			temp = _evaluateOrderList.at(i);
			if (temp.id == id)
			{
				_myEvaluateOrder = _evaluateOrderList.at(i).order;

				//数据更新后，重新获取一次，这样才是最新的数据
				temp = _evaluateOrderList.at(i);
				return temp;
			}
		}
	}

 	temp.order = _myEvaluateOrder;
 	temp.name = GameInforEditer::getGameInfor()->getUserName();
 	temp.val = GameInforEditer::getGameInfor()->getMyEvaluateCount();
 	temp.id = GameInforEditer::getGameInfor()->getClientId();
	temp.time = curTime.tv_usec;

	return temp;
}
void RankingListManager::setMyPerfectPassOrder(unsigned int val)
{
	_myPerfectPassOrder = val;
}
void RankingListManager::setMyEvaluateOrder(unsigned int val)
{
	_myEvaluateOrder = val;
}

//3星通关
vector<RankList_PerfectPassOrEvaluate> RankingListManager::getOrderPerfectPass()
{
	return _perfectPassOrderList;
}
//成就
vector<RankList_PerfectPassOrEvaluate> RankingListManager::getOrderEvaluate()
{
	return _evaluateOrderList;
}
void RankingListManager::setOrderPerfectPass(vector<RankList_PerfectPassOrEvaluate> vec)
{
	_perfectPassOrderList = vec;
	_perfectPassOrderList.push_back(getMyPerfectPass());
	save();
}
void RankingListManager::setOrderEvaluate(vector<RankList_PerfectPassOrEvaluate> vec)
{
	_evaluateOrderList = vec;
	_evaluateOrderList.push_back(getMyEvaluate());
	save();
}

void RankingListManager::reorderList(vector<RankList_PerfectPassOrEvaluate>& vec)
{
	if (vec.size() <= 0)
	{
		return;
	}
	std::qsort(&vec.at(0), vec.size(), sizeof(RankList_PerfectPassOrEvaluate), reorderByVal);
	
	unsigned int id = GameInforEditer::getGameInfor()->getClientId();
	int iCount = vec.size();
	for (int i = 0; i < iCount; i++)
	{
		if (id == vec.at(i).id )
		{
			if (i <= iCount-1)
			{
				vec.at(i).order = i+1;
			}
		}
		else 
		{
			vec.at(i).order = i+1;
		}
	}
}

void RankingListManager::save()
{
	Json::Value root;
	Json::StyledWriter writer;

	Json::Value array;
	Json::Value data;
	RankList_PerfectPassOrEvaluate temp;

	int maxPerfectPassOrderCount = GameLayerFactory::getInstance()->getTotalSectionNum()*3;
	int achevementCount = AchievementManager::getInstance()->getDefaultAchievementList().size();

	for (size_t i = 0; i < _evaluateOrderList.size(); ++i)
	{
		temp = _evaluateOrderList.at(i);
		data[Json_Value] = temp.val;
		data[Json_Time] = temp.time;
		data[Json_Order] = temp.order;
		data[Json_Name] = temp.name;
		data[Json_ID] = temp.id;
		if ((int)temp.val > achevementCount)
		{
			continue;
		}
		array.append(data); 
	}

	root[Json_EvaluateOrder] = array;

	array.clear();
	for (size_t i = 0; i < _perfectPassOrderList.size(); ++i)
	{
		temp = _perfectPassOrderList.at(i);
		data[Json_Value] = temp.val;
		data[Json_Time] = temp.time;
		data[Json_Order] = temp.order;
		data[Json_Name] = temp.name;
		data[Json_ID] = temp.id;
		if ((int)temp.val > maxPerfectPassOrderCount)
		{
			continue;
		}

		array.append(data);
	}

	root[Json_PerfectPassOrder] = array;
	string datastring = writer.write(root);

	bool bEncrypt = (CC_PLATFORM_WIN32 != CC_TARGET_PLATFORM);
	if (bEncrypt)
	{
		char key[16];
		GameInforEditer::getGameInfor()->getAESKey(key, bEncrypt);

		AES aes;
		aes.saveFileAndEncryptText((CCFileUtils::sharedFileUtils()->getWritablePath()+RankingListFileName).c_str(), key, datastring);
	}
	else
	{
		LFFileUtiles::getLFFileUtiles()->writeDataToFile(datastring, CCFileUtils::sharedFileUtils()->getWritablePath()+RankingListFileName);
	}
}

