#ifndef __collectLayerDaojuScrollView_h_
#define __collectLayerDaojuScrollView_h_

#include "cocos2d.h"
#include "GridScrollView.h"
#include "JuqingDaoJuManager.h"

class collectLayerDaojuScrollView : public GridScrollView, public ClickableItemCreater
{
public:
	collectLayerDaojuScrollView(CCSize viewPortSize,int gridNum,int gridNumInX,int gridNumInY,float gridPadding = 10.0f,CCPoint scrollViewPortOffset = CCPointZero,float pageInval = 40.0f);
	~collectLayerDaojuScrollView(void);

	static collectLayerDaojuScrollView* createView(CCSize viewPortSize,int gridNum,int gridNumInX,int gridNumInY,float gridPadding = 10.0f, CCPoint scrollViewPortOffset = CCPointZero,float pageInval = 40.0f);

protected:
	//重写父类的;
	//创建一个Item，根据传入的页面号，x(从左到右的号)，y(从上到下的号)
	virtual ClickableItem* createGridItem(int pageIndex,int xIndex,int yIndex);

	//重写，为了拦截滑动事件，以通知MaskLayer是否显示上一页，下一页的按钮
	virtual void pageSelected(int pageIndex){};
	virtual void pageSelectedEnd(int pageIndex);
	void scheduleUpTableViewPrioority(CCNode* pNode);

	std::vector<JuqingDaoJu>	m_vDaojuInfoList;
};

class collectDaojuItem : public ClickableItem
{
public:
	collectDaojuItem();
	~collectDaojuItem();

	static collectDaojuItem* createDaojuColletItem(JuqingDaoJu &info);
	bool initItem(JuqingDaoJu &info);

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

#endif // _collectLayerDaojuScrollView_h_