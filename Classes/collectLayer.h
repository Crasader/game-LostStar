#ifndef __COLLECTLYAER_H__
#define __COLLECTLYAER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "BaseShakeLayer.h"

USING_NS_CC_EXT;

class collectLayer : 
	public BaseShakeLayer
{
public:
	collectLayer(void);
	~collectLayer(void);

	virtual bool init();

	virtual void onEnter();
	virtual void onExit();
	virtual void registerWithTouchDispatcher(void);

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	CREATE_FUNC(collectLayer);

public:

	enum _iTag
	{
		bg_tag = 1,
		close_btn_tag,
		daoju_toggle_tag,			//三个节点的tag必须连续
		chahua_toggle_tag,
		chengjiu_toggle_tag,
		daoju_node_tag,
		chahua_node_tag,
		chengjiu_node_tag,
		daoju_node_tableview_tag,
		chengjiu_scrollview_tag,
		share_menu_tag,
		pageindex1_tag,
		pageindex2_tag,
		pageindex3_tag,
		pageindex4_tag,
		biankuang_tag,
	};

	enum _iCurNodeIndex
	{
		daoju_node_index = 0,
		chahua_node_index,
		chengjiu_node_index,
	};

public:
	void initMemeber();
	void initUI();
	void initMenuToggle();
	void clickToggleCallBack(cocos2d::CCObject* pObj);
	void initBtn();
	void menuBtnCallBack(cocos2d::CCObject* pObj);
	void setToggleState();
	void initDaojuNode();
//	cocos2d::CCSprite* createDaoju(JuqingDaoJu* daojuInfo);
	void initchahuaNode();
	void showSharedBtn(cocos2d::CCNode* pNode);
	void share(CCObject* pObj);
	void chahuaBackDone(CCNode* pNode);
	void initChengjiuNode();
	void setPageIndex(int nodeindex, int pageindex);
	void setPageIndex(int pageindex);
	void removeCurLayer(CCNode* pNode);
	void scheduleUpTableViewPrioority(float dt);
	void setTableViewVisble(CCNode* pNode, bool e);
	void chahuaGoBack();
	void showBigChahua(CCPoint touchPoint);

	inline void teachCollect()
	{
		m_iCurShowNodeIndex = chahua_node_index;
		this->setToggleState();
	}
private:
	cocos2d::CCNode*			m_pDaojuNode;
	cocos2d::CCNode*			m_pChahuaNode;
	cocos2d::CCNode*			m_pChengjiuNode;

	cocos2d::CCSprite*			m_pBg;
	cocos2d::CCMenu*			m_pMenu;
	cocos2d::CCArray*			m_pChahuaArray;
	cocos2d::CCSprite*			m_pCurBigChahua;
	cocos2d::CCPoint			m_ChahuaStartPoint;

	int			m_iCurShowNodeIndex;
	bool		m_bIsShowBigChahua;
	
	int			m_iCurChahuaIndex;
};

#endif //__COLLECTLYAER_H__