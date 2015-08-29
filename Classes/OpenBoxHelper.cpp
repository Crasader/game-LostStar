#include "OpenBoxHelper.h"
//#include "History.h"
#include "GameInforEditer.h"
#include "JuqingDaoJuManager.h"
#include "UserDataManager.h"
#include "LevelManager.h"
#include "GameLayerFactory.h"

OpenBoxHelper * OpenBoxHelper::_helper = 0;


OpenBoxHelper* OpenBoxHelper::getHelper()
{
	if (! _helper)
	{
		_helper = new OpenBoxHelper;
		srand((unsigned) time(NULL));
	}

	return _helper;
}


void OpenBoxHelper::startMake(int count, OpenBoxComeFrom from)
{
	_itemVec = makeRandomBoxData(count, from);
}

vector<BuyItem> OpenBoxHelper::getBoxData()
{
	return _itemVec;
}

vector<BuyItem> OpenBoxHelper::makeRandomBoxData(int count, OpenBoxComeFrom from)
{
	vector<BuyItem> vec;
	for (int i=0; i<count; ++i)
	{
		vec.push_back(getOneRandomItem());
	}

	// 	玩家开启宝箱数=0时，开启宝箱，必然获得一个正在进行章节的缺少的剧情道具。
	//	每关第一次从攻略图按钮进来抽宝箱，除了抽出的道具外，直接额外赠送一个本关攻略图。(每关第一次没有图来抽时)
	// 	第一次从超能道具按钮进来抽宝箱的话，必获得超能道具*1。
	// 	第一次从剧情道具收集界面进来抽宝箱，必获得一个玩家缺少的剧情道具。
	//  之后玩家再开启宝箱时，记录开启次数，每开启10次（累计）获得一个正在进行章节的缺少的剧情道具（轮巡机制只针对累计10次获得，每单次开启抽出剧情道具可重复）。
	GameInforEditer *_gameInfo = GameInforEditer::getGameInfor();
//	History *_history = History::getHistory();
	UserDataManager *_dataManager = UserDataManager::getInstance();
	int globalSectionId = (LevelManager::getManager()->getLastSeasonId()-1)*GameLayerFactory::getInstance()->getAllSectionNumBySeasonId(LevelManager::getManager()->getLastSeasonId()) + LevelManager::getManager()->getLastSectionId();

	bool val = false;

	//保存 开宝箱的个数
	_gameInfo->addOpenboxNum(count);

// 	if (from == from_GameInSuperTool && (_history->getFirstOpenBoxThing(Openbox_firstFromSuperTool, val) && val))
// 	{
// 		vec.at(0).item = BuyItem::itemtype_SuperTools;
// 		_history->putFirstOpenBoxThing(Openbox_firstFromSuperTool, false);
// 	}
// 	else 
		if (from == from_GameInHelpMap && _dataManager->getOneSectionMapState(globalSectionId) <= 0) {
		BuyItem oneItem;
		oneItem.item = BuyItem::itemtype_StrategyMap;
		oneItem.daojuId = globalSectionId; // 1-100
		vec.push_back(oneItem);
		//_history->putFirstOpenBoxThing(Openbox_firstFromHelpMap, false);  其实不需要记录
	}
	else if (from == from_MarketStore)
	{
// 		if (_history->getFirstOpenBoxThing(Openbox_firstFromMarketItem, val) && val)
// 		{
// 			vec.at(0) = getOneNotHaveItem();
// 			_history->putFirstOpenBoxThing(Openbox_firstFromMarketItem, false);
// 		}
// 		else
		{
			if (_gameInfo->getOpenboxNum() % 10 == 0)
			{
				vec.at(0) = getOneNotHaveItem();
			}
		}
	}
	else if (from == from_SeasonSelector)
	{
// 		if (_history->getFirstOpenBoxThing(Openbox_firstFromSeasonSel, val) && val)
// 		{
// 			vec.at(0) = getOneNotHaveItem();
// 			_history->putFirstOpenBoxThing(Openbox_firstFromSeasonSel, false);
// 		}
// 		else
		{
			if (_gameInfo->getOpenboxNum() % 10 == 0)
			{
				vec.at(0) = getOneNotHaveItem();
			}
		}
	}

	if (count == 10)
	{
		vec.at(0) = getOneNotHaveItem();
	}


	return vec;
}

BuyItem OpenBoxHelper::getOneRandomItem()
{
	BuyItem item;
	int totalnum = GameLayerFactory::getInstance()->getTotalSectionNum();
	float n = CCRANDOM_0_1() * totalnum + 1;  // 1-100
	int m = CCRANDOM_0_1() * totalnum; // 0- 99
// 	80%概率抽取到关卡攻略图，可抽取到所有已开放关卡的攻略图（每次抽取可重复抽到），判定抽取到攻略图后，随机选一个攻略图
// 	17%概率抽取到超能道具
// 	3%的概率抽取到剧情道具，只可抽取到正在进行章节中出现的4种剧情道具（每次抽取可重复抽到），判定抽取到剧情道具后，随机选一个剧情道具
	if (n >= 1 && n <= 80.0f)
	{
		item.item = BuyItem::itemtype_StrategyMap;
		item.daojuId = m % totalnum + 1; // 1-100
	}
	else if (n > 80.0 && n <= 97.0f)
	{
		item.item = BuyItem::itemtype_SuperTools;
	}
	else if (n > 97.0 && n <= 100.0f)
	{
		item.item = BuyItem::itemtype_SpecialItem;
		item.daojuId = m % 4 + 1; // 1-4
		int seasonId = GameInforEditer::getGameInfor()->getLastPlayedSeasonId();
		item.daojuId += (seasonId - 1) * 4;
	}
	else
	{
		item.item = BuyItem::itemtype_StrategyMap;
		item.daojuId = m % totalnum + 1; // 1-100
	}

	return item;
}

BuyItem OpenBoxHelper::getOneNotHaveItem()
{
	int seasonId = GameInforEditer::getGameInfor()->getLastPlayedSeasonId();
	vector<JuqingDaoJu> vec = JuqingDaoJuManager::getManager()->getAllItemsInSeason(seasonId);

	int index = 0;
	if (vec.size() > 0)
	{
		int icount = vec.size();
		index = rand()%icount;
	}

	BuyItem item;
	item.item = BuyItem::itemtype_SpecialItem;
	item.daojuId = vec.at(index).daojuId;

	for (size_t i=0; i<vec.size(); ++i)
	{
		if (vec.at(i).state == DJS_NotGetted)
		{
			item.item = BuyItem::itemtype_SpecialItem;
			item.daojuId = vec.at(i).daojuId;
			return item;
		}
	}

	return item;
}