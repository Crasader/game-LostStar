//
//  MenuLayer.cpp
//  demo-box2d
//
//  Created by Davidloeng loeng on 12-6-26.
//  Copyright (c) 2012? __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "MenuLayer.h"
#include "BaseScense.h"
#include "GameScenseDefine.h"
#include "LevelManager.h"
#include "ResManager.h"
#include "MainFrameLayer.h"
#include "GameInforEditer.h"
#include "GameLayerFactory.h"
#include "SpriteHelper.h"
#include "LoadingLayer.h"
#include "AchievementManager.h"
#include "Music.h"
#include "BackgroudLayerFactory.h"
#include "Debug.h"
#include "VersionControl.h"
#include "DoorLayer.h"
#include "font.h"
#include "LFStrings.h"
#include "MarketLayer.h"
#include "IOSStoreLayer.h"
#include "IOSMarketUseSuperToolLayer.h"
#include "UserDataManager.h"
#include "Debug.h"
#include "MessageDisplay.h"
#include "VersionControl.h"
#include "History.h"
#include "Stopgame.h"
#include "Uimsg.h"
#include "Myobject.h"
#include "Stopgameright.h"
#include "AppMacros.h"
#include "CmdHelper.h"
#include "Chaonengdaoju.h"
#include "Goluetu.h"

#define SHOW_NEXT_PREV_BTN			0

MenuLayer* MenuLayer::instance;
float  MenuLayer::time_show=0.25f;

#define talk_btn_tag	324
#define TAG_HELP   22222
#define TAG_HELP_BG	22223
#define DisplaySuperTool_RestartTimes 5   //未通关 点击第 XX 次restart就提示使用超能力道具
#define DisplayGuide_RestartTimes 5   //未达3星 点击第 XX 次restart就提示看攻略

int MenuLayer::mLastSeasonId = 0;
int MenuLayer::mLastSectionId = 0;
int MenuLayer::reStarNum = 0;
bool MenuLayer::needHelpItemAction = true;
bool MenuLayer::needSuperToolAction = true;

// 记录，用于防止崩溃的bug
static bool isPaused = false;


void MenuLayer::Huidoanmenu(CCObject* object){

	CCLOG("menulayer----->huidian");
	int oprate_tag=(int)(long long)object;
	if(oprate_tag==Gamecontinu_tag){//menuchuxian 

		this->setGameMenuEnable(true);
		//stop3->removeFromParentAndCleanup(true);
		CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(Tag_StopGame3Layer);
		BaseScense::getCurrentScene()->setCollisionEnable(true);
		this->setTouchEnabled(true);
	}else if(oprate_tag==Gameother_tag){

		
	}






}

MenuLayer::MenuLayer(int scenseIndex){
	
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(MenuLayer::Huidoanmenu), MSG_GAME_NEI, NULL);

	starstate = NULL;
	m_lMaskLayer = NULL;
	m_bIsTeachUseSuperTool = false;
	instance = this;
	tempLastBackToMain = false;
	m_index = 0;
	GameLayerFactory* layerFactory = GameLayerFactory::getInstance();
	std::pair<int, int> info1 = layerFactory->getLevelFormat(scenseIndex);
	seasonId = info1.first;
	sectionId = info1.second;

    mScreenSize = CCDirector::sharedDirector()->getWinSize();
	addItemLayer = NULL;
	m_superToolItem = NULL;

	canJumpLevel = false;
	setTouchEnabled(false);
	isPaused = false;
	layer = NULL;
	
	// BUG 2013-5-3 修改不断restart,help的崩溃bug
	scene = NULL;

	{
		if (mLastSeasonId == seasonId && mLastSectionId == sectionId)
		{
			reStarNum ++;

			if (reStarNum % DisplaySuperTool_RestartTimes == 0)
				needSuperToolAction = true;
			if (reStarNum % DisplayGuide_RestartTimes == 0 )//&& !needSuperToolAction)
				needHelpItemAction = true;
		}else
		{
			//首次初始化
			reStarNum = 0;
			mLastSeasonId = seasonId;
			mLastSectionId = sectionId;
			needSuperToolAction = false;
			needHelpItemAction = false;
		}
	}

	ResManager* resManager = ResManager::getManager();

		CCMenuItemImage* pauseItem = CCMenuItemImage::create(
			resManager->getSharedFilePath(g_buttonPath+"menu.png").c_str(),
			resManager->getSharedFilePath(g_buttonPath+"menu_select.png").c_str(),
			this,SEL_MenuHandler(&MenuLayer::pauseItemPressed));
		m_restartItem = CCMenuItemImage::create(
			resManager->getSharedFilePath(g_buttonPath+"restart.png").c_str(),
			resManager->getSharedFilePath(g_buttonPath+"restart_select.png").c_str(),
			this,SEL_MenuHandler(&MenuLayer::restartItemPressed));

		CCMenuItemImage* packageItem = CCMenuItemImage::create(
			resManager->getSharedFilePath(g_buttonPath+"packsack.png").c_str(),
			resManager->getSharedFilePath(g_buttonPath+"packsack_select.png").c_str(),
			this,SEL_MenuHandler(&MenuLayer::packageItemPressed));

		//add by jiangjie
//		CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MenuLayer::talkLoginEnd), MSG_SDKLoginRsp, NULL);
		CCMenuItemImage* talkItem = CCMenuItemImage::create(
			resManager->getSharedFilePath(g_buttonPath+"liaotian.png").c_str(),
			resManager->getSharedFilePath(g_buttonPath+"liaotian_select.png").c_str(),
			this,SEL_MenuHandler(&MenuLayer::talkItemPressed));
		talkItem->setTag(talk_btn_tag);
		talkItem->setVisible(false);


#if CMGE_TALK_ENABLE != 0
            if (!UserDataManager::getInstance()->isSdkLogin())
            {
                talkItem->setVisible(true);
            }
#endif

		//end
		
		gameMenu = CCMenu::create(pauseItem,m_restartItem,packageItem,talkItem,NULL);
		gameMenu->setPosition(CCPointZero);
		addChild(gameMenu);
		gameMenu->retain();

		pauseItem->setPosition(ccp(46,/*mScreenSize.height - */pauseItem->getContentSize().height/2 - 15));
		m_restartItem->setPosition(ccp(mScreenSize.width-46,pauseItem->getPositionY()));
		packageItem->setPosition(ccp(mScreenSize.width-50,mScreenSize.height - 150));

		m_gongluetuItem = CCMenuItemImage::create(
			resManager->getSharedFilePath(g_stop+"ZT_anniu_gongluetu.png").c_str(),
			resManager->getSharedFilePath(g_stop+"ZT_anniu_gongluetu.png").c_str(),
			this,SEL_MenuHandler(&MenuLayer::helpItemPressed));
		gameMenu->addChild(m_gongluetuItem);
		m_gongluetuItem->setScale(0.8f);
		m_gongluetuItem->setPosition(ccp(mScreenSize.width - m_gongluetuItem->getContentSize().width/2*m_gongluetuItem->getScale() - 15, mScreenSize.height - m_gongluetuItem->getContentSize().height/2*m_gongluetuItem->getScale() - 15));

		m_superToolItem = CCMenuItemImage::create(
			resManager->getSharedFilePath(g_stop+"ZT_anniu_chaonengdaoju.png").c_str(),
			resManager->getSharedFilePath(g_stop+"ZT_anniu_chaonengdaoju.png").c_str(),
			this,SEL_MenuHandler(&MenuLayer::superToolItemPressed));
		gameMenu->addChild(m_superToolItem);
		m_superToolItem->setScale(0.8f);
		m_superToolItem->setPosition(ccp(m_superToolItem->getContentSize().width/2*m_superToolItem->getScale() + 15, m_gongluetuItem->getPositionY()));

		CCMenuItemImage* nextItem = CCMenuItemImage::create(
			resManager->getSharedFilePath(g_buttonPath+"guide_next.png").c_str(),
			resManager->getSharedFilePath(g_buttonPath+"guide_next_select.png").c_str(),
			this,SEL_MenuHandler(&MenuLayer::nextSection));
		gameMenu->addChild(nextItem);
		nextItem->setScale(0.5f);
		nextItem->setPosition(ccp(mScreenSize.width - nextItem->getContentSize().width/2*nextItem->getScale(), mScreenSize.height - nextItem->getContentSize().height/2*nextItem->getScale() - 65));

		CCMenuItemImage* preScetionItem = CCMenuItemImage::create(
			resManager->getSharedFilePath(g_buttonPath+"seasonItem_buttom_back.png").c_str(),
			resManager->getSharedFilePath(g_buttonPath+"seasonItem_buttom_back_select.png").c_str(),
			this,SEL_MenuHandler(&MenuLayer::preSection));
		gameMenu->addChild(preScetionItem);
		preScetionItem->setScale(0.65f);
		preScetionItem->setPosition(ccp(preScetionItem->getContentSize().width/2*preScetionItem->getScale(), mScreenSize.height - preScetionItem->getContentSize().height/2*preScetionItem->getScale() - 65));

#if SHOW_NEXT_PREV_BTN == 0
		nextItem->setVisible(false);
		preScetionItem->setVisible(false);
#endif
#ifndef DEBUG_ENABLE_PACKAGE
		packageItem->setVisible(false);
		packageItem->setEnabled(false);
#endif



// 开启了debug的话，就不需要有隐藏Help的代码了。
#ifndef DEBUG_ENABLE_ALLWAYS_SHOW_HELP
		// 控制10次以下 隐藏帮助按钮
		/*if (reStarNum < 10)
		{
			helpItem->setVisible(false);
			helpItem->setEnabled(false);
		}else
		{
			if (needHelpItemAction)
			{
				helpItem->setOpacity(0);
				helpItem->runAction(CCSequence::create(
					CCDelayTime::create(0.5f),
					CCFadeTo::create(0.3f,255),
					CCRepeat::create(CCSequence::create(
					CCScaleTo::create(0.2f,0.7f),
					CCScaleTo::create(0.2f,1.3f),
					NULL),3),
					CCRepeat::create(CCSequence::create(
					CCRotateTo::create(0.1f,30.0f),
					CCRotateTo::create(0.1f,-30.0f),
					NULL),3),
					CCRotateTo::create(0.1f,0.0f),
					CCScaleTo::create(0.1f,1.0f),
					NULL));

				needHelpItemAction = false;
			}
		}*/

	CCSize size = CCEGLView::sharedOpenGLView()->getFrameSize();
	CCLabelTTF* want_solution = NULL;
	if (reStarNum >= DisplayGuide_RestartTimes)
	{
		SectionInfor* infor = GameInforEditer::getGameInfor()->getSectionInfor(seasonId,sectionId);
		// 玩了3次了，还没得到过3星就提示
		if (infor!=NULL && infor->iStarNum < 3 && needHelpItemAction)
		{
			float scale1 = m_gongluetuItem->getScale();
			if ((int)size.width % (int)designResolutionSize.width == 0 
				&& (int)size.height % (int)designResolutionSize.height == 0 )
			{
				scale1 = 0.52f;
			}
			m_gongluetuItem->setOpacity(0);
			m_gongluetuItem->runAction(CCSequence::create(
				CCDelayTime::create(0.5f),
				CCFadeTo::create(0.3f, 255),
				CCRepeat::create(CCSequence::create(
				CCScaleTo::create(0.2f, scale1-0.3f),
				CCScaleTo::create(0.2f, scale1+0.3f),
				NULL),3),
				CCRepeat::create(CCSequence::create(
				CCRotateTo::create(0.1f,30.0f),
				CCRotateTo::create(0.1f,-30.0f),
				NULL),3),
				CCRotateTo::create(0.1f, 0.0f),
				CCScaleTo::create(0.1f, scale1),
				NULL));

			want_solution = CCLabelTTF::create(
				LFStrings::getValue("want_soloution").c_str(),
				fontStr_KlavikaBold, 30);
			want_solution->setAnchorPoint(ccp(1.0f, 0.5f));
			want_solution->setOpacity(0);
			want_solution->setPosition(ccp(
				m_gongluetuItem->getPositionX()-m_gongluetuItem->getContentSize().width/2,
				m_gongluetuItem->getPositionY()));
			addChild(want_solution);
			want_solution->runAction(CCSequence::create(
				CCDelayTime::create(1.4f),
				CCFadeTo::create(0.3f,255),
				CCDelayTime::create(3.5f),
				CCFadeTo::create(0.2f,0),
				NULL
				));

			needHelpItemAction = false;
		}
	}

	if ((int)size.width % (int)designResolutionSize.width == 0 
		&& (int)size.height % (int)designResolutionSize.height == 0 )
	{
		m_gongluetuItem->setScale(0.52f);
		m_superToolItem->setScale(m_gongluetuItem->getScale());
		nextItem->setPosition(ccp(mScreenSize.width - nextItem->getContentSize().width/2*nextItem->getScale(), mScreenSize.height - nextItem->getContentSize().height/2*nextItem->getScale() - 5));
		preScetionItem->setPosition(ccp(preScetionItem->getContentSize().width/2*preScetionItem->getScale(), mScreenSize.height - preScetionItem->getContentSize().height/2*preScetionItem->getScale() - 5));

		m_superToolItem->setPosition(ccp(pauseItem->getPositionX() + pauseItem->getContentSize().width/2 + m_superToolItem->getContentSize().width/2*m_superToolItem->getScale() + 10, pauseItem->getPositionY()));
		m_gongluetuItem->setPosition(ccp(m_superToolItem->getPositionX() + m_superToolItem->getContentSize().width/2*m_superToolItem->getScale() + m_gongluetuItem->getContentSize().width/2*m_gongluetuItem->getScale() + 20, m_superToolItem->getPositionY()));
	
		if (want_solution)
		{
			want_solution->setAnchorPoint(ccp(0.0f, 0.0f));
			want_solution->setPosition(ccp(m_gongluetuItem->boundingBox().getMaxX(), m_gongluetuItem->boundingBox().getMaxY() + 25));
		}
	}

	if (LevelManager::getManager()->getLastSeasonId() == 1 && LevelManager::getManager()->getLastSectionId() == 1)
	{
		m_superToolItem->setVisible(false);
		m_gongluetuItem->setVisible(false);

		if (want_solution)
		{
			want_solution->setVisible(false);
		}
	}
#endif

	{
// 		if (reStarNum >= DisplaySuperTool_RestartTimes)
// 		{
// 			SectionInfor* infor = GameInforEditer::getGameInfor()->getSectionInfor(seasonId,sectionId);
// 			// 完了5次还没成功
// 			if (infor!=NULL && infor->score <= 0 && needSuperToolAction)
// 			{
// 				superToolItem->setOpacity(0);
// 				superToolItem->runAction(CCSequence::create(
// 					CCDelayTime::create(0.5f),
// 					CCFadeTo::create(0.3f,255),
// 					CCRepeat::create(CCSequence::create(
// 					CCScaleTo::create(0.2f,0.7f),
// 					CCScaleTo::create(0.2f,1.3f),
// 					NULL),3),
// 					CCRepeat::create(CCSequence::create(
// 					CCRotateTo::create(0.1f,30.0f),
// 					CCRotateTo::create(0.1f,-30.0f),
// 					NULL),3),
// 					CCRotateTo::create(0.1f,0.0f),
// 					CCScaleTo::create(0.1f,1.0f),
// 					NULL));
// 
// 				CCLabelTTF* want_super = CCLabelTTF::create(
// 					LFStrings::getValue("want_supertool").c_str(),
// 					fontStr_katong,30,CCSizeMake(0,0),kCCTextAlignmentRight);
// 				want_super->setAnchorPoint(ccp(0.5f,0.5f));
// 				want_super->setOpacity(0);
// 				want_super->setPosition(ccp(
// 					superToolItem->getPositionX()+helpItem->getContentSize().width/2+want_super->getContentSize().width/2,
// 					superToolItem->getPositionY()));
// 				addChild(want_super);
// 				want_super->runAction(CCSequence::create(
// 					CCDelayTime::create(1.4f),
// 					CCFadeTo::create(0.3f,255),
// 					CCDelayTime::create(3.5f),
// 					CCFadeTo::create(0.2f,0),
// 					NULL
// 					));
// 
// 				needSuperToolAction = false;
// 			}
//		}
	}

// 轻量级的版本，不显示帮助按钮
#if Version_Lite == 1
	helpItem->stopAllActions();
	helpItem->setVisible(false);
	helpItem->setEnabled(false);
#endif

	{
		CCMenuItemImage* back = CCMenuItemImage::create(
			resManager->getSharedFilePath(g_buttonPath+"cross.png").c_str(),
			resManager->getSharedFilePath(g_buttonPath+"cross.png").c_str(),
			this,SEL_MenuHandler(&MenuLayer::pause_ResumeItemPressed));
		selectUIMenu = CCMenu::create(back,NULL);
		addChild(selectUIMenu,1);
		selectUIMenu->setVisible(false);
		selectUIMenu->setEnabled(false);
		selectUIMenu->setPosition(CCPointZero);
		back->setPosition(ccp(1000,750));
	}

	//暂停的界面
	{
		pauseBg = CCSprite::create(resManager->getSharedFilePath(g_mainlayerPath+"gray16.png").c_str(),CCRectMake(0,0,mScreenSize.width,mScreenSize.height));
		addChild(pauseBg);
		pauseBg->setPosition(ccp(mScreenSize.width/2,mScreenSize.height/2));  

		CCMenuItemImage* resumeItem = CCMenuItemImage::create(
			//resManager->getFilePathByLanguageType("congratulation_main.png").c_str(),
			//resManager->getFilePathByLanguageType("congratulation_main_select.png").c_str(),
			resManager->getSharedFilePath(g_buttonPath+"zanting_anniu_jixu.png").c_str(),
			resManager->getSharedFilePath(g_buttonPath+"zanting_anniu_jixu_select.png").c_str(),
			this,SEL_MenuHandler(&MenuLayer::pause_ResumeItemPressed));

		CCMenuItemImage* selectLevelItem = CCMenuItemImage::create(
			//resManager->getFilePathByLanguageType("zanting_anniu_jixu.png").c_str(),
			//resManager->getFilePathByLanguageType("zanting_anniu_jixu_select.png").c_str(),
			resManager->getSharedFilePath(g_buttonPath+"congratulation_main.png").c_str(),
			resManager->getSharedFilePath(g_buttonPath+"congratulation_main_select.png").c_str(),
			this,SEL_MenuHandler(&MenuLayer::pause_SelectLevelPressed));
		CCMenuItemImage* mainItem = CCMenuItemImage::create(
			//resManager->getFilePathByLanguageType("zanting_anniu_zhuye.png").c_str(),
			//resManager->getFilePathByLanguageType("zanting_anniu_zhuye_select.png").c_str(),
			resManager->getSharedFilePath(g_buttonPath+"zanting_anniu_zhuye.png").c_str(),
			resManager->getSharedFilePath(g_buttonPath+"zanting_anniu_zhuye_select.png").c_str(),
			this,SEL_MenuHandler(&MenuLayer::pause_MainPressed));


		CCMenuItemImage* itemEffctiveSound = NULL;
		CCMenuItemImage* itemBackgroundSound = NULL;
		//音效开关
		{
			if ( Music::sharedMusicPlayer()->isEffectiveMute() )
			{
				itemEffctiveSound = CCMenuItemImage::create(
					resManager->getSharedFilePath(g_buttonPath+"set_sound_off.png").c_str(),
					resManager->getSharedFilePath(g_buttonPath+"set_sound_off_select.png").c_str(),
					this,
					SEL_MenuHandler(&MenuLayer::effctiveSoundClicked));
			}else
			{
				itemEffctiveSound = CCMenuItemImage::create(
					resManager->getSharedFilePath(g_buttonPath+"set_sound_on.png").c_str(),
					resManager->getSharedFilePath(g_buttonPath+"set_sound_on_select.png").c_str(),
					this,
					SEL_MenuHandler(&MenuLayer::effctiveSoundClicked));
			}
		}

		//背景音乐开关
		{
			if ( Music::sharedMusicPlayer()->isBackgroundMute() )
			{
				itemBackgroundSound = CCMenuItemImage::create(
					resManager->getSharedFilePath(g_buttonPath+"set_music_off.png").c_str(),
					resManager->getSharedFilePath(g_buttonPath+"set_music_off_select.png").c_str(),
					this,
					SEL_MenuHandler(&MenuLayer::soundClicked));
			}else
			{
				itemBackgroundSound = CCMenuItemImage::create(
					resManager->getSharedFilePath(g_buttonPath+"set_music_on.png").c_str(),
					resManager->getSharedFilePath(g_buttonPath+"set_music_on_select.png").c_str(),
					this,
					SEL_MenuHandler(&MenuLayer::soundClicked));
			}
		}

		pauseMenu = CCMenu::create(resumeItem,selectLevelItem,mainItem,NULL);
		pauseMenu->addChild(itemEffctiveSound);
		pauseMenu->addChild(itemBackgroundSound);
		pauseMenu->setPosition(CCPointZero);
		addChild(pauseMenu);

		float startPosY = mScreenSize.height/2+selectLevelItem->getContentSize().height/2;
		float startPosX = mScreenSize.width/2;
		float distance = 50;

		resumeItem->setPosition(ccp(startPosX + resumeItem->getContentSize().width + distance, startPosY));
		selectLevelItem->setPosition(ccp(startPosX, startPosY));
		mainItem->setPosition(ccp(startPosX - mainItem->getContentSize().width -distance, startPosY));

		itemBackgroundSound->setPosition(ccp(startPosX - itemBackgroundSound->getContentSize().width/2 - distance/2, startPosY - itemBackgroundSound->getContentSize().height - distance));
		itemEffctiveSound->setPosition(ccp(startPosX + itemEffctiveSound->getContentSize().width/2 + distance/2, itemBackgroundSound->getPositionY()));
	}

	
	showGame();

	//scheduleOnce(schedule_selector(MenuLayer::showRannerAd),0.5f);
	//scheduleOnce(schedule_selector(MenuLayer::hideBannerAd),15);
	addCollectToolLayer();

	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
		SEL_CallFuncO(&MenuLayer::onMarketClosed), MSG_MarketClosed, NULL);
}
MenuLayer::~MenuLayer(){

	
	gameMenu->release();
	
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_GAME_NEI);
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MarketClosed);
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);


}

void MenuLayer::soundClicked(CCObject* obj)
{
	CCMenuItemImage* itemBackgroundSound = (CCMenuItemImage*)obj;
	Music::sharedMusicPlayer()->buttonClicked();

	if (Music::sharedMusicPlayer()->isBackgroundMute()	)
	{//
		itemBackgroundSound->initWithNormalImage(
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"set_music_on.png").c_str(),
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"set_music_on_select.png").c_str(),
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"set_music_on.png").c_str(),
			this,SEL_MenuHandler(&MenuLayer::soundClicked));
		Music::sharedMusicPlayer()->setBackgroundMute(false);
	}else
	{
		itemBackgroundSound->initWithNormalImage(
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"set_music_off.png").c_str(),
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"set_music_off_select.png").c_str(),
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"set_music_off.png").c_str(),
			this,SEL_MenuHandler(&MenuLayer::soundClicked));

		Music::sharedMusicPlayer()->setBackgroundMute();
	}

	GameInforEditer::getGameInfor()->saveSessonInforList();
}
void MenuLayer::effctiveSoundClicked(CCObject* obj)
{
	CCMenuItemImage* itemEffctiveSound = (CCMenuItemImage*)obj;
	Music::sharedMusicPlayer()->buttonClicked();
	if (Music::sharedMusicPlayer()->isEffectiveMute())
	{//
		itemEffctiveSound->initWithNormalImage(
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"set_sound_on.png").c_str(),
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"set_sound_on_select.png").c_str(),
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"set_sound_on.png").c_str(),
			this,SEL_MenuHandler(&MenuLayer::effctiveSoundClicked));
		Music::sharedMusicPlayer()->setEffectiveMute(false);
	}else
	{
		itemEffctiveSound->initWithNormalImage(
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"set_sound_off.png").c_str(),
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"set_sound_off_select.png").c_str(),
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"set_sound_off.png").c_str(),
			this,SEL_MenuHandler(&MenuLayer::effctiveSoundClicked));

		Music::sharedMusicPlayer()->setEffectiveMute();
	}

	GameInforEditer::getGameInfor()->saveSessonInforList();
}

void MenuLayer::onMarketClosed(CCObject *msg)
{
//	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BuyHelpMapSuccess);
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_USE_SUPERTOOL);

	Music::sharedMusicPlayer()->buttonClicked();
	pause_ResumeItemPressed(NULL);
}

void MenuLayer::showRannerAd(float t)
{
	// 显示横幅广告
	//AdManager::getManager()->adShowBannerAd();


}
void MenuLayer::hideBannerAd(float t)
{
	// 关闭横幅广告
	//AdManager::getManager()->adHideBannerAd();
}
void MenuLayer::onExit()
{
	LFLayer::onExit();
	unscheduleAllSelectors();
	////// 关闭横幅广告
	//hideBannerAd(0);
}
void MenuLayer::pause(bool bPauseMusic/* = false*/)
{
	if (isPaused)
	{
		return;
	}
	if (bPauseMusic)
	{
		Music::sharedMusicPlayer()->pauseBackgroundSound();
	}
	Music::sharedMusicPlayer()->pauseAllEffects();
	setTouchEnabled(true);
	isPaused = true;

	BaseScense* scene = dynamic_cast<BaseScense*>(CCDirector::sharedDirector()->getRunningScene());
	if (scene)
	{
		if (scene->getGuideLayer())
		{
			scene->getGuideLayer()->pause();
		}

		SpriteHelper::pauseNodeAndChildren(scene->getBackgroudCoverLayer());
	}
	
	SpriteHelper::pauseNodeAndChildren(layer);
	SpriteHelper::pauseSubNodeAndChildrenInParentNodeByTag(layer,Tag_Box_Boiler);
	SpriteHelper::pauseSubNodeAndChildrenInParentNodeByTag(layer,Tag_Box_RedStar);
	if (layer)
	{
		layer->setLayerFreezy(true);
		layer->setTouchEnabled(false);
		layer->setCCBladeVisible(false);
		layer->setWaitingPause(true);
		layer->endThrowPointSchedule();
	}
	// 暂停碎片
	{
		SpriteHelper::pauseSubNodeAndChildrenInParentNodeByTag(layer,Tag_Fragment);
	}

	//道具影藏
	CCNode *djImg = CCDirector::sharedDirector()->getRunningScene()->getChildByTag(Tag_SpecialItem_Copy);
	if (djImg) djImg->setVisible(false);

}
void MenuLayer::resume(bool bResumeMusic/* = false*/)
{
	if (!isPaused)
	{
		return;
	}
	if (bResumeMusic)
	{
		Music::sharedMusicPlayer()->resumeBackgroundSound();
	}
	
	Music::sharedMusicPlayer()->resumeAllEffects();
	BaseScense* scene = dynamic_cast<BaseScense*>(CCDirector::sharedDirector()->getRunningScene());
	if (scene)
	{
		if (scene->getGuideLayer())
			scene->getGuideLayer()->resume();

		SpriteHelper::resumeNodeAndChildren(scene->getBackgroudCoverLayer());
	}
	isPaused = false;
	setTouchEnabled(false);

	SpriteHelper::resumeNodeAndChildren(layer);
	SpriteHelper::resumeSubNodeAndChildrenInParentNodeByTag(layer,Tag_Box_Boiler);
	SpriteHelper::resumeSubNodeAndChildrenInParentNodeByTag(layer,Tag_Box_RedStar);

	if (layer)
	{
		layer->setLayerFreezy(false);
		layer->setTouchEnabled(true);
		layer->setCCBladeVisible(true);
		layer->setWaitingPause(false);
		layer->begineThrowPointSchedule(0);
	}
	// 恢复碎片
	{
		SpriteHelper::resumeSubNodeAndChildrenInParentNodeByTag(layer,Tag_Fragment);
	}

	//道具影藏
	CCNode *djImg = CCDirector::sharedDirector()->getRunningScene()->getChildByTag(Tag_SpecialItem_Copy);
	if (djImg) djImg->setVisible(true);
}
MenuLayer* MenuLayer::getCurrentLayer()
{
	return instance;
}
CCMenu* MenuLayer::getGameMenu()
{
	return gameMenu;
}
void MenuLayer::lfTouchesEnded(CCPoint glPoint)
{
#ifdef DEBUG_ENABLE_TOUCHMOVE_JUMP_LEVEL
	if (!canJumpLevel)
	{
		return;
	}
	// 向右滑，上一关
	if (glPoint.x - mBeginPos.x > 100)
	{
		//GameLayerFactory* layerFactory = GameLayerFactory::getInstance();
		int preSectionId = sectionId - 1;

		if(preSectionId > 0)
			LevelManager::getManager()->preSection();
		else
			pause_ResumeItemPressed(NULL);
	}

	// 向右滑，下一关
	if (glPoint.x - mBeginPos.x < -100)
	{
		GameLayerFactory* layerFactory = GameLayerFactory::getInstance();
		int nextSeasonId = layerFactory->getNextSeasonId(seasonId,sectionId);
		//int nextSectionId = layerFactory->getNextSectionId(seasonId,sectionId);

		if (nextSeasonId == seasonId)//不跨season,可Skip
		{
			LevelManager::getManager()->nextSection();
		}else
		{
			pause_ResumeItemPressed(NULL);
		}
	}
#endif
}
void MenuLayer::lfClick(CCPoint glPoint)
{
	// 如果是添加元素的界面，那么就得判断点击的位置是否是可以添加Item，可以的话，我们就不认为是
	// 该次点击想恢复游戏界面
	if (addItemLayer && addItemLayer->isVisible() && addItemLayer->pickItem(glPoint) != NULL)
	{
		return;
	}
	CCSprite* help = (CCSprite*)getChildByTag(TAG_HELP);
	if (help)
	{
		if (!help->boundingBox().containsPoint(glPoint))
		{
			removeChildByTag(TAG_HELP,true);
			removeChildByTag(TAG_HELP_BG,true);
		}else
		{
			return;
		}
	}
	pause_ResumeItemPressed(NULL);
}

void MenuLayer::nextSection(cocos2d::CCObject *obj)
{
	LevelManager::getManager()->setShowLoadingAnimation(false);
	LevelManager::getManager()->nextSection();
}

void MenuLayer::preSection(cocos2d::CCObject *obj)
{
	LevelManager::getManager()->setShowLoadingAnimation(false);
	LevelManager::getManager()->preSection();
}

void MenuLayer::restartItemPressed(cocos2d::CCObject *obj)
{
	// BUG 2013-05-03 #2
	{
		if (isPaused)
		{
			CCLog("restartItemPressed bug ....");
			return;
		}

		// 防止Bug.....
		CCMenuItemImage* pauseItem = (CCMenuItemImage*)obj;
		if (pauseItem->getParent() != gameMenu)
		{
			CCLog("Bug.....");
			return;
		}
	}

	Music::sharedMusicPlayer()->buttonClicked();

	if (LFAlert_FreeTimeOut::isTimeToShowBeforePlay(seasonId,sectionId))
	{
		pause();
		LFAlert_FreeTimeOut::showAlert(seasonId,sectionId)->
			addAutoRemoveObserver(this,SEL_CallFuncO(&MenuLayer::buyOnePlayTimeSuc),BUY_ONE_PLAY_TIME_SUC)->
//			addAutoRemoveObserver(this,SEL_CallFuncO(&MenuLayer::buyOnePlayTimeUseMoneyRsp),MSG_PP_SectionTimeLimiteRsp)->
			addAutoRemoveObserver(this,SEL_CallFuncO(&MenuLayer::buyOnePlayTimeFrameDismised),MSG_BUY_SECTION_FRAME_AUTO_DISSMISS);
		return;
	}

	// 重新开始....
	if (layer!=NULL)
	{
		layer->freezeLayer();
	}
    if (scene !=NULL) {
        scene->freezyBgLayer();
    }
	AchievementManager::getInstance()->restartItemClicked();

	LevelManager::getManager()->retry(false);


	//analytics
	//记录玩家开始一关游戏 --- end (failed)
	const char* pEventInfo = CCString::createWithFormat("%02d-%02d", LevelManager::getManager()->getLastSeasonId(), LevelManager::getManager()->getLastSectionId())->getCString();
	CmdHelper::getInstance()->getAnalyticsInstance()->onFailed(pEventInfo, "restart");
}

void MenuLayer::buyOnePlayTimeSuc(CCObject* alertNode)
{
	// 重新开始....
	if (layer!=NULL)
	{
		layer->freezeLayer();
	}
	if (scene !=NULL) {
		scene->freezyBgLayer();
	}
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,BUY_ONE_PLAY_TIME_SUC);
	AchievementManager::getInstance()->restartItemClicked();
	LevelManager::getManager()->retry(false);
}

void MenuLayer::buyOnePlayTimeUseMoneyRsp(CCObject* payRspWrapper)
{
// 	PPCmd_BuySectionTimeLimitRsp* rsp = (PPCmd_BuySectionTimeLimitRsp*)((PayRspWrapper*)payRspWrapper)->data;
// 	if (rsp->rspCode == PP_Success && rsp->seasonId == seasonId && rsp->sectionId == sectionId)
// 	{
// 		buyOnePlayTimeSuc(NULL);
// 	}
}

void MenuLayer::buyOnePlayTimeFrameDismised(CCObject* alertNode)
{
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_BUY_SECTION_FRAME_AUTO_DISSMISS);
	resume();
}




void MenuLayer::pauseItemPressed(CCObject* obj)
{
    
	// BUG 2013-05-03 #1
	{
		if (isPaused)
		{
			CCLog("pauseItemPressed bug ....");
			return;
		}
		

		// 防止Bug.....
		CCMenuItemImage* pauseItem = (CCMenuItemImage*)obj;
		if (pauseItem->getParent() != gameMenu)
		{
			CCLog("Bug.....");
			//lfClick(pauseItem->getPosition());
			return;
		}
	}


	//移除吐槽动画
	CCNode* node = CCDirector::sharedDirector()->getRunningScene()->getChildByTag(LONG_TIME_ACTION_TAG);
	if (node != NULL)
	{
		node->removeFromParent();
	}
	canJumpLevel = true;
	this->getParent();

	showppauseLayer();
	Music::sharedMusicPlayer()->buttonClicked();
}


void MenuLayer::actionHou(CCNode* node){

	 node->setVisible(false);

}

void MenuLayer::showppauseLayer(){
	this->setTouchEnabled(false);
	BaseScense::getCurrentScene()->setCollisionEnable(false);
	BaseContentLayer::shareBaseContentLayer()->setInfoVisible(false);
	BaseContentLayer::shareBaseContentLayer()->setRoleVisible(false);
	if (BaseScense::getCurrentScene()->getGuideLayer())
	{
		BaseScense::getCurrentScene()->getGuideLayer()->setVisible(false);
	}
	pause(true);
	SpriteHelper::VisibleSubNodeAndChildrenInParentNodeByTag(BaseContentLayer::shareBaseContentLayer(), Tag_Fragment, false);
	setGameMenuEnable(false);

	float time = 0.2f;

	//得到齿轮的背景（）
	CCLayer* clLayer=BaseScense::getCurrentScene()->getBackgroudCoverLayer();
	if(clLayer){

		clLayer->runAction(CCFadeOut::create(time_show));
	}
	nodesallFade(clLayer,true);
	/*
	if(clLayer!=NULL){
		clLayer->runAction(
			     CCSequence::create(
			            CCFadeOut::create(time),
						CCCallFuncN::create(this,callfuncN_selector(MenuLayer::actionHou)),
						NULL
				 )
				 );
	}
	*/
	//得到背景层
	//->setVisible(false);
	//得到所有的元素（木块，星星）
	BaseContentLayer::shareBaseContentLayer()->runAction(CCFadeOut::create(time_show));
	nodesallFade(BaseContentLayer::shareBaseContentLayer(),true);
	/*
	CCArray* children = BaseContentLayer::shareBaseContentLayer()->getChildren();
	int num = children->count();
	for (int i=0;i<num;i++)
	{
		CCNode* temp = (CCNode*)children->objectAtIndex(i);
		//temp->setVisible(false);
		// menuitems.at(i)->runAction(CCFadeIn::create(dt));
		temp->runAction(
			CCSequence::create(
			CCFadeOut::create(time),
			CCCallFuncN::create(this,callfuncN_selector(MenuLayer::actionHou)),
			NULL
			)
			);
	}
    */
    
    Stopgame3* stop3=Stopgame3::create();
	CCDirector::sharedDirector()->getRunningScene()->addChild(stop3, 1, Tag_StopGame3Layer);

	Stopgame* stop=Stopgame::createBymark(true);
	CCDirector::sharedDirector()->getRunningScene()->addChild(stop);

	Stopgameright* stop2=Stopgameright::create();
	CCDirector::sharedDirector()->getRunningScene()->addChild(stop2);
	
	
	
	
}

void MenuLayer::nodesallFade(CCNode* node,bool mark){

	if(node){
		CCArray* nodes=node->getChildren();
		if(nodes){
		for(size_t i=0;i<nodes->count();i++){
			CCNode* node= (CCNode*)nodes->objectAtIndex(i);
			//冰冻炸弹效果:点击了之后才显示，不然会有冰冻炸弹和效果同时出现的情况
			if (node->getTag() == Tag_Box_Frost)
			{
				if(((TimerDec*)node)->getIsDecing())
				{
					node->setVisible(true);
				}
				continue;
			}	
			else if (node->getTag() == Tag_Box_Frost_Par)
			{
				continue;
			}
			else if (node->getTag() == Tag_Box_Bomb_DarkWhole)
			{
				//黑洞点击了后悔隐藏黑洞图片，所以点击了之后，就不要再显示了，不然会有黑洞和黑洞效果同时出现的情况
				//黑洞的粒子也是要在点击了黑洞了之后才能显示
				if(((DarkHoleBomb*)node)->getIsBombed())
				{
					node->setVisible(false);
					if (CCNode* pNode = node->getChildByTag(Tag_Box_DarkHoleBomb_Par))
					{
						pNode->setVisible(true);
					}
				}
				continue;
			}
			else if (node->getTag() == Tag_Box_DarkHoleBomb_Par)
			{
				continue;
			}
			CCActionInterval* action = NULL;
			if(mark==false){
				action=CCFadeIn::create(time_show);
			}else{
				action=CCFadeOut::create(time_show);
			}
			if (node->getTag() == Tag_Box_RedStar_Mask)
			{
				node->stopAllActions();

				if (!mark)
				{
					node->runAction(CCRepeat::create(CCSequence::create(CCFadeTo::create(0.9f,0),CCFadeTo::create(0.9f,180),NULL),-1));
				}
			}
			node->runAction(action);

			if(node->getChildren()!=NULL&&node->getChildren()->count()>0){
				nodesallFade(node,mark);
			}

			if (i == nodes->count()-1)
			{
				BaseContentLayer::shareBaseContentLayer()->setVisible(!mark);
			}
		}
		}
	}
}

void MenuLayer::pause_ResumeItemPressed(CCObject* obj)
{
	if (obj!=NULL)
	{
		Music::sharedMusicPlayer()->buttonClicked();
	}
	showGame();
	resume();
}
void MenuLayer::packageItemPressed(CCObject* obj)
{
#ifdef DEBUG_ENABLE_PACKAGE
	Music::sharedMusicPlayer()->buttonClicked();
	canJumpLevel = false;
	showItemSelectUI();
	pause();
#endif
}

void MenuLayer::superToolItemPressed(CCObject* obj)
{
	if (isPaused)
	{
		CCLog("helpItemPressed bug ....");
		return;
	}

	pause(true);
	needSuperToolAction = false;
	CCNotificationCenter::sharedNotificationCenter()->addObserver(
		this,SEL_CallFuncO(&MenuLayer::useSuperTool),MSG_USE_SUPERTOOL, 0);
	Chaonengdaoju* daoju = Chaonengdaoju::creteBytag(true);
	daoju->setTouchPriority(kCCMenuHandlerPriority);
	CCDirector::sharedDirector()->getRunningScene()->addChild(daoju,1024);

	CCLayerColor* pColor = CCLayerColor::create(ccc4(0.0f, 0.0f, 0.0f, 125.0f));
	daoju->addChild(pColor, -1);
	Music::sharedMusicPlayer()->buttonClicked();
#if 0
	if (isPaused)
	{
		CCLog("helpItemPressed bug ....");
		pause_ResumeItemPressed(obj);
//		return;
	}
	//移除吐槽动画
	CCNode* node = CCDirector::sharedDirector()->getRunningScene()->getChildByTag(LONG_TIME_ACTION_TAG);
	if (node != NULL)
	{
		node->removeFromParent();
	}
	if (m_bIsTeachUseSuperTool)
	{
		m_lMaskLayer->removeFromParent();
	}
	else
	{
		CCNotificationCenter::sharedNotificationCenter()->addObserver(
			this,SEL_CallFuncO(&MenuLayer::useSuperTool),MSG_USE_SUPERTOOL, 0);
	}


#if ((CC_TARGET_PLATFORM == CC_PLATFORM_IOS && Version_Platform == 0) || DEBUG_IOSMarket)

	if (!m_bIsTeachUseSuperTool)
	{
		if (GameInforEditer::getGameInfor()->getSuperToolCount(ST_SuperTool) > 0)
		{
			IOSMarketUseSuperToolLayer *layer = new IOSMarketUseSuperToolLayer;
			CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 200);
			layer->release();
		}
		else 
		{
			MessageDisplay *layer = MessageDisplay::create();
			layer->setComeFrom(from_GameInSuperTool);
			CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 200);
		}
	}
	else
	{
		IOSMarketUseSuperToolLayer *layer = new IOSMarketUseSuperToolLayer;
		CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 200);
		layer->release();
		layer->beginTeachUseSuperTool();
		layer->setTeachCallBack(this,SEL_CallFunc(&MenuLayer::teachCallBack));
		m_bIsTeachUseSuperTool = false;
//		superToolItem->stopAllActions();
	}
#else
	m_index	= MS_SuperTools;
	MarketLayer *market = new MarketLayer(MS_SuperTools, false);
	CCDirector::sharedDirector()->getRunningScene()->addChild(market, 200, marketlayer_tag);
	if (m_bIsTeachUseSuperTool)
	{
		market->beginTeachUseSuperTool();
		market->setTeachCallBack(this,SEL_CallFunc(&MenuLayer::teachCallBack));
		m_bIsTeachUseSuperTool = false;
//		superToolItem->stopAllActions();
	}
	market->release();
#endif

	pause();
	needSuperToolAction = false;
#endif
}

void MenuLayer::useSuperTool(CCObject* alertNode)
{
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_USE_SUPERTOOL);
	setBtnvisible(false);
//	GameInforEditer::getGameInfor()->subSuperTool(ST_SuperTool, 1);
	resume(true);
}

void MenuLayer::setBtnvisible(bool e)
{
	if(m_gongluetuItem)
	{
		m_gongluetuItem->setVisible(e);
	}
	m_superToolItem->setVisible(e);
	m_restartItem->setVisible(e);
}


void MenuLayer::buySuperToolUseMoneyRsp(CCObject* payRspWrapper)
{
// 	PPCmd_BuySuperToolRsp* rsp = (PPCmd_BuySuperToolRsp*)((PayRspWrapper*)payRspWrapper)->data;
// 	if (rsp->rspCode == PP_Success)
// 	{
// 		BaseBuyAlert::removeAllBuyAlerts();
// 		// 重新创建一次，刷新
// 		LFAlert_BuySuperTool::showAlert()->
// 			addAutoRemoveObserver(this,SEL_CallFuncO(&MenuLayer::useSuperTool),MSG_USE_SUPERTOOL)->
// 			addAutoRemoveObserver(this,SEL_CallFuncO(&MenuLayer::superFrameClosed),MSG_BUY_TOOL_FRAME_AUTO_DISSMISS);
// 	}
}

void MenuLayer::superFrameClosed(CCObject* alertNode)
{
	resume();
}

void MenuLayer::helpItemPressed(CCObject* obj)
{
	if (isPaused)
	{
		CCLog("helpItemPressed bug ....");
		pause_ResumeItemPressed(obj);
		return;
	}
	needHelpItemAction = false;
	pause();
	Goluetu* gt_layer = Goluetu::creteBytag(true);
	gt_layer->setTouchPriority(kCCMenuHandlerPriority);
	gt_layer->drawGongluetu();
	CCDirector::sharedDirector()->getRunningScene()->addChild(gt_layer, 1000);

	CCLayerColor* pColor = CCLayerColor::create(ccc4(0.0f, 0.0f, 0.0f, 125.0f));
	gt_layer->addChild(pColor, -1);

	Music::sharedMusicPlayer()->buttonClicked();
#if 0
	if (isPaused)
	{
		CCLog("helpItemPressed bug ....");
		pause_ResumeItemPressed(obj);
//		return;
	}

	//移除吐槽气泡对话框
	CCNode* node = CCDirector::sharedDirector()->getRunningScene()->getChildByTag(LONG_TIME_ACTION_TAG);
	if (node != NULL)
	{
		node->removeFromParent();
	}

	Music::sharedMusicPlayer()->buttonClicked();
	canJumpLevel = true;
	needHelpItemAction = false;
//	setGameMenuEnable(false);
	pause();
// 	pauseBg->setVisible(true);
// 	pauseBg->stopAllActions();
// 	pauseBg->setOpacity(0);
// 	pauseBg->runAction(CCFadeTo::create(0.2f,255));

	//const SectionPlayData* data = GameInforEditer::getGameInfor()->getReadOnlySectionPlayData(seasonId,sectionId);
	//if (data == NULL || !data->usedHelp) // 还没用过
	//{
	//	if (GameInforEditer::getGameInfor()->getHelpTimeLeft() <= 0)
	//	{
	//		LFAlert_BuySolution::showAlert()->
	//			addAutoRemoveObserver(this,SEL_CallFuncO(&MenuLayer::buyHelpUseMoneyRsp),MSG_PP_SolutionTimeRsp)->
	//			addAutoRemoveObserver(this,SEL_CallFuncO(&MenuLayer::buyHelpUseStarSuc),BUY_ONE_HELP_TIME_USESTAR_SUC)->
	//			addAutoRemoveObserver(this,SEL_CallFuncO(&MenuLayer::buyHelpFrameClosed),MSG_BUY_HELP_FRAME_AUTO_DISSMISS);
	//		return;
	//	}
	//}


#if ((CC_TARGET_PLATFORM == CC_PLATFORM_IOS && Version_Platform == 0) || DEBUG_IOSMarket)
	if (UserDataManager::getInstance()->getOneSectionMapState((seasonId-1)*PerGuanKaCount + sectionId) > 0)
	{
		addHelpToCurrentLayer();
	}
	else 
	{

		CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
			callfuncO_selector(MenuLayer::buyHelpMapSuccess), MSG_BuyHelpMapSuccess, NULL);

		MessageDisplay *layer = MessageDisplay::create();
		layer->setComeFrom(from_GameInHelpMap);
		CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 200);
	}
#else	
	if (GameInforEditer::getGameInfor()->getHelpTimeLeft() <= 0)
	{
		CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
			callfuncO_selector(MenuLayer::buyHelpMapSuccess), MSG_BuyHelpMapSuccess, NULL);
		m_index	= MS_StrategyMap;
		MarketLayer *market = new MarketLayer(MS_StrategyMap, false);
		CCDirector::sharedDirector()->getRunningScene()->addChild(market, 200, marketlayer_tag);
	}
	else
	{
		addHelpToCurrentLayer();
	}
#endif

#endif

}
void MenuLayer::buyHelpMapSuccess(CCObject *msg)
{
	//发送商店关闭消息在这个消息之前，但是商店关闭消息会恢复游戏，所以我们在这儿调用移除攻略图购买监听
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BuyHelpMapSuccess);
	helpItemPressed(NULL);
}
void MenuLayer::addHelpToCurrentLayer()
{
	//// 扣次数
	//const SectionPlayData* data = GameInforEditer::getGameInfor()->getReadOnlySectionPlayData(seasonId,sectionId);
	//if (data == NULL || !data->usedHelp) // 还没用过
	//{
	//	GameInforEditer::getGameInfor()->subHelpNum(1);
	//	GameInforEditer::getGameInfor()->setSectionHelpUsed(seasonId,sectionId);
	//}
	
	//当前关卡使用攻略图
	SectionInfor* info = GameInforEditer::getGameInfor()->getSectionInfor(seasonId, sectionId);
	if (info)
	{
		info->bUseHelpMap = true;
	}
	GameInforEditer::getGameInfor()->saveSessonInforList();

	if(getChildByTag(TAG_HELP))
		return;
	// 帮助
	do{
		ResManager* resMgr = ResManager::getManager();
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		CCLayerColor* pBglayer = CCLayerColor::create(ccc4(0, 0, 0, 0));
		addChild(pBglayer, 9);
		pBglayer->setTag(TAG_HELP_BG);
		pBglayer->setScale(5.0f);
		pBglayer->runAction(CCFadeTo::create(0.2f, 125));

		CCSprite* helpBg = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+"help_bg.png").c_str());
		CC_BREAK_IF(!helpBg);
		addChild(helpBg,10,TAG_HELP);
		

		CCMenuItemImage* closeItem = CCMenuItemImage::create(
			resMgr->getSharedFilePath(g_buttonPath+"guanbi.png").c_str(),
			resMgr->getSharedFilePath(g_buttonPath+"guanbi_select.png").c_str(),
			this,SEL_MenuHandler(&MenuLayer::closeHelpClicked));
		CCMenu* menu = CCMenu::create(closeItem,NULL);
		menu->setPosition(CCPointZero);
		helpBg->addChild(menu);
		closeItem->setPosition(ccp(698,500+1));

		helpBg->setPosition(ccp(winSize.width/2,winSize.height/2));
		helpBg->setScale(0.0f);
		helpBg->runAction(CCSequence::create(
			CCScaleTo::create(0.3f,1.1f),
			CCScaleTo::create(0.15f,0.9f),
			CCScaleTo::create(0.05f,1.0f),
			NULL));

		char fileName[56];
		sprintf(fileName,"help_%d_%d.png",seasonId,sectionId);
		CCSprite* help = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+fileName).c_str());
		CC_BREAK_IF(!help);
		helpBg->addChild(help);
		help->setPosition(ccp(helpBg->getContentSize().width/2,helpBg->getContentSize().height/2));
		help->setScale(2.0f);

	}while(false);
}

void MenuLayer::buyHelpUseStarSuc(CCObject* obj)
{
	addHelpToCurrentLayer();
}
void MenuLayer::buyHelpUseMoneyRsp(CCObject* payRspWrapper)
{
// 	PPCmd_BuySolutionTimeRsp* rsp = (PPCmd_BuySolutionTimeRsp*)((PayRspWrapper*)payRspWrapper)->data;
// 	if (rsp->rspCode == PP_Success)
// 	{
// 		// 移除对话框
// 		BaseBuyAlert::removeAllBuyAlerts();
// 		addHelpToCurrentLayer();
// 	}
}
// 
void MenuLayer::buyHelpFrameClosed(CCObject* alertNode)
{
	closeHelpClicked(NULL);
}


void MenuLayer::closeHelpClicked(CCObject* obj)
{
	Music::sharedMusicPlayer()->buttonClicked();
	removeChildByTag(TAG_HELP,true);
	removeChildByTag(TAG_HELP_BG,true);
	pause_ResumeItemPressed(NULL);
}
void MenuLayer::pause_SelectLevelPressed(CCObject* obj)
{
	gotoSelectLevelPage();
}
void MenuLayer::pause_MainPressed(CCObject* obj)
{
// #if Version_Platform == 3 || Version_Platform == 5
// 	MessageDisplay *layer = MessageDisplay::create();
// 	layer->setComeFrom(from_ClickMainBtn);
// 	CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 200);
// #else
	gotoMainPage();
//#endif
}

void MenuLayer::talkItemPressed(CCObject* pObj)
{
	//CmdHelper::getHelper()->cmdSDKLogin();
}

void MenuLayer::gotoMainPage()
{
	setTouchEnabled(false);

	Music::sharedMusicPlayer()->buttonClicked();
	if (layer!=NULL)
	{
		layer->freezeLayer();
		layer->unscheduleAllSelectors();
	}
	if (scene !=NULL) {
		scene->freezyBgLayer();
	}

	tempLastBackToMain = true;
	CCScene* currentScene = CCDirector::sharedDirector()->getRunningScene();

#if USE_DOOR_REPLACE_CURTAIN == 1
	DoorLayer* layer = new DoorLayer(seasonId,sectionId,true);
	currentScene->addChild(layer,128);
#else
	CurtainLayer* layer = new CurtainLayer(seasonId,sectionId,true);
	currentScene->addChild(layer,128);
#endif
	setGameMenuEnable(false);

	pauseMenu->setVertexZ(false);
	pauseMenu->setEnabled(false);
	setVisible(false);
	layer->setAutoRemve(false);
	layer->action(this);

	layer->release();
}
void MenuLayer::gotoSelectLevelPage()
{
	setTouchEnabled(false);

	Music::sharedMusicPlayer()->buttonClicked();
	tempLastBackToMain = false;
	CCScene* currentScene = CCDirector::sharedDirector()->getRunningScene();
#if USE_DOOR_REPLACE_CURTAIN == 1
	DoorLayer* layer = new DoorLayer(seasonId,sectionId,true);
	currentScene->addChild(layer,128);
#else
	CurtainLayer* layer = new CurtainLayer(seasonId,sectionId,true);
	currentScene->addChild(layer,128);
#endif


	setGameMenuEnable(false);

	pauseMenu->setVertexZ(false);
	pauseMenu->setEnabled(false);
	setVisible(false);
	layer->setAutoRemve(false);
	layer->action(this);
	layer->release();
}

void MenuLayer::curtainActionEnd()
{
	CCScene* currentScene = CCDirector::sharedDirector()->getRunningScene();
	currentScene->removeChildByTag(Tag_Layer_Background,true);
	currentScene->removeChildByTag(Tag_Layer_shadow,true);
	currentScene->removeChildByTag(Tag_Layer_BaseContent,true);
	currentScene->removeChildByTag(Tag_Layer_Notification,true);

	if (tempLastBackToMain)
	{
		LoadingLayer* layer = new LoadingLayer(Intent_GameIn2Main);
		currentScene->addChild(layer,128);
		layer->release();
	}else
	{
		LoadingLayer* layer = new LoadingLayer(Intent_GameIn2Section);
		currentScene->addChild(layer,128);
		layer->release();
	}	
}
void MenuLayer::pause_SkipLevelPressed(CCObject* obj)
{
	setTouchEnabled(false);
	Music::sharedMusicPlayer()->buttonClicked();
	LevelManager::getManager()->nextSection();
}
void MenuLayer::setBaseContentLayer(BaseContentLayer* layer)
{
	this->layer = layer;
	
	//暂时不需要这层
//	if (!addItemLayer)
//	{
// 		addItemLayer = new AddItemLayer(layer);
// 		addChild(addItemLayer,10);
// 		addItemLayer->release();
// 		addItemLayer->setVisible(false);
// 		addItemLayer->setTouchEnabled(false);
//	}
}
void MenuLayer::setBaseScene(BaseScense *scene)
{
    this->scene = scene;
}

void MenuLayer::showGame()
{
	if (pauseMenu) {
		pauseMenu->setVisible(false);
		pauseMenu->setEnabled(false);
	}
	if (selectUIMenu){
		selectUIMenu->setEnabled(false);
		selectUIMenu->setVisible(false);
	}
	if(addItemLayer)
	{
		addItemLayer->setVisible(false);
		addItemLayer->setTouchEnabled(false);
	}
	if (pauseBg) pauseBg->setVisible(false);

	setGameMenuEnable(true);


}
void MenuLayer::showPause()
{
	pauseMenu->setVisible(true);
	pauseMenu->setEnabled(true);
	pauseBg->setVisible(true);

	pauseBg->stopAllActions();
	pauseBg->setOpacity(0);
	pauseBg->runAction(CCFadeTo::create(0.2f,255));

	setGameMenuEnable(false);
}
void MenuLayer::showItemSelectUI()
{
	selectUIMenu->setVisible(true);
	selectUIMenu->setEnabled(true);

	if(addItemLayer)
	{
		addItemLayer->setVisible(true);
		addItemLayer->setTouchEnabled(true);
	}

	pauseBg->setVisible(true);
	pauseBg->stopAllActions();
	pauseBg->setOpacity(0);
	pauseBg->runAction(CCFadeTo::create(0.2f,255));

	setGameMenuEnable(false);
}

void MenuLayer::setGameMenuEnable(bool enbale)
{
	gameMenu->setVisible(enbale);
	gameMenu->setEnabled(enbale);

	//CCArray* menuChildren = gameMenu->getChildren();
	//int childNum = gameMenu->getChildrenCount();
	//if (menuChildren && childNum > 0)
	//{
	//	CCObject* pObject = NULL;
	//	CCARRAY_FOREACH(menuChildren, pObject)
	//	{
	//		CCNode* pChild = dynamic_cast<CCNode*>(pObject);
	//		if ( (CCMenuItem*)pChild != NULL)
	//		{
	//			((CCMenuItem*)pChild)->setEnabled(enbale);
	//			//((CCMenuItem*)pChild)->setVisible(enbale);
	//		}
	//	}
	//}
}

void MenuLayer::talkLoginEnd(CCObject* pObj)
{
	if (UserDataManager::getInstance()->isSdkLogin())
	{
		this->setTalkBtnvisible(false);
	}
}

void MenuLayer::setTalkBtnvisible(bool e)
{
	if (gameMenu)
	{
		CCMenuItemImage* pTalkItem = (CCMenuItemImage*)gameMenu->getChildByTag(talk_btn_tag);
		if (pTalkItem && pTalkItem->isVisible() != e)
		{
			pTalkItem->setVisible(e);
		}
	}
}
void MenuLayer::addCollectToolLayer()
{
	//屏蔽剧情道具相关
/*	
	CCSprite *spr = CCSprite::create(CCString::createWithFormat(ResManager::getManager()->getSharedFilePath(g_gameinPath+"daojukuang_youxizhong_act%d.png").c_str(),seasonId)->getCString());
	m_lCollectLayer = spr;
	CCSize size = spr->getContentSize();
	if (spr)
	{
		spr->setPosition(ccp(superToolItem->getPositionX(),superToolItem->getPositionY() - spr->getContentSize().height / 2 - superToolItem->getContentSize().height));
		addChild(spr,-1);

		vector<JuqingDaoJu> juqingDaoJu = JuqingDaoJuManager::getManager()->getItemInSeason(seasonId);

		CCPoint offestPos = CCPointMake(size.width / 2,60);
		int side_y = 5;
		int side_height = 57;
		for (vector<JuqingDaoJu>::iterator iter = juqingDaoJu.begin(); iter != juqingDaoJu.end() ; iter++)
		{
			//iter->daojuId == 
			if (iter->state == DJS_NotGetted)
			{
				continue;
			}
			int id = iter->daojuId;
			id %= 4;
			id--;
			if (id == -1)
			{
				id = 3;
			}
			CCPoint pos;
			pos.x = offestPos.x;
			pos.y = offestPos.y + (3 - id) * (side_y + 57);
			CCSprite *daojuSpr = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_daojuPath+iter->fileName).c_str());
			daojuSpr->setPosition(pos);
			daojuSpr->setScale(0.38f);
			spr->addChild(daojuSpr);
		}
	}
*/
}
CCPoint MenuLayer::getJuqingDaojuPos(int id)
{
	CCPoint beginPos = m_lCollectLayer->getPosition();

	//屏蔽剧情道具相关
/*
	CCPoint offestPos = CCPointMake(m_lCollectLayer->getContentSize().width / 2,60);
	int side_y = 5;
	int side_height = 57;
	
	id %= 4;
	id--;
	if (id == -1)
	{
		id = 3;
	}
	CCPoint pos;
	pos.x = offestPos.x + beginPos.x - m_lCollectLayer->getContentSize().width / 2;
	pos.y = offestPos.y + (3 - id) * (side_y + 57) + beginPos.y - m_lCollectLayer->getContentSize().height / 2;
	return pos;
*/
	return beginPos;
}
void MenuLayer::getJuqingDaoju(JuqingDaoJu &daoju)
{
	//屏蔽剧情道具相关
/*
	int id = daoju.daojuId;

	CCPoint offestPos = CCPointMake(m_lCollectLayer->getContentSize().width / 2,60);
	int side_y = 5;
	int side_height = 57;

	id %= 4;
	id--;
	if (id == -1)
	{
		id = 3;
	}
	CCPoint pos;
	pos.x = offestPos.x;
	pos.y = offestPos.y + (3 - id) * (side_y + 57);

	CCSprite *spr = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_daojuPath+daoju.fileName).c_str());
	spr->setPosition(pos);
	spr->setScale(0.38f);
	m_lCollectLayer->addChild(spr);
	spr->runAction(CCSequence::create(CCScaleTo::create(0.5f,0.75f),CCScaleTo::create(0.2f,0.38f),NULL));
*/
}

//stopgame3

Stopgame3::Stopgame3(void){

}
Stopgame3::~Stopgame3(void){


}


bool Stopgame3::init(){

	if(!CCLayer::init()){
		return false;
	}
	CCSize size=CCDirector::sharedDirector()->getWinSize();
	this->setContentSize(size);
	ccColor4B mr;
	mr.r=0;mr.g=0;mr.b=0;mr.a=125;
	CCLayerColor* colorLayer1 = CCLayerColor::create(mr);
	addChild(colorLayer1);

	return true;
}
void Stopgame3::onEnter(){

	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,getZOrder(),true);
}
void Stopgame3::onExit(){

	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();

}



bool Stopgame3::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	return true;

}