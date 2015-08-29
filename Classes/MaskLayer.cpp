#include "MaskLayer.h"
#include "ResManager.h"
#include "LevelManager.h"
#include "GameInforEditer.h"
#include "SectionSelectorLayer.h"
#include "GameLayerFactory.h"
#include "JuqingDaoJuManager.h"
#include "MarketLayer.h"
#include "IOSStoreLayer.h"
#include "Debug.h"
#include "rankingLayer.h"
#include "VersionControl.h"
static MaskLayer* currentInstance;

#define Tag_MenuItem_DisableMenu 10
#define Tag_MenuItem_SpecialItem_ 21 // 21-24

MaskLayer::MaskLayer(int seasonId,int pageNum)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	ResManager* resMgr = ResManager::getManager();
	mSeasonId = seasonId;

	CCMenuItemImage* backItem = CCMenuItemImage::create(
		resMgr->getSharedFilePath(g_buttonPath+"seasonItem_buttom_back.png").c_str(),
		resMgr->getSharedFilePath(g_buttonPath+"seasonItem_buttom_back_select.png").c_str(),
		this,
		SEL_MenuHandler(&MaskLayer::menuBackClick));

	nextItem = CCMenuItemImage::create(
		resMgr->getSharedFilePath(g_seasonlayerPath+"sectionItem_buttom_right.png").c_str(),
		resMgr->getSharedFilePath(g_seasonlayerPath+"sectionItem_buttom_right_select.png").c_str(),
		this,
		SEL_MenuHandler(&MaskLayer::nextPageClicked));

	frontItem = CCMenuItemImage::create(
		resMgr->getSharedFilePath(g_seasonlayerPath+"sectionItem_buttom_left.png").c_str(),
		resMgr->getSharedFilePath(g_seasonlayerPath+"sectionItem_buttom_left_select.png").c_str(),
		this,
		SEL_MenuHandler(&MaskLayer::frontPageClicked));

	//排行榜   商店
	CCMenuItemImage* storeItem, *rankingItem;
	{
		storeItem = CCMenuItemImage::create(
			resMgr->getSharedFilePath(g_buttonPath+"main_button.png").c_str(),
			resMgr->getSharedFilePath(g_buttonPath+"main_button_select.png").c_str(),
			this,SEL_MenuHandler(&MaskLayer::storeClicked));
		storeItem->setPosition(ccp(winSize.width - 58,58));
		CCSprite *storePic = CCSprite::create(resMgr->getSharedFilePath(g_buttonPath+"main_anniu_shangdian.png").c_str());
		storeItem->addChild(storePic);
		storePic->setPosition(ccp(storeItem->getContentSize().width/2,storeItem->getContentSize().height/2));

		rankingItem = CCMenuItemImage::create(
			resMgr->getSharedFilePath(g_buttonPath+"main_button.png").c_str(),
			resMgr->getSharedFilePath(g_buttonPath+"main_button_select.png").c_str(),
			this,SEL_MenuHandler(&MaskLayer::rankingClicked));
		rankingItem->setPosition(ccp(winSize.width - 58 - 120,58));
		CCSprite *rankingPic = CCSprite::create(resMgr->getSharedFilePath(g_buttonPath+"main_anniu_paihangbang.png").c_str());
		rankingItem->addChild(rankingPic);
		rankingPic->setPosition(ccp(rankingItem->getContentSize().width/2,rankingItem->getContentSize().height/2));
        
#if Version_Platform == 9
        storeItem->setVisible(false);
        rankingItem->setPosition(ccp(winSize.width - 58,58));
#endif
	}


	CCMenu* menu = CCMenu::create(backItem,nextItem,frontItem,storeItem,rankingItem,NULL);
	addChild(menu);
	menu->setPosition(CCPointZero);

	backItem->setPosition(ccp(58,58));

	frontItem->setPosition(ccp(80,winSize.height/2));
	nextItem->setPosition(ccp(winSize.width-80,winSize.height/2));

	bar = new PageBar(pageNum);
	addChild(bar);
	bar->release();
	bar->setPosition(ccp(winSize.width/2,46));

	//显示星星的总数
	/*{
		CCSprite* star = CCSprite::create(resMgr->getSharedFilePath("sectionItem_star.png").c_str());
		addChild(star);

		int totalSectionNum = GameLayerFactory::getInstance()->getSectionNumBySeasonId(seasonId);

		CCLabelAtlas* lableTotal = CCLabelAtlas::create(
			"0123456789",
			resMgr->getSharedFilePath("season_star_amount.png").c_str(),
			30, 47,//24
			'0');
		addChild(lableTotal);
		char str[128];
		sprintf(str,"%d",totalSectionNum*3);
		lableTotal->setString(str);

		CCLabelAtlas* lableCurrentScore = CCLabelAtlas::create(
			"0123456789",
			resMgr->getSharedFilePath("season_star_amount.png").c_str(),
			30, 47,//24
			'0');
		addChild(lableCurrentScore);

		SeasonInfor* seasonInfor = GameInforEditer::getGameInfor()->getSeasonInfor(seasonId);
		if (seasonInfor!=NULL)
		{
			char str[128];
			sprintf(str,"%d",seasonInfor->score);
			lableCurrentScore->setString(str);
		}else
		{
			lableCurrentScore->setString("0");
		}

		CCSprite* line = CCSprite::create(resMgr->getSharedFilePath("season_star_num_line.png").c_str());
		addChild(line);

		float distance = 18.0f;
		star->setPosition(ccp(winSize.width-46,winSize.height-50));
		lableTotal->setPosition(ccp(star->getPositionX() - lableTotal->getContentSize().width - distance,star->getPositionY()-lableTotal->getContentSize().height/2));
		line->setPosition(ccp(lableTotal->getPositionX() - line->getContentSize().width/2,star->getPositionY()));
		lableCurrentScore->setPosition(ccp(line->getPositionX() - lableCurrentScore->getContentSize().width - distance,lableTotal->getPositionY()));
	}*/

	//屏蔽剧情道具相关
/*
	//显示获得的道具
	{
		CCSize winsize = CCDirector::sharedDirector()->getWinSize();
		CCSprite *board = CCSprite::create(resMgr->getSharedFilePath(g_seasonlayerPath+"guanqiaxuanze_daoju_shouji_2.png").c_str());
		addChild(board);
		board->setPosition(ccp(winsize.width/2,winsize.height-board->getContentSize().height/2));
		//addChild(board);
		mDJ_Menu = CCMenu::create();
		board->addChild(mDJ_Menu);
		mDJ_Menu->setPosition(ccp(0,0));
// 		CCMenuItemImage *it = CCMenuItemImage::create(
// 			resMgr->getSharedFilePath("main_share.png").c_str(),
// 			resMgr->getSharedFilePath("main_share.png").c_str(),
// 			this, menu_selector(MaskLayer::menuItemClicked_SpecialItemBar));
// 		mDJ_Menu->addChild(it, 0, Tag_MenuItem_DisableMenu);
// 		it->setPosition(ccp(282, 48));

		vector<JuqingDaoJu> itemVec = JuqingDaoJuManager::getManager()->getAllItemsInSeason(mSeasonId);
		CCSprite* daojux1 = CCSprite::create(resMgr->getSharedFilePath(g_daojuPath+"daoju_xiaoguo_1.png").c_str());
		CCSprite* daojux2 = CCSprite::create(resMgr->getSharedFilePath(g_daojuPath+"daoju_xiaoguo_2.png").c_str());
		CCSprite* daojux3 = CCSprite::create(resMgr->getSharedFilePath(g_daojuPath+"daoju_xiaoguo_3.png").c_str());
		for (int i=0; i<4 && i<itemVec.size(); ++i)
		{
			CCMenuItemImage *item = NULL;
			if (itemVec.at(i).state == DJS_NotGetted)
			{
				item = CCMenuItemImage::create(
					resMgr->getSharedFilePath(g_seasonlayerPath+"guanqiaxuanze_daoju_shouji_weizhi.png").c_str(),
					resMgr->getSharedFilePath(g_seasonlayerPath+"guanqiaxuanze_daoju_shouji_weizhi_select.png").c_str(),
					this, menu_selector(MaskLayer::menuItemClicked_SpecialItemBar));
			}
			else
			{
				item = CCMenuItemImage::create(
					resMgr->getSharedFilePath(g_seasonlayerPath+"guanqiaxuanze_daoju_shouji_yiyou.png").c_str(),
					resMgr->getSharedFilePath(g_seasonlayerPath+"guanqiaxuanze_daoju_shouji_yiyou.png").c_str(),
					this, menu_selector(MaskLayer::menuItemClicked_SpecialItemBar));
				CCSprite *img = CCSprite::create(resMgr->getSharedFilePath(g_daojuPath+itemVec.at(i).fileName).c_str());
				item->addChild(img,1);

				img->setPosition(ccp(item->getContentSize().width/2, item->getContentSize().height/2));
				img->setScale(0.45f);

// 				CCSprite* daojux1Spr = CCSprite::createWithTexture(daojux1->getTexture());
// 				daojux1Spr->setScale(1.0f/0.42f);
// 				daojux1Spr->setPosition(ccp(item->getContentSize().width/2,item->getContentSize().height/2));
// 				daojux1Spr->runAction(CCRepeatForever::create(CCSequence::create(CCRotateTo::create(2.0f,-180),CCRotateTo::create(2.0f,-360),NULL)));
// 				item->addChild(daojux1Spr);
// 				CCSprite* daojux2Spr = CCSprite::createWithTexture(daojux2->getTexture());
// 				daojux2Spr->setScale(1.0f/0.42f);
// 				daojux2Spr->setPosition(ccp(item->getContentSize().width/2,item->getContentSize().height/2));
// 				daojux2Spr->runAction(CCRepeatForever::create(CCSequence::create(CCRotateTo::create(1.5f,180),CCRotateTo::create(1.5f,360),NULL)));
// 				item->addChild(daojux2Spr);
// 				CCSprite* daojux3Spr = CCSprite::createWithTexture(daojux3->getTexture());
// 				daojux3Spr->setScale(1.0f/0.42f);
// 				daojux3Spr->setPosition(ccp(item->getContentSize().width/2,item->getContentSize().height/2));
// 				item->addChild(daojux3Spr);
				
				CCSprite *mask = CCSprite::create(resMgr->getSharedFilePath(g_seasonlayerPath+"guanqiaxuanze_daoju_shouji_yiyou_gaizi.png").c_str());
				item->addChild(mask,1);
				mask->setPosition(img->getPosition());
			}
			item->setEnabled(false);
			mDJ_Menu->addChild(item, 0, Tag_MenuItem_SpecialItem_ + i);
			item->setPosition(ccp(105+i*120, 61));
		}
	}
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
		callfuncO_selector(MaskLayer::listener_buySuccess), MSG_PayEndRsp, NULL);
*/
}

MaskLayer::~MaskLayer()
{
//	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PayEndRsp);
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

MaskLayer* MaskLayer::createLayer(int seasonId,int pageNum)
{
	MaskLayer* layer = new MaskLayer(seasonId,pageNum);
	currentInstance = layer;
	layer->autorelease();
	return  layer;
}

void MaskLayer::menuBackClick(CCObject* obj)
{
	Music::sharedMusicPlayer()->buttonClicked();
	obj = obj;
	//LevelManager::getManager()->goToSeasonUI(true);
	LevelManager::getManager()->goToSeasonUI(false);
}
void MaskLayer::menuHomeClick(CCObject* obj)
{
	Music::sharedMusicPlayer()->buttonClicked();
	obj = obj;
    CCScene* scene = MainFrameLayer::createScene();
	CCTransitionFade* tran = CCTransitionFade::create(0.5,scene);
	CCDirector::sharedDirector()->replaceScene(tran);
}


void MaskLayer::nextPageClicked(CCObject* obj)
{
	Music::sharedMusicPlayer()->buttonClicked();
	SectionSelectorLayer::getCurrentInstance()->nextPage();
}
void MaskLayer::frontPageClicked(CCObject* obj)
{
	Music::sharedMusicPlayer()->buttonClicked();
	SectionSelectorLayer::getCurrentInstance()->frontPage();
}
void MaskLayer::setNext_FrontOpacity(GLubyte opacity)
{
	frontItem->setOpacity(opacity);
	nextItem->setOpacity(opacity);
}
void MaskLayer::setPageIndex(int pageIndex)
{
	setFrontButtonVisible(true);
	setNextButtonVisible(true);
	bar->setPageIndex(pageIndex);
	if (bar->getCurrentPageIndex() <= 0)
	{
		setFrontButtonVisible(false);
	}
	if (bar->getCurrentPageIndex() >= (bar->getPageNum()-1) )
	{
		setNextButtonVisible(false);
	}
}
void MaskLayer::setNextButtonVisible(bool visibility)
{
	nextItem->setVisible(visibility);
	nextItem->setEnabled(visibility);
}
void MaskLayer::setFrontButtonVisible(bool visibility)
{
	frontItem->setVisible(visibility);
	frontItem->setEnabled(visibility);
}

MaskLayer* MaskLayer::getCurrentInsstance()
{
	return currentInstance;
}

void MaskLayer::menuItemClicked_SpecialItemBar(CCObject *pSender)
{
	/*
	CCMenuItem *item = (CCMenuItem*)pSender;
	int tag = ((CCMenuItemImage*)pSender)->getTag();
	CCMenu *menu = (CCMenu*)((CCMenuItem*)pSender)->getParent();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	if (tag == Tag_MenuItem_DisableMenu)
	{
#define time 0.5f
		if (menu && menu->getParent())
		{
			CCSprite *board = (CCSprite*)menu->getParent();
			CCPoint pos = board->getPosition();
			/*if (pos.y < winSize.height-100)  //展开的   70
			{
				pos.y = winSize.height-70;
				item->runAction(CCSequence::create(
					CCSpawn::create(CCRotateBy::create(time, -180), CCMoveBy::create(time, ccp(0, 6)), NULL),
					CCCallFuncN::create(this, callfuncN_selector(MaskLayer::callfuncN_menuScrollEnd)),
					NULL));
			}
			else
			{
				pos.y = winSize.height-board->getContentSize().height;
				item->runAction(CCSequence::create(
					CCSpawn::create(CCRotateBy::create(time, 180), CCMoveBy::create(time, ccp(0, -6)), NULL),
					CCCallFuncN::create(this, callfuncN_selector(MaskLayer::callfuncN_menuScrollEnd)),
					NULL));
			}
			//board->runAction(CCMoveTo::create(time, pos));
			//menu->setEnabled(false);
		}
	}
	else if (tag >= Tag_MenuItem_SpecialItem_ && tag < Tag_MenuItem_SpecialItem_+4)
	{
// 		MarketLayer *layer = new MarketLayer(MS_SpecialItem);
// 		addChild(layer, 1024);
// 		layer->release();
	}*/
}

void MaskLayer::callfuncN_menuScrollEnd(CCNode *node)
{
	CCMenuItem *item = dynamic_cast<CCMenuItem*>(node);
	if (item && item->getParent()) 
	{
		CCMenu *menu = (CCMenu*)item->getParent();
		menu->setEnabled(true);
	}
}


void MaskLayer::storeClicked(CCObject *obj)
{
#if ((CC_TARGET_PLATFORM == CC_PLATFORM_IOS && Version_Platform == 0) || DEBUG_IOSMarket)
	IOSStoreLayer *layer = IOSStoreLayer::create();
	CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 200);
#else
	MarketLayer *layer = new MarketLayer(MS_SpecialItem);
	CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 200, marketlayer_tag);
	layer->release();
#endif
}

void MaskLayer::listener_buySuccess(CCObject *msg)
{
// 	SPCmd_PayEndRsp* rsp = (SPCmd_PayEndRsp*)((MSG_Rsp*)msg)->getData();
// 	//收到该消息的时候，数据已经更新了
// 	if (rsp->err_code==Success && rsp->status)
// 	{
// //		refreshSpecialItem();
// 	}
}

void MaskLayer::refreshSpecialItem()
{
	//屏蔽剧情道具相关
/*
	ResManager* resMgr = ResManager::getManager();
	vector<JuqingDaoJu> itemVec = JuqingDaoJuManager::getManager()->getAllItemsInSeason(mSeasonId);
	for (int i=0; i<4 && i<itemVec.size(); ++i)
	{
		CCMenuItemImage *item = (CCMenuItemImage*)mDJ_Menu->getChildByTag(Tag_MenuItem_SpecialItem_ + i);
		if (itemVec.at(i).state != DJS_NotGetted)
		{
			item->setNormalImage(CCSprite::create(resMgr->getSharedFilePath(g_seasonlayerPath+"guanqiaxuanze_daoju_shouji_yiyou.png").c_str()));
			item->setSelectedImage(CCSprite::create(resMgr->getSharedFilePath(g_seasonlayerPath+"guanqiaxuanze_daoju_shouji_yiyou.png").c_str()));
			item->setEnabled(false);
			CCSprite *img = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_daojuPath+itemVec.at(i).fileName).c_str());
			item->addChild(img);
			img->setPosition(ccp(item->getContentSize().width/2, item->getContentSize().height/2));
			img->setScale(0.45f);
			CCSprite *mask = CCSprite::create(resMgr->getSharedFilePath(g_seasonlayerPath+"guanqiaxuanze_daoju_shouji_yiyou_gaizi.png").c_str());
			item->addChild(mask);
			mask->setPosition(img->getPosition());
		}
	}
*/
}

void MaskLayer::rankingClicked(CCObject *obj)
{
	rankingLayer* pLayer = rankingLayer::create();
	CCDirector::sharedDirector()->getRunningScene()->addChild(pLayer, 200); 
}


//===================页面索引条======================//
PageBar::PageBar(int pageNum)
{
	init();
	ResManager* resMgr = ResManager::getManager();
	float itemWidth = 23;		// 元素宽度
	float itemHeight = 23;		// 元素宽度
	float itemFrag = 10;		// 元素之间的空格
	setContentSize(CCSizeMake( (itemWidth+itemFrag)*pageNum,itemHeight*1.5f));
	mPageNum = pageNum;
	mCurrentPageIndex = 0;
	itemVec.clear();

	if(pageNum <= 1)
		setVisible(false);

	float posX = 0.0f;
	float posY = getContentSize().height/2;
	for (int i=0;i<pageNum;i++)
	{
		CCSprite* item = CCSprite::create(resMgr->getSharedFilePath(g_seasonlayerPath+"season_point.png").c_str());
		itemVec.insert(itemVec.end(),item);
		addChild(item);

		posX += item->getContentSize().width;
		item->setPosition(ccp(posX,posY));
		posX += itemFrag;
	}
	setPageIndex(mCurrentPageIndex);
}
int PageBar::getCurrentPageIndex()
{
	return mCurrentPageIndex;
}
void PageBar::setPageIndex(int pageIndex)
{
	if ( pageIndex < 0 || pageIndex > (mPageNum-1) )
		return;

	itemVec[mCurrentPageIndex]->initWithFile(ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"season_point.png").c_str());
	itemVec[pageIndex]->initWithFile(ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"season_point_select.png").c_str());
	mCurrentPageIndex = pageIndex;
}
int PageBar::getPageNum()
{
	return mPageNum;
}
