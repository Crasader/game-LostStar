//
//  Bomb.cpp
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-13.
//
//

#include "Bomb.h"
#include "SpriteHelper.h"
#include "ResManager.h"
#include "animatepacker/AnimatePacker.h"
#include "CustomeSprite.h"

Bomb::Bomb(bool full)
{
    init();
    bombAction = NULL;
    
    if (full)
    {
// 		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("fullBomb.plist");
// 		CCFiniteTimeAction* action = (CCFiniteTimeAction*)SpriteHelper::easyCreateAction_pList("fullBomb%d.png",1,3,0.08f,false);
// 		
//         if (action != NULL) {
//             bombAction = SpriteHelper::createAction(action,this,SEL_CallFuncN(&Bomb::bombEnd));
//             bombAction->retain();
//         }
        
    }else
    {
		CCAssert(false,"不在支持half");
        //int halfLength = 10;
        //std::string halfBombPic[] = {
        //    manager->getSharedFilePath("halfBomb1.png"),manager->getSharedFilePath("halfBomb2.png"),manager->getSharedFilePath("halfBomb3.png"),
        //    manager->getSharedFilePath("halfBomb4.png"),manager->getSharedFilePath("halfBomb5.png"),manager->getSharedFilePath("halfBomb6.png"),
        //    manager->getSharedFilePath("halfBomb7.png"),manager->getSharedFilePath("halfBomb8.png"),manager->getSharedFilePath("halfBomb9.png"),
        //    manager->getSharedFilePath("halfBomb10.png")};
        //CCFiniteTimeAction* action = (CCFiniteTimeAction*)SpriteHelper::createAction(halfBombPic, halfLength, 0.08f,false);

        //if (action != NULL) {
        //    bombAction = SpriteHelper::createAction(action,SEL_CallFuncO(&Bomb::bombEnd), this);
        //    bombAction->retain();
        //}
    }
    
	//// 创建一个粒子系统
	//{
	//	par = createParticle();
	//}

}

//爆炸后的粒子效果
CCParticleSystem* Bomb::createParticle()
{
	ResManager* manager = ResManager::getManager();
	CCParticleSystem* m_emitter = new CCParticleFire();
	m_emitter->initWithTotalParticles(50);
	m_emitter->setBlendAdditive(false);

	m_emitter->setAutoRemoveOnFinish(true);
	m_emitter->setDuration(0.5f);
	m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage(manager->getSharedFilePath(g_gameinPath+"fire.png").c_str()));
	m_emitter->setPosVar(ccp(60,60));
	m_emitter->setStartColor(ccc4f(0.8f,0.8f,0.8f,0.1f));
	m_emitter->setEndColor(ccc4f(0.1f,0.1f,0.1f,0.06f));
	m_emitter->setEndColorVar(ccc4f(0.0f,0.0f,0.0f,0.0f));

	m_emitter->setLife(0.8f);
	m_emitter->setLifeVar(0.3f);
	m_emitter->setSpeed(30.0f);
	m_emitter->setSpeedVar(20.0f);

	m_emitter->setScale(getScale());

	m_emitter->autorelease();

	return m_emitter;
}

Bomb::~Bomb()
{
    CC_SAFE_RELEASE(bombAction);

	// 防止调试的时候崩溃
	if (CC_PLATFORM_IOS == CC_TARGET_PLATFORM)
	{
		CC_SAFE_RELEASE(par);
	}
}

Bomb* Bomb::createBomb(bool full)
{
    Bomb* b = new Bomb(full);
    
    b->autorelease();
    
    return b;
}
void Bomb::bomb()
{
	//CCLog("==============Bomb::bomb=======================");
	stopAllActions();
	setVisible(true);
	RecordSprite* pBomb = new RecordSprite;
	pBomb->initWithTexture(this->getTexture());
	pBomb->autorelease();
	pBomb->setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));
	this->addChild(pBomb);	
	pBomb->runAction(CCSequence::create(
		 AnimatePacker::getInstance()->getAnimate("fullbomb"), 
		 CCCallFuncN::create(this, callfuncN_selector(Bomb::bombEnd)),
		 NULL));
//     if (bombAction != NULL)
//     {
//         runAction(bombAction);
//     }else
//     {
//         CCLog("========>waring: current season have increct bomb pic !");
//     }
	par = NULL;

	//安卓平台不需要这个粒子系统
	if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	{
		par = createParticle();
		getParent()->addChild(par, 10);
		par->setPosition(getPosition());
	}
}

void Bomb::onExit()
{
	//CCLog("==============Bomb::onExit=======================");
	CCSprite::onExit();
//	BombCache::getCache()->addOneInBuffer(this);
}

void Bomb::bombEnd(CCNode *bomb)
{
	//CCLog("==============Bomb::bombEnd=======================");
    Bomb* b = (Bomb*)bomb;
	b->getParent()->setVisible(false);
	b->setVisible(false);
	b->removeFromParent();

	//放回到Buffer中
//	BombCache::getCache()->addOneInBuffer(this);
    //b->getParent()->removeChild(b, true);
}


BombCache* BombCache::cache;
BombCache::BombCache()
{
	bufferVec.clear();
// 	for (int i=0;i<10;i++)
// 	{
// 		newOneInBuffer();
// 	}
	
}

BombCache* BombCache::getCache()
{
	if (cache == NULL)
	{
		cache = new BombCache();
	}
	return cache;
	
}
void BombCache::newOneInBuffer()
{
	Bomb* bomb = new Bomb(true);
	bomb->autorelease();
	bufferVec.insert(bufferVec.end(),bomb);
}
void BombCache::addOneInBuffer(Bomb* bomb)
{
	if (bomb == NULL)
		return;
	for (size_t i=0;i<bufferVec.size();i++)
	{
		if (bomb == bufferVec[i])
			return;
	}
	bufferVec.insert(bufferVec.end(),bomb);
}
Bomb* BombCache::getBomb()
{
	if (bufferVec.size() <= 0)
		newOneInBuffer();
	Bomb* bomb = bufferVec[0];
	if (bomb == NULL)
	{
		CCAssert(false,"BombCache error!");
	}
	bufferVec.erase(bufferVec.begin());
	return bomb;
}
void BombCache::clear()
{
	for (size_t i=0;i<bufferVec.size();i++)
	{
		bufferVec[i]->release();
	}
	bufferVec.clear();
}