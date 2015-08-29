#include "collectLayerDaojuScrollView.h"
#include "UserDataManager.h"
#include "collectLayer.h"
#include "font.h"
#include "GameInforEditer.h"
#include "effect/BaseLightEffectNode.h"

#include "ResManager.h"

USING_NS_CC;

collectLayerDaojuScrollView::collectLayerDaojuScrollView(CCSize viewPortSize,int gridNum,int gridNumInX,int gridNumInY,float gridPadding /* = 10.0f */,CCPoint scrollViewPortOffset /* = CCPointZero */,float pageInval /* = 40.0f */)
	:GridScrollView(viewPortSize,gridNum,gridNumInX,gridNumInY,gridPadding,scrollViewPortOffset,pageInval)
{

}

collectLayerDaojuScrollView::~collectLayerDaojuScrollView(void)
{
//	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

collectLayerDaojuScrollView* collectLayerDaojuScrollView::createView(CCSize viewPortSize,int gridNum,int gridNumInX,int gridNumInY,float gridPadding /* = 10.0f */,CCPoint scrollViewPortOffset /* = CCPointZero */,float pageInval /* = 40.0f */)
{
	collectLayerDaojuScrollView* pView = new collectLayerDaojuScrollView(viewPortSize, gridNum, gridNumInX, gridNumInY);
	pView->autorelease();
	//获取要显示的所有道具的个数
	pView->m_vDaojuInfoList.clear();
	pView->m_vDaojuInfoList = JuqingDaoJuManager::getManager()->getAllGettedItems();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(pView);
	pView->runAction(CCSequence::create(CCDelayTime::create(0.1f),
		CCCallFuncN::create(pView, callfuncN_selector(collectLayerDaojuScrollView::scheduleUpTableViewPrioority)), 
		NULL));

	return pView;
}

void collectLayerDaojuScrollView::scheduleUpTableViewPrioority(CCNode* pNode)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate((collectLayerDaojuScrollView*)pNode, kCCMenuHandlerPriority-1, false);
}

ClickableItem* collectLayerDaojuScrollView::createGridItem(int pageIndex,int xIndex,int yIndex)
{
	size_t index = pageIndex*gridNumInX*gridNumInY+xIndex+yIndex*gridNumInX;
	collectDaojuItem* pItem = NULL;
	JuqingDaoJu info;
	info.fileName = ResManager::getManager()->getSharedFilePath(g_collectlayerPath+"main_shoucang_daoju_weihuode.png");
	info.describe = "";
	info.daojuName = "";
	info.state =  DJS_NotGetted;

	if (index >= 0 && index < m_vDaojuInfoList.size())
	{
		pItem = collectDaojuItem::createDaojuColletItem(m_vDaojuInfoList.at(index));
	}
	else
	{
		pItem = collectDaojuItem::createDaojuColletItem(info);
	}
	return pItem;
}

void collectLayerDaojuScrollView::pageSelectedEnd(int pageIndex)
{
	CCLog("pageindex is %d", pageIndex);
	collectLayer* pParent = dynamic_cast<collectLayer*>(this->getParent()->getParent()->getParent());
	if (pParent)
	{
		pParent->setPageIndex(pageIndex);
	}
}

collectDaojuItem::collectDaojuItem()
{

}

collectDaojuItem::~collectDaojuItem()
{

}

collectDaojuItem* collectDaojuItem::createDaojuColletItem(JuqingDaoJu &info)
{
	collectDaojuItem* pItem = new collectDaojuItem();
	if (pItem->initItem(info))
	{
		pItem->autorelease();
		pItem->setClickable(false);
		return pItem;
	}
	CC_SAFE_DELETE(pItem);
	return NULL;
}

bool collectDaojuItem::initItem(JuqingDaoJu &info)
{
	bool bret = false;
	
	if (info.state == DJS_NotGetted)
	{
		if (CCSprite::initWithFile(ResManager::getManager()->getSharedFilePath(g_storelayerPath+"main_shangdian_daoju_bg_lock.png").c_str()))
		{
			bret = true;
		}
	}
	else
	{
		if (CCSprite::initWithFile(ResManager::getManager()->getSharedFilePath(g_storelayerPath+"main_shangdian_daoju_bg.png").c_str()))
		{
			bret = true;

			BaseLightEffectNode *s = BaseLightEffectNode::create();
			this->addChild(s);
		}
	}

	if (!bret)
	{
		return bret;
	}

	//显示道具的名字
	CCLabelTTF* pTitle = CCLabelTTF::create(info.daojuName.c_str(), fontStr_katong, 24);
	this->addChild(pTitle);
	pTitle->setColor(fontColor_Store);
	pTitle->setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height - 40));

	//显示道具的描述
	CCLabelTTF* pDescribe = CCLabelTTF::create(info.describe.c_str(), fontStr_katong, 24, CCSizeMake(180, 60), kCCTextAlignmentLeft);
	this->addChild(pDescribe);
	pDescribe->setAnchorPoint(ccp(0, 0.5f));
	pDescribe->setHorizontalAlignment(kCCTextAlignmentLeft);
	pDescribe->setPosition(ccp(10, 85));

	//获取关卡名字
	std::string seasonName = "";
	if (info.state != DJS_NotGetted)
	{
		int seasonId = 0;
		if (info.daojuId < 5) seasonId = 1;
		else if (info.daojuId < 9) seasonId = 2;
		else if (info.daojuId < 13) seasonId = 3;
		else if (info.daojuId < 17) seasonId = 4;
		else if (info.daojuId < 21) seasonId = 5;

		seasonName = GameInforEditer::getGameInfor()->getSeasonName(seasonId);
	}

	//显示道具的关卡名字
	CCLabelTTF* pLevelName = CCLabelTTF::create(seasonName.c_str(), fontStr_katong, 24, CCSizeMake(180, 60), kCCTextAlignmentLeft);
	this->addChild(pLevelName);
	pLevelName->setHorizontalAlignment(kCCTextAlignmentCenter);
	pLevelName->setPosition(ccp(this->getContentSize().width/2, 20));

	string path = ResManager::getManager()->getSharedFilePath(g_daojuPath+info.fileName);
	if (info.state == DJS_NotGetted)
	{
		path = info.fileName;
	}

	//显示图标
	CCSprite* pIcon = CCSprite::create(path.c_str());
//	if (pIcon)
	{
		pIcon->setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2+20));
		this->addChild(pIcon);
		pIcon->setScale(0.8f);

		CCSprite *mask = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_storelayerPath+"main_shangdian_daoju_fugai.png").c_str());
		this->addChild(mask, 2);
		mask->setPosition(ccp(pIcon->getPositionX(), pIcon->getPositionY()+20));
	}

	return bret;
}
