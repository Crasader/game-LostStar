//
//  BackgroudLayerS4.h
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-19.
//
//

#ifndef __RedStarFall__BackgroudLayerS4__
#define __RedStarFall__BackgroudLayerS4__

#include <iostream>
#include "BaseBackgroundLayer.h"

class BackgroundLayer_OuterSpace  : public BaseBackgroundLayer
{
public:
    BackgroundLayer_OuterSpace(int seasonId, int seactionId);
    
private:
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    //爆炸发生了就会回调
    virtual void bombed(int currentBombPower,int maxPower);
};

//===========================================

//航空器
//在太空中乱飘

class Aircraft : public BaseDynamicObj
{
public:
    Aircraft();
    ~Aircraft();
    
    void beginToFreeMove();
private:
    CCAction* fastFrowardAct;///快速
    CCMoveBy* fastFrowardMove;
    
    CCAction* breakAct;//减速
    CCMoveBy* breakMove;
    
    CCAction* scurryRightAct;//右急转
    CCMoveBy* scurryRightMove;
    
    CCAction* turnRightAct;//右转
    CCMoveBy* turnRightMove;
    
    CCAction* frowardAct;//直行
    CCMoveBy* frowardMove;
    
    CCAction* turnLeftAct;//左转
    CCMoveBy* turnLeftMove;
    
    CCAction* scurryLeftAct;//左急转
    CCMoveBy* scurryLeftMove;
    
};


//===========================================

#define Tramcar_TIME_ACTION 15

//采矿车
class Tramcar : public BaseDynamicObj
{
public:
    Tramcar();
    ~Tramcar();
    
private:
    
    void doAction(float time);
    void appearEnd(CCObject* car);
    void scareEnd(CCObject* car);
    virtual void backgroudClicked(CCPoint glPoint);
    
    bool isScaring;
    CCFiniteTimeAction* appearAct;
    CCFiniteTimeAction* pickAct;
    CCFiniteTimeAction* scareAct;
};


//============================================

#define Volcano_TIME_ACTION 40

//火山 season4_volcano_22-ipadhd.png
class Volcano : public BaseDynamicObj
{
public:
    Volcano();
    
private:
    void doAction(float time);
    void actionEnd(CCObject* v);
    
    virtual void backgroudClicked(CCPoint glPoint);
    
    CCFiniteTimeAction* eruptionAct;//喷发
};



//============================================

#define Satellite_TIME_ACTION 2
//卫星 
class Satellite : public BaseDynamicObj
{
public:
    Satellite(CCPoint origin,float radius);
    
private:
    
    CCAction* action;
    float radius;
    CCPoint origin;
    float currentAngel;
    
    //根据指定的角度，设置他在上面的位置和他应该旋转的角度
    void setPosOnCurcel(float angle);
    void tick(float time);
    void doAction(float time);
};


//============================================
#define UFO_TIME_ACTION 2
//UFO
class UFO : public BaseDynamicObj
{
public:
    UFO();
    
private:
    void doAction(float time);
    
    void appearEnd(CCObject* ufo);
    
    void runAwayEnd(CCObject* ufo);
    
    virtual void backgroudClicked(CCPoint glPoint);
    
    bool isRuningAway;
    CCFiniteTimeAction* appearAct;
    CCFiniteTimeAction* electricityAct;//电流
    CCFiniteTimeAction* runAwayAct;
};


//============================================

#define Shuttle01_TIME_ACTION 2

//一种飞船
class Shuttle01 : public BaseDynamicObj
{
public:
    Shuttle01();
    
private:
    void doAction(float time);
    
    void appearEnd(CCObject* shuttle);
    
    void flyOutOffWin(CCObject* shuttle);
    
    virtual void backgroudClicked(cocos2d::CCPoint glPoint);
    
    void disappearEnd(CCObject* shuttle);
    
    bool isDisappearing;
    
    CCPoint lastAppearPos;
    
    CCFiniteTimeAction* appearAct;
    CCFiniteTimeAction* flyAct;
    CCFiniteTimeAction* disAppearAct;
};


//============================================

#define Shuttle02_TIME_ACTION 7

//一种飞船
class Shuttle02 : public BaseDynamicObj
{
public:
    Shuttle02();
    
private:
    void doAction(float time);
    
    void appearEnd(CCObject* shuttle);
    
    void flyEnd(CCObject* shuttle);
    
    virtual void backgroudClicked(cocos2d::CCPoint glPoint);
    
    void disappearEnd(CCObject* shuttle);
    
    bool isDisappearing;
    
    CCPoint lastAppearPos;
    
    CCFiniteTimeAction* appearAct;
    CCFiniteTimeAction* flyAct;
    CCFiniteTimeAction* disAppearAct;
};






#endif /* defined(__RedStarFall__BackgroudLayerS4__) */
