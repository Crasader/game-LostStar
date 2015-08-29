//
//  BackgroudLayerS6.h
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-19.
//
//

#ifndef __RedStarFall__BackgroudLayerS6__
#define __RedStarFall__BackgroudLayerS6__
#include <iostream>
#include "BaseBackgroundLayer.h"

class BackgroundLayer_Castle  : public BaseBackgroundLayer
{
public:
    BackgroundLayer_Castle(int seasonId, int seactionId);
    
private:
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    //爆炸发生了就会回调
    virtual void bombed(int currentBombPower,int maxPower);
};


//=======================================

#define Battle_TIME_ACTION 26
//战斗的动画
class Battle : public BaseDynamicObj
{
public:
    Battle();
private:
    virtual bool startAction();
    void doAction(float time);
    void appearEnd(CCObject* p);
    void deathEnd(CCObject* p);
    
    void backgroudClicked(CCPoint glPoint);
    
    bool isDisappearing;
    
    CCAction* appearAct;
    CCAction* fightAct;
    CCAction* deathAct;
};



//=======================================

#define Ghost_TIME_ACTION 19
//魔鬼的动画
class Ghost : public BaseDynamicObj
{
public:
    Ghost();
private:
    virtual bool startAction();
    void doAction(float time);
    void appearEnd(CCObject* p);
    void disappearEnd(CCObject* p);
    
    void backgroudClicked(CCPoint glPoint);
    
    bool isDisappearing;
    
    CCAction* appearAct;
    CCAction* flutterAct;
    CCAction* disappearAct;
};


//=======================================

//小怪
class Imp: public BaseDynamicObj
{
public:
    Imp();
};


//=======================================
#define Mushroom_TIME_ACTION 7
//蘑菇  season6_mushroom_move_24-ipadhd.png  season6_mushroom_blast_13-ipadhd.png
class Mushroom : public BaseDynamicObj
{
public:
    Mushroom();
private:
    virtual bool startAction();
    void doAction(float time);
    void moveEnd(CCObject* m);
    void backgroudClicked(CCPoint glPoint);
    void blastEnd(CCObject* m);
    
    CCAction* move;
    CCAction* blast;
    bool isDisappearing;
};

//=======================================
#define Saliva_TIME_ACTION 5
//口水怪  
class Saliva : public BaseDynamicObj
{
public:
    Saliva();
private:
//    virtual bool startAction();
    void doAction(float time);
    void openEnd(CCObject* s);
    void backgroudClicked(CCPoint glPoint);
    void closeEnd(CCObject* s);

    CCAction* openAct;
    CCAction* closeAct;
    bool isCloseing;
};


//=======================================
#define Skull_TIME_ACTION 8
//头骨  
class Skull : public BaseDynamicObj
{
public:
    
    Skull();

private:
//    virtual bool startAction();
    void doAction(float time);
    void blowEnd(CCObject* m);
    void backgroudClicked(CCPoint glPoint);
    void blastEnd(CCObject* m);

    bool isBlasting;
    
    CCAction* blowAct;
    CCAction* igniteAct;//燃烧
    CCAction* blastAct;//爆炸
    
};




//=======================================
#define Vampire_TIME_ACTION 32

class Vampire : public BaseDynamicObj
{
public:
    Vampire();
private:
    virtual bool startAction();
    void doAction(float time);
    
    
    CCAction* fly;
    CCAction* turn;
    CCAction* stare;
    CCAction* runaway;
};




#endif /* defined(__RedStarFall__BackgroudLayerS6__) */
