#pragma once

#include "cocos2d.h"

using namespace cocos2d;

#define zOrder_WaitingLayer 10000
#define TAG_WaitingLayer 82953069
#define TOUCH_PRIORITY_WaitingLayer -2048

class NetworkWaitingLayer : public CCLayerColor
{
public:
	static void showWaiting(bool showWaitingSprite=true);//显示转圈否
	static void hideWaiting();


	static NetworkWaitingLayer* create(bool bShowWaitingUI=true);
	bool init(bool bShowWaitingUI);
	void setSwallowTouch(bool bSwallow=true);

	virtual void onEnter() override;
	virtual void onExit() override;

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) override;
	virtual void ccTouchMoved(CCTouch *touch, CCEvent *unused_event) override;
	virtual void ccTouchEnded(CCTouch *touch, CCEvent *unused_event) override;

protected:
	bool m_bSwallowTouch;
};