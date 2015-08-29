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

class CongratulationLayer : public cocos2d::CCLayer,CurtainLayerCallback {
private:
  
	CCNode* container;

	int score;
	bool nextToSelector;

	CCMenu* menu;
	GameScore mGameScore;

private:
	CCParticleSystem* createPar(CCPoint pos,const char* file,float width,float height);

	virtual void curtainActionEnd(){};
	
	void showBoarder();
	void showMenuItem(float dt);

public:
    CongratulationLayer(int scenseIndex,int oldStarNum = 0);
    ~CongratulationLayer();
    void setNextButtonToSelector();
protected:

    void restartItemPressed(CCObject* obj);
	void nextItemPressed(CCObject* obj);
	void mainItemPressed(CCObject* obj);
	void menuItemPressed(CCObject* obj);

	void onEnter();
	void onExit();
	void showZiTi(CCNode* pParent);
	void showZiTiDone(CCNode* pNode);
	CCSize mScreenSize;
    
	int mSeasonId;
	int mSectionId;

	int m_nMenuItemCount;
};

#endif
