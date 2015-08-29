#ifndef __BaseMainOptionLayer_H__
#define __BaseMainOptionLayer_H__

#include "cocos2d.h"
#include <string>
using namespace std;
using namespace cocos2d;

#define PosX_start 46
#define PosY 46
#define Distance 80

class BaseMainOptionLayer : public CCLayer
{
private:
	bool action;
	CCMenu* frameButtons;
	void actionEnd(float time);
protected:
// 	CCMenuItemImage* twitter;
// 	CCMenuItemImage* facebook;
// 	CCMenuItemImage* itemSina;
	CCMenuItemImage* leaderboardImage;
	CCMenuItemImage* itemExit;
	CCMenuItemImage* setItem;
	CCMenuItemImage* resetGame;
	CCMenuItemImage* itemBackgroundSound;
	CCMenuItemImage* itemEffctiveSound;
    
    CCMenuItemImage* userInfoBtn;
    
	bool drawerOuted;

	virtual void facebookClicked(CCObject* obj);
	virtual void twitterClicked(CCObject* obj);
	virtual void sinaClicked(CCObject* obj);
	virtual void soundClicked(CCObject* obj);
	virtual void effctiveSoundClicked(CCObject* obj);
	virtual void vedioClicked(CCObject* obj);
	virtual void settingClicked(CCObject* obj);
	virtual void gameCenterItemClick(CCObject* obj);
	virtual void resetGameItemClick(CCObject* obj);
	virtual void mingxieItemClicked(CCObject* obj);
	virtual void menuExitCallback(CCObject* pSender);
	virtual void menuLeaderboardCallback(CCObject* pSender);
    
    //用户信息按钮
    virtual void menuUserInfoCallBack(CCObject* pObj);

	void hideAll(bool enable = false);

	void setItemDisable(bool able);

	string getSharedFilePath(std::string str);

public:
	BaseMainOptionLayer(void);
	~BaseMainOptionLayer(void);

	static BaseMainOptionLayer* createLayer();
};

#endif
