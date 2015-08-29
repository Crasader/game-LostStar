//
//  SkipLayer.cpp
//  demo-box2d
//
//  Created by Davidloeng loeng on 12-6-27.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "SkipLayer.h"
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


SkipLayer::SkipLayer(int scenseIndex){
	container = CCSprite::create();
	lfAlert = NULL;
	currentIndex = scenseIndex;
    mScreenSize = CCDirector::sharedDirector()->getWinSize();
	container->setContentSize(CCSizeMake(1024,768));
	addChild(container,10);
	container->setPosition(ccp(mScreenSize.width/2,mScreenSize.height/2));

	GameLayerFactory* layerFactory = GameLayerFactory::getInstance();
	std::pair<int, int> info1 = layerFactory->getLevelFormat(scenseIndex);
	mSeasonId = info1.first;
	mSectionId = info1.second;

    score = 0;
	toShowStarNum = 0;
	mGameScore = Runtime::getRuntime()->getGameScore();
	keyItemMedal = CCSprite::create(ResManager::getManager()->getFilePathByLanguageType("keyItemMedal.png").c_str());
	betterScore = CCSprite::create(ResManager::getManager()->getFilePathByLanguageType("betterScoreMedal.png").c_str());
	
	container->addChild(keyItemMedal,10);
	container->addChild(betterScore,10);
	keyItemMedal->setVisible(false);
	betterScore->setVisible(false);

    nextToSelector = false;
    do{
		ResManager* resMgr = ResManager::getManager();

#if USE_DOOR_REPLACE_CURTAIN == 1
		curtainLayer = new DoorLayer(mSeasonId,mSectionId,true);
#else
		curtainLayer = new CurtainLayer(mSeasonId,mSectionId,true);
#endif
		ICurtain* curtain = dynamic_cast<ICurtain*>(curtainLayer);
		CCAssert(curtain!=NULL,"");
		curtain->setAutoRemve(false);
		curtain->action(this);

		addChild(curtainLayer);
		curtainLayer->release();
		
		

		//界面按钮
		{
			CCMenuItemImage* itemNext = CCMenuItemImage::create(
				resMgr->getSharedFilePath("congratulation_next.png").c_str(),
				resMgr->getSharedFilePath("congratulation_next_select.png").c_str(),
				this,SEL_MenuHandler(&SkipLayer::nextItemPressed));
			CCMenuItemImage* itemRestart = CCMenuItemImage::create(
				resMgr->getSharedFilePath("congratulation_restart.png").c_str(),
				resMgr->getSharedFilePath("congratulation_restart_select.png").c_str(),
				this,SEL_MenuHandler(&SkipLayer::restartItemPressed));
			CCMenuItemImage* itemMain = CCMenuItemImage::create(
				resMgr->getSharedFilePath("congratulation_main.png").c_str(),
				resMgr->getSharedFilePath("congratulation_main_select.png").c_str(),
				this,SEL_MenuHandler(&SkipLayer::mainItemPressed));

			menu = CCMenu::create(itemNext,itemRestart,itemMain, NULL);
			menu->setPosition(CCPointZero);
			container->addChild(menu,5);

			float posX = 202;
			float posY = 178;
			float distance = 310;

			itemRestart->setPosition(ccp(posX,posY));posX+=distance;
			itemMain->setPosition(ccp(posX,posY));posX+=distance;
			itemNext->setPosition(ccp(posX,posY));posX+=distance;
		}

		menu->setEnabled(false);
		menu->setVisible(false);
		schedule(schedule_selector(SkipLayer::timeToShow),0.6f);
     
    }while (0);


	{
		Music::sharedMusicPlayer()->loadEffectSound("star1.wav");	
		Music::sharedMusicPlayer()->loadEffectSound("star2.wav");	
		Music::sharedMusicPlayer()->loadEffectSound("star3.wav");	
		Music::sharedMusicPlayer()->loadEffectSound("laugh.wav");
	}
}

void SkipLayer::onEnter()
{
	CCLayer::onEnter();
}
void SkipLayer::onExit()
{
	CCLayer::onExit();
}


SkipLayer::~SkipLayer(){
 
}
void SkipLayer::curtainActionEnd()
{
	// 注释下面这句，用于修复进入下一章黑屏的bug
	//BaseContentLayer::shareBaseContentLayer()->removeFromParentAndCleanup(true);
}
void SkipLayer::timeToShow(float t)
{
	unschedule(schedule_selector(SkipLayer::timeToShow));
	//
	int index = currentIndex;
	if (index < 0)
	{
		index = 0;
		CCLog("error : currentIndex(=%d) smaller than 0",currentIndex);
	}
	showStar(mGameScore.getStarNum());
	showTime(mGameScore.timeCost);
	showScore(mGameScore.usedItemNum);
	//勋章的显示
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		SectionInfor* sectionInfor = GameInforEditer::getGameInfor()->getSectionInfor(mSeasonId,mSectionId);
		//if (sectionInfor->keyItemMedalNum  < mGameScore.keyItemMedalNum || 
		//	sectionInfor->keyTableMedalNum < mGameScore.keyTableMedalNum )
		if (mGameScore.keyItemMedalNum >= 1|| 
			mGameScore.keyTableMedalNum >= 1)
		{// 动画显示勋章
			keyItemMedal->setVisible(true);
			keyItemMedal->setOpacity(100);
			keyItemMedal->setScale(2.0f);
			keyItemMedal->setPosition(ccp(250,463));
			keyItemMedal->runAction(CCFadeTo::create(0.5f,255));
			keyItemMedal->runAction(CCScaleTo::create(0.5f,1.0f));
		}else if (sectionInfor && (sectionInfor->keyItemMedalNum > 0 || sectionInfor->keyTableMedalNum > 0))
		{// 直接显示勋章
			
			keyItemMedal->setVisible(true);
			keyItemMedal->setPosition(ccp(200,463));
		}

		// 不显示勋章
		keyItemMedal->setVisible(false);

		//if (true)
		if (mGameScore.isABetterScore)
		{
			betterScore->setVisible(true);
			betterScore->setOpacity(100);
			betterScore->setScale(2.0f);
			betterScore->setPosition(ccp(896,463));
			betterScore->runAction(CCFadeTo::create(0.5f,255));
			betterScore->runAction(CCScaleTo::create(0.5f,1.0f));
		}
	}
	menu->setEnabled(true);
	menu->setVisible(true);

}
void SkipLayer::scoreUpdate(float time)
{
    showScore(++score);
}

void SkipLayer::showTime(float time)
{
	ResManager* resMgr = ResManager::getManager();
	CCSprite* timer = CCSprite::create(resMgr->getSharedFilePath("congratulation_time.png").c_str());
	container->addChild(timer);
	timer->setPosition(ccp(400 - 20,300));
	CCLabelAtlas* lable = CCLabelAtlas::create(
		"0123456789",
		resMgr->getSharedFilePath("season_section_num.png").c_str(),
		40, 49,//24
		'0');
	container->addChild(lable);

	CCSprite* addLable = CCSprite::create(resMgr->getSharedFilePath("season_section_num_plus.png").c_str());
	container->addChild(addLable);addLable->setVisible(false);

	int value = (time + 0.5f);	// 进一法
	char temp[128];
	if (value > 999)
	{
		value = 999;
		addLable->setVisible(true);
	}
	sprintf(temp,"%d",value);
	lable->setString(temp);
	lable->setPosition(ccp(timer->getPositionX()+35,timer->getPositionY()-lable->getContentSize().height/2));
	addLable->setPosition(ccp(lable->getPositionX() + lable->getContentSize().width + addLable->getContentSize().width/2,timer->getPositionY()));
}
void SkipLayer::showStar(int starNum)
{
	ResManager* resMgr = ResManager::getManager();

	//float itemWidth = 163;
	//float posY = 550;
	//float distance = 180;
	//float totalStar = 3;
	//float posX = 350;

	CCSprite* starBoarder = CCSprite::create(resMgr->getSharedFilePath("congratulation_star_board.png").c_str());
	container->addChild(starBoarder);
	starBoarder->setPosition(ccp(512,600));

	string starFileName[] = {
		"congratulation_star_left.png",
		"congratulation_star_middle.png",
		"congratulation_star_right.png"
	};
	string commentFileName[] = {
		"congratulation_good.png",
		"congratulation_great.png",
		"congratulation_perfect.png"
	};
	CCPoint starPos[] = {
		CCPointMake(112,107),
		CCPointMake(288,151),
		CCPointMake(477,106)
	};

	for (int i=0;i<starNum;i++)
	{
		CCSprite* star = CCSprite::create(resMgr->getSharedFilePath(starFileName[i]).c_str());
		star->setPosition(starPos[i]);
		star->setScale(0.0f);
		starBoarder->addChild(star);
		star->runAction(CCSequence::create(
			CCDelayTime::create(0.2f),
			CCDelayTime::create( (i+1)*0.3f),
			CCCallFuncN::create(this,SEL_CallFuncN(&SkipLayer::beforeShowStar)),
			CCScaleTo::create(0.1f,1.0f),
			NULL));
	}

	CCSprite* commentLable = CCSprite::create(resMgr->getFilePathByLanguageType(commentFileName[starNum-1]).c_str());
	container->addChild(commentLable);
	commentLable->setPosition(ccp(512,420));
	commentLable->setOpacity(0);
	commentLable->runAction(CCSequence::create(
		CCDelayTime::create(starNum*0.3+0.4f),
		CCCallFuncN::create(this,SEL_CallFuncN(&SkipLayer::beforShowTitle)),
		CCFadeTo::create(0.2f,255),
		NULL));
	
}

void SkipLayer::beforeShowStar(CCNode* star)
{
	toShowStarNum ++;
	string file = SpriteHelper::splicingNum_String("star%d.wav",toShowStarNum);
	Music::sharedMusicPlayer()->playEffectSound(file.c_str());
}

void SkipLayer::beforShowTitle(CCNode* title)
{
	Music::sharedMusicPlayer()->playEffectSound("laugh.wav");
}

void SkipLayer::showScore(int score)
{
	ResManager* resMgr = ResManager::getManager();
	CCSprite* finger = CCSprite::create(resMgr->getSharedFilePath("finger.png").c_str());
	finger->setScale(0.65f);
	container->addChild(finger);
	finger->setPosition(ccp(600 + 20+35,300+20));

	CCLabelAtlas* lable = CCLabelAtlas::create(
		"0123456789",
		resMgr->getSharedFilePath("season_section_num.png").c_str(),
		40, 49,//24
		'0');
	container->addChild(lable);
    
    char scoreChar[128];
    sprintf(scoreChar, "%d",score);
    lable->setString(scoreChar);
	lable->setPosition(ccp(finger->getPositionX()+35,finger->getPositionY()-lable->getContentSize().height/2));
}



void SkipLayer::restartItemPressed(cocos2d::CCObject *obj){
	Music::sharedMusicPlayer()->buttonClicked();
	LevelManager::getManager()->retry(true);
}
void SkipLayer::mainItemPressed(CCObject* obj)
{
	Music::sharedMusicPlayer()->buttonClicked();
	CCArray* children = getChildren();
	int num = children->count();
	for (int i=0;i<num;i++)
	{
		CCNode* temp = (CCNode*)children->objectAtIndex(i);
		if ( curtainLayer != temp)
		{
			temp->setVisible(false);
		}
	}

	CCScene* currentScene = CCDirector::sharedDirector()->getRunningScene();
	currentScene->removeChildByTag(Tag_Layer_Background,true);
	currentScene->removeChildByTag(Tag_Layer_shadow,true);
	currentScene->removeChildByTag(Tag_Layer_BaseContent,true);
	currentScene->removeChildByTag(Tag_Layer_Notification,true);

	LoadingLayer* layer = new LoadingLayer(Intent_GameIn2Section);
	CCDirector::sharedDirector()->getRunningScene()->addChild(layer,128);
	layer->release();
}
void SkipLayer::menuItemPressed(CCObject* obj)
{
	Music::sharedMusicPlayer()->buttonClicked();
}

void SkipLayer::nextItemPressed(cocos2d::CCObject *obj){

	Music::sharedMusicPlayer()->buttonClicked();
	if (curtainLayer)
	{
		curtainLayer->stopAllActions();
	}
	
	if (nextToSelector)
	{

		CCArray* children = getChildren();
		int num = children->count();
		for (int i=0;i<num;i++)
		{
			CCNode* temp = (CCNode*)children->objectAtIndex(i);
			if ( curtainLayer != temp)
			{
				temp->setVisible(false);
			}
		}
		LoadingLayer* layer = new LoadingLayer(Intent_GameIn2Season);
		CCDirector::sharedDirector()->getRunningScene()->addChild(layer,128);
		layer->release();
		//LevelManager::getManager()->goToSeasonUI();
		nextToSelector = false;
	}else
	{
		LevelManager::getManager()->setShowLoadingAnimation(true);
		LevelManager::getManager()->nextSection();
	}
}

void SkipLayer::setNextButtonToSelector()
{
	nextToSelector = true;
}




