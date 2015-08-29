#include "collectLayerScrollView.h"
#include "UserDataManager.h"
#include "collectLayer.h"
#include "ResManager.h"
USING_NS_CC;

collectLayerScrollView::collectLayerScrollView(CCSize viewPortSize,int gridNum,int gridNumInX,int gridNumInY,float gridPadding /* = 10.0f */,CCPoint scrollViewPortOffset /* = CCPointZero */,float pageInval /* = 40.0f */)
	:GridScrollView(viewPortSize,gridNum,gridNumInX,gridNumInY,gridPadding,scrollViewPortOffset,pageInval)
{

}

collectLayerScrollView::~collectLayerScrollView(void)
{
//	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

collectLayerScrollView* collectLayerScrollView::createView(CCSize viewPortSize,int gridNum,int gridNumInX,int gridNumInY,float gridPadding /* = 10.0f */,CCPoint scrollViewPortOffset /* = CCPointZero */,float pageInval /* = 40.0f */)
{
	collectLayerScrollView* pView = new collectLayerScrollView(viewPortSize, gridNum, gridNumInX, gridNumInY);
	pView->autorelease();

	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(pView);
	pView->runAction(CCSequence::create(CCDelayTime::create(0.1f),
		CCCallFuncN::create(pView, callfuncN_selector(collectLayerScrollView::scheduleUpTableViewPrioority)), 
		NULL));

	return pView;
}

void collectLayerScrollView::scheduleUpTableViewPrioority(CCNode* pNode)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate((collectLayerScrollView*)pNode, kCCMenuHandlerPriority-1, false);
}

ClickableItem* collectLayerScrollView::createGridItem(int pageIndex,int xIndex,int yIndex)
{
	size_t index = pageIndex*gridNumInX*gridNumInY+xIndex+yIndex*gridNumInX;
	std::vector<std::string> achievementFilenameVector = UserDataManager::getInstance()->getAchievements();
	collectItem* pItem = NULL;
	if (index >= 0 && index < achievementFilenameVector.size())
	{
		pItem = collectItem::createColletItem(achievementFilenameVector.at(index));
	}
	else
	{
		pItem = collectItem::createColletItem("");
	}

	return pItem;
}

void collectLayerScrollView::pageSelectedEnd(int pageIndex)
{
	CCLog("pageindex is %d", pageIndex);
	collectLayer* pParent = dynamic_cast<collectLayer*>(this->getParent()->getParent()->getParent());
	if (pParent)
	{
		pParent->setPageIndex(pageIndex);
	}
}

collectItem::collectItem()
{

}

collectItem::~collectItem()
{

}

collectItem* collectItem::createColletItem(std::string titleFilename)
{
	collectItem* pItem = new collectItem();
	if (pItem->initItem(titleFilename))
	{
		pItem->autorelease();
		pItem->setClickable(false);
		return pItem;
	}
	CC_SAFE_DELETE(pItem);
	return NULL;
}

bool collectItem::initItem(std::string titleFilename)
{
	bool bret = false;
	//未获得成就
	if (titleFilename == "")
	{
		if (CCSprite::initWithFile(ResManager::getManager()->getSharedFilePath(g_collectlayerPath+"main_shoucang_chengjiu_weihuode.png").c_str()))
		{
			bret = true;
		}
	}
	else
	{
		if (CCSprite::initWithFile(ResManager::getManager()->getSharedFilePath(g_collectlayerPath+"main_shoucang_chengjiu.png").c_str()))
		{
			bret = true;
		}
	}

	if (bret)
	{
		CCSprite* pTitle = CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_achievPath+titleFilename).c_str());
//		if (pTitle)
		if (titleFilename != "")
		{
			this->addChild(pTitle);
			pTitle->setPosition(ccp(this->getContentSize().width/2, -pTitle->getContentSize().height/2));
		}
	}

	return bret;
}
