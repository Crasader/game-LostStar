#ifndef __SeasonSelectorLayer_H__
#define __SeasonSelectorLayer_H__

#include "cocos2d.h"
#include "GridScrollView.h"
#include "ResManager.h"
#include <vector>
#include "SpriteHelper.h"
#include "LevelManager.h"
#include "MainFrameLayer.h"
#include "TexturePreLoad.h"
#include "GameLayerFactory.h"
#include "LFStrings.h"
#include "font.h"
#include "LFAlerts.h"
#include "LFToast.h"
#include "MarketLayer.h"
#include "SeasonSelectorLayerGameGoods.h"
#include "collectLayer.h"
#include "IOSStoreLayer.h"
#include "Debug.h"
#include "rankingLayer.h"
#include "BaseMoveUI.h"
#include "MainThingEffect.h"
#include "History.h"
#include "VersionControl.h"
using namespace std;

class SeasonSelectorLayerBackgroud : public CCLayer,public BaseMoveUI
{
private:
	vector<CCSprite*> items;
	
public:
	
	SeasonSelectorLayerBackgroud()
	{
		CCSprite* bg = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_mainlayerPath+"main_background.png").c_str());
		addChild(bg);
		bg->setScale(1382.f/1024.f);
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		bg->setPosition(ccp(winSize.width/2,winSize.height/2));

		constract_sort(GameLayerFactory::getInstance()->getConfigSeasonNum());

		moveThing();
	}

	void constract_sort(int num)
	{
		for (size_t i=0;i<items.size();i++)
		{
			items[i]->removeFromParentAndCleanup(true);
		}
		items.clear();

		float winWidth = CCDirector::sharedDirector()->getWinSize().width;
		int seasonNum = num;
		float itemDistance = 30.0f;
		float itemPosY = 60.0f;
		float startPosX = (winWidth - seasonNum * itemDistance)/2.0f;
		for (int i=0;i<seasonNum;i++)
		{
			CCSprite*  seasonMarkItem = NULL;
			if (i == 0)
				seasonMarkItem = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"season_item_selected.png").c_str());
			else
				seasonMarkItem = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"season_item_disselected.png").c_str());

			addChild(seasonMarkItem);
			seasonMarkItem->setPosition(ccp(startPosX,itemPosY));
			items.insert(items.end(),seasonMarkItem);
			startPosX += itemDistance;
		}
	}

	void setSelectedPageIndex(int index)
	{	
		for (size_t i= 0;i<items.size();i++)
		{
			if (i == index)
			{
				items[i]->initWithFile(ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"season_item_selected.png").c_str());
			}else
			{
				items[i]->initWithFile(ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"season_item_disselected.png").c_str());
			}
		}
	}	
private:
	vector<MainThingEffect*>		m_sMainThingList;
private:
	void moveThing()
	{
		int pos[8][2] = {{160,460},{222,218},{1207,313},{458,188},{680,48},{815,265},{998,172},{1241,500}};
		for (int i = 1; i < 9; i ++)
		{
			CCString *str = CCString::createWithFormat((ResManager::getManager()->getSharedFilePath()+g_mainlayerPath+"main_xiaoguo_piaofu_%d.png").c_str(),i);
			CCSprite *oneSpr = CCSprite::create(str->getCString());
			MainThingEffect *one = MainThingEffect::create();
			one->initWithTexture(oneSpr->getTexture());
			//one->setAnchorPoint(CCPointZero);
			one->setPosition(ccp(pos[i - 1][0],pos[i - 1][1]));
			addChild(one);
			one->setTag(i);
			one->setParent(this);
			m_sMainThingList.push_back(one);
		}
		for (vector<MainThingEffect*>::iterator iter = m_sMainThingList.begin();iter != m_sMainThingList.end();iter++)
		{
			(*iter)->moveEffect();
		}
	}
	void moveChangeEnd(MainThingEffect *node)
	{
		node->moveEffect();
	}
	
};

//
//
class SeasonSelectorLayerTotalScore : public CCLayer
{
private:
	CCSprite* total;
	CCSprite* star;
	CCLabelAtlas* lableTotal;
public:
	SeasonSelectorLayerTotalScore()
	{
		total = NULL;
		star  = NULL;
		lableTotal = NULL;

		CCSprite* container = CCSprite::create();
		
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		container->setPosition(ccp(winSize.width/2,winSize.height/2));
		container->setContentSize(CCSizeMake(winSize.width,winSize.height));
		addChild(container,1);

		ResManager* resMgr = ResManager::getManager();

		total = CCSprite::create(resMgr->getFilePathByLanguageType(g_seasonlayerPath+"season_amount.png").c_str());
		container->addChild(total);

		star = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+"congratulation_star_little.png").c_str());
		//star->setScale(0.4f);
		//star->setRotation(-8.0f);
		container->addChild(star);

		int score = GameInforEditer::getGameInfor()->getTotalFreeScore();
		lableTotal = CCLabelAtlas::create(
			"0123456789",
			resMgr->getSharedFilePath(g_seasonlayerPath+"season_star_amount.png").c_str(),
			30, 47,//24
			'0');
		container->addChild(lableTotal);
		setScore(score);

// 		CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
// 			SEL_CallFuncO(&SeasonSelectorLayerTotalScore::buyStarRsp),
// 			MSG_PP_BuyStarRsp,NULL);

// 		CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
// 			SEL_CallFuncO(&SeasonSelectorLayerTotalScore::payEnd),
// 			MSG_PayEndRsp, NULL);
// 
// 		CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
// 			SEL_CallFuncO(&SeasonSelectorLayerTotalScore::openBoxRsp),
// 			MSG_openBoxRsp, NULL);
	}

	~SeasonSelectorLayerTotalScore()
	{
		total = NULL;
		star  = NULL;
		lableTotal = NULL;
//		CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_PP_BuyStarRsp);
// 		CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_PayEndRsp);
// 		CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_openBoxRsp);
		CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
	}
	void buyStarRsp(CCObject *payRspWrapper)
	{
// 		PPCmd_BuyStarRsp* rsp = (PPCmd_BuyStarRsp*)(((PayRspWrapper*)payRspWrapper)->data);
// 		if (rsp->rspCode == PP_Success)
// 		{
// 			int score = GameInforEditer::getGameInfor()->getTotalFreeScore();
// 			setScore(score);
// 		}
	}

	void setScore(int score) 
	{
		if (!lableTotal || !star || !total)
		{
			return;
		}
		float dis = 10;
		char str[128];
		sprintf(str,"%d",score);

		lableTotal->setString(str);
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		star->setPosition(ccp(winSize.width - star->getContentSize().width/2*star->getScale(), 768-45));
		lableTotal->setPosition(ccp(
			star->getPositionX() - lableTotal->getContentSize().width - star->getContentSize().width/2 * star->getScale() - dis,
			star->getPositionY() - lableTotal->getContentSize().height/2));
		total->setPosition(ccp(lableTotal->getPositionX() - total->getContentSize().width/2 - dis,star->getPositionY()));

	}

	void payEnd(CCObject* pObj)
	{
		int iStarCount = GameInforEditer::getGameInfor()->getTotalFreeScore();
		this->setScore(iStarCount);
	}

	void openBoxRsp(CCObject* pObj)
	{
		int iStarCount = GameInforEditer::getGameInfor()->getTotalFreeScore();
		this->setScore(iStarCount);
	}
};
//
class SeasonSelectorLayerTitle : public CCLayer
{
private:
	CCSprite* bg;
	vector<CCSprite*> titles;
public:
	SeasonSelectorLayerTitle()
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		int seasonNum = GameLayerFactory::getInstance()->getConfigSeasonNum();
		for (int i=0;i<seasonNum;i++)
		{
			string file = ResManager::getManager()->getLanguagePathString()+g_seasonlayerPath+SpriteHelper::splicingNum_String("season_title_%d.png",i+1);
			CCSprite*  title= CCSprite::create(file.c_str());
			addChild(title,1);
			title->setPosition(ccp(winSize.width/2,680));
			titles.insert(titles.end(),title);
		}
		setSeletedSeasonId(1);
	}

	void setSeletedSeasonId(int id)
	{
		for (size_t i=0;i<titles.size();i++)
		{
			if ( (i+1) == id)
			{
				titles[i]->setVisible(true);
			}else
			{
				titles[i]->setVisible(false);
			}
		}
	}
};
class SeasonSelectorLayerMenu;
class SeasonItem;

class SeasonSelectorLayer : public GridScrollView,ClickableItemCreater
{
private:
	static SeasonSelectorLayer* instance;
	static int lastSeasonId;

	// 控制广告页是否可以翻到
	bool canGoToLeftPage;
	bool canGoToRightPage;

	CCNode* scheduleNode;

	SeasonSelectorLayer(CCSize viewPortSize,int gridNum,int gridNumInX,int gridNumInY,float gridPadding = 10.0f,CCPoint scrollViewPortOffset = CCPointZero,float pageInval = 40.0f);

	// 元素创建的回调函数
	virtual ClickableItem* createGridItem(int pageIndex,int xIndex,int yIndex);

	void tick(float time);

	virtual bool pageScrollIntercept(int pageIndex);

	//// 手势滑动的事件拦截，如果广告不可用，不能滑到广告页
	//virtual void intentToScrollToRightPage();//意图滑到右边页
	//virtual void intentToScrollToLeftPage();//意图滑到左边页

	virtual void pageSelected(int pageIndex);
	virtual void pageSelectedEnd(int pageIndex);
	void actionEnd(CCNode* node);
	void zoomOutEnd(CCNode* node);
	// 重写，完成 移动和点击的区分，对于点击的不是本页的，就翻页
	virtual void itemClicked(ClickableItem* clickedObj); 
	virtual void onTouchBegan(CCTouch *pTouch, CCEvent *pEvent,CCPoint glPoint);
	virtual void onTouchMove(CCTouch *pTouch, CCEvent *pEvent,CCPoint glPoint);	// 重载，拦截
	virtual void onTouchEnd(CCTouch *pTouch, CCEvent *pEvent,CCPoint glPoint);

	void timeToHideFrontNextItem(float time);

	//void showInterstitialAd(float t);

	SeasonSelectorLayerBackgroud* bg;
	SeasonSelectorLayerGameGoods* gamegoods;//游戏剧情道具信息显示类
	SeasonSelectorLayerTitle* title;
	SeasonSelectorLayerMenu* menu;

	int mSeasonId;

	int mTempClickedSeasonId;

	SeasonItem	*m_iSeasonItem;
	//scf
	//滑动view结束之前选中的seasonId
	//为了显示移触广告方便
	int m_seasonId_beforeSelectEnd;

	CCNode		*m_nShakeItem;

public:
	inline SeasonItem *getSeasonItem()
	{
		return m_iSeasonItem;
	}
	static SeasonSelectorLayer* createSeasonSelectorLayer(int selectedSeasonId);
	static SeasonSelectorLayer* getCurrentLayer();
	static int lastCreatedId(){return lastSeasonId;};

	void setUnlockSeasonId(int seasonId);

	void scrollToSeason(int seasonId,float time);

	void zoomIn(bool animation,bool jumpToSection = true);
	void zoomOut(bool animation);

	void nextPage(){intentToScrollToRightPage();};
	void frontPage(){intentToScrollToLeftPage();};

	SeasonItem* getCurrentSelectedSeason();

	void seasonItemCliked(int seasonId);
	void setCombinedLayers(SeasonSelectorLayerBackgroud* bg,SeasonSelectorLayerMenu* menu,SeasonSelectorLayerTitle* title,SeasonSelectorLayerGameGoods* gamegoods=NULL);
	
};
//
class SeasonSelectorLayerMenu : public CCLayer
{
private:
	MaskLayerUI		*m_sMaskLayer;
	CCMenuItemImage* nextItem;
	CCMenuItemImage* frontItem;
	CCMenuItemImage* storeItem;
	CCMenuItemImage* mCollectItem;
	CCMenu* menu;
	SeasonSelectorLayer* seasonLayer;

	CCMenu* backMenu;

	CCLabelTTF		*m_tLabelTeach;
public:
	SeasonSelectorLayerMenu()
		:m_tLabelTeach(NULL)
		,m_sMaskLayer(NULL)
	{
		seasonLayer = NULL;
		mCollectItem = NULL;
		ResManager* resMgr = ResManager::getManager();
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCMenuItemImage* backItem = CCMenuItemImage::create(
			resMgr->getSharedFilePath(g_buttonPath+"seasonItem_buttom_back.png").c_str(),
			resMgr->getSharedFilePath(g_buttonPath+"seasonItem_buttom_back_select.png").c_str(),
			this,
			SEL_MenuHandler(&SeasonSelectorLayerMenu::menuBackClick));

		nextItem = CCMenuItemImage::create(
			resMgr->getSharedFilePath(g_seasonlayerPath+"seasonItem_buttom_right.png").c_str(),
			resMgr->getSharedFilePath(g_seasonlayerPath+"seasonItem_buttom_right_select.png").c_str(),
			this,
			SEL_MenuHandler(&SeasonSelectorLayerMenu::nextPageClicked));

		frontItem = CCMenuItemImage::create(
			resMgr->getSharedFilePath(g_seasonlayerPath+"seasonItem_buttom_left.png").c_str(),
			resMgr->getSharedFilePath(g_seasonlayerPath+"seasonItem_buttom_left_select.png").c_str(),
			this,
			SEL_MenuHandler(&SeasonSelectorLayerMenu::frontPageClicked));


		menu = CCMenu::create(nextItem,frontItem,NULL);
		addChild(menu);
		menu->setPosition(CCPointZero);

		backMenu = CCMenu::create(backItem,NULL);
		addChild(backMenu);
		backMenu->setPosition(CCPointZero);


		{
			// 			storeItem = CCMenuItemImage::create(
			// 				"goumai_tanchukuang_anniu.png",
			// 				"goumai_tanchukuang_anniu_select.png",
			// 				this,SEL_MenuHandler(&SeasonSelectorLayerMenu::menuStoreClicked));
			// 			CCLabelTTF* _lable = CCLabelTTF::create(LFStrings::getValue("store").c_str(),fontStr_katong,30,CCSizeMake(340,0),kCCTextAlignmentCenter);
			// 			storeItem->addChild(_lable,0);
			// 			_lable->setAnchorPoint(ccp(0.5f,0.5f));
			// 			_lable->setPosition(ccp(storeItem->getContentSize().width/2,storeItem->getContentSize().height/2));
			//			storeItem->setPosition(ccp(storeItem->getContentSize().width/2 + 20,storeItem->getContentSize().height/2 + 20));

			//排行榜   商店  收藏
			CCMenuItemImage *rankingItem;
			GameInforEditer* info = GameInforEditer::getGameInfor();
			{
				storeItem = CCMenuItemImage::create(
					resMgr->getSharedFilePath(g_buttonPath+"main_button.png").c_str(),
					resMgr->getSharedFilePath(g_buttonPath+"main_button_select.png").c_str(),
					this,SEL_MenuHandler(&SeasonSelectorLayerMenu::menuStoreClicked));
				storeItem->setPosition(ccp(winSize.width - 58,58));
				CCSprite *storePic = CCSprite::create(resMgr->getSharedFilePath(g_buttonPath+"main_anniu_shangdian.png").c_str());
				storeItem->addChild(storePic);
				storePic->setPosition(ccp(storeItem->getContentSize().width/2,storeItem->getContentSize().height/2));

				rankingItem = CCMenuItemImage::create(
					resMgr->getSharedFilePath(g_buttonPath+"main_button.png").c_str(),
					resMgr->getSharedFilePath(g_buttonPath+"main_button_select.png").c_str(),
					this,SEL_MenuHandler(&SeasonSelectorLayerMenu::menuRankingClicked));
				rankingItem->setPosition(ccp(winSize.width - 58 - 120,58));
				CCSprite *rankingPic = CCSprite::create(resMgr->getSharedFilePath(g_buttonPath+"main_anniu_paihangbang.png").c_str());
				rankingItem->addChild(rankingPic);
				rankingPic->setPosition(ccp(rankingItem->getContentSize().width/2,rankingItem->getContentSize().height/2));

				mCollectItem = CCMenuItemImage::create(
					resMgr->getSharedFilePath(g_buttonPath+"main_button.png").c_str(),
					resMgr->getSharedFilePath(g_buttonPath+"main_button_select.png").c_str(),
					this,SEL_MenuHandler(&SeasonSelectorLayerMenu::collectClicked));
				mCollectItem->setPosition(ccp(winSize.width - 58, 58 + 120));
				CCSprite *collectPic = CCSprite::create(resMgr->getSharedFilePath(g_buttonPath+"main_anniu_daojushouji.png").c_str());
				mCollectItem->addChild(collectPic);
				collectPic->setPosition(ccp(mCollectItem->getContentSize().width/2,mCollectItem->getContentSize().height/2));
				if(info->getLastPlayedSeasonId()<2)
				{
					mCollectItem->setVisible(true);
				}
			}

			CCMenu* m = CCMenu::create(storeItem,rankingItem,mCollectItem,NULL);
			addChild(m);
			m->setPosition(CCPointZero);

		}

		backItem->setPosition(ccp(58,58));//58  700
		frontItem->setPosition(ccp(80,winSize.height/2));
		nextItem->setPosition(ccp(winSize.width-80,winSize.height/2));
	}
	void beginCollectTeach()
	{
		MaskLayerUI *layer = MaskLayerUI::create();
		layer->addCircle(ccp(CCDirector::sharedDirector()->getWinSize().width - 58,58 + 120));
		layer->ignoreAnchorPointForPosition(false);
		layer->setAnchorPoint(CCPointZero);
		layer->setPosition(CCPointZero);
		layer->setContentSize(CCDirector::sharedDirector()->getWinSize());
		addChild(layer,10);
		m_sMaskLayer = layer;
		m_tLabelTeach = CCLabelTTF::create(LFStrings::getValue("TeachFirstOpenUnlock").c_str(), fontStr_katong,30.0f);
		m_tLabelTeach->setColor(ccWHITE);
		m_tLabelTeach->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 300,58 + 150));
		addChild(m_tLabelTeach,11);
	}
	//解锁新关卡加入收藏按钮跳动
	void onJoinCollect(){
		if(mCollectItem){
			mCollectItem->setVisible(true);
			
			if (CCUserDefault::sharedUserDefault()->getBoolForKey(FirstOpenUnlock,true))
			{
				mCollectItem->runAction(CCSequence::create(CCScaleTo::create(0.1f,0.8f),CCScaleTo::create(0.08f,1.1f),CCScaleTo::create(0.08f,1.0f),CCCallFunc::create(this,SEL_CallFunc(&SeasonSelectorLayerMenu::beginCollectTeach)),NULL));
				CCUserDefault::sharedUserDefault()->setBoolForKey(FirstOpenUnlock,false);
				CCUserDefault::sharedUserDefault()->flush();
			}
			else
			{
				mCollectItem->runAction(CCSequence::create(CCScaleTo::create(0.1f,0.8f),CCScaleTo::create(0.08f,1.1f),CCScaleTo::create(0.08f,1.0f),NULL));
			}
			
		}
	}
	
	void collectClicked(CCObject* object){
		if (m_tLabelTeach)
		{
			m_tLabelTeach->removeFromParent();
			m_sMaskLayer->removeFromParent();
		}
		collectLayer* collec = collectLayer::create();
		CCDirector::sharedDirector()->getRunningScene()->addChild(collec, 200);
		collec->teachCollect();
	}

	void setSeasonlayer(SeasonSelectorLayer* layer)
	{
		seasonLayer = layer;
	}

	void setEnbale(bool enbale)
	{
		menu->setVisible(enbale);
		menu->setEnabled(enbale);
	}

	void menuBackClick(CCObject* obj)
	{
		Music::sharedMusicPlayer()->buttonClicked();
		obj = obj;
		CCScene* scene = MainFrameLayer::createScene();
		CCTransitionFade* tran = CCTransitionFade::create(0.5,scene);
		CCDirector::sharedDirector()->replaceScene(tran);
	}
	void menuStoreClicked(CCObject* obj)
	{
		Music::sharedMusicPlayer()->buttonClicked();

#if ((CC_TARGET_PLATFORM == CC_PLATFORM_IOS && Version_Platform == 0) || DEBUG_IOSMarket)
		IOSStoreLayer *layer = IOSStoreLayer::create();
		layer->setComefrom(from_MarketStore);
		CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 200);
#else
		MarketLayer *layer = new MarketLayer(MS_SpecialItem);
		CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 200, marketlayer_tag);
		layer->release();
#endif
		
	}
	void menuRankingClicked(CCObject *obj)
	{
		rankingLayer* pLayer = rankingLayer::create();
		CCDirector::sharedDirector()->getRunningScene()->addChild(pLayer, 200); 
	}

	void buySeasonAndItsSectionsRsp(CCObject* payRspWrapper);

	void nextPageClicked(CCObject* obj)
	{
		Music::sharedMusicPlayer()->buttonClicked();
		SeasonSelectorLayer::getCurrentLayer()->nextPage();
	}
	void frontPageClicked(CCObject* obj)
	{
		Music::sharedMusicPlayer()->buttonClicked();
		SeasonSelectorLayer::getCurrentLayer()->frontPage();
	}
	void setNext_FrontOpacity(GLubyte opacity)
	{
		frontItem->setOpacity(opacity);
		nextItem->setOpacity(opacity);
	}
	void setNextButtonVisible(bool visibility)
	{
		nextItem->setVisible(visibility);
		nextItem->setEnabled(visibility);
	}
	void setFrontButtonVisible(bool visibility)
	{
		frontItem->setVisible(visibility);
		frontItem->setEnabled(visibility);
	}
	CCMenuItemImage* getFrontItem(){return frontItem;};
	CCMenuItemImage* getNextItem(){return nextItem;};
};

enum BuyState
{
	BState_NoBuy = 0,
	BState_BuyQuerying,
	BState_BuyConnectError,
	BState_BuySuccess,
};

class SeasonItem : public ClickableItem
{
public:
	SeasonItem(int seasonId,bool locked,SeasonInfor* infor = NULL);
	// 设置盖在底纹Item上面的Item的透明度
	void setUpperItemOpacity(GLubyte opacity);

	virtual void setScale(float fScale);

	void setSeasonSelectorLayer(SeasonSelectorLayer* layer);

	void runUnlockAction(float delay);
	bool isloked(){return mLocked;};
	int getSeasonId(){return mSeasonId;};
public:
	void firstUnLock();
private:
	void zhangjieLightEnd(CCNode *node);
	inline void showShakeItem(float dt)
	{
		//showShake(this);
		showZhangjieLight(this);
	}
	void showShake(CCNode *node);
	void showZhangjieLight(CCNode *node);
	void nodeAndAllChildRunAction(CCNode* node, CCActionInterval* action);

private:
	int	m_nNodeCount;
	int mSeasonId;
	bool mLocked;

	CCSprite* star ;
	CCNode* lableTotal;
	CCSprite* unLock;
	CCSprite* season_lock;

	SeasonInfor* itemInfor;
	SeasonSelectorLayer* mLayer;

	CCSprite* upperLightRight;
	CCSprite* curtainRight;
	CCSprite* lowerCurtainRight;

	CCSprite* upperLightLeft;
	CCSprite* curtainLeft;
	CCSprite* lowerCurtainLeft;

	CCSprite* curtainUpperShadow;	// 剪影
	CCSprite* curtainGril;	
	CCSprite* floorShadow;	// 地面上的阴影

	//直接由ScrollView回掉
	virtual void clickedMe();
	virtual void touchMe();
	virtual void leaveMe();

	void addBuyObserver();
	void removeBuyObserver();

	void buySeasonRsp(CCObject* payRspWrapper);
	void buySeasonAndItsSectionsRsp(CCObject* payRspWrapper);

	void actionEnd(CCNode* node);

	virtual void onExit();
};

#endif