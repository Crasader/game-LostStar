//
//  TeamInfoLayer.cpp
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-18.
//
//

#include "TeamInfoLayer.h"
#include "ResManager.h"
#include "MainFrameLayer.h"
#include "TeamInfoBackUI.h"
#include "VersionControl.h"
#include "font.h"

TeamInfoLayer::TeamInfoLayer()
{
    ResManager* resMgr = ResManager::getManager();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	//CCSprite* title = CCSprite::create(resMgr->getSharedFilePath("main_title.png").c_str());
	//addChild(title);
	//title->setPosition(ccp(winSize.width/2,winSize.height/2));
	//title->setScale(0.6f);

	string imgName = "Credits_zh1.png";
#if Version_Platform == 4
	imgName = "Credits_zh1.png";
#else
	imgName = "Credits_zh2.png";
#endif
	CCSprite* content = CCSprite::create(resMgr->getFilePathByLanguageType(g_mainlayerPath+imgName).c_str());
	//content->setPosition(ccp(winSize.width/2,winSize.height/2 - content->getContentSize().height));
	content->setPosition(ccp(winSize.width/2,winSize.height/2 - content->getContentSize().height/2));
	addChild(content);

	//版本号
	{
		CCLabelTTF *versionLab = CCLabelTTF::create(Current_Version_Code, fontStr_katong, 36);
		content->addChild(versionLab);
		versionLab->setPosition(ccp(content->getContentSize().width/2, -versionLab->getContentSize().height/2));
	}

	buttomPos = ccp(0,-50);
	topPos = ccp(0,content->getContentSize().height);


	action = CCRepeat::create(CCMoveBy::create(1.0f,ccp(0,30)),-1);
	runAction(action);
	action->retain();

	schedule(schedule_selector(TeamInfoLayer::tick));

}

TeamInfoLayer::~TeamInfoLayer()
{
    action->release();
}

CCScene* TeamInfoLayer::scene()
{
	ResManager* resMgr = ResManager::getManager();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	
	TeamInfoBackUI* bg = TeamInfoBackUI::create();

	TeamInfoLayer* inforLayer = new TeamInfoLayer();
	
	CCMenuItemImage* itemBack = CCMenuItemImage::create(resMgr->getSharedFilePath(g_buttonPath+"seasonItem_buttom_back.png").c_str(),
		resMgr->getSharedFilePath(g_buttonPath+"seasonItem_buttom_back_select.png").c_str(),
		inforLayer, SEL_MenuHandler(&TeamInfoLayer::backItemClick));
	itemBack->setPosition(ccp(58,700));
	CCMenu* menu = CCMenu::create(itemBack,NULL);
	menu->setPosition(CCPointZero);
	bg->addChild(menu,2);
	

	CCScene* s = CCScene::create();
	s->addChild(bg);
	s->addChild(inforLayer);
	inforLayer->release();

	return s;
}


void TeamInfoLayer::backItemClick(cocos2d::CCObject *obj)
{
	Music::sharedMusicPlayer()->buttonClicked();

	CCScene* scene = MainFrameLayer::createScene();
	CCTransitionFade* tran = CCTransitionFade::create(0.3f,scene);
	CCDirector::sharedDirector()->replaceScene(tran);
}

void TeamInfoLayer::lfTouchesBegan(CCPoint glPoint)
{
	stopAllActions();
	lastPos = glPoint;
}

void TeamInfoLayer::lfTouchesMoved(CCPoint glPoint)
{
	float dis = (glPoint.y - lastPos.y)/2.0f;

	if ( getPositionY() + dis < buttomPos.y )
	{
		setPositionY(buttomPos.y);
	}else
	{
		setPositionY(getPositionY() + dis);
	}

	if (getPositionY() + dis > topPos.y )
	{
		setPositionY(topPos.y);
	}else
	{
		setPositionY(getPositionY() + dis);
	}
	lastPos = glPoint;
}

void TeamInfoLayer::lfTouchesEnded(CCPoint glPoint)
{
	stopAllActions();
	runAction(action);
}

void TeamInfoLayer::tick(float time)
{
	if (getPositionY() < buttomPos.y)
	{
		//stopAllActions();
		setPositionY(buttomPos.y);
	}
	
	if (getPositionY() > topPos.y)
	{
		stopAllActions();
		setPositionY(topPos.y);
	}
}