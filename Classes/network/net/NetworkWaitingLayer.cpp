#include "NetworkWaitingLayer.h"
#include "ResManager.h"


NetworkWaitingLayer* NetworkWaitingLayer::create(bool bShowWaitingUI)
{
	NetworkWaitingLayer* layer = new NetworkWaitingLayer;
	if (layer && layer->init(bShowWaitingUI))
	{
		layer->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(layer);
		layer = nullptr;
	}
	return layer;
}
bool NetworkWaitingLayer::init(bool bShowWaitingUI)
{
	m_bSwallowTouch = true;

	if (! CCLayerColor::initWithColor(ccc4(0,0,0,80))) return false;

	if (bShowWaitingUI)
	{
		CCSprite* sbg = CCSprite::create(ResManager::getManager()->getSharedFilePath("UI_zairu_bg.png").c_str());
		this->addChild(sbg);
		sbg->setPosition(CCDirector::sharedDirector()->getWinSize()/2);

		CCSprite* s = CCSprite::create(ResManager::getManager()->getSharedFilePath("UI_zairu_zhuandong.png").c_str());
		this->addChild(s);
		s->setPosition(sbg->getPosition());
		s->runAction(CCRepeatForever::create(CCRotateBy::create(1, 360)));
	}
	this->setTag(TAG_WaitingLayer);
	this->setZOrder(zOrder_WaitingLayer);
	return true;
}

void NetworkWaitingLayer::setSwallowTouch(bool bSwallow)
{
	m_bSwallowTouch = bSwallow;
}

void NetworkWaitingLayer::onEnter()
{
	CCLayerColor::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, TOUCH_PRIORITY_WaitingLayer, true);
}
void NetworkWaitingLayer::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayerColor::onExit();
}

bool NetworkWaitingLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return m_bSwallowTouch;
}

void NetworkWaitingLayer::ccTouchMoved(CCTouch *touch, CCEvent *unused_event)
{
}
void NetworkWaitingLayer::ccTouchEnded(CCTouch *touch, CCEvent *unused_event)
{

}

void NetworkWaitingLayer::showWaiting(bool showWaitingSprite)
{
	NetworkWaitingLayer* pLayer = dynamic_cast<NetworkWaitingLayer*>(CCDirector::sharedDirector()->getRunningScene()->getChildByTag(TAG_WaitingLayer));
	if (pLayer){
		pLayer->setVisible(true);
		pLayer->setSwallowTouch(true);
	}else {
		CCDirector::sharedDirector()->getRunningScene()->
			addChild(NetworkWaitingLayer::create(showWaitingSprite), zOrder_WaitingLayer, TAG_WaitingLayer);
	}
}
void NetworkWaitingLayer::hideWaiting()
{
	NetworkWaitingLayer* pLayer = dynamic_cast<NetworkWaitingLayer*>(CCDirector::sharedDirector()->getRunningScene()->getChildByTag(TAG_WaitingLayer));
	if (pLayer) {
		pLayer->setVisible(false);
		pLayer->setSwallowTouch(false);
	}
}