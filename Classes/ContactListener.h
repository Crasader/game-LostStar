//
//  ContactListener.h
//  demo-box2d
//
//  Created by Davidloeng loeng on 12-6-20.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef demo_box2d_ContactListener_h
#define demo_box2d_ContactListener_h
#include "Box2D/Box2D.h"
#include <list>

class BaseScense;
class BaseContentLayer;

//
#define MinImpulseForCollisionEffectSound 1.0f
#define MinDeltaSpeedForCollisionEffectSound 2.0f

class ContactListener : public b2ContactListener{
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    
    BaseContentLayer* mLayer;
    
public:
    ContactListener(BaseContentLayer* layer);
};

#endif
