#ifndef __RANKINGLYAER_H__
#define __RANKINGLYAER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "RankingListManager.h"

USING_NS_CC_EXT;

#include "JuqingDaoJuManager.h"

class rankingLayer : 
	public cocos2d::CCLayerColor,
	public cocos2d::extension::CCTableViewDataSource,
	public cocos2d::extension::CCTableViewDelegate
{
public:
	rankingLayer(void);
	~rankingLayer(void);

	virtual bool init();

	virtual void onEnter();
	virtual void onExit();
	virtual void registerWithTouchDispatcher(void);

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	// CCScrollViewDelegate
	virtual void scrollViewDidScroll(CCScrollView* view){}
	virtual void scrollViewDidZoom(CCScrollView* view){}
	// CCTableViewDelegate
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell){};
	// CCTableViewDataSource
	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);

	CREATE_FUNC(rankingLayer);

public:

	enum _iTag
	{
		bg_tag = 1,
		close_btn_tag,
		sanxing_btn_tag,
		chengjiu_btn_tag,
		share_btn_tag,
		sanxiang_tableview_tag,
		chengjiu_tableview_tag,
		owner_tag,
		mingci_tag,
		username_tag,
		numstr_tag,
	};

	enum _TableviewType
	{
		sanxiang_tableview_type = 1,
		chengjiu_tableview_type
	};

public:
	void initMemeber();
	void initUI();
	void initBtn();
	void menuBtnCallBack(cocos2d::CCObject* pObj);
	void share(CCObject* pObj);
	void removeCurLayer(CCNode* pNode);

	void initTableView(_TableviewType tableviewType);
	void scheduleUpTableViewPrioority(CCNode* pNode);
	cocos2d::CCSprite* createCell(int iIndex, RankList_PerfectPassOrEvaluate rankingInfo);
	void covertIntToString(int iNum, std::string& outString);
	void setOwnerInfo(RankList_PerfectPassOrEvaluate info);
	void receiveMsg(cocos2d::CCObject* pMsg);
	void setToggleState();
	void menuToggleCallBack(CCObject* pObj);
private:

	cocos2d::CCSprite*			m_pBg;
	cocos2d::CCMenu*			m_pMenu;

	_TableviewType				m_iTableViewType;

	int							m_iCellCount;

	std::vector<RankList_PerfectPassOrEvaluate>		m_rankSanxingInfoVector;
	std::vector<RankList_PerfectPassOrEvaluate>			m_rankChengjiuInfoVector;

	RankList_PerfectPassOrEvaluate					m_OwnerRankingSanxingInfo;
	RankList_PerfectPassOrEvaluate						m_OwnerRankingChengjiuInfo;
};

#endif //__RANKINGLYAER_H__