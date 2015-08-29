#pragma once 

#include "MarketItemDefine.h"

class MarketBuyManager
{
public:
	static bool setBuyItemInfo(BuyItem *item)
	{
        //
//		if (m_hasPayButNoComeBack){
//			return false;
//		}
//		changeState(true);
		m_curBuyItem.item = item->item;
		m_curBuyItem.daojuId = item->daojuId;
		return true;
	}
	static BuyItem getBuyItemInfo()
	{
		return m_curBuyItem;
	}
	//当支付后，还没返回 不能继续支付 m_hasPayButNoComeBack为false 才可支付
//	static void changeState(bool lock)
//	{
//		m_hasPayButNoComeBack = lock;
//	}
private:
	MarketBuyManager(){m_hasPayButNoComeBack=false;}


	static BuyItem m_curBuyItem;
	static bool m_hasPayButNoComeBack;
};