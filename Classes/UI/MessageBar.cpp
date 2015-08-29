#include "MessageBar.h"
#include "ResManager.h"
#include "font.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

enum TAG
{
	Tag_Bg = 10,
	Tag_MessageBar = 34532,
};

void MessageBar::show(const char* msg)
{
	CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(Tag_MessageBar);
	MessageBar *layer = new MessageBar();
	layer->autorelease();
	layer->init(msg);
	CCDirector::sharedDirector()->getRunningScene()->addChild(layer, zOrder_MESSAGE_LAYER, Tag_MessageBar);
}


bool MessageBar::init(const char* msg)
{
	this->removeAllChildren();
	if (! CCLayerColor::initWithColor(ccc4(0,0,0,100))) return false;

	CCScale9Sprite* bg=CCScale9Sprite::create(ResManager::getManager()->getSharedFilePath("tanchuangkuang_bg.png").c_str());
	this->addChild(bg, 0, Tag_Bg);

	CCLabelTTF *label = CCLabelTTF::create(msg, fontStr_KlavikaBold, 28, CCSize(CCDirector::sharedDirector()->getWinSize().width*0.7-100, 0), kCCTextAlignmentCenter);
	CCSize labelSize = label->getContentSize();
	bg->setContentSize(CCSize(labelSize.width+100, labelSize.height+60));
	bg->addChild(label);
	bg->setPosition(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2);
	CCSize bgSize = bg->getContentSize();
	label->setPosition(ccp(bgSize.width/2, bgSize.height/2));

	CCSprite* title = CCSprite::create(ResManager::getManager()->getSharedFilePath("tanchuangkuang_tishi.png").c_str());
	bg->addChild(title, 1);
	title->setPosition(ccp(0+14, bgSize.height-14));

	return true;
}

void MessageBar::onEnter()
{
	CCLayerColor::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, TOUCH_PRIORITY_MessageLayer, true);
}
void MessageBar::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayerColor::onExit();
}
bool MessageBar::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}
void MessageBar::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLOG("MessageDisplay::ccTouchEnded");
	this->removeFromParent();
	return;

	CCScale9Sprite* pBg = dynamic_cast<CCScale9Sprite*>(this->getChildByTag(Tag_Bg));
	if (pBg)
	{
		CCRect rect = pBg->boundingBox();
		if (rect.containsPoint(pTouch->getLocation()))
		{
			this->removeFromParent();
		}else
		{
			pBg->runAction(CCSequence::create(
				CCScaleTo::create(0.05f, 1.1f),
				CCScaleTo::create(0.05f, 1.0f),
				nullptr));
		}
	}
}