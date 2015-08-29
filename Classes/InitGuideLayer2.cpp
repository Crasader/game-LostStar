#include "InitGuideLayer2.h"
#include "LevelManager.h"
#include "SpriteHelper.h"
#include "CurtainLayerClosedFrame.h"
#include "Music.h"
#include "VersionControl.h"
#include "DoorLayer.h"

InitGuideLayer2::InitGuideLayer2()
{
	// 播放游戏的音效
	Music::sharedMusicPlayer()->playPlayMusic();

	ResManager* resMgr = ResManager::getManager();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCSprite* bg = CCSprite::create(resMgr->getSharedFilePath(g_mainlayerPath+"main_background.png").c_str());
	bg->setScale(1382.f/1024.f);
	addChild(bg,-1);
	bg->setPosition(ccp(winSize.width/2,winSize.height/2));

	CCNode* container = CCNode::create();
	addChild(container);
	container->setPositionX((winSize.width-1024)/2.0f);
	//container->setPositionX(1024);

// 	guide_pic_1 = createSprite("guide_pic_1.png",container,ccp(257,538));
// 	guide_pic_2 = createSprite("guide_pic_2.png",container,ccp(482,538));
// 	guide_pic_2_cover = createSprite("guide_pic_2_cover.png",container,ccp(482,538));
// 	guide_pic_3 = createSprite("guide_pic_3.png",container,ccp(733,538));
// 	guide_pic_4 = createSprite("guide_pic_4.png",container,ccp(310,282));
// 	guide_pic_5 = createSprite("guide_pic_5_10.png",container,ccp(691,282));
// 
// 	guide_pic_1->setOpacity(0);
// 	guide_pic_2->setOpacity(0);
// 	guide_pic_2_cover->setOpacity(0);
// 	guide_pic_3->setOpacity(0);
// 	guide_pic_4->setOpacity(0);
// 	guide_pic_5->setOpacity(0);


	guide_pic_1 = createSprite("kaichang_juqing_1.png",container,ccp(282,566));
	guide_pic_2 = createSprite("kaichang_juqing_2.png",container,ccp(742,566));
	guide_pic_2_cover = createSprite("kaichang_juqing_2_cover.png",container,ccp(772,768-157));
	guide_pic_3 = createSprite("kaichang_juqing_3.png",container,ccp(512,768-527));

	guide_pic_1->setOpacity(0);
	guide_pic_2->setOpacity(0);
	guide_pic_2_cover->setOpacity(0);
	guide_pic_3->setOpacity(0);

// 	string format = ResManager::getManager()->getFilePrefixPath() + "guide_pic_5_%d.png";
// 	
// 	CCFiniteTimeAction* guide5Action = SpriteHelper::easyCreateAction(format.c_str(),1,10,0.15f,false);

	float delay = 0.0f;
	float dis = 1.0f;
	{
		guide_pic_1->setScale(0.0f);
		guide_pic_1->runAction(CCSequence::create(
			CCDelayTime::create(delay += 0.0f),
			CCFadeTo::create(0.01f,255),
			CCScaleTo::create(0.5f,1.2f),
			CCScaleTo::create(0.1f,1.0f),
			NULL));
	}
	{
		guide_pic_2->setScale(0.0f);
		guide_pic_2->runAction(CCSequence::create(
			CCDelayTime::create(delay += dis),
			CCFadeTo::create(0.01f,255),
			CCScaleTo::create(0.5f,1.2f),
			CCScaleTo::create(0.1f,1.0f),
			NULL));
	}
	{
		guide_pic_2_cover->setScale(0.0f);
		guide_pic_2_cover->runAction(CCSequence::create(
			CCDelayTime::create(delay += dis/2.0f),
			CCFadeTo::create(0.01f,255),
			CCScaleTo::create(0.1f,1.0f),
			CCRotateTo::create(0.1f,30.0f),
			CCRotateTo::create(0.1f,-30.0f),
			CCRotateTo::create(0.1f,0.0f),
			NULL));
	}

	{
		guide_pic_3->setScale(0.0f);
		guide_pic_3->runAction(CCSequence::create(
			CCDelayTime::create(delay += dis),
			CCFadeTo::create(0.01f,255),
			CCScaleTo::create(0.5f,1.2f),
			CCScaleTo::create(0.1f,1.0f),
			NULL));

		//guide_pic_3->runAction(CCSequence::create(
		//	CCDelayTime::create(delay += 0.3f),
		//	CCFadeTo::create(0.01f,255),
		//	//CCFadeOutTRTiles::create(ccg(30,30), 2.5f)->reverse(),
		//	CCTurnOffTiles::create(25, ccg(48,32) , 0.5)->reverse(),
		//	NULL));
	}

// 	{
// 		guide_pic_4->setScale(0.0f);
// 		guide_pic_4->runAction(CCSequence::create(
// 			CCDelayTime::create(delay += dis),
// 			CCFadeTo::create(0.01f,255),
// 			CCScaleTo::create(0.5f,1.0f),NULL));
// 	}
// 	{
// 		guide_pic_5->setScale(0.0f);
// 		guide_pic_5->runAction(CCSequence::create(
// 			CCDelayTime::create(delay += dis),
// 			CCFadeTo::create(0.01f,255),
// 			CCScaleTo::create(0.5f,1.0f),
// 			CCRepeat::create(guide5Action,-1),
// 			NULL));
// 	}

	//按钮
	{
		ResManager* resMgr = ResManager::getManager();
		CCMenuItemImage* next = CCMenuItemImage::create(
			resMgr->getSharedFilePath(g_buttonPath+"guide_next.png").c_str(),
			resMgr->getSharedFilePath(g_buttonPath+"guide_next_select.png").c_str(),
			this,
			SEL_MenuHandler(&InitGuideLayer2::nextClicked)
			);
		next->setPosition(ccp(930,84));

		CCMenuItemImage* restar_test = CCMenuItemImage::create(
			resMgr->getSharedFilePath(g_buttonPath+"restart.png").c_str(),
			resMgr->getSharedFilePath(g_buttonPath+"restart_select.png").c_str(),
			this,SEL_MenuHandler(&InitGuideLayer2::reTry));

		restar_test->setPosition(ccp(512,50));

		{
			restar_test->setEnabled(false);
			restar_test->setVisible(false);
		}
		menu = CCMenu::create(next,restar_test,NULL);
		container->addChild(menu);
		menu->setPosition(CCPointZero);
	}
	moveThing();
	//scheduleOnce(schedule_selector(InitGuideLayer2::end),3.0f);
}
CCSprite* InitGuideLayer2::createSprite(const char* file,CCNode* parent,CCPoint pos, int zOrder /*= 0*/)
{
	ResManager* resMgr = ResManager::getManager();
	CCSprite* sprite = CCSprite::create(resMgr->getSharedFilePath(file).c_str());
	parent->addChild(sprite,zOrder);
	sprite->setPosition(pos);
	return sprite;
}

void InitGuideLayer2::nextClicked(CCObject* obj)
{
	Music::sharedMusicPlayer()->buttonClicked();
	menu->setEnabled(false);
	menu->setVisible(false);
	goToGame();
}
void InitGuideLayer2::reTry(CCObject* obj)
{
	Music::sharedMusicPlayer()->buttonClicked();
	LevelManager::getManager()->goToInitGuideUI();
}
void InitGuideLayer2::end(float t)
{
	goToGame();
}
void InitGuideLayer2::goToGame()
{
	{
		LoadingLayer* layer = new LoadingLayer(Intent_Section2GameIn,1);
		CCScene* pScene = CCDirector::sharedDirector()->getRunningScene();
		pScene->addChild(layer,5);layer->release();


#if USE_DOOR_REPLACE_CURTAIN == 1
		CCLayer* bg_layer = new DoorLayer(1,1,false);
		pScene->addChild(bg_layer,4);
		bg_layer->release();
#else
		CurtainLayerClosedFrame* curtain = CurtainLayerClosedFrame::create();
		pScene->addChild(curtain,4);
#endif
		
	}
}