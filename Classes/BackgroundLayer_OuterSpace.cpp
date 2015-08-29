//
//  BackgroudLayerS4.cpp
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-19.
//
//

#include "BackgroundLayer_OuterSpace.h"
#include "SpriteHelper.h"
#include <cmath>

BackgroundLayer_OuterSpace::BackgroundLayer_OuterSpace(int seasonId, int seactionId):BaseBackgroundLayer(seasonId,seactionId)
{
//    //创建一个航空器
//    {
//        Aircraft* aircraft = new Aircraft();
//        addChild(aircraft);
//        addDynamicObj(aircraft);
//        aircraft->release();
//        aircraft->setPosition(ccp(400,70));
//        
//        aircraft->beginToFreeMove();
//    }
    
    //创建一个采矿车
    {
        easyAddObjToLayer(new Tramcar(), ccp(130,89));
    }
    
    //创建一个火山
    {
        easyAddObjToLayer(new Volcano(), ccp(83,190),true);
    }
    
    //创建一个卫星
    {
        Satellite* s = new Satellite(ccp(727,-60),210);
        
        addChild(s);
        addDynamicObj(s);
        s->release();
    }
//    
//    {//创建一个UFO
//        easyAddObjToLayer(new UFO(), ccp(863,574));
//    }
//    
//    {//创建一个Shuttle01
//        easyAddObjToLayer(new Shuttle01(), ccp(1054,518));
//    }
//    
//    {//创建一个Shuttle02
//        easyAddObjToLayer(new Shuttle02(), ccp(913,336));
//    }
}

void BackgroundLayer_OuterSpace::ccTouchesEnded(cocos2d::CCSet *touches, cocos2d::CCEvent *event)
{
    BaseBackgroundLayer::ccTouchesEnded(touches, event);
    
}

void BackgroundLayer_OuterSpace::bombed(int currentBombPower, int maxPower)
{
    BaseBackgroundLayer::bombed(currentBombPower, maxPower);
}



//==========================================
//  航空器
//==========================================
Aircraft::Aircraft()
{
    fastFrowardAct = easyCreateAction("aircraft_fast_%d.png", 1, 4, 0.2f,true);
    breakAct = easyCreateAction("aircraft_brake_%d.png", 1, 3, 0.2f,true);
    scurryRightAct = easyCreateAction("aircraft_scurry_right_%d.png", 1, 4, 0.1f,true);
    turnRightAct = easyCreateAction("aircraft_turnright_%d.png", 1, 4, 0.1f,true);
    frowardAct = easyCreateAction("aircraft_%d.png", 1, 4, 0.1f,true);
    scurryLeftAct = easyCreateAction("aircraft_scurry_left_%d.png", 1, 4, 0.1f,true);
    turnLeftAct = easyCreateAction("aircraft_turnleft_%d.png", 1, 4, 0.1f,true);
    
    fastFrowardAct->retain();
    breakAct->retain();
    scurryRightAct->retain();
    turnRightAct->retain();
    frowardAct->retain();
    scurryLeftAct->retain();
    turnLeftAct->retain();
    
    fastFrowardMove = breakMove = scurryRightMove = turnRightMove = frowardMove = turnLeftMove = scurryLeftMove = NULL;
}

Aircraft::~Aircraft()
{
    CC_SAFE_RELEASE(fastFrowardAct);
    CC_SAFE_RELEASE(fastFrowardMove);
    
    CC_SAFE_RELEASE(breakAct);
    CC_SAFE_RELEASE(breakMove);
    
    CC_SAFE_RELEASE(scurryRightAct);
    CC_SAFE_RELEASE(scurryRightMove);
    
    CC_SAFE_RELEASE(turnRightAct);
    CC_SAFE_RELEASE(turnRightMove);
    
    CC_SAFE_RELEASE(frowardAct);
    CC_SAFE_RELEASE(frowardMove);
    
    CC_SAFE_RELEASE(turnLeftAct);
    CC_SAFE_RELEASE(turnLeftMove);
    
    CC_SAFE_RELEASE(scurryLeftAct);
    CC_SAFE_RELEASE(scurryLeftMove);
}

void Aircraft::beginToFreeMove()
{
    if (getParent() == NULL) {
        CCLog("Aircraft must be add to a node !");
        return;
    }
    
    {//测试
        
//        runAction(turnRightAct);
        //        runAction(CCMoveBy::create(5.0f, CCPoint(0, 600)));//向前运动600
        //        runAction(CCRotateBy::create(5.0f, 90.0f));//旋转90度
        
        
        
//        setRotation(0);
//        
//        runAction(CCSequence::create(
//                                     CCRotateBy::create(1.0f, 90.0f),
//                                     CCMoveTo::create(5.0f, convertToWorldSpace(CCPoint(0,600))),
//                                     NULL));
    }
    
}



//==========================================
//  采矿车
//==========================================

Tramcar::Tramcar()
{
    //自动retain，到时自己release的方法
    appearAct = easyCreateAction_Retain("car_appear_%d.png", 1, 55, 0.2f,
                                        this,//season4_car_appear_1.png
                                        SEL_CallFuncO(&Tramcar::appearEnd));
    pickAct = easyCreateAction_Retain("car_pick_%d.png", 1, 17, 0.2f,true);//season4_car_pick_17.png
    
    
    scareAct = easyCreateAction_Retain("car_scare_%d.png", 1, 16, 0.2f,
                                        this,//season4_car_scare_16.png
                                        SEL_CallFuncO(&Tramcar::scareEnd));
    
    initWithFile( resMgr->getSharedFilePath("car_appear_1.png").c_str() );
    
    schedule(schedule_selector(Tramcar::doAction),Tramcar_TIME_ACTION);
}

Tramcar::~Tramcar()
{
    
}


void Tramcar::doAction(float time)
{
    unschedule(schedule_selector(Tramcar::doAction));
    isAction = true;
    isScaring = false;
    
    runAction(appearAct);
}

void Tramcar::appearEnd(cocos2d::CCObject *car)
{
    Tramcar* tramcar = (Tramcar*)car;
    
    tramcar->stopAllActions();
    tramcar->runAction(tramcar->pickAct);
}

void Tramcar::scareEnd(cocos2d::CCObject *car)
{
    Tramcar* tramcar = (Tramcar*)car;
    tramcar->isAction = false;
    tramcar->isScaring = false;
    
    //重新开始
    tramcar->unschedule(schedule_selector(Tramcar::doAction));
    tramcar->schedule(schedule_selector(Tramcar::doAction),Tramcar_TIME_ACTION);
    
}

void Tramcar::backgroudClicked(cocos2d::CCPoint glPoint)
{
    if (!isAction) {
        unschedule(schedule_selector(Tramcar::doAction));
        schedule(schedule_selector(Tramcar::doAction),Tramcar_TIME_ACTION);
    }else{
        if (!isScaring) {
            stopAllActions();
            
            runAction(scareAct);
            
            isScaring = true;
        }
    }
}


//==========================================
//  火山
//==========================================
Volcano::Volcano()
{
    eruptionAct = easyCreateAction_Retain("volcano_%d.png", 1, 22, 0.1f,
                                          this,
                                          SEL_CallFuncO(&Volcano::actionEnd));
    schedule(schedule_selector(Volcano::doAction),Volcano_TIME_ACTION);
}

void Volcano::doAction(float time)
{
    isAction = true;
    unschedule(schedule_selector(Volcano::doAction));
    setVisible(true);
    runAction(eruptionAct);
}

void Volcano:: actionEnd(cocos2d::CCObject *v)
{
    Volcano* volcano = (Volcano*)v;
    
    volcano->stopAllActions();
    volcano->schedule(schedule_selector(Volcano::doAction),Volcano_TIME_ACTION);
    volcano->isAction = false;
    volcano->setVisible(false);
}

void Volcano::backgroudClicked(cocos2d::CCPoint glPoint)
{
    if (!isAction) {
        unschedule(schedule_selector(Volcano::doAction));
        schedule(schedule_selector(Volcano::doAction),Volcano_TIME_ACTION);
    }
}



//==========================================
//  卫星   //season4_satellite_3.png
//==========================================

Satellite::Satellite(CCPoint origin,float radius)
{
    this->origin = origin;
    this->radius = radius;
    
    action = easyCreateAction_Retain("satellite_%d.png", 1, 3, 0.1f,true);
    
    schedule(schedule_selector(Satellite::doAction),Satellite_TIME_ACTION);
    
    initWithFile(resMgr->getSharedFilePath("satellite_1.png").c_str() );
    
    currentAngel = 90.0f;
    //初始位置角度为0处,即在右边，
    setPosOnCurcel(currentAngel);

}

void Satellite::doAction(float time)
{
    unschedule(schedule_selector(Satellite::doAction));
    
    schedule(schedule_selector(Satellite::tick));
    runAction(action);
}

void Satellite::tick(float time)
{
    currentAngel += 0.1f;
//    currentAngel += 1.0f;
    
//    if (currentAngel >= 360.0f) {
//        currentAngel = 0.0f;
//    }
    
    if (currentAngel >= 200.0f) {
        currentAngel = 10.0f;//为了能不要在不可见的地方待太久了
    }
    
    setPosOnCurcel(currentAngel);
}

void Satellite::setPosOnCurcel(float angle)
{
    float deltaX = cos(CC_DEGREES_TO_RADIANS(angle))*radius;
    float deltaY = sin(CC_DEGREES_TO_RADIANS(angle))*radius;
    setPosition(ccp(origin.x + deltaX , origin.y + deltaY));
    
    setRotation(90.0f - angle);
}




//==========================================
//  UFO  //season4_ufo_appear_20.png season4_ufo_electricity_4.png season4_ufo_runaway_13.png
//==========================================

UFO::UFO()
{
    appearAct = easyCreateAction_Retain("ufo_appear_%d.png", 1, 20, 0.1f,
                                        this, SEL_CallFuncO(&UFO::appearEnd));
    
    electricityAct = easyCreateAction_Retain("ufo_electricity_%d.png", 1, 4, 0.1f,true);
    
    runAwayAct = easyCreateAction_Retain("ufo_runaway_%d.png", 1, 13, 0.1f, this,
                                         SEL_CallFuncO(&UFO::runAwayEnd));
    
    
    schedule(schedule_selector(UFO::doAction),UFO_TIME_ACTION);
}

void UFO::doAction(float time)
{
    unschedule(schedule_selector(UFO::doAction));
    isAction = true;
    isRuningAway = false;
    setVisible(true);
    runAction(appearAct);
}

void UFO::appearEnd(cocos2d::CCObject *ufo)
{
    UFO* u = (UFO*)ufo;
    
    u->stopAllActions();
    u->runAction(u->electricityAct);
    
    u->isRuningAway = false;
}

void UFO::backgroudClicked(cocos2d::CCPoint glPoint)
{
    if (!isAction) {
        unschedule(schedule_selector(UFO::doAction));
        schedule(schedule_selector(UFO::doAction),UFO_TIME_ACTION);
    }else{
        if (!isRuningAway) {
            
            stopAllActions();
            
            runAction(runAwayAct);
            
            isRuningAway = true;
        }
    }
}

void UFO::runAwayEnd(cocos2d::CCObject *ufo)
{
    UFO* u = (UFO*)ufo;
    
    u->stopAllActions();
    
    u->isAction = false;
    u->isRuningAway = false;
    u->setVisible(false);
    u->schedule(schedule_selector(UFO::doAction),UFO_TIME_ACTION);
}


//==========================================
//  一个飞船  //season4_shuttle_appear_6.png season4_shuttle_disappear_4.png season4_shuttle_fly_3.png
//==========================================


Shuttle01::Shuttle01()
{
    appearAct = easyCreateAction_Retain("shuttle_appear_%d.png", 1, 6, 0.1f,
                                        this,SEL_CallFuncO(&Shuttle01::appearEnd));
    flyAct = easyCreateAction_Retain("shuttle_fly_%d.png", 1, 3, 0.1f,true);
    
    disAppearAct = easyCreateAction_Retain("shuttle_disappear_%d.png", 1, 4, 0.1f,
                                           this, SEL_CallFuncO(&Shuttle01::disappearEnd));
    
    schedule(schedule_selector(Shuttle01::doAction),Shuttle01_TIME_ACTION);
    
    setRotation(-90.0f);
}

void Shuttle01::doAction(float time)
{
    unschedule(schedule_selector(Shuttle01::doAction));
    isAction = true;
    isDisappearing = false;
    setVisible(true);
    
    runAction(appearAct);
}

void Shuttle01::appearEnd(cocos2d::CCObject *shuttle)
{
    Shuttle01* s = (Shuttle01*)shuttle;
    s->stopAllActions();
    s->runAction(s->flyAct);
    s->lastAppearPos = s->getPosition();
    
    CCMoveTo* moveTo = CCMoveTo::create(20.0f, ccp(-50,s->getPositionY()));
    
    CCAction* action = SpriteHelper::createAction(moveTo, SEL_CallFuncO(&Shuttle01::flyOutOffWin),s);
    
    s->runAction(action);
}

void Shuttle01::flyOutOffWin(cocos2d::CCObject *shuttle)
{
    Shuttle01* s = (Shuttle01*)shuttle;
    s->setPosition(s->lastAppearPos);
    s->stopAllActions();
    s->setVisible(false);
    s->schedule(schedule_selector(Shuttle01::doAction),Shuttle01_TIME_ACTION);

}

void Shuttle01::backgroudClicked(cocos2d::CCPoint glPoint)
{
    if (!isAction) {
        unschedule(schedule_selector(Shuttle01::doAction));
        schedule(schedule_selector(Shuttle01::doAction),Shuttle01_TIME_ACTION);
    }
//    else{
//        if (!isDisappearing) {
//            
//            stopAllActions();
//            
//            runAction(disAppearAct);
//            
//            isDisappearing = true;
//        }
//    }
}

void Shuttle01::disappearEnd(cocos2d::CCObject *shuttle)
{
    Shuttle01* s = (Shuttle01*)shuttle;
    
    s->isDisappearing = false;
    s->isAction = false;
    s->setVisible(false);
    
    
    s->flyOutOffWin(shuttle);
    
}




//==========================================
//  一个飞船  //season4_shuttle2_appear_12.png season4_shuttle2_disappear_9.png season4_shuttle2_fly_3.png
//==========================================

Shuttle02::Shuttle02()
{
    appearAct = easyCreateAction_Retain("shuttle2_appear_%d.png", 1, 12, 0.1f,
                                 this, SEL_CallFuncO(&Shuttle02::appearEnd));
    flyAct = easyCreateAction_Retain("shuttle2_fly_%d.png",1,3,true);
    
    disAppearAct = easyCreateAction_Retain("shuttle2_disappear_%d.png", 1, 9, 0.1f,
                                           this, SEL_CallFuncO(&Shuttle02::disappearEnd));
    
    schedule(schedule_selector(Shuttle02::doAction),Shuttle02_TIME_ACTION);
}

void Shuttle02::doAction(float time)
{
    unschedule(schedule_selector(Shuttle02::doAction));
    lastAppearPos = getPosition();
    setVisible(true);
    isAction = true;
    isDisappearing = false;
    
    runAction(appearAct);
    
}

void Shuttle02::appearEnd(cocos2d::CCObject *shuttle)
{
    Shuttle02* s = (Shuttle02*)shuttle;
    
    s->stopAllActions();
    CCMoveTo* moveTo = CCMoveTo::create(10.0f, ccp(100,s->getPositionY()));
    CCAction* action = SpriteHelper::createAction(moveTo, SEL_CallFuncO(&Shuttle02::flyEnd),
                               s);
    s->runAction(s->flyAct);
    s->runAction(action);
}

void Shuttle02::flyEnd(cocos2d::CCObject *shuttle)
{
    Shuttle02* s = (Shuttle02*)shuttle;
    
    s->stopAllActions();
    
    s->runAction(s->disAppearAct);
    
    s->isDisappearing = true;
}

void Shuttle02::disappearEnd(cocos2d::CCObject *shuttle)
{
    Shuttle02* s = (Shuttle02*)shuttle;
    
    s->stopAllActions();
    
    s->isAction = false;
    s->isDisappearing = false;
    
    s->setVisible(false);
    
    s->setPosition(s->lastAppearPos);
    
    s->schedule(schedule_selector(Shuttle02::doAction),Shuttle02_TIME_ACTION);
}

void Shuttle02::backgroudClicked(cocos2d::CCPoint glPoint)
{
    if (!isAction) {
        unschedule(schedule_selector(Shuttle02::doAction));
        schedule(schedule_selector(Shuttle02::doAction),Shuttle02_TIME_ACTION);
    }else{
        if (!isDisappearing) {
            stopAllActions();
            
            runAction(disAppearAct);
            
            isDisappearing = true;
        }
    }
}



