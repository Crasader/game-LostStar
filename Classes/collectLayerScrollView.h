#ifndef __collectLayerScrollView_h_
#define __collectLayerScrollView_h_

#include "cocos2d.h"
#include "GridScrollView.h"

class collectLayerScrollView : public GridScrollView, public ClickableItemCreater
{
public:
	collectLayerScrollView(CCSize viewPortSize,int gridNum,int gridNumInX,int gridNumInY,float gridPadding = 10.0f,CCPoint scrollViewPortOffset = CCPointZero,float pageInval = 40.0f);
	~collectLayerScrollView(void);

	static collectLayerScrollView* createView(CCSize viewPortSize,int gridNum,int gridNumInX,int gridNumInY,float gridPadding = 10.0f, CCPoint scrollViewPortOffset = CCPointZero,float pageInval = 40.0f);

protected:
	//重写父类的;
	//创建一个Item，根据传入的页面号，x(从左到右的号)，y(从上到下的号)
	virtual ClickableItem* createGridItem(int pageIndex,int xIndex,int yIndex);

	//重写，为了拦截滑动事件，以通知MaskLayer是否显示上一页，下一页的按钮
	virtual void pageSelected(int pageIndex){};
	virtual void pageSelectedEnd(int pageIndex);
	void scheduleUpTableViewPrioority(CCNode* pNode);
};

class collectItem : public ClickableItem
{
public:
	collectItem();
	~collectItem();

	static collectItem* createColletItem(std::string titleFilename);
	bool initItem(std::string titleFilename);

protected:
	virtual void clickedMe()
	{
	}
	//触摸到了我，那么默认是放大
	virtual void touchMe()
	{
	}
	//手指离开了我，默认是变回原大小
	virtual void leaveMe()
	{
	}
};

#endif // __collectLayerScrollView_h_