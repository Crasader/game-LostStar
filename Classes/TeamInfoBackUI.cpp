//
//  TeamInfoLayer.cpp
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-18.
//
//

#include "TeamInfoBackUI.h"
#include "ResManager.h"
#include "MainFrameLayer.h"

TeamInfoBackUI::TeamInfoBackUI()
{
	ResManager* resMgr = ResManager::getManager();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCSprite* backG = CCSprite::create(resMgr->getSharedFilePath(g_mainlayerPath+"main_background.png").c_str());
	backG->setScale(1382.f/1024.f);
	backG->setPosition(CCPointMake(winSize.width/2, winSize.height/2));
	addChild(backG,-1);
	int pos[8][2] = {{160,460},{222,218},{1207,313},{458,188},{680,48},{815,265},{998,172},{1241,500}};
	for (int i = 1; i < 9; i ++)
	{
		CCString *str = CCString::createWithFormat((resMgr->getSharedFilePath()+g_mainlayerPath+"main_xiaoguo_piaofu_%d.png").c_str(),i);
		CCSprite *oneSpr = CCSprite::create(str->getCString());
		MainThingEffect *one = MainThingEffect::create();
		one->initWithTexture(oneSpr->getTexture());
		//one->setAnchorPoint(CCPointZero);
		one->setPosition(ccp(pos[i - 1][0],pos[i - 1][1]));
		addChild(one,-1);
		one->setTag(i);
		one->setParent(this);
		m_sMainThingList.push_back(one);
	}
	moveThing();

}
void TeamInfoBackUI::moveThing()
{
	for (vector<MainThingEffect*>::iterator iter = m_sMainThingList.begin();iter != m_sMainThingList.end();iter++)
	{
		(*iter)->moveEffect();
	}
}
void TeamInfoBackUI::moveChangeEnd(MainThingEffect *node)
{
	node->moveEffect();
}
TeamInfoBackUI::~TeamInfoBackUI()
{
}