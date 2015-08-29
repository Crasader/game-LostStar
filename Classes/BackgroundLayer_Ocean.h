//
//  BackgroudLayerS3.h
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-17.
//
//

#ifndef __RedStarFall__BackgroudLayerS3__
#define __RedStarFall__BackgroudLayerS3__

#include <iostream>

#include "BaseBackgroundLayer.h"

#define CRETAE_Bubble_TIME_ACTION 5
//第3章的背景
class BackgroundLayer_Ocean  : public BaseBackgroundLayer
{
public:
    BackgroundLayer_Ocean(int seasonId, int seactionId);
    
private:
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    //爆炸发生了就会回调
    virtual void bombed(int currentBombPower,int maxPower);

    void toCreateBubble(float time);
};



//=====================================

//水藻
class Kelp : public BaseDynamicObj
{
public:
    Kelp();
    ~Kelp();
private :
    //摇摆动画
    CCAction* floatAct;
};

//=====================================

#define LanternFish_TIME_ACTION 5

//灯笼鱼
class LanternFish:public BaseDynamicObj
{
public:
    LanternFish();
    ~LanternFish();
private :
    
    //开始动画,如果不需要动画，就返回false，那么系统就会自动调用下一个。
    virtual bool startAction();
    
    //游泳动画
    CCAction* swimAct;

    //惊吓之后逃跑的动画
    CCAction* scareAndRunAway;

    bool runingAway;
    
    void doAction(float time);
    void backgroudClicked(CCPoint glPoint);
    
    void runAwayMoveEnd(CCObject* fish);
    void swimend(CCObject* fish);
};



//=====================================

#define SailFish_TIME_ACTION 15

//旗鱼，快速游动即可
class SailFish:public BaseDynamicObj
{
public:
    SailFish();
    ~SailFish();
private:
    virtual bool startAction();
    void doAction(float time);
    void fastSwimEnd(CCObject* fish);
    CCAction* fastSwim;
};




//=====================================

#define Shark_TIME_ACTION 7

//鲨鱼
class Shark : public BaseDynamicObj
{
public:
    Shark();
    ~Shark();
private:
    void doAction(float time);
    void swimEnd(CCObject* shark);
    
    CCAction* swimAct;
};



//====================================

#define Turtle_TIME_ACTION 8

//海龟
class Turtle : public BaseDynamicObj
{
public:
    Turtle();
    ~Turtle();
private:
    void doAction(float time);
    
    void appearEnd(CCObject* t);
    void swimEnd(CCObject* t);
    
    CCAction* appearAct;
    CCAction* swimAct;
};



//====================================

#define Squid_TIME_ACTION 15
//乌贼
class Squid : public BaseDynamicObj
{
public:
    Squid();
    ~Squid();
private:
    virtual bool startAction();
    void doAction(float time);
    void appearEnd(CCObject* squid);
    void runAwayEnd(CCObject* squid);
    void backgroudClicked(CCPoint glPoint);
    
    bool isRuningAway;
    bool canRunAway;
    CCAction* appearAct;
    CCAction* swimAct;
    CCAction* runAwayAct;
};

//====================================

//#define Bubble_TIME_ACTION 3

//气泡
class Bubble : public BaseDynamicObj
{
public:
    static void createBubble(int num,BaseBackgroundLayer* layer,float startX,float endX,float startY,float endY);
private:
    Bubble(float posX,float posY);
    ~Bubble();
    
    void moveEnd(CCObject* bubble);
    
    CCAction* popUp;
    CCAction* scaleUp;
};








#endif /* defined(__RedStarFall__BackgroudLayerS3__) */
