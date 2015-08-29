#pragma once

#include "cocos2d.h"
#include <string>
#include "JuqingDaoJuManager.h"

using std::string;
using namespace cocos2d;



class BuyItem
{
public:
	enum ItemType //每一个购买项
	{
		none                = 0,
		itemtype_SuperTools ,  //超能力道具
		itemtype_SuperTools_x8,   //超能力道具*8
		itemtype_SpecialItem,     //特殊道具
		itemtype_StrategyMap,	  //攻略图
		itemtype_UnLockSeason,	  //解锁关卡

		//IOS专用。  ios只能购买星星
		itemtype_Star_6,    
		itemtype_Star_12, 
		itemtype_Star_18, 
		itemtype_Star_25, 
		itemtype_Star_30,   
	};
	static int getPriceBuyItemType(ItemType type)
	{
		switch (type)
		{
		case BuyItem::itemtype_SuperTools:
		case BuyItem::itemtype_SpecialItem:
			return 1;
			break;
		case BuyItem::itemtype_SuperTools_x8:
		case BuyItem::itemtype_StrategyMap:
		case BuyItem::itemtype_UnLockSeason:
			return 6;
			break;

		case BuyItem::itemtype_Star_6:
			return 6;
			break;
		case BuyItem::itemtype_Star_12:
			return 12;
			break;
		case BuyItem::itemtype_Star_18:
			return 18;
			break;
		case BuyItem::itemtype_Star_25:
			return 25;
			break;
		case BuyItem::itemtype_Star_30:
			return 30;
			break;
		default:
			CCAssert(false, "");
			return 0;
			break;
		}
	}
	static string getInfoBuyItemType(ItemType type)
	{
		switch (type)
		{
		case BuyItem::itemtype_SuperTools:
			return "itemtype_SuperTools";
			break;
		case BuyItem::itemtype_SuperTools_x8:
			return "itemtype_SuperTools_x8";
			break;
		case BuyItem::itemtype_SpecialItem:
			return "itemtype_SpecialItem";
			break;
		case BuyItem::itemtype_StrategyMap:
			return "itemtype_StrategyMap";
			break;
		case BuyItem::itemtype_UnLockSeason:
			return "itemtype_UnLockSeason";
			break;
		case BuyItem::itemtype_Star_6:
			return "itemtype_Star_6";
			break;
		case BuyItem::itemtype_Star_12:
			return "itemtype_Star_12";
			break;
		case BuyItem::itemtype_Star_18:
			return "itemtype_Star_18";
			break;
		case BuyItem::itemtype_Star_25:
			return "itemtype_Star_25";
			break;
		case BuyItem::itemtype_Star_30:
			return "itemtype_Star_30";
			break;
		default:
			CCAssert(false, "");
			return "";
			break;
		}
	}
    
    static string getInfoBuyItem(BuyItem item)
	{
		switch (item.item)
		{
            case BuyItem::itemtype_SuperTools:
                return "itemtype_SuperTools";
                break;
            case BuyItem::itemtype_SuperTools_x8:
                return "itemtype_SuperTools_x8";
                break;
			case BuyItem::itemtype_UnLockSeason:
				return "itemtype_UnLockSeason";
				break;
            case BuyItem::itemtype_SpecialItem:
                return JuqingDaoJuManager::getManager()->getDaoJuById(item.daojuId).daojuName;
 //               return "itemtype_SpecialItem";
                break;
            case BuyItem::itemtype_StrategyMap:
                return "itemtype_StrategyMap";
                break;
            case BuyItem::itemtype_Star_6:
                return "itemtype_Star_6";
                break;
            case BuyItem::itemtype_Star_12:
                return "itemtype_Star_12";
                break;
            case BuyItem::itemtype_Star_18:
                return "itemtype_Star_18";
                break;
            case BuyItem::itemtype_Star_25:
                return "itemtype_Star_25";
                break;
            case BuyItem::itemtype_Star_30:
                return "itemtype_Star_30";
                break;
            default:
                CCAssert(false, "");
                return "";
                break;
		}
	}


	BuyItem()
	{
		item = none;
		daojuId = 0;
	}
	//data 
	ItemType item;
	int daojuId;  //只有item 为itemtype_SpecialItem才有效
};
