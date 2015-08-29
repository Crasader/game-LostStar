//
//  BackgroundLayer.h
//  demo-box2d
//
//  Created by Davidloeng loeng on 12-6-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef demo_box2d_BackgroundLayer_h
#define demo_box2d_BackgroundLayer_h
#include "CommonDefine.h"
#include "cocos2d.h"
#include "BaseBackgroundLayer.h"

class Bird;

class BackgroundLayer_City : public BaseBackgroundLayer {
private:
    void tick(ccTime dt);
    
public:
    BackgroundLayer_City(int seasonId, int seactionId);
    ~BackgroundLayer_City();

    list<CCNode*> getTables();

    bool isTableHaveSpaceToLand(CCNode* table);
    bool isRedStarHaveSpaceToLand(CCNode* redStar);
protected:

    
};



//====================================

#define Bird_TIME_ACTION 10

//小鸟
class Bird : public BaseDynamicObj
{
public:
    Bird();

protected:
    int currentFlyDestnation;
	~Bird();
private:     
    //开始动画
    virtual bool startAction();
    
    void doAction(float time);
    //飞行，飞到终点
	void fly(CCPoint p1,CCPoint p2,CCPoint mEndPos);
    
	void birdTurnEnd(CCObject* bird);
	void onCurveMoveEnd(CCObject* bird);
	void landingEnd(CCObject* bird);
	void disapear(CCObject* bird);
    //吓跑了
	void away();
    virtual void backgroudClicked(CCPoint glPoint);
    
    
    bool isRuning;
    
    CCAction* turnAction ;
    CCFiniteTimeAction* onCurveAction;
    CCAction* flyAction;
    CCAction* landingAction;
    CCFiniteTimeAction* peckAction;
    CCAction* runawayAction;
    
};


//====================================
#define AirShip_TIME_ACTION 5

//飞船
class AirShip :public BaseDynamicObj
{
public:
    AirShip();
private:
    
    void doAction(float time);
    void moveEnd(CCObject* airShip);
    
    virtual void backgroudClicked(CCPoint glPoint);
    
    CCAction* flyAct;
    
    CCPoint lastActionPos;
};


//====================================
#define AirPlane_TIME_ACTION 7
//飞机
class AirPlane :public BaseDynamicObj
{
public:
    AirPlane();
private:
    void doAction(float time);
    void moveEnd(CCObject* airPlane);
    
    virtual void backgroudClicked(CCPoint glPoint);
    
    CCAction* flyAct;
    CCPoint lastActionPos;
};


#endif
