#pragma once

#include <string>
#include <vector>
#include "sqlite/sqlite3.h"

using std::string;
using std::vector;

//插图信息
typedef struct tag_illustration 
{
	string fileName;   //图片文件名
// 	string name;       //插画名称
// 	string describe;   //描述信息
	int seasonId;      //章节id
}IllustrationInfo;



class UserDataManager
{
private:
	UserDataManager();
public:
	static UserDataManager* getInstance();
	static void destroy();
	void initData();
	~UserDataManager();

public:
	//获取插画集
	vector<IllustrationInfo> getIllustration();
    
    size_t getIllstrationSaveToServerData();
protected:
	string getIllustrationFileStr(int id);


public:
	//是否已经登录 c-talk
	bool isSdkLogin(){return m_isSdkLogin;}
	void setSdkLoginState(bool loged){m_isSdkLogin = loged;}

	//以获得的成就
	vector<string> getAchievements();
	//总共能获得的成就数量
	size_t getAllAchieve(){return 30;}

	//攻略图   没有就是false
	vector<int> getAllHelpMapsState();
	int getOneSectionMapState(int section1_100);
	void addOneSectionMap(int section, int addNum);

	//特殊 剧情道具
	vector<int> getSpecialItemNum();
	vector<int> getSpecialItemState();
	int getSpecialItemNum(int daojuId);
	void addSpecialItemNum(int daojuId, int addNum);
	void updateSpecialItemState(int daojuId, int state);
  
private:
	vector<int> m_helpMap;
	vector<int> m_specialItem;
	vector<int> m_specialItemState;
	sqlite3 *m_pDb;

	bool m_isSdkLogin;
};