#include "IOSStoreLayerScrollView.h"
#include "UserDataManager.h"
#include "JuqingDaoJuManager.h"
#include "font.h"
#include "OpenBoxHelper.h"
#include "LFStrings.h"
#include "IOSStoreLayer.h"
#include "ResManager.h"
#include "GameLayerFactory.h"
USING_NS_CC;

IOSStoreLayerScrollView::IOSStoreLayerScrollView(CCSize viewPortSize,int gridNum,int gridNumInX,int gridNumInY,float gridPadding /* = 10.0f */,CCPoint scrollViewPortOffset /* = CCPointZero */,float pageInval /* = 40.0f */)
	:GridScrollView(viewPortSize,gridNum,gridNumInX,gridNumInY,gridPadding,scrollViewPortOffset,pageInval)
{

}

IOSStoreLayerScrollView::~IOSStoreLayerScrollView(void)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

IOSStoreLayerScrollView* IOSStoreLayerScrollView::createView(CCSize viewPortSize,int gridNum,int gridNumInX,int gridNumInY,float gridPadding /* = 10.0f */,CCPoint scrollViewPortOffset /* = CCPointZero */,float pageInval /* = 40.0f */)
{
	IOSStoreLayerScrollView* pView = new IOSStoreLayerScrollView(viewPortSize, gridNum, gridNumInX, gridNumInY);
	pView->autorelease();

	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(pView);
	pView->runAction(CCSequence::create(CCDelayTime::create(0.01f),
		CCCallFuncN::create(pView, callfuncN_selector(IOSStoreLayerScrollView::scheduleUpTableViewPrioority)), 
		NULL));

	return pView;
}

void IOSStoreLayerScrollView::scheduleUpTableViewPrioority(CCNode* pNode)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate((IOSStoreLayerScrollView*)pNode, kCCMenuHandlerPriority-1, false);
}

ClickableItem* IOSStoreLayerScrollView::createGridItem(int pageIndex,int xIndex,int yIndex)
{
	size_t index = pageIndex*gridNumInX*gridNumInY+xIndex+yIndex*gridNumInX;
	std::vector<BuyItem> buyItemVector;
	if (IOSStoreLayer* pLayer =  dynamic_cast<IOSStoreLayer*>(this->getParent()->getParent()))
	{
		buyItemVector = pLayer->getKaiBaoxiangRsqData();
	}

	IOSStoreItem* pItem = NULL;
	if (index >= 0 && index < buyItemVector.size())
	{
		pItem = IOSStoreItem::createIOSStoreItem(buyItemVector.at(index));
	}

	return pItem;
}

void IOSStoreLayerScrollView::pageSelectedEnd(int pageIndex)
{
	CCLog("pageindex is %d", pageIndex);
}

IOSStoreItem::IOSStoreItem()
{

}

IOSStoreItem::~IOSStoreItem()
{

}

IOSStoreItem* IOSStoreItem::createIOSStoreItem(BuyItem buyiteminfo)
{
	IOSStoreItem* pItem = new IOSStoreItem();
	if (pItem->initItem(buyiteminfo))
	{
		pItem->autorelease();
		pItem->setClickable(false);
//		pItem->setScale(0.9f);
		return pItem;
	}
	CC_SAFE_DELETE(pItem);
	return NULL;
}

bool IOSStoreItem::initItem(BuyItem buyiteminfo)
{
	bool bret = false;

	if (CCSprite::initWithFile(ResManager::getManager()->getSharedFilePath(g_storelayerPath+"baoxiang_kuang.png").c_str()))
	{
		bret = true;
	}

	if (bret)
	{
		//图标
		JuqingDaoJu daojuinfo;
		if (buyiteminfo.item == BuyItem::itemtype_SpecialItem)
		{
			daojuinfo = JuqingDaoJuManager::getManager()->getDaoJuById(buyiteminfo.daojuId);
		}
		else if (buyiteminfo.item == BuyItem::itemtype_SuperTools)
		{
			daojuinfo.daojuName = LFStrings::getValue("SuperTool_Name");
			daojuinfo.fileName = ResManager::getManager()->getSharedFilePath(g_daojuPath+"daoju_gongluetu_chaonengli.png");
		}
		else if (buyiteminfo.item == BuyItem::itemtype_StrategyMap)
		{
			int sectionnum = GameLayerFactory::getInstance()->getAllSectionNumBySeasonId(1);
			int level = buyiteminfo.daojuId/sectionnum;
			int index = buyiteminfo.daojuId%sectionnum;
			if (level == 0 && index == 0)
			{
				level = level+1;
				index = sectionnum;
			}
			else if (index == 0)
			{
				level = level;
				index = sectionnum;
			}
			else
			{
				++level;
			}
			daojuinfo.daojuName = CCString::createWithFormat(LFStrings::getValue("LevelandIndex").c_str(), level, index)->getCString() ;
			daojuinfo.fileName = ResManager::getManager()->getSharedFilePath(g_daojuPath+"daoju_gongluetu.png");
		}
		
		if (buyiteminfo.item == BuyItem::itemtype_SpecialItem)
		{
			daojuinfo.fileName = ResManager::getManager()->getSharedFilePath(g_daojuPath+daojuinfo.fileName);
		}

		CCSprite* pIcon = CCSprite::create(daojuinfo.fileName.c_str());
		if (pIcon)
		{
			this->addChild(pIcon);
			pIcon->setScale(0.6f);
			pIcon->setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));

			//名字
			CCLabelTTF* pNameTTF = CCLabelTTF::create(daojuinfo.daojuName.c_str(), fontStr_katong, 36);
			pIcon->addChild(pNameTTF, 1);
			pNameTTF->setPosition(ccp(pIcon->getContentSize().width/2, -50));
		}
	}

	return bret;
}
