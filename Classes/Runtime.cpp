#include "Runtime.h"
#include "Cocos2d.h"
#include "GameLayerFactory.h"	//关卡配置数据
#include "GameInforEditer.h"	//关卡成绩数据
#include "MenuLayer.h"
#include "GameInforEditer.h"
#include "LevelManager.h"
#include "BaseScense.h"
#include "BaseContentLayer.h"
#include "BaseContentWrapLayer.h"

using namespace cocos2d;

Runtime* Runtime::instance;

Runtime::Runtime()
{
	clearData();
}

Runtime* Runtime::getRuntime()
{
	if (instance == NULL)
	{
		instance = new Runtime();
	}
	return instance;
}

void Runtime::clearData()
{
    effectiveClickNum = 0;
	clickNum = 0;
	canTiming = true;
	usedItemNum = 0;
	usedKeyItemNum = 0;
	timeCost = 0;
	isSuccessedOnKeyTable = false;
	effectiveGameScore = false;
	isUsedSuperTool = false;
}

void Runtime::click()
{
	clickNum++;
}
void Runtime::effectiveClick()
{
	effectiveClickNum++;
	//刷新评价栏星星
	if (StarState* pLayer = (StarState*)BaseScense::getCurrentScene()->getBaseContentWrapLayer()->getChildByTag(Tag_StarStateBar))
	{
		pLayer->onClick(effectiveClickNum);
	}
	
	// 有效点击之后，才开始计时
	canTiming = true;
}

GameScore Runtime::getGameScore()
{
	CCAssert(effectiveGameScore == true,"缓存中数据还不能用！需要先调用genGameScore生成缓存数据");
	return mGameScoreBuffer;
}
// 这里就是生成策略
// 生成当前游戏获得的数据
// 如果已经获得了勋章，这里就不会再记录勋章了
//GameScore Runtime::genGameScore()
//{
//	CCAssert(effectiveGameScore == false,"缓存中有有效数据，不能再用这个方法调用来获取数据了，使用getGameScore");
//	GameLayer* layer = GameLayerFactory::getInstance()->getGameLayer();
//	SectionInfor* section = GameInforEditer::getGameInfor()->getSectionInfor(GameLayerFactory::getInstance()->getLastCreateSeasonId(),GameLayerFactory::getInstance()->getLastCreateSectionId());
//	GameScore score;
//
//	score.clickedNum = clickNum;
//	score.effectiveClickedNum = effectiveClickNum;
//	score.usedItemNum = usedItemNum;
//	score.usedKeyItemNum = usedKeyItemNum;
//	score.timeCost = timeCost;
//	score.isSuccessedOnKeyTable = isSuccessedOnKeyTable;
//	score.isUsedSuperTool = isUsedSuperTool;
//	if (timeCost <= layer->min_time_cost)
//		score.timeStar = 1;
//	//点击次数是代表：如果点击次数小于等于这么多，就给满分
//	if (usedItemNum <= layer->min_click_num)
//	{
//		score.clickStar = 2;
//	}
//	else
//	{
//		score.clickStar = 1;
//	}
//
//	// 用了超能道具，没有星星,给一颗
//	if (isUsedSuperTool)
//	{
//		score.timeStar = 0;
//		score.clickStar = 1; 
//	}
//
//	if (isSuccessedOnKeyTable &&
//		(section == NULL || section->keyTableMedalNum < 1)		// 游戏记录中，还没有获得这个勋章，那么就获得一次
//		)
//	{//在关键台面上成功，获得1个勋章
//		score.keyTableMedalNum = 1;
//	}
//	// 本关有关键元素,而且没有使用关键元素，那么就给予一个勋章
//	if (layer->getKeyItemNum() > 0 && usedKeyItemNum <= 0
//		&& (section == NULL || section->keyItemMedalNum < 1)	// 游戏记录中，还没有获得这个勋章，就获得一次。
//		)
//	{
//		score.keyItemMedalNum = 1;
//	}
//
//	// 如果这次的数据比记录的好，那么就标记好成绩
//	if (section!= NULL && section->isSuccessed == true && section->score < score.getStarNum() )
//	{
//		score.isABetterScore = true;
//	}else
//	{
//		score.isABetterScore = false;
//	}
//
//	score.log();
//	//缓存起来
//	mGameScoreBuffer = score;
//	effectiveGameScore = true;
//
//	return score;
//}


GameScore Runtime::genGameScore()
{
	CCAssert(effectiveGameScore == false,"缓存中有有效数据，不能再用这个方法调用来获取数据了，使用getGameScore");
	GameLayerInfo* layer = GameLayerFactory::getInstance()->getGameLayer();
	SectionInfor* section = GameInforEditer::getGameInfor()->getSectionInfor(GameLayerFactory::getInstance()->getLastCreateSeasonId(),GameLayerFactory::getInstance()->getLastCreateSectionId());
	GameScore score;

	score.clickedNum = clickNum;
	score.effectiveClickedNum = effectiveClickNum;
	score.usedItemNum = usedItemNum;
	score.usedKeyItemNum = usedKeyItemNum;
	score.timeCost = timeCost;
	score.isSuccessedOnKeyTable = isSuccessedOnKeyTable;
	score.isUsedSuperTool = isUsedSuperTool;
	score.timeStar = 0; // 不再关心时间

//	if (usedItemNum <= layer->min_click_num)
	{
		score.clickStar = 1;
	}
//	else
	{
		// 目前占时每多2个，就少一个星星
//		int sorce_left =  3.0f - ((float)(usedItemNum - layer->min_click_num))/2.0f;
//		score.clickStar = MAX(0,sorce_left);
		//二星三星数配置
		const ConfigSectionInfo* levelinfo = GameLayerFactory::getInstance()->getConfigSectionInfo(LevelManager::getManager()->getLastSeasonId(), LevelManager::getManager()->getLastSectionId());
		//得到三星评价

		if (score.effectiveClickedNum <= levelinfo->iPefect_Gold && score.effectiveClickedNum >= 1)
		{
			score.clickStar = 3;
		}
		else if (score.effectiveClickedNum <= levelinfo->iBest_Gold && score.effectiveClickedNum > levelinfo->iPefect_Gold)
		{
			score.clickStar = 2;
		}
	}

	// 用了超能道具，没有星星,给一颗
	if (isUsedSuperTool)
	{
		score.timeStar = 0;
		score.clickStar = 1; 
	}

	if (isSuccessedOnKeyTable &&
		(section == NULL || section->keyTableMedalNum < 1)		// 游戏记录中，还没有获得这个勋章，那么就获得一次
		)
	{//在关键台面上成功，获得1个勋章
		score.keyTableMedalNum = 1;
	}
	// 本关有关键元素,而且没有使用关键元素，那么就给予一个勋章
	if (layer->getKeyItemNum() > 0 && usedKeyItemNum <= 0
		&& (section == NULL || section->keyItemMedalNum < 1)	// 游戏记录中，还没有获得这个勋章，就获得一次。
		)
	{
		score.keyItemMedalNum = 1;
	}

	// 如果这次的数据比记录的好，那么就标记好成绩
	if (section!= NULL && section->isSuccessed == true && section->iStarNum < score.getStarNum() )
	{
		score.isABetterScore = true;
	}else
	{
		score.isABetterScore = false;
	}

	score.log();
	//缓存起来
	mGameScoreBuffer = score;
	effectiveGameScore = true;

	return score;
}


void Runtime::gameLoop(float time)
{
	if (canTiming)
	{
		timeCost += time;
	}
}
void Runtime::setSuccessdOnKeyTable()
{
	isSuccessedOnKeyTable = true;
}
void Runtime::setUsedSuperTool(bool used)
{
	isUsedSuperTool = used;
}

void Runtime::oneMoreItemUsed()
{
	usedItemNum ++;
}
void Runtime::oneMoreKeyItemUsed()
{
	usedKeyItemNum ++;
}