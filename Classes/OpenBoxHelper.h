#pragma once

#include <vector>
#include "MarketItemDefine.h"

using std::vector;

enum OpenBoxComeFrom
{
	from_MarketStore = 0,
	from_GameInSuperTool,
	from_GameInHelpMap,
	from_SeasonSelector,

	from_ClickMainBtn,
	from_ExitGame,
};

class OpenBoxHelper
{
private:
	OpenBoxHelper(){}
	BuyItem getOneRandomItem();
	BuyItem getOneNotHaveItem();
	//第一次在攻略图处抽奖、会送该关卡的攻略图
	vector<BuyItem> makeRandomBoxData(int count, OpenBoxComeFrom from);

	static OpenBoxHelper * _helper;

	vector<BuyItem> _itemVec;
public:
	static OpenBoxHelper* getHelper();
	void startMake(int count, OpenBoxComeFrom from);

	//获取这次抽宝箱的数据、 返回值请自己保存，不要多次调用。
	vector<BuyItem> getBoxData();  //
};