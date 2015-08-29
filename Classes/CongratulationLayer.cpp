//
//  CongratulationLayer.cpp
//  demo-box2d
//
//  Created by Davidloeng loeng on 12-6-27.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "CongratulationLayer.h"
#include "BaseScense.h"
#include "GameScenseDefine.h"
#include "GameLayerFactory.h"
#include "LevelManager.h"
#include "ResManager.h"
#include "GameInforEditer.h"
#include "Runtime.h"
#include "LevelManager.h"
#include "MainFrameLayer.h"
#include "SpriteHelper.h"
#include "LoadingLayer.h"
#include "Music.h"
#include "LFShareKit.h"
#include "VersionControl.h"
#include "CurtainLayer.h"
#include "DoorLayer.h"
#include "LFAlerts.h"
#include "UserDataManager.h"
#include "font.h"
#include "BaseContentWrapLayer.h"
#if (Version_Lite == 1)
#define MIN_SUCESS_NUM_FOR_COMMENTS 0
#define SectionNumForComments 15 //13关之后就要求评价
#else
#define MIN_SUCESS_NUM_FOR_COMMENTS 5
#define SectionNumForComments 25 //25关之后就要求评价
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//#include "protocol.h"
extern unsigned char g_AppStoreUrlType;
extern std::string g_AppStoreUpdateCommentMsg;
std::string g_AppStoreUpdateCommentUrl = "";


#endif

static int sucessedNum = 0;

static bool showAdFlagInited = false;
// 128 足够了，每一章目前最多才20关，一共是100关以后也不会多余128关的
static bool showAdFlag[128] = {false};
static int currentIndex = 0;
static int showAdArrayLength = sizeof(showAdFlag)/sizeof(bool);
// 刷新showAdFlag的数据
// numForShowAd 指定多少关就会显示一个广告，也必然是这么多关里面一定会显示广告
static void adFlagRefresh(int numForShowAd)
{
	if (numForShowAd <=0 )
		numForShowAd = 1;
	
	// 先随机生成数据
	int arrayLength = sizeof(showAdFlag)/sizeof(bool);
	float percent = 1.0f/((float)numForShowAd);
	for (int i=0;i<arrayLength;i++)
	{
		showAdFlag[i] = (CCRANDOM_0_1() <= percent);
	}
	
	bool haveAd = false;
	// 修复，规格化
	for (int i=0;i<arrayLength;)
	{
		haveAd = false;
		for (int j=0;j<numForShowAd;j++)
		{
			// 发现true了，那么就直接把后面的设置为false
			if(showAdFlag[i+j])
			{
				haveAd = true;
				for (int h=1;h<numForShowAd - j;h++)
				{
					showAdFlag[i+j+h] = false;
				}
				break;
			}
		}

		// 没有true，那么随机选一个当做true
		if (!haveAd)
		{
			int addIndex = (int)(((float)numForShowAd) * CCRANDOM_0_1());
			showAdFlag[i + addIndex] = true;
		}

		i += numForShowAd;
		
	}
	
	// log
	{
		CCLog("====================showAdFlag=============================");
		for(int i=0;i<arrayLength;i++)
		{
			CCLog("showAdFlag[%d] = [%s]",i,showAdFlag[i]?"TRUE":"FALSE");
		}
		CCLog("====================showAdFlag=============================");
	}
	
}

CongratulationLayer::CongratulationLayer(int scenseIndex,int oldStarNum)
: m_nMenuItemCount(0)
{    
	container = CCSprite::create();
	currentIndex = scenseIndex;
    mScreenSize = CCDirector::sharedDirector()->getWinSize();
	container->setContentSize(mScreenSize);
	addChild(container,10);
	container->setPosition(ccp(mScreenSize.width/2,mScreenSize.height/2));

	GameLayerFactory* layerFactory = GameLayerFactory::getInstance();
	std::pair<int, int> info1 = layerFactory->getLevelFormat(scenseIndex);

	mSeasonId = info1.first;
	mSectionId = info1.second;
    score = 0;
	sucessedNum ++;
	mGameScore = Runtime::getRuntime()->getGameScore();
	//记录本关时间
    SectionInfor* info = GameInforEditer::getGameInfor()->getSectionInfor(mSeasonId, mSectionId);
	if (info)
	{
		if (!info->isSuccessed)
		{
			info->fFirstCost = mGameScore.timeCost;
		}
		//保存更优的数据
		if (info->isBetterThanMe(mGameScore.timeCost, mGameScore.effectiveClickedNum, mGameScore.clickStar, mGameScore.isUsedSuperTool))
		{
			info->fTimeCost = mGameScore.timeCost;
			info->effectiveClickNum = mGameScore.effectiveClickedNum;
			info->iStarNum = mGameScore.clickStar;

			if (Recorder::getInstance()->setOneSectionRecord(GameLayerFactory::getInstance()->getSectionIndex(mSeasonId,mSectionId), 
				BaseContentLayer::shareBaseContentLayer()->getAllFramesData()))
			{
				CCLog("record OK");
			}
			else
			{
				CCLog("record Error");
			}
		}
	}
	GameInforEditer::getGameInfor()->saveSessonInforList();
	if (!showAdFlagInited)
	{
		adFlagRefresh(4);
		showAdFlagInited = true;
	}

//     TaskInfo taskinfo;
//     taskinfo.star = mGameScore.getStarNum();
//     taskinfo.task_id = (mSeasonId-1)*PerGuanKaCount+mSectionId;
//     CmdHelper::getHelper()->cmdSaveTaskInfoDataToServer(taskinfo);

    nextToSelector = false;
	
	ResManager* resMgr = ResManager::getManager();

	{
		Music::sharedMusicPlayer()->loadEffectSound("star1.wav");	
		Music::sharedMusicPlayer()->loadEffectSound("star2.wav");	
		Music::sharedMusicPlayer()->loadEffectSound("star3.wav");	
		Music::sharedMusicPlayer()->loadEffectSound("laugh.wav");
	}

	showBoarder();

// 	//20140710 scf 新需求：在成功界面或刷新关卡的时机：如果距离上次出广告超过3分钟，则弹出一次广告
// 	//2.每5小关关通关后，弹出插屏广告
// 	AdManager::getManager()->showAdByTimeDelay();
	menu = CCMenu::create();
	menu->setPosition(CCPointZero);
	container->addChild(menu,5);
	menu->setEnabled(false);

	//结算界面的背景需要增加一层黑色透明背景10%（25.5）
	CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 25.5f));
	this->addChild(pLayerColor, -1);
	pLayerColor->setOpacity(0.0f);
	pLayerColor->runAction(CCFadeTo::create(0.3f, 25.5f));
}
void CongratulationLayer::showMenuItem(float dt)
{
	m_nMenuItemCount++;
	ResManager* resMgr = ResManager::getManager();
	//界面按钮
	{
		float posX = mScreenSize.width/2;
		float posY = mScreenSize.height/2 - 20;
		float xDistance = 150;
		float yDistance = 150;
		if (m_nMenuItemCount == 1)
		{
			CCMenuItemImage* itemRestart = CCMenuItemImage::create(
				resMgr->getSharedFilePath(g_buttonPath+"congratulation_restart.png").c_str(),
				resMgr->getSharedFilePath(g_buttonPath+"congratulation_restart_select.png").c_str(),
				this,SEL_MenuHandler(&CongratulationLayer::restartItemPressed));
			menu->addChild(itemRestart);
			itemRestart->setPosition(ccp(posX - itemRestart->getContentSize().width / 2 - xDistance, posY - itemRestart->getContentSize().height / 2 - yDistance));
		}
		else if (m_nMenuItemCount == 2)
		{
			CCMenuItemImage* itemMain = CCMenuItemImage::create(
				resMgr->getSharedFilePath(g_buttonPath+"congratulation_main.png").c_str(),
				resMgr->getSharedFilePath(g_buttonPath+"congratulation_main_select.png").c_str(),
				this,SEL_MenuHandler(&CongratulationLayer::mainItemPressed));
			menu->addChild(itemMain);
			itemMain->setPosition(ccp(posX, posY - itemMain->getContentSize().height / 2 - yDistance));
		}
		else if (m_nMenuItemCount == 3)
		{
			CCMenuItemImage* itemNext = CCMenuItemImage::create(
				resMgr->getSharedFilePath(g_buttonPath+"congratulation_next.png").c_str(),
				resMgr->getSharedFilePath(g_buttonPath+"congratulation_next_select.png").c_str(),
				resMgr->getSharedFilePath(g_buttonPath+"congratulation_next_select.png").c_str(),
				this,SEL_MenuHandler(&CongratulationLayer::nextItemPressed));
			menu->addChild(itemNext);
			itemNext->setPosition(ccp(posX + itemNext->getContentSize().width / 2 + xDistance, posY - itemNext->getContentSize().height / 2 - yDistance));
			menu->setEnabled(true);

			this->unschedule(SEL_SCHEDULE(&CongratulationLayer::showMenuItem));
		}
	}
}
void CongratulationLayer::onEnter()
{
	CCLayer::onEnter();
}
void CongratulationLayer::onExit()
{
	CCLayer::onExit();
}


CongratulationLayer::~CongratulationLayer(){
}

void CongratulationLayer::showBoarder()
{
	ResManager* resMgr = ResManager::getManager();
	CCSprite* starBoarder = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+"congratulation_star_board.png").c_str());
	container->addChild(starBoarder);
	starBoarder->setOpacity(0.0f);
	starBoarder->setPosition(ccp(mScreenSize.width/2, 600));
	starBoarder->runAction(CCSequence::create(
		CCFadeIn::create(0.3f),
		CCCallFuncN::create(this, callfuncN_selector(CongratulationLayer::showZiTi)),
		NULL));

// 	CCSprite* bg = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath + "congratulation_texiao_xiangsu.png").c_str());
// 	starBoarder->addChild(bg, -1);
// 	bg->setPosition(starBoarder->getContentSize()/2);
// 
// 	bg = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath + "congratulation_texiao_01.png").c_str());
// 	starBoarder->addChild(bg, -1);
// 	bg->setPosition(starBoarder->getContentSize() / 2);
// 

	CCArmature* pArmature = CCArmature::create("Star_UI_jiesuan");
	pArmature->getAnimation()->play("jiesuan_guangxiao");
	starBoarder->addChild(pArmature, -1);
	pArmature->setPosition(ccp(starBoarder->getContentSize().width/2, starBoarder->getContentSize().height/2));
	pArmature->setScale(2.0f);
}

void CongratulationLayer::showZiTi(CCNode* pBoard)
{
	CCLabelAtlas* pPingJia = CCLabelAtlas::create(
		"Level completed",
		ResManager::getManager()->getSharedFilePath(g_gameinPath+"ziti_01.png").c_str(),
		29, 41,
		' ');
	pPingJia->setAnchorPoint(ccp(0.5f, 0.5f));
	container->addChild(pPingJia);
	pPingJia->setPosition(ccp(pBoard->getPosition().x, pBoard->getPositionY() + 10));
	pPingJia->setOpacity(0.0f);

	if (mGameScore.clickStar == 3)
	{
		CCLabelAtlas* pWanMei = CCLabelAtlas::create(
			"Prefect!",
			ResManager::getManager()->getSharedFilePath(g_gameinPath+"ziti_01.png").c_str(),
			29, 41,
			' ');
		pWanMei->setAnchorPoint(ccp(0.5f, 0.5f));
		container->addChild(pWanMei);
		pWanMei->setOpacity(0.0f);
		pWanMei->setPosition(ccp(mScreenSize.width/2, mScreenSize.height/2 + pWanMei->getContentSize().height + 50));
		pWanMei->runAction(CCSequence::create(
			CCFadeIn::create(0.3f),
			NULL));

		CCArmature* pArmature = CCArmature::create("xuhualizi");
		pArmature->getAnimation()->play("xuehua001");
		this->addChild(pArmature, 100);
		pArmature->setPosition(ccp(mScreenSize.width/2, mScreenSize.height/2 + 50));

//		createPar(ccp(mScreenSize.width / 2, mScreenSize.height + 50), "card.png", container->getContentSize().width, 160);
	}

	pPingJia->runAction(CCSequence::create(
		CCFadeIn::create(0.3f),
		CCCallFuncN::create(this, callfuncN_selector(CongratulationLayer::showZiTiDone)),
		NULL));

	GameInforEditer::getGameInfor()->saveSessonInforList();
}

void CongratulationLayer::showZiTiDone(CCNode* pNode)
{
	this->schedule(schedule_selector(CongratulationLayer::showMenuItem), 0.3f);
}

void CongratulationLayer::restartItemPressed(cocos2d::CCObject *obj){
	Music::sharedMusicPlayer()->buttonClicked();
//	LevelManager::getManager()->retry(true);
	
	CCArray* children = getChildren();
	int num = children->count();
	for (int i = 0; i < num; i++)
	{
		CCNode* temp = (CCNode*)children->objectAtIndex(i);
		temp->setVisible(false);
	}
	LevelManager::getManager()->setShowLoadingAnimation(true);
	LevelManager::getManager()->goTo(LevelManager::getManager()->getLastSeasonId(), LevelManager::getManager()->getLastSectionId());

}

void CongratulationLayer::mainItemPressed(CCObject* obj)
{
	Music::sharedMusicPlayer()->buttonClicked();
	CCArray* children = getChildren();
	int num = children->count();
	for (int i=0;i<num;i++)
	{
		CCNode* temp = (CCNode*)children->objectAtIndex(i);
		temp->setVisible(false);
	}

	BaseScense::getCurrentScene()->getBackgroudCoverLayer()->setVisible(false);
	children = BaseScense::getCurrentScene()->getBaseContentWrapLayer()->getChildren();
	num = children->count();
	for (int i=0;i<num;i++)
	{
		CCNode* temp = (CCNode*)children->objectAtIndex(i);
		temp->setVisible(false);
	}

	CCScene* currentScene = CCDirector::sharedDirector()->getRunningScene();
	currentScene->removeChildByTag(Tag_Layer_Background,true);
	currentScene->removeChildByTag(Tag_Layer_shadow,true);
	currentScene->removeChildByTag(Tag_Layer_BaseContent,true);
	currentScene->removeChildByTag(Tag_Layer_Notification,true);

	LoadingLayer* layer = new LoadingLayer(Intent_GameIn2Section);
	CCDirector::sharedDirector()->getRunningScene()->addChild(layer,getZOrder()+1);
	layer->release();
}
void CongratulationLayer::menuItemPressed(CCObject* obj)
{
	Music::sharedMusicPlayer()->buttonClicked();
}

void CongratulationLayer::nextItemPressed(cocos2d::CCObject *obj){

	CCMenuItem *item = (CCMenuItem *)obj;
	item->setEnabled(false);
	Music::sharedMusicPlayer()->buttonClicked();
	if (nextToSelector)
	{
		LoadingLayer* layer = new LoadingLayer(Intent_GameIn2Season);
		CCDirector::sharedDirector()->getRunningScene()->addChild(layer,getZOrder()+1);
		layer->release();
		//LevelManager::getManager()->goToSeasonUI();
		nextToSelector = false;
	}else
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS && Version_Platform == 0) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && Version_Platform == 5) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		GameLayerFactory* layerFactory = GameLayerFactory::getInstance();
		int lastSeasonId = LevelManager::getManager()->getLastSeasonId();
		int lastSectionId = LevelManager::getManager()->getLastSectionId();
		int nextSeasonId = layerFactory->getNextSeasonId(lastSeasonId,lastSectionId);
		int nextSectionId = layerFactory->getNextSectionId(lastSeasonId,lastSectionId);

//		int index = nextSectionId + ( nextSeasonId - 1 ) * GameLayerFactory::getInstance()->getSectionNumBySeasonId(nextSeasonId);
		if (GameInforEditer::getGameInfor()->isChallengeSection(nextSeasonId, nextSectionId))
		{
			mainItemPressed(NULL);
			return ;
		}
#endif
		LevelManager::getManager()->setShowLoadingAnimation(true);
		LevelManager::getManager()->nextSection();
	}
	CCArray* children = getChildren();
	int num = children->count();
	for (int i=0;i<num;i++)
	{
		CCNode* temp = (CCNode*)children->objectAtIndex(i);
		temp->setVisible(false);
	}

}

void CongratulationLayer::setNextButtonToSelector()
{
	nextToSelector = true;
}

CCParticleSystem* CongratulationLayer::createPar(CCPoint pos,const char* file,float width,float height)
{
	CCParticleSystem *m_emitter = new CCParticleSnow();
	m_emitter->initWithTotalParticles(2000);
	addChild(m_emitter, 10);

	m_emitter->setPosition(pos);
	m_emitter->setPosVar(ccp(width,height));
	m_emitter->setLife(0.5f);
	m_emitter->setLifeVar(0.5f);
	m_emitter->setAutoRemoveOnFinish(true);
	m_emitter->setDuration(0.5f);
	m_emitter->setGravity(CCPointMake(0,-10));
	m_emitter->setSpeed(400);
	m_emitter->setSpeedVar(10);
	m_emitter->setStartColor(ccc4f(0.5f,0.5f,0.5f,1.0f));
	m_emitter->setStartColorVar(ccc4f(0.5f,0.5f,0.5f,0.1f));
	m_emitter->setEndColor(ccc4f(0.1f,0.1f,0.1f,0.2f));
	m_emitter->setEndColorVar(ccc4f(0.1f,0.1f,0.1f,0.2f));
	m_emitter->setEndSpinVar(360);
	m_emitter->setEmissionRate(m_emitter->getTotalParticles()/m_emitter->getLife());
	m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage(ResManager::getManager()->getSharedFilePath(g_gameinPath+file).c_str()));
	m_emitter->setBlendAdditive(true);

	return m_emitter;
}