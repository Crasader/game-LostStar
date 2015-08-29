#include "MarketItemView.h"
#include "JuqingDaoJuManager.h"
#include "LevelManager.h"
#include "LFStrings.h"
#include "font.h"
#include "GameInforEditer.h"
#include "effect/BaseLightEffectNode.h"
#include "SafeBoarder.h"
#include "VersionControl.h"

//////
MarketContaintLayer *MarketContaintLayer::createWithMarketState(marketState state, bool all)
{
	MarketContaintLayer *ret = NULL;
	switch (state)
	{
	case MS_SuperTools:
		ret = new MarketSuperToolsView;
		break;
	case MS_SpecialItem:
		ret = new MarketSpecialItemView;
		break;
	case MS_StrategyMap:
		ret = new MarketStrategyMapView;
		break;
	default:
		break;
	}

	if (ret)
	{
		ret->autorelease();
		ret->setBgMode(all);
		ret->fixTableViewOffset();
	}
	return ret; 
}

MarketContaintLayer::MarketContaintLayer()
{
	CCLayer::init();

	mTableView = CCTableView::create(this, CCSizeMake(920, 657-260));
	this->addChild(mTableView);
	mTableView->setPosition(ccp(0,0));
	//	mTableView->setPosition(ccp(35, 34));
	mTableView->setDelegate(this);
	mTableView->setDirection(kCCScrollViewDirectionHorizontal);


	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(mTableView);
	scheduleOnce(schedule_selector(MarketContaintLayer::scheduleUpTableViewPrioority), 0);
    
//    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_PayEndRsp);
 //   CCNotificationCenter::sharedNotificationCenter()->addObserver(this,	callfuncO_selector(MarketContaintLayer::buyRsp), MSG_PayEndRsp, NULL);
}

MarketContaintLayer::~MarketContaintLayer()
{
     CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

void MarketContaintLayer::scheduleUpTableViewPrioority(float dt)
{
#if Version_Platform != 7
	if (mTableView)
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(mTableView, BgLayerTouchPriority-1, true);
	}
#endif
}

void MarketContaintLayer::scheduleRemoveTableTouch(float dt)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(mTableView);
}

void MarketContaintLayer::fixTableViewOffset()
{
	unsigned int num = numberOfCellsInTableView(mTableView);

	CCSize size = tableCellSizeForIndex(mTableView, 0);
	CCPoint offset = mTableView->getContentOffset();

	if (mAll)
	{
		if (num <= 4)
		{
			offset.x = offset.x + size.width * (4-num)/2.0f;
			mTableView->setContentOffset(offset);
			scheduleOnce(schedule_selector(MarketContaintLayer::scheduleRemoveTableTouch), 0);
		}
	}
	else
	{
		if (num <= 2)
		{
			offset.x = offset.x + size.width * (2-num)/2.0f;
			mTableView->setContentOffset(offset);
			scheduleOnce(schedule_selector(MarketContaintLayer::scheduleRemoveTableTouch), 0);
		}
	}

}

void  MarketContaintLayer::callfuncND_ChangeMenuTouchPriority(CCNode* node, void *priority)
{
	CCMenu *menu = dynamic_cast<CCMenu*>(node);
	int pri = (int)(long long)priority;

	if (menu)
	{
		menu->setHandlerPriority(pri);
	}
}

CCTableViewCell* MarketContaintLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell *cell = table->dequeueCell();
	if (cell)
	{
		cell->removeAllChildren();
	}
	else
	{
		cell = new CCTableViewCell;
		cell->autorelease();
	}

	CCSprite *c = getTableCellContent(idx);
	if (!c)
	{
		return NULL;
	}
	cell->addChild(c);

	CCSize size = tableCellSizeForIndex(table, idx);
	c->setPosition(ccp(size.width/2, size.height/2));

	return cell;
}

void MarketContaintLayer::menuitemClicked_Buy(CCObject *pSender)
{
	//保存购买项
	int itemData = (int)(long long)(((CCMenuItem*)pSender)->getUserData());
	int tagDaojuId = ((CCMenuItem*)pSender)->getTag();

// 	//监听购买返回的消息
// 	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
// 		callfuncO_selector(MarketContaintLayer::buyRsp), MSG_PayEndRsp, NULL);
}

void MarketContaintLayer::buyRsp(CCObject *msg)
{
// 	NetworkWaitingUI::hideWaiting();
// 	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PayEndRsp);
// 
// 	SPCmd_PayEndRsp* rsp = (SPCmd_PayEndRsp*)((MSG_Rsp*)msg)->getData();
// 	if (rsp->err_code != Success)
// 	{
// 		CCMessageBox(LFStrings::getValue("shujucuowu").c_str(), LFStrings::getValue("cuowu").c_str());
// 		return;
// 	}
// 
// 	if (! rsp->status && rsp->hasSecret) //同步校验失败
// 	{
// 		CCMessageBox(LFStrings::getValue("Order_Secret_Error").c_str(), LFStrings::getValue("cuowu").c_str());
// 		return;
// 	}
// 
// 	if (! rsp->hasSecret && ! rsp->status) //sdk充值失败  可能是用户点击进入充值后 就退出了
// 	{
// 		return;
// 	}
// 
// 
// 	//成功处理    //rsp->hasSecret=false  rsp->status=true  这种情况属于ios正版  不需要my server同步
// 	if (rsp->status)
// 	{
// 		CCMessageBox(LFStrings::getValue("Order_Secret_OK").c_str(), LFStrings::getValue("Tip_Title").c_str());
// 
// 	}

}



//MS_SuperTools
MarketSuperToolsView::MarketSuperToolsView()
{
	mTableView->reloadData();
}

unsigned int MarketSuperToolsView::numberOfCellsInTableView(CCTableView *table)
{
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//	return 1;
//#else
	return 2;
//#endif
}

CCSprite * MarketSuperToolsView::getTableCellContent(int idx)
{
	CCSprite *s = CCSprite::create();
	s->initWithFile(ResManager::getManager()->getSharedFilePath(g_storelayerPath+"main_shangdian_daoju_bg.png").c_str());

	{
		//太阳镜     /      未获得
		CCString temp;
		string menustr1, menustr2;
		int menuItemData = BuyItem::none;   //购买按钮 usedata  记录当前准备买什么, 

		if (idx == 0)
		{
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//			temp = LFStrings::getValue("shijianjinzhi") + " *8";
//			menustr1 = "main_shangdian_anniu_6yuan.png";
//			menustr2 = "main_shangdian_anniu_6yuan_select.png";
//			menuItemData = BuyItem::itemtype_SuperTools_x8;

			temp = LFStrings::getValue("shijianjinzhi");
#if Version_Platform == 10
			menustr1 = "main_shangdian_anniu_1yuan_91.png";
			menustr2 = "main_shangdian_anniu_1yuan_select_91.png";
#else
			menustr1 = "main_shangdian_anniu_1yuan.png";
			menustr2 = "main_shangdian_anniu_1yuan_select.png";
#endif
			menuItemData = BuyItem::itemtype_SuperTools;
		}
		else
		{
			temp = LFStrings::getValue("shijianjinzhi") + " *8";
#if Version_Platform == 10
			menustr1 = "main_shangdian_anniu_6yuan_91.png";
			menustr2 = "main_shangdian_anniu_6yuan_select_91.png";
#else
			menustr1 = "main_shangdian_anniu_6rmb.png";
			menustr2 = "main_shangdian_anniu_6rmb_select.png";
#endif
			menuItemData = BuyItem::itemtype_SuperTools_x8;
		}

		CCLabelTTF* title = CCLabelTTF::create(temp.getCString(),fontStr_katong
			,32,CCSizeMake(203,0),kCCTextAlignmentCenter);
		title->setColor(fontColor_Store);
		s->addChild(title);
		title->setPosition(ccp(101, 337));

		//tool
		CCSprite *item = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_daojuPath+"daoju_gongluetu_chaonengli.png").c_str());
		s->addChild(item);
		item->setPosition(ccp(101, 377-166));
		CCSprite *mask = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_storelayerPath+"main_shangdian_daoju_fugai.png").c_str());
		s->addChild(mask);
		mask->setPosition(ccp(101, 377-151));
		{
			float delay = 0.2f;
			for (int i=0;i<5;i++)
			{
				CCSprite* wave = CCSprite::create(ResManager::getManager()->getSharedFilePath("superpower.png").c_str());
				item->addChild(wave,0);
				wave->setPosition(ccp(item->getContentSize().width/2,item->getContentSize().height/2));
				wave->setScale(0.0f);

				// 整体变透明
				wave->setOpacity(200);

				// 旋转
				wave->runAction(CCSequence::create(CCDelayTime::create(delay*i),CCRepeat::create(CCRotateBy::create(3.0f,360),-1),NULL));
				wave->runAction(CCSequence::create(
					CCDelayTime::create(delay*i),

					CCRepeat::create(
					CCSequence::create(
					CCSpawn::create(
					CCFadeTo::create(0.8f,0),
					CCScaleTo::create(1.0f,1.0f),NULL),
					CCSpawn::create(CCScaleTo::create(0.05f, 0), CCFadeTo::create(0.01f, 200), NULL),NULL)
					,-1),

					NULL
					));
			}
		}



		//direction
		CCLabelTTF *direction = CCLabelTTF::create(LFStrings::getValue("SuperToolDirection").c_str(),fontStr_katong
			,20,CCSizeMake(180,0),kCCTextAlignmentLeft);
		direction->setColor(ccWHITE);
		s->addChild(direction);
		direction->setAnchorPoint(ccp(0.5,1));
		direction->setPosition(ccp(101, 377-271));

		//menu
		CCMenu *menu = CCMenu::create();
		s->addChild(menu);
		menu->setPosition(ccp(0,0));
		menu->runAction(CCCallFuncND::create(this, callfuncND_selector(MarketContaintLayer::callfuncND_ChangeMenuTouchPriority)
			, (void*)(BgLayerTouchPriority - 2)));
		CCMenuItemImage *buy = CCMenuItemImage::create(
			ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+menustr1).c_str(),
			ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+menustr2).c_str(),
			this, menu_selector(MarketContaintLayer::menuitemClicked_Buy));
		menu->addChild(buy);
		buy->setPosition(ccp(101, 20));
		buy->setUserData((void*)menuItemData);
	}

	return s;
}

void MarketSuperToolsView::refreshlayer_buysuccess(CCObject *msg)
{
	unsigned int count = numberOfCellsInTableView(mTableView);
	for (unsigned int i=0; i<count; ++i)
	{
		mTableView->updateCellAtIndex(i);
	}
}

//MS_SpecialItem
MarketSpecialItemView::MarketSpecialItemView()
{
	int seasonId = GameInforEditer::getGameInfor()->getLastPlayedSeasonId();
	//屏蔽剧情道具相关
//	mItems = JuqingDaoJuManager::getManager()->getAllItemsInSeason(seasonId);

	mTableView->reloadData();
}

unsigned int MarketSpecialItemView::numberOfCellsInTableView(CCTableView *table)
{
//	return mItems.size();
	return 0;
}

CCSprite * MarketSpecialItemView::getTableCellContent(int idx)
{
	return NULL;
	//屏蔽剧情道具相关
/*	JuqingDaoJu oneDaoju = mItems.at(idx);

	CCString temp;
	if (oneDaoju.state == DJS_NotGetted)
		temp = "main_shangdian_daoju_bg_lock.png";
	else 
		temp = "main_shangdian_daoju_bg.png";
	BaseLightEffectNode *s = BaseLightEffectNode::create();
	s->initWithFile(ResManager::getManager()->getSharedFilePath(g_storelayerPath+temp.getCString()).c_str());


	{
		//太阳镜     /      未获得
		CCString temp;
		string menustr1, menustr2;
		int menuItemData = BuyItem::none;   //购买按钮 usedata  记录当前准备买什么
		int tagDaojuId = 0;  //

		{
			if (oneDaoju.state == DJS_NotGetted)
				temp = LFStrings::getValue("weihuode");
			else 
				temp = oneDaoju.daojuName;
#if Version_Platform == 10
			menustr1 = "main_shangdian_anniu_1yuan_91.png";
			menustr2 = "main_shangdian_anniu_1yuan_select_91.png";
#else
			menustr1 = "main_shangdian_anniu_1yuan.png";
			menustr2 = "main_shangdian_anniu_1yuan_select.png";
#endif
			menuItemData = BuyItem::itemtype_SpecialItem;
			tagDaojuId = oneDaoju.daojuId;
		}

		CCLabelTTF* title = CCLabelTTF::create(temp.getCString(),fontStr_katong
			,32,CCSizeMake(203,0),kCCTextAlignmentCenter);
		title->setColor(fontColor_Store);
		s->addChild(title);
		title->setPosition(ccp(101, 337));

		//tool
		CCSprite *item = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_daojuPath+oneDaoju.fileName).c_str());
		s->addChild(item);
		item->setPosition(ccp(101, 377-166));
		item->setScale(0.75f);
		CCSprite *mask = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_storelayerPath+"main_shangdian_daoju_fugai.png").c_str());
		s->addChild(mask);
		mask->setPosition(ccp(101, 377-151));

		//direction
		CCLabelTTF *direction = CCLabelTTF::create(oneDaoju.describe.c_str(),fontStr_katong
			,20,CCSizeMake(180,0),kCCTextAlignmentLeft);
		direction->setColor(ccWHITE);
		s->addChild(direction);
		direction->setAnchorPoint(ccp(0.5,1));
		direction->setPosition(ccp(101, 377-271));

		if (oneDaoju.state == DJS_NotGetted)
		{
			//menu
			CCMenu *menu = CCMenu::create();
			s->addChild(menu);
			menu->setPosition(ccp(0,0));
			menu->runAction(CCCallFuncND::create(this, callfuncND_selector(MarketContaintLayer::callfuncND_ChangeMenuTouchPriority)
				, (void*)(BgLayerTouchPriority - 2)));
			CCMenuItemImage *buy = CCMenuItemImage::create(
				ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+menustr1).c_str(),
				ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+menustr2).c_str(),
				this, menu_selector(MarketContaintLayer::menuitemClicked_Buy));
			menu->addChild(buy, 0, tagDaojuId);
			buy->setPosition(ccp(101, 20));
			buy->setUserData((void*)menuItemData);
		}
		else
		{
			CCLabelTTF *direction = CCLabelTTF::create(LFStrings::getValue("yiyongyou").c_str(),fontStr_katong
				,24,CCSizeMake(180,0),kCCTextAlignmentCenter);
			direction->setColor(ccWHITE);
			s->addChild(direction);
			direction->setAnchorPoint(ccp(0.5,0));
			direction->setPosition(ccp(101, 20));
		}
	}

	return s;
*/
}

void MarketSpecialItemView::refreshlayer_buysuccess(CCObject *msg)
{
	int seasonId = GameInforEditer::getGameInfor()->getLastPlayedSeasonId();
//	mItems = JuqingDaoJuManager::getManager()->getAllItemsInSeason(seasonId);
	unsigned int count = numberOfCellsInTableView(mTableView);
	for (unsigned int i=0; i<count; ++i)
	{
		mTableView->updateCellAtIndex(i);
	}
}

//MS_StrategyMap
MarketStrategyMapView::MarketStrategyMapView()
{
	mTableView->reloadData();
}

void MarketStrategyMapView::refreshlayer_buysuccess(CCObject *msg)
{
	unsigned int count = numberOfCellsInTableView(mTableView);
	for (unsigned int i=0; i<count; ++i)
	{
		mTableView->updateCellAtIndex(i);
	}
}

unsigned int MarketStrategyMapView::numberOfCellsInTableView(CCTableView *table)
{
	return 1;
}

CCSprite * MarketStrategyMapView::getTableCellContent(int idx)
{
	BaseLightEffectNode *s = BaseLightEffectNode::create();
	s->initWithFile(ResManager::getManager()->getSharedFilePath(g_storelayerPath+"main_shangdian_daoju_bg.png").c_str());

	{
		//太阳镜     /      未获得
		CCString temp;
		string menustr1, menustr2;
		int menuItemData = BuyItem::none;   //购买按钮 usedata  记录当前准备买什么

		{
			temp = LFStrings::getValue("gongluetu");
#if Version_Platform == 10
			menustr1 = "main_shangdian_anniu_6yuan_91.png";
			menustr2 = "main_shangdian_anniu_6yuan_select_91.png";
#else
			menustr1 = "main_shangdian_anniu_6rmb.png";
			menustr2 = "main_shangdian_anniu_6rmb_select.png";
#endif
			menuItemData = BuyItem::itemtype_StrategyMap;
		}

		CCLabelTTF* title = CCLabelTTF::create(temp.getCString(),fontStr_katong
			,32,CCSizeMake(203,0),kCCTextAlignmentCenter);
		title->setColor(fontColor_Store);
		s->addChild(title);
		title->setPosition(ccp(101, 337));

		//tool
		CCSprite *item = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_daojuPath+"daoju_gongluetu.png").c_str());
		s->addChild(item);
		item->setPosition(ccp(101, 377-166));
		item->setScale(0.75f);
		CCSprite *mask = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_storelayerPath+"main_shangdian_daoju_fugai.png").c_str());
		s->addChild(mask);
		mask->setPosition(ccp(101, 377-151));

		//direction
		CCLabelTTF *direction = CCLabelTTF::create(LFStrings::getValue("StrategyMapDirection").c_str(),fontStr_katong
			,20,CCSizeMake(180,0),kCCTextAlignmentLeft);
		direction->setColor(ccWHITE);
		s->addChild(direction);
		direction->setAnchorPoint(ccp(0.5,1));
		direction->setPosition(ccp(101, 377-271));

		if (GameInforEditer::getGameInfor()->getHelpTimeLeft() <= 0)
		{
			//menu
			CCMenu *menu = CCMenu::create();
			s->addChild(menu);
			menu->setPosition(ccp(0,0));
			menu->runAction(CCCallFuncND::create(this, callfuncND_selector(MarketContaintLayer::callfuncND_ChangeMenuTouchPriority)
				, (void*)(BgLayerTouchPriority - 2)));
			CCMenuItemImage *buy = CCMenuItemImage::create(
				ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+menustr1).c_str(),
				ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+menustr2).c_str(),
				this, menu_selector(MarketContaintLayer::menuitemClicked_Buy));
			menu->addChild(buy);
			buy->setPosition(ccp(101, 20));
			buy->setUserData((void*)menuItemData);
		}
		else
		{
			CCLabelTTF *direction = CCLabelTTF::create(LFStrings::getValue("yiyongyou").c_str(),fontStr_katong
				,24,CCSizeMake(180,0),kCCTextAlignmentCenter);
			direction->setColor(ccWHITE);
			s->addChild(direction);
			direction->setAnchorPoint(ccp(0.5,0));
			direction->setPosition(ccp(101, 20));
		}
	}

	return s;
}