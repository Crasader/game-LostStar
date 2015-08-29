#pragma once

#include "cocos2d.h"
#include "JuqingDaoJuManager.h"
#include "MarketItemDefine.h"
#include "BaseShakeLayer.h"
#include "MaskLayerUI.h"
using namespace cocos2d;



#define MSG_MarketClosed "marketClosed"
#define MSG_BuyHelpMapSuccess "buyhelpmapsuccess"


enum marketState
{
	MS_SuperTools = 1,
	MS_SpecialItem,
	MS_StrategyMap,
};

#define BgLayerTouchPriority -1000  //market layer专用   放在上面的按钮  priority累减
class MarketContaintLayer;
//商店
class MarketLayer : public BaseShakeLayer
{
public:
	MarketLayer(marketState state, bool all=true);
	~MarketLayer();

	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	void listener_refreshLayer_buySuccess(CCObject *msg);

	void showShake();

	inline void beginTeachUseSuperTool()
	{
		if (m_iMenuItem)
		{
			m_bIsUseTeachSuperTool = true;
			CCSize bgSize = mBg->getContentSize();
			CCPoint beginPos = mBg->getPosition();
			CCPoint pos = CCPointMake(beginPos.x - bgSize.width / 2,beginPos.y - bgSize.height / 2);
			CCSize size = CCDirector::sharedDirector()->getWinSize();
			MaskLayerUI *layer = MaskLayerUI::create();
			layer->addCircle(ccp(pos.x + m_iMenuItem->getPositionX() , pos.y + m_iMenuItem->getPositionY()));
			layer->ignoreAnchorPointForPosition(false);
			layer->setAnchorPoint(CCPointZero);
			layer->setPosition(CCPointZero);
			layer->setContentSize(size);
			layer->setTouchPriority(BgLayerTouchPriority);
			addChild(layer,10);
			m_lMaskLayer = layer;
			m_iMenuItem->setVisible(true);
		}
	}
	inline void setTeachCallBack(CCObject *obj,SEL_CallFunc handler)
	{
		m_oTeachTarger = obj;
		m_hTeachCallBackHandler = handler;
	}
protected:
	void menuItemClicked_Close(CCObject *pSender);
	void menuItemClicked_item(CCObject *pSender);
	void useSuperToolClicked(CCObject *obj);

	void refreshAllMenuItem();
	void resetContaintLayer();
private:
	inline void replaceCallBack(CCNode *node)
	{
		node->removeFromParent();
	}
	void setADVisible(bool e, bool bRemove = false);
	void initUI();
private:
	marketState mState;
	bool mAll;

	CCSprite *mBg;
	MarketContaintLayer *mContentLayer;
	CCMenu *mMenu;
	CCSize mWinSize;
	CCMenuItem			*m_iMenuItem;
	bool				m_bIsUseTeachSuperTool;

	CCObject			*m_oTeachTarger;
	SEL_CallFunc		m_hTeachCallBackHandler;
	MaskLayerUI			*m_lMaskLayer;
};
