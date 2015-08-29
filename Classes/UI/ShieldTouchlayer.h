#pragma once
#include "cocos2d.h"
USING_NS_CC;

class ShieldTouchlayer :public CCLayer
{
public:
	ShieldTouchlayer(void);
	~ShieldTouchlayer(void);
	CREATE_FUNC(ShieldTouchlayer);
	virtual bool init();  
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void setTouchlevel(int level);
};

