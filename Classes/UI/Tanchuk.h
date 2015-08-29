#pragma once

#include <map>
#include <vector>
#include "cocos-ext.h" 
#include "cocos2d.h"
#include "Baselisttable.h"

USING_NS_CC; 
USING_NS_CC_EXT; 
using namespace std;

class Tanchuk:public CCLayer
{
public:
	Tanchuk(void);
	~Tanchuk(void);

	CREATE_FUNC(Tanchuk);
	virtual bool init();  
	virtual void onEnter();
	virtual void onExit();
	static Tanchuk*  creteTag(int tag);
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

public:

	void drawNonetWork();
	void allcallback(CCObject* object);
	void fillOuty(CCMenuItemImage* p,string imgfile);

private:
	
	
	CCSize size;
	CCSize boxsize;
	int tag_type;
};

