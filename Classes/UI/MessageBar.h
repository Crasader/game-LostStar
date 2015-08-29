#pragma once

#include "cocos2d.h"
#include <string>
using namespace cocos2d;
using std::string;

#define zOrder_MESSAGE_LAYER 3069
#define TOUCH_PRIORITY_MessageLayer -1024

class MessageBar : public CCLayerColor
{
public:
	static void show(const char* msg);

	bool init(const char* msg);

	virtual void onEnter() override;
	virtual void onExit() override;
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) override;
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) override;
};

