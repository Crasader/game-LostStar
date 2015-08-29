//
//  BackgroundLayer.cpp
//  demo-box2d
//
//  Created by Davidloeng loeng on 12-6-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "BackgroundLayer_City.h"
#include "ResManager.h"
#include "SpriteHelper.h"
#include "LevelManager.h"

//只飞向桌子
#define FLY_TO_TABLE 0
//只飞向红星
#define FLY_TO_REDSTAR 1
//随机飞向桌子或者红星
#define FLY_TO_RANDOM 2

BackgroundLayer_City::BackgroundLayer_City(int seasonId, int seactionId):BaseBackgroundLayer(seasonId,seactionId)
{
    
    {//飞行物体
        easyAddObjToLayer(new AirShip(), ccp(1094,500));
        
        easyAddObjToLayer(new AirPlane(), ccp(-50,600));

        Bird* b = new Bird();
        
        easyAddObjToLayer(b, CCPointMake(945,163));
        
        addTriggerDynamicObj(b);
        
        b->setBaseBackgroundLayer(this);
    }

//	schedule( schedule_selector(BackgroundLayer::tick) );

}

bool BackgroundLayer_City::isRedStarHaveSpaceToLand(CCNode* redStar)
{
    
    float redStarTopPosX = redStar->getPositionX();
    float redStarTopPosY = redStar->getPositionY() + redStar->getContentSize().height/2;

    b2Vec2 posLB((redStarTopPosX-12)/PTM_RATIO,(redStarTopPosY+12)/PTM_RATIO);
    b2Vec2 posRB((redStarTopPosX+12)/PTM_RATIO,(redStarTopPosY+12)/PTM_RATIO);
    b2Vec2 posLT((redStarTopPosX-12)/PTM_RATIO,(redStarTopPosY+36)/PTM_RATIO);
    b2Vec2 posRT((redStarTopPosX+12)/PTM_RATIO,(redStarTopPosY+36)/PTM_RATIO);

    //如果这个区域干净，那么就让鸟飞过去
    if (baseContentLayer->whichFixtureContainThePoint(posLB) == NULL &&
        baseContentLayer->whichFixtureContainThePoint(posRB) == NULL &&
        baseContentLayer->whichFixtureContainThePoint(posLT) == NULL &&
        baseContentLayer->whichFixtureContainThePoint(posRT) == NULL)
    {
        return true;
    }

    return false;
}
bool BackgroundLayer_City::isTableHaveSpaceToLand(CCNode* table)
{
    float areaBoxCenterPosX = table->getPositionX() + table->getContentSize().width/2 - 24;
    float areaBoxCenterPosY = table->getPositionY() + table->getContentSize().height/2 + 24;
    
    b2Vec2 posLB((areaBoxCenterPosX-12)/PTM_RATIO,(areaBoxCenterPosY-12)/PTM_RATIO);
    b2Vec2 posRB((areaBoxCenterPosX+12)/PTM_RATIO,(areaBoxCenterPosY-12)/PTM_RATIO);
    b2Vec2 posLT((areaBoxCenterPosX-12)/PTM_RATIO,(areaBoxCenterPosY+12)/PTM_RATIO);
    b2Vec2 posRT((areaBoxCenterPosX+12)/PTM_RATIO,(areaBoxCenterPosY+12)/PTM_RATIO);
    
    //如果这个区域干净，那么就让鸟飞过去
    if (baseContentLayer->whichFixtureContainThePoint(posLB) == NULL &&
        baseContentLayer->whichFixtureContainThePoint(posRB) == NULL &&
        baseContentLayer->whichFixtureContainThePoint(posLT) == NULL &&
        baseContentLayer->whichFixtureContainThePoint(posRT) == NULL)
    {
        return true;
    }
    
    return false;
}

BackgroundLayer_City::~BackgroundLayer_City(){
    unschedule( schedule_selector(BackgroundLayer_City::tick) );
}


void BackgroundLayer_City::tick(ccTime dt)
{    
    
}

list<CCNode*> BackgroundLayer_City::getTables()
{
    return getTableList(baseContentLayer->getWorld());
}





//=============================================
// 下面是Bird的实现
//=============================================

int birdFlyConfig[16] = {
    FLY_TO_RANDOM,  FLY_TO_TABLE,   FLY_TO_REDSTAR, FLY_TO_RANDOM,
    FLY_TO_RANDOM,  FLY_TO_RANDOM,  FLY_TO_RANDOM,  FLY_TO_RANDOM,
    FLY_TO_TABLE,   FLY_TO_REDSTAR, FLY_TO_RANDOM,  FLY_TO_RANDOM,
    FLY_TO_TABLE,   FLY_TO_RANDOM,  FLY_TO_RANDOM,  FLY_TO_RANDOM};

Bird::Bird()
{
    isRuning = false;
    
    turnAction = easyCreateAction_Retain("bird_turn_%d.png", 1, 11, 0.1f,
                                         this, SEL_CallFuncO(&Bird::birdTurnEnd));


    flyAction = easyCreateAction_Retain("bird_fly_%d.png", 1, 16, 0.02f,true);

    onCurveAction = NULL;

    
    landingAction = easyCreateAction_Retain("bird_landing_%d.png", 1, 6, 0.1f,
                                            this, SEL_CallFuncO(&Bird::landingEnd));
    
    peckAction = easyCreateAction_Retain("bird_peck_%d.png", 1, 4, 0.14f,true);

    
    runawayAction = easyCreateAction_Retain("bird_runaway_%d.png", 1, 15, 0.1f,
                                            this, SEL_CallFuncO(&Bird::disapear));
    
    initWithFile(ResManager::getManager()->getSharedFilePath("bird_turn_1.png").c_str());
    
    
//    schedule( schedule_selector(Bird::doAction),Bird_TIME_ACTION);
    
    currentFlyDestnation = FLY_TO_RANDOM;
}
Bird::~Bird()
{
    stopAllActions();
    CC_SAFE_FREE(onCurveAction);
}
bool Bird::startAction()
{
    if (!isAction) {
        doAction(0);
    }
    
    return true;
}
void Bird::doAction(float time)
{
//    unschedule( schedule_selector(Bird::doAction));
    isRuning = false;
    isAction = true;
    

    //小鸟飞行的数据准备
    {
        int sectionId = LevelManager::getManager()->getLastSeasonId();
        float endPosX = 0.0f;
        float endPosY = 500.0f;
        CCPoint startPos = getPosition();
        
        currentFlyDestnation = birdFlyConfig[sectionId-1];
        
        BackgroundLayer_City* layer = (BackgroundLayer_City*)baseBgLayer;

        
        CCNode* dest = layer->getRedStar();
        
        bool canFly = false;
        
        //优先判断红星
        {
//            if (layer->isRedStarHaveSpaceToLand(dest)) {
//
//                endPosX = dest->getPositionX();
//                endPosY = dest->getPositionY() + dest->getContentSize().height - 15;
//            }else{
//
//                list<CCNode*> tables = layer->getTables();
//                list<CCNode*>::iterator it;
//                for (it = tables.begin(); it!=tables.end(); it++) {
//
//                    if (layer->isTableHaveSpaceToLand(*it)) {
//
//                        endPosX = (*it)->getPositionX() + (*it)->getContentSize().width/2 - 10;
//                        endPosY = (*it)->getPositionY() + (*it)->getContentSize().height - 4;
//                        
//                        break;
//                    }
//                
//                }
//                
//            }
        }

        //优先判断左桌面
        {
            bool tableCanUse = false;
            
            list<CCNode*> tables = layer->getTables();
            list<CCNode*>::iterator it;
            for (it = tables.begin(); it!=tables.end(); it++) {
                
                if (layer->isTableHaveSpaceToLand(*it)) {
                    
                    endPosX = (*it)->getPositionX() + (*it)->getContentSize().width/2 - 10;
                    endPosY = (*it)->getPositionY() + (*it)->getContentSize().height - 4;
                    
                    tableCanUse = true;
                    canFly = true;
                    break;
                }
                
            }
            
            if (!tableCanUse && dest!=NULL && layer->isRedStarHaveSpaceToLand(dest)) {
                
                endPosX = dest->getPositionX();
                endPosY = dest->getPositionY() + dest->getContentSize().height - 15;
                
                canFly = true;
            }
        }
        
        if (!canFly) {
            return;
        }
        
        CCPoint endPos = CCPointMake(endPosX - startPos.x, endPosY - startPos.y);
        

        CCPoint controlPos1 = CCPointZero;
        CCPoint controlPos2 = CCPointZero;
        
        controlPos1.x = (endPosX - startPos.x)/3;
        controlPos1.y = (endPosY - startPos.y)+100;
        
        controlPos2.x = (endPosX - startPos.x)/3 * 2;
        controlPos2.y = (endPosY - startPos.y)+50;
        
        
        fly(controlPos1,controlPos2,endPos);
    }
    
}

void Bird::fly(cocos2d::CCPoint p1, cocos2d::CCPoint p2, cocos2d::CCPoint mEndPos)
{
    isAction = true;
    isRuning = false;
    
    ccBezierConfig bezier;
    bezier.endPosition=mEndPos;
    bezier.controlPoint_1=p1;
    bezier.controlPoint_2=p2;
    
    CCFiniteTimeAction* bezierAct = CCBezierBy::create(1.0f, bezier);
    onCurveAction = (CCFiniteTimeAction*)SpriteHelper::createAction(bezierAct,
                                                                    SEL_CallFuncO(&Bird::onCurveMoveEnd),
                                                                    this);
    onCurveAction->retain();
    if (turnAction == NULL) {
        return;
    }
	runAction(turnAction);
}

void Bird::birdTurnEnd(CCObject* bird)
{

	Bird* _bird = ((Bird*)bird);
		
    if (_bird->onCurveAction == NULL || _bird->flyAction == NULL) {
        return;
    }
    
	//2个动画一起播放
	_bird->runAction(_bird->onCurveAction);
	_bird->runAction(_bird->flyAction);
}
void Bird::onCurveMoveEnd(CCObject* bird)
{

	Bird* _bird = ((Bird*)bird);

	_bird->stopAllActions();
    
    if (_bird->landingAction == NULL) {
        return;
    }
	_bird->runAction(_bird->landingAction);
}

void Bird::landingEnd(CCObject* bird)
{

	Bird* _bird = ((Bird*)bird);
    
    if (_bird->peckAction == NULL) {
        return;
    }
	_bird->runAction(_bird->peckAction);
}


void Bird::backgroudClicked(cocos2d::CCPoint glPoint)
{
    if (!isAction) {
//        unschedule( schedule_selector(Bird::doAction));
//        schedule( schedule_selector(Bird::doAction),Bird_TIME_ACTION);
    }else{
        if (!isRuning)
        {
            away();
            isRuning = true;
        }
    }
    
}

void Bird::away()
{	
	stopAllActions();
    
    if (runawayAction != NULL) {
        runAction(runawayAction);
    }
	
}
void Bird::disapear(CCObject* bird)
{
	Bird* _bird = ((Bird*)bird);
	_bird->stopAllActions();
	_bird->getParent()->removeChild(_bird,true);
    _bird->removeFromLayerDynamicObjList();
}


//=============================================
// 下面是AirShip的实现
//=============================================

AirShip::AirShip()
{
    flyAct = easyCreateAction_Retain("airship_%d.png", 1, 5, 0.1f,true);
    
    schedule(schedule_selector(AirShip::doAction),AirShip_TIME_ACTION);
}


void AirShip::doAction(float time)
{
    unschedule(schedule_selector(AirShip::doAction));
    isAction = true;
    
    lastActionPos = getPosition();
    CCMoveTo* moveTo = CCMoveTo::create(40.0f, ccp(-100,getPositionY()));
    
    CCAction* move = SpriteHelper::createAction(moveTo,
                                                SEL_CallFuncO(&AirShip::moveEnd), this);
    runAction(flyAct);
    runAction(move);
}
void AirShip::moveEnd(CCObject* airShip)
{
    AirShip* a = (AirShip*)airShip;
    
    a->setPosition(a->lastActionPos);
    
    a->stopAllActions();
    a->schedule(schedule_selector(AirShip::doAction),AirShip_TIME_ACTION*2);
}

void AirShip::backgroudClicked(CCPoint glPoint)
{
    
}




//=============================================
// 下面是AirShip的实现
//=============================================

AirPlane::AirPlane()
{
    flyAct = easyCreateAction_Retain("airplane_%d.png", 1, 5, 0.1f,true);
    
    schedule(schedule_selector(AirPlane::doAction),AirPlane_TIME_ACTION);
}


void AirPlane::doAction(float time)
{
    unschedule(schedule_selector(AirPlane::doAction));
    isAction = true;
    
    lastActionPos = getPosition();
    CCMoveTo* moveTo = CCMoveTo::create(20.0f, ccp(1124,getPositionY()));
    
    CCAction* move = SpriteHelper::createAction(moveTo,
                                                SEL_CallFuncO(&AirPlane::moveEnd), this);
    runAction(flyAct);
    runAction(move);
}
void AirPlane::moveEnd(CCObject* airPlane)
{
    AirPlane* a = (AirPlane*)airPlane;
    
    a->setPosition(a->lastActionPos);
    
    a->stopAllActions();
    a->schedule(schedule_selector(AirPlane::doAction),((float)AirPlane_TIME_ACTION*3.0f)*CCRANDOM_0_1());
}

void AirPlane::backgroudClicked(CCPoint glPoint)
{
    
}






