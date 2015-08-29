#include "SectionSelectorLayer.h"
#include "GameLayerFactory.h"
#include "GameScenseDefine.h"
#include "ResManager.h"
#include "MaskLayer.h"
#include "Music.h"
#include "cocos-ext.h"
#include "LFAlerts.h"
#include "History.h"
#include "LFToast.h"
#include "JuqingDaoJuManager.h"
#include "UserDataManager.h"
using namespace extension;

SectionSelectorLayer* SectionSelectorLayer::instance;
//int SectionSelectorLayer::lastSectionId = 0;

SectionSelectorLayer::SectionSelectorLayer(CCSize viewPortSize,int gridNum,int gridNumInX,int gridNumInY,float gridPadding /* = 10.0f*/,CCPoint scrollViewPortOffset /* = CCPointZero*/,float pageInval/* = 40.0f*/):GridScrollView(viewPortSize,gridNum,gridNumInX,gridNumInY,gridPadding,scrollViewPortOffset,pageInval)
{
	instance = this;
	schedule(schedule_selector(SectionSelectorLayer::tick));


//	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,SEL_CallFuncO(&SectionSelectorLayer::buyGameRsp),MSG_PP_SeasonAndItsSectionsRsp,NULL);


	// 关闭裁剪
	m_bClippingToBounds = false;
	{
		Music::sharedMusicPlayer()->playMainMusic();
	}
}

void SectionSelectorLayer::buyGameRsp(CCObject* payRspWrapper)
{
// 	PayRspWrapper* rspWrapper = (PayRspWrapper*)payRspWrapper;
// 	PPCmd_BuySeasonAndItsSectionsRsp* rsp = (PPCmd_BuySeasonAndItsSectionsRsp*)rspWrapper->data;
// 	if (rsp->rspCode == PP_Success)
// 	{
// 		// 移除对话框
// 		BaseBuyAlert::removeAllBuyAlerts();
// 		//CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_PP_SeasonAndItsSectionsRsp);
// 		// 刷新当前界面
// 		prepareToDisplay();
// 	}else
// 	{
// 		//// 失败
// 		//LFToast::makeToast(rsp->unsuccess_infor.c_str(),3.0f);
// 	}
}

SectionSelectorLayer::~SectionSelectorLayer()
{
//	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_PP_SeasonAndItsSectionsRsp);

}

SectionSelectorLayer* SectionSelectorLayer::createLayer(int seasonId,int lastSeasonId,int lastSectionId,LevelSelectListener* listener)
{
	int sectionNum = GameLayerFactory::getInstance()->getAllSectionNumBySeasonId(seasonId);
	float padding = 100;
	CCSize _visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	SectionSelectorLayer* layer = new SectionSelectorLayer(
		CCSizeMake(_visibleSize.width,_visibleSize.height),	// 视口大小，超出的就看不到了
		sectionNum,				// 元素个数
		Section_Item_Num_In_Row,
		Section_Item_Num_In_Col,// 每一行的元素个数，每一列的元素个数
		padding,				// 整体的内容在视口中的padding
		CCPoint(0,-30),			// 整体内容相对左上角的偏移,推荐是半个Item的宽度，半个Item的高度
		-100					// 每一页之间的间隔
		);
	layer->setPaddingLeft(125,false);
	layer->setPaddingRight(125,false);
	layer->setPaddingTop(175,false);
	layer->setPaddingButtom(117,false);
	layer->seasonId = seasonId;
	layer->listener = listener;
	layer->setClickableCreater(layer);
	//开始显示
	layer->prepareToDisplay();
	//不可以滚动
	//layer->setEnableScroll(false);

	{//加入一个背景
		CCSprite* bg = CCSprite::create(
			ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"transparent16.png").c_str(),
			CCRectMake(0,0,1024,768));
		layer->container = bg;
		CCRect conRect = layer->container->boundingBox();
		CCSize win = CCDirector::sharedDirector()->getWinSize();
		layer->container->setPosition( ccp( (win.width-conRect.size.width)/2,(win.height-conRect.size.height)/2 ));
		//这里就把这个container添加到了GridScrollView::createContainner返回的Sprite里面
		//其实这里的layer->container的用处就是设定了点击的范围而已
		layer->addChild(layer->container,-1);
	}

	layer->gridItemFlatAction();

	int page = 0;
	//如果还是进入到了上次的season，那么就默认滚动到上次选过的关卡页面;
	int x = CCUserDefault::sharedUserDefault()->getIntegerForKey(CCString::createWithFormat("Last_season%d_seation", seasonId)->getCString());
// 	if (seasonId == lastSeasonId)
// 	{
// 		page = layer->getPageIndex(x-1);
// 	}
// 	else
// 	{
// 		layer->scrollToPageSimple(page,0);
// 	}
	page = layer->getPageIndex(x-1);
	layer->scrollToPageSimple(page,0);
	layer->pageSelected(page);

	return layer;
}
void SectionSelectorLayer::gridItemFlatAction()
{
	for (int pageIndex = 0;pageIndex < cal_PageNum;pageIndex++)
	{
		CCArray* firstPageItems = getPageItems(pageIndex);
		// 我们按照从左到右，从上到下，依次取出每一个Item
		// 然后让其做动画

		float scaleTo1Time = 0.05f;
		float scaleTo2Time = 0.02f;
		float delayTime = 0.0f;
		for (size_t i=0;i<firstPageItems->count();i++)
		{
			int index_x = i%gridNumInX;
			int index_y = (i - index_x)/gridNumInX;
			delayTime = (index_x+index_y)*(scaleTo2Time+scaleTo1Time);

			CCDelayTime* delay = CCDelayTime::create(delayTime);
			((CCSprite*)firstPageItems->objectAtIndex(i))->setScale(0.0f);
			((CCSprite*)firstPageItems->objectAtIndex(i))->runAction(CCSequence::create(
				CCDelayTime::create(0.2f),
				delay,
				CCCallFuncN::create(this,SEL_CallFuncN(&SectionSelectorLayer::gridItemPreAppear)),
				CCScaleTo::create(scaleTo1Time,Section_Item_Scale + 0.2f),
				CCScaleTo::create(scaleTo2Time,Section_Item_Scale),NULL));
		}
	}
	
}
void SectionSelectorLayer::gridItemPreAppear(CCNode* item)
{
	Music::sharedMusicPlayer()->playEffectSound("sectionItemAppear.wav");
}

void SectionSelectorLayer::onTouchEnd(CCTouch *pTouch, CCEvent *pEvent,CCPoint glPoint)
{
	GridScrollView::onTouchEnd(pTouch,pEvent,glPoint);

	// 重新启动
	unschedule(schedule_selector(SectionSelectorLayer::tick));
	schedule(schedule_selector(SectionSelectorLayer::tick));
}

void SectionSelectorLayer::tick(float t)
{
	float minDistance = 2048;
	//第i页的Offset就应该是getPageOffsetX(i-1)，偏移越多就越透明
	for (int i=0;i<cal_PageNum;i++)
	{
		float distance = abs(getContentOffset().x - getPageOffsetX(i));
		if ( distance < minDistance)
			minDistance = distance;

		int opacity = 255 - distance/1024.0f * 255;
		if (opacity < 0)
			opacity = 0;
		setPageItemOpacity(i,opacity);
	}

	int opacity = 255 - minDistance/512.0f * 255;
	if (opacity < 0)
		opacity = 0;
	MaskLayer::getCurrentInsstance()->setNext_FrontOpacity((GLubyte)opacity);
}

void SectionSelectorLayer::onTouch(CCTouch *pTouch, CCEvent *pEvent,CCPoint glClickPoint)
{
	//调用父类的实现
	this->GridScrollView::onTouch(pTouch,pEvent,glClickPoint);
    
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCRect viewPort = CCRectMake((winSize.width-mScrollViewPortSize.width)/2,(winSize.height-mScrollViewPortSize.height)/2,mScrollViewPortSize.width,mScrollViewPortSize.height);
	//CCRect viewPort = container->boundingBox();
	
	if (!viewPort.containsPoint(glClickPoint))
	{//点击到了外面
		if (listener!=NULL)
		{
			listener->sectionBackToSeason(seasonId);
		}
	}
}

ClickableItem* SectionSelectorLayer::createGridItem(int pageIndex,int xIndex,int yIndex)
{
	int sectionId = (pageIndex * cal_gridNumInOnePage + yIndex * gridNumInX + xIndex) + 1;

	SectionItem* item = SectionItem::sectionWithId(seasonId,sectionId);

	item->setLevelSelectListener(listener);

	return item;
}
void SectionSelectorLayer::pageSelected(int pageIndex)
{
	if (MaskLayer::getCurrentInsstance())
		MaskLayer::getCurrentInsstance()->setPageIndex(pageIndex);
}


//================================
//下面是SectionItem的数据
//================================
SectionItem::SectionItem(int seasonId,int sectionId)
{
	listener = NULL;
	infor = GameInforEditer::getGameInfor()->getSectionInfor(seasonId,sectionId);
	bool ignoreAllLock = GameInforEditer::getGameInfor()->isIgnoreAllLock();
	if (infor == NULL)
	{
		this->canChoose = false || ignoreAllLock;
		this->currentScore = 0;
		this->sectionId = sectionId;
		this->seasonId =seasonId;
	}else
	{
		this->canChoose = (!infor->locked) || ignoreAllLock;//这些都应该中记录文件中读取;
		this->currentScore = infor->iStarNum;
		this->sectionId = sectionId;
		this->seasonId =seasonId;
	}
}
SectionItem::~SectionItem()
{
}

SectionItem* SectionItem::sectionWithId(int seasonId,int sectionId,bool canChoose)
{
	SectionItem* button = sectionWithId(seasonId,sectionId);
	button->autorelease();
	button->initButton(button,canChoose);
	return button;
}

SectionItem* SectionItem::sectionWithId(int seasonId,int sectionId)
{
	SectionItem* button = new SectionItem(seasonId,sectionId);
	button->autorelease();
	button->initButton(button,button->canChoose);
	return button;
}
//void SectionItem::initButton(SectionItem* button,bool canChoose)
//{
//	button->canChoose = canChoose;
//    char sectionItemBg[128];
//    sprintf(sectionItemBg, "sectionItem_bg_%d.png",seasonId); //sectionItem_bg_1.png
//	button->initWithFile(ResManager::getManager()->getSharedFilePath(sectionItemBg).c_str());
//
//	char str[128];
//	sprintf(str, "season%d_sectionId%d.png", seasonId,sectionId);//season1_sectionId1.png
//
//	CCSprite* lable = CCSprite::create(ResManager::getManager()->getSharedFilePath(str).c_str());
//	lable->setPosition(CCPointMake(button->boundingBox().size.width/2, button->boundingBox().size.height/2));
//	button->addChild(lable);
//
//	if (!canChoose)
//	{//加锁
//        
//        char lockFile[128];
//        sprintf(lockFile, "section_lock_%d.png",seasonId);
//        
//		CCSprite* lock = CCSprite::create(ResManager::getManager()->getSharedFilePath(lockFile).c_str());
//		lock->setPosition(ccp(44,2));
//		button->addChild(lock);
//	}
//}

void SectionItem::initButton(SectionItem* button,bool canChoose)
{
	button->canChoose = canChoose;
	ResManager* resMgr = ResManager::getManager();
	
	if (!canChoose)
	{
		initWithFile(resMgr->getSharedFilePath(g_seasonlayerPath+"sectionItem_lock_bg.png").c_str());
		//屏蔽剧情道具相关
/*		JuqingDaoJu data;
		if (JuqingDaoJuManager::getManager()->getItemInSection(seasonId, sectionId, data))
		{
			CCSprite *img = CCSprite::create("sectionItem_daoju_weihuode.png");
			addChild(img);
			img->setPosition(ccp(getContentSize().width-7, getContentSize().height));
		}
*/
	}else
	{
		char fileName[128];
		sprintf(fileName, (resMgr->getSharedFilePath()+g_seasonlayerPath+"sectionItem_%d_bg.png").c_str(),button->currentScore);
		initWithFile(fileName);
		if (button->currentScore == 0)
		{
			CCSprite *ef = CCSprite::create(resMgr->getSharedFilePath(g_seasonlayerPath+"sectionItem_new_bg.png").c_str());
			addChild(ef);
			ef->setPosition(ccp(ef->getContentSize().width/2, ef->getContentSize().height/2 - 3));
			ef->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(1.5f), CCFadeIn::create(1.5f), NULL)));
		}
		//屏蔽剧情道具相关
/*		JuqingDaoJu data;
		if (JuqingDaoJuManager::getManager()->getItemInSection(seasonId, sectionId, data) && data.state != DJS_NotGetted)
		{
			CCSprite *img = CCSprite::create(resMgr->getSharedFilePath(g_daojuPath+data.fileName).c_str());
			addChild(img);
			img->setPosition(ccp(getContentSize().width/2, 120));
			img->setScale(0.75f);
		}
		*/
		if (false)
		{
		}
		else 
		{
	
			CCLabelAtlas* lable = CCLabelAtlas::create(
				"0123456789",
				resMgr->getSharedFilePath(g_seasonlayerPath+"season_section_num.png").c_str(),
				40, 49,//24
				'0');
			addChild(lable);
			char id[5];
			sprintf(id,"%d",button->sectionId);
			lable->setString(id);
			lable->setPosition(ccp(getContentSize().width/2 - lable->getContentSize().width/2,90));
			
			//屏蔽剧情道具相关
/*			if (JuqingDaoJuManager::getManager()->getItemInSection(seasonId, sectionId, data) && data.state == DJS_NotGetted)
			{
				CCSprite *img = CCSprite::create("sectionItem_daoju_weihuode.png");
				addChild(img);
				img->setPosition(ccp(getContentSize().width-5, getContentSize().height+10));

				CCAnimation *animation = CCAnimation::create();
				for (int idx=1; idx<=16; ++idx){
					animation->addSpriteFrameWithFileName(CCString::createWithFormat("sectionItem_daoju_weihuode_%d.png", idx)->getCString());
				}
				animation->setDelayPerUnit(0.02f);
				img->runAction(CCRepeatForever::create(CCSequence::create(CCAnimate::create(animation), CCDelayTime::create(1), NULL)));
			}
*/
		}
	}
}

void SectionItem::setLevelSelectListener(LevelSelectListener* levelSelectListener)
{
	this->listener = levelSelectListener;
}

void SectionItem::buyOnePlayTimeSuc(CCObject* alert)
{
	if (listener)
	{
		listener->sectionSelected(sectionId,getPosition());
	}
}
void SectionItem::buySectionWithMoney(CCObject* payRspWrapper)
{
	buyOnePlayTimeSuc(NULL);
}
void SectionItem::setScale(float fScale){
	CCSprite::setScale(Section_Item_Scale*fScale);
}
float SectionItem::getScale(){
	return CCSprite::getScale() / Section_Item_Scale;
}
void SectionItem::clickedMe()
{
	Music::sharedMusicPlayer()->playEffectSound("section_clicked.wav");
    ClickableItem::clickedMe();
	if (listener != NULL && canChoose)
	{
		int index = sectionId + ( seasonId - 1 ) * GameLayerFactory::getInstance()->getAllSectionNumBySeasonId(seasonId);
		
		if (GameInforEditer::getGameInfor()->isChallengeSection(sectionId, seasonId))
		{
			LFAlerts_UnLockSeason *buy = LFAlerts_UnLockSeason::showAlert(index);
			if (buy != NULL)
			{
				return ;
			}
		}
/*		// 如果没有买过，并且是第一次玩，并且没有提示过，那么就提示购买
#define ALTER_BUY_GAME_SHOWED	"ALTER_BUY_GAME_SHOWED"
		if (seasonId == 2 && sectionId == 1)
		{
			bool showed = false;
			History::getHistory()->getBoolFromApp(ALTER_BUY_GAME_SHOWED,showed);
			SectionInfor* sectionInfor = GameInforEditer::getGameInfor()->getSectionInfor(seasonId,sectionId);
			if (!showed && sectionInfor->isPlayed == false && !GameInforEditer::getGameInfor()->isBuyedSeason(seasonId))
			{
				History::getHistory()->putInApp(ALTER_BUY_GAME_SHOWED,true);
				History::getHistory()->save(); 
				LFAlert_BuyGame::showAlert();
				return;
			}
		}*/
		


		//if (LFAlert_FreeTimeOut::isTimeToShowBeforePlay(seasonId,sectionId))
		//{
		//	LFAlert_FreeTimeOut::showAlert(seasonId,sectionId)->
		//		addAutoRemoveObserver(this,SEL_CallFuncO(&SectionItem::buySectionWithMoney),MSG_PP_SectionTimeLimiteRsp)->
		//		addAutoRemoveObserver(this,SEL_CallFuncO(&SectionItem::buyOnePlayTimeSuc),BUY_ONE_PLAY_TIME_SUC);
		//	return;
		//}

		//if (LFAlert_WaitTimeOver::isTimeToShowBeforePlay(seasonId,sectionId))
		//{
		//	LFAlert_WaitTimeOver::showAlert(seasonId,sectionId);
		//	GameInforEditer::getGameInfor()->startSectionTimeCounter(seasonId,sectionId);
		//	return;
		//}

		listener->sectionSelected(sectionId,getPosition());
	}
}