#ifndef __RedStarFall__TeamInfoBackUI__
#define __RedStarFall__TeamInfoBackUI__

#include <iostream>
#include "cocos2d.h"
#include <string>
#include "BaseMoveUI.h"
#include "MainThingEffect.h"

using namespace cocos2d;
using namespace std;

class TeamInfoBackUI : public CCLayer,public BaseMoveUI
{
public:
	TeamInfoBackUI();

	virtual ~TeamInfoBackUI();

	CREATE_FUNC(TeamInfoBackUI);
public:
	virtual void moveChangeEnd(MainThingEffect *node);

	vector<MainThingEffect*>		m_sMainThingList;

	void moveThing();
};
#endif /* defined(__RedStarFall__TeamInfoLayer__) */
