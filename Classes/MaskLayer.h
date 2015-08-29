#ifndef __MaskLayer_H__
#define __MaskLayer_H__

#include "cocos2d.h"
#include "CrossPlatformSizeManager.h"
#include "MainFrameLayer.h"
#include <vector>

using namespace std;
using namespace cocos2d;
//
//是选关场景的最上面的透明Layer，用于拦截触摸事件，实现就是点击到了menu元素，然后滑动也是可以实现的。;
//还实现了在这个场景上可以添加一些其他的效果，按钮等，比如返回按钮。;
//
//
class PageBar;
class MaskLayer : public cocos2d::CCLayer
{
private:
	int mSeasonId;
	CCMenuItemImage* nextItem;
	CCMenuItemImage* frontItem;
	PageBar* bar;
	CCMenu *mDJ_Menu;

	MaskLayer(int seasonId,int pageNum);
	void menuBackClick(CCObject* obj);
	void menuHomeClick(CCObject* obj);

	void storeClicked(CCObject *obj);
	void rankingClicked(CCObject *obj);

	void menuItemClicked_SpecialItemBar(CCObject *pSender);
	void callfuncN_menuScrollEnd(CCNode *node);

	void nextPageClicked(CCObject* obj);
	void frontPageClicked(CCObject* obj);

	void setNextButtonVisible(bool visibility);
	void setFrontButtonVisible(bool visibility);

	void listener_buySuccess(CCObject *msg);
	void refreshSpecialItem();

public:
	~MaskLayer();
	static MaskLayer* createLayer(int seasonId,int pageNum);
	static MaskLayer* getCurrentInsstance();
	void setPageIndex(int pageIndex);
	void setNext_FrontOpacity(GLubyte opacity);

};

/**
 *	页面的页面标签条
 */
class PageBar : public CCSprite
{
private:
	int mPageNum;
	int mCurrentPageIndex;
	vector<CCSprite*> itemVec;
public:
	PageBar(int pageNum);
	int getCurrentPageIndex();
	void setPageIndex(int pageIndex);
	int getPageNum();
};

#endif