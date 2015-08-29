#include "History.h"
#include <stdlib.h>
#include "LFFileUtiles.h"
#include "include_json.h"
#include "GameInforEditer.h"
#include "SocialSeasonInfor.h"
#include "AES.h"

#define HISTORY_FILE "infor/history_v1_2.txt"
static const string SCOPE_MARK	= "SCOPE_";

// fuck !!!
// 不能在这里new
// 因为对于android的话，还在loadLib的时候就要new了
// 但是这里却又需要android的数据getDeviceUniqueId，这时Activity都没有
// 所以这里会崩溃的
//History* History::instance = new History();

History* History::instance = NULL;

History::History(void)
{
	// 只有win32不加密
	encrypt = (CC_PLATFORM_WIN32 != CC_TARGET_PLATFORM);
	GameInforEditer::getAESKey(m_AES_key, encrypt); //该函数使用静态，是为了避免gameinforediter在history之前初始化
}
History::~History(void)
{  
}


History* History::getHistory()
{
	if (instance == NULL)
	{
		instance = new History();
	}
	return instance;
}

void History::save(string jsonDoc)
{
	bool success = false;
	if (encrypt)
	{
		AES aes;
		success = aes.saveFileAndEncryptText(
			LFFileUtiles::getLFFileUtiles()->getDocFilePath(HISTORY_FILE).c_str(),
			m_AES_key,jsonDoc);
	}else
	{
		success = LFFileUtiles::getLFFileUtiles()->writeToDocFile(HISTORY_FILE,jsonDoc);
	}

	if (!success)
	{
		CCLog("Error :%s --> save error!",__FUNCTION__);
		CCLog("=======================================================================");
		CCLog("%s", jsonDoc.c_str());
		CCLog("=======================================================================");
		CCAssert(false,"save history file error!");
	}
}

void History::save()
{
	save(genJsonString());
}

void History::initData()
{
	LFFileUtiles* fileUtiles = LFFileUtiles::getLFFileUtiles();
	bool succ = fileUtiles->copyFileFromResToDocIfNotExist(HISTORY_FILE,encrypt?m_AES_key:NULL);

	if (!succ)
	{
		CCAssert(false,"copy file from res to doc error!");
		return;
	}

	string jsonDoc = "";
	if (encrypt)
	{
		AES aes;
		bool suc = aes.ReadFileAndDecryptText(
			fileUtiles->getDocFilePath(HISTORY_FILE).c_str(),
			m_AES_key,jsonDoc);
		if (!suc)
		{
			CCLog("Error : %s\n=======readInforFile(%s) faild!",__FUNCTION__,HISTORY_FILE);
			CCAssert(false,"");
			return;
		}
	}
	else
	{
		unsigned long size;
		const char* doc = fileUtiles->readDocFile(HISTORY_FILE,&size);
		//文件读取失败就返回NULL;
		if (size<=0)
		{
			CCLog("Error : %s=======readInforFile(%s) faild!",__FUNCTION__,HISTORY_FILE);
			CCAssert(false,"Load histroy file from doc error!");
			return;
		}
		jsonDoc = string(doc, size);
		delete [] doc;
	}
	//unsigned long size;
	//const char* doc = fileUtiles->readDocFile(HISTORY_FILE,&size);
	////文件读取失败就返回NULL;
	//if (size<=0)
	//{
	//	CCAssert(false,"Load histroy file from doc error!");
	//	return;
	//}
	//string jsonDoc = string(doc);
	//delete [] doc;

	initFromJsonString(jsonDoc);
}

void History::initFromJsonString(string jsonDoc)
{
	Json::Reader reader;
	Json::Value root;

	//解析不成功就返回空;
	if (!reader.parse(jsonDoc, root))
	{
		CCAssert(false,"History::initFromJsonString--->parse json file error!");
		return;
	}

	vector<int> scopeList;
	//解析SCOPE_NAME_LIST
	{
		Json::Value _SCOPE_NAME_LIST = root["SCOPE_NAME_LIST"];

		if (!_SCOPE_NAME_LIST.isNull() && _SCOPE_NAME_LIST.isArray())
		{
			int _scopeNum = _SCOPE_NAME_LIST.size();
			for (int i=0;i<_scopeNum;i++)
			{
				Json::Value onePair = _SCOPE_NAME_LIST[i];
				string _name = onePair["scope_name"].asString();

				// 去除SCOPE_MARK的这一节
				string _id = _name.substr(SCOPE_MARK.length(),_name.length());
				int scope = atoi(_id.c_str());
				scopeList.push_back(scope);
			}
		}
	}

	// 解析每一个scope
	{
		histotys.clear();

		char tempScope[64];
		int scopeNum = scopeList.size();
		for (int i=0;i<scopeNum;i++)
		{
			OneScope scope;
			scope.scope = scopeList[i];
			sprintf(tempScope,"%s%d",SCOPE_MARK.c_str(),scope.scope); 

			Json::Value _SCOPE = root[tempScope];

			Json::Value _OWNER_LIST = _SCOPE["owner_list"];
			if (!_OWNER_LIST.isNull() && _OWNER_LIST.isArray())
			{
				int ownerNum = _OWNER_LIST.size();
				for (int ownerIndex = 0;ownerIndex<ownerNum;ownerIndex++)
				{
					Json::Value _ONE_OWNER = _OWNER_LIST[ownerIndex];
					string owner = _ONE_OWNER["owner"].asString();

					OwnerHistory owerHistory;
					owerHistory.owner = owner;

					Json::Value _History_LIST = _ONE_OWNER["history_list"];
					if (!_History_LIST.isNull() && _History_LIST.isArray())
					{
						int historyNum = _History_LIST.size();
						for (int historyIndex = 0;historyIndex<historyNum;historyIndex++)
						{
							Json::Value _ONE_HISTORY = _History_LIST[historyIndex];
							string key = _ONE_HISTORY["key"].asString();
							HistoryValueItem valueItem;

							if (_ONE_HISTORY["value"].isInt())
							{
								int iValue = _ONE_HISTORY["value"].asInt();
								valueItem.putInt(iValue);
							}
							else if (_ONE_HISTORY["value"].isBool())
							{
								bool bValue = _ONE_HISTORY["value"].asBool();
								valueItem.putBool(bValue);
							}
							else if (_ONE_HISTORY["value"].isString())
							{
								string sValue = _ONE_HISTORY["value"].asString();
								valueItem.putString(sValue);
							}else
							{
								CCAssert(false,"History::initFromJsonString--->Not surported data format!");
							}

							// 检测是否已经存在
							{
								map<string,HistoryValueItem>::iterator it = owerHistory.ownerHistory.find(key);
								if (it != owerHistory.ownerHistory.end())
								{
									CCLog("History-->Waring : the key [%s] is exist in scope [%d],where owner is [%s].", key.c_str(),scope.scope,owner.c_str());
								}
							}

							// 添加进owner
							owerHistory.ownerHistory.insert(pair<string,HistoryValueItem>(key,valueItem));
						}
					}

					// 添加进scope
					scope.owners.push_back(owerHistory);
				}
			}

			// 添加进history
			histotys.push_back(scope);
		}
	}

	// 校验scope是否重复

	// 校验owner在同一个scope中是否重复


	CCLog("History init success!");

	//CCLog(genJsonString().c_str());
}

string History::genJsonString()
{
	Json::Value root;
	//生成SCOPE_NAME_LIST的信息
	{
		char scopeName[64];
		int scopeNum = histotys.size();
		for (int i=0;i<scopeNum;i++)
		{
			Json::Value scopeInfor;
			sprintf(scopeName,"%s%d",SCOPE_MARK.c_str(),histotys[i].scope);
			scopeInfor["scope_name"] = Json::Value(string(scopeName));

			root["SCOPE_NAME_LIST"].append(scopeInfor);
		}
	}

	// 挨个的存放Scope信息
	{
		int scopeNum = histotys.size();
		char scopeName[64];
		for (int scopeIndex = 0;scopeIndex<scopeNum;scopeIndex++)
		{
			sprintf(scopeName,"%s%d",SCOPE_MARK.c_str(),histotys[scopeIndex].scope);

			Json::Value oneScope;

			int ownerNum = histotys[scopeIndex].owners.size();
			for (int ownerIndex = 0;ownerIndex<ownerNum;ownerIndex++)
			{
				Json::Value oneOwner;
				oneOwner["owner"] = Json::Value(histotys[scopeIndex].owners[ownerIndex].owner);

				map<string,HistoryValueItem>::iterator it = histotys[scopeIndex].owners[ownerIndex].ownerHistory.begin();
				for (;it!= histotys[scopeIndex].owners[ownerIndex].ownerHistory.end();it++)
				{
					Json::Value oneHistory;
					oneHistory["key"] = Json::Value(it->first);
					if (it->second.valueType == VType_INT)
					{
						oneHistory["value"] = Json::Value(it->second.getInt());
					}
					else if (it->second.valueType == VType_BOOL)
					{
						oneHistory["value"] = Json::Value(it->second.getBool());
					}
					else if (it->second.valueType == VType_STRING)
					{
						oneHistory["value"] = Json::Value(it->second.getString());
					}else
					{
						CCAssert(false,"Not surported data format!");
						continue;
					}

					oneOwner["history_list"].append(oneHistory);
				}

				oneScope["owner_list"].append(oneOwner);
			}

			root[scopeName] = oneScope;
		}
	}

	//得到配置文本
	Json::StyledWriter styled_writer;
	return styled_writer.write(root);
}

void History::put(string key,HistoryValueItem value,string owner,int scope)
{
	bool foundOwer = false;
	int scopeIndex = -1;

	map<string,HistoryValueItem>::iterator it;

	int scopeNum = histotys.size();
	for (int i=0;i<scopeNum;i++)
	{
		if (histotys[i].scope == scope)
		{// scope

			scopeIndex = i;

			int ownerNum = histotys[i].owners.size();

			for (int j=0;j<ownerNum;j++)
			{
				if (histotys[i].owners[j].owner == owner)
				{// owner

					foundOwer = true;

					it = histotys[i].owners[j].ownerHistory.find(key);
					if (it != histotys[i].owners[j].ownerHistory.end())
					{// key

						// 更新值
						it->second = value;

						return;
					}else
					{// 插入新值

						histotys[i].owners[j].ownerHistory.insert(pair<string,HistoryValueItem>(key,value));

						return;
					}
				}
			}// owner循环
		}
	}// scope 循环

	// 根本就没找到scope，那么就新建scope
	if (scopeIndex == -1)
	{
		OneScope newScope;
		newScope.scope = scope;

		OwnerHistory newOwner;
		newOwner.owner = owner;

		newOwner.ownerHistory.insert(pair<string,HistoryValueItem>(key,value));

		newScope.owners.push_back(newOwner);

		histotys.push_back(newScope);	// 存放scope
	}

	if (!foundOwer && scopeIndex >= 0)
	{
		OwnerHistory newOwner;
		newOwner.owner = owner;

		newOwner.ownerHistory.insert(pair<string,HistoryValueItem>(key,value));

		histotys[scopeIndex].owners.push_back(newOwner); // 存放owner
	}
}

void History::put(string key,int value,string owner,int scope)
{
	HistoryValueItem item;
	item.putInt(value);
	put(key,item,owner,scope);
}
void History::put(string key,bool value,string owner,int scope)
{
	HistoryValueItem item;
	item.putBool(value);
	put(key,item,owner,scope);
}
void History::put(string key,string value,string owner,int scope)
{
	HistoryValueItem item;
	item.putString(value);
	put(key,item,owner,scope);
}

void History::putInApp(string key,int value)
{
	put(key,value,OWNER_APP,SCOPE_APP);
}
void History::putInApp(string key,bool value)
{
	put(key,value,OWNER_APP,SCOPE_APP);
}
void History::putInApp(string key,string value)
{
	put(key,value,OWNER_APP,SCOPE_APP);
}
void History::putInUser(string key,int value,string userId)
{
	put(key,value,userId,SCOPE_USERID);
}
void History::putInUser(string key,bool value,string userId)
{
	put(key,value,userId,SCOPE_USERID);
}
void History::putInUser(string key,string value,string userId)
{
	put(key,value,userId,SCOPE_USERID);
}

void History::putInCurrentUser(string key,int value)
{
	CCAssert(false,"红星大挪移目前没有user这个概念");
	//string userId = GameInforEditer::getGameInfor()->getLastUsername();
	//putInUser(key,value,userId);
}
void History::putInCurrentUser(string key,bool value)
{
	CCAssert(false,"红星大挪移目前没有user这个概念");

	//string userId = GameInforEditer::getGameInfor()->getLastUsername();
	//putInUser(key,value,userId);
}
void History::putInCurrentUser(string key,string value)
{
	CCAssert(false,"红星大挪移目前没有user这个概念");

	//string userId = GameInforEditer::getGameInfor()->getLastUsername();
	//putInUser(key,value,userId);
}



void History::putInHero(string key,int value,int mainHeroId)
{
	char temp[64];
	sprintf(temp,"%d",mainHeroId);
	put(key,value,string(temp),SCOPE_HEROID);
}
void History::putInHero(string key,bool value,int mainHeroId)
{
	char temp[64];
	sprintf(temp,"%d",mainHeroId);
	put(key,value,string(temp),SCOPE_HEROID);
}
void History::putInHero(string key,string value,int mainHeroId)
{
	char temp[64];
	sprintf(temp,"%d",mainHeroId);
	put(key,value,string(temp),SCOPE_HEROID);
}


void History::putInCurrentHero(string key,int value)
{
	CCAssert(false,"红星大挪移目前没有hero这个概念");

	//int heroId = GameInforEditer::getGameInfor()->getLastLoginedHeroId();
	//putInHero(key,value,heroId);
}
void History::putInCurrentHero(string key,bool value)
{
	CCAssert(false,"红星大挪移目前没有hero这个概念");

	//int heroId = GameInforEditer::getGameInfor()->getLastLoginedHeroId();
	//putInHero(key,value,heroId);
}
void History::putInCurrentHero(string key,string value)
{
	CCAssert(false,"红星大挪移目前没有hero这个概念");

	//int heroId = GameInforEditer::getGameInfor()->getLastLoginedHeroId();
	//putInHero(key,value,heroId);
}


bool History::getValue(string key,string owner,int scope,HistoryValueItem& value)
{
	map<string,HistoryValueItem>::iterator it;

	int scopeNum = histotys.size();
	for (int i=0;i<scopeNum;i++)
	{
		if (histotys[i].scope == scope)
		{// scope

			int ownerNum = histotys[i].owners.size();

			for (int j=0;j<ownerNum;j++)
			{
				if (histotys[i].owners[j].owner == owner)
				{// owner

					it = histotys[i].owners[j].ownerHistory.find(key);
					if (it != histotys[i].owners[j].ownerHistory.end())
					{// key
						value = it->second;
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool History::getInt(string key,string owner,int scope,int& value)
{
	HistoryValueItem item;
	if (getValue(key,owner,scope,item))
	{
		value = item.getInt();
		return true;
	}
	return false;
}
bool History::getBool(string key,string owner,int scope,bool& value)
{
	HistoryValueItem item;
	if (getValue(key,owner,scope,item))
	{
		value = item.getBool();
		return true;
	}
	return false;
}
bool History::getString(string key,string owner,int scope,string& value)
{
	HistoryValueItem item;
	if (getValue(key,owner,scope,item))
	{
		value = item.getString();
		return true;
	}
	return false;
}

bool History::getIntFromApp(string key,int& value)
{
	return getInt(key,OWNER_APP,SCOPE_APP,value);
}
bool History::getBoolFromApp(string key,bool& value)
{
	return getBool(key,OWNER_APP,SCOPE_APP,value);
}
bool History::getStringFromApp(string key,string& value)
{
	return getString(key,OWNER_APP,SCOPE_APP,value);
}

bool History::getIntFromUser(string key,string userId,int& value)
{
	return getInt(key,userId,SCOPE_USERID,value);
}
bool History::getBoolFromUser(string key,string userId,bool& value)
{
	return getBool(key,userId,SCOPE_USERID,value);
}
bool History::getStringFromUser(string key,string userId,string& value)
{
	return getString(key,userId,SCOPE_USERID,value);
}

bool History::getIntFromCurrentUser(string key,int& value)
{
	CCAssert(false,"红星大挪移目前没有user这个概念");
	return false;

	//string userId = GameInforEditer::getGameInfor()->getLastUsername();
	//return getIntFromUser(key,userId,value);
}
bool History::getBoolFromCurrentUser(string key,bool& value)
{
	CCAssert(false,"红星大挪移目前没有user这个概念");
	return false;

	//string userId = GameInforEditer::getGameInfor()->getLastUsername();
	//return getBoolFromUser(key,userId,value);
}
bool History::getStringFromCurrentUser(string key,string& value)
{
	CCAssert(false,"红星大挪移目前没有user这个概念");
	return false;

	//string userId = GameInforEditer::getGameInfor()->getLastUsername();
	//return getStringFromUser(key,userId,value);
}


bool History::getIntFromHero(string key,int heroId,int& value)
{
	char temp[64];
	sprintf(temp,"%d",heroId);
	return getInt(key,string(temp),SCOPE_HEROID,value);
}
bool History::getBoolFromHero(string key,int heroId,bool& value)
{
	char temp[64];
	sprintf(temp,"%d",heroId);
	return getBool(key,string(temp),SCOPE_HEROID,value);
}
bool History::getStringFromHero(string key,int heroId,string& value)
{
	char temp[64]; 
	sprintf(temp,"%d",heroId);
	return getString(key,string(temp),SCOPE_HEROID,value);
}

bool History::getIntFromCurrentHero(string key,int& value)
{
	CCAssert(false,"红星大挪移目前没有hero这个概念");
	return false;

	//int heroId = GameInforEditer::getGameInfor()->getLastLoginedHeroId();
	//return getIntFromHero(key,heroId,value);
}
bool History::getBoolFromCurrentHero(string key,bool& value)
{
	CCAssert(false,"红星大挪移目前没有hero这个概念");
	return false;

	//int heroId = GameInforEditer::getGameInfor()->getLastLoginedHeroId();
	//return getBoolFromHero(key,heroId,value);
}
bool History::getStringFromCurrentHero(string key,string& value)
{
	CCAssert(false,"红星大挪移目前没有hero这个概念");
	return false;

	//int heroId = GameInforEditer::getGameInfor()->getLastLoginedHeroId();
	//return getStringFromHero(key,heroId,value);
}


// key:PlatformInfo   value:platform,id
void History::setLastUserPlatformAndID(const string& platform, const string& id, const string& name, const string& pictureUrl)
{
	string key   = "PlatformInfo";
	string value = platform;
	value += ",";
	value += id;
	this->putInApp(key, value);
	this->putInApp("UserName", name);
	this->putInApp("UserPicture", pictureUrl);
	save();
}
bool History::getLastUserPlatformAndID(string& platform, string& id, string* name/*=NULL*/, string* pictureUrl/*=NULL*/)
{
	string key = "PlatformInfo";
	string value;// = "Facebook,123456789";
	bool ret = this->getStringFromApp(key, value);
	if (name) ret = ret && this->getStringFromApp("UserName", *name);
	if (pictureUrl) ret = ret && this->getStringFromApp("UserPicture", *pictureUrl);
	if (!ret) return false;

	int idx = value.find_first_of(",");
	platform = value.substr(0, idx);
	id = value.substr(idx+1, value.size());
	return true;
}