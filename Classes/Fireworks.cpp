//
//  Fireworks.cpp
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-14.
//
//

#include "Fireworks.h"
#include "SpriteHelper.h"
#include "ResManager.h"

Fireworks::Fireworks(CCPoint moveBy,float flyTime,float bombTime,bool colorful /** = false **/)
{
    init();
    
    this->moveBy = SpriteHelper::createAction(CCMoveBy::create(flyTime, moveBy),
                               SEL_CallFuncO(&Fireworks::fireworksMoveEnd),
                               this);
    
    CCFiniteTimeAction* bombAction = NULL;
    
    if (colorful) {
        int bombFrameNum = 32;
        ResManager* manager = ResManager::getManager();
        std::string bombFiles[] = {
            manager->getSharedFilePath("fireworksC1.png"),manager->getSharedFilePath("fireworksC2.png"),
            manager->getSharedFilePath("fireworksC3.png"),manager->getSharedFilePath("fireworksC4.png"),
            manager->getSharedFilePath("fireworksC5.png"),manager->getSharedFilePath("fireworksC6.png"),
            manager->getSharedFilePath("fireworksC7.png"),manager->getSharedFilePath("fireworksC8.png"),
            manager->getSharedFilePath("fireworksC9.png"),manager->getSharedFilePath("fireworksC10.png"),
            manager->getSharedFilePath("fireworksC11.png"),manager->getSharedFilePath("fireworksC12.png"),
            manager->getSharedFilePath("fireworksC13.png"),manager->getSharedFilePath("fireworksC14.png"),
            manager->getSharedFilePath("fireworksC15.png"),manager->getSharedFilePath("fireworksC16.png"),
            manager->getSharedFilePath("fireworksC17.png"),manager->getSharedFilePath("fireworksC18.png"),
            manager->getSharedFilePath("fireworksC19.png"),manager->getSharedFilePath("fireworksC20.png"),
            manager->getSharedFilePath("fireworksC21.png"),manager->getSharedFilePath("fireworksC22.png"),
            manager->getSharedFilePath("fireworksC23.png"),manager->getSharedFilePath("fireworksC24.png"),
            manager->getSharedFilePath("fireworksC25.png"),manager->getSharedFilePath("fireworksC26.png"),
            manager->getSharedFilePath("fireworksC27.png"),manager->getSharedFilePath("fireworksC28.png"),
            manager->getSharedFilePath("fireworksC29.png"),manager->getSharedFilePath("fireworksC30.png"),
            manager->getSharedFilePath("fireworksC31.png"),manager->getSharedFilePath("fireworksC32.png")};
        
        //注意如果要回掉函数，那么一定不能是永远重复的
        bombAction = (CCFiniteTimeAction*)SpriteHelper::createAction(bombFiles,
                                                                     bombFrameNum,
                                                                     bombTime/bombFrameNum,
                                                                     false);
    }else{
        
        int bombFrameNum = 27;
        ResManager* manager = ResManager::getManager();
        std::string bombFiles[] = {
            manager->getSharedFilePath("fireworks1.png"),manager->getSharedFilePath("fireworks2.png"),
            manager->getSharedFilePath("fireworks3.png"),manager->getSharedFilePath("fireworks4.png"),
            manager->getSharedFilePath("fireworks5.png"),manager->getSharedFilePath("fireworks6.png"),
            manager->getSharedFilePath("fireworks7.png"),manager->getSharedFilePath("fireworks8.png"),
            manager->getSharedFilePath("fireworks9.png"),manager->getSharedFilePath("fireworks10.png"),
            manager->getSharedFilePath("fireworks11.png"),manager->getSharedFilePath("fireworks12.png"),
            manager->getSharedFilePath("fireworks13.png"),manager->getSharedFilePath("fireworks14.png"),
            manager->getSharedFilePath("fireworks15.png"),manager->getSharedFilePath("fireworks16.png"),
            manager->getSharedFilePath("fireworks17.png"),manager->getSharedFilePath("fireworks18.png"),
            manager->getSharedFilePath("fireworks19.png"),manager->getSharedFilePath("fireworks20.png"),
            manager->getSharedFilePath("fireworks21.png"),manager->getSharedFilePath("fireworks22.png"),
            manager->getSharedFilePath("fireworks23.png"),manager->getSharedFilePath("fireworks24.png"),
            manager->getSharedFilePath("fireworks25.png"),manager->getSharedFilePath("fireworks26.png"),
            manager->getSharedFilePath("fireworks27.png")};
        
        //注意如果要回掉函数，那么一定不能是永远重复的
        bombAction = (CCFiniteTimeAction*)SpriteHelper::createAction(bombFiles,
                                                                     bombFrameNum,
                                                                     bombTime/bombFrameNum,
                                                                     false);
    }
    
    this->bomb = SpriteHelper::createAction(bombAction,
                                            SEL_CallFuncO(&Fireworks::fireworksBombEnd),
                                            this);
    
    this->bomb->retain();
    this->moveBy->retain();
    
}
Fireworks::~Fireworks()
{
    CC_SAFE_RELEASE(moveBy);
    CC_SAFE_RELEASE(bomb);
}

void Fireworks::fire()
{
    CCDelayTime* delay = CCDelayTime::create(1.0f);
    CCAction* action = CCSequence::create(delay,(CCFiniteTimeAction*)moveBy,(CCFiniteTimeAction*)bomb,NULL);
    runAction(action);
}


//目前是空处理，什么也没有;
void Fireworks::fireworksMoveEnd(CCObject* fireworks)
{
    
}

void Fireworks::fireworksBombEnd(CCObject* fireworks)
{
    Fireworks* fW = (Fireworks*)fireworks;
    
    fW->removeFromParentAndCleanup(true);
}