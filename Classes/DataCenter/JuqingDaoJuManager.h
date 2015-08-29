#pragma once
#include <stdlib.h>
#include "cocos2d.h"
#include <string>
#include <vector>
#include "Box2D/Box2D.h"

using namespace cocos2d;
using namespace std;

#define ITEM_NUM_IN_EACH_SEASON	4

enum DaoJuState
{
	DJS_NotGetted = 0,		// 还没得到
	DJS_GettedByGame = 1,	// 游戏中得到了
	DJS_GettedByBuy = 2,	// 购买了
};
// 一个道具
struct JuqingDaoJu
{
	unsigned int daojuId;	// 1~20，一共20个,1~4再第一章，后面的依次排列
	unsigned int sectionId;	// 所在的某个关卡
	b2Vec2 posInGame;		// 道具在游戏中的位置
	string fileName;		// 道具图片 有白边
	string fileName_WithOutWhiteBoarder;
	string fileName_Origin;  //
	string daojuName;       // 道具名字
	string describe;        // 道具描述信息
	DaoJuState state;		// 是否已经获取到了


	JuqingDaoJu()
	{
		daojuId = 0;
		sectionId = 0;
		posInGame = b2Vec2(0,0);
		fileName = "";
		state = DJS_NotGetted;
	}
};



class JuqingDaoJuManager
{
private:

	// 严格按照Id顺序排列
	// items[0].daojuId == 1
	vector<JuqingDaoJu> items;
	bool encrypt; // 加密为true
	char m_AES_key[16];

	static JuqingDaoJuManager* mInstance;
	JuqingDaoJuManager(void);
public:
	static JuqingDaoJuManager* getManager();
	JuqingDaoJu getDaoJuById(int id);

	// 获取已经获得到的Item
	vector<JuqingDaoJu> getAllGettedItems();
	// 获取某个season下面的Item
	vector<JuqingDaoJu> getAllItemsInSeason(int seasonId);
	// 获取所有的Item
	vector<JuqingDaoJu> getAllItems();
	// 某个season的剧情道具是否已经集齐
	bool isCollectAllInSeason(int seasonId);
	// 某个season的剧情道具是否可以解锁
	bool canUnlockedInSeason(int seasonId);

	// 获取某个season中个的道具
	vector<JuqingDaoJu> getItemInSeason(int seasonId);
	// 某章节的道具，购买的个数
	int howManyItemBuyedInSeason(int seasonId);

	// 获取某个关卡配置的道具
	// 没有就返回false
	bool getItemInSection(int seasonId,int sectionId,JuqingDaoJu& outData);
	// 是否有道具配置在某个关卡
	bool isItemConfigInSection(int seasonId,int sectionId);

	// 某个关卡的数据是否已经得到
	// 调用的前提是这关有数据
	bool isItemGettedInSection(int seasonId,int sectionId);
	// 设置某一关的道具已经获取
	// 调用条件是有道具
	void setItemIsGettedState(int seasonId,int sectionId,DaoJuState stat);
	void setItemIsGettedState(int daojuId,DaoJuState stat);
	bool saveData();
	string getDaoJuInfoStr();

	// 是否某个Item属于某个章节
	bool isItemBelongToSection(size_t itemId, int seasonId);
    
    void reset();
    
    //获取服务器需要的数据，格式为“10001110000011”，其中1表示获取，0表示未获取
    size_t getSaveToServerData();
    
    void saveServerDataToFile(size_t value);

	~JuqingDaoJuManager(void);
};

inline bool JuqingDaoJuManager::isItemBelongToSection(size_t itemId, int seasonId)
{
	CCAssert(itemId >= 1 && seasonId >= 1,"");

	return ( (itemId-1) / ITEM_NUM_IN_EACH_SEASON == (seasonId-1) );
}
