#ifndef __IOSStoreLayer_h__
#define __IOSStoreLayer_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "BaseShakeLayer.h"

USING_NS_CC_EXT;

#include "JuqingDaoJuManager.h"
#include "OpenBoxHelper.h"

class IOSStoreLayer : 
	public BaseShakeLayer,
	public cocos2d::extension::CCTableViewDataSource,
	public cocos2d::extension::CCTableViewDelegate
{
public:
	IOSStoreLayer(void);
	~IOSStoreLayer(void);

	virtual bool init();

	virtual void onEnter();
	virtual void onExit();
	virtual void registerWithTouchDispatcher(void);

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	// CCScrollViewDelegate
	virtual void scrollViewDidScroll(CCScrollView* view);
	virtual void scrollViewDidZoom(CCScrollView* view){};
	// CCTableViewDelegate
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	// CCTableViewDataSource
	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);

	CREATE_FUNC(IOSStoreLayer);
public:

	enum _iTag
	{
		rmb6_btn_tag = 6,
		rmb12_btn_tag = 12,
		rmb18_btn_tag = 18,
		rmb25_btn_tag = 25,
		rmb30_btn_tag = 30,
		bg_tag = 100,
		close_btn_tag,
		kaibaoxiang_toggle_tag,			//三个节点的tag必须连续
		buystar_toggle_tag,
		kaibaoxiang_node_tag,
		buystar_node_tag,
		kaibaoxiang_node_tableview_tag,
		buystar_node_tableview_tag,
		tip_bg_tag,
		lefttop_tip_str_tag,
		center_tip_str_tag,
		star_count_str_tag,
		onetimes_btn_tag,
		tentimes_btn_tag,
		box_tag,
		guanquan_tag,
		box_big_star_tag,
		box_star_tag,
		getitem_scrollview_tag,
		buy_one_again,
		buy_ten_again,
		ok_btn_tag,
		getitme_node_tag,
		left_arrow_tag,
		right_arrow_tag,
		leftbg_tag,
		leftbg_string_tag,
		downbg_tag,
		downbg_string_tag,
		ad_tip_bg_tag,
		ad_tip_string_tag,
	};

	enum _iCurNodeIndex
	{
		kaibaoxiang_node_index = 0,
		buystar_node_index,
	};

	//同时包含购买星星和宝箱的信息
	typedef struct _CellInfo
	{
		_CellInfo()
		{
			iPrice = 0;
			iBaoxiangCount = 0;
			bBaoxiang = true;
			nameStr = "";
			describeStr = "";
			iconNameStr = "";
			iconBgNameStr = "";
			btnIconNorNameStr = "";
			btnIconSelNameStr = "";
		}

		int		iPrice;				//星星的价格
		int		iBaoxiangCount;		//抽取宝箱个数
		bool	bBaoxiang;			//是否是宝箱
		string nameStr;				//名字
		string describeStr;			//描述
		string iconNameStr;			//中间图标名字
		string iconBgNameStr;		//十个宝箱后面的背景
		string btnIconNorNameStr;	//按钮正常状态图片
		string btnIconSelNameStr;	//按钮选中状态图片
	}CellInfo;

private:
	void initMemeber();
	void initUI();
	void initMenuToggle();
	void clickToggleCallBack(cocos2d::CCObject* pObj);
	void initBtn();
	void menuBtnCallBack(cocos2d::CCObject* pObj);
	void initKaibaoxiangNode();
	CCSprite* createCellSpr(int index, CellInfo* daojuInfo);
	void initBuystarNode();
	void removeCurLayer(CCNode* pNode = NULL);
	void UpTableViewPrioority(CCNode* pNode);
	void setCenterTipStr(const char* str);
	void setLeftTopTipStr(const char* str);
	void setStarCount(int num);
	void setTableViewVisble(CCNode* pNode, bool e);
	void setTableViewVisble(CCNode* pNode);
	void showBoxAniLayer();
	void showOpenBoxAni(CCNode* pNode);
	void showRandStars(CCNode* pParent);
	CCPointArray* randStarPoint(int iCount);
	float randScale();
	float randRotation();
	int randStarCount();
	void boxStartMove(CCNode* pBox);
	void showStarsFadeEffect(CCNode* pBox);
	void showItemsNode();
	void payEnd(CCObject* pObj);
	void setKaiBaoxiangBtnVisiable();
	void openBoxRsp(CCObject* pObj);
	void backMainNode();

public:
	void setToggleStateByNodeIndex(int iNodeIndex);
	void setComefrom(OpenBoxComeFrom openboxComefrom);
	OpenBoxComeFrom getComefrom(){return m_enumComeFrom;};
	std::vector<BuyItem> getKaiBaoxiangRsqData(){return m_kaiBaoxiangRspVector;};
	void setTipBgVisible(bool e);
	void setLeftBgString(const char* charString);
	void setDownBgVisible(bool e);
	void setLeftTopTipStrVisible(bool e);
	void setADVisible(bool e, bool bRemove = false);
private:
	CCScale9Sprite*			m_pKaibaoxiangNode;
	CCScale9Sprite*			m_pBuyStarNode;

	cocos2d::CCSprite*			m_pBg;
	cocos2d::CCMenu*			m_pBgMenu;

	cocos2d::CCNode*			m_pGetItemNode;
	
	int			m_iBaoxiangCellCount;
	int			m_iCurShowNodeIndex;
	int			m_iBuyStarCellCount;

	bool		m_bBaoxiangBuyTenTimes;
	OpenBoxComeFrom				m_enumComeFrom;

	std::vector<CellInfo>		m_baoxiangInfoVector;
	std::vector<CellInfo>		m_buystarInfoVector;
	
	std::vector<BuyItem>		m_kaiBaoxiangRspVector;

};

#endif //__IOSStoreLayer_h__