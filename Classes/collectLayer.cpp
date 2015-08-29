#include "collectLayer.h"

#include "UserDataManager.h"
#include "font.h"
#include "collectLayerScrollView.h"
#include "collectLayerDaojuScrollView.h"
#include "GameInforEditer.h"
#include "LFStrings.h"
#include "VersionControl.h"
#include "ResManager.h"
#include "AchievementManager.h"


USING_NS_CC;

collectLayer::collectLayer(void)
{
	m_pChahuaNode = NULL;
	m_pDaojuNode = NULL;
	m_pChengjiuNode = NULL;
	m_pChahuaArray = NULL;

}

collectLayer::~collectLayer(void)
{
}

bool collectLayer::init()
{
	if (!BaseShakeLayer::init())
	{
		return false;
	}

	this->initMemeber();
	this->initUI();
	this->initMenuToggle();
	this->initBtn();

	//屏蔽剧情道具相关
//	this->initDaojuNode();
	this->initchahuaNode();
	this->initChengjiuNode();


	this->setToggleState();

	return true;
}

void collectLayer::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

void collectLayer::onEnter()
{
	this->setTouchEnabled(true);
	this->setTouchMode(kCCTouchesOneByOne);

	BaseShakeLayer::onEnter();
}

void collectLayer::onExit()
{
	BaseShakeLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCTableView* pView = (CCTableView*)m_pDaojuNode->getChildByTag(daoju_node_tableview_tag);
	if (pView)
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(pView);
	}
}

void collectLayer::initMemeber()
{
	CC_SAFE_RELEASE_NULL(m_pChahuaArray);
	m_pChahuaArray = CCArray::create();
	m_pChahuaArray->retain();

	m_iCurChahuaIndex = 0;
	m_iCurShowNodeIndex = 0;
	m_bIsShowBigChahua = false;
	m_pCurBigChahua = NULL;
}

void collectLayer::initUI()
{
	//初始化背景
	m_pBg = CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_collectlayerPath+"main_shoucang_bg.png").c_str());
	this->addChild(m_pBg, 1, bg_tag);
	m_pBg->setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));

	m_pDaojuNode = CCNode::create();
	m_pChahuaNode = CCNode::create();
	m_pChengjiuNode = CCNode::create();

	m_pBg->addChild(m_pDaojuNode, 3, daoju_node_tag);
	m_pBg->addChild(m_pChahuaNode, 3, chahua_node_tag);
	m_pBg->addChild(m_pChengjiuNode, 3, chengjiu_node_tag);

	m_pMenu = CCMenu::create();
	m_pMenu->setPosition(CCPointZero);
	m_pBg->addChild(m_pMenu, 2);
}

void collectLayer::initMenuToggle()
{
	//道具按钮开关
	CCMenuItemSprite* pNorItem = CCMenuItemSprite::create(CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_collectlayerPath+"main_shangdian_anniu_teshuwupin.png").c_str()),
		CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_collectlayerPath+"main_shangdian_anniu_teshuwupin.png").c_str()));
	CCMenuItemSprite* pPreItem = CCMenuItemSprite::create(CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_collectlayerPath+"main_shangdian_anniu_teshuwupin_select.png").c_str()), 
		CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_collectlayerPath+"main_shangdian_anniu_teshuwupin_select.png").c_str()));

	CCMenuItemToggle* pToggle = CCMenuItemToggle::createWithTarget(this, 
		menu_selector(collectLayer::clickToggleCallBack), 
		pNorItem,
		pPreItem, NULL);
	pToggle->setTag(daoju_toggle_tag);
	pToggle->setPosition(ccp(m_pBg->getContentSize().width/2 - pToggle->getContentSize().width, 
		this->getContentSize().height/2 + pToggle->getContentSize().width/2-15));
	pToggle->setSelectedIndex(1);
	m_pMenu->addChild(pToggle);

	//插画按钮开关
	pNorItem = CCMenuItemSprite::create(CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_collectlayerPath+"main_shoucang_anniu_chahuaji.png").c_str()),
		CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_collectlayerPath+"main_shoucang_anniu_chahuaji.png").c_str()));
	pPreItem = CCMenuItemSprite::create(CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_collectlayerPath+"main_shoucang_anniu_chahuaji_select.png").c_str()),
		CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_collectlayerPath+"main_shoucang_anniu_chahuaji_select.png").c_str()));

	pToggle = CCMenuItemToggle::createWithTarget(this, 
		menu_selector(collectLayer::clickToggleCallBack), 
		pNorItem,
		pPreItem, NULL);
	pToggle->setTag(chahua_toggle_tag);
	pToggle->setPosition(ccp(m_pBg->getContentSize().width/2, 
		this->getContentSize().height/2 + pToggle->getContentSize().width/2-15));
	m_pMenu->addChild(pToggle);

	//成就按钮开关
	pNorItem = CCMenuItemSprite::create(CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_collectlayerPath+"main_shoucang_anniu_cehngjiu.png").c_str()),
		CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_collectlayerPath+"main_shoucang_anniu_cehngjiu.png").c_str()));
	pPreItem = CCMenuItemSprite::create(CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_collectlayerPath+"main_shoucang_anniu_cehngjiu_select.png").c_str()), 
		CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_collectlayerPath+"main_shoucang_anniu_cehngjiu_select.png").c_str()));

	pToggle = CCMenuItemToggle::createWithTarget(this, 
		menu_selector(collectLayer::clickToggleCallBack), 
		pNorItem,
		pPreItem, NULL);
	pToggle->setTag(chengjiu_toggle_tag);
	pToggle->setPosition(ccp(m_pBg->getContentSize().width/2 + pToggle->getContentSize().width, 
		this->getContentSize().height/2 + pToggle->getContentSize().width/2-15));
	m_pMenu->addChild(pToggle);

}

void collectLayer::initBtn()
{
	CCMenuItemImage* pClose = CCMenuItemImage::create(ResManager::getManager()->getSharedFilePath(g_buttonPath+"guanbi.png").c_str(), 
		ResManager::getManager()->getSharedFilePath(g_buttonPath+"guanbi_select.png").c_str(), 
		this,
		menu_selector(collectLayer::menuBtnCallBack));
	pClose->setTag(close_btn_tag);
	m_pMenu->addChild(pClose);

	CCNode* pBg = this->getChildByTag(bg_tag);
	if (pBg)
	{
		pClose->setPosition(ccp(pBg->getContentSize().width - pClose->getContentSize().width/2 + 20, 
			pBg->getContentSize().height - pClose->getContentSize().height/2 - 40));
	}
}

void collectLayer::initDaojuNode()
{
	int iDaojuCount = JuqingDaoJuManager::getManager()->getAllItems().size();
	collectLayerDaojuScrollView* pView = collectLayerDaojuScrollView::createView(CCSizeMake(920, 400), iDaojuCount, 4, 1);
	m_pDaojuNode->addChild(pView);
	pView->setPosition(ccp(35, 40));
	pView->setTouchPriority(kCCMenuHandlerPriority);
	pView->setClickableCreater(pView);
	pView->prepareToDisplay();
	pView->setTag(daoju_node_tableview_tag);

	//添加滑动点
	int count = iDaojuCount/4;
	if (iDaojuCount%4 != 0)
	{
		count++; 
	}
	for (int i = 0; i < count; ++i)
	{
		CCSprite* pPageNode = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"season_point.png").c_str());
		m_pDaojuNode->addChild(pPageNode);
		pPageNode->setPosition(ccp(m_pBg->getContentSize().width/2+(i-1)*pPageNode->getContentSize().width, pPageNode->getContentSize().height+20));
		pPageNode->setTag(pageindex1_tag+i);
	}

	this->setPageIndex(daoju_node_index, 0);
}

void collectLayer::scheduleUpTableViewPrioority(float dt)
{
	CCTableView* pView = (CCTableView*)m_pDaojuNode->getChildByTag(daoju_node_tableview_tag);
	if (pView)
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(pView, kCCMenuHandlerPriority-1, false);
	}
}

void collectLayer::initchahuaNode()
{
	CCPointArray* pPositions = CCPointArray::create(6);
	pPositions->addControlPoint(ccp(165, 330));
	pPositions->addControlPoint(ccp(485, 330));
	pPositions->addControlPoint(ccp(805, 330));
	pPositions->addControlPoint(ccp(325, 130));
	pPositions->addControlPoint(ccp(645, 130));

	//添加相框
	for (size_t i = 0; i < pPositions->count(); ++i)
	{
		CCSprite* pXiangkuang = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_collectlayerPath+"main_shoucang_xiangkuang.png").c_str());
		m_pChahuaNode->addChild(pXiangkuang);
		pXiangkuang->setPosition(pPositions->getControlPointAtIndex(i));
	}
	
	//初始化图片
	std::vector<IllustrationInfo> chahuaInfoVector = UserDataManager::getInstance()->getIllustration();
	IllustrationInfo info;
	for (size_t i = 0; i < chahuaInfoVector.size(); i++)
	{
		info = chahuaInfoVector.at(i);
		CCSprite* pChahua = CCSprite::create(info.fileName.c_str());
		pChahua->setPosition(pPositions->getControlPointAtIndex(i));
		pChahua->setScale(0.21f);
		m_pChahuaNode->addChild(pChahua);
		m_pChahuaArray->addObject(pChahua);
	}
}

void collectLayer::initChengjiuNode()
{
	int iTotalCount = AchievementManager::getInstance()->getDefaultAchievementList().size();
	collectLayerScrollView* pView = collectLayerScrollView::createView(CCSizeMake(920, 400), iTotalCount, 4, 2);
	m_pChengjiuNode->addChild(pView);
	pView->setPosition(ccp(35, 50));
	pView->setTouchPriority(kCCMenuHandlerPriority);
	pView->setClickableCreater(pView);
	pView->prepareToDisplay();
	pView->setTag(chengjiu_scrollview_tag);

	//添加滑动点
	for (int i = 0; i < iTotalCount/8+1; ++i)
	{
		CCSprite* pPageNode = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"season_point.png").c_str());
		m_pChengjiuNode->addChild(pPageNode);
		pPageNode->setPosition(ccp(m_pBg->getContentSize().width/2+(i-1)*pPageNode->getContentSize().width, pPageNode->getContentSize().height+20));
		pPageNode->setTag(pageindex1_tag+i);
	}

	this->setPageIndex(chengjiu_node_index, 0);
}

void collectLayer::setPageIndex(int pageindex)
{
	this->setPageIndex(m_iCurShowNodeIndex, pageindex);
}

void collectLayer::setPageIndex(int nodeindex, int pageindex)
{
	CCNode* pNode = NULL;
	if (nodeindex == chengjiu_node_index)
	{
		pNode = m_pChengjiuNode;
	}
	else if (nodeindex == daoju_node_index)
	{
		pNode = m_pDaojuNode;
	}

	if (!pNode)
	{
		return;
	}

	CCSprite* pSpr = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"season_point.png").c_str());
	CCSpriteFrame* pFrame1 = CCSpriteFrame::createWithTexture(pSpr->getTexture(), pSpr->getTextureRect());
	
	pSpr = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"season_point_select.png").c_str());
	CCSpriteFrame* pFrame2 = CCSpriteFrame::createWithTexture(pSpr->getTexture(), pSpr->getTextureRect());
	CCSprite* pTemp = NULL;
	for (int i = 0; i < 8; ++i)
	{
		pTemp = (CCSprite*)pNode->getChildByTag(pageindex1_tag+i);
		if (!pTemp)
		{
			continue;
		}
		if (pageindex == i)
		{
			pTemp->setDisplayFrame(pFrame2);
		}
		else
		{
			pTemp->setDisplayFrame(pFrame1);
		}
	}
}

void collectLayer::menuBtnCallBack(CCObject* pObj)
{
	CCLog("menubtn call back");
	CCMenuItemImage* pItme = (CCMenuItemImage*)pObj;
	int iTag = pItme->getTag();
	switch (iTag)
	{
	case close_btn_tag:
		{
			m_pBg->runAction(CCSequence::create(CCScaleTo::create(0.2f, 0.0f), CCCallFuncN::create(this, callfuncN_selector(collectLayer::removeCurLayer)), NULL));
		}
		break;

	default:
		break;
	}
}

void collectLayer::removeCurLayer(CCNode* pNode)
{
	this->removeFromParent();
}

void collectLayer::clickToggleCallBack(CCObject* pObj)
{
	CCLog("menutoggle call back");
	CCMenuItemToggle* pToggle = (CCMenuItemToggle*)pObj;
	int iTag = pToggle->getTag();
	switch (iTag)
	{
	case daoju_toggle_tag:
		{
			m_iCurShowNodeIndex = daoju_node_index;
		}
		break;

	case chahua_toggle_tag:
		{
			m_iCurShowNodeIndex = chahua_node_index;
		}
		break;

	case chengjiu_toggle_tag:
		{
			m_iCurShowNodeIndex = chengjiu_node_index;
		}
		break;

	default:
		break;
	}

	this->setToggleState();
}

void collectLayer::setToggleState()
{
	CCMenuItemToggle* pToggle = NULL;
	CCNode* pNode = NULL;
	for (int i = 0; i < 3; ++i)
	{
		pNode = m_pBg->getChildByTag(daoju_node_tag+i);
		pToggle = (CCMenuItemToggle*)m_pMenu->getChildByTag(daoju_toggle_tag+i);
		if (m_iCurShowNodeIndex == i)
		{
			if (pNode)
			{
				pNode->setVisible(true);
				this->setTableViewVisble(pNode, true);
				pToggle->setSelectedIndex(1);
			}
		}
		else
		{
			if (pNode)
			{
				pNode->setVisible(false);
				this->setTableViewVisble(pNode, false);
				pToggle->setSelectedIndex(0);
			}
		}
	}
}

void collectLayer::setTableViewVisble(CCNode* pNode, bool e)
{
	int iTag = pNode->getTag();
	if (iTag == daoju_node_tag)
	{
		if (CCNode* p = pNode->getChildByTag(daoju_node_tableview_tag))
		{
			p->setVisible(e);
		}
	}
	else if (iTag == chengjiu_node_tag)
	{
		if (CCNode* p = pNode->getChildByTag(chengjiu_scrollview_tag))
		{
			p->setVisible(e);
		}
	}
}

bool collectLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint touchPoint = pTouch->getLocation();

	if (m_iCurShowNodeIndex == chahua_node_index)
	{
		//大插画显示
		if ( m_bIsShowBigChahua)
		{
			m_iCurChahuaIndex = 0;
			if (m_pCurBigChahua && !m_pCurBigChahua->boundingBox().containsPoint(m_pBg->convertToNodeSpace(touchPoint)))
			{
//				CCNode* pNode = m_pCurBigChahua->getChildByTag(biankuang_tag);
//				if (pNode)
				{
					CCMenu* psharedMenu = (CCMenu*)m_pCurBigChahua->getChildByTag(share_menu_tag);
					if (psharedMenu)
					{
						this->chahuaGoBack();
					}
				}	
			}
		}
		//大插画没显示
		else
		{
			this->showBigChahua(touchPoint);
		}
	}
	return true;
}

void collectLayer::showBigChahua(CCPoint touchPoint)
{
	CCSprite* pChahua = NULL;
	for (size_t i = 0; i < m_pChahuaArray->count(); ++i)
	{
		pChahua = (CCSprite*)m_pChahuaArray->objectAtIndex(i);
		if (pChahua->boundingBox().containsPoint(m_pBg->convertToNodeSpace(touchPoint)))
		{
			m_iCurChahuaIndex = i;
			//加上白色边框
// 			CCSprite* pBiankaung = CCSprite::create("chatu_biankuang.png");
// 			pBiankaung->setTag(biankuang_tag);
// 			pChahua->addChild(pBiankaung, 1);
// 			pBiankaung->setPosition(ccp(pChahua->getContentSize().width/2, pChahua->getContentSize().height/2));

			pChahua->setZOrder(m_pChahuaNode->getChildrenCount());
			m_ChahuaStartPoint = pChahua->getPosition();
			m_pCurBigChahua = pChahua;
			//显示插画时,禁用其他的按钮菜单
			m_pMenu->setEnabled(false);
			pChahua->runAction(CCSequence::create(
				CCSpawn::create(
				CCMoveTo::create(0.2f, ccp(m_pBg->getContentSize().width/2, m_pBg->getContentSize().height/2)),
				CCScaleTo::create(0.2f, 0.75f),
				NULL), 
				CCCallFuncN::create(this, callfuncN_selector(collectLayer::showSharedBtn)),
				NULL));
			break;
		}
	}
}

void collectLayer::chahuaGoBack()
{
	if (m_pCurBigChahua)
	{
		m_pCurBigChahua->removeAllChildren();
// 		CCNode* pNode = m_pCurBigChahua->getChildByTag(biankuang_tag);
// 		if (pNode)
// 		{
// 			pNode->removeFromParent();
// 		}	

		m_pCurBigChahua->runAction(CCSequence::create(
			CCSpawn::create(
			CCMoveTo::create(0.2f, m_ChahuaStartPoint),
			CCScaleTo::create(0.2f, 0.21f),
			NULL), 
			CCCallFuncN::create(this, callfuncN_selector(collectLayer::chahuaBackDone)),
			NULL));
	}
}


void collectLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

}

void collectLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

}

void collectLayer::showSharedBtn(CCNode* pNode)
{
	m_bIsShowBigChahua = true;

	CCMenuItemImage* pSharedBtn = CCMenuItemImage::create(ResManager::getManager()->getFilePathByLanguageType(g_buttonPath+"main_shoucang_anniu_fenxiang.png").c_str(), 
		ResManager::getManager()->getFilePathByLanguageType(g_buttonPath+"main_shoucang_anniu_fenxiang_select.png").c_str(),
		this,
		menu_selector(collectLayer::share));
	pSharedBtn->setVisible(false);
	
//	CCNode* pBiankuang = pNode->getChildByTag(biankuang_tag);
//	if (pBiankuang)
	{
		CCMenu* pSharedMenu = CCMenu::create();
		m_pCurBigChahua->addChild(pSharedMenu, 5, share_menu_tag);
		pSharedMenu->setPosition(CCPointZero);
		pSharedMenu->addChild(pSharedBtn);
		pSharedBtn->setScale(4/3);
		pSharedBtn->setPosition(ccp(m_pCurBigChahua->getContentSize().width/2, 0));
	}

}

void collectLayer::share(CCObject* pObj)
{
	CCLog("shared");
    string url = "https://itunes.apple.com/us/app/fang-kai-na-xing-xing/id869763433?l=zh&ls=1&mt=8";
	this->chahuaGoBack();
}

void collectLayer::chahuaBackDone(CCNode* pNode)
{
// 	CCNode* pBiankuang = pNode->getChildByTag(biankuang_tag);
// 	if (pBiankuang)
// 	{
// 		pBiankuang->removeFromParent();
// 	}
	m_pCurBigChahua = NULL;
	m_bIsShowBigChahua = false;
	m_pMenu->setEnabled(true);
	pNode->setZOrder(1);
}