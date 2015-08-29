//
//  MenuLayer.h
//  demo-box2d
//
//  Created by Davidloeng loeng on 12-6-26.
//  Copyright (c) 2012? __MyCompanyName__. All rights reserved.
//

#ifndef demo_box2d_MenuLayer_h
#define demo_box2d_MenuLayer_h

#include "CommonDefine.h"
#include "cocos2d.h"
#include "BaseContentLayer.h"
#include "BaseScense.h"
#include "AddItemLayer.h"
#include "CurtainLayer.h"
#include "LFLayer.h"
#include "StarState.h"
#include "MaskLayerUI.h"
#include "LFAlerts.h"
#include "JuqingDaoJuManager.h"

using namespace cocos2d;

class Stopgame3;
class MenuLayer : public LFLayer,CurtainLayerCallback {
private:
	CCSprite		*m_lCollectLayer;
	bool			m_bIsTeachUseSuperTool;
	MaskLayerUI		*m_lMaskLayer;
	BaseContentLayer* layer;
    BaseScense* scene;
	AddItemLayer* addItemLayer;
	CCMenuItemImage* m_gongluetuItem;
	CCMenuItemImage* m_superToolItem;
	CCMenuItemImage* m_restartItem;
	StarState* starstate;

	CCMenu* gameMenu;
	CCMenu* pauseMenu;
	CCMenu* selectUIMenu;

	bool tempLastBackToMain;

	bool canJumpLevel;

	CCSprite* pauseBg;

	static MenuLayer* instance;

public:
	void showPause();		//显示暂停界面
	void showGame();		//显示游戏界面
	void showItemSelectUI();//显示暂停界面

private:
	void showRannerAd(float t);
	void hideBannerAd(float t);

	void onExit();

	virtual void curtainActionEnd();

	virtual void lfTouchesEnded(CCPoint glPoint);
	virtual void lfClick(CCPoint glPoint);

	void closeHelpClicked(CCObject* obj);

	

	void addCollectToolLayer();//游戏左侧的道具获得界面


public:
    void setGameMenuEnable(bool enbale);
    MenuLayer(int scenseIndex);
    ~MenuLayer();

    void setBaseContentLayer(BaseContentLayer* layer);
    void setBaseScene(BaseScense* scene);
	static MenuLayer* getCurrentLayer();

	CCMenu* getGameMenu();
	CCMenuItem* getSuperTools(){return m_superToolItem;}

	inline CCMenuItem* getRestartItem(){return m_restartItem;}
	StarState* getStarState(){return starstate;}
	void setStarState(StarState* pStarState){starstate = pStarState;};
	void gotoMainPage();
	void gotoSelectLevelPage();

	void pause(bool bPauseMusic = false);
	void resume(bool bResumeMusic = false);

	CCPoint getJuqingDaojuPos(int id);
	void getJuqingDaoju(JuqingDaoJu &daoju);
	inline void beginTeachUseSuperTool()
	{
		m_bIsTeachUseSuperTool = true;

		MaskLayerUI *layer = MaskLayerUI::create();
		layer->addCircle(m_superToolItem->getPosition());
		layer->setPosition(CCPointZero);
		addChild(layer,10);
		m_lMaskLayer = layer;

		m_superToolItem->runAction(CCRepeatForever::create(CCSequence::create(
			CCScaleTo::create(0.8f,1.15f),
			CCScaleTo::create(0.8f,1.0f),
			NULL)));
	}
	inline void setTeachCallBack(CCObject *obj,SEL_CallFunc handler)
	{
		m_oTeachTarger = obj;
		m_hTeachCallBackHandler = handler;
	}
	inline void teachCallBack()
	{
		CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_USE_SUPERTOOL, NULL);
		resume();
		(m_oTeachTarger->*m_hTeachCallBackHandler)();
	}
public:

	void showppauseLayer();
	void actionHou(CCNode* node);
	void Huidoanmenu(CCObject* object);
	void nodesallFade(CCNode* node,bool mark);


	void setBtnvisible(bool e);

private:

	static float time_show; 
protected:
    void restartItemPressed(CCObject* obj);
	void pauseItemPressed(CCObject* obj);
	void packageItemPressed(CCObject* obj);
	void superToolItemPressed(CCObject* obj);
	void helpItemPressed(CCObject* obj);

	void talkItemPressed(CCObject* pObj);

	void onMarketClosed(CCObject *msg);
	// 购买成功
	void buyHelpUseStarSuc(CCObject* obj);
	void buyHelpUseMoneyRsp(CCObject* payRspWrapper);
	void buyHelpMapSuccess(CCObject *msg); //market收到购买成功后才会调用这里
	// 
	void buyHelpFrameClosed(CCObject* alertNode);
	void addHelpToCurrentLayer();

	// 使用/购买超能道具窗口自动关闭了
	void superFrameClosed(CCObject* alertNode);
	//
	void useSuperTool(CCObject* alertNode);

	void buySuperToolUseMoneyRsp(CCObject* payRspWrapper);


	void pause_ResumeItemPressed(CCObject* obj);
	void pause_SelectLevelPressed(CCObject* obj);
	void pause_SkipLevelPressed(CCObject* obj);
	void pause_MainPressed(CCObject* obj);

	void buyOnePlayTimeSuc(CCObject* alertNode);
	void buyOnePlayTimeUseMoneyRsp(CCObject* payRspWrapper);
	void buyOnePlayTimeFrameDismised(CCObject* alertNode);


	//void menuItemPressed(CCObject* obj);

	void talkLoginEnd(CCObject* pObj);
	void setTalkBtnvisible(bool e);
	void soundClicked(CCObject* obj);
	void effctiveSoundClicked(CCObject* obj);
	int m_index;

    CCSize mScreenSize;
	int seasonId;
	int sectionId;

	static int mLastSeasonId;
	static int mLastSectionId;
	static int reStarNum;
	static bool needHelpItemAction;
	static bool needSuperToolAction;
	CCObject			*m_oTeachTarger;
	SEL_CallFunc		m_hTeachCallBackHandler;



	void nextSection(cocos2d::CCObject *obj);
	void preSection(cocos2d::CCObject *obj);
};


class Stopgame3 :public CCLayer
{
public:
	Stopgame3(void);
	~Stopgame3(void);
	CREATE_FUNC(Stopgame3);
	virtual bool init();  
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};

#endif
