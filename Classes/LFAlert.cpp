﻿#include "LFAlert.h"
#include "LFUtile.h"
#include "ResManager.h"

using namespace LF;

LFAlert::LFAlert(bool modal /* = true */,int priority /* = Default_Priority */)
{
	init();
	this->priority = priority;

	{
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCSprite* bg = CCSprite::create();
		bg->initWithFile(ResManager::getManager()->getSharedFilePath(g_mainlayerPath+"gray16.png").c_str());
		addChild(bg,-1);
		bg->setTextureRect(CCRectMake(0,0,visibleSize.width,visibleSize.height));
		bg->setPosition(ccp(bg->getContentSize().width/2,bg->getContentSize().height/2));
	}


	mModal = modal;
	mBaseNode = NULL;
	mAutoDismiss = false;
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,priority,true);
}

void LFAlert::setAutoDismiss(const char* dismissMsg,bool autoDismiss /* = true */)
{
	mAutoDismiss = autoDismiss;
	mMessage = string(dismissMsg);
}

void LFAlert::setMenuItemEnable(bool enable)
{
	mMenu->setEnabled(enable);
}

LFAlert* LFAlert::setBaseContainer(CCNode* base,CCMenu* menu,bool useDefaultPos /* = true */)
{
	// 内部会自动添加到图层，所有外部不需要addChild
	CCAssert(base->getParent() == NULL,"");
	CCAssert(menu->getParent() == NULL,"");

	mBaseNode = base;
	mMenu = menu;

	if (useDefaultPos)
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		mBaseNode->setPosition(ccp(winSize.width/2,winSize.height/2));
	}
	addChild(mBaseNode);

	mBaseNode->addChild(mMenu);
	mMenu->setPosition(CCPointZero);

	scheduleOnce(schedule_selector(LFAlert::lazeyChangeMenuPriority),0);

	return this;
}

void LFAlert::show(int z_order /* = 1024 */)
{
	CCDirector::sharedDirector()->getRunningScene()->addChild(this,z_order);
}


void LFAlert::lazeyChangeMenuPriority(float t)
{
	mMenu->setHandlerPriority(priority);
}
void LFAlert::onEnter()
{
	CCLayer::onEnter();
}
bool LFAlert::canTouch(CCPoint glPoint)
{
	if (mBaseNode && mMenu)
	{
		CCArray* menuChildren = mMenu->getChildren();
		int childNum = mMenu->getChildrenCount();

		if (menuChildren && childNum > 0)
		{
			CCObject* pObject = NULL;
			CCARRAY_FOREACH(menuChildren, pObject)
			{
				CCNode* pChild = dynamic_cast<CCNode*>(pObject);
				if (pChild && pChild->isVisible() && ((CCMenuItem*)pChild)->isEnabled())
				{
					CCPoint local = pChild->convertToNodeSpace(glPoint);
					CCRect r = ((CCMenuItem*)pChild)->rect();
					r.origin = CCPointZero;

					//if (CCRect::CCRectContainsPoint(r, local))
					if (r.containsPoint(local))
					{
						return true;
					}
				}
			}
		}

		return false;

		//return CCRect::CCRectContainsPoint(mBaseNode->boundingBox(),glPoint);
	}
	return false;

}

// 返回为false的时候，TouchDispatcher将会相应下一个优先级的触摸代理。 
// CCLayer中的setTouchEnabled，如果设置为true，就会以0为优先级来注册触摸监听代理，
// 优先级的值越小，优先级越高，触摸响应是按照优先级来排列响应的。 

bool LFAlert::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint pos = lfConvertToLocalPos(pTouch);

	// 如果有添加显示的
	if (mBaseNode)
	{
		if (!canTouch(pos))
		{
			if (mAutoDismiss && !mBaseNode->boundingBox().containsPoint(pos))
			{
				CCNotificationCenter::sharedNotificationCenter()->postNotification(mMessage.c_str(),this);
				CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
				removeFromParent();
				return mModal;
			}

			mBaseNode->stopAllActions();
			mBaseNode->runAction(CCSequence::create(
				CCScaleTo::create(0.1f,1.1f),
				CCScaleTo::create(0.1f,0.9f),
				CCScaleTo::create(0.05f,1.0f),
				NULL
				));
		}else
		{
			return false;
		}
	}
	return mModal;
}


void LFAlert::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}