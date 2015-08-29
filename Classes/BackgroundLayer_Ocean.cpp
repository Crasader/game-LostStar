//
//  BackgroudLayerS3.cpp
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-17.
//
//

#include "BackgroundLayer_Ocean.h"
#include "SpriteHelper.h"

BackgroundLayer_Ocean::BackgroundLayer_Ocean(int seasonId, int seactionId):BaseBackgroundLayer(seasonId,seactionId)
{
    //创建一个水藻
    {
        easyAddObjToLayer(new Kelp(), ccp(995,102));
    }
    
    //创建一个灯笼鱼
    {
        BaseDynamicObj* obj =  easyAddObjToLayer(new LanternFish(), ccp(-100,329));
        addTriggerDynamicObj(obj);
    }
    
    //创建一个旗鱼
    { 
        BaseDynamicObj* obj =  easyAddObjToLayer(new SailFish(), ccp(1144,600));
        addTriggerDynamicObj(obj);
    }
    
//    //创建一个鲨鱼
//    {
//        Shark* shark = new Shark();
//        
//        addDynamicObj(shark);
//        
//        shark->setPosition(ccp(875,585));
//        
//        addChild(shark);
//        
//        shark->release();
//    }
    
    //创建一个海龟
    {
        easyAddObjToLayer(new Turtle(), ccp(20,124));
        
    }
    
    //创建一个章鱼
    {
        BaseDynamicObj* obj =  easyAddObjToLayer(new Squid(), ccp(164,48));
        addTriggerDynamicObj(obj);
    }
    
    //创建气泡
    {
        Bubble::createBubble(7, this, 0, 1024, 0, 20);
    }
    
    schedule(schedule_selector(BackgroundLayer_Ocean::toCreateBubble),CRETAE_Bubble_TIME_ACTION);
}

void BackgroundLayer_Ocean::toCreateBubble(float time)
{
//    unschedule(schedule_selector(BackgroudLayerS3::toCreateBubble));
    int bubbleNum = 5.0f * CCRANDOM_0_1() +2.0f;
    float startX = 20.0f*CCRANDOM_0_1();
    float endX = 1000+ 24*CCRANDOM_0_1();
    
    float startY = 10*CCRANDOM_0_1();
    float endY = startY + 10*CCRANDOM_0_1();
    Bubble::createBubble(bubbleNum, this, startX, endX, startY, endY);
}

void BackgroundLayer_Ocean::ccTouchesEnded(cocos2d::CCSet *touches, cocos2d::CCEvent *event)
{
    //先调用父类的处理
    BaseBackgroundLayer::ccTouchesEnded(touches, event);
    
    unschedule(schedule_selector(BackgroundLayer_Ocean::toCreateBubble));
    schedule(schedule_selector(BackgroundLayer_Ocean::toCreateBubble),CRETAE_Bubble_TIME_ACTION);
    
}

void BackgroundLayer_Ocean::bombed(int currentBombPower, int maxPower)
{
    BaseBackgroundLayer::bombed(currentBombPower, maxPower);
    
//    //创建气泡
//    toCreateBubble(0.0f);
    float percent = (float)currentBombPower/(float)maxPower;
    int bubbleNum = 40*percent;
    
    Bubble::createBubble(bubbleNum, this, 0, 1024, 0, 20);
    
//    CCPoint pos = getPosition();
//    
//    float dis = 30 * percent;
////    float angel = getRotation();
//    float time = 0.05f;
//    
//    CCMoveTo* moveLeft = CCMoveTo::create(time, ccp(pos.x-dis,pos.y));
//    CCMoveTo* moveRight = CCMoveTo::create(time, ccp(pos.x+dis,pos.y));
//    
//    CCMoveTo* moveReset = CCMoveTo::create(time, pos);
//    
//    CCRepeat* L_R = CCRepeat::create(CCSequence::create(moveLeft,moveRight,moveReset,NULL), 2);
//
//    runAction(L_R);

}




//===================================
// 水藻
//===================================

Kelp::Kelp()
{

    floatAct = easyCreateAction_Retain("kelp_float_%d.png", 1, 5, 0.3f,true);
    
    runAction(floatAct);
}

Kelp::~Kelp()
{
}

//===================================
// 灯笼鱼
//===================================

LanternFish::LanternFish()
{

    //创建游动动画
    {
        swimAct = easyCreateAction_Retain("lanternfish_swim_%d.png", 1, 8, 0.2f,true);
    }
    
    //创建惊吓，逃跑动画
    {

        CCFiniteTimeAction* tempScareAct = easyCreateAction("lanternfish_scare.png", 1, 1, 0.5f,false);
        
        CCFiniteTimeAction* tempRunAway = easyCreateAction("lanterfish_runaway_%d.png", 1, 3, 0.2f,true);
        
        scareAndRunAway = CCSequence::create(tempScareAct,tempRunAway,NULL);
        
        scareAndRunAway->retain();
    }
    
    runingAway = false;
//    schedule(schedule_selector(LanternFish::doAction),LanternFish_TIME_ACTION);
}

LanternFish::~LanternFish()
{
    CC_SAFE_RELEASE(scareAndRunAway);
}

bool LanternFish::startAction()
{
    if (!isAction) {
         doAction(0);
    }
    return true;
}

void LanternFish::doAction(float time)
{
//    unschedule(schedule_selector(LanternFish::doAction));
    stopAllActions();
    CCFiniteTimeAction* swimMoveAct = CCMoveTo::create(10.0f, CCPointMake(1124, getPositionY()));
    CCAction* move = SpriteHelper::createAction(swimMoveAct,
                                                SEL_CallFuncO(&LanternFish::swimend),
                                                this);

    runAction(swimAct);
    runAction(move);
    isAction = true;
    runingAway = false;
}

void LanternFish::backgroudClicked(CCPoint glPoint)
{
    if (!isAction)
    {//如果还没开始运动，那么重新计时
//        unschedule(schedule_selector(LanternFish::doAction));
//        schedule(schedule_selector(LanternFish::doAction),LanternFish_TIME_ACTION);
    }else
    {
        if (!runingAway) {
            stopAllActions();
            float time = (1024-getPositionX())/1024 * 5.0f;
            CCFiniteTimeAction* runMoveAct = CCMoveTo::create(time, CCPointMake(1124, getPositionY()));
            CCAction* move = SpriteHelper::createAction(runMoveAct,
                                                        SEL_CallFuncO(&LanternFish::runAwayMoveEnd),
                                                        this);
            runAction(scareAndRunAway);
            runAction(move);
            runingAway = true;
        }
    }
}

void LanternFish::runAwayMoveEnd(cocos2d::CCObject *fish)
{
    LanternFish* lanternFish = (LanternFish*)fish;
    
    lanternFish->stopAllActions();
    lanternFish->setPositionX(-100);
    
//    lanternFish->unschedule(schedule_selector(LanternFish::doAction));
    //重新开始
//    lanternFish->schedule(schedule_selector(LanternFish::doAction),LanternFish_TIME_ACTION);
    
    lanternFish->runingAway = false;
    lanternFish->isAction = false;
}

void LanternFish::swimend(cocos2d::CCObject *fish)
{
    LanternFish* lanternFish = (LanternFish*)fish;
    lanternFish->stopAllActions();
    lanternFish->setPositionX(-100);
    
    lanternFish->isAction = false;
    
//    lanternFish->schedule(schedule_selector(LanternFish::doAction),LanternFish_TIME_ACTION+((float)(LanternFish_TIME_ACTION*2.0f))*CCRANDOM_0_1());
//    lanternFish->doAction(0);
}


//===================================
// 旗鱼  season3_sailfish_2.png
//===================================

SailFish::SailFish()
{
    
    fastSwim = easyCreateAction_Retain("sailfish_%d.png", 1, 3, 0.1f,true);
    
//    schedule(schedule_selector(SailFish::doAction),SailFish_TIME_ACTION);
}
SailFish::~SailFish()
{
}

bool SailFish::startAction()
{
    if (!isAction) {
        doAction(0);
    }
    return true;
}
void SailFish::doAction(float time)
{
//    unschedule(schedule_selector(SailFish::doAction));
    
    isAction = true;
    
    CCAction* move = SpriteHelper::createAction(CCMoveTo::create(5.0f, ccp(-100,getPositionY())),
                                                SEL_CallFuncO(&SailFish::fastSwimEnd),
                                                this);
    
    runAction(move);
    runAction(fastSwim);
}
void SailFish::fastSwimEnd(cocos2d::CCObject *fish)
{
    SailFish* sailFish = (SailFish*)fish;
    
    sailFish->stopAllActions();
    
    sailFish->setPosition(ccp(1144,sailFish->getPositionY()));
    
//    sailFish->schedule(schedule_selector(SailFish::doAction),SailFish_TIME_ACTION+((float)SailFish_TIME_ACTION*2.0f)*CCRANDOM_0_1());
    sailFish->isAction = false;
}


//===================================
// 鲨鱼  season3_shark_1.png
//===================================

Shark::Shark()
{
    swimAct = easyCreateAction("shark_%d.png"/*format*/, 1/*startNum*/,
                               3/*totalNum*/, 0.3f/*delay*/,true/*repeat*/);
    swimAct->retain();
    
    schedule(schedule_selector(Shark::doAction),Shark_TIME_ACTION);
}

Shark::~Shark()
{
    CC_SAFE_RELEASE(swimAct);
}

void Shark::doAction(float time)
{
    unschedule(schedule_selector(Shark::doAction));
    CCAction* move = SpriteHelper::createAction(CCMoveTo::create(25.f, ccp(-100,getPositionY())),
                                                SEL_CallFuncO(&Shark::swimEnd),
                                                this);
    runAction(move);
    
    runAction(swimAct);
}

void Shark::swimEnd(cocos2d::CCObject *shark)
{
    Shark* _shark = (Shark*)shark;
    
    _shark->stopAllActions();
    
    _shark->setPosition(ccp(1144,_shark->getPositionY()));
    
    _shark->schedule(schedule_selector(Shark::doAction),(float)Shark_TIME_ACTION*2.0f*CCRANDOM_0_1());
    
}


//===================================
// 海龟 season3_turtle_appear_1.png
//===================================

Turtle::Turtle()
{
    appearAct = easyCreateAction("turtle_appear_%d.png", 1, 9, 0.2f,
                                 this, SEL_CallFuncO(&Turtle::appearEnd));
    
    swimAct = easyCreateAction("turtle_swim_%d.png", 1, 4, 1.0f,true);
    
    appearAct->retain();
    
    swimAct->retain();
    
    schedule(schedule_selector(Turtle::doAction),Turtle_TIME_ACTION);
}

Turtle::~Turtle()
{
    CC_SAFE_RELEASE(swimAct);
    CC_SAFE_RELEASE(appearAct);
}

void Turtle::doAction(float time)
{
    unschedule(schedule_selector(Turtle::doAction));
    
    stopAllActions();
    
    runAction(appearAct);
}

void Turtle::appearEnd(cocos2d::CCObject *t)
{
    Turtle* turtle = (Turtle*)t;
    
    turtle->stopAllActions();
    
    CCAction* move = SpriteHelper::createAction(CCMoveTo::create(30.0f, ccp(1144,turtle->getPositionY())),
                                                SEL_CallFuncO(&Turtle::swimEnd),
                                                turtle);
    
    turtle->runAction(turtle->swimAct);
    turtle->runAction(move);
}

void Turtle::swimEnd(cocos2d::CCObject *t)
{
    Turtle* turtle = (Turtle*)t;
    
    turtle->stopAllActions();
    turtle->setPosition(ccp(-100,turtle->getPositionY()));
    
    turtle->schedule(schedule_selector(Turtle::doAction),(float)Turtle_TIME_ACTION*2.0f*CCRANDOM_0_1());
}


//===================================
// 章鱼 season3_squid_appear_5-ipadhd.png season3_squid_swim_2-ipadhd.png season3_squid_runway_2-ipadhd.png
//===================================

Squid::Squid()
{
    appearAct = easyCreateAction("squid_appear_%d.png", 1, 17, 0.1f, this, SEL_CallFuncO(&Squid::appearEnd));
    swimAct = easyCreateAction("squid_swim_%d.png", 1, 10, 0.2f,true);
    runAwayAct = easyCreateAction("squid_runway_%d.png", 1, 6, 0.1f, this, SEL_CallFuncO(&Squid::runAwayEnd));
    
    appearAct->retain();
    swimAct->retain();
    runAwayAct->retain();
    
    canRunAway = false;
    isRuningAway = false;
    
//    schedule(schedule_selector(Squid::doAction),Squid_TIME_ACTION);
}

Squid::~Squid()
{
    CC_SAFE_RELEASE(appearAct);
    CC_SAFE_RELEASE(swimAct);
    CC_SAFE_RELEASE(runAwayAct);
}
bool Squid::startAction()
{
    if (!isAction) {
        doAction(0);
    }
    return true;
}
void Squid::doAction(float time)
{
    isAction = true;
    canRunAway = false;
    isRuningAway = false;
    setVisible(true);
//    unschedule(schedule_selector(Squid::doAction));
    
    runAction(appearAct);
}

void Squid::appearEnd(cocos2d::CCObject *squid)
{
    Squid* _squid = (Squid*)squid;
    
    _squid->stopAllActions();
    
    //可以逃跑了，以免帧衔接不上
    _squid->canRunAway = true;
    
    _squid->runAction(_squid->swimAct);

}

void Squid::runAwayEnd(cocos2d::CCObject *squid)
{
    Squid* _squid = (Squid*)squid;
    
    _squid->isAction = false;
    _squid->isRuningAway = false;
    
    _squid->setVisible(false);
//    _squid->schedule(schedule_selector(Squid::doAction),Squid_TIME_ACTION);
    _squid->isAction = false;
}

void Squid::backgroudClicked(cocos2d::CCPoint glPoint)
{
    if (!isAction) {
        //重新计时
//        unschedule(schedule_selector(Squid::doAction));
//        schedule(schedule_selector(Squid::doAction),Squid_TIME_ACTION);
    }else
    {
        if (!isRuningAway && canRunAway) {
            stopAllActions();
            
            runAction(runAwayAct);
            
            isRuningAway = true;
        }
    }
}




//===================================
// 气泡 season3_bubble_1.png -- season3_bubble_7.png
//===================================

void Bubble::createBubble(int num, BaseBackgroundLayer *layer, float startX, float endX, float startY, float endY)
{
    for (int i=0; i<num; i++) {
        
        float x = startX + (endX - startX)*CCRANDOM_0_1();
        float y = startY + (endY - startY)*CCRANDOM_0_1();
        
        Bubble* b = new Bubble(x,y);
        
        layer->addDynamicObj(b);
        layer->addChild(b);
        
        b->release();
        
    }
}

Bubble::Bubble(float posX,float posY)
{
    setPosition(ccp(posX,posY));
    
    float time = CCRANDOM_0_1()*10.0f + 10.0f;
    
    int bubbleIndex = (float)CCRANDOM_0_1()*7.0f+1;
    if (bubbleIndex>7) {
        bubbleIndex = 7;
    }
    
    char fileName[56];
    sprintf(fileName, "bubble_%d.png",bubbleIndex);
    
    initWithFile(resMgr->getSharedFilePath(fileName).c_str());
    
    setScale(0.2f);
    
    popUp =
    SpriteHelper::createAction(CCMoveTo::create(time, ccp(getPositionX(),800)),
                               SEL_CallFuncO(&Bubble::moveEnd),
                               this);
    scaleUp = CCScaleTo::create(time,0.4f);
    
    
    runAction(popUp);
    runAction(scaleUp);
}

Bubble::~Bubble()
{
    
}

void Bubble::moveEnd(cocos2d::CCObject *bubble)
{
    Bubble* b = (Bubble*)bubble;
    b->removeFromParentAndCleanup(true);
    b->removeFromLayerDynamicObjList();
}

