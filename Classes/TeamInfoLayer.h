//
//  TeamInfoLayer.h
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-18.
//
//

#ifndef __RedStarFall__TeamInfoLayer__
#define __RedStarFall__TeamInfoLayer__

#include <iostream>
#include "cocos2d.h"
#include <string>
#include "LFLayer.h"
#include "BaseMoveUI.h"
#include "MainThingEffect.h"

using namespace cocos2d;
using namespace std;

class TeamInfoLayer : public LFLayer
{
public:
    TeamInfoLayer();

	static CCScene* scene();

    ~TeamInfoLayer();

private:
    CCPoint buttomPos;
	CCPoint topPos;

	CCAction* action;
	CCPoint lastPos;


private:

    void backItemClick(CCObject* obj);

	void tick(float time);
    
	virtual void lfTouchesBegan(CCPoint glPoint);
	virtual void lfTouchesMoved(CCPoint glPoint);
	virtual void lfTouchesEnded(CCPoint glPoint);

};
#endif /* defined(__RedStarFall__TeamInfoLayer__) */
