//
//  Fireworks.h
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-14.
//
//

#ifndef __RedStarFall__Fireworks__
#define __RedStarFall__Fireworks__

#include "cocos2d.h"
using namespace cocos2d;

//一个礼花的类
class Fireworks : public CCSprite 
{
public:
    //构造一个礼花，礼花的射程以及偏移等都是由moveBy控制
    //礼花到达moveBy的位置之后就会发生爆炸,
    Fireworks(CCPoint moveBy,float flyTime,float bombTime,bool colorful = false);
    
    //发送礼花
    void fire();
    
    ~Fireworks();
protected:
    void fireworksMoveEnd(CCObject* fireworks);
    void fireworksBombEnd(CCObject* fireworks);
    
    CCAction* moveBy;
    CCAction* bomb;
};


#endif /* defined(__RedStarFall__Fireworks__) */
