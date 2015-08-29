#include "MarketLayer.h"
#include "LevelManager.h"
#include "MarketItemView.h"
#include "LFStrings.h"
#include "font.h"
#include "GameInforEditer.h"
#include "LFAlerts.h"
#include "LFToast.h"
#include "History.h"
#include "BaseContentLayer.h"
#include "VersionControl.h"
#define Tag_MenuItem_Close 1
#define Tag_MenuItem_SuperTool 11
#define Tag_MenuItem_SpecialItem 12
#define Tag_MenuItem_StrategyMap 13
#define ad_tip_bg_tag			35
#define ad_tip_string_tag		36
//
#define Tag_MenuItem_Use 21
#define Tag_Label_Num    22222
extern int g_MarketLayerIndex;

MarketLayer::MarketLayer(marketState state, bool all)
	:BaseShakeLayer()
	,mState(state)
	, mContentLayer(NULL)
	, mAll(all)
	,m_iMenuItem(NULL)
	,m_bIsUseTeachSuperTool(false)
	, m_lMaskLayer(NULL)
{
	if (g_MarketLayerIndex != -1)
	{
		if (g_MarketLayerIndex == 0)
		{
			mState = MS_SuperTools;
		}
		else if (g_MarketLayerIndex == 1)
		{
			mState = MS_SpecialItem;
		}
		else if (g_MarketLayerIndex == 2)
		{
			mState = MS_StrategyMap;
		}
	}
	this->initUI();
}

void MarketLayer::initUI()
{
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
	this->removeAllChildren();

	BaseShakeLayer::init();
	mWinSize = CCDirector::sharedDirector()->getWinSize();

	if (mAll)
	{
		mBg = CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_bg.png").c_str());
	}
	else
	{
		mBg = CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"youxizhong_shangdian_bg.png").c_str());
	}
	addChild(mBg);
	mBg->setPosition(ccp(mWinSize.width/2, mWinSize.height/2));
	mBg->setAnchorPoint(ccp(0.5,0.5));
	//
// 	CCSprite *pauseBg = CCSprite::create("gray16.png",CCRectMake(0,0,mWinSize.width,mWinSize.height));
// 	addChild(pauseBg,-1);
// 	pauseBg->setPosition(ccp(mWinSize.width/2,mWinSize.height/2));  


	mMenu = CCMenu::create();
	mBg->addChild(mMenu);
	mMenu->setPosition(ccp(0,0));
	mMenu->setTouchPriority(BgLayerTouchPriority - 1);
	CCMenuItemImage *closeItem = CCMenuItemImage::create(
		ResManager::getManager()->getSharedFilePath(g_buttonPath+"guanbi.png").c_str(),
		ResManager::getManager()->getSharedFilePath(g_buttonPath+"guanbi_select.png").c_str(),
		this, menu_selector(MarketLayer::menuItemClicked_Close));
	mMenu->addChild(closeItem, 0, Tag_MenuItem_Close);
	closeItem->setPosition(ccp(mBg->getContentSize().width-10, mBg->getContentSize().height-60));
	ResManager* resMgr = ResManager::getManager();
	if (mAll)
	{
		{
			CCMenuItemImage *item = CCMenuItemImage::create(
				resMgr->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_chaonengdaoju.png").c_str(),
				resMgr->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_chaonengdaoju_select.png").c_str(),
				this, menu_selector(MarketLayer::menuItemClicked_item));
			mMenu->addChild(item, 0, Tag_MenuItem_SuperTool);
			item->setPosition(ccp(126, 468));
		}
		{
			CCMenuItemImage *item = CCMenuItemImage::create(
				resMgr->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_teshuwupin.png").c_str(),
				resMgr->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_teshuwupin_select.png").c_str(),
				this, menu_selector(MarketLayer::menuItemClicked_item));
			mMenu->addChild(item, 0, Tag_MenuItem_SpecialItem);
			item->setPosition(ccp(324, 468));
		}
		{
			CCMenuItemImage *item = CCMenuItemImage::create(
				resMgr->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_gongluetu.png").c_str(),
				resMgr->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_gongluetu_select.png").c_str(),
				this, menu_selector(MarketLayer::menuItemClicked_item));
			mMenu->addChild(item, 0, Tag_MenuItem_StrategyMap);
			item->setPosition(ccp(522, 468));
		}

		switch (mState)
		{
		case MS_SuperTools:
			menuItemClicked_item(mMenu->getChildByTag(Tag_MenuItem_SuperTool));
			break;
		case MS_SpecialItem:
			menuItemClicked_item(mMenu->getChildByTag(Tag_MenuItem_SpecialItem));
			break;
		case MS_StrategyMap:
			menuItemClicked_item(mMenu->getChildByTag(Tag_MenuItem_StrategyMap));
			break;
		default:
			break;
		}
	}
	else
	{
		CCString str;
		CCLabelTTF *title;
		int count;
		CCLabelTTF *numL;//MS_SuperTools
		CCMenuItemImage *useItem; //MS_SuperTools
		switch (mState)
		{
		case MS_SuperTools:
			//拥有数量
		//	count = GameInforEditer::getGameInfor()->getSuperToolCount(ST_SuperTool);
			count = 0;
			str.initWithFormat("%s: %d", LFStrings::getValue("yongyoushuliang").c_str(), count);
			numL = CCLabelTTF::create(str.getCString(), fontStr_katong, 24, CCSize(270,0), kCCTextAlignmentCenter);
			numL->setColor(fontColor_Alert);
			mBg->addChild(numL, 0, Tag_Label_Num);
			numL->setPosition(ccp(mBg->getContentSize().width/2, 530-52));
			//
			useItem = CCMenuItemImage::create(
				resMgr->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_shiyong.png").c_str(),
				resMgr->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_shiyong_select.png").c_str(),
				this, menu_selector(MarketLayer::useSuperToolClicked));
			mMenu->addChild(useItem, 0,Tag_MenuItem_Use);
			useItem->setPosition(ccp(528,474));
			if (count <= 0)
			{
				useItem->setVisible(false);
			}
			//
			str = LFStrings::getValue("IOS_SuperTool_storeTitle");
			title = CCLabelTTF::create(str.getCString(), fontStr_katong, 32, CCSize(426,0), kCCTextAlignmentCenter);

			m_iMenuItem = useItem;
			break;
		case MS_SpecialItem:
			break;
		case MS_StrategyMap:
			str = LFStrings::getValue("StrategyMap_storeTitle");
			title = CCLabelTTF::create(str.getCString(), fontStr_katong, 32, CCSize(426,0), kCCTextAlignmentCenter);
			title->setAnchorPoint(ccp(0.5, 0.75));
			break;
		default:
			break;
		}

		//title
		title->setColor(fontColor_Alert);
		mBg->addChild(title);
		title->setPosition(ccp(mBg->getContentSize().width/2, 530));

		//
		mContentLayer = MarketContaintLayer::createWithMarketState(mState, false);
		mBg->addChild(mContentLayer);
		mContentLayer->setPosition(ccp(95, 34));
	}


// 	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
// 		callfuncO_selector(MarketLayer::listener_refreshLayer_buySuccess), MSG_PayEndRsp, NULL);

}


MarketLayer::~MarketLayer()
{
	g_MarketLayerIndex = -1;
//	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PayEndRsp);
}	

void MarketLayer::setADVisible(bool e, bool bRemove /* = false*/)
{
	if (!mBg)
	{
		return;
	}
	CCNode* pnode = mBg->getChildByTag(ad_tip_string_tag);
	if (pnode && pnode->isVisible() != e)
	{
		pnode->setVisible(e);
		if (bRemove)
		{
			pnode->removeFromParent();
		}
	}
	pnode = mBg->getChildByTag(ad_tip_bg_tag);
	if (pnode && pnode->isVisible() != e)
	{
		pnode->setVisible(e);
		if (bRemove)
		{
			pnode->removeFromParent();
		}
	}
}

void MarketLayer::menuItemClicked_Close(CCObject *pSender)
{
	this->runAction(CCSequence::create(
		CCScaleTo::create(0.1f, 0),
		CCRemoveSelf::create(true),
		NULL));


	CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MarketClosed, NULL);
}

void MarketLayer::useSuperToolClicked(CCObject *obj)
{
	if (m_bIsUseTeachSuperTool)
	{
		if (m_lMaskLayer)
		{
			m_lMaskLayer->removeFromParent();
		}
		
		m_bIsUseTeachSuperTool = false;
		if (m_oTeachTarger)
		{
			(m_oTeachTarger->*m_hTeachCallBackHandler)();
		}
	}
	else
	{
		//CCSprite *spr = (CCSprite *)obj;
		//if (CCUserDefault::sharedUserDefault()->getBoolForKey(IsFirstUseSurperTool, true))
		//{
		//BaseContentLayer::shareBaseContentLayer()->runNoOpAction(1);
			CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_USE_SUPERTOOL, NULL);
			//LFToast::makeToast(LFStrings::getValue("clickScreenToUse"),ccp(spr->getPosition().x / 2 , spr->getPosition().y),2.0f);
			//CCUserDefault::sharedUserDefault()->setBoolForKey(IsFirstUseSurperTool,false);
			//CCUserDefault::sharedUserDefault()->flush();
		//}
	}
	//
	
	menuItemClicked_Close(0);
}

void MarketLayer::menuItemClicked_item(CCObject *pSender)
{
	CCMenuItemImage *item = (CCMenuItemImage*)pSender;
	int tag = item->getTag();
	if (tag == Tag_MenuItem_SuperTool)
	{
		mState = MS_SuperTools;
		g_MarketLayerIndex = 0;
	}
	else if (tag == Tag_MenuItem_SpecialItem)
	{
		mState = MS_SpecialItem;
		g_MarketLayerIndex = 1;
	}
	else if (tag == Tag_MenuItem_StrategyMap)
	{
		mState = MS_StrategyMap;
		g_MarketLayerIndex = 2;
	}

	refreshAllMenuItem();
	resetContaintLayer();
}

void MarketLayer::refreshAllMenuItem()
{
	CCMenuItemImage *item = NULL;
	ResManager* resMgr = ResManager::getManager();
	item = (CCMenuItemImage*)mMenu->getChildByTag(Tag_MenuItem_SuperTool);
	if (item)
	{
		item->initWithNormalImage(
			resMgr->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_chaonengdaoju.png").c_str(),
			resMgr->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_chaonengdaoju_select.png").c_str(),
			NULL,
			this, menu_selector(MarketLayer::menuItemClicked_item));
	}

	item = (CCMenuItemImage*)mMenu->getChildByTag(Tag_MenuItem_SpecialItem);
	if (item)
	{
		item->initWithNormalImage(
			resMgr->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_teshuwupin.png").c_str(),
			resMgr->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_teshuwupin_select.png").c_str(),
			NULL,
			this, menu_selector(MarketLayer::menuItemClicked_item));
	}

	item = (CCMenuItemImage*)mMenu->getChildByTag(Tag_MenuItem_StrategyMap);
	if (item)
	{
		item->initWithNormalImage(
			resMgr->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_gongluetu.png").c_str(),
			resMgr->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_gongluetu_select.png").c_str(),
			NULL,
			this, menu_selector(MarketLayer::menuItemClicked_item));
	}



	switch (mState)
	{
	case MS_SuperTools:
		item = (CCMenuItemImage*)mMenu->getChildByTag(Tag_MenuItem_SuperTool);
		if (item)
		{
			item->initWithNormalImage(
				resMgr->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_chaonengdaoju_select.png").c_str(),
				resMgr->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_chaonengdaoju.png").c_str(),
				NULL,
				this, menu_selector(MarketLayer::menuItemClicked_item));
		}
		break;
	case MS_SpecialItem:
		item = (CCMenuItemImage*)mMenu->getChildByTag(Tag_MenuItem_SpecialItem);
		if (item)
		{
			item->initWithNormalImage(
				resMgr->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_teshuwupin_select.png").c_str(),
				resMgr->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_teshuwupin.png").c_str(),
				NULL,
				this, menu_selector(MarketLayer::menuItemClicked_item));
		}
		break;
	case MS_StrategyMap:
		item = (CCMenuItemImage*)mMenu->getChildByTag(Tag_MenuItem_StrategyMap);
		if (item)
		{
			item->initWithNormalImage(
				resMgr->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_gongluetu_select.png").c_str(),
				resMgr->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_gongluetu.png").c_str(),
				NULL,
				this, menu_selector(MarketLayer::menuItemClicked_item));
		}
		break;
	default:
		break;
	}
}

void MarketLayer::resetContaintLayer()
{
	if (mContentLayer)
	{
		CCLayer *outLayer = mContentLayer;
		mContentLayer = NULL;
		mContentLayer = MarketContaintLayer::createWithMarketState(mState, true);
		CCSize size = mBg->getContentSize();
// 		mContentLayer->ignoreAnchorPointForPosition(false);
// 		mContentLayer->setAnchorPoint(ccp(0.5,0.5));
// 		mContentLayer->setPosition(ccp(115 + size.width / 2, 74 + size.height / 2));
		mContentLayer->setPosition(ccp(35, 34));
		mBg->addChild(mContentLayer);
		this->replaceLayer(outLayer,mContentLayer,this,SEL_CallFuncN(&MarketLayer::replaceCallBack));
	}
	else
	{
		CCSize size = mBg->getContentSize();
		mContentLayer = MarketContaintLayer::createWithMarketState(mState, true);
// 		mContentLayer->ignoreAnchorPointForPosition(false);
// 		mContentLayer->setAnchorPoint(ccp(0.5,0.5));
// 		mContentLayer->setPosition(ccp(115 + size.width / 2, 74 + size.height / 2));
		mContentLayer->setPosition(ccp(35, 34));
		mBg->addChild(mContentLayer);
	}
}

void MarketLayer::onEnter()
{
	BaseShakeLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, BgLayerTouchPriority, true);
	
}

void MarketLayer::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	BaseShakeLayer::onExit();
}

bool MarketLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void MarketLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

}

void MarketLayer::showShake()
{
	this->setScale(0.8f);
	this->runAction(CCSequence::create(
		CCFadeOut::create(0),
		CCSpawn::create(CCFadeIn::create(0.25f),
		CCScaleTo::create(0.25f,1.1f),
		NULL),
		CCScaleTo::create(0.1f,0.9f),
		CCScaleTo::create(0.1f,1.05f),
		CCScaleTo::create(0.1f,1.0f),
		NULL));
}

void MarketLayer::listener_refreshLayer_buySuccess(CCObject *msg)
{
// 	SPCmd_PayEndRsp* rsp = (SPCmd_PayEndRsp*)((MSG_Rsp*)msg)->getData();
// 	
// 	if (rsp->err_code==Success && rsp->status)
// 	{
// 		this->setADVisible(false, true);
// 		mContentLayer->refreshlayer_buysuccess(msg);
// 		if (! mAll)
// 		{
// 			size_t strategyMapCanUseNum = GameInforEditer::getGameInfor()->getHelpTimeLeft();
// //			int count = GameInforEditer::getGameInfor()->getSuperToolCount(ST_SuperTool);
// 			int count = 0;
// 			CCLabelTTF *numL;
// 			CCMenuItemImage* useItem;
// 			switch (mState)
// 			{
// 			case MS_SuperTools:
// 				numL = (CCLabelTTF*)mBg->getChildByTag(Tag_Label_Num);
// 				if (numL)
// 				{
// 					CCString str;
// 					str.initWithFormat("%s: %d", LFStrings::getValue("yongyoushuliang").c_str(), count);
// 					numL->setString(str.getCString());
// 				}
// 				useItem = (CCMenuItemImage*)mMenu->getChildByTag(Tag_MenuItem_Use);
// 				if (useItem && count > 0)
// 					useItem->setVisible(true);
// 				break;
// 			case MS_SpecialItem:
// 				break;
// 			case MS_StrategyMap:
// 				//为了解决游戏中购买攻略图成功后，不自动弹出攻略图的问题
// 				//购买攻略图成功后，先发送商店关闭消息，然后再发送攻略图购买成功消息
// 				menuItemClicked_Close(0);
// 				CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BuyHelpMapSuccess, NULL);
// 				break;
// 			default:
// 				break;
// 			}
// 			mContentLayer->refreshlayer_buysuccess(NULL);
// 		}
// 	}
// 	else
// 	{
// 		g_MarketLayerIndex = -1;
// 	}
}