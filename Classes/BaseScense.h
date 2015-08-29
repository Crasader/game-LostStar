//
//  BaseScense.h
//  demo-box2d
//
//  Created by Davidloeng loeng on 12-6-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef demo_box2d_BaseScense_h
#define demo_box2d_BaseScense_h
#include "CommonDefine.h"
#include "cocos2d.h"
//#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "Box2D/Box2D.h"
//#else
//#include "Box2D.h"
//#endif

//#include "BaseContentLayer.h"
#include "BaseBackgroundLayer.h"
#include "MenuLayer.h"
#include "AchievementNotification.h"
#include "BaseGuideLayer.h"

using namespace cocos2d;
class MenuLayer;
class BaseContentWrapLayer;
class WaitTimeToClickLayer;

#define Tag_Layer_BaseContent		124
#define Tag_Layer_Background		125
#define Tag_Layer_shadow			126
#define Tag_Layer_Notification		127
#define Tag_Layer_WatingLayer		128
#define Tag_Layer_Background_Cover	150
#define Tag_Layer_PlayRecordBaseContetWapper		151
#define Tag_Layer_PlayRecord		152
#define Tag_Layer_BaseContentLayer	153
#define Tag_Layer_BaseContetWapper	154




class BaseScense: public cocos2d::CCScene {
public:
    BaseScense(int sceneIndex);
    ~BaseScense();

	static BaseScense* getCurrentScene();
    
    void setContentLayer(BaseContentLayer* contentLayer);
    WaitTimeToClickLayer* getWaitingLayer();

	BaseContentWrapLayer* getBaseContentWrapLayer();

    void congratulation();
    void failed();
    //发生了爆炸
    void bombed(int currentBombPower,int maxPower);
    void freezyBgLayer();
    
	CCLayer* getBackgroudCoverLayer();
	CCLayer* getBackgroudLayer();
	BaseGuideLayer* getGuideLayer();

	//add by jiangjie
	bool getCollisionEnable(){return m_bCollisionEnable;};
	void setCollisionEnable(bool e){m_bCollisionEnable = e;};
	MenuLayer* getMenuLayer(){return mMenuLayer;};

	void setContentLayerWapper(BaseContentWrapLayer* contentLayerWapper, bool bAdd = true);
	void setGuideLayer(BaseGuideLayer* pGuideLayer);

	void setBackgroudLayer(BaseBackgroundLayer* pLayer);

	void setSectionIndex(int iSectionIndex){mSceneIndex = iSectionIndex;};

	void setMenuLayer(MenuLayer* pMenuLayer);

	void initAllGameLayerNeedLayers();
	//end
protected:
    BaseContentLayer* mContentLayer;
	BaseContentWrapLayer* mContentLayerWapper;
    BaseBackgroundLayer* mBackgroundLayer;
    MenuLayer* mMenuLayer;
	AchievementNotification* notificationLayer;
	BaseGuideLayer* guideLayer;

    int mSceneIndex;
    int seasonId;
    int sectionId;

	virtual void removeChildByTag(int tag, bool cleanup);

	void buySectionSuc(CCObject* obj);
	void buySectionWithMoney(CCObject* payRspWrapper);

	//add by jiangjie
	bool		m_bCollisionEnable;
	//end

	bool needUnlockAction;
	//整个游戏完成！
	void wholeGameFinished();
	//某一章玩完毕，此章的id为：seasonId，打开下一章的锁，开启第一关
	void wholeSeasonFinished(int seasonId);
	//一个Section完毕，保存关卡数据到内存
	void sectionFinished(int seasonId,int sectionId,int nextSeasonId,int nextSectionId);
};


#endif
