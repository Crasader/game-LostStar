#pragma once

#include <map>
#include "cocos-ext.h" 
#include "cocos2d.h"
USING_NS_CC; 
USING_NS_CC_EXT; 

using namespace std;

class Starmain:public CCLayer
{
public:
	Starmain(void);
	~Starmain(void);

	CREATE_FUNC(Starmain);
	virtual bool init();  
	virtual void onEnter();
	virtual void onExit();

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	//动
	void dataLoaded(float percent);
	void fillOuty(CCMenuItemImage* p,string imgfile);
	void playerAnimation(CCNode* node,string animationame,CCPoint point,bool isloop);
	void animationEvent(CCArmature *armature, MovementEventType movementType, const char *movementID);
	void xinUi();
	void yuntuoXian();
	void Buttonfadein();

	void facebooklogOut(CCObject* object);
	void facebookClicked(CCObject* obj);
	void setClicked(CCObject* obj);
	void achiveClicked(CCObject* obj);
	void gamecenterClicked(CCObject* obj);
	void onFrameEvent(CCBone *bone, const char *evt, int originFrameIndex, int currentFrameIndex);
	void startItemClicked(CCObject* obj);
	void playDengDaiAnimation();

	void showNewChengjiuAimation(CCNode* pParent);
	void showNewChengjiuWaitingAimation(CCNode* pParent);
	void removeNewChengjiuAimation(CCNode* pParent);

	void update(float delta);
	void changeBGColor(CCObject* pObj);
	void facebookloginshuangxin(CCObject* obj);
	void refreshBgColor(CCObject* obj);
	void refreshlog();
	void refreshLogoutButton(CCObject* obj);
	CCScale9Sprite* getBg(){return mBG;};
	int	getTempSeaonId(){return m_iTempSeaonId;};
	void refreshLuaguge(CCObject* object);
	void getImagecolor4Bpos(CCImage *p,CCPoint point,int idx);

	void drawupdatelayer();
	void actionlayer();
	void playaction(float dt);
public:
	CCScale9Sprite* mBG;
	CCRenderTexture* mRenderStart;
private:

	float width;
	float hight;
//	CCArmature* armature;
	int mark;
	CCMenuItemImage *mStartItem;
	int m_iTempSeaonId;
	CCMenuItemImage* facebooklogoutbutton;
	CCMenuItemImage* facebooklogin;
	CCMenuItemImage* acivebutton;
	CCMenuItemImage* setbutton;
	CCMenuItemImage* gameceterbutton;
	CCLabelTTF* pLabel;
	CCLabelTTF* pLabelm;
	CCLabelTTF* pLabelm_x;
	map<int,ccColor3B> colors; 
	CCSize size;


	CCLayer* top;
	CCPoint top_start;
	CCPoint top_end;

	CCLayer* left;
	CCPoint  left_start;
	CCPoint  left_end;

	CCLayer* right;
	CCPoint  right_start;
	CCPoint  right_end;

	CCSprite* spbt;
};

