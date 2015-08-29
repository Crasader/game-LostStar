#ifndef __SectionSelectorLayer_H__
#define __SectionSelectorLayer_H__

#include "cocos2d.h"
#include "MaskLayer.h"
#include "GridScrollView.h"
#include "LevelSelectListener.h"
#include "GameInforEditer.h"
#include "ClickableItemCreater.h"

using namespace cocos2d;

#define Section_Item_Num_In_Row 4
#define Section_Item_Num_In_Col 2
#define Section_Item_Scale 1.0f

class SectionSelectorLayer:public GridScrollView,ClickableItemCreater
{
private:
	SectionSelectorLayer(CCSize viewPortSize,int gridNum,int gridNumInX,int gridNumInY,float gridPadding = 10.0f,CCPoint scrollViewPortOffset = CCPointZero,float pageInval = 40.0f);
	//static int lastSectionId;
	int seasonId;
	LevelSelectListener* listener;
	static SectionSelectorLayer* instance;

	CCSprite* container;

	virtual void onTouch(CCTouch *pTouch, CCEvent *pEvent,CCPoint glClickPoint);
	virtual void onTouchEnd(CCTouch *pTouch, CCEvent *pEvent,CCPoint glPoint);
	void tick(float t);

	void gridItemFlatAction();	// 元素平铺效果

	void gridItemPreAppear(CCNode* item);

	void buyGameRsp(CCObject* payRspWrapper);

protected:
	//重写父类的;
	//创建一个Item，根据传入的页面号，x(从左到右的号)，y(从上到下的号)
	virtual ClickableItem* createGridItem(int pageIndex,int xIndex,int yIndex);

	//重写，为了拦截滑动事件，以通知MaskLayer是否显示上一页，下一页的按钮
	virtual void pageSelected(int pageIndex);
	virtual void pageSelectedEnd(int pageIndex){};
public:
	~SectionSelectorLayer();
	static SectionSelectorLayer* createLayer(int seasonId,int lastSeasonId,int lastSectionId,LevelSelectListener* listener);//获得场景
	//static int lastCreatedId(){return lastSectionId;};
	CCNode* getForgeContainer(){return container;};
	static SectionSelectorLayer* getCurrentInstance(){return instance;};
	void nextPage(){intentToScrollToRightPage();};
	void frontPage(){intentToScrollToLeftPage();};
};


//是section选择界面的每一个section的角色
class  SectionItem : public ClickableItem
{
private:
	LevelSelectListener* listener;
	SectionInfor* infor;
	bool canChoose;//是否可以选择;
	int currentScore;//该关卡的得分;
	int sectionId;//关卡的代号：从0开始;
	int seasonId;
	SectionItem(int seasonId,int sectionId);
	//void clickCallBack(SectionSelectorLayer* sectionLayer);//点击回调，也就是在点击效果完毕之后的回调
	void initButton(SectionItem* button,bool canChoose);

	void setScale(float fScale);
	float getScale();
	void buyOnePlayTimeSuc(CCObject* alert);
	void buySectionWithMoney(CCObject* payRspWrapper);
public:
	~SectionItem();
	void setLevelSelectListener(LevelSelectListener* levelSelectListener);
	static SectionItem* sectionWithId(int seasonId,int sectionId,bool canChoose);//创建一个Section按钮
	static SectionItem* sectionWithId(int seasonId,int sectionId);

	//直接由ScrollView回掉
	virtual void clickedMe();
};


#endif