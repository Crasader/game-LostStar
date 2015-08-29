#pragma once

#include <map>
#include <vector>
#include "cocos-ext.h" 
#include "cocos2d.h"


USING_NS_CC; 
USING_NS_CC_EXT; 
using namespace std;

class Gameset :public CCLayer
{
public:
	Gameset(void);
	~Gameset(void);
	CREATE_FUNC(Gameset);
	virtual bool init();  
	virtual void onEnter();
	virtual void onExit();

public:

	void drawSet();
	void drawLuaguage();
	void drawAbout();
#if 0
	bool isetLeftbutton();
	bool isrightbutton();

	void setIsyingchang();
	void setLuangugeByid(int id);
#endif
public:

	void allCallback(CCObject* object);
	void changeLanguage(int idx);
	void focusLanguageBar(CCMenuItemImage* bar);
	void onSelectLanguage(CCObject* object);
	void fillOuty(CCMenuItemImage* p,string imgfile,bool scale,float dt);
	void changeBGColor(CCObject* pObj);
private:

	CCSize size;
	CCLayer* set_layer;
	CCLayer* lag_layer;
	CCLayer* about_layer;
	int mark_l;

	CCLabelTTF* type_label;
	CCLabelTTF* fy_label;
	CCSprite* select_isp;

	CCMenuItemImage* itemleft;
	CCMenuItemImage* itemright;


	int mark_idx;

	int cz_idx;

	CCScale9Sprite* mBG;

	static const ccLanguageType SUPPORTED_LANGUAGE[];
	static const char*			SUPPORTED_LANGUAGE_NAME[];
	int _supportedLanguageNum;

	CCMenuItemImage* _curSelectLanguageItem;
	CCSprite* _lanItemMarker;
	
};

