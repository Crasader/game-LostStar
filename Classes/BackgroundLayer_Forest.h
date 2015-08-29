//
//  BackgroudLayerS5.h
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-19.
//
//

#ifndef __RedStarFall__BackgroudLayerS5__
#define __RedStarFall__BackgroudLayerS5__

#include <iostream>
#include "BaseBackgroundLayer.h"

#define CRETAE_Dandelion_TIME_ACTION 5

class BackgroundLayer_Forest  : public BaseBackgroundLayer
{
public:
    BackgroundLayer_Forest(int seasonId, int seactionId);
    
private:
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    //爆炸发生了就会回调
    virtual void bombed(int currentBombPower,int maxPower);
    
    void createDandelion(float time);
};


//=======================================

//飞絮的配置信息
typedef struct _DandelionConfig{
    float startMinX;
    float startMaxX;
    float startMinY;
    float startMaxY;
    float endMinX;
    float endMaxX;
    float endMinY;
    float endMaxY;
    
    int num;
    
    //默认是漫天飞舞
    _DandelionConfig()
    {//默认是漫天飞舞
        startMinX = 0;
        startMaxX = 1024;
        
        startMinY = 0;
        startMaxY = 768;
        
        endMinX = 0;
        endMaxX = 1024;
        
        endMinY = 0;
        endMaxY = 1024;
        
        num = 100;
    }
    
    _DandelionConfig& operator = (const _DandelionConfig& obj){
        
        startMinX = obj.startMinX;
        startMaxX = obj.startMaxX;
        
        startMinY = obj.startMinY;
        startMaxY = obj.startMaxY;
        
        endMinX = obj.endMinX;
        endMaxX = obj.endMaxX;
        
        endMinY = obj.endMinY;
        endMaxY = obj.endMaxY;
        
        num = obj.num;
        
        return *this;
    };

    
} DandelionConfig;

//飞絮
class Dandelion : public BaseDynamicObj
{
public:
    static void createDandelionsIntoLayer(const DandelionConfig config,BaseBackgroundLayer* layer);
private:
    Dandelion(float posx,float posy,float endPosX,float endPosY);
    
    //到达终点
    void arrived(CCNode* dandelion);
    
    CCAction* createFallAction(float fallTime,float endX,float endY,float roTime,
                               float fAngle1,float fAngle2,SEL_CallFuncN callBack);
    
    
    float endPosX;
    float endPosY;
    
    float time;//叶子下落的时间 = 10
    float roTime;//叶子单向摆动一次时间 = 2.5
    float fAngle1;//叶子逆时针摆动角度 = -80
    float fAngle2;//顺时针摆动角度 = 80
};



//=======================================

#define Monkey_TIME_ACTION 10

//猴子
class Monkey : public BaseDynamicObj
{
public:
    Monkey();
private:
    virtual bool startAction();
    void doAction(float time);
    void monkeyAppearEnd(CCObject* m);
    void monkeyRunEnd(CCObject* m);
    void backgroudClicked(CCPoint glPoint);
    
    bool isRuning;
    CCAction* monkeyAppearAct;
    CCAction* monkeyBlinkAct;
    CCAction* monkeyRunAwayAct;
};

//=======================================

#define Plant_TIME_ACTION 25
//植物
class Plant : public BaseDynamicObj
{
public:
    Plant();
private:
    virtual bool startAction();
    void doAction(float time);
    void growUpEnd(CCObject* p);
    void disappearEnd(CCObject* p);
    
    void backgroudClicked(CCPoint glPoint);
    
    bool isDisappearing;
    
    CCAction* growUpAct;
    CCAction* devilAct;//出现魔鬼
    CCAction* disappearAct;
};




//=======================================

#define PumpkinEyes_TIME_ACTION 5
//南瓜眼睛
class PumpkinEyes : public BaseDynamicObj
{
public:
    PumpkinEyes();
private:
    void doAction(float time);

    CCAction* blink;
};


//=======================================
//蜗牛
class Snail : public BaseDynamicObj
{
public:
    Snail();
private:
    void actionEnd(CCObject* snail);
    void backgroudClicked(CCPoint glPoint);
    CCAction* leftAct;
    CCAction* rightAct;
};

//========================================
//蜘蛛 season5_spider_down_15.png  season5_spider_up_14.png

#define Spider_TIME_ACTION 8

class Spider  : public BaseDynamicObj
{
public:
    Spider();
private:
    virtual bool startAction();
    void doAction(float time);
    void actionUpEnd(CCObject* s);
    void actionDownEnd(CCObject* s);
    void backgroudClicked(CCPoint glPoint);
    
    bool isUping;
    CCAction* actionUp;
    CCAction* actionDown;
};



#endif /* defined(__RedStarFall__BackgroudLayerS5__) */
