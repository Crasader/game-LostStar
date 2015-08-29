#include "SeasonSelectorLayer.h"
#include "GameInforEditer.h"
#include "GameLayerFactory.h"
#include "LevelManager.h"
#include "ResManager.h"
#include "SpriteHelper.h"
#include "Music.h"
#include "VersionControl.h"
#include "LFShareKit.h"
#include "LFStrings.h"
#include "font.h"
#include "LFMessageBox.h"
#include "LFUtile.h"
#include "LFAlerts.h"
#include "JuqingDaoJuManager.h"
#include "History.h"
#define Hide_Time 2.0f

#define HEADER_AD_NUM 1
#define FOOTER_AD_NUM 1


static int Header_Ad_Num = 0;		// 这里一定要是0，除非是测试用，可以用其他的数据
static int Footer_Ad_Num = 0;		// 这里一定要是0，除非是测试用，可以用其他的数据

// 重新输入电话号码的次数
static int reInputPhoneNumberTime = 0;

SeasonSelectorLayer* SeasonSelectorLayer::instance;
int SeasonSelectorLayer::lastSeasonId = 1;
SeasonSelectorLayer::SeasonSelectorLayer(CCSize viewPortSize,int gridNum,int gridNumInX,int gridNumInY,float gridPadding /* = 10.0f */,CCPoint scrollViewPortOffset /* = CCPointZero */,float pageInval /* = 40.0f */)
:GridScrollView(viewPortSize,gridNum,gridNumInX,gridNumInY,gridPadding,scrollViewPortOffset,pageInval)
{
	this->bg = NULL;
	this->menu = NULL;
	this->title = NULL;
	this->gamegoods = NULL;
	canGoToLeftPage = canGoToRightPage = true;

	mTempClickedSeasonId = 1;
	instance = this;

	schedule(schedule_selector(SeasonSelectorLayer::tick));

	// 关闭裁剪
	m_bClippingToBounds = false;

	{
		Music::sharedMusicPlayer()->playMainMusic();
	}

// 	// 尝试初始化
// 	if (!OrderList::isPayChannelInfoInited())
// 	{
// // 		PlantformProtocal pp;
// // 		CmdHelper::getHelper()->cmdQueryPayChannelInfo(pp.getPLMN());
// 	}


	//移触广告专用
	m_seasonId_beforeSelectEnd = 0;
}
SeasonSelectorLayer* SeasonSelectorLayer::getCurrentLayer()
{
	return instance;
}
SeasonSelectorLayer* SeasonSelectorLayer::createSeasonSelectorLayer(int selectedSeasonId)
{
	CCSize _winSize = CCDirector::sharedDirector()->getWinSize();
	CCSize _winSizePixel = CCDirector::sharedDirector()->getWinSizeInPixels();
	CCSize _visibleSize = CCDirector::sharedDirector()->getVisibleSize();


	int seasonNum = GameLayerFactory::getInstance()->getConfigSeasonNum()+Header_Ad_Num+Footer_Ad_Num;
	SeasonSelectorLayer* layer = new SeasonSelectorLayer(
		CCSizeMake(_visibleSize.width,_visibleSize.height),
		seasonNum,
		1,1,
		10,				// pading，这里没有用，因为只有一个元素，于是就没用
		ccp(0,0/*34*/),		// offset
		-400			// 页面间的间隔
		);
	layer->setClickableCreater(layer); 
	layer->prepareToDisplay();
	layer->mSeasonId = selectedSeasonId;
	lastSeasonId = selectedSeasonId;
	//yc ad
	layer->m_seasonId_beforeSelectEnd = layer->mSeasonId;

	CCAssert(selectedSeasonId>=1&&selectedSeasonId<=seasonNum,"");

	// 直接选中这个页面
	layer->scrollToPageSimple(selectedSeasonId-1+Header_Ad_Num,0);

	layer->autorelease();
	return layer;
}
//
void SeasonSelectorLayer::scrollToSeason(int seasonId,float time)
{
	scrollToPageSimple(seasonId-1 + Header_Ad_Num,time);
}
void SeasonSelectorLayer::setUnlockSeasonId(int seasonId)
{
	if (seasonId > GameLayerFactory::getInstance()->getConfigSeasonNum() || seasonId < 0)
	{
		CCLog("Waring setUnlockSeasonId ---> seasonId must bigger than 0 and smaller than %d",GameLayerFactory::getInstance()->getConfigSeasonNum());
		return;
	}

	scrollToPageSimple(seasonId-1 + Header_Ad_Num,1.5f);
// 
// 	//测试用 强制修改状态为购买状态
// 	//vector<JuqingDaoJu> allitem = JuqingDaoJuManager::getManager()->getAllItemsInSeason(seasonId);
// 	//int i;
// 	//for(i=0;i<allitem.size();++i)
// 	//	JuqingDaoJuManager::getManager()->setItemIsGettedState(seasonId,(seasonId-1)*ITEM_NUM_IN_EACH_SEASON+1+i,seasonId == 1 ? DJS_GettedByBuy : DJS_NotGetted);
// 
// 	bool isGettedAllGoods = JuqingDaoJuManager::getManager()->canUnlockedInSeason(seasonId);
// 	CCLog("%s Card:%d All Getted: %s",__FUNCTION__,seasonId,isGettedAllGoods?"ok":"no");
// 	if(isGettedAllGoods){
// 		CCLog("get all goods");
// 		//TODO: 处理需要播放解锁关卡的显示
// 		scrollToPageSimple(seasonId-1 + Header_Ad_Num,1.5f);
// 	}else{
// 		CCLog("not get all goods");
// 		scrollToPageSimple(seasonId-1 + Header_Ad_Num,1.5f);
// 		return;
// 	}

}
void SeasonSelectorLayer::pageSelectedEnd(int pageIndex)
{
	//解锁动画
	if (gamegoods && GameInforEditer::getGameInfor()->canUnclock(lastSeasonId, false))
	{
		gamegoods->openUnlock();

		SeasonItem* item = (SeasonItem*)gridItemArray->objectAtIndex(lastSeasonId - 1 + Header_Ad_Num);
		item->runUnlockAction(1.4f);

		GameInforEditer::getGameInfor()->unLockSeasonForce(lastSeasonId);
		GameInforEditer::getGameInfor()->saveSessonInforList();
	}

// 	//yc ad
// 	//1.在第一章的上一个滑屏处和第五章的下一个滑屏处，即当前显示第一章关卡按钮，
// 	//再向右滑动，或者当前显示第五章关卡按钮，再向左滑动，显示插屏广告，必须点击关闭广告才能滑动屏幕
// 	if (m_seasonId_beforeSelectEnd == 1 && pageIndex == 0){
// 		AdManager::getManager()->showAd();
// 	}
// 	else if (m_seasonId_beforeSelectEnd == 5 && pageIndex == 5){
// 		AdManager::getManager()->showAd();
// 	}
	m_seasonId_beforeSelectEnd = lastSeasonId;
}

ClickableItem* SeasonSelectorLayer::createGridItem(int pageIndex,int xIndex,int yIndex)
{
	{
		int seasonNumInSeasonInfor = GameLayerFactory::getInstance()->getConfigSeasonNum();
		// 是头部广告，那么就自己创建一个返回
		if (pageIndex < Header_Ad_Num)
		{
			ClickableItem* ad = new ClickableItem();
			ad->initWithFile(ResManager::getManager()->getSharedFilePath("head_ad.png").c_str());
			CCLog("createGridItem header ad");
			ad->autorelease();
			return ad;
		}
		// 是尾部广告
		else if (pageIndex > seasonNumInSeasonInfor-1 + Header_Ad_Num)
		{
			ClickableItem* ad = new ClickableItem();
			ad->initWithFile(ResManager::getManager()->getSharedFilePath("end_ad.png").c_str());
			CCLog("createGridItem footer ad");
			ad->autorelease();
			return ad;
		}else
		{
			// 更改索引
			pageIndex -= Header_Ad_Num;
		}
	}
	
	GameInforEditer* infor = GameInforEditer::getGameInfor();
	int seasonId = pageIndex + 1;
	bool locked = true;

	SeasonInfor* seasonInfor = infor->getSeasonInfor(seasonId);
	if ( (seasonInfor == NULL || seasonInfor->locked) && !GameInforEditer::getGameInfor()->isIgnoreAllLock() )
	{
		locked = true;
	}else
	{
		locked = false;
	}
	
	SeasonItem* item = new SeasonItem(seasonId,locked,seasonInfor);
	item->autorelease();
	item->setSeasonSelectorLayer(this);
	m_iSeasonItem = item;
	if (CCUserDefault::sharedUserDefault()->getBoolForKey(IsFirstSelectSeason,true))
	{
		if (!locked)
		{
			CCUserDefault::sharedUserDefault()->setBoolForKey(IsFirstSelectSeason,false);
			CCUserDefault::sharedUserDefault()->flush();
			item->firstUnLock();
		}
	}
	else
	{
		int new_season_id = CCUserDefault::sharedUserDefault()->getIntegerForKey(TheNewSelectSeason,0);
		if ( new_season_id != 0)
		{
			if ((new_season_id == seasonId) && (!locked))
			{
				item->firstUnLock();
			}
		}
	}
	
	
	return item;
}


SeasonItem* SeasonSelectorLayer::getCurrentSelectedSeason()
{
	SeasonItem* item = (SeasonItem*) gridItemArray->objectAtIndex(lastSeasonId - 1 + Header_Ad_Num);
	return item;
}
//
bool SeasonSelectorLayer::pageScrollIntercept(int pageIndex)
{
	// 默认处理
	return GridScrollView::pageScrollIntercept(pageIndex);
}

void SeasonSelectorLayer::pageSelected(int pageIndex)
{
	CCLog("SeasonSelectorLayer::pageSelected index = %d",pageIndex);
	{
		int seasonNumInSeasonInfor = GameLayerFactory::getInstance()->getConfigSeasonNum();
		// 是广告
		if (pageIndex < Header_Ad_Num)
		{
			lastSeasonId = 1;
			if (title)
				title->setVisible(false);
		}
		// 是尾部广告
		else if (pageIndex > seasonNumInSeasonInfor-1 + Header_Ad_Num)
		{
			lastSeasonId = seasonNumInSeasonInfor;
			if (title)
				title->setVisible(false);
		}
		else
		{
			lastSeasonId = pageIndex - Header_Ad_Num + 1;
			if (title)
				title->setVisible(true);
		}
	}

	CCLog("SeasonSelectorLayer::pageSelected ---> change bg index = %d,title id = %d",pageIndex,lastSeasonId);
	if (bg)
		bg->setSelectedPageIndex(pageIndex);
	if (title)
		title->setSeletedSeasonId(lastSeasonId);
	if(gamegoods)
		gamegoods->setSeasonId(lastSeasonId);

	if (menu)
	{
		menu->setFrontButtonVisible(true);
		menu->setNextButtonVisible(true);
	}
	if (pageIndex <= 0 && menu)
	{
		menu->setFrontButtonVisible(false);
	}
	if (pageIndex >= (cal_PageNum-1) && menu)
	{
		menu->setNextButtonVisible(false);
	}

	SeasonItem* item = (SeasonItem*) gridItemArray->objectAtIndex(lastSeasonId - 1 + Header_Ad_Num);
	SeasonInfor* seasonInfor = GameInforEditer::getGameInfor()->getSeasonInfor(lastSeasonId);
	if (GameInforEditer::getGameInfor()->canUnclock(lastSeasonId))
	{
		GameInforEditer::getGameInfor()->unLockSeasonForce(lastSeasonId);
		GameInforEditer::getGameInfor()->saveSessonInforList();
		item->runUnlockAction(0.3f);
	}
	else if ( item->isloked() && ( (seasonInfor && seasonInfor->locked == false) || GameInforEditer::getGameInfor()->isIgnoreAllLock() ) )
	{
		item->runUnlockAction(0.3f);
	} 
}

void SeasonSelectorLayer::itemClicked(ClickableItem* clickedObj)
{
	int clickedPage = getPageIndex(clickedObj);
	if (cal_currentShowedPageIndex == clickedPage)
	{
		GridScrollView::itemClicked(clickedObj);
	}else
	{
		scrollToPageSimple(clickedPage);
	}
}
void SeasonSelectorLayer::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent,CCPoint glPoint)
{
	GridScrollView::onTouchBegan(pTouch,pEvent,glPoint);
	unschedule(schedule_selector(SeasonSelectorLayer::timeToHideFrontNextItem));

	if (menu)
	{
		menu->getFrontItem()->stopAllActions();
		menu->getNextItem()->stopAllActions();
		menu->getFrontItem()->setEnabled(true);
		menu->getNextItem()->setEnabled(true);
		menu->getFrontItem()->runAction(CCFadeTo::create(0.3f,255));
		menu->getNextItem()->runAction(CCFadeTo::create(0.3f,255));
	}

}

void SeasonSelectorLayer::onTouchMove(CCTouch *pTouch, CCEvent *pEvent,CCPoint glPoint)
{
	GridScrollView::onTouchMove(pTouch,pEvent,glPoint);
}
void SeasonSelectorLayer::onTouchEnd(CCTouch *pTouch, CCEvent *pEvent,CCPoint glPoint)
{
	GridScrollView::onTouchEnd(pTouch,pEvent,glPoint);
	schedule(schedule_selector(SeasonSelectorLayer::timeToHideFrontNextItem),Hide_Time);
	// 重新启动，因为MyScrollView会cancel所有的
	unschedule(schedule_selector(SeasonSelectorLayer::tick));
	schedule(schedule_selector(SeasonSelectorLayer::tick));
}

void SeasonSelectorLayer::timeToHideFrontNextItem(float time)
{
	unschedule(schedule_selector(SeasonSelectorLayer::timeToHideFrontNextItem));
	if (menu)
	{
		menu->getFrontItem()->stopAllActions();
		menu->getNextItem()->stopAllActions();
		menu->getFrontItem()->setEnabled(false);
		menu->getNextItem()->setEnabled(false);
		menu->getFrontItem()->runAction(CCFadeTo::create(0.3f,0));
		menu->getNextItem()->runAction(CCFadeTo::create(0.3f,0));
	}
}

void SeasonSelectorLayer::setCombinedLayers
(SeasonSelectorLayerBackgroud* bg,SeasonSelectorLayerMenu* menu,SeasonSelectorLayerTitle* title,SeasonSelectorLayerGameGoods* gamegoods)
{
	this->bg = bg;
	this->menu = menu;
	this->title = title;
	this->gamegoods = gamegoods;
	
	menu->setEnbale(false);

	if (mSeasonId<=1 && menu)
	{
		menu->setFrontButtonVisible(false);
	}
	if (mSeasonId >= cal_PageNum && menu)
	{
		menu->setNextButtonVisible(false);
	}

	schedule(schedule_selector(SeasonSelectorLayer::timeToHideFrontNextItem),Hide_Time);
	if (bg)
	{
		bg->constract_sort(cal_PageNum);
		bg->setSelectedPageIndex(cal_currentShowedPageIndex);
	}
	if (title)
	{
		title->setSeletedSeasonId(mSeasonId);
	}
	if(gamegoods)
		gamegoods->setSeasonId(mSeasonId);
}
void SeasonSelectorLayer::seasonItemCliked(int seasonId)
{
	mTempClickedSeasonId = seasonId;
	if(menu)
		menu->setVisible(false);
	zoomIn(true);
}
void SeasonSelectorLayer::zoomIn(bool animation,bool jumpToSection/* = true*/)
{
	float actionTime = 0.2f;
	CCObject* obj = gridItemArray->objectAtIndex(mTempClickedSeasonId-1 + Header_Ad_Num);
	CCSprite* tempItem = dynamic_cast<SeasonItem*> (obj);
	float scale = 1.0f/tempItem->getScale();
	float moveUpY = 40.0f;
	//float moveUpY = 120.0f;
	float moveY = (1.0f-scale)*winSize.width/2.0f;
	float moveX = (1.0f-scale)*winSize.width/2.0f;


	if (animation)
	{
		setTouchEnabled(false);
		CCFiniteTimeAction* action = CCSequence::create(CCScaleTo::create(actionTime,scale),CCCallFuncN::create(this,SEL_CallFuncN(&SeasonSelectorLayer::actionEnd)),NULL);
		CCFiniteTimeAction* actionWithoutCallBack =CCScaleTo::create(actionTime,scale);
		CCMoveBy* actionMoveBy = CCMoveBy::create(actionTime,ccp(0,moveUpY));
		if (!jumpToSection)
		{
			runAction((CCFiniteTimeAction*)actionWithoutCallBack->copy()->autorelease());
		}else
		{
			runAction((CCFiniteTimeAction*)action->copy()->autorelease());
		}
		runAction(actionMoveBy);
		if (bg)
		{
			bg->runAction((CCFiniteTimeAction*)actionWithoutCallBack->copy()->autorelease());
			bg->runAction(CCMoveTo::create(actionTime,ccp(moveX,moveY+moveUpY)));
		}
		if (title)
		{
			title->runAction((CCFiniteTimeAction*)actionWithoutCallBack->copy()->autorelease());
			title->runAction(CCMoveTo::create(actionTime,ccp(moveX,moveY+moveUpY)));
		}
	}else
	{
		setScale(scale);
		setPositionY(getPositionY()+moveUpY);

		if (bg)
		{
			bg->setScale(scale);
			bg->setPosition(ccp(moveX,moveY));
		}

		if (title)
		{
			title->setScale(scale);
			title->setPosition(ccp(moveX,moveY));
		}
	}
	
}
void SeasonSelectorLayer::zoomOut(bool animation)
{
	float actionTime = 0.2f;
	//CCObject* obj = gridItemArray->objectAtIndex(mTempClickedSeasonId-1);
	//CCSprite* tempItem = dynamic_cast<SeasonItem*> (obj);
	float scale = 1.0f;
	float moveUpY = -40.0f;
	float moveY = -(1.0f-scale)*winSize.width/2.0f;
	float moveX = -(1.0f-scale)*winSize.width/2.0f;

	if (animation)
	{
		setTouchEnabled(true);
		//CCFiniteTimeAction* action = CCSequence::create(CCScaleTo::create(actionTime,scale),CCCallFuncN::create(this,SEL_CallFuncN(&SeasonSelectorLayer::zoomOutEnd)),NULL);
		CCFiniteTimeAction* actionWithoutCallBack =CCScaleTo::create(actionTime,scale);
		CCMoveBy* actionMoveBy = CCMoveBy::create(actionTime,ccp(0,moveUpY));
		runAction((CCFiniteTimeAction*)actionWithoutCallBack->copy()->autorelease());
		runAction(actionMoveBy);
		if (bg)
		{
			bg->runAction((CCFiniteTimeAction*)actionWithoutCallBack->copy()->autorelease());
			bg->runAction(CCMoveTo::create(actionTime,ccp(moveX,moveY)));
		}
		if (title)
		{
			title->runAction((CCFiniteTimeAction*)actionWithoutCallBack->copy()->autorelease());
			title->runAction(CCMoveTo::create(actionTime,ccp(moveX,moveY)));
		}
	}else
	{
		setScale(scale);
		setPositionY(getPositionY()+moveUpY);

		bg->setScale(scale);
		bg->setPosition(ccp(moveX,moveY));
		title->setScale(scale);
		title->setPosition(ccp(moveX,moveY));
	}
}
void SeasonSelectorLayer::zoomOutEnd(CCNode* node)
{
	setTouchEnabled(true);
}
void SeasonSelectorLayer::actionEnd(CCNode* node)
{
	setTouchEnabled(true);
	LevelManager::getManager()->goToSectionUI(mTempClickedSeasonId,false);
}

void SeasonSelectorLayer::tick(float time)
{
	float minDistance = 2048;
	//第i页的Offset就应该是getPageOffsetX(i-1)，偏移越多就越透明
	for (int i=0;i<cal_PageNum;i++)
	{
		float distance = abs(getContentOffset().x - getPageOffsetX(i));
		if ( distance < minDistance)
			minDistance = distance;

		//int opacity = 255 - distance/1024.0f * 255;
		int opacity = 255 - distance/(1024.0f*0.5f) * 255;
		if (opacity < 0)
			opacity = 0;
		//setPageItemOpacity(i,opacity);
		
		CCArray* pageItems = getPageItems(i);

		for (size_t j=0;j<pageItems->count();j++)
		{
			//((SeasonItem*)pageItems->objectAtIndex(j))->setUpperItemOpacity(opacity);

			// 使用 中间缩放的形式
			float scale = 1.0f - distance/(1024.0f*2.0f);
			((SeasonItem*)pageItems->objectAtIndex(j))->setAnchorPoint(ccp(0.5f,0.5f));
			((SeasonItem*)pageItems->objectAtIndex(j))->setScale(scale);
		}
	}

	int opacity = 255 - minDistance/512.0f * 255;
	if (opacity < 0)
		opacity = 0;
}


//////////////////////////////////////////////////////////////////////////
// SeasonItem
//////////////////////////////////////////////////////////////////////////
SeasonItem::SeasonItem(int seasonId,bool locked,SeasonInfor* infor /*= NULL*/)
{
	mSeasonId = seasonId;
	mLocked = locked;
	mLayer = NULL;
	itemInfor = infor;
	upperLightRight = NULL;
	curtainRight = NULL;
	lowerCurtainRight = NULL;
	upperLightLeft = NULL;
	curtainLeft = NULL;
	lowerCurtainLeft = NULL;
	curtainUpperShadow = NULL;	// 剪影
	curtainGril = NULL;
	floorShadow = NULL;	// 地面上的阴影

	star = NULL;
	lableTotal = NULL;
	unLock = NULL;
	season_lock = NULL;

	init();

	{	
		float offset_x = 11;
		float offset_y = 0;
		initWithFile(ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"curtain.png").c_str());
		//CCSprite *bg_spr = CCSprite::create(ResManager::getManager()->getSharedFilePath("curtain.png").c_str());
		//this->initWithTexture(bg_spr->getTexture());
		//this->ignoreAnchorPointForPosition(false);
		//this->setAnchorPoint(ccp(0.5,0.5));
		//bg_spr->setAnchorPoint(ccp(0.5,0.5));
		//bg_spr->setPosition(CCPointZero);
		//addChild(bg_spr,-1);
		//this->setTouchEnabled(false);

		// 不可点击
		if (itemInfor && itemInfor->seasonType == type_CommingSoon)
		{
			//int tempCurtainId = 1;
			curtainGril = CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_seasonlayerPath+"season_new_game.png").c_str());
			addChild(curtainGril,128);
			curtainGril->setPosition(ccp(getContentSize().width/2+offset_x,getContentSize().height/2+offset_y));
			curtainGril->setScale(1.0f/getScale());
		}
		else
		{// 只用一层了，不在用几层了
			int	tempGirlId = seasonId;
			int tempShadowId = seasonId;

			string shadowFile = ResManager::getManager()->getSharedFilePath() +g_seasonlayerPath+ SpriteHelper::splicingNum_String("season_item_curtain_shadow_s%d.png",tempShadowId);
			curtainUpperShadow = CCSprite::create(shadowFile.c_str());
			addChild(curtainUpperShadow,128);
			curtainUpperShadow->setPosition(ccp(getContentSize().width/2+offset_x,getContentSize().height/2+offset_y));
			curtainUpperShadow->setScale(1.0f/getScale());

			string girlFile = ResManager::getManager()->getSharedFilePath() + g_seasonlayerPath+SpriteHelper::splicingNum_String("season_item_curtain_girl_s%d.png",tempGirlId);
			curtainGril = CCSprite::create(girlFile.c_str());
			addChild(curtainGril,128);
			curtainGril->setPosition(ccp(getContentSize().width/2+offset_x,getContentSize().height/2+offset_y));
			curtainGril->setScale(1.0f/getScale());

			// 星星 和 锁
			{
				ResManager* resMgr = ResManager::getManager();
#if Current_SeasonItem_Theme == Theme_StarToUnlock
				int needStarNum = GameInforEditer::getGameInfor()->getScoreForUnlock(seasonId);
				CCLabelAtlas* label = CCLabelAtlas::create(
					"0123456789",
					resMgr->getSharedFilePath(g_seasonlayerPath+"season_star_amount.png").c_str(),
					30, 47,//24
					'0');
				lableTotal = label;
				char str[128];
				sprintf(str,"%d",needStarNum);
				label->setString(str);

				star = CCSprite::create(resMgr->getSharedFilePath(g_seasonlayerPath+"congratulation_star_middle.png").c_str());
				star->setScale(0.3f);
				star->setRotation(-8.0f);

				unLock = CCSprite::create(resMgr->getFilePathByLanguageType(g_seasonlayerPath+"season_unlock.png").c_str());
				season_lock = CCSprite::create(resMgr->getSharedFilePath(g_seasonlayerPath+"season_lock.png").c_str());

				curtainUpperShadow->addChild(label);
				curtainUpperShadow->addChild(star);
				curtainUpperShadow->addChild(unLock);
				curtainUpperShadow->addChild(season_lock);


				season_lock->setPosition(ccp(514,0));
				unLock->setPosition(ccp(
					season_lock->getPositionX() - season_lock->getContentSize().width/2 - unLock->getContentSize().width/2,
					season_lock->getPositionY() - unLock->getContentSize().height * 1.2f));
				star->setPosition(ccp(
					season_lock->getPositionX() - season_lock->getContentSize().width/2 - star->getContentSize().width/2*star->getScale(),
					season_lock->getPositionY()));

				label->setPosition(ccp(
					star->getPositionX() - label->getContentSize().width - star->getContentSize().width/2*star->getScale(),
					star->getPositionY() - label->getContentSize().height/2));
#elif Current_SeasonItem_Theme == Theme_PayToUnlock

				star = NULL;
				char temp[512];
// 				if (OrderList::isPayChannelInfoInited())
// 				{
// 					sprintf(temp,
// 						LFStrings::getValue("needMoney").c_str(),
// 						OrderList::getPriceBySeasonId(mSeasonId),
// 						OrderList::getMoneySymble().c_str());
// 				}else
// 				{
// 					sprintf(temp, "%s", LFStrings::getValue("needMoney_unkown").c_str());
// 				}
				
				CCLabelTTF* lable = CCLabelTTF::create("",fontStr_katong,40,CCSizeMake(450,0),kCCTextAlignmentRight);
				lable->setString(temp);
				lable->setAnchorPoint(ccp(1.0f,0.5f));
				lableTotal = lable;
				lable->setVisible(false);

				unLock = CCSprite::create(resMgr->getFilePathByLanguageType(g_seasonlayerPath+"season_unlock.png").c_str());
				season_lock = CCSprite::create(resMgr->getSharedFilePath(g_seasonlayerPath+"season_lock.png").c_str());

				curtainUpperShadow->addChild(lable);
				curtainUpperShadow->addChild(unLock);
				curtainUpperShadow->addChild(season_lock);

				season_lock->setPosition(ccp(514,0));
				unLock->setPosition(ccp(
					season_lock->getPositionX() - season_lock->getContentSize().width/2 - unLock->getContentSize().width/2,
					season_lock->getPositionY() - unLock->getContentSize().height * 1.2f));

				lable->setPosition(ccp(unLock->getPositionX() + unLock->getContentSize().width/2,season_lock->getPositionY()));

#else
#error SeasonItem Theme is Not Surport  
#endif
			}

			//--原6元购买通关的状态
			//bool ignoreAllLock = GameInforEditer::getGameInfor()->isIgnoreAllLock();

			//通过剧情道具并且通关上一章节才可以解锁下一章节的关卡

			//屏蔽剧情道具相关
			//bool isGettedAllGoods = JuqingDaoJuManager::getManager()->canUnlockedInSeason(seasonId);
			bool isGettedAllGoods = true;
			// 锁住的
			if (locked && !isGettedAllGoods)
			{
				if (curtainGril)
					curtainGril->setVisible(false);
				if (curtainUpperShadow)
					curtainUpperShadow->setVisible(true);
			}
			else
			{
				if (curtainGril)
					curtainGril->setVisible(true);
				if (curtainUpperShadow)
					curtainUpperShadow->setVisible(false);
			}
		}

		return;
	}
}
void SeasonItem::runUnlockAction(float delay)
{
	// 不可点击
	if (itemInfor && itemInfor->seasonType == type_CommingSoon)
		return;
	
	mLocked = false;

	if(star)
	{
		star->setVisible(false);
	}
	if (lableTotal)
	{
		lableTotal->setVisible(false);
	}
	if (season_lock)
	{
		unLock->setVisible(false);
		season_lock->runAction(CCSequence::create(CCDelayTime::create(delay),CCFadeTo::create(0.5f,0),NULL));
	}

	if (curtainGril)
		curtainGril->setVisible(true);
	if (curtainUpperShadow)
		curtainUpperShadow->setVisible(true);

	if (curtainGril)
		curtainGril->setOpacity(0);
	if (curtainUpperShadow)
		curtainUpperShadow->setOpacity(255);

	if (curtainGril)
		curtainGril->runAction(CCSequence::create(CCDelayTime::create(delay),CCFadeTo::create(0.5f,255),NULL));
	if (curtainUpperShadow)
		curtainUpperShadow->runAction(CCSequence::create(CCDelayTime::create(delay),CCFadeTo::create(0.5f,0),NULL));
}
void SeasonItem::setScale(float fScale)
{
	CCSprite::setScale(1.0f*fScale);
	//CCSprite::setScale(0.4f*fScale);
}
void SeasonItem::setSeasonSelectorLayer(SeasonSelectorLayer* layer)
{
	mLayer = layer;
}
void SeasonItem::firstUnLock()
{
	this->schedule(SEL_SCHEDULE(&SeasonItem::showShakeItem),2.0f);
}

void SeasonItem::showShake(CCNode *node)
{
	m_nNodeCount = 0;
	node->setScale(0.8f);
	/*nodeAndAllChildRunAction(node,CCRepeatForever::create(CCSequence::create(
		CCScaleTo::create(0.3f,1.05f),
		CCScaleTo::create(0.1f,1.0f),
		CCDelayTime::create(2.0f),
		NULL)));*/
	nodeAndAllChildRunAction(node,CCSequence::create(
		CCScaleTo::create(0.3f,1.05f),
		CCScaleTo::create(0.1f,1.0f),
		//CCCallFuncN::create(this,callfuncN_selector(SeasonItem::showZhangjieLight)),
		NULL));
}
void SeasonItem::nodeAndAllChildRunAction(CCNode* node, CCActionInterval* action)
{
	m_nNodeCount++;
	if(node == NULL || action == NULL)
		return;
	//childe
	CCArray* childs = node->getChildren();
	if (childs != NULL)
	{
		for (int i=0; i!=childs->count(); ++i)
		{
			nodeAndAllChildRunAction((CCNode*)childs->objectAtIndex(i), (CCActionInterval*)action->copy()->autorelease());
		}
	}
	
	if (m_nNodeCount == 1)
	{
		node->runAction(CCSequence::create(action,CCCallFuncN::create(this,callfuncN_selector(SeasonItem::showZhangjieLight)),NULL));
	}
	else
	{
		node->runAction(action);
	}
	m_nNodeCount--;
}
void SeasonItem::showZhangjieLight(CCNode *node)
{
	const float moveTime = 0.8f;
	const float fadeTime = 0.8f;
	CCSprite *topLightSpr = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"zhangjie_xuanze_xiaoguo_liuguang_2.png").c_str());
	topLightSpr->setAnchorPoint(ccp(0.5,0.5));
	topLightSpr->setPosition(ccp(180,509));
	this->addChild(topLightSpr);

	topLightSpr->runAction(CCSequence::create(CCSpawn::create(CCFadeIn::create(0.1f),
		CCMoveBy::create(moveTime,ccp(118,0)),
		NULL),
		CCSpawn::create(CCFadeOut::create(fadeTime),
		CCMoveBy::create(moveTime,ccp(118,0)),
		NULL),
		CCCallFuncN::create(this,SEL_CallFuncN(&SeasonItem::zhangjieLightEnd)),
		NULL));


	CCSprite *bottomLightSpr = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"zhangjie_xuanze_xiaoguo_liuguang_2.png").c_str());
	bottomLightSpr->setAnchorPoint(ccp(0.5,0.5));
	bottomLightSpr->setPosition(ccp(568,9));
	this->addChild(bottomLightSpr);

	bottomLightSpr->runAction(CCSequence::create(CCSpawn::create(CCFadeIn::create(0.1f),
		CCMoveBy::create(moveTime,ccp(-280,0)),
		NULL),
		CCSpawn::create(CCFadeOut::create(fadeTime),
		CCMoveBy::create(moveTime,ccp(-280,0)),
		NULL),
		CCCallFuncN::create(this,SEL_CallFuncN(&SeasonItem::zhangjieLightEnd)),
		NULL));
	
}
void SeasonItem::zhangjieLightEnd(CCNode *node)
{
	if (node)
	{
		node->removeFromParent();
	}
}
void SeasonItem::setUpperItemOpacity(GLubyte opacity)
{
	return;

	if (upperLightRight)
		upperLightRight->setOpacity(opacity);
	if (curtainRight)
		curtainRight->setOpacity(opacity);
	if (upperLightLeft)
		upperLightLeft->setOpacity(opacity);
	if (curtainLeft)
		curtainLeft->setOpacity(opacity);

	if (floorShadow)
		floorShadow->setOpacity(opacity);
	if (curtainUpperShadow)
		curtainUpperShadow->setOpacity(opacity);
	if (curtainGril)
		curtainGril->setOpacity(opacity);

}
void SeasonItem::clickedMe()
{
	Music::sharedMusicPlayer()->playEffectSound("season_clicked.wav");
	// 不可点击
	if (itemInfor && itemInfor->seasonType == type_CommingSoon)
		return;

	if (itemInfor && itemInfor->seasonType == type_URL)
	{
		std::string *sp = static_cast<std::string*>(itemInfor->data);
		std::string s = *sp;
		LFShareKit::getKit()->openURl(s);
		return;
	}

	do{
		if (itemInfor && itemInfor->seasonType != type_Normal)
			break;;
		bool ignoreAllLock = GameInforEditer::getGameInfor()->isIgnoreAllLock();
		// 锁住的
		if (mLocked && !ignoreAllLock)
		{
			//addBuyObserver();
			//IPaymentManager* manager = PaymentFactory::getFactory()->getCurrentPaymentManager();
			//manager->reset();
			//manager->showBySeasonUI(mSeasonId);
			GameInforEditer* info = GameInforEditer::getGameInfor();
			if(info->getLastPlayedSeasonId() == mSeasonId - 1){
				LFAlert_BuySeasonAndItsSections::showAlert(mSeasonId);
					//->addAutoRemoveObserver(this,SEL_CallFuncO(&SeasonItem::buySeasonAndItsSectionsRsp),MSG_PP_SeasonAndItsSectionsRsp);
			}

			return;
		}
		ClickableItem::clickedMe();
		LevelManager::getManager()->goToSectionUI(mSeasonId,true);
	}while(false);
}

void SeasonItem::addBuyObserver()
{
//	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,SEL_CallFuncO(&SeasonItem::buySeasonRsp),MSG_PP_BuySeasonRsp,NULL);
}
void SeasonItem::removeBuyObserver()
{
//	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_PP_BuySeasonRsp);

}
void SeasonItem::buySeasonAndItsSectionsRsp(CCObject* payRspWrapper)
{
// 	removeBuyObserver();
// 	PayRspWrapper* rspWrapper = (PayRspWrapper*)payRspWrapper;
// 	PPCmd_BuySeasonAndItsSectionsRsp* rsp = (PPCmd_BuySeasonAndItsSectionsRsp*)rspWrapper->data;
// 	if (rsp->rspCode == PP_Success)
// 	{
// 		// 移除对话框
// 		BaseBuyAlert::removeAllBuyAlerts();
// 
// 		SeasonInfor* infor = GameInforEditer::getGameInfor()->getSeasonInfor(mSeasonId);
// 		if (infor && infor->locked == false)
// 		{
// 			runUnlockAction(0.2f);
// 		}
// 	}else
// 	{
// 		// 失败
// 		LFToast::makeToast(rsp->unsuccess_infor.c_str(),3.0f);
// 	}
}
void SeasonItem::buySeasonRsp(CCObject* payRspWrapper)
{
// 	removeBuyObserver();
// 	PayRspWrapper* rspWrapper = (PayRspWrapper*)payRspWrapper;
// 	PPCmd_BuySeasonRsp* rsp = (PPCmd_BuySeasonRsp*)rspWrapper->data;
// 	if (rsp->rspCode == PP_Success)
// 	{
// 		SeasonInfor* infor = GameInforEditer::getGameInfor()->getSeasonInfor(mSeasonId);
// 		if (infor && infor->locked == false)
// 		{
// 			runUnlockAction(0.2f);
// 		}
// 	}else
// 	{
// 		// 失败
// 		LFToast::makeToast(rsp->unsuccess_infor.c_str(),3.0f);
// 	}
}

void SeasonItem::actionEnd(CCNode* node)
{
	//if (mLayer)
	//	mLayer->setTouchEnabled(true);
	//CCScene* runningScene = CCDirector::sharedDirector()->getRunningScene();
	//runningScene->runAction(CCSequence::create(CCDelayTime::create(1.5f),CCScaleTo::create(0.5f,1.0f),NULL));

	//mLayer->runAction(CCSequence::create(CCDelayTime::create(1.5f),CCScaleTo::create(0.5f,1.0f),NULL));
}

void SeasonItem::touchMe()
{
	return;
	setColor(ccGRAY);

	CCArray* children = getChildren();
	for (size_t i=0;i<getChildrenCount();i++)
	{
		((CCSprite*)children->objectAtIndex(i))->setColor(ccGRAY);
	}
}
void SeasonItem::leaveMe()
{
	return;
	setColor(ccWHITE);
	CCArray* children = getChildren();
	for (size_t i=0;i<getChildrenCount();i++)
	{
		((CCSprite*)children->objectAtIndex(i))->setColor(ccWHITE);
	}
}

void SeasonItem::onExit()
{
	ClickableItem::onExit();
	removeBuyObserver();
}


///////////////////////////////////////////////////////////////////////////
// SeasonSelectorLayerMenu
//////////////////////////////////////////////////////////////////////////
void SeasonSelectorLayerMenu::buySeasonAndItsSectionsRsp(CCObject* payRspWrapper)
{
// 	PayRspWrapper* rspWrapper = (PayRspWrapper*)payRspWrapper;
// 	PPCmd_BuySeasonAndItsSectionsRsp* rsp = (PPCmd_BuySeasonAndItsSectionsRsp*)rspWrapper->data;
// 	if (rsp->rspCode == PP_Success)
// 	{
// 		// 移除对话框
// 		BaseBuyAlert::removeAllBuyAlerts();
// 		SeasonItem* item = seasonLayer->getCurrentSelectedSeason();
// 		SeasonInfor* infor = GameInforEditer::getGameInfor()->getSeasonInfor(item->getSeasonId());
// 		if (infor && infor->locked == false && item->getSeasonId() != 1)
// 		{
// 			item->runUnlockAction(0.2f);
// 		}
// 	}else
// 	{
// 		// 失败
// 		LFToast::makeToast(rsp->unsuccess_infor.c_str(),3.0f);
// 	}
}