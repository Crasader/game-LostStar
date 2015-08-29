#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"

using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::extension;

class Stopgame :public CCLayer
{
public:

	Stopgame(bool ismove);
	Stopgame(void);
	~Stopgame(void);
	CREATE_FUNC(Stopgame);
	virtual bool init();  
	virtual void onEnter();
	virtual void onExit();

	void drawStop();
	void allCallback(CCObject* pSender);

	void fillOuty(CCMenuItemImage* p,string imgfile);
	void runactionDh();
	void actionQux();
	void dealWith();
	void baselayerOprate();

	void baselayerNode(CCNode* node);
	void nodesallFade(CCNode* node,bool mark);
	void Fadeinnodes(CCNode* node);
	void Fadeoutnodes(CCNode* node);
	static  Stopgame* createBymark(bool isMove);
	void    Stopmsg(CCObject* object);
	void actionxiaoshi();
	void end();
public:

	bool mark;
	bool isjixu;

private:
	CCSize size;
	vector<CCMenuItemImage*> menuitems;
	CCLayer* layer_stop;
	CCScale9Sprite* sprite;
	bool isMove;
	bool button;
};






