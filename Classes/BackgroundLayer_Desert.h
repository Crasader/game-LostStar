//
//  BackgroudLayerS2.h
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-15.
//
//

#ifndef __RedStarFall__BackgroudLayerS2__
#define __RedStarFall__BackgroudLayerS2__

#include "BaseBackgroundLayer.h"

//第二章的背景
class BackgroundLayer_Desert  : public BaseBackgroundLayer 
{
public:
    BackgroundLayer_Desert(int seasonId, int seactionId);
};

//-============================================

//乌鸦5秒之后就会开始自己的动作
#define Crow_TIME_ACTION 15
//乌鸦
class Crow : public BaseDynamicObj
{
public:
    Crow();
    ~Crow();
private:
    virtual bool startAction();
    void doAction(float time);
    void actionEnd(CCObject* crow);
    virtual void backgroudClicked(CCPoint glPoint);
    
    bool runingAway;
    CCAction* sleepAction;
    CCAction* runAwayAction;
};


//-============================================

#define Mouse_TIME_ACTION 20
//老鼠的定义
class Mouse : public BaseDynamicObj
{
public:
    Mouse();
    ~Mouse();
private:
    virtual bool startAction();
    void doAction(float time);
    void actionEnd(CCObject* mouse);
    virtual void backgroudClicked(CCPoint glPoint);
    
    CCAction* mouseApearAndShakeAct;
    CCAction* mouseRunAwayAct;
    bool runingAway;
};


//-============================================

#define Skeleton_TIME_ACTION 10

//骷髅的定义
class Skeleton : public BaseDynamicObj
{
public:
    Skeleton();
    ~Skeleton();
private:
    virtual bool startAction();
    void doAction(float time);
    void actionEnd(CCObject* skeleton);
    virtual void backgroudClicked(CCPoint glPoint);
    
    CCAction* smokeAndBlinkAct;
    CCAction* restoreAct;
    
    bool restoring;
};



#endif /* defined(__RedStarFall__BackgroudLayerS2__) */
