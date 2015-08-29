#include "IOSMarketUseSuperToolLayer.h"
#include "LevelManager.h"
#include "MarketItemView.h"
#include "LFStrings.h"
#include "font.h"
#include "GameInforEditer.h"
#include "LFAlerts.h"
#include "History.h"
#include "LFToast.h"
#include "BaseContentLayer.h"
#define Tag_MenuItem_Close 1
#define Tag_MenuItem_SuperTool 11
#define Tag_MenuItem_SpecialItem 12
#define Tag_MenuItem_StrategyMap 13
#define daoju_bg_tag				14

//
#define Tag_MenuItem_Use 21
#define Tag_Label_Num    22222


IOSMarketUseSuperToolLayer::IOSMarketUseSuperToolLayer()
{
	CCLayer::init();
	m_bIsUseTeachSuperTool = false;
	mWinSize = CCDirector::sharedDirector()->getWinSize();

	mBg = CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"youxizhong_shangdian_bg.png").c_str());;

	addChild(mBg);
	mBg->setPosition(ccp(mWinSize.width/2, mWinSize.height/2));
	//
	CCSprite *pauseBg = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_mainlayerPath+"gray16.png").c_str(),CCRectMake(0,0,mWinSize.width,mWinSize.height));
	addChild(pauseBg,-1);
	pauseBg->setPosition(ccp(mWinSize.width/2,mWinSize.height/2));  


	mMenu = CCMenu::create();
	mBg->addChild(mMenu, 2);
	mMenu->setPosition(ccp(0, 1));
	mMenu->setTouchPriority(BgLayerTouchPriority - 1);
	CCMenuItemImage *closeItem = CCMenuItemImage::create(
		ResManager::getManager()->getSharedFilePath(g_buttonPath+"guanbi.png").c_str(),
		ResManager::getManager()->getSharedFilePath(g_buttonPath+"guanbi_select.png").c_str(),
		this, menu_selector(IOSMarketUseSuperToolLayer::menuItemClicked_Close));
	mMenu->addChild(closeItem, 0, Tag_MenuItem_Close);
	closeItem->setPosition(ccp(mBg->getContentSize().width-10, mBg->getContentSize().height-60));
		
	CCString str;
	CCLabelTTF *title;
	int count;
	CCLabelTTF *numL;//MS_SuperTools
	CCMenuItemImage *useItem; //MS_SuperTools


	//道具背景
	CCSprite* pDaojuBg = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_storelayerPath+"main_shangdian_daoju_bg.png").c_str());
	mBg->addChild(pDaojuBg, 1, daoju_bg_tag);
	pDaojuBg->setPosition(ccp(mBg->getContentSize().width/2, mBg->getContentSize().height/2 - 120));

	//标题
	str = LFStrings::getValue("IOS_SuperTool_storeTitle");
	title = CCLabelTTF::create(str.getCString(), fontStr_katong, 32, CCSize(500, 80), kCCTextAlignmentCenter);
	title->setColor(fontColor_Alert);
	mBg->addChild(title);
	title->setPosition(ccp(mBg->getContentSize().width/2, 500));


	//道具名字
	str = LFStrings::getValue("SuperTool_Name");
	CCLabelTTF* pNameTTF = CCLabelTTF::create(str.getCString(), fontStr_katong, 32, CCSize(620, 80), kCCTextAlignmentCenter);
	pDaojuBg->addChild(pNameTTF);
	pNameTTF->setColor(fontColor_Store);
	pNameTTF->setPosition(ccp(pDaojuBg->getContentSize().width/2, pDaojuBg->getContentSize().height - 60));

	//道具图标
	CCSprite* pIcon = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_daojuPath+"daoju_gongluetu_chaonengli.png").c_str());
	if (pIcon)
	{
		pIcon->setPosition(ccp(pDaojuBg->getContentSize().width/2, pDaojuBg->getContentSize().height/2+20));
		pDaojuBg->addChild(pIcon);
		pIcon->setScale(0.8f);

		{
			float delay = 0.2f;
			for (int i=0;i<5;i++)
			{
				CCSprite* wave = CCSprite::create(ResManager::getManager()->getSharedFilePath("superpower.png").c_str());
				pIcon->addChild(wave,0);
				wave->setPosition(ccp(pIcon->getContentSize().width/2,pIcon->getContentSize().height/2));
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



	}

	//拥有数量
	count = 0;
//	count = GameInforEditer::getGameInfor()->getSuperToolCount(ST_SuperTool);
	str.initWithFormat("%s: %d", LFStrings::getValue("yongyoushuliang").c_str(), count);
	numL = CCLabelTTF::create(str.getCString(), fontStr_katong, 24, CCSize(270,0), kCCTextAlignmentCenter);
	pDaojuBg->addChild(numL, 0, Tag_Label_Num);
	numL->setPosition(ccp(pDaojuBg->getContentSize().width/2, 80));
	
	//使用按钮
	useItem = CCMenuItemImage::create(
		ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_shiyong.png").c_str(),
		ResManager::getManager()->getFilePathByLanguageType(g_storelayerPath+"main_shangdian_anniu_shiyong_select.png").c_str(),
		this, menu_selector(IOSMarketUseSuperToolLayer::useSuperToolClicked));
	mMenu->addChild(useItem, 0,Tag_MenuItem_Use);
	useItem->setPosition(ccp(pDaojuBg->getPositionX(), pDaojuBg->getPositionY() - pDaojuBg->getContentSize().height/2 + useItem->getContentSize().height/2 - 10));
	if (count <= 0)
		useItem->setVisible(false);
//	if (m_bIsUseTeachSuperTool)
	{
		m_iMenuItem = useItem;
	}

// 	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
// 		callfuncO_selector(IOSMarketUseSuperToolLayer::listener_refreshLayer_buySuccess), MSG_PayEndRsp, NULL);
}

IOSMarketUseSuperToolLayer::~IOSMarketUseSuperToolLayer()
{
//	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PayEndRsp);
}	

void IOSMarketUseSuperToolLayer::menuItemClicked_Close(CCObject *pSender)
{
	mBg->runAction(CCSequence::create(
		CCScaleTo::create(0.1f, 0),
		CCCallFuncN::create(this, callfuncN_selector(IOSMarketUseSuperToolLayer::removeCurLayer)),
		NULL));

	CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MarketClosed, NULL);
}

void IOSMarketUseSuperToolLayer::removeCurLayer(CCNode* pNode)
{
	this->removeFromParent();
}

void IOSMarketUseSuperToolLayer::useSuperToolClicked(CCObject *obj)
{
	/*CCSprite *spr = (CCSprite *)obj;
	if (CCUserDefault::sharedUserDefault()->getBoolForKey(IsFirstUseSurperTool, true))
	{
		BaseContentLayer::shareBaseContentLayer()->runNoOpAction(1);
		//LFToast::makeToast(LFStrings::getValue("clickScreenToUse"),ccp(spr->getPosition().x / 2 , spr->getPosition().y),2.0f);
		CCUserDefault::sharedUserDefault()->setBoolForKey(IsFirstUseSurperTool,false);
		CCUserDefault::sharedUserDefault()->flush();
	}*/
	//
	if (m_bIsUseTeachSuperTool)
	{
		if (m_sMaskLayer)
		{
			m_sMaskLayer->removeFromParent();
		}
		m_bIsUseTeachSuperTool = false;
		(m_oTeachTarger->*m_hTeachCallBackHandler)();
	}
	else{
		CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_USE_SUPERTOOL, NULL);
	}
	
	menuItemClicked_Close(0);
}

void IOSMarketUseSuperToolLayer::menuItemClicked_item(CCObject *pSender)
{
	refreshAllMenuItem();
}

void IOSMarketUseSuperToolLayer::refreshAllMenuItem()
{
	CCMenuItemImage *item = (CCMenuItemImage*)mMenu->getChildByTag(Tag_MenuItem_SuperTool);
	if (item)
	{
		item->initWithNormalImage(
			"main_shangdian_anniu_chaonengdaoju.png",
			"main_shangdian_anniu_chaonengdaoju_select.png",
			NULL,
			this, menu_selector(IOSMarketUseSuperToolLayer::menuItemClicked_item));
	}
}

void IOSMarketUseSuperToolLayer::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, BgLayerTouchPriority, true);
}

void IOSMarketUseSuperToolLayer::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}

bool IOSMarketUseSuperToolLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void IOSMarketUseSuperToolLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

}

void IOSMarketUseSuperToolLayer::listener_refreshLayer_buySuccess(CCObject *msg)
{
// 	SPCmd_PayEndRsp* rsp = (SPCmd_PayEndRsp*)((MSG_Rsp*)msg)->getData();
// 
// 	if (rsp->err_code==Success && rsp->status)
// 	{
// 		size_t strategyMapCanUseNum = GameInforEditer::getGameInfor()->getHelpTimeLeft();
// 
// 		// change 20150122
// 		//int count = GameInforEditer::getGameInfor()->getSuperToolCount(ST_SuperTool);
// 		int count = 0;
// 		CCLabelTTF *numL;
// 		CCMenuItemImage* useItem;
// 
// 		numL = (CCLabelTTF*)mBg->getChildByTag(Tag_Label_Num);
// 		if (numL)
// 		{
// 			CCString str;
// 			str.initWithFormat("%s: %d", LFStrings::getValue("yongyoushuliang").c_str(), count);
// 			numL->setString(str.getCString());
// 		}
// 		useItem = (CCMenuItemImage*)mMenu->getChildByTag(Tag_MenuItem_Use);
// 		if (useItem && count > 0)
// 			useItem->setVisible(true);
// 	}
}