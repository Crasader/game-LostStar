#include "AchievementNotification.h"
#include "ResManager.h"
#include "SpriteHelper.h"
#include "Music.h"
#include <vector>
#include "LFFileUtiles.h"
#include "cocos-ext.h"
#include "font.h"
USING_NS_CC_EXT;

#define  Tag_Board  11111

vector<Achievement> waitingTaskVec;

vector<Achievement*> temp_Must_Delete;

void addOneTask(Achievement a)
{
	// 加在后面
	waitingTaskVec.insert(waitingTaskVec.end(),a);
}
// 返回第一个，就是最先加入的一个
Achievement getAchievement()
{
	return waitingTaskVec[0];
}
void removeOneTask(Achievement a)
{
	// 从前面找，找到就移除
	vector<Achievement>::iterator it = waitingTaskVec.begin();
	for(;it != waitingTaskVec.end();it++)
	{
		if ( (*it) == a)
		{
			waitingTaskVec.erase(it);
			break;
		}
	}
}

void removeAllTask()
{
    waitingTaskVec.clear();
}

AchievementNotification* AchievementNotification::instance;

AchievementNotification::AchievementNotification()
{
	ResManager* resMgr = ResManager::getManager();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	fileName = resMgr->getSharedFilePath(g_achievPath+"CJ_tanchu_bg.png");
	CCTextureCache::sharedTextureCache()->addImage(fileName.c_str());

	parFileName = resMgr->getSharedFilePath(g_gameinPath+"card.png");

	Music::sharedMusicPlayer()->loadEffectSound("achievement.wav");

	mShowing = false;
	forceTermination = false;

	instance = this;
}

AchievementNotification* AchievementNotification::create()
{
	AchievementNotification* layer = new AchievementNotification();
	layer->autorelease();
	return layer;
}
AchievementNotification* AchievementNotification::currentLayer()
{
	return instance;
}

void AchievementNotification::onEnter()
{
	LFLayer::onEnter();
	//CCLog("AchievementNotification::onEnter ----- ");

	scheduleOnce(schedule_selector(AchievementNotification::startMonitoring),1.0f);
}
void AchievementNotification::onEnterTransitionDidFinish()
{
	LFLayer::onEnterTransitionDidFinish();
	//CCLog("AchievementNotification::onEnterTransitionDidFinish ----- ");
	scheduleOnce(schedule_selector(AchievementNotification::startMonitoring),1.0f);
}
void AchievementNotification::onExit()
{
	LFLayer::onExit();
	//CCLog("AchievementNotification::onExit ----- ");

	for (size_t i=0;i<temp_Must_Delete.size();i++)
	{
		if (temp_Must_Delete[i])
		{
			delete temp_Must_Delete[i];
		}
	}
	temp_Must_Delete.clear();

	forceTermination = true;	// 设置强制终止
	unschedule(schedule_selector(AchievementNotification::tick));
	removeAllChildrenWithCleanup(true);
}
void AchievementNotification::startMonitoring(float t)
{
	unschedule(schedule_selector(AchievementNotification::tick));
	schedule(schedule_selector(AchievementNotification::tick));
}
void AchievementNotification::stopMonitoring()
{
	unschedule(schedule_selector(AchievementNotification::tick));
}
void AchievementNotification::tick(float t)
{
	// 还有没有完成的通知
	if (waitingTaskVec.size() > 0)
	{
		// 如果当前没有显示，那么就立刻显示一个
		if(!mShowing)
		{
			showImmediately(getAchievement());
		}
	}
}
void AchievementNotification::showAchievement(Achievement a)
{
	addOneTask(a);
}

void AchievementNotification::showImmediately(Achievement a)
{
	mShowing = true;
	CCTexture2D* tex = CCTextureCache::sharedTextureCache()->textureForKey(fileName.c_str());
	if (tex == NULL)
	{
		tex = CCTextureCache::sharedTextureCache()->addImage(fileName.c_str());
	}
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();


	CCScale9Sprite* pBoard = CCScale9Sprite::create(CCRectMake(tex->getContentSize().width/2, 0, 2, tex->getContentSize().height), fileName.c_str());
	addChild(pBoard,0,Tag_Board);
	pBoard->setPosition(ccp(winSize.width/2,winSize.height + pBoard->getContentSize().height) );

	CCSprite* pJinBei = CCSprite::create(ResManager::getManager()->getFilePath(g_public+"CJ_icon_jiangbei.png").c_str());
	pBoard->addChild(pJinBei, 1);
	pJinBei->setPosition(ccp(pJinBei->getContentSize().width, pBoard->getContentSize().height/2));

	CCLabelTTF* pName = CCLabelTTF::create(a.name.c_str(), fontStr_KlavikaBold, 40);
	pName->setColor(fontColor_Acheivement_TTF);
	CCSprite* pIcon = CCSprite::create(ResManager::getManager()->getFilePath(g_public+"CJ_icon_wancheng.png").c_str());
	int x = pName->getContentSize().width + pIcon->getContentSize().width - 420;
	if (x > 0)
	{
		pBoard->setContentSize(ccp(pBoard->getContentSize().width+x, pBoard->getContentSize().height));
	}
	pBoard->addChild(pName);

	int xoffset = 10;

	pName->setPosition(ccp(pBoard->getContentSize().width/2 + pIcon->getContentSize().width/2 + xoffset, pBoard->getContentSize().height/2));

	pBoard->addChild(pIcon);
	pIcon->setPosition(ccp(pName->getPositionX() - pName->getContentSize().width/2 - pIcon->getContentSize().width/2 - xoffset, pName->getPositionY()));

	Achievement* achievementCopy = new Achievement(a);
//	*achievementCopy = a;
	temp_Must_Delete.insert(temp_Must_Delete.end(),achievementCopy);

	pBoard->runAction(CCSequence::create(
		CCMoveTo::create(0.5f,ccp(pBoard->getPositionX(),winSize.height-pBoard->getContentSize().height/2)),
		CCDelayTime::create(0.1f),
		CCCallFuncND::create(this,SEL_CallFuncND(&AchievementNotification::boardShowing),achievementCopy),
		CCDelayTime::create(1.3f),
		CCMoveTo::create(0.2f,ccp(pBoard->getPositionX(),winSize.height+pBoard->getContentSize().height)),
		CCFadeTo::create(0.1f,0),
		CCCallFuncN::create(this,SEL_CallFuncN(&AchievementNotification::boardShowEnd)),NULL));

	createPar(ccp(winSize.width/2,winSize.height),parFileName.c_str(),pBoard->getContentSize().width * 1.2f,pBoard->getContentSize().height);

	Music::sharedMusicPlayer()->playEffectSound("achievement.wav");

	CCLog("AchievementNotification::showImmediately name = %s" ,a.name.c_str());
}

void AchievementNotification::boardShowing(CCNode* board,void* achievement)
{
	Achievement* a = (Achievement*)achievement;
	// 正常运行的情况，才会把这个任务移除，不然就加入到队列，再显示一次
	if (!forceTermination)
		removeOneTask(*a);

	CCLog("AchievementNotification::boardShowing , name = %s",a->name.c_str());

	// 清除
	for (int i=0;i < temp_Must_Delete.size();i++)
	{
		if (temp_Must_Delete[i] == a)
		{
			temp_Must_Delete[i] = NULL;
            temp_Must_Delete.erase(temp_Must_Delete.begin()+i);
			CC_SAFE_DELETE(a);

			break;
		}
	}
}
void AchievementNotification::boardShowEnd(CCNode* board)
{
	//Achievement* a = (Achievement*)achievement;
	board->removeFromParentAndCleanup(true);
	CCNode* boardTemp = getChildByTag(Tag_Board);
	if (boardTemp != NULL) // 还有board在显示
	{
		mShowing = true;
	}else
	{
		mShowing = false;
	}

	//CCLog("AchievementNotification::boardShowEnd , name = %s",a->achievementName.c_str());

	//// 正常运行的情况，才会把这个任务移除，不然就加入到队列，再显示一次
	//if (!forceTermination)
	//	removeOneTask(*a);

	//// 清除
	//for (int i=temp_Must_Delete.size() - 1 ;i >= 0;i--)
	//{
	//	if (temp_Must_Delete[i] == a)
	//	{
	//		temp_Must_Delete[i] = NULL;

	//		delete a;

	//		break;
	//	}
	//}
}

void AchievementNotification::setString(CCLabelTTF* lable , CCSprite* base,string str)
{
	lable->setString(str.c_str());

	lable->setPositionY(base->getContentSize().height/2);
	lable->setPositionX(base->getContentSize().width/2);
}

CCParticleSystem* AchievementNotification::createPar(CCPoint pos,const char* file,float width,float height)
{
	CCParticleSystem *m_emitter = new CCParticleSnow();
	m_emitter->initWithTotalParticles(100);
	addChild(m_emitter, 10);

	m_emitter->setPosition(pos);
	m_emitter->setPosVar(ccp(width,height));
	m_emitter->setLife(1.0f);
	m_emitter->setLifeVar(0.5f);
	m_emitter->setAutoRemoveOnFinish(true);
	m_emitter->setDuration(1.0f);
	m_emitter->setGravity(CCPointMake(0,-10));
	m_emitter->setSpeed(130);
	m_emitter->setSpeedVar(30);
	m_emitter->setStartColor(ccc4f(0.5f,0.5f,0.5f,1.0f));
	m_emitter->setStartColorVar(ccc4f(0.5f,0.5f,0.5f,0.1f));
	m_emitter->setEndColor(ccc4f(0.1f,0.1f,0.1f,0.2f));
	m_emitter->setEndColorVar(ccc4f(0.1f,0.1f,0.1f,0.2f));
	m_emitter->setEndSpinVar(360);
	m_emitter->setEmissionRate(m_emitter->getTotalParticles()/m_emitter->getLife());
	m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage(file) );
	m_emitter->setBlendAdditive(true);

	return m_emitter;

}