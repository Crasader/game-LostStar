#pragma once

#include <string>
#include <map>
#include <vector>
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

#define OWNER_APP		"APP"		// 默认的唯一的全局数据

#define	SCOPE_APP		0		// 应用程序级别		---> 对应的组就是SCOPE_0
#define	SCOPE_USERID	1		// 用户账号级别		---> 对应的组就是SCOPE_1
#define	SCOPE_HEROID	2		// 用户的主英雄的级别	---> 对应的组就是SCOPE_2

enum ValueType
{
	VType_INT = 0,
	VType_BOOL,
	VType_STRING,
};

#define Show_SuperTool_Guide		"Show_SuperTool_Guide"
#define Show_Unlock_Animation       "Show_Unlock_Animation"
#define Show_Guide_1_2_Teach        "Show_Guide_1_2_Teach"
#define Scale_Teach_Is_Done			"Scale_Teach_Is_Done"
#define Juqingdaoju_Teach_Is_Done	"Juqingdaoju_Teach_Is_Done"
#define IsFirstUseSurperTool		"IsFirstUseSurperTool"
#define IsFirstCollect				"IsFirstCollect"
#define IsFirstSelectSeason			"IsFirstSelectSeason"
#define TheNewSelectSeason			"TheNewSelectSeason"
#define BshowAd						"bShowAd"
#define FirstSuperTool				"FirstSuperTool"
#define FirstOpenUnlock				"FirstOpenUnlock"
// history的一个元素
struct HistoryValueItem
{
	ValueType valueType;	// 值
	bool bValue;
	int iValue;
	string sValue;

	void putInt(int value)
	{
		valueType = VType_INT;
		iValue = value;
	}
	void putString(string value)
	{
		valueType = VType_STRING;
		sValue = value;
	}
	void putBool(bool value)
	{
		valueType = VType_BOOL;
		bValue = value;
	}

	int getInt()
	{
		CCAssert(valueType == VType_INT,"");
		return iValue;
	}
	bool getBool()
	{
		CCAssert(valueType == VType_BOOL,"");
		return bValue;
	}
	string getString()
	{
		CCAssert(valueType == VType_STRING,"");
		return sValue;
	}
};

// history_list的一个元素
struct OwnerHistory
{
	string owner;			// 所有者
	// key到value的映射
	map<string,HistoryValueItem> ownerHistory;
};

struct OneScope
{
	int scope;
	vector<OwnerHistory> owners;
};

/**
 * 历史数据的保存文件
 * 文件可分为N个级别，通常有{SCOPE_APP,SCOPE_USERID,SCOPE_HEROID}，级别越小那么适配度就越广，级别越大，适配度就越小。
 *
 * 例如，存放如下键值对:success1_2:true
 * 作为用户[camellia],在SCOPE_APP中存放这个值，那么这个APP上的其他用户取success1_2也会取到
 * 如果scope是SCOPE_USERID，那么只能是这个用户可以取到，如果scope是SCOPE_HEROID只能是指定的英雄登陆才取的到
 *
 * 存放步奏，直接addHistory即可
 * 取值的步奏，从适配度越小的地方开始找，找到就返回，没找到就找大一点的适配度，直到找到或者不存在就返回。
 * 当然也可以直接指定只找某个scope的，或者某个适配度及其以上的
 *
 * <<<目前还不支持，如果在大的scope没找到，就去小的scope中寻找。>>>
 */
class History
{
private:
	History(void);

	// 存放的历史数据
	// 由N多scope组成
	vector<OneScope> histotys;

	static History* instance;

	bool encrypt; // 加密为true
	char m_AES_key[16];

	// 通过一个json的字符串初始化
	void initFromJsonString(string jsonDoc);
	// 保存到本地
	void save(string jsonDoc);

	string genJsonString();
public:
	static History* getHistory();

	void initData();
	void save();
protected:
	// 存放一个key-value对，指定一个scope和所有者
	// 通常用的scope有SCOPE_APP，SCOPE_USERID，SCOPE_HEROID
	// 当然也可以自己定义其他的scope，scope越小，适配度就越大，搜寻顺序就越靠后，目前scope都没有用
	// owner：所有者，scope中应该全局唯一
	void put(string key,HistoryValueItem value,string owner,int scope);
	void put(string key,int value,string owner,int scope);
	void put(string key,bool value,string owner,int scope);
	void put(string key,string value,string owner,int scope);

	// 放到APP级别,ower就是默认的OWNER_APP
	void putInApp(string key,int value);
	void putInApp(string key,bool value);
	void putInApp(string key,string value);
	// 放到用户级别，需要传入用户的id
	void putInUser(string key,int value,string userId);
	void putInUser(string key,bool value,string userId);
	void putInUser(string key,string value,string userId);

	// 存放到当前的用户里面
	void putInCurrentUser(string key,int value);
	void putInCurrentUser(string key,bool value);
	void putInCurrentUser(string key,string value);


	// 放到主英雄级别，需要传入英雄的id
	void putInHero(string key,int value,int mainHeroId);
	void putInHero(string key,bool value,int mainHeroId);
	void putInHero(string key,string value,int mainHeroId);

	// 存放到当前的英雄里面
	void putInCurrentHero(string key,int value);
	void putInCurrentHero(string key,bool value);
	void putInCurrentHero(string key,string value);

	// 获取历史数据，只寻找指定的scope里面的数据
	bool getValue(string key,string owner,int scope,HistoryValueItem& value);
	bool getInt(string key,string owner,int scope,int& value);
	bool getBool(string key,string owner,int scope,bool& value);
	bool getString(string key,string owner,int scope,string& value);

	// 从app中获取
	bool getIntFromApp(string key,int& value);
	bool getBoolFromApp(string key,bool& value);
	bool getStringFromApp(string key,string& value);

	// 从User中获取
	bool getIntFromUser(string key,string userId,int& value);
	bool getBoolFromUser(string key,string userId,bool& value);
	bool getStringFromUser(string key,string userId,string& value);

	// 根据当前登录的用户获取数据
	bool getIntFromCurrentUser(string key,int& value);
	bool getBoolFromCurrentUser(string key,bool& value);
	bool getStringFromCurrentUser(string key,string& value);

	// 从HeroId中获取
	bool getIntFromHero(string key,int heroId,int& value);
	bool getBoolFromHero(string key,int heroId,bool& value);
	bool getStringFromHero(string key,int heroId,string& value);

	// 根据当前登录的英雄获取数据
	bool getIntFromCurrentHero(string key,int& value);
	bool getBoolFromCurrentHero(string key,bool& value);
	bool getStringFromCurrentHero(string key,string& value);

public:
	//20150202 scf
	//需要接入社交平台，记录最后一次登录的哪个平台 和 该平台用户的账号id
	//const string& platform   平台， 如：Facebook、TencentQQ
	//const string& id       userID， 如：123456
	void setLastUserPlatformAndID(const string& platform, const string& id, const string& name, const string& pictureUrl);// key:PlatformInfo   value:platform,id
	bool getLastUserPlatformAndID(string& platform, string& id, string* name=NULL, string* pictureUrl=NULL);

	~History(void);
};

