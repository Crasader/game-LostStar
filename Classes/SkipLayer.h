//
//  CongratulationLayer.h
//  demo-box2d
//
//  Created by Davidloeng loeng on 12-6-27.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef demo_box2d_CongratulationLayer_h
#define demo_box2d_CongratulationLayer_h

#include "CommonDefine.h"
#include "cocos2d.h"
#include "Runtime.h"
#include "LFAlert.h"
#include "ICurtain.h"

using namespace cocos2d;

#define SCORE_LABLE_TAG 123455

class SkipLayer : public cocos2d::CCLayer,CurtainLayerCallback {
private:
    
	CCNode* container;

	int score;
	bool nextToSelector;
	CCMenu* menu;
	int toShowStarNum;

	int currentIndex;

	LFAlert* lfAlert;
	GameScore mGameScore;

	CCSprite* betterScore;		// 成绩提高了的勋章
	CCSprite* keyItemMedal;			// 关键元素的勋章
	CCNode* curtainLayer;

	virtual void curtainActionEnd();

    void showScore(int score);
	void showTime(float time);
    void scoreUpdate(float time);
	void showStar(int starNum);

	void beforeShowStar(CCNode* star);

	void beforShowTitle(CCNode* title);

	void showCommentAlert(float time);

	void timeToShow(float t);

	void lfAlertItemClicked();
public:
    SkipLayer(int scenseIndex);
    ~SkipLayer();
    void setNextButtonToSelector();
protected:

    void restartItemPressed(CCObject* obj);
	void nextItemPressed(CCObject* obj);
	void mainItemPressed(CCObject* obj);
	void menuItemPressed(CCObject* obj);

	void onEnter();
	void onExit();

    CCSize mScreenSize;
    
	int mSeasonId;
	int mSectionId;
};

#endif
