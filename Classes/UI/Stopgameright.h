#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Stopgame.h"

using namespace std;
USING_NS_CC;

class Stopgameright :public CCLayer
{
public:
	Stopgameright(void);
	~Stopgameright(void);

	CREATE_FUNC(Stopgameright);
	virtual bool init();  
	virtual void onEnter();
	virtual void onExit();
	void showNewChengjiuAimation(CCNode* pParent);
	void showNewChengjiuWaitingAimation(CCNode* pParent);
	void stopAction();
	void fillOuty(CCMenuItemImage* p,string imgfile);
	void allCallback(CCObject* pSender);
	void actionFaihui();
	void dealWith();
    void actionend();
	void Fadeinnodes(CCNode* node);
	void Fadeoutnodes(CCNode* node);
	//virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void setIsgb(bool mark);
	void Stopgamerightmsg(CCObject* object);
	void actionxiaoshi();
	void end();
	void payResult(CCObject* pMsg);
	void getImagecolor4Bpos(CCImage *p,CCPoint point,int idx);

	void refreshChaondj(CCObject* obj);

     

private:

	
	
	CCSize size;
	vector<CCSprite*> sps;
	vector<CCMenuItemImage*> ccbuttons;
	CCPoint poits;
	bool isGb;
	CCLayer* layer_bt;
	map<int,ccColor3B> colors; 
	bool butoon;
	CCMenuItemImage* yinx_bt;

	CCSprite* pXin;
	CCLabelTTF* num_cndj;
	CCSprite* pXiaoGuo;
};
