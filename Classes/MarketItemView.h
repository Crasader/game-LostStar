#pragma once

#include "JuqingDaoJuManager.h"
#include "MarketLayer.h"
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d::extension;
using namespace cocos2d;


//super
class MarketContaintLayer : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	static MarketContaintLayer *createWithMarketState(marketState state, bool all);
	MarketContaintLayer();
    ~MarketContaintLayer();
public:
	// CCScrollViewDelegate
	virtual void scrollViewDidScroll(CCScrollView* view){}
	virtual void scrollViewDidZoom(CCScrollView* view){}
	// CCTableViewDelegate
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell){}
	virtual void tableCellHighlight(CCTableView* table, CCTableViewCell* cell){}
	virtual void tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell){}
	virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell){}
	// CCTableViewDataSource
	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx){return CCSize(230, 396);}
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table){return 0;}


protected:
	virtual CCSprite * getTableCellContent(int idx) = 0;

public:
	//utiles
	//table view
	void scheduleUpTableViewPrioority(float dt);
	void scheduleRemoveTableTouch(float dt);
	void fixTableViewOffset();
	//buy
	void menuitemClicked_Buy(CCObject *pSender);
	void buyRsp(CCObject *msg);
	//buy success refresh layer
	virtual void refreshlayer_buysuccess(CCObject *msg){}
	//menu
	void callfuncND_ChangeMenuTouchPriority(CCNode* node, void *priority);

protected: 
	CCTableView *mTableView;
	bool mAll;
public:
	void setBgMode(bool all){mAll = all;}
};


//MS_SuperTools
class MarketSuperToolsView : public MarketContaintLayer
{
public:
	MarketSuperToolsView();
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual void refreshlayer_buysuccess(CCObject *msg);
protected:
	virtual CCSprite * getTableCellContent(int idx);
};

//MS_SpecialItem
class MarketSpecialItemView : public MarketContaintLayer
{
public:
	MarketSpecialItemView();
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual void refreshlayer_buysuccess(CCObject *msg);
protected:
	virtual CCSprite * getTableCellContent(int idx);

private:
//	vector<JuqingDaoJu> mItems;
};

//MS_StrategyMap
class MarketStrategyMapView : public MarketContaintLayer
{
public:
	MarketStrategyMapView();
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual void refreshlayer_buysuccess(CCObject *msg);
protected:
	virtual CCSprite * getTableCellContent(int idx);
};
