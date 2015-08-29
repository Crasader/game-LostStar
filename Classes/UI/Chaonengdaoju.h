#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Stopgameright.h"
#include "Baselisttable.h"

using namespace std;
USING_NS_CC;


class Chaonengdaoju :public CCLayer{

public:
	Chaonengdaoju(void);
	Chaonengdaoju(bool mark);
	~Chaonengdaoju(void);
	CREATE_FUNC(Chaonengdaoju);


	virtual bool init();  
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

	static Chaonengdaoju*  creteBytag(bool mark);
public:

	void drawBuju();
	void faihui(CCObject* pSender);
	void fillOuty(CCMenuItemImage* p,string imgfile);
	void actionFaihui();
	void actionFaihuiW();
	void setSizewidth(float stopwidth);
	void baselayerNode2(CCNode* node);
	void Fadeinnodes(CCNode* node);
	void Fadeoutnodes(CCNode* node);
	//void setStopgameright(Stopgameright* p);
	void usebuton(CCObject* object);

	void changeaptity(CCNode* node,float aptityvalue);
	void refreshChaondjnumber(CCObject* obj);

	void updatenumberUI();
private:
	CCSize size;
	float stopwidth;

	CCScale9Sprite* back;
	Baselisttable* list;
	CCLayer* layer_back;
	Stopgameright* game;
	CCLabelTTF* cn_lable;
	CCMenuItemImage* use_ccbt;
 	bool mark_prarent;
    
};
